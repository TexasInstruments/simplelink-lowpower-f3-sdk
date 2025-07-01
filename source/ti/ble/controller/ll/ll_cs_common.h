/******************************************************************************

 @file  ll_cs_common.h

 @brief Channel Sounding common header

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2025, Texas Instruments Incorporated

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
#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/stack_util/cs_types.h"

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
#define CS_FILTERED_CHAN_MAX_SIZE              72
#define CS_RNDM_SIZE                           16U
#define CS_DRBG_NUM_BITS                       (CS_RNDM_SIZE * BITS_PER_BYTE)
#define CS_MAX_TRANSACTION_IDS                 10
#define CS_MAX_ALLOWED_CH_IDX                  70
#define CS_MIN_NUM_OF_CHN                      15
#define CS_MAX_PROCEDURE_LEN                   0xFFFF  // *0.625ms = 40s
#define CS_MIN_SUBEVENT_LEN                    1250    // us
#define CS_MAX_SUBEVENT_LEN                    4000000 // us
#define CS_RESULT_EVENT_HEADER_LEN             16      // The fields of the subevent results event
#define CS_CONTINUE_RESULT_EVENT_HEADER_LEN    9
#define CS_MIN_STEPS_PER_SUBEVENT              2   // SPEC definition
#define CS_MAX_STEPS_PER_SUBEVENT              160 // SPEC definition
#define CS_MAX_STEPS_PER_PROCEDURE             256U // SPEC definition
#define CS_MIN_EVENT_INTERVAL                  3
#define CS_MIN_PROCEDURE_INTERVAL              3
#define CS_MAX_PROCEDURE_INTERVAL              0xFFFF
#define CS_MIN_SUBEVENTS_PER_EVENT             1U
#define CS_MAX_SUBEVENTS_PER_PROCEDURE         32

// Capabilities
#define CS_MAX_NUM_CONFIG_SUPPORTED            0x04
#define CS_INDEFINITE_PROCEDURES_SUPPORTED     0x00

// Default number of antennas. used only as an init value
#define CS_DEFAULT_NUM_ANTENNAS                0x01

#define CS_MAX_ANT_PATH_SUPPORTED              0x04
#define CS_MODE_3_SUPPORTED                    0x01
#define CS_NUM_CS_SYNC_EXCHANGES_SUPPORTED     0xFF // number of CS sync exchanges required to satisfy the percision requirements
#define CS_OPTIONAL_PHY_SUPPORTED              0x06 // 0: RFU, 1:2M PHY, 2: 2M BT PHY
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
// The mandatory requirements are taken by default and are not part
// of this bitwise number.
#define CS_T_IP1_IP2_CAP          0x0078 // supported: 40, 50, 60, 80us, (145us Mandatory)
#define CS_T_FCS_CAP              0x01C0 // supported: 80us, 100us, 120us, (150us Mandatory)
#define CS_T_PM_CAP               0x00   // supported: 40us Mandatory

#define CS_T_SW_CAP               0x01   // antenna switch period

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

// CS Sync antenna selection values (as in the spec)
// To be used for CS_SYNC packets by the local Controller
#define CS_SYNC_ANT_SELECT_1                  0x01U
#define CS_SYNC_ANT_SELECT_2                  0x02U
#define CS_SYNC_ANT_SELECT_3                  0x03U
#define CS_SYNC_ANT_SELECT_4                  0x04U
#define CS_SYNC_ANT_SELECT_REPETITIVE         0xFEU     // Antennas to be used, in repetitive order from 0x01 to 0x04
#define CS_SYNC_ANT_SELECT_NO_REC             0xFFU     // Host does not have a recommendation
#define CS_SYNC_ANT_LOOP_MASK                 0x80U     // CS Sync antenna selection masking for loop indication

// CS Sync antenna selection default value when no recommendation has been given by the host
#define CS_SYNC_ANT_DEFAULT_VALUE             CS_SYNC_ANT_SELECT_1

#define CS_ANT_PERMUTATION_LOOP_VALUE         0xFFU     // Antenna permutation loop value
#define CS_ANT_PERMUTATION_LOOP_MASK          0x80U     // Antenna permutation Masking for loop indication

#define CS_PREFERRED_PEER_ANT_MASK            0x0FU     // Preffered peer antenna mask (4 MSB are RFU)
#define CS_PREFERRED_PEER_ANT_DONT_CARE       0x0FU     // Used when peer doesn't care about our orderring of antennas

#define CS_ANTENNAS_GPIOS_ARRAY_SIZE          0x04U     // Antennas gpios values array size
#define CS_ANTENNAS_NUM_BITS                  0x02U     // Number of bits to represent a muxing value of a single antenna
#define CS_ANTENNAS_BITMASK                   0x03U     // Bitmask for a single antenna representation

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
#define CS_OFFSET_MIN                          1500    // this is the minimum offset value we can handle
#define CS_OFFSET_MAX                          4000000 // this is the maximum offset value defined by the spec

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
#define CS_USE_MIN_TX_POWER                    0x7E
#define CS_USE_MAX_TX_POWER                    0x7F
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
#define CS_DEFAULT_PEER_ANTENNA                1
#define CS_DEFAULT_ENABLE                      CS_DISABLE
#define CS_DEFAULT_TERMINATE_STATE             CS_TERMINATE_DISABLE
#define CS_PCT_MASK                            0xFFF
#define CS_DEFAULT_SUBEVENT_INTERVAL           0U

// CS tx_power range
#define CS_MAX_TX_POWER_VALUE                  20
#define CS_MIN_TX_POWER_VALUE                  -127

/* Number of max elements (steps) in a Tx buffer */
/* ------------------------------------------*/
#define CS_MAX_NUM_STEPS_IN_TX_BUFF            30U

