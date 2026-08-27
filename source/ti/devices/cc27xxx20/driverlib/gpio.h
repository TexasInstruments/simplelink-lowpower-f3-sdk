/******************************************************************************
 *  Filename:       gpio.h
 *
 *  Description:    Defines and prototypes for the GPIO.
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

#ifndef ti_devices_gpio__include
#define ti_devices_gpio__include

//*****************************************************************************
//
//! \addtogroup peripheral_group
//! @{
//! \addtogroup gpio_api
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
#include "../inc/hw_gpio.h"
#include "debug.h"

//*****************************************************************************
//
// Check for legal range of variable dioNumber
//
//*****************************************************************************
#ifdef DRIVERLIB_DEBUG
static bool dioNumberLegal(uint32_t dioNumber)
{
    // TODO: Implement properly
    // Get number of GPIOs supported by platform. Number of DIOs is NUMDIO + 1.
    // Actual number available may be less for some package types.
    uint32_t numDio = 1 + ((HWREG(GPIO_BASE + GPIO_O_DESCEX) & GPIO_DESCEX_NUMDIO_M) >> GPIO_DESCEX_NUMDIO_S);
    return (dioNumber < numDio);
}
#endif

//*****************************************************************************
//
// The following values define the bit field for the GPIO DIOs.
//
//*****************************************************************************
#define GPIO_DIO_0_MASK  0x0000000000000001ULL //!< GPIO DIO 0 mask
#define GPIO_DIO_1_MASK  0x0000000000000002ULL //!< GPIO DIO 1 mask
#define GPIO_DIO_2_MASK  0x0000000000000004ULL //!< GPIO DIO 2 mask
#define GPIO_DIO_3_MASK  0x0000000000000008ULL //!< GPIO DIO 3 mask
#define GPIO_DIO_4_MASK  0x0000000000000010ULL //!< GPIO DIO 4 mask
#define GPIO_DIO_5_MASK  0x0000000000000020ULL //!< GPIO DIO 5 mask
#define GPIO_DIO_6_MASK  0x0000000000000040ULL //!< GPIO DIO 6 mask
#define GPIO_DIO_7_MASK  0x0000000000000080ULL //!< GPIO DIO 7 mask
#define GPIO_DIO_8_MASK  0x0000000000000100ULL //!< GPIO DIO 8 mask
#define GPIO_DIO_9_MASK  0x0000000000000200ULL //!< GPIO DIO 9 mask
#define GPIO_DIO_10_MASK 0x0000000000000400ULL //!< GPIO DIO 10 mask
#define GPIO_DIO_11_MASK 0x0000000000000800ULL //!< GPIO DIO 11 mask
#define GPIO_DIO_12_MASK 0x0000000000001000ULL //!< GPIO DIO 12 mask
#define GPIO_DIO_13_MASK 0x0000000000002000ULL //!< GPIO DIO 13 mask
#define GPIO_DIO_14_MASK 0x0000000000004000ULL //!< GPIO DIO 14 mask
#define GPIO_DIO_15_MASK 0x0000000000008000ULL //!< GPIO DIO 15 mask
#define GPIO_DIO_16_MASK 0x0000000000010000ULL //!< GPIO DIO 16 mask
#define GPIO_DIO_17_MASK 0x0000000000020000ULL //!< GPIO DIO 17 mask
#define GPIO_DIO_18_MASK 0x0000000000040000ULL //!< GPIO DIO 18 mask
#define GPIO_DIO_19_MASK 0x0000000000080000ULL //!< GPIO DIO 19 mask
#define GPIO_DIO_20_MASK 0x0000000000100000ULL //!< GPIO DIO 20 mask
#define GPIO_DIO_21_MASK 0x0000000000200000ULL //!< GPIO DIO 21 mask
#define GPIO_DIO_22_MASK 0x0000000000400000ULL //!< GPIO DIO 22 mask
#define GPIO_DIO_23_MASK 0x0000000000800000ULL //!< GPIO DIO 23 mask
#define GPIO_DIO_24_MASK 0x0000000001000000ULL //!< GPIO DIO 24 mask
#define GPIO_DIO_25_MASK 0x0000000002000000ULL //!< GPIO DIO 25 mask
#define GPIO_DIO_26_MASK 0x0000000004000000ULL //!< GPIO DIO 26 mask
#define GPIO_DIO_27_MASK 0x0000000008000000ULL //!< GPIO DIO 27 mask
#define GPIO_DIO_28_MASK 0x0000000010000000ULL //!< GPIO DIO 28 mask
#define GPIO_DIO_29_MASK 0x0000000020000000ULL //!< GPIO DIO 29 mask
#define GPIO_DIO_30_MASK 0x0000000040000000ULL //!< GPIO DIO 30 mask
#define GPIO_DIO_31_MASK 0x0000000080000000ULL //!< GPIO DIO 31 mask
#define GPIO_DIO_32_MASK 0x0000000100000000ULL //!< GPIO DIO 32 mask
#define GPIO_DIO_33_MASK 0x0000000200000000ULL //!< GPIO DIO 33 mask
#define GPIO_DIO_34_MASK 0x0000000400000000ULL //!< GPIO DIO 34 mask
#define GPIO_DIO_35_MASK 0x0000000800000000ULL //!< GPIO DIO 35 mask
#define GPIO_DIO_36_MASK 0x0000001000000000ULL //!< GPIO DIO 36 mask
#define GPIO_DIO_37_MASK 0x0000002000000000ULL //!< GPIO DIO 37 mask
#define GPIO_DIO_38_MASK 0x0000004000000000ULL //!< GPIO DIO 38 mask
#define GPIO_DIO_39_MASK 0x0000008000000000ULL //!< GPIO DIO 39 mask
//! GPIO mask containing all DIOs
#define GPIO_DIO_ALL_MASK                                                                                              \
    (GPIO_DIO_0_MASK | GPIO_DIO_1_MASK | GPIO_DIO_2_MASK | GPIO_DIO_3_MASK | GPIO_DIO_4_MASK | GPIO_DIO_5_MASK |       \
     GPIO_DIO_6_MASK | GPIO_DIO_7_MASK | GPIO_DIO_8_MASK | GPIO_DIO_9_MASK | GPIO_DIO_10_MASK | GPIO_DIO_11_MASK |     \
     GPIO_DIO_12_MASK | GPIO_DIO_13_MASK | GPIO_DIO_14_MASK | GPIO_DIO_15_MASK | GPIO_DIO_16_MASK | GPIO_DIO_17_MASK | \
     GPIO_DIO_18_MASK | GPIO_DIO_19_MASK | GPIO_DIO_20_MASK | GPIO_DIO_21_MASK | GPIO_DIO_22_MASK | GPIO_DIO_23_MASK | \
     GPIO_DIO_24_MASK | GPIO_DIO_25_MASK | GPIO_DIO_26_MASK | GPIO_DIO_27_MASK | GPIO_DIO_28_MASK | GPIO_DIO_29_MASK | \
     GPIO_DIO_30_MASK | GPIO_DIO_31_MASK | GPIO_DIO_32_MASK | GPIO_DIO_33_MASK | GPIO_DIO_34_MASK | GPIO_DIO_35_MASK | \
     GPIO_DIO_36_MASK | GPIO_DIO_37_MASK | GPIO_DIO_38_MASK | GPIO_DIO_39_MASK)

//*****************************************************************************
//
// Define constants that shall be passed as the outputEnableValue parameter to
// GPIOSetOutputEnableDio() and will be returned from the function
// GPIOGetOutputEnableDio().
//
//*****************************************************************************
#define GPIO_OUTPUT_DISABLE GPIO_DOE3_0_DIO0_DIS //!< DIO output is disabled
#define GPIO_OUTPUT_ENABLE  GPIO_DOE3_0_DIO0_EN  //!< DIO output is enabled

//*****************************************************************************
//
//! Register address offset between lower (31-0) and upper (63-32) GPIO banks
//!
//! This device supports 40 DIOs which are split across two register banks:
//! - DIOs 0-31 use registers with suffix _31_0 (e.g., DOUT31_0, DIN31_0)
//! - DIOs 32-39 use registers with suffix _63_32 (e.g., DOUT63_32, DIN63_32)
//!
//! This constant defines the address offset between corresponding registers
//! in these two banks. It is used internally by the driverlib implementation
//! to efficiently access both register banks within multi-DIO functions.
//!
//! \note This constant is for internal driverlib use only and should not be
//! used by application code.
//!
//! The compile-time checks below verify that all register pairs maintain
//! consistent offsets, which allows the driver to use this single offset
//! value for all register accesses.
//
//*****************************************************************************
#define GPIO_63_32_REGISTER_OFFSET (GPIO_O_DOUT63_32 - GPIO_O_DOUT31_0)

#if (((GPIO_O_DOUTSET31_0 + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_DOUTSET63_32) || \
     ((GPIO_O_DOUTCLR31_0 + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_DOUTCLR63_32) || \
     ((GPIO_O_DOUTTGL31_0 + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_DOUTTGL63_32) || \
     ((GPIO_O_DOE31_0 + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_DOE63_32) ||         \
     ((GPIO_O_DOESET31_0 + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_DOESET63_32) ||   \
     ((GPIO_O_DOECLR31_0 + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_DOECLR63_32) ||   \
     ((GPIO_O_DOETGL31_0 + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_DOETGL63_32) ||   \
     ((GPIO_O_DIN31_0 + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_DIN63_32) ||         \
     ((GPIO_O_IMASK + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_IMASKEX) ||            \
     ((GPIO_O_RIS + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_RISEX) ||                \
     ((GPIO_O_MIS + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_MISEX) ||                \
     ((GPIO_O_ISET + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_ISETEX) ||              \
     ((GPIO_O_ICLR + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_ICLREX) ||              \
     ((GPIO_O_IMSET + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_IMSETEX) ||            \
     ((GPIO_O_IMCLR + GPIO_63_32_REGISTER_OFFSET) != GPIO_O_IMCLREX))
    #error "Offset between 31-0 registers and 63-32 registers is inconsistent" \
           "This is not handled by the current implementation!."
#endif
//*****************************************************************************
//
// API Functions and prototypes
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Reads a specific DIO.
//!
//! \param dioNumber specifies the DIO to read (0-39).
//!
//! \return Returns 0 or 1 reflecting the input value of the specified DIO.
//!
//! \sa \ref GPIOReadMultiDio(), \ref GPIOWriteDio(), \ref GPIOWriteMultiDio()
//
//*****************************************************************************
__STATIC_INLINE uint32_t GPIOReadDio(uint32_t dioNumber)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    // Return the input value from the specified DIO.
    return (uint32_t)HWREGB(GPIO_BASE + GPIO_O_DIN3_0 + dioNumber);
}

//*****************************************************************************
//
//! \brief Reads output register of a specific DIO.
//!
//! \param dioNumber specifies the DIO to read (0-39).
//!
//! \return Returns 0 or 1 reflecting the data out value of the specified DIO.
//!
//
//*****************************************************************************
__STATIC_INLINE uint32_t GPIOReadDioOutputBuffer(uint32_t dioNumber)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    // Return the data out value from the specified DIO.
    return (uint32_t)HWREGB(GPIO_BASE + GPIO_O_DOUT3_0 + dioNumber);
}

//*****************************************************************************
//
//! \brief Reads the input value for the specified DIOs.
//!
//! This function returns the input value for multiple DIOs.
//! The value returned is not shifted and hence matches the corresponding
//! dioMask bits.
//!
//! \note This will not read all the specified DIOs at the same time. DIO0-DIO31
//! will be read separately from DIO32-DIO39.
//!
//! \param dioMask is the bit-mask representation of the DIOs to read.
//! The parameter must be a bitwise OR'ed combination of the following:
//! - \ref GPIO_DIO_0_MASK
//! - ...
//! - \ref GPIO_DIO_39_MASK
//!
//! \return Returns a bit vector reflecting the input value of the corresponding
//! DIOs.
//! - 0 : Corresponding DIO is low.
//! - 1 : Corresponding DIO is high.
//!
//! \sa \ref GPIOReadDio(), \ref GPIOWriteDio(), \ref GPIOWriteMultiDio()
//
//*****************************************************************************
__STATIC_INLINE uint64_t GPIOReadMultiDio(uint64_t dioMask)
{
    // Check the arguments.
    ASSERT((dioMask & GPIO_DIO_ALL_MASK) == dioMask);

    // Read the lower 32 IOs and the upper 32 IOs and combine into a 64 bit
    // value.
    uint32_t din31_0  = HWREG(GPIO_BASE + GPIO_O_DIN31_0);
    uint32_t din63_32 = HWREG(GPIO_BASE + GPIO_O_DIN63_32);
    uint64_t din63_0  = ((uint64_t)din31_0) | (((uint64_t)din63_32) << 32);

    // Apply the DIO mask and return the result.
    return din63_0 & dioMask;
}

//*****************************************************************************
//
//! \brief Writes a value to a specific DIO.
//!
//! \param dioNumber specifies the DIO to update (0-39).
//! \param value specifies the value to write
//! - 0 : Logic zero (low)
//! - 1 : Logic one (high)
//!
//! \return None
//!
//! \sa \ref GPIOWriteMultiDio(), \ref GPIOReadDio(), \ref GPIOReadMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOWriteDio(uint32_t dioNumber, uint32_t value)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));
    ASSERT((value == 0) || (value == 1));

    // Write 0 or 1 to the byte indexed DOUT map
    HWREGB(GPIO_BASE + GPIO_O_DOUT3_0 + dioNumber) = (uint8_t)value;
}

//*****************************************************************************
//
//! \brief Writes masked data to the specified DIOs.
//!
//! Enables for writing multiple bits simultaneously.
//! The value to write must be shifted so it matches the corresponding dioMask
//! bits.
//!
//! \note Note that this is a read-modify-write operation and hence not atomic.
//!
//! \note This will not write to all the specified DIOs at the same time.
//! DIO0-DIO31 will be written separately from DIO32-DIO39.
//!
//! \param dioMask is the bit-mask representation of the DIOs to write.
//! The parameter must be a bitwise OR'ed combination of the following:
//! - \ref GPIO_DIO_0_MASK
//! - ...
//! - \ref GPIO_DIO_39_MASK
//!
//! \param bitVectoredValue holds the value to be written to the corresponding
//! DIO-bits.
//!
//! \return None
//!
//! \sa \ref GPIOWriteDio(), \ref GPIOReadDio(), \ref GPIOReadMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOWriteMultiDio(uint64_t dioMask, uint64_t bitVectoredValue)
{
    // Check the arguments.
    ASSERT((dioMask & GPIO_DIO_ALL_MASK) == dioMask);

    // Split dioMask and bitVectoredValue into lower and upper 32 bits.
    uint32_t dioMask31_0           = dioMask & 0xFFFFFFFFU;
    uint32_t dioMask63_32          = (dioMask >> 32) & 0xFFFFFFFFU;
    uint32_t bitVectoredValue31_0  = bitVectoredValue & 0xFFFFFFFFU;
    uint32_t bitVectoredValue63_32 = (bitVectoredValue >> 32) & 0xFFFFFFFFU;

    // Read the DOUT registers.
    uint32_t dout31_0  = HWREG(GPIO_BASE + GPIO_O_DOUT31_0);
    uint32_t dout63_32 = HWREG(GPIO_BASE + GPIO_O_DOUT63_32);

    // Modify the DOUT registers.
    dout31_0  = (dout31_0 & ~dioMask31_0) | (bitVectoredValue31_0 & dioMask31_0);
    dout63_32 = (dout63_32 & ~dioMask63_32) | (bitVectoredValue63_32 & dioMask63_32);

    // Write back the DOUT registers.
    HWREG(GPIO_BASE + GPIO_O_DOUT31_0)  = dout31_0;
    HWREG(GPIO_BASE + GPIO_O_DOUT63_32) = dout63_32;
}

//*****************************************************************************
//
//! \brief Sets a specific DIO to 1 (high).
//!
//! \param dioNumber specifies the DIO to set (0-39).
//!
//! \return None
//!
//! \sa \ref GPIOSetMultiDio(), \ref GPIOClearDio(), \ref GPIOClearMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOSetDio(uint32_t dioNumber)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    // Determine register and bit index based on DIO number.
    uint32_t regIndex = dioNumber / 32;
    uint32_t bitIndex = dioNumber % 32;

    // Set the specified DIO.
    HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0 + regIndex*GPIO_63_32_REGISTER_OFFSET) = (1 << bitIndex);
}

//*****************************************************************************
//
//! \brief Sets the specified DIOs to 1 (high).
//!
//! \note This will not set all the specified DIOs at the same time. DIO0-DIO31
//! will be set separately from DIO32-DIO39.
//!
//! \param dioMask is the bit-mask representation of the DIOs to set.
//! The parameter must be a bitwise OR'ed combination of the following:
//! - \ref GPIO_DIO_0_MASK
//! - ...
//! - \ref GPIO_DIO_39_MASK
//!
//! \return None
//!
//! \sa \ref GPIOSetDio(), \ref GPIOClearDio(), \ref GPIOClearMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOSetMultiDio(uint64_t dioMask)
{
    // Check the arguments.
    ASSERT((dioMask & GPIO_DIO_ALL_MASK) == dioMask);

    // Set the DIOs.
    HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0)  = dioMask & 0xFFFFFFFFU;
    HWREG(GPIO_BASE + GPIO_O_DOUTSET63_32) = (dioMask >> 32) & 0xFFFFFFFFU;
}

//*****************************************************************************
//
//! \brief Clears a specific DIO to 0 (low).
//!
//! \param dioNumber specifies the DIO to clear (0-39).
//!
//! \return None
//!
//! \sa \ref GPIOClearMultiDio(), \ref GPIOSetDio(), \ref GPIOSetMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOClearDio(uint32_t dioNumber)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    // Determine register and bit index based on DIO number.
    uint32_t regIndex = dioNumber / 32;
    uint32_t bitIndex = dioNumber % 32;

    // Clear the specified DIO.
    HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0 + regIndex*GPIO_63_32_REGISTER_OFFSET) = (1 << bitIndex);
}

//*****************************************************************************
//
//! \brief Clears the specified DIOs to 0 (low).
//!
//! \note This will not clear all the specified DIOs at the same time.
//! DIO0-DIO31 will be cleared separately from DIO32-DIO39.
//!
//! \param dioMask is the bit-mask representation of the DIOs to clear.
//! The parameter must be a bitwise OR'ed combination of the following:
//! - \ref GPIO_DIO_0_MASK
//! - ...
//! - \ref GPIO_DIO_39_MASK
//!
//! \return None
//!
//! \sa \ref GPIOClearDio(), \ref GPIOSetDio(), \ref GPIOSetMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOClearMultiDio(uint64_t dioMask)
{
    // Check the arguments.
    ASSERT((dioMask & GPIO_DIO_ALL_MASK) == dioMask);

    // Clear the DIOs.
    HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0)  = dioMask & 0xFFFFFFFFU;
    HWREG(GPIO_BASE + GPIO_O_DOUTCLR63_32) = (dioMask >> 32) & 0xFFFFFFFFU;
}

//*****************************************************************************
//
//! \brief Toggles a specific DIO.
//!
//! \param dioNumber specifies the DIO to toggle (0-39).
//!
//! \return None
//!
//! \sa \ref GPIOToggleMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOToggleDio(uint32_t dioNumber)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    // Toggle the specified DIO.
    HWREGB(GPIO_BASE + GPIO_O_DOUTTGL3_0 + dioNumber) = (uint8_t)GPIO_DOUTTGL3_0_DIO0_TOGGLE;
}

//*****************************************************************************
//
//! \brief Toggles the specified DIOs.
//!
//! \note This will not toggle all the specified DIOs at the same time.
//! DIO0-DIO31 will be toggled separately from DIO32-DIO39.
//!
//! \param dioMask is the bit-mask representation of the DIOs to toggle.
//! The parameter must be a bitwise OR'ed combination of the following:
//! - \ref GPIO_DIO_0_MASK
//! - ...
//! - \ref GPIO_DIO_39_MASK
//!
//! \return None
//!
//! \sa \ref GPIOToggleDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOToggleMultiDio(uint64_t dioMask)
{
    // Check the arguments.
    ASSERT((dioMask & GPIO_DIO_ALL_MASK) == dioMask);

    // Toggle the DIOs.
    HWREG(GPIO_BASE + GPIO_O_DOUTTGL31_0)  = dioMask & 0xFFFFFFFFU;
    HWREG(GPIO_BASE + GPIO_O_DOUTTGL63_32) = (dioMask >> 32) & 0xFFFFFFFFU;
}

//*****************************************************************************
//
//! \brief Gets the output enable status of a specific DIO.
//!
//! This function returns the output enable status for the specified DIO.
//! The DIO can be configured as either input or output under software control.
//!
//! \param dioNumber specifies the DIO to get the output enable setting from
//! (0-39).
//!
//! \return Returns one of the enumerated data types (0 or 1):
//! - \ref GPIO_OUTPUT_DISABLE : DIO output is disabled.
//! - \ref GPIO_OUTPUT_ENABLE  : DIO output is enabled.
//!
//! \sa \ref GPIOGetOutputEnableMultiDio(), \ref GPIOSetOutputEnableDio(),
//! \ref GPIOSetOutputEnableMultiDio()
//
//*****************************************************************************
__STATIC_INLINE uint32_t GPIOGetOutputEnableDio(uint32_t dioNumber)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    // Return the output enable status for the specified DIO.
    return (uint32_t)HWREGB(GPIO_BASE + GPIO_O_DOE3_0 + dioNumber);
}

//*****************************************************************************
//
//! \brief Gets the output enable setting of the specified DIOs.
//!
//! This function returns the output enable setting for multiple DIOs.
//! The value returned is not shifted and hence matches the corresponding
//! dioMask bits.
//!
//! \note This will not read the output enable setting for all the specified
//! DIOs at the same time. The output enable setting for DIO0-DIO31 will be read
//! separately from the output enable setting for DIO32-DIO39.
//!
//! \param dioMask is the bit-mask representation of the DIOs to return the
//! output enable settings from.
//! The parameter must be a bitwise OR'ed combination of the following:
//! - \ref GPIO_DIO_0_MASK
//! - ...
//! - \ref GPIO_DIO_39_MASK
//!
//! \return Returns the output enable setting for multiple DIOs as a bit vector
//! corresponding to the dioMask bits.
//! - 0 : Corresponding DIO is configured with output disabled.
//! - 1 : Corresponding DIO is configured with output enabled.
//!
//! \sa \ref GPIOGetOutputEnableDio(), \ref GPIOSetOutputEnableDio(),
//! \ref GPIOSetOutputEnableMultiDio()
//
//*****************************************************************************
__STATIC_INLINE uint64_t GPIOGetOutputEnableMultiDio(uint64_t dioMask)
{
    // Check the arguments.
    ASSERT((dioMask & GPIO_DIO_ALL_MASK) == dioMask);

    // Read the output enable status for the lower 32 IOs and the upper 32 IOs
    // and combine into a 64 bit value.
    uint32_t doe31_0  = HWREG(GPIO_BASE + GPIO_O_DOE31_0);
    uint32_t doe63_32 = HWREG(GPIO_BASE + GPIO_O_DOE63_32);
    uint64_t doe63_0  = ((uint64_t)doe31_0) | (((uint64_t)doe63_32) << 32);

    // Apply the DIO mask and return the result.
    return doe63_0 & dioMask;
}

//*****************************************************************************
//
//! \brief Sets output enable of a specific DIO.
//!
//! This function sets the GPIO output enable bit for the specified DIO.
//! The DIO can be configured as either input or output under software control.
//!
//! \param dioNumber specifies the DIO to configure (0-39).
//! \param outputEnableValue specifies the output enable setting of the
//! specified DIO:
//! - \ref GPIO_OUTPUT_DISABLE : DIO output is disabled.
//! - \ref GPIO_OUTPUT_ENABLE  : DIO output is enabled.
//!
//! \return None
//!
//! \sa \ref GPIOSetOutputEnableMultiDio(), \ref GPIOGetOutputEnableDio(),
//! \ref GPIOGetOutputEnableMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOSetOutputEnableDio(uint32_t dioNumber, uint32_t outputEnableValue)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));
    ASSERT((outputEnableValue == GPIO_OUTPUT_DISABLE) || (outputEnableValue == GPIO_OUTPUT_ENABLE));

    // Update the output enable bit for the specified DIO.
    HWREGB(GPIO_BASE + GPIO_O_DOE3_0 + dioNumber) = (uint8_t)outputEnableValue;
}

//*****************************************************************************
//
//! \brief Configures the output enable setting for all specified DIOs.
//!
//! This function configures the output enable setting for the specified DIOs.
//! The output enable setting must be shifted so it matches the corresponding
//! dioMask bits. The DIOs can be configured as either an input or output under
//! software control.
//!
//! \note Note that this is a read-modify-write operation and hence not atomic.
//!
//! \note This will not set the output enable setting for all the specified
//! DIOs at the same time. The output enable setting for DIO0-DIO31 will be set
//! separately from the output enable setting for DIO32-DIO39.
//!
//! \param dioMask is the bit-mask representation of the DIOs on which to
//! configure the output enable setting. The parameter must be a bitwise OR'ed
//! combination of the following:
//! - \ref GPIO_DIO_0_MASK
//! - ...
//! - \ref GPIO_DIO_39_MASK
//!
//! \param bitVectoredOutputEnable holds the output enable setting the
//! corresponding DIO-bits:
//! - 0 : Corresponding DIO is configured with output disabled.
//! - 1 : Corresponding DIO is configured with output enabled.
//!
//! \return None
//!
//! \sa \ref GPIOSetOutputEnableDio(), \ref GPIOGetOutputEnableDio(),
//! \ref GPIOGetOutputEnableMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOSetOutputEnableMultiDio(uint64_t dioMask, uint64_t bitVectoredOutputEnable)
{
    // Check the arguments.
    ASSERT((dioMask & GPIO_DIO_ALL_MASK) == dioMask);

    // Split dioMask and bitVectoredOutputEnable into lower and upper 32 bits.
    uint32_t dioMask31_0                  = dioMask & 0xFFFFFFFFU;
    uint32_t dioMask63_32                 = (dioMask >> 32) & 0xFFFFFFFFU;
    uint32_t bitVectoredOutputEnable31_0  = bitVectoredOutputEnable & 0xFFFFFFFFU;
    uint32_t bitVectoredOutputEnable63_32 = (bitVectoredOutputEnable >> 32) & 0xFFFFFFFFU;

    // Read the DOE registers.
    uint32_t doe31_0  = HWREG(GPIO_BASE + GPIO_O_DOE31_0);
    uint32_t doe63_32 = HWREG(GPIO_BASE + GPIO_O_DOE63_32);

    // Modify the DOE registers.
    doe31_0  = (doe31_0 & ~dioMask31_0) | (bitVectoredOutputEnable31_0 & dioMask31_0);
    doe63_32 = (doe63_32 & ~dioMask63_32) | (bitVectoredOutputEnable63_32 & dioMask63_32);

    // Write back the DOE registers.
    HWREG(GPIO_BASE + GPIO_O_DOE31_0)  = doe31_0;
    HWREG(GPIO_BASE + GPIO_O_DOE63_32) = doe63_32;
}

//*****************************************************************************
//
//! \brief Gets the event status of a specific DIO.
//!
//! \param dioNumber specifies the DIO to get the event status from (0-39).
//!
//! \param masked selects either raw or masked interrupt status.
//! - \c true  : Masked interrupt status is required.
//! - \c false : Raw interrupt status is required.
//!
//! \return Returns the current event status on the specified DIO.
//! - 0 : Non-triggered event.
//! - 1 : Triggered event.
//!
//! \sa \ref GPIOGetEventMultiDio(), \ref GPIOClearEventDio(),
//! \ref GPIOClearEventMultiDio()
//
//*****************************************************************************
__STATIC_INLINE uint32_t GPIOGetEventDio(uint32_t dioNumber, bool masked)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    uint32_t regOffset = GPIO_O_RIS;
    uint32_t bitIndex  = dioNumber % 32;

    if (masked)
    {
        regOffset = GPIO_O_MIS;
    }

    // Adjust register offset based on DIO number.
    // This selects RIS/MIS for DIO0-DIO31 and RISEX/MISEX for DIO32-DIO39.
    regOffset += (dioNumber / 32) * 4;

    // Return the event status for the specified DIO.
    return ((HWREG(GPIO_BASE + regOffset) >> bitIndex) & 1);
}

//*****************************************************************************
//
//! \brief Gets the event status of the specified DIOs.
//!
//! This function returns the event status for multiple DIOs.
//! The value returned is not shifted and hence matches the corresponding
//! dioMask bits.
//!
//! \note This will not read the event status for all the specified DIOs at the
//! same time. The event status for DIO0-DIO31 will be read separately from the
//! event status for DIO32-DIO39.
//!
//! \param dioMask is the bit-mask representation of the DIOs to get the
//! event status from (0-39).
//! The parameter must be a bitwise OR'ed combination of the following:
//! - \ref GPIO_DIO_0_MASK
//! - ...
//! - \ref GPIO_DIO_39_MASK
//!
//! \param masked selects either raw or masked interrupt status.
//! - \c true  : Masked interrupt status is required.
//! - \c false : Raw interrupt status is required.
//!
//! \return Returns a bit vector with the current event status corresponding to
//! the specified DIOs.
//! - 0 : Corresponding DIO has no triggered event.
//! - 1 : Corresponding DIO has a triggered event.
//!
//! \sa \ref GPIOGetEventDio(), \ref GPIOClearEventDio(),
//! \ref GPIOClearEventMultiDio()
//
//*****************************************************************************
__STATIC_INLINE uint64_t GPIOGetEventMultiDio(uint64_t dioMask, bool masked)
{
    // Check the arguments.
    ASSERT((dioMask & GPIO_DIO_ALL_MASK) == dioMask);

    uint32_t regOffset = GPIO_O_RIS;

    if (masked)
    {
        regOffset = GPIO_O_MIS;
    }

    // Read the event status for the lower 32 IOs and the upper 32 IOs and
    // combine into a 64 bit value.
    uint32_t event31_0  = HWREG(GPIO_BASE + regOffset);
    uint32_t event63_32 = HWREG(GPIO_BASE + regOffset + GPIO_63_32_REGISTER_OFFSET);
    uint64_t event63_0  = ((uint64_t)event31_0) | (((uint64_t)event63_32) << 32);

    // Apply the DIO mask and return the result.
    return event63_0 & dioMask;
}

//*****************************************************************************
//
//! \brief Clears the IO event status of a specific DIO.
//!
//! \param dioNumber specifies the DIO on which to clear the event status
//! (0-39).
//!
//! \return None
//!
//! \sa \ref GPIOClearEventMultiDio(), \ref GPIOGetEventDio(),
//! \ref GPIOGetEventMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOClearEventDio(uint32_t dioNumber)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    // Determine register and bit index based on DIO number.
    uint32_t regIndex = dioNumber / 32;
    uint32_t bitIndex = dioNumber % 32;

    // Clear the event status for the specified DIO.
    HWREG(GPIO_BASE + GPIO_O_ICLR + regIndex*GPIO_63_32_REGISTER_OFFSET) = (1 << bitIndex);
}

//*****************************************************************************
//
//! \brief Clears the IO event status on the specified DIOs.
//!
//! \note This will not clear the event status for all the specified DIOs at the
//! same time. The event status for DIO0-DIO31 will be cleared separately from
//! the event status for DIO32-DIO39.
//!
//! \param dioMask is the bit-mask representation of the DIOs on which to
//! clear the events status.
//! The parameter must be a bitwise OR'ed combination of the following:
//! - \ref GPIO_DIO_0_MASK
//! - ...
//! - \ref GPIO_DIO_39_MASK
//!
//! \return None
//!
//! \sa \ref GPIOClearEventDio(), \ref GPIOGetEventDio(),
//! \ref GPIOGetEventMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOClearEventMultiDio(uint64_t dioMask)
{
    // Check the arguments.
    ASSERT((dioMask & GPIO_DIO_ALL_MASK) == dioMask);

    // Clear the event status for the specified DIOs.
    HWREG(GPIO_BASE + GPIO_O_ICLR)   = dioMask & 0xFFFFFFFFU;
    HWREG(GPIO_BASE + GPIO_O_ICLREX) = (dioMask >> 32) & 0xFFFFFFFFU;
}

//*****************************************************************************
//
//! \brief Disable events for a specific DIO.
//!
//! \param dioNumber specifies the DIO to disable the events from (0-39).
//!
//! \return None
//!
//! \sa \ref GPIOEnableEventDio(), \ref GPIOGetEventEnableDio(),
//! \ref GPIOSetEventEnableMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIODisableEventDio(uint32_t dioNumber)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    // Determine register and bit index based on DIO number.
    uint32_t regIndex = dioNumber / 32;
    uint32_t bitIndex = dioNumber % 32;

    // Disable the event for the specified DIO.
    HWREG(GPIO_BASE + GPIO_O_IMCLR + regIndex*GPIO_63_32_REGISTER_OFFSET) = (1 << bitIndex);
}

//*****************************************************************************
//
//! \brief Enable events for a specific DIO.
//!
//! Prior to enabling the events, any pending events will be cleared.
//!
//! \param dioNumber specifies the DIO to enable the events from (0-39).
//!
//! \return None
//!
//! \sa \ref GPIODisableEventDio(), \ref GPIOGetEventEnableDio(),
//! \ref GPIOSetEventEnableMultiDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOEnableEventDio(uint32_t dioNumber)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    // Determine register and bit index based on DIO number.
    uint32_t regIndex = dioNumber / 32;
    uint32_t bitIndex = dioNumber % 32;

    // Clear any pending events before enabling the mask. It is possible under
    // some circumstances for edge events to be cached in RIS, even if detection
    // is disabled. We want to make sure these events are not dispatched.
    GPIOClearEventDio(dioNumber);

    // Enable the event for the specified DIO.
    HWREG(GPIO_BASE + GPIO_O_IMSET + regIndex*GPIO_63_32_REGISTER_OFFSET) = (1 << bitIndex);
}

//*****************************************************************************
//
//! \brief Gets the event enabled state for a specific DIO.
//!
//! \param dioNumber specifies the DIO to get the event enabled state from
//! (0-39).
//!
//! \return Returns the current event enabled state on the specified DIO.
//! - 0 : Event disabled.
//! - 1 : Event enabled.
//!
//! \sa \ref GPIODisableEventDio(), \ref GPIOEnableEventDio(),
//! \ref GPIOSetEventEnableMultiDio()
//
//*****************************************************************************
__STATIC_INLINE uint32_t GPIOGetEventEnableDio(uint32_t dioNumber)
{
    // Check the arguments.
    ASSERT(dioNumberLegal(dioNumber));

    // Determine register and bit index based on DIO number.
    uint32_t regIndex = dioNumber / 32;
    uint32_t bitIndex = dioNumber % 32;

    // Return the event status for the specified DIO.
    return ((HWREG(GPIO_BASE + GPIO_O_IMASK + regIndex*GPIO_63_32_REGISTER_OFFSET) >> bitIndex) & 1);
}

//*****************************************************************************
//
//! \brief Configures the event enable setting for all specified DIOs.
//!
//! This function configures the event enable setting for the specified DIOs.
//! The event enable setting must be shifted so it matches the corresponding
//! dioMask bits.
//!
//! Prior to enabling the events, any pending events will be cleared. This will
//! only be done to the DIOs with the corresponding bit set in dioMask and
//! bitVectoredEventEnable
//!
//! \note Note that this is a read-modify-write operation and hence not atomic.
//!
//! \note This will not set the event enable status for all the specified DIOs
//! at the same time. The event enable status for DIO0-DIO31 will be set
//! separately from the event enable status for DIO32-DIO39.
//!
//! \param dioMask is the bit-mask representation of the DIOs on which to
//! configure the event enable setting. The parameter must be a bitwise OR'ed
//! combination of the following:
//! - \ref GPIO_DIO_0_MASK
//! - ...
//! - \ref GPIO_DIO_39_MASK
//!
//! \param bitVectoredEventEnable holds the event enable setting for the
//! corresponding DIO-bits:
//! - 0 : Corresponding DIO is configured with event disabled.
//! - 1 : Corresponding DIO is configured with event enabled.
//!
//! \return None
//!
//! \sa \ref GPIOGetEventEnableDio(), \ref GPIOEnableEventDio(),
//! \ref GPIODisableEventDio()
//
//*****************************************************************************
__STATIC_INLINE void GPIOSetEventEnableMultiDio(uint64_t dioMask, uint64_t bitVectoredEventEnable)
{
    // Check the arguments.
    ASSERT((dioMask & GPIO_DIO_ALL_MASK) == dioMask);

    // Clear any pending events before enabling the mask. It is possible under
    // some circumstances for edge events to be cached in RIS, even if detection
    // is disabled. We want to make sure these events are not dispatched.
    GPIOClearEventMultiDio(dioMask & bitVectoredEventEnable);

    // Split dioMask and bitVectoredEventEnable into lower and upper 32 bits.
    uint32_t dioMask31_0                 = dioMask & 0xFFFFFFFFU;
    uint32_t dioMask63_32                = (dioMask >> 32) & 0xFFFFFFFFU;
    uint32_t bitVectoredEventEnable31_0  = bitVectoredEventEnable & 0xFFFFFFFFU;
    uint32_t bitVectoredEventEnable63_32 = (bitVectoredEventEnable >> 32) & 0xFFFFFFFFU;

    // Read the IMASK registers.
    uint32_t imask31_0  = HWREG(GPIO_BASE + GPIO_O_IMASK);
    uint32_t imask63_32 = HWREG(GPIO_BASE + GPIO_O_IMASKEX);

    // Modify the IMASK registers.
    imask31_0  = (imask31_0 & ~dioMask31_0) | (bitVectoredEventEnable31_0 & dioMask31_0);
    imask63_32 = (imask63_32 & ~dioMask63_32) | (bitVectoredEventEnable63_32 & dioMask63_32);

    // Write back the IMASK registers.
    HWREG(GPIO_BASE + GPIO_O_IMASK)   = imask31_0;
    HWREG(GPIO_BASE + GPIO_O_IMASKEX) = imask63_32;
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

#endif // ti_devices_gpio__include
