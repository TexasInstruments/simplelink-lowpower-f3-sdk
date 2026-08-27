/*
 * Copyright (c) 2025-2026 Texas Instruments Incorporated
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

#include <stdint.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/Power.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/prcm.h)
#include DeviceFamily_constructPath(inc/hw_lrfddbell.h)

#ifdef __cplusplus
extern "C" {
#endif

/*! The latency to reserve for resume from STANDBY (usec). */
#define PowerCC283X_RESUMETIMESTANDBY (400U)

/*! The total latency to reserve for entry to and exit from STANDBY (usec). */
#define PowerCC283X_TOTALTIMESTANDBY (500U)

/*! The initial delay when waking from STANDBY (usec). */
#define PowerCC283X_WAKEDELAYSTANDBY (150U)

/* \cond NODOC */
/* The control of the peripherals are split between multiple groups.
 * These defines are used to differentiate between the groups.
 * The bits in the PowerCC283X_PERIPH_GROUP_M mask are used to store the group
 * id, and the bits in the PowerCC283X_PERIPH_INDEX_M mask are used to store
 * index used for that specific group. The interpretation of the index is
 * specific to the group.
 */
#define PowerCC283X_PERIPH_GROUP_M    (0xFF00U)
#define PowerCC283X_PERIPH_GROUP_PRCM (0x0000U)
#define PowerCC283X_PERIPH_GROUP_LRFD (0x0100U)
#define PowerCC283X_PERIPH_INDEX_M    (0x00FFU)

/* \endcond */

/* \cond NODOC*/
typedef uint16_t PowerLPF4_Resource; /* Power resource identifier */
/* \endcond */

/* Resource IDs */

/*! Resource ID: General Purpose I/O */
#define PowerLPF4_PERIPH_GPIO (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_GPIO))

/*! Resource ID: UART 0 */
#define PowerLPF4_PERIPH_UART0 (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_UART0))

/*! Resource ID: UART 1 */
#define PowerLPF4_PERIPH_UART1 (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_UART1))

/*! Resource ID: I2C 0 */
#define PowerLPF4_PERIPH_I2C0 (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_I2C0))

/*! Resource ID: SPI 0 */
#define PowerLPF4_PERIPH_SPI0 (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_SPI0))

/*! Resource ID: ADC 0 */
#define PowerLPF4_PERIPH_ADC0 (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_ADC0))

/*! Resource ID: AES Security Module */
#define PowerLPF4_PERIPH_AES (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_LAES))

/*! Resource ID: uDMA Controller */
#define PowerLPF4_PERIPH_DMA (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_DMA))

/*! Resource ID: TRNG */
#define PowerLPF4_PERIPH_TRNG (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_TRNG))

/*! Resource ID: General Purpose Timer 0 */
#define PowerLPF4_PERIPH_LGPT0 (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_LGPT0))

/*! Resource ID: General Purpose Timer 1 */
#define PowerLPF4_PERIPH_LGPT1 (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_LGPT1))

/*! Resource ID: General Purpose Timer 2 */
#define PowerLPF4_PERIPH_LGPT2 (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_LGPT2))

/*! Resource ID: General Purpose Timer 3 */
#define PowerLPF4_PERIPH_LGPT3 (PowerCC283X_PERIPH_GROUP_PRCM | (PRCM_CLOCK_LGPT3))

/*! @brief Resource ID: LRFD
 *
 *  This can used to enable the clock for LRFD, without having to enable the
 *  clock for any of the sub-clocks in LRFD.
 *
 *  To enable specific sub-clocks in LRFD, sub-clock specific resource IDs can
 *  be used. Using those resource IDs will automatically enable the main LRFD
 *  clock, so this resource ID (#PowerLPF4_PERIPH_LRFD) is not necessary.
 *  The sub-clock resource IDs are:
 *  - #PowerLPF4_PERIPH_LRFD_CEE
 *  - #PowerLPF4_PERIPH_LRFD_PBE
 *  - #PowerLPF4_PERIPH_LRFD_MDM
 *  - #PowerLPF4_PERIPH_LRFD_RFE
 *  - #PowerLPF4_PERIPH_LRFD_S2R
 *  - #PowerLPF4_PERIPH_LRFD_TRC
 *  - #PowerLPF4_PERIPH_LRFD_PBERAM
 *  - #PowerLPF4_PERIPH_LRFD_DBRAM
 *  - #PowerLPF4_PERIPH_LRFD_MCERAM
 *  - #PowerLPF4_PERIPH_LRFD_RFERAM
 *  - #PowerLPF4_PERIPH_LRFD_CEERAM
 *  - #PowerLPF4_PERIPH_LRFD_MDMRAM
 */
