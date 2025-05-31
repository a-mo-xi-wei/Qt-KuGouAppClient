/*
  util - 通用工具函数集合实现
*/

#include "util.h"

#include <ctime>

#if defined(WIN32)
    #include <WinSock2.h>
    #include <Windows.h>
    #include <direct.h>
    #include <io.h>
#else
    #include <sys/time.h>
    #include <stdio.h>
    #include <unistd.h>

    /** @brief 跨平台睡眠函数（非 Windows 平台）
     *
     *  @param mSeconds 睡眠时间（毫秒）
     */
    void Sleep(long mSeconds) {
        usleep(mSeconds * 1000);
    }
#endif

/** @brief 跨平台毫秒级睡眠函数
 *
 *  @param mSecond 睡眠时间（毫秒）
 */
void mSleep(int mSecond) {
#if defined(WIN32)
    Sleep(mSecond);
#else
    usleep(mSecond * 1000);
#endif
}

/** @namespace Util
 *  @brief 通用工具函数命名空间
 */
namespace Util {

    /** @brief 获取 UTC 时间戳
     *
     *  @return UTC 时间戳（毫秒）
     */
    uint64_t GetUtcTime() {
        struct timeval tv;
        mingw_gettimeofday(&tv, NULL);
        uint64_t timestamp = (uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
        return timestamp;
    }

    /** @brief 获取本地时间戳
     *
     *  @return 本地时间戳（毫秒）
     */
    uint64_t GetLocalTime() {
        struct timeval tv;
        mingw_gettimeofday(&tv, NULL);
        uint64_t current_time_ms = (uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
        return current_time_ms;
    }

    /** @brief 获取今天 0 点的 UTC 时间戳
     *
     *  @return 今天 0 点的 UTC 时间戳（毫秒）
     */
    uint64_t GetTodayUtcTime() {
        time_t ts = time(nullptr);
        struct tm* tUTC = gmtime(&ts);
        tUTC->tm_hour = 0;
        tUTC->tm_min = 0;
        tUTC->tm_sec = 0;
        uint64_t timestamp = (uint64_t)mktime(tUTC) * 1000;
        return timestamp;
    }

    /** @brief 获取当前时间相对于本周的 UTC 时间戳
     *
     *  @return 当前时间相对于本周的 UTC 时间戳（毫秒）
     */
    uint64_t GetUtcTimeInCurWeek() {
        struct timeval tv;
        mingw_gettimeofday(&tv, NULL);
        uint64_t timestamp = tv.tv_usec / 1000;
        time_t ts = time(nullptr);
        struct tm* tUTC = gmtime(&ts);
        timestamp += (tUTC->tm_hour * 3600000 + tUTC->tm_min * 60000 + tUTC->tm_sec * 1000);
        timestamp += tUTC->tm_wday * 86400000;
        return timestamp;
    }

};