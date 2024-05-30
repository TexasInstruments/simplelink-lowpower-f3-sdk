/* adapter_psa_key_management.c
 *
 * Implementation of the PSA API.
 *
 * This file implements the key management services.
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

#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/c_adapter_psa.h>              // configuration
#include <third_party/hsmddk/include/Kit/DriverFramework/Basic_Defs_API/incl/basic_defs.h>
#include <third_party/hsmddk/include/Kit/Log/incl/log.h>
#include <third_party/hsmddk/include/Kit/DriverFramework/CLib_Abstraction_API/incl/clib.h>                       // size_t
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/psa/crypto.h>                 // the API to implement
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/psa/internal_trusted_storage.h> // PSA storage API reference
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/psa_key_store.h>              // psaInt_KeyStore_*
#include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/adapter_vex.h>                // VexTokenCmd_*_t, VexTokenRslt_*_t
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_exchangetoken.h>
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_system.h>
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_asset.h>
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_key_management.h>
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_asym_parse.h>
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_asym_write.h>
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_internal.h>
#include <third_party/hsmddk/include/Kit/EIP130/DomainHelper/incl/eip130_domain_ecc_curves.h>   // EIP130DOMAIN_ECC_FAMILY_*

#ifdef PSA_ENABLE_DEBUG_FUNCTIONS
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_debug.h>
#include <inttypes.h>
#endif


/*----------------------------------------------------------------------------
 * Definitions and macros
 */
/** Maximum length of SHA1/SHA224/SHA256 HMAC keys */
#define PSA_SYM_ALGO_MAX_HMAC_SHA2_KEY_SIZE     (512U / 8U)

/** Maximum length of SHA384/SHA512 HMAC keys */
#define PSA_SYM_ALGO_MAX_HMAC_SHA512_KEY_SIZE   (1024U / 8U)

/** Key administration
 *  Note: Entry 0 is not used therefore + 1U, to keep the number
 *        of entries to PSA_MAX_KEY_BUFF_ENTRIES */
static psa_key_context_t gl_PSA_Key[PSA_MAX_KEY_BUFF_ENTRIES + 1U] = { 0 };


/*----------------------------------------------------------------------------
 * local_AsymBigIntToHW
 */
static void
local_AsymBigIntToHW(const Asn1Der_BigNumber * const pBigInt,
                     const size_t ModulusSizeBits,
                     const uint8_t BeginItem,
                     const uint8_t Items,
                     uint8_t * pBlob)
{
    uint8_t * ptr = pBlob;
    uint32_t RemainingLength = PSA_ASYM_DATA_SIZE_B2WB(ModulusSizeBits);
    uint32_t CopySize = (uint32_t)pBigInt->Length;

    /* Convert big integer from application to HW format */
    /* - Initialize header */
    *ptr = (uint8_t)(ModulusSizeBits);
    ptr++;
    *ptr = (uint8_t)(ModulusSizeBits >> 8);
    ptr++;
    *ptr = (uint8_t)(BeginItem);
    ptr++;
    *ptr = (uint8_t)(Items);
    ptr++;
    /* - Copy data */
    if (CopySize > RemainingLength)
    {
        /* Prevent buffer overrun */
        LOG_WARN("Warning: Truncated BigInt data to HW conversion (%u > %u)\n",
                 CopySize, RemainingLength);
        CopySize = RemainingLength;
    }
    else
    {
        /* MISRA - Intentially empty */
    }
    /* - Reverse the data */
    psaInt_ReverseMemCpy(ptr, pBigInt->pData, CopySize);
    /* - Zeroize remaining part if needed */
    RemainingLength -= CopySize;
    if (0U != RemainingLength)
    {
        (void)memset(&ptr[CopySize], 0, RemainingLength);
    }
    else
    {
        /* MISRA - Intentially empty */
    }
}


/*----------------------------------------------------------------------------
 * local_AsymBigIntPointToHW
 */
static void
local_AsymBigIntPointToHW(const Asn1Der_BigNumber * const pBigInt,
                          const size_t ModulusSizeBits,
                          const uint8_t Items,
                          uint8_t * const pBlob)
{
    Asn1Der_BigNumber LocalBigInt;

    LocalBigInt.pData = pBigInt->pData;
    LocalBigInt.Length = pBigInt->Length / 2U;
    local_AsymBigIntToHW(&LocalBigInt, ModulusSizeBits, 0, Items, pBlob);

    LocalBigInt.pData = &pBigInt->pData[LocalBigInt.Length];
    local_AsymBigIntToHW(&LocalBigInt, ModulusSizeBits,
                         1, (uint8_t)(Items & 0x0FU),
                         &pBlob[PSA_ASYM_DATA_VHEADER +
                                PSA_ASYM_DATA_SIZE_B2WB(ModulusSizeBits)]);
}


/*----------------------------------------------------------------------------
 * local_SymKeyType
 *
 * This function handles key asset creation for symmetric keys types.
 * attributes
 *     active key attributes.
 * key_size
 *      key size.
 * AssetPolicy
 *      pointer to fill asset policy.
 *
 * Return Value:
 *     psa_status_t
 *                  one of psa status
 *     AssetPolicy
 *                  Asset policy
 */
static psa_status_t
local_SymKeyType(const psa_key_attributes_t * attributes,
                 size_t key_size,
                 PsaPolicyMask_t * AssetPolicy)
{
    psa_status_t funcres = PSA_SUCCESS;

    if (PSA_ALG_IS_MAC(attributes->alg))
    {
        if (PSA_ALG_IS_HMAC(attributes->alg))
        {
            /* Hash based MAC operation */
            *AssetPolicy = EIP130_ASSET_POLICY_SYM_MACHASH;
            if (PSA_KEY_USAGE_SIGN_MESSAGE == (attributes->usage & PSA_KEY_USAGE_SIGN_MESSAGE))
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_SCDIRENCGEN;
            }
            else if (PSA_KEY_USAGE_VERIFY_MESSAGE == (attributes->usage & PSA_KEY_USAGE_VERIFY_MESSAGE))
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_SCDIRDECVRFY;
            }
            else
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            switch (PSA_ALG_HMAC_GET_HASH(attributes->alg))
            {
            case PSA_ALG_SHA_1:
                /* Minimum (security) Length limit for HMAC keys is 112/8 */
                if ((key_size < (112U / 8U)) ||
                    (key_size > PSA_SYM_ALGO_MAX_HMAC_SHA2_KEY_SIZE))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCAHSHA1;
                }
                break;
            case PSA_ALG_SHA_224:
                if ((key_size < ((224U / 8U) / 2U)) ||
                    (key_size > PSA_SYM_ALGO_MAX_HMAC_SHA2_KEY_SIZE))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCAHSHA224;
                }
                break;
            case PSA_ALG_SHA_256:
                if ((key_size < ((256U / 8U) / 2U)) ||
                    (key_size > PSA_SYM_ALGO_MAX_HMAC_SHA2_KEY_SIZE))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCAHSHA256;
                }
                break;
#ifndef PSA_REMOVE_SYM_ALGO_SHA512
            case PSA_ALG_SHA_384:
                if ((key_size < ((384U / 8U) / 2U)) ||
                    (key_size > PSA_SYM_ALGO_MAX_HMAC_SHA512_KEY_SIZE))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCAHSHA384;
                }
                break;
            case PSA_ALG_SHA_512:
                if ((key_size < ((512U / 8U) / 2U)) ||
                    (key_size > PSA_SYM_ALGO_MAX_HMAC_SHA512_KEY_SIZE))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCAHSHA512;
                }
                break;
#endif
#ifndef PSA_REMOVE_SYM_ALGO_SHA3
            case PSA_ALG_SHA3_224:
                if ((key_size < ((224U / 8U) / 2U)) ||
                    (key_size > (1152U / 8U)))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCAHSHA3_224;
                }
                break;
            case PSA_ALG_SHA3_256:
                if ((key_size < ((256U / 8U) / 2U)) ||
                    (key_size > (1088U / 8U)))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCAHSHA3_256;
                }
                break;
            case PSA_ALG_SHA3_384:
                if ((key_size < ((384U / 8U) / 2U)) ||
                    (key_size > (832U / 8U)))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCAHSHA3_384;
                }
                break;
            case PSA_ALG_SHA3_512:
                if ((key_size < ((512U / 8U) / 2U)) ||
                    (key_size > (576U / 8U)))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCAHSHA3_512;
                }
                break;
#endif
            default:
                funcres = PSA_ERROR_INVALID_ARGUMENT;
                break;
            }
        }
        else
        {
            /* Cipher based MAC operation */
            if ((key_size != (128U / 8U)) &&
                (key_size != (192U / 8U)) &&
                (key_size != (256U / 8U)))
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                *AssetPolicy = EIP130_ASSET_POLICY_SYM_MACCIPHER;
                if (PSA_KEY_USAGE_SIGN_MESSAGE == (attributes->usage & PSA_KEY_USAGE_SIGN_MESSAGE))
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCDIRENCGEN;
                }
                else if (PSA_KEY_USAGE_VERIFY_MESSAGE == (attributes->usage & PSA_KEY_USAGE_VERIFY_MESSAGE))
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCDIRDECVRFY;
                }
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                if (PSA_ALG_CMAC == attributes->alg)
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCMCMCMAC;
                }
                else if (PSA_ALG_CBC_MAC == attributes->alg)
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCMCMCBCMAC;
                }
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                if (PSA_KEY_TYPE_AES == attributes->type)
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCACAES;
                }
#ifndef PSA_REMOVE_SYM_ALGO_ARIA
                else if (PSA_KEY_TYPE_ARIA == attributes->type)
                {
                    *AssetPolicy |= EIP130_ASSET_POLICY_SCACARIA;
                }
#endif
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
            }
        }
    }
    else if (PSA_ALG_IS_CIPHER(attributes->alg))
    {
        *AssetPolicy = EIP130_ASSET_POLICY_SYM_CIPHERBULK;
        if (PSA_KEY_USAGE_ENCRYPT == (attributes->usage & PSA_KEY_USAGE_ENCRYPT))
        {
            *AssetPolicy |= EIP130_ASSET_POLICY_SCDIRENCGEN;
        }
        else if (PSA_KEY_USAGE_DECRYPT == (attributes->usage & PSA_KEY_USAGE_DECRYPT))
        {
            *AssetPolicy |= EIP130_ASSET_POLICY_SCDIRDECVRFY;
        }
        else
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        if (PSA_KEY_TYPE_AES == attributes->type)
        {
            *AssetPolicy |= EIP130_ASSET_POLICY_SCACAES;
            if (PSA_ALG_XTS == attributes->alg)
            {
                if ((key_size != (256U / 8U)) &&
                    (key_size != (512U / 8U)))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    /* MISRA - Intentially empty */
                }
            }
            else
            {
                if ((key_size != (128U / 8U)) &&
                    (key_size != (192U / 8U)) &&
                    (key_size != (256U / 8U)))
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    /* MISRA - Intentially empty */
                }
            }
        }
#ifndef PSA_REMOVE_SYM_ALGO_ARIA
        else if (PSA_KEY_TYPE_ARIA == attributes->type)
        {
            if ((key_size != (128U / 8U)) &&
                (key_size != (192U / 8U)) &&
                (key_size != (256U / 8U)))
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_SCACARIA;
            }
        }
#endif
#ifndef PSA_REMOVE_SYM_ALGO_3DES
        else if (PSA_KEY_TYPE_DES == attributes->type)
        {
            if (key_size != (3U * (64U / 8U)))
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_SCACTDES;
            }
        }
#endif
        else
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        switch (attributes->alg)
        {
        case PSA_ALG_CTR:
#ifndef PSA_REMOVE_SYM_ALGO_3DES
            if (PSA_KEY_TYPE_DES != attributes->type)
#endif
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_SCMCBCTR32;
            }
#ifndef PSA_REMOVE_SYM_ALGO_3DES
            else
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
#endif
            break;
        case PSA_ALG_XTS:
            if (PSA_KEY_TYPE_AES == attributes->type)
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_SCMCBXTS;
            }
            else
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            break;
        case PSA_ALG_ECB_NO_PADDING:
            *AssetPolicy |= EIP130_ASSET_POLICY_SCMCBECB;
            break;
        case PSA_ALG_CBC_NO_PADDING:
            *AssetPolicy |= EIP130_ASSET_POLICY_SCMCBCBC;
            break;
        default:
            funcres = PSA_ERROR_INVALID_ARGUMENT;
            break;
        }
    }
    else if (PSA_ALG_IS_AEAD(attributes->alg))
    {
        *AssetPolicy = EIP130_ASSET_POLICY_SYM_CIPHERAUTH;
        if (PSA_KEY_USAGE_ENCRYPT == (attributes->usage & PSA_KEY_USAGE_ENCRYPT))
        {
            *AssetPolicy |= EIP130_ASSET_POLICY_SCDIRENCGEN;
        }
        else if (PSA_KEY_USAGE_DECRYPT == (attributes->usage & PSA_KEY_USAGE_DECRYPT))
        {
            *AssetPolicy |= EIP130_ASSET_POLICY_SCDIRDECVRFY;
        }
        else
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        if (PSA_KEY_TYPE_AES == attributes->type)
        {
            if ((key_size != (128U / 8U)) &&
                (key_size != (192U / 8U)) &&
                (key_size != (256U / 8U)))
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_SCACAES;
            }
        }
#ifndef PSA_REMOVE_SYM_ALGO_ARIA
        else if (PSA_KEY_TYPE_ARIA == attributes->type)
        {
            if ((key_size != (128U / 8U)) &&
                (key_size != (192U / 8U)) &&
                (key_size != (256U / 8U)))
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_SCACARIA;
            }
        }
#endif
#ifndef PSA_REMOVE_SYM_ALGO_CHACHA20
        else if (PSA_KEY_TYPE_CHACHA20 == attributes->type)
        {
            if (key_size != (256U / 8U))
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_SCACCHACHA20;
            }
        }
