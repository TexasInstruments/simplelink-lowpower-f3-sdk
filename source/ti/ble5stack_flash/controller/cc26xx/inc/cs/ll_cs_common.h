/******************************************************************************

 @file  ll_cs_common.h

 @brief Channel Sounding common header

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2024, Texas Instruments Incorporated

 All rights reserved not granted herein.
 Limited License.

 Texas Instruments Incorporated grants a world-wide, royalty-free,
 non-exclusive license under copyrights and patents it now or hereafter
 owns or controls to make, have made, use, import, offer to sell and sell
 ("Utilize") this software subject to the terms herein. With respect to the
 foregoing patent license, such license is granted solely to the extent that
 any such patent is necessary to Utilize the software alone. The patent
 license shall not apply to any combinations which include this software,
 other than combinations with devices manufactured by or for TI ("TI
 Devices"). No hardware patent is licensed hereunder.

 Redistributions must preserve existing copyright notices and reproduce
 this license (including the above copyright notice and the disclaimer and
 (if applicable) source code license limitations below) in the documentation
 and/or other materials provided with the distribution.

 Redistribution and use in binary form, without modification, are permitted
 provided that the following conditions are met:

   * No reverse engineering, decompilation, or disassembly of this software
     is permitted with respect to any software provided in binary form.
   * Any redistribution and use are licensed by TI for use only with TI Devices.
   * Nothing shall obligate TI to provide you with source code for the software
     licensed and provided to you in object code.

 If software source code is provided to you, modification and redistribution
 of the source code are permitted provided that the following conditions are
 met:

   * Any redistribution and use of the source code, including any resulting
     derivative works, are licensed by TI for use only with TI Devices.
   * Any redistribution and use of any object code compiled from the source
     code and any resulting derivative works, are licensed by TI for use
     only with TI Devices.

 Neither the name of Texas Instruments Incorporated nor the names of its
 suppliers may be used to endorse or promote products derived from this
 software without specific prior written permission.

 DISCLAIMER.

 THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/
#ifndef LL_CS_COMMON_H
#define LL_CS_COMMON_H
/*******************************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "ll_common.h"

#include <ti/drivers/rcl/commands/ble_cs.h>
#include <ti/drivers/rcl/handlers/ble_cs.h>
/*******************************************************************************
 * CONSTANTS
 */
// Lengthes & Sizes
#define CS_CSIV_LEN                            16
#define CS_CSIN_LEN                            8
#define CS_CSPV_LEN                            CS_CSIV_LEN
#define CS_PV_C_LEN                            CS_CSPV_LEN/2
#define CS_IN_C_LEN                            CS_CSIN_LEN/2
#define CS_IV_C_LEN                            CS_CSIV_LEN/2
#define CS_FAE_TBL_LEN                         72 // octets
#define CS_CHM_SIZE                            10U // octets
#define CS_FILTERED_CHAN_MAX_SIZE              72
#define CS_RNDM_SIZE                           16U
#define CS_MAX_NUM_CONFIG_IDS                  4
#define CS_DRBG_NUM_BITS                       128
#define CS_MAX_TRANSACTION_IDS                 10
#define CS_MAX_ALLOWED_CH_IDX                  70
#define CS_MIN_NUM_OF_CHN                      15
#define CS_MAX_PROCEDURE_LEN                   0xFFFF  // *0.625ms = 40s
#define CS_MIN_SUBEVENT_LEN                    1250    // us
#define CS_MAX_SUBEVENT_LEN                    4000000 // us
#define CS_RESULT_EVENT_HEADER_LEN             16      // The fields of the subevent results event
#define CS_CONTINUE_RESULT_EVENT_HEADER_LEN    9
#define CS_STEP_BUFF_MAX_SIZE                  30U
#define CS_MAX_RESULT_DATA_LEN                 15
#define CS_MIN_STEPS_PER_SUBEVENT              2   // SPEC definition
#define CS_MAX_STEPS_PER_SUBEVENT              160 // SPEC definition
#define CS_MAX_STEPS_PER_PROCEDURE             256U // SPEC definition
#define CS_RES_BUFF_MAX_SIZE                   CS_MAX_STEPS_PER_SUBEVENT // number of step results in the buffer.
#define CS_SUBEVENT_RESULTS_LEN                sizeof(RCL_CmdBleCs_SubeventResults)
#define CS_SUBEVENT_RESULTS_CONT_LEN           sizeof(RCL_CmdBleCs_SubeventResults) - 7
#define CS_SUBEVENT_HDR_LEN                    4   // Subevent Code, ConnHandle, ConfigId
#define CS_SUBEVENT_PROC_INFO_LEN              7   // ConnEvt, ProcCount,FreqComp, PwrLevel
#define CS_SUBEVENT_CONT_IRR_INFO              5   // exlude this size from the subevent continue
#define CS_STEP_RES_HEADER_LEN                 3   // mode, channel, dataLen
#define HCI_STEP_DESCRIPTORS_SIZE              3   // the 3 bytes describing the steps: mode, channel, size
#define CS_RESULT_DATA_SIZE                    CS_SUBEVENT_RESULTS_LEN +\
                                               (CS_MAX_RESULT_DATA_LEN+3)*CS_RES_BUFF_MAX_SIZE
