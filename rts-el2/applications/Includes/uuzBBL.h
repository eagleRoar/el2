/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-02   ZhouXiaomin     first version
 */
#ifndef __UUZ_BBL_H
#define __UUZ_BBL_H
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

u8 uuz_ucProtocolHeadByIsAssert(u8 ucHead);
u8 uuz_ucQueryDataByBBLIsAssert(const u8* ucRxCode, u8 ucRxLen);
u8 uuz_ucQueryLengthByBBLIsAssert(const u8* ucRxCode, u8 ucRxLen);

#ifdef __cplusplus
}
#endif
#endif // __UUZ_BBL_H
