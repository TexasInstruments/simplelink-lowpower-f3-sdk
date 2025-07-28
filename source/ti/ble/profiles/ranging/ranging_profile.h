/******************************************************************************

 @file  ranging_profile.h

 @brief This file contains the ranging profile (RRSP) definitions and prototypes.

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
#ifndef RANGING_PROFILE_H
#define RANGING_PROFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "ti/ble/app_util/framework/bleapputil_api.h"
#include "ti/ble/services/ranging/ranging_server.h"

/*********************************************************************
 * DEFINES
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * Profile Callback
 */

//Callback to indicate client characteristic configuration has been updated for ranging characteristics
typedef void ( *RRSP_CccUpdate_t )( uint16_t connHandle, uint16_t pValue );

//Callback to indicate status updated
typedef void ( *RRSP_StatusUpdate_t )( uint8_t status , uint16_t connHandle, uint16_t rangingCounter );

// Struct to hold the callback functions
typedef struct
{
  RRSP_CccUpdate_t      pfnOnCccUpdateCB;
  RRSP_StatusUpdate_t   pfnStatusUpdateCB;
} RRSP_cb_t;

// Struct to hold the segmentation header information
typedef struct
{
  uint8_t firstSegment : 1;  // Bit 0: Indicates if this is the first segment (0 = False, 1 = True)
  uint8_t lastSegment  : 1;  // Bit 1: Indicates if this is the last segment (0 = False, 1 = True)
  uint8_t segmentIndex : 6;  // Bits 2-7: Segment Index (0 to 63)
} RRSP_SegmentationHeader_t;

// Struct to hold the one segment of the data
typedef struct
{
    RRSP_SegmentationHeader_t segmentationHeader;              // Header for segmentation information
    uint8_t                   segmentData[ATT_MAX_MTU_SIZE];   // Data for the segment, size is limited to ATT_MAX_MTU_SIZE
} RRSP_Segment_t;

// this struct responsible of managing the segmentation sending process
typedef struct
{
    uint16_t currentConnHandle; // Current connection handle for the ongoing segmentation/send process
    uint8_t  segmentCounter;    // The next Seg to send
    uint8_t  lastSegment;       // Last segment flag (0 = False, 1 = True)
    uint8_t  rangingCounter;    // Ranging counter for the current procedure
    uint8_t  busy;              // Flag to indicate if the segmentation process is busy (0 = False, 1 = True)
    uint8_t  waitForBusy;       // Flag to indicate if the server should send to peer device that it is busy (0 = False, 1 = True)
    uint8_t  waitForConf;       // Flag to indicate if the process is waiting for confirmation (0 = False, 1 = True)
    uint8_t  waitForNoti;       // Flag to indicate if the process is waiting for notification data sent (0 = False, 1 = True)
} RRSP_Segmentation_Process_t;

// Struct to hold the subevent process information
typedef struct
{
    uint8_t subeventNumOfSteps;      // Number of steps in a subevent.
    uint16_t startOfSubeventoffset;  // Start offset of the subevent in the received data
    uint16_t rangingCounter;         // Ranging counter for the current procedure
} RRSP_receive_subevent_t;

// Struct to hold the procedure status
typedef enum
{
  RRSP_SENDING_PROCEDURE_STARTED           = 0x00,  // Get Ranging Data command opcode
  RRSP_SENDING_PROCEDURE_ENDED             = 0x01,  // Acknowledge Ranging Data command opcode
  RRSP_STATUS_SENDING_PROCEDURE_ABORTED    = 0x02,  // Retrieve Lost Segments command opcode
} RRSP_procedureStatus_e;

// Struct to hold the CS subevent results
typedef struct
{
  uint8_t  csEvtOpcode;             //!< CS Event Code
  uint16_t connHandle;              //!< Connection handle
  uint8_t  configID;                //!< Configuration ID
  uint16_t startAclConnectionEvent; //!< Start ACL connection event
  uint16_t procedureCounter;        //!< Procedure counter
  int16_t  frequencyCompensation;   //!< Frequency compensation
  int8_t   referencePowerLevel;     //!< Reference power level
  uint8_t  procedureDoneStatus;     //!< Procedure done status
  uint8_t  subeventDoneStatus;      //!< Subevent done status
  uint8_t  abortReason;             //!< Abort reason
  uint8_t  numAntennaPath;          //!< Number of antenna paths
  uint8_t  numStepsReported;        //!< Number of steps reported
  uint16_t dataLen;                 //!< Data length
  uint8_t  data[];                  //!< Data
} RRSP_csSubEventResults_t;

