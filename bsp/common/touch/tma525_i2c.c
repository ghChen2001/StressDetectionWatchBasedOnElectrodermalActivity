/**
 * @file tma525_i2c.c
 * @brief
 *
 */

#include "touch.h"

#ifdef TOUCH_I2C_TMA525
#include "bflb_mtimer.h"
#include "bflb_gpio.h"
#include "bflb_i2c.h"
#include "tma525_i2c.h"

static struct bflb_device_s *touch_tma525_i2c = NULL;
struct bflb_device_s *tma525_i2c_gpio = NULL;
// int count = 0;

static void tma525_i2c_gpio_init(void)
{
    tma525_i2c_gpio = bflb_device_get_by_name("gpio");

    /* I2C0_SCL */
    bflb_gpio_init(tma525_i2c_gpio, TOUCH_I2C_SCL_PIN, GPIO_FUNC_I2C1 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    /* I2C0_SDA */
    bflb_gpio_init(tma525_i2c_gpio, TOUCH_I2C_SDA_PIN, GPIO_FUNC_I2C1 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_2);
    /* TOUCH_RESET */
    bflb_gpio_init(tma525_i2c_gpio, TOUCH_RESET, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    /* TOUCH_INTERRUPT */
    bflb_gpio_init(tma525_i2c_gpio, TOUCH_INTERRUPT, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
}

static int tma525_i2c_peripheral_init(void)
{
    touch_tma525_i2c = bflb_device_get_by_name("i2c1");

    if (touch_tma525_i2c) {
        // printf("ft6x36 i2c gpio init\r\n");
        /* init i2c gpio */
        tma525_i2c_gpio_init();
        /* init i2c 200k */
        bflb_i2c_init(touch_tma525_i2c, 400000);
    } else {
        printf("i2c device get fail\r\n");
        return -1;
    }

    return 0;
}

static int tma525_i2c_read_byte(uint16_t register_addr, uint8_t *data_buf, uint16_t len)
{
    static struct bflb_i2c_msg_s msg[2];

    msg[0].addr = TMA525_SLAVE_ADDR;
    msg[0].flags = I2C_M_NOSTOP;
    msg[0].buffer = &register_addr;
    msg[0].length = 2;

    msg[1].addr = TMA525_SLAVE_ADDR;
    msg[1].flags = I2C_M_READ;
    msg[1].buffer = data_buf;
    msg[1].length = len;
    bflb_i2c_transfer(touch_tma525_i2c, msg, 2);

    return 0;
}

static int tma525_i2c_write_byte(uint16_t register_addr, uint8_t *data_buf, uint16_t len)
{
    static struct bflb_i2c_msg_s msg[2];

    msg[0].addr = TMA525_SLAVE_ADDR;
    msg[0].flags = I2C_M_NOSTOP;
    msg[0].buffer = &register_addr;
    msg[0].length = 2;

    msg[1].addr = TMA525_SLAVE_ADDR;
    msg[1].flags = 0;
    msg[1].buffer = data_buf;
    msg[1].length = len;

    bflb_i2c_transfer(touch_tma525_i2c, msg, 2);
    return 0;
}

// int tma525_get_gesture_id()
// {
//     uint8_t data_buf = CST816D_GEST_ID_NO_GESTURE;

//     if (tma525_i2c_read_byte(CST816D_GEST_ID_REG, &data_buf, 1)) {
//         return -1;
//     }

//     return data_buf;
// }

int tma525_i2c_init(touch_coord_t *max_value)
{
    uint16_t chip_id = 0;
    printf("tma525 i2c init\r\n");

    tma525_i2c_peripheral_init();

    bflb_gpio_reset(tma525_i2c_gpio, TOUCH_RESET);
    bflb_mtimer_delay_ms(10);
    bflb_gpio_set(tma525_i2c_gpio, TOUCH_RESET);
    bflb_mtimer_delay_ms(50);

    if (tma525_i2c_read_byte(CHIP_ID_REG, (uint8_t *)&chip_id, 2)) {
        return -1;
    }
    printf("Chip ID: 0x%04x\r\n", chip_id);

    return 0;
}

int tma525_i2c_read(uint8_t *point_num, touch_coord_t *touch_coord, uint8_t max_num)
{
    // // return 0;
    // count++;
    // uint8_t data_buf[5];
    // *point_num = 0;

    // if (point_num == NULL || touch_coord == NULL || max_num == 0) {
    //     printf("LVGL Para Error\r\n");
    //     return -1;
    // }

    // if (tma525_i2c_read_byte(CST816D_FINGER_NUM, &data_buf[0], 1)) {
    //     printf("Read Error\r\n");
    //     return -1;
    // }

    // /* no touch or err */
    // if (data_buf[0] == 0x00 || data_buf[0] >= 0x02) {
    //     if (data_buf[0] >= 0x02) {
    //         // Error
    //         printf("Value Error %d %d\r\n", data_buf[0], count);
    //         return -2;
    //     } else {
    //         // No touch
    //         // printf("No Touch\r\n");
    //         return 0;
    //     }
    // }

    // /* Get the first point */
    // tma525_i2c_read_byte(CST816D_XposH, &data_buf[1], 4);
    // touch_coord[0].coord_x = ((uint16_t)(data_buf[1] & 0x0F) << 8) | (uint16_t)data_buf[2];
    // touch_coord[0].coord_y = ((uint16_t)(data_buf[3] & 0x0F) << 8) | (uint16_t)data_buf[4];
    // *point_num = 1;
    // // printf("x: %d, y: %d\r\n", touch_coord[0].coord_x, touch_coord[0].coord_y);

    return 0;
}

#endif
