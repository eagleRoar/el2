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
#define DBG_SECTION_NAME "UITA"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
/* -----------------------------TEMP UNITS SET-----------------------------------*/
void temp_units_init(void)
{
    if (xInit._temp_units == 0) {
        xInit._temp_units = 1;
    }
}

void temp_units_disp(void)
{
    temp_units_init();
    if (xUI.cID != uuzUI_MENU_TEMP) {
        ui_id_set(uuzUI_MENU_TEMP, 2);
        foucs_set(get_data(_D_LT, _L_TEMP_UNITS));
        LOG_I("Temperature Units UI open");
        value_set(&xCursor, 1, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawToOLED(0, 0, "TEMP UNITS", 10);
        //C/F
        temp_units_value_disp();
        //开启光标闪烁
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        if (xUI.blink == 1) {
            //光标闪烁
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void temp_units_opt(u16 keyId)
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
            temp_units_value_disp();
            xUI.blink = 1;
            break;
        case _key_down:
            //刷新标记
            xUI.blink = 0;
            //移动焦点
            if (xUI.cFoucs == (xUI.maxFoucs - 1)) {
                foucs_set(0);
            } else {
                foucs_move(keyId);
            }
            //显示界面
            temp_units_value_disp();
            //刷新标记
            xUI.blink = 1;
            break;
        case _key_enter:
            //C or F
            if (get_data(_D_LT, _L_TEMP_UNITS) != xUI.cFoucs) {
                //切换单位，将偏移数据置为0
                set_data(_D_LT, _L_CAL_VAL1, 127);
                set_data(_D_LT, _L_CAL_VAL2, 127);
                //将相关配置数据做变更
                u8 ch = 0;
                u8 step = 0;
                for (ch = 0; ch < 2; ch++) {
                    for (step = 0; step < 4; step++) {
                        if (xUI.cFoucs == _TYPE_F) {
                            //转换配置数据为华氏度
                            set_data(_D_LT, _L_CH1_NIGHT_ALARM_TEMP + _CH_LVL(ch) + step,
                                    uuz_usTempUnit_ConvCplt(
                                            get_data(_D_LT, _L_CH1_NIGHT_ALARM_TEMP + _CH_LVL(ch) + step)));
                        } else {
                            //转换配置数据为摄氏度
                            set_data(_D_LT, _L_CH1_NIGHT_ALARM_TEMP + _CH_LVL(ch) + step,
                                    uuz_usTempF2C_ConvCplt(
                                            get_data(_D_LT, _L_CH1_NIGHT_ALARM_TEMP + _CH_LVL(ch) + step)));
                        }
                    }
                }
                //保存温度单位数据
                set_data(_D_LT, _L_TEMP_UNITS, xUI.cFoucs);
                //保存相关数据
                save_data();
            }
            show_ui(uuzUI_TEMP_SEL);
            break;
        case _key_back:
            show_ui(xUI.cID / 10);
            break;
        default:
            break;
    }
}

void temp_units_value_disp(void)
{
    if (xUI.cID == uuzUI_MENU_TEMP) {
        //C/F
        if (xUI.cFoucs == 1) {
            disp_units(_OLED_, 1, 1, _UINT_F);
        } else {
            disp_units(_OLED_, 1, 1, _UINT_C);
        }
    }
}
/* -----------------------------TEMP CAL SELECT-----------------------------------*/
void temp_select_disp(void)
{
    if (xUI.cID != uuzUI_TEMP_SEL) {
        ui_id_set(uuzUI_TEMP_SEL, 2);
        foucs_set(0);
        LOG_I("Temperature select cal UI open");
        value_set(&xCursor, 1, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawToOLED(0, 0, "CALIBRATE", 9);
        //填充数据
        drawToOLED(1, 1, cmd_y_n[xUI.cFoucs], 3);
        //开启闪烁
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        if (xUI.blink == 1) {
            //光标闪烁
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void temp_select_opt(u16 keyId)
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
            drawToOLED(1, 1, cmd_y_n[xUI.cFoucs], 3);
            xUI.blink = 1;
            break;
        case _key_down:
            //刷新标记
            xUI.blink = 0;
            //移动焦点
            if (xUI.cFoucs == (xUI.maxFoucs - 1)) {
                foucs_set(0);
            } else {
                foucs_move(keyId);
            }
            //显示界面
            drawToOLED(1, 1, cmd_y_n[xUI.cFoucs], 3);
            xUI.blink = 1;
            break;
        case _key_enter:
            if (xUI.cFoucs == 0) {
                //NO
                show_ui(uuzUI_MENU);
            } else {
                //YES
                show_ui(uuzUI_TEMP_CAL);
            }
            break;
        case _key_back:
            show_ui(uuzUI_MENU);
            break;
        default:
            break;
    }
}
/* -----------------------------TEMP CAL -----------------------------------*/
Value_Typedef_t xCal[2];
void temp_cal_init(void)
{
    if (xInit._temp_cal == 0) {
        for (u8 ucIndex; ucIndex < 2; ucIndex++) {
            xCal[ucIndex].val = 0;
            xCal[ucIndex].blink = 0;
            xCal[ucIndex].foucs = ucIndex * 2 + 1;
            xCal[ucIndex].h = ucIndex;
            xCal[ucIndex].w = 11;
            xCal[ucIndex].cmd[0] = 'C';    // C or F
        }
        xInit._temp_cal = 1;
    }
}

void temp_cal_disp(void)
{
    u8 ucIndex = 0;

    if (xUI.cID != uuzUI_TEMP_CAL) {
        ui_id_set(uuzUI_TEMP_CAL, 4);
        //显示线路1/2的灯光状态--温度状态
        ucIndex = _CHANNEL_1;
        if (xSta[_S_CH1_TEMP_EN + _CH_DIS(ucIndex)] == 0) {
            //显示提示
            value_set(&xCal[ucIndex], 0, 5);
            xCal[ucIndex].val = _TEMP_NULL;
        } else {
            //有提示数据
            value_set(&xCal[ucIndex], 0, 11);
            //显示线路的温度值
            xCal[ucIndex].val = xSta[_S_CH1_TEMP_VAL + _CH_DIS(ucIndex)];
            //如果数据不相同,或者单位不同
            if (get_data(_D_LT, _L_TEMP_UNITS) == _TYPE_F) {
                xCal[ucIndex].cmd[0] = 'F';
            } else {
                xCal[ucIndex].cmd[0] = 'C';
            }
            xCache.ucCal_Val[ucIndex] = 127;
        }

        //线路2
        ucIndex = _CHANNEL_2;
        if (xSta[_S_CH1_TEMP_EN + _CH_DIS(ucIndex)] == 0) {
            //显示提示
            value_set(&xCal[ucIndex], 1, 5);
            xCal[ucIndex].val = _TEMP_NULL;
        } else {
            //有提示数据
            value_set(&xCal[ucIndex], 1, 11);
            //显示线路的温度值
            xCal[ucIndex].val = xSta[_S_CH1_TEMP_VAL + _CH_DIS(ucIndex)];
            //如果数据不相同,或者单位不同
            if (get_data(_D_LT, _L_TEMP_UNITS) == _TYPE_F) {
                xCal[ucIndex].cmd[0] = 'F';
            } else {
                xCal[ucIndex].cmd[0] = 'C';
            }
            xCache.ucCal_Val[ucIndex] = 127;
        }
        foucs_set(0);
        LOG_I("Temperature cal UI open");
        value_set(&xCursor, 0, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        //显示标题
        drawToOLED(0, 1, "T1:", 3);
        drawToOLED(1, 1, "T2:", 3);
        //显示内容
        temp_cal_value_disp(2);
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        if (xUI.blink == 1) {
            //光标闪烁
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void temp_cal_opt(u16 keyId)
{
    switch (keyId)
    {
        case _key_l_down:
            temp_cal_opt(_key_down);
            break;
        case _key_down:
            //刷新标记
            xUI.blink = 0;
            if (xUI.cFoucs == 0) {
                //清除旧光标
                drawNull(_OLED_, xCursor.h, xCursor.w, 1);
                //移动光标位置
                xUI.cFoucs = 2;
                temp_cal_cursor_disp();
            } else if (xUI.cFoucs == 1) {
                if (xCache.ucCal_Val[_CHANNEL_1] > 0) {
                    xCache.ucCal_Val[_CHANNEL_1]--;
                    //显示数据
                    temp_cal_value_disp(_CHANNEL_1);
                }
            } else if (xUI.cFoucs == 2) {
                //清除旧光标
                drawNull(_OLED_, xCursor.h, xCursor.w, 1);
                //移动光标位置
                xUI.cFoucs = 0;
                temp_cal_cursor_disp();
            } else if (xUI.cFoucs == 3) {
                //大于最小值
                if (xCache.ucCal_Val[_CHANNEL_2] > 0) {
                    xCache.ucCal_Val[_CHANNEL_2]--;
                    //显示数据
                    temp_cal_value_disp(_CHANNEL_2);
                }
            }
            xUI.blink = 1;
            break;
        case _key_l_up:
            temp_cal_opt(_key_up);
            break;
        case _key_up:
            //刷新标记
            xUI.blink = 0;
            //移动焦点
            if (xUI.cFoucs == 2) {
                //清除旧光标
                drawNull(_OLED_, xCursor.h, xCursor.w, 1);
                //移动光标位置
                xUI.cFoucs = 0;
                temp_cal_cursor_disp();
            } else if (xUI.cFoucs == 1) {
                if (xCache.ucCal_Val[_CHANNEL_1] < 256) {
                    xCache.ucCal_Val[_CHANNEL_1]++;
                    //显示数据
                    temp_cal_value_disp(_CHANNEL_1);
                }
            } else if (xUI.cFoucs == 0) {
                //清除旧光标
                drawNull(_OLED_, xCursor.h, xCursor.w, 1);
                //移动光标位置
                xUI.cFoucs = 2;
                temp_cal_cursor_disp();
            } else if (xUI.cFoucs == 3) {
                //大于最小值
                if (xCache.ucCal_Val[_CHANNEL_2] < 256) {
                    xCache.ucCal_Val[_CHANNEL_2]++;
                    //显示数据
                    temp_cal_value_disp(_CHANNEL_2);
                }
            }
            //刷新标记
            xUI.blink = 1;
            break;
        case _key_enter:
            xUI.blink = 0;
            //在工作范围内
            if (xUI.cFoucs % 2 == 0) {
                //清除旧光标
                drawNull(_OLED_, xCursor.h, xCursor.w, 1);
                //移动光标位置
                xUI.cFoucs++;
                temp_cal_cursor_disp();
            } else if (xUI.cFoucs == 1 || xUI.cFoucs == 3) {
                set_data(_D_LT, (_L_CAL_VAL1 + _CH_LVL((xUI.cFoucs - 1) / 2)), xCache.ucCal_Val[(xUI.cFoucs - 1) / 2]);
                //保存相关数据
                save_data();
                //清除旧光标
                drawNull(_OLED_, xCursor.h, xCursor.w, 1);
                xUI.cFoucs--;
                temp_cal_cursor_disp();
            }
            xUI.blink = 1;
            break;
        case _key_back:
            show_ui(uuzUI_MENU);
            break;
        default:
            break;
    }
}

void temp_cal_cursor_disp(void)
{
    if (xUI.cID == uuzUI_TEMP_CAL) {
        //更新光标
        if (xUI.cFoucs == 0) {
            value_set(&xCursor, 0, 0);
        } else if (xUI.cFoucs == 1) {
            value_set(&xCursor, 0, 10);
        } else if (xUI.cFoucs == 2) {
            value_set(&xCursor, 1, 0);
        } else if (xUI.cFoucs == 3) {
            value_set(&xCursor, 1, 10);
        }
    }
}

void temp_cal_value_disp(u8 step)
{
    Value_Typedef_t xValue;

    if (xUI.cID == uuzUI_TEMP_CAL) {
        //显示线路1/2的灯光状态--温度状态
        if (step == 0 || step == 2) {
            //先读取温度设备是否在线
            if (xCal[_CHANNEL_1].val == _TEMP_NULL) {
                //显示未连接提示
                xCal[_CHANNEL_1].cmd[0] = 'R';
                drawToOLED(0, 5, SENSOR_ERR_TIP2, rt_strlen(SENSOR_ERR_TIP2));
            } else {
                //有提示数据
                if (xCal[_CHANNEL_1].cmd[0] == 'R') {
                    //如果有传感器设备错误提示,清除数据,显示标题
                    clear_screen(_SYNC_1);
                    drawToOLED(0, 1, "T1:", 3);
                }
                //获取单位数据
                rt_memcpy(&xValue, &xCal[_CHANNEL_1], uuzVALUE_LEN);
                xValue.val += xCache.ucCal_Val[_CHANNEL_1];
                xValue.val -= 127;
                disp_temperature(_OLED_, &xValue, NULL);
                LOG_I("1-v:%d,h:%d,w:%d", xValue.val, xValue.h, xValue.w);
            }
        }

        if (step == 1 || step == 2) {
            //先读取温度设备是否在线
            if (xCal[_CHANNEL_2].val == _TEMP_NULL) {
                //显示未连接提示
                xCal[_CHANNEL_2].cmd[0] = 'R';
                drawToOLED(1, 5, SENSOR_ERR_TIP2, rt_strlen(SENSOR_ERR_TIP2));
            } else {
                //有提示数据
                if (xCal[_CHANNEL_2].cmd[0] == 'R') {
                    //如果有传感器设备错误提示,清除数据,显示标题
                    clear_screen(_SYNC_2);
                    drawToOLED(1, 1, "T2:", 3);
                }
                rt_memcpy(&xValue, &xCal[_CHANNEL_2], uuzVALUE_LEN);
                xValue.val += xCache.ucCal_Val[_CHANNEL_2];
                xValue.val -= 127;
                disp_temperature(_OLED_, &xValue, NULL);
                LOG_I("2-v:%d,h:%d,w:%d", xValue.val, xValue.h, xValue.w);
            }
        }
    }
}
/* ---------------------------------------------------------------------------*/