#define PowerLPF4_PERIPH_LRFD (PowerCC283X_PERIPH_GROUP_LRFD | LRFD_CLK_DEP_LRFD_S)

/*! Resource ID: LRFD CEE */
#define PowerLPF4_PERIPH_LRFD_CEE (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_CEE_S)

/*! Resource ID: LRFD PBE */
#define PowerLPF4_PERIPH_LRFD_PBE (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_PBE_S)

/*! Resource ID: LRFD Modem */
#define PowerLPF4_PERIPH_LRFD_MDM (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_MDM_S)

/*! Resource ID: LRFD RFE */
#define PowerLPF4_PERIPH_LRFD_RFE (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_RFE_S)

/*! Resource ID: LRFD Samples2RAM */
#define PowerLPF4_PERIPH_LRFD_S2R (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_S2R_S)

/*! Resource ID: LRFD Tracer */
#define PowerLPF4_PERIPH_LRFD_TRC (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_TRC_S)

/*! Resource ID: LRFD PBE RAM */
#define PowerLPF4_PERIPH_LRFD_PBERAM (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_PBERAM_S)

/*! Resource ID: LRFD Doorbell RAM */
#define PowerLPF4_PERIPH_LRFD_DBRAM (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_DBRAM_S)

/*! Resource ID: LRFD MCE RAM */
#define PowerLPF4_PERIPH_LRFD_MCERAM (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_MCERAM_S)

/*! Resource ID: LRFD RFE RAM */
#define PowerLPF4_PERIPH_LRFD_RFERAM (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_RFERAM_S)

/*! Resource ID: LRFD CEE RAM */
#define PowerLPF4_PERIPH_LRFD_CEERAM (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_CEERAM_S)

/*! Resource ID: LRFD MDMRAM */
#define PowerLPF4_PERIPH_LRFD_MDMRAM (PowerCC283X_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_MDMRAM_S)

/* \cond NODOC */
/* Number of resources in the different resource groups. This is possibly larger
 * than the peripheral instance count on various device variants. This is
 * because some indexes might not be used for a given variant, resulting in a
 * sparse table.
 */
#define PowerCC283X_NUMRESOURCES_PRCM ((PRCM_CLOCK_MAX_VALUE) + 1U)
#define PowerCC283X_NUMRESOURCES_LRFD (16U)

/* \endcond */

#define PowerLPF4_STANDBY (0x1U) /*!< The STANDBY sleep state */

/* Constraints
 *
 * Constraints restrict a specific system behavior from occurring or guarantee
 * a specified effect until released.
 */

/*! Constraint: Disallow a transition to the SHUTDOWN state */
#define PowerLPF4_DISALLOW_SHUTDOWN (0U)

/*! Constraint: Disallow a transition to the STANDBY sleep state */
#define PowerLPF4_DISALLOW_STANDBY (1U)

/*! Constraint: Disallow a transition to the IDLE sleep state */
#define PowerLPF4_DISALLOW_IDLE (2U)

/*! Constraint: Flash memory needs to enabled during IDLE */
#define PowerLPF4_NEED_FLASH_IN_IDLE (3U)

/* \cond NODOC */
#define PowerCC283X_NUMCONSTRAINTS (4U) /* Number of constraints supported */
/* \endcond */

/*
 *  Events
 *
 *  Each event must be a power of two and must be sequential
 *  without any gaps.
 */

/*! Power event: The device is entering the STANDBY sleep state */
#define PowerLPF4_ENTERING_STANDBY ((uint_fast16_t)((uint_fast16_t)1U << 0))

/*! Power event: The device is entering the SHUTDOWN state */
#define PowerLPF4_ENTERING_SHUTDOWN ((uint_fast16_t)((uint_fast16_t)1U << 1))

/*! Power event: The device is waking up from the STANDBY sleep state */
#define PowerLPF4_AWAKE_STANDBY ((uint_fast16_t)((uint_fast16_t)1U << 2))

/*! Power event: The high frequency (HF) crystal oscillator is now available
 *   for use (HFXT) by the digital domain
 */
#define PowerLPF4_HFXT_AVAILABLE ((uint_fast16_t)((uint_fast16_t)1U << 3))

/*! Power event: The system has switched to the low frequency clock source
 *   configured in CCFG
 */
#define PowerLPF4_LFCLK_SWITCHED ((uint_fast16_t)((uint_fast16_t)1U << 4))

/* \cond NODOC */
#define PowerCC283X_NUMEVENTS (5U) /* Number of events supported */
/* \endcond */

