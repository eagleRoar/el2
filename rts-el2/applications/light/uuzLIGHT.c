/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-18   ZhouXiaomin     first version
 */
/******************************************************************************/
#include <rtthread.h>
#include <string.h>
#include <uuzUI.h>
/******************************************************************************/
#include "typedefDEF.h"
#include "uuzDAC.h"
#include "uuzGPIO.h"
#include "uuzLED.h"
#include "uuzOpt.h"
#include "uuzRTC.h"
#include "uuzTEMP.h"
/******************************************************************************/
#include "typedefUI.h"
#include "uuzConfig1602.h"
#include "uuzConfigLIGHT.h"
#include "uuzLIGHT.h"
/******************************************************************************/
//Debug Log Config
#define DBG_ENABLE
#define DBG_SECTION_NAME "LIGHT"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
/******************************************************************************/
/* ------------------------------------------------------------------*/
//ExLDA1Cache_Typedef_t xExLDA1Cache;
//LightCache_Typedef_t xLightCache;
/******************************************************************************/
/**
 * @brief:  LIGHT的工作模式
 * @returns:  int
 * @Author: Zhou Xiaomin
 * @Date: 2020-01-07 14:31:03
 */
int light_opt_init(void)
{
    rt_err_t ret = RT_EOK;
    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("light_rd", light_opt_thread_entry,
    RT_NULL, 2500, 18, 20);

    /* 创建成功则启动线程 */
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
        LOG_I("start Thread [light opt]");
    } else {
        LOG_E("start Thread [light opt] failed");
        ret = RT_ERROR;
    }

    return ret;
}

/**
 * @brief:   LIGHT线程运行函数
 * @returns:   void
 * @Author: Zhou Xiaomin
 * @Date: 2020-01-07 14:33:10
 */
