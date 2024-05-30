/*
 * Copyright (c) 2022-2024, Texas Instruments Incorporated
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

#include <stdint.h>
#include <stdlib.h>

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>

#include <ti/drivers/watchdog/WatchdogLPF3.h>

#include <ti/drivers/Watchdog.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_ckmd.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)

/* Function prototypes */
void WatchdogLPF3_clear(Watchdog_Handle handle);
void WatchdogLPF3_close(Watchdog_Handle handle);
int_fast16_t WatchdogLPF3_control(Watchdog_Handle handle, uint_fast16_t cmd, void *arg);
void WatchdogLPF3_init(Watchdog_Handle handle);
Watchdog_Handle WatchdogLPF3_open(Watchdog_Handle handle, Watchdog_Params *params);
int_fast16_t WatchdogLPF3_setReload(Watchdog_Handle handle, uint32_t ticks);
uint32_t WatchdogLPF3_convertMsToTicks(Watchdog_Handle handle, uint32_t milliseconds);

/* WatchdogLPF3 internal functions */
static void WatchdogLPF3_initHw(Watchdog_Handle handle);

/* WatchdogLPF3 global variables */
static volatile uint32_t reloadValue; /* Reload value in ticks */

/* Watchdog function table for CC23X0 implementation */
const Watchdog_FxnTable WatchdogLPF3_fxnTable = {WatchdogLPF3_clear,
                                                 WatchdogLPF3_close,
                                                 WatchdogLPF3_control,
                                                 WatchdogLPF3_init,
                                                 WatchdogLPF3_open,
                                                 WatchdogLPF3_setReload,
                                                 WatchdogLPF3_convertMsToTicks};

#define SCLK_LF_FREQ_HZ  32768      /* LFCLK frequency in Hz clocking Watchdog */
#define MAX_RELOAD_VALUE 0xFFFFFFFF /* Maximum allowable reload value */
#define MS_RATIO         1000       /* millisecond to second ratio */
#define WATCHDOG_UNLOCK  0x1ACCE551 /* Watchdog unlocking value */

/* TODO: Remove once CC27XX register names catch up */
#if (DeviceFamily_ID == DeviceFamily_ID_CC27XX)
    #define CKMD_O_WDTLOCK           CKMD_O_LOCK
    #define CKMD_O_WDTCNT            CKMD_O_CNT
    #define CKMD_O_WDTTEST           CKMD_O_TEST
    #define CKMD_WDTTEST_STALLEN_EN  CKMD_TEST_STALLEN_EN
    #define CKMD_WDTTEST_STALLEN_DIS CKMD_TEST_STALLEN_DIS
#endif

/*
 *  ======== WatchdogLPF3_lock ========
 */
static inline void WatchdogLPF3_lock(void)
{
    HWREG(CKMD_BASE + CKMD_O_WDTLOCK) = 0x0;
}

/*
 *  ======== WatchdogLPF3_unlock ========
 */
static inline void WatchdogLPF3_unlock(void)
{
    HWREG(CKMD_BASE + CKMD_O_WDTLOCK) = WATCHDOG_UNLOCK;
}

/*
 *  ======== WatchdogLPF3_isLocked ========
 */
static inline bool WatchdogLPF3_isLocked(void)
{
    return HWREG(CKMD_BASE + CKMD_O_WDTLOCK) == 1;
}

/*
 *  ======== WatchdogLPF3_setReloadValue ========
 */
static inline void WatchdogLPF3_setReloadValue(uint32_t ticks)
{
    HWREG(CKMD_BASE + CKMD_O_WDTCNT) = ticks;
}

/*
 *  ======== WatchdogLPF3_enableStall ========
 */
static inline void WatchdogLPF3_enableStall(void)
{
    HWREG(CKMD_BASE + CKMD_O_WDTTEST) = CKMD_WDTTEST_STALLEN_EN;
}

/*
 *  ======== WatchdogLPF3_disableStall ========
 */
static inline void WatchdogLPF3_disableStall(void)
{
    HWREG(CKMD_BASE + CKMD_O_WDTTEST) = CKMD_WDTTEST_STALLEN_DIS;
}

/*
 *  ======== WatchdogLPF3_clear ========
 */
void WatchdogLPF3_clear(Watchdog_Handle handle)
{
    uintptr_t key;

    /* disable preemption while unlocking Watchdog registers */
    key = HwiP_disable();

    /* unlock the Watchdog configuration registers */
    WatchdogLPF3_unlock();

    /* make sure the Watchdog is unlocked before continuing */
    while (WatchdogLPF3_isLocked()) {}

    WatchdogLPF3_setReloadValue(reloadValue);

    /* lock the Watchdog configuration registers */
    WatchdogLPF3_lock();

    HwiP_restore(key);
}

/*
 *  ======== WatchdogLPF3_close ========
 */
