/*
  VideoEncodedFrame - 编码视频帧管理类实现
*/

#include "VideoEncodedFrame.h"

#include "util/util.h"

/** @brief 构造函数
 *
 *  初始化 VideoEncodedFrame 对象，设置初始 UTC 时间戳。
 */
VideoEncodedFrame::VideoEncodedFrame() {
    m_timestamp_ms = Util::GetUtcTime();
}

/** @brief 析构函数
 *
 *  销毁 VideoEncodedFrame 对象，释放 NAL 单元资源。
 */
VideoEncodedFrame::~VideoEncodedFrame() {
    NALUParsing::FreeNALU(m_nalu);
    m_nalu = nullptr;
}

/** @brief 设置 NAL 单元
 *
 *  设置编码帧的 NAL 单元和时间戳。
 *
 *  @param nalu NAL 单元指针
 *  @param time 时间戳（毫秒）
 */
void VideoEncodedFrame::setNalu(T_NALU *nalu, const int64_t &time) {
    m_nalu = nalu;
    m_pts = time;
}

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
void VideoEncodedFrame::setNalu(uint8_t *buffer, const int &len, const bool &isAllocBuffer, const T_NALU_TYPE &type, const uint64_t &time) {
    T_NALU *nalu = NALUParsing::AllocNALU(len, type, isAllocBuffer);

    nalu->type = type;

    if (type == T_NALU_H264) {
        T_H264_NALU_HEADER *nalu_header = (T_H264_NALU_HEADER *)(buffer);

        nalu->nalu.h264Nalu.startcodeprefix_len = 4;      //! 4 for parameter sets and first slice in picture, 3 for everything else (suggested)
//        nalu->nalu.h264Nalu.len = len;                 //! Length of the NAL unit (Excluding the start code, which does not belong to the NALU)
        nalu->nalu.h264Nalu.forbidden_bit = 0;            //! should be always FALSE
        nalu->nalu.h264Nalu.nal_reference_idc = nalu_header->NRI;        //! NALU_PRIORITY_xxxx
        nalu->nalu.h264Nalu.nal_unit_type = nalu_header->TYPE;            //! NALU_TYPE_xxxx
        nalu->nalu.h264Nalu.lost_packets = false;  //! true, if packet loss is detected

        if (isAllocBuffer)
        {
            memcpy(nalu->nalu.h264Nalu.buf, buffer, len);  //! contains the first byte followed by the EBSP
        }
        else
        {
            nalu->nalu.h264Nalu.buf = buffer;
        }
        nalu->nalu.h264Nalu.max_size = len;
        nalu->nalu.h264Nalu.len = len;

//        {
//            char *bufTmp = (char*)(Buf + StartCode);
//            char s[10];
//            itoa(bufTmp[0], s, 2);
//            fprintf(stderr, "%s %08s %x %d\n", __FUNCTION__, s, bufTmp[0] , nalu_header->TYPE);
//        }
    }
    else
    {
        T_H265_NALU_HEADER *nalu_header = (T_H265_NALU_HEADER *)buffer;

        nalu->nalu.h265Nalu.startCodeLen = 4;      //! 4 for parameter sets and first slice in picture, 3 for everything else (suggested)
        nalu->nalu.h265Nalu.h265NaluHeader = *nalu_header;

        if (isAllocBuffer)
        {
            memcpy(nalu->nalu.h265Nalu.buf, buffer, len);  //! contains the first byte followed by the EBSP
        }
        else
        {
            nalu->nalu.h265Nalu.buf = buffer;
        }

//        nalu->nalu.h265Nalu.max_size = len;
        nalu->nalu.h265Nalu.len = len;

//        {
//            char *bufTmp = (char*)(buffer);
//            fprintf(stderr, "%s %02x%02x%02x%02x%02x%02x %d %d\n", __FUNCTION__, bufTmp[0], bufTmp[1], bufTmp[2], bufTmp[3], bufTmp[4], bufTmp[5], nalu->nalu.h265Nalu.h265NaluHeader.nal_unit_type, nalu_header->nal_unit_type);
//        }
    }

    if (m_nalu != nullptr)
    {
        NALUParsing::FreeNALU(m_nalu); //释放NALU内存
    }

    m_nalu = nalu;
    m_pts = time;
}

/** @brief 获取帧缓冲区
 *
 *  @return 帧缓冲区指针
 */
uint8_t *VideoEncodedFrame::buffer() {
    uint8_t *buffer = nullptr;

    do {
        if (!m_nalu) {
            break;
        }

        if (m_nalu->type == T_NALU_H264) {
            buffer = m_nalu->nalu.h264Nalu.buf;
        } else if (m_nalu->type == T_NALU_H265) {
            buffer = m_nalu->nalu.h265Nalu.buf;
        }

    } while (0);

    return buffer;
}

/** @brief 获取帧缓冲区大小
 *
 *  @return 帧缓冲区大小
 */
unsigned int VideoEncodedFrame::size() {
    unsigned int buffer_size = 0;

    do {
        if (!m_nalu) {
            break;
        }

        if (m_nalu->type == T_NALU_H264) {
            buffer_size = m_nalu->nalu.h264Nalu.len;
        } else if (m_nalu->type == T_NALU_H265) {
            buffer_size = m_nalu->nalu.h265Nalu.len;
        }

    } while (0);

    return buffer_size;
}