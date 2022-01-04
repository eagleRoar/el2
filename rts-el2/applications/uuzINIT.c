/******************************************************************************/
#include <rtthread.h>
#include "drv_flash.h"
#include <string.h>
/******************************************************************************/
#include <uuzINIT.h>
#include "uuzGPIO.h"
#include "uuzOpt.h"
#include "typedefINIT.h"
#include "uuzTEMP.h"
#include "uuzRTC.h"
/******************************************************************************/
#define DBG_ENABLE
#define DBG_SECTION_NAME "INIT"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
/******************************************************************************/
static rt_mutex_t flash_mutex = RT_NULL;
SysConfig_Typedef_t xSysConfig;
Logs_Typedef_t xLogs[__LOG_MAX ];
DeviceCache_Typedef_t xCache;
u16 xSta[__STA_MAX ];    //状态值
/******************************************************************************/
/**
 * @brief uuz_vLocalInit
 * @param argument
 */
void uuz_vLocalInit(void)
{

    /* 创建一个Flash互斥量 */
    flash_mutex = rt_mutex_create("flash_m", RT_IPC_FLAG_FIFO);
    if (flash_mutex == RT_NULL) {
        LOG_I("create flash mutex failed.");
    }

    /* 初始化设备配置数据 */
    uuz_vDeviceConfigInit(0);
    /* 更新设备缓存数据 */
    uuz_vDeviceCacheDefaultInit();
}

/**
 * @brief uuz_vDeviceConfigInit
 * @param ucMask
 */
void uuz_vDeviceConfigInit(u8 ucMask)
{
    int readLen;

    if (!ucMask) {
        //从Flash读取Device Info相关数据
        readLen = stm32_flash_read(SYS_CONFIG_ADDRESS, (u8*) &xSysConfig, uuzCONFIG_LEN);
        LOG_I("Need Read:%d, ReadLength:%d", uuzCONFIG_LEN, readLen);
        if (readLen != uuzCONFIG_LEN) {
            // 读取失败，重置数据
            ucMask = 1;
        }
        //从Flash读取Logs Info相关数据
        readLen = stm32_flash_read(SYS_LOGS_ADDRESS, (u8*) &xLogs[0], (__LOG_MAX * uuzLOGS_LEN));
        if (readLen != (__LOG_MAX * uuzLOGS_LEN)) {
            // 读取失败，重置数据
            ucMask = 1;
        }
    }

    //初始化标记位或配置标记错误或结束符错误
    if ((ucMask) || (xSysConfig.end != 0xBCDEU)) {
        if (ucMask == 1) {
            LOG_E("User Init SysConfig");
        }

        if (xSysConfig.end != 0xBCDEU) {
            LOG_E("System end Flag is Error[%d]", xSysConfig.end);
        }
        uuz_vDeviceConfigDefaultInit();
    } else {
        //读取成功
        //读取系统配置数据
        xSysConfig.Sys[_D_BOOT_COUNT]++;
    }

    LOG_I("Read configuration is Success [%d].", xSysConfig.Sys[_D_BOOT_COUNT]);
    LOG_I("[SW]version:%d.%02d.%02d ", (xSysConfig.Sys[_D_SW_VERSION] / 10000),
            ((xSysConfig.Sys[_D_SW_VERSION] % 10000) / 100), (xSysConfig.Sys[_D_SW_VERSION] % 100));
    LOG_I("[HW]version:%d.%02d.%02d ", (xSysConfig.Sys[_D_HW_VERSION] / 10000),
            ((xSysConfig.Sys[_D_HW_VERSION] % 10000) / 100), (xSysConfig.Sys[_D_HW_VERSION] % 100));

    /* 保存数据FLASH*/
    save_data();
}

/**
 * @brief uuz_vDeviceConfigDefaultInit
 */
