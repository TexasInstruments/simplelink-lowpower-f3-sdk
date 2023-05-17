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

#include <stdint.h>

/* Kernel services */
#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/HwiP.h>

/* TI drivers */
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCLPF3.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOLPF3.h>
#include <ti/drivers/Power.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(driverlib/adc.h)
#include DeviceFamily_constructPath(driverlib/cpu.h)

/*
 * =============================================================================
 * Public Function Declarations
 * =============================================================================
 */
void ADCLPF3_close(ADC_Handle handle);
void ADCLPF3_init(ADC_Handle handle);
ADC_Handle ADCLPF3_open(ADC_Handle handle, ADC_Params *params);
int_fast16_t ADCLPF3_convert(ADC_Handle handle, uint16_t *value);
int_fast16_t ADCLPF3_convertChain(ADC_Handle *handleList, uint16_t *dataBuffer, uint8_t channelCount);
int_fast16_t ADCLPF3_control(ADC_Handle handle, uint_fast16_t cmd, void *arg);
uint32_t ADCLPF3_convertToMicroVolts(ADC_Handle handle, uint16_t adcValue);

/*
 * =============================================================================
 * Private Function Declarations
 * =============================================================================
 */

/*
 * =============================================================================
 * Constants
 * =============================================================================
 */

/* ADC function table for ADCLPF3 implementation */
const ADC_FxnTable ADCLPF3_fxnTable = {ADCLPF3_close,
                                       ADCLPF3_control,
                                       ADCLPF3_convert,
                                       ADCLPF3_convertChain,
                                       ADCLPF3_convertToMicroVolts,
                                       ADCLPF3_init,
                                       ADCLPF3_open};

/*
 * =============================================================================
 * Private Global Variables
 * =============================================================================
 */

/* Keep track of the adc handle instance to create and delete adcSemaphore */
static uint16_t adcInstance = 0;

/* Semaphore to arbitrate access to the single ADC peripheral between multiple handles */
static SemaphoreP_Struct adcSemaphore;

/*
 * =============================================================================
 * Function Definitions
 * =============================================================================
 */

/*
 *  ======== ADCLPF3_init ========
 */
void ADCLPF3_init(ADC_Handle handle)
{
    ADCLPF3_Object *object;

    /* Get the object */
    object = handle->object;

    /* Mark the object as available */
    object->isOpen = false;
}

/*
 *  ======== ADCLPF3_open ========
 */
ADC_Handle ADCLPF3_open(ADC_Handle handle, ADC_Params *params)
{
    ADCLPF3_Object *object;
    ADCLPF3_HWAttrs const *hwAttrs;

    DebugP_assert(handle);

    /* Get object and hwAttrs */
    object  = handle->object;
    hwAttrs = handle->hwAttrs;

    /* Determine if the driver was already opened */
    uint32_t key = HwiP_disable();

    if (object->isOpen)
    {
        DebugP_log0("ADC: Error! Already in use.");
        HwiP_restore(key);
        return NULL;
    }
    object->isOpen = true;

    /* Remember thread safety protection setting */
    object->isProtected = params->isProtected;

    /* If this is the first handle requested, set up the semaphore as well */
    if (adcInstance == 0)
    {
        /* Setup semaphore */
        SemaphoreP_constructBinary(&adcSemaphore, 1);
    }
    adcInstance++;

    /* Register power dependency - i.e. power up and enable clock for ADC */
    Power_setDependency(PowerLPF3_PERIPH_ADC0);

    HwiP_restore(key);

    /* Set pins to analog function. If pin is unused, value is set to GPIO_INVALID_INDEX */
    GPIO_setConfigAndMux(hwAttrs->adcInputDIO, GPIO_CFG_INPUT, GPIO_MUX_PORTCFG_PFUNC6);
    GPIO_setConfigAndMux(hwAttrs->adcRefPosDIO, GPIO_CFG_INPUT, GPIO_MUX_PORTCFG_PFUNC6);
    GPIO_setConfigAndMux(hwAttrs->adcRefNegDIO, GPIO_CFG_INPUT, GPIO_MUX_PORTCFG_PFUNC6);

    DebugP_log0("ADC: Object opened");

    return handle;
}

