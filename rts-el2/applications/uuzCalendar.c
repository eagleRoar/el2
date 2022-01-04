/* Includes ------------------------------------------------------------------*/
#include "uuzCalendar.h"
#include "uuzOpt.h"
/* --------------------------------------------------------------------------*/
/**
 * @brief 检查闰年
 * @param year
 * @return
 */
u8 isLeapMonth(u32 year)
{
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        return uuzTRUE;
    } else {
        return uuzFALSE;
    }
}

/**
 * @brief 获取年的天数
 * @param year
 * @return
 */
u32 allDaysOfYear(u32 year)
{
    u32 total = 0;
    if (isLeapMonth(year)) {
        total = 366;
    } else {
        total = 365;
    }

    return total;
}

/**
 * @brief 返回1900年从1月1号到输入日期一共有多少天
 * @param year
 * @param month
 * @param day
 * @return
 */
u32 allDays(u32 year, u32 month, u32 day)
{
    u32 total = 0;

    switch (month)
    {
        case 12:
            total += 30;
        case 11:
            total += 31;
        case 10:
            total += 30;
        case 9:
            total += 31;
        case 8:
            total += 31;
        case 7:
            total += 30;
        case 6:
            total += 31;
        case 5:
            total += 30;
        case 4:
            total += 31;
        case 3:
            if (isLeapMonth(year)) {
                total += 29;
            } else {
                total += 28;
            }
        case 2:
            total += 31;
        case 1:
            total += day;
            break;
    }

    return total;
}

/**
 * @brief 返回当月有多少天
 * @param year
 * @param month
 * @return
 */
u32 monthDays(u32 year, u32 month)
{
    u32 total = 0;

    switch (month)
    {
        case 12:
            total = 31;
            break;
        case 11:
            total = 30;
            break;
        case 10:
            total = 31;
            break;
        case 9:
            total = 30;
            break;
        case 8:
            total = 31;
            break;
        case 7:
            total = 31;
            break;
        case 6:
            total = 30;
            break;
        case 5:
            total = 31;
            break;
        case 4:
            total = 30;
            break;
        case 3:
            total = 31;
            break;
        case 2:
            if (isLeapMonth(year)) {
                total = 29;
            } else {
                total = 28;
            }
            break;
        case 1:
            total = 31;
            break;
    }
    return total;
}

/**
 * @brief 1900到到前一年的12月31日的总天数
 * @param year
 * @return
 */
u32 allDays2Year(u32 year)
{
    u32 total = 0;
    u32 n;
    for (n = 1900; n < year; n++) {
        if (isLeapMonth(n)) {
            total += 366;
        } else {
            total += 365;
        }
    }
    return total;
}

// 检测日期是否合法
u32 isTrueYear(u32 year)
{
    while (year < 1900 || year > 2038) {
        return uuzFALSE;
    }

    return year;
}

u32 isTrueMonth(u32 month)
{
    while (month < 1 || month > 12) {
        return uuzFALSE;
    }
    return month;
}

u32 isTrueDay(u32 day, u32 monthDay)
{
    while (day <= 0 || day > monthDay) {
        return uuzFALSE;
    }
    return day;
}