#define CS_RESULT_BUFF_SIZE                    sizeof(RCL_MultiBuffer) +\
                                               CS_RESULT_DATA_SIZE
#define CS_MIN_EVENT_INTERVAL                  3
#define CS_MIN_PROCEDURE_INTERVAL              3
#define CS_MAX_PROCEDURE_INTERVAL              0xFFFF


// Capabilities
#define CS_MAX_NUM_CONFIG_SUPPORTED            0x04
#define CS_INDEFINITE_PROCEDURES_SUPPORTED     0x00
#define CS_MAX_NUM_ANT_SUPPORTED               0x01
#define CS_MAX_ANT_PATH_SUPPORTED              0x01
#define CS_MODE_3_SUPPORTED                    0x01
#define CS_NUM_CS_SYNC_EXCHANGES_SUPPORTED     0xFF // number of CS sync exchanges required to satisfy the percision requirements
#define CS_OPTIONAL_PHY_SUPPORTED              0x03 // 2M PHY and 2M BT PHY
#define CS_RTT_CAPABILITY_NOT_SUPPORTED        0x00
#define CS_CAPABILITY_NOT_SUPPORTED            0

// RTT supported, if bit is zero, the RTT requirement is 150 ns, otherwise, it's 10 ns.
#define CS_RTT_SUPPORTED                       0x00 // bit0 - rtt AA Only, bit, bit1-Sounding, bit2 - random

// CS Times

// CS Time Types
#define CS_T_IP1 0
#define CS_T_IP2 1
#define CS_T_FCS 3
#define CS_T_PM  4

// CS Time Capabilities
// The following are bitwise values.
// Each bit that is on, means that the time requirement is supported
// The mandatoy requirements are taken by default and are not part
// of this bitwise number.
#define CS_T_IP1_IP2_CAP          0x0040 // supported: 80us, (145us Mandatory)
#define CS_T_FCS_CAP              0x0180 // supported: 100us, 120us, (150us Mandatory)
#define CS_T_PM_CAP               0x03   // supported: 10us, 20us , (40us Mandatory)
#define CS_T_SW_CAP               0x04   // antenna switch period

// Mandatory Values bit index
#define CS_MANDATORY_TIP_IDX                     7
#define CS_MANDATORY_TFCS_IDX                    9
#define CS_MANDATORY_TPM_IDX                     2
#define CS_MANDATORY_TSW                         2
#define CS_INVALID_TIME_INDX                     0xFF

// Time in microseconds for the antenna switch period
#define CS_ANTENNA_SW_PERIOD_0                0x01
#define CS_ANTENNA_SW_PERIOD_1                0x02
#define CS_ANTENNA_SW_PERIOD_2                0x04
#define CS_ANTENNA_SW_PERIOD_3                0x0A

// Possible Values for Timing Capability
#define CS_T_0US                              0x00
#define CS_T_1US                              0x01
#define CS_T_2US                              0x02
#define CS_T_4US                              0x04
#define CS_T_10US                             0x0A
#define CS_T_15US                             0x0F
#define CS_T_20US                             0x14
#define CS_T_30US                             0x1E
#define CS_T_40US                             0x28
#define CS_T_50US                             0x32
#define CS_T_60US                             0x3C
#define CS_T_80US                             0x50
#define CS_T_100US                            0x64
#define CS_T_120US                            0x78
#define CS_T_145US                            0x91
#define CS_T_150US                            0x96

/* CS Event Offset
 * The time from the ACl anchor point to
 * The start of the first CS subevent
 * The min value must be betwen 500us to 4s
 * The actual max value can be up to the connInterval*/
#define CS_OFFSET_MIN                          9000 // this is the minimun we can handle ATM

