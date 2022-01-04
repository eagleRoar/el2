/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include "string.h"
/* ------------------------- package ----------------------------------------*/
#include "drv_flash.h"
#include "agile_button.h"
/* -------------------------------------------------------------------------------*/
#include "uuzINIT.h"
#include "typedefDEF.h"
#include "uuzBTN.h"
/* -------------------------------------------------------------------------------*/
#include "uuzTEMP.h"
#include "uuzDAC.h"
#include "uuzRTC.h"
#include "uuzLED.h"
/* -------------------------------------------------------------------------------*/
#include "uuzUI.h"
#include "typedefUI.h"
#include "uuzOLED_1602.h"
#include "uuzUI_1602.h"
#include "uuzConfig1602.h"
#include "uuzUI_Cache.h"
#include "uuzConfigLIGHT.h"
#include "uuzLIGHT.h"
#include "typedefLIGHT.h"
/* -------------------------------------------------------------------------------*/
#define DBG_ENABLE
#define DBG_SECTION_NAME "1602"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

struct _ui_init_t xInit;

//短按判断
void ui_short_event(rt_base_t btn_pin)
{
    //有按键，清除按键标记
    xUI.delay = 0;
    //通过界面判断入口
    switch (xUI.cID)
    {
        case uuzUI_LOGO:
            logo_opt(btn_pin);
            break;
        case uuzUI_SHOW_CH:
            show_ch_opt(btn_pin);
            break;
        case uuzUI_SHOW_DATE:
            show_date_opt(btn_pin);
            break;
        case uuzUI_SHOW_TA:
            show_ta_opt(btn_pin);
            break;
        case uuzUI_LIGHT:
            light_opt(btn_pin);
            break;
        case uuzUI_MENU:
            menu_opt(btn_pin);
            break;
        case uuzUI_MENU_LOGS:
            logs_opt(btn_pin);
            break;
        case uuzUI_MENU_CH1:
        case uuzUI_MENU_CH2:
            channel_opt(btn_pin);
            break;
        case uuzUI_MENU_TEMP:
            temp_units_opt(btn_pin);
            break;
        case uuzUI_TEMP_SEL:
            temp_select_opt(btn_pin);
            break;
        case uuzUI_TEMP_CAL:
            temp_cal_opt(btn_pin);
            break;
        case uuzUI_MENU_MODE:
            ch2_mode_opt(btn_pin);
            break;
        case uuzUI_MENU_TIME:
            time_opt(btn_pin);
            break;
        case uuzUI_MENU_LANG:
            lang_opt(btn_pin);
            break;
        case uuzUI_MENU_INFO:
            info_opt(btn_pin);
            break;
        case uuzUI_MENU_FAC:
            factory_reset_opt(btn_pin);
            break;
        case uuzUI_OUTPUT_MODE:
            output_mode_opt(btn_pin);
            break;
        case uuzUI_OUTPUT_LEVEL:
            output_level_opt(btn_pin);
            break;
        case uuzUI_LIGHT_CYCLE:
            light_cycle_opt(btn_pin);
            break;
        case uuzUI_SET_TEMP_LEVELS:
            set_temp_levels_opt(btn_pin);
            break;
        case uuzUI_SUNRISE_SUNSET:
            sunrise_sunset_opt(btn_pin);
            break;
        case uuzUI_ECM_CONFIG:
            ecm_config_opt(btn_pin);
            break;
        case uuzUI_DISPLAY_MODE:
            display_mode_opt(btn_pin);
            break;
        default:
            break;
    }
}

//长按判断，每隔1秒触发1次
void ui_hold_event(rt_base_t btn_pin)
{
    //有按键，清除按键标记
    xUI.delay = 0;
    //通过界面判断入口
    switch (xUI.cID)
    {
        case uuzUI_SHOW_CH:
            show_ch_opt(btn_pin);
            break;
        case uuzUI_SHOW_DATE:
            show_date_opt(btn_pin);
            break;
        case uuzUI_SHOW_TA:
            show_ta_opt(btn_pin);
            break;
        case uuzUI_MENU_TIME:
            time_opt(btn_pin);
            break;
        case uuzUI_LIGHT:
            light_opt(btn_pin);
            break;
        case uuzUI_MENU_CH1:
        case uuzUI_MENU_CH2:
            channel_opt(btn_pin);
            break;
        case uuzUI_OUTPUT_LEVEL:
            output_level_opt(btn_pin);
            break;
        case uuzUI_LIGHT_CYCLE:
            light_cycle_opt(btn_pin);
            break;
        case uuzUI_SET_TEMP_LEVELS:
            set_temp_levels_opt(btn_pin);
            break;
        case uuzUI_SUNRISE_SUNSET:
            sunrise_sunset_opt(btn_pin);
            break;
        case uuzUI_TEMP_CAL:
            temp_cal_opt(btn_pin);
            break;
        default:
            break;
    }
}

