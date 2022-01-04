/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include "string.h"
/* ------------------------- package ----------------------------------------*/
#include "drv_flash.h"
/* -------------------------------------------------------------------------------*/
#include "uuzINIT.h"
#include "typedefDEF.h"
/* -------------------------------------------------------------------------------*/
#include "uuzBTN.h"
#include "uuzRTC.h"
#include "uuzOpt.h"
#include "uuzTEMP.h"
/* -------------------------------------------------------------------------------*/
#include "uuzUI.h"
#include "typedefUI.h"
#include "uuzOLED_1602.h"
#include "uuzUI_1602.h"
#include "uuzConfig1602.h"
#include "uuzUI_Cache.h"
/* -------------------------------------------------------------------------------*/
#define DBG_ENABLE
#define DBG_SECTION_NAME "UI|M"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
/* -----------------------------MENU LIST -----------------------------------*/
void menu_disp(void)
{
    if (xUI.cID != uuzUI_MENU) {
        ui_id_set(uuzUI_MENU, MENU_LIST_MAX);
        //如果是下一级界面返回,则直接切换焦点
        if ((xUI.pID > uuzUI_MENU_LOGS) && (xUI.pID <= uuzUI_MENU_FAC)) {
            foucs_set(xUI.pID % 10);
        } else if ((xUI.pID == uuzUI_TEMP_SEL) || (xUI.pID == uuzUI_TEMP_CAL)) {
            foucs_set(uuzUI_MENU_TEMP % 10);
        } else {
            foucs_set(0);
        }
        LOG_I("Menu List UI open");
        //设置光标位置
        value_set(&xCursor, 0, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //更新相关数据
        menu_list_disp();
        //清除延时计数
        xUI.delay = 0;
        //开始光标闪烁
        xUI.blink = 1;
    } else {
        //光标闪烁
        if (xUI.blink == 1) {
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void menu_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_up:
            if (xUI.cFoucs > 0) {
                //关闭闪烁
                xUI.blink = 0;
                //清除焦点
                drawNull(_OLED_, xCursor.h, xCursor.w, 1);
                //移动光标
                foucs_move(keyId);
                //刷新界面
                menu_list_disp();
                //开启闪烁
                xUI.blink = 1;
            }
            break;
        case _key_down:
            if (xUI.cFoucs < (xUI.maxFoucs - 1)) {
                //关闭闪烁
                xUI.blink = 0;
                //清除焦点
                drawNull(_OLED_, xCursor.h, xCursor.w, 1);
                //移动光标
                foucs_move(keyId);
                //刷新界面
                menu_list_disp();
                //开启闪烁
                xUI.blink = 1;
            }
            break;
        case _key_enter:
            switch (xUI.cFoucs)
            {
                case 0:    //LOGS
                    show_ui(uuzUI_MENU_LOGS);
                    break;
                case 1:    //CHANNEL 1
                case 2:    //CHANNEL 2
                    xCache.channel = xUI.cFoucs - 1;
                    show_ui(uuzUI_MENU_CH1 + xCache.channel);
                    break;
                case 3:    //TEMP PROBE
                    show_ui(uuzUI_MENU_TEMP);
                    break;
                case 4:    //CH2 MODE
                    show_ui(uuzUI_MENU_MODE);
                    break;
                case 5:    //SYSTEM TIME
                    show_ui(uuzUI_MENU_TIME);
                    break;
                case 6:    //LANGUAGE
                    show_ui(uuzUI_MENU_LANG);
                    break;
                case 7:    //INFO
                    show_ui(uuzUI_MENU_INFO);
                    break;
                case 8:    //FACTORY RESET
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

void menu_list_disp(void)
{
    u8 line1 = 0;
    u8 line2 = 1;

    if (xUI.cID == uuzUI_MENU) {
        //显示列表菜单
        if (xUI.cFoucs < xUI.maxFoucs) {
            if (xUI.cFoucs == 0) {
                line1 = 0;
                line2 = 1;
                value_set(&xCursor, 0, 0);
            } else {
                line1 = xUI.cFoucs - 1;
                line2 = xUI.cFoucs;
                value_set(&xCursor, 1, 0);
            }

            //立即显示数据
            drawToOLED(0, 1, cmd_1[line1], 13);
            drawToOLED(1, 1, cmd_1[line2], 13);
        }
    }
}
/* -----------------LANGUAGE MODE----------------------*/
void lang_disp(void)
{
    if (xUI.cID != (uuzUI_MENU_LANG)) {
        ui_id_set(uuzUI_MENU_LANG, 5);
        //获取的语言的配置显示模式
        foucs_set(get_data(_D_SYS, _D_LANGUAGE));
        LOG_I("Language UI open");
        value_set(&xCursor, 1, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawToOLED(0, 0, cmd_1[xUI.cID % 10], 13);
        //显示系统的语言选择
        drawToOLED(1, 1, cmd_1_6[xUI.cFoucs], 10);
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        if (xUI.blink == 1) {
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void lang_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_down:
            xUI.blink = 0;
            drawNull(_OLED_, xCursor.h, xCursor.w, 1);
            if (xUI.cFoucs == (xUI.maxFoucs - 1)) {
                xUI.cFoucs = 0;
            } else {
                foucs_move(keyId);
            }
            //显示系统的语言选择
            drawToOLED(1, 1, cmd_1_6[xUI.cFoucs], 10);
            xUI.blink = 1;
            break;
        case _key_up:
            xUI.blink = 0;
            drawNull(_OLED_, xCursor.h, xCursor.w, 1);
            if (xUI.cFoucs == 0) {
                xUI.cFoucs = (xUI.maxFoucs - 1);
            } else {
                foucs_move(keyId);
            }
            //显示系统的语言选择
            drawToOLED(1, 1, cmd_1_6[xUI.cFoucs], 10);
            xUI.blink = 1;
            break;
        case _key_enter:
            //在工作范围内
            if (xUI.cFoucs < xUI.maxFoucs) {
                //保存相应数据
                set_data(_D_SYS, _D_LANGUAGE, xUI.cFoucs);
                //保存相关数据
                save_data();
                LOG_I("save ui open");
                //退到前一个界面
                show_ui(xUI.cID / 10);
            }
            break;
        case _key_back:
            show_ui(xUI.cID / 10);
            break;
        default:
            break;
    }
}
/* -----------------------------FACTORY RESET-----------------------------------*/
void factory_reset_disp(void)
{
    if (xUI.cID != uuzUI_MENU_FAC) {
        ui_id_set(uuzUI_MENU_FAC, 2);
        foucs_set(0);
        LOG_I("Device Info UI open");
        value_set(&xCursor, 1, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawToOLED(0, 0, cmd_1[xUI.cID % 10], 13);
        //显示YES/NO
        drawToOLED(1, 1, cmd_y_n[xUI.cFoucs], 3);
        //光标闪烁
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        //光标闪烁
        if (xUI.blink == 1) {
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void factory_reset_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_up:
            //刷新标记
            xUI.blink = 0;
            //移动焦点
            if (xUI.cFoucs == 0) {
                foucs_set(xUI.maxFoucs - 1);
            } else {
                foucs_move(keyId);
            }
            //显示界面
            //显示YES/NO
            drawToOLED(1, 1, cmd_y_n[xUI.cFoucs], 3);
            xUI.blink = 1;
            break;
        case _key_down:
            xUI.blink = 0;
            //移动焦点
            if (xUI.cFoucs == (xUI.maxFoucs - 1)) {
                foucs_set(0);
            } else {
                foucs_move(keyId);
            }
            //显示YES/NO
            drawToOLED(1, 1, cmd_y_n[xUI.cFoucs], 3);
            xUI.blink = 1;
            break;
        case _key_enter:
            if (xUI.cFoucs == 1) {
                //YES
                /* 更新设备缓存数据 */
                uuz_vDeviceCacheDefaultInit();
                /* 清除温度探头的数据 */
                uuz_vTempData_Init();
                /* 更新时间数据的数据 */
                set_rtc(2020, 1, 1, 8, 0, 0);
                /* 初始化设备配置数据 */
                uuz_vDeviceConfigInit(1);
            }
            //YES or NO
            show_ui(uuzUI_MENU);
            break;
        case _key_back:
            show_ui(xUI.cID / 10);
            break;
        default:
            break;
    }
}
/* -----------------------------LOGS SHOW-----------------------------------*/
void logs_disp(void)
{
    if (xUI.cID != uuzUI_MENU_LOGS) {
        ui_id_set(uuzUI_MENU_LOGS, LOGS_LIST_MAX);
        foucs_set(0);
        LOG_I("Logs UI open");
        //显示数据
        logs_info_disp();
        //清除延时
        xUI.delay = 0;
        //关闭闪烁
        xUI.blink = 0;
    }
}

void logs_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_up:
            //移动焦点
            if (xUI.cFoucs == 0) {
                foucs_set(xUI.maxFoucs - 1);
            } else {
                foucs_move(keyId);
            }
            //显示界面数据
            logs_info_disp();
            break;
        case _key_down:
            //移动焦点
            if (xUI.cFoucs == (xUI.maxFoucs - 1)) {
                foucs_set(0);
            } else {
                foucs_move(keyId);
            }
            //显示界面数据
            logs_info_disp();
            break;
        case _key_enter:
            case _key_back:
            show_ui(xUI.cID / 10);
            break;
        default:
            break;
    }
}

void logs_info_disp(void)
{
    char cmd[OLED_W] =
            { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 };
    char cmd_t[OLED_W];
    u16 usHigh = 0;
    u16 usLow = 0;

    if (xUI.cID == uuzUI_MENU_LOGS) {
        if (xUI.cFoucs < xUI.maxFoucs) {
            //标题
            drawToOLED(0, 0, cmd_1_0[xUI.cFoucs], OLED_W);
            //显示相关数据
            if (xUI.cFoucs < _G_POWER_DOWN) {
                if (xLogs[xUI.cFoucs].en == 0) {
                    //没有有效数据
                    drawToOLED(1, 0, uuzDATA_NA, OLED_W);
                } else {
                    if (xSysConfig.Sys[_D_DATE_FORMAT] == _TYPE_DDMMYY) {
                        rt_sprintf(cmd, "%02d/%02d/%02d  %02d:%02d", xLogs[xUI.cFoucs].date[2],
                                xLogs[xUI.cFoucs].date[1], (xLogs[xUI.cFoucs].date[0] % 100), xLogs[xUI.cFoucs].time[0],
                                xLogs[xUI.cFoucs].time[1]);
                    } else {
                        rt_sprintf(cmd, "%02d/%02d/%02d  %02d:%02d", xLogs[xUI.cFoucs].date[1],
                                xLogs[xUI.cFoucs].date[2], (xLogs[xUI.cFoucs].date[0] % 100), xLogs[xUI.cFoucs].time[0],
                                xLogs[xUI.cFoucs].time[1]);
                    }
                    drawToOLED(1, 0, cmd, OLED_W);
                }
            } else if (xUI.cFoucs == _G_POWER_DOWN) {
                if (xSysConfig.Sys[_D_DATE_FORMAT] == _TYPE_DDMMYY) {
                    rt_sprintf(cmd, "%02d/%02d/%02d  %02d:%02d", xLogs[xUI.cFoucs].date[2], xLogs[xUI.cFoucs].date[1],
                            (xLogs[xUI.cFoucs].date[0] % 100), xLogs[xUI.cFoucs].time[0], xLogs[xUI.cFoucs].time[1]);
                } else {
                    rt_sprintf(cmd, "%02d/%02d/%02d  %02d:%02d", xLogs[xUI.cFoucs].date[1], xLogs[xUI.cFoucs].date[2],
                            (xLogs[xUI.cFoucs].date[0] % 100), xLogs[xUI.cFoucs].time[0], xLogs[xUI.cFoucs].time[1]);
                }
                drawToOLED(1, 0, cmd, OLED_W);
            } else {
                if (xLogs[xUI.cFoucs].en == 1) {
                    usLow = usU8ToU16(xLogs[xUI.cFoucs].data, uuzMSB);
                    usHigh = usU8ToU16((xLogs[xUI.cFoucs].data + 2), uuzMSB);
                    if ((usLow == _TEMP_NULL) || (usHigh == _TEMP_NULL)) {
                        //如果没有任何数据
                        drawToOLED(1, 0, uuzTEMP_NA, OLED_W);
                    } else {
                        if (xSysConfig.Light[_L_TEMP_UNITS] == _TYPE_F) {
                            //华氏度
                            //高温度
                            if ((usHigh >= 320) && (usHigh < 1000)) {
                                rt_sprintf(cmd, "H:%d.%dF", usHigh / 10, usHigh % 10);
                            } else if ((usHigh >= 1000) && (usHigh < 2000)) {
                                rt_sprintf(cmd, "H:%d.%d", usHigh / 10, usHigh % 10);
                            } else {
                                strcpy(cmd, "H:N/A");
                            }
                            //低温度
                            if ((usLow >= 320) && (usLow < 1000)) {
                                rt_sprintf(cmd_t, "L:%d.%dF", usLow / 10, usLow % 10);
                            } else if ((usLow >= 1000) && (usLow < 2000)) {
                                rt_sprintf(cmd_t, "L:%d.%d", usLow / 10, usLow % 10);
                            } else {
                                strcpy(cmd, "L:N/A");
                            }
                        } else {
                            //摄氏度
                            //高温度
                            if ((usHigh >= 0) && (usHigh < 1000)) {
                                rt_sprintf(cmd, "H:%d.%dC", usHigh / 10, usHigh % 10);
                            } else {
                                strcpy(cmd, "H:N/A");
                            }
                            //低温度
                            if ((usLow >= 0) && (usLow < 1000)) {
                                rt_sprintf(cmd_t, "L:%d.%dC", usLow / 10, usLow % 10);
                            } else {
                                strcpy(cmd, "L:N/A");
                            }
                        }
                        //组合数据
                        rt_memcpy(cmd + 9, cmd_t, rt_strlen(cmd_t));
                        //显示相关数据
                        drawToOLED(1, 0, cmd, OLED_W);
                    }
                } else {
                    //如果没有任何数据
                    drawToOLED(1, 0, uuzTEMP_NA, OLED_W);
                }
            }
        }
    }
}
/* ---------------------------------------------------------------------------*/
