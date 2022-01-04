/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-09   ZhouXiaomin     first version
 */
#ifndef UUZ_RTC_H
#define UUZ_RTC_H

#include "board.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

extern struct tm rTm;

void rtc_init(u16 year, u8 mon, u8 day, u8 hh, u8 mm, u8 ss);
void uuz_vRTCDataGet(void);
u16 uuz_usRTC_GetMinutes(void);
void uuz_vRTC_Current(void);

#ifdef __cplusplus
}
#endif
#endif // UUZ_RTC_H
