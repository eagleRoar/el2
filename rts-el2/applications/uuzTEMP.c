/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include <uuzSENSOR.h>
#include "uuzTEMP.h"
#include "math.h"
#include "uuzADC.h"
#include "uuzSENSOR.h"
#include "uuzGPIO.h"
#include "typedefUI.h"

#define DBG_ENABLE
#define DBG_SECTION_NAME "TEMP"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

static u16 NTCTAB[241] =
        { 0x0E7A, 0x0E70, 0x0E65, 0x0E5B, 0x0E50, 0x0E45, 0x0E3A, 0x0E2E, 0x0E23, 0x0E17, 0x0E0B, 0x0DFF, 0x0DF2,
                0x0DE5,
                0x0DD8, 0x0DCB, 0x0DBE, 0x0DB0, 0x0DA3, 0x0D95, 0x0D86, 0x0D78, 0x0D69, 0x0D5A, 0x0D4B, 0x0D3C, 0x0D2C,
                0x0D1C, 0x0D0C, 0x0CFC, 0x0CEB, 0x0CDB, 0x0CCA, 0x0CB9, 0x0CA7, 0x0C96, 0x0C84, 0x0C72, 0x0C60, 0x0C4E,
                0x0C3B, 0x0C28, 0x0C15, 0x0C02, 0x0BEF, 0x0BDB, 0x0BC8, 0x0BB4, 0x0BA0, 0x0B8C, 0x0B77, 0x0B63, 0x0B4E,
                0x0B39, 0x0B24, 0x0B0F, 0x0AFA, 0x0AE5, 0x0ACF, 0x0ABA, 0x0AA4, 0x0A8E, 0x0A78, 0x0A62, 0x0A4C, 0x0A36,
                0x0A1F, 0x0A09, 0x09F2, 0x09DC, 0x09C5, 0x09AF, 0x0998, 0x0981, 0x096B, 0x0954, 0x093D, 0x0926, 0x0910,
                0x08F9, 0x08E2, 0x08CB, 0x08B4, 0x089D, 0x0887, 0x0870, 0x0859, 0x0843, 0x082C, 0x0815, 0x07FF, 0x07E9,
                0x07D2, 0x07BC, 0x07A6, 0x0790, 0x077A, 0x0764, 0x074E, 0x0738, 0x0722, 0x070D, 0x06F7, 0x06E2, 0x06CD,
                0x06B8, 0x06A3, 0x068E, 0x0679, 0x0665, 0x0650, 0x063C, 0x0628, 0x0614, 0x0600, 0x05ED, 0x05D9, 0x05C6,
                0x05B3, 0x05A0, 0x058D, 0x057A, 0x0568, 0x0555, 0x0543, 0x0531, 0x051F, 0x050E, 0x04FC, 0x04EB, 0x04DA,
                0x04C9, 0x04B8, 0x04A8, 0x0497, 0x0487, 0x0477, 0x0467, 0x0457, 0x0448, 0x0439, 0x0429, 0x041A, 0x040C,
                0x03FD, 0x03EF, 0x03E0, 0x03D2, 0x03C4, 0x03B7, 0x03A9, 0x039C, 0x038E, 0x0381, 0x0374, 0x0368, 0x035B,
                0x034F, 0x0343, 0x0337, 0x032B, 0x031F, 0x0313, 0x0308, 0x02FD, 0x02F1, 0x02E7, 0x02DC, 0x02D1, 0x02C7,
                0x02BC, 0x02B2, 0x02A8, 0x029E, 0x0294, 0x028B, 0x0281, 0x0278, 0x026F, 0x0265, 0x025C, 0x0254, 0x024B,
                0x0242, 0x023A, 0x0232, 0x0229, 0x0221, 0x0219, 0x0212, 0x020A, 0x0202, 0x01FB, 0x01F3, 0x01EC, 0x01E5,
                0x01DE, 0x01D7, 0x01D0, 0x01C9, 0x01C3, 0x01BC, 0x02E7, 0x01AF, 0x01A9, 0x01A3, 0x019D, 0x0197, 0x0191,
                0x018B, 0x0186, 0x0180, 0x017A, 0x0175, 0x0170, 0x016A, 0x0165, 0x0160, 0x015B, 0x0156, 0x0151, 0x014D,
                0x0148, 0x0143, 0x013F, 0x013A, 0x0136, 0x0131, 0x012D, 0x0129, 0x0125, 0x0120, 0x011C, 0x0118, 0x0114,
                0x0111, 0x010D, 0x0109, 0x0105, 0x0102, 0x00FE, };
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
u16 uuz_usTempData_Get(u16 voltage, Sensor_Typedef_t * sensor)
{
    u16 tempVal = voltage;

    if (voltage > 0) {
        uuz_vSensorData_Anglysis(voltage, sensor);
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
u16 uuz_usTempUnit_ConvCplt(u16 usT)
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
void uuz_vTemperature_Get(Temperature_Cache_Typedef_t * xTemp)
{
    u8 ch = xTemp->channel - CH1_TA_CHANNEL;
    //读取温度传感器的状态,高电平为有效
    xSta[_S_CH1_TEMP_EN + _CH_DIS(ch)] = (rt_pin_read(xTemp->pin) == PIN_HIGH) ? (1U) : (0U);
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
