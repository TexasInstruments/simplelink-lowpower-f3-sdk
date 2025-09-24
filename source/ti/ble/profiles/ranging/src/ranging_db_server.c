/******************************************************************************

 @file  ranging_db_server.c

 @brief This file contains the ranging server database APIs and functionality.

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
#include "ti/ble/stack_util/icall/app/icall_ble_api.h"
#include "ti/ble/profiles/ranging/ranging_db_server.h"

/*********************************************************************
 * MACROS
 */

// Maximum number of procedures supported
#define RANGING_DB_SERVER_MAX_NUM_PROCEDURES (1)
// Maximum size of a procedure in bytes
#define RANGING_DB_SERVER_MAX_PROCEDURE_SIZE (0x1400)
// Invalid index
#define RANGING_DB_SERVER_INVALID_INDEX      (-1)

/*********************************************************************
 * TYPEDEFS
 */

/*
* This structure is used to store and manage information related to
* ranging database procedures in the BLE ranging profile server.
*/
typedef struct
{
    /* Procedure unique ID */
    RangingDBServer_procedureId_t procedureId;

    /* Index of the next available byte to write in the procedure buffer (buffer actual size) */
    uint16_t                      curDataLen;

    /* Buffer to hold the procedure data */
    uint8_t                       pBuf[RANGING_DB_SERVER_MAX_PROCEDURE_SIZE];

    /* Index of the next procedure in the static array ( @ref RANGING_DB_SERVER_INVALID_INDEX if none) */
    int8_t                        nextProcIndex;
} RangingDBServer_procedureDB_t;

/*
* This structure is used to manage the ranging database server control block.
*/
typedef struct
{
    /* Ranging Profile DB. each entry represent different procedure. */
    RangingDBServer_procedureDB_t procedureDB[RANGING_DB_SERVER_MAX_NUM_PROCEDURES];

    /* Head index of the linked list (Oldest procedure) */
    int8_t                        procedureHead;

    /* Tail index of the linked list (Most recent procedure) */
    int8_t                        procedureTail;

    /* Current size of the linked list */
    uint8_t                       procedureCount;
} RangingDBServer_ControlBlock_t;

/*********************************************************************
 * LOCAL VARIABLES
 */

static RangingDBServer_ControlBlock_t gRangingDBServerCB;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t rangingDBServer_GetProcedureIndex(RangingDBServer_procedureId_t procedureId);
static uint8_t rangingDBServer_IsDBFull(void);
static uint8_t rangingDBServer_RemoveProcedure(RangingDBServer_procedureId_t procedureId);
static RangingDBServer_procedureId_t rangingDBServer_RemoveLRUProcedure(void);
static RangingDBServer_procedureDB_t* rangingDBServer_AddNewProcedure(RangingDBServer_procedureId_t procedureId);
static void rangingDBServer_ClearIndex(uint8_t index);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
* @fn      RangingDBServer_InitDB
*
* @brief   This function initializes the ranging procedure database.
*
* input parameters
*
* @param  None.
*
* output parameters
*
* @param  None.
*
* @return  None.
*/
void RangingDBServer_InitDB(void)
{
    // Initialize the ranging procedure database
    for (uint8_t i = 0; i < RANGING_DB_SERVER_MAX_NUM_PROCEDURES; i++)
    {
        // Initialize each procedure entry
        rangingDBServer_ClearIndex(i);
    }
    // Set the head and tail of the linked list to invalid index
    gRangingDBServerCB.procedureHead = RANGING_DB_SERVER_INVALID_INDEX;
    gRangingDBServerCB.procedureTail = RANGING_DB_SERVER_INVALID_INDEX;
    // Set the procedure count to zero
    gRangingDBServerCB.procedureCount = 0;
}

