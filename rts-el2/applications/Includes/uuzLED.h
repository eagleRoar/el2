/*
 * @Copyright (c) 2006-2018 RT-Thread Development Team:  
 * @SPDX-License-Identifier: Apache-2.0:  
 * @Change Logs: 
 * @Date: 2020-02-21 09:40:14
 * @LastEditors: Zhou Xiaomin
 * @LastEditTime: 2020-02-25 18:15:06
 * @Description:  
 */
#ifndef __UUZ_LED_H
#define __UUZ_LED_H
#include "ledblink.h"

#define LED_TICK_TIME (10U)//10毫米

enum
{
    _LD_POWER = 1,    //电源灯
    _LD_TA1,    //通道温度报警1
    _LD_TA2,    //通道温度报警2
    _LD_CH1,    //通道1状态
    _LD_CH2    //通道2状态

};

#ifdef __cplusplus
extern "C" {
#endif

void rt_led_init(void);
int rt_led_timer_init(void);

#ifdef __cplusplus
}
#endif
#endif // UUZ_LED_H
