/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon
 * company) or an affiliate of Cypress Semiconductor Corporation. All rights
 * reserved.
 * Copyright (c) 2023-2025, Texas Instruments Incorporated. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include "cmsis.h"
#include "spm.h"
#include "tfm_hal_interrupt.h"
#include "tfm_peripherals_def.h"
#include "interrupt.h"
#include "load/interrupt_defs.h"
#include "platform_irq.h"

#include <ti/drivers/dpl/HwiP.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_ints.h)

/* Function prototypes */
void INT_CPUIRQ2_Handler(void);
enum tfm_hal_status_t cpuirq2_irqn_init(void *p_pt, const struct irq_load_info_t *p_ildi);
psa_flih_result_t cpuirq2_irqn_flih(void);

/* Globals to store interrupt info */
static struct irq_t ff_cpuirq2;

/*
 * Quantized default IRQ priority, the value is:
 * (Number of configurable priority) / 4: (1UL << __NVIC_PRIO_BITS) / 4
 */
#define DEFAULT_IRQ_PRIORITY (1UL << (__NVIC_PRIO_BITS - 2U))

enum tfm_hal_status_t cpuirq2_irqn_init(void *p_pt, const struct irq_load_info_t *p_ildi)
{
    uint32_t targetState;
    enum tfm_hal_status_t status = TFM_HAL_SUCCESS;
    /* Store interrupt info */
    ff_cpuirq2.p_ildi = p_ildi;
    ff_cpuirq2.p_pt   = p_pt;

    NVIC_SetPriority(CPUIRQ2_IRQn, DEFAULT_IRQ_PRIORITY);
    /* Set interrupt as Secure */
    targetState = NVIC_ClearTargetState(CPUIRQ2_IRQn);

    /* Verify that target state is secure */
    if (targetState != 0U)
    {
        status = TFM_HAL_ERROR_GENERIC;
    }

    /* Disable interrupt */
    NVIC_DisableIRQ(CPUIRQ2_IRQn);

    return status;
}

void INT_CPUIRQ2_Handler(void)
{
    spm_handle_interrupt(ff_cpuirq2.p_pt, ff_cpuirq2.p_ildi);
}

/*
 *  ======== cpuirq2_irqn_flih ========
 */
psa_flih_result_t cpuirq2_irqn_flih(void)
{
    HwiP_dispatchInterrupt(INT_CPUIRQ2);

    return PSA_FLIH_NO_SIGNAL;
}
