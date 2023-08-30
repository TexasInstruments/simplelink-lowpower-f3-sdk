/******************************************************************************
 *  Filename:       tempdiode.c
 *
 *  Description:    Driverlib for the high accuracy temperature diode
 *
 *  Copyright (c) 2023, Texas Instruments Incorporated
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

#include <stdint.h>

#include "adc.h"
#include "hapi.h"
#include "tempdiode.h"
#include "interrupt.h"

#include "../inc/hw_clkctl.h"
#include "../inc/hw_fcfg.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_pmud.h"
#include "../inc/hw_sys0.h"
#include "../inc/hw_types.h"

#ifdef __IAR_SYSTEMS_ICC__
__ramfunc static uint32_t TempDiodeRamHWREG(uint32_t address, uint32_t data);
#elif (defined(__GNUC__) || defined(__clang__))
static uint32_t TempDiodeRamHWREG(uint32_t address, uint32_t data) __attribute__((section(".TI.ramfunc"), noinline));
#else
    #error Unsupported Compiler
#endif

static void enableADC(void);
static int32_t voltageToTemp(uint32_t microVolts);

/* Temporary PMUD PREFSYS register definition. */
#ifdef PMUD_O_PREFSYS
    #error "PMUD_O_PREFSYS defined in DOC release! Remove definitition below."
#endif

#define PMUD_O_PREFSYS     0x00000080U
#define PMUD_PREFSYS_TEST2 0x00000004U

//*****************************************************************************
//
// Writes data to register while executing from RAM
//
//*****************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
__ramfunc static uint32_t TempDiodeRamHWREG(uint32_t address, uint32_t data)
{
    HWREG(address) = data;
    /* Wait at least 11 us before proceeding with any flash operations */
    HapiWaitUs(11);
    return HWREG(address);
}
#elif (defined(__GNUC__) || defined(__clang__))
static uint32_t TempDiodeRamHWREG(uint32_t address, uint32_t data)
{
    HWREG(address) = data;
    /* Wait at least 11 us before proceeding with any flash operations */
    HapiWaitUs(11);
    return HWREG(address);
}
#else
    #error Unsupported Compiler
#endif

//*****************************************************************************
//
// Enable and configure the ADC. Parameters are the same as used during trim-procedure
//
//*****************************************************************************
static void enableADC(void)
{
    /* Enable ADC peripheral */
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET0) = CLKCTL_CLKENSET0_ADC0;

    /* Configure ADC CTL-register 0 */
    ADCSetMemctlRange(0, 0);

    /* Set maximum sampling duration for highest accuracy */
    ADCSetSampleDuration(ADC_CLOCK_DIVIDER_48, 1023);

    /* Use 12-bit resolution */
    ADCSetResolution(ADC_RESOLUTION_12_BIT);

    /* Use internal 1.4V reference, sample on internal channel 14 (VA_ATEST1) */
    ADCSetInput(ADC_FIXED_REFERENCE_1V4, ADC_MEMCTL0_CHANSEL_CHAN_14, 0);

    /* Adjust ADC to 1.4V reference */
    ADCSetAdjustmentOffset(ADC_FIXED_REFERENCE_1V4);

    /* Perform single conversion */
    ADCSetSequence(ADC_SEQUENCE_SINGLE);
}

//*****************************************************************************
//
// Converts a voltage (uV) measured across the diode to temperature (degC)
//
//*****************************************************************************
static int32_t voltageToTemp(uint32_t microVolts)
{
    /* Convert to millivolts with 4 fractional bits,
     * which is the same format as stored in fcfg
     */
    int32_t milliVolts = ((microVolts << 4) + 500) / 1000;

    /* Get diode voltage drop at 30 degC from fcfg */
    int32_t voltageAt30Deg = fcfg->appTrims.cc23x0r5.auxDiodeCal30C.auxDiodeVoltage -
                             fcfg->appTrims.cc23x0r5.auxDiodeCal30C.auxDiodeGnd;

    /* Get diode voltage drop at 125 degC from fcfg */
    int32_t voltageAt125Deg = fcfg->appTrims.cc23x0r5.auxDiodeCal125C.auxDiodeVoltage -
                              fcfg->appTrims.cc23x0r5.auxDiodeCal125C.auxDiodeGnd;

    /* Fit measured voltage onto the line defined by the two points in fcfg */
    int32_t divisor = voltageAt125Deg - voltageAt30Deg;

    int32_t temperature = 30 + ((milliVolts - voltageAt30Deg) * (125 - 30) + (divisor / 2)) / divisor;

    return temperature;
}

//*****************************************************************************
//
// Reads temperature from high accuracy temperature diode
//
//*****************************************************************************
int32_t TempDiodeGetTemp(void)
{
    uint32_t diodeVoltage = 0;
    bool intAlreadyDisabled;

    /* Unlock ATESTCFG register and connect VR_ATEST to VA_ATEST */
    HWREG(SYS0_BASE + SYS0_O_ATESTCFG) = 0x5A000000 | SYS0_ATESTCFG_VR2VA1 | SYS0_ATESTCFG_VR2VA0;

    /* Disable interrupts and keep track of whether interrupts were already
     * disabled or not
     */
    intAlreadyDisabled = IntDisableMaster();

    /* Connect 1uA IREF to test bus. This is done via a function executing from
     * RAM, while interrupts are disabled, to ensure that no flash-operations
     * are active when the reference current is enabled.
     */
    TempDiodeRamHWREG(PMUD_BASE + PMUD_O_PREFSYS, PMUD_PREFSYS_TEST2);

    /* If interrupts were already disabled, then they should be left disabled.
     * If interrupts were already enabled, then they should be re-enabled here.
     */
    if (intAlreadyDisabled == false)
    {
        IntEnableMaster();
    }

    enableADC();

    /* Measure the high side of the diode */
    HWREG(SYS0_BASE + SYS0_O_TSENSCFG) = SYS0_TSENSCFG_SEL_VALUE;

    /* Do 4 ADC conversions for averaging */
    for (uint32_t i = 0; i < 4; i++)
    {
        ADCManualTrigger();
        CPUDelay(3);
        diodeVoltage += ADCReadResult(0);
    }

    /* Measure ground (low side of the diode) */
    HWREG(SYS0_BASE + SYS0_O_TSENSCFG) = SYS0_TSENSCFG_SEL_GND;

    /* Do 4 ADC conversions for averaging */
    for (uint32_t i = 0; i < 4; i++)
    {
        ADCManualTrigger();
        CPUDelay(3);
        diodeVoltage -= ADCReadResult(0);
    }

    /* Get average of 4 readings */
    diodeVoltage >>= 2;

    /* Convert raw reading to microvolts */
    diodeVoltage = ADCValueToMicrovolts(diodeVoltage, ADC_RESOLUTION_12_BIT, 1400000);

    /* Disable ADC */
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR0) = CLKCTL_CLKENCLR0_ADC0;

    /* Reset ATB */
    HWREG(SYS0_BASE + SYS0_O_ATESTCFG) = 0x5A00000F;

    /* Disable temperature diode measurement */
    HWREG(SYS0_BASE + SYS0_O_TSENSCFG) &= ~SYS0_TSENSCFG_SEL_M;

    /* Disconnect all test reference signals */
    intAlreadyDisabled = IntDisableMaster();
    TempDiodeRamHWREG(PMUD_BASE + PMUD_O_PREFSYS, 0);
    if (intAlreadyDisabled == false)
    {
        IntEnableMaster();
    }

    return voltageToTemp(diodeVoltage);
}
