/******************************************************************************

 @file  tx_power.h

 @brief This file contains the TX Power service definitions and prototypes.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2010-2024, Texas Instruments Incorporated
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

#ifndef TX_POWER_H
#define TX_POWER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Service UUID
#define TPL_SERV_UUID 0x1804

/************************************
 ***** Characteristic defines *******
 ************************************/

// Tx Power Level
#define TPLS_TX_POWER_LEVEL_ID   0
#define TPLS_TX_POWER_LEVEL_UUID 0x2A07

/************************************
 ********** AI Product Data *********
 ************************************/

#define TPLS_TX_POWER_LEVEL_LEN    1

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * Profile Callbacks
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      TPLS_addService
 *
 * @brief   This function initializes the TX Power Level service
 *          by registering GATT attributes with the GATT server.
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t TPLS_addService( void );

/*********************************************************************
 * @fn      TPLS_setParameter
 *
 * @brief   Set an TX Power Level Service parameter.
 *
 * @param   param - Characteristic UUID
 * @param   pValue - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 * @param   len - length of data to write
 *
 * @return  SUCCESS or error status
 */
bStatus_t TPLS_setParameter(uint8 param, void *pValue, uint16 len);

/*********************************************************************
 * @fn      TPLS_getParameter
 *
 * @brief   Get an TX Power Level parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  SUCCESS or error status
 */
bStatus_t TPLS_getParameter( uint8 param, void *pValue );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* TX_POWER_H */
