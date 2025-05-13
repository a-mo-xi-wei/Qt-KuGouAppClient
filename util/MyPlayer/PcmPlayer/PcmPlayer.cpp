/*
  PcmPlayer - PCM 音频播放器基类实现
*/

#include "PcmPlayer.h"
#include "PcmVolumeControl.h"

#include <qDebug>

/** @brief 构造函数
 *
 *  初始化 PcmPlayer 对象。
 */
PcmPlayer::PcmPlayer() {
}

/** @brief 析构函数
 *
 *  销毁 PcmPlayer 对象。
 */
PcmPlayer::~PcmPlayer() {
}

/** @brief 开始播放
 *
 *  初始化音频设备并开始播放。
 *
 *  @return true 表示成功
 */
bool PcmPlayer::startPlay() {
    auto now = std::chrono::system_clock::now();
    auto tick = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    m_last_try_open_device_time = tick;

    bool is_succeed = openDevice();
    m_device_opened = is_succeed;
    m_device_open_failed = !m_device_opened;

    m_cache_size = 0.15 * m_sample_rate * m_channel; ///< 计算150ms的缓存大小

    m_is_stop = false;
    m_is_pause = false;

    return is_succeed;
}

/** @brief 停止播放
 *
 *  关闭音频设备并停止播放。
 *
 *  @return true 表示成功
 */
bool PcmPlayer::stopPlay() {
    m_is_stop = true;
    m_is_pause = true;
    m_cond_audio.notify_all();
    const bool isSucceed = closeDevice();
    m_device_opened = false;
    return isSucceed;
}

/** @brief 输入 PCM 帧
 *
 *  将 PCM 帧添加到播放队列。
 *
 *  @param frame PCM 帧智能指针
 *  @return 当前队列中的帧数
 */
int PcmPlayer::inputPCMFrame(PCMFramePtr frame) {
    int frame_size = 0;

    if (m_device_opened) { ///< 音频设备打开的情况下才处理播放
        {
            std::lock_guard<std::mutex> lck(m_mutex_audio);
            m_pcm_frame_list.push_back(frame);
            frame_size = m_pcm_frame_list.size();
            m_cond_audio.notify_all();
        }

        int channels = frame->channels();
        int sample_rate = frame->sampleRate();

        ///< 用于实现 播放队列太大的时候，提高播放采样率，用于直播消延时
        static int use_sample_rate = frame->sampleRate();
        if (frame_size > 20) {
            // use_sample_rate = 32000;
            use_sample_rate = sample_rate * 2;
        } else if (frame_size < 10 && use_sample_rate != sample_rate) {
            use_sample_rate = sample_rate;
        }

        if (m_sample_rate != use_sample_rate || m_channel != channels || !m_device_opened) {
            m_sample_rate = use_sample_rate;
            m_channel = channels;

            stopPlay();
            startPlay();
        }
    } else {
        auto now = std::chrono::system_clock::now();
        auto tick = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

        if (tick - m_last_try_open_device_time > 50) {
            ///< 音频设备未打开，则每隔50ms尝试打开一次
            printf("try to open audio device ... \n");
            stopPlay();
            startPlay();
        }
    }
    return frame_size;
}

/** @brief 获取 PCM 帧队列大小
 *
 *  @return 当前队列中的帧数
 */
int PcmPlayer::getPcmFrameSize() {
    int size = m_pcm_frame_list.size();
    return size;
}

/** @brief 检查帧队列是否已满
 *
 *  @return true 表示队列已满
 */
// bool PcmPlayer::isFrameFull()
// {
//     return (m_ring_buffer->GetValidSize() > m_cache_size);
// }

/** @brief 清空帧队列
 */
void PcmPlayer::clearFrame() {
    std::lock_guard<std::mutex> lck(m_mutex_audio);
    m_pcm_frame_list.clear();
}

/** @brief 播放音频缓冲区
 *
 *  处理音频数据并输出到音频设备。
 *
 *  @param stream 输出音频流
 *  @param len 输出流长度
 */
void PcmPlayer::playAudioBuffer(void *stream, int len) {
    if (m_is_pause) { ///< 检查暂停或停止状态
        memset(stream, 0, len); ///< 输出静音
        return;
    }

    while (m_last_frame_buffer_size < len) {
        std::unique_lock<std::mutex> lck(m_mutex_audio);
        while (m_pcm_frame_list.empty() && !m_is_stop) {
            if (m_cond_audio.wait_for(lck, std::chrono::milliseconds(500)) == std::cv_status::timeout) {
                break;
            }
        }

        if (m_pcm_frame_list.empty() || m_is_stop) {
            break;
        }

        PCMFramePtr pcm_frame = m_pcm_frame_list.front();
        m_pcm_frame_list.pop_front();

        memcpy(m_last_frame_buffer + m_last_frame_buffer_size, pcm_frame->buffer(), pcm_frame->size());
        m_last_frame_buffer_size += pcm_frame->size();

        m_current_pts = pcm_frame->pts();
    }

    if (m_last_frame_buffer_size > 0) {
        int buffer_size = std::min(m_last_frame_buffer_size, len);
        if (m_is_mute) {
            memset(stream, 0x0, len);
        } else {
            PcmVolumeControl::RaiseVolume((char*)m_last_frame_buffer, buffer_size, 1, m_volume);
            memcpy(stream, m_last_frame_buffer, buffer_size);
        }

        m_last_frame_buffer_size -= buffer_size;
        if (m_last_frame_buffer_size > 0) {
            memmove(m_last_frame_buffer, m_last_frame_buffer + buffer_size, m_last_frame_buffer_size);
        }
    }
}