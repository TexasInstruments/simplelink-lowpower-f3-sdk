/******************************************************************************
 *  Filename:       evtsvt_veneers.c
 *
 *  Description:    Veneers for accessing EVTSVT API from non-secure context.
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

#include "evtsvt.h"

#include <third_party/tfm/secure_fw/include/security_defs.h> // __tz_c_veneer
#include <third_party/tfm/secure_fw/spm/include/utilities.h> // tfm_core_panic()

//*****************************************************************************
//
// Local variables
//
//*****************************************************************************

//*****************************************************************************
//
// Subscriber permission configuration typedef (does not cover DMA subscribers,
// see EVTSVTDmaChMask). Each subscriber is represented by a bit. If a
// subscriber bit is set (1), it is accessible by an NS application, otherwise
// not. The position of the named elements in the struct (for example
// CPUIRQ6SEL) is found using this formula:
//
// bitNo = (regAddr - lowestRegAddr) / 4
//
// Where divide by 4 accounts for the byte size of each register.
//
// To access the bits using the array form of the union, the subscriber-to-bit
// mapping is calculated using these formulas:
//
// wordIdx = ((regAddr - lowestRegAddr) / 4) / 32
// bitIdx = ((regAddr - lowestRegAddr) / 4) % 32
//
// Divide by 32 gives the array offset, and modulo 32 gives the bit position in
// that 32-bit array entry.
//
//*****************************************************************************
typedef union
{
    uint32_t array[2];
    struct
    {
        struct
        {
            uint32_t CPUIRQ0SEL:1; // Bit 0
            uint32_t CPUIRQ1SEL:1;
            uint32_t CPUIRQ2SEL:1;
            uint32_t CPUIRQ3SEL:1;
            uint32_t CPUIRQ4SEL:1;
            uint32_t CPUIRQ5SEL:1;
            uint32_t CPUIRQ6SEL:1;
            uint32_t CPUIRQ7SEL:1;
            uint32_t CPUIRQ8SEL:1;
            uint32_t CPUIRQ9SEL:1;
            uint32_t CPUIRQ10SEL:1;
            uint32_t CPUIRQ11SEL:1;
            uint32_t CPUIRQ12SEL:1;
            uint32_t CPUIRQ13SEL:1;
            uint32_t CPUIRQ14SEL:1;
            uint32_t CPUIRQ15SEL:1;
            uint32_t CPUIRQ16SEL:1;
            uint32_t CPUIRQ17SEL:1;
            uint32_t CPUIRQ18SEL:1;
            uint32_t CPUIRQ19SEL:1;
            uint32_t CPUIRQ20SEL:1;
            uint32_t CPUIRQ21SEL:1;
            uint32_t CPUIRQ22SEL:1;
            uint32_t CPUIRQ23SEL:1;
            uint32_t CPUIRQ24SEL:1;
            uint32_t CPUIRQ25SEL:1;
            uint32_t CPUIRQ26SEL:1;
            uint32_t CPUIRQ27SEL:1;
            uint32_t CPUIRQ28SEL:1;
            uint32_t CPUIRQ29SEL:1;
            uint32_t CPUIRQ30SEL:1;
            uint32_t CPUIRQ31SEL:1; // Bit 31
        } word0;
        struct
        {
            uint32_t CPUIRQ32SEL:1; // Bit 0
            uint32_t SYSTIMC0SEL:1;
            uint32_t SYSTIMC1SEL:1;
            uint32_t SYSTIMC2SEL:1;
            uint32_t SYSTIMC3SEL:1;
            uint32_t SYSTIMC4SEL:1;
            uint32_t SYSTIMC5SEL:1;
            uint32_t ADCTRGSEL:1;
            uint32_t LGPTSYNCSEL:1;
            uint32_t LGPT0IN0SEL:1;
            uint32_t LGPT0IN1SEL:1;
            uint32_t LGPT0IN2SEL:1;
            uint32_t LGPT0TENSEL:1;
            uint32_t LGPT1IN0SEL:1;
            uint32_t LGPT1IN1SEL:1;
            uint32_t LGPT1IN2SEL:1;
            uint32_t LGPT1TENSEL:1;
            uint32_t LGPT2IN0SEL:1;
            uint32_t LGPT2IN1SEL:1;
            uint32_t LGPT2IN2SEL:1;
            uint32_t LGPT2TENSEL:1;
            uint32_t LGPT3IN0SEL:1;
            uint32_t LGPT3IN1SEL:1;
            uint32_t LGPT3IN2SEL:1;
            uint32_t LGPT3TENSEL:1;
            uint32_t LRFDIN0SEL:1;
            uint32_t LRFDIN1SEL:1;
            uint32_t LRFDIN2SEL:1;
            uint32_t I2SSTMPSEL:1; // Bit 28
            uint32_t reserved29_31:3;
        } word1;
    };
} EVTSVTSubscriberMask;

//*****************************************************************************
//
// Subscriber permission configuration for non-secure applications. Reserved
// bits are always 0.
//
//*****************************************************************************
static const EVTSVTSubscriberMask subscriberMask = {
    .word0.CPUIRQ0SEL    = 1,
    .word0.CPUIRQ1SEL    = 1,
    .word0.CPUIRQ2SEL    = 1,
    .word0.CPUIRQ3SEL    = 0, // Used by secure Power driver
    .word0.CPUIRQ4SEL    = 1,
    // CPUIRQ5-CPUIRQ15 are all fixed, so changing them should not be attempted
    .word0.CPUIRQ5SEL    = 0,
    .word0.CPUIRQ6SEL    = 0,
    .word0.CPUIRQ7SEL    = 0,
    .word0.CPUIRQ8SEL    = 0,
    .word0.CPUIRQ9SEL    = 0,
    .word0.CPUIRQ10SEL   = 0,
    .word0.CPUIRQ11SEL   = 0,
    .word0.CPUIRQ12SEL   = 0,
    .word0.CPUIRQ13SEL   = 0,
    .word0.CPUIRQ14SEL   = 0,
    .word0.CPUIRQ15SEL   = 0,
    .word0.CPUIRQ16SEL   = 1,
    .word0.CPUIRQ17SEL   = 1,
    // CPUIRQ18-CPUIRQ32 are all fixed, so changing them should not be attempted
    .word0.CPUIRQ18SEL   = 0,
    .word0.CPUIRQ19SEL   = 0,
    .word0.CPUIRQ20SEL   = 0,
    .word0.CPUIRQ21SEL   = 0,
    .word0.CPUIRQ22SEL   = 0,
    .word0.CPUIRQ23SEL   = 0,
    .word0.CPUIRQ24SEL   = 0,
    .word0.CPUIRQ25SEL   = 0,
    .word0.CPUIRQ26SEL   = 0,
    .word0.CPUIRQ27SEL   = 0,
    .word0.CPUIRQ28SEL   = 0,
    .word0.CPUIRQ29SEL   = 0,
    .word0.CPUIRQ30SEL   = 0,
    .word0.CPUIRQ31SEL   = 0,
    .word1.CPUIRQ32SEL   = 0,
    .word1.SYSTIMC0SEL   = 1,
    .word1.SYSTIMC1SEL   = 1,
    .word1.SYSTIMC2SEL   = 1,
    .word1.SYSTIMC3SEL   = 1,
    .word1.SYSTIMC4SEL   = 1,
    .word1.SYSTIMC5SEL   = 1,
    .word1.ADCTRGSEL     = 1,
    .word1.LGPTSYNCSEL   = 1,
    .word1.LGPT0IN0SEL   = 1,
    .word1.LGPT0IN1SEL   = 1,
    .word1.LGPT0IN2SEL   = 1,
    .word1.LGPT0TENSEL   = 1,
    .word1.LGPT1IN0SEL   = 1,
    .word1.LGPT1IN1SEL   = 1,
    .word1.LGPT1IN2SEL   = 1,
    .word1.LGPT1TENSEL   = 1,
    .word1.LGPT2IN0SEL   = 1,
    .word1.LGPT2IN1SEL   = 1,
    .word1.LGPT2IN2SEL   = 1,
    .word1.LGPT2TENSEL   = 1,
    .word1.LGPT3IN0SEL   = 1,
    .word1.LGPT3IN1SEL   = 1,
    .word1.LGPT3IN2SEL   = 1,
    .word1.LGPT3TENSEL   = 1,
    .word1.LRFDIN0SEL    = 1,
    .word1.LRFDIN1SEL    = 1,
    .word1.LRFDIN2SEL    = 1,
    .word1.I2SSTMPSEL    = 1,
    .word1.reserved29_31 = 0,
};

//*****************************************************************************
//
// PUBID permission configuration typedef. Each PUBID is represented by a bit.
// If a PUBID bit is set (1), it can be used by an NS application, otherwise
// not. The position of the named elements in the struct (for example
// SYSTIM_COMB) is found using this formula:
//
// bitNo = pubId
//
// Note: If there are gaps in the PUBID values, there will be gaps in the
// bitmask, named reserved<start>[_<stop>]. These bits are not in use and must
// be set to 0.
//
// To access the bits using the array form of the union, the PUBID-to-bit
// mapping is calculated using these formulas:
//
// wordIdx = pubId / 32
// bitIdx =  pubId % 32
//
// Divide by 32 gives the array offset, and modulo 32 gives the bit position in
// that 32-bit array entry.
//
// For example, using EVTSVT_PUB_SYSTIM0 = 35:
// elementIdx = 35 / 32 = 1
// bitIdx = 35 % 32 = 3
// SYSTIM0's PUBID is represented by bit 3 in pubIdMask.array[1]
//
//*****************************************************************************
typedef union
{
    uint32_t array[3];
    struct
    {
        struct
        {
            uint32_t NONE:1; // Bit 0
            uint32_t reserved1:1;
            uint32_t AON_PMU_COMB:1;
            uint32_t AON_CKM_COMB:1;
            uint32_t AON_RTC_COMB:1;
            uint32_t AON_DBG_COMB:1;
            uint32_t AON_LPMCMP_IRQ:1;
            uint32_t AON_IOC_COMB:1;
            uint32_t reserved8_15:8;
            uint32_t SYSTIM_COMB:1;
            uint32_t GPIO_COMB:1;
            uint32_t GPIO_EVT:1;
            uint32_t FLASH_IRQ:1;
            uint32_t LRFD_IRQ0:1;
            uint32_t LRFD_IRQ1:1;
            uint32_t LRFD_IRQ2:1;
            uint32_t SPI0_COMB:1;
            uint32_t ADC_COMB:1;
            uint32_t ADC_EVT:1;
            uint32_t LGPT0_COMB:1;
            uint32_t LGPT1_COMB:1;
            uint32_t DMA_DONE_COMB:1;
            uint32_t DMA_ERR:1;
            uint32_t AES_COMB:1;
            uint32_t UART0_COMB:1; // Bit 31
        } word0;
        struct
        {
            uint32_t I2C0_IRQ:1; // Bit 0
            uint32_t SYSTIM_HB:1;
            uint32_t SYSTIM_LT:1;
            uint32_t SYSTIM0:1;
            uint32_t SYSTIM1:1;
            uint32_t SYSTIM2:1;
            uint32_t SYSTIM3:1;
            uint32_t SYSTIM4:1;
            uint32_t LGPT0C0:1;
            uint32_t LGPT0C1:1;
            uint32_t LGPT0C2:1;
            uint32_t LGPT0_DMA:1;
            uint32_t LGPT0_ADC:1;
            uint32_t LGPT1C0:1;
            uint32_t LGPT1C1:1;
            uint32_t LGPT1C2:1;
            uint32_t LGPT1_DMA:1;
            uint32_t LGPT1_ADC:1;
            uint32_t LRFD_EVT0:1;
            uint32_t LRFD_EVT1:1;
            uint32_t LRFD_EVT2:1;
            uint32_t UART1_COMB:1;
            uint32_t LGPT2C0:1;
            uint32_t LGPT2C1:1;
            uint32_t LGPT2C2:1;
            uint32_t LGPT2_COMB:1;
            uint32_t LGPT2_DMA:1;
            uint32_t LGPT2_ADC:1;
            uint32_t LGPT3C0:1;
            uint32_t LGPT3C1:1;
            uint32_t LGPT3C2:1;
            uint32_t LGPT3_COMB:1; // Bit 31
        } word1;
        struct
        {
            uint32_t LGPT3_DMA:1; // Bit 0
            uint32_t LGPT3_ADC:1;
            uint32_t I2S_IRQ:1;
            uint32_t CAN_IRQ:1;
            uint32_t CAN_EVT:1;
            uint32_t SPI1_COMB:1;
            uint32_t APU_IRQ:1;
            uint32_t reserved7_10:4;
            uint32_t GPIO_EVT1:1;
            uint32_t SYSTIM5:1; // Bit 12
            uint32_t reserved13_31:19;
        } word2;
    };
} EVTSVTPubIdMask;

//*****************************************************************************
//
// PUBID permission configuration for non-secure applications. Reserved bits
// are always 0.
//
//*****************************************************************************
static const EVTSVTPubIdMask pubIdMask = {
    .word0.NONE           = 1,
    .word0.reserved1      = 0,
    .word0.AON_PMU_COMB   = 1,
    .word0.AON_CKM_COMB   = 0,
    .word0.AON_RTC_COMB   = 1,
    .word0.AON_DBG_COMB   = 1,
    .word0.AON_LPMCMP_IRQ = 1,
    .word0.AON_IOC_COMB   = 1,
    .word0.reserved8_15   = 0,
    .word0.SYSTIM_COMB    = 1,
    .word0.GPIO_COMB      = 1,
    .word0.GPIO_EVT       = 1,
    .word0.FLASH_IRQ      = 0,
    .word0.LRFD_IRQ0      = 1,
    .word0.LRFD_IRQ1      = 1,
    .word0.LRFD_IRQ2      = 1,
    .word0.SPI0_COMB      = 1,
    .word0.ADC_COMB       = 1,
    .word0.ADC_EVT        = 1,
    .word0.LGPT0_COMB     = 1,
    .word0.LGPT1_COMB     = 1,
    .word0.DMA_DONE_COMB  = 1,
    .word0.DMA_ERR        = 1,
    .word0.AES_COMB       = 1,
    .word0.UART0_COMB     = 1,
    .word1.I2C0_IRQ       = 1,
    .word1.SYSTIM_HB      = 1,
    .word1.SYSTIM_LT      = 1,
    .word1.SYSTIM0        = 1,
    .word1.SYSTIM1        = 1,
    .word1.SYSTIM2        = 1,
    .word1.SYSTIM3        = 1,
    .word1.SYSTIM4        = 1,
    .word1.LGPT0C0        = 1,
    .word1.LGPT0C1        = 1,
    .word1.LGPT0C2        = 1,
    .word1.LGPT0_DMA      = 1,
    .word1.LGPT0_ADC      = 1,
    .word1.LGPT1C0        = 1,
    .word1.LGPT1C1        = 1,
    .word1.LGPT1C2        = 1,
    .word1.LGPT1_DMA      = 1,
    .word1.LGPT1_ADC      = 1,
    .word1.LRFD_EVT0      = 1,
    .word1.LRFD_EVT1      = 1,
    .word1.LRFD_EVT2      = 1,
    .word1.UART1_COMB     = 1,
    .word1.LGPT2C0        = 1,
    .word1.LGPT2C1        = 1,
    .word1.LGPT2C2        = 1,
    .word1.LGPT2_COMB     = 1,
    .word1.LGPT2_DMA      = 1,
    .word1.LGPT2_ADC      = 1,
    .word1.LGPT3C0        = 1,
    .word1.LGPT3C1        = 1,
    .word1.LGPT3C2        = 1,
    .word1.LGPT3_COMB     = 1,
    .word2.LGPT3_DMA      = 1,
    .word2.LGPT3_ADC      = 1,
    .word2.I2S_IRQ        = 1,
    .word2.CAN_IRQ        = 1,
    .word2.CAN_EVT        = 1,
    .word2.SPI1_COMB      = 1,
    .word2.APU_IRQ        = 1,
    .word2.reserved7_10   = 0,
    .word2.GPIO_EVT1      = 1,
    .word2.SYSTIM5        = 1,
    .word2.reserved13_31  = 0,
};

//*****************************************************************************
//
// IPID permission configuration typedef. Each IPID is represented by a bit. If
// an IPID bit is set (1), it can be used by an NS application, otherwise not.
// The position of the named elements in the struct (for example LRFDTRG) is
// found using this formula:
//
// bitNo = ipId
//
// To access the bits using the val32 form of the union, the ipId-to-bit
// mapping is calculated using the same formula as above.
//
//*****************************************************************************
typedef union
{
    uint32_t val32;
    struct
    {
        uint32_t SPI0TXTRG:1; // Bit 0
        uint32_t SPI0RXTRG:1;
        uint32_t LRFDTRG:1;
        uint32_t LAESTRGA:1;
        uint32_t LAESTRGB:1;
        uint32_t ADC0TRG:1;
        uint32_t UART0TXTRG:1;
        uint32_t UART0RXTRG:1;
        uint32_t CANTRGA:1;
        uint32_t CANTRGB:1;
        uint32_t SPI1TXTRG:1;
        uint32_t SPI1RXTRG:1;
        uint32_t UART1TXTRG:1;
        uint32_t UART1RXTRG:1; // Bit 13
        uint32_t reserved14_31:18;
    };
} EVTSVTIpIdMask;

//*****************************************************************************
//
// IPID permission configuration for non-secure applications. Reserved bits
// are always 0.
//
//*****************************************************************************
static const EVTSVTIpIdMask ipIdMask = {
    .SPI0TXTRG     = 1,
    .SPI0RXTRG     = 1,
    .LRFDTRG       = 1,
    .LAESTRGA      = 1,
    .LAESTRGB      = 1,
    .ADC0TRG       = 1,
    .UART0TXTRG    = 1,
    .UART0RXTRG    = 1,
    .CANTRGA       = 1,
    .CANTRGB       = 1,
    .SPI1TXTRG     = 1,
    .SPI1RXTRG     = 1,
    .UART1TXTRG    = 1,
    .UART1RXTRG    = 1,
    .reserved14_31 = 0,
};

//*****************************************************************************
//
// DMA channel permission configuration typedef. Each DMA channel is represented
// by a bit. If a DMA channel bit is set (1), it is accessible by an NS
// application, otherwise not. The position of the named elements in the struct
// (for example DMACH4) is based on the channel's register address, and is found
// using this formula:
//
// bitNo = (regAddr - lowestRegAddr) / 4
//
// Where divide by 4 accounts for the byte size of each register.
//
// To access the bits using the val32 form of the union, the channel-to-bit
// mapping is calculated using the same formula as above.
//
//*****************************************************************************
typedef union
{
    uint32_t val32;
    struct
    {
        uint32_t DMACH0:1; // Bit 0
        uint32_t DMACH1:1;
        uint32_t DMACH2:1;
        uint32_t DMACH3:1;
        uint32_t DMACH4:1;
        uint32_t DMACH5:1;
        uint32_t DMACH6:1;
        uint32_t DMACH7:1;
        // Channel 10 and 11 register addresses come before channel 8 and 9.
        uint32_t DMACH10:1;
        uint32_t DMACH11:1;
        uint32_t DMACH8:1;
        uint32_t DMACH9:1; // Bit 11
        uint32_t reserved12_31:20;
    };
} EVTSVTDmaChMask;

//*****************************************************************************
//
// DMA channel permission configuration for non-secure applications. Reserved
// bits are always 0.
//
//*****************************************************************************
static const EVTSVTDmaChMask dmaChMask = {
    .DMACH0        = 1,
    .DMACH1        = 1,
    .DMACH2        = 1,
    .DMACH3        = 1,
    .DMACH4        = 1,
    .DMACH5        = 1,
    .DMACH6        = 1,
    .DMACH7        = 1,
    .DMACH8        = 1,
    .DMACH9        = 1,
    .DMACH10       = 1,
    .DMACH11       = 1,
    .reserved12_31 = 0,
};

//*****************************************************************************
//
// Local functions
//
//*****************************************************************************

//*****************************************************************************
//
// Validate the provided PUBID. Panic if PUBID is invalid or not allowed.
//
//*****************************************************************************
static void validatePubId(uint32_t pubId)
{
    // Check PUBID bounds
    if (pubId > EVTSVT_PUBID_MAX_VAL)
    {
        // Out-of-bounds PUBID
        tfm_core_panic();
    }

    // Check if PUBID is allowed
    uint32_t elementIdx = (pubId / 32);
    uint32_t bitIdx     = (pubId % 32);

    if (((1 << bitIdx) & pubIdMask.array[elementIdx]) == 0)
    {
        // NS side is not allowed to configure this PUBID
        tfm_core_panic();
    }
}

//*****************************************************************************
//
// Veneers
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Veneer for calling EVTSVTConfigureDma() from non-secure context.
//!
//! \warning This function is not included in driverlib.a nor driverlib_ns.a,
//! but is a part of the veneers library generated together with the secure
//! TF-M image.
//!
//! \sa EVTSVTConfigureDma()
//
//*****************************************************************************
__tz_c_veneer void EVTSVTConfigureDma_veneer(uint32_t channel, uint32_t id)
{
    // Check channel bounds
    if ((channel < EVTSVT_DMA_CH_MIN_VAL) || (channel > EVTSVT_DMA_CH_MAX_VAL))
    {
        tfm_core_panic();
    }

    // Check channel alignment
    if ((channel % 4) != 0)
    {
        tfm_core_panic();
    }

    // Convert from channel address offset to position in the mask.
    uint32_t chMaskPos = (channel - EVTSVT_DMA_CH_MIN_VAL) / sizeof(uint32_t);

    // Check if channel is allowed
    if (((1 << chMaskPos) & dmaChMask.val32) == 0)
    {
        // NS side is not allowed to configure this channel number
        tfm_core_panic();
    }

    if (channel <= EVTSVT_DMA_CH7)
    {
        // Channel 0-7, these channels use IPID.
        // Check IPID bounds
        if (id > EVTSVT_IPID_MAX_VAL)
        {
            // Out-of-bounds IPID
            tfm_core_panic();
        }

        // Check if IPID is allowed
        if (((1 << id) & ipIdMask.val32) == 0)
        {
            // NS side is not allowed to configure this IPID
            tfm_core_panic();
        }
    }
    else if (channel <= EVTSVT_DMA_CH_MAX_VAL)
    {
        // These channels use PUBID.
        // The function returns if the PUBID is valid.
        validatePubId(id);
    }
    else
    {
        // We should never get here.
        tfm_core_panic();
    }

    // Apply configuration
    EVTSVTConfigureDma(channel, id);
}

//*****************************************************************************
//
//! \brief Veneer for calling EVTSVTConfigureEvent() from non-secure context.
//!
//! \warning This function is not included in driverlib.a nor driverlib_ns.a,
//! but is a part of the veneers library generated together with the secure
//! TF-M image.
//!
//! \sa EVTSVTConfigureEvent()
//
//*****************************************************************************
__tz_c_veneer void EVTSVTConfigureEvent_veneer(uint32_t subscriber, uint32_t pubId)
{
    // Check subscriber bounds
    if ((subscriber > EVTSVT_SUB_MAX_VAL) || (subscriber < EVTSVT_SUB_MIN_VAL))
    {
        // Out-of-bounds
        tfm_core_panic();
    }

    // Check subscriber alignment
    if ((subscriber % 4) != 0)
    {
        tfm_core_panic();
    }

    // Check publisher bounds
    if (pubId > EVTSVT_PUBID_MAX_VAL)
    {
        // Out-of-bounds PUBID
        tfm_core_panic();
    }

    // Convert from subscriber address offset to position in the mask.
    uint32_t subscriberMaskPos = (subscriber - EVTSVT_SUB_MIN_VAL) / sizeof(uint32_t);

    // Check if subscriber is allowed to be configured
    uint32_t elementIdx = (subscriberMaskPos / 32);
    uint32_t bitIdx     = (subscriberMaskPos % 32);
    if (((1 << bitIdx) & subscriberMask.array[elementIdx]) == 0)
    {
        // NS side is not allowed to configure this subscriber
        tfm_core_panic();
    }

    // Validate PUBID, panics if invalid.
    validatePubId(pubId);

    // Apply configuration
    EVTSVTConfigureEvent(subscriber, pubId);
}