void light_opt_thread_entry(void* parameter)
{
    static u32 delayCount = 0;
    u8 alarmSta = uuzLIGHT_ALM_NULL;
    u8 channel = _CHANNEL_1;

    //默认ALARM的输出为NC
    rt_pin_write(ALM_CTR, PIN_LOW);  //切换ALM为NC

    uuz_vLightSunRiseSet_Complete(_CHANNEL_1);  //清除线路1日升日落状态
    uuz_vLightSunRiseSet_Complete(_CHANNEL_2);  //清除线路2日升日落状态

    //数据当前有效值
    while (1) {
        //等待5秒保护时间
        if (xSta[_S_FLAG_STARTUP] == 1) {
            //如果处于手动操作界面，不执行自动工作
            if (xSta[_S_LIGHT_MANUAL] == 0) {
                //NOTE:线路1相关信息
                channel = _CHANNEL_1;
                //NOTE:根据当前温度信息，判断是否有报警信息
                alarmSta = uuz_vLightALM_Query(channel);
                //NOTE:判断是否需要刷新数据
                if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] != alarmSta) {
                    if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_NULL) {
                        xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] = alarmSta;
                    } else {
                        if (alarmSta == uuzLIGHT_ALM_NULL) {
                            xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] = uuzLIGHT_ALM_CLR;
                        } else {
                            xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] = alarmSta;
                        }
                    }
                    //NOTE:执行相关动作
                    uuz_vLightALM_Event(channel);
                }

                //NOTE:读取CH1日升日落时间
                uuz_vLightRiseSet_Query(channel);
                //NOTE:初始化Light的相关状态, 根据灯光工作模式判断当前状态, 设置工作状态
                xSta[_S_CH1_LT_STA + _CH_DIS(channel)] = uuz_vLightTime_Query(channel);
                switch (xSta[_S_CH1_LT_STA + _CH_DIS(channel)]) {  //NOTE:处理获取到的灯光CH-1状态
                    case uuzLTS_ON:  //NOTE:如果是开启动作
                        if (((xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_ON)  //当前设备已经开启情况下，
                        && (xSta[_S_CH1_DAC_VAL + _CH_DIS(channel)]
                                != get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel))))  //NOTE:设备如果设备已经开启,判断亮度是否一致或者设备是关闭状态，
                        || (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_OFF)) {   //当前设备已经关闭的情况
                            uuz_vLightOpt_Set(channel, uuzLTS_ON,
                                    get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel)), 0);   //执行开启设备动作
                        }
                        //NOTE:CH-2 MODE
                        if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_FOLLOW) {   //跟随模式
                            uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_ON,
                                    get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel)), 0);    //跟随开启设备状态
                        } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INVERSE) {   //互斥模式
                            uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 0);    //关闭灯光设备
                        }
                        break;
                    case uuzLTS_OFF:    //NOTE:如果是关闭动作
                        if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_ON) {    //NOTE:如果当前设备是开启状态，执行关闭动作
                            uuz_vLightOpt_Set(channel, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 0);    //关闭灯光设备
                            if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)]) {   //如果处于日升或日落状态
                                uuz_vLightSunRiseSet_Complete(channel);
                            }
                        }
                        //NOTE:CH-2 MODE判断
                        if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] != uuzLIGHT_ALM_OFF) {   //如果是过热报警状态，不触发联动
                            if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_FOLLOW) {  //跟随模式
                                uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 0);    //关闭灯光设备
                            } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INVERSE) {  //互斥模式
                                uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_ON, get_data(_D_LT, _L_CH1_OUTPUT_LEVEL), 0);
                            }
                        }
                        break;
                    case uuzLTS_DIM:
                        //自动10%调光
                        //NOTE:如果当前设备为10%智能控温状态(开启状态)，执行10%亮度开启
                        //NOTE:判断设备当前是亮度大于10%，如果是开启状态且亮度大于10%，则强制将亮度降低为10%以下
                        if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_ON) {    //NOTE:如果当前设备是开启状态，执行关闭动作
                            uuz_vLightOpt_Set(channel, uuzLTS_ON, uuzLIGHT_VAL_MIN, 0);  //开启最低亮度
                        }

                        //NOTE:CH-2 MODE判断
                        if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_FOLLOW) {  //跟随模式
                            uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_ON, uuzLIGHT_VAL_MIN, 0);
                        } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INVERSE) {  //互斥模式
                            uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 0);
                        }
                        break;
                    case uuzLTS_RISE:
                        //日升模式标签
                        if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_NULL) {    //当前非日升模式
                            if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_OFF) {   //NOTE:如果当前设备为关闭状态，执行日升启动动作
                                uuz_vLightSunRise_Start(channel);   //NOTE:日升模式，初始化日升数据
                            }
                        } else if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_RISE) {  //当前日升模式
                            uuz_vLightSunRise_Event(channel);   //日升循环操作
                        } else {
                            uuz_vLightSunRiseSet_Complete(channel);
                        }
                        break;
                    case uuzLTS_SET:
                        //日落模式标签
                        if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_NULL) {    //当前非日落模式
                            if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_ON) {   //NOTE:如果当前设备为开启状态，执行日落启动动作
                                uuz_vLightSunSet_Start(channel);    //NOTE:日落模式，执行相关动作
                            }
                        } else if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_SET) {  //当前日落模式
                            uuz_vLightSunSet_Event(channel);
                        } else {
                            uuz_vLightSunRiseSet_Complete(channel);
                        }
                        break;
                    case uuzLTS_NULL:
                        //NOTE:判断是否处于日升或日落延时状态，执行对应的数据处理
                        //有日升日落时间
                        //1秒检测1次,并查看相关数据来执行
                        if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_FOLLOW) {  //如果跟随模式
                            if (xSta[_S_CH1_DAC_STA + _CH_DIS(_CHANNEL_1)] == uuzLTS_ON) {
                                if ((xSta[_S_CH1_RS_STA + _CH_DIS(channel)] != uuzLTS_RISE)  //不处于日升状态
                                && (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] != uuzLTS_SET)) {    //不处于日落状态
                                    if (((xSta[_S_CH1_DAC_STA + _CH_DIS(_CHANNEL_2)]
                                            != xSta[_S_CH1_DAC_STA + _CH_DIS(_CHANNEL_1)]))
                                            || (xSta[_S_CH1_DAC_VAL + _CH_DIS(_CHANNEL_2)]
                                                    != get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(_CHANNEL_1)))) {
                                        uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_ON,
                                                get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(_CHANNEL_1)), 0);
                                    }
                                }
                            } else {
                                if (xSta[_S_CH1_DAC_STA + _CH_DIS(_CHANNEL_1)] == uuzLTS_OFF) {
                                    if (xSta[_S_CH1_DAC_STA + _CH_DIS(_CHANNEL_2)] == uuzLTS_ON) {
                                        uuz_vLightOpt_Set(
                                        _CHANNEL_2,
                                                uuzLTS_OFF,
                                                get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(_CHANNEL_1)),
                                                0);
                                    }
                                }
                            }
                        } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INVERSE) {  //如果互斥模式
                            if (xSta[_S_CH1_DAC_STA + _CH_DIS(_CHANNEL_1)] == uuzLTS_ON) {
                                uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_OFF, get_data(_D_LT, _L_CH1_OUTPUT_LEVEL), 0);
                            } else {
                                if (xSta[_S_CH1_LT_ALM + _CH_DIS(_CHANNEL_1)] != uuzLIGHT_ALM_OFF) {  //如果是过热报警状态，不触发联动
                                    uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_ON, get_data(_D_LT, _L_CH1_OUTPUT_LEVEL), 0);
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
                //LOG_I("Light State:%d", xSta[_S_CH1_LT_STA + _CH_DIS(channel)]);
                //执行Light的相关事件
                uuz_vLightOpt_Event(channel);

                //NOTE:-----------------------------------------------------------------------------
                //NOTE:线路2相关信息
                channel = _CHANNEL_2;
                alarmSta = uuz_vLightALM_Query(channel);
                //NOTE:判断是否需要刷新数据
                if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] != alarmSta) {
                    if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_NULL) {
                        xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] = alarmSta;
                    } else {
                        if (alarmSta == uuzLIGHT_ALM_NULL) {
                            xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] = uuzLIGHT_ALM_CLR;
                        } else {
                            xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] = alarmSta;
                        }
                    }
                    //NOTE:执行相关动作
                    uuz_vLightALM_Event(channel);
                }

                if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INDEPENDENT) {  //NOTE:根据当前温度信息，判断是否有报警信息
                    //NOTE:读取CH2日升日落时间
                    uuz_vLightRiseSet_Query(channel);
                    //NOTE:初始化Light的相关状态, 根据灯光工作模式判断当前状态, 设置工作状态
                    xSta[_S_CH1_LT_STA + _CH_DIS(channel)] = uuz_vLightTime_Query(channel);
                    //NOTE:处理获取到的灯光CH-2状态
                    switch (xSta[_S_CH1_LT_STA + _CH_DIS(channel)]) {
                        case uuzLTS_ON:
                            //NOTE:如果是开启动作
                            //NOTE:设备如果设备已经开启,判断亮度是否一致或者设备是关闭状态，
                            if (((xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_ON)  //当前设备已经开启情况下，
                            && (xSta[_S_CH1_DAC_VAL + _CH_DIS(channel)]
                                    != get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel))))  //NOTE:设备如果设备已经开启,判断亮度是否一致或者设备是关闭状态，
                            || (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_OFF)) {   //当前设备已经关闭的情况
                                uuz_vLightOpt_Set(channel, uuzLTS_ON,
                                        get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel)), 0);   //执行开启设备动作
                            }
                            break;
                        case uuzLTS_OFF:
                            //NOTE:如果当前设备上开启状态，执行关闭动作
                            if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_ON) {    //NOTE:如果当前设备是开启状态，执行关闭动作
                                uuz_vLightOpt_Set(channel, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 0);    //关闭灯光设备
                                if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)]) {   //如果处于日升或日落状态
                                    uuz_vLightSunRiseSet_Complete(channel);
                                }
                            }
                            break;
                        case uuzLTS_DIM:
                            //NOTE:如果当前设备为10%智能控温状态(开启状态)，执行10%亮度开启
                            //NOTE:判断设备当前是亮度大于10%，如果是开启状态且亮度大于10%，则强制将亮度降低为10%以下
                            if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_ON) {    //NOTE:如果当前设备是开启状态，执行关闭动作
                                uuz_vLightOpt_Set(channel, uuzLTS_ON, uuzLIGHT_VAL_MIN, 0);  //开启最低亮度
                            }
                            break;
                        case uuzLTS_RISE:
                            if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_NULL) {    //当前非日升模式
                                if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_OFF) {  //NOTE:如果当前设备为关闭状态，执行日升启动动作
                                    uuz_vLightSunRise_Start(channel);   //NOTE:日升模式，初始化日升数据
                                }
                            } else if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_RISE) {  //当前日升模式
                                uuz_vLightSunRise_Event(channel);   //日升循环操作
                            } else {
                                uuz_vLightSunRiseSet_Complete(channel);
                            }

                            break;
                        case uuzLTS_SET:
                            if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_NULL) {    //当前非日落模式
                                if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_ON) {  //NOTE:如果当前设备为开启状态，执行日落启动动作
                                    uuz_vLightSunSet_Start(channel);    //NOTE:日落模式，执行相关动作
                                }
                            } else if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_SET) {  //当前日落模式
                                uuz_vLightSunSet_Event(channel);
                            } else {
                                uuz_vLightSunRiseSet_Complete(channel);
                            }
                            break;
                        case uuzLTS_NULL:
                            break;
                        default:
                            break;
                    }
                }
                //LOG_I("Light State:%d", xSta[_S_CH1_LT_STA + _CH_DIS(channel)]);
                //执行CH-2Light的相关事件
                uuz_vLightOpt_Event(_CHANNEL_2);
            }

            //处理报警的NO和NC输出
            uuz_vLightALM_Opt();
        }
        delayCount++;
        //每1000ms一次
        rt_thread_delay(1000);
    }
}

