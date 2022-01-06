/* Includes ------------------------------------------------------------------*/
#include "uuzADC.h"
#include "typedefDEF.h"
#include "typedefUI.h"
#include "uuzConfigLight.h"
#include "uuzDAC.h"
#include "uuzLIGHT.h"
#include "uuzOpt.h"
#include "uuzSENSOR.h"
#include "uuzTEMP.h"
#include <uuzINIT.h>

#include <uuzUI.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME "ADC "
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

rt_adc_device_t voltage_dev;
rt_adc_device_t temperature_dev;

rt_uint16_t voltage_in_val = 0;

/**
 * @note 温度同步线程
 * @param parameter
 */
void adc_entry(void* parameter)
{
    Temperature_Cache_Typedef_t* xTaCache;
    Temperature_Cache_Typedef_t* xDacCache;

    u8 IsSaveLogsToFlash = 0;
    u8 cmd[8];
    u32 ulCount = 0;
    u32 value = 0;

    adc_init();
    /* 更新温度缓存区 */
    uuz_vTempData_Init();
    uuz_vTempDevice_Init();

    while (1) {
        //清空保存标记
        IsSaveLogsToFlash = 0;
        //第120ms时，采集电源ADC
        if (ulCount == 0) {
            //采集电源的电压值
            xSta[_S_PVD_ADC] = (u16) adc_read(voltage_dev, PVD_VOL_CHANNEL);
            //计算当前的电压值，需要大于12V（120）
            value = xSta[_S_PVD_ADC] * 33 * 11;
            xSta[_S_PVD_VAL] = value / 4095;
        }

        //第250ms时，采集DAC输出的ADC
        if (ulCount == 200) {
            //读取CH-1电压的ADC数据
            xDacCache = &xDacCache[_CHANNEL_1];
            xSta[_S_CH1_DAC_ADC] = uuz_usDacData_Get((u16) adc_read(voltage_dev, CH1_VOL_CHANNEL),
                    &xDacData[_CHANNEL_1]);
            xSta[_S_CH1_DAC_VAL] = dac_to_level(_CHANNEL_1, xSta[_S_CH1_DAC_ADC]);
            xSta[_S_CH1_DAC_STA] = (xSta[_S_CH1_DAC_VAL] >= get_data(_D_LT, _L_MIN_LEVEL)) ? (uuzLTS_ON) : (uuzLTS_OFF);
            LOG_D("DAC1:%d-VAL:%d-STA:%d", xSta[_S_CH1_DAC_ADC], xSta[_S_CH1_DAC_VAL], xSta[_S_CH1_DAC_STA]);

            //读取CH-2电压的ADC数据
            xDacCache = &xDacCache[_CHANNEL_2];
            xSta[_S_CH2_DAC_ADC] = uuz_usDacData_Get((u16) adc_read(voltage_dev, CH2_VOL_CHANNEL),
                    &xDacData[_CHANNEL_2]);
            xSta[_S_CH2_DAC_VAL] = dac_to_level(_CHANNEL_2, xSta[_S_CH2_DAC_ADC]);
            xSta[_S_CH2_DAC_STA] = (xSta[_S_CH2_DAC_VAL] >= get_data(_D_LT, _L_MIN_LEVEL)) ? (uuzLTS_ON) : (uuzLTS_OFF);

#if 0
            //每1秒更新一次灯光状态
            if (xUI.cID = uuzUI_LIGHT) {
                if (xSta[_S_LIGHT_MANUAL] == 0) {
                    rt_light_event();
                }
            } else {
                rt_light_event();
            }
#endif
        }

#if 0
        if (ulCount == 800) {
            LOG_I("Read:xSTA[1]:%d-%d", xSta[_S_CH1_DAC_STA], xSta[_S_CH1_DAC_VAL]);
            LOG_I("Read:xSTA[2]:%d-%d", xSta[_S_CH2_DAC_STA], xSta[_S_CH2_DAC_VAL]);
        }
#endif

        //第500ms时，采集温度对应的ADC
        if (ulCount == 400) {
            //Channel - 1
            xTaCache = &xTemperatureCache[_CHANNEL_1];
            //读取温度1数据
            xSta[_S_CH1_TEMP_VAL] = uuz_vTemperature_Val_Get(xTaCache);
            //如果为华氏度显示
            if (xSysConfig.Light[_L_TEMP_UNITS] == _TYPE_F) {
                //转换温度为华氏度
                xSta[_S_CH1_TEMP_VAL] = uuz_usTemp2Int_ConvCplt(uuz_usTempC2F_ConvCplt(xSta[_S_CH1_TEMP_VAL]));
            }
            //判断是否补偿
            xSta[_S_CH1_TEMP_VAL] = uuz_vCompensation_Get(xSta[_S_CH1_TEMP_VAL], _CHANNEL_1);
            //获取温度的最大值和最小值
            IsSaveLogsToFlash += uuz_usTempValue_LimitGet(xSta[_S_CH1_TEMP_VAL], _CHANNEL_1);
            //设置LOG相关数据:CH1
            if (IsSaveLogsToFlash == 1) {
                //写入相关数据
                vU16ToU8(cmd, xSta[_S_CH1_TEMP_L], uuzMSB);
                vU16ToU8(cmd + 2, xSta[_S_CH1_TEMP_H], uuzMSB);
                set_logs(_G_CH1_TA_INFO, cmd, 4);
            }
            //Channel - 2
            xTaCache = &xTemperatureCache[_CHANNEL_2];
            //读取温度1数据
            uuz_vTemperature_Get(xTaCache);
            //如果为华氏度显示
            if (xSysConfig.Light[_L_TEMP_UNITS] == _TYPE_F) {
                //转换温度为华氏度
                xSta[_S_CH2_TEMP_VAL] = uuz_usTemp2Int_ConvCplt(uuz_usTempC2F_ConvCplt(xSta[_S_CH2_TEMP_VAL]));
            }
            //判断是否补偿
            xSta[_S_CH2_TEMP_VAL] = uuz_vCompensation_Get(xSta[_S_CH2_TEMP_VAL], _CHANNEL_2);
            //获取温度的最大值和最小值
            IsSaveLogsToFlash += uuz_usTempValue_LimitGet(xSta[_S_CH2_TEMP_VAL], _CHANNEL_2);
            //设置LOG相关数据:CH2
            if ((IsSaveLogsToFlash == 2) || (IsSaveLogsToFlash == 3)) {
                //写入相关数据
                vU16ToU8(cmd, xSta[_S_CH2_TEMP_L], uuzMSB);
                vU16ToU8(cmd + 2, xSta[_S_CH2_TEMP_H], uuzMSB);
                set_logs(_G_CH2_TA_INFO, cmd, 4);
            }
            voltage_in_val = adc_read(voltage_dev, VOL_IN_CHANNEL);
        }

        //有相关标记，保存数据
        if (IsSaveLogsToFlash) {
            save_logs();
            IsSaveLogsToFlash = 0;
        }
        ulCount++;

        //1000ms后重新循环
        if (ulCount >= 1000) {
            ulCount = 1;
        }
        rt_thread_mdelay(1);
    }
}

