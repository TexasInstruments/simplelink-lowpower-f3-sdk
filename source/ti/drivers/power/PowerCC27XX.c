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
/*
 *  ======== PowerCC27XX.c ========
 */

#include <stdbool.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/DebugP.h>

#include <ti/drivers/Power.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_clkctl.h)
#include DeviceFamily_constructPath(inc/hw_evtsvt.h)
#include DeviceFamily_constructPath(inc/hw_evtull.h)
#include DeviceFamily_constructPath(inc/hw_ckmd.h)
#include DeviceFamily_constructPath(inc/hw_rtc.h)
#include DeviceFamily_constructPath(inc/hw_systim.h)
#include DeviceFamily_constructPath(inc/hw_pmctl.h)
#include DeviceFamily_constructPath(inc/hw_pmud.h)
#include DeviceFamily_constructPath(inc/hw_sys0.h)
#include DeviceFamily_constructPath(inc/hw_ioc.h)
#include DeviceFamily_constructPath(driverlib/cpu.h)
#include DeviceFamily_constructPath(driverlib/hapi.h)
#include DeviceFamily_constructPath(driverlib/gpio.h)
#include DeviceFamily_constructPath(driverlib/lrfd.h)
#include DeviceFamily_constructPath(cmsis/core/cmsis_compiler.h)

/* Forward declarations */
int_fast16_t PowerCC27XX_notify(uint_fast16_t eventType);
static void PowerCC27XX_oscillatorISR(uintptr_t arg);
static void PowerCC27XX_rtcISR(uintptr_t arg);
static void PowerCC27XX_enterStandby(void);
static void PowerCC27XX_setDependencyCount(uint_fast16_t resourceId, uint8_t count);
bool PowerCC27XX_isValidResourceId(uint_fast16_t resourceId);

/* Externs */
extern const PowerCC27XX_Config PowerCC27XX_config;
extern const uint_least8_t GPIO_pinLowerBound;
extern const uint_least8_t GPIO_pinUpperBound;

/* Function Macros */
#define IOC_ADDR(index) (IOC_BASE + IOC_O_IOC0 + (sizeof(uint32_t) * index))

/* Macro used to extract the resource group  from a resource ID */
#define RESOURCE_GROUP(resourceId) ((resourceId)&PowerCC27XX_PERIPH_GROUP_M)

/* Macro used to extract the bit index shift encoded from a resource ID */
#define RESOURCE_BIT_INDEX(resourceId) ((resourceId)&PowerCC27XX_PERIPH_BIT_INDEX_M)

/* Static Globals */

/* Array to maintain constraint reference counts */
uint8_t constraintCounts[PowerCC27XX_NUMCONSTRAINTS];

/* Mask of Power constraints for quick access */
uint32_t constraintMask = 0;

/* Arrays to maintain resource dependency reference counts.
 * Each resource group will have an array associated with it, and the arrays can
 * be indexed using the bit index shift value encoded in the resource ID.
 */
uint8_t resourceCountsClkctl0[PowerCC27XX_NUMRESOURCES_CLKCTL0];
uint8_t resourceCountsClkctl1[PowerCC27XX_NUMRESOURCES_CLKCTL1];
uint8_t resourceCountsLrfd[PowerCC27XX_NUMRESOURCES_LRFD];

/* Keeps track of the configured Power policy. Power_idleFunc() will not run
 * the policy if this is set to NULL
 */
Power_PolicyFxn policyFxn = NULL;

/* Is the Power policy enabled? */
bool isPolicyEnabled = false;

/* Has the Power driver been initialized */
bool isInitialized = false;

/* Power state of the system. Idle, active, standby, etc */
uint8_t powerState = Power_ACTIVE;

/* Event notification list */
List_List notifyList;

/* Interrupt for handling clock switching */
HwiP_Struct ckmHwi;

/* Interrupt for ClockP and Power policy */
HwiP_Struct clockHwi;

/* ****************** Power APIs ******************** */

/*
 *  ======== Power_init ========
 */