/**
 * @brief 返回相应频道的报警状态
 * @param channel
 * @return
 */
u8 uuz_vLightALM_Query(u8 channel)
{
    u16 usCurrentTP = 0;
    u16 usShutDownTa = 0;
    u16 usAutoDimTa = 0;
    u8 ucAlarmOpt = uuzLIGHT_ALM_NULL;

    //频道1有温度相关设备
    if (xSta[_S_CH1_TEMP_EN + _CH_DIS(channel)] == 1) {
        //当前温度
        usCurrentTP = xSta[_S_CH1_TEMP_VAL + _CH_DIS(channel)];
        //LOG_D("[%d]C:%d,DIM:%d", channel, usCurrentTP, xSysConfig.Light[_L_CH1_AUTO_DIM_TEMP + _CH_LVL(channel)]);
        //获取对应单位的温度数据
        usShutDownTa = xSysConfig.Light[_L_CH1_SHUTDOWN_TEMP + _CH_LVL(channel)];
        usAutoDimTa = xSysConfig.Light[_L_CH1_AUTO_DIM_TEMP + _CH_LVL(channel)];
        //1-判断是否达到LIGHT的温度相关设置 100%,自动关闭灯光
        //2-判断有效报警标记,添加报警累计数
        //3-或者判断有效退出报警标记，减少报警累计数
        //4-达到有效报警次数，进入报警状态
        //4-达到有效报警次数减为0，退出报警状态
        //5-记录相关报警信息
        //6-切换报警灯光显示
        if (usCurrentTP >= usShutDownTa) {
            //判断有效标记
            if (xSta[_S_CH1_LT_SHUT_C + _CH_DIS(channel)] < uuzALM_MAX) {
                xSta[_S_CH1_LT_SHUT_C + _CH_DIS(channel)]++;
            }
        } else {
            if (xSta[_S_CH1_LT_SHUT_C + _CH_DIS(channel)] > 0) {
                //离开报警状态，清除标记
                xSta[_S_CH1_LT_SHUT_C + _CH_DIS(channel)]--;
            }
        }

        //满足关闭的报警状态
        if ((xSta[_S_CH1_LT_SHUT_C + _CH_DIS(channel)] == uuzALM_MAX)
                || (xSta[_S_CH1_LT_SHUT_C + _CH_DIS(channel)] == 0)) {
            //判断是否需要发送相关数据
            if (xSta[_S_CH1_LT_SHUT_P + _CH_DIS(channel)] != xSta[_S_CH1_LT_SHUT_C + _CH_DIS(channel)]) {
                xSta[_S_CH1_LT_SHUT_P + _CH_DIS(channel)] = xSta[_S_CH1_LT_SHUT_C + _CH_DIS(channel)];
            }
        }

        //1-判断是否达到LIGHT的温度相关设置,自动关闭亮度为50%
        //2-判断有效报警标记,添加报警累计数
        //3-或者判断有效退出报警标记，减少报警累计数
        //4-达到有效报警次数，进入报警状态
        //4-达到有效报警次数减为0，退出报警状态
        //5-记录相关报警信息
        //6-切换报警灯光显示
        if (usCurrentTP >= usAutoDimTa) {
            //判断有效标记,添加报警累计数
            if (xSta[_S_CH1_LT_DIM_C + _CH_DIS(channel)] < uuzALM_MAX) {
                xSta[_S_CH1_LT_DIM_C + _CH_DIS(channel)]++;
            }
        } else {
            //退出报警状态，减去报警累计数
            if (xSta[_S_CH1_LT_DIM_C + _CH_DIS(channel)] > 0) {
                //离开报警状态，清除标记
                xSta[_S_CH1_LT_DIM_C + _CH_DIS(channel)]--;
            }
        }
        //达到满足50%的报警状态
        if ((xSta[_S_CH1_LT_DIM_C + _CH_DIS(channel)] == uuzALM_MAX)
                || (xSta[_S_CH1_LT_DIM_C + _CH_DIS(channel)] == 0)) {
            //判断是否需要发送相关数据
            if (xSta[_S_CH1_LT_DIM_P + _CH_DIS(channel)] != xSta[_S_CH1_LT_DIM_C + _CH_DIS(channel)]) {
                xSta[_S_CH1_LT_DIM_P + _CH_DIS(channel)] = xSta[_S_CH1_LT_DIM_C + _CH_DIS(channel)];
            }
        }

        //初始化报警数据
        ucAlarmOpt = uuzLIGHT_ALM_NULL;
        //有自动关闭的报警数据
        if (xSta[_S_CH1_LT_SHUT_P + _CH_DIS(channel)] == uuzALM_MAX) {
            ucAlarmOpt = uuzLIGHT_ALM_OFF;
        }

        //有自动降温的报警数据
        if (ucAlarmOpt == uuzLIGHT_ALM_NULL) {
            if (xSta[_S_CH1_LT_DIM_P + _CH_DIS(channel)] == uuzALM_MAX) {
                ucAlarmOpt = uuzLIGHT_ALM_DIM;
            }
        }
    } else {
        //没有温度实际数据，清空报警数据,根据需要保存数据状态
        ucAlarmOpt = uuzLIGHT_ALM_NULL;
    }

    return ucAlarmOpt;
}

/**
 * @brief 报警事件处理
 * @param channel
 */
void uuz_vLightALM_Event(u8 channel)
{
    u8 ucAlarmToFlash = 0;

    if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_OFF) {
        //NOTE:有自动关闭的报警数据
        //添加报警记录
        set_logs((_G_CH1_SHUTDOWN + channel), NULL, 0);
        //保存数据标记
        ucAlarmToFlash = 1;
    } else if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_DIM) {
        //NOTE:有自动降温的报警数据
        //添加报警记录
        set_logs((_G_CH1_AUTO_DIM + channel), NULL, 0);
        //保存数据标记
        ucAlarmToFlash = 1;
    }

    //NOTE:有报警标记
    if (ucAlarmToFlash) {
        //NOTE:保存日志数据
        save_logs();
    }
}

/**
 * @brief 计算日升日落的实际时间
 *
 * @param channel
 */
