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

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOLPF3.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_ioc.h)
#include DeviceFamily_constructPath(inc/hw_gpio.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_pmctl.h)

static bool initCalled = false;

/* HW interrupt structure for I/O interrupt handler */
static HwiP_Struct gpioHwi;

/* Link to config values defined by sysconfig */
extern GPIO_Config GPIO_config;
extern const uint_least8_t GPIO_pinLowerBound;
extern const uint_least8_t GPIO_pinUpperBound;

/* Local helper function declarations */
static void setPinmaskNonatomic(uint_least8_t index, uint32_t registerBaseAddress);

#define IOC_BASE_PIN_REG 0x00000100
#define IOC_ADDR(index)  (IOC_BASE + IOC_BASE_PIN_REG + (sizeof(uint32_t) * index))

/*
 *  ======== GPIO_clearInt ========
 */
void GPIO_clearInt(uint_least8_t index)
{
    if (index == GPIO_INVALID_INDEX)
    {
        return;
    }

    setPinmaskNonatomic(index, GPIO_O_ICLR);
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

    setPinmaskNonatomic(index, GPIO_O_IMCLR);
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

    /* Clear any pending events before enabling the mask. It is possible under
     * some circumstances for edge events to be cached in RIS, even if detection
     * is disabled. We want to make sure these events are not dispatched.
     */
    GPIO_clearInt(index);
    setPinmaskNonatomic(index, GPIO_O_IMSET);
}

/*
 *  ======== GPIO_hwiIntFxn ========
 *  Hwi function that processes GPIO interrupts.
 */
