/*
 * Copyright (c) 2022-2023, Texas Instruments Incorporated
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
 *  ## Implementation #
 *  This header file defines the power resources, constraints, events, sleep
 *  states and transition latencies for CC27XX.
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
#define PowerCC27XX_RESUMETIMESTANDBY 400

/*! The total latency to reserve for entry to and exit from STANDBY (usec). */
#define PowerCC27XX_TOTALTIMESTANDBY 500

/*! The initial delay when waking from STANDBY (usec). */
#define PowerCC27XX_WAKEDELAYSTANDBY 100

// TODO: Remove these once they have been defined in driverlib (LPRF_DOC-118)
#define CLKCTL_DESCEX0_HSCMP_S 26
#define CLKCTL_DESCEX1_LGPT4_S 0
#define CLKCTL_DESCEX1_LGPT5_S 1

/* \cond */
/* The control of the peripherals are split between multiple groups.
 * These defines are used to differentiate between the groups.
 * The bits in the PowerCC27XX_PERIPH_GROUP_M mask is used to store the group id,
 * and the bits in the PowerCC27XX_PERIPH_BIT_INDEX_M mask is used to store the
 * bit index shift value in the register for the given group.
 */
#define PowerCC27XX_PERIPH_GROUP_M       0xFF00
#define PowerCC27XX_PERIPH_GROUP_CLKCTL0 0x0000
#define PowerCC27XX_PERIPH_GROUP_CLKCTL1 0x0100
#define PowerCC27XX_PERIPH_GROUP_LRFD    0x0200
#define PowerCC27XX_PERIPH_BIT_INDEX_M   0x00FF

/* \endcond */
/* Resource IDs */

/*!< Resource ID: General Purpose I/O */
#define PowerLPF3_PERIPH_GPIO (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_GPIO_S)

/*!< Resource ID: UART 0 */
#define PowerLPF3_PERIPH_UART0 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_UART0_S)

/*!< Resource ID: UART 1 */
#define PowerLPF3_PERIPH_UART1 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_UART0_S)

/*!< Resource ID: I2C 0 */
#define PowerLPF3_PERIPH_I2C0 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_I2C0_S)

/*!< Resource ID: SPI 0 */
#define PowerLPF3_PERIPH_SPI0 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_SPI0_S)

/*!< Resource ID: SPI 1 */
#define PowerLPF3_PERIPH_SPI1 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_SPI1_S)

/*!< Resource ID: ADC */
#define PowerLPF3_PERIPH_ADC0 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_ADC0_S)

/*!< Resource ID: AES Security Module */
#define PowerLPF3_PERIPH_AES (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_LAES_S)

/*!< Resource ID: uDMA Controller */
#define PowerLPF3_PERIPH_DMA (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_DMA_S)

/*!< Resource ID: I2S */
#define PowerLPF3_PERIPH_I2S (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_I2S_S)

/*!< Resource ID: High-Speed Comparator */
#define PowerLPF3_PERIPH_HSCMP (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_HSCMP_S)

/*!< Resource ID: General Purpose Timer 0 */
#define PowerLPF3_PERIPH_LGPT0 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_LGPT0_S)

/*!< Resource ID: General Purpose Timer 1 */
#define PowerLPF3_PERIPH_LGPT1 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_LGPT1_S)

/*!< Resource ID: General Purpose Timer 2 */
#define PowerLPF3_PERIPH_LGPT2 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_LGPT2_S)

/*!< Resource ID: General Purpose Timer 3 */
#define PowerLPF3_PERIPH_LGPT3 (PowerCC27XX_PERIPH_GROUP_CLKCTL0 | CLKCTL_DESCEX0_LGPT3_S)

/*!< Resource ID: General Purpose Timer 4 */
#define PowerLPF3_PERIPH_LGPT4 (PowerCC27XX_PERIPH_GROUP_CLKCTL1 | CLKCTL_DESCEX1_LGPT4_S)

/*!< Resource ID: General Purpose Timer 5 */
#define PowerLPF3_PERIPH_LGPT5 (PowerCC27XX_PERIPH_GROUP_CLKCTL1 | CLKCTL_DESCEX1_LGPT5_S)

/*!< Resource ID: Hardware Security Module */
#define PowerLPF3_PERIPH_HSM (PowerCC27XX_PERIPH_GROUP_CLKCTL1 | CLKCTL_DESCEX1_HSM_S)

/*!< Resource ID: Vector Computational Engine */
#define PowerLPF3_PERIPH_VCE (PowerCC27XX_PERIPH_GROUP_CLKCTL1 | CLKCTL_DESCEX1_VCE_S)

