/*
 * Copyright (c) 2024-2025, Texas Instruments Incorporated
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
/*
 *  ======== PowerCC27XX_s.c ========
 */

#include "interrupt.h"

#include <stdbool.h>
#include <string.h>

#include <third_party/tfm/secure_fw/spm/include/utilities.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/DebugP.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC27XX.h>
#include <ti/drivers/power/PowerCC27XX_s.h>
#include <ti/drivers/Temperature.h>

#include <ti/drivers/tfm/SecureCallback.h>
#include <ti/drivers/tfm/SecureCallback_s.h>

#include <ti/drivers/utils/Math.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_clkctl.h)
#include DeviceFamily_constructPath(inc/hw_evtull.h)
#include DeviceFamily_constructPath(inc/hw_ckmd.h)
#include DeviceFamily_constructPath(inc/hw_rtc.h)
#include DeviceFamily_constructPath(inc/hw_systim.h)
#include DeviceFamily_constructPath(inc/hw_pmctl.h)
#include DeviceFamily_constructPath(inc/hw_ioc.h)
#include DeviceFamily_constructPath(inc/hw_icb.h)
#include DeviceFamily_constructPath(inc/hw_fcfg.h)
#include DeviceFamily_constructPath(driverlib/ckmd.h)
#include DeviceFamily_constructPath(driverlib/evtsvt.h)
#include DeviceFamily_constructPath(driverlib/hapi.h)
#include DeviceFamily_constructPath(driverlib/gpio.h)
#include DeviceFamily_constructPath(driverlib/lrfd.h)
#include DeviceFamily_constructPath(driverlib/pmctl.h)
#include DeviceFamily_constructPath(driverlib/ull.h)
#include DeviceFamily_constructPath(cmsis/core/cmsis_compiler.h)

/* Type definitions */

/* Function type to qualify a LFCLK source.
 * To be called from PowerCC27XX_oscillatorISR()
 * maskedStatus is CMKD.MIS at the entry of PowerCC27XX_oscillatorISR()
 *
 * The function should return true if the qualification process is done,
 * otherwise false.
 */
typedef bool (*PowerLPF3_LfclkQualFxn)(uint32_t maskedStatus);

/* LFCLK TDC trigger sources used for LFCLK qualification. */
typedef enum
{
    PowerCC27XX_LFXT = CKMD_TRIGSRC_STOP_POL_HIGH | CKMD_TRIGSRC_STOP_SRC_LFXT | CKMD_TRIGSRC_START_POL_HIGH |
                       CKMD_TRIGSRC_START_SRC_LFXT,
} PowerCC27XX_LfclkTdcTrigSrc;

/* Forward declarations */
psa_flih_result_t cpuirq3_irqn_flih(void);
static int_fast16_t PowerCC27XX_notify(uint_fast16_t eventType);
static int_fast16_t PowerCC27XX_notify_s(uint_fast16_t eventType);
static int_fast16_t PowerCC27XX_notify_ns(uint_fast16_t eventType);
int_fast16_t PowerCC27XX_s_setDependency(Power_Resource resourceId, PowerCC27XX_SecurityState secState);
int_fast16_t PowerCC27XX_s_releaseDependency(Power_Resource resourceId, PowerCC27XX_SecurityState secState);
int_fast16_t PowerCC27XX_s_setNonSecureConstraintPointers(
    volatile const uint32_t *constraintMask,
    volatile const uint8_t (*const constraintCounts)[PowerCC27XX_NUMCONSTRAINTS]);
bool PowerCC27XX_isValidResourceId(Power_Resource resourceId);
uint_fast16_t PowerCC27XX_s_getCurrentEventsMask(void);
int_fast16_t PowerCC27XX_s_setNonSecureNotificationHandle(SecureCallback_Handle handle);

static void PowerCC27XX_oscillatorISR(uintptr_t arg);
static void PowerCC27XX_enterStandby(void);
int_fast16_t PowerCC27XX_s_getDependencyCount(Power_Resource resourceId, PowerCC27XX_SecurityState secState);
static void PowerCC27XX_s_setDependencyCount(Power_Resource resourceId,
                                             uint8_t count,
                                             PowerCC27XX_SecurityState secState);
static int_fast16_t PowerCC27XX_s_getConstraintCount(uint_fast16_t constraintId, PowerCC27XX_SecurityState secState);
static void PowerCC27XX_startHFXT(void);
static uint_fast16_t PowerCC27XX_getResourceGroup(Power_Resource resourceId);
static uint8_t PowerCC27XX_getResourceBitIndex(Power_Resource resourceId);
static uint_fast16_t PowerCC27XX_getIocAddr(uint_fast16_t iocIndex);

static void PowerCC27XX_startLfclkTdcMeasurement(PowerCC27XX_LfclkTdcTrigSrc tdcTrigSrc);
static void PowerCC27XX_restartLfclkTdcMeasurement(void);
static bool PowerCC27XX_lfxtQual(uint32_t maskedStatus);

/* List of limitations:
 *  - No HFXT amplitude compensation (requires ClockP)
 *  - No SWTCXO (requires Temperature driver)
 */

/* Externs */
extern const uint_least8_t GPIO_pinLowerBound;
extern const uint_least8_t GPIO_pinUpperBound;

/* Function Macros */

/* Workaround for missing enums for CKMD_AFTRACKCTL_RATIO */
#define CKMD_AFTRACKCTL_RATIO__90P3168MHZ 0x0880DEE9U
#define CKMD_AFTRACKCTL_RATIO__98P304MHZ  0x07D00000U

/* Initial LFINC value for the LFINC filter. The value is representing the
 * nominal LFOSC frequency of 32.768 kHz in us with 16 fractional bits.
 */
#define LFINCCTL_INT_START_VALUE (0x1E8480U)

#define SYSTIMER_CHANNEL_COUNT (5U)

/* Max number of ClockP ticks into the future supported by this ClockP
 * implementation.
 * Under the hood, ClockP uses the SysTimer whose events trigger immediately if
 * the compare value is less than 2^22 systimer ticks in the past
 * (4.194sec at 1us resolution). Therefore, the max number of SysTimer ticks you
 * can schedule into the future is 2^32 - 2^22 - 1 ticks (~= 4290 sec at 1us
 * resolution).
 */
#define MAX_SYSTIMER_DELTA (0xFFBFFFFFU)

/* Mask of events that are handled synchronously for
 * subscribers on the non-secure side.
 */
#define SYNCHRONOUS_NS_EVENTS (PowerLPF3_ENTERING_STANDBY | PowerLPF3_AWAKE_STANDBY | PowerLPF3_ENTERING_SHUTDOWN)

/* Static Globals */

/* LFCLK Qualification function */
static PowerLPF3_LfclkQualFxn PowerLPF3_lfclkQualFxn = NULL;

/* Array to maintain secure constraint reference counts.
 * The counts for the security states are summed in Power_getConstraintCount().
 * Declare volatile variable to ensure the toolchain does not use
 * stack for the variable and does not optimize this memory allocation away.
 */
static volatile uint8_t constraintCounts_s[PowerCC27XX_NUMCONSTRAINTS] = {0};

/* Pointer to array to maintain non-secure constraint reference counts.
 * The counts for the security states are summed in Power_getConstraintCount().
 * The pointer is declared volatile since it can be changed asynchronously by
 * the NS side and it is pointing to volatile memory, because the array on
 * the non-secure side is volatile and can be changed asynchronously.
 */
static volatile const uint8_t (*volatile constraintCounts_ns)[PowerCC27XX_NUMCONSTRAINTS] = NULL;

/* Secure mask of Power constraints for quick access.
 * Declare volatile variable to ensure the toolchain does not use
 * stack for the variable and does not optimize this memory allocation away.
 */
static volatile uint32_t constraintMask_s = 0;

/* Pointer to non-secure mask of Power constraints for quick access.
 * The pointer is declared volatile since it can be changed asynchronously by
 * the NS side and it is pointing to volatile memory, because the mask on
 * the non-secure side is volatile and can be changed asynchronously.
 */
static volatile const uint32_t *volatile constraintMask_ns = NULL;

/* Arrays to maintain resource dependency reference counts.
 * Each resource group will have an array associated with it, and the arrays can
 * be indexed using the bit index shift value encoded in the resource ID.
 * There is one array per security state.
 * The counts for the security states are summed in Power_getDependencyCount().
 * Declare volatile variable to ensure the toolchain does not use
 * stack for the variable and does not optimize this memory allocation away.
 */
static volatile uint8_t resourceCountsClkctl0[PowerCC27XX_NUM_SECURITY_STATES][PowerCC27XX_NUMRESOURCES_CLKCTL0];
static volatile uint8_t resourceCountsClkctl1[PowerCC27XX_NUM_SECURITY_STATES][PowerCC27XX_NUMRESOURCES_CLKCTL1];
static volatile uint8_t resourceCountsLrfd[PowerCC27XX_NUM_SECURITY_STATES][PowerCC27XX_NUMRESOURCES_LRFD];

/* Power state of the system. Idle, active, standby, etc */
static uint8_t powerState = Power_ACTIVE;

/* Secure event notification list */
static List_List notifyList_s;

/* Events mask of pending events not yet processed by the non-secure
 * side. The mask does not include events synchronously posted to the non-secure
 * side (see #SYNCHRONOUS_NS_EVENTS).
 */
static uint_fast16_t notifyEventsMask = 0;

/* The SecureCallback handle used for posting notifications to the non-secure
 * side. This is set by the non-secure side through veneers.
 */
static SecureCallback_Handle notifySecureCallbackHandle = NULL;

/* Non-static Globals */

/* ****************** Power APIs ******************** */

/*
 *  ======== cpuirq3_irqn_flih ========
 */
psa_flih_result_t cpuirq3_irqn_flih(void)
{
    HwiP_dispatchInterrupt(INT_CPUIRQ3);

    return PSA_FLIH_NO_SIGNAL;
}

