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

/*
 *  ======== ComparatorLPF3LP.c ========
 */

#include <ti/drivers/comparator/ComparatorLPF3LP.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOLPF3.h>

#include DeviceFamily_constructPath(inc/hw_evtsvt.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(driverlib/cpu.h)
#include DeviceFamily_constructPath(driverlib/hapi.h)

/* Settle time required by LPCMP to have a valid output after being enabled */
#define ComparatorLPF3LP_SETTLE_TIME 110

/* Function declarations */
void ComparatorLPF3LP_close(Comparator_Handle handle);
Comparator_OutputLevel ComparatorLPF3LP_getLevel(Comparator_Handle handle);
Comparator_Trigger ComparatorLPF3LP_getTrigger(Comparator_Handle handle);
void ComparatorLPF3LP_init(const Comparator_Handle handle);
Comparator_Handle ComparatorLPF3LP_open(Comparator_Handle handle, Comparator_Params *params);
int_fast16_t ComparatorLPF3LP_setTrigger(Comparator_Handle handle, Comparator_Trigger trigger);
int_fast16_t ComparatorLPF3LP_start(Comparator_Handle handle);
void ComparatorLPF3LP_stop(Comparator_Handle handle);

/* Comparator function table for ComparatorLPF3LP implementation */
const Comparator_FxnTable ComparatorLPF3LP_fxnTable = {ComparatorLPF3LP_close,
                                                       ComparatorLPF3LP_getLevel,
                                                       ComparatorLPF3LP_getTrigger,
                                                       ComparatorLPF3LP_init,
                                                       ComparatorLPF3LP_open,
                                                       ComparatorLPF3LP_setTrigger,
                                                       ComparatorLPF3LP_start,
                                                       ComparatorLPF3LP_stop};

/* Static Function Declarations */
/*
 * ========= ComparatorLPF3LP_hwiIntFxn ===========
 */
static void ComparatorLPF3LP_hwiIntFxn(uintptr_t arg)
{
    ComparatorLPF3LP_Object *object = ((Comparator_Handle)arg)->object;

    /* Clear pending interrupts and invoke user callback */
    LPCMPClearEvent();
    if (object->callbackFxn)
    {
        object->callbackFxn((Comparator_Handle)arg, Comparator_STATUS_SUCCESS, object->trigger);
    }
}

/*
 * ======== ComparatorLPF3LP_close =========
 */
void ComparatorLPF3LP_close(Comparator_Handle handle)
{
    uintptr_t key;
    ComparatorLPF3LP_Object *object = handle->object;

    key = HwiP_disable();

    if (object->isOpen == false)
    {
        HwiP_restore(key);
        return;
    }

    if (object->isRunning == true)
    {
        ComparatorLPF3LP_stop(handle);
        /* Consume potentially pending Comparator events that could have triggered while executing this critical
         * section. In this way we avoid an interrupt triggering while closing the Comparator instance.
         */
        LPCMPClearEvent();
    }

    object->isOpen = false;
    HwiP_restore(key);

    /* Destruct Comparator Hwi */
    HwiP_destruct(&object->hwi);
}

/*
 * ======= ComparatorLPF3LP_getLevel =======
 */
Comparator_OutputLevel ComparatorLPF3LP_getLevel(Comparator_Handle handle)
{
    bool outputHigh;
    uintptr_t key;
    ComparatorLPF3LP_Object *object = handle->object;

    /* Only return a valid value if the comparator has been started */
    key = HwiP_disable();
    if (object->isRunning == false)
    {
        HwiP_restore(key);
        return Comparator_OUTPUT_NOT_AVAILABLE;
    }

    /* Wrapping the reading of the output in a critical section is needed as an
     * interrupt could turn the Comparator off and make this value invalid
     */
    outputHigh = LPCMPIsOutputHigh();
    HwiP_restore(key);

    if (outputHigh == true)
    {

        return Comparator_OUTPUT_HIGH;
    }
    else
    {
        return Comparator_OUTPUT_LOW;
    }
}

/*
 * ====== ComparatorLPF3LP_getTrigger ======
 */
Comparator_Trigger ComparatorLPF3LP_getTrigger(Comparator_Handle handle)
{
    ComparatorLPF3LP_Object *object = handle->object;

    /* Verify if the instance has already been open */
    if (object->isOpen == false)
    {
        return Comparator_TRIGGER_NONE;
    }

    return object->trigger;
}

/*
 * ========= ComparatorLPF3LP_init =========
 */
