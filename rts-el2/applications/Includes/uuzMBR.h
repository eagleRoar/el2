/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-03   ZhouXiaomin     first version
 */
#ifndef __UUZ_MBR_H
#define __UUZ_MBR_H

#include "typedefDEF.h"

#ifdef __cplusplus
extern "C" {
#endif

u8 uuz_ucDataHeadIsAssert(u8 ucCode);
u8 uuz_ucDataOrderIsAssert(u8 ucCode);
u8 uuz_ucDataLengthIsAssert(const u8* ucCode);

#ifdef __cplusplus
}
#endif
#endif // __UUZ_MBR_H
