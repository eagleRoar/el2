#ifndef __UUZ_UI_H
#define __UUZ_UI_H

#include "typedefDEF.h"
#include "typedefUI.h"
#include "typedefINIT.h"
#include "uuzConfig1602.h"

extern UI_Cache_Typedef_t xUI;

#ifdef __cplusplus
extern "C" {
#endif

//启动UI同步线程
void ui_sync_init(void);
void ui_thread_entry(void* parameter);
//界面的按键反应
void ui_short_event(rt_base_t btn_pin);
void ui_hold_event(rt_base_t btn_pin);
//UI的ID和Foucs相关操作
void ui_id_set(u16 id, u8 max);
void foucs_set(u8 foucs);
void foucs_move(u16 key);
u8 foucs_move_cycle(u16 key);
void foucs_single_move(u16 key);
void show_ui(u16 id);
//灯光强度转换
u16 level_state_get(u16 ch, u8 type);
u16 watt_get(u16 level, u8 mode);

#ifdef __cplusplus
}
#endif
#endif // __UUZ_UI_H
