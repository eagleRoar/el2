/* Includes ------------------------------------------------------------------*/
#include "uuzTEMP.h"
#include "math.h"
#include "typedefUI.h"
#include "uuzADC.h"
#include "uuzLED.h"
#include "uuzGPIO.h"
#include "uuzSENSOR.h"
#include <rtthread.h>
#include <uuzSENSOR.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME "TEMP"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

//3435
static u16 NTCTAB[241] =
            {
                    0xE2B,
                    0xE1F,
                    0xE14,
                    0xE08,
                    0xDFD,
                    0xDF1,
                    0xDE5,
                    0xDD8,
                    0xDCC,
                    0xDBF,
                    0xDB2,
                    0xDA5,
                    0xD98,
                    0xD8A,
                    0xD7D,
                    0xD6F,
                    0xD61,
                    0xD53,
                    0xD44,
                    0xD36,
                    0xD27,
                    0xD18,
                    0xD09,
                    0xCFA,
                    0xCEA,
                    0xCDB,
                    0xCCB,
                    0xCBB,
                    0xCAB,
                    0xC9A,
                    0xC8A,
                    0xC79,
                    0xC68,
                    0xC57,
                    0xC46,
                    0xC35,
                    0xC24,
                    0xC12,
                    0xC00,
                    0xBEE,
                    0xBDC,
                    0xBCA,
                    0xBB8,
                    0xBA6,
                    0xB93,
                    0xB81,
                    0xB6E,
                    0xB5B,
                    0xB48,
                    0xB35,
                    0xB22,
                    0xB0E,
                    0xAFB,
                    0xAE8,
                    0xAD4,
                    0xAC0,
                    0xAAD,
                    0xA99,
                    0xA85,
                    0xA71,
                    0xA5D,
                    0xA49,
                    0xA35,
                    0xA21,
                    0xA0D,
                    0x9F8,
                    0x9E4,
                    0x9D0,
                    0x9BC,
                    0x9A7,
                    0x993,
                    0x97F,
                    0x96A,
                    0x956,
                    0x941,
                    0x92D,
                    0x919,
                    0x904,
                    0x8F0,
                    0x8DC,
                    0x8C8,
                    0x8B3,
                    0x89F,
                    0x88B,
                    0x877,
                    0x863,
                    0x84F,
                    0x83B,
                    0x827,
                    0x813,
                    0x7FF,
                    0x7EB,
                    0x7D8,
                    0x7C4,
                    0x7B0,
                    0x79D,
                    0x78A,
                    0x776,
                    0x763,
                    0x750,
                    0x73D,
                    0x72A,
                    0x717,
                    0x705,
                    0x6F2,
                    0x6DF,
                    0x6CD,
                    0x6BB,
                    0x6A9,
                    0x697,
                    0x685,
                    0x673,
                    0x661,
                    0x650,
                    0x63E,
                    0x62D,
                    0x61C,
                    0x60A,
                    0x5F9,
                    0x5E9,
                    0x5D8,
                    0x5C7,
                    0x5B7,
                    0x5A7,
                    0x596,
                    0x586,
                    0x577,
                    0x567,
                    0x557,
                    0x548,
                    0x538,
                    0x529,
                    0x51A,
                    0x50B,
                    0x4FC,
                    0x4EE,
                    0x4DF,
                    0x4D1,
                    0x4C3,
                    0x4B4,
                    0x4A6,
                    0x499,
                    0x48B,
                    0x47D,
                    0x470,
                    0x463,
                    0x456,
                    0x449,
                    0x43C,
                    0x42F,
                    0x422,
                    0x416,
                    0x40A,
                    0x3FD,
                    0x3F1,
                    0x3E6,
                    0x3DA,
                    0x3CE,
                    0x3C3,
                    0x3B7,
                    0x3AC,
                    0x3A1,
                    0x396,
                    0x38B,
                    0x380,
                    0x375,
                    0x36B,
                    0x360,
                    0x356,
                    0x34C,
                    0x342,
                    0x338,
                    0x32E,
                    0x325,
                    0x31B,
                    0x312,
                    0x308,
                    0x2FF,
                    0x2F6,
                    0x2ED,
                    0x2E4,
                    0x2DB,
                    0x2D2,
                    0x2CA,
                    0x2C1,
                    0x2B9,
                    0x2B1,
                    0x2A9,
                    0x2A1,
                    0x299,
                    0x291,
                    0x289,
                    0x281,
                    0x27A,
                    0x272,
                    0x26B,
                    0x264,
                    0x25C,
                    0x255,
                    0x24E,
                    0x247,
                    0x240,
                    0x233,
                    0x233,
                    0x22C,
                    0x226,
                    0x21F,
                    0x219,
                    0x213,
                    0x20D,
                    0x207,
                    0x200,
                    0x1FB,
                    0x1F5,
                    0x1EF,
                    0x1E9,
                    0x1E3,
                    0x1DE,
                    0x1D8,
                    0x1D3,
                    0x1CE,
                    0x1C8,
                    0x1C3,
                    0x1BE,
                    0x1B9,
                    0x1B4,
                    0x1AF,
                    0x1AA,
                    0x1A5,
                    0x1A0,
                    0x19B,
                    0x197,
                    0x192,
                    0x18E,
                    0x189,
                    0x185,
                    0x180,
                    0x17C,
                    0x178,
                    0x174,
                    0x16F, };
