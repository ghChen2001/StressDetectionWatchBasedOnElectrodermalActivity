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
    BT_GATT_PRIMARY_SERVICE(BT_UUID_PLOTEDA),
    BT_GATT_CHARACTERISTIC(BT_UUID_PLOTEDA_DATA, BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(blvl_ccc_cfg_changed,
                BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
};

struct bt_gatt_service ploteda = BT_GATT_SERVICE(attrs);

void ploteda_init(void)
{
    bt_gatt_service_register(&ploteda);
}

int bt_gatt_ploteda_update(float *edaData, uint16_t len)
{
    int rc;

    float *data;

    data = (float *)malloc((len + 1) * sizeof(float));

    for (uint16_t i = 0; i < len + 1; i++) {
        data[i] = 0x00000000;
    }

    *((uint16_t *)data) = len;
    printf("%04x\n", *((uint16_t *)data));
    // ((uint16_t *)data)[1] = 0x0000;

    for (uint16_t i = 1; i < len + 1; i++) {
        data[i] = edaData[i - 1];
    }

    rc = bt_gatt_notify(NULL, &ploteda.attrs[1], data, (len + 1) * sizeof(float));

    return rc == -ENOTCONN ?
               0 :
               rc;
}

#if !defined(BFLB_BLE)
SYS_INIT(bas_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
#endif
