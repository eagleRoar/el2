#ifndef __TYPEDEF_UI_H
#define __TYPEDEF_UI_H

#include "typedefDEF.h"
#include "typedefUI.h"
#include "uuzConfigLIGHT.h"

//通道标记
#define _CHANNEL_1  (0x00U)     //通道1
#define _CHANNEL_2  (0x01U)     //通道2

//按下状态
#define _PRESS_UP  (0x00U)     //没有按下状态
#define _PRESS_DOWN  (0x01U)     //按下状态
#define _PRESS_MODIFY  (0x02U)     //修改状态

//亮度相关
#define _LIGHT_MIN  (uuzLIGHT_VAL_MIN)       //亮度最小值
#define _LIGHT_MAX  (115U)      //亮度最大值
#define _LIGHT_STEP  (1U)       //亮度单步值

//ECM相关
#define _LIGHT_ECM_DAY  (0U)      //对应白天
#define _LIGHT_ECM_NIGHT (1U)       //对应晚上

//灯光模式
#define _LIGHT_MODE_OFF (0U)     //灯光关闭模式
#define _LIGHT_MODE_ON (1U)      //灯光开启模式
#define _LIGHT_MODE_AUTO (2U)    //灯光自动模式

//延时界面循环时间
#define _UI_DELAY_TIME  (250U)  //250ms
#define _UI_DELAY_SEC   (1000U/(2 * _UI_DELAY_TIME))
#define _UI_AUTO_BACK_SEC  (30U)   //30秒自动返回

typedef struct __value_typedef_t
{
    u16 val;        //当前的值
    u8 blink;       //闪烁
    u16 foucs;      //当前焦点值
    u8 w;           //宽边值
    u8 h;           //竖边值
    char cmd[16];    //显示值
} Value_Typedef_t;
#define uuzVALUE_LEN (sizeof(Value_Typedef_t))

typedef struct __ui_typedef_t
{
    //初始化状态
    //ID编号
    u16 cID;
    u16 pID;
    //焦点编号
    u16 maxFoucs;
    u16 cFoucs;
    u16 pFoucs;
    //界面切换延时
    u32 delay;
    //按下标记
    u8 press;
    u8 blink;       //开启光标闪烁
    u8 pSign;       //闪烁的位置0,1,2,3

} UI_Cache_Typedef_t;

//结构体长度
#define uuzUI_LEN (sizeof(UI_Cache_Typedef_t))

#endif // __TYPEDEF_UI_H
