/*
  nalu - NAL 单元解析类实现
*/

#include "nalu.h"

#include <cstring>

/** @brief 构造函数
 *
 *  初始化 NAL 单元解析器，分配初始缓冲区（300 KB）。
 */
NALUParsing::NALUParsing() {
    m_buffer_max_size = 300 * 1024;
    m_buffer = (uint8_t*)malloc(m_buffer_max_size);
    m_buffer_size = 0;
}

/** @brief 析构函数
 *
 *  销毁 NAL 单元解析器，释放缓冲区资源。
 */
NALUParsing::~NALUParsing() {
    free(m_buffer);
}

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
int NALUParsing::inputData(T_NALU_TYPE type, uint8_t *buf, int len, bool clear_old_buffer) {
    if (clear_old_buffer) {
        m_buffer_size = 0;
    }

    if ((m_buffer_size + len) > m_buffer_max_size) {
        m_buffer_max_size = m_buffer_size + len;
        realloc(m_buffer, m_buffer_max_size);
    }

    memcpy(m_buffer + m_buffer_size, buf, len);
    m_buffer_size += len;

    m_video_type = type;

    return m_buffer_size;
}

/** @brief 获取下一帧
 *
 *  从缓冲区中解析并返回一个完整的 NAL 单元，基于 H.264/H.265 的起始码（0x00000001 或 0x000001）。
 *
 *  @return NAL 单元指针，若无完整帧则返回 nullptr
 */
T_NALU *NALUParsing::getNextFrame() {
    /*根据h264文件的特性  逐个字节搜索 直到遇到h264的帧头 视为获取到了完整的一帧h264视频数据*/

///    关于起始码startcode的两种形式：3字节的0x000001和4字节的0x00000001
///    3字节的0x000001只有一种场合下使用，就是一个完整的帧被编为多个slice的时候，
///    包含这些slice的nalu使用3字节起始码。其余场合都是4字节的。
///    因此查找一帧的话，只需要查找四字节的起始码即可。

    ///首先查找第一个起始码

    int pos = 0; //记录当前处理的数据偏移量
    int StartCode = 0;

    while (true) {
        unsigned char* Buf = m_buffer + pos;
        int lenth = m_buffer_size - pos; //剩余没有处理的数据长度
        if (lenth <= 4) {
            return nullptr;
        }
        ///查找起始码(0x00000001)
        if (Buf[0] == 0 && Buf[1] == 0 && Buf[2] == 1) {
            StartCode = 3;
            break;
        } else if (Buf[0] == 0 && Buf[1] == 0 && Buf[2] == 0 && Buf[3] == 1) {
            StartCode = 4;
            break;
        } else {
            //否则 往后查找一个字节
            pos++;
        }
    }
    ///然后查找下一个起始码查找第一个起始码
    int pos_2 = pos + StartCode; //记录当前处理的数据偏移量
    int StartCode_2 = 0;

    while (true) {
        unsigned char* Buf = m_buffer + pos_2;
        int lenth = m_buffer_size - pos_2; //剩余没有处理的数据长度
        if (lenth <= 4) {
            return nullptr;
        }
        ///查找起始码(0x00000001)
        if (Buf[0] == 0 && Buf[1] == 0 && Buf[2] == 1) {
            StartCode_2 = 3;
            break;
        } else if (Buf[0] == 0 && Buf[1] == 0 && Buf[2] == 0 && Buf[3] == 1) {
            StartCode_2 = 4;
            break;
        } else {
            //否则 往后查找一个字节
            pos_2++;
        }
    }

    /// 现在 pos和pos_2之间的数据就是一帧数据了
    /// 把他取出来

    ///由于传递给ffmpeg解码的数据 需要带上起始码 因此这里的nalu带上了起始码
    unsigned char* Buf = m_buffer + pos; //这帧数据的起始数据(包含起始码)
    int naluSize = pos_2 - pos; //nalu数据大小 包含起始码

    T_NALU *nalu = AllocNALU(naluSize, m_video_type);

    if (m_video_type == T_NALU_H264) {
        T_H264_NALU_HEADER *nalu_header = (T_H264_NALU_HEADER *)(Buf + StartCode);

        nalu->nalu.h264Nalu.startcodeprefix_len = StartCode;      //! 4 for parameter sets and first slice in picture, 3 for everything else (suggested)
        nalu->nalu.h264Nalu.len = naluSize;                 //nalu数据大小 包含起始码
        nalu->nalu.h264Nalu.forbidden_bit = 0;            //! should be always FALSE
        nalu->nalu.h264Nalu.nal_reference_idc = nalu_header->NRI;        //! NALU_PRIORITY_xxxx
        nalu->nalu.h264Nalu.nal_unit_type = nalu_header->TYPE;            //! NALU_TYPE_xxxx
        nalu->nalu.h264Nalu.lost_packets = false;  //! true, if packet loss is detected
        memcpy(nalu->nalu.h264Nalu.buf, Buf, naluSize);  //! contains the first byte followed by the EBSP

//        {
//            char *bufTmp = (char*)(Buf + StartCode);
//            char s[10];
//            itoa(bufTmp[0], s, 2);
//            fprintf(stderr, "%s %08s %x nalu_header->TYPE=%d naluSize=%d\n", __FUNCTION__, s, bufTmp[0] , nalu_header->TYPE, naluSize);
//        }
    }
    else
    {
        T_H265_NALU_HEADER *nalu_header = (T_H265_NALU_HEADER *)(Buf + StartCode);

        nalu->nalu.h265Nalu.startCodeLen = StartCode;      //! 4 for parameter sets and first slice in picture, 3 for everything else (suggested)
        nalu->nalu.h265Nalu.len = naluSize;                 //nalu数据大小 包含起始码
        nalu->nalu.h265Nalu.h265NaluHeader = *nalu_header;
        memcpy(nalu->nalu.h265Nalu.buf, Buf, naluSize);  //! contains the first byte followed by the EBSP

//        {
//            char *bufTmp = (char*)(Buf);
//            fprintf(stderr, "%s %02x%02x%02x%02x%02x%02x %d %d\n", __FUNCTION__, bufTmp[0], bufTmp[1], bufTmp[2], bufTmp[3], bufTmp[4], bufTmp[5], nalu->nalu.h265Nalu.h265NaluHeader.nal_unit_type, nalu_header->nal_unit_type);
//        }
    }

    /// 将这一帧数据去掉
    /// 把后一帧数据覆盖上来
    int leftSize = m_buffer_size - pos_2;
    memmove(m_buffer, m_buffer + pos_2, leftSize);
    m_buffer_size = leftSize;

    return nalu;
}