/*
 *  ======== Power_init ========
 * Note, unlike the NS Power_init() and the S-only Power_init(), this S
 * Power_init() function is not guaranteed to be called while interrupts are
 * disabled before the OS is started.
 */
int_fast16_t Power_init(void)
{
    HwiP_Params ckmHwiParams;
    int_fast16_t status;
    uintptr_t key = HwiP_disable();

    static HwiP_Struct ckmHwi;         /* Interrupt for handling clock switching */
    static bool isInitialized = false; /* Has the Power driver been initialized */

    /* If this function has already been called, just return */
    if (isInitialized)
    {
        HwiP_restore(key);
        status = Power_SOK;
    }
    else
    {
        isInitialized = true;
        HwiP_restore(key);

        /* Construct the CKM hwi responsible for oscillator related events.
         * Since there is no dedicated CKM interrupt line, we need to mux one of
         * the configurable lines to the CKM.
         * CPUIRQ3 is dedicated to this purpose.
         * Do not enable interrupt before end of the function.
         * Ignore return value of HwiP_construct() since it is never returning
         * NULL.
         */
        HwiP_Params_init(&ckmHwiParams);
        ckmHwiParams.enableInt = false;
        (void)HwiP_construct(&ckmHwi, INT_CPUIRQ3, PowerCC27XX_oscillatorISR, &ckmHwiParams);

        /* Mux the AON_CKM_COMB event to CPUIRQ3 */
        EVTSVTConfigureEvent(EVTSVT_SUB_CPUIRQ3, EVTSVT_PUB_AON_CKM_COMB);

        /* Clear VDDIOPGIO to disable pad power. This is a workaround for a
         * CC27xxx10 ROM bug that leaves the bit set rather than cleared. For other
         * devices, this workaround has no effect.
         */
        HWREG(PMCTL_BASE + PMCTL_O_AONRCLR1) |= PMCTL_AONRCLR1_VDDIOPGIO_CLR;

        /* Enable a selection of CKM signals as interrupt sources. For now,
         * we will stick to AMPSETTLED since it is related to existing notification
         * and HFXTFAULT and TRACKREFLOSS to be able to handle HFXT clock loss.
         * TRACKREFLOSS may occur when entering and exiting fake standby with the
         * debugger attached.
         */
        HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMSET_AMPSETTLED | CKMD_IMSET_HFXTFAULT | CKMD_IMSET_TRACKREFLOSS;

        /* Use RTC channel 0 in compare mode. Channel 1 could be used for other
         * purposes.
         */
        HWREG(RTC_BASE + RTC_O_IMSET) = RTC_IMSET_EV0_SET;

        /* Configure RTC to halt when CPU stopped during debug */
        HWREG(RTC_BASE + RTC_O_EMU) = RTC_EMU_HALT_STOP;

        /* Configure SysTimer to halt when CPU stopped during debug. The setting
         * is sync'd from RTC_EMU on each wakeup from standby.
         */
        HWREG(SYSTIM_BASE + SYSTIM_O_EMU) = SYSTIM_EMU_HALT_STOP;

        /* Start HFXT */
        PowerCC27XX_startHFXT();

        /* Enable tracking loop with HFXT as reference. This will automatically
         * calibrate LFOSC against HFXT whenever HFXT is enabled; usually after
         * waking up from standby.
         * This is needed to ensure fast HFXT startup and a reasonably accurate
         * LFOSC frequency.
         */
        HWREG(CKMD_BASE + CKMD_O_HFTRACKCTL) |= CKMD_HFTRACKCTL_EN_M | CKMD_HFTRACKCTL_REFCLK_HFXT;

        /* Enable RTC as a standby wakeup source */
        HWREG(EVTULL_BASE + EVTULL_O_WKUPMASK) = EVTULL_WKUPMASK_AON_RTC_COMB_M;

        /* Enable the CKMD interrupt */
        HwiP_enableInterrupt(INT_CPUIRQ3);

        status = Power_SOK;
    }

    return status;
}

/*
 *  ======== Power_getConstraintMask ========
 *  Get a bitmask indicating the constraints that have been registered with
 *  Power. This combines both secure and non-secure constraints.
 */
uint_fast32_t Power_getConstraintMask(void)
{
    uint_fast32_t result = (uint_fast32_t)constraintMask_s;

    /* OR non-secure constraint mask to result if it exists */
    if (constraintMask_ns != NULL)
    {
        result |= (uint_fast32_t)*constraintMask_ns;
    }

    return result;
}

/*
 *  ======== Power_getDependencyCount ========
 *  Get the count of dependencies that are currently declared upon a resource.
 *  This combines both secure and non-secure dependencies.
 */
int_fast16_t Power_getDependencyCount(Power_Resource resourceId)
{
    int_fast16_t secureResult;
    int_fast16_t nonSecureResult;
    int_fast16_t combinedResult;

    secureResult    = PowerCC27XX_s_getDependencyCount(resourceId, PowerCC27XX_SECURE);
    nonSecureResult = PowerCC27XX_s_getDependencyCount(resourceId, PowerCC27XX_NONSECURE);

    if (secureResult < 0)
    {
        /* A negative result from PowerCC27XX_s_getDependencyCount() is an error
         * code. This means that an error occurred when trying to get the secure
         * dependency count. The secure error takes priority, and the error
         * code is returned directly regardless of the non-secure result.
         */
        combinedResult = secureResult;
    }
    else if (nonSecureResult < 0)
    {
        /* A negative result from PowerCC27XX_s_getDependencyCount() is an error
         * code. This means that an error occurred when trying to get the
         * non-secure dependency count. We don't want non-secure errors to cause
         * an error code returned to secure code. So in this case we just ignore
         * the non-secure result and return the secure successful result.
         */
        combinedResult = secureResult;
    }
    else
    {
        /* No errors occurred, return sum of secure and non-secure results */
        combinedResult = secureResult + nonSecureResult;
    }

    return combinedResult;
}

/*
 *  ======== PowerCC27XX_s_getDependencyCount ========
 *  Get the count of dependencies that are currently declared, by a certain
 *  security state, upon a resource.
 */
int_fast16_t PowerCC27XX_s_getDependencyCount(Power_Resource resourceId, PowerCC27XX_SecurityState secState)
{
    uint8_t bitIndex;
    uint_fast16_t group;
    int_fast16_t result;

    DebugP_assert(PowerCC27XX_isValidResourceId(resourceId));

    bitIndex = PowerCC27XX_getResourceBitIndex(resourceId);
    group    = PowerCC27XX_getResourceGroup(resourceId);

    if (group == PowerCC27XX_PERIPH_GROUP_CLKCTL0)
    {
        result = (int_fast16_t)resourceCountsClkctl0[secState][bitIndex];
    }
    else if (group == PowerCC27XX_PERIPH_GROUP_CLKCTL1)
    {
        result = (int_fast16_t)resourceCountsClkctl1[secState][bitIndex];
    }
    else if (group == PowerCC27XX_PERIPH_GROUP_LRFD)
    {
        result = (int_fast16_t)resourceCountsLrfd[secState][bitIndex];
    }
    else
    {
        result = (int_fast16_t)Power_EINVALIDINPUT;
    }

    return result;
}

/*
 *  ======== Power_getConstraintCount ========
 *  Get the count of constraints that are currently set on a certain
 *  operational transition.
 *  This combines both secure and non-secure constraints.
 */
int_fast16_t Power_getConstraintCount(uint_fast16_t constraintId)
{
    int_fast16_t secureResult;
    int_fast16_t nonSecureResult;
    int_fast16_t combinedResult;

    secureResult    = PowerCC27XX_s_getConstraintCount(constraintId, PowerCC27XX_SECURE);
    nonSecureResult = PowerCC27XX_s_getConstraintCount(constraintId, PowerCC27XX_NONSECURE);

    if (secureResult < 0)
    {
        /* A negative result from PowerCC27XX_s_getConstraintCount() is an error
         * code. This means that an error occurred when trying to get the secure
         * constraint count. The secure error takes priority, and the error
         * code is returned directly regardless of the non-secure result.
         */
        combinedResult = secureResult;
    }
    else if (nonSecureResult < 0)
    {
        /* A negative result from PowerCC27XX_s_getConstraintCount() is an error
         * code. This means that an error occurred when trying to get the
         * non-secure constraint count. We don't want non-secure errors to cause
         * an error code returned to secure code. So in this case we just ignore
         * the non-secure result and return the secure successful result.
         */
        combinedResult = secureResult;
    }
    else
    {
        /* No errors occurred, return sum of secure and non-secure results */
        combinedResult = secureResult + nonSecureResult;
    }

    return combinedResult;
}

/*
 *  ======== PowerCC27XX_s_getConstraintCount ========
 *  Get the count of constraints that are currently set, by a certain security
 *  state, on a certain operational transition.
 */
static int_fast16_t PowerCC27XX_s_getConstraintCount(uint_fast16_t constraintId, PowerCC27XX_SecurityState secState)
{
    DebugP_assert(constraintId < PowerCC27XX_NUMCONSTRAINTS);

    int_fast16_t result = Power_EINVALIDINPUT;

    if (constraintId < PowerCC27XX_NUMCONSTRAINTS)
    {
        if (secState == PowerCC27XX_SECURE)
        {
            result = (int_fast16_t)constraintCounts_s[constraintId];
        }
        else /* PowerCC27XX_NONSECURE */
        {
            /* Only try to access non-secure constraint count, if it has been
             * initialized.
             */
            if (constraintCounts_ns != NULL)
            {
                result = (int_fast16_t)(*constraintCounts_ns)[constraintId];
            }
        }
    }

    return result;
}

/*
 *  ======== PowerCC27XX_s_setNonSecureConstraintPointers ========
 */