/**
 * @note 启动ADC读取同步线程
 */
void adc_sync_init(void)
{
    /* 创建 adc 线程 */
    rt_thread_t thread = rt_thread_create("adc chk", adc_entry,
    RT_NULL, 2048, 24, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
    }
}

rt_uint32_t adc_read(rt_adc_device_t dev, rt_uint32_t channel)
{
    rt_uint32_t value = 0;
    rt_err_t ret = RT_EOK;
    /* 使能设备 */
    ret = rt_adc_enable(dev, channel);
    /* 读取采样值 */
    value = rt_adc_read(dev, channel);
#if 0
    if (channel == 8 || channel == 9 || channel == 17) {
        LOG_D("the value[%d] is :%d", channel, value);
    }
#endif
    /* 转换为对应电压值 */
    //vol = value * REFER_VOLTAGE / CONVERT_BITS;
    //LOG_D("the voltage is :%d.%02d \n", vol / 100, vol % 100);
    /* 关闭通道 */
    ret = rt_adc_disable(dev, channel);

    if (ret == RT_EOK) {
        return value;
    } else {
        return (4095U);
    }
}

void adc_init(void)
{
    /* 查找设备 */
    voltage_dev = (rt_adc_device_t) rt_device_find(VOLTAGE_DEV_NAME);
    if (voltage_dev == RT_NULL) {
        LOG_D("adc sample run failed! can't find %s device!", VOLTAGE_DEV_NAME);
        return;
    }

    /* 查找设备 */
    temperature_dev = (rt_adc_device_t) rt_device_find(TEMPERATURE_DEV_NAME);
    if (temperature_dev == RT_NULL) {
        LOG_D("adc sample run failed! can't find %s device!", TEMPERATURE_DEV_NAME);
        return;
    }
}
