#include "MusicPlayer.h"
#include <QDebug>

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

//Use SDL
#define USE_SDL 1

//挂起毫秒时间
void millisecondSleep(uint msecond)
{
#ifdef _WIN32
    //Windows
    Sleep(msecond);
#else
    //Linux...
    usleep(msecond*1000);
#endif
}

static bool logAudio = false;


QMutex uniqueThreadMutex;		//保证线程 Run 内初始化队列 和 销毁队列等操作不同时进行

/////////////////////////////////////////////////
/* 包队列相关操作 */

//static bool g_isQuit=false; //清空了

// 包队列初始化
void packet_queue_init(PacketQueue* q)
{
    memset(q, 0, sizeof(PacketQueue));
    q->last_pkt = nullptr;
    q->first_pkt = nullptr;
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

// 放入packet到队列中，不带头指针的队
/*
int packet_queue_put(PacketQueue*q, AVPacket *pkt)
{
    AVPacketList *pktl;
    if (av_dup_packet(pkt) < 0)
        return -1;

    pktl = static_cast<AVPacketList *>(av_malloc(sizeof(AVPacketList)));
    if (!pktl)
        return -1;

    pktl->pkt = *pkt;
    pktl->next = nullptr;

    SDL_LockMutex(q->mutex);

    if (!q->last_pkt) // 队列为空，新插入元素为第一个元素
        q->first_pkt = pktl;
    else // 插入队尾
        q->last_pkt->next = pktl;

    q->last_pkt = pktl;

    q->nb_packets++;
    q->size += pkt->size;

    SDL_CondSignal(q->cond);
    SDL_UnlockMutex(q->mutex);
    return 0;
}
*/
int packet_queue_put(PacketQueue *q, AVPacket *pkt) {
    AVPacketList *pktl;
    AVPacket tmp_pkt; // 创建临时 packet 用于引用

    // 初始化临时 packet
    if (av_packet_ref(&tmp_pkt, pkt) < 0) {
        return -1;
    }

    pktl = static_cast<AVPacketList *>(av_malloc(sizeof(AVPacketList)));
    if (!pktl) {
        av_packet_unref(&tmp_pkt); // 释放临时 packet
        return -1;
    }

    pktl->pkt = tmp_pkt; // 引用计数管理
    pktl->next = nullptr;

    // 队列操作部分保持不变...
    SDL_LockMutex(q->mutex);

    if (!q->last_pkt)
        q->first_pkt = pktl;
    else
        q->last_pkt->next = pktl;

    q->last_pkt = pktl;
    q->nb_packets++;
    q->size += tmp_pkt.size;

    SDL_CondSignal(q->cond);
    SDL_UnlockMutex(q->mutex);
    return 0;
}

// 从队列中取出packet*
int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block, bool *forceQuitFlag)
{
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    int tryTimes = 0;
    const int maxTryTimes = 100000; // 设置合理阈值

    for (;;)
    {
         if (forceQuitFlag && *forceQuitFlag) //由外界通知退出
        {
            SDL_UnlockMutex(q->mutex);  //解锁退出
            return -1;
        }
        pkt1 = q->first_pkt;
        if (pkt1) {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt) {
                q->last_pkt = nullptr;
            }
            q->nb_packets--;
            q->size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else
        {
            tryTimes++;
            if (tryTimes > maxTryTimes) // 超时退出
            {
                qDebug() << "Timeout waiting for audio data";
                SDL_UnlockMutex(q->mutex);
                return -1;
            }
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

void packet_queue_flush(PacketQueue *q)
{
    SDL_LockMutex(q->mutex);
    AVPacketList *pkt, *pkt1;
    for(pkt = q->first_pkt; pkt != nullptr; pkt = pkt1)
    {
        pkt1 = pkt->next;
        //if(pkt1->pkt.data != (uint8_t *)"FLUSH")
        //{
        //}
        av_packet_unref(&pkt->pkt);
        av_freep(&pkt);

    }
    q->last_pkt = nullptr;
    q->first_pkt = nullptr;
    q->nb_packets = 0;
    q->size = 0;
    SDL_UnlockMutex(q->mutex);
}

void destroy_queue_context(PacketQueue* q)
{
    SDL_DestroyMutex(q->mutex);
    SDL_DestroyCond(q->cond);
}

//////////////////////////////////////////////

int PlayThread::audio_decode_frame(mediaState* MS, uint8_t* audio_buf)
{
    static AVFrame *pframe=nullptr;        //一帧
    static SwrContext*pSwr_ctx=nullptr;    //转换
    AVPacket packet;                    //包

    int decodeLen=0;
    int got_frame = 0;
    int audio_pkt_size=0;
    uint8_t* audio_pkt_data=nullptr;

    if(pframe)
       av_frame_free(&pframe);
    pframe=av_frame_alloc();

    while (true)
    {
        if (packet_queue_get(&MS->audioq, &packet, 0,&m_forceQuit) < 0)
        {
            qDebug() << "Failed to get packet from queue";
            return -1;
        }
        audio_pkt_data =packet.data;
        audio_pkt_size = packet.size;

        if (packet.pts != AV_NOPTS_VALUE)
        {

            //方式一:
            //packet->pts 时间基于  AVStream->time_base units
            //外部时间基于 1/AV_TIME_BASE 即 1微秒
            //使用 av_rescale_q 转换得到 微秒时间
            AVRational aVRational = { 1, AV_TIME_BASE };
            int64_t res = av_rescale_q(packet.pts, pFormatCtx->streams[audioStream]->time_base, aVRational);


            static int64_t lastRes = 0;		//用于记录最后一次的时间
            static int64_t tryTimes = 0;

            if (lastRes != res)				//与上次时间不同时，发送位置改变信号
            {
                MS->audio_clock = res * 1.0 / 1000;
                lastRes = res;
                tryTimes = 0;
            }
            else
            {
                tryTimes++;
                if (tryTimes >= 1e8)
                {
                    qDebug() << "no data in list for 1e8 times access";
                    AGStatus = AGS_FINISH;
                }
            }

            //方式二：
            //MS->audio_clock = (double)av_q2d(MS->aStream->time_base) * (double)packet.pts;
            //MS->audio_clock *= 1000;

            if(logAudio)
            {
                logAudio = false;
                qDebug() << "to " <<MS->audio_clock ;
            }
        }

        while (audio_pkt_size > 0)
        {
            // decodeLen = avcodec_decode_audio4(MS->acct, pframe, &got_frame, &packet);
            // if (decodeLen < 0) // 出错，跳过
            //     break;
            int ret;

            // 发送待解码的压缩数据（packet）给解码器
            ret = avcodec_send_packet(MS->acct, &packet);
            if (ret < 0) {
                // 发送失败，跳过或处理错误
                break;
            }

            // 接收解码后的 PCM 数据帧（frame）
            ret = avcodec_receive_frame(MS->acct, pframe);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                // 暂时无数据或已解码完成，不算错误
                continue;
            } else if (ret < 0) {
                // 解码出错
                break;
            }

            // 此时 pframe 中就是有效的音频数据
            got_frame = 1;


            audio_pkt_data += decodeLen;
            audio_pkt_size -= decodeLen;

            if (pframe->channels > 0 && pframe->channel_layout == 0)
                pframe->channel_layout = av_get_default_channel_layout(pframe->channels);
            else if (pframe->channels == 0 && pframe->channel_layout > 0)
                pframe->channels = av_get_channel_layout_nb_channels(pframe->channel_layout);


            if (pSwr_ctx)
                swr_free(&pSwr_ctx);
            pSwr_ctx = swr_alloc_set_opts(nullptr, MS->wanted_frame->channel_layout,
                                         (AVSampleFormat)MS->wanted_frame->format,
                                         MS->wanted_frame->sample_rate,
                                         pframe->channel_layout,
                                         static_cast<AVSampleFormat>(pframe->format),
                                         pframe->sample_rate, 0, nullptr);
            swr_init(pSwr_ctx);


            int dst_nb_samples = av_rescale_rnd(swr_get_delay(pSwr_ctx, pframe->sample_rate) + pframe->nb_samples, pframe->sample_rate, pframe->sample_rate, AVRounding(1));
            int len2 = swr_convert(pSwr_ctx, &audio_buf, dst_nb_samples,const_cast<const uint8_t **>(pframe->data), pframe->nb_samples);
            if (len2 < 0)
                break;

            av_packet_unref(&packet);
            return MS->wanted_frame->channels * len2 * av_get_bytes_per_sample(static_cast<AVSampleFormat>(MS->wanted_frame->format));
        }
    }
    return -1;
}


//////////////////////////////////////////////


void PlayThread::run()
{
    AGSStatusMutex.lock();		//保证	AGSStatus 状态 某些访问操作 的原子性
    uniqueThreadMutex.lock();   //保证线程不能同时进来同时使用播放设备，实现 SDL 播放设备互斥访问;

    ResetToInitAll();           //重置以初始化所有状态

    if(initDeviceAndFfmpegContext())    //尝试初始化播放设备 和 ffmpeg 上下文
    {
        bIsDeviceInit = true;
        emit durationChanged(pFormatCtx->duration);
    }
    else
    {
        //释放所有分配的内存，解锁
        isEndByForce = false;
        ReleaseAll();
        uniqueThreadMutex.unlock();
        AGSStatusMutex.unlock();
        return;
    }

    playDevice();               //初始化后需要启用设备

    generateAudioDataLoop();    //数据生产循环

    //释放所有分配的内存，解锁
    ReleaseAll();
    uniqueThreadMutex.unlock();
}

void PlayThread::setAGStatus(AudioGenStatus status)
{
    AGSStatusMutex.lock();		//保证	AGSStatus 状态 某些访问操作 的原子性
    AGStatus = status;
    if(AGStatus == AGS_FINISH)
    {
        isEndByForce = true;
    }
    AGSStatusMutex.unlock();
}

int PlayThread::getVolume() const
{
    return m_MS.volume;
}

void PlayThread::setVolume(int value)
{
    m_MS.volume = static_cast<uint8_t>(value);
}

int PlayThread::getMsDuration() const
//获得毫秒为度量的总长度
{
    if (pFormatCtx == nullptr || !bIsDeviceInit)
        return 0;
    else
        return pFormatCtx->duration / 1000;
}

int  PlayThread::getCurrentTime() const
{
    return static_cast<int>(m_MS.audio_clock);
}

bool PlayThread::getIsDeviceInit() const
{
    return bIsDeviceInit;
}//实现互斥访问 isDeviceInit 的接口

void PlayThread::setMusicPath(QString path)
{
    musicPath = path;
}

//获得音频产生方式状态
AudioGenStatus PlayThread::getAGStatus() const {
    return AGStatus;
}

void  PlayThread::seekToPos(quint64 pos)
{
    millisecondToSeek = pos;
    AGStatus = AGS_SEEK;
}

//尝试初始化播放设备 和 ffmpeg 上下文
bool PlayThread::initDeviceAndFfmpegContext()
{
    char url[1024];
    index = 0;

    //char url[]="xiaoqingge.mp3";              //test
    //char url[]="小情歌.mp3";
    //char url[]="Janice Morning.mp3";

    //url = "A - Something's Going On.mp3";
    //char url[] ="A - Starbucks.mp3";
    //char url[] ="A - The Distance.mp3";
    //char url[] ="Acreix - Visions.mp3";
    //A - Starbucks.wav

    strcpy(url, musicPath.toUtf8());            //播放路径

    avformat_network_init();
    m_MS.fct = pFormatCtx = avformat_alloc_context();
    //Open
    if(avformat_open_input(&pFormatCtx,url,nullptr,nullptr)!=0){
        qDebug() << "Couldn't open input stream.\n";
        emit errorOccur(1,tr("无法打开媒体输入流"));//"Couldn't open input stream."
        return false;
    }
    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx,nullptr)<0){
        qDebug() << "Couldn't find stream information.\n";
        emit errorOccur(2,tr("媒体输入流中找不到任何可播放数据")); //"Couldn't find stream information."
        return false;
    }

    //读取专辑等头文字信息
    //读取metadata中所有的tag
    AVDictionaryEntry *tag = nullptr;
//    while ((tag = av_dict_get(pFormatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))){
//       printf("[Tag:%s , Value: %s]\n", tag->key, tag->value);
//    }

    // 尝试获取 album
    tag = av_dict_get(pFormatCtx->metadata, "album", nullptr, AV_DICT_MATCH_CASE);
    if (tag) {
        album = tag->value;  // 找到时赋值
    } else {
        album = "";  // 未找到时设为空字符串
    }
    emit albumFound(album);  // 无论是否找到，都发射信号

    // 尝试获取 title
    tag = av_dict_get(pFormatCtx->metadata, "title", nullptr, AV_DICT_MATCH_CASE);
    if (tag) {
        title = tag->value;  // 找到时赋值
    } else {
        title = "";  // 未找到时设为空字符串
    }
    emit titleFound(title);  // 无论是否找到，都发射信号

    // 尝试获取 artist
    tag = av_dict_get(pFormatCtx->metadata, "artist", nullptr, AV_DICT_MATCH_CASE);
    if (tag) {
        artist = tag->value;  // 找到时赋值
    } else {
        artist = "";  // 未找到时设为空字符串
    }
    emit artistFound(artist);  // 无论是否找到，都发射信号

    //读取专辑图片
    picture = QPixmap();
    if (strcmp(pFormatCtx->iformat->name, "wav") == 0 || strcmp(pFormatCtx->iformat->name, "mp3") == 0)
    {
        //读取音频的专辑图片
        // read the format headers
        if (pFormatCtx->iformat->read_header(pFormatCtx) < 0) {
            qDebug() << "No header format";
            //return;
        }

        for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
            if (pFormatCtx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
                AVPacket pkt = pFormatCtx->streams[i]->attached_pic;
                //使用QImage读取完整图片数据（注意，图片数据是为解析的文件数据，需要用QImage::fromdata来解析读取）
                QImage img = QImage::fromData((uchar*)pkt.data, pkt.size);
                picture = QPixmap::fromImage(img);
                break;
            }
        }emit pictureFound(picture);  // 无论是否找到图片，都发射信号
    } else {
        // 非 WAV 或 MP3 格式，也发射信号，传递空 QPixmap
        emit pictureFound(picture);
    }

    // Dump valid information onto standard error
    av_dump_format(pFormatCtx, 0, url, false);

    // Find the first audio stream
    /*
    audioStream=-1;
    for(unsigned int i=0; i < pFormatCtx->nb_streams; i++)
    {
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
            m_MS.aStream = pFormatCtx->streams[i];
            audioStream = i;
            break;
        }
    }

    videoStream = -1;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_MS.vStream = pFormatCtx->streams[i];
            videoStream = i;
            break;
        }
        */
    audioStream = -1;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            m_MS.aStream = pFormatCtx->streams[i];
            audioStream = i;

            // 创建 AVCodecContext 并填充
            const AVCodec *audioCodec = avcodec_find_decoder(m_MS.aStream->codecpar->codec_id);
            pCodecCtx = avcodec_alloc_context3(audioCodec);
            avcodec_parameters_to_context(pCodecCtx, m_MS.aStream->codecpar);
            avcodec_open2(pCodecCtx, audioCodec, nullptr);

            break;
        }
    }

    videoStream = -1;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_MS.vStream = pFormatCtx->streams[i];
            videoStream = i;

            // 创建 AVCodecContext 并填充
            const AVCodec *videoCodec = avcodec_find_decoder(m_MS.vStream->codecpar->codec_id);
            pCodecCtx = avcodec_alloc_context3(videoCodec);
            avcodec_parameters_to_context(pCodecCtx, m_MS.vStream->codecpar);
            avcodec_open2(pCodecCtx, videoCodec, nullptr);

            break;
        }
    }

    if(audioStream==-1){
        qDebug() << "Didn't find a audio stream.\n";
        emit errorOccur(3,tr("媒体输入流中找不到任何音频数据"));//"Didn't find an audio stream."
        return false;
    }


    // Get a pointer to the codec context for the audio stream
    //m_MS.acct =  pCodecCtx=pFormatCtx->streams[audioStream]->codec;
    const AVCodec *codec = avcodec_find_decoder(pFormatCtx->streams[audioStream]->codecpar->codec_id);
    avcodec_open2(m_MS.acct = pCodecCtx = avcodec_alloc_context3(codec), codec, nullptr);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[audioStream]->codecpar);


    // Find the decoder for the audio stream
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==nullptr){
        qDebug() << "Codec not found.\n";
        emit errorOccur(4,"ffmpeg模块无法找到可用解码器"); //Codec not found
        return false;
    }

    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec,nullptr)<0){
        qDebug() << "Could not open codec.\n";
        emit errorOccur(5,"ffmpeg模块无法启用解码器"); //"Could not open codec."
        return false;
    }

    //Out Audio Param
    out_channel_layout=AV_CH_LAYOUT_STEREO;
    //nb_samples: AAC-1024 MP3-1152
    out_nb_samples=pCodecCtx->frame_size;
    out_sample_fmt=AV_SAMPLE_FMT_S16;
    out_sample_rate=44100;
    out_channels=av_get_channel_layout_nb_channels(out_channel_layout);
    //Out Buffer Size
    out_buffer_size=av_samples_get_buffer_size(nullptr,out_channels ,out_nb_samples,out_sample_fmt, 1);

    out_buffer=static_cast<uint8_t *>(av_malloc(MAX_AUDIO_FRAME_SIZE * 2));

