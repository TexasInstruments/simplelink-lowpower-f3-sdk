/******************************************************************************

@file  bleapputil_timers.h

@brief This file contains the BLE App Util timers APIs.

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

#ifndef BLEAPPUTIL_TIMERS_H
#define BLEAPPUTIL_TIMERS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

#include <stdint.h>
#include <stdbool.h>

/*********************************************************************
 * MACROS
 */

#define TIMER_SEC_TO_MS(sec)        sec*1000
#define TIMER_MINUTES_TO_MS(min)    TIMER_SEC_TO_MS(min*60)
#define TIMER_HOURS_TO_MS(hour)     TIMER_MINUTES_TO_MS(hour*60)
#define TIMER_DAYS_TO_MS(day)       TIMER_HOURS_TO_MS(day*24)

// Invalid Timer Handle value
#define BLEAPPUTIL_TIMER_INVALID_HANDLE        0xFF

/*********************************************************************
 * TYPEDEFS
 */

typedef enum
{
    BLEAPPUTIL_TIMER_TIMEOUT,
    BLEAPPUTIL_TIMER_ABORTED
}BLEAppUtil_timerTermReason_e;

typedef uint8_t BLEAppUtil_timerHandle;

typedef void (*BLEAppUtil_timerCB_t)(BLEAppUtil_timerHandle timerHandle, BLEAppUtil_timerTermReason_e reason, void *pData);

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
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
BLEAppUtil_timerHandle BLEAppUtil_startTimer(BLEAppUtil_timerCB_t timerCB, uint32_t timeInMS, bool isPeriodic, void *pData);

/*********************************************************************
 * @fn      BLEAppUtil_abortTimer
 *
 * @brief   Abort a timer instance
 *
 * @param   timerHandle - the handle of the timer to abort
 *
 * @return  None
 */
int32_t BLEAppUtil_abortTimer(BLEAppUtil_timerHandle timerHandle);

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
int32_t BLEAppUtil_reschedulePeriodicTimer(BLEAppUtil_timerHandle timerHandle, uint32_t time, void *pData);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* BLEAPPUTIL_TIMERS_H */
