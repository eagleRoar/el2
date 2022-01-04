/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include "string.h"
/* ------------------------- package ----------------------------------------*/
#include "drv_flash.h"
#include "agile_button.h"
/* -------------------------------------------------------------------------------*/
#include "uuzINIT.h"
#include "uuzUI.h"
#include "typedefDEF.h"
#include "uuzBTN.h"
/* -------------------------------------------------------------------------------*/
#include "uuzTEMP.h"
#include "uuzDAC.h"
/* -------------------------------------------------------------------------------*/
#include "typedefUI.h"
#include "uuzUI.h"
#include "uuzConfigLIGHT.h"
/* -------------------------------------------------------------------------------*/

#define DBG_ENABLE
#define DBG_SECTION_NAME " UI "
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

UI_Cache_Typedef_t xUI;

/**
 * @note 设置界面ID数据
 * @param id
 */
void ui_id_set(u16 id, u8 max)
{
    if (xUI.cID != id) {
        //保存上一个界面数据
        xUI.pID = xUI.cID;
        //重新定义界面数据
        xUI.cID = id;
        //本界面全部焦点数量
        xUI.maxFoucs = max;
    }
}

/**
 * @note 设置界面焦点
 * @param foucs
 */
void foucs_set(u8 foucs)
{
    if (xUI.cFoucs != foucs) {
        xUI.pFoucs = xUI.cFoucs;
        xUI.cFoucs = foucs;
    }
}

/**
 * @note 向上或者向下移动焦点
 * @param key
 */
void foucs_move(u16 key)
{
    if (xUI.maxFoucs) {
        u8 foucs = xUI.cFoucs;
        if ((key == _key_down) || (key == _key_l_down)) {
            //如果是向下移动
            if (foucs < (xUI.maxFoucs - 1)) {
                foucs++;
            }
            foucs_set(foucs);
        } else if ((key == _key_up) || (key == _key_l_up)) {
            //如果是向上移动
            if (foucs > 0) {
                foucs--;
            }
            foucs_set(foucs);
        }
    }
}

/**
 * @note 向上或者向下移动焦点
 * @param key
 * @return 返回是否执行数据
 */
u8 foucs_move_cycle(u16 key)
{
    u8 isMove = 0;
    if (xUI.maxFoucs) {
        u8 foucs = xUI.cFoucs;
        if ((key == _key_down) || (key == _key_l_down)) {
            //如果是向下移动
            if (foucs < (xUI.maxFoucs - 1)) {
                foucs++;
                foucs_set(foucs);
                isMove = 1;
            }
        } else if ((key == _key_up) || (key == _key_l_up)) {
            //如果是向上移动
            if (foucs > 0) {
                foucs--;
                foucs_set(foucs);
                isMove = 1;
            }
        }
    }

    return isMove;
}

/**
 * @brief 焦点单向移动
 * @param key
 */
void foucs_single_move(u16 key)
{
    u8 foucs = xUI.cFoucs;
    if (xUI.maxFoucs) {
        if (key == _key_enter) {
            //如果是向下移动
            if (foucs < xUI.maxFoucs) {
                foucs++;
            }
            foucs_set(foucs);
        }
    }
}

/**
 * @note 界面同步线程初始化
 */
void ui_sync_init(void)
{
    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("ui sync", ui_thread_entry,
    RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
    }
}

/**
 * @brief 将强度数据转换成瓦数显示
 * @param level
 * @param mode
 * @return
 */
u16 watt_get(u16 level, u8 mode)
{
    u16 watt = 0;
    //计算使用的瓦数
    switch (mode)
    {
        case 1:    //1000W
            watt = 1000 * level / 100;
            break;
        case 2:    //750W
            watt = 750 * level / 100;
            break;
        case 3:    //600W
            watt = 600 * level / 100;
            break;
        case 4:    //400W
            watt = 400 * level / 100;
            break;
        default:    //%
            watt = 0;
            break;
    }

    return watt;
}

/**
 * @brief 获取灯光相关缓存信息
 * @param ch
 * @param type
 * @return
 */
u16 level_state_get(u16 ch, u8 type)
{
    u16 watt = 0;

    //type:0-读取实时数据;1-更新修改数据
    if (ch == _CHANNEL_1 || ch == _CHANNEL_2) {
        if (type == 0) {
            xCache.level_M[ch] = get_data(_D_LT, _L_CH1_OUTPUT_MODE + _CH_LVL(ch));
            xCache.level_D[ch] = get_data(_D_LT, _L_CH1_DISPLAY_MODE + _CH_LVL(ch));
            //获取设备当前值
            if (xSta[_S_CH1_DAC_OPT + _CH_DIS(ch)] == 1) {
                xCache.level_L[ch] = dac_to_level(ch, xSta[_S_CH1_DAC_ADC + _CH_DIS(ch)]);
            } else {
                xCache.level_L[ch] = 100;
            }
        }
        //获取瓦数
        xCache.level_W[ch] = watt_get(xCache.level_L[ch], xCache.level_D[ch]);
        //LOG_I("CH[%d]--M:%d-D:%d-L:%d-W:%d", ch, xCache.level_M[ch], xCache.level_D[ch], xCache.level_L[ch],
        //             xCache.level_W[ch]);
    }

    return watt;
}
/* ---------------------------------------------------------------------------*/
