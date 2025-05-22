/******************************************************************************

 @file  rom_init.c

 @brief This file contains the externs for BLE Controller and OSAL ROM
        initialization.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2017-2025, Texas Instruments Incorporated
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

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/bcomdef.h" // include for DFL flag
#include "ti/ble/stack_util/lib_opt/map_direct.h"

#ifndef CONTROLLER_ONLY
#include "ti/ble/host/gap/gap_advertiser_internal.h"
#include "ti/ble/host/gap/gap_scanner_internal.h"
#include "ti/ble/host/gap/gap_internal.h"
#include "ti/ble/host/sm/sm.h"
#include "ti/ble/host/l2cap/l2cap_internal.h"
#include "ti/ble/host/handover/l2cap_handover.h"
#include "ti/ble/host/handover/gatt_handover.h"
#include "ti/ble/host/gap/gap_initiator.h"
#include "ti/ble/host/gap/gap_initiator_internal.h"
#include "ti/ble/host/sm/sm_internal.h"
#include "ti/ble/host/handover/handover.h"
#include "ti/ble/host/gapbondmgr/gapbondmgr.h"
#endif //CONTROLLER_ONLY

#ifdef CONNECTION_HANDOVER
#include "ti/ble/controller/ll/ll_handover_cn.h"
#include "ti/ble/controller/ll/ll_handover_sn.h"
#endif

#include "ti/ble/controller/ll/ll.h"
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/ll/ll_enc.h"
#include "ti/ble/controller/ll/ll_al.h"
#include "ti/ble/controller/ll/ll_timer_drift.h"
#include "ti/ble/controller/ll/ll_rat.h"
#include "ti/ble/controller/ll/ll_privacy.h"

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * @fn          BLE ROM Spinlock
 *
 * @brief       This routine is used to trap indexing errors in R2R JT.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void ROM_Spinlock( void )
{
  volatile uint8 i = 1;

  while(i);
}

/*******************************************************************************
 * PROTOTYPES
 */

/*******************************************************************************
 * Periodic Adv hooks
 */
uint8 MAP_gapAdv_periodicAdvCmdCompleteCBs( void *pMsg )
{
#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) ) && defined(USE_PERIODIC_ADV)
  return gapAdv_periodicAdvCmdCompleteCBs(pMsg);
#else
  return TRUE;
#endif
}

/*******************************************************************************
 * Periodic Scan hooks
 */
uint8_t MAP_gapScan_periodicAdvCmdCompleteCBs( void *pMsg )
{
  uint8_t status = TRUE;
#ifndef CONTROLLER_ONLY
#ifdef USE_PERIODIC_SCAN
  status = gapScan_periodicAdvCmdCompleteCBs(pMsg);
#endif // USE_PERIODIC_SCAN
#endif // CONTROLLER_ONLY
  return status;
}

uint8_t MAP_gapScan_periodicAdvCmdStatusCBs( void *pMsg )
{
  uint8_t status = TRUE;
#ifndef CONTROLLER_ONLY
#ifdef USE_PERIODIC_SCAN
  status = gapScan_periodicAdvCmdStatusCBs(pMsg);
#endif // USE_PERIODIC_SCAN
#endif // CONTROLLER_ONLY
  return status;
}

uint8_t MAP_gapScan_processBLEPeriodicAdvCBs( void *pMsg )
{
  uint8_t status = TRUE;
#ifndef CONTROLLER_ONLY
#ifdef USE_PERIODIC_SCAN
  status = gapScan_processBLEPeriodicAdvCBs(pMsg);
#endif // USE_PERIODIC_SCAN
#endif // CONTROLLER_ONLY
  return status;
}

/**
* These hooks created to change the call to the relevant HCI command
* instead of calling the controller directly.
* This is needed to support the relevant command complete events
* that are passed to the application when using BLE3_CMD
* compilation flag
*/
uint8_t LE_SetExtAdvData_hook( void * pMsg )
{
#ifdef BLE3_CMD
  return HCI_LE_SetExtAdvData(pMsg);
#else
  return LE_SetExtAdvData(pMsg);
#endif
}
uint8_t LE_SetExtScanRspData_hook( void * pMsg)
{
#ifdef BLE3_CMD
  return HCI_LE_SetExtScanRspData(pMsg);
#else
  return LE_SetExtScanRspData(pMsg);
#endif
}

