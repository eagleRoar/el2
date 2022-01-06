/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include <rtthread.h>
/* ------------------------- package ----------------------------------------*/
#include "agile_button.h"
#include "drv_flash.h"
/* -------------------------------------------------------------------------------*/
#include "typedefDEF.h"
#include "uuzBTN.h"
#include "uuzDAC.h"
#include "uuzINIT.h"
#include "uuzTEMP.h"
//-----------------------------------------------------
#include "typedefUI.h"
#include "uuzConfig1309.h"
#include "uuzUI.h"
#include "uuzUI_1309.h"

#define DBG_ENABLE
#define DBG_SECTION_NAME "1309 "
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

//短按判断
void ui_short_event(rt_base_t btn_pin)
{
    //通过界面判断入口
    switch (xUI.cID) {
#if 0
    case uuzUI_SAVE_OK:
        save_opt(btn_pin);
        break;
    case uuzUI_FAC_SUC:
        factory_tip_opt(btn_pin);
        break;
    case uuzUI_LOGO:
        logo_opt(btn_pin);
        break;
    case uuzUI_SHOW_CH:
        show_ch_opt(btn_pin);
        break;
    case uuzUI_MENU:
        setup_opt(btn_pin);
        break;
    case uuzUI_LIGHT:
        light_opt(btn_pin);
        break;
    case uuzUI_MENU_CH1:
    case uuzUI_MENU_CH2:
        channel_opt(btn_pin);
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
    case uuzUI_DISPLAY_MODE:
        display_mode_opt(btn_pin);
        break;
    case uuzUI_MENU_TEMP:
        temp_probe_opt(btn_pin);
        break;
    case uuzUI_TEMP_UNIT:
        temp_unit_opt(btn_pin);
        break;
    case uuzUI_TEMP_CAL:
        temp_cal_opt(btn_pin);
        break;
    case uuzUI_SUNRISE_SUNSET:
        sunrise_sunset_opt(btn_pin);
        break;
    case uuzUI_ECM_CONFIG:
        ecm_config_opt(btn_pin);
        break;
    case uuzUI_MENU_LANG:
        language_config_opt(btn_pin);
        break;
    case uuzUI_MENU_INFO:
        system_info_opt(btn_pin);
        break;
    case uuzUI_MENU_FAC:
        factory_reset_opt(btn_pin);
        break;
#endif
    default:
        break;
    }
}

//长按判断，每隔1秒触发1次
void ui_hold_event(rt_base_t btn_pin)
{
    //通过界面判断入口
    switch (xUI.cID) {
#if 0
    case uuzUI_LOGO:
    case uuzUI_MENU_INFO:
        logo_opt(btn_pin);
        break;
    case uuzUI_SHOW_CH:
        show_ch_opt(btn_pin);
        break;
    case uuzUI_MENU:
        setup_opt(btn_pin);
        break;
    case uuzUI_LIGHT:
        light_opt(btn_pin);
        break;
    case uuzUI_OUTPUT_LEVEL:
        output_level_opt(btn_pin);
        break;
    case uuzUI_LIGHT_CYCLE:
        light_cycle_opt(btn_pin);
        break;
    case uuzUI_TEMP_CAL:
        temp_cal_opt(btn_pin);
        break;
    case uuzUI_SUNRISE_SUNSET:
        sunrise_sunset_opt(btn_pin);
        break;
#endif
    default:
        break;
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

    if (type == 0) {
        if (ch == _CHANNEL_1 || ch == _CHANNEL_2) {
            xCache.level = get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(ch));
            xCache.watt_mode = get_data(_D_LT, _L_CH1_DISPLAY_MODE + _CH_LVL(ch));
            xCache.chMode[ch] = get_data(_D_LT, _L_CH1_OUTPUT_MODE + _CH_LVL(ch));
        }
    }
    //计算使用的瓦数
    switch (xCache.watt_mode) {
    case 0:
        watt = 400 * xCache.level / 100;
        break;
    case 1:
        watt = 600 * xCache.level / 100;
        break;
    case 2:
        watt = 750 * xCache.level / 100;
        break;
    case 3:
        watt = 1000 * xCache.level / 100;
        break;
    default:
        watt = 0;
        break;
    }
    return watt;
}

/**
 *
 * @note 根据频道数据获取当前的W数
 * @param ch
 * @param level 当前的灯光强度
 * @param type
 * @return
 */
u16 level_single_get(u16 ch, u16 level, u8 type)
{
    u16 watt = 0;

#if 0
    if (type == 0) {
        if (ch == _CHANNEL_1) {
            xCache.watt_mode = get_data(_TYPE_LIGHT, uuzLT_CH1_DISPLAY_MODE);
        } else if (ch == _CHANNEL_2) {
            xCache.watt_mode = get_data(_TYPE_LIGHT, uuzLT_CH2_DISPLAY_MODE);
        }
    }
#endif
    //计算使用的瓦数
    switch (xCache.watt_mode) {
    case 0:
        watt = 400 * level / 100;
        break;
    case 1:
        watt = 600 * level / 100;
        break;
    case 2:
        watt = 750 * level / 100;
        break;
    case 3:
        watt = 1000 * level / 100;
        break;
    default:
        watt = 0;
        break;
    }
    return watt;
}

/**
 * @note 界面同步线程
 * @param parameter
 */
void ui_thread_entry(void* parameter)
{
    //显示启动LOGO界面
    xUI.cID = uuzUI_NULL;
    //show_ui(uuzUI_LOGO);
    while (1) {
#if 0
        xUI.delay++;
        switch (xUI.cID) {
        case uuzUI_LOGO:
            //主LOGO界面等待5秒切换到显示界面
            if (xUI.delay >= 5 * 2) {
                show_ui(uuzUI_SHOW_CH);
            }
            break;
        case uuzUI_SHOW_CH:
            //每1秒刷新界面数据
            if (xUI.delay % 2) {
                show_ui(xUI.cID);
            }
            break;
        case uuzUI_LIGHT_CYCLE:
        case uuzUI_TEMP_CAL:
        case uuzUI_SUNRISE_SUNSET:
            //光标闪烁
            if (xUI.press == _PRESS_DOWN) {
                show_ui(xUI.cID);
            }
            break;
        case uuzUI_SAVE_OK:
            if (xUI.delay >= 3 * 2) {
                //等待3秒自动消失
                save_opt(_key_enter);
            }
            break;
        case uuzUI_LIGHT:
            if (xUI.delay % 2) {
                show_ui(uuzUI_LIGHT);
            }
            break;
        default:
            break;
        }
#endif
        //LOG_I("UI:[%d]:Delay:[%d]", xUI.cID, xUI.delay);
        rt_thread_mdelay(500);
    }
}

/* -----------------------------SHOW MASK -----------------------------------*/
void show_label(u16 _font, u8 x, u8 y)
{
    if (_font == _UINT_C) {
        //是摄氏度
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //画一个圆圈
        u8g2_DrawEllipse(&u8g2_el2, x, y - 9, 1, 1, U8G2_DRAW_ALL);
        u8g2_DrawStr(&u8g2_el2, x + 2, y, "C");
    } else if (_font == _UINT_F) {
        //是华氏度
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //画一个圆圈
        u8g2_DrawEllipse(&u8g2_el2, x, y - 9, 1, 1, U8G2_DRAW_ALL);
        u8g2_DrawStr(&u8g2_el2, x + 2, y, "F");
    } else if (_font == _UINT_TAB) {
        u8g2_SetFont(&u8g2_el2, u8g2_font_unifont_t_symbols);
        u8g2_DrawGlyph(&u8g2_el2, x, y, _font);
    }
}
#if 0
/* -----------------------------SAVE TIP-----------------------------------*/
void save_disp(void)
{
    if (xUI.cID != uuzUI_SAVE_OK) {
        ui_id_set(uuzUI_SAVE_OK, 1);
        foucs_set(0);

        // 清空OLED屏幕
        u8g2_ClearBuffer(&u8g2_el2);
        //直接显示数据
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //显示方框
        u8g2_DrawRFrame(&u8g2_el2, 10, 24, 108, 24, 2);
        //显示数据
        u8g2_DrawStr(&u8g2_el2, 14, 40, "Saved Success");
        //刷新OLED屏幕数据gg
        u8g2_SendBuffer(&u8g2_el2);
        xUI.delay = 0;
        LOG_I("save ui open");
        //保存相关数据
        save_data();
    }
}

