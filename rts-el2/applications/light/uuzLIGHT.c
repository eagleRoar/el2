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
/******************************************************************************/
#include "typedefDEF.h"
#include "uuzOpt.h"
#include "uuzTEMP.h"
#include "uuzRTC.h"
#include "uuzDAC.h"
#include "uuzGPIO.h"
#include "uuzLED.h"
/******************************************************************************/
#include "typedefUI.h"
#include "uuzConfigLIGHT.h"
#include "uuzConfig1602.h"
#include "uuzUI.h"
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
LightCache_Typedef_t xLightCache;
u32 testCount = 0;
extern struct rt_messagequeue mq;//Justin debug

int test(void)
{
    rt_err_t ret = RT_EOK;

    /*创建线程*/
    rt_thread_t thread = rt_thread_create("test_rd", test_entry, RT_NULL, 1024, 18, 20);
    rt_thread_t thread1 = rt_thread_create("test_rd1", test_entry1, RT_NULL, 1024, 18, 20);
    /*创建线程成功,之后启动线程*/
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        LOG_I("start thread [test]");
    } else {
        LOG_E("start thread [test] failed");
        ret = RT_ERROR;
    }

    if(thread1 != RT_NULL)
    {
        rt_thread_startup(thread1);
        LOG_I("start thread [test]");
    } else {
        LOG_E("start thread [test] failed");
        ret = RT_ERROR;
    }


    return ret;
}

void test_entry1(void* parameter)
{
    while(1)
    {
        testCount++;

        rt_thread_delay(500);
    }
}

void test_entry(void* parameter)
{
    u8 mqNum = 0;
    rt_err_t ret = RT_EOK;

    while(1)
    {
        mqNum = testCount % 10;

        ret = rt_mq_send(&mq, &mqNum, 1);

        if(ret != RT_EOK)
        {
            LOG_E("send messagequeue failed");
        }

        testCount++;
        //LOG_I("count = %d",testCount);

        rt_thread_delay(500);
    }
}

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
    RT_NULL, 2048, 18, 20);

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
    //数据当前有效值
    while (1) {
        if (xSta[_S_FLAG_STARTUP]) {
            //如果是手动状态
            if (xUI.cID != uuzUI_LIGHT) {
                //判断延时等待时间过去后，再进入工作模式
                uuz_vLightALM_Event(_CHANNEL_1);
                //初始化Light的相关状态
                uuz_vLightTime_Query(_CHANNEL_1);
                //初始化Light的SunRS相关状态
                uuz_vLightSunRS_Query(_CHANNEL_1);
                //1秒执行1次
                if (delayCount % 2 == 0) {
                    //触发延时后的事件处理
                    uuz_vLightSunRS_Event(_CHANNEL_1);
                    //执行Light的相关事件
                    uuz_vLightOpt_Event(_CHANNEL_1);
                }

                //判断延时等待时间过去后，再进入工作模式
                uuz_vLightALM_Event(_CHANNEL_2);
                //初始化Light的相关状态
                uuz_vLightTime_Query(_CHANNEL_2);
                //初始化Light的SunRS相关状态
                uuz_vLightSunRS_Query(_CHANNEL_2);
                if (delayCount % 2 == 0) {
                    //触发延时后的事件处理
                    uuz_vLightSunRS_Event(_CHANNEL_2);
                    //执行Light的相关事件
                    uuz_vLightOpt_Event(_CHANNEL_2);
                }
            }
        }

        delayCount++;
        //每1000ms一次
        rt_thread_delay(500);
    }
}

/**
 * @brief 投入灯光相关数据
 *
 */
void uuz_vLightOpt_Set(u8 channel, u8 ucSta, u8 ucValue, u8 ucDelay)
{
    xLightCache.opt[channel].ucEnable = 0x01U;
    xLightCache.opt[channel].ucSta = ucSta;
    xLightCache.opt[channel].ucValue = ucValue;
    xLightCache.opt[channel].ucDelay = ucDelay;
}

/**
 *
 * @brief:  灯光报警相关判断
 * @param channel
 */
