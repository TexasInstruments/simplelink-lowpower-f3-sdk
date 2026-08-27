/******************************************************************************

 @file  ll_connection_monitor.h

 @brief This file contains internal interfaces for the Controller Connection Monitor.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025-2026, Texas Instruments Incorporated

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

#ifndef LL_CONNECTION_MONITOR_H
#define LL_CONNECTION_MONITOR_H

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll.h"
#include "ti/ble/stack_util/connection_monitor_types.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */
#define CM_INVALID_DATA_SIZE               0U
#define CM_EMPTY_PACKET_OCTETS             0U
#define CM_INVALID_ACCESS_ADDR             0xFFFFFFFFU
#define CM_DEFAULT_ADJUSTMENTS_EVT_TRIES   4
#define CM_INVALID_PKT_PHY                 0xFFU


/*******************************************************************************
 * TYPEDEFS
 */

// The controller's Connection Monitor data structure needed by
// @cmStartMonitorParams_t to start the monitoring proccess.
typedef struct
{
  /* General Connection Data */
  uint32_t accessAddr;                             // saved synchronization word to be used by Peripheral
  uint32_t crcInit;                                // connection CRC initialization value (24 bits)
  uint32_t timeToNextEvent;                        // The time left to the next event
  uint16_t currentEvent;                           // current event number
  uint16_t expirationValue;                        // number of events to a LSTO expiration
  /* Connection Parameters */
  uint16_t connInterval;                           // connection interval
  /* Channel Map */
  uint8_t  nextChan;                               // The unmapped channel for the next active connection event
  uint8_t  currentChan;                            // the current unmapped channel for the completed connection event
  uint8_t  currentMappedChan;                      // Currently used mapped channel
  uint8_t  hopLength;                              // used for finding next data channel at next connection event
  uint8_t  chanMap[ LL_NUM_BYTES_FOR_CHAN_MAP ];   // bit map corresponding to the data channels 0..39 //5 Bytes
  /* Peer Address Information */
  uint8_t  peerAddr[ LL_DEVICE_ADDR_LEN ];         // Peer address
  uint8_t  peerAddrType:1;                         // Peer address type
  /* PHY Information */
  uint8_t  curPhy:3;                               // current PHY
  uint8_t  phyOpts:2;                              // Coded phy options
  uint8_t  chanSelAlgo:2;                          // Channel selection algorithm
  /* Central Contribution */
  uint16_t mstSCA;                                 // Central's portion of connection SCA
  /* If there any pending update*/
  cmConnUpdateEvt_t pendingUpdateEvt;              // Connection update event data
} llCmDataFull_t;

/** 
* @brief Enum of the state of the connection monitor
*/
typedef enum
{
  LL_CM_STATE_SYNCING,       //!< The connection monitor trying to find the first packet.
  LL_CM_STATE_ADJUSTMENT,    //!< The connection monitor is in adjustment state.
  LL_CM_STATE_ACTIVE,        //!< The connection monitor is active and reports the rssi.
} llCmState_e;

/** 
* @brief Connection Monitor Connection Role
*/
typedef enum
{
  LL_CM_PERIPHERAL_ROLE       = 0x02,
  LL_CM_CENTRAL_ROLE          = 0x01,
} llCmConnectionMaskRole_e;


// The controller's Connection Monitor data structure
typedef struct
{
  uint32_t               relRxTimeoutTime;            //! Time before timing out the first packet of the event
  uint32_t               lastScanDuration;            //! Last scan RX duration
  uint32_t               timeStampCentral;            //! Last valid timeStamp Central
  uint32_t               timeStampPeripheral;         //! Last valid timeStamp Peripheral
  uint32_t               maxDataTicks;                //! The maximum ticks it takes to snd a the maximum payload data
  uint32_t               emptyPktLenInTicks;          //! The len of empty packet in ticks by current phy
  uint16_t               lastValidCentralEvent;       //! The last valid central event
  uint16_t               lastAdjustmentUpdateEvent;   //! The last valid central event
  uint8_t                lastPktLength;               //! The length of the last packet received
  uint8_t                lastCentralPktPhy;           //! The phy used to receive the central last valid packet @RCL_Ble5_RxPhy
  uint8_t                rssiCentral;                 //! Last Rssi value Central
  uint8_t                rssiPeripheral;              //! Last Rssi value Peripheral
  uint8_t                rxEntryNum;                  //! The number of the current packets recived in the RX window
  uint8_t                adjustEvtNum;                //! The number of events left to create adjusment to the connection
  uint8_t                syncAttemptsLeft;            //! The number of attempts left trying to find the first packet before stopping the session,
  // Flags
  uint8_t                isCentralPktValid:1;         //! Flag that indicates if the central packet is valid
  uint8_t                isPeripheralPktValid:1;      //! Flag that indicates if the peripheral packet is valid
  uint8_t                firstAdjustmentPkt:1;        //! Flag that indicates if the first packet after an adjustment
  uint8_t                startEvtSent:1;              //! Flag that indicates if the start event was sent to the registered callback
  uint8_t                addedOverheadFlag:1;         //! Flag that indicates if the overhead was added to the connection RX window.
  uint8_t                pad:2;                       //! Padding, reserved for future use.
  // End of flags
  llCmState_e            state;                       //! The current state of the connection monitor @ref llCmState_e
  llConnState_t          *pllConn;
} llCmConnState_t;