// Other Definitions
#define CS_SEC_NO_OFFSET                       0
#define CS_SEC_USE_OFFSET                      1
#define CS_CONFIG_EXISTS                       0
#define CS_CONFIG_REMOVED                      1
#define CHM_NO_CHANGE                          0
#define CHM_CHANGE                             1
#define INVALID_CONFIG_ID                      0x3FU
#define INVALID_CS_CHANNEL_IDX                 0
#define MAX_ANTENNA_PATHS                      4
#define CS_TRANSACTION_NUM                     10
#define CS_DEFAULT_TFM                         80
#define CS_FC_UNAVAILABLE                      0xC000
#define CS_NUM_ACI                             8
#define CS_NUM_ROLES                           2
#define CS_NUM_TIP_OPTIONS                     8
#define CS_NUM_TFCS_OPTIONS                    10
#define CS_NUM_TPM_OPTIONS                     4
#define CS_USE_MIN_TX_POWER                    0x7F
#define CS_USE_MAX_TX_POWER                    0x7E
#define CS_3_OCTETS_MASK                       0x00FFFFFF
#define CS_RFU                                 0x0U
#define CS_CTRL_PKT_OFFSET                     17U
#define CS_8_BITS_SIZE                         8U
#define CS_1_BYTE_MASK                         0xFFU

// Procedure complete flag reset mask
#define CS_RESET_START_PROCEDURE_FLAG          0x1F
#define CS_RESET_CHM_UPDATE_FLAG               0x2F
#define CS_RESET_CONFIG_FLAG                   0x3B

// Default Procedure Params
#define CS_DEFAULT_PROCEDURE_DUR               CS_MAX_PROCEDURE_LEN
#define CS_DEFAULT_MIN_PROC_INTERVAL           1
#define CS_DEFAULT_MAX_PROC_INTERVAL           3
#define CS_DEFAULT_MAX_PROC_COUNT              0xFFFF
#define CS_DEFAULT_MIN_SUBEVENT_LEN            0x4E2U    // 1250 us
#define CS_DEFAULT_MAX_SUBEVENT_LEN            0x3D0900U //us = 4s
#define CS_DEFAULT_TONE_ANTENNA_CFG            1
#define CS_DEFAULT_PHY                         1
#define CS_DEFAULT_TX_PWR_DELTA                0
#define CS_DEFAULT_PEER_ANTENNA                0
#define CS_DEFAULT_ENABLE                      CS_DISABLE
#define CS_DEFAULT_TERMINATE_STATE             CS_TERMINATE_DISABLE
#define CS_PCT_MASK                            0xFFF

// CS tx_power range
#define CS_MAX_TX_POWER_VALUE                  20
#define CS_MIN_TX_POWER_VALUE                  -127

/* Max Steps Per HCI event */
#define MAX_STEPS_PER_HCI_EVENT 9
#define MAX_RESULT_SIZE         245

/*******************************************************************************
 * ENUMS
 */
typedef enum csPhysSupported_e
{
    CS_LE_1M_PHY_SUPPORTED = 0x00,
    CS_LE_2M_PHY_SUPPORTED = 0x01
} csPhysSupported_e;

typedef enum csSyncPhy_e
{
    CS_LE_1M_SYNC_PHY = 0x01,
    CS_LE_2M_SYNC_PHY = 0x02
} csSyncPhy_e;

typedef enum csEnable_e
{
    CS_DISABLE = 0,
    CS_ENABLE = 1,
} csEnable_e;

// Flag that indicate that the procedure state should be
// updated to CS_DISABLE at the end of the current procedure
typedef enum csTerminateState_e
{
    CS_TERMINATE_DISABLE = 0U,       /* Indicate that the enable field shouldn't be changed*/
    CS_TERMINATE_RECEIVED = 1U
} csTerminateState_e;

// CS Role Mask
typedef enum csRoleMask_e
{
    CS_INITIATOR_MASK = 0x01,
    CS_REFLECTOR_MASK = 0x02
} csRoleMask_e;

// CS Roles
typedef enum csRole_e
{
    CS_ROLE_INITIATOR = 0,
    CS_ROLE_REFLECTOR = 1
} csRole_e;

// Modes
typedef enum csMode_e
{
    CS_MODE_0 = 0,
    CS_MODE_1 = 1,
    CS_MODE_2 = 2,
    CS_MODE_3 = 3,
    CS_NON_MODE_0 = 4,
    CS_MODE_UNUSED = 0xFF
} csMode_e;