//SDL------------------
#if USE_SDL
    //Init
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        const char* errorString = SDL_GetError();
        qDebug() << "Could not initialize SDL - %s\n", errorString;
        emit errorOccur(6,QString(tr("无法初始化播放设备模块 SDL - %s.")).arg(errorString)); //QString("Could not initialize SDL - %s.").arg(errorString)
        return false;
    }

    //SDL_AudioSpec
    wanted_spec.freq = out_sample_rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = out_channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = out_nb_samples;
    wanted_spec.size = 1024;
    wanted_spec.callback = fillAudio;
    wanted_spec.userdata =  &m_MS;

    if (SDL_OpenAudio(&wanted_spec, nullptr)<0){
        qDebug() << "Can't open audio.\n";
        emit errorOccur(7,tr("播放设备模块 SDL 无法打开指定音频数据"));//"Can't open audio."
        return false;
    }
#endif

    //FIX:Some Codec's Context Information is missing
    in_channel_layout=av_get_default_channel_layout(pCodecCtx->channels);
    //Swr

    m_MS.wanted_frame=av_frame_alloc();
    m_MS.wanted_frame->channel_layout = out_channel_layout;
    m_MS.wanted_frame->format = out_sample_fmt;
    m_MS.wanted_frame->sample_rate = out_sample_rate;
    m_MS.wanted_frame->channels = out_channels;

    m_MS.playThread = this;

    return true;
}

