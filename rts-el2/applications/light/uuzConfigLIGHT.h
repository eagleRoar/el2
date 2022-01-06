#ifndef __UUZ_CONFIG_LIGHT_H
#define __UUZ_CONFIG_LIGHT_H

//Light Channel State
enum
{
    uuzLTS_OFF = 0x00U,    //关闭状态
    uuzLTS_ON = 0x01U,    //开启状态
    uuzLTS_RISE = 0x02U,    //日升状态
    uuzLTS_SET = 0x03U,    //日落状态
    uuzLTS_DIM = 0x04U,    //调压状态
    uuzLTS_NULL    //不工作状态
};
//Light Channel Sign
#define uuzLIGHT_DELAY_WAIT (0x00U)
#define uuzLIGHT_DELAY_OFF (0x01U)
#define uuzLIGHT_DELAY_ON (0x02U)
//LIght Alarm Sign
#define uuzLIGHT_ALM_NULL (0x00U)
#define uuzLIGHT_ALM_DIM (0x01U)
#define uuzLIGHT_ALM_OFF (0x02U)
#define uuzLIGHT_ALM_CLR (0x03U)    //消除报警数据

//Channel-1/2 Work Mode
#define uuzLTM_OFF (0x00U)
#define uuzLTM_ON (0x01U)
#define uuzLTM_AUTO (0x02U)

//Channel-2  Mode
#define uuzLIGHT_FOLLOW (0x00U)
#define uuzLIGHT_INVERSE (0x01U)
#define uuzLIGHT_INDEPENDENT (0x02U)

//单次报警次数，持续5秒为报警状态
//200ms * 25 = 5sec
#define uuzALM_MAX (5U)
//阶段灯光数据(1%)
#define uuzLIGHT_STEP_VAL (1U)
//调节最大最小值(10%~115%)
#define uuzLIGHT_VAL_OFF (0U)
#define uuzLIGHT_VAL_MIN (30U)
#define uuzLIGHT_VAL_DIM (50U)
#define uuzLIGHT_VAL_MAX (115U)

#endif // __UUZ_CONFIG_LIGHT_H
