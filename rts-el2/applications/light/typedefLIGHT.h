#ifndef __TYPEDEF_LIGHT_H
#define __TYPEDEF_LIGHT_H
#include "typedefDEF.h"

typedef struct exlda1config_t
{

    u8 ucEnable;    //设备数据有效性
    u8 ucOutMax[2];    //0~255
    u8 ucLedType[2];    //LED|HIM模式
    u8 ucMode[2];    //LINE1-Auto/OFF|LING2-Invset/follow
    u16 usHIDDelay[2];    //HID Delay Time
    u16 usOn[2];    //启动时间
    u16 usOff[2];    //结束时间
    //the Same Data
    u16 usSunRS;    //Sun Rise | Sun Set
    u16 usDimTp;    //降频温值
    u16 usOffTp;    //关闭温度值

} ExLDA1Config_Typedef_t;
#define uuzLDA1_CONFIG_LEN sizeof(ExLDA1Config_Typedef_t)

typedef struct exlda1value_t
{

    u8 ucSta; /* 0-OFF or 1-ON */
    u8 ucValue; /* 0 ~ 255%*/
    u8 ucSence; /* 当前场景数量 */
    u8 ucIsSame; /* 设备趋势数量,表示线路趋势是开启还是关闭 */

} ExLDA1Value_Typedef_t;

typedef struct exlda1opt_t
{

    u8 ucEnable; /* 需要执行设备:0 or 1*/
    u8 ucDelay; /* 延时设备标记*/
    u8 ucSta; /* 0-OFF or 1-ON */
    u8 ucValue; /* 0 ~ 255%*/

} ExLDA1_Opt_Typedef_t;

typedef struct flagsta_t
{
    u8 sta;    //设备状态
    u8 sta_p;    //记录设备状态
} FlagSta_t;

typedef struct exlda1cache_t
{
    //临时配置
    ExLDA1Config_Typedef_t xConfig;

    //温度报警信号
    //50% Mode Alarm
    u8 ucDimMask;
    u8 ucDimMask_Prev;

    //OFF Mode Alarm
    u8 ucOffMask;
    u8 ucOffMask_Prev;

    //温度报警执行标记
    u8 ucAlarmOpt;

    //HID启动延时
    u8 ucHID_Mask[2];
    u32 ulHID_TimeOut[2];
    u32 ulHID_TimeMax[2];

    u8 ucOptMask[2];    //延时缓存动作标记
    u32 ulTime_C[2];    //当前时间总值
    u32 ulTime_T[2];    //阶段时间总值
    u8 ucDelayMask[2];    //延时标记
    u8 ucSignMask[2];    //执行方向标记 0 = null | 1 = close |  2 = open
    u8 ucVal_C[2];    //目标当前值
    u8 ucVal_T[2];    //目标总值
    u8 ucVal_M[2];    //阶段百分比
    u8 ucVal_S[2];    //阶段启动或关闭阶段

    //需要执行设备的存储库
    ExLDA1_Opt_Typedef_t xOpt[2];

} ExLDA1Cache_Typedef_t;

typedef struct lightcache_t
{
    //临时配置
    ExLDA1Config_Typedef_t xConfig;

    //温度报警信号
    //50% Mode Alarm
    FlagSta_t xAutoDim[2];
    //OFF Mode Alarm
    FlagSta_t xAutoOff[2];
    //温度报警执行标记
    u8 alarmSta[2];
    //HID启动延时
    u8 ucHID_DelayFlag[2];
    u32 ulHID_TimeOut[2];
    u32 ulHID_TimeMax[2];
    //时间判断执行判断
    u8 ucTimerSta[2];    //执行方向标记 0 = null | 1 = close |  2 = open
    //SunRS/SunSet
    u8 delaySta[2];    //延时标记
    u8 val_c[2];    //目标当前值
    u8 val_t[2];    //目标总值
    u8 val_m[2];    //阶段百分比
    u8 val_s[2];    //阶段启动或关闭阶段
    //时间
    u8 timeSta[2];    //延时标记
    u32 time_c[2];    //当前时间总值
    u32 time_t[2];    //阶段时间总值
    //需要执行设备的存储库
    ExLDA1_Opt_Typedef_t opt[2];

} LightCache_Typedef_t;
#endif // __TYPEDEF_LIGHT_H
