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
void INT_CPUIRQ3_Handler(void);
void INT_HSM_OTP_IRQ_Handler(void);
void INT_HSM_SEC_IRQ_Handler(void);
enum tfm_hal_status_t cpuirq3_irqn_init(void *p_pt, const struct irq_load_info_t *p_ildi);
enum tfm_hal_status_t hsm_otp_irq_irqn_init(void *p_pt, const struct irq_load_info_t *p_ildi);
enum tfm_hal_status_t hsm_sec_irq_irqn_init(void *p_pt, const struct irq_load_info_t *p_ildi);
psa_flih_result_t cpuirq3_irqn_flih(void);
psa_flih_result_t hsm_secure_irqn_flih(void);
psa_flih_result_t hsm_otp_irqn_flih(void);

/* Globals to store interrupt info */
static struct irq_t ff_cpuirq3;
static struct irq_t ff_hsm_otp_irq;
static struct irq_t ff_hsm_secure_irq;

/*
 * Quantized default IRQ priority, the value is:
 * (Number of configurable priority) / 4: (1UL << __NVIC_PRIO_BITS) / 4
 */
#define DEFAULT_IRQ_PRIORITY (1UL << (__NVIC_PRIO_BITS - 2U))

/*
 *  ======== cpuirq3_irqn_init ========
 */
enum tfm_hal_status_t cpuirq3_irqn_init(void *p_pt, const struct irq_load_info_t *p_ildi)
{
    uint32_t targetState;
    enum tfm_hal_status_t status = TFM_HAL_SUCCESS;

    /* Store interrupt info */
    ff_cpuirq3.p_ildi = p_ildi;
    ff_cpuirq3.p_pt   = p_pt;

    NVIC_SetPriority(CPUIRQ3_IRQn, DEFAULT_IRQ_PRIORITY);
    /* Set interrupt as Secure */
    targetState = NVIC_ClearTargetState(CPUIRQ3_IRQn);

    /* Verify that target state is secure */
    if (targetState != 0U)
    {
        status = TFM_HAL_ERROR_GENERIC;
    }

    /* Disable interrupt */
    NVIC_DisableIRQ(CPUIRQ3_IRQn);

    return status;
}

/*
 *  ======== hsm_otp_irq_irqn_init ========
 */
enum tfm_hal_status_t hsm_otp_irq_irqn_init(void *p_pt, const struct irq_load_info_t *p_ildi)
{
    uint32_t targetState;
    enum tfm_hal_status_t status = TFM_HAL_SUCCESS;

    /* Store interrupt info */
    ff_hsm_otp_irq.p_ildi = p_ildi;
    ff_hsm_otp_irq.p_pt   = p_pt;

    NVIC_SetPriority(HSM_OTP_IRQ_IRQn, DEFAULT_IRQ_PRIORITY);
    /* Set interrupt as Secure */
    targetState = NVIC_ClearTargetState(HSM_OTP_IRQ_IRQn);

    /* Verify that target state is secure */
    if (targetState != 0U)
    {
        status = TFM_HAL_ERROR_GENERIC;
    }

    NVIC_DisableIRQ(HSM_OTP_IRQ_IRQn);

    return status;
}

/*
 *  ======== hsm_sec_irq_irqn_init ========
 */
enum tfm_hal_status_t hsm_sec_irq_irqn_init(void *p_pt, const struct irq_load_info_t *p_ildi)
{
    uint32_t targetState;
    enum tfm_hal_status_t status = TFM_HAL_SUCCESS;

    /* Store interrupt info */
    ff_hsm_secure_irq.p_ildi = p_ildi;
    ff_hsm_secure_irq.p_pt   = p_pt;

    NVIC_SetPriority(HSM_SEC_IRQ_IRQn, DEFAULT_IRQ_PRIORITY);
    /* Set interrupt as Secure */
    targetState = NVIC_ClearTargetState(HSM_SEC_IRQ_IRQn);

    /* Verify that target state is secure */
    if (targetState != 0U)
    {
        status = TFM_HAL_ERROR_GENERIC;
    }

    NVIC_DisableIRQ(HSM_SEC_IRQ_IRQn);

    return status;
}

/*
 *  ======== INT_CPUIRQ3_Handler ========
 */
void INT_CPUIRQ3_Handler(void)
{
    spm_handle_interrupt(ff_cpuirq3.p_pt, ff_cpuirq3.p_ildi);
}

/*
 *  ======== INT_HSM_OTP_IRQ_Handler ========
 */
void INT_HSM_OTP_IRQ_Handler(void)
{
    spm_handle_interrupt(ff_hsm_otp_irq.p_pt, ff_hsm_otp_irq.p_ildi);
}

/*
 *  ======== INT_HSM_SEC_IRQ_Handler ========
 */
void INT_HSM_SEC_IRQ_Handler(void)
{
    spm_handle_interrupt(ff_hsm_secure_irq.p_pt, ff_hsm_secure_irq.p_ildi);
}

/*
 *  ======== cpuirq3_irqn_flih ========
 */
psa_flih_result_t cpuirq3_irqn_flih(void)
{
    HwiP_dispatchInterrupt(INT_CPUIRQ3);

    return PSA_FLIH_NO_SIGNAL;
}

/*
 *  ======== hsm_secure_irqn_flih ========
 */
psa_flih_result_t hsm_secure_irqn_flih(void)
{
    HwiP_dispatchInterrupt(INT_HSM_SEC_IRQ);

    return PSA_FLIH_NO_SIGNAL;
}

/*
 *  ======== hsm_otp_irqn_flih ========
 */
psa_flih_result_t hsm_otp_irqn_flih(void)
{
    HwiP_dispatchInterrupt(INT_HSM_OTP_IRQ);

    return PSA_FLIH_NO_SIGNAL;
}
