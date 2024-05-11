/** @file
 *  @brief GATT Plot Bvp
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
#include "plotbvp.h"
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
    BT_GATT_PRIMARY_SERVICE(BT_UUID_PLOTBVP),
    BT_GATT_CHARACTERISTIC(BT_UUID_PLOTBVP_DATA, BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(blvl_ccc_cfg_changed,
                BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
};

struct bt_gatt_service plotbvp = BT_GATT_SERVICE(attrs);

void plotbvp_init(void)
{
    bt_gatt_service_register(&plotbvp);
}

int bt_gatt_plotbvp_update(int32_t *bvpData)
{
    int rc;

    // int32_t data = bvpData;

    rc = bt_gatt_notify(NULL, &plotbvp.attrs[1], bvpData, 20 * sizeof(int32_t));
    
    return rc == -ENOTCONN ?
               0 :
               rc;
}

#if !defined(BFLB_BLE)
SYS_INIT(bas_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
#endif
