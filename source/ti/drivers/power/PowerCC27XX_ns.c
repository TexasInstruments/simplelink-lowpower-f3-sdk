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
 *  ======== PowerCC27XX_ns.c ========
 */

#include <stdbool.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/DebugP.h>

#include <ti/drivers/ITM.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC27XX.h>
#include <ti/drivers/power/PowerCC27XX_veneers.h>
#include <ti/drivers/tfm/SecureCallback.h>

#include <ti/log/Log.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_ckmd.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

/* Forward declarations */
int_fast16_t PowerCC27XX_notify(uint_fast16_t eventType);
bool PowerCC27XX_isValidResourceId(Power_Resource resourceId);
static void PowerCC27XX_rtcISR(uintptr_t arg);
static void PowerCC27XX_notifySecureCallbackFxn(uintptr_t arg);

/* Externs */
extern const PowerCC27XX_Config PowerCC27XX_config;

/* Macro for weak definition of the Power Log module */
Log_MODULE_DEFINE_WEAK(LogModule_Power, {0});

/* Function Macros */
/* Macro used to extract the resource group  from a resource ID */
#define RESOURCE_GROUP(resourceId) ((resourceId)&PowerCC27XX_PERIPH_GROUP_M)

/* Macro used to extract the bit index shift encoded from a resource ID */
#define RESOURCE_BIT_INDEX(resourceId) ((resourceId)&PowerCC27XX_PERIPH_BIT_INDEX_M)

/* Type definitions */

/* Static Globals */

/* Array to maintain non-secure constraint reference counts.
 * Declare volatile variable to ensure the toolchain does not use
 * stack for the variable and does not optimize this memory allocation away.
 */
static volatile uint8_t constraintCounts_ns[PowerCC27XX_NUMCONSTRAINTS];

/* Mask of non-secure Power constraints for quick access.
 * Declare volatile variable to ensure the toolchain does not use
 * stack for the variable and does not optimize this memory allocation away.
 */
static volatile uint32_t constraintMask_ns = 0;

/* Keeps track of the configured Power policy. Power_idleFunc() will not run
 * the policy if this is set to NULL
 */
static Power_PolicyFxn policyFxn = NULL;

/* Is the Power policy enabled? */
static bool isPolicyEnabled = false;

/* Has the Power driver been initialized */
static bool isInitialized = false;

/* Non-secure event notification list */
static List_List notifyList_ns;

/* SecureCallback object for receiving Power notifications */
static SecureCallback_Object notifySecureCallbackObj;

/* Non-static Globals */

/* Interrupt for ClockP and Power policy */
HwiP_Struct clockHwi;

/* ****************** Power APIs ******************** */

/*
 *  ======== Power_init ========
 */
int_fast16_t Power_init()
{
    SecureCallback_Handle scbHandle;

    /* If this function has already been called, just return */
    if (isInitialized)
    {
        return Power_SOK;
    }

    isInitialized = true;

    isPolicyEnabled = (PowerCC27XX_config.policyFxn != NULL);

    policyFxn = PowerCC27XX_config.policyFxn;

    /* Tell the secure Power driver where to read the non-secure constraints */
    PowerCC27XX_setNonSecureConstraintPointers_veneer(&constraintMask_ns, &constraintCounts_ns);

    /* Construct the ClockP hwi responsible for timing service events.
     * This Hwi is time multiplexed between the SysTimer and the RTC for use
     * by the ClockP and Power policy respectively.
     * Since there is no dedicated RTC or SysTimer interrupt line, we need to
     * mux one of the configurable lines to the CKM.
     * CPUIRQ16 is dedicated to this purpose.
     */
    HwiP_construct(&clockHwi, INT_CPUIRQ16, PowerCC27XX_rtcISR, NULL);

    /* Initialize SecureCB driver */
    SecureCallback_init();

    /* Construct the SecureCallback for getting callbacks from the secure side */
    scbHandle = SecureCallback_construct(&notifySecureCallbackObj, PowerCC27XX_notifySecureCallbackFxn, 0);

    /* Tell the secure Power driver about the SecureCallback handle.
     * The veneer panics if the handle is invalid.
     */
    PowerCC27XX_setNonSecureNotificationHandle_veneer(scbHandle);

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
    return (uint_fast32_t)constraintMask_ns;
}

/*
 *  ======== Power_getDependencyCount ========
 *  Get the count of dependencies that are currently declared upon a resource.
 */
int_fast16_t Power_getDependencyCount(Power_Resource resourceId)
{
    return Power_getDependencyCount_veneer(resourceId);
}

