/**
 * @file cst816d_i2c.h
 * @brief
 *
 */
#ifndef _CST816D_I2C_H
#define _CST816D_I2C_H

#include "bflb_core.h"
#include "touch.h"

#define CST816D_I2C_SLAVE_ADDR       0x2A >> 1

/* Maximum border values of the touchscreen pad that the chip can handle */
#define CST816D_MAX_WIDTH            ((uint16_t)320)
#define CST816D_MAX_HEIGHT           ((uint16_t)386)

/* Max detectable simultaneous touch points */
#define CST816D_I2C_MAX_POINT        1

/* Register of the current mode */
#define CST816D_DEV_MODE_REG         0xFA
#define CST816D_MODE_POINT           0x60
#define CST816D_MODE_GESTURE         0x11
#define CST816D_MODE_POINT_GESTURE   0x71

#define CST816D_DEV_LOWPOWER_REG     0xFE
#define CST816D_DEV_LOWPOWER_EN      0x00
#define CST816D_DEV_LOWPOWER_DIS     0xFF

/* Register of the touch information */
#define CST816D_GEST_ID_REG          0x01
#define CST816D_FINGER_NUM           0x02
#define CST816D_XposH                0x03
#define CST816D_XposL                0x04
#define CST816D_YposH                0x05
#define CST816D_YposL                0x06

/* Possible values returned by CST816D_GEST_ID_REG */
#define CST816D_GEST_ID_NO_GESTURE   0x00
#define CST816D_GEST_ID_MOVE_UP      0x01
#define CST816D_GEST_ID_MOVE_RIGHT   0x04
#define CST816D_GEST_ID_MOVE_DOWN    0x02
#define CST816D_GEST_ID_MOVE_LEFT    0x03
#define CST816D_GEST_ID_SINGLE_CLICK 0x05
#define CST816D_GEST_ID_DOUBLE_CLICK 0x0B
#define CST816D_GEST_ID_LONG_PUSH    0x0C

/* Register of the chip ID */
#define CST816D_DEV_ID_REG           0xA7
#define CST816D_DEV_ID               0xB6
#define CST816D_DEV_FWVER_REG        0xA9

/* Register of the */

int cst816d_i2c_init(touch_coord_t *max_value);
int cst816d_i2c_get_gesture_id();
int cst816d_i2c_read(uint8_t *point_num, touch_coord_t *touch_coord, uint8_t max_num);

#endif /* __CST816D_H */