// CS Ctrl Procedures
typedef enum csProcedures_e
{
    CS_NO_ACTIVE_PROCEDURE             = 0x00,
    CS_SECURITY_PROCEDURE              = 0x01,
    CS_CAPABILITIES_EXCHANGE_PROCEDURE = 0x02,
    CS_CONFIG_PROCEDURE                = 0x04,
    CS_FAE_TABLE_UPDATE_PROCEDURE      = 0x08,
    CS_CHM_UPDATE_PROCEDURE            = 0x10,
    CS_START_PROCEDURE                 = 0x20,
    CS_IND                             = 0x40,
    CS_TERMINATE_PROCEDURE             = 0x80
} csProcedures_e;

// CS Sub Features
typedef enum csSubFeatures_e
{
    CS_COMPANION_SIGNAL                  = 0x01, // bit 0 of sub features
    CS_NO_TRANSMITTER_FREQ_ACTUATION_ERR = 0x02, // bit 1 of sub features
    CS_CHANNLE_SELECTION_ALG_3C          = 0x04, // bit 2 of sub features
    CS_PHASE_BASED_RANGING               = 0x08  // bit 3 of sub features
} csSubFeatures_e;

typedef enum csTransactionId_e
{
    CS_TID_CHANNEL_SELECTION_NON_MODE_0,
    CS_TID_CHANNEL_SELECTION_MODE_0,
    CS_TID_SUB_MODE_INSERTION,
    CS_TID_CS_TONE_SLOT,
    CS_TID_ANTENNA_PATH_PERMUTATION,
    CS_TID_AA_GENERATION,
    CS_TID_SOUNDING_SEQUENCE_POSITION,
    CS_TID_SOUNDING_SEQUENCE_SELECTION,
    CS_TID_RANDOM_SEQUENCE_GENERATION,
    CS_TID_RANDOM_BACKTRACKING_RESISTENCE,
} csTransactionId_e;

typedef enum csRttType_e
{
    RTT_COARSE,
    RTT_SOUNDING_SEQ_32,
    RTT_SOUNDING_SEQ_96,
    RTT_RNDM_SEQ_32,
    RTT_RNDM_SEQ_64,
    RTT_RNDM_SEQ_96,
    RTT_RNDM_SEQ_128,
} csRttType_e;

typedef enum csChannelSelectionAlgs_e
{
    CS_CHANNEL_SEL_ALG_3B,
    CS_CHANNEL_SEL_ALG_3C,
} csChannelSelectionAlgs_e;

typedef enum csChannelIndexArray_e
{
    CS_MODE_0_CHANNEL_INDX_ARR,
    CS_NON_MODE_0_CHANNEL_INDX_ARR,
} csChannelIndexArray_e;

typedef enum csModeRole_e
{
    MODE_UNKOWN,
    MODE_0_INITIATOR,
    MODE_0_REFLECTOR,
    MODE_1_INIT_REFL,
    MODE_2_INIT_REFL,
    MODE_3_INIT_REFL,
} csModeRole_e;

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
    CS_STATUS_INVALID_STEP_MODE
} csStatus_e;

typedef enum csProcDoneStat_e
{
    CS_REPORT_DONE     = 0x0,
    CS_REPORT_NOT_DONE = 0x1,
    CS_ABORTED         = 0xF
} csProcDoneStat_e;

typedef enum csAbortReason_e
{
    CS_NO_ABORT              = 0x0,
    CS_ABORT_REQUEST         = 0x1,
    CS_ABORT_CHM             = 0x2,
    CS_ABORT_INSTANT_PASSED  = 0x3,
    CS_ABORT_UNSPECIFIED     = 0xF
} csAbortReason_e;

typedef enum csBleRole_e
{
    CS_BLE_ROLE_NOT_SET,
    CS_BLE_ROLE_CENTRAL,
    CS_BLE_ROLE_PERIPHERAL
} csBleRole;

typedef enum csSubeventInfo_e
{
    CS_SE_INFO_NUM_STPES,
    CS_SE_INFO_STEP_COUNT,
    CS_SE_INFO_REPORT_COUNT
} csSubeventInfo_e;

typedef enum csProcedureCounter_e
{
    CS_PROC_INFO_SUBEVENT_C,
    CS_PROC_INFO_EVENT_C,
    CS_PROC_C,
    CS_PROC_ALL_C
} csProcedureCounter_e;

typedef enum csACI_e
{
    ACI_A1_B1 = 0,
    ACI_A2_B1 = 1,
    ACI_A3_B1 = 2,
    ACI_A4_B1 = 3,
    ACI_A1_B2 = 4,
    ACI_A1_B3 = 5,
    ACI_A1_B4 = 6,
    ACI_A2_B2 = 7
} csACI_e;

typedef enum csNewSubevent_e
{
    CS_NEW_SUBEVENT,
    CS_CONTINUE_SUBEVENT
} csNewSubevent_e;

