/*
 * Copyright (c) 2025, Texas Instruments Incorporated. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Provisioning is handled by the TI Crypto Partition so stub functions are
 * implemented here for use by the TF-M core.
 */

#include "config_tfm.h"
#include "tfm_plat_provisioning.h"

#ifdef TFM_DUMMY_PROVISIONING
#error "Dummy provisioning is not supported on this platform"
#endif

int tfm_plat_provisioning_is_required(void)
{
    /* Provisioning is handled by the TI Crypto SP on this platform. Always
     * return 0 so that the SPM core will skip provisioning.
     */
    return 0;
}

enum tfm_plat_err_t tfm_plat_provisioning_perform(void)
{
    /* Nothing to do since provisioning is handled by the TI Crypto SP on this
     * platform.
     */
   return TFM_PLAT_ERR_SUCCESS;
}

void tfm_plat_provisioning_check_for_dummy_keys(void)
{
    /* Dummy provisioning is not supported on this platform */
}
