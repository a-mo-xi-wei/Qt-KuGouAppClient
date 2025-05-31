/*
  PcmPlayer_SDL - SDL 音频播放器类实现
*/

#include "PcmPlayer_SDL.h"

/** @brief SDL 音频回调函数
 *
 *  静态回调函数，调用 PcmPlayer_SDL 的播放缓冲区处理逻辑。
 *
 *  @param userdata 用户数据（PcmPlayer_SDL 实例）
 *  @param stream 输出音频流
 *  @param len 输出流长度
 */
void PcmPlayer_SDL::sdlAudioCallBackFunc(void *userdata, Uint8 *stream, int len) {
    //fprintf(stderr, "%s function called and \n", __FUNCTION__);
    PcmPlayer_SDL *player = (PcmPlayer_SDL*)userdata;
    player->playAudioBuffer(stream, len);
}

/** @brief 构造函数
 *
 *  初始化 PcmPlayer_SDL 对象。
 */
PcmPlayer_SDL::PcmPlayer_SDL() {
//    ///SDL初始化需要放入子线程中，否则有些电脑会有问题。
//    if (SDL_Init(SDL_INIT_AUDIO))
//    {
//        fprintf(stderr, "Could not initialize SDL - %s. \n", SDL_GetError());
//    }
}

/** @brief 析构函数
 *
 *  销毁 PcmPlayer_SDL 对象，释放 SDL 资源。
 */
PcmPlayer_SDL::~PcmPlayer_SDL() {
    SDL_Quit();
}

/** @brief 获取音频设备列表
 *
 *  @return 音频设备列表
 */
std::list<AudioDevice> PcmPlayer_SDL::getAudiDeviceList() {
    std::list<AudioDevice> deviceList;

    int num = SDL_GetNumAudioDevices(0);
    for (int i = 0; i < num; i++) {
        AudioDevice device;
        device.deviceId = i;
        device.deviceName = SDL_GetAudioDeviceName(i, 0);

        deviceList.push_back(device);
    }

    return deviceList;
}

/** @brief 打开音频设备
 *
 *  初始化 SDL 音频设备并设置播放格式（AUDIO_S16SYS，双声道，44100Hz）。
 *
 *  @return true 表示成功
 */
bool PcmPlayer_SDL::openDevice() {
    ///< SDL初始化需要放入子线程中，否则有些电脑会有问题。
    if (SDL_Init(SDL_INIT_AUDIO)) {
        fprintf(stderr, "Could not initialize SDL - %s. \n", SDL_GetError());
    }

    ///< 打开SDL，并设置播放的格式为:AUDIO_S16LSB 双声道，44100hz
    ///< 后期使用ffmpeg解码完音频后，需要重采样成和这个一样的格式，否则播放会有杂音
    SDL_AudioSpec wanted_spec, spec;
//    int wanted_nb_channels = 2;
//    int samplerate = 44100;
    int wanted_nb_channels = m_channel;
    int samplerate = m_sample_rate;

    wanted_spec.channels = wanted_nb_channels;
    wanted_spec.freq = samplerate;
    wanted_spec.format = AUDIO_S16SYS; // 具体含义请查看“SDL宏定义”部分
    wanted_spec.silence = 0;            // 0指示静音
    wanted_spec.samples = 1024;  // 自定义SDL缓冲区大小
    wanted_spec.callback = sdlAudioCallBackFunc;  // 回调函数
    wanted_spec.userdata = this;                  // 传给上面回调函数的外带数据

    int num = SDL_GetNumAudioDevices(0);
    for (int i = 0; i < num; i++) {
        mAudioID = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(i, 0), false, &wanted_spec, &spec, 0);
        if (mAudioID > 0) {
            break;
        }
    }
// printf("NUM=%d mAudioID=%d \n", num, mAudioID);
    /* 检查实际使用的配置（保存在spec,由SDL_OpenAudio()填充） */
//    if (spec.format != AUDIO_S16SYS)
    if (mAudioID <= 0)
    {
        fprintf(stderr, "Failed to open audio device\n");
        return false;
    }
    else
    {
        //fprintf(stderr, "Audio device opened: ID=%d\n", mAudioID);
        SDL_LockAudioDevice(mAudioID);
        SDL_PauseAudioDevice(mAudioID, 0);
        SDL_UnlockAudioDevice(mAudioID);
    }
    return true;
}

/** @brief 关闭音频设备
 *
 *  关闭 SDL 音频设备并清理资源。
 *
 *  @return true 表示成功
 */
bool PcmPlayer_SDL::closeDevice() {
    //fprintf(stderr, "%s function called and \n", __FUNCTION__);
    if (mAudioID > 0) {
        SDL_LockAudioDevice(mAudioID);
        SDL_PauseAudioDevice(mAudioID, 1);
        SDL_UnlockAudioDevice(mAudioID);
        SDL_CloseAudioDevice(mAudioID);
    }

    SDL_Quit();

    mAudioID = 0;

    return true;
}

/** @brief 暂停音频设备
 */
void PcmPlayer_SDL::pauseDevice() {
    if (mAudioID > 0) {
        SDL_LockAudioDevice(mAudioID);
        SDL_PauseAudioDevice(mAudioID, 1); // 暂停音频设备
        SDL_UnlockAudioDevice(mAudioID);
        SDL_ClearQueuedAudio(mAudioID); // 清空 SDL 音频队列
    }
}

/** @brief 恢复音频设备
 */
void PcmPlayer_SDL::resumeDevice() {
    if (mAudioID > 0) {
        SDL_LockAudioDevice(mAudioID);
        SDL_PauseAudioDevice(mAudioID, 0); // 恢复音频设备
        SDL_UnlockAudioDevice(mAudioID);
    }
}