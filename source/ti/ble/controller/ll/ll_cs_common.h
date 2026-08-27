/******************************************************************************

 @file  ll_cs_common.h

 @brief Channel Sounding common header

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2026, Texas Instruments Incorporated

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
#include "ti/ble/controller/ll/ll_csdrbg.h"

#include <ti/drivers/rcl/commands/ble_cs.h>
#include <ti/drivers/rcl/handlers/ble_cs.h>
#include "ti/ble/controller/ll/ll_soc.h"
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
#define CS_MIN_SUBEVENT_LEN                    1250U    // us
#define CS_MAX_SUBEVENT_LEN                    4000000U // us
#define CS_RESULT_EVENT_HEADER_LEN             16      // The fields of the subevent results event
#define CS_CONTINUE_RESULT_EVENT_HEADER_LEN    9
#define CS_MIN_STEPS_PER_SUBEVENT              2   // SPEC definition
#define CS_MAX_STEPS_PER_SUBEVENT              160 // SPEC definition
#define CS_MAX_STEPS_PER_PROCEDURE             256U // SPEC definition
#define CS_MIN_MODE_0_STEPS                    1 // SPEC definition
#define CS_MAX_MODE_0_STEPS                    3 // SPEC definition
#define CS_MIN_MAIN_MODE_REPETITION_STEPS      0 // SPEC definition
#define CS_MAX_MAIN_MODE_REPETITIONS_STEPS     3 // SPEC definition
#define CS_MIN_SUB_MODE_MAIN_MODE_STEPS        1 // SPEC definition
#define CS_SUB_MODE_INSERTION_STEPS_NUM        1 // SPEC definition
#define CS_MIN_EVENT_INTERVAL                  1
#define CS_MIN_PROCEDURE_INTERVAL              3
#define CS_MAX_PROCEDURE_INTERVAL              0xFFFF
#define CS_INVALID_PROCEDURE_INTERVAL          0U
#define CS_MIN_SUBEVENTS_PER_EVENT             1U
#define CS_MAX_SUBEVENTS_PER_PROCEDURE         32
#define CS_INFINITE_PROCEDURE_REPETITIONS      0U
#define CS_SINGLE_PROCEDURE                    1U
#define CS_TX_PWR_DELTA_NO_RECOMMENDATION      ((int8_t)-128)

// Capabilities
#define CS_MAX_NUM_CONFIG_SUPPORTED            0x04
#define CS_INDEFINITE_PROCEDURES_SUPPORTED     0x00

// CS Role Mask
#define CS_NO_ROLE_MASK                        0x00U
#define CS_INITIATOR_MASK                      0x01U
#define CS_REFLECTOR_MASK                      0x02U

#define CS_MAX_ANT_PATH_SUPPORTED              0x04
#define CS_MODE_3_SUPPORTED                    0x01
#define CS_NUM_CS_SYNC_EXCHANGES_SUPPORTED     0xFF // number of CS sync exchanges required to satisfy the percision requirements
#define CS_OPTIONAL_PHY_SUPPORTED              0x06 // 0: RFU, 1:2M PHY, 2: 2M BT PHY
#define CS_RTT_CAPABILITY_NOT_SUPPORTED        0x00
#define CS_CAPABILITY_NOT_SUPPORTED            0
#define CS_MIN_CHANNEL_REPEAT                  1

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
#define CS_T_PM_CAP               0x02   // supported: 20us, (40us Mandatory)

#define CS_T_SW_CAP               0x0A   // antenna switch period
#define CS_T_SW_DEFAULT           0U     // Default Value (when default num antennas (1) is used)

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

// Default number of antennas. used only as an init value
#define CS_DEFAULT_NUM_ANTENNAS                0x01

// General min and max number of antennas which can be used for CS
#define CS_MIN_NUM_ANTENNAS                    0x01U
#define CS_MAX_NUM_ANTENNAS                    0x04U

// CS Sync antenna selection values (as in the spec)
// To be used for CS_SYNC packets by the local Controller
#define CS_SYNC_ANT_SELECT_1                    0x01U
#define CS_SYNC_ANT_SELECT_2                    0x02U
#define CS_SYNC_ANT_SELECT_3                    0x03U
#define CS_SYNC_ANT_SELECT_4                    0x04U
#define CS_SYNC_ANT_SELECT_REPETITIVE_DOUBLE    0xFDU     // Antennas to be used, in repetitive order {1, 1, 2, 2, ..., numAnt, numAnt}
#define CS_SYNC_ANT_SELECT_REPETITIVE_SINGLE    0xFEU     // Antennas to be used, in repetitive order {1, 2, ..., numAnt}
#define CS_SYNC_ANT_SELECT_NO_REC               0xFFU     // Host does not have a recommendation
#define CS_SYNC_ANT_FIRST_OCCURRENCE            0         // Indicates first occurrence of an antenna in repetitive modes
#define CS_SYNC_ANT_SECOND_OCCURRENCE           1         // Indicates second occurrence of an antenna in repetitive modes

// CS Sync antenna selection default value when no recommendation has been given by the host
#define CS_SYNC_ANT_DEFAULT_VALUE             CS_SYNC_ANT_SELECT_1

#define CS_ANT_PERMUTATION_LOOP_VALUE         0xFFU     // Antenna permutation loop value
#define CS_ANT_PERMUTATION_LOOP_MASK          0x80U     // Antenna permutation Masking for loop indication

#define CS_PREFERRED_PEER_ANT_MASK            0x0FU     // Preffered peer antenna mask (4 MSB are RFU)
#define CS_PREFERRED_PEER_ANT_DONT_CARE       0x0FU     // Used when peer doesn't care about our orderring of antennas
#define CS_MIN_PREFERRED_PEER_ANT             0x01U     // Min value for preferred peer antenna
#define CS_MAX_PREFERRED_PEER_ANT             0x0FU     // Max value for preferred peer antenna

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
#define CS_OWN_OFFSET_MIN                      SOC_CS_OWN_OFFSET_MIN  // SoC-dependent, see ll_soc.h
#define CS_OWN_CODED_OFFSET_MIN                2500    /* NOTE: The coded PHY offset used here is hardcoded and doesn't account for
                                                        * the specific S2/S8 sub-coding scheme. While this value
                                                        * works for all cases, S2 coding could potentially use a smaller offset
                                                        * for improved efficiency in future implementations.
                                                        */

