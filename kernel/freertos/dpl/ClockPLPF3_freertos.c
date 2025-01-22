/*
 * Copyright (c) 2021-2024, Texas Instruments Incorporated
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
 *  ======== ClockPLPF3_freertos.c ========
 */

#include <stdlib.h>

#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/utils/List.h>

#include "ClockPLPF3.h"

#include <FreeRTOS.h>

/* Driverlib includes*/
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_evtsvt.h)
#include DeviceFamily_constructPath(inc/hw_systim.h)
#include DeviceFamily_constructPath(driverlib/interrupt.h)

/* Defines */
#if DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X0
    #define CLOCK_FREQUENCY_DIVIDER (48000000U / configCPU_CLOCK_HZ)
#elif DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX
    #define CLOCK_FREQUENCY_DIVIDER (96000000U / configCPU_CLOCK_HZ)
#endif

/** Max number of ClockP ticks into the future supported by this ClockP
 * implementation.
 *
 * Under the hood, ClockP uses the SysTimer whose events trigger immediately if
 * the compare value is less than 2^22 systimer ticks in the past
 * (4.194sec at 1us resolution). Therefore, the max number of SysTimer ticks you
 * can schedule into the future is 2^32 - 2^22 - 1 ticks (~= 4290 sec at 1us
 * resolution). */
#define ClockP_PERIOD_MAX     (0xFFBFFFFFU / ClockP_TICK_PERIOD)
/** Max number of seconds into the future supported by this ClockP
 * implementation.
 *
 * This limit affects ClockP_sleep() */
#define ClockP_PERIOD_MAX_SEC 4290U

/* Processing overhead.
 *
 * Empirically deduced processing overhead to ensure
 * ClockP_usleep() to be more accurate.
 * TODO: Revise for CC27XX devices (TIDRIVERS-5793)
 */
#define ClockP_PROC_OVERHEAD_US (99U * CLOCK_FREQUENCY_DIVIDER)

/* Get the current ClockP tick value */
#define getClockPTick() (HWREG(SYSTIM_BASE + SYSTIM_O_TIME1U) / ClockP_TICK_PERIOD)

/* The name of this struct and the names of its members are used by ROV */
typedef struct ClockP_Obj
{
    List_Elem elem;       ///< Clock's List element. Must be first in struct
    uint32_t timeout;     ///< Timeout value (used for one-shot)
    uint32_t currTimeout; ///< Next timeout value in number of tick periods
    uint32_t period;      ///< Period of periodic clock. 0 for one-shot.
    volatile bool active; ///< Clock is active
    ClockP_Fxn fxn;       ///< Callback function
    uintptr_t arg;        ///< Argument passed to callback function
} ClockP_Obj;

/* Shared variables */
/* ClockP and Power policy share interrupt CPUIRQ16, and therefore Hwi object. */
extern HwiP_Struct clockHwi;

/* Local variables */
/* The names of these variables are used by ROV */
static bool ClockP_initialized = false;

/* Upper 32 bits of the 64-bit SysTickCount */
static uint32_t upperSystemTicks64 = 0;

/* The existence of a variable with this name is the signal to ROV
 * that it is used on a CC23XX/CC27XX device
 */
static List_List ClockP_list;
static volatile uint32_t ClockP_ticks;
static uint32_t ClockP_nextScheduledTick;
static bool ClockP_inWorkFunc;
static bool ClockP_startOrStopDuringWorkFunc;
static ClockP_Params ClockP_defaultParams = {
    .startFlag = false,
    .period    = 0,
    .arg       = 0,
};

/* Function declarations */
static void ClockP_workFuncDynamic(uintptr_t arg);
static void ClockP_hwiCallback(uintptr_t arg0);
static void sleepTicks(uint32_t ticks);
static void sleepClkFxn(uintptr_t arg0);
static void ClockP_scheduleNextTick(uint32_t absTick);

/* Callback function to increment 64-bit counter on 32-bit counter overflow */
static void systemTicks64Callback(uintptr_t arg);
/*
 *  ======== ClockP_Params_init ========
 */
