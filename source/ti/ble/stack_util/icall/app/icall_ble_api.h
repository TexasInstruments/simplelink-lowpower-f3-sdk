/******************************************************************************

 @file  icall_ble_api.h

 @brief Redefinition of all BLE APIs when ICALL_LITE is in use

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2016-2025, Texas Instruments Incorporated
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

#ifndef ICALL_BLE_API_H
#define ICALL_BLE_API_H

#ifdef __cplusplus
extern "C"
{
#endif

/* This Header file contains all call structure definition */
#include "ti/ble/stack_util/icall/app/icall_ble_apimsg.h"

/*********************************************************************
 * INCLUDES
 */

#include "ti/ble/stack_util/icall/app/icall_api_idx.h"

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
extern uint8 buildRevision(ICall_BuildRevision *pBuildRev);

#ifndef CONTROLLER_ONLY
extern void *L2CAP_bm_alloc( uint16 size );
extern void GATT_bm_free(gattMsg_t *pMsg, uint8 opcode);
extern void *GATT_bm_alloc(uint16 connHandle, uint8 opcode, uint16 size,
                           uint16 *pSizeAlloc);
#endif /* !CONTROLLER_ONLY */

extern uint16 lastAppOpcodeSent;
/*********************************************************************
 * MACROS
 */

/* all the API bellow has been extracted from the following files:
  - gap.h
  - hci.h
  - l2cap.h
  - gatt.h
  - gattserverapp.h
  - linkdb.h
  - att.h
*/

// this macro should only be used within HCI test and host test app, where
// two host can request HCI command (internal embedded application and
// external Host or internal stack host)
// _api is API function name to call, e.g. HCI_ReadBDADDRCmd,
// for example, if the HCI application wants to call HCI_ReadBDADDRCmd();, it
// will be translated into:
// EMBEDDED_HOST(HCI_ReadBDADDRCmd)
// Another example, if the HCI application wants to call
// HCI_EXT_ConnEventNoticeCmd(pAttRsp->connHandle, selfEntity, 0); , it will be
// translated into:
// EMBEDDED_HOST(HCI_EXT_ConnEventNoticeCmd, pAttRsp->connHandle, selfEntity, 0)