Sensor_Typedef_t xTempData[2];    //CH1|CH2
Temperature_Cache_Typedef_t xTemperatureCache[2];    //CH1|CH2
/* ----------------------------------------------------------------*/
//查表函数
u8 look_up_table(u16 data)
{
    u16 begin, end, middle;
    u8 index;

    begin = 0;
    end = 240;
    index = 0;

    if (data >= NTCTAB[begin]) {
        return begin;
    } else if (data <= NTCTAB[end]) {
        return end;
    }

    while (begin < end) {
        middle = (begin + end) / 2;
        if (data == NTCTAB[middle]) {
            break;
        }
        if (data < NTCTAB[middle] && data > NTCTAB[middle + 1]) {
            break;
        }
        if (data > NTCTAB[middle]) {
            end = middle;
        } else {
            begin = middle;
        }
        if (index++ > 240) {
            break;
        }
    }

    if (begin > end) {
        return 0;
    }
    return middle;
}
/* ----------------------------------------------------------------*/
/**
 * @brief 获取相关集合温度
 */
void uuz_vTempData_Init(void)
{
    rt_memset(&xTempData[0], 0x00, sizeof(Sensor_Typedef_t) * 2);
    rt_memset(&xTemperatureCache[0], 0x00, (sizeof(xTemperatureCache) * 2));
}

/**
 * @brief 设置温度采集设备参数
 */
void uuz_vTempDevice_Init(void)
{
    //通道1-温度探头1
    xTemperatureCache[0].pin = CH1_TA_EN;
    xTemperatureCache[0].dev = temperature_dev;
    xTemperatureCache[0].channel = CH1_TA_CHANNEL;
    //通道2-温度探头2
    xTemperatureCache[1].pin = CH2_TA_EN;
    xTemperatureCache[1].dev = temperature_dev;
    xTemperatureCache[1].channel = CH2_TA_CHANNEL;

#if 0
    u16 _NTCTAB[241];
    for (u8 ucIndex = 0; ucIndex < 241; ucIndex++) {
        _NTCTAB[ucIndex] = NTCTAB[ucIndex];
        if ((ucIndex % 10) == 0) {
            rt_kprintf("\r\n");
        }
        rt_kprintf("0x%04X,", _NTCTAB[ucIndex]);
    }
#endif
}

/**
 * @brief 获取相关集合温度
 * @param voltage   //当前值AD
 * @param sensor    //临时缓存
 * @return
 */