void ClockP_Params_init(ClockP_Params *params)
{
    /* structure copy */
    *params = ClockP_defaultParams;
}

/*
 *  ======== ClockP_startup ========
 */
void ClockP_startup(void)
{
    if (!ClockP_initialized)
    {
        uint32_t nowTick;
        intptr_t key;

        /* Get current value as early as possible */
        nowTick = getClockPTick();

        /* Clear any pending interrupts on SysTimer channel 0 */
        HWREG(SYSTIM_BASE + SYSTIM_O_ICLR) = SYSTIM_ICLR_EV0_CLR;

        /* Configure SysTimer channel 0 to compare mode with timer resolution of
         * 1 us.
         */
        HWREG(SYSTIM_BASE + SYSTIM_O_CH0CFG) = 0;

        /* Make SysTimer halt on CPU debug halt */
        HWREG(SYSTIM_BASE + SYSTIM_O_EMU) = SYSTIM_EMU_HALT_STOP;

        /* HWI clockHwi is owned by the Power driver, but multiplexed between
         * ClockP and the Power policy (this is handled by the Power driver).
         * All ClockP must do is to set the callback function and mux the
         * SysTimer channel 0 signal to CPUIRQ16.
         */
        HwiP_setFunc(&clockHwi, ClockP_hwiCallback, (uintptr_t)NULL);
        HWREG(EVTSVT_BASE + EVTSVT_O_CPUIRQ16SEL) = EVTSVT_CPUIRQ16SEL_PUBID_SYSTIM0;

        /* Set IMASK for channel 0. IMASK is used by the power driver to know
         * which systimer channels are active.
         */
        HWREG(SYSTIM_BASE + SYSTIM_O_IMSET) = SYSTIM_IMSET_EV0_SET;

        /* Initialize ClockP variables */
        List_clearList(&ClockP_list);
        ClockP_ticks                     = nowTick;
        ClockP_nextScheduledTick         = (uint32_t)(nowTick + ClockP_PERIOD_MAX);
        ClockP_inWorkFunc                = false;
        ClockP_startOrStopDuringWorkFunc = false;

        ClockP_initialized = true;

        /* Start the clock */
        key = HwiP_disable();
        ClockP_scheduleNextTick(ClockP_nextScheduledTick);
        HwiP_restore(key);
    }
}

/*
 *  ======== ClockP_getTicksUntilInterrupt  ========
 */
uint32_t ClockP_getTicksUntilInterrupt(void)
{
    uint32_t ticks;

    ticks = ClockP_nextScheduledTick - getClockPTick();

    /* Clamp value to zero if nextScheduledTick is less than current */
    if (ticks > ClockP_PERIOD_MAX)
    {
        ticks = 0;
    }

    return (ticks);
}

/*
 *  ======== ClockP_scheduleNextTick  ========
 *  Must be called with global interrupts disabled!
 */
void ClockP_scheduleNextTick(uint32_t absTick)
{
    /* Reprogram the timer for the new period and next interrupt */
    uint32_t newSystim = (uint32_t)(absTick * ClockP_TICK_PERIOD);

    /* At this point, we no longer care about the previously set compare value,
     * but we might end up getting a pending interrupt from the old compare
     * value because it could now be in the past. To prevent the the CPU from
     * vectoring to the ISR for the wrong compare value, the pending interrupt
     * needs to be cleared, but before that is done, the channel also needs to
     * be un-armed. This is for the case where the interrupt would become
     * pending after clearing the interrupt, but before updating the compare
     * value.
     */

    /* Un-arm SysTimer channel 0 */
    HWREG(SYSTIM_BASE + SYSTIM_O_ARMCLR) = SYSTIM_ARMCLR_CH0_CLR;

    /* Clear pending interrupt */
    HwiP_clearInterrupt(INT_CPUIRQ16);

    /* Note: Channel interrupt flag is automatically cleared when writing a
     * compare value, but the pending status is not cleared in NVIC, which is
     * why the above two statements are needed.
     * Writing a new compare value will re-arm the channel.
     */
    HWREG(SYSTIM_BASE + SYSTIM_O_CH0CC) = newSystim;

    /* Remember this */
    ClockP_nextScheduledTick = absTick;
}

