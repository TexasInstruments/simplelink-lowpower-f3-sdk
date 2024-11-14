/******************************************************************************

 @file  ll_cs_mgr.h

 @brief The LL CS Manager handles the Link Layer part of the CS LE HCI commands.
        This module is responsible for parameter and condition validation
        before it decides to continue working on the command.
        This module also enqueues CS control Packet opcodes.

 @note The error codes marked with * are per specfication. The rest of the
       Error codes that were used so that the HCI can parse them.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2022-2024, Texas Instruments Incorporated

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

/*******************************************************************************
 * INCLUDES
 */
#include "ll_cs_common.h"
#include "cs/ll_cs_test.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          LL_CS_SecurityEnable
 *
 * @brief       Start or restart the CS Security Start procedure.
 * This API is called by the HCI when the host needs to start the
 * CS Security Procedure. Only a Central should initiate this
 * procedure, after a connection has been established and
 * encrypted. This API shall call the CS Security module to
 * generate CS Security Vectors.
 * Once the vectors were generated properly, it shall set them in
 * the CS Database using the stucture csSecVectors_t.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      status:
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED if connection is not
 *                           encrypted if the controller is a peripheral
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if CS is not supported
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION if connHandle is inactive
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_SecurityEnable(uint16 connId);

/*******************************************************************************
 * @fn          LL_CS_ReadLocalSupportedCapabilites
 *
 * @brief       Read CS capabilities supported by the local Controller.
 * Device's own capabilities are constant and shall be retrieved
 * from the CS Database using llCsDbGetLocalCapabilities
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pLocalCapabilities - pointer to local capabilities
 *
 * output parameters
 *
 * @param       pLocalCapabilities.
 *
 * @return      status:
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED if the CS feature is not
 *                                            supported
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e
LL_CS_ReadLocalSupportedCapabilites(csCapabilities_t* pLocalCapabilities);

/*******************************************************************************
 * @fn          LL_CS_ReadRemoteSupportedCapabilities
 *
 * @brief       Query the CS capabilities supported by the remote Controller.
 * This API results in the CS control Procedure CS Capabilities
 * Exchange by sending the control packet LL_CS_CAPABILITIES_REQUEST.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - Connection Handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      status:
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED if the CS feature is not
 *                                                 supported
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION if the connection handle is
 *                                                  inactive
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if the CS feature is not
 *                                                  supported
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_ReadRemoteSupportedCapabilities(uint16 connId);

/*******************************************************************************
 * @fn          LL_CS_SetDefaultSettings
 *
 * @brief       Set default CS settings in the local Controller.
 * Enable or disable the CS role options in the local Controller.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - Connection Handle
 * @param       defaultSettings - default settings: role enable and
 *                                csSyncAntennaSelection
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      status:
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER if this command disabled a
 *                                                   role that is in use.
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if cs sync antenna not
 *                                                    supported
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if the CS feature is not
 *                                                    supported
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_SetDefaultSettings(uint16 connId,
                                  csDefaultSettings_t* defaultSettings);

/*******************************************************************************
 * @fn          LL_CS_ReadLocalFAETable
 *
 * @brief       Read the per-channel Mode 0 Frequency Actuation Error table of
 *              the local Controller.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pFaeTable - pointer to FAE table
 *
 * output parameters
 *
 * @param       pFaeTable.
 *
 * @return      status:
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if non-zero FAE table is
 *                                                    not supported
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if the CS feature is not
 *                                                    supported
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER if pointer is invalid (null)
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_ReadLocalFAETable(csFaeTbl_t* pFaeTable);

/*******************************************************************************
 * @fn          LL_CS_ReadRemoteFAETable
 *
 * @brief       Read the per-channel Mode 0 FAE table of the remote Controller.
 * This API starts the LL CS Mode-0 FAE Table Update Control
 * Procedure by sending LL_CS_FAE_REQ.
 * If the remote does not support the not support non-zero FAE
 * Generate the LE_CS_Read_Remote_FAE_Table_Complete
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Handle
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      status:
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER for invalid connId
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION if the connection handle
 *                                                  is inactive
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if the CS feature is not
 *                                                  supported
 *             LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_ReadRemoteFAETable(uint16 connId);

/*******************************************************************************
 * @fn          LL_CS_WriteRemoteFAETable
 *
 * @brief       Write the per-channel Mode 0 FAE table of the remote controller
 * The remote controller in a reflector role (if previously known
 * by the host) The API sets the provided FAE table in the CS Database
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection Handle
 * @param       pFaeTbl - Pointer to reflector table
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      status:
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER for invalid connId, or
 *                                                    pointer
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION if the connection handle is
 *                                                    inactive
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if non-zero FAE table is not
 *                                                    supported by the host
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_WriteRemoteFAETable(uint16 connId, int8* pFaeTbl);

/*******************************************************************************
 * @fn          LL_CS_CreateConfig
 *
 * @brief       Create a new CS configuration
 * with the identifier Config_ID in the local and remote Controller.
 * Sets the configuration in the CS database.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - Connection Handle
 * @param       pConfig - pointer Configuration parameters struct
 * @param       createContext - If enabled, write configuration in both local
 *                              and remote Controller
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status:
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if the configuration
 *                                        parameters are not supported locally.
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER  if role is not enabled.
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED if capabilities
 *                                                 are not previously known
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER if any parameter is invalid
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if CS is not supported
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION if connection is inactive
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_CreateConfig(uint16 connId, csConfigurationSet_t* pConfig,
                            uint8 createContext);

/*******************************************************************************
 * @fn          LL_CS_RemoveConfig
 *
 * @brief       Remove a CS configuration
 * Identified by Config ID from the local controller for the specified
 * connection. Remove config changes config.enable from CS_ENABLE to CS_DISABLE
 * in the LL CS Database.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection handle
 * @param       configId - configuration ID
 * output parameters
 *
 * @param       None.
 *
 * @return      Status:
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER if configId does not exist
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED if a CS Procedure was
 *                                                    enabled for the config ID
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if CS is not supported
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER if connId is invalid
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION if connection is inactive
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_RemoveConfig(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          LL_CS_SetChannelClassification
 *
 * @brief       Update the channel classification based on its local info
 * This channel classification persists until overwritten with a
 * subsequent HCI_LE_CS_Set_CS_Channel_Classification command or
 * until the Controller is reset. The Controller may combine the
 * channel classification information provided by the Host along
 * with local channel classification information to send an updated
 * CS channel map to the remote Controller.
 *
 * updates should be sent within 10 seconds of the host knowing
 * that the channel classification has changed.
 *
 * The interval between two successive commands shall be at least
 * 1 second.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pChannelClassification - Pointer to the channel classification
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status:
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER if pChannelClassification
 *                                                   is invalid
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if CS is not supported
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_SetChannelClassification(uint8* pChannelClassification);

/*******************************************************************************
 * @fn          LL_CS_SetProcedureParameters
 *
 * @brief       Set CS Procedure Parameters
 * Set the parameters for the scheduling of one or more CS
 * procedures by the local Controller with the remote device for
 * the CS Config_ID and the Connection_Handle parameters
 * This API sets the provided CS Parameters using llCsDbSetProcedureParams.
 *
 * @note the procedure parameter: maxProcedureCount can be set to 0
 * (indefinite), in this case the procedure will run indefinitely and since
 * CS is a higher priority than BLE task, this means that other BLE connections
 * may be starved and lost.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId  - connection handle
 * @param       configId  - configuration ID
 * @param       csProcParams  - CS Procedure parameters
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status:
 *              LL_STATUS_ERROR_DUE_TO_LIMITED_RESOURCES if parameter exceed
 *                                    capabilities or coexistence constraints.
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER if config ID is disabled,
 *                                    not existent or in use
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if CS is not supported
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION if the connection handle is
 *                                                  inactive
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_SetProcedureParameters(uint16 connId, uint8 configId,
                                      csProcedureParams_t* csProcParams);

/*******************************************************************************
 * @fn          LL_CS_ProcedureEnable
 *
 * @brief       Enable or disable the scheduling of CS procedures
 *
 * This API results in the CS control Procedure CS Start by sending
 * the control packet LL_CS_REQ.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId - connection handle
 * @param       configId - configuration Id
 * @param       enable - enable or disable the scheduling of a CS procedure
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status:
 *              LL_STATUS_ERROR_COMMAND_DISALLOWED if CS procedure is enabled
 *                                                for this config Id.
 *              LL_STATUS_ERROR_UNEXPECTED_PARAMETER if config ID is disabled
 *                                                     ,not existent or in use
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if CS is not supported
 *              LL_STATUS_ERROR_INACTIVE_CONNECTION if the connection handle
 *                                                  is inactive
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_ProcedureEnable(uint16 connId, uint8 configId, uint8 enable);

/*******************************************************************************
 * @fn          LL_CS_Test
 *
 * @brief       Start a CS Test
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pParams - Pointer CS Test Params
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if CS is not supported
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_Test(csTestParams_t* pParams);

/*******************************************************************************
 * @fn          LL_CS_TestEnd
 *
 * @brief       End a CS test
 * Where the DUT is placed in the role of either the initiator or reflector.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Status
 *              LL_STATUS_ERROR_FEATURE_NOT_SUPPORTED if CS is not supported
 *              LL_STATUS_SUCCESS when command is successful
 */