void PlayThread::playDevice()
{
    //Play
    SDL_PauseAudio(0);

    emit audioPlay();
    //emit volumeChanged(m_MS.volume);
}

void PlayThread::pauseDevice()
{
    //pause
    SDL_PauseAudio(1);

    emit audioPause();
}

void PlayThread::generateAudioDataLoop()
{
    AGStatus = AGS_PLAYING;
    AGSStatusMutex.unlock();		//从线程启动到 这里设置为 AGS_PLAYING 之间，与 setAGStatus 中的逻辑 应该是互斥的，
                                    //否则可能启动了线程，还没有在这里设置 AGS_PLAYING， 却同时被想要结束线程者先设置 AGS_FINISH
                                    //导致外界设置的 AGS_FINISH 被 AGS_PLAYING 替代而导致无法预期的逻辑

    //[注:由于ffmpeg版本原因，ffmpeg 4.0.1 版本，在重头播放1秒多时有噪音，经试验，调用 av_seek_frame 后则会间接消除该噪音]
    //[若 后面更改 ffmpeg版本，可尝试去掉 seekToPos(10); 看看会不会在 1秒多 出现噪音]
    seekToPos(10);

    AVPacket packet;

    AVPacket *ppacket = nullptr;  //分配用于转换的数据包(输入)
    AVFrame	*pFrame = nullptr;    //分配用于转换的数据包(解码输出)

    while(!m_forceQuit)
    {
        switch (AGStatus) {
        case AGS_PLAYING:
        {
            SDL_Delay(10);  //此句很重要，否则CPU占用会很高

            int result=av_read_frame(m_MS.fct, &packet);
            if(0==result)
            {
                 if (packet.stream_index == audioStream)
                     packet_queue_put(&m_MS.audioq, &packet);//添加到队列中去
                 else
                     av_packet_unref(&packet);
            }
            else
            {
                int curTime = getCurrentTime()*0.001;
                int durat = getDuration()*0.000001;
                if(curTime >= durat)//播放到尾端
                {
                    qDebug() << "ending reached";
                    AGStatus = AGS_FINISH;
                    isEndByForce = false;
                    break;
                }
            }
        }
            break;

        case AGS_SEEK:
        {
               AVRational aVRational = {1, 1000};
               int64_t res = av_rescale_q(millisecondToSeek ,aVRational,pFormatCtx->streams[audioStream]->time_base);

               SDL_PauseAudio(1);
               //block here
               if (av_seek_frame(m_MS.fct, audioStream, res, AVSEEK_FLAG_ANY) < 0)
               {
                   //printf("Error to seek audio frame.\n");
                   qDebug()<<"seek error";
               }
               else
               {
                   logAudio = true;
                   qDebug()<<"seek successful  " << "  from " << m_MS.audio_clock << " to :";
                   if (audioStream!=-1) //audio
                   {
                    avcodec_flush_buffers(pCodecCtx);

                       packet_queue_flush(&m_MS.audioq); //清除队列
                   }
               }
               SDL_PauseAudio(0);

                AGStatus = AGS_PLAYING;
        }
            break;

        case AGS_FINISH:
            m_forceQuit = true;

            break;
        default:
            break;
        }
    }

    if(ppacket)             //释放 packet 本身
        av_free(ppacket);
    if(pFrame)              //释放 frame 本身
       av_frame_free(&pFrame);
}

