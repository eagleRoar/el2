/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-02   ZhouXiaomin     first version
 */
#ifndef __UUZ_BTN_H
#define __UUZ_BTN_H
#include <board.h>
#include "uuzGPIO.h"

enum
{
    //短按标记
    _key_down = 21,
    _key_enter,
    _key_light,
    _key_up,
    _key_back,

    //长按标记
    _key_l_down = 121,
    _key_l_enter,
    _key_l_light,
    _key_l_up,
    _key_l_back
};

#define _BTN_LONG_INTERVAL  (1000U)

#ifdef __cplusplus
extern "C" {
#endif

void uuz_btn_init(void);

#ifdef __cplusplus
}
#endif
#endif // __UUZ_BTN_H
