/*
 * Copyright (c) 2025-2026, Texas Instruments Incorporated
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
#include <string.h>
#include <stdbool.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOLPF4.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/ioc.h)
#include DeviceFamily_constructPath(driverlib/gpio.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

static bool initCalled = false;

/* HW interrupt structure for I/O interrupt handler */
static HwiP_Struct gpioHwi;

/* Link to config values defined by sysconfig */
extern GPIO_Config GPIO_config;
extern const uint_least8_t GPIO_pinLowerBound;
extern const uint_least8_t GPIO_pinUpperBound;
extern const GPIOLPF4_ShuffleNetworkConfig GPIOLPF4_shuffleNetworkConfig;

/*
 *  ======== GPIO_clearInt ========
 */
void GPIO_clearInt(uint_least8_t index)
{
    if (index == GPIO_INVALID_INDEX)
    {
        return;
    }

    GPIOClearEventDio(index);
}

/*
 *  ======== GPIO_disableInt ========
 */
void GPIO_disableInt(uint_least8_t index)
{
    if (index == GPIO_INVALID_INDEX)
    {
        return;
    }

    GPIODisableEventDio(index);
}

/*
 *  ======== GPIO_enableInt ========
 */
void GPIO_enableInt(uint_least8_t index)
{
    if (index == GPIO_INVALID_INDEX)
    {
        return;
    }

    GPIOEnableEventDio(index);
}

/*
 *  ======== GPIO_hwiIntFxn ========
 *  Hwi function that processes GPIO interrupts.
 */
void GPIO_hwiIntFxn(uintptr_t arg)
{
    uint32_t flagIndex;
    uint64_t eventMask;

    /* Get and clear the interrupt mask */
    eventMask = GPIOGetEventMultiDio(GPIO_DIO_ALL_MASK, true);
    GPIOClearEventMultiDio(eventMask);

    uint32_t eventMask31_0  = (uint32_t)eventMask;
    uint32_t eventMask63_32 = (uint32_t)(eventMask >> 32);

    while (eventMask31_0)
    {
        /* MASK_TO_PIN only detects the highest set bit */
        flagIndex = GPIO_MASK_TO_PIN(eventMask31_0);

        /* So it's safe to use PIN_TO_MASK to clear that bit */
        eventMask31_0 &= ~GPIO_PIN_TO_MASK(flagIndex);

        /* Only invoke callback if this pin also has interrupts enabled in IOC
         * This is because event-flags can be generated independently of interrupts
         */
        if (GPIO_config.callbacks[flagIndex] != NULL)
        {
            GPIO_config.callbacks[flagIndex](flagIndex);
        }
    }

    /* Below loop should hopefully get optimized away for devices with 32 or
     * less DIOs since the GPIOGetEventMultiDio() driverlib function call will
     * get inlined and for the devices with 32 or less DIOs the upper word of
     * eventMask will always be 0.
     */
    while (eventMask63_32)
    {
        /* MASK_TO_PIN only detects the highest set bit */
        flagIndex = GPIO_MASK_TO_PIN(eventMask63_32);

        /* So it's safe to use PIN_TO_MASK to clear that bit */
        eventMask63_32 &= ~GPIO_PIN_TO_MASK(flagIndex);

        /* Only invoke callback if this pin also has interrupts enabled in IOC
         * This is because event-flags can be generated independently of interrupts
         */
        if (GPIO_config.callbacks[flagIndex + 32] != NULL)
        {
            GPIO_config.callbacks[flagIndex + 32](flagIndex + 32);
        }
    }
}

/*
 *  ======== GPIO_init ========
 */
void GPIO_init()
{
    uintptr_t key;
    unsigned int i;
    HwiP_Params hwiParams;
    uint64_t configMask       = 0x0ULL;
    uint64_t outputEnableMask = 0x0ULL;
    uint64_t intEnableMask    = 0x0ULL;

    key = HwiP_disable();

    if (initCalled)
    {
        HwiP_restore(key);
        return;
    }
    initCalled = true;
    HwiP_restore(key);

    Power_setDependency(PowerLPF4_PERIPH_GPIO);

    /* Setup HWI handler */
    HwiP_Params_init(&hwiParams);
    hwiParams.priority = GPIO_config.intPriority;
    HwiP_construct(&gpioHwi, INT_GPIO_COMB0, GPIO_hwiIntFxn, &hwiParams);

    for (i = GPIO_pinLowerBound; i <= GPIO_pinUpperBound; i++)
    {
        uint32_t pinConfig = GPIO_config.configs[i];

        /* Skip handling any DO_NOT_CONFIG IOs */
        if (pinConfig & GPIO_CFG_DO_NOT_CONFIG)
        {
            continue;
        }
        else
        {
            configMask |= (1ULL << i);
        }

        if (pinConfig & GPIOLPF4_CFG_PIN_IS_INPUT_INTERNAL)
        {
            if (pinConfig & GPIO_CFG_INT_ENABLE)
            {
                intEnableMask |= (1ULL << i);
            }
        }
        else
        {
            outputEnableMask |= 1ULL << i;
            GPIO_write(i, pinConfig & GPIO_CFG_OUT_HIGH ? 1 : 0);
        }

        /* Mask off the three mux bits, since they contain special configs */
        IOCSetConfigAndMux(i, pinConfig & GPIOLPF4_CFG_IOC_M, IOC_MUX_GPIO);
    }

    GPIOSetOutputEnableMultiDio(configMask, outputEnableMask);
    GPIOSetEventEnableMultiDio(configMask, intEnableMask);

    GPIOLPF4_applyShuffleNetworkConfig(&GPIOLPF4_shuffleNetworkConfig);
}

