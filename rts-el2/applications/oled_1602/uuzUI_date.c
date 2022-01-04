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
#include "uuzCalendar.h"
/* -------------------------------------------------------------------------------*/
#include "uuzUI.h"
#include "typedefUI.h"
#include "uuzOLED_1602.h"
#include "uuzUI_1602.h"
#include "uuzConfig1602.h"
#include "uuzUI_Cache.h"
/* -------------------------------------------------------------------------------*/

#define DBG_ENABLE
#define DBG_SECTION_NAME "UI|T"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

/* -----------------------------TIME SETUP -----------------------------------*/
Value_Typedef_t xTime;          //foucs:0
Value_Typedef_t xHour;          //foucs:1
Value_Typedef_t xMin;           //foucs:2
Value_Typedef_t xTitle;         //foucs:3
Value_Typedef_t xYear;          //foucs:4
Value_Typedef_t xMonth;         //foucs:5
Value_Typedef_t xDay;           //foucs:6
extern struct rt_messagequeue mq;//Justin debug
void time_init(void)
{
    if (xInit._menu_time == 0) {
        //时制:0-24H;1-12H
        xTime.val = 0;
        xTime.blink = 1;
        xTime.foucs = 0;
        xTime.h = 0;
        xTime.w = 0;
        xTime.cmd[0] = ' ';    //""
        //小时
        xHour.val = 0;
        xHour.blink = 1;
        xHour.foucs = 1;
        xHour.h = 1;
        xHour.w = 0;
        xHour.cmd[0] = '\0';    //""
        //分钟
        xMin.val = 0;
        xMin.blink = 1;
        xMin.foucs = 2;
        xMin.h = 1;
        xMin.w = 3;
        xMin.cmd[0] = '\0';    //" "
        //Date Format
        xTitle.val = 0;
        xTitle.blink = 1;
        xTitle.foucs = 3;
        xTitle.h = 0;
        xTitle.w = 0;
        strcpy(xTitle.cmd, "MM/DD/YY");    //"MM-DD-YY"
        //Year
        xYear.val = 0;
        xYear.blink = 1;
        xYear.foucs = 4;
        xYear.h = 1;
        xYear.w = 6;
        xYear.cmd[0] = '\0';    //""
        //Month
        xMonth.val = 0;
        xMonth.blink = 1;
        xMonth.foucs = 5;
        xMonth.h = 1;
        xMonth.w = 0;
        xMonth.cmd[0] = '\0';    //""
        //Day
        xDay.val = 0;
        xDay.blink = 1;
        xDay.foucs = 6;
        xDay.h = 1;
        xDay.w = 3;
        xDay.cmd[0] = '\0';    //""
        //完成标记
        xInit._menu_time = 1;
    }
}