/*********************************************************************
 * @fn      RangingDBServer_procedureStart
 *
 * @brief  This function initializes new ranging procedure data in the database.
 *
 * @note  The caller should check if the pProcedureId value was changed to
 *        determine if data was overwritten.
 *
 * input parameters
 *
 * @param  pProcedureId - Pointer to the procedure ID structure.
 * @param  pData - Pointer to the data to be added.
 * @param  dataLen - Length of the data to be added.
 *
 * output parameters
 *
 * @param pProcedureId - if data was over written, this will contain the procedure ID
 *                       that was overwritten.
 *
 * @return  SUCCESS - if procedure ID was added successfully.
 *          FAILURE - if procedure ID already exists in the database.
 *          bleNoResources - if no resources available to add the procedure.
 *          bleInvalidRange - if the data length exceeds the maximum size.
 *          INVALIDPARAMETER - if the input parameters are invalid.
 */
uint8_t RangingDBServer_procedureStart(RangingDBServer_procedureId_t *pProcedureId, uint8_t *pData, uint16_t dataLen)
{
    uint8_t status = INVALIDPARAMETER;
    uint8_t index;
    RangingDBServer_procedureDB_t *procedure = NULL;
    // Procedure ID to be used for the new procedure
    RangingDBServer_procedureId_t recvProcedureId;

    // validate the input parameters
    if ( (pProcedureId != NULL) && (pData != NULL) && (dataLen != 0) )
    {
        // Check if the procedure ID already exists in the database
        index = rangingDBServer_GetProcedureIndex(*pProcedureId);
        if (index < RANGING_DB_SERVER_MAX_NUM_PROCEDURES)
        {
            // Procedure ID already exists, status FAILURE
            status = FAILURE;
        }
        else
        {
            // Save the received procedure ID
            recvProcedureId = *pProcedureId;

            // Check if the database is full
            if (rangingDBServer_IsDBFull() == TRUE)
            {
                /*
                 * Database is full, remove the oldest procedure and
                 * update the procedure ID to the one that was removed
                 */
                *pProcedureId = rangingDBServer_RemoveLRUProcedure();
            }

            // Add the new procedure ID to the database
            procedure = rangingDBServer_AddNewProcedure(recvProcedureId);

            if (procedure == NULL)
            {
                // No resources available
                status = bleNoResources;
            }
            else if ( (procedure->curDataLen + dataLen) > RANGING_DB_SERVER_MAX_PROCEDURE_SIZE )
            {
                // the data length exceeds the maximum size
                status = bleInvalidRange;
            }
            else
            {
                // Copy the data to the procedure buffer
                memcpy(&procedure->pBuf[procedure->curDataLen], pData, dataLen);
                // Update curDataLen
                procedure->curDataLen += dataLen;
                // Update status
                status = SUCCESS;
            }
        }
    }

    return (status);
}

/*********************************************************************
 * @fn      RangingDBServer_AddData
 *
 * @brief   This function add raw data to the Ranging DB for a given
 *          procedure ID in the next available place.
 *
 * input parameters
 *
 * @param   procedureId - procedure ID structure.
 * @param   dataLen - Length of the data to be added.
 * @param   pData - Pointer to the data to be added.
 *
 * output parameters
 *
 * @param   None.
 *
 * @return  SUCCESS - if data was added successfully.
 *          INVALIDPARAMETER - if the input parameters are invalid.
 *          bleInvalidRange - if the data length exceeds the maximum size.
 */
uint8_t RangingDBServer_AddData(RangingDBServer_procedureId_t procedureId, uint8_t *pData, uint16_t dataLen)
{
    uint8_t status = INVALIDPARAMETER;
    uint8_t index = rangingDBServer_GetProcedureIndex(procedureId);

    // Check if the procedure ID is valid or if the data pointer is NULL
    if ( (pData != NULL) && (index < RANGING_DB_SERVER_MAX_NUM_PROCEDURES) && (dataLen != 0) )
    {
        // Check if the written data creating overflow
        if (gRangingDBServerCB.procedureDB[index].curDataLen + dataLen <= RANGING_DB_SERVER_MAX_PROCEDURE_SIZE)
        {
            // Copy the data to the procedure buffer
            memcpy(&gRangingDBServerCB.procedureDB[index].pBuf[gRangingDBServerCB.procedureDB[index].curDataLen], pData, dataLen);
            // Update the last written byte index
            gRangingDBServerCB.procedureDB[index].curDataLen += dataLen;
            // Successfully added data
            status = SUCCESS;
        }
        else
        {
            // Data length exceeds maximum size
            status = bleInvalidRange;
        }

    }

    return status;
}