#define CS_OFFSET_MIN                          500     // this is the minimum offset value defined by the spec
#define CS_OFFSET_MAX                          4000000 // this is the maximum offset value defined by the spec

// Other Definitions
#define CS_SEC_NO_OFFSET                       0
#define CS_SEC_USE_OFFSET                      1
#define CS_CONFIG_EXISTS                       0
#define CS_CONFIG_REMOVED                      1
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
#define CS_DEFAULT_MAX_PROC_COUNT              0xFFFF
#define CS_DEFAULT_MIN_SUBEVENT_LEN            0x4E2U    // 1250 us
#define CS_DEFAULT_MAX_SUBEVENT_LEN            0x3D0900U //us = 4s
#define CS_DEFAULT_SUBEVENT_INTERVAL           0U

// CS tx_power range
#define CS_DEFAULT_TX_POWER                    0

/* Number of max elements (steps) in a Tx buffer */
/* ------------------------------------------*/
#define CS_MAX_NUM_STEPS_IN_TX_BUFF            10U

/* Stable Phase Test */
#define CS_STABLE_PHASE_TONE_DURATION_US       326

/* CS Channel Map Update type */
#define CS_CHM_UPDATE_FORCE_IMMEDIATE          1U
#define CS_CHM_UPDATE_WAIT_FOR_INSTANT         0U

#define CS_EVENT_STATUS_NONE                   0
#define CS_EVENT_STATUS_STARTED                1


#define CS_PROCEDURE_COUNTER_NONE              0
#define CS_PROCEDURE_COUNTER_INCREMENTED       1

/*******************************************************************************
 * ENUMS
 */

// CS Ctrl Procedures
typedef enum csProcedures_e
{
    CS_NO_ACTIVE_PROCEDURE             = 0x00,
    CS_SECURITY_PROCEDURE              = 0x01,
    CS_CAPABILITIES_EXCHANGE_PROCEDURE = 0x02,
    CS_CACHED_REMOTE_CAPS_PROCEDURE    = 0x04,
    CS_CONFIG_PROCEDURE                = 0x08,
    CS_FAE_TABLE_UPDATE_PROCEDURE      = 0x10,
    CS_CHM_UPDATE_PROCEDURE            = 0x20,
    CS_START_PROCEDURE                 = 0x40,
    CS_TERMINATE_PROCEDURE             = 0x80
} csProcedures_e;

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

typedef enum csSubeventInfo_e
{
    CS_SE_INFO_STEPS_NUM, /* Total Number of steps in subevent */
    CS_SE_INFO_STEPS_C, /* Count of steps generated for subevent */
    CS_SE_INFO_ALL_C
} csSubeventInfo_e;

typedef enum csProcedureCounter_e
{
    CS_PROC_INFO_SUBEVENT_C,
    CS_PROC_INFO_SUBEVENT_PER_PROC_C,
    CS_PROC_INFO_EVENT_C
} csProcedureCounter_e;

typedef enum csSubeventType
{
    CS_NEW_SUBEVENT,
    CS_CONTINUE_SUBEVENT
} csSubeventType_e;


typedef enum csStepType
{
    CS_STEP_TYPE_NONE,
    CS_STEP_TYPE_MODE_0,
    CS_STEP_TYPE_MAIN_MODE_REPETITIONS,
    CS_STEP_TYPE_SUB_MODE_INSERTION
} csStepType_e;

/*******************************************************************************
 * MACROS
 */

#define CS_GET_BIT(val, bitIdx) ((val >> bitIdx) & 1)
#define CS_SHIFT_RIGHT(val, n) (val >> n)
/* Translate units from 0.625ms into us Done by multiplication by (1000 * 0.625) */
#define T625MS2US(units_625us) (units_625us * 625)

/* Subevent interval is just the subevent Len and the subevent spacing in 0.625 ms */
#define CS_SUBEVENT_INTERVAL( subeventLen ) (CONVERT_1US_TO_0_625MS(subeventLen + CS_SUBEVENT_SPACE))

/* Events Per Procedure is the number of whole events that would fit into a CS procedure Len */
#define CS_EVENTS_PER_PROCEDURE(procedureLen, eventInterval, connInterval) (procedureLen / (eventInterval * connInterval))

/* The value shall be greater than or equal to the Offset_Min value and shall be less than the LE connection interval. */
#define CS_CALC_OFFSET_MAX(offsetMax, offsetMin, connInterval) ((offsetMax < offsetMin) ? offsetMin : ((offsetMax > (T625MS2US(connInterval) - 1)) ? (T625MS2US(connInterval) - 1) : offsetMax))
#define CS_CALC_OFFSET_MIN(phy) ((phy == LL_PHY_CODED) ? CS_OWN_CODED_OFFSET_MIN : CS_OWN_OFFSET_MIN)

#define CS_CONNEVENT_OFFSET(taskId) ((taskId == LL_TASK_ID_CENTRAL) ? CS_CENT_CONNEVENT_OFFSET : CS_PERI_CONNEVENT_OFFSET)

// This Macro will check if any of channels 0-1, 23-25, 77-78 are used (they
// shouldn't be)
#define CS_TEST_CHANNEL_MAP_RESTRICTED(channelMap)                             \
    (channelMap[0] & 0x03 || channelMap[2] & 0x80 || channelMap[3] & 0x03 ||   \
     channelMap[9] & 0xE0)

/* Tone Extension Bits are used in their reversed order */
#define CS_REVERSE_TONE_EXTENSION_BITS(bits)                                   \
    ((((bits & 0x01U) << 1U) | ((bits & 0x02U) >> 1U)) & 0x3U)

/* Determine Number of steps in buffer */
#define CS_NUM_BUFF_STEPS(nSteps) ((nSteps > CS_MAX_NUM_STEPS_IN_TX_BUFF) ?            \
                                                CS_MAX_NUM_STEPS_IN_TX_BUFF : nSteps)

#define CS_IS_SUBEVENT_VALID(subeventLen) ( (subeventLen >= CS_MIN_SUBEVENT_LEN) && \
                                            (subeventLen < CS_MAX_SUBEVENT_LEN) )

#define CS_IS_CONFIG_ID_VALID(configId)   (configId < CS_MAX_NUM_CONFIG_IDS)

/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * TYPEDEFS
 */
typedef struct
{
    csChm_t  *pChanMap;                  // Pointer to the channel map
    uint32_t lastUpdateRequestTime;      // Last time the ChM update API has called @ref LL_CS_SetChannelClassification
    bool     isAllowedToUpdate;          // Is it allowed to update the ChM, the update should be done only once per 1 sec
} csLocalChm_t;

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

