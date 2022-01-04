/*
 * @Copyright (c) 2006-2018 RT-Thread Development Team:  
 * @SPDX-License-Identifier: Apache-2.0:  
 * @Date: 2020-01-01 09:23:03
 * @LastEditors  : Zhou Xiaomin
 * @LastEditTime : 2020-01-04 18:22:07
 * @Description:  
 */
#ifndef __UUZ_LIGHT_EVENT_H
#define __UUZ_LIGHT_EVENT_H

#include <board.h>
#include "typedefLIGHT.h"

extern LightCache_Typedef_t xLightCache;

#ifdef __cplusplus
extern "C" {
#endif

int test(void);//Justin debug 仅仅测试
void test_entry(void* parameter);//Justin debug 仅仅测试
void test_entry1(void* parameter);//Justin debug 仅仅测试
int light_opt_init(void);
void light_opt_thread_entry(void* parameter);
//报警相关函数-V2.0
void uuz_vLightALM_Event(u8 channel);
//时间相关检查-V2.0
void uuz_vLightTime_Query(u8 channel);
//SunRS相关检查-V2.0
void uuz_vLightSunRS_Query(u8 channel);
void uuz_vLightSunRS_Event(u8 channel);
u32 uuz_vLightSunRS_Set(u8 time);
void uuz_vLightSunRS_Stop(u8 channel);
//Light的相关执行设备-V2.0
void uuz_vLightOpt_Set(u8 channel, u8 ucSta, u8 ucValue, u8 ucDelay);
void uuz_vLightOpt_Event(u8 channel);
#if 0
void lda1_sunrs_opt_thread_entry(void* parameter);
//HID相关函数-V2.0
void uuz_vLightHID_Event(void);
void uuz_vLightHID_Start(u8 ucLine, u8 ucSta);
#endif
#ifdef __cplusplus
}
#endif

#endif // __UUZ_LIGHT_EVENT_H
