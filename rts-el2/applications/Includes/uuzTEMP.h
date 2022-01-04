#ifndef _UUZ_TEMP_H
#define _UUZ_TEMP_H

#include <typedefTEMP.h>
#include <typedefDEF.h>
#include <uuzINIT.h>
#include <typedefSENSOR.h>

#ifdef __cplusplus
extern "C" {
#endif

extern Temperature_Cache_Typedef_t xTemperatureCache[2];    //CH1|CH2

void uuz_vTempData_Init(void);
void uuz_vTempDevice_Init(void);
u16 uuz_usTempData_Get(u16 voltage, Sensor_Typedef_t * sensor);
u16 uuz_usTempData_Convert(u16 voltage);
u16 uuz_usTempUnit_ConvCplt(u16 usT);
u16 uuz_usTempF2C_ConvCplt(u16 usT);
u16 uuz_usTemp2Int_ConvCplt(u16 usT);

u8 uuz_usTempValue_LimitGet(u16 usT, u8 ch);
void uuz_vTemperature_Get(Temperature_Cache_Typedef_t * xTemp);
u16 uuz_vCompensation_Get(u16 usTemperature, u8 ch);

#ifdef __cplusplus
}
#endif

#endif // UUZ_TEMP_H
