#ifndef _TYPEDEF_SENSOR_H
#define _TYPEDEF_SENSOR_H

#include "typedefDEF.h"

#define ANALYSIS_COUNT (5U)

typedef struct _sensor_data_t
{
    u16 sensorData[ANALYSIS_COUNT];
    s16 count;
    u16 low;
    u16 high;
    u16 middle;
    u16 analysis;
    u16 ccount;

} Sensor_Typedef_t;

#endif // _TYPEDEF_SENSOR_H
