/******************************************************************************
 @file:       ble_user_config.h

 @brief:    This file contains user configurable variables for the BLE
            Controller and Host. This file is also used for defining the
            type of RF Front End used with the TI device, such as using
            a Differential front end with External Bias, etc. Please see
            below for more detail.

            To change the default values of configurable variables:
              - Include the followings in your application main.c file:
                #ifndef USE_DEFAULT_USER_CFG

                #include "ti/ble/app_util/config/ble_user_config.h"

                // BLE user defined configuration
                bleUserCfg_t user0Cfg = BLE_USER_CFG;

                #endif // USE_DEFAULT_USER_CFG
              - Set the preprocessor symbol MAX_NUM_BLE_CONNS, MAX_NUM_PDU,
                MAX_PDU_SIZE, L2CAP_NUM_PSM or L2CAP_NUM_CO_CHANNELS
                to a desired value in your application project.
              - Include "ble_user_config.h" in your stack OSAL_ICallBle.c
                file.
              - Call setBleUserConfig at the start of stack_main. Actually,
                it is okay to set the variables anywhere in stack_main as
                long as it is BEFORE osal_init_system, but best to set at
                the very start of stack_main.

            Note: User configurable variables are only used during the
                  initialization of the Controller and Host. Changing
                  the values of these variables after this will have no
                  effect.

            Note: To use the default user configurable variables, define
                  the preprocessor symbol USE_DEFAULT_USER_CFG in your
                  application project.

            For example:
              - In your application main.c, include:
                #ifndef USE_DEFAULT_USER_CFG

                #include "bleUserConfig.h"

                // BLE user defined configuration
                bleUserCfg_t user0Cfg = BLE_USER_CFG;
                #endif // USE_DEFAULT_USER_CFG
              - In your application project, set the preprocessor symbol
                MAX_NUM_BLE_CONNS to 1 to change the maximum number of BLE
                connections to 1 from the default value of 3.
              - In your stack OSAL_ICallBle.c file, call setBleUserCfg to
                update the user configuration variables:
                #include "bleUserConfig.h"
                :
                int stack_main(void *arg)
                {
                  setBleUserConfig((bleUserCfg_t *)arg);
                  :
                }

            Default values:
              maxNumConns       : 1
              maxNumPDUs        : 5
              maxPduSize        : 27 (or 69 if Secure Connection enabled)
              maxNumPSM         : 3
              maxNumCoChannels  : 3
              maxAcceptListElems: 16
              maxResolvListElems: 10

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2014-2025, Texas Instruments Incorporated
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
#ifndef BLE_USER_CONFIG_H
#define BLE_USER_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */

#include "ti/ble/controller/ll/ll_user_config.h"
#include "ti/ble/stack_util/icall/app/icall_user_config.h"

#include "ti/ble/stack_util/icall/stack/ble_dispatch_lite.h"
#include "ti/ble/stack_util/health_toolkit/assert.h"
#include "ti/ble/stack_util/bcomdef.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

// Defines only required by Application.
// Note: ICALL_STACK0_ADDR is assigned the entry point of the stack image and
//       is only defined for the Application project
#if defined(ICALL_STACK0_ADDR)

#ifndef CONFIG_ZEPHYR
#include <ti_drivers_config.h>
#endif // CONFIG_ZEPHYR

// RF Front End Settings
// Note: The use of these values completely depends on how the PCB is laid out.
//       Please see Device Package and Evaluation Module (EM) Board below.
#define RF_FE_DIFFERENTIAL              0
#define RF_FE_SINGLE_ENDED_RFP          1
#define RF_FE_SINGLE_ENDED_RFN          2
#define RF_FE_ANT_DIVERSITY_RFP_FIRST   3
#define RF_FE_ANT_DIVERSITY_RFN_FIRST   4
#define RF_FE_SINGLE_ENDED_RFP_EXT_PINS 5
#define RF_FE_SINGLE_ENDED_RFN_EXT_PINS 6
//
#define RF_FE_INT_BIAS                  (0<<3)
#define RF_FE_EXT_BIAS                  (1<<3)

// This is a common file for the legacy and sysconfig examples,
// the parameters under ifndef SYSCFG are defined in this file for
// the legacy examples and generated using the sysconfig tool for
// the sysconfig examples
#ifndef SYSCFG
// Maximum number of BLE connections. It should be set based on the
// device GAP role. Here're some recommended values:
//      * Central:     3
//      * Peripheral:  1
//      * Observer:    0
//      * Broadcaster: 0
// Note: When the GAP role includes Peripheral and no v4.1 Controller features
//       are configured, MAX_NUM_BLE_CONNS must not be greater than 1
#ifndef MAX_NUM_BLE_CONNS
  #define MAX_NUM_BLE_CONNS             1