/*
 *  ======== ClockP_walkQueueDynamic ========
 *  Walk the Clock Queue for TickMode_DYNAMIC, optionally servicing a
 *  specific tick
 *
 *  Returns the number of ticks from thisTick to the next timeout.
 *  If no future timeouts exists, ~0 is returned.
 */
uint32_t ClockP_walkQueueDynamic(bool service, uint32_t thisTick)
{
    uint32_t distance = ~0;
    List_List *list   = &ClockP_list;
    List_Elem *elem;
    ClockP_Obj *obj;
    uint32_t delta;

    /* Traverse clock queue */
    for (elem = List_head(list); elem != NULL; elem = List_next(elem))
    {

        obj = (ClockP_Obj *)elem;

        /* If the object is active ... */
        if (obj->active == true)
        {

            /* Optionally service if tick matches timeout */
            if (service == true)
            {

                /* If this object is timing out update its state */
                if (obj->currTimeout == thisTick)
                {

                    if (obj->period == 0)
                    {
                        /* Oneshot: Mark object idle */
                        obj->active = false;
                    }
                    else
                    {
                        /* Periodic: Refresh timeout */
                        obj->currTimeout += (obj->period / CLOCK_FREQUENCY_DIVIDER);
                    }

                    /* Call handler */
                    obj->fxn(obj->arg);
                }
            }

            /* If object is still active, update distance to soonest timeout */
            if (obj->active == true)
            {

                delta = obj->currTimeout - thisTick;

                /* If this is the soonest timeout, update distance to soonest */
                if (delta < distance)
                {
                    distance = delta;
                }
            }
        }
    }

    return (distance);
}

/*
 *  ======== ClockP_workFuncDynamic ========
 *  Service Clock Queue for TickMode_DYNAMIC
 */
void ClockP_workFuncDynamic(uintptr_t arg)
{
    uintptr_t hwiKey;

    /* The tick count at the entry of this function. This is our definition of
     * "now".
     */
    uint32_t nowTick;

    /* The tick count for the current timeout to service */
    uint32_t timeoutTick;

    /* The number of ticks between the current timeout to service and the next
     * timeout.
     */
    uint32_t distance;

    /* The number of ticks since the current timeout to service relative to now.
     * (nowTick)
     */
    uint32_t ticksSinceTimeout;

    /* The next tick to schedule at the end of this function */
    uint32_t nextTick;

    hwiKey = HwiP_disable();

    /* Get current tick count. */
    nowTick = getClockPTick();

    /* Set flags while actively servicing queue */
    ClockP_inWorkFunc                = true;
    ClockP_startOrStopDuringWorkFunc = false;

    /* Determine the first expired timeout to service.
     * This function will be called by ClockP_hwiCallback after the "next
     * scheduled tick" has expired. The "next scheduled tick" will therefore be
     * now or in the past (until it is updated at the end of this function).
     * The "next scheduled tick" will either be the value of a timeout or it
     * will be a dummy tick that was scheduled ClockP_PERIOD_MAX ticks into the
     * future. In both scenarios the "next scheduled tick" will be treated as a
     * timeout. If it was a dummy tick, it will just result in no timeouts being
     * serviced for that specific tick.
     */
    timeoutTick       = ClockP_nextScheduledTick;
    /* Number of ticks since the timeout to service */
    ticksSinceTimeout = nowTick - timeoutTick;

    HwiP_restore(hwiKey);

    /* In the first iteration of below loop, the distance is set to 0, to ensure
     * that the the first expired timeout will be serviced.
     */
    distance = 0;

    /* Walk queue until the next timeout is in the future. */
    while (ticksSinceTimeout >= distance)
    {
        /* Determine the next timeout to service */
        timeoutTick += distance;
        /* Number of ticks since the timeout */
        ticksSinceTimeout -= distance;
        /* Walk queue and service timeout(s) and return the distance to the next
         * timeout.
         */
        distance = ClockP_walkQueueDynamic(true, timeoutTick);
    }

    hwiKey = HwiP_disable();

    /* If ClockP_start() or ClockP_stop() was called during processing of queue,
     * re-walk to update distance.
     */
    if (ClockP_startOrStopDuringWorkFunc == true)
    {
        distance = ClockP_walkQueueDynamic(false, timeoutTick);
    }

    /* Cap the distance to the maximum distance supported by the timer */
    if (distance > ClockP_PERIOD_MAX)
    {
        distance = ClockP_PERIOD_MAX;
    }

    /* Next tick is the latest timeout that was serviced plus the distance to
     * the next timeout.
     */
    nextTick = timeoutTick + distance;

    /* Reprogram for next expected tick */
    ClockP_scheduleNextTick(nextTick);

    ClockP_inWorkFunc = false;
    ClockP_ticks      = timeoutTick;

    HwiP_restore(hwiKey);
}