/*
 *  ======== ADCLPF3_close ========
 */
void ADCLPF3_close(ADC_Handle handle)
{
    ADCLPF3_Object *object;

    DebugP_assert(handle);

    object = handle->object;

    uint32_t key = HwiP_disable();

    if (object->isOpen)
    {
        adcInstance--;
        if (adcInstance == 0)
        {
            SemaphoreP_destruct(&adcSemaphore);
        }

        /* Remove power dependency */
        Power_releaseDependency(PowerLPF3_PERIPH_ADC0);
        DebugP_log0("ADC: Object closed");
    }

    object->isOpen = false;
    HwiP_restore(key);
}

/*
 *  ======== ADCLPF3_control ========
 */
int_fast16_t ADCLPF3_control(ADC_Handle handle, uint_fast16_t cmd, void *arg)
{
    /* No implementation yet */
    return ADC_STATUS_UNDEFINEDCMD;
}

/*
 *  ======== ADCLPF3_convert ========
 */
int_fast16_t ADCLPF3_convert(ADC_Handle handle, uint16_t *value)
{
    ADCLPF3_HWAttrs const *hwAttrs;
    ADCLPF3_Object *object;
    int_fast16_t conversionResult = ADC_STATUS_SUCCESS;
    uint32_t conversionValue;
    uint32_t interruptStatus;

    DebugP_assert(handle);

    /* Get handle */
    hwAttrs = handle->hwAttrs;

    /* Get the object */
    object = handle->object;

    if (object->isProtected)
    {
        /* Acquire the lock for this particular ADC handle */
        SemaphoreP_pend(&adcSemaphore, SemaphoreP_WAIT_FOREVER);
    }

    /* Set constraints to guarantee operation */
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    /* Specify range of ctrl-registers for conversion. Use ctrl-register 0 */
    ADCSetMemctlRange(0, 0);

    /* Set clock-divider and sampling duration */
    ADCSetSampleDuration(hwAttrs->adcClkkDivider, hwAttrs->samplingDuration);

    /* Set resolution */
    ADCSetResolution(hwAttrs->resolutionBits);

    /* Set reference source */
    ADCSetInput(hwAttrs->refSource, hwAttrs->internalChannel, 0);

    /* Pass correct offset-value to ADC peripheral, depending on reference source */
    ADCSetAdjustmentOffset(hwAttrs->refSource);

    /* Trigger a conversion */
    ADCManualTrigger();

    /* There is a delay of 9 cycles between writing the SC_START bit, and
     * the BUSY-bit going high. If we start polling too early, we will miss it.
     * delay 3 loops, where each loop is minimum 3 cycles.
     */
    CPUDelay(3);

    /* Read out conversion (blocking while ADC is busy) */
    conversionValue = ADCReadResult(0);

    /* Check if something went wrong. (Underflow or overflow) */
    interruptStatus = ADCRawInterruptStatus();

    if (interruptStatus & (ADC_INT_OVIFG | ADC_INT_UVIFG))
    {
        conversionResult = ADC_STATUS_ERROR;
    }

    /* Clear all interrupts generated by ADC */
    ADCClearInterrupt(0xFFFFFFFF);

    /* Allow entering standby again after ADC conversion complete */
    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

    if (object->isProtected)
    {
        /* Release the lock for this particular ADC handle */
        SemaphoreP_post(&adcSemaphore);
    }

    /* If we want to return the trimmed value, calculate it here. */
    if (hwAttrs->returnAdjustedVal)
    {
        uint16_t gain   = ADCGetAdjustmentGain(hwAttrs->refSource);
        conversionValue = ADCAdjustValueForGain(conversionValue, hwAttrs->resolutionBits, gain);
    }

    *value = conversionValue;

    /* Return the status-code of the conversion */
    return conversionResult;
}

/*
 *  ======== ADCLPF3_convertChain ========
 */
