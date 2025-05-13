/*
  VideoFrame - 视频帧管理类
*/

#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <cstdint>
#include <memory>

#include "VideoRawFrame.h"
#include "VideoEncodedFrame.h"

#define VideoFramePtr std::shared_ptr<VideoFrame> ///< VideoFrame 的智能指针类型

/** @class VideoFrame
 *  @brief 管理视频帧的类
 *
 *  该类用于存储和管理编码或原始视频帧，支持 H.264、H.265、YUV420P、RGB 等格式。
 */
class VideoFrame {
public:
    /** @enum FrameType
     *  @brief 视频帧类型
     */
    enum FrameType {
        VIDEOFRAME_TYPE_NONE = -1,     ///< 无类型
        VIDEOFRAME_TYPE_YUV420P,       ///< 平面 YUV 4:2:0，12bpp（每 2x2 Y 样本有 1 个 Cr 和 Cb 样本）
        VIDEOFRAME_TYPE_RGB8,          ///< 8 位 RGB
        VIDEOFRAME_TYPE_RGB24,         ///< 打包 RGB 8:8:8，24bpp，RGBRGB...
        VIDEOFRAME_TYPE_H264,          ///< H.264 编码帧
        VIDEOFRAME_TYPE_H265           ///< H.265 编码帧
    };

    /** @brief 构造函数
     *
     *  初始化 VideoFrame 对象。
     */
    VideoFrame();

    /** @brief 析构函数
     *
     *  销毁 VideoFrame 对象。
     */
    ~VideoFrame();

    /** @brief 设置编码帧
     *
     *  设置视频帧为编码帧（H.264 或 H.265）。
     *
     *  @param frame 编码帧智能指针
     */
    void setFrame(std::shared_ptr<VideoEncodedFrame> frame);

    /** @brief 设置原始帧
     *
     *  设置视频帧为原始帧（YUV 或 RGB）。
     *
     *  @param frame 原始帧智能指针
     */
    void setFrame(std::shared_ptr<VideoRawFrame> frame);

    /** @brief 获取帧缓冲区
     *
     *  @return 帧缓冲区指针
     */
    uint8_t *buffer();

    /** @brief 获取帧缓冲区大小
     *
     *  @return 帧缓冲区大小
     */
    int size();

    /** @brief 获取帧宽度
     *
     *  @return 帧宽度
     */
    int width() const { return m_width; }

    /** @brief 获取帧高度
     *
     *  @return 帧高度
     */
    int height() const { return m_hegiht; }

protected:
    /** @brief 帧类型
     */
    FrameType m_type = VIDEOFRAME_TYPE_NONE;

    /** @brief 帧宽度
     */
    int m_width = -100;

    /** @brief 帧高度
     */
    int m_hegiht = -100;

    /** @brief 时间戳
     */
    int64_t m_pts = 0;

    /** @brief 编码帧
     */
    std::shared_ptr<VideoEncodedFrame> m_encoded_frame = nullptr;

    /** @brief 原始帧
     */
    std::shared_ptr<VideoRawFrame> m_raw_frame = nullptr;
};

#endif // VIDEOFRAME_H