int_fast16_t Power_init()
{
    /* If this function has already been called, just return */
    if (isInitialized)
    {
        return Power_SOK;
    }

    isInitialized = true;

    isPolicyEnabled = PowerCC27XX_config.policyFxn != NULL;

    policyFxn = PowerCC27XX_config.policyFxn;

    /* Construct the CKM hwi responsible for oscillator related events.
     * Since there is no dedicated CKM interrupt line, we need to mux one of
     * the configurable lines to the CKM.
     * CPUIRQ3 is dedicated to this purpose.
     */
    HwiP_construct(&ckmHwi, INT_CPUIRQ3, PowerCC27XX_oscillatorISR, NULL);

    HWREG(EVTSVT_BASE + EVTSVT_O_CPUIRQ3SEL) = EVTSVT_CPUIRQ3SEL_PUBID_AON_CKM_COMB;

    /* Enable a selection of CKM signals as interrupt sources. For now,
     * we will stick to LKCLKGOOD and AMPSETTLED since those are related to existing
     * notifications.
     */
    HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMASK_LFCLKGOOD | CKMD_IMASK_AMPSETTLED | CKMD_IMASK_HFXTFAULT;

    HwiP_enableInterrupt(INT_CPUIRQ3);

    /* Construct the ClockP hwi responsible for timing service events.
     * This Hwi is time multiplexed between the SysTimer and the RTC for use
     * by the ClockP and Power policy respectively.
     * Since there is no dedicated RTC or SysTimer interrupt line, we need to
     * mux one of the configurable lines to the CKM.
     * CPUIRQ16 is dedicated to this purpose.
     */
    HwiP_construct(&clockHwi, INT_CPUIRQ16, PowerCC27XX_rtcISR, NULL);

    /* Use RTC channel 0 in compare mode. Channel 1 could be used for other
     * purposes.
     */
    HWREG(RTC_BASE + RTC_O_IMSET) = RTC_IMSET_EV0_SET;

    /* Configure RTC to halt when CPU stopped during debug */
    HWREG(RTC_BASE + RTC_O_EMU) = RTC_EMU_HALT_STOP;

    /* Configure SysTimer to halt when CPU stopped during debug.
     * This setting needs to be re-applied in the Power policy after waking up
     * from standby.
     */
    HWREG(SYSTIM_BASE + SYSTIM_O_EMU) = SYSTIM_EMU_HALT_STOP;

    /* Enable HFXT and configure it to automatically start up when the device
     * wakes up from standby.
     * OR-in to avoid overwriting other fields with non-zero reset values.
     */
    /* TODO: move to CCFG copyList? */
    HWREG(CKMD_BASE + CKMD_O_HFXTCTL) |= CKMD_HFXTCTL_EN_M | CKMD_HFXTCTL_AUTOEN_M;

    // /* Enable tracking loop with HFXT as reference. This will automatically
    //  * calibrate LFOSC against HFXT whenever HFXT is enabled; usually after
    //  * waking up from standby.
    //  * This is needed to ensure fast HFXT startup and a reasonably accurate
    //  * LFOSC frequency.
    //  */
    // HWREG(CKMD_BASE + CKMD_O_HFTRACKCTL) |= CKMD_HFTRACKCTL_EN_M | CKMD_HFTRACKCTL_REFCLK_HFXT;
    // /* TODO: Register removed on CC27xx?
    //  * OR in value to ensure trims are not overwritten */
    // HWREG(CKMD_BASE + CKMD_O_LOOPCFG) |= CKMD_LOOPCFG_BOOT_DONE;

    // /* TODO: Bitfield missing for CC27xx. Find out if not applicable for CC27xx?
    // Enable LFINC updates when the HFOSC tracking loop is running */
    // HWREG(CKMD_BASE + CKMD_O_LFINCCTL) |= CKMD_LFINCCTL_EN_ONTRACK;

    /* Enable GPIO and RTC standby wakeup sources */
    /* TODO: move to CCFG copyList? */
    HWREG(EVTULL_BASE + EVTULL_O_WKUPMASK) = EVTULL_WKUPMASK_AON_IOC_COMB_M | EVTULL_WKUPMASK_AON_RTC_COMB_M;

    /* Enable ULDO */
    // HWREG(PMUD_BASE + PMUD_O_PREG0) |= PMUD_PREG0_UDIGLDO_EN_EN;

    // /* Enable DCDC */
    // HWREG(PMCTL_BASE + PMCTL_O_VDDRCTL) |= PMCTL_VDDRCTL_SELECT_DCDC;

    // /* TODO: Hack for untrimmed devices: Reduce recharge comp level to minimum */
    // HWREG(SYS0_BASE + SYS0_O_TMUTE4) &= ~SYS0_TMUTE4_RECHCOMPREFLVL_M;

    // // Set IDAC change delay. CKMD.AMPCFG1.IDACDLY=0xF
    // HWREG(CKMD_BASE + CKMD_O_AMPCFG1) &= ~CKMD_AMPCFG1_IDACDLY_M;
    // HWREG(CKMD_BASE + CKMD_O_AMPCFG1) |= 0xFU << CKMD_AMPCFG1_IDACDLY_S;

    // // Set Update rate for the AMPCOMP update rate. CKMD.AMPCFG0.FSMRATE=0x17 (250K)
    // HWREG(CKMD_BASE + CKMD_O_AMPCFG0) &= ~CKMD_AMPCFG0_FSMRATE_M;
    // HWREG(CKMD_BASE + CKMD_O_AMPCFG0) |= CKMD_AMPCFG0_FSMRATE__250K << CKMD_AMPCFG0_FSMRATE_S;

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
    return constraintMask;
}