void PlayThread::clearContextAndCloseDevice() {
    // 释放 FFmpeg 资源
    if (pCodecCtx) {
        avcodec_free_context(&pCodecCtx); // 正确释放编解码器上下文
        pCodecCtx = nullptr;
    }

    if (pFormatCtx) {
        avformat_close_input(&pFormatCtx); // 自动释放格式上下文内存
        pFormatCtx = nullptr;              // 显式置空防止野指针
    }

    av_free(out_buffer);      // 释放音频缓冲区
    out_buffer = nullptr;

    packet_queue_flush(&m_MS.audioq); // 清空音频包队列

    // 释放 SDL 资源
#if USE_SDL
    SDL_CloseAudio();                   // 关闭音频设备
    SDL_QuitSubSystem(SDL_INIT_AUDIO);  // 仅关闭音频子系统
#endif
}

//获得设备状态
SDL_AudioStatus PlayThread::GetDeviceStatus()
{
    return SDL_GetAudioStatus();
}

//重置以初始化所有状态
void PlayThread::ResetToInitAll()
{
    //g_isQuit= false;                        //退出标志重置
    m_forceQuit = false; // 初始化私有退出标志

    m_MS.clear();                       //重设音频相关的上下文状态
    packet_queue_init(&m_MS.audioq);    //初始化队列

    //重置所有指针
    pFormatCtx = nullptr;
    pCodecCtx = nullptr;
    pCodec = nullptr;
    out_buffer = nullptr;
    au_convert_ctx = nullptr;
}

