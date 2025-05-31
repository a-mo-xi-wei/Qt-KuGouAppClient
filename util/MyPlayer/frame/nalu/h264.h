/*
h264 - H.264 视频编码相关定义
*/

#ifndef H264_H
#define H264_H

#include <cstdlib>

#define NALU_TYPE_SLICE 1 ///< 非 IDR 图像的编码片
#define NALU_TYPE_DPA 2 ///< 数据分区 A
#define NALU_TYPE_DPB 3 ///< 数据分区 B
#define NALU_TYPE_DPC 4 ///< 数据分区 C
#define NALU_TYPE_IDR 5 ///< 即时解码刷新 (IDR) 图像
#define NALU_TYPE_SEI 6 ///< 补充增强信息 (SEI)
#define NALU_TYPE_SPS 7 ///< 序列参数集 (SPS)
#define NALU_TYPE_PPS 8 ///< 图像参数集 (PPS)
#define NALU_TYPE_AUD 9 ///< 访问单元分隔符
#define NALU_TYPE_EOSEQ 10 ///< 序列结束
#define NALU_TYPE_EOSTREAM 11 ///< 流结束
#define NALU_TYPE_FILL 12 ///< 填充数据

/** @struct T_H264_NALU
 *  @brief H.264 NAL 单元结构
 */
typedef struct {
    /** @brief 起始码长度
     *
     *  通常为 4（参数集和图像的第一个片）或 3（其他情况）。
     */
    int startcodeprefix_len;

    /** @brief NAL 单元长度
     *
     *  不包括起始码的长度。
     */
    unsigned len;

    /** @brief 缓冲区最大大小
     */
    unsigned max_size;

    /** @brief 禁止位
     *
     *  应始终为 FALSE。
     */
    int forbidden_bit;

    /** @brief NAL 参考优先级
     *
     *  表示 NAL 单元的重要性。
     */
    int nal_reference_idc;

    /** @brief NAL 单元类型
     *
     *  参见 NALU_TYPE_xxxx 定义。
     */
    int nal_unit_type;

    /** @brief NAL 单元数据
     *
     *  包含第一个字节和后续的 EBSP 数据。
     */
    unsigned char *buf;

    /** @brief 丢包标志
     *
     *  如果检测到丢包，则为 true。
     */
    unsigned short lost_packets;
} T_H264_NALU;

/** @struct T_H264_NALU_HEADER
 *  @brief H.264 NAL 单元头部
 *
 *  占用 1 字节，包含禁止位、参考优先级和单元类型。
 */
#pragma pack(1)
typedef struct {
    /** @brief NAL 单元类型
     *
     *  5 位，参见 NALU_TYPE_xxxx 定义。
     */
    unsigned char TYPE:5;

    /** @brief NAL 参考索引
     *
     *  2 位，表示参考优先级。
     */
    unsigned char NRI:2;

    /** @brief 禁止位
     *
     *  1 位，应始终为 0。
     */
    unsigned char F:1;
} T_H264_NALU_HEADER;
#pragma pack()

#endif // H264_H