/******************************************************************************

 @file  ranging_db_client.h

 @brief This file contains the ranging client database APIs and functionality.

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

 /*********************************************************************
  * MACROS
  */
#define MAX_NUM_PROCEDURES 1        // Maximum number of procedures supported
#define MAX_PROCEDURE_SIZE 0x1400   // Maximum size of a procedure in bytes

 /*********************************************************************
  * TYPEDEFS
  */
 // Structure to hold the ranging procedure data for each connection handle.
typedef struct
{
    uint8_t   procedureData[MAX_PROCEDURE_SIZE]; // Buffer to hold the procedure data.
    uint16_t  connHandle;                        // Connection handle.
} RangingDB_procedureData_t;

 /*********************************************************************
  * Profile Callback
  */

 /*********************************************************************
  * API FUNCTIONS
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
uint8_t RangingDBClient_initDB(void);

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
uint8_t RangingDBClient_procedureOpen(uint16_t connHandle);

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
uint8_t RangingDBClient_procedureClose(uint16_t connHandle);

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
uint8_t RangingDBClient_addData(uint16_t connHandle, uint16_t offset ,uint16_t datalen, uint8_t *pData);

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
uint8_t* RangingDBClient_getData(uint16_t connHandle);

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
uint8_t RangingDBClient_clearProcedure(uint16_t connHandle);
