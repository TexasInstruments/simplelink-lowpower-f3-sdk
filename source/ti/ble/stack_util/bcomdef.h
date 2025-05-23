/******************************************************************************

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2025, Texas Instruments Incorporated
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

/**
 *  @file  bcomdef.h
 *  @brief      Type definitions and macros for BLE stack.
 */

#ifndef BCOMDEF_H
#define BCOMDEF_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */

#include "ti/ble/stack_util/comdef.h"

/*********************************************************************
 * CONSTANTS
 */

/// @cond NODOC
#if defined ( HOST_CONFIG )
  // Set the Controller Configuration
  #if ( HOST_CONFIG == ( CENTRAL_CFG | PERIPHERAL_CFG ) )
    #define CTRL_CONFIG   ( ADV_NCONN_CFG | ADV_CONN_CFG | SCAN_CFG | INIT_CFG )
  #elif ( HOST_CONFIG == ( CENTRAL_CFG | BROADCASTER_CFG ) )
    #define CTRL_CONFIG   ( ADV_NCONN_CFG | SCAN_CFG | INIT_CFG )
  #elif ( HOST_CONFIG == ( PERIPHERAL_CFG | OBSERVER_CFG ) )
    #define CTRL_CONFIG   ( ADV_NCONN_CFG | ADV_CONN_CFG | SCAN_CFG )
  #elif ( HOST_CONFIG == ( BROADCASTER_CFG | OBSERVER_CFG ) )
    #define CTRL_CONFIG   ( ADV_NCONN_CFG | SCAN_CFG )
  #elif ( HOST_CONFIG == CENTRAL_CFG )
    #define CTRL_CONFIG   ( SCAN_CFG | INIT_CFG )
  #elif ( HOST_CONFIG == PERIPHERAL_CFG )
    #define CTRL_CONFIG   ( ADV_NCONN_CFG | ADV_CONN_CFG )
  #elif ( HOST_CONFIG == OBSERVER_CFG )
    #define CTRL_CONFIG   SCAN_CFG
  #elif ( HOST_CONFIG == BROADCASTER_CFG )
    #define CTRL_CONFIG   ADV_NCONN_CFG
  #else
    #error "Build Configuration Error: Invalid Host Role!"
  #endif
#else
  // Controller Sanity Check: Stop build when no configuration is defined.
  #if !defined( CTRL_CONFIG ) || !( CTRL_CONFIG & ( ADV_NCONN_CFG | \
                                                    ADV_CONN_CFG  | \
                                                    SCAN_CFG      | \
                                                    INIT_CFG ) )
    #error "Build Configuration Error: At least one Controller build component required!"
  #endif // no Controller build components defined
#endif

// If BLE_V41_FEATURES is defined, map it to CTRL_V41_CONFIG
#if defined ( BLE_V41_FEATURES ) && !defined ( CTRL_V41_CONFIG )
  #define CTRL_V41_CONFIG       ( ( BLE_V41_FEATURES ) & CTRL_V41_MASK )
#elif defined ( BLE_V41_FEATURES ) && defined ( CTRL_V41_CONFIG )
  #error "Build Configuration Error: Cannot define both BLE_V41_FEATURES and CTRL_V41_CONFIG!"
#endif // BLE_V41_FEATURES

// If BLE_V50_FEATURES is defined, map it to CTRL_V50_CONFIG
#if defined ( BLE_V50_FEATURES ) && !defined ( CTRL_V50_CONFIG )
  #define CTRL_V50_CONFIG       BLE_V50_FEATURES
#elif defined ( BLE_V50_FEATURES ) && defined ( CTRL_V50_CONFIG )
  #error "Build Configuration Error: Cannot define both BLE_V50_FEATURES and CTRL_V50_CONFIG!"
#endif // BLE_V41_FEATURES

#if defined(CC2540) || defined(CC2541) || defined(CC2541S)
  #if !defined ( MAX_NUM_BLE_CONNS )
    #if ( CTRL_CONFIG & INIT_CFG )
      #define MAX_NUM_BLE_CONNS                       3
    #elif ( !( CTRL_CONFIG & INIT_CFG ) && ( CTRL_CONFIG & ADV_CONN_CFG ) )
      #define MAX_NUM_BLE_CONNS                       1
    #else // no connection needed
      #define MAX_NUM_BLE_CONNS                       0
    #endif // CTRL_CONFIG=INIT_CFG
  #endif // !MAX_NUM_BLE_CONNS
#endif // CC2540 | CC2541 | CC2541S

/// @endcond // NODOC