// Struct to hold the CS subevent results continuation
typedef struct
{
  uint8_t  csEvtOpcode;          //!< CS Event Code
  uint16_t connHandle;           //!< Connection handle
  uint8_t  configID;             //!< Configuration ID
  uint8_t  procedureDoneStatus;  //!< Procedure done status
  uint8_t  subeventDoneStatus;   //!< Subevent done status
  uint8_t  abortReason;          //!< Abort reason
  uint8_t  numAntennaPath;       //!< Number of antenna paths
  uint8_t  numStepsReported;     //!< Number of steps reported
  uint16_t  dataLen;             //!< Data length
  uint8_t  data[];               //!< Data
} RRSP_csSubEventResultsContinue_t;

/*
* Enum to hold the registration status of the ranging server profile.
* Bit Mapping:
* First bit  - notification
* Second bit - indication
* Third bit  - on-demand
* Fourth bit - real-time
*/

typedef enum
{
  RRSP_UNREGISTER           = 0x00,  // Unregister from the Ranging Server service
  RRSP_ON_DEMAND_NOTI       = 0x05,  // On-Demand Ranging Data Notification (0101)
  RRSP_ON_DEMAND_IND        = 0x06,  // On-Demand Ranging Data Indication   (0110)
  RRSP_REAL_TIME_NOTI       = 0x09,  // Real-Time Ranging Data Notification (1001)
  RRSP_REAL_TIME_IND        = 0x0A,  // Real-Time Ranging Data Indication   (1010)
} RRSP_RegistrationStatus_e;

/*********************************************************************
 * API FUNCTIONS
 */
/*********************************************************************
 * @fn      RRSP_start
 *
 * @brief   This function adds the Ranging Server service,
 *          registers the service callback function and initializes
 *          the service characteristics
 *
 * @param   appCallbacks - pointer to application callback
 * @param   features - Ranging feature value

 * @return  SUCCESS or stack call status
 */
uint8_t RRSP_start(RRSP_cb_t *appCallbacks, uint32_t features);

/*********************************************************************
 * @fn      RRSP_ProcedureStarted
 *
 * @brief   This function is called to indicate that a ranging procedure has started.
 *
 * @param   connHandle - Connection handle of the device that started the procedure.
 * @param   rangingCounter - The ranging counter for the procedure.
 * @param   pRangingHeader - Pointer to the ranging header containing procedure information.
 *
 * @retun  None
 */
uint8_t RRSP_ProcedureStarted(uint16_t connHandle, uint16_t rangingCounter, uint8_t *pRangingHeader);

/*********************************************************************
 * @fn      RRSP_AddSubeventResult
 *
 * @brief   This function adds a subevent result to the ranging procedure DB.
 *
 * @param   pSubeventResult - Pointer to the subevent result data to be added.
 *
 * @return SUCCESS or stack call status
 */
uint8_t RRSP_AddSubeventResult(RRSP_csSubEventResults_t* pSubeventResult);

/*********************************************************************
 * @fn      RRSP_AddSubeventContinueResult
 *
 * @brief   This function adds a subevent result to the ranging procedure DB.
 *
 * @param   pSubeventContResult - Pointer to the continue subevent result data to be added.
 *
 * @return SUCCESS or stack call status
 */
uint8_t RRSP_AddSubeventContinueResult(RRSP_csSubEventResultsContinue_t* pSubeventContResult);

/*********************************************************************
 * @fn      RRSP_ProcedureDone
 *
 * @brief   This function is called to indicate that a ranging procedure has stopped
 *          and now the profile aknowledge that he is ready to transfer data.
 *
 * @param   connHandle - Connection handle of the device that stopped the procedure.
 * @param   rangingCounter - The ranging counter for the procedure.
 *
 * @retun  SUCCESS or stack call status
 */
uint8_t RRSP_ProcedureDone(uint16_t connHandle, uint16_t rangingCounter);

/*********************************************************************
 * @fn      RRSP_RegistrationStatus
 *
 * @brief   This function is called to check the registration status
 *          of a connection handle
 *
 * @param   connHandle - Connection handle of the device to check registration status.
 *
 * @retun   registration status of the connection handle (@ref RRSP_RegistrationStatus_e)
 */
RRSP_RegistrationStatus_e RRSP_RegistrationStatus(uint16_t connHandle);

#endif