void save_opt(u16 keyId)
{
    if (keyId == _key_enter) {
        show_ui(xUI.pID);
    }
}
/* -----------------------------FACTORY RESET-----------------------------------*/
void factory_tip_disp(void)
{
    if (xUI.cID != uuzUI_FAC_SUC) {
        ui_id_set(uuzUI_FAC_SUC, 1);
        foucs_set(0);

        // 清空OLED屏幕
        u8g2_ClearBuffer(&u8g2_el2);
        //直接显示数据
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //显示方框
        u8g2_DrawRFrame(&u8g2_el2, 10, 10, 108, 44, 2);
        //显示数据
        u8g2_DrawStr(&u8g2_el2, 14, 24, "Factory Reset");
        u8g2_DrawStr(&u8g2_el2, 32, 44, "Success!");
        //刷新OLED屏幕数据gg
        u8g2_SendBuffer(&u8g2_el2);
        xUI.delay = 0;
        LOG_I("factory reset ui open");
    }
}

void factory_tip_opt(u16 keyId)
{
    if (keyId == _key_enter) {
        show_ui(xUI.pID);
    }
}

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
        case uuzUI_MENU:
        setup_disp();
        break;
        case uuzUI_LIGHT:
        light_disp();
        break;
        case uuzUI_MENU_CH1:
        case uuzUI_MENU_CH2:
        break;
        case uuzUI_MENU_TEMP:
        temp_probe_disp();
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
        case uuzUI_DISPLAY_MODE:
        display_mode_disp();
        break;
        case uuzUI_TEMP_UNIT:
        temp_unit_disp();
        break;
        case uuzUI_TEMP_CAL:
        temp_cal_disp();
        break;
        case uuzUI_ECM_CONFIG:
        ecm_config_disp();
        break;
        case uuzUI_SUNRISE_SUNSET:
        sunrise_sunset_disp();
        break;
        case uuzUI_MENU_LANG:
        language_config_disp();
        break;
        case uuzUI_MENU_INFO:
        system_info_disp();
        break;
        case uuzUI_MENU_FAC:
        factory_reset_disp();
        break;
        case uuzUI_SAVE_OK:
        save_disp();
        break;
        case uuzUI_FAC_SUC:
        factory_tip_disp();
        break;
        default:
        break;
    }
}
/* -----------------------------DEVICE LOGO-----------------------------------*/
void logo_disp(void)
{
    if (xUI.cID != uuzUI_LOGO) {
        ui_id_set(uuzUI_LOGO, 1);
        foucs_set(0);
        LOG_I("Logo UI open");
        xUI.delay = 0;
    }

// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
// 设置字体
    u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
//填充数据
    u8g2_DrawStr(&u8g2_el2, 10, 26, "GAVITA-EL2");
// 设置字体
    u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
//填充数据
    u8g2_DrawStr(&u8g2_el2, 20, 50, "@2020 V1.4");
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
}

void logo_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_enter:
        case _key_back:
        show_ui(uuzUI_SHOW_CH);
        break;
        default:
        break;
    }
}
/* -----------------------------DEVICE SHOW-----------------------------------*/
void show_ch_disp(void)
{
    if (xUI.cID != uuzUI_SHOW_CH) {
        ui_id_set(uuzUI_SHOW_CH, 1);
        foucs_set(0);
        LOG_I("Show UI open");
    }
    // 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
    //黑底绿字
    u8g2_SetDrawColor(&u8g2_el2, 1);
    //更新相关数据
    show_value_disp();
    //刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void show_ch_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_up:
        //u8g2_SetDrawColor(&u8g2_el2, 0);
        //show_ui(uuzUI_SHOW_CH);
        break;
        case _key_down:
        //u8g2_SetDrawColor(&u8g2_el2, 1);
        //show_ui(uuzUI_SHOW_CH);
        break;
        case _key_l_enter:
        show_ui(uuzUI_MENU);
        break;
        case _key_light:
        show_ui(uuzUI_LIGHT);
        break;
        default:
        break;
    }
}

void show_value_disp(void)
{
    u16 watt = 0;
    char cmd[16];
    u8 x = 0, y = 14;

    if (xUI.cID == uuzUI_SHOW_CH) {
        // 设置标题
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        u8g2_DrawStr(&u8g2_el2, x + 20, 14, "CH-1");
        u8g2_DrawStr(&u8g2_el2, x + 84, 14, "CH-2");
        //画横线和竖线
        u8g2_DrawLine(&u8g2_el2, 64, 0, 64, 64);
        u8g2_DrawLine(&u8g2_el2, 0, 18, 128, 18);
        //填充数据
        watt = level_get(_CHANNEL_1, 0);
        if (watt != 0) {
            rt_sprintf(cmd, "%02dW", watt);
        } else {
            rt_sprintf(cmd, "%02d%", xCache.level);
        }
        u8g2_DrawStr(&u8g2_el2, x + 10, y + 22, cmd);
        watt = level_get(_CHANNEL_2, 0);
        if (watt != 0) {
            rt_sprintf(cmd, "%02dW", watt);
        } else {
            rt_sprintf(cmd, "%02d%", xCache.level);
        }
        u8g2_DrawStr(&u8g2_el2, x + 75, y + 22, cmd);
        if (xTemperatureCache[0].enable == 1) {
            rt_sprintf(cmd, " %02d.%d", (xTemperatureCache[0].temperature / 10),
                    (xTemperatureCache[0].temperature % 10));
        } else {
            strcpy(cmd, " ----");
        }
        u8g2_DrawStr(&u8g2_el2, x + 1, y + 39, cmd);
        if (xTemperatureCache[1].enable == 1) {
            rt_sprintf(cmd, " %02d.%d", (xTemperatureCache[1].temperature / 10),
                    (xTemperatureCache[1].temperature % 10));
        } else {
            strcpy(cmd, " ----");
        }
        u8g2_DrawStr(&u8g2_el2, x + 65, y + 39, cmd);
        //显示单位
        if (xSysConfig.Light[uuzLT_TEMP_UNITS] == 0x00) {
            show_label(_UINT_C, 50, y + 39);
            show_label(_UINT_C, 114, y + 39);
        } else {
            show_label(_UINT_F, 50, y + 39);
            show_label(_UINT_F, 114, y + 39);
        }
    }
}

/* -----------------------------LIGHT CONTROLLER SHOW-----------------------------------*/
void light_disp(void)
{
    if (xUI.cID != uuzUI_LIGHT) {
        ui_id_set(uuzUI_LIGHT, 2);
        xCache.levelOpt[_CHANNEL_1] = get_data(_TYPE_LIGHT, uuzLT_CH1_OUTPUT_LEVEL);
        xCache.levelOpt[_CHANNEL_2] = get_data(_TYPE_LIGHT, uuzLT_CH2_OUTPUT_LEVEL);
        foucs_set(0);
        LOG_I("light control open");
        xUI.delay = 0;
    }
    // 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
    //更新相关数据
    light_value_disp();
    //刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
}