//! Default Public and Random Address Length
#define B_ADDR_LEN                                6

//! Default key length
#define KEYLEN                                    16

//! Defines for decomposed Address
//! 2 BYTES LSB
#define LSB_2_BYTES                               2

//! 4 BYTES MSB
#define MSB_4_BYTES                               4

//! BLE Channel Map length
#define B_CHANNEL_MAP_LEN                         5

//! BLE Event mask length
#define B_EVENT_MASK_LEN                          8

//! BLE Local Name length
#define B_LOCAL_NAME_LEN                          248

//! BLE Maximum Advertising Packet Length
#define B_MAX_ADV_LEN                             31

//! BLE Random Number Size
#define B_RANDOM_NUM_SIZE                         8

//! BLE Feature Supported length
#define B_FEATURE_SUPPORT_LENGTH                  8

//! BLE Default Passcode
#define B_APP_DEFAULT_PASSCODE                    123456

//! Stack misc settings bitmask
#define EXTENDED_STACK_SETTINGS_DEFAULT           0x00

//! Stack misc settings bitmask
#define CENTRAL_GUARD_TIME_ENABLE                 0x01
#define CC2652RB_OVERRIDE_USED                    0x02

#define bleInvalidTaskID                INVALID_TASK  //!< Task ID isn't setup properly
#define bleNotReady                     0x10  //!< Not ready to perform task
#define bleAlreadyInRequestedMode       0x11  //!< Already performing that task
#define bleIncorrectMode                0x12  //!< Not setup properly to perform that task
#define bleMemAllocError                0x13  //!< Memory allocation error occurred
#define bleNotConnected                 0x14  //!< Can't perform function when not in a connection
#define bleNoResources                  0x15  //!< There are no resource available
#define blePending                      0x16  //!< Waiting
#define bleTimeout                      0x17  //!< Timed out performing function
#define bleInvalidRange                 0x18  //!< A parameter is out of range
#define bleLinkEncrypted                0x19  //!< The link is already encrypted
#define bleProcedureComplete            0x1A  //!< The Procedure is completed
#define bleInvalidMtuSize               0x1B  //!< MTU size is out of range
#define blePairingTimedOut              0x1C  //!< Previous pairing attempt timed out
#define bleMemFreeError                 0x1D  //!< Memory free error occurred
#define bleInternalError                0x1E  //!< Internal error not due to application
#define bleDisallowed                   0x1F  //!< Command is not allowed

// GAP Status Return Values - returned as bStatus_t
#define bleGAPUserCanceled              0x30  //!< The user canceled the task
#define bleGAPConnNotAcceptable         0x31  //!< The connection was not accepted
#define bleGAPBondRejected              0x32  //!< The bond information was rejected.
#define bleGAPBufferInUse               0x33  //!< The buffer is in use elsewhere
#define bleGAPNotFound                  0x34  //!< No handle / buffer found
#define bleGAPFilteredOut               0x35  //!< The received PDU was filtered out.
#define bleGAPBondItemNotFound          0x36  //!< An item of a bond record was not found.

// ATT Status Return Values - returned as bStatus_t
#define bleInvalidPDU                   0x40  //!< The attribute PDU is invalid
#define bleInsufficientAuthen           0x41  //!< The attribute has insufficient authentication
#define bleInsufficientEncrypt          0x42  //!< The attribute has insufficient encryption
#define bleInsufficientKeySize          0x43  //!< The attribute has insufficient encryption key size

// L2CAP Status Return Values - returned as bStatus_t

#define INVALID_TASK_ID                 0xFF  //!< Task ID isn't setup properly

// Note: Maximum NVID index is 0x03FF (10 bits) - this is a NV driver limitation
#define MAX_NUM_NV_RECORDS              50

// Device NV Items -    Range 0 - 0x1F
#define BLE_NVID_IRK                    0x02   //!< The Device's IRK
#define BLE_NVID_CSRK                   0x03   //!< The Device's CSRK
#define BLE_NVID_ADDR_MODE              0x04   //!< The Device's address type (@ref GAP_Addr_Modes_t)
#define BLE_LRU_BOND_LIST               0x05   //!< The Device's order of bond indexes in least recently used order
#define BLE_NVID_RANDOM_ADDR            0x06   //!< The Device's random address if set by the current @ref GAP_DeviceInit

