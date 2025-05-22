/******************************************************************************

 @file  battery_profile.c

 @brief This file contains the Battery profile sample for use with the Basic BLE
         Profiles sample application.

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
#include <string.h>
#include "ti/ble/stack_util/icall/app/icall.h"
/* This Header file contains all BLE API and icall structure definition */
#include "ti/ble/stack_util/icall/app/icall_ble_api.h"
#include <ti/ble/profiles/battery/battery_profile.h>

/*********************************************************************
 * DEFINES
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************/

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static BAP_cb_t *bap_appCB = NULL;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void BAP_onBattLevelCCCUpdateCB( char *pValue );

/*********************************************************************
 * SERVER CALLBACKS
 */
// Battery profile callback functions
static BAS_cb_t bap_serverCB =
{
 BAP_onBattLevelCCCUpdateCB,
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      BAP_start
 *
 * @brief   This function adds the Battery Server service,
 *          registers the service callback function and initializes
 *          the service characteristics
 *
 * @param   appCallbacks - pointer to application callback
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t BAP_start( BAP_cb_t *appCallbacks )
{
    uint8_t status = SUCCESS;

    // Add Battery service
    status = BAS_addService();

    if ( status == SUCCESS )
    {
        // Register to service callback function
        status = BAS_registerProfileCBs( &bap_serverCB );

        if ( status == SUCCESS )
        {
            // Registers the application callback function
            if ( appCallbacks )
            {
                bap_appCB = appCallbacks;
            }
        }
    }
    // Return status value
    return ( status );
}

/*********************************************************************
 * @fn      BAP_onBattLevelCCCUpdateCB
 *
 * @brief   Callback from BAS Server indicating CCC has been update
 *          for Level characteristic
 *
 * @param   pValue - pointer to data
 *
 * @return  None
 */
static void BAP_onBattLevelCCCUpdateCB( char *pValue )
{
    BAS_cccUpdate_t *cccUpdate = ( BAS_cccUpdate_t * )pValue;

    // Verify input parameters
    if ( NULL != pValue )
    {
        // If the callback function is not NULL, notify the application
        if ( ( NULL != bap_appCB ) && ( NULL != bap_appCB->pfnLevelOnCccUpdateCB ) )
        {
            bap_appCB->pfnLevelOnCccUpdateCB( cccUpdate->connHandle, cccUpdate->value );
        }
    }
    return;
}

/*********************************************************************
 * @fn      BAP_setBattLevel
 *
 * @brief   Create BAP level and send it
 *
 * @param   level - a Battery Level value.

 * @return  SUCCESS or stack call status
 */
bStatus_t BAP_setBattLevel( uint8_t level )
{
    uint8_t status = SUCCESS;

    status = BAS_setParameter( BAS_LEVEL_ID, BAS_LEVEL_MIN_LEN, &level );

    // Return status value
    return ( status );
}

/*********************************************************************
*********************************************************************/
