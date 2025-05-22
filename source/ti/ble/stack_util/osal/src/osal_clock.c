/******************************************************************************

 @file  osal_clock.c

 @brief OSAL Clock definition and manipulation functions.

 Group: WCS, LPC, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2008-2025, Texas Instruments Incorporated
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

#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/stack_util/osal/osal.h"
#include "ti/ble/stack_util/osal/osal_clock.h"

/*********************************************************************
 * MACROS
 */

#define	YearLength(yr)	((uint16)(IsLeapYear(yr) ? 366 : 365))

/*********************************************************************
 * CONSTANTS
 */

#define	BEGYEAR  2000     //  UTC started at 00:00:00 January 1, 2000

#define	DAY      86400UL  // 24 hours * 60 minutes * 60 seconds

/* Check Below for an explanation */
#define COUNTER_TICK320US 204775UL

/* converted COUNTER_TICK320US from 320us ticks to ms */
#define COUNTER_ELAPSEDMS 65528UL

/*
 * Each tick is 320us so a value greater than 3 implies
 * that at least one millisecond has elapsed 320us*4 > 1 ms
 */
#define TIMER_CLOCK_UPDATE 4

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

  #define CONVERT_MS_TO_S_ELAPSED_REMAINDER( x, y, z ) st(     \
    y += x / 1000;                                             \
    z = x % 1000;                                              \
  )

/*********************************************************************
 * LOCAL VARIABLES
 */

static uint32 timeMSec = 0;

// number of seconds since 0 hrs, 0 minutes, 0 seconds, on the
// 1st of January 2000 UTC
UTCTime OSAL_timeSeconds = 0;

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */
static uint8 monthLength( uint8 lpyr, uint8 mon );

static void osalClockUpdate( uint32 elapsedMSec );

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osalTimeUpdate
 *
 * @brief   Uses the free running rollover count of the MAC backoff timer;
 *          this timer runs freely with a constant 320 usec interval.  The
 *          count of 320-usec ticks is converted to msecs and used to update
 *          the OSAL clock and Timers by invoking osalClockUpdate() and
 *          osalTimerUpdate().  This function is intended to be invoked
 *          from the background, not interrupt level.
 *
 * @param   None.
 *
 * @return  None.
 */
void osalTimeUpdate( void )
{
}

/*********************************************************************
 * @fn      osalClockUpdate
 *
 * @brief   Updates the OSAL Clock time with elapsed milliseconds.
 *
 * @param   elapsedMSec - elapsed milliseconds
 *
 * @return  none
 */
static void osalClockUpdate( uint32 elapsedMSec )
{
  uint32 tmp;
  halIntState_t intState;

  HAL_ENTER_CRITICAL_SECTION(intState);
  // Add elapsed milliseconds to the saved millisecond portion of time
  timeMSec += elapsedMSec;

  // Roll up milliseconds to the number of seconds
  if ( timeMSec >= 1000 )
  {
    tmp = timeMSec;
    CONVERT_MS_TO_S_ELAPSED_REMAINDER(tmp, OSAL_timeSeconds, timeMSec);
  }
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osalAdjustTimer
 *
 * @brief   Updates the OSAL Clock and Timer with elapsed milliseconds.
 *
 * @param   MSec - elapsed milliseconds
 *
 * @return  none
 */
void osalAdjustTimer(uint32 Msec )
{
  /* Disable SysTick interrupts */

  osalClockUpdate(Msec);
  osalTimerUpdate(Msec);
}

/*********************************************************************
 * @fn      osal_setClock
 *
 * @brief   Set the new time.  This will only set the seconds portion
 *          of time and doesn't change the factional second counter.
 *
 * @param   newTime - number of seconds since 0 hrs, 0 minutes,
 *                    0 seconds, on the 1st of January 2000 UTC
 *
 * @return  none
 */
void osal_setClock( UTCTime newTime )
{
  HAL_CRITICAL_STATEMENT(OSAL_timeSeconds = newTime);
}

/*********************************************************************
 * @fn      osal_getClock
 *
 * @brief   Gets the current time.  This will only return the seconds
 *          portion of time and doesn't include the factional second
 *          counter.
 *
 * @param   none
 *
 * @return  number of seconds since 0 hrs, 0 minutes, 0 seconds,
 *          on the 1st of January 2000 UTC
 */
UTCTime osal_getClock( void )
{
  return ( OSAL_timeSeconds );
}

/*********************************************************************
 * @fn      osal_ConvertUTCTime
 *
 * @brief   Converts UTCTime to UTCTimeStruct
 *
 * @param   tm - pointer to breakdown struct
 *
 * @param   secTime - number of seconds since 0 hrs, 0 minutes,
 *          0 seconds, on the 1st of January 2000 UTC
 *
 * @return  none
 */
void osal_ConvertUTCTime( UTCTimeStruct *tm, UTCTime secTime )
{
  // calculate the time less than a day - hours, minutes, seconds
  {
    uint32 day = secTime % DAY;
    tm->seconds = day % 60UL;
    tm->minutes = (day % 3600UL) / 60UL;
    tm->hour = day / 3600UL;
  }

  // Fill in the calendar - day, month, year
  {
    uint16 numDays = secTime / DAY;
    tm->year = BEGYEAR;
    while ( numDays >= YearLength( tm->year ) )
    {
      numDays -= YearLength( tm->year );
      tm->year++;
    }

    tm->month = 0;
    while ( numDays >= monthLength( IsLeapYear( tm->year ), tm->month ) )
    {
      numDays -= monthLength( IsLeapYear( tm->year ), tm->month );
      tm->month++;
    }

    tm->day = numDays;
  }
}

/*********************************************************************
 * @fn      monthLength
 *
 * @param   lpyr - 1 for leap year, 0 if not
 *
 * @param   mon - 0 - 11 (jan - dec)
 *
 * @return  number of days in specified month
 */
static uint8 monthLength( uint8 lpyr, uint8 mon )
{
  uint8 days = 31;

	if ( mon == 1 ) // feb
  {
		days = ( 28 + lpyr );
  }
  else
  {
    if ( mon > 6 ) // aug-dec
    {
      mon--;
    }

    if ( mon & 1 )
    {
      days = 30;
    }
  }

	return ( days );
}

/*********************************************************************
 * @fn      osal_ConvertUTCSecs
 *
 * @brief   Converts a UTCTimeStruct to UTCTime
 *
 * @param   tm - pointer to provided struct
 *
 * @return  number of seconds since 00:00:00 on 01/01/2000 (UTC)
 */
UTCTime osal_ConvertUTCSecs( UTCTimeStruct *tm )
{
  uint32 seconds;

  /* Seconds for the partial day */
  seconds = (((tm->hour * 60UL) + tm->minutes) * 60UL) + tm->seconds;

  /* Account for previous complete days */
  {
    /* Start with complete days in current month */
    uint16 days = tm->day;

    /* Next, complete months in current year */
    {
      int8 month = tm->month;
      while ( --month >= 0 )
      {
        days += monthLength( IsLeapYear( tm->year ), month );
      }
    }

    /* Next, complete years before current year */
    {
      uint16 year = tm->year;
      while ( --year >= BEGYEAR )
      {
        days += YearLength( year );
      }
    }

    /* Add total seconds before partial day */
    seconds += (days * DAY);
  }

  return ( seconds );
}
