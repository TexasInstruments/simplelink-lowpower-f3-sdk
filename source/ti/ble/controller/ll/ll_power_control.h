/******************************************************************************

 @file  ll_power_control.h

 @brief This file contains the Link Layer (LL) power control APIs, control
        packets handling and the events raised to the host.

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
#ifndef LL_POWER_CONTROL_H
#define LL_POWER_CONTROL_H
/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/controller/ll/ll_common.h"

/*******************************************************************************
 * CONSTANTS
 */

// Power control phy types defines for coded S2 and coded S8 for hci and
// controller layer
#define PHY_CODED_S8_HCI_DEFINE         0x3U
#define PHY_CODED_S2_HCI_DEFINE         0x4U
#define PHY_CODED_S8_CONTROLLER_DEFINE  0x4U
#define PHY_CODED_S2_CONTROLLER_DEFINE  0x8U

// Define thresholds for the RSSI monitor to use if enable
// If not defined by the user, the default values will be used
// We should create set/get config for these values (same as we did for the rssi monitor)
#ifndef PWR_CTRL_RSSI_LOW_THRESHOLD
#define PWR_CTRL_RSSI_LOW_THRESHOLD     -70
#endif

#ifndef PWR_CTRL_RSSI_HIGH_THRESHOLD
#define PWR_CTRL_RSSI_HIGH_THRESHOLD    0
#endif

// Default value for the incremental step of the delta value in the power control
// request when reaching RSSI threshold
#ifndef PWR_CTRL_DELTA_STEP_DB
#define PWR_CTRL_DELTA_STEP_DB          10
#endif

// Min/max field values
#define MIN_OR_MAX_TXPOWER_NOT_REACHED  0x0U
#define MIN_TXPOWER_REACHED             0x1U
#define MAX_TXPOWER_REACHED             0x2U
#define MIN_OR_MAX_CALC_LOCAL           0x3U

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * TYPEDEFS
 */

// Event reason parameter values
typedef enum {
  TXPOWER_CHANGE_EVENT_LOCAL = 0x0,
  TXPOWER_CHANGE_EVENT_REMOTE = 0x1,
  TXPOWER_CHANGE_EVENT_HOST_REQ = 0x2 // HCI_LE_Read_Remote_Transmit_Power_Level command completed
} txPowerChangeEventReason_e;

// Power control request or indication packet information
typedef struct
{
  int8_t  delta;                 // Delta field value the local device need to send, dB units
  uint8_t phy;                   // Phy field value the local device need to send
} powerControlReqIndPacket_t;

// Power control response packet information
typedef struct
{
  int8_t   delta;                 // Delta field value the local device need to send, dB units
  uint8_t  phy;                   // Phy field value the local device need to send
  uint8_t  apr;                   // Apr field value the local device need to send
} powerControlRspPacket_t;

// Power control procedure information for every connection
typedef struct
{
  powerControlReqIndPacket_t   reqPacket;          // Struct to hold phy and delta fields of the request packet
  powerControlRspPacket_t      rspPacket;          // Struct to hold phy, delta and apr fields of the response packet
  powerControlReqIndPacket_t   indPacket;          // Struct to hold phy and delta fields of the indication packet
  uint8_t                      powerControlConfig; // Bitmask of configurations set for this connection
  uint8_t                      powerControlStates; // Bitmask of states set for this connection
  int8_t                       txPowerLevelDbm;    // The power level on the current phy, range defined by RCL, default value by user in dBm from global txPower
} powerControlInfo_t;

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
 * @fn          LL_PwrCtrl_Open
 *
 * @brief       Initiate power control data array
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS, LL_STATUS_ERROR_MEM_CAPACITY_EXCEEDED
 */
llStatus_t LL_PwrCtrl_Open(void);

/*******************************************************************************
 * @fn          LL_PwrCtrl_Close
 *
 * @brief       Free allocated power control data array
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void LL_PwrCtrl_Close(void);

/*******************************************************************************
 * @fn          LL_PwrCtrl_OpenConn
 *
 * @brief       Initiate power control fields for a specific connection
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       connId - Connection Identifier
 *
 * output parameters
 *
 * @param       connPtr
 *
 * @return      None
 */
void LL_PwrCtrl_OpenConn(uint8_t connId);

/*******************************************************************************
 * @fn          LL_PwrCtrl_CloseConn
 *
 * @brief       Clear and set default values to a specific connection id
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       connId  - Connection Identifier
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void LL_PwrCtrl_CloseConn(uint8_t connId);

/*******************************************************************************
 * @fn          LL_PwrCtrl_HandleProcedure
 *
 * @brief       Handle the power control packets that is received in the process
 *              control procedure
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       connPtr     - Pointer to the current connection
 * @param       ctrlPktType - Control packet type
 *
 * output parameters
 *
 * @param       connPtr - Updated control packet queue, control packet timeout
 *
 * @return      LL_STATUS_SUCCESS
 *              LL_CTRL_PROC_STATUS_TERMINATE
 */
llStatus_t LL_PwrCtrl_HandleProcedure(llConnState_t *connPtr, uint8_t ctrlPktType);

