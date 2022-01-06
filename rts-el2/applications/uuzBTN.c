#include "uuzBTN.h"
#include "uuzINIT.h"
#include <agile_button.h>
#include <uuzUI.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME "BTN "
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

static agile_btn_t* key0 = RT_NULL;
static agile_btn_t* key1 = RT_NULL;
static agile_btn_t* key2 = RT_NULL;
static agile_btn_t* key3 = RT_NULL;
static agile_btn_t* key4 = RT_NULL;

//短按判断
static void btn_click_event_cb(agile_btn_t* btn)
{
    //LOG_I("[click] pin:%d state:%d repeat:%d, hold_time:%d", btn->pin, btn->state, btn->repeat_cnt, btn->hold_time);

    //按键时长是足够
    if (btn->hold_time > 50) {
        //通过界面判断入口
        ui_short_event(btn->pin);
    } else {
        btn->hold_cycle_time = 250;
        xSta[_S_KEY_LONG] = 0;
    }
}

//长按判断，每隔1秒触发1次
static void btn_hold_event_cb(agile_btn_t* btn)
{
    //LOG_I("[click] pin:%d state:%d repeat:%d, hold_time:%d", btn->pin + 100, btn->state, btn->repeat_cnt, btn->hold_time);

    //长按超过1秒表示有按键
    if (btn->hold_time > _BTN_LONG_INTERVAL) {
        if (xUI.cID != uuzUI_LIGHT_CYCLE) {
            if (btn->hold_time >= 2 * _BTN_LONG_INTERVAL) {
                btn->hold_cycle_time = 175;
            } else if (btn->hold_time >= 3 * _BTN_LONG_INTERVAL) {
                btn->hold_cycle_time = 125;
            } else if (btn->hold_time >= 4 * _BTN_LONG_INTERVAL) {
                btn->hold_cycle_time = 50;
            } else {
                btn->hold_cycle_time = 250;
            }
        } else {
            btn->hold_cycle_time = 500;
        }
        xSta[_S_KEY_LONG] = 1;
        //通过界面判断入口
        ui_hold_event(btn->pin + 100);
    }
}

void uuz_btn_init(void)
{
    if (key0 == RT_NULL) {
        key0 = agile_btn_create(KEY_DOWN, PIN_LOW, PIN_MODE_INPUT);
        agile_btn_set_event_cb(key0, BTN_CLICK_EVENT, btn_click_event_cb);
        agile_btn_set_event_cb(key0, BTN_HOLD_EVENT, btn_hold_event_cb);
        agile_btn_start(key0);
    }

    if (key1 == RT_NULL) {
        key1 = agile_btn_create(KEY_ENTER, PIN_LOW, PIN_MODE_INPUT);
        agile_btn_set_event_cb(key1, BTN_CLICK_EVENT, btn_click_event_cb);
        agile_btn_set_event_cb(key1, BTN_HOLD_EVENT, btn_hold_event_cb);
        agile_btn_start(key1);
    }

    if (key2 == RT_NULL) {
        key2 = agile_btn_create(KEY_LIGHT, PIN_LOW, PIN_MODE_INPUT);
        agile_btn_set_event_cb(key2, BTN_CLICK_EVENT, btn_click_event_cb);
        agile_btn_set_event_cb(key2, BTN_HOLD_EVENT, btn_hold_event_cb);
        agile_btn_start(key2);
    }
    if (key3 == RT_NULL) {
        key3 = agile_btn_create(KEY_UP, PIN_LOW, PIN_MODE_INPUT);
        agile_btn_set_event_cb(key3, BTN_CLICK_EVENT, btn_click_event_cb);
        agile_btn_set_event_cb(key3, BTN_HOLD_EVENT, btn_hold_event_cb);
        agile_btn_start(key3);
    }

    if (key4 == RT_NULL) {
        key4 = agile_btn_create(KEY_BACK, PIN_LOW, PIN_MODE_INPUT);
        agile_btn_set_event_cb(key4, BTN_CLICK_EVENT, btn_click_event_cb);
        agile_btn_set_event_cb(key4, BTN_HOLD_EVENT, btn_hold_event_cb);
        agile_btn_start(key4);
    }
}