#endif
        else
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        switch (attributes->alg)
        {
#if !defined(PSA_REMOVE_SYM_ALGO_CHACHA20) && \
    !defined(PSA_REMOVE_SYM_ALGO_POLY1305)
        case PSA_ALG_CHACHA20_POLY1305:
#endif
        case PSA_ALG_CCM:
            *AssetPolicy |= EIP130_ASSET_POLICY_SCMCACCM;
            break;
        case PSA_ALG_GCM:
            *AssetPolicy |= EIP130_ASSET_POLICY_SCMCAGCM;
            break;
        default:
            funcres = PSA_ERROR_INVALID_ARGUMENT;
            break;
        }
    }
    else
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    if (PSA_SUCCESS == funcres)
    {
        if (!psaInt_IsAccessSecure())
        {
            *AssetPolicy |= PSA_POLICY_SOURCE_NON_SECURE;
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }
    else
    {
        *AssetPolicy = (PsaPolicyMask_t)0;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * local_AsymHashPolicy
 *
 * This function get hash mode to be used in asymmetric key and update
 * the policy.
 *
 * alg
 *     active alg to get hash type from.
 * asset_policy
 *      asset policy to update with hash policy.
 *
 * Return Value:
 *     psa_status_t
 *                  one of psa status
 */
static psa_status_t
local_AsymHashPolicy(psa_algorithm_t alg,
                     PsaPolicyMask_t *asset_policy,
                     uint32_t ModulusSizeBits,
                     uint8_t check)
{
    psa_status_t funcres = PSA_SUCCESS;

    if (0U != check)
    {
        size_t DigestNBits = (8U * PSA_HASH_LENGTH(PSA_ALG_GET_HASH(alg)));
        if (PSA_ALG_SHA_512 == (PSA_ALG_GET_HASH(alg)))
        {
            if ((ModulusSizeBits < DigestNBits) && (ModulusSizeBits != 255U))
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
        }
        else
        {
            if (ModulusSizeBits < DigestNBits)
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
        }
    }
    else
    {
        /* MISRA - Intentially empty */
    }
    if (PSA_SUCCESS == funcres)
    {
        switch (PSA_ALG_GET_HASH(alg))
        {
        case PSA_ALG_SHA_1:
            *asset_policy |= EIP130_ASSET_POLICY_ACH_SHA1;
            break;
        case PSA_ALG_SHA_224:
            *asset_policy |= EIP130_ASSET_POLICY_ACH_SHA224;
            break;
        case PSA_ALG_SHA_256:
            *asset_policy |= EIP130_ASSET_POLICY_ACH_SHA256;
            break;
#ifndef PSA_REMOVE_SYM_ALGO_SHA512
        case PSA_ALG_SHA_384:
            *asset_policy |= EIP130_ASSET_POLICY_ACH_SHA384;
            break;
        case PSA_ALG_SHA_512:
            *asset_policy |= EIP130_ASSET_POLICY_ACH_SHA512;
            break;
#endif
        default:
            funcres = PSA_ERROR_INVALID_ARGUMENT;
            break;
        }
    }
    else
    {
        /* Error already set */
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * local_AsymKeyType
 *
 * This function handles key asset creation for Asymmetric keys types.
 * attributes
 *     active key attributes.
 * data_length
 *      data size.
 * key
 *      key ID.
 *
 * Return Value:
 *     psa_status_t
 *                  one of psa status
 *     key
 *                  Key ID
 */
static psa_status_t
local_AsymKeyType(const size_t PersistentItemSize,
                  const psa_key_attributes_t * attributes,
                  const uint8_t * data,
                  size_t data_length,
                  uint8_t * pOutputData,
                  size_t * pOutputDataLength,
                  size_t * modulus_size,
                  size_t * exponentsize,
                  PsaPolicyMask_t * AssetPolicy)
{
    psa_status_t funcres = PSA_SUCCESS;

    if (PSA_ALG_IS_SIGN(attributes->alg))
    {
        *AssetPolicy = EIP130_ASSET_POLICY_ASYM_SIGNVERIFY;
        if (PSA_KEY_TYPE_IS_RSA(attributes->type))
        {
            funcres = local_AsymHashPolicy(attributes->alg, AssetPolicy, 0U, 0U);
            if (PSA_SUCCESS == funcres)
            {
                Asn1Der_BigNumber Modulus;
                Asn1Der_BigNumber PubExponent;

                if (PSA_KEY_TYPE_IS_KEY_PAIR(attributes->type))
                {
                    Asn1Der_BigNumber PrvExponent;

#ifdef EIP130_ENABLE_DPA_SUPPORT
                    *AssetPolicy |= EIP130_ASSET_POLICY_AC_NDPA;
#endif
                    if (PSA_ALG_IS_RSA_PSS(attributes->alg))
                    {
                        *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA |
                                        EIP130_ASSET_POLICY_ACA_RSAPSS;

                        if (0U == PersistentItemSize)
                        {
                            funcres = PSAInt_AsymParse_RSA_PSS(data, data_length,
                                                               &Modulus, &PrvExponent,
                                                               &PubExponent);
                        }
                        else
                        {
                            /* MISRA - Intentially empty */
                        }
                    }
                    else if (PSA_ALG_IS_RSA_PKCS1V15_SIGN(attributes->alg))
                    {
                        *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA |
                                        EIP130_ASSET_POLICY_ACA_RSAPKCS1V15;

                        if (0U == PersistentItemSize)
                        {
                            funcres = PSAInt_AsymParse_RSA_PKCS1(data, data_length,
                                                                 &Modulus, &PrvExponent,
                                                                 &PubExponent);
                        }
                        else
                        {
                            /* MISRA - Intentially empty */
                        }
                    }
                    else
                    {
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
                    if (PSA_SUCCESS == funcres)
                    {
                        if (0U == PersistentItemSize)
                        {
                            *modulus_size = PSA_BYTES_TO_BITS(Modulus.Length);

                            if ((0U != attributes->bits) &&
                                (*modulus_size != attributes->bits))
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                            else
                            {
                                *exponentsize = *modulus_size;

                                /* Convert to VaultIP HW format */
                                if ((NULL == pOutputData) ||
                                    (NULL == pOutputDataLength) ||
                                    (*pOutputDataLength < (2U * PSA_ASYM_DATA_SIZE_VWB(*modulus_size))))
                                {
                                    funcres = PSA_ERROR_BUFFER_TOO_SMALL;
                                }
                                else
                                {
                                    *pOutputDataLength = 2U * PSA_ASYM_DATA_SIZE_VWB(*modulus_size);
                                    local_AsymBigIntToHW(&Modulus, *modulus_size,
                                                         0, 2, pOutputData);
                                    local_AsymBigIntToHW(&PrvExponent, *exponentsize,
                                                         1, 2, &pOutputData[PSA_ASYM_DATA_VHEADER +
                                                         Modulus.Length]);
                                }
                            }
                        }
                        else
                        {
                            if (0U != attributes->bits)
                            {
                                *modulus_size = attributes->bits;
                                *exponentsize = attributes->bits;
                            }
                            else
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                        }
                    }
                    else
                    {
                        /* Error already set */
                    }
                }
                else if (PSA_KEY_TYPE_IS_PUBLIC_KEY(attributes->type))
                {
                    uint8_t fPSS;

                    if (0U == PersistentItemSize)
                    {
                        funcres = PSAInt_AsymParse_RSA_PubKey(data, data_length,
                                                              &Modulus, &PubExponent,
                                                              &fPSS);
                    }
                    else
                    {
                        if (PSA_ALG_IS_RSA_PSS(attributes->alg))
                        {
                            fPSS = 255U;
                        }
                        else
                        {
                            fPSS = 0U;
                        }
                    }
                    if (PSA_SUCCESS == funcres)
                    {
                        if (PSA_ALG_IS_RSA_PSS(attributes->alg))
                        {
                            if (0U != fPSS)
                            {
                                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_RSAPSS |
                                                EIP130_ASSET_POLICY_PUBLICDATA;
                            }
                            else
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                        }
                        else if (PSA_ALG_IS_RSA_PKCS1V15_SIGN(attributes->alg))
                        {
                            if (0U == fPSS)
                            {
                                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_RSAPKCS1V15 |
                                                EIP130_ASSET_POLICY_PUBLICDATA;
                            }
                            else
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                        }
                        else
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                    }
                    else
                    {
                        /* Error already set */
                    }
                    if (PSA_SUCCESS == funcres)
                    {
                        if (0U == PersistentItemSize)
                        {
                            *modulus_size = PSA_BYTES_TO_BITS(Modulus.Length);

                            if ((0U != attributes->bits) &&
                                (*modulus_size != attributes->bits))
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                            else
                            {
                                *exponentsize = PSA_BYTES_TO_BITS(PubExponent.Length);

                                /* Convert to VaultIP HW format */
                                if ((NULL == pOutputData) ||
                                    (NULL == pOutputDataLength) ||
                                    (*pOutputDataLength < (PSA_ASYM_DATA_SIZE_VWB(*modulus_size) +
                                                           PSA_ASYM_DATA_SIZE_VWB(*exponentsize))))
                                {
                                    funcres = PSA_ERROR_BUFFER_TOO_SMALL;
                                }
                                else
                                {
                                    *pOutputDataLength = (PSA_ASYM_DATA_SIZE_VWB(*modulus_size) +
                                                          PSA_ASYM_DATA_SIZE_VWB(*exponentsize));
                                    local_AsymBigIntToHW(&Modulus, *modulus_size,
                                                         0, 2, pOutputData);
                                    local_AsymBigIntToHW(&PubExponent, *exponentsize,
                                                         1, 2, &pOutputData[PSA_ASYM_DATA_VHEADER +
                                                                            Modulus.Length]);
                                }
                            }
                        }
                        else
                        {
                            if (0U != attributes->bits)
                            {
                                *modulus_size = attributes->bits;
                                *exponentsize = PersistentItemSize -
                                                (2 * PSA_ASYM_DATA_VHEADER) -
                                                PSA_ASYM_DATA_SIZE_VWB(attributes->bits);
                            }
                            else
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                        }
                    }
                    else
                    {
                        /* Error already set */
                    }
                }
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
            }
            else
            {
                /* Error already set */
            }
        }
        else if (PSA_KEY_TYPE_IS_ECC(attributes->type))
        {
            if (PSA_ALG_IS_ECDSA(attributes->alg))
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDSA;

                funcres = local_AsymHashPolicy(attributes->alg,
                                               AssetPolicy, attributes->bits, 255U);
                if (PSA_SUCCESS == funcres)
                {
                    Asn1Der_BigNumber KeyInfo;
                    size_t CurveBits;
                    uint8_t CurveFamily;
                    uint8_t items;

                    if (PSA_KEY_TYPE_IS_KEY_PAIR(attributes->type))
                    {
                        *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA;

                        if (0U == PersistentItemSize)
                        {
                            funcres = PSAInt_AsymParse_EC_SEC1(data, data_length,
                                                               &KeyInfo,
                                                               &CurveFamily,
                                                               &CurveBits,
                                                               NULL);
                            if (PSA_SUCCESS == funcres)
                            {
                                switch (CurveFamily)
                                {
                                case EIP130DOMAIN_ECC_FAMILY_NIST_P:
                                    if (PSA_ECC_FAMILY_SECP_R1 !=
                                        PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                                    {
                                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                                    }
                                    else
                                    {
                                        items = 0x1U;
                                    }
                                    break;
                                case EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1:
                                    if (PSA_ECC_FAMILY_BRAINPOOL_P_R1 !=
                                        PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                                    {
                                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                                    }
                                    else
                                    {
                                        items = 0x11U;
#ifdef EIP130_ENABLE_DPA_SUPPORT
                                        *AssetPolicy |= EIP130_ASSET_POLICY_AC_NDPA;
#endif
                                    }
                                    break;
                                default:
                                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                                    break;
                                }
                            }
                            else
                            {
                                /* Error already set */
                            }
                        }
                        else
                        {
                            switch (PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                            {
                            case PSA_ECC_FAMILY_SECP_R1:
                                break;
                            case PSA_ECC_FAMILY_BRAINPOOL_P_R1:
#ifdef EIP130_ENABLE_DPA_SUPPORT
                                *AssetPolicy |= EIP130_ASSET_POLICY_AC_NDPA;
#endif
                                break;
                            default:
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                                break;
                            }
                        }
                        if (PSA_SUCCESS == funcres)
                        {
                            if (0U == PersistentItemSize)
                            {
                                if (attributes->bits != CurveBits)
                                {
                                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                                }
                                else
                                {
                                    /* Convert to VaultIP HW format */
                                    if ((NULL == pOutputData) ||
                                        (NULL == pOutputDataLength) ||
                                        (*pOutputDataLength < (PSA_ASYM_DATA_SIZE_VWB(CurveBits))))
                                    {
                                        funcres = PSA_ERROR_BUFFER_TOO_SMALL;
                                    }
                                    else
                                    {
                                        *modulus_size = CurveBits;
                                        *exponentsize = CurveBits;

                                        *pOutputDataLength = PSA_ASYM_DATA_SIZE_VWB(CurveBits);
                                        local_AsymBigIntToHW(&KeyInfo, CurveBits, 0,
                                                             items, pOutputData);
                                    }
                                }
                            }
                            else
                            {
                                if (0U != attributes->bits)
                                {
                                    *modulus_size = attributes->bits;
                                    *exponentsize = attributes->bits;
                                }
                                else
                                {
                                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                                }
                            }
                        }
                        else
                        {
                            /* Error already set */
                        }
                    }
                    else if (PSA_KEY_TYPE_IS_PUBLIC_KEY(attributes->type))
                    {
                        if (0U == PersistentItemSize)
                        {
                            uint8_t fECDH;
                            funcres = psaInt_KeyMgmtReadECPubKey(data, data_length,
                                                                 PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type),
                                                                 attributes->bits,
                                                                 &CurveFamily, &CurveBits,
                                                                 &fECDH,
                                                                 pOutputData,
                                                                 pOutputDataLength);
                            if (PSA_SUCCESS == funcres)
                            {
                                if (0U != fECDH)
                                {
                                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                                }
                                else
                                {
                                    *modulus_size = CurveBits;
                                    *exponentsize = CurveBits;
                                }
                            }
                            else
                            {
                                /* Error already set */
                            }
                        }
                        else
                        {
                            if (0U != attributes->bits)
                            {
                                *modulus_size = attributes->bits;
                                *exponentsize = attributes->bits;
                            }
                            else
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                        }
                    }
                    else
                    {
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
                }
                else
                {
                    /* Error already set */
                }
            }
            else if (PSA_ALG_PURE_EDDSA == attributes->alg)
            {
                Asn1Der_BigNumber KeyInfo;
                size_t CurveBits;
                uint8_t CurveFamily;

                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_EDDSA | EIP130_ASSET_POLICY_ACH_SHA512;

                if (PSA_KEY_TYPE_IS_KEY_PAIR(attributes->type))
                {
#ifdef EIP130_ENABLE_DPA_SUPPORT
                    *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA |
                                    EIP130_ASSET_POLICY_AC_NDPA;
#else
                    *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA;
#endif
                    if (0U == PersistentItemSize)
                    {
                        funcres = PSAInt_AsymParse_EC_EdX(data, data_length,
                                                          &KeyInfo,
                                                          &CurveFamily, &CurveBits,
                                                          NULL);
                        if (PSA_SUCCESS == funcres)
                        {
                            if ((CurveFamily != EIP130DOMAIN_ECC_FAMILY_TWISTED_EDWARDS) ||
                                (PSA_ECC_FAMILY_TWISTED_EDWARDS !=
                                 PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type)))
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                            else
                            {
                                if (attributes->bits != CurveBits)
                                {
                                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                                }
                                else
                                {
                                    *modulus_size = CurveBits;
                                    *exponentsize = CurveBits;

                                    /* Convert to VaultIP HW format */
                                    if ((NULL == pOutputData) ||
                                        (NULL == pOutputDataLength) ||
                                        (*pOutputDataLength < PSA_ASYM_DATA_SIZE_VWB(CurveBits)))
                                    {
                                        funcres = PSA_ERROR_BUFFER_TOO_SMALL;
                                    }
                                    else
                                    {
                                        *pOutputDataLength = PSA_ASYM_DATA_SIZE_VWB(CurveBits);
                                        local_AsymBigIntToHW(&KeyInfo, CurveBits, 0,
                                                             0x11U, pOutputData);
                                    }
                                }
                            }
                        }
                        else
                        {
                            /* Error already set */
                        }
                    }
                    else
                    {
                        if (PSA_ECC_FAMILY_TWISTED_EDWARDS !=
                             PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                        else
                        {
                            if (0U != attributes->bits)
                            {
                                *modulus_size = attributes->bits;
                                *exponentsize = attributes->bits;
                            }
                            else
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                        }
                    }
                }
                else if (PSA_KEY_TYPE_IS_PUBLIC_KEY(attributes->type))
                {
                    uint8_t fECDH;

                    if (0U == PersistentItemSize)
                    {
                        funcres = psaInt_KeyMgmtReadECPubKey(data, data_length,
                                                             PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type),
                                                             attributes->bits,
                                                             &CurveFamily, &CurveBits,
                                                             &fECDH,
                                                             pOutputData,
                                                             pOutputDataLength);
                        if (PSA_SUCCESS == funcres)
                        {
                            if (0U != fECDH)
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                            else
                            {
                                *modulus_size = CurveBits;
                                *exponentsize = CurveBits;
                            }
                        }
                        else
                        {
                            /* Error already set */
                        }
                    }
                    else
                    {
                        if (0U != attributes->bits)
                        {
                            *modulus_size = attributes->bits;
                            *exponentsize = attributes->bits;
                        }
                        else
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                    }
                }
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
            }
            else
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
        }
        else
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
    }
    else if (PSA_ALG_IS_ASYMMETRIC_ENCRYPTION(attributes->alg))
    {
        *AssetPolicy = EIP130_ASSET_POLICY_ASYM_DECENC;
        if (PSA_ALG_RSA_PKCS1V15_CRYPT == attributes->alg)
        {
            *AssetPolicy |= EIP130_ASSET_POLICY_ACA_RSAPKCS1V15;
        }
        else if (PSA_ALG_IS_RSA_OAEP(attributes->alg))
        {
            *AssetPolicy |= EIP130_ASSET_POLICY_ACA_RSAOEAP;
            funcres = local_AsymHashPolicy(attributes->alg, AssetPolicy, 0U, 0U);
        }
        else
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        if (PSA_SUCCESS == funcres)
        {
            Asn1Der_BigNumber Modulus;
            Asn1Der_BigNumber PubExponent;

            if (PSA_KEY_TYPE_IS_KEY_PAIR(attributes->type))
            {
                Asn1Der_BigNumber PrvExponent;

#ifdef EIP130_ENABLE_DPA_SUPPORT
                *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA |
                                EIP130_ASSET_POLICY_AC_NDPA |
                                EIP130_ASSET_POLICY_STANY;
#else
                *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA |
                                EIP130_ASSET_POLICY_STANY;
#endif
                if (0U == PersistentItemSize)
                {
                    funcres = PSAInt_AsymParse_RSA_PKCS1(data, data_length,
                                                         &Modulus, &PrvExponent,
                                                         &PubExponent);
                    if (PSA_SUCCESS == funcres)
                    {
                        if (PrvExponent.Length <= Modulus.Length)
                        {
                            *modulus_size = PSA_BYTES_TO_BITS(Modulus.Length);
                            *exponentsize = *modulus_size;

                            /* Convert to VaultIP HW format */
                            if ((NULL == pOutputData) ||
                                (NULL == pOutputDataLength) ||
                                (*pOutputDataLength < (2U * PSA_ASYM_DATA_SIZE_VWB(*modulus_size))))
                            {
                                funcres = PSA_ERROR_BUFFER_TOO_SMALL;
                            }
                            else
                            {
                                *pOutputDataLength = 2U * PSA_ASYM_DATA_SIZE_VWB(*modulus_size);
                                if (attributes->bits == (*modulus_size))
                                {
                                    local_AsymBigIntToHW(&Modulus, *modulus_size,
                                                         0, 2, pOutputData);
                                    local_AsymBigIntToHW(&PrvExponent, *exponentsize,
                                                         1, 2, &pOutputData[PSA_ASYM_DATA_VHEADER +
                                                                            Modulus.Length]);
                                }
                                else
                                {
                                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                                }
                            }
                        }
                        else
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                    }
                    else
                    {
                        /* Error already set */
                    }
                }
                else
                {
                    if (0U != attributes->bits)
                    {
                        *modulus_size = attributes->bits;
                        *exponentsize = attributes->bits;
                    }
                    else
                    {
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
                }
            }
            else if (PSA_KEY_TYPE_IS_PUBLIC_KEY(attributes->type))
            {
                uint8_t fPSS;

                if (0U == PersistentItemSize)
                {
                    funcres = PSAInt_AsymParse_RSA_PubKey(data, data_length,
                                                          &Modulus, &PubExponent,
                                                          &fPSS);
                    if (PSA_SUCCESS == funcres)
                    {
                        if (0U == fPSS)
                        {
                            *modulus_size = PSA_BYTES_TO_BITS(Modulus.Length);
                            *exponentsize = PSA_BYTES_TO_BITS(PubExponent.Length);

                            /* Convert to VaultIP HW format */
                            if ((NULL == pOutputData) ||
                                (NULL == pOutputDataLength) ||
                                (*pOutputDataLength < (PSA_ASYM_DATA_SIZE_VWB(*modulus_size) +
                                                       PSA_ASYM_DATA_SIZE_VWB(*exponentsize))))
                            {
                                funcres = PSA_ERROR_BUFFER_TOO_SMALL;
                            }
                            else
                            {
                                *pOutputDataLength = (PSA_ASYM_DATA_SIZE_VWB(*modulus_size) +
                                                      PSA_ASYM_DATA_SIZE_VWB(*exponentsize));
                                if (attributes->bits == (*modulus_size))
                                {
                                    local_AsymBigIntToHW(&Modulus, *modulus_size,
                                                         0, 2, pOutputData);
                                    local_AsymBigIntToHW(&PubExponent, *exponentsize,
                                                         1, 2, &pOutputData[PSA_ASYM_DATA_VHEADER +
                                                                            Modulus.Length]);
                                }
                                else
                                {
                                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                                }
                            }
                        }
                        else
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                    }
                    else
                    {
                        /* Error already set */
                    }
                }
                else
                {
                    if (0U != attributes->bits)
                    {
                        *modulus_size = attributes->bits;
                        *exponentsize = attributes->bits;
                    }
                    else
                    {
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
                }
            }
            else
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
        }
        else
        {
            /* Error already set */
        }
    }
    else if (PSA_ALG_IS_KEY_AGREEMENT(attributes->alg))
    {
        *AssetPolicy = EIP130_ASSET_POLICY_ASYM_KEYEXCHANGE;
        if (PSA_ALG_IS_FFDH(attributes->alg))
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        else if (PSA_ALG_IS_ECDH(attributes->alg))
        {
            Asn1Der_BigNumber KeyInfo;
            size_t CurveBits;
            uint8_t CurveFamily;
            uint8_t items;

            if (PSA_KEY_TYPE_IS_KEY_PAIR(attributes->type))
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA |
                                EIP130_ASSET_POLICY_STANY;

                if (PSA_ECC_FAMILY_MONTGOMERY ==
                    PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                {
                    if (0U == PersistentItemSize)
                    {
                        funcres = PSAInt_AsymParse_EC_EdX(data, data_length,
                                                          &KeyInfo,
                                                          &CurveFamily, &CurveBits,
                                                          NULL);
                    }
                    else
                    {
                        /* MISRA - Intentially empty */
                    }
                }
                else if ((PSA_ECC_FAMILY_SECP_R1 ==
                          PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type)) ||
                         (PSA_ECC_FAMILY_BRAINPOOL_P_R1 ==
                          PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type)))
                {
                    if (0U == PersistentItemSize)
                    {
                        funcres = PSAInt_AsymParse_EC_SEC1(data, data_length,
                                                           &KeyInfo,
                                                           &CurveFamily, &CurveBits,
                                                           NULL);
                    }
                    else
                    {
                        /* MISRA - Intentially empty */
                    }
                }
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                if (PSA_SUCCESS == funcres)
                {
                    if (0U == PersistentItemSize)
                    {
                        switch (CurveFamily)
                        {
                        case EIP130DOMAIN_ECC_FAMILY_NIST_P:
                            if (PSA_ECC_FAMILY_SECP_R1 !=
                                    PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                            else
                            {
                                items = 0x1U;
                                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDH;
                            }
                            break;
                        case EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1:
                            if (PSA_ECC_FAMILY_BRAINPOOL_P_R1 !=
                                    PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                            else
                            {
                                items = 0x11U;
#ifdef EIP130_ENABLE_DPA_SUPPORT
                                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDH |
                                                EIP130_ASSET_POLICY_AC_NDPA;
#else
                                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDH;
#endif
                            }
                            break;
                        case EIP130DOMAIN_ECC_FAMILY_MONTGOMERY:
                            if (PSA_ECC_FAMILY_MONTGOMERY !=
                                    PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                            {
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                            }
                            else
                            {
                                items = 0x01U;
#ifdef EIP130_ENABLE_DPA_SUPPORT
                                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_CURVE25519 |
                                                EIP130_ASSET_POLICY_AC_NDPA;
#else
                                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_CURVE25519;
#endif
                            }
                            break;
                        default:
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                            break;
                        }
                    }
                    else
                    {
                        switch (PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                        {
                        case PSA_ECC_FAMILY_SECP_R1:
                            *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDH;
                            break;
                        case PSA_ECC_FAMILY_BRAINPOOL_P_R1:
#ifdef EIP130_ENABLE_DPA_SUPPORT
                            *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDH |
                                            EIP130_ASSET_POLICY_AC_NDPA;
#else
                            *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDH;
#endif
                            break;
                        case PSA_ECC_FAMILY_MONTGOMERY:
#ifdef EIP130_ENABLE_DPA_SUPPORT
                            *AssetPolicy |= EIP130_ASSET_POLICY_ACA_CURVE25519 |
                                            EIP130_ASSET_POLICY_AC_NDPA;
#else
                            *AssetPolicy |= EIP130_ASSET_POLICY_ACA_CURVE25519;
#endif
                            break;
                        default:
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                            break;
                        }
                    }
                }
                else
                {
                    /* Error already set */
                }
                if (PSA_SUCCESS == funcres)
                {
                    if (0U == PersistentItemSize)
                    {
                        if (attributes->bits != CurveBits)
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                        else
                        {
                            *modulus_size = CurveBits;
                            *exponentsize = CurveBits;

                            /* Convert to VaultIP HW format */
                            if ((NULL == pOutputData) ||
                                (NULL == pOutputDataLength) ||
                                (*pOutputDataLength < PSA_ASYM_DATA_SIZE_VWB(CurveBits)))
                            {
                                funcres = PSA_ERROR_BUFFER_TOO_SMALL;
                            }
                            else
                            {
                                *pOutputDataLength = PSA_ASYM_DATA_SIZE_VWB(CurveBits);
                                local_AsymBigIntToHW(&KeyInfo, CurveBits, 0,
                                                     items, pOutputData);
                            }
                        }
                    }
                    else
                    {
                        if (0U != attributes->bits)
                        {
                            *modulus_size = attributes->bits;
                            *exponentsize = attributes->bits;
                        }
                        else
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                    }
                }
                else
                {
                    /* Error already set */
                }
            }
            else if (PSA_KEY_TYPE_IS_PUBLIC_KEY(attributes->type))
            {
                uint8_t fECDH = 0U;

                if (0U == PersistentItemSize)
                {
                    funcres = psaInt_KeyMgmtReadECPubKey(data, data_length,
                                                         PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type),
                                                         attributes->bits,
                                                         &CurveFamily, &CurveBits,
                                                         &fECDH,
                                                         pOutputData,
                                                         pOutputDataLength);
                }
                else
                {
                    fECDH = 255U;       /* Assume ECDH */
                    switch (PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                    {
                    case PSA_ECC_FAMILY_SECP_R1:
                    case PSA_ECC_FAMILY_BRAINPOOL_P_R1:
                        *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDH;
                        break;
                    case PSA_ECC_FAMILY_MONTGOMERY:
                        *AssetPolicy |= EIP130_ASSET_POLICY_ACA_CURVE25519;
                        break;
                    default:
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                        break;
                    }
                }
                if (PSA_SUCCESS == funcres)
                {
                    if (0U == PersistentItemSize)
                    {
                        if (0U == fECDH)
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                        else
                        {
                            *modulus_size = CurveBits;
                            *exponentsize = CurveBits;

                            switch (CurveFamily)
                            {
                            case EIP130DOMAIN_ECC_FAMILY_NIST_P:
                            case EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1:
                                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDH;
                                break;
                            case EIP130DOMAIN_ECC_FAMILY_MONTGOMERY:
                                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_CURVE25519;
                                break;
                            default:
                                funcres = PSA_ERROR_INVALID_ARGUMENT;
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (0U != attributes->bits)
                        {
                            *modulus_size = attributes->bits;
                            *exponentsize = attributes->bits;
                        }
                        else
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                    }
                }
                else
                {
                    /* Error already set */
                }
            }
            else
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
        }
        else
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
    }
    else
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    if (PSA_SUCCESS == funcres)
    {
        if (!psaInt_IsAccessSecure())
        {
            *AssetPolicy |= PSA_POLICY_SOURCE_NON_SECURE;
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }
    else
    {
        *AssetPolicy = (PsaPolicyMask_t)0;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * local_AsymBigIntToHW
 */
static psa_status_t
local_AsymKeyGenType(const psa_key_attributes_t * attributes,
                     uint8_t * pCurveFamily,
                     size_t * pModulusSize,
                     uint8_t * pfECDH,
                     PsaPolicyMask_t * AssetPolicy)
{
    psa_status_t funcres = PSA_SUCCESS;

    if (PSA_ALG_IS_SIGN(attributes->alg))
    {
        *pfECDH = 0U;
        *AssetPolicy = EIP130_ASSET_POLICY_ASYM_SIGNVERIFY;
        if (PSA_KEY_TYPE_IS_ECC(attributes->type))
        {
            *pModulusSize = (size_t)attributes->bits;

            if (PSA_ALG_IS_ECDSA(attributes->alg))
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDSA;

                if (PSA_ECC_FAMILY_SECP_R1 ==
                    PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                {
                    *pCurveFamily = EIP130DOMAIN_ECC_FAMILY_NIST_P;
                }
                else if (PSA_ECC_FAMILY_BRAINPOOL_P_R1 ==
                         PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                {
                    *pCurveFamily = EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1;
                }
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                if (PSA_SUCCESS == funcres)
                {
                    funcres = local_AsymHashPolicy(attributes->alg, AssetPolicy,
                                                   attributes->bits, 255U);
                }
                else
                {
                    /* Error already set */
                }
                if (PSA_SUCCESS == funcres)
                {
                    if (PSA_KEY_TYPE_IS_KEY_PAIR(attributes->type))
                    {
                        *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA;
#ifdef EIP130_ENABLE_DPA_SUPPORT
                        if (EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1 == *pCurveFamily)
                        {
                            *AssetPolicy |= EIP130_ASSET_POLICY_AC_NDPA;
                        }
#endif
                    }
                    else if (PSA_KEY_TYPE_IS_PUBLIC_KEY(attributes->type))
                    {
                        /* The AssetPolicy is already OK */
                    }
                    else
                    {
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
                }
                else
                {
                    /* Error already set */
                }
            }
            else if (PSA_ALG_PURE_EDDSA == attributes->alg)
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_EDDSA | EIP130_ASSET_POLICY_ACH_SHA512;

                if (PSA_ECC_FAMILY_TWISTED_EDWARDS ==
                    PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
                {
                    *pCurveFamily = EIP130DOMAIN_ECC_FAMILY_TWISTED_EDWARDS;
                }
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                if (PSA_SUCCESS == funcres)
                {
                    if (PSA_KEY_TYPE_IS_KEY_PAIR(attributes->type))
                    {
#ifdef EIP130_ENABLE_DPA_SUPPORT
                        *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA |
                                        EIP130_ASSET_POLICY_AC_NDPA;
#else
                        *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA;
#endif
                    }
                    else if (PSA_KEY_TYPE_IS_PUBLIC_KEY(attributes->type))
                    {
                        /* The AssetPolicy is already OK */
                    }
                    else
                    {
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
                }
                else
                {
                    /* Error already set */
                }
            }
            else
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
        }
        else
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
    }
    else if (PSA_ALG_IS_KEY_AGREEMENT(attributes->alg))
    {
        *AssetPolicy = EIP130_ASSET_POLICY_ASYM_KEYEXCHANGE;
        if (PSA_ALG_IS_FFDH(attributes->alg))
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        else if (PSA_ALG_IS_ECDH(attributes->alg))
        {
            *pfECDH = 255U;
            *pModulusSize = (size_t)attributes->bits;
            if (PSA_ECC_FAMILY_SECP_R1 ==
                PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
            {
                *pCurveFamily = EIP130DOMAIN_ECC_FAMILY_NIST_P;
                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDH;
            }
            else if (PSA_ECC_FAMILY_BRAINPOOL_P_R1 ==
                     PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
            {
                *pCurveFamily = EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1;
                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_ECDH;
            }
            else if (PSA_ECC_FAMILY_MONTGOMERY ==
                     PSA_KEY_TYPE_ECC_GET_FAMILY(attributes->type))
            {
                *pCurveFamily = EIP130DOMAIN_ECC_FAMILY_MONTGOMERY;
                *AssetPolicy |= EIP130_ASSET_POLICY_ACA_CURVE25519;
            }
            else
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            if (PSA_KEY_TYPE_IS_KEY_PAIR(attributes->type))
            {
                *AssetPolicy |= EIP130_ASSET_POLICY_PRIVATEDATA |
                                EIP130_ASSET_POLICY_STANY;

                switch (*pCurveFamily)
                {
                case EIP130DOMAIN_ECC_FAMILY_NIST_P:
                    break;
                case EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1:
                case EIP130DOMAIN_ECC_FAMILY_MONTGOMERY:
#ifdef EIP130_ENABLE_DPA_SUPPORT
                    *AssetPolicy |= EIP130_ASSET_POLICY_AC_NDPA;
#endif
                    break;
                default:
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                    break;
                }
            }
            else if (PSA_KEY_TYPE_IS_PUBLIC_KEY(attributes->type))
            {
                /* The AssetPolicy is already OK */
            }
            else
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
        }
        else
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
    }
    else
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    if (PSA_SUCCESS == funcres)
    {
        if (!psaInt_IsAccessSecure())
        {
            *AssetPolicy |= PSA_POLICY_SOURCE_NON_SECURE;
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }
    else
    {
        *AssetPolicy = (PsaPolicyMask_t)0;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * local_AsymKeyGenPrv
 *
 * Generates the private key for the ECDH, ECDSA, X25519 and ed25519
 * functionality based on standard EC curves.
 */
static psa_status_t
local_AsymKeyGenPrv(PsaPolicyMask_t AssetPolicy,
                    uint8_t CurveFamily,
                    size_t ModulusSize,
                    PsaAssetId_t * pAssetId,
                    const uint8_t * output,
                    const size_t * output_length)
{
    PsaAssetId_t KekAssetId = PSA_ASSETID_INVALID;
    PsaAssetId_t PrvKeyAssetId = PSA_ASSETID_INVALID;
    PsaAssetId_t PubKeyAssetId = PSA_ASSETID_INVALID;
    PsaAssetId_t DomainAssetId = PSA_ASSETID_INVALID;
    size_t KeySize;
    VexTokenCmd_PkAssetGenKey_t t_cmd;
    VexTokenRslt_Generic_t t_res;
    psa_status_t funcres;

    funcres = psaInt_AsymEccInstallCurve(CurveFamily, ModulusSize,
                                         &DomainAssetId);
    if (PSA_SUCCESS == funcres)
    {
        /* Create the private key Asset */
        KeySize = PSA_ASYM_DATA_SIZE_VWB(ModulusSize);
        if (NULL == pAssetId)
        {
            /* Assume key blob must be output */
            if ((NULL == output) ||
                (NULL == output_length))
            {
                /* Safeguard: Should not happen
                 *            because the buffer is always provided */
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                if (*output_length < PSA_KEYBLOB_SIZE(KeySize))
                {
                    /* Safeguard: Should not happen
                     *            because the buffer should be big enough */
                    funcres = PSA_ERROR_BUFFER_TOO_SMALL;
                }
                else
                {
                    /* MISRA - Intentially empty */
                }
            }
        }
        else
        {
            /* MISRA - Intentially empty */
        }
        if (PSA_SUCCESS == funcres)
        {
            funcres = psaInt_AssetAlloc(AssetPolicy, KeySize, &PrvKeyAssetId);
        }
        else
        {
            /* Error already set */
        }
    }
    else
    {
        /* Error already set */
    }
    if (PSA_SUCCESS == funcres)
    {
        /* Create the public key Asset */
        PsaPolicyMask_t PubAssetPolicy = (AssetPolicy & (~(EIP130_ASSET_POLICY_PRIVATEDATA |
                                                           EIP130_ASSET_POLICY_AC_NDPA|
                                                           EIP130_ASSET_POLICY_STANY))) |
                                         EIP130_ASSET_POLICY_PUBLICDATA;
        switch (CurveFamily)
        {
        case EIP130DOMAIN_ECC_FAMILY_NIST_P:
        case EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1:
            funcres = psaInt_AssetAlloc(PubAssetPolicy, (2U * KeySize),
                                        &PubKeyAssetId);
            break;
        case EIP130DOMAIN_ECC_FAMILY_MONTGOMERY:
        case EIP130DOMAIN_ECC_FAMILY_TWISTED_EDWARDS:
            funcres = psaInt_AssetAlloc(PubAssetPolicy, KeySize,
                                        &PubKeyAssetId);
            break;
        default:
            /* Safeguard: Should never happen, checked when installing the curve */
            funcres = PSA_ERROR_NOT_SUPPORTED;
            break;
        }
    }
    else
    {
        /* Error already set */
    }
    if ((PSA_SUCCESS == funcres) &&
        (NULL == pAssetId))
    {
        /* Get the KEK AssetId for the private key key blob  */
        KekAssetId = psaInt_AssetGetKeyBlobKEK();
        if (PSA_ASSETID_INVALID == KekAssetId)
        {
            funcres = PSA_ERROR_HARDWARE_FAILURE;
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }
    else
    {
        /* Error already set */
    }
    if (PSA_SUCCESS == funcres)
    {
        /* Generate a key pair */
        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_PUBLIC_KEY;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_PK_WITHASSETS;
        if (CurveFamily == EIP130DOMAIN_ECC_FAMILY_MONTGOMERY)
        {
            t_cmd.Method = (uint32_t)VEXTOKEN_PKAS_CURVE25519_GEN_KEYPAIR;
        }
        else if (CurveFamily == EIP130DOMAIN_ECC_FAMILY_TWISTED_EDWARDS)
        {
            t_cmd.Method = (uint32_t)VEXTOKEN_PKAS_EDDSA_GEN_KEYPAIR;
        }
        else
        {
            t_cmd.Method = (uint32_t)VEXTOKEN_PKAS_ECDH_ECDSA_GEN_KEYPAIR;
        }
        t_cmd.ModulusSizeInBits = (uint32_t)ModulusSize;
        t_cmd.DivisorSizeInBits = (uint32_t)ModulusSize;
        t_cmd.PrivKeyAssetId = PrvKeyAssetId;
        t_cmd.PubKeyAssetId = PubKeyAssetId;
        t_cmd.DomainAssetId = DomainAssetId;
        t_cmd.KekAssetId = KekAssetId;
        if (PSA_ASSETID_INVALID != KekAssetId)
        {
            psaInt_AssetGetKeyBlobLabel(t_cmd.AssociatedData,
                                        &t_cmd.AssociatedDataSize);
        }
        else
        {
            t_cmd.AssociatedDataSize = 0U;
        }
        t_cmd.KeyBlob_p = output;
        t_cmd.KeyBlobSize = (uint32_t)PSA_KEYBLOB_SIZE(KeySize);
        t_cmd.PubKey_p = NULL;
        t_cmd.PubKeySize = 0U;
        t_res.Result = 0;

        /* Exchange service request with the next driver level */
        funcres = psaInt_ExchangeToken((VexTokenCmd_Generic_t *)&t_cmd,
                                       sizeof(t_cmd),
                                       (VexTokenRslt_Generic_t *)&t_res,
                                       sizeof(t_res));
        if (PSA_SUCCESS == funcres)
        {
            if (0 > t_res.Result)
            {
#ifdef PSA_LOG_LOWLEVEL_ERROR
                LOG_WARN("Abort - %s()=%d\n", __func__, t_res.Result);
#endif
                funcres = PSA_ERROR_CORRUPTION_DETECTED;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
        }
        else
        {
            /* Error already set */
        }
    }
    else
    {
        /* MISRA - Intentially empty */
    }

    /* Release the used Assets if applicable */
    if (PSA_ASSETID_INVALID != PrvKeyAssetId)
    {
        if (NULL == pAssetId)
        {
            /* Assume private key key blob output */
            (void)psaInt_AssetFree(PrvKeyAssetId);
        }
        else
        {
            /* Return private key AssetId */
            *pAssetId = PrvKeyAssetId;
        }
    }
    else
    {
        /* MISRA - Intentially empty */
    }
    if (PSA_ASSETID_INVALID != PubKeyAssetId)
    {
        (void)psaInt_AssetFree(PubKeyAssetId);
    }
    else
    {
        /* MISRA - Intentially empty */
    }
    if (PSA_ASSETID_INVALID != KekAssetId)
    {
        (void)psaInt_AssetFree(KekAssetId);
    }
    else
    {
        /* MISRA - Intentially empty */
    }
    if (PSA_ASSETID_INVALID != DomainAssetId)
    {
        (void)psaInt_AssetFree(DomainAssetId);
    }
    else
    {
        /* MISRA - Intentially empty */
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * local_AsymKeyGenPub
 *
 * Generates the public key for the giving ECDH, ECDSA, X25519 and ed25519
 * private key.
 */
static psa_status_t
local_AsymKeyGenPub(PsaAssetId_t PrvKeyAssetId,
                    PsaPolicyMask_t AssetPolicy,
                    uint8_t CurveFamily,
                    size_t ModulusSize,
                    uint8_t fECDH,
                    uint8_t * data,
                    size_t data_size,
                    size_t * data_length)
{
    PsaAssetId_t DomainAssetId = PSA_ASSETID_INVALID;
    PsaAssetId_t PubKeyAssetId = PSA_ASSETID_INVALID;
    size_t PubKeySize;
    size_t RealPubKeySize;
    uint32_t Method;
    psa_status_t funcres;

    funcres = psaInt_AsymEccInstallCurve(CurveFamily, ModulusSize,
                                         &DomainAssetId);
    if (PSA_SUCCESS == funcres)
    {
        /* Create the public key Asset */
        switch (CurveFamily)
        {
        case EIP130DOMAIN_ECC_FAMILY_NIST_P:
        case EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1:
            Method = (uint32_t)VEXTOKEN_PKAS_ECDH_ECDSA_GEN_PUBKEY;
            PubKeySize = 2U * PSA_ASYM_DATA_SIZE_VWB(ModulusSize);
            RealPubKeySize = 2U * PSA_ASYM_DATA_SIZE_B2B(ModulusSize);
            break;
        case EIP130DOMAIN_ECC_FAMILY_MONTGOMERY:
            Method = (uint32_t)VEXTOKEN_PKAS_CURVE25519_GEN_PUBKEY;
            PubKeySize = PSA_ASYM_DATA_SIZE_VWB(ModulusSize);
            RealPubKeySize = PSA_ASYM_DATA_SIZE_B2B(ModulusSize);
            break;
        case EIP130DOMAIN_ECC_FAMILY_TWISTED_EDWARDS:
            Method = (uint32_t)VEXTOKEN_PKAS_EDDSA_GEN_PUBKEY;
            PubKeySize = PSA_ASYM_DATA_SIZE_VWB(ModulusSize);
            RealPubKeySize = PSA_ASYM_DATA_SIZE_B2B(ModulusSize);
            break;
        default:
            /* Safeguard: Should never happen, already checked earlier */
            funcres = PSA_ERROR_NOT_SUPPORTED;
            break;
        }

        if (PSA_SUCCESS == funcres)
        {
            if (data_size < RealPubKeySize)
            {
                funcres = PSA_ERROR_BUFFER_TOO_SMALL;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
        }
        else
        {
            /* Error already set */
        }
        if (PSA_SUCCESS == funcres)
        {
            PsaPolicyMask_t PubKeyAssetPolicy;

            PubKeyAssetPolicy = (AssetPolicy & (~(EIP130_ASSET_POLICY_PRIVATEDATA |
                                                  EIP130_ASSET_POLICY_AC_NDPA|
                                                  EIP130_ASSET_POLICY_STANY))) |
                                EIP130_ASSET_POLICY_PUBLICDATA;

            funcres = psaInt_AssetAlloc(PubKeyAssetPolicy, PubKeySize,
                                        &PubKeyAssetId);
        }
        else
        {
            /* Error already set */
        }
    }
    else
    {
        /* Error already set */
    }
    if (PSA_SUCCESS == funcres)
    {
        /* Generate the public key Asset */
        VexTokenCmd_PkAssetGenKey_t t_cmd;
        VexTokenRslt_Generic_t t_res;
        uint8_t PublicKeyBuffer[2U * (PSA_ASYM_DATA_VHEADER + (4U * ((521U + 31U) / 32U)))];

        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_PUBLIC_KEY;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_PK_WITHASSETS;
        t_cmd.Method = Method;
        t_cmd.ModulusSizeInBits = (uint32_t)ModulusSize;
        t_cmd.DivisorSizeInBits = (uint32_t)ModulusSize;
        t_cmd.PrivKeyAssetId = PrvKeyAssetId;
        t_cmd.PubKeyAssetId = PubKeyAssetId;
        t_cmd.DomainAssetId = DomainAssetId;
        t_cmd.KekAssetId = PSA_ASSETID_INVALID;
        t_cmd.AssociatedDataSize = 0U;
        t_cmd.KeyBlob_p = NULL;
        t_cmd.KeyBlobSize = 0U;
        t_cmd.PubKey_p = PublicKeyBuffer;
        t_cmd.PubKeySize = (uint32_t)PubKeySize;
        t_res.Result = 0;

        /* Exchange service request with the next driver level */
        funcres = psaInt_ExchangeToken((VexTokenCmd_Generic_t *)&t_cmd,
                                       sizeof(t_cmd),
                                       (VexTokenRslt_Generic_t *)&t_res,
                                       sizeof(t_res));
        if (PSA_SUCCESS == funcres)
        {
            if (0 > t_res.Result)
            {
#ifdef PSA_LOG_LOWLEVEL_ERROR
                LOG_WARN("Abort - %s()=%d\n", __func__, t_res.Result);
#endif
                funcres = PSA_ERROR_CORRUPTION_DETECTED;
            }
            else
            {
                uint8_t * pActualBegin = NULL;
                Asn1Der_BigNumber PubKey;

                /* Convert the public key to host DER format */
                PubKeySize = PSA_ASYM_DATA_SIZE_B2B(ModulusSize);
                psaInt_ReverseMemCpy(data, &PublicKeyBuffer[PSA_ASYM_DATA_VHEADER],
                                     PubKeySize);
                if (RealPubKeySize != PubKeySize)
                {
                    psaInt_ReverseMemCpy(&data[PubKeySize],
                                         &PublicKeyBuffer[(2U * PSA_ASYM_DATA_VHEADER) +
                                                          PSA_ASYM_DATA_SIZE_B2WB(ModulusSize)],
                                         PubKeySize);
                }
                else
                {
                    /* MISRA - Intentially empty */
                }
                (void)memcpy(PublicKeyBuffer, data, RealPubKeySize);

                PubKey.pData = PublicKeyBuffer;
                PubKey.Length = RealPubKeySize;
                PubKey.Sign = 0U;
                funcres = PSAInt_AsymWrite_EC_PubKey(&PubKey,
                                                     CurveFamily,
                                                     ModulusSize,
                                                     fECDH,
                                                     data_size, data,
                                                     &pActualBegin);
                if (PSA_SUCCESS == funcres)
                {
                    if (pActualBegin != data)
                    {
                        /* Move DER public key to the begin of the data buffer */
                        size_t Length = data_size - (size_t)(pActualBegin - data);
                        uint8_t * pPtr = data;

                        *data_length = Length;
                        for (; Length != 0U; Length--)
                        {
                            *pPtr = *pActualBegin;
                            pPtr++;
                            pActualBegin++;
                        }
                    }
                    else
                    {
                        /* Shall not directly happen due to the provided buffer */
                        *data_length = data_size;
                    }
                }
                else
                {
                    /* Error already set */
                }
            }
        }
        else
        {
            /* Error already set */
        }
    }
    else
    {
        /* Error already set */
    }

    /* Release the used Assets if applicable */
    if (PSA_ASSETID_INVALID != PubKeyAssetId)
    {
        (void)psaInt_AssetFree(PubKeyAssetId);
    }
    else
    {
        /* MISRA - Intentially empty */
    }
    if (PSA_ASSETID_INVALID != DomainAssetId)
    {
        (void)psaInt_AssetFree(DomainAssetId);
    }
    else
    {
        /* MISRA - Intentially empty */
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * local_GetFreeKeyEntry
 *
 * Searches for an empty key entry and returns its KeyId.
 * Note: The key entry is allocated for use.
 */
static psa_status_t
local_GetFreeKeyEntry(psa_key_id_t * pKeyId)
{
    psa_status_t funcres = PSA_SUCCESS;
    psa_key_id_t i = 1;

    for (i = 1; i <= PSA_MAX_KEY_BUFF_ENTRIES; i++)
    {
        if (0U == gl_PSA_Key[i].fAllocated)
        {
            break;
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }
    if (i > PSA_MAX_KEY_BUFF_ENTRIES)
    {
        funcres = PSA_ERROR_INSUFFICIENT_STORAGE;
        *pKeyId = PSA_KEY_ID_NULL;
    }
    else
    {
        gl_PSA_Key[i].fAllocated = 255U;
        *pKeyId = i;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * local_RemoveKey
 *
 * Removes/Releases the specified key entry and when fPersistent is set also
 * from the persistent key store.
 */
static psa_status_t
local_RemoveKey(psa_key_id_t key,
               bool fPersistent)
{
    psa_status_t funcres = PSA_SUCCESS;
    psa_key_context_t * pKey = &gl_PSA_Key[key];

    if (0U == pKey->KeyInUseCounter)
    {
        if (PSA_ASSETID_INVALID != pKey->key_assetId)
        {
            /* Remove the HSM Asset Store entry */
            (void)psaInt_AssetFree(pKey->key_assetId);
        }
        else
        {
            /* MISRA - Intentially empty */
        }
        if (NULL != pKey->key)
        {
            /* Remove the volatile entry */
            psaInt_KeyStore_VolatileFree(pKey->key);
        }
        else
        {
            /* MISRA - Intentially empty */
        }
        if ((true == fPersistent) &&
            (PSA_KEY_ID_NULL != pKey->attributes.id))
        {
            /* Remove the persistent entry, when not in-use to prevent issues */
            psa_key_id_t uid = pKey->attributes.id;
            uint32_t key_index;

            for (key_index = 0U; key_index <= PSA_MAX_KEY_BUFF_ENTRIES; key_index++)
            {
                if ((0U != gl_PSA_Key[key_index].fAllocated) &&
                    (gl_PSA_Key[key_index].attributes.id == uid) &&
                    (key_index != key))
                {
                    break;
                }
            }
            if (key_index > PSA_MAX_KEY_BUFF_ENTRIES)
            {
                /* The only one, thus the entry can be removed safely */
                funcres = psaInt_KeyStore_PersistentRelease(pKey->attributes.id);
            }
            else
            {
                /* MISRA - Intentially empty */
            }
        }
        else
        {
            /* MISRA - Intentially empty */
        }
        if (PSA_SUCCESS == funcres)
        {
            /* Make sure that the key entry is cleared */
            (void)memset(pKey, 0, sizeof(psa_key_context_t));
        }
        else
        {
            /* Error already set */
        }
    }
    else
    {
        /* The in-use counter is not zero */
        funcres = PSA_ERROR_INVALID_HANDLE;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * local_LoadImport
 *
 * Imports the Asset (Key) Blob (wrapped form) of the key into VaultIP/HSM.
 */
static psa_status_t
local_LoadImport(const PsaAssetId_t TargetAssetId,
                 const uint8_t * const KeyBlob_p,
                 const size_t KeyBlobSize_p)
{
    PsaAssetId_t KekAssetId;
    psa_status_t funcres;

    KekAssetId = psaInt_AssetGetKeyBlobKEK();
    if (KekAssetId != PSA_ASSETID_INVALID)
    {
        funcres = psaInt_AssetLoadImport(TargetAssetId, KekAssetId,
                                         KeyBlob_p, KeyBlobSize_p);
        (void)psaInt_AssetFree(KekAssetId);
    }
    else
    {
        funcres = PSA_ERROR_HARDWARE_FAILURE;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * local_LoadPlaintextExport
 *
 * Creates, loads and frees an Asset with the provided plaintext data and
 * returns the Asset (Key) Blob of the data (wrapped form).
 */
static psa_status_t
local_LoadPlaintextExport(PsaPolicyMask_t TargetAssetPolicy,
                          const uint8_t * data,
                          size_t data_length,
                          uint8_t * const KeyBlob_p,
                          size_t * const KeyBlobSize_p)
{
    PsaAssetId_t TargetAssetId = PSA_ASSETID_INVALID;
    PsaAssetId_t KekAssetId;
    psa_status_t funcres;

    KekAssetId = psaInt_AssetGetKeyBlobKEK();
    if (KekAssetId != PSA_ASSETID_INVALID)
    {
        funcres = psaInt_AssetAlloc(TargetAssetPolicy, data_length,
                                    &TargetAssetId);
        if (PSA_SUCCESS == funcres)
        {
            funcres = psaInt_AssetLoadPlaintextExport(TargetAssetId,
                                                      data, data_length,
                                                      KekAssetId,
                                                      KeyBlob_p, KeyBlobSize_p);
            (void)psaInt_AssetFree(TargetAssetId);
        }
        else
        {
            /* Error already set */
        }
        (void)psaInt_AssetFree(KekAssetId);
    }
    else
    {
        funcres = PSA_ERROR_HARDWARE_FAILURE;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * local_LoadRandomExport
 *
 * Creates, loads and frees an Asset with random data and returns the Asset
 * (Key) Blob of the generate data (wrapped form).
 */
static psa_status_t
local_LoadRandomExport(PsaPolicyMask_t TargetAssetPolicy,
                       size_t data_length,
                       uint8_t * const KeyBlob_p,
                       size_t * const KeyBlobSize_p)
{
    PsaAssetId_t TargetAssetId = PSA_ASSETID_INVALID;
    PsaAssetId_t KekAssetId;
    psa_status_t funcres;

    KekAssetId = psaInt_AssetGetKeyBlobKEK();
    if (KekAssetId != PSA_ASSETID_INVALID)
    {
        funcres = psaInt_AssetAlloc(TargetAssetPolicy, data_length,
                                    &TargetAssetId);
        if (PSA_SUCCESS == funcres)
        {
            funcres = psaInt_AssetLoadRandomExport(TargetAssetId,
                                                   KekAssetId,
                                                   KeyBlob_p, KeyBlobSize_p);
            (void)psaInt_AssetFree(TargetAssetId);
        }
        else
        {
            /* Error already set */
        }
        (void)psaInt_AssetFree(KekAssetId);
    }
    else
    {
        funcres = PSA_ERROR_HARDWARE_FAILURE;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_KeyMgmtGetKey
 *
 * Returns the key entry for the specified KeyId, when in use.
 */
psa_status_t
psaInt_KeyMgmtGetKey(psa_key_id_t key,
                     psa_key_context_t ** ppKeyEntry)
{
    psa_status_t funcres = PSA_SUCCESS;

    if ((PSA_KEY_ID_NULL == key) ||
        (key > PSA_MAX_KEY_BUFF_ENTRIES) ||
        (0U == gl_PSA_Key[key].fAllocated))
    {
        funcres = PSA_ERROR_INVALID_HANDLE;
    }
    else
    {
        *ppKeyEntry = &gl_PSA_Key[key];
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_KeyMgmtLoadKey
 *
 * Loads the key in VaultIP/HSM.
 */
psa_status_t
psaInt_KeyMgmtLoadKey(psa_key_context_t * pKey,
                      uint32_t * const pAssetId,
                      uint8_t * const pData,
                      size_t const DataSize,
                      uint32_t * const pActualDataSize)
{
    psa_status_t funcres = PSA_SUCCESS;
    uint8_t KeyDataBuffer[16+776];

    /* Note: when pKey is given it is expected to be correct */
    if ((NULL != pKey) &&
        (((NULL != pAssetId) && (NULL == pData) && (0U == DataSize) && (NULL == pActualDataSize)) ||
         ((NULL == pAssetId) && (NULL != pData) && (0U != DataSize) && (NULL != pActualDataSize))))
    {
        if (pKey->key_assetId != PSA_ASSETID_INVALID)
        {
            /* Key is already in the Asset Store */
            if (NULL != pAssetId)
            {
                *pAssetId = pKey->key_assetId;
            }
            else
            {
                funcres = PSA_ERROR_NOT_PERMITTED;

                /* Set default */
                (void)memset(pData, 0, DataSize);
                *pActualDataSize = 0U;
            }
        }
        else
        {
            /* Key is in the PSA key storage (either in RAM or NVM)  */
            psa_key_location_t key_location;

            key_location = PSA_KEY_LIFETIME_GET_LOCATION(pKey->attributes.lifetime);

            if (NULL != pAssetId)
            {
                *pAssetId = PSA_ASSETID_INVALID;

                funcres = psaInt_AssetAlloc(pKey->AssetPolicy, pKey->key_size,
                                            &pKey->key_assetId);
                if (PSA_SUCCESS == funcres)
                {
                    if (NULL != pKey->key)
                    {
                        /* Key is in RAM */
                        if (PSA_KEY_LOCATION_LOCAL_STORAGE == key_location)
                        {
                            /* Key data is available as plaintext */
                            funcres = psaInt_AssetLoadPlaintext(pKey->key_assetId,
                                                                pKey->key,
                                                                pKey->key_size);
                        }
                        else if (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location)
                        {
                            /* Key data is available as Asset blob (wrapped form) */
                            funcres = local_LoadImport(pKey->key_assetId,
                                                       pKey->key,
                                                       PSA_KEYBLOB_SIZE(pKey->key_size));
                        }
                        else
                        {
                            /* Safeguard: Should not happen, blocked by earlier tests */
                            funcres = PSA_ERROR_CORRUPTION_DETECTED;
                        }
                    }
                    else if (PSA_KEY_ID_NULL != pKey->attributes.id)
                    {
                        size_t KeyDataLen = 0U;

                        /* Key is in NVM */
                        if (PSA_KEY_LOCATION_LOCAL_STORAGE == key_location)
                        {
                            /* Key data is available as plaintext */
                            funcres = psaInt_KeyStore_PersistentRead(pKey->attributes.id,
                                                                     pKey->key_size,
                                                                     KeyDataBuffer,
                                                                     &KeyDataLen);
                            if (PSA_SUCCESS == funcres)
                            {
                                if (KeyDataLen == pKey->key_size)
                                {
                                    funcres = psaInt_AssetLoadPlaintext(pKey->key_assetId,
                                                                        KeyDataBuffer,
                                                                        KeyDataLen);
                                }
                                else
                                {
                                    /* Safeguard: Should not happen */
                                    funcres = PSA_ERROR_CORRUPTION_DETECTED;
                                }
                            }
                            else
                            {
                                /* Error already set */
                            }
                        }
                        else if (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location)
                        {
                            /* Key data is available as Asset blob (wrapped form) */
                            funcres = psaInt_KeyStore_PersistentRead(pKey->attributes.id,
                                                                     PSA_KEYBLOB_SIZE(pKey->key_size),
                                                                     KeyDataBuffer,
                                                                     &KeyDataLen);
                            if (PSA_SUCCESS == funcres)
                            {
                                if (KeyDataLen == PSA_KEYBLOB_SIZE(pKey->key_size))
                                {
                                    funcres = local_LoadImport(pKey->key_assetId,
                                                               KeyDataBuffer,
                                                               KeyDataLen);
                                }
                                else
                                {
                                    /* Safeguard: Should not happen */
                                    funcres = PSA_ERROR_CORRUPTION_DETECTED;
                                }
                            }
                            else
                            {
                                /* Error already set */
                            }
                        }
                        else
                        {
                            /* Safeguard: Should not happen, blocked by earlier tests */
                            funcres = PSA_ERROR_CORRUPTION_DETECTED;
                        }
                    }
                    else
                    {
                        /* Safeguard: Should not happen, blocked by earlier tests */
                        funcres = PSA_ERROR_CORRUPTION_DETECTED;
                    }

                    if (funcres != PSA_SUCCESS)
                    {
                        /* Cleanup the Asset */
                        (void)psaInt_AssetFree(pKey->key_assetId);
                        pKey->key_assetId = PSA_ASSETID_INVALID;
                    }
                    else
                    {
                        *pAssetId = pKey->key_assetId;
                    }
                }
                else
                {
                    /* The most likely error is that the Asset Store is full.
                     * The minor likely error is that the Asset Policy/size is wrong
                     */
                    funcres = PSA_ERROR_HARDWARE_FAILURE;

                    /* Make sure AssetId is invalid */
                    pKey->key_assetId = PSA_ASSETID_INVALID;
                }
            }
            else
            {
                /* Set default */
                (void)memset(pData, 0, DataSize);
                *pActualDataSize = 0U;

                if (NULL != pKey->key)
                {
                    /* Key is in RAM */
                    if (PSA_KEY_LOCATION_LOCAL_STORAGE == key_location)
                    {
                        /* Key data is available as plaintext */
                        if (DataSize >= pKey->key_size)
                        {
                            (void)memcpy(pData, pKey->key, pKey->key_size);
                            *pActualDataSize = (uint32_t)pKey->key_size;
                        }
                        else
                        {
                            funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                    }
                    else if (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location)
                    {
                        /* Key data is available as Asset blob (wrapped form) */
                        funcres = PSA_ERROR_NOT_PERMITTED;
                    }
                    else
                    {
                        /* Safeguard: Should not happen, blocked by earlier tests */
                        funcres = PSA_ERROR_CORRUPTION_DETECTED;
                    }
                }
                else if (PSA_KEY_ID_NULL != pKey->attributes.id)
                {
                    /* Key is in NVM */
                    if (PSA_KEY_LOCATION_LOCAL_STORAGE == key_location)
                    {
                        size_t KeyLen = 0U;

                        if (DataSize < pKey->key_size)
                        {
                            funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                        else
                        {
                            funcres = psaInt_KeyStore_PersistentRead(pKey->attributes.id,
                                                                     pKey->key_size,
                                                                     KeyDataBuffer,
                                                                     &KeyLen);
                        }
                        if (PSA_SUCCESS == funcres)
                        {
                            if (KeyLen == pKey->key_size)
                            {
                                (void)memcpy(pData, KeyDataBuffer, pKey->key_size);
                                *pActualDataSize = (uint32_t)pKey->key_size;
                            }
                            else
                            {
                                /* Safeguard: Should not happen */
                                funcres = PSA_ERROR_CORRUPTION_DETECTED;
                            }
                        }
                        else
                        {
                            /* Error already set */
                        }
                    }
                    else if (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location)
                    {
                        /* Key data is available as Asset blob (wrapped form) */
                        funcres = PSA_ERROR_NOT_PERMITTED;
                    }
                    else
                    {
                        /* Safeguard: Should not happen, blocked by earlier tests */
                        funcres = PSA_ERROR_CORRUPTION_DETECTED;
                    }
                }
                else
                {
                    /* Safeguard: Should not happen, blocked by earlier tests */
                    funcres = PSA_ERROR_CORRUPTION_DETECTED;
                }
            }
        }
    }
    else
    {
        /* Safeguard: Should not happen, blocked by psaInt_KeyMgmtGetKey() */
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_KeyMgmtReleaseKey
 *
 * Relaeses/Frees the key from VaultIP/HSM.
 */
void
psaInt_KeyMgmtReleaseKey(psa_key_context_t * pKey)
{
    if (NULL != pKey)
    {
        if (pKey->key_assetId != PSA_ASSETID_INVALID)
        {
            if ((NULL != pKey->key) ||
                (PSA_KEY_ID_NULL != pKey->attributes.id))
            {
                (void)psaInt_AssetFree(pKey->key_assetId);
                pKey->key_assetId = PSA_ASSETID_INVALID;
            }
            else
            {
                /* Key is intended to be in the Asset Store */
            }
        }
        else
        {
            /* Key is not in the Asset Store */
        }
    }
    else
    {
        /* Invalid key reference */
    }
}

/*----------------------------------------------------------------------------
 * psaInt_KeyMgmtSetKeyInUse
 *
 * Increment key in use counter.
 */
psa_status_t
psaInt_KeyMgmtSetKeyInUse(psa_key_id_t key)
{
    psa_status_t funcres = PSA_SUCCESS;

    if ((PSA_KEY_ID_NULL == key) ||
        (key > PSA_MAX_KEY_BUFF_ENTRIES) ||
        (0U == gl_PSA_Key[key].fAllocated))
    {
        /* Safeguard: Should not happen, blocked by psaInt_KeyMgmtGetKey() */
        funcres = PSA_ERROR_INVALID_HANDLE;
    }
    else
    {
        gl_PSA_Key[key].KeyInUseCounter++;
    }

    return funcres;
}

/*----------------------------------------------------------------------------
 * psaInt_KeyMgmtClrKeyInUse
 *
 * Decrement key in use counter.
 */
psa_status_t
psaInt_KeyMgmtClrKeyInUse(psa_key_id_t key)
{
    psa_status_t funcres = PSA_SUCCESS;

    if ((PSA_KEY_ID_NULL == key) ||
        (key > PSA_MAX_KEY_BUFF_ENTRIES) ||
        (0U == gl_PSA_Key[key].fAllocated))
    {
        /* Safeguard: Can only happen when called by an abort operation */
        funcres = PSA_ERROR_INVALID_HANDLE;
    }
    else
    {
        if (0U != gl_PSA_Key[key].KeyInUseCounter)
        {
            gl_PSA_Key[key].KeyInUseCounter--;
        }
        else
        {
            /* Safeguard: Should not happen */
            funcres = PSA_ERROR_CORRUPTION_DETECTED;
        }
    }

    return funcres;
}

/*----------------------------------------------------------------------------
 * psaInt_KeyMgmtReadECPubKey
 *
 * Reads/parses an DER format public key and returns the public key data in
 * VaultIP/HSM HW format.
 */
psa_status_t
psaInt_KeyMgmtReadECPubKey(const uint8_t * pData,
                           size_t DataLen,
                           psa_ecc_family_t ExpCurveFamily,
                           size_t ExpCurveBits,
                           uint8_t * pCurveFamily,
                           size_t * pCurveBits,
                           uint8_t * pfECDH,
                           uint8_t * pOutputData,
                           size_t * pOutputDataLength)
{
    Asn1Der_BigNumber PubKey;
    psa_status_t funcres;
    uint8_t items;

    funcres = PSAInt_AsymParse_EC_PubKey(pData, DataLen,
                                         pCurveFamily, pCurveBits,
                                         &PubKey, pfECDH);
    if (PSA_SUCCESS == funcres)
    {
        if (ExpCurveBits != *pCurveBits)
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        else
        {
            switch (*pCurveFamily)
            {
            case EIP130DOMAIN_ECC_FAMILY_NIST_P:
                if (PSA_ECC_FAMILY_SECP_R1 != ExpCurveFamily)
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    items = 0x2U;
                }
                break;
            case EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1:
                if (PSA_ECC_FAMILY_BRAINPOOL_P_R1 != ExpCurveFamily)
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    items = 0x12U;
                }
                break;
            case EIP130DOMAIN_ECC_FAMILY_MONTGOMERY:
                if (PSA_ECC_FAMILY_MONTGOMERY != ExpCurveFamily)
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    items = 0x01U;
                }
                break;
            case EIP130DOMAIN_ECC_FAMILY_TWISTED_EDWARDS:
                if (PSA_ECC_FAMILY_TWISTED_EDWARDS != ExpCurveFamily)
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    items = 0x11U;
                }
                break;
            default:
                funcres = PSA_ERROR_INVALID_ARGUMENT;
                break;
            }
        }
    }
    else
    {
        /* Error already set */
    }
    if (PSA_SUCCESS == funcres)
    {
        /* Convert to VaultIP HW format */
        if ((NULL == pOutputData) ||
            (NULL == pOutputDataLength) ||
            (*pOutputDataLength < ((items & 0x0FU) * PSA_ASYM_DATA_SIZE_VWB(*pCurveBits))))
        {
            /* Safeguard: Should not happen */
            funcres = PSA_ERROR_BUFFER_TOO_SMALL;
        }
        else
        {
            *pOutputDataLength = (items & 0x0FU) * PSA_ASYM_DATA_SIZE_VWB(*pCurveBits);
            if (0x01U == (items & 0x0FU))
            {
                local_AsymBigIntToHW(&PubKey, *pCurveBits, 0, items,
                                     pOutputData);
            }
            else
            {
                local_AsymBigIntPointToHW(&PubKey, *pCurveBits, items,
                                          pOutputData);
            }
        }
    }
    else
    {
        /* Error already set */
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_import_key
 *
 * create an Asset and load the key and return Asset ID.
 */
psa_status_t
psa_import_key(const psa_key_attributes_t * attributes,
               const uint8_t * data,
               size_t data_length,
               psa_key_id_t * key)
{
    psa_key_context_t * pKey = NULL;
    PsaPolicyMask_t AssetPolicy = (PsaPolicyMask_t)0U;
    psa_status_t funcres = PSA_SUCCESS;
    psa_key_persistence_t key_persistence;
    psa_key_location_t key_location;
    size_t modulus_size = 0U;
    size_t exponentsize = 0U;
    size_t DataSize = 0U;
    size_t DataToSaveSize = 0U;
    const uint8_t * pDataToSave = NULL;
    uint8_t dataBuffer[16+776];
    bool fClrPersistent = true;

    if ((NULL == key) ||
        (NULL == attributes) ||
        (PSA_KEY_TYPE_NONE == attributes->type) ||
        (PSA_ALG_NONE == attributes->alg))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
    {
        key_location = PSA_KEY_LIFETIME_GET_LOCATION(attributes->lifetime);
        key_persistence = PSA_KEY_LIFETIME_GET_PERSISTENCE(attributes->lifetime);

        if (PSA_KEY_ID_NULL != attributes->id)
        {
            if (PSA_KEY_PERSISTENCE_DEFAULT == key_persistence)
            {
                struct psa_storage_info_t info;

                funcres = psaInt_KeyStore_PersistentInfo(attributes->id, &info);
                if (PSA_SUCCESS == funcres)
                {
                    /* A persistent item exists in the PSA Key Store
                     * Note: The actual size check is performed later */
                    fClrPersistent = false;
                    if (PSA_KEY_LOCATION_LOCAL_STORAGE == key_location)
                    {
                        DataToSaveSize = info.size;
                    }
                    else
                    {
                        /* Correct to the actual data size */
                        DataToSaveSize = (info.size - (128U / 8U));
                    }
                }
                else if (PSA_ERROR_DOES_NOT_EXIST == funcres)
                {
                    /* Mark as OK for now */
                    funcres = PSA_SUCCESS;
                }
                else
                {
                    /* Another error, which is already set */
                }
            }
            else
            {
                /* When attributes->id is set, the key is assumed to be
                 * persistently stored in the PSA Key Store */
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
        }
        else
        {
            /* Intended to be stored either in the volatile PSA Key Store or
             * the HSM Asset Store */
        }
    }
    if (PSA_SUCCESS == funcres)
    {
        if (PSA_KEY_TYPE_IS_UNSTRUCTURED(attributes->type))
        {
            if (fClrPersistent)
            {
                /* New item */
                if (NULL == data)
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                else
                {
                    pDataToSave = data;
                    DataToSaveSize = data_length;
                }
            }
            else
            {
                /* Existing persistent item */
                if (0U != attributes->bits)
                {
                    DataSize = (size_t)PSA_BITS_TO_BYTES(attributes->bits);
                    if ((0U != data_length) && (data_length != DataSize))
                    {
                        /* Force invalid data length */
                        DataSize = 0U;
                    }
                    else
                    {
                        /* MISRA - Intentially empty */
                    }
                }
                else
                {
                    DataSize = data_length;
                }
                if (0U != DataSize)
                {
                    if ((PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location) ||
                        (PSA_KEY_LOCATION_LOCAL_STORAGE == key_location))
                    {
                        if (DataToSaveSize != DataSize)
                        {
                            funcres = PSA_ERROR_ALREADY_EXISTS;
                        }
                        else
                        {
                            /* Data length looks OK */
                        }
                    }
                    else
                    {
                        /* Not supported storage location */
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
                }
                else
                {
                    /* At least one length indication shall be available */
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
            }
            if (PSA_SUCCESS == funcres)
            {
                funcres = local_SymKeyType(attributes, DataToSaveSize, &AssetPolicy);
            }
            else
            {
                /* Error already set */
            }
        }
        else if (PSA_KEY_TYPE_IS_ASYMMETRIC(attributes->type))
        {
            DataSize = sizeof(dataBuffer);
            funcres = local_AsymKeyType(DataToSaveSize, attributes,
                                        data, data_length,
                                        dataBuffer, &DataSize,
                                        &modulus_size, &exponentsize,
                                        &AssetPolicy);
            pDataToSave = dataBuffer;
            if (fClrPersistent)
            {
                DataToSaveSize = DataSize;
            }
        }
        else
        {
            /* No vendor-defined key functionality */
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        if (PSA_SUCCESS == funcres)
        {
            funcres = local_GetFreeKeyEntry(key);
            if (PSA_SUCCESS == funcres)
            {
                pKey = &gl_PSA_Key[*key];
                if (PSA_KEY_LOCATION_LOCAL_STORAGE == key_location)
                {
                    if (PSA_KEY_PERSISTENCE_VOLATILE == key_persistence)
                    {
                        pKey->key = psaInt_KeyStore_VolatileAlloc(DataToSaveSize);
                        if (NULL == pKey->key)
                        {
                            funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                        else
                        {
                            (void)memcpy(pKey->key, pDataToSave, DataToSaveSize);
                        }
                    }
                    else if (PSA_KEY_PERSISTENCE_DEFAULT == key_persistence)
                    {
                        if (fClrPersistent)
                        {
                            /* New persistent item */
                            funcres = psaInt_KeyStore_PersistentWrite(attributes->id,
                                                                      DataToSaveSize,
                                                                      pDataToSave);
                        }
                        else if (PSA_KEY_USAGE_CACHE == (attributes->usage & PSA_KEY_USAGE_CACHE))
                        {
                            /* Read persistent item */
                            funcres = psaInt_KeyStore_PersistentRead(attributes->id,
                                                                     DataToSaveSize,
                                                                     dataBuffer,
                                                                     &DataSize);
                            if ((PSA_SUCCESS == funcres) &&
                                (DataSize != DataToSaveSize))
                            {
                                /* Safeguard: Should not happen */
                                funcres = PSA_ERROR_CORRUPTION_DETECTED;
                            }
                            else
                            {
                                pDataToSave = dataBuffer;
                            }
                        }
                        else
                        {
                            /* MISRA - Intentially empty */
                        }
                        if (PSA_SUCCESS == funcres)
                        {
                            if (PSA_KEY_USAGE_CACHE == (attributes->usage & PSA_KEY_USAGE_CACHE))
                            {
                                pKey->key = psaInt_KeyStore_VolatileAlloc(DataToSaveSize);
                                if (NULL != pKey->key)
                                {
                                    (void)memcpy(pKey->key, pDataToSave, DataToSaveSize);
                                }
                                else
                                {
                                    funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                                }
                            }
                            else
                            {
                                pKey->attributes.id = attributes->id;
                            }
                        }
                        else
                        {
                            /* Error already set */
                        }
                    }
                    else
                    {
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
                }
                else if (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location)
                {
                    DataSize = PSA_KEYBLOB_SIZE(DataToSaveSize);
                    if (PSA_KEY_PERSISTENCE_VOLATILE == key_persistence)
                    {
                        pKey->key = psaInt_KeyStore_VolatileAlloc(DataSize);
                        if (NULL == pKey->key)
                        {
                            funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                        else
                        {
                            funcres = local_LoadPlaintextExport(AssetPolicy,
                                                                pDataToSave,
                                                                DataToSaveSize,
                                                                pKey->key,
                                                                &DataSize);
                        }
                    }
                    else if (PSA_KEY_PERSISTENCE_DEFAULT == key_persistence)
                    {
                        if (fClrPersistent)
                        {
                            /* New persistent item */
                            if (DataSize > sizeof(dataBuffer))
                            {
                                funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                            }
                            else
                            {
                                funcres = local_LoadPlaintextExport(AssetPolicy,
                                                                    pDataToSave,
                                                                    DataToSaveSize,
                                                                    dataBuffer,
                                                                    &DataSize);
                            }
                            if (PSA_SUCCESS == funcres)
                            {
                                funcres = psaInt_KeyStore_PersistentWrite(attributes->id,
                                                                          DataSize,
                                                                          dataBuffer);
                            }
                            else
                            {
                                /* Error already set */
                            }
                        }
                        else if (PSA_KEY_USAGE_CACHE == (attributes->usage & PSA_KEY_USAGE_CACHE))
                        {
                            /* Read persistent item */
                            funcres = psaInt_KeyStore_PersistentRead(attributes->id,
                                                                     DataSize,
                                                                     dataBuffer,
                                                                     &DataSize);
                            if ((PSA_SUCCESS == funcres) &&
                                (DataSize != PSA_KEYBLOB_SIZE(DataToSaveSize)))
                            {
                                /* Safeguard: Should not happen */
                                funcres = PSA_ERROR_CORRUPTION_DETECTED;
                            }
                            else
                            {
                                /* MISRA - Intentially empty */
                            }
                        }
                        else
                        {
                            /* MISRA - Intentially empty */
                        }
                        if (PSA_SUCCESS == funcres)
                        {
                            if (PSA_KEY_USAGE_CACHE == (attributes->usage & PSA_KEY_USAGE_CACHE))
                            {
                                pKey->key = psaInt_KeyStore_VolatileAlloc(DataSize);
                                if (NULL != pKey->key)
                                {
                                    (void)memcpy(pKey->key, dataBuffer, DataSize);
                                }
                                else
                                {
                                    funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                                }
                            }
                            else
                            {
                                pKey->attributes.id = attributes->id;
                            }
                        }
                        else
                        {
                            /* Error already set */
                        }
                    }
                    else if (PSA_KEY_PERSISTENCE_HSM_ASSET_STORE == key_persistence)
                    {
                        funcres = psaInt_AssetAlloc(AssetPolicy, DataToSaveSize,
                                                    &pKey->key_assetId);
                        if (PSA_SUCCESS == funcres)
                        {
                            funcres = psaInt_AssetLoadPlaintext(pKey->key_assetId,
                                                                pDataToSave,
                                                                DataToSaveSize);
                        }
                        else
                        {
                            /* Error already set */
                        }
                    }
                    else
                    {
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
                }
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }

                if (PSA_SUCCESS != funcres)
                {
                    /* Do necessary cleanup */
                    (void)local_RemoveKey(*key, fClrPersistent);
                }
                else
                {
                    /* Initialize remaining part of the key entry */
                    pKey->attributes.type = attributes->type;
                    pKey->attributes.bits = attributes->bits;
                    pKey->attributes.lifetime = attributes->lifetime;
                    pKey->attributes.usage = attributes->usage;
                    pKey->attributes.alg = attributes->alg;
                    pKey->key_size = DataToSaveSize;
                    pKey->AssetPolicy = AssetPolicy;
                    pKey->modulus_size = modulus_size;
                    pKey->exponentsize = exponentsize;
                }
            }
            else
            {
                /* Error already set */
            }
        }
        else
        {
            /* Error already set */
        }
    }
    else
    {
        /* Error already set */
    }
    if ((PSA_SUCCESS != funcres) && (key != NULL))
    {
        /* Make sure the returned key is invalid */
        *key = PSA_KEY_ID_NULL;
    }
    else
    {
        /* MISRA - Intentially empty */
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_generate_key
 *
 * create asset, Generate random key and load it.
 */
psa_status_t
psa_generate_key(const psa_key_attributes_t * attributes,
                 psa_key_id_t * key)
{
    psa_status_t funcres = PSA_SUCCESS;
    psa_key_persistence_t KeyPersistence;
    psa_key_location_t KeyLocation;
    psa_key_context_t * pKey = NULL;
    PsaPolicyMask_t AssetPolicy = (PsaPolicyMask_t)0;
    size_t ModulusSize = 0U;
    size_t ExponentSize = 0U;
    size_t KeySize = 0U;
    uint8_t CurveFamily = 0U;
    uint8_t KeyDataBuffer[16+776];

    if ((NULL == key) ||
        (NULL == attributes) ||
        (PSA_KEY_TYPE_NONE == attributes->type) ||
        (0U == attributes->bits) ||
        (PSA_ALG_NONE == attributes->alg))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
    {
        if (PSA_KEY_TYPE_IS_UNSTRUCTURED(attributes->type))
        {
            /* Set the actual key size */
            KeySize = (size_t)PSA_BITS_TO_BYTES(attributes->bits);

            funcres = local_SymKeyType(attributes, KeySize, &AssetPolicy);
        }
        else if (PSA_KEY_TYPE_IS_ASYMMETRIC(attributes->type))
        {
            if (PSA_KEY_TYPE_IS_RSA(attributes->type))
            {
                funcres = PSA_ERROR_NOT_SUPPORTED;
            }
            else
            {
                uint8_t fECDH;

                funcres = local_AsymKeyGenType(attributes, &CurveFamily,
                                               &ModulusSize, &fECDH,
                                               &AssetPolicy);

                /* Set the actual key size */
                KeySize = PSA_ASYM_DATA_SIZE_VWB(ModulusSize);
            }
        }
        else
        {
            /* No vendor-defined key functionality */
            funcres = PSA_ERROR_NOT_SUPPORTED;
        }
        if (PSA_SUCCESS == funcres)
        {
            if (0U == KeySize)
            {
                /* Safeguard: Should not happen but safety first */
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
        }
        else
        {
            /* Error already set */
        }
        if (PSA_SUCCESS == funcres)
        {
            KeyPersistence = PSA_KEY_LIFETIME_GET_PERSISTENCE(attributes->lifetime);
            KeyLocation = PSA_KEY_LIFETIME_GET_LOCATION(attributes->lifetime);
            if (PSA_KEY_LOCATION_LOCAL_STORAGE == KeyLocation)
            {
                /* Key will be in plaintext form in the PSA key store */
                if (PSA_KEY_TYPE_IS_ASYMMETRIC(attributes->type))
                {
                    /* Asymmetric crypto
                     * => A private key is never available in the plaintext */
                    funcres = PSA_ERROR_NOT_SUPPORTED;
                }
                else if (PSA_KEY_PERSISTENCE_VOLATILE == KeyPersistence)
                {
                    /* Symmetric crypto for volatile storage
                     * => Generate a key */
                    funcres = PSA_ERROR_DOES_NOT_EXIST;
                }
                else if (PSA_KEY_PERSISTENCE_DEFAULT == KeyPersistence)
                {
                    /* Symmetric crypto for persistent storage
                     * => First check availability in persistent storage
                     * => If not exists, generate a key */
                    struct psa_storage_info_t info;

                    funcres = psaInt_KeyStore_PersistentInfo(attributes->id,
                                                             &info);
                    if (PSA_SUCCESS == funcres)
                    {
                        funcres = PSA_ERROR_ALREADY_EXISTS;
                    }
                    else
                    {
                        /* Error already set */
                    }
                }
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                if (PSA_ERROR_DOES_NOT_EXIST == funcres)
                {
                    /* Generate a random key */
                    if (sizeof(KeyDataBuffer) < KeySize)
                    {
                        funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                    }
                    else
                    {
                        funcres = psa_generate_random(KeyDataBuffer, KeySize);
                    }
                    if (PSA_SUCCESS == funcres)
                    {
                        funcres = local_GetFreeKeyEntry(key);
                    }
                    else
                    {
                        /* Error already set */
                    }
                    if (PSA_SUCCESS == funcres)
                    {
                        pKey = &gl_PSA_Key[*key];
                        if (PSA_KEY_PERSISTENCE_VOLATILE == KeyPersistence)
                        {
                            /* Store in volatile storage */
                            pKey->key = psaInt_KeyStore_VolatileAlloc(KeySize);
                            if (NULL == pKey->key)
                            {
                                funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                            }
                            else
                            {
                                (void)memcpy(pKey->key, KeyDataBuffer, KeySize);
                            }
                        }
                        else
                        {
                            /* Store in persistent storage */
                            funcres = psaInt_KeyStore_PersistentWrite(attributes->id,
                                                                      KeySize,
                                                                      KeyDataBuffer);
                            if (PSA_SUCCESS == funcres)
                            {
                                if (PSA_KEY_USAGE_CACHE == (attributes->usage & PSA_KEY_USAGE_CACHE))
                                {
                                    pKey->key = psaInt_KeyStore_VolatileAlloc(KeySize);
                                    if (NULL != pKey->key)
                                    {
                                        (void)memcpy(pKey->key, KeyDataBuffer, KeySize);
                                    }
                                    else
                                    {
                                        funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                                    }
                                }
                                else
                                {
                                    pKey->attributes.id = attributes->id;
                                }
                            }
                            else
                            {
                                /* Error already set */
                            }
                        }
                    }
                    else
                    {
                        /* Error already set */
                    }
                }
                else
                {
                    /* MISRA - Intentially empty */
                }
            }
            else if (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == KeyLocation)
            {
                /* Key will be in wrapped form (Asset Blob) form in the PSA key store */
                size_t KeyBlobSize = PSA_KEYBLOB_SIZE(KeySize);

                if (PSA_KEY_PERSISTENCE_DEFAULT == KeyPersistence)
                {
                    /* (A)Symmetric crypto for persistent storage
                     * => First check availability in persistent storage
                     * => If not exists, generate a key pair or key */
                    struct psa_storage_info_t info;

                    funcres = psaInt_KeyStore_PersistentInfo(attributes->id,
                                                             &info);
                    if (PSA_SUCCESS == funcres)
                    {
                        funcres = PSA_ERROR_ALREADY_EXISTS;
                    }
                    else
                    {
                        /* Error already set */
                    }
                }
                else if ((PSA_KEY_PERSISTENCE_VOLATILE == KeyPersistence) ||
                         (PSA_KEY_PERSISTENCE_HSM_ASSET_STORE == KeyPersistence))
                {
                    /* (A)Symmetric crypto for volatile storage or VaultIP
                     * => Generate a key pair or key */
                    funcres = PSA_ERROR_DOES_NOT_EXIST;
                }
                else
                {
                    funcres = PSA_ERROR_INVALID_ARGUMENT;
                }
                if (PSA_ERROR_DOES_NOT_EXIST == funcres)
                {
                    if (PSA_KEY_PERSISTENCE_HSM_ASSET_STORE == KeyPersistence)
                    {
                        funcres = local_GetFreeKeyEntry(key);
                        if (PSA_SUCCESS == funcres)
                        {
                            pKey = &gl_PSA_Key[*key];
                            if (PSA_KEY_TYPE_IS_ASYMMETRIC(attributes->type))
                            {
                                /* Generate ECDH/ECDSA/Curve25519/EdDSA key pair */
                                funcres = local_AsymKeyGenPrv(AssetPolicy,
                                                              CurveFamily,
                                                              ModulusSize,
                                                              &pKey->key_assetId,
                                                              NULL, NULL);
                            }
                            else
                            {
                                /* Generate a random key */
                                funcres = psaInt_AssetAlloc(AssetPolicy, KeySize,
                                                            &pKey->key_assetId);
                                if (PSA_SUCCESS == funcres)
                                {
                                    funcres = psaInt_AssetLoadRandom(pKey->key_assetId);
                                    if (PSA_SUCCESS != funcres)
                                    {
                                        (void)psaInt_AssetFree(pKey->key_assetId);
                                        pKey->key_assetId = PSA_ASSETID_INVALID;
                                    }
                                    else
                                    {
                                        /* MISRA - Intentially empty */
                                    }
                                }
                                else
                                {
                                    /* Error already set */
                                }
                            }
                        }
                        else
                        {
                            /* Error already set */
                        }
                    }
                    else
                    {
                        if (sizeof(KeyDataBuffer) < KeyBlobSize)
                        {
                            funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                        }
                        else if (PSA_KEY_TYPE_IS_ASYMMETRIC(attributes->type))
                        {
                            /* Generate ECDH/ECDSA/Curve25519/EdDSA key pair */
                            funcres = local_AsymKeyGenPrv(AssetPolicy,
                                                          CurveFamily,
                                                          ModulusSize,
                                                          NULL,
                                                          KeyDataBuffer,
                                                          &KeyBlobSize);
                        }
                        else
                        {
                            /* Generate a random key */
                            funcres = local_LoadRandomExport(AssetPolicy,
                                                             KeySize,
                                                             KeyDataBuffer,
                                                             &KeyBlobSize);
                        }
                        if (PSA_SUCCESS == funcres)
                        {
                            funcres = local_GetFreeKeyEntry(key);
                        }
                        else
                        {
                            /* Error already set */
                        }
                        if (PSA_SUCCESS == funcres)
                        {
                            pKey = &gl_PSA_Key[*key];
                            if (PSA_KEY_PERSISTENCE_VOLATILE == KeyPersistence)
                            {
                                /* Store in volatile storage */
                                pKey->key = psaInt_KeyStore_VolatileAlloc(KeyBlobSize);
                                if (NULL == pKey->key)
                                {
                                    funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                                }
                                else
                                {
                                    (void)memcpy(pKey->key,
                                                 KeyDataBuffer, KeyBlobSize);
                                }
                            }
                            else
                            {
                                /* Store in persistent storage */
                                funcres = psaInt_KeyStore_PersistentWrite(attributes->id,
                                                                          KeyBlobSize,
                                                                          KeyDataBuffer);
                                if (PSA_SUCCESS == funcres)
                                {
                                    if (PSA_KEY_USAGE_CACHE == (attributes->usage & PSA_KEY_USAGE_CACHE))
                                    {
                                        pKey->key = psaInt_KeyStore_VolatileAlloc(KeyBlobSize);
                                        if (NULL != pKey->key)
                                        {
                                            (void)memcpy(pKey->key, KeyDataBuffer, KeyBlobSize);
                                        }
                                        else
                                        {
                                            funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                                        }
                                    }
                                    else
                                    {
                                        pKey->attributes.id = attributes->id;
                                    }
                                }
                                else
                                {
                                    /* Error already set */
                                }
                            }
                        }
                        else
                        {
                            /* Error already set */
                        }
                    }
                }
            }
            else
            {
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
        }
        else
        {
            /* Error already set */
        }
    }
    if (PSA_SUCCESS == funcres)
    {
        if (NULL != pKey)
        {
            /* Initialize remaining part of the key entry */
            pKey->attributes.type = attributes->type;
            pKey->attributes.bits = attributes->bits;
            pKey->attributes.lifetime = attributes->lifetime;
            pKey->attributes.usage = attributes->usage;
            pKey->attributes.alg = attributes->alg;
            pKey->key_size = KeySize;
            pKey->AssetPolicy = AssetPolicy;
            pKey->modulus_size = ModulusSize;
            pKey->exponentsize = ExponentSize;
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }
    else
    {
        if (NULL != pKey)
        {
            /* Make sure the key entry is cleared */
            (void)local_RemoveKey(*key, true);
        }
        else
        {
            /* MISRA - Intentially empty */
        }

        /* Make sure the returned key is invalid */
        if (NULL != key)
        {
            *key = PSA_KEY_ID_NULL;
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_destroy_key
 *
 * Delete original key and its copies.
 */
psa_status_t
psa_destroy_key(psa_key_id_t key)
{
    psa_status_t funcres = PSA_SUCCESS;

    if ((PSA_KEY_ID_NULL == key) || (key > PSA_MAX_KEY_BUFF_ENTRIES))
    {
        funcres = PSA_ERROR_INVALID_HANDLE;
    }
    else
    {
        psa_key_id_t key_index;

        /* Check if original key has copies and delete them */
        for (key_index = 0U; key_index <= PSA_MAX_KEY_BUFF_ENTRIES; key_index++)
        {
            if ((0U != gl_PSA_Key[key_index].fAllocated) &&
                (key == gl_PSA_Key[key_index].source_key_id))
            {
                funcres = local_RemoveKey(key_index, true);
                if (PSA_SUCCESS != funcres)
                {
                    break;
                }
                else
                {
                    /* MISRA - Intentially empty */
                }
            }
            else
            {
                /* MISRA - Intentially empty */
            }
        }

        if (PSA_SUCCESS == funcres)
        {
            /* Delete the original key*/
            funcres = local_RemoveKey(key, true);
        }
        else
        {
            /* Error already set */
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_purge_key
 *
 * remove the asset refrenced by key.
 */
psa_status_t
psa_purge_key(psa_key_id_t key)
{
    psa_status_t funcres = PSA_SUCCESS;

    if ((PSA_KEY_ID_NULL == key) || (key > PSA_MAX_KEY_BUFF_ENTRIES))
    {
        funcres = PSA_ERROR_INVALID_HANDLE;
    }
    else
    {
        funcres = local_RemoveKey(key, false);
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_get_key_attributes
 *
 * Return key attributes of the requested key ID.
 */
psa_status_t
psa_get_key_attributes(psa_key_id_t key,
                       psa_key_attributes_t * attributes)
{
    psa_key_context_t * pKey;
    psa_status_t funcres;

    funcres = psaInt_KeyMgmtGetKey(key, &pKey);
    if (PSA_SUCCESS == funcres)
    {
        if (NULL == attributes)
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        else
        {
            attributes->id = pKey->attributes.id;
            attributes->bits = pKey->attributes.bits;
            attributes->type = pKey->attributes.type;
            attributes->lifetime = pKey->attributes.lifetime;
            attributes->alg = pKey->attributes.alg;
            attributes->usage = pKey->attributes.usage;
        }
    }
    else
    {
        /* Error already set */
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_reset_key_attributes
 *
 * Reset key attributes.
 */
void
psa_reset_key_attributes(psa_key_attributes_t * attributes)
{
    if (NULL != attributes)
    {
        attributes->type = PSA_KEY_TYPE_NONE;
        attributes->bits = 0U;
        attributes->lifetime = PSA_KEY_LIFETIME_VOLATILE;
        attributes->id = PSA_KEY_ID_NULL;
        attributes->usage = 0U;
        attributes->alg = PSA_ALG_NONE;
    }
}


/*----------------------------------------------------------------------------
 * psa_copy_key
 *
 * Copy a key.
 */
psa_status_t
psa_copy_key(psa_key_id_t source_key,
             const psa_key_attributes_t * attributes,
             psa_key_id_t * target_key)
{
    psa_key_context_t * pSrcKeyEntry;
    size_t CopyKeySize;
    psa_key_persistence_t key_persistence_src;
    psa_key_location_t key_location_src;
    psa_key_persistence_t key_persistence_trgt;
    psa_key_location_t key_location_trgt;
    psa_status_t funcres;
    uint8_t KeyDataBuffer[16+776];

    funcres = psaInt_KeyMgmtGetKey(source_key, &pSrcKeyEntry);
    if (PSA_SUCCESS == funcres)
    {
        /* get source key data */
        key_persistence_src = PSA_KEY_LIFETIME_GET_PERSISTENCE(pSrcKeyEntry->attributes.lifetime);
        key_location_src = PSA_KEY_LIFETIME_GET_LOCATION(pSrcKeyEntry->attributes.lifetime);

        if ((NULL == attributes) ||
            (NULL == target_key) ||
            ((0U != attributes->bits) &&
             (pSrcKeyEntry->key_size != (size_t)PSA_BITS_TO_BYTES(attributes->bits))) ||
            ((attributes->type != PSA_KEY_TYPE_NONE) &&
             (pSrcKeyEntry->attributes.type != attributes->type)))
        {
            funcres = PSA_ERROR_INVALID_ARGUMENT;
        }
        else if ((PSA_KEY_ID_NULL != pSrcKeyEntry->source_key_id) ||
                 (PSA_KEY_USAGE_COPY != (pSrcKeyEntry->attributes.usage & PSA_KEY_USAGE_COPY)) ||
                 ((PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location_src) &&
                  (PSA_KEY_PERSISTENCE_HSM_ASSET_STORE == key_persistence_src)))
        {
            funcres = PSA_ERROR_NOT_PERMITTED;
        }
        else
        {
            key_persistence_trgt = PSA_KEY_LIFETIME_GET_PERSISTENCE(attributes->lifetime);
            key_location_trgt = PSA_KEY_LIFETIME_GET_LOCATION(attributes->lifetime);

            funcres = local_GetFreeKeyEntry(target_key);
            if (PSA_SUCCESS == funcres)
            {
                psa_key_context_t * pTrgtKeyEntry = &gl_PSA_Key[*target_key];

                pTrgtKeyEntry->source_key_id = source_key;
                pTrgtKeyEntry->attributes.lifetime = attributes->lifetime;
                pTrgtKeyEntry->attributes.id = attributes->id;
                pTrgtKeyEntry->attributes.usage = attributes->usage;
                pTrgtKeyEntry->attributes.type = pSrcKeyEntry->attributes.type;
                pTrgtKeyEntry->attributes.bits = pSrcKeyEntry->attributes.bits;
                pTrgtKeyEntry->attributes.alg = pSrcKeyEntry->attributes.alg;
                pTrgtKeyEntry->key_size = pSrcKeyEntry->key_size;
                pTrgtKeyEntry->AssetPolicy = pSrcKeyEntry->AssetPolicy;
                pTrgtKeyEntry->modulus_size = pSrcKeyEntry->modulus_size;
                pTrgtKeyEntry->exponentsize = pSrcKeyEntry->exponentsize;

                /* First get the source key from right location */
                if (PSA_KEY_LOCATION_LOCAL_STORAGE == key_location_src)
                {
                    /* Source key is stored in plaintext format */
                    CopyKeySize = pSrcKeyEntry->key_size;
                }
                else if ((PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location_src) &&
                         (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location_trgt))
                {
                    /* Source key is stored in wrapped format */
                    CopyKeySize = PSA_KEYBLOB_SIZE(pSrcKeyEntry->key_size);
                }
                else
                {
                    funcres = PSA_ERROR_NOT_PERMITTED;
                }
                if (PSA_SUCCESS == funcres)
                {
                    if (PSA_KEY_PERSISTENCE_VOLATILE == key_persistence_src)
                    {
                        /* Source key is in volatile storage */
                        (void)memcpy(KeyDataBuffer, pSrcKeyEntry->key, CopyKeySize);
                    }
                    else if ((PSA_KEY_PERSISTENCE_DEFAULT == key_persistence_src) &&
                             (PSA_KEY_ID_NULL != pSrcKeyEntry->attributes.id))
                    {
                        /* Source key is in persistent storage */
                        size_t KeyDataLen;

                        funcres = psaInt_KeyStore_PersistentRead(pSrcKeyEntry->attributes.id,
                                                                 CopyKeySize,
                                                                 KeyDataBuffer,
                                                                 &KeyDataLen);
                        if ((PSA_SUCCESS == funcres) &&
                            (KeyDataLen != CopyKeySize))
                        {
                            /* Safeguard: Should not happen */
                            funcres = PSA_ERROR_CORRUPTION_DETECTED;
                        }
                        else
                        {
                            /* MISRA - Intentially empty */
                        }
                    }
                    else
                    {
                        /* Safeguard: Should not happen */
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
                }
                else
                {
                    /* Error already set */
                }
                if (PSA_SUCCESS == funcres)
                {
                    /* Check the location of the target key */
                    if (PSA_KEY_LOCATION_LOCAL_STORAGE == key_location_trgt)
                    {
                        if (PSA_KEY_PERSISTENCE_VOLATILE == key_persistence_trgt)
                        {
                            /* Store in volatile storage */
                            pTrgtKeyEntry->key = psaInt_KeyStore_VolatileAlloc(CopyKeySize);
                            if (NULL == pTrgtKeyEntry->key)
                            {
                                funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                            }
                            else
                            {
                                (void)memcpy(pTrgtKeyEntry->key, KeyDataBuffer, CopyKeySize);
                            }
                        }
                        else if ((PSA_KEY_PERSISTENCE_DEFAULT == key_persistence_trgt) &&
                                 (PSA_KEY_ID_NULL != pTrgtKeyEntry->attributes.id))
                        {
                            /* Store in persistent storage */
                            funcres = psaInt_KeyStore_PersistentWrite(pTrgtKeyEntry->attributes.id,
                                                                      CopyKeySize,
                                                                      KeyDataBuffer);
                        }
                        else
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                    }
                    else if (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location_trgt)
                    {
                        /* Target key goes to Secure Element store */
                        if (PSA_KEY_PERSISTENCE_VOLATILE == key_persistence_trgt)
                        {
                            if (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location_src)
                            {
                                /* Source key is already in wrapped format */
                                pTrgtKeyEntry->key = psaInt_KeyStore_VolatileAlloc(CopyKeySize);
                                if (NULL == pTrgtKeyEntry->key)
                                {
                                    funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                                }
                                else
                                {
                                    (void)memcpy(pTrgtKeyEntry->key,
                                                 KeyDataBuffer, CopyKeySize);
                                }
                            }
                            else
                            {
                                /* Source key is in plaintext format and must
                                 * be converted to wrapped format */
                                size_t KeyBlobSize = PSA_KEYBLOB_SIZE(CopyKeySize);

                                pTrgtKeyEntry->key = psaInt_KeyStore_VolatileAlloc(KeyBlobSize);
                                if (NULL == pTrgtKeyEntry->key)
                                {
                                    funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
                                }
                                else
                                {
                                    funcres = local_LoadPlaintextExport(pTrgtKeyEntry->AssetPolicy,
                                                                        KeyDataBuffer,
                                                                        CopyKeySize,
                                                                        pTrgtKeyEntry->key,
                                                                        &KeyBlobSize);
                                }
                            }
                        }
                        else if (PSA_KEY_PERSISTENCE_HSM_ASSET_STORE == key_persistence_trgt)
                        {
                            /* Target key goes to VaultIP */
                            funcres = psaInt_AssetAlloc(pTrgtKeyEntry->AssetPolicy,
                                                        pSrcKeyEntry->key_size,
                                                        &pTrgtKeyEntry->key_assetId);
                            if (PSA_SUCCESS == funcres)
                            {
                                if (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location_src)
                                {
                                    /* Source key is in wrapped format */
                                    funcres = local_LoadImport(pTrgtKeyEntry->key_assetId,
                                                               KeyDataBuffer,
                                                               PSA_KEYBLOB_SIZE(pSrcKeyEntry->key_size));
                                }
                                else
                                {
                                    /* Source key is in plaintext format */
                                    funcres = psaInt_AssetLoadPlaintext(pTrgtKeyEntry->key_assetId,
                                                                        KeyDataBuffer,
                                                                        pSrcKeyEntry->key_size);
                                }
                            }
                            else
                            {
                                /* Error already set */
                            }
                        }
                        else if ((PSA_KEY_PERSISTENCE_DEFAULT == key_persistence_trgt) &&
                                 (PSA_KEY_ID_NULL != pTrgtKeyEntry->attributes.id))
                        {
                            if (PSA_KEY_LOCATION_PRIMARY_SECURE_ELEMENT == key_location_src)
                            {
                                /* Source key is already in wrapped format */
                                funcres = psaInt_KeyStore_PersistentWrite(pTrgtKeyEntry->attributes.id,
                                                                          CopyKeySize,
                                                                          KeyDataBuffer);
                            }
                            else
                            {
                                /* Source key is in plaintext format and must
                                 * be converted to wrapped format */
                                size_t KeyBlobSize = PSA_KEYBLOB_SIZE(CopyKeySize);

                                funcres = local_LoadPlaintextExport(pTrgtKeyEntry->AssetPolicy,
                                                                    KeyDataBuffer,
                                                                    CopyKeySize,
                                                                    KeyDataBuffer,
                                                                    &KeyBlobSize);
                                if (PSA_SUCCESS == funcres)
                                {
                                    /* Store in persistent storage */
                                    funcres = psaInt_KeyStore_PersistentWrite(pTrgtKeyEntry->attributes.id,
                                                                              KeyBlobSize,
                                                                              KeyDataBuffer);
                                }
                                else
                                {
                                    /* Error already set */
                                }
                            }
                        }
                        else
                        {
                            funcres = PSA_ERROR_INVALID_ARGUMENT;
                        }
                    }
                    else
                    {
                        funcres = PSA_ERROR_NOT_PERMITTED;
                    }
                }
                else
                {
                    /* Error already set */
                }
                if (funcres != PSA_SUCCESS)
                {
                    /* Do necessary cleanup */
                    (void)local_RemoveKey(*target_key, true);
                }
                else
                {
                    /* MISRA - Intentially empty */
                }
            }
            else
            {
                /* Error already set */
            }
        }
    }
    else
    {
        /* Error return is already set */
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_export_key
 */
psa_status_t
psa_export_key(psa_key_id_t key,
               uint8_t * data,
               size_t data_size,
               size_t * data_length)
{
    /* VaultIP/EIP-130 is not able to export an Asset according the definition
     * for this function. Therefor, the function will only export a key that
     * is in PSA Key Store in plaintext. In all other case, the error
     * PSA_ERROR_NOT_SUPPORTED will be returned.
     */
    psa_key_context_t * pKey;
    psa_status_t funcres;

    funcres = psaInt_KeyMgmtGetKey(key, &pKey);
    if (PSA_SUCCESS != funcres)
    {
        /* Error return is already set */
    }
    else if ((NULL == data) ||
             (NULL == data_length))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else if (PSA_KEY_USAGE_EXPORT != (pKey->attributes.usage & PSA_KEY_USAGE_EXPORT))
    {
        funcres = PSA_ERROR_NOT_PERMITTED;
    }
    else if (data_size < (size_t)(PSA_EXPORT_KEY_OUTPUT_SIZE(pKey->attributes.type,
                                                             pKey->attributes.bits)))
    {
        funcres = PSA_ERROR_BUFFER_TOO_SMALL;
    }
    else
    {
        psa_key_location_t key_location;
        psa_key_persistence_t key_persistence;

        key_location = PSA_KEY_LIFETIME_GET_LOCATION(pKey->attributes.lifetime);
        key_persistence = PSA_KEY_LIFETIME_GET_PERSISTENCE(pKey->attributes.lifetime);
        if (PSA_KEY_LOCATION_LOCAL_STORAGE == key_location)
        {
            if (PSA_KEY_TYPE_IS_ASYMMETRIC(pKey->attributes.type))
            {
                funcres = PSA_ERROR_NOT_SUPPORTED;
            }
            else if (PSA_KEY_PERSISTENCE_VOLATILE == key_persistence)
            {
                *data_length = pKey->key_size;
                (void)memcpy(data, pKey->key, *data_length);
            }
            else if (PSA_KEY_PERSISTENCE_DEFAULT == key_persistence)
            {
                funcres = psaInt_KeyStore_PersistentRead(pKey->attributes.id,
                                                         pKey->key_size,
                                                         data, data_length);
                if ((PSA_SUCCESS != funcres) ||
                    (*data_length != pKey->key_size))
                {
                    /* Safeguard: Should not happen but safety first */
                    funcres = PSA_ERROR_CORRUPTION_DETECTED;
                    *data_length = 0U;
                    (void)memset(data, 0, data_size);
                }
                else
                {
                    /* MISRA - Intentially empty */
                }
            }
            else
            {
                /* Safeguard: Should never happen, already blocked by
                 *            psa_import_key() or psa_generate_key() */
                funcres = PSA_ERROR_INVALID_ARGUMENT;
            }
        }
        else
        {
            funcres = PSA_ERROR_NOT_SUPPORTED;
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psa_export_public_key
 */
psa_status_t
psa_export_public_key(psa_key_id_t key,
                      uint8_t * data,
                      size_t data_size,
                      size_t * data_length)
{
    psa_key_context_t *pKey = NULL;
    uint8_t CurveFamily = 0U;
    PsaAssetId_t PrvKeyAssetId = PSA_ASSETID_INVALID;
    psa_status_t funcres = PSA_SUCCESS;
    uint8_t fECDH = 0U;

    if ((NULL == data) ||
        (NULL == data_length) ||
        (data_size == 0U))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
    {
        funcres = psaInt_KeyMgmtGetKey(key, &pKey);
    }
    if (PSA_SUCCESS == funcres)
    {
        /* Key found
         * - Key must be an asymmetric ECC related key pair (private key) */
        if (PSA_KEY_TYPE_IS_ECC_KEY_PAIR(pKey->attributes.type))
        {
            psa_ecc_family_t Family;

            if (PSA_ALG_IS_KEY_AGREEMENT(pKey->attributes.alg))
            {
                fECDH = 255U;
            }
            else
            {
                /* MISRA - Intentially empty */
            }

            Family = PSA_KEY_TYPE_ECC_GET_FAMILY(pKey->attributes.type);
            if (PSA_ECC_FAMILY_SECP_R1 == Family)
            {
                CurveFamily = EIP130DOMAIN_ECC_FAMILY_NIST_P;
            }
            else if (PSA_ECC_FAMILY_BRAINPOOL_P_R1 == Family)
            {
               CurveFamily = EIP130DOMAIN_ECC_FAMILY_BRAINPOOL_R1;
            }
            else if ((PSA_ECC_FAMILY_MONTGOMERY == Family) &&
                     (fECDH != 0U))
            {
                CurveFamily = EIP130DOMAIN_ECC_FAMILY_MONTGOMERY;
            }
            else if ((PSA_ECC_FAMILY_TWISTED_EDWARDS == Family) &&
                     (fECDH == 0U))
            {
                CurveFamily = EIP130DOMAIN_ECC_FAMILY_TWISTED_EDWARDS;
            }
            else
            {
                /* Safeguard: Should never happen, checked by psa_import_key()
                 *            or psa_generate_key() */
                funcres = PSA_ERROR_NOT_SUPPORTED;
            }
        }
        else
        {
            funcres = PSA_ERROR_NOT_SUPPORTED;
        }
    }
    else
    {
        /* Error already set */
    }
    if (PSA_SUCCESS == funcres)
    {
        funcres = psaInt_KeyMgmtLoadKey(pKey, &PrvKeyAssetId, NULL, 0, NULL);
    }
    else
    {
        /* Error already set */
    }
    if (PSA_SUCCESS == funcres)
    {
        /* Note: The data_size is adapted to prevent out-of-bounds warnings
         *       that could be generated due to the way the public key write
         *       is constructed. */
        funcres = local_AsymKeyGenPub(PrvKeyAssetId, pKey->AssetPolicy,
                                      CurveFamily,
                                      (size_t)pKey->attributes.bits,
                                      fECDH,
                                      data, (data_size - 1U), data_length);
    }
    else
    {
        /* Error already set */
    }

    if (PSA_ASSETID_INVALID != PrvKeyAssetId)
    {
        psaInt_KeyMgmtReleaseKey(pKey);
    }
    else
    {
        /* MISRA - Intentially empty */
    }

    return funcres;
}


#ifdef PSA_ENABLE_DEBUG_FUNCTIONS
/*----------------------------------------------------------------------------
 * psaDebug_KeyStoreDump
 */
void
psaDebug_KeyStoreDump(void)
{
    psa_status_t funcres;
    uint32_t key_index;

    for (key_index = 0U; key_index <= PSA_MAX_KEY_BUFF_ENTRIES; key_index++)
    {
        if (0U != gl_PSA_Key[key_index].fAllocated)
        {
            const psa_key_context_t * const pKey = &gl_PSA_Key[key_index];

            LOG_CRIT("Key %u exists with a size of %u bytes (0x%" PRIx64 ")\n",
                     key_index, (uint32_t)pKey->key_size, pKey->AssetPolicy);
            if (0U != pKey->key_assetId)
            {
                LOG_CRIT("  ==> VaultIP Asset Store (0x%X)\n",
                         pKey->key_assetId);
            }
            else
            {
                /* MISRA - Intentially empty */
            }
            if (NULL != pKey->key)
            {
                LOG_CRIT("  ==> Volatile PSA Key Store\n");
            }
            else
            {
                /* MISRA - Intentially empty */
            }
            if (PSA_KEY_ID_NULL != pKey->attributes.id)
            {
                LOG_CRIT("  ==> Persistent PSA Key Store (%u)\n",
                         pKey->attributes.id);
            }
            else
            {
                /* MISRA - Intentially empty */
            }
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }

    for (key_index = 1U; key_index < PSA_MAX_KEY_BITS; key_index++)
    {
        struct psa_storage_info_t info;

        funcres = psaInt_KeyStore_PersistentInfo(key_index, &info);
        if (PSA_ERROR_INVALID_HANDLE == funcres)
        {
            /* All UID handled */
            break;
        }
        else
        {
            if (PSA_SUCCESS == funcres)
            {
                LOG_CRIT("Persistent Key %u exists with a size of %u bytes\n",
                         key_index, (uint32_t)info.size);
            }
            else
            {
                /* MISRA - Intentially empty */
            }
        }
    }
}
#endif


/* end of file adapter_psa_key_management.c */
