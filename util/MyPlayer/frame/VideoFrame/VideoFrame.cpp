/*
VideoFrame - 视频帧管理类实现
*/

#include "VideoFrame.h"

/** @brief 构造函数
 *
 *  初始化 VideoFrame 对象。
 */
VideoFrame::VideoFrame() {
}

/** @brief 析构函数
 *
 *  销毁 VideoFrame 对象。
 */
VideoFrame::~VideoFrame() {
}

/** @brief 设置编码帧
 *
 *  设置视频帧为编码帧（H.264 或 H.265）。
 *
 *  @param frame 编码帧智能指针
 */
void VideoFrame::setFrame(std::shared_ptr<VideoEncodedFrame> frame) {
    if (frame->nalu()->type == T_NALU_H264) {
        m_type = VIDEOFRAME_TYPE_H264;
    } else if (frame->nalu()->type == T_NALU_H265) {
        m_type = VIDEOFRAME_TYPE_H265;
    }

    m_pts = frame->pts();

    m_encoded_frame = frame;
}

/** @brief 设置原始帧
 *
 *  设置视频帧为原始帧（YUV 或 RGB）。
 *
 *  @param frame 原始帧智能指针
 */
void VideoFrame::setFrame(std::shared_ptr<VideoRawFrame> frame) {
    m_type = (FrameType)frame->type();

    m_width = frame->width();
    m_hegiht = frame->height();
    m_pts = frame->pts();

    m_raw_frame = frame;
}

/** @brief 获取帧缓冲区
 *
 *  @return 帧缓冲区指针
 */
uint8_t *VideoFrame::buffer() {
    if (m_encoded_frame) {
        return m_encoded_frame->buffer();
    } else if (m_raw_frame) {
        return m_raw_frame->buffer();
    }
    return nullptr;
}

/** @brief 获取帧缓冲区大小
 *
 *  @return 帧缓冲区大小
 */
int VideoFrame::size() {
    if (m_encoded_frame) {
        return m_encoded_frame->size();
    } else if (m_raw_frame) {
        return m_raw_frame->size();
    }
    return 0;
}