#endif

/************************************/

// Specifies whether LFOSC (RCOSC) was configured in CCFG module by the user
#define SRC_CLK_IS_LFOSC                0

// User configurable extra PPM for peripheral RX window widening
// This only applies if SRC_CLK_IS_LFOSC is set to 1, default value will be 1500PPM
#define USER_CFG_LFOSC_EXTRA_PPM        1500

// bitmask of extended stack settings
#ifndef EXTENDED_STACK_SETTINGS
#define EXTENDED_STACK_SETTINGS         0x00
#endif

// bitmask which enables the offset of the overrides for BAW
#define CC2652RB_OVERRIDE_USED          0x02

// Maximum number of BLE HCI PDUs. If the maximum number connections (above)
// is set to 0 then this number should also be set to 0.
#ifndef MAX_NUM_PDU
  #define MAX_NUM_PDU                   5
#endif

// Maximum size in bytes of the BLE HCI PDU. Valid range: 27 to 255
// The maximum ATT_MTU is MAX_PDU_SIZE - 4.
#ifndef MAX_PDU_SIZE
  #define MAX_PDU_SIZE                  69
#endif

#ifdef CHANNEL_SOUNDING
#define CS_STEPS_RESULTS_FORMAT         1   // By default set it to 'Custom' format
#endif

#endif //SYSCFG

// Maximum number of L2CAP Protocol/Service Multiplexers (PSM)
#ifndef L2CAP_NUM_PSM
  #define L2CAP_NUM_PSM                 3
#endif

// Maximum number of L2CAP Connection Oriented Channels
#ifndef L2CAP_NUM_CO_CHANNELS
  #define L2CAP_NUM_CO_CHANNELS         8
#endif

#ifndef CFG_MAX_NUM_RL_ENTRIES
#ifdef GAP_BOND_MGR
#define CFG_MAX_NUM_RL_ENTRIES             GAP_BONDINGS_MAX     // at 60 bytes per RL entry
#else
#define CFG_MAX_NUM_RL_ENTRIES             MAX_NUM_RL_ENTRIES  // at 60 bytes per RL entry
#endif
#endif

#ifndef CFG_MAX_NUM_AL_ENTRIES
#define CFG_MAX_NUM_AL_ENTRIES             MAX_NUM_AL_ENTRIES  // at 8 bytes per AL entry
#endif

#ifndef ADV_RPT_INC_CHANNEL
#define ADV_RPT_INC_CHANNEL            0
#endif

//
// Device Package and Evaluation Module (EM) Board
//

#ifndef PM_STARTUP_MARGIN
  #define PM_STARTUP_MARGIN             300
#endif

#define BLE_USER_CFG                    { &bleStackConfig,         \
                                          &bleAppServiceInfoTable }

// Make sure there's enough heap needed for BLE connection Tx buffers, which
// is based on MAX_PDU_SIZE and MAX_NUM_PDU configured by the application.
// The heap memory needed for BLE connection Tx buffers should not be more
// that 1/3 of the total ICall heap size (HEAPMGR_SIZE).
//
//  Notes: Over the Air (OTA) PDU Size = 27, and LL Header Size = 14
//         If HEAPMGR_SIZE = 0 then auto-size heap is being used
//
#if (MAX_NUM_BLE_CONNS > 0) && !defined(NO_HEAPSIZE_VALIDATE) && (HEAPMGR_SIZE != 0)
  #if  (((((MAX_PDU_SIZE / 27) + 1) * MAX_NUM_PDU) * (27 + 14)) > (HEAPMGR_SIZE / 3))
    #warning Not enough heap for configured MAX_NUM_PDU and MAX_PDU_SIZE! Adjust HEAPMGR_SIZE.
  #endif
#endif

/********* SDAA *********/
// Use default values if default value is not yet defined
#ifndef SDAA_MAX_THRESHOLD
#define SDAA_MAX_THRESHOLD 10
// Raise error if values not in the correct boundaries (value in %)
#elif(SDAA_MAX_THRESHOLD < 0 || SDAA_MAX_THRESHOLD > 100)
#error "SDAA_MAX_THRESHOLD value should be between 0 and 100"
#endif

#ifndef SDAA_RX_WINDOW_DURATION
#define SDAA_RX_WINDOW_DURATION 12
// Raise error if values not in the correct boundaries (value in 10us jump)
#elif (SDAA_RX_WINDOW_DURATION < 12 || SDAA_RX_WINDOW_DURATION > 100)
#error "SDAA_RX_WINDOW_DURATION should be between 12 and 100"
#endif