typedef enum csNextSubevent_e
{
    CS_PREP_CURR_SUBEVENT,
    CS_PREP_NEXT_SUBEVENT,
} csNextSubevent_e;

/*******************************************************************************
 * MACROS
 */

#define RESET_CS_FLAG(connId, flagId)                                          \
    (llCs[connId].completedProcedures &= flagId)
#define CS_GET_BIT(val, bitIdx) ((val >> bitIdx) & 1)
#define CS_SHIFT_RIGHT(val, n) (val >> n)
#define T625MS2US(x) x * 625

// This Macro will check if any of channels 0-1, 23-25, 77-78 are used (they
// shouldn't be)
#define CS_TEST_CHANNEL_MAP_RESTRICTED(channelMap)                             \
    (channelMap[0] & 0x03 || channelMap[2] & 0x80 || channelMap[3] & 0x03 ||   \
     channelMap[9] & 0xE0)

#define GET_MAX_STEP_DATA_LEN(numAntPaths)                                     \
    (SIZE_MODE_3_INIT_REFL + (numAntPaths + 1) * 4)

/* Tone Extension Bits are used in their reversed order */
#define CS_REVERSE_TONE_EXTENSION_BITS(bits)                                   \
    ((((bits & 0x01U) << 1U) | ((bits & 0x02U) >> 1U)) & 0x3U)

/* Determine Number of steps in buffer */
#define CS_NUM_BUFF_STEPS(nSteps) ((nSteps > CS_STEP_BUFF_MAX_SIZE) ?            \
                                                CS_STEP_BUFF_MAX_SIZE : nSteps)

/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * TYPEDEFS
 */

typedef struct
{
    int8 faeTbl[CS_FAE_TBL_LEN];
} csFaeTbl_t;

typedef struct
{
    uint8 channelMap[CS_CHM_SIZE];
} csChm_t;

typedef struct
{
    uint8 update:1;        // update flag
    uint32 lastUpdateTime; // interval between two successive updates sent shall be at least 1 second
} csChmUpdate_t;

struct rndmBitCache_t
{
    uint8 rndBits[CS_RNDM_SIZE];
    uint8 numBitsUsed;
} __attribute__((packed));
// Typedef separated from packed struct due to MisraC
typedef struct rndmBitCache_t rndmBitCache_t;

typedef struct
{
    uint8 CSIV[CS_CSIV_LEN];
    uint8 CSIN[CS_CSIN_LEN];
    uint8 CSPV[CS_CSPV_LEN];
} csSecVectors_t;

struct csCapabilities_t
{
    uint8 optionalModes;      /* indicates which of the optional CS mdes are supported */
    uint8 rttCap;             /* indicate which of the time-of-flight accuracy requirements are met */
    uint8 rttAAOnlyN;         /* Number of CS steps of single packet exchanges needed */
    uint8 rttSoundingN;       /* Number of CS steps of single packet exchanges needed */
    uint8 rttRandomPayloadN;  /* Num of CS steps of single packet exchange needed */
    uint16 nadmSounding;      /* NADM Sounding Capability */
    uint16 nadmRandomSeq;     /* NADM Random Sequence Capability */
    uint8 optionalCsSyncPhy;  /* supported CS sync PHYs, bit mapped field */
    uint8 numAntennas:4;      /* the number of antenna elements that are available for CS tone exchanges */
    uint8 maxAntPath:4;       /* max number of antenna paths that are supported */
    uint8 role:2;             /* initiator or reflector or both */
    uint8 rfu0:1;
    uint8 noFAE:1;
    uint8 chSel3c:1;          /* channel selection 3c support */
    uint8 csBasedRanging:1;
    uint8 rfu1:2;
    uint8 numConfig;          /* Number of CS configurations supported per conn */
    uint16 maxProcedures;     /* Max num of CS procedures supported */
    uint8  tSwCap;            /* Antenna switch time capability */
    uint16 tIp1Cap;           /* tIP1 Capability */
    uint16 tIp2Cap;           /* tTP2 Capability */
    uint16 tFcsCap;           /* tFCS Capability */
    uint16 tPmCsap;           /* tPM Capability  */
    uint8  snrTxCap;          /* Spec defines an additional byte for RFU */
} __attribute__((packed));
// Typedef separated from packed struct due to MisraC
typedef struct csCapabilities_t csCapabilities_t;

