/* Includes ------------------------------------------------------------------*/
#include <uuzINIT.h>
#include "typedefDEF.h"
#include "uuzDAC.h"
#include "typedefUI.h"

#define DBG_ENABLE
#define DBG_SECTION_NAME "DAC "
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

DAC_HandleTypeDef hdac;

void MX_DAC_Init(void)
{
    DAC_ChannelConfTypeDef sConfig =
        { 0 };

    /** DAC Initialization
     */
    hdac.Instance = DAC;
    if (HAL_DAC_Init(&hdac) != HAL_OK) {
        Error_Handler();
    }
    /** DAC channel OUT1 config
     */
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
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
 * @brief 亮度等级转换成DAC
 * @param ch
 * @param level
 */
void level_to_dac(u16 ch, u16 level)
{
    u32 value = 4095 * 10 * level;
    //提供DAC值Value
    value /= 49;
    xSta[_S_CH1_DAC_OUT + _CH_DIS(ch)] = value / 33;
    //LOG_I("Level:[%d]--xSTA[%d]:%d", level, _S_CH1_DAC_OUT + _CH_DIS(ch), xSta[_S_CH1_DAC_OUT + _CH_DIS(ch)]);
    //执行标记
    xSta[_S_CH1_DAC_OPT + _CH_DIS(ch)] = 1;
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
    //LOG_I("value-1 = %d", value);
    value *= 33;
    value *= 49;
    value /= 10;
    value /= 4095;
    //LOG_I("value-5 = %d", value);
    return (u16) (value);
}

void dac_entry(void * parameter)
{
    MX_DAC_Init();
    /*关闭设备输出状态*/
    HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
    HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);
    //刚上电 初始化DAC状态为0V
    dac_set_value(_CHANNEL_1, 0);
    dac_set_value(_CHANNEL_2, 0);
    /* 更新DAC输出 */
    while (1) {
        if (xSta[_S_FLAG_STARTUP]) {
            if (xSta[_S_CH1_DAC_OPT + _CH_DIS(_CHANNEL_1)] == 1) {
                dac_set_value(_CHANNEL_1, xSta[_S_CH1_DAC_OUT + _CH_DIS(_CHANNEL_1)]);
                xSta[_S_CH1_DAC_OPT + _CH_DIS(_CHANNEL_1)] = 0;
                //LOG_I("Level1[%d]", xSta[_S_CH1_DAC_OUT + _CH_DIS(_CHANNEL_1)]);
            }

            if (xSta[_S_CH1_DAC_OPT + _CH_DIS(_CHANNEL_2)] == 1) {
                dac_set_value(_CHANNEL_2, xSta[_S_CH1_DAC_OUT + _CH_DIS(_CHANNEL_2)]);
                xSta[_S_CH1_DAC_OPT + _CH_DIS(_CHANNEL_2)] = 0;
                //LOG_I("Level2[%d]", xSta[_S_CH1_DAC_OUT + _CH_DIS(_CHANNEL_2)]);
            }
        }
        rt_thread_mdelay(500);
    }
}

void dac_init(void)
{
    /* 创建 dac 线程 */
    rt_thread_t thread = rt_thread_create("dac opt", dac_entry,
    RT_NULL, 512, 24, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
    }
}

void dac_set_value(u8 ch, u16 value)
{
    u32 dac_ch;

    if (ch == _CHANNEL_1) {
        dac_ch = DAC_CHANNEL_1;
    } else {
        dac_ch = DAC_CHANNEL_2;
    }
    HAL_DAC_Stop(&hdac, dac_ch);
    HAL_DAC_SetValue(&hdac, dac_ch, DAC_ALIGN_12B_R, value);
    HAL_DAC_Start(&hdac, dac_ch);
}