/* Stable Phase Test */
#define CS_STABLE_PHASE_TONE_DURATION_US       326

/*******************************************************************************
 * ENUMS
 */

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

// CS Ctrl Procedures
typedef enum csProcedures_e
{
    CS_NO_ACTIVE_PROCEDURE             = 0x00,
    CS_SECURITY_PROCEDURE              = 0x01,
    CS_CAPABILITIES_EXCHANGE_PROCEDURE = 0x02,
    CS_CONFIG_PROCEDURE                = 0x04,
    CS_FAE_TABLE_UPDATE_PROCEDURE      = 0x08,
    CS_CHM_UPDATE_PROCEDURE            = 0x10,
    CS_IND                             = 0x20,
    CS_TERMINATE_PROCEDURE             = 0x40
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
    CS_TID_RANDOM_BACKTRACKING_RESISTANCE,
} csTransactionId_e;

typedef enum csChannelIndexArray_e
{
    CS_MODE_0_CHANNEL_INDX_ARR,
    CS_NON_MODE_0_CHANNEL_INDX_ARR,
} csChannelIndexArray_e;

typedef enum csBleRole_e
{
    CS_BLE_ROLE_NOT_SET,
    CS_BLE_ROLE_CENTRAL,
    CS_BLE_ROLE_PERIPHERAL
} csBleRole;

typedef enum csSubeventInfo_e
{
    CS_SE_INFO_NUM_STEPS, /* Total Number of steps in subevent */
    CS_SE_INFO_STEP_COUNT /* Count of steps generated for subevent */
} csSubeventInfo_e;

typedef enum csProcedureCounter_e
{
    CS_PROC_INFO_SUBEVENT_C,
    CS_PROC_INFO_EVENT_C,
    CS_PROC_C,
    CS_PROC_ALL_C
} csProcedureCounter_e;

typedef enum csNextSubevent_e
{
    CS_PREP_CURR_SUBEVENT,
    CS_PREP_NEXT_SUBEVENT,
} csNextSubevent_e;

typedef enum csSubeventType_e
{
    CS_NEW_SUBEVENT,
    CS_CONTINUE_SUBEVENT
} csSubeventType_e;

/*******************************************************************************
 * MACROS
 */

#define RESET_CS_FLAG(connId, flagId)                                          \
    (llCs[connId].completedProcedures &= flagId)
