/* Includes ------------------------------------------------------------------*/
#include <uuzINIT.h>
#include "typedefDEF.h"
#include "typedefINIT.h"
#include "rtc.h"
#include "stm32f1xx_hal_rtc_ex.h"
#include "uuzRTC.h"
#include "board.h"
#include <rtthread.h>
#include <rtdevice.h>
//-----------------------------------------------
#define DBG_ENABLE
#define DBG_SECTION_NAME "PVD"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
//-----------------------------------------------
extern RTC_HandleTypeDef RTC_Handler;

void pvd_sync_init(void)
{
    xLogs[_G_POWER_DOWN].en = 1;
    //日期
    xLogs[_G_POWER_DOWN].date[0] = HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR3) + 1900;
    xLogs[_G_POWER_DOWN].date[1] = HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR4) + 1;
    xLogs[_G_POWER_DOWN].date[2] = HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR5);
    //时间
    xLogs[_G_POWER_DOWN].time[0] = HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR6);
    xLogs[_G_POWER_DOWN].time[1] = HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR7);
    xLogs[_G_POWER_DOWN].time[2] = HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR8);

#if 0
    //显示数据
    LOG_I("[RD]DR3=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR3));
    LOG_I("[RD]DR4=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR4));
    LOG_I("[RD]DR5=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR5));
    LOG_I("[RD]DR6=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR6));
    LOG_I("[RD]DR7=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR7));
    LOG_I("[RD]DR7=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR8));
#endif

    //保存相关数据
    save_logs();
    rt_thread_mdelay(100);
}

void pvd_data_update(void)
{
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR3, rTm.tm_year);
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR4, rTm.tm_mon);
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR5, rTm.tm_mday);
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR6, rTm.tm_hour);
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR7, rTm.tm_min);
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR8, rTm.tm_sec);

    //更新日期数据
    if ((rTm.tm_year - 100) != HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR12)) {
        HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR10, rTm.tm_year - 100);
        HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR11, rTm.tm_mon + 1);
        HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR12, rTm.tm_mday);
        HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR13, rTm.tm_wday + 1);
    }

#if 0
    LOG_I("[UP]DR3=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR3));
    LOG_I("[UP]DR4=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR4));
    LOG_I("[UP]DR5=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR5));
    LOG_I("[UP]DR6=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR6));
    LOG_I("[UP]DR7=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR7));
    LOG_I("[RD]DR8=%d", HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR8));
#endif
}

/**
 * @param state:0:清除标记，1:添加标记
 */
void pvd_enable_set(u8 state)
{
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR2, state);
}

