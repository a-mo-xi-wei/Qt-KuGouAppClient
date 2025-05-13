/*
AACFrame - AAC 音频帧管理类实现
*/

#include "AACFrame.h"

#include "util/util.h"

/** @brief 构造函数
 *
 *  初始化 AACFrame 对象，设置初始时间戳。
 */
AACFrame::AACFrame() {
    mFrameBuffer = nullptr;
    mFrameBufferSize = 0;
    m_timestamp_ms = Util::GetUtcTime();
}

/** @brief 析构函数
 *
 *  销毁 AACFrame 对象，释放缓冲区资源。
 */
AACFrame::~AACFrame() {
    if (mFrameBuffer != nullptr) {
        free(mFrameBuffer);
        mFrameBuffer = nullptr;
        mFrameBufferSize = 0;
    }
}

/** @brief 设置 ADTS 头部
 *
 *  设置 AAC 帧的 ADTS 头部信息。
 *
 *  @param adts ADTS 头部结构
 */
void AACFrame::setAdtsHeader(const ADTS_HEADER &adts) {
    mAdtsHeader = adts;
}

/** @brief 设置帧缓冲区
 *
 *  设置 AAC 音频帧的缓冲区数据。
 *
 *  @param buffer 缓冲区数据
 *  @param size 缓冲区大小
 */
void AACFrame::setFrameBuffer(const uint8_t *const buffer, const unsigned int &size) {
    if (mFrameBufferSize < size) {
        if (mFrameBuffer != nullptr) {
            free(mFrameBuffer);
        }
        mFrameBuffer = static_cast<uint8_t*>(malloc(size));
    }

    memcpy(mFrameBuffer, buffer, size);
    mFrameBufferSize = size;
}

/** @brief 设置帧缓冲区（包含 ADTS 头部）
 *
 *  设置 AAC 音频帧的 ADTS 头部和数据缓冲区。
 *
 *  @param adtsBuffer ADTS 头部缓冲区
 *  @param adtsSize ADTS 头部大小
 *  @param buffer 数据缓冲区
 *  @param size 数据缓冲区大小
 */
void AACFrame::setFrameBuffer(const uint8_t *const adtsBuffer, const unsigned int &adtsSize, const uint8_t *const buffer, const unsigned int &size) {
    if (mFrameBufferSize < (size + adtsSize)) {
        if (mFrameBuffer != nullptr) {
            free(mFrameBuffer);
        }
        mFrameBuffer = static_cast<uint8_t*>(malloc(size + adtsSize));
    }

    memcpy(mFrameBuffer, adtsBuffer, adtsSize);
    memcpy(mFrameBuffer + adtsSize, buffer, size);
    mFrameBufferSize = (size + adtsSize);
}