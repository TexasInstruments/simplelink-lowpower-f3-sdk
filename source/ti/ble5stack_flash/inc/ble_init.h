/******************************************************************************

 Group: WCS, LPC, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024, Texas Instruments Incorporated
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
/******************************************************************************

 @file  ble_init.h

 @brief BLE initialization interface APIs

 *****************************************************************************/

#ifndef BLE_INIT_H
#define BLE_INIT_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */
#include "hal_types.h"
#include "hal_assert.h"
#include "hci_api.h"

/********************************************************************************
* DEFINES
* */
#define BLE_INIT_NO_WAIT           0
#define BLE_INIT_WAIT_FOREVER      ~(0)

/*********************************************************************
 * TYPEDEFS
 */
typedef struct bleServicesParams
{
  uint32 bleServices;                    // Bitwise flags for services initialization.
  uint32 syncInitTimeoutTicks;           // Timeout in Ticks for the API to wait for the initialization
                                         // sequence to complete.
                                         // BLE_INIT_NO_WAIT - the function will return immediately.
                                         // BLE_INIT_WAIT_FOREVER - the function will return only after the initialization is completed
                                         // Other values will determine a specific timeout for synchronous initialization sequence.
  hciControllerToHostCallbacks_t hciCbs; // Callbacks for the HCI
  assertCback_t assertCallback;          // Callback for user defined assert handling
} bleServicesParams_t;

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */


/********************************************************************************
 * @fn            BLE_ServicesInit
 *
 * @brief         The `BLE_ServicesInit` function is responsible for initializing
 *                the BLE services based on the input parameters specified
 *                in `pServiceParams`. This function offers an option for a synchronous
 *                initialization sequence, meaning that the calling task will
 *                be blocked until the stack indicates that the initialization
 *                is complete.
 *                If the initialization fails, `BLE_ServicesInit()`
 *                will return a failure status.
 *
 * input parameters
 *
 * @param         pServiceParams - pointer to BLE Params initialization options
 *
 * output parameters
 *
 * @return        SUCCESS / FAILURE.
 *
 * */
uint32 BLE_ServicesInit(const bleServicesParams_t *pServiceParams);

/********************************************************************************
 * @fn            BLE_ServicesParamsInit
 *
 * @brief         The `BLE_ServicesParamsInit` function provides a default
 *                configuration for the bleServicesParams_t structure
 *
 * input parameters
 *
 * @param         pServiceParams - pointer to BLE Services initialization options
 * @param         size           - size of the initialization structure
 *
 * output parameters
 *
 * @return        SUCCESS - in case pServiceParams initialization succeed
 *                FAILURE in case of
 *                   - Compatibility/versions mismatch (the check is done based
 *                     on size of the bleServicesParams_t).
 *                   - Parameters validation
 *
 * */
uint32 BLE_ServicesParamsInit(bleServicesParams_t *pServiceParams, size_t size);

/*******************************************************************************
 * @fn          RegisterAssertCback
 *
 * @brief       This routine registers the Application's assert handler.
 *
 * input parameters
 *
 * @param       appAssertHandler - Application's assert handler.
 *
 * output parameters
 *
 * @param       appAssertHandler - callback to handle the assert in
 *                                 application level.
 *
 * @return      None.
 */
void RegisterAssertCback(assertCback_t appAssertHandler);

#ifdef __cplusplus
}
#endif

#endif /* BLE_INIT_H */
