/******************************************************************************

 @file  osal_pwrmgr.c

 @brief This file contains the OSAL Power Management API.

 Group: WCS, LPC, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2004-2025, Texas Instruments Incorporated
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
#include "ti/ble/stack_util/osal/osal_tasks.h"
#include "ti/ble/stack_util/osal/osal_timers.h"
#include "ti/ble/stack_util/osal/osal_pwrmgr.h"

#include "ti/ble/stack_util/icall/app/icall.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/* This global variable stores the power management attributes.
 */
pwrmgr_attribute_t pwrmgr_attribute;
uint8 pwrmgr_initialized = FALSE;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osal_pwrmgr_init
 *
 * @brief   Initialize the power management system.
 *
 * @param   none.
 *
 * @return  none.
 */
void osal_pwrmgr_init( void )
{
  pwrmgr_attribute.pwrmgr_task_state = 0;            // Cleared.  All set to conserve
  pwrmgr_initialized = TRUE;
}

/*********************************************************************
 * @fn      osal_pwrmgr_task_state
 *
 * @brief   This function is called by each task to state whether or
 *          not this task wants to conserve power.
 *
 * @param   task_id - calling task ID.
 *          state - whether the calling task wants to
 *          conserve power or not.
 *
 * @return  TRUE if power is required; FALSE is power is not required
 */
uint8 osal_pwrmgr_task_state( uint8 task_id, uint8 state )
{
  halIntState_t intState;
  bool pwrRequired = TRUE;

  if ( task_id >= tasksCnt )
    return ( pwrRequired );

  if ( !pwrmgr_initialized )
  {
    /* If voting is made before this module is initialized,
     * pwrmgr_task_state will reset later when the module is
     * initialized, and cause incorrect activity count.
     */
    return ( pwrRequired );
  }

  HAL_ENTER_CRITICAL_SECTION( intState );

  if ( state == PWRMGR_CONSERVE )
  {
    uint16 cache = pwrmgr_attribute.pwrmgr_task_state;
    // Clear the task state flag
    pwrmgr_attribute.pwrmgr_task_state &= ~(1 << task_id );
    if (cache != 0 && pwrmgr_attribute.pwrmgr_task_state == 0)
    {
      /* Decrement activity counter */
      pwrRequired = ICall_pwrUpdActivityCounter(FALSE);
    }
  }
  else
  {
    if (pwrmgr_attribute.pwrmgr_task_state == 0)
    {
      /* Increment activity counter */
      (void)ICall_pwrUpdActivityCounter(TRUE);

    }
    // Set the task state flag
    pwrmgr_attribute.pwrmgr_task_state |= (1 << task_id);
  }

  HAL_EXIT_CRITICAL_SECTION( intState );

  return ( pwrRequired );
}

/*********************************************************************
*********************************************************************/
