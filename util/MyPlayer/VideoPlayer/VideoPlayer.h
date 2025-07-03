/*
  VideoPlayer - 多媒体播放器类
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QTimer>
#include <thread>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/time.h>
    #include <libavutil/pixfmt.h>
    #include <libavutil/display.h>
    #include <libavutil/avstring.h>
    #include <libavutil/opt.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
    #include <libavutil/imgutils.h>
    #include <libavfilter/avfilter.h>
    #include <libavfilter/buffersink.h>
    #include <libavfilter/buffersrc.h>
}

#define CONFIG_AVFILTER 1 ///< 启用滤镜，用于旋转带角度的视频
#define SDL_AUDIO_BUFFER_SIZE 1024 ///< SDL 音频缓冲区大小
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000 ///< 1 秒 48kHz 32 位音频最大帧大小
#define MAX_AUDIO_SIZE (50 * 20) ///< 最大音频队列大小
#define MAX_VIDEO_SIZE (25 * 20) ///< 最大视频队列大小
#define FLUSH_DATA "FLUSH" ///< 用于清空解码器的数据标志

#if defined(MYPLAYER_LIBRARY)
#define MYPLAYER_EXPORT Q_DECL_EXPORT
#else
#define MYPLAYER_EXPORT Q_DECL_IMPORT
#endif
#include "util/util.h"
#include "util/thread.h"
#include "PcmPlayer/PcmPlayer.h"
#include "frame/AudioFrame/AACFrame.h"
#include "frame/VideoFrame/VideoFrame.h"

/** @class VideoPlayer
 *  @brief 多媒体播放器类
 *
 *  实现音视频文件的播放功能，使用 FFmpeg 进行解码，支持暂停、跳转、音量控制等功能。
 *  通过回调函数与界面交互，基于 C++11 实现，便于跨平台移植。
 */
class MYPLAYER_EXPORT VideoPlayer : public QObject, public Util::Thread
{
    Q_OBJECT
public:
    /** @enum State
     *  @brief 播放器状态
     */
    enum State
    {
        Playing = 0, ///< 正在播放
        Pause,       ///< 暂停
        Stop,        ///< 停止
        ReadError,   ///< 读取错误
    };

    /** @class EventHandle
     *  @brief 事件处理接口
     *
     *  定义播放器与界面交互的回调函数。
     */
    class EventHandle
    {
    public:
        /** @brief 打开视频文件失败回调
         *
         *  @param code 错误码
         */
        virtual void onOpenVideoFileFailed(const int &code) = 0;

        /** @brief 打开 SDL 失败回调
         *
         *  @param code 错误码
         */
        virtual void onOpenSdlFailed(const int &code) = 0;

        /** @brief 视频时长变更回调
         *
         *  @param uSec 总时长（微秒）
         */
        virtual void onTotalTimeChanged(const int64_t &uSec) = 0;

        /** @brief 播放器状态变更回调
         *
         *  @param state 播放器状态
         *  @param hasVideo 是否包含视频流
         *  @param hasAudio 是否包含音频流
         */
        virtual void onPlayerStateChanged(const VideoPlayer::State &state, const bool &hasVideo, const bool &hasAudio) = 0;

        /** @brief 显示视频帧回调
         *
         *  @param videoFrame 视频帧数据
         */
        virtual void onDisplayVideo(VideoRawFramePtr videoFrame) = 0;

        /** @brief 视频编码帧回调
         *
         *  @param video_frame 编码视频帧
         */
        virtual void onVideoBuffer(VideoEncodedFramePtr video_frame) {}

        /** @brief 音频编码帧回调
         *
         *  @param audio_frame AAC 音频帧
         */
        virtual void onAudioBuffer(AACFramePtr audio_frame) {}

        /** @brief PCM 音频帧回调
         *
         *  @param audio_frame PCM 音频帧
         */
        virtual void onAudioBuffer(PCMFramePtr audio_frame) {}
    };

    /** @brief 构造函数
     *
     *  @param parent 父对象
     */
    explicit VideoPlayer(QObject *parent = nullptr);

    /** @brief 初始化播放器
     *
     *  必须调用一次以初始化 FFmpeg 和网络支持。
     *
     *  @return 初始化是否成功
     */
    static bool initPlayer();

