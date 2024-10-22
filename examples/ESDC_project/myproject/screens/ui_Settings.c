// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.2.0
// Project name: SquareLine_Project

#include "../ui.h"
#include "../ui_events.h"
#include "main_head.h"
#include "lv_port_disp.h"

void ui_onWifiChanged(lv_event_t *e);
void ui_onBleChanged(lv_event_t *e);
void ui_onAccEnabled(lv_event_t *e);
void ui_onEdaEnabled(lv_event_t *e);
void ui_onTempEnabled(lv_event_t *e);
void ui_onHrEnabled(lv_event_t *e);
void ui_onAlgoEnabled(lv_event_t *e);
void ui_onMscEnabled(lv_event_t *e);
void ui_onBrightnessSliderChanged(lv_event_t *e);

void ui_Settings_screen_init(void)
{
    ui_Settings = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Settings, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_CHAIN_HOR); /// Flags
    lv_obj_set_style_bg_color(ui_Settings, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Flex Layout.
    lv_obj_t *flex_container = lv_obj_create(ui_Settings);
    lv_obj_set_layout(flex_container, LV_LAYOUT_FLEX);
    lv_obj_set_size(flex_container, 230, 215);
    lv_obj_set_x(flex_container, 5);
    lv_obj_set_y(flex_container, 45);
    lv_obj_set_flex_align(flex_container, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_flex_flow(flex_container, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_row(flex_container, 20, LV_PART_MAIN);

    ui_SettingsLabel = lv_label_create(ui_Settings);
    lv_obj_set_width(ui_SettingsLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_SettingsLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_SettingsLabel, 80);
    lv_obj_set_y(ui_SettingsLabel, 8);
    lv_label_set_text(ui_SettingsLabel, "Settings");
    lv_obj_set_style_text_color(ui_SettingsLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SettingsLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SettingsLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // BLE Switch
    ui_Switch_BLE_Label = lv_label_create(flex_container);
    lv_obj_set_width(ui_Switch_BLE_Label, 100); /// 1
    lv_obj_set_height(ui_Switch_BLE_Label, 30); /// 1
    lv_obj_set_x(ui_Switch_BLE_Label, 20);
    lv_obj_set_y(ui_Switch_BLE_Label, 55);
    lv_label_set_text(ui_Switch_BLE_Label, "Bluetooth");
    // lv_obj_set_style_text_align(ui_Switch_BLE_Label, LV_ALIGN_LEFT_MID, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_Switch_BLE_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Switch_BLE_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Switch_BLE_Label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch_BLE = lv_switch_create(flex_container);
    lv_obj_set_width(ui_Switch_BLE, 60);  /// 1
    lv_obj_set_height(ui_Switch_BLE, 30); /// 1
    lv_obj_set_x(ui_Switch_BLE, 150);
    lv_obj_set_y(ui_Switch_BLE, 50);
    lv_obj_add_state(ui_Switch_BLE, LV_STATE_CHECKED);
    lv_obj_add_event_cb(ui_Switch_BLE, ui_onBleChanged, LV_EVENT_CLICKED, NULL);

    // Wifi Switch
    ui_Switch_WIFI_Label = lv_label_create(flex_container);
    lv_obj_set_width(ui_Switch_WIFI_Label, 100); /// 1
    lv_obj_set_height(ui_Switch_WIFI_Label, 30); /// 1
    // lv_obj_set_x(ui_Switch_WIFI_Label, 20);
    // lv_obj_set_y(ui_Switch_WIFI_Label, 55);
    lv_label_set_text(ui_Switch_WIFI_Label, "Wifi");
    // lv_obj_set_style_text_align(ui_Switch_BLE_Label, LV_ALIGN_LEFT_MID, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_Switch_WIFI_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Switch_WIFI_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Switch_WIFI_Label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch_WIFI = lv_switch_create(flex_container);
    lv_obj_set_width(ui_Switch_WIFI, 60);  /// 1
    lv_obj_set_height(ui_Switch_WIFI, 30); /// 1
    // lv_obj_set_x(ui_Switch_WIFI, 150);
    // lv_obj_set_y(ui_Switch_WIFI, 50);
    lv_obj_add_state(ui_Switch_WIFI, LV_STATE_CHECKED);
    lv_obj_add_event_cb(ui_Switch_WIFI, ui_onWifiChanged, LV_EVENT_CLICKED, NULL);

    // Brightness Slider
    ui_Label_Brightness = lv_label_create(flex_container);
    lv_obj_set_width(ui_Label_Brightness, 200); /// 1
    lv_obj_set_height(ui_Label_Brightness, 20); /// 1
    lv_label_set_text(ui_Label_Brightness, "Brightness");
    // lv_obj_set_style_text_align(ui_Label_Brightness, LV_ALIGN_LEFT_MID, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_Label_Brightness, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label_Brightness, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label_Brightness, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Slider_Brightness = lv_slider_create(flex_container);
    lv_slider_set_range(ui_Slider_Brightness, 20, 255);
    lv_slider_set_value(ui_Slider_Brightness, 0xB3, LV_ANIM_OFF);
    lv_obj_set_size(ui_Slider_Brightness, 175, 10);
    lv_obj_set_style_pad_all(flex_container, 10, 0);
    // lv_obj_set_style_pad_row(flex_container, 10, 0);

    static lv_style_t style_indicator;
    lv_style_init(&style_indicator);
    lv_style_set_bg_color(&style_indicator, lv_color_hex(0x37B9F5));
    // lv_style_set_bg_grad_color(&style_indicator, lv_color_hex(0x1464F0));
    // lv_style_set_bg_grad_dir(&style_indicator, LV_GRAD_DIR_HOR);
    // lv_style_set_pad_bottom(&style_indicator, 10);

    lv_obj_add_style(ui_Slider_Brightness, &style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(ui_Slider_Brightness, &style_indicator, LV_PART_KNOB);

    lv_obj_set_style_outline_color(ui_Slider_Brightness, lv_color_hex(0x0096FF), LV_PART_KNOB);
    lv_obj_add_event_cb(ui_Slider_Brightness, ui_onBrightnessSliderChanged, LV_EVENT_VALUE_CHANGED, NULL);

    // ACC Label
    ui_Switch_ACC_Label = lv_label_create(flex_container);
    lv_obj_set_width(ui_Switch_ACC_Label, 100); /// 1
    lv_obj_set_height(ui_Switch_ACC_Label, 30); /// 1
    lv_obj_set_x(ui_Switch_ACC_Label, 20);
    lv_obj_set_y(ui_Switch_ACC_Label, 55);
    lv_label_set_text(ui_Switch_ACC_Label, "ACC");
    // lv_obj_set_style_text_align(ui_Switch_ACC_Label, LV_ALIGN_LEFT_MID, LV_PART_MAIN);
    lv_obj_set_style_text_color(ui_Switch_ACC_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Switch_ACC_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Switch_ACC_Label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch_ACC = lv_switch_create(flex_container);
    lv_obj_set_width(ui_Switch_ACC, 60);  /// 1
    lv_obj_set_height(ui_Switch_ACC, 30); /// 1
    lv_obj_set_x(ui_Switch_ACC, 150);
    lv_obj_set_y(ui_Switch_ACC, 50);
    lv_obj_add_state(ui_Switch_ACC, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Switch_ACC, ui_onAccEnabled, LV_EVENT_CLICKED, NULL);

    ui_Switch_EDA_Label = lv_label_create(flex_container);
    lv_obj_set_width(ui_Switch_EDA_Label, 100); /// 1
    lv_obj_set_height(ui_Switch_EDA_Label, 30); /// 1
    lv_obj_set_x(ui_Switch_EDA_Label, 20);
    lv_obj_set_y(ui_Switch_EDA_Label, 95);
    lv_label_set_text(ui_Switch_EDA_Label, "EDA");
    lv_obj_set_style_text_color(ui_Switch_EDA_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Switch_EDA_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Switch_EDA_Label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch_EDA = lv_switch_create(flex_container);
    lv_obj_set_width(ui_Switch_EDA, 60);  /// 1
    lv_obj_set_height(ui_Switch_EDA, 30); /// 1
    lv_obj_set_x(ui_Switch_EDA, 150);
    lv_obj_set_y(ui_Switch_EDA, 90);
    lv_obj_add_state(ui_Switch_EDA, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Switch_EDA, ui_onEdaEnabled, LV_EVENT_CLICKED, NULL);

    ui_Switch_TEMP_Label = lv_label_create(flex_container);
    lv_obj_set_width(ui_Switch_TEMP_Label, 100); /// 1
    lv_obj_set_height(ui_Switch_TEMP_Label, 30); /// 1
    lv_obj_set_x(ui_Switch_TEMP_Label, 20);
    lv_obj_set_y(ui_Switch_TEMP_Label, 135);
    lv_label_set_text(ui_Switch_TEMP_Label, "TEMP");
    lv_obj_set_style_text_color(ui_Switch_TEMP_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Switch_TEMP_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Switch_TEMP_Label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch_TEMP = lv_switch_create(flex_container);
    lv_obj_set_width(ui_Switch_TEMP, 60);  /// 1
    lv_obj_set_height(ui_Switch_TEMP, 30); /// 1
    lv_obj_set_x(ui_Switch_TEMP, 150);
    lv_obj_set_y(ui_Switch_TEMP, 130);
    lv_obj_add_state(ui_Switch_TEMP, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Switch_TEMP, ui_onTempEnabled, LV_EVENT_CLICKED, NULL);

    ui_Switch_HR_Label = lv_label_create(flex_container);
    lv_obj_set_width(ui_Switch_HR_Label, 100); /// 1
    lv_obj_set_height(ui_Switch_HR_Label, 30); /// 1
    lv_obj_set_x(ui_Switch_HR_Label, 20);
    lv_obj_set_y(ui_Switch_HR_Label, 175);
    lv_label_set_text(ui_Switch_HR_Label, "HR");
    lv_obj_set_style_text_color(ui_Switch_HR_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Switch_HR_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Switch_HR_Label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch_HR = lv_switch_create(flex_container);
    lv_obj_set_width(ui_Switch_HR, 60);  /// 1
    lv_obj_set_height(ui_Switch_HR, 30); /// 1
    lv_obj_set_x(ui_Switch_HR, 150);
    lv_obj_set_y(ui_Switch_HR, 170);
    lv_obj_add_state(ui_Switch_HR, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Switch_HR, ui_onHrEnabled, LV_EVENT_CLICKED, NULL);

    ui_Switch_TF_Label = lv_label_create(flex_container);
    lv_obj_set_width(ui_Switch_TF_Label, 100); /// 1
    lv_obj_set_height(ui_Switch_TF_Label, 30); /// 1
    lv_obj_set_x(ui_Switch_TF_Label, 20);
    lv_obj_set_y(ui_Switch_TF_Label, 175);
    lv_label_set_text(ui_Switch_TF_Label, "TensorFlow");
    lv_obj_set_style_text_color(ui_Switch_TF_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Switch_TF_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Switch_TF_Label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch_TF = lv_switch_create(flex_container);
    lv_obj_set_width(ui_Switch_TF, 60);  /// 1
    lv_obj_set_height(ui_Switch_TF, 30); /// 1
    lv_obj_set_x(ui_Switch_TF, 150);
    lv_obj_set_y(ui_Switch_TF, 170);
    lv_obj_add_state(ui_Switch_TF, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Switch_TF, ui_onAlgoEnabled, LV_EVENT_CLICKED, NULL);

    ui_Switch_MSC_Label = lv_label_create(flex_container);
    lv_obj_set_width(ui_Switch_MSC_Label, 100); /// 1
    lv_obj_set_height(ui_Switch_MSC_Label, 30); /// 1
    lv_obj_set_x(ui_Switch_MSC_Label, 20);
    lv_obj_set_y(ui_Switch_MSC_Label, 215);
    lv_label_set_text(ui_Switch_MSC_Label, "MSC");
    lv_obj_set_style_text_color(ui_Switch_MSC_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Switch_MSC_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Switch_MSC_Label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch_MSC = lv_switch_create(flex_container);
    lv_obj_set_width(ui_Switch_MSC, 60);  /// 1
    lv_obj_set_height(ui_Switch_MSC, 30); /// 1
    lv_obj_set_x(ui_Switch_MSC, 150);
    lv_obj_set_y(ui_Switch_MSC, 210);
    lv_obj_add_state(ui_Switch_MSC, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Switch_MSC, ui_onMscEnabled, LV_EVENT_CLICKED, NULL);

    // static lv_style_t style_red;
    // lv_style_init(&style_red);
    // lv_style_set_bg_color(&style_red, lv_color_hex(0xFFFFFF));
    // lv_obj_add_style(ui_Settings, &style_red, LV_PART_SCROLLBAR);
    // lv_obj_add_event_cb()

    ui_Button_Back = lv_img_create(ui_Settings);
    lv_obj_set_width(ui_Button_Back, 60);  /// 1
    lv_obj_set_height(ui_Button_Back, 30); /// 1
    lv_obj_set_x(ui_Button_Back, 120);
    lv_obj_set_y(ui_Button_Back, 265);
    lv_img_set_src(ui_Button_Back, LV_SYMBOL_DOWN);
    lv_obj_add_event_cb(ui_Button_Back, ui_event_Settings, LV_EVENT_CLICKED, NULL);

    lv_obj_add_event_cb(ui_Settings, ui_event_Settings, LV_EVENT_ALL, NULL);
}

void ui_onBleChanged(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    // printf("STATE %d\r\n", lv_obj_get_state(target));
    if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_DEFAULT | LV_STATE_FOCUSED))) {
        printf("Ble Clicked DEFAULT\r\n");
        ble_off();
    } else if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_CHECKED | LV_STATE_FOCUSED))) {
        printf("Ble Clicked CHECKED\r\n");
        ble_on();
    }
}

void ui_onWifiChanged(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    // printf("STATE %d\r\n", lv_obj_get_state(target));
    if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_DEFAULT | LV_STATE_FOCUSED))) {
        printf("Wifi Clicked DEFAULT\r\n");
        wifi_off();
    } else if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_CHECKED | LV_STATE_FOCUSED))) {
        printf("Wifi Clicked CHECKED\r\n");
        wifi_on();
    }
}

