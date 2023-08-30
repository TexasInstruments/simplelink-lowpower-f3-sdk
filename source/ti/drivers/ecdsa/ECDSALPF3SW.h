/*
 * Copyright (c) 2021-2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       ECDSALPF3SW.h
 *
 *  @brief      ECDSA software driver implementation for the Low Power F3 family
 *
 *  This file should only be included in the board file to fill the ECDSA_config
 *  struct.
 *
 *  # Hardware and Implementation Details #
 *
 *  The driver is backed by a software ECC library implementation.
 *
 *  # Supported Curves #
 *
 *  The driver implementation supports the following curves for ECDSA:
 *
 *  | Curves Supported |
 *  |------------------|
 *  | NISTP256         |
 *  | NISTP224         |
 */

#ifndef ti_drivers_ecdsa_ECDSALPF3SW__include
#define ti_drivers_ecdsa_ECDSALPF3SW__include

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/ECDSA.h>

#include <third_party/ecc/include/lowlevelapi.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief      ECDSALPF3SW Hardware Attributes
 *
 *  ECDSALPF3SW hardware attributes should be included in the board file
 *  and pointed to by the ECDSA_config struct.
 */
typedef struct
{
    uint8_t dummy;
} ECDSALPF3SW_HWAttrs;

/*
 * Performance of the ECC SW library is scaled via the window size parameter
 * which defines the number of bits of the scalar that will be processed together
 * during the execution of the scalar multiplication. A larger window size
 * will have higher performance at the cost of increased memory consumption.
 * A window size of 3 was selected for the best trade-off of performance and
 * memory consumption. WorkZone size was empirically measured.
 *
 * ---------------------------------
 * |             |    NIST P256    |
 * | Window Size |  WorkZone Size  |
 * |             |     (words)     |
 * |-------------|-----------------|
 * |      2      |       227       |
 * |      3      |       254       |
 * |      4      |       308       |
 * |      5      |       416       |
 * ---------------------------------
 */
#ifndef ECDSALPF3SW_ECC_WINDOW_SIZE
    #define ECDSALPF3SW_ECC_WINDOW_SIZE 3
#endif

/*
 * ECC Workzone size is based on worst case empirical measurement of ECDH
 * operations on supported curves with the selected window size.
 */
#if (ECDSALPF3SW_ECC_WINDOW_SIZE == 2)
    #define ECDSALPF3SW_ECC_WORKZONE_WORDS 227
#elif (ECDSALPF3SW_ECC_WINDOW_SIZE == 3)
    #define ECDSALPF3SW_ECC_WORKZONE_WORDS 254
#elif (ECDSALPF3SW_ECC_WINDOW_SIZE == 4)
    #define ECDSALPF3SW_ECC_WORKZONE_WORDS 308
#elif (ECDSALPF3SW_ECC_WINDOW_SIZE == 5)
    #define ECDSALPF3SW_ECC_WORKZONE_WORDS 416
#else
    #error "Invalid ECDSALPF3SW_ECC_WINDOW_SIZE"
#endif

/*!
 *  @brief      ECDSALPF3SW Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    bool isOpen;
    ECDSA_ReturnBehavior returnBehavior; /* Callback mode is not supported */
    ECC_State eccState;
    uint32_t eccWorkZone[ECDSALPF3SW_ECC_WORKZONE_WORDS];
} ECDSALPF3SW_Object;

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_ecdsa_ECDSALPF3SW__include */
