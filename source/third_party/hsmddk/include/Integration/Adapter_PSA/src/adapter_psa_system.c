/* adapter_psa_system.c
 *
 * Implementation of the PSA API.
 *
 * This file implements the system services.
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
#include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/adapter_vex.h>                // VexTokenCmd_*_t, VexTokenRslt_*_t
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_exchangetoken.h>
#include <third_party/hsmddk/include/Integration/Adapter_PSA/incl/adapter_psa_system.h>


/*----------------------------------------------------------------------------
 * psaInt_SystemGetState
 */
psa_status_t
psaInt_SystemGetState(uint8_t * const OtpErrorCode_p,
                      uint16_t * const OtpErrorLocation_p,
                      uint8_t * const Mode_p,
                      uint8_t * const SelfTestError_p,
                      uint8_t * const CryptoOfficer_p,
                      uint8_t * const HostID_p,
                      uint8_t * const NonSecure_p,
                      uint32_t * const Identity_p)
{
    VexTokenCmd_Generic_t t_cmd;
    VexTokenRslt_SystemInfo_t t_res;
    psa_status_t funcres;

    /* Format service request */
    t_cmd.OpCode = (uint32_t)VEXTOKEN_OPCODE_SYSTEM;
    t_cmd.SubCode = (uint32_t)VEXTOKEN_SUBCODE_SYSTEMINFO;
    (void)memset(&t_res, 0, sizeof(t_res));

    /* Exchange service request with the next driver level */
    funcres = psaInt_ExchangeToken(&t_cmd, sizeof(t_cmd),
                                   (VexTokenRslt_Generic_t *)&t_res,
                                   sizeof(t_res));
    if (PSA_SUCCESS == funcres)
    {
        if (0 > t_res.Result)
        {
#ifdef PSA_LOG_LOWLEVEL_ERROR
            LOG_WARN("Abort - %s()=%d\n", __func__, t_res.Result);
#endif
            funcres = PSA_ERROR_HARDWARE_FAILURE;
        }
        else
        {
            /* return requested information */
            if (NULL != OtpErrorCode_p)
            {
                *OtpErrorCode_p = t_res.OTP.ErrorCode;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
            if (NULL != OtpErrorLocation_p)
            {
                *OtpErrorLocation_p = t_res.OTP.ErrorLocation;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
            if (NULL != Mode_p)
            {
                *Mode_p = t_res.Self.Mode;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
            if (NULL != SelfTestError_p)
            {
                *SelfTestError_p = t_res.Self.ErrorTest;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
            if (NULL != CryptoOfficer_p)
            {
                *CryptoOfficer_p = t_res.Self.CryptoOfficer;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
            if (NULL != HostID_p)
            {
                *HostID_p = t_res.Self.HostID;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
            if (NULL != NonSecure_p)
            {
                *NonSecure_p = t_res.Self.NonSecure;
            }
            else
            {
                /* MISRA - Intentially empty */
            }
            if (NULL != Identity_p)
            {
                *Identity_p = t_res.Self.Identity;
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

    return funcres;
}


/*----------------------------------------------------------------------------
 * psaInt_IsAccessSecure
 */
bool
psaInt_IsAccessSecure(void)
{
    static int32_t gl_PsaAccessType = 0;
    bool Secure;

    if ((0x55AA55 != gl_PsaAccessType) && (0x882288 != gl_PsaAccessType))
    {
        psa_status_t funcres;
        uint8_t NonSecure = 0U;

        funcres = psaInt_SystemGetState(NULL, NULL, NULL, NULL, NULL, NULL,
                                        &NonSecure, NULL);
        if (PSA_SUCCESS == funcres)
        {
            if (0U != NonSecure)
            {
                /* Set Non-secure indication */
                gl_PsaAccessType = 0x55AA55;
            }
            else
            {
                /* Set secure indication */
                gl_PsaAccessType = 0x882288;
            }
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

    if (0x882288 == gl_PsaAccessType)
    {
        /* Secure */
        Secure = true;
    }
    else
    {
        /* Non-secure */
        Secure = false;
    }

    return Secure;
}


/* end of file adapter_psa_system.c */