int_fast16_t ADCLPF3_convertChain(ADC_Handle *handleList, uint16_t *dataBuffer, uint8_t channelCount)
{
    ADCLPF3_HWAttrs const *hwAttrs;
    int_fast16_t conversionResult = ADC_STATUS_SUCCESS;
    uint32_t conversionValue;
    uint32_t interruptStatus;

    /* Acquire the lock used arbitrate access to the ADC peripheral
     * between multiple handles.
     */
    SemaphoreP_pend(&adcSemaphore, SemaphoreP_WAIT_FOREVER);

    /* Set constraints to guarantee operation */
    Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

    /* Specify range of ctrl-registers for conversion */
    ADCSetMemctlRange(0, 0);

    for (uint32_t i = 0; i < channelCount; i++)
    {
        DebugP_assert(handleList[i]);

        /* Get the particular handle */
        hwAttrs = handleList[i]->hwAttrs;

        /* Set clock-divider and sampling duration */
        ADCSetSampleDuration(hwAttrs->adcClkkDivider, hwAttrs->samplingDuration);

        /* Set resolution */
        ADCSetResolution(hwAttrs->resolutionBits);

        /* Set reference source */
        ADCSetInput(hwAttrs->refSource, hwAttrs->internalChannel, 0);

        /* Pass correct offset-value to ADC peripheral, depending on reference source */
        ADCSetAdjustmentOffset(hwAttrs->refSource);

        /* Trigger a conversion */
        ADCManualTrigger();

        /* There is a delay of 9 cycles between writing the SC_START bit, and
         * the BUSY-bit going high. If we start polling too early, we will miss it.
         * delay 3 loops, where each loop is minimum 3 cycles.
         */
        CPUDelay(3);

        /* Read out conversion (blocking while ADC is busy) */
        conversionValue = ADCReadResult(0);

        /* If we want to return the trimmed value, calculate it here. */
        if (hwAttrs->returnAdjustedVal)
        {
            uint16_t gain   = ADCGetAdjustmentGain(hwAttrs->refSource);
            conversionValue = ADCAdjustValueForGain(conversionValue, hwAttrs->resolutionBits, gain);
        }

        dataBuffer[i] = conversionValue;
    }

    /* Check if something went wrong. (Underflow or overflow) */
    interruptStatus = ADCRawInterruptStatus();

    if (interruptStatus & (ADC_INT_OVIFG | ADC_INT_UVIFG))
    {
        conversionResult = ADC_STATUS_ERROR;
    }

    /* Clear all interrupts generated by ADC */
    ADCClearInterrupt(0xFFFFFFFF);

    /* Allow entering standby again after ADC conversion complete */
    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);

    /* Release the lock used arbitrate access to the single ADC peripheral
     * between multiple handles.
     */
    SemaphoreP_post(&adcSemaphore);

    /* Return the status-code of the conversion */
    return conversionResult;
}

/*
 *  ======== ADCLPF3_convertToMicroVolts ========
 */
uint32_t ADCLPF3_convertToMicroVolts(ADC_Handle handle, uint16_t adcValue)
{
    ADCLPF3_HWAttrs const *hwAttrs;
    uint32_t adjustedValue;
    uint32_t microVolts;

    DebugP_assert(handle);

    /* Get the pointer to the hwAttrs */
    hwAttrs = handle->hwAttrs;

    /* If returnAdjustedVal is set, the raw value has already been adjusted.
     * If not, we need to adjust it here. (This is why the logic here appears inverted)
     */
    if (hwAttrs->returnAdjustedVal)
    {
        adjustedValue = adcValue;
    }
    else
    {
        uint16_t gain = ADCGetAdjustmentGain(hwAttrs->refSource);
        adjustedValue = ADCAdjustValueForGain(adcValue, hwAttrs->resolutionBits, gain);
    }

    microVolts = ADCValueToMicrovolts(adjustedValue, hwAttrs->resolutionBits, hwAttrs->refVoltage);

    return microVolts;
}