void uuz_vLightALM_Event(u8 channel)
{
    u16 usCurrentTP = 0;
    u8 ucAlarmOpt = 0;
    u8 ucAlarmToFlash = 0;

    //频道1有温度相关设备
    if (xSta[_S_CH1_TEMP_EN + _CH_DIS(channel)] == 1) {
        usCurrentTP = xSta[_S_CH1_TEMP_VAL + _CH_DIS(channel)];
        //判断是否达到LIGHT的温度相关设置,自动关闭亮度为50%
        if (usCurrentTP >= xSysConfig.Light[_L_CH1_AUTO_DIM_TEMP + _CH_LVL(channel)]) {
            //判断有效标记
            if (xLightCache.xAutoDim[channel].sta < uuzALM_MAX) {
                xLightCache.xAutoDim[channel].sta++;
            }
        } else {
            if (xLightCache.xAutoDim[channel].sta > 0) {
                //离开报警状态，清除标记
                xLightCache.xAutoDim[channel].sta--;
            }
        }
        //达到满足50%的报警状态
        if (xLightCache.xAutoDim[channel].sta == uuzALM_MAX) {
            //判断是否需要发送相关数据
            if (xLightCache.xAutoDim[channel].sta_p != xLightCache.xAutoDim[channel].sta) {
                xLightCache.xAutoDim[channel].sta_p = xLightCache.xAutoDim[channel].sta;
            }
        }
        //判断是否达到LIGHT的温度相关设置 100%,自动关闭灯光
        if (usCurrentTP >= xSysConfig.Light[_L_CH1_SHUTDOWN_TEMP + _CH_LVL(channel)]) {
            //判断有效标记
            if (xLightCache.xAutoOff[channel].sta < uuzALM_MAX) {
                xLightCache.xAutoOff[channel].sta++;
            }
        } else {
            if (xLightCache.xAutoOff[channel].sta > 0) {
                //离开报警状态，清除标记
                xLightCache.xAutoOff[channel].sta--;
            }
        }
        //满足关闭的报警状态
        if (xLightCache.xAutoOff[channel].sta == uuzALM_MAX) {
            //判断是否需要发送相关数据
            if (xLightCache.xAutoOff[channel].sta_p != xLightCache.xAutoOff[channel].sta) {
                xLightCache.xAutoOff[channel].sta_p = xLightCache.xAutoOff[channel].sta;
            }
        }

        //没有报警数据
        ucAlarmOpt = uuzLIGHT_ALM_NULL;
        //有自动关闭的报警数据
        if (xLightCache.xAutoOff[channel].sta_p == uuzALM_MAX) {
            //保存记录
            ucAlarmOpt = uuzLIGHT_ALM_OFF;
            set_logs((_G_CH1_SHUTDOWN + channel), NULL, 0);
            ucAlarmToFlash = 1;

            //打开高速灯光
            if (xSta[_S_LED_TA1 + _CH_DIS(channel)] != 2) {
                led_blink(_LD_TA1 + channel, 250, 300);
                //标记灯光状态
                xSta[_S_LED_TA1 + _CH_DIS(channel)] = 2;
            }
        }

        //有自动降温的报警数据
        if (ucAlarmOpt == uuzLIGHT_ALM_NULL) {
            if (xLightCache.xAutoDim[channel].sta_p == uuzALM_MAX) {
                ucAlarmOpt = uuzLIGHT_ALM_DIM;
                set_logs((_G_CH1_AUTO_DIM + channel), NULL, 0);
                ucAlarmToFlash = 1;
                //打开快速灯光
                if (xSta[_S_LED_TA1 + _CH_DIS(channel)] != 1) {
                    led_blink(_LD_TA1 + channel, 500, 500);
                    xSta[_S_LED_TA1 + _CH_DIS(channel)] = 1;
                }
            }
        }

        if (ucAlarmOpt == uuzLIGHT_ALM_NULL) {
            //关闭灯光
            led_off(_LD_TA1 + channel);
            xSta[_S_LED_TA1 + _CH_DIS(channel)] = 0;
        }

        //有历史数据,清除状态
        if (xLightCache.alarmSta[channel] != ucAlarmOpt) {
            xLightCache.alarmSta[channel] = ucAlarmOpt;
        }
    } else {
        //没有温度实际数据，清空报警数据,根据需要保存数据状态
        xLightCache.alarmSta[channel] = uuzLIGHT_ALM_NULL;
        if (xLightCache.xAutoDim[channel].sta_p != xLightCache.xAutoDim[channel].sta) {
            xLightCache.xAutoDim[channel].sta_p = 0;
        }
        if (xLightCache.xAutoOff[channel].sta_p != xLightCache.xAutoOff[channel].sta) {
            xLightCache.xAutoOff[channel].sta_p = 0;
        }
    }

    //保存日志数据
    if (ucAlarmToFlash) {
        save_logs();
    }
}

