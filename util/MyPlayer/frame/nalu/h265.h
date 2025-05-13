/*
  h265 - H.265 视频编码相关定义
*/

#ifndef H265_H
#define H265_H

#include <cstdlib>

/** @enum E_HEVC_NALU_TYPE
 *  @brief H.265 NAL 单元类型
 */
typedef enum e_hevc_nalu_type {
    HEVC_NAL_TRAIL_N    = 0,  ///< 非参考跟踪帧
    HEVC_NAL_TRAIL_R    = 1,  ///< 参考跟踪帧
    HEVC_NAL_TSA_N      = 2,  ///< 非参考时间子层访问
    HEVC_NAL_TSA_R      = 3,  ///< 参考时间子层访问
    HEVC_NAL_STSA_N     = 4,  ///< 非参考步进时间子层访问
    HEVC_NAL_STSA_R     = 5,  ///< 参考步进时间子层访问
    HEVC_NAL_RADL_N     = 6,  ///< 非参考随机访问解码引导
    HEVC_NAL_RADL_R     = 7,  ///< 参考随机访问解码引导
    HEVC_NAL_RASL_N     = 8,  ///< 非参考随机访问跳跃
    HEVC_NAL_RASL_R     = 9,  ///< 参考随机访问跳跃
    HEVC_NAL_VCL_N10    = 10, ///< 非参考视频编码层 10
    HEVC_NAL_VCL_R11    = 11, ///< 参考视频编码层 11
    HEVC_NAL_VCL_N12    = 12, ///< 非参考视频编码层 12
    HEVC_NAL_VCL_R13    = 13, ///< 参考视频编码层 13
    HEVC_NAL_VCL_N14    = 14, ///< 非参考视频编码层 14
    HEVC_NAL_VCL_R15    = 15, ///< 参考视频编码层 15
    HEVC_NAL_BLA_W_LP   = 16, ///< 带引导图片的断裂链接访问
    HEVC_NAL_BLA_W_RADL = 17, ///< 带随机访问解码引导的断裂链接访问
    HEVC_NAL_BLA_N_LP   = 18, ///< 无引导图片的断裂链接访问
    HEVC_NAL_IDR_W_RADL = 19, ///< 带随机访问解码引导的 IDR
    HEVC_NAL_IDR_N_LP   = 20, ///< 无引导图片的 IDR
    HEVC_NAL_CRA_NUT    = 21, ///< 干净随机访问
    HEVC_NAL_IRAP_VCL22 = 22, ///< 帧内随机访问点 22
    HEVC_NAL_IRAP_VCL23 = 23, ///< 帧内随机访问点 23
    HEVC_NAL_RSV_VCL24  = 24, ///< 保留视频编码层 24
    HEVC_NAL_RSV_VCL25  = 25, ///< 保留视频编码层 25
    HEVC_NAL_RSV_VCL26  = 26, ///< 保留视频编码层 26
    HEVC_NAL_RSV_VCL27  = 27, ///< 保留视频编码层 27
    HEVC_NAL_RSV_VCL28  = 28, ///< 保留视频编码层 28
    HEVC_NAL_RSV_VCL29  = 29, ///< 保留视频编码层 29
    HEVC_NAL_RSV_VCL30  = 30, ///< 保留视频编码层 30
    HEVC_NAL_RSV_VCL31  = 31, ///< 保留视频编码层 31
    HEVC_NAL_VPS        = 32, ///< 视频参数集
    HEVC_NAL_SPS        = 33, ///< 序列参数集
    HEVC_NAL_PPS        = 34, ///< 图像参数集
    HEVC_NAL_AUD        = 35, ///< 访问单元分隔符
    HEVC_NAL_EOS_NUT    = 36, ///< 序列结束
    HEVC_NAL_EOB_NUT    = 37, ///< 比特流结束
    HEVC_NAL_FD_NUT     = 38, ///< 填充数据
    HEVC_NAL_SEI_PREFIX = 39, ///< 前缀补充增强信息
    HEVC_NAL_SEI_SUFFIX = 40  ///< 后缀补充增强信息
} E_HEVC_NALU_TYPE;

/** @struct T_H265_NALU_HEADER
 *  @brief H.265 NAL 单元头部
 *
 *  定义 H.265 NAL 单元头部结构，包含禁止位、单元类型、层 ID 和时间 ID。
 */
#pragma pack(1)
typedef struct t_h265_nalu_header {
    /** @brief 禁止位
     *
     *  1 位，应始终为 0。
     */
    unsigned char forbidden_zero_bit:1;

    /** @brief NAL 单元类型
     *
     *  6 位，参见 E_HEVC_NALU_TYPE 定义。
     */
    unsigned char nal_unit_type:6;

    /** @brief 层 ID
     *
     *  6 位，表示层级 ID。
     */
    unsigned char nuh_layer_id:6;

    /** @brief 时间 ID 加 1
     *
     *  3 位，表示时间层 ID 加 1。
     */
    unsigned char nuh_temporal_id_plus1:3;
} T_H265_NALU_HEADER;

/** @struct T_H265_NALU
 *  @brief H.265 NAL 单元结构
 */
typedef struct t_h265_nalu {
    /** @brief 起始码长度
     *
     *  通常为 4（参数集和图像的第一个片）或 3（其他情况）。
     */
    int startCodeLen;

    /** @brief NAL 单元头部
     */
    T_H265_NALU_HEADER h265NaluHeader;

    /** @brief NAL 单元长度
     *
     *  包含起始码的长度。
     */
    unsigned int len;

    /** @brief NAL 单元数据
     *
     *  包含起始码和后续的 EBSP 数据。
     */
    unsigned char *buf;
} T_H265_NALU;
#pragma pack()

#endif // H265_H