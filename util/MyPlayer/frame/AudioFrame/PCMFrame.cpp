/*
PCMFrame - PCM 音频帧管理类实现
*/

#include "PCMFrame.h"

#include "util/util.h"

/** @brief 构造函数
 *
 *  初始化 PCMFrame 对象，设置初始时间戳。
 */
PCMFrame::PCMFrame() {
    mFrameBuffer = nullptr;
    mFrameBufferSize = 0;
    m_timestamp_ms = Util::GetUtcTime();
}

/** @brief 析构函数
 *
 *  销毁 PCMFrame 对象，释放缓冲区资源。
 */
PCMFrame::~PCMFrame() {
    if (mFrameBuffer != nullptr) {
        free(mFrameBuffer);
        mFrameBuffer = nullptr;
        mFrameBufferSize = 0;
    }
}

/** @brief 设置帧信息
 *
 *  设置音频帧的采样率、声道数、时间戳和帧类型。
 *
 *  @param sample_rate 采样率
 *  @param channels 声道数
 *  @param pts 时间戳
 *  @param type 帧类型
 */
void PCMFrame::setFrameInfo(int sample_rate, int channels, uint32_t pts, FrameType type) {
    m_sample_rate = sample_rate;
    m_channels = channels;
    m_pts = pts;
    m_type = type;
}

/** @brief 设置帧缓冲区
 *
 *  设置 PCM 音频帧的缓冲区数据。
 *
 *  @param buffer 缓冲区数据
 *  @param size 缓冲区大小
 */
void PCMFrame::setFrameBuffer(const uint8_t *const buffer, const unsigned int &size) {
    if (mFrameBufferSize < size) {
        if (mFrameBuffer != nullptr) {
            free(mFrameBuffer);
        }
        mFrameBuffer = static_cast<uint8_t*>(malloc(size));
    }

    memcpy(mFrameBuffer, buffer, size);
    mFrameBufferSize = size;
}