/* \cond NODOC
 * Some peripherals for the CC283x devices use the LPF3 driver implementations
 * which are using PowerLPF3_* defines. To be able to re-use these drivers,
 * the PowerLPF4_* defines for this device are also defined as PowerLPF3_*
 * defines.
 */
#define PowerLPF3_PERIPH_GPIO        PowerLPF4_PERIPH_GPIO
#define PowerLPF3_PERIPH_UART0       PowerLPF4_PERIPH_UART0
#define PowerLPF3_PERIPH_UART1       PowerLPF4_PERIPH_UART1
#define PowerLPF3_PERIPH_I2C0        PowerLPF4_PERIPH_I2C0
#define PowerLPF3_PERIPH_SPI0        PowerLPF4_PERIPH_SPI0
#define PowerLPF3_PERIPH_ADC0        PowerLPF4_PERIPH_ADC0
#define PowerLPF3_PERIPH_AES         PowerLPF4_PERIPH_AES
#define PowerLPF3_PERIPH_DMA         PowerLPF4_PERIPH_DMA
#define PowerLPF3_PERIPH_TRNG        PowerLPF4_PERIPH_TRNG
#define PowerLPF3_PERIPH_LGPT0       PowerLPF4_PERIPH_LGPT0
#define PowerLPF3_PERIPH_LGPT1       PowerLPF4_PERIPH_LGPT1
#define PowerLPF3_PERIPH_LGPT2       PowerLPF4_PERIPH_LGPT2
#define PowerLPF3_PERIPH_LGPT3       PowerLPF4_PERIPH_LGPT3
#define PowerLPF3_PERIPH_LRFD        PowerLPF4_PERIPH_LRFD
#define PowerLPF3_PERIPH_LRFD_PBE    PowerLPF4_PERIPH_LRFD_PBE
#define PowerLPF3_PERIPH_LRFD_MDM    PowerLPF4_PERIPH_LRFD_MDM
#define PowerLPF3_PERIPH_LRFD_RFE    PowerLPF4_PERIPH_LRFD_RFE
#define PowerLPF3_PERIPH_LRFD_S2R    PowerLPF4_PERIPH_LRFD_S2R
#define PowerLPF3_PERIPH_LRFD_TRC    PowerLPF4_PERIPH_LRFD_TRC
#define PowerLPF3_PERIPH_LRFD_PBERAM PowerLPF4_PERIPH_LRFD_PBERAM
#define PowerLPF3_PERIPH_LRFD_MCERAM PowerLPF4_PERIPH_LRFD_MCERAM
#define PowerLPF3_PERIPH_LRFD_RFERAM PowerLPF4_PERIPH_LRFD_RFERAM
#define PowerLPF3_PERIPH_LRFD_DSBRAM PowerLPF4_PERIPH_LRFD_DSBRAM
#define PowerLPF3_PERIPH_LRFD_BUFRAM PowerLPF4_PERIPH_LRFD_BUFRAM
#define PowerLPF3_PERIPH_LRFD_S2RRAM PowerLPF4_PERIPH_LRFD_S2RRAM
#define PowerLPF3_STANDBY            PowerLPF4_STANDBY
#define PowerLPF3_DISALLOW_SHUTDOWN  PowerLPF4_DISALLOW_SHUTDOWN
#define PowerLPF3_DISALLOW_STANDBY   PowerLPF4_DISALLOW_STANDBY
#define PowerLPF3_DISALLOW_IDLE      PowerLPF4_DISALLOW_IDLE
#define PowerLPF3_NEED_FLASH_IN_IDLE PowerLPF4_NEED_FLASH_IN_IDLE
#define PowerLPF3_ENTERING_STANDBY   PowerLPF4_ENTERING_STANDBY
#define PowerLPF3_ENTERING_SHUTDOWN  PowerLPF4_ENTERING_SHUTDOWN
#define PowerLPF3_AWAKE_STANDBY      PowerLPF4_AWAKE_STANDBY
#define PowerLPF3_HFXT_AVAILABLE     PowerLPF4_HFXT_AVAILABLE
#define PowerLPF3_LFCLK_SWITCHED     PowerLPF4_LFCLK_SWITCHED
/* \endcond */

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
    PowerLPF4_RESET_UNKNOWN = UINT32_MAX,
} PowerLPF4_ResetReason;

/*!
 *  @brief  The wait for interrupt (WFI) policy
 *
 *  This is a lightweight Power Policy which simply invokes CPU wait for
 *  interrupt.
 *
 *  This policy can be selected statically via the policyFxn pointer in the
 *  PowerCC283X_Config structure, or dynamically at runtime, via
 *  Power_setPolicy().
 */
void PowerCC283X_doWFI(void);

