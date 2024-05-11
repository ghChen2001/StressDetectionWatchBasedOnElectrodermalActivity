/** @file
 *  @brief CTS Service sample
 */

/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <sys/errno.h>

#include "bluetooth.h"
#include "conn.h"
#include "gatt.h"
#include "uuid.h"
#include "cts.h"
#include "bflb_rtc.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "ins5699s.h"

extern SemaphoreHandle_t xMutex_RTC;

static uint8_t ct[10];
static uint8_t ct_update;

static void ct_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    /* TODO: Handle value */
}

static ssize_t read_ct(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                       void *buf, uint16_t len, uint16_t offset)
{
    const char *value = attr->user_data;

    printf("read_ct\r\n");

    return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
                             sizeof(ct));
}

static ssize_t write_ct(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                        const void *buf, uint16_t len, uint16_t offset,
                        uint8_t flags)
{
    uint8_t *value = attr->user_data;
    struct bflb_tm time;

    printf("write_ct\r\n");

    if (offset + len > sizeof(ct)) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }

    memcpy(value + offset, buf, len);
    ct_update = 1U;

    time.tm_year = (value[1] << 8) | value[0];
    printf("time.tm_year %d\r\n", time.tm_year);
    time.tm_mon = value[2];
    printf("time.tm_mon %d\r\n", time.tm_mon);
    time.tm_mday = value[3];
    time.tm_hour = value[4];
    time.tm_min = value[5];
    time.tm_sec = value[6];
    time.tm_wday = value[7];

    xSemaphoreTake(xMutex_RTC, portMAX_DELAY);
    ins5699_set_time(&time);
    ins5699_get_time(&time);
    bflb_rtc_set_utc_time(&time); // set time into on-chip rtc.
    xSemaphoreGive(xMutex_RTC);

    printf("write_ct %d\r\n", value[8]);

    return len;
}

// /* Current Time Service Declaration */
// BT_GATT_SERVICE_DEFINE(cts_cvs,
// 	BT_GATT_PRIMARY_SERVICE(BT_UUID_CTS),
// 	BT_GATT_CHARACTERISTIC(BT_UUID_CTS_CURRENT_TIME, BT_GATT_CHRC_READ |
// 			       BT_GATT_CHRC_NOTIFY | BT_GATT_CHRC_WRITE,
// 			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
// 			       read_ct, write_ct, ct),
// 	BT_GATT_CCC(ct_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
// );

static struct bt_gatt_attr attrs[] = {
    BT_GATT_PRIMARY_SERVICE(BT_UUID_CTS),
    BT_GATT_CHARACTERISTIC(BT_UUID_CTS_CURRENT_TIME, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY | BT_GATT_CHRC_WRITE,
                           BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                           read_ct, write_ct, ct),
    BT_GATT_CCC(ct_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
};

struct bt_gatt_service cts_cvs = BT_GATT_SERVICE(attrs);

static void generate_current_time(uint8_t *buf)
{
    uint16_t year;

    /* 'Exact Time 256' contains 'Day Date Time' which contains
	 * 'Date Time' - characteristic contains fields for:
	 * year, month, day, hours, minutes and seconds.
	 */

    year = 2015;
    memcpy(buf, &year, 2); /* year */
    buf[2] = 5U;           /* months starting from 1 */
    buf[3] = 30U;          /* day */
    buf[4] = 12U;          /* hours */
    buf[5] = 45U;          /* minutes */
    buf[6] = 30U;          /* seconds */

    /* 'Day of Week' part of 'Day Date Time' */
    buf[7] = 1U; /* day of week starting from 1 */

    /* 'Fractions 256 part of 'Exact Time 256' */
    buf[8] = 0U;

    /* Adjust reason */
    buf[9] = 0U; /* No update, change, etc */
}

void cts_init(void)
{
    bt_gatt_service_register(&cts_cvs);
    /* Simulate current time for Current Time Service */
    generate_current_time(ct);
}

void cts_notify(void)
{ /* Current Time Service updates only when time is changed */
    if (!ct_update) {
        return;
    }

    ct_update = 0U;
    bt_gatt_notify(NULL, &cts_cvs.attrs[1], &ct, sizeof(ct));
}