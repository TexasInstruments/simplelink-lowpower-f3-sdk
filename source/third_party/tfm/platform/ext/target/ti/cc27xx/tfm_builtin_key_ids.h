/*
 * Copyright (c) 2025, Texas Instruments Incorporated. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_BUILTIN_KEY_IDS_H__
#define __TFM_BUILTIN_KEY_IDS_H__

#include <psa/crypto.h>

/**
 * \brief The persistent key identifiers for TF-M builtin keys.
 *
 * \note The value of TFM_BUILTIN_KEY_ID_MIN (and therefore of the whole range) is
 *       completely arbitrary except for being inside the PSA builtin keys range.
 *       The range is specified by the limits defined through MBEDTLS_PSA_KEY_ID_BUILTIN_MIN
 *       and MBEDTLS_PSA_KEY_ID_BUILTIN_MAX
 */
enum tfm_builtin_key_id_t {
    TFM_BUILTIN_KEY_ID_HUK = PSA_KEY_ID_HSM_HUK,
    TFM_BUILTIN_KEY_ID_IAK = PSA_KEY_ID_IAK,
};
#endif /* __TFM_BUILTIN_KEY_IDS_H__ */
