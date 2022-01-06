/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-09   ZhouXiaomin     first version
 */
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#include <rtthread.h>
#include <string.h>
/* ------------------------------------------------------------------*/
#include "uuzInit.h"
#include "uuzRTC.h"
/* ------------------------------------------------------------------*/
#define DBG_ENABLE
#define DBG_SECTION_NAME "RTC"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
/* ------------------------------------------------------------------*/
#ifdef BSP_USING_ONCHIP_RTC
/* ------------------------------------------------------------------*/
struct tm rTm;
/* ---------------------------------------------------------------------------*/
/**
 * @brief uuz_vRTCDataGet
 */
void uuz_vRTCDataGet(void)
{
    time_t now;
    struct tm* p_tm;

    /* get current time */
    now = time(RT_NULL);
    /* lock scheduler. */
    rt_enter_critical();
    /* converts calendar time time into local time. */
    p_tm = localtime(&now);
    /* copy the statically located variable */
    rt_memcpy(&rTm, p_tm, sizeof(struct tm));
    /* unlock scheduler. */
    rt_exit_critical();
}

/**
 * @brief 计算当前的分钟数据
 * @return  u16 mins
 */
u16 uuz_usRTC_GetMinutes(void)
{
    return (rTm.tm_hour * 60 + rTm.tm_min);
}

/**
 * @brief 计算当前的秒数
 * @return  u32 seconds
 */
u32 uuz_ulRTC_GetSeconds(void)
{
    return (uuz_usRTC_GetMinutes() * 60 + rTm.tm_sec);
}

/**
 * @brief uuz_vRTC_Current
 */
void uuz_vRTC_Current(void)
{
    LOG_I("Current:[%04d-%02d-%02d %02d:%02d:%02d]", rTm.tm_year + 1900, rTm.tm_mon + 1, rTm.tm_mday, rTm.tm_hour,
            rTm.tm_min, rTm.tm_sec);
}
#endif
/*-----------------------------------------------------------------*/
