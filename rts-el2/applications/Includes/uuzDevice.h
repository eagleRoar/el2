/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-04   ZhouXiaomin     first version
 */
#ifndef _UUZ_DEVICE_H_
#define _UUZ_DEVICE_H_

#include "uuzUart_Config.h"
#include "uuzUart_Typedef.h"
#include <board.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(BSP_USING_UART)
#if defined(BSP_USING_UART3)
extern typedef_Uart xDeviceRx;
rt_err_t device_uart_input(rt_device_t dev, rt_size_t size);
/* 接收回调函数 */
void device_thread_entry(void* parameter);
void uuz_vDeviceReceiveFromUSART(u8* ucRxCode, u8* ucTxCode);
#endif /* BSP_USING_UART8 */

#endif /* BSP_USING_UART */

#ifdef __cplusplus
}
#endif

#endif /* _UUZ_DEVICE_H_ */