    /** @brief 设置事件处理回调
     *
     *  @param handle 事件处理接口指针
     */
    void setEventHandle(VideoPlayer::EventHandle *handle) { m_event_handle = handle; }

    /** @brief 开始播放
     *
     *  @param filePath 文件路径
     *  @return 是否成功开始播放
     */
    bool startPlay(const std::string &filePath);

    /** @brief 重新播放
     *
     *  @param isWait 是否等待线程结束
     *  @return 是否成功重新播放
     */
    bool replay(bool isWait = false);

    /** @brief 继续播放
     *
     *  用于暂停后恢复播放。
     *
     *  @return 是否成功继续播放
     */
    bool play();

    /** @brief 暂停播放
     *
     *  @return 是否成功暂停
     */
    bool pause();

    /** @brief 停止播放
     *
     *  @param isWait 是否等待线程结束
     *  @return 是否成功停止
     */
    bool stop(bool isWait = true);

    /** @brief 跳转到指定位置
     *
     *  @param pos 跳转位置（微秒）
     */
    void seek(int64_t pos);

    /** @brief 设置播放能力
     *
     *  配置视频解码、编码视频回调、音频播放和编码音频回调。
     *
     *  @param video_decode 是否启用视频解码
     *  @param encoded_video_callback 是否启用编码视频回调
     *  @param audio_play 是否启用音频播放
     *  @param encoded_audio_callback 是否启用编码音频回调
     */
    void setAbility(bool video_decode, bool encoded_video_callback, bool audio_play, bool encoded_audio_callback);

    /** @brief 设置静音
     *
     *  @param isMute 是否静音
     */
    void setMute(bool isMute);

    /** @brief 设置音量
     *
     *  @param value 音量值（0~1，超过1表示放大）
     */
    void setVolume(float value);

    /** @brief 获取音量
     *
     *  @return 当前音量值
     */
    float getVolume() { return mVolume; }

    /** @brief 设置播放速度
     *
     *  @param value 播放速度值（0.5~1.5，超过1表示放大）
     */
    void setSpeed(const float& value);

    /** @brief 获取播放速度
     *
     *  @return 当前播放速度值
     */
    float getSpeed() { return m_speed; }

    /** @brief 获取总时长
     *
     *  @return 总时长（微秒）
     */
    int64_t getTotalTime();

    /** @brief 获取当前播放时间
     *
     *  @return 当前时间（秒）
     */
    int64_t getCurrentTime();

    /** @brief 获取音乐文件路径
     *
     *  @return 音乐文件路径
     */
    QString getMusicPath() const { return m_file_path.c_str(); }

    /** @brief 获取播放器状态
     *
     *  @return 当前播放器状态
     */
    State state() const { return m_state; }

signals:
    /** @brief 专辑信息变更信号
     *
     *  @param album 专辑名称
     */
    void albumFound(QString);

    /** @brief 艺术家信息变更信号
     *
     *  @param artist 艺术家名称
     */
    void artistFound(QString);

    /** @brief 标题信息变更信号
     *
     *  @param title 标题
     */
    void titleFound(QString);

    /** @brief 专辑图片变更信号
     *
     *  @param picture 专辑图片
     */
    void pictureFound(QPixmap);

    /** @brief 播放位置变更信号
     *
     *  @param position 当前播放位置（毫秒）
     */
    void positionChanged(int position);

    /** @brief 总时长变更信号
     *
     *  @param duration 总时长（毫秒）
     */
    void durationChanged(int duration);

    /** @brief 播放开始信号
     */
    void audioPlay();

    /** @brief 播放暂停信号
     */
    void audioPause();

    /** @brief 播放结束信号
     */
    void audioFinish();

    /** @brief 错误发生信号
     *
     *  @param msg 错误信息
     */
    void errorOccur(QString msg);

protected:
    /** @brief 读取视频文件线程函数
     *
     *  重载 Thread 类的 run 方法，负责读取音视频数据。
     */
    void run();

    /** @brief 视频解码线程函数
     */
    void decodeVideoThread();

    /** @brief 音频解码线程函数
     */
    void decodeAudioThread();


    // static void sdlAudioCallBackFunc(void *userdata, Uint8 *stream, int len);
    // void sdlAudioCallBack(Uint8 *stream, int len);
    // int decodeAudioFrame(bool isBlock = false);

private:
    /** @brief 获取音频时钟
     *
     *  @return 音频时钟（毫秒）
     */
    int64_t getAudioClock();

