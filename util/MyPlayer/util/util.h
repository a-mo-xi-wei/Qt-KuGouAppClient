/*
util - 通用工具函数集合
*/

#pragma once

#include <string>
#include <cstdint>

#if defined(WIN32)
    #include <WinSock2.h>
    #include <Windows.h>
    #include <direct.h>
    #include <io.h> //C (Windows)    access
#else
    #include <sys/time.h>
    #include <stdio.h>
    #include <unistd.h>
#endif

#if defined(WIN32)
#else
    void Sleep(long mSeconds); ///< 跨平台睡眠函数（非 Windows 平台）
#endif

void mSleep(int mSecond); ///< 跨平台毫秒级睡眠函数

/** @namespace Util
 *  @brief 通用工具函数命名空间
 */
namespace Util {
    /** @brief 毫秒级睡眠
     *
     *  @param mSecond 睡眠时间（毫秒）
     */
    void mSleep(int mSecond);

    /** @brief 获取 UTC 时间戳
     *
     *  @return UTC 时间戳（毫秒）
     */
    uint64_t GetUtcTime();

    /** @brief 获取本地时间戳
     *
     *  @return 本地时间戳（毫秒）
     */
    uint64_t GetLocalTime();

    /** @brief 获取今天 0 点的 UTC 时间戳
     *
     *  @return 今天 0 点的 UTC 时间戳（毫秒）
     */
    uint64_t GetTodayUtcTime();

    /** @brief 获取当前时间相对于本周的 UTC 时间戳
     *
     *  @return 当前时间相对于本周的 UTC 时间戳（毫秒）
     */
    uint64_t GetUtcTimeInCurWeek();

    /** @brief 获取本地时间字符串
     *
     *  @return 格式化的本地时间字符串
     */
    std::string getLocalTimeInStringFormat();
};
