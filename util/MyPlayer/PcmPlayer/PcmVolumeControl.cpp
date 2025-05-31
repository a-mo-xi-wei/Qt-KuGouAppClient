/*
  PcmVolumeControl - PCM 音量控制类实现
  Copyright (C) 2023
*/

#include "PcmVolumeControl.h"

typedef signed char int8_t; ///< 8 位有符号整数
typedef signed short int16_t; ///< 16 位有符号整数
typedef signed int int32_t; ///< 32 位有符号整数
typedef unsigned char uint8_t; ///< 8 位无符号整数
typedef unsigned short uint16_t; ///< 16 位无符号整数
typedef unsigned int uint32_t; ///< 32 位无符号整数
typedef unsigned long DWORD; ///< 无符号长整型
typedef int BOOL; ///< 布尔类型
typedef unsigned char BYTE; ///< 字节类型
typedef unsigned short WORD; ///< 字类型
typedef float FLOAT; ///< 浮点类型
typedef FLOAT *PFLOAT; ///< 浮点指针类型
typedef int INT; ///< 整型
typedef unsigned int UINT; ///< 无符号整型
typedef unsigned int *PUINT; ///< 无符号整型指针
typedef unsigned long ULONG_PTR, *PULONG_PTR; ///< 无符号长指针
typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR; ///< 无符号长指针
#define MAKEWORD(a, b) ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8)) ///< 构造 16 位字
#define MAKELONG(a, b) ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16)) ///< 构造 32 位长整型
#define LOWORD(l) ((WORD)(((DWORD_PTR)(l)) & 0xffff)) ///< 获取 32 位值的低 16 位
#define HIWORD(l) ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff)) ///< 获取 32 位值的高 16 位
#define LOBYTE(w) ((BYTE)(((DWORD_PTR)(w)) & 0xff)) ///< 获取 16 位值的低 8 位
#define HIBYTE(w) ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff)) ///< 获取 16 位值的高 8 位

/** @brief 构造函数
 *
 *  初始化 PcmVolumeControl 对象。
 */
PcmVolumeControl::PcmVolumeControl() {
}

/** @brief 调节音量
 *
 *  对指定的 PCM 音频数据块进行音量增益处理。
 *
 *  @param buf 音频数据块首地址
 *  @param size 数据块长度
 *  @param uRepeat 重复增益次数（通常为 1）
 *  @param vol 增益倍数（可小于 1）
 */
void PcmVolumeControl::RaiseVolume(char* buf, int size, int uRepeat, double vol) {
    if (!size) {
        return;
    }
    for (int i = 0; i < size; i += 2) {
        short wData;
        wData = MAKEWORD(buf[i], buf[i + 1]);
        long dwData = wData;
        for (int j = 0; j < uRepeat; j++) {
            dwData = dwData * vol;
            if (dwData < -0x8000) {
                dwData = -0x8000;
            } else if (dwData > 0x7FFF) {
                dwData = 0x7FFF;
            }
        }
        wData = LOWORD(dwData);
        buf[i] = LOBYTE(wData);
        buf[i + 1] = HIBYTE(wData);
    }
}