// Bonding NV Items -   Range  0x20 - ( (GAP_BONDINGS_MAX * GAP_BOND_REC_IDS) - 1 )
#define BLE_NVID_GAP_BOND_START         0x20                                                                  //!< Start of the GAP Bond Manager's NV IDs
#define BLE_NVID_GAP_BOND_END           (BLE_NVID_GAP_BOND_START + (MAX_NUM_NV_RECORDS * GAP_BOND_REC_IDS) - 1) //!< End of the GAP Bond Manager's NV IDs Range
                                                                                                              //!< 50 Bonds * 6 Rec IDs

// GATT Configuration NV Items - Range  (BLE_NVID_GAP_BOND_END + 1) - (BLE_NVID_GATT_CFG_START + GAP_BONDINGS_MAX - 1) - This must match the number of Bonding entries
#define BLE_NVID_GATT_CFG_START         (BLE_NVID_GAP_BOND_END + 1)                                           //!< Start of the GATT Configuration NV IDs
#define BLE_NVID_GATT_CFG_END           (BLE_NVID_GATT_CFG_START + MAX_NUM_NV_RECORDS - 1)                      //!< End of the GATT Configuration NV IDs

// Customer NV Items - Range  (BLE_NVID_GATT_CFG_END + 1) - (BLE_NVID_CUST_START + GAP_BONDINGS_MAX - 1) - This must match the number of Bonding entries
#define BLE_NVID_CUST_START             (BLE_NVID_GATT_CFG_END + 1)                                           //!< Start of the Customer's NV IDs
#define BLE_NVID_CUST_END               (BLE_NVID_CUST_START + MAX_NUM_NV_RECORDS - 1)                          //!< End of the Customer's NV IDs

// BLE Mesh NV IDs Start
#define BLE_NVID_MESH_START            BLE_NVID_CUST_END + 1

/*********************************************************************
 * BLE OSAL GAP GLOBAL Events
 */
#define GAP_EVENT_SIGN_COUNTER_CHANGED  0x4000  //!< The device level sign counter changed

// GAP - Messages IDs (0xD0 - 0xDF)
#define GAP_MSG_EVENT                         0xD0 //!< Incoming GAP message

// SM - Messages IDs (0xC1 - 0xCF)
#define SM_NEW_RAND_KEY_EVENT                 0xC1 //!< New Rand Key Event message
#define SM_MSG_EVENT                          0xC2 //!< Incoming SM message

// GATT - Messages IDs (0xB0 - 0xBF)
#define GATT_MSG_EVENT                        0xB0 //!< Incoming GATT message
#define GATT_SERV_MSG_EVENT                   0xB1 //!< Incoming GATT Serv App message

// L2CAP - Messages IDs (0xA0 - 0xAF)
#define L2CAP_DATA_EVENT                      0xA0 //!< Incoming data on a channel
#define L2CAP_SIGNAL_EVENT                    0xA2 //!< Incoming Signaling message

// HCI - Messages IDs (0x90 - 0x9F)
#define HCI_DATA_EVENT                        0x90 //!< HCI Data Event message
#define HCI_GAP_EVENT_EVENT                   0x91 //!< GAP Event message
#define HCI_SMP_EVENT_EVENT                   0x92 //!< SMP Event message
#define HCI_EXT_CMD_EVENT                     0x93 //!< HCI Extended Command Event message
#define HCI_SMP_META_EVENT_EVENT              0x94 //!< SMP Meta Event message
#define HCI_GAP_META_EVENT_EVENT              0x95 //!< GAP Meta Event message

// ICall and Dispatch - Messages IDs (0x80 - 0x8F)
#define ICALL_EVENT_EVENT                     0x80 //!< ICall Event message
#define ICALL_CMD_EVENT                       0x81 //!< ICall Command Event message
#define DISPATCH_CMD_EVENT                    0x82 //!< Dispatch Command Event message

#ifdef __TI_COMPILER_VERSION__
#define PRAGMA_OPTIMIZE_NONE
#elif defined(__IAR_SYSTEMS_ICC__)
#define PRAGMA_OPTIMIZE_NONE _Pragma("optimize=none")
#endif

/*********************************************************************
 * TYPEDEFS
 */

  //! BLE Generic Status return
typedef uint8_t bStatus_t;


/*********************************************************************
 * System Events
 */

/*********************************************************************
 * Global System Messages
 */

/*********************************************************************
 * MACROS
 */

/// @brief TI Base 128-bit UUID: F000XXXX-0451-4000-B000-000000000000
#define TI_BASE_UUID_128( uuid )  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, \
                                  0x00, 0x40, 0x51, 0x04, LO_UINT16( uuid ), HI_UINT16( uuid ), 0x00, 0xF0

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* BCOMDEF_H */
