/******************************************************************************
 *  Filename:       auxtempsense.c
 *
 *  Description:    Driver for the high accuracy temperature sensor
 *
 *  Copyright (c) 2025-2026, Texas Instruments Incorporated
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
#include <stdbool.h>
#include <stdint.h>

#include "adc.h"
#include "auxtempsense.h"
#include "interrupt.h"
#include "cpu.h"

#include "../inc/hw_clkctl.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_sys0.h"
#include "../inc/hw_types.h"
#include "../inc/hw_fcfg.h"

//*****************************************************************************
//
// Values
//
//*****************************************************************************

// ATESTCFG register's write permission value (defined here until available in
// hw_sys0.h).
#define SYS0_ATESTCFG_KEY_UNLOCK (0x5AU << SYS0_ATESTCFG_KEY_S)

// There is a delay of 9 cycles on CC23X1 between writing the SC_START bit, and
// the BUSY-bit going high. If we start polling too early, we will miss it.
// Delay 3 loops, where each loop is minimum 3 cycles.
#define ADC_MAX_START_TO_BUSY_DELAY 3

// Default configuration for the Analog Test Bus (ATB) when resetting to a safe
// state.
// This configuration is used to ensure all test
// points are properly grounded and all signal paths are disconnected after
// temperature measurements are complete.
//
// The configuration consists of the following switch settings:
//
// - SYS0_ATESTCFG_SHTVR0_CLOSE: Shorts VR_ATEST_A0 (Voltage Reference A0) to
//   ground. This discharges any residual voltage on the reference line and
//   prevents floating inputs that could cause unpredictable behavior or
//   power consumption.
//
// - SYS0_ATESTCFG_SHTVR1_CLOSE: Shorts VR_ATEST_A1 (Voltage Reference A1) to
//   ground. Similar to SHTVR0, this ensures the second reference line is
//   properly discharged and grounded.
//
// - SYS0_ATESTCFG_SHTVA0_CLOSE: Shorts VA_ATEST_A0 (Voltage Analog A0) to
//   ground. This discharges the analog signal path that was used for
//   temperature sensing, preventing any residual voltage from affecting
//   future measurements.
//
// - SYS0_ATESTCFG_SHTVA1_CLOSE: Shorts VA_ATEST_A1 (Voltage Analog A1) to
//   ground. This ensures the second analog signal path is properly discharged.
//
// - SYS0_ATESTCFG_VR2VA1_OPEN: Opens the isolation switch between VR_ATEST_A1
//   and VA_ATEST_A1. This disconnects the voltage reference from the analog
//   voltage path, ensuring complete isolation between different parts of the
//   circuit.
//
// - SYS0_ATESTCFG_VA2VA0_OPEN: Opens the isolation switch between VA_ATEST_A0
//   and VA_PAD_A0. This disconnects the internal analog voltage path from the
//   external pad, preventing external interference with the internal circuitry.
//
// - SYS0_ATESTCFG_VA2VA1_OPEN: Opens the isolation switch between VA_ATEST_A1
//   and VA_PAD_A1. Similar to VA2VA0, this isolates the second analog path
//   from external connections.
#define AUXTEMPSENSE_ATEST_SWITCH_CFG_DEFAULT                                               \
    (SYS0_ATESTCFG_SHTVR0_CLOSE | SYS0_ATESTCFG_SHTVR1_CLOSE | SYS0_ATESTCFG_SHTVA0_CLOSE | \
     SYS0_ATESTCFG_SHTVA1_CLOSE | SYS0_ATESTCFG_VR2VA1_OPEN | SYS0_ATESTCFG_VA2VA0_OPEN | SYS0_ATESTCFG_VA2VA1_OPEN)

// Configuration for the Analog Test Bus (ATB) when performing temperature
// sensing.
// This configuration is used to establish the
// necessary signal routing paths for accurate temperature measurements.
//
// The configuration consists of the following switch settings:
//
// - SYS0_ATESTCFG_VR2VA0_CLOSE: Closes the isolation switch between VR_ATEST_A0
//   and VA_ATEST_A0. This critical connection routes the temperature sensor's
//   reference voltage to the analog signal path, allowing the ADC to measure
//   the voltage that varies linearly with temperature.
//
// - SYS0_ATESTCFG_VA2VA0_CLOSE: Closes the isolation switch between VA_ATEST_A0
//   and VA_PAD_A0. This connects the internal analog signal path to the
//   appropriate pad, completing the circuit needed for temperature measurement.
//
// - SYS0_ATESTCFG_SHTVR0_OPEN: Opens the shorting switch that would otherwise
//   ground VR_ATEST_A0, allowing the reference voltage to pass through.
//
// - SYS0_ATESTCFG_SHTVR1_OPEN: Opens the shorting switch for VR_ATEST_A1,
//   preventing any unwanted grounding of this reference line during measurement.
//
// - SYS0_ATESTCFG_SHTVA0_OPEN: Opens the shorting switch for VA_ATEST_A0,
//   allowing the analog signal to pass through without being grounded.
//
// - SYS0_ATESTCFG_SHTVA1_OPEN: Opens the shorting switch for VA_ATEST_A1,
//   preventing any unwanted grounding of this analog line during measurement.
//
// - SYS0_ATESTCFG_VR2VA1_OPEN: Keeps the isolation switch between VR_ATEST_A1
//   and VA_ATEST_A1 open, as this path is not used for the temperature
//   measurement.
//
// - SYS0_ATESTCFG_VA2VA1_OPEN: Keeps the isolation switch between VA_ATEST_A1
//   and VA_PAD_A1 open, as this path is not used for the temperature
//   measurement.
#define AUXTEMPSENSE_ATEST_SWITCH_CFG_TO_ADC13                                                                         \
    (SYS0_ATESTCFG_VR2VA0_CLOSE | SYS0_ATESTCFG_VA2VA0_CLOSE | SYS0_ATESTCFG_SHTVR0_OPEN | SYS0_ATESTCFG_SHTVR1_OPEN | \
     SYS0_ATESTCFG_SHTVA0_OPEN | SYS0_ATESTCFG_SHTVA1_OPEN | SYS0_ATESTCFG_VR2VA1_OPEN | SYS0_ATESTCFG_VA2VA1_OPEN)

//*****************************************************************************
//
// Forward declarations
//
//*****************************************************************************
static void AuxTempSenseEnableADC(void);
static int32_t AuxTempSenseVoltageToTempQ4(uint32_t adcMicroVolts);
static void AuxTempSenseEnableADCClock(void);
static void AuxTempSenseDisableADCClock(void);
static void AuxTempSenseResetAtest(void);
static void AuxTempSenseConfigureAtest(void);
static void AuxTempSenseEnableSensor(void);
static void AuxTempSenseDisableSensor(void);

//*****************************************************************************
//
//! \brief Enable and configure the ADC with parameters optimized for temperature
//! measurement.
//!
//! Uses the same configuration as during factory trim-procedure.
//!
//! \return None
//!
//*****************************************************************************
static void AuxTempSenseEnableADC(void)
{
    // Enable ADC peripheral
    AuxTempSenseEnableADCClock();

    // Disable ADC conversions to allow changes to the ADC configuration
    ADCDisableConversion();

    // Configure ADC CTL-register 0
    ADCSetMemctlRange(0, 0);

    // Set maximum sampling duration for highest accuracy
    ADCSetSampleDuration(ADC_CLOCK_DIVIDER_48, 1023);

    // Use 12-bit resolution
    ADCSetResolution(ADC_RESOLUTION_12_BIT);

    // Use internal 1.4V reference, sample on internal channel 13
    ADCSetInput(ADC_FIXED_REFERENCE_1V4, ADC_MEMCTL0_CHANSEL_CHAN_13, 0);

    // Adjust ADC to 1.4V reference
    ADCSetAdjustmentOffset(ADC_FIXED_REFERENCE_1V4);

    // Perform single conversion
    ADCSetSequence(ADC_SEQUENCE_SINGLE);

    // Set trigger source to software
    ADCSetTriggerSource(ADC_TRIGGER_SOURCE_SOFTWARE);

    // Set sampling mode to automatic, to use the sample duration configured
    // above with ADCSetSampleDuration()
    ADCSetSamplingMode(ADC_SAMPLE_MODE_AUTO);

    // Enable conversion. The ADC will wait for the software trigger
    ADCEnableConversion();
}

//*****************************************************************************
//
//! \brief Convert measured sensor voltage to temperature with 4 fractional bits.
//!
//! \param adcMicroVolts The measured sensor voltage in microvolts.
//!
//! \return Temperature in degrees Celsius with 4 fractional bits (Q28.4 format).
//!         To get the actual temperature, divide the return value by 16.
//!
//*****************************************************************************
static int32_t AuxTempSenseVoltageToTempQ4(uint32_t adcMicroVolts)
{
    // The Aux Temperature Sensor is a high accuracy temperature sensor, with
    // voltage (in Volts) that varies linearly with temperature (in DegC)
    // according to the following equation:
    // v_out = 0.001885*temp + (v_offset - 30 * 0.001885)
    // where v_offset is the voltage at 30 DegC, which varies from device to
    // device.
    //
    // The v_offset value is stored in FCFG register's AUX_TEMP_SENS_30C field.
    // Rearranging the equation to solve for temperature gives:
    // temp = (v_out - v_offset + 30 * 0.001885) / 0.001885
    //
    // For better accuracy, the equation is implemented in micro-volts and
    // degrees C:
    // temp = (((10^6) * v_out) - ((10^6) * v_offset) +
    //        ((10^6) * (30 * 0.001885))) / ((10^6) * 0.001885)
    // temp = (v_out_uv - v_offset_uv + 56550) / 1885
    // where, v_out_uv    is the measured adc voltage in microvolts,
    //        v_offset_uv is the offset voltage in microvolts,
    //        56550       is the voltage difference in microvolts for
    //                    30DegC (30 * 1885 uV),
    //        1885 is the slope in microvolts per DegC (1885 uV/DegC).

    // Read calibration value from factory configuration (FCFG)
    // v_offset is stored as a 16-bit value with 4 fractional bits (Q12.4 format)
    int32_t millivoltsAt30C = fcfg->appTrims.cc23x1.auxTempSens.auxTempSens30C;

    // Convert from 16-bit representation (with 4 fractional bits) to microvolts
    // v_offset_uv = (millivoltsAt30C / 16) * 1000
    //             = millivoltsAt30C * (1000 / 16)
    //             = millivoltsAt30C * 62.5
    // To avoid floating point, use integer arithmetic:
    //             = (millivoltAt30C * 62) + (millivoltAt30C / 2)
    //             = (millivoltAt30C * 62) + (millivoltAt30C >> 1)
    int32_t voltageOffsetInMicroVolts     = (millivoltsAt30C * 62) + (millivoltsAt30C >> 1);
    int32_t microVoltsDifferenceFor30DegC = 56550;
    int32_t slopeInMicroVoltPerDegC       = 1885;

    // Compute signed numerator:
    // delta_Voltage = (v_out_uv - v_offset_uv + voltageDifferenceFor30DegC)
    // This value may be negative, so use int32_t.
    int32_t deltaVoltageInMicroVolts = adcMicroVolts - voltageOffsetInMicroVolts + microVoltsDifferenceFor30DegC;

    // Scale to Q4 (multiply by 16) before division by slope
    // tempInQ4Format = (delta_Voltage * 16) / slopeInMicroVoltPerDegC
    // The result is in Q4 format (temperature * 16).
    int32_t numerator = deltaVoltageInMicroVolts * 16;

    // Apply rounding to nearest integer when dividing by slopeInMicroVoltPerDegC
    if (numerator >= 0)
    {
        numerator += (slopeInMicroVoltPerDegC / 2);
    }
    else
    {
        numerator -= (slopeInMicroVoltPerDegC / 2);
    }

    // Result in Q4 (temp * 16)
    int32_t tempInQ4Format = numerator / slopeInMicroVoltPerDegC;

    // Returned value must be divided by 16 to get degrees Celsius
    return tempInQ4Format;
}

//*****************************************************************************
//
// Configure ADC, perform multiple measurements and return the temperature.
//
//*****************************************************************************
int32_t AuxTempSenseGetTemperature(void)
{
    uint32_t sensorVoltage = 0;
    bool intAlreadyDisabled;

    // Unlock ATESTCFG register
    AuxTempSenseConfigureAtest();

    // Enable aux temperature sensor sensor
    AuxTempSenseEnableSensor();

    // Disable interrupts and keep track of whether interrupts were already
    // disabled or not. This creates a critical section to ensure accurate ADC
    // measurements by preventing any interrupt handlers from disrupting the
    // precise timing required for the temperature sensing sequence.
    intAlreadyDisabled = IntDisableMaster();

    // Enable and configure ADC for voltage measurement across the sensor sensor
    AuxTempSenseEnableADC();

    // Perform a dummy-read of the ADC for better settling
    ADCEnableConversion();
    ADCStartConversion();
    // CPU Delay to wait for ADC conversion to complete
    CPUDelay(ADC_MAX_START_TO_BUSY_DELAY);
    ADCReadResult(0);

    // Do 4 ADC conversions for averaging
    for (uint32_t i = 0; i < 4; i++)
    {
        ADCEnableConversion();
        ADCStartConversion();
        CPUDelay(ADC_MAX_START_TO_BUSY_DELAY);
        sensorVoltage += ADCReadResult(0);
    }

    // Calculate an average of the 4 readings, rounded to the nearest integer
    sensorVoltage = (sensorVoltage + 2) / 4;

    // Adjust ADC value to compensate for device/reference specific gain
    uint16_t gain = ADCGetAdjustmentGain(ADC_FIXED_REFERENCE_1V4);
    sensorVoltage = ADCAdjustValueForGain(sensorVoltage, ADC_RESOLUTION_12_BIT, gain);

    // Convert raw reading to microvolts
    // Gain has been adjusted to 1.4V reference. 1.4V = 1400000uV
    sensorVoltage = ADCValueToMicrovolts(sensorVoltage, ADC_RESOLUTION_12_BIT, 1400000);

    // Disable ADC
    AuxTempSenseDisableADCClock();

    // Reset Analog Test Bus
    AuxTempSenseResetAtest();

    // Disable aux temperature sensor measurement
    AuxTempSenseDisableSensor();

    // Re-enable interrupts if they were not already disabled before entering
    // this function. This preserves the interrupt state that existed before
    // the critical section, maintaining proper system behavior.
    if (intAlreadyDisabled == false)
    {
        IntEnableMaster();
    }

    return AuxTempSenseVoltageToTempQ4(sensorVoltage);
}

//*****************************************************************************
//
// Secure implementations of ADC Clock Control functions
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Enable ADC clock.
//!
//! This function enables the ADC peripheral clock by setting the appropriate
//! bit in the clock control register.
//!
//! \return None
//!
//*****************************************************************************
static void AuxTempSenseEnableADCClock(void)
{
    // Enable ADC peripheral by setting ADC Clock using clock control register
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET0) = CLKCTL_CLKENSET0_ADC0;
}

//*****************************************************************************
//
//! \brief Disable ADC clock.
//!
//! This function disables the ADC peripheral clock by clearing the appropriate
//! bit in the clock control register.
//!
//! \return None
//!
//*****************************************************************************
static void AuxTempSenseDisableADCClock(void)
{
    // Disable ADC peripheral by clearing ADC Clock using clock control register
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR0) = CLKCTL_CLKENCLR0_ADC0;
}

//*****************************************************************************
//
// Secure implementations of System's Analog Test Bus Configuration functions
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Reset the Analog Test Bus (ATB) to a safe default state.
//!
//! This function resets the Analog Test Bus by configuring the ATESTCFG register
//! to a safe default state. It ensures that all test signals are properly
//! disconnected and grounded after temperature measurements are complete,
//! preventing any residual voltages or floating inputs that could affect
//! system behavior or power consumption.
//!
//! The function performs the following actions:
//! - Unlocks the ATESTCFG register for writing
//! - Selects VDDBOOST as the supply voltage for the ATB switches
//! - Configures all shorting switches to ground (SHTVR0, SHTVR1, SHTVA0, SHTVA1)
//! - Opens all isolation switches between signal paths (VR2VA1, VA2VA0, VA2VA1)
//!
//! \return None
//!
//*****************************************************************************
static void AuxTempSenseResetAtest(void)
{
    // Reset the Analog Test Bus (ATB) by writing to the ATESTCFG register with:
    //
    // - SYS0_ATESTCFG_KEY_UNLOCK: Write the unlock key (0x5A) to enable register
    //   modification. This is required for any write to the ATESTCFG register.
    //
    // - SYS0_ATESTCFG_VSEL_VDDBST: Select VDDBOOST as the supply voltage for
    //   the ATEST switches. VDDBOOST provides a more stable supply voltage
    //   compared to VDDS, which may fluctuate under varying system loads.
    //   This ensures reliable operation of the switches during the reset process.
    //
    // - AUXTEMPSENSE_ATEST_SWITCH_CFG_DEFAULT: Apply the default switch
    //   configuration that grounds all test points (by closing shorting switches)
    //   and disconnects all signal paths (by opening isolation switches).
    //   This returns the ATB to a known safe state, preventing any floating
    //   inputs or residual voltages from affecting future operations.
    HWREG(SYS0_BASE + SYS0_O_ATESTCFG) = SYS0_ATESTCFG_KEY_UNLOCK | SYS0_ATESTCFG_VSEL_VDDBST |
                                         AUXTEMPSENSE_ATEST_SWITCH_CFG_DEFAULT;
}

//*****************************************************************************
//
// Secure implementations of Aux Temperature Sensor functions
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Configure the Analog Test Bus (ATB) for temperature sensing operations.
//!
//! This function configures the Analog Test Bus by setting appropriate bits
//! in the ATESTCFG register to establish the necessary signal routing paths
//! for accurate auxiliary temperature sensor measurements. It creates a specific
//! signal path that allows the ADC to measure the voltage across the temperature
//! sensor, which varies linearly with temperature.
//!
//! The function performs the following actions:
//! - Unlocks the ATESTCFG register for writing
//! - Selects VDDS as the supply voltage for the ATB switches
//! - Opens all shorting switches to ground to prevent signal shorting
//! - Closes specific isolation switches to create the required signal path
//! - Keeps unused isolation switches open to prevent interference
//!
//! \return None
//!
//*****************************************************************************
static void AuxTempSenseConfigureAtest(void)
{
    // Configure the Analog Test Bus (ATB) for temperature sensing by writing
    // to the ATESTCFG register with:
    //
    // - SYS0_ATESTCFG_KEY_UNLOCK: Write the unlock key (0x5A) to enable register
    //   modification. This is required for any write to the ATESTCFG register.
    //
    // - SYS0_ATESTCFG_VSEL_VDDS: Select VDDS (standard supply voltage) as the
    //   supply for ATEST switches instead of VDDBOOST. Using VDDS provides a
    //   more accurate reference for temperature measurements, as it better
    //   represents the operating conditions of the temperature sensor.
    //
    // - AUXTEMPSENSE_ATEST_SWITCH_CFG_TO_ADC13: Apply the specific switch
    //   configuration needed for temperature sensing:
    //   * Close VR2VA0 to connect the voltage reference to the analog path
    //   * Close VA2VA0 to complete the signal path to the ADC
    //   * Open all shorting switches to prevent grounding of signals
    //   * Keep unused isolation switches open to prevent interference
    //
    // This configuration creates a precise signal path that allows the ADC to
    // measure the voltage across the temperature sensor on channel 13, which
    // varies linearly with temperature according to the sensor's characteristics.
    HWREG(SYS0_BASE + SYS0_O_ATESTCFG) = SYS0_ATESTCFG_KEY_UNLOCK | SYS0_ATESTCFG_VSEL_VDDS |
                                         AUXTEMPSENSE_ATEST_SWITCH_CFG_TO_ADC13;
}

//*****************************************************************************
//
//! \brief Enable the auxiliary temperature sensor.
//!
//! This function enables the auxiliary temperature sensor by setting appropriate
//! bits in the TSENSCFG register.
//!
//! \return None
//!
//*****************************************************************************
static void AuxTempSenseEnableSensor(void)
{
    // Enable auxiliary temperature sensor
    HWREG(SYS0_BASE + SYS0_O_TSENSCFG) = SYS0_TSENSCFG_TSENS2EN_EN | SYS0_TSENSCFG_SEL_DISABLE;
}

//*****************************************************************************
//
//! \brief Disable the auxiliary temperature sensor.
//!
//! This function disables the auxiliary temperature sensor by clearing
//! the enable bit in the TSENSCFG register.
//!
//! \return None
//!
//*****************************************************************************
static void AuxTempSenseDisableSensor(void)
{
    // Disable Aux temperature sensor measurement
    HWREG(SYS0_BASE + SYS0_O_TSENSCFG) = SYS0_TSENSCFG_TSENS2EN_DIS | SYS0_TSENSCFG_SEL_DISABLE;
}
