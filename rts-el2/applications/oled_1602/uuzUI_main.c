/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include <rtthread.h>
#include <uuzUI.h>
/* ------------------------- package ----------------------------------------*/
#include "agile_button.h"
#include "drv_flash.h"
/* -------------------------------------------------------------------------------*/
#include "typedefDEF.h"
#include "uuzBTN.h"
#include "uuzINIT.h"
#include "uuzLED.h"
#include "uuzRTC.h"
/* -------------------------------------------------------------------------------*/
#include "uuzDAC.h"
#include "uuzLIGHT.h"
#include "uuzTEMP.h"
/* -------------------------------------------------------------------------------*/
#include "typedefUI.h"
#include "uuzConfig1602.h"
#include "uuzConfigLIGHT.h"
#include "uuzLIGHT.h"
#include "uuzOLED_1602.h"
#include "uuzUI_1602.h"
#include "uuzUI_Cache.h"
/* -------------------------------------------------------------------------------*/
#define DBG_ENABLE
#define DBG_SECTION_NAME "MAIN"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
/* -----------------------------DEVICE LOGO-----------------------------------*/
void logo_init(void)
{
    if (xInit._logo == 0) {
        //三角标
        xCursor.val = 0;
        xCursor.blink = 1;
        xCursor.foucs = 0;
        xCursor.h = 1;
        xCursor.w = 0;
        xCursor.cmd[0] = _UINT_TAB;  //>
        xInit._logo = 1;
    }
}

void logo_disp(void)
{
    char cmd[OLED_W];

    logo_init();
    if (xUI.cID != uuzUI_LOGO) {
        ui_id_set(uuzUI_LOGO, 1);
        foucs_set(0);
        LOG_I("Logo UI open");
        //清除缓存区
        clear_cache_data(_SYNC_A);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //填充数据
        //型号标签
        if (xSysConfig.Sys[_D_DEVICE_TYPE] == 0x00) {
            drawString(_DATA_, 0, 0, "LIGHT CONTROLLER");
        } else if (xSysConfig.Sys[_D_DEVICE_TYPE] == 0x01) {
            drawString(_DATA_, 0, 0, "LIGHT CONTROLLER");
        }
        //硬件版本号
        rt_sprintf(cmd, "@2020 V%d.%02d", (xSysConfig.Sys[_D_HW_VERSION] / 100), (xSysConfig.Sys[_D_HW_VERSION] % 100));
        drawString(_DATA_, 1, 2, cmd);
        //刷新OLED屏幕数据
        sync_oled(_SYNC_A);
        xUI.delay = 0;
    }
}

void logo_opt(u16 keyId)
{
    switch (keyId) {
        case _key_enter:
            case _key_back:
            show_ch_disp();
            break;
        default:
            break;
    }
}

/* -----------------------------DEVICE SHOW CHANNEL-----------------------------------*/
//界面下的数值项
//频道灯光实时状态 OFF|10%~100%
Value_Typedef_t xCh1;
Value_Typedef_t xCh2;
//频道温度实时数值 25.0C|77.0F
Value_Typedef_t xTa1;
Value_Typedef_t xTa2;
//初始化界面控件参数
void show_ch_init(void)
{
    //初始化控件
    if (xInit._show_ch == 0) {
        xCh1.val = 0xFFU;
        xCh1.blink = 0;
        xCh1.foucs = 0;
        xCh1.h = 0;
        xCh1.w = 0;
        xCh1.cmd[0] = '%';  //W

        xCh2.val = 0xFFU;
        xCh2.blink = 0;
        xCh2.foucs = 0;
        xCh2.h = 1;
        xCh2.w = 0;
        xCh2.cmd[0] = '%';  //W

        xTa1.val = 0xFFU;
        xTa1.blink = 0;
        xTa1.foucs = 0;
        xTa1.h = 0;
        xTa1.w = 6;
        xTa1.cmd[0] = 'C';  //C

        xTa2.val = 0xFFU;
        xTa2.blink = 0;
        xTa2.foucs = 0;
        xTa2.h = 1;
        xTa2.w = 6;
        xTa2.cmd[0] = 'C';  //C
        //完成标记
        xInit._show_ch = 1;
    }
}

