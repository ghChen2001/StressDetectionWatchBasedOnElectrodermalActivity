/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLOTTEMP_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLOTTEMP_H_

/**
 * @brief Plot Temp (PLOTTEMP)
 * @defgroup bt_gatt_plottemp Plot Temp (PLOTTEMP)
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

void plottemp_init(void);

int bt_gatt_plottemp_update(float * Data);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLOTTEMP_H_ */