struct csConfig_t
{
    uint8 configId:6;         /* CS configuration ID */
    uint8 state:2;            /* 0b00 disabled, 0b01 enabled */
    csChm_t channelMap;       /* channel map. */
    uint8 chMRepetition;      /* number of times the ChM field will be cycled through */
                              /* for non-mode 0 steps within a CS procedure */
    uint8 mainMode;           /* which CS modes are to be used */
    uint8 subMode;            /* which CS modes are to be used */
    uint8 mainModeMinSteps;   /* range of Main_Mode steps to be executed before */
                              /* a Sub_Mode step is executed */
    uint8 mainModeMaxSteps;
    uint8 mainModeRepetition; /* num of main mode steps from the last CS subevent to be repeated */
    uint8 modeZeroSteps;      /*  number of mode 0 steps to be included at the beginning of each CS Subevent */
    uint8 csSyncPhy;          /* transmit and receive PHY to be used */
    uint8 rttType:4;          /* which RTT variant is to be used */
    uint8 role:2;
    uint8 rfu0:2;
    uint8 chSel:4;            /* channel selection algorithm to be used */
    uint8 ch3cShape:4;        /* selected shape to be rendered */
    uint8 ch3CJump;           /* one of the valid CSChannelJump values defined in table 32? */
    uint8 tIP1;               /* Index of the period used between RTT packets */
    uint8 tIP2;               /* Index of the interlude period used between CS tones */
    uint8 tFCs;               /* Index used for frequency changes */
    uint8 tPM;                /* Index for the measurement period of CS tones */
    uint8 rfu1;
} __attribute__((packed));
// Typedef separated from packed struct due to MisraC
typedef struct csConfig_t csConfigurationSet_t;

typedef struct
{
    uint8 roleEn;
    uint8 csSyncAntennaSelection;
    int8  maxTxPower;
} csDefaultSettings_t;

typedef struct
{
    uint16 maxProcedureDur;           /* duration in 0.625 miliseconds */
    uint16 minProcedureInterval;      /* min num of conn events between consecutive CS procedures */
    uint16 maxProcedureInterval;      /* max num of conn events between consecutive CS procedures */
    uint16 maxProcedureCount;         /* max num of CS procedures to be scheduled (0 - indefinite) */
    uint32 minSubEventLen;            /* Min SubEvent Len in microseconds, range 1250us to 4s */
    uint32 maxSubEventLen;            /* Max SubEvent Len in microseconds, range 1250us to 4s */
    csACI_e toneAntennaConfigSelection; /* Antenna Config Index */
    uint8 phy;                        /* PHY */
    uint8 txPwrDelta;                 /* Tx Power Delta, in signed dB */
    uint8 preferredPeerAntenna;       /* Preferred peer Antenna */
    uint8 snrCtrlI;                   /* SNR Control Initiator */
    uint8 snrCtrlR;                   /* SNR Control Reflector */
    uint8 enable:1;                   /* is Procedure Enabled */
} csProcedureParams_t;

struct csProcedureEnable_t
{
    uint8   configId:6;              /* REQ | RSP | IND */
    uint8   rfu:2;                   /* REQ | RSP | IND */
    uint16  connEventCount;          /* REQ | RSP | IND */
    uint32  offset;                  /*  X  |  X  | IND */
    uint32  offsetMin;               /* REQ | RSP |  X  */
    uint32  offsetMax;               /* REQ | RSP |  X  */
    uint16  maxProcedureDur;         /* REQ |  X  |  X  */
    uint16  eventInterval;           /* REQ | RSP | IND */ /* units of connInt */
    uint8   subEventsPerEvent;       /* REQ | RSP | IND */ /* num of CS SubEvents in a CS Event */
    uint16  subEventInterval;        /* REQ | RSP | IND */ /* units 625 us*/
    uint32  subEventLen;             /* REQ | RSP | IND */ /* units microseconds, range 1250us to 4s */
    uint16  procedureInterval;       /* REQ |  X  |  X  */
    uint16  procedureCount;          /* REQ |  X  |  X  */
    csACI_e ACI;                     /* REQ | RSP | IND */
    uint8   preferredPeerAntenna;    /* REQ |  X  |  X  */
    uint8   phy;                     /* REQ | RSP | IND */
    uint8   pwrDelta;                /* REQ | RSP | IND */
    uint8   txSnrI:4;                /* REQ |  X  |  X  */
    uint8   txSnrR:4;                /* REQ |  X  |  X  */
} __attribute__((packed));
// Typedef separated from packed struct due to MisraC
typedef struct csProcedureEnable_t csProcedureEnable_t;

