/**
 * @file chsc5816_i2c.c
 * @brief
 *
 */

#ifndef _CHSC5816_I2C_H
#define _CHSC5816_I2C_H

#include "bflb_core.h"
#include "touch.h"

#define CHSC5816_I2C_SLAVE_ADDR 0x5C >> 1
#define CHSC5816_I2C_MAX_POINT 2

#define MAX_IO_BUFFER_LEN   32
#define PID_VID_MASK        (0xffffff00)
#define NVM_W               0x0
#define NVM_R               0x3
#define CORE_R              0x4
#define CORE_W              0x5

#define TP_CMD_BUFF_ADDR    0x20000000
#define TP_RSP_BUFF_ADDR    0x20000000
#define TP_WR_BUFF_ADDR     0x20002000
#define TP_RD_BUFF_ADDR     0x20002400
#define TP_HOLD_MCU_ADDR    0x40007000
#define TP_AUTO_FEED_ADDR   0x40007010
#define TP_REMAP_MCU_ADDR   0x40007000
#define TP_RELEASE_MCU_ADDR 0x40007000
#define TP_HOLD_MCU_VAL     0x12044000
#define TP_AUTO_FEED_VAL    0x0000925a
#define TP_REMAP_MCU_VAL    0x12044002
#define TP_RELEASE_MCU_VAL  0x12044003
#define VID_PID_BACKUP_ADDR (40 * 1024 + 0x10)

enum SEMI_DRV_ERR {
    SEMI_DRV_ERR_OK = 0,
    SEMI_DRV_ERR_HAL_IO,
    SEMI_DRV_ERR_NO_INIT,
    SEMI_DRV_ERR_TIMEOUT,
    SEMI_DRV_ERR_CHECKSUM,
    SEMI_DRV_ERR_RESPONSE,
    SEMI_DRV_INVALID_CMD,
    SEMI_DRV_INVALID_PARAM,
    SEMI_DRV_ERR_NOT_MATCH,
};

/*ctp work staus*/
#define CTP_POINTING_WORK    0x00000000
#define CTP_READY_UPGRADE    (1 << 1)
#define CTP_UPGRAD_RUNING    (1 << 2)
#define CTP_SLFTEST_RUNING   (1 << 3)
#define CTP_SUSPEND_GATE     (1 << 16)
#define CTP_GUESTURE_GATE    (1 << 17)
#define CTP_PROXIMITY_GATE   (1 << 18)
#define CTP_GLOVE_GATE       (1 << 19)
#define CTP_ORIENTATION_GATE (1 << 20)

enum CMD_TYPE_ID {
    CMD_NA = 0x0f,
    CMD_IDENTITY = 0x01,
    CMD_CTP_SSCAN = 0x02,
    CMD_CTP_IOCTL = 0x03,
    CMD_MEM_WR = 0x30,
    CMD_MEM_RD = 0x31,
    CMD_FLASH_ERASE = 0x32,
    CMD_FW_SUM = 0x33,
    CMD_WRITE_REGISTER = 0X35,
    CMD_READ_REGISTER = 0X36,
    CMD_BSPR_WRITE = 0x37,
    CMD_BSPR_READ = 0x38,
};

//cammand struct for mcap
struct m_ctp_cmd_std_t {
    unsigned short chk; // 16 bit checksum
    unsigned short d0;  //data 0
    unsigned short d1;  //data 1
    unsigned short d2;  //data 2
    unsigned short d3;  //data 3
    unsigned short d4;  //data 4
    unsigned short d5;  //data 5

    unsigned char id;  //offset 15
    unsigned char tag; //offset 16
};

//response struct for mcap
struct m_ctp_rsp_std_t {
    unsigned short chk; // 16 bit checksum
    unsigned short d0;  //data 0
    unsigned short d1;  //data 1
    unsigned short d2;  //data 2
    unsigned short d3;  //data 3
    unsigned short d4;  //data 4
    unsigned short d5;  //data 5

    unsigned char cc; //offset 15
    unsigned char id; //offset 16
};

typedef struct sm_touch_dev {
    unsigned int ctp_status;
    unsigned int vid_pid; //0xVID_PID_CFGVER
    unsigned int raw_adr;
    unsigned short fw_ver;
    unsigned short setup_ok;
    unsigned char needUpd;
    unsigned char *updPdata;
    unsigned short newBootLen;
} sm_touch_dev, *psm_touch_dev;

typedef struct _img_header_t {
    uint16_t fw_ver;
    uint16_t resv;
    uint32_t sig;
    uint32_t vid_pid;
    uint16_t raw_offet;
    uint16_t dif_offet;
} img_header_t;

struct chsc_updfile_header {
    uint32_t sig;
    uint32_t resv;
    uint32_t n_cfg;
    uint32_t n_match;
    uint32_t len_cfg;
    uint32_t len_boot;
};

union rpt_point_t {
    struct
    {
        unsigned char x_l8;
        unsigned char y_l8;
        unsigned char z;
        unsigned char x_h4  : 4;
        unsigned char y_h4  : 4;
        unsigned char id    : 4;
        unsigned char event : 4;
    } rp;
    unsigned char data[5];
};

int chsc5816_i2c_init(touch_coord_t *max_value);
int chsc5816_i2c_read(uint8_t *point_num, touch_coord_t *touch_coord, uint8_t max_num);

#endif