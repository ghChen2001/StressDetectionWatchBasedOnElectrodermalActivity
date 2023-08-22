/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_MYSVC_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_MYSVC_H_

/**
 * @brief My service (MYSVC)
 * @defgroup bt_gatt_mysvc My service (MYSVC)
 * @ingroup bluetooth
 * @{
 *
 * [Experimental] Users should note that the APIs can change
 * as a part of ongoing development.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/types.h>

void mysvc_init(void);

int bt_gatt_mysvc_update(u8_t hr, uint16_t pedo, char *t, u8_t s);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_BAS_H_ */
