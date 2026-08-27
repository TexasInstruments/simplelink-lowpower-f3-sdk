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
 *  ======== PowerCC283X.c ========
 */

#include <stdbool.h>
#include <string.h>

#include <ti/drivers/dpl/DebugP.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC283X.h>

#include <ti/log/Log.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/lrfd.h)
#include DeviceFamily_constructPath(driverlib/prcm.h)
#include DeviceFamily_constructPath(driverlib/systimer.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_prcmull.h)
#include DeviceFamily_constructPath(cmsis/core/cmsis_compiler.h)

/* Type definitions */

/* Forward declarations */
int_fast16_t PowerCC283X_notify(uint_fast16_t eventType);
static void PowerCC283X_setDependencyCount(Power_Resource resourceId, uint8_t count);
bool PowerCC283X_isValidResourceId(Power_Resource resourceId);

/* Externs */
extern const PowerCC283X_Config PowerCC283X_config;

/* Macro for weak definition of the Power Log module */
Log_MODULE_DEFINE_WEAK(LogModule_Power, Log_MODULE_INIT_SINK_DUMMY);

/* Function Macros */

/* Macro used to extract the resource group from a resource ID */
#define RESOURCE_GROUP(resourceId) ((resourceId)&PowerCC283X_PERIPH_GROUP_M)

/* Macro used to extract the index for the given group from a resource ID */
#define RESOURCE_INDEX(resourceId) ((resourceId)&PowerCC283X_PERIPH_INDEX_M)

/* Static Globals */

/* Array to maintain constraint reference counts.
 * Declare volatile variable to ensure the toolchain does not use
 * stack for the variable and does not optimize this memory allocation away.
 */