void show_ch_disp(void)
{
    /*为了解决屏幕黑屏现象，重启屏幕操作*/
    static int resetCnt = 0;

    if(resetCnt++ >= 5)
    {
        oled_reset();
        resetCnt = 0;
    }

    show_ch_init();
    if (xUI.cID != uuzUI_SHOW_CH) {
        ui_id_set(uuzUI_SHOW_CH, 0);
        foucs_set(0);
        LOG_I("Show UI open");
        xSta[_S_LIGHT_MANUAL] = 0;
        //初始化数据
        xCh1.val = 65535;
        xCh1.cmd[0] = '%';
        xTa1.val = 65535;
        xCh2.val = 65535;
        xCh2.cmd[0] = '%';
        xTa2.val = 65535;
        //清除缓存区
        clear_cache_data(_SYNC_A);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //更新相关数据
        ch1_info_disp(_DATA_);
        ch2_info_disp(_DATA_);
        //刷新OLED屏幕数据
        sync_oled(_SYNC_A);
        //清除计数延时
        xUI.delay = 0;
        //该界面不闪烁
        xUI.blink = 0;
    } else {
        ch1_info_disp(_OLED_);
        ch2_info_disp(_OLED_);
    }
}

void show_ch_opt(u16 keyId)
{
    switch (keyId) {
        case _key_up:
            show_ui(uuzUI_SHOW_DATE);
            break;
        case _key_down:
            show_ui(uuzUI_SHOW_TA);
            break;
        case _key_enter:
            show_ui(uuzUI_MENU);
            break;
        case _key_light:
            show_ui(uuzUI_LIGHT);
            break;
        case _key_l_back:
            //长按删除异常数据
            btn_long_event();
            break;
        default:
            break;
    }
}

/**
 * @brief 显示线路1的数据
 * 
 * @param type 
 */
void ch1_info_disp(u8 type)
{
    if (xUI.cID == uuzUI_SHOW_CH) {
        //显示线路1/2的灯光状态--温度状态
        //先读取温度设备是否在线
        if (xSta[_S_CH1_TEMP_EN] == 1) {
            //如果有传感器设备错误提示,清除数据
            if (xCh1.cmd[0] == 'S') {
                disp_txt(type, &xCh1, SENSOR_NULL_TIP);
                xCh1.cmd[0] = '%';
            }
            //设置标题
            //显示线路的灯光强度
            if (get_data(_D_LT, _L_CH1_OUTPUT_MODE) == _LIGHT_MODE_OFF) {
                if (xCh1.val != 0) {
                    //如果数据不相同
                    disp_txt(type, &xCh1, "OFF ");
                    xCh1.val = 0;
                }
            } else {
                //设备为关闭状态
                if (xSta[_S_CH1_DAC_STA] == uuzLTS_OFF) {
                    if (xCh1.val != 0) {
                        //NOTE:如果是关闭状态
                        disp_txt(type, &xCh1, "OFF ");
                        xCh1.val = 0;
                    }
                } else {
                    level_get(_CHANNEL_1, 0);
                    if (xCh1.val != xCache.level) {
                        xCh1.val = xCache.level;
                        disp_value(type, &xCh1, "%");
                    }
                }
            }

            //如果数据不相同,或者单位不同
            if (xTa1.val != xSta[_S_CH1_TEMP_VAL]) {
                xTa1.val = xSta[_S_CH1_TEMP_VAL];
                //检测单位F/C
                if (get_data(_D_LT, _L_TEMP_UNITS) == _TYPE_F) {
                    disp_temperature(type, &xTa1, "F");
                } else {
                    disp_temperature(type, &xTa1, "C");
                }
            }
        } else {
            //显示错误数据提示
            if (xCh1.cmd[0] != 'S') {
                disp_txt(type, &xCh1, SENSOR_ERR_TIP);
                xCh1.cmd[0] = 'S';
                xCh1.val = 65535;
                xTa1.val = 65535;
            }
        }
    }
}

/**
 * @brief 显示线路2的数据
 * 
 * @param type 
 */
