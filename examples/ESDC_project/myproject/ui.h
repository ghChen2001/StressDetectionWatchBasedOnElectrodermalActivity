// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.2.0
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_Settings
void ui_Settings_screen_init(void);
void ui_event_Settings(lv_event_t *e);
extern lv_obj_t *ui_Settings;
extern lv_obj_t *ui_SettingsLabel;
extern lv_obj_t *ui_Switch_HR;
extern lv_obj_t *ui_Switch_HR_Label;
extern lv_obj_t *ui_Switch_TF;
extern lv_obj_t *ui_Switch_TF_Label;
extern lv_obj_t *ui_Switch_EDA;
extern lv_obj_t *ui_Switch_EDA_Label;
extern lv_obj_t *ui_Switch_TEMP;
extern lv_obj_t *ui_Switch_TEMP_Label;
extern lv_obj_t *ui_Switch_ACC;
extern lv_obj_t *ui_Switch_ACC_Label;
extern lv_obj_t *ui_Switch_LOG;
extern lv_obj_t *ui_Switch_MSC_Label;
extern lv_obj_t *ui_Switch_MSC;
extern lv_obj_t *ui_Slider_Brightness;
extern lv_obj_t *ui_Button_Back;
extern lv_obj_t *ui_Label_Brightness;
extern lv_obj_t *ui_Switch_BLE_Label;
extern lv_obj_t *ui_Switch_BLE;
extern lv_obj_t *ui_Switch_WIFI_Label;
extern lv_obj_t *ui_Switch_WIFI;

// SCREEN: ui_Home
void ui_Home_screen_init(void);
void ui_event_Home(lv_event_t *e);
extern lv_obj_t *ui_Home;
extern lv_obj_t *ui_Hour;
extern lv_obj_t *ui_Minute;
extern lv_obj_t *ui_Weekday;
extern lv_obj_t *ui_Date;
extern lv_obj_t *ui_Mon;
extern lv_obj_t *ui_Second;
extern lv_obj_t *ui_Label2;
extern lv_obj_t *ui_AD5940;
extern lv_obj_t *ui_AD5940_WarmUp;
extern lv_obj_t *ui_psb;
extern lv_obj_t *ui_ArcHr;
extern lv_obj_t *ui_ArcStep;
extern lv_obj_t *ui_ArcMental;
extern lv_obj_t *ui_LabelMental;
extern lv_obj_t *ui_LabelHR;
extern lv_obj_t *ui_LabelTemp;
extern lv_obj_t *ui_LabelNumTemp;
extern lv_obj_t *ui_LabeHR;
extern lv_obj_t *ui_ImageTouchLock;
extern lv_obj_t *ui_ImageWifi;
extern lv_obj_t *ui_ImageWifiOff;
extern lv_obj_t *ui_ImageBle;
extern lv_obj_t *ui_ImageBleOff;
extern lv_obj_t *ui_ImageFootStep;
extern lv_obj_t *ui_ImageHR;
extern lv_obj_t *ui_ImageMental;
extern lv_obj_t *ui_WeatherImage;
extern lv_obj_t *ui_WeatherLabel1;
extern lv_obj_t *ui_WeatherLabel2;
extern lv_obj_t *ui_stepCntLabel;
extern lv_obj_t *ui_ImageStress;
extern lv_obj_t *ui_ImageNeutral;
extern lv_obj_t *ui_ImageLocation;
extern lv_obj_t *ui_LabelCity;
extern lv_obj_t *ui_LabelBat;

// SCREEN: ui_Sensor
void ui_Sensor_screen_init(void);
void ui_event_Sensor(lv_event_t *e);
extern lv_obj_t *ui_Sensor;
extern lv_obj_t *ui_LabelSensor;
extern lv_obj_t *ui_SensorTempurature;
extern lv_obj_t *ui_ChartTempurature;
extern lv_obj_t *ui_SensorAcceleration_;
extern lv_obj_t *ui_ChartAcceleration;
extern lv_obj_t *ui_LabelAcceX;
extern lv_obj_t *ui_LabelAcceY;
extern lv_obj_t *ui_LabelAcceZ;
// SCREEN: ui_Debug
void ui_Debug_screen_init(void);
void ui_event_Debug(lv_event_t *e);
extern lv_obj_t *ui_Debug;
extern lv_obj_t *ui_LabelDebug;
extern lv_obj_t *ui_SensorEDA;
extern lv_obj_t *ui_ChartEDA;
extern lv_obj_t *ui_LabelEda1;
extern lv_obj_t *ui_LabelEda2;
extern lv_obj_t *ui_LabelEda3;
extern lv_obj_t *ui_LabelBvp;
extern lv_obj_t *ui_LabelTemp;
extern lv_obj_t *ui_LabelMdl1;
extern lv_obj_t *ui_LabelMdl2;

