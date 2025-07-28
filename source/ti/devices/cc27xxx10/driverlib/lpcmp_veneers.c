/******************************************************************************
 *  Filename:       lpcmp_veneers.c
 *
 *  Description:    Veneers for accessing LPCMP API from non-secure context.
 *
 *  Copyright (c) 2025 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1) Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2) Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  3) Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#include "lpcmp.h"

#include <third_party/tfm/secure_fw/include/security_defs.h> // __tz_c_veneer

__tz_c_veneer void LPCMPEnable_veneer(void)
{
    LPCMPEnable();
}

__tz_c_veneer void LPCMPDisable_veneer(void)
{
    LPCMPDisable();
}

__tz_c_veneer void LPCMPEnableEvent_veneer(void)
{
    LPCMPEnableEvent();
}

__tz_c_veneer void LPCMPDisableEvent_veneer(void)
{
    LPCMPDisableEvent();
}

__tz_c_veneer void LPCMPClearEvent_veneer(void)
{
    LPCMPClearEvent();
}

__tz_c_veneer void LPCMPEnableWakeup_veneer(void)
{
    LPCMPEnableWakeup();
}

__tz_c_veneer void LPCMPDisableWakeup_veneer(void)
{
    LPCMPDisableWakeup();
}

__tz_c_veneer void LPCMPSelectNegativeInput_veneer(uint32_t input)
{
    LPCMPSelectNegativeInput(input);
}

__tz_c_veneer bool LPCMPIsOutputHigh_veneer(void)
{
    return LPCMPIsOutputHigh();
}

__tz_c_veneer void LPCMPSelectPositiveInput_veneer(uint32_t input)
{
    LPCMPSelectPositiveInput(input);
}

__tz_c_veneer void LPCMPSetPolarity_veneer(uint32_t polarity)
{
    LPCMPSetPolarity(polarity);
}

__tz_c_veneer void LPCMPSetDividerRatio_veneer(uint32_t divider)
{
    LPCMPSetDividerRatio(divider);
}

__tz_c_veneer void LPCMPSetDividerPath_veneer(uint32_t path)
{
    LPCMPSetDividerPath(path);
}