void uuz_vLightRiseSet_Query(u8 channel)
{
    //计算到秒
    u32 tRise = 0;
    u32 tSet = 0;
    u32 tOn = 0;
    u32 tOff = 0;
    u32 tLong = 0;
    u32 tRiseEnd = 0;
    u8 fRiseSet = 0;

    u32 tCurrent = uuz_ulRTC_GetSeconds();
    tRise = get_data(_D_LT, _L_CH1_SUNRISE_DELAY + _CH_LVL(channel)) * 60;  //日升时间
    tSet = get_data(_D_LT, _L_CH1_SUNSET_DELAY + _CH_LVL(channel)) * 60;  //日落时间
    tOn = get_data(_D_LT, _L_CH1_CYCLE_ON + _CH_LVL(channel)) * 60;  //启动时间
    tOff = get_data(_D_LT, _L_CH1_CYCLE_OFF + _CH_LVL(channel)) * 60;  //结束时间

    //计算启动时间的起点
    //计算持续时间长短
    //TODO:判断tOn是否大于tOff,如果大于，进入日升日落判断，如果小于，不进行相关判断
    if (tOff == tOn) {
        //没有相关数据，执行数据清零
        xSta[_S_CH1_LT_ON + _CH_DIS(channel)] = 0;  //开启设备时间
        xSta[_S_CH1_LT_OFF + _CH_DIS(channel)] = 0;  //关闭设备时间
        xSta[_S_CH1_LT_RISE + _CH_DIS(channel)] = 0;    //日升启动时间点
        xSta[_S_CH1_RS_RISE + _CH_DIS(channel)] = 0;    //日升启动时长
        xSta[_S_CH1_LT_SET + _CH_DIS(channel)] = 0;  //日落启动时间点
        xSta[_S_CH1_RS_SET + _CH_DIS(channel)] = 0;  //日落启动时长
    } else {
        if (tOff > tOn) {   //正向时间，进入日升日落判断
            if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_NULL) {
                fRiseSet = 0;
                tLong = (tOff - tOn) / 2;   //获取分段时间长度
                tRiseEnd = tOn + tLong - tRise;   //获取日升启动最晚时间点
                //获取更新后的时长
                xSta[_S_CH1_LT_ON + _CH_DIS(channel)] = tOn;
                xSta[_S_CH1_LT_OFF + _CH_DIS(channel)] = tOff;

                if (tRise) {   //有日升时间设置
                    if ((tCurrent >= tOn) && (tCurrent <= tRiseEnd)) {    //处于允许日升启动的时间段内
                        xSta[_S_CH1_LT_RISE + _CH_DIS(channel)] = tCurrent;    //获取日升启动时间
                        xSta[_S_CH1_RS_RISE + _CH_DIS(channel)] = tRise;    //获取日升时间长度
                    } else {
                        xSta[_S_CH1_LT_RISE + _CH_DIS(channel)] = tOn;    //获取日升启动时间
                        xSta[_S_CH1_RS_RISE + _CH_DIS(channel)] = tRise;    //获取日升时间长度
                    }
                    fRiseSet = 1;
                }

                if (!fRiseSet) {    //没有有效的日升日落标记
                    xSta[_S_CH1_LT_RISE + _CH_DIS(channel)] = 0;    //日升时间无效,将启动时间清零
                    xSta[_S_CH1_RS_RISE + _CH_DIS(channel)] = 0;    //日升时间无效,为启动时长清零
                }

                if (tSet) {  //有日落时间设置
                    if (tLong >= tSet) {    //半时长比日落时间长，日落时间有效
                        xSta[_S_CH1_LT_SET + _CH_DIS(channel)] = tOff - tSet;  //有日落数据,获取日落启动时间
                        xSta[_S_CH1_RS_SET + _CH_DIS(channel)] = tSet;  //获取日落长度
                    }
                    fRiseSet = 1;
                }

                if (!fRiseSet) {    //没有有效的日升日落标记
                    xSta[_S_CH1_LT_SET + _CH_DIS(channel)] = 0;    //日落时间无效,将启动时间清零
                    xSta[_S_CH1_RS_SET + _CH_DIS(channel)] = 0;    //日落时间无效,为启动时长清零
                }
            }
            //LOG_D("TIMER[%d-%d]-RISE[%d-%d]",
            //       xSta[_S_CH1_LT_ON], xSta[_S_CH1_LT_OFF],
            //xSta[_S_CH1_LT_RISE], xSta[_S_CH1_RS_RISE]);
        } else {    //跨天时间段，不执行日升判断
            //获取更新后的时长
            xSta[_S_CH1_LT_ON + _CH_DIS(channel)] = tOn;
            xSta[_S_CH1_LT_OFF + _CH_DIS(channel)] = tOff;
            xSta[_S_CH1_LT_RISE + _CH_DIS(channel)] = 0;
            xSta[_S_CH1_RS_RISE + _CH_DIS(channel)] = 0;
            xSta[_S_CH1_LT_SET + _CH_DIS(channel)] = 0;
            xSta[_S_CH1_RS_SET + _CH_DIS(channel)] = 0;
        }

    }
}

/**
 * @brief:  灯光时间相关判断
 * @param channel
 * @return
 */
