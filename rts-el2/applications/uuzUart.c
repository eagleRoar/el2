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
#include <rtdevice.h>
/**************************************************************/
#include "uuzOpt.h"
#include "uuzUart.h"
/**************************************************************/
#define DBG_ENABLE
#define DBG_SECTION_NAME "UART"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
#if defined(BSP_USING_UART)
//--------------------------------------------------------------
/**
 * @brief uart_init
 * @param uart
 * @param name
 * @param rx_sem_name
 * @param thread_name
 * @param baud_rate
 * @param xEvent
 * @param thread_entry
 * @param ulMem
 * @return
 */

int uart_init(typedef_Uart* uart,
    char* name, char* rx_sem_name, char* thread_name,
    rt_uint32_t baud_rate, kitEvent xEvent, p_kitEvent thread_entry,
    rt_uint32_t ulMem, rt_uint8_t type)
{
    rt_err_t ret = RT_EOK;

    rt_strncpy(uart->name, name, RT_NAME_MAX);
    /* 串口配置数据 */
    struct serial_configure xConfig = RT_SERIAL_CONFIG_DEFAULT;
    xConfig.baud_rate = baud_rate;
    /* 查找串口设备 */
    uart->serial = rt_device_find(uart->name);

    if (!uart->serial) {
        LOG_E("find %s failed!", uart->name);
        return RT_ERROR;
    }

    /* 控制串口设备，通过控制接口传入命令控制字，与控制参数 */
    rt_device_control(uart->serial, RT_DEVICE_CTRL_CONFIG, &xConfig);

    /* 初始化接收信号量 */
    rt_sem_init(&uart->rx_sem, rx_sem_name, 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送方式打开串口设备 */
    rt_device_open(uart->serial, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(uart->serial, xEvent);
    /* 发送字符串 */
    LOG_I("Open [%s]:[%d]-[%s]!", uart->name,
        xConfig.baud_rate, rx_sem_name);

    //记录串口工作类型RS232|RS485|RS232(RTU)|RS485(RTU)
    uart->serial_type = type;
    //初始工作状态
    uart->flag = 0;
    uart->time_dy = 0;
    if (baud_rate == BAUD_RATE_9600) {
        //9600的延时3.5ms --> 7ms
        uart->time_max = 7;
    } else if (baud_rate == BAUD_RATE_115200) {
        //115200的延时1.5ms --> 3ms
        uart->time_max = 3;
    } else {
        //输入默认值7ms
        uart->time_max = 7;
    }

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create(
        thread_name,
        thread_entry,
        (void*)uart,
        ulMem,
        10,
        10);

    /* 创建成功则启动线程 */
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
        uart->len = 0;
        LOG_I("Startup thread [%s][%s][%d]", uart->name, thread_name, ulMem);
    } else {
        ret = RT_ERROR;
    }

    return ret;
}

//定义设备通讯通道
/**
 * @brief 发送数据的公共USART函数
 * 
 * @param pxUart 
 * @param pucTxCode 
 * @param ucLen 
 */
void rt_uart_send_entry(typedef_Uart* pxUart, const u8* pucTxCode, u8 ucLen)
{
    if ((pucTxCode) && (ucLen)) {

        if ((pxUart->serial_type == 1) || (pxUart->serial_type == 3)) {
            /* 打开CLK的GPIO */
            uuzUSART_CLK_TX;
            rt_thread_mdelay(2);
        }

        //发送RS485数据
        rt_device_write(pxUart->serial, 0, pucTxCode, ucLen);
        if ((pxUart->serial_type == 2) || (pxUart->serial_type == 3)) {
            //Modbus-RTU系列,记录发送数据
            rt_memcpy(pxUart->txbuff, pucTxCode, ucLen);
        }

        if ((pxUart->serial_type == 1) || (pxUart->serial_type == 3)) {
            /* 关闭CLK的GPIO */
            uuzUSART_CLK_RX;
            rt_thread_mdelay(1);
        }
    }
}

/**
 * @brief 串口设备机接收函数
 * 
 * @param parameter 
 */
void rt_uart_thread_entry(void* parameter)
{
    typedef_Uart* uart = (typedef_Uart*)parameter;
    uart->len = 0;

    while (1) {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(uart->serial, 0, &uart->buff, 1) != 1) {
            uart->rxbuff[uart->len] = uart->buff;
            uart->len++;

            //清空判断标记,和1m定时器线程进行判断
            uart->time_dy = 0;
            if (uart->flag == 0) {
                uart->flag = 1;
            }

            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&uart->rx_sem, RT_WAITING_FOREVER);
        }
    }
}

/**
 * @brief 串口延时判断函数
 * 
 * @param parameter 
 */
void rt_uart_delay_event(void* parameter)
{
    typedef_Uart* uart = (typedef_Uart*)parameter;
    // Delay Event
    if (uart->flag == 1) {
        //如果等待时间大于延时时间
        if (uart->time_dy >= uart->time_max) {
            uart->readylen = 1;
            uart->flag = 0;
            uart->time_dy = 0;
        }
        uart->time_dy++;
    }
}

#endif /* BSP_USING_UART */
