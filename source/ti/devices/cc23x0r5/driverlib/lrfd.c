/******************************************************************************
 *  Filename:       lrfd.c
 *
 *  Description:    Driver for LRFD
 *
 *  Copyright (c) 2023-2025 Texas Instruments Incorporated
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

#include "lrfd.h"

#include <stdbool.h>
#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_clkctl.h"
#include "../inc/hw_lrfddbell.h"

#include "../driverlib/interrupt.h"
#include "../driverlib/clkctl.h"

static uint16_t lrfdClockDependencySets[LRFD_NUM_CLK_DEP];

//*****************************************************************************
//
// Set clock dependency on a set of modules in LRFD.
//
//*****************************************************************************
void LRFDSetClockDependency(uint16_t mask, uint8_t dependencySetId)
{
    if (dependencySetId < LRFD_NUM_CLK_DEP)
    {
        lrfdClockDependencySets[dependencySetId] |= mask;
    }
    LRFDApplyClockDependencies();
}

//*****************************************************************************
//
// Release clock dependency on a set of modules in LRFD.
//
//*****************************************************************************
void LRFDReleaseClockDependency(uint16_t mask, uint8_t dependencySetId)
{
    if (dependencySetId < LRFD_NUM_CLK_DEP)
    {
        lrfdClockDependencySets[dependencySetId] &= ~mask;
    }
    LRFDApplyClockDependencies();
}

//*****************************************************************************
//
// Apply the currently set clock dependencies.
//
//*****************************************************************************
void LRFDApplyClockDependencies(void)
{
    // The combined clock dependencies for all dependency sets. It represents
    // the value to be written to the LRFDDBELL_O_CLKCTL register.
    uint16_t clkctl = 0;

    bool lrfdClocked = (HWREG(CLKCTL_BASE + CLKCTL_O_CLKCFG0) & CLKCTL_CLKCFG0_LRFD_M) == CLKCTL_CLKCFG0_LRFD_CLK_EN;

    // Merge the clock dependencies for all sets
    for (int i = 0; i < LRFD_NUM_CLK_DEP; i++)
    {
        clkctl |= lrfdClockDependencySets[i];
    }

    if (lrfdClocked)
    {
        // BRIDGE bit should not be needed, as hardware will automatically
        // enable the clock when needed. The bit should be always be 0 in the
        // HW, and is thus cleared here.
        HWREG(LRFDDBELL_BASE + LRFDDBELL_O_CLKCTL) = clkctl & ~LRFDDBELL_CLKCTL_BRIDGE_M;

        if (clkctl == 0)
        {
            // Disable LRFD module clock
            CLKCTLDisableLrfdClock();
        }
    }
    else
    {
        if (clkctl != 0)
        {
            // Enable LRFD module clock
            CLKCTLEnableLrfdClock();

            // BRIDGE bit should not be needed as hardware will automatically
            // enable the clock when needed. The bit should always be 0 in the
            // HW, and is thus cleared here. The BRIDGE bit can be set as a
            // dependency, making clkctl non-zero, to indicate the need for the
            // LRFD module clock to be enabled, but no internal LRFD clocks.
            HWREG(LRFDDBELL_BASE + LRFDDBELL_O_CLKCTL) = clkctl & ~LRFDDBELL_CLKCTL_BRIDGE_M;
        }
    }
}