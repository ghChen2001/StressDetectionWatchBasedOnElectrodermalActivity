#ifndef _M117_DRV_H
#define _M117_DRV_H

#include <stdint.h>
#include <stddef.h>

#define M117_I2C_ADDR 0x44

/* 16 bit Command */
#define M117_WRITE_CONFIG   0x5206
#define M117_READ_STATUS    0xF32D
#define M117_READ_TEMP      0x0000
#define M117_CLEAR_STATUS   0x3041
#define M117_SOFT_RST       0x30A2
#define M117_START_COV      0xCC44
#define M117_BREAK_COV      0x3093

void m117_convert(void);
void m117_config(void);
void m117_i2c_read(uint16_t, uint8_t *, uint8_t);
void m117_i2c_write(uint16_t, uint8_t *, uint8_t);
uint8_t m117_read_value(uint16_t *);
void m117_soft_reset(void);

#endif