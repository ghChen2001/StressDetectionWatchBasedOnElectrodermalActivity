#ifndef __TMP117_DRV_H_
#define __TMP117_DRV_H_

#include <stdint.h>

#define TMP117_DeviceID1             0x48 //	GND
#define TMP117_DeviceID2             0x49 //	Vcc
#define TMP117_DeviceID3             0x4A //	SDA
#define TMP117_DeviceID4             0x4B //	SCL

/*
Registers
*/
#define TMP117_TemperatureRegister   0x00
#define TMP117_ConfigurationRegister 0x01
#define TMP117_TemperatureHighLimit  0x02
#define TMP117_TemperatureLowLimit   0x03

#define TMP117_EEPROM_Uclock         0x04
#define TMP117_EEPROM1               0x05
#define TMP117_EEPROM2               0x06
#define TMP117_EEPROM3               0x08

#define TMP117_Temperature_Offset    0x07
#define TMP117_ID_Register           0x0F

int8_t tmp117_get_chipid(uint16_t *chipid);
int8_t tmp117_get_tempurature(float *tempurature);
int8_t tmp117_get_configuration(uint16_t *config);

int8_t tmp117_set_configuration(uint8_t bytehigh, uint8_t bytelow);
int8_t tmp117_set_highlimit(uint8_t bytehigh, uint8_t bytelow);
int8_t tmp117_set_lowlimit(uint8_t bytehigh, uint8_t bytelow);
int8_t tmp117_set_tempoffset(uint8_t bytehigh, uint8_t bytelow);

void tmp117_StartConv(void);
uint8_t tmp117_DataReady(void);
void tmp117_Initialization_DEFAULT(void);
void tmp117_Initialization(void);

int8_t tmp117_i2c_write(uint8_t *txbuf, uint16_t len);
int8_t tmp117_i2c_read(uint8_t addr, uint8_t *rxbuf, uint16_t len);

#endif