void light_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_up:
        case _key_l_up:
        if (xCache.levelOpt[xUI.cFoucs] < 115) {
            xCache.levelOpt[xUI.cFoucs] += 5;
        }
        dac_value_get(xUI.cFoucs, xCache.levelOpt[xUI.cFoucs]);
        light_disp();
        break;
        case _key_down:
        case _key_l_down:
        if (xCache.levelOpt[xUI.cFoucs] > 10) {
            xCache.levelOpt[xUI.cFoucs] -= 5;
        }
        dac_value_get(xUI.cFoucs, xCache.levelOpt[xUI.cFoucs]);
        light_disp();
        break;
        case _key_enter:
        //切换焦点
        if (xUI.cFoucs == _CHANNEL_1) {
            xUI.cFoucs = _CHANNEL_2;
        } else {
            xUI.cFoucs = _CHANNEL_1;
        }
        light_disp();
        break;
        case _key_back:
        show_ui(uuzUI_SHOW_CH);
        break;
        default:
        break;
    }
}

void light_value_disp(void)
{
    char cmd[25];
    u8 ucIndex;
    u8 x = 15, y = 14, sY = 38;
    u8 step = 2;
    u8 min = (xUI.cFoucs / step) * step;
    u8 max = (xUI.cFoucs / step) * step + step;

    if (xUI.cID == uuzUI_LIGHT) {
        //显示标题
        u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
        u8g2_DrawStr(&u8g2_el2, 2, 18, "LIGHT LEVEL");
        //显示设置列表
        // 设置字体
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //填充数据
        for (ucIndex = min; ucIndex < max; ucIndex++) {
            //显示全部数据
            rt_sprintf(cmd, "CH%d:%02d%", (ucIndex + 1), xCache.levelOpt[ucIndex]);
            u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd);
            xCache.watt = level_single_get(ucIndex, xCache.levelOpt[ucIndex], 1);
            if (xCache.watt != 0) {
                //填充数据
                rt_sprintf(cmd, "%02dW", xCache.watt);
                u8g2_DrawStr(&u8g2_el2, x + 70, sY + y * ucIndex, cmd);
            } else {
                u8g2_DrawStr(&u8g2_el2, x + 70, sY + y * ucIndex, "   ");
            }
        }
        //显示光标
        show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
    }
}
/* -----------------------------DEVICE SETUP-----------------------------------*/
void setup_disp(void)
{
    if (xUI.cID != uuzUI_MENU) {
        ui_id_set(uuzUI_MENU, 8);
        //如果是下一级界面返回,则直接切换焦点
        if ((xUI.pID > uuzUI_MENU_LOGS) && (xUI.pID <= uuzUI_MENU_FAC)) {
            foucs_set(xUI.pID % 10);
        } else if (xUI.pID == uuzUI_LOGO) {
            foucs_set(uuzUI_MENU_INFO % 10);
        } else {
            foucs_set(0);
        }
        LOG_I("Setup UI open");
    }
    // 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
    //更新相关数据
    setup_value_disp();
    //刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void setup_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
        case _key_up:
        foucs_move(keyId);
        show_ui(uuzUI_MENU);
        break;
        case _key_enter:
        switch (xUI.cFoucs)
        {
            case 0x01U:
            case 0x02U:
            //记录当前的操作的通道数据
            xCache.CH = xUI.cFoucs - 0x01U;
            channel_disp();
            break;
            case 0x03U:
            show_ui(uuzUI_MENU_TEMP);
            break;
            case 0x05:
            show_ui(uuzUI_MENU_LANG);
            break;
            case 0x06:
            show_ui(uuzUI_MENU_INFO);
            break;
            case 0x07:
            show_ui(uuzUI_MENU_FAC);
            break;
            default:
            break;
        }
        break;
        case _key_back:
        show_ui(uuzUI_SHOW_CH);
        break;
        default:
        break;
    }
}

void setup_value_disp(void)
{
    u8 ucIndex = 0;
    u8 x = 15, y = 14, sY = 15;
    u8 step = 4;
    u8 min = (xUI.cFoucs / step) * step;
    u8 max = (xUI.cFoucs / step) * step + step;

    if (xUI.cID == uuzUI_MENU) {
        //显示设置列表
        // 设置字体
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //填充数据
        for (ucIndex = min; ucIndex < max; ucIndex++) {
            u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd_1[ucIndex]);
        }
        //显示光标
        show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
    }
}
/* ----------------------------CHANNEL-1 SETUP-----------------------------------*/
/* ----------------------------CHANNEL-2 SETUP-----------------------------------*/
void channel_disp(void)
{
    if (xUI.cID != (uuzUI_MENU_CH1 + xCache.CH)) {
        ui_id_set((uuzUI_MENU_CH1 + xCache.CH), 7);
        if ((xUI.pID >= uuzUI_OUTPUT_MODE) && (xUI.pID <= uuzUI_DISPLAY_MODE)) {
            foucs_set(xUI.pID % 10);
        } else {
            foucs_set(0);
        }
        LOG_I("Channel-%d UI open", (xCache.CH + 1));
    }
// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
//更新相关数据
    channel_value_disp();
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void channel_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
        case _key_up:
        foucs_move(keyId);
        channel_disp();
        break;
        case _key_enter:
        switch (xUI.cFoucs)
        {
            case 0x00:
            show_ui(uuzUI_OUTPUT_MODE);
            break;
            case 0x01:
            show_ui(uuzUI_OUTPUT_LEVEL);
            break;
            case 0x02:
            show_ui(uuzUI_LIGHT_CYCLE);
            break;
            case 0x04:
            show_ui(uuzUI_SUNRISE_SUNSET);
            break;
            case 0x05:
            show_ui(uuzUI_ECM_CONFIG);
            break;
            case 0x06:
            show_ui(uuzUI_DISPLAY_MODE);
            break;
            default:
            break;
        }
        break;
        case _key_back:
        show_ui(uuzUI_MENU);
        break;
        default:
        break;
    }
}

void channel_value_disp(void)
{
    u8 ucIndex = 0;
    u8 x = 15, y = 14, sY = 32;
    u8 step = 3;
    u8 max = (xUI.cFoucs / step + 1) * step;
    max = (max > xUI.maxFoucs) ? (xUI.maxFoucs) : (max);
    u8 min = (xUI.cFoucs / step) * step;

    if ((xUI.cID == uuzUI_MENU_CH1) || (xUI.cID == uuzUI_MENU_CH2)) {
        //显示标题
        u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
        u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1[xUI.cID % 10]);
        //显示设置列表
        // 设置字体
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //填充数据
        for (ucIndex = min; ucIndex < max; ucIndex++) {
            u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd_1_1[ucIndex]);
        }
        //显示光标
        show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
    }
}
/* -----------------CHANNEL-1/2 SETUP DISPLAY MODE----------------------*/
void display_mode_disp(void)
{
    if (xUI.cID != (uuzUI_DISPLAY_MODE)) {
        ui_id_set(uuzUI_DISPLAY_MODE, 5);
        //获取的CH灯光的显示模式
        if (xCache.CH == _CHANNEL_1) {
            foucs_set(get_data(_TYPE_LIGHT, uuzLT_CH1_DISPLAY_MODE));
        } else if (xCache.CH == _CHANNEL_2) {
            foucs_set(get_data(_TYPE_LIGHT, uuzLT_CH2_DISPLAY_MODE));
        }
        LOG_I("[%d]Output Mode UI open", xCache.CH);
    }
    // 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
    //更新相关数据
    display_mode_value_disp();
    //刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void display_mode_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
        case _key_up:
        foucs_move(keyId);
        show_ui(uuzUI_DISPLAY_MODE);
        break;
        case _key_enter:
        //在工作范围内
        if (xUI.cFoucs >= 0 && xUI.cFoucs < xUI.maxFoucs) {
            //保存相应数据
            if (xCache.CH == _CHANNEL_1) {
                set_data(_TYPE_LIGHT, uuzLT_CH1_DISPLAY_MODE, xUI.cFoucs);
            } else if (xCache.CH == _CHANNEL_2) {
                set_data(_TYPE_LIGHT, uuzLT_CH2_DISPLAY_MODE, xUI.cFoucs);
            }
            show_ui(uuzUI_SAVE_OK);
        }
        break;
        case _key_back:
        channel_disp();
        break;
        default:
        break;
    }
}