void time_disp(void)
{
    u8 buffer = 0;
    rt_err_t ret = RT_EOK;//Justin debug
    //初始化设置
    time_init();

    ret = rt_mq_recv(&mq, &buffer, sizeof(buffer), RT_WAITING_FOREVER);//Justin debug
    if(ret != RT_EOK)
    {
        LOG_E("receive messagequeue failed");
    }

    if (xUI.cID != uuzUI_MENU_TIME) {
        ui_id_set(uuzUI_MENU_TIME, 7);
        //如果没有设置过日期
        if (xSysConfig.Sys[_D_INIT_STATE] == 0x00U) {
            //初始化相关设备
            //Time
            //24-HOUR
            xTime.val = _TYPE_24HOUR;
            xTime.cmd[0] = ' ';
            xHour.val = buffer;//8;//Justin debug
            xMin.val = 0;
            //Date-Title
            xTitle.val = _TYPE_MMDDYY;
            xYear.val = 20;
            xMonth.val = 1;
            xDay.val = 1;
            //MM-DD-YY
            value_set(&xMonth, 1, 0);
            value_set(&xDay, 1, 3);
        } else {
            //读取当前时间到rTm
            //获取时间
            xTime.val = get_data(_D_SYS, _D_TIME_FORMAT);
            xHour.val = rTm.tm_hour;
            xMin.val = rTm.tm_min;
            //12/24小时制
            if (xTime.val == _TYPE_12HOUR) {
                if (xHour.val > 12) {
                    xTime.cmd[0] = 'P';
                    xHour.val %= 12;
                } else {
                    xTime.cmd[0] = 'A';
                }
            } else {
                //获取时间
                if (xTime.cmd[0] == 'P') {
                    xHour.val += 12;
                }
                xTime.cmd[0] = ' ';
            }
            //显示年份格式
            xTitle.val = get_data(_D_SYS, _D_DATE_FORMAT);
            //年
            xYear.val = (rTm.tm_year + 1900) % 100;
            //月
            xMonth.val = rTm.tm_mon + 1;
            //天
            xDay.val = rTm.tm_mday;
            //年份格式（DD-MM-YY|MM-DD-YY)坐标设置
            if (xTitle.val == _TYPE_DDMMYY) {
                strcpy(xTitle.cmd, "DD/MM/YY");
                value_set(&xMonth, 1, 3);
                value_set(&xDay, 1, 0);
            } else {
                strcpy(xTitle.cmd, "MM/DD/YY");
                value_set(&xMonth, 1, 0);
                value_set(&xDay, 1, 3);
            }
        }
        foucs_set(0);
        LOG_I("Time Setup UI open");
        // 清空OLED屏幕
        clear_screen(_SYNC_A);
        //更新时间相关数据
        time_set_disp(3);
        xUI.delay = 0;
        xUI.blink = 1;
        //LOG_I("Setup:[%04d-%02d-%02d %02d:%02d]", xYear.val, xMonth.val, xDay.val, xHour.val, xMin.val);
    } else {
        //更新相关数据
        if (xUI.blink == 1) {
            if (xUI.cFoucs < 3) {
                time_set_disp(xUI.cFoucs);
            } else {
                date_set_disp(xUI.cFoucs);
            }
        }
    }
}