#define CS_GET_BIT(val, bitIdx) ((val >> bitIdx) & 1)
#define CS_SHIFT_RIGHT(val, n) (val >> n)
/* Translate units from 0.625ms into us Done by multiplication by (1000 * 0.625) */
#define T625MS2US(units_625us) (units_625us * 625)

/* Subevent interval is just the subevent Len and the subevent spacing in 0.625 ms */
#define CS_SUBEVENT_INTERVAL( subeventLen ) (CONVERT_1US_TO_0_625MS(subeventLen + CS_SUBEVENT_SPACE))

/* Events Per Procedure is the number of whole events that would fit into a CS procedure Len */
#define CS_EVENTS_PER_PROCEDURE(procedureLen, eventInterval, connInterval) (procedureLen / (eventInterval * connInterval))

/* The Offset_Min value shall be greater than or equal to 500 µs and less than 4 seconds.  */
#define CS_CALC_OFFSET_MIN(offsetMin) ((offsetMin < CS_OFFSET_MIN) ? CS_OFFSET_MIN : ((offsetMin > CS_OFFSET_MAX) ? CS_OFFSET_MAX : offsetMin))

/* The value shall be greater than or equal to the Offset_Min value and shall be less than the LE connection interval. */
#define CS_CALC_OFFSET_MAX(offsetMax, offsetMin, connInterval) ((offsetMax < offsetMin) ? offsetMin : ((offsetMax > (T625MS2US(connInterval) - CS_MIN_SUBEVENT_LEN)) ? (T625MS2US(connInterval) - CS_MIN_SUBEVENT_LEN) : offsetMax))

#define CS_CONNEVENT_OFFSET(taskId) ((taskId == LL_TASK_ID_CENTRAL) ? CS_CENT_CONNEVENT_OFFSET : CS_PERI_CONNEVENT_OFFSET)

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
#define CS_NUM_BUFF_STEPS(nSteps) ((nSteps > CS_MAX_NUM_STEPS_IN_TX_BUFF) ?            \
                                                CS_MAX_NUM_STEPS_IN_TX_BUFF : nSteps)

#define CS_IS_SUBEVENT_VALID(subeventLen) ( (subeventLen >= CS_MIN_SUBEVENT_LEN) && \
                                            (subeventLen <= CS_MAX_SUBEVENT_LEN) )
/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * TYPEDEFS
 */
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

/* CS Capabilities */
/* This struct is based on the PDU CS_CAPABILTITIES_REQ/RSP in the SPEC Core_v6.0 */
/* Therefore, shall not be changed unless the there is an update to the PDU */
struct llCsCapab_t
{
    uint8_t  optionalModes;      //!< indicates which of the optional CS modes are supported
    uint8_t  rttCap;             //!< indicate which of the time-of-flight accuracy requirements are met
    uint8_t  rttAAOnlyN;         //!< Number of CS steps of single packet exchanges needed
    uint8_t  rttSoundingN;       //!< Number of CS steps of single packet exchanges needed
    uint8_t  rttRandomPayloadN;  //!< Num of CS steps of single packet exchange needed
    uint16_t nadmSounding;       //!< NADM Sounding Capability
    uint16_t nadmRandomSeq;      //!< NADM Random Sequence Capability
    uint8_t  optionalCsSyncPhy;  //!< supported CS sync PHYs, bit mapped field
    uint8_t  numAntennas:4;      //!< the number of antenna elements that are available for CS tone exchanges
    uint8_t  maxAntPath:4;       //!< max number of antenna paths that are supported
    uint8_t  role:2;             //!< initiator or reflector or both
    uint8_t  rfu0:1;             //!< reserved for future use
    uint8_t  noFAE:1;            //!< No FAE
    uint8_t  chSel3c:1;          //!< channel selection 3c support
    uint8_t  csBasedRanging:1;   //!< CS based ranging
    uint8_t  rfu1:2;             //!< reserved for future use
    uint8_t  numConfig;          //!< Number of CS configurations supported per conn
    uint16_t maxProcedures;      //!< Max num of CS procedures supported
    uint8_t  tSwCap;             //!< Antenna switch time capability
    uint16_t tIp1Cap;            //!< tIP1 Capability
    uint16_t tIp2Cap;            //!< tTP2 Capability
    uint16_t tFcsCap;            //!< tFCS Capability
    uint16_t tPmCsap;            //!< tPM Capability
    uint8_t  snrTxCap;           //!< Spec defines an additional byte for RFU
} __attribute__((packed));
// Typedef separated from packed struct due to MisraC
typedef struct llCsCapab_t llCsCapabilities_t;

