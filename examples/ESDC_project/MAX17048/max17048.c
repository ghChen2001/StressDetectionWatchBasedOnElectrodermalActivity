#include "max17048.h"
#include "bflb_mtimer.h"
#include <FreeRTOS.h>
#include "semphr.h"
#include "task.h"
#include "bflb_i2c.h"

extern struct bflb_device_s *i2c1;
extern SemaphoreHandle_t xMutex_IIC1;

#define REG_VCELL   0x02
#define REG_SOC     0x04
#define REG_MODE    0x06
#define REG_VERSION 0x08
#define REG_HIBRT   0x0A
#define REG_CONFIG  0x0C
#define REG_VALRT   0x14
#define REG_CRATE   0x16
#define REG_VRESET  0x18
#define REG_STATUS  0x1A
#define REG_CMD     0xFE

static int max17048_readreg(uint8_t reg, uint8_t *data, uint8_t len)
{
    struct bflb_i2c_msg_s msgs[2];
    int err = 0;
    uint8_t sbuf[1] = { reg };

    msgs[0].addr = MAX17048_ADDR;
    msgs[0].flags = I2C_M_NOSTOP;
    msgs[0].buffer = sbuf;
    msgs[0].length = 1;

    msgs[1].addr = MAX17048_ADDR;
    msgs[1].flags = I2C_M_READ;
    msgs[1].buffer = data;
    msgs[1].length = len;

    xSemaphoreTake(xMutex_IIC1, portMAX_DELAY);
    err = bflb_i2c_transfer(i2c1, msgs, 2);
    xSemaphoreGive(xMutex_IIC1);

    return err;
}

static int max17048_writereg(uint8_t reg, uint8_t data1, uint8_t data2)
{
    struct bflb_i2c_msg_s msgs[1];
    int err = 0;
    uint8_t subaddr[3] = { reg, data1, data2 };

    msgs[0].addr = MAX17048_ADDR;
    msgs[0].flags = 0;
    msgs[0].buffer = subaddr;
    msgs[0].length = 3;

    xSemaphoreTake(xMutex_IIC1, portMAX_DELAY);
    err = bflb_i2c_transfer(i2c1, msgs, 1);
    xSemaphoreGive(xMutex_IIC1);

    return err;
}

uint16_t max17048_get_version(void)
{
    uint8_t version[2] = { 0 };
    max17048_readreg(REG_VERSION, version, 2);

    return (version[0] << 8) | version[1];
}

uint16_t max17048_get_vcell(void)
{
    uint8_t vcell[2] = { 0 };
    max17048_readreg(REG_VCELL, vcell, 2);

    return (vcell[0] << 8) | vcell[1];
}

float max17048_get_SOC(void)
{
    uint8_t soc[2] = { 0 };
    max17048_readreg(REG_SOC, soc, 2);

    return (float)((soc[0] << 8) | soc[1]) / 256.0;
}

float max17048_get_CRATE(void)
{
    uint8_t crate[2] = { 0 };
    max17048_readreg(REG_CRATE, crate, 2);

    return (float)((crate[0] << 8) | crate[1]) * 0.208;
}

uint8_t max17048_set_int(bool alsc_en, uint8_t alarm_soc)
{
    uint8_t config = 0x1C; // default.
    if (alsc_en) {
        config |= 0x40;
    }

    if ((alarm_soc > 0) && (alarm_soc <= 32)) {
        config &= 0xE0;
        config |= (uint8_t)(32 - alarm_soc);
    }

    max17048_writereg(REG_CONFIG, 0x97, config);

    return 0;
}