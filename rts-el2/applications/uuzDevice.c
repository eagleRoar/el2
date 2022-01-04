/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-27   ZhouXiaomin     first version
 * 2019-12-26   ZhouXiaomin     add recvice function
 */
/*include*******************************************************/
#include <rtthread.h>
#include <uuzINIT.h>
/**************************************************************/
#include "uuzOpt.h"
#include "uuzUart.h"
#include "uuzBBL.h"
#include "uuzConfigBBL.h"
#include "uuzConfigMBR.h"
#include "uuzMBR.h"
/**************************************************************/
#include "uuzDevice.h"
/**************************************************************/
#define DBG_ENABLE
#define DBG_SECTION_NAME "D|R"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
/**************************************************************/
#if defined(BSP_USING_UART)
#if defined(BSP_USING_UART3)
/**
 * @brief  定义设备通讯通道
 */
typedef_Uart xDeviceRx;
/**
 * @brief 设备接口函数
 * 
 * @param dev 
 * @param size 
 * @return rt_err_t 
 */
rt_err_t device_uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    //读取缓存数据
    rt_sem_release(&xDeviceRx.rx_sem);
    return RT_EOK;
}

/**
 * @brief device_thread_entry
 * @param parameter
 */
void device_thread_entry(void* parameter)
{
    typedef_Uart* uart = (typedef_Uart*) parameter;
    //u8 ucProtocol = uuzPROTOCOL_NULL;

    if (uart->len >= 5) {
        /* Read Head Code is 'BBL' */
        //ucProtocol = uuz_ucProtocolHeadByIsAssert(uart->rxbuff[0]);
        //ucProtocol = uuzPROTOCOL_BBL;
        //if (ucProtocol != uuzPROTOCOL_NULL) {
        //达到检测长度，进行CRC16对比（LSB）
        u16 usUhCRC = usModbusRTU_CRC(uart->rxbuff, (uart->len - 2));  //计算当前的CRC16
        u16 usRdCRC = usU8ToU16((uart->rxbuff + (uart->len - 2)), uuzLSB);  //读取协议中的CRC16
#if 1
        rt_kprintf("Device:");
        for (u8 ucIndex = 0; ucIndex < uart->len; ucIndex++) {
            //最大一次显示24个字符
            if ((ucIndex != 0) && ((ucIndex % 24) == 0)) {
                rt_kprintf("\r\n");
            }
            rt_kprintf("%02X ", uart->rxbuff[ucIndex]);
        }
        rt_kprintf("\r\n");
#endif
        //如果CRC校验成功
        if (usUhCRC == usRdCRC) {
            uuz_vDeviceReceiveFromUSART(uart->rxbuff, uart->txbuff);
        }
    }
    //清空接收区
    uart->len = 0;
    //清空接收标记
    uart->readylen = 0;
}

/**
 * @brief  设备实际处理函数
 * 
 * @param ucRxCode 
 * @param ucTxCode 
 */
void uuz_vDeviceReceiveFromUSART(u8* ucRxCode, u8* ucTxCode)
{
#if 0
// Init Temp Data
    u8 ucRegSta = 0;
    u8 ucBroadcastID[4] = {0x00, 0x00, 0x00, 0x00};

    if (ucRxCode) {
        //读取设备的modbus和对应设备号
        u8 ucHead = ucRxCode[0];
        if (ucHead == uuzPROTOCOL_BBL) {
            // MODBUS-RTU RPOTOCOL
            u8 ucTxHead = ucTxCode[0];
            u16 usTxRegAddr = usU8ToU16(ucTxCode + 2, uuzMSB);

            //发送端和接收端相同
            if (ucHead == ucTxHead) {
                //读取发送参数
                u8 ucOpt = ucRxCode[1];
                u8 ucIndex = ucIDtoVal(ucHead, uuzDEV_SL_ACO, uuzUART_DEVICE);

                //没有错误数据
                if (ucIndex != uuzDEV_TEST) {
                    switch (ucOpt)
                    {
                        case uuzMBR_READ_HOLDING_REGISTER:
                        //获取到相关数据
                        if (usTxRegAddr == uuzADDR_RW_ACS_STA_DATA) {
                            //获取实时数据
                            uuz_vExACSDataGet((ucRxCode + 3), &xDevValue.xValue_ST[ucIndex]);
                            //更新连接状态
                            uuz_vDevice_IDReset(&xDevState.xST[ucIndex]);
                        }
                        break;
                        case uuzMBR_WRITE_REGISTER:
                        if (usTxRegAddr == uuzADDR_RW_ACS_STA_DATA) {
                            ucIndex = ucIDtoVal(ucHead, uuzDEV_SL_ACO, uuzUART_DEVICE);
                            //获取实时数据
                            uuz_vExACSDataGet((ucRxCode + 4), &xDevValue.xValue_ST[ucIndex]);
                            //更新连接状态
                            uuz_vDevice_IDReset(&xDevState.xST[ucIndex]);
                        }
                        break;
                        default:
                        break;
                    }
                }
            } else {
                LOG_E("The data header to be received is inconsistent with the sender");
            }
        }
    }
#endif
}
#endif /* BSP_USING_UART3 */
#endif /* BSP_USING_UART */