/*
 *  ======== ClockP_hwiCallback ========
 */
void ClockP_hwiCallback(uintptr_t arg)
{
    /* ClockP is using raw SysTimer channel 0 interrupt. Clearing the channel 0
     * flag is strictly not necessary, but doing it here to avoid confusion for
     * anyone using the SysTimer combined event.
     */
    HWREG(SYSTIM_BASE + SYSTIM_O_ICLR) = SYSTIM_ICLR_EV0_CLR;

    /* Run worker function */
    ClockP_workFuncDynamic(arg);
}

/*
 *  ======== ClockP_construct ========
 */
ClockP_Handle ClockP_construct(ClockP_Struct *handle, ClockP_Fxn fxn, uint32_t timeout, ClockP_Params *params)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;

    if (handle == NULL)
    {
        return NULL;
    }

    if (params == NULL)
    {
        params = &ClockP_defaultParams;
    }

    obj->period  = params->period;
    obj->timeout = timeout;
    obj->fxn     = fxn;
    obj->arg     = params->arg;
    obj->active  = false;

    ClockP_startup();

    /* Clock object is always placed on the ClockP work queue */
    List_put(&ClockP_list, &obj->elem);

    if (params->startFlag)
    {
        ClockP_start(obj);
    }

    return ((ClockP_Handle)handle);
}

/*
 *  ======== ClockP_create ========
 */
ClockP_Handle ClockP_create(ClockP_Fxn clkFxn, uint32_t timeout, ClockP_Params *params)
{
    ClockP_Handle handle;

    handle = (ClockP_Handle)malloc(sizeof(ClockP_Obj));

    /* ClockP_construct will check handle for NULL, no need here */
    handle = ClockP_construct((ClockP_Struct *)handle, clkFxn, timeout, params);

    return (handle);
}

/*
 *  ======== ClockP_destruct ========
 */
void ClockP_destruct(ClockP_Struct *clk)
{
    ClockP_Obj *obj = (ClockP_Obj *)clk;

    List_remove(&ClockP_list, &obj->elem);
}

/*
 *  ======== ClockP_add ========
 */
void ClockP_add(ClockP_Struct *handle, ClockP_Fxn fxn, uint32_t timeout, uintptr_t arg)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;

    obj->period  = 0;
    obj->timeout = timeout;
    obj->fxn     = fxn;
    obj->arg     = arg;
    obj->active  = false;

    /* Clock object is always placed on Clock work Q */
    List_put(&ClockP_list, &obj->elem);
}

/*
 *  ======== ClockP_delete ========
 */
void ClockP_delete(ClockP_Handle handle)
{
    ClockP_destruct((ClockP_Struct *)handle);

    free(handle);
}

/*
 *  ======== ClockP_start ========
 *  Set the Clock object's currTimeout value and set its active flag
 *  to true.
 */
