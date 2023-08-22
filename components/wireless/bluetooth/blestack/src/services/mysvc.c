/** @file
 *  @brief GATT My Service
 */

/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sys/errno.h>
#include <stdbool.h>
#include <zephyr/types.h>

#include "bluetooth.h"
#include "conn.h"
#include "gatt.h"
#include "uuid.h"
#include "mysvc.h"

static u8_t heartRate = 0;
static u8_t pedoCnt = 0;
static u8_t tempurature = 0;
static u8_t stressLabel = 0;

static void blvl_ccc_cfg_changed(const struct bt_gatt_attr *attr,
                                 u16_t value)
{
    ARG_UNUSED(attr);

    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

#if !defined(BFLB_BLE)
    LOG_INF("BAS Notifications %s", notif_enabled ? "enabled" : "disabled");
#endif
}

// static ssize_t read_blvl(struct bt_conn *conn,
//                          const struct bt_gatt_attr *attr, void *buf,
//                          u16_t len, u16_t offset)
// {
//     u8_t lvl8 = battery_level;

//     return bt_gatt_attr_read(conn, attr, buf, len, offset, &lvl8,
//                              sizeof(lvl8));
// }

static struct bt_gatt_attr attrs[] = {
    BT_GATT_PRIMARY_SERVICE(BT_UUID_MYSVC),
    BT_GATT_CHARACTERISTIC(BT_UUID_MYSVC_DATA, BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(blvl_ccc_cfg_changed,
                BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
};

struct bt_gatt_service mysvc = BT_GATT_SERVICE(attrs);

void mysvc_init(void)
{
    bt_gatt_service_register(&mysvc);
}

int bt_gatt_mysvc_update(u8_t hr, uint16_t pedo, char* t, u8_t s)
{
    int rc;

    static uint8_t data[7];

    data[0] = hr;
    data[1] = (pedo >> 8) & 0xFF;
    data[2] = pedo & 0xFF;
    data[3] = s;
    data[4] = t[0];
    data[5] = t[1];
    data[6] = t[3];

    rc = bt_gatt_notify(NULL, &mysvc.attrs[1], &data, sizeof(data));

    return rc == -ENOTCONN ? 0 : rc;
}

#if !defined(BFLB_BLE)
SYS_INIT(bas_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
#endif
