#ifndef __UUZ_CONFIG_BBL_H
#define __UUZ_CONFIG_BBL_H

//uuzMBR_CODE
/* ----------------------- Defines ------------------------------------------*/
#define uuzBBL_HEAD (0xFAU) /*! BBL Head address */

//Command for Reg
#define uuzBBL_BROADCAST_ID (0x00U) /*! BBL broadcast address. */
#define uuzBBL_ACS_ID_REG (0x01U) /*! AC Station 注册命令. */
#define uuzBBL_SEND_ID_REG (0x7FU) /*! 通用注册命令 */
#define uuzBBL_SEND_ID_TEST (0xAAU) /*! 位置定位命令 */

//ACK/PEPLY
#define uuzBBL_ACK (0x00U)
#define uuzBBL_REPLY (0x80U)

//HEAD TYPE
#define uuzPROTOCOL_NULL (0x00U) //无效标志
#define uuzPROTOCOL_BBL (0x01U)
#define uuzPROTOCOL_MBR (0x02U)

#endif // __UUZ_CONFIG_BBL_H
