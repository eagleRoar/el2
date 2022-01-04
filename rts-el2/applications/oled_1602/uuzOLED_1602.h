#ifndef __UUZ_OLED_1602_H
#define __UUZ_OLED_1602_H

#include "typedefDEF.h"
#include "uuzConfig1602.h"

//OLED初始化
extern OledLcd_Typedef_t oled;

#ifdef __cplusplus
extern "C" {
#endif

void font_table_set(rt_uint8_t fontlib);
void drawString(rt_uint8_t type, rt_uint8_t h, rt_uint8_t w, char * data);
void drawToDATA(rt_uint8_t h, rt_uint8_t w, char * data, rt_uint8_t l);
void drawToOLED(rt_uint8_t h, rt_uint8_t w, char * data, rt_uint8_t l);
void drawNull(rt_uint8_t type, rt_uint8_t h, rt_uint8_t w, rt_uint8_t length);
void clear_screen(u8 screen);
void clear_cache_data(u8 screen);
void sync_oled(u8 screen);

#ifdef __cplusplus
}
#endif
#endif // __UUZ_CONFIG_1602_H