static volatile uint8_t constraintCounts[PowerCC283X_NUMCONSTRAINTS];

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
static volatile uint8_t resourceCountsPrcm[PowerCC283X_NUMRESOURCES_PRCM];
static volatile uint8_t resourceCountsLrfd[PowerCC283X_NUMRESOURCES_LRFD];

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

    isPolicyEnabled = (PowerCC283X_config.policyFxn != NULL);

    policyFxn = PowerCC283X_config.policyFxn;

    /* Update Systimer from Initialize to Ready State. */
    HWREG(SYSTIM_BASE + SYSTIM_O_CFG)= SYSTIM_CFG_EN;

    /* Construct the ClockP hwi responsible for timing service events.
     * ClockP will configure the ISR function when it is initialized. So NULL
     * is used here, since it is not expected to be called before ClockP is
     * initialized.
     */
    HwiP_construct(&clockHwi, INT_CPUIRQ5, NULL, NULL);

    /* Make SysTimer halt on CPU debug halt */
    SysTimerSetDebugConfig(SYSTIMER_DEBUG_CONFIG_HALT_STOP);

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
    DebugP_assert(PowerCC283X_isValidResourceId(resourceId));

    int_fast16_t result;
    uint8_t index       = RESOURCE_INDEX(resourceId);
    uint_fast16_t group = RESOURCE_GROUP(resourceId);

    if (group == PowerCC283X_PERIPH_GROUP_PRCM)
    {
        result = (int_fast16_t)resourceCountsPrcm[index];
    }
    else if (group == PowerCC283X_PERIPH_GROUP_LRFD)
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
    DebugP_assert(constraintId < PowerCC283X_NUMCONSTRAINTS);

    int_fast16_t result = Power_EINVALIDINPUT;

    if (constraintId < PowerCC283X_NUMCONSTRAINTS)
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
    /* TODO: Make this a dynamic system based on the dynamically computed delta
     * between RTC timeout and re-enabling interrupts
     */

    uint_fast32_t latency = 0;

    if (type == Power_RESUME)
    {
        if (sleepState == PowerLPF4_STANDBY)
        {
            latency = PowerCC283X_RESUMETIMESTANDBY;
        }
    }
    else
    {
        if (sleepState == PowerLPF4_STANDBY)
        {
            latency = PowerCC283X_TOTALTIMESTANDBY;
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

    DebugP_assert(constraintId < PowerCC283X_NUMCONSTRAINTS);

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

    DebugP_assert(constraintId < PowerCC283X_NUMCONSTRAINTS);

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

    DebugP_assert(PowerCC283X_isValidResourceId(resourceId));

    key = HwiP_disable();

    /* Buffer previous reference count */
    previousCount = Power_getDependencyCount(resourceId);

    /* Increment reference count */
    PowerCC283X_setDependencyCount(resourceId, previousCount + 1);

    /* If the resource was NOT activated previously ... */
    if (previousCount == 0)
    {
        index = RESOURCE_INDEX(resourceId);
        /* Turn on the peripheral */
        switch (RESOURCE_GROUP(resourceId))
        {
            case PowerCC283X_PERIPH_GROUP_PRCM:
                PRCMEnableClock(index);
                break;
            case PowerCC283X_PERIPH_GROUP_LRFD:
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

    DebugP_assert(PowerCC283X_isValidResourceId(resourceId));

    key = HwiP_disable();

    resourceCount = Power_getDependencyCount(resourceId);

    DebugP_assert(resourceCount != 0);

    /* Decrement the reference count */
    resourceCount--;
    PowerCC283X_setDependencyCount(resourceId, resourceCount);

    /* If this was the last dependency being released.. */
    if (resourceCount == 0)
    {
        index = RESOURCE_INDEX(resourceId);
        /* Turn off the peripheral */
        switch (RESOURCE_GROUP(resourceId))
        {
            case PowerCC283X_PERIPH_GROUP_PRCM:
                PRCMDisableClock(index);
                break;
            case PowerCC283X_PERIPH_GROUP_LRFD:
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
    /* TODO: Implement function. */
    /* If we get here, failed to shutdown, return error code */
    return Power_EFAIL;
}

/*
 *  ======== Power_sleep ========
 */
int_fast16_t Power_sleep(uint_fast16_t sleepState)
{
    /* TODO: Implement function. */
    return Power_EFAIL;
}

/*
 *  ======== Power_reset ========
 */
void Power_reset(void)
{
    /* TODO: Implement function. */
    while (1) {}
}

/*
 *  ======== PowerCC283X_doWFI ========
 */
void PowerCC283X_doWFI(void)
{
    uint32_t constraints;
    bool idleAllowed;
    bool flashNotNeeded;

    constraints    = Power_getConstraintMask();
    idleAllowed    = (constraints & (1 << PowerLPF4_DISALLOW_IDLE)) == 0;
    flashNotNeeded = (constraints & (1 << PowerLPF4_NEED_FLASH_IN_IDLE)) == 0U;

    if (idleAllowed)
    {

        /* Configure device to turn on/off flash LDO when in idle */
        if (flashNotNeeded)
        {
            /* FlashLdo can be turned off when in IDLE */
            PRCMEnableFlashLdoOffInIdle();
        }
        else
        {
            /* FlashLdo cannot be turned off when in IDLE */
            PRCMDisableFlashLdoOffInIdle();
        }

        /* Enter idle */
        __WFI();
    }
}

/*
 *  ======== PowerLPF4_getResetReason ========
 */
PowerLPF4_ResetReason PowerLPF4_getResetReason(void)
{
    /* TODO: Implement function. */
    return PowerLPF4_RESET_UNKNOWN;
}

/*
 *  ======== PowerLPF4_releaseLatches ========
 */
void PowerLPF4_releaseLatches(void)
{
    /* TODO: Implement function. */
    while (1) {}
}

/*
 *  ======== PowerLPF4_selectLFOSC ========
 */
void PowerLPF4_selectLFOSC(void)
{
    /* TODO: Implement function. */
    while (1) {}
}

/*
 *  ======== PowerLPF4_selectLFXT ========
 */
void PowerLPF4_selectLFXT(void)
{
    /* TODO: Implement function. */
    while (1) {}
}

/*
 *  ======== PowerLPF4_selectEXTLF ========
 */
void PowerLPF4_selectEXTLF(void)
{
    /* TODO: Implement function. */
    while (1) {}
}

/*
 *  ======== PowerCC283X_notify ========
 *  Send notifications to registered clients.
 *  Note: Task scheduling is disabled when this function is called.
 */
int_fast16_t PowerCC283X_notify(uint_fast16_t eventType)
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

        /* TODO: Change Power_registerNotify() & Power_unregisterNotify() to use
         * index instead of bitmask and then use multiple lists just like in
         * the TI-RTOS scheduler.
         * We currently walk the entire list 4 times when entering/exiting
         * standby + switching HFXT...
         */

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
                           "PowerCC283X_notify: Invoking notification fxn at address 0x%x with event type 0x%x and clientArg 0x%x",
                           notifyFxn,
                           eventType,
                           clientArg);

                notifyStatus = (int_fast16_t)(*(Power_NotifyFxn)notifyFxn)(eventType, 0, clientArg);

                /* If client declared error stop all further notifications */
                if (notifyStatus != Power_NOTIFYDONE)
                {
                    Log_printf(LogModule_Power,
                               Log_WARNING,
                               "PowerCC283X_notify: Notification fxn reported error, fxn at address 0x%x with event type 0x%x and notifyStatus 0x%x",
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
 *  ======== PowerCC283X_setDependencyCount ========
 */
static void PowerCC283X_setDependencyCount(Power_Resource resourceId, uint8_t count)
{
    uint8_t index       = RESOURCE_INDEX(resourceId);
    uint_fast16_t group = RESOURCE_GROUP(resourceId);

    DebugP_assert(PowerCC283X_isValidResourceId(resourceId));

    if (group == PowerCC283X_PERIPH_GROUP_PRCM)
    {
        resourceCountsPrcm[index] = count;
    }
    else if (group == PowerCC283X_PERIPH_GROUP_LRFD)
    {
        resourceCountsLrfd[index] = count;
    }
}

/*
 *  ======== PowerCC283X_isValidResourceId ========
 */
bool PowerCC283X_isValidResourceId(Power_Resource resourceId)
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
            case PowerCC283X_PERIPH_GROUP_PRCM:
                result = index < PowerCC283X_NUMRESOURCES_PRCM;
                break;
            case PowerCC283X_PERIPH_GROUP_LRFD:
                result = index < PowerCC283X_NUMRESOURCES_LRFD;
                break;
            default:
                result = false;
                break;
        }
    }

    return result;
}

/*
 *  ======== PowerLPF4_sleep ========
 */
int_fast16_t PowerLPF4_sleep(uint32_t nextEventTimeUs)
{
    /* TODO: Implement function. */
    return Power_EFAIL;
}