/**
 * @brief 长按3秒清除数据
 */
void btn_long_event(void)
{
    static u8 btnCount = 0;

    //有PVD报警情况下，3秒清空相关PVD数据
    if (btnCount >= 3) {
        if (xSta[_S_FLAG_PVD] == 1) {
            xSta[_S_FLAG_PVD] = 0;
            //切换灯常量
            led_on(_LD_POWER);
        }

        xLogs[_G_CH1_AUTO_DIM].en = 0;
        xSta[_S_LED_TA1] = 0;

        xLogs[_G_CH2_AUTO_DIM].en = 0;
        xSta[_S_LED_TA2] = 0;
        xLogs[_G_CH1_SHUTDOWN].en = 0;
        xSta[_S_LED_TA1] = 0;

        xSta[_S_LED_TA2] = 0;
        xLogs[_G_CH2_SHUTDOWN].en = 0;

        //保存相关数据
        save_logs();
        btnCount = 0;
    }
    btnCount++;
}

/**
 * @note 界面同步线程
 * @param parameter
 */
void ui_thread_entry(void* parameter)
{
    //显示启动LOGO界面
    xUI.cID = uuzUI_NULL;
    //初始化界面标记
    show_ui(uuzUI_LOGO);

    while (1) {
        xUI.delay++;
        if ((xUI.delay % (_UI_AUTO_BACK_SEC * _UI_DELAY_SEC)) == 0) {
            if ((xUI.cID == uuzUI_MENU_TIME) && (xSysConfig.Sys[_D_INIT_STATE] == 0)) {
            } else {
                show_ui(uuzUI_SHOW_CH);
            }
        } else {
            switch (xUI.cID)
            {
                case uuzUI_LOGO:
                    //主LOGO界面等待2秒切换到显示界面
                    if (xUI.delay > (2 * _UI_DELAY_SEC)) {
                        if (xSysConfig.Sys[_D_INIT_STATE] == 0x00U) {
                            show_ui(uuzUI_MENU_TIME);
                        } else {
                            show_ui(uuzUI_SHOW_CH);
                        }
                    }
                    break;
                case uuzUI_SHOW_CH:
                case uuzUI_SHOW_DATE:
                case uuzUI_SHOW_TA:
                    //每1秒刷新界面数据
                    if ((xUI.delay % (1 * _UI_DELAY_SEC)) == 0) {
                        show_ui(xUI.cID);
                    }
                    break;
                case uuzUI_LIGHT:
                case uuzUI_MENU:
                case uuzUI_MENU_CH1:
                case uuzUI_MENU_CH2:
                case uuzUI_MENU_FAC:
                case uuzUI_MENU_TEMP:
                case uuzUI_MENU_MODE:
                case uuzUI_MENU_LANG:
                case uuzUI_OUTPUT_MODE:
                case uuzUI_OUTPUT_LEVEL:
                case uuzUI_DISPLAY_MODE:
                case uuzUI_LIGHT_CYCLE:
                case uuzUI_SUNRISE_SUNSET:
                case uuzUI_ECM_CONFIG:
                case uuzUI_SET_TEMP_LEVELS:
                case uuzUI_MENU_TIME:
                case uuzUI_TEMP_SEL:
                case uuzUI_TEMP_CAL:
                    if (xUI.blink == 1) {
                        show_ui(xUI.cID);
                    }
                    break;
                case uuzUI_MENU_INFO:
                    break;
                default:
                    break;
            }
        }
        //250ms
        rt_thread_mdelay(2 * _UI_DELAY_TIME);
    }
}

/**
 * @brief 显示界面
 * @param id
 */
