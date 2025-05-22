/******************************************************************************
 *  Filename:       ckmd.h
 *
 *  Description:    Defines and prototypes for the CKMD module.
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

#ifndef __CKMD_H__
#define __CKMD_H__

//*****************************************************************************
//
//! \addtogroup system_control_group
//! @{
//! \addtogroup ckmd_api
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
#include "../inc/hw_ckmd.h"
#include "../inc/hw_fcfg.h"

//*****************************************************************************
//
//! \name LFOSC Temperature Coefficient Temperature Limits
//!
//! If the temperature in Celsius is within the range [
//! \ref CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MIN,
//! \ref CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX ], inclusive, then the
//! temperature coefficient \ref CKMDGetLfoscMidTempCoefficientPpmPerC() shall
//! be used, otherwise \ref CKMDGetLfoscExtTempCoefficientPpmPerC().
//!
//! If \ref CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MIN is INT16_MIN, then there is no
//! lower limit for the above mentioned temperature range.
//!
//! If \ref CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX is INT16_MAX, then there is no
//! upper limit for the above mentioned temperature range.
//!
//! \{
//
//*****************************************************************************
#define CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MIN (INT16_MIN)
#define CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX (70)
//! \}

//*****************************************************************************
//! Values to pass to \ref CKMDSelectCanClock() to select the CAN clock source
//@{
//*****************************************************************************
//! Select the AFOSC as the CAN clock source
#define CKMD_CAN_CLOCK_SOURCE_CLKAF CKMD_CANCLKSEL_SRC_CLKAF
//! Select the 96MHz CLKHF (HFOSC) as the CAN clock source
#define CKMD_CAN_CLOCK_SOURCE_CLKHF CKMD_CANCLKSEL_SRC_CLKHF
//! Disable clock for CAN
#define CKMD_CAN_CLOCK_SOURCE_NONE  CKMD_CANCLKSEL_SRC_DIS
//@}

//*****************************************************************************
//! Values to pass to \ref CKMDSelectAfclk() to select the AFCLK source
//@{
//*****************************************************************************
//! Select the AFOSC as the AFCLK source
#define CKMD_AFCLK_SOURCE_CLKAF  CKMD_AFCLKSEL_SRC_CLKAF
//! Select the 96MHz CLKHF (HFOSC) as the AFCLK source
#define CKMD_AFCLK_SOURCE_CLKHF  CKMD_AFCLKSEL_SRC_CLKHF
//! Select the 48MHz reference clock (HFXT) as the AFCLK source
#define CKMD_AFCLK_SOURCE_CLKREF CKMD_AFCLKSEL_SRC_CLKREF
//! Disable the AFCLK
#define CKMD_AFCLK_SOURCE_NONE   CKMD_AFCLKSEL_SRC_DIS
//@}

//! Watchdog unlocking value
#define CKMD_WATCHDOG_UNLOCK 0x1ACCE551

//*****************************************************************************
//
// API Functions and prototypes
//
//*****************************************************************************

#ifndef DRIVERLIB_NS
//*****************************************************************************
//
// Below functions will only be compiled into the driverlib.a library and not
// the driverlib_ns.a library.
// Non-secure applications might be able to access some/all of these functions
// through veneers declared further below in the #else case.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Sets initial HFXT capacitor ramp trims
//!
//! This function sets the initial HFXT capacitor ramp trims. It will
//! overwrite the initial HFXT ramp trim values set during startup.
//!
//! This trim value is used when initially turning on the HFXT.
//!
//! \note The value for \c capTrim must be found experimentally based on the
//!       chosen crystal and desired ramp behaviour.
//!
//! \param q1CapTrim Initial Q1 capacitor trim.
//! \param q2CapTrim Initial Q2 capacitor trim.
//!
//! \return None
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetInitialCapTrim(uint32_t q1CapTrim, uint32_t q2CapTrim)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & ~(CKMD_HFXTINIT_Q1CAP_M | CKMD_HFXTINIT_Q2CAP_M);
    tmp |= (q1CapTrim << CKMD_HFXTINIT_Q1CAP_S) & CKMD_HFXTINIT_Q1CAP_M;
    tmp |= (q2CapTrim << CKMD_HFXTINIT_Q2CAP_S) & CKMD_HFXTINIT_Q2CAP_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTINIT) = tmp;
}

//*****************************************************************************
//
//! \brief Sets initial HFXT Q1 capacitor ramp trim
//!
//! This function sets the  initial HFXT Q1 capacitor ramp trim. It will
//! overwrite the initial HFXT ramp trim value set during startup.
//!
//! This trim value is used when initially turning on the HFXT.
//!
//! \note The value for \c q1Cap must be found experimentally based on the
//!       chosen crystal and desired ramp behaviour.
//!
//! \param q1Cap Initial Q1 capacitor trim.
//!
//! \return None
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetInitialQ1CapTrim(uint32_t q1Cap)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & ~CKMD_HFXTINIT_Q1CAP_M;
    tmp |= (q1Cap << CKMD_HFXTINIT_Q1CAP_S) & CKMD_HFXTINIT_Q1CAP_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTINIT) = tmp;
}

//*****************************************************************************
//
//! \brief Sets initial HFXT Q2 capacitor ramp trim
//!
//! This function sets the initial HFXT Q2 capacitor ramp trim. It will
//! overwrite the initial HFXT ramp trim value set during startup.
//!
//! This trim value is used when initially turning on the HFXT.
//!
//! \note The value for \c q2Cap must be found experimentally based on the
//!       chosen crystal and desired ramp behaviour.
//!
//! \param q2Cap Initial Q2 capacitor trim.
//!
//! \return None
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetInitialQ2CapTrim(uint32_t q2Cap)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & ~CKMD_HFXTINIT_Q2CAP_M;
    tmp |= (q2Cap << CKMD_HFXTINIT_Q2CAP_S) & CKMD_HFXTINIT_Q2CAP_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTINIT) = tmp;
}

//*****************************************************************************
//
//! \brief Sets initial HFXT IREF ramp trim
//!
//! This function sets initial HFXT IREF ramp trim. It will overwrite the
//! initial HFXT ramp trim value set during startup.
//!
//! This trim value is used when initially turning on the HFXT.
//!
//! \note The value for \c iref must be found experimentally based on the
//!       chosen crystal and desired ramp behaviour.
//!
//! \param iref Initial IREF trim.
//!
//! \return None
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetInitialIrefTrim(uint32_t iref)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & ~CKMD_HFXTINIT_IREF_M;
    tmp |= (iref << CKMD_HFXTINIT_IREF_S) & CKMD_HFXTINIT_IREF_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTINIT) = tmp;
}

//*****************************************************************************
//
//! \brief Sets initial HFXT IDAC ramp trim
//!
//! This function sets the initial HFXT IDAC ramp trim. It will overwrite the
//! initial HFXT ramp trim value set during startup.
//!
//! This trim value is used when initially turning on the HFXT.
//!
//! \note The value for \c idac must be found experimentally based on the
//!       chosen crystal and desired ramp behaviour.
//!
//! \param idac Initial IDAC trim.
//!
//! \return None
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetInitialIdacTrim(uint32_t idac)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & ~CKMD_HFXTINIT_IDAC_M;
    tmp |= (idac << CKMD_HFXTINIT_IDAC_S) & CKMD_HFXTINIT_IDAC_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTINIT) = tmp;
}

//*****************************************************************************
//
//! \brief Sets initial HFXT amplitude threshold ramp trim
//!
//! This function sets the initial HFXT amplitude threshold ramp trim. It will
//! overwrite the initial HFXT ramp trim value set during startup.
//!
//! This trim value is used when initially turning on the HFXT.
//!
//! \note The value for \c amplitudeThreshold must be found experimentally based
//!       on the chosen crystal and desired ramp behaviour.
//!
//! \param amplitudeThreshold Initial amplitude threshold trim.
//!
//! \return None
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetInitialAmplitudeThresholdTrim(uint32_t amplitudeThreshold)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & ~CKMD_HFXTINIT_AMPTHR_M;
    tmp |= (amplitudeThreshold << CKMD_HFXTINIT_AMPTHR_S) & CKMD_HFXTINIT_AMPTHR_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTINIT) = tmp;
}

//*****************************************************************************
//
//! \brief Gets initial HFXT Q1 capacitor ramp trim
//!
//! This function gets the initial HFXT Q1 capacitor ramp trim.
//!
//! \return Initial Q1 capacitor trim.
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE uint32_t CKMDGetInitialQ1CapTrim(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & CKMD_HFXTINIT_Q1CAP_M) >> CKMD_HFXTINIT_Q1CAP_S;
}

//*****************************************************************************
//
//! \brief Gets initial HFXT Q2 capacitor ramp trim
//!
//! This function gets the initial HFXT Q2 capacitor ramp trim.
//!
//! \return Initial Q2 capacitor trim.
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE uint32_t CKMDGetInitialQ2CapTrim(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & CKMD_HFXTINIT_Q2CAP_M) >> CKMD_HFXTINIT_Q2CAP_S;
}

//*****************************************************************************
//
//! \brief Gets initial HFXT IREF ramp trim
//!
//! This function gets the initial HFXT IREF ramp trim.
//!
//! \return Initial IREF trim.
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE uint32_t CKMDGetInitialIrefTrim(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & CKMD_HFXTINIT_IREF_M) >> CKMD_HFXTINIT_IREF_S;
}

//*****************************************************************************
//
//! \brief Gets initial HFXT IDAC ramp trim
//!
//! This function gets the initial HFXT IDAC ramp trim.
//!
//! \return Initial IDAC trim.
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE uint32_t CKMDGetInitialIdacTrim(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & CKMD_HFXTINIT_IDAC_M) >> CKMD_HFXTINIT_IDAC_S;
}

//*****************************************************************************
//
//! \brief Gets initial HFXT amplitude threshold ramp trim
//!
//! This function initial HFXT amplitude threshold ramp trim.
//!
//! \return Initial amplitude threshold trim.
//!
//! \sa CKMD_O_HFXTINIT for trim range
//
//*****************************************************************************
__STATIC_INLINE uint32_t CKMDGetInitialAmplitudeThresholdTrim(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFXTINIT) & CKMD_HFXTINIT_AMPTHR_M) >> CKMD_HFXTINIT_AMPTHR_S;
}

//*****************************************************************************
//
//! \brief Sets target HFXT capacitor ramp trims
//!
//! This function sets the target HFXT capacitor ramp trims. It will
//! overwrite the target HFXT ramp trim values set during startup.
//!
//! This trim value is used when target turning on the HFXT.
//!
//! \note The value for \c capTrim must be found experimentally based on the
//!       chosen crystal and desired ramp behaviour.
//!
//! \param q1CapTrim Target Q1 capacitor trim.
//! \param q2CapTrim Target Q2 capacitor trim.
//!
//! \return None
//!
//! \sa CKMD_O_HFXTTARG for trim range
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetTargetCapTrim(uint32_t q1CapTrim, uint32_t q2CapTrim)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & ~(CKMD_HFXTTARG_Q1CAP_M | CKMD_HFXTTARG_Q2CAP_M);
    tmp |= (q1CapTrim << CKMD_HFXTTARG_Q1CAP_S) & CKMD_HFXTTARG_Q1CAP_M;
    tmp |= (q2CapTrim << CKMD_HFXTTARG_Q2CAP_S) & CKMD_HFXTTARG_Q2CAP_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTTARG) = tmp;
}

//*****************************************************************************
//
//! \brief Sets target HFXT Q1 capacitor ramp trim
//!
//! This function target HFXT Q1 capacitor ramp trim. It will overwrite the
//! target HFXT ramp trim value set during startup.
//!
//! This trim value is used after HFXT has ramped to change crystal performance.
//!
//! \note The value for \c q1Cap must be found experimentally based on the
//!       chosen crystal and desired ramp behaviour.
//!
//! \param q1Cap Target Q1 capacitor trim.
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetTargetQ1CapTrim(uint32_t q1Cap)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & ~CKMD_HFXTTARG_Q1CAP_M;
    tmp |= (q1Cap << CKMD_HFXTTARG_Q1CAP_S) & CKMD_HFXTTARG_Q1CAP_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTTARG) = tmp;
}

//*****************************************************************************
//
//! \brief Sets target HFXT Q2 capacitor ramp trim
//!
//! This function target HFXT Q2 capacitor ramp trim. It will overwrite the
//! target HFXT ramp trim value set during startup.
//!
//! This trim value is used after HFXT has ramped to change crystal performance.
//!
//! \note The value for \c q2Cap must be found experimentally based on the
//!       chosen crystal and desired ramp behaviour.
//!
//! \param q2Cap Target Q2 capacitor trim.
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetTargetQ2CapTrim(uint32_t q2Cap)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & ~CKMD_HFXTTARG_Q2CAP_M;
    tmp |= (q2Cap << CKMD_HFXTTARG_Q2CAP_S) & CKMD_HFXTTARG_Q2CAP_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTTARG) = tmp;
}

//*****************************************************************************
//
//! \brief Sets target HFXT IREF ramp trim
//!
//! This function target HFXT IREF ramp trim. It will overwrite the target
//! HFXT ramp trim value set during startup.
//!
//! This trim value is used after HFXT has ramped to change crystal performance.
//!
//! \note The value for \c iref must be found experimentally based on the
//!       chosen crystal and desired ramp behaviour.
//!
//! \param iref Target IREF trim.
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetTargetIrefTrim(uint32_t iref)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & ~CKMD_HFXTTARG_IREF_M;
    tmp |= (iref << CKMD_HFXTTARG_IREF_S) & CKMD_HFXTTARG_IREF_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTTARG) = tmp;
}

//*****************************************************************************
//
//! \brief Sets target HFXT IDAC ramp trim
//!
//! This function target HFXT IDAC ramp trim. It will overwrite the target
//! HFXT ramp trim value set during startup.
//!
//! This trim value is used after HFXT has ramped to change crystal performance.
//!
//! \note The value for \c idac must be found experimentally based on the
//!       chosen crystal and desired ramp behaviour.
//!
//! \param idac Target IDAC trim.
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetTargetIdacTrim(uint32_t idac)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & ~CKMD_HFXTTARG_IDAC_M;
    tmp |= (idac << CKMD_HFXTTARG_IDAC_S) & CKMD_HFXTTARG_IDAC_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTTARG) = tmp;
}

//*****************************************************************************
//
//! \brief Sets target HFXT amplitude threshold ramp trim
//!
//! This function target HFXT amplitude threshold ramp trim. It will overwrite
//! the target HFXT ramp trim value set during startup.
//!
//! This trim value is used after HFXT has ramped to change crystal performance.
//!
//! \note The value for \c amplitudeThreshold must be found experimentally based
//!       on the chosen crystal and desired ramp behaviour.
//!
//! \param amplitudeThreshold Target amplitude threshold trim.
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetTargetAmplitudeThresholdTrim(uint32_t amplitudeThreshold)
{
    uint32_t tmp = HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & ~CKMD_HFXTTARG_AMPTHR_M;
    tmp |= (amplitudeThreshold << CKMD_HFXTTARG_AMPTHR_S) & CKMD_HFXTTARG_AMPTHR_M;
    HWREG(CKMD_BASE + CKMD_O_HFXTTARG) = tmp;
}

//*****************************************************************************
//
//! \brief Gets target HFXT Q1 capacitor ramp trim
//!
//! This function gets the target HFXT Q1 capacitor ramp trim.
//!
//! \return Target Q1 capacitor trim.
//!
//! \sa CKMD_O_HFXTTARG for trim range
//
//*****************************************************************************
__STATIC_INLINE uint32_t CKMDGetTargetQ1CapTrim(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & CKMD_HFXTTARG_Q1CAP_M) >> CKMD_HFXTTARG_Q1CAP_S;
}

//*****************************************************************************
//
//! \brief Gets target HFXT Q2 capacitor ramp trim
//!
//! This function gets the target HFXT Q2 capacitor ramp trim.
//!
//! \return Target Q2 capacitor trim.
//!
//! \sa CKMD_O_HFXTTARG for trim range
//
//*****************************************************************************
__STATIC_INLINE uint32_t CKMDGetTargetQ2CapTrim(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & CKMD_HFXTTARG_Q2CAP_M) >> CKMD_HFXTTARG_Q2CAP_S;
}

//*****************************************************************************
//
//! \brief Gets target HFXT IREF ramp trim
//!
//! This function gets the target HFXT IREF ramp trim.
//!
//! \return Target IREF trim.
//!
//! \sa CKMD_O_HFXTTARG for trim range
//
//*****************************************************************************
__STATIC_INLINE uint32_t CKMDGetTargetIrefTrim(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & CKMD_HFXTTARG_IREF_M) >> CKMD_HFXTTARG_IREF_S;
}

//*****************************************************************************
//
//! \brief Gets target HFXT IDAC ramp trim
//!
//! This function gets the target HFXT IDAC ramp trim.
//!
//! \return Target IDAC trim.
//!
//! \sa CKMD_O_HFXTTARG for trim range
//
//*****************************************************************************
__STATIC_INLINE uint32_t CKMDGetTargetIdacTrim(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & CKMD_HFXTTARG_IDAC_M) >> CKMD_HFXTTARG_IDAC_S;
}

//*****************************************************************************
//
//! \brief Gets target HFXT amplitude threshold ramp trim
//!
//! This function target HFXT amplitude threshold ramp trim.
//!
//! \return Target amplitude threshold trim.
//!
//! \sa CKMD_O_HFXTTARG for trim range
//
//*****************************************************************************
__STATIC_INLINE uint32_t CKMDGetTargetAmplitudeThresholdTrim(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFXTTARG) & CKMD_HFXTTARG_AMPTHR_M) >> CKMD_HFXTTARG_AMPTHR_S;
}

//*****************************************************************************
//
//! \brief Enable high performance clock buffer
//
//*****************************************************************************
__STATIC_INLINE void CKMDEnableHighPerformanceClockBuffer(void)
{
    HWREG(CKMD_BASE + CKMD_O_HFXTCTL) |= CKMD_HFXTCTL_HPBUFEN;
}

//*****************************************************************************
//
//! \brief Disable high performance clock buffer
//
//*****************************************************************************
__STATIC_INLINE void CKMDDisableHighPerformanceClockBuffer(void)
{
    HWREG(CKMD_BASE + CKMD_O_HFXTCTL) &= ~CKMD_HFXTCTL_HPBUFEN;
}

//*****************************************************************************
//
//! \brief Gets the worst-case LFOSC frequency jump due to RTN.
//!
//! \return The absolute value of the worst-case jump due to RTN in ppm.
//
//*****************************************************************************
__STATIC_INLINE uint_least16_t CKMDGetLfoscRtnPpm(void)
{
    // Only App trims revision 5 and newer has a ppmRtn field. For older
    // revisions use a default value of 20 (equivalent to 600ppm)
    uint8_t ppmRtn = 20;
    if (fcfg->appTrims.revision >= 0x5)
    {
        ppmRtn = fcfg->appTrims.cc27xx.misc0.lfoscPpmRtn;
    }

    // The ppmTempMid field uses units of 30ppm, convert to ppm
    return ppmRtn * 30;
}

//*****************************************************************************
//
//! \brief Gets the the worst-case LFOSC temperature coefficient in the "middle"
//! temperature range.
//!
//! This function can be used to determine the the worst-case LFOSC temperature
//! coefficient in units of ppm/C in the temperature range [
//! \ref CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MIN,
//! \ref CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX ].
//!
//! \return The absolute value of worst-case temperature coefficient in ppm/C.
//
//*****************************************************************************
__STATIC_INLINE uint_least16_t CKMDGetLfoscMidTempCoefficientPpmPerC(void)
{
    // Only App trims revision 5 and newer has a ppmTempMid field. For older
    // revisions use a default value of 20 (equivalent to 400ppm/C)
    uint8_t ppmTempMid = 20;
    if (fcfg->appTrims.revision >= 0x5)
    {
        ppmTempMid = fcfg->appTrims.cc27xx.misc0.lfoscPpmTempMid;
    }

    // The ppmTempMid field uses units of 20ppm/C, convert to ppm/C
    return ppmTempMid * 20;
}

//*****************************************************************************
//
//! \brief Gets the the worst-case LFOSC temperature coefficient in the
//! "extended" temperature range.
//!
//! This function can be used to determine the the worst-case LFOSC temperature
//! coefficient in units of ppm/C when the temperature is outside of the
//! temperature range [
//! \ref CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MIN,
//! \ref CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX ].
//!
//! \return The absolute value of worst-case temperature coefficient in ppm/C.
//
//*****************************************************************************
__STATIC_INLINE uint_least16_t CKMDGetLfoscExtTempCoefficientPpmPerC(void)
{
    // Only App trims revision 5 and newer has a ppmTempExt field. For older
    // revisions use a default value of 20 (equivalent to 400ppm/C)
    uint8_t ppmTempExt = 20;
    if (fcfg->appTrims.revision >= 0x5)
    {
        ppmTempExt = fcfg->appTrims.cc27xx.misc0.lfoscPpmTempExt;
    }

    // The ppmTempExt field uses units of 20ppm/C, convert to ppm/C
    return ppmTempExt * 20;
}

//*****************************************************************************
//
//! \brief Unlock write access to the Watchdog
//!
//! The function will wait until the Watchdog has actually been unlocked, before
//! returning.
//
//*****************************************************************************
__STATIC_INLINE void CKMDUnlockWatchdog(void)
{
    // Unlock the Watchdog
    HWREG(CKMD_BASE + CKMD_O_LOCK) = CKMD_WATCHDOG_UNLOCK;

    // Make sure the Watchdog is unlocked before continuing
    while (HWREG(CKMD_BASE + CKMD_O_LOCK) == 1) {}
}

//*****************************************************************************
//
//! \brief Lock write access to the Watchdog
//
//*****************************************************************************
__STATIC_INLINE void CKMDLockWatchdog(void)
{
    HWREG(CKMD_BASE + CKMD_O_LOCK) = 0x0;
}

//*****************************************************************************
//
//! \brief Set watchdog counter value.
//!
//! Calling this function will immediately start (or restart) the counter. It
//! will count down from the \c value
//!
//! \param value is the value to write to the Watchdog counter.
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetWatchdogCounter(uint32_t value)
{
    HWREG(CKMD_BASE + CKMD_O_CNT) = value;
}

//*****************************************************************************
//
//! \brief Configure if the Watchdog should stop when the CPU is halted by a
//! debugger
//!
//! \param stopWhenCpuIsHalted if true, the Watchdog will stop counting while
//! the CPU is stopped by a debugger. If false, the Watchdog will continue
//! counting while the CPU is stopped by a debugger.
//
//*****************************************************************************
__STATIC_INLINE void CKMDSetWatchdogDebugConfig(bool stopWhenCpuIsHalted)
{
    HWREG(CKMD_BASE + CKMD_O_TEST) = stopWhenCpuIsHalted ? CKMD_TEST_STALLEN_EN : CKMD_TEST_STALLEN_DIS;
}

//*****************************************************************************
//
//! \brief Selects the clock source for the CAN peripheral
//!
//! \param source is the clock source to select:
//!  - \ref CKMD_CAN_CLOCK_SOURCE_CLKAF
//!  - \ref CKMD_CAN_CLOCK_SOURCE_CLKHF
//!  - \ref CKMD_CAN_CLOCK_SOURCE_NONE
//
//*****************************************************************************
__STATIC_INLINE void CKMDSelectCanClock(uint32_t source)
{
    HWREG(CKMD_BASE + CKMD_O_CANCLKSEL) = source & CKMD_CANCLKSEL_SRC_M;
}

//*****************************************************************************
//
//! \brief Selects the clock source for AFCLK
//!
//! \param source is the clock source to select:
//! - \ref CKMD_AFCLK_SOURCE_CLKAF
//! - \ref CKMD_AFCLK_SOURCE_CLKHF
//! - \ref CKMD_AFCLK_SOURCE_CLKREF
//! - \ref CKMD_AFCLK_SOURCE_NONE
//
//*****************************************************************************
__STATIC_INLINE void CKMDSelectAfclk(uint32_t source)
{
    HWREG(CKMD_BASE + CKMD_O_AFCLKSEL) = source & CKMD_AFCLKSEL_SRC_M;
}

#else
//*****************************************************************************
//
// Secure-only API functions available from non-secure context through
// veneers.
//
// These functions must be linked in from from the secure veneer library.
//
//*****************************************************************************

extern void CKMDEnableHighPerformanceClockBuffer_veneer(void);
    #define CKMDEnableHighPerformanceClockBuffer  CKMDEnableHighPerformanceClockBuffer_veneer

extern void CKMDDisableHighPerformanceClockBuffer_veneer(void);
    #define CKMDDisableHighPerformanceClockBuffer CKMDDisableHighPerformanceClockBuffer_veneer

extern uint_least16_t CKMDGetLfoscRtnPpm_veneer(void);
    #define CKMDGetLfoscRtnPpm                    CKMDGetLfoscRtnPpm_veneer

extern uint_least16_t CKMDGetLfoscMidTempCoefficientPpmPerC_veneer(void);
    #define CKMDGetLfoscMidTempCoefficientPpmPerC CKMDGetLfoscMidTempCoefficientPpmPerC_veneer

extern uint_least16_t CKMDGetLfoscExtTempCoefficientPpmPerC_veneer(void);
    #define CKMDGetLfoscExtTempCoefficientPpmPerC CKMDGetLfoscExtTempCoefficientPpmPerC_veneer

extern void CKMDUnlockWatchdog_veneer(void);
    #define CKMDUnlockWatchdog                    CKMDUnlockWatchdog_veneer

extern void CKMDLockWatchdog_veneer(void);
    #define CKMDLockWatchdog                      CKMDLockWatchdog_veneer

extern void CKMDSetWatchdogCounter_veneer(uint32_t value);
    #define CKMDSetWatchdogCounter                CKMDSetWatchdogCounter_veneer

extern void CKMDSetWatchdogDebugConfig_veneer(bool stopWhenCpuIsHalted);
    #define CKMDSetWatchdogDebugConfig            CKMDSetWatchdogDebugConfig_veneer

extern void CKMDSelectCanClock_veneer(uint32_t source);
    #define CKMDSelectCanClock                    CKMDSelectCanClock_veneer

extern void CKMDSelectAfclk_veneer(uint32_t source);
    #define CKMDSelectAfclk                       CKMDSelectAfclk_veneer

#endif

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

#endif // __CKMD_H__