/*
 * CS Capabilities — internal (DB/API) representation.
 *
 * Unpacked struct with natural alignment; no wire-format constraints.
 * Bitfields from the CS_CAPABILITIES_REQ/RSP PDU (wire bytes 10 and 11) are
 * expanded into individual uint8_t fields so that field access carries no
 * packed-struct overhead.
 * This is the type used everywhere except at PDU parse/serialize boundaries.
 *
 * Wire-format encoding/decoding is handled exclusively by:
 *   llCsCapab_ParsePDU()      — PDU bytes -> llCsCapabilities_t
 *   llCsCapab_SerializePDU()  — llCsCapabilities_t -> PDU bytes
 */
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
    uint8_t  numAntennas;        //!< number of antenna elements (wire byte 10 bits [3:0])
    uint8_t  maxAntPath;         //!< max antenna paths supported (wire byte 10 bits [7:4])
    uint8_t  role;               //!< initiator or reflector or both (wire byte 11 bits [1:0])
    /* rfu0 (wire byte 11 bit [2]) not stored; written as 0 on serialize */
    uint8_t  noFAE;              //!< No FAE (wire byte 11 bit [3])
    uint8_t  chSel3c;            //!< channel selection 3c support (wire byte 11 bit [4])
    uint8_t  csBasedRanging;     //!< CS based ranging (wire byte 11 bit [5])
    /* rfu1 (wire byte 11 bits [7:6]) not stored; written as 0 on serialize */
    uint8_t  numConfig;          //!< Number of CS configurations supported per conn
    uint16_t maxProcedures;      //!< Max num of CS procedures supported
    uint8_t  tSwCap;             //!< Antenna switch time capability
    uint16_t tIp1Cap;            //!< tIP1 Capability
    uint16_t tIp2Cap;            //!< tTP2 Capability
    uint16_t tFcsCap;            //!< tFCS Capability
    uint16_t tPmCsap;            //!< tPM Capability
    uint8_t  snrTxCap;           //!< Spec defines an additional byte for RFU
};
// Typedef separated from struct due to MisraC
typedef struct llCsCapab_t llCsCapabilities_t;

/*
 * CS Capabilities PDU wire layout — packed, matches CS_CAPABILITIES_REQ/RSP PDU payload.
 * (Core Spec v6.0, Vol 6, Part B)
 *
 * Used ONLY inside llCsCapab_ParsePDU() and llCsCapab_SerializePDU().
 * Never stored in the DB or passed through the API.
 *
 *   [0]     optionalModes
 *   [1]     rttCap
 *   [2]     rttAAOnlyN
 *   [3]     rttSoundingN
 *   [4]     rttRandomPayloadN
 *   [5-6]   nadmSounding (little-endian)
 *   [7-8]   nadmRandomSeq (little-endian)
 *   [9]     optionalCsSyncPhy
 *   [10]    numAntennas[3:0] | maxAntPath[7:4]
 *   [11]    role[1:0] | rfu0[2] | noFAE[3] | chSel3c[4] | csBasedRanging[5] | rfu1[7:6]
 *   [12]    numConfig
 *   [13-14] maxProcedures (little-endian)
 *   [15]    tSwCap
 *   [16-17] tIp1Cap (little-endian)
 *   [18-19] tIp2Cap (little-endian)
 *   [20-21] tFcsCap (little-endian)
 *   [22-23] tPmCsap (little-endian)
 *   [24]    snrTxCap
 */
struct llCsCapabPdu_t
{
    uint8_t optionalModes;
    uint8_t rttCap;
    uint8_t rttAAOnlyN;
    uint8_t rttSoundingN;
    uint8_t rttRandomPayloadN;
    uint8_t nadmSounding[2];    /* little-endian */
    uint8_t nadmRandomSeq[2];   /* little-endian */
    uint8_t optionalCsSyncPhy;
    uint8_t byte10;             /* numAntennas[3:0] | maxAntPath[7:4]                                     */
    uint8_t byte11;             /* role[1:0] | rfu0[2] | noFAE[3] | chSel3c[4] | csBasedRanging[5] | rfu1[7:6] */
    uint8_t numConfig;
    uint8_t maxProcedures[2];   /* little-endian */
    uint8_t tSwCap;
    uint8_t tIp1Cap[2];         /* little-endian */
    uint8_t tIp2Cap[2];         /* little-endian */
    uint8_t tFcsCap[2];         /* little-endian */
    uint8_t tPmCsap[2];         /* little-endian */
    uint8_t snrTxCap;
} __attribute__((packed));
// Typedef separated from struct due to MisraC
typedef struct llCsCapabPdu_t llCsCapabPdu_t;

/* Paranoia check: wire payload must be exactly 25 bytes */
_Static_assert(sizeof(llCsCapabPdu_t) == 25, "llCsCapabPdu_t size mismatch with CS_CAPABILITIES PDU");

/* Bit-field decode helpers for llCsCapabPdu_t byte10 */
#define CS_CAPAB_PDU_GET_NUM_ANT(b)        ((uint8_t)((b) & 0x0FU))
#define CS_CAPAB_PDU_GET_MAX_ANT_PATH(b)   ((uint8_t)(((b) >> 4U) & 0x0FU))

/* Bit-field decode helpers for llCsCapabPdu_t byte11 */
#define CS_CAPAB_PDU_GET_ROLE(b)           ((uint8_t)((b) & 0x03U))
#define CS_CAPAB_PDU_GET_NO_FAE(b)         ((uint8_t)(((b) >> 3U) & 0x01U))
#define CS_CAPAB_PDU_GET_CH_SEL_3C(b)      ((uint8_t)(((b) >> 4U) & 0x01U))
#define CS_CAPAB_PDU_GET_CS_BASED_RNG(b)   ((uint8_t)(((b) >> 5U) & 0x01U))

/* Bit-field encode helpers — rfu0 and rfu1 are always written as 0 */
#define CS_CAPAB_PDU_SET_BYTE10(ant, path) \
    ((uint8_t)(((ant) & 0x0FU) | (((path) & 0x0FU) << 4U)))
#define CS_CAPAB_PDU_SET_BYTE11(role, noFAE, ch3c, csRng) \
    ((uint8_t)(((role) & 0x03U) | (((noFAE) & 0x01U) << 3U) | \
               (((ch3c) & 0x01U) << 4U) | (((csRng) & 0x01U) << 5U)))