uint8_t LE_SetExtAdvEnable_hook( void * pMsg)
{
#ifdef BLE3_CMD
  return HCI_LE_SetAdvStatus(pMsg);
#else
  return LE_SetExtAdvEnable(pMsg);
#endif
}


uint8 MAP_gapAdv_handleAdvHciCmdComplete( void *pMsg )
{
#ifdef BLE3_CMD
  return gapAdv_handleAdvHciCmdComplete(pMsg);
#else
  return TRUE;
#endif
}

/*******************************************************************************
 * Link time configuration functions
 */

// (CENTRAL_CFG | OBSERVER_CFG) functions
uint8 MAP_gapScan_init(void)
{
#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
  return gapScan_init();
#else
  return LL_STATUS_SUCCESS;
#endif
}
uint8 MAP_SM_InitiatorInit(void)
{
#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
  return SM_InitiatorInit();
#else
  return LL_STATUS_SUCCESS;
#endif
}
void MAP_gap_CentConnRegister(void)
{
#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
  gap_CentConnRegister();
#endif
}
void MAP_gapScan_processSessionEndEvt(void* pSession, uint8_t status)
{
#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
  gapScan_processSessionEndEvt( pSession, status);
#endif
}

// (PERIPHERAL_CFG | BROADCASTER_CFG) functions
uint8 MAP_gapAdv_init(void)
{
#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )
  return gapAdv_init();
#else
  return LL_STATUS_SUCCESS;
#endif
}
uint8 MAP_SM_ResponderInit(void)
{
#if ( HOST_CONFIG & ( PERIPHERAL_CFG ) )
  return SM_ResponderInit();
#else
  return LL_STATUS_SUCCESS;
#endif
}
void MAP_gap_PeriConnRegister(void)
{
#if ( HOST_CONFIG & ( PERIPHERAL_CFG ) )
  gap_PeriConnRegister();
#endif
}

// (L2CAP_COC_CFG) functions
uint8  MAP_l2capSendNextSegment(void)
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return l2capSendNextSegment();
#else
  return ( FALSE );
#endif
}

uint8  MAP_l2capReassembleSegment(void *pPkt )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return l2capReassembleSegment( pPkt );
#else
  return ( TRUE );
#endif
}

uint8  MAP_L2CAP_ParseConnectReq( void *pCmd, uint8 *pData, uint16 len )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return L2CAP_ParseConnectReq( pCmd, pData, len );
#else
  return ( FAILURE );
#endif
}

uint8  MAP_l2capParseConnectRsp( void *pCmd, uint8 *pData, uint16 len )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return l2capParseConnectRsp( pCmd, pData, len );
#else
  return ( FAILURE );
#endif
}

uint8  MAP_L2CAP_ParseFlowCtrlCredit( void *pCmd, uint8 *pData, uint16 len )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return L2CAP_ParseFlowCtrlCredit( pCmd, pData, len );
#else
  return ( FAILURE );
#endif
}

uint8  MAP_l2capParseDisconnectReq( void *pCmd, uint8 *pData, uint16 len )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return l2capParseDisconnectReq( pCmd, pData, len );
#else
  return ( FAILURE );
#endif
}

uint8  MAP_l2capParseDisconnectRsp( void *pCmd, uint8 *pData, uint16 len )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return l2capParseDisconnectRsp( pCmd, pData, len );
#else
  return ( FAILURE );
#endif
}

uint8  MAP_L2CAP_DisconnectReq( uint16 connHandle, uint16 CID )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return L2CAP_DisconnectReq( connHandle, CID );
#else
  return ( INVALIDPARAMETER );
#endif
}

uint16 MAP_l2capBuildDisconnectRsp( uint8 *pBuf, uint8 *pData )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return l2capBuildDisconnectRsp( pBuf, pData );
#else
  return ( FAILURE );
#endif
}

void   MAP_l2capProcessConnectReq( uint16 connHandle, uint8 id, void *pConnReq )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  l2capProcessConnectReq( connHandle, id, pConnReq );
#endif
}

void   MAP_l2capGetCoChannelInfo( void *pChannel, void *pInfo )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  l2capGetCoChannelInfo( pChannel, pInfo );
#endif
}

void   MAP_l2capNotifyChannelEstEvt( void *pChannel, uint8 status, uint16 result )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  l2capNotifyChannelEstEvt( pChannel, status, result );
#endif
}