void time_opt(u16 keyId)
{
    u8 ucIsSaved = 0;

    switch (keyId)
    {
        case _key_l_up:
            time_opt(_key_up);
            break;
        case _key_up:
            xUI.blink = 0;
            if (xUI.cFoucs == xTime.foucs) {
                //24H/12H
                xTime.val = (xTime.val == _TYPE_24HOUR) ? (_TYPE_12HOUR) : (_TYPE_24HOUR);
                //刷新数据
                if (xTime.val == _TYPE_12HOUR) {
                    //判断小时时间
                    if (xHour.val > 12) {
                        xTime.cmd[0] = 'P';
                        xHour.val %= 12;
                    } else {
                        xTime.cmd[0] = 'A';
                    }
                } else {
                    //显示24-HOUR
                    //判断小时时间
                    if (xTime.cmd[0] == 'P') {
                        xHour.val += 12;
                        xTime.cmd[0] = ' ';
                    }
                }
            } else if (xUI.cFoucs == xHour.foucs) {
                //Time: hour
                if (xTime.val == _TYPE_12HOUR) {
                    if (xTime.cmd[0] == 'A') {
                        //上午切换成下午
                        if (xHour.val == 12) {
                            xHour.val = 1;
                            xTime.cmd[0] = 'P';
                        } else {
                            xHour.val++;
                        }
                    } else if (xTime.cmd[0] == 'P') {
                        //下午切换成上午
                        if (xHour.val == 11) {
                            xHour.val = 0;
                            xTime.cmd[0] = 'A';
                        } else {
                            xHour.val++;
                        }
                    }
                } else {
                    if (xHour.val == 23) {
                        xHour.val = 0;
                    } else {
                        xHour.val++;
                    }
                }
            } else if (xUI.cFoucs == xMin.foucs) {
                //Time: min
                if (xMin.val == 59) {
                    xMin.val = 0;
                } else {
                    xMin.val++;
                }
            } else if (xUI.cFoucs == xTitle.foucs) {
                //Date Format
                //修改临时数据
                xTitle.val = (xTitle.val == _TYPE_MMDDYY) ? (_TYPE_DDMMYY) : (_TYPE_MMDDYY);
                //更新界面布局
                if (xTitle.val == _TYPE_MMDDYY) {
                    strcpy(xTitle.cmd, "MM/DD/YY");
                    value_set(&xMonth, 1, 0);
                    value_set(&xDay, 1, 3);
                } else {
                    strcpy(xTitle.cmd, "DD/MM/YY");
                    value_set(&xMonth, 1, 3);
                    value_set(&xDay, 1, 0);
                }
            } else if (xUI.cFoucs == 4) {
                //Year
                //Max Year = 2038
                if (xYear.val == 38) {
                    xYear.val = 0;    //2000
                } else {
                    xYear.val++;
                }
            } else if (xUI.cFoucs == 5) {
                //Month
                //修改临时数据(0~12)
                if (xMonth.val == 12) {
                    xMonth.val = 1;
                } else {
                    xMonth.val++;
                }
            } else if (xUI.cFoucs == 6) {
                //Day
                //修改临时数据(0~31)
                if (xDay.val >= monthDays((xYear.val + 2000), xMonth.val)) {
                    xDay.val = 1;
                } else {
                    xDay.val++;
                }
            }

            //显示界面数据
            if (xUI.cFoucs < 3) {
                time_set_disp(xUI.cFoucs);
            } else {
                date_set_disp(xUI.cFoucs);
            }
            xUI.blink = 1;
            break;
        case _key_l_down:
            time_opt(_key_down);
            break;
        case _key_down:
            xUI.blink = 0;
            if (xUI.cFoucs == xTime.foucs) {
                //24H/12H
                xTime.val = (xTime.val == _TYPE_24HOUR) ? (_TYPE_12HOUR) : (_TYPE_24HOUR);
                //刷新数据
                if (xTime.val == _TYPE_12HOUR) {
                    //判断小时时间
                    if (xHour.val > 12) {
                        xTime.cmd[0] = 'P';
                        xHour.val %= 12;
                    } else {
                        xTime.cmd[0] = 'A';
                    }

                } else {
                    //显示24-HOUR
                    //判断小时时间
                    if (xTime.cmd[0] == 'P') {
                        xHour.val += 12;
                        xTime.cmd[0] = ' ';
                    }
                }
            } else if (xUI.cFoucs == xHour.foucs) {
                //Time: hour
                if (xTime.val == _TYPE_12HOUR) {
                    if (xTime.cmd[0] == 'A') {
                        //上午切换成下午
                        if (xHour.val == 0) {
                            xHour.val = 11;
                            xTime.cmd[0] = 'P';
                        } else {
                            xHour.val--;
                        }
                    } else if (xTime.cmd[0] == 'P') {
                        //下午切换成上午
                        if (xHour.val == 1) {
                            xHour.val = 12;
                            xTime.cmd[0] = 'A';
                        } else {
                            xHour.val--;
                        }
                    }
                } else {
                    if (xHour.val == 0) {
                        xHour.val = 23;
                    } else {
                        xHour.val--;
                    }
                }
            } else if (xUI.cFoucs == xMin.foucs) {
                //Time: min
                if (xMin.val == 0) {
                    xMin.val = 59;
                } else {
                    xMin.val--;
                }
            } else if (xUI.cFoucs == xTitle.foucs) {
                //Date Format
                //修改临时数据
                xTitle.val = (xTitle.val == _TYPE_MMDDYY) ? (_TYPE_DDMMYY) : (_TYPE_MMDDYY);
                //更新界面布局
                if (xTitle.val == _TYPE_MMDDYY) {
                    strcpy(xTitle.cmd, "MM/DD/YY");
                    value_set(&xMonth, 1, 0);
                    value_set(&xDay, 1, 3);
                } else {
                    strcpy(xTitle.cmd, "DD/MM/YY");
                    value_set(&xMonth, 1, 3);
                    value_set(&xDay, 1, 0);
                }
            } else if (xUI.cFoucs == xYear.foucs) {
                //Year
                //Max Year = 2038
                if (xYear.val == 0) {
                    xYear.val = 38;    //2000
                } else {
                    xYear.val--;
                }
            } else if (xUI.cFoucs == xMonth.foucs) {
                //Month
                //修改临时数据(1~12)
                if (xMonth.val == 1) {
                    xMonth.val = 12;
                } else {
                    xMonth.val--;
                }
            } else if (xUI.cFoucs == xDay.foucs) {
                //Day
                //修改临时数据(1~31)
                if (xDay.val == 1) {
                    xDay.val = monthDays((xYear.val + 2000), xMonth.val);
                } else {
                    xDay.val--;
                }
            }

            //显示界面数据
            if (xUI.cFoucs < 3) {
                time_set_disp(xUI.cFoucs);
            } else {
                date_set_disp(xUI.cFoucs);
            }
            xUI.blink = 1;
            break;
        case _key_enter:
            if (xUI.cFoucs < (xUI.maxFoucs - 1)) {
                xUI.cFoucs++;
                //切换焦点
                foucs_set(xUI.cFoucs);
                //刷新屏幕数据
                if (xUI.cFoucs < 3) {
                    time_set_disp(3);
                } else {
                    date_set_disp(7);
                }
            } else {
                ucIsSaved = 0;
                //更新保存时间数据
                //设置时间
                if ((xTime.val == _TYPE_12HOUR) && (xTime.cmd[0] == 'P')) {
                    //在12进制的下午，刷新相关数据
                    set_rtc((xYear.val + 2000), xMonth.val, xDay.val, xHour.val + 12, xMin.val, 0);
                } else {
                    set_rtc((xYear.val + 2000), xMonth.val, xDay.val, xHour.val, xMin.val, 0);
                }
                //设置时制格式
                if (xTime.val != get_data(_D_SYS, _D_TIME_FORMAT)) {
                    set_data(_D_SYS, _D_TIME_FORMAT, xTime.val);
                    ucIsSaved = 1;
                }
                //设置日期显示格式
                if (xTitle.val != get_data(_D_SYS, _D_DATE_FORMAT)) {
                    set_data(_D_SYS, _D_DATE_FORMAT, xTitle.val);
                    ucIsSaved = 1;
                }
                if (xSysConfig.Sys[_D_INIT_STATE] == 0x00) {
                    //如果是初次设置
                    set_data(_D_SYS, _D_INIT_STATE, 0x01);
                    ucIsSaved = 1;
                    show_ui(uuzUI_SHOW_CH);
                } else {
                    ucIsSaved = 1;
                    show_ui(uuzUI_MENU);
                }
                if (ucIsSaved == 1) {
                    /* 保存数据FLASH*/
                    save_data();
                }
            }
            break;
        case _key_back:
            //未完成设置立刻退出
            if (xSysConfig.Sys[_D_INIT_STATE] == 0x00) {
                show_ui(uuzUI_SHOW_CH);
            } else {
                show_ui(uuzUI_MENU);
            }
            break;
        default:
            break;
    }
}

