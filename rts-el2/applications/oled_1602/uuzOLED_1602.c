//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include "uuzGPIO.h"
#include <rtthread.h>
#include "uuzConfig1602.h"

//-----------------OLED端口定义----------------
#define RS1602(P)	(rt_pin_write(LCD_DC, P))
#define RWB1602(P)	(rt_pin_write(LCD_WR, P))
#define RDB1602(P)	(rt_pin_write(LCD_RD, P))

//OLED初始化
OledLcd_Typedef_t oled;
static rt_mutex_t oled_mutex = RT_NULL;

/**
 * @brief 启动初始的间隔延时
 * @param time
 */
void delay(rt_uint32_t time)
{
    rt_uint8_t ucIndex;
    //125ns
    while (time--) {
        for (ucIndex = 0; ucIndex < 125; ucIndex++)
            ;
    }
}

/**
 * @brief 检测设备是否处理完成
 * @return
 */
rt_uint8_t isbusy(void)
{
    rt_uint8_t result;

    RS1602(0);    //RS=0;
    RDB1602(0);    //RDB=0;
    rt_pin_mode(LCD_DB7, PIN_MODE_INPUT);
    result = rt_pin_read(LCD_DB7);
    RDB1602(1);    //RDB=1;
    rt_pin_mode(LCD_DB7, PIN_MODE_OUTPUT);
    return result;
}

/**
 * @brief 写入配置命令
 * @param dictate
 */
void write_cmd(rt_uint8_t dictate)
{
    while (isbusy() == 1)
        ;
    RS1602(0);		//RS=0;
    RWB1602(0);		//RWB=0;
    GPIOC->ODR &= ~(0XFF);
    GPIOC->ODR |= dictate;    //PC[7:0]
    RWB1602(1);    //RWB=1;
}

/**
 * @brief 写入DDRAM地址设置
 * @param address (LINE-1:0x00~0x0F|LINE-2:0x40~0x4F)
 */
void write_address(rt_uint8_t address)
{
    write_cmd(address | 0x80);
}

/**
 * @brief 写入实际数据
 * @param data
 */
void write_data(rt_uint8_t data)
{
#if 0
    while (isbusy() == 1)
    ;
#endif
    RS1602(1);    //RS=1;
    RWB1602(0);    //RWB=0;
    GPIOC->ODR &= ~(0XFF);
    GPIOC->ODR |= data;    //PC[7:0]
    RWB1602(1);		//RWB=1;
}

/**
 * @brief 写入CGRAM地址数据(0x00~0x08)
 * @note 对应地址(0x40~0x7F)
 */
void cgram_init(void)
{
#if 0
    rt_uint8_t m;
    rt_uint8_t data[24] =
    {		//
        0x1f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1f,//
        0x1f, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1f,//
        0x1f, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f,//
    };
    for (m = 0; m < 24; m++) {
        //CMD
        write_cmd(0x40 + m);
        write_data(data[m]);
    }
#endif
}

#if 0
/**
 * @brief 读取OLED相关参数
 * @return
 */
rt_uint8_t oled_read(void)
{
    rt_uint8_t A;
    while (isbusy() == 1)
    ;
    rt_pin_mode(LCD_DB0, PIN_MODE_INPUT);
    rt_pin_mode(LCD_DB1, PIN_MODE_INPUT);
    rt_pin_mode(LCD_DB2, PIN_MODE_INPUT);
    rt_pin_mode(LCD_DB3, PIN_MODE_INPUT);
    rt_pin_mode(LCD_DB4, PIN_MODE_INPUT);
    rt_pin_mode(LCD_DB5, PIN_MODE_INPUT);
    rt_pin_mode(LCD_DB6, PIN_MODE_INPUT);
    rt_pin_mode(LCD_DB7, PIN_MODE_INPUT);
    //P1=0xff;
    RS1602(1);// RS=1;
    RDB1602(0);// RDB=0;

    A = 0;//P1;
    RDB1602(1);// RDB=1;
    rt_pin_mode(LCD_DB0, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB1, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB2, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB3, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB4, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB5, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB6, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_DB7, PIN_MODE_OUTPUT);
    return A;
}
#endif

/**
 * @brief 清除缓存数据
 * @param screen
 */