/*
 *  ======== Power_getDependencyCount ========
 *  Get the count of dependencies that are currently declared upon a resource.
 */
int_fast16_t Power_getDependencyCount(uint_fast16_t resourceId)
{
    DebugP_assert(PowerCC27XX_isValidResourceId(resourceId));

    uint8_t bitIndex    = RESOURCE_BIT_INDEX(resourceId);
    uint_fast16_t group = RESOURCE_GROUP(resourceId);
    if (group == PowerCC27XX_PERIPH_GROUP_CLKCTL0)
    {
        return (int_fast16_t)resourceCountsClkctl0[bitIndex];
    }
    else if (group == PowerCC27XX_PERIPH_GROUP_CLKCTL1)
    {
        return (int_fast16_t)resourceCountsClkctl1[bitIndex];
    }
    else if (group == PowerCC27XX_PERIPH_GROUP_LRFD)
    {
        return (int_fast16_t)resourceCountsLrfd[bitIndex];
    }

    return (int_fast16_t)Power_EINVALIDINPUT;
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
 *  ======== Power_getTransitionState ========
 *  Get the current sleep transition state.
 */
uint_fast16_t Power_getTransitionState(void)
{
    return powerState;
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
        status = Power_EINVALIDPOINTER;
    }
    else
    {
        notifyObj->eventTypes = eventTypes;
        notifyObj->notifyFxn  = notifyFxn;
        notifyObj->clientArg  = clientArg;

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

    DebugP_assert(constraintId < PowerCC27XX_NUMCONSTRAINTS);

    key = HwiP_disable();

    /* Set the specified constraint in the constraintMask for faster access */
    constraintMask |= 1 << constraintId;

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

    DebugP_assert(constraintId < PowerCC27XX_NUMCONSTRAINTS);

    key = HwiP_disable();

    DebugP_assert(constraintCounts[constraintId] != 0);

    constraintCounts[constraintId]--;

    /* Only update the constraint mask if we removed the constraint entirely */
    if (constraintCounts[constraintId] == 0)
    {
        constraintMask &= ~(1 << constraintId);
    }

    HwiP_restore(key);

    return Power_SOK;
}

/*
 *  ======== Power_setDependency ========
 *  Declare a dependency upon a resource.
 */
int_fast16_t Power_setDependency(uint_fast16_t resourceId)
{
    uint8_t previousCount;
    uintptr_t key;
    uint8_t bitIndex;

    DebugP_assert(PowerCC27XX_isValidResourceId(resourceId));

    key = HwiP_disable();

    /* Buffer previous reference count */
    previousCount = Power_getDependencyCount(resourceId);

    /* Increment reference count */
    PowerCC27XX_setDependencyCount(resourceId, previousCount + 1);

    /* If the resource was NOT activated previously ... */
    if (previousCount == 0)
    {
        bitIndex = RESOURCE_BIT_INDEX(resourceId);
        /* Turn on the peripheral */
        switch (RESOURCE_GROUP(resourceId))
        {
            case PowerCC27XX_PERIPH_GROUP_CLKCTL0:
                HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET0) = 1 << bitIndex;
                break;
            case PowerCC27XX_PERIPH_GROUP_CLKCTL1:
                HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET1) = 1 << bitIndex;
                break;
            case PowerCC27XX_PERIPH_GROUP_LRFD:
                LRFDSetClockDependency(1 << bitIndex, LRFD_CLK_DEP_POWER);
                break;
            default:
                break;
        }
    }

    HwiP_restore(key);

    return Power_SOK;
}

