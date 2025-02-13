/* adapter_vex_sym_keywrap.c
 *
 * Implementation of the VEX API.
 *
 * This file implements the symmetric crypto key wrap services.
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

#include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/c_adapter_vex.h>          // configuration

#include <third_party/hsmddk/include/Kit/DriverFramework/Basic_Defs_API/incl/basic_defs.h>
#include <third_party/hsmddk/include/Kit/DriverFramework/CLib_Abstraction_API/incl/clib.h>

#include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/adapter_vex_internal.h>   // API implementation
#include <third_party/hsmddk/include/Integration/Adapter_Generic/incl/adapter_bufmanager.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_wrap.h>


/*----------------------------------------------------------------------------
 * vex_SymKeyWrap
 */
VexStatus_t
vex_SymKeyWrap(
        const VexTokenCmd_KeyWrap_t * const CommandToken_p,
        VexTokenRslt_KeyWrap_t * const ResultToken_p)
{
    VexStatus_t funcres = VEX_NO_MEMORY;
    uint16_t TokenID;
    uint64_t SrcDataAddr = 0U;
    uint64_t DstDataAddr = 0U;
    bool fCopy = false;
    Eip130Token_Command_t CommandToken;
    Eip130Token_Result_t ResultToken;

    // Convert input buffer and get address for token
    SrcDataAddr = BufManager_Map(vex_ConvertFromUint32ToBool(CommandToken_p->fFromUserSpace),
                                 BUFMANAGER_BUFFERTYPE_IN,
                                 CommandToken_p->SrcData_p,
                                 CommandToken_p->SrcDataSize,
                                 NULL);
    if (SrcDataAddr != 0U)
    {
        // Get output address for token
        TokenID = vex_DeviceGetTokenID();
        DstDataAddr = BufManager_Map(vex_ConvertFromUint32ToBool(CommandToken_p->fFromUserSpace),
                                     BUFMANAGER_BUFFERTYPE_OUT,
                                     CommandToken_p->DstData_p,
                                     CommandToken_p->DstDataSize,
                                     (void *)&TokenID);
        if (DstDataAddr != 0U)
        {
            // Format command token
            (void)memset(&CommandToken, 0, sizeof(CommandToken));
            Eip130Token_Command_WrapUnwrap(
                &CommandToken,
                vex_ConvertFromUint32ToBool((uint32_t)CommandToken_p->fWrap),
                SrcDataAddr, (uint16_t)CommandToken_p->SrcDataSize,
                DstDataAddr);
            Eip130Token_Command_WrapUnwrap_SetAlgorithm(
                &CommandToken,
                CommandToken_p->Algorithm);
            Eip130Token_Command_WrapUnwrap_SetKeyLength(
                &CommandToken,
                CommandToken_p->KeySize);
            if (CommandToken_p->KeyAssetId != 0U)
            {
                Eip130Token_Command_WrapUnwrap_SetASLoadKey(
                    &CommandToken,
                    CommandToken_p->KeyAssetId);
            }
            else
            {
                Eip130Token_Command_WrapUnwrap_CopyKey(
                    &CommandToken,
                    CommandToken_p->Key,
                    CommandToken_p->KeySize);
            }
#ifdef VEX_CHECK_DMA_WITH_TOKEN_ID
            Eip130Token_Command_SetTokenID(&CommandToken, TokenID, true);
#else
            Eip130Token_Command_SetTokenID(&CommandToken, TokenID, false);
#endif

            // Initialize result token
            (void)memset(&ResultToken, 0, sizeof(ResultToken));

            // Exchange token with the EIP-130 HW
            funcres = vex_PhysicalTokenExchange(&CommandToken, &ResultToken);
            if (funcres == VEX_SUCCESS)
            {
                ResultToken_p->Result = Eip130Token_Result_Code(&ResultToken);
                if (ResultToken_p->Result >= 0)
                {
                    // Copy output data
                    fCopy = true;

                    Eip130Token_Result_WrapUnwrap_GetDataLength(
                        &ResultToken,
                        &ResultToken_p->Size);
                }
            }
        }
    }

    // Release used buffers, if needed
    if (SrcDataAddr != 0U)
    {
        (void)BufManager_Unmap(SrcDataAddr, false, false, 0U);
    }
    if (DstDataAddr != 0U)
    {
        if (BufManager_Unmap(DstDataAddr, true, fCopy, 0U) != 0)
        {
            funcres = VEX_DATA_MAPPING_ERROR;
        }
    }

    return funcres;
}


/* end of file adapter_vex_sym_keywrap.c */
