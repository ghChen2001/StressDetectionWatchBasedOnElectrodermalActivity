#ifndef _TOUCH_CONF_USER_H_
#define _TOUCH_CONF_USER_H_

/* i2c interface
    TOUCH_I2C_FT6X36
    TOUCH_I2C_GT911
    TOUCH_I2C_CST816D
    TOUCH_I2C_TMA525
*/
/* Select Touch Type */
#define TOUCH_I2C_TMA525

/* touch interface */
#define TOUCH_INTERFACE_SPI 1
#define TOUCH_INTERFACE_I2C 2

/* touch interface pin config */
#define TOUCH_I2C_SCL_PIN   GPIO_PIN_0
#define TOUCH_I2C_SDA_PIN   GPIO_PIN_1
#define TOUCH_RESET         GPIO_PIN_30
#ifdef TOUCH_I2C_TMA525
#define TOUCH_INTERRUPT     GPIO_PIN_3
#endif

#endif // _TOUCH_CONF_USER_H_