u8 uuz_vLightTime_Query(u8 channel)
{
    u8 state = 0;
    //提取工作时间和延时时间
    u32 ulOnT = xSta[_S_CH1_LT_ON + _CH_DIS(channel)];
    u32 ulOffT = xSta[_S_CH1_LT_OFF + _CH_DIS(channel)];
    u32 ulStart = xSta[_S_CH1_LT_RISE + _CH_DIS(channel)];
    u32 ulStop = xSta[_S_CH1_LT_SET + _CH_DIS(channel)];
    u32 ulTime = uuz_ulRTC_GetSeconds();

    u16 usMode = get_data(_D_LT, _L_CH1_OUTPUT_MODE + _CH_LVL(channel));            //NOTE:当前线路的灯光工作模式

    state = uuzLTS_NULL;        //NOTE:默认无动作
    if (usMode == uuzLTM_AUTO) {    //NOTE:处于自动模式
        LOG_D("STA:%d", xSta[_S_CH1_DAC_STA + _CH_DIS(channel)]);
        //NOTE:通过时间判断端口的开启关闭状态
        //NOTE:在时段内，开启设备[On Time --- Off Time]
        //NOTE:按秒计算是否开启灯光数据
        //NOTE:如果已经关闭完成，直接关闭灯光
        //NOTE:如果当前为开灯状态,需要执行关灯、日落相关状态
        if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_ON) {        //NOTE:当前是开启状态
            if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_NULL) {        //NOTE:如果当前没有报警状态
                if (ulOffT < ulOnT) {   //跨天时间
                    if ((ulTime > ulOffT) && (ulTime < ulOnT)) {   //如果在时间范围之外,关闭灯光
                        state = uuzLTS_OFF;                //关闭灯光标记
                    } else {   //在时间范围之内
                        if (xSta[_S_CH1_DAC_VAL + _CH_DIS(channel)]
                                != get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel))) {   //有强度变化
                            state = uuzLTS_ON;                //开启灯光标记
                        }
                    }
                } else if (ulOffT == ulOnT) {                //开关时间相等
                    state = uuzLTS_OFF;                //关闭灯光标记
                } else {    //非跨天时间
                    if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_RISE) {        //NOTE:有日升标记
                        state = uuzLTS_RISE;      //维持日升标记
                    } else if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_SET) {        //NOTE:有日落标记
                        state = uuzLTS_SET;      //维持日落标记
                    } else {    //非日升或日落状态下
                        if (xSta[_S_CH1_RS_SET + _CH_DIS(channel)] > 0) {        //NOTE:有日落时间数据,标记执行日落工作
                            if (ulTime == ulStop) {  //如果达到日落启动时间点
                                state = uuzLTS_SET;  //日落操作标记
                            } else if ((ulTime <= ulOffT) && (ulTime >= ulOnT)) {    //如果在时间范围之内
                                if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_SET) {  //如果当前已经是日落标记
                                    state = uuzLTS_SET;  //日落操作标记
                                } else {
                                    state = uuzLTS_ON;                //开启灯光标记
                                }
                            } else {    //如果在时间范围之外
                                state = uuzLTS_OFF;    //关闭灯光标记
                            }
                        } else {
                            if ((ulTime > ulOnT) && (ulTime < ulOffT)) {                //在时间范围之内
                                if (xSta[_S_CH1_DAC_VAL + _CH_DIS(channel)]
                                        != get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel))) {        //有强度变化
                                    state = uuzLTS_ON;      //开启灯光标记
                                }
                            } else {    //在时间范围之外
                                state = uuzLTS_OFF;    //关闭灯光标记
                            }
                        }
                    }
                }
            } else if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_CLR) {   //NOTE:原来有报警数据，重置报警措施
                if (ulOffT < ulOnT) {   //跨天时间
                    if ((ulTime > ulOffT) && (ulTime < ulOnT)) {   //如果在时间范围之外,关闭灯光
                        state = uuzLTS_OFF;     //关闭灯光标记
                    } else {
                        state = uuzLTS_ON;      //开启灯光标记
                    }
                } else if (ulOffT == ulOnT) {                //开关时间相等
                    state = uuzLTS_OFF;         //关闭灯光标记
                } else {    //非跨天时间
                    if ((ulTime > ulOnT) && (ulTime < ulOffT)) {                //在时间范围之内
                        if (xSta[_S_CH1_DAC_VAL + _CH_DIS(channel)]
                                != get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel))) {        //有强度变化
                            state = uuzLTS_ON;      //开启灯光标记
                        }
                    } else {
                        state = uuzLTS_OFF;         //关闭灯光标记
                    }
                }
            } else if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_OFF) {  //NOTE:如果当前为关闭的报警状态
                state = uuzLTS_OFF;
            } else if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_DIM) {    //如果是当前为50%调压的报警状态
                state = uuzLTS_DIM;         //如果当前为50%调压的报警状态
                if (ulOffT < ulOnT) {   //开启时间跨天
                    if ((ulTime > ulOffT) && (ulTime < ulOnT)) {    //在时间范围外
                        state = uuzLTS_OFF;         //关闭灯光标记
                    }
                } else if (ulOffT == ulOnT) {
                    state = uuzLTS_OFF;         //关闭灯光标记
                } else {
                    if ((ulTime < ulOnT) || (ulTime > ulOffT)) {
                        state = uuzLTS_OFF;         //关闭灯光标记
                    }
                }
            }
        } else {        //NOTE:如果当前为关灯状态,需要执行关灯、日升相关状态
            //LOG_D("ALARM[%d]-TIMER[%d-%d]-CURR[%d-%d]",
            //xSta[_S_CH1_LT_ALM + _CH_DIS(channel)], ulOnT, ulOffT, ulTime, ulStart);
            if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_NULL) {            //NOTE:如果当前没有报警状态
                if (ulOffT < ulOnT) {   //跨天时间
                    if ((ulTime > ulOffT) && (ulTime < ulOnT)) {   //如果在时间范围之外,关闭灯光
                        state = uuzLTS_OFF;   //关闭灯光状态
                    } else {
                        state = uuzLTS_ON;   //开启灯光状态
                    }
                } else if (ulOffT > ulOnT) {   //没有跨天时间
                    if (xSta[_S_CH1_RS_RISE + _CH_DIS(channel)] > 0) {            //NOTE:有日升时间数据,标记执行日升工作
                        if (ulTime == ulStart) {            //达到日升时间点
                            state = uuzLTS_RISE;            //进入日升状态
                        } else if ((ulTime >= ulOnT) && (ulTime <= ulOffT)) {            //在开启时间范围内
                            if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_RISE) {  //如果当前已经是日升标记
                                state = uuzLTS_RISE;  //开启日升标记
                            } else {
                                state = uuzLTS_ON;            //开启灯光状态
                            }
                        } else {            //在开启时间范围外
                            state = uuzLTS_OFF;            //关闭灯光状态
                        }
                    } else {                    //日升时间 = 0,表示直接打开设备
                        if ((ulTime < ulOnT) || (ulTime > ulOffT)) {   //如果在时间范围之外
                            state = uuzLTS_OFF;   //关闭灯光状态
                        } else {
                            state = uuzLTS_ON;   //开启灯光状态
                        }
                    }
                } else {
                    state = uuzLTS_OFF;   //关闭灯光状态
                }
            } else if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_CLR) {   //NOTE:原来有报警数据，重置报警措施
                state = uuzLTS_ON;   //开启灯光状态
                if (ulOffT < ulOnT) {   //跨天时间
                    if ((ulTime > ulOffT) && (ulTime < ulOnT)) {   //如果在时间范围之外,关闭灯光
                        state = uuzLTS_OFF;   //关闭灯光状态
                    }
                } else if (ulOffT > ulOnT) {   //没有跨天时间
                    if ((ulTime < ulOnT) || (ulTime > ulOffT)) {   //如果在时间范围之外
                        state = uuzLTS_OFF;   //关闭灯光状态
                    }
                }
            } else if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] == uuzLIGHT_ALM_DIM) {
                state = uuzLTS_DIM;   //如果当前为50%调压的报警状态
                if (ulOffT < ulOnT) {   //跨天时间
                    if ((ulTime > ulOffT) && (ulTime < ulOnT)) {   //如果在时间范围之外,关闭灯光
                        state = uuzLTS_OFF;   //关闭灯光状态
                    }
                } else if (ulOffT > ulOnT) {   //没有跨天时间
                    if ((ulTime < ulOnT) || (ulTime > ulOffT)) {   //如果在时间范围之外
                        state = uuzLTS_OFF;   //关闭灯光状态
                    }
                }
            }
        }
    } else if (usMode == uuzLTM_OFF) {  //NOTE:当前为关闭模式
        if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_ON) {  //当前设备输出状态为开启
            state = uuzLTS_OFF;  //关闭设备标记
        }
    } else if (usMode == uuzLTM_ON) {   //NOTE：当前为开启模式
        if (xSta[_S_CH1_DAC_STA + _CH_DIS(channel)] == uuzLTS_OFF) {    //当前设备输出状态为关闭
            state = uuzLTS_ON;  //开启设备标记
        } else {    //当前输出状态为开启
            if (xSta[_S_CH1_DAC_VAL + _CH_DIS(channel)]
                    != get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel)
                            )) {   //当前输出状态和设置灯光强度不一致
                state = uuzLTS_ON;
            }
        }
    }

    return state;   //NOTE:返回当前工作状态
}

