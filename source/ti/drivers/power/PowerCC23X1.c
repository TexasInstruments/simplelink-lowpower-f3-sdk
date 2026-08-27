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
/*
 *  ======== PowerCC23X1.c ========
 */

#include <stdbool.h>
#include <string.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/DebugP.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC23X1.h>
#include <ti/drivers/Temperature.h>
#include <ti/drivers/GPIO.h>

#include <ti/drivers/utils/Math.h>

#include <ti/log/Log.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_evtull.h)
#include DeviceFamily_constructPath(inc/hw_ckmd.h)
#include DeviceFamily_constructPath(inc/hw_rtc.h)
#include DeviceFamily_constructPath(inc/hw_systim.h)
#include DeviceFamily_constructPath(inc/hw_pmctl.h)
#include DeviceFamily_constructPath(inc/hw_ioc.h)
#include DeviceFamily_constructPath(driverlib/ckmd.h)
#include DeviceFamily_constructPath(driverlib/clkctl.h)
#include DeviceFamily_constructPath(driverlib/evtsvt.h)
#include DeviceFamily_constructPath(driverlib/hapi.h)
#include DeviceFamily_constructPath(driverlib/gpio.h)
#include DeviceFamily_constructPath(driverlib/lrfd.h)
#include DeviceFamily_constructPath(driverlib/pmctl.h)
#include DeviceFamily_constructPath(driverlib/systick.h)
#include DeviceFamily_constructPath(driverlib/systimer.h)
#include DeviceFamily_constructPath(driverlib/ull.h)
#include DeviceFamily_constructPath(cmsis/core/cmsis_compiler.h)

/* Missing driverlib defines.
 * TODO: Use driverlib defines when/if they exist or verify values.
 */
#define CKMD_HFTRACKCTL_RATIO_REF48M 0x00400000U
#define CKMD_TRIGSRC_STOP_SRC_LFXT   0x00000200U
#define CKMD_TRIGSRC_START_SRC_LFXT  0x00000002U
#define CKMD_LFCLKSEL_MAIN_LFXT      0x00000002U
#define CKMD_LFCLKSEL_PRE_LFXT       0x00000008U

/* Type definitions */

/*! Type used for passing configuration information to HFXT temperature
 * notification callback
 */
typedef union
{
    struct
    {
        int16_t delta;
        int16_t threshold;
    } temperature;
    uint32_t value;
} PowerCC23X1_hfxtConfig;

/* Function type to qualify a LFCLK source.
 * To be called from PowerCC23X1_oscillatorISR()
 * maskedStatus is CMKD.MIS at the entry of PowerCC23X1_oscillatorISR()
 *
 * The function should return true if the qualification process is done,
 * otherwise false.
 */
typedef bool (*PowerLPF3_LfclkQualFxn)(uint32_t maskedStatus);

/* LFCLK TDC trigger sources used for LFCLK qualification. */
typedef enum
{
    PowerCC23X1_LFOSC = CKMD_TRIGSRC_STOP_POL_HIGH | CKMD_TRIGSRC_STOP_SRC_LFOSC | CKMD_TRIGSRC_START_POL_HIGH |
                        CKMD_TRIGSRC_START_SRC_LFOSC,
    PowerCC23X1_LFXT = CKMD_TRIGSRC_STOP_POL_HIGH | CKMD_TRIGSRC_STOP_SRC_LFXT | CKMD_TRIGSRC_START_POL_HIGH |
                       CKMD_TRIGSRC_START_SRC_LFXT,
} PowerCC23X1_LfclkTdcTrigSrc;

/* Forward declarations */
int_fast16_t PowerCC23X1_notify(uint_fast16_t eventType);
static void PowerCC23X1_oscillatorISR(uintptr_t arg);
static void PowerCC23X1_rtcISR(uintptr_t arg);
static void PowerCC23X1_enterStandby(void);
static void PowerCC23X1_setDependencyCount(Power_Resource resourceId, uint8_t count);
bool PowerCC23X1_isValidResourceId(Power_Resource resourceId);
static void PowerCC23X1_startHFXT(void);

static void PowerCC23X1_hfxtCompensateFxn(int16_t currentTemperature,
                                          int16_t thresholdTemperature,
                                          uintptr_t clientArg,
                                          Temperature_NotifyObj *notifyObject);
static uint32_t PowerCC23X1_temperatureToRatio(int16_t temperature);
static void PowerCC23X1_updateHFXTRatio(uint32_t ratio);

static void PowerCC23X1_hfxtAmpsettledTimeout(uintptr_t arg);
static void PowerCC23X1_initialHfxtAmpCompClockCb(uintptr_t searchDone);
static void PowerCC23X1_forceHfxtFsmToRamp1(void);
static void PowerCC23X1_startContHfxtAmpMeasurements(void);
static void PowerCC23X1_stopContHfxtAmpMeasurements(void);
static uint32_t PowerCC23X1_getHfxtAmpMeasurement(void);

static void PowerCC23X1_startLfclkTdcMeasurement(PowerCC23X1_LfclkTdcTrigSrc tdcTrigSrc);
static void PowerCC23X1_restartLfclkTdcMeasurement(void);
static bool PowerCC23X1_extlfQual(uint32_t maskedStatus);
static bool PowerCC23X1_lfxtQual(uint32_t maskedStatus);
static bool PowerCC23X1_lfoscQual(uint32_t maskedStatus);

/* Externs */
extern const PowerCC23X1_Config PowerCC23X1_config;
extern const uint_least8_t GPIO_pinLowerBound;
extern const uint_least8_t GPIO_pinUpperBound;
extern const uint_least8_t PowerLPF3_extlfPin;
extern const uint_least8_t PowerLPF3_extlfPinMux;

/* Macro for weak definition of the Power Log module */
Log_MODULE_DEFINE_WEAK(LogModule_Power, Log_MODULE_INIT_SINK_DUMMY);

/* Function Macros */
#define IOC_ADDR(index) (IOC_BASE + IOC_O_IOC0 + (sizeof(uint32_t) * (index)))

/* Macro used to extract the resource group  from a resource ID */
#define RESOURCE_GROUP(resourceId) ((resourceId)&PowerCC23X1_PERIPH_GROUP_M)

/* Macro used to extract the index for the given group from a resource ID */
#define RESOURCE_INDEX(resourceId) ((resourceId)&PowerCC23X1_PERIPH_INDEX_M)

#define HFXT_COMP_MAX_TEMP (125)
#define HFXT_COMP_MIN_TEMP (-40)

/* Timeout value used to detect if HFXT FSM is stuck in RAMP0 state */
#define HFXT_AMP_COMP_START_TIMEOUT_US (500U)

/* Time to wait after changing HFXTTARG.IREF and measuring the resulting
 * HFXT amplitude
 */
#define HFXT_AMP_COMP_MEASUREMENT_US (1000U)

/* The limits for the allowed target IREF values to be passed to
 * CKMDSetTargetIrefTrim()
 */
#define HFXT_TARGET_IREF_MAX (8U)
#define HFXT_TARGET_IREF_MIN (3U)

/* Initial LFINC value for the LFINC filter. The value is representing the
 * nominal LFOSC frequency of 32.768 kHz in us with 16 fractional bits.
 */
#define LFINCCTL_INT_START_VALUE (0x1E8480U)

/* Static Globals */

/*! Temperature notification to compensate the HFXT */
static Temperature_NotifyObj PowerCC23X1_hfxtCompNotifyObj = {0};

/* Initialize value to the CKMD:HFTRACKCTL.RATIO field reset value */
static uint32_t hfxtCompRatio = CKMD_HFTRACKCTL_RATIO_REF48M;

/*! Temperature compensation coefficients for HFXT. */
static struct
{
    int32_t P0;
    int32_t P1;
    int32_t P2;
    int32_t P3;
    uint8_t shift;
} PowerCC23X1_hfxtCompCoefficients;

/*! Temperature compensation coefficients for the internal cap array.
 *  The variables are weakly defined to allow the application to override them.
 */
__attribute__((weak)) const uint32_t PowerLPF3_capArrayP0   = 553648128;
__attribute__((weak)) const uint32_t PowerLPF3_capArrayP1   = 4697620;
__attribute__((weak)) const uint8_t PowerLPF3_capArrayShift = 26;

/* LFCLK Qualification function */
static PowerLPF3_LfclkQualFxn PowerLPF3_lfclkQualFxn = NULL;

/* Global state variable to track if HFXT compensation is enabled or not.
 * It is used to check whether temperature notifications should be re-enabled
 * after an update or not, in case compensation has been asynchronously disabled
 */
static bool PowerCC23X1_hfxtCompEnabled = false;

/* Array to maintain constraint reference counts.
 * Declare volatile variable to ensure the toolchain does not use
 * stack for the variable and does not optimize this memory allocation away.
 */
static volatile uint8_t constraintCounts[PowerCC23X1_NUMCONSTRAINTS];

/* Mask of Power constraints for quick access.
 * Declare volatile variable to ensure the toolchain does not use
 * stack for the variable and does not optimize this memory allocation away.
 */
static volatile uint32_t constraintMask = 0;

/* Arrays to maintain resource dependency reference counts.
 * Each resource group will have an array associated with it, and the arrays can
 * be indexed using the bit index shift value encoded in the resource ID.
 * Declare volatile variable to ensure the toolchain does not use
 * stack for the variable and does not optimize this memory allocation away.
 */
static volatile uint8_t resourceCountsClkctl[PowerCC23X1_NUMRESOURCES_CLKCTL];
static volatile uint8_t resourceCountsLrfd[PowerCC23X1_NUMRESOURCES_LRFD];

/* Keeps track of the configured Power policy. Power_idleFunc() will not run
 * the policy if this is set to NULL
 */
static Power_PolicyFxn policyFxn = NULL;

/* Is the Power policy enabled? */
static bool isPolicyEnabled = false;

/* Has the Power driver been initialized */
static bool isInitialized = false;

/* Power state of the system. Idle, active, standby, etc */
static uint8_t powerState = Power_ACTIVE;

/* Event notification list */
static List_List notifyList;

/* Interrupt for handling clock switching */
static HwiP_Struct ckmHwi;

