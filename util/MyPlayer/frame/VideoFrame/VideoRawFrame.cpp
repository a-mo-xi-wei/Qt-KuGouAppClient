/*
  VideoRawFrame - 原始视频帧管理类实现
*/

#include "VideoRawFrame.h"
#include <cstdio>

#include "util/util.h"

#ifdef ENABLE_FFMPEG
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavutil/imgutils.h>
}
#endif

/** @brief 构造函数
 *
 *  初始化 VideoRawFrame 对象，设置初始 UTC 时间戳。
 */
VideoRawFrame::VideoRawFrame() {
    mFrameBuffer = nullptr;
    mFrameBufferSize = 0;
    mPts = 0;
    m_timestamp_ms = Util::GetUtcTime();
}

/** @brief 析构函数
 *
 *  销毁 VideoRawFrame 对象，释放缓冲区资源。
 */
VideoRawFrame::~VideoRawFrame() {
    if (mFrameBuffer != nullptr) {
        free(mFrameBuffer);
        mFrameBuffer = nullptr;
        mFrameBufferSize = 0;
    }
}

/** @brief 初始化缓冲区
 *
 *  根据宽度、高度和帧类型初始化帧缓冲区。
 *
 *  @param width 帧宽度
 *  @param height 帧高度
 *  @param type 帧类型
 *  @param time 时间戳（毫秒）
 */
void VideoRawFrame::initBuffer(const int &width, const int &height, const FrameType &type, int64_t time) {
    if (mFrameBuffer != nullptr) {
        free(mFrameBuffer);
        mFrameBuffer = nullptr;
    }

    mWidth = width;
    mHegiht = height;

    mPts = time;

    mType = type;

    int size = 0;
    if (type == FRAME_TYPE_YUV420P) {
#ifdef ENABLE_FFMPEG
        size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, width, height, 1);  ///< 按1字节进行内存对齐,得到的内存大小最接近实际大小
#else
        size = width * height * 3 / 2;
#endif
    } else if (type == FRAME_TYPE_RGB8) {
        size = width * height;
    } else if (type == FRAME_TYPE_RGB24) {
        size = width * height * 3;
    }

    mFrameBuffer = (uint8_t*)malloc(size);
    mFrameBufferSize = size;
}

/** @brief 设置帧缓冲区
 *
 *  设置整个帧的缓冲区数据。
 *
 *  @param buf 缓冲区数据
 */
void VideoRawFrame::setFramebuf(const uint8_t *buf) {
    if (mFrameBuffer && buf) {
        memcpy(mFrameBuffer, buf, mFrameBufferSize);
    } else {
        printf("%s line=%d setFramebuf error!\n", __FUNCTION__, __LINE__);
    }
}

/** @brief 设置 Y 分量缓冲区
 *
 *  设置 YUV 帧的 Y 分量数据。
 *
 *  @param buf Y 分量数据
 */
void VideoRawFrame::setYbuf(const uint8_t *buf) {
    int Ysize = mWidth * mHegiht;
    memcpy(mFrameBuffer, buf, Ysize);
}

/** @brief 设置 U 分量缓冲区
 *
 *  设置 YUV 帧的 U 分量数据。
 *
 *  @param buf U 分量数据
 */
void VideoRawFrame::setUbuf(const uint8_t *buf) {
    int Ysize = mWidth * mHegiht;
    memcpy(mFrameBuffer + Ysize, buf, Ysize / 4);
}

/** @brief 设置 V 分量缓冲区
 *
 *  设置 YUV 帧的 V 分量数据。
 *
 *  @param buf V 分量数据
 */
void VideoRawFrame::setVbuf(const uint8_t *buf) {
    int Ysize = mWidth * mHegiht;
    memcpy(mFrameBuffer + Ysize + Ysize / 4, buf, Ysize / 4);
}