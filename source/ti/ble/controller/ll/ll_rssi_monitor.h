/******************************************************************************

 @file  ll_rssi_monitor.h

 @brief This file contains the RSSI monitoring functions and APIs in the
        controller layer (LL).

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025, Texas Instruments Incorporated

 All rights reserved not granted herein.
 Limited License.

 Texas Instruments Incorporated grants a world-wide, royalty-free,
 non-exclusive license under copyrights and patents it now or hereafter
 owns or controls to make, have made, use, import, offer to sell and sell
 ("Utilize") this software subject to the terms herein. With respect to the
 foregoing patent license, such license is granted solely to the extent that
 any such patent is necessary to Utilize the software alone. The patent
 license shall not apply to any combinations which include this software,
 other than combinations with devices manufactured by or for TI ("TI
 Devices"). No hardware patent is licensed hereunder.

 Redistributions must preserve existing copyright notices and reproduce
 this license (including the above copyright notice and the disclaimer and
 (if applicable) source code license limitations below) in the documentation
 and/or other materials provided with the distribution.

 Redistribution and use in binary form, without modification, are permitted
 provided that the following conditions are met:

   * No reverse engineering, decompilation, or disassembly of this software
     is permitted with respect to any software provided in binary form.
   * Any redistribution and use are licensed by TI for use only with TI Devices.
   * Nothing shall obligate TI to provide you with source code for the software
     licensed and provided to you in object code.

 If software source code is provided to you, modification and redistribution
 of the source code are permitted provided that the following conditions are
 met:

   * Any redistribution and use of the source code, including any resulting
     derivative works, are licensed by TI for use only with TI Devices.
   * Any redistribution and use of any object code compiled from the source
     code and any resulting derivative works, are licensed by TI for use
     only with TI Devices.

 Neither the name of Texas Instruments Incorporated nor the names of its
 suppliers may be used to endorse or promote products derived from this
 software without specific prior written permission.

 DISCLAIMER.

 THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

#ifndef LL_RSSI_MONITOR_H
#define LL_RSSI_MONITOR_H

#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/controller/ll/ll.h"

/*******************************************************************************
 * CONSTANTS
 */

// Maximum number of callback the RSSI monitor can handle
#ifndef RSSI_MON_MAX_NUM_CLIENTS
#define RSSI_MON_MAX_NUM_CLIENTS        4U
#endif

// The max weight possible, should be change by multiple of 10
// As of now - up to 100 (the variable that holds this define is in size of int8_t)
#ifndef RSSI_MON_CONFIG_MAX_WEIGHT
#define RSSI_MON_CONFIG_MAX_WEIGHT      10
#endif

// Minimum number of samples by default
// As of now - up to 255 (the variable that holds this define is in size of uint8_t)
#define RSSI_MON_CONFIG_MIN_NUM_SAMPLES 10U

// Invalid register callback handle
#define RSSI_MON_INVALID_REG_HANDLE     0x7FU

// Invalid RSSI statistic value
#define RSSI_MON_INVALID_VALUE          LRF_RSSI_INVALID

// Define to handle register for all connections
#define RSSI_MON_ALL_CONNECTIONS        0x7F

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * TYPEDEFS
 */

// typedef for the RSSI monitor callback
typedef uint8_t llRssiMonCbHandle_t;

// RSSI monitor threshold types
typedef enum
{
  RSSI_MONITOR_LOW_THRESHOLD_PASS = 0x1,
  RSSI_MONITOR_HIGH_THRESHOLD_PASS = 0x2,
  RSSI_MONITOR_BACK_TO_RANGE = 0x3
} llRssiMonThresh_e;

/*
* @brief LL RSSI Monitor Callback
*
* @note
* This callback will be triggered when an RSSI threshold is passed for each
* new RSSI statistic calculated.
*
* @param threshPass - RSSI_MONITOR_HIGH_THRESHOLD_PASS if the high threshold passed
*                   - RSSI_MONITOR_LOW_THRESHOLD_PASS if the low threshold passed
* @param connHandle - llRssiMonCbHandle_t
*
* @return None
*/
typedef void (*pfnRssiMonThreshCB_t)(llRssiMonCbHandle_t handle, llRssiMonThresh_e threshPass);

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          LL_RssiMon_Open
 *
 * @brief       Initializes the RSSI Monitor by allocating memory for the global
 *              configuration, the global RSSI connection data, the global callback
 *              data and the global callback connection flag.
 *              It also sets default values for these structures.
 *
 * @Design      BLE_LOKI-2764
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      LL_STATUS_SUCCESS if initialization was successful,
 *              LL_STATUS_ERROR_MEM_CAPACITY_EXCEEDED if memory allocation failed.
 */
llStatus_t LL_RssiMon_Open(void);

/*******************************************************************************
 * @fn          LL_RssiMon_Close
 *
 * @brief       Deinitializes the RSSI Monitor by freeing the allocated memory
 *              for the global configuration, the global RSSI connection data,
 *              the global callback data and the global callback connection flag.
 *
 * @Design      BLE_LOKI-2764
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS
 *
 */
llStatus_t LL_RssiMon_Close(void);