/* Function to be called to start the initial HFXT Amplitude compensation on the
 * next AMPSETTLED interrupt.
 * Will be NULL if initial compensation should not be
 * be performed. I.e. if it has already been performed or if it is not enabled.
 */
static PowerLPF3_StartInitialHfxtAmpCompFxn startInitialHfxtAmpCompFxn = NULL;

/* Clock object used by HFXT amplitude compensation. It is reused for multiple
 * purposes. The callback function of the ClockP object will be changed
 * dynamically, depending on the use case. The different use cases are described
 * below:
 * - Currently waiting for the AMPSETTLED interrupt, after starting HFXT
 *   - Callback function: PowerCC23X1_hfxtAmpsettledTimeout
 *   - Used for timeout to detect if HFXT FSM gets stuck in RAMP0 state
 * - Currently performing the initial HFXT amplitude compensation, where we are
 *   doing a full search for the optimal HFXTTARG.IREF value,
 *   after the AMPSETTLED signal and before the PowerLPF3_HFXT_AVAILABLE
 *   notification.
 *   - Callback function: PowerCC23X1_initialHfxtAmpCompClockCb
 *   - The clock is used to schedule measurements and evaluations of the HFXT
 *     amplitude.
 * - After PowerLPF3_HFXT_AVAILABLE notification:
 *   - Callback function: NULL
 *   - Currently not used. Could potentially be used to periodically check if
 *     amplitude adjustments are needed.
 *
 * Dynamically changing the callback function is done instead of having a common
 * callback function, to be able to only link the functions that are actually
 * used. For example, if the initial HFXT amplitude compensation at boot is not
 * enabled, then there will be no reference to
 * PowerLPF3_startInitialHfxtAmpComp() which is the only place that references
 * PowerCC23X1_initialHfxtAmpCompClockCb(), and therefore neither functions
 * will be linked in the application.
 */
static ClockP_Struct hfxtAmpCompClock;

/* Non-static Globals */

/* Interrupt for ClockP and Power policy */
HwiP_Struct clockHwi;

/* ****************** Power APIs ******************** */

/*
 *  ======== Power_init ========
 */
int_fast16_t Power_init(void)
{
    /* If this function has already been called, just return */
    if (isInitialized)
    {
        return Power_SOK;
    }

    isInitialized = true;

    isPolicyEnabled = (PowerCC23X1_config.policyFxn != NULL);

    policyFxn = PowerCC23X1_config.policyFxn;

    startInitialHfxtAmpCompFxn = PowerCC23X1_config.startInitialHfxtAmpCompFxn;

    /* Construct the CKM hwi responsible for oscillator related events.
     * Since there is no dedicated CKM interrupt line, we need to mux one of
     * the configurable lines to the CKM.
     * CPUIRQ3 is dedicated to this purpose.
     */
    HwiP_construct(&ckmHwi, INT_CPUIRQ3, PowerCC23X1_oscillatorISR, NULL);

    /* Mux the AON_CKM_COMB event to CPUIRQ3 */
    EVTSVTConfigureEvent(EVTSVT_SUB_CPUIRQ3, EVTSVT_PUB_AON_CKM_COMB);

    /* Enable a selection of CKM signals as interrupt sources. For now,
     * we will stick to AMPSETTLED since it is related to existing notification
     * and XBHFFAULT and TRACKREFLOSS to be able to handle HFXT clock loss.
     * TRACKREFLOSS may occur when entering and exiting fake standby with the
     * debugger attached.
     */
    HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMSET_AMPSETTLED | CKMD_IMSET_XBHFFAULT | CKMD_IMSET_TRACKREFLOSS;

    HwiP_enableInterrupt(INT_CPUIRQ3);

    /* Construct the ClockP hwi responsible for timing service events.
     * This Hwi is time multiplexed between the SysTimer and the RTC for use
     * by the ClockP and Power policy respectively.
     * Since there is no dedicated RTC or SysTimer interrupt line, we need to
     * mux one of the configurable lines to the CKM.
     * CPUIRQ16 is dedicated to this purpose.
     */
    HwiP_construct(&clockHwi, INT_CPUIRQ16, PowerCC23X1_rtcISR, NULL);

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

    /* Disable automatic periodic adjustments of HFXT amplitude. It will instead
     * be done in SW.
     * With this setting it is not necessary to restart HFXT after changing
     * IREF.
     */
    HWREG(CKMD_BASE + CKMD_O_AMPCFG1) &= ~CKMD_AMPCFG1_INTERVAL_M;

    /* Set target HFXT IREF to the max value, to ensure it is high enough.
     * It will be gradually updated as needed later, as part of the HFXT
     * amplitude compensation.
     */
    CKMDSetTargetIrefTrim(HFXT_TARGET_IREF_MAX);

    /* Construct HFXT amplitude compensation clock.
     * At boot, the clock is used to detect if the HFXT FSM gets stuck in the
     * RAMP0 state.
     */
    ClockP_construct(&hfxtAmpCompClock,
                     PowerCC23X1_hfxtAmpsettledTimeout,
                     HFXT_AMP_COMP_START_TIMEOUT_US / ClockP_getSystemTickPeriod(),
                     NULL);

    /* Start HFXT */
    PowerCC23X1_startHFXT();

    /* Start timeout clock.
     * Note, interrupts are guaranteed to be disabled during Power_init(), so
     * there is no risk of the AMPSETTLED callback stopping the clock before it
     * is started.
     */
    ClockP_start(&hfxtAmpCompClock);

    /* Enable tracking loop with HFXT as reference. This will automatically
     * calibrate LFOSC against HFXT whenever HFXT is enabled; usually after
     * waking up from standby.
     * This is needed to ensure fast HFXT startup and a reasonably accurate
     * LFOSC frequency.
     */
    HWREG(CKMD_BASE + CKMD_O_HFTRACKCTL) |= CKMD_HFTRACKCTL_EN_M | CKMD_HFTRACKCTL_REFCLK_XBHF;

    /* Enable RTC as a standby wakeup source */
    HWREG(EVTULL_BASE + EVTULL_O_WKUPMASK) = EVTULL_WKUPMASK_AON_RTC_COMB_M;

    return Power_SOK;
}

/*
 *  ======== Power_disablePolicy ========
 *  Do not run the configured policy
 */
bool Power_disablePolicy(void)
{
    bool wasPolicyEnabled = isPolicyEnabled;

    isPolicyEnabled = false;

    return wasPolicyEnabled;
}

/*
 *  ======== Power_enablePolicy ========
 *  Run the configured policy
 */
void Power_enablePolicy(void)
{
    isPolicyEnabled = true;
}

/*
 *  ======== Power_setPolicy ========
 *  Set the Power policy function
 */
void Power_setPolicy(Power_PolicyFxn policy)
{
    policyFxn = policy;
}

/*
 *  ======== Power_getConstraintMask ========
 *  Get a bitmask indicating the constraints that have been registered with
 *  Power.
 */
uint_fast32_t Power_getConstraintMask(void)
{
    return (uint_fast32_t)constraintMask;
}

/*
 *  ======== Power_getDependencyCount ========
 *  Get the count of dependencies that are currently declared upon a resource.
 */
