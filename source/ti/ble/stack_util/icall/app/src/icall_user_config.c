/******************************************************************************

 @file  icall_user_config.c

 @brief This file contains generic user configurable variables for icall thread.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2016-2025, Texas Instruments Incorporated
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

#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/stack_util/icall/app/icall_user_config.h"
#include "ti/ble/stack_util/icall/app/icall.h"

#include <ti/drivers/AESCCM.h>
#include <ti/drivers/AESECB.h>

#include <ti/drivers/aesccm/AESCCMLPF3.h>
#include <ti/drivers/aesecb/AESECBLPF3.h>
#include <ti/drivers/ecdh/ECDHLPF3SW.h>
#include <ti/drivers/cryptoutils/sharedresources/CryptoResourceLPF3.h>
#include <ti/drivers/RNG.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyPlaintext.h>

#include <ti/drivers/ECDH.h>
#include <ti/drivers/utils/Random.h>

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */
#ifdef EXT_HAL_ASSERT
  #define ASSERT_CBACK                  &halAssertCback
#else // !EXT_HAL_ASSERT
  #define ASSERT_CBACK                  &appAssertCback
#endif // EXT_HAL_ASSERT

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

// this table needs to be field by the application , so it cannot be store in flash.
applicationService_t bleAppServiceInfoTable =
{
  .timerTickPeriod     = 0,               // timerTick_period, This need to be filled at runtime, or the stack will assert an error.
  .timerMaxMillisecond = 0,               // timerMaxMillisecond. This need to be filled at runtime, or the stack will assert an error.
  .assertCback         = ASSERT_CBACK,
};

/*******************************************************************************
 */
