/******************************************************************************

 @file  ranging_db_client.h

 @brief This file contains the ranging client database APIs and functionality.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025-2026, Texas Instruments Incorporated
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

#ifndef RANGING_DB_CLIENT_H
#define RANGING_DB_CLIENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef RANGING_CLIENT

 /*********************************************************************
  * INCLUDES
  */

#include "ti/ble/services/ranging/ranging_types.h"
#include "ti/drivers/utils/List.h"

 /*********************************************************************
  * MACROS
  */

 /*********************************************************************
  * DEFINES
  */

// Maximum number of procedures supported
#define RANGING_DB_CLIENT_MAX_NUM_PROC  ((uint8_t) 1)

// Invalid handle value
#define RANGING_DB_CLIENT_INVALID_HANDLE ((uint8_t) 0xFF)

 /*********************************************************************
  * TYPEDEFS
  */

/**
 * @brief Structure representing a segment element in the Ranging DB Client.
 *
 * This structure holds information about a single segment, including its
 * position in a list, segment number, size, and a pointer to the segment data.
 */
typedef struct {
    List_Elem elem;         // List element for linking in a list structure.
    uint16_t segmentNum;    // The segment number (identifier for the segment).
    uint16_t segmentSize;   // The size of the segment data in bytes.
    uint8_t* segmentData;   // Pointer to segment data structure
} RangingDBClient_segmentElem_t;

/**
 * @brief Structure representing a procedure segments reader in the Ranging DB Client.
 *
 * This structure holds information about the segmented procedure data, including
 * a list of segment elements, the number of segments, the size of each segment,
 * and the size of the last segment.
 *
 * It is used for reading and parsing the segmented data of a ranging procedure,
 * using dedicated functions to retrieve ranging headers and subevents:
 * @ref RangingDBClient_getRangingHeader and @ref RangingDBClient_getNextSubevent.
 *
 * In order to free the resources allocated for this structure, the function
 * @ref RangingDBClient_freeSegmentsReader should be used.
 *
 * @warning The structure maintains internal state (segment index, offset, bytes read)
 *          to maintain sequential reading of the data.
 *          Do not modify these fields directly; use the provided API functions instead.
 */
typedef struct
{
    // Segmented procedure data
    List_List segmentDataList;    // List of segment elements. Elements are of type @ref RangingDBClient_segmentElem_t
    uint8_t   numSegments;        // number of segments in the procedure
    uint16_t  totalSegmentsSize;  // total size of all segments in bytes

    // Parser state
    RangingDBClient_segmentElem_t* segmentIndex;  // current segment index element
    uint16_t  offset;                             // current offset in the current segment
    uint16_t  bytesRead;                          // total bytes read so far

} RangingDBClient_procedureSegmentsReader_t;

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
 * @brief   This function open a new ranging procedure DB.
 *
 * input parameters
 *
 * @param   None
 *
 * output parameters
 *
 * @param   None
 *
 * @return  @ref RANGING_DB_CLIENT_INVALID_HANDLE - if no available entry found in the DB.
 * @return  Handle of the opened procedure DB otherwise
 */
uint8_t RangingDBClient_procedureOpen(void);

/*********************************************************************
 * @fn      RangingDBClient_procedureClose
 *
 * @brief   This function closes the ranging procedure DB.
 *
 * input parameters
 *
 * @param   handle - Handle of the procedure DB to close.
 *
 * output parameters
 *
 * @param   None
 *
 * @return SUCCESS - if the given handle was successfully cleared.
 * @return INVALIDPARAMETER - if the given handle is invalid or not
 *                            assigned.
 */
uint8_t RangingDBClient_procedureClose(uint8_t handle);

/*********************************************************************
 * @fn      RangingDBClient_addData
 *
 * @brief   This function Add raw Data to the Ranging DB.
 *          It adds the data as long as the total procedure size
 *          does not exceed the maximum allowed size, and the segment
 *          number is valid and was not added before.
 *
 * input parameters
 *
 * @param   handle - Handle of the procedure DB.
 * @param   segmentNum - Segment number in the procedure data to add the new data.
 * @param   datalen - Length of the data to be added.
 * @param   pData - Pointer to the data to be added.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  SUCCESS - if the data was successfully added.
 *          bleMemAllocError - if memory allocation failed.
 *          INVALIDPARAMETER - if the input parameters are invalid.
 */