/*********************************************************************
 * @fn      RangingDBServer_GetData
 *
 * @brief   This function retrieves data from the ranging procedure DB.
 *
 * input parameters
 *
 * @param   pProcedureId - Procedure ID structure.
 * @param   dataLen - Pointer to the length of the data to retrieve.
 * @param   offset - Offset from which to start retrieving data.
 *
 * output parameters
 *
 * @param   dataLen - returns the actual length of the data retrieved.
 * @param   dataFinished - Pointer to a flag indicating if all data of
 *                         the procedure has been retrieved.
 *                         if TRUE - all data has been retrieved.
 *                         if FALSE - more data is available.
 *
 * @return  Pointer to the requested data segment, or NULL if invalid
 *          parameters or no data available.
 */
uint8_t* RangingDBServer_GetData(RangingDBServer_procedureId_t procedureId, uint16_t *dataLen, uint16_t offset, uint8_t *dataFinished)
{
    uint8_t *retData = NULL;
    uint8_t index = rangingDBServer_GetProcedureIndex(procedureId);

    // Check if the procedure ID is valid
    if ( (dataLen != NULL)                               &&
         (dataFinished != NULL)                          &&
         (index < RANGING_DB_SERVER_MAX_NUM_PROCEDURES)  &&
         (offset < RANGING_DB_SERVER_MAX_PROCEDURE_SIZE)
       )
    {
        // Set data Finished indicator to FALSE
        *dataFinished = FALSE;

        // Check if the offset is within the valid range
        if (offset < gRangingDBServerCB.procedureDB[index].curDataLen)
        {
            // Check If the requested data length is greater than the available data
            if ( (*dataLen + offset) >= gRangingDBServerCB.procedureDB[index].curDataLen )
            {
                // Indicate that all data has been retrieved
                *dataFinished = TRUE;
                // Set the actual data length to the remaining data
                *dataLen = gRangingDBServerCB.procedureDB[index].curDataLen - offset;
            }
            retData = &gRangingDBServerCB.procedureDB[index].pBuf[offset];
        }
    }

    return retData;
}

/*********************************************************************
 * @fn      RangingDBServer_ClearProcedure
 *
 * @brief   This function clears the ranging procedure data for a given procedure ID.
 *
 * input parameters
 *
 * @param   procedureId - Procedure ID of the device.
 *
 * output parameters
 *
 * @param   None.
 *
 * @return  SUCCESS or INVALIDPARAMETER if the procedure ID is invalid.
 */
uint8_t RangingDBServer_ClearProcedure(RangingDBServer_procedureId_t procedureId)
{
    uint8_t status = INVALIDPARAMETER;
    uint8_t index = rangingDBServer_GetProcedureIndex(procedureId);

    // Check if the connection handle is valid
    if (index < RANGING_DB_SERVER_MAX_NUM_PROCEDURES)
    {
        // Remove the procedure from the database
        status = rangingDBServer_RemoveProcedure(procedureId);
        if (status == SUCCESS)
        {
            // Clear the procedure data
            rangingDBServer_ClearIndex(index);
        }
    }
    return status;
}

/*********************************************************************
 * @fn      RangingDBServer_ClearConnHandle
 *
 * @brief   This function clears all procedure data associated with a given connection handle.
 *
 * input parameters
 *
 * @param   connHandle - Connection handle of the device.
 *
 * output parameters
 *
 * @param   None.
 *
 * @return  None
 */
void RangingDBServer_ClearConnHandle(uint16_t connHandle)
{
    // Clear all procedures associated with the connection handle
    for (uint8_t i = 0; i < RANGING_DB_SERVER_MAX_NUM_PROCEDURES; i++)
    {
        if (gRangingDBServerCB.procedureDB[i].procedureId.connHandle == connHandle)
        {
            // Remove the procedure from the linked list
            rangingDBServer_RemoveProcedure(gRangingDBServerCB.procedureDB[i].procedureId);
        }
    }
}

