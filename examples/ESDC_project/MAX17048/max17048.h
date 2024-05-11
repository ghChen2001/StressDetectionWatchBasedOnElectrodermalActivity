#ifndef MAX17048_H_
#define MAX17048_H_

// #include "stdio.h"
#include "bl616_glb.h"

#define MAX17048_ADDR 0x6C >> 1

uint16_t max17048_get_version(void);
uint16_t max17048_get_vcell(void);
float max17048_get_SOC(void);
float max17048_get_CRATE(void);
uint8_t max17048_set_int(bool alsc_en, uint8_t alarm_soc);

#endif