void display_mode_value_disp(void)
{

    u8 ucIndex = 0;
    u8 x = 15, y = 14, sY = 32;
    u8 step = 3;
    u8 min = (xUI.cFoucs / step) * step;
    u8 max = (xUI.cFoucs / step + 1) * step;
    max = (max > xUI.maxFoucs) ? (xUI.maxFoucs) : (max);

    if (xUI.cID == uuzUI_DISPLAY_MODE) {
        //显示标题
        u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
        u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1_1[xUI.cID % 10]);
        //显示设置列表
        // 设置字体
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //填充数据
        for (ucIndex = min; ucIndex < max; ucIndex++) {
            u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd_1_1_6[ucIndex]);
        }

        //显示光标
        show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
    }
}
/* -----------------CHANNEL-1/2 SETUP OUTPUT LEVEL----------------------*/
void output_level_disp(void)
{
    if (xUI.cID != (uuzUI_OUTPUT_LEVEL)) {
        ui_id_set(uuzUI_OUTPUT_LEVEL, 1);
        xCache.watt = level_get(xCache.CH, 0);
        foucs_set(xCache.watt_mode);
        LOG_I("Output Level UI open");
    }
    // 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
    //更新相关数据
    output_level_value_disp();
    //刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void output_level_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
        case _key_l_down:
        if (xUI.cFoucs == 0) {
            if (xCache.level > _LIGHT_MIN) {
                xCache.level -= _LIGHT_STEP;
            }
            xCache.watt = level_single_get(xCache.CH, xCache.level, 1);
            show_ui(uuzUI_OUTPUT_LEVEL);
        }
        break;
        case _key_up:
        case _key_l_up:
        if (xUI.cFoucs == 0) {
            if (xCache.level < _LIGHT_MAX) {
                xCache.level += _LIGHT_STEP;
            }
            xCache.watt = level_single_get(xCache.CH, xCache.level, 1);
            show_ui(uuzUI_OUTPUT_LEVEL);
        }
        break;
        case _key_enter:
        if (xUI.cFoucs == 0) {
            if ((xCache.level >= _LIGHT_MIN) && (xCache.level <= _LIGHT_MAX)) {
                if (xCache.CH == _CHANNEL_1) {
                    set_data(_TYPE_LIGHT, uuzLT_CH1_OUTPUT_LEVEL, xCache.level);
                } else if (xCache.CH == _CHANNEL_2) {
                    set_data(_TYPE_LIGHT, uuzLT_CH2_OUTPUT_LEVEL, xCache.level);
                }
                show_ui(uuzUI_SAVE_OK);
            }
        }
        break;
        case _key_back:
        channel_disp();
        break;
        default:
        break;
    }
}

void output_level_value_disp(void)
{
    char cmd[32];
    u8 x = 24, y = 0, sY = 46;

    if (xUI.cID == uuzUI_OUTPUT_LEVEL) {
        //显示标题
        u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
        u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1_1[xUI.cID % 10]);

        //显示设置列表
        // 设置字体
        u8g2_SetFont(&u8g2_el2, u8g2_font_t0_16b_mr);
        if (xCache.level <= _LIGHT_MAX) {
            //填充数据
            rt_sprintf(cmd, "%02d%", xCache.level);
            u8g2_DrawStr(&u8g2_el2, x, sY, cmd);
        } else {
            u8g2_DrawStr(&u8g2_el2, x, sY, "ERROR");
        }

        //显示光标
        show_label(_UINT_TAB, (x - 16), (sY + 2 + y));
        //显示数据标记状态
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        if (xCache.level <= _LIGHT_MIN) {
            u8g2_DrawStr(&u8g2_el2, x + 50, sY, "MIN");
        } else if (xCache.level >= _LIGHT_MAX) {
            u8g2_DrawStr(&u8g2_el2, x + 50, sY, "MAX");
        } else {
            if (xCache.watt != 0) {
                //填充数据
                rt_sprintf(cmd, "%02dW", xCache.watt);
                u8g2_DrawStr(&u8g2_el2, x + 50, sY, cmd);
            } else {
                u8g2_DrawStr(&u8g2_el2, x + 50, sY, "   ");

            }
        }
    }
}
/* -----------------CHANNEL-1/2 SETUP OUTPUT MODE----------------------*/
void output_mode_disp(void)
{
    if (xUI.cID != (uuzUI_OUTPUT_MODE)) {
        ui_id_set(uuzUI_OUTPUT_MODE, 3);
        //获取的CH灯光的显示模式
        if (xCache.CH == _CHANNEL_1) {
            foucs_set(get_data(_TYPE_LIGHT, uuzLT_CH1_OUTPUT_MODE));
        } else if (xCache.CH == _CHANNEL_2) {
            foucs_set(get_data(_TYPE_LIGHT, uuzLT_CH2_OUTPUT_MODE));
        }
        LOG_I("[%d]Output Mode UI open", xCache.CH);
    }
// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
//更新相关数据
    output_mode_value_disp();
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void output_mode_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
        case _key_up:
        foucs_move(keyId);
        show_ui(uuzUI_OUTPUT_MODE);
        break;
        case _key_enter:
        //在工作范围内
        if (xUI.cFoucs >= 0 && xUI.cFoucs < xUI.maxFoucs) {
            //保存相应数据
            if (xCache.CH == _CHANNEL_1) {
                set_data(_TYPE_LIGHT, uuzLT_CH1_OUTPUT_MODE, xUI.cFoucs);
            } else if (xCache.CH == _CHANNEL_2) {
                set_data(_TYPE_LIGHT, uuzLT_CH2_OUTPUT_MODE, xUI.cFoucs);
            }
            show_ui(uuzUI_SAVE_OK);
        }
        break;
        case _key_back:
        channel_disp();
        break;
        default:
        break;
    }
}

void output_mode_value_disp(void)
{

    u8 ucIndex = 0;
    u8 x = 15, y = 14, sY = 32;
    u8 step = 3;
    u8 min = (xUI.cFoucs / step) * step;
    u8 max = (xUI.cFoucs / step + 1) * step;
    max = (max > xUI.maxFoucs) ? (xUI.maxFoucs) : (max);

    if (xUI.cID == uuzUI_OUTPUT_MODE) {
        //显示标题
        u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
        u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1_1[xUI.cID % 10]);
        //显示设置列表
        // 设置字体
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //填充数据
        for (ucIndex = min; ucIndex < max; ucIndex++) {
            if (xCache.CH == _CHANNEL_1) {
                u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd_1_1_1[ucIndex]);
            } else if (xCache.CH == _CHANNEL_2) {
                u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd_1_2_1[ucIndex]);
            }
        }

        //显示光标
        show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
    }
}
/* ---------------------------- LIGHT CYCLE -----------------------------------*/
void light_cycle_disp(void)
{
    if (xUI.cID != (uuzUI_LIGHT_CYCLE)) {
        ui_id_set(uuzUI_LIGHT_CYCLE, 2);
        foucs_set(0);
        if (xCache.CH == _CHANNEL_1) {
            time_get((get_data(_TYPE_LIGHT, uuzLT_CH1_CYCLE_ON)), &xCache.time_ch[0]);    //开启时间设置
            time_get((get_data(_TYPE_LIGHT, uuzLT_CH1_CYCLE_OFF)), &xCache.time_ch[1]);//关闭时间设置
        } else if (xCache.CH == _CHANNEL_2) {
            time_get((get_data(_TYPE_LIGHT, uuzLT_CH2_CYCLE_ON)), &xCache.time_ch[0]);    //开启时间设置
            time_get((get_data(_TYPE_LIGHT, uuzLT_CH2_CYCLE_OFF)), &xCache.time_ch[1]);//关闭时间设置
        }
        LOG_I("Temp probe UI open");
        xUI.pSign = 0;
        xUI.press = _PRESS_UP;
        xUI.delay = 0;
    }
// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
//更新相关数据
    light_cycle_value_disp();
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
}