void ch2_info_disp(u8 type)
{
    //实时缓存数据
    Value_Typedef_t* xCh;
    Value_Typedef_t* xTa;

    if (xUI.cID == uuzUI_SHOW_CH) {
        xCh = &xCh2;
        xTa = &xTa2;

        //显示线路1/2的灯光状态--温度状态
        //先读取温度设备是否在线
        if (xSta[_S_CH2_TEMP_EN] == 1) {
            //如果有传感器设备错误提示,清除数据
            if (xCh->cmd[0] == 'S') {
                disp_txt(type, xCh, SENSOR_NULL_TIP);
                xCh->cmd[0] = '%';
            }
            //设置标题
            //如果是线路2-跟随模式
            if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_FOLLOW) {
                //跟随模式
                //显示线路1的灯光模式
                if (get_data(_D_LT, _L_CH1_OUTPUT_MODE) == _LIGHT_MODE_OFF) {
                    if (xCh->val != 0) {
                        //如果数据不相同
                        disp_txt(type, xCh, "OFF ");
                        xCh->val = 0;
                    }
                } else {
                    //设备为关闭状态
                    if (xSta[_S_CH1_DAC_STA] == uuzLTS_OFF) {
                        if (xCh->val != 0) {
                            //NOTE:如果是关闭状态
                            disp_txt(type, xCh, "OFF ");
                            xCh->val = 0;
                        }
                    } else {
                        if (xCh->val != get_data(_D_LT, _L_CH1_OUTPUT_LEVEL)) {
                            xCh->val = get_data(_D_LT, _L_CH1_OUTPUT_LEVEL);
                            disp_value(type, xCh, "%");
                        }
                    }
                }
            } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INVERSE) {
                //互斥模式
                //显示线路1的相反灯光强度
                if (get_data(_D_LT, _L_CH1_OUTPUT_MODE) == _LIGHT_MODE_OFF) {
                    //线路2表示为常亮
                    if (xCh->val != get_data(_D_LT, _L_CH1_OUTPUT_LEVEL)) {
                        xCh->val = get_data(_D_LT, _L_CH1_OUTPUT_LEVEL);
                        disp_value(type, xCh, "%");
                    }
                } else {
                    //线路1设备为开启模式,线路2按照亮度显示
                    if (xSta[_S_CH2_DAC_STA] == uuzLTS_OFF) {
                        if (xCh->val != 0) {
                            //NOTE:如果是关闭状态
                            disp_txt(type, xCh, "OFF ");
                            xCh->val = 0;
                        }
                    } else {
                        if (xCh->val != get_data(_D_LT, _L_CH1_OUTPUT_LEVEL)) {
                            xCh->val = get_data(_D_LT, _L_CH1_OUTPUT_LEVEL);
                            disp_value(type, xCh, "%");
                        }
                    }
                }
            } else if (get_data(_D_LT, _L_CH2_MODE) == uuzLIGHT_INDEPENDENT) {
                //独立模式
                //显示线路的灯光强度
                if (get_data(_D_LT, _L_CH2_OUTPUT_MODE) == _LIGHT_MODE_OFF) {
                    if (xCh->val != 0) {
                        //如果数据不相同
                        disp_txt(type, xCh, "OFF ");
                        xCh->val = 0;
                    }
                } else {
                    if (xSta[_S_CH2_DAC_STA] == uuzLTS_OFF) {
                        if (xCh->val != 0) {
                            //如果数据不相同
                            disp_txt(type, xCh, "OFF ");
                            xCh->val = 0;
                        }
                    } else {
                        if (xCh->val != get_data(_D_LT, _L_CH2_OUTPUT_LEVEL)) {
                            xCh->val = get_data(_D_LT, _L_CH2_OUTPUT_LEVEL);
                            disp_value(type, xCh, "%");
                        }
                    }
                }
            }

            //如果数据不相同,或者单位不同
            if (xTa->val != xSta[_S_CH2_TEMP_VAL]) {
                xTa->val = xSta[_S_CH2_TEMP_VAL];
                //检测单位F/C
                if (get_data(_D_LT, _L_TEMP_UNITS) == _TYPE_F) {
                    disp_temperature(type, xTa, "F");
                } else {
                    disp_temperature(type, xTa, "C");
                }
            }
        } else {
            if (xCh->cmd[0] != 'S') {
                disp_txt(type, xCh, SENSOR_ERR_TIP);
                xCh->cmd[0] = 'S';
                xCh->val = 65535;
                xTa->val = 65535;
            }
        }
    }
}
/* -----------------------------DEVICE SHOW DATE-----------------------------------*/
//界面下的数值项
//日期值和时间值
Value_Typedef_t xDate_S;
Value_Typedef_t xTime_S;
//初始化界面控件参数
void show_date_init(void)
{
    //初始化控件
    if (xInit._show_date == 0) {
        xDate_S.val = 0;
        xDate_S.blink = 0;
        xDate_S.foucs = 0;
        xDate_S.h = 1;
        xDate_S.w = 0;
        xDate_S.cmd[0] = '\0';  //W

        xTime_S.val = 0;
        xTime_S.blink = 0;
        xTime_S.foucs = 0;
        xTime_S.h = 1;
        xTime_S.w = 10;
        xTime_S.cmd[0] = '\0';  //W

        //完成标记
        xInit._show_date = 1;
    }
}

