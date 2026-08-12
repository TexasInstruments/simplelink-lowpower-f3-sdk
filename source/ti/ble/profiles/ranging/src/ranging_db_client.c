/******************************************************************************

 @file  ranging_db_client.c

 @brief This file contains the implementation of the ranging database APIs and
        functionality.

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

#ifdef RANGING_CLIENT
/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include "ti/ble/stack_util/icall/app/icall.h"
/* This Header file contains all BLE API and icall structure definition */
#include "ti/ble/stack_util/icall/app/icall_ble_api.h"
#include "ti/ble/profiles/ranging/ranging_db_client.h"
#include "ti/drivers/utils/List.h"

/*********************************************************************
 * MACROS
 */

// Maximum size of a procedure in bytes
#define RANGING_DB_CLIENT_MAX_PROC_SIZE 0x1400

// Maximum number of segments until wrap around
#define RANGING_DB_CLIENT_MAX_NUM_SEGMENTS 64

// Macro to check if a bit is set in a 64 bit mask
#define RANGING_DB_CLIENT_IS_BIT_SET(bitMask, bit) (((bitMask) & ((uint64_t)1 << (bit))) != 0)

// Macro to check all bits are set in a 64 bit mask
#define RANGING_DB_CLIENT_ARE_ALL_BITS_SET(bitMask) ((bitMask) == 0xFFFFFFFFFFFFFFFF)

/*********************************************************************
 * TYPEDEFS
 */

 // Structure to hold the ranging procedure data for each handle.
typedef struct
{
    // Indicates if this entry is used.
    bool isUsed;

    // Total number of segments added.
    uint16_t totalNumSegments;

    // Total size of all segments added in bytes
    uint16_t totalSegmentsSize;

    // Bitmask to keep track of received segments
    uint64_t segmentsReceivedBitMask;

    // Count of how many times the segments have wrapped around (happens each 64 segments)
    uint8_t segmentsWrapAroundCount;

    // List of segments. Elements of type @ref RangingDBClient_segmentElem_t, Ordered by segment numbers.
    List_List segmentDataList;

} RangingDBClient_procedureData_t;

/*********************************************************************
 * LOCAL VARIABLES
 */
// Ranging Profile DB, each entry represents a different handle
static RangingDBClient_procedureData_t gRangingProcedureDB[RANGING_DB_CLIENT_MAX_NUM_PROC];

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void rangingDBClient_clearIndex(uint8_t index);
static void rangingDBClient_clearProcedure(uint8_t index);
static uint8_t rangingDBClient_getEmptyIndex(void);
static void rangingDBClient_clearList(List_List *list);
static uint8_t rangingDBClient_validateSegments(uint8_t index);
static uint8_t rangingDBClient_getNBytes(RangingDBClient_procedureSegmentsReader_t* segmentsReader, uint16_t n, uint8_t* buffer);
static uint16_t rangingDBClient_getSubeventDataSize(RangingDBClient_procedureSegmentsReader_t segmentsReader, uint8_t numStepsReported,
                                                    uint8_t numAntPath, uint8_t role);
static uint8_t rangingDBClient_getSubeventData(RangingDBClient_procedureSegmentsReader_t* segmentsReader, uint8_t numStepsReported,
                                               uint8_t numAntPath, uint8_t role, uint8_t** subeventDataOut);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*******************************************************************************
 * Public function defined in ranging_db_client.h.
 */
uint8_t RangingDBClient_initDB(void)
{
    // Initialize the ranging procedure database
    for (uint8_t i = 0; i < RANGING_DB_CLIENT_MAX_NUM_PROC; i++)
    {
        rangingDBClient_clearIndex(i);
    }

    return SUCCESS;
}

/*******************************************************************************
 * Public function defined in ranging_db_client.h.
 */
