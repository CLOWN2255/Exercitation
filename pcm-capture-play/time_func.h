/*
    2017.08.11 create by aflyeaglenku
    2017.08.30 modify by aflyeaglenku
*/
#ifndef time_func_h
#define time_func_h
#include "stdintx.h"
#include <time.h>
#include <stdio.h>
#include <sys/timeb.h>
//#include "npthread.h"
#pragma warning(disable: 4244)
#define _1_SECOND_MS 1000
#define _1_SECOND_US 1000000
#define _1_SECOND_NS 1000000000


#ifdef WIN32
#include <Windows.h>
#define    CLOCK_REALTIME_COARSE 5  /* linux 值 */


inline uint64_t gettime_s();

inline int clock_gettime(int, struct timeval *tv)
{
    /*
    精确到毫秒的 GetSystemTime / GetLocalTime

    Windows SDK 中有两个精确到毫秒的获取当前时间的函数：

        - GetSystemTime：获取 UTC 时间。
        - GetLocalTime：获取当地时间。

    这两个函数的返回都是：SYSTEMTIME 
    这个结构体占用了 16 个字节，它的定义如下：

    typedef struct _SYSTEMTIME {
        WORD wYear;
        WORD wMonth;
        WORD wDayOfWeek;
        WORD wDay;
        WORD wHour;
        WORD wMinute;
        WORD wSecond;
        WORD wMilliseconds;
    } SYSTEMTIME, *PSYSTEMTIME;

    其中 wYear 表示的范围是：从 1601 到 30827。范围很广了，应该能用到天荒地老了。:-)
    */
    /*
    精确到 100 纳秒的 GetSystemTimeAsFileTime

    究竟能不能达到 100 纳秒的精确度呢？在 Windows SDK 中有一个结构体：FILETIME，它可以记录精度达到 100ns 的时间。用哪个函数得到这个值呢？可以用 GetSystemTimeAsFileTime。

    但是，不要高兴得太早，虽然 FILETIME 能够达到如此高的精度，但是这个函数我连试都懒得试。为什么呢？因为 Windows 系统并不是一个实时操作系统（Windows Embedded Compact 2013 是一个实时系统），其时钟精度一般认为是 15 ~ 16 毫秒。

    Windows Sysinternals 给我们提供了一个查看你使用的 Windows 系统的时钟分辨率的小工具：ClockRes v2.0。把它下载下来在控制台中执行，结果如下：
    
    __int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
    wintime -= 116444736000000000i64;  //1jan1601 to 1jan1970

    tv->tv_sec = wintime / 10000000i64;           //seconds
    tv->tv_usec = wintime % 10000000i64 / 10;      //nano-seconds
    */

    SYSTEMTIME local_time = { 0 };
    GetLocalTime(&local_time);
    tv->tv_sec = gettime_s();
    tv->tv_usec = local_time.wMilliseconds*1000;
    return 0;
}

//inline int clock_gettime(int, struct timespec *spec)
//{
//    /*
//    __int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
//
//    printf("wintime = %lld\r\n",wintime);
//
//    wintime -= 116444736000000000i64;  //1jan1601 to 1jan1970
//    spec->tv_sec = wintime / 10000000i64;           //seconds
//    spec->tv_nsec = wintime % 10000000i64 * 100;      //nano-seconds
//    */
//
//    SYSTEMTIME local_time = { 0 };
//    GetLocalTime(&local_time);
//    spec->tv_sec = gettime_s();
//    spec->tv_nsec = local_time.wMilliseconds*1000*1000;
//
//    return 0;
//}

#endif

#define LTMSEL uint64_t
/*
1s = 1000 ms
   = 1000*1000 us
   = 1000*1000*1000 ns
*/
/*
获取从1970-0-0 0:0:0到现在的秒
在windows下，time_t占8字节。
*/
inline  uint64_t gettime_s()
{
    return time(0);
}