void show_date_disp(void)
{
    show_date_init();
    if (xUI.cID != uuzUI_SHOW_DATE) {
        ui_id_set(uuzUI_SHOW_DATE, 0);
        foucs_set(0);
        LOG_I("Show UI open");
        //清除缓存区
        clear_cache_data(_SYNC_A);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawString(_DATA_, 0, 0, "DATE");
        drawString(_DATA_, 0, 10, "TIME");
        //更新相关数据
        date_data_disp(_DATA_, 0);
        //刷新OLED屏幕数据
        sync_oled(_SYNC_A);
        xUI.delay = 0;
    } else {
        date_data_disp(_OLED_, 0);
    }
}

void show_date_opt(u16 keyId)
{
    switch (keyId) {
        case _key_up:
            show_ui(uuzUI_SHOW_TA);
            break;
        case _key_down:
            show_ui(uuzUI_SHOW_CH);
            break;
        case _key_enter:
            show_ui(uuzUI_MENU);
            break;
        case _key_light:
            show_ui(uuzUI_LIGHT);
            break;
        case _key_l_back:
            //长按删除异常数据
            btn_long_event();
            break;
        default:
            break;
    }
}

/**
 *
 * @param type
 * @param step 0:all; 1:Date; 2:Time
 */
void date_data_disp(u8 type, u8 step)
{
    if (xUI.cID == uuzUI_SHOW_DATE) {
        //显示日期
        if (step == 0 || step == 1) {
            if (xSysConfig.Sys[_D_DATE_FORMAT] == _TYPE_MMDDYY) {
                rt_sprintf(xDate_S.cmd, "%02d/%02d/%02d", (rTm.tm_mon + 1), rTm.tm_mday, ((rTm.tm_year + 1900) % 100));
            } else {
                rt_sprintf(xDate_S.cmd, "%02d/%02d/%02d", rTm.tm_mday, (rTm.tm_mon + 1), ((rTm.tm_year + 1900) % 100));
            }
            //显示日期数据
            disp_txt(type, &xDate_S, NULL);
        }
        //显示时间
        if (step == 0 || step == 2) {
            if (xSysConfig.Sys[_D_TIME_FORMAT] == _TYPE_12HOUR) {
                if (rTm.tm_hour < 12) {
                    rt_sprintf(xTime_S.cmd, "%02d:%02dA", rTm.tm_hour, rTm.tm_min);
                } else {
                    rt_sprintf(xTime_S.cmd, "%02d:%02dP", rTm.tm_hour % 12, rTm.tm_min);
                }
            } else {
                rt_sprintf(xTime_S.cmd, "%02d:%02d ", rTm.tm_hour, rTm.tm_min);
            }
            //显示时间数据
            disp_txt(type, &xTime_S, NULL);
        }
    }
}

/* -----------------------------DEVICE SHOW TA-----------------------------------*/
//高温度和低温度
Value_Typedef_t xTaH[2];
Value_Typedef_t xTaL[2];
//初始化界面控件参数
void show_ta_init(void)
{
    //初始化控件
    if (xInit._show_ta == 0) {

        for (u8 ucIndex = 0; ucIndex < 2; ucIndex++) {
            xTaH[ucIndex].val = 0;
            xTaH[ucIndex].blink = 0;
            xTaH[ucIndex].foucs = 0;
            xTaH[ucIndex].h = 0 + ucIndex;
            xTaH[ucIndex].w = 2;
            xTaH[ucIndex].cmd[0] = '\0';  //W

            xTaL[ucIndex].val = 0;
            xTaL[ucIndex].blink = 0;
            xTaL[ucIndex].foucs = 0;
            xTaL[ucIndex].h = 0 + ucIndex;
            xTaL[ucIndex].w = 10;
            xTaL[ucIndex].cmd[0] = '\0';  //W
        }

        //完成标记
        xInit._show_ta = 1;
    }
}

