#ifndef __UUZ_OPT_H
#define __UUZ_OPT_H

#include "typedefDEF.h"

#define optUUZ_MAX(a, b) ((a > b) ? (a) : (b))
#define optUUZ_MIN(a, b) ((a <= b) ? (a) : (b))
#define optUUZ_COMPARE(a, b) ((a == b) ? (1U) : (0U))

/*the operate is for bit */
#define optUUZ_SET_BIT(a, b) ((a) |= (1 << b))
#define optUUZ_CLEAR_BIT(a, b) ((a) &= ~(1 << b))
#define optUUZ_IS_BIT_SET(a, b) ((a) & (1 << b))

#define uuzLSB (0x00U)
#define uuzMSB (0x01U)

#define uuzFALSE (0x00U)
#define uuzTRUE (!uuzFALSE)

/* CR+LF : 0D 0A */
#define uuzCR (0x0D)
#define uuzLF (0x0A)

#define _INVALID (0x00U)

#define uuzCONFIG_RAND_OPT (!_INVALID)

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------*/
/**
 * @brief StringToHex
 * @param str
 * @param out
 * @return
 */
int StringToHex(char* str, u8* out);

/*
 * INPUT:
 * pucValue: input the data by (uint8_t)
 * ucMode : [MSB] or [LSB]
 *
 * OUTPUT: the data by (uint16_t)
*/
u16 usU8ToU16(const u8* pucValue, u8 ucMode);
/*
 * INPUT:
 * usValue: input the data by (uint16_t)
 * ucMode : [MSB] or [LSB]
 *
 * OUTPUT: void
 *         ucHex  output the data by (uint8_t)
*/
void vU16ToU8(u8* pucHex, u16 usValue, u8 ucMode);
/*
 * INPUT:
 * pucData: input the data for SUM-CRC
 * ulLen :  the length for data
 *
 * OUTPUT: the value for data (SUM-CRC)
*/
/*
 * INPUT:
 * pucValue: input the data by (uint8_t)
 * ucMode : [MSB] or [LSB]
 *
 * OUTPUT: the data by (uint32_t)
*/
u32 ulU8ToU32(const u8* pucValue, u8 ucMode);

/*
 * INPUT:
 * ulValue: input the data by (uint32_t)
 * ucMode : [MSB] or [LSB]
 *
 *
 * OUTPUT: void
 *         ucHex  output the data by (uint8_t)
*/
void vU32ToU8(u8* pucHex, u32 ulValue, u8 ucMode);

u8 ucSUM_CRC(const u8* pucData, u32 ulLen);
/*
 * INPUT:
 * pucData: input the data for CRC16
 * ulLen :  the length for data
 *
 * OUTPUT: the value for data (CRC16)
*/
u16 usModbusRTU_CRC(const u8* pucData, u32 ulLen);

u8 ucGetRandData(u32 ulSeed, u8 ucRange);
u16 usGetRandData(u32 ulSeed, u16 usRange);
u32 ulGetRandData(u32 ulSeed, u32 ulRange);

#ifdef __cplusplus
}
#endif
#endif // __UUZ_OPT_H