/*******************************************************************************
 * @fn          LL_RssiMon_Reg
 *
 * @brief       Registers for RSSI monitoring for a specific connection with
 *              specified RSSI thresholds and a callback function.
 *
 * @Design      BLE_LOKI-2764
 *
 * input parameters
 *
 * @param       connId         - Connection ID
 * @param       lowRssiThresh  - Lowest RSSI value acceptable by the App/Controller
 * @param       highRssiThresh - Highest RSSI value acceptable by the App/Controller
 * @param       pfnCB          - Callback function pointer
 *
 * output parameters
 *
 * None.
 *
 * @return      llRssiMonCbHandle_t if registration was successful,
 *              RSSI_MON_INVALID_REG_HANDLE if registration failed.
 */
llRssiMonCbHandle_t LL_RssiMon_Reg( uint8_t  connId,
                                    int8_t   lowRssiThresh,
                                    int8_t   highRssiThresh,
                                    pfnRssiMonThreshCB_t pfnCB );

/*******************************************************************************
 * @fn          LL_RssiMon_Unreg
 *
 * @brief       Unregisters for RSSI monitoring for a specific registration handle.
 *
 * @Design      BLE_LOKI-2764
 *
 * input parameters
 *
 * @param       regHandle  - Registration handle
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS if unregistration was successful,
 *              RSSI_MON_INVALID_REG_HANDLE if the registration handle is invalid.
 */
llStatus_t LL_RssiMon_Unreg(llRssiMonCbHandle_t regHandle);

/*******************************************************************************
 * @fn          LL_RssiMon_SetConfig
 *
 * @brief       Sets the RSSI monitor configuration fields per connection that
 *              are used to calculate the RSSI statistic value.
 *
 * @Design      BLE_LOKI-2764
 *
 * input parameters
 *
 * @param       incomeRssiWeight  - Weight of incoming RSSI value
 * @param       minRssiSamples    - Minimum RSSI samples for stability
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS if configuration was set successfully,
 *              INVALID_CONFIG if the configuration is invalid.
 */
llStatus_t LL_RssiMon_SetConfig( int8_t  incomeRssiWeight,
                                 uint8_t minRssiSamples );

/*******************************************************************************
 * @fn          LL_RssiMon_GetConfig
 *
 * @brief       Retrieves the RSSI monitor configuration fields per connection.
 *
 * @Design      BLE_LOKI-2764
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * @param       pIncomeRssiWeight - Pointer to fill the incoming RSSI weight
 * @param       pMinRssiSamples   - Pointer to fill the minimum RSSI samples
 *
 * @return      LL_STATUS_SUCCESS if configuration was retrieved successfully,
 *              INVALID_CONFIG if the configuration is invalid.
 */
llStatus_t LL_RssiMon_GetConfig( int8_t  *pIncomeRssiWeight,
                                 uint8_t *pMinRssiSamples );

/*******************************************************************************
 * @fn          LL_RssiMon_GetRssiStat
 *
 * @brief       Retrieves the current RSSI statistic value per connection.
 *              Returns RSSI statistic value only if there is an open registration
 *              for that connection.
 *
 * @Design      BLE_LOKI-2764
 *
 * input parameters
 *
 * @param       connId - Connection id
 *
 * output parameters
 *
 * @param       pRssiStat - Pointer to fill the current RSSI statistic value
 *
 * @return      LL_STATUS_SUCCESS if the the function was executed as expected.
 *              LL_STAUTS_ERROR_BAD_PARAMETER otherwise
 */
llStatus_t LL_RssiMon_GetRssiStat(uint8_t connId, int8_t *pRssiStat);

/*******************************************************************************
 * @fn          LL_RssiMon_Execute
 *
 * @brief       Calculates the current RSSI statistic value for a specific connection
 *              if there is an open registration for the connection. After calculation,
 *              it checks all registered callback thresholds and calls the callback
 *              function if needed.
 *
 * @Design      BLE_LOKI-2764
 *
 * input parameters
 *
 * @param       connId - Connection id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS if the the function was executed as expected.
 *              LL_STAUTS_ERROR_BAD_PARAMETER otherwise
 *
 */
llStatus_t LL_RssiMon_Execute(uint8_t connId);

/*******************************************************************************
 * @fn          LL_RssiMon_ClearConnData
 *
 * @brief       Clears the global RSSI connection data for a specific connection ID
 *              or for all connections.
 *
 * @Design      BLE_LOKI-2764
 *
 * input parameters
 *
 * @param       connId - Connection id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS if the the function was executed as expected.
 *              LL_STAUTS_ERROR_BAD_PARAMETER otherwise
 */
llStatus_t LL_RssiMon_ClearConnData(uint8_t connId);

/*******************************************************************************
 * @fn          LL_RssiMon_UnregByConnId
 *
 * @brief       Unregisters for RSSI monitoring for a specific connection ID.
 *
 * @Design      BLE_LOKI-2764
 *
 * input parameters
 *
 * @param       connId - Connection id
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS if unregistration was successful,
 *              LL_STATUS_ERROR_BAD_PARAMETER if the connection ID is invalid.
 */
llStatus_t LL_RssiMon_UnregByConnId(uint8_t connId);

#endif // LL_RSSI_MONITOR_H
