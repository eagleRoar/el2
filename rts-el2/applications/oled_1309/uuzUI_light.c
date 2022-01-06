/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include <rtthread.h>
/* ------------------------- package ----------------------------------------*/
#include "drv_flash.h"
/* -------------------------------------------------------------------------------*/
#include "typedefDEF.h"
#include "uuzINIT.h"
/* -------------------------------------------------------------------------------*/
#include "uuzBTN.h"
#include "uuzRTC.h"
#include "uuzTEMP.h"
/* -------------------------------------------------------------------------------*/
#include "typedefUI.h"
//#include "uuzConfig1602.h"
//#include "uuzOLED_1602.h"
#include "uuzUI.h"
//#include "uuzUI_1602.h"
#include "uuzUI_Cache.h"
/* -------------------------------------------------------------------------------*/
#define DBG_ENABLE
#define DBG_SECTION_NAME "UI|L"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
#if 0
/* -----------------------------MENU LIST -----------------------------------*/
void channel_init(void)
{
    if (xInit._channel == 0) {
        xInit._channel = 1;
    }
}

void channel_disp(void)
{
    //初始化设置
    channel_init();
    if (xUI.cID != (uuzUI_MENU_CH1 + xCache.channel)) {
        ui_id_set((uuzUI_MENU_CH1 + xCache.channel), LIGHT_LIST_MAX);
        if ((xUI.pID >= uuzUI_OUTPUT_MODE) && (xUI.pID <= uuzUI_DISPLAY_MODE)) {
            foucs_set(xUI.pID % 10);
        } else {
            foucs_set(0);
        }
        LOG_I("Channel-%d UI open", (xCache.channel + 1));
        value_set(&xCursor, 0, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //更新相关数据
        channel_list_disp();
        //刷新OLED屏幕数据
        //sync_oled(_SYNC_A);
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

void channel_opt(u16 keyId)
{
    switch (keyId) {
        case _key_down:
        if (xUI.cFoucs < (xUI.maxFoucs - 1)) {
            //关闭闪烁
            xUI.blink = 0;
            //清除焦点
            drawNull(_OLED_, xCursor.h, xCursor.w, 1);
            //移动光标
            foucs_move(keyId);
            // 清空OLED屏幕
            //clear_screen(_SYNC_A);
            //刷新界面
            channel_list_disp();
            //开启闪烁
            xUI.blink = 1;
        }
        break;
        case _key_up:
        if (xUI.cFoucs > 0) {
            //关闭闪烁
            xUI.blink = 0;
            //清除焦点
            drawNull(_OLED_, xCursor.h, xCursor.w, 1);
            //移动光标
            foucs_move(keyId);
            // 清空OLED屏幕
            //clear_screen(_SYNC_A);
            //刷新界面
            channel_list_disp();
            //开启闪烁
            xUI.blink = 1;
        }
        break;
        case _key_enter:
        switch (xUI.cFoucs) {
            case 0x00:
            show_ui(uuzUI_OUTPUT_MODE);
            break;
            case 0x01:
            show_ui(uuzUI_OUTPUT_LEVEL);
            break;
            case 0x02:
            show_ui(uuzUI_LIGHT_CYCLE);
            break;
            case 0x03:
            show_ui(uuzUI_SET_TEMP_LEVELS);
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

void channel_list_disp(void)
{
    u8 line1 = 0;
    u8 line2 = 1;

    if (xUI.cID == (uuzUI_MENU_CH1 + xCache.channel)) {
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
            drawToOLED(0, 1, cmd_1_1[line1], 15);
            drawToOLED(1, 1, cmd_1_1[line2], 15);
        }
    }
}
/* -----------------CHANNEL CH2 MODE----------------------*/
void ch2_mode_init(void)
{
    if (xInit._menu_mode == 0) {
        xInit._menu_mode = 1;
    }
}

void ch2_mode_disp(void)
{
    ch2_mode_init();
    if (xUI.cID != (uuzUI_MENU_MODE)) {
        ui_id_set(uuzUI_MENU_MODE, 3);
        //获取的CH2灯光的配置显示模式
        foucs_set(get_data(_D_LT, _L_CH2_MODE));
        LOG_I("CH2 Mode UI open");
        value_set(&xCursor, 1, 0);
        clear_cache_data(_SYNC_A);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //填充数据
        ch2_mode_value_disp(_DATA_);
        //刷新OLED屏幕数据
        sync_oled(_SYNC_A);
        xUI.delay = 0;
    } else {
        if (xUI.blink == 0) {
            //更新相关数据
            ch2_mode_value_disp(_OLED_);
            xUI.blink = 1;
        } else {
            //光标闪烁
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void ch2_mode_opt(u16 keyId)
{
    switch (keyId) {
        case _key_down:
        if (xUI.cFoucs == (xUI.maxFoucs - 1)) {
            xUI.cFoucs = 0;
        } else {
            foucs_move(keyId);
        }
        xUI.blink = 0;
        show_ui(xUI.cID);
        break;
        case _key_up:
        if (xUI.cFoucs == 0) {
            xUI.cFoucs = (xUI.maxFoucs - 1);
        } else {
            foucs_move(keyId);
        }
        xUI.blink = 0;
        show_ui(xUI.cID);
        break;
        case _key_enter:
        //在工作范围内
        if (xUI.cFoucs < xUI.maxFoucs) {
            //保存相应数据
            set_data(_D_LT, _L_CH2_MODE, xUI.cFoucs);
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

void ch2_mode_value_disp(u8 type)
{
    if (xUI.cID == uuzUI_MENU_MODE) {
        if (type == _DATA_) {
            //显示标题
            drawString(type, 0, 0, cmd_1[xUI.cID % 10]);
            //显示光标
            disp_cursor(type, &xCursor);
        }
        //显示CH2的工作模式
        drawString(type, 1, 1, cmd_1_4[xUI.cFoucs]);
    }
}
/* -----------------CHANNEL-1/2 SETUP OUTPUT MODE----------------------*/
void output_mode_init(void)
{
    if (xInit._output_mode == 0) {
        xInit._output_mode = 1;
    }
}

void output_mode_disp(void)
{
    output_mode_init();
    if (xUI.cID != (uuzUI_OUTPUT_MODE)) {
        //关闭光标闪烁
        xUI.blink = 0;
        ui_id_set(uuzUI_OUTPUT_MODE, 3);
        //获取的CH灯光的显示模式
        foucs_set(get_data(_D_LT, _L_CH1_OUTPUT_MODE + _CH_LVL(xCache.channel)));
        LOG_I("[%d]Output Mode UI open", xCache.channel);
        value_set(&xCursor, 1, 0);
        //清除缓存数据
        clear_cache_data(_SYNC_A);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawToDATA(0, 0, cmd_1_1[xUI.cID % 10], 11);
        //填充c数据
        drawToDATA(1, 1, cmd_1_1_1[xUI.cFoucs], 4);
        //显示光标
        disp_cursor(_DATA_, &xCursor);
        //刷新OLED屏幕数据
        sync_oled(_SYNC_A);
        //关闭延时
        xUI.delay = 0;
        //开启光标闪烁
        xUI.blink = 1;
    } else {
        //光标闪烁
        disp_cursor(_OLED_, &xCursor);
    }
}

void output_mode_opt(u16 keyId)
{
    switch (keyId) {
        case _key_down:
        xUI.blink = 0;
        if (foucs_move_cycle(keyId)) {
            //显示NO/YES/AUTO
            drawToOLED(1, 1, cmd_1_1_1[xUI.cFoucs], 4);
        }
        xUI.blink = 1;
        break;
        case _key_up:
        xUI.blink = 0;
        if (xUI.cFoucs == 0) {
            xUI.cFoucs = (xUI.maxFoucs - 1);
            //显示NO/YES/AUTO
            drawToOLED(1, 1, cmd_1_1_1[xUI.cFoucs], 4);
        } else {
            if (foucs_move_cycle(keyId)) {
                //显示NO/YES/AUTO
                drawToOLED(1, 1, cmd_1_1_1[xUI.cFoucs], 4);
            }
        }
        xUI.blink = 1;
        break;
        case _key_enter:
        //在工作范围内
        if (xUI.cFoucs < xUI.maxFoucs) {
            //保存相应数据
            set_data(_D_LT, (_L_CH1_OUTPUT_MODE + _CH_LVL(xCache.channel)), xUI.cFoucs);
            //保存相关数据
            save_data();
            LOG_I("save ui open");
            //刷新模式缓存数据
            xCache.chMode[xCache.channel] = xUI.cFoucs;
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
/* -----------------CHANNEL-1/2 SETUP OUTPUT LEVEL----------------------*/
Value_Typedef_t xLevelOpt;  //光标位
void output_level_init(void)
{
    if (xInit._output_level == 0) {
        //亮度等级
        xLevelOpt.val = 0;
        xLevelOpt.blink = 0;
        xLevelOpt.foucs = 0;
        xLevelOpt.h = 1;
        xLevelOpt.w = 1;
        xLevelOpt.cmd[0] = '%';
        xLevelOpt.cmd[1] = ' ';//解决显示位数3位到4位的问题
        xInit._output_level = 1;
    }
}
void output_level_disp(void)
{
    output_level_init();
    if (xUI.cID != (uuzUI_OUTPUT_LEVEL)) {
        ui_id_set(uuzUI_OUTPUT_LEVEL, 1);
        //获取的CH灯光的强度
        xLevelOpt.val = get_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(xCache.channel));
        foucs_set(0);
        value_set(&xCursor, 1, 0);
        //清除缓存数据
        clear_cache_data(_SYNC_A);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawString(_OLED_, 0, 0, cmd_1_1[xUI.cID % 10]);
        //显示光标
        disp_cursor(_OLED_, &xCursor);
        //填充数据
        //显示50%~115%
        disp_value(_OLED_, &xLevelOpt, NULL);
        //刷新OLED屏幕数据
        //sync_oled(_SYNC_A);
        xUI.delay = 0;
    } else {
        //光标闪烁
        disp_cursor(_OLED_, &xCursor);
    }
}

void output_level_opt(u16 keyId)
{
    switch (keyId) {
        case _key_down:
        case _key_l_down:
        if (xLevelOpt.val > 50) {
            xUI.blink = 0;
            xLevelOpt.val--;
            disp_value(_OLED_, &xLevelOpt, NULL);
            xUI.blink = 1;
        }
        break;
        case _key_up:
        case _key_l_up:
        if (xLevelOpt.val < 115) {
            xUI.blink = 0;
            xLevelOpt.val++;
            disp_value(_OLED_, &xLevelOpt, NULL);
            xUI.blink = 1;
        }
        break;
        case _key_enter:
        //在工作范围内
        if ((xLevelOpt.val >= 50) && (xLevelOpt.val <= 115)) {
            xUI.blink = 0;
            set_data(_D_LT, _L_CH1_OUTPUT_LEVEL + _CH_LVL(xCache.channel), xLevelOpt.val);
            //保存相关数据
            save_data();
            //返回上一页
            show_ui(xUI.cID / 10);
            xUI.blink = 1;
        }
        break;
        case _key_back:
        show_ui(xUI.cID / 10);
        break;
        default:
        break;
    }
}
/* -----------------CHANNEL-1/2 SETUP LIGHT CYCLE----------------------*/
Value_Typedef_t xTimeSet[6];  //开始时间HH:MM-结束时间HH:MM
void light_cycle_init(void)
{
    u8 ucIndex = 0;
    if (xInit._light_cycle == 0) {
        for (ucIndex = 0; ucIndex < 4; ucIndex++) {
            xTimeSet[ucIndex].val = 0;
            xTimeSet[ucIndex].blink = 0;
            xTimeSet[ucIndex].foucs = ucIndex;
            xTimeSet[ucIndex].h = ucIndex / 2;
            xTimeSet[ucIndex].w = (ucIndex % 2) * 3 + 8;
            xTimeSet[ucIndex].cmd[0] = '\0';  //NULL
        }
        for (ucIndex = 4; ucIndex < 6; ucIndex++) {
            xTimeSet[ucIndex].val = 0;
            xTimeSet[ucIndex].blink = 0;
            xTimeSet[ucIndex].foucs = ucIndex;
            xTimeSet[ucIndex].h = ucIndex % 2;
            xTimeSet[ucIndex].w = 13;
            xTimeSet[ucIndex].cmd[0] = ' ';  //SPACE
        }
        xInit._light_cycle = 1;
    }
}

void light_cycle_disp(void)
{
    light_cycle_init();
    if (xUI.cID != (uuzUI_LIGHT_CYCLE)) {
        ui_id_set(uuzUI_LIGHT_CYCLE, 4);
        //获取的CH灯光的定时时间
        foucs_set(0);
        //初始化时间参数
        xCache.timeCH[0] = get_data(_D_LT, _L_CH1_CYCLE_ON + _CH_LVL(xCache.channel));
        xCache.timeCH[1] = get_data(_D_LT, _L_CH1_CYCLE_OFF + _CH_LVL(xCache.channel));

        //显示时制
        if (xSysConfig.Sys[_D_TIME_FORMAT] == _TYPE_12HOUR) {
            //显示尾标A/P
            if ((xCache.timeCH[0] / 60) < 12) {
                xTimeSet[4].cmd[0] = 'A';
                xTimeSet[4].val = 0;
            } else {
                xTimeSet[4].cmd[0] = 'P';
                xTimeSet[4].val = 1;
            }
            if ((xCache.timeCH[1] / 60) < 12) {
                xTimeSet[5].cmd[0] = 'A';
                xTimeSet[5].val = 0;
            } else {
                xTimeSet[5].cmd[0] = 'P';
                xTimeSet[5].val = 1;
            }
            //加载数据
            xTimeSet[0].val = (xCache.timeCH[0] / 60) % 12;
            xTimeSet[1].val = (xCache.timeCH[0] % 60);
            xTimeSet[2].val = (xCache.timeCH[1] / 60) % 12;
            xTimeSet[3].val = (xCache.timeCH[1] % 60);
        } else {
            //加载数据
            xTimeSet[0].val = (xCache.timeCH[0] / 60);
            xTimeSet[1].val = (xCache.timeCH[0] % 60);
            xTimeSet[2].val = (xCache.timeCH[1] / 60);
            xTimeSet[3].val = (xCache.timeCH[1] % 60);
            //显示尾标为空
            xTimeSet[4].cmd[0] = ' ';
            xTimeSet[5].cmd[0] = ' ';
        }
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawToOLED(0, 0, "ON", 2);
        drawToOLED(1, 10, ":", 1);
        drawToOLED(1, 0, "OFF", 3);
        drawToOLED(0, 10, ":", 1);
        //刷新闪烁位置
        light_cycle_value_disp(_OLED_, 5);
        //填充数据
        light_cycle_value_disp(_OLED_, 4);
        //刷新尾标(A/P)
        drawToOLED(xTimeSet[4].h, xTimeSet[4].w, xTimeSet[4].cmd, 1);
        drawToOLED(xTimeSet[5].h, xTimeSet[5].w, xTimeSet[5].cmd, 1);
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        //光标闪烁
        if (xUI.blink == 1) {
            disp_time(_OLED_, &xTimeSet[xUI.cFoucs]);
        }
    }
}

void light_cycle_opt(u16 keyId)
{
    switch (keyId) {
        case _key_up:
        case _key_l_up:
        xUI.blink = 0;
        xUI.delay = 0;
        if (xUI.cFoucs == 0) {
            //CYCLE-ON-TIME-HOUR
            if (xCache.timeCH[0] >= 23 * 60) {
                xCache.timeCH[0] -= (23 * 60);
            } else {
                xCache.timeCH[0] += 60;
            }
        } else if (xUI.cFoucs == 1) {
            //CYCLE-ON-TIME-MIN
            if ((xCache.timeCH[0] % 60) == 59) {
                xCache.timeCH[0] -= 59;
            } else {
                xCache.timeCH[0]++;
            }
        } else if (xUI.cFoucs == 2) {
            //CYCLE-OFF-TIME-HOUR
            if (xCache.timeCH[1] >= 23 * 60) {
                xCache.timeCH[1] -= (23 * 60);
            } else {
                xCache.timeCH[1] += 60;
            }
        } else if (xUI.cFoucs == 3) {
            //CYCLE-OFF-TIME-MIN
            if ((xCache.timeCH[1] % 60) == 59) {
                xCache.timeCH[1] -= 59;
            } else {
                xCache.timeCH[1]++;
            }
        }
        //显示时制
        if (xSysConfig.Sys[_D_TIME_FORMAT] == _TYPE_12HOUR) {
            //显示尾标A/P
            if ((xCache.timeCH[0] / 60) < 12) {
                xTimeSet[4].cmd[0] = 'A';
                xTimeSet[4].val = 0;
            } else {
                xTimeSet[4].cmd[0] = 'P';
                xTimeSet[4].val = 1;
            }
            if ((xCache.timeCH[1] / 60) < 12) {
                xTimeSet[5].cmd[0] = 'A';
                xTimeSet[5].val = 0;
            } else {
                xTimeSet[5].cmd[0] = 'P';
                xTimeSet[5].val = 1;
            }
            //加载数据
            xTimeSet[0].val = (xCache.timeCH[0] / 60) % 12;
            xTimeSet[1].val = (xCache.timeCH[0] % 60);
            xTimeSet[2].val = (xCache.timeCH[1] / 60) % 12;
            xTimeSet[3].val = (xCache.timeCH[1] % 60);
        } else {
            //显示尾标为空
            xTimeSet[4].cmd[0] = ' ';
            xTimeSet[5].cmd[0] = ' ';
            //加载数据
            xTimeSet[0].val = (xCache.timeCH[0] / 60);
            xTimeSet[1].val = (xCache.timeCH[0] % 60);
            xTimeSet[2].val = (xCache.timeCH[1] / 60);
            xTimeSet[3].val = (xCache.timeCH[1] % 60);
        }
        //刷新单个数据
        disp_time(_OLED_, &xTimeSet[xUI.cFoucs]);
        //刷新尾标(A/P)
        if (xUI.cFoucs < 2) {
            drawToOLED(xTimeSet[4].h, xTimeSet[4].w, xTimeSet[4].cmd, 1);
        } else if (xUI.cFoucs < 4) {
            drawToOLED(xTimeSet[5].h, xTimeSet[5].w, xTimeSet[5].cmd, 1);
        }
        xUI.blink = 1;
        break;
        case _key_down:
        case _key_l_down:
        xUI.blink = 0;
        xUI.delay = 0;
        if (xUI.cFoucs == 0) {
            if (xCache.timeCH[0] < 60) {
                xCache.timeCH[0] += 23 * 60;
            } else {
                xCache.timeCH[0] -= 60;
            }
        } else if (xUI.cFoucs == 1) {
            if ((xCache.timeCH[0] % 60) == 0) {
                xCache.timeCH[0] += 59;
            } else {
                xCache.timeCH[0]--;
            }
        } else if (xUI.cFoucs == 2) {
            if (xCache.timeCH[1] < 60) {
                xCache.timeCH[1] += 23 * 60;
            } else {
                xCache.timeCH[1] -= 60;
            }
        } else if (xUI.cFoucs == 3) {
            if ((xCache.timeCH[1] % 60) == 0) {
                xCache.timeCH[1] += 59;
            } else {
                xCache.timeCH[1]--;
            }
        }
        //显示时制
        if (xSysConfig.Sys[_D_TIME_FORMAT] == _TYPE_12HOUR) {
            //显示尾标A/P
            if ((xCache.timeCH[0] / 60) < 12) {
                xTimeSet[4].cmd[0] = 'A';
                xTimeSet[4].val = 0;
            } else {
                xTimeSet[4].cmd[0] = 'P';
                xTimeSet[4].val = 1;
            }
            if ((xCache.timeCH[1] / 60) < 12) {
                xTimeSet[5].cmd[0] = 'A';
                xTimeSet[5].val = 0;
            } else {
                xTimeSet[5].cmd[0] = 'P';
                xTimeSet[5].val = 1;
            }
            //加载数据
            xTimeSet[0].val = (xCache.timeCH[0] / 60) % 12;
            xTimeSet[1].val = (xCache.timeCH[0] % 60);
            xTimeSet[2].val = (xCache.timeCH[1] / 60) % 12;
            xTimeSet[3].val = (xCache.timeCH[1] % 60);
        } else {
            //显示尾标为空
            xTimeSet[4].cmd[0] = ' ';
            xTimeSet[5].cmd[0] = ' ';
            //加载数据
            xTimeSet[0].val = (xCache.timeCH[0] / 60);
            xTimeSet[1].val = (xCache.timeCH[0] % 60);
            xTimeSet[2].val = (xCache.timeCH[1] / 60);
            xTimeSet[3].val = (xCache.timeCH[1] % 60);
        }
        disp_time(_OLED_, &xTimeSet[xUI.cFoucs]);
        //刷新尾标(A/P)
        if (xUI.cFoucs < 2) {
            drawToOLED(xTimeSet[4].h, xTimeSet[4].w, xTimeSet[4].cmd, 1);
        } else if (xUI.cFoucs < 4) {
            drawToOLED(xTimeSet[5].h, xTimeSet[5].w, xTimeSet[5].cmd, 1);
        }
        xUI.blink = 1;
        break;
        case _key_enter:
        //移动焦点
        if (xUI.cFoucs < (xUI.maxFoucs - 1)) {
            //刷新标记
            xUI.blink = 0;
            //移动光标
            foucs_single_move(keyId);
            //刷新闪烁位置
            light_cycle_value_disp(_OLED_, 4);
            xUI.blink = 1;
        } else {
            //移动到最后焦点,设置循环数据
            set_data(_D_LT, _L_CH1_CYCLE_ON + _CH_LVL(xCache.channel), xCache.timeCH[0]);
            set_data(_D_LT, _L_CH1_CYCLE_OFF + _CH_LVL(xCache.channel), xCache.timeCH[1]);
            //保存相关数据
            save_data();
            //返回上一页
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

/**
 *
 * @param type
 * @param step:焦点0/1/2/3--4:全部刷新
 */
void light_cycle_value_disp(u8 type, u8 step)
{
    u8 ucIndex = 0;
    if (xUI.cID == uuzUI_LIGHT_CYCLE) {
        //显示时间-小时-分钟
        if (step < 4) {
            //单个刷新
            disp_time(type, &xTimeSet[step]);
        } else {
            //显示光标闪烁
            for (ucIndex = 0; ucIndex < 4; ucIndex++) {
                if (ucIndex == xUI.cFoucs) {
                    //光标闪烁
                    xTimeSet[ucIndex].blink = 1;
                } else {
                    //清空光标参数
                    xTimeSet[ucIndex].blink = 0;
                }
            }
            //全部刷新
            for (ucIndex = 0; ucIndex < 4; ucIndex++) {
                disp_time(type, &xTimeSet[ucIndex]);
            }
        }
    }
}
/* -----------------CHANNEL-1/2 SET TEMP_LEVELS----------------------*/
Value_Typedef_t xLTa[4];  //和灯光相关温度设置信息
void set_temp_levels_init(void)
{
    u8 h[4] = {0, 1, 1, 1};
    if (xInit._set_temp_levels == 0) {
        //Night Temperature Alarm时间数据
        //Day Temperature Alarm时间数据
        //Auto-Dim Temperature Alarm时间数据
        //Auto-Off Temperature Alarm时间数据
        for (u8 ucIndex = 0; ucIndex < 4; ucIndex++) {
            xLTa[ucIndex].val = 0;
            xLTa[ucIndex].blink = 0;
            xLTa[ucIndex].foucs = ucIndex;
            xLTa[ucIndex].h = h[ucIndex];
            xLTa[ucIndex].w = 11;
            xLTa[ucIndex].cmd[0] = 'C';
        }
        xInit._set_temp_levels = 1;
    }
}

void set_temp_levels_disp(void)
{
    u8 ucIndex = 0;
    set_temp_levels_init();

    if (xUI.cID != (uuzUI_SET_TEMP_LEVELS)) {
        ui_id_set(uuzUI_SET_TEMP_LEVELS, 4);
        //获取的CH灯光的温度报警值
        for (ucIndex = 0; ucIndex < 4; ucIndex++) {
            //获取相关数据
            xLTa[ucIndex].val = get_data(_D_LT, _L_CH1_NIGHT_ALARM_TEMP + _CH_LVL(xCache.channel) + ucIndex);
            //修改单位
            if (get_data(_D_LT, _L_TEMP_UNITS) == _TYPE_F) {
                xLTa[ucIndex].cmd[0] = 'F';
            } else {
                xLTa[ucIndex].cmd[0] = 'C';
            }
        }
        foucs_set(0);
        //光标位置
        value_set(&xCursor, 0, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        set_temp_levels_title_disp(_OLED_);
        //填充数据
        set_temp_levels_value_disp(_OLED_, xUI.maxFoucs);
        //显示光标
        set_temp_levels_cursor_disp(_OLED_);
        //刷新OLED屏幕数据
        //sync_oled(_SYNC_A);
        //开启闪烁
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        //光标闪烁
        disp_cursor(_OLED_, &xCursor);
    }
}

void set_temp_levels_opt(u16 keyId)
{
    u8 foucs = (xUI.cFoucs - 1) / 2;

    switch (keyId) {
        case _key_down:
        //关闭闪烁
        xUI.blink = 0;
        if ((xUI.cFoucs == 0) || (xUI.cFoucs == 2) || (xUI.cFoucs == 4)) {
            xUI.cFoucs += 2;
            //换页，显示标题
            set_temp_levels_title_disp(_OLED_);
            //显示光标
            set_temp_levels_cursor_disp(_OLED_);
            //填充数据
            set_temp_levels_value_disp(_OLED_, xUI.maxFoucs);
        } else if ((xUI.cFoucs == 1) || (xUI.cFoucs == 3) || (xUI.cFoucs == 5) || (xUI.cFoucs == 7)) {
            //大于最小值
            if (xLTa[foucs].val > set_temp_limit_get(0, foucs)) {
                xLTa[foucs].val--;
                //填充数据
                set_temp_levels_value_disp(_OLED_, foucs);
            }
        }
        //启动闪烁
        xUI.blink = 1;
        break;
        case _key_l_down:
        if ((xUI.cFoucs == 1) || (xUI.cFoucs == 3) || (xUI.cFoucs == 5) || (xUI.cFoucs == 7)) {
            set_temp_levels_opt(_key_down);
        }
        break;
        case _key_up:
        xUI.blink = 0;
        if ((xUI.cFoucs == 2) || (xUI.cFoucs == 4) || (xUI.cFoucs == 6)) {
            xUI.cFoucs -= 2;
            //换页，显示标题
            set_temp_levels_title_disp(_OLED_);
            //填充数据
            set_temp_levels_value_disp(_OLED_, xUI.maxFoucs);
            //显示光标
            set_temp_levels_cursor_disp(_OLED_);
        } else if ((xUI.cFoucs == 1) || (xUI.cFoucs == 3) || (xUI.cFoucs == 5) || (xUI.cFoucs == 7)) {
            //小于最大值
            if (xLTa[foucs].val < set_temp_limit_get(1, foucs)) {
                xLTa[foucs].val++;
                //填充数据
                set_temp_levels_value_disp(_OLED_, foucs);
            }
        }
        xUI.blink = 1;
        break;
        case _key_l_up:
        if ((xUI.cFoucs == 1) || (xUI.cFoucs == 3) || (xUI.cFoucs == 5) || (xUI.cFoucs == 7)) {
            set_temp_levels_opt(_key_up);
        }
        break;
        case _key_enter:
        xUI.blink = 0;
        //在工作范围内
        if ((xUI.cFoucs == 0) || (xUI.cFoucs == 2) || (xUI.cFoucs == 4) || (xUI.cFoucs == 6)) {
            //切换到编辑位置
            xUI.cFoucs++;
            //换页，显示标题
            set_temp_levels_title_disp(_OLED_);
            //填充数据
            set_temp_levels_value_disp(_OLED_, xUI.maxFoucs);
            //显示光标
            set_temp_levels_cursor_disp(_OLED_);
        } else if ((xUI.cFoucs == 1) || (xUI.cFoucs == 3) || (xUI.cFoucs == 5) || (xUI.cFoucs == 7)) {
            set_data(_D_LT, _L_CH1_NIGHT_ALARM_TEMP + _CH_LVL(xCache.channel) + foucs, xLTa[foucs].val);
            //保存相关数据
            save_data();
            xUI.cFoucs--;
            //显示光标
            set_temp_levels_cursor_disp(_OLED_);
        }
        xUI.blink = 1;
        break;
        case _key_back:
        if ((xUI.cFoucs % 2)) {
            foucs_set(xUI.cFoucs - 1);
            xUI.blink = 0;
            show_ui(xUI.cID);
        } else {
            show_ui(xUI.cID / 10);
        }
        break;
        default:
        break;
    }
}

/**
 * @brief 计算温度调节的上下限值
 * @param type:0-min;1-max;
 * @param foucs:0-1-2-3
 * @return
 */
u16 set_temp_limit_get(u8 type, u16 foucs)
{
    u16 _Tmin = 0;
    u16 _Tmax = 0;

    //获取摄氏度的最大最小值
    u16 _cmax[4] = {300, 300, 350, 500};  //max = 30.0C
    u16 _cmin[4] = {100, 100, 275, 300};  //min = 10.0C
    //获取华氏度的最大最小值
    u16 _fmax[4] = {860, 860, 950, 1220};
    u16 _fmin[4] = {500, 500, 815, 860};

    //加载当前焦点的最大最小值
    if (get_data(_D_LT, _L_TEMP_UNITS) == _TYPE_F) {
        _Tmax = _fmax[foucs];
        _Tmin = _fmin[foucs];
    } else {
        _Tmax = _cmax[foucs];
        _Tmin = _cmin[foucs];
    }

    if (type == 0) {
        return _Tmin;
    } else {
        return _Tmax;
    }
}

void set_temp_levels_cursor_disp(u8 type)
{
    if (xUI.cID == uuzUI_SET_TEMP_LEVELS) {
        //消除原有光标
        drawNull(_OLED_, xCursor.h, xCursor.w, 1);
        if (xUI.cFoucs == 0) {
            //显示光标在栏1-0
            value_set(&xCursor, 0, 0);
        } else if (xUI.cFoucs == 1) {
            //显示光标在栏1-2
            value_set(&xCursor, 0, 10);
        } else if (xUI.cFoucs == 2) {
            //显示光标在栏2-1
            value_set(&xCursor, 1, 0);
        } else if (xUI.cFoucs == 3) {
            //显示光标在栏2-2
            value_set(&xCursor, 1, 10);
        } else if (xUI.cFoucs == 4) {
            //显示光标在栏2-1
            value_set(&xCursor, 1, 0);
        } else if (xUI.cFoucs == 5) {
            //显示光标在栏2-2
            value_set(&xCursor, 1, 10);
        } else if (xUI.cFoucs == 6) {
            //显示光标在栏2-1
            value_set(&xCursor, 1, 0);
        } else if (xUI.cFoucs == 7) {
            //显示光标在栏2-2
            value_set(&xCursor, 1, 10);
        }
        disp_cursor(_OLED_, &xCursor);
    }
}

void set_temp_levels_title_disp(u8 type)
{
    if (xUI.cID == uuzUI_SET_TEMP_LEVELS) {

        switch (xUI.cFoucs) {
            case 0:
            case 1:
            case 2:
            case 3:
            //显示数据在栏1-0/1-1/2-0/2-1
            value_set(&xLTa[0], 0, 11);
            value_set(&xLTa[1], 1, 11);
            //显示标题
            drawToOLED(0, 1, cmd_1_1_4[0], 8);
            drawToOLED(1, 1, cmd_1_1_4[1], 8);
            break;
            case 4:
            case 5:
            //显示数据在栏2-0/2-1
            value_set(&xLTa[1], 0, 11);
            value_set(&xLTa[2], 1, 11);
            drawToOLED(0, 1, cmd_1_1_4[1], 8);
            drawToOLED(1, 1, cmd_1_1_4[2], 8);
            break;
            case 6:
            case 7:
            //显示数据在栏2-0/2-1
            value_set(&xLTa[2], 0, 11);
            value_set(&xLTa[3], 1, 11);
            drawToOLED(0, 1, cmd_1_1_4[2], 8);
            drawToOLED(1, 1, cmd_1_1_4[3], 8);
            break;
            default:
            break;
        }
    }
}

/**
 *
 * @param type:_OLED_/_DATA_
 * @param step:0->Night/1->Day/2->Auto-Dim/3->Shutdown:::4:All
 */
void set_temp_levels_value_disp(u8 type, u8 step)
{
    if (xUI.cID == uuzUI_SET_TEMP_LEVELS) {
        //显示温度值(10C~30C)
        //LOG_I("1:%c-2:%c-3:%c-4:%c", xNight.cmd[0], xDay.cmd[0], xAutoDim.cmd[0], xShutDown.cmd[0]);
        if (step == 4) {
            if (xUI.cFoucs < 4) {
                disp_temperature(type, &xLTa[0], NULL);
                disp_temperature(type, &xLTa[1], NULL);
            } else if (xUI.cFoucs < 6) {
                disp_temperature(type, &xLTa[1], NULL);
                disp_temperature(type, &xLTa[2], NULL);
            } else if (xUI.cFoucs < 8) {
                disp_temperature(type, &xLTa[2], NULL);
                disp_temperature(type, &xLTa[3], NULL);
            }
        } else {
            disp_temperature(type, &xLTa[step], NULL);
        }
    }
}
/* -----------------CHANNEL-1/2 SUNRISE SUNSET----------------------*/
Value_Typedef_t xSunRise;
Value_Typedef_t xSunSet;
void sunrise_sunset_init(void)
{
    if (xInit._sunrise_sunset == 0) {
        //SunRise时间数据
        xSunRise.val = 0;
        xSunRise.blink = 0;
        xSunRise.foucs = 1;
        xSunRise.h = 0;
        xSunRise.w = 14;
        xSunRise.cmd[0] = '\0';
        //SunSet时间数据
        xSunSet.val = 0;
        xSunSet.blink = 0;
        xSunSet.foucs = 2;
        xSunSet.h = 1;
        xSunSet.w = 14;
        xSunSet.cmd[0] = '\0';
        xInit._sunrise_sunset = 1;
    }
}

void sunrise_sunset_disp(void)
{
    sunrise_sunset_init();
    if (xUI.cID != (uuzUI_SUNRISE_SUNSET)) {
        ui_id_set(uuzUI_SUNRISE_SUNSET, 4);
        //获取的CH灯光的日升日落数据
        xSunRise.val = get_data(_D_LT, _L_CH1_SUNRISE_DELAY + _CH_LVL(xCache.channel));
        xSunSet.val = get_data(_D_LT, _L_CH1_SUNSET_DELAY + _CH_LVL(xCache.channel));
        //设置焦点
        foucs_set(0);
        //光标位置
        value_set(&xCursor, 0, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawToOLED(0, 1, "RAMP UP  ", 9);
        drawToOLED(1, 1, "RAMP DOWN", 9);
        //填充数据
        sunrise_sunset_value_disp(_OLED_, 2);
        //显示光标
        disp_cursor(_OLED_, &xCursor);
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        if (xUI.blink == 1) {
            //光标闪烁
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void sunrise_sunset_opt(u16 keyId)
{
    switch (keyId) {
        case _key_down:
        xUI.blink = 0;
        if (xUI.cFoucs == 0) {
            //清除旧光标
            drawNull(_OLED_, xCursor.h, xCursor.w, 1);
            xUI.cFoucs = 2;
            //更新光标数据
            sunrise_sunset_cursor_disp();
        } else if (xUI.cFoucs == 1) {
            if (xSunRise.val > 0) {
                xSunRise.val--;
                //显示数据
                sunrise_sunset_value_disp(_OLED_, (xUI.cFoucs - 1) / 2);
            }
        } else if (xUI.cFoucs == 3) {
            if (xSunSet.val > 0) {
                xSunSet.val--;
                //显示数据
                sunrise_sunset_value_disp(_OLED_, (xUI.cFoucs - 1) / 2);
            }
        }
        xUI.blink = 1;
        break;
        case _key_l_down:
        if ((xUI.cFoucs == 1) || (xUI.cFoucs == 3)) {
            sunrise_sunset_opt(_key_down);
        }
        break;
        case _key_up:
        xUI.blink = 0;
        if (xUI.cFoucs == 2) {
            //清除旧光标
            drawNull(_OLED_, xCursor.h, xCursor.w, 1);
            xUI.cFoucs = 0;
            //更新光标数据
            sunrise_sunset_cursor_disp();
        } else if (xUI.cFoucs == 1) {
            if (xSunRise.val < 30) {
                xSunRise.val++;
                sunrise_sunset_value_disp(_OLED_, (xUI.cFoucs - 1) / 2);
            }
        } else if (xUI.cFoucs == 3) {
            if (xSunSet.val < 30) {
                xSunSet.val++;
                sunrise_sunset_value_disp(_OLED_, (xUI.cFoucs - 1) / 2);
            }
        }
        xUI.blink = 1;
        break;
        case _key_l_up:
        if ((xUI.cFoucs == 1) || (xUI.cFoucs == 3)) {
            sunrise_sunset_opt(_key_up);
        }
        break;
        case _key_enter:
        //在工作范围内
        xUI.blink = 0;
        //清除旧光标
        drawNull(_OLED_, xCursor.h, xCursor.w, 1);
        if (xUI.cFoucs % 2 == 0) {
            xUI.cFoucs++;
        } else if (xUI.cFoucs == 1) {
            set_data(_D_LT, _L_CH1_SUNRISE_DELAY + _CH_LVL(xCache.channel), xSunRise.val);
            //保存相关数据
            save_data();
            xUI.cFoucs = 0;
        } else if (xUI.cFoucs == 3) {
            set_data(_D_LT, _L_CH1_SUNSET_DELAY + _CH_LVL(xCache.channel), xSunSet.val);
            //保存相关数据
            save_data();
            xUI.cFoucs = 2;
        }
        sunrise_sunset_cursor_disp();
        xUI.blink = 1;
        break;
        case _key_back:
        show_ui(xUI.cID / 10);
        break;
        default:
        break;
    }
}

void sunrise_sunset_cursor_disp(void)
{
    //更新光标
    if (xUI.cFoucs == 0) {
        value_set(&xCursor, 0, 0);
    } else if (xUI.cFoucs == 1) {
        value_set(&xCursor, 0, 13);
    } else if (xUI.cFoucs == 2) {
        value_set(&xCursor, 1, 0);
    } else if (xUI.cFoucs == 3) {
        value_set(&xCursor, 1, 13);
    }
}

void sunrise_sunset_value_disp(u8 type, u8 step)
{
    char cmd[OLED_W];

    if (xUI.cID == uuzUI_SUNRISE_SUNSET) {
        //显示SUNRISE | SUNSET
        if (step == 0 || step == 2) {
            rt_sprintf(cmd, "%02d", xSunRise.val);
            drawToOLED(xSunRise.h, xSunRise.w, cmd, 2);
        }
        if (step == 1 || step == 2) {
            rt_sprintf(cmd, "%02d", xSunSet.val);
            drawToOLED(xSunSet.h, xSunSet.w, cmd, 2);
        }
    }
}
/* -----------------CHANNEL-1/2 SETUP ECM CONFIGE----------------------*/
void ecm_config_init(void)
{
    if (xInit._ecm_config == 0) {
        xInit._ecm_config = 1;
    }
}

void ecm_config_disp(void)
{
    //初始化界面
    ecm_config_init();
    //显示界面
    if (xUI.cID != (uuzUI_ECM_CONFIG)) {
        ui_id_set(uuzUI_ECM_CONFIG, 2);
        //获取的CH的频道ECM工作模式,数据刚好相反
        foucs_set(get_data(_D_LT, (_L_CH1_ECM_MODE + _CH_LVL(xCache.channel))));
        LOG_I("[%d]ECM CONFIG UI open", xCache.channel);
        value_set(&xCursor, 1, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawToOLED(0, 0, "ECM ON DURING", 13);
        //显示DAY|NIGHT
        drawToOLED(1, 1, cmd_1_1_4[xUI.cFoucs], 8);
        //光标闪烁
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        if (xUI.blink == 1) {
            //光标闪烁
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void ecm_config_opt(u16 keyId)
{
    switch (keyId) {
        case _key_down:
        case _key_up:
        xUI.blink = 0;
        xUI.cFoucs = (xUI.cFoucs == (xUI.maxFoucs - 1)) ? (0) : (1);
        //显示DAY|NIGHT
        drawToOLED(1, 1, cmd_1_1_4[xUI.cFoucs], 8);
        xUI.blink = 1;
        break;
        case _key_enter:
        //在工作范围内
        if (xUI.cFoucs < xUI.maxFoucs) {
            //保存相应数据
            set_data(_D_LT, _L_CH1_ECM_MODE + _CH_LVL(xCache.channel), xUI.cFoucs);
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
/* -----------------CHANNEL-1/2 SETUP DISPLAY MODE----------------------*/
Value_Typedef_t xDispMode;  //光标位
void display_mode_init(void)
{
    if (xInit._display_mode == 0) {
        //亮度等级
        xDispMode.val = 0;
        xDispMode.blink = 0;
        xDispMode.foucs = 0;
        xDispMode.h = 1;
        xDispMode.w = 1;
        xDispMode.cmd[0] = '\0';//NULL
        xInit._display_mode = 1;
    }
}

void display_mode_disp(void)
{
    display_mode_init();
    if (xUI.cID != (uuzUI_DISPLAY_MODE)) {
        ui_id_set(uuzUI_DISPLAY_MODE, 5);
        //获取的CH灯光的显示方式
        xUI.cFoucs = get_data(_D_LT, _L_CH1_DISPLAY_MODE + _CH_LVL(xCache.channel));
        foucs_set(xUI.cFoucs);
        value_set(&xCursor, 1, 0);
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //显示标题
        drawToOLED(0, 0, "DISPLAY W OR %", 14);
        //填充数据
        display_mode_value_disp();
        xUI.delay = 0;
        xUI.blink = 1;
    } else {
        if (xUI.blink == 1) {
            //光标闪烁
            disp_cursor(_OLED_, &xCursor);
        }
    }
}

void display_mode_opt(u16 keyId)
{
    switch (keyId) {
        case _key_down:
        case _key_up:
        xUI.blink = 0;
        if (foucs_move_cycle(keyId)) {
            display_mode_value_disp();
        }
        xUI.blink = 1;
        break;
        case _key_enter:
        //在选项确定
        if (xUI.cFoucs < xUI.maxFoucs) {
            set_data(_D_LT, _L_CH1_DISPLAY_MODE + _CH_LVL(xCache.channel), xUI.cFoucs);
            //保存相关数据
            save_data();
            //返回上一页
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

void display_mode_value_disp(void)
{
    char cmd[OLED_W];
    char watt[4][5] = {"1000", "750 ", "600 ", "400 "};

    if (xUI.cID == uuzUI_DISPLAY_MODE) {
        //显示选择项
        switch (xUI.cFoucs) {
            case 0:  //100%
            xDispMode.val = get_data(_D_LT, _L_CH1_OUTPUT_LEVEL);
            rt_sprintf(cmd, "%03d%", xDispMode.val);
            drawToOLED(xDispMode.h, xDispMode.w, cmd, 4);
            break;
            case 1://1000W
            case 2://750W
            case 3://600W
            case 4://400
            rt_memcpy(cmd, watt[xUI.cFoucs - 1], 4);
            drawToOLED(xDispMode.h, xDispMode.w, cmd, 4);
            break;
            default:
            break;
        }
    }
}
#endif
/* ---------------------------------------------------------------------------*/