int_fast16_t PowerCC27XX_s_setNonSecureConstraintPointers(
    volatile const uint32_t *constraintMask,
    volatile const uint8_t (*const constraintCounts)[PowerCC27XX_NUMCONSTRAINTS])
{
    constraintMask_ns   = constraintMask;
    constraintCounts_ns = constraintCounts;
    return Power_SOK;
}

/*
 *  ======== Power_getTransitionLatency ========
 *  Get the transition latency for a sleep state.  The latency is reported
 *  in units of microseconds.
 */
uint_fast32_t Power_getTransitionLatency(uint_fast16_t sleepState, uint_fast16_t type)
{
    /* TODO: Make this a dynamic system based on the dynamically computed delta
     * between RTC timeout and re-enabling interrupts
     */

    uint_fast32_t latency = 0;

    if (type == Power_RESUME)
    {
        if (sleepState == PowerLPF3_STANDBY)
        {
            latency = PowerCC27XX_RESUMETIMESTANDBY;
        }
    }
    else
    {
        if (sleepState == PowerLPF3_STANDBY)
        {
            latency = PowerCC27XX_TOTALTIMESTANDBY;
        }
    }

    return latency;
}

/*
 *  ======== Power_getTransitionState ========
 *  Get the current sleep transition state.
 */
uint_fast16_t Power_getTransitionState(void)
{
    return (uint_fast16_t)powerState;
}

/*
 *  ======== Power_registerNotify ========
 *  Register a function to be called on a specific power event.
 */
int_fast16_t Power_registerNotify(Power_NotifyObj *pNotifyObj,
                                  uint_fast16_t eventTypes,
                                  Power_NotifyFxn notifyFxn,
                                  uintptr_t clientArg)
{
    int_fast16_t status = Power_SOK;

    /* Check for NULL pointers */
    if ((pNotifyObj == NULL) || (notifyFxn == NULL))
    {
        status = Power_EINVALIDPOINTER;
    }
    else
    {
        pNotifyObj->eventTypes = eventTypes;
        pNotifyObj->notifyFxn  = notifyFxn;
        pNotifyObj->clientArg  = clientArg;

        /* Place notify object on event notification queue. Assume that
         * List_Elem struct member is the first struct member in
         * Power_NotifyObj.
         */
        List_put(&notifyList_s, &pNotifyObj->link);
    }

    return status;
}

/*
 *  ======== Power_unregisterNotify ========
 *  Unregister for a power notification.
 *
 */
void Power_unregisterNotify(Power_NotifyObj *pNotifyObj)
{
    /* Remove notify object from its event queue */
    List_remove(&notifyList_s, &pNotifyObj->link);
}

/*
 *  ======== Power_setConstraint ========
 *  Declare an operational constraint for the Secure security state.
 *  This is only to be used by the secure image. It must NOT be made accesible
 *  to the non-secure app through a veneer.
 */
int_fast16_t Power_setConstraint(uint_fast16_t constraintId)
{
    uintptr_t key;
    int_fast16_t status = Power_SOK;

    if (constraintId >= PowerCC27XX_NUMCONSTRAINTS)
    {
        status = Power_EINVALIDINPUT;
    }

    if (status == Power_SOK)
    {
        key = HwiP_disable();

        /* Set the specified constraint in the constraintMask_s for faster access */
        constraintMask_s |= ((uint32_t)1U << constraintId);

        if (constraintCounts_s[constraintId] == (uint8_t)UINT8_MAX)
        {
            status = Power_EFAIL;
        }
        else
        {
            /* Increment the specified constraint count */
            constraintCounts_s[constraintId]++;
        }

        HwiP_restore(key);
    }

    return status;
}

/*
 *  ======== Power_releaseConstraint ========
 *  Release a previously declared constraint for the Secure security state.
 *  This is only to be used by the secure image. It must NOT be made accesible
 *  to the non-secure app through a veneer.
 */
int_fast16_t Power_releaseConstraint(uint_fast16_t constraintId)
{
    uintptr_t key;
    int_fast16_t status = Power_SOK;

    if (constraintId >= PowerCC27XX_NUMCONSTRAINTS)
    {
        status = Power_EINVALIDINPUT;
    }
    else
    {
        key = HwiP_disable();

        if (constraintCounts_s[constraintId] == 0U)
        {
            status = Power_EFAIL;
        }
        else
        {
            constraintCounts_s[constraintId]--;

            /* Only update the constraint mask if we removed the constraint entirely */
            if (constraintCounts_s[constraintId] == 0U)
            {
                constraintMask_s &= (~((uint32_t)1U << constraintId));
            }
        }

        HwiP_restore(key);
    }

    return status;
}

/*
 *  ======== Power_setDependency ========
 *  Declare a dependency upon a resource for the Secure security state.
 *  This is only to be used by the secure image. It must not be made accesible
 *  to the non-secure app through a veneer.
 */
int_fast16_t Power_setDependency(Power_Resource resourceId)
{
    return PowerCC27XX_s_setDependency(resourceId, PowerCC27XX_SECURE);
}

/*
 *  ======== PowerCC27XX_s_setDependency ========
 *  Declare a dependency upon a resource for a certain security state.
 *  If called from the non-secure app, through a veneer, then secState must be
 *  PowerCC27XX_NONSECURE.
 */
int_fast16_t PowerCC27XX_s_setDependency(Power_Resource resourceId, PowerCC27XX_SecurityState secState)
{
    int_fast16_t previousCount;
    int_fast16_t previousTotalCount;
    uintptr_t key;
    uint8_t bitIndex;
    uint32_t bitMask;
    int_fast16_t status;

    DebugP_assert(PowerCC27XX_isValidResourceId(resourceId));

    key = HwiP_disable();

    /* Buffer previous reference count for the specified security state */
    previousCount = PowerCC27XX_s_getDependencyCount(resourceId, secState);

    /* Buffer to total reference count across all security states */
    previousTotalCount = Power_getDependencyCount(resourceId);

    if (previousCount < 0)
    {
        /* A negative result from PowerCC27XX_s_getDependencyCount() is an error
         * code. Set the status to the error code returned by
         * PowerCC27XX_s_getDependencyCount().
         */
        status = previousCount;
    }
    else if (previousTotalCount < 0)
    {
        /* A negative result from Power_getDependencyCount() is an error code.
         * Set the status to the error code returned by
         * Power_getDependencyCount().
         */
        status = previousTotalCount;
    }
    else if (previousCount >= UINT8_MAX)
    {
        /* If the resource count is already at its max value, then incrementing
         * it would result in overflow, so return error and do not increment
         * the resource count.
         */
        status = Power_EFAIL;
    }
    else
    {
        status = Power_SOK;
    }

    if (status == Power_SOK)
    {
        /* Increment reference count */
        PowerCC27XX_s_setDependencyCount(resourceId, (uint8_t)previousCount + 1U, secState);

        /* If the resource was NOT activated previously ... */
        if (previousTotalCount == 0)
        {
            bitIndex = PowerCC27XX_getResourceBitIndex(resourceId);
            bitMask  = ((uint32_t)1U << bitIndex);

            /* Turn on the peripheral */
            switch (PowerCC27XX_getResourceGroup(resourceId))
            {
                case PowerCC27XX_PERIPH_GROUP_CLKCTL0:
                    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET0) = bitMask;
                    break;
                case PowerCC27XX_PERIPH_GROUP_CLKCTL1:
                    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET1) = bitMask;
                    break;
                case PowerCC27XX_PERIPH_GROUP_LRFD:
                    LRFDSetClockDependency((uint16_t)bitMask, LRFD_CLK_DEP_POWER);
                    break;
                default:
                    break;
            }
        }
    }

    HwiP_restore(key);

    return Power_SOK;
}

/*
 *  ======== Power_releaseDependency ========
 *  Release a previously declared dependency for the Secure security state.
 *  This is only to be used by the secure image. It must not be made accesible
 *  to the non-secure app through a veneer.
 */
int_fast16_t Power_releaseDependency(Power_Resource resourceId)
{
    return PowerCC27XX_s_releaseDependency(resourceId, PowerCC27XX_SECURE);
}

/*
 *  ======== PowerCC27XX_s_releaseDependency ========
 *  Release a previously declared dependency for a certain security state.
 *  If called from the non-secure app, through a veneer, then secState must be
 *  PowerCC27XX_NONSECURE.
 */
int_fast16_t PowerCC27XX_s_releaseDependency(Power_Resource resourceId, PowerCC27XX_SecurityState secState)
{
    int_fast16_t resourceCount;
    int_fast16_t totalResourceCount;
    uintptr_t key;
    uint8_t bitIndex;
    uint32_t bitMask;
    int_fast16_t status;

    DebugP_assert(PowerCC27XX_isValidResourceId(resourceId));

    key = HwiP_disable();

    /* Buffer previous reference count for the specified security state */
    resourceCount = PowerCC27XX_s_getDependencyCount(resourceId, secState);

    DebugP_assert(resourceCount != 0);

    if (resourceCount < 0)
    {
        /* A negative result from PowerCC27XX_s_getDependencyCount() is an error
         * code. Set the status to the error code returned by
         * PowerCC27XX_s_getDependencyCount().
         */
        status = resourceCount;
    }
    else if (resourceCount == 0)
    {
        /* If the resource count is 0, then decrementing the resource count
         * would result in underflow, so return error and do not decrement
         * the resource count.
         */
        status = Power_EFAIL;
    }
    else
    {
        status = Power_SOK;
    }

    if (status == Power_SOK)
    {
        /* Decrement the reference count */
        resourceCount--;
        PowerCC27XX_s_setDependencyCount(resourceId, (uint8_t)resourceCount, secState);

        /* Get the total resource count across all security states. */
        totalResourceCount = Power_getDependencyCount(resourceId);

        /* If this was the last dependency being released.. */
        if (totalResourceCount == 0)
        {
            bitIndex = PowerCC27XX_getResourceBitIndex(resourceId);
            bitMask  = ((uint32_t)1U << bitIndex);

            /* Turn off the peripheral */
            switch (PowerCC27XX_getResourceGroup(resourceId))
            {
                case PowerCC27XX_PERIPH_GROUP_CLKCTL0:
                    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR0) = bitMask;
                    break;
                case PowerCC27XX_PERIPH_GROUP_CLKCTL1:
                    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR1) = bitMask;
                    break;
                case PowerCC27XX_PERIPH_GROUP_LRFD:
                    LRFDReleaseClockDependency((uint16_t)bitMask, LRFD_CLK_DEP_POWER);
                    break;
                default:
                    break;
            }
        }
    }

    HwiP_restore(key);

    return status;
}

