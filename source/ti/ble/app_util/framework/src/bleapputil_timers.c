/******************************************************************************

@file  bleapputil_timers.c

@brief This file contains the implementation of the BLE App Util timers APIs.
       The timers are implemented using the DPL ClockP APIs.

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
#include <stdlib.h>
#include <ti/drivers/dpl/ClockP.h>
#include "ti/ble/app_util/framework/bleapputil_timers.h"
#include "ti/ble/app_util/framework/bleapputil_api.h"
#include "ti/ble/app_util/framework/bleapputil_internal.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/

typedef struct
{
    BLEAppUtil_timerHandle handle;
    BLEAppUtil_timerCB_t timerCB;
    ClockP_Handle clockHandle;
    uint32_t timeInMS;
    bool isPeriodic;
    void * pData;
}BLEAppUtil_timerNode_t;

typedef struct
{
    BLEAppUtil_timerNode_t           *timer;
    struct BLEAppUtil_timersList_t   *next;
} BLEAppUtil_timersList_t;

/*********************************************************************
* GLOBAL VARIABLES
*/

/*********************************************************************
* LOCAL VARIABLES
*/

BLEAppUtil_timersList_t *BLEAppUtilTimersHead = NULL;

/*********************************************************************
* LOCAL FUNCTIONS
*/

// Timers APIs
int32_t bleapputil_createTimer(BLEAppUtil_timerNode_t *pTimer);
int32_t bleapputil_deleteTimer(BLEAppUtil_timerNode_t *pTimer);
int32_t bleapputil_startTimer(BLEAppUtil_timerNode_t *pTimer);
int32_t bleapputil_stopTimer(BLEAppUtil_timerNode_t *pTimer);
int32_t bleapputil_rescheduleTimer(BLEAppUtil_timerNode_t *pTimer);
int32_t bleapputil_convertMsecToTicks(uint32_t timeInMS, uint32_t *timeInticks);

// Timers list APIs
bStatus_t bleapputil_addTimerToList(BLEAppUtil_timerNode_t *timer);
bStatus_t bleapputil_removeTimerFromList(BLEAppUtil_timerNode_t * pTimer);
BLEAppUtil_timerNode_t *bleapputil_getTimerFromList(BLEAppUtil_timerHandle timerHandle);

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * @fn      BLEAppUtil_startTimer
 *
 * @brief   Start a timer instance
 *
 * @param   timerCB     - callback function that will be called when the timer expired
 * @param   timeInMS    - the time to set in the timer
 * @param   isPeriodic  - a flag that indicates if the timer is periodic or not
 * @param   pData       - the data to pass to the timer callback
 *
 * @return  Returns the timer handle
 */
BLEAppUtil_timerHandle BLEAppUtil_startTimer(BLEAppUtil_timerCB_t timerCB, uint32_t timeInMS, bool isPeriodic, void *pData)
{
    bStatus_t status = SUCCESS;
    BLEAppUtil_timerHandle handle = BLEAPPUTIL_TIMER_INVALID_HANDLE;
    BLEAppUtil_timerNode_t *newTimer = NULL;

    if (timeInMS == 0)
    {
        return BLEAPPUTIL_TIMER_INVALID_HANDLE;
    }

    // Allocate a new item
    newTimer = (BLEAppUtil_timerNode_t *)BLEAppUtil_malloc(sizeof(BLEAppUtil_timerNode_t));
    if(newTimer == NULL)
    {
        return BLEAPPUTIL_TIMER_INVALID_HANDLE;
    }

    // generate a random handle
    do
    {
        handle = rand();
        if(bleapputil_getTimerFromList(handle) != NULL)
        {
            handle = BLEAPPUTIL_TIMER_INVALID_HANDLE;
        }
    }
    while (handle == BLEAPPUTIL_TIMER_INVALID_HANDLE);

    // Save the timer parameters
    newTimer->handle        = handle;
    newTimer->timerCB       = timerCB;
    newTimer->timeInMS      = timeInMS;
    newTimer->isPeriodic    = isPeriodic;
    newTimer->pData         = pData;

    // Add the timer to the list
    status = bleapputil_addTimerToList(newTimer);
    if(status != SUCCESS)
    {
        BLEAppUtil_free(newTimer);
        newTimer = NULL;
        return BLEAPPUTIL_TIMER_INVALID_HANDLE;
    }
    // Start the timer
    status = bleapputil_createTimer(newTimer);
    if(status != SUCCESS)
    {
        // Remove the timer from the linked list
        bleapputil_removeTimerFromList(newTimer);
        return BLEAPPUTIL_TIMER_INVALID_HANDLE;
    }

    return handle;
}