int_fast16_t Power_getDependencyCount(Power_Resource resourceId)
{
    DebugP_assert(PowerCC23X1_isValidResourceId(resourceId));

    int_fast16_t result;
    uint8_t index       = RESOURCE_INDEX(resourceId);
    uint_fast16_t group = RESOURCE_GROUP(resourceId);

    if (group == PowerCC23X1_PERIPH_GROUP_CLKCTL)
    {
        result = (int_fast16_t)resourceCountsClkctl[index];
    }
    else if (group == PowerCC23X1_PERIPH_GROUP_LRFD)
    {
        result = (int_fast16_t)resourceCountsLrfd[index];
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
 *  operational transition
 */
int_fast16_t Power_getConstraintCount(uint_fast16_t constraintId)
{
    DebugP_assert(constraintId < PowerCC23X1_NUMCONSTRAINTS);

    int_fast16_t result = Power_EINVALIDINPUT;

    if (constraintId < PowerCC23X1_NUMCONSTRAINTS)
    {
        result = (int_fast16_t)constraintCounts[constraintId];
    }

    return result;
}

/*
 *  ======== Power_getTransitionLatency ========
 *  Get the transition latency for a sleep state.  The latency is reported
 *  in units of microseconds.
 */
uint_fast32_t Power_getTransitionLatency(uint_fast16_t sleepState, uint_fast16_t type)
{
    uint_fast32_t latency = 0;

    if (type == Power_RESUME)
    {
        if (sleepState == PowerLPF3_STANDBY)
        {
            latency = PowerCC23X1_RESUMETIMESTANDBY;
        }
    }
    else
    {
        if (sleepState == PowerLPF3_STANDBY)
        {
            latency = PowerCC23X1_TOTALTIMESTANDBY;
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
 *  ======== Power_idleFunc ========
 *  Function needs to be plugged into the idle loop.
 *  It calls the configured policy function if it is not NULL.
 */
void Power_idleFunc()
{
    if (policyFxn != NULL && isPolicyEnabled == true)
    {
        (*(policyFxn))();
    }
}

/*
 *  ======== Power_registerNotify ========
 *  Register a function to be called on a specific power event.
 */
int_fast16_t Power_registerNotify(Power_NotifyObj *notifyObj,
                                  uint_fast16_t eventTypes,
                                  Power_NotifyFxn notifyFxn,
                                  uintptr_t clientArg)
{
    int_fast16_t status = Power_SOK;

    /* Check for NULL pointers */
    if ((notifyObj == NULL) || (notifyFxn == NULL))
    {
        Log_printf(LogModule_Power,
                   Log_WARNING,
                   "Power_registerNotify: Notify registration failed due to NULL pointer");

        status = Power_EINVALIDPOINTER;
    }
    else
    {
        notifyObj->eventTypes = eventTypes;
        notifyObj->notifyFxn  = notifyFxn;
        notifyObj->clientArg  = clientArg;

        Log_printf(LogModule_Power,
                   Log_INFO,
                   "Power_registerNotify: Register fxn at address 0x%x with event types 0x%x and clientArg 0x%x",
                   notifyFxn,
                   eventTypes,
                   clientArg);

        /* Place notify object on event notification queue. Assume that
         * List_Elem struct member is the first struct member in
         * Power_NotifyObj.
         */
        List_put(&notifyList, (List_Elem *)notifyObj);
    }

    return status;
}

/*
 *  ======== Power_unregisterNotify ========
 *  Unregister for a power notification.
 *
 */
void Power_unregisterNotify(Power_NotifyObj *notifyObj)
{
    Log_printf(LogModule_Power,
               Log_INFO,
               "Power_unregisterNotify: Unregister fxn at address 0x%x with event types 0x%x and clientArg 0x%x",
               notifyObj->notifyFxn,
               notifyObj->eventTypes,
               notifyObj->clientArg);

    /* Remove notify object from its event queue */
    List_remove(&notifyList, (List_Elem *)notifyObj);
}

/*
 *  ======== Power_setConstraint ========
 *  Declare an operational constraint.
 */
int_fast16_t Power_setConstraint(uint_fast16_t constraintId)
{
    uintptr_t key;

    DebugP_assert(constraintId < PowerCC23X1_NUMCONSTRAINTS);

    key = HwiP_disable();

    /* Set the specified constraint in the constraintMask for faster access */
    constraintMask |= 1U << constraintId;

    /* Increment the specified constraint count */
    constraintCounts[constraintId]++;

    HwiP_restore(key);

    return Power_SOK;
}

/*
 *  ======== Power_releaseConstraint ========
 *  Release a previously declared constraint.
 */
int_fast16_t Power_releaseConstraint(uint_fast16_t constraintId)
{
    uintptr_t key;

    DebugP_assert(constraintId < PowerCC23X1_NUMCONSTRAINTS);

    key = HwiP_disable();

    DebugP_assert(constraintCounts[constraintId] != 0U);

    constraintCounts[constraintId]--;

    /* Only update the constraint mask if we removed the constraint entirely */
    if (constraintCounts[constraintId] == 0U)
    {
        constraintMask &= ~(1U << constraintId);
    }

    HwiP_restore(key);

    return Power_SOK;
}

/*
 *  ======== Power_setDependency ========
 *  Declare a dependency upon a resource.
 */
int_fast16_t Power_setDependency(Power_Resource resourceId)
{
    uint8_t previousCount;
    uintptr_t key;
    uint8_t index;

    DebugP_assert(PowerCC23X1_isValidResourceId(resourceId));

    key = HwiP_disable();

    /* Buffer previous reference count */
    previousCount = Power_getDependencyCount(resourceId);

    /* Increment reference count */
    PowerCC23X1_setDependencyCount(resourceId, previousCount + 1);

    /* If the resource was NOT activated previously ... */
    if (previousCount == 0)
    {
        index = RESOURCE_INDEX(resourceId);
        /* Turn on the peripheral */
        switch (RESOURCE_GROUP(resourceId))
        {
            case PowerCC23X1_PERIPH_GROUP_CLKCTL:
                CLKCTLEnable(index);
                break;
            case PowerCC23X1_PERIPH_GROUP_LRFD:
                LRFDSetClockDependency(1U << index, LRFD_CLK_DEP_POWER);
                break;
            default:
                break;
        }
    }

    Log_printf(LogModule_Power,
               Log_INFO,
               "Power_setDependency: Updated resource counter = %d for resource ID = 0x%x",
               Power_getDependencyCount(resourceId), resourceId);

    HwiP_restore(key);

    return Power_SOK;
}

/*
 *  ======== Power_releaseDependency ========
 *  Release a previously declared dependency.
 */
int_fast16_t Power_releaseDependency(Power_Resource resourceId)
{
    uint8_t resourceCount;
    uintptr_t key;
    uint8_t index;

    DebugP_assert(PowerCC23X1_isValidResourceId(resourceId));

    key = HwiP_disable();

    resourceCount = Power_getDependencyCount(resourceId);

    DebugP_assert(resourceCount != 0);

    /* Decrement the reference count */
    resourceCount--;
    PowerCC23X1_setDependencyCount(resourceId, resourceCount);

    /* If this was the last dependency being released.. */
    if (resourceCount == 0)
    {
        index = RESOURCE_INDEX(resourceId);
        /* Turn off the peripheral */
        switch (RESOURCE_GROUP(resourceId))
        {
            case PowerCC23X1_PERIPH_GROUP_CLKCTL:
                CLKCTLDisable(index);
                break;
            case PowerCC23X1_PERIPH_GROUP_LRFD:
                LRFDReleaseClockDependency(1U << index, LRFD_CLK_DEP_POWER);
                break;
            default:
                break;
        }
    }

    Log_printf(LogModule_Power,
               Log_INFO,
               "Power_releaseDependency: Updated resource counter = %d for resource ID = 0x%x",
               Power_getDependencyCount(resourceId), resourceId);

    HwiP_restore(key);

    return Power_SOK;
}

/*
 *  ======== Power_shutdown ========
 */
int_fast16_t Power_shutdown(uint_fast16_t shutdownState, uint_fast32_t shutdownTime)
{
    int_fast16_t notifyStatus;
    uintptr_t hwiKey;
    uint8_t i;
    bool ioPending = false;

    hwiKey = HwiP_disable();

    /* Check if there is a constraint to prohibit shutdown */
    if ((Power_getConstraintMask() & (1U << PowerLPF3_DISALLOW_SHUTDOWN)) != 0U)
    {
        HwiP_restore(hwiKey);
        return Power_ECHANGE_NOT_ALLOWED;
    }

    /* Check whether we were transitioning to another power state */
    if (powerState != Power_ACTIVE)
    {
        HwiP_restore(hwiKey);
        return Power_EBUSY;
    }

    /* Set new transition state to entering shutdown */
    powerState = Power_ENTERING_SHUTDOWN;

    /* Signal all clients registered for pre-shutdown notification */
    notifyStatus = PowerCC23X1_notify(PowerLPF3_ENTERING_SHUTDOWN);

    for (i = GPIO_pinLowerBound; i <= GPIO_pinUpperBound; i++)
    {
        /* Read WUCFGSD field once and check both values. */
        uint32_t ioShutdownConfig = HWREG(IOC_ADDR(i)) & IOC_IOC0_WUCFGSD_M;

        if (((ioShutdownConfig == IOC_IOC0_WUCFGSD_WAKE_HIGH) || (ioShutdownConfig == IOC_IOC0_WUCFGSD_WAKE_LOW)) &&
            (GPIOGetEventDio(i, false) != 0U))
        {
            ioPending = true;
        }
    }

    /* If no IO event is pending on a shutdown wakeup IO, go to shutdown */
    if (ioPending == false && notifyStatus == Power_SOK)
    {
        HWREG(PMCTL_BASE + PMCTL_O_SHTDWN) = PMCTL_SHTDWN_KEY_VALID;
    }

    /* If shutdown succeeded, should never get here */

    powerState = Power_ACTIVE;

    HwiP_restore(hwiKey);

    /* If we get here, failed to shutdown, return error code */
    return Power_EFAIL;
}

/*
 *  ======== Power_sleep ========
 */
int_fast16_t Power_sleep(uint_fast16_t sleepState)
{
    int_fast16_t status = Power_SOK;

    /* Signal all clients registered for pre standby wakeup notification */
    status = PowerCC23X1_notify(PowerLPF3_ENTERING_STANDBY);

    /* Check for any error */
    if (status != Power_SOK)
    {
        Log_printf(LogModule_Power,
                   Log_WARNING,
                   "Power_sleep: Entering standby failed with status = 0x%x",
                   status);

        powerState = Power_ACTIVE;
        return status;
    }

    /* Adjust HFXT amplitude if needed */
    int_fast8_t adjustment = PowerLPF3_getHfxtAmpAdjustment();

    /* Stop continuous measurements of HFXT amplitude before entering standby,
     * since the same hardware is needed to start the HFXT when waking up from
     * standby.
     */
    PowerCC23X1_stopContHfxtAmpMeasurements();

    if (adjustment != 0)
    {
        PowerLPF3_adjustHfxtAmp(adjustment);

        /* Explicitly turn off HFXT before entering standby. This is to ensure
         * that the HFXT is actually re-started with the new amplitude. In
         * some cases the HFXT will not be turned off when entering standby. For
         * example when entering fake standby with the debugger attached.
         */
        HWREG(CKMD_BASE + CKMD_O_XBHFCTL) &= ~CKMD_XBHFCTL_XBHFEN_M;
    }

    /* Call wrapper function to ensure that R0-R3 are saved and restored before
     * and after this function call. Otherwise, compilers will attempt to stash
     * values on the stack while on the PSP and then restore them just after
     * HapiEnterStandby() on the MSP. Which will cause wildly unexpected
     * behaviour.
     */
    PowerCC23X1_enterStandby();

    /* Now that we have returned and are executing code from flash again, start
     * up the HFXT. The HFXT might already have been enabled automatically by
     * hardware, but calling below ensures that it has been enabled and that the
     * necessary power constraints and interrupts are set and enabled.
     */
    PowerCC23X1_startHFXT();

    /* Now clear the transition state before re-enabling the scheduler */
    powerState = Power_ACTIVE;

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
 *  ======== PowerCC23X1_doWFI ========
 */
void PowerCC23X1_doWFI(void)
{
    uint32_t constraints;
    bool idleAllowed;
    bool flashNotNeeded;

    constraints    = Power_getConstraintMask();
    idleAllowed    = (constraints & (1 << PowerLPF3_DISALLOW_IDLE)) == 0;
    flashNotNeeded = (constraints & (1 << PowerLPF3_NEED_FLASH_IN_IDLE)) == 0U;

    if (idleAllowed)
    {

        /* Configure device to turn on/off flash LDO when in idle */
        if (flashNotNeeded)
        {
            /* FlashLdo can be turned off when in IDLE */
            CLKCTLEnableFlashLdoOffInIdle();
        }
        else
        {
            /* FlashLdo cannot be turned off when in IDLE */
            CLKCTLDisableFlashLdoOffInIdle();
        }

        /* Enter idle */
        __WFI();
    }
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
 *  ======== PowerCC23X1_enterStandby ========
 *  Wrapper function to ensure that R0-R3 are saved and restored before and
 *  after this function call. Otherwise, compilers will attempt to stash
 *  values on the stack while on the PSP and then restore them just after
 *  HapiEnterStandby() on the MSP. Which will cause wildly unexpected behaviour.
 */
void PowerCC23X1_enterStandby(void)
{
    /* Declare static volatile variable to ensure the toolchain does not use
     * stack for the variable and does not optimize this memory allocation
     * away.
     */
    static volatile uint32_t controlPreStandby;

    /* Clear all CKM LDO SW control bits. If we do not do this before entering
     * standby, the LDO will remain on in standby and consume power. We do not
     * disable it earlier after turning on HFXT to avoid waiting 20us to safely
     * disable it. NOTE: The SW Override bit must explicitly be cleared first,
     * before clearing the remaining bits, otherwise there is a risk of clock
     * loss in the tracking loop.
     */
    HWREG(CKMD_BASE + CKMD_O_LDOCTL) &= ~CKMD_LDOCTL_SWOVR_M;
    /* Clear remaining bits */
    HWREG(CKMD_BASE + CKMD_O_LDOCTL) = 0x0U;

    /* Stash current CONTROL configuration to re-apply after wakeup.
     * Depending on the kernel used, we could be on PSP or MSP
     */
    controlPreStandby = __get_CONTROL();

    /* Switch to MSP. HapiEnterStandby() must execute from MSP since the
     * device reboots into privileged mode on MSP and HapiEnterStandby()
     * assumes it will be called running on MSP.
     */
    __set_CONTROL(0U << 1U);

    /* - Save CPU state on MSP and MSP in CLKCTL_STBYPTR
     * - Enter standby
     * - Exit standby
     * - Restore CPU state from MSP
     * - Apply copyList
     */
    HapiEnterStandby(NULL);

    /* Switch back to previous stack pointer. */
    __set_CONTROL(controlPreStandby);
}

/*
 *  ======== PowerLPF3_selectLFOSC ========
 */
void PowerLPF3_selectLFOSC(void)
{
    uintptr_t key;

    /* Start LFOSC */
    HWREG(CKMD_BASE + CKMD_O_LFOSCCTL) = CKMD_LFOSCCTL_EN;

    /* Initialize LFINCCTL and LFINCCTL2 registers */
    HWREG(CKMD_BASE + CKMD_O_LFINCCTL) = (CKMD_LFINCCTL_PREVENTSTBY_ON | CKMD_LFINCCTL_KEEPXBHFEN_EN |
                                          (LFINCCTL_INT_START_VALUE << CKMD_LFINCCTL_INT_S) |
                                          CKMD_LFINCCTL_STOPGEAR_LOW | CKMD_LFINCCTL_ERRTHR_LARGE |
                                          CKMD_LFINCCTL_GEARRSTRT_TWOTHR | CKMD_LFINCCTL_SOFTRSTRT_ON |
                                          CKMD_LFINCCTL_EN_EN);

    HWREG(CKMD_BASE + CKMD_O_LFINCCTL2) |= CKMD_LFINCCTL2_ADJUSTLFINC;

    /* Enable TRACKREFOOR */
    HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMSET_TRACKREFOOR;

    Log_printf(LogModule_Power, Log_INFO, "PowerLPF3_selectLFOSC: LFOSC started");

    /* Set LFCLK qualification function to be called by
     * PowerCC23X1_oscillatorISR(). This function will process and evaluate the
     * result of the TDC measurement which is started below.
     */
    PowerLPF3_lfclkQualFxn = PowerCC23X1_lfoscQual;

    /* Disallow standby until LF clock is running.
     * The PowerCC23X1_oscillatorISR() function will release the constraint once
     * PowerLPF3_lfclkQualFxn returns true.
     */
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    key = HwiP_disable();

    /* If the AMPSETTLED mask is no longer set, it means that the AMPSETTLED
     * interrupt has already occured and been handled, so the HFXT is ready
     * to be used by the TDC. Otherwise, HFXT is not ready and the TDC will
     * be started by PowerCC23X1_lfoscQual() instead when receiving the
     * AMPSETTLED interrupt.
     */
    if ((HWREG(CKMD_BASE + CKMD_O_IMASK) & CKMD_IMASK_AMPSETTLED_M) != CKMD_IMASK_AMPSETTLED_M)
    {
        /* Start TDC measurement of LFOSC. Result will be processed by
         * PowerCC23X1_lfoscQual()
         */
        PowerCC23X1_startLfclkTdcMeasurement(PowerCC23X1_LFOSC);
    }

    HwiP_restore(key);
}

/*
 *  ======== PowerLPF3_selectLFXT ========
 */
void PowerLPF3_selectLFXT(void)
{
    uintptr_t key;

    /* Start LFXT */
    HWREG(CKMD_BASE + CKMD_O_LFXTCTL) = CKMD_LFXTCTL_EN;

    Log_printf(LogModule_Power, Log_INFO, "PowerLPF3_selectLFXT: LFXT started");

    /* Set LFCLK qualification function to be called by
     * PowerCC23X1_oscillatorISR(). This function will process and evaluate the
     * result of the TDC measurement which is started below.
     */
    PowerLPF3_lfclkQualFxn = PowerCC23X1_lfxtQual;

    /* Disallow standby until LF clock is running.
     * The PowerCC23X1_oscillatorISR() function will release the constraint once
     * PowerLPF3_lfclkQualFxn returns true.
     */
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    key = HwiP_disable();

    /* If the AMPSETTLED mask is no longer set, it means that the AMPSETTLED
     * interrupt has already occured and been handled, so the HFXT is ready
     * to be used by the TDC. Otherwise, HFXT is not ready and the TDC will
     * be started by PowerCC23X1_lfxtQual() instead.
     */
    if ((HWREG(CKMD_BASE + CKMD_O_IMASK) & CKMD_IMASK_AMPSETTLED_M) != CKMD_IMASK_AMPSETTLED_M)
    {
        /* Start TDC measurement of LFXT. Result will be processed by
         * PowerCC23X1_lfoscQual()
         */
        PowerCC23X1_startLfclkTdcMeasurement(PowerCC23X1_LFXT);
    }

    HwiP_restore(key);
}

/*
 *  ======== PowerLPF3_selectEXTLF ========
 */
void PowerLPF3_selectEXTLF(void)
{
    /* Configure EXTLF to the right mux */
    GPIO_setConfigAndMux(PowerLPF3_extlfPin, GPIO_CFG_INPUT, PowerLPF3_extlfPinMux);

    Log_printf(LogModule_Power, Log_INFO, "PowerLPF3_selectEXTLF: EXTLF pin muxing configured");

    /* Set LFINC override to 31.25 kHz.
     *
     * The value is calculated as period in microseconds with 16 fractional
     * bits.
     * The EXTLF runs at 31.25 kHz -> 1 / 31250 Hz = 32 us.
     * 32 * 2^16 = 2097152 = 0x00200000
     */
    HWREG(CKMD_BASE + CKMD_O_LFINCOVR) = 0x00200000U | CKMD_LFINCOVR_OVERRIDE;

    /* Directly switch to EXTLF, LFTICK will be generated by HFOSC until EXTLF
     * is running.
     */
    HWREG(CKMD_BASE + CKMD_O_LFCLKSEL) = CKMD_LFCLKSEL_MAIN_EXTLF;

    /* Set EXTLF qualification function to be called by
     * PowerCC23X1_oscillatorISR(). This function will return true when getting
     * the LFCLKGOOD interrupt.
     */
    PowerLPF3_lfclkQualFxn = PowerCC23X1_extlfQual;

    /* Disallow standby until LF clock is running.
     * The PowerCC23X1_oscillatorISR() function will release the constraint once
     * PowerLPF3_lfclkQualFxn returns true.
     */
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    /* Enable LFCLKGOOD interrupts */
    HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMASK_LFCLKGOOD;
}

/*
 *  ======== PowerCC23X1_extlfQual ========
 */
static bool PowerCC23X1_extlfQual(uint32_t maskedStatus)
{
    /* For EXTLF, the qualification is done when the HW indicates LFCLKGOOD */
    return (maskedStatus & CKMD_MIS_LFCLKGOOD_M) != 0U;
}

/*
 *  ======== PowerCC23X1_lfxtQual ========
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
static bool PowerCC23X1_lfxtQual(uint32_t maskedStatus)
{
    static int32_t lastTdcCount = 0;
    bool result                 = false;

    if ((maskedStatus & CKMD_MIS_AMPSETTLED_M) != 0U)
    {
        PowerCC23X1_startLfclkTdcMeasurement(PowerCC23X1_LFXT);
    }
    else if ((maskedStatus & CKMD_MIS_TDCDONE_M) != 0U)
    {
        Log_printf(LogModule_Power, Log_INFO, "PowerCC23X1_lfxtQual: TDC measurement done");

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
             * 100 ppm of 1500000 edges is 150 edges, and 500 ppm of 1500000
             * edges is 750 edges
             */
            isClockGood = (Math_ABS(lastOffset) < 750) && (Math_ABS(edgeOffset) < 750) &&
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
            PowerCC23X1_restartLfclkTdcMeasurement();
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
 *  ======== PowerCC23X1_lfoscQual ========
 *  - When getting the AMPSETTLED interrupt:
 *    - Start a TDC measurement of LFOSC
 *  - When getting the TDCDONE interrupt:
 *    - Determine if LFOSC is good.
 *      - If LFOSC is not good, the TDC will be restarted, and TDCDONE interrupt
 *        will be re-enabled.
 *      - If LFOSC is good:
 *        - The LFTICK interrupt will be enabled.
 *        - "Seed" the LFINC filter based on the period measured by the TDC.
 *  - When getting the LFTICK interrupt:
 *    - Select the LFOSC as the LFCLK source.
 *    - Return true to indicate that the qualification process is done.
 */
static bool PowerCC23X1_lfoscQual(uint32_t maskedStatus)
{
    static int32_t lastTdcCount = 0;
    bool result                 = false;

    if ((maskedStatus & CKMD_MIS_AMPSETTLED_M) != 0U)
    {
        PowerCC23X1_startLfclkTdcMeasurement(PowerCC23X1_LFOSC);
    }
    else if ((maskedStatus & CKMD_MIS_TDCDONE_M) != 0U)
    {
        Log_printf(LogModule_Power, Log_INFO, "PowerCC23X1_lfoscQual: TDC measurement done");

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

            /* For LFOSC the clock is considered good if the frequency is
             * within 32.768 kHz +/-3%, and the frequency is within +/-100
             * ppm of the last measurement.
             *
             * The expected number of edges is (96 MHz/32.768 kHz)*512 =
             * 1500000.
             * 3% of 1500000 edges, is 45000 and 100 ppm of 1500000 edges is
             * 150 edges.
             */
            isClockGood = (Math_ABS(lastOffset) < 45000) && (Math_ABS(edgeOffset) < 45000) &&
                          (Math_ABS(edgeDeltaLast) < 150);
        }

        if (isClockGood)
        {
            /* Clock is stable */

            /* Use calculated period to "seed" LFINC value */
            uint32_t tmpLfincctl = HWREG(CKMD_BASE + CKMD_O_LFINCCTL);
            tmpLfincctl &= ~CKMD_LFINCCTL_INT_M;

            /* Time of one LFOSC period in us: (edges/512)/96
             * Time of one LFOSC period in us, with 16 fractional bits (LFINC):
             *    ((edges/512)/96)*2^16 =
             *    ((edges/(2^9))/(3*2^5))*2^16 =
             *    (edges*2^2)/3 =
             *    (edges<<2)/3
             */
            tmpLfincctl |= (((edges << 2U) / 3U) << CKMD_LFINCCTL_INT_S) & CKMD_LFINCCTL_INT_M;

            HWREG(CKMD_BASE + CKMD_O_LFINCCTL) = tmpLfincctl;

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
            PowerCC23X1_restartLfclkTdcMeasurement();
        }
    }
    else if ((maskedStatus & CKMD_MIS_LFTICK_M) != 0U)
    {
        /* Select LFOSC as LFCLK source */
        HWREG(CKMD_BASE + CKMD_O_LFCLKSEL) = CKMD_LFCLKSEL_PRE_LFOSC | CKMD_LFCLKSEL_MAIN_LFOSC;

        /* Qualification is done */
        result = true;
    }
    else
    {
        /* Do nothing if none of the interrupts of interest to LFOSC
         * qualification are set
         */
    }

    return result;
}

/*
 *  ======== PowerCC23X1_startLfclkTdcMeasurement ========
 * This function will start a TDC measurment of the specified LFCLK source.
 * The counter will increment on each edfe of CLKSVT (48MHz), meaning the
 * counter will increment with a frequency of 96MHz. The TDC will be configured
 * to measure 512 periods of the LFCLK source. Meaning the expected result for a
 * 32.786kHz LFCLK will be (96MHz/32.786kHz)*512 = 1500000
 *
 * This function will also enable the TDCDONE interrupt. It is the caller's
 * responsibility to handle this interrupt and process the result.
 */
static void PowerCC23X1_startLfclkTdcMeasurement(PowerCC23X1_LfclkTdcTrigSrc tdcTrigSrc)
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
 *  ======== PowerCC23X1_restartLfclkTdcMeasurement ========
 */
static void PowerCC23X1_restartLfclkTdcMeasurement(void)
{
    /* Clear status from previous TDC measurement */
    HWREG(CKMD_BASE + CKMD_O_CTL) = CKMD_CTL_CMD_CLR_RESULT;

    /* Start new TDC measurement */
    HWREG(CKMD_BASE + CKMD_O_CTL) = CKMD_CTL_CMD_RUN_SYNC_START;

    /* Re-enable the TDCDONE interrupt */
    HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMCLR_TDCDONE;
}

/*
 *  ======== PowerCC23X1_oscillatorISR ========
 */
static void PowerCC23X1_oscillatorISR(uintptr_t arg)
{
    uint32_t maskedStatus = HWREG(CKMD_BASE + CKMD_O_MIS);

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
         * for XBHFGOOD and LFTICK to be set before entering standby. This is
         * achieved by clearing LFTICK once AMPSETTLED is set (which occurs
         * after XBHFGOOD is set), and then waiting for LFTICK to be set again
         * before entering standby.
         */
        HWREG(CKMD_BASE + CKMD_O_ICLR) = CKMD_ICLR_LFTICK;

        /* Stop AMPSETTLED timeout clock and change callback function of clock
         * object (NULL: not used)
         */
        ClockP_stop(&hfxtAmpCompClock);
        ClockP_setFunc(&hfxtAmpCompClock, NULL, 0);

        /* Start continuous measurements of HFXT amplitude */
        PowerCC23X1_startContHfxtAmpMeasurements();

        /* If enabled, start the initial HFXT amplitude compensation */
        if (startInitialHfxtAmpCompFxn != NULL)
        {
            /* Start the asynchronous initial HFXT amplitude compensation.
             * When the HFXT amplitude compensation is done the
             * PowerLPF3_HFXT_AVAILABLE notification will be posted, and the
             * PowerLPF3_DISALLOW_STANDBY constraint will be released.
             */
            startInitialHfxtAmpCompFxn();

            /* Only do the initial HFXT amplitude compensation once */
            startInitialHfxtAmpCompFxn = NULL;
        }
        else
        {
            Log_printf(LogModule_Power, Log_INFO, "PowerCC23X1_oscillatorISR: HFXT is available");

            PowerCC23X1_notify(PowerLPF3_HFXT_AVAILABLE);

            /* Allow standby again now that HFXT has finished starting */
            Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
        }
    }

    if ((maskedStatus & (CKMD_MIS_XBHFFAULT_M | CKMD_MIS_TRACKREFLOSS_M)) != 0U)
    {
        Log_printf(LogModule_Power, Log_WARNING, "PowerCC23X1_oscillatorISR: HFXT fault and/or TRACKREFLOSS, restarting HFXT");

        /* If there was a XBHFFAULT or TRACKREFLOSS, restart HFXT. Consider
         * issuing a notification to allow logging. If we keep it like this, we
         * could get stuck in an infinite loop restarting a faulty oscillator.
         * Then again, it is not like there is a great way to recover from that.
         */
        HWREG(CKMD_BASE + CKMD_O_XBHFCTL) &= ~CKMD_XBHFCTL_XBHFEN_M;

        /* Stop continuous measurements of HFXT amplitude. The hardware used is
         * needed to start HFXT.
         */
        PowerCC23X1_stopContHfxtAmpMeasurements();

        /* Max out IREF */
        CKMDSetTargetIrefTrim(HFXT_TARGET_IREF_MAX);

        /* If a HFXT fault or trackrefloss has occurred, the HFXT is
         * restarted.
         * When the HFXT is started, a "disallow standby" constraint is set, and
         * it is released when the HFXT has started (when getting the AMPSETTLED
         * signal). However, if an HFXT fault occurs while waiting for
         * AMPSETTLED, then the HFXT will be re-started setting a new constraint
         * without releasing the previous one. The constraint will end up
         * getting set twice but only released once (assuming the AMPSETTLED
         * signal is generated after the re-start).
         * To prevent this asymmetry, add a check to release the constraint if
         * the system is still waiting for the AMPSETTLED signal before
         * restarting the HFXT.
         */
        if (((maskedStatus & CKMD_MIS_AMPSETTLED_M) == 0U) &&
            ((HWREG(CKMD_BASE + CKMD_O_IMASK) & CKMD_IMASK_AMPSETTLED_M) == CKMD_IMASK_AMPSETTLED_M))
        {
            Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
        }

        /* Start up the HFXT using the workaround for the HFXT amplitude control
         * ADC bias point.
         */
        PowerCC23X1_startHFXT();

        /* Re-enable interrupts */
        HWREG(CKMD_BASE + CKMD_O_IMSET) = maskedStatus & (CKMD_MIS_XBHFFAULT_M | CKMD_MIS_TRACKREFLOSS_M);
    }

    if ((maskedStatus & CKMD_MIS_TRACKREFLOSS_M) != 0U)
    {
        Log_printf(LogModule_Power, Log_WARNING, "PowerCC23X1_oscillatorISR: TRACKREFLOSS, re-enable tracking");

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

            Log_printf(LogModule_Power, Log_INFO, "PowerCC23X1_oscillatorISR: LFCLK is ready");

            /* Enable LF clock monitoring */
            HWREG(CKMD_BASE + CKMD_O_LFMONCTL) = CKMD_LFMONCTL_EN;

            /* Enable LF clock loss reset while in standby */
            HWREG(PMCTL_BASE + PMCTL_O_RSTCTL) |= PMCTL_RSTCTL_LFLOSS_ARMED;

            /* Send out notification for LF clock switch */
            PowerCC23X1_notify(PowerLPF3_LFCLK_SWITCHED);

            /* Allow standby again now that we have sent out the notification */
            Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
        }
    }

    if ((maskedStatus & CKMD_MIS_TRACKREFOOR_M) != 0U)
    {
        Log_printf(LogModule_Power, Log_ERROR, "PowerCC23X1_oscillatorISR: TRACKREFOOR, entering endless loop");

        while (1) {}
    }
}

/*
 *  ======== PowerCC23X1_rtcISR ========
 */
static void PowerCC23X1_rtcISR(uintptr_t arg)
{
    /* We should never get here since we will just use the interrupt to wake
     * up the device from standby and then consume the interrupt while
     * interrupts are disabled.
     */
    HWREG(RTC_BASE + RTC_O_ICLR) = HWREG(RTC_BASE + RTC_O_MIS);
}

/*
 *  ======== PowerCC23X1_startHFXT ========
 * It should be ensured that the system does not vector to the CKMD interrupt
 * (PowerCC23X1_oscillatorISR) while this function is executing.
 * Meaning any of the following must be true:
 *  - Interrupts are disabled, or
 *  - This function is called from PowerCC23X1_oscillatorISR(), since nested
 *    vectoring to the same interrupt is not possible.
 */
static void PowerCC23X1_startHFXT(void)
{

    /* Start HFXT and enable automatic enabling after waking up from standby */
    HWREG(CKMD_BASE + CKMD_O_XBHFCTL) |= CKMD_XBHFCTL_XBHFEN | CKMD_XBHFCTL_XBHFAUTOEN;

    /* Disallow standby until AMPSETTLED is true */
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

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
 *  ======== PowerCC23X1_notify ========
 *  Send notifications to registered clients.
 *  Note: Task scheduling is disabled when this function is called.
 */
int_fast16_t PowerCC23X1_notify(uint_fast16_t eventType)
{
    int_fast16_t notifyStatus;
    Power_NotifyFxn notifyFxn;
    uintptr_t clientArg;
    List_Elem *elem;

    /* If queue is empty, return immediately */
    if (!List_empty(&notifyList))
    {
        /* Point to first client notify object */
        elem = List_head(&notifyList);

        /* Walk the queue and notify each registered client of the event */
        do
        {
            if ((((Power_NotifyObj *)elem)->eventTypes & eventType) != 0U)
            {
                /* Pull params from notify object */
                notifyFxn = ((Power_NotifyObj *)elem)->notifyFxn;
                clientArg = ((Power_NotifyObj *)elem)->clientArg;

                /* Call the client's notification function */
                Log_printf(LogModule_Power,
                           Log_VERBOSE,
                           "PowerCC23X1_notify: Invoking notification fxn at address 0x%x with event type 0x%x and clientArg 0x%x",
                           notifyFxn,
                           eventType,
                           clientArg);

                notifyStatus = (int_fast16_t)(*(Power_NotifyFxn)notifyFxn)(eventType, 0, clientArg);

                /* If client declared error stop all further notifications */
                if (notifyStatus != Power_NOTIFYDONE)
                {
                    Log_printf(LogModule_Power,
                               Log_WARNING,
                               "PowerCC23X1_notify: Notification fxn reported error, fxn at address 0x%x with event type 0x%x and notifyStatus 0x%x",
                               notifyFxn,
                               eventType,
                               notifyStatus);

                    return Power_EFAIL;
                }
            }

            /* Get next element in the notification queue */
            elem = List_next(elem);

        } while (elem != NULL);
    }

    return Power_SOK;
}

/*
 *  ======== PowerCC23X1_setDependencyCount ========
 */
void PowerCC23X1_setDependencyCount(Power_Resource resourceId, uint8_t count)
{
    uint8_t index       = RESOURCE_INDEX(resourceId);
    uint_fast16_t group = RESOURCE_GROUP(resourceId);

    DebugP_assert(PowerCC23X1_isValidResourceId(resourceId));

    if (group == PowerCC23X1_PERIPH_GROUP_CLKCTL)
    {
        resourceCountsClkctl[index] = count;
    }
    else if (group == PowerCC23X1_PERIPH_GROUP_LRFD)
    {
        resourceCountsLrfd[index] = count;
    }
}

/*
 *  ======== PowerCC23X1_isValidResourceId ========
 */
bool PowerCC23X1_isValidResourceId(Power_Resource resourceId)
{
    uint8_t index       = RESOURCE_INDEX(resourceId);
    uint_fast16_t group = RESOURCE_GROUP(resourceId);
    bool result;

    if (resourceId != ((uint32_t)index | (uint32_t)group))
    {
        result = false;
    }
    else
    {
        switch (group)
        {
            case PowerCC23X1_PERIPH_GROUP_CLKCTL:
                result = index < PowerCC23X1_NUMRESOURCES_CLKCTL;
                break;
            case PowerCC23X1_PERIPH_GROUP_LRFD:
                result = index < PowerCC23X1_NUMRESOURCES_LRFD;
                break;
            default:
                result = false;
                break;
        }
    }

    return result;
}

/*
 *  ======== PowerCC23X1_temperatureToRatio ========
 */
static uint32_t PowerCC23X1_temperatureToRatio(int16_t temperature)
{
    /* Calculate unshifted ppm offset. Fixed-point coefficients are assumed to
     * be set so that this computation does not overflow 32 bits in the -40, 125
     * degC range.
     */
    int32_t hfxtPpmOffset = PowerCC23X1_hfxtCompCoefficients.P3 * temperature * temperature * temperature +
                            PowerCC23X1_hfxtCompCoefficients.P2 * temperature * temperature +
                            PowerCC23X1_hfxtCompCoefficients.P1 * temperature + PowerCC23X1_hfxtCompCoefficients.P0;

    /* Calculate correct frequency offset, using shifted hfxtPpmOffset.
     * Frequency offset = 48000000 Hz * (hfxtPpmOffset >> shift) / 1000000
     *                  = 48 Hz * hfxtPpmOffset >> shift
     * Do 64-bit multiplication, since this will most likely overflow 32 bits.
     * Signed right-shift will result in an arithmetic shift operation.
     */
#if !(defined(__IAR_SYSTEMS_ICC__) || (defined(__clang__) && defined(__ti_version__)) || defined(__GNUC__))
    #warning The following signed right-shift operation is implementation-defined
#endif
    int32_t hfxtFreqOffset = (int32_t)((48LL * (int64_t)hfxtPpmOffset) >> PowerCC23X1_hfxtCompCoefficients.shift);

    /* Calculate unshifted ppm offset for Cap array compensation. Fixed-point coefficients are assumed to
     * be set so that this computation does not overflow 32 bits in the -40, 125
     * degC range.
     */
    int32_t hfxtCapArrayPpmOffset = PowerLPF3_capArrayP1 * temperature + PowerLPF3_capArrayP0;

#if !(defined(__IAR_SYSTEMS_ICC__) || (defined(__clang__) && defined(__ti_version__)) || defined(__GNUC__))
    #warning The following signed right-shift operation is implementation-defined
#endif
    int32_t capArrayOffset = (int32_t)((48LL * (int64_t)hfxtCapArrayPpmOffset) >> PowerLPF3_capArrayShift);

    /* Calculate the actual reference input frequency to the tracking loop,
     * accounting for the HFXT offset and cap array offset
     */
    int32_t refFreq = 48000000 + hfxtFreqOffset + capArrayOffset;

    /* Calculate word to write to HFTRACKCTL.RATIO. Expression taken from
     * register description: ratio = 24MHz / (2 * reference_frequency) * 2^24
     * 64-bit division is required, which is truncated to 32 bit
     */
    uint32_t ratio = (uint32_t)(0xB71B00000000LL / (int64_t)refFreq);

    return ratio;
}

/*
 *  ======== PowerCC23X1_updateHFXTRatio ========
 */
static void PowerCC23X1_updateHFXTRatio(uint32_t ratio)
{
    /* Store ratio so a later forced update has the right value */
    hfxtCompRatio = ratio;
    /* Check if updates are disabled */
    if ((Power_getConstraintMask() & (1 << PowerLPF3_DISALLOW_SWTCXO)) == 0)
    {
        /* Update HFTRACKCTL atomically */
        uintptr_t key = HwiP_disable();
        uint32_t temp = HWREG(CKMD_BASE + CKMD_O_HFTRACKCTL) & ~CKMD_HFTRACKCTL_RATIO_M;
        temp |= ratio & CKMD_HFTRACKCTL_RATIO_M;
        HWREG(CKMD_BASE + CKMD_O_HFTRACKCTL) = temp;
        HwiP_restore(key);
    }
}

/*
 *  ======== PowerCC23X1_hfxtCompensateFxn ========
 */
static void PowerCC23X1_hfxtCompensateFxn(int16_t currentTemperature,
                                          int16_t thresholdTemperature,
                                          uintptr_t clientArg,
                                          Temperature_NotifyObj *notifyObject)
{
    uintptr_t key;
    uint32_t ratio;
    int16_t inputTemperature = currentTemperature;
    PowerCC23X1_hfxtConfig hfxtConfig;
    hfxtConfig.value = (uint32_t)clientArg;

    /* Sanitize current temperature to fall within valid range. This ensures
     * that 32-bit overflow does not occur in the ppm calculation below.
     */
    if (currentTemperature > HFXT_COMP_MAX_TEMP)
    {
        currentTemperature = HFXT_COMP_MAX_TEMP;
    }

    if (currentTemperature < HFXT_COMP_MIN_TEMP)
    {
        currentTemperature = HFXT_COMP_MIN_TEMP;
    }

    key = HwiP_disable();

    /* If HFXT compensation has been disabled asynchronously during execution of
     * this callback then do not re-register the notification object
     */
    if (PowerCC23X1_hfxtCompEnabled)
    {
        Log_printf(LogModule_Power,
                   Log_INFO,
                   "PowerCC23X1_hfxtCompensateFxn: Registering notification. Temp = %d, Temp threshold = %d",
                   currentTemperature,
                   hfxtConfig.temperature.threshold);

        if (currentTemperature > hfxtConfig.temperature.threshold)
        {
            /* If temperature is above compensation threshold then compute a
             * compensated ratio-value and update the ratio register in the
             * tracking loop
             */
            ratio = PowerCC23X1_temperatureToRatio(currentTemperature);
            PowerCC23X1_updateHFXTRatio(ratio);

            if (inputTemperature <= HFXT_COMP_MIN_TEMP + hfxtConfig.temperature.delta)
            {
                /* If input temperature is at or below minimum temperature + delta, register
                 * a new high notification. Notification thresholds must be crossed to trigger,
                 * so the higher limit is decreased by 1 to get a notification after a temperature change of delta.
                 */
                Temperature_registerNotifyHigh(notifyObject,
                                               currentTemperature + hfxtConfig.temperature.delta - 1,
                                               PowerCC23X1_hfxtCompensateFxn,
                                               clientArg);
            }
            else if (inputTemperature >= HFXT_COMP_MAX_TEMP - hfxtConfig.temperature.delta)
            {
                /* If input temperature is at or higher than maximum temperature - delta, register
                 * a new low notification. Notification thresholds must be crossed to trigger,
                 * so the lower limit is increased by 1 to get a notification after a temperature change of delta.
                 */
                Temperature_registerNotifyLow(notifyObject,
                                              currentTemperature - hfxtConfig.temperature.delta + 1,
                                              PowerCC23X1_hfxtCompensateFxn,
                                              clientArg);
            }
            else
            {
                /* Register the notification again with updated thresholds. Notification thresholds must be crossed to
                 * trigger, so the upper and lower limits are decreased by 1 to maintain a range of +/- delta.
                 */
                Temperature_registerNotifyRange(notifyObject,
                                                currentTemperature + hfxtConfig.temperature.delta - 1,
                                                currentTemperature - hfxtConfig.temperature.delta + 1,
                                                PowerCC23X1_hfxtCompensateFxn,
                                                clientArg);
            }
        }
        else
        {
            /* If temperature is at or below compensation threshold then reset
             * the tracking loop ratio to remove any compensation, and register
             * a new high notification. The new limit should not be lower than
             * the compensation threshold.
             */
            PowerCC23X1_updateHFXTRatio(CKMD_HFTRACKCTL_RATIO_REF48M);

            Temperature_registerNotifyHigh(notifyObject,
                                           Math_MAX(hfxtConfig.temperature.threshold,
                                                    currentTemperature + hfxtConfig.temperature.delta - 1),
                                           PowerCC23X1_hfxtCompensateFxn,
                                           clientArg);
        }
    }

    HwiP_restore(key);
}

/*
 *  ======== PowerLPF3_initHFXTCompensation ========
 */
void PowerLPF3_initHFXTCompensation(int32_t P0,
                                    int32_t P1,
                                    int32_t P2,
                                    int32_t P3,
                                    uint8_t shift,
                                    __attribute__((unused)) bool fcfgInsertion)
{
    PowerCC23X1_hfxtCompCoefficients.P0    = P0;
    PowerCC23X1_hfxtCompCoefficients.P1    = P1;
    PowerCC23X1_hfxtCompCoefficients.P2    = P2;
    PowerCC23X1_hfxtCompCoefficients.P3    = P3;
    PowerCC23X1_hfxtCompCoefficients.shift = shift;

    /* If device offers FCFG insertion data it will be factored in here.
     * Currently no device supports this.
     */
}

/*
 *  ======== PowerLPF3_enableHFXTCompensation ========
 */
void PowerLPF3_enableHFXTCompensation(int16_t tempThreshold, int16_t tempDelta)
{

    if (PowerCC23X1_hfxtCompEnabled == false)
    {
        PowerCC23X1_hfxtCompEnabled = true;

        Temperature_init();

        int16_t currentTemperature = Temperature_getTemperature();

        PowerCC23X1_hfxtConfig config;
        config.temperature.threshold = tempThreshold;
        config.temperature.delta     = tempDelta;

        /* Only perform temperature compensation if the temperature is above the
         * set threshold. If it is not, then register a high notification on the
         * threshold
         */
        if (currentTemperature > tempThreshold)
        {
            PowerCC23X1_hfxtCompensateFxn(currentTemperature,
                                          0,
                                          (uintptr_t)config.value,
                                          &PowerCC23X1_hfxtCompNotifyObj);
        }
        else
        {
            Temperature_registerNotifyHigh(&PowerCC23X1_hfxtCompNotifyObj,
                                           tempThreshold,
                                           PowerCC23X1_hfxtCompensateFxn,
                                           (uintptr_t)config.value);
        }

        Log_printf(LogModule_Power,
                   Log_INFO,
                   "PowerLPF3_enableHFXTCompensation: Compensation enabled. Temp = %d, Temp threshold = %d",
                   currentTemperature,
                   tempThreshold);
    }
}

/*
 *  ======== PowerLPF3_disableHFXTCompensation ========
 */
void PowerLPF3_disableHFXTCompensation(void)
{

    uintptr_t key = HwiP_disable();
    if (PowerCC23X1_hfxtCompEnabled == true)
    {
        PowerCC23X1_hfxtCompEnabled = false;

        Temperature_unregisterNotify(&PowerCC23X1_hfxtCompNotifyObj);

        /* Update HFTRACKCTL.RATIO to reset-value */
        PowerCC23X1_updateHFXTRatio(CKMD_HFTRACKCTL_RATIO_REF48M);
    }
    HwiP_restore(key);

    Log_printf(LogModule_Power,
               Log_INFO,
               "PowerLPF3_disableHFXTCompensation: Compensation disabled");
}

/*
 *  ======== PowerLPF3_forceHFXTCompensationUpdate ========
 */
void PowerLPF3_forceHFXTCompensationUpdate(void)
{
    PowerCC23X1_updateHFXTRatio(hfxtCompRatio);
}

/*
 *  ======== PowerLPF3_getHFXTCompensationRatio ========
 */
uint32_t PowerLPF3_getHFXTCompensationRatio(void)
{
    return (HWREG(CKMD_BASE + CKMD_O_HFTRACKCTL) & CKMD_HFTRACKCTL_RATIO_M) >> CKMD_HFTRACKCTL_RATIO_S;
}

/*
 *  ======== PowerLPF3_startInitialHfxtAmpComp ========
 * Function to start the initial HFXT amplitude compensation.
 * If initial HFXT compensation is to be performed, this function is to be
 * called in the interrupt handler for the AMPSETTLED interrupt instead of
 * posting a PowerLPF3_HFXT_AVAILABLE notification and releasing the
 * PowerLPF3_DISALLOW_STANDBY constraint.
 *
 * The initial HFXT amplitude compensation assumes that the HFXTTARG.IREF value
 * is set to the max value (HFXT_TARGET_IREF_MAX) when this function is called.
 * The IREF value will then be updated in steps until the optimal IREF
 * value has been found. This is the steps taken:
 * 1. Wait HFXT_AMP_COMP_MEASUREMENT_US us, to let the HFXT amplitude settle
 *    before measuring it.
 * 2. Measure the HFXT amplitude.
 * 3. If amplitude is above 16, decrement IREF, but never lower than
 *    HFXT_TARGET_IREF_MIN.
 * 4. If an adjustment was made, go to step 1.
 * 5. If amplitude is below 10, increment IREF back to previous value. No new
 *    measurement is required.
 * 6. If an adjustment was made, wait HFXT_AMP_COMP_MEASUREMENT_US us, to let
 *    the HFXT amplitude settle.
 * 7. Post PowerLPF3_HFXT_AVAILABLE notification and release
 *    PowerLPF3_DISALLOW_STANDBY constraint.
 *
 * The waits mentioned in step 1 and 6 will be done asynchronously using
 * hfxtAmpCompClock, and most of above logic is handled in the callback function
 * for the clock (PowerCC23X1_initialHfxtAmpCompClockCb).
 *
 * This function is only responsible for starting the process by starting the
 * clock to perform the first iteration of step 1.
 *
 * In the worst case (in terms of how long the process takes), the IREF value
 * will be decremented to HFXT_TARGET_IREF_MIN and then incremented back to
 * (HFXT_TARGET_IREF_MIN + 1). This means that the IREF will be changed
 * ((HFXT_TARGET_IREF_MAX - HFXT_TARGET_IREF_MIN) + 1) times. After each change
 * we need to wait for HFXT_AMP_COMP_MEASUREMENT_US, and including waiting
 * before the first measurement it means that the initial HFXT amplitude
 * compensation will take up to
 * ((HFXT_TARGET_IREF_MAX - HFXT_TARGET_IREF_MIN) + 2)*HFXT_AMP_COMP_MEASUREMENT_US us
 * plus aditional processing time.
 */
void PowerLPF3_startInitialHfxtAmpComp(void)
{
    /* Update hfxtAmpCompClock callback function */
    ClockP_setFunc(&hfxtAmpCompClock, PowerCC23X1_initialHfxtAmpCompClockCb, 0);

    /* Set timeout for the first HFXT amplitude measurement */
    ClockP_setTimeout(&hfxtAmpCompClock, HFXT_AMP_COMP_MEASUREMENT_US / ClockP_getSystemTickPeriod());

    /* Start clock */
    ClockP_start(&hfxtAmpCompClock);
}

/*
 *  ======== PowerLPF3_getHfxtAmpAdjustment ========
 */
int_fast8_t PowerLPF3_getHfxtAmpAdjustment(void)
{
    uint32_t hfxtAmplitude   = PowerCC23X1_getHfxtAmpMeasurement();
    uint32_t currentIrefTrim = CKMDGetTargetIrefTrim();

    if (hfxtAmplitude < 10 && currentIrefTrim < HFXT_TARGET_IREF_MAX)
    {
        /* Increase amplitude if the amplitude is too small */
        return +1;
    }
    else if (hfxtAmplitude > 16 && currentIrefTrim > HFXT_TARGET_IREF_MIN)
    {
        /* Decrease the amplitude if the amplitude is too big */
        return -1;
    }
    else
    {
        /* No adjustment is needed */
        return 0;
    }
}

/*
 *  ======== PowerLPF3_adjustHfxtAmp ========
 */
void PowerLPF3_adjustHfxtAmp(int_fast8_t adjustment)
{
    uint32_t newTargetIref = CKMDGetTargetIrefTrim() + adjustment;
    CKMDSetTargetIrefTrim(newTargetIref);
}

/*
 *  ======== PowerCC23X1_initialHfxtAmpCompClockCb ========
 * Callback function for the hfxtAmpCompClock object when it is used for the
 * initial HFXT amplitude compensation.
 *
 * params:
 *  - searchDone: If true (non-zero), the search for the optimal IREF value has
 *                already been found.
 */
static void PowerCC23X1_initialHfxtAmpCompClockCb(uintptr_t searchDone)
{

    /* Has the optimal IREF value been found?
     * Might be updated below.
     */
    bool optimalIref = (bool)searchDone;

    /* Has the IREF value been changed?
     * Will be updated below if the IREF value is changed.
     */
    bool irefChanged = false;

    /* Continue search if the optimal IREF has not yet been found */
    if (optimalIref == false)
    {
        /* Measure amplitude */
        uint32_t hfxtAmplitude = PowerCC23X1_getHfxtAmpMeasurement();

        /* Get current IREF value */
        uint32_t currentIrefTrim = CKMDGetTargetIrefTrim();

        /* Determine if IREF needs to be changed.
         * Note, PowerLPF3_getHfxtAmpAdjustment() is not used here, because the
         * amplitude limits used here are different.
         */
        if (hfxtAmplitude < 10 && currentIrefTrim < HFXT_TARGET_IREF_MAX)
        {
            /* Increase amplitude if the amplitude is too small */
            CKMDSetTargetIrefTrim(currentIrefTrim + 1);
            irefChanged = true;

            /* No new measurement is required, because we know
             * that IREF = currentIrefTrim + 1 causes an amplitude above 10,
             * otherwise IREF would not have been decreased.
             */
            optimalIref = true;
        }
        else if (hfxtAmplitude > 16 && currentIrefTrim > HFXT_TARGET_IREF_MIN)
        {
            /* Decrease amplitude if the amplitude is too big */
            CKMDSetTargetIrefTrim(currentIrefTrim - 1);
            irefChanged = true;
        }
        else
        {
            /* IREF is optimal since since the HFXT amplitude is already in
             * the optimal range (10-16)
             */
            optimalIref = true;
        }
    }

    if (irefChanged)
    {
        /* If IREF has changed, start the clock again. If the optimal IREF
         * value has been found, then the search is done, so in the next clock
         * callback no measurements are needed (searchDone is true).
         */
        ClockP_setFunc(&hfxtAmpCompClock, PowerCC23X1_initialHfxtAmpCompClockCb, optimalIref);

        /* Set timeout */
        ClockP_setTimeout(&hfxtAmpCompClock, HFXT_AMP_COMP_MEASUREMENT_US / ClockP_getSystemTickPeriod());

        /* Start clock */
        ClockP_start(&hfxtAmpCompClock);
    }
    else
    {
        /* Update hfxtAmpCompClock callback function (NULL because the clock is
         * no longer used)
         */
        ClockP_setFunc(&hfxtAmpCompClock, NULL, 0);

        Log_printf(LogModule_Power, Log_INFO, "PowerCC23X1_initialHfxtAmpCompClockCb: HFXT is available");

        /* Send HFXT available notification */
        PowerCC23X1_notify(PowerLPF3_HFXT_AVAILABLE);

        /* Allow standby again now that the optimal IREF value has been found */
        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    }
}

/*
 *  ======== PowerCC23X1_hfxtAmpsettledTimeout ========
 * Callback function for the hfxtAmpCompClock object when it is used as a
 * timeout for the AMPSETTLED signal. If the timeout occurs (i.e. if this
 * function is called), and if the HFXT FSM is stuck in the RAMP0 state,
 * it will be forced to the RAMP1 state.
 */
static void PowerCC23X1_hfxtAmpsettledTimeout(uintptr_t arg)
{
    /* Disable interrupts */
    uintptr_t key = HwiP_disable();

    /* Timeout occured while waiting for AMPSETTLED */
    if ((HWREG(CKMD_BASE + CKMD_O_AMPSTAT) & CKMD_AMPSTAT_STATE_M) == CKMD_AMPSTAT_STATE_RAMP0)
    {
        PowerCC23X1_forceHfxtFsmToRamp1();
    }

    HwiP_restore(key);
}

/*
 *  ======== PowerCC23X1_forceHfxtFsmToRamp1 ========
 * Force HFXT FSM to RAMP1 state. This function should only be called if the
 * HFXT FSM is stuck in RAMP0 state.
 */
static void PowerCC23X1_forceHfxtFsmToRamp1(void)
{
    /* Max out IREF */
    CKMDSetTargetIrefTrim(HFXT_TARGET_IREF_MAX);

    /* Force a transition to RAMP1 state */
    HWREG(CKMD_BASE + CKMD_O_AMPADCCTL) = (CKMD_AMPADCCTL_SWOVR | CKMD_AMPADCCTL_SRCSEL_PEAK |
                                           CKMD_AMPADCCTL_ADCEN_ENABLE | CKMD_AMPADCCTL_COMPSTRT);

    /* Wait until state changes */
    while ((HWREG(CKMD_BASE + CKMD_O_AMPSTAT) & CKMD_AMPSTAT_STATE_M) == CKMD_AMPSTAT_STATE_RAMP0) {}

    /* Disable AMPADCCTL.SWOVR and let the FSM take control again */
    HWREG(CKMD_BASE + CKMD_O_AMPADCCTL) &= ~CKMD_AMPADCCTL_SWOVR;
}

/*
 *  ======== PowerCC23X1_startContHfxtAmpMeasurements ========
 * Start continuous measurements of the HFXT amplitude.
 * This is done to quickly be able to read the amplitude using
 * PowerCC23X1_getHfxtAmpMeasurement() when a measurement is needed.
 */
static void PowerCC23X1_startContHfxtAmpMeasurements(void)
{
    /* Force amplitude measurement - Set SRCSEL = PEAK */
    HWREG(CKMD_BASE + CKMD_O_AMPADCCTL) = (CKMD_AMPADCCTL_SWOVR | CKMD_AMPADCCTL_PEAKDETEN_ENABLE |
                                           CKMD_AMPADCCTL_ADCEN_ENABLE | CKMD_AMPADCCTL_SRCSEL_PEAK |
                                           CKMD_AMPADCCTL_SARSTRT);
}

/*
 *  ======== PowerCC23X1_stopContHfxtAmpMeasurements ========
 * Stop the continuous HFXT amplitude measurements started by
 * PowerCC23X1_startContHfxtAmpMeasurements().
 */
static void PowerCC23X1_stopContHfxtAmpMeasurements(void)
{
    /* Stop SAR conversions before releasing SWOVR. SWOVR must not be cleared
     * while the ADC is in SETUP state: during SETUP, MUX_SEL_HW points to
     * BIAS, so releasing SWOVR then causes BIAS=0 to be read. With BIAS=0,
     * amplitude (peakRaw - DC_bias) is overestimated, Ramp 0 exits too early,
     * and the HFXT fails qualification.
     */
    HWREG(CKMD_BASE + CKMD_O_AMPADCCTL) &= ~CKMD_AMPADCCTL_SARSTRT;

    /* Wait for any in-progress SETUP state to complete before clearing SWOVR.
     * This takes 1 clock cycle in the worst case. With FSM update rate set to 2
     * MHz (CKMD:AMPCFG0:FSMRATE), that's 0.5 us minimum. 1us is used because
     * HapiWaitUs is used for the delay, which has a 1 us resolution.
     */
    HapiWaitUs(1);

    /* Transfer ADC control back to HW to prepare for next HFXT wakeup DC bias
     * measurement.
     */
    HWREG(CKMD_BASE + CKMD_O_AMPADCCTL) &= ~CKMD_AMPADCCTL_SWOVR;
}

/*
 *  ======== PowerCC23X1_getHfxtAmpMeasurement ========
 * Read the latest HFXT amplitude measurement.
 * Continuous measurements must have been started using
 * PowerCC23X1_startContHfxtAmpMeasurements().
 */
static uint32_t PowerCC23X1_getHfxtAmpMeasurement(void)
{
    /* Read result in AMPADCSTAT */
    uint32_t ampAdcStat = HWREG(CKMD_BASE + CKMD_O_AMPADCSTAT);
    uint32_t peakRaw    = (ampAdcStat & CKMD_AMPADCSTAT_PEAKRAW_M) >> CKMD_AMPADCSTAT_PEAKRAW_S;
    uint32_t bias       = (ampAdcStat & CKMD_AMPADCSTAT_BIAS_M) >> CKMD_AMPADCSTAT_BIAS_S;

    /* Compute the PEAK value in SW to be able to handle negative values, which
     * can occur for small amplitudes.
     * In case of negative values, the result will be capped at 0.
     * According to the register descriptions PEAK = 2*PEAKRAW - BIAS
     */
    uint32_t result = 2 * peakRaw > bias ? 2 * peakRaw - bias : 0;

    return result;
}

/*
 *  ======== PowerLPF3_sleep ========
 */
int_fast16_t PowerLPF3_sleep(uint32_t nextEventTimeUs)
{
    uint32_t sysTimerIMASK;
    uint32_t sysTimerARMSET;
    uint32_t sysTimerTimeouts[SYSTIMER_CHANNEL_COUNT];
    int_fast16_t result = Power_SOK;

    /* Disable SysTick */
    SysTickDisable();

    /* Save SysTimer IMASK to restore afterwards */
    sysTimerIMASK = HWREG(SYSTIM_BASE + SYSTIM_O_IMASK);

    /* Get current armed status of all SysTimer channels */
    sysTimerARMSET = HWREG(SYSTIM_BASE + SYSTIM_O_ARMSET);

    /* Store SysTimer timeouts */
    memcpy(sysTimerTimeouts, (void *)(SYSTIM_BASE + SYSTIM_O_CH0CCSR), sizeof(sysTimerTimeouts));

    /* Switch CPUIRQ16 in event fabric to RTC.
     * Since the CC23X1 only has limited interrupt lines, we need to switch the
     * interrupt line from SysTimer to RTC in the event fabric. The triggered
     * interrupt will wake up the device with interrupts disabled. We can
     * consume that interrupt event without vectoring to the ISR and then change
     * the event fabric signal back to the SysTimer. Thus, there is no need to
     * swap out the actual interrupt function of the clockHwi.
     */
    EVTSVTConfigureEvent(EVTSVT_SUB_CPUIRQ16, EVTSVT_PUB_AON_RTC_COMB);

    /* Clear interrupt in case it triggered since we disabled interrupts */
    HwiP_clearInterrupt(INT_CPUIRQ16);

    /* Ensure the device wakes up early enough to reinitialise the
     * HW and take care of housekeeping.
     */
    nextEventTimeUs -= PowerCC23X1_WAKEDELAYSTANDBY;

    /* RTC channel 0 compare is automatically armed upon writing the
     * compare value. It will automatically be disarmed when it
     * triggers.
     */
    HWREG(RTC_BASE + RTC_O_CH0CC1U) = nextEventTimeUs;

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
    while ((HWREG(SYSTIM_BASE + SYSTIM_O_STATUS) & (SYSTIM_STATUS_VAL_M | SYSTIM_STATUS_SYNCUP_M)) !=
            SYSTIM_STATUS_VAL_RUN)
    {}

    /* Restore SysTimer timeouts */
    memcpy((void *)(SYSTIM_BASE + SYSTIM_O_CH0CCSR), sysTimerTimeouts, sizeof(sysTimerTimeouts));

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
    PowerCC23X1_notify(PowerLPF3_AWAKE_STANDBY);

    return result;
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
     * need to wait until the next LFTICK once XBHFGOOD is set before going to
     * sleep. This is achieved by clearing the LFTICK interrupt once AMPSETTLED
     * is set (which occurs after XBHFGOOD), and then check against LFTICK
     * before entering standby.
     */
    if ((HWREG(CKMD_BASE + CKMD_O_LFCLKSEL) & CKMD_LFCLKSEL_MAIN_M) == CKMD_LFCLKSEL_MAIN_LFOSC)
    {
        result = ((HWREG(CKMD_BASE + CKMD_O_RIS) & CKMD_RIS_LFTICK_M) == CKMD_RIS_LFTICK);
    }

    return result;
}