/*********************************************************************
 * @fn      RangingDBServer_GetCurrentDataLen
 *
 * @brief   This function retrieves the current data length for a given procedure ID
 *
 * input parameters
 *
 * @param   procedureId - Procedure ID of the device.
 *
 * output parameters
 *
 * @param   None.
 *
 * @return Current data length,
 *         or RANGING_DB_SERVER_INVALID_DATA_LEN if the procedure ID is invalid.
 */
uint16_t RangingDBServer_GetCurrentDataLen(RangingDBServer_procedureId_t procedureId)
{
    uint16_t dataLen = RANGING_DB_SERVER_INVALID_DATA_LEN;
    uint8_t index = rangingDBServer_GetProcedureIndex(procedureId);

    // Check if the procedure ID is valid
    if (index < RANGING_DB_SERVER_MAX_NUM_PROCEDURES)
    {
        // Get the current data length
        dataLen = gRangingDBServerCB.procedureDB[index].curDataLen;
    }

    return dataLen;
}

/*********************************************************************
 * @fn      RangingDBServer_UpdateData
 *
 * @brief   This function updates raw data to the Ranging DB for
 *          a given procedure ID
 *
 * input parameters
 *
 * @param   procedureId - procedure ID structure.
 * @param   pData - Pointer to the data to be updated.
 * @param   dataLen - Length of the data to be updated.
 * @param   offset - Offset from which to start updating data.
 *
 * output parameters
 *
 * @param   None.
 *
 * @return  SUCCESS - if data was updated successfully.
 *          INVALIDPARAMETER - if the input parameters are invalid.
 *          bleInvalidRange - if the data length exceeds the maximum size.
 */
uint8_t RangingDBServer_UpdateData(RangingDBServer_procedureId_t procedureId, uint8_t *pData, uint16_t dataLen, uint16_t offset)
{
    uint8_t status = INVALIDPARAMETER;
    uint8_t index = rangingDBServer_GetProcedureIndex(procedureId);

    // Check if the procedure ID is valid or if the data pointer is NULL
    if ( (pData != NULL) && (index < RANGING_DB_SERVER_MAX_NUM_PROCEDURES) && (dataLen != 0) )
    {
        // Check if the written data creating overflow
        if (offset + dataLen <= gRangingDBServerCB.procedureDB[index].curDataLen)
        {
            // Copy the data to the procedure buffer
            memcpy(&gRangingDBServerCB.procedureDB[index].pBuf[offset], pData, dataLen);
            // Successfully updated data
            status = SUCCESS;
        }
        else
        {
            // Data length exceeds maximum size
            status = bleInvalidRange;
        }
    }

    return status;
}

/*********************************************************************
 * @fn      RangingDBServer_isProcedureExist
 *
 * @brief   This function checks if a procedure ID exists in the database.
 *
 * input parameters
 *
 * @param   procedureId - procedure ID structure.
 *
 * output parameters
 *
 * @param   None.
 *
 * @return  TRUE - if the procedure ID exists in the database.
 *          FALSE - if the procedure ID does not exist in the database.
 */
