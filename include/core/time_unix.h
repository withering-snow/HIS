#ifndef HIS_TIME_UNIX_H
#define HIS_TIME_UNIX_H

#include <base.h>

#define INVALID_TIME (-1LL)

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

/**
 * @brief 将时间戳转为字符串，只保留日期，如 "2023-10-27"
 * @param time_stamp 传入时间戳
 * @return 返回对应的时间字符串
 */
const char* Time_to_string_date(long long time_stamp);

/**
 * @brief 将 Unix 时间戳转换为 YYYYMMDD 格式的整数
 * @example 1712880000 -> 20240412
 */
int Time_to_int_date(long long ts)

/**
 * @brief 根据出生时间戳计算当前年龄
 * @param birth_ts 出生时的 Unix 时间戳
 * @return 整数周岁
 */
int Time_CalculateAge(long long birth_ts);

#endif// HIS_TIME_UNIX_H