/**
 * @brief:  灯光时间相关判断
 */
void uuz_vLightTime_Query(u8 channel)
{
    u8 state;
    u16 usRTCTime = uuz_usRTC_GetMinutes();

    //根据CH-2逻辑，判断对应的工作状态
    if (channel == _CHANNEL_2) {
        if (xSysConfig.Light[_L_CH2_MODE] == uuzLIGHT_FOLLOW) {
            xLightCache.ucTimerSta[channel] = xLightCache.ucTimerSta[_CHANNEL_1];
            //获取到状态，返回数据
            return;
        } else if (xSysConfig.Light[_L_CH2_MODE] == uuzLIGHT_INVERSE) {
            xLightCache.ucTimerSta[channel] = (~xLightCache.ucTimerSta[_CHANNEL_1]);
            //获取到状态，返回数据
            return;
        }
    }

    if (xSysConfig.Light[_L_CH1_OUTPUT_MODE + _CH_LVL(channel)] == uuzLIGHT_AUTO) {
        //通过时间判断端口的开启关闭状态
        if ((usRTCTime >= xSysConfig.Light[_L_CH1_CYCLE_ON + _CH_LVL(channel)])
                && (usRTCTime < xSysConfig.Light[_L_CH1_CYCLE_OFF + _CH_LVL(channel)])) {
            //在时段内，开启设备[On Time --- Off Time]
            state = uuzLIGHT_ON;
        } else {
            //在上升下降的延时时间内
            if (xLightCache.delaySta[channel]) {
                state = uuzLIGHT_DELAY;
            } else {
                state = uuzLIGHT_OFF;
            }
        }
    } else if (xSysConfig.Light[_L_CH1_OUTPUT_MODE + _CH_LVL(channel)] == uuzLIGHT_ON) {
        state = uuzLIGHT_ON;
    } else if (xSysConfig.Light[_L_CH1_OUTPUT_MODE + _CH_LVL(channel)] == uuzLIGHT_OFF) {
        state = uuzLIGHT_OFF;
    }
    //判断当前状态, 设置工作状态
    xLightCache.ucTimerSta[channel] = state;
}

/*
 * @note:  启动SunRS的延时动作,设定启动函数
 */
