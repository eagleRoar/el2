#ifndef __UUZ_UI_1309_H
#define __UUZ_UI_1309_H

#include "typedefDEF.h"
#include "typedefUI.h"
#include <u8g2_port.h>

extern u8g2_t u8g2_el2;

#ifdef __cplusplus
extern "C" {
#endif

void show_label(u16 _font, u8 x, u8 y);
u16 level_get(u16 ch, u8 type);
u16 level_single_get(u16 ch, u16 level, u8 type);

//SAVE-TIP界面
void save_disp(void);
void save_opt(u16 keyId);
//FACTORY-RESET-TIP界面
void factory_tip_disp(void);
void factory_tip_opt(u16 keyId);
//LOGO界面
void logo_disp(void);
void logo_opt(u16 keyId);
//SHOW界面
void show_ch_disp(void);
void show_ch_opt(u16 keyId);
void show_value_disp(void);
//LIGHTS界面
void light_disp(void);
void light_opt(u16 keyId);
void light_value_disp(void);
//灯光控制界面
void light_disp(void);
void light_opt(u16 keyId);
void light_disp(void);
//SETUP界面
void setup_disp(void);
void setup_opt(u16 keyId);
void setup_value_disp(void);
//SETUP-CHANNEL-1/2界面
void channel_disp(void);
void channel_opt(u16 keyId);
void channel_value_disp(void);
//SETUP-CHANNEL-OUTPUT_MODE
void output_mode_disp(void);
void output_mode_opt(u16 keyId);
void output_mode_value_disp(void);
//SETUP-CHANNEL-OUTPUT_LEVEL
void output_level_disp(void);
void output_level_opt(u16 keyId);
void output_level_value_disp(void);
//SETUP-CHANNEL-LIGHT_CYCLE
void light_cycle_disp(void);
void light_cycle_opt(u16 keyId);
void light_cycle_value_disp(void);
//SETUP-CHANNEL-SUNRISE-SUNSET
void sunrise_sunset_disp(void);
void sunrise_sunset_opt(u16 keyId);
void sunrise_sunset_value_disp(void);
//SETUP-CHANNEL-ECM-CONFIG
void ecm_config_disp(void);
void ecm_config_opt(u16 keyId);
//SETUP-CHANNEL-DISPLAY_MODE
void display_mode_disp(void);
void display_mode_opt(u16 keyId);
void display_mode_value_disp(void);
//SETUP-TEMP-PROBE界面
void temp_probe_disp(void);
void temp_probe_opt(u16 keyId);
void temp_probe_value_disp(void);
//SETUP-TEMP-UNIT界面
void temp_unit_disp(void);
void temp_unit_opt(u16 keyId);
//SETUP-TEMP-CALIBRATION界面
void temp_cal_disp(void);
void temp_cal_opt(u16 keyId);
void temp_cal_value_disp(void);
//SETUP-LANGUAGE界面
void language_config_disp(void);
void language_config_opt(u16 keyId);
//SETUP-SYSTEM-INFO界面
void system_info_disp(void);
void system_info_opt(u16 keyId);
//SETUP-FACTORY-RESET界面
void factory_reset_disp(void);
void factory_reset_opt(u16 keyId);

#ifdef __cplusplus
}
#endif
#endif // __UUZ_UI_1309_H