//释放所有可能分配的内存
void PlayThread::ReleaseAll()
{
    emit audioFinish(isEndByForce);		 //音频播放结束信号
    emit durationChanged(0); //播放结束，总长重置为 0

    clearContextAndCloseDevice();           //重置播放器上下文，并关闭设备

    destroy_queue_context(&m_MS.audioq);

    bIsDeviceInit = false;
}

/* The audio function callback takes the following parameters:
 * stream: A pointer to the audio buffer to be filled
 * len: The length (in bytes) of the audio buffer
*/
void  PlayThread::fillAudio(void *udata,Uint8 *stream,int len){

   mediaState* MS = static_cast<mediaState *>(udata);
   memset(stream, 0, len);
   static int audio_buf_pos=0;
   static int audio_buf_size=0;
   //使用静态变量的原因是audio_buf_size一次数据长度为4608,mp3哈，回调的len又太小，不能让它每执行audio_callback函数就来执行audio_decode_frame解码函数

   while(len>0) //要读len这么长的数据长能退出
   {
       uint8_t audio_buff[MAX_AUDIO_FRAME_SIZE];
       if(audio_buf_pos>=audio_buf_size)//如果完成
       {
           audio_buf_size = MS->playThread->audio_decode_frame(MS, audio_buff);
           if (audio_buf_size < 0)
               return;//-1 为退出标志
           audio_buf_pos=0;
       }

      int audio_len= audio_buf_size-audio_buf_pos;
      if(audio_len>len)
          audio_len=len;


       SDL_MixAudio(stream, audio_buff+audio_buf_pos, audio_len, MS->volume);
       len-=audio_len;
       audio_buf_pos+=audio_len;
       stream+=audio_len;
    }
}
//-----------------