// The controller's global Connection Monitor data structure
typedef struct
{
  uint8_t         numCmConns;            //! Number of connected connection monitors
  cmReportEvt_t   currentCmReport;       //! The current connection monitor report
  llCmConnState_t *llCmConnection;       //! Array of connections
} llCmConns_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/************************************
 * Connecetion Monitor Serving APIs *
 ************************************/

/*******************************************************************************
 * @fn          LL_CMS_RegisterCBs
 *
 * @brief       Register the callback function at the Controller for the CMS
 *
 * @param       pCBs - pointer to the callback functions
 *
 * @return      CM_SUCCESS, CM_INVALID_PARAMS @ref cmErrorCodes_e
 *
 */
cmErrorCodes_e LL_CMS_RegisterCBs( const cmsCBs_t *pCBs );

/*******************************************************************************
 * @fn          LL_CMS_GetConnDataSize
 *
 * @brief       Request the controller data size and returns the sum of
 *              the controller and host data size

 * @return      The connection data size
 */
uint8_t LL_CMS_GetConnDataSize( void );

/*******************************************************************************
 * @fn         LL_CMS_GetConnData
 *
 * @brief      Get the connection data
 *
 * @param      connHandle - the connection handle
 * @param      pData - pointer to the data buffer
 *
 * @return     CM_SUCCESS, CM_INCORRECT_MODE, CM_INVALID_PARAMS,
 *             CM_NOT_CONNECTED, CM_FAILURE @ref cmErrorCodes_e
 *
 * @output     The params values and buffer in @ref cmsConnDataParams_t will be filled
 *             if the command succeed.
 */
cmErrorCodes_e LL_CMS_GetConnData( uint16_t connHandle, cmsConnDataParams_t *pParams );

/************************************
 *    Connection Monitor APIs       *
 ************************************/

/*******************************************************************************
 * @fn          LL_CM_RegisterCBs
 *
 * @brief       Register the callback function at the Controller
 *
 * @param       pCBs - pointer to the callback functions
 *
 * @return      CM_SUCCESS, CM_INVALID_PARAMS @ref cmErrorCodes_e
 *
 */
cmErrorCodes_e LL_CM_RegisterCBs( const cmCBs_t *pCBs );

/*******************************************************************************
 * @fn        LL_CM_StartMonitor
 *
 * @brief     Starts the connection monitoring process with the connection data given.
 *
 * @param     pParams - Pointer to the monitor parameters
 *
 * @return    CM_SUCCESS, CM_NOT_CONNECTED, CM_ALREADY_REQUESTED,
 *            CM_INVALID_PARAMS, CM_NO_RESOURCE,
 *            CM_CONNECTION_LIMIT_EXCEEDED, CM_UNSUPPORTED @ref cmErrorCodes_e
 *
 * @output    This function will send a @ref CM_TRACKING_START_EVT event to the registered
 *            callback @ref pfnCmConnStatusEvtCB in case of a success monitoring start, otherwise it will send a
 *            @ref CM_TRACKING_STOP_EVT event after the end of the trying to sync up to
 *            maxSyncAttempts in the @ref cmStartMonitorParams_t.
 *
 * @note      If the return code is CM_SUCCESS if there any running advertise, it will be disabled,
 *            and an event of @ref GAP_EVT_ADV_END_AFTER_DISABLE
 *
 * @note      After a success monitoring start, the application should expect
 *            @ref cmReportEvt_t event with the RSSI by calling @ref pfnCmReportEvtCB.
 */
cmErrorCodes_e LL_CM_StartMonitor( cmStartMonitorParams_t *pParams );

/*******************************************************************************
 * @fn         LL_CM_StopMonitor
 *
 * @brief      Stops the connection monitoring process.
 *
 * @param      connHandle - the connection handle
 *
 * @return     CM_SUCCESS, CM_NOT_CONNECTED, CM_ALREADY_REQUESTED @ref cmErrorCodes_e
 */
cmErrorCodes_e LL_CM_StopMonitor( uint16_t connHandle );

/*******************************************************************************
 * @fn         LL_CM_UpdateConn
 *
 * @brief      Update the connection monitoring process.
 *
 * @param      connUpdateEvt - contains the targeted access address and
 *                             the update type and params.
 *
 * @return     CM_SUCCESS, CM_NOT_CONNECTED,
 *             CM_INVALID_PARAMS, CM_ALREADY_REQUESTED @ref cmErrorCodes_e
 */