/*********************************************************************
 * @fn      BLEAppUtil_abortTimer
 *
 * @brief   Abort a timer instance
 *
 * @param   timerHandle - the handle of the timer to abort
 *
 * @return  None
 */
int32_t BLEAppUtil_abortTimer(BLEAppUtil_timerHandle timerHandle)
{
    BLEAppUtil_timerNode_t *pTimer = bleapputil_getTimerFromList(timerHandle);

    if(pTimer)
    {
        // Stop the timer
        bleapputil_deleteTimer(pTimer);
        // Call the timer callback registered by the application
        // to indicate that the timer was aborted
        pTimer->timerCB(pTimer->handle, BLEAPPUTIL_TIMER_ABORTED, pTimer->pData);
        // Remove the timer from the linked list
        bleapputil_removeTimerFromList(pTimer);

        return SUCCESS;
    }

    // TODO: Add an error code for invalid handle
    return FAILURE;
}

/*********************************************************************
 * @fn      BLEAppUtil_reschedulePeriodicTimer
 *
 * @brief   Restart a timer instance
 *
 * @param   timerHandle - the handle of the timer to reschedule
 * @param   timeInMS    - the new time
 * @param   pData       - the data to pass to the timer callback
 *
 * @return  None
 */
int32_t BLEAppUtil_reschedulePeriodicTimer(BLEAppUtil_timerHandle timerHandle, uint32_t timeInMS, void *pData)
{
    int32_t status = FAILURE;
    BLEAppUtil_timerNode_t *pTimer = bleapputil_getTimerFromList(timerHandle);

    if (timeInMS == 0 || pTimer == NULL || !pTimer->isPeriodic)
    {
        return status;
    }

    if(pTimer)
    {
        // Update the timer parameters
        pTimer->timeInMS   = timeInMS;
        pTimer->pData       = pData;

        // Update the new time value and start the timer
        status = bleapputil_rescheduleTimer(pTimer);

        return status;
    }

    return status;
}

/*********************************************************************
* LOCAL FUNCTIONS
*/

/*********************************************************************
 * @fn      bleapputil_addTimerToList
 *
 * @brief   Adds a timer to the linked list
 *
 * @param   timer - pointer of the timer to add
 *
 * @return  SUCCESS, FAILURE
 */
bStatus_t bleapputil_addTimerToList(BLEAppUtil_timerNode_t *timer)
{
    BLEAppUtil_timersList_t *newTimer;

    // Allocate the new handler space
    newTimer = (BLEAppUtil_timersList_t *)BLEAppUtil_malloc(sizeof(BLEAppUtil_timersList_t));

    // If the allocation failed, return an error
    if(newTimer == NULL)
    {
        return FAILURE;
    }

    // Set the parameters of the new item
    newTimer->timer = timer;
    newTimer->next = NULL;

    // The head is NULL
    if(BLEAppUtilTimersHead == NULL)
    {
        BLEAppUtilTimersHead = newTimer;
    }
    else
    {
        // Add the new timer to the head
        newTimer->next = (struct BLEAppUtil_timersList_t *)BLEAppUtilTimersHead;
        BLEAppUtilTimersHead = newTimer;
    }

    return SUCCESS;
}

/*********************************************************************
 * @fn      bleapputil_removeTimerFromList
 *
 * @brief   Removes a timer from the linked list
 *
 * @param   pTimer - pointer of the timer to remove
 *
 * @return  SUCCESS, INVALIDPARAMETER
 */
