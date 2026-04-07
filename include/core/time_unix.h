#ifndef HIS_TIME_UNIX_H
#define HIS_TIME_UNIX_H

#include <base.h>

/**
 * @brief 获取系统时间戳，用于存储
 * @return 当前系统时间戳
 */
long long Time_now(void);

/**
 * @brief 将时间戳转为字符串，如 "2023-10-27 10:30:00"
 * @param time_stamp 传入时间戳
 * @return 返回对应的时间字符串
 */
const char* Time_to_string(long long time_stamp);

#endif// HIS_TIME_UNIX_H