    /** @brief 将视频数据包加入队列
     *
     *  @param pkt 视频数据包
     *  @return 是否成功加入队列
     */
    bool inputVideoQuene(const AVPacket &pkt);

    /** @brief 清空视频队列
     */
    void clearVideoQuene();

    /** @brief 将音频数据包加入队列
     *
     *  @param pkt 音频数据包
     *  @return 是否成功加入队列
     */
    bool inputAudioQuene(const AVPacket &pkt);

    /** @brief 清空音频队列
     */
    void clearAudioQuene();

    /** @brief 配置滤镜图
     *
     *  @param graph 滤镜图
     *  @param filtergraph 滤镜描述
     *  @param source_ctx 源滤镜上下文
     *  @param sink_ctx 输出滤镜上下文
     *  @return 配置结果
     */
    int configure_filtergraph(AVFilterGraph *graph, const char *filtergraph, AVFilterContext *source_ctx, AVFilterContext *sink_ctx);

    /** @brief 配置视频滤镜
     *
     *  @param graph 滤镜图
     *  @param vfilters 视频滤镜描述
     *  @param frame 视频帧
     *  @return 配置结果
     */
    int configure_video_filters(AVFilterGraph *graph, const char *vfilters, AVFrame *frame);

    /** @brief 打开视频文件失败回调
     *
     *  @param code 错误码
     */
    void doOpenVideoFileFailed(const int &code = 0);

    /** @brief 打开 SDL 失败回调
     *
     *  @param code 错误码
     */
    void doOpenSdlFailed(const int &code);

    /** @brief 视频时长变更回调
     *
     *  @param uSec 总时长（微秒）
     */
    void doTotalTimeChanged(const int64_t &uSec);

    /** @brief 播放器状态变更回调
     *
     *  @param state 播放器状态
     *  @param hasVideo 是否包含视频流
     *  @param hasAudio 是否包含音频流
     */
    void doPlayerStateChanged(const VideoPlayer::State &state, const bool &hasVideo, const bool &hasAudio);

    /** @brief 显示视频数据回调
     *
     *  @param yuv420Buffer YUV420P 数据缓冲区
     *  @param width 视频宽度
     *  @param height 视频高度
     */
    void doDisplayVideo(const uint8_t *yuv420Buffer, const int &width, const int &height);

    /** @brief 尝试解码元数据
     *
     *  @param data 元数据字符
     *  @return 解码后的字符串
     */
    static QString tryDecode(const char *data);

    /** @brief 解析元数据
     *
     *  @param pFormatCtx FFmpeg 格式上下文
     */
    void parseMetadata(AVFormatContext *pFormatCtx);