void uuz_vDeviceConfigDefaultInit(void)
{
    //恢复设备ID参数默认设置
    rt_memset(&xSysConfig, 0x00, uuzCONFIG_LEN);

    //设备系统日志信息
    xSysConfig.Sys[_D_INIT_STATE] = 0x00U; /* 0 - Time Set isn't Inited; 1 - Time Set is Inited */
    xSysConfig.Sys[_D_BOOT_COUNT] = 0x01U;
    xSysConfig.Sys[_D_SW_VERSION] = 10000U;      //V1.00.00
    xSysConfig.Sys[_D_HW_VERSION] = 495U;        //V4.95
    xSysConfig.Sys[_D_DEVICE_TYPE] = uuzCONFIG_DEVICE_TYPE;
    xSysConfig.Sys[_D_PLC_STATION] = 0xF7U;      //默认设置值:0xF7U
    xSysConfig.Sys[_D_DATE_FORMAT] = 0x00U;       //默认设置值:01-MM/DD/YY
    xSysConfig.Sys[_D_TIME_FORMAT] = _TYPE_24HOUR;      //默认设置值:24HOUR
    xSysConfig.Sys[_D_LANGUAGE] = 0x00U;         //默认设置值:0:English
    //设备从机信息
    xSysConfig.Sys[_D_IS_CONNECTED] = 0x00;       //连接状态
    xSysConfig.Sys[_D_SLAVE_STATION] = 0x01;        //从机设备Modbus ID
    xSysConfig.Sys[_D_SLAVE_TYPE] = 0x01;        //从机设备类型
    //设备的灯光部分的相关设置
    xSysConfig.Light[_L_TEMP_UNITS] = _TYPE_C;         //0:C摄氏度
    //Channel-1
    xSysConfig.Light[_L_CH1_OUTPUT_MODE] = 0x00U;    //0:OFF MODE
    xSysConfig.Light[_L_CH1_CYCLE_ON] = 0U;        //480=8:00
    xSysConfig.Light[_L_CH1_CYCLE_OFF] = 720U;       //960=18:00
    xSysConfig.Light[_L_CH1_OUTPUT_LEVEL] = 100U;    //100%
    xSysConfig.Light[_L_CH1_DISPLAY_MODE] = 0x00U;    //0:%;1:1000W;2:750W;3:600W;4:400W
    xSysConfig.Light[_L_CH1_SUNRISE_DELAY] = 0x0FU;    //0:NONE(0~30)min
    xSysConfig.Light[_L_CH1_SUNSET_DELAY] = 0x0FU;    //0:NONE(0~30)min
    xSysConfig.Light[_L_CH1_ECM_MODE] = 0x00U;       //0:Day Mode
    //Light Config for Temperature
    xSysConfig.Light[_L_CH1_NIGHT_ALARM_TEMP] = 275U;    //300=30.0C
    xSysConfig.Light[_L_CH1_DAY_ALARM_TEMP] = 275U;    //350=35.0C
    xSysConfig.Light[_L_CH1_AUTO_DIM_TEMP] = 300U;    //300=30.0C
    xSysConfig.Light[_L_CH1_SHUTDOWN_TEMP] = 350U;    //350=35.0C
    xSysConfig.Light[_L_CH1_TEMP_HIGH] = _TEMP_NULL;    //_TEMP_NULL=255.0C表示N/A
    xSysConfig.Light[_L_CH1_TEMP_LOW] = _TEMP_NULL;    //_TEMP_NULL=255.0C表示N/A
    //Channel-2
    xSysConfig.Light[_L_CH2_OUTPUT_MODE] = 0x00U;    //0:FOLLOW MAIN
    xSysConfig.Light[_L_CH2_CYCLE_ON] = 0U;        //480=8:00
    xSysConfig.Light[_L_CH2_CYCLE_OFF] = 720U;       //960=18:00
    xSysConfig.Light[_L_CH2_OUTPUT_LEVEL] = 100U;    //100%
    xSysConfig.Light[_L_CH2_DISPLAY_MODE] = 0x00U;    //0:%;1:1000W;2:750W;3:600W;4:400W
    xSysConfig.Light[_L_CH2_SUNRISE_DELAY] = 0x0FU;    //0:NONE(0~30)min
    xSysConfig.Light[_L_CH2_SUNSET_DELAY] = 0x0FU;    //0:NONE(0~30)min
    xSysConfig.Light[_L_CH2_ECM_MODE] = 0x00U;       //0:Day Mode
    //Light Config for Temperature
    xSysConfig.Light[_L_CH2_NIGHT_ALARM_TEMP] = 275U;    //300=30.0C
    xSysConfig.Light[_L_CH2_DAY_ALARM_TEMP] = 275U;    //350=35.0C
    xSysConfig.Light[_L_CH2_AUTO_DIM_TEMP] = 300U;    //300=30.0C
    xSysConfig.Light[_L_CH2_SHUTDOWN_TEMP] = 350U;    //350=35.0C
    xSysConfig.Light[_L_CH2_TEMP_HIGH] = _TEMP_NULL;    //_TEMP_NULL=255.0C表示N/A
    xSysConfig.Light[_L_CH2_TEMP_LOW] = _TEMP_NULL;    //_TEMP_NULL=255.0C表示N/A
    xSysConfig.Light[_L_CAL_VAL1] = 127U;            //大于127为正,小于127为负，默认为127，1为0.1C(0~256：表示-12.7C～12.8)
    xSysConfig.Light[_L_CAL_VAL2] = 127U;            //大于127为正,小于127为负，默认为127，1为0.1C(0~256：表示-12.7C～12.8)
    //灯光频道
    xSysConfig.Light[_L_CH2_MODE] = 0x00U;           //0:FOLLOW模式;;1:INDEPENDENT; 2:INVERSE;
    //添加结束默认值
    xSysConfig.end = 0xBCDEU;
    //初始化日志信息
    rt_memset(&xLogs[0], 0x00, (uuzLOGS_LEN * __LOG_MAX));
}

