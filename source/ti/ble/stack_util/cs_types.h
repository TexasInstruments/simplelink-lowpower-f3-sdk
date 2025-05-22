/******************************************************************************

 @file  cs_types.h

 @brief This file defines the types and constants used for Channel Sounding.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2025, Texas Instruments Incorporated
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
#ifndef CS_TYPES_H
#define CS_TYPES_H
/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/bcomdef.h"

/*******************************************************************************
 * CONSTANTS
 */

// Tables length
#define CS_CHM_SIZE                            10U // octets
#define CS_FAE_TBL_LEN                         72  // octets

/** @defgroup CS_Procedure_Done_Status Channel Sounding Procedure Done Status
 * @{
 */
// Procedure done status
#define    CS_PROCEDURE_DONE     0x0            //!< Procedure completed successfully
#define    CS_PROCEDURE_ACTIVE   0x1            //!< Procedure is currently active
#define    CS_PROCEDURE_INACTIVE 0x2            //!< Procedure is inactive
#define    CS_PROCEDURE_ABORTED  0xF            //!< Procedure was aborted
/** @} */

/** @defgroup CS_Abort_Reason Channel Sounding Abort Reason
 * @{
 */
// Abort reason
#define    CS_NO_ABORT              0x0         //!< No abort
#define    CS_ABORT_REQUEST         0x1         //!< Abort requested
#define    CS_ABORT_CHM             0x2         //!< Abort due to channel map
#define    CS_ABORT_INSTANT_PASSED  0x3         //!< Abort due to instant passed
#define    CS_ABORT_UNSPECIFIED     0xF         //!< Unspecified abort reason
/** @} */

/** @defgroup CS_Sync_Phy_Supported Channel Sounding Sync PHY Support
 * @{
 */
// CS sync phy support
#define    CS_LE_1M_SYNC_PHY     0x01     //!< LE 1M PHY for synchronization
#define    CS_LE_2M_SYNC_PHY     0x02     //!< LE 2M PHY for synchronization
#define    CS_LE_2M2BT_SYNC_PHY  0x03     //!< LE 2M2BT PHY for synchronization
/** @} */

/** @defgroup CS_RTT_Type Channel Sounding RTT Type
 * @{
 */
// RTT type
#define    RTT_COARSE           0          //!< Access address only mode
#define    RTT_SOUNDING_SEQ_32  1          //!< Sounding sequence of 32 bit, RTT_SOUNDING is not supported
#define    RTT_SOUNDING_SEQ_96  2          //!< Sounding sequence of 96 bit, RTT_SOUNDING is not supported
#define    RTT_RNDM_SEQ_32      3          //!< Random sequence payload of 32 bit
#define    RTT_RNDM_SEQ_64      4          //!< Random sequence payload of 64 bit
#define    RTT_RNDM_SEQ_96      5          //!< Random sequence payload of 96 bit
#define    RTT_RNDM_SEQ_128     6          //!< Random sequence payload of 128 bit
/** @} */

/** @defgroup CS_Role Channel Sounding Role
 * @{
 */
#define    CS_ROLE_INITIATOR  0          //!< Initiator role
#define    CS_ROLE_REFLECTOR  1          //!< Reflector role
/** @} */

/** @defgroup CS_Mode Channel Sounding Procedure Mode
 * @{
 */
#define    CS_MODE_0        0          //!< Mode 0
#define    CS_MODE_1        1          //!< Mode 1
#define    CS_MODE_2        2          //!< Mode 2
#define    CS_MODE_3        3          //!< Mode 3
#define    CS_NON_MODE_0    4          //!< internal usage
#define    CS_MODE_UNUSED   0xFFU       //!< used only in submode
/** @} */

/** @defgroup CS_Chan_Sel_Alg Channel Selection Algorithm
 * @{
 */
#define    CS_CHANNEL_SEL_ALG_3B  0    //!< Channel Selection Algorithm #3b
#define    CS_CHANNEL_SEL_ALG_3C  1    //!< Channel Selection Algorithm #3c
/** @} */

/** @defgroup CS_Phy_supported Channel Sounding PHY Support
 * @{
 */
#define    CS_LE_1M_PHY_SUPPORTED     0x00      //!< LE 1M PHY supported
#define    CS_LE_2M_PHY_SUPPORTED     0x01      //!< LE 2M PHY supported
#define    CS_LE_2M2BT_PHY_SUPPORTED  0x02      //!< LE 2M2BT PHY supported
/** @} */

/** @defgroup CS_Mode_Role Channel Sounding Results Mode
 * @{
 */
