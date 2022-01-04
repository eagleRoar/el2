#ifndef __UUZ_UI_CACHE_H
#define __UUZ_UI_CACHE_H
/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
/* ------------------------- package ----------------------------------------*/
#include "typedefDEF.h"
#include "uuzConfig1602.h"
/* -------------------------------------------------------------------------------*/
#define MENU_LIST_MAX (9U)//EL-2最大栏数
//#define LIGHT_LIST_MAX (6U)//EL-1最大栏数
#define LIGHT_LIST_MAX (7U)//EL-2最大栏数
//#define LOGS_LIST_MAX (4U)//EL-1的LOGS最大栏数
#define LOGS_LIST_MAX (7U)//EL-2的LOGS最大栏数

/* -------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
char cmd_y_n[2][OLED_W];
char cmd_1[MENU_LIST_MAX][OLED_W];
char cmd_1_0[LOGS_LIST_MAX][OLED_W];
char cmd_1_1[LIGHT_LIST_MAX][OLED_W];
char cmd_1_4[3][OLED_W];
char cmd_1_6[5][OLED_W];
char cmd_1_1_1[3][OLED_W];
char cmd_1_1_4[4][OLED_W];

Value_Typedef_t xCursor;
#ifdef __cplusplus
}
#endif
#endif // __UUZ_UI_CACHE_H