/*
 * CS Configuration — internal (DB/API) representation.
 *
 * Unpacked struct with natural alignment; no wire-format constraints.
 * Bitfields from the CS_CONFIG_REQ PDU are expanded into individual uint8
 * fields so that field access carries no packed-struct overhead.
 * This is the type used everywhere except at PDU parse/serialize boundaries.
 *
 * Wire-format encoding/decoding is handled exclusively by:
 *   csConfig_ParsePDU()   — PDU bytes -> csConfigurationSet_t
 *   csConfig_SerializePDU() — csConfigurationSet_t -> PDU bytes
 */
struct csConfig_t
{
    uint8 configId;           /* CS configuration ID (wire byte 0 bits [5:0]) */
    uint8 action;             /* 0b00 disabled, 0b01 enabled (wire byte 0 bits [7:6]) */
    csChm_t channelMap;       /* channel map */
    uint8 chMRepetition;      /* number of times the ChM field will be cycled through */
                              /* for non-mode 0 steps within a CS procedure */
    uint8 mainMode;           /* which CS modes are to be used */
    uint8 subMode;            /* which CS modes are to be used */
    uint8 mainModeMinSteps;   /* range of Main_Mode steps to be executed before */
                              /* a Sub_Mode step is executed */
    uint8 mainModeMaxSteps;
    uint8 mainModeRepetition; /* num of main mode steps from the last CS subevent to be repeated */
    uint8 modeZeroSteps;      /* number of mode 0 steps to be included at the beginning of each CS Subevent */
    uint8 csSyncPhy;          /* transmit and receive PHY to be used */
    uint8 rttType;            /* which RTT variant is to be used (wire byte 19 bits [3:0]) */
    uint8 role;               /* CS role (wire byte 19 bits [5:4]) */
    uint8 chSel;              /* channel selection algorithm to be used (wire byte 20 bits [3:0]) */
    uint8 ch3cShape;          /* selected shape to be rendered (wire byte 20 bits [7:4]) */
    uint8 ch3CJump;           /* one of the valid CSChannelJump values */
    uint8 tIP1;               /* Index of the period used between RTT packets */
    uint8 tIP2;               /* Index of the interlude period used between CS tones */
    uint8 tFCs;               /* Index used for frequency changes */
    uint8 tPM;                /* Index for the measurement period of CS tones */
    /* Note: rfu0 (wire byte 19 bits [7:6]) and rfu1 (wire byte 26) are not stored;
     * both are written as 0 on serialize */
};
// Typedef separated from struct due to MisraC
typedef struct csConfig_t csConfigurationSet_t;

/*
 * CS Configuration PDU wire layout — packed, matches CS_CONFIG_REQ PDU payload.
 * (Core Spec v6.0, Vol 6, Part B, CS_CONFIG_REQ)
 *
 * Used ONLY inside csConfig_ParsePDU() and csConfig_SerializePDU().
 * Never stored in the DB or passed through the API.
 *
 *   [0]     configId[5:0] | action[7:6]
 *   [1-10]  channelMap[10]
 *   [11]    chMRepetition
 *   [12]    mainMode
 *   [13]    subMode
 *   [14]    mainModeMinSteps
 *   [15]    mainModeMaxSteps
 *   [16]    mainModeRepetition
 *   [17]    modeZeroSteps
 *   [18]    csSyncPhy
 *   [19]    rttType[3:0] | role[5:4] | rfu0[7:6]
 *   [20]    chSel[3:0] | ch3cShape[7:4]
 *   [21]    ch3CJump
 *   [22]    tIP1
 *   [23]    tIP2
 *   [24]    tFCs
 *   [25]    tPM
 *   [26]    rfu1
 */
struct csConfigPdu_t
{
    uint8 byte0;                      /* configId[5:0] | action[7:6]         */
    uint8 channelMap[CS_CHM_SIZE];    /* 10-byte channel map                 */
    uint8 chMRepetition;
    uint8 mainMode;
    uint8 subMode;
    uint8 mainModeMinSteps;
    uint8 mainModeMaxSteps;
    uint8 mainModeRepetition;
    uint8 modeZeroSteps;
    uint8 csSyncPhy;
    uint8 byte19;                     /* rttType[3:0] | role[5:4] | rfu0[7:6] */
    uint8 byte20;                     /* chSel[3:0]   | ch3cShape[7:4]        */
    uint8 ch3CJump;
    uint8 tIP1;
    uint8 tIP2;
    uint8 tFCs;
    uint8 tPM;
    uint8 rfu1;
} __attribute__((packed));
// Typedef separated from struct due to MisraC
typedef struct csConfigPdu_t csConfigPdu_t;

/* Paranoia check: wire payload must be exactly 27 bytes (LL_CS_CONFIG_REQ_PL_LEN - 1) */
_Static_assert(sizeof(csConfigPdu_t) == 27, "csConfigPdu_t size mismatch with CS_CONFIG_REQ PDU");

/* Bit-field decode helpers for csConfigPdu_t packed bytes */
#define CS_CFG_PDU_GET_CONFIG_ID(b)   ((uint8)((b) & 0x3FU))
#define CS_CFG_PDU_GET_ACTION(b)      ((uint8)(((b) >> 6U) & 0x03U))
#define CS_CFG_PDU_GET_RTT_TYPE(b)    ((uint8)((b) & 0x0FU))
#define CS_CFG_PDU_GET_ROLE(b)        ((uint8)(((b) >> 4U) & 0x03U))
#define CS_CFG_PDU_GET_CH_SEL(b)      ((uint8)((b) & 0x0FU))
#define CS_CFG_PDU_GET_CH3C_SHAPE(b)  ((uint8)(((b) >> 4U) & 0x0FU))

/* Bit-field encode helpers — rfu0 is always written as 0 */
#define CS_CFG_PDU_SET_BYTE0(id, act)      ((uint8)(((id) & 0x3FU) | (((act) & 0x03U) << 6U)))
#define CS_CFG_PDU_SET_BYTE19(rtt, role)   ((uint8)(((rtt) & 0x0FU) | (((role) & 0x03U) << 4U)))
#define CS_CFG_PDU_SET_BYTE20(sel, shape)  ((uint8)(((sel) & 0x0FU) | (((shape) & 0x0FU) << 4U)))