void show_ta_disp(void)
{
    show_ta_init();
    if (xUI.cID != uuzUI_SHOW_TA) {
        ui_id_set(uuzUI_SHOW_TA, 0);
        foucs_set(0);
        LOG_I("Show Ta UI open");
        //清除缓存区
        clear_cache_data(_SYNC_A);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题数据
        drawString(_DATA_, 0, 0, "H:");
        drawString(_DATA_, 0, 8, "L:");
        drawString(_DATA_, 1, 0, "H:");
        drawString(_DATA_, 1, 8, "L:");
        //更新相关数据
        ta_data_disp(_DATA_, 0);
        //刷新OLED屏幕数据
        sync_oled(_SYNC_A);
        //该界面不闪烁
        xUI.delay = 0;
        xUI.blink = 0;
    } else {
        //如果数据有更新，刷新界面
        if (xTaH[_CHANNEL_1].val != xSta[_S_CH1_TEMP_H]) {
            ta_data_disp(_OLED_, 1);
        }
        if (xTaL[_CHANNEL_1].val != xSta[_S_CH1_TEMP_L]) {
            ta_data_disp(_OLED_, 2);
        }
        if (xTaH[_CHANNEL_2].val != xSta[_S_CH2_TEMP_H]) {
            ta_data_disp(_OLED_, 3);
        }
        if (xTaL[_CHANNEL_2].val != xSta[_S_CH2_TEMP_L]) {
            ta_data_disp(_OLED_, 4);
        }
    }
}

void show_ta_opt(u16 keyId)
{
    switch (keyId) {
        case _key_up:
            show_ui(uuzUI_SHOW_CH);
            break;
        case _key_down:
            show_ui(uuzUI_SHOW_DATE);
            break;
        case _key_enter:
            show_ui(uuzUI_MENU);
            break;
        case _key_light:
            show_ui(uuzUI_LIGHT);
            break;
        case _key_l_back:
            //长按删除异常数据
            btn_long_event();
            break;
        default:
            break;
    }
}

/**
 *
 * @param type
 * @param step 0:all;1:CH1-H;2:CH1-L;3-CH2-H;4-CH2-L
 */