void uuz_vLightSunRS_Query(u8 channel)
{
    //有日升数据
    if (xLightCache.alarmSta[channel] == uuzLIGHT_ALM_OFF) {
        //有报警信息，立即关闭
        xLightCache.ucTimerSta[channel] = uuzLIGHT_OFF;
        //报警数据，直接关闭设备
        uuz_vLightOpt_Set(channel, uuzLIGHT_OFF, uuzLIGHT_VAL_OFF, 0);
    } else {
        //Light有需要发送的状态数据
        if (xLightCache.ucTimerSta[channel] == uuzLIGHT_ON) {
            //如果是自动模式
            if (xSysConfig.Light[_L_CH1_OUTPUT_MODE + _CH_LVL(channel)] == uuzLIGHT_AUTO) {
                //如果有延时数据
                if (xSysConfig.Light[_L_CH1_SUNRISE_DELAY + _CH_LVL(channel)] > 0) {
                    //未有延时标记
                    if (xLightCache.delaySta[channel] == 0) {
                        //配置启动灯光梯度阶段
                        xLightCache.val_m[channel] = uuzLIGHT_STEP_VAL;
                        //目标开启设备,无报警状态,设置当前需要达到的目标亮度
                        if (xLightCache.alarmSta[channel] == uuzLIGHT_ALM_NULL) {
                            xLightCache.val_t[channel] = xSysConfig.Light[_L_CH1_OUTPUT_LEVEL + _CH_LVL(channel)];
                        } else if (xLightCache.alarmSta[channel] == uuzLIGHT_ALM_DIM) {
                            xLightCache.val_t[channel] = uuzLIGHT_VAL_DIM;
                        }
                        //设置启动亮度，如果是灭到亮,设置50%
                        xLightCache.val_c[channel] = uuzLIGHT_VAL_DIM;
                        //当前值小于目标值
                        if (xLightCache.val_c[channel] < xLightCache.val_t[channel]) {
                            //C < T:需要提升
                            //获得需要切换的总时间
                            //-------------------->获取工作时间段 ( 0 ~ N )秒
                            xLightCache.time_c[channel] = 0;
                            //获取每一阶段的时间长度
                            xLightCache.time_t[channel] = uuz_vLightSunRS_Set(xLightCache.val_t[channel]);
                            //执行标记
                            xLightCache.timeSta[channel] = 1;                        //Delay Open
                            //立即进入延时等待状态
                            xLightCache.delaySta[channel] = 1;
                        } else {
                            //C >= T:不需要提升，直接开启设备
                            uuz_vLightOpt_Set(channel, uuzLIGHT_ON, xLightCache.val_t[channel], 0);
                        }
                    }
                } else {
                    //C >= T:不需要提升，直接开启设备
                    uuz_vLightOpt_Set(channel, uuzLIGHT_ON, xSysConfig.Light[_L_CH1_OUTPUT_LEVEL + _CH_LVL(channel)],
                            0);
                }
            } else {
                //不是自动模式，立即开启/关闭数据
                uuz_vLightOpt_Set(channel, uuzLIGHT_ON, xSysConfig.Light[_L_CH1_OUTPUT_LEVEL + _CH_LVL(channel)], 0);
            }
        }

        //有日落数据
        if (xLightCache.ucTimerSta[channel] == uuzLIGHT_OFF) {
            //如果是自动模式
            if (xSysConfig.Light[_L_CH1_OUTPUT_MODE + _CH_LVL(channel)] == uuzLIGHT_AUTO) {
                //如果有延时数据
                if (xSysConfig.Light[_L_CH1_SUNSET_DELAY + _CH_LVL(channel)] > 0) {
                    //需要关闭设备，判断是否需要降落关闭
                    if (xLightCache.delaySta[channel] == 0) {
                        xLightCache.val_m[channel] = uuzLIGHT_STEP_VAL;
                        //记录当前输出的强度
                        xLightCache.val_c[channel] = dac_to_level(channel, xSta[_S_CH1_DAC_OUT + _CH_DIS(channel)]);
                        xLightCache.val_t[channel] = uuzLIGHT_VAL_DIM;
                        //当前状态判断
                        if (xLightCache.val_c[channel] > xLightCache.val_t[channel]) {
                            //-------------------->获取工作时间段 ( 0 ~ N )秒
                            xLightCache.time_c[channel] = 0;
                            //获取每一阶段的时间长度
                            xLightCache.time_t[channel] = uuz_vLightSunRS_Set(xLightCache.time_c[channel]);
                            //执行标记
                            xLightCache.timeSta[channel] = 1;                //Delay Open
                            //立即进入延时等待状态
                            xLightCache.delaySta[channel] = 1;
                        } else {
                            //C <= T:不需要提升，直接关闭设备
                            uuz_vLightOpt_Set(channel, uuzLIGHT_OFF, uuzLIGHT_VAL_OFF, 0);
                        }
                    }
                } else {
                    //C >= T:不需要提升，直接关闭设备
                    uuz_vLightOpt_Set(channel, uuzLIGHT_OFF, uuzLIGHT_VAL_OFF, 0);
                }
            } else {
                //不是自动模式，立即开启/关闭数据
                uuz_vLightOpt_Set(channel, uuzLIGHT_OFF, uuzLIGHT_VAL_OFF, 0);
            }
        }
    }
}

/*
 * @note:  启动SunRS的延时动作事件
 */
