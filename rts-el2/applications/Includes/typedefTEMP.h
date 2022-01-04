#ifndef _TYPEDEF_TEMP_H
#define _TYPEDEF_TEMP_H

#include "typedefDEF.h"
#include <rtdevice.h>

typedef struct temperature_cache_t
{
    //通道数据是否存在
    rt_base_t pin;          //对应端口
    rt_adc_device_t dev;    //对应adc设备
    u8 channel;             //对应adc通道

} Temperature_Cache_Typedef_t;

#endif // _TYPEDEF_TEMP_H