#ifndef SDAA_MAX_BLOCKED_CHANNEL_TIME
#define SDAA_MAX_BLOCKED_CHANNEL_TIME    1 //1sec
#elif (SDAA_MAX_BLOCKED_CHANNEL_TIME < 0 || SDAA_MAX_BLOCKED_CHANNEL_TIME > 255)
#error "SDAA_MAX_BLOCKED_CHANNEL_TIME should be between 0 and 255"
#endif

#ifndef SDAA_CONST_OBSERV_TIME
#define SDAA_CONST_OBSERV_TIME    true  // True or false value that determine if the SDAA will use fixed observation time or a changing one.
#elif (SDAA_CONST_OBSERV_TIME != 0 && SDAA_CONST_OBSERV_TIME != 1)
#error "SDAA_CONST_OBSERV_TIME should be true or false"
#endif

#ifndef SDAA_OBSERVATION_TIME
#define SDAA_OBSERVATION_TIME    1  // (100ms)
#elif (SDAA_OBSERVATION_TIME < 1 || SDAA_OBSERVATION_TIME > 3000)
#error "SDAA_CONST_OBSERV_TIME should be between 1 and 3000 (100ms to 5min )"
#endif

#ifndef SDAA_TX_POWER_GAIN
#define SDAA_TX_POWER_GAIN    0  // (dBm)
#elif (SDAA_TX_POWER_GAIN < -20 || SDAA_TX_POWER_GAIN > 20)
#error "SDAA_TX_POWER_GAIN should be between -40 and 20 dBm"
#endif

#ifndef SDAA_RX_POWER_GAIN
#define SDAA_RX_POWER_GAIN    0  // (dBm)
#elif (SDAA_RX_POWER_GAIN < -20 || SDAA_RX_POWER_GAIN > 20)
#error "SDAA_RX_POWER_GAIN should be between -40 and 20 dBm"
#endif

/************************************/

#endif // !(CTRL_CONFIG | HOST_CONFIG)

/*******************************************************************************
 * TYPEDEFS
 */

typedef struct
{
  uint8_t                     maxNumConns;
  uint8_t                     maxNumPDUs;
  uint8_t                     maxPduSize;
  uint8_t                     maxNumPSM;
  uint8_t                     maxNumCoChannels;
  uint8_t                     maxAcceptListElems;
  uint8_t                     maxResolvListElems;
  pfnBMAlloc_t                *pfnBMAlloc;
  pfnBMFree_t                 *pfnBMFree;
  ECCParams_CurveParams       *eccParams;
  pfnFastStateUpdate_t        fastStateUpdateCb;
  uint32_t                    bleStackType;
  uint32_t                    extStackSettings; // | reserved | use CC2652RB | MasterGuard |
                                                // |   31..2  |       1      |      0      |
  uint8                       advReportIncChannel;
    // Rssi monitor configuration
  uint8_t                     rssiMonitorMaxCliNum;       // Maximum number of callback the RSSI monitor can handle
  int16_t                     rssiMonitorMaxRssiWeight;   // The max weight possibe, should be change by multiple of 10
  int8_t                      pwrCtrlRssiLowThreshold;    // RSSI low threshold for power control
  int8_t                      pwrCtrlRssiHighThreshold;   // RSSI high threshold for power control
  int8_t                      pwrCtrlDeltaStepDb;         // Delta step in dB for power control threshold passed
} stackSpecific_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */
// The Tx Power value
extern int8 defaultTxPowerDbm;

extern pfnBMAlloc_t pfnBMAlloc;
extern pfnBMFree_t  pfnBMFree;

extern uint16_t bleUserCfg_maxPduSize;
extern uint16_t llUserConfig_maxPduSize;

extern uint8  userCfgClockType;
extern uint16 userCfgAdditionalPPM;

extern const stackSpecific_t       bleStackConfig;
extern applicationService_t        bleAppServiceInfoTable;

/*********************************************************************
 * FUNCTIONS
 */
extern void setBleUserConfig(icall_userCfg_t *userCfg);
extern void RegisterAssertCback(assertCback_t appAssertHandler);
extern void DefaultAssertCback(uint8 assertCause, uint8 assertSubCause);
extern assertCback_t appAssertCback; // only App's ble_user_config.c
extern assertCback_t halAssertCback; // only Stack's ble_user_config.c

#ifdef __cplusplus
}
#endif

#endif /* BLE_USER_CONFIG_H */