/*
 *  ======== Power_releaseDependency ========
 *  Release a previously declared dependency.
 */
int_fast16_t Power_releaseDependency(uint_fast16_t resourceId)
{
    uint8_t resourceCount;
    uintptr_t key;
    uint8_t bitIndex;

    DebugP_assert(PowerCC27XX_isValidResourceId(resourceId));

    key = HwiP_disable();

    resourceCount = Power_getDependencyCount(resourceId);

    DebugP_assert(resourceCount != 0);

    /* Decrement the reference count */
    resourceCount--;
    PowerCC27XX_setDependencyCount(resourceId, resourceCount);

    /* If this was the last dependency being released.. */
    if (resourceCount == 0)
    {
        bitIndex = RESOURCE_BIT_INDEX(resourceId);
        /* Turn off the peripheral */
        switch (RESOURCE_GROUP(resourceId))
        {
            case PowerCC27XX_PERIPH_GROUP_CLKCTL0:
                HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR0) = 1 << bitIndex;
                break;
            case PowerCC27XX_PERIPH_GROUP_CLKCTL1:
                HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR1) = 1 << bitIndex;
                break;
            case PowerCC27XX_PERIPH_GROUP_LRFD:
                LRFDReleaseClockDependency(1 << bitIndex, LRFD_CLK_DEP_POWER);
                break;
            default:
                break;
        }
    }

    HwiP_restore(key);

    return Power_SOK;
}

/*
 *  ======== Power_shutdown ========
 */
