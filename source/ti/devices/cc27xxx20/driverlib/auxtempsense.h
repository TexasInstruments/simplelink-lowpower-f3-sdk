/******************************************************************************
 *  Filename:       auxtempsense.h
 *
 *  Description:    Defines and prototypes for reading the high accuracy
 *                  temperature sensor
 *
 *  Copyright (c) 2025-2026 Texas Instruments Incorporated
 *  All rights reserved.
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
 *  3) Neither the name of the ORGANIZATION nor the names of its contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission.
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

#ifndef ti_devices_auxtempsense__include
#define ti_devices_auxtempsense__include

//*****************************************************************************
//
//! \addtogroup peripheral_group
//! @{
//! \addtogroup auxtempsense_api
//! @{
//
//*****************************************************************************

#include <stdint.h>

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

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
//! \brief Read temperature from the high accuracy auxiliary temperature sensor.
//!
//! This function configures the ADC, performs multiple temperature measurements,
//! and converts the measured voltage to temperature.
//!
//! \return Temperature in degrees Celsius with 4 fractional bits (Q28.4 format).
//!         To get the actual temperature, divide the return value by 16.
//!
//*****************************************************************************
extern int32_t AuxTempSenseGetTemperature(void);

#else
//*****************************************************************************
//
// Secure-only API functions available from non-secure context through
// veneers.
//
// These functions must be linked in from from the secure veneer library.
//
//*****************************************************************************

extern int32_t AuxTempSenseGetTemperature_veneer(void);
    #define AuxTempSenseGetTemperature AuxTempSenseGetTemperature_veneer
#endif // DRIVERLIB_NS

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

#endif // ti_devices_auxtempsense__include
