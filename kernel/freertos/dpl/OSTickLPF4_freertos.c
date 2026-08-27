/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
 *  ======== OSTickLPF4_freertos.c ========
 */

#include <stdbool.h>

/* Driver header files */
#include <ti/drivers/Power.h>

#include "OSTickLPF4.h"

/* FreeRTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

/* Driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(cmsis/core/cmsis_compiler.h)
#include DeviceFamily_constructPath(driverlib/evtsvt.h)
#include DeviceFamily_constructPath(driverlib/systimer.h)

/* Number of microseconds between each FreeRTOS OS tick */
#define FREERTOS_TICKPERIOD_US (1000000 / configTICK_RATE_HZ)

#if ((FREERTOS_TICKPERIOD_US * configTICK_RATE_HZ) != 1000000)
    #error "configTICK_RATE_HZ must divide 1000000 evenly"
#endif

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC283X)
    #define CLOCK_FREQUENCY_DIVIDER (96000000U / configCPU_CLOCK_HZ)
#else
    #error "Unsupported Device Family"
#endif

static void OSTickLPF4_freertosTickIsr(void);

/*
 *  ======== vPortSetupTimerInterrupt ========
 */
void vPortSetupTimerInterrupt(void)
{
    /* Get current SysTimer value */
    uint32_t currentTime1Us = SysTimerGetTime1Us();

    /* Un-arm the the FreeRTOS Tick SysTimer channel to put it in a known
     * state.
     */
    SysTimerDisarmChannel(OSTickLPF4_TICK_SYSTIM_CHANNEL);

    /* Clear any pending interrupts on the FreeRTOS Tick SysTimer channel. */
    SysTimerClearInterrupt(OSTickLPF4_TICK_SYSTIM_CHANNEL);

    /* Configure the FreeRTOS Tick SysTimer channel to compare mode with timer
     * resolution of 1 us.
     */
    SysTimerSetChannelConfig(OSTickLPF4_TICK_SYSTIM_CHANNEL,
                             SYSTIMER_CONFIG_MODE_IDLE | SYSTIMER_CONFIG_RESOLUTION_1US);

    /* Plug the FreeRTOS Tick ISR directly into the vector table to reduce
     * overhead. This is possible because the ISR already yields if a context
     * switch is required.
     */
    HwiP_plug(OSTickLPF4_TICK_INT, (void *)OSTickLPF4_freertosTickIsr);

    /* Mux the SysTimer event for SysTimer channel OSTickLPF4_TICK_SYSTIM_CHANNEL
     * to interrupt line OSTickLPF4_TICK_INT.
     */
    EVTSVTConfigureEvent(OSTickLPF4_TICK_EVTSVT_SUB, OSTickLPF4_TICK_EVTSVT_PUB);

    /* Clear the IMASK for FreeRTOS Tick channel. IMASK is used by the power
     * driver to know which systimer channels are active, but the FreeRTOS Tick
     * SysTimer channel is a special case, that the Power driver is handling
     * differently.
     */
    SysTimerDisableInterrupt(OSTickLPF4_TICK_SYSTIM_CHANNEL);

    /* Set compare value for the FreeRTOS Tick SysTimer channel. This will arm
     * the channel.
     */
    SysTimerSetCompareValue(OSTickLPF4_TICK_SYSTIM_CHANNEL,
                            currentTime1Us + (FREERTOS_TICKPERIOD_US / CLOCK_FREQUENCY_DIVIDER));

    /* Enable the SysTimer interrupt. */
    HwiP_enableInterrupt(OSTickLPF4_TICK_INT);
}

/*
 *  ======== OSTickLPF4_freertosTickIsr ========
 */
static void OSTickLPF4_freertosTickIsr(void)
{
    /* Disable interrupts. */
    uintptr_t key = __get_PRIMASK();
    __set_PRIMASK(1);

    /* FreeRTOS Tick is using dedicated SysTimer channel event. Clearing the
     * flag for the combined interrupt is strictly not necessary, but doing it
     * here to avoid confusion for anyone using the SysTimer combined event.
     */
    SysTimerClearInterrupt(OSTickLPF4_TICK_SYSTIM_CHANNEL);

    /* Set the next compare value */
    SysTimerSetCompareValue(OSTickLPF4_TICK_SYSTIM_CHANNEL,
                            SysTimerGetCaptureCompareValue(OSTickLPF4_TICK_SYSTIM_CHANNEL) +
                                (FREERTOS_TICKPERIOD_US / CLOCK_FREQUENCY_DIVIDER));

    /* Increment the FreeRTOS tick and determine if a switch is required. */
    BaseType_t switchRequired = xTaskIncrementTick();

    /* Yield, if switch is required. */
    portYIELD_FROM_ISR(switchRequired);

    /* Restore interrupts. */
    __set_PRIMASK(key);
}