#include "ins5699s.h"
#include "bflb_mtimer.h"
#include <FreeRTOS.h>
#include "semphr.h"
#include "task.h"
#include "bflb_i2c.h"

extern struct bflb_device_s *i2c1;
extern SemaphoreHandle_t xMutex_IIC1;
static uint8_t ins5699 = 0x00;

int ins5699_write_regs(uint8_t start_addr, uint8_t len, const uint8_t *data)
{
    struct bflb_i2c_msg_s msgs[1];
    int ins5699_err = 0;
    uint8_t subaddr[1 + len];

    subaddr[0] = start_addr;
    for (uint8_t i = 0; i < len; i++) {
        subaddr[1 + i] = data[i];
    }

    msgs[0].addr = SE_RTC_ADDR >> 1;
    msgs[0].flags = 0;
    msgs[0].buffer = subaddr;
    msgs[0].length = len + 1;

    if (xSemaphoreTake(xMutex_IIC1, portMAX_DELAY) == pdTRUE) {
        ins5699_err = bflb_i2c_transfer(i2c1, msgs, 1);
        xSemaphoreGive(xMutex_IIC1);
    }

    return ins5699_err;
}

int ins5699_read_regs(uint8_t start_addr, uint8_t len, uint8_t *data)
{
    struct bflb_i2c_msg_s msgs[2];
    int ins5699_err = 0;
    uint8_t sbuf[1] = { start_addr };

    msgs[0].addr = SE_RTC_ADDR >> 1;
    msgs[0].flags = I2C_M_NOSTOP;
    msgs[0].buffer = sbuf;
    msgs[0].length = 1;

    msgs[1].addr = SE_RTC_ADDR >> 1;
    msgs[1].flags = I2C_M_READ;
    msgs[1].buffer = data;
    msgs[1].length = len;

    if (xSemaphoreTake(xMutex_IIC1, portMAX_DELAY) == pdTRUE) {
        ins5699_err = bflb_i2c_transfer(i2c1, msgs, 2);
        xSemaphoreGive(xMutex_IIC1);
    }

    return ins5699_err;
}

int ins5699_write_reg(uint8_t start_addr, uint8_t data)
{
    struct bflb_i2c_msg_s msgs[1];
    int ins5699_err = 0;
    uint8_t subaddr[2] = { start_addr, data };

    msgs[0].addr = SE_RTC_ADDR >> 1;
    msgs[0].flags = 0;
    msgs[0].buffer = subaddr;
    msgs[0].length = 2;

    if (xSemaphoreTake(xMutex_IIC1, portMAX_DELAY) == pdTRUE) {
        ins5699_err = bflb_i2c_transfer(i2c1, msgs, 1);
        xSemaphoreGive(xMutex_IIC1);
    }

    return ins5699_err;
}

int ins5699_read_reg(uint8_t start_addr, uint8_t *data)
{
    struct bflb_i2c_msg_s msgs[2];
    int ins5699_err = 0;
    uint8_t sbuf[1] = { start_addr };

    msgs[0].addr = SE_RTC_ADDR >> 1;
    msgs[0].flags = I2C_M_NOSTOP;
    msgs[0].buffer = sbuf;
    msgs[0].length = 1;

    msgs[1].addr = SE_RTC_ADDR >> 1;
    msgs[1].flags = I2C_M_READ;
    msgs[1].buffer = data;
    msgs[1].length = 1;

    // printf("00\r\n");
    if (xSemaphoreTake(xMutex_IIC1, portMAX_DELAY) == pdTRUE) {
        ins5699_err = bflb_i2c_transfer(i2c1, msgs, 2);
        xSemaphoreGive(xMutex_IIC1);
    }
    // printf("000\r\n");
    return ins5699_err;
}

static void mdelay(uint8_t t)
{
#ifdef CONFIG_FREERTOS
    vTaskDelay(t);
#else
    bflb_mtimer_delay_ms(t);
#endif
}

int ins5699_get_week_day(uint8_t reg_week_day)
{
    int i, tm_wday = -1;

    for (i = 0; i < 7; i++) {
        if (reg_week_day & 1) {
            tm_wday = i;
            break;
        }
        reg_week_day >>= 1;
    }

    return tm_wday;
}