//MusicPlayer::MusicPlayer(QObject* parent):QObject(parent),m_volume(128)
MusicPlayer::MusicPlayer(QObject* parent):QObject(parent),m_volume(20)
{
    playThread = new PlayThread(this);
    connect(playThread, &PlayThread::audioPlay,[=](){emit audioPlay();});
    connect(playThread, &PlayThread::audioPause,[=](){emit audioPause();});
    connect(playThread, &PlayThread::audioFinish,[=](bool isEndByForce){
        bIsLock = true;
        audioFinishedToThreadExitMutex.lock();
        emit audioFinish(isEndByForce);});
    connect(playThread, &PlayThread::finished,[=](){
        if(bIsLock)
        {
            audioFinishedToThreadExitMutex.unlock();
            bIsLock = false;
        }
    });
    connect(playThread, &PlayThread::volumeChanged,[=](uint8_t volume){
        emit volumeChanged(volume);}
    );
    connect(playThread, &PlayThread::durationChanged,[=](qint64 duration){emit durationChanged(duration/1000);});

    connect(playThread, &PlayThread::albumFound,[=](const QString &album){
        m_album=album; emit albumFound(album);}
    );
    connect(playThread, &PlayThread::artistFound,[=](const QString &artist){m_artist=artist; emit artistFound(artist);});
    connect(playThread, &PlayThread::titleFound,[=](const QString &title){m_title=title; emit titleFound(title);});
    connect(playThread, &PlayThread::pictureFound,[=](const QPixmap &picture){m_picture=picture; emit pictureFound(picture);});

    //发送错误信号除去弹窗提示，由于“Widgets must be created in the GUI thread” 窗口必须在UI线程创建，
    // 所以发送异步信号 QueuedConnection；经过实际运行结果来看，还需要等待 slot 执行完再返回，所以用 BlockingQueuedConnection
    connect(playThread, SIGNAL(errorOccur(int,QString)), this, SLOT(onErrorOccurs(int,QString)),
            Qt::BlockingQueuedConnection);

    //setNotifyInterval(500);
    m_interval = 100;
    m_positionUpdateTimer.setInterval(m_interval);
    connect(&m_positionUpdateTimer,SIGNAL(timeout()),this, SLOT(sendPosChangedSignal() ));

    m_position = 0;

    bIsLock = false;
}