u16 uuz_usTempData_Get(u16 voltage, Sensor_Typedef_t* sensor)
{
    u16 tempVal = voltage;

    if (voltage > 0) {
        uuz_vSensorData_Anglysis(voltage, sensor, 20);
        tempVal = (sensor->middle != 0) ? (sensor->middle) : (sensor->analysis);
    }

    return tempVal;
}

/**
 *
 * @brief 将AD值转换成温度值(C)
 * @param voltage
 * @return
 */
u16 uuz_usTempData_Convert(u16 voltage)
{
    u32 ulTmpT = 0;
    u32 a, b;
    u8 num;

    //设置值大于0
    if ((voltage <= NTCTAB[40]) && (voltage > NTCTAB[240])) {
        num = look_up_table(voltage);
        //LOG_I("voltage:%d, num:%d, num+1:%d", voltage, NTCTAB[num], NTCTAB[num + 1]);
        a = ((voltage - NTCTAB[num]) >= 0) ? (voltage - NTCTAB[num]) : (~(voltage - NTCTAB[num]));
        b = ((NTCTAB[num + 1] - NTCTAB[num]) >= 0) ?
                                                     (NTCTAB[num + 1] - NTCTAB[num]) :
                                                     (~(NTCTAB[num + 1] - NTCTAB[num]));
        ulTmpT = 50 * a / b;
        if (ulTmpT % 10 > 5) {
            ulTmpT = ulTmpT / 10 + 1;
        } else {
            ulTmpT = ulTmpT / 10;
        }
        ulTmpT = ulTmpT + (num - 40) * 5;
    } else {
        ulTmpT = 0;
    }
    return (u16) ulTmpT;
}

/**
 * @brief 将摄氏度转换成华氏度
 * @param usT
 * @return
 */
u16 uuz_usTempC2F_ConvCplt(u16 usT)
{
    u32 ulTmpT = usT;
    /* 摄氏度->华氏度转换 */
    ulTmpT = (ulTmpT * 9 / 5) + 320;
    return (u16) ulTmpT;
}

/**
 * @brief 将华氏度转换成摄氏度
 * @param usT
 * @return
 */
u16 uuz_usTempF2C_ConvCplt(u16 usT)
{
    u32 ulTmpT = usT;
    if (ulTmpT < 320) {
        //如果是0下温度
        ulTmpT = 0;
    } else {
        /* 华氏度->摄氏度转换 */
        ulTmpT = (ulTmpT - 320) * 5 / 9;
    }
    return (u16) ulTmpT;
}

/**
 * @brief 配置数据精度为0.1，数据求整显示
 * @param usT
 * @return
 */
u16 uuz_usTemp2Int_ConvCplt(u16 usT)
{
    u32 ulTmpT = usT;

    /* 华氏度->摄氏度转换 */
    if ((ulTmpT % 10) >= 5) {
        ulTmpT = ulTmpT / 10 + 1;
    } else {
        ulTmpT = ulTmpT / 10;
    }
    ulTmpT *= 10;
    return (u16) ulTmpT;
}

/**
 *
 * @brief 获取实时温度的最大值，最小值,并返回保存标记
 * @param usT
 * @param ch
 * @return
 */
