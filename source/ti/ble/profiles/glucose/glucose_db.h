/******************************************************************************

 @file  glucose_db.h

 @brief This file contains the Data Stream profile definitions and prototypes.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2025, Texas Instruments Incorporated
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

#ifndef GLUCOSEDB_H
#define GLUCOSEDB_H


/*********************************************************************
 * INCLUDES
 */

#include <ti/ble/services/glucose/glucose_server.h>

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

// Max Number of Records in the DB
#define GL_DB_MAX_NUM_OF_RECORDS   210

// GL Measurements Data Base Records Info
// (This is due to the cyclic manner of the data base)
typedef struct
{
  uint16               measDBMaxNumRecords;    // The current max number of records in the data base
  uint16               measDBHead;             // The index of the minimum record - Head
  uint16               measDBTail;             // The index of the maximum record - Tail
} GLS_DB_recordsInfo_entry_t;

/*********************************************************************
 * Profile Callback
 */

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      GL_DB_initDataBase
 *
 * @brief   This function initializes the GL's measurements records
 *          data base variables and array.
 *
 * @param   samplingInterval - Current Measurements Interval.
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t GL_DB_initDataBase(void);

/*********************************************************************
 * @fn      GL_DB_AddMeasRecord
 *
 * @brief   This function add a measurement record to the data base.
 *
 *
 * @param   pMeasRecord - pointer to the record's structure info.
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t GL_DB_addMeasRecord( GLS_measRecord_t *pMeasRecord );

/*********************************************************************
 * @fn      GL_DB_GetMeasRecord
 *
 * @brief   This function gets a measurement record (single) from the data base.
 *          Notice that the records number needs to be in the current records range.
 *
 * @param   timeOffset    - the time offset of the wanted record to extract.
 * @param   pMeasRecord   - pointer to the record's structure info that
 *                          would be filled.
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t GL_DB_getMeasRecord(uint16 timeOffset, GLS_measRecord_t **pMeasRecord);

/*********************************************************************
 * @fn      GL_DB_GetCurrentNumOfRecords
 *
 * @brief   This function gets the current number of measurements records
 *          in the data base.
 *
 * @param   None.
 *
 * @return  GL_DB_NumOfRecords Value
 */
uint16 GL_DB_getCurrentNumOfRecords(void);

/*********************************************************************
 * @fn      GL_DB_getCurrentNumOfRecords
 *
 * @brief   This function gets the current number of measurements records
 *          in the data base.
 *
 * @param   None.
 *
 * @return  GL_DB_NumOfRecords ValueGREATER
 */
uint16 GL_DB_getNumOfRecordsGreaterOrEqual( uint16 timeOffset);

/*********************************************************************
 * @fn      GL_DB_getFirstSequenceNum
 *
 * @brief   This function returns the oldest sequence number in the data base
 *
 * @param   None.
 *
 * @return  GL_DB_NumOfRecords ValueGREATER
 */
uint16 GL_DB_getFirstSequenceNum(void);

/*********************************************************************
 * @fn      GL_DB_getLastSequenceNum
 *
 * @brief   This function returns the newest sequence number in the data base
 *
 * @param   None.
 *
 * @return  GL_DB_NumOfRecords ValueGREATER
 */
uint16 GL_DB_getLastSequenceNum(void);

/*********************************************************************
*********************************************************************/

#endif
