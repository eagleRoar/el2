#ifndef __UUZ_DAC_H
#define __UUZ_DAC_H

#include "typedefDEF.h"
#include <stm32f1xx_hal_dac.h>

#ifdef __cplusplus
extern "C" {
#endif

//启动温度读取同步线程
void dac_init(void);
void dac_set_value(u8 ch, u16 value);
void level_to_dac(u16 ch, u16 level);
u16 dac_to_level(u16 ch, u16 adc);

#ifdef __cplusplus
}
#endif
#endif // __UUZ_DAC_H