csStatus_e LL_CS_TestEnd(void);

/*******************************************************************************
 * EVENT COMPLETE CALLBACK FUNCTIONS
 */

/*******************************************************************************
 * @fn          HCI_CS_ReadRemoteSupportedCapabilitiesCback
 *
 * @brief       Callback for reading the remote CS capabilities
 *
 * input parameters
 *
 * @param       status - status
 * @param       connHandle - connection handle
 * @param       peerCapabilities- peerCapabilities
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
extern void
HCI_CS_ReadRemoteSupportedCapabilitiesCback(uint8 status, uint16 connHandle,
                                            csCapabilities_t* peerCapabilities);

/*******************************************************************************
 * @fn          HCI_CS_ConfigCompleteCback
 *
 * @brief       Callback to send CS Config Complete event
 *
 * input parameters
 *
 * @param       status     - status
 * @param       connHandle - connection handle
 * @param       csConfig   - peerCapabilities
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
extern void HCI_CS_ConfigCompleteCback(uint8 status, uint16 connHandle,
                                       csConfigurationSet_t* csConfig);

/*******************************************************************************
 * @fn          HCI_CS_ReadRemoteFAETableCompleteCback
 *
 * @brief       callback function that returns the result of reading the remote
 *              FAE table
 *
 * input parameters
 *
 * @param       status
 * @param       connHandle connection handle
 * @param       faeTable   pointer to remote FAE table
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
extern void HCI_CS_ReadRemoteFAETableCompleteCback(uint8 status,
                                                   uint16 connHandle,
                                                   uint8* faeTable);

/*******************************************************************************
 * @fn          HCI_CS_SecurityEnableCompleteCback
 *
 * @brief       Security Enable complete callback function
 *
 * input parameters
 *
 * @param       status     - event status
 * @param       connHanlde - connection identifier
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
extern void HCI_CS_SecurityEnableCompleteCback(uint8 status, uint16 connHandle);

/*******************************************************************************
 * @fn          HCI_CS_ProcedureEnableCompleteCback
 *
 * @brief       Procedure Enable Complete Event callback
 *
 * input parameters
 *
 * @param       status     - event status
 * @param       connHanlde - connection identifier
 * @param       enable     - indicates enable or disable
 * @param       enableData - enable data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
extern void
HCI_CS_ProcedureEnableCompleteCback(uint8 status, uint16 connHandle,
                                    uint8 enable,
                                    csProcedureEnable_t* enableData);

/*******************************************************************************
 * @fn          HCI_CS_SubeventResultCback
 *
 * @brief       Subevent results callback
 *
 * input parameters
 *
 * @param       pRes - pointer to results data
 * @param       dataLength - length of data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
extern void HCI_CS_SubeventResultCback(void* pRes, uint16 dataLength);

/*******************************************************************************
 * @fn          HCI_CS_SubeventResultContinueCback
 *
 * @brief       Subevent result continue callback
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       hdr - pointer to results header
 * @param       data - pointer to results data
 * @param       dataLength - length of data
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_CS_SubeventResultContinueCback(void* hdr, const void* data, uint16 dataLength);

/*******************************************************************************
 * @fn          HCI_CS_TestEndCompleteCback
 *
 * @brief       CS Test End Complete callback
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       status - command status
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HCI_CS_TestEndCompleteCback(uint8 status);

/*******************************************************************************
 */
