#ifndef __UUZ_INIT_H
#define __UUZ_INIT_H

#include "typedefINIT.h"
#include <board.h>

//设备系统数据
extern SysConfig_Typedef_t xSysConfig;
//设备日志数据
Logs_Typedef_t xLogs[__LOG_MAX];
//临时缓存数据
extern DeviceCache_Typedef_t xCache;
extern u32 xSta[__STA_MAX]; //状态值

#ifdef __cplusplus
extern "C" {
#endif

void uuz_vLocalInit(void);
void uuz_vDeviceConfigInit(u8 ucMask);
/* 默认配置初始化 */
void uuz_vDeviceConfigDefaultInit(void);
void uuz_vDeviceCacheDefaultInit(void);
void uuz_vSysConfig_ModbusID_Set(u16 usID, u8 ucType);

void PVD_Config(void);

//配置数据读取
u16 get_data(u8 addr, u8 num);
void set_data(u8 addr, u8 num, u16 data);
void save_data(void);
void set_logs(u8 num, u8* data, u8 len);
void save_logs(void);
void pvd_sync_init(void);
void pvd_data_update(void);
void pvd_enable_set(u8 state);

#ifdef __cplusplus
}
#endif

#endif // __UUZ_INIT_H
