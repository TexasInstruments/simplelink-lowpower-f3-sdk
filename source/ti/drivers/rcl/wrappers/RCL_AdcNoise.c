/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

#include <ti/devices/DeviceFamily.h>

#include <ti/drivers/rcl/hal/hal.h>
#include <ti/drivers/rcl/RCL.h>
#include <ti/drivers/rcl/RCL_Scheduler.h>
#include <ti/drivers/rcl/commands/adc_noise.h>
#include <ti/drivers/rcl/LRFCC23X0.h>

#include DeviceFamily_constructPath(driverlib/dbell_regs.h)
#include DeviceFamily_constructPath(rf_patches/lrf_rfe_binary_ble5.h)
#include DeviceFamily_constructPath(rf_patches/lrf_rfe_binary_ble5_nopll.h)

#include DeviceFamily_constructPath(settings/lrf_rfe_cfg_fe_common.h)
#include DeviceFamily_constructPath(settings/lrf_rfe_cfg_synth_common.h)
#include DeviceFamily_constructPath(settings/lrf_rfe_cfg_synth_common_ram.h)

#include DeviceFamily_constructPath(driverlib/mdm_regs.h)
#include DeviceFamily_constructPath(driverlib/pbe_ble5_ram_regs.h)
#include DeviceFamily_constructPath(driverlib/pbe_common_ram_regs.h)

#include <rcl_settings_adc_noise.h>

/* Place necessary RCL structs in BUFRAM to avoid using static SYSRAM. This saves 148 B */
#define RCL_CLIENT_ADDR     (BUF_RAM_BASE_ADDR)
#define RCL_ADC_NOISE_CMD_ADDR   (RCL_CLIENT_ADDR + sizeof(RCL_Client))
#define CALLBACK_ADDR       (RCL_ADC_NOISE_CMD_ADDR + sizeof(RCL_CmdAdcNoiseGet))

#define RCL_CLIENT_PTR      ((RCL_Client*)RCL_CLIENT_ADDR)
#define RCL_ADC_NOISE_CMD_PTR    ((RCL_CmdAdcNoiseGet*)RCL_ADC_NOISE_CMD_ADDR)
#define CALLBACK_PTR        ((applicationCallback_t*)CALLBACK_ADDR)

#define STATUS_SUCCESS 0
#define STATUS_ERROR -1
#define RCL_STATUS_TO_WRAPPER_STATUS(x) ((x) == RCL_CommandStatus_Finished ? STATUS_SUCCESS : STATUS_ERROR)

/* Callback function type */
typedef void (*applicationCallback_t)(uint32_t* buffer, uint32_t numWords, int_fast16_t status);

/******************************************************************************
 * Internal callback function
 ******************************************************************************/
static void adcNoiseCallback(RCL_Command *cmd, LRF_Events lrfEvents, RCL_Events rclEvents)
{
    RCL_CmdAdcNoiseGet *adcCmd = (RCL_CmdAdcNoiseGet *)cmd;

    applicationCallback_t callback = *CALLBACK_PTR;

    callback(adcCmd->output, adcCmd->numWords, RCL_STATUS_TO_WRAPPER_STATUS(cmd->status));

    /* Close the RCL client */
    RCL_close(cmd->runtime.client);

    /* This must come after closing because command and client structs are in BUFRAM */
    LRF_clearAppClockEnable(DBELL_CLKCTL_BUFRAM_BM);

    /* Release power constraint to allow standby */
    hal_power_release_constraint();
}

/******************************************************************************
 * Public API
 ******************************************************************************/


/*
 *  ======== RCL_AdcNoise_get_samples_blocking ========
 */
int_fast16_t RCL_AdcNoise_get_samples_blocking(uint32_t* buffer, uint32_t numWords)
{
    RCL_CommandStatus status;
    RCL_CmdAdcNoiseGet *adcNoiseCmd = RCL_ADC_NOISE_CMD_PTR;

    /* Turn on BUFRAM before calling RCL_open, since the RCL_client resides in BUFRAM */
    LRF_setAppClockEnable(DBELL_CLKCTL_BUFRAM_BM);

    /* Prevent the system from going to standby because BUFRAM doesn't have retention */
    hal_power_set_constraint();

    RCL_init();

    /* Open client and provide settings */
    RCL_Handle h = RCL_open(RCL_CLIENT_PTR, &LRF_configAdcNoise);

    *adcNoiseCmd = RCL_CmdAdcNoiseGet_Default();

    /* Set number of sample words */
    adcNoiseCmd->numWords = numWords;
    adcNoiseCmd->output = buffer;

    /* Submit the command */
    status = RCL_Command_submit(h, adcNoiseCmd);

    if (status <= RCL_CommandStatus_Finished)
    {
        /* Wait for the command to finish. The thread will yield and other threads can run. */
        status = RCL_Command_pend(adcNoiseCmd);
    }

    /* Close the RCL client */
    RCL_close(h);

    /* This must come after closing because command and client structs are in BUFRAM */
    LRF_clearAppClockEnable(DBELL_CLKCTL_BUFRAM_BM);

    /* Release power constraint to allow standby */
    hal_power_release_constraint();

    return RCL_STATUS_TO_WRAPPER_STATUS(status);
}

/*
 *  ======== RCL_AdcNoise_get_samples_callback ========
 *
 *  NOTE: This function must be called from a task context, with interrupts enabled
 */
int_fast16_t RCL_AdcNoise_get_samples_callback(uint32_t* buffer, uint32_t numWords, applicationCallback_t callback)
{
    RCL_CommandStatus status;
    RCL_CmdAdcNoiseGet *adcNoiseCmd = RCL_ADC_NOISE_CMD_PTR;

    /* Turn on BUFRAM before calling RCL_open, since the RCL_client resides in BUFRAM */
    LRF_setAppClockEnable(DBELL_CLKCTL_BUFRAM_BM);

    /* Prevent the system from going to standby because BUFRAM doesn't have retention */
    hal_power_set_constraint();

    RCL_init();

    /* Open client and provide settings */
    RCL_Handle h = RCL_open(RCL_CLIENT_PTR, &LRF_configAdcNoise);

    *adcNoiseCmd = RCL_CmdAdcNoiseGet_Default();

    /* Set number of sample words */
    adcNoiseCmd->numWords = numWords;
    adcNoiseCmd->output = buffer;

    /* Set callback function to be run at the end of the command */
    adcNoiseCmd->common.runtime.callback = adcNoiseCallback;

    *CALLBACK_PTR = callback;

    /* Run callback at the end of the command only */
    adcNoiseCmd->common.runtime.rclCallbackMask.value = RCL_EventLastCmdDone.value;

    /* Submit the command */
    status = RCL_Command_submit(h, adcNoiseCmd);

    return RCL_STATUS_TO_WRAPPER_STATUS(status);
}
