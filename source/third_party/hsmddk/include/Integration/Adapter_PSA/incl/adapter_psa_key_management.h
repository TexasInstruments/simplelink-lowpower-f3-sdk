/**
 * @file adapter_psa_key_management.h
 *
 * @brief PSA API - Key Management services definitions
 */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*   Module        : DDK-130_bsd                                              */
/*   Version       : 4.1.1                                                    */
/*   Configuration : DDK_EIP130_BSD                                           */
/*                                                                            */
/*   Date          : 2023-Mar-10                                              */
/*                                                                            */
/* Copyright (c) 2014-2023 by Rambus, Inc. and/or its subsidiaries.           */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or without         */
/* modification, are permitted provided that the following conditions are     */
/* met:                                                                       */
/*                                                                            */
/* 1. Redistributions of source code must retain the above copyright          */
/* notice, this list of conditions and the following disclaimer.              */
/*                                                                            */
/* 2. Redistributions in binary form must reproduce the above copyright       */
/* notice, this list of conditions and the following disclaimer in the        */
/* documentation and/or other materials provided with the distribution.       */
/*                                                                            */
/* 3. Neither the name of the copyright holder nor the names of its           */
/* contributors may be used to endorse or promote products derived from       */
/* this software without specific prior written permission.                   */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR      */
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT       */
/* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT           */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,      */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY      */
/* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT        */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE      */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.       */
/* -------------------------------------------------------------------------- */

#ifndef INCLUDE_GUARD_ADAPTER_PSA_KEY_MANAGEMENT_H
#define INCLUDE_GUARD_ADAPTER_PSA_KEY_MANAGEMENT_H

#include <third_party/hsmddk/include/Config/cs_adapter_psa.h>         // PSA_MAX_KEY_BUFF_ENTRIES

#include <third_party/hsmddk/include/Kit/DriverFramework/Basic_Defs_API/incl/basic_defs.h>             // uint8_t, uint16_t, uint32_t, uint64_t
#include <third_party/hsmddk/include/Kit/DriverFramework/CLib_Abstraction_API/incl/clib.h>                   // size_t
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/psa/crypto.h>             // psa_status_t, psa_key_id_t, psa_key_context_t
                                    // psa_ecc_family_t

/*----------------------------------------------------------------------------
 * Definitions and macros
 */

typedef struct
{
    psa_key_attributes_t attributes;
    size_t key_size;
    size_t modulus_size;
    size_t exponentsize;
    uint32_t KeyInUseCounter;
    PsaPolicyMask_t AssetPolicy;
    PsaAssetId_t key_assetId;
    psa_key_id_t source_key_id;
    uint8_t *key;
    uint8_t fAllocated;
} psa_key_context_t;

psa_status_t
psaInt_KeyMgmtGetKey(psa_key_id_t key,
                     psa_key_context_t ** ppKeyEntry);

psa_status_t
psaInt_KeyMgmtLoadKey(psa_key_context_t * pKey,
                      uint32_t * const pAssetId,
                      uint8_t * const pData,
                      size_t const DataSize,
                      uint32_t * const pActualDataSize);

void
psaInt_KeyMgmtReleaseKey(psa_key_context_t * pKey);

psa_status_t
psaInt_KeyMgmtSetKeyInUse(psa_key_id_t key);

psa_status_t
psaInt_KeyMgmtClrKeyInUse(psa_key_id_t key);

psa_status_t
psaInt_KeyMgmtReadECPubKey(const uint8_t * pData,
                           size_t DataLen,
                           psa_ecc_family_t ExpCurveFamily,
                           size_t ExpCurveBits,
                           uint8_t * pCurveFamily,
                           size_t * pCurveBits,
                           uint8_t * pfECDH,
                           uint8_t * pOutputData,
                           size_t * pOutputDataLength);

#endif /* INCLUDE_GUARD_ADAPTER_PSA_KEY_MANAGEMENT_H */

/* end of file adapter_psa_key_management.h */
