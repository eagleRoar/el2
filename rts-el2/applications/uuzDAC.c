/* Includes ------------------------------------------------------------------*/
#include "uuzDAC.h"
#include "typedefDEF.h"
#include "typedefUI.h"
#include "uuzLED.h"
#include "uuzSENSOR.h"
#include <uuzINIT.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME "DAC "
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

DAC_HandleTypeDef hdac;
Sensor_Typedef_t xDacData[2]; //CH1|CH2

void MX_DAC_Init(void)
{
    DAC_ChannelConfTypeDef sConfig = { 0 };

    /** DAC Initialization
     */
    hdac.Instance = DAC;
    if (HAL_DAC_Init(&hdac) != HAL_OK) {
        Error_Handler();
    }
    /** DAC channel OUT1 config
     */
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
    if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    /** DAC channel OUT2 config
     */
    if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief 获取相关集合DAC
 */
void uuz_vDacData_Init(void)
{
    rt_memset(&xDacData[0], 0x00, sizeof(Sensor_Typedef_t) * 2);
}

/**
 * @brief 获取相关集合DAC
 * @param voltage   //当前值AD
 * @param sensor    //临时缓存
 * @return
 */
u16 uuz_usDacData_Get(u16 voltage, Sensor_Typedef_t* sensor)
{
    u16 dacVal = voltage;

    if (voltage > 0) {
        uuz_vSensorData_Anglysis(voltage, sensor, 10);
        dacVal = (sensor->middle != 0) ? (sensor->middle) : (sensor->analysis);
    }

    return dacVal;
}

/**
 * @brief DAC转换成亮度等级
 * @param ch
 * @param adc
 * @return
 */
u16 dac_to_level(u16 ch, u16 adc)
{
    u32 value = adc;
    value *= 33;
    value *= 490;
    value /= 4095;
    value /= 100;
    //LOG_I("[%d]adc:%d--value:%d", ch, adc, value);
    return (u16)(value);
}

void dac_init(void)
{
    MX_DAC_Init();
    uuz_vDacData_Init();
    /*关闭设备输出状态*/
    HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
    HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);

    /* NOTE:刚上电 初始化DAC状态为0V*/
    dac_set_value(_CHANNEL_1, 0);
    dac_set_value(_CHANNEL_2, 0);
}

void dac_set_value(u8 ch, u16 level)
{
    //计算DAC值Value
    u32 value = 4095 * 10 * level;
    value /= 49;
    value /= 33;

    //输出DAC值
    u32 dac_ch = (ch == _CHANNEL_1) ? (DAC_CHANNEL_1) : (DAC_CHANNEL_2);
    HAL_DAC_Stop(&hdac, dac_ch);
    HAL_DAC_SetValue(&hdac, dac_ch, DAC_ALIGN_12B_R, value);
    HAL_DAC_Start(&hdac, dac_ch);
}
