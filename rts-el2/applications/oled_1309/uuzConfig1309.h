#ifndef __UUZ_CONFIG_1309_H
#define __UUZ_CONFIG_1309_H

#include "typedefDEF.h"
#include <u8g2_port.h>
#include <rtthread.h>

//字体默认相关参数
#define _FONT_DEFAULT (u8g2_font_7x13B_mr)
#define _FONT_TITLE_DEFAULT (u8g2_font_t0_18b_mr)

#define uuzFONT_DEFAULT_W (18U)
#define uuzFONT_DEFAULT_H (16U)

struct _ui_init_t
{
    u8 _logo;
    u8 _show_ch;
    u8 _show_date;
    u8 _show_ta;
    u8 _light;
    u8 _menu;
    u8 _menu_logs;
    u8 _channel;
    u8 _output_mode;
    u8 _output_level;
    u8 _light_cycle;
    u8 _set_temp_levels;
    u8 _sunrise_sunset;
    u8 _ecm_config;
    u8 _display_mode;
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
    uuzUI_MENU_TIME,           //时间设置界面 34
    uuzUI_MENU_LANG,           //语言切换 35
    uuzUI_MENU_INFO,           //系统信息切换 36
    uuzUI_MENU_FAC,            //恢复出厂设置 37

    uuzUI_OUTPUT_MODE = 310U,    //Channel 输出模式 310
    uuzUI_OUTPUT_LEVEL,         //Channel 输出等级 311
    uuzUI_LIGHT_CYCLE,          //Channel 灯光循环设置 312
    uuzUI_SET_TEMP_LEVELS,      //Channel 设置温度参数 313
    uuzUI_SUNRISE_SUNSET,       //Channel 日出日落设置 314
    uuzUI_ECM_CONFIG,           //Channel ECM相关配置 315
    uuzUI_DISPLAY_MODE,         //Channel 输出显示模式 316

    uuzUI_TEMP_UNIT = 330U,            //Temperature的单位菜单 330
    uuzUI_TEMP_CAL,             //Temperature的校正菜单

    uuzUI_SAVE_OK = 1000U,      //数据保存成功
    uuzUI_SAVE_FAIL = 1001U,    //数据保存失败
    uuzUI_FAC_SUC = 1002U       //数据恢复出厂成功
} _ENUM_UI_ID_t;

//图形宏
#define _UINT_C (0x2103U)       //摄氏度单位 C
#define _UINT_F (0x2109U)       //华氏度单位 F
#define _UINT_TAB (0x25B6U)     //三角标记

#endif //__UUZ_CONFIG_1309_H