/**
 *
 * @param type: 0:刷新时制;1:刷新时;2:刷新分钟;3:全部刷新
 */
void time_set_disp(u8 type)
{
    char cmd[OLED_W];
    u8 IsShow = 1;

    //刷新时间数据
    if (xUI.cID == uuzUI_MENU_TIME) {
        // 设置标题
        if (type == 3) {
            //显示标题
            drawToOLED(0, 3, "HOUR", 4);
            drawToOLED(1, 2, ":", 1);
        }

        //显示时制相关数据/显示小时数据
        if (type == xTime.foucs || type == xHour.foucs || type == 3) {
            //显示12-HOUR/24-HOUR
            //显示A/P尾坠
            if (xTime.val == _TYPE_12HOUR) {
                drawToOLED(xMin.h, xMin.w + 2, xTime.cmd, 1);
            } else {
                drawToOLED(xMin.h, xMin.w + 2, " ", 1);
            }
            IsShow = 1;
            //显示闪烁信息
            if (xTime.blink == 1 && xTime.foucs == xUI.cFoucs) {
                if (xUI.delay % 2 == 1) {
                    drawToOLED(xTime.h, xTime.w, "  ", 2);
                    IsShow = 0;
                }
            }

            if (IsShow) {
                rt_sprintf(cmd, "%02d", xTime.val);
                drawToOLED(xTime.h, xTime.w, cmd, 2);
            }

            IsShow = 1;
            //显示闪烁信息
            if (xHour.blink == 1 && xHour.foucs == xUI.cFoucs) {
                if (xUI.delay % 2 == 1) {
                    drawToOLED(xHour.h, xHour.w, "  ", 2);
                    IsShow = 0;
                }
            }

            if (IsShow) {
                rt_sprintf(cmd, "%02d", xHour.val);
                drawToOLED(xHour.h, xHour.w, cmd, 2);
            }
        }

        //显示分钟数据
        if (type == xMin.foucs || type == 3) {
            IsShow = 1;
            //显示闪烁信息
            if (xMin.blink == 1 && xMin.foucs == xUI.cFoucs) {
                if (xUI.delay % 2 == 1) {
                    drawToOLED(xMin.h, xMin.w, "  ", 2);
                    IsShow = 0;
                }
            }

            if (IsShow) {
                rt_sprintf(cmd, "%02d", xMin.val);
                drawToOLED(xMin.h, xMin.w, cmd, 2);
            }
        }
    }
}