void light_cycle_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_l_up:
        xUI.press = _PRESS_MODIFY;
        case _key_up:
        if (xUI.press == _PRESS_UP) {
            foucs_move(keyId);
            light_cycle_disp();
        } else {
            if (((xUI.press == _PRESS_MODIFY) && (keyId == _key_l_up)) || (keyId == _key_up)) {
                if (xUI.pSign == 0) {
                    //ON-Hour
                    if (xCache.time_ch[0].h == 23) {
                        xCache.time_ch[0].h = 0;
                    } else {
                        xCache.time_ch[0].h++;
                    }
                } else if (xUI.pSign == 1) {
                    //ON-Min
                    if (xCache.time_ch[0].m == 59) {
                        xCache.time_ch[0].m = 0;
                    } else {
                        xCache.time_ch[0].m++;
                    }
                } else if (xUI.pSign == 2) {
                    //OFF-Hour
                    if (xCache.time_ch[1].h == 23) {
                        xCache.time_ch[1].h = 0;
                    } else {
                        xCache.time_ch[1].h++;
                    }
                } else if (xUI.pSign == 3) {
                    //OFF-Min
                    if (xCache.time_ch[1].m == 59) {
                        xCache.time_ch[1].m = 0;
                    } else {
                        xCache.time_ch[1].m++;
                    }
                }
                light_cycle_disp();
            }
            xUI.press = _PRESS_DOWN;
        }
        break;
        case _key_l_down:
        xUI.press = _PRESS_MODIFY;
        case _key_down:
        if (xUI.press == _PRESS_UP) {
            foucs_move(keyId);
            light_cycle_disp();
        } else {
            if (((xUI.press == _PRESS_MODIFY) && (keyId == _key_l_down)) || (keyId == _key_down)) {
                if (xUI.pSign == 0) {
                    //ON-Hour
                    if (xCache.time_ch[0].h == 0) {
                        xCache.time_ch[0].h = 23;
                    } else {
                        xCache.time_ch[0].h--;
                    }
                } else if (xUI.pSign == 1) {
                    //ON-Min
                    if (xCache.time_ch[0].m == 0) {
                        xCache.time_ch[0].m = 59;
                    } else {
                        xCache.time_ch[0].m--;
                    }
                } else if (xUI.pSign == 2) {
                    //OFF-Hour
                    if (xCache.time_ch[1].h == 0) {
                        xCache.time_ch[1].h = 23;
                    } else {
                        xCache.time_ch[1].h--;
                    }
                } else if (xUI.pSign == 3) {
                    //OFF-Min
                    if (xCache.time_ch[1].m == 0) {
                        xCache.time_ch[1].m = 59;
                    } else {
                        xCache.time_ch[1].m--;
                    }
                }
                light_cycle_disp();
            }
            xUI.press = _PRESS_DOWN;
        }
        break;
        case _key_enter:
        //如果是未按下状态
        if (xUI.press == _PRESS_UP) {
            //进入按下状态
            xUI.press = _PRESS_DOWN;
            if (xUI.cFoucs == 0x00) {
                xUI.pSign = 0;
            } else {
                xUI.pSign = 2;
            }
        } else {
            //在按下状态下再次按下,切换焦点
            xUI.pSign++;
            if (xUI.cFoucs == 0x00 || xUI.cFoucs == 0x01) {
                if (xUI.pSign == 2) {
                    //跳转到下一个设置状态
                    xUI.cFoucs++;
                    light_cycle_disp();
                } else if (xUI.pSign == 4) {
                    //设置完成,退出设置状态,还原焦点
                    xUI.press = _PRESS_UP;
                    xUI.cFoucs--;
                    xUI.pSign = 0;
                    light_cycle_disp();
                    //保存相关数据
                    u16 time;
                    //ON TIME
                    if (xCache.CH == _CHANNEL_1) {
                        time = time_set(&xCache.time_ch[0]);
                        set_data(_TYPE_LIGHT, uuzLT_CH1_CYCLE_ON, time);
                        time = time_set(&xCache.time_ch[1]);
                        set_data(_TYPE_LIGHT, uuzLT_CH1_CYCLE_OFF, time);
                    } else if (xCache.CH == _CHANNEL_2) {
                        time = time_set(&xCache.time_ch[0]);
                        set_data(_TYPE_LIGHT, uuzLT_CH2_CYCLE_ON, time);
                        time = time_set(&xCache.time_ch[1]);
                        set_data(_TYPE_LIGHT, uuzLT_CH2_CYCLE_OFF, time);
                    }
                }
            }
        }
        break;
        case _key_back:
        channel_disp();
        break;
        default:
        break;
    }
}

void light_cycle_value_disp(void)
{
    char cmd[25];
    u8 isAll = 0;
    u8 ucIndex;
    u8 x = 15, y = 14, sY = 38;
    u8 step = 2;
    u8 min = (xUI.cFoucs / step) * step;
    u8 max = (xUI.cFoucs / step) * step + step;

    if (xUI.cID == uuzUI_LIGHT_CYCLE) {
        //显示标题
        u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
        u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1_1[xUI.cID % 10]);
        //显示设置列表
        // 设置字体
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //填充数据
        for (ucIndex = min; ucIndex < max; ucIndex++) {
            isAll = 1;
            if (xUI.press == _PRESS_DOWN) {
                //每500ms显示1次
                if (xUI.delay % 2 == 0) {
                    //显示闪烁数据
                    if (ucIndex == 0) {
                        if (xUI.pSign == 0) {
                            rt_sprintf(cmd, "%s  :%02d", cmd_1_1_3[ucIndex], xCache.time_ch[ucIndex].m);
                            isAll = 0;
                        } else if (xUI.pSign == 1) {
                            rt_sprintf(cmd, "%s%02d:  ", cmd_1_1_3[ucIndex], xCache.time_ch[ucIndex].h);
                            isAll = 0;
                        }
                    } else {
                        if (xUI.pSign == 2) {
                            rt_sprintf(cmd, "%s  :%02d", cmd_1_1_3[ucIndex], xCache.time_ch[ucIndex].m);
                            isAll = 0;
                        } else if (xUI.pSign == 3) {
                            rt_sprintf(cmd, "%s%02d:  ", cmd_1_1_3[ucIndex], xCache.time_ch[ucIndex].h);
                            isAll = 0;
                        }
                    }
                }
            }
            if (isAll == 1) {
                //显示全部数据
                rt_sprintf(cmd, "%s%02d:%02d", cmd_1_1_3[ucIndex], xCache.time_ch[ucIndex].h,
                        xCache.time_ch[ucIndex].m);
            }
            u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd);
        }
        //显示光标
        show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
    }
}
/* ---------------------------- ECM CONFIG-----------------------------------*/
void ecm_config_disp(void)
{
    if (xUI.cID != (uuzUI_ECM_CONFIG)) {
        ui_id_set(uuzUI_ECM_CONFIG, 2);
        if (xCache.CH == _CHANNEL_1) {
            foucs_set(get_data(_TYPE_LIGHT, uuzLT_CH1_ECM_MODE));
        } else if (xCache.CH == _CHANNEL_2) {
            foucs_set(get_data(_TYPE_LIGHT, uuzLT_CH2_ECM_MODE));
        }
        LOG_I("ecm config UI open");
    }
// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
//更新相关数据
    u8 ucIndex;
    u8 x = 15, y = 14, sY = 38;
    u8 step = 2;
    u8 max = (xUI.cFoucs / step + 1) * step;
    u8 min = (xUI.cFoucs / step) * step;

//显示标题
    u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
    u8g2_DrawStr(&u8g2_el2, 2, 18, "ECM ON DURING");
//显示设置列表
// 设置字体
    u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
//填充数据
    for (ucIndex = min; ucIndex < max; ucIndex++) {
        u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd_1_1_5[ucIndex]);
    }