/*
 *  ======== GPIO_read ========
 */
uint_fast8_t GPIO_read(uint_least8_t index)
{
    return GPIOReadDio(index);
}

/*
 *  ======== GPIO_setConfig ========
 */
int_fast16_t GPIO_setConfig(uint_least8_t index, GPIO_PinConfig pinConfig)
{
    return GPIO_setConfigAndMux(index, pinConfig, GPIO_MUX_GPIO);
}

/*
 *  ======== GPIO_setInterruptConfig ========
 */
void GPIO_setInterruptConfig(uint_least8_t index, GPIO_PinConfig config)
{
    uintptr_t key;

    if (index == GPIO_INVALID_INDEX)
    {
        return;
    }

    /* Disable interrupt since IOCSetEdgeConfig() is not atomic */
    key = HwiP_disable();
    IOCSetEdgeDetectConfig(index, config & IOC_CONFIG_EDGE_DETECT_M);
    HwiP_restore(key);

    if (config & GPIO_CFG_INT_ENABLE)
    {
        GPIO_enableInt(index);
    }
    else
    {
        GPIO_disableInt(index);
    }
}

/*
 *  ======== GPIO_getConfig ========
 */
void GPIO_getConfig(uint_least8_t index, GPIO_PinConfig *pinConfig)
{
    /* Get IOC configuration.
     * The non-IOC configuration values will be written further below.
     */
    uint32_t configValue = IOCGetConfig(index);

    if (GPIOGetEventEnableDio(index))
    {
        configValue |= GPIO_CFG_INT_ENABLE;
    }

    if (GPIOGetOutputEnableDio(index) == GPIO_OUTPUT_DISABLE)
    {
        configValue |= GPIOLPF4_CFG_PIN_IS_INPUT_INTERNAL;
    }
    /* Only set this if the pin is an output */
    else if (GPIOReadDioOutputBuffer(index) ^ ((configValue & GPIO_CFG_INVERT_ON_INTERNAL) != 0))
    {
        configValue |= GPIO_CFG_OUTPUT_DEFAULT_HIGH_INTERNAL;
    }

    *pinConfig = configValue;
}

/*
 *  ======== GPIO_toggle ========
 */
void GPIO_toggle(uint_least8_t index)
{
    if (index == GPIO_INVALID_INDEX)
    {
        return;
    }

    GPIOToggleDio(index);
}

/*
 *  ======== GPIO_write ========
 */
void GPIO_write(uint_least8_t index, unsigned int value)
{
    if (index == GPIO_INVALID_INDEX)
    {
        return;
    }
    GPIOWriteDio(index, value & 0x1);
}

/*
 *  ======== GPIO_getMux ========
 */
uint32_t GPIO_getMux(uint_least8_t index)
{
    return IOCGetMux(index);
}

/*
 *  ======== GPIO_setConfigAndMux ========
 */
int_fast16_t GPIO_setConfigAndMux(uint_least8_t index, GPIO_PinConfig pinConfig, uint32_t mux)
{
    /* Return immediately if pin should not be configured */
    if (pinConfig & GPIO_CFG_DO_NOT_CONFIG)
    {
        return GPIO_STATUS_SUCCESS;
    }

    if (index == GPIO_INVALID_INDEX)
    {
        return GPIO_STATUS_ERROR;
    }

    bool setPinToOutput = !(pinConfig & GPIOLPF4_CFG_PIN_IS_INPUT_INTERNAL);

    /* The pin will be an output after configuring */
    if (setPinToOutput)
    {
        /* Set the new default value and enable output */
        GPIO_write(index, pinConfig & GPIO_CFG_OUT_HIGH ? 1 : 0);
        GPIOSetOutputEnableDio(index, GPIO_OUTPUT_ENABLE);
    }

    if (pinConfig & GPIO_CFG_INT_ENABLE)
    {
        GPIO_enableInt(index);
    }
    else
    {
        GPIO_disableInt(index);
    }

    /* Mask off the bits containing non-IOC configuration values and apply */
    GPIO_PinConfig tmpConfig = pinConfig & GPIOLPF4_CFG_IOC_M;
    IOCSetConfigAndMux(index, tmpConfig, mux);

    if (!setPinToOutput)
    {
        GPIOSetOutputEnableDio(index, GPIO_OUTPUT_DISABLE);
    }

    return GPIO_STATUS_SUCCESS;
}

/*
 *  ======== GPIOLPF4_computeShuffleNetworkConfig ========
 */
int_fast16_t GPIOLPF4_computeShuffleNetworkConfig(const GPIOLPF4_ShuffleNetworkRoutes *routes,
                                                  void *tmpBuffer,
                                                  GPIOLPF4_ShuffleNetworkConfig *config)
{
    /* Copy routes to temporary buffer. This is done since
     * IOCComputeShuffleNetworkConfig is modifying the data during the
     * computation.
     */
    memcpy(tmpBuffer, routes->routes, routes->size * sizeof(routes->routes[0]));

    IOCComputeShuffleNetworkConfig(tmpBuffer, routes->size, &(config->configRegs[0]));

    return GPIO_STATUS_SUCCESS;
}

/*
 *  ======== GPIOLPF4_applyShuffleNetworkConfig ========
 */
int_fast16_t GPIOLPF4_applyShuffleNetworkConfig(const GPIOLPF4_ShuffleNetworkConfig *config)
{
    IOCSetShuffleNetworkConfig(config->configRegs);
    return GPIO_STATUS_SUCCESS;
}