MusicPlayer::~MusicPlayer() {

    playThread->setAGStatus(AGS_FINISH); //置结束位，并等待线程退出才结束，否则 playThread 的释放会导致访问异常

    while (playThread->isRunning())
        millisecondSleep(5);
}

void MusicPlayer::setMedia(const QString &path)
{
    musicPath = path;
    playThread->setMusicPath(path);
    emit metaDataChanged();
}

QString MusicPlayer::getMusicPath()
{
    return musicPath;
}

//音乐文件信息
QString MusicPlayer::getTitle()
{
    return m_title;
}

QString MusicPlayer::getArtist()
{
    return m_artist;
}

QString MusicPlayer::getAlbum()
{
    return m_album;
}

QPixmap MusicPlayer::getPicture()
{
    return m_picture;
}

//stop() 并 play();
void MusicPlayer::reload()
{
    stop();

    while(playThread->isRunning())
        millisecondSleep(10); //等待结束


    play();

    if(!m_positionUpdateTimer.isActive())
        m_positionUpdateTimer.start();
}

//播放控制
void MusicPlayer::play()
{
    playThread->setVolume(m_volume);  //初始化声音值

    audioFinishedToThreadExitMutex.lock();
    if (!playThread->getIsDeviceInit())
    {
        playThread->start(QThread::Priority::HighestPriority);
    }
    else
        playThread->playDevice();
    audioFinishedToThreadExitMutex.unlock();

    if(!m_positionUpdateTimer.isActive())
        m_positionUpdateTimer.start();

}

