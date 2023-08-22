#include "bflb_gpio.h"

#define AD5940_SCK_PIN                     GPIO_PIN_25

#define AD5940_MISO_PIN                    GPIO_PIN_18

#define AD5940_MOSI_PIN                    GPIO_PIN_19

#define AD5940_CS_PIN                      GPIO_PIN_20

#define AD5940_RST_PIN                     GPIO_PIN_24

#define KEY0                               GPIO_PIN_23

void KEY0_Interrupt(int irq, void *arg);

void AD5940_IntInit(void);