/*!< Resource ID: MCAN */
#define PowerLPF3_PERIPH_MCAN (PowerCC27XX_PERIPH_GROUP_CLKCTL1 | CLKCTL_DESCEX1_MCAN_S)

/*!< Resource ID: LRFD Tracer */
#define PowerLPF3_PERIPH_LFRD_TRC (PowerCC27XX_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_TRC_S)

/*!< Resource ID: LRFD S2R RAM */
#define PowerLPF3_PERIPH_LFRD_S2RRAM (PowerCC27XX_PERIPH_GROUP_LRFD | LRFDDBELL_CLKCTL_S2RRAM_S)

/* \cond */
/* Number of resources in the different resource groups. This is possibly larger
 * than the peripheral instance count on various device variants. This is
 * because some indexes might not be used for a given variant, resulting in a
 * sparse table.
 */
#define PowerCC27XX_NUMRESOURCES_CLKCTL0 31
#define PowerCC27XX_NUMRESOURCES_CLKCTL1 7
#define PowerCC27XX_NUMRESOURCES_LRFD    12

/* \endcond */

#define PowerLPF3_STANDBY 0x1 /*!< The STANDBY sleep state */
/* \cond */
/* Internal flags for enabling/disabling resources */
#define PowerLPF3_ENABLE  1
#define PowerLPF3_DISABLE 0
/* \endcond */

/* Constraints
 *
 * Constraints restrict a specific system behavior from occurring or guarantee
 * a specified effect until released.
 */

/*!< Constraint: Disallow a transition to the SHUTDOWN state */
#define PowerLPF3_DISALLOW_SHUTDOWN 0

/*!< Constraint: Disallow a transition to the STANDBY sleep state */
#define PowerLPF3_DISALLOW_STANDBY 1

/*!< Constraint: Disallow a transition to the IDLE sleep state */
#define PowerLPF3_DISALLOW_IDLE 2

/*!< Constraint: Flash memory needs to enabled during IDLE */
#define PowerLPF3_NEED_FLASH_IN_IDLE 3

/* \cond */
#define PowerCC27XX_NUMCONSTRAINTS 4 /* Number of constraints supported */
/* \endcond */

/*
 *  Events
 *
 *  Each event must be a power of two and must be sequential
 *  without any gaps.
 */

/*!< Power event: The device is entering the STANDBY sleep state */
#define PowerLPF3_ENTERING_STANDBY (1 << 0)

/*!< Power event: The device is entering the SHUTDOWN state */
#define PowerLPF3_ENTERING_SHUTDOWN (1 << 1)

/*!< Power event: The device is waking up from the STANDBY sleep state */
#define PowerLPF3_AWAKE_STANDBY (1 << 2)

/*!< Power event: The high frequency (HF) crystal oscillator is now available
 *   for use (HFXT) by the digital domain
 */
#define PowerLPF3_HFXT_AVAILABLE (1 << 3)

/*!< Power event: The system has switched to the low frequency clock source
 *   configured in CCFG
 */
#define PowerLPF3_LFCLK_SWITCHED (1 << 4)

/* \cond */
#define PowerCC27XX_NUMEVENTS 5 /* Number of events supported */
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
    /*! Device reset trggered by software writing to RSTCTL.SYSRST */
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
} PowerLPF3_ResetReason;

/*!
 *  @brief  The wait for interrupt (WFI) policy
 *
 *  This is a lightweight Power Policy which simply invokes CPU wait for
 *  interrupt.
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
static inline PowerLPF3_ResetReason PowerLPF3_getResetReason(void)
{
    return (PowerLPF3_ResetReason)PMCTLGetResetReason();
}

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
static inline void PowerLPF3_releaseLatches(void)
{
    HWREG(PMCTL_BASE + PMCTL_O_SLPCTL) = PMCTL_SLPCTL_SLPN_DIS;
}

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
 * @pre Power_init()
 * @sa PowerLPF3_selectLFXT()
 */
void PowerLPF3_selectLFOSC(void);

/*!
 * @brief Select LFXT as LFCLK source
 *
 * Turn on the LFXT and choose it as LFCLK source. Once LFCLK has switched,
 * the #PowerLPF3_LFCLK_SWITCHED notification will be issued and all
 * subscribers to this event will be notified.
 *
 * @pre Power_init()
 * @sa PowerLPF3_selectLFOSC()
 */
void PowerLPF3_selectLFXT(void);

void PowerCC27XX_schedulerDisable(void);
void PowerCC27XX_schedulerRestore(void);

#define Power_getPerformanceLevel(void)  0
#define Power_setPerformanceLevel(level) Power_EFAIL

#ifdef __cplusplus
}
#endif

#endif /* POWER_CC27XX_ */