/**
 * @brief 启动SUNRISE的动作
 * @param channel
 */
void uuz_vLightSunRise_Start(u8 channel)
{
    //NOTE:初始化日升参数
    xSta[_S_CH1_RS_CUR + _CH_DIS(channel)] = uuzLIGHT_VAL_MIN;    //获取启动光强10%
    xSta[_S_CH1_RS_TUR + _CH_DIS(channel)] = get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel));   //获取目标相关数据,线路设置值
    xSta[_S_CH1_RS_TMP + _CH_DIS(channel)] = xSta[_S_CH1_RS_RISE + _CH_DIS(channel)];   //总日升工作时间

    //计算每段时间长度:计算提升次数,每份1%
    if (xSta[_S_CH1_RS_TUR + _CH_DIS(channel)] > xSta[_S_CH1_RS_CUR + _CH_DIS(channel)]) {  //如果目标强度大于启动最小强度
        xSta[_S_CH1_RS_SMP + _CH_DIS(channel)] = xSta[_S_CH1_RS_RISE + _CH_DIS(channel)]
                / (xSta[_S_CH1_RS_TUR + _CH_DIS(channel)] - xSta[_S_CH1_RS_CUR + _CH_DIS(channel)]);  //每段时间长度=总时间/(光强目标值-光强最小值)
    } else {
        xSta[_S_CH1_RS_SMP + _CH_DIS(channel)] = xSta[_S_CH1_RS_TMP + _CH_DIS(channel)];   //每份时间=总时间
    }

    xSta[_S_CH1_RS_TIM + _CH_DIS(channel)] = 0x00U;  //定时时间清零
    xSta[_S_CH1_RS_STA + _CH_DIS(channel)] = uuzLTS_RISE;   //如果还没有启动日升模式,添加日升标记
    uuz_vLightOpt_Set(channel, uuzLTS_ON, xSta[_S_CH1_RS_CUR + _CH_DIS(channel)], 5);   //以最小强度启动灯光

    //NOTE:CH-2 MODE判断
    if (channel == _CHANNEL_1) {    //如果是线路1和线路2关联
        if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_FOLLOW) {  //关联模式
            uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_ON, xSta[_S_CH1_RS_CUR + _CH_DIS(channel)], 5);    //按最小强度启动线路2设备
        } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INVERSE) {  //互斥模式
            uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 0);  //关闭灯光
        }
    }
}

/**
 * @brief 执行SUN-RISE的事件
 *
 * @param channel
 */
void uuz_vLightSunRise_Event(u8 channel)
{
    //NOTE:如果有日升时间，则启动日升动作
    if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_RISE) {  //当前处于日升状态
        xSta[_S_CH1_RS_TIM + _CH_DIS(channel)]++;  //时间没有到，自动计数添加

        if (xSta[_S_CH1_RS_TIM + _CH_DIS(channel)] >= xSta[_S_CH1_RS_SMP + _CH_DIS(channel)]) {  //判断是否完成一次启动循环
            uuz_vLightOpt_Set(channel, uuzLTS_ON, xSta[_S_CH1_RS_CUR + _CH_DIS(channel)], 5);  //执行当前值动作
            //NOTE:CH-2 MODE判断
            if (channel == _CHANNEL_1) {    //当前为线路1
                if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_FOLLOW) {  //是跟随模式
                    uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_ON, xSta[_S_CH1_RS_CUR + _CH_DIS(channel)], 5);  //同步执行动作
                } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INVERSE) {  //当前是互斥模式
                    uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 0);  //关闭灯光
                }
            }
            if (xSta[_S_CH1_RS_CUR + _CH_DIS(channel)] >= xSta[_S_CH1_RS_TUR + _CH_DIS(channel)]) {  //当前灯光强度大于等于灯光目标值
                uuz_vLightSunRiseSet_Complete(channel);    //完成日升动作，清除日升标记
            }
            xSta[_S_CH1_RS_CUR + _CH_DIS(channel)] += 0x01U;  //每次提高1%亮度
            xSta[_S_CH1_RS_TIM + _CH_DIS(channel)] = 0U;  //阶段达到，准备执行灯光动作,清除定时计数
        }
    }
}

/**
 * @brief 完成日升或日落动作事件
 * @param channel
 */
void uuz_vLightSunRiseSet_Complete(u8 channel)
{
    xSta[_S_CH1_RS_SMP + _CH_DIS(channel)] = 0;
    xSta[_S_CH1_RS_TMP + _CH_DIS(channel)] = 0;
    xSta[_S_CH1_RS_CUR + _CH_DIS(channel)] = 0;
    xSta[_S_CH1_RS_TUR + _CH_DIS(channel)] = 0;
    xSta[_S_CH1_RS_TIM + _CH_DIS(channel)] = 0;
    xSta[_S_CH1_RS_STA + _CH_DIS(channel)] = uuzLTS_NULL;   //完成日升动作，清除日升标记
}

/**
 * @brief 启动SUN-SET的动作
 * @param channel
 */
void uuz_vLightSunSet_Start(u8 channel)
{
    //NOTE:初始化日升参数
    xSta[_S_CH1_RS_CUR + _CH_DIS(channel)] = xSta[_S_CH1_DAC_VAL + _CH_DIS(channel)];    //获取当前光强,线路实时值
    xSta[_S_CH1_RS_TUR + _CH_DIS(channel)] = uuzLIGHT_VAL_MIN;  //获取目标相关数据最小值
    xSta[_S_CH1_RS_TMP + _CH_DIS(channel)] = xSta[_S_CH1_RS_SET + _CH_DIS(channel)];   //总日落工作时间

    //计算每段时间长度:计算提升次数,每份1%
    if (xSta[_S_CH1_RS_TUR + _CH_DIS(channel)] < xSta[_S_CH1_RS_CUR + _CH_DIS(channel)]) {  //如果当前强度大于结束最小强度
        xSta[_S_CH1_RS_SMP + _CH_DIS(channel)] = xSta[_S_CH1_RS_SET + _CH_DIS(channel)]
                / (xSta[_S_CH1_RS_CUR + _CH_DIS(channel)] - xSta[_S_CH1_RS_TUR + _CH_DIS(channel)]);  //每段时间长度=总时间/(光强当前值-光强最小值)
    } else {
        xSta[_S_CH1_RS_SMP + _CH_DIS(channel)] = xSta[_S_CH1_RS_TMP + _CH_DIS(channel)];   //每份时间=总时间
    }
    xSta[_S_CH1_RS_TIM + _CH_DIS(channel)] = 0x00U;  //定时时间清零
    xSta[_S_CH1_RS_STA + _CH_DIS(channel)] = uuzLTS_SET;   //如果还没有启动日落模式,添加日落标记
    uuz_vLightOpt_Set(channel, uuzLTS_ON, xSta[_S_CH1_RS_CUR + _CH_DIS(channel)], 5);   //以当前强度启动灯光

    if (channel == _CHANNEL_1) {    //如果是线路1和线路2关联
        if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_FOLLOW) {  //关联模式
            uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_ON, xSta[_S_CH1_RS_CUR + _CH_DIS(channel)], 5);    //按当前强度启动线路2设备
        } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INVERSE) {  //互斥模式
            uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 0);  //关闭灯光
        }
    }
}