u8 uuz_usTempValue_LimitGet(u16 usT, u8 ch)
{
    u8 saveToFlash = 0;
    u16 min = 0;

    if (xSysConfig.Light[_L_TEMP_UNITS] == _TYPE_F) {
        min = 320;    //32F = 0C
    }

    //有效数据
    if (usT > min) {
        //获取最高值
        if (xSta[_S_CH1_TEMP_H + _CH_DIS(ch)] == _TEMP_NULL) {
            xSta[_S_CH1_TEMP_H + _CH_DIS(ch)] = usT;
            saveToFlash = (1 + ch);
        } else if (xSta[_S_CH1_TEMP_H + _CH_DIS(ch)] < usT) {
            xSta[_S_CH1_TEMP_H + _CH_DIS(ch)] = usT;
            saveToFlash = (1 + ch);
        }
        //获取最低值
        if (xSta[_S_CH1_TEMP_L + _CH_DIS(ch)] == _TEMP_NULL) {
            xSta[_S_CH1_TEMP_L + _CH_DIS(ch)] = usT;
            saveToFlash = (1 + ch);
        } else if (xSta[_S_CH1_TEMP_L + _CH_DIS(ch)] > usT) {
            xSta[_S_CH1_TEMP_L + _CH_DIS(ch)] = usT;
            saveToFlash = (1 + ch);
        }
    } else {
        xSta[_S_CH1_TEMP_H + _CH_DIS(ch)] = _TEMP_NULL;
        xSta[_S_CH1_TEMP_L + _CH_DIS(ch)] = _TEMP_NULL;
    }

    return saveToFlash;
}

/**
 *
 * @brief 检测到有效数据,并转换
 * @param xTemp
 */
void uuz_vTemperature_Get(Temperature_Cache_Typedef_t* xTemp)
{
    u8 ch = xTemp->channel - CH1_TA_CHANNEL;
    //读取温度传感器的状态,高电平为有效
    xSta[_S_CH1_TEMP_EN + _CH_DIS(ch)] = (rt_pin_read(xTemp->pin) == PIN_LOW) ? (1U) : (0U);
    if (xSta[_S_CH1_TEMP_EN + _CH_DIS(ch)] == 1) {
        //读取温度通道的相关数据,对数据进行除抖处理
        xSta[_S_CH1_TEMP_ADC + _CH_DIS(ch)] = uuz_usTempData_Get((u16) adc_read(xTemp->dev, xTemp->channel),
                &xTempData[ch]);
        //将数据转换成温度值(AD->C)
        xSta[_S_CH1_TEMP_VAL + _CH_DIS(ch)] = uuz_usTempData_Convert(xSta[_S_CH1_TEMP_ADC + _CH_DIS(ch)]);
    } else {
        //TODO:温度断线，需要刷新特定界面
    }
}

/**
 * @brief 检测到有效数据，并转换返回数据
 * @param xTemp
 * @return 返回相关摄氏度温度
 */
rt_uint16_t uuz_vTemperature_Val_Get(Temperature_Cache_Typedef_t* xTemp)
{
    u8 ch = xTemp->channel - CH1_TA_CHANNEL;
    rt_uint16_t value = 0;

    //读取温度传感器的状态,高电平为有效
    xSta[_S_CH1_TEMP_EN + _CH_DIS(ch)] = (rt_pin_read(xTemp->pin) == PIN_LOW) ? (1U) : (0U);
    if (xSta[_S_CH1_TEMP_EN + _CH_DIS(ch)] == 1) {
        //读取温度通道的相关数据,对数据进行除抖处理
        xSta[_S_CH1_TEMP_ADC + _CH_DIS(ch)] = uuz_usTempData_Get((u16) adc_read(xTemp->dev, xTemp->channel),
                &xTempData[ch]);
        //将数据转换成温度值(AD->C)
        value = uuz_usTempData_Convert(xSta[_S_CH1_TEMP_ADC + _CH_DIS(ch)]);
    } else {
        //TODO:温度断线，需要刷新特定界面
    }

    return value;
}

/**
 * @brief 显示补偿数据
 * @param usTemperature
 * @param ch
 * @return 返回值
 */
u16 uuz_vCompensation_Get(u16 usTemperature, u8 ch)
{
    u16 usValue = usTemperature + 127;
    //灯光补偿信息
    if ((ch == _CHANNEL_1) || (ch == _CHANNEL_2)) {
        usValue = usTemperature + xSysConfig.Light[_L_CAL_VAL1 + ch];
    }
    //返回补偿值
    return (usValue - 127);
}
