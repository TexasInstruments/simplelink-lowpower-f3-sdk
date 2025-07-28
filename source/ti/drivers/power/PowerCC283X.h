/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
 *  @file       PowerCC283X.h
 *
 *  @brief      Power manager interface for CC283X devices
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
 *  states and transition latencies for CC283X.
 *
 *  ============================================================================
 */

#ifndef ti_drivers_power_PowerCC283X__include
#define ti_drivers_power_PowerCC283X__include

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/Power.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Resource IDs */

/*! Resource ID: General Purpose I/O */
#define PowerLPF3_PERIPH_GPIO (0U)

/*! Resource ID: UART 0 */
#define PowerLPF3_PERIPH_UART0 (0U)

/*! Resource ID: UART 1 */
#define PowerLPF3_PERIPH_UART1 (0U)

/*! Resource ID: I2C 0 */
#define PowerLPF3_PERIPH_I2C0 (0U)

/*! Resource ID: SPI 0 */
#define PowerLPF3_PERIPH_SPI0 (0U)

/*! Resource ID: AES Security Module */
#define PowerLPF3_PERIPH_AES (0U)

/*! Resource ID: uDMA Controller */
#define PowerLPF3_PERIPH_DMA (0U)

/*! Resource ID: General Purpose Timer 0 */
#define PowerLPF3_PERIPH_LGPT0 (0U)

/*! Resource ID: General Purpose Timer 1 */
#define PowerLPF3_PERIPH_LGPT1 (0U)

/*! Resource ID: General Purpose Timer 2 */
#define PowerLPF3_PERIPH_LGPT2 (0U)

/*! Resource ID: General Purpose Timer 3 */
#define PowerLPF3_PERIPH_LGPT3 (0U)

#define PowerLPF3_STANDBY (0x1U) /*!< The STANDBY sleep state */

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
     *    PowerCC283X_doWFI() - a simple policy that invokes CPU wait for
     *    interrupt (WFI)
     *
     *    PowerCC283X_standbyPolicy() - an aggressive policy that considers
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
} PowerCC283X_Config;

/*!
 * @brief Reasons the device has booted or rebooted.
 */
typedef enum
{
    /*! Unknown reset reason */
    PowerLPF3_RESET_UNKNOWN = UINT32_MAX,
} PowerLPF3_ResetReason;

/*!
 *  @brief  The wait for interrupt (WFI) policy
 *
 *  This is a lightweight Power Policy which simply invokes CPU wait for
 *  interrupt.
 *
 *  This policy can be selected statically via the policyFxn pointer in the
 *  PowerCC23X0_Config structure, or dynamically at runtime, via
 *  Power_setPolicy().
 */
void PowerCC283X_doWFI(void);

/*!
 * @brief Returns the reason for the most recent reset or wakeup
 *
 * @return #PowerLPF3_ResetReason
 * @pre Power_shutdown()
 * @post PowerLPF3_releaseLatches()
 */
PowerLPF3_ResetReason PowerLPF3_getResetReason(void);

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
 *  PowerCC23X0_Config structure, or specified at runtime with
 *  Power_setPolicy()), and the Power Policy must be enabled (either via
 *  'enablePolicy' in the PowerCC23X0_Config structure, or via a call to
 *  Power_enablePolicy() at runtime).
 */
void PowerCC283X_standbyPolicy(void);

/*!
 * @brief Select LFOSC as LFCLK source
 *
 * Turn on the LFOSC and choose it as LFCLK source. Once LFCLK has switched, the
 * #PowerLPF3_LFCLK_SWITCHED notification will be issued and all subscribers to
 * this event will be notified.
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
 *  - Configure RTC to wake up the device #PowerCC283X_WAKEDELAYSTANDBY
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

void PowerCC283X_schedulerDisable(void);
void PowerCC283X_schedulerRestore(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_power_PowerCC283X__include */
