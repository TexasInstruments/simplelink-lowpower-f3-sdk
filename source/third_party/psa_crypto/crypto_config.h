/*
 *  Copyright 2024-2025, Texas Instruments Incorporated
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef CRYPTO_CONFIG_H
#define CRYPTO_CONFIG_H

#include <ti/devices/DeviceFamily.h>

#ifndef TFM_BUILD
    /* Secure only configuration */

    #define ENABLE_TI_CRYPTO_AESCBC

    #define ENABLE_TI_CRYPTO_AESCCM

    #define ENABLE_TI_CRYPTO_AESCMAC

    #define ENABLE_TI_CRYPTO_AESCTR

    #define ENABLE_TI_CRYPTO_AESECB

    #define ENABLE_TI_CRYPTO_AESGCM

    #define ENABLE_TI_CRYPTO_ECDH

    #define ENABLE_TI_CRYPTO_ECDSA

    /* CC35xx does not currently support EDDSA driver */
    #if (DeviceFamily_PARENT != DeviceFamily_PARENT_CC35XX)
        #define ENABLE_TI_CRYPTO_EDDSA
    #endif

    /* Devices with HSM use RNG driver for random number operations */
    #if ((DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) || (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX))
        #define ENABLE_TI_CRYPTO_RNG
    #else
        #define ENABLE_TI_CRYPTO_TRNG
    #endif

    #define ENABLE_TI_CRYPTO_SHA2

#endif /* TFM_BUILD */

#endif /* CRYPTO_CONFIG_H */