//显示光标
    show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void ecm_config_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
        case _key_up:
        foucs_move(keyId);
        show_ui(uuzUI_ECM_CONFIG);
        break;
        case _key_enter:
        if (xUI.cFoucs < xUI.maxFoucs) {
            if (xCache.CH == _CHANNEL_1) {
                set_data(_TYPE_LIGHT, uuzLT_CH1_ECM_MODE, xUI.cFoucs);
            } else if (xCache.CH == _CHANNEL_2) {
                set_data(_TYPE_LIGHT, uuzLT_CH1_ECM_MODE, xUI.cFoucs);
            }
            //保存相关数据
            save_disp();
        }
        break;
        case _key_back:
        channel_disp();
        break;
        default:
        break;
    }
}
/* -------------------------- SUNRISE/SUNSET---------------------------------*/
void sunrise_sunset_disp(void)
{
    if (xUI.cID != (uuzUI_SUNRISE_SUNSET)) {
        ui_id_set(uuzUI_SUNRISE_SUNSET, 2);
        if (xCache.CH == _CHANNEL_1) {
            xCache.usRamp[0] = get_data(_TYPE_LIGHT, uuzLT_CH1_SUNRISE_DELAY);
            xCache.usRamp[1] = get_data(_TYPE_LIGHT, uuzLT_CH1_SUNSET_DELAY);
        } else if (xCache.CH == _CHANNEL_2) {
            xCache.usRamp[0] = get_data(_TYPE_LIGHT, uuzLT_CH2_SUNRISE_DELAY);
            xCache.usRamp[1] = get_data(_TYPE_LIGHT, uuzLT_CH2_SUNSET_DELAY);
        }
        foucs_set(0);
        LOG_I("sun-rise-set UI open");
        xUI.delay = 0;
    }
// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
//显示相关数据
    sunrise_sunset_value_disp();
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
}

void sunrise_sunset_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_l_up:
        xUI.press = _PRESS_MODIFY;
        case _key_up:
        if (xUI.press == _PRESS_UP) {
            foucs_move(keyId);
            show_ui(uuzUI_SUNRISE_SUNSET);
        } else {
            if (((xUI.press == _PRESS_MODIFY) && (keyId == _key_l_up)) || (keyId == _key_up)) {
                if (xUI.pSign == 0) {
                    if (xCache.usRamp[0] == 30) {
                        xCache.usRamp[0] = 0;
                    } else {
                        xCache.usRamp[0]++;    //1min
                    }
                    show_ui(uuzUI_SUNRISE_SUNSET);
                } else if (xUI.pSign == 1) {
                    if (xCache.usRamp[1] == 30) {
                        xCache.usRamp[1] = 0;
                    } else {
                        xCache.usRamp[1]++;    //1min
                    }
                    show_ui(uuzUI_SUNRISE_SUNSET);
                }
                xUI.press = _PRESS_DOWN;
            }
        }
        break;
        case _key_l_down:
        xUI.press = _PRESS_MODIFY;
        case _key_down:
        if (xUI.press == _PRESS_UP) {
            foucs_move(keyId);
            show_ui(uuzUI_SUNRISE_SUNSET);
        } else {
            if (((xUI.press == _PRESS_MODIFY) && (keyId == _key_l_down)) || (keyId == _key_down)) {
                if (xUI.pSign == 0) {
                    if (xCache.usRamp[0] == 0) {
                        xCache.usRamp[0] = 30;
                    } else {
                        xCache.usRamp[0]--;    //1min
                    }
                    show_ui(uuzUI_SUNRISE_SUNSET);
                } else if (xUI.pSign == 1) {
                    if (xCache.usRamp[1] == 0) {
                        xCache.usRamp[1] = 30;
                    } else {
                        xCache.usRamp[1]--;    //1min
                    }
                    show_ui(uuzUI_SUNRISE_SUNSET);
                }
                xUI.press = _PRESS_DOWN;
            }
        }
        break;
        case _key_enter:
        //如果是未按下状态
        if (xUI.press == _PRESS_UP) {
            //进入按下状态
            xUI.press = _PRESS_DOWN;
            xUI.pSign = xUI.cFoucs;
        } else {
            //在按下状态下再次按下,切换焦点
            xUI.press = _PRESS_UP;
            if (xCache.CH == _CHANNEL_1) {
                set_data(_TYPE_LIGHT, uuzLT_CH1_SUNRISE_DELAY, xCache.usRamp[0]);
                set_data(_TYPE_LIGHT, uuzLT_CH1_SUNSET_DELAY, xCache.usRamp[1]);
            } else {
                set_data(_TYPE_LIGHT, uuzLT_CH2_SUNRISE_DELAY, xCache.usRamp[0]);
                set_data(_TYPE_LIGHT, uuzLT_CH2_SUNSET_DELAY, xCache.usRamp[1]);
            }
            //TODO: Save Data
            save_disp();
        }
        break;
        case _key_back:
        channel_disp();
        break;
        default:
        break;
    }
}

void sunrise_sunset_value_disp(void)
{
    char cmd[25];
    u8 isAll = 0;
    u8 ucIndex;
    u8 x = 15, y = 14, sY = 38;
    u8 step = 2;
    u8 max = (xUI.cFoucs / step + 1) * step;
    u8 min = (xUI.cFoucs / step) * step;

    if (xUI.cID == uuzUI_SUNRISE_SUNSET) {
        //显示标题
        u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
        u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1_1[xUI.cID % 10]);
        //显示设置列表
        // 设置字体
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //填充数据
        for (ucIndex = min; ucIndex < max; ucIndex++) {
            isAll = 1;
            if (xUI.press == _PRESS_DOWN) {
                //每500ms显示1次
                if (xUI.delay % 2 == 0) {
                    //显示闪烁数据
                    if (((ucIndex == 0) && (xUI.pSign == 0)) || ((ucIndex == 1) && (xUI.pSign == 1))) {
                        rt_sprintf(cmd, "%s    ", cmd_1_1_4[ucIndex]);
                        isAll = 0;
                    }
                }
            }
            if (isAll == 1) {
                //显示全部数据
                if (ucIndex == 0) {
                    if (xCache.usRamp[0] == 0x00) {
                        rt_sprintf(cmd, "%sNONE", cmd_1_1_4[0]);
                    } else {
                        rt_sprintf(cmd, "%s  %02d", cmd_1_1_4[0], xCache.usRamp[0]);
                    }
                } else {
                    if (xCache.usRamp[1] == 0x00) {
                        rt_sprintf(cmd, "%sNONE", cmd_1_1_4[1]);
                    } else {
                        rt_sprintf(cmd, "%s  %02d", cmd_1_1_4[1], xCache.usRamp[1]);
                    }
                }
            }
            u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd);
        }
        //显示光标
        show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
    }
}
/* ---------------------------- TEMPERATRUE PROBE -----------------------------------*/
void temp_probe_disp(void)
{
    if (xUI.cID != (uuzUI_MENU_TEMP)) {
        ui_id_set(uuzUI_MENU_TEMP, 2);
        if ((xUI.pID >= uuzUI_TEMP_UNIT) && (xUI.pID >= uuzUI_TEMP_CAL)) {
            foucs_set(xUI.pID % 10);
        } else {
            foucs_set(0);
        }
        LOG_I("Temp probe UI open");
    }
    // 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
    //更新相关数据
    temp_probe_value_disp();
    //刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void temp_probe_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
        case _key_up:
        foucs_move(keyId);
        show_ui(uuzUI_MENU_TEMP);
        break;
        case _key_enter:
        switch (xUI.cFoucs)
        {
            case 0x00:
            show_ui(uuzUI_TEMP_UNIT);
            break;
            case 0x01:
            show_ui(uuzUI_TEMP_CAL);
            break;
            default:
            break;
        }
        break;
        case _key_back:
        show_ui(uuzUI_MENU);
        break;
        default:
        break;
    }
}

