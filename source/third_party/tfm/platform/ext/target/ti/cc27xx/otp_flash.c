/*
 * Copyright (c) 2025, Texas Instruments Incorporated. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_plat_defs.h"

/* NOTE: For the security of the protected storage system, the bootloader
 * rollback protection, and the protection of cryptographic material  it is
 * CRITICAL to use a internal (in-die) persistent memory for the implementation
 * of the OTP_NV_COUNTERS flash area (see flash_otp_nv_layout.c).
 */

enum tfm_plat_err_t tfm_plat_otp_init(void)
{
    /* Do nothing. OTP is not currently supported. */
    return TFM_PLAT_ERR_SUCCESS;
}
