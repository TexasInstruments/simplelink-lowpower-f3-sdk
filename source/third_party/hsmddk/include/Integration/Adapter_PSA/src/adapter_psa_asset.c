/* adapter_psa_asset.c
 *
 * Implementation of the PSA API.
 *
 * This file implements the Asset Store services.
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
#include <third_party/hsmddk/include/Kit/DriverFramework/Basic_Defs_API/incl/basic_defs.h>                 // uint8_t, uint16_t, uint32_t, uint64_t
#include <third_party/hsmddk/include/Kit/DriverFramework/CLib_Abstraction_API/incl/clib.h>                       // size_t
#include <third_party/hsmddk/include/Kit/Log/incl/log.h>
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/psa/crypto.h>
#include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/adapter_vex.h>                // VexTokenCmd_*_t, VexTokenRslt_*_t
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_exchangetoken.h>
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_asset.h>          // the API to implement
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_system.h>
#ifdef PSA_LOG_LOWLEVEL_ERROR
#include <inttypes.h>
#endif


static const uint8_t gl_PSA_ADLabelKeyblob[] = PSA_AS_KEYBLOB_ADLABEL;


/*----------------------------------------------------------------------------
 * psaInt_AssetAlloc
 *
 * Allocate an Asset and set its policy. Its content is setup later.
 */
