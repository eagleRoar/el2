/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-09     RT-Thread    first version
 */

#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <uuzINIT.h>
#include <uuzUI.h>
//----------------------------------
#include "uuzADC.h"
#include "uuzBTN.h"
#include "uuzDAC.h"
#include "uuzDevice.h"
#include "uuzLED.h"
#include "uuzLIGHT.h"
#include "uuzOLED.h"
#include "uuzOpt.h"
#include "uuzRTC.h"
#include "uuzUart.h"
#include "uuzUartEvent.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

int main(void)
{
    int count = 1;

    //初始化配置信息
    uuz_vLocalInit();
    //打开OLED屏幕
    oled_init();
    //打开按键线程[BTN]
    uuz_btn_init();
    //打开线程[LED]
    rt_led_timer_init();
    //初始化数据采集[ADC]
    adc_sync_init();
    //启动DAC输出
    dac_init();
    //显示初始化主界面
    ui_sync_init();
    //Light操作
    light_opt_init();
    //读取PVD的时间相关数据
    pvd_sync_init();

    while (count++) {
        //同步读取RTC时间,每秒1次
        uuz_vRTCDataGet();
        //每10秒显示时间
        if (count % 30 == 0) {
            uuz_vRTC_Current();
        }

        //如果启动未完成
        if (xSta[_S_FLAG_STARTUP] == 0) {
            //等待5秒后，表示启动完成
            if (count >= 5) {
                xSta[_S_FLAG_STARTUP] = 1;
            }
        } else {
            //启动完成后,添加断电日志
            if (xLogs[_G_POWER_DOWN].en == 1) {
                xLogs[_G_POWER_DOWN].en = 0;
                xSta[_S_FLAG_PVD] = 1;
                //切换灯闪烁
                led_blink(_LD_POWER, 500, 500);
            }
        }

        //每1秒更新一次BR寄存器
        pvd_data_update();

        //延时1秒
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}