void clear_cache_data(u8 screen)
{
    //清空缓存数据
    rt_mutex_take(oled_mutex, RT_WAITING_FOREVER);
    if (screen == _SYNC_1 || screen == _SYNC_A) {
        rt_memset(&oled.data[_SYNC_1], 0x20, OLED_W);
    }
    if (screen == _SYNC_2 || screen == _SYNC_A) {
        rt_memset(&oled.data[_SYNC_2], 0x20, OLED_W);
    }
    rt_mutex_release(oled_mutex);
}

/**
 */
/**
 * @brief 清屏命令
 * @param screen 0:上半屏；1:下半屏; 2:全屏
 */
void clear_screen(u8 screen)
{
    rt_uint8_t ucIndex = 0;
    rt_uint8_t addr = 0;

    //清屏
    if (screen == _SYNC_A) {
        while (isbusy() == 1)
            ;
        write_cmd(0x01);
    } else {
        //_SYNC_1/_SYNC_2
        //清除一行数据
        for (ucIndex = 0; ucIndex < OLED_W; ucIndex++) {
            addr = 0x40U * screen + ucIndex;
            //清空位置
            write_address(addr);
            write_data(0x20U);
        }
    }
}

/**
 * @brief 光标回到初始位
 */
void corsor_return(void)
{
    while (isbusy() == 1)
        ;
    write_cmd(0x02);
}

/**
 * @brief 写入字库
 * @param fontlib:
 *                  0x38=English/Japanese
 *                  0x39=Western European #1
 *                  0x3A=English/Russian
 *                  0x3B=Western European #2
 */
void font_table_set(rt_uint8_t fontlib)
{
    while (isbusy() == 1)
        ;
    write_cmd(fontlib);
}

/**
 * @brief 关于光标的模式相关
 * @param display: on-0x04, off-0x00;
 * @param cursor: on-0x02, off-0x00;
 * @param blink: on-0x01, off-0x00
 */
void display_mode_set(rt_uint8_t display, rt_uint8_t cursor, rt_uint8_t blink)
{
    while (isbusy() == 1)
        ;
    rt_uint8_t cmd = 0x08;
    cmd |= display;
    cmd |= cursor;
    cmd |= blink;
    write_cmd(cmd);
}

void address_mode_set(void)
{
    while (isbusy() == 1)
        ;
    write_cmd(0x06);		//显示地址递增，即写一个数据后，显示位置右移一位
}

/**
 *
 * @brief 在缓存区上写入一个字符串
 * @param type: 0x01-Cache; 0x00-LCD
 * @param h: 高度位置(0~1)
 * @param w: 宽度位置(0~17)
 * @param data: 相关数据
 */
void drawString(rt_uint8_t type, rt_uint8_t h, rt_uint8_t w, char * data)
{
    rt_uint8_t length;
    rt_uint8_t ucIndex = 0;
    rt_uint8_t addr = 0;

    if ((h < OLED_H) && (w < OLED_W)) {
        length = rt_strlen(data);
        //计算长度
        if (type == _DATA_) {
            //加载数据
            length = ((w + length) > OLED_W) ? (OLED_W) : (length);
            rt_memcpy(oled.data[h] + w, data, length);
        } else if (type == _OLED_) {
            length = ((w + length) >= OLED_W) ? (OLED_W) : (w + length);
            rt_mutex_take(oled_mutex, RT_WAITING_FOREVER);
            for (ucIndex = w; ucIndex < length; ucIndex++) {
                addr = 0x40U * h + ucIndex;
                //清空位置
                write_address(addr);
                write_data(0x20U);
                write_address(addr);
                write_data(data[ucIndex - w]);
            }
            rt_mutex_release(oled_mutex);
        }
    }
}

/**
 *
 * @brief 写入字符串到缓冲区
 * @param h: 高度位置(0~1)
 * @param w: 宽度位置(0~17)
 * @param data: 相关数据
 * @param l: 写入的数据长度
 */
void drawToDATA(rt_uint8_t h, rt_uint8_t w, char * data, rt_uint8_t l)
{
    if ((h < OLED_H) && (w < OLED_W)) {
        //加载数据
        if ((w + l) > OLED_W) {
            rt_memcpy(oled.data[h] + w, data, (OLED_W - w));
        } else {
            rt_memcpy(oled.data[h] + w, data, l);
        }
    }
}

