/*
  PCMFrame - PCM 音频帧管理类
*/

#ifndef PCMFRAME_H
#define PCMFRAME_H

#include <cstdint>

extern "C" 
{
    #include <libavformat/avformat.h>
}

/** @def PCMFramePtr
 *  @brief PCMFrame 的智能指针类型
 *
 *  定义为 PCMFrame 类的 std::shared_ptr。
 */
#define PCMFramePtr std::shared_ptr<PCMFrame>

/** @class PCMFrame
 *  @brief 管理 PCM 音频帧的类
 *
 *  该类用于存储和管理 PCM 音频帧数据，支持设置帧缓冲区、帧信息和时间戳。本程序固定使用 AV_SAMPLE_FMT_S16、44100 Hz、双声道。
 */
class PCMFrame {
public:
    /** @enum FrameType
     *  @brief PCM 帧类型
     */
    enum FrameType {
        PCMFRAME_TYPE_NONE = -1,  /**< 无类型 */
        PCMFRAME_TYPE_PCM,        /**< PCM 格式 */
        PCMFRAME_TYPE_PCMU,       /**< PCMU 格式 */
        PCMFRAME_TYPE_PCMA,       /**< PCMA 格式 */
        PCMFRAME_TYPE_G711U,      /**< G711U 格式 */
        PCMFRAME_TYPE_G711A       /**< G711A 格式 */
    };

    /** @brief 构造函数
     *
     *  初始化 PCMFrame 对象。
     */
    PCMFrame();

    /** @brief 析构函数
     *
     *  销毁 PCMFrame 对象，释放缓冲区资源。
     */
    ~PCMFrame();

    /** @brief 设置帧缓冲区
     *
     *  设置 PCM 音频帧的缓冲区数据。
     *
     *  @param buffer 缓冲区数据
     *  @param size 缓冲区大小
     */
    void setFrameBuffer(const uint8_t *const buffer, const unsigned int &size);

    /** @brief 获取帧缓冲区
     *
     *  @return 帧缓冲区指针
     */
    uint8_t *buffer() { return mFrameBuffer; }

    /** @brief 获取帧缓冲区大小
     *
     *  @return 帧缓冲区大小
     */
    unsigned int size() { return mFrameBufferSize; }

    /** @brief 设置帧信息
     *
     *  设置音频帧的采样率、声道数、时间戳和帧类型。
     *
     *  @param sample_rate 采样率
     *  @param channels 声道数
     *  @param pts 时间戳
     *  @param type 帧类型，默认为 PCMFRAME_TYPE_PCM
     */
    void setFrameInfo(int sample_rate, int channels, uint32_t pts, FrameType type = PCMFRAME_TYPE_PCM);

    /** @brief 获取时间戳
     *
     *  @return 帧的时间戳
     */
    uint32_t pts() { return m_pts; }

    /** @brief 获取采样率
     *
     *  @return 帧的采样率
     */
    int sampleRate() { return m_sample_rate; }

    /** @brief 获取声道数
     *
     *  @return 帧的声道数
     */
    int channels() { return m_channels; }

    /** @brief 获取帧类型
     *
     *  @return 帧的类型
     */
    FrameType type() { return m_type; }

    /** @brief 设置 UTC 时间戳
     *
     *  @param t UTC 时间戳（毫秒）
     */
    void setTimeStamp(uint64_t t) { m_timestamp_ms = t; }

    /** @brief 获取 UTC 时间戳
     *
     *  @return UTC 时间戳（毫秒）
     */
    uint64_t timeStamp() { return m_timestamp_ms; }

private:
    /** @brief PCM 数据缓冲区
     */
    uint8_t *mFrameBuffer = nullptr;

    /** @brief PCM 数据缓冲区大小
     */
    unsigned int mFrameBufferSize = 0;

    /** @brief 时间戳
     */
    uint32_t m_pts = 0;

    /** @brief 采样率
     */
    int m_sample_rate = 0;

    /** @brief 声道数
     */
    int m_channels = 0;

    /** @brief 帧类型
     */
    FrameType m_type = PCMFRAME_TYPE_NONE;

    /** @brief UTC 时间戳（毫秒）
     */
    uint64_t m_timestamp_ms = 0;
};

#endif // PCMFRAME_H