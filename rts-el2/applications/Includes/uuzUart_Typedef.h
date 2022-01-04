/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-27   ZhouXiaomin     first version
 */

#ifndef _UUZ_UART_TYPEDEF_H_
#define _UUZ_UART_TYPEDEF_H_

#include "uuzUart_Config.h"
#include <board.h>
#include <rtthread.h>

typedef rt_err_t (*kitEvent)(rt_device_t, rt_size_t);
typedef void (*p_kitEvent)(void* parameter);

typedef struct uuz_uart {
    //起始标记
    char name[RT_NAME_MAX];
    //Serial Name
    rt_device_t serial;
    //Callback
    struct rt_semaphore rx_sem;
    //回调函数地址
    kitEvent* rx_cb;
    p_kitEvent* rx_entry;

    //串口类型
    //0:RS232;1:RS485;2:RS232(RTU);3-RS485(RTU)
    rt_uint8_t serial_type;
    //接收区域
    rt_uint16_t len;
    rt_uint16_t readylen;
    rt_uint8_t buff;
    rt_uint8_t rxbuff[uuzUART_LEN];
    rt_uint8_t txbuff[uuzUART_LEN];
    //接收标记
    rt_uint8_t flag; // 0/1
    rt_uint8_t time_dy; // 实时的延时时间
    rt_uint8_t time_max; //最大延时时间Max 0~255m
} typedef_Uart;

#endif /* _UUZ_UART_TYPEDEF_H_ */
