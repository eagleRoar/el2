#ifndef CALENDAR_H
#define CALENDAR_H

#include "typedefINIT.h"

#ifdef __cplusplus
extern "C" {
#endif

u8 isLeapMonth(u32 year);
u32 allDaysOfYear(u32 year);
u32 allDays(u32 year, u32 month, u32 day);
u32 monthDays(u32 year, u32 month);
u32 allDays2Year(u32 year);
u32 isTrueYear(u32 year);
u32 isTrueMonth(u32 month);
u32 isTrueDay(u32 day, u32 monthDay);

#ifdef __cplusplus
}
#endif

#endif // CALENDAR_H
