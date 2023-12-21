/**
 * @file tma525_i2c.h
 * @brief
 *
 */
#ifndef _TMA525_I2C_H
#define _TMA525_I2C_H

#include "bflb_core.h"
#include "touch.h"

#define TMA525_SLAVE_ADDR            0x24 >> 1
#define TMA525_SWRESET_DELAY_US      100
#define TMA525_DISP_RESX  320
#define TMA525_DISP_RESY  360

#define TMA525_I2C_MAX_POINT        1

#define __DRV_ZTW523_H__

#define TOUCH_POINT_MODE                      0
#define MAX_SUPPORTED_FINGER_NUM              1
#define TPD_RES_139_46_MAX_X                  453
#define TPD_RES_139_46_MAX_Y                  453
#define TPD_RES_119_42_MAX_X                  389
#define TPD_RES_119_42_MAX_Y                  389
#define I2C_BUFFER_SIZE                       2
#define TC_SECTOR_SZ                          8
#define PAGE_SIZE                             64

#define TP_POWERON_DELAY                      10
#define CHIP_ON_DELAY                         10
#define CHIP_OFF_DELAY                        70
#define FIRMWARE_ON_DELAY                     50
#define ENABLE                                1
#define DISABLE                               0

#define CHIP_ID_REG                           0xCC00
#define CHIP_ID_VALUE                         0xE628
// #define TMA525_SLAVE_ADDR                     0x24

// different technique in 1.39 and 1.19
// 1.39:Lens GFF
// 1.19:DEO oncell

#define HW_ID_BOE_LENS_139_46                 0x0103
#define HW_ID_EDO_LENS_139_46                 0x0203
#define HW_ID_EDO_EDO_119_42                  0x2

// chip code
#define ZTW523_CHIP_CODE                      0xE628

//! @name Register Map
//! @{
#define ZINITIX_SWRESET_CMD                   0x0000
#define ZINITIX_WAKEUP_CMD                    0x0001
#define ZINITIX_IDLE_CMD                      0x0004
#define ZINITIX_SLEEP_CMD                     0x0005
#define ZINITIX_CLEAR_INT_STATUS_CMD          0x0003
#define ZINITIX_CALIBRATE_CMD                 0x0006
#define ZINITIX_SAVE_STATUS_CMD               0x0007
#define ZINITIX_SAVE_CALIBRATION_CMD          0x0008
#define ZINITIX_RECALL_FACTORY_CMD            0x000f
#define ZINITIX_VENDOR_CMD                    0xC000
#define ZINITIX_INTN_CLEAR_CMD                0xC004
#define ZINITIX_NVM_INIT_CMD                  0xC002
#define ZINITIX_PROGRAM_START_CMD             0xC001
#define ZINITIX_NVM_VPP                       0xC003
#define ZINITIX_NVM_WP                        0xC104
#define ZINITIX_POSTURE_REG                   0x0126

#define ZINITIX_INIT_RETRY_CNT                3
#define TOUCH_CHECK_SHORT_MODE                14
#define TOUCH_SEC_MODE                        48
#define TOUCH_REF_MODE                        10
#define TOUCH_NORMAL_MODE                     5
#define TOUCH_DELTA_MODE                      3
#define TOUCH_DND_MODE                        6
#define TOUCH_PDND_MODE                       11
#define NORMAL_SHORT_VALUE                    1000

#define ZINITIX_SENSITIVITY                   0x0020
#define ZINITIX_I2C_CHECKSUM_WCNT             0x016a
#define ZINITIX_I2C_CHECKSUM_RESULT           0x016c
#define ZINITIX_DEBUG_REG                     0x0115
#define ZINITIX_TOUCH_MODE                    0x0010
#define ZINITIX_CHIP_REVISION                 0x0011
#define ZINITIX_FIRMWARE_VERSION              0x0012
#define ZINITIX_MINOR_FW_VERSION              0x0121
#define ZINITIX_DATA_VERSION_REG              0x0013
#define ZINITIX_HW_ID                         0x0014
#define ZINITIX_SUPPORTED_FINGER_NUM          0x0015
#define ZINITIX_EEPROM_INFO                   0x0018
#define ZINITIX_INITIAL_TOUCH_MODE            0x0019
#define ZINITIX_TOTAL_NUMBER_OF_X             0x0060
#define ZINITIX_TOTAL_NUMBER_OF_Y             0x0061
#define ZINITIX_DELAY_RAW_FOR_HOST            0x007f
#define ZINITIX_BUTTON_SUPPORTED_NUM          0x00B0
#define ZINITIX_BUTTON_SENSITIVITY            0x00B2
#define ZINITIX_X_RESOLUTION                  0x00C0
#define ZINITIX_Y_RESOLUTION                  0x00C1
#define ZINITIX_POINT_STATUS_REG              0x0080
#define ZINITIX_ICON_STATUS_REG               0x00AA
#define ZINITIX_AFE_FREQUENCY                 0x0100
#define ZINITIX_DND_N_COUNT                   0x0122
#define ZINITIX_DND_U_COUNT                   0x0135
#define ZINITIX_RAWDATA_REG                   0x0200
#define ZINITIX_EEPROM_INFO_REG               0x0018
#define ZINITIX_INT_ENABLE_FLAG               0x00f0
#define ZINITIX_PERIODICAL_INTERRUPT_INTERVAL 0x00f1
#define ZINITIX_CHECKSUM_RESULT               0x012c
#define ZINITIX_INIT_FLASH                    0x01d0
#define ZINITIX_WRITE_FLASH                   0x01d1
#define ZINITIX_READ_FLASH                    0x01d2
#define ZINITIX_VENDOR_REG                    0xC000
#define ZINITIX_NVM_REG                       0xC002
#define ZINITIX_VENDOR_ID                     0x001C
#define ZINITIX_VENDOR_ID_VALUE               0x5A49

#define BIT_PT_CNT_CHANGE                     0
#define BIT_DOWN                              1
#define BIT_MOVE                              2
#define BIT_UP                                3
#define BIT_PALM                              4
#define BIT_PALM_REJECT                       5
#define BIT_WAKEUP                            6
#define RESERVED_1                            7
#define BIT_WEIGHT_CHANGE                     8
#define BIT_PT_NO_CHANGE                      9
#define BIT_REJECT                            10
#define BIT_PT_EXIST                          11
#define RESERVED_2                            12
#define BIT_MUST_ZERO                         13
#define BIT_DEBUG                             14
#define BIT_ICON_EVENT                        15

#define SUB_BIT_EXIST                         0
#define SUB_BIT_DOWN                          1
#define SUB_BIT_MOVE                          2
#define SUB_BIT_UP                            3
#define SUB_BIT_UPDATE                        4
#define SUB_BIT_WAIT                          5

//! @}

int tma525_init(touch_coord_t *max_value);
int tma525_get_gesture_id();
int tma525_read(uint8_t *point_num, touch_coord_t *touch_coord, uint8_t max_num);

#endif /* __CST816D_H */
