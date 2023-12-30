/**
 * @file chsc5816_i2c.c
 * @brief
 *
 */

#include "touch.h"

#ifdef TOUCH_I2C_CHSC5816

#include "bflb_mtimer.h"
#include "bflb_gpio.h"
#include "bflb_i2c.h"
#include "chsc5816_i2c.h"

static struct bflb_device_s *touch_chsc5816_i2c = NULL;
static struct bflb_device_s *chsc5816_i2c_gpio = NULL;
int count = 0;

static void chsc5816_i2c_gpio_init(void)
{
    chsc5816_i2c_gpio = bflb_device_get_by_name("gpio");

    /* I2C0_SCL */
    bflb_gpio_init(chsc5816_i2c_gpio, TOUCH_I2C_SCL_PIN, GPIO_FUNC_I2C1 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    /* I2C0_SDA */
    bflb_gpio_init(chsc5816_i2c_gpio, TOUCH_I2C_SDA_PIN, GPIO_FUNC_I2C1 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
}

static int chsc5816_i2c_peripheral_init(void)
{
    touch_chsc5816_i2c = bflb_device_get_by_name("i2c1");

    if (touch_chsc5816_i2c) {
        /* init i2c gpio */
        chsc5816_i2c_gpio_init();
        /* init i2c 400k */
        bflb_i2c_init(touch_chsc5816_i2c, 100000);
    } else {
        printf("i2c device get fail\r\n");
        return -1;
    }

    return 0;
}

static void chsc5816_msdelay(uint32_t millisecs)
{
    bflb_mtimer_delay_ms(millisecs);
}

static int chsc5816_i2c_read_byte(uint32_t register_addr, uint8_t *data_buf, uint16_t len)
{
    static struct bflb_i2c_msg_s msg[2];

    uint8_t toSend[] = {
        (register_addr >> 24) & 0x8,
        (register_addr >> 16) & 0x8,
        (register_addr >> 8) & 0x8,
        (register_addr >> 0) & 0x8,
    };

    msg[0].addr = CHSC5816_I2C_SLAVE_ADDR;
    msg[0].flags = NULL;
    msg[0].buffer = toSend;
    msg[0].length = 4;

    msg[1].flags = I2C_M_READ;
    msg[1].buffer = data_buf;
    msg[1].length = len;

    return bflb_i2c_transfer(touch_chsc5816_i2c, msg, 2);
}

static int chsc5816_i2c_write_byte(uint32_t register_addr, uint8_t *data_buf, uint16_t len)
{
    static struct bflb_i2c_msg_s msg[2];

    uint8_t toSend[] = {
        (register_addr >> 24) & 0x8,
        (register_addr >> 16) & 0x8,
        (register_addr >> 8) & 0x8,
        (register_addr >> 0) & 0x8,
    };

    msg[0].addr = CHSC5816_I2C_SLAVE_ADDR;
    msg[0].flags = NULL;
    msg[0].buffer = toSend;
    msg[0].length = 4;

    msg[1].addr = CHSC5816_I2C_SLAVE_ADDR;
    msg[1].flags = NULL;
    msg[1].buffer = data_buf;
    msg[1].length = len;

    return bflb_i2c_transfer(touch_chsc5816_i2c, msg, 2);
}

static void chsc5816_reset(void)
{
    bflb_gpio_reset(chsc5816_i2c_gpio, TOUCH_RESET); //GPIO_PIN_SET
    chsc5816_msdelay(5);
    bflb_gpio_set(chsc5816_i2c_gpio, TOUCH_RESET); //GPIO_PIN_RESET
    chsc5816_msdelay(3);
}

int chsc5816_i2c_init(touch_coord_t *max_value)
{
    uint8_t data_buf;
    uint8_t u32Data[4] = {0};
    printf("chsc5816_i2c_init ENTER\n");
    chsc5816_i2c_peripheral_init();

    for (uint8_t retry = 0; retry < 3; retry++) {
        chsc5816_reset();

        if (!chsc5816_i2c_read_byte(0x20000004, u32Data, 4)) {
            return -1;
        } else {
            printf("is chsc5816! X-res %d, Y-res %d\n", u32Data[0], u32Data[1]);
        }
    }
    return 0;
}

int chsc5816_i2c_read(uint8_t *point_num, touch_coord_t *touch_coord, uint8_t max_num)
{
    return 0;
}

#endif