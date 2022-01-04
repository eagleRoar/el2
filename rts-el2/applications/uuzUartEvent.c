/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-27   ZhouXiaomin     first version
 */
/*include*******************************************************/
#include <rtthread.h>
/**************************************************************/
#include "uuzOpt.h"
#include "uuzUart.h"
/**************************************************************/
#include "uuzDevice.h"
/**************************************************************/
#define DBG_ENABLE
#define DBG_SECTION_NAME "UT|E"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
#if defined(BSP_USING_UART)
//--------------------------------------------------------------

/**
 * @brief 延时函数加载
 * 
 * @param parameter 
 */
void uart_delay_entry(void* parameter)
{
    while (1) {
        //判断延时接收处理
        rt_uart_delay_event(&xDeviceRx);
        rt_thread_mdelay(1);
    }
}

/**
 * @brief 串口处理事件
 * 
 * @param parameter 
 */
void uart_event_entry(void* parameter)
{

    while (1) {
        if (xDeviceRx.readylen) {
            device_thread_entry(&xDeviceRx);
        }
        rt_thread_mdelay(10);
    }
}

/**
 * @brief 事件定时器
 * 
 * @return int 
 */
int uart_event_init(void)
{
    rt_err_t ret = RT_EOK;
    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("event_u", uart_event_entry,
    RT_NULL, 372, 14, 10);

    /* 创建成功则启动线程 */
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
        LOG_I("start Thread [event uart] sucess");
    } else {
        LOG_E("start Thread [event uart] failed");
        ret = RT_ERROR;
    }

    if (ret == RT_EOK) {
        /* 创建 serial 线程 */
        rt_thread_t thread = rt_thread_create("delay_u", uart_delay_entry,
        RT_NULL, 256, 12, 10);

        /* 创建成功则启动线程 */
        if (thread != RT_NULL) {
            rt_thread_startup(thread);
            LOG_I("start Thread [delay uart] success");
        } else {
            LOG_E("start Thread [delay uart] failed");
            ret = RT_ERROR;
        }
    }

    return ret;
}
#endif /* BSP_USING_UART */