void ClockP_start(ClockP_Handle handle)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;
    uintptr_t key   = HwiP_disable();

    uint32_t nowTick;
    uint32_t nowDelta;
    uint32_t scheduledTick;
    uint32_t scheduledDelta;
    uint32_t remainingTicks;
    bool objectServiced = false;

    /* if Clock is NOT currently processing its Q */
    if (ClockP_inWorkFunc == false)
    {

        /* get current tick count */
        nowTick = getClockPTick();

        nowDelta       = nowTick - ClockP_ticks;
        scheduledTick  = ClockP_nextScheduledTick;
        scheduledDelta = scheduledTick - ClockP_ticks;

        /* Check if this new timeout is before next scheduled tick ... */
        if (nowDelta <= scheduledDelta)
        {
            objectServiced = true;

            /* Start new Clock object */
            obj->currTimeout = nowTick + (obj->timeout / CLOCK_FREQUENCY_DIVIDER);
            obj->active      = true;

            /* How many ticks until scheduled tick? */
            remainingTicks = scheduledTick - nowTick;

            if ((obj->timeout / CLOCK_FREQUENCY_DIVIDER) < remainingTicks)
            {
                ClockP_scheduleNextTick(obj->currTimeout);
            }
        }
    }

    if (objectServiced == false)
    {
        /* Get current tick count */
        nowTick = getClockPTick();

        /* Start new Clock object */
        obj->currTimeout = nowTick + (obj->timeout / CLOCK_FREQUENCY_DIVIDER);
        obj->active      = true;

        if (ClockP_inWorkFunc == true)
        {
            ClockP_startOrStopDuringWorkFunc = true;
        }
    }

    HwiP_restore(key);
}

/*
 *  ======== ClockP_stop ========
 *  Remove and clear Clock object's queue elem from clockQ
 */
void ClockP_stop(ClockP_Handle handle)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;
    uintptr_t key   = HwiP_disable();

    uint32_t nowTick;
    uint32_t nowDelta;
    uint32_t scheduledTick;
    uint32_t scheduledDelta;
    uint32_t newScheduledTickDelta;

    obj->active = false;

    if (ClockP_inWorkFunc)
    {
        /* If in the work function, let it handle scheduling the next tick */
        ClockP_startOrStopDuringWorkFunc = true;
    }
    else
    {
        /* Re-compute next scheduled tick, if the current one is the timeout of
         * the stopped clock.
         */
        if (obj->currTimeout == ClockP_nextScheduledTick)
        {
            /* Get current tick count */
            nowTick = getClockPTick();

            nowDelta       = nowTick - ClockP_ticks;
            scheduledTick  = ClockP_nextScheduledTick;
            scheduledDelta = scheduledTick - ClockP_ticks;

            /* Check if "now" is before the next scheduled tick.
             * If "now" is after the next scheduled tick (i.e. the next
             * scheduled tick is in the past), then there will be a pending
             * interrupt, and the rescheduling will be done in
             * ClockP_workFuncDynamic(), instead of below.
             */
            if (nowDelta < scheduledDelta)
            {
                /* Determine distance to next tick */
                newScheduledTickDelta = ClockP_walkQueueDynamic(false, nowTick);

                /* Cap the distance to the maximum distance supported by the timer */
                if (newScheduledTickDelta > ClockP_PERIOD_MAX)
                {
                    newScheduledTickDelta = ClockP_PERIOD_MAX;
                }

                /* Schedule the next tick */
                ClockP_scheduleNextTick(nowTick + newScheduledTickDelta);

                ClockP_ticks = nowTick;
            }
        }
    }

    HwiP_restore(key);
}

/*
 *  ======== ClockP_setFunc ========
 */
void ClockP_setFunc(ClockP_Handle handle, ClockP_Fxn clockFxn, uintptr_t arg)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;

    uintptr_t key = HwiP_disable();

    obj->fxn = clockFxn;
    obj->arg = arg;

    HwiP_restore(key);
}

/*
 *  ======== ClockP_setTimeout ========
 */
void ClockP_setTimeout(ClockP_Handle handle, uint32_t timeout)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;

    obj->timeout = timeout;
}