typedef struct
{
    uint8_t roleEn;
    uint8_t csSyncAntennaSelection;
    int8_t  maxTxPower;
} csDefaultSettings_t;

typedef struct
{
    uint16 nTotal;    /* Planned/configured ceiling (e.g., steps per subevent, procedure instances) */
    uint16 nCounter;  /* Running count toward the ceiling */
} csCounter_t;

/* CS Procedure Enable Data — negotiated parameters stored in the DB and used during CS execution.
 * Populated from CS_REQ/RSP/IND PDUs; fields not needed at runtime (configId, txSnrI/R) are
 * kept in csProcedureParams_t instead. */
typedef struct
{
    uint32_t offset;               /*  X  |  X  | IND */ /* Selected event offset (μs) */
    uint32_t offsetMin;            /* REQ | RSP |  X  */ /* Min acceptable offset (μs) — negotiation */
    uint32_t offsetMax;            /* REQ | RSP |  X  */ /* Max acceptable offset (μs) — negotiation */
    uint16_t eventAnchorConnEvent; /* REQ | RSP | IND */
                                   /* Live scheduling tracker: the ACL connection-event counter value
                                    * on which the *next* CS event (not procedure) is expected to fire.
                                    *
                                    * Initialised from the connEventCount field of the CS_REQ / CS_RSP /
                                    * CS_IND PDU that establishes the first CS event.  Thereafter it is
                                    * advanced by eventInterval (conn events) each time the RCL reports
                                    * a CS event done, so it always reflects the next scheduled CS event.
                                    *
                                    * NOTE: this field diverges from csProcRepetitions_t.procedureAnchorConnEvent
                                    * during a multi-event procedure.  procedureAnchorConnEvent stays fixed at
                                    * the start of the current repetition period; eventAnchorConnEvent moves
                                    * forward with every CS event within that period.  They are re-synchronised
                                    * (set to the same value) only at procedure boundaries. */
    uint16_t eventInterval;        /* REQ | RSP | IND */ /* ACL conn events between CS event anchors */
    uint16_t subEventInterval;     /* REQ | RSP | IND */ /* Time between subevents (625 μs units) */
    uint16_t maxProcedureDur;      /* REQ |  X  |  X  */ /* Max procedure duration (625 μs units) */
    uint16_t procedureInterval;    /* REQ |  X  |  X  */ /* Interval between procedures (conn intervals) */
    uint16_t procedureCount;       /* REQ |  X  |  X  */ /* Number of procedures to run (OTA nTotal) */
    uint32_t subEventLen;          /* REQ | RSP | IND */ /* Subevent duration (μs), 1250–4 000 000 */
    uint8_t  subEventsPerEvent;    /* REQ | RSP | IND */ /* Subevents anchored off the same ACL event */
    csACI_e  ACI;                  /* REQ | RSP | IND */ /* Antenna Configuration Index */
    uint8_t  preferredPeerAntenna; /* REQ |  X  |  X  */ /* Preferred peer antenna bitmap */
    uint8_t  phy;                  /* REQ | RSP | IND */ /* PHY used for CS */
    int8_t   pwrDelta;             /* REQ | RSP | IND */ /* TX power delta */
} csEnableProcedureCtrlData_t;

// Channel Map
typedef struct
{
    uint8 shuffledChanIdxArray[CS_FILTERED_CHAN_MAX_SIZE];  /* Channel Index Array */
    uint8 numChanUsed;                                      /* Channel used as we iterate over the channels  */
    uint8 numRepetitions;                                   /* number of times the channel array was repeated */
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
    uint8_t stepCount;          /* Number of main mode channels currently stored for repetition in the next subevent */
    uint8_t channelArrayIndex;  /* Current index in the circular buffer for storing/retrieving main mode channels */
    uint8_t channelArray[CS_MAX_MAIN_MODE_REPETITIONS_STEPS];  /* Circular buffer storing channel indices for repetition */
    bool isFull;                /* Flag indicating whether the circular buffer contains the maximum allowed number of channels */
} csMainModeRepetitionsInfo_t;

typedef struct
{
    uint8_t channelIdx;         /* Channel index to be used for subMode Mode1 insertions (reuses channel from previous mainMode step) */
    uint8_t stepCount;          /* Total number of subMode insertions scheduled for current subevent */
    uint8_t nextIndex;          /* Number of mainMode steps to execute before the next subMode insertion */
    bool isCompleted;           /* Indicates whether the last generated subMode insertion index for this subevent have been scheduled */
} csSubModeInsertionInfo_t;

typedef struct
{
    uint16 subeventCounter;                 /* subevent per event counter */
    uint16 subeventPerProcedureCounter;     /* subevent per procedure counter */
    uint16 eventCounter;                    /* event counter */
} csProcCnt_t;

typedef struct csDoneInfo
{
    uint8 status;                           /* The status of CS procedure termination */
    uint8 reason;                           /* The reason of CS procedure termination */
} csDoneInfo_t;

typedef struct
{
    uint8_t previousProcedureStatus:1;      /* true if a previous procedure was invalid - all subevents in previous procedure were unsynced */
    uint8_t csReqIntiatedByPeer:1;          /* Marks if the peer initiated CS a request */
    uint8_t reserved:6;                     /* Reserved for future use */
} csRepetitionsFlags_t;

typedef struct
{
    csCounter_t             repetitions;          /* nTotal = procedures to run, nCounter = completed */
    uint16_t                procedureAnchorConnEvent;
                                                  /* Repetition-period anchor: the ACL connection-event counter value
                                                   * at which the *current* CS repetition period began (i.e. the start
                                                   * of the last scheduled CS procedure, not the individual CS events
                                                   * within it).
                                                   *
                                                   * Set once at the start of each procedure (from the connEventCount
                                                   * negotiated in the CS_REQ / CS_IND PDU) and advanced by
                                                   * procedureInterval at every procedure boundary.  It is never
                                                   * touched by the RCL mid-procedure.
                                                   *
                                                   * Used by llCsRealignProceduresRepetition to detect missed
                                                   * procedures (MAP_llEventCmp(currentEvent, procedureAnchorConnEvent))
                                                   * and by llCsPrepareNextProcedure as the base from which the next
                                                   * repetition start is computed:
                                                   *   nextConnEvent = procedureAnchorConnEvent + procedureInterval
                                                   *
                                                   * NOTE: contrast with csEnableProcedureCtrlData_t.eventAnchorConnEvent, which
                                                   * is the per-event scheduler trigger and moves by eventInterval
                                                   * inside a procedure. */
    uint16_t                peerTermProcCount;    /* The peer procedure counter received in the terminate indication */
    csDoneInfo_t            repetitionsDone;      /* Marks the DoneInfo of the procedure repetitions */
    csRepetitionsFlags_t    flags;
} csProcRepetitions_t;

