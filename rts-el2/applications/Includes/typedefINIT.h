/*
 * @Copyright (c) 2006-2018 RT-Thread Development Team:  
 * @SPDX-License-Identifier: Apache-2.0:  
 * @Change Logs: 
 * @Date: 2020-02-21 09:25:27
 * @LastEditors: Zhou Xiaomin
 * @LastEditTime: 2020-02-25 19:14:47
 * @Description:  
 */
#ifndef __TYPEDEF_INIT_H
#define __TYPEDEF_INIT_H

#include "typedefDEF.h"

typedef enum
{
    _D_INIT_STATE = 0x0000U,        //[0]配置初始化状态(0:未初始|1：已初始化)
    _D_BOOT_COUNT,                  //[1]启动次数
    _D_SW_VERSION,                  //[2]设备软件版本号(10000 = Version:1.00.00)
    _D_HW_VERSION,                  //[3]设备硬件版本号(495 = V4.95)
    _D_DEVICE_TYPE,                 //[4]设备类型 :0x00 Gavita Master Controller EL1
                                    //          :0x01 Gavita Master Controller EL2
    _D_PLC_STATION,                 //[5]设备的主机编号:0xF7
    _D_DATE_FORMAT,                 //[6]系统日期设置(0:MM/DD/YY;1:DD/MM/YY)
    _D_TIME_FORMAT,                 //[7]系统时制(0:24-HOUR;1:12-HOUR)
    _D_LANGUAGE,                    //[8]语言(0:English)
    _D_IS_CONNECTED,                 //[9]从机连接状态:0x00未连接；0x01:已连接
    _D_SLAVE_STATION,                  //[10]从机设备Modbus ID:0x01
    _D_SLAVE_TYPE,                  //[11]从机设备类型:0x01
    __SYSTEM_MAX
} ENV_SYSTEM_T;

typedef enum
{
    _L_TEMP_UNITS,                       //[0]温度单位(0:C;1:F)
    //-------------->Channel1
    _L_CH1_OUTPUT_MODE,                  //[1]主线灯光模式(0:OFF; 1:ON; 2:AUTO)
    _L_CH1_CYCLE_ON,                     //[2]主线开启时间(08:00)
    _L_CH1_CYCLE_OFF,                    //[3]主线关闭时间(22:00)
    _L_CH1_OUTPUT_LEVEL,                 //[4]主线灯光输出强度(10%-115%)
    _L_CH1_DISPLAY_MODE,                 //[5]显示总瓦数方式(0:%;1:W)
    _L_CH1_SUNRISE_DELAY,                    //[6]日升时间长度(NONE-00|Max:30mins)
    _L_CH1_SUNSET_DELAY,                     //[7]日落时间长度(NONE-00|Max:30mins)
    _L_CH1_ECM_MODE,                     //[8]显示灯光关联的工作方式ECM(0:Day; 1:Night)
                                         //Day:Activate the ECM during lights-on;
                                         //Night:Activate the ECM during lights-off;
    _L_CH1_NIGHT_ALARM_TEMP,                 //[9]夜晚报警温度(27.5C)
    _L_CH1_DAY_ALARM_TEMP,                   //[10]白天报警温度(27.5C)
    _L_CH1_AUTO_DIM_TEMP,                    //[11]自动调节温度(27.5C)
    _L_CH1_SHUTDOWN_TEMP,                    //[12]关闭报警温度(27.5C)
    _L_CH1_TEMP_HIGH,                        //[13]当前获取的最高温度(35.0C)
    _L_CH1_TEMP_LOW,                         //[14]当前获取的最高温度(35.0C)
    //-------------->Channel2
    _L_CH2_OUTPUT_MODE,                  //[15]副线灯光模式(1:独立 independent; 2:互斥 inverse; 0:跟随 follow main)
    _L_CH2_CYCLE_ON,                     //[16]主线开启时间(08:00)
    _L_CH2_CYCLE_OFF,                    //[17]主线关闭时间(22:00)
    _L_CH2_OUTPUT_LEVEL,                 //[18]副线灯光输出强度(10%-115%)
    _L_CH2_DISPLAY_MODE,                 //[19]显示总瓦数方式(0:%;1:W)
    _L_CH2_SUNRISE_DELAY,                    //[20]日升时间长度(NONE-00|Max:30mins)
    _L_CH2_SUNSET_DELAY,                     //[21]日落时间长度(NONE-00|Max:30mins)
    _L_CH2_ECM_MODE,                     //[22]显示灯光关联的工作方式ECM(0:Day; 1:Night)
                                         //Day:Activate the ECM during lights-on;
                                         //Night:Activate the ECM during lights-off;
    _L_CH2_NIGHT_ALARM_TEMP,                 //[23]夜晚报警温度(27.5C)
    _L_CH2_DAY_ALARM_TEMP,                   //[24]白天报警温度(27.5C)
    _L_CH2_AUTO_DIM_TEMP,                    //[25]自动调节温度(27.5C)
    _L_CH2_SHUTDOWN_TEMP,                    //[26]关闭报警温度(27.5C)
    _L_CH2_TEMP_HIGH,                        //[27]当前获取的最高温度(35.0C)
    _L_CH2_TEMP_LOW,                         //[28]当前获取的最高温度(35.0C)
    //--------------->温度传感器
    _L_CAL_VAL1,                         //[29]校正温度传感器1-校正值(127)--小于表示负值,大于表示正值(-12.7~~12.7C)
    _L_CAL_VAL2,                         //[30]校正温度传感器2-校正值(127)--小于表示负值,大于表示正值(-12.7~~12.7C)
    //频道2的工作模式
    _L_CH2_MODE,                         //[31]频道工作模式(FOLLOW MODE | INVERSE | INDEPENDENT)
    __LIGHT_MAX
} ENV_LIGHT_T;