#define    MODE_UNKOWN         0x00
#define    MODE_0_INITIATOR    0x01
#define    MODE_0_REFLECTOR    0x02
#define    MODE_1_INIT_REFL    0x03
#define    MODE_2_INIT_REFL    0x04
#define    MODE_3_INIT_REFL    0x05
/** @} */

/** @defgroup CS_Enable Channel Sounding Procedure Enable/Disable
 * @{
 */
#define    CS_DISABLE   0
#define    CS_ENABLE    1
/** @} */


/**
 * Extracts I and Q branches based on pct value (24 bits)
 */
#define CS_EXTRACT_I_BRANCH(pct)    (pct & 0xFFF)
#define CS_EXTRACT_Q_BRANCH(pct)    ((pct >> 12) & 0xFFF)

#define CS_MAX_PERMUTATION_INDEX_1_ANT        0x00U     // Max antenna permutation index for 1 antenna
#define CS_MAX_PERMUTATION_INDEX_2_ANT        0x01U     // Max antenna permutation index for 2 antennas
#define CS_MAX_PERMUTATION_INDEX_3_ANT        0x05U     // Max antenna permutation index for 3 antennas
#define CS_MAX_PERMUTATION_INDEX_4_ANT        0x17U     // Max antenna permutation index for 4 antennas
#define CS_RANGING_MAX_PERMUTATION_INDEX      CS_MAX_PERMUTATION_INDEX_4_ANT

#define CS_MAX_NUM_CONFIG_IDS                  4

/*******************************************************************************
 * ENUMS
 */

// Error Codes
typedef enum csStatus_e {
    CS_STATUS_SUCCESS,
    CS_STATUS_INACTIVE_CONNECTION   = 0x02, /* SPEC defined error code, Inactive connection */
    CS_STATUS_INSUFFICIENT_MEMORY   = 0x07, /* SPEC defined error code, Insufficient memory */
    CS_STATUS_COMMAND_DISALLOWED    = 0x0C, /* SPEC defined error code, command disallowed */
    CS_STATUS_LIMITED_RESOURCES     = 0x0D, /* SPEC defined error code, limited resources */
    CS_STATUS_UNEXPECTED_PARAMETER  = 0x12, /* SPEC defined error code, unexpected parameter */
    CS_STATUS_FEATURE_NOT_SUPPORTED = 0x11, /* SPEC defined error code, feature not supproted */
    CS_STATUS_INVALID_LL_PARAM      = 0x1E, /* SPEC defined error code, invalid LL parameter */
    CS_STATUS_UNSPECIFIED_ERROR     = 0x1F, /* SPEC defined error code, unspecified */
    CS_STATUS_INSUFFICIENT_SECURITY = 0x2F, /* SPEC defined error code, insufficient security */
    CS_STATUS_INVALID_CONN_PTR      = 0xA0, /* Custom CS error codes */
    CS_STATUS_INVALID_BUFFER,
    CS_STATUS_CONNECTION_TERMINATED,
    CS_STATUS_INVALID_PKT_LEN,
    CS_STATUS_INVALID_CHM,
    CS_STATUS_INVALID_CONFIG_ID,
    CS_STATUS_PROCEDURE_IN_PROGRESS,
    CS_STATUS_UNKNOWN_CTRL_PKT,
    CS_STATUS_UNSUPPORTED_FEATURE,
    CS_STATUS_DISABLED_CONFIG_ID,
    CS_STATUS_INVALID_RESULT_CB,
    CS_STATUS_PROCEDURE_DISABLED,
    CS_STATUS_RCL_HANDLE_ERROR,
    CS_STATUS_RCL_COMMAND_ERROR,
    CS_STATUS_RCL_INIT_ERROR,
    CS_STATUS_RCL_SETUP_ERROR,
    CS_STATUS_RCL_SUBMIT_ERROR,
    CS_STATUS_SUBEVENT_SETUP_ERROR,
    CS_STATUS_DRBG_INIT_FAIL,
    CS_STATUS_CONFIG_ENABLED,
    CS_STATUS_INVALID_CHAN_IDX,
    CS_STATUS_INVALID_STEP_MODE,
    CS_STATUS_INVOKE_FUNC_FAIL
} csStatus_e;

typedef enum
{
    ACI_A1_B1 = 0,         //!< Initiator uses 1 antenna, Reflector uses 1 antenna
    ACI_A2_B1 = 1,         //!< Initiator uses 2 antennas, Reflector uses 1 antenna
    ACI_A3_B1 = 2,         //!< Initiator uses 3 antennas, Reflector uses 1 antenna
    ACI_A4_B1 = 3,         //!< Initiator uses 4 antennas, Reflector uses 1 antenna
    ACI_A1_B2 = 4,         //!< Initiator uses 1 antenna, Reflector uses 2 antennas
    ACI_A1_B3 = 5,         //!< Initiator uses 1 antenna, Reflector uses 3 antennas
    ACI_A1_B4 = 6,         //!< Initiator uses 1 antenna, Reflector uses 4 antennas
    ACI_A2_B2 = 7          //!< Initiator uses 2 antennas, Reflector uses 2 antennas
} csACI_e;

