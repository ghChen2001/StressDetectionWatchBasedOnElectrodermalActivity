/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLOTACC_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLOTACC_H_

/**
 * @brief Plot Eda (PLOTEDA)
 * @defgroup bt_gatt_ploteda Plot Eda (PLOTEDA)
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

void plotacc_init(void);

int bt_gatt_plotacc_update(float *Data);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLOTACC_H_ */