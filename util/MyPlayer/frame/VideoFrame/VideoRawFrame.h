/*
  VideoRawFrame - 原始视频帧管理类
*/

#pragma once

#include <cstdint>

#define VideoRawFramePtr std::shared_ptr<VideoRawFrame> ///< VideoRawFrame 的智能指针类型

/** @class VideoRawFrame
 *  @brief 管理原始视频帧的类
 *
 *  该类用于存储和管理原始视频帧（YUV 或 RGB 格式），支持缓冲区初始化和分量设置。
 */
class VideoRawFrame {
public:
    /** @enum FrameType
     *  @brief 原始视频帧类型
     */
    enum FrameType {
        FRAME_TYPE_NONE = -1,     ///< 无类型
        FRAME_TYPE_YUV420P,       ///< 平面 YUV 4:2:0，12bpp（每 2x2 Y 样本有 1 个 Cr 和 Cb 样本）
        FRAME_TYPE_RGB8,          ///< 8 位 RGB
        FRAME_TYPE_RGB24          ///< 打包 RGB 8:8:8，24bpp，RGBRGB...
    };

    /** @brief 构造函数
     *
     *  初始化 VideoRawFrame 对象。
     */
    VideoRawFrame();

    /** @brief 析构函数
     *
     *  销毁 VideoRawFrame 对象，释放缓冲区资源。
     */
    ~VideoRawFrame();

    /** @brief 初始化缓冲区
     *
     *  根据宽度、高度和帧类型初始化帧缓冲区。
     *
     *  @param width 帧宽度
     *  @param height 帧高度
     *  @param type 帧类型
     *  @param time 时间戳（毫秒）
     */
    void initBuffer(const int &width, const int &height, const FrameType &type, int64_t time = 0);

    /** @brief 设置帧缓冲区
     *
     *  设置整个帧的缓冲区数据。
     *
     *  @param buf 缓冲区数据
     */
    void setFramebuf(const uint8_t *buf);

    /** @brief 设置 Y 分量缓冲区
     *
     *  设置 YUV 帧的 Y 分量数据。
     *
     *  @param buf Y 分量数据
     */
    void setYbuf(const uint8_t *buf);

    /** @brief 设置 U 分量缓冲区
     *
     *  设置 YUV 帧的 U 分量数据。
     *
     *  @param buf U 分量数据
     */
    void setUbuf(const uint8_t *buf);

    /** @brief 设置 V 分量缓冲区
     *
     *  设置 YUV 帧的 V 分量数据。
     *
     *  @param buf V 分量数据
     */
    void setVbuf(const uint8_t *buf);

    /** @brief 获取帧缓冲区
     *
     *  @return 帧缓冲区指针
     */
    uint8_t *buffer() { return mFrameBuffer; }

    /** @brief 获取帧宽度
     *
     *  @return 帧宽度
     */
    int width() { return mWidth; }

    /** @brief 获取帧高度
     *
     *  @return 帧高度
     */
    int height() { return mHegiht; }

    /** @brief 获取帧缓冲区大小
     *
     *  @return 帧缓冲区大小
     */
    int size() { return mFrameBufferSize; }

    /** @brief 设置时间戳
     *
     *  @param pts 时间戳
     */
    void setPts(const int64_t &pts) { mPts = pts; }

    /** @brief 获取时间戳
     *
     *  @return 帧的时间戳
     */
    int64_t pts() { return mPts; }

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

    /** @brief 获取帧类型
     *
     *  @return 帧的类型
     */
    FrameType type() { return mType; }

protected:
    /** @brief 帧类型
     */
    FrameType mType;

    /** @brief 帧缓冲区
     */
    uint8_t *mFrameBuffer = nullptr;

    /** @brief 帧缓冲区大小
     */
    int mFrameBufferSize = 0;

    /** @brief 帧宽度
     */
    int mWidth;

    /** @brief 帧高度
     */
    int mHegiht;

    /** @brief 时间戳
     */
    int64_t mPts;

    /** @brief UTC 时间戳（毫秒）
     */
    uint64_t m_timestamp_ms = 0;
};