void ta_data_disp(u8 type, u8 step)
{
    if (xUI.cID == uuzUI_SHOW_TA) {
        //CH-1 -HIGH
        if (step == 0 || step == 1) {
            //传感器路数1
            if (xSysConfig.Light[_L_TEMP_UNITS] == _TYPE_F) {
                //华氏度F
                if (xSta[_S_CH1_TEMP_H] > 320 && xSta[_S_CH1_TEMP_H] < 1000) {
                    rt_sprintf(xTaH[_CHANNEL_1].cmd, "%d.%dF", xSta[_S_CH1_TEMP_H] / 10, xSta[_S_CH1_TEMP_H] % 10);
                } else if (xSta[_S_CH1_TEMP_H] > 1000 && xSta[_S_CH1_TEMP_H] < 2000) {
                    rt_sprintf(xTaH[_CHANNEL_1].cmd, "%d.%d", xSta[_S_CH1_TEMP_H] / 10, xSta[_S_CH1_TEMP_H] % 10);
                } else {
                    //加载错误提示
                    strcpy(xTaH[_CHANNEL_1].cmd, "N/A");
                }
            } else {
                //摄氏度C
                if (xSta[_S_CH1_TEMP_H] > 0 && xSta[_S_CH1_TEMP_H] < 1000) {
                    rt_sprintf(xTaH[_CHANNEL_1].cmd, "%02d.%dC", xSta[_S_CH1_TEMP_H] / 10, xSta[_S_CH1_TEMP_H] % 10);
                } else {
                    //加载错误提示
                    strcpy(xTaH[_CHANNEL_1].cmd, "N/A");
                }
            }
            //显示数据
            disp_txt(type, &xTaH[_CHANNEL_1], NULL);
            //缓存当前显示数据
            xTaH[_CHANNEL_1].val = xSta[_S_CH1_TEMP_H];
        }

        //CH-1 LOW
        if (step == 0 || step == 2) {
            //传感器路数1
            if (xSysConfig.Light[_L_TEMP_UNITS] == _TYPE_F) {
                //华氏度F
                if (xSta[_S_CH1_TEMP_L] > 320 && xSta[_S_CH1_TEMP_L] < 1000) {
                    rt_sprintf(xTaL[_CHANNEL_1].cmd, "%d.%dF", xSta[_S_CH1_TEMP_L] / 10, xSta[_S_CH1_TEMP_L] % 10);
                } else if (xSta[_S_CH1_TEMP_L] > 1000 && xSta[_S_CH1_TEMP_L] < 2000) {
                    rt_sprintf(xTaL[_CHANNEL_1].cmd, "%d.%d", xSta[_S_CH1_TEMP_L] / 10, xSta[_S_CH1_TEMP_L] % 10);
                } else {
                    //加载错误提示
                    strcpy(xTaL[_CHANNEL_1].cmd, "N/A");
                }
            } else {
                //摄氏度C
                if (xSta[_S_CH1_TEMP_L] > 0 && xSta[_S_CH1_TEMP_L] < 1000) {
                    rt_sprintf(xTaL[_CHANNEL_1].cmd, "%02d.%dC", xSta[_S_CH1_TEMP_L] / 10, xSta[_S_CH1_TEMP_L] % 10);
                } else {
                    //加载错误提示
                    strcpy(xTaL[_CHANNEL_1].cmd, "N/A");
                }
            }
            //显示数据
            disp_txt(type, &xTaL[_CHANNEL_1], NULL);
            //缓存当前显示数据
            xTaL[_CHANNEL_1].val = xSta[_S_CH1_TEMP_L];
        }

        //CH-2 -HIGH
        if (step == 0 || step == 3) {
            //传感器路数1
            if (xSysConfig.Light[_L_TEMP_UNITS] == _TYPE_F) {
                //华氏度F
                if (xSta[_S_CH2_TEMP_H] > 320 && xSta[_S_CH2_TEMP_H] < 1000) {
                    rt_sprintf(xTaH[_CHANNEL_2].cmd, "%d.%dF", xSta[_S_CH2_TEMP_H] / 10, xSta[_S_CH2_TEMP_H] % 10);
                } else if (xSta[_S_CH2_TEMP_H] > 1000 && xSta[_S_CH2_TEMP_H] < 2000) {
                    rt_sprintf(xTaH[_CHANNEL_2].cmd, "%d.%d", xSta[_S_CH2_TEMP_H] / 10, xSta[_S_CH2_TEMP_H] % 10);
                } else {
                    //加载错误提示
                    strcpy(xTaH[_CHANNEL_2].cmd, "N/A");
                }
            } else {
                //摄氏度C
                if (xSta[_S_CH2_TEMP_H] > 0 && xSta[_S_CH2_TEMP_H] < 1000) {
                    rt_sprintf(xTaH[_CHANNEL_2].cmd, "%02d.%dC", xSta[_S_CH2_TEMP_H] / 10, xSta[_S_CH2_TEMP_H] % 10);
                } else {
                    //加载错误提示
                    strcpy(xTaH[_CHANNEL_2].cmd, "N/A");
                }
            }
            //显示数据
            disp_txt(type, &xTaH[_CHANNEL_2], NULL);
            //缓存当前显示数据
            xTaH[_CHANNEL_2].val = xSta[_S_CH2_TEMP_H];
        }

        //CH-2 LOW
        if (step == 0 || step == 4) {
            //传感器路数1
            if (xSysConfig.Light[_L_TEMP_UNITS] == _TYPE_F) {
                //华氏度F
                if (xSta[_S_CH2_TEMP_L] > 320 && xSta[_S_CH2_TEMP_L] < 1000) {
                    rt_sprintf(xTaL[_CHANNEL_2].cmd, "%d.%dF", xSta[_S_CH2_TEMP_L] / 10, xSta[_S_CH2_TEMP_L] % 10);
                } else if (xSta[_S_CH2_TEMP_L] > 1000 && xSta[_S_CH2_TEMP_L] < 2000) {
                    rt_sprintf(xTaL[_CHANNEL_2].cmd, "%d.%d", xSta[_S_CH2_TEMP_L] / 10, xSta[_S_CH2_TEMP_L] % 10);
                } else {
                    //加载错误提示
                    strcpy(xTaL[_CHANNEL_2].cmd, "N/A");
                }
            } else {
                //摄氏度C
                if (xSta[_S_CH2_TEMP_L] > 0 && xSta[_S_CH2_TEMP_L] < 1000) {
                    rt_sprintf(xTaL[_CHANNEL_2].cmd, "%02d.%dC", xSta[_S_CH2_TEMP_L] / 10, xSta[_S_CH2_TEMP_L] % 10);
                } else {
                    //加载错误提示
                    strcpy(xTaL[_CHANNEL_2].cmd, "N/A");
                }
            }
            //显示数据
            disp_txt(type, &xTaL[_CHANNEL_2], NULL);
            //缓存当前显示数据
            xTaL[_CHANNEL_2].val = xSta[_S_CH2_TEMP_L];
        }
    }
}

