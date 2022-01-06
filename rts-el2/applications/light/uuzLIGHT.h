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

//extern LightCache_Typedef_t xLightCache;

#ifdef __cplusplus
extern "C" {
#endif

int light_opt_init(void);
void rt_light_init(void);
void rt_light_event(void);
void rt_light_ch_event(u8 ch, u8 state);
void light_opt_thread_entry(void* parameter);
//报警相关函数-V3.0
u8 uuz_vLightALM_Query(u8 channel);
void uuz_vLightALM_Event(u8 channel);
void uuz_vLightALM_Opt(void);
//时间相关检查-V3.0
u8 uuz_vLightTime_Query(u8 channel);
//SunRS相关检查-V3.0
void uuz_vLightRiseSet_Query(u8 channel);
void uuz_vLightSunRise_Start(u8 channel);
void uuz_vLightSunSet_Start(u8 channel);
void uuz_vLightSunRise_Event(u8 channel);
void uuz_vLightSunSet_Event(u8 channel);
void uuz_vLightSunRiseSet_Complete(u8 channel);
//Light的相关执行设备-V2.0
void uuz_vLightOpt_Set(u8 channel, u8 ucSta, u8 ucValue, u8 ucDelay);
void uuz_vLightOpt_Event(u8 channel);
#ifdef __cplusplus
}
#endif

#endif // __UUZ_LIGHT_EVENT_H