typedef struct
{
    uint8_t procedureCounterIncremented:1;        /* Marks if the Procedure Counbter was incremented for this procedure */
    uint8_t validProcedure:1;                     /* Marks if a procedure was valid - at least one subevent was good */
    uint8_t eventStarted:1;                       /* Marks if the Event has started properly */
    uint8_t reserved:5;                           /* Reserved for future use */
} csFlags_t;

typedef struct
{
    uint8_t antennaIndex:3;  /* Antenna selection, 1 .. (number of antennas) */

    /* Indicates whether to repeat an antennas selection.
     * 0 = first occurrence
     * 1 = second occurrence
     * used in mode @ref CS_SYNC_ANT_SELECT_REPETITIVE_DOUBLE only */
    uint8_t repeat:1;
    uint8_t reserved:4;
} csSyncAntennaSelection_t;

typedef struct
{
    uint16_t                tSw;                  /* Antenna switching time as determined by both devices capabilities */

    /* Antennas indices mapping from physical antenna index to its index as it appears in the RCL Command.
     * Each 2 bits represents the final index of a specific antenna.
     * Example: MSB: [0, 2, 3, 1] :LSB
     * Means that 1st antenna maps to index 1 in the RCL command
     *            2nd antenna maps to index 3 in the RCL command
     *            3rd antenna maps to index 2 in the RCL command
     *            4th antenna maps to index 0 in the RCL command
     * This value is modified depending on preferredPeerAntenna parameter.
     */
    uint8_t     mapPhysicalAntToRCL;
    uint8_t     mapRCLAntToPhysical;                /* inversion mapping of @ref mapPhysicalAntToRCL (RCL ant to physical) */
    csSyncAntennaSelection_t     csSyncAntenna;     /* The CS Sync antenna to be used in the CS SYNC packets */
    uint8_t gpioVals[CS_ANTENNAS_GPIOS_ARRAY_SIZE]; /* The GPIO values to be used for each antenna */
} csProcedureAntennasInfo_t;

typedef struct
{
    csProcCnt_t                 counters;             /* Counters used for the procedure */
    csCounter_t                 subEventInfo;         /* Subevent step counters: nTotal = planned, nCounter = generated */
    csMainModeRepetitionsInfo_t mModeRepetitions;     /* Main Mode Repetitions Info */
    csSubModeInsertionInfo_t    subModeInsertion;     /* Submode insertions info */
    uint16_t                    mMStepsRemain;        /* Number of main mode steps remain to be done. When this reaches 0, the procedure ends. */
    uint32_t                    eventAnchorPoint;     /* The time from which consecutive subevents are anchored. */
    csFlags_t                   csFlags;              /* CS Flags Per Connection */
    csDoneInfo_t                procedureDone;        /* Marks the DoneInfo of the procedure */
    csDoneInfo_t                subEventDone;         /* Marks the DoneInfo of the subEvent */
} csProcedureInfo_t;

typedef struct
{
    csConfigurationSet_t configSet;       /* CS config */
    csProcedureParams_t  procedureParams; /* Host procedure params — persistent across procedures */
} llCsConfig_t;

typedef struct
{
    csChm_t  currClassifiedPeerChanMap;     // Classified peer's channel map
    csChm_t  classifiedPeerChanMapReq;      // Classified peer's channel map that still not reached instant.
    uint16_t chanMapUpdateEvent;            // Event counter when the channel map update should take place
    uint8_t  pendingChanMapUpdate:1;          // 1 if need to send channel map indication to peer, 0 otherwise
    uint8_t  useClassifiedPeerChanMap:1;      // 1 if need to use the peer's classified channel map, 0 otherwise
    uint8_t  reserved:6;
} llCsChannelMapClassification_t;

typedef struct
{
    drbgParams_t csDrbgParams;                                      /* CS DRBG Parameters */
    uint8_t completedProcedures;                                    /* Bitmap of completed procedures @ref csProcedures_e */
    uint8_t activeCsCtrlProcedure;                                  /* Active CS Ctrl Procedure  @ref csProcedures_e */
    llCsCapabilities_t peerCapabilities;                            /* Peer capabilities */
    csDefaultSettings_t defaultSettings;                            /* CS Default Settings */
    llCsConfig_t hostConfig[CS_MAX_NUM_CONFIG_IDS];                 /* CS host-configured params (config set + procedure params) — read-only during procedures */
    csConfigurationSet_t* llCsPendingConfigReqCtrlSet;              /* CS Config Request Control Packet structure per connection */
    llCsChannelMapClassification_t peerChannelMapClassification;    /* Channel Map Classification */
    csFaeTbl_t* peerFaeTbl;                                         /* Peer FAE table */
} llCs_t;

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          llCsCheckConnection
 *
 * @brief       Does basic validation for parameters that are checked
 * repetetively in multiple APIs.
 * This checks if the connection Handle is valid and active.
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
 * @return      CS_STATUS_UNEXPECTED_PARAMETER - numAnt is not in range of @ref CS_MIN_NUM_ANTENNAS
 *                                               to @ref CS_MAX_NUM_ANTENNAS
 *              CS_STATUS_LIMITED_RESOURCES - If the number of bits set in prefAnt param is less
 *                                      than the number of antenna elements denoated by the
 *                                      ACI param.
 *                                      If the number of bits set in prefAnt param exceeds numAnt
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
 * @fn          llCsMapAntennaIndex
 *
 * @brief       Maps an antenna index to another according to a given mapping.
 *
 * input parameters
 *
 * @param       indexToMap - Index to map (1-4).
 * @param       indicesMapping - Mapping to apply.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      If the given index is between 1 to 4 - The mapped index
 *              else - the given index
 */
uint8_t llCsMapAntennaIndex(uint8_t indexToMap, uint8_t indicesMapping);

/*******************************************************************************
 * @fn          llCsMapPhysicalAntennaToRclIndex
 *
 * @brief       Maps a physical antenna index to RCL antenna index according to
 *              the mapping saved in the given connection.
 *
 * input parameters
 *
 * @param       connId - Connection ID to grab the mapping from
 * @param       indexToMap - Index to map (1-4).
 *
 * output parameters
 *
 * @param       None
 *
 * @return      If the given index is between 1 to 4 - The mapped index
 *              else - the given index
 */