/* -----------------------------DEVICE INFO-----------------------------------*/
void info_disp(void)
{
    char cmd[OLED_W];

    if (xUI.cID != uuzUI_MENU_INFO) {
        ui_id_set(uuzUI_MENU_INFO, 0);
        foucs_set(0);
        LOG_I("Device Info UI open");
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //填充数据
        //硬件版本号
        rt_sprintf(cmd, "FIRMWARE   v%d.%02d", (xSysConfig.Sys[_D_HW_VERSION] / 100),
                (xSysConfig.Sys[_D_HW_VERSION] % 100));
        drawToOLED(0, 0, cmd, OLED_W);
        //型号标签
        if (xSysConfig.Sys[_D_DEVICE_TYPE] == _EL1) {
            drawToOLED(1, 0, "SYSTEM      BR1 ", OLED_W);
        } else if (xSysConfig.Sys[_D_DEVICE_TYPE] == _EL2) {
            drawToOLED(1, 0, "SYSTEM      BR2 ", OLED_W);
        }
        xUI.delay = 0;
        xUI.blink = 0;
    }
}

void info_opt(u16 keyId)
{
    LOG_I("keyID = %d, UI-ID = %d", keyId, xUI.cID);
    switch (keyId) {
        case _key_back:
            show_ui(xUI.cID / 10);
            break;
        default:
            break;
    }
}

/* -----------------------------LIGHT SHOW-----------------------------------*/
Value_Typedef_t xLevel[2];
Value_Typedef_t xWatt[2];
//初始化界面控件参数
void light_init(void)
{
    u8 ucIndex = 0;
    //初始化控件
    if (xInit._light == 0) {
        for (ucIndex = 0; ucIndex < 2; ucIndex++) {

            xLevel[ucIndex].val = 0;
            xLevel[ucIndex].blink = 0;
            xLevel[ucIndex].foucs = 0;
            xLevel[ucIndex].h = ucIndex;
            xLevel[ucIndex].w = 6;
            xLevel[ucIndex].cmd[0] = '%';  //C
            xLevel[ucIndex].cmd[1] = ' ';  //切换长度100->99
            xLevel[ucIndex].cmd[2] = ' ';  //切换长度10->9

            xWatt[ucIndex].val = 0;
            xWatt[ucIndex].blink = 0;
            xWatt[ucIndex].foucs = 0;
            xWatt[ucIndex].h = ucIndex;
            xWatt[ucIndex].w = 11;
            xWatt[ucIndex].cmd[0] = 'W';  //C
            xWatt[ucIndex].cmd[1] = ' ';  //切换长度1000->999
            xWatt[ucIndex].cmd[2] = ' ';  //切换长度100->99
        }

        LOG_I("Light info UI open");
        //完成标记
        xInit._light = 1;
    }
}