/** @brief 获取 IDR 帧头部信息
 *
 *  提取 IDR 帧的头部信息（H.264 的 SPS/PPS 或 H.265 的 VPS/SPS/PPS）。
 *
 *  @param buf 输出缓冲区
 *  @param buf_len 输出缓冲区长度
 *  @return 0 表示成功
 */
int NALUParsing::getIDRHeaderBuffer(uint8_t *buf, unsigned int &buf_len) {
    int ret = 0;
    buf_len = 0;

//    FILE *fp1 = fopen("1.h265", "wb");
//    fwrite(m_buffer, 1, m_buffer_size, fp1);
//    fclose(fp1);

    while (true)
    {
        T_NALU *nalu = getNextFrame();

        if (nalu == nullptr) {
            break;
        }

        int nal_unit_type = 0;
        uint8_t *nalu_buf = nullptr;
        int nalu_size = 0;

        if (nalu->type == T_NALU_H264) {
            nal_unit_type = nalu->nalu.h264Nalu.nal_unit_type;
            nalu_buf = nalu->nalu.h264Nalu.buf;
            nalu_size = nalu->nalu.h264Nalu.len;

            if (nal_unit_type == NALU_TYPE_SPS || nal_unit_type == NALU_TYPE_PPS) {
                memcpy(buf + buf_len, nalu_buf, nalu_size);
                buf_len += nalu_size;
            }
        } else if (nalu->type == T_NALU_H265) {
            nal_unit_type = nalu->nalu.h265Nalu.h265NaluHeader.nal_unit_type;
            nalu_buf = nalu->nalu.h265Nalu.buf;
            nalu_size = nalu->nalu.h265Nalu.len;

            //32:vps 33:sps 34:pps
            if (nal_unit_type == HEVC_NAL_VPS || nal_unit_type == HEVC_NAL_SPS || nal_unit_type == HEVC_NAL_PPS)
            {
                memcpy(buf + buf_len, nalu_buf, nalu_size);
                buf_len += nalu_size;
            }
        }

        FreeNALU(nalu);

//        printf("%s %d nal_unit_type=%d %d %d\n", __FILE__, __LINE__, nal_unit_type, nalu_size, m_buffer_size);
    }

//    FILE *fp2 = fopen("1.vps", "wb");
//    fwrite(buf, 1, buf_len, fp2);
//    fclose(fp2);

    return 0;
}

/** @brief 分配 NAL 单元
 *
 *  为 NAL 单元分配内存。
 *
 *  @param buffer_size 缓冲区大小
 *  @param type 视频类型（H.264 或 H.265）
 *  @param is_alloc_buffer 是否分配缓冲区
 *  @return 分配的 NAL 单元指针
 */
T_NALU *NALUParsing::AllocNALU(const int &buffer_size, const T_NALU_TYPE &type, const bool &is_alloc_buffer) {
    T_NALU *n = nullptr;

    n = (T_NALU*)malloc(sizeof(T_NALU));

    n->type = type;

    if (type == T_NALU_H264) {
        if (is_alloc_buffer) {
            n->nalu.h264Nalu.max_size = buffer_size;	//Assign buffer size
            n->nalu.h264Nalu.buf = (unsigned char*)malloc(buffer_size);
            n->nalu.h264Nalu.len = buffer_size;
        } else {
            n->nalu.h264Nalu.max_size = 0;	//Assign buffer size
            n->nalu.h264Nalu.buf = nullptr;
            n->nalu.h264Nalu.len = 0;
        }
    } else {
        if (is_alloc_buffer) {
            n->nalu.h265Nalu.buf = (unsigned char*)malloc(buffer_size);
            n->nalu.h265Nalu.len = buffer_size;
        } else {
            n->nalu.h265Nalu.buf = nullptr;
            n->nalu.h265Nalu.len = 0;
        }
    }

    return n;
}

/** @brief 释放 NAL 单元
 *
 *  释放 NAL 单元及其缓冲区资源。
 *
 *  @param n NAL 单元指针
 */
void NALUParsing::FreeNALU(T_NALU *n) {
    if (n == nullptr) return;

    if (n->type == T_NALU_H264) {
        if (n->nalu.h264Nalu.buf != nullptr) {
            free(n->nalu.h264Nalu.buf);
        }
    } else {
        if (n->nalu.h265Nalu.buf != nullptr) {
            free(n->nalu.h265Nalu.buf);
        }
    }

    free(n);
}