bStatus_t bleapputil_removeTimerFromList(BLEAppUtil_timerNode_t * pTimer)
{
    BLEAppUtil_timersList_t *curr = BLEAppUtilTimersHead;
    BLEAppUtil_timersList_t *prev = NULL;
    bStatus_t status = INVALIDPARAMETER;

    // Go over the timers list
    while(curr != NULL)
    {
        // The item containing the handle to remove is found
        if(curr->timer == pTimer)
        {
            // The item is the head
            if(prev == NULL)
            {
                // Change the head to point the next item in the list
                BLEAppUtilTimersHead = (BLEAppUtil_timersList_t *)curr->next;
            }
            // The item is not the head
            else
            {
                prev->next = curr->next;
            }

            // Free the timer + item
            BLEAppUtil_free(pTimer);
            BLEAppUtil_free(curr);
            pTimer = NULL;
            curr = NULL;

            // Set the status to SUCCESS
            status = SUCCESS;
            break;
        }

        // Update the prev item to point the current item
        prev = curr;
        // Update the current item to point the next item
        curr = (BLEAppUtil_timersList_t *)prev->next;
    }

    return status;
}

/*********************************************************************
 * @fn      bleapputil_getTimerFromList
 *
 * @brief   Return the timer associated to the handle
 *
 * @param   timerHandle - The handle of the timer to return
 *
 * @return  A pointer to the timer data
 */
BLEAppUtil_timerNode_t *bleapputil_getTimerFromList(BLEAppUtil_timerHandle timerHandle)
{
    // Get the timer associated with the handle
    BLEAppUtil_timersList_t * listItem = BLEAppUtilTimersHead;
    while(listItem)
    {
        if(listItem->timer->handle == timerHandle)
        {
            return listItem->timer;
        }
        listItem = (BLEAppUtil_timersList_t *)listItem->next;
    }
    return NULL;
}

/*********************************************************************
 * @fn      bleapputil_localTimerCB
 *
 * @brief   A local timer that will be called from the BLE App Util
 *          context and calls the timer callback registered by the
 *          application
 *
 * @param   timerHandle - The handle of the timer to return
 *
 * @return  None
 */
static void bleapputil_localTimerCB(char *pData)
{
    BLEAppUtil_timerNode_t *pTimer = NULL;

    if (pData != NULL)
    {
        memcpy(&pTimer, pData, sizeof(uint32_t));

        // Call the timer callback registered by the application
        pTimer->timerCB(pTimer->handle, BLEAPPUTIL_TIMER_TIMEOUT, pTimer->pData);

        // If the timer is not periodic remove it from the list
        if(!pTimer->isPeriodic)
        {
            // Stop the timer
            bleapputil_deleteTimer(pTimer);
            // Remove the timer from the linked list
            bleapputil_removeTimerFromList(pTimer);
        }
    }
}

/*********************************************************************
 * @fn      bleapputil_TimerFunc
 *
 * @brief   Timer event handler function
 *          This function is used to implement the wake up scheduler
 *
 * @param   arg - argument passed to the timer
 *
 * @return  None
 */
static void bleapputil_TimerFunc(uintptr_t arg)
{
    BLEAppUtil_timerNode_t *pTimer = (BLEAppUtil_timerNode_t *) (arg);
    char * pData = NULL;

    // Allocate the space and copy the data to pass in the callback
    pData = (char *)BLEAppUtil_malloc(sizeof(uint32_t));
    if (pData != NULL)
    {
      memcpy(pData, &pTimer, sizeof(uint32_t));

      // Move the local callback to BLE App Util context
      BLEAppUtil_invokeFunction(bleapputil_localTimerCB, pData);
    }
}

/*********************************************************************
 * @fn      bleapputil_createTimer
 *
 * @brief   Initialize a Timer instance
 *
 * @param   pTimer - pointer of the timer to create an instance for
 *
 * @return  None
 */