/*
 *  ======== Power_shutdown ========
 */
int_fast16_t Power_shutdown(uint_fast16_t shutdownState, uint_fast32_t shutdownTime)
{
    int_fast16_t notifyStatus;
    uintptr_t hwiKey;
    uint_fast16_t i;
    int_fast16_t status = Power_SOK;
    bool ioPending      = false;

    (void)shutdownState;
    (void)shutdownTime;

    hwiKey = HwiP_disable();

    /* Check if there is a constraint to prohibit shutdown */
    if ((uint_fast32_t)(Power_getConstraintMask() & ((uint_fast32_t)1U << PowerLPF3_DISALLOW_SHUTDOWN)) != 0U)
    {
        status = Power_ECHANGE_NOT_ALLOWED;
    }

    if (status == Power_SOK)
    {
        /* Check whether we were transitioning to another power state */
        if (powerState != Power_ACTIVE)
        {
            status = Power_EBUSY;
        }
    }

    if (status == Power_SOK)
    {
        /* Set new transition state to entering shutdown */
        powerState = Power_ENTERING_SHUTDOWN;

        /* Signal all clients registered for pre-shutdown notification */
        notifyStatus = PowerCC27XX_notify(PowerLPF3_ENTERING_SHUTDOWN);

        for (i = GPIO_pinLowerBound; i <= GPIO_pinUpperBound; i++)
        {
            /* Read WUCFGSD field once and check both values */
            uint32_t ioShutdownConfig = HWREG(PowerCC27XX_getIocAddr(i)) & IOC_IOC0_WUCFGSD_M;

            if (((ioShutdownConfig == IOC_IOC0_WUCFGSD_WAKE_HIGH) || (ioShutdownConfig == IOC_IOC0_WUCFGSD_WAKE_LOW)) &&
                (GPIOGetEventDio((uint32_t)i) != 0U))
            {
                ioPending = true;
            }
        }

        /* If no IO event is pending on a shutdown wakeup IO, go to shutdown */
        if ((ioPending == false) && (notifyStatus == Power_SOK))
        {
            HWREG(PMCTL_BASE + PMCTL_O_SHTDWN) = PMCTL_SHTDWN_KEY_VALID;
        }

        /* If shutdown succeeded, should never get here */
        status = Power_EFAIL;

        powerState = Power_ACTIVE;
    }

    HwiP_restore(hwiKey);

    /* If we get here, failed to shutdown, return error code */
    return status;
}

/*
 *  ======== Power_sleep ========
 */
int_fast16_t Power_sleep(uint_fast16_t sleepState)
{
    int_fast16_t status = Power_SOK;
    uint32_t SysTick_NS_CSR;
    uint32_t SysTick_NS_RVR;
    uint32_t ICB_NS_ACTLR;

    (void)sleepState;

    /* Signal all clients registered for pre standby wakeup notification */
    status = PowerCC27XX_notify(PowerLPF3_ENTERING_STANDBY);

    /* Check for any error */
    if (status != Power_SOK)
    {
        powerState = Power_ACTIVE;
    }
    else
    {
        /* Clear the events mask. */
        notifyEventsMask = 0;

        /* Due to a bug in the ROM code on some devices, the NS SysTick and NS
         * ICB are not restored correctly so we need to store and restore the
         * state here instead.
         */

        /* Store ICB_NS.ACTLR. Add 0x20000 to access NS ICB */
        ICB_NS_ACTLR = HWREG(ICB_BASE + 0x20000U + ICB_O_ACTLR);

        /* Store SysTick_NS.CSR/RVR */
        SysTick_NS_CSR = SysTick_NS->CTRL;
        SysTick_NS_RVR = SysTick_NS->LOAD;

        /* Call wrapper function to ensure that R0-R3 are saved and restored before
         * and after this function call. Otherwise, compilers will attempt to stash
         * values on the stack while on the PSP and then restore them just after
         * HapiEnterStandby() on the MSP. Which will cause wildly unexpected
         * behaviour.
         */
        PowerCC27XX_enterStandby();

        /* Restore ICB_NS.ACTLR. Add 0x20000 to access NS ICB */
        HWREG(ICB_BASE + 0x20000U + ICB_O_ACTLR) = ICB_NS_ACTLR;

        /* Restore SysTick_NS.CSR/RVR */
        SysTick_NS->CTRL = SysTick_NS_CSR;
        SysTick_NS->LOAD = SysTick_NS_RVR;

        /* Now that we have returned and are executing code from flash again, start
         * up the HFXT. The HFXT might already have been enabled automatically by
         * hardware, but calling below ensures that it has been enabled and that the
         * necessary power constraints and interrupts are set and enabled.
         */
        PowerCC27XX_startHFXT();

        /* Now clear the transition state before re-enabling the scheduler */
        powerState = Power_ACTIVE;
    }

    return status;
}

/*
 *  ======== Power_reset ========
 */
void Power_reset(void)
{
    PMCTLResetSystem();
}

/*
 *  ======== PowerLPF3_getResetReason ========
 */
PowerLPF3_ResetReason PowerLPF3_getResetReason(void)
{
    PowerLPF3_ResetReason resetReason;
    uint32_t pmctlResetReason = PMCTLGetResetReason();
    switch (pmctlResetReason)
    {
        case (uint32_t)PowerLPF3_RESET_SHUTDOWN_IO:
            resetReason = PowerLPF3_RESET_SHUTDOWN_IO;
            break;
        case (uint32_t)PowerLPF3_RESET_SHUTDOWN_SWD:
            resetReason = PowerLPF3_RESET_SHUTDOWN_SWD;
            break;
        case (uint32_t)PowerLPF3_RESET_WATCHDOG:
            resetReason = PowerLPF3_RESET_WATCHDOG;
            break;
        case (uint32_t)PowerLPF3_RESET_SYSTEM:
            resetReason = PowerLPF3_RESET_SYSTEM;
            break;
        case (uint32_t)PowerLPF3_RESET_CPU:
            resetReason = PowerLPF3_RESET_CPU;
            break;
        case (uint32_t)PowerLPF3_RESET_LOCKUP:
            resetReason = PowerLPF3_RESET_LOCKUP;
            break;
        case (uint32_t)PowerLPF3_RESET_TSD:
            resetReason = PowerLPF3_RESET_TSD;
            break;
        case (uint32_t)PowerLPF3_RESET_SWD:
            resetReason = PowerLPF3_RESET_SWD;
            break;
        case (uint32_t)PowerLPF3_RESET_LFXT:
            resetReason = PowerLPF3_RESET_LFXT;
            break;
        case (uint32_t)PowerLPF3_RESET_VDDR:
            resetReason = PowerLPF3_RESET_VDDR;
            break;
        case (uint32_t)PowerLPF3_RESET_VDDS:
            resetReason = PowerLPF3_RESET_VDDS;
            break;
        case (uint32_t)PowerLPF3_RESET_PIN:
            resetReason = PowerLPF3_RESET_PIN;
            break;
        case (uint32_t)PowerLPF3_RESET_POR:
            resetReason = PowerLPF3_RESET_POR;
            break;
        default:
            resetReason = PowerLPF3_RESET_UNKNOWN;
            break;
    }
    return resetReason;
}

/*
 *  ======== PowerLPF3_releaseLatches ========
 */
void PowerLPF3_releaseLatches(void)
{
    HWREG(PMCTL_BASE + PMCTL_O_SLPCTL) = PMCTL_SLPCTL_SLPN_DIS;
}

/*
 *  ======== PowerCC27XX_enterStandby ========
 *  Wrapper function to ensure that R0-R3 are saved and restored before and
 *  after this function call. Otherwise, compilers will attempt to stash
 *  values on the stack while on the PSP and then restore them just after
 *  HapiEnterStandby() on the MSP. Which will cause wildly unexpected behaviour.
 */
static void PowerCC27XX_enterStandby(void)
{
    /* Read current stack pointer from CONTROL */
    uint32_t controlPreStandby = __get_CONTROL();

    if ((controlPreStandby & 0x02U) == 0x02U)
    {
        /* Currently on PSP stack, switching to MSP.
         * HapiEnterStandby() must execute from MSP since the
         * device reboots into privileged mode on MSP and HapiEnterStandby()
         * assumes it will be called running on MSP.
         */
        __set_CONTROL(0x00U);

        /* - Save CPU state on MSP and MSP in CLKCTL_STBYPTR
         * - Enter standby
         * - Exit standby
         * - Restore CPU state from MSP
         * - Apply copyList
         */
        HapiEnterStandby(NULL);

        /* Switch back to PSP stack, where controlPreStandby is stored */
        __set_CONTROL(0x02U);

        /* Restore control register */
        __set_CONTROL(controlPreStandby);
    }
    else
    {
        /* Stack is already MSP.
         * - Save CPU state on MSP and MSP in CLKCTL_STBYPTR
         * - Enter standby
         * - Exit standby
         * - Restore CPU state from MSP
         * - Apply copyList
         */
        HapiEnterStandby(NULL);
    }
}

