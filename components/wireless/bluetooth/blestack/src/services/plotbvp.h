/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLOTBVP_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLOTBVP_H_

/**
 * @brief Plot Bvp (PLOTBVP)
 * @defgroup bt_gatt_plotbvp Plot Bvp (PLOTBVP)
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

void plotbvp_init(void);

int bt_gatt_plotbvp_update(int32_t *bvpData); // long: int32_t

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLOTBVP_H_ */