/*
获取从1970-0-0 0:0:0到现在的毫秒
*/
inline uint64_t gettime_ms()
{
    struct timeb t1;
    ftime(&t1);
    uint64_t tms = t1.time * 1000 + t1.millitm;
    return tms;
    /*
    获取整型毫秒数的另一种方法：
    time(0)*1000 + SYSTEMTIME.wMilliseconds
    */
}
//
//inline uint64_t GetMsel() //兼容网力函数
//{
//#ifdef WIN32
//    uint64_t ft;
//    GetSystemTimeAsFileTime((FILETIME*)&ft);
//    /* 116444736000000000i64: Number of 100 nanosecond units from 1/1/1601 to 1/1/1970 */
//    return (ft - (116444736000000000i64 + 5000i64)) / 10000i64;
//#else
//    static int clock_id = 5; /* CLOCK_REALTIME_COARSE */
//    struct timespec tp;
//    if(0 == clock_gettime(clock_id, &tp))
//        return (LTMSEL)tp.tv_sec*1000 + (tp.tv_nsec+(500*1000))/(1000*1000);
//    clock_id = CLOCK_REALTIME;
//    clock_gettime(clock_id, &tp);
//    return (LTMSEL)tp.tv_sec*1000 + (tp.tv_nsec+(500*1000))/(1000*1000);
////#else
////    /* 某些平台可能需要下面的旧的实现，此法效率要低一些 */
////    struct timeval tv;
////    gettimeofday(&tv, NULL);
////    return (LTMSEL)tv.tv_sec*1000 + (tv.tv_usec+500)/1000;
//#endif
//}

/*
获取从1970-0-0 0:0:0到现在的微秒
*/
//inline void gettime_abs_us(timespec &abstime, uint32_t plus_us);
//inline uint64_t gettime_us()
//{
//    timespec abstime;
//    gettime_abs_us(abstime,0);
//    uint64_t tms = abstime.tv_sec*1000000 + abstime.tv_nsec/1000;
//    return tms;
//}

/*
获取从1970-0-0 0:0:0到现在的秒
*/
//inline void gettime_abs_s(timespec &abstime, uint32_t plus_s)
//{
//    abstime.tv_sec = 0;
//    abstime.tv_nsec = 0;
//#ifdef CLOCK_REALTIME_COARSE
//    /*
//    CLOCK_REALTIME_COARSE 比 CLOCK_REALTIME 更快速，但精度也相对较低。
//    CLOCK_REALTIME_COARSE 在RHEL5.8上未定义
//    */
//    clock_gettime(CLOCK_REALTIME_COARSE, &abstime);
//#else
//    clock_gettime(CLOCK_REALTIME,&abstime);
//#endif
//    abstime.tv_sec += plus_s;
//}

/*
获取从1970-0-0 0:0:0到现在的毫秒
*/
//inline void gettime_abs_ms(timespec &abstime, uint32_t plus_ms)
//{
//    /*
//    1s = 1000 ms
//       = 1000*1000 us
//       = 1000*1000*1000 ns
//       = 1,000,000,000 ns
//
//    int 的最大值是：4,294,967,295
//
//    可见，当超过4.294秒时，即plus_ms超过4294ms时，代码
//        abstime.tv_nsec += plus_ms*1000*1000;
//    肯定会越界，引起不可预估的代码错误。
//
//    所以，防止越界处理处理的思路是，将plus_ms超过1秒部分与不足1秒部分分开处理。
//    */
//
//    gettime_abs_s(abstime,plus_ms/(_1_SECOND_MS));
//
//    uint32_t tmp = plus_ms%(_1_SECOND_MS);
//    tmp *= 1000000;//转为纳秒
//    tmp += abstime.tv_nsec;
//
//    abstime.tv_sec += tmp/(_1_SECOND_NS);
//    abstime.tv_nsec = tmp%(_1_SECOND_NS);
//}

/*
获取从1970-0-0 0:0:0到现在的微秒
*/
//inline void gettime_abs_us(timespec &abstime, uint32_t plus_us)
//{
//    gettime_abs_s(abstime,plus_us/(_1_SECOND_US));
//
//    uint32_t tmp = plus_us%(_1_SECOND_US);
//    tmp *= 1000;//转为纳秒
//    tmp += abstime.tv_nsec;
//
//    abstime.tv_sec += tmp/(_1_SECOND_NS);
//    abstime.tv_nsec = tmp%(_1_SECOND_NS);
//}

/*
时间互转
源格式：yyyy-MM-ddTHH:mm:ss
目的格式：time_t
*/
inline time_t timeconvert(const char *buf)
{
    time_t second;
    struct tm p;
    char temp;
    sscanf(buf,"%4d%c%2d%c%2d%c%2d%c%2d%c%2d%c",
        &(p.tm_year),
        &temp,
        &(p.tm_mon),
        &temp,
        &(p.tm_mday),
        &temp,
        &(p.tm_hour),
        &temp,
        &(p.tm_min),
        &temp,
        &(p.tm_sec),
        &temp);

    p.tm_year -= 1900;

    p.tm_mon -= 1;

    return (second = mktime(&p));
}

#endif
