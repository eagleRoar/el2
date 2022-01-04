#ifndef __UUZ_ADC_H
#define __UUZ_ADC_H

#include "typedefDEF.h"
#include <rtthread.h>
#include <rtdevice.h>

#define VOLTAGE_DEV_NAME      "adc1"      /* ADC 设备名称 */
#define TEMPERATURE_DEV_NAME  "adc2"      /* ADC 设备名称 */
#define PVD_VOL_CHANNEL       (0U)        /* ADC1 通道 */
#define CH1_VOL_CHANNEL       (6U)        /* ADC1 通道 */
#define CH2_VOL_CHANNEL       (7U)        /* ADC1 通道 */
#define CH1_TA_CHANNEL        (8U)        /* ADC2 通道 */
#define CH2_TA_CHANNEL        (9U)        /* ADC2 通道 */

#define REFER_VOLTAGE         (330U)      /* 参考电压 3.3V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS          (1 << 12)   /* 转换位数为12位 */

#define TEMPERATURE_AD_FULL   (4095U)     /* 温度的AD满值 */
//温度相关
#define TEMPERATRUE_BASE      (250U)     /* 基础温度换算250=25.0 */

extern rt_adc_device_t voltage_dev;
extern rt_adc_device_t temperature_dev;

#ifdef __cplusplus
extern "C" {
#endif

//启动温度读取同步线程
void adc_sync_init(void);
void adc_init(void);
rt_uint32_t adc_read(rt_adc_device_t dev, rt_uint32_t channel);

#ifdef __cplusplus
}
#endif
#endif // __UUZ_ADC_H
