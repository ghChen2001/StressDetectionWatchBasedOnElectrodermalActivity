// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.2.0
// Project name: SquareLine_Project

#ifndef _UI_EVENTS_H
#define _UI_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

void ui_UpdateTempuratureChart(int16_t temp);
void ui_UpdateAccelerationChart(int16_t ax, int16_t ay, int16_t az);
void ui_UpdateHRLabel(int16_t hr);
void ui_UpdateTempLabel(float temp);
void ui_UpdateSecLabel(uint8_t sec);
void ui_UpdateMinLabel(uint8_t min);
void ui_UpdateHourLabel(uint8_t hour);
void ui_UpdateMonLabel(uint8_t mon);
void ui_UpdateWeekLabel(uint8_t week);
void ui_UpdateYearLabel(uint16_t year);
void ui_UpdateMentalLabel(int classNum, float posb);
void ui_setWifiImg(uint8_t isConnected);
void ui_setBleImg(uint8_t isConnected);
void ui_UpdateEDAChart(float *EDAwindow, uint8_t len);
void ui_UpdateEDAChartRange(uint16_t max, uint16_t min);
void ui_UpdateEda1(float min, float avg, float std);
void ui_UpdateEda2(int16_t num, float avg);
void ui_UpdateEda3(float min, float max, float std);
void ui_UpdateBvp(float min, float max, float avg, uint16_t hr);
void ui_UpdateTemp(float temp);
void ui_UpdateMdl1(float out1, float out2, float out3);
void ui_UpdateMdl2(float out1, float out2);
void ui_updateWeather(char *cityName, char *weatherText, uint8_t weatherCode, char *weatherTemp);
void ui_UpdateStepLabel(int16_t step);
void ui_UpdatePPGChart(signed long green, signed long red, signed long ired);

#endif