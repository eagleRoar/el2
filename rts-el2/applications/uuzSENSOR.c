/* Includes ------------------------------------------------------------------*/
#include "uuzSENSOR.h"
/* ----------------------------------------------------------------*/
void uuz_vSensorData_Anglysis(u16 data, Sensor_Typedef_t * sensor)
{
    u16 count = sensor->count;

    if (count >= ANALYSIS_COUNT) {
        sensor->analysis = uuz_vAdcVoltage_Get(sensor->sensorData, count);
        //初始化结构数据
        if ((sensor->low == 0) && (sensor->high == 0)) {
            sensor->low = sensor->analysis;
            sensor->high = sensor->analysis;
        }
        //更新传感器数据组的上限和下限
        sensor->low = (sensor->low > sensor->analysis) ? (sensor->analysis) : (sensor->low);
        sensor->high = (sensor->high < sensor->analysis) ? (sensor->analysis) : (sensor->high);

        if (((sensor->high - sensor->low) > 22) || (sensor->ccount > 30)) {
            sensor->low = 0;
            sensor->high = 0;
            sensor->middle = 0;
            sensor->ccount = 0;
        } else {
            sensor->middle = (sensor->low + sensor->high) / 2;
        }
        count = 0;
        sensor->ccount++;
    }

    sensor->sensorData[count++] = data;
    sensor->count = count;
}

u16 uuz_vAdcVoltage_Get(const u16 * SensorData, s16 count)
{
    s16 i;
    u16 max, min;
    u16 avg;
    u32 amount = 0;

    if (count > 0) {
        if (count < 3) {
            //less than (ADC_FILTER_MAX), calculated directly statistics
            for (i = 0; i < count; i++) {
                amount += SensorData[i];
            }
            avg = amount / count;

        } else {
            if (SensorData[0] < SensorData[1]) {
                min = SensorData[0];
                max = SensorData[1];
            } else {
                min = SensorData[1];
                max = SensorData[0];
            }

            for (i = 2; i < count; i++) {
                if (SensorData[i] < min) {
                    amount += min;
                    min = SensorData[i];
                } else {
                    if (SensorData[i] > max) {
                        amount += max;
                        max = SensorData[i];
                    } else {
                        amount += SensorData[i];
                    }
                }
            }
            avg = amount / (count - 2);
        }
        return avg;
    } else {
        return 0;
    }
}
/*-----------------------------------------------------------------*/