/* CS Configuration */
/* This struct is based on the PDU CS_CONFIG_REQ/RSP in the SPEC Core_v6.0 */
/* Therefore, shall not be changed unless the there is an update to the PDU */
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

/* CS Procedure Enable */
/* This struct is based on the PDU CS_REQ/RSP/IND in the SPEC Core_v6.0 */
/* Therefore, shall not be changed unless there is an update to the PDU */
struct csProcedureEnable_t
{
    uint8   configId:6;              /* REQ | RSP | IND */
    uint8   rfu:2;                   /* REQ | RSP | IND */
    uint16  connEventCount;          /* REQ | RSP | IND */
    uint32  offset;                  /*  X  |  X  | IND */ /* microseconds */
    uint32  offsetMin;               /* REQ | RSP |  X  */ /* microseconds */
    uint32  offsetMax;               /* REQ | RSP |  X  */ /* microseconds */
    uint16  maxProcedureDur;         /* REQ |  X  |  X  */ /* 625 microseconds */
    uint16  eventInterval;           /* REQ | RSP | IND */ /* units of connInt */
    uint8   subEventsPerEvent;       /* REQ | RSP | IND */ /* num of CS SubEvents in a CS Event */
    uint16  subEventInterval;        /* REQ | RSP | IND */ /* units 625 us*/
    uint32  subEventLen;             /* REQ | RSP | IND */ /* units microseconds, range 1250us to 4s */
    uint16  procedureInterval;       /* REQ |  X  |  X  */ /* units of connInt */
    uint16  procedureCount;          /* REQ |  X  |  X  */
    csACI_e ACI;                     /* REQ | RSP | IND */
    uint8   preferredPeerAntenna;    /* REQ |  X  |  X  */
    uint8   phy;                     /* REQ | RSP | IND */
    uint8   pwrDelta;                /* REQ | RSP | IND */
    uint8   txSnrI:4;                /* REQ |  X  |  X  */
    uint8   txSnrR:4;                /* REQ |  X  |  X  */
};
// Typedef separated from packed struct due to MisraC
typedef struct csProcedureEnable_t csProcedureEnable_t;

// Channel Map
typedef struct
{
    uint8* shuffledChanIdxArray; /* Channel Index Array */
    uint8 numChanUsed; /* Channel used as we iterate over the channels  */
    uint8 numRepetitions; /* number of times the channel array was repeated */
    uint8 selectionAlgo;
} modeSpecificChanInfo_t;

typedef struct
{
    uint8 filteredChanArr[CS_FILTERED_CHAN_MAX_SIZE];
    modeSpecificChanInfo_t mode0;
    modeSpecificChanInfo_t nonMode0;
    uint8 numChans;
} csChanInfo_t;

typedef struct
{
    uint16 numSteps;
    uint16 stepCount;
} csSubeventInfo_t;

typedef struct
{
    uint16 subeventCounter;  /* subevent counter */
    uint16 eventCounter;     /* event counter */
    uint16 procedureCounter; /* procedure counter */
} csProcCnt_t;

typedef struct
{
    csProcCnt_t counters;
    uint16_t    mMStepsRemain;      /* Number of main mode steps remain to be done. When this reaches 0, the procedure ends. */
    uint16_t    eventsPerProcedure; /* MAX_PROC_LEN / (EVENT_INTERVAL) * connEvent */
    uint32_t    eventAnchorPoint;   /* The time from which consecutive subevents are anchored. */
    uint16_t    tSw;                /* Antenna switching time as determined by both devices capabilities */

    /* Antennas indices mapping. Each 2 bits represents the final index of a specific antenna,
     * as it appears in the RCL Command.
     * Default value: MSB: [3, 2, 1, 0] :LSB
     * Means that 1st antenna index is 0
     *            2nd antenna index is 1
     *            3rd antenna index is 2
     *            4th antenna index is 3
     * This value is modified depending on preferredPeerAntenna parameter.
     */
    uint8_t     antIndicesRCLMapping;

    uint8_t     nextProcedure;      /* Marks if a procedure is done and we need to prepare another */
    csNextSubevent_e nextSubevent;  /* Marks if a subevent is submitted and we need to prepare another. */
    uint8_t procedureDoneStatus;    /* Marks whether a procedure is done for reporting */
} csProcedureInfo_t;

