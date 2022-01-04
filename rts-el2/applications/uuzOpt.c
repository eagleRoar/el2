/*****************************************************
 * @projectName   BHM-Master-V1
 * @fileName      uuzOpt.c
 * @brief         通用操作函数
 * @author        Zhou Xiaomin
 * @date          2019-07-01
 *******************************************************/
/* Includes ------------------------------------------------------------------*/
#include "uuzOpt.h"
#include <stdlib.h>
#include <string.h>
/* ---------------------------------------------------------------------------*/
/**
 * @brief StringToHex
 * @brief 下面定义的字符串中的字符只能是0-F的字符，但是不区分大小写的，
 *        前面是安装两位为一个数字进行转换，最后一个数字如果还是两位的则正常转换;
 *        如果只剩一位的话则在前面补零输出.
 * @param str
 * @param out
 * @return
 */
int StringToHex(char* str, u8* out)
{
    char* p = str;
    char high = 0, low = 0;
    int tmplen = strlen(p), cnt = 0;
    tmplen = strlen(p);

    while (cnt < (tmplen / 2)) {
        high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
        low = (*(++p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p)-48 - 7 : *(p)-48;
        out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
        p++;
        cnt++;
    }
    if (tmplen % 2 != 0) {
        out[cnt] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
    }

    return tmplen / 2 + tmplen % 2;
}

/*
 * INPUT:
 * pucValue: input the data by (uint8_t)
 * ucMode : LSB [0](1->0) or MSB [1] (0->1)
 *
 * OUTPUT: the data by (uint16_t)
*/
u16 usU8ToU16(const u8* pucValue, u8 ucMode)
{
    if (pucValue) {
        return ((pucValue[1 - ucMode] << 8) + pucValue[ucMode]);
    } else {
        return uuzFALSE;
    }
}

/*
 * INPUT:
 * usValue: input the data by (uint16_t)
 * ucMode : LSB [0] (1->0) or MSB [1] (0->1)
 *
 * OUTPUT: void
 *         ucHex  output the data by (uint8_t)
*/
void vU16ToU8(u8* pucHex, u16 usValue, u8 ucMode)
{
    if (pucHex) {
        pucHex[1 - ucMode] = (usValue >> 8) & 0xFF;
        pucHex[ucMode] = usValue & 0xFF;
    }
}

/*
 * INPUT:
 * pucValue: input the data by (uint8_t)
 * ucMode : [0](3->0) or [1] (0->3)
 *
 * OUTPUT: the data by (uint32_t)
*/
u32 ulU8ToU32(const u8* pucValue, u8 ucMode)
{
    u32 ulValue;

    if (pucValue) {
        if (ucMode == 1) {
            ulValue = (u32)((((u32)pucValue[0]) << 24) | (((u32)pucValue[1]) << 16)
                | (((u32)pucValue[2]) << 8) | ((u32)pucValue[3]));
        } else {
            ulValue = (u32)((((u32)pucValue[3]) << 24) | (((u32)pucValue[2]) << 16)
                | (((u32)pucValue[1]) << 8) | ((u32)pucValue[0]));
        }
        return ulValue;
    } else {
        return uuzFALSE;
    }
}

float fU8ToU32(const u8* pucValue, u8 ucMode)
{
    float fValue;
    u32 ulValue;

    if (pucValue) {
        if (ucMode == 1) {
            ulValue = (u32)((((u32)pucValue[0]) << 24) | (((u32)pucValue[1]) << 16)
                | (((u32)pucValue[2]) << 8) | ((u32)pucValue[3]));
        } else {
            ulValue = (u32)((((u32)pucValue[3]) << 24) | (((u32)pucValue[2]) << 16)
                | (((u32)pucValue[1]) << 8) | ((u32)pucValue[0]));
        }
        fValue = *(u32*)&ulValue;
        return fValue;
    } else {
        return 0;
    }
}

/*
 * INPUT:
 * ulValue: input the data by (uint32_t)
 * ucMode : [0](1->0) or [1] (0->1)
 *
 * OUTPUT: void
 *         ucHex  output the data by (uint8_t)
*/
void vU32ToU8(u8* pucHex, u32 ulValue, u8 ucMode)
{
    if (pucHex) {
        if (ucMode == 1) {
            pucHex[0] = (ulValue >> 24) & 0xFF;
            pucHex[1] = (ulValue >> 16) & 0xFF;
            pucHex[2] = (ulValue >> 8) & 0xFF;
            pucHex[3] = (ulValue)&0xFF;
        } else {
            pucHex[3] = (ulValue >> 24) & 0xFF;
            pucHex[2] = (ulValue >> 16) & 0xFF;
            pucHex[1] = (ulValue >> 8) & 0xFF;
            pucHex[0] = (ulValue)&0xFF;
        }
    }
}

/*
 * INPUT:
 * pucData: input the data for SUM-CRC
 * ulLen : the length of the data
 *
 * OUTPUT: the value for (CRC-SUM)
*/
u8 ucSUM_CRC(const u8* pucData, u32 ulLen)
{
    u8 ucCRC = 0;

    if (pucData) {
        while (ulLen) {
            ucCRC += *pucData++;
            ucCRC &= 0xFF;
            ulLen--;
        }
    }

    return ucCRC;
}

/*
 * INPUT:
 * pucData: input the data for CRC16
 * ulLen : the length of the data
 *
 * OUTPUT: the value for (CRC16)
*/
u16 usModbusRTU_CRC(const u8* pucData, u32 ulLen)
{
    u8 ucIndex = 0U;
    u16 usCRC = 0xFFFFU;

    while (ulLen > 0U) {
        usCRC ^= *pucData++;
        while (ucIndex < 8U) {
            if (usCRC & 0x0001U) {
                usCRC >>= 1U;
                usCRC ^= 0xA001U;
            } else {
                usCRC >>= 1U;
            }
            ucIndex++;
        }
        ucIndex = 0U;
        ulLen--;
    }
    return usCRC;
}

#if 0
//-----------------------------------------------------------------
// CRC_16校验码表(由上面的比特型CRC16校验函数生成，共256项，512字节)
//-----------------------------------------------------------------
u16 CRC_T16[] =
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0,
    0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 0xCC01, 0x0CC0, 0x0D80, 0xCD41,
    0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0,
    0x0880, 0xC841, 0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1,
    0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341,
    0x1100, 0xD1C1, 0xD081, 0x1040, 0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1,
    0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0,
    0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 0x2800, 0xE8C1, 0xE981, 0x2940,
    0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1,
    0xEC81, 0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 0xA001, 0x60C0,
    0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740,
    0xA501, 0x65C0, 0x6480, 0xA441, 0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0,
    0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0,
    0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 0xB401, 0x74C0, 0x7580, 0xB541,
    0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0,
    0x7080, 0xB041, 0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 0x9C01, 0x5CC0,
    0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40,
    0x9901, 0x59C0, 0x5880, 0x9841, 0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1,
    0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0,
    0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};
//-----------------------------------------------------------------
// CRC_16校验函数(查表法)
// uCRC16 = 0xFFFF;									校验码初值统一约定为0xFFFF;
//-----------------------------------------------------------------
void CRC_16(u8 d) //CRC_16校验函数,查表法
{
    uCRC16 = (uCRC16 >> 8) ^ CRC_T16[(uCRC16 & 0xFF) ^ d];
}
#endif

/*-----------------------------------------------------------------*/
