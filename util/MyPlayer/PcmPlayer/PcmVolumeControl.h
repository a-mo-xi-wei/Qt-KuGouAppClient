/*
*  PcmVolumeControl - PCM 音量控制类
 *  叶海辉
 *  QQ群121376426
 *  http://blog.yundiantech.com/
 */

#ifndef PCMVOLUMECONTROL_H
#define PCMVOLUMECONTROL_H

/** @class PcmVolumeControl
 *  @brief PCM 音量控制类
 *
 *  该类提供静态方法用于调节 PCM 音频数据的音量。
 */
class PcmVolumeControl {
public:
    /** @brief 构造函数
     *
     *  初始化 PcmVolumeControl 对象。
     */
    PcmVolumeControl();

    /** @brief 调节音量
     *
     *  对指定的 PCM 音频数据块进行音量增益处理。
     *
     *  @param buf 音频数据块首地址
     *  @param size 数据块长度
     *  @param uRepeat 重复增益次数（通常为 1）
     *  @param vol 增益倍数（可小于 1）
     */
    static void RaiseVolume(char* buf, int size, int uRepeat, double vol);
};

#endif // PCMVOLUMECONTROL_H