typedef enum
{
    _G_CH1_AUTO_DIM = 0x0000U,        //[0]CH-1自动调光记录
    _G_CH2_AUTO_DIM,        //[1]CH-2自动调光记录
    _G_CH1_SHUTDOWN,        //[2]CH-1自动关闭记录
    _G_CH2_SHUTDOWN,        //[3]CH-2自动关闭记录
    _G_POWER_DOWN,          //[4]异常关闭记录
    _G_CH1_TA_INFO,         //[5]CH-1温度高记录
    _G_CH2_TA_INFO,         //[6]CH-2温度高记录
    __LOG_MAX
} ENV_LOG_T;

typedef enum
{
    //设备默认ID
    _S_DEVICE_ID1 = 0x0000U,        //[0]设备ID-0-1
    _S_DEVICE_ID2,        //[1]设备ID-2-3
    //状态标记
    _S_FLAG_STARTUP,        //[2]启动标记
    _S_FLAG_PVD,        //[3]中断标记
    _S_FLAG_ERR,        //[4]传感器中断标记
    //当前灯光状态显示
    _S_MODE_CH2,        //[5]CH-2和CH-1关联模式
    //按键长按状态
    _S_KEY_LONG,        //长按为1，放开为0
    //灯光的状态属性
    //断电时间标记
    _S_PVD_YEAR,        //断电保存年份
    _S_PVD_MON,         //断电保存月份
    _S_PVD_DAY,         //断电保存天数
    _S_PVD_HOUR,        //断电保存小时
    _S_PVD_MIN,         //断电保存分钟
    //当前电源电压缓存
    _S_PVD_ADC,        //当前电源电压ADC缓存
    _S_PVD_VAL,        //当前电源电压缓存
    //Channel-1相关数据
    _S_CH1_DAC_ADC,        //ADC读取的CH-1的DAC的值
    _S_CH1_DAC_OUT,        //CH-1的DAC的输出值
    _S_CH1_DAC_OPT,        //CH-1的DAC的输出状态
    _S_CH1_TEMP_EN,        //读取的CH-1的温度使能
    _S_CH1_TEMP_ADC,        //ADC读取的CH-1的采集值
    _S_CH1_TEMP_VAL,        //读取的CH-1的温度值
    _S_CH1_TEMP_H,        //读取的CH-1的高温度值
    _S_CH1_TEMP_L,        //读取的CH-1的低温度值
    _S_LED_CH1,         //线路1的灯光状态
    _S_LED_TA1,         //线路1的灯光状态
    //Channel-2相关数据
    _S_CH2_DAC_ADC,        //ADC读取的CH-2的DAC的值
    _S_CH2_DAC_OUT,        //CH-2的DAC的输出值
    _S_CH2_DAC_OPT,        //CH-2的DAC的输出状态
    _S_CH2_TEMP_EN,        //读取的CH-2的温度使能
    _S_CH2_TEMP_ADC,        //ADC读取的CH-2的采集值
    _S_CH2_TEMP_VAL,        //读取的CH-2的温度值
    _S_CH2_TEMP_H,        //读取的CH-2的高温度值
    _S_CH2_TEMP_L,        //读取的CH-2的低温度值
    _S_LED_CH2,         //线路2的灯光状态
    _S_LED_TA2,         //线路2的灯光状态
    //状态值数量
    __STA_MAX           //状态值数量
} ENV_STA_T;