/*
 *  ======== PowerLPF3_selectLFXT ========
 */
void PowerLPF3_selectLFXT(void)
{
    uintptr_t key;
    int_fast16_t status;

    /* Set LFCLK  */
    HWREG(CKMD_BASE + CKMD_O_LFCLKSEL) = CKMD_LFCLKSEL_MAIN_LFXT;

    /* Start LFXT */
    HWREG(CKMD_BASE + CKMD_O_LFXTCTL) = CKMD_LFXTCTL_EN;

    /* Set LFCLK qualification function to be called by
     * PowerCC27XX_oscillatorISR(). This function will process and evaluate the
     * result of the TDC measurement which is started below.
     */
    PowerLPF3_lfclkQualFxn = PowerCC27XX_lfxtQual;

    /* Disallow standby until LF clock is running.
     * The PowerCC27XX_oscillatorISR() function will release the constraint once
     * PowerLPF3_lfclkQualFxn returns true.
     */
    status = Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    if (status != Power_SOK)
    {
        tfm_core_panic();
    }

    key = HwiP_disable();

    /* If the AMPSETTLED mask is no longer set, it means that the AMPSETTLED
     * interrupt has already occured and been handled, so the HFXT is ready
     * to be used by the TDC. Otherwise, HFXT is not ready and the TDC will
     * be started by PowerCC27XX_lfxtQual() instead.
     */
    if ((HWREG(CKMD_BASE + CKMD_O_IMASK) & CKMD_IMASK_AMPSETTLED_M) != CKMD_IMASK_AMPSETTLED_M)
    {
        /* Start TDC measurement of LFXT. Result will be processed by
         * PowerCC27XX_lfoscQual()
         */
        PowerCC27XX_startLfclkTdcMeasurement(PowerCC27XX_LFXT);
    }

    HwiP_restore(key);
}

/*
 *  ======== PowerCC27XX_lfxtQual ========
 *  - When getting the AMPSETTLED interrupt:
 *    - Start a TDC measurement of LFOSC
 *  - When getting the TDCDONE interrupt:
 *    - Determine if LFXT is good based on the TDC result.
 *      - If LFXT is not good, the TDC will be restarted, and TDCDONE interrupt
 *        will be re-enabled.
 *      - If LFXT is good, the LFTICK interrupt will be enabled.
 *  - When getting the LFTICK interrupt:
 *     - Select LFXT as the LFCLK source.
 *     - The LFTICK interrupt will remain enabled.
 *  - When getting the second LFTICK interrupt:
 *     - Configure the LFINC override value to match the LFXT frequency of
 *       32.768 kHz.
 *     - Return true to indicate that the qualification process is done.
 */
static bool PowerCC27XX_lfxtQual(uint32_t maskedStatus)
{
    static int32_t lastTdcCount = 0;
    bool result                 = false;

    if ((maskedStatus & CKMD_MIS_AMPSETTLED_M) != 0U)
    {
        PowerCC27XX_startLfclkTdcMeasurement(PowerCC27XX_LFXT);
    }
    else if ((maskedStatus & CKMD_MIS_TDCDONE_M) != 0U)
    {
        bool isClockGood = false;

        /* Read result from TDC */
        uint32_t edges = HWREG(CKMD_BASE + CKMD_O_RESULT);

        /* Only use TDC result if it is not too big to fit in a signed 32 bit
         * integer, to support casting to int32_t below. If it does not fit,
         * then the clock is not considered good.
         */
        if (edges <= (uint32_t)INT32_MAX)
        {
            /* Compute delta compared to the previous result */
            int32_t edgeDeltaLast = (int32_t)edges - lastTdcCount;

            /* Compute the offset of the previous measurement */
            int32_t lastOffset = lastTdcCount - 1500000;

            /* Store the result of the TDC */
            lastTdcCount = (int32_t)edges;

            /* Calculate offset from the expected result.
             * The expected number of edges is (96MHz/32.768kHz)*512 = 1500000
             */
            int32_t edgeOffset = (int32_t)edges - 1500000;

            /* For LFXT the clock is considered good if the frequency is
             * within 32.768 kHz +/-100 ppm, and the frequency is within
             * +/-100 ppm of the last measurement.
             *
             * The expected number of edges is (96 MHz/32.768 kHz)*512 =
             * 1500000.
             * 100 ppm of 1500000 edges is 150 edges.
             */
            isClockGood = (Math_ABS(lastOffset) < 150) && (Math_ABS(edgeOffset) < 150) &&
                          (Math_ABS(edgeDeltaLast) < 150);
        }

        if (isClockGood)
        {
            /* Clock is stable */

            /* Clear any pending LFTICK interrupt */
            HWREG(CKMD_BASE + CKMD_O_ICLR) = CKMD_ICLR_LFTICK;

            /* Enable LFTICK interrupt */
            HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMSET_LFTICK;

            /* Reset lastTdcCount */
            lastTdcCount = 0;
        }
        else
        {
            /* Clock is not stable. Start a new TDC measurement */
            PowerCC27XX_restartLfclkTdcMeasurement();
        }
    }
    else if ((maskedStatus & CKMD_MIS_LFTICK_M) != 0U)
    {
        if (HWREG(CKMD_BASE + CKMD_O_LFCLKSEL) != (CKMD_LFCLKSEL_PRE_LFXT | CKMD_LFCLKSEL_MAIN_LFXT))
        {
            /* Select LFXT as LFCLK source */
            HWREG(CKMD_BASE + CKMD_O_LFCLKSEL) = CKMD_LFCLKSEL_PRE_LFXT | CKMD_LFCLKSEL_MAIN_LFXT;

            /* Wait for next LFTICK to change LFINC override
             * Re-enable LFTICK interrupt. The next interrupt will
             * trigger the else statement below.
             */
            HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMSET_LFTICK;
        }
        else
        {
            /* Set LFINC override to 32.768 kHz.
             *
             * The value is calculated as period in microseconds with 16
             * fractional bits.
             * The LFXT runs at 32.768 kHz -> 1 / 32768 Hz = 30.5176 us.
             * 30.5176 * 2^16 = 2000000 = 0x001E8480
             */
            HWREG(CKMD_BASE + CKMD_O_LFINCOVR) = 0x001E8480U | CKMD_LFINCOVR_OVERRIDE;

            /* Qualification is done */
            result = true;
        }
    }
    else
    {
        /* Do nothing if none of the interrupts of interest to LFXT
         * qualification are set
         */
    }

    return result;
}

/*
 *  ======== PowerCC27XX_startLfclkTdcMeasurement ========
 * This function will start a TDC measurment of the specified LFCLK source.
 * The counter will increment on each edfe of CLKSVT (48MHz), meaning the
 * counter will increment with a frequency of 96MHz. The TDC will be configured
 * to measure 512 periods of the LFCLK source. Meaning the expected result for a
 * 32.786kHz LFCLK will be (96MHz/32.786kHz)*512 = 1500000
 *
 * This function will also enable the TDCDONE interrupt. It is the caller's
 * responsibility to handle this interrupt and process the result.
 */
static void PowerCC27XX_startLfclkTdcMeasurement(PowerCC27XX_LfclkTdcTrigSrc tdcTrigSrc)
{
    /* Configure TDC to measure the LFCLK source to be */
    HWREG(CKMD_BASE + CKMD_O_TRIGSRC) = (uint32_t)tdcTrigSrc;

    /* Enable stop-counter */
    HWREG(CKMD_BASE + CKMD_O_TRIGCNTCFG) = CKMD_TRIGCNTCFG_EN;

    /* Select CLKSVT (48MHz) as clock source for TDC. The TDC is counting edges.
     * There are two edges per clock cycle, so the edge frequency will be
     * 2*48MHz = 96MHz.
     */
    HWREG(CKMD_BASE + CKMD_O_TDCCLKSEL) = CKMD_TDCCLKSEL_REFCLK_CLKSVT;

    /* Configure TDC to count for 512 LFCLK periods */
    HWREG(CKMD_BASE + CKMD_O_TRIGCNTLOAD) = 512U;

    /* Clear status from previous TDC measurement */
    HWREG(CKMD_BASE + CKMD_O_CTL) = CKMD_CTL_CMD_CLR_RESULT;

    /* Clear any pending TDCDONE interrupt */
    HWREG(CKMD_BASE + CKMD_O_ICLR) = CKMD_ICLR_TDCDONE;

    /* Start TDC measurement */
    HWREG(CKMD_BASE + CKMD_O_CTL) = CKMD_CTL_CMD_RUN_SYNC_START;

    /* Enable TDCDONE interrupt */
    HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMASK_TDCDONE;
}

/*
 *  ======== PowerCC27XX_restartLfclkTdcMeasurement ========
 */
static void PowerCC27XX_restartLfclkTdcMeasurement(void)
{
    /* Clear status from previous TDC measurement */
    HWREG(CKMD_BASE + CKMD_O_CTL) = CKMD_CTL_CMD_CLR_RESULT;

    /* Start new TDC measurement */
    HWREG(CKMD_BASE + CKMD_O_CTL) = CKMD_CTL_CMD_RUN_SYNC_START;

    /* Re-enable the TDCDONE interrupt */
    HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMCLR_TDCDONE;
}

/*
 *  ======== PowerCC27XX_oscillatorISR ========
 */