void light_disp(void)
{
    light_init();

    if (xUI.cID != uuzUI_LIGHT) {
        ui_id_set(uuzUI_LIGHT, 2);
        foucs_set(0);
        LOG_I("Light info UI open");
        xUI.blink = 0;
        xSta[_S_LIGHT_MANUAL] = 0;
        //刚进入界面获取相关数据
        level_state_get(_CHANNEL_1, 0);
        level_state_get(_CHANNEL_2, 0);
        //刷新光标位置(h/w)
        value_set(&xCursor, xUI.cFoucs, 5);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //更新相关数据
        //显示不变的标题
        light_title_disp(_CHANNEL_1);
        light_title_disp(_CHANNEL_2);
        //显示线路1/2的灯光状态--强度--瓦数
        //显示等级数据
        light_level_disp(_OLED_, _CHANNEL_1);
        light_level_disp(_OLED_, _CHANNEL_2);
        //如果有瓦数显示，显示瓦数
        light_watt_disp(_OLED_, _CHANNEL_1);
        light_watt_disp(_OLED_, _CHANNEL_2);
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        if (xSta[_S_LIGHT_MANUAL] == 0) {
            //刚进入界面获取相关数据
            level_state_get(_CHANNEL_1, 1);
            level_state_get(_CHANNEL_2, 1);
            //显示等级数据
            light_level_disp(_OLED_, _CHANNEL_1);
            light_level_disp(_OLED_, _CHANNEL_2);
            //如果有瓦数显示，显示瓦数
            light_watt_disp(_OLED_, _CHANNEL_1);
            light_watt_disp(_OLED_, _CHANNEL_2);
        }
        //光标闪烁
        if (xUI.blink == 1) {
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void light_opt(u16 keyId)
{
    switch (keyId) {
        case _key_up:
            case _key_l_up:
            xUI.blink = 0;
            if (xCache.level_L[xUI.cFoucs] < _LIGHT_MAX) {
                xCache.level_L[xUI.cFoucs] += _LIGHT_STEP;
                level_state_get(xUI.cFoucs, 1);
                xSta[_S_LIGHT_MANUAL] = 1;
                //设置DAC的手动当前VALUE值
                dac_set_value(xUI.cFoucs, xCache.level_L[xUI.cFoucs]);
                //显示等级数据
                light_level_disp(_OLED_, xUI.cFoucs);
                //如果有瓦数显示，显示瓦数
                if (xWatt[xUI.cFoucs].val > 0) {
                    light_watt_disp(_OLED_, xUI.cFoucs);
                }
                //处理灯光参数
                rt_light_ch_event(xUI.cFoucs, uuzLTS_ON);
            }
            xUI.blink = 1;
            break;
        case _key_down:
            case _key_l_down:
            xUI.blink = 0;
            if (xCache.level_L[xUI.cFoucs] > _LIGHT_MIN) {
                xCache.level_L[xUI.cFoucs] -= _LIGHT_STEP;
                level_state_get(xUI.cFoucs, 1);
                xSta[_S_LIGHT_MANUAL] = 1;
                //设置DAC的手动当前VALUE值
                dac_set_value(xUI.cFoucs, xCache.level_L[xUI.cFoucs]);
                //显示等级数据
                light_level_disp(_OLED_, xUI.cFoucs);
                //如果有瓦数显示，显示瓦数
                if (xWatt[xUI.cFoucs].val > 0) {
                    light_watt_disp(_OLED_, xUI.cFoucs);
                }
                //处理灯光参数
                rt_light_ch_event(xUI.cFoucs, uuzLTS_ON);
            }
            xUI.blink = 1;
            break;
        case _key_enter:
            //刷新标记
            xUI.blink = 0;
            drawNull(_OLED_, xCursor.h, xCursor.w, 1);
            //移动焦点
            foucs_set((!xUI.cFoucs) ? (1U) : (0U));
            //刷新光标位置
            value_set(&xCursor, xUI.cFoucs, 5);
            //显示界面
            xUI.blink = 1;
            break;
        case _key_back:
            //清除标记
            xSta[_S_LIGHT_MANUAL] = 0;
            show_ui(uuzUI_SHOW_CH);
            break;
        default:
            break;
    }
}

void light_title_disp(u8 ch)
{
    char cmd[OLED_W];

    if (xUI.cID == uuzUI_LIGHT) {
        //设置标题
        rt_sprintf(cmd, "CH%d:", (ch + 1));
        drawToOLED(ch, 0, cmd, 4);
    }
}

void light_level_disp(u8 type, u8 ch)
{
    Value_Typedef_t* level = &xLevel[ch];

    if (xUI.cID == uuzUI_LIGHT) {
        //显示线路的灯光强度
        level->val = xCache.level_L[ch];
        disp_value(type, level, NULL);
        //LOG_I("L-CH:%d", ch);
    }
}

void light_watt_disp(u8 type, u8 ch)
{
    Value_Typedef_t* watt = &xWatt[ch];

    if (xUI.cID == uuzUI_LIGHT) {
        //加载瓦数
        watt->val = xCache.level_W[ch];
        //显示线路的瓦数
        if (watt->val == 0) {
            drawNull(type, watt->h, watt->w, 5);
        } else {
            disp_value(type, watt, NULL);
        }
        //LOG_I("W-CH:%d", ch);
    }
}
/* ---------------------------------------------------------------------------*/