typedef struct csTerminateInfo
{
    csTerminateState_e terminateState; /* Flag indicating whether to terminate the upcoming CS Procedure */
    uint8 errorCode;       /* The reason the CS procedure will be terminated */
} csTerminateInfo_t;

typedef struct
{
    uint8 completedProcedures;                                      /* Bitmap of completed procedures*/
    uint8 activeCsCtrlProcedure;                                    /* Active CS Ctrl Procedure */
    uint8 currentConfigId;                                          /* Current Config ID */
    csFaeTbl_t* peerFaeTbl;                                         /* Peer FAE table */
    csSecVectors_t securityVectors;                                 /* Security vectors */
    llCsCapabilities_t peerCapabilities;                            /* Peer capabilities */
    csDefaultSettings_t defaultSettings;                            /* CS Default Settings */
    csConfigurationSet_t configSet[CS_MAX_NUM_CONFIG_IDS];          /* CS config */
    csProcedureParams_t procedureParams[CS_MAX_NUM_CONFIG_IDS];     /* Procedure Params */
    csProcedureEnable_t procedureEnableData[CS_MAX_NUM_CONFIG_IDS]; /* Procedure Enable */
    csChanInfo_t filteredChanIdx[CS_MAX_NUM_CONFIG_IDS];            /* Channel Index Array */
    csSubeventInfo_t subEventInfo;                                  /* Subevent Info */
    csProcedureInfo_t procedureInfo;                                /* Procedure Info */
    csTerminateInfo_t terminateInfo;                                /* Procedure Termination Info */
} llCs_t;

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
csStatus_e llCsConfigurationCheck(uint16 connId, const csConfigurationSet_t *pConfig);

/*******************************************************************************
 * @fn          llCsCheckSyncAntennaSelection
 *
 * @brief       Check if the value of CS Sync antenna selection is valid.
 *              Takes into account the number of antennas supportred by the device.
 *
 * input parameters
 *
 * @param       csSyncAntennaSelection - CS Sync antenna selection value
 *
 * output parameters
 *
 * @param       None.
 *
 *
 * @return      CS_STATUS_UNEXPECTED_PARAMETER - If the given value is not valid.
 *                                               If the given antenna ordering valueis
 *                                               greater-than supported number of
 *                                               antennas.
 *
 *              CS_STATUS_SUCCESS - otherwise
 */
csStatus_e llCsCheckSyncAntennaSelection(uint8 csSyncAntennaSelection);

/*******************************************************************************
 * @fn          llCsCheckACI
 *
 * @brief       Checks that an ACI value complies with capabilities.
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
 * @return      CS_STATUS_UNEXPECTED_PARAMETER - If ACI not in range of 0 to 7.
 *                                               If own\peer number of antennas is
 *                                               less-than number of antennas derived
 *                                               by ACI value.
 *                                               If own\peer N_AP derived by ACI value is
 *                                               greater-than corresponds max antenna paths.
 *
 *              CS_STATUS_SUCCESS - otherwise
 */
csStatus_e llCsCheckACI(csACI_e ACI, uint8 configId, uint16 connId);

/*******************************************************************************
 * @fn          llCsCheckPreferredAntenna
 *
 * @brief       Checks that a preferred ordered antenna elements to be used
 *              by a device complies with a given ACI and a given number of antennas.
 *
 * input parameters
 *
 * @param       ACI - Anetnna Config Index. assumed valid.
 * @param       ownerRole - Role of the owner of the antennas. assumed valid.
 * @param       numAnt - Number of the antennas the owner has.
 * @param       prefAnt - Preferred ordered antenna elements.
 *                        This value is represented by 4 bits bitmap, while
 *                        the 4 MSB are ignored.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      CS_STATUS_UNEXPECTED_PARAMETER -
 *                          If the number of bits set in prefAnt param is less than the
 *                          number of antenna elements denoated by the ACI param.
 *                          If the number of bits set in prefAnt param exceeds numAnt
 *
 *              CS_STATUS_SUCCESS - otherwise
 */