bool RangingDBServer_isProcedureExist(RangingDBServer_procedureId_t procedureId)
{
    bool isExists = FALSE;

    if(rangingDBServer_GetProcedureIndex(procedureId) < RANGING_DB_SERVER_MAX_NUM_PROCEDURES)
    {
        // Procedure ID exists in the database
        isExists = TRUE;
    }

    return isExists;
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      rangingDBServer_GetProcedureIndex
 *
 * @brief   This function retrieves the index associated with a given
 *          procedure ID in the ranging database.
 *
 * input parameters
 *
 * @param   procedureId - Procedure ID of the device.
 *
 * output parameters
 *
 * @param   None.
 *
 * @return  Index of the procedure or an invalid value if the procedure ID is not found.
 */
static uint8_t rangingDBServer_GetProcedureIndex(RangingDBServer_procedureId_t procedureId)
{
    int8_t currentIndex = gRangingDBServerCB.procedureHead;
    uint8_t retIndex = RANGING_DB_SERVER_MAX_NUM_PROCEDURES;

    // Iterate through the linked list to find the procedure ID
    while (currentIndex != RANGING_DB_SERVER_INVALID_INDEX)
    {
        if (gRangingDBServerCB.procedureDB[currentIndex].procedureId.connHandle == procedureId.connHandle &&
            gRangingDBServerCB.procedureDB[currentIndex].procedureId.rangingCounter == procedureId.rangingCounter)
        {
            // Found the procedure
            retIndex = currentIndex;
            break;
        }
        // Move to the next procedure
        currentIndex = gRangingDBServerCB.procedureDB[currentIndex].nextProcIndex;
    }
    return (retIndex);
}

/*********************************************************************
 * @fn      rangingDBServer_IsDBFull
 *
 * @brief   This function checks if the ranging database is full.
 *
 * input parameters
 *
 * @param   None.
 *
 * output parameters
 *
 * @param   None.
 *
 * @return  TRUE if the database is full, FALSE otherwise.
 */
static uint8_t rangingDBServer_IsDBFull(void)
{
    uint8_t reVal = FALSE;
    if ( gRangingDBServerCB.procedureCount >= RANGING_DB_SERVER_MAX_NUM_PROCEDURES )
    {
        // Database is full
        reVal = TRUE;
    }
    return (reVal);
}

/*********************************************************************
 * @fn      rangingDBServer_RemoveProcedure
 *
 * @brief   This function removes a procedure from the ranging database.
 *
 * input parameters
 *
 * @param   procedureId - Procedure ID of the device.
 *
 * output parameters
 *
 * @param   None.
 *
 * @return  SUCCESS or INVALIDPARAMETER if the procedure ID is invalid.
 */
static uint8_t rangingDBServer_RemoveProcedure(RangingDBServer_procedureId_t procedureId)
{
    int8_t currentIndex = gRangingDBServerCB.procedureHead;
    int8_t previousIndex = RANGING_DB_SERVER_INVALID_INDEX;
    uint8_t status = INVALIDPARAMETER;

    // Iterate through the linked list to find the procedure ID
    while (currentIndex != RANGING_DB_SERVER_INVALID_INDEX)
    {
        // Check if the current procedure ID matches the one to be removed
        if (gRangingDBServerCB.procedureDB[currentIndex].procedureId.connHandle == procedureId.connHandle &&
            gRangingDBServerCB.procedureDB[currentIndex].procedureId.rangingCounter == procedureId.rangingCounter)
        {

            // Removing the head
            if (previousIndex == RANGING_DB_SERVER_INVALID_INDEX)
            {
                // Update head
                gRangingDBServerCB.procedureHead = gRangingDBServerCB.procedureDB[currentIndex].nextProcIndex;
            }
            else
            {
                // Link the previous node to the next node
                gRangingDBServerCB.procedureDB[previousIndex].nextProcIndex = gRangingDBServerCB.procedureDB[currentIndex].nextProcIndex;
            }
            if (currentIndex == gRangingDBServerCB.procedureTail)
            {
                // Removing the tail old and update new tail
                gRangingDBServerCB.procedureTail = previousIndex;
            }

            // Clear the procedure data
            rangingDBServer_ClearIndex(currentIndex);

            // Reset the procedure ID
            gRangingDBServerCB.procedureCount--;

            // Update the status to indicate success
            status = SUCCESS;
            break;
        }

        // Move to the next procedure
        previousIndex = currentIndex;

        // Update the current index to the next one
        currentIndex = gRangingDBServerCB.procedureDB[currentIndex].nextProcIndex;
    }
    return (status);
}

/*********************************************************************
 * @fn      rangingDBServer_RemoveLRUProcedure
 *
 * @brief   This function removes the LRU (last recently used)
 *          procedure from the ranging database.
 *
 * input parameters
 *
 * @param  None
 *
 * output parameters
 *
 * @param  None
 *
 * @return  The ID of the removed procedure
 */
static RangingDBServer_procedureId_t rangingDBServer_RemoveLRUProcedure(void)
{
    RangingDBServer_procedureId_t removedProcedureId = {0, LINKDB_CONNHANDLE_INVALID};
    int8_t lruIndex = gRangingDBServerCB.procedureHead;

    // Check if the database is empty
    if (gRangingDBServerCB.procedureCount != 0)
    {
        // Save the LRU procedure ID
        removedProcedureId = gRangingDBServerCB.procedureDB[lruIndex].procedureId;
        // Remove the LRU procedure
        rangingDBServer_RemoveProcedure(removedProcedureId);
    }

    return (removedProcedureId);
}

/*********************************************************************
 * @fn      rangingDBServer_AddNewProcedure
 *
 * @brief   This function adds a new procedure to the ranging database.
 *
 * input parameters
 *
 * @param   procedureId - Procedure ID of the device.
 *
 * output parameters
 *
 * @param   None.
 *
 * @return  Pointer to the new procedure or NULL if no resources available.
 */
static RangingDBServer_procedureDB_t* rangingDBServer_AddNewProcedure(RangingDBServer_procedureId_t procedureId)
{
    int8_t newIndex = RANGING_DB_SERVER_INVALID_INDEX;
    RangingDBServer_procedureDB_t *newProcedure = NULL;

    if (rangingDBServer_IsDBFull() == FALSE)
    {
        // Find an unused slot
        for (uint8_t i = 0; i < RANGING_DB_SERVER_MAX_NUM_PROCEDURES; i++)
        {
            if (gRangingDBServerCB.procedureDB[i].procedureId.connHandle == LINKDB_CONNHANDLE_INVALID)
            {
                newIndex = i;
                break;
            }
        }
        if (newIndex == RANGING_DB_SERVER_INVALID_INDEX)
        {
            return NULL;
        }
        // Initialize the new procedure
        gRangingDBServerCB.procedureDB[newIndex].procedureId = procedureId;
        gRangingDBServerCB.procedureDB[newIndex].curDataLen = 0;
        gRangingDBServerCB.procedureDB[newIndex].nextProcIndex = RANGING_DB_SERVER_INVALID_INDEX;

        // Add to the linked list
        if (gRangingDBServerCB.procedureTail == RANGING_DB_SERVER_INVALID_INDEX)
        {
            // List is empty, set head and tail to the new index
            gRangingDBServerCB.procedureHead = newIndex;
        }
        else
        {
            // Link the new procedure to the tail
            gRangingDBServerCB.procedureDB[gRangingDBServerCB.procedureTail].nextProcIndex = newIndex;
        }

        // Update the tail to the new index
        gRangingDBServerCB.procedureTail = newIndex;

        // Increment the procedure count
        gRangingDBServerCB.procedureCount++;

        newProcedure = &gRangingDBServerCB.procedureDB[newIndex];
    }

    return (newProcedure);
}

/*********************************************************************
 * @fn      rangingDBServer_ClearIndex
 *
 * @brief   This function clears the procedure data for a given index.
 *
 * input parameters
 *
 * @param   index - Index of the procedure to clear.
 *
 * @return  None.
 */
static void rangingDBServer_ClearIndex(uint8_t index)
{
    if (index < RANGING_DB_SERVER_MAX_NUM_PROCEDURES)
    {
        // Set the connection handle to invalid
        gRangingDBServerCB.procedureDB[index].procedureId.connHandle = LINKDB_CONNHANDLE_INVALID;
        // Reset the ranging counter
        gRangingDBServerCB.procedureDB[index].procedureId.rangingCounter = 0;
        // Reset the current data length
        gRangingDBServerCB.procedureDB[index].curDataLen = 0;
        // Clear the next procedure index
        gRangingDBServerCB.procedureDB[index].nextProcIndex = RANGING_DB_SERVER_INVALID_INDEX;
        // Clear the procedure buffer
        memset(gRangingDBServerCB.procedureDB[index].pBuf, 0, RANGING_DB_SERVER_MAX_PROCEDURE_SIZE);
    }
}