/*******************************************************************************
 * @fn          LL_PwrCtrl_HandleRxPackets
 *
 * @brief       Handle the power control packets received from the peer device
 *              Control packets that will be handle here:
 *              LL_CTRL_POWER_CONTROL_REQ
 *              LL_CTRL_POWER_CONTROL_RSP
 *              LL_CTRL_POWER_CONTROL_IND
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       connPtr            - Pointer to the current connection
 * @param       pCtrlPktPayload    - Control packet payload
 * @param       opcode             - Which control packet are we handling
 *
 * output parameters
 *
 * @param       connPtr - Updated fields in the connection pointer
 *
 * @return      None
 */
void LL_PwrCtrl_HandleRxPackets( llConnState_t *connPtr,
                                 const uint8_t *pCtrlPktPayload,
                                 uint8_t        opcode );

/*******************************************************************************
 * @fn          LL_PwrCtrl_HandlePhyChangeEvent
 *
 * @brief       Handle the power control procedure when phy change happen
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       connPtr - Pointer to the current connection
 *
 * output parameters
 *
 * @param       connPtr - Updated power control struct inside the connPtr
 *
 * @return      None
 */
void LL_PwrCtrl_HandlePhyChangeEvent(llConnState_t *connPtr);

/*******************************************************************************
 * @fn          LL_PwrCtrl_SetupPwrCtrlPkt
 *
 * @brief       This function is used to setup the power control packets; Request,
 *              Response, or Indication
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       connPtr        - Pointer to the current connection
 * @param       ctrlPktType    - Packet type we are handling
 *
 * output parameters
 *
 * @param       pCtrlPktData   - Pointer to the control packet data payload to fill
 *
 * @return      None.
 */
void LL_PwrCtrl_SetupPwrCtrlPkt( const llConnState_t *connPtr,
                                 uint8_t              ctrlPktType,
                                 uint8_t             *pCtrlPktPayload );

/*******************************************************************************
 * @fn          LL_PwrCtrl_SetFeatureBit
 *
 * @brief       Set the power control feature bit in deviceFeatureSet.featureSet
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       connPtr - Pointer to the current connection
 *
 * @return      Mask bit for the power control feature bit
 */
llStatus_t LL_PwrCtrl_SetFeatureBit(void);

/*******************************************************************************
 * @fn          LL_PwrCtrl_UpdatePwrLevelAllConn
 *
 * @brief       Update all power level on all active connections if the power
 *              control feature is activated
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void LL_PwrCtrl_UpdatePwrLevelAllConn(void);

/*******************************************************************************
 * @fn          LL_PwrCtrl_ReadRemoteTransPwrLevelCmd
 *
 * @brief       Handle the HCI call for Read Remote Transmit Power Level Command
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       connHandle - Connection ID
 * @param       txPhy      - Which phy to check the power level in the peer device
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
llStatus_t LL_PwrCtrl_ReadRemoteTransPwrLevelCmd(uint16_t connHandle, uint8_t txPhy);

/*******************************************************************************
 * @fn          LL_PwrCtrl_EnhancedReadTransPwrLevelCmd
 *
 * @brief       Handle the HCI call for Enhanced Read Transmit Power Level Command
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       connHandle      - Connection ID
 * @param       txPhy           - Which phy to check the power in the peer devcie
 *
 * output parameters
 *
 * @param       pCurrentTxPower - Pointer to fill the current tx power value
 * @param       pMaxTxPower     - Poinetr to fill the max tx power value
 *
 * @return      LL_STATUS_SUCCESS
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
llStatus_t LL_PwrCtrl_EnhancedReadTransPwrLevelCmd( uint16_t connHandle,
                                                    uint8_t  txPhy,
                                                    int8_t  *pCurrentTxPower,
                                                    int8_t  *pMaxTxPower );

/*******************************************************************************
 * @fn          LL_PwrCtrl_SetTransPwrRptEnableCmd
 *
 * @brief       Enable or disable the local or remote reporting events
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       connHandle      - Connection ID
 * @param       localEnable     - Enable or disable local change power events
 * @param       remoteEnable    - Enable or disable remote change power events
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
llStatus_t LL_PwrCtrl_SetTransPwrRptEnableCmd( uint16_t connHandle,
                                               uint8_t  localEnable,
                                               uint8_t  remoteEnable );

/*******************************************************************************
 * @fn          LL_EXT_PwrCtrl_SendPwrCtrlReqCmd
 *
 * @brief       Send power control request packet with desired delta value
 *
 * @Design      BLE_LOKI-1969
 *
 * input parameters
 *
 * @param       connHandle      - Connection ID
 * @param       txPhy           - Which phy to check the power in the peer devcie
 * @param       deltaPowerDb    - Amount of Tx Power to ask the peer device to change (dB)
 * @param       aprEnable       - Enable or disable the usage of the APR value
 *
 * output parameters
 *
 * @param       None
 *
 * @return      LL_STATUS_SUCCESS
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION
 */
llStatus_t LL_EXT_PwrCtrl_SendPwrCtrlReqCmd( uint16_t connHandle,
                                             uint8_t  txPhy,
                                             int8_t   deltaPowerDb,
                                             uint8_t  enableApr );

#endif // LL_POWER_CONTROL_H