csStatus_e llCsCheckPreferredAntenna(csACI_e ACI, uint8_t ownerRole, uint8 numAnt,
                                     uint8 prefAnt);

/*******************************************************************************
 * @fn          llCsGetNumPaths
 *
 * @brief       Calculates number of paths according to a given ACI value
 *
 * input parameters
 *
 * @param       ACI - Anetnna Config Index. assumed valid.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      number of paths according to the ACI value
 */
uint8 llCsGetNumPaths(csACI_e ACI);

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
 * @fn          llCsMapAntennaMuxIndex
 *
 * @brief       Maps index to another according to a given indices mapping bitmap.
 *              indices are represented by 2 bits.
 *              indices are between 0 to 3.
 *
 * input parameters
 *
 * @param       indicesMapping  - Bitmap of indices mapping valus, 2 bits for each index.
 *                                first index (0) represented by the 2 LSB.
 *                                fourth index (3) represented by the 2 MSB.
 * @param       indexToMap      - Index to map.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      If the given index is between 0 to 3 - The mapped index
 *              else - the given index
 */
uint8_t llCsMapAntennaMuxIndex(uint8_t indicesMapping, uint8_t indexToMap);

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
 * @fn          llCsGetRemainingStepsInCurrSubEvent
 *
 * @brief       llCsGetRemainingStepsInCurrSubEvent
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
uint16 llCsGetRemainingStepsInCurrSubEvent(uint16 connId);

/*******************************************************************************
 * @fn          llCsGetNextLoopValue
 *
 * @brief       This function gets a value and returnes the next value
 *              when loop is configured.
 *              If loop isn't configured, the given value will be returned as is.
 *              Loop is determined based on the MSB of the given value
 *              (1 - loop, 0 - no loop).
 *              This function takes into account minimum and maximum values.
 *
 * input parameters
 *
 * @param       currValue - Current value. Assumed in range of [minValue, maxValue].
 * @param       minValue  - Minimum value for looping wraparound
 * @param       maxValue  - Maximum value for looping wraparound.
 *                          assumed equal or greater than minValue.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      The next value based on loop configuration.
 */
uint8 llCsGetNextLoopValue(uint8 currValue, uint8 minValue, uint8 maxValue);


/*******************************************************************************
 * @fn          llCs_getNextConnEvent
 *
 * @brief       Get next connection event
 * This function calculates the actual next connection event based on current
 * time, connection interval and last start time.
 *
 * input parameters
 *
 * @param       connPtr - Connection Pointer
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Up-to-date Next connection event
 * */
uint16_t llCs_getNextConnEvent(const llConnState_t *connPtr);

/*******************************************************************************
 * @fn          llCs_checkProcedureParams
 *
 * @brief       Check if the procedure parameters are valid
 *
 * input parameters
 *
 * @param       csReq - Pointer to LL_CS_REQ
 * @param       connPtr - Connection Pointer
 *
 * output parameters
 *
 * @param       None
 *
 * @return      STATUS - Success if the procedure parameters are valid, Fail otherwise
 * */
csStatus_e llCs_checkProcedureParams(const csProcedureEnable_t* csReq, const llConnState_t* connPtr);

/*******************************************************************************
 * @fn          llCsSubeventsPerEvent
 *
 * @brief       Calculate How many subevent will fit into 1 event
 *
 * input parameters
 *
 * @param       connInterval - connection interval
 * @param       subeventInterval - CS subevent Interval
 * @param       eventOffset - CS event offset
 *
 * output parameters
 *
 * @param       None
 *
 * @return      num subevents per event
 */
uint8 llCsSubeventsPerEvent(uint16 connInterval, uint16 subEventInterval, uint32 eventOffset);

#endif // LL_CS_COMMON_H
