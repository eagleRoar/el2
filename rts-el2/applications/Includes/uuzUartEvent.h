/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-27   ZhouXiaomin     first version
 */

#ifndef _UUZ_UART_EVENT_H_
#define _UUZ_UART_EVENT_H_

#include "uuzUart_Config.h"
#include "uuzUart_Typedef.h"
#include <board.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(BSP_USING_UART)
//uart延时公共函数
int uart_delay_init(void);
int uart_event_init(void);
#endif /* BSP_USING_UART */

#ifdef __cplusplus
}
#endif

#endif /* _UUZ_UART_EVENT_H_ */
