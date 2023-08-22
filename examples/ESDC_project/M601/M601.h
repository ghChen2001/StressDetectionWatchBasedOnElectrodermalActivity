#include "bflb_mtimer.h"
#include "bflb_gpio.h"

#define M601_OUT            GPIO_PIN_14
#define M601_IN             GPIO_PIN_20

#define M601_Measure_Period 10.5

void M601_DQ_reset(void);
void M601_DQ_set(void);
bool M601_ow_resetpresence(void);
bool M601_read_bit(void);
void M601_write_bit(char);
unsigned char M601_read_byte(void);
void M601_write_byte(char);
void M601_Read_ROMCode(void);
void M601_Single_point_Read_Temperature(void);
bool M601_DQ_read(void);
unsigned char M601_CRC8MY(unsigned char *, unsigned char);