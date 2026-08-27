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
 *  ======== SystemCC27XX_s.c ========
 *
 */

#include <stdbool.h>
#include "psa/service.h"

#include <ti/drivers/Power.h>
#include <ti/drivers/batterymonitor/BatMonSupportLPF3.h>
#include <ti/drivers/BatteryMonitor.h>
#include <ti/drivers/Temperature.h>
#include <ti/drivers/tfm/SecureCallback.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_ints.h)
#include DeviceFamily_constructPath(driverlib/pmctl.h)
#include DeviceFamily_constructPath(driverlib/ckmd.h)
#include DeviceFamily_constructPath(driverlib/evtsvt.h)

/* The range of pins available on this device
 * TODO: Make configurable
 */
const uint_least8_t GPIO_pinLowerBound = 0U;
const uint_least8_t GPIO_pinUpperBound = 28U;

/* SecureCallback config, used by SecureCallback_s_post() */
const SecureCallback_Config SecureCallback_config = {
    .intNumber   = INT_SW1,
    .intPriority = 0xFFU,
};

/* BatMonSupportLPF3 config, used by BatMonSupportLPF3_s.c */
const BatMonSupportLPF3_Config BatMonSupportLPF3_config = {
    .intNum          = INT_CPUIRQ2,
    .intPriority     = 0xFFFFFFFFU,
    .intSubscriberId = EVTSVT_SUB_CPUIRQ2,
};

void SystemCC27XX_init(void);
void System_sp_main(void);

/*
 *  ======== SystemCC27XX_init ========
 */
void SystemCC27XX_init(void)
{
#if CONFIG_SYSTEM_OVERRIDE_CAP_ARRAY == 1
    /* Apply cap array configuration, as defined in config_tfm_project.h */
    CKMDSetInitialCapTrim(CONFIG_SYSTEM_CAP_ARRAY_Q1, CONFIG_SYSTEM_CAP_ARRAY_Q2);
    CKMDSetTargetCapTrim(CONFIG_SYSTEM_CAP_ARRAY_Q1, CONFIG_SYSTEM_CAP_ARRAY_Q2);
#endif

    /* Initialize power driver. Ignore return value since it never returns an
     * error.
     */
    (void)Power_init();

    /* TODO: Make LFCLK and voltage regulator configurable */
    PowerLPF3_selectLFXT();
    PMCTLSetVoltageRegulator(PMCTL_VOLTAGE_REGULATOR_DCDC);
}

/*
 *  ======== System_sp_main ========
 */
void System_sp_main(void)
{
    (void)BatteryMonitor_init();
    (void)Temperature_init();
    /* Dummy event loop.
     * Continually wait for input signals. Don't handle any.
     */
    while (true)
    {
        (void)psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
    }
}