/**
 *
 * @brief 立即写入字符串
 * @param h: 高度位置(0~1)
 * @param w: 宽度位置(0~17)
 * @param data: 相关数据
 * @param l: 写入的数据长度
 */
void drawToOLED(rt_uint8_t h, rt_uint8_t w, char * data, rt_uint8_t l)
{
    rt_uint8_t length;
    rt_uint8_t ucIndex = 0;
    rt_uint8_t addr = 0;

    //计算写入后的数据位置
    length = ((w + l) >= OLED_W) ? (OLED_W) : (w + l);
    //按顺序写入数据
    rt_mutex_take(oled_mutex, RT_WAITING_FOREVER);
    for (ucIndex = w; ucIndex < length; ucIndex++) {
        addr = 0x40U * h + ucIndex;
        //清空位置
        write_address(addr);
        write_data(0x20U);
        //写入数据
        write_address(addr);
        write_data(data[ucIndex - w]);
    }
    rt_mutex_release(oled_mutex);
}

/**
 * @brief 在特定区域刷新空白
 * @param type
 * @param h
 * @param w
 * @param length
 */
void drawNull(rt_uint8_t type, rt_uint8_t h, rt_uint8_t w, rt_uint8_t length)
{
    rt_uint8_t ucIndex = 0;

    if ((h < OLED_H) && (w < OLED_W)) {
        //计算长度
        if (type == _DATA_) {
            //加载数据
            length = ((w + length) >= OLED_W) ? (OLED_W) : (length);
            rt_memset(oled.data[h] + w, 0x20U, length);
        } else if (type == _OLED_) {
            length = ((w + length) >= OLED_W) ? (OLED_W) : (w + length);
            rt_mutex_take(oled_mutex, RT_WAITING_FOREVER);
            for (ucIndex = w; ucIndex < length; ucIndex++) {
                //清空位置
                write_address(0x40U * h + ucIndex);
                write_data(0x20U);
            }
            rt_mutex_release(oled_mutex);
        }
    }
}

/**
 * @brief 统一刷新屏幕数据
 * @param screen 0:上半屏；1:下半屏; 2:全屏
 */
void sync_oled(u8 screen)
{
    rt_uint8_t ucIndex = 0;

    rt_mutex_take(oled_mutex, RT_WAITING_FOREVER);
    if (screen == _SYNC_1 || screen == _SYNC_A) {
        write_address(0x00U);
        for (ucIndex = 0; ucIndex < OLED_W; ucIndex++) {
            write_data(oled.data[_SYNC_1][ucIndex]);
        }
    }
    if (screen == _SYNC_2 || screen == _SYNC_A) {
        write_address(0x40U);
        for (ucIndex = 0; ucIndex < OLED_W; ucIndex++) {
            write_data(oled.data[_SYNC_2][ucIndex]);
        }
    }
    rt_mutex_release(oled_mutex);
}

/**
 * @brief 初始化驱动OLED
 */
void oled_init(void)
{

    /* 创建一个OLED互斥量 */
    oled_mutex = rt_mutex_create("oled_m", RT_IPC_FLAG_FIFO);

    //初始相关参数
    clear_screen(_SYNC_A);
    font_table_set(FONT_LIB_EUR2);
    display_mode_set(_DISPLAY_ON, _CURSOR_OFF, _BLINK_OFF);
    address_mode_set();
    cgram_init();
    corsor_return();
    clear_screen(_SYNC_A);

}

/**
 * @brief 重置驱动OLED
 * @author Qiu yijie
 * @date 2022.01.05
 */
void oled_reset(void)
{
    /*加锁*/
    rt_mutex_take(oled_mutex, RT_WAITING_FOREVER);

    //初始相关参数
//    clear_screen(_SYNC_A);
    font_table_set(FONT_LIB_EUR2);
    display_mode_set(_DISPLAY_ON, _CURSOR_OFF, _BLINK_OFF);
    address_mode_set();
    cgram_init();
    corsor_return();
//    clear_screen(_SYNC_A);

    /*解锁*/
    rt_mutex_release(oled_mutex);
}