// Channel Map
typedef struct
{
    uint8* shuffledChanIdxArray; /* Channel Index Array */
    uint8 numChanUsed; /* Channel used as we iterate over the channels  */
    uint8 numRepetitions; /* number of times the channel array was repeated */
    union {
        uint8 selectionAlgo;
        uint8 rfu;
    };
} modeSpecificChanInfo_t;

typedef struct
{
    uint8 numChans;
    uint8 filteredChanArr[72];
    modeSpecificChanInfo_t mode0;
    modeSpecificChanInfo_t nonMode0;
} csChanInfo_t;

typedef union
{
    uint8 subEventInfo[3];      /* {subEventNumSteps, stepCount, reportCount} */
    struct
    {
        uint32_t allInfo;
    };
} csSubeventInfo_t;

typedef struct
{
    union
    {
        struct
        {
            uint16 counters[3];     /* subevent, event, procedure */
        };
        struct
        {
            uint16 subeventCounter;  /* subevent counter */
            uint16 eventCounter;     /* event counter */
            uint16 procedureCounter; /* procedure counter */
        };
    };
} csProcCnt_t;

typedef struct
{
    csProcCnt_t counters;
    uint16      mMStepsRemain;      /* Number of main mode steps remain to be done. When this reaches 0, the procedure ends. */
    uint16      eventsPerProcedure; /* MAX_PROC_LEN / (EVENT_INTERVAL) * connEvent */
    uint32_t    eventAnchorPoint;   /* The time from which consecutive subevents are anchored. */
    uint8       nextProcedure;      /* Marks if a procedure is done and we need to prepare another */
    csNextSubevent_e nextSubevent;  /* Marks if a subevent is submitted and we need to prepare another. */
} csProcedureInfo_t;

typedef struct csTerminateInfo
{
    csTerminateState_e terminateState; /* Flag indicating whether to terminate the upcoming CS Procedure */
    uint16 procedureCnt;   /* CS Procedure Count by the time this is executed */
    uint8 errorCode;       /* The reason the CS procedure will be terminated */
} csTerminateInfo_t;

typedef struct
{
    uint8 completedProcedures;                                      /* Bitmap of completed procedures*/
    uint8 activeCsCtrlProcedure;                                    /* Active CS Ctrl Procedure */
    uint16 csProcCounter;                                           /* CS procedure counter */
    uint8 currentConfigId;                                          /* Current Config ID */
    csFaeTbl_t* peerFaeTbl;                                         /* Peer FAE table */
    csSecVectors_t securityVectors;                                 /* Security vectors */
    csCapabilities_t peerCapabilities;                              /* Peer capabilities */
    csDefaultSettings_t defaultSettings;                            /* CS Default Settings */
    csConfigurationSet_t configSet[CS_MAX_NUM_CONFIG_IDS];          /* CS config */
    csProcedureParams_t procedureParams[CS_MAX_NUM_CONFIG_IDS];     /* Procedure Params */
    csProcedureEnable_t procedureEnableData[CS_MAX_NUM_CONFIG_IDS]; /* Procedure Enable */
    csChanInfo_t filteredChanIdx[CS_MAX_NUM_CONFIG_IDS];            /* Channel Index Array */
    csSubeventInfo_t subEventInfo;                                  /* Subevent Info */
    csProcedureInfo_t procedureInfo;                                /* Procedure Info */
    csTerminateInfo_t terminateInfo;                                /* Procedure Termination Info */
} llCs_t;

typedef struct csSubeventRes
{
    uint8  subeventCode;
    uint16 connectionHandle;
    uint8  configID;
    uint16 startAclConnectionEvent;
    uint16 procedureCounter;
    int16  frequencyCompensation;
    uint8  referencePowerLevel;
    uint8  procedureDoneStatus;
    uint8  subeventDoneStatus;
    uint8  abortReason;
    uint8  numAntennaPath;
    uint8  numStepsReported;
    uint8  data[];
} csSubeventRes_t;

typedef struct csContSubeventRes
{
    uint8  subeventCode;
    uint16 connectionHandle;
    uint8  configID;
    uint8  procedureDoneStatus;
    uint8  subeventDoneStatus;
    uint8  abortReason;
    uint8  numAntennaPath;
    uint8  numStepsReported;
    uint8  data[];
} csContSubeventRes_t;

/* Mode, Channel, DataLen */
struct csStepResHdr_t
{
    uint8 mode;
    uint8 channel;
    uint8 dataLen;
} __attribute__((packed));
// Typedef separated from packed struct due to MisraC
typedef struct csStepResHdr_t csStepResHdr_t;

