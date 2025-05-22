/*****************************************************************************

 @file  battery_server.h

 @brief This file contains the Battery service definitions and prototypes.

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
#ifndef BASSERVER_H
#define BASSERVER_H

#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************
 * INCLUDES
 */

#include "ti/ble/app_util/framework/bleapputil_api.h"

/*********************************************************************
 * DEFINES
 */
//service UUID
#define BAS_SERV_UUID             0x180F

/************************************
 ***** Characteristic defines *******
 ************************************/
// BAS Level
#define BAS_LEVEL_ID              0
#define BAS_LEVEL_UUID            0x2A19
#define BAS_LEVEL_CHAR_LEN        1

/************************************
 ******* BAS Level ************
 ************************************/

//Battery Level struct
typedef struct
{
    uint8_t level;
} BAS_level_t;

#define BAS_LEVEL_MAX_LEN         sizeof( BAS_level_t )  // Maximum length of Battery Level
#define BAS_LEVEL_MIN_LEN         1                      // Minimum length of Battery Level
#define BAS_LEVEL_MAX             100                    // Maximum Battery Level value
#define BAS_LEVEL_MIN             0                      // Minimum Battery Level value

/************************************
 *********** General ****************
 ************************************/
// Data structure used to store CCC update
typedef struct
{
    uint16_t connHandle;
    uint8_t value;
} BAS_cccUpdate_t;

/*********************************************************************
 * Profile Callbacks
 */

// Callback to indicate client characteristic configuration has been updated on Battery Level characteristic
typedef void ( *BAS_levelOnCCCUpdate_t )( char *cccUpdate );

typedef struct
{
    BAS_levelOnCCCUpdate_t sfnLevelOnCCCUpdateCB; // Called when client characteristic configuration has been updated on Battery Level characteristic
} BAS_cb_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      BAS_setParameter
 *
 * @brief   Set a BAS Service parameter.
 *
 * @param   param - Profile parameter ID
 * @param   pValue - pointer to data to write. This is dependent on
 *          the parameter ID will be cast to the appropriate
 *          data type (example: data type of uint16_t will be cast to
 *          uint16_t pointer).
 * @param   len - length of data to write
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t BAS_setParameter( uint8_t param, uint8_t len, void *value );

/*********************************************************************
 * @fn      BAS_getParameter
 *
 * @brief   Get a BAS parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and will be cast to the appropriate
 *          data type (example: data type of uint16_t will be cast to
 *          uint16_t pointer).
 *
 * @return  SUCCESS or error status
 */
bStatus_t BAS_getParameter( uint8_t param, void *pValue );

/*********************************************************************
 * @fn      BAS_addService
 *
 * @brief   This function initializes the Battery Service Server
 *          by registering GATT attributes with the GATT server.
 */
bStatus_t BAS_addService( void );

/*********************************************************************
 * @fn      BAS_registerProfileCBs
 *
 * @brief   Registers the profile callback function. Only call
 *          this function once.
 *
 * @param   profileCallback - pointer to profile callback.
 *
 * @return  SUCCESS or INVALID PARAMETER
 */
bStatus_t BAS_registerProfileCBs( BAS_cb_t *profileCallback );

/*********************************************************************
 *********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* BASSERVER_H */
