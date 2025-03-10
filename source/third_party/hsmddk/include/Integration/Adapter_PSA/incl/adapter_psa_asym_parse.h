/**
 * @file adapter_psa_asym_parse.h
 *
 * @brief PSA API - Asymetric Crypto parse definitions
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

#ifndef INCLUDE_GUARD_ADAPTER_PSA_ASYM_PARSE_H
#define INCLUDE_GUARD_ADAPTER_PSA_ASYM_PARSE_H

#include <third_party/hsmddk/include/Config/cs_adapter_psa.h>         // MBEDTLS_MAX_KEY_BUFF_ENTRIES

#include <third_party/hsmddk/include/Kit/DriverFramework/Basic_Defs_API/incl/basic_defs.h>             // uint8_t, uint16_t, uint32_t, uint64_t
#include <third_party/hsmddk/include/Kit/DriverFramework/CLib_Abstraction_API/incl/clib.h>                   // size_t
#include <third_party/hsmddk/include/Integration/Asn1Der/incl/asn1der.h>                // ASN1 / DER defines and functions
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/psa/crypto.h>             // psa_status_t

/*----------------------------------------------------------------------------
 * Definitions and macros
 */

psa_status_t
PSAInt_AsymParse_EC_SEC1(const uint8_t * pData,
                         size_t DataLen,
                         Asn1Der_BigNumber * pPrvKey,
                         uint8_t * pCurveFamily,
                         size_t * pCurveBits,
                         Asn1Der_BigNumber * pPubKey);

psa_status_t
PSAInt_AsymParse_EC_EdX(const uint8_t * pData,
                        size_t DataLen,
                        Asn1Der_BigNumber * pPrvKey,
                        uint8_t * pCurveFamily,
                        size_t * pCurveBits,
                        Asn1Der_BigNumber * pPubKey);

psa_status_t
PSAInt_AsymParse_EC_PubKey(const uint8_t * pData,
                           size_t DataLen,
                           uint8_t * pCurveFamily,
                           size_t * pCurveBits,
                           Asn1Der_BigNumber * pPubKey,
                           uint8_t * pfECDH);

psa_status_t
PSAInt_AsymParse_RSA_PKCS1(const uint8_t * pData,
                           size_t DataLen,
                           Asn1Der_BigNumber * pModulus,
                           Asn1Der_BigNumber * pPrivateExponent,
                           Asn1Der_BigNumber * pPublicExponent);

psa_status_t
PSAInt_AsymParse_RSA_PSS(const uint8_t * pData,
                         size_t DataLen,
                         Asn1Der_BigNumber * pModulus,
                         Asn1Der_BigNumber * pPrivateExponent,
                         Asn1Der_BigNumber * pPublicExponent);

psa_status_t
PSAInt_AsymParse_RSA_PubKey(const uint8_t * pData,
                            size_t DataLen,
                            Asn1Der_BigNumber * pModulus,
                            Asn1Der_BigNumber * pExponent,
                            uint8_t * pfPSS);

#endif /* INCLUDE_GUARD_ADAPTER_PSA_ASYM_PARSE_H */

/* end of file adapter_psa_asym_parse.h */
