/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-27   ZhouXiaomin     first version
 */

#ifndef _UUZ_UART_H_
#define _UUZ_UART_H_

#include "uuzUart_Config.h"
#include "uuzUart_Typedef.h"
#include <board.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(BSP_USING_UART)
//uart公共函数
int uart_init(typedef_Uart* uart,
    char* name, /* 串口名称*/
    char* rx_sem, /* 回调函数名称*/
    char* thread_name, /* 线程名称*/
    rt_uint32_t baud_rate, /*串口波特率 */
    kitEvent xEvent, /*回调实现函数*/
    p_kitEvent thread_entry, /*线程实现函数*/
    rt_uint32_t ulMem, /* 线程的空间分配量 byte*/
    rt_uint8_t type); /*串口类型*/

void rt_uart_send_entry(typedef_Uart* pxUart, const u8* pucTxCode, u8 ucLen);
void rt_uart_thread_entry(void* parameter);
void rt_uart_delay_event(void* parameter);

#endif /* BSP_USING_UART */

#ifdef __cplusplus
}
#endif

#endif /* _UUZ_UART_H_ */