/**
 * @brief uuz_vDeviceCacheDefaultInit
 */
void uuz_vDeviceCacheDefaultInit(void)
{
    //初始化缓存数据
    rt_memset(&xCache, 0x00, sizeof(DeviceCache_Typedef_t));
    //初始化缓存数据
    rt_memset(xSta, 0x00, __STA_MAX);
    //临时编号
    xSta[_S_DEVICE_ID1] = 0x1BBC;
    xSta[_S_DEVICE_ID2] = 0xCE04;
}

/**
 * @brief 设置设备相关的MODBUS-ID
 * @param usID(01~247)
 * @param ucType:0:Master-ID 1:Slave-ID
 */
void uuz_vSysConfig_ModbusID_Set(u16 usID, u8 ucType)
{
    if (ucType == 0) {
        xSysConfig.Sys[_D_PLC_STATION] = usID;
    } else if (ucType == 1) {
        xSysConfig.Sys[_D_SLAVE_STATION] = usID;
    }
}

/**
 * @brief 获取数据库数据，返回u16的数据
 *
 * @param type
 * @param num
 * @return 返回的数据(u16)
 */
u16 get_data(u8 type, u8 num)
{
    u16 data = 0xFF;

    if (type == _D_SYS) {
        //System Data
        if (num < __SYSTEM_MAX) {
            data = xSysConfig.Sys[num];
        }
    } else if (type == _D_LT) {
        //Light Data
        if (num < __LIGHT_MAX) {
            data = xSysConfig.Light[num];
        }
    }

    return data;
}

/**
 * @brief 给具体地址设置数据
 *
 * @param type
 * @param num
 * @param data
 */
void set_data(u8 type, u8 num, u16 data)
{
    if (type == _D_SYS) {
        //System Data
        if (num < __SYSTEM_MAX) {
            xSysConfig.Sys[num] = data;
        }
    } else if (type == _D_LT) {
        //Light Data
        if (num < __LIGHT_MAX) {
            xSysConfig.Light[num] = data;
        }
    }
}

/**
 *
 * @brief 保存相关LOG
 * @param num
 * @param data
 * @param len
 */
void set_logs(u8 num, u8 * data, u8 len)
{
    //Logs Data
    if (num < __LOG_MAX) {
        //保存日志时间记录
        xLogs[num].en = 0x01U;
        //日期
        xLogs[num].date[0] = rTm.tm_year + 1900;               //Year
        xLogs[num].date[1] = rTm.tm_mon + 1;                //Month
        xLogs[num].date[2] = rTm.tm_mday;               //Day
        //时间
        xLogs[num].time[0] = rTm.tm_hour;               //Hour
        xLogs[num].time[1] = rTm.tm_min;                //Min
        xLogs[num].time[2] = 0x00U;                     //Second

        //保存相关数据
        if (data != NULL) {
            rt_memcpy(&xLogs[num].data, data, len);
        }
    }
}

/**
 * @brief  保存当前数据
 */
void save_data(void)
{
    rt_mutex_take(flash_mutex, RT_WAITING_FOREVER);
    /* 保存数据FLASH*/
    stm32_flash_erase(SYS_CONFIG_ADDRESS, uuzCONFIG_LEN);
    stm32_flash_write(SYS_CONFIG_ADDRESS, (u8*) &xSysConfig, uuzCONFIG_LEN);
    rt_mutex_release(flash_mutex);
}

/**
 * @brief  保存日志
 */
void save_logs(void)
{
    rt_mutex_take(flash_mutex, RT_WAITING_FOREVER);
    /* 保存数据FLASH*/
    stm32_flash_erase(SYS_LOGS_ADDRESS, (__LOG_MAX * uuzLOGS_LEN));
    stm32_flash_write(SYS_LOGS_ADDRESS, (u8*) &xLogs[0], (__LOG_MAX * uuzLOGS_LEN));
    rt_mutex_release(flash_mutex);
}
