/******************************************************************************
 *  Filename:       clkctl.h
 *
 *  Description:    Defines and prototypes for the CLKCTL module.
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

#ifndef __CLKCTL_H__
#define __CLKCTL_H__

//*****************************************************************************
//
//! \addtogroup system_control_group
//! @{
//! \addtogroup clkctl_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_clkctl.h"

//*****************************************************************************
//
//! \brief Enable clock for LRFD
//!
//! \warning This API is only meant to be used by the LRFD driverlib module and
//! should not be used directly.
//
//*****************************************************************************
__STATIC_INLINE void CLKCTLEnableLrfdClock(void)
{
    // Enable LRFD module clock
    HWREG( CLKCTL_BASE + CLKCTL_O_CLKENSET0 ) = CLKCTL_CLKENSET0_LRFD;

    // Wait for LRFD clock to be enabled. It is not expected that the LRFD clock
    // will ever be disabled, but this will add sufficient delay before
    // potentially using LRFD directly after this function.
    while ((HWREG(CLKCTL_BASE + CLKCTL_O_CLKCFG0) & CLKCTL_CLKCFG0_LRFD_M) != CLKCTL_CLKCFG0_LRFD_CLK_EN) {}
}

//*****************************************************************************
//
//! \brief Disable clock for LRFD
//!
//! \note This API is only meant to be used by the LRFD driverlib module and
//! should not be used directly.
//
//*****************************************************************************
__STATIC_INLINE void CLKCTLDisableLrfdClock(void)
{
    HWREG( CLKCTL_BASE + CLKCTL_O_CLKENCLR0 ) = CLKCTL_CLKENCLR0_LRFD;
}

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
//! Close the Doxygen group.
//! @}
//! @}
//
//*****************************************************************************

#endif // __CLKCTL_H__