void WatchdogLPF3_close(Watchdog_Handle handle)
{
    /*
     *  Not supported for CC23X0 - Once the Watchdog module is started
     *  it can only be stopped by a device reset.
     */
    DebugP_assert(false);
}

/*
 *  ======== WatchdogLPF3_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int_fast16_t WatchdogLPF3_control(Watchdog_Handle handle, uint_fast16_t cmd, void *arg)
{
    /* Not supported on CC23X0 */
    return (Watchdog_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== WatchdogLPF3_init ========
 */
void WatchdogLPF3_init(Watchdog_Handle handle)
{
    WatchdogLPF3_Object *object = handle->object;
    object->isOpen              = false;
}

/*
 *  ======== WatchdogLPF3_open ========
 */
Watchdog_Handle WatchdogLPF3_open(Watchdog_Handle handle, Watchdog_Params *params)
{
    uintptr_t key;
    WatchdogLPF3_Object *object;

    /* get the pointer to the object and hwAttrs */
    object = handle->object;

    /* disable preemption while checking if the Watchdog is open. */
    key = HwiP_disable();

    /* Check if the Watchdog is open already with the hwAttrs */
    if (object->isOpen == true)
    {
        HwiP_restore(key);
        return (NULL);
    }

    object->isOpen = true;
    HwiP_restore(key);

    /* initialize the Watchdog object */
    object->debugStallMode = params->debugStallMode;

    /* initialize the watchdog hardware */
    WatchdogLPF3_initHw(handle);

    /* return handle of the Watchdog object */
    return (handle);
}

/*
 *  ======== WatchdogLPF3_setReload ========
 */
int_fast16_t WatchdogLPF3_setReload(Watchdog_Handle handle, uint32_t ticks)
{
    uintptr_t key;

    /* disable preemption while unlocking Watchdog registers */
    key = HwiP_disable();

    /* unlock the Watchdog configuration registers */
    WatchdogLPF3_unlock();

    /* make sure the Watchdog is unlocked before continuing */
    while (WatchdogLPF3_isLocked()) {}

    /* update the reload value */
    reloadValue = ticks;
    WatchdogLPF3_setReloadValue(reloadValue);

    /* lock register access */
    WatchdogLPF3_lock();

    HwiP_restore(key);

    return (Watchdog_STATUS_SUCCESS);
}

/*
 *  ======== WatchdogLPF3_initHw ========
 *  This function initializes the Watchdog hardware module.
 *
 *  @pre    Function assumes that the Watchdog handle is pointing to a hardware
 *          module which has already been opened.
 */
static void WatchdogLPF3_initHw(Watchdog_Handle handle)
{
    uintptr_t key;
    uint32_t tickValue;
    WatchdogLPF3_Object *object;
    WatchdogLPF3_HWAttrs const *hwAttrs;

    /* get the pointer to the object and hwAttrs */
    object  = handle->object;
    hwAttrs = handle->hwAttrs;

    /* convert milliseconds to watchdog timer ticks */
    tickValue = WatchdogLPF3_convertMsToTicks(handle, hwAttrs->reloadValue);

    /* disable preemption while unlocking WatchDog registers */
    key = HwiP_disable();

    /* unlock the Watchdog configuration registers */
    WatchdogLPF3_unlock();

    /* make sure the Watchdog is unlocked before continuing */
    while (WatchdogLPF3_isLocked()) {}

    /* set debug stall mode */
    if (object->debugStallMode == Watchdog_DEBUG_STALL_ON)
    {
        WatchdogLPF3_enableStall();
    }
    else
    {
        WatchdogLPF3_disableStall();
    }

    /* store reloadValue for future clear() operations */
    reloadValue = tickValue;
    WatchdogLPF3_setReloadValue(reloadValue);

    /* lock the Watchdog configuration registers */
    WatchdogLPF3_lock();

    HwiP_restore(key);
}

/*
 *  ======== WatchdogLPF3_convertMsToTicks ========
 *  This function converts the input value from milliseconds to
 *  Watchdog clock ticks.
 */
uint32_t WatchdogLPF3_convertMsToTicks(Watchdog_Handle handle, uint32_t milliseconds)
{
    uint32_t tickValue;
    uint32_t maxConvertMs;

    /* Determine maximum allowed period with the current SCLK_LF frequency */
    maxConvertMs = (MAX_RELOAD_VALUE / SCLK_LF_FREQ_HZ) * MS_RATIO;

    /* convert milliseconds to watchdog timer ticks */
    /* check if value exceeds maximum */
    if (milliseconds > maxConvertMs)
    {
        tickValue = 0; /* return zero to indicate overflow */
    }
    else
    {
        /* avoid overflow of the intermediate multiplication when calculating tickValue */
        tickValue = (milliseconds / MS_RATIO) * SCLK_LF_FREQ_HZ +
                    ((milliseconds % MS_RATIO) * SCLK_LF_FREQ_HZ) / MS_RATIO;
    }

    return (tickValue);
}
