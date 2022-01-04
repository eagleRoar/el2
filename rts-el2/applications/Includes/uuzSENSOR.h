#ifndef UUZ_SENSOR_H
#define UUZ_SENSOR_H

#include "typedefSENSOR.h"
#include "typedefDEF.h"

#ifdef __cplusplus
extern "C" {
#endif

u16  uuz_vAdcVoltage_Get(const u16 * SensorData, s16 count);
void uuz_vSensorData_Anglysis(u16 data, Sensor_Typedef_t * sensor);

#ifdef __cplusplus
}
#endif
#endif // UUZ_SENSOR_H
