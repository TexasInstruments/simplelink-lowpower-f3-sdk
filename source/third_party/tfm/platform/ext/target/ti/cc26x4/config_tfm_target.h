/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CONFIG_TFM_TARGET_H__
#define __CONFIG_TFM_TARGET_H__

/* Conditional to avoid config_crypto_check.h error: "Invalid config: NOT CRYPTO_NV_SEED AND NOT CRYPTO_HW_ACCELERATOR!" */
#if (TEST_S != ON)  
/* Use stored NV seed to provide entropy */
#define CRYPTO_NV_SEED                         0
#endif

/* The maximum asset size to be stored in the Protected Storage area. */
#define PS_MAX_ASSET_SIZE                      512

#endif /* __CONFIG_TFM_TARGET_H__ */
