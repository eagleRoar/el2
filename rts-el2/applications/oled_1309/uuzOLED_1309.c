#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

// You may reference Drivers/drv_gpio.c for pinout
// In u8x8.h #define U8X8_USE_PINS

#include <u8g2_port.h>
u8g2_t u8g2_el2;

#define SSD1309_8080_PIN_D0                    32  // PC0
#define SSD1309_8080_PIN_D1                    33  // PC1
#define SSD1309_8080_PIN_D2                    34  // PC2
#define SSD1309_8080_PIN_D3                    35  // PC3
#define SSD1309_8080_PIN_D4                    36  // PC4
#define SSD1309_8080_PIN_D5                    37  // PC5
#define SSD1309_8080_PIN_D6                    38  // PC6
#define SSD1309_8080_PIN_D7                    39  // PC7
#define SSD1309_8080_PIN_EN                    1   // PA1
#define SSD1309_8080_PIN_CS                    40  // PC8
#define SSD1309_8080_PIN_DC                    3   // PA3
#define SSD1309_8080_PIN_RST                   0   // PA0

void u8x8_SetPin_8Bit_8080(u8x8_t *u8x8, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5,
        uint8_t d6, uint8_t d7, uint8_t wr, uint8_t cs, uint8_t dc, uint8_t reset)
{
    u8x8_SetPin(u8x8, U8X8_PIN_D0, d0);
    u8x8_SetPin(u8x8, U8X8_PIN_D1, d1);
    u8x8_SetPin(u8x8, U8X8_PIN_D2, d2);
    u8x8_SetPin(u8x8, U8X8_PIN_D3, d3);
    u8x8_SetPin(u8x8, U8X8_PIN_D4, d4);
    u8x8_SetPin(u8x8, U8X8_PIN_D5, d5);
    u8x8_SetPin(u8x8, U8X8_PIN_D6, d6);
    u8x8_SetPin(u8x8, U8X8_PIN_D7, d7);
    u8x8_SetPin(u8x8, U8X8_PIN_E, wr);
    u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
    u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
    u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}

//static void st7920_12864_8080_example(int argc, char *argv[])
void oled_init(void)
{
    // Initialization
    u8g2_Setup_ssd1309_128x64_el2_1(&u8g2_el2, U8G2_R0, u8x8_byte_8bit_8080mode, u8x8_rt_gpio_and_delay);
    u8x8_SetPin_8Bit_8080(u8g2_GetU8x8(&u8g2_el2),
    SSD1309_8080_PIN_D0, SSD1309_8080_PIN_D1,
    SSD1309_8080_PIN_D2, SSD1309_8080_PIN_D3,
    SSD1309_8080_PIN_D4, SSD1309_8080_PIN_D5,
    SSD1309_8080_PIN_D6, SSD1309_8080_PIN_D7,
    SSD1309_8080_PIN_EN, SSD1309_8080_PIN_CS,
    SSD1309_8080_PIN_DC, SSD1309_8080_PIN_RST);

    u8g2_InitDisplay(&u8g2_el2);
    u8g2_SetPowerSave(&u8g2_el2, 0);

    // Draw Graphics
    /* full buffer example, setup procedure ends in _f */
    u8g2_SetFont(&u8g2_el2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2_el2, 100, 56, 0x2603);
    u8g2_SendBuffer(&u8g2_el2);
}
MSH_CMD_EXPORT(oled_init, ssd1309 12864 LCD sample);