void ComparatorLPF3LP_init(const Comparator_Handle handle)
{
    ComparatorLPF3LP_Object *object = handle->object;

    object->isOpen    = false;
    object->isRunning = false;
}

/*
 * ========= ComparatorLPF3LP_open =========
 */
Comparator_Handle ComparatorLPF3LP_open(Comparator_Handle handle, Comparator_Params *params)
{
    uintptr_t key;
    ComparatorLPF3LP_HWAttrs const *hwAttrs = handle->hwAttrs;
    ComparatorLPF3LP_Object *object         = handle->object;
    HwiP_Params hwiParams;
    uint32_t interruptConfig;

    /* Check if this instance is already in use */
    key = HwiP_disable();

    if (object->isOpen == true)
    {
        HwiP_restore(key);
        return NULL;
    }

    object->isOpen = true;
    HwiP_restore(key);

    /* Set up comparator HWI */
    HwiP_Params_init(&hwiParams);
    hwiParams.arg      = (uintptr_t)handle;
    hwiParams.priority = hwAttrs->intPriority;
    if (params->trigger == Comparator_TRIGGER_NONE)
    {
        hwiParams.enableInt = false;
    }
    else
    {
        /* Configure the interrupt line selected in SysConfig to output LPCMP events.
         * The offset calculation below implies that configurable interrupt lines
         * on Low Power F3 devices have:
         * - Configuration registers with contiguous addresses in memory.
         * - Consecutive interrupt numbers
         */
        HWREG(EVTSVT_BASE + EVTSVT_O_CPUIRQ0SEL + (hwAttrs->intNum - INT_CPUIRQ0) * sizeof(uint32_t)) = hwAttrs->intMux;

        hwiParams.enableInt = true;
    }

    HwiP_construct(&object->hwi, hwAttrs->intNum, &ComparatorLPF3LP_hwiIntFxn, &hwiParams);

    /* Map API interrupt level to LPF3 LPCMP interrupt configuration */
    switch (params->trigger)
    {
        case Comparator_TRIGGER_RISING:
            interruptConfig = LPCMP_POLARITY_RISE;
            break;
        case Comparator_TRIGGER_FALLING:
            interruptConfig = LPCMP_POLARITY_FALL;
            break;
        case Comparator_TRIGGER_NONE:
            break;
        default:
            /* Incorrect argument */
            HwiP_destruct(&object->hwi);
            object->isOpen = false;
            return NULL;
    }

    /* Set up object by copying in appropriate params and hwattrs */
    object->trigger     = params->trigger;
    object->callbackFxn = params->callbackFxn;

    /* Disable comparator interrupts during configuration */
    LPCMPDisableEvent();

    /* Set interrupt trigger polarity and enable wakeup in case event generation
     * is enabled in parameters
     */
    if (params->trigger != Comparator_TRIGGER_NONE)
    {
        LPCMPSetPolarity(interruptConfig);
        LPCMPEnableWakeup();
    }

    /* Configure positive input */
    /* In case we are using an external pin we must configure the related GPIO */
    if (hwAttrs->positiveInputChannel != ComparatorLPF3LP_POS_INPUT_VDDS)
    {
        GPIO_setConfigAndMux(hwAttrs->positiveInputPin, GPIO_CFG_NO_DIR, hwAttrs->positiveInputPinMux);
    }
    LPCMPSelectPositiveInput((uint32_t)hwAttrs->positiveInputChannel);

    /* Configure negative input */
    /* In case we are using an external pin we must configure the related GPIO */
    if ((hwAttrs->negativeInputChannel != ComparatorLPF3LP_NEG_INPUT_VDDS) &&
        (hwAttrs->negativeInputChannel != ComparatorLPF3LP_NEG_INPUT_VDDD))
    {
        GPIO_setConfigAndMux(hwAttrs->negativeInputPin, GPIO_CFG_NO_DIR, hwAttrs->negativeInputPinMux);
    }
    LPCMPSelectNegativeInput((uint32_t)hwAttrs->negativeInputChannel);

    /* Configure voltage divider */
    LPCMPSetDividerRatio((uint32_t)hwAttrs->voltageDividerFactor);
    LPCMPSetDividerPath((uint32_t)hwAttrs->voltageDividerPath);

    return handle;
}

/*
 * ====== ComparatorLPF3LP_setTrigger ======
 */
