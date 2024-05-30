/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 *  @file       ECDSALPF3HSM.h
 *
 *  @brief      ECDSA HSM IP driver implementation for the Low Power F3 family
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
 *  | NISTP2384        |
 *  | NISTP521         |
 *  | BRP256           |
 *  | BRP384           |
 *  | BRP512           |
 */

#ifndef ti_drivers_ecdsa_ECDSALPF3HSM__include
#define ti_drivers_ecdsa_ECDSALPF3HSM__include

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/ECDSA.h>

#include <third_party/ecc/include/lowlevelapi.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Defines */
/* The largest signature will be for NIST521 which will produce 72B per component (S and R) */
#define ECDSA_COMPONENT_VECTOR_LENGTH 144

/*!
 *  @brief      ECDSALPF3HSM Hardware Attributes
 *
 *  ECDSALPF3HSM hardware attributes should be included in the board file
 *  and pointed to by the ECDSA_config struct.
 */
typedef struct
{
    uint8_t dummy;
} ECDSALPF3HSM_HWAttrs;

/*!
 *  @brief  Enum for the curve domain id supported by the driver.
 */
typedef enum
{
    ECDSA_DOMAIN_ID_SEC = 0,
    ECDSA_DOMAIN_ID_BRP = 1,
} ECDSA_DomainID;

/*!
 *  @brief      ECDSALPF3HSM Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    CryptoKey *key;
    bool isOpen;
    ECDSA_ReturnBehavior returnBehavior; /* Callback mode is not supported */
    int_fast16_t returnStatus;
    ECDSA_CurveType curveType;
    ECDSA_DomainID domainId;
    ECDSA_CurveLength curveLength;
    ECDSA_DigestLength digestLength;
    ECDSA_OperationType operationType;
    ECDSA_Operation *operation;
    ECDSA_CallbackFxn callbackFxn;
    int_fast16_t hsmStatus;
    uint32_t keyAssetID;
    uint32_t paramAssetID;
    const uint8_t *curveParam;
    uint32_t curveParamSize;
    uint8_t *input;
    uint8_t signature[ECDSA_COMPONENT_VECTOR_LENGTH];
} ECDSALPF3HSM_Object;

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_ecdsa_ECDSALPF3HSM__include */
