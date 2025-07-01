/*****************************************************************************

 @file  ranging.h

 @brief This file contains the ranging service definitions and prototypes.

 Group: WCS, BTS
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
#ifndef RANGING_H
#define RANGING_H
#ifdef __cplusplus

extern "C"
{
#endif
/*********************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/comdef.h"

/*********************************************************************
 * DEFINES
 */

// Service UUID
#define RANGING_SERVICE_UUID        0x185B

/************************************
 ***** Characteristic defines *******
 ************************************/

// RAS Feature
#define RAS_FEATURE_UUID            0x2C14

// RAS Ranging Data real time
#define RAS_REAL_TIME_UUID          0x2C15

// RAS Ranging Data on demand
#define RAS_ON_DEMAND_UUID          0x2C16

// RAS control point
#define RAS_CONTROL_POINT_UUID      0x2C17

// RAS Ranging Data is ready
#define RAS_DATA_READY_UUID         0x2C18

// RAS Ranging Data is overwritten
#define RAS_DATA_OVERWRITTEN_UUID   0x2C19

/************************************
 ************ RAS lengths ***********
 ************************************/

#define RAS_FEAT_LEN               4
#define RAS_READY_LEN              2
#define RAS_OVERWRITTEN_LEN        2
#define RAS_CP_RSP_MAX_LEN         5
#define RAS_CP_COMMANDS_MAX_LEN    5
#define RAS_CP_COMMANDS_MIN_LEN    1
#define RAS_CP_GET_DATA_CMD_LEN    3
#define RAS_CP_ACK_DATA_CMD_LEN    3
#define RAS_CP_RETRIEVE_CMD_LEN    5
#define RAS_CP_ABORT_CMD_LEN       1

#define RAS_CP_RSP_COMPLETE_DATA_RSP_LEN           2 // Length of the Complete Data Response
#define RAS_CP_RSP_COMPLETE_LOST_DATA_SEG_RSP_LEN  3 // Length of the Complete Lost Data Segment Response
#define RAS_CP_RSP_CODE_LEN                        2 // Length of the Response Code

#define RAS_STEP_HDR_LEN    1U // @ref Ranging_subEventResultsStep_t header length

// RAS Features for the RAS Features Characteristic
// The RAS Features Characteristic is a 32-bit bitmask that indicates the features
// supported by the RAS server. The bitmask is defined as follows:
// Bit 0: Real Time Ranging Data
// Bit 1: Retrieve Lost Segments
// Bit 2: Abort Operation
// Bit 3: Filter Data
// Bit 4-31: RFU
typedef enum
{
  RAS_FEATURES_REAL_TIME          = (uint32_t) BV(0),
  RAS_FEATURES_RETREIVE_LOST_SEG  = (uint32_t) BV(1),
  RAS_FEATURES_ABORT_OPERATION    = (uint32_t) BV(2),
  RAS_FEATURES_FILTER_DATA        = (uint32_t) BV(3),
} RAS_features_e;

/************************************
 ************ Control Point **********
 ************************************/

// RAS Control Point Command
typedef enum
{
  RAS_CP_OPCODE_GET_RANGING_DATA     = 0x00,  // Get Ranging Data command opcode
  RAS_CP_OPCODE_ACK_RANGING_DATA     = 0x01,  // Acknowledge Ranging Data command opcode
  RAS_CP_OPCODE_RETREIVE_LOST_SEG    = 0x02,  // Retrieve Lost Segments command opcode
  RAS_CP_OPCODE_ABORT_OPERATION      = 0x03,  // Abort Operation command opcode
  RAS_CP_OPCODE_SET_FILTER           = 0x04,  // Set Filter command opcode
} RAS_CP_cmd_opCode_e;

// RAS Control Point Opcode Response
typedef enum
{
  RAS_CP_OPCODE_COMPLETE_DATA_RSP              = 0x00,  // Get Ranging Data command opcode
  RAS_CP_OPCODE_COMPLETE_LOST_DATA_SEG_RSP     = 0x01,  // Acknowledge Ranging Data command opcode
  RAS_CP_OPCODE_RSP_CODE                       = 0x02,  // Retrieve Lost Segments command opcode
} RAS_CP_rsp_opCode_e;

// RAS Control Point Response Code
typedef enum
{
  RAS_CP_RSP_CODE_VAL_SUCCESS = 0x01,          // Success response code
  RAS_CP_RSP_CODE_VAL_OPCODE_NOT_SUPPORTED,    // Opcode not supported response code
  RAS_CP_RSP_CODE_VAL_INVALID_PARAM,           // Invalid parameter response code
  RAS_CP_RSP_CODE_VAL_SUCCESS_PERSISTED,       // Success persisted response code
  RAS_CP_RSP_CODE_ABORT_UNSUCCESSFUL,          // Abort unsuccessful response code
  RAS_CP_RSP_CODE_PROCEDURE_NOT_COMP,          // Procedure not completed response code
  RAS_CP_RSP_CODE_SERVER_BUSY,                 // Server busy response code
  RAS_CP_RSP_CODE_NO_RECORDS_FOUND,            // No records found response code
} RASRspCodeVal_e;

/*******************************************************************************
 * TYPEDEFS
 */

// Control point message structure
PACKED_ALIGNED_TYPEDEF_STRUCT
{
  uint8_t  opCode;
  uint16_t param1;
  uint8_t  param2;
  uint8_t  param3;
} RAS_cpMsg_t;

// Subevent results step data structure
typedef struct
{
    uint8_t stepMode; // Step mode
    uint8_t stepData; // Step data
} Ranging_subEventResultsStep_t;

// Subevent header structure
typedef struct
{
  uint16_t startAclConnEvt;         // Start of ACL connection event
  uint16_t freqCompenstation;       // Frequency compensation value
  uint8_t  rangingDoneStatus : 4;   // Ranging done status (0-15)
  uint8_t  subeventDoneStatus : 4;  // Subevent done status (0-15)
  uint8_t  rangingAbortReason : 4;  // ranging abort reason (0-15)
  uint8_t  subeventAbortReason : 4; // subevent abort reason (0-15)
  int8_t   referencePowerLvl;       // Reference power level (-127 to 20 in dBm)
  uint8_t  numStepsReported;        // Number of steps reported in the ranging procedure (0-255)
} Ranging_subEventHeader_t;

// Subevent data structure
typedef struct
{
  Ranging_subEventHeader_t subEventHeader; // Header of the subevent data
  uint8_t data[];                          // Buffer to hold all setps of the subevent.
} Ranging_subEvent_t;

// Ranging data header structure
typedef struct
{
  uint16_t rangingCounter : 12;  // Lower 12 bits of CS Procedure_Counter
  uint16_t configID : 4;         // CS configuration identifier (0-3)
  int8_t   selectedTxPower;      // Transmit power level (-127 to 20 dBm)
  uint8_t  antennaPathsMask;     // Antenna paths mask (Bit0-Bit3: paths, Bits4-7: RFU)
} Ranging_RangingHeader_t;

// Ranging data structure
typedef struct
{
    Ranging_RangingHeader_t rangingHeader;   // Header of the ranging data
    uint8_t                 subEventsBuff[]; // Buffer to hold all sub events data array of Ranging_subEvent_t.
} Ranging_RangingData_t;

#ifdef __cplusplus
}
#endif

#endif /* RANGING_H */
