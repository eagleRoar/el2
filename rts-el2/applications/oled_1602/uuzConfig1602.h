#ifndef __UUZ_CONFIG_1602_H
#define __UUZ_CONFIG_1602_H

#include "typedefDEF.h"

//----------------OLED屏幕显示----------------
#define OLED_W  (16U)   //屏幕宽度
#define OLED_H  (2U)   //屏幕宽度

typedef struct _oled_lcd_t
{
    //缓存数据区
    u8 data[OLED_H][OLED_W];
    u8 x;
    u8 y;
} OledLcd_Typedef_t;

//界面初始化标记
struct _ui_init_t
{
    //缓存数据区标记
    u8 _logo;
    u8 _show_ch;
    u8 _show_date;
    u8 _show_ta;
    u8 _light;
    //Menu
    u8 _menu;
    u8 _menu_logs;
    //Light Menu
    u8 _channel;
    u8 _output_mode;
    u8 _output_level;
    u8 _light_cycle;
    u8 _set_temp_levels;
    u8 _sunrise_sunset;
    u8 _ecm_config;
    u8 _display_mode;
    //System Time
    u8 _temp_units;
    u8 _temp_cal;
    u8 _menu_mode;
    u8 _menu_time;
    u8 _menu_lang;
    u8 _menu_info;
    u8 _menu_fac;
};

//界面编号相关
typedef enum
{
    uuzUI_NULL = 0U,            //初始化界面 0
    uuzUI_LOGO,                 //主界面 1
    uuzUI_SHOW_CH,              //频道信息 2 频道灯光信息
    uuzUI_MENU,                 //设置界面 3 菜单信息
    uuzUI_SHOW_DATE,            //频道信息 4 日期相关信息
    uuzUI_SHOW_TA,              //频道信息 5 温度相关告警信息
    uuzUI_LIGHT,                //显示界面 6 灯光相关信息

    uuzUI_MENU_LOGS = 30U,     //log的相关信息显示 30
    uuzUI_MENU_CH1,            //Channel-1的设置菜单 31
    uuzUI_MENU_CH2,            //Channel-1的设置菜单 32
    uuzUI_MENU_TEMP,           //Temperature的设置菜单 33
    uuzUI_MENU_MODE,           //Channel-2的mode配置34
    uuzUI_MENU_TIME,           //时间设置界面 35
    uuzUI_MENU_LANG,           //语言切换 36
    uuzUI_MENU_INFO,           //系统信息切换 37
    uuzUI_MENU_FAC,            //恢复出厂设置 38

    uuzUI_OUTPUT_MODE = 310U,    //Channel 输出模式 310
    uuzUI_OUTPUT_LEVEL,         //Channel 输出等级 311
    uuzUI_LIGHT_CYCLE,          //Channel 灯光循环设置 312
    uuzUI_SET_TEMP_LEVELS,      //Channel 设置温度参数 313
    uuzUI_SUNRISE_SUNSET,       //Channel 日出日落设置 314
    uuzUI_ECM_CONFIG,           //Channel ECM相关配置 315
    uuzUI_DISPLAY_MODE,         //Channel 输出显示模式 316

    uuzUI_TEMP_UNIT = 330U,     //Temperature的单位菜单 330
    uuzUI_TEMP_SEL,                 //Temperature的校正选择331
    uuzUI_TEMP_CAL,             //Temperature的校正菜单332

    uuzUI_SAVE_OK = 1000U,      //数据保存成功
    uuzUI_SAVE_FAIL = 1001U,    //数据保存失败
    uuzUI_FAC_SUC = 1002U       //数据恢复出厂成功
} _ENUM_UI_ID_t;

#define SENSOR_ERR_TIP   "SENSOR REMOVED"
#define SENSOR_ERR_TIP2  "REMOVED       "
#define SENSOR_NULL_TIP  "              "

//字库代码
#define FONT_LIB_EN_JP (0x38U)
#define FONT_LIB_EUR1 (0x39U)
#define FONT_LIB_EN_RU (0x3AU)
#define FONT_LIB_EUR2 (0x3BU)

//字库显示配置
#define _DISPLAY_ON (0x04U)
#define _DISPLAY_OFF (0x00U)
#define _CURSOR_ON (0x02U)
#define _CURSOR_OFF (0x00U)
#define _BLINK_ON (0x01U)
#define _BLINK_OFF (0x00U)

//刷新屏幕状态
#define _SYNC_1 (0x00U) //刷新上半屏
#define _SYNC_2 (0x01U) //刷新下半屏
#define _SYNC_A (0x02U) //刷新全屏

//图形宏
#define _UINT_C (0x43U)       //摄氏度单位 C
#define _UINT_F (0x46U)       //华氏度单位 F
#define _UINT_TAB (0xF6U)     //三角标记

//屏幕缓存参数
#define _OLED_ (0x00U) //写到屏幕上
#define _DATA_ (0x01U) //写到缓存区上

//显示预加载内容
#define uuzDATA_NA "N/A             "
#define uuzTEMP_NA "H:N/A    L:N/A  "
#define uuzNULL_NA "                "

#endif // __UUZ_CONFIG_1602_H