uint8_t RangingDBClient_procedureOpen(void)
{
    // Initialize the index to an invalid value
    uint8_t index = RANGING_DB_CLIENT_INVALID_HANDLE;

    // Get an empty index if possible
    index = rangingDBClient_getEmptyIndex();

    // If found an empty index
    if (index != RANGING_DB_CLIENT_INVALID_HANDLE)
    {
        // Assign a new entry
        gRangingProcedureDB[index].isUsed = true;
    }

    return index;
}

/*******************************************************************************
 * Public function defined in ranging_db_client.h.
 */
uint8_t RangingDBClient_procedureClose(uint8_t handle)
{
    uint8_t status = SUCCESS;

    if (handle >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        gRangingProcedureDB[handle].isUsed != true)
    {
        status = INVALIDPARAMETER;
    }

    if (status == SUCCESS)
    {
        rangingDBClient_clearIndex(handle);
    }

    return status;
}

/*******************************************************************************
 * Public function defined in ranging_db_client.h.
 */
uint8_t RangingDBClient_addData(uint8_t handle, uint8_t segmentNum, uint16_t datalen, uint8_t *pData)
{
    uint8_t status = SUCCESS;
    uint8_t index = handle;

    // Effective segment number considering segments numbers wrap arounds
    uint16_t effectiveSegmentNum = 0;

    // Check parameters
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        gRangingProcedureDB[index].isUsed != true ||
        pData == NULL ||
        datalen == 0)
    {
        return INVALIDPARAMETER;
    }

    // Check:
    // 1. We are not exceeding the maximum procedure size
    // 2. The segment number is valid
    // 3. The segment number was not added before
    if( ((datalen + gRangingProcedureDB[index].totalSegmentsSize) < RANGING_DB_CLIENT_MAX_PROC_SIZE) &&
        (segmentNum < RANGING_DB_CLIENT_MAX_NUM_SEGMENTS) &&
        (!RANGING_DB_CLIENT_IS_BIT_SET(gRangingProcedureDB[index].segmentsReceivedBitMask, segmentNum)) )
    {
        // Allocate memory for the new segment element
        RangingDBClient_segmentElem_t *newSegmentElem = (RangingDBClient_segmentElem_t *)ICall_malloc(sizeof(RangingDBClient_segmentElem_t));

        if (newSegmentElem != NULL)
        {
            // Allocate memory for the segment data
            newSegmentElem->segmentData = ICall_malloc(datalen);
        }

        if (newSegmentElem != NULL && newSegmentElem->segmentData != NULL)
        {
            // Calculate the effective segment number considering segments numbers wrap arounds
            effectiveSegmentNum = gRangingProcedureDB[index].segmentsWrapAroundCount * RANGING_DB_CLIENT_MAX_NUM_SEGMENTS + segmentNum;

            // Initialize the new segment element data info
            newSegmentElem->segmentNum = effectiveSegmentNum;
            newSegmentElem->segmentSize = datalen;

            // Copy the segment data
            memcpy(newSegmentElem->segmentData, pData, datalen);

            // Traverse the list from tail to head in order to find the correct position to insert the new segment
            List_Elem *elem = List_tail(&gRangingProcedureDB[index].segmentDataList);
            RangingDBClient_segmentElem_t *currElem = NULL;

            bool inserted = false;
            while (elem != NULL)
            {
                currElem = (RangingDBClient_segmentElem_t *)elem;
                if (currElem->segmentNum < newSegmentElem->segmentNum)
                {
                    if (elem->next != NULL)
                    {
                        // Insert between elem and elemNext
                        List_insert(&gRangingProcedureDB[index].segmentDataList, (List_Elem *)newSegmentElem, elem->next);
                    }
                    else
                    {
                        // Insert at the tail
                        List_put(&gRangingProcedureDB[index].segmentDataList, (List_Elem *)newSegmentElem);
                    }

                    inserted = true;
                    break;
                }
                elem = List_prev(elem);
            }

            if (!inserted)
            {
                // Insert at the head if no smaller segmentNum found
                List_putHead(&gRangingProcedureDB[index].segmentDataList, (List_Elem *)newSegmentElem);
            }

            // Update total number of segments and total size
            gRangingProcedureDB[index].totalNumSegments++;
            gRangingProcedureDB[index].totalSegmentsSize += datalen;

            // Update segments received bit mask according to the received segment number
            gRangingProcedureDB[index].segmentsReceivedBitMask |= ((uint64_t)1 << segmentNum);

            // If received all 64 segments, the next segments numbers will be 0-63 again,
            // therefore increment wrap arounds counter and reset the received bit mask
            if (RANGING_DB_CLIENT_ARE_ALL_BITS_SET(gRangingProcedureDB[index].segmentsReceivedBitMask))
            {
                gRangingProcedureDB[index].segmentsWrapAroundCount++;
                gRangingProcedureDB[index].segmentsReceivedBitMask = 0;
            }
        }
        else
        {
            // Memory allocation failed
            if (newSegmentElem != NULL)
            {
                ICall_free(newSegmentElem);
            }
            status = bleMemAllocError;
        }
    }
    else
    {
        // Invalid input parameters
        status = INVALIDPARAMETER;
    }

    return status;
}

