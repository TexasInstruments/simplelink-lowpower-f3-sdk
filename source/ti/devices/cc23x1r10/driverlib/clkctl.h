/******************************************************************************
 *  Filename:       clkctl.h
 *
 *  Description:    Defines and prototypes for the CLKCTL module.
 *
 *  Copyright (c) 2025-2026 Texas Instruments Incorporated
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

#ifndef ti_devices_clkctl__include
#define ti_devices_clkctl__include

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

#include "../inc/hw_ints.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_types.h"
#include "../inc/hw_clkctl.h"
#include "debug.h"
#include "interrupt.h"

//! \cond NODOC
//! Mask for field used to store the bit index in the CLKCTL_* macros
#define CLKCTL_BIT_INDEX_M      (0x1FU)
//! Mask for field used to store the register index in the CLKCTL_* macros
#define CLKCTL_REGISTER_INDEX_M (0xE0U)
//! Shift value of the field used to store the register index in the
//! CLKCTL_* macros
#define CLKCTL_REGISTER_INDEX_S (5U)
//! \endcond

//! \name Clocks that can be enabled/disabled by CLKCTL
//!
//! These macros define the clocks that can be enabled/disabled using the
//! \ref CLKCTLEnable() and \ref CLKCTLDisable(). The exception is
//! \ref CLKCTL_MAX_VALUE, which is used to indicate the maximum possible
//! value of all the other clocks.
//!
//! The values of the defines are chosen such that \ref CLKCTL_MAX_VALUE is
//! minimized while still providing a simple interface with the hardware.
//!
//! \note Implementation note that should not be relied on by the user of these
//! macros: The values are the bit indicies for the CLKCTL.CLKCFGn registers.
//! The 5 least significant bits are used to store the bit index of the register
//! and the remaining 3 most significant bits in the byte are used to store
//! the register index. This allows for a maximum of 8 registers, each with 32
//! bits.
//! \{
//! \hideinitializer LGPT3 Clock
#define CLKCTL_LGPT3     ((CLKCTL_CLKENSET0_LGPT3_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer LGPT2 Clock
#define CLKCTL_LGPT2     ((CLKCTL_CLKENSET0_LGPT2_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer LGPT1 Clock
#define CLKCTL_LGPT1     ((CLKCTL_CLKENSET0_LGPT1_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer LGPT0 Clock
#define CLKCTL_LGPT0     ((CLKCTL_CLKENSET0_LGPT0_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer I2S Clock
#define CLKCTL_I2S       ((CLKCTL_CLKENSET0_I2S_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer DMA Clock
#define CLKCTL_DMA       ((CLKCTL_CLKENSET0_DMA_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer LAES Clock
#define CLKCTL_LAES      ((CLKCTL_CLKENSET0_LAES_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer ADC0 Clock
#define CLKCTL_ADC0      ((CLKCTL_CLKENSET0_ADC0_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer SPI0 Clock
#define CLKCTL_SPI0      ((CLKCTL_CLKENSET0_SPI0_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer SPI1 Clock
#define CLKCTL_SPI1      ((CLKCTL_CLKENSET0_SPI1_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer I2C0 Clock
#define CLKCTL_I2C0      ((CLKCTL_CLKENSET0_I2C0_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer I2C1 Clock
#define CLKCTL_I2C1      ((CLKCTL_CLKENSET0_I2C1_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer UART0 Clock
#define CLKCTL_UART0     ((CLKCTL_CLKENSET0_UART0_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer UART1 Clock
#define CLKCTL_UART1     ((CLKCTL_CLKENSET0_UART1_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer LRFD Clock
#define CLKCTL_LRFD      ((CLKCTL_CLKENSET0_LRFD_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer GPIO Clock
#define CLKCTL_GPIO      ((CLKCTL_CLKENSET0_GPIO_S) | (0U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer HSM Clock
#define CLKCTL_HSM       ((CLKCTL_CLKENSET1_HSM_S) | (1U << CLKCTL_REGISTER_INDEX_S))
//! \hideinitializer Max possible value of any of the CLKCTL_* defines
#define CLKCTL_MAX_VALUE (CLKCTL_HSM)
//! \}

#if CLKCTL_MAX_VALUE > 0xFF
    #error CLKCTL APIs and implementations assume CLKCTL_MAX_VALUE fit \
    within one byte.
#endif

//*****************************************************************************
//
//! \brief Enables the clock for a peripheral.
//!
//! This function enables the clock for a peripheral.
//!
//! \param peripheral specifies the peripheral.
//! The parameter must be one of the following values:
//! - \ref CLKCTL_LGPT3
//! - \ref CLKCTL_LGPT2
//! - \ref CLKCTL_LGPT1
//! - \ref CLKCTL_LGPT0
//! - \ref CLKCTL_I2S
//! - \ref CLKCTL_DMA
//! - \ref CLKCTL_LAES
//! - \ref CLKCTL_ADC0
//! - \ref CLKCTL_SPI0
//! - \ref CLKCTL_SPI1
//! - \ref CLKCTL_I2C0
//! - \ref CLKCTL_I2C1
//! - \ref CLKCTL_UART0
//! - \ref CLKCTL_UART1
//! - \ref CLKCTL_LRFD
//! - \ref CLKCTL_GPIO
//! - \ref CLKCTL_HSM
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CLKCTLEnable(uint32_t peripheral)
{
    // Check the arguments
    ASSERT(peripheral <= CLKCTL_MAX_VALUE);

    uint32_t registerIndex                                      = peripheral >> CLKCTL_REGISTER_INDEX_S;
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET0 + registerIndex * 4) = (1U << (peripheral & CLKCTL_BIT_INDEX_M));
}

//*****************************************************************************
//
//! \brief Disables the clock for a peripheral.
//!
//! This function disables the clock for a peripheral.
//!
//! \param peripheral specifies the peripheral.
//! The parameter must be one of the following values:
//! - \ref CLKCTL_LGPT3
//! - \ref CLKCTL_LGPT2
//! - \ref CLKCTL_LGPT1
//! - \ref CLKCTL_LGPT0
//! - \ref CLKCTL_I2S
//! - \ref CLKCTL_DMA
//! - \ref CLKCTL_LAES
//! - \ref CLKCTL_ADC0
//! - \ref CLKCTL_SPI0
//! - \ref CLKCTL_SPI1
//! - \ref CLKCTL_I2C0
//! - \ref CLKCTL_I2C1
//! - \ref CLKCTL_UART0
//! - \ref CLKCTL_UART1
//! - \ref CLKCTL_LRFD
//! - \ref CLKCTL_GPIO
//! - \ref CLKCTL_HSM
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CLKCTLDisable(uint32_t peripheral)
{
    // Check the arguments
    ASSERT(peripheral <= CLKCTL_MAX_VALUE);

    uint32_t registerIndex                                      = peripheral >> CLKCTL_REGISTER_INDEX_S;
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR0 + registerIndex * 4) = (1U << (peripheral & CLKCTL_BIT_INDEX_M));
}

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
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET0) = CLKCTL_CLKENSET0_LRFD;

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
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR0) = CLKCTL_CLKENCLR0_LRFD;
}

//*****************************************************************************
//
//! \brief Enable Flash LDO to be off in IDLE
//
//*****************************************************************************
__STATIC_INLINE void CLKCTLEnableFlashLdoOffInIdle(void)
{
    HWREG( CLKCTL_BASE + CLKCTL_O_IDLECFG ) = CLKCTL_IDLECFG_MODE_LDO_OFF;
}

//*****************************************************************************
//
//! \brief Disable Flash LDO to be off in IDLE
//
//*****************************************************************************
__STATIC_INLINE void CLKCTLDisableFlashLdoOffInIdle(void)
{
    HWREG( CLKCTL_BASE + CLKCTL_O_IDLECFG ) = CLKCTL_IDLECFG_MODE_LDO_ON;
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

#endif // ti_devices_clkctl__include
