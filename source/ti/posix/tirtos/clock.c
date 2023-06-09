/*
 * Copyright (c) 2015-2022 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== clock.c ========
 */

#include <xdc/std.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/hal/Seconds.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <time.h>
#include <errno.h>

#include "pthread_util.h"

static Void clockFxn(UArg arg);
static UInt32 getClockTicks(Void);

static UInt32 prevTicks     = 0;
static UInt32 rolloverCount = 0;
static Clock_Struct clockStruct;
static Bool firstTime = TRUE;

/*
 *  ======== clock_gettime ========
 */
int clock_gettime(clockid_t clockId, struct timespec *ts)
{
    Seconds_Time t;
    UInt32 secs;
    UInt32 ticks;
    UInt32 remTicks;
    UInt32 remSecs;
    UInt key;
    UInt32 numRollovers;

    if ((clockId != CLOCK_MONOTONIC) && (clockId != CLOCK_REALTIME))
    {
        errno = EINVAL;
        return (-1);
    }

    if (clockId == CLOCK_REALTIME)
    {
        Seconds_getTime(&t);

#if defined(_TARGET_DEFAULTS_TO_TIME64) || (defined(__TI_TIME_USES_64) && __TI_TIME_USES_64)
        ts->tv_sec = t.secsHi;
        ts->tv_sec = ts->tv_sec << 32;
        ts->tv_sec = ts->tv_sec | t.secs;
#else
        ts->tv_sec = t.secs;
#endif
        ts->tv_nsec = t.nsecs;
    }
    else
    {
        /* CLOCK_MONOTONIC */
        key = Hwi_disable();

        if (firstTime)
        {
            Clock_addI(Clock_handle(&clockStruct), (Clock_FuncPtr)clockFxn, (UInt)0xFFFFFFFF, (UArg)0);
            Clock_setPeriod(Clock_handle(&clockStruct), 0xFFFFFFFF);
            Clock_startI(Clock_handle(&clockStruct));
            firstTime = FALSE;
        }

        ticks        = getClockTicks();
        numRollovers = rolloverCount;

        Hwi_restore(key);

        secs = ticks / TICKS_PER_SEC;

        /* Remaining ticks after seconds are subtracted from tick count */
        remTicks = ticks - (secs * TICKS_PER_SEC);

        /* Add contribution of remaining ticks from tick count rollovers */
        remTicks += REM_TICKS * numRollovers;

        remSecs = remTicks / TICKS_PER_SEC;
        remTicks -= remSecs * TICKS_PER_SEC;

        ts->tv_sec  = (time_t)secs + remSecs + (MAX_SECONDS * numRollovers);
        ts->tv_nsec = (unsigned long)(remTicks * Clock_tickPeriod * 1000);
    }

    return (0);
}

/*
 *  ======== clock_nanosleep ========
 */
int clock_nanosleep(clockid_t clockId, int flags, const struct timespec *rqtp, struct timespec *rmtp)
{
    uint32_t ticks;

    if ((clockId != CLOCK_MONOTONIC) && (clockId != CLOCK_REALTIME))
    {
        return (EINVAL);
    }

    if ((rmtp != NULL) && (flags & TIMER_ABSTIME) == 0)
    {
        /*
         *  In the relative case, rmtp will contain the amount of time
         *  remaining (requested time - actual time slept).  For BIOS,
         *  this will always be 0.
         */
        rmtp->tv_nsec = rmtp->tv_sec = 0;
    }

    if (flags & TIMER_ABSTIME)
    {
        if (_pthread_abstime2ticks(clockId, rqtp, &ticks) != 0)
        {
            return (EINVAL);
        }

        if (ticks == 0)
        {
            return (0);
        }
    }
    else
    {
        /* max interval, needs to be fixed: TIRTOS-1314 */
        if (rqtp->tv_sec >= MAX_SECONDS)
        {
            return (EINVAL);
        }

        if ((rqtp->tv_sec == 0) && (rqtp->tv_nsec == 0))
        {
            return (0);
        }

        ticks = rqtp->tv_sec * (1000000 / Clock_tickPeriod);

        /* Take the ceiling */
        ticks += (rqtp->tv_nsec + Clock_tickPeriod * 1000 - 1) / (Clock_tickPeriod * 1000);

        /*
         *  Add one tick to ensure the timeout is not less than the
         *  amount of time requested.  The Clock may be about to tick,
         *  and that counts as one tick even though the amount of time
         *  until this tick is less than Clock_tickPeriod.
         */
        ticks++;
    }

    Task_sleep(ticks);

    return (0);
}

/*
 *  ======== clock_settime ========
 */
int clock_settime(clockid_t clock_id, const struct timespec *ts)
{

    if (clock_id != CLOCK_REALTIME)
    {
        errno = EINVAL;
        return (-1);
    }

    if ((ts->tv_nsec < 0) || (ts->tv_nsec >= 1000000000))
    {
        errno = EINVAL;
        return (-1);
    }

#if BIOS_version < 0x67500
    Seconds_set(ts->tv_sec);
#else
    {
        Seconds_Time its;

    #if defined(_TARGET_DEFAULTS_TO_TIME64) || (defined(__TI_TIME_USES_64) && __TI_TIME_USES_64)
        its.secsHi = (UInt32)(ts->tv_sec >> 32);
        its.secs = (UInt32)(ts->tv_sec);
    #else
        its.secsHi = 0;
        its.secs   = (UInt32)(ts->tv_sec);
    #endif
        its.nsecs = (UInt32)(ts->tv_nsec);
        Seconds_setTime(&its);
    }
#endif

    return (0);
}

/*
 *  ======== clock_getticks ========
 *  Internal function for testing.
 */
void ti_sysbios_posix_clock_getticks(UInt32 *ticksLo, UInt32 *ticksHi)
{
    UInt key;

    key = Hwi_disable();

    *ticksLo = getClockTicks();
    *ticksHi = rolloverCount;

    Hwi_restore(key);
}

/*
 *  ======== clockFxn ========
 */
static Void clockFxn(UArg arg)
{
    UInt key;

    key = Hwi_disable();

    (Void) getClockTicks();

    Hwi_restore(key);
}

/*
 *  ======== getClockTicks ========
 *  Call with interrupts disabled.
 */
static UInt32 getClockTicks(Void)
{
    UInt32 ticks;

    ticks = Clock_getTicks();
    if (ticks < prevTicks)
    {
        rolloverCount++;
    }
    prevTicks = ticks;

    return (ticks);
}
