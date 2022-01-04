/*
 * @Copyright (c) 2006-2018 RT-Thread Development Team:  
 * @SPDX-License-Identifier: Apache-2.0:  
 * @Change Logs: 
 * @Date: 2020-02-21 09:35:46
 * @LastEditors: Zhou Xiaomin
 * @LastEditTime: 2020-02-25 18:15:13
 * @Description:  
 */
/* Includes -------------------------------------------------------------*/
#include <rtthread.h>
#include "uuzGPIO.h"
#include "uuzLED.h"
/* ---------------------------------------------------------------------------*/
#define DBG_ENABLE
#define DBG_SECTION_NAME "LED "
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
/* ---------------------------------------------------------------------------*/
void led_run(void* parameter)
{
    while (1) {
        //关于LED相关的接口函数
        led_process(LED_TICK_TIME);
        rt_thread_mdelay(LED_TICK_TIME);
    }
}

int rt_led_timer_init(void)
{
    //初始化信号灯对象

    led_add_device(LD_POW);  //Green->-Power:_LD_POWER
    led_add_device(LD_R1);  //Red1-->Temperature Alarm for CH1:_LD_TA1
    led_add_device(LD_R2);  //Red2-->Temperature Alarm for CH2:_LD_TA2
    led_add_device(LD_B1);  //Blue1-->CH1 Output:_LD_CH1
    led_add_device(LD_B2);  //BLue2-->CH2 Output:_LD_CH2

    //初始化LED状态
    led_on(_LD_POWER);
    led_off(_LD_CH1);
    led_off(_LD_CH2);
    led_off(_LD_TA1);
    led_off(_LD_TA2);

    rt_thread_t tid = RT_NULL;
    tid = rt_thread_create("tid_led",
            led_run,
            RT_NULL,
            256,
            RT_THREAD_PRIORITY_MAX / 2,
            100);

    if (tid != RT_NULL) {
        rt_thread_startup(tid);
        LOG_I("[Thread]:[signal_led] startup.");
    }

    return RT_EOK;
}
