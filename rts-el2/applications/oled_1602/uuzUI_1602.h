#ifndef __UUZ_UI_1602_H
#define __UUZ_UI_1602_H

#include "typedefDEF.h"
#include "typedefUI.h"
#include "uuzConfig1602.h"

extern UI_Cache_Typedef_t xUI;
extern struct _ui_init_t xInit;

#ifdef __cplusplus
extern "C" {
#endif

//通用函数
void value_set(Value_Typedef_t* xValue, u32 h, u32 w);
void disp_txt(rt_uint8_t type, Value_Typedef_t* xValue, char* cmd);
void disp_value(rt_uint8_t type, Value_Typedef_t* xValue, char* cmd);
void disp_time(rt_uint8_t type, Value_Typedef_t* xValue);
void disp_date(rt_uint8_t type, Value_Typedef_t* xValue);
void disp_temperature(rt_uint8_t type, Value_Typedef_t* xValue, char* cmd);
void disp_cursor(rt_uint8_t type, Value_Typedef_t* xValue);
void disp_units(rt_uint8_t type, u8 h, u8 w, u8 tip);
void btn_long_event(void);
u16 level_get(u16 ch, u8 type);

//LOGO界面
void logo_init(void);
void logo_disp(void);
void logo_opt(u16 keyId);
//SAVE-TIP界面
void save_disp(void);
void save_opt(u16 keyId);
//SHOW-CHANNEL界面
void show_ch_disp(void);
void show_ch_opt(u16 keyId);
void ch1_info_disp(u8 type);
void ch2_info_disp(u8 type);
//SHOW-DATE界面
void show_date_init(void);
void show_date_disp(void);
void show_date_opt(u16 keyId);
void date_data_disp(u8 type, u8 step);
//SHOW-TA界面
void show_ta_init(void);
void show_ta_disp(void);
void show_ta_opt(u16 keyId);
void ta_data_disp(u8 type, u8 step);
//LIGHT INFO界面
void light_init(void);
void light_disp(void);
void light_opt(u16 keyId);
void light_title_disp(u8 ch);
void light_level_disp(u8 type, u8 ch);
void light_watt_disp(u8 type, u8 ch);
//MENU LIST
void menu_init(void);
void menu_disp(void);
void menu_opt(u16 keyId);
void menu_list_disp(void);
//LOGS LIST
void logs_init(void);
void logs_disp(void);
void logs_opt(u16 keyId);
void logs_info_disp(void);
//CHANNEL INFO界面
void channel_init(void);
void channel_disp(void);
void channel_opt(u16 keyId);
void channel_list_disp(void);
//SETUP-CHANNEL-OUTPUT_MODE
void output_mode_init(void);
void output_mode_disp(void);
void output_mode_opt(u16 keyId);
//SETUP-CHANNEL-OUTPUT_LEVEL
void output_level_init(void);
void output_level_disp(void);
void output_level_opt(u16 keyId);
//SETUP-CHANNEL-LIGHT CYCLE
void light_cycle_init(void);
void light_cycle_disp(void);
void light_cycle_opt(u16 keyId);
void light_cycle_value_disp(u8 type, u8 step);
//SETUP-CHANNEL-SET-TEMP-LEVELS
void set_temp_levels_init(void);
void set_temp_levels_disp(void);
void set_temp_levels_opt(u16 keyId);
void set_temp_levels_cursor_disp(u8 type);
void set_temp_levels_title_disp(u8 type);
void set_temp_levels_value_disp(u8 type, u8 step);
u16 set_temp_limit_get(u8 type, u16 foucs);
//SETUP-CHANNEL-SUNRISE-SUNSET
void sunrise_sunset_init(void);
void sunrise_sunset_disp(void);
void sunrise_sunset_opt(u16 keyId);
void sunrise_sunset_cursor_disp(void);
void sunrise_sunset_value_disp(u8 type, u8 step);
//SETUP-CHANNEL-ECM-CONFIG
void ecm_config_init(void);
void ecm_config_disp(void);
void ecm_config_opt(u16 keyId);
//SETUP-CHANNEL-DISPLAY_MODE
void display_mode_init(void);
void display_mode_disp(void);
void display_mode_opt(u16 keyId);
void display_mode_value_disp(void);
//TEMP PROBE界面
void temp_units_init(void);
void temp_units_disp(void);
void temp_units_opt(u16 keyId);
void temp_units_value_disp(void);
//TEMP SELECT界面
void temp_select_disp(void);
void temp_select_opt(u16 keyId);
//TEMP CAL界面
void temp_cal_init(void);
void temp_cal_disp(void);
void temp_cal_opt(u16 keyId);
void temp_cal_cursor_disp(void);
void temp_cal_value_disp(u8 step);
//CH2 MODE界面
void ch2_mode_init(void);
void ch2_mode_disp(void);
void ch2_mode_opt(u16 keyId);
void ch2_mode_value_disp(u8 type);
//LANGUAGE SET界面
void lang_init(void);
void lang_disp(void);
void lang_opt(u16 keyId);
//TIME SET界面
void time_disp(void);
void time_opt(u16 keyId);
void time_info_disp(u8 type);
void time_set_disp(u8 type);
void date_set_disp(u8 type);
//DEVICE INFO界面
void info_disp(void);
void info_opt(u16 keyId);
//FACTORY RESET初始化复位界面
void factory_reset_init(void);
void factory_reset_disp(void);
void factory_reset_opt(u16 keyId);
void reset_btn_disp(u8 type);

#ifdef __cplusplus
}
#endif
#endif // __UUZ_UI_H
