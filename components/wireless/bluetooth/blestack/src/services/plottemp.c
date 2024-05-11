/** @file
 *  @brief GATT Plot Eda
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
#include "ploteda.h"
#include <stdlib.h>

static void blvl_ccc_cfg_changed(const struct bt_gatt_attr *attr,
                                 u16_t value)
{
    ARG_UNUSED(attr);

    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

#if !defined(BFLB_BLE)
    LOG_INF("BAS Notifications %s", notif_enabled ? "enabled" : "disabled");
#endif
}

static struct bt_gatt_attr attrs[] = {
    BT_GATT_PRIMARY_SERVICE(BT_UUID_PLOTTEMP),
    BT_GATT_CHARACTERISTIC(BT_UUID_PLOTTEMP_DATA, BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(blvl_ccc_cfg_changed,
                BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
};

struct bt_gatt_service plottemp = BT_GATT_SERVICE(attrs);

void plottemp_init(void)
{
    bt_gatt_service_register(&plottemp);
}

int bt_gatt_plottemp_update(float *Data)
{
    int rc;

    rc = bt_gatt_notify(NULL, &plottemp.attrs[1], Data, 1 * sizeof(float));
    
    return rc == -ENOTCONN ?
               0 :
               rc;
}

#if !defined(BFLB_BLE)
SYS_INIT(bas_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
#endif
