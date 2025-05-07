#ifndef AUDIOPLAYERSDL_H
#define AUDIOPLAYERSDL_H

extern "C"
{
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_audio.h>
}

#include "PcmPlayer/PcmPlayer.h"

class PcmPlayer_SDL : public PcmPlayer
{
    Q_OBJECT
public:
    PcmPlayer_SDL();
    ~PcmPlayer_SDL();

    std::list<AudioDevice> getAudiDeviceList(); //获取音频设备列表

private:
    ///本播放器中SDL仅用于播放音频，不用做别的用途
    ///SDL播放音频相关
    SDL_AudioDeviceID mAudioID = 0;
    bool openDevice();
    bool closeDevice();

    void sdlAudioCallBack(Uint8 *stream, int len);

protected:
    static void sdlAudioCallBackFunc(void *userdata, Uint8 *stream, int len);

};

#endif // AUDIOPLAYERSDL_H
