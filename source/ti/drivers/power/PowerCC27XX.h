/*
 * Copyright (c) 2022-2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       PowerCC27XX.h
 *
 *  @brief      Power manager interface for CC27XX
 *
 *  The Power header file should be included in an application by including the
 *  top level header file as follows:
 *  @code
 *  #include <ti/drivers/Power.h>
 *  @endcode
 *
 *  Refer to @ref Power.h for a complete description of APIs.
 *
 *  ## Implementation ##
 *  This header file defines the power resources, constraints, events, sleep
 *  states and transition latencies for CC27XX.
 *
 *  @anchor ti_drivers_PowerCC27XX_HFXT_Amplitude_Compensation
 *  ## HFXT Amplitude Compensation ##
 *  The CC27XX Power driver will configure the HFXT amplitude to the highest
 *  possible value at boot (in #Power_init()). Each time the device
 *  enters standby the HFXT amplitude will be updated to ensure the optimal
 *  amplitude is used. It will take up to 5 iterations (5 standby entries)
 *  after boot until the optimal amplitude has been found. This process will
 *  ensure that the amplitude is kept in an optimal range if the operating
 *  conditions change, as long as the device regularly enters standby.
 *
 *  ### Applications that rarely enters standby ###
 *  The amplitude adjustments done at standby entry is sufficient for
 *  applications that regularly enters standby, but if the application does not
 *  regularly enter standby, then #PowerLPF3_getHfxtAmpAdjustment() and
 *  #PowerLPF3_adjustHfxtAmp() can be used to check if an adjustment is needed,
 *  and perform the adjustment if needed.
 *
 *  @anchor ti_drivers_PowerCC27XX_Initial_HFXT_Amplitude_Compensation
 *  ### Initial HFXT Amplitude Compensation ###
 *  If the application requires that the HFXT amplitude is already in the
 *  optimal range after boot, then Initial HFXT Amplitude Compensation can be
 *  enabled with #PowerCC27XX_Config.startInitialHfxtAmpCompFxn.
 *  If initial HFXT amplitude compensation is enabled, the optimal amplitude
 *  will be found at/after boot, meaning it will take longer before HFXT is
 *  ready after boot, but when it is ready the amplitude will already be in the
 *  optimal range. This process is done asynchronously, so the application can
 *  do other tasks while waiting for HFXT to be ready.
 *
 *  Enabling initial HFXT amplitude compensation will result in more flash usage
 *  and longer time from boot to the first RF operation.
 *
 *  @anchor ti_drivers_PowerCC27XX_HFXT_Temperature_Compensation
 *  ### HFXT Temperature Compensation (Software TCXO) ###
 *  HFXT frequency is known to vary with temperature,
 *  landing outside acceptable operational range. By enabling compensation,
 *  the device will correct for this variation above the selected temperature.
 *  The compensation is enabled by default on CC27X0 devices.
 *  The ppm offset used to compensate the HFXT can be approximated by a
 *  third order polynomial function of temperature in Celsius,
 *  see #PowerLPF3_initHFXTCompensation().
 *
 *  @anchor ti_drivers_PowerCC27XX_TFM_Enabled_Applications
 *  ## TFM Enabled Applications ##
 *  The Power driver has certain restrictions and limitations in TFM enabled
 *  application and certain APIs operate slightly differently compared to
 *  secure-only applications. Some APIs are also not available from non-secure
 *  contexts.
 *
 *  ### Constraints and Resource Dependencies ###
 *  When called from non-secure context, #Power_getDependencyCount(),
 *  #Power_getConstraintCount() and  #Power_getConstraintMask() will return the
 *  non-secure dependency count, constraint count and constraint mask,
 *  respectively. This means that a count or mask bit of zero does not
 *  necessarily mean that there are no dependencies or constraints set, since
 *  the secure side might still have a dependency or constraints set.
 *
 *  ### Unavailable APIs from Non-Secure Context ###
 *  * #Power_getTransitionState()
 *
 *  ============================================================================
 */

#ifndef ti_drivers_power_PowerCC27XX_
#define ti_drivers_power_PowerCC27XX_

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/Power.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_pmctl.h)
#include DeviceFamily_constructPath(inc/hw_clkctl.h)
#include DeviceFamily_constructPath(inc/hw_lrfddbell.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(driverlib/pmctl.h)