int bcd2int(uint8_t bcd)
{
    return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

uint8_t int2bcd(int i)
{
    return ((i % 10) & 0x0F) + ((((i % 100 - i % 10) / 10) << 4) & 0xF0);
}

//----------------------------------------------------------------------
// ins5699_get_time()
// gets the current time from the ins5699 registers
//
//----------------------------------------------------------------------
int ins5699_get_time(struct bflb_tm *dt)
{
    uint8_t date[7];
    int ins5699_err;

    ins5699_err = ins5699_read_regs(INS5699_BTC_SEC, 7, date);
    if (ins5699_err)
        return ins5699_err;

    printf("%s: read 0x%02x 0x%02x "
           "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
           __func__,
           date[0], date[1], date[2], date[3], date[4], date[5], date[6]);

    dt->tm_sec = bcd2int(date[INS5699_BTC_SEC] & 0x7f);
    dt->tm_min = bcd2int(date[INS5699_BTC_MIN] & 0x7f);
    dt->tm_hour = bcd2int(date[INS5699_BTC_HOUR] & 0x3f);
    dt->tm_wday = ins5699_get_week_day(date[INS5699_BTC_WEEK] & 0x7f);
    dt->tm_mday = bcd2int(date[INS5699_BTC_DAY] & 0x3f);
    dt->tm_mon = bcd2int(date[INS5699_BTC_MONTH] & 0x1f) - 1;
    dt->tm_year = bcd2int(date[INS5699_BTC_YEAR]);

    if (dt->tm_year < 70)
        dt->tm_year += 100;

    printf("%s: date %ds %dm %dh %dwd %dmd %dm %dy\n", __func__,
           dt->tm_sec, dt->tm_min, dt->tm_hour, dt->tm_wday,
           dt->tm_mday, dt->tm_mon, dt->tm_year);

    return 1;
}

//----------------------------------------------------------------------
// ins5699_set_time()
// Sets the current time in the ins5699 registers
//
//----------------------------------------------------------------------
int ins5699_set_time(struct bflb_tm *dt)
{
    uint8_t date[7];
    int ret = 0;

    date[INS5699_BTC_SEC] = int2bcd(dt->tm_sec);
    date[INS5699_BTC_MIN] = int2bcd(dt->tm_min);
    date[INS5699_BTC_HOUR] = int2bcd(dt->tm_hour);
    date[INS5699_BTC_WEEK] = 1 << (dt->tm_wday);
    date[INS5699_BTC_DAY] = int2bcd(dt->tm_mday);
    date[INS5699_BTC_MONTH] = int2bcd(dt->tm_mon + 1);
    date[INS5699_BTC_YEAR] = int2bcd(dt->tm_year % 100);

    printf("%s: write %ds %dm %dh %dwd %dmd %dm %dy\n", __func__,
           dt->tm_sec, dt->tm_min, dt->tm_hour, dt->tm_wday,
           dt->tm_mday, dt->tm_mon, dt->tm_year);

    ret = ins5699_write_regs(INS5699_BTC_SEC, 7, date);

    return ret;
}

//----------------------------------------------------------------------
// ins5699_init()
// initializes the ins5699
//
//----------------------------------------------------------------------
int ins5699_init()
{
    uint8_t init_array[] = {
        0x00,
        0x00,
        0x00,
        0x40,
        0x01,
        0x01,
        0x00,
        0x00, // 0x07 RAM
        0x00, // 0x08 MIN ALARM
        0x00, // 0x09 HR ALARM
        0x00, // 0x0A WEEK/DAY ALARM
        0x00, // 0x0B Counter
        0x00, // 0x0C Counter
        0x02, // 0x0D Extension
        0x00, // 0x0E Flag
        0x40, // 0x0F Control
        0x00, // 0x10
        0x00,
        0x00,
        0x40,
        0x01,
        0x01,
        0x00, // 0x16
    };

    ins5699_write_regs(INS5699_BTC_SEC, 23, init_array);

    ins5699_write_reg(0x18, 0x00);

    ins5699_write_reg(INS5699_EXT_TIMER_CNT_0, 0x00);
    ins5699_write_reg(INS5699_EXT_TIMER_CNT_1, 0x00);

    ins5699_write_reg(INS5699_EXT_EXT, 0x02);
    ins5699_write_reg(INS5699_EXT_FLAG, 0x00);
    ins5699_write_reg(INS5699_EXT_CTRL, 0x40);
}

//----------------------------------------------------------------------
// ins5699_recover()
// initializes the ins5699
//
//----------------------------------------------------------------------
static int ins5699_recover()
{
    int ins5699_err;
    unsigned char value_init = 0;
    unsigned char value_5c = 0;
    unsigned char value_20reg = 0;
    unsigned char value_5c_temp = 0;
    ins5699_err = ins5699_write_reg(0x30, 0xd1);
    if (ins5699_err)
        return ins5699_err;

    ins5699_err = ins5699_write_reg(0x40, 0x00);
    if (ins5699_err)
        return ins5699_err;

    ins5699_err = ins5699_write_reg(0x32, 0x81);
    if (ins5699_err)
        return ins5699_err;

    mdelay(50);

    ins5699_err = ins5699_write_reg(0x32, 0x80);
    if (ins5699_err)
        return ins5699_err;

    mdelay(50);
    ins5699_err = ins5699_write_reg(0x32, 0x04);
    if (ins5699_err)
        return ins5699_err;

    //check rtc vertion
    ins5699_err = ins5699_read_reg(0x20, &value_20reg);
    if (ins5699_err)
        return ins5699_err;
    if (value_20reg == 0XD1) {
        return ins5699_err;
    } else if (value_20reg == 0XD2)
        ;
    else {
        ins5699_err = 1;
        return ins5699_err;
    }

    //write 5c
    ins5699_err = ins5699_read_reg(0x5c, &value_init);
    if (ins5699_err)
        return ins5699_err;
    value_5c = value_init | 0X08;
    ins5699_err = ins5699_write_reg(0x5c, value_5c);
    if (ins5699_err)
        return ins5699_err;
    ins5699_err = ins5699_read_reg(0x5c, &value_5c_temp);
    if (ins5699_err)
        return ins5699_err;
    if (value_5c_temp != value_5c) {
        ins5699_err = 1;
        return ins5699_err;
    }

    mdelay(50);

    value_5c = value_5c & 0XF7;
    ins5699_err = ins5699_write_reg(0x5c, value_5c);
    if (ins5699_err)
        return ins5699_err;
    ins5699_err = ins5699_write_reg(0x5c, value_init);
    if (ins5699_err)
        return ins5699_err;
    ins5699_err = ins5699_read_reg(0x5c, &value_5c_temp);
    if (ins5699_err)
        return ins5699_err;
    if (value_5c_temp != value_init) {
        ins5699_err = 1;
        return ins5699_err;
    }
    ins5699_err = ins5699_write_reg(0x30, 0x00);
    if (ins5699_err)
        return ins5699_err;

    return ins5699_err;
}

//----------------------------------------------------------------------
// ins5699_init_client()
// initializes the ins5699
//
//----------------------------------------------------------------------
int ins5699_init_client(int *need_reset)
{
    uint8_t flags;
    int need_clear = 0;
    int ins5699_err = 0;
    printf("0\r\n");
    ins5699_err = ins5699_read_reg(INS5699_BTC_FLAG, &flags);
    printf("ins5699_err %d\r\n", ins5699_err);

    if (ins5699_err)
        return ins5699_err;
    printf("1\r\n");
    //***turn off 0x32 for current  2022-03-10
    ins5699_err = ins5699_write_reg(0x30, 0xD1);
    if (ins5699_err)
        return ins5699_err;
    printf("2\r\n");
    ins5699_err = ins5699_write_reg(0x32, 0x04);
    if (ins5699_err)
        return ins5699_err;

    ins5699_err = ins5699_write_reg(0x30, 0x00);
    if (ins5699_err)
        return ins5699_err;
    printf("3\r\n");
    //***

    if (flags & INS5699_BTC_FLAG_VDET) {
        printf("Temperature compensation is stop detected.\n");
        need_clear = 1;
    }

    if (flags & INS5699_BTC_FLAG_VLF) {
        printf("Data loss is detected. All registers must be initialized.\n");
        *need_reset = 1;
        need_clear = 1;
    }

    if (flags & INS5699_BTC_FLAG_AF) {
        printf("Alarm was detected\n");
        need_clear = 1;
    }

    if (flags & INS5699_BTC_FLAG_TF) {
        printf("Timer was detected\n");
        need_clear = 1;
    }

    if (flags & INS5699_BTC_FLAG_UF) {
        printf("Update was detected\n");
        need_clear = 1;
    }

    if (*need_reset) {
        //clear ctrl register
        ins5699_err = ins5699_write_reg(INS5699_BTC_CTRL, INS5699_BTC_CTRL_CSEL0);
        if (ins5699_err)
            return ins5699_err;

        //set second update
        ins5699_err = ins5699_write_reg(INS5699_BTC_EXT, 0x00);
        if (ins5699_err)
            return ins5699_err;
    }

    if (need_clear) {
        //clear flag register
        ins5699_err = ins5699_write_reg(INS5699_BTC_FLAG, 0x00);
        if (ins5699_err)
            return ins5699_err;
    }

    if (*need_reset) {
        /* Solution for drop of vdd&vbat voltage.*/
        ins5699_err = ins5699_recover();
        if (ins5699_err)
            return ins5699_err;
    }

#ifdef CHARGEBAT
    ins5699_err = ins5699_write_reg(0x21, 0x81);
    if (ins5699_err)
        return ins5699_err;
#endif

    if (need_clear && *need_reset) {
        ins5699_init();
    }

    return ins5699_err;
}