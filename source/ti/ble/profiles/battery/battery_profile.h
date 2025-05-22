/******************************************************************************

 @file  battery_profile.h

 @brief This file contains the battery profile definitions and prototypes.

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
#ifndef BAPROFILE_H
#define BAPROFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <ti/ble/services/battery/battery_server.h>

/*********************************************************************
 * DEFINES
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * Application Callback
 */
//Callback to indicate client characteristic configuration has been updated for Battery Level characteristic
typedef void ( *BAP_onBattLevelCCCUpdate_t )( uint16_t connHandle, uint16_t pValue );

typedef struct
{
    BAP_onBattLevelCCCUpdate_t pfnLevelOnCccUpdateCB;   // Called when client characteristic configuration has been updated on Battery Level characteristic
} BAP_cb_t;

/*********************************************************************
 * API FUNCTIONS
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
bStatus_t BAP_start( BAP_cb_t *appCallbacks );

/*********************************************************************
 * @fn      BAP_setBattLevel
 *
 * @brief   Send Battery Level to any registered device
 *
 * @param   level - a Battery Level value.
 * @return  SUCCESS or stack call status
 */
bStatus_t BAP_setBattLevel( uint8_t level );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* BAPROFILE_H */