#define PREFIX(_name) (IDX_##_name)
#define EMBEDDED_HOST(_api, ...) \
{ \
  lastAppOpcodeSent = (uint32_t)  PREFIX(_api); \
  _api(##__VA_ARGS__); \
}

/* HCI API */
/***********/
#define HCI_ReadRssiCmd(...)                        (icall_directAPI((uint32_t) IDX_HCI_ReadRssiCmd , ##__VA_ARGS__))
#define HCI_SetEventMaskCmd(...)                    (icall_directAPI((uint32_t) IDX_HCI_SetEventMaskCmd , ##__VA_ARGS__))
#define HCI_SetEventMaskPage2Cmd(...)               (icall_directAPI((uint32_t) IDX_HCI_SetEventMaskPage2Cmd , ##__VA_ARGS__))
#define HCI_ResetCmd(...)                           (icall_directAPI((uint32_t) IDX_HCI_ResetCmd))
#define HCI_ReadLocalVersionInfoCmd(...)            (icall_directAPI((uint32_t) IDX_HCI_ReadLocalVersionInfoCmd))
#define HCI_ReadLocalSupportedCommandsCmd(...)      (icall_directAPI((uint32_t) IDX_HCI_ReadLocalSupportedCommandsCmd))
#define HCI_ReadLocalSupportedFeaturesCmd(...)      (icall_directAPI((uint32_t) IDX_HCI_ReadLocalSupportedFeaturesCmd))
#define HCI_ReadBDADDRCmd(...)                      (icall_directAPI((uint32_t) IDX_HCI_ReadBDADDRCmd))
#define HCI_HostNumCompletedPktCmd(...)             (icall_directAPI((uint32_t) IDX_HCI_HostNumCompletedPktCmd , ##__VA_ARGS__))
#define HCI_HostBufferSizeCmd(...)                  (icall_directAPI((uint32_t) IDX_HCI_HostBufferSizeCmd , ##__VA_ARGS__))
#define HCI_SetControllerToHostFlowCtrlCmd(...)     (icall_directAPI((uint32_t) IDX_HCI_SetControllerToHostFlowCtrlCmd , ##__VA_ARGS__))
#define HCI_ReadRemoteVersionInfoCmd(...)           (icall_directAPI((uint32_t) IDX_HCI_ReadRemoteVersionInfoCmd , ##__VA_ARGS__))
#define HCI_DisconnectCmd(...)                      (icall_directAPI((uint32_t) IDX_HCI_DisconnectCmd , ##__VA_ARGS__))
#define HCI_ReadTransmitPowerLevelCmd(...)          (icall_directAPI((uint32_t) IDX_HCI_ReadTransmitPowerLevelCmd , ##__VA_ARGS__))

#define HCI_LE_ReceiverTestCmd(...)                 (icall_directAPI((uint32_t) IDX_HCI_LE_ReceiverTestCmd , ##__VA_ARGS__))
#define HCI_LE_TransmitterTestCmd(...)              (icall_directAPI((uint32_t) IDX_HCI_LE_TransmitterTestCmd , ##__VA_ARGS__))
#define HCI_LE_TestEndCmd(...)                      (icall_directAPI((uint32_t) IDX_HCI_LE_TestEndCmd))
#define HCI_LE_EncryptCmd(...)                      (icall_directAPI((uint32_t) IDX_HCI_LE_EncryptCmd , ##__VA_ARGS__))
#define HCI_LE_RandCmd(...)                         (icall_directAPI((uint32_t) IDX_HCI_LE_RandCmd))
#define HCI_LE_ReadSupportedStatesCmd(...)          (icall_directAPI((uint32_t) IDX_HCI_LE_ReadSupportedStatesCmd))
#define HCI_LE_ReadAcceptListSizeCmd(...)           (icall_directAPI((uint32_t) IDX_HCI_LE_ReadAcceptListSizeCmd))
#define HCI_LE_ClearAcceptListCmd(...)              (icall_directAPI((uint32_t) IDX_HCI_LE_ClearAcceptListCmd))
#define HCI_LE_AddAcceptListCmd(...)                (icall_directAPI((uint32_t) IDX_HCI_LE_AddAcceptListCmd , ##__VA_ARGS__))
#define HCI_LE_RemoveAcceptListCmd(...)             (icall_directAPI((uint32_t) IDX_HCI_LE_RemoveAcceptListCmd , ##__VA_ARGS__))
#define HCI_LE_ReadWhiteListSizeCmd(...)            (icall_directAPI((uint32_t) IDX_HCI_LE_ReadAcceptListSizeCmd))
#define HCI_LE_ClearWhiteListCmd(...)               (icall_directAPI((uint32_t) IDX_HCI_LE_ClearAcceptListCmd))
#define HCI_LE_AddWhiteListCmd(...)                 (icall_directAPI((uint32_t) IDX_HCI_LE_AddAcceptListCmd , ##__VA_ARGS__))
#define HCI_LE_RemoveWhiteListCmd(...)              (icall_directAPI((uint32_t) IDX_HCI_LE_RemoveAcceptListCmd , ##__VA_ARGS__))
#define HCI_LE_SetEventMaskCmd(...)                 (icall_directAPI((uint32_t) IDX_HCI_LE_SetEventMaskCmd , ##__VA_ARGS__))
#define HCI_LE_ReadLocalSupportedFeaturesCmd(...)   (icall_directAPI((uint32_t) IDX_HCI_LE_ReadLocalSupportedFeaturesCmd))
#define HCI_LE_ReadBufSizeCmd(...)                  (icall_directAPI((uint32_t) IDX_HCI_LE_ReadBufSizeCmd))
#define HCI_LE_SetRandomAddressCmd(...)             (icall_directAPI((uint32_t) IDX_HCI_LE_SetRandomAddressCmd , ##__VA_ARGS__))
#define HCI_LE_ReadAdvChanTxPowerCmd(...)           (icall_directAPI((uint32_t) IDX_HCI_LE_ReadAdvChanTxPowerCmd))
#define HCI_LE_ReadChannelMapCmd(...)               (icall_directAPI((uint32_t) IDX_HCI_LE_ReadChannelMapCmd , ##__VA_ARGS__))
#define HCI_LE_ReadRemoteUsedFeaturesCmd(...)       (icall_directAPI((uint32_t) IDX_HCI_LE_ReadRemoteUsedFeaturesCmd , ##__VA_ARGS__))
#define HCI_LE_SetHostChanClassificationCmd(...)    (icall_directAPI((uint32_t) IDX_HCI_LE_SetHostChanClassificationCmd , ##__VA_ARGS__))
#define HCI_LE_SetHostFeature(...)                  (icall_directAPI((uint32_t) IDX_HCI_LE_SetHostFeature , ##__VA_ARGS__))
/* HCI V4.2 DLE API */
/*******************/
#define HCI_LE_SetDataLenCmd(...)                   (icall_directAPI((uint32_t) IDX_HCI_LE_SetDataLenCmd , ##__VA_ARGS__))
#define HCI_LE_ReadSuggestedDefaultDataLenCmd(...)  (icall_directAPI((uint32_t) IDX_HCI_LE_ReadSuggestedDefaultDataLenCmd))
#define HCI_LE_WriteSuggestedDefaultDataLenCmd(...) (icall_directAPI((uint32_t) IDX_HCI_LE_WriteSuggestedDefaultDataLenCmd , ##__VA_ARGS__))
#define HCI_LE_ReadMaxDataLenCmd(...)               (icall_directAPI((uint32_t) IDX_HCI_LE_ReadMaxDataLenCmd))
/* HCI V4.1 API */
/****************/
#define HCI_ReadAuthPayloadTimeoutCmd(...)          (icall_directAPI((uint32_t) IDX_HCI_ReadAuthPayloadTimeoutCmd , ##__VA_ARGS__))
#define HCI_WriteAuthPayloadTimeoutCmd(...)         (icall_directAPI((uint32_t) IDX_HCI_WriteAuthPayloadTimeoutCmd , ##__VA_ARGS__))
#define HCI_LE_RemoteConnParamReqReplyCmd(...)      (icall_directAPI((uint32_t) IDX_HCI_LE_RemoteConnParamReqReplyCmd , ##__VA_ARGS__))
#define HCI_LE_RemoteConnParamReqNegReplyCmd(...)   (icall_directAPI((uint32_t) IDX_HCI_LE_RemoteConnParamReqNegReplyCmd , ##__VA_ARGS__))


/* HCI V4.2 Privacy API */
/************************/
#define HCI_LE_AddDeviceToResolvingListCmd(...)           (icall_directAPI((uint32_t) IDX_HCI_LE_AddDeviceToResolvingListCmd , ##__VA_ARGS__))
#define HCI_LE_RemoveDeviceFromResolvingListCmd(...)      (icall_directAPI((uint32_t) IDX_HCI_LE_RemoveDeviceFromResolvingListCmd , ##__VA_ARGS__))
#define HCI_LE_ClearResolvingListCmd(...)                 (icall_directAPI((uint32_t) IDX_HCI_LE_ClearResolvingListCmd))
#define HCI_LE_ReadResolvingListSizeCmd(...)              (icall_directAPI((uint32_t) IDX_HCI_LE_ReadResolvingListSizeCmd))
#define HCI_LE_ReadPeerResolvableAddressCmd(...)          (icall_directAPI((uint32_t) IDX_HCI_LE_ReadPeerResolvableAddressCmd , ##__VA_ARGS__))
#define HCI_LE_ReadLocalResolvableAddressCmd(...)         (icall_directAPI((uint32_t) IDX_HCI_LE_ReadLocalResolvableAddressCmd , ##__VA_ARGS__))
#define HCI_LE_SetAddressResolutionEnableCmd(...)         (icall_directAPI((uint32_t) IDX_HCI_LE_SetAddressResolutionEnableCmd , ##__VA_ARGS__))
#define HCI_LE_SetResolvablePrivateAddressTimeoutCmd(...) (icall_directAPI((uint32_t) IDX_HCI_LE_SetResolvablePrivateAddressTimeoutCmd , ##__VA_ARGS__))
#define HCI_LE_SetPrivacyModeCmd(...)                     (icall_directAPI((uint32_t) IDX_HCI_LE_SetPrivacyModeCmd , ##__VA_ARGS__))
/* HCI V4.2 SC API */
/********************/
#define HCI_LE_ReadLocalP256PublicKeyCmd(...)             (icall_directAPI((uint32_t) IDX_HCI_LE_ReadLocalP256PublicKeyCmd))
#define HCI_LE_GenerateDHKeyCmd(...)                      (icall_directAPI((uint32_t) IDX_HCI_LE_GenerateDHKeyCmd , ##__VA_ARGS__))
/* HCI V5.0 - 2M and Coded PHY */
/************************/
#define HCI_LE_ReadPhyCmd(...)                            (icall_directAPI((uint32_t) IDX_HCI_LE_ReadPhyCmd , ##__VA_ARGS__))
#define HCI_LE_SetDefaultPhyCmd(...)                      (icall_directAPI((uint32_t) IDX_HCI_LE_SetDefaultPhyCmd , ##__VA_ARGS__))
#define HCI_LE_SetPhyCmd(...)                             (icall_directAPI((uint32_t) IDX_HCI_LE_SetPhyCmd , ##__VA_ARGS__))
#define HCI_LE_EnhancedRxTestCmd(...)                     (icall_directAPI((uint32_t) IDX_HCI_LE_EnhancedRxTestCmd , ##__VA_ARGS__))
#define HCI_LE_EnhancedTxTestCmd(...)                     (icall_directAPI((uint32_t) IDX_HCI_LE_EnhancedTxTestCmd , ##__VA_ARGS__))
#define HCI_LE_EnhancedCteRxTestCmd(...)                  (icall_directAPI((uint32_t) IDX_HCI_LE_EnhancedCteRxTestCmd , ##__VA_ARGS__))
#define HCI_LE_EnhancedCteTxTestCmd(...)                  (icall_directAPI((uint32_t) IDX_HCI_LE_EnhancedCteTxTestCmd , ##__VA_ARGS__))
/* HCI V5.0 - Advertising Extension */
/************************/
#define LE_SetAdvSetRandAddr(...)                         (icall_directAPI((uint32_t) IDX_LE_SetAdvSetRandAddr, ##__VA_ARGS__))
#define LE_SetExtAdvParams(...)                           (icall_directAPI((uint32_t) IDX_LE_SetExtAdvParams, ##__VA_ARGS__))
#define LE_SetExtAdvData(...)                             (icall_directAPI((uint32_t) IDX_LE_SetExtAdvData, ##__VA_ARGS__))
#define LE_SetExtScanRspData(...)                         (icall_directAPI((uint32_t) IDX_LE_SetExtScanRspData, ##__VA_ARGS__))
#define LE_SetExtAdvEnable(...)                           (icall_directAPI((uint32_t) IDX_LE_SetExtAdvEnable, ##__VA_ARGS__))
#define LE_RemoveAdvSet(...)                              (icall_directAPI((uint32_t) IDX_LE_RemoveAdvSet, ##__VA_ARGS__))
#define LE_ClearAdvSets(...)                              (icall_directAPI((uint32_t) IDX_LE_ClearAdvSets))
#define LE_SetExtScanParams(...)                          (icall_directAPI((uint32_t) IDX_LE_SetExtScanParams, ##__VA_ARGS__))
#define LE_SetExtScanEnable(...)                          (icall_directAPI((uint32_t) IDX_LE_SetExtScanEnable, ##__VA_ARGS__))
#define LE_ReadMaxAdvDataLen(...)                         (icall_directAPI((uint32_t) IDX_LE_ReadMaxAdvDataLen))
#define LE_ReadNumSupportedAdvSets(...)                   (icall_directAPI((uint32_t) IDX_LE_ReadNumSupportedAdvSets))
#define LL_AE_RegCBack(...)                               (icall_directAPI((uint32_t) IDX_LL_AE_RegCBack, ##__VA_ARGS__))
#define LE_ExtCreateConn(...)                             (icall_directAPI((uint32_t) IDX_LE_ExtCreateConn, ##__VA_ARGS__))
/* HCI V5.2 SC API */
/********************/
#define HCI_LE_GenerateDHKeyV2Cmd(...)                    (icall_directAPI((uint32_t) IDX_HCI_LE_GenerateDHKeyV2Cmd , ##__VA_ARGS__))
//ROLES
#define HCI_LE_SetAdvParamCmd(...)                        (icall_directAPI((uint32_t) IDX_HCI_LE_SetAdvParamCmd , ##__VA_ARGS__))
#define HCI_LE_SetAdvDataCmd(...)                         (icall_directAPI((uint32_t) IDX_HCI_LE_SetAdvDataCmd , ##__VA_ARGS__))
#define HCI_LE_SetScanRspDataCmd(...)                     (icall_directAPI((uint32_t) IDX_HCI_LE_SetScanRspDataCmd , ##__VA_ARGS__))
#define HCI_LE_SetAdvEnableCmd(...)                       (icall_directAPI((uint32_t) IDX_HCI_LE_SetAdvEnableCmd , ##__VA_ARGS__))
#define HCI_LE_SetScanParamCmd(...)                       (icall_directAPI((uint32_t) IDX_HCI_LE_SetScanParamCmd , ##__VA_ARGS__))
#define HCI_LE_SetScanEnableCmd(...)                      (icall_directAPI((uint32_t) IDX_HCI_LE_SetScanEnableCmd , ##__VA_ARGS__))
#define HCI_LE_CreateConnCancelCmd(...)                   (icall_directAPI((uint32_t) IDX_HCI_LE_CreateConnCancelCmd))
#define HCI_LE_StartEncyptCmd(...)                        (icall_directAPI((uint32_t) IDX_HCI_LE_StartEncyptCmd , ##__VA_ARGS__))
#define HCI_LE_ConnUpdateCmd(...)                         (icall_directAPI((uint32_t) IDX_HCI_LE_ConnUpdateCmd , ##__VA_ARGS__))
#define HCI_LE_LtkReqReplyCmd(...)                        (icall_directAPI((uint32_t) IDX_HCI_LE_LtkReqReplyCmd , ##__VA_ARGS__))
#define HCI_LE_LtkReqNegReplyCmd(...)                     (icall_directAPI((uint32_t) IDX_HCI_LE_LtkReqNegReplyCmd , ##__VA_ARGS__))
#define HCI_LE_ReadTxPowerCmd(...)                        (icall_directAPI((uint32_t) IDX_HCI_LE_ReadTxPowerCmd))
#define HCI_LE_ReadRfPathCompCmd(...)                     (icall_directAPI((uint32_t) IDX_HCI_LE_ReadRfPathCompCmd))
#define HCI_LE_WriteRfPathCompCmd(...)                    (icall_directAPI((uint32_t) IDX_HCI_LE_WriteRfPathCompCmd , ##__VA_ARGS__))

/* HCI V5.1 - Constant Tone Extension */
#define HCI_LE_SetConnectionCteReceiveParamsCmd(...)      (icall_directAPI((uint32_t) IDX_HCI_LE_SetConnectionCteReceiveParamsCmd , ##__VA_ARGS__))
#define HCI_LE_SetConnectionCteTransmitParamsCmd(...)     (icall_directAPI((uint32_t) IDX_HCI_LE_SetConnectionCteTransmitParamsCmd , ##__VA_ARGS__))
#define HCI_LE_SetConnectionCteRequestEnableCmd(...)      (icall_directAPI((uint32_t) IDX_HCI_LE_SetConnectionCteRequestEnableCmd , ##__VA_ARGS__))
#define HCI_LE_SetConnectionCteResponseEnableCmd(...)     (icall_directAPI((uint32_t) IDX_HCI_LE_SetConnectionCteResponseEnableCmd , ##__VA_ARGS__))
#define HCI_LE_ReadAntennaInformationCmd(...)             (icall_directAPI((uint32_t) IDX_HCI_LE_ReadAntennaInformationCmd))
/* Periodic Adv */
#define HCI_LE_SetPeriodicAdvParamsCmd(...)               (icall_directAPI((uint32_t) IDX_HCI_LE_SetPeriodicAdvParamsCmd , ##__VA_ARGS__))
#define HCI_LE_SetPeriodicAdvDataCmd(...)                 (icall_directAPI((uint32_t) IDX_HCI_LE_SetPeriodicAdvDataCmd , ##__VA_ARGS__))
#define HCI_LE_SetPeriodicAdvEnableCmd(...)               (icall_directAPI((uint32_t) IDX_HCI_LE_SetPeriodicAdvEnableCmd , ##__VA_ARGS__))
#define HCI_LE_PeriodicAdvCreateSyncCmd(...)              (icall_directAPI((uint32_t) IDX_HCI_LE_PeriodicAdvCreateSyncCmd , ##__VA_ARGS__))
#define HCI_LE_PeriodicAdvCreateSyncCancelCmd(...)        (icall_directAPI((uint32_t) IDX_HCI_LE_PeriodicAdvCreateSyncCancelCmd))
#define HCI_LE_PeriodicAdvTerminateSyncCmd(...)           (icall_directAPI((uint32_t) IDX_HCI_LE_PeriodicAdvTerminateSyncCmd , ##__VA_ARGS__))
#define HCI_LE_AddDeviceToPeriodicAdvListCmd(...)         (icall_directAPI((uint32_t) IDX_HCI_LE_AddDeviceToPeriodicAdvListCmd , ##__VA_ARGS__))
#define HCI_LE_RemoveDeviceFromPeriodicAdvListCmd(...)    (icall_directAPI((uint32_t) IDX_HCI_LE_RemoveDeviceFromPeriodicAdvListCmd , ##__VA_ARGS__))
#define HCI_LE_ClearPeriodicAdvListCmd(...)               (icall_directAPI((uint32_t) IDX_HCI_LE_ClearPeriodicAdvListCmd))
#define HCI_LE_ReadPeriodicAdvListSizeCmd(...)            (icall_directAPI((uint32_t) IDX_HCI_LE_ReadPeriodicAdvListSizeCmd))
#define HCI_LE_SetPeriodicAdvReceiveEnableCmd(...)        (icall_directAPI((uint32_t) IDX_HCI_LE_SetPeriodicAdvReceiveEnableCmd , ##__VA_ARGS__))
#define HCI_LE_SetExtScanRspData(...)                     (icall_directAPI((uint32_t) IDX_HCI_LE_SetExtScanRspData , ##__VA_ARGS__))
#define HCI_LE_SetExtAdvData(...)                         (icall_directAPI((uint32_t) IDX_HCI_LE_SetExtAdvData , ##__VA_ARGS__))
#define HCI_LE_SetAdvStatus(...)                          (icall_directAPI((uint32_t) IDX_HCI_LE_SetAdvStatus , ##__VA_ARGS__))

/* HCI POWER CONTROL API */
#define HCI_LE_ReadRemoteTransmitPowerLevelCmd(...)       (icall_directAPI((uint32_t) IDX_HCI_LE_ReadRemoteTransmitPowerLevelCmd , ##__VA_ARGS__))
#define HCI_LE_EnhancedReadTransmitPowerLevelCmd(...)     (icall_directAPI((uint32_t) IDX_HCI_LE_EnhancedReadTransmitPowerLevelCmd , ##__VA_ARGS__))
#define HCI_LE_SetTransmitPowerReportingEnableCmd(...)    (icall_directAPI((uint32_t) IDX_HCI_LE_SetTransmitPowerReportingEnableCmd , ##__VA_ARGS__))

/* Connection Handover */
#define LL_Handover_StartSN(...)                          (icall_directAPI((uint32_t) IDX_LL_Handover_StartSN , ##__VA_ARGS__))
#define LL_Handover_CloseSN(...)                          (icall_directAPI((uint32_t) IDX_LL_Handover_CloseSN , ##__VA_ARGS__))
#define LL_Handover_RegisterSNCb(...)                     (icall_directAPI((uint32_t) IDX_LL_Handover_RegisterSNCb , ##__VA_ARGS__))
#define LL_Handover_GetSNDataSize(...)                    (icall_directAPI((uint32_t) IDX_LL_Handover_GetSNDataSize , ##__VA_ARGS__))
#define LL_Handover_StartCN(...)                          (icall_directAPI((uint32_t) IDX_LL_Handover_StartCN , ##__VA_ARGS__))
#define LL_Handover_RegisterCNCb(...)                     (icall_directAPI((uint32_t) IDX_LL_Handover_RegisterCNCb , ##__VA_ARGS__))

/* HCI Extended API */
/********************/
#define HCI_EXT_SetFastTxResponseTimeCmd(...)                      (icall_directAPI((uint32_t) IDX_HCI_EXT_SetFastTxResponseTimeCmd , ##__VA_ARGS__))
#define HCI_EXT_SetPeripheralLatencyOverrideCmd(...)               (icall_directAPI((uint32_t) IDX_HCI_EXT_SetPeripheralLatencyOverrideCmd , ##__VA_ARGS__))
#define HCI_EXT_SetSlaveLatencyOverrideCmd(...)                    (icall_directAPI((uint32_t) IDX_HCI_EXT_SetPeripheralLatencyOverrideCmd , ##__VA_ARGS__))
#define HCI_EXT_SetTxPowerDbmCmd(...)                              (icall_directAPI((uint32_t) IDX_HCI_EXT_SetTxPowerDbmCmd , ##__VA_ARGS__))
#define HCI_EXT_BuildRevisionCmd(...)                              (icall_directAPI((uint32_t) IDX_HCI_EXT_BuildRevisionCmd , ##__VA_ARGS__))
#define HCI_EXT_DelaySleepCmd(...)                                 (icall_directAPI((uint32_t) IDX_HCI_EXT_DelaySleepCmd , ##__VA_ARGS__))
#define HCI_EXT_DecryptCmd(...)                                    (icall_directAPI((uint32_t) IDX_HCI_EXT_DecryptCmd , ##__VA_ARGS__))
#define HCI_EXT_EnablePTMCmd(...)                                  (icall_directAPI((uint32_t) IDX_HCI_EXT_EnablePTMCmd))
#define HCI_EXT_ModemTestTxCmd(...)                                (icall_directAPI((uint32_t) IDX_HCI_EXT_ModemTestTxCmd , ##__VA_ARGS__))
#define HCI_EXT_ModemHopTestTxCmd(...)                             (icall_directAPI((uint32_t) IDX_HCI_EXT_ModemHopTestTxCmd))
#define HCI_EXT_ModemTestRxCmd(...)                                (icall_directAPI((uint32_t) IDX_HCI_EXT_ModemTestRxCmd , ##__VA_ARGS__))
#define HCI_EXT_EnhancedModemTestTxCmd(...)                        (icall_directAPI((uint32_t) IDX_HCI_EXT_EnhancedModemTestTxCmd , ##__VA_ARGS__))
#define HCI_EXT_EnhancedModemHopTestTxCmd(...)                     (icall_directAPI((uint32_t) IDX_HCI_EXT_EnhancedModemHopTestTxCmd , ##__VA_ARGS__))
#define HCI_EXT_EnhancedModemTestRxCmd(...)                        (icall_directAPI((uint32_t) IDX_HCI_EXT_EnhancedModemTestRxCmd , ##__VA_ARGS__))
#define HCI_EXT_EndModemTestCmd(...)                               (icall_directAPI((uint32_t) IDX_HCI_EXT_EndModemTestCmd))
#define HCI_EXT_SetBDADDRCmd(...)                                  (icall_directAPI((uint32_t) IDX_HCI_EXT_SetBDADDRCmd , ##__VA_ARGS__))
#define HCI_EXT_ResetSystemCmd(...)                                (icall_directAPI((uint32_t) IDX_HCI_EXT_ResetSystemCmd , ##__VA_ARGS__))
#define HCI_EXT_SetLocalSupportedFeaturesCmd(...)                  (icall_directAPI((uint32_t) IDX_HCI_EXT_SetLocalSupportedFeaturesCmd , ##__VA_ARGS__))
#define HCI_EXT_SetMaxDtmTxPowerDbmCmd(...)                        (icall_directAPI((uint32_t) IDX_HCI_EXT_SetMaxDtmTxPowerDbmCmd , ##__VA_ARGS__))
#define HCI_EXT_SetRxGainCmd(...)                                  (icall_directAPI((uint32_t) IDX_HCI_EXT_SetRxGainCmd , ##__VA_ARGS__))
#define HCI_EXT_ClkDivOnHaltCmd(...)                               (icall_directAPI((uint32_t) IDX_HCI_EXT_ClkDivOnHaltCmd , ##__VA_ARGS__))
#define HCI_EXT_DeclareNvUsageCmd(...)                             (icall_directAPI((uint32_t) IDX_HCI_EXT_DeclareNvUsageCmd , ##__VA_ARGS__))
#define HCI_EXT_MapPmIoPortCmd(...)                                (icall_directAPI((uint32_t) IDX_HCI_EXT_MapPmIoPortCmd , ##__VA_ARGS__))
#define HCI_EXT_DisconnectImmedCmd(...)                            (icall_directAPI((uint32_t) IDX_HCI_EXT_DisconnectImmedCmd , ##__VA_ARGS__))
#define HCI_EXT_PacketErrorRateCmd(...)                            (icall_directAPI((uint32_t) IDX_HCI_EXT_PacketErrorRateCmd , ##__VA_ARGS__))
#define HCI_EXT_NumComplPktsLimitCmd(...)                          (icall_directAPI((uint32_t) IDX_HCI_EXT_NumComplPktsLimitCmd , ##__VA_ARGS__))
#define HCI_EXT_OnePktPerEvtCmd(...)                               (icall_directAPI((uint32_t) IDX_HCI_EXT_OnePktPerEvtCmd , ##__VA_ARGS__))
#define HCI_EXT_SetSCACmd(...)                                     (icall_directAPI((uint32_t) IDX_HCI_EXT_SetSCACmd , ##__VA_ARGS__))
#define HCI_EXT_GetConnInfoCmd(...)                                (icall_directAPI((uint32_t) IDX_HCI_EXT_GetConnInfoCmd , ##__VA_ARGS__))
#define HCI_EXT_GetActiveConnInfoCmd(...)                          (icall_directAPI((uint32_t) IDX_HCI_EXT_GetActiveConnInfoCmd , ##__VA_ARGS__))
#define HCI_EXT_OverlappedProcessingCmd(...)                       (icall_directAPI((uint32_t) IDX_HCI_EXT_OverlappedProcessingCmd , ##__VA_ARGS__))
#define HCI_EXT_SetMaxDataLenCmd(...)                              (icall_directAPI((uint32_t) IDX_HCI_EXT_SetMaxDataLenCmd , ##__VA_ARGS__))
#define HCI_EXT_LLTestModeCmd(...)                                 (icall_directAPI((uint32_t) IDX_HCI_EXT_LLTestModeCmd , ##__VA_ARGS__))
#define HCI_EXT_PERbyChanCmd(...)                                  (icall_directAPI((uint32_t) IDX_HCI_EXT_PERbyChanCmd , ##__VA_ARGS__))
#define HCI_EXT_SetDtmTxPktCntCmd(...)                             (icall_directAPI((uint32_t) IDX_HCI_EXT_SetDtmTxPktCntCmd , ##__VA_ARGS__))
#define HCI_EXT_SetPinOutputCmd(...)                               (icall_directAPI((uint32_t) IDX_HCI_EXT_SetPinOutputCmd , ##__VA_ARGS__))
#define HCI_EXT_SetLocationingAccuracyCmd(...)                     (icall_directAPI((uint32_t) IDX_HCI_EXT_SetLocationingAccuracyCmd , ##__VA_ARGS__))
#define HCI_EXT_SetAdvSetRandAddrCmd(...)                          (icall_directAPI((uint32_t) IDX_HCI_EXT_SetAdvSetRandAddrCmd , ##__VA_ARGS__))
#define HCI_EXT_SetVirtualAdvAddrCmd(...)                          (icall_directAPI((uint32_t) IDX_HCI_EXT_SetVirtualAdvAddrCmd , ##__VA_ARGS__))
#define HCI_EXT_CoexEnableCmd(...)                                 (icall_directAPI((uint32_t) IDX_HCI_EXT_CoexEnableCmd , ##__VA_ARGS__))
#define HCI_EXT_SetExtScanChannels(...)                            (icall_directAPI((uint32_t) IDX_HCI_EXT_SetExtScanChannels , ##__VA_ARGS__))
#define HCI_EXT_SetQOSParameters(...)                              (icall_directAPI((uint32_t) IDX_HCI_EXT_SetQOSParameters , ##__VA_ARGS__))
#define HCI_EXT_SetQOSDefaultParameters(...)                       (icall_directAPI((uint32_t) IDX_HCI_EXT_SetQOSDefaultParameters , ##__VA_ARGS__))
#define HCI_EXT_SetHostDefChanClassificationCmd(...)               (icall_directAPI((uint32_t) IDX_HCI_EXT_SetHostDefChanClassificationCmd , ##__VA_ARGS__))
#define HCI_EXT_SetHostConnChanClassificationCmd(...)              (icall_directAPI((uint32_t) IDX_HCI_EXT_SetHostConnChanClassificationCmd , ##__VA_ARGS__))
#define HCI_EXT_GetRxStatisticsCmd(...)                            (icall_directAPI((uint32_t) IDX_HCI_EXT_GetRxStatisticsCmd , ##__VA_ARGS__))
#define HCI_EXT_GetTxStatisticsCmd(...)                            (icall_directAPI((uint32_t) IDX_HCI_EXT_GetTxStatisticsCmd , ##__VA_ARGS__))
#define HCI_EXT_GetCoexStatisticsCmd(...)                          (icall_directAPI((uint32_t) IDX_HCI_EXT_GetCoexStatisticsCmd , ##__VA_ARGS__))
#define HCI_EXT_SendPowerControlRequestCmd(...)                    (icall_directAPI((uint32_t) IDX_HCI_EXT_SendPowerControlRequestCmd , ##__VA_ARGS__))
#define HCI_EXT_SetDefaultAntenna(...)                             (icall_directAPI((uint32_t) IDX_HCI_EXT_SetDefaultAntenna , ##__VA_ARGS__))
#define HCI_EXT_RssiMon_RegCmd(...)                                (icall_directAPI((uint32_t) IDX_HCI_EXT_RssiMon_RegCmd , ##__VA_ARGS__))
#define HCI_EXT_RssiMon_UnregCmd(...)                              (icall_directAPI((uint32_t) IDX_HCI_EXT_RssiMon_UnregCmd , ##__VA_ARGS__))
#define HCI_EXT_RssiMon_SetConfigCmd(...)                          (icall_directAPI((uint32_t) IDX_HCI_EXT_RssiMon_SetConfigCmd , ##__VA_ARGS__))
#define HCI_EXT_RssiMon_GetConfigCmd(...)                          (icall_directAPI((uint32_t) IDX_HCI_EXT_RssiMon_GetConfigCmd))
#define HCI_EXT_RssiMon_GetRssiStatCmd(...)                        (icall_directAPI((uint32_t) IDX_HCI_EXT_RssiMon_GetRssiStatCmd , ##__VA_ARGS__))

/* HCI API Cont'd */
/******************/
/* TODO this may not be needed */
#define HCI_ProcessRawMessage(...)                                 (icall_directAPI((uint32_t) IDX_HCI_ProcessRawMessage , ##__VA_ARGS__))


#define HCI_SendDataPkt(...)                                       (icall_directAPI((uint32_t) IDX_HCI_SendDataPkt , ##__VA_ARGS__))
#define HCI_TL_getCmdResponderID(...)                              (icall_directAPI((uint32_t) IDX_HCI_TL_getCmdResponderID, ##__VA_ARGS__))
#define HCI_CommandStatusEvent(...)                                (icall_directAPI((uint32_t) IDX_HCI_CommandStatusEvent, ##__VA_ARGS__))
#define HCI_CommandCompleteEvent(...)                              (icall_directAPI((uint32_t) IDX_HCI_CommandCompleteEvent, ##__VA_ARGS__))
#define HCI_VendorSpecifcCommandCompleteEvent(...)                 (icall_directAPI((uint32_t) IDX_HCI_VendorSpecifcCommandCompleteEvent , ##__VA_ARGS__))
#define HCI_bm_alloc(...)                                          (icall_directAPI((uint32_t) IDX_HCI_bm_alloc , ##__VA_ARGS__))
#define HCI_bm_free(...)                                           (icall_directAPI((uint32_t) IDX_HCI_bm_free , ##__VA_ARGS__))
/* the following API are not available for now */

#define HCI_TestAppTaskRegister(...)                               (AssertHandler(0,0)) // icall_directAPI((uint32_t) IDX_HCI_TestAppTaskRegister , ##__VA_ARGS__) // =>need taskId
#define HCI_GAPTaskRegister(...)                                   (AssertHandler(0,0)) // icall_directAPI((uint32_t) IDX_HCI_GAPTaskRegister , ##__VA_ARGS__) // =>need taskId
#define HCI_L2CAPTaskRegister(...)                                 (AssertHandler(0,0)) // icall_directAPI((uint32_t) IDX_HCI_L2CAPTaskRegister , ##__VA_ARGS__) // =>need taskId
#define HCI_SMPTaskRegister(...)                                   (AssertHandler(0,0)) // icall_directAPI((uint32_t) IDX_HCI_SMPTaskRegister , ##__VA_ARGS__) // =>need taskId
#define HCI_ExtTaskRegister(...)                                   (AssertHandler(0,0)) // icall_directAPI((uint32_t) IDX_HCI_ExtTaskRegister , ##__VA_ARGS__) // =>need taskId


/* SNV API */
/***********/
#define osal_snv_read(...)      (icall_directAPI((uint32_t) IDX_osal_snv_read , ##__VA_ARGS__))
#define osal_snv_write(...)     (icall_directAPI((uint32_t) IDX_osal_snv_write , ##__VA_ARGS__))
/* the following API are not available for now */
#define osal_snv_init(...)      (AssertHandler(0,0))
#define osal_snv_ext_write(...) (AssertHandler(0,0))
#define osal_snv_compact(...)   (AssertHandler(0,0))


/* UTIL API */
/************/
#define NPI_RegisterTask(taskID) (icall_directAPI((uint32_t) IDX_NPI_RegisterTask, ICall_getLocalMsgEntityId(ICALL_SERVICE_CLASS_BLE_MSG, taskID)))
#define buildRevision(...)       (icall_directAPI((uint32_t) IDX_buildRevision , ##__VA_ARGS__))

/* GAP-GATT service API */
/************************/
#define GGS_SetParameter(...)   (icall_directAPI((uint32_t) IDX_GGS_SetParameter  , ##__VA_ARGS__))
#define GGS_AddService(...)     (icall_directAPI((uint32_t) IDX_GGS_AddService , ##__VA_ARGS__))
#define GGS_GetParameter(...)   (icall_directAPI((uint32_t) IDX_GGS_GetParameter , ##__VA_ARGS__))
#define GGS_RegisterAppCBs(...) (icall_directAPI((uint32_t) IDX_GGS_RegisterAppCBs , ##__VA_ARGS__))
#define GGS_SetParamValue(...)  (icall_directAPI((uint32_t) IDX_GGS_SetParamValue , ##__VA_ARGS__))
#define GGS_GetParamValue(...)  (icall_directAPI((uint32_t) IDX_GGS_GetParamValue))
/* the following API are not available for now */
#define GGS_DelService(...)     (AssertHandler(0,0))


/* GAP Bond Manager API */
/************************/
#define GAPBondMgr_SetParameter(...)            (icall_directAPI((uint32_t) IDX_GAPBondMgr_SetParameter , ##__VA_ARGS__))
#define GAPBondMgr_GetParameter(...)            (icall_directAPI((uint32_t) IDX_GAPBondMgr_GetParameter , ##__VA_ARGS__))
#define GAPBondMgr_Pair(...)                    (icall_directAPI((uint32_t) IDX_GAPBondMgr_Pair , ##__VA_ARGS__))
#define GAPBondMgr_ServiceChangeInd(...)        (icall_directAPI((uint32_t) IDX_GAPBondMgr_ServiceChangeInd , ##__VA_ARGS__))
#define GAPBondMgr_Register(...)                (icall_directAPI((uint32_t) IDX_GAPBondMgr_Register , ##__VA_ARGS__))
#define GAPBondMgr_PasscodeRsp(...)             (icall_directAPI((uint32_t) IDX_GAPBondMgr_PasscodeRsp , ##__VA_ARGS__))
#define GAPBondMgr_FindAddr(...)                (icall_directAPI((uint32_t) IDX_GAPBondMgr_FindAddr , ##__VA_ARGS__))
#define GAPBondMgr_ProcessGAPMsg(...)           (icall_directAPI((uint32_t) IDX_GAPBondMgr_ProcessGAPMsg , ##__VA_ARGS__))
#define GAPBondMgr_SCGetLocalOOBParameters(...) (icall_directAPI((uint32_t) IDX_GAPBondMgr_SCGetLocalOOBParameters, ##__VA_ARGS__))
#define GAPBondMgr_SCSetRemoteOOBParameters(...)(icall_directAPI((uint32_t) IDX_GAPBondMgr_SCSetRemoteOOBParameters, ##__VA_ARGS__))
#define GAPBondMgr_GenerateEccKeys(...)         (icall_directAPI((uint32_t) IDX_GAPBondMgr_GenerateEccKeys))
#define GapBondMgr_readBondFromNV(...)          (icall_directAPI((uint32_t) IDX_GapBondMgr_readBondFromNV, ##__VA_ARGS__))
#define GapBondMgr_writeBondToNv(...)           (icall_directAPI((uint32_t) IDX_GapBondMgr_writeBondToNv, ##__VA_ARGS__))
#define GapBondMgr_GetPrevAuth(...)             (icall_directAPI((uint32_t) IDX_GapBondMgr_GetPrevAuth, ##__VA_ARGS__))
#define GapBondMgr_StartEnc(...)                (icall_directAPI((uint32_t) IDX_GapBondMgr_StartEnc, ##__VA_ARGS__))

/* the following API are not available for now */
#define GAPBondMgr_ReadCentAddrResChar(...)     (AssertHandler(0,0))


/* GAP API */
/***********/
// Initialization and Configuration
#define GAP_DeviceInit(profileRole, taskID, addrType, pRandomAddr)                      (icall_directAPI((uint32_t) IDX_GAP_DeviceInit, profileRole, ICall_getLocalMsgEntityId(ICALL_SERVICE_CLASS_BLE_MSG, taskID), addrType, pRandomAddr))
#define GAP_RegisterForMsgs(taskID)                                                     (icall_directAPI((uint32_t) IDX_GAP_RegisterForMsgs, ICall_getLocalMsgEntityId(ICALL_SERVICE_CLASS_BLE_MSG, taskID)))

#define GAP_SetParamValue(...)                                                          (icall_directAPI((uint32_t) IDX_GAP_SetParamValue , ##__VA_ARGS__))
#define GAP_GetParamValue(...)                                                          (icall_directAPI((uint32_t) IDX_GAP_GetParamValue , ##__VA_ARGS__))
#define GapConfig_SetParameter(...)                                                     (icall_directAPI((uint32_t) IDX_GapConfig_SetParameter , ##__VA_ARGS__))
#define GAP_GetIRK(...)                                                                 (icall_directAPI((uint32_t) IDX_GAP_GetIRK))
#define GAP_GetSRK(...)                                                                 (icall_directAPI((uint32_t) IDX_GAP_GetSRK))
/* the following API are not available for now */
#define GAP_RegisterBondMgrCBs(...)                                                     (AssertHandler(0,0))

// Link Establishment
#define GAP_TerminateLinkReq(...)                                                       (icall_directAPI((uint32_t) IDX_GAP_TerminateLinkReq , ##__VA_ARGS__))
#define GAP_UpdateLinkParamReq(...)                                                     (icall_directAPI((uint32_t) IDX_GAP_UpdateLinkParamReq , ##__VA_ARGS__))
#define GAP_UpdateLinkParamReqReply(...)                                                (icall_directAPI((uint32_t) IDX_GAP_UpdateLinkParamReqReply , ##__VA_ARGS__))
#define Gap_RegisterConnEventCb(...)                                                    (icall_directAPI((uint32_t) IDX_Gap_RegisterConnEventCb , ##__VA_ARGS__))
/* the following API are not available for now */
#define GAP_NumActiveConnections(...)            AssertHandler(0,0)

// Pairing
#define GAP_Authenticate(...)                                                           (icall_directAPI((uint32_t) IDX_GAP_Authenticate , ##__VA_ARGS__))
#define GAP_TerminateAuth(...)                                                          (icall_directAPI((uint32_t) IDX_GAP_TerminateAuth , ##__VA_ARGS__))
#define GAP_PasskeyUpdate(...)                                                          (icall_directAPI((uint32_t) IDX_GAP_PasskeyUpdate , ##__VA_ARGS__))
#define GAP_SendPeripheralSecurityRequest(...)                                          (icall_directAPI((uint32_t) IDX_GAP_SendPeripheralSecurityRequest , ##__VA_ARGS__))
#define GAP_SendSlaveSecurityRequest(...)                                               (icall_directAPI((uint32_t) IDX_GAP_SendPeripheralSecurityRequest , ##__VA_ARGS__))
#define GAP_Signable(...)                                                               (icall_directAPI((uint32_t) IDX_GAP_Signable , ##__VA_ARGS__))
#define GAP_Bond(...)                                                                   (icall_directAPI((uint32_t) IDX_GAP_Bond , ##__VA_ARGS__))
#define Gap_ReplyToLTKReq(...)                                                          (icall_directAPI((uint32_t) IDX_Gap_ReplyToLTKReq , ##__VA_ARGS__))
/* the following API are not available for now */
#define GAP_PasscodeUpdate(...)                                                         (AssertHandler(0,0))

// GAP Advertiser module
#define GapAdv_create(...)                                                              (icall_directAPI((uint32_t) IDX_GapAdv_create , ##__VA_ARGS__))
#define GapAdv_destroy(...)                                                             (icall_directAPI((uint32_t) IDX_GapAdv_destroy , ##__VA_ARGS__))
#define GapAdv_enable(...)                                                              (icall_directAPI((uint32_t) IDX_GapAdv_enable , ##__VA_ARGS__))
#define GapAdv_disable(...)                                                             (icall_directAPI((uint32_t) IDX_GapAdv_disable , ##__VA_ARGS__))
#define GapAdv_setParam(...)                                                            (icall_directAPI((uint32_t) IDX_GapAdv_setParam , ##__VA_ARGS__))
#define GapAdv_getParam(...)                                                            (icall_directAPI((uint32_t) IDX_GapAdv_getParam , ##__VA_ARGS__))
#define GapAdv_getBuffer(...)                                                           (icall_directAPI((uint32_t) IDX_GapAdv_getBuffer , ##__VA_ARGS__))
#define GapAdv_loadByHandle(...)                                                        (icall_directAPI((uint32_t) IDX_GapAdv_loadByHandle , ##__VA_ARGS__))
#define GapAdv_loadByBuffer(...)                                                        (icall_directAPI((uint32_t) IDX_GapAdv_loadByBuffer , ##__VA_ARGS__))
#define GapAdv_prepareLoadByHandle(...)                                                 (icall_directAPI((uint32_t) IDX_GapAdv_prepareLoadByHandle , ##__VA_ARGS__))
#define GapAdv_prepareLoadByBuffer(...)                                                 (icall_directAPI((uint32_t) IDX_GapAdv_prepareLoadByBuffer , ##__VA_ARGS__))
#define GapAdv_abortLoad(...)                                                           (icall_directAPI((uint32_t) IDX_GapAdv_abortLoad , ##__VA_ARGS__))
#define GapAdv_setEventMask(...)                                                        (icall_directAPI((uint32_t) IDX_GapAdv_setEventMask , ##__VA_ARGS__))
#define GapAdv_setVirtualAdvAddr(...)                                                   (icall_directAPI((uint32_t) IDX_GapAdv_setVirtualAdvAddr , ##__VA_ARGS__))
#define GapAdv_SetPeriodicAdvParams(...)                                                (icall_directAPI((uint32_t) IDX_GapAdv_SetPeriodicAdvParams , ##__VA_ARGS__))
#define GapAdv_SetPeriodicAdvData(...)                                                  (icall_directAPI((uint32_t) IDX_GapAdv_SetPeriodicAdvData , ##__VA_ARGS__))
#define GapAdv_SetPeriodicAdvEnable(...)                                                (icall_directAPI((uint32_t) IDX_GapAdv_SetPeriodicAdvEnable , ##__VA_ARGS__))

// GAP Scanner module
#define GapScan_registerCb(...)                                                         (icall_directAPI((uint32_t) IDX_GapScan_registerCb , ##__VA_ARGS__))
#define GapScan_setPhyParams(...)                                                       (icall_directAPI((uint32_t) IDX_GapScan_setPhyParams , ##__VA_ARGS__))
#define GapScan_getPhyParams(...)                                                       (icall_directAPI((uint32_t) IDX_GapScan_getPhyParams , ##__VA_ARGS__))
#define GapScan_setParam(...)                                                           (icall_directAPI((uint32_t) IDX_GapScan_setParam , ##__VA_ARGS__))
#define GapScan_getParam(...)                                                           (icall_directAPI((uint32_t) IDX_GapScan_getParam , ##__VA_ARGS__))
#define GapScan_setEventMask(...)                                                       (icall_directAPI((uint32_t) IDX_GapScan_setEventMask , ##__VA_ARGS__))
#define GapScan_enable(...)                                                             (icall_directAPI((uint32_t) IDX_GapScan_enable , ##__VA_ARGS__))
#define GapScan_disable(...)                                                            (icall_directAPI((uint32_t) IDX_GapScan_disable))
#define GapScan_getAdvReport(...)                                                       (icall_directAPI((uint32_t) IDX_GapScan_getAdvReport , ##__VA_ARGS__))
#define GapScan_discardAdvReportList(...)                                               (icall_directAPI((uint32_t) IDX_GapScan_discardAdvReportList , ##__VA_ARGS__))
#define GapScan_PeriodicAdvCreateSync(...)                                              (icall_directAPI((uint32_t) IDX_GapScan_PeriodicAdvCreateSync , ##__VA_ARGS__))
#define GapScan_PeriodicAdvCreateSyncCancel(...)                                        (icall_directAPI((uint32_t) IDX_GapScan_PeriodicAdvCreateSyncCancel , ##__VA_ARGS__))
#define GapScan_PeriodicAdvTerminateSync(...)                                           (icall_directAPI((uint32_t) IDX_GapScan_PeriodicAdvTerminateSync , ##__VA_ARGS__))
#define GapScan_SetPeriodicAdvReceiveEnable(...)                                        (icall_directAPI((uint32_t) IDX_GapScan_SetPeriodicAdvReceiveEnable , ##__VA_ARGS__))
#define GapScan_AddDeviceToPeriodicAdvList(...)                                         (icall_directAPI((uint32_t) IDX_GapScan_AddDeviceToPeriodicAdvList , ##__VA_ARGS__))
#define GapScan_RemoveDeviceFromPeriodicAdvList(...)                                    (icall_directAPI((uint32_t) IDX_GapScan_RemoveDeviceFromPeriodicAdvList , ##__VA_ARGS__))
#define GapScan_ReadPeriodicAdvListSize(...)                                            (icall_directAPI((uint32_t) IDX_GapScan_ReadPeriodicAdvListSize))
#define GapScan_ClearPeriodicAdvList(...)                                               (icall_directAPI((uint32_t) IDX_GapScan_ClearPeriodicAdvList))

// GAP Initiator Module
#define GapInit_setPhyParam(...)                                                        (icall_directAPI((uint32_t) IDX_GapInit_setPhyParam , ##__VA_ARGS__))
#define GapInit_getPhyParam(...)                                                        (icall_directAPI((uint32_t) IDX_GapInit_getPhyParam , ##__VA_ARGS__))
#define GapInit_connect(...)                                                            (icall_directAPI((uint32_t) IDX_GapInit_connect , ##__VA_ARGS__))
#define GapInit_connectAl(...)                                                          (icall_directAPI((uint32_t) IDX_GapInit_connectAl , ##__VA_ARGS__))
#define GapInit_connectWl(...)                                                          (icall_directAPI((uint32_t) IDX_GapInit_connectAl , ##__VA_ARGS__))
#define GapInit_cancelConnect(...)                                                      (icall_directAPI((uint32_t) IDX_GapInit_cancelConnect))

/* L2CAP API */
/*************/
#define L2CAP_DeregisterPsm(taskID, psm)                         (icall_directAPI((uint32_t) IDX_L2CAP_DeregisterPsm, ICall_getLocalMsgEntityId(ICALL_SERVICE_CLASS_BLE_MSG, taskID), psm))
#define L2CAP_ConnParamUpdateReq(connHandle, pUpdateReq, taskID) (icall_directAPI((uint32_t) IDX_L2CAP_ConnParamUpdateReq, connHandle, pUpdateReq, ICall_getLocalMsgEntityId(ICALL_SERVICE_CLASS_BLE_MSG, taskID)))
#define L2CAP_RegisterFlowCtrlTask(taskID)                       (icall_directAPI((uint32_t) IDX_L2CAP_RegisterFlowCtrlTask, ICall_getLocalMsgEntityId(ICALL_SERVICE_CLASS_BLE_MSG, taskID)))
#define L2CAP_InfoReq(connHandle, pInfoReq, taskID)              (icall_directAPI((uint32_t) IDX_L2CAP_InfoReq, connHandle, pInfoReq, ICall_getLocalMsgEntityId(ICALL_SERVICE_CLASS_BLE_MSG, taskID)))

#define L2CAP_RegisterPsm(...)                                   (icall_directAPI((uint32_t) IDX_L2CAP_RegisterPsm , ##__VA_ARGS__))
#define L2CAP_PsmInfo(...)                                       (icall_directAPI((uint32_t) IDX_L2CAP_PsmInfo , ##__VA_ARGS__))
#define L2CAP_PsmChannels(...)                                   (icall_directAPI((uint32_t) IDX_L2CAP_PsmChannels , ##__VA_ARGS__))
#define L2CAP_ChannelInfo(...)                                   (icall_directAPI((uint32_t) IDX_L2CAP_ChannelInfo , ##__VA_ARGS__))
#define L2CAP_ConnectReq(...)                                    (icall_directAPI((uint32_t) IDX_L2CAP_ConnectReq , ##__VA_ARGS__))
#define L2CAP_ConnectRsp(...)                                    (icall_directAPI((uint32_t) IDX_L2CAP_ConnectRsp , ##__VA_ARGS__))
#define L2CAP_DisconnectReq(...)                                 (icall_directAPI((uint32_t) IDX_L2CAP_DisconnectReq , ##__VA_ARGS__))
#define L2CAP_FlowCtrlCredit(...)                                (icall_directAPI((uint32_t) IDX_L2CAP_FlowCtrlCredit , ##__VA_ARGS__))
#define L2CAP_SendSDU(...)                                       (icall_directAPI((uint32_t) IDX_L2CAP_SendSDU , ##__VA_ARGS__))

/* the following API are not available for now */
#define L2CAP_CmdReject(...)                                     (icall_directAPI((uint32_t) IDX_L2CAP_CmdReject , ##__VA_ARGS__))                    //(AssertHandler(0,0)) //icall_directAPI((uint32_t) IDX_L2CAP_CmdReject , ##__VA_ARGS__)
#define L2CAP_ConnParamUpdateRsp(...)                            (icall_directAPI((uint32_t) IDX_L2CAP_ConnParamUpdateRsp , ##__VA_ARGS__))           //(AssertHandler(0,0)) //icall_directAPI((uint32_t) IDX_L2CAP_ConnParamUpdateRsp , ##__VA_ARGS__)
#define L2CAP_SetUserConfig(...)                                 (icall_directAPI((uint32_t) IDX_L2CAP_SetUserConfig , ##__VA_ARGS__))                //(AssertHandler(0,0)) //icall_directAPI((uint32_t) IDX_L2CAP_SetUserConfig , ##__VA_ARGS__)
#define L2CAP_SetBufSize(...)                                    (icall_directAPI((uint32_t) IDX_L2CAP_SetBufSize , ##__VA_ARGS__))                   //(AssertHandler(0,0)) //icall_directAPI((uint32_t) IDX_L2CAP_SetBufSize , ##__VA_ARGS__)
#define L2CAP_GetMTU(...)                                        (icall_directAPI((uint32_t) IDX_L2CAP_GetMTU))                       //(AssertHandler(0,0)) //icall_directAPI((uint32_t) IDX_L2CAP_GetMTU , ##__VA_ARGS__)

// L2CAP Utility API Functions
#define L2CAP_SetParamValue(...)                                 (icall_directAPI((uint32_t) IDX_L2CAP_SetParamValue , ##__VA_ARGS__))
#define L2CAP_GetParamValue(...)                                 (icall_directAPI((uint32_t) IDX_L2CAP_GetParamValue))
/* the following API are not available for now */
#define L2CAP_RegisterApp(...)                                   (AssertHandler(0,0)) // =>need taskId
#define L2CAP_SendData(...)                                      (AssertHandler(0,0))
#define L2CAP_SetControllerToHostFlowCtrl(...)                   (AssertHandler(0,0)) //icall_directAPI((uint32_t) IDX_L2CAP_SetControllerToHostFlowCtrl , ##__VA_ARGS__)
#define L2CAP_HostNumCompletedPkts(...)                          (AssertHandler(0,0)) //icall_directAPI((uint32_t) IDX_L2CAP_HostNumCompletedPkts , ##__VA_ARGS__)


/* GATT API */
/************/
// GATT Client
#define GATT_RegisterForReq(taskID)                    (icall_directAPI((uint32_t) IDX_GATT_RegisterForReq, ICall_getLocalMsgEntityId(ICALL_SERVICE_CLASS_BLE_MSG, taskID)))

/* the following API are not available for now */
#define GATT_InitServer(...)                           (AssertHandler(0,0))
#define GATT_VerifyReadPermissions(...)                (AssertHandler(0,0))
#define GATT_VerifyWritePermissions(...)               (AssertHandler(0,0))
#define GATT_ServiceChangedInd(...)                    (AssertHandler(0,0))// =>need taskId
#define GATT_FindHandleUUID(...)                       (AssertHandler(0,0))
#define GATT_FindNextAttr(...)                         (AssertHandler(0,0))
#define GATT_ServiceNumAttrs(...)                      (AssertHandler(0,0))
#define GATT_ServiceEncKeySize(...)                    (AssertHandler(0,0))


// GATT Client and Server Common APIs
/* the following API are not available for now */
#define GATT_NotifyEvent(...)                                                   (AssertHandler(0,0))


//GATT Buffer Management APIs
// Those are enabled in a different way...,
// do not uncomment
//#define GATT_bm_alloc(...)                                                    (icall_directAPI((uint32_t) GATT_bm_alloc))
//#define GATT_bm_free(...)                                                     (icall_directAPI((uint32_t) GATT_bm_free))


/* GATT SERVER APPLICATION API */
/*******************************/
#define GATTQual_AddService(...)                                (icall_directAPI((uint32_t) IDX_GATTQual_AddService , ##__VA_ARGS__))
#define GATTTest_AddService(...)                                (icall_directAPI((uint32_t) IDX_GATTTest_AddService , ##__VA_ARGS__))
#define GATT_SetNextHandle(...)                                 (icall_directAPI((uint32_t) IDX_GATT_SetNextHandle , ##__VA_ARGS__))

/* the following API are not available for now */
#define GATTServApp_DelService(...)                             User_error_GATTServApp_DelService_called_by_application
#define GATTServApp_UpdateCharCfg(...)                          User_error_GATTServApp_UpdateCharCfg_called_by_application
#define GATTServApp_ReadAttr(...)                               User_error_GATTServApp_ReadAttr_called_by_application
#define GATTServApp_WriteAttr(...)                              User_error_GATTServApp_WriteAttr_called_by_application
// part of the application, do not uncomment :
// #define GATTServApp_InitCharCfg(...)                         (AssertHandler(0,0))
// #define GATTServApp_ProcessCharCfg(...)                      (AssertHandler(0,0)) // => need taskId
// #define GATTServApp_FindAttr(...)                            (AssertHandler(0,0))
// #define GATTServApp_ProcessCCCWriteReq(...)                  (AssertHandler(0,0))
// #define GATTServApp_ReadCharCfg(...)                         (AssertHandler(0,0))
// #define GATTServApp_WriteCharCfg(...)                        (AssertHandler(0,0))


/* LINK DB API */
/***************/
#define linkDB_NumActive(...)                    (icall_directAPI((uint32_t) IDX_linkDB_NumActive))
#define linkDB_GetInfo(...)                      (icall_directAPI((uint32_t) IDX_linkDB_GetInfo , ##__VA_ARGS__))
#define linkDB_State(...)                        (icall_directAPI((uint32_t) IDX_linkDB_State , ##__VA_ARGS__))
#define linkDB_NumConns(...)                     (icall_directAPI((uint32_t) IDX_linkDB_NumConns))
/* the following API are not available for now */
#define linkDB_Init(...)                         (AssertHandler(0,0))
#define linkDB_Register(...)                     (AssertHandler(0,0))
#define linkDB_Add(...)                          (AssertHandler(0,0))
#define linkDB_Remove(...)                       (AssertHandler(0,0))
#define linkDB_Update(...)                       (AssertHandler(0,0))
#define linkDB_UpdateMTU(...)                    (AssertHandler(0,0))
#define linkDB_MTU(...)                          (AssertHandler(0,0))
#define linkDB_Find(...)                         (AssertHandler(0,0))
#define linkDB_Authen(...)                       (AssertHandler(0,0))
#define linkDB_Role(...)                         (AssertHandler(0,0))
#define linkDB_PerformFunc(...)                  (AssertHandler(0,0))
#define linkDB_SecurityModeSCOnly(...)           (AssertHandler(0,0))

/* ATT API */
/***********/
//Attribute Client Public APIs
#define ATT_HandleValueCfm(...)                 (icall_directAPI((uint32_t) IDX_ATT_HandleValueCfm , ##__VA_ARGS__))
#define ATT_ErrorRsp(...)                       (icall_directAPI((uint32_t) IDX_ATT_ErrorRsp , ##__VA_ARGS__))
#define ATT_ReadBlobRsp(...)                    (icall_directAPI((uint32_t) IDX_ATT_ReadBlobRsp , ##__VA_ARGS__))
#define ATT_ExecuteWriteRsp(...)                (icall_directAPI((uint32_t) IDX_ATT_ExecuteWriteRsp , ##__VA_ARGS__))
#define ATT_WriteRsp(...)                       (icall_directAPI((uint32_t) IDX_ATT_WriteRsp , ##__VA_ARGS__))
#define ATT_ReadRsp(...)                        (icall_directAPI((uint32_t) IDX_ATT_ReadRsp , ##__VA_ARGS__))
#define ATT_ParseExchangeMTUReq(...)            (icall_directAPI((uint32_t) IDX_ATT_ParseExchangeMTUReq , ##__VA_ARGS__))

   /* the following API are not available for now */
#define ATT_ExchangeMTUReq(...)                 (AssertHandler(0,0))
#define ATT_FindInfoReq(...)                    (AssertHandler(0,0))
#define ATT_FindByTypeValueReq(...)             (AssertHandler(0,0))
#define ATT_ReadByTypeReq(...)                  (AssertHandler(0,0))
#define ATT_ReadReq(...)                        (AssertHandler(0,0))
#define ATT_ReadBlobReq(...)                    (AssertHandler(0,0))
#define ATT_ReadMultiReq(...)                   (AssertHandler(0,0))
#define ATT_ReadByGrpTypeReq(...)               (AssertHandler(0,0))
#define ATT_WriteReq(...)                       (AssertHandler(0,0))
#define ATT_PrepareWriteReq(...)                (AssertHandler(0,0))
#define ATT_ExecuteWriteReq(...)                (AssertHandler(0,0))

//Attribute Server Public APIs
#define ATT_ExchangeMTURsp(...)                 (icall_directAPI((uint32_t) IDX_ATT_ExchangeMTURsp , ##__VA_ARGS__))
#define ATT_FindInfoRsp(...)                    (icall_directAPI((uint32_t) IDX_ATT_FindInfoRsp , ##__VA_ARGS__))
#define ATT_FindByTypeValueRsp(...)             (icall_directAPI((uint32_t) IDX_ATT_FindByTypeValueRsp , ##__VA_ARGS__))
#define ATT_ReadByTypeRsp(...)                  (icall_directAPI((uint32_t) IDX_ATT_ReadByTypeRsp , ##__VA_ARGS__))
#define ATT_ReadMultiRsp(...)                   (icall_directAPI((uint32_t) IDX_ATT_ReadMultiRsp , ##__VA_ARGS__))
#define ATT_ReadByGrpTypeRsp(...)               (icall_directAPI((uint32_t) IDX_ATT_ReadByGrpTypeRsp , ##__VA_ARGS__))
#define ATT_PrepareWriteRsp(...)                (icall_directAPI((uint32_t) IDX_ATT_PrepareWriteRsp , ##__VA_ARGS__))
/* the following API are not available for now */
#define ATT_HandleValueNoti(...)                (AssertHandler(0,0)
#define ATT_HandleValueInd(...)                 (AssertHandler(0,0)


// Attribute Common Public APIs
#define ATT_SetParamValue(...)                  (icall_directAPI((uint32_t) IDX_ATT_SetParamValue , ##__VA_ARGS__))
#define ATT_GetParamValue(...)                  (icall_directAPI((uint32_t) IDX_ATT_GetParamValue))
/* the following API are not available for now */
#define ATT_UpdateMTU(...)                      (AssertHandler(0,0))
#define ATT_GetMTU(...)                         (AssertHandler(0,0))
#define ATT_RegisterServer(...)                 (AssertHandler(0,0))
#define ATT_RegisterClient(...)                 (AssertHandler(0,0))

/* Security Manager API */
/************************/
#define SM_GetScConfirmOob(...)                 (icall_directAPI((uint32_t) IDX_SM_GetScConfirmOob , ##__VA_ARGS__))
#define SM_GetEccKeys(...)                      (icall_directAPI((uint32_t) IDX_SM_GetEccKeys))
#define SM_GetDHKey(...)                        (icall_directAPI((uint32_t) IDX_SM_GetDHKey , ##__VA_ARGS__))
#define SM_RegisterTask(taskID)                 (icall_directAPI((uint32_t) IDX_SM_RegisterTask, ICall_getLocalMsgEntityId(ICALL_SERVICE_CLASS_BLE_MSG, taskID)))
#define SM_GenerateRandBuf(...)                 (icall_directAPI((uint32_t) IDX_SM_GenerateRandBuf, ##__VA_ARGS__))
#define SM_AuthenticatedPairingOnlyMode(...)    (icall_directAPI((uint32_t) IDX_SM_AuthenticatedPairingOnlyMode))

/* Osal Bufmgr API      */
/************************/
#define osal_bm_reg_callback(...)               (icall_directAPI((uint32_t) IDX_osal_bm_reg_callback , ##__VA_ARGS__))

/* HCI CS API */
/**************/
#define HCI_CS_ReadLocalSupportedCapabilities(...)    (icall_directAPI((uint32_t) IDX_HCI_LE_CS_ReadLocalSupportedCapabilities))
#define HCI_CS_ReadRemoteSupportedCapabilities(...)   (icall_directAPI((uint32_t) IDX_HCI_LE_CS_ReadRemoteSupportedCapabilities , ##__VA_ARGS__))
#define HCI_LE_CS_CreateConfig(...)                   (icall_directAPI((uint32_t) IDX_HCI_LE_CS_CreateConfig , ##__VA_ARGS__))
#define HCI_LE_CS_SecurityEnable(...)                 (icall_directAPI((uint32_t) IDX_HCI_LE_CS_SecurityEnable , ##__VA_ARGS__))
#define HCI_LE_CS_SetDefaultSettings(...)             (icall_directAPI((uint32_t) IDX_HCI_LE_CS_SetDefaultSettings , ##__VA_ARGS__))
#define HCI_LE_CS_ReadRemoteFAETable(...)             (icall_directAPI((uint32_t) IDX_HCI_LE_CS_ReadRemoteFAETable , ##__VA_ARGS__))
#define HCI_LE_CS_WriteRemoteFAETable(...)            (icall_directAPI((uint32_t) IDX_HCI_LE_CS_WriteRemoteFAETable , ##__VA_ARGS__))
#define HCI_LE_CS_RemoveConfig(...)                   (icall_directAPI((uint32_t) IDX_HCI_LE_CS_RemoveConfig , ##__VA_ARGS__))
#define HCI_LE_CS_SetChannelClassification(...)       (icall_directAPI((uint32_t) IDX_HCI_LE_CS_SetChannelClassification , ##__VA_ARGS__))
#define HCI_LE_CS_SetProcedureParameters(...)         (icall_directAPI((uint32_t) IDX_HCI_LE_CS_SetProcedureParameters , ##__VA_ARGS__))
#define HCI_LE_CS_ProcedureEnable(...)                (icall_directAPI((uint32_t) IDX_HCI_LE_CS_ProcedureEnable , ##__VA_ARGS__))
#define HCI_LE_CS_Test(...)                           (icall_directAPI((uint32_t) IDX_HCI_LE_CS_Test , ##__VA_ARGS__))
#define HCI_LE_CS_TestEnd(...)                        (icall_directAPI((uint32_t) IDX_HCI_LE_CS_TestEnd))

/* Host Handover APIs */
#define Handover_RegisterSNCBs(...)             (icall_directAPI((uint32_t) IDX_Handover_RegisterSNCBs, ##__VA_ARGS__))
#define Handover_GetSNDataSize(...)             (icall_directAPI((uint32_t) IDX_Handover_GetSNDataSize, ##__VA_ARGS__))
#define Handover_InitSNParams(...)              (icall_directAPI((uint32_t) IDX_Handover_InitSNParams, ##__VA_ARGS__))
#define Handover_StartSN(...)                   (icall_directAPI((uint32_t) IDX_Handover_StartSN, ##__VA_ARGS__))
#define Handover_RegisterCNCBs(...)             (icall_directAPI((uint32_t) IDX_Handover_RegisterCNCBs, ##__VA_ARGS__))
#define Handover_InitCNParams(...)              (icall_directAPI((uint32_t) IDX_Handover_InitCNParams, ##__VA_ARGS__))
#define Handover_StartCN(...)                   (icall_directAPI((uint32_t) IDX_Handover_StartCN, ##__VA_ARGS__))
#define Handover_CloseSN(...)                   (icall_directAPI((uint32_t) IDX_Handover_CloseSN, ##__VA_ARGS__))

#ifdef __cplusplus
}
#endif

#endif /* ICALL_BLE_API_H */