void  *MAP_l2capFindRemoteCID( uint16 connHandle, uint16 CID )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return l2capFindRemoteCID( connHandle, CID );
#else
  return ( NULL );
#endif
}

void   MAP_l2capNotifyChannelTermEvt( void *pChannel, uint8 status, uint16 reason )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  l2capNotifyChannelTermEvt( pChannel, status, reason );
#endif
}

void  *MAP_l2capFindLocalCID( uint16 connHandle, uint16 CID )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return l2capFindLocalCID( connHandle, CID );
#else
  return ( NULL );
#endif
}

void   MAP_l2capDisconnectChannel( void *pChannel, uint16 reason )
{
#if defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  l2capDisconnectChannel( pChannel, reason );
#endif
}

// (CENTRAL_CFG) functions
uint8 MAP_gapIsInitiating( void )
{
#if (HOST_CONFIG & CENTRAL_CFG)
  return gapIsInitiating();
#else
  return ( FALSE );
#endif
}
uint8 MAP_GapInit_cancelConnect( void )
{
#if (HOST_CONFIG & CENTRAL_CFG)
  return GapInit_cancelConnect();
#else
  return ( bleIncorrectMode );
#endif
}
uint8 MAP_smpInitiatorContProcessPairingPubKey( void )
{
#if (HOST_CONFIG & CENTRAL_CFG)
  return smpInitiatorContProcessPairingPubKey();
#else
  return LL_STATUS_ERROR_INVALID_PARAMS;
#endif
}
void MAP_gapInit_initiatingEnd( void )
{
#if (HOST_CONFIG & CENTRAL_CFG)
  gapInit_initiatingEnd();
#endif
}
void MAP_gapInit_sendConnCancelledEvt( void )
{
#if (HOST_CONFIG & CENTRAL_CFG)
  gapInit_sendConnCancelledEvt();
#endif
}

/*******************************************************************************
* Connection Handover
*/

uint8_t *MAP_l2cap_handoverAdvanceDataPointer( uint8_t *pHandoverData )
{
#ifdef CONNECTION_HANDOVER
  return l2cap_handoverAdvanceDataPointer(pHandoverData);
#else
  VOID pHandoverData;
  return NULL;
#endif
}

void MAP_L2CAP_HandoverInitSN( void )
{
#if defined (CONNECTION_HANDOVER) && defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  L2CAP_HandoverInitSN();
#endif
}

uint8_t MAP_L2CAP_Handover_StartCN( uint8_t *pHandoverData )
{
#if defined (CONNECTION_HANDOVER) && defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return L2CAP_Handover_StartCN(pHandoverData);
#else
  VOID pHandoverData;
  return UFAILURE;
#endif
}
uint8_t MAP_L2CAP_Handover_GetSNDataSize( uint16_t connHandle )
{
#ifdef CONNECTION_HANDOVER
#if defined (CONNECTION_HANDOVER) && defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return L2CAP_Handover_GetSNDataSize(connHandle);
#else
  VOID connHandle;
  return HANDOVER_L2CAP_COMPONENT_SIZE;
#endif
#else // !CONNECTION_HANDOVER
  VOID connHandle;
  return 0U;
#endif
}

void *MAP_l2capHandoverGetPsm( const void *pHandoverPsmData )
{
#if defined (CONNECTION_HANDOVER) && defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return l2capHandoverGetPsm(pHandoverPsmData);
#else
  VOID pHandoverPsmData;
  return NULL;
#endif
}

uint8_t MAP_L2CAP_HandoverTriggerDataTransfer( void )
{
#if defined (CONNECTION_HANDOVER) && defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return L2CAP_HandoverTriggerDataTransfer();
#else
  return USUCCESS;
#endif
}

uint8_t MAP_l2capHandoverFindAndReplaceChannelStatus( uint16_t connHandle, uint8_t newStatus )
{
#if defined (CONNECTION_HANDOVER) && defined (BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  return l2capHandoverFindAndReplaceChannelStatus(connHandle, newStatus);
#else
  VOID connHandle;
  VOID newStatus;
  return USUCCESS;
#endif
}

void MAP_Handover_ApplyCnData(uint16_t connHandle)
{
#ifdef CONNECTION_HANDOVER
  Handover_ApplyCnData(connHandle);
#else
  VOID connHandle;
#endif
}