/**
 *
 * @param type: 3:刷新日期制;4:刷新年;5:刷新月;6:刷新日;7:全部刷新
 */
void date_set_disp(u8 type)
{
    char cmd[OLED_W];
    u8 IsShow = 1;

    //刷新时间数据
    if (xUI.cID == uuzUI_MENU_TIME) {
        // 设置标题
        if (type == 7) {
            //显示标题
            drawToOLED(0, 9, "FORMAT", 6);
            drawToOLED(1, 2, "/", 1);
            drawToOLED(1, 5, "/", 1);
        }

        //显示时制相关数据
        if (type == xTitle.foucs || type == 7) {
            IsShow = 1;
            //显示闪烁信息
            if (xTitle.blink == 1 && xTitle.foucs == xUI.cFoucs) {
                if (xUI.delay % 2 == 1) {
                    drawToOLED(xTitle.h, xTitle.w, "        ", 8);
                    IsShow = 0;
                }
            }

            if (IsShow) {
                drawToOLED(xTitle.h, xTitle.w, xTitle.cmd, 8);
            }
        }

        //显示Year数据
        if (type == xYear.foucs || type == 7) {
            IsShow = 1;
            //显示闪烁信息
            if (xYear.blink == 1 && xYear.foucs == xUI.cFoucs) {
                if (xUI.delay % 2 == 1) {
                    drawToOLED(xYear.h, xYear.w, "  ", 2);
                    IsShow = 0;
                }
            }

            if (IsShow) {
                rt_sprintf(cmd, "%02d", xYear.val);
                drawToOLED(xYear.h, xYear.w, cmd, 2);
            }
        }

        //显示Month数据
        if (type == xTitle.foucs || type == xMonth.foucs || type == 7) {
            IsShow = 1;
            //显示闪烁信息
            if (xMonth.blink == 1 && xMonth.foucs == xUI.cFoucs) {
                if (xUI.delay % 2 == 1) {
                    drawToOLED(xMonth.h, xMonth.w, "  ", 2);
                    IsShow = 0;
                }
            }

            if (IsShow) {
                rt_sprintf(cmd, "%02d", xMonth.val);
                drawToOLED(xMonth.h, xMonth.w, cmd, 2);
            }
        }

        //显示Day数据
        if (type == xTitle.foucs || type == xDay.foucs || type == 7) {
            IsShow = 1;
            //显示闪烁信息
            if (xDay.blink == 1 && xDay.foucs == xUI.cFoucs) {
                if (xUI.delay % 2 == 1) {
                    drawToOLED(xDay.h, xDay.w, "  ", 2);
                    IsShow = 0;
                }
            }

            if (IsShow) {
                rt_sprintf(cmd, "%02d", xDay.val);
                drawToOLED(xDay.h, xDay.w, cmd, 2);
            }
        }
    }
}
/* ---------------------------------------------------------------------------*/