void temp_probe_value_disp(void)
{

    u8 ucIndex;
    u8 x = 15, y = 14, sY = 38;
    u8 step = 2;
    u8 min = (xUI.cFoucs / step) * step;
    u8 max = (xUI.cFoucs / step) * step + step;

    if (xUI.cID == uuzUI_MENU_TEMP) {
        //显示标题
        u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
        u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1[xUI.cID % 10]);
        //显示设置列表
        // 设置字体
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //填充数据
        for (ucIndex = min; ucIndex < max; ucIndex++) {
            u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd_1_3[ucIndex]);
        }
        //显示光标
        show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
    }
}
/* ---------------------------- TEMPERATRUE UNIT SELECT -----------------------------------*/
void temp_unit_disp(void)
{
    if (xUI.cID != (uuzUI_TEMP_UNIT)) {
        ui_id_set(uuzUI_TEMP_UNIT, 2);
        foucs_set(get_data(_TYPE_LIGHT, uuzLT_TEMP_UNITS));
        LOG_I("Temp unit UI open");
    }
// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
//更新相关数据
    u8 x = 15, y = 14, sY = 38;
    u8 step = 2;

//显示标题
    u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
    u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1_3[xUI.cID % 10]);
//显示设置列表
// 设置字体
    u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
//填充数据
//C-List1
    show_label(_UINT_C, x, sY);
//F-List2
    show_label(_UINT_F, x, (sY + y));
//显示光标
    show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void temp_unit_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
        case _key_up:
        foucs_move(keyId);
        temp_unit_disp();
        break;
        case _key_enter:
        if (xUI.cFoucs < xUI.maxFoucs) {
            //如果数据发生变化
            if (xUI.cFoucs != get_data(_TYPE_LIGHT, uuzLT_TEMP_UNITS)) {
                set_data(_TYPE_LIGHT, uuzLT_TEMP_UNITS, xUI.cFoucs);
                //显示保存提示界面
                save_disp();
            }
        }
        break;
        case _key_back:
        show_ui(uuzUI_MENU_TEMP);
        break;
        default:
        break;
    }
}
/* ---------------------------- TEMP CALIBRATE -----------------------------------*/
void temp_cal_disp(void)
{
    if (xUI.cID != (uuzUI_TEMP_CAL)) {
        ui_id_set(uuzUI_TEMP_CAL, 2);
        foucs_set(0);
        xCache.cal[0].cal = get_data(_TYPE_LIGHT, uuzLT_CAL_TEMP1);
        xCache.cal[0].adv = get_data(_TYPE_LIGHT, uuzLT_CAL_VAL1);
        xCache.cal[1].cal = get_data(_TYPE_LIGHT, uuzLT_CAL_TEMP2);
        xCache.cal[1].adv = get_data(_TYPE_LIGHT, uuzLT_CAL_VAL2);
        LOG_I("Temp calibrate UI open");
        xUI.delay = 0;
    }
// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
//更新相关数据
    temp_cal_value_disp();
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
}

void temp_cal_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_l_up:
        xUI.press = _PRESS_MODIFY;
        case _key_up:
        if (xUI.press == _PRESS_UP) {
            foucs_move(keyId);
            temp_cal_disp();
        } else {
            if (((xUI.press == _PRESS_MODIFY) && (keyId == _key_l_up)) || (keyId == _key_up)) {
                if (xUI.pSign == 0) {
                    //对应传感器T1
                    if (xSysConfig.Light[uuzLT_TEMP_UNITS] == 0x00) {
                        //摄氏度
                        if (xCache.cal[0].cal == 500) {
                            xCache.cal[0].cal = 0;
                        } else {
                            xCache.cal[0].cal++;    //every is 0.1C or 0.2F
                        }
                    } else {
                        //华氏度
                        if (xSysConfig.Light[uuzLT_TEMP_UNITS] == 0x01) {
                            if (xCache.cal[0].cal == 1220) {
                                xCache.cal[0].cal = 32;
                            } else {
                                xCache.cal[0].cal += 2;    //every is 0.1C or 0.2F
                            }
                        }
                    }
                    temp_cal_disp();
                } else if (xUI.pSign == 1) {
                    //对应传感器T2
                    if (xSysConfig.Light[uuzLT_TEMP_UNITS] == 0x00) {
                        //摄氏度
                        if (xCache.cal[1].cal == 500) {
                            xCache.cal[1].cal = 0;
                        } else {
                            xCache.cal[1].cal++;    //every is 0.1C or 0.2F
                        }
                    } else {
                        //华氏度
                        if (xSysConfig.Light[uuzLT_TEMP_UNITS] == 0x01) {
                            if (xCache.cal[1].cal == 1220) {
                                xCache.cal[1].cal = 32;
                            } else {
                                xCache.cal[1].cal += 2;    //every is 0.1C or 0.2F
                            }
                        }
                    }
                    temp_cal_disp();
                }
                xUI.press = _PRESS_DOWN;
            }
        }
        break;
        case _key_l_down:
        xUI.press = _PRESS_MODIFY;
        case _key_down:
        if (xUI.press == _PRESS_UP) {
            foucs_move(keyId);
            temp_cal_disp();
        } else {
            if (((xUI.press == _PRESS_MODIFY) && (keyId == _key_l_down)) || (keyId == _key_down)) {
                if (xUI.pSign == 0) {
                    //对应传感器T1
                    if (xSysConfig.Light[uuzLT_TEMP_UNITS] == 0x00) {
                        //摄氏度
                        if (xCache.cal[0].cal == 0) {
                            xCache.cal[0].cal = 500;
                        } else {
                            xCache.cal[0].cal--;    //every is 0.1C or 0.2F
                        }
                    } else {
                        //华氏度
                        if (xSysConfig.Light[uuzLT_TEMP_UNITS] == 0x01) {
                            if (xCache.cal[0].cal == 320) {
                                xCache.cal[0].cal = 1220;
                            } else {
                                xCache.cal[0].cal -= 2;    //every is 0.1C or 0.2F
                            }
                        }
                    }
                    temp_cal_disp();
                } else if (xUI.pSign == 1) {
                    //对应传感器T2
                    if (xSysConfig.Light[uuzLT_TEMP_UNITS] == 0x00) {
                        //摄氏度
                        if (xCache.cal[0].cal == 0) {
                            xCache.cal[0].cal = 500;
                        } else {
                            xCache.cal[0].cal--;    //every is 0.1C or 0.2F
                        }
                    } else {
                        //华氏度
                        if (xSysConfig.Light[uuzLT_TEMP_UNITS] == 0x01) {
                            if (xCache.cal[1].cal == 320) {
                                xCache.cal[1].cal = 1220;
                            } else {
                                xCache.cal[1].cal -= 2;    //every is 0.1C or 0.2F
                            }
                        }
                    }
                    temp_cal_disp();
                }
                xUI.press = _PRESS_DOWN;
            }
        }
        break;
        case _key_enter:
        //如果是未按下状态
        if (xUI.press == _PRESS_UP) {
            //进入按下状态
            xUI.press = _PRESS_DOWN;
            xUI.pSign = xUI.cFoucs;
        } else {
            //在按下状态下再次按下,切换焦点
            xUI.press = _PRESS_UP;
            set_data(_TYPE_LIGHT, uuzLT_CAL_TEMP1, xCache.cal[0].cal);
            set_data(_TYPE_LIGHT, uuzLT_CAL_TEMP2, xCache.cal[1].cal);
            set_data(_TYPE_LIGHT, uuzLT_CAL_VAL1, xCache.cal[0].adv);
            set_data(_TYPE_LIGHT, uuzLT_CAL_VAL2, xCache.cal[1].adv);
            //TODO: Save Data
            save_disp();
        }
        break;
        case _key_back:
        show_ui(uuzUI_MENU_TEMP);
        break;
        default:
        break;
    }
}