void MusicPlayer::pause() const {
    if(state() == PlayingState)
        playThread->pauseDevice();

}

void MusicPlayer::stop()
{
    playThread->setAGStatus( AGS_FINISH );
    playThread->requestForceQuit(); // 通过公有接口设置退出标志

    if(m_positionUpdateTimer.isActive())
    {
        m_positionUpdateTimer.stop();
        emit positionChanged(0);       //停止了timer ，自己发送0时间
    }

    //停止音乐必须保证线程真的退出(否则 playThread->bIsDeviceInit 可能判断成立，而实际线程还没退出，导致下一次 播放 playThread->playDevice() 没能及时起作用)
    while(playThread->isRunning())
        millisecondSleep(10); //等待结束

    if(bIsLock)
    {
        audioFinishedToThreadExitMutex.unlock();
        bIsLock = false;
    }
    //emit sig_playThreadFinished();
}

//跳到时间点播放（单位 毫秒）
void MusicPlayer::seek(quint64 pos) const {
    //先获得总长
    quint64 total = duration();
    if(pos > total)
    {
        pos = total;
    }

    playThread->seekToPos(pos);
}

//往后跳（单位 毫秒）
void MusicPlayer::forwordSeek(quint64 step) const {
    seek(m_position + step);
}

//往回跳（单位 毫秒）
void MusicPlayer::backwardSeek(quint64 step) const {
    seek(step > m_position ? 0: m_position - step);
}

 //音量大小范围 0-100
void MusicPlayer::setVolume(int volume)
{
    //if(volume > 128)
    //   volume = 128;
    if(volume > 100)
        volume = 100;

    if(volume < 0)
        volume = 0;

    m_volume = volume;

    playThread->setVolume(volume);
}

int MusicPlayer::getVolume() const {
    return playThread->getVolume();
}

//获得当总时长（单位 毫秒）
quint64 MusicPlayer::duration() const {
    return playThread->getMsDuration();
}

//获得当总位置（单位 毫秒）
quint64 MusicPlayer::position() const {
    return m_position;
}

//设置通知间隔（歌曲位置进度）
void MusicPlayer::setNotifyInterval(int msec)
{
    m_interval = msec;
    m_positionUpdateTimer.setInterval(m_interval);
}

void MusicPlayer::sendPosChangedSignal()
{
    m_position = playThread->getCurrentTime();
    emit positionChanged(m_position);
}

void MusicPlayer::onErrorOccurs(int code, const QString &msg)
{
    emit errorOccur(code, msg);
}

MusicPlayer::Status MusicPlayer::state() const {
    SDL_AudioStatus deviceStatus = playThread->GetDeviceStatus();
    switch (deviceStatus) {
    case SDL_AUDIO_PLAYING:
        return PlayingState;
        break;
    case SDL_AUDIO_PAUSED:
        return PausedState;
        break;
    case SDL_AUDIO_STOPPED:
    default:

        if (playThread->isRunning())  //实际上，deviceStatus 并不能完全表示播放线程已经结束
            return PlayingState;	  //还需要播放线程正在退出，这里才认为是播放结束
        else
            return StoppedState;

        break;
    }
}