#define _CH_LVL(CH) ((_L_CH2_OUTPUT_MODE - _L_CH1_OUTPUT_MODE) * CH)
#define _CH_DIS(CH) ((_S_CH2_DAC_ADC - _S_CH1_DAC_ADC) * CH)
/*
 * define
 * 1:EL-2:GAVITA-EL2
 */
#define uuzCONFIG_DEVICE_TYPE (0x01U)   //表示配置数据
#define SYS_CONFIG_ADDRESS        ((rt_uint32_t)0x0801F000)
//Address(1F400 -- 1F800 -- 1FC00 -- 20000
#define SYS_LOGS_ADDRESS          ((rt_uint32_t)0x0801F800)

#define _D_SYS   (0U)             //表示数据类型为系统数据(读写)
#define _D_LT    (1U)             //表示数据类型为灯光数据(读写)
#define _D_LOG   (2U)             //表示数据类型为日志数据(只读)
#define _D_STA   (3U)             //表示数据类型为状态数据(只读)
//时间显示格式
#define _TYPE_24HOUR   (24U)             //表示数据类型为24-HOUR
#define _TYPE_12HOUR   (12U)             //表示数据类型为12-HOUR
//日期显示格式
#define _TYPE_MMDDYY   (0U)             //表示数据类型为MM-DD-YY
#define _TYPE_DDMMYY   (1U)             //表示数据类型为DD-MM-YY
//温度单位显示格式
#define _TYPE_C   (0U)                  //表示温度单位是摄氏度C
#define _TYPE_F   (1U)                  //表示温度单位是华氏度F
//温度的无效值
#define _TEMP_NULL   (65535U)             //表示该位置没有数据(0xFFFF)
//设备的型号标记
#define _EL1   (0U)                  //表示当前型号是EL-1
#define _EL2   (1U)                  //表示当前型号是EL-2

typedef struct sysconfig_t
{
    //解释同上
    u16 Sys[__SYSTEM_MAX ];                 //系统配置
    u16 Light[__LIGHT_MAX ];                //灯光配置
    u16 end;                //结尾符号

} SysConfig_Typedef_t;
#define uuzCONFIG_LEN (sizeof(SysConfig_Typedef_t))

//日志记录
typedef struct logs_t
{
    //解释同上
    u16 en;                                                //数据有效性
    u16 date[3];                                        //日期:YY-MM-DD
    u16 time[3];                                        //时间:HH:MM:SS
    u8 data[8];                                          //数据大小8

} Logs_Typedef_t;
#define uuzLOGS_LEN (sizeof(Logs_Typedef_t))

typedef struct temp_cal_cache_t
{
    u16 cal;                //需要的校正值
    u16 adv;                //对应的AD偏移值

} TempCal_Cache_t;

typedef struct devicecache_t
{
    //当前设备操作频道状态显示
    u8 channel;                                 //_CHANNLE_1/_CHANNEL_2
    //当前灯光状态显示
    u16 watt;                                   //当前的最大瓦数
    u16 watt_mode;                              //当前的瓦数模式
    u16 chMode[2];                              //当前线路灯光状态
    //灯光数据调节
    u16 level;                                  //界面当前操作的亮度值
    u16 levelOpt[2];                            //当前操作瓦数
    u16 timeCH[2];                              //界面当前操作的开启/关闭时间
    TempCal_Cache_t cal[2];                     //温度校正界面
    //灯光手动调节界面
    //uuzUI_LIHGT
    u16 level_L[2];                             //界面当前操作的亮度值
    u16 level_W[2];                             //界面当前操作的瓦数
    u16 level_M[2];                             //界面灯光的工作模式
    u16 level_D[2];                             //界面灯光的瓦数显示模式
    //Sunise---SunSet
    u16 usRamp[2];                              //日升日落数据
    //CAL Val
    u8 ucCal_Val[2];                            //频道1和频道2的校正误差值

} DeviceCache_Typedef_t;

#endif // __TYPEDEF_INIT_H
