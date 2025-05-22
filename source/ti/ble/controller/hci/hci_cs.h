/******************************************************************************

 @file  hci_cs.h

 @brief This file defines the HCI API functions for Channel Sounding.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2025, Texas Instruments Incorporated

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
#ifndef HCI_CS_H
#define HCI_CS_H
/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/controller/ll/ll_cs_common.h"

/*******************************************************************************
 * @fn          HCI_LE_CS_ReadLocalSupportedCapabilities
 *
 * @brief       Read the local Controller's supported CS capabilities.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_ReadLocalSupportedCapabilities(void);

/*******************************************************************************
 * @fn          HCI_LE_CS_ReadRemoteSupportedCapabilities
 *
 * @brief       Read the remote Controller's supported CS capabilities.
 *
 * @param       connHandle - Connection handle.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_ReadRemoteSupportedCapabilities(uint16 connHandle);

/*******************************************************************************
 * @fn          HCI_LE_CS_SecurityEnable
 *
 * @brief       Enable CS security for the specified connection.
 *
 * @param       connHandle - Connection handle.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_SecurityEnable(uint16 connHandle);

/*******************************************************************************
 * @fn          HCI_LE_CS_SetDefaultSettings
 *
 * @brief       Set default CS settings in the local Controller.
 *
 * @param       connHandle - Connection handle.
 * @param       roleEnable - Role enable settings.
 * @param       csSyncAntennaSelection - CS sync antenna selection.
 * @param       maxTxPower - Maximum transmit power.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_SetDefaultSettings(uint16 connHandle,
                                     uint8 roleEnable,
                                     uint8 csSyncAntennaSelection,
                                     int8 maxTxPower);

/*******************************************************************************
 * @fn          HCI_LE_CS_ReadRemoteFAETable
 *
 * @brief       Read the remote Controller's FAE table.
 *
 * @param       connHandle - Connection handle.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_ReadRemoteFAETable(uint16 connHandle);

/*******************************************************************************
 * @fn          HCI_LE_CS_WriteRemoteFAETable
 *
 * @brief       Write the remote Controller's FAE table.
 *
 * @param       connHandle - Connection handle.
 * @param       reflectorFaeTable - Pointer to the reflector FAE table.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_WriteRemoteFAETable(uint16 connHandle,
                                      void* reflectorFaeTable);

/*******************************************************************************
 * @fn          HCI_LE_CS_CreateConfig
 *
 * @brief       Create a new CS configuration.
 *
 * @param       connHandle - Connection handle.
 * @param       configID - Configuration ID.
 * @param       createContext - Create context flag.
 * @param       pBufConfig - Pointer to the configuration buffer.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_CreateConfig(uint16 connHandle,
                               uint8 configID,
                               uint8 createContext,
                               csConfigBuffer_t* pBufConfig);

/*******************************************************************************
 * @fn          HCI_LE_CS_RemoveConfig
 *
 * @brief       Remove a CS configuration.
 *
 * @param       connHandle - Connection handle.
 * @param       configID - Configuration ID.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_RemoveConfig(uint16 connHandle,
                               uint8 configID);

/*******************************************************************************
 * @fn          HCI_LE_CS_SetChannelClassification
 *
 * @brief       Set the channel classification.
 *
 * @param       channelClassification - Pointer to the channel classification.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_SetChannelClassification(uint8* channelClassification);

/*******************************************************************************
 * @fn          HCI_LE_CS_SetProcedureParameters
 *
 * @brief       Set CS procedure parameters.
 *
 * @param       connHandle - Connection handle.
 * @param       configID - Configuration ID.
 * @param       pParams - Pointer to the procedure parameters.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_SetProcedureParameters(uint16 connHandle,
                                         uint8 configID,
                                         uint8* pParams);

/*******************************************************************************
 * @fn          HCI_LE_CS_ProcedureEnable
 *
 * @brief       Enable or disable the scheduling of CS procedures.
 *
 * @param       connHandle - Connection handle.
 * @param       configID - Configuration ID.
 * @param       enable - Enable or disable flag.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_ProcedureEnable(uint16 connHandle,
                                  uint8 configID,
                                  uint8 enable);

/*******************************************************************************
 * @fn          HCI_LE_CS_Test
 *
 * @brief       Start a CS test.
 *
 * @param       pParams - Pointer to the test parameters.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_Test(uint8* pParams);

/*******************************************************************************
 * @fn          HCI_LE_CS_TestEnd
 *
 * @brief       End a CS test.
 *
 * @return      @ref HCI_SUCCESS
 */
uint8_t HCI_LE_CS_TestEnd(void);

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
                                            llCsCapabilities_t* peerCapabilities);

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
                                       const csConfigurationSet_t* csConfig);

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
#endif // HCI_CS_H