static void PowerCC27XX_oscillatorISR(uintptr_t arg)
{
    uint32_t maskedStatus = HWREG(CKMD_BASE + CKMD_O_MIS);
    int_fast16_t status;

    (void)arg;

    /* Manipulating ICLR alone does not actually do anything. The CKM_COMB
     * signals are almost all level values that reset one cycle after writing to
     * ICLR. We need to update the mask instead to avoid looping in the ISR
     */
    HWREG(CKMD_BASE + CKMD_O_ICLR)  = maskedStatus;
    HWREG(CKMD_BASE + CKMD_O_IMCLR) = maskedStatus;

    if ((maskedStatus & CKMD_MIS_AMPSETTLED_M) != 0U)
    {
        /* It has been observed a brief period of ~15us occurring ~130us after
         * starting HFXT where FLTSETTLED pulses high. If the idle loop attempts
         * to enter standby while FLTSETTLED pulses high, it may enter standby
         * before the filter is truly settled. To prevent this, we need to wait
         * for HFXTGOOD and LFTICK to be set before entering standby. This is
         * achieved by clearing LFTICK once AMPSETTLED is set (which occurs
         * after HFXTGOOD is set), and then waiting for LFTICK to be set again
         * before entering standby.
         */
        HWREG(CKMD_BASE + CKMD_O_ICLR) = CKMD_ICLR_LFTICK;

        /* Send out notification for HFXT available. Do not care if it fails. */
        (void)PowerCC27XX_notify(PowerLPF3_HFXT_AVAILABLE);

        /* Allow standby again now that HFXT has finished starting */
        status = Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

        if (status != Power_SOK)
        {
            tfm_core_panic();
        }
    }

    if ((maskedStatus & (CKMD_MIS_HFXTFAULT_M | CKMD_MIS_TRACKREFLOSS_M)) != 0U)
    {
        /* If there was a HFXTFAULT or TRACKREFLOSS, restart HFXT. Consider
         * issuing a notification to allow logging. If we keep it like this, we
         * could get stuck in an infinite loop restarting a faulty oscillator.
         * Then again, it is not like there is a great way to recover from that.
         */
        HWREG(CKMD_BASE + CKMD_O_HFXTCTL) &= ~(uint32_t)CKMD_HFXTCTL_EN_M;

        /* Start up the HFXT using the workaround for the HFXT amplitude control ADC
         * bias point
         */
        PowerCC27XX_startHFXT();

        /* Re-enable interrupts */
        HWREG(CKMD_BASE + CKMD_O_IMSET) = maskedStatus & (CKMD_MIS_HFXTFAULT_M | CKMD_MIS_TRACKREFLOSS_M);
    }

    if ((maskedStatus & CKMD_MIS_TRACKREFLOSS_M) != 0U)
    {
        /* Disable interrupts as HFXT SWTCXO may interrupt and modify
         * HFTRACKCTL with a higher priority depending on user interrupt
         * priorities.
         */
        uintptr_t key = HwiP_disable();

        /* Disable tracking */
        HWREG(CKMD_BASE + CKMD_O_HFTRACKCTL) &= ~CKMD_HFTRACKCTL_EN_M;

        /* Re-enable tracking */
        HWREG(CKMD_BASE + CKMD_O_HFTRACKCTL) |= CKMD_HFTRACKCTL_EN_M;

        HwiP_restore(key);

        /* Re-enable TRACKREFLOSS */
        HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMSET_TRACKREFLOSS_M;
    }

    if (PowerLPF3_lfclkQualFxn != NULL)
    {
        if (PowerLPF3_lfclkQualFxn(maskedStatus))
        {
            /* LFCLK Qualification is done. Set qualification function to NULL,
             * so it will no longer be called. It is no longer needed, and
             * any additional calls could result in releasing the
             * PowerLPF3_DISALLOW_STANDBY constraint below multiple times.
             */
            PowerLPF3_lfclkQualFxn = NULL;

            /* Enable LF clock monitoring */
            HWREG(CKMD_BASE + CKMD_O_LFMONCTL) = CKMD_LFMONCTL_EN;

            /* Enable LF clock loss reset while in standby */
            HWREG(PMCTL_BASE + PMCTL_O_RSTCTL) |= PMCTL_RSTCTL_LFLOSS_ARMED;

            /* Send out notification for LF clock switch. Do not care if it
             * fails.
             */
            (void)PowerCC27XX_notify(PowerLPF3_LFCLK_SWITCHED);

            /* Allow standby again now that we have sent out the notification */
            status = Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

            if (status != Power_SOK)
            {
                tfm_core_panic();
            }
        }
    }

    if ((maskedStatus & CKMD_MIS_TRACKREFOOR_M) != 0U)
    {
        tfm_core_panic();
    }
}

/*
 *  ======== PowerCC27XX_startHFXT ========
 * It should be ensured that the system does not vector to the CKMD interrupt
 * (PowerCC27XX_oscillatorISR) while this function is executing.
 * Meaning any of the following must be true:
 *  - Interrupts are disabled, or
 *  - This function is called from PowerCC27XX_oscillatorISR(), since nested
 *    vectoring to the same interrupt is not possible.
 */
static void PowerCC27XX_startHFXT(void)
{
    int_fast16_t status;

    /* Start HFXT and enable automatic enabling after waking up from standby */
    HWREG(CKMD_BASE + CKMD_O_HFXTCTL) |= CKMD_HFXTCTL_EN | CKMD_HFXTCTL_AUTOEN;

    /* Disallow standby until AMPSETTLED is true */
    status = Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    if (status != Power_SOK)
    {
        tfm_core_panic();
    }

    /* Enable the AMPSETTLED interrupt.
     * Since it is a level status signal, it remains asserted when we are
     * running on HFXT and cannot be cleared.
     * The oscillator interrupt removes it from the interrupt mask to prevent
     * repeated vectoring.
     */
    HWREG(CKMD_BASE + CKMD_O_ICLR)  = CKMD_ICLR_AMPSETTLED;
    HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMSET_AMPSETTLED;
}

/*
 *  ======== PowerLPF3_startAFOSC ========
 */
int_fast16_t PowerLPF3_startAFOSC(PowerLPF3_AfoscFreq frequency)
{
    int_fast16_t status = Power_SOK;
    uint16_t mode;
    uint16_t mid;
    uint16_t coarse;
    uint32_t trackingRatio;

    /* Set status to failed if AFOSC is already enabled */
    if ((HWREG(CKMD_BASE + CKMD_O_AFOSCCTL) & CKMD_AFOSCCTL_EN_M) == CKMD_AFOSCCTL_EN)
    {
        status = Power_EFAIL;
    }

    if (status == Power_SOK)
    {
        /* Read AFOSC trims and select tracking loop ratio */
        switch (frequency)
        {
            case PowerLPF3_AFOSC_FREQ_80MHZ:
                mode          = fcfg->appTrims.cc27xx.afosc80MHZ.mode;
                mid           = fcfg->appTrims.cc27xx.afosc80MHZ.mid;
                coarse        = fcfg->appTrims.cc27xx.afosc80MHZ.coarse;
                trackingRatio = CKMD_AFTRACKCTL_RATIO__80MHZ;
                break;
            case PowerLPF3_AFOSC_FREQ_90P3168MHZ:
                mode          = fcfg->appTrims.cc27xx.afosc90MHZ.mode;
                mid           = fcfg->appTrims.cc27xx.afosc90MHZ.mid;
                coarse        = fcfg->appTrims.cc27xx.afosc90MHZ.coarse;
                trackingRatio = CKMD_AFTRACKCTL_RATIO__90P3168MHZ;
                break;
            case PowerLPF3_AFOSC_FREQ_98P304MHZ:
                mode          = fcfg->appTrims.cc27xx.afosc98MHZ.mode;
                mid           = fcfg->appTrims.cc27xx.afosc98MHZ.mid;
                coarse        = fcfg->appTrims.cc27xx.afosc98MHZ.coarse;
                trackingRatio = CKMD_AFTRACKCTL_RATIO__98P304MHZ;
                break;
            default:
                /* Return error */
                status = Power_EINVALIDINPUT;
                break;
        }
    }

    if (status == Power_SOK)
    {
        /* Write AFOSC to temporary variable representing the upper half word
         * (16bits) of CKMD.TRIM0. The CKMD_TRIM0_*_S defines define the bit shift
         * values for fields in the full word (32 bits) CKMD.TRIM0 register. 16 is
         * subtracted from the full word bit shift values to convert them to bit
         * shift values for the upper half word.
         */
        uint16_t upperHalfwordTrim0Tmp = 0U;
        upperHalfwordTrim0Tmp |= (uint16_t)(mode << (CKMD_TRIM0_AFOSC_MODE_S - 16U));
        upperHalfwordTrim0Tmp |= (uint16_t)(mid << (CKMD_TRIM0_AFOSC_MID_S - 16U));
        upperHalfwordTrim0Tmp |= (uint16_t)(coarse << (CKMD_TRIM0_AFOSC_COARSE_S - 16U));

        /* Write the AFOSC trims to CKMD with half word access, to prevent writing
         * to the HFOSC trims.
         */
        HWREGH(CKMD_BASE + CKMD_O_TRIM0 + 2U) = upperHalfwordTrim0Tmp;

        /* Set tracking ratio.
         * Note, trackingRatio is already bit aligned with the RATIO field.
         */
        uint32_t afTrackCtl = HWREG(CKMD_BASE + CKMD_O_AFTRACKCTL);
        afTrackCtl &= ~CKMD_AFTRACKCTL_RATIO_M;
        afTrackCtl |= trackingRatio;
        HWREG(CKMD_BASE + CKMD_O_AFTRACKCTL) = afTrackCtl;

        /* Enable AFOSC and enable auto-disable on standby entry */
        HWREG(CKMD_BASE + CKMD_O_AFOSCCTL) |= CKMD_AFOSCCTL_EN | CKMD_AFOSCCTL_AUTODIS;

        /* Wait until AFOSC good indication */
        while ((HWREG(CKMD_BASE + CKMD_O_RIS) & CKMD_RIS_AFOSCGOOD_M) != CKMD_RIS_AFOSCGOOD) {}

        /* Enable the AFOSC tracking loop */
        HWREG(CKMD_BASE + CKMD_O_AFTRACKCTL) |= CKMD_AFTRACKCTL_EN;
    }

    return status;
}