uint8_t llCsMapPhysicalAntennaToRclIndex(uint16_t connId, uint8_t indexToMap);

/*******************************************************************************
 * @fn          llCsMapRclAntennaToPhysicalIndex
 *
 * @brief       Maps an RCL antenna index to physical antenna index according to
 *              the mapping saved in the given connection.
 *
 * input parameters
 *
 * @param       connId - Connection ID to grab the mapping from
 * @param       indexToMap - Index to map (1-4).
 *
 * output parameters
 *
 * @param       None
 *
 * @return      If the given index is between 1 to 4 - The mapped index
 *              else - the given index
 */
uint8_t llCsMapRclAntennaToPhysicalIndex(uint16_t connId, uint8_t indexToMap);

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
 * @return      Remaining number of Steps in current SubEvent
 */
uint8_t llCsGetRemainingStepsInCurrSubEvent(uint16 connId);

/*******************************************************************************
 * @fn          llCsGetRemainingStepsInCurrStepBuffer
 *
 * @brief       llCsGetRemainingStepsInCurrStepBuffer
 *
 * input parameters
 *
 * @param       connId - Connection Identifier
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Remaining number of Steps in current Step Buffer
 */
uint8_t llCsGetRemainingStepsInCurrStepBuffer(uint16 connId);

/*******************************************************************************
 * @fn          llCsGetNextLoopValue
 *
 * @brief       This function gets a value and returns the next value
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
uint8_t llCsGetNextLoopValue(uint8_t currValue, uint8_t minValue, uint8_t maxValue);

/*******************************************************************************
 * @fn          llCsSyncAntGetNextLoopValue
 *
 * @brief       This function gets a CS Sync antenna value and returns the next value
 *              based on the given mode.
 *
 * input parameters
 *
 * @param       current - Current CS Sync antenna selection value.
 * @param       mode    - One of the relevant modes:
 *                        @ref CS_SYNC_ANT_SELECT_REPETITIVE_DOUBLE
 *                        @ref CS_SYNC_ANT_SELECT_REPETITIVE_SINGLE
 *
 * @returns    The next CS Sync antenna selection value based on the given mode.
 * */
csSyncAntennaSelection_t llCsSyncAntGetNextLoopValue(csSyncAntennaSelection_t current, uint8_t mode);

/*******************************************************************************
 * @fn          llCsGetNextConnEvent
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
uint16_t llCsGetCurrentConnEvent(const llConnState_t *connPtr);

/*******************************************************************************
 * @fn          llCsGetSelectedTxPowerRaw
 *
 * @brief       Get selected TX Power to be used by the local device for CS procedures.
 *              The returned value is the raw value of the TX Power, before any adjustments
 *              based on the local device power table.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Raw value of Selected TX Power
 * */
int8_t llCsGetSelectedTxPowerRaw(uint16_t connHandle);

/*******************************************************************************
 * @fn          llCsGetSelectedTxPower
 *
 * @brief       Get selected TX Power to be used by the local device for CS procedures.
 *              The returned value is the actual TX Power value that will be used by the radio,
 *              based on the power table.
 *
 * input parameters
 *
 * @param       connHandle - Connection handle
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Selected TX Power
 * */
int8_t llCsGetSelectedTxPower(uint16_t connHandle);

/*******************************************************************************
 * @fn          llCsCheckChannelMap
 *
 * @brief       Check if the combined channel map of two channel maps is valid
 *              according to the BLE specification.
 *
 * input parameters
 *
 * @param       chanMap1 - Pointer to channel map 1
 * @param       chanMap2 - Pointer to channel map 2
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS_STATUS_SUCCESS - If the combined channel map is valid
 *              CS_STATUS_INVALID_CHM - If the combined channel map is invalid
 * */
csStatus_e llCsCheckChannelMap( csChm_t chanMap1, csChm_t chanMap2 );

/*******************************************************************************
 * @fn          llCsCheckProcedureEnableParams
 *
 * @brief       Check if the procedure parameters are valid
 *
 * input parameters
 *
 * @param       connPtr  - Connection Pointer
 * @param       configId - CS configuration ID
 * @param       pEnable  - Pointer to procedure data to validate
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS_STATUS_SUCCESS if the procedure parameters are valid, error otherwise
 * */
csStatus_e llCsCheckProcedureEnableParams(const llConnState_t *connPtr, uint8_t configId, const csEnableProcedureCtrlData_t *pEnable);

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

/*******************************************************************************
 * @fn          llCsProcessChanMapUpdate
 *
 * @brief       Process Channel Map Update
 *              This function checks if a channel map update is pending and if the instant
 *              has passed. If so, it applies the new channel map.
 *
 * input parameters
 *
 * @param       connId - Connection Identifier
 * @param       forceUpdate - If true, the channel map update will be processed
 *                            regardless of the instant.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 * */
void llCsProcessChanMapUpdate( uint8_t connId, bool forceUpdate );

/*******************************************************************************
 * @fn          llCsGetChannelMapForProcedure
 *
 * @brief       Get the channel map to be used for the ongoing procedure.
 *
 *              This function returns the channel map to be used for the ongoing
 *              procedure. If config is not valid, it returns NULL.
 *
 * input parameters
 *
 * @param       connId - Connection Identifier
 * @param       configId - CS configuration ID
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Pointer to channel map to be used for the ongoing procedure,
 *              or NULL if no procedure is ongoing.
 * */
const csChm_t *llCsGetChannelMapForProcedure(uint16_t connId, uint8_t configId);

/*******************************************************************************
 * @fn          llCsCheckConfigValidChannelMap
 *
 * @brief       Check if a channel map is valid
 *              This function checks if a channel map is valid according to the
 *              BLE specification.
 *
 * input parameters
 *
 * @param       pChM - Pointer to channel map
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS_STATUS_SUCCESS - If the channel map is valid
 *              CS_STATUS_INVALID_CHM - If the channel map is invalid
 * */
csStatus_e llCsCheckConfigValidChannelMap( const csChm_t *pChM );

/*******************************************************************************
 * @fn          llCsSendChanMapIndToActiveConns
 *
 * @brief       Send LL_CTRL_CS_CHANNEL_MAP_IND to all active connections.
 *
 * input parameters
 *
 * @param       none
 *
 * output parameters
 *
 * @return      none
 */