void ui_onAccEnabled(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    // printf("STATE %d\r\n", lv_obj_get_state(target));
    if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_DEFAULT | LV_STATE_FOCUSED))) {
        printf("Acc Clicked DEFAULT\r\n");
        ACCE_task_pre_delete();
    } else if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_CHECKED | LV_STATE_FOCUSED))) {
        printf("Acc Clicked CHECKED\r\n");
        ACCE_task_create();
    }
}

void ui_onEdaEnabled(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    // printf("STATE %d\r\n", lv_obj_get_state(target));
    if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_DEFAULT | LV_STATE_FOCUSED))) {
        printf("Eda Clicked DEFAULT\r\n");
        EDA_task_pre_delete();
    } else if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_CHECKED | LV_STATE_FOCUSED))) {
        printf("Eda Clicked CHECKED\r\n");
        EDA_task_create();
    }
}

void ui_onTempEnabled(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    // printf("STATE %d\r\n", lv_obj_get_state(target));
    if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_DEFAULT | LV_STATE_FOCUSED))) {
        printf("Temp Clicked DEFAULT\r\n");
        TEMP_task_pre_delete();
    } else if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_CHECKED | LV_STATE_FOCUSED))) {
        printf("Temp Clicked CHECKED\r\n");
        TEMP_task_create();
    }
}