/*
 *  ======== ClockP_setPeriod ========
 */
void ClockP_setPeriod(ClockP_Handle handle, uint32_t period)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;

    obj->period = period;
}

/*
 *  ======== ClockP_getTimeout ========
 */
uint32_t ClockP_getTimeout(ClockP_Handle handle)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;

    if (obj->active == true)
    {
        return (obj->currTimeout - getClockPTick());
    }
    else
    {
        return (obj->timeout);
    }
}

/*
 *  ======== ClockP_isActive ========
 */
bool ClockP_isActive(ClockP_Handle handle)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;

    return (obj->active);
}

/*
 *  ======== ClockP_getCpuFreq ========
 */
void ClockP_getCpuFreq(ClockP_FreqHz *freq)
{
    freq->lo = (uint32_t)configCPU_CLOCK_HZ;
    freq->hi = 0;
}

/*
 *  ======== ClockP_getSystemTickPeriod ========
 */
uint32_t ClockP_getSystemTickPeriod(void)
{
    return (ClockP_TICK_PERIOD);
}

/*
 *  ======== ClockP_getSystemTicks ========
 */
uint32_t ClockP_getSystemTicks(void)
{
    /* SysTimer is always running */
    return (getClockPTick());
}

/*
 *  ======== systemTicks64Callback ========
 */
static void systemTicks64Callback(uintptr_t arg)
{
    /* Disable interrupts while updating upper global upperSystemTicks64 */
    uintptr_t key = HwiP_disable();

    /* Update upper 32 bits of 64 bit system ticks if overflow has occurred */
    if (HWREG(SYSTIM_BASE + SYSTIM_O_RIS) & SYSTIM_RIS_OVFL_M)
    {
        /* Update upper 32 bits of 64 bit system ticks */
        upperSystemTicks64++;

        /* Clear overflow flag */
        HWREG(SYSTIM_BASE + SYSTIM_O_ICLR) = SYSTIM_ICLR_OVFL_CLR;
    }

    HwiP_restore(key);
}

/*
 *  ======== ClockP_getSystemTicks64 ========
 */
uint64_t ClockP_getSystemTicks64(void)
{
    static ClockP_Struct systemTicks64Clock;
    static bool systemTicks64Initialised = false;
    ClockP_Params params;
    uint32_t lowerSystemTicks64;
    uintptr_t key;
    uint64_t tickValue;

    key = HwiP_disable();

    /* Initialise clock needed to maintain 64-bit SystemTicks when function
     * is called for the first time.
     */
    if (!systemTicks64Initialised)
    {
        /* Clear overflow flag in case it is already set */
        HWREG(SYSTIM_BASE + SYSTIM_O_ICLR) = SYSTIM_ICLR_OVFL_CLR;

        ClockP_Params_init(&params);

        /* Start clock immediately when created */
        params.startFlag = true;
        /* The clock should ideally trigger with same frequency as 32-bit
         * overflow, but that would require a period longer than
         * ClockP_PERIOD_MAX. So we use double the frequency and only update
         * upperSystemTicks64 in the callback function if the overflow (OVFL)
         * interrupt flag is set.
         */
        params.period    = ((uint64_t)UINT32_MAX + 1) / 2;

        uint32_t currentTick = ClockP_getSystemTicks();

        /* The clock must be synchronized with the overflow event.
         * Calculate ticks until next overflow.
         */
        uint32_t delayedStart = UINT32_MAX - currentTick + 1;

        /* If the next overflow is too far in the future, then subtract the
         * period of the clock.
         */
        if (delayedStart > ClockP_PERIOD_MAX)
        {
            delayedStart -= params.period;
        }

        /* Construct and start the clock */
        ClockP_construct(&systemTicks64Clock, systemTicks64Callback, delayedStart, &params);

        systemTicks64Initialised = true;
    }

    lowerSystemTicks64 = ClockP_getSystemTicks();

    /* If the lower 32 bits have recently overflowed, but the upper 32 bits
     * have not yet been incremented (i.e. systemTicks64Callback() has not yet
     * executed) then increment the upper 32 bits here.
     */
    if (HWREG(SYSTIM_BASE + SYSTIM_O_RIS) & SYSTIM_RIS_OVFL_M)
    {
        /* Update upper 32 bits of 64 bit system ticks */
        upperSystemTicks64++;

        /* Clear overflow flag, to prevent upper 32 bits from also being
         * incremented in systemTicks64Callback()
         */
        HWREG(SYSTIM_BASE + SYSTIM_O_ICLR) = SYSTIM_ICLR_OVFL_CLR;

        /* Read the lower 32 bits again since the overflow might have happened
         * after the last read
         */
        lowerSystemTicks64 = ClockP_getSystemTicks();
    }

    /* Return the upper 32 bits + lower 32 bits as is */
    tickValue = ((uint64_t)upperSystemTicks64 << 32) | lowerSystemTicks64;

    HwiP_restore(key);

    return tickValue;
}

