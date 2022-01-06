#ifndef __UUZ_UI_CACHE_H
#define __UUZ_UI_CACHE_H
/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
/* ------------------------- package ----------------------------------------*/
#include "typedefDEF.h"
#include "uuzConfig1309.h"
/* -------------------------------------------------------------------------------*/
#define MENU_LIST_MAX (9U) //EL-2最大栏数
//#define LIGHT_LIST_MAX (6U)//EL-1最大栏数
#define LIGHT_LIST_MAX (7U) //EL-2最大栏数
//#define LOGS_LIST_MAX (4U)//EL-1的LOGS最大栏数
#define LOGS_LIST_MAX (7U) //EL-2的LOGS最大栏数

/* -------------------------------------------------------------------------------*/
extern Value_Typedef_t xCursor;

#ifdef __cplusplus
extern "C" {
#endif
char cmd_1[8][14];
char cmd_1_1[7][16];
char cmd_1_1_1[3][14];
char cmd_1_1_3[2][14];
char cmd_1_1_4[2][18];
char cmd_1_1_5[2][12];
char cmd_1_1_6[5][20];
char cmd_1_2_1[3][18];
char cmd_1_3[2][14];
char cmd_1_5[4][16];
char cmd_1_7[2][10];

#ifdef __cplusplus
}
#endif
#endif // __UUZ_UI_CACHE_H