psa_status_t
psaInt_AssetAlloc(const PsaPolicyMask_t AssetPolicy,
                  const size_t AssetSize,
                  PsaAssetId_t * const AssetId_p)
{
    VexTokenCmd_AssetCreate_t t_cmd;
    VexTokenRslt_AssetCreate_t t_res;
    psa_status_t funcres;

#ifdef PSA_STRICT_ARGS
    if (((PsaPolicyMask_t)0U == AssetPolicy) ||
        (PSA_ASSET_SIZE_MAX < AssetSize) ||
        (NULL == AssetId_p))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
#endif
    {
        *AssetId_p = PSA_ASSETID_INVALID;

        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_ASSETMANAGEMENT;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_ASSETCREATE;
        t_cmd.PolicyLo = (uint32_t)(AssetPolicy);
        t_cmd.PolicyHi = (uint32_t)(AssetPolicy >> 32);
        t_cmd.Size = (uint32_t)AssetSize;
        t_res.Result = 0;
        t_res.AssetId = PSA_ASSETID_INVALID;

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
                LOG_WARN("Abort - %s(0x%" PRIx64 ", %u)=%d\n",
                         __func__, AssetPolicy, (uint32_t)AssetSize, t_res.Result);
#endif
                funcres = PSA_ERROR_CORRUPTION_DETECTED;
            }
            else
            {
                *AssetId_p = (PsaAssetId_t)t_res.AssetId;
            }
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_AssetFree
 *
 * Free the Asset referenced by AssetId.
 */
psa_status_t
psaInt_AssetFree(const PsaAssetId_t AssetId)
{
    VexTokenCmd_AssetDelete_t t_cmd;
    VexTokenRslt_Generic_t t_res;
    psa_status_t funcres;

#ifdef PSA_STRICT_ARGS
    if (PSA_ASSETID_INVALID == AssetId)
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
#endif
    {
        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_ASSETMANAGEMENT;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_ASSETDELETE;
        t_cmd.AssetId = (uint32_t)AssetId;
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
            /* MISRA - Intentially empty */
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_AssetLoadPlaintext
 *
 * Setup the content of the asset referenced by TargetAssetId from the
 * given plain data.
 */
psa_status_t
psaInt_AssetLoadPlaintext(const PsaAssetId_t TargetAssetId,
                          const uint8_t * Data_p,
                          const size_t DataSize)
{
    VexTokenCmd_AssetLoadPlaintext_t t_cmd;
    VexTokenRslt_Generic_t t_res;
    psa_status_t funcres;

#ifdef PSA_STRICT_ARGS
    if ((PSA_ASSETID_INVALID == TargetAssetId) ||
        (NULL == Data_p) ||
        (0U == DataSize) ||
        (PSA_ASSET_SIZE_MAX < DataSize))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
#endif
    {
        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_ASSETMANAGEMENT;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_ASSETLOAD;
        t_cmd.Method = (uint32_t)VEXTOKEN_ASSETLOAD_PLAINTEXT;
        t_cmd.AssetId = (uint32_t)TargetAssetId;
        t_cmd.Data_p = (const uint8_t *)Data_p;
        t_cmd.DataSize = (uint32_t)DataSize;
        t_cmd.KekAssetId = PSA_ASSETID_INVALID;
        t_cmd.AssociatedDataSize = 0U;
        t_cmd.KeyBlob_p = NULL;
        t_cmd.KeyBlobSize = 0U;
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
            /* MISRA - Intentially empty */
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_AssetLoadPlaintextExport
 *
 * Setup the content of the asset referenced by TargetAssetId from the
 * given plain data and export the resulting asset as a key blob.
 * Use the given KEK and AD to create the key blob.
 */
psa_status_t
psaInt_AssetLoadPlaintextExport(const PsaAssetId_t TargetAssetId,
                                const uint8_t * const Data_p,
                                const size_t DataSize,
                                const PsaAssetId_t KekAssetId,
                                uint8_t * const KeyBlob_p,
                                size_t * const KeyBlobSize_p)
{
    VexTokenCmd_AssetLoadPlaintext_t t_cmd;
    VexTokenRslt_AssetLoad_t t_res;
    psa_status_t funcres;

    COMPILE_STATIC_ASSERT((sizeof(gl_PSA_ADLabelKeyblob) - 1U) <= sizeof(t_cmd.AssociatedData));

#ifdef PSA_STRICT_ARGS
    if ((DataSize > PSA_ASSET_SIZE_MAX) ||
        (KeyBlob_p == NULL) ||
        (KeyBlobSize_p == NULL))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
#endif
    {
        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_ASSETMANAGEMENT;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_ASSETLOAD;
        t_cmd.Method = (uint32_t)VEXTOKEN_ASSETLOAD_PLAINTEXT;
        t_cmd.AssetId = (uint32_t)TargetAssetId;
        t_cmd.Data_p = (const uint8_t *)Data_p;
        t_cmd.DataSize = (uint32_t)DataSize;
        t_cmd.KekAssetId = (uint32_t)KekAssetId;
        t_cmd.AssociatedDataSize = (uint32_t)(sizeof(gl_PSA_ADLabelKeyblob) - 1U);
        (void)memcpy(t_cmd.AssociatedData, gl_PSA_ADLabelKeyblob, t_cmd.AssociatedDataSize);
        t_cmd.KeyBlob_p = (uint8_t *)KeyBlob_p;
#ifndef PSA_STRICT_ARGS
        if (KeyBlobSize_p != NULL)
#endif
        {
            t_cmd.KeyBlobSize = (uint32_t)*KeyBlobSize_p;
        }
#ifndef PSA_STRICT_ARGS
        else
        {
            t_cmd.KeyBlobSize = 0U;
        }
#endif
        t_res.Result = 0;
        t_res.DataBlobSize = 0U;

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
                if (0U == t_res.DataBlobSize)
                {
                    LOG_CRIT("Abort - NO Asset Blob\n");
                    funcres = PSA_ERROR_CORRUPTION_DETECTED;
                }
                else
                {
#ifndef PSA_STRICT_ARGS
                    if (NULL != KeyBlobSize_p)
#endif
                    {
                        *KeyBlobSize_p = t_res.DataBlobSize;
                    }
#ifndef PSA_STRICT_ARGS
                    else
                    {
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
#endif
                }
            }
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_AssetLoadImport
 *
 * Setup the content of the asset referenced by TargetAssetId from the
 * given key blob. Use the given KEK and AD to unwrap the key blob.
 */
psa_status_t
psaInt_AssetLoadImport(const PsaAssetId_t TargetAssetId,
                       const PsaAssetId_t KekAssetId,
                       const uint8_t * const KeyBlob_p,
                       const size_t KeyBlobSize)
{
    VexTokenCmd_AssetLoadImport_t t_cmd;
    VexTokenRslt_Generic_t t_res;
    psa_status_t funcres;

    COMPILE_STATIC_ASSERT((sizeof(gl_PSA_ADLabelKeyblob) - 1U) <= sizeof(t_cmd.AssociatedData));

#ifdef PSA_STRICT_ARGS
    if ((KeyBlobSize > PSA_KEYBLOB_SIZE(PSA_ASSET_SIZE_MAX)) ||
        (KeyBlob_p == NULL) || (KeyBlobSize == 0U))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
#endif
    {
        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_ASSETMANAGEMENT;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_ASSETLOAD;
        t_cmd.Method = (uint32_t)VEXTOKEN_ASSETLOAD_IMPORT;
        t_cmd.AssetId = (uint32_t)TargetAssetId;
        t_cmd.KekAssetId = (uint32_t)KekAssetId;
        t_cmd.AssociatedDataSize = (uint32_t)(sizeof(gl_PSA_ADLabelKeyblob) - 1U);
        (void)memcpy(t_cmd.AssociatedData, gl_PSA_ADLabelKeyblob, t_cmd.AssociatedDataSize);
        t_cmd.KeyBlob_p = (const uint8_t *)KeyBlob_p;
        t_cmd.KeyBlobSize = (uint32_t)KeyBlobSize;
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
            /* MISRA - Intentially empty */
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_AssetLoadRandom
 *
 * Setup the content of the asset referenced by TargetAssetId with random
 * data obtained from the RNG.
 */
psa_status_t
psaInt_AssetLoadRandom(const PsaAssetId_t TargetAssetId)
{
    VexTokenCmd_AssetLoadRandom_t t_cmd;
    VexTokenRslt_Generic_t t_res;
    psa_status_t funcres;

#ifdef PSA_STRICT_ARGS
    if (PSA_ASSETID_INVALID == TargetAssetId)
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
#endif
    {
        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_ASSETMANAGEMENT;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_ASSETLOAD;
        t_cmd.Method = (uint32_t)VEXTOKEN_ASSETLOAD_RANDOM;
        t_cmd.AssetId = (uint32_t)TargetAssetId;
        t_cmd.KekAssetId = PSA_ASSETID_INVALID;
        t_cmd.AssociatedDataSize = 0U;
        t_cmd.KeyBlob_p = NULL;
        t_cmd.KeyBlobSize = 0U;
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
            /* MISRA - Intentially empty */
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_AssetLoadRandomExport
 *
 * Setup the content of the asset referenced by TargetAssetId with random
 * data obtained from the RNG and export the resulting asset as a key blob.
 * Use the given KEK and AD to create the key blob.
 */
psa_status_t
psaInt_AssetLoadRandomExport(const PsaAssetId_t TargetAssetId,
                             const PsaAssetId_t KekAssetId,
                             uint8_t * const KeyBlob_p,
                             size_t * const KeyBlobSize_p)
{
    VexTokenCmd_AssetLoadRandom_t t_cmd;
    VexTokenRslt_AssetLoad_t t_res;
    psa_status_t funcres;

    COMPILE_STATIC_ASSERT((sizeof(gl_PSA_ADLabelKeyblob) - 1U) <= sizeof(t_cmd.AssociatedData));

#ifdef PSA_STRICT_ARGS
    if ((KeyBlob_p == NULL) || (KeyBlobSize_p == NULL))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
#endif
    {
        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_ASSETMANAGEMENT;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_ASSETLOAD;
        t_cmd.Method = (uint32_t)VEXTOKEN_ASSETLOAD_RANDOM;
        t_cmd.AssetId = (uint32_t)TargetAssetId;
        t_cmd.KekAssetId = (uint32_t)KekAssetId;
        t_cmd.AssociatedDataSize = (uint32_t)(sizeof(gl_PSA_ADLabelKeyblob) - 1U);
        (void)memcpy(t_cmd.AssociatedData, gl_PSA_ADLabelKeyblob, t_cmd.AssociatedDataSize);
        t_cmd.KeyBlob_p = (uint8_t *)KeyBlob_p;
#ifndef PSA_STRICT_ARGS
        if (KeyBlobSize_p != NULL)
#endif
        {
            t_cmd.KeyBlobSize = (uint32_t)*KeyBlobSize_p;
        }
#ifndef PSA_STRICT_ARGS
        else
        {
            /* Safeguard for when strict args checking is not enabled */
            t_cmd.KeyBlobSize = 0U;
        }
#endif
        t_res.Result = 0;
        t_res.DataBlobSize = 0U;

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
                if (0U == t_res.DataBlobSize)
                {
                    LOG_CRIT("Abort - NO Asset Blob\n");
                    funcres = PSA_ERROR_CORRUPTION_DETECTED;
                }
                else
                {
#ifndef PSA_STRICT_ARGS
                    if (NULL != KeyBlobSize_p)
#endif
                    {
                        *KeyBlobSize_p = t_res.DataBlobSize;
                    }
#ifndef PSA_STRICT_ARGS
                    else
                    {
                        funcres = PSA_ERROR_INVALID_ARGUMENT;
                    }
#endif
                }
            }
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_AssetLoadDerive
 *
 * Setup the content of the asset referenced by TargetAssetId by deriving it
 * from the given KDK and label info.
 */
psa_status_t
psaInt_AssetLoadDerive(const PsaAssetId_t TargetAssetId,
                       const PsaAssetId_t KdkAssetId,
                       const uint8_t * const AssociatedData_p,
                       const size_t AssociatedDataSize,
                       const bool fCounter,
                       const bool fRFC5869,
                       const uint8_t * const Salt_p,
                       const size_t SaltSize,
                       const uint8_t * const IV_p,
                       const size_t IVSize,
                       const uint8_t AssetNumber)
{
    VexTokenCmd_AssetLoadDerive_t t_cmd;
    VexTokenRslt_Generic_t t_res;
    psa_status_t funcres;

#ifdef PSA_STRICT_ARGS
    if ((PSA_ASSETID_INVALID == TargetAssetId) ||
        (PSA_ASSETID_INVALID == KdkAssetId) ||
        (NULL == AssociatedData_p) ||
        (0U == AssociatedDataSize) ||
        (fCounter && fRFC5869))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else if (AssociatedDataSize > sizeof(t_cmd.AssociatedData))
    {
        funcres = PSA_ERROR_INSUFFICIENT_MEMORY;
    }
    else
#endif
    {
        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_ASSETMANAGEMENT;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_ASSETLOAD;
        t_cmd.Method = (uint32_t)VEXTOKEN_ASSETLOAD_DERIVE;
        t_cmd.AssetId = (uint32_t)TargetAssetId;
        t_cmd.KdkAssetId = (uint32_t)KdkAssetId;
        (void)memcpy(t_cmd.AssociatedData, AssociatedData_p, AssociatedDataSize);
        t_cmd.AssociatedDataSize = (uint32_t)AssociatedDataSize;
        t_cmd.Salt_p = (const uint8_t *)Salt_p;
        t_cmd.SaltSize = (uint32_t)SaltSize;
        t_cmd.IV_p = (const uint8_t *)IV_p;
        t_cmd.IVSize = (uint32_t)IVSize;
        t_cmd.AssetNumber = AssetNumber;
        if (fCounter)
        {
            t_cmd.fCounter = 1U;
        }
        else
        {
            t_cmd.fCounter = 0U;
        }
        if (fRFC5869)
        {
            t_cmd.fRFC5869 = 1U;
        }
        else
        {
            t_cmd.fRFC5869 = 0U;
        }
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
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_PublicDataRead
 *
 * Read a public data object.
 */
psa_status_t
psaInt_PublicDataRead(const PsaAssetId_t AssetId,
                      uint8_t * const Data_p,
                      const size_t DataSize,
                      size_t * output_length)
{
    VexTokenCmd_PublicData_t t_cmd;
    VexTokenRslt_PublicData_t t_res;
    psa_status_t funcres;

#ifdef PSA_STRICT_ARGS
    if ((PSA_ASSETID_INVALID == AssetId) ||
        (NULL == Data_p) ||
        (0U == DataSize) ||
        (NULL == output_length))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
#endif
    {
        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_ASSETMANAGEMENT;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_PUBLICDATA;
        t_cmd.AssetId = (uint32_t)AssetId;
        t_cmd.Data_p = (const uint8_t *)Data_p;
        t_cmd.DataSize = (uint32_t)DataSize;
        t_res.Result = 0;
        t_res.DataSize = 0U;

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
                /* Zeroize remaining part of the buffer */
                uint32_t i = t_res.DataSize;
                *output_length = (size_t)t_res.DataSize;
                for (; i < DataSize; i++)
                {
                    Data_p[i] = 0U;
                }
            }
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_AssetSearch
 *
 * Get the AssetId for the provided StaticAssetNumber.
 */
psa_status_t
psaInt_AssetSearch(const uint16_t StaticAssetNumber,
                   PsaAssetId_t * const AssetId_p,
                   size_t * const AssetSize_p)
{
    VexTokenCmd_AssetSearch_t t_cmd;
    VexTokenRslt_AssetSearch_t t_res;
    psa_status_t funcres;

#ifdef PSA_STRICT_ARGS
    if ((NULL == AssetId_p) ||
        (StaticAssetNumber > (PSA_ASSET_NUMBER_MAX + PSA_ASSET_NUMBER_CONSTANTS)))
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
#endif
    {
        *AssetId_p = PSA_ASSETID_INVALID;

        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_ASSETMANAGEMENT;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_ASSETSEARCH;
        t_cmd.Number = StaticAssetNumber;
        t_res.Result = 0;
        t_res.AssetId = PSA_ASSETID_INVALID;
        t_res.Size = 0U;

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
                funcres = PSA_ERROR_DOES_NOT_EXIST;
            }
            else
            {
                *AssetId_p = (PsaAssetId_t)t_res.AssetId;
                if (AssetSize_p != NULL)
                {
                    *AssetSize_p = (size_t)t_res.Size;
                }
                else
                {
                    /* MISRA - Intentially empty */
                }
            }
        }
    }

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_AssetGetKeyBlobKEK
 *
 * Get the AssetId of the Asset Blob KEK.
 * This means that the root key must be available and the KEK Asset creation
 * and initialization (derivation) must successful.
 */
PsaAssetId_t
psaInt_AssetGetKeyBlobKEK(void)
{
    static const uint8_t ADLabelDerive[] = PSA_AS_DERIVE_ADLABEL;
    PsaAssetId_t RootKeyAssetId = PSA_ASSETID_INVALID;
    PsaAssetId_t KekAssetId = PSA_ASSETID_INVALID;
    psa_status_t funcres;

    (void)psaInt_AssetSearch(PSA_ASSETNUMBER_HUK, &RootKeyAssetId, NULL);
    if (RootKeyAssetId != PSA_ASSETID_INVALID)
    {
        PsaPolicyMask_t KekAssetPolicy = EIP130_ASSET_POLICY_SYM_WRAP |
                                         EIP130_ASSET_POLICY_SCAWAESSIV |
                                         EIP130_ASSET_POLICY_SCDIRENCDEC;
        if (!psaInt_IsAccessSecure())
        {
            KekAssetPolicy |= PSA_POLICY_SOURCE_NON_SECURE;
        }
        else
        {
            /* MISRA - Intentially empty */
        }
        funcres = psaInt_AssetAlloc(KekAssetPolicy, 64, &KekAssetId);
        if (PSA_SUCCESS == funcres)
        {
            funcres = psaInt_AssetLoadDerive(KekAssetId, RootKeyAssetId,
                                             ADLabelDerive,
                                             (sizeof(ADLabelDerive) - 1U),
                                             false, false, NULL, 0, NULL, 0,
                                             PSA_AS_DERIVE_MC_ASSETNUMBER);
            if (PSA_SUCCESS != funcres)
            {
                LOG_CRIT("Failed: Asset Blob KEK initialization\n");
                (void)psaInt_AssetFree(KekAssetId);
                KekAssetId = PSA_ASSETID_INVALID;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
        }
        else
        {
            LOG_CRIT("Failed: Asset Blob KEK creation\n");
        }
    }
    else
    {
        LOG_CRIT("Failed: Get Root key reference\n");
    }

    return KekAssetId;
}


/*----------------------------------------------------------------------------
 * psaInt_AssetGetKeyBlobLabel
 *
 * Get the Associated Data (label) for the Asset Blob.
 */
void
psaInt_AssetGetKeyBlobLabel(uint8_t *pData,
                            uint32_t * pDataSize)
{
    *pDataSize = (uint32_t)(sizeof(gl_PSA_ADLabelKeyblob) - 1U);
    (void)memcpy(pData, gl_PSA_ADLabelKeyblob, *pDataSize);
}


/*----------------------------------------------------------------------------
 * psaInt_AsymEccInstallCurve
 *
 * Get curve family and curvebits and create asset and load it with the selected
 * family and return Asset ID for the asset.
 */
psa_status_t
psaInt_AsymEccInstallCurve(const uint8_t CurveFamily,
                           const size_t CurveBits,
                           PsaAssetId_t * const AssetId_p)
{
    psa_status_t funcres = PSA_SUCCESS;
    VexTokenCmd_PkAssetInstallCurve_t t_cmd;
    VexTokenRslt_AssetCreate_t t_res;

    if (NULL == AssetId_p)
    {
        funcres = PSA_ERROR_INVALID_ARGUMENT;
    }
    else
    {
        /* Format service request */
        t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_PUBLIC_KEY;
        t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_PK_WITHASSETS;
        t_cmd.Method = (uint32_t)VEXTOKEN_PKAS_INSTALL_ECCURVE;
        t_cmd.CurveFamily = (uint32_t)CurveFamily;
        t_cmd.CurveBits = (uint32_t)CurveBits;
        t_res.Result = 0;
        t_res.AssetId = 0U;

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
                funcres = PSA_ERROR_CORRUPTION_DETECTED;
#endif
            }
            else
            {
                *AssetId_p = (PsaAssetId_t)t_res.AssetId;
            }
        }
        else
        {
            /* MISRA - Intentially empty */
        }
    }

    return funcres;
}


/* end of file adapter_psa_asset.c */
