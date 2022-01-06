/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-22     Fish       the first version
 */
/**
 *
 * @return
 */
#include "uuzGPIO.h"

int uuz_gpio_init(void)
{
#if 0
    /* set LED0 pin mode to output */
    rt_pin_mode(LD_POW, PIN_MODE_OUTPUT);
    rt_pin_mode(LD_R1, PIN_MODE_OUTPUT);
    rt_pin_mode(LD_R2, PIN_MODE_OUTPUT);
    rt_pin_mode(LD_B1, PIN_MODE_OUTPUT);
    rt_pin_mode(LD_B2, PIN_MODE_OUTPUT);

    //初始基本状态
    rt_pin_write(LD_POW, PIN_LOW);
    rt_pin_write(LD_R1, PIN_HIGH);
    rt_pin_write(LD_R2, PIN_HIGH);
    rt_pin_write(LD_B1, PIN_HIGH);
    rt_pin_write(LD_B2, PIN_HIGH);
#endif

    //按键相关类型设
    rt_pin_mode(KEY_COM, PIN_MODE_OUTPUT);
    //按键公共脚，保持低电平
    rt_pin_write(KEY_COM, PIN_LOW);

    //OLCD脚位初始化
    rt_pin_mode(LCD_DB0, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB1, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB2, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB3, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB4, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB5, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB6, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB7, PIN_MODE_OUTPUT);

    rt_pin_mode(RST_CTR, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_WR, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_RD, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_CS, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DC, PIN_MODE_OUTPUT);

    rt_pin_mode(ECM1_CTR, PIN_MODE_OUTPUT);
    rt_pin_mode(ECM2_CTR, PIN_MODE_OUTPUT);
    rt_pin_mode(ALM_CTR, PIN_MODE_OUTPUT);

    //需要配置成GPIO-OUTPUT
    rt_pin_mode(RCC_1, PIN_MODE_OUTPUT);
    rt_pin_mode(RCC_0, PIN_MODE_OUTPUT);

    //手动启动OLCD的PIN脚
    rt_pin_write(LCD_RD, PIN_HIGH);

    return RT_EOK;
}

/**
 *
 * @param ucSta
 */
void uuz_vDebugSta_Setup(rt_uint8_t ucSta)
{
    if (ucSta == 0) {
        rt_pin_mode(USB_DD, PIN_MODE_INPUT);
        rt_pin_mode(USB_DA, PIN_MODE_INPUT);
    } else {
        rt_pin_mode(DBG_TX, PIN_MODE_INPUT);
        rt_pin_mode(DBG_RX, PIN_MODE_INPUT);
    }
}