uint8_t RangingDBClient_addData(uint8_t handle, uint8_t segmentNum, uint16_t datalen, uint8_t *pData);

/*********************************************************************
 * @fn      RangingDBClient_getData
 *
 * @brief   This function retrieves data from the ranging procedure DB.
 *          It builds a segments reader structure that can be used to
 *          parse the procedure data.
 *          Before building, it validates that all segments have been received
 *          and that the data is continuous, according to segments numbers.
 *          If successful, the procedure is cleared from the DB.
 *
 * input parameters
 *
 * @param   handle - Handle of the procedure DB.
 *
 * output parameters
 *
 * @param   segmentsReader - Pointer to the procedure data reader structure to be built
 *
 * @return  SUCCESS - if the data was successfully retrieved.
 *          INVALIDPARAMETER - if the input parameters are invalid, or
 *                             not all segments have been received.
 *          bleMemAllocError - if memory allocation failed.
 */
uint8_t RangingDBClient_getData(uint8_t handle, RangingDBClient_procedureSegmentsReader_t* segmentsReader);

/*********************************************************************
 * @fn      RangingDBClient_clearProcedure
 *
 * @brief   This function clears the ranging procedure data for a given
 *          handle.
 *
 * input parameters
 *
 * @param   handle - Handle of the procedure DB.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  SUCCESS - if the given handle was successfully cleared.
 *          INVALIDPARAMETER - if the given handle is invalid.
 */
uint8_t RangingDBClient_clearProcedure(uint8_t handle);

/*********************************************************************
 * @fn      RangingDBClient_getRangingHeader
 *
 * @brief   This function retrieves the ranging header from the procedure
 *          segments reader.
 *          If successful, the segments reader is updated to point after
 *          the ranging header.
 *          If the function fails, the segments reader is not modified.
 *
 * input parameters
 *
 * @param   segmentsReader - segments reader structure to use. Assumed to be
 *                           positioned at the start of the subevent header.
 * @param   rangingHeaderOut - Pointer to store the retrieved ranging header.
 *
 * @return  SUCCESS - if the ranging header was successfully read.
 *          FAILURE - if there was an error during parsing the data or invalid parameters.
 */
uint8_t RangingDBClient_getRangingHeader(RangingDBClient_procedureSegmentsReader_t* segmentsReader, Ranging_RangingHeader_t* rangingHeaderOut);

/*********************************************************************
 * @fn      RangingDBClient_getNextSubevent
 *
 * @brief   This function retrieves the next subevent data from the
 *          procedure segments reader. Should be called after
 *          @ref RangingDBClient_getRangingHeader
 *
 * input parameters
 *
 * @param   segmentsReader - segments reader structure to use. Assumed to be
 *                           positioned at the start of the subevent header.
 *                           If the function fails, the segmentsReader is not modified.
 * @param   numAntPath - number of antenna paths used in the ranging procedure.
 *                       Implied by the ranging header previously read.
 * @param   role - role of the device in the ranging procedure.
 *
 * output parameters
 *
 * @param   subeventHeaderOut - Pointer to store the retrieved subevent header.
 *                              If NULL, the function will return @ref bleMemAllocError
 * @param   subeventDataOut - Pointer to store the retrieved subevent data.
 *                            Memory for this pointer is allocated within the
 *                            function and should be freed by the caller.
 *                            If NULL, the function will return @ref bleMemAllocError
 *
 * @return  SUCCESS - if the subevent header and data was successfully retrieved.
 *          FAILURE - if there was an error during parsing the data or invalid parameters.
 *          bleMemAllocError - if memory allocation failed.
 */
uint8_t RangingDBClient_getNextSubevent(RangingDBClient_procedureSegmentsReader_t* segmentsReader, uint8_t numAntPath, uint8_t role,
                                        Ranging_subEventHeader_t* subeventHeaderOut, uint8_t** subeventDataOut);

/*********************************************************************
 * @fn      RangingDBClient_freeSegmentsReader
 *
 * @brief   This function frees the segments data allocated
 *          within a procedure segments reader structure and
 *          clears its contents.
 *
 * input parameters
 *
 * @param   segmentsReader - Pointer to the procedure segments reader structure.
 *                           If NULL, the function does nothing.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
void RangingDBClient_freeSegmentsReader(RangingDBClient_procedureSegmentsReader_t* segmentsReader);

#endif // RANGING_CLIENT

#ifdef __cplusplus
}
#endif

#endif /* RANGING_DB_CLIENT_H */
