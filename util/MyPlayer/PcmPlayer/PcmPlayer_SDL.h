/*
PcmPlayer_SDL - SDL 音频播放器类
*/

#ifndef PCMPLAYER_SDL_H
#define PCMPLAYER_SDL_H

extern "C" {
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
}

#include "PcmPlayer/PcmPlayer.h"

/** @class PcmPlayer_SDL
 *  @brief SDL 音频播放器类，继承自 PcmPlayer
 *
 *  该类使用 SDL 库实现 PCM 音频播放，支持设备管理、暂停和恢复功能。
 */
class PcmPlayer_SDL : public PcmPlayer {
    Q_OBJECT
public:
    /** @brief 构造函数
     *
     *  初始化 PcmPlayer_SDL 对象。
     */
    PcmPlayer_SDL();

    /** @brief 析构函数
     *
     *  销毁 PcmPlayer_SDL 对象，释放 SDL 资源。
     */
    ~PcmPlayer_SDL() override;

    /** @brief 获取音频设备列表
     *
     *  @return 音频设备列表
     */
    std::list<AudioDevice> getAudiDeviceList() override;

    /** @brief 暂停音频设备
     */
    void pauseDevice();

    /** @brief 恢复音频设备
     */
    void resumeDevice();

private:
    /** @brief SDL 音频设备 ID
     */
    SDL_AudioDeviceID mAudioID = 0;

    /** @brief 打开音频设备
     *
     *  初始化 SDL 音频设备并设置播放格式。
     *
     *  @return true 表示成功
     */
    bool openDevice() override;

    /** @brief 关闭音频设备
     *
     *  关闭 SDL 音频设备并清理资源。
     *
     *  @return true 表示成功
     */
    bool closeDevice() override;

    /** @brief SDL 音频回调函数
     *
     *  静态回调函数，传递给 SDL 处理音频数据。
     *
     *  @param userdata 用户数据（PcmPlayer_SDL 实例）
     *  @param stream 输出音频流
     *  @param len 输出流长度
     */
    static void sdlAudioCallBackFunc(void *userdata, Uint8 *stream, int len);
};

#endif // PCMPLAYER_SDL_H