void show_ui(u16 id)
{
    switch (id)
    {
        case uuzUI_LOGO:
            logo_disp();
            break;
        case uuzUI_SHOW_CH:
            show_ch_disp();
            break;
        case uuzUI_SHOW_DATE:
            show_date_disp();
            break;
        case uuzUI_SHOW_TA:
            show_ta_disp();
            break;
        case uuzUI_LIGHT:
            light_disp();
            break;
        case uuzUI_MENU_LOGS:
            logs_disp();
            break;
        case uuzUI_MENU_CH1:
        case uuzUI_MENU_CH2:
            channel_disp();
            break;
        case uuzUI_OUTPUT_MODE:
            output_mode_disp();
            break;
        case uuzUI_OUTPUT_LEVEL:
            output_level_disp();
            break;
        case uuzUI_LIGHT_CYCLE:
            light_cycle_disp();
            break;
        case uuzUI_SET_TEMP_LEVELS:
            set_temp_levels_disp();
            break;
        case uuzUI_SUNRISE_SUNSET:
            sunrise_sunset_disp();
            break;
        case uuzUI_ECM_CONFIG:
            ecm_config_disp();
            break;
        case uuzUI_DISPLAY_MODE:
            display_mode_disp();
            break;
        case uuzUI_MENU:
            menu_disp();
            break;
        case uuzUI_MENU_TEMP:
            temp_units_disp();
            break;
        case uuzUI_TEMP_SEL:
            temp_select_disp();
            break;
        case uuzUI_TEMP_CAL:
            temp_cal_disp();
            break;
        case uuzUI_MENU_MODE:
            ch2_mode_disp();
            break;
        case uuzUI_MENU_TIME:
            time_disp();
            break;
        case uuzUI_MENU_LANG:
            lang_disp();
            break;
        case uuzUI_MENU_INFO:
            info_disp();
            break;
        case uuzUI_MENU_FAC:
            factory_reset_disp();
            break;
        default:
            break;
    }
}

/**
 * @brief 显示Value控件的字符串
 * @param type
 * @param xValue
 * @param cmd
 */
void disp_txt(rt_uint8_t type, Value_Typedef_t * xValue, char * cmd)
{
    char data[OLED_W];
    u8 uclength = 0;
    u8 IsShow = 1;

    if ((xValue->h < OLED_H) && (xValue->w < OLED_W)) {
        if (cmd != NULL) {
            rt_sprintf(data, "%s", cmd);
        } else {
            rt_sprintf(data, "%s", xValue->cmd);
        }

        //判断光标闪烁
        if (xUI.cFoucs == xValue->foucs) {
            if (xValue->blink == 1) {
                if ((xUI.delay % 2) == 0) {
                    uclength = rt_strlen(data);
                    drawNull(type, xValue->h, xValue->w, uclength);
                    IsShow = 0;
                }
            }
        }

        if (IsShow) {
            drawString(type, xValue->h, xValue->w, data);
        }
    }
}

/**
 * @brief 显示Value控件数值
 * @param type
 * @param xValue
 * @param cmd
 */
void disp_value(rt_uint8_t type, Value_Typedef_t * xValue, char * cmd)
{
    char data[OLED_W];
    u8 uclength = 0;
    u8 IsShow = 1;

    if ((xValue->h < OLED_H) && (xValue->w < OLED_W)) {
        if (cmd != NULL) {
            rt_sprintf(data, "%d%s", xValue->val, cmd);
        } else {
            rt_sprintf(data, "%d%s", xValue->val, xValue->cmd);
        }
        uclength = rt_strlen(data);
        if (xUI.cFoucs == xValue->foucs) {
            if (xValue->blink == 1) {
                if ((xUI.delay % 2) == 0) {
                    drawNull(type, xValue->h, xValue->w, uclength);
                    IsShow = 0;
                }
            }
        }
        if (IsShow) {
            drawString(type, xValue->h, xValue->w, data);
        }
    }
}

/**
 *
 * @brief 显示xValue控件Time时间
 * @param type
 * @param format
 * @param xValue
 */
void disp_time(rt_uint8_t type, Value_Typedef_t * xValue)
{
    char data[OLED_W];
    u8 uclength = 0;
    u8 IsShow = 1;

    if ((xValue->h < OLED_H) && (xValue->w < OLED_W)) {
        rt_sprintf(data, "%02d%c", xValue->val, xValue->cmd[0]);
        if (xUI.cFoucs == xValue->foucs) {
            if (xValue->blink == 1) {
                if ((xUI.delay % 2) == 0) {
                    uclength = rt_strlen(data);
                    drawNull(type, xValue->h, xValue->w, uclength);
                    IsShow = 0;
                }
            }
        }
        if (IsShow) {
            drawString(type, xValue->h, xValue->w, data);
        }
    }
}