// SCREEN: ui_HearteRate
void ui_HearteRate_screen_init(void);
void ui_event_HearteRate(lv_event_t *e);
extern lv_obj_t *ui_HearteRate;
extern lv_obj_t *ui_LabelHearteRate;
extern lv_obj_t *ui_ChartPPG;
extern lv_obj_t *ui____initial_actions0;

// LV_IMG_DECLARE(ui_img_189584499);    // assets\bg2(1).png
LV_IMG_DECLARE(ui_img_wifi_png);         // assets\wifi.png
LV_IMG_DECLARE(ui_img_1560645021);       // assets\wifi-off.png
LV_IMG_DECLARE(ui_img_bluetoothon_png);  // assets\bluetoothon.png
LV_IMG_DECLARE(ui_img_footsteps_png);    // assets\footsteps.png
LV_IMG_DECLARE(ui_img_pulse_png);        // assets\pulse.png
LV_IMG_DECLARE(ui_img_1916941551);       // assets\chart-bubble.png
LV_IMG_DECLARE(ui_img_872619278);        // assets\emoticon-neutral.png
LV_IMG_DECLARE(ui_img_208425557);        // assets\emoticon-sad.png
LV_IMG_DECLARE(ui_img_bluetoothoff_png); // assets\bluetoothoff.png
LV_IMG_DECLARE(ui_img_location_png);     // assets\location.png

LV_IMG_DECLARE(ui_img_352157552);  // assets\black\0@1x.png
LV_IMG_DECLARE(ui_img_700619099);  // assets\black\10@1x.png
LV_IMG_DECLARE(ui_img_700722490);  // assets\black\11@1x.png
LV_IMG_DECLARE(ui_img_700690717);  // assets\black\12@1x.png
LV_IMG_DECLARE(ui_img_700794364);  // assets\black\13@1x.png
LV_IMG_DECLARE(ui_img_700475607);  // assets\black\14@1x.png
LV_IMG_DECLARE(ui_img_700578998);  // assets\black\15@1x.png
LV_IMG_DECLARE(ui_img_700547225);  // assets\black\16@1x.png
LV_IMG_DECLARE(ui_img_700650872);  // assets\black\17@1x.png
LV_IMG_DECLARE(ui_img_700435506);  // assets\black\19@1x.png
LV_IMG_DECLARE(ui_img_904422901);  // assets\black\1@1x.png
LV_IMG_DECLARE(ui_img_1587055926); // assets\black\20@1x.png
LV_IMG_DECLARE(ui_img_1587159317); // assets\black\21@1x.png
LV_IMG_DECLARE(ui_img_1587127544); // assets\black\22@1x.png
LV_IMG_DECLARE(ui_img_1587231191); // assets\black\23@1x.png
LV_IMG_DECLARE(ui_img_1586912434); // assets\black\24@1x.png
LV_IMG_DECLARE(ui_img_1587015825); // assets\black\25@1x.png
LV_IMG_DECLARE(ui_img_1586984052); // assets\black\26@1x.png
LV_IMG_DECLARE(ui_img_1586768942); // assets\black\28@1x.png
LV_IMG_DECLARE(ui_img_1659974289); // assets\black\30@1x.png
LV_IMG_DECLARE(ui_img_1660077680); // assets\black\31@1x.png
LV_IMG_DECLARE(ui_img_1660045907); // assets\black\32@1x.png
LV_IMG_DECLARE(ui_img_1659830797); // assets\black\34@1x.png
LV_IMG_DECLARE(ui_img_1659902415); // assets\black\36@1x.png
LV_IMG_DECLARE(ui_img_1660006062); // assets\black\37@1x.png
LV_IMG_DECLARE(ui_img_1114156892); // assets\black\4@1x.png
LV_IMG_DECLARE(ui_img_1666422241); // assets\black\5@1x.png
LV_IMG_DECLARE(ui_img_1395092270); // assets\black\6@1x.png
LV_IMG_DECLARE(ui_img_1711384410); // assets\black\99@1x.png
LV_IMG_DECLARE(ui_img_619575779);  // assets\black\9@1x.png

LV_FONT_DECLARE(ui_font_Chinese);
LV_FONT_DECLARE(ui_font_DateFont);
// LV_FONT_DECLARE(ui_font_H1);
LV_FONT_DECLARE(ui_font_Number_big);
// LV_FONT_DECLARE(ui_font_Number_extra);
LV_FONT_DECLARE(ui_font_SecondFont);
// LV_FONT_DECLARE(ui_font_Subtitle);
LV_FONT_DECLARE(ui_font_TempFont);
// LV_FONT_DECLARE(ui_font_TempNumberFont);
LV_FONT_DECLARE(ui_font_Title);
LV_FONT_DECLARE(ui_font_YearFont);
LV_FONT_DECLARE(ui_font_lock);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
