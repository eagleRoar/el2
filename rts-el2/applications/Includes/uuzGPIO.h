/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-22     Fish       the first version
 */
#ifndef APPLICATIONS_INCLUDES_UUZGPIO_H_
#define APPLICATIONS_INCLUDES_UUZGPIO_H_

#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>

//LED
#define LD_POW GET_PIN(C, 9)
#define LD_R1 GET_PIN(C, 10)
#define LD_R2 GET_PIN(C, 11)
#define LD_B1 GET_PIN(C, 12)
#define LD_B2 GET_PIN(C, 13)
//LCD GPIO
#define RST_CTR GET_PIN(A, 15)
#define LCD_WR GET_PIN(A, 1)
#define LCD_RD GET_PIN(A, 2)
#define LCD_DC GET_PIN(A, 3)
#define LCD_DB0 GET_PIN(C, 0)
#define LCD_DB1 GET_PIN(C, 1)
#define LCD_DB2 GET_PIN(C, 2)
#define LCD_DB3 GET_PIN(C, 3)
#define LCD_DB4 GET_PIN(C, 4)
#define LCD_DB5 GET_PIN(C, 5)
#define LCD_DB6 GET_PIN(C, 6)
#define LCD_DB7 GET_PIN(C, 7)
#define LCD_CS GET_PIN(C, 8)
//ADC
#define CH1_PW GET_PIN(A, 6)
#define CH2_PW GET_PIN(A, 7)
//ADC Temperature
#define CH1_TA GET_PIN(B, 0)
#define CH2_TA GET_PIN(B, 1)
//Temperature Enable
#define CH1_TA_EN GET_PIN(B, 2)
#define CH2_TA_EN GET_PIN(B, 3)
//Uart Sensor
#define SENSOR_TX GET_PIN(B, 10)
#define SENSOR_RX GET_PIN(B, 11)
#define RS485_CLK GET_PIN(B, 12)
//ECM Control
#define ECM1_CTR GET_PIN(B, 13)
#define ECM2_CTR GET_PIN(B, 14)
//Alarm Enable
#define ALM_CTR GET_PIN(B, 15)
//DEBUG Uart
#define DBG_TX GET_PIN(A, 9)
#define DBG_RX GET_PIN(A, 10)
//USB
#define USB_POW_CTR GET_PIN(A, 8)
#define USB_DD GET_PIN(A, 11)
#define USB_DA GET_PIN(A, 12)
//KEY
#define KEY_COM GET_PIN(B, 4)
#define KEY_DOWN GET_PIN(B, 5)
#define KEY_ENTER GET_PIN(B, 6)
#define KEY_UP GET_PIN(B, 7)
#define KEY_LIGHT GET_PIN(B, 8)
#define KEY_BACK GET_PIN(B, 9)

#define RCC_0 GET_PIN(D, 0)
#define RCC_1 GET_PIN(D, 1)
#define LT_BLK GET_PIN(D, 2)

#ifdef __cplusplus
extern "C" {
#endif

int uuz_gpio_init(void);
void uuz_vDebugSta_Setup(rt_uint8_t ucSta);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_INCLUDES_UUZGPIO_H_ */