/**
 *
 * @brief 启动SUNSET的事件
 * @param channel
 */
void uuz_vLightSunSet_Event(u8 channel)
{
    //NOTE:如果有日落时间，则启动日落动作
    if (xSta[_S_CH1_RS_STA + _CH_DIS(channel)] == uuzLTS_SET) {  //当前处于日落状态
        xSta[_S_CH1_RS_TIM + _CH_DIS(channel)]++;  //时间没有到，自动计数添加
        if (xSta[_S_CH1_RS_TIM + _CH_DIS(channel)] >= xSta[_S_CH1_RS_SMP + _CH_DIS(channel)]) {  //判断是否完成一次启动循环

            uuz_vLightOpt_Set(channel, uuzLTS_ON, xSta[_S_CH1_RS_CUR + _CH_DIS(channel)], 5);  //执行当前值动作
            //NOTE:CH-2 MODE判断
            if (channel == _CHANNEL_1) {    //当前为线路1
                if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_FOLLOW) {  //是跟随模式
                    uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_ON, xSta[_S_CH1_RS_CUR + _CH_DIS(channel)], 5);  //同步执行动作
                } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INVERSE) {  //当前是互斥模式
                    uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 0);  //关闭灯光
                }
            }

            if (xSta[_S_CH1_RS_CUR + _CH_DIS(channel)] <= xSta[_S_CH1_RS_TUR + _CH_DIS(channel)]) {  //当前灯光强度小于等于灯光目标值
                uuz_vLightOpt_Set(channel, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 5);    //达到日落最低值，关闭灯光
                //NOTE:CH-2 MODE判断
                if (channel == _CHANNEL_1) {    //当前为线路1
                    if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_FOLLOW) {  //是跟随模式
                        uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_OFF, uuzLIGHT_VAL_OFF, 0);  //关闭灯光
                    } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INVERSE) {  //当前是互斥模式
                        if (xSta[_S_CH1_LT_ALM + _CH_DIS(channel)] != uuzLIGHT_ALM_OFF) {   //如果是过热报警状态，不触发联动
                            uuz_vLightOpt_Set(_CHANNEL_2, uuzLTS_ON,
                                    get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(channel)), 0);   //执行开启设备动作
                        }
                    }
                }

                uuz_vLightSunRiseSet_Complete(channel);    //完成日落动作，清除日落标记
            }
            xSta[_S_CH1_RS_CUR + _CH_DIS(channel)] -= 0x01U;  //每次提高1%亮度
            xSta[_S_CH1_RS_TIM + _CH_DIS(channel)] = 0U;  //阶段达到，准备执行灯光动作,清除定时计数
        }
    }
}

/**
 *
 * @brief 投入灯光相关数据
 * @param channel   :相关频道
 * @param ucSta     :工作状态
 * @param ucValue   :亮度等级
 * @param ucDelay   :等待延时
 */
void uuz_vLightOpt_Set(u8 channel, u8 ucSta, u8 ucValue, u8 ucDelay)
{
    xSta[_S_CH1_OPT_STA + _CH_DIS(channel)] = ucSta;
    xSta[_S_CH1_OPT_VAL + _CH_DIS(channel)] = ucValue;
    xSta[_S_CH1_OPT_DT + _CH_DIS(channel)] = ucDelay;
    xSta[_S_CH1_OPT_EN + _CH_DIS(channel)] = 0x01U;
}

/**
 * @brief 设备执行相关阶段
 *
 */
void uuz_vLightOpt_Event(u8 channel)
{
    //LOG_I("En:%d-Sta:%d-Val:%d", xLightCache.opt[channel].ucEnable, xLightCache.opt[channel].ucSta, LightCache.opt[channel].ucValue);
    if (xSta[_S_CH1_OPT_EN + _CH_DIS(channel)]) {
        //NOTE:判断LIGHT设备的相关状态
        //输出对应的DAC
        if (xSta[_S_CH1_OPT_STA + _CH_DIS(channel)] == uuzLTS_ON) {
            if (xSta[_S_CH1_OPT_DT + _CH_DIS(channel)]) {
                //如果有延时，执行相关延时时间
                rt_thread_mdelay(xSta[_S_CH1_OPT_DT + _CH_DIS(channel)]);
            }
            dac_set_value(channel, xSta[_S_CH1_OPT_VAL + _CH_DIS(channel)]);        //输出相关DAC
            rt_light_ch_event(channel, uuzLTS_ON);
        } else {
            dac_set_value(channel, uuzLIGHT_VAL_OFF);        //关闭输出
            rt_light_ch_event(channel, uuzLTS_OFF);
        }
        //LOG_D("xSta[%d]:%d", _S_LED_CH1 + _CH_LVL(channel), xSta[_S_LED_CH1 + _CH_LVL(channel)]);

        //ECM Config Operate
        //a CO2 source, light or watering unit may be activated during lightson periods.
        //A heater may be activated during lights-off periods.
        //Channel 2 can only be set if it operates in independent mode.
        //If it operates in follow mode, the setting of channel 1 will be followed.
        //If it operates in inverse mode, the setting of channel 1 will be inversed.
        if (xSta[_S_CH1_OPT_STA + _CH_DIS(channel)] == uuzLTS_OFF) {
            if (xSysConfig.Light[_L_CH1_ECM_MODE + _CH_LVL(channel)] == _LIGHT_ECM_DAY) {
                if (channel == _CHANNEL_1) {
                    rt_pin_write(ECM1_CTR, PIN_HIGH);
                } else {
                    rt_pin_write(ECM2_CTR, PIN_HIGH);
                }
            } else {
                if (channel == _CHANNEL_1) {
                    rt_pin_write(ECM1_CTR, PIN_LOW);
                } else {
                    rt_pin_write(ECM2_CTR, PIN_LOW);
                }
            }
        } else {
            if (xSysConfig.Light[_L_CH1_ECM_MODE + _CH_LVL(channel)] == _LIGHT_ECM_NIGHT) {
                if (channel == _CHANNEL_1) {
                    rt_pin_write(ECM1_CTR, PIN_HIGH);
                } else {
                    rt_pin_write(ECM2_CTR, PIN_HIGH);
                }
            } else {
                if (channel == _CHANNEL_1) {
                    rt_pin_write(ECM1_CTR, PIN_LOW);
                } else {
                    rt_pin_write(ECM2_CTR, PIN_LOW);
                }
            }
        }
        xSta[_S_CH1_OPT_EN + _CH_DIS(channel)] = 0;  //清除执行标记
    }
}