/*!
 * @brief Returns the reason for the most recent reset or wakeup
 *
 * @return #PowerLPF4_ResetReason
 * @pre Power_shutdown()
 * @post PowerLPF4_releaseLatches()
 */
PowerLPF4_ResetReason PowerLPF4_getResetReason(void);

/*!
 *  @brief  The STANDBY Power Policy
 *
 *  This is an aggressive Power Policy, which considers active constraints,
 *  sleep state transition latencies, and time until the next scheduled
 *  work, and automatically transitions the device into the deepest sleep state
 *  possible.
 *
 *  The first goal is to enter STANDBY; if that is not appropriate
 *  given current conditions (e.g., the sleep transition latency is greater
 *  greater than the time until the next scheduled Clock event), then
 *  the secondary goal is the IDLE state; if that is disallowed (e.g., if
 *  the PowerLPF4_DISALLOW_IDLE constraint is declared), then the policy
 *  will fallback and simply invoke WFI, to clock gate the CPU until the next
 *  interrupt.
 *
 *  In order for this policy to run, it must be selected as the Power
 *  Policy (either by being specified as the 'policyFxn' in the
 *  PowerCC283X_Config structure, or specified at runtime with
 *  Power_setPolicy()), and the Power Policy must be enabled (either via
 *  'enablePolicy' in the PowerCC283X_Config structure, or via a call to
 *  Power_enablePolicy() at runtime).
 */
void PowerCC283X_standbyPolicy(void);

/*!
 * @brief Select LFOSC as LFCLK source
 *
 * Turn on the LFOSC and choose it as LFCLK source. Once LFCLK has switched, the
 * #PowerLPF4_LFCLK_SWITCHED notification will be issued and all subscribers to
 * this event will be notified.
 *
 * @warning Dynamic switching between LF clocks is not supported. Once one of
 * PowerLPF4_selectLFOSC(), PowerLPF4_selectLFXT() or PowerLPF4_selectEXTLF()
 * is called in an application, the other two may not be invoked thereafter.
 *
 * @pre Power_init()
 * @sa PowerLPF4_selectLFXT()
 * @sa PowerLPF4_selectEXTLF()
 */
void PowerLPF4_selectLFOSC(void);

/*!
 * @brief Select LFXT as LFCLK source
 *
 * Turn on the LFXT and choose it as LFCLK source. Once LFCLK has switched,
 * the #PowerLPF4_LFCLK_SWITCHED notification will be issued and all
 * subscribers to this event will be notified.
 *
 * @warning Dynamic switching between LF clocks is not supported. Once one of
 * PowerLPF4_selectLFOSC(), PowerLPF4_selectLFXT() or PowerLPF4_selectEXTLF()
 * is called in an application, the other two may not be invoked thereafter.
 *
 * @pre Power_init()
 * @sa PowerLPF4_selectLFOSC()
 * @sa PowerLPF4_selectEXTLF()
 */
void PowerLPF4_selectLFXT(void);

/*!
 * @brief Select EXTLF as LFCLK source
 *
 * Choose an external 31.25 kHz square wave as the LFCLK source as input.
 * Once LFCLK has switched, the #PowerLPF4_LFCLK_SWITCHED notification
 * will be issued and all subscribers to this event will be notified.
 *
 * This function requires the following symbols to be defined.
 *  - \c PowerLPF4_extlfPin (uint8_t): The DIO number of the pin to be used as the EXTLF pin.
 *  - \c PowerLPF4_extlfPinMux (uint8_t): Mux value used to mux the EXTLF signal to \c PowerLPF4_extlfPin.
 * If using SysConfig, the symbols will be defined in ti_drivers_config.c.
 *
 * @warning Dynamic switching between LF clocks is not supported. Once one of
 * PowerLPF4_selectLFOSC(), PowerLPF4_selectLFXT() or PowerLPF4_selectEXTLF()
 * is called in an application, the other two may not be invoked thereafter.
 *
 * @pre Power_init()
 * @sa PowerLPF4_selectLFOSC()
 * @sa PowerLPF4_selectLFXT()
 */
void PowerLPF4_selectEXTLF(void);

/*!
 *  @brief  Transition the device into standby and configure RTC to wakeup
 *          the device ahead of a specified time.
 *
 *  This function is called from the power policy when the decision has been
 *  made to put the device in standby. This function returns to the caller
 *  (the policy function) after the device wakes up from standby.
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
int_fast16_t PowerLPF4_sleep(uint32_t nextEventTimeUs);

void PowerCC283X_schedulerDisable(void);
void PowerCC283X_schedulerRestore(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_power_PowerCC283X__include */