/*
 *  ======== PowerLPF3_stopAFOSC ========
 */
void PowerLPF3_stopAFOSC(void)
{
    /* Disable AFOSC tracking loop */
    HWREG(CKMD_BASE + CKMD_O_AFTRACKCTL) &= (~(uint32_t)CKMD_AFTRACKCTL_EN_M);

    /* Disable AFOSC */
    HWREG(CKMD_BASE + CKMD_O_AFOSCCTL) &= (~(uint32_t)CKMD_AFOSCCTL_EN_M);
}

/*
 *  ======== PowerCC27XX_notify ========
 *  Send notifications to registered clients.
 *  Note: Task scheduling is disabled when this function is called.
 */
static int_fast16_t PowerCC27XX_notify(uint_fast16_t eventType)
{
    int_fast16_t status;

    /* Handle secure notifications. */
    status = PowerCC27XX_notify_s(eventType);

    if (status == Power_SOK)
    {
        /* Handle non-secure notifications, ignoring the return value. */
        (void)PowerCC27XX_notify_ns(eventType);
    }

    return status;
}

/*
 *  ======== PowerCC27XX_notify_s ========
 *  Send notifications to registered clients on the secure side.
 */
static int_fast16_t PowerCC27XX_notify_s(uint_fast16_t eventType)
{
    int_fast16_t notifyStatus;
    Power_NotifyFxn notifyFxn;
    uintptr_t clientArg;
    List_Elem *elem;
    int_fast16_t status = Power_SOK;

    /* If queue is empty, return immediately. */
    if (!List_empty(&notifyList_s))
    {
        /* Point to first client notify object. */
        elem = List_head(&notifyList_s);

        /* Walk the queue and notify each registered client of the event. */
        do
        {
            if ((((Power_NotifyObj *)elem)->eventTypes & eventType) != 0U)
            {
                /* Pull params from notify object. */
                notifyFxn = ((Power_NotifyObj *)elem)->notifyFxn;
                clientArg = ((Power_NotifyObj *)elem)->clientArg;

                /* Call the client's notification function. */
                notifyStatus = (int_fast16_t)(*(Power_NotifyFxn)notifyFxn)(eventType, 0, clientArg);

                /* If client declared error stop all further notifications. */
                if (notifyStatus != Power_NOTIFYDONE)
                {
                    status = Power_EFAIL;
                }
            }

            /* Get next element in the notification queue. */
            elem = List_next(elem);

        } while ((elem != NULL) && (status == Power_SOK));
    }

    return status;
}

/*
 *  ======== PowerCC27XX_notify_ns ========
 *  Send notifications to registered clients on the non-secure side.
 *  Depending on the event type, the notifications are posted asynchronously or
 *  synchronously.
 */
static int_fast16_t PowerCC27XX_notify_ns(uint_fast16_t eventType)
{
    bool postSynchronously;

    /* Decide how to handle the event. */
    postSynchronously = ((eventType & SYNCHRONOUS_NS_EVENTS) != 0U);

    if (postSynchronously == false)
    {
        /* Update events mask for asynchronous consumption by NS side. */
        uintptr_t key = HwiP_disable();
        notifyEventsMask |= eventType;
        HwiP_restore(key);
    }

    /* Post notifications if the NS side has registered a callback handle. */
    if (notifySecureCallbackHandle != NULL)
    {
        if (postSynchronously)
        {
            /* Tell callback function which eventType to process. */
            notifySecureCallbackHandle->arg = eventType;

            /* Post synchronously (call the callback function immediately). */
            SecureCallback_s_postSynchronous(notifySecureCallbackHandle);

            /* Reset callback argument. */
            notifySecureCallbackHandle->arg = (uintptr_t)NULL;
        }
        else
        {
            /* Post the event (handled asyncronously). */
            SecureCallback_s_post(notifySecureCallbackHandle);
        }
    }

    return Power_SOK;
}

/*
 *  ======== PowerCC27XX_s_setDependencyCount ========
 */
static void PowerCC27XX_s_setDependencyCount(Power_Resource resourceId,
                                             uint8_t count,
                                             PowerCC27XX_SecurityState secState)
{
    uint8_t bitIndex;
    uint_fast16_t group;

    DebugP_assert(PowerCC27XX_isValidResourceId(resourceId));

    bitIndex = PowerCC27XX_getResourceBitIndex(resourceId);
    group    = PowerCC27XX_getResourceGroup(resourceId);

    if (group == PowerCC27XX_PERIPH_GROUP_CLKCTL0)
    {
        resourceCountsClkctl0[secState][bitIndex] = count;
    }
    else if (group == PowerCC27XX_PERIPH_GROUP_CLKCTL1)
    {
        resourceCountsClkctl1[secState][bitIndex] = count;
    }
    else if (group == PowerCC27XX_PERIPH_GROUP_LRFD)
    {
        resourceCountsLrfd[secState][bitIndex] = count;
    }
    else
    {
        /* Do nothing. Else statement is needed to be MISRA-C compliant. */
    }
}

/*
 *  ======== PowerCC27XX_isValidResourceId ========
 */
bool PowerCC27XX_isValidResourceId(Power_Resource resourceId)
{
    uint8_t bitIndex    = PowerCC27XX_getResourceBitIndex(resourceId);
    uint_fast16_t group = PowerCC27XX_getResourceGroup(resourceId);

    bool result;

    if (resourceId != ((uint32_t)bitIndex | (uint32_t)group))
    {
        result = false;
    }
    else
    {
        switch (group)
        {
            case PowerCC27XX_PERIPH_GROUP_CLKCTL0:
                result = bitIndex < PowerCC27XX_NUMRESOURCES_CLKCTL0;
                break;
            case PowerCC27XX_PERIPH_GROUP_CLKCTL1:
                result = bitIndex < PowerCC27XX_NUMRESOURCES_CLKCTL1;
                break;
            case PowerCC27XX_PERIPH_GROUP_LRFD:
                result = bitIndex < PowerCC27XX_NUMRESOURCES_LRFD;
                break;
            default:
                result = false;
                break;
        }
    }

    return result;
}

/*
 *  ======== PowerCC27XX_s_getCurrentEventsMask ========
 */
uint_fast16_t PowerCC27XX_s_getCurrentEventsMask(void)
{
    uintptr_t key;
    uint_fast16_t currentMask;

    key = HwiP_disable();

    /* Get the current events mask to pass to the NS side, and clear the
     * secure-side variable.
     */
    currentMask      = notifyEventsMask;
    notifyEventsMask = 0;

    HwiP_restore(key);

    return currentMask;
}

/*
 *  ======== PowerCC27XX_s_setNonSecureNotificationHandle ========
 */
int_fast16_t PowerCC27XX_s_setNonSecureNotificationHandle(SecureCallback_Handle handle)
{
    /* Argument verification is handled in the veneer. */

    notifySecureCallbackHandle = handle;

    /* If there are events pending, post them immediately. */
    if ((notifySecureCallbackHandle != NULL) && (notifyEventsMask != 0U))
    {
        SecureCallback_s_post(notifySecureCallbackHandle);
    }

    return Power_SOK;
}

/*
 *  ======== PowerCC27XX_getResourceGroup ========
 * Extract the resource group  from a resource ID
 */
static inline uint_fast16_t PowerCC27XX_getResourceGroup(Power_Resource resourceId)
{
    return (uint_fast16_t)resourceId & PowerCC27XX_PERIPH_GROUP_M;
}

/*
 *  ======== PowerCC27XX_getResourceBitIndex ========
 * Extract the bit index shift encoded from a resource ID
 */
static inline uint8_t PowerCC27XX_getResourceBitIndex(Power_Resource resourceId)
{
    return (uint8_t)(resourceId & PowerCC27XX_PERIPH_BIT_INDEX_M);
}

/*
 *  ======== PowerCC27XX_getIocAddr ========
 */
static inline uint_fast16_t PowerCC27XX_getIocAddr(uint_fast16_t iocIndex)
{
    return IOC_BASE + IOC_O_IOC0 + (sizeof(uint32_t) * (iocIndex));
}

/*
 *  ======== PowerLPF3_sleep ========
 * This function is called from the NS power policy function.
 *
 * The function will enter standby if allowed, using RTC as a wakeup
 * source. If not it will enter idle if allowed.
 */