void temp_cal_value_disp(void)
{
    char cmd[25];
    u8 isAll = 0;
    u8 ucIndex;
    u8 x = 15, y = 14, sY = 38;
    u8 step = 2;
    u8 min = (xUI.cFoucs / step) * step;
    u8 max = (xUI.cFoucs / step) * step + step;

    if (xUI.cID == uuzUI_TEMP_CAL) {
        //显示标题
        u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
        u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1_3[xUI.cID % 10]);
        //显示设置列表
        // 设置字体
        u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
        //填充数据
        for (ucIndex = min; ucIndex < max; ucIndex++) {
            if (xTemperatureCache[ucIndex].enable == 1) {
                isAll = 1;
                if (xUI.press == _PRESS_DOWN) {
                    //每500ms显示1次
                    if (xUI.delay % 2 == 0) {
                        //显示闪烁数据
                        if (ucIndex == 0) {
                            if (xUI.pSign == 0) {
                                rt_sprintf(cmd, "T%d:          ", (ucIndex + 1));
                                isAll = 0;
                            }
                        } else {
                            if (xUI.pSign == 1) {
                                rt_sprintf(cmd, "T%d:          ", (ucIndex + 1));
                                isAll = 0;
                            }
                        }
                    }
                }
                if (isAll == 1) {
                    //显示全部数据
                    rt_sprintf(cmd, "T%d:      %2d.%d", (ucIndex + 1), xCache.cal[ucIndex].cal / 10,
                            xCache.cal[ucIndex].cal % 10);
                }
                u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd);
                //显示单位
                if (xSysConfig.Light[uuzLT_TEMP_UNITS] == 0x00) {
                    show_label(_UINT_C, 114, (sY + y * ((ucIndex - min) % step)));
                } else {
                    show_label(_UINT_F, 114, (sY + y * ((ucIndex - min) % step)));
                }
            } else {
                rt_sprintf(cmd, "T%d: REMOVED", (ucIndex + 1));
                u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd);
            }
        }
        //显示光标
        show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
    }
}
/* ---------------------------- LANGUAGE-----------------------------------*/
void language_config_disp(void)
{
    if (xUI.cID != (uuzUI_MENU_LANG)) {
        ui_id_set(uuzUI_MENU_LANG, 4);
        foucs_set(get_data(_TYPE_SYSTEM, uuzMASTER_LANGUAGE));
        LOG_I("language config UI open");
    }
// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
//更新相关数据
    u8 ucIndex;
    u8 x = 15, y = 14, sY = 38;
    u8 step = 2;
    u8 max = (xUI.cFoucs / step + 1) * step;
    u8 min = (xUI.cFoucs / step) * step;

//显示标题
    u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
    u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1[xUI.cID % 10]);
//显示设置列表
// 设置字体
    u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
//填充数据
    for (ucIndex = min; ucIndex < max; ucIndex++) {
        u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd_1_5[ucIndex]);
    }
//显示光标
    show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void language_config_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
        case _key_up:
        foucs_move(keyId);
        show_ui(uuzUI_MENU_LANG);
        break;
        case _key_enter:
        if (xUI.cFoucs < xUI.maxFoucs) {
            set_data(_TYPE_SYSTEM, uuzMASTER_LANGUAGE, xUI.cFoucs);
            //保存相关数据
            save_disp();
        }
        break;
        case _key_back:
        show_ui(uuzUI_MENU);
        break;
        default:
        break;
    }
}
/* ---------------------------- SYSTEM INFO-----------------------------------*/
//和启动的界面相同
void system_info_disp(void)
{
    if (xUI.cID != uuzUI_MENU_INFO) {
        ui_id_set(uuzUI_MENU_INFO, 1);
        foucs_set(0);
        LOG_I("System Info open");
        xUI.delay = 0;
    }

// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
// 设置字体
    u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
//填充数据
    u8g2_DrawStr(&u8g2_el2, 10, 26, "GAVITA-EL2");
// 设置字体
    u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
//填充数据
    u8g2_DrawStr(&u8g2_el2, 20, 50, "@2020 v1.4");
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
}

void system_info_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_light:
        break;
        default:
        show_ui(xUI.pID);
        break;
    }
}
/* ----------------------------FACTORY RESET-----------------------------------*/
void factory_reset_disp(void)
{
    if (xUI.cID != (uuzUI_MENU_FAC)) {
        ui_id_set(uuzUI_MENU_FAC, 2);
        foucs_set(0);
        LOG_I("factory reset UI open");
    }
// 清空OLED屏幕
    u8g2_ClearBuffer(&u8g2_el2);
//更新相关数据
    u8 ucIndex;
    u8 x = 15, y = 14, sY = 38;
    u8 step = 2;
    u8 max = (xUI.cFoucs / step + 1) * step;
    u8 min = (xUI.cFoucs / step) * step;

//显示标题
    u8g2_SetFont(&u8g2_el2, _FONT_TITLE_DEFAULT);
    u8g2_DrawStr(&u8g2_el2, 2, 18, cmd_1[xUI.cID % 10]);
//显示设置列表
// 设置字体
    u8g2_SetFont(&u8g2_el2, _FONT_DEFAULT);
//填充数据
    for (ucIndex = min; ucIndex < max; ucIndex++) {
        u8g2_DrawStr(&u8g2_el2, x, (sY + y * ((ucIndex - min) % step)), cmd_1_7[ucIndex]);
    }
//显示光标
    show_label(_UINT_TAB, (x - 14), (sY + 2 + y * (xUI.cFoucs % step)));
//刷新OLED屏幕数据
    u8g2_SendBuffer(&u8g2_el2);
    xUI.delay = 0;
}

void factory_reset_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
        case _key_up:
        foucs_move(keyId);
        show_ui(uuzUI_MENU_FAC);
        break;
        case _key_enter:
        if (xUI.cFoucs == 0) {
            /* 初始化设备配置数据 */
            uuz_vDeviceConfigInit(1);
            /* 更新设备缓存数据 */
            uuz_vDeviceCacheDefaultInit();
            show_ui(uuzUI_FAC_SUC);
        } else {
            show_ui(uuzUI_MENU);
        }
        break;
        case _key_back:
        show_ui(uuzUI_MENU);
        break;
        default:
        break;
    }
}
#endif
/* ---------------------------------------------------------------------------*/