void ui_onHrEnabled(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    // printf("STATE %d\r\n", lv_obj_get_state(target));
    if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_DEFAULT | LV_STATE_FOCUSED))) {
        printf("Hr Clicked DEFAULT\r\n");
        HR_task_pre_delete();
    } else if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_CHECKED | LV_STATE_FOCUSED))) {
        printf("Hr Clicked CHECKED\r\n");
        HR_task_create();
    }
}
void ui_onMscEnabled(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    // printf("STATE %d\r\n", lv_obj_get_state(target));
    if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_DEFAULT | LV_STATE_FOCUSED))) {
        printf("Msc Clicked DEFAULT\r\n");
        // EDA_task_create();
        // ACCE_task_create();
        // TEMP_task_create();
        // HR_task_create();
        msc2fatfs();
        lv_obj_clear_state(ui_Switch_EDA, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_Switch_ACC, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_Switch_HR, LV_STATE_DISABLED);
        lv_obj_clear_state(ui_Switch_TEMP, LV_STATE_DISABLED);
    } else if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_CHECKED | LV_STATE_FOCUSED))) {
        printf("Msc Clicked CHECKED\r\n");
        EDA_task_pre_delete();
        TEMP_task_pre_delete();
        HR_task_pre_delete();
        ACCE_task_pre_delete();
        Algo_task_pre_delete();
        fatfs2msc();
        lv_obj_clear_state(ui_Switch_EDA, LV_STATE_CHECKED);
        lv_obj_clear_state(ui_Switch_ACC, LV_STATE_CHECKED);
        lv_obj_clear_state(ui_Switch_HR, LV_STATE_CHECKED);
        lv_obj_clear_state(ui_Switch_TEMP, LV_STATE_CHECKED);
        lv_obj_add_state(ui_Switch_EDA, LV_STATE_DISABLED);
        lv_obj_add_state(ui_Switch_ACC, LV_STATE_DISABLED);
        lv_obj_add_state(ui_Switch_HR, LV_STATE_DISABLED);
        lv_obj_add_state(ui_Switch_TEMP, LV_STATE_DISABLED);
    }
}

void ui_onBrightnessSliderChanged(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    printf("ui_onBrightnessSliderChanged: %d\r\n", lv_slider_get_value(target));

    lv_port_disp_set_brightness((uint16_t)lv_slider_get_value(target));
}

void ui_onAlgoEnabled(lv_event_t *e){
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    // printf("STATE %d\r\n", lv_obj_get_state(target));
    if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_DEFAULT | LV_STATE_FOCUSED))) {
        printf("Algo Clicked DEFAULT\r\n");
        Algo_task_pre_delete();
    } else if ((event_code == LV_EVENT_CLICKED) && (lv_obj_get_state(target) == (LV_STATE_CHECKED | LV_STATE_FOCUSED))) {
        printf("Algo Clicked CHECKED\r\n");
        Algo_task_create();
    }
}