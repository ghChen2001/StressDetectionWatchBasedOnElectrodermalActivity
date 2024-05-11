#ifndef INS5699S_H
#define INS5699S_H

#include "bflb_core.h"
#include "bflb_rtc.h"

// Time
#include <lwip/apps/sntp.h>
#include "bflb_timestamp.h"

#include "time.h"

// INS5699 Basic Time and Calendar Register definitions
#define INS5699_BTC_SEC               0x00
#define INS5699_BTC_MIN               0x01
#define INS5699_BTC_HOUR              0x02
#define INS5699_BTC_WEEK              0x03
#define INS5699_BTC_DAY               0x04
#define INS5699_BTC_MONTH             0x05
#define INS5699_BTC_YEAR              0x06
#define INS5699_BTC_RAM               0x07
#define INS5699_BTC_ALARM_MIN         0x08
#define INS5699_BTC_ALARM_HOUR        0x09
#define INS5699_BTC_ALARM_WEEK_OR_DAY 0x0A
#define INS5699_BTC_TIMER_CNT_0       0x0B
#define INS5699_BTC_TIMER_CNT_1       0x0C
#define INS5699_BTC_EXT               0x0D
#define INS5699_BTC_FLAG              0x0E
#define INS5699_BTC_CTRL              0x0F

// INS5699 Extension Register 1 definitions
#define INS5699_EXT_SEC               0x10
#define INS5699_EXT_MIN               0x11
#define INS5699_EXT_HOUR              0x12
#define INS5699_EXT_WEEK              0x13
#define INS5699_EXT_DAY               0x14
#define INS5699_EXT_MONTH             0x15
#define INS5699_EXT_YEAR              0x16
#define INS5699_EXT_TEMP              0x17
#define INS5699_EXT_BACKUP            0x18

#define INS5699_EXT_TIMER_CNT_0       0x1B
#define INS5699_EXT_TIMER_CNT_1       0x1C
#define INS5699_EXT_EXT               0x1D
#define INS5699_EXT_FLAG              0x1E
#define INS5699_EXT_CTRL              0x1F

// Flag INS5699_BTC_EXT Register bit positions
#define INS5699_BTC_EXT_TSEL0         (1 << 0)
#define INS5699_BTC_EXT_TSEL1         (1 << 1)
#define INS5699_BTC_EXT_FSEL0         (1 << 2)
#define INS5699_BTC_EXT_FSEL1         (1 << 3)
#define INS5699_BTC_EXT_TE            (1 << 4)
#define INS5699_BTC_EXT_USEL          (1 << 5)
#define INS5699_BTC_EXT_WADA          (1 << 6)
#define INS5699_BTC_EXT_TEST          (1 << 7)

// Flag INS5699_BTC_FLAG Register bit positions
#define INS5699_BTC_FLAG_VDET         (1 << 0)
#define INS5699_BTC_FLAG_VLF          (1 << 1)

#define INS5699_BTC_FLAG_AF           (1 << 3)
#define INS5699_BTC_FLAG_TF           (1 << 4)
#define INS5699_BTC_FLAG_UF           (1 << 5)

// Flag INS5699_BTC_CTRL Register bit positions
#define INS5699_BTC_CTRL_RESET        (1 << 0)

#define INS5699_BTC_CTRL_AIE          (1 << 3)
#define INS5699_BTC_CTRL_TIE          (1 << 4)
#define INS5699_BTC_CTRL_UIE          (1 << 5)
#define INS5699_BTC_CTRL_CSEL0        (1 << 6)
#define INS5699_BTC_CTRL_CSEL1        (1 << 7)

#define SE_RTC_ADDR                   0x64

#define CHARGEBAT

typedef struct {
    uint8_t number;
    uint8_t value;
} reg_data;

//----------------------------------------------------------------------
// ins5699_get_time()
// gets the current time from the ins5699 registers
//
//----------------------------------------------------------------------
int ins5699_get_time(struct bflb_tm *dt);

//----------------------------------------------------------------------
// ins5699_set_time()
// Sets the current time in the ins5699 registers
//
//----------------------------------------------------------------------
int ins5699_set_time(struct bflb_tm *dt);

//----------------------------------------------------------------------
// ins5699_init_client()
// initializes the ins5699
//
//----------------------------------------------------------------------
int ins5699_init_client(int *need_reset);

#endif