void uuz_vLightSunRS_Event(u8 channel)
{
    if (xSysConfig.Light[_L_CH1_OUTPUT_MODE + _CH_LVL(channel)] == uuzLIGHT_AUTO) {
        //有延时数据
        if (xLightCache.ucTimerSta[channel] == uuzLIGHT_ON) {
            //有延时 && 有操作
            if ((xLightCache.delaySta[channel]) && (xLightCache.timeSta[channel])) {
                //方向判断--开启方向-->ON
                if (xLightCache.val_c[channel] >= xLightCache.val_t[channel]) {
                    //已经到最大值了,停止延时指令
                    xLightCache.delaySta[channel] = 0;
                    xLightCache.timeSta[channel] = 0;
                    uuz_vLightOpt_Set(channel, uuzLIGHT_ON, xLightCache.val_t[channel], 0);
                    //关闭操作趋势
                    xLightCache.ucTimerSta[channel] = uuzLIGHT_NULL;
                } else {
                    uuz_vLightOpt_Set(channel, uuzLIGHT_ON, xLightCache.val_c[channel], 0);
                    //刷新强度数据
                    xLightCache.val_c[channel] += xLightCache.val_m[channel];
                    //增加后的值大于最大值，将最大值等于当前执行值,下次结束Sun Rise
                    if (xLightCache.val_c[channel] >= xLightCache.val_t[channel]) {
                        xLightCache.val_c[channel] = xLightCache.val_t[channel];
                    }
                }
                xLightCache.timeSta[channel] = 0;
            }
        } else if (xLightCache.ucTimerSta[channel] == uuzLIGHT_OFF) {
            //有延时 && 有操作
            if ((xLightCache.delaySta[channel]) && (xLightCache.timeSta[channel])) {
                //方向判断--关闭方向
                if (xLightCache.val_c[channel] <= xLightCache.val_t[channel]) {
                    //已经到最大值了,停止延时指令
                    xLightCache.delaySta[channel] = 0;
                    xLightCache.timeSta[channel] = 0;
                    uuz_vLightOpt_Set(channel, uuzLIGHT_OFF, uuzLIGHT_VAL_OFF, 0);
                    //关闭操作趋势
                    xLightCache.ucTimerSta[channel] = uuzLIGHT_NULL;
                } else {
                    uuz_vLightOpt_Set(channel, uuzLIGHT_ON, xLightCache.val_c[channel], 0);
                    //刷新强度数据
                    xLightCache.val_c[channel] -= xLightCache.val_m[channel];
                }
                xLightCache.timeSta[channel] = 0;
            }
        } else {
            xLightCache.delaySta[channel] = 0;
            xLightCache.timeSta[channel] = 0;
        }

        //阶段时间增加
        xLightCache.time_c[channel]++;
        //开启或者关闭方向
        //判断是否达到一个工作阶段
        if (xLightCache.time_c[channel] >= xLightCache.time_t[channel]) {
            xLightCache.time_c[channel] = 0;
            xLightCache.timeSta[channel] = 0;
        }
    }
}

/**
 * @note:  读取SunRS模式下的时间参数
 */
u32 uuz_vLightSunRS_Set(u8 time)
{
    u8 ucStep = 0;
    u32 ulTimeOut = xLightCache.xConfig.usSunRS * 60;
    //实时获得需要切换的阶段数
    //在usDelay时间内，从50%依次开启灯光，按阶梯提高灯光亮度,每次5%
    //在usDelay时间内，从T依次关闭灯光，按阶梯减少灯光亮度,每次5%
    ucStep = (time - uuzLIGHT_VAL_DIM) / uuzLIGHT_STEP_VAL;
    return (ulTimeOut / ucStep);
}

/*
 * @note:   关闭延时启动、关闭动作Mask
 */
void uuz_vLightSunRS_Stop(u8 channel)
{
    //关闭灯光清除延时启动参数
    if (xLightCache.delaySta[channel]) {
        xLightCache.delaySta[channel] = 0;
        xLightCache.timeSta[channel] = 0;
    }
}

/**
 * @brief 设备执行相关阶段
 *
 */
void uuz_vLightOpt_Event(u8 channel)
{
    //判断LIGHT设备的相关状态
    if (xLightCache.opt[channel].ucEnable) {
        //输出对应的DAC
        if (xLightCache.opt[channel].ucSta == uuzLIGHT_ON) {
            //输出相关设备
            level_to_dac(channel, xLightCache.opt[channel].ucValue);
            //打开灯光
            if (xSta[_S_LED_CH1 + _CH_DIS(channel)] == 0) {
                led_on(_LD_CH1 + channel);
                xSta[_S_LED_CH1 + _CH_DIS(channel)] = 1;
            }
        } else {
            //关闭输出
            level_to_dac(channel, uuzLIGHT_VAL_OFF);
            //关闭灯光
            led_off(_LD_CH1 + channel);
            xSta[_S_LED_CH1 + _CH_DIS(channel)] = 0;
        }
        //LOG_D("xSta[%d]:%d", _S_LED_CH1 + _CH_LVL(channel), xSta[_S_LED_CH1 + _CH_LVL(channel)]);

        //ECM Config Operate
        //a CO2 source, light or watering unit may be activated during lightson periods.
        //A heater may be activated during lights-off periods.
        //Channel 2 can only be set if it operates in independent mode.
        //If it operates in follow mode, the setting of channel 1 will be followed.
        //If it operates in inverse mode, the setting of channel 1 will be inversed.
        if (xLightCache.opt[channel].ucValue == uuzLIGHT_OFF) {
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
        } else {
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
        }
        //清除执行标记
        xLightCache.opt[channel].ucEnable = 0;
    }
}
