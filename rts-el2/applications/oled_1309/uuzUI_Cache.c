/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
/* ------------------------- package ----------------------------------------*/
#include "typedefDEF.h"
#include "typedefUI.h"
#include "uuzConfig1309.h"
#include "uuzUI_Cache.h"
/* -------------------------------------------------------------------------------*/
//一级设备标题
char cmd_1[8][14] = {
    //1级菜单
    "LOGS", //1级菜单:日志查看
    "CHANNEL 1", //1级菜单:通道1配置
    "CHANNEL 2", //1级菜单:通道2配置
    "TEMP PROBE", //1级菜单:温度配置
    "SYSTEM TIME", //1级菜单:时间设置
    "LANGUAGE", //1级菜单:语言设置
    "SYSTEM INFO", //1级菜单:系统信息
    "FACTORY RESET", //1级菜单:恢复出厂设置
};

char cmd_1_1[7][16] = {
    //2级菜单
    "OUTPUT MODE", //2级菜单:输出模式
    "OUTPUT LEVEL", //2级菜单:输出等级
    "LIGHT CYCLE", //2级菜单:灯光控制模式
    "SET TEMP LEVELS", //2级菜单:设置温度相关参数
    "SUNRISE/SET", //2级菜单:设置日升日落的相关时间
    "ECM CONFIG", //2级菜单:ECM配置
    "DISPLAY MODE", //2级菜单:显示单位 (W or %)
};

char cmd_1_1_1[3][14] = {
    //3级菜单
    "AUTO", //3级菜单:自动模式
    "ON", //3级菜单:开启模式
    "OFF", //3级菜单:关闭模式
};

char cmd_1_1_3[2][14] = {
    //3级菜单
    "ON       ", //3级菜单:开启时间
    "OFF      ", //3级菜单:关闭时间
};

char cmd_1_1_4[2][18] = {
    //3级菜单
    "RAMP UP    ", //3级菜单:SunRise 时间
    "RAMP DOWN  ", //3级菜单:SunSet 时间
};

char cmd_1_1_5[2][12] = {
    //3级菜单
    "DAY ", //3级菜单:开启时间
    "NIGHT ", //3级菜单:关闭时间
};

char cmd_1_1_6[5][20] = {
    //3级菜单
    "400W BALLASTS ", //3级菜单:400W
    "600W BALLASTS ", //3级菜单:600W
    "750W BALLASTS ", //3级菜单:750W
    "1000W BALLASTS ", //3级菜单:1000W
    "100% ", //3级菜单:百分比显示
};

char cmd_1_2_1[3][18] = {
    //3级菜单
    "FOLLOW MAIN", //3级菜单:跟随模式
    "INDEPENDENT", //3级菜单:独立模式
    "INVERSE", //3级菜单:互斥模式
};

char cmd_1_3[2][14] = {
    //2级菜单
    "TEMP UNITS", //2级菜单:温度单位
    "CALIBRATE", //2级菜单:校正按键
};

char cmd_1_5[4][16] = {
    //2级菜单
    "ENGLISH", //2级菜单:英文
    "NEDERLANDS", //2级菜单:荷兰语
    "FRANCAIS", //2级菜单:法语
    "DEUTSCH", //2级菜单:德语
};

char cmd_1_7[2][10] = {
    //2级菜单
    "YES", //2级菜单:确认
    "NO", //2级菜单:取消
};

//-----------------------------------------------------------------//
//定义默认光标显示
Value_Typedef_t xCursor;