cmErrorCodes_e LL_CM_UpdateConn( cmConnUpdateEvt_t *connUpdateEvt );

/*******************************************************************************
 * @fn          llCmDynamicAlloc
 *
 *
 * @brief       This function is used to dynamically allocate memory needed by
 *              the Controller to maintain the Connections with CM Role.
 *
 *              Note: This is a one time allocation, the memory of which is
 *                    never freed! So for all intents and purposes, essentially
 *                    static.
 *
 * @param       None.
 *
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS
 *              LL_STATUS_ERROR_MEM_CAPACITY_EXCEEDED
 */
llStatus_t llCmDynamicAlloc( void );

/*******************************************************************************
 * @fn          llCmDynamicFree
 *
 * @brief       This function is used to free any dynamically allocated memory.

 * @param       None
 *
 * @return      None
 */
void llCmDynamicFree( void );

/*******************************************************************************
 * @fn          llCmUpdateRxBuffersForActiveCm
 *
 * @brief       This function is used to update the Rx Buffer pointers in the link parameters
 *              for a connection monitor @ref cmLinkParam
 *
 * @param       connId    - The connection id.
 * @param       rxBuffers - Pointer to the head of the MultiBuffer list.
 *
 * @return      None
 */
void llCmUpdateRxBuffersForActiveCm( uint8_t connId, List_List *rxBuffers );

/*******************************************************************************
 * @fn          llCmUpdateRclCmdChannel
 *
 * @brief       This function is used to update the channel of the RCL command
 *
 * @param       connPtr - Pointer to the current connection.
 *
 * @return      None
 */
void llCmUpdateRclCmdChannel( llConnState_t *connPtr, uint8_t channel);

/*******************************************************************************
 * @fn          llCmUpdateRclCmdPhyFeatures
 *
 * @brief       This function is used to update the phy features of the RCL command
 *
 * @param       connPtr - Pointer to the current connection.
 *
 * @return      None
 */
void llCmUpdateRclCmdPhyFeatures( uint8_t connId, uint16_t phyFeatures );

/*******************************************************************************
 * @fn          llCmCleanCmdRxBuffers
 *
 * @brief       This function is used to clean the RX buffers of the command
 *              of the relevant connection id
 *
 * @param       connId    - The connection id.
 *
 * @return      None
 */
void llCmCleanCmdRxBuffers( uint8_t connId );

/*******************************************************************************
 * @fn          llCmInitStats
 *
 * @brief       This function is used to clear and initialize the statistics
 *              structure
 *
 * @param       None
 *
 * @return      None
 */
void llCmInitStats( void );

/*******************************************************************************
 * @fn          llCmsConnUpdateInd
 *
 * @brief       This function is used to indicates that there was an update of a
 *              connection
 *
 * @param       connPtr    - The connection pointer of the updated connection
 * @param       updateType - The update type @ref LL_CTRL_PHY_UPDATE_IND/
 *                           LL_CTRL_CHANNEL_MAP_IND/LL_CTRL_CONNECTION_UPDATE_IND
 *
 * @return      None
 */
void llCmsConnUpdateInd( llConnState_t *connPtr, uint8_t updateType );

/*******************************************************************************
 * @fn          llCmDisableCurAdv
 *
 * @brief       This function disable the adv set running before building the
 *              connection on the connection monitor
 *
 * @param       None
 *
 * @return      CM_SUCCESS, CM_INVALID_PARAMS @ref cmErrorCodes_e
 */
uint8_t llCmDisableCurAdv( void );

/*******************************************************************************
 * @fn          llCmRxEntryDoneEventHandler
 *
 * @brief       This function handle an rx entry done for the connection monitor
 *              role.
 *
 * @param       pDataEntry - pointer to the rx entry.
 *
 * @return      None
 */
void llCmRxEntryDoneEventHandler(RCL_Buffer_DataEntry *pDataEntry );

/*******************************************************************************
 * @fn          llCm_TaskEnd
 *
 * @brief       This function called after the radio command finishes for post
 *              processing.
 *
 * @param       pDataEntry - pointer to the rx entry.
 *
 * @return      None
 */
void llCm_TaskEnd( void );

/*******************************************************************************
 * @fn          llCmSwitchToAdjustmentIfNeeded
 *
 * @brief       This function checks if the connection needs an adjustment, and it
 *              updates the state if needed or forced.
 *
 * @param       connPtr - Pointer to the current connection.
 * @param       force   - Force the adjustment even if not needed.
 *
 * @return      true if adjustment is needed, false otherwise.
 */
bool llCmCheckIfAdjustmentNeeded( llConnState_t *connPtr, bool force );

#endif /* LL_CONNECTION_MONITOR_H */