int32_t bleapputil_createTimer(BLEAppUtil_timerNode_t *pTimer)
{
    int32_t status = SUCCESS;
    uint32_t timeInTicks = 0;

    // Timer params
    ClockP_Params params;

    // Initialize the clock params
    ClockP_Params_init(&params);

    // Set the clock params
    params.period = pTimer->isPeriodic;
    params.startFlag = false;
    params.arg = (uintptr_t)pTimer;

    // Convert the time in ms to ticks
    status = bleapputil_convertMsecToTicks(pTimer->timeInMS, &timeInTicks);
    if (status == SUCCESS)
    {
        // Create the clock
        pTimer->clockHandle = ClockP_create(bleapputil_TimerFunc, timeInTicks, &params);

        // Start the timer
        status = bleapputil_startTimer(pTimer);
    }

    return status;
}

/*********************************************************************
 * @fn      bleapputil_deleteTimer
 *
 * @brief   Delete a Timer instance
 *
 * @param   pTimer - pointer of the timer to delete
 *
 * @return  None
 */
int32_t bleapputil_deleteTimer(BLEAppUtil_timerNode_t *pTimer)
{
    int32_t status = SUCCESS;

    status = bleapputil_stopTimer(pTimer);
    if (status == SUCCESS)
    {
        // Delete the clock
        ClockP_delete(pTimer->clockHandle);
    }

    return status;
}

/*********************************************************************
 * @fn      bleapputil_startTimer
 *
 * @brief   Start a Timer instance
 *
 * @param   pTimer - pointer of the timer to start
 *
 * @return  None
 */
int32_t bleapputil_startTimer(BLEAppUtil_timerNode_t *pTimer)
{
    int32_t status = SUCCESS;

    // Start the clock
    ClockP_start(pTimer->clockHandle);

    return status;
}

/*********************************************************************
 * @fn      bleapputil_stopTimer
 *
 * @brief   Stop a Timer instance
 *
 * @param   pTimer - pointer of the timer to stop
 *
 * @return  None
 */
int32_t bleapputil_stopTimer(BLEAppUtil_timerNode_t *pTimer)
{
    int32_t status = SUCCESS;

    // Check if the clock is active
    bool isActive = ClockP_isActive(pTimer->clockHandle);
    if (isActive == true)
    {
        // Stop the clock
        ClockP_stop(pTimer->clockHandle);
    }

    return status;
}

/*********************************************************************
 * @fn      bleapputil_rescheduleTimer
 *
 * @brief   Reschedule a Timer instance
 *
 * @param   pTimer - pointer of the timer to reschedule
 *
 * @return  None
 */
int32_t bleapputil_rescheduleTimer(BLEAppUtil_timerNode_t *pTimer)
{
    int32_t status = SUCCESS;
    uint32_t timeInTicks = 0;

    // Stop the timer
    status = bleapputil_stopTimer(pTimer);
    if (status == SUCCESS)
    {
        // Convert the time in ms to ticks
        status = bleapputil_convertMsecToTicks(pTimer->timeInMS, &timeInTicks);
        if (status == SUCCESS)
        {
            // Set the new timeout
            ClockP_setTimeout(pTimer->clockHandle, timeInTicks);

            // Start the timer
            status = bleapputil_startTimer(pTimer);
        }
    }

    return status;
}

/*********************************************************************
 * @fn      bleapputil_convertMsecToTicks
 *
 * @brief   Convert milliseconds to ticks
 *
 * @param   timeInMS - the time in ms to convert
 * @output  timeInticks - pointer to the converted time in ticks
 *
 * @return  SUCCESS, FAILURE
 */
int32_t bleapputil_convertMsecToTicks(uint32_t timeInMS, uint32_t *timeInticks)
{
    int32_t status = SUCCESS;

    if (timeInticks == NULL)
    {
        status = FAILURE;
    }
    else
    {
        // Convert the time in ms to ticks
        *timeInticks = (timeInMS * 1000) / ClockP_getSystemTickPeriod();
    }

    return status;
}

/*********************************************************************
*********************************************************************/