/*
 *  ======== Power_getConstraintCount ========
 *  Get the count of constraints that are currently set on a certain
 *  operational transition
 */
int_fast16_t Power_getConstraintCount(uint_fast16_t constraintId)
{
    int_fast16_t result = Power_EINVALIDINPUT;

    if (constraintId < PowerCC27XX_NUMCONSTRAINTS)
    {
        result = (int_fast16_t)constraintCounts_ns[constraintId];
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
    /* TODO: Make this a dynamic system based on the dynamically computed delta
     * between RTC timeout and re-enabling interrupts
     */

    uint32_t latency = 0;

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
                   "Power_registerNotify (NS): Notify registration failed due to NULL pointer");

        status = Power_EINVALIDPOINTER;
    }
    else
    {
        notifyObj->eventTypes = eventTypes;
        notifyObj->notifyFxn  = notifyFxn;
        notifyObj->clientArg  = clientArg;

        Log_printf(LogModule_Power,
                   Log_INFO,
                   "Power_registerNotify (NS): Register fxn at address 0x%x with event types 0x%x and clientArg 0x%x",
                   notifyFxn,
                   eventTypes,
                   clientArg);

        /* Place notify object on event notification queue. Assume that
         * List_Elem struct member is the first struct member in
         * Power_NotifyObj.
         */
        List_put(&notifyList_ns, (List_Elem *)notifyObj);
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
               "Power_unregisterNotify (NS): Unregister fxn at address 0x%x with event types 0x%x and clientArg 0x%x",
               notifyObj->notifyFxn,
               notifyObj->eventTypes,
               notifyObj->clientArg);

    /* Remove notify object from its event queue */
    List_remove(&notifyList_ns, (List_Elem *)notifyObj);
}

/*
 *  ======== Power_setConstraint ========
 *  Declare an operational constraint.
 */
