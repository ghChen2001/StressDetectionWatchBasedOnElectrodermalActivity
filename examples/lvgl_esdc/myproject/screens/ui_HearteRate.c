// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.2.0
// Project name: SquareLine_Project

#include "../ui.h"

void ui_HearteRate_screen_init(void)
{
    ui_HearteRate = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_HearteRate, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_img_src(ui_HearteRate, &ui_img_189584499, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui_HearteRate, 120, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_HearteRate, ui_event_HearteRate, LV_EVENT_ALL, NULL);

}
