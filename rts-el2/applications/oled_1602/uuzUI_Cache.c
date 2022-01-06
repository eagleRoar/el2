/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
/* ------------------------- package ----------------------------------------*/
#include "typedefDEF.h"
#include "typedefUI.h"
#include "uuzConfig1602.h"
#include "uuzUI_Cache.h"
/* -------------------------------------------------------------------------------*/
char cmd_1[MENU_LIST_MAX][OLED_W] =
    {
    //1级菜单
            "LOGS         ",//1级菜单:日志查看
            "CHANNEL 1    ",    //1级菜单:通道1配置
            "CHANNEL 2    ",    //1级菜单:通道2配置
            "TEMP PROBE   ",    //1级菜单:温度配置
            "CH2 MODE     ",    //1级菜单:线路配置
            "SYSTEM TIME  ",    //1级菜单:时间设置
            "LANGUAGE     ",    //1级菜单:语言设置
            "INFO         ",    //1级菜单:系统信息
            "FACTORY RESET",    //1级菜单:恢复出厂设置
        };

//2级设备标题(YES/NO)
char cmd_y_n[2][OLED_W] =
    {
    //2级菜单
            "NO ",//2级菜单:NO
            "YES",    //2级菜单:YES
        };

//2级设备标题
char cmd_1_0[LOGS_LIST_MAX][OLED_W] =
    {
    //2级菜单
            "LAST AUTO-DIM C1",//2级菜单:CH-1频道自动调光数据
            "LAST AUTO-DIM C2",     //2级菜单:CH-2频道自动调光数据
            "LAST SHUTDOWN C1",     //2级菜单:CH-1频道自动关闭数据
            "LAST SHUTDOWN C2",     //2级菜单:CH-2频道自动关闭数据
            "LAST POWER LOSS ",     //2级菜单:自动关机记录
            "CHANNEL 1 LOG   ",     //2级菜单:CH-1最高温和最低温记录
            "CHANNEL 2 LOG   ",     //2级菜单:CH-2最高温和最低温记录
        };

//2级设备标题
char cmd_1_1[LIGHT_LIST_MAX][OLED_W] =
    {
    //2级菜单
            "OUTPUT MODE    ",//2级菜单:输出模式
            "OUTPUT LEVEL   ",    //2级菜单:输出等级
            "LIGHT CYCLE    ",    //2级菜单:灯光控制模式
            "SET TEMP LEVELS",    //2级菜单:设置温度相关参数
            "SUNRISE/SUNSET ",    //2级菜单:设置日升日落的相关时间
            "ECM CONFIG     ",    //2级菜单:ECM配置
            "DISPLAY MODE   ",    //2级菜单:显示单位 (W or %)
        };

//2级设备标题
char cmd_1_4[3][OLED_W] =
    {
    //2级菜单
            "FOLLOW MAIN ",//2级菜单：跟随模式
            "INVERSE     ",    //2级菜单:相反模式
            "INDEPENDENT ",    //2级菜单:独立模式
        };

//2级设备标题
char cmd_1_6[5][OLED_W] =
    {
    //2级菜单
            "ENGLISH   ",//2级菜单：英语
            "ESPANOL   ",    //2级菜单:西班牙语
            "DEUTSCH   ",    //2级菜单:德语
            "FRANCAIS  ",    //2级菜单:法语
            "NEDERLANDS",    //2级菜单:荷兰语
        };

//3级设备标题(output mode)
char cmd_1_1_1[3][OLED_W] =
    {
    //3级菜单
            "OFF ",//3级菜单:关闭模式
            "ON  ",    //3级菜单:开启模式
            "AUTO",    //3级菜单:自动模式
        };

//3级设备标题(set temp levels)
char cmd_1_1_4[4][OLED_W] =
    {
    //3级菜单
            "NIGHT   ",//3级菜单:夜晚温度模式
            "DAY     ",    //3级菜单:白天温度模式
            "AUTO-DIM",    //3级菜单:自动调温模式
            "SHUTDOWN",    //3级菜单:自动关闭模式
        };

//-----------------------------------------------------------------//
//定义默认光标显示
Value_Typedef_t xCursor;