int_fast16_t Power_setConstraint(uint_fast16_t constraintId)
{
    uintptr_t key;

    DebugP_assert(constraintId < PowerCC27XX_NUMCONSTRAINTS);

    key = HwiP_disable();

    /* Set the specified constraint in the constraintMask for faster access */
    constraintMask_ns |= 1U << constraintId;

    /* Increment the specified constraint count */
    constraintCounts_ns[constraintId]++;

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

    DebugP_assert(constraintId < PowerCC27XX_NUMCONSTRAINTS);

    key = HwiP_disable();

    DebugP_assert(constraintCounts_ns[constraintId] != 0U);

    constraintCounts_ns[constraintId]--;

    /* Only update the constraint mask if we removed the constraint entirely */
    if (constraintCounts_ns[constraintId] == 0U)
    {
        constraintMask_ns &= ~(1U << constraintId);
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
    return Power_setDependency_veneer(resourceId);
}

/*
 *  ======== Power_releaseDependency ========
 *  Release a previously declared dependency.
 */
int_fast16_t Power_releaseDependency(Power_Resource resourceId)
{
    return Power_releaseDependency_veneer(resourceId);
}

/*
 *  ======== Power_shutdown ========
 */
int_fast16_t Power_shutdown(uint_fast16_t shutdownState, uint_fast32_t shutdownTime)
{
    return Power_shutdown_veneer(shutdownState, shutdownTime);
}

/*
 *  ======== Power_reset ========
 */
void Power_reset(void)
{
    Power_reset_veneer();
}

/*
 *  ======== PowerCC27XX_doWFI ========
 */
void PowerCC27XX_doWFI(void)
{
    /* Flush any remaining log messages in the ITM */
    ITM_flush();

    /* Call secure PowerCC27XX_doWFI() function such that it can respect the
     * PowerLPF3_DISALLOW_IDLE constraint.
     */
    PowerCC27XX_doWFI_veneer();

    /* Restore ITM settings */
    ITM_restore();
}

/*
 *  ======== PowerLPF3_getResetReason ========
 */
PowerLPF3_ResetReason PowerLPF3_getResetReason(void)
{
    return PowerLPF3_getResetReason_veneer();
}

/*
 *  ======== PowerLPF3_releaseLatches ========
 */
void PowerLPF3_releaseLatches(void)
{
    PowerLPF3_releaseLatches_veneer();
}

/*
 *  ======== PowerCC27XX_rtcISR ========
 */
static void PowerCC27XX_rtcISR(uintptr_t arg)
{
    /* We should never get here since we will just use the interrupt to wake
     * up the device from standby and then consume the interrupt while
     * interrupts are disabled.
     */
    while (1) {}
}

/*
 *  ======== PowerCC27XX_notifySecureCallbackFxn ========
 */
static void PowerCC27XX_notifySecureCallbackFxn(uintptr_t arg)
{
    uint_fast16_t eventsMask = (uint_fast16_t)arg;

    if (eventsMask)
    {
        /*
         * Special case: Synchronously handled event.
         */

        /* Only a single event, so we can post immediately. */
        if (eventsMask < (1 << PowerCC27XX_NUMEVENTS))
        {
            PowerCC27XX_notify(eventsMask);
        }
    }
    else
    {
        /*
         * Normal case: Asynchronously handled events
         */

        /* Get pending events from the secure side */
        eventsMask = PowerCC27XX_getCurrentEventsMask_veneer();

        /* There will always be at least 1 event, so no checking for null */

        /* Notify subscribers of each pending event */
        for (uint32_t event = 1; event < (1 << PowerCC27XX_NUMEVENTS); event <<= 1)
        {
            if (event & eventsMask)
            {
                PowerCC27XX_notify(event);
            }
        }
    }
}

/*
 *  ======== PowerLPF3_startAFOSC ========
 */
int_fast16_t PowerLPF3_startAFOSC(PowerLPF3_AfoscFreq frequency)
{
    return PowerLPF3_startAFOSC_veneer(frequency);
}

/*
 *  ======== PowerLPF3_stopAFOSC ========
 */
void PowerLPF3_stopAFOSC(void)
{
    PowerLPF3_stopAFOSC_veneer();
}

/*
 *  ======== PowerCC27XX_isValidResourceId ========
 */
bool PowerCC27XX_isValidResourceId(Power_Resource resourceId)
{
    uint8_t bitIndex    = RESOURCE_BIT_INDEX(resourceId);
    uint_fast16_t group = RESOURCE_GROUP(resourceId);

    if (group == PowerCC27XX_PERIPH_GROUP_CLKCTL0)
    {
        return bitIndex < PowerCC27XX_NUMRESOURCES_CLKCTL0;
    }
    else if (group == PowerCC27XX_PERIPH_GROUP_CLKCTL1)
    {
        return bitIndex < PowerCC27XX_NUMRESOURCES_CLKCTL1;
    }
    else if (group == PowerCC27XX_PERIPH_GROUP_LRFD)
    {
        return bitIndex < PowerCC27XX_NUMRESOURCES_LRFD;
    }
    else
    {
        return false;
    }
}

/*
 *  ======== PowerLPF3_forceHFXTCompensationUpdate ========
 */
void PowerLPF3_forceHFXTCompensationUpdate(void)
{
    /* HFXT compensation is currently not supported in TFM enabled applications
     * so just do nothing.
     */
}

/*
 *  ======== PowerLPF3_getHFXTCompensationRatio ========
 */
uint32_t PowerLPF3_getHFXTCompensationRatio(void)
{
    /* HFXT compensation is currently not supported in TFM enabled applications
     * so just return the default ratio.
     */
    return CKMD_HFTRACKCTL_RATIO_REF48M;
}

/*
 *  ======== PowerCC27XX_notify ========
 *  Send notifications to registered clients on the non-secure side.
 */
int_fast16_t PowerCC27XX_notify(uint_fast16_t eventType)
{
    int_fast16_t notifyStatus;
    Power_NotifyFxn notifyFxn;
    uintptr_t clientArg;
    List_Elem *elem;

    /* If queue is empty, return immediately */
    if (!List_empty(&notifyList_ns))
    {
        /* Point to first client notify object */
        elem = List_head(&notifyList_ns);

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
                           "PowerCC27XX_notify (NS): Invoking notification fxn at address 0x%x with event type 0x%x and clientArg 0x%x",
                           notifyFxn,
                           eventType,
                           clientArg);

                notifyStatus = (int_fast16_t)(*(Power_NotifyFxn)notifyFxn)(eventType, 0, clientArg);

                /* If client declared error stop all further notifications */
                if (notifyStatus != Power_NOTIFYDONE)
                {
                    Log_printf(LogModule_Power,
                               Log_WARNING,
                               "PowerCC27XX_notify (NS): Notification fxn reported error, fxn at address 0x%x with event type 0x%x and notifyStatus 0x%x",
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
 *  ======== PowerLPF3_sleep ========
 */
int_fast16_t PowerLPF3_sleep(uint32_t nextEventTimeUs)
{
    return PowerLPF3_sleep_veneer(nextEventTimeUs);
}

/*
 *  ======== PowerLPF3_isLfincFilterAllowingStandby ========
 */
bool PowerLPF3_isLfincFilterAllowingStandby(void)
{
    /* Always return true, since the LFINC filter will be checked on the S side
     * before entering standby.
     */
    return true;
}