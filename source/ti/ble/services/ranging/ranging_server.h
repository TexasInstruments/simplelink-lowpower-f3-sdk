/*****************************************************************************

 @file  ranging_server.h

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
 #ifndef RANGINGSERVER_H
 #define RANGINGSERVER_H

 #ifdef __cplusplus
 extern "C"
 {
 #endif
 /*********************************************************************
 * INCLUDES
 */

#include "ti/ble/services/ranging/ranging_types.h"

/*********************************************************************
 * DEFINES
 */

/************************************
 ***** Characteristic defines *******
 ************************************/

// RAS Feature
#define RAS_FEAT_ID            0

// RAS Ranging Data real time
#define RAS_REAL_TIME_ID       1

// RAS Ranging Data on demand
#define RAS_ON_DEMAND_ID       2

// RAS control point
#define RAS_CONTROL_POINT_ID   3

// RAS Ranging Data is ready
#define RAS_READY_ID           4

// RAS Ranging Data is overwritten
#define RAS_OVERWRITTEN_ID     5

/************************************
 ************ Control Point ***********
 ************************************/

// Control Point Message Struct
typedef struct
{
  uint16_t    connHandle;
  uint8_t     msgLength;
  RAS_cpMsg_t CPMsg;
} RAS_CPCB_t;

/************************************
 *********** General ****************
 ************************************/

// Data structure used to store CCC update
typedef struct
{
  uint16_t connHandle;
  uint16_t uuid;
  uint8_t  value;
} RAS_cccUpdate_t;

/*********************************************************************
 * Profile Callbacks
 */
// Callback to indicate client characteristic configuration has been update
typedef void ( *RAS_CccUpdate_t )( char *cccUpdate );

// Callback when control point request is received
typedef void ( *RAS_CPReqCB_t )( char *CPReq );

typedef void ( *RAS_segmentSentCompleteCB_t  )( char *connHandle, uint8_t status);

// Server CallBack struct
typedef struct
{
  RAS_CccUpdate_t             pfnCccUpdateCB;           // Called when CCC was update for one of the characteristics
  RAS_CPReqCB_t               pfnCPReqCB;               // Called when CP request is received
  RAS_segmentSentCompleteCB_t pfnSegmentSendCompleteCB; // Called when a segment has been sent successfully
} RAS_cb_t;

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      RAS_addService
 *
 * @brief   Initialize the Ranging Server service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  SUCCESS or bleMemAllocError
 */
uint8_t RAS_addService(void);

/*********************************************************************
 * @fn      RAS_registerProfileCBs
 *
 * @brief   Registers the profile callback function. Only call
 *          this function once.
 *
 * @param   profileCallback - pointer to profile callback.
 *
 * @return  SUCCESS or INVALIDPARAMETER
 */
uint8_t RAS_registerProfileCBs(RAS_cb_t *profileCallback);

/*********************************************************************
 * @fn      RAS_setParameter
 *
 * @brief   Set a Ranging Service parameter.
 *
 * @param   param -  Characteristic UUID
 * @param   pValue - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 * @param   len - length of data to write
 *
 * @return  SUCCESS or error status
 */
uint8_t RAS_setParameter(uint16_t connHandle, uint8_t param, void *pValue, uint16 len);

/*********************************************************************
 * @fn      RAS_getParameter
 *
 * @brief   Get a RAS parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  SUCCESS or error status
 */
uint8_t RAS_getParameter(uint8_t param, void *pValue);
#ifdef __cplusplus
}
#endif

#endif /* RANGINGSERVER_H */