/**
 * @brief 处理温度相关的NO-NC输出和灯光输出
 *
 */
void uuz_vLightALM_Opt(void)
{
    //输出NO/NC的相关信息
    if (
    ((xSta[_S_CH1_TEMP_EN] == 1)  //线路1有报警数据
    && ((xSta[_S_CH1_LT_ALM] == uuzLIGHT_ALM_OFF) || (xSta[_S_CH1_LT_ALM] == uuzLIGHT_ALM_DIM)))
            || ((xSta[_S_CH2_TEMP_EN] == 1)  //线路2有报警数据
            && ((xSta[_S_CH2_LT_ALM] == uuzLIGHT_ALM_OFF) || (xSta[_S_CH2_LT_ALM] == uuzLIGHT_ALM_DIM)))) {
        rt_pin_write(ALM_CTR, PIN_HIGH);  //切换ALM为NO,输出报警
    } else {
        rt_pin_write(ALM_CTR, PIN_LOW);  //切换ALM为NC,关闭输出报警
    }

    //LOG_D("a1:%d-a2:%d", xSta[_S_CH1_LT_ALM], xSta[_S_CH2_LT_ALM]);
    u8 ucIsAlarm = 0;

    //有线路1数据
    if (xSta[_S_CH1_TEMP_EN] == 1) {
        if (xSta[_S_CH1_LT_ALM] == uuzLIGHT_ALM_OFF) {
            if (xSta[_S_LED_TA1] == 0) {
                led_blink(_LD_TA1, 250, 300);  //打开闪烁灯光(快：250-300)
                //标记灯光开关状态
                xSta[_S_LED_TA1] = 2;
            }
            ucIsAlarm = 1;
        } else if (xSta[_S_CH1_LT_ALM] == uuzLIGHT_ALM_DIM) {
            if (xSta[_S_LED_TA1] == 0) {
                led_blink(_LD_TA1, 500, 500);                //打开闪烁灯光(中：500-500)
                //标记灯光开关状态
                xSta[_S_LED_TA1] = 1;
            }
            ucIsAlarm = 1;
        }
    } else {
        if (xSta[_S_LED_TA1] == 0) {                //没有传感器
            led_blink(_LD_TA1, 800, 1000);                //打开闪烁灯光(慢：800-1000)
            xSta[_S_LED_TA1] = 3;                //标记灯光开关状态
        }
        ucIsAlarm = 1;
    }

    //NOTE:没有报警，关闭灯光闪烁
    if (ucIsAlarm == 0) {
        led_off(_LD_TA1);
        xSta[_S_LED_TA1] = 0;
    }

    //有线路2数据
    ucIsAlarm = 0;
    if (xSta[_S_CH2_TEMP_EN] == 1) {
        if (xSta[_S_CH2_LT_ALM] == uuzLIGHT_ALM_OFF) {

            if (xSta[_S_LED_TA2] == 0) {
                led_blink(_LD_TA2, 250, 300);                //打开闪烁灯光(快：250-300)
                //标记灯光开关状态
                xSta[_S_LED_TA2] = 2;
            }
            ucIsAlarm = 1;
        } else if (xSta[_S_CH2_LT_ALM] == uuzLIGHT_ALM_DIM) {

            if (xSta[_S_LED_TA2] == 0) {
                led_blink(_LD_TA2, 500, 500);                //打开闪烁灯光(中：500-500)
                //标记灯光开关状态
                xSta[_S_LED_TA2] = 1;
            }
            ucIsAlarm = 1;
        }
    } else {
        //没有传感器
        //打开闪烁灯光(慢：800-1000)
        if (xSta[_S_LED_TA2] == 0) {
            led_blink(_LD_TA2, 800, 1000);
            xSta[_S_LED_TA2] = 3;        //标记灯光开关状态
        }
        ucIsAlarm = 1;
    }

    //NOTE:没有报警，关闭灯光闪烁
    if (ucIsAlarm == 0) {
        led_off(_LD_TA2);
        xSta[_S_LED_TA2] = 0;
    }
}

/**
 * @brief 清除灯光的输出状态
 *
 */
void rt_light_init(void)
{

    dac_set_value(_CHANNEL_1, uuzLIGHT_VAL_OFF);  //关闭灯光输出
    rt_pin_write(ECM1_CTR, PIN_HIGH);  //关闭ECM输出
    dac_set_value(_CHANNEL_2, uuzLIGHT_VAL_OFF);  //关闭灯光输出
    rt_pin_write(ECM2_CTR, PIN_HIGH);  //关闭ECM输出
    rt_pin_write(ALM_CTR, PIN_LOW);  //切换ALM为NC

    //初始化CH1/CH2灯光状态
    rt_light_event();
}

/**
 * @brief 灯光状态处理
 *
 */
void rt_light_event(void)
{
    //如果设置为0表示关闭
    //线路1
    //LOG_I("CH1:%d-CH2:%d", xSta[_S_LED_CH1 + _CH_DIS(_CHANNEL_1)], xSta[_S_LED_CH1 + _CH_DIS(_CHANNEL_2)]);
    if (xSta[_S_CH1_DAC_STA] > 0) {
        //打开灯光
        if (xSta[_S_LED_CH1] == 0) {
            led_on(_LD_CH1);
            xSta[_S_LED_CH1] = 1;
        }
    } else {
        //关闭灯光
        led_off(_LD_CH1);
        xSta[_S_LED_CH1] = 0;
    }

    //线路2
    if (xSta[_S_CH2_DAC_STA] > 0) {
        //打开灯光
        if (xSta[_S_LED_CH2] == 0) {
            led_on(_LD_CH2);
            xSta[_S_LED_CH2] = 1;
        }
    } else {
        //关闭灯光
        led_off(_LD_CH2);
        xSta[_S_LED_CH2] = 0;
    }
}

/**
 * @brief  单路状态切换
 *
 * @param ch
 */
void rt_light_ch_event(u8 ch, u8 state)
{
    //如果设置为0表示关闭
    //线路1 / 线路2
    if (state > 0) {
        //打开灯光
        if (xSta[_S_LED_CH1 + _CH_DIS(ch)] == 0) {
            led_on(_LD_CH1 + ch);
            xSta[_S_LED_CH1 + _CH_DIS(ch)] = 1;
        }
    } else {
        //关闭灯光
        led_off(_LD_CH1 + ch);
        xSta[_S_LED_CH1 + _CH_DIS(ch)] = 0;
    }
}