void GPIO_hwiIntFxn(uintptr_t arg)
{
    uint32_t flagIndex;
    uint32_t eventMask;

    /* Get and clear the interrupt mask */
    eventMask                      = HWREG(GPIO_BASE + GPIO_O_MIS);
    HWREG(GPIO_BASE + GPIO_O_ICLR) = eventMask;

    while (eventMask)
    {
        /* MASK_TO_PIN only detects the highest set bit */
        flagIndex = GPIO_MASK_TO_PIN(eventMask);

        /* So it's safe to use PIN_TO_MASK to clear that bit */
        eventMask &= ~GPIO_PIN_TO_MASK(flagIndex);

        if (GPIO_config.callbacks[flagIndex] != NULL)
        {
            GPIO_config.callbacks[flagIndex](flagIndex);
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
    uint32_t outputEnableMask = 0x0;
    uint32_t intEnableMask    = 0x0;

    key = HwiP_disable();

    if (initCalled)
    {
        HwiP_restore(key);
        return;
    }
    initCalled = true;
    HwiP_restore(key);

    Power_setDependency(PowerLPF3_PERIPH_GPIO);

#ifdef DeviceFamily_CC27XX
    /* Enable pad power to use GPIOs by setting VDDIOPGIO. This is only done
     * for CC27XX to support split rails.
     */
    HWREG(PMCTL_BASE + PMCTL_O_AONRSET1) |= PMCTL_AONRSET1_VDDIOPGIO_SET;
#endif

    /* Setup HWI handler */
    HwiP_Params_init(&hwiParams);
    hwiParams.priority = GPIO_config.intPriority;
    HwiP_construct(&gpioHwi, INT_GPIO_COMB, GPIO_hwiIntFxn, &hwiParams);

    for (i = GPIO_pinLowerBound; i <= GPIO_pinUpperBound; i++)
    {
        uint32_t pinConfig = GPIO_config.configs[i];

        /* Skip handling any DO_NOT_CONFIG IOs */
        if (pinConfig & GPIO_CFG_DO_NOT_CONFIG)
        {
            continue;
        }

        if (pinConfig & GPIOLPF3_CFG_PIN_IS_INPUT_INTERNAL)
        {
            if (pinConfig & GPIO_CFG_INT_ENABLE)
            {
                intEnableMask |= (1 << i);
            }
        }
        else
        {
            outputEnableMask |= 1 << i;
            GPIO_write(i, pinConfig & GPIO_CFG_OUT_HIGH ? 1 : 0);
        }

        /* Mask off the three mux bits, since they contain special configs */
        HWREG(IOC_ADDR(i)) = pinConfig & GPIOLPF3_CFG_IOC_M;
    }

    HWREG(GPIO_BASE + GPIO_O_DOE31_0) = outputEnableMask;
    HWREG(GPIO_BASE + GPIO_O_IMASK)   = intEnableMask;
}

/*
 *  ======== GPIO_read ========
 */
uint_fast8_t GPIO_read(uint_least8_t index)
{
    return HWREGB(GPIO_BASE + GPIO_O_DIN3_0 + index);
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

    /* We access the third byte to modify only bits 17 and 18 */
    uint32_t iocfgRegAddr = IOC_ADDR(index) + 2;

    /* Shift down and mask away all non-interrupt configuration */
    uint8_t maskedConfig = ((config & IOC_IOC3_EDGEDET_M) >> IOC_IOC3_EDGEDET_S);

    /* Invert the edge detection mask, mask out current interrupt config and apply the new one */
    key                           = HwiP_disable();
    uint8_t currentRegisterConfig = HWREGB(iocfgRegAddr) & (uint8_t)(~(IOC_IOC3_EDGEDET_M >> IOC_IOC3_EDGEDET_S));
    HWREGB(iocfgRegAddr)          = currentRegisterConfig | maskedConfig;
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
    uint32_t iocfgRegAddr = IOC_ADDR(index);
    uint32_t mask         = GPIO_PIN_TO_MASK(index);

    /* Mask off the bits reserved for non-IOC configuration values.
     * The non-IOC configuration values will be written further below.
     */
    uint32_t configValue = HWREG(iocfgRegAddr) & GPIOLPF3_CFG_IOC_M;

    if (HWREGB(GPIO_BASE + GPIO_O_IMASK) & mask)
    {
        configValue |= GPIO_CFG_INT_ENABLE;
    }

    if ((HWREGB(GPIO_BASE + GPIO_O_DOE31_0) & mask) == 0)
    {
        configValue |= GPIOLPF3_CFG_PIN_IS_INPUT_INTERNAL;
    }
    /* Only set this if the pin is an output */
    else if (GPIO_read(index) ^ ((configValue & GPIO_CFG_INVERT_ON_INTERNAL) != 0))
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

    HWREGB(GPIO_BASE + GPIO_O_DOUTTGL3_0 + index) = 1;
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

    HWREGB(GPIO_BASE + GPIO_O_DOUT3_0 + index) = (value & 0x1);
}

/*
 * ======== setPinmaskNonatomic ========
 * Use this to set a single bit in a W1C (Write One to Clear) register
 * Doesn't disable interrupts, because concurrency issues don't impact W1C regs
 */
static void setPinmaskNonatomic(uint_least8_t index, uint32_t registerBaseAddress)
{
    uint32_t mask                          = GPIO_PIN_TO_MASK(index);
    HWREG(GPIO_BASE + registerBaseAddress) = mask;
}

/*
 *  ======== GPIO_getMux ========
 */
uint32_t GPIO_getMux(uint_least8_t index)
{
    uint32_t iocfgRegAddr = IOC_ADDR(index);
    return HWREG(iocfgRegAddr) & 0x07;
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

    uint32_t iocfgRegAddr = IOC_ADDR(index);
    bool setPinToOutput   = !(pinConfig & GPIOLPF3_CFG_PIN_IS_INPUT_INTERNAL);

    /* The pin will be an output after configuring */
    if (setPinToOutput)
    {
        /* Set the new default value and enable output */
        GPIO_write(index, pinConfig & GPIO_CFG_OUT_HIGH ? 1 : 0);
        setPinmaskNonatomic(index, GPIO_O_DOESET31_0);
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
    GPIO_PinConfig tmpConfig = pinConfig & GPIOLPF3_CFG_IOC_M;
    HWREG(iocfgRegAddr)      = tmpConfig | mux;

    if (!setPinToOutput)
    {
        setPinmaskNonatomic(index, GPIO_O_DOECLR31_0);
    }

    return GPIO_STATUS_SUCCESS;
}
