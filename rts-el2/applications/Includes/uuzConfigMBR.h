/*
 * Copyright (c) 2006-2018 RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-03   ZhouXiaomin     first version
 */
#ifndef __UUZ_CONFIG_MBR_H
#define __UUZ_CONFIG_MBR_H
//uuzMBR_CODE
/* ----------------------- Defines ------------------------------------------*/
#define uuzMBR_ADDRESS_BROADCAST (0x00U) /*! Modbus broadcast address. */
#define uuzMBR_ADDRESS_MIN (0x01U) /*! Smallest possible slave address. */
#define uuzMBR_ADDRESS_MAX (0xF7U) /*! Biggest possible slave address. */

#define uuzMBR_READ_COILS (0x01U)
#define uuzMBR_READ_HOLDING_REGISTER (0x03U)
#define uuzMBR_WRITE_SINGLE_COIL (0x05U)
#define uuzMBR_WRITE_REGISTER (0x06U)
#define uuzMBR_WRITE_MULTIPLE_REGISTERS (0x10U)
#define uuzMBR_ERROR (0x80U)

#if (_INVALID) //NOT USE CODE
#define uuzMBR_NONE (0x00U)
#define uuzMBR_READ_DISCRETE_INPUTS (0x02U)
#define uuzMBR_WRITE_MULTIPLE_COILS (0x0FU)
#define uuzMBR_READ_INPUT_REGISTER (0x04U)
#define uuzMBR_READWRITE_MULTIPLE_REGISTERS (0x17U)
#define uuzMBR_DIAG_READ_EXCEPTION (0x07U)
#define uuzMBR_DIAG_DIAGNOSTIC (0x08U)
#define uuzMBR_DIAG_GET_COM_EVENT_CNT (0x0BU)
#define uuzMBR_DIAG_GET_COM_EVENT_LOG (0x0CU)
#define uuzMBR_OTHER_REPORT_SLAVEID (0x11U)
#endif

#endif // __UUZ_CONFIG_MBR_H
