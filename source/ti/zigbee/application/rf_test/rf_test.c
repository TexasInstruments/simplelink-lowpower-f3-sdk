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

#include <ti/log/Log.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/TimestampP.h>
#include "ti_rf_test.h"
#include "zboss_api.h"
#define RSSI_READINGS 30
#define CHANNEL_NUM 18
#define TX_POWER 5


MAIN()
{
    int8_t rssi = 0;
    uint32_t tx_cw_duration = 30000000;
    rf_test_ret return_status;

    /* Initialization */
    return_status = rf_test_radio_init();
    if (return_status != RF_TEST_SUCCESS) {
        Log_printf(LogModule_Zigbee_App, LOG_ERROR , "rf_test_radio_init failed");
    }
    rf_test_set_channel(CHANNEL_NUM);
    rf_test_set_power(TX_POWER); /* Only required for TX */

    /* TX carrier wave for 30 seconds */
    return_status = rf_test_start_tx_cw(RF_TEST_MODULATED_SIGNAL_MODE);
    if (return_status != RF_TEST_SUCCESS) {
        Log_printf(LogModule_Zigbee_App, LOG_ERROR , "rf_test_start_tx_cw failed");
    }
    ClockP_usleep(tx_cw_duration);
    return_status = rf_test_stop_operation();
    if (return_status != RF_TEST_SUCCESS) {
        Log_printf(LogModule_Zigbee_App, LOG_ERROR , "rf_test_stop_operation failed");
    }

    /* Setup RX and gather RSSI for 30 seconds */
    return_status = rf_test_start_rx();
    if (return_status != RF_TEST_SUCCESS) {
        Log_printf(LogModule_Zigbee_App, LOG_ERROR , "rf_test_start_rx failed");
    }
    for (int i= 0 ; i < RSSI_READINGS; i++){
        rssi = rf_test_get_rssi();
        Log_printf(LogModule_Zigbee_App, Log_INFO , "RSSI value=%d", rssi);
        ClockP_sleep(1);

    }
    return_status = rf_test_stop_operation();
    if (return_status != RF_TEST_SUCCESS) {
        Log_printf(LogModule_Zigbee_App, LOG_ERROR , "rf_test_stop_operation failed");
    }

    return_status = rf_test_teardown_radio();
    if (return_status != RF_TEST_SUCCESS) {
        Log_printf(LogModule_Zigbee_App, LOG_ERROR , "rf_test_teardown_radio failed");
    }
    while(1);
}