    /** @brief 从文件名解析艺术家和标题
     *
     *  @param filePath 文件路径
     *  @return 艺术家和标题对
     */
    QPair<QString, QString> parseArtistAndTitleFromFilename(const QString &filePath);

public:
    bool mIsOpenStream; ///< 是否正在打开流
    int64_t mCallStartTime = 0; ///< 调用开始时间

private:
    std::string m_file_path;                    ///< 文件路径
    bool m_is_live_mode = false;                ///< 是否为直播流
    float m_speed = 1;                          ///< 播放倍速
    State m_state;                              ///< 播放器状态
    bool mIsMute;                               ///< 是否静音
    float mVolume;                              ///< 音量（0~1，超过1表示放大）
    int seek_req = 0;                           ///< 跳转请求标志
    int64_t seek_pos;                           ///< 跳转位置（微秒）
    int seek_flag_audio;                        ///< 音频跳转标志
    int seek_flag_video;                        ///< 视频跳转标志
    int64_t seek_time;                          ///< 跳转时间（毫秒）
    bool mIsNeedPause;                          ///< 暂停请求标志
    bool mIsPause;                              ///< 暂停标志
    bool mIsQuit;                               ///< 停止标志
    bool mIsReadFinished;                       ///< 文件读取完成标志
    bool mIsReadThreadFinished;                 ///< 读取线程结束标志
    bool mIsVideoThreadFinished;                ///< 视频解码线程结束标志
    bool mIsAudioThreadFinished;                ///< 音频解码线程结束标志
    bool mIsReadError = false;                  ///< 读取错误标志
    uint64_t mVideoStartTime;                   ///< 视频播放开始时间
    uint64_t mPauseStartTime;                   ///< 暂停开始时间
    int64_t audio_clock;                        ///< 音频时钟（毫秒）
    int64_t video_clock;                        ///< 视频时钟（毫秒）
    AVStream *mVideoStream = nullptr;           ///< 视频流
    AVStream *mAudioStream = nullptr;           ///< 音频流
    AVFormatContext *pFormatCtx = nullptr;      ///< FFmpeg 格式上下文
    AVCodecContext *pCodecCtx = nullptr;        ///< 视频解码上下文
    AVCodec *pCodec = nullptr;                  ///< 视频解码器
    AVCodecContext *aCodecCtx = nullptr;        ///< 音频解码上下文
    AVCodec *aCodec = nullptr;                  ///< 音频解码器
    AVFrame *aFrame = nullptr;                  ///< 音频帧
    AVFrame *aFrame_ReSample = nullptr;         ///< 重采样音频帧
    SwrContext *swrCtx = nullptr;               ///< 音频重采样上下文
    enum AVSampleFormat in_sample_fmt;          ///< 输入采样格式
    enum AVSampleFormat out_sample_fmt;         ///< 输出采样格式
    int m_in_sample_rate;                       ///< 输入采样率
    int m_out_sample_rate;                      ///< 输出采样率
    int audio_tgt_channels;                     ///< 目标声道数
    int out_ch_layout;                          ///< 输出声道布局
    unsigned int audio_buf_size;                ///< 音频缓冲区大小
    unsigned int audio_buf_index;               ///< 音频缓冲区索引
    uint8_t audio_buf[AVCODEC_MAX_AUDIO_FRAME_SIZE * 4]; ///< 音频缓冲区
    int autorotate = 1;                         ///< 自动旋转视频
    int find_stream_info = 1;                   ///< 查找流信息
    int filter_nbthreads = 0;                   ///< 滤镜线程数
    const char **vfilters_list = nullptr;       ///< 视频滤镜列表
    int nb_vfilters = 0;                        ///< 视频滤镜数量
    char *afilters = nullptr;                   ///< 音频滤镜
    int vfilter_idx;                            ///< 视频滤镜索引
    AVFilterContext *in_video_filter;           ///< 输入视频滤镜
    AVFilterContext *out_video_filter;          ///< 输出视频滤镜
    Thread *m_thread_video = nullptr;           ///< 视频解码线程
    std::mutex m_mutex_video;                   ///< 视频队列互斥锁
    std::condition_variable m_cond_video;       ///< 视频队列条件变量
    std::list<AVPacket> m_video_pkt_list;       ///< 视频数据包队列
    bool m_enable_video_decode = true;          ///< 是否启用视频解码
    bool m_enable_encoded_video_callback = false; ///< 是否启用编码视频回调
    Thread *m_thread_audio = nullptr;           ///< 音频解码线程
    std::mutex m_mutex_audio;                   ///< 音频队列互斥锁
    std::condition_variable m_cond_audio;       ///< 音频队列条件变量
    std::list<AVPacket> m_audio_pkt_list;       ///< 音频数据包队列
    bool m_enable_audio_play = true;            ///< 是否启用音频播放
    bool m_enable_encoded_audio_callback = false; ///< 是否启用编码音频回调
#ifdef USE_PCM_PLAYER
    PcmPlayer *m_pcm_player = nullptr;          ///< PCM 播放器
#endif
    EventHandle *m_event_handle = nullptr;      ///< 事件处理回调
    QTimer m_positionUpdateTimer;               ///< 播放位置更新定时器
    QString m_musicAlbum;                       ///< 音乐专辑
    QString m_musicTitle;                       ///< 音乐标题
    QString m_musicArtist;                      ///< 音乐艺术家
    QPixmap m_musicPicture;                     ///< 音乐图片
    std::mutex m_mutex_read_finished;           ///< 读取线程结束互斥锁
    std::condition_variable m_cond_read_finished; ///< 读取线程结束条件变量
    bool mIsNaturalEnd = false;                 ///< 是否自然结束标志
};

#endif // VIDEOPLAYER_H