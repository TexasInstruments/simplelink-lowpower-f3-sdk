/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2026, Texas Instruments Incorporated
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

 #ifndef SENSOR_MAC_CONFIG_SETTINGS_H
 #define SENSOR_MAC_CONFIG_SETTINGS_H 1
 
 #define CONFIG_PAN_ID               0xFFFF       // Currently set to a default value to indicate that one is not set
 #define CONFIG_CHANNEL_PAGE         0            // Always 0 since we're working with 2.4 GHz in this use case
 // #define CONFIG_CHANNEL_MASK         (0x7FFF800)  // Scan for beacon across all channels
 #define CONFIG_CHANNEL_MASK         (1L << (23))   // Shift 1 to the left by the desired channel number (between 11 and 26)
 
 // Application-level operations
 #define CONFIG_POLLING_INTERVAL        2000   // 2 seconds
 #define CONFIG_REPORTING_INTERVAL      3000   // 3 seconds
 #define CONFIG_SCAN_BACKOFF_INTERVAL   5000   // 5 seconds
 #define CONFIG_ORPHAN_BACKOFF_INTERVAL 30000  // 30 seconds
 // default of 5, range is from 0 to 255 (see weird formula on how this is defined in MAC specs, difficult to understand)
 // Just setting scan duration to 5 for now since it's being done that way in some test cases, and because that's the deafult value of 15.4 stack
 #define CONFIG_SCAN_DURATION       5
 // Taking what's already in advanced_config.h in 15.4 stack. This is the number of consecutive maximum poll/data request failures before handling the matter.
 #define CONFIG_MAX_DATA_FAILURES   5
 
 // MAC-level operations
 #define CONFIG_MAC_BEACON_ORDER     0xF
 #define CONFIG_MAC_SUPERFRAME_ORDER 0xFF
 
 #define CONFIG_RX_ON_IDLE           0 // sleepy device
 #define CONFIG_PHY_CURRENT_PAGE     0 // took from what's already in sensor.c
 
 #define CONFIG_AUTO_START           0
 
 #endif // SENSOR_MAC_CONFIG_SETTINGS