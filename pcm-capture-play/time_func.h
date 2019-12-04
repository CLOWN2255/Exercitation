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
#define    CLOCK_REALTIME_COARSE 5  /* linux ֵ */


inline uint64_t gettime_s();

inline int clock_gettime(int, struct timeval *tv)
{
    /*
    ��ȷ������� GetSystemTime / GetLocalTime

    Windows SDK ����������ȷ������Ļ�ȡ��ǰʱ��ĺ�����

        - GetSystemTime����ȡ UTC ʱ�䡣
        - GetLocalTime����ȡ����ʱ�䡣

    �����������ķ��ض��ǣ�SYSTEMTIME 
    ����ṹ��ռ���� 16 ���ֽڣ����Ķ������£�

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

    ���� wYear ��ʾ�ķ�Χ�ǣ��� 1601 �� 30827����Χ�ܹ��ˣ�Ӧ�����õ���ĵ����ˡ�:-)
    */
    /*
    ��ȷ�� 100 ����� GetSystemTimeAsFileTime

    �����ܲ��ܴﵽ 100 ����ľ�ȷ���أ��� Windows SDK ����һ���ṹ�壺FILETIME�������Լ�¼���ȴﵽ 100ns ��ʱ�䡣���ĸ������õ����ֵ�أ������� GetSystemTimeAsFileTime��

    ���ǣ���Ҫ���˵�̫�磬��Ȼ FILETIME �ܹ��ﵽ��˸ߵľ��ȣ�����������������Զ������ԡ�Ϊʲô�أ���Ϊ Windows ϵͳ������һ��ʵʱ����ϵͳ��Windows Embedded Compact 2013 ��һ��ʵʱϵͳ������ʱ�Ӿ���һ����Ϊ�� 15 ~ 16 ���롣

    Windows Sysinternals �������ṩ��һ���鿴��ʹ�õ� Windows ϵͳ��ʱ�ӷֱ��ʵ�С���ߣ�ClockRes v2.0���������������ڿ���̨��ִ�У�������£�
    
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
��ȡ��1970-0-0 0:0:0�����ڵ���
��windows�£�time_tռ8�ֽڡ�
*/
inline  uint64_t gettime_s()
{
    return time(0);
}

/*
��ȡ��1970-0-0 0:0:0�����ڵĺ���
*/
inline uint64_t gettime_ms()
{
    struct timeb t1;
    ftime(&t1);
    uint64_t tms = t1.time * 1000 + t1.millitm;
    return tms;
    /*
    ��ȡ���ͺ���������һ�ַ�����
    time(0)*1000 + SYSTEMTIME.wMilliseconds
    */
}
//
//inline uint64_t GetMsel() //������������
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
////    /* ĳЩƽ̨������Ҫ����ľɵ�ʵ�֣��˷�Ч��Ҫ��һЩ */
////    struct timeval tv;
////    gettimeofday(&tv, NULL);
////    return (LTMSEL)tv.tv_sec*1000 + (tv.tv_usec+500)/1000;
//#endif
//}

/*
��ȡ��1970-0-0 0:0:0�����ڵ�΢��
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
��ȡ��1970-0-0 0:0:0�����ڵ���
*/
//inline void gettime_abs_s(timespec &abstime, uint32_t plus_s)
//{
//    abstime.tv_sec = 0;
//    abstime.tv_nsec = 0;
//#ifdef CLOCK_REALTIME_COARSE
//    /*
//    CLOCK_REALTIME_COARSE �� CLOCK_REALTIME �����٣�������Ҳ��Խϵ͡�
//    CLOCK_REALTIME_COARSE ��RHEL5.8��δ����
//    */
//    clock_gettime(CLOCK_REALTIME_COARSE, &abstime);
//#else
//    clock_gettime(CLOCK_REALTIME,&abstime);
//#endif
//    abstime.tv_sec += plus_s;
//}

/*
��ȡ��1970-0-0 0:0:0�����ڵĺ���
*/
//inline void gettime_abs_ms(timespec &abstime, uint32_t plus_ms)
//{
//    /*
//    1s = 1000 ms
//       = 1000*1000 us
//       = 1000*1000*1000 ns
//       = 1,000,000,000 ns
//
//    int �����ֵ�ǣ�4,294,967,295
//
//    �ɼ���������4.294��ʱ����plus_ms����4294msʱ������
//        abstime.tv_nsec += plus_ms*1000*1000;
//    �϶���Խ�磬���𲻿�Ԥ���Ĵ������
//
//    ���ԣ���ֹԽ�紦�����˼·�ǣ���plus_ms����1�벿���벻��1�벿�ַֿ�����
//    */
//
//    gettime_abs_s(abstime,plus_ms/(_1_SECOND_MS));
//
//    uint32_t tmp = plus_ms%(_1_SECOND_MS);
//    tmp *= 1000000;//תΪ����
//    tmp += abstime.tv_nsec;
//
//    abstime.tv_sec += tmp/(_1_SECOND_NS);
//    abstime.tv_nsec = tmp%(_1_SECOND_NS);
//}

/*
��ȡ��1970-0-0 0:0:0�����ڵ�΢��
*/
//inline void gettime_abs_us(timespec &abstime, uint32_t plus_us)
//{
//    gettime_abs_s(abstime,plus_us/(_1_SECOND_US));
//
//    uint32_t tmp = plus_us%(_1_SECOND_US);
//    tmp *= 1000;//תΪ����
//    tmp += abstime.tv_nsec;
//
//    abstime.tv_sec += tmp/(_1_SECOND_NS);
//    abstime.tv_nsec = tmp%(_1_SECOND_NS);
//}

/*
ʱ�以ת
Դ��ʽ��yyyy-MM-ddTHH:mm:ss
Ŀ�ĸ�ʽ��time_t
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
