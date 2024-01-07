#include <unistd.h>
#include <stdlib.h>

void onenet_close();
void onenet_transfer(float temp, uint32_t hr, int ML_output, uint32_t stepCnt);
void onenet_transfer_GSR(float *gsr, uint16_t len);
void weather_get(char *location, char *city_name, char *weather_text, uint8_t *weather_code, char *weather_temp);