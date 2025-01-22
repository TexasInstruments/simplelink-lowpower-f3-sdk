/*
 *  Copyright 2024, Texas Instruments Incorporated
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

#ifdef TFM_BUILD
    /* Enable crypto drivers based on SysConfig */
    #include "ti_drivers_config.h" /* SysConfig generated driver config */

    #ifdef CONFIG_TI_DRIVERS_AESCBC_COUNT
        #define ENABLE_TI_CRYPTO_AESCBC
    #endif

    #ifdef CONFIG_TI_DRIVERS_AESCCM_COUNT
        #define ENABLE_TI_CRYPTO_AESCCM
    #endif

    #ifdef CONFIG_TI_DRIVERS_AESCMAC_COUNT
        /* Two AESCMAC drivers instances are req'd to support both CMAC and CBCMAC */
        #if (CONFIG_TI_DRIVERS_AESCMAC_COUNT < 2)
            #error "At least 2 AES CMAC driver instances are required"
        #endif
        #define ENABLE_TI_CRYPTO_AESCMAC
    #endif

    #ifdef CONFIG_TI_DRIVERS_AESCTR_COUNT
        #define ENABLE_TI_CRYPTO_AESCTR
    #endif

    #ifdef CONFIG_TI_DRIVERS_AESECB_COUNT
        #define ENABLE_TI_CRYPTO_AESECB
    #endif

    #ifdef CONFIG_TI_DRIVERS_AESGCM_COUNT
        #define ENABLE_TI_CRYPTO_AESGCM
    #endif

    #ifdef CONFIG_TI_DRIVERS_ECDH_COUNT
        #define ENABLE_TI_CRYPTO_ECDH
    #endif

    #ifdef CONFIG_TI_DRIVERS_ECDSA_COUNT
        #define ENABLE_TI_CRYPTO_ECDSA
    #endif

    #ifdef CONFIG_TI_DRIVERS_EDDSA_COUNT
        #define ENABLE_TI_CRYPTO_EDDSA
    #endif

    #ifdef CONFIG_TI_DRIVERS_SHA2_COUNT
        #define ENABLE_TI_CRYPTO_SHA2
    #endif

    #ifdef CONFIG_TI_DRIVERS_RNG_COUNT
        #define ENABLE_TI_CRYPTO_TRNG
    #endif

#else
    /* Secure only configuration */

    #define ENABLE_TI_CRYPTO_AESCBC

    #define ENABLE_TI_CRYPTO_AESCCM

    #define ENABLE_TI_CRYPTO_AESCMAC

    #define ENABLE_TI_CRYPTO_AESCTR

    #define ENABLE_TI_CRYPTO_AESECB

    #define ENABLE_TI_CRYPTO_AESGCM

    #define ENABLE_TI_CRYPTO_ECDH

    #define ENABLE_TI_CRYPTO_ECDSA

    // #define ENABLE_TI_CRYPTO_EDDSA

    #define ENABLE_TI_CRYPTO_RNG

    #define ENABLE_TI_CRYPTO_SHA2

    #define ENABLE_TI_CRYPTO_TRNG

#endif /* TFM_BUILD */

#endif /* CRYPTO_CONFIG_H */
