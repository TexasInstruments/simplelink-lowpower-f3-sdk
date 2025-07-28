/******************************************************************************

 @file  ranging_db_server.h

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

#ifndef RANGING_DB_SERVER_H
#define RANGING_DB_SERVER_H

 /*********************************************************************
  * INCLUDES
  */

 /*********************************************************************
  * MACROS
  */

 // Invalid data length
#define RANGING_DB_SERVER_INVALID_DATA_LEN 0xFFFF

 /*********************************************************************
  * TYPEDEFS
  */

/*
* This struct represents a unique identifier for
* a ranging procedure in the database.
*/
typedef struct
{
    uint16_t rangingCounter;            // Current Ranging Counter
    uint16_t connHandle;                // Connection handle
} RangingDBServer_procedureId_t;

 /*********************************************************************
  * API FUNCTIONS
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
void RangingDBServer_InitDB(void);

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
uint8_t RangingDBServer_procedureStart(RangingDBServer_procedureId_t *pProcedureId, uint8_t *pData, uint16_t dataLen);

/*********************************************************************
 * @fn      RangingDBServer_AddData
 *
 * @brief   This function Add raw data to the ranging DB for a given
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
uint8_t RangingDBServer_AddData(RangingDBServer_procedureId_t procedureId, uint8_t *pData, uint16_t dataLen);

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
uint8_t* RangingDBServer_GetData(RangingDBServer_procedureId_t procedureId, uint16_t *dataLen, uint16_t offset, uint8_t *dataFinished);

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
uint8_t RangingDBServer_ClearProcedure(RangingDBServer_procedureId_t procedureId);

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
void RangingDBServer_ClearConnHandle(uint16_t connHandle);

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
uint16_t RangingDBServer_GetCurrentDataLen(RangingDBServer_procedureId_t procedureId);

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
uint8_t RangingDBServer_UpdateData(RangingDBServer_procedureId_t procedureId, uint8_t *pData, uint16_t dataLen, uint16_t offset);

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
bool RangingDBServer_isProcedureExist(RangingDBServer_procedureId_t procedureId);

#endif // RANGING_DB_SERVER_H