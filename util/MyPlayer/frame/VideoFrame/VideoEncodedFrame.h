/*
  VideoEncodedFrame - 编码视频帧管理类
*/

#ifndef VIDEOENCODEDFRAME_H
#define VIDEOENCODEDFRAME_H

#include <cstdint>

#include "nalu/nalu.h"

#define VideoEncodedFramePtr std::shared_ptr<VideoEncodedFrame> ///< VideoEncodedFrame 的智能指针类型

/** @class VideoEncodedFrame
 *  @brief 管理编码视频帧的类
 *
 *  该类用于存储和管理 H.264 或 H.265 编码视频帧，包括 NAL 单元、关键帧标志和时间戳。
 */
class VideoEncodedFrame {
public:
    /** @brief 构造函数
     *
     *  初始化 VideoEncodedFrame 对象。
     */
    VideoEncodedFrame();

    /** @brief 析构函数
     *
     *  销毁 VideoEncodedFrame 对象，释放 NAL 单元资源。
     */
    ~VideoEncodedFrame();

    /** @brief 设置 NAL 单元
     *
     *  设置编码帧的 NAL 单元和时间戳。
     *
     *  @param nalu NAL 单元指针
     *  @param time 时间戳（毫秒）
     */
    void setNalu(T_NALU *nalu, const int64_t &time = 0);

    /** @brief 设置 NAL 单元（从缓冲区）
     *
     *  从缓冲区数据构造 NAL 单元并设置编码帧。
     *
     *  @param buffer 缓冲区数据
     *  @param len 缓冲区长度
     *  @param isAllocBuffer 是否分配新缓冲区
     *  @param type NAL 单元类型（H.264 或 H.265）
     *  @param time 时间戳（毫秒）
     */
    void setNalu(uint8_t *buffer, const int &len, const bool &isAllocBuffer, const T_NALU_TYPE &type, const uint64_t &time = 0);

    /** @brief 设置关键帧标志
     *
     *  @param is_key_frame 是否为关键帧
     */
    void setIsKeyFrame(const bool &is_key_frame) { m_key_frame = is_key_frame; }

    /** @brief 获取 NAL 单元
     *
     *  @return NAL 单元指针
     */
    T_NALU *nalu() { return m_nalu; }

    /** @brief 检查是否为关键帧
     *
     *  @return true 如果是关键帧
     */
    bool isKeyFrame() { return m_key_frame; }

    /** @brief 获取时间戳
     *
     *  @return 帧的时间戳
     */
    uint64_t pts() { return m_pts; }

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

    /** @brief 获取帧缓冲区
     *
     *  @return 帧缓冲区指针
     */
    uint8_t *buffer();

    /** @brief 获取帧缓冲区大小
     *
     *  @return 帧缓冲区大小
     */
    unsigned int size();

private:
    /** @brief NAL 单元
     */
    T_NALU *m_nalu = nullptr;

    /** @brief 关键帧标志
     */
    bool m_key_frame = false;

    /** @brief 时间戳
     */
    uint64_t m_pts = 0;

    /** @brief UTC 时间戳（毫秒）
     */
    uint64_t m_timestamp_ms = 0;
};

#endif // VIDEOENCODEDFRAME_H