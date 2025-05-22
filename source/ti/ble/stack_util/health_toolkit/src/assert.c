/******************************************************************************

 @file  assert.c

  @brief This file contains the Hardware Abstraction Layer (HAL) Assert APIs
         used to handle asserts in system software. The assert handler, and
         its behavior, depend on the build time define (please see header file).

 Group: WCS, LPC, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2006-2025, Texas Instruments Incorporated
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

/*******************************************************************************
 * INCLUDES
 */

#include "ti/ble/stack_util/health_toolkit/assert.h"
#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/stack_util/comdef.h"
#include <ti/drivers/Power.h>

/*******************************************************************************
 * MACROS
 */

// Compile Time Assertions - Integrity check of type sizes.
HAL_ASSERT_SIZE(  int8, 1);
HAL_ASSERT_SIZE( uint8, 1);
HAL_ASSERT_SIZE( int16, 2);
HAL_ASSERT_SIZE(uint16, 2);
HAL_ASSERT_SIZE( int32, 4);
HAL_ASSERT_SIZE(uint32, 4);

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

// HAL Assert callback pointer
assertCback_t halAssertCback = (assertCback_t)halAssertSpinlock;

// optional general purpose subcause - the value dpepends on assert cause.
uint8 assertSubcause = HAL_ASSERT_SUBCAUSE_NONE;

// used to decide how FALSE is handled (see halAssertHandlerExt)
uint8 legacyMode = HAL_ASSERT_LEGACY_MODE_ENABLED;


/*******************************************************************************
 * @fn          halAssertInit API
 *
 * @brief       This function is used by the Stack code to override the build-
 *              time initialization values of assert Callback and Legacy Mode,
 *              thus allowing a project to change the defaults without having
 *              to change the common (shared) header file assert.h. The
 *              default values are: NULL and HAL_ASSERT_LEGACY_MODE_ENABLED.
 *
 * input parameters
 *
 * @param       initAssertCback: Pointer to HAL Assert callback.
 * @param       initLegacyMode:  HAL_ASSERT_LEGACY_MODE_ENABLED |
 *                               HAL_ASSERT_LEGACY_MODE_DISABLED
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void halAssertInit( assertCback_t initAssertCback, uint8 initLegacyMode )
{
  // check if the callback pointer is invalid
  if ( initAssertCback == NULL )
  {
    // set the assert callback pointer to a valid handler
    halAssertCback = (assertCback_t)halAssertSpinlock;
  }
  else // callback pointer is valid
  {
    // set the assert callback pointer
    halAssertCback = initAssertCback;
  }

  // set the legacyMode
  legacyMode = initLegacyMode;

  return;
}


/*******************************************************************************
 * @fn          halAssertHandler API
 *
 * @brief       This function is to trap software execution. The assert action
 *              depends on the defines:
 *
 *              HAL_ASSERT_RESET  - Reset the device.
 *              HAL_ASSERT_SPIN   - Spinlock.
 *              Otherwise:        - Just return.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void halAssertHandler( void )
{
#if defined( HAL_ASSERT_RESET )
  Power_reset();
#elif defined( HAL_ASSERT_SPIN )
  halAssertSpinlock();
#endif

  return;
}


/*******************************************************************************
 * @fn          halAssertHandlerExt API
 *
 * @brief       This function is the Extended halAssertHandler, and is backward
 *              compatible with the original halAssertHandler. The assert
 *              causes can be TRUE and FALSE, per usual, or a value from
 *              1..0xFF (see header file for possible assert cause values).
 *              When FALSE, the legacy mode (a global defined as part of the
 *              call to HAL_ASSERT_Init) will determine whether the original
 *              halAssertHandler call is used or the registered callback (if
 *              any) is used.
 *
 * input parameters
 *
 * @param       assertCause: TRUE, FALSE, 1..0xFF.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void halAssertHandlerExt( uint8 assertCause )
{
  // check if there is no cause
  if ( (assertCause == FALSE) &&
       (legacyMode == HAL_ASSERT_LEGACY_MODE_ENABLED) )
  {
    halAssertHandler();

    return;
  }

  // evoke registered callback
  // Note: The pointer halAssertCback will never be NULL due to defaults.
  // Note: The value of assertSubcause, if used, should be set before the call
  //       to HAL_ASSERT. For example:
  // HAL_ASSERT_SET_SUBCAUSE( HAL_ASSERT_SUBCAUSE_FW_INERNAL_ERROR );
  // HAL_ASSERT( HAL_ASSERT_CAUSE_INTERNAL_ERROR );
  halAssertCback( assertCause, assertSubcause );

  // call base HAL Assert handler in case callback handler didn't trap
  halAssertHandler();

  return;
}

/*******************************************************************************
 * @fn          halAssertSpinlock API
 *
 * @brief       This function is to trap software execution.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void halAssertSpinlock( void )
{
#ifdef HAL_ASSERT_SPIN
  volatile uint8 i = 1;

  HAL_DISABLE_INTERRUPTS();
  while(i);
  HAL_ENABLE_INTERRUPTS();
#endif // HAL_ASSERT_SPIN

  return;
}

/*******************************************************************************
*/