/*******************************************************************************
 * Public function defined in ranging_db_client.h.
 */
uint8_t RangingDBClient_getData(uint8_t handle, RangingDBClient_procedureSegmentsReader_t* segmentsReader)
{
    uint8_t status = SUCCESS;
    uint8_t index = handle;

    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        gRangingProcedureDB[index].isUsed != true ||
        segmentsReader == NULL)
    {
        status = INVALIDPARAMETER;
    }

    // If procedure data exist
    if (status == SUCCESS && gRangingProcedureDB[index].totalNumSegments > 0)
    {
        // Validate the segments first
        status = rangingDBClient_validateSegments(index);

        // Build the segments reader to be returned to the caller
        if (status == SUCCESS)
        {
            // Initiate segmentsReader fields
            segmentsReader->segmentDataList = gRangingProcedureDB[index].segmentDataList;
            segmentsReader->numSegments = gRangingProcedureDB[index].totalNumSegments;
            segmentsReader->totalSegmentsSize = gRangingProcedureDB[index].totalSegmentsSize;
            segmentsReader->segmentIndex = (RangingDBClient_segmentElem_t*) gRangingProcedureDB[index].segmentDataList.head;
            segmentsReader->offset = 0;
            segmentsReader->bytesRead = 0;

            // reset the list in the DB as its ownership is transferred to the caller
            List_clearList(&gRangingProcedureDB[index].segmentDataList);

            // After resetting the list, update the DB state to reflect the empty list.
            // Note: this won't free the list as it's already not part of the DB.
            rangingDBClient_clearProcedure(index);
        }
    }

    return status;
}

/*******************************************************************************
 * Public function defined in ranging_db_client.h.
 */
