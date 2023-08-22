#include "tmp117_drv.h"
#include "bflb_mtimer.h"

uint8_t rxbuf[2];
uint8_t txbuf[3];

int8_t tmp117_get_chipid(uint16_t *chipid)
{
    // uint8_t rxbuf[2] = { 0 };
    int8_t ret_code;
    ret_code = tmp117_i2c_read(TMP117_ID_Register, rxbuf, 2);
    *chipid = (rxbuf[0] << 8) | rxbuf[1];
    return ret_code;
}

int8_t tmp117_get_tempurature(float *tempurature)
{
    // uint8_t rxbuf[2] = { 0 };
    int8_t ret_code;
    ret_code = tmp117_i2c_read(TMP117_TemperatureRegister, rxbuf, 2);
    *tempurature = ((rxbuf[0] << 8) | rxbuf[1]) * 0.0078125;
    return ret_code;
}

int8_t tmp117_get_configuration(uint16_t *config)
{
    // uint8_t rxbuf[2] = { 0 };
    int8_t ret_code;
    ret_code = tmp117_i2c_read(TMP117_ConfigurationRegister, rxbuf, 2);
    *config = (rxbuf[0] << 8) | rxbuf[1];
    return ret_code;
}

int8_t tmp117_set_configuration(uint8_t bytehigh, uint8_t bytelow)
{
    txbuf[0] = TMP117_ConfigurationRegister;
    txbuf[1] = bytehigh;
    txbuf[2] = bytelow;
    return tmp117_i2c_write(txbuf, 3);
}

int8_t tmp117_set_highlimit(uint8_t bytehigh, uint8_t bytelow)
{
    txbuf[0] = TMP117_TemperatureHighLimit;
    txbuf[1] = bytehigh;
    txbuf[2] = bytelow;
    return tmp117_i2c_write(txbuf, 3);
}

int8_t tmp117_set_lowlimit(uint8_t bytehigh, uint8_t bytelow)
{
    txbuf[0] = TMP117_TemperatureLowLimit;
    txbuf[1] = bytehigh;
    txbuf[2] = bytelow;
    return tmp117_i2c_write(txbuf, 3);
}

int8_t tmp117_set_tempoffset(uint8_t bytehigh, uint8_t bytelow)
{
    txbuf[0] = TMP117_Temperature_Offset;
    txbuf[1] = bytehigh;
    txbuf[2] = bytelow;
    return tmp117_i2c_write(txbuf, 3);
}

void tmp117_Initialization(void)
{
    // Soft Reset
    // 0b00001100(0x0C) high 单稳态模式
    // 0b00100110(0x26) low   avg=8   转换周期为125ms 2ms软复位
    tmp117_set_configuration(0x0C, 0x26);
    bflb_mtimer_delay_ms(3);
    printf("Tmp117 Reset\r\n");
    // 初始化并开始一次转换
    // 0b00001100(0x0C) high 单稳态模式
    // 0b00100100(0x24) low   avg=8   转换周期为125ms
    tmp117_set_configuration(0x0C, 0x24);
}

void tmp117_StartConv(void)
{
    // 开始一次转换
    // 0b00001100(0x0C) high 单稳态模式
    // 0b00100100(0x24) low   avg=8   转换周期为125ms
    tmp117_set_configuration(0x0C, 0x24);
}

uint8_t tmp117_DataReady(void)
{
    // data ready bit 0b0010 0000 0000 0000
    uint16_t config;
    tmp117_get_configuration(&config);

    if (config & 0x2000)
        return 1;
    else
        return 0;
}

void tmp117_Initialization_DEFAULT(void)
{
    tmp117_set_configuration(0x02, 0x20);
    tmp117_set_tempoffset(0x00, 0x00);
    tmp117_set_lowlimit(0x80, 0x00);
    tmp117_set_highlimit(0x60, 0x00);
}