/*
  PcmPlayer - PCM 音频播放器基类
*/

#ifndef PCMPLAYER_H
#define PCMPLAYER_H

#include <QPixmap>
#include <QObject>
#include <string>
#include <list>
#include <mutex>
#include <condition_variable>

#include "frame/AudioFrame/PCMFrame.h"

/** @struct AudioDevice
 *  @brief 音频设备信息
 */
struct AudioDevice {
    /** @brief 设备名称
     */
    std::string deviceName;

    /** @brief 设备 ID
     */
    uint32_t deviceId;
};

/** @class PcmPlayer
 *  @brief PCM 音频播放器基类
 *
 *  该类提供 PCM 音频播放的基本功能，包括设备管理、帧输入、音量控制等。
 */
class PcmPlayer : public QObject {
    Q_OBJECT
public:
    /** @brief 构造函数
     *
     *  初始化 PcmPlayer 对象。
     */
    PcmPlayer();

    /** @brief 析构函数
     *
     *  销毁 PcmPlayer 对象。
     */
    virtual ~PcmPlayer();

    /** @brief 获取音频设备列表
     *
     *  纯虚函数，需由子类实现。
     *
     *  @return 音频设备列表
     */
    virtual std::list<AudioDevice> getAudiDeviceList() = 0;

    /** @brief 开始播放
     *
     *  初始化音频设备并开始播放。
     *
     *  @return true 表示成功
     */
    bool startPlay();

    /** @brief 停止播放
     *
     *  关闭音频设备并停止播放。
     *
     *  @return true 表示成功
     */
    bool stopPlay();

    /** @brief 输入 PCM 帧
     *
     *  将 PCM 帧添加到播放队列。
     *
     *  @param frame PCM 帧智能指针
     *  @return 当前队列中的帧数
     */
    int inputPCMFrame(PCMFramePtr frame);

    /** @brief 获取 PCM 帧队列大小
     *
     *  @return 当前队列中的帧数
     */
    int getPcmFrameSize();

    /** @brief 检查帧队列是否已满
     *
     *  @return true 表示队列已满
     */
    bool isFrameFull();

    /** @brief 清空帧队列
     */
    void clearFrame();

    /** @brief 获取当前播放帧时间戳
     *
     *  @return 当前播放帧的时间戳
     */
    uint32_t getCurrentPts() const { return m_current_pts; }

    /** @brief 设置暂停状态
     *
     *  @param pause 是否暂停
     */
    void setPause(bool pause) { m_is_pause = pause; }

    /** @brief 设置播放速度
     *
     *  @param speed 播放速度倍数
     */
    void setSpeed(float speed) { m_play_speed = speed; }

    /** @brief 设置静音状态
     *
     *  @param is_mute 是否静音
     */
    void setMute(const bool is_mute) { m_is_mute = is_mute; }

    /** @brief 设置音量
     *
     *  @param value 音量值（0~1，超过 1 表示放大）
     */
    void setVolume(float value) { m_volume = value; }

    /** @brief 获取音量
     *
     *  @return 当前音量值
     */
    float getVolume() const { return m_volume; }

    /** @brief 检查设备是否已打开
     *
     *  @return true 表示设备已打开
     */
    bool deviceOpened() const { return m_device_opened; }

    /** @brief 检查设备是否打开失败
     *
     *  @return true 表示设备打开失败
     */
    bool deviceOpenFailed() const { return m_device_open_failed; }

protected:
    /** @brief 音频队列互斥锁
     */
    std::mutex m_mutex_audio;

    /** @brief 音频队列条件变量
     */
    std::condition_variable m_cond_audio;

    /** @brief PCM 帧队列
     */
    std::list<PCMFramePtr> m_pcm_frame_list;

    /** @brief 播放速度
     */
    float m_play_speed = 1.0f;

    /** @brief 上一次未处理完的数据缓冲区
     */
    uint8_t m_last_frame_buffer[10240];

    /** @brief 上一次未处理完的数据大小
     */
    int m_last_frame_buffer_size = 0;

    /** @brief 当前播放帧时间戳
     */
    uint32_t m_current_pts = 0;

    /** @brief 设备是否已打开
     */
    bool m_device_opened = false;

    /** @brief 设备是否打开失败
     */
    bool m_device_open_failed = false;

    /** @brief 上一次尝试打开设备的时间
     */
    uint64_t m_last_try_open_device_time = 0;

    /** @brief 采样率
     */
    int m_sample_rate = 0;

    /** @brief 声道数
     */
    int m_channel = 0;

    /** @brief 缓存大小
     */
    int m_cache_size = 81920;

    /** @brief 是否停止播放
     */
    bool m_is_stop = false;

    /** @brief 是否暂停播放
     */
    bool m_is_pause = false;

    /** @brief 是否静音
     */
    bool m_is_mute = false;

    /** @brief 音量值
     */
    float m_volume = 1.0f;

    /** @brief 打开音频设备
     *
     *  纯虚函数，需由子类实现。
     *
     *  @return true 表示成功
     */
    virtual bool openDevice() = 0;

    /** @brief 关闭音频设备
     *
     *  纯虚函数，需由子类实现。
     *
     *  @return true 表示成功
     */
    virtual bool closeDevice() = 0;

    /** @brief 播放音频缓冲区
     *
     *  处理音频数据并输出到音频设备。
     *
     *  @param stream 输出音频流
     *  @param len 输出流长度
     */
    void playAudioBuffer(void *stream, int len);
};

#endif // PCMPLAYER_H