int_fast16_t PowerLPF3_sleep(uint32_t nextEventTimeUs)
{
    uint32_t constraints;
    uint32_t soonestDelta;
    uint32_t ticksBefore;
    uint32_t sysTimerIMASK;
    uint32_t sysTimerARMSET;
    uint32_t sysTimerTimeouts[SYSTIMER_CHANNEL_COUNT];
    uint32_t key;
    uint32_t nsKey;
    bool standbyAllowed;
    bool idleAllowed;
    bool doIdle         = false;
    int_fast16_t result = Power_SOK;

    /* Disallow NS and S interrupts */
    nsKey = __TZ_get_PRIMASK_NS();
    __TZ_set_PRIMASK_NS(1U);
    key = __get_PRIMASK();
    __set_PRIMASK(1U);

    /* Check state of constraints */
    constraints    = Power_getConstraintMask();
    standbyAllowed = (constraints & ((uint32_t)1U << PowerLPF3_DISALLOW_STANDBY)) == 0U;
    idleAllowed    = (constraints & ((uint32_t)1U << PowerLPF3_DISALLOW_IDLE)) == 0U;

    /* If we are using LFOSC, it has been observed a brief period of ~15us
     * occurring ~130us after starting HFXT where FLTSETTLED pulses high. If the
     * idle loop attempts to enter standby while FLTSETTLED pulses high, it may
     * enter standby before the filter is truly settled. To prevent this, we
     * need to wait until the next LFTICK once HFXTGOOD is set before going to
     * sleep. This is achieved by clearing the LFTICK interrupt once AMPSETTLED
     * is set (which occurs after HFXTGOOD), and then check against LFTICK
     * before entering standby. We also cannot enter idle instead of standby
     * because otherwise we could end up waiting for the next standby wakeup
     * signal from the RTC or another wakeup source while we are still in idle.
     * That could be a very long time.
     * But if standby is currently disallowed from the constraints, that means
     * we do want to enter idle since something set that constraint and will
     * lift it again.
     */
    if (standbyAllowed && (PowerLPF3_isLfincFilterAllowingStandby() == false))
    {
        /* We cannot enter standby until LFINC filter has settled, we also
         * cannot enter idle instead of standby because otherwise we could end
         * up waiting for the next standby wakeup signal from SysTimer or
         * another wakeup source while we are still in idle. That could be a
         * very long time. But if standby is currently disallowed from the
         * constraints, that means we do want to enter idle since something set
         * that constraint and will lift it again.
         */
        standbyAllowed = false;
        idleAllowed    = false;
    }

    if (standbyAllowed)
    {
        /* When working with SysTimer/RTC values, we have to work with relative
         * values to properly handle when the timer overflows. We take the
         * difference between the next event (nextEventTimeUs) and "now"
         * (ticksBefore) to work with relative values. Underflow might occur in
         * the subtraction, but this is intentional.
         * If the next event is in the past then soonestDelta will be greater
         * than MAX_SYSTIMER_DELTA.
         */
        ticksBefore  = HWREG(SYSTIM_BASE + SYSTIM_O_TIME1U);
        soonestDelta = nextEventTimeUs - ticksBefore;
        if ((soonestDelta > PowerCC27XX_TOTALTIMESTANDBY) && (soonestDelta <= MAX_SYSTIMER_DELTA))
        {
            /* Disable NS SysTick */
            SysTick_NS->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

            /* Save SysTimer IMASK to restore afterwards */
            sysTimerIMASK = HWREG(SYSTIM_BASE + SYSTIM_O_IMASK);

            /* Get current armed status of all SysTimer channels */
            sysTimerARMSET = HWREG(SYSTIM_BASE + SYSTIM_O_ARMSET);

            /* Store SysTimer timeouts */
            (void)memcpy(sysTimerTimeouts, (uint32_t *)(SYSTIM_BASE + SYSTIM_O_CH0CCSR), sizeof(sysTimerTimeouts));

            /* Switch CPUIRQ16 in event fabric to RTC Since the CC27XX only has
             * limited interrupt lines, we need to switch the interrupt line
             * from SysTimer to RTC in the event fabric. The triggered interrupt
             * will wake up the device with interrupts disabled. We can consume
             * that interrupt event without vectoring to the ISR and then change
             * the event fabric signal back to the SysTimer. Thus, there is no
             * need to swap out the actual interrupt function of the clockHwi.
             */
            EVTSVTConfigureEvent(EVTSVT_SUB_CPUIRQ16, EVTSVT_PUB_AON_RTC_COMB);

            /* Clear interrupt in case it triggered since we disabled interrupts */
            HwiP_clearInterrupt(INT_CPUIRQ16);

            /* Ensure the device wakes up early enough to reinitialise the
             * HW and take care of housekeeping.
             */
            soonestDelta -= PowerCC27XX_WAKEDELAYSTANDBY;

            /* RTC channel 0 compare is automatically armed upon writing the
             * compare value. It will automatically be disarmed when it
             * triggers.
             */
            HWREG(RTC_BASE + RTC_O_CH0CC1U) = ticksBefore + soonestDelta;

            /* Go to standby mode */
            result = Power_sleep(PowerLPF3_STANDBY);

            /* Disarm RTC compare event in case we woke up from a GPIO or BATMON
             * event. If the RTC times out after clearing RIS and the pending
             * NVIC bit but before we swap event fabric subscribers for the
             * shared interrupt line, we will be left with a pending interrupt
             * in the NVIC that the ClockP callback may not gracefully handle
             * since it did not cause it itself.
             */
            HWREG(RTC_BASE + RTC_O_ARMCLR) = RTC_ARMCLR_CH0_CLR;

            /* Clear the RTC wakeup event */
            HWREG(RTC_BASE + RTC_O_ICLR) = RTC_ICLR_EV0_CLR;

            /* Explicitly read back from ULL domain to guarantee clearing RIS
             * takes effect before clearing the pending NVIC interrupt to avoid
             * the NVIC re-asserting on a set RIS.
             */
            ULLSync();

            /* Clear any pending interrupt in the NVIC */
            HwiP_clearInterrupt(INT_CPUIRQ16);

            /* Switch CPUIRQ16 in event fabric back to SysTimer */
            EVTSVTConfigureEvent(EVTSVT_SUB_CPUIRQ16, EVTSVT_PUB_SYSTIM0);

            /* When waking up from standby, the SysTimer may not have
             * synchronised with the RTC by now. Wait for SysTimer
             * synchronisation with the RTC to complete. This should not take
             * more than one LFCLK period.
             *
             * We need to wait both for RUN to be set and SYNCUP to go low. Any
             * other register state will cause undefined behaviour.
             */
            while (HWREG(SYSTIM_BASE + SYSTIM_O_STATUS) != SYSTIM_STATUS_VAL_RUN) {}

            /* Restore SysTimer timeouts */
            (void)memcpy((uint32_t *)(SYSTIM_BASE + SYSTIM_O_CH0CCSR), sysTimerTimeouts, sizeof(sysTimerTimeouts));

            /* Restore SysTimer armed state. This will rearm all previously
             * armed timeouts restored above and cause any that occurred in the
             * past to trigger immediately.
             */
            HWREG(SYSTIM_BASE + SYSTIM_O_ARMSET) = sysTimerARMSET;

            /* Restore SysTimer IMASK */
            HWREG(SYSTIM_BASE + SYSTIM_O_IMASK) = sysTimerIMASK;

            /* Re-configure LRFD clocks */
            LRFDApplyClockDependencies();

            /* Signal clients registered for standby wakeup notification;
             * this should be used to initialize any timing critical or IO
             * dependent hardware.
             * The callback needs to go out after the SysTimer is restored
             * such that notifications can invoke RCL and ClockP APIs if needed.
             */
            (void)PowerCC27XX_notify(PowerLPF3_AWAKE_STANDBY);
        }
        else if (idleAllowed)
        {
            /* Since the event is not far enough in the future, idle should
             * be used instead, since it is allowed.
             */
            doIdle = true;
        }
        else
        {
            /* Empty else statment to satisfy MISRA-C */
        }
    }
    else if (idleAllowed)
    {
        doIdle = true;
    }
    else
    {
        /* Empty else statment to satisfy MISRA-C */
    }

    /* Restore S interrupts */
    __set_PRIMASK(key);

    if (doIdle)
    {
        /* PowerCC27XX_doWFI() should be called with S interrupts enabled */
        PowerCC27XX_doWFI();
    }
    /* Restore NS interrupts */
    __TZ_set_PRIMASK_NS(nsKey);

    return result;
}

/*
 *  ======== PowerCC27XX_doWFI ========
 */
void PowerCC27XX_doWFI(void)
{
    uint32_t constraints;
    bool idleAllowed;
    bool pendingNsInt;
    bool pendingSInt;
    uint32_t nsKey;

    /* Disallow NS interrupts while looping */
    nsKey = __TZ_get_PRIMASK_NS();
    __TZ_set_PRIMASK_NS(1);

    /* Loop until a NS or S interrupt becomes pending. However, S interrupts
     * are not disabled above, so the CPU will be able to vector to the S
     * interrupts as soon as they become pending, assuming nothing else prevents
     * it, and thus this loop essentially just waits for NS interrupts to become
     * pending, which is what we want.
     */
    do
    {
        constraints = Power_getConstraintMask();
        idleAllowed = (constraints & ((uint32_t)1U << PowerLPF3_DISALLOW_IDLE)) == 0U;
        if (idleAllowed)
        {
            __WFI();
        }
        /* Determine if there are any pending NS interrupts */
        pendingNsInt = ((SCB_NS->ICSR & SCB_ICSR_VECTPENDING_Msk) >> SCB_ICSR_VECTPENDING_Pos) != 0U;

        /* Determine if there are any pending S interrupts. If there is a
         * pending S interrupt, it means that something prevents the CPU from
         * vectoring to it. For example, S interrupts could be disabled.
         * Exit the loop if that is the case.
         */
        pendingSInt = ((SCB->ICSR & SCB_ICSR_VECTPENDING_Msk) >> SCB_ICSR_VECTPENDING_Pos) != 0U;
    } while ((pendingNsInt == false) && (pendingSInt == false));

    /* Restore NS interrupts */
    __TZ_set_PRIMASK_NS(nsKey);
}

/*
 *  ======== PowerLPF3_isLfincFilterAllowingStandby ========
 */
bool PowerLPF3_isLfincFilterAllowingStandby(void)
{
    bool result = true;
    /* If we are using LFOSC, it has been observed a brief period of ~15us
     * occurring ~130us after starting HFXT where FLTSETTLED pulses high. If the
     * idle loop attempts to enter standby while FLTSETTLED pulses high, it may
     * enter standby before the filter is truly settled. To prevent this, we
     * need to wait until the next LFTICK once HFXTGOOD is set before going to
     * sleep. This is achieved by clearing the LFTICK interrupt once AMPSETTLED
     * is set (which occurs after HFXTGOOD), and then check against LFTICK
     * before entering standby.
     */
    if ((HWREG(CKMD_BASE + CKMD_O_LFCLKSEL) & CKMD_LFCLKSEL_MAIN_M) == CKMD_LFCLKSEL_MAIN_LFOSC)
    {
        result = ((HWREG(CKMD_BASE + CKMD_O_RIS) & CKMD_RIS_LFTICK_M) == CKMD_RIS_LFTICK);
    }

    return result;
}