struct csRfFlags_t
{
    uint8 stepsBuffDone:1;
    uint8 resBuffDone:1;
    uint8 swTriggered:1;
    uint8 csSubeventDone:1;
} __attribute__((packed));
// Typedef separated from packed struct due to MisraC
typedef struct csRfFlags_t csRfFlags_t;

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          llCsCheckConnection
 *
 * @brief       Does basic validation for parameters that are checked
 * repetetively in multiple APIs.
 * This checks if the connection Handle is valid and active
 * And if CS is supported for this connection.
 *
 * @design      BLE_LOKI-506
 * input parameters
 *
 * @param       connId - connection handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              LL_STATUS_ERROR_BAD_PARAMETER if the connection handle is invalid
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION if the connection is inactive
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED is CS is not supported for
 *                                                    this connection
 */
csStatus_e llCsCheckConnection(llConnState_t* connPtr);

/*******************************************************************************
 * @fn          llCsConfigurationCheck
 *
 * @brief       Checks if the CS configuration complies with the remote
 *              and local capabilities.
 *
 * input parameters
 *
 * @param       connId - connection handle
 * @param       pBuf - pointer to CS configuration
 *
 * @design      BLE_LOKI-506
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if parameters are not
 *              supported
 */
csStatus_e llCsConfigurationCheck(uint16 connId, void* pBuf);

/*******************************************************************************
 * @fn          llCsCheckACI
 *
 * @brief       Check if the value of ACI complies with capabilities.
 *            * Get the N_AP of initiator and reflector of given ACI
 *            * Check that the N_AP's of the ACI do not exceed those
 *              in the capabilties, respectively.
 *
 * input parameters
 *
 * @param       ACI - Anetnna Config Index
 * @param       configId - cs configID
 * @param       connID - connection ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
csStatus_e llCsCheckACI(csACI_e ACI, uint8 configId, uint16 connId);

/*******************************************************************************
 * @fn          llCsConfigIdSafeToUse
 *
 * @brief       Check if a config Id exists, and safe to use.
 * That is, it is not already in use by a procedure and was not not removed
 * by Remove Config command
 *
 * input parameters
 *
 * @param       connId - connection handle
 * @param       configId - config Id
 *
 * @design      BLE_LOKI-506
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER if configID doesnt exist
 *              CS_STATUS_CONFIG_ENABLED - if CONFIG is already in use by
 *                                         by another procedure
 *              LL_STATUS_SUCCESS otherwise
 */
csStatus_e llCsConfigIdSafeToUse(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsConfigIdCheck
 *
 * @brief       Check ConfigId range and state
 *
 * input parameters
 *
 * @param       connId - connection Id
 * @param       configId - CS config ID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_STATUS_UNEXPECTED_PARAMETER if config ID is invalid
 *              CS_STATUS_DISABLED_CONFIG_ID  - if its state is disabled
 *              CS_STATUS_SUCCESS - otherwise
 */
csStatus_e llCsConfigIdCheck(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsNumOnBit
 *
 * @brief       Count the amount of one Bit in the array.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pBitMapArray            - channel bit map.
 * @param       amountOfBytesInMapArray - the amount of bytes in pBitMapArray.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      chCount                 - amount of valid channels
 */
uint8 llCsNumOnBit(uint8* pBitMapArray, uint8 amountOfBytesInMapArray);

/*******************************************************************************
 * @fn          llCsGetAbortReason
 *
 * @brief       Get CS Abort Reason
 * This function returns the abort reason based on the CS Terminate error code
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - Connection ID
 *
 * output parameters
 *
 * @param       None
 *
 * @return      abortReason
 */
uint8 llCsGetAbortReason(uint8 connId);

/*******************************************************************************
 * @fn          llCsGetProcDoneStatus
 *
 * @brief       Get ProcedureDoneStatus
 * This function checks if a CS procedure repetition was aborted and updates the
 * ProcedureDoneStatus.
 * Otherwise, the current status remains the same
 *
 * input parameters
 *
 * @param       isProcedureDone - Original Procedure Done Status
 * @param       abortReason - CS Procedure Abort Reason
 *
 * output parameters
 *
 * @param       None
 *
 * @return      ProcedureDoneStatus
 */
uint8 llCsGetProcDoneStatus(uint8 isProcedureDone, uint8 abortReason);

/*******************************************************************************
 * @fn          llCsDbGetNumStepsInBuffer
 *
 * @brief       llCsDbGetNumStepsInBuffer
 *
 * input parameters
 *
 * @param       connId - Connection Identifier
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Num Steps in Buffer
 */
uint8 llCsGetNumStepsInBuffer(uint16 connId);

#endif // LL_CS_COMMON_H
