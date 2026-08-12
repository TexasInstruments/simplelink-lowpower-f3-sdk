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

static enum
{
    LRFD_NOT_CLOCKED          = 0,
    LRFD_CLOCKED              = 1,
    LRFD_UNKNOWN_CLOCK_STATUS = 2,
} lrfdClockStatus = LRFD_UNKNOWN_CLOCK_STATUS;

static uint16_t lrfdClockDependencySets[LRFD_NUM_CLK_DEP];

#ifdef DRIVERLIB_NS
// For NS driverlib, there is no API to change the pointer, so it is made const
// to help the compiler optimize the code.
static uint16_t (*const lrfdClockDependencySetsPtr)[LRFD_NUM_CLK_DEP] = &lrfdClockDependencySets;
#else
static uint16_t (*lrfdClockDependencySetsPtr)[LRFD_NUM_CLK_DEP] = &lrfdClockDependencySets;
#endif

#ifndef DRIVERLIB_NS
//*****************************************************************************
//
// Secure only function used by the NS LRFD driverlib module to inform the
// S LRFD driverlib module of where to read/write the clock dependencies
// from/to.
//
//*****************************************************************************
void LRFDSetDependencySetsPtr(uint16_t (*dependencySetsPtr)[LRFD_NUM_CLK_DEP])
{
    lrfdClockDependencySetsPtr = dependencySetsPtr;

    // Copy dependencies to the new pointer
    for (uint8_t i = 0; i < LRFD_NUM_CLK_DEP; i++)
    {
        (*lrfdClockDependencySetsPtr)[i] |= lrfdClockDependencySets[i];
    }
}
#else
extern void LRFDSetDependencySetsPtr_veneer(uint16_t (*dependencySetsPtr)[LRFD_NUM_CLK_DEP]);
    #define LRFDSetDependencySetsPtr LRFDSetDependencySetsPtr_veneer
#endif

//*****************************************************************************
//
// Set clock dependency on a set of modules in LRFD.
//
//*****************************************************************************
void LRFDSetClockDependency(uint16_t mask, uint8_t dependencySetId)
{
    if (dependencySetId < LRFD_NUM_CLK_DEP)
    {
        (*lrfdClockDependencySetsPtr)[dependencySetId] |= mask;
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
        (*lrfdClockDependencySetsPtr)[dependencySetId] &= ~mask;
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

#ifdef DRIVERLIB_NS
    // The NS side owns the LRFD clock dependencies. The NS side needs to inform
    // the S side of where the dependencies are stored.
    static bool secureSideInitialized = false;
    if (secureSideInitialized == false)
    {
        // Initialize the secure side
        LRFDSetDependencySetsPtr(&lrfdClockDependencySets);
        secureSideInitialized = true;
    }
#endif

    // Merge the clock dependencies for all sets
    for (int i = 0; i < LRFD_NUM_CLK_DEP; i++)
    {
        clkctl |= (*lrfdClockDependencySetsPtr)[i];
    }

    if (clkctl == 0)
    {
        // No dependencies set, so LRFD should not be clocked
        if (lrfdClockStatus != LRFD_NOT_CLOCKED)
        {
            // Disable LRFD module clock. Only do so if it is not already
            // disabled. This is to prevent unnecessarily having to call into
            // the secure side from the non-secure side in TFM enabled
            // applications.
            CLKCTLDisableLrfdClock();
            lrfdClockStatus = LRFD_NOT_CLOCKED;
        }
    }
    else
    {
        // At least one dependency is set, so LRFD should be clocked
        if (lrfdClockStatus != LRFD_CLOCKED)
        {
            // Enable LRFD module clock. Only do so if it is not already
            // enabled. This is to prevent unnecessarily having to call into the
            // secure side from the non-secure side in TFM enabled applications.
            CLKCTLEnableLrfdClock();
            lrfdClockStatus = LRFD_CLOCKED;
        }

        // BRIDGE bit should not be needed as hardware will automatically
        // enable the clock when needed. The bit should always be 0 in the HW,
        // and is thus cleared here. The BRIDGE bit can be set as a dependency,
        // making clkctl non-zero, to indicate the need for the LRFD module
        // clock to be enabled, but no internal LRFD clocks.
        HWREG(LRFDDBELL_BASE + LRFDDBELL_O_CLKCTL) = clkctl & ~LRFDDBELL_CLKCTL_BRIDGE_M;
    }
}