/*******************************************************************************
 * STRUCTURES
 */

typedef struct
{
  int8 faeTbl[CS_FAE_TBL_LEN];    //!< FAE table
} csFaeTbl_t;

typedef struct
{
  uint8 channelMap[CS_CHM_SIZE];  //!< Channel map
} csChm_t;

typedef struct
{
    uint8_t  optionalModes;      //!< indicates which of the optional CS modes are supported
    uint8_t  rttCap;             //!< indicate which of the time-of-flight accuracy requirements are met
    uint8_t  rttAAOnlyN;         //!< Number of CS steps of single packet exchanges needed
    uint8_t  rttSoundingN;       //!< Number of CS steps of single packet exchanges needed
    uint8_t  rttRandomPayloadN;  //!< Num of CS steps of single packet exchange needed
    uint16_t nadmSounding;       //!< NADM Sounding Capability
    uint16_t nadmRandomSeq;      //!< NADM Random Sequence Capability
    uint8_t  optionalCsSyncPhy;  //!< supported CS sync PHYs, bit mapped field
    uint8_t  numAntennas;        //!< the number of antenna elements that are available for CS tone exchanges
    uint8_t  maxAntPath;         //!< max number of antenna paths that are supported
    uint8_t  role;               //!< initiator or reflector or both
    uint8_t  rfu0;               //!< reserved for future use
    uint8_t  noFAE;              //!< No FAE
    uint8_t  chSel3c;            //!< channel selection 3c support
    uint8_t  csBasedRanging;     //!< CS based ranging
    uint8_t  rfu1;               //!< reserved for future use
    uint8_t  numConfig;          //!< Number of CS configurations supported per conn
    uint16_t maxProcedures;      //!< Max num of CS procedures supported
    uint8_t  tSwCap;             //!< Antenna switch time capability
    uint16_t tIp1Cap;            //!< tIP1 Capability
    uint16_t tIp2Cap;            //!< tTP2 Capability
    uint16_t tFcsCap;            //!< tFCS Capability
    uint16_t tPmCsap;            //!< tPM Capability
    uint8_t  snrTxCap;           //!< Spec defines an additional byte for RFU
} csCapabilities_t;

typedef struct
{
  uint16_t maxProcedureDur;            //!< duration in 0.625 milliseconds
  uint16_t minProcedureInterval;       //!< min num of conn events between consecutive CS procedures
  uint16_t maxProcedureInterval;       //!< max num of conn events between consecutive CS procedures
  uint16_t maxProcedureCount;          //!< max num of CS procedures to be scheduled (0 - indefinite)
  uint32_t minSubEventLen;             //!< Min SubEvent Len in microseconds, range 1250us to 4s
  uint32_t maxSubEventLen;             //!< Max SubEvent Len in microseconds, range 1250us to 4s
  csACI_e  aci;                        //!< Antenna Config Index
  uint8_t  phy;                        //!< PHY
  uint8_t  txPwrDelta;                 //!< Tx Power Delta, in signed dB
  uint8_t  preferredPeerAntenna;       //!< Preferred peer Antenna
  uint8_t  snrCtrlI;                   //!< SNR Control Initiator
  uint8_t  snrCtrlR;                   //!< SNR Control Reflector
  uint8_t  enable;                     //!< is Procedure Enabled
} csProcedureParams_t;

typedef struct
{
  uint8_t mainMode;                  //!< Main mode of operation
  uint8_t subMode;                   //!< Sub mode of operation
  uint8_t mainModeMinSteps;          //!< Minimum steps for main mode
  uint8_t mainModeMaxSteps;          //!< Maximum steps for main mode
  uint8_t mainModeRepetition;        //!< Repetition count for main mode
  uint8_t modeZeroSteps;             //!< Steps for mode zero
  uint8_t role;                      //!< Role of the device
  uint8_t rttType;                   //!< RTT type
  uint8_t csSyncPhy;                 //!< PHY for CS sync
  csChm_t channelMap;                //!< channel map.
  uint8_t chMRepetition;             //!< Channel map repetition
  uint8_t chSel;                     //!< Channel selection
  uint8_t ch3cShape;                 //!< 3C shape
  uint8_t ch3CJump;                  //!< 3C jump
} csConfigBuffer_t;

#endif // CS_TYPES_H