/*
 *  ======== ClockP_sleep ========
 */
void ClockP_sleep(uint32_t sec)
{
    uint32_t ticksToSleep;

    if (sec > ClockP_PERIOD_MAX_SEC)
    {
        sec = ClockP_PERIOD_MAX_SEC;
    }
    /* Convert from seconds to number of ticks */
    ticksToSleep = (sec * 1000000U) / ClockP_TICK_PERIOD;
    sleepTicks(ticksToSleep);
}

/*
 *  ======== ClockP_usleep ========
 */
void ClockP_usleep(uint32_t usec)
{
    uint32_t currTick;
    uint32_t endTick;
    uint32_t ticksToSleep;

    /* Systimer is always running, get tick as soon as possible */
    currTick = getClockPTick();

    /* Make sure we sleep at least one tick if usec > 0 */
    endTick = currTick + ((usec + ClockP_TICK_PERIOD - 1) / (ClockP_TICK_PERIOD * CLOCK_FREQUENCY_DIVIDER));

    /* If usec large enough, sleep for the appropriate number of clock ticks. */
    if (usec > ClockP_PROC_OVERHEAD_US)
    {
        ClockP_startup();
        ticksToSleep = (usec - ClockP_PROC_OVERHEAD_US) / ClockP_TICK_PERIOD;
        sleepTicks(ticksToSleep);
    }

    /* Spin remaining time */
    do
    {
        currTick = getClockPTick();
    } while (currTick < endTick);
}

/*
 *  ======== ClockP_staticObjectSize ========
 *  Internal function for testing that ClockP_Struct is large enough
 *  to hold ClockP object.
 */
size_t ClockP_staticObjectSize(void)
{
    return (sizeof(ClockP_Obj));
}

/*
 *  ======== sleepTicks ========
 *  Sleep for a given number of ClockP ticks.
 */
static void sleepTicks(uint32_t ticks)
{
    /* Cap to max number of ticks supported */
    if (ticks > ClockP_PERIOD_MAX)
    {
        ticks = ClockP_PERIOD_MAX;
    }

    SemaphoreP_Struct semStruct;
    ClockP_Struct clkStruct;
    ClockP_Params clkParams;
    SemaphoreP_Handle sem;

    /* Construct a semaphore, and a clock object to post the semaphore */
    sem = SemaphoreP_construct(&semStruct, 0, NULL);
    ClockP_Params_init(&clkParams);
    clkParams.startFlag = true;
    clkParams.arg       = (uintptr_t)sem;
    ClockP_construct(&clkStruct, sleepClkFxn, ticks, &clkParams);

    /* Pend forever on the semaphore, wait for ClockP callback to post it */
    SemaphoreP_pend(sem, SemaphoreP_WAIT_FOREVER);

    /* Clean up */
    SemaphoreP_destruct(&semStruct);
    ClockP_destruct(&clkStruct);
}

/*
 *  ======== sleepClkFxn ========
 *  Timeout function for sleepTicks().
 */
static void sleepClkFxn(uintptr_t arg0)
{
    SemaphoreP_post((SemaphoreP_Handle)arg0);
}
