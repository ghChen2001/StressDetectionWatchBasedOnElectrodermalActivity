// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.2.0
// Project name: SquareLine_Project

#include "../ui.h"
#include "../ui_events.h"

void ui_Home_screen_init(void)
{
    ui_Home = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Home, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    // lv_obj_set_style_bg_img_src(ui_Home, &ui_img_189584499, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_img_opa(ui_Home, 160, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Hour = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Hour, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Hour, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Hour, -60);
    lv_obj_set_y(ui_Hour, -90);
    lv_obj_set_align(ui_Hour, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Hour, "00");
    lv_obj_set_style_text_color(ui_Hour, lv_color_hex(0xE2C3F5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Hour, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Hour, &ui_font_Number_big, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Minute = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Minute, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Minute, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Minute, 46);
    lv_obj_set_y(ui_Minute, 10);
    lv_obj_set_align(ui_Minute, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Minute, "00");
    lv_obj_set_style_text_color(ui_Minute, lv_color_hex(0xF4A1EA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Minute, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Minute, &ui_font_Number_big, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Weekday = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Weekday, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Weekday, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Weekday, 75);
    lv_obj_set_y(ui_Weekday, -130);
    lv_obj_set_align(ui_Weekday, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Weekday, "星期三");
    lv_obj_set_style_text_font(ui_Weekday, &ui_font_Chinese, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Date = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Date, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Date, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Date, 95);
    lv_obj_set_y(ui_Date, -97);
    lv_obj_set_align(ui_Date, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Date, "00");
    lv_obj_set_style_text_font(ui_Date, &ui_font_DateFont, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Mon = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Mon, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Mon, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Mon, 62);
    lv_obj_set_y(ui_Mon, -97);
    lv_obj_set_align(ui_Mon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Mon, "00.");
    lv_obj_set_style_text_font(ui_Mon, &ui_font_DateFont, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Second = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Second, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Second, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Second, 133);
    lv_obj_set_y(ui_Second, 30);
    lv_obj_set_align(ui_Second, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Second, "00");
    lv_obj_set_style_text_color(ui_Second, lv_color_hex(0xB58DFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Second, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Second, &ui_font_SecondFont, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label2 = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label2, 73);
    lv_obj_set_y(ui_Label2, -61);
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "2023");
    lv_obj_set_style_text_font(ui_Label2, &ui_font_DateFont, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_AD5940 = lv_label_create(ui_Home);
    lv_obj_set_width(ui_AD5940, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_AD5940, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_AD5940, -60);
    lv_obj_set_y(ui_AD5940, -165);
    lv_obj_set_align(ui_AD5940, LV_ALIGN_CENTER);
    lv_label_set_text(ui_AD5940, "AD5940 Initializing...");
    lv_obj_set_style_text_font(ui_AD5940, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_AD5940, LV_OBJ_FLAG_HIDDEN);

    ui_ArcHr = lv_arc_create(ui_Home);
    lv_obj_set_width(ui_ArcHr, 75);
    lv_obj_set_height(ui_ArcHr, 75);
    lv_obj_set_x(ui_ArcHr, -100);
    lv_obj_set_y(ui_ArcHr, 120);
    lv_obj_set_align(ui_ArcHr, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_ArcHr, LV_STATE_DISABLED);       /// States
    lv_obj_add_flag(ui_ArcHr, LV_OBJ_FLAG_EVENT_BUBBLE); /// Flags
    lv_obj_clear_flag(ui_ArcHr, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_arc_set_value(ui_ArcHr, 50);
    lv_obj_set_style_arc_width(ui_ArcHr, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(ui_ArcHr, lv_color_hex(0xBB0020), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_ArcHr, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_ArcHr, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_ArcHr, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ArcHr, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_ArcStep = lv_arc_create(ui_Home);
    lv_obj_set_width(ui_ArcStep, 75);
    lv_obj_set_height(ui_ArcStep, 75);
    lv_obj_set_x(ui_ArcStep, -10);
    lv_obj_set_y(ui_ArcStep, 120);
    lv_obj_set_align(ui_ArcStep, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_ArcStep, LV_STATE_DISABLED);       /// States
    lv_obj_add_flag(ui_ArcStep, LV_OBJ_FLAG_EVENT_BUBBLE); /// Flags
    lv_obj_clear_flag(ui_ArcStep, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_arc_set_range(ui_ArcStep, 0, 4000);
    lv_arc_set_value(ui_ArcStep, 0);
    lv_obj_set_style_arc_width(ui_ArcStep, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(ui_ArcStep, lv_color_hex(0xEB8334), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_ArcStep, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_ArcStep, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_ArcStep, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ArcStep, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_ArcMental = lv_arc_create(ui_Home);
    lv_obj_set_width(ui_ArcMental, 75);
    lv_obj_set_height(ui_ArcMental, 75);
    lv_obj_set_x(ui_ArcMental, 80);
    lv_obj_set_y(ui_ArcMental, 120);
    lv_obj_set_align(ui_ArcMental, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_ArcMental, LV_STATE_DISABLED);       /// States
    lv_obj_add_flag(ui_ArcMental, LV_OBJ_FLAG_EVENT_BUBBLE); /// Flags
    lv_obj_clear_flag(ui_ArcMental, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_arc_set_value(ui_ArcMental, 50);
    lv_obj_set_style_arc_width(ui_ArcMental, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(ui_ArcMental, lv_color_hex(0x9B31BF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_ArcMental, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_ArcMental, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_ArcMental, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ArcMental, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    // ui_ArcStep = lv_arc_create(ui_Home);
    // lv_obj_set_width(ui_ArcStep, 75);
    // lv_obj_set_height(ui_ArcStep, 75);
    // lv_obj_set_x(ui_ArcStep, -10);
    // lv_obj_set_y(ui_ArcStep, 100);
    // lv_obj_set_align(ui_ArcStep, LV_ALIGN_CENTER);
    // lv_obj_add_state(ui_ArcStep, LV_STATE_DISABLED);       /// States
    // lv_obj_add_flag(ui_ArcStep, LV_OBJ_FLAG_EVENT_BUBBLE); /// Flags
    // lv_obj_clear_flag(ui_ArcStep, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    // lv_arc_set_value(ui_ArcStep, 50);
    // lv_obj_set_style_arc_width(ui_ArcStep, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    // lv_obj_set_style_arc_color(ui_ArcStep, lv_color_hex(0xEB8334), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // lv_obj_set_style_arc_opa(ui_ArcStep, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // lv_obj_set_style_arc_width(ui_ArcStep, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // lv_obj_set_style_bg_color(ui_ArcStep, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(ui_ArcStep, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    // ui_ArcMental = lv_arc_create(ui_Home);
    // lv_obj_set_width(ui_ArcMental, 75);
    // lv_obj_set_height(ui_ArcMental, 75);
    // lv_obj_set_x(ui_ArcMental, 80);
    // lv_obj_set_y(ui_ArcMental, 100);
    // lv_obj_set_align(ui_ArcMental, LV_ALIGN_CENTER);
    // lv_obj_add_state(ui_ArcMental, LV_STATE_DISABLED);       /// States
    // lv_obj_add_flag(ui_ArcMental, LV_OBJ_FLAG_EVENT_BUBBLE); /// Flags
    // lv_obj_clear_flag(ui_ArcMental, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    // lv_arc_set_value(ui_ArcMental, 50);
    // lv_obj_set_style_arc_width(ui_ArcMental, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    // lv_obj_set_style_arc_color(ui_ArcMental, lv_color_hex(0x9B31BF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // lv_obj_set_style_arc_opa(ui_ArcMental, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    // lv_obj_set_style_arc_width(ui_ArcMental, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // lv_obj_set_style_bg_color(ui_ArcMental, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(ui_ArcMental, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    // ui_LabelMental = lv_label_create(ui_Home);
    // lv_obj_set_width(ui_LabelMental, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(ui_LabelMental, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(ui_LabelMental, 40);
    // lv_obj_set_y(ui_LabelMental, 100);
    // lv_obj_set_align(ui_LabelMental, LV_ALIGN_CENTER);
    // lv_label_set_text(ui_LabelMental, "----");
    // lv_obj_set_style_text_color(ui_LabelMental, lv_color_hex(0xA1FDFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_font(ui_LabelMental, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_psb = lv_label_create(ui_Home);
    // lv_obj_set_width(ui_psb, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(ui_psb, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(ui_psb, 40);
    // lv_obj_set_y(ui_psb, 130);
    // lv_obj_set_align(ui_psb, LV_ALIGN_CENTER);
    // lv_label_set_text(ui_psb, "--%");
    // lv_obj_set_style_text_color(ui_psb, lv_color_hex(0xA1FDFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_font(ui_psb, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelHR = lv_label_create(ui_Home);
    lv_obj_set_width(ui_LabelHR, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_LabelHR, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_LabelHR, -100);
    lv_obj_set_y(ui_LabelHR, 120);
    lv_obj_set_align(ui_LabelHR, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelHR, "85");
    lv_obj_set_style_text_font(ui_LabelHR, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabelTemp = lv_label_create(ui_Home);
    // lv_obj_set_width(ui_LabelTemp, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(ui_LabelTemp, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(ui_LabelTemp, -119);
    // lv_obj_set_y(ui_LabelTemp, 166);
    // lv_obj_set_align(ui_LabelTemp, LV_ALIGN_CENTER);
    // lv_label_set_text(ui_LabelTemp, "温度");
    // lv_obj_set_style_text_font(ui_LabelTemp, &ui_font_TempFont, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabeHR = lv_label_create(ui_Home);
    // lv_obj_set_width(ui_LabeHR, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height(ui_LabeHR, LV_SIZE_CONTENT); /// 1
    // lv_obj_set_x(ui_LabeHR, -101);
    // lv_obj_set_y(ui_LabeHR, 133);
    // lv_obj_set_align(ui_LabeHR, LV_ALIGN_CENTER);
    // lv_label_set_text(ui_LabeHR, "HR");
    // lv_obj_set_style_text_font(ui_LabeHR, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageWifi = lv_img_create(ui_Home);
    lv_img_set_src(ui_ImageWifi, &ui_img_wifi_png);
    lv_obj_set_width(ui_ImageWifi, LV_SIZE_CONTENT);  /// 28
    lv_obj_set_height(ui_ImageWifi, LV_SIZE_CONTENT); /// 28
    lv_obj_set_x(ui_ImageWifi, 112);
    lv_obj_set_y(ui_ImageWifi, -172);
    lv_obj_set_align(ui_ImageWifi, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageWifi, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageWifi, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_ImageWifiOff = lv_img_create(ui_Home);
    lv_img_set_src(ui_ImageWifiOff, &ui_img_1560645021);
    lv_obj_set_width(ui_ImageWifiOff, LV_SIZE_CONTENT);  /// 28
    lv_obj_set_height(ui_ImageWifiOff, LV_SIZE_CONTENT); /// 28
    lv_obj_set_x(ui_ImageWifiOff, 112);
    lv_obj_set_y(ui_ImageWifiOff, -172);
    lv_obj_set_align(ui_ImageWifiOff, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageWifiOff, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageWifiOff, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_ImageBle = lv_img_create(ui_Home);
    lv_img_set_src(ui_ImageBle, &ui_img_bluetoothon_png);
    lv_obj_set_width(ui_ImageBle, LV_SIZE_CONTENT);  /// 36
    lv_obj_set_height(ui_ImageBle, LV_SIZE_CONTENT); /// 36
    lv_obj_set_x(ui_ImageBle, 75);
    lv_obj_set_y(ui_ImageBle, -170);
    lv_obj_set_align(ui_ImageBle, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageBle, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageBle, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_add_flag(ui_ImageBle, LV_OBJ_FLAG_HIDDEN);

    ui_ImageBleOff = lv_img_create(ui_Home);
    lv_img_set_src(ui_ImageBleOff, &ui_img_bluetoothoff_png);
    lv_obj_set_width(ui_ImageBleOff, LV_SIZE_CONTENT);  /// 36
    lv_obj_set_height(ui_ImageBleOff, LV_SIZE_CONTENT); /// 36
    lv_obj_set_x(ui_ImageBleOff, 75);
    lv_obj_set_y(ui_ImageBleOff, -170);
    lv_obj_set_align(ui_ImageBleOff, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageBleOff, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageBleOff, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_ImageFootStep = lv_img_create(ui_Home);
    lv_img_set_src(ui_ImageFootStep, &ui_img_footsteps_png);
    lv_obj_set_width(ui_ImageFootStep, LV_SIZE_CONTENT);  /// 16
    lv_obj_set_height(ui_ImageFootStep, LV_SIZE_CONTENT); /// 16
    lv_obj_set_x(ui_ImageFootStep, -10);
    lv_obj_set_y(ui_ImageFootStep, 153);
    lv_obj_set_align(ui_ImageFootStep, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageFootStep, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageFootStep, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_stepCntLabel = lv_label_create(ui_Home);
    lv_obj_set_width(ui_stepCntLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_stepCntLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_stepCntLabel, -10);
    lv_obj_set_y(ui_stepCntLabel, 120);
    lv_obj_set_align(ui_stepCntLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_stepCntLabel, "0");

    ui_ImageHR = lv_img_create(ui_Home);
    lv_img_set_src(ui_ImageHR, &ui_img_pulse_png);
    lv_obj_set_width(ui_ImageHR, LV_SIZE_CONTENT);  /// 20
    lv_obj_set_height(ui_ImageHR, LV_SIZE_CONTENT); /// 20
    lv_obj_set_x(ui_ImageHR, -100);
    lv_obj_set_y(ui_ImageHR, 153);
    lv_obj_set_align(ui_ImageHR, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageHR, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageHR, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_ImageMental = lv_img_create(ui_Home);
    lv_img_set_src(ui_ImageMental, &ui_img_1916941551);
    lv_obj_set_width(ui_ImageMental, LV_SIZE_CONTENT);  /// 20
    lv_obj_set_height(ui_ImageMental, LV_SIZE_CONTENT); /// 20
    lv_obj_set_x(ui_ImageMental, 80);
    lv_obj_set_y(ui_ImageMental, 153);
    lv_obj_set_align(ui_ImageMental, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageMental, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageMental, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_ImageStress = lv_img_create(ui_Home);
    lv_img_set_src(ui_ImageStress, &ui_img_208425557);
    lv_obj_set_width(ui_ImageStress, LV_SIZE_CONTENT);  /// 42
    lv_obj_set_height(ui_ImageStress, LV_SIZE_CONTENT); /// 42
    lv_obj_set_x(ui_ImageStress, 80);
    lv_obj_set_y(ui_ImageStress, 120);
    lv_obj_set_align(ui_ImageStress, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageStress, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageStress, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_add_flag(ui_ImageStress, LV_OBJ_FLAG_HIDDEN);

    ui_ImageNeutral = lv_img_create(ui_Home);
    lv_img_set_src(ui_ImageNeutral, &ui_img_872619278);
    lv_obj_set_width(ui_ImageNeutral, LV_SIZE_CONTENT);  /// 42
    lv_obj_set_height(ui_ImageNeutral, LV_SIZE_CONTENT); /// 42
    lv_obj_set_x(ui_ImageNeutral, 80);
    lv_obj_set_y(ui_ImageNeutral, 120);
    lv_obj_set_align(ui_ImageNeutral, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageNeutral, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageNeutral, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_WeatherImage = lv_img_create(ui_Home);
    lv_img_set_src(ui_WeatherImage, &ui_img_1711384410);
    lv_obj_set_width(ui_WeatherImage, LV_SIZE_CONTENT);  /// 56
    lv_obj_set_height(ui_WeatherImage, LV_SIZE_CONTENT); /// 54
    lv_obj_set_x(ui_WeatherImage, -117);
    lv_obj_set_y(ui_WeatherImage, 14);
    lv_obj_set_align(ui_WeatherImage, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_WeatherImage, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_WeatherImage, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_ImageLocation = lv_img_create(ui_Home);
    lv_img_set_src(ui_ImageLocation, &ui_img_location_png);
    lv_obj_set_width(ui_ImageLocation, LV_SIZE_CONTENT);  /// 16
    lv_obj_set_height(ui_ImageLocation, LV_SIZE_CONTENT); /// 16
    lv_obj_set_x(ui_ImageLocation, -125);
    lv_obj_set_y(ui_ImageLocation, -30);
    lv_obj_set_align(ui_ImageLocation, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageLocation, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(ui_ImageLocation, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_WeatherLabel1 = lv_label_create(ui_Home);
    lv_obj_set_width(ui_WeatherLabel1, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_WeatherLabel1, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_WeatherLabel1, -55);
    lv_obj_set_y(ui_WeatherLabel1, -4);
    lv_obj_set_align(ui_WeatherLabel1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherLabel1, "N/A");
    lv_obj_set_style_text_font(ui_WeatherLabel1, &ui_font_TempFont, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WeatherLabel2 = lv_label_create(ui_Home);
    lv_obj_set_width(ui_WeatherLabel2, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_WeatherLabel2, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_WeatherLabel2, -57);
    lv_obj_set_y(ui_WeatherLabel2, 25);
    lv_obj_set_align(ui_WeatherLabel2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WeatherLabel2, "N/A ℃");
    lv_obj_set_style_text_font(ui_WeatherLabel2, &ui_font_TempFont, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelCity = lv_label_create(ui_Home);
    lv_obj_set_width(ui_LabelCity, 100);
    lv_obj_set_height(ui_LabelCity, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_LabelCity, -60);
    lv_obj_set_y(ui_LabelCity, -30);
    lv_obj_set_align(ui_LabelCity, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelCity, "N/A");
    lv_obj_set_style_text_align(ui_LabelCity, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelCity, &ui_font_TempFont, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Home, ui_event_Home, LV_EVENT_ALL, NULL);
}

void ui_UpdateHRLabel(int16_t hr)
{
    char strtemp[4] = { 0 };
    itoa(hr, strtemp, 10);
    lv_label_set_text(ui_LabelHR, strtemp);

    lv_arc_set_value(ui_ArcHr, hr / 2.5);
}

void ui_UpdateStepLabel(int16_t step)
{
    char strtemp[4] = { 0 };
    itoa(step, strtemp, 10);
    lv_label_set_text(ui_stepCntLabel, strtemp);

    lv_arc_set_value(ui_ArcStep, step);
}

void ui_UpdateSecLabel(uint8_t sec)
{
    char strtemp[3] = { 0 };
    char strtemp2[3] = "0";
    itoa(sec, strtemp, 10);
    if (sec < 10) {
        strcat(strtemp2, strtemp);
        lv_label_set_text(ui_Second, strtemp2);
    } else {
        lv_label_set_text(ui_Second, strtemp);
    }
}

void ui_UpdateMinLabel(uint8_t min)
{
    char strtemp[3] = { 0 };
    char strtemp2[3] = "0";
    itoa(min, strtemp, 10);
    if (min < 10) {
        strcat(strtemp2, strtemp);
        lv_label_set_text(ui_Minute, strtemp2);
    } else {
        lv_label_set_text(ui_Minute, strtemp);
    }
}

void ui_UpdateHourLabel(uint8_t hour)
{
    char strtemp[3] = { 0 };
    char strtemp2[3] = "0";
    itoa(hour, strtemp, 10);
    if (hour < 10) {
        strcat(strtemp2, strtemp);
        lv_label_set_text(ui_Hour, strtemp2);
    } else {
        lv_label_set_text(ui_Hour, strtemp);
    }
}

void ui_UpdateDateLabel(uint8_t date)
{
    char strtemp[3] = { 0 };
    char strtemp2[3] = "0";
    itoa(date, strtemp, 10);
    if (date < 10) {
        strcat(strtemp2, strtemp);
        lv_label_set_text(ui_Date, strtemp2);
    } else {
        lv_label_set_text(ui_Date, strtemp);
    }
}

void ui_UpdateMonLabel(uint8_t mon)
{
    char strtemp[4] = { 0 };
    char strtemp2[4] = "0";
    itoa(mon, strtemp, 10);
    if (mon < 10) {
        strcat(strtemp2, strtemp);
        strcat(strtemp2, ".");
        lv_label_set_text(ui_Mon, strtemp2);
    } else {
        strcat(strtemp, ".");
        lv_label_set_text(ui_Mon, strtemp);
    }
}

void ui_UpdateYearLabel(uint16_t year)
{
    char strtemp[5] = { 0 };
    itoa(year, strtemp, 10);
    lv_label_set_text(ui_Label2, strtemp);
}

void ui_UpdateWeekLabel(uint8_t week)
{
    switch (week) {
        case 1:
            lv_label_set_text(ui_Weekday, "星期一");
            break;
        case 2:
            lv_label_set_text(ui_Weekday, "星期二");
            break;
        case 3:
            lv_label_set_text(ui_Weekday, "星期三");
            break;
        case 4:
            lv_label_set_text(ui_Weekday, "星期四");
            break;
        case 5:
            lv_label_set_text(ui_Weekday, "星期五");
            break;
        case 6:
            lv_label_set_text(ui_Weekday, "星期六");
            break;
        case 0:
            lv_label_set_text(ui_Weekday, "星期日");
            break;

        default:
            break;
    }
}

void ui_UpdateMentalLabel(int classNum, float posb)
{
    // char strtemp[4];

    if (classNum == 0) {
        // lv_label_set_text(ui_LabelMental, "Neutral");
        lv_obj_add_flag(ui_ImageStress, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_ImageNeutral, LV_OBJ_FLAG_HIDDEN);
    } else if (classNum == 1) {
        // lv_label_set_text(ui_LabelMental, "Stress");
        lv_obj_add_flag(ui_ImageNeutral, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_ImageStress, LV_OBJ_FLAG_HIDDEN);
    } else {
        // lv_label_set_text(ui_LabelMental, "Amusement");
    }

    // sprintf(strtemp, "%.0f%%", posb * 100);
    // lv_label_set_text(ui_psb, strtemp);
    lv_arc_set_value(ui_ArcMental, (posb - 0.5) * 200);
}

void ui_setWifiImg(uint8_t isConnected)
{
    if (isConnected) {
        lv_obj_add_flag(ui_ImageWifiOff, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_ImageWifi, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(ui_ImageWifiOff, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_ImageWifi, LV_OBJ_FLAG_HIDDEN);
    }
}

void ui_setBleImg(uint8_t isConnected)
{
    if (isConnected) {
        lv_obj_add_flag(ui_ImageBleOff, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_ImageBle, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(ui_ImageBleOff, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_ImageBle, LV_OBJ_FLAG_HIDDEN);
    }
}

void ui_updateWeather(char *cityName, char *weatherText, uint8_t weatherCode, char *weatherTemp)
{
    lv_label_set_text(ui_LabelCity, cityName);
    strcat(weatherTemp, "℃");
    lv_label_set_text(ui_WeatherLabel1, weatherText);
    lv_label_set_text(ui_WeatherLabel2, weatherTemp);

    switch (weatherCode) {
        case 0:
            lv_img_set_src(ui_WeatherImage, &ui_img_352157552);
            break;
        case 1:
            lv_img_set_src(ui_WeatherImage, &ui_img_904422901);
            break;
        case 2:
            lv_img_set_src(ui_WeatherImage, &ui_img_352157552);
            break;
        case 3:
            lv_img_set_src(ui_WeatherImage, &ui_img_904422901);
            break;
        case 4:
            lv_img_set_src(ui_WeatherImage, &ui_img_1114156892);
            break;
        case 5:
            lv_img_set_src(ui_WeatherImage, &ui_img_1666422241);
            break;
        case 6:
            lv_img_set_src(ui_WeatherImage, &ui_img_1395092270);
            break;
        case 7:
            lv_img_set_src(ui_WeatherImage, &ui_img_1666422241);
            break;
        case 8:
            lv_img_set_src(ui_WeatherImage, &ui_img_1395092270);
            break;
        case 9:
            lv_img_set_src(ui_WeatherImage, &ui_img_619575779);
            break;
        case 10:
            lv_img_set_src(ui_WeatherImage, &ui_img_700619099);
            break;
        case 11:
            lv_img_set_src(ui_WeatherImage, &ui_img_700722490);
            break;
        case 12:
            lv_img_set_src(ui_WeatherImage, &ui_img_700690717);
            break;
        case 13:
            lv_img_set_src(ui_WeatherImage, &ui_img_700794364);
            break;
        case 14:
            lv_img_set_src(ui_WeatherImage, &ui_img_700475607);
            break;
        case 15:
            lv_img_set_src(ui_WeatherImage, &ui_img_700578998);
            break;
        case 16:
            lv_img_set_src(ui_WeatherImage, &ui_img_700547225);
            break;
        case 17:
            lv_img_set_src(ui_WeatherImage, &ui_img_700650872);
            break;
        case 18:
            lv_img_set_src(ui_WeatherImage, &ui_img_700650872);
            break;
        case 19:
            lv_img_set_src(ui_WeatherImage, &ui_img_700435506);
            break;
        case 20:
            lv_img_set_src(ui_WeatherImage, &ui_img_1587055926);
            break;
        case 21:
            lv_img_set_src(ui_WeatherImage, &ui_img_1587159317);
            break;
        case 22:
            lv_img_set_src(ui_WeatherImage, &ui_img_1587127544);
            break;
        case 23:
            lv_img_set_src(ui_WeatherImage, &ui_img_1587231191);
            break;
        case 24:
            lv_img_set_src(ui_WeatherImage, &ui_img_1586912434);
            break;
        case 25:
            lv_img_set_src(ui_WeatherImage, &ui_img_1587015825);
            break;
        case 26:
            lv_img_set_src(ui_WeatherImage, &ui_img_1586984052);
            break;
        case 27:
            lv_img_set_src(ui_WeatherImage, &ui_img_1586984052);
            break;
        case 28:
            lv_img_set_src(ui_WeatherImage, &ui_img_1586768942);
            break;
        case 29:
            lv_img_set_src(ui_WeatherImage, &ui_img_1586768942);
            break;
        case 30:
            lv_img_set_src(ui_WeatherImage, &ui_img_1659974289);
            break;
        case 31:
            lv_img_set_src(ui_WeatherImage, &ui_img_1660077680);
            break;
        case 32:
            lv_img_set_src(ui_WeatherImage, &ui_img_1660045907);
            break;
        case 33:
            lv_img_set_src(ui_WeatherImage, &ui_img_1660045907);
            break;
        case 34:
            lv_img_set_src(ui_WeatherImage, &ui_img_1659830797);
            break;
        case 35:
            lv_img_set_src(ui_WeatherImage, &ui_img_1659830797);
            break;
        case 36:
            lv_img_set_src(ui_WeatherImage, &ui_img_1659902415);
            break;
        case 37:
            lv_img_set_src(ui_WeatherImage, &ui_img_1660006062);
            break;
        case 38:
            lv_img_set_src(ui_WeatherImage, &ui_img_352157552);
            break;
        case 99:
            lv_img_set_src(ui_WeatherImage, &ui_img_1711384410);
            break;

        default:
            lv_img_set_src(ui_WeatherImage, &ui_img_1711384410);
            break;
    }
}