/**
 *
 * @brief 显示xValue控件Date时间
 * @param type
 * @param format
 * @param xValue
 */
void disp_date(rt_uint8_t type, Value_Typedef_t * xValue)
{
    char data[OLED_W];
    u8 IsShow = 1;

    if ((xValue->h < OLED_H) && (xValue->w < OLED_W)) {
        rt_sprintf(data, "%02d", xValue->val);
        if (xUI.cFoucs == xValue->foucs) {
            if (xValue->blink == 1) {
                if ((xUI.delay % 2) == 0) {
                    drawNull(type, xValue->h, xValue->w, 2);
                    IsShow = 0;
                }
            }
        }

        if (IsShow) {
            drawString(type, xValue->h, xValue->w, data);
        }
    }
}

/**
 * @brief 显示xValue的Temperature的数据
 * @param type
 * @param xValue
 * @param cmd
 */
void disp_temperature(rt_uint8_t type, Value_Typedef_t * xValue, char * cmd)
{
    char data[OLED_W];
    u8 uclength = 0;
    u8 IsShow = 1;

    if ((xValue->h < OLED_H) && (xValue->w < OLED_W)) {
        if (cmd != NULL) {
            rt_sprintf(data, "%d.%d%s", xValue->val / 10, xValue->val % 10, cmd);
        } else {
            rt_sprintf(data, "%d.%d%s", xValue->val / 10, xValue->val % 10, xValue->cmd);
        }

        if (xUI.cFoucs == xValue->foucs) {
            if (xValue->blink == 1) {
                if ((xUI.delay % 2) == 0) {
                    uclength = rt_strlen(data);
                    drawNull(type, xValue->h, xValue->w, uclength);
                    IsShow = 0;
                }
            }
        }
        if (IsShow) {
            drawString(type, xValue->h, xValue->w, data);
        }
    }
}

/**
 * @brief 显示光标标记
 * @param type
 * @param xValue
 */
void disp_cursor(rt_uint8_t type, Value_Typedef_t * xValue)
{
    u8 IsShow = 1;
    if ((xValue->h < OLED_H) && (xValue->w < OLED_W)) {
        if (xValue->blink == 1) {
            if ((xUI.delay % 2) == 0) {
                drawNull(type, xValue->h, xValue->w, 1);
                IsShow = 0;
            }
        }

        if (IsShow) {
            drawString(type, xValue->h, xValue->w, xValue->cmd);
        }
    }
}
/**
 * @brief 显示温度单位
 * @param type
 * @param xValue
 */
void disp_units(rt_uint8_t type, u8 h, u8 w, u8 tip)
{
    char cmd[OLED_W];
    if ((h < OLED_H) && (w < OLED_W)) {
        cmd[0] = 0xB2;
        if ((w + 1) < OLED_W) {
            cmd[1] = tip;
        }
        drawString(type, h, w, cmd);
    }
}

/**
 * @note 获取当前频道的灯光状态
 * @param ch
 * @param type
 * @return 返回瓦数(Display Mode 0 ~ 3)或者百分比数据（Display Mode:4)
 */
u16 level_get(u16 ch, u8 type)
{
    u16 watt = 0;

    //type:0-读取实时数据;1-更新修改数据
    if (type == 0) {
        if (ch == _CHANNEL_1 || ch == _CHANNEL_2) {
            xCache.level = get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(ch));
            xCache.watt_mode = get_data(_D_LT, _L_CH1_DISPLAY_MODE + _CH_LVL(ch));
            xCache.chMode[ch] = get_data(_D_LT, _L_CH1_OUTPUT_MODE + _CH_LVL(ch));
        }
    }
    //计算使用的瓦数
    switch (xCache.watt_mode)
    {
        case 1:    //1000W
            watt = 1000 * xCache.level / 100;
            break;
        case 2:    //750W
            watt = 750 * xCache.level / 100;
            break;
        case 3:    //600W
            watt = 600 * xCache.level / 100;
            break;
        case 4:    //400W
            watt = 400 * xCache.level / 100;
            break;
        default:    //%
            watt = 0;
            break;
    }
    return watt;
}

/**
 * @brief 设置控件的坐标轴
 * @param xValue
 * @param h
 * @param w
 */
void value_set(Value_Typedef_t * xValue, u32 h, u32 w)
{
    if (xValue != NULL) {
        xValue->h = h;
        xValue->w = w;
    }
}
