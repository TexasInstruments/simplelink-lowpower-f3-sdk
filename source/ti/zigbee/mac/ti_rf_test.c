/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "ti_rf_test.h"
#include <ti/drivers/dpl/SwiP.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/log/Log.h>
#include <ti/drivers/rcl/RCL.h>
#include <ti/drivers/rcl/LRF.h>
#include <ti/drivers/rcl/RCL_Scheduler.h>
#include <ti/drivers/rcl/commands/ieee.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_fcfg.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)

#define CW_FREQUENCY_OFFSET 500000U
#define RX_TX_TIMEOUT 1000

static RCL_CmdIeeeTxTest  txTestCmd;
static RCL_Handle RCL_handle;
static RCL_Client rclClient;
extern const LRF_Config LRF_config_ieee_802_15_4_t;
static RCL_CmdIeeeRxTx  ieeeRxCmd;
static uint32_t rxActionBuf[RCL_Buffer_bytesToWords(sizeof(RCL_CmdIeee_RxAction)) + RCL_Buffer_bytesToWords(sizeof(RCL_CmdIeee_PanConfig))];
static RCL_CmdIeee_RxAction *rxAction = (RCL_CmdIeee_RxAction *)&rxActionBuf;
static uint16_t DEFAULT_PATTERN = 0x057B;
SemaphoreP_Handle rfSemaphore;
static bool rxActive = false;
static bool txActive = false;

rf_test_ret rf_test_set_channel(uint8_t channel)
{
    if (channel < 11 || channel > 26) {
        return RF_TEST_ERROR;
    }
    txTestCmd.rfFrequency = RCL_CMD_IEEE_CHANNEL_FREQUENCY(channel);
    ieeeRxCmd.rfFrequency = RCL_CMD_IEEE_CHANNEL_FREQUENCY(channel);

    return RF_TEST_SUCCESS;
}

rf_test_ret rf_test_set_power(int8_t power)
{
    RCL_Command_TxPower txPower;
    txPower.dBm = power;
    txPower.fraction = 0;
    txTestCmd.txPower = txPower;
    return RF_TEST_SUCCESS;
}

void rf_Callback(RCL_Command *cmd, LRF_Events lrfEvents, RCL_Events rclEvents){
    if (rclEvents.cmdStarted){
        SemaphoreP_post(rfSemaphore);
    }
}

rf_test_ret rf_test_radio_init()
{
    RCL_init();
    RCL_handle = RCL_open(&rclClient, &LRF_config_ieee_802_15_4_t);
    if(NULL == RCL_handle)
    {
        return RF_TEST_ERROR;
    }
    ieeeRxCmd = RCL_CmdIeeeRxTx_DefaultRuntime();
    txTestCmd = RCL_CmdIeeeTxTest_DefaultRuntime();
    *rxAction = RCL_CmdIeee_RxAction_DefaultRuntime();
    ieeeRxCmd.rxAction = rxAction;
    ieeeRxCmd.common.scheduling = RCL_Schedule_Now;
    ieeeRxCmd.common.runtime.callback = rf_Callback;
    ieeeRxCmd.common.runtime.rclCallbackMask.value = RCL_EventCmdStarted.value;
    txTestCmd.common.runtime.callback = rf_Callback;
    txTestCmd.common.runtime.rclCallbackMask.value = RCL_EventCmdStarted.value;
    txTestCmd.common.scheduling = RCL_Schedule_Now;

    SemaphoreP_Params semParams;
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    rfSemaphore = SemaphoreP_create(0,&semParams);
    return RF_TEST_SUCCESS;
}

rf_test_ret rf_test_start_tx_cw(rf_test_tx_mode mode)
{
    if (rxActive || txActive) {
        return RF_TEST_ERROR;
    }

    if (mode == RF_TEST_CONTINUOUS_WAVE_MODE)
    {
        txTestCmd.config.sendCw = 1;
        txTestCmd.rfFrequency = txTestCmd.rfFrequency + CW_FREQUENCY_OFFSET;
    }
    else
    {
        txTestCmd.config.sendCw = 0;
        txTestCmd.config.whitenMode = RCL_CMD_IEEE_WH_MODE_OFF;
        txTestCmd.txWord = DEFAULT_PATTERN;
        txTestCmd.rfFrequency = txTestCmd.rfFrequency;
    }

    RCL_CommandStatus status = RCL_Command_submit(RCL_handle, &txTestCmd);
    if (status >= RCL_CommandStatus_Error){
        return RF_TEST_ERROR;
    }
    SemaphoreP_pend(rfSemaphore, RX_TX_TIMEOUT);
    txActive = true;

    return RF_TEST_SUCCESS;
}

rf_test_ret rf_test_stop_operation()
{
    RCL_CommandStatus status;
    if (rxActive)
    {
        status = RCL_Command_stop(&ieeeRxCmd, RCL_StopType_Hard);
    }
    else
    {
        status = RCL_Command_stop(&txTestCmd, RCL_StopType_Hard);
    }
    if (status >= RCL_CommandStatus_Error){
        return RF_TEST_ERROR;
    }
    rxActive = false;
    txActive = false;
    return RF_TEST_SUCCESS;
}

int8_t rf_test_get_rssi(){
    return RCL_readRssi();
}

rf_test_ret rf_test_start_rx()
{
    const uint32_t timeoutMs = 5000;
    const uint32_t pollIntervalMs = 10;
    uint32_t elapsedMs = 0;
    RCL_CommandStatus status = RCL_Command_submit(RCL_handle, &ieeeRxCmd);
    if (status >= RCL_CommandStatus_Error){
        return RF_TEST_ERROR;
    }
    SemaphoreP_pend(rfSemaphore, RX_TX_TIMEOUT);

    // This polling loop for 5 seconds is required to ensure we give sufficient time for the radio to start receiving
    // and for a valid RSSI value to be available. The RSSI may not be immediately valid after starting RX.
    while (elapsedMs < timeoutMs) {
        if (rf_test_get_rssi() != LRF_RSSI_INVALID) {
            rxActive = true;
            return RF_TEST_SUCCESS;
        }
        ClockP_sleep(pollIntervalMs / 1000); // Sleep for pollIntervalMs (converted to seconds)
        elapsedMs += pollIntervalMs;
    }
    return RF_TEST_ERROR;
}

rf_test_ret rf_test_teardown_radio()
{
    RCL_close(RCL_handle);
    return RF_TEST_SUCCESS;
}