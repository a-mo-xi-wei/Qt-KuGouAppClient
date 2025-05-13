/*
  AACFrame - AAC 音频帧管理类
*/

#ifndef AACFRAME_H
#define AACFRAME_H

#include <cstdint>

/** @def ADTS_HEADER_LENTH
 *  @brief ADTS 头部长度
 *
 *  固定为 7 字节。
 */
#define ADTS_HEADER_LENTH 7

/*
sampling_frequency_index sampling frequeny [Hz]
0x0                           96000
0x1                           88200
0x2                           64000
0x3                           48000
0x4                           44100
0x5                           32000
0x6                           24000
0x7                           22050
0x8                           16000
0x9                           2000
0xa                           11025
0xb                           8000
0xc                           reserved
0xd                           reserved
0xe                           reserved
0xf                           reserved
*/
typedef struct
{
    unsigned int syncword;  //12 bslbf 同步字The bit string ‘1111 1111 1111’，说明一个ADTS帧的开始
    unsigned int id;        //1 bslbf   MPEG 标示符, 设置为1
    unsigned int layer;     //2 uimsbf Indicates which layer is used. Set to ‘00’
    unsigned int protection_absent;  //1 bslbf  表示是否误码校验
    unsigned int profile;            //2 uimsbf  表示使用哪个级别的AAC，如01 Low Complexity(LC)--- AACLC
    unsigned int sf_index;           //4 uimsbf  表示使用的采样率下标
    unsigned int private_bit;        //1 bslbf
    unsigned int channel_configuration;  //3 uimsbf  表示声道数
    unsigned int original;               //1 bslbf
    unsigned int home;                   //1 bslbf
    /*下面的为改变的参数即每一帧都不同*/
    unsigned int copyright_identification_bit;   //1 bslbf
    unsigned int copyright_identification_start; //1 bslbf
    unsigned int aac_frame_length;               // 13 bslbf  一个ADTS帧的长度包括ADTS头和raw data block
    unsigned int adts_buffer_fullness;           //11 bslbf     0x7FF 说明是码率可变的码流

    /*no_raw_data_blocks_in_frame 表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧.
    所以说number_of_raw_data_blocks_in_frame == 0
    表示说ADTS帧中有一个AAC数据块并不是说没有。(一个AAC原始帧包含一段时间内1024个采样及相关数据)
    */
    unsigned int no_raw_data_blocks_in_frame;    //2 uimsfb
} ADTS_HEADER;


/** @def AACFramePtr
 *  @brief AACFrame 的智能指针类型
 *
 *  定义为 AACFrame 类的 std::shared_ptr。
 */
#define AACFramePtr std::shared_ptr<AACFrame>

/** @class AACFrame
 *  @brief 管理 AAC 音频帧的类
 *
 *  该类用于存储和管理 AAC 音频帧数据，包括 ADTS 头部、帧缓冲区和时间戳。
 */
class AACFrame {
public:
    /** @brief 构造函数
     *
     *  初始化 AACFrame 对象。
     */
    AACFrame();

    /** @brief 析构函数
     *
     *  销毁 AACFrame 对象，释放缓冲区资源。
     */
    ~AACFrame();

    /** @brief 设置 ADTS 头部
     *
     *  设置 AAC 帧的 ADTS 头部信息。
     *
     *  @param adts ADTS 头部结构
     */
    void setAdtsHeader(const ADTS_HEADER &adts);

    /** @brief 设置帧缓冲区
     *
     *  设置 AAC 音频帧的缓冲区数据。
     *
     *  @param buffer 缓冲区数据
     *  @param size 缓冲区大小
     */
    void setFrameBuffer(const uint8_t *const buffer, const unsigned int &size);

    /** @brief 设置帧缓冲区（包含 ADTS 头部）
     *
     *  设置 AAC 音频帧的 ADTS 头部和数据缓冲区。
     *
     *  @param adtsBuffer ADTS 头部缓冲区
     *  @param adtsSize ADTS 头部大小
     *  @param buffer 数据缓冲区
     *  @param size 数据缓冲区大小
     */
    void setFrameBuffer(const uint8_t *const adtsBuffer, const unsigned int &adtsSize, const uint8_t *const buffer, const unsigned int &size);

    /** @brief 获取帧缓冲区
     *
     *  @return 帧缓冲区指针（包含 ADTS 头部）
     */
    uint8_t *buffer() { return mFrameBuffer; }

    /** @brief 获取帧缓冲区大小
     *
     *  @return 帧缓冲区大小（包含 ADTS 头部）
     */
    unsigned int size() { return mFrameBufferSize; }

    /** @brief 设置时间戳
     *
     *  @param pts 时间戳
     */
    void setPts(uint32_t pts) { m_pts = pts; }

    /** @brief 获取时间戳
     *
     *  @return 帧的时间戳
     */
    uint32_t pts() { return m_pts; }

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

private:
    /** @brief ADTS 头部信息
     */
    ADTS_HEADER mAdtsHeader;

    /** @brief AAC 数据缓冲区（包含 ADTS 头部）
     */
    uint8_t *mFrameBuffer;

    /** @brief AAC 数据缓冲区大小（包含 ADTS 头部）
     */
    unsigned int mFrameBufferSize;

    /** @brief 时间戳
     */
    uint32_t m_pts = 0;

    /** @brief UTC 时间戳（毫秒）
     */
    uint64_t m_timestamp_ms = 0;
};

#endif // AACFRAME_H