uint8_t RangingDBClient_clearProcedure(uint8_t handle)
{
    uint8_t status = SUCCESS;
    uint8_t index = handle;

    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC ||
        gRangingProcedureDB[index].isUsed != true)
    {
        status = INVALIDPARAMETER;
    }

    if(status == SUCCESS)
    {
        // Clear the procedure DB for the given handle
        rangingDBClient_clearProcedure(index);
    }

    return status;
}

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
uint8_t RangingDBClient_getRangingHeader(RangingDBClient_procedureSegmentsReader_t* segmentsReader, Ranging_RangingHeader_t* rangingHeaderOut)
{
     uint8_t status = SUCCESS;

     // copy of the segments reader to avoid modifying the original one in case of failure
     RangingDBClient_procedureSegmentsReader_t segmentsReaderCopy;
     Ranging_RangingHeader_t rangingHeader;

     if (segmentsReader == NULL || rangingHeaderOut == NULL)
     {
         status = FAILURE;
     }

     if (status == SUCCESS)
     {
         // Copy the segments reader
         segmentsReaderCopy = *segmentsReader;

         // Get the ranging header
         status = rangingDBClient_getNBytes(&segmentsReaderCopy, sizeof(Ranging_RangingHeader_t), (uint8_t*) &rangingHeader);
     }

     if (status == SUCCESS)
     {
         // Update the original segments reader only after successful read
         *segmentsReader = segmentsReaderCopy;

         // Output the results
         *rangingHeaderOut = rangingHeader;
     }

     return status;
}

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
                                        Ranging_subEventHeader_t* subeventHeaderOut, uint8_t** subeventDataOut)
{
    uint8_t status = SUCCESS;

    // Reset subevent variables
    Ranging_subEventHeader_t subeventHeader;
    uint8_t* subeventData = NULL;

    // copy of the segments reader to avoid modifying the original one in case of failure
    RangingDBClient_procedureSegmentsReader_t segmentsReaderCopy;

    if (segmentsReader == NULL ||
        subeventHeaderOut == NULL ||
        subeventDataOut == NULL ||
        (numAntPath < CS_MIN_ANT_PATHS) || (numAntPath > CS_MAX_ANT_PATHS) ||
        (role != CS_ROLE_INITIATOR && role != CS_ROLE_REFLECTOR))
    {
        status = FAILURE;
    }

    if (status == SUCCESS)
    {
        // Copy the segments reader
        segmentsReaderCopy = *segmentsReader;

        // Get the first subevent header
        status = rangingDBClient_getNBytes(&segmentsReaderCopy, sizeof(Ranging_subEventHeader_t), (uint8_t*) &subeventHeader);
    }

    if (status == SUCCESS)
    {
        // Get the subevent data
        status = rangingDBClient_getSubeventData(&segmentsReaderCopy, subeventHeader.numStepsReported, numAntPath, role, &subeventData);
    }

    if (status == SUCCESS)
    {
        // Output the results
        *subeventHeaderOut = subeventHeader;
        *subeventDataOut = subeventData;

        // Update the original segments reader only after successful read
        *segmentsReader = segmentsReaderCopy;
    }

    return status;
}

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
void RangingDBClient_freeSegmentsReader(RangingDBClient_procedureSegmentsReader_t* segmentsReader)
{
    if (segmentsReader != NULL)
    {
        // Free each segment data in the segments list
        rangingDBClient_clearList(&segmentsReader->segmentDataList);

        // Clear the segmentsReader structure
        memset(segmentsReader, 0, sizeof(RangingDBClient_procedureSegmentsReader_t));
    }
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

 /*********************************************************************
 * @fn      rangingDBClient_clearIndex
 *
 * @brief   This function clears the ranging procedure data for a given
 *          index in the DB.
 *          Releases all allocated memory for segments.
 *
 * input parameters
 *
 * @param   index - Index in the DB. If invalid, the function does nothing.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
static void rangingDBClient_clearIndex(uint8_t index)
{
    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        gRangingProcedureDB[index].isUsed = false;
        rangingDBClient_clearProcedure(index);
    }
}

 /*********************************************************************
 * @fn      rangingDBClient_clearProcedure
 *
 * @brief   This function clears the ranging procedure data for a given
 *          index in the DB, without clearing the isUsed flag,
 *          i.e the index is still assigned.
 *          Releases all allocated memory for segments.
 *
 * input parameters
 *
 * @param   index - Index in the DB. If invalid, the function does nothing.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
static void rangingDBClient_clearProcedure(uint8_t index)
{
    if (index < RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        gRangingProcedureDB[index].totalNumSegments = 0;
        gRangingProcedureDB[index].totalSegmentsSize = 0;
        gRangingProcedureDB[index].segmentsReceivedBitMask = 0;
        gRangingProcedureDB[index].segmentsWrapAroundCount = 0;
        rangingDBClient_clearList(&gRangingProcedureDB[index].segmentDataList);
    }
}

/*********************************************************************
 * @fn      rangingDBClient_getEmptyIndex
 *
 * @brief   This function retrieves an empty index from the
 *          ranging procedure DB, if exists.
 *
 * input parameters
 *
 * @param   None
 *
 * output parameters
 *
 * @param   None
 *
 * @return Index of an empty entry in the DB.
 * @return @ref RANGING_DB_CLIENT_INVALID_HANDLE - if no empty entry found.
 */
static uint8_t rangingDBClient_getEmptyIndex(void)
{
    uint8_t index = RANGING_DB_CLIENT_INVALID_HANDLE;

    // Loop through the ranging procedure database to find an empty entry
    for(uint8_t i = 0; i < RANGING_DB_CLIENT_MAX_NUM_PROC; i++)
    {
        if(gRangingProcedureDB[i].isUsed != true)
        {
            // Found, store the index
            index = i;
            break;
        }
    }

    return index;
}

/*********************************************************************
 * @fn      rangingDBClient_validateSegments
 *
 * @brief   This function validates that all segments are received
 *          for a given procedure data.
 *          To be used before retrieving the procedure data.
 *
 * input parameters
 *
 * @param   index - Index of the ranging procedure DB.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  INVALIDPARAMETER - if the input parameters are invalid.
 *          FAILURE - If validation failed.
 *          SUCCESS - if all segments are valid.
 */
static uint8_t rangingDBClient_validateSegments(uint8_t index)
{
    uint8_t status = SUCCESS;

    // Validate input parameters
    if (index >= RANGING_DB_CLIENT_MAX_NUM_PROC)
    {
        return INVALIDPARAMETER;
    }

    // Validate that all segment numbers are present from 0 to totalNumSegments-1.
    // Validate that all segments size is the same, except possibly the last segment.
    if (status == SUCCESS)
    {
        List_Elem *elem = List_head(&gRangingProcedureDB[index].segmentDataList);
        RangingDBClient_segmentElem_t *segmentElem;
        uint16_t prevSegmentNum = 0;

        while (elem != NULL)
        {
            segmentElem = (RangingDBClient_segmentElem_t *)elem;

            // Validate segments numbers continuity
            if ((elem == List_head(&gRangingProcedureDB[index].segmentDataList) &&
                 segmentElem->segmentNum != 0) ||
                (elem != List_head(&gRangingProcedureDB[index].segmentDataList) &&
                 segmentElem->segmentNum != (prevSegmentNum + 1)))
            {
                status = FAILURE;
                break;
            }

            // Save the current segment number and go to the next element
            prevSegmentNum = segmentElem->segmentNum;
            elem = List_next(elem);
        }
    }

    return status;
}

/*********************************************************************
 * @fn      rangingDBClient_clearList
 *
 * @brief   This function clears a list of segment elements,
 *          freeing their allocated memory if exists, and
 *          finally clears the list structure.
 *
 * input parameters
 *
 * @param   list - Pointer to the list to be cleared.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  None
 */
static void rangingDBClient_clearList(List_List *list)
{
    List_Elem *elem;
    RangingDBClient_segmentElem_t *segmentElem;

    if (list != NULL)
    {
        // Remove all elements from the list
        while ((elem = List_get(list)) != NULL)
        {
            segmentElem = (RangingDBClient_segmentElem_t *)elem;

            // Free the segment data
            if (segmentElem->segmentData != NULL)
            {
                ICall_free(segmentElem->segmentData);
            }
            // Free the segment element
            ICall_free(segmentElem);
        }

        // Finally, clear the list structure
        List_clearList(list);
    }
}

/*********************************************************************
 * @fn      rangingDBClient_getNBytes
 *
 * @brief   This function retrieves the next N bytes from the procedure segments reader.
 *          It updates the segments reader state accordingly.
 *
 * input parameters
 *
 * @param   segmentsReader - Pointer to the procedure segments reader structure.
 *                           If the function returns FAILURE, the segmentsReader metadata
 *                           might be edited, in this case its values are not reliable.
 * @param   n - Number of bytes to retrieve.
 *
 * output parameters
 *
 * @param   buffer - Pointer to store the retrieved bytes. If NULL, the values are not returned.
 *                   Assumed to have enough space for the requested bytes.
 *                   If the function returns FAILURE, the buffer might still be partially filled,
 *                   although its values are not reliable.
 *
 * @return  SUCCESS - if the requested bytes were successfully retrieved.
 *          FAILURE - if there are not enough bytes to read or segmentsReader is invalid.
 */
static uint8_t rangingDBClient_getNBytes(RangingDBClient_procedureSegmentsReader_t* segmentsReader, uint16_t n, uint8_t* buffer)
{
    uint8_t status = FAILURE;
    uint16_t bytesRemaining;    // Remaining bytes to read

    // Check parameters and that there are enough bytes to read
    if (segmentsReader != NULL && segmentsReader->segmentIndex != NULL &&
        (segmentsReader->bytesRead + n) <= segmentsReader->totalSegmentsSize)
    {
        status = SUCCESS;

        // Initialize remaining bytes to read
        bytesRemaining = n;

        // Loop until all requested bytes are read or no more segments
        while(bytesRemaining > 0 && segmentsReader->segmentIndex != NULL)
        {
            uint16_t bytesAvailableInSegment;   // Bytes available in the current segment
            uint16_t bytesToCopy;               // Bytes to copy from the current segment

            // Calculate how many bytes are available in the current segment
            bytesAvailableInSegment = segmentsReader->segmentIndex->segmentSize - segmentsReader->offset;

            // Calculate how many bytes to copy from the current segment
            bytesToCopy = (bytesRemaining < bytesAvailableInSegment) ? bytesRemaining : bytesAvailableInSegment;

            // Update remaining bytes to read
            if (bytesRemaining > bytesToCopy)
            {
                bytesRemaining -= bytesToCopy;
            }
            else if (bytesRemaining == bytesToCopy)
            {
                bytesRemaining = 0;
            }
            else // bytesRemaining < bytesToCopy
            {
                // This case should not happen due to how bytesToCopy is calculated
                status = FAILURE;
                break;
            }

            // Copy the bytes to the buffer if provided
            if (buffer != NULL)
            {
                memcpy(buffer, &segmentsReader->segmentIndex->segmentData[segmentsReader->offset], bytesToCopy);

                // Move the buffer pointer forward
                buffer += bytesToCopy;
            }

            // Update the segments reader state
            segmentsReader->offset += bytesToCopy;
            segmentsReader->bytesRead += bytesToCopy;

            // If we reached the end of the current segment, move to the next segment
            if (segmentsReader->offset == segmentsReader->segmentIndex->segmentSize)
            {
                // Move to the next segment
                segmentsReader->segmentIndex = (RangingDBClient_segmentElem_t*) segmentsReader->segmentIndex->elem.next;
                segmentsReader->offset = 0;
            }

            // If all requested bytes have been read, exit the loop
            if (bytesRemaining == 0)
            {
                break;
            }
        }
    }

    return status;
}

/*********************************************************************
 * @fn      rangingDBClient_getSubeventDataSize
 *
 * @brief   This function calculates the size of the subevent data in Ranging
 *          procedure based on the number of steps reported, number of antenna paths,
 *          and role.
 *
 * input parameters
 *
 * @param   segmentsReader - segments reader structure to use. Assumed to be
 *                           positioned at the start of the subevent data.
 * @param   numStepsReported - number of steps reported in the subevent.
 * @param   numAntPath - number of antenna paths used in the ranging procedure.
 * @param   role - role of the device in the ranging procedure.
 *
 * output parameters
 *
 * @param   None
 *
 * @return  Size of the subevent data in bytes, or 0 if an error occurred.
 */
static uint16_t rangingDBClient_getSubeventDataSize(RangingDBClient_procedureSegmentsReader_t segmentsReader, uint8_t numStepsReported,
                                                        uint8_t numAntPath, uint8_t role)
{
    Ranging_subEventResultsStep_t stepHdr;
    uint8_t stepLength = 0;     // Current step length
    uint16_t totalLength = 0;   // Total subevent length

    for(uint16_t i = 0; i < numStepsReported; i++)
    {
        // Reset current step length
        stepLength = 0;

        // Get step header
        if (rangingDBClient_getNBytes(&segmentsReader, sizeof(Ranging_subEventResultsStep_t), (uint8_t*) &stepHdr) != SUCCESS)
        {
            return 0;
        }

        // Get step length based on step mode, peer role and number of antenna paths
        stepLength = CS_GetStepLength(stepHdr.stepMode, role, numAntPath);

        if (stepLength == 0)
        {
            return 0;
        }

        // Read step data to move the segments reader forward
        if (rangingDBClient_getNBytes(&segmentsReader, stepLength, NULL) != SUCCESS)
        {
            return 0;
        }

        // Add the size of the step header and step data length to the total length
        totalLength += sizeof(stepHdr) + stepLength;
    }

    return totalLength;
}

/*********************************************************************
 * @fn      rangingDBClient_getSubeventData
 *
 * @brief   This function retrieves the subevent data from the procedure
 *          segments reader.
 *
 * input parameters
 *
 * @param   segmentsReader - Pointer to the procedure segments reader structure.
 * @param   numStepsReported - number of steps reported in the subevent.
 * @param   numAntPath - number of antenna paths used in the ranging procedure.
 * @param   role - role of the device in the ranging procedure.
 *
 * output parameters
 *
 * @param   subeventDataOut - Pointer to store the retrieved subevent data.
 *                            Memory is allocated within the function and should
 *                            be freed by the caller. If NULL, the function will
 *                            return @ref bleMemAllocError
 *
 * @return  SUCCESS - if the subevent data was successfully retrieved.
 *          FAILURE - if there was an error during parsing the data or invalid parameters.
 *          bleMemAllocError - if memory allocation failed.
 */
static uint8_t rangingDBClient_getSubeventData(RangingDBClient_procedureSegmentsReader_t* segmentsReader, uint8_t numStepsReported,
                                               uint8_t numAntPath, uint8_t role, uint8_t** subeventDataOut)
{
    uint8_t status = SUCCESS;
    uint8_t* subeventData = NULL;
    uint16_t subeventSize = 0;

    if (segmentsReader == NULL || subeventDataOut == NULL)
    {
        status = FAILURE;
    }

    if (status == SUCCESS)
    {
        // Get the subevent data size
        subeventSize = rangingDBClient_getSubeventDataSize(*segmentsReader, numStepsReported, numAntPath, role);

        if (subeventSize == 0)
        {
            status = FAILURE;
        }
    }

    if (status == SUCCESS)
    {
        // Allocate memory for the subevent data
        subeventData = (uint8_t*) ICall_malloc(subeventSize);

        if (subeventData == NULL)
        {
            status = bleMemAllocError;
        }
    }

    if (status == SUCCESS)
    {
        // Get the subevent data
        status = rangingDBClient_getNBytes(segmentsReader, subeventSize, subeventData);
    }

    if (status != SUCCESS)
    {
        // Free allocated memory in case of failure
        if (subeventData != NULL)
        {
            ICall_free(subeventData);
        }
    }
    else
    {
        if (subeventDataOut != NULL)
        {
            // Output the results
            *subeventDataOut = subeventData;
        }
    }

    return status;
}

#endif // RANGING_CLIENT