int_fast16_t Power_shutdown(uint_fast16_t shutdownState, uint_fast32_t shutdownTime)
{
    int_fast16_t notifyStatus;
    uint32_t hwiKey;
    uint8_t i;
    bool ioPending = false;

    hwiKey = HwiP_disable();

    /* Check if there is a constraint to prohibit shutdown */
    if (Power_getConstraintMask() & (1 << PowerLPF3_DISALLOW_SHUTDOWN))
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
    notifyStatus = PowerCC27XX_notify(PowerLPF3_ENTERING_SHUTDOWN);

    for (i = GPIO_pinLowerBound; i <= GPIO_pinUpperBound; i++)
    {
        /* Read WUCFGSD field once and check both values */
        uint32_t ioShutdownConfig = HWREG(IOC_ADDR(i)) & IOC_IOC0_WUCFGSD_M;

        if ((ioShutdownConfig == IOC_IOC0_WUCFGSD_WAKE_HIGH || ioShutdownConfig == IOC_IOC0_WUCFGSD_WAKE_LOW) &&
            GPIOGetEventDio(i))
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
    status = PowerCC27XX_notify(PowerLPF3_ENTERING_STANDBY);

    /* Check for any error */
    if (status != Power_SOK)
    {
        powerState = Power_ACTIVE;
        return status;
    }

    /* Call wrapper function to ensure that R0-R3 are saved and restored before
     * and after this function call. Otherwise, compilers will attempt to stash
     * values on the stack while on the PSP and then restore them just after
     * HapiEnterStandby() on the MSP. Which will cause wildly unexpected
     * behaviour.
     */
    PowerCC27XX_enterStandby();

    /* Restore SYSTIMER halt during debug break.
     * This bit is cleared when waking up from standby since the SYSTIMER is in
     * the SVT domain.
     * Any breakpoints set before this write will cause the SYSTIMER to desync
     * and likely break program execution.
     */
    HWREG(SYSTIM_BASE + SYSTIM_O_EMU) = SYSTIM_EMU_HALT_STOP;

    /* Re-enable the AMPSETTLED interrupt.
     * Since it is a level status signal, it remains asserted when we are
     * running on HFXT and cannot be cleared.
     * The oscillator interrupt removes it from the interrupt mask to prevent
     * repeated vectoring.
     * Now that we are exiting standby, we need to enable it again to vector to
     * the interrupt once more.
     */
    HWREG(CKMD_BASE + CKMD_O_IMSET) = CKMD_IMSET_AMPSETTLED;

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
 *  ======== PowerCC27XX_doWFI ========
 */
void PowerCC27XX_doWFI(void)
{
    __WFI();
}

/*
 *  ======== PowerCC27XX_enterStandby ========
 *  Wrapper function to ensure that R0-R3 are saved and restored before and
 *  after this function call. Otherwise, compilers will attempt to stash
 *  values on the stack while on the PSP and then restore them just after
 *  HapiEnterStandby() on the MSP. Which will cause wildly unexpected behaviour.
 */
void PowerCC27XX_enterStandby(void)
{
    /* Declare static volatile variable to ensure the toolchain does not use
     * stack for the variable and does not optimize this memory allocation
     * away.
     */
    static volatile uint32_t controlPreStandby;

    /* Stash current CONTROL configuration to re-apply after wakeup.
     * Depending on the kernel used, we could be on PSP or MSP
     */
    controlPreStandby = __get_CONTROL();

    /* Switch to MSP. HapiEnterStandby() must execute from MSP since the
     * device reboots into privileged mode on MSP and HapiEnterStandby()
     * assumes it will be called running on MSP.
     */
    __set_CONTROL(0 << 1);

    /* TODO: add copyList support to Power driver */

    /* - Save CPU state on MSP and MSP in CLKCTL_STBYPTR
     * - Enter standby
     * - Exit standby
     * - Restore CPU state from MSP
     * - Apply copyList
     */
    HapiEnterStandby(NULL);

    /* Just go into WFI while we work on FPGA
     * TODO: reenable standby fxn when silicon arrives
     */
    //__WFI();

    /* Switch back to previous stack pointer. */
    __set_CONTROL(controlPreStandby);
}

/*
 *  ======== PowerLPF3_selectLFOSC ========
 */
void PowerLPF3_selectLFOSC(void)
{
    /* Set LFOSC to PRECLK and LFXT to LFCLK */
    HWREG(CKMD_BASE + CKMD_O_LFCLKSEL) = CKMD_LFCLKSEL_MAIN_LFOSC;

    /* Clear any existing interrupt flags */
    HWREG(CKMD_BASE + CKMD_O_IMCLR) = CKMD_IMCLR_LFCLKGOOD;

    /* Start LFOSC */
    HWREG(CKMD_BASE + CKMD_O_LFOSCCTL) = CKMD_LFOSCCTL_EN;

    /* Enable nanoamp bias */
    HWREG(CKMD_BASE + CKMD_O_NABIASCTL) = CKMD_NABIASCTL_EN;

    /* Enable LF clock monitoring */
    HWREG(CKMD_BASE + CKMD_O_LFMONCTL) = CKMD_LFMONCTL_EN;
}

/*
 *  ======== PowerLPF3_selectLFXT ========
 */
void PowerLPF3_selectLFXT(void)
{
    /* Set LFINC override to 32.768 kHz */
    HWREG(CKMD_BASE + CKMD_O_LFINCOVR) = 0x001E8480 | CKMD_LFINCOVR_OVERRIDE_M;

    /* Set LFCLK  */
    HWREG(CKMD_BASE + CKMD_O_LFCLKSEL) = CKMD_LFCLKSEL_MAIN_LFXT;

    /* Clear any existing interrupt flags */
    HWREG(CKMD_BASE + CKMD_O_IMCLR) = CKMD_IMCLR_LFCLKGOOD;

    /* Start LFXT */
    HWREG(CKMD_BASE + CKMD_O_LFXTCTL) = CKMD_LFXTCTL_EN;

    /* Enable LF clock monitoring */
    HWREG(CKMD_BASE + CKMD_O_LFMONCTL) = CKMD_LFMONCTL_EN;
}

/*
 *  ======== PowerCC23X0_oscillatorISR ========
 */
static void PowerCC27XX_oscillatorISR(uintptr_t arg)
{
    uint32_t maskedStatus = HWREG(CKMD_BASE + CKMD_O_MIS);

    /* Manipulating ICLR alone does not actually do anything. The CKM_COMB
     * signals are all level values that reset one cycle after writing to
     * ICLR. We need to update the mask instead to avoid looping in the ISR
     */
    HWREG(CKMD_BASE + CKMD_O_ICLR)  = maskedStatus;
    HWREG(CKMD_BASE + CKMD_O_IMCLR) = maskedStatus;

    if (maskedStatus & CKMD_MIS_AMPSETTLED_M)
    {
        PowerCC27XX_notify(PowerLPF3_HFXT_AVAILABLE);
    }
    else if (maskedStatus & CKMD_MIS_HFXTFAULT_M)
    {
        /* If there was a fault, restart HFXT. Consider issuing a notification
         * to allow logging. If we keep it like this, we could get stuck in an
         * infinite loop restarting a faulty oscillator. Then again, it is not
         * like there is a great way to recover from that.
         */
        HWREG(CKMD_BASE + CKMD_O_HFXTCTL) &= ~CKMD_HFXTCTL_EN_M;
        HWREG(CKMD_BASE + CKMD_O_HFXTCTL) |= CKMD_HFXTCTL_EN_M;
    }

    if (maskedStatus & CKMD_MIS_LFCLKGOOD_M)
    {
        PowerCC27XX_notify(PowerLPF3_LFCLK_SWITCHED);
    }
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
    HWREG(RTC_BASE + RTC_O_ICLR) = HWREG(RTC_BASE + RTC_O_MIS);
}

/*
 *  ======== notify ========
 *  Send notifications to registered clients.
 *  Note: Task scheduling is disabled when this function is called.
 */
int_fast16_t PowerCC27XX_notify(uint_fast16_t eventType)
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
            if (((Power_NotifyObj *)elem)->eventTypes & eventType)
            {
                /* Pull params from notify object */
                notifyFxn = ((Power_NotifyObj *)elem)->notifyFxn;
                clientArg = ((Power_NotifyObj *)elem)->clientArg;

                /* Call the client's notification function */
                notifyStatus = (int_fast16_t)(*(Power_NotifyFxn)notifyFxn)(eventType, 0, clientArg);

                /* If client declared error stop all further notifications */
                if (notifyStatus != Power_NOTIFYDONE)
                {
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
 *  ======== PowerCC27XX_setDependencyCount ========
 */
void PowerCC27XX_setDependencyCount(uint_fast16_t resourceId, uint8_t count)
{
    uint8_t bitIndex    = RESOURCE_BIT_INDEX(resourceId);
    uint_fast16_t group = RESOURCE_GROUP(resourceId);

    DebugP_assert(PowerCC27XX_isValidResourceId(resourceId));

    if (group == PowerCC27XX_PERIPH_GROUP_CLKCTL0)
    {
        resourceCountsClkctl0[bitIndex] = count;
    }
    else if (group == PowerCC27XX_PERIPH_GROUP_CLKCTL1)
    {
        resourceCountsClkctl1[bitIndex] = count;
    }
    else if (group == PowerCC27XX_PERIPH_GROUP_LRFD)
    {
        resourceCountsLrfd[bitIndex] = count;
    }
}

/*
 *  ======== PowerCC27XX_isValidResourceId ========
 */
bool PowerCC27XX_isValidResourceId(uint_fast16_t resourceId)
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