void llCsSendChanMapIndToActiveConns( void );

/*******************************************************************************
 * @fn          llCsIsChannelClassificationAllowed
 *
 * @brief       Check if channel classification is allowed.
 *              Channel classification is allowed only if at least 1 second
 *              has passed since the last update.
 *
 * input parameters
 *
 * @param       currentTime - Current time in milliseconds
 *
 * output parameters
 *
 * @param       None
 *
 * @return      true if channel classification is allowed, false otherwise
 */
bool llCsIsChannelClassificationAllowed( uint32_t currentTime );

/*******************************************************************************
 * @fn          llCsApplyChanMapUpdate
 *
 * @brief       Applies the Channel Map update received from peer device
 *              in the CS DB.
 *
 * input parameters
 *
 * @param       connId - connection Id
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsApplyChanMapUpdate(uint8_t connId);

/*******************************************************************************
 * @fn          llCsCheckCapabilitiesParams
 *
 * @brief       Check if capabilities parameters are valid
 *
 * input parameters
 *
 * @param       pCaps - Pointer to capabilities structure
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS_STATUS_UNEXPECTED_PARAMETER if one of the parameters is invalid
 *              CS_STATUS_SUCCESS otherwise
 * */
csStatus_e llCsCheckCapabilitiesParams(const llCsCapabilities_t* pCaps);


/*******************************************************************************
 * @fn          llCsSetConfigReqFilterChannelMap
 *
 * @brief       Update the Channel Map classification in the given config set
 *
 * input parameters
 *
 * @param pConfigSet - pointer to configuration set
 * @param       pChm - pointer to input channel map
 *
 * output parameters
 *
 * @return      None
 */
void llCsSetConfigReqFilterChannelMap(csConfigurationSet_t* pConfigSet, const csChm_t* pFilteredChM);

/*******************************************************************************
 * @fn          llCsSetConfigReqtTiming
 *
 * @brief Set the timing parameters for a specific connection and configuration.
 *
 * This function sets the timing parameters for a given Update the Channel Map
 * classification in the given config set
 *
 * input parameters
 *
 * @param pConfigSet pointer to configuration set
 * @param tIP1 Timing parameter IP1.
 * @param tIP2 Timing parameter IP2.
 * @param tFCs Timing parameter FCs.
 * @param tPM Timing parameter PM.
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsSetConfigReqtTiming(csConfigurationSet_t* pConfigSet,
                                     uint8 tIP1,
                                     uint8 tIP2,
                                     uint8 tFCs,
                                     uint8 tPM);


/*******************************************************************************
 * @fn          llCsSetConfigReqtSubModeInsertionRange
 * @brief       Set the 'main mode number of steps' minimum and maximum values
 *
 * input parameters
 *
 * @param       pConfigSet   - pointer to configuration set
 * @param       numStepsMin  - minimum number of steps
 * @param       numStepsMax  - maximum number of steps
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsSetConfigReqtSubModeInsertionRange(csConfigurationSet_t* pConfigSet, uint8 numStepsMin, uint8 numStepsMax);

/*******************************************************************************
 * @fn          csConfig_ParsePDU
 *
 * @brief       Parse a CS_CONFIG_REQ PDU payload into an unpacked
 *              csConfigurationSet_t.  This is the sole entry point for
 *              air-received configuration data; it replaces the previous
 *              direct cast of the PDU buffer to csConfigurationSet_t*.
 *
 * input parameters
 *
 * @param pDst  - Output: pointer to csConfigurationSet_t to populate.
 * @param pBuf  - Input:  27-byte PDU payload (wire layout = csConfigPdu_t).
 *
 * output parameters
 *
 * @return      None
 */
void csConfig_ParsePDU(csConfigurationSet_t *pDst, const uint8 *pBuf);

/*******************************************************************************
 * @fn          csConfig_SerializePDU
 *
 * @brief       Serialize a csConfigurationSet_t into a CS_CONFIG_REQ PDU
 *              payload.  This is the sole exit point for configuration data
 *              going over the air; it replaces the previous direct memcpy of
 *              csConfigurationSet_t to the PDU buffer.
 *
 * input parameters
 *
 * @param pBuf  - Output: 27-byte PDU payload buffer to fill.
 * @param pSrc  - Input:  pointer to csConfigurationSet_t from DB.
 *
 * output parameters
 *
 * @return      None
 */
void csConfig_SerializePDU(uint8 *pBuf, const csConfigurationSet_t *pSrc);

/*******************************************************************************
 * @fn          llCsCapab_ParsePDU
 *
 * @brief       Parse a CS_CAPABILITIES_REQ/RSP PDU payload into an unpacked
 *              llCsCapabilities_t.  This is the sole entry point for
 *              air-received capabilities data; it replaces the previous
 *              direct cast of the PDU buffer to llCsCapabilities_t*.
 *
 * input parameters
 *
 * @param pDst  - Output: pointer to llCsCapabilities_t to populate.
 * @param pBuf  - Input:  25-byte PDU payload (wire layout = llCsCapabPdu_t).
 *
 * output parameters
 *
 * @return      None
 */
void llCsCapab_ParsePDU(llCsCapabilities_t *pDst, const uint8 *pBuf);

/*******************************************************************************
 * @fn          llCsCapab_SerializePDU
 *
 * @brief       Serialize a llCsCapabilities_t into a CS_CAPABILITIES_REQ/RSP
 *              PDU payload.  This is the sole exit point for capabilities data
 *              going over the air; it replaces the previous direct memcpy of
 *              llCsCapabilities_t to the PDU buffer.
 *
 * input parameters
 *
 * @param pBuf  - Output: 25-byte PDU payload buffer to fill.
 * @param pSrc  - Input:  pointer to llCsCapabilities_t from DB.
 *
 * output parameters
 *
 * @return      None
 */
void llCsCapab_SerializePDU(uint8 *pBuf, const llCsCapabilities_t *pSrc);

/*******************************************************************************
 * @fn          llCSHasNoFunctionPreventingPending
 *
 * @brief       This function check if this connection has an instant in progress
 *              (this excludes Channel Sounding)
 * input parameters
 *
 * @param       connPtr - Connection pointer
 *
 * output parameters
 *
 * @return FALSE if there isn't a control procedure active else TRUE.
 */uint8 llCSHasNoFunctionPreventingPending(llConnState_t *connPtr);

#endif // LL_CS_COMMON_H
