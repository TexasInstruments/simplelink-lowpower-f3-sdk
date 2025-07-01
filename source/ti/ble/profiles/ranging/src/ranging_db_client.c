/******************************************************************************

 @file  ranging_db_client.c

 @brief This file contains the implementation of the ranging database APIs and
        functionality.

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

/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include "ti/ble/stack_util/icall/app/icall.h"
/* This Header file contains all BLE API and icall structure definition */
#include "ti/ble/stack_util/icall/app/icall_ble_api.h"
#include "ti/ble/profiles/ranging/ranging_db_client.h"

/*********************************************************************
 * MACROS
 */
// Invalid index
#define INVALID_INDEX 0xFF

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
// Ranging Profile Data base, each entry represent different connection handle
static RangingDB_procedureData_t gRangingProcedureDB[MAX_NUM_PROCEDURES];

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t rangingDBClient_GetIndex( uint16_t connHandle );

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      RangingDBClient_initDB
 *
 * @brief   This function initializes the ranging procedure data base
 *          variables and array.
 *
 * input parameters
 *
 * @param   None
 *
 * output parameters
 *
 * @param   None
 *
 * @return  SUCCESS or stack call status
 */
uint8_t RangingDBClient_initDB(void)
{
  // Initialize the ranging procedure database
  for (uint8_t i = 0; i < MAX_NUM_PROCEDURES; i++)
  {
    gRangingProcedureDB[i].connHandle = LINKDB_CONNHANDLE_INVALID;
  }
  return SUCCESS;
}

/*********************************************************************
 * @fn      RangingDBClient_procedureOpen
 *
 * @brief   This function open the ranging procedure data base
 *          variables and array per connection handle.
 *
 * input parameters
 *
 * @param   connHandle - Connection handle.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  SUCCESS - if the connection handle was successfully assigned.
 *          FAILURE - if no available entry found in the DB.
 *          INVALIDPARAMETER - if the connection handle is invalid.
 */
uint8_t RangingDBClient_procedureOpen(uint16_t connHandle)
{
    // Initialize the index to an invalid value
    uint8_t index = INVALID_INDEX;

    // Check if the connection handle is valid
    if (connHandle == LINKDB_CONNHANDLE_INVALID)
    {
        return INVALIDPARAMETER;
    }

    // Check if connHandle exist in the DB
    index = rangingDBClient_GetIndex(connHandle);
    if (index != INVALID_INDEX)
    {
        // If connHandle exist return success
        // and do not assign it again
        return SUCCESS;
    }

    // Found an empty entry, assign the connection handle
    index = rangingDBClient_GetIndex(LINKDB_CONNHANDLE_INVALID);
    if (index != INVALID_INDEX)
    {
        // Assign the connection handle to the empty entry
        gRangingProcedureDB[index].connHandle = connHandle;
        return SUCCESS;
    }

    // No available entry found in the DB
    return FAILURE;
}

/*********************************************************************
 * @fn      RangingDBClient_procedureClose
 *
 * @brief   This function closes the ranging procedure data base.
 *
 * input parameters
 *
 * @param   connHandle - Connection handle.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  SUCCESS - if the connection handle was successfully cleared.
 *          INVALIDPARAMETER - if the connection handle is invalid.
 */
uint8_t RangingDBClient_procedureClose(uint16_t connHandle)
{
    uint8_t status = INVALIDPARAMETER;

    // Get the index of the connection handle
    uint8_t index = rangingDBClient_GetIndex(connHandle);
    if(index != INVALID_INDEX)
    {
        // Clear the connection handle
        gRangingProcedureDB[index].connHandle = LINKDB_CONNHANDLE_INVALID;
        status = SUCCESS;
    }

    return status;
}

/*********************************************************************
 * @fn      RangingDBClient_addData
 *
 * @brief   This function Add raw Data to the Ranging DB.
 *
 * input parameters
 *
 * @param   connHandle - Connection handle.
 * @param   datalen - Length of the data to be added.
 * @param   pData - Pointer to the data to be added.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  SUCCESS - if the data was successfully added.
 *          INVALIDPARAMETER - if the input parameters are invalid.
 */
uint8_t RangingDBClient_addData(uint16_t connHandle, uint16_t offset ,uint16_t datalen, uint8_t *pData)
{
    uint8_t status = SUCCESS;
    // Get the index of the connection handle
    uint8_t index = rangingDBClient_GetIndex(connHandle);

    // Check if the connection handle, data length and offset are valid.
    if( (pData != NULL) ||
        (index != INVALID_INDEX) ||
        ((datalen + offset) < MAX_PROCEDURE_SIZE) )
    {
        // Add the data to the procedure DB
        memcpy(&gRangingProcedureDB[index].procedureData[offset], pData, datalen);
    }
    else
    {
        // Invalid input parameters
        status = INVALIDPARAMETER;
    }

    return status;
}

/*********************************************************************
 * @fn      RangingDBClient_getData
 *
 * @brief   This function retrieves data from the ranging procedure DB.
 *
 * input parameters
 *
 * @param   connHandle - Connection handle.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  Pointer to the procedure data
 *          NULL - if invalid parameters or no data available.
 */
uint8_t* RangingDBClient_getData(uint16_t connHandle)
{
    // Get the index of the connection handle
    uint8_t index = rangingDBClient_GetIndex(connHandle);

    if(index == INVALID_INDEX)
    {
        // Invalid parameters or no data available
        return NULL;
    }
    else
    {
        // Return the pointer to the procedure data
        return gRangingProcedureDB[index].procedureData;
    }
}

/*********************************************************************
 * @fn      RangingDBClient_clearProcedure
 *
 * @brief   This function clears the ranging procedure data for a given
 *          connection handle.
 *
 * input parameters
 *
 * @param   connHandle - Connection handle.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  SUCCESS - if the connection handle was successfully cleared.
 *         INVALIDPARAMETER - if the connection handle is invalid.
 */
uint8_t RangingDBClient_clearProcedure(uint16_t connHandle)
{
    uint8_t status = INVALIDPARAMETER;
    // Get the index of the connection handle
    uint8_t index = rangingDBClient_GetIndex(connHandle);

    if(index != INVALID_INDEX)
    {
        // Clear the procedure DB for the given connection handle
        memset(&gRangingProcedureDB[index], 0, MAX_PROCEDURE_SIZE);
        status = SUCCESS;
    }

    return status;
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      rangingDBClient_GetIndex
 *
 * @brief   This function retrieves the index of the ranging procedure DB
 *          for a given connection handle.
 *
 * input parameters
 *
 * @param   connHandle - Connection handle.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  Index of the ranging procedure DB,
 *          INVALID_INDEX - if the connection handle is not found.
 */
static uint8_t rangingDBClient_GetIndex(uint16_t connHandle)
{
    uint8_t index = INVALID_INDEX;

    // Loop through the ranging procedure database to find the connection handle
    for(uint8_t i = 0; i < MAX_NUM_PROCEDURES; i++)
    {
        if(gRangingProcedureDB[i].connHandle == connHandle)
        {
            // Found the connection handle, store the index
            index = i;
            break;
        }
    }

    return index;
}
