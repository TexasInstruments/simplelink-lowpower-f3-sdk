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
#ifndef TI_RF_TEST_H
#define TI_RF_TEST_H 1

#include <stdint.h>
#include <stdbool.h>

typedef enum rf_test_tx_mode_e {
    RF_TEST_CONTINUOUS_WAVE_MODE = 0,
    RF_TEST_MODULATED_SIGNAL_MODE = 1
} rf_test_tx_mode;

typedef enum rf_test_ret_e {
    RF_TEST_ERROR = -1,
    RF_TEST_SUCCESS = 0
} rf_test_ret;

/**
 * @brief Set the RF channel for testing.
 * @param channel RF channel number to set.
 */
rf_test_ret rf_test_set_channel(uint8_t channel);

/**
 * @brief Set the RF transmit power.
 * @param power Transmit power level to set.
 */
rf_test_ret rf_test_set_power(int8_t power);


/**
 * @brief Initialize the RF radio hardware for testing.
 */
rf_test_ret rf_test_radio_init();

/**
 * @brief Start TX transmission.
 * @param mode Transmission mode (0: Continuous Wave, 1: Modulated Signal).
 */
rf_test_ret rf_test_start_tx_cw(rf_test_tx_mode mode);

/**
 * @brief Set up the RF radio to receive mode for RSSI measurement.
 */
rf_test_ret rf_test_start_rx();

/**
 * @brief Get the current RSSI (Received Signal Strength Indicator) value.
 * @return RSSI value in dBm.
 */
int8_t rf_test_get_rssi();

/**
 * @brief Aborts current operation, either it's RX or TX.
 */
rf_test_ret rf_test_stop_operation();

/**
 * @brief Tear down radio.
 */
rf_test_ret rf_test_teardown_radio();

#endif /* TI_RF_TEST_H */