#ifdef __cplusplus
extern "C" {
#endif

/*! The latency to reserve for resume from STANDBY (usec). */
#define PowerCC27XX_RESUMETIMESTANDBY (400U)

/*! The total latency to reserve for entry to and exit from STANDBY (usec). */
#define PowerCC27XX_TOTALTIMESTANDBY (500U)

/*! The initial delay when waking from STANDBY (usec). */
#define PowerCC27XX_WAKEDELAYSTANDBY (150U)

/* Default lower threshold for when HFXT compensation is enabled */
#define PowerCC27XX_HFXT_THRESHOLD_TEMP_DEFAULT (-40)

/* \cond */
/* The control of the peripherals are split between multiple groups.
 * These defines are used to differentiate between the groups.
 * The bits in the PowerCC27XX_PERIPH_GROUP_M mask is used to store the group id,
 * and the bits in the PowerCC27XX_PERIPH_BIT_INDEX_M mask is used to store the
 * bit index shift value in the register for the given group.
 */
#define PowerCC27XX_PERIPH_GROUP_M       (0xFF00U)
#define PowerCC27XX_PERIPH_GROUP_CLKCTL0 (0x0000U)
#define PowerCC27XX_PERIPH_GROUP_CLKCTL1 (0x0100U)
#define PowerCC27XX_PERIPH_GROUP_LRFD    (0x0200U)
#define PowerCC27XX_PERIPH_BIT_INDEX_M   (0x00FFU)

/* \endcond */

/* \cond */
typedef uint16_t PowerLPF3_Resource; /* Power resource identifier */
/* \endcond */

/* Resource IDs */

/*! Resource ID: General Purpose I/O */
#define PowerLPF3_PERIPH_GPIO (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_GPIO_S)

/*! Resource ID: UART 0 */
#define PowerLPF3_PERIPH_UART0 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_UART0_S)

/*! Resource ID: UART 1 */
#define PowerLPF3_PERIPH_UART1 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_UART1_S)

/*! Resource ID: I2C 0 */
#define PowerLPF3_PERIPH_I2C0 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_I2C0_S)

/*! Resource ID: SPI 0 */
#define PowerLPF3_PERIPH_SPI0 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_SPI0_S)

/*! Resource ID: SPI 1 */
#define PowerLPF3_PERIPH_SPI1 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_SPI1_S)

/*! Resource ID: ADC */
#define PowerLPF3_PERIPH_ADC0 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_ADC0_S)

/*! Resource ID: AES Security Module */
#define PowerLPF3_PERIPH_AES (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_LAES_S)

/*! Resource ID: uDMA Controller */
#define PowerLPF3_PERIPH_DMA (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_DMA_S)

/*! Resource ID: I2S */
#define PowerLPF3_PERIPH_I2S (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_I2S_S)

/*! Resource ID: General Purpose Timer 0 */
#define PowerLPF3_PERIPH_LGPT0 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_LGPT0_S)

/*! Resource ID: General Purpose Timer 1 */
#define PowerLPF3_PERIPH_LGPT1 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_LGPT1_S)

/*! Resource ID: General Purpose Timer 2 */
#define PowerLPF3_PERIPH_LGPT2 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_LGPT2_S)

/*! Resource ID: General Purpose Timer 3 */
#define PowerLPF3_PERIPH_LGPT3 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_LGPT3_S)

/*! Resource ID: Hardware Security Module */
#define PowerLPF3_PERIPH_HSM (PowerCC27XX_PERIPH_GROUP_CLKCTL1 | CLKCTL_DESCEX1_HSM_S)

/*! Resource ID: Algorithm Processing Unit */
#define PowerLPF3_PERIPH_APU (PowerCC27XX_PERIPH_GROUP_CLKCTL1 | CLKCTL_DESCEX1_APU_S)

/*! Resource ID: MCAN */
#define PowerLPF3_PERIPH_MCAN (PowerCC27XX_PERIPH_GROUP_CLKCTL1 | CLKCTL_DESCEX1_MCAN_S)

/*! Resource ID: LRFD Tracer */
#define PowerLPF3_PERIPH_LFRD_TRC (PowerCC27XX_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_TRC_S)

/*! Resource ID: LRFD S2R RAM */
#define PowerLPF3_PERIPH_LFRD_S2RRAM (PowerCC27XX_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_S2RRAM_S)

/* \cond */
/* Number of resources in the different resource groups. This is possibly larger
 * than the peripheral instance count on various device variants. This is
 * because some indexes might not be used for a given variant, resulting in a
 * sparse table.
 */
#define PowerCC27XX_NUMRESOURCES_CLKCTL0 (31U)
#define PowerCC27XX_NUMRESOURCES_CLKCTL1 (7U)
#define PowerCC27XX_NUMRESOURCES_LRFD    (12U)

/* \endcond */

#define PowerLPF3_STANDBY (0x1U) /*!< The STANDBY sleep state */
/* \cond */

/* Constraints
 *
 * Constraints restrict a specific system behavior from occurring or guarantee
 * a specified effect until released.
 */

/*! Constraint: Disallow a transition to the SHUTDOWN state */
#define PowerLPF3_DISALLOW_SHUTDOWN (0U)

/*! Constraint: Disallow a transition to the STANDBY sleep state */
#define PowerLPF3_DISALLOW_STANDBY (1U)

/*! Constraint: Disallow a transition to the IDLE sleep state */
#define PowerLPF3_DISALLOW_IDLE (2U)

/*! Constraint: Flash memory needs to enabled during IDLE */
#define PowerLPF3_NEED_FLASH_IN_IDLE (3U)

/*! Constraint: Disallow software TCXO during RF operations */
#define PowerLPF3_DISALLOW_SWTCXO (4U)

/* \cond */
#define PowerCC27XX_NUMCONSTRAINTS (5U) /* Number of constraints supported */
/* \endcond */

/*
 *  Events
 *
 *  Each event must be a power of two and must be sequential
 *  without any gaps.
 */

/*! Power event: The device is entering the STANDBY sleep state */
#define PowerLPF3_ENTERING_STANDBY ((uint_fast16_t)((uint_fast16_t)1U << 0))

/*! Power event: The device is entering the SHUTDOWN state */
#define PowerLPF3_ENTERING_SHUTDOWN ((uint_fast16_t)((uint_fast16_t)1U << 1))

/*! Power event: The device is waking up from the STANDBY sleep state */
#define PowerLPF3_AWAKE_STANDBY ((uint_fast16_t)((uint_fast16_t)1U << 2))

/*! Power event: The high frequency (HF) crystal oscillator is now available
 *   for use (HFXT) by the digital domain
 */
#define PowerLPF3_HFXT_AVAILABLE ((uint_fast16_t)((uint_fast16_t)1U << 3))

/*! Power event: The system has switched to the low frequency clock source
 *   configured in CCFG
 */
#define PowerLPF3_LFCLK_SWITCHED ((uint_fast16_t)((uint_fast16_t)1U << 4))

/* \cond */
#define PowerCC27XX_NUMEVENTS (5U) /* Number of events supported */
/* \endcond */

/*!
 *  @brief Function pointer to #PowerLPF3_startInitialHfxtAmpComp() or NULL.
 *
 *  This type is only allowed to have the value NULL or be a
 *  pointer to #PowerLPF3_startInitialHfxtAmpComp().
 */
typedef void (*PowerLPF3_StartInitialHfxtAmpCompFxn)(void);

/*! @brief Global configuration structure */
typedef struct
{
    /*!
     *  @brief The Power Policy's initialization function
     *
     *  If the policy does not have an initialization function, 'NULL'
     *  should be specified.
     */
    Power_PolicyInitFxn policyInitFxn;
    /*!
     *  @brief The Power Policy function
     *
     *  When enabled, this function is invoked in the idle loop, to
     *  opportunistically select and activate sleep states.
     *
     *  Two reference policies are provided:
     *
     *    PowerCC27XX_doWFI() - a simple policy that invokes CPU wait for
     *    interrupt (WFI)
     *
     *    PowerCC27XX_standbyPolicy() - an agressive policy that considers
     *    constraints, time until next scheduled work, and sleep state
     *    latencies, and optionally puts the device into the STANDBY state,
     *    the IDLE state, or as a minimum, WFI.
     *
     *  Custom policies can be written, and specified via this function pointer.
     *
     *  In addition to this static selection, the Power Policy can be
     *  dynamically changed at runtime, via the Power_setPolicy() API.
     *
     *  If not NULL, the policy function will be invoked once for each pass
     *  of the idle loop.
     *
     *  The power policy can be dynamically
     *  enabled and disabled at runtime, via the Power_enablePolicy() and
     *  Power_disablePolicy() functions, respectively.
     */
    Power_PolicyFxn policyFxn;

    /*!
     *  @brief Pointer to the function to start the initial HFXT amplitude
               compensation.
     *
     *  This is used to enable/disable the initial HFXT amplitude compensation
     *  feature.
     *
     * The allowed values for this field is:
     *  - NULL: Initial HFXT amplitude compensation is disabled.
     *  - Pointer to #PowerLPF3_startInitialHfxtAmpComp(): Initial HFXT
     *    amplitude compensation is disabled.
     *
     * @sa @ref ti_drivers_PowerCC27XX_Initial_HFXT_Amplitude_Compensation "Initial HFXT Amplitude Compensation"
     */
    PowerLPF3_StartInitialHfxtAmpCompFxn startInitialHfxtAmpCompFxn;
} PowerCC27XX_Config;

/*!
 * @brief Reasons the device has booted or rebooted.
 */
typedef enum
{
    /*! Device woke up from shutdown due to an IO event */
    PowerLPF3_RESET_SHUTDOWN_IO  = PMCTL_RESET_SHUTDOWN_IO,
    /*! Device woke up from shutdown due to an SWD event */
    PowerLPF3_RESET_SHUTDOWN_SWD = PMCTL_RESET_SHUTDOWN_SWD,
    /*! Device reset because of a watchdog timeout. */
    PowerLPF3_RESET_WATCHDOG     = PMCTL_RESET_WATCHDOG,
    /*! Device reset triggered by software writing to RSTCTL.SYSRST */
    PowerLPF3_RESET_SYSTEM       = PMCTL_RESET_SYSTEM,
    /*! Device reset triggered by CPU reset event */
    PowerLPF3_RESET_CPU          = PMCTL_RESET_CPU,
    /*! Device reset triggered by CPU lockup event */
    PowerLPF3_RESET_LOCKUP       = PMCTL_RESET_LOCKUP,
    /*! Device woke up from thermal shutdown after temperature drop */
    PowerLPF3_RESET_TSD          = PMCTL_RESET_TSD,
    /*! Device woke up due to Serial Wire Debug event */
    PowerLPF3_RESET_SWD          = PMCTL_RESET_SWD,
    /*! Device reset due to LFXT clock loss */
    PowerLPF3_RESET_LFXT         = PMCTL_RESET_LFXT,
    /*! Device reset due to VDDR brownout event */
    PowerLPF3_RESET_VDDR         = PMCTL_RESET_VDDR,
    /*! Device reset due to VDDS brownout event */
    PowerLPF3_RESET_VDDS         = PMCTL_RESET_VDDS,
    /*! Device reset due to pin reset */
    PowerLPF3_RESET_PIN          = PMCTL_RESET_PIN,
    /*! Device booted due to power on reset */
    PowerLPF3_RESET_POR          = PMCTL_RESET_POR,
    /*! Unknown reset reason */
    PowerLPF3_RESET_UNKNOWN      = UINT32_MAX,
} PowerLPF3_ResetReason;

/*!
 * @brief The possible frequencies to configure the AFOSC to.
 *
 * The value of each enumeration represents the frequency in Hz.
 */
typedef enum
{
    PowerLPF3_AFOSC_FREQ_80MHZ      = 80000000U, /*!< 80 MHz */
    PowerLPF3_AFOSC_FREQ_90P3168MHZ = 90316800U, /*!< 90.3168 MHz */
    PowerLPF3_AFOSC_FREQ_98P304MHZ  = 98304000U, /*!< 98.304 MHz */
} PowerLPF3_AfoscFreq;

/*!
 * @brief Possible LFOSC PPM requirements to be used by the LFOSC compensation
 */
typedef enum
{
    /*! @brief 500 PPM */
    PowerLPF3_LFOSC_PPM_500 = 500U,
} PowerLPF3_LfoscCompensationPpmRequirement;

/*! @brief LFOSC Compensation Profile structure.
 *
 * When LFOSC Compensation is enabled, HFXT will periodically be turned on to
 * calibrate the LFOSC. The duration of these calibrations and the interval
 * between them are computed based on the values in this structure, and other
 * parameters such as the current temperature.
 *
 * @sa #PowerLPF3_setLfoscCompensationProfile()
 */
typedef struct
{
    /*! @brief The expected interval in us between system wakeups.
     *
     * This would for example be 1000000 (1s) for a 1s BLE connection interval.
     */
    uint32_t systemWakeupIntervalUsec;

    /*! @brief The PPM requirements */
    PowerLPF3_LfoscCompensationPpmRequirement ppmRequirement;

    /*! @brief The maximum additional jitter allowed per wakeup beyond the ppm requirement in us */
    uint16_t maxAllowedJitterUsec;

    /*! @brief The absolute value of the worst case expected temperature
     *  gradient in mC/s.
     */
    uint16_t temperatureGradientMilliCelsiusPerSec;

} PowerLPF3_LfoscCompensationProfile;

/*!
 *  @brief  The wait for interrupt (WFI) policy
 *
 *  This is a lightweight Power Policy which simply invokes CPU wait for
 *  interrupt.
 *
 *  @note For TFM enabled applications, the CPU wait for interrupt instruction
 *  (WFI) is invoked in a loop until a NS interrupt becomes pending. The loop
 *  will allow the CPU to vector to S interrupts. If something is not allowing
 *  that, for example if S interrupts are disabled, then the function will also
 *  return.
 *
 *  This policy can be selected statically via the policyFxn pointer in the
 *  PowerCC27XX_Config structure, or dynamically at runtime, via
 *  Power_setPolicy().
 */
void PowerCC27XX_doWFI(void);

/*!
 * @brief Returns the reason for the most recent reset or wakeup
 *
 * @return #PowerLPF3_ResetReason
 * @pre Power_shutdown()
 * @post PowerLPF3_releaseLatches()
 */
PowerLPF3_ResetReason PowerLPF3_getResetReason(void);

/*!
 * @brief Unlatch all IOs
 *
 * This function releases the latches on all frozen IOs. This function should be
 * called after waking up from shutdown and reconfiguring the IO state so as not
 * to cause glitches.
 *
 * @note Calling this function will clear the reset reason register if it was
 *       #PowerLPF3_RESET_SHUTDOWN_IO or #PowerLPF3_RESET_SHUTDOWN_SWD and
 *       cause PowerLPF3_getResetReason() not to return the true reset reason.
 *
 * @pre Power_shutdown()
 * @pre PowerLPF3_getResetReason()
 */
void PowerLPF3_releaseLatches(void);

/*!
 *  @brief  The STANDBY Power Policy
 *
 *  This is an agressive Power Policy, which considers active constraints,
 *  sleep state transition latencies, and time until the next scheduled
 *  work, and automatically transitions the device into the deepest sleep state
 *  possible.
 *
 *  The first goal is to enter STANDBY; if that is not appropriate
 *  given current conditions (e.g., the sleep transition latency is greater
 *  greater than the time until the next scheduled Clock event), then
 *  the secondary goal is the IDLE state; if that is disallowed (e.g., if
 *  the PowerLPF3_DISALLOW_IDLE constraint is declared), then the policy
 *  will fallback and simply invoke WFI, to clock gate the CPU until the next
 *  interrupt.
 *
 *  In order for this policy to run, it must be selected as the Power
 *  Policy (either by being specified as the 'policyFxn' in the
 *  PowerCC27XX_Config structure, or specified at runtime with
 *  Power_setPolicy()), and the Power Policy must be enabled (either via
 *  'enablePolicy' in the PowerCC27XX_Config structure, or via a call to
 *  Power_enablePolicy() at runtime).
 */
void PowerCC27XX_standbyPolicy(void);

/*!
 * @brief Select LFOSC as LFCLK source
 *
 * Turn on the LFOSC and choose it as LFCLK source. Once LFCLK has switched,
 * the #PowerLPF3_LFCLK_SWITCHED notification will be issued and all
 * subscribers to this event will be notified.
 *
 * @warning Dynamic switching between LF clocks is not supported. Once one of
 * PowerLPF3_selectLFOSC(), PowerLPF3_selectLFXT() or PowerLPF3_selectEXTLF()
 * is called in an application, the other two may not be invoked thereafter.
 *
 * @pre Power_init()
 * @sa PowerLPF3_selectLFXT()
 * @sa PowerLPF3_selectEXTLF()
 */
void PowerLPF3_selectLFOSC(void);

/*!
 * @brief Select LFXT as LFCLK source
 *
 * Turn on the LFXT and choose it as LFCLK source. Once LFCLK has switched,
 * the #PowerLPF3_LFCLK_SWITCHED notification will be issued and all
 * subscribers to this event will be notified.
 *
 * @warning Dynamic switching between LF clocks is not supported. Once one of
 * PowerLPF3_selectLFOSC(), PowerLPF3_selectLFXT() or PowerLPF3_selectEXTLF()
 * is called in an application, the other two may not be invoked thereafter.
 *
 * @pre Power_init()
 * @sa PowerLPF3_selectLFOSC()
 * @sa PowerLPF3_selectEXTLF()
 */
void PowerLPF3_selectLFXT(void);

/*!
 * @brief Select EXTLF as LFCLK source
 *
 * Choose an external 31.25 kHz square wave as the LFCLK source as input.
 * Once LFCLK has switched, the #PowerLPF3_LFCLK_SWITCHED notification
 * will be issued and all subscribers to this event will be notified.
 *
 * This function requires the following symbols to be defined.
 *  - \c PowerLPF3_extlfPin (uint8_t): The DIO number of the pin to be used as the EXTLF pin.
 *  - \c PowerLPf3_extlfPinMux (uint8_t): Mux value used to mux the EXTLF signal to \c PowerLPF3_extlfPin.
 * If using SysConfig, the symbols will be defined in ti_drivers_config.c.
 *
 * @warning Dynamic switching between LF clocks is not supported. Once one of
 * PowerLPF3_selectLFOSC(), PowerLPF3_selectLFXT() or PowerLPF3_selectEXTLF()
 * is called in an application, the other two may not be invoked thereafter.
 *
 * @pre Power_init()
 * @sa PowerLPF3_selectLFOSC()
 * @sa PowerLPF3_selectLFXT()
 */
void PowerLPF3_selectEXTLF(void);

/*!
 * @brief Initialize HFXT temperature compensation coefficients
 *
 * Initialize the parameters used for HFXT temperature coefficients. They approximate
 * the ppm offset of the HFXT frequency with the following polynomial as a function of
 * temperature (degC), where P_3 = P3 / 2^shift, P_2 = P2 / 2^shift, etc..
 * ppm(T) = P_3*T^3 + P_2*T^2 + P_1*T + P_0
 *
 * @note The fixed-point coefficients and the shift argument must ensure that the computation does
 * not overflow 32 bits in the -40, 125 degC range.
 *
 * @param[in] P0    0th-order coefficient, multiplied by 2^shift
 * @param[in] P1    1st-order coefficient, multiplied by 2^shift
 * @param[in] P2    2nd-order coefficient, multiplied by 2^shift
 * @param[in] P3    3rd-order coefficient, multiplied by 2^shift
 * @param[in] shift Shift-value for scaling fixed-point coefficients
 * @param[in] fcfgInsertion Boolean used to indicate presence of HFXT FCFG data.
 *
 * @pre Power_init()
 */
void PowerLPF3_initHFXTCompensation(int32_t P0, int32_t P1, int32_t P2, int32_t P3, uint8_t shift, bool fcfgInsertion);

/*!
 * @brief Enable HFXT temperature compensation
 *
 * Enable automatic compensation for temperature-based frequency-drift of HFXT
 *
 * This function should only be called once, but can be invoked again if
 * PowerLPF3_enableHFXTCompensation has been called
 *
 * @param[in] tempThreshold Threshold above which temperature compensation will
 * be performed. This can be useful to save power consumption if HFXT
 * performance is acceptable at low temperatures, and only required at
 * high temperatures. If the threshold is set to for example 80 degrees, then
 * the first compensation will occur once the temperature reaches 81 degrees.
 * Units in degrees Celsius.
 * @param[in] tempDelta Delta describing how much the temperature can drift
 * before compensation is applied. If compensation is performed at 81 degrees,
 * and the delta is set to 5, then a re-compensation is performed at either
 * 76 degrees or 86 degrees, depending on which temperature state occurs first.
 * Units in degrees Celsius.
 *
 * @pre PowerLPF3_initHFXTCompensation()
 */
void PowerLPF3_enableHFXTCompensation(int16_t tempThreshold, int16_t tempDelta);

/*!
 * @brief Disable HFXT temperature compensation
 *
 * Disable automatic compensation for temperature-based frequency-drift of HFXT
 *
 * @note Calling this function will also undo any previous temperature compensation that has been
 * performed in the past, and HFXT will become uncompensated
 *
 * @pre PowerLPF3_enableHFXTCompensation()
 */
void PowerLPF3_disableHFXTCompensation(void);

/*!
 * @brief Force HFXT temperature compensation update
 *
 * Should be called after releasing the #PowerLPF3_DISALLOW_SWTCXO constraint
 * to ensure the correct HFXT frequency.
 *
 * @pre PowerLPF3_enableHFXTCompensation()
 */
void PowerLPF3_forceHFXTCompensationUpdate(void);

/*!
 * @brief Get the current HFXT compensation ratio.
 *
 * The compensation ratio is the ratio between the nominal HFXT freqeuncy
 * (48MHz) and the currently expected HFXT frequency.
 *
 * The format of the ratio is 4.22 (4 integer bits and 22 fractional bits).
 *
 * @return @c 48MHz*2^22/f where @c f is the currently expected HFXT frequency.
 */
uint32_t PowerLPF3_getHFXTCompensationRatio(void);

/*!
 *  @brief  Start the AFOSC
 *
 *  This function will start the AFOSC if it is not already running. The AFOSC
 *  will be configured to the specified frequency and it will be configured to
 *  automatically be disabled when entering STANDBY, meaning this function will
 *  need to be called again after waking up from STANDBY.
 *
 *  If the AFOSC is already running, no change will be done, and an error code
 *  will be returned. This indicates that another SW component is "owning" the
 *  AFOSC.
 *
 *  @param[in]  frequency      The desired frequency of the AFOSC.
 *
 *  @return #Power_SOK on success,
 *          #Power_EFAIL if AFOSC is already running,
 *          #Power_EINVALIDINPUT if an invalid frequency is specified.
 *
 *  @sa  #PowerLPF3_stopAFOSC()
 */
int_fast16_t PowerLPF3_startAFOSC(PowerLPF3_AfoscFreq frequency);

/*!
 *  @brief  Stop the AFOSC
 *
 *  This function will stop the AFOSC.
 *
 *  @sa  #PowerLPF3_startAFOSC()
 */
void PowerLPF3_stopAFOSC(void);

/*!
 * @brief Start initial compensation of the HFXT amplitude
 *
 * @warning This function must not be called by the application. It is only
 * intended to be specified for #PowerCC27XX_Config.startInitialHfxtAmpCompFxn
 */
void PowerLPF3_startInitialHfxtAmpComp(void);

/*!
 * @brief Check if HFXT amplitude needs to be adjusted
 *
 * The HFXT amplitude needs to regularly be checked and if needed adjusted.
 * The Power driver will check the amplitude and adjust it when the device
 * enters standby. However if the device does not enter standby often enough,
 * then additional checks and adjustments are needed.
 *
 * This function can be used to check if adjustment of the amplitude is needed,
 * and #PowerLPF3_adjustHfxtAmp() can be used to adjust the amplitude if needed.
 *
 * @return
 *  - +1: The amplitude needs to be increased
 *  -  0: No adjustments are needed
 *  - -1: The amplitude needs to be decreased
 *
 * @sa #PowerLPF3_adjustHfxtAmp()
 */
int_fast8_t PowerLPF3_getHfxtAmpAdjustment(void);

/*!
 * @brief Adjust the HFXT amplitude
 *
 * This function is to be used to adjust the HFXT amplitude if an adjustment is
 * needed according to #PowerLPF3_getHfxtAmpAdjustment(). Please refer to the
 * documentation of #PowerLPF3_getHfxtAmpAdjustment() for more details.
 *
 * @note The radio must be disabled when adjusting the HFXT amplitude, and while
 * the amplitude is settling.
 * After this function returns, the caller must wait until the HFXT amplitude
 * has settled.
 *
 * @param adjustment the value returned by #PowerLPF3_getHfxtAmpAdjustment().
 *                   No other value is allowed.
 *
 * @pre #PowerLPF3_getHfxtAmpAdjustment() must be called, and it must return
 * a non-zero value (indicating that adjustment is needed)
 * @sa #PowerLPF3_getHfxtAmpAdjustment()
 */
void PowerLPF3_adjustHfxtAmp(int_fast8_t adjustment);

/*!
 * @brief Initialize LFOSC Compensation
 *
 * This function initializes the internal state of the LFOSC compensation. It
 * must be called before calling any other LFOSC Compensation functions. Calling
 * this function multiple times will only have an effect the first time.
 *
 * Calling context: Task, Main.
 */
void PowerLPF3_initLfoscCompensation(void);

/*!
 * @brief Set LFOSC Compensation Profile
 *
 * Set the LFOSC Compensation profile to use when LFOSC compensation is enabled.
 *
 * This must be called before enabling LFOSC compensation, and it can be called
 * while the compensation is enabled to change the profile to be used for the
 * compensation.
 *
 * Calling context: Task, Main.
 *
 * @param[in] profile  Pointer to the LFOSC Compensation Profile to be used.
 * The profile data will be copied to an internal structure, meaning the profile
 * can be changed or deleted after this function returns without affecting the
 * LFOSC compensation.
 *
 * @pre #PowerLPF3_initLfoscCompensation()
 */
void PowerLPF3_setLfoscCompensationProfile(const PowerLPF3_LfoscCompensationProfile *profile);

/*!
 * @brief Enable LFOSC Compensation
 *
 * Enable compensation of LFOSC.
 *
 * This function should only be called once, but can be invoked again if
 * #PowerLPF3_disableLfoscCompensation() has been called.
 *
 * Calling context: Task, Main.
 *
 * @pre #PowerLPF3_setLfoscCompensationProfile()
 */
void PowerLPF3_enableLfoscCompensation(void);

/*!
 * @brief Disable LFOSC Compensation
 *
 * Disable compensation of LFOSC.
 *
 * Calling context: Task, Main.
 *
 * @pre #PowerLPF3_enableLfoscCompensation()
 */
void PowerLPF3_disableLfoscCompensation(void);

/*!
 *  @brief  Transition the device into standby and configure RTC to wakeup
 *          the device ahead of a specified time.
 *
 *  This function is called from the power policy when the decision has been
 *  made to put the device in standby. This function returns to the caller
 *  (the policy function) after the device wakes up from standby.
 *
 *  The function is doing the following:
 *  - Disable SysTick (For TFM enabled apps, the NS SysTick is disabled)
 *  - Store SysTimer state
 *  - Configure RTC to wake up the device #PowerCC27XX_WAKEDELAYSTANDBY
 *    microseconds before the next event, @c nextEventTimeUs. This is to ensure
 *    that the device is ready to service the event at time @c nextEventTimeUs.
 *  - Post the #PowerLPF3_ENTERING_STANDBY notification
 *  - Adjust HFXT amplitude, if needed.
 *  - Enter standby
 *  - Start HFXT
 *  - Disarm RTC
 *  - Restore SysTimer state
 *  - Configure LRFD clocks
 *  - Post the #PowerLPF3_AWAKE_STANDBY notification
 *
 *  @note The SysTick is not re-enabled by this function, it is the caller's
 *  responsibility to re-enable the SysTick if needed after this function
 *  returns.
 *
 *  @warning For TFM enabled applications, the above steps are only performed
 *  if standby is a allowed by the secure power constraints and if
 *  @c nextEventTimeUs is far enough in the future. If standby is not allowed
 *  and idle is allowed, the device will enter idle instead. In this case
 *  @c nextEventTimeUs will be ignored, so it is the caller's responsibility to
 *  set up the necessary wake up source in case the device enters idle instead
 *  of standby. For example setting up SysTick or SysTimer. If idle is also not
 *  allowed, the function will return immediately.
 *
 *  @warning This function must be called with interrupts disabled, and
 *  should not be called directly by the application, or by any drivers.
 *  This function does not check declared constraints; the policy function
 *  must check constraints before calling this function to initiate sleep.

 *
 *  @param[in]  nextEventTimeUs    the SysTimer time of the next event, this
 *                                 must be far enough in the future.
 *
 *  @retval  #Power_SOK on success, the device has slept and is awake again.
 *
 *  @retval  #Power_EFAIL if an error occurred during client notifications, or
 *  if a general failure occurred.
 */
int_fast16_t PowerLPF3_sleep(uint32_t nextEventTimeUs);

/**
 * @brief Checks if the LFINC filter is ready to go to standby or not.
 *
 * @note This is separate from the normal standby constraint system in the Power
 * driver.
 *
 * @return true if standby is allowed by the LFINC filter, false otherwise.
 */
bool PowerLPF3_isLfincFilterAllowingStandby(void);

void PowerCC27XX_schedulerDisable(void);
void PowerCC27XX_schedulerRestore(void);

#ifdef __cplusplus
}
#endif

#endif /* POWER_CC27XX_ */