int_fast16_t ComparatorLPF3LP_setTrigger(Comparator_Handle handle, Comparator_Trigger trigger)
{
    ComparatorLPF3LP_HWAttrs const *hwAttrs = handle->hwAttrs;
    ComparatorLPF3LP_Object *object         = handle->object;
    uint32_t interruptConfig;

    /* Map API interrupt level to LPCMP interrupt configuration */
    switch (trigger)
    {
        case Comparator_TRIGGER_RISING:
            interruptConfig = LPCMP_POLARITY_RISE;
            break;
        case Comparator_TRIGGER_FALLING:
            interruptConfig = LPCMP_POLARITY_FALL;
            break;
        case Comparator_TRIGGER_NONE:
            break;
        default:
            /* Incorrect argument */
            return Comparator_STATUS_ERROR;
    }

    /* Disable event generation while re-configuring comparator */
    LPCMPDisableEvent();

    /* Temporarily disable the comparator in case it is already running */
    if (object->isRunning == true)
    {
        LPCMPDisable();
    }

    if (trigger == Comparator_TRIGGER_NONE)
    {
        HwiP_disableInterrupt(hwAttrs->intNum);
        LPCMPDisableWakeup();
    }
    else
    {
        LPCMPSetPolarity(interruptConfig);
        LPCMPEnableWakeup();

        /* In case interrupts were disabled we need to enable the constructed Hwi and mux the interrupt line to LPCMP */
        if (object->trigger == Comparator_TRIGGER_NONE)
        {
            HwiP_enableInterrupt(hwAttrs->intNum);
            HWREG(EVTSVT_BASE + EVTSVT_O_CPUIRQ0SEL + (hwAttrs->intNum - INT_CPUIRQ0) * sizeof(uint32_t)) = hwAttrs->intMux;
        }
    }
    object->trigger = trigger;

    /* Re-enable the comparator in case it was previously running.
     * As it is done in ComparatorLPF3LP_start, we need to wait for 110us after
     * enabling the Comparator for its output to settle. Only at this point we can
     * clear the false event potentially generated.
     */
    if (object->isRunning == true)
    {
        LPCMPEnable();
        HapiWaitUs(ComparatorLPF3LP_SETTLE_TIME);
    }
    LPCMPClearEvent();

    /* Re-enable event generation if set to trigger on a specific edge */
    if (trigger != Comparator_TRIGGER_NONE)
    {
        LPCMPEnableEvent();
    }

    return Comparator_STATUS_SUCCESS;
}

/*
 * ======== ComparatorLPF3LP_start =========
 */
int_fast16_t ComparatorLPF3LP_start(Comparator_Handle handle)
{
    uintptr_t key;
    ComparatorLPF3LP_Object *object = handle->object;

    /* Make sure the comparator is opened and not running */
    key = HwiP_disable();
    if ((object->isRunning == true) || (object->isOpen == false))
    {
        HwiP_restore(key);
        return Comparator_STATUS_ERROR;
    }
    object->isRunning = true;
    HwiP_restore(key);

    /* Enable the comparator and event generation.
     * When enabling the module, if the positive input is above the negative input
     * threshold and the trigger is set on rising edge, an event is immediately
     * generated. A delay of 110us is needed to allow for the Comparator output
     * to settle and the potential event to be recorded before we can clear it.
     * Only then we can enable event generation towards the CPU.
     */
    LPCMPEnable();
    HapiWaitUs(ComparatorLPF3LP_SETTLE_TIME);
    LPCMPClearEvent();
    if (object->trigger != Comparator_TRIGGER_NONE)
    {
        LPCMPEnableEvent();
    }

    return Comparator_STATUS_SUCCESS;
}

/*
 * ========= ComparatorLPF3LP_stop =========
 */
void ComparatorLPF3LP_stop(Comparator_Handle handle)
{
    uintptr_t key;
    ComparatorLPF3LP_Object *object = handle->object;

    /* Disable interrupts while re-configuring comparator */
    key = HwiP_disable();
    if (object->isRunning == false)
    {
        HwiP_restore(key);
        return;
    }
    /* Disable event generation and comparator module.
     * If the output of the Comparator is high and the module is set to generate
     * events on falling edge, a false event will be triggered upon disabling the
     * module. However, events have been disabled before turning off the Comparator
     * so the Hwi won't be executed. The flag event will still be set, but will
     * be cleared when ComparatorLPF3LP_start is called before enabling event
     * generation. For this reason it is safe not to wait for one SCLK_LF clock
     * cycle here (LPCMP is clocked at 32KHz, therefore it updates its
     * output and generate events every ~31us) for the event to be generated and
     * then cleared.
     */
    LPCMPDisableEvent();
    LPCMPDisable();
    object->isRunning = false;
    HwiP_restore(key);
}
