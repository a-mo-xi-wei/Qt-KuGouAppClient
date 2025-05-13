/*
  nalu - NAL 单元解析类
*/

#ifndef NALU_H
#define NALU_H

#include <cstdint>

#include "h264.h"
#include "h265.h"

/** @enum T_NALU_TYPE
 *  @brief NAL 单元类型
 */
enum T_NALU_TYPE {
    T_NALU_H264 = 0, ///< H.264 视频流
    T_NALU_H265      ///< H.265 视频流
};

/** @struct T_NALU
 *  @brief NAL 单元结构
 *
 *  包含 H.264 或 H.265 的 NAL 单元数据。
 */
typedef struct {
    /** @brief NAL 单元类型
     */
    T_NALU_TYPE type;

    /** @brief NAL 单元数据
     *
     *  联合体，存储 H.264 或 H.265 的 NAL 单元。
     */
    union {
        T_H264_NALU h264Nalu; /**< H.264 NAL 单元 */
        T_H265_NALU h265Nalu; /**< H.265 NAL 单元 */
    } nalu;
} T_NALU;

/** @class NALUParsing
 *  @brief 解析 H.264/H.265 视频流中的 NAL 单元
 *
 *  该类用于从连续的 H.264 或 H.265 数据流中解析出 NAL 单元。
 */
class NALUParsing {
public:
    /** @brief 构造函数
     *
     *  初始化 NAL 单元解析器，分配初始缓冲区。
     */
    NALUParsing();

    /** @brief 析构函数
     *
     *  销毁 NAL 单元解析器，释放缓冲区资源。
     */
    ~NALUParsing();

    /** @brief 输入视频数据
     *
     *  将 H.264 或 H.265 数据输入解析器。
     *
     *  @param type 视频类型（H.264 或 H.265）
     *  @param buf 输入数据缓冲区
     *  @param len 输入数据长度
     *  @param clear_old_buffer 是否清除已有缓冲区
     *  @return 当前缓冲区总大小
     */
    int inputData(T_NALU_TYPE type, uint8_t *buf, int len, bool clear_old_buffer);

    /** @brief 获取下一帧
     *
     *  从缓冲区中解析并返回一个完整的 NAL 单元。
     *
     *  @return NAL 单元指针，若无完整帧则返回 nullptr
     */
    T_NALU* getNextFrame();

    /** @brief 获取 IDR 帧头部信息
     *
     *  提取 IDR 帧的头部信息（VPS、SPS、PPS）。
     *
     *  @param buf 输出缓冲区
     *  @param buf_len 输出缓冲区长度
     *  @return 0 表示成功
     */
    int getIDRHeaderBuffer(uint8_t *buf, unsigned int &buf_len);

    /** @brief 分配 NAL 单元
     *
     *  为 NAL 单元分配内存。
     *
     *  @param buffer_size 缓冲区大小
     *  @param type 视频类型（H.264 或 H.265）
     *  @param is_alloc_buffer 是否分配缓冲区
     *  @return 分配的 NAL 单元指针
     */
    static T_NALU *AllocNALU(const int &buffer_size, const T_NALU_TYPE &type, const bool &is_alloc_buffer = true);

    /** @brief 释放 NAL 单元
     *
     *  释放 NAL 单元及其缓冲区资源。
     *
     *  @param n NAL 单元指针
     */
    static void FreeNALU(T_NALU *n);

private:
    /** @brief 数据缓冲区
     */
    uint8_t *m_buffer = nullptr;

    /** @brief 当前缓冲区大小
     */
    int m_buffer_size = 0;

    /** @brief 缓冲区最大大小
     */
    int m_buffer_max_size = 0;

    /** @brief 视频类型
     *
     *  区分 H.264 或 H.265。
     */
    T_NALU_TYPE m_video_type;
};

#endif // NALU_H