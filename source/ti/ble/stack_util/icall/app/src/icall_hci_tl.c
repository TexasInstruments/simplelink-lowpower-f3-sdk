/******************************************************************************

 @file  icall_hci_tl.c

 @brief This file contains the HCI TL implementation to transpose HCI serial
        packets into HCI function calls to the Stack.

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

/*********************************************************************
 * INCLUDES
 */
#include "ti/ble/controller/hci/hci_event.h"
#include <string.h>
#include "ti/ble/stack_util/icall/app/icall.h"
#include "ti/ble/stack_util/comdef.h"
/* This Header file contains all BLE API and icall structure definition */
#include "ti/ble/stack_util/icall/app/icall_ble_api.h"
#include "ti/ble/stack_util/icall/app/icall_hci_tl.h"
#include "ti/ble/controller/ll/ll.h"
#include "ti/ble/stack_util/lib_opt/map_direct.h"

#ifndef CONTROLLER_ONLY
#include "ti/ble/host/gap/gap_internal.h"
#include "ti/ble/host/sm/sm_internal.h"
#include "ti/ble/host/gapbondmgr/gapbondmgr_internal.h"
// host Stub headers
#include "ti/ble/stack_util/lib_opt/host_stub_gap_bond_mgr.h"
#endif // CONTROLLER_ONLY

#if  (defined(PTM_MODE) && defined(HCI_TL_FULL))
#error "Either PTM_MODE or HCI_TL_FULL should be enable, not both! (Both are TL defines)"
#endif /*  defined (PTM_MODE))) && (defined(PTM_MODE)) */

#if  defined(TESTMODES) && (!(HOST_CONFIG & ( CENTRAL_CFG | PERIPHERAL_CFG )))
#error " TESTMODES should not be defined if CENTRAL_CFG or PERIPHERAL_CFG is not defined"
#endif /* defined(TESTMODES) && (!(HOST_CONFIG & ( CENTRAL_CFG | PERIPHERAL_CFG ))) */

//stub hedears
#include "ti/ble/stack_util/lib_opt/ctrl_stub_initiator.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_connectable.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_scanner.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_adv_nconn.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_legacy_cmd.h"

/*********************************************************************
 * MACROS
 */

/**
 * 32 bits of data are used to describe the arguments of each HCI command.
 * a total of 8 arguments are allowed and the bits are divided equally between
 * each argument, thus allocating 4 bits to describe all possible rules for
 * interpreting the serialized arguments of an HCI buffer into a parameterized
 * list for calling the variadic function macro which maps a serial packet's
 * opcode to the corresponding HCI function in the Stack. 4 bits allow for 16
 * different rules, with the used bit permutations described below.
 *
 * Multi-octet values are little-endian unless otherwise specified.
 *
 * Value  | Rule
 *********|*********************************************************************
 * 0b0000 | No Parameter, signifies end of parsing
 * 0b0001 | 1 byte parameter
 * 0b0010 | 2 byte parameter
 * 0b0011 | 4 byte parameter (not used)
 * 0b0100 | reserved
 * 0b0101 | pointer to a single byte parameter
 * 0b0110 | pointer to 2 byte parameter
 * 0b0111 | reserved
 * 0b1000 | reserved
 * 0b1001 | 1 byte parameter implying the len (in units specified by the rule)
 *          of upcoming buffer(s)
 * 0b1010 | reserved
 * 0b1011 | reserved
 * 0b1100 | pointer to offset 0 of a 2 byte buffer
 * 0b1101 | pointer to offset 0 of a 6 byte buffer (BLE Address)
 * 0b1110 | pointer to offset 0 of a 8 byte buffer
 * 0b1111 | pointer to offset 0 of a 16 byte buffer (key buffer)
 *******************************************************************************
 *
 * Alternate meanings:
 *                     The command index field is a 16 bit field which may
 * have greater than 256 indexes, but would never require the full range.  The
 * highest order bit of this field can be used to switch the rules parameter
 * from specifying a pointer to an extended array of rules for large commands.
 */
// Number of parameters supported
#define HCI_MAX_NUM_ARGS                          12
#define HCI_DEFAULT_NUM_RULES                      8

// number of bits used to describe each parameter
#define PARAM_BIT_WIDTH                           4
#define PARAM_BIT_MASK                            0x0000000F

// Macro for condensing rules into a 32 bit value.
#define PARAM(x, i)                               ((x) << ((i)))
#define PARAM0(x)                                 PARAM(x, 0)
#define PARAM1(x)                                 PARAM(x, 1 * PARAM_BIT_WIDTH)
#define PARAM2(x)                                 PARAM(x, 2 * PARAM_BIT_WIDTH)
#define PARAM3(x)                                 PARAM(x, 3 * PARAM_BIT_WIDTH)
#define PARAM4(x)                                 PARAM(x, 4 * PARAM_BIT_WIDTH)
#define PARAM5(x)                                 PARAM(x, 5 * PARAM_BIT_WIDTH)
#define PARAM6(x)                                 PARAM(x, 6 * PARAM_BIT_WIDTH)
#define PARAM7(x)                                 PARAM(x, 7 * PARAM_BIT_WIDTH)

#define HPARAMS(x0, x1, x2, x3, x4, x5, x6, x7)   (PARAM7(x7) | \
                                                   PARAM6(x6) | \
                                                   PARAM5(x5) | \
                                                   PARAM4(x4) | \
                                                   PARAM3(x3) | \
                                                   PARAM2(x2) | \
                                                   PARAM1(x1) | \
                                                   PARAM0(x0))

#define HPARAMS_DUPLE(x0, x1)                     (PARAM1(x1) | PARAM0(x0))

#define EXTENDED_PARAMS_FLAG                      0x80000000
#define PARAM_FLAGS                               (EXTENDED_PARAMS_FLAG)
#define EXTENDED_PARAMS_CMD_IDX_GROUP(idx)        ((EXTENDED_PARAMS_FLAG) | (idx))
#define IS_EXTENDED_PARAMS(cmdIdxGroup)           ((cmdIdxGroup) & (EXTENDED_PARAMS_FLAG))
#define GET_CMD_IDX(cmdIdxGroup)                  ((cmdIdxGroup) & ~(PARAM_FLAGS))
#define GET_RULE(pRules, arg_i)                   ((uint8_t)(((pRules)[(arg_i)/2] >> ((arg_i) % 2) * PARAM_BIT_WIDTH) & PARAM_BIT_MASK))
#define HOST_OPCODE(csg, opcode)                  ((((csg) & 0x7) << 7) | ((opcode) & 0x7F))


#define RSP_PAYLOAD_IDX                           6
#define MAX_RSP_DATA_LEN                          100
#define MAX_RSP_BUF                               (RSP_PAYLOAD_IDX + MAX_RSP_DATA_LEN)

#define GET_HEAP_STATS_LIGHT                      0x1
#define GET_HEAP_STATS_FULL                       0x2
#define GET_THREAD_STATS                          0x4

#if !defined(HCI_EXT_APP_OUT_BUF)
  #define HCI_EXT_APP_OUT_BUF                     44
#endif // !HCI_EXT_APP_OUT_BUF

#define KEYDIST_SENC                              0x01
#define KEYDIST_SID                               0x02
#define KEYDIST_SSIGN                             0x04
#define KEYDIST_SLINK                             0x08
#define KEYDIST_MENC                              0x10
#define KEYDIST_MID                               0x20
#define KEYDIST_MSIGN                             0x40
#define KEYDIST_MLINK                             0x80

// Maximum number of reliable writes supported by Attribute Client
#define GATT_MAX_NUM_RELIABLE_WRITES              5

#if !defined(STACK_REVISION)
  #define STACK_REVISION                          0x010100
#endif // STACK_REVISION

#define PROXY_ID(id) (ICall_getLocalMsgEntityId(ICALL_SERVICE_CLASS_BLE_MSG, (id)))

// Note: Size of Extended Adv Report Less Appended two byte Vendor Specific DID field.
#define HCI_AE_EVENT_LENGTH                      (sizeof(aeExtAdvRptEvt_t) - sizeof(((aeExtAdvRptEvt_t *)0)->pData) - sizeof(uint16))
#define MAX_REPORT_DATA_SIZE                     200
#define SCAN_EVENT_DATA_COMPLETE_MASK            0xFF9F
#define SCAN_EVENT_INCOMPLETE_WITH_MORE          0x0020

#define GAP_SCAN_EVENT_LENGTH                    (sizeof(GapScan_Evt_AdvRpt_t) - sizeof(((GapScan_Evt_AdvRpt_t *)0)->pData))
#define GAP_SCAN_EVENT_HEADER                    7

#define HCI_MAX_TL_AE_DATA_LEN                   251

#define ADV_LEGACY_SET_HANDLE                    0
#define ADV_INVALID_SET_HANDLE                   0xFF

#ifdef BLE3_CMD
#define GAP_MAKE_ADDR_RS(pAddr)  ((pAddr[B_ADDR_LEN-1] |= STATIC_ADDR_HDR))
#endif
/*********************************************************************
 * TYPEDEFS
 */


typedef struct
{
  uint8_t  pktType;
  uint16_t opCode;
  uint16_t  len;
  uint8_t  *pData;
} hciExtCmd_t;

/* Advertising Extension management */
typedef struct hci_tl_advSet_t hci_tl_advSet_t;

// Extended Advertising Set Entry
struct hci_tl_advSet_t
{
    hci_tl_advSet_t       *next;            // ptr to next handle in list, if any
    uint8_t               handle;
    aeRandAddrCmd_t       setAddrCmd;
    aeEnableCmd_t         enableCmdParams;
    aeSetParamCmd_t       advCmdParams;
    aeSetDataCmd_t        advCmdData;
    aeEnableScanCmd_t     enableScanCmdParams;
    aeSetDataCmd_t        scanCmdData;
};

// Structure containing the parameter for the GAP Scanner Event callback
typedef struct
{
  uint32_t event;
  uint8_t* pData;
}scanEvtCallback_t;

// Structure containing the parameter for the GAP advertiser Event callback
typedef struct
{
  uint32_t event;
  uint8_t* pData;
}advEvtCallback_t;

// Structure containing the parameter for the HCI(LL) Event callback
typedef struct
{
  uint8_t event;
  union
  {
    uint8_t handle;
    void*   pData;
  }data;
}hci_tl_AdvEvtCallback_t;

// Structure containing the parameter for the HCI(LL) Scan Event callback
typedef struct
{
  uint8_t event;
  void *  pData;
}hci_tl_ScanEvtCallback_t;

// Structure containing the parameter for the passcode Event callback
typedef struct
{
  uint8_t connectionHandle;
} passcodeEvtCallback_t;

/*********************************************************************
 * EXTERNS
 */

extern const uint16_t ll_buildRevision;
#ifndef CONTROLLER_ONLY
extern gattAttribute_t gattAttrTbl[];
#endif // !CONTROLLER_ONLY
/*********************************************************************
 * LOCAL VARIABLES
 */

static hci_tl_advSet_t      *hci_tl_advSetList = NULL;
static aeSetScanParamCmd_t  hci_tl_cmdScanParams;
static aeEnableScanCmd_t    hci_tl_cmdScanEnable;

#if defined(HCI_TL_FULL)

#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
static uint8_t              host_tl_gapScannerInitialized;
#endif

#ifdef BLE3_CMD
#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )
// Default - infinite advertising
uint16_t advDuration = 0;
uint8_t advHandleLegacy = 0xFF;
uint8_t advEventCntr = 0;
uint8_t maxConnReached = 0;
#endif

#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
// Default - infinite scan
uint16_t scanDuration = 0;
uint8_t numDev = 0;
uint8_t scanSummarySent = 0;
#endif

uint8_t advNotice = 0;
uint8_t scanNotice = 0;
uint8_t sendEstEvt = 0;
uint8_t maxNumReports = 0;
uint8_t legacyConnCancel = 0;
deviceInfo_t *deviceInfoArr = NULL;
uint8_t makeDiscFlag = 0;
uint8_t endDiscFlag = 0;
#endif // BLE3_CMD

uint32_t host_tl_defaultPasscode = B_APP_DEFAULT_PASSCODE;
#ifdef HOST_CONFIG
// Passcode.
static void host_tl_passcodeCB(uint8_t *deviceAddr, uint16_t connectionHandle,
                               uint8_t uiInputs, uint8_t uiOutputs,
                               uint32_t numComparison);

// Bond Manager Callbacks
static const gapBondCBs_t host_tl_bondCB =
{
  (pfnPasscodeCB_t)host_tl_passcodeCB,  //  Passcode callback
  NULL		                              //  Pairing state callback
};
#endif // HOST_CONFIG

#endif //HCI_TL_FULL

#if (defined(HCI_TL_FULL) || defined(PTM_MODE))  && defined(HOST_CONFIG)
static ICall_EntityID appTaskID;

#ifdef HCI_TL_FULL
// Outgoing response
static uint8_t rspBuf[MAX_RSP_BUF];

// Outgoing event
static uint8_t out_msg[HCI_EXT_APP_OUT_BUF];
uint8_t *pLongMsg;
#endif /* HCI_TL_FULL */

#endif // (HCI_TL_FULL || PTM_MODE)  && HOST_CONFIG

// Callback for sending command status.
static HCI_TL_CommandStatusCB_t HCI_TL_CommandStatusCB = NULL;

// Callback for posting callback event (switching from SWI to Thread).
static HCI_TL_CalllbackEvtProcessCB_t HCI_TL_CallbackEvtProcessCB = NULL;


// Store the type of command that are used.
// Once it is set, it can only be change by reset HCI (HCI_RESET command)
uint8_t legacyCmdStatusAdv = HCI_LEGACY_CMD_STATUS_UNDEFINED;
uint8_t legacyCmdStatusScan = HCI_LEGACY_CMD_STATUS_UNDEFINED;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

#if (defined(HCI_TL_FULL) || defined(PTM_MODE))
static void HCI_TL_SendCommandPkt(hciPacket_t *pMsg);

//HOST
#ifdef HOST_CONFIG

#if !defined(PTM_MODE)
// PTM_MODE: No Host processing required (GAP, GATT, L2CAP, Etc...)

static void HCI_TL_SendExtensionCmdPkt(hciPacket_t *pMsg);
static uint8_t processExtMsg(hciPacket_t *pMsg);

#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
static uint8_t processExtMsgL2CAP(uint8_t cmdID, hciExtCmd_t *pCmd, uint8_t *pRspDataLen);
static uint8_t processExtMsgATT(uint8_t cmdID, hciExtCmd_t *pCmd);
static uint8_t processExtMsgGATT(uint8_t cmdID, hciExtCmd_t *pCmd, uint8_t *pRspDataLen);
#endif /* (CENTRAL_CFG | PERIPHERAL_CFG */

static uint8_t processExtMsgGAP(uint8_t cmdID, hciExtCmd_t *pCmd, uint8_t *pRspDataLen);
static uint8_t processExtMsgUTIL(uint8_t cmdID, hciExtCmd_t *pCmd, uint8_t *pRspDataLen);

#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
static uint8_t *createMsgPayload(uint8_t *pBuf, uint16_t len);
static uint8_t *createSignedMsgPayload(uint8_t sig, uint8_t cmd, uint8_t *pBuf, uint16_t len);
static uint8_t *createPayload(uint8_t *pBuf, uint16_t len, uint8_t sigLen);
static uint8_t mapATT2BLEStatus(uint8_t status);
static uint8_t buildHCIExtHeader(uint8_t *pBuf, uint16_t event, uint8_t status, uint16_t connHandle);
#endif /* (CENTRAL_CFG | PERIPHERAL_CFG */

static uint8_t checkNVLen(osalSnvId_t id, osalSnvLen_t len);

static uint8_t processEvents(ICall_Hdr *pMsg);
static uint8_t *processEventsGAP(gapEventHdr_t *pMsg, uint8_t *pOutMsg,
                               uint16_t *pMsgLen, uint8_t *pAllocated,
                               uint8_t *pDeallocate);
#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
static uint8_t *processEventsSM(smEventHdr_t *pMsg, uint8_t *pOutMsg,
                               uint8_t *pMsgLen, uint8_t *pAllocated);
static uint8_t *processEventsL2CAP(l2capSignalEvent_t *pPkt, uint8_t *pOutMsg,
                                  uint8_t *pMsgLen);
static uint8_t *processDataL2CAP(l2capDataEvent_t *pPkt, uint8_t *pOutMsg,
                                uint16_t *pMsgLen, uint8_t *pAllocated);
static uint8_t *processEventsGATT(gattMsgEvent_t *pPkt, uint8_t *pOutMsg,
                                 uint16_t *pMsgLen, uint8_t *pAllocated);
#if !defined(GATT_DB_OFF_CHIP)
static uint8_t *processEventsGATTServ(gattEventHdr_t *pPkt, uint8_t *pMsg,
                                     uint8_t *pMsgLen);
#endif /* GATT_DB_OFF_CHIP */

#endif /* (CENTRAL_CFG | PERIPHERAL_CFG) */

#if defined(BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
static uint16_t l2capVerifySecCB(uint16_t connHandle, uint8_t id, l2capConnectReq_t *pReq);
static uint8_t buildCoChannelInfo(uint16_t CID, l2capCoCInfo_t *pInfo, uint8_t *pRspBuf);
#endif /* BLE_V41_FEATURES & L2CAP_COC_CFG */

#if defined(GATT_DB_OFF_CHIP)
static uint8_t addAttrRec(gattService_t *pServ, uint8_t *pUUID, uint8_t len,
                         uint8_t permissions, uint16_t *pTotalAttrs, uint8_t *pRspDataLen);
static const uint8_t *findUUIDRec(uint8_t *pUUID, uint8_t len);
static void freeAttrRecs(gattService_t *pServ);
#endif // GATT_DB_OFF_CHIP

// Passcode.
static void host_tl_passcodeCB(uint8_t *deviceAddr, uint16_t connectionHandle,
                               uint8_t uiInputs, uint8_t uiOutputs,
                               uint32_t numComparison);


static void host_tl_passcodeCBProcess(passcodeEvtCallback_t *pData);

#endif /* (PTM_MODE) */

#endif /* HOST_CONFIG */

#endif /* (defined(HCI_TL_FULL) || defined(PTM_MODE) ) */

#if (defined(HCI_TL_FULL) || defined(PTM_MODE))
static void HCI_TL_SendVSEvent(uint8_t *pBuf, uint16_t dataLen);
static void HCI_TL_getMemStats(uint8_t mask, uint8_t* pData, uint8_t *len);

#ifdef HOST_CONFIG

#ifndef PTM_MODE
#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )

static void      host_tl_advEvtCallback(uint32_t event, void *pData, uint32_t arg);
static void      host_tl_advEvtCallbackProcess(advEvtCallback_t *advEvtCallback);
#endif // PERIPHERAL_CFG | BROADCASTER_CFG

#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
static void      host_tl_scanEvtCallback(uint32_t event, void *pData, uintptr_t arg);
static void      host_tl_scanEvtCallbackProcess(scanEvtCallback_t * scanEvtCallback);
static void      host_tl_sendAdvReport(uint32_t event, GapScan_Evt_AdvRpt_t * advRpt);
#endif // CENTRAL_CFG | OBSERVER_CFG

#if ( HOST_CONFIG & ( PERIPHERAL_CFG | CENTRAL_CFG ) )
static void      host_tl_connEvtCallback(Gap_ConnEventRpt_t *pReport);
static void      host_tl_connEvtCallbackProcess(Gap_ConnEventRpt_t *pReport);
#endif // PERIPHERAL_CFG | CENTRAL_CF
#endif /* (PTM_MODE) */

#else //!HOST_CONFIG

static uint8_t processExtraHCICmd(hciPacket_t *pMsg);

#endif //HOST_CONFIG

// Advertize static functions:
static hci_tl_advSet_t* hci_tl_GetAdvSet(uint8_t handle);
static void             hci_tl_InitAdvSetParams(hci_tl_advSet_t *pAdvSet);
static void             hci_tl_RemoveAdvSet(uint8_t handle);
static void             hci_tl_removePendingData(uint8_t* pendingAdvData);
static uint8_t*         hci_tl_createPendingData(uint8_t *pNewData, uint16_t len, uint8_t* pData);
static void             hci_tl_managedAEdata(uint16_t mode, aeSetDataCmd_t * pCmddata, uint8_t *pData);
static uint8_t*         hci_tl_appendPendingData(uint8_t *pStorage, uint16_t lenStorage,
                                                 uint8_t* pData, int16_t len);
static uint8            hci_tl_isValidRandomAddressForAdv (hci_tl_advSet_t *pAdvSet);

#endif /* (defined(HCI_TL_FULL) || defined(PTM_MODE)) */

// Scanner static functions:
static void      hci_tl_legacyScanCback(uint8_t event, aeExtAdvRptEvt_t *extAdvRpt);
static void      hci_tl_legacyScanEventCallbackProcess(hci_tl_ScanEvtCallback_t *extAdvRpt);
static void      hci_tl_setDefaultScanParams (aeSetScanParamCmd_t *pcmdScanParams);
static uint8     hci_tl_isValidRandomAddressForScan (aeSetScanParamCmd_t *cmdScanParams);

/*********************************************************************
 * @fn      HCI_TL_Init
 *
 * @brief   Initialize HCI TL.
 *
 * @param   overwriteCB     - callback used to allow custom override the contents of the
 *                            serial buffer.
 *          csCB            - Callback to process command status
 *          evtCB           - Callback to post event related to Callback from LL
 *          taskID - Task ID of task to process and forward messages to the TL.
 *
 * @return  none.
 */
void HCI_TL_Init(HCI_TL_ParameterOverwriteCB_t overwriteCB,
                 HCI_TL_CommandStatusCB_t csCB,
                 HCI_TL_CalllbackEvtProcessCB_t evtCB,
                 ICall_EntityID taskID)
{
  HCI_TL_CommandStatusCB = csCB;
  (void) HCI_TL_CommandStatusCB;

  HCI_TL_CallbackEvtProcessCB = evtCB;
  (void) HCI_TL_CallbackEvtProcessCB;

#if (defined(HCI_TL_FULL) || defined(PTM_MODE))  && defined(HOST_CONFIG)
  appTaskID = taskID;
#endif

#if ( HOST_CONFIG & ( CENTRAL_CFG | PERIPHERAL_CFG ) )
#if (defined(HCI_TL_FULL) || defined(PTM_MODE)) && defined(HOST_CONFIG)

  // Register for GATT local events and ATT Responses pending for transmission
  GATT_RegisterForMsgs(appTaskID);

  // Register to receive incoming ATT Indications/Notifications
  GATT_RegisterForInd(appTaskID);

  #if !defined(GATT_DB_OFF_CHIP)
    // Register with GATT Server App for event messages
    GATTServApp_RegisterForMsg(appTaskID);
  #else
    // Register with GATT Server for GATT messages
    GATT_RegisterForReq(appTaskID);
  #endif // GATT_DB_OFF_CHIP
#endif // (HCI_TL_FULL || PTM_MODE) && HOST_CONFIG
#endif // (CENTRAL_CFG | PERIPHERAL_CFG)
}

/*******************************************************************************
 * Check that if random own address type configure as random identity,
 * random address configured before. if not: return FALSE,
 *                                      O.W: return TRUE.
 */
uint8 isValidRandomAddressForCreateConn ( aeCreateConnCmd_t HCI_TL_createConnParam )
{
    // If the own address Type is not random return TRUE
    if ((HCI_TL_createConnParam.ownAddrType != LL_DEV_ADDR_TYPE_RANDOM) &&
        (HCI_TL_createConnParam.ownAddrType != LL_DEV_ADDR_TYPE_RANDOM_ID))
    {
      return TRUE;
    }

    // else, own address type is random. Check if random address was configured.
    return LL_IsRandomAddressConfigured();
}


#if (defined(HCI_TL_FULL) || defined(PTM_MODE))
/*********************************************************************
 * @fn      HCI_TL_sendSystemReport
 *
 * @brief   Used to return specific system error over UART.
 *
 * @param   id - id of the module reporting the error.
 *          status - type of error
 *          info -   more information linked to the error or the module
 * @return  none.
 */
void HCI_TL_sendSystemReport(uint8_t id, uint8_t status, uint16_t info)
{
    // Unsupported Event for now...
    uint8_t data[6];
    data[0] = LO_UINT16(HCI_EXT_UTIL_SYSTEM_ERROR);
    data[1] = HI_UINT16(HCI_EXT_UTIL_SYSTEM_ERROR);
    data[2] = status;          //Status
    data[3] = id;  //Module
    data[4] = LO_UINT16(info);  //Event Generating the error
    data[5] = HI_UINT16(info);
    HCI_TL_SendVSEvent(data, sizeof(data));
}

/*********************************************************************
 * @fn      HCI_TL_sendMemoryReport
 *
 * @brief   Used to return memory usage statistic.
 *
 * @param   reportMask - bitfield of the different memory stat to return.
 * @return  none.
 */
void HCI_TL_sendMemoryReport(uint8_t reportMask)
{
    // Unsupported Event for now...
    uint8_t data[30];
    uint8_t len = 0;
    //Gather Memory Stat Buffer
    data[0] = LO_UINT16(HCI_EXT_UTIL_MEM_STATS_EVENT);
    data[1] = HI_UINT16(HCI_EXT_UTIL_MEM_STATS_EVENT);
    data[2] = 0;                //Status
    data[3] = HOST_TL_ID;       //Module
    HCI_TL_getMemStats((reportMask & 0x3), &data[4], &len);
    HCI_TL_SendVSEvent(data, len+4);
}

/*********************************************************************
 * @fn      HCI_TL_getMemStats
 *
 * @brief   fill a buffer with memory statistic
 *
 * @param   mask - bitfield of the different memory stat to return.
 *          rspBuf: buffer where the information will be stored, must be allocated by the caller
 *          len:    length of the filled data: must be allcoated by the caller.
 * @return  none.
 */
static void HCI_TL_getMemStats(uint8_t memStatCmd, uint8_t* rspBuf, uint8_t *len)
{
    uint8_t rspIndex = 0;

    rspBuf[rspIndex++] = 0;
    if (memStatCmd & GET_HEAP_STATS_LIGHT)
    {
        ICall_heapStats_t stats;
        ICall_getHeapStats(&stats);
        rspBuf[0] |= GET_HEAP_STATS_LIGHT;
        rspBuf[rspIndex++]   = GET_HEAP_STATS_LIGHT;
        //  Heap Size
        rspBuf[rspIndex++] = BREAK_UINT32(stats.totalSize, 0);
        rspBuf[rspIndex++] = BREAK_UINT32(stats.totalSize, 1);
        rspBuf[rspIndex++] = BREAK_UINT32(stats.totalSize, 2);
        rspBuf[rspIndex++] = BREAK_UINT32(stats.totalSize, 3);

        // Heap size free
        rspBuf[rspIndex++] = BREAK_UINT32(stats.totalFreeSize, 0);
        rspBuf[rspIndex++] = BREAK_UINT32(stats.totalFreeSize, 1);
        rspBuf[rspIndex++] = BREAK_UINT32(stats.totalFreeSize, 2);
        rspBuf[rspIndex++] = BREAK_UINT32(stats.totalFreeSize, 3);

        // Biggest free buffer available
        rspBuf[rspIndex++]  = BREAK_UINT32(stats.largestFreeSize, 0);
        rspBuf[rspIndex++] = BREAK_UINT32(stats.largestFreeSize, 1);
        rspBuf[rspIndex++] = BREAK_UINT32(stats.largestFreeSize, 2);
        rspBuf[rspIndex++] = BREAK_UINT32(stats.largestFreeSize, 3);
    }
    *len = rspIndex;
}
#endif /* (defined(HCI_TL_FULL) || defined(PTM_MODE)) */

/*********************************************************************
 * @fn      HCI_TL_SendToStack
 *
 * @brief   Translate serial buffer into it's corresponding function and
 *          parameterize the arguments to send to the Stack (Host or controller).
 *
 * @param   pHciMsg - pointer to a serialized HCI command (hciPacket_t) or data packet (hciDataPacket_t) message structure.
 *
 * @return  none.
 */
void HCI_TL_SendToStack(uint8_t *pHciMsg)
{
#if (defined(HCI_TL_FULL) || defined(PTM_MODE))
  hciPacket_t *pMsg;

  pMsg = (hciPacket_t *)pHciMsg;

  // if there is a host configuration
#ifdef HOST_CONFIG
#if !defined (PTM_MODE)
  if (pMsg->hdr.event == HCI_EXT_CMD_EVENT)
  {
    HCI_TL_SendExtensionCmdPkt(pMsg);
  }
  else
#endif /* defined (PTM_MODE) */
#endif /* HOST_CONFIG */
  // What type of packet is this?
  if (pMsg->hdr.event == HCI_HOST_TO_CTRL_CMD_EVENT)
  {
    // Command.  Interpret the command and send to Host.
    HCI_TL_SendCommandPkt(pMsg);
  }
  else if (pMsg->hdr.event == HCI_HOST_TO_CTRL_DATA_EVENT)
  {
    OPT_HCI_TL_SendDataPkt(pHciMsg);
  }
#endif /* (defined(HCI_TL_FULL) || defined(PTM_MODE)) */
}

/*********************************************************************
 * @fn      HCI_HostToControllerSend
 *
 * @brief   Translate HCI raw packet buffer into it's corresponding function and
 *          parameterize the arguments to send to the controller only.
 *
 * @param   pHciPkt - pointer to a raw buffer of HCI command or data packet.
 * @param   pktLen - the hci packet length
 *
 * @return  0 for success, negative number for error.
 */
int HCI_HostToControllerSend(uint8_t *pHciPkt, uint16_t pktLen)
{
    int status = HCI_STATUS_ERROR_OUT_OF_MEMORY;
    uint8 pktType = 0;

    // Validate packet buffer
    if (NULL != pHciPkt)
    {
      // Get packet type
      pktType = pHciPkt[0];

      // Command packet type
      if ((pktType == HCI_CMD_PACKET) || (pktType == HCI_EXTENDED_CMD_PACKET))
      {
        hciPacket_t *pCmdPkt = NULL;
        uint8  cmdPktHdrLen   = 0;      // size of the HCI command header
        uint16 cmdPktParamLen = 0;      // size of the HCI command payload
        uint16 cmdPktTotalLen = 0;      // size of whole pHciPkt, including the HCI data header

        // Parse the packet len
        if (pktType == HCI_CMD_PACKET)
        {
          cmdPktHdrLen = HCI_CMD_MIN_LENGTH;
          cmdPktParamLen = pHciPkt[3];
        }
        else // Extended command packet
        {
          cmdPktHdrLen = HCI_CMD_MIN_LENGTH + 1;
          cmdPktParamLen = BUILD_UINT16(pHciPkt[3], pHciPkt[4]);
        }

        // Set command packet total length
        cmdPktTotalLen = cmdPktHdrLen + cmdPktParamLen;

        // Verify that the input packet length (pktLen) is equal to the
        // calculated packet length (cmdPktTotalLen).
        // cmdPktHdrLen   is the size of the HCI command header
        // cmdPktParamLen is the size of the HCI command payload
        // cmdPktTotalLen is the whole pHciPkt size, including the HCI data header
        if (cmdPktTotalLen == pktLen)
        {
          // Allocate memory for the command packet.
          // This will include hciPacket_t header which will hold the whole command
          // (hci header + hci payload), as received from the host
          pCmdPkt = (hciPacket_t *) ICall_allocMsg(sizeof(hciPacket_t) + cmdPktTotalLen);

          if(pCmdPkt)
          {
            // Set header specific fields
            pCmdPkt->hdr.status = 0xFF;

            // This is a normal host-to-controller event
            pCmdPkt->hdr.event = HCI_HOST_TO_CTRL_CMD_EVENT;

            // Set pData to the first byte after the hciPacket osal header
            pCmdPkt->pData = (uint8_t *)pCmdPkt + sizeof(hciPacket_t);

            // Copy the raw hci data
            memcpy(pCmdPkt->pData, pHciPkt, cmdPktTotalLen);

#if (defined(HCI_TL_FULL) || defined(PTM_MODE))
            // Handoff the packet to the controller
            HCI_TL_SendCommandPkt(pCmdPkt);
#endif
            // Free the allocated packet and its pData
            ICall_freeMsg(pCmdPkt);

            // Set the status to sucess
            status = HCI_STATUS_SUCCESS;
          }
        }
        else
        {
          // Wrong HCI packet length
          status = HCI_STATUS_ERROR_INVALID_PACKET_LENGTH;
        }
      }
      // ACL data packet type
      else if (pktType == HCI_ACL_DATA_PACKET)
      {
        hciDataPacket_t *pDataPkt = NULL;
        uint16 dataPktHandle = 0;
        uint16 dataPktLen    = 0;        // size of the HCI data payload

        // Parse the data packet connection handle and flags
        dataPktHandle = BUILD_UINT16(pHciPkt[1], pHciPkt[2]);

        // Parse the data packet length
        dataPktLen = BUILD_UINT16(pHciPkt[3], pHciPkt[4]);

        // pktLen     is the whole pHciPkt size, including the HCI data header
        // dataPktLen is the size of the HCI data payload
        // Verify that the input packet length (pktLen - HCI_DATA_MIN_LENGTH) is equal to the
        // calculated data packet length (dataPktLen).
        if (dataPktLen == pktLen - HCI_DATA_MIN_LENGTH)
        {
          // Allocate memory for the data packet.
          // hciDataPacket_t holds the meta-data information for the received data packet
          // Thus, we drop the received HCI data header and save only the raw data payload.
          pDataPkt = (hciDataPacket_t *) ICall_allocMsg(sizeof(hciDataPacket_t) + dataPktLen);

          if (pDataPkt)
          {
            // Set packet header
            pDataPkt->hdr.event = HCI_HOST_TO_CTRL_DATA_EVENT;
            pDataPkt->hdr.status = 0xFF;
            // Set packet type
            pDataPkt->pktType = pktType;
            // Mask out PB and BC Flags
            pDataPkt->connHandle = dataPktHandle & 0x0FFF;
            // Isolate PB Flag
            pDataPkt->pbFlag = (dataPktHandle & 0x3000) >> 12;
            // Set packet length
            pDataPkt->pktLen = dataPktLen;

            // Set pData to the first byte after the hciDataPacket_t osal header and packet attributes
            pDataPkt->pData = (uint8_t *)pDataPkt + sizeof(hciDataPacket_t);

            // Drop the received HCI data header and copy only the raw data payload.
            memcpy(pDataPkt->pData, pHciPkt + HCI_DATA_MIN_LENGTH, dataPktLen);

            // Handoff the packet to the controller
            OPT_HCI_TL_SendDataPkt((uint8_t *)pDataPkt);

            // Free the allocated packet and its pData
            ICall_freeMsg(pDataPkt);

            // Set the status to sucess
            status = HCI_STATUS_SUCCESS;
          }
        }
        else
        {
          // Wrong HCI packet length
          status = HCI_STATUS_ERROR_INVALID_PACKET_LENGTH;
        }
      }
      else
      {
        // Wrong HCI packet type
        status = HCI_STATUS_ERROR_INVALID_PACKET_TYPE;
      }
    }
    else
    {
      // Invalid HCI packet pointer
      status = HCI_STATUS_ERROR_INVALID_PACKET_BUFFER;
    }

    return status;
}

#if (defined(HCI_TL_FULL) || defined(PTM_MODE))
/*********************************************************************
 * @fn      HCI_TL_SendCommandPkt
 *
 * @brief   Interprets an HCI command packet based off the translation table and
 *          calls the corresponding BLE API with the arguments provided.
 *
 * @param   pMsg - HCI Command serial packet to translate.
 *
 * @return  none.
 */
static void HCI_TL_SendCommandPkt( hciPacket_t *pMsg )
{
  uint8_t status = HCI_ERROR_CODE_UNKNOWN_HCI_CMD;
  uint16_t cmdOpCode;

  // Retrieve opcode
  cmdOpCode = BUILD_UINT16( pMsg->pData[1], pMsg->pData[2] );

#ifdef HOST_CONFIG

  (void)cmdOpCode;
  status = HCI_CMD_Parser( pMsg->pData );

#ifdef BLE3_CMD

  // adv and scan notice hci command doesn't return command complete or command status event.
  if( cmdOpCode == HCI_EXT_ADV_EVENT_NOTICE )
  {
    advNotice = 1;
    status = HCI_SUCCESS;
  }
  else if( cmdOpCode == HCI_EXT_SCAN_EVENT_NOTICE )
  {
    scanNotice = 1;
    status = HCI_SUCCESS;
  }
#endif // BLE3_CMD

#else //!HOST_CONFIG

  //For Legacy command revert the call order
  status = OPT_HCI_TL_legacyProcessHCICmd(pMsg);

#endif //HOST_CONFIG

  if ( status == HCI_ERROR_CODE_UNKNOWN_HCI_CMD )
  {
    MAP_HCI_CommandCompleteEvent( cmdOpCode, sizeof(status), &status );
  }
}

/*********************************************************************
 * @fn      checkLegacyHCICmdMode
 *
 * @brief   Check command opcode and return operation mode.
 *
 * @param   opcode - command opcode
 *
 * @return  operation mode
 */
static uint8_t checkLegacyHCICmdMode(uint16_t opcode)
{
  uint8_t rtnVal = HCI_LEGACY_CMD_STATUS_UNDEFINED;

  switch(opcode)
  {
    case HCI_LE_SET_ADV_PARAM:
    case HCI_LE_READ_ADV_CHANNEL_TX_POWER:
    case HCI_LE_SET_ADV_DATA:
    case HCI_LE_SET_SCAN_RSP_DATA:
    case HCI_LE_SET_ADV_ENABLE:
    {
      rtnVal = HCI_LEGACY_CMD_STATUS_BT4_ADV;
      break;
    }
    case HCI_LE_SET_EXT_ADV_PARAMETERS:
    case HCI_LE_SET_EXT_ADV_DATA:
    case HCI_LE_SET_EXT_SCAN_RESPONSE_DATA:
    case HCI_LE_SET_EXT_ADV_ENABLE:
    case HCI_LE_READ_MAX_ADV_DATA_LENGTH:
    case HCI_LE_READ_NUM_SUPPORTED_ADV_SETS:
    case HCI_LE_REMOVE_ADV_SET:
    case HCI_LE_CLEAR_ADV_SETS:
    case HCI_LE_SET_PERIODIC_ADV_PARAMETERS:
    case HCI_LE_SET_PERIODIC_ADV_DATA:
    case HCI_LE_SET_PERIODIC_ADV_ENABLE:
    case HCI_EXT_LE_SET_EXT_ADV_DATA:
    {
      rtnVal = HCI_LEGACY_CMD_STATUS_BT5_ADV;
      break;
    }
    case HCI_LE_SET_SCAN_PARAM:
    case HCI_LE_SET_SCAN_ENABLE:
    case HCI_LE_CREATE_CONNECTION:
    {
      rtnVal = HCI_LEGACY_CMD_STATUS_BT4_SCAN;
      break;
    }
    case HCI_LE_SET_EXT_SCAN_PARAMETERS:
    case HCI_LE_SET_EXT_SCAN_ENABLE:
    case HCI_LE_EXT_CREATE_CONN:
    case HCI_LE_PERIODIC_ADV_CREATE_SYNC:
    case HCI_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL:
    case HCI_LE_PERIODIC_ADV_TERMINATE_SYNC:
    case HCI_LE_ADD_DEVICE_TO_PERIODIC_ADV_LIST:
    case HCI_LE_REMOVE_DEVICE_FROM_PERIODIC_ADV_LIST:
    case HCI_LE_CLEAR_PERIODIC_ADV_LIST:
    case HCI_LE_READ_PERIODIC_ADV_LIST_SIZE:
    {
      rtnVal = HCI_LEGACY_CMD_STATUS_BT5_SCAN;
      break;
    }
    default:
    {
      rtnVal = HCI_LEGACY_CMD_STATUS_UNDEFINED;
      break;
    }
  }
  return rtnVal;
}

/*********************************************************************
 * @fn      checkLegacyHCICmdStatus
 *
 * @brief   Check and set the legacy command operation mode.
 *          if the mode is already set to a different mode, return failure (1)
 *          if the mode is not set, set it and return success (0)
 *          if the same mode is already set, return success (0)
 * @param   mode - operation mode to check and set
 *
 * @return  0: SUCCESS,
 *          1: failure, wrong mode, the command should be rejected.
 */
uint8_t checkLegacyHCICmdStatus(uint16_t opcode)
{
  uint8_t mode = checkLegacyHCICmdMode(opcode);

  if ((mode == HCI_LEGACY_CMD_STATUS_BT4_ADV) || (mode == HCI_LEGACY_CMD_STATUS_BT5_ADV))
  {
    if (legacyCmdStatusAdv == HCI_LEGACY_CMD_STATUS_UNDEFINED)
    {
      legacyCmdStatusAdv = mode;
      return(FALSE);
    }
    else
    {
      return(legacyCmdStatusAdv != mode);
    }
  }
  else if ((mode == HCI_LEGACY_CMD_STATUS_BT4_SCAN) || (mode == HCI_LEGACY_CMD_STATUS_BT5_SCAN))
  {
    if (legacyCmdStatusScan == HCI_LEGACY_CMD_STATUS_UNDEFINED)
    {
      legacyCmdStatusScan = mode;
      return(FALSE);
    }
    else
    {
      return(legacyCmdStatusScan != mode);
    }
  }
  else
  {
    return(FALSE);
  }
}

/*********************************************************************
 * @fn      processExtraHCICmd
 *
 * @brief   Process all HCI command that was not found in the predefined table
 *
 * @param   pMsg -     HCI Msg packet
 * @return  retVal     HCI Status
 */
static uint8_t processExtraHCICmd(hciPacket_t *pMsg)
{
    uint8_t retVal = HCI_SUCCESS;
    uint8_t retAdvVal;
    uint8_t retLegacyVal;
    uint8_t packetType;
    uint16_t cmdOpCode;
    uint8_t *param; // pointer into HCI command parameters.

    // Retrieve packetType
    packetType = pMsg->pData[0];

    // Retrieve opcode
    cmdOpCode = BUILD_UINT16( pMsg->pData[1], pMsg->pData[2] );

    // Retrieve pointer to parameter
    if ( packetType == HCI_EXTENDED_CMD_PACKET )
    {
      param = &pMsg->pData[5];
    }
    else
    {
      param = &pMsg->pData[4];
    }

    retAdvVal = OPT_processAdvExtraHCICmd( pMsg );
    if (retAdvVal != HCI_ERROR_CODE_UNKNOWN_HCI_CMD)
    {
      // Command found in processAdvExtraHCICmd, so return the status.
      return ( retAdvVal );
    }

    retLegacyVal = OPT_processLegacyExtraHCICmd( pMsg );
    if (retLegacyVal != HCI_ERROR_CODE_UNKNOWN_HCI_CMD)
    {
      // Command found in processLegacyExtraHCICmd, so return the status.
      return ( retLegacyVal );
    }

    // Else, Comand not found in processAdvExtraHCICmd, so continue...
    switch(cmdOpCode)
    {
        case HCI_LE_EXT_CREATE_CONN:
        {
            llStatus_t status;
            status = OPT_HCI_TL_ext_create_conn(cmdOpCode, param);
            HCI_CommandStatusEvent(status, cmdOpCode);
            break;
        }
// Extended Scanner
        case HCI_LE_SET_EXT_SCAN_ENABLE:
        {
            llStatus_t status;
            status = OPT_HCI_TL_set_ext_scan_enable(cmdOpCode, param, &hci_tl_cmdScanEnable, &hci_tl_cmdScanParams);

            HCI_CommandCompleteEvent(cmdOpCode,
                                     sizeof(status),
                                     &status);
            break;
        }
        case HCI_LE_SET_EXT_SCAN_PARAMETERS:
        {
            llStatus_t status;
            status = OPT_HCI_TL_set_ext_scan_param(cmdOpCode, param, &hci_tl_cmdScanParams);

            HCI_CommandCompleteEvent(cmdOpCode,
                                     sizeof(status),
                                     &status);
            break;

        }
        default:
        {
          retVal = HCI_ERROR_CODE_UNKNOWN_HCI_CMD;
        }
    }
    return retVal;
}

#ifndef PTM_MODE
// Following function are not used by PTM

#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
/*********************************************************************
 * @fn      host_tl_scanEvtCallback
 *
 * @brief   Function callback for the HOST GAP Scanner module.
 *
 * @param   event - the event that trig this callback
 *          length - length of the data link to the event
 *          pData  - pointer to the data linked to the event.
 *          arg    - user argument
 *
 * @return  none
 */
static void host_tl_scanEvtCallback(uint32_t event, void *pData, uintptr_t arg)
{
  (void) arg;
  if (HCI_TL_CallbackEvtProcessCB)
  {
    scanEvtCallback_t * scanEvtCallback = NULL;

    if (event != GAP_EVT_INSUFFICIENT_MEMORY)
    {
      scanEvtCallback = ICall_mallocLimited(sizeof(scanEvtCallback_t));
    }

    if (scanEvtCallback)
    {
      scanEvtCallback->event = event;
      scanEvtCallback->pData = pData;

      if( !HCI_TL_CallbackEvtProcessCB((void*) scanEvtCallback, (void*) host_tl_scanEvtCallbackProcess))
      {
        // Not enough Heap...
        if (event == GAP_EVT_ADV_REPORT)
        {
          if(pData)
          {
            if (((GapScan_Evt_AdvRpt_t*)pData)->pData)
            {
              ICall_free(((GapScan_Evt_AdvRpt_t*)pData)->pData);
            }
            ICall_free(pData);
          }
        }
        else
        {
          if(pData)
          {
            ICall_free(pData);
          }
        }
        ICall_free(scanEvtCallback);
        /* three is no heap memory exceeded callback here anymore due to app floating */
      }
    }
    else
    {
      if (event == GAP_EVT_ADV_REPORT)
      {
        if(pData)
        {
          if (((GapScan_Evt_AdvRpt_t*)pData)->pData)
          {
            ICall_free(((GapScan_Evt_AdvRpt_t*)pData)->pData);
          }
          ICall_free(pData);
        }
      }
      else
      {
        if(pData)
        {
          ICall_free(pData);
        }
      }
      HCI_TL_sendSystemReport(HOST_TL_ID, LL_STATUS_ERROR_OUT_OF_HEAP, HCI_EXT_GAP_ADV_SCAN_EVENT);
    }
  }
}

/*********************************************************************
 * @fn      host_tl_scanEvtCallbackProcess
 *
 * @brief   Function callback for the HOST GAP Scanner module.
 *
 * @param   event - the event that trig this callback
 *          length - length of the data link to the event
 *          pData  - pointer to the data linked to the event.
 *
 * @return  none
 */
static void host_tl_scanEvtCallbackProcess(scanEvtCallback_t * scanEvtCallback)
{
  uint32_t event;
  uint8_t *pData;

  if (scanEvtCallback)
  {
    event = scanEvtCallback->event;
    pData = scanEvtCallback->pData;

    //Only One event can be signal at a time
    switch(event)
    {
      case GAP_EVT_SCAN_ENABLED:
      case GAP_EVT_SCAN_PRD_ENDED:
      case GAP_EVT_SCAN_DUR_ENDED:
      case GAP_EVT_SCAN_INT_ENDED:
      case GAP_EVT_SCAN_WND_ENDED:
      {
#ifdef BLE3_CMD
        if ( (event == GAP_EVT_SCAN_INT_ENDED) ||
             (event == GAP_EVT_SCAN_WND_ENDED) )
        {
          // send scan notice event
          uint8_t data[5] = {0};
          data[0] = LO_UINT16(HCI_EXT_SCAN_EVENT_NOTICE_EVENT);
          data[1] = HI_UINT16(HCI_EXT_SCAN_EVENT_NOTICE_EVENT);
          data[2] = SUCCESS;
          data[3] = LO_UINT16(HCI_EXT_SCAN_EVENT_NOTICE);
          data[4] = HI_UINT16(HCI_EXT_SCAN_EVENT_NOTICE);

          HCI_TL_SendVSEvent(data, 5);
        }
        else
        {
          if( maxNumReports && (NULL != deviceInfoArr) )
          {
            uint8_t *pDataOut = NULL;
            uint16_t pktLen = 4 + 8*numDev;
            uint8_t i;

            pDataOut = ICall_mallocLimited(pktLen);
            if( pDataOut )
            {
              uint16_t ind = 4;
              pDataOut[0] = LO_UINT16(HCI_EXT_GAP_DEVICE_DISCOVERY_EVENT);
              pDataOut[1] = HI_UINT16(HCI_EXT_GAP_DEVICE_DISCOVERY_EVENT);
              pDataOut[2] = 0;      // Status
              pDataOut[3] = numDev; // Number of devices discovered
              // Copy the devices information
              for (i = 0; i < numDev; i++)
              {
                pDataOut[ind] = deviceInfoArr[i].evtType;
                ind++;
                pDataOut[ind] = deviceInfoArr[i].addrType;
                ind++;
                VOID memcpy(&pDataOut[ind], deviceInfoArr[i].addr, B_ADDR_LEN);
                ind += B_ADDR_LEN;
              }
              HCI_TL_SendVSEvent(pDataOut, pktLen);
              scanSummarySent = 1;
              numDev = 0;

              ICall_free(pDataOut);
            }
          }
        }
#else // !BLE3_CMD
        uint8_t data[7];
        data[0] = LO_UINT16(HCI_EXT_GAP_ADV_SCAN_EVENT);
        data[1] = HI_UINT16(HCI_EXT_GAP_ADV_SCAN_EVENT);
        data[2] = 0;  // Status
        data[3] = BREAK_UINT32(event, 0);
        data[4] = BREAK_UINT32(event, 1);
        data[5] = BREAK_UINT32(event, 2);
        data[6] = BREAK_UINT32(event, 3);
        HCI_TL_SendVSEvent(data, sizeof(data));
#endif // BLE3_CMD
        if (pData)
        {
          ICall_free(pData);
        }
        break;
      }

      case GAP_EVT_SCAN_DISABLED:
      {
#ifdef BLE3_CMD
        if( !scanSummarySent )
        {
          uint8_t data[4];
          data[0] = LO_UINT16(HCI_EXT_GAP_DEVICE_DISCOVERY_EVENT);
          data[1] = HI_UINT16(HCI_EXT_GAP_DEVICE_DISCOVERY_EVENT);
          data[2] = bleGAPUserCanceled; // Status
          data[3] = 0; // numDev;
          // Send the event
          HCI_TL_SendVSEvent(data, sizeof(data));
          numDev = 0;
        }
        else
        {
          scanSummarySent = 0;
        }
#else // !BLE3_CMD
        if (pData)
        {
          GapScan_Evt_End_t * advEndRpt = (GapScan_Evt_End_t *) pData;
          uint8_t data[9];
          data[0] = LO_UINT16(HCI_EXT_GAP_ADV_SCAN_EVENT);
          data[1] = HI_UINT16(HCI_EXT_GAP_ADV_SCAN_EVENT);
          data[2] = 0; // Status
          data[3] = BREAK_UINT32(event, 0);
          data[4] = BREAK_UINT32(event, 1);
          data[5] = BREAK_UINT32(event, 2);
          data[6] = BREAK_UINT32(event, 3);
          data[7] = advEndRpt->reason;    //Reason
          data[8] = advEndRpt->numReport; //Number of Report collected
          HCI_TL_SendVSEvent(data, sizeof(data));
          ICall_free(pData);
          pData = NULL;
        }
#endif // BLE_CMD
        if (pData)
        {
          ICall_free(pData);
        }
        break;
      }

      case GAP_EVT_ADV_REPORT:
      {
        host_tl_sendAdvReport(event, (GapScan_Evt_AdvRpt_t *) pData);
        break;
      }

      default:
      {
        if (pData)
        {
          ICall_free(pData);
        }
        break;
      }
    }
    ICall_free(scanEvtCallback);
  }
}


/*********************************************************************
 * @fn      host_tl_sendAdvReport
 *
 * @brief   Send a advertisement report as a vendor specific event.
 *
 * @param   advRpt - pointer to the report.
 *
 * @return  none
 */
static void host_tl_sendAdvReport(uint32_t event, GapScan_Evt_AdvRpt_t * advRpt)
{
#ifndef BLE3_CMD
  hciPacket_t *msg;
  uint16_t totalLength = 0;
#endif
  uint16_t remainingLength;
  uint8_t dataLen;

  if (advRpt == NULL)
  {
      return;
  }
  remainingLength = advRpt->dataLen;

  // Got the Report, Map it to the Extended Report Event...
  do
  {
    //Check Length, if bigger than MAX_REPORT_DATA_SIZE, split it...
    if (remainingLength > MAX_REPORT_DATA_SIZE)
    {
      dataLen = MAX_REPORT_DATA_SIZE;
    }
    else
    {
      dataLen = remainingLength;
    }
#ifdef BLE3_CMD
    // BLE3 reports only Legacy advertisement
    if( advRpt->evtType & 0x10 )
    {
      if( (maxNumReports) && (numDev < maxNumReports) ||
          (!maxNumReports))
      {
        uint8_t rptLen = 13 + dataLen;
        uint8_t *dataOut = NULL;

        dataOut = (uint8_t *)osal_mem_allocLimited(rptLen);
        if( dataOut )
        {
          // Build the event
          dataOut[0] = LO_UINT16(HCI_EXT_GAP_DEVICE_INFO_EVENT);
          dataOut[1] = HI_UINT16(HCI_EXT_GAP_DEVICE_INFO_EVENT);
          dataOut[2] = 0; // status
          dataOut[3] = getAgamaToBLE3EventProp(advRpt->evtType); // adv type
          dataOut[4] = advRpt->addrType;
          // Peer's address
          osal_memcpy(&dataOut[5], advRpt->addr, B_ADDR_LEN);
          dataOut[11] = (uint8_t)advRpt->rssi;
          dataOut[12] = dataLen;
          // Copy the advData
          osal_memcpy(&dataOut[13], advRpt->pData, dataLen);

          HCI_TL_SendVSEvent(dataOut, rptLen);

          // Saving the device info
          // Do it only once, so in case the report is split into few iterations we won't add the same device multiple times)
          if ( maxNumReports && (NULL != deviceInfoArr ) && (dataLen == remainingLength))
          {
            deviceInfoArr[numDev].evtType = dataOut[3];
            deviceInfoArr[numDev].addrType = dataOut[4];
            osal_memcpy(deviceInfoArr[numDev].addr, advRpt->addr, B_ADDR_LEN);
            numDev++;
          }

          //Free the message and the payload
          ICall_free(dataOut);
        }
      }
    }
#else // !BLE3_CMD
    // OSAL message header + HCI event header + data
    totalLength = sizeof(hciPacket_t) + HCI_EVENT_MIN_LENGTH + \
                  GAP_SCAN_EVENT_HEADER + GAP_SCAN_EVENT_LENGTH + dataLen;

    // allocate memory for OSAL hdr + packet
    msg = (hciPacket_t *)ICall_allocMsg(totalLength);
    if (msg)
    {
      // message type, HCI event type
      msg->hdr.event  = HCI_CTRL_TO_HOST_EVENT;
      msg->hdr.status = 0xFF;

      // packet
      msg->pData    = (uint8_t*)(msg+1);
      msg->pData[0] = HCI_EVENT_PACKET;
      msg->pData[1] = HCI_VE_EVENT_CODE;
      msg->pData[2] = GAP_SCAN_EVENT_LENGTH + GAP_SCAN_EVENT_HEADER + dataLen;

      msg->pData[3] = LO_UINT16(HCI_EXT_GAP_ADV_SCAN_EVENT);
      msg->pData[4] = HI_UINT16(HCI_EXT_GAP_ADV_SCAN_EVENT);
      msg->pData[5] = 0; // Status
      msg->pData[6] = BREAK_UINT32(event, 0);
      msg->pData[7] = BREAK_UINT32(event, 1);
      msg->pData[8] = BREAK_UINT32(event, 2);
      msg->pData[9] = BREAK_UINT32(event, 3);
      msg->pData[10] = advRpt->evtType;  //Start of the event structure.
      msg->pData[11] = advRpt->addrType;
      memcpy(&msg->pData[12], advRpt->addr, B_ADDR_LEN);
      msg->pData[18] = advRpt->primPhy;
      msg->pData[19] = advRpt->secPhy;
      msg->pData[20] = advRpt->advSid;
      msg->pData[21] = advRpt->txPower;
      msg->pData[22] = advRpt->rssi;
      msg->pData[23] = advRpt->directAddrType;
      memcpy(&msg->pData[24], advRpt->directAddr, B_ADDR_LEN);
      msg->pData[30] = LO_UINT16(advRpt->periodicAdvInt);
      msg->pData[31] = HI_UINT16(advRpt->periodicAdvInt);
      msg->pData[32] = LO_UINT16(dataLen);
      msg->pData[33] = HI_UINT16(dataLen);
      // copy data
      if (advRpt->dataLen)
      {
          memcpy(&msg->pData[34], advRpt->pData + (advRpt->dataLen - remainingLength) , dataLen);
      }

      if (remainingLength > MAX_REPORT_DATA_SIZE)
      {
          // This is not the last packet
          msg->pData[10] &= AE_EVT_TYPE_COMPLETE_MASK;
          msg->pData[10] |= AE_EVT_TYPE_INCOMPLETE_MORE_TO_COME;
      }

      // Send to High Layer.
      if (HCI_TL_CommandStatusCB)
      {
        HCI_TL_CommandStatusCB(msg->pData, HCI_EVENT_MIN_LENGTH + msg->pData[2]);
      }

      // We're done with this message.
      ICall_freeMsg(msg);
    }
    else
    {
      HCI_TL_sendSystemReport(HOST_TL_ID, LL_STATUS_ERROR_OUT_OF_HEAP, HCI_EXT_GAP_ADV_SCAN_EVENT);
    }
#endif // BLE3_CMD
    // Update the local variable to send the rest of the payload.
    remainingLength-=dataLen;
  }while(remainingLength > 0);

  //Free the message and the payload
  if (advRpt)
  {
    if (advRpt->pData)
    {
      ICall_free(advRpt->pData);
    }
    ICall_free(advRpt);
  }
}
#endif //CENTRAL_CFG | OBSERVER_CFG

#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )
/*********************************************************************
 * @fn      host_tl_advEvtCallback
 *
 * @brief   Function callback for the HOST GAP advertiser module.
 *
 * @param   handle - handle of the advertisement set trigging the event.
 *          event - the event that trig this callback
 *          length - length of the data link to the event
 *          pData  - pointer to the data linked to the event.
 *
 * @return  none
 */
static void host_tl_advEvtCallback(uint32_t event, void *pData, uint32_t arg)
{
  if (HCI_TL_CallbackEvtProcessCB)
  {
    advEvtCallback_t * advEvtCallback = ICall_malloc(sizeof(advEvtCallback_t));
    if (advEvtCallback)
    {
      advEvtCallback->event = event;
      advEvtCallback->pData = pData;
      if (!HCI_TL_CallbackEvtProcessCB((void*) advEvtCallback, (void*) host_tl_advEvtCallbackProcess))
      {
        // Not enough Heap...
        ICall_free(pData);
        ICall_free(advEvtCallback);
        HCI_TL_sendSystemReport(HCI_TL_ID, LL_STATUS_ERROR_OUT_OF_HEAP, HCI_LE_EVENT_CODE);
      }
    }
    else
    {
     ICall_free(pData);
     HCI_TL_sendSystemReport(HOST_TL_ID, LL_STATUS_ERROR_OUT_OF_HEAP, HCI_EXT_GAP_ADV_SCAN_EVENT);
    }
  }
}

/*********************************************************************
 * @fn      host_tl_advEvtCallback
 *
 * @brief   Function callback for the HOST GAP advertiser module.
 *
 * @param   advEvtCallback - structure containing all info from the event.
 *
 * @return  none
 */
static void host_tl_advEvtCallbackProcess(advEvtCallback_t *advEvtCallback)
{
  if (advEvtCallback)
  {
#ifndef BLE3_CMD
    uint8_t status = SUCCESS;
    uint8_t dataLen = 0;
    // Handle is always the first byte
    uint8_t advHandle = *(uint8_t *)(advEvtCallback->pData);
    // Don't include handle since it was already extracted
    void *pData = (char *)advEvtCallback->pData + 1;
#endif

    uint8_t *dataOut = NULL;
    uint32_t event = advEvtCallback->event;

    switch(event)
    {
#ifdef BLE3_CMD
      case GAP_EVT_ADV_END:
      {
        // Allocate data to send over TL
        uint8_t totalLength = 5;
        dataOut = (uint8_t *)osal_mem_alloc(totalLength);
        dataOut[0] = LO_UINT16(HCI_EXT_ADV_EVENT_NOTICE_EVENT);
        dataOut[1] = HI_UINT16(HCI_EXT_ADV_EVENT_NOTICE_EVENT);
        dataOut[2] = SUCCESS;
        dataOut[3] = LO_UINT16(HCI_EXT_ADV_EVENT_NOTICE);
        dataOut[4] = HI_UINT16(HCI_EXT_ADV_EVENT_NOTICE);

        HCI_TL_SendVSEvent(dataOut, totalLength);
        break;
      }

      case GAP_EVT_ADV_START_AFTER_ENABLE:
      case GAP_EVT_ADV_END_AFTER_DISABLE:
      {
        // Do not use these events for BLE3 Commands
        // for BLE3 use GAP_ADV_MAKE_DISCOVERABLE_DONE_EVENT and GAP_ADV_END_DISCOVERABLE_DONE_EVENT
        break;
      }
#else // !BLE3_CMD
      case GAP_EVT_ADV_SET_TERMINATED:
        // Extract status
        status = ((GapAdv_setTerm_t *)pData)->status;

        // Don't include status as part of data
        pData = &(((GapAdv_setTerm_t *)pData)->connHandle);

        dataLen = sizeof(uint16_t) + // 2 bytes for connHandle
                  sizeof(uint8_t);   // 1 byte for numComAdvEvts
        break;

      case GAP_EVT_SCAN_REQ_RECEIVED:
        dataLen = sizeof(uint8_t) +  // 1 byte for scanAddrType
                  B_ADDR_LEN      +  // Address length
                  sizeof(uint8_t) +  // 1 byte for channel
                  sizeof(uint8_t);   // 1 byte for rssi
        break;

      case GAP_EVT_ADV_DATA_TRUNCATED:
        dataLen = sizeof(uint8_t) +   // 1 byte for availAdvDataLen
                  sizeof(uint16_t);   // 2 byte for advDataLen
        break;

      case GAP_EVT_INSUFFICIENT_MEMORY:
        break;
#endif // BLE3_CMD

      default:
        // The default values set above will be used
        break;
    }

#ifndef BLE3_CMD
    // Allocate data to send over TL
    uint8_t totalLength = HCI_EXT_GAP_ADV_EVENT_MIN_LENGTH + dataLen;
    dataOut = (uint8_t *)osal_mem_alloc(totalLength);

    // If data was allocated
    if(dataOut)
    {
      dataOut[0] = LO_UINT16(HCI_EXT_GAP_ADV_SCAN_EVENT);
      dataOut[1] = HI_UINT16(HCI_EXT_GAP_ADV_SCAN_EVENT);
      dataOut[2] = status;
      dataOut[3] = BREAK_UINT32(event, 0);
      dataOut[4] = BREAK_UINT32(event, 1);
      dataOut[5] = BREAK_UINT32(event, 2);
      dataOut[6] = BREAK_UINT32(event, 3);
      dataOut[7] = advHandle;
      dataOut[8] = dataLen;
      osal_memcpy(&dataOut[9], (uint8_t *) pData, dataLen);

      HCI_TL_SendVSEvent(dataOut, totalLength);
    }
#endif // !BLE3_CMD

    //Free the message and the payload
    if (dataOut)
    {
      ICall_free(dataOut);
    }

    if (advEvtCallback->pData)
    {
      ICall_free(advEvtCallback->pData);
    }

    if (advEvtCallback)
    {
      ICall_free(advEvtCallback);
    }
  }
}
#endif // ( PERIPHERAL_CFG | BROADCASTER_CFG )
#endif /* (PTM_MODE) */

/*********************************************************************
 * @fn      hci_tl_managedAEdata
 *
 * @brief   perform Adv and Scan data update.
 *
 * @param   mode - opCode of the command setting the data.
 *          pCmdData - pointer to the parameter structure describing the data to set.
 *          pData    - pointer to the data to set.
 *
 * @return  none
 */
static void hci_tl_managedAEdata(uint16_t mode, aeSetDataCmd_t *pCmdData, uint8_t *pData)
{
    llStatus_t status = LL_STATUS_ERROR_BAD_PARAMETER;

    // The hypothesis is that setting fragment of advertisement payload will be
    // done for only one Advertisement Set at a time:
    // - no interleaving between adv set
    // - no interleaving between the mode (adv data or scan response data)
    static uint8_t *pFragmentedData = NULL;
    static uint16_t fragmentedDataLen = 0;

    if (!pCmdData)
    {
        status = LL_STATUS_ERROR_INVALID_PARAMS;
    }
    else
    {
        switch(pCmdData->operation)
        {
            case AE_DATA_OP_NEXT_FRAG:
            {
                if ( (pCmdData->dataLen == 0) || (!pFragmentedData))
                {
                  status = LL_STATUS_ERROR_BAD_PARAMETER;
                  break;
                }

                pFragmentedData = hci_tl_appendPendingData(pFragmentedData, fragmentedDataLen, pData, pCmdData->dataLen);
                if (!pFragmentedData)
                {
                    status = LL_STATUS_ERROR_OUT_OF_HEAP;
                    fragmentedDataLen = 0;
                    break;
                }
                fragmentedDataLen += pCmdData->dataLen;

                // Mock the Link Layer Status
                status = LL_STATUS_SUCCESS;
                break;
            }
            case AE_DATA_OP_FIRST_FRAG:
            {
                if (pCmdData->dataLen == 0)
                {
                  status = LL_STATUS_ERROR_BAD_PARAMETER;
                  break;
                }

                if (pFragmentedData)
                {
                  // The buffer was already preallocated, free it and restart from the beginning.
                  hci_tl_removePendingData(pFragmentedData);
                  fragmentedDataLen = 0;
                }

                // Passing NULL as the first parameter prevent to free any existing data.
                // Data will be used only if the fragmentation is successful, meaning:
                // First --> intermediate --> Last segment  happens.
                // if the sequence does not happens, all pre-allocated buffer will be free
                // but the existing data will still exist and be used.
                pFragmentedData = hci_tl_createPendingData(NULL, pCmdData->dataLen, pData);
                if (!pFragmentedData)
                {
                    status = LL_STATUS_ERROR_OUT_OF_HEAP;
                    break;
                }
                fragmentedDataLen = pCmdData->dataLen;

                // Mock the Link Layer Status
                status = LL_STATUS_SUCCESS;
                break;
            }
            case AE_DATA_OP_LAST_FRAG:
            {
                if ((pCmdData->dataLen == 0) || (!pFragmentedData))
                {
                  status = LL_STATUS_ERROR_BAD_PARAMETER;
                  break;
                }

                pFragmentedData = hci_tl_appendPendingData(pFragmentedData, fragmentedDataLen, pData, pCmdData->dataLen);
                if (!pFragmentedData)
                {
                    status = LL_STATUS_ERROR_OUT_OF_HEAP;
                    fragmentedDataLen = 0;
                    break;
                }
                fragmentedDataLen += pCmdData->dataLen;

                // Assign the data to the set
                if (pCmdData->pData)
                {
                  ICall_free(pCmdData->pData);
                }
                pCmdData->pData = pFragmentedData;

                //Overwrite the command parameter structure.
                pCmdData->dataLen = fragmentedDataLen;
                pCmdData->operation = AE_DATA_OP_COMPLETE;

                // Reset the temporary buffer to NULL, so it can be reused properly
                pFragmentedData = NULL;
                fragmentedDataLen = 0;

                if (mode == HCI_LE_SET_EXT_ADV_DATA)
                {
                    status = LE_SetExtAdvData(pCmdData);
                }
                else if (mode == HCI_LE_SET_EXT_SCAN_RESPONSE_DATA)
                {
                    // Scan Response Data
                    status = LE_SetExtScanRspData(pCmdData);
                }
                else
                {
                    // other mode like HCI_EXT_LE_SET_EXT_ADV_DATA should
                    // never be used with fragmentation
                    status = LL_STATUS_ERROR_BAD_PARAMETER;
                    // Failure, free the pre-allocated buffer...
                    hci_tl_removePendingData(pCmdData->pData);
                    break;
                }

                if (status != LL_STATUS_SUCCESS)
                {
                    // Failure, free the pre-allocated buffer...
                    hci_tl_removePendingData(pCmdData->pData);
                }
                break;
            }
            case AE_DATA_OP_UNCHANGED:
            {
                if ((mode == HCI_LE_SET_EXT_ADV_DATA) ||
                    (mode == HCI_EXT_LE_SET_EXT_ADV_DATA))
                {
                  status = LE_SetExtAdvData(pCmdData);
                }
                else
                {
                  status = LL_STATUS_ERROR_BAD_PARAMETER;
                }
                break;
            }
            case AE_DATA_OP_COMPLETE:
            {
                // Free the previous allocated data and allocate the a pointer for the new data
                pCmdData->pData = hci_tl_createPendingData(pCmdData->pData, pCmdData->dataLen, pData);

                if(pCmdData->dataLen == 0 || pCmdData->pData != NULL)
                {
                    if ((mode == HCI_LE_SET_EXT_ADV_DATA)     ||
                        (mode == HCI_EXT_LE_SET_EXT_ADV_DATA) ||
                        (mode == HCI_LE_SET_ADV_DATA))
                    {
                        status = LE_SetExtAdvData(pCmdData);
                    }
                    else
                    {
                        // Scan Response Data
                        status = LE_SetExtScanRspData(pCmdData);
                    }

                    if (status != LL_STATUS_SUCCESS)
                    {
                        // Failure, free the pre-allocated buffer...
                        hci_tl_removePendingData(pCmdData->pData);
                    }
                }
                else
                {
                    status = LL_STATUS_ERROR_OUT_OF_HEAP;
                }
                break;
            }
            default:
                status = LL_STATUS_ERROR_BAD_PARAMETER;
                break;
        }
    }
    HCI_CommandCompleteEvent(mode,
                             sizeof(status),
                             &status);
}

/*********************************************************************
 * @fn      hci_tl_removePendingData
 *
 * @brief   remove any pending Adv data setup that was not complete..
 *
 * @param   pendingAdvData - pointer to teh data to remove
 *
 * @return  none
 */
static void hci_tl_removePendingData(uint8_t* pendingAdvData)
{
    if (pendingAdvData)
    {
        ICall_free(pendingAdvData);
    }
}

/*********************************************************************
 * @fn      hci_tl_createPendingData
 *
 * @brief   Copy the payload to a new allocated buffer.
 *          if the pointer to the previous data is not NULL, then it
 *          will be free first then reassign.
 *
 * @param   pStorage - pointer to the previous storage of the data
 *          len - length of the data payload
 *          pData - pointer to the data payload
 *
 *
 * @return  return new pointer to the new allocated buffer
 */
static uint8_t* hci_tl_createPendingData(uint8_t *pStorage, uint16_t len, uint8_t* pData)
{
    uint8_t* pDataStore = NULL;
    if(len > 0)
    {
        pDataStore = ICall_malloc(len);
        if(pDataStore != NULL)
        {
            memcpy(pDataStore, pData, len);
        }
    }
    if(pStorage != NULL)
    {
        ICall_free(pStorage);
    }
    return pDataStore;
}

/*********************************************************************
 * @fn      hci_tl_appendPendingData
 *
 * @brief   append a data buffer after another one.
 *          a new buffer will be allocated, then the initial buffer will be copy into it.
 *          the new data will be appended, then the initial buffer will be free.
 *
 * @param   pStorage - pointer to the initial storage of the data
 *          len - length of the initial storage
 *          pData - pointer to the data payload to append
 *          len - length of the data payload to append
 *
 *
 * @return  return new pointer to the new allocated buffer
 */
static uint8_t* hci_tl_appendPendingData(uint8_t *pStorage, uint16_t lenStorage,
                                         uint8_t* pData, int16_t len)
{
    uint8_t* pDataStore;

    if(pStorage)
    {
        pDataStore = ICall_malloc(lenStorage + len);
        if (!pDataStore)
        {
            // Not enough Heap, free everything.
            ICall_free(pStorage);
            return pDataStore;
        }
        memcpy(pDataStore, pStorage, lenStorage);
        memcpy(pDataStore + lenStorage, pData, len);
        ICall_free(pStorage);
    }
    else
    {
        // NULL pointer pass has parameter, wrong behavior, free everything.
        return NULL;
    }
    return pDataStore;
}

/*********************************************************************
 * @fn      hci_tl_GetAdvSet
 *
 * @brief   Get a set from the AE linked list. if the set does not exist,
 *          creates one.
 *
 * @param   handle - handle of the set to add.
 *
 * @return  the pointer to the set, or NULL if out of memory to allocated the set.
 */
static hci_tl_advSet_t *hci_tl_GetAdvSet(uint8_t handle)
{
    hci_tl_advSet_t *pAdvSet = hci_tl_advSetList;

    // search for handle
    while(pAdvSet)
    {
      if (handle == pAdvSet->handle)
      {
        return(pAdvSet);
      }

      if (pAdvSet->next == NULL)
      {
        break;
      }

      // advance to next entry
      pAdvSet = pAdvSet->next;
    }

    // check if we have a valid pointer
    // Note: A valid pointer means the list wasn't empty, and we have last entry.
    //       A NULL pointer means the list was empty.
   if (pAdvSet)
    {
        pAdvSet->next = ICall_malloc(sizeof(hci_tl_advSet_t));
        pAdvSet = pAdvSet->next;
    }
    else // Adv Set List is empty
    {
        pAdvSet = (hci_tl_advSet_t *) ICall_malloc(sizeof(hci_tl_advSet_t));
        hci_tl_advSetList = pAdvSet;
    }

    if (pAdvSet)
    {
        memset(pAdvSet, 0, sizeof(hci_tl_advSet_t));
        pAdvSet->advCmdParams.handle = ADV_INVALID_SET_HANDLE;
        // set the handle
        pAdvSet->handle = handle;
    }
    return(pAdvSet);

}

/*********************************************************************
 * @fn      hci_tl_InitAdvSetParams
 *
 * @brief   Init a set params to default legacy.
 *
 * @param   handle - handle of the set to remove.
 *
 * @return  none.
 */
static void hci_tl_InitAdvSetParams(hci_tl_advSet_t *pAdvSet)
{
  pAdvSet->advCmdParams.handle          = ADV_LEGACY_SET_HANDLE;
  pAdvSet->advCmdParams.eventProps[0]   = LO_UINT16(AE_EVT_TYPE_CONN_SCAN_ADV_IND);
  pAdvSet->advCmdParams.eventProps[1]   = HI_UINT16(AE_EVT_TYPE_CONN_SCAN_ADV_IND);
  pAdvSet->advCmdParams.primIntMin[0]   = 0xa0; //100ms
  pAdvSet->advCmdParams.primIntMin[1]   = 0;
  pAdvSet->advCmdParams.primIntMin[2]   = 0;
  pAdvSet->advCmdParams.primIntMax[0]   = 0xa0; //100ms
  pAdvSet->advCmdParams.primIntMax[1]   = 0;
  pAdvSet->advCmdParams.primIntMax[2]   = 0;
  pAdvSet->advCmdParams.primChanMap     = LL_ADV_CHAN_ALL;
  pAdvSet->advCmdParams.ownAddrType     = LL_DEV_ADDR_TYPE_PUBLIC;
  pAdvSet->advCmdParams.peerAddrType    = LL_DEV_ADDR_TYPE_PUBLIC;
  memset(pAdvSet->advCmdParams.peerAddr, 0, B_ADDR_LEN);
  pAdvSet->advCmdParams.filterPolicy    = LL_ADV_AL_POLICY_ANY_REQ;
  pAdvSet->advCmdParams.txPower         = AE_TX_POWER_NO_PREFERENCE;
  pAdvSet->advCmdParams.primPhy         = AE_PHY_1_MBPS;
  pAdvSet->advCmdParams.secMaxSkip      = 0;
  pAdvSet->advCmdParams.secPhy          = AE_PHY_1_MBPS;
  pAdvSet->advCmdParams.sid             = 0;
  pAdvSet->advCmdParams.notifyEnableFlags = 0;
}

/*********************************************************************
 * @fn      hci_tl_RemoveAdvSet
 *
 * @brief   remove a set from the AE linked list.
 *
 * @param   handle - handle of the set to remove.
 *
 * @return  none.
 */
static void hci_tl_RemoveAdvSet(uint8_t handle)
{
  hci_tl_advSet_t *pPre = hci_tl_advSetList;
  hci_tl_advSet_t *pCur = hci_tl_advSetList;

  while(pCur)
  {
    if (pCur->handle == handle)
    {
      // chain over the handle to be removed
      pPre->next = pCur->next;

      if (pCur == hci_tl_advSetList)
      {
          hci_tl_advSetList = pCur->next;
      }

      // remove advertising set
      if (pCur->advCmdData.pData)
      {
          ICall_free(pCur->advCmdData.pData);
      }
      if (pCur->scanCmdData.pData)
      {
          ICall_free(pCur->scanCmdData.pData);
      }
      ICall_free(pCur);
      return;
    }

    // not found yet, so on to next entry in list
    pPre = pCur;
    pCur = pCur->next;
  }
}

/*********************************************************************
 * @fn      HCI_TL_SendDataPkt
 *
 * @brief   Interprets an HCI data packet and sends to the Stack.
 *
 * @param   pMsg - HCI data serial packet to translate.
 *
 * @return  none.
 */
void HCI_TL_SendDataPkt(uint8_t *pMsg)
{
#ifndef HOST_CONFIG
  /* Host configuration should never access directly HCI_TL_SendDataPkt */
  hciDataPacket_t *pDataPkt = (hciDataPacket_t *) pMsg;

  // LE only accepts Data packets of type ACL.
  if ((pDataPkt) && (pDataPkt->pktType == HCI_ACL_DATA_PACKET))
  {
    // Allocate data with bm data
    uint8_t *pData = (uint8_t *) HCI_bm_alloc(pDataPkt->pktLen);

    if ((pDataPkt->pData) && (pData))
    {
      memcpy(pData, pDataPkt->pData, pDataPkt->pktLen);

      if (HCI_SendDataPkt(pDataPkt->connHandle,
                          pDataPkt->pbFlag,
                          pDataPkt->pktLen,
                          pData) != HCI_SUCCESS )
      {
        HCI_bm_free(pData);
      }
    }
  }
#endif /* HOST_CONFIG */
}

static void HCI_TL_SendVSEvent(uint8_t *pBuf, uint16_t dataLen)
{
  hciPacket_t *msg;
  uint16_t totalLength;

  // OSAL message header + HCI event header + data
  totalLength = sizeof(hciPacket_t) + HCI_EVENT_MIN_LENGTH + dataLen;

  // Check for overlap - dataLen is too big, drop packet
  if (totalLength < dataLen)
  {
    return;
  }

  // allocate memory for OSAL hdr + packet
  msg = (hciPacket_t *)ICall_allocMsgLimited(totalLength);

  if (msg)
  {
    // If data length is smaller than 0x100 bytes, process as regular event
    if (dataLen < 0x100)
    {
      // message type, HCI event type
      msg->hdr.event  = HCI_CTRL_TO_HOST_EVENT;
      msg->hdr.status = 0xFF;

      // packet
      msg->pData    = (uint8_t*)(msg+1);
      msg->pData[0] = HCI_EVENT_PACKET;
      msg->pData[1] = HCI_VE_EVENT_CODE;
      msg->pData[2] = dataLen;
      // copy data
      if (dataLen)
      {
        memcpy(&(msg->pData[3]), pBuf, dataLen);
      }
    }
    // If data length is larger than 0x00FF bytes, process as extended event
    else
    {
      // message type, HCI extended event type
      msg->hdr.event  = HCI_CTRL_TO_HOST_EVENT;
      msg->hdr.status = 0xFF;

      msg->pData    = (uint8_t*)(msg+1);
      msg->pData[0] = HCI_EXTENDED_EVENT_PACKET;
      msg->pData[1] = LO_UINT16(dataLen);
      msg->pData[2] = HI_UINT16(dataLen);
      memcpy(&(msg->pData[3]), pBuf, dataLen);
    }
    // Send to High Layer.
    if (HCI_TL_CommandStatusCB)
    {
      HCI_TL_CommandStatusCB(msg->pData, HCI_EVENT_MIN_LENGTH + dataLen);
    }

    // We're done with this message.
    ICall_freeMsg(msg);
  }
}

/************************** Host Command Translation **************************/
#ifdef HOST_CONFIG

#if !defined (PTM_MODE)
// PTM_MODE does not interact with teh host (L2CAP, GAP, ATT, Etc...)
/*********************************************************************
 * @fn      HCI_TL_processStructuredEvent
 *
 * @brief   Interprets a structured Event from the BLE Host and serializes it.
 *
 * @param   pEvt - structured event to serialize.
 *
 * @return  TRUE to deallocate pEvt message, False otherwise.
 */
uint8_t HCI_TL_processStructuredEvent(ICall_Hdr *pEvt)
{
  return(processEvents(pEvt));
}

/*********************************************************************
 * @fn      HCI_TL_SendExtensionCmdPkt
 *
 * @brief   Interprets an HCI extension command packet and sends to the Stack.
 *
 * @param   pMsg - HCI command serial packet to translate.
 *
 * @return  none.
 */
static void HCI_TL_SendExtensionCmdPkt(hciPacket_t *pMsg)
{
  // Process Extension messages not bound for the HCI layer.
  processExtMsg(pMsg);
}


/*********************************************************************
 * @fn      processExtMsg
 *
 * @brief   Parse and process incoming HCI extension messages.
 *
 * @param   pMsg - incoming HCI extension message.
 *
 * @return  none
 */
static uint8_t processExtMsg(hciPacket_t *pMsg)
{
  uint8_t deallocateIncoming;
  bStatus_t stat = SUCCESS;
  uint8_t rspDataLen = 0;
  hciExtCmd_t msg;
  uint8_t *pBuf = pMsg->pData;

  // Parse the header
  msg.pktType = *pBuf++;
  msg.opCode = BUILD_UINT16(pBuf[0], pBuf[1]);
  pBuf += 2;

  if (msg.pktType == HCI_EXTENDED_CMD_PACKET)
  {
    msg.len = BUILD_UINT16(*pBuf,*(pBuf+1)) ;
    pBuf+=2;
  }
  else
  {
    msg.len = *pBuf++;
  }
  msg.pData = pBuf;

  switch(msg.opCode >> 7)
  {
#if defined(HOST_CONFIG)
#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
    case HCI_EXT_L2CAP_SUBGRP:
      stat = processExtMsgL2CAP((msg.opCode & 0x007F), &msg, &rspDataLen);
      break;

    case HCI_EXT_ATT_SUBGRP:
      stat = processExtMsgATT((msg.opCode & 0x007F), &msg);
      break;

    case HCI_EXT_GATT_SUBGRP:
      stat = processExtMsgGATT((msg.opCode & 0x007F), &msg, &rspDataLen);
      break;
#endif /* (CENTRAL_CFG | PERIPHERAL_CFG) */
    case HCI_EXT_GAP_SUBGRP:
      stat = processExtMsgGAP((msg.opCode & 0x007F), &msg, &rspDataLen);
      break;
#endif // HOST_CONFIG

    case HCI_EXT_UTIL_SUBGRP:
      stat = processExtMsgUTIL((msg.opCode & 0x007F), &msg, &rspDataLen);
      break;

    default:
      stat = FAILURE;
      break;
  }

  deallocateIncoming = FALSE;

  // Send back an immediate response
  rspBuf[0] = LO_UINT16(HCI_EXT_GAP_CMD_STATUS_EVENT);
  rspBuf[1] = HI_UINT16(HCI_EXT_GAP_CMD_STATUS_EVENT);
  rspBuf[2] = stat;
  rspBuf[3] = LO_UINT16(0xFC00 | msg.opCode);
  rspBuf[4] = HI_UINT16(0xFC00 | msg.opCode);
  rspBuf[5] = rspDataLen;

  // IMPORTANT!! Fill in Payload (if needed) in case statement

  HCI_TL_SendVSEvent(rspBuf, (6 + rspDataLen));

#ifdef BLE3_CMD
  if( sendEstEvt )
  {
    // An error occurred when "HCI_EXT_GAP_EST_LINK_REQ" was called
    uint8_t pDataOut[20] = {0};

    pDataOut[0] = LO_UINT16(HCI_EXT_GAP_LINK_ESTABLISHED_EVENT);
    pDataOut[1] = HI_UINT16(HCI_EXT_GAP_LINK_ESTABLISHED_EVENT);
    pDataOut[2] = bleIncorrectMode;
    pDataOut[12] = GAP_PROFILE_CENTRAL;

    HCI_TL_SendVSEvent(pDataOut, 20);
    sendEstEvt = 0;
  }

#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )
  if( maxConnReached )
  {
    uint8_t pDataOut[3] = {0};

    // Send Make discoverable done with max connection reached
    pDataOut[0] = LO_UINT16(HCI_EXT_GAP_MAKE_DISCOVERABLE_DONE_EVENT);
    pDataOut[1] = HI_UINT16(HCI_EXT_GAP_MAKE_DISCOVERABLE_DONE_EVENT);
    pDataOut[2] = HCI_ERROR_CODE_CONN_LIMIT_EXCEEDED;

    HCI_TL_SendVSEvent(pDataOut, 3);
    maxConnReached = 0;
  }
#endif // HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG )

#endif// BLE3_CMD

  return(deallocateIncoming);
}

/*********************************************************************
 * @fn      processExtMsgUTIL
 *
 * @brief   Parse and process incoming HCI extension UTIL messages.
 *
 * @param   cmdID - incoming HCI extension command ID.
 * @param   pCmd - incoming HCI extension message.
 * @param   pRspDataLen - response data length to be returned.
 *
 * @return  SUCCESS, INVALIDPARAMETER and FAILURE.
 */
static uint8_t processExtMsgUTIL(uint8_t cmdID, hciExtCmd_t *pCmd, uint8_t *pRspDataLen)
{
  bStatus_t stat = SUCCESS;

  *pRspDataLen = 0;

  switch(cmdID)
  {
    case HCI_EXT_UTIL_NV_READ:
      {
        uint8_t *pBuf = pCmd->pData;
        osalSnvId_t id  = BUILD_UINT16(pBuf[0], pBuf[1]);
        osalSnvLen_t len = pBuf[2];

        // This has a limitation of only allowing a max data length because of the fixed buffer.
        if ((len < MAX_RSP_DATA_LEN) && (checkNVLen(id, len) == SUCCESS))
        {
          stat = osal_snv_read(id, len, &rspBuf[RSP_PAYLOAD_IDX]);
          if (stat == SUCCESS)
          {
            *pRspDataLen = pBuf[2];
          }
        }
        else
        {
          stat = INVALIDPARAMETER;
        }
      }
      break;

    case HCI_EXT_UTIL_NV_WRITE:
      {
        uint8_t *pBuf = pCmd->pData;
        osalSnvId_t id  = BUILD_UINT16(pBuf[0], pBuf[1]);
        osalSnvLen_t len = pBuf[2];
        if (checkNVLen(id, len) == SUCCESS)
        {
          stat = osal_snv_write(id, len, &pBuf[3]);
        }
        else
        {
          stat = INVALIDPARAMETER;
        }
      }
      break;

    case HCI_EXT_UTIL_BUILD_REV:
      {
        ICall_BuildRevision buildRev;

        VOID buildRevision(&buildRev);

        // Stack revision
        //  Byte 0: Major
        //  Byte 1: Minor
        //  Byte 2: Patch
        rspBuf[RSP_PAYLOAD_IDX]   = BREAK_UINT32(buildRev.stackVersion, 0);
        rspBuf[RSP_PAYLOAD_IDX+1] = BREAK_UINT32(buildRev.stackVersion, 1);
        rspBuf[RSP_PAYLOAD_IDX+2] = BREAK_UINT32(buildRev.stackVersion, 2);

        // Build revision
        rspBuf[RSP_PAYLOAD_IDX+3] = LO_UINT16(buildRev.buildVersion);
        rspBuf[RSP_PAYLOAD_IDX+4] = HI_UINT16(buildRev.buildVersion);

        // Stack info (Byte 5)
        rspBuf[RSP_PAYLOAD_IDX+5] = buildRev.stackInfo;

        // Controller info - part 1 (Byte 6)
        rspBuf[RSP_PAYLOAD_IDX+6] = LO_UINT16(buildRev.ctrlInfo);

        // Controller info - part 2 (Byte 7)
        rspBuf[RSP_PAYLOAD_IDX+7] = 0; // reserved

        // Host info - part 1 (Byte 8)
        rspBuf[RSP_PAYLOAD_IDX+8] = LO_UINT16(buildRev.hostInfo);

        // Host info - part 2 (Byte 9)
        rspBuf[RSP_PAYLOAD_IDX+9] = 0; // reserved

        *pRspDataLen = 10;
      }
      break;
    case HCI_EXT_UTIL_GET_MEM_STATS:
      {
        uint8_t memStatCmd  = pCmd->pData[0];
        HCI_TL_getMemStats(memStatCmd, &rspBuf[RSP_PAYLOAD_IDX], pRspDataLen);
      }
      break;


    default:
      stat = FAILURE;
      break;
  }

  return(stat);
}



#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
/*********************************************************************
 * @fn      processExtMsgL2CAP
 *
 * @brief   Parse and process incoming HCI extension L2CAP messages.
 *
 * @param   pCmd - incoming HCI extension message.
 *
 * @return  SUCCESS or FAILURE
 */
static uint8_t processExtMsgL2CAP(uint8_t cmdID, hciExtCmd_t *pCmd, uint8_t *pRspDataLen)
{
  uint8_t *pBuf = pCmd->pData;
  uint16_t connHandle = BUILD_UINT16(pBuf[0], pBuf[1]); // connHandle, CID or PSM
  l2capSignalCmd_t cmd;
  bStatus_t stat;

  switch(cmdID)
  {
#if defined(BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
    case HCI_EXT_L2CAP_DATA:
      if (pCmd->len > 4)
      {
        uint8_t *pPayload = createMsgPayload(&pBuf[4], pCmd->len - 4);
        if (pPayload != NULL)
        {
          l2capPacket_t pkt;

          pkt.connHandle = connHandle;
          pkt.CID = BUILD_UINT16(pBuf[2], pBuf[3]);
          pkt.pPayload = pPayload;
          pkt.len = pCmd->len - 4;

          // Send SDU over dynamic channel
          stat = L2CAP_SendSDU(&pkt);
          if (stat != SUCCESS)
          {
            MAP_osal_bm_free(pPayload);
          }
        }
        else
        {
          stat = bleMemAllocError;
        }
      }
      else
      {
        stat = FAILURE;
      }
      break;

    case HCI_EXT_L2CAP_REGISTER_PSM:
      if (pCmd->len == 10)
      {
        l2capPsm_t psm;

        psm.psm = connHandle; // connHandle is PSM here
        psm.mtu = BUILD_UINT16(pBuf[2], pBuf[3]);
        psm.mps = psm.mtu - L2CAP_HDR_SIZE; // MPS corresponds to the maximum PDU payload size (after L2CAP header reduction)
        psm.initPeerCredits = BUILD_UINT16(pBuf[4], pBuf[5]);
        psm.peerCreditThreshold = BUILD_UINT16(pBuf[6], pBuf[7]);
        psm.maxNumChannels = pBuf[8];
        psm.pfnVerifySecCB = pBuf[9] ? l2capVerifySecCB : NULL;
        psm.taskId = PROXY_ID(appTaskID);

        stat = L2CAP_RegisterPsm(&psm);
      }
      else
      {
        stat = FAILURE;
      }
      break;

    case HCI_EXT_L2CAP_DEREGISTER_PSM:
      if (pCmd->len == 2)
      {
        stat = L2CAP_DeregisterPsm(appTaskID, connHandle); // connHandle is PSM here
      }
      else
      {
        stat = FAILURE;
      }
      break;

    case HCI_EXT_L2CAP_PSM_INFO:
      if (pCmd->len == 2)
      {
        l2capPsmInfo_t info;

        stat = L2CAP_PsmInfo(connHandle, &info); // connHandle is PSM here
        if (stat == SUCCESS)
        {
          *pRspDataLen = 10;
          rspBuf[RSP_PAYLOAD_IDX]   = LO_UINT16(info.mtu);
          rspBuf[RSP_PAYLOAD_IDX+1] = HI_UINT16(info.mtu);

          rspBuf[RSP_PAYLOAD_IDX+2] = LO_UINT16(info.mps);
          rspBuf[RSP_PAYLOAD_IDX+3] = HI_UINT16(info.mps);

          rspBuf[RSP_PAYLOAD_IDX+4] = LO_UINT16(info.initPeerCredits);
          rspBuf[RSP_PAYLOAD_IDX+5] = HI_UINT16(info.initPeerCredits);

          rspBuf[RSP_PAYLOAD_IDX+6] = LO_UINT16(info.peerCreditThreshold);
          rspBuf[RSP_PAYLOAD_IDX+7] = HI_UINT16(info.peerCreditThreshold);

          rspBuf[RSP_PAYLOAD_IDX+8] = info.maxNumChannels;
          rspBuf[RSP_PAYLOAD_IDX+9] = info.numActiveChannels;
        }
      }
      else
      {
        stat = FAILURE;
      }
      break;

    case HCI_EXT_L2CAP_PSM_CHANNELS:
      if (pCmd->len == 2)
      {
        l2capPsmInfo_t info;

        stat = L2CAP_PsmInfo(connHandle, &info); // connHandle is PSM here
        if ((stat == SUCCESS) && (info.numActiveChannels > 0))
        {
          uint8_t numCIDs = info.numActiveChannels;
          uint16_t sizeOfInfo = sizeof(l2capLocalChannelInfo_t) * numCIDs;
          uint16_t *pCIDs = (uint16_t *)ICall_malloc(sizeOfInfo);
          if (pCIDs != NULL)
          {
            stat = L2CAP_PsmChannels(connHandle, numCIDs, pCIDs);
            if (stat == SUCCESS)
            {
              uint8_t *pRspBuf = &rspBuf[RSP_PAYLOAD_IDX];
              uint8_t i;

              for (i = 0; i < sizeOfInfo; i++)
              {
                *pRspBuf++ = LO_UINT16(pCIDs[i]);
                *pRspBuf++ = HI_UINT16(pCIDs[i]);
              }

              *pRspDataLen = numCIDs * 4;
            }

            VOID ICall_free(pCIDs);
          }
          else
          {
            stat = bleMemAllocError;
          }
        }
      }
      else
      {
        stat = FAILURE;
      }
      break;

    case HCI_EXT_L2CAP_CHANNEL_INFO:
      if (pCmd->len == 4)
      {
        l2capChannelInfo_t channelInfo;

        uint16_t CID = BUILD_UINT16(pBuf[2], pBuf[3]);
        stat = L2CAP_ChannelInfo(connHandle, CID, &channelInfo);
        if (stat == SUCCESS)
        {
          rspBuf[RSP_PAYLOAD_IDX] = channelInfo.state;
          rspBuf[RSP_PAYLOAD_IDX + 1] = LO_UINT16(connHandle);
          rspBuf[RSP_PAYLOAD_IDX + 2] = HI_UINT16(connHandle);

          *pRspDataLen = 3 + buildCoChannelInfo(CID, &channelInfo.info,
                                                 &rspBuf[RSP_PAYLOAD_IDX + 3]);
        }
      }
      else
      {
        stat = FAILURE;
      }
      break;

    case L2CAP_CONNECT_IND:
      if (pCmd->len == 6)
      {
        uint16_t psm = BUILD_UINT16(pBuf[2], pBuf[3]);
        uint16_t peerPsm = BUILD_UINT16(pBuf[4], pBuf[5]);

        stat = L2CAP_ConnectReq(connHandle, psm, peerPsm);
      }
      else
      {
        stat = FAILURE;
      }
      break;

    case L2CAP_CONNECT_RSP:
      if (pCmd->len == 5)
      {
        uint16_t result = BUILD_UINT16(pBuf[3], pBuf[4]);

        stat = L2CAP_ConnectRsp(connHandle, pBuf[2], result);
      }
      else
      {
        stat = FAILURE;
      }
      break;

    case L2CAP_DISCONNECT_REQ:
      if (pCmd->len == 4)
      {
        uint16_t CID = BUILD_UINT16(pBuf[2], pBuf[3]);
        stat = L2CAP_DisconnectReq(connHandle, CID);
      }
      else
      {
        stat = FAILURE;
      }
      break;

    case L2CAP_FLOW_CTRL_CREDIT:
      // pBuf[2] because the first two bytes are the connection handle
      stat = L2CAP_ParseFlowCtrlCredit(&cmd, &(pBuf[2]), pCmd->len-2);
      if (stat == SUCCESS)
      {
        stat = L2CAP_FlowCtrlCredit(connHandle, cmd.credit.CID, cmd.credit.credits);
      }
      break;
#endif //(BLE_V41_FEATURES & L2CAP_COC_CFG)

    case L2CAP_PARAM_UPDATE_REQ:
      stat = L2CAP_ParseParamUpdateReq(&cmd, &pBuf[2], pCmd->len-2);
      if (stat == SUCCESS)
      {
        stat =  L2CAP_ConnParamUpdateReq(connHandle, &cmd.updateReq, appTaskID);
      }
      break;

    case L2CAP_INFO_REQ:
      stat = L2CAP_ParseInfoReq(&cmd, &pBuf[2], pCmd->len-2);
      if (stat == SUCCESS)
      {
        stat = L2CAP_InfoReq(connHandle, &cmd.infoReq, appTaskID);
      }
      break;

    default:
      stat = FAILURE;
      break;
  }

  return(stat);
}

/*********************************************************************
 * @fn      processExtMsgATT
 *
 * @brief   Parse and process incoming HCI extension ATT messages.
 *
 * @param   pCmd - incoming HCI extension message.
 *
 * @return  SUCCESS, INVALIDPARAMETER, FAILURE,
 *          bleInvalidPDU, bleInsufficientAuthen,
 *          bleInsufficientKeySize, bleInsufficientEncrypt or bleMemAllocError
 */
static uint8_t processExtMsgATT(uint8_t cmdID, hciExtCmd_t *pCmd)
{
  static uint8_t numPrepareWrites = 0;
  static attPrepareWriteReq_t *pPrepareWrites = NULL;
  uint8_t *pBuf, *pPayload = NULL, safeToDealloc = TRUE;
  uint16_t connHandle;
  attMsg_t msg;
  bStatus_t stat = bleInvalidPDU;

  // Make sure received buffer contains at least connection handle (2 octets)
  if (pCmd->len < 2)
  {
    return(stat);
  }

  pBuf = pCmd->pData;
  connHandle = BUILD_UINT16(pBuf[0], pBuf[1]);

  switch(cmdID)
  {
    case ATT_ERROR_RSP:
      if (ATT_ParseErrorRsp(&pBuf[2], pCmd->len-2, &msg) == SUCCESS)
      {
        stat = ATT_ErrorRsp(connHandle, &msg.errorRsp);
      }
      break;

    case ATT_EXCHANGE_MTU_REQ:
      if (ATT_ParseExchangeMTUReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                    pCmd->len-2, &msg) == SUCCESS)
      {
        stat = GATT_ExchangeMTU(connHandle, &msg.exchangeMTUReq, appTaskID);
      }
      break;

    case ATT_EXCHANGE_MTU_RSP:
      if (ATT_ParseExchangeMTURsp(&pBuf[2], pCmd->len-2, &msg) == SUCCESS)
      {
        stat = ATT_ExchangeMTURsp(connHandle, &msg.exchangeMTURsp);
      }
      break;

    case ATT_FIND_INFO_REQ:
      if (ATT_ParseFindInfoReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                 pCmd->len-2, &msg) == SUCCESS)
      {
        stat = GATT_DiscAllCharDescs(connHandle, msg.findInfoReq.startHandle,
                                      msg.findInfoReq.endHandle, appTaskID);
      }
      break;

    case ATT_FIND_INFO_RSP:
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParseFindInfoRsp(pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          stat = ATT_FindInfoRsp(connHandle, &msg.findInfoRsp);
          if (stat == SUCCESS)
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_FIND_BY_TYPE_VALUE_REQ:
      if (ATT_ParseFindByTypeValueReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                        pCmd->len-2, &msg) == SUCCESS)
      {
        attFindByTypeValueReq_t *pReq = &msg.findByTypeValueReq;

        // Find out what's been requested
        if (gattPrimaryServiceType(pReq->type)     &&
             (pReq->startHandle == GATT_MIN_HANDLE) &&
             (pReq->endHandle   == GATT_MAX_HANDLE))
        {
          // Discover primary service by service UUID
          stat = GATT_DiscPrimaryServiceByUUID(connHandle, pReq->pValue,
                                                pReq->len, appTaskID);
        }
        else
        {
          stat = INVALIDPARAMETER;
        }
      }
      break;

    case ATT_FIND_BY_TYPE_VALUE_RSP:
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParseFindByTypeValueRsp(pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          stat = ATT_FindByTypeValueRsp(connHandle, &msg.findByTypeValueRsp);
          if (stat == SUCCESS)
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_READ_BY_TYPE_REQ:
      if (ATT_ParseReadByTypeReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                   pCmd->len-2, &msg) == SUCCESS)
      {
        attReadByTypeReq_t *pReq = &msg.readByTypeReq;

        // Find out what's been requested
        if (gattIncludeType(pReq->type))
        {
          // Find included services
          stat = GATT_FindIncludedServices(connHandle, pReq->startHandle,
                                            pReq->endHandle, appTaskID);
        }
        else if (gattCharacterType(pReq->type))
        {
          // Discover all characteristics of a service
          stat = GATT_DiscAllChars(connHandle, pReq->startHandle,
                                    pReq->endHandle, appTaskID);
        }
        else
        {
          // Read using characteristic UUID
          stat = GATT_ReadUsingCharUUID(connHandle, pReq, appTaskID);
        }
      }
      break;

    case ATT_READ_BY_TYPE_RSP:
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParseReadByTypeRsp(pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          stat = ATT_ReadByTypeRsp(connHandle, &msg.readByTypeRsp);
          if (stat == SUCCESS)
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_READ_REQ:
      if (ATT_ParseReadReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                             pCmd->len-2, &msg) == SUCCESS)
      {
        // Read Characteristic Value or Read Characteristic Descriptor
        stat = GATT_ReadCharValue(connHandle, &msg.readReq, appTaskID);
      }
      break;

    case ATT_READ_RSP:
      // Create payload if attribute value is present
      if (pCmd->len > 2)
      {
        pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      }

      if ((pCmd->len == 2) || (pPayload != NULL))
      {
        if (ATT_ParseReadRsp(pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          stat = ATT_ReadRsp(connHandle, &msg.readRsp);
          if  (stat == SUCCESS)
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_READ_BLOB_REQ:
      if (ATT_ParseReadBlobReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                 pCmd->len-2, &msg) == SUCCESS)
      {
        // Read long characteristic value
        stat = GATT_ReadLongCharValue(connHandle, &msg.readBlobReq, appTaskID);
      }
      break;

    case ATT_READ_BLOB_RSP:
      // Create payload if attribute value is present
      if (pCmd->len > 2)
      {
        pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      }

      if ((pCmd->len == 2) || (pPayload != NULL))
      {
        if (ATT_ParseReadBlobRsp(pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          stat = ATT_ReadBlobRsp(connHandle, &msg.readBlobRsp);
          if (stat == SUCCESS)
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_READ_MULTI_REQ:
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParseReadMultiReq(ATT_SIG_NOT_INCLUDED, FALSE, pPayload,
                                    pCmd->len-2, &msg) == SUCCESS)
        {
          stat = GATT_ReadMultiCharValues(connHandle, &msg.readMultiReq, appTaskID);
          if (stat == SUCCESS)
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_READ_MULTI_RSP:
      // Create payload if attribute value is present
      if (pCmd->len > 2)
      {
        pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      }

      if ((pCmd->len == 2) || (pPayload != NULL))
      {
        if (ATT_ParseReadMultiRsp(pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          stat = ATT_ReadMultiRsp(connHandle, &msg.readMultiRsp);
          if (stat == SUCCESS)
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_READ_BY_GRP_TYPE_REQ:
      if (ATT_ParseReadByTypeReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                   pCmd->len-2, &msg) == SUCCESS)
      {
        attReadByGrpTypeReq_t *pReq = &msg.readByGrpTypeReq;

        // Find out what's been requested
        if (gattPrimaryServiceType(pReq->type)     &&
             (pReq->startHandle == GATT_MIN_HANDLE) &&
             (pReq->endHandle   == GATT_MAX_HANDLE))
        {
          // Discover all primary services
          stat = GATT_DiscAllPrimaryServices(connHandle, appTaskID);
        }
        else
        {
          stat = INVALIDPARAMETER;
        }
      }
      break;

    case ATT_READ_BY_GRP_TYPE_RSP:
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParseReadByGrpTypeRsp(pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          stat = ATT_ReadByGrpTypeRsp(connHandle, &msg.readByGrpTypeRsp);
          if (stat == SUCCESS)
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_WRITE_REQ:
      pPayload = createSignedMsgPayload(pBuf[2], pBuf[3], &pBuf[4], pCmd->len-4);
      if (pPayload != NULL)
      {
        if (ATT_ParseWriteReq(pBuf[2], pBuf[3], pPayload,
                                pCmd->len-4, &msg) == SUCCESS)
        {
          attWriteReq_t *pReq = &msg.writeReq;

          if (pReq->cmd == FALSE)
          {
            // Write Characteristic Value or Write Characteristic Descriptor
            stat = GATT_WriteCharValue(connHandle, pReq, appTaskID);
          }
          else
          {
            if (pReq->sig == FALSE)
            {
              // Write Without Response
              stat = GATT_WriteNoRsp(connHandle, pReq);
            }
            else
            {
              // Signed Write Without Response
              stat = GATT_SignedWriteNoRsp(connHandle, pReq);
            }
          }

          if ((stat == SUCCESS) && (pReq->pValue != NULL))
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_WRITE_RSP:
      if (ATT_ParseWriteRsp(&pBuf[2], pCmd->len-2, &msg) == SUCCESS)
      {
        stat = ATT_WriteRsp(connHandle);
      }
      break;

    case ATT_PREPARE_WRITE_REQ:
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParsePrepareWriteReq(ATT_SIG_NOT_INCLUDED, FALSE, pPayload,
                                       pCmd->len-2, &msg) == SUCCESS)
        {
          attPrepareWriteReq_t *pReq = &msg.prepareWriteReq;

#if !defined(GATT_DB_OFF_CHIP) && defined(TESTMODES)
          if (GATTServApp_GetParamValue() == GATT_TESTMODE_PREPARE_WRITE)
          {
            // Send the Prepare Write Request right away - needed for GATT testing
            stat = GATT_PrepareWriteReq(connHandle, pReq, appTaskID);
            if ((stat == SUCCESS) && (pReq->pValue != NULL))
            {
              safeToDealloc = FALSE; // payload passed to GATT
            }
          }
          else
#endif // !GATT_DB_OFF_CHIP && TESTMODE
          {
            // GATT Reliable Writes
            if (pPrepareWrites == NULL)
            {
              // First allocated buffer for the Prepare Write Requests
              pPrepareWrites = ICall_malloc(GATT_MAX_NUM_RELIABLE_WRITES * sizeof(attPrepareWriteReq_t));
            }

            if (pPrepareWrites != NULL)
            {
              if (numPrepareWrites < GATT_MAX_NUM_RELIABLE_WRITES)
              {
                // Save the Prepare Write Request for now
                VOID memcpy(&(pPrepareWrites[numPrepareWrites++]),
                                  pReq, sizeof(attPrepareWriteReq_t));

                // Request was parsed and saved successfully
                stat = SUCCESS;
                safeToDealloc = FALSE; // payload saved for later
              }
              else
              {
                stat = INVALIDPARAMETER;
              }
            }
            else
            {
              stat = bleMemAllocError;
            }
          }
        }
      }
      break;

    case ATT_PREPARE_WRITE_RSP:
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParsePrepareWriteRsp(pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          attPrepareWriteRsp_t *pRsp = &msg.prepareWriteRsp;

          stat = ATT_PrepareWriteRsp(connHandle, pRsp);
          if ((stat == SUCCESS) && (pRsp->pValue != NULL))
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_EXECUTE_WRITE_REQ:
      if (ATT_ParseExecuteWriteReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                     pCmd->len-2, &msg) == SUCCESS)
      {
#if !defined(GATT_DB_OFF_CHIP) && defined(TESTMODES)
        if (GATTServApp_GetParamValue() == GATT_TESTMODE_PREPARE_WRITE)
        {
          // Send the Execute Write Request right away - needed for GATT testing
          stat = GATT_ExecuteWriteReq(connHandle, &msg.executeWriteReq, appTaskID);
        }
        else
#endif // !GATT_DB_OFF_CHIP && TESTMODE
        if (pPrepareWrites != NULL)
        {
          // GATT Reliable Writes - send all saved Prepare Write Requests
          stat = GATT_ReliableWrites(connHandle, pPrepareWrites, numPrepareWrites,
                                      msg.executeWriteReq.flags, appTaskID);
          if (stat != SUCCESS)
          {
            uint8_t i;

            // Free saved payload buffers
            for (i = 0; i < numPrepareWrites; i++)
            {
              if (pPrepareWrites[i].pValue != NULL)
              {
                BM_free(pPrepareWrites[i].pValue);
              }
            }

            ICall_free(pPrepareWrites);
          }
          // else pPrepareWrites will be freed by GATT Client

          // Reset GATT Reliable Writes variables
          pPrepareWrites = NULL;
          numPrepareWrites = 0;
        }
        else
        {
          stat = INVALIDPARAMETER;
        }
      }
      break;

    case ATT_EXECUTE_WRITE_RSP:
      if (ATT_ParseExecuteWriteRsp(&pBuf[2], pCmd->len-2, &msg) == SUCCESS)
      {
        stat = ATT_ExecuteWriteRsp(connHandle);
      }
      break;

    case ATT_HANDLE_VALUE_NOTI:
      pPayload = createMsgPayload(&pBuf[3], pCmd->len-3);
      gattAttribute_t *pAttrNoti;
      uint16 cccHandle, value;
      gattCharCfg_t *notCharCfg;
      attHandleValueNoti_t *pNoti;

      if (pPayload != NULL)
      {
        if (ATT_ParseHandleValueInd(ATT_SIG_NOT_INCLUDED, FALSE, pPayload,
                                      pCmd->len-3, &msg) == SUCCESS)
        {
          pNoti = &msg.handleValueNoti;
          // only if handle is bigger than zero, look at the ATT table for permission
          if(pNoti->handle > 0)
          {
              cccHandle = GATT_INVALID_HANDLE;
              pAttrNoti =(gattAttribute_t *)GATT_FindHandle(pNoti->handle+1, &cccHandle); //get Handle from table
              notCharCfg = (gattCharCfg_t *)*(uint32_t*)(pAttrNoti->pValue); //get value from Table
              if (notCharCfg == NULL)
              {
                stat = INVALIDPARAMETER;
                break;
              }
              value = GATTServApp_ReadCharCfg(connHandle, notCharCfg);
          }
          // if ATT table value enable notifications or the handle is 0 send Notification
          if ((value & GATT_CLIENT_CFG_NOTIFY) || (pNoti->handle == 0))
          {
           stat = GATT_Notification(connHandle, pNoti, pBuf[2]);
           if ((stat == SUCCESS) && (pNoti->pValue != NULL))
           {
             safeToDealloc = FALSE; // payload passed to GATT
           }
          }
          else
          {
            stat = FAILURE;
          }
        }
      }
      break;

    case ATT_HANDLE_VALUE_IND:
      pPayload = createMsgPayload(&pBuf[3], pCmd->len-3);
      gattAttribute_t *pAttrInd;
      gattCharCfg_t *indCharCfg;
      if (pPayload != NULL)
      {
        if (ATT_ParseHandleValueInd(ATT_SIG_NOT_INCLUDED, FALSE, pPayload,
                                      pCmd->len-3, &msg) == SUCCESS)
        {
          attHandleValueInd_t *pInd = &msg.handleValueInd;
#ifndef BLE3_CMD
          uint16 value = 0;
          if(pInd->handle > 0)
          {
            uint16 cccHandle;
            cccHandle = GATT_INVALID_HANDLE;
            pAttrInd = (gattAttribute_t *)GATT_FindHandle(pInd->handle+1, &cccHandle);
            indCharCfg = (gattCharCfg_t *)*(uint32_t*)(pAttrInd->pValue);
            if(indCharCfg == NULL)
            {
              stat = INVALIDPARAMETER;
             break;
            }
            value = GATTServApp_ReadCharCfg(connHandle, indCharCfg);
          }
          // if ATT table value enable notifications or the handle is 0 send Indication
          if((value & GATT_CLIENT_CFG_INDICATE) || (pInd->handle == 0))
          {
#endif
            stat = GATT_Indication(connHandle, pInd, pBuf[2], appTaskID);
            if ((stat == SUCCESS) && (pInd->pValue != NULL))
            {
              safeToDealloc = FALSE; // payload passed to GATT
            }
          }
#ifndef BLE3_CMD
          else
          {
            stat = FAILURE;
          }
        }
#endif
      }
      break;

    case ATT_HANDLE_VALUE_CFM:
      if (ATT_ParseHandleValueCfm(&pBuf[2], pCmd->len-2, &msg) == SUCCESS)
      {
        stat = ATT_HandleValueCfm(connHandle);
      }
      break;

    default:
      stat = FAILURE;
      break;
  }

  if ((pPayload != NULL) && safeToDealloc)
  {
    // Free payload
    BM_free(pPayload);
  }

  return(mapATT2BLEStatus(stat));
}

/*********************************************************************
 * @fn      processExtMsgGATT
 *
 * @brief   Parse and process incoming HCI extension GATT messages.
 *
 * @param   cmdID - incoming HCI extension command ID.
 * @param   pCmd - incoming HCI extension message.
 * @param   pRspDataLen - response data length to be returned.
 *
 * @return  SUCCESS, INVALIDPARAMETER, FAILURE,
 *          bleInvalidPDU or bleMemAllocError
 */
static uint8_t processExtMsgGATT(uint8_t cmdID, hciExtCmd_t *pCmd, uint8_t *pRspDataLen)
{
#if defined(GATT_DB_OFF_CHIP)
  static uint16_t totalAttrs = 0;
  static gattService_t service = { 0, 0, NULL };
#endif // GATT_DB_OFF_CHIP
  uint8_t *pBuf, *pPayload = NULL, safeToDealloc = TRUE;
  uint16_t connHandle;
  attMsg_t msg;
  bStatus_t stat = bleInvalidPDU;

  // Make sure received buffer is at least 2-octet long
  if (pCmd->len < 2)
  {
    return(stat);
  }

  pBuf = pCmd->pData;
  connHandle = BUILD_UINT16(pBuf[0], pBuf[1]);

  switch(cmdID)
  {
    case ATT_EXCHANGE_MTU_REQ: // GATT Exchange MTU
      if (ATT_ParseExchangeMTUReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                    pCmd->len-2, &msg) == SUCCESS)
      {
        stat = GATT_ExchangeMTU(connHandle, &msg.exchangeMTUReq, appTaskID);
      }
      break;

    case ATT_READ_BY_GRP_TYPE_REQ: // GATT Discover All Primary Services
      stat = GATT_DiscAllPrimaryServices(connHandle, appTaskID);
      break;

    case ATT_FIND_BY_TYPE_VALUE_REQ: // GATT Discover Primary Service By UUID
      stat = GATT_DiscPrimaryServiceByUUID(connHandle, &pBuf[2],
                                            pCmd->len-2, appTaskID);
      break;

    case GATT_FIND_INCLUDED_SERVICES: // GATT Find Included Services
    case GATT_DISC_ALL_CHARS: // GATT Discover All Characteristics
      if ((pCmd->len-2) == ATT_READ_BY_TYPE_REQ_FIXED_SIZE)
      {
        // First requested handle number
        uint16_t startHandle = BUILD_UINT16(pBuf[2], pBuf[3]);

        // Last requested handle number
        uint16_t endHandle = BUILD_UINT16(pBuf[4], pBuf[5]);

        if (cmdID == GATT_FIND_INCLUDED_SERVICES)
        {
          stat = GATT_FindIncludedServices(connHandle, startHandle,
                                            endHandle, appTaskID);
        }
        else
        {
          stat = GATT_DiscAllChars(connHandle, startHandle,
                                    endHandle, appTaskID);
        }
      }
      break;

    case ATT_READ_BY_TYPE_REQ: // GATT Discover Characteristics by UUID
      if (ATT_ParseReadByTypeReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                   pCmd->len-2, &msg) == SUCCESS)
      {
        stat = GATT_DiscCharsByUUID(connHandle, &msg.readByTypeReq, appTaskID);
      }
      break;

    case ATT_FIND_INFO_REQ: // GATT Discover All Characteristic Descriptors
      if (ATT_ParseFindInfoReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                 pCmd->len-2, &msg) == SUCCESS)
      {
        stat = GATT_DiscAllCharDescs(connHandle, msg.findInfoReq.startHandle,
                                      msg.findInfoReq.endHandle, appTaskID);
      }
      break;

    case ATT_READ_REQ: // GATT Read Characteristic Value
      if (ATT_ParseReadReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                             pCmd->len-2, &msg) == SUCCESS)
      {
        stat = GATT_ReadCharValue(connHandle, &msg.readReq, appTaskID);
      }
      break;

    case GATT_READ_USING_CHAR_UUID: // GATT Read Using Characteristic UUID
      if (ATT_ParseReadByTypeReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                   pCmd->len-2, &msg) == SUCCESS)
      {
        stat = GATT_ReadUsingCharUUID(connHandle, &msg.readByTypeReq, appTaskID);
      }
      break;

    case ATT_READ_BLOB_REQ: // GATT Read Long Characteristic Value
      if (ATT_ParseReadBlobReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                 pCmd->len-2, &msg) == SUCCESS)
      {
        stat = GATT_ReadLongCharValue(connHandle, &msg.readBlobReq, appTaskID);
      }
      break;

    case ATT_READ_MULTI_REQ: // GATT Read Multiple Characteristic Values
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParseReadMultiReq(ATT_SIG_NOT_INCLUDED, FALSE, pPayload,
                                    pCmd->len-2, &msg) == SUCCESS)
        {
          stat = GATT_ReadMultiCharValues(connHandle, &msg.readMultiReq, appTaskID);
          if (stat == SUCCESS)
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case GATT_WRITE_NO_RSP: // GATT Write Without Response
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParseWriteReq(FALSE, TRUE, pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          attWriteReq_t *pReq = &msg.writeReq;

          stat = GATT_WriteNoRsp(connHandle, pReq);
          if ((stat == SUCCESS) && (pReq->pValue != NULL))
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case GATT_SIGNED_WRITE_NO_RSP: // GATT Signed Write Without Response
      pPayload = createSignedMsgPayload(TRUE, TRUE, &pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParseWriteReq(TRUE, TRUE, pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          attWriteReq_t *pReq = &msg.writeReq;

          stat = GATT_SignedWriteNoRsp(connHandle, pReq);
          if ((stat == SUCCESS) && (pReq->pValue != NULL))
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_WRITE_REQ: // GATT Write Characteristic Value
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParseWriteReq(FALSE, FALSE, pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          attWriteReq_t *pReq = &msg.writeReq;

          stat = GATT_WriteCharValue(connHandle, pReq, appTaskID);
          if ((stat == SUCCESS) && (pReq->pValue != NULL))
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_PREPARE_WRITE_REQ: // GATT Write Long Characteristic Value
    case GATT_WRITE_LONG_CHAR_DESC: // GATT Write Long Characteristic Descriptor
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParsePrepareWriteReq(ATT_SIG_NOT_INCLUDED, FALSE, pPayload,
                                       pCmd->len-2, &msg) == SUCCESS)
        {
          attPrepareWriteReq_t *pReq = &msg.prepareWriteReq;
          if (cmdID == ATT_PREPARE_WRITE_REQ)
          {
            stat = GATT_WriteLongCharValue(connHandle, pReq, appTaskID);
          }
          else
          {
            stat = GATT_WriteLongCharDesc(connHandle, pReq, appTaskID);
          }

          if ((stat == SUCCESS) && (pReq->pValue != NULL))
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case GATT_RELIABLE_WRITES: // GATT Reliable Writes
      if (pCmd->len-2 > 0)
      {
        uint8_t numReqs = pBuf[2];

        if ((numReqs > 0) && (numReqs <= GATT_MAX_NUM_RELIABLE_WRITES))
        {
          // First allocated buffer for the Prepare Write Requests
          attPrepareWriteReq_t *pReqs = ICall_malloc(numReqs * sizeof(attPrepareWriteReq_t));
          if (pReqs != NULL)
          {
            uint8_t i;

            VOID memset(pReqs, 0, numReqs * sizeof(attPrepareWriteReq_t));

            pBuf += 3; // pass connHandle and numReqs

            // Create payload buffer for each Prepare Write Request
            for (i = 0; i < numReqs; i++)
            {
              // length of request is length of attribute value plus fixed fields.
              // request format: length (1) + handle (2) + offset (2) + attribute value
              uint8_t reqLen = ATT_PREPARE_WRITE_REQ_FIXED_SIZE;

              if (*pBuf > 0)
              {
                reqLen += *pBuf++;
                pPayload = createMsgPayload(pBuf, reqLen);
                if (pPayload == NULL)
                {
                  stat = bleMemAllocError;
                  break;
                }

                VOID ATT_ParsePrepareWriteReq(ATT_SIG_NOT_INCLUDED, FALSE, pPayload,
                                               reqLen, (attMsg_t *)&(pReqs[i]));
              }
              else // no attribute value
              {
                pBuf++;
                VOID ATT_ParsePrepareWriteReq(ATT_SIG_NOT_INCLUDED, FALSE, pBuf,
                                               reqLen, (attMsg_t *)&(pReqs[i]));
              }

              // Next request
              pBuf += reqLen;
            }

            // See if all requests were parsed successfully
            if (i == numReqs)
            {
              // Send all saved Prepare Write Requests
              stat = GATT_ReliableWrites(connHandle, pReqs, numReqs,
                                          ATT_WRITE_PREPARED_VALUES, appTaskID);
            }

            if (stat != SUCCESS)
            {
              // Free payload buffers first
              for (i = 0; i < numReqs; i++)
              {
                if (pReqs[i].pValue != NULL)
                {
                  BM_free(pReqs[i].pValue);
                }
              }

              ICall_free(pReqs);
            }
            // else pReqs will be freed by GATT Client
          }
          else
          {
            stat = bleMemAllocError;
          }
        }
        else
        {
          stat = INVALIDPARAMETER;
        }
      }
      break;

    case GATT_READ_CHAR_DESC: // GATT Read Characteristic Descriptor
      if (ATT_ParseReadReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                             pCmd->len-2, &msg) == SUCCESS)
      {
        stat = GATT_ReadCharDesc(connHandle, &msg.readReq, appTaskID);
      }
      break;

    case GATT_READ_LONG_CHAR_DESC: // GATT Read Long Characteristic Descriptor
      if (ATT_ParseReadBlobReq(ATT_SIG_NOT_INCLUDED, FALSE, &pBuf[2],
                                 pCmd->len-2, &msg) == SUCCESS)
      {
        stat = GATT_ReadLongCharDesc(connHandle, &msg.readBlobReq, appTaskID);
      }
      break;

    case GATT_WRITE_CHAR_DESC: // GATT Write Characteristic Descriptor
      pPayload = createMsgPayload(&pBuf[2], pCmd->len-2);
      if (pPayload != NULL)
      {
        if (ATT_ParseWriteReq(FALSE, FALSE, pPayload, pCmd->len-2, &msg) == SUCCESS)
        {
          attWriteReq_t *pReq = &msg.writeReq;

          stat = GATT_WriteCharDesc(connHandle, pReq, appTaskID);
          if ((stat == SUCCESS) && (pReq->pValue != NULL))
          {
            safeToDealloc = FALSE; // payload passed to GATT
          }
        }
      }
      break;

    case ATT_HANDLE_VALUE_NOTI:
    case ATT_HANDLE_VALUE_IND:
      pPayload = createMsgPayload(&pBuf[3], pCmd->len-3);
      gattAttribute_t *pAttrNoti;
      uint16 cccHandle, value;
      attHandleValueInd_t *pNoti;
      if (pPayload != NULL)
      {
        if (ATT_ParseHandleValueInd(ATT_SIG_NOT_INCLUDED, FALSE, pPayload,
                                      pCmd->len-3, &msg) == SUCCESS)
        {
          pNoti = &msg.handleValueInd;
          // only if handle is bigger than zero, look at the ATT table for permission
          if(pNoti->handle > 0)
          {
            cccHandle = GATT_INVALID_HANDLE;
            pAttrNoti = (gattAttribute_t *)MAP_GATT_FindHandle(pNoti->handle+1, &cccHandle); //get Handle from table
            gattCharCfg_t *indCharCfg = (gattCharCfg_t *)*(uint32_t*)(pAttrNoti->pValue); //get value from Table
            if (indCharCfg == NULL)
            {
              stat = INVALIDPARAMETER;
              break;
            }
            value = MAP_GATTServApp_ReadCharCfg(connHandle, indCharCfg);
          }
          if (cmdID==ATT_HANDLE_VALUE_IND)
          {
            // send indication only if requested by the client or for handle 0
            if ((value & GATT_CLIENT_CFG_INDICATE) || (pNoti->handle == 0))
            {
              stat = GATT_Indication(connHandle, pNoti, pBuf[2], appTaskID);
              if ((stat == SUCCESS) && (pNoti->pValue != NULL))
              {
                safeToDealloc = FALSE; // payload passed to GATT
              }
            }
            else
            {
              stat = FAILURE;
            }
          }
          else
          {
            //send notification only if requested by the client or for handle 0
            if ((value & GATT_CLIENT_CFG_NOTIFY) || (pNoti->handle == 0))
            {
              stat = GATT_Notification(connHandle, (attHandleValueNoti_t *)pNoti, pBuf[2]);
              if ((stat == SUCCESS) && (pNoti->pValue != NULL))
              {
                safeToDealloc = FALSE; // payload passed to GATT
              }
            }
            else
            {
              stat = FAILURE;
            }
          }
        }
      }
      break;

#if defined(GATT_DB_OFF_CHIP) // These GATT commands don't include connHandle field
    case HCI_EXT_GATT_ADD_SERVICE:
      if (service.attrs == NULL)
      {
        // Service type must be 2 octets (Primary or Secondary)
        if (pCmd->len-3 == ATT_BT_UUID_SIZE)
        {
          uint16_t uuid = BUILD_UINT16(pBuf[0], pBuf[1]);
          uint16_t numAttrs = BUILD_UINT16(pBuf[2], pBuf[3]);

          if (((uuid == GATT_PRIMARY_SERVICE_UUID)     ||
                 (uuid == GATT_SECONDARY_SERVICE_UUID)) &&
               (numAttrs > 0))
          {
            uint8_t encKeySize = pBuf[4];

            if ((encKeySize >= GATT_MIN_ENCRYPT_KEY_SIZE) &&
                 (encKeySize <= GATT_MAX_ENCRYPT_KEY_SIZE))
            {
              // Allocate buffer for the attribute table
              service.attrs = ICall_malloc(numAttrs * sizeof(gattAttribute_t));
              if (service.attrs != NULL)
              {
                // Zero out all attribute fields
                VOID memset(service.attrs, 0, numAttrs * sizeof(gattAttribute_t));

                totalAttrs = numAttrs;
                service.encKeySize = encKeySize;

                // Set up service record
                stat = addAttrRec(&service, pBuf, ATT_BT_UUID_SIZE,
                                   GATT_PERMIT_READ, &totalAttrs, pRspDataLen);
              }
              else
              {
                stat = bleMemAllocError;
              }
            }
            else
            {
              stat = bleInvalidRange;
            }
          }
          else
          {
            stat = INVALIDPARAMETER;
          }
        }
      }
      else
      {
        stat = blePending;
      }
      break;

    case HCI_EXT_GATT_DEL_SERVICE:
      {
        uint16_t handle = BUILD_UINT16(pBuf[0], pBuf[1]);

        if (handle == 0x0000)
        {
          // Service is not registered with GATT yet
          freeAttrRecs(&service);

          totalAttrs = 0;
        }
        else
        {
          gattService_t serv;

          // Service is already registered with the GATT Server
          stat = GATT_DeregisterService(handle, &serv);
          if (stat == SUCCESS)
          {
            freeAttrRecs(&serv);
          }
        }

        stat = SUCCESS;
      }
      break;

    case HCI_EXT_GATT_ADD_ATTRIBUTE:
      if (service.attrs != NULL)
      {
        if ((pCmd->len-1 == ATT_UUID_SIZE) ||
             (pCmd->len-1 == ATT_BT_UUID_SIZE))
        {
          // Add attribute record to the service being added
          stat = addAttrRec(&service, pBuf, pCmd->len-1,
                             pBuf[pCmd->len-1], &totalAttrs, pRspDataLen);
        }
      }
      else // no corresponding service
      {
        stat = INVALIDPARAMETER;
      }
      break;
    case HCI_EXT_GATT_UPDATE_MTU:
      stat = GATT_UpdateMTU(BUILD_UINT16(pBuf[0], pBuf[1]),  // connHandle
                              BUILD_UINT16(pBuf[2], pBuf[3])); // mtuSize
      break;
#endif // GATT_DB_OFF_CHIP

    default:
      stat = FAILURE;
      break;
  }

  if ((pPayload != NULL) && safeToDealloc)
  {
    // Free payload
    BM_free(pPayload);
  }

  return(mapATT2BLEStatus(stat));
}
#endif /* (CENTRAL_CFG | PERIPHERAL_CFG) */

/*********************************************************************
 * @fn      processExtMsgGAP
 *
 * @brief   Parse and process incoming HCI extension GAP messages.
 *
 * @param   cmdID - incoming HCI extension command ID.
 * @param   pCmd - incoming HCI extension message.
 * @param   pRspDataLen - response data length to be returned.
 *
 * @return  SUCCESS, INVALIDPARAMETER, FAILURE,
 *          or bleMemAllocError
 */
static uint8_t processExtMsgGAP(uint8_t cmdID, hciExtCmd_t *pCmd, uint8_t *pRspDataLen)
{
  uint8_t *pBuf = pCmd->pData;
  bStatus_t stat = SUCCESS;

  switch(cmdID)
  {
    case HCI_EXT_GAP_DEVICE_INIT:
    {
#if BLE3_CMD
      uint8_t profileRole = pBuf[0];
      uint8_t IRK[KEYLEN] = {0};
      uint8_t SRK[KEYLEN] = {0};
	  uint8_t addr[B_ADDR_LEN] = {0}; // The device address type and address must be configures with GAP_configDeviceAddr

      // Register with bond manager after starting device - not to prevent using GapBondMgr with BLE3_CMD flag
      GAPBondMgr_Register((gapBondCBs_t *) &host_tl_bondCB);

      if( !osal_memcmp(&pBuf[2+KEYLEN], SRK, KEYLEN) )
      {
        // Copy and set the SRK
        osal_memcpy(SRK, &pBuf[2+KEYLEN], KEYLEN);
        GapConfig_SetParameter(GAP_CONFIG_PARAM_SRK, SRK);
      }

      if( !osal_memcmp(&pBuf[1], IRK, KEYLEN) )
      {
        // Copy and set the IRK
        osal_memcpy(IRK, &pBuf[2], KEYLEN);
        GapConfig_SetParameter(GAP_CONFIG_PARAM_IRK, IRK);

        // This mode is set to enable saving the IRK given
        stat = GAP_DeviceInit(profileRole, appTaskID, ADDRMODE_RP_WITH_PUBLIC_ID, addr);
      }
      else
      {
        // IRK was all 0's - the address mode can be changed with HCI_EXT_GAP_CONFIG_DEVICE_ADDR
        stat = GAP_DeviceInit(profileRole, appTaskID, ADDRMODE_PUBLIC, addr);
      }
      // Maximum number of adv reports
      maxNumReports  = pBuf[1];

      if( maxNumReports )
      {
        // Allocating the memory for device info
        deviceInfoArr = (deviceInfo_t *)osal_mem_alloc(maxNumReports*sizeof(deviceInfo_t));
        osal_memset(deviceInfoArr, 0, maxNumReports*sizeof(deviceInfo_t));
      }

      // ignoring pSignCounter

#else // !BLE3_CMD
      uint8_t zeroAddr[B_ADDR_LEN] = {0};

      // Register with bond manager after starting device.
      GAPBondMgr_Register((gapBondCBs_t *) &host_tl_bondCB);

      // if the specified random addr is all zeros, replace it with NULL
      stat = GAP_DeviceInit(pBuf[0], appTaskID, pBuf[1],
                            memcmp(&pBuf[2], zeroAddr, B_ADDR_LEN) ?
                              &pBuf[2] : NULL);
#endif // BLE3_CMD
      break;
    }

    case HCI_EXT_GAP_CONFIG_SET_PARAM:
    {
      stat = GapConfig_SetParameter(pBuf[0],            // param
                                    &pBuf[1]);         // pValue
      break;
    }

#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
    case HCI_EXT_GAP_TERMINATE_LINK:
    {
#ifndef BLE3_CMD
      stat = GAP_TerminateLinkReq(BUILD_UINT16(pBuf[0], pBuf[1]), pBuf[2]);
#else //BLE3_CMD
      uint16_t connHandle = BUILD_UINT16(pBuf[0], pBuf[1]);
      if( connHandle != 0xFFFE )
      {
        stat = GAP_TerminateLinkReq(BUILD_UINT16(pBuf[0], pBuf[1]), pBuf[2]);
      }
      else
      {
#if (HOST_CONFIG & (CENTRAL_CFG))
        stat = GapInit_cancelConnect();
        legacyConnCancel = 1;
#else
        stat = FAILURE;
#endif
      }
#endif
      break;
    }

    case HCI_EXT_GAP_UPDATE_LINK_PARAM_REQ:
    {
      gapUpdateLinkParamReq_t updateLinkReq;

      updateLinkReq.connectionHandle = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      updateLinkReq.intervalMin = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      updateLinkReq.intervalMax = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      updateLinkReq.connLatency = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      updateLinkReq.connTimeout = BUILD_UINT16(pBuf[0], pBuf[1]);

      stat = GAP_UpdateLinkParamReq(&updateLinkReq);
      break;
    }

    case HCI_EXT_GAP_UPDATE_LINK_PARAM_REQ_REPLY:
    {
      gapUpdateLinkParamReqReply_t updateLinkReply;

      updateLinkReply.connectionHandle = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      updateLinkReply.intervalMin = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      updateLinkReply.intervalMax = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      updateLinkReply.connLatency = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      updateLinkReply.connTimeout = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      updateLinkReply.signalIdentifier = *pBuf++;
      updateLinkReply.accepted    = *pBuf;

      stat = GAP_UpdateLinkParamReqReply(&updateLinkReply);
      break;
    }

    case HCI_EXT_GAP_REGISTER_CONN_EVT:
    {
      stat = Gap_RegisterConnEventCb(host_tl_connEvtCallback,
                                     pBuf[0],                         // action
                                     pBuf[3],                         // event type
                                     BUILD_UINT16(pBuf[1], pBuf[2])); // connHandle
      break;
    }

    case HCI_EXT_GAP_AUTHENTICATE:
    {
      uint8_t tmp;
      gapAuthParams_t params;
      gapPairingReq_t pairReq;
      gapPairingReq_t *pPairReq = NULL;

      VOID memset(&params, 0, sizeof (gapAuthParams_t));

      params.connectionHandle = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;

      params.secReqs.ioCaps = *pBuf++;
      params.secReqs.oobAvailable = *pBuf++;
      VOID memcpy(params.secReqs.oob, pBuf, KEYLEN);
      pBuf += KEYLEN;

      VOID memcpy(params.secReqs.oobConfirm, pBuf, KEYLEN);
      pBuf += KEYLEN;
      params.secReqs.localOobAvailable = *pBuf++;
      VOID memcpy(params.secReqs.localOob, pBuf, KEYLEN);
      pBuf += KEYLEN;
      params.secReqs.isSCOnlyMode = *pBuf++;
      params.secReqs.eccKeys.isUsed = *pBuf++;
      VOID memcpy(params.secReqs.eccKeys.sK, pBuf, SM_ECC_KEY_LEN);
      pBuf += SM_ECC_KEY_LEN;
      VOID memcpy(params.secReqs.eccKeys.pK_x, pBuf, SM_ECC_KEY_LEN);
      pBuf += SM_ECC_KEY_LEN;
      VOID memcpy(params.secReqs.eccKeys.pK_y, pBuf, SM_ECC_KEY_LEN);
      pBuf += SM_ECC_KEY_LEN;

      params.secReqs.authReq = MAP_smResetAuthReqReservedBits(*pBuf++);
      params.secReqs.maxEncKeySize = *pBuf++;

      tmp = *pBuf++;
      params.secReqs.keyDist.pEncKey = (tmp & KEYDIST_SENC) ? TRUE : FALSE;
      params.secReqs.keyDist.pIdKey = (tmp & KEYDIST_SID) ? TRUE : FALSE;
      params.secReqs.keyDist.pSign = (tmp & KEYDIST_SSIGN) ? TRUE : FALSE;
      params.secReqs.keyDist.pLinkKey = (tmp & KEYDIST_SLINK) ? TRUE : FALSE;
      params.secReqs.keyDist.pReserved = 0;
      params.secReqs.keyDist.cEncKey = (tmp & KEYDIST_MENC) ? TRUE : FALSE;
      params.secReqs.keyDist.cIdKey = (tmp & KEYDIST_MID) ? TRUE : FALSE;
      params.secReqs.keyDist.cSign = (tmp & KEYDIST_MSIGN) ? TRUE : FALSE;
      params.secReqs.keyDist.cLinkKey = (tmp & KEYDIST_MLINK) ? TRUE : FALSE;
      params.secReqs.keyDist.cReserved = 0;

      tmp = *pBuf++; // enable disable
      if (tmp)
      {
        pairReq.ioCap = *pBuf++;
        pairReq.oobDataFlag = *pBuf++;
        pairReq.authReq = MAP_smResetAuthReqReservedBits(*pBuf++);
        pairReq.maxEncKeySize = *pBuf++;
        tmp = *pBuf++;
        pairReq.keyDist.pEncKey = (tmp & KEYDIST_SENC) ? TRUE : FALSE;
        pairReq.keyDist.pIdKey = (tmp & KEYDIST_SID) ? TRUE : FALSE;
        pairReq.keyDist.pSign = (tmp & KEYDIST_SSIGN) ? TRUE : FALSE;
        pairReq.keyDist.pLinkKey = (tmp & KEYDIST_SLINK) ? TRUE : FALSE;
        pairReq.keyDist.pReserved = 0;
        pairReq.keyDist.cEncKey = (tmp & KEYDIST_MENC) ? TRUE : FALSE;
        pairReq.keyDist.cIdKey = (tmp & KEYDIST_MID) ? TRUE : FALSE;
        pairReq.keyDist.cSign = (tmp & KEYDIST_MSIGN) ? TRUE : FALSE;
        pairReq.keyDist.cLinkKey = (tmp & KEYDIST_MLINK) ? TRUE : FALSE;
        pairReq.keyDist.cReserved = 0;
        pPairReq = &pairReq;
      }

      stat = GAP_Authenticate(&params, pPairReq);

      break;
    }

    case HCI_EXT_GAP_TERMINATE_AUTH:
      stat = GAP_TerminateAuth(BUILD_UINT16(pBuf[0], pBuf[1]), pBuf[2]);
      break;

    case HCI_EXT_GAP_BOND:
    {
      uint16_t connectionHandle;
      smSecurityInfo_t securityInfo;
      uint8_t authenticated;
      uint8_t secureConnections;

      // Do Security Information part
      connectionHandle = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      authenticated = *pBuf++;
      secureConnections = *pBuf++;
      VOID memcpy(securityInfo.ltk, pBuf, KEYLEN);
      pBuf += KEYLEN;
      securityInfo.div = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;
      VOID memcpy(securityInfo.rand, pBuf, B_RANDOM_NUM_SIZE);
      pBuf += B_RANDOM_NUM_SIZE;
      securityInfo.keySize = *pBuf++;

      stat = GAP_Bond(connectionHandle, authenticated, secureConnections,
                       &securityInfo, TRUE);
      break;
    }

    case HCI_EXT_GAP_SIGNABLE:
    {
      uint16_t connectionHandle;
      uint8_t authenticated;
      smSigningInfo_t signing;

      connectionHandle = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += 2;

      authenticated = *pBuf++;

      VOID memcpy(signing.srk, pBuf, KEYLEN);
      pBuf += KEYLEN;

      signing.signCounter = BUILD_UINT32(pBuf[0], pBuf[1], pBuf[2], pBuf[3]);

      stat = GAP_Signable(connectionHandle, authenticated, &signing);
      break;
    }

    case HCI_EXT_GAP_PASSKEY_UPDATE:
      stat = GAP_PasskeyUpdate(&pBuf[2], BUILD_UINT16(pBuf[0], pBuf[1]));
      break;

#endif // CENTRAL_CFG | PERIPHERAL_CFG

    case HCI_EXT_GAP_SET_PARAM:
    {
      uint16_t id = (uint16_t)pBuf[0];
      uint16_t value = BUILD_UINT16(pBuf[1], pBuf[2]);

#ifdef BLE3_CMD
      if( (id != TGAP_AUTH_TASK_ID) && (id < TGAP_PARAMID_MAX) )
      {
        stat = BLE3ToAgama_setParam(id, value);
      }
      else
      {
        stat = INVALIDPARAMETER;
      }
#else // !BLE3_CMD
#if !defined(GATT_DB_OFF_CHIP) && defined(TESTMODES)
      if (id == GAP_PARAM_GATT_TESTCODE)
      {
        GATTServApp_SetParamValue(value);
      }
      else if (id == GAP_PARAM_ATT_TESTCODE)
      {
        ATT_SetParamValue(value);
      }
      else if (id == GAP_PARAM_L2CAP_TESTCODE)
      {
        L2CAP_SetParamValue(value);
      }
      else
#endif // !GATT_DB_OFF_CHIP && TESTMODES
      if (id == GAP_PARAM_GGS_PARAMS)
      {
        GGS_SetParamValue(value);
      }
      else if ((id != GAP_PARAM_AUTH_TASK_ID) && (id < GAP_PARAMID_MAX))
      {
        stat = GAP_SetParamValue(id, value);
      }
      else
      {
        stat = INVALIDPARAMETER;
      }
#endif // BLE3_CMD
      break;
    }

    case HCI_EXT_GAP_GET_PARAM:
    {
      uint16_t paramValue = 0xFFFF;
      uint16_t param = (uint16_t)pBuf[0];

      if (param < 0x00FF)
      {
#if !defined(GATT_DB_OFF_CHIP) && defined(TESTMODES)
        if (param == GAP_PARAM_GATT_TESTCODE)
        {
          paramValue = GATTServApp_GetParamValue();
        }
        else if (param == GAP_PARAM_ATT_TESTCODE)
        {
          paramValue = ATT_GetParamValue();
        }
        else if (param == GAP_PARAM_GGS_PARAMS)
        {
          paramValue = GGS_GetParamValue();
        }
        else if (param == GAP_PARAM_L2CAP_TESTCODE)
        {
          paramValue = L2CAP_GetParamValue();
        }
        else
#endif // !GATT_DB_OFF_CHIP && TESTMODES
        if ((param != GAP_PARAM_AUTH_TASK_ID) && (param < GAP_PARAMID_MAX))
        {
          paramValue = GAP_GetParamValue(param);
        }
      }
      if (paramValue != 0xFFFF)
      {
        stat = SUCCESS;
      }
      else
      {
        stat = INVALIDPARAMETER;
      }
      *pRspDataLen = 2;
      rspBuf[RSP_PAYLOAD_IDX] = LO_UINT16(paramValue);
      rspBuf[RSP_PAYLOAD_IDX+1] = HI_UINT16(paramValue);

      break;
    }

#if (HOST_CONFIG & PERIPHERAL_CFG)
    case HCI_EXT_GAP_PERIPHERAL_SECURITY_REQ_UPDATE:
      stat = GAP_SendPeripheralSecurityRequest(BUILD_UINT16(pBuf[0], pBuf[1]), pBuf[2]);
      break;
#endif // PERIPHERAL_CFG

    case HCI_EXT_GAP_BOND_SET_PARAM:
    {
      stat = OPT_HCI_TL_EXT_gapBondSetParam( pBuf );
      break;
    }

    case HCI_EXT_GAP_BOND_GET_PARAM:
    {
      uint8_t len = 0;
      uint16_t id = BUILD_UINT16(pBuf[0], pBuf[1]);

      switch (id)
      {
        case GAPBOND_OOB_DATA:
          stat = GAPBondMgr_GetParameter(id, &rspBuf[RSP_PAYLOAD_IDX+2]);
          len = KEYLEN;
          break;

        case GAPBOND_ECC_KEYS:
          stat = GAPBondMgr_GetParameter(id, &rspBuf[RSP_PAYLOAD_IDX+2]);
          len = sizeof(gapBondEccKeys_t);
          break;

        case GAPBOND_DEFAULT_PASSCODE:
          stat =  SUCCESS;
          len = sizeof(host_tl_defaultPasscode);
          memcpy(&rspBuf[RSP_PAYLOAD_IDX+2], &host_tl_defaultPasscode, len);
          break;

        default:
          stat = GAPBondMgr_GetParameter(id, &rspBuf[RSP_PAYLOAD_IDX+2]);
          len = 1;
          break;
      }
      if (stat == SUCCESS)
      {
        rspBuf[RSP_PAYLOAD_IDX] = pBuf[0];  // paramID byte 0
        rspBuf[RSP_PAYLOAD_IDX+1] = pBuf[1];  // paramID byte 1
        *pRspDataLen = len + 2;
      }
      else
      {
        *pRspDataLen = 0;
      }
      break;
    }

#if !defined(GATT_NO_SERVICE_CHANGED)
    case HCI_EXT_GAP_BOND_SERVICE_CHANGE:
      stat = GAPBondMgr_ServiceChangeInd(BUILD_UINT16(pBuf[0], pBuf[1]), pBuf[2]);
      break;
#endif // !GATT_NO_SERVICE_CHANGED

    case HCI_EXT_GAP_BOND_GET_OOB_PARAMS:
    {
      stat = GAPBondMgr_SCGetLocalOOBParameters((gapBondOOBData_t *)&rspBuf[RSP_PAYLOAD_IDX]);
      if (stat == SUCCESS)
      {
        *pRspDataLen = sizeof(gapBondOOBData_t);
      }
      else
      {
        *pRspDataLen = 0;
      }
      break;
    }

    case HCI_EXT_GAP_BOND_SET_OOB_PARAMS:
    {
      stat = GAPBondMgr_SCSetRemoteOOBParameters((gapBondOOBData_t *)&pBuf[0], pBuf[2*KEYLEN]);
      break;
    }

    case HCI_EXT_GAP_BOND_PAIR:
    {
      stat = GAPBondMgr_Pair(BUILD_UINT16(pBuf[0], pBuf[1]));
    }
    break;

    case HCI_EXT_GAP_BOND_FIND_ADDR:
    {
      stat = GAPBondMgr_FindAddr(&pBuf[0],                    // input addr
                                 pBuf[B_ADDR_LEN],            // addr Type
                                 &rspBuf[RSP_PAYLOAD_IDX],    // bond index
                                 &rspBuf[RSP_PAYLOAD_IDX+1],  // id type
                                 &rspBuf[RSP_PAYLOAD_IDX+2]); // id addr

      // If a bond was found
      if (stat == SUCCESS)
      {
        *pRspDataLen = B_ADDR_LEN + 2;
      }
      else
      {
        *pRspDataLen = 0;
      }
    }
    break;

    case HCI_EXT_GAP_BOND_PASSCODE_RSP:
    {
      stat = GAPBondMgr_PasscodeRsp(BUILD_UINT16(pBuf[0], pBuf[1]),  // conn handle
                                    pBuf[2],                         // status
                                    BUILD_UINT32(pBuf[3], pBuf[4],
                                                 pBuf[5], pBuf[6])); // passcode
    }
    break;

    case HCI_EXT_GAP_SET_PRIVACY_MODE:
      stat = GAP_SetPrivacyMode((GAP_Peer_Addr_Types_t) pBuf[0],  // addrType
                                                       &pBuf[1],  // address
                                                        pBuf[7]); // mode
      break;
#if ( HOST_CONFIG & ( CENTRAL_CFG | PERIPHERAL_CFG ))
    case HCI_EXT_SM_REGISTER_TASK:
      SM_RegisterTask(appTaskID);
      break;

    case HCI_EXT_SM_GET_ECCKEYS:
      stat = SM_GetEccKeys();
      break;

    case HCI_EXT_SM_GET_DHKEY:
      // each field is 32 bytes
      stat = SM_GetDHKey(pBuf, &pBuf[32], &pBuf[64]);
      break;

    case HCI_EXT_SM_GET_CONFIRM_OOB:
      stat = SM_GetScConfirmOob(pBuf,
                                &pBuf[32],
                                &rspBuf[RSP_PAYLOAD_IDX]);

      *pRspDataLen = 16;
      break;
#endif /* #if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG )) */

#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )
    case HCI_EXT_GAP_ADV_CREATE:
    {
      // Temporary memory for adv params
      GapAdv_params_t advParams;
      advParams.eventProps = BUILD_UINT16(pBuf[0], pBuf[1]);
      advParams.primIntMin = BUILD_UINT32(pBuf[2], pBuf[3], pBuf[4], 0x00);
      advParams.primIntMax = BUILD_UINT32(pBuf[5], pBuf[6], pBuf[7], 0x00);
      advParams.primChanMap = (GapAdv_advChannels_t) pBuf[8];
      advParams.peerAddrType = (GAP_Peer_Addr_Types_t) pBuf[9];
      advParams.peerAddr[0] = pBuf[10];
      advParams.peerAddr[1] = pBuf[11];
      advParams.peerAddr[2] = pBuf[12];
      advParams.peerAddr[3] = pBuf[13];
      advParams.peerAddr[4] = pBuf[14];
      advParams.peerAddr[5] = pBuf[15];
      advParams.filterPolicy = (GapAdv_filterPolicy_t) pBuf[16];
      advParams.txPower = (int8_t) pBuf[17];
      advParams.primPhy = (GapAdv_primaryPHY_t) pBuf[18];
      advParams.secPhy = (GapAdv_secondaryPHY_t) pBuf[19];
      advParams.sid = pBuf[20];

      stat = GapAdv_create(host_tl_advEvtCallback, &advParams,
                           &rspBuf[RSP_PAYLOAD_IDX]);
      *pRspDataLen = 1;
      break;
    }
    case HCI_EXT_GAP_ADV_ENABLE:
    {
      stat = GapAdv_enable(pBuf[0],                         // handle
                           pBuf[1],                         // enable options
                           BUILD_UINT16(pBuf[2], pBuf[3])); // durationOrMaxEvents
      break;
    }
    case HCI_EXT_GAP_ADV_DISABLE:
    {
      stat = GapAdv_disable(pBuf[0]);
      break;
    }
    case HCI_EXT_GAP_ADV_DESTROY:
    {
      stat = GapAdv_destroy(pBuf[0], GAP_ADV_FREE_OPTION_ADV_DATA|GAP_ADV_FREE_OPTION_SCAN_RESP_DATA);
      break;
    }
    case HCI_EXT_GAP_ADV_SET_PARAM:
    {
      stat = GapAdv_setParam(pBuf[0],   // handle
                             pBuf[1],   // ParamID
                             &pBuf[2]); // pData
      break;
    }
    case HCI_EXT_GAP_ADV_GET_PARAM:
    {
      stat = GapAdv_getParam(pBuf[0],                    // handle
                             pBuf[1],                    // paramId
                             &rspBuf[RSP_PAYLOAD_IDX+1], // pValue
                             pRspDataLen);               // pLen

      if(stat == SUCCESS)
      {
        rspBuf[RSP_PAYLOAD_IDX] = pBuf[1];  // paramID
        (*pRspDataLen)++;                   // Add 1 for paramId
      }
      else
      {
        *pRspDataLen = 0;
      }

      break;
    }
    case HCI_EXT_GAP_ADV_LOAD_DATA:
    {
      uint8_t  handle = pBuf[0];
      // Add 1 to map data type to GapAdv_freeBufferOptions_t
      GapAdv_freeBufferOptions_t freeOption = (GapAdv_freeBufferOptions_t) (pBuf[1] + 1);
      uint16_t length = BUILD_UINT16(pBuf[2], pBuf[3]);

      if (length == 0)
      {
        // No Adv Data to load
        stat = SUCCESS;
      }
      else
      {

        // Pause advertising and free old buffer if it exists
        GapAdv_prepareLoadByHandle(handle, freeOption);

        // Allocate memory for the new buffers
        uint8_t *pData = ICall_mallocLimited(length);

        // If successfully allocated
        if(pData)
        {
          // Copy data from transport layer to new buffer
          memcpy(pData, &pBuf[4], length);

          // Load new buffer and restart advertising if it was paused
          stat = GapAdv_loadByHandle(handle, pBuf[1], length, pData);
        }
        else
        {
          stat = bleMemAllocError;
        }
      }
    }
    break;

   case HCI_EXT_GAP_ADV_SET_EVENT_MASK:
   {
     stat = GapAdv_setEventMask(pBuf[0],                             // handle
                               (GapAdv_eventMaskFlags_t)BUILD_UINT16(pBuf[1], pBuf[2])); // mask
     break;
   }
   case HCI_EXT_GAP_ADV_SET_VIRTUAL_ADV_ADDR:
   {
     stat = GapAdv_setVirtualAdvAddr(pBuf[0],   // advHandle
                                     &pBuf[1]); // bdAddr
     break;
   }

   case HCI_EXT_GAP_ADV_SET_PERIODIC_ADV_PARAMS:
   {
     GapAdv_periodicAdvParams_t pPriodicParams;

     pPriodicParams.periodicAdvIntervalMin = BUILD_UINT16(pBuf[1],pBuf[2]);
     pPriodicParams.periodicAdvIntervalMax = BUILD_UINT16(pBuf[3],pBuf[4]);
     pPriodicParams.periodicAdvProp = BUILD_UINT16(pBuf[5],pBuf[6]);
     stat = GapAdv_SetPeriodicAdvParams(pBuf[0], &pPriodicParams);
     break;
   }

   case HCI_EXT_GAP_ADV_SET_PERIODIC_ADV_DATA:
   {
     GapAdv_periodicAdvData_t pPeriodicData;

     pPeriodicData.operation = pBuf[1];
     pPeriodicData.dataLength = pBuf[2];

     if ( (pPeriodicData.dataLength > 0) )
     {
       // Copy the data
       pPeriodicData.pData = MAP_osal_mem_alloc(pPeriodicData.dataLength);
       if ( pPeriodicData.pData != NULL )
       {
         MAP_osal_memcpy(pPeriodicData.pData, &pBuf[3], pPeriodicData.dataLength);
         stat = GapAdv_SetPeriodicAdvData( pBuf[0], &pPeriodicData);

         // Free the allocated memory
         ICall_free(pPeriodicData.pData);
       }
     }
     else
     {
       stat = GapAdv_SetPeriodicAdvData( pBuf[0], &pPeriodicData);
     }
     break;
   }

   case HCI_EXT_GAP_ADV_SET_PERIODIC_ADV_ENABLE:
   {
     stat = GapAdv_SetPeriodicAdvEnable(pBuf[0], pBuf[1]);
     break;
   }
#endif // (PERIPHERAL_CFG | BROADCASTER_CFG )

#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
   case HCI_EXT_GAP_SCAN_ENABLE:
   {
     if (!host_tl_gapScannerInitialized)
     {
       stat = GapScan_registerCb(host_tl_scanEvtCallback, 0);
       if (!stat)
       {
           host_tl_gapScannerInitialized = 1;
       }
     }
     stat = GapScan_enable(BUILD_UINT16(pBuf[0], pBuf[1]), // Period
                           BUILD_UINT16(pBuf[2], pBuf[3]), // duration
                           pBuf[4]);                       // record
     break;
   }
   case HCI_EXT_GAP_SCAN_GET_ADV_REPORT:
   {
     stat = GapScan_getAdvReport(pBuf[0],
                        (GapScan_Evt_AdvRpt_t *) &rspBuf[RSP_PAYLOAD_IDX]);

     *pRspDataLen = sizeof(GapScan_Evt_AdvRpt_t) - 4;
     break;
   }
   case HCI_EXT_GAP_SCAN_DISABLE:
   {
     stat = GapScan_disable();
     break;
   }
   case HCI_EXT_GAP_SCAN_SET_PHY_PARAMS:
   {
     stat = GapScan_setPhyParams(pBuf[0],                         // PHY
                                 pBuf[1],                         // type
                                 BUILD_UINT16(pBuf[2], pBuf[3]),  // interval
                                 BUILD_UINT16(pBuf[4], pBuf[5])); // window
     break;
   }
   case HCI_EXT_GAP_SCAN_GET_PHY_PARAMS:
   {
     stat = GapScan_getPhyParams(pBuf[0],                         // PHY
                                 &rspBuf[RSP_PAYLOAD_IDX],        // type
                                 &rspBuf[RSP_PAYLOAD_IDX+1],      // interval
                                 &rspBuf[RSP_PAYLOAD_IDX+3]);     // window
     *pRspDataLen = 5;
     break;
   }
   case HCI_EXT_GAP_SCAN_SET_PARAM:
   {
     stat = GapScan_setParam(pBuf[0], &pBuf[1]);
     break;
   }
   case HCI_EXT_GAP_SCAN_GET_PARAM:
   {
     stat = GapScan_getParam(pBuf[0],                    // paramId
                             &rspBuf[RSP_PAYLOAD_IDX+1], // pValue
                             pRspDataLen);               // pLen
     if (stat == SUCCESS)
     {
       rspBuf[RSP_PAYLOAD_IDX] = pBuf[0];  // paramID
        (*pRspDataLen)++;                   // Add 1 for paramId
     }
     else
     {
       *pRspDataLen = 0;
     }
     break;
   }
   case HCI_EXT_GAP_SCAN_SET_EVENT_MASK:
   {
     GapScan_setEventMask(BUILD_UINT32(pBuf[0], pBuf[1],
                                       pBuf[2], pBuf[3]));

     // GapScan_setEventMask is a void function
     stat = SUCCESS;
     break;
   }

   case HCI_EXT_GAP_SCAN_CREATE_SYNC:
   {
     GapScan_PeriodicAdvCreateSyncParams_t syncParams;

     syncParams.options = pBuf[1];
     syncParams.advAddrType = pBuf[2];
     MAP_osal_memcpy(syncParams.advAddress, &pBuf[3], B_ADDR_LEN);
     syncParams.skip = BUILD_UINT16(pBuf[9],pBuf[10]);
     syncParams.syncTimeout = BUILD_UINT16(pBuf[11],pBuf[12]);
     syncParams.syncCteType = pBuf[13];

     stat = GapScan_PeriodicAdvCreateSync(pBuf[0], &syncParams);
     break;
    }

     case HCI_EXT_GAP_SCAN_CREATE_SYNC_CANCEL:
     {
       stat = GapScan_PeriodicAdvCreateSyncCancel();
       break;
     }

     case HCI_EXT_GAP_SCAN_TERMINATE_SYNC:
     {
       uint16_t syncHandle = BUILD_UINT16(pBuf[0],pBuf[1]);
       stat = GapScan_PeriodicAdvTerminateSync(syncHandle);
       break;
     }

     case HCI_EXT_GAP_SCAN_RECEIVE_PERIODIC_ADV_REPORT:
     {
       uint16_t syncHandle = BUILD_UINT16(pBuf[0],pBuf[1]);
       stat = GapScan_SetPeriodicAdvReceiveEnable(syncHandle, pBuf[2]);
       break;
     }

     case HCI_EXT_GAP_SCAN_ADD_DEVICE_PERIODIC_ADV_LIST:
     {
       uint8_t advAddr[B_ADDR_LEN];
       MAP_osal_memcpy(advAddr, &pBuf[1], B_ADDR_LEN);
       stat = GapScan_AddDeviceToPeriodicAdvList(pBuf[0], advAddr, pBuf[7]);
       break;
     }

     case HCI_EXT_GAP_SCAN_REMOVE_DEVICE_PERIODIC_ADV_LIST:
     {
       uint8_t advAddr[B_ADDR_LEN];
       MAP_osal_memcpy(advAddr, &pBuf[1], B_ADDR_LEN);
       stat = GapScan_RemoveDeviceFromPeriodicAdvList(pBuf[0], advAddr, pBuf[7]);
       break;
     }

     case HCI_EXT_GAP_SCAN_READ_ERIODIC_ADV_LIST_SIZE:
     {
       stat = GapScan_ReadPeriodicAdvListSize();
       break;
     }

     case HCI_EXT_GAP_SCAN_CLEAR_PERIODIC_ADV_LIST:
     {
       stat = GapScan_ClearPeriodicAdvList();
       break;
     }
#endif // ( CENTRAL_CFG | OBSERVER_CFG )

#if ( HOST_CONFIG & CENTRAL_CFG )
 case HCI_EXT_GAP_INIT_SET_PHY_PARAM:
   {
     stat = GapInit_setPhyParam(pBuf[0],                         // PHY
                                pBuf[1],                         // paramID
                                BUILD_UINT16(pBuf[2], pBuf[3])); // value
     break;
   }
   case HCI_EXT_GAP_INIT_GET_PHY_PARAM:
   {
     uint16_t tempValue;

     // Use tempValue to make sure the passed pointer is aligned
     stat = GapInit_getPhyParam(pBuf[0],                         // PHY
                                pBuf[1],                         // paramID
                                &tempValue);                     // value

     // Save the value
     rspBuf[RSP_PAYLOAD_IDX+1] = LO_UINT16(tempValue);
     rspBuf[RSP_PAYLOAD_IDX+2] = HI_UINT16(tempValue);
     rspBuf[RSP_PAYLOAD_IDX] = pBuf[1];
     *pRspDataLen = 3;
     break;
   }
   case HCI_EXT_GAP_INIT_CONNECT:
   {
     stat = GapInit_connect(pBuf[0],                             // peerAddrType
                            &pBuf[1],                            // peerAddress
                            pBuf[7],                             // phys
                            BUILD_UINT16(pBuf[8], pBuf[9]));     // timeout
     break;
   }
   case HCI_EXT_GAP_INIT_CONNECT_AL:
   {
     stat = GapInit_connectAl(pBuf[0],                             // phys
                              BUILD_UINT16(pBuf[1], pBuf[2]));     // timeout
     break;
   }
   case HCI_EXT_GAP_INIT_CANCEL_CONNECT:
   {
     stat = GapInit_cancelConnect();
     break;
   }
#endif // CENTRAL_CFG


#ifdef BLE3_CMD
    case HCI_EXT_GAP_CONFIG_DEVICE_ADDR:
    {
      GAP_Addr_Modes_t addrType = (GAP_Addr_Modes_t)pBuf[0];
      uint8_t addr[B_ADDR_LEN] = {0};

      // When the device address is ADDRMODE_PUBLIC or ADDRMODE_RANDOM the IRK shall be all 0's
      if( (addrType == ADDRMODE_PUBLIC) ||
	   (addrType == ADDRMODE_RANDOM) )
      {
        uint8_t tempKey[KEYLEN] = {0};
        GapConfig_SetParameter(GAP_CONFIG_PARAM_IRK, tempKey);
      }
      // Set the BD address
      if ( addrType == ADDRMODE_PUBLIC )
      {
        osal_memcpy(addr, &pBuf[1], B_ADDR_LEN);
        HCI_EXT_SetBDADDRCmd(addr);
      }

      // if address type is random or public set irk to 0 if not 0 already
      // If own address type is random static or RPA with random static...
      if( addrType == ADDRMODE_RANDOM
         || addrType == ADDRMODE_RP_WITH_RANDOM_ID )
      {
        osal_memcpy(addr, &pBuf[1], B_ADDR_LEN);

        // Check if the random address is valid
        // If all bits excluding the 2 MSBs are all 0's...
        if(((MAP_osal_isbufset(addr, 0x00, B_ADDR_LEN - 1) &&
           ((addr[B_ADDR_LEN - 1] & 0x3F) == 0)) ||
            // Or all bites are 1's
            MAP_osal_isbufset(addr, 0xFF, B_ADDR_LEN)))
        {
          return INVALIDPARAMETER;
        }
        else if(!(GAP_IS_ADDR_RS(addr)))
        {
           // This is an invalid random static address
           GAP_MAKE_ADDR_RS(addr);
         }
         // If valid random static address, put it to the controller
         stat = HCI_LE_SetRandomAddressCmd(addr);
      }
      // Set the internal GAP address mode
      gapSetDevAddressMode(addrType);
      break;
    }

#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )
    case HCI_EXT_GAP_MAKE_DISCOVERABLE:
    {
      uint16_t eventProps = 0;
      GapAdv_advChannels_t chanMap = (GapAdv_advChannels_t)pBuf[8];
      GAP_Peer_Addr_Types_t addrType = (GAP_Peer_Addr_Types_t)pBuf[1];
      GapAdv_filterPolicy_t fltPolicy = (GapAdv_filterPolicy_t)pBuf[9];

      // Fix the advertising parameter
      eventProps = getBLE3ToAgamaEventProp(pBuf[0]);

      if( eventProps & GAP_ADV_PROP_CONNECTABLE )
      {
        // This is a connectable advertising
        // check if we haven't reached the maximum number of connections
        uint8_t numConns = linkDB_NumActive();

        if( numConns >= MAX_NUM_BLE_CONNS )
        {
          maxConnReached = 1;
          return SUCCESS;
        }
      }

      if( advHandleLegacy == 0xFF )
      {
        // Sanity check. In BLE3 GAP_UpdateAdvertisingData must be called first
        GapAdv_params_t advParamLegacy = GAPADV_PARAMS_LEGACY_SCANN_CONN;

        // Update the advertising parameters struct
        advParamLegacy.eventProps = eventProps;
        advParamLegacy.peerAddrType = addrType;
        advParamLegacy.primChanMap = chanMap;
        advParamLegacy.filterPolicy = fltPolicy;

        // Create Advertisement set and assign handle
        stat = GapAdv_create(&host_tl_advEvtCallback, &advParamLegacy,
                             &advHandleLegacy);
      }
      else
      {
        // Updating the advertising parameters
        stat = GapAdv_setParam(advHandleLegacy, GAP_ADV_PARAM_PROPS, &eventProps);
        if( stat != SUCCESS )
        {
          if( stat == bleNotReady )
          {
            // The advertising is already enabled
            return bleAlreadyInRequestedMode;
          }
          return stat;
        }

        stat = GapAdv_setParam(advHandleLegacy, GAP_ADV_PARAM_PEER_ADDRESS_TYPE, &addrType);
        if( stat != SUCCESS )
        {
          if( stat == bleNotReady )
          {
            return bleAlreadyInRequestedMode;
          }
          return stat;
        }

        uint8_t addr[B_ADDR_LEN];
        osal_memcpy(addr, &pBuf[2], B_ADDR_LEN);
        stat = GapAdv_setParam(advHandleLegacy, GAP_ADV_PARAM_PEER_ADDRESS, addr);
        if( stat != SUCCESS )
        {
          if( stat == bleNotReady )
          {
            return bleAlreadyInRequestedMode;
          }
          return stat;
        }

        stat = GapAdv_setParam(advHandleLegacy, GAP_ADV_PARAM_PRIMARY_CHANNEL_MAP, &chanMap);
        if( stat != SUCCESS )
        {
          if( stat == bleNotReady )
          {
            return bleAlreadyInRequestedMode;
          }
          return stat;
        }

        stat = GapAdv_setParam(advHandleLegacy, GAP_ADV_PARAM_FILTER_POLICY, &fltPolicy);
        if( stat != SUCCESS )
        {
          if( stat == bleNotReady )
          {
            return bleAlreadyInRequestedMode;
          }
          return stat;
        }
      }

      if( advNotice )
      {
        // Set event mask - Adv Notice Events enabled
        stat = GapAdv_setEventMask(advHandleLegacy, GAP_ADV_EVT_MASK_END );
      }
      else
      {
        stat = GapAdv_setEventMask(advHandleLegacy, 0 );
      }

      // To enable adv notice event HCI_EXT_AdvEventNotice should be called before each GAP_MakeDiscoverable
      advNotice = 0;

      if( stat != SUCCESS )
      {
        return stat;
      }

      // Enable this flag to mark that GapAdv_enable was called.
      makeDiscFlag = 1;

      if( advDuration == 0 )
      {
        // Infinite advertisement
        stat = GapAdv_enable(advHandleLegacy, GAP_ADV_ENABLE_OPTIONS_USE_MAX , 0);
      }
      else
      {
        // Time limited advertising
        stat = GapAdv_enable(advHandleLegacy, GAP_ADV_ENABLE_OPTIONS_USE_DURATION, advDuration);
      }
      if( (stat != SUCCESS) && (stat != bleInvalidRange) )
      {
        // With the above GapAdv_params we should not receive bleInvalidRange from the host
        // We should not expected to receive GAP_ADV_MAKE_DISCOVERABLE_DONE_EVENT
        makeDiscFlag = 0;
      }
      break;
    }

    case HCI_EXT_GAP_END_DISC:
    {
      // Enable this flag to mark that GapAdv_disable was called.
      endDiscFlag = 1;
      stat = GapAdv_disable(advHandleLegacy);
      if( (stat != SUCCESS)  && (stat != bleInternalError) )
      {
        // The error returned from the host. We should not expected to receive GAP_ADV_END_DISCOVERABLE_DONE_EVENT
        endDiscFlag = 0;
      }
      break;
    }

    case HCI_EXT_GAP_UPDATE_ADV_DATA:
    {
      uint8_t dataType = pBuf[0];
      uint8_t dataLen = pBuf[1];

      if( advHandleLegacy == 0xFF )
      {
        GapAdv_params_t advParamLegacy = GAPADV_PARAMS_LEGACY_SCANN_CONN;

        // Create Advertisement set and assign handle
        stat = GapAdv_create(&host_tl_advEvtCallback, &advParamLegacy,
                             &advHandleLegacy);
      }

      if( stat != SUCCESS )
      {
        return stat;
      }

      uint8_t *pData = ICall_malloc(dataLen);

      if( pData == NULL )
      {
        return bleMemAllocError;
      }

      osal_memcpy(pData, &pBuf[2], dataLen);

      if( dataType == 1 )
      {
        // Advertising Data
        // Pause advertising and free old buffer if it exists
        GapAdv_prepareLoadByHandle(advHandleLegacy, GAP_ADV_FREE_OPTION_ADV_DATA);
        stat = GapAdv_loadByHandle(advHandleLegacy, GAP_ADV_DATA_TYPE_ADV, dataLen, pData);
      }
      else
      {
        // Scan response Data
        // Pause advertising and free old buffer if it exists
        GapAdv_prepareLoadByHandle(advHandleLegacy, GAP_ADV_FREE_OPTION_SCAN_RESP_DATA);
        stat = GapAdv_loadByHandle(advHandleLegacy, GAP_ADV_DATA_TYPE_SCAN_RSP, dataLen, pData);
      }
      break;
    }
#endif // ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )

#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
    case HCI_EXT_GAP_DEVICE_DISC_REQ:
    {
      uint8_t discMode = pBuf[0];
      uint8_t activeScan = pBuf[1];
      uint8_t acceptList = pBuf[2];
      uint16_t scanWin;
      uint16_t scanInt;
      GapScan_ScanType_t scanType;

      if (!host_tl_gapScannerInitialized)
      {
        stat = GapScan_registerCb(host_tl_scanEvtCallback, 0);
        if (stat != SUCCESS)
        {
          stat = bleIncorrectMode;
          break;
        }
        host_tl_gapScannerInitialized = 1;
      }

      if( scanNotice )
      {
        // With scan notice events
        GapScan_setEventMask( GAP_EVT_SCAN_DISABLED |
                              GAP_EVT_SCAN_DUR_ENDED |
                              GAP_EVT_ADV_REPORT |
                              GAP_EVT_SCAN_INT_ENDED); // could be  changed to GAP_EVT_SCAN_WND_ENDED
      }
      else
      {
        GapScan_setEventMask( GAP_EVT_SCAN_DISABLED |
                              GAP_EVT_SCAN_DUR_ENDED |
                              GAP_EVT_ADV_REPORT );
      }

      // To enable scan notice event HCI_EXT_ScanEventNotice should be called before each GAP_DeviceDiscoveryRequest
      scanNotice = 0;

      // acceptlist filter options
      stat = GapScan_setParam(SCAN_PARAM_FLT_POLICY, &acceptList);
      if( stat == bleIncorrectMode )
      {
        stat = bleAlreadyInRequestedMode;
        break;
      }
      else if( (stat != SUCCESS) && (stat != bleIncorrectMode) )
      {
        stat = bleIncorrectMode;
        break;
      }

      // Discovery mode
      if( discMode == 0x03 )
      {
        discMode = SCAN_FLT_DISC_DISABLE;
      }
      stat = GapScan_setParam(SCAN_PARAM_FLT_DISC_MODE, &discMode);
      if( stat == bleIncorrectMode )
      {
        stat = bleAlreadyInRequestedMode;
        break;
      }
      else if( (stat != SUCCESS) && (stat != bleIncorrectMode) )
      {
        stat = bleIncorrectMode;
        break;
      }

      // Get the phy scanning values
      stat = GapScan_getPhyParams(SCAN_PRIM_PHY_1M, &scanType, &scanInt, &scanWin);
      if( stat != SUCCESS )
      {
        stat = bleIncorrectMode;
		break;
      }

      // Update scan type
      stat = GapScan_setPhyParams(SCAN_PRIM_PHY_1M,(GapScan_ScanType_t)activeScan, scanInt, scanWin);
      if( stat != SUCCESS )
      {
        stat = bleIncorrectMode;
        break;
      }

      stat = GapScan_enable(0, scanDuration, maxNumReports);
      if( stat == SUCCESS )
      {
        // Do nothing
      }
	  else
      {
        if( (stat != bleAlreadyInRequestedMode) && (stat != bleMemAllocError) )
        {
          stat = bleIncorrectMode;
          break;
        }
      }
      break;
    }

    case HCI_EXT_GAP_DEVICE_DISC_CANCEL:
    {
      GapScan_disable();
      break;
    }

    case HCI_EXT_GAP_EST_LINK_REQ:
    {
      // highDutyCycle - not relevant. The scan setting need to be change using GAP_SetParam with the relevant parameters ID
      uint8_t acceptList = pBuf[1];

      if( acceptList == 1 )
      {
        stat = GapInit_connectAl(INIT_PHY_1M, 0);
      }
      else if( acceptList == 0 )
      {
        GAP_Peer_Addr_Types_t addrType = (GAP_Peer_Addr_Types_t)pBuf[2];
        uint8_t addr[B_ADDR_LEN] = {0};
        // Copy the peer's addr
        osal_memcpy(addr, &pBuf[3], B_ADDR_LEN);

        stat = GapInit_connect(addrType, addr, INIT_PHY_1M, 0);
      }
	  else
	  {
	  stat = bleIncorrectMode;
	  }

      if( stat != SUCCESS)
      {
        sendEstEvt = 1;
        stat = SUCCESS;
      }
      break;
    }
#endif // ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )

#endif // BLE3_CMD

    default:
      stat = FAILURE;
      break;
  }

  return(stat);
}

/*********************************************************************
 * @brief Sets a GAP Bond Manager parameter.
 *
 * This function is used to set a specific parameter in the GAP Bond Manager.
 *
 * @param pBuf Pointer to the buffer containing the parameter ID and value to be set.
 *             The buffer format and content depend on the specific parameter being set.
 *
 * @return Status of the operation. Typically, a success or error code.
 */
uint8_t HCI_TL_EXT_gapBondSetParam(uint8_t *pBuf)
{
    uint8_t stat;
    uint16_t id = BUILD_UINT16(pBuf[0], pBuf[1]);
    uint8_t *pValue;

    if(id == GAPBOND_DEFAULT_PASSCODE)
    {
      // First build passcode
      if(pBuf[2] == 2)
        host_tl_defaultPasscode = (BUILD_UINT32(pBuf[3], pBuf[3 + 1], 0L, 0L));
      else if(pBuf[2] == 3)
        host_tl_defaultPasscode = (BUILD_UINT32(pBuf[3], pBuf[3 + 1], pBuf[3 + 2], 0L));
      else if(pBuf[2] == 4)
        host_tl_defaultPasscode = (BUILD_UINT32(pBuf[3], pBuf[3 + 1], pBuf[3 + 2], pBuf[3 + 3]));
      else
        host_tl_defaultPasscode = ((uint32)pBuf[3]);

      stat = SUCCESS;
    }
    else
    {
      pValue = &pBuf[3];
      stat = GAPBondMgr_SetParameter(id, pBuf[2],  pValue);
    }
    return ( stat );
}

#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
/*********************************************************************
 * @fn      createMsgPayload
 *
 * @brief   Create payload buffer for OTA message to be sent.
 *
 * @param   pBuf - pointer to message data
 * @param   len - length of message data
 *
 * @return  pointer to payload. NULL if alloc fails.
 */
static uint8_t *createMsgPayload(uint8_t *pBuf, uint16_t len)
{
  return(createPayload(pBuf, len, 0));
}
#endif /* (CENTRAL_CFG | PERIPHERAL_CFG) */

#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
/**
 * Receive connection events
 *
 * @param pReport connection event report
 */
static void host_tl_connEvtCallback(Gap_ConnEventRpt_t *pReport)
{
  if (HCI_TL_CallbackEvtProcessCB((void *) pReport,
                                  (void *) host_tl_connEvtCallbackProcess) ==
                                  FALSE)
  {
    // Allocation of event to switch to app context failed. Consider this event
    // lost so free the corresponding data.
    ICall_free(pReport);
  }
}

/**
 * Process connection event callback in app context
 *
 * @param pReport connection event report
 */
static void host_tl_connEvtCallbackProcess(Gap_ConnEventRpt_t *pReport)
{
  uint8_t index = 0;
  // Fill up event
#ifdef BLE3_CMD
  uint8_t data[16];
  data[index++] = LO_UINT16(HCI_EXT_GAP_BLE3_CONN_EVT_NOTICE);
  data[index++] = HI_UINT16(HCI_EXT_GAP_BLE3_CONN_EVT_NOTICE);
#else
  uint8_t data[24];
  data[index++] = LO_UINT16(HCI_EXT_GAP_CONN_EVT_NOTICE);
  data[index++] = HI_UINT16(HCI_EXT_GAP_CONN_EVT_NOTICE);
#endif
  data[index++] = pReport->status;
  data[index++] = LO_UINT16(pReport->handle);
  data[index++] = HI_UINT16(pReport->handle);
  data[index++] = pReport->channel;
#ifndef BLE3_CMD
  data[index++] = pReport->phy;
#endif
  data[index++] = pReport->lastRssi;
  data[index++] = LO_UINT16(pReport->packets);
  data[index++] = HI_UINT16(pReport->packets);
  data[index++] = LO_UINT16(pReport->errors);
  data[index++] = HI_UINT16(pReport->errors);
  data[index++] = pReport->nextTaskType;
  data[index++] = BREAK_UINT32(pReport->nextTaskTime, 0);
  data[index++] = BREAK_UINT32(pReport->nextTaskTime, 1);
  data[index++] = BREAK_UINT32(pReport->nextTaskTime, 2);
  data[index++] = BREAK_UINT32(pReport->nextTaskTime, 3);
#ifndef BLE3_CMD
  data[index++] = LO_UINT16(pReport->eventCounter);
  data[index++] = HI_UINT16(pReport->eventCounter);
  data[index++] = BREAK_UINT32(pReport->timeStamp, 0);
  data[index++] = BREAK_UINT32(pReport->timeStamp, 1);
  data[index++] = BREAK_UINT32(pReport->timeStamp, 2);
  data[index++] = BREAK_UINT32(pReport->timeStamp, 3);
  data[index++] = pReport->eventType;
#endif
  // Send Connection Event information over transport layer
  HCI_TL_SendVSEvent(data, sizeof(data));

  // Free connection report since it has been sent over TL
  ICall_free(pReport);
}
#endif /* (CENTRAL_CFG | PERIPHERAL_CFG) */

/*********************************************************************
 * @fn      host_tl_passcodeCB
 *
 * @brief   Passcode callback.
 *
 * @return  none
 */
static void host_tl_passcodeCB(uint8_t *deviceAddr, uint16_t connectionHandle,
                               uint8_t uiInputs, uint8_t uiOutputs,
                               uint32_t numComparison)
{
  // If numComparison is zero and uiInputs are TRUE,
  // Send_GAPBondMgr_SetParameter shall be called
  // by the script in response to GAP_PasskeyNeeded event
  if ((numComparison != 0 || !uiInputs) && HCI_TL_CallbackEvtProcessCB)
  {
    passcodeEvtCallback_t * passcodeEvtCallback = ICall_malloc(sizeof(passcodeEvtCallback_t));

    if (passcodeEvtCallback)
    {
        passcodeEvtCallback->connectionHandle = connectionHandle;

        HCI_TL_CallbackEvtProcessCB( (void*) passcodeEvtCallback, (void*) host_tl_passcodeCBProcess);
     }
  }
}

/*********************************************************************
 * @fn      host_tl_passcodeCBProcess
 *
 * @brief   Process passcode event (translation from the callback).
 *
 * @return  none
 */
static void host_tl_passcodeCBProcess(passcodeEvtCallback_t * pPasscodeEvt)
{
  // Send passcode response right away, without going through an event...
  GAPBondMgr_PasscodeRsp(pPasscodeEvt->connectionHandle, SUCCESS, host_tl_defaultPasscode);
}

#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
/*********************************************************************
 * @fn      createMsgPayload
 *
 * @brief   Create payload buffer for OTA message to be sent.
 *
 * @param   sig - whether to include authentication signature
 * @param   cmd - whether it's write command
 * @param   pBuf - pointer to message data
 * @param   len - length of message data
 *
 * @return  pointer to payload. NULL if alloc fails.
 */
static uint8_t *createSignedMsgPayload(uint8_t sig, uint8_t cmd, uint8_t *pBuf, uint16_t len)
{
  if (sig && cmd)
  {
    // Need to allocate space for Authentication Signature
    return(createPayload(pBuf, len, ATT_AUTHEN_SIG_LEN));
  }

  return(createPayload(pBuf, len, 0));
}

/*********************************************************************
 * @fn      createPayload
 *
 * @brief   Create payload buffer for OTA message plus authentication signature
 *          to be sent.
 *
 * @param   pBuf - pointer to message data
 * @param   len - length of message data
 * @param   sigLen - length of authentication signature
 *
 * @return  pointer to payload. NULL if alloc fails.
 */
static uint8_t *createPayload(uint8_t *pBuf, uint16_t len, uint8_t sigLen)
{
  // Create payload buffer for OTA message to be sent
  uint8_t *pPayload = (uint8_t *) L2CAP_bm_alloc(len + ATT_OPCODE_SIZE + sigLen);
  if (pPayload != NULL)
  {

    ICall_CSState key;

    /* Critical section is entered to disable interrupts that might cause call
     * to callback due to race condition */
    key = ICall_enterCriticalSection();
    pPayload = osal_bm_adjust_header(pPayload, -ATT_OPCODE_SIZE);
    ICall_leaveCriticalSection(key);

    // Copy received data over
    VOID memcpy(pPayload, pBuf, len);

    return(pPayload);
  }

  return(NULL);
}
#endif /* (CENTRAL_CFG | PERIPHERAL_CFG) */

// Do not use SNV drivers with FPGA. Unverified behavior
/*********************************************************************
 * @fn      checkNVLen
 *
 * @brief   Checks the size of NV items.
 *
 * @param   id - NV ID.
 * @param   len - lengths in bytes of item.
 *
 * @return  SUCCESS, INVALIDPARAMETER or FAILURE
 */
static uint8_t checkNVLen(osalSnvId_t id, osalSnvLen_t len)
{
  uint8_t stat = SUCCESS;

  switch (id)
  {
    case BLE_NVID_CSRK:
    case BLE_NVID_IRK:
      if (len != KEYLEN)
      {
        stat = INVALIDPARAMETER;
      }
      break;

    default:
      if ((id >= BLE_NVID_GAP_BOND_START) && (id <= BLE_NVID_GAP_BOND_END))
      {
        stat = OPT_gapBondMgr_CheckNVLen(id, len);
      }
      else
#if !defined(NO_OSAL_SNV)
      if ((id >= BLE_NVID_CUST_START) && (id <= BLE_NVID_CUST_END))
      {
        stat = SUCCESS;
      }
      else
#endif
      {
        stat = INVALIDPARAMETER;  // Initialize status to failure
      }
      break;
  }

  return(stat);
}

#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
/*********************************************************************
 * @fn      mapATT2BLEStatus
 *
 * @brief   Map ATT error code to BLE Generic status code.
 *
 * @param   status - ATT status
 *
 * @return  BLE Generic status
 */
static uint8_t mapATT2BLEStatus(uint8_t status)
{
  uint8_t stat;

  switch (status)
  {
    case ATT_ERR_INSUFFICIENT_AUTHEN:
      // Returned from Send routines
      stat = bleInsufficientAuthen;
      break;

    case ATT_ERR_INSUFFICIENT_ENCRYPT:
      // Returned from Send routines
      stat = bleInsufficientEncrypt;
      break;

    case ATT_ERR_INSUFFICIENT_KEY_SIZE:
      // Returned from Send routines
      stat = bleInsufficientKeySize;
      break;

    default:
      stat = status;
  }

  return(stat);
}
#endif /* (CENTRAL_CFG | PERIPHERAL_CFG) */

/*********************************************************************
 * @fn      processEvents
 *
 * @brief   Process an incoming Event messages.
 *
 * @param   pMsg - message to process
 *
 * @return  TRUE to deallocate, FALSE otherwise
 */
static uint8_t processEvents(ICall_Hdr *pMsg)
{
  uint16_t msgLen = 0;
  uint8_t *pBuf = NULL;
  uint8_t allocated = FALSE;
  uint8_t deallocateIncoming = TRUE;

  VOID memset(out_msg, 0, sizeof (out_msg));

  switch (pMsg->event)
  {
    case GAP_MSG_EVENT:
      pBuf = processEventsGAP((gapEventHdr_t *)pMsg, out_msg, (uint16_t *)&msgLen, &allocated, &deallocateIncoming);
      break;

#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
    case SM_MSG_EVENT:
      pBuf = processEventsSM((smEventHdr_t *)pMsg, out_msg, (uint8_t *)&msgLen, &allocated);
      break;

    case L2CAP_SIGNAL_EVENT:
      pBuf = processEventsL2CAP((l2capSignalEvent_t *)pMsg, out_msg, (uint8_t *)&msgLen);
      break;

    case L2CAP_DATA_EVENT:
      pBuf = processDataL2CAP((l2capDataEvent_t *)pMsg, out_msg, &msgLen, &allocated);
      break;

    case GATT_MSG_EVENT:
      pBuf = processEventsGATT((gattMsgEvent_t *)pMsg, out_msg, &msgLen, &allocated);
      break;
#if !defined(GATT_DB_OFF_CHIP)
    case GATT_SERV_MSG_EVENT:
      pBuf = processEventsGATTServ((gattEventHdr_t *)pMsg, out_msg, (uint8_t *)&msgLen);
      break;
#endif
#endif /*  (CENTRAL_CFG | PERIPHERAL_CFG) */
    default:
      break; // ignore
  }

  if (msgLen)
  {
    HCI_TL_SendVSEvent(pBuf, msgLen);
    //HCI_SendControllerToHostEvent(HCI_VE_EVENT_CODE,  msgLen, pBuf);
  }

  if ((pBuf != NULL) && (allocated == TRUE))
  {
    ICall_free(pBuf);

    if (!deallocateIncoming)
    {
      return(FALSE);
    }
  }

  // Free the pLongMsg allocation
  if(pLongMsg)
  {
    MAP_osal_mem_free(pLongMsg);
    pLongMsg = NULL;
  }

  return(TRUE);
}

/*********************************************************************
 * @fn      processEventsGAP
 *
 * @brief   Process an incoming GAP Event messages.
 *
 * @param   pMsg - message to process
 * @param   pOutMsg - outgoing message to be built
 * @param   pMsgLen - length of outgoing message
 * @param   pAllocated - whether outgoing message is locally allocated
 * @param   pDeallocate - whether it's safe to deallocate incoming message
 *
 * @return  outgoing message
 */
static uint8_t *processEventsGAP(gapEventHdr_t *pMsg, uint8_t *pOutMsg, uint16_t *pMsgLen,
                                uint8_t *pAllocated, uint8_t *pDeallocate)
{
  uint16_t msgLen = 0;
  uint8_t *pBuf = NULL;

  *pDeallocate = TRUE;

  switch (pMsg->opcode)
  {
    case GAP_DEVICE_INIT_DONE_EVENT:
      {
        gapDeviceInitDoneEvent_t *pPkt = (gapDeviceInitDoneEvent_t *)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_DEVICE_INIT_DONE_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_DEVICE_INIT_DONE_EVENT);
        pOutMsg[2] = pPkt->hdr.status;
        VOID memcpy(&pOutMsg[3], pPkt->devAddr, B_ADDR_LEN);
        pOutMsg[9]  = LO_UINT16(pPkt->dataPktLen);
        pOutMsg[10] = HI_UINT16(pPkt->dataPktLen);
        pOutMsg[11] = pPkt->numDataPkts;

        // Copy the Device's local keys
        VOID memcpy(&pOutMsg[12], (uint8_t*) GAP_GetIRK(), KEYLEN);
        VOID memcpy(&pOutMsg[12+KEYLEN], (uint8_t*) GAP_GetSRK(), KEYLEN);

        pBuf = pOutMsg;
        msgLen = 44;
      }
      break;

    case GAP_LINK_ESTABLISHED_EVENT:
      {
        gapEstLinkReqEvent_t *pPkt = (gapEstLinkReqEvent_t *)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_LINK_ESTABLISHED_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_LINK_ESTABLISHED_EVENT);
        pOutMsg[2] = pPkt->hdr.status;
        pOutMsg[3] = pPkt->devAddrType;
        VOID memcpy(&(pOutMsg[4]), pPkt->devAddr, B_ADDR_LEN);
        pOutMsg[10] = LO_UINT16(pPkt->connectionHandle);
        pOutMsg[11] = HI_UINT16(pPkt->connectionHandle);
        pOutMsg[12] = pPkt->connRole;
        pOutMsg[13] = LO_UINT16(pPkt->connInterval);
        pOutMsg[14] = HI_UINT16(pPkt->connInterval);
        pOutMsg[15] = LO_UINT16(pPkt->connLatency);
        pOutMsg[16] = HI_UINT16(pPkt->connLatency);
        pOutMsg[17] = LO_UINT16(pPkt->connTimeout);
        pOutMsg[18] = HI_UINT16(pPkt->connTimeout);
        pOutMsg[19] = pPkt->clockAccuracy;
        pBuf = pOutMsg;
        msgLen = 20;
      }
      break;

    case GAP_LINK_TERMINATED_EVENT:
      {
        gapTerminateLinkEvent_t *pPkt = (gapTerminateLinkEvent_t *)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_LINK_TERMINATED_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_LINK_TERMINATED_EVENT);
        pOutMsg[2] = pPkt->hdr.status;
        pOutMsg[3] = LO_UINT16(pPkt->connectionHandle);
        pOutMsg[4] = HI_UINT16(pPkt->connectionHandle);
        pOutMsg[5] = pPkt->reason;
        pBuf = pOutMsg;
        msgLen = 6;
      }
      break;

    case GAP_UPDATE_LINK_PARAM_REQ_EVENT:
      {
        gapUpdateLinkParamReqEvent_t *pPkt =
                                           (gapUpdateLinkParamReqEvent_t *)pMsg;

        pOutMsg[0]  = LO_UINT16(HCI_EXT_GAP_LINK_PARAM_UPDATE_REQ_EVENT);
        pOutMsg[1]  = HI_UINT16(HCI_EXT_GAP_LINK_PARAM_UPDATE_REQ_EVENT);
        pOutMsg[2]  = pPkt->hdr.status;
        pOutMsg[3]  = LO_UINT16(pPkt->req.connectionHandle);
        pOutMsg[4]  = HI_UINT16(pPkt->req.connectionHandle);
        pOutMsg[5]  = LO_UINT16(pPkt->req.intervalMin);
        pOutMsg[6]  = HI_UINT16(pPkt->req.intervalMin);
        pOutMsg[7]  = LO_UINT16(pPkt->req.intervalMax);
        pOutMsg[8]  = HI_UINT16(pPkt->req.intervalMax);
        pOutMsg[9]  = LO_UINT16(pPkt->req.connLatency);
        pOutMsg[10] = HI_UINT16(pPkt->req.connLatency);
        pOutMsg[11] = LO_UINT16(pPkt->req.connTimeout);
        pOutMsg[12] = HI_UINT16(pPkt->req.connTimeout);
        pOutMsg[13] = pPkt->req.signalIdentifier;
        pBuf = pOutMsg;
        msgLen = 14;
      }
      break;

    case GAP_LINK_PARAM_UPDATE_EVENT:
      {
        gapLinkUpdateEvent_t *pPkt = (gapLinkUpdateEvent_t *)pMsg;

        pOutMsg[0]  = LO_UINT16(HCI_EXT_GAP_LINK_PARAM_UPDATE_EVENT);
        pOutMsg[1]  = HI_UINT16(HCI_EXT_GAP_LINK_PARAM_UPDATE_EVENT);
        pOutMsg[2]  = pPkt->hdr.status;
        pOutMsg[3]  = LO_UINT16(pPkt->connectionHandle);
        pOutMsg[4]  = HI_UINT16(pPkt->connectionHandle);
        pOutMsg[5]  = LO_UINT16(pPkt->connInterval);
        pOutMsg[6]  = HI_UINT16(pPkt->connInterval);
        pOutMsg[7]  = LO_UINT16(pPkt->connLatency);
        pOutMsg[8]  = HI_UINT16(pPkt->connLatency);
        pOutMsg[9]  = LO_UINT16(pPkt->connTimeout);
        pOutMsg[10] = HI_UINT16(pPkt->connTimeout);
        pBuf = pOutMsg;
        msgLen = 11;
      }
      break;

    case GAP_SIGNATURE_UPDATED_EVENT:
      {
        gapSignUpdateEvent_t *pPkt = (gapSignUpdateEvent_t *)pMsg;

        pOutMsg[0]  = LO_UINT16(HCI_EXT_GAP_SIGNATURE_UPDATED_EVENT);
        pOutMsg[1]  = HI_UINT16(HCI_EXT_GAP_SIGNATURE_UPDATED_EVENT);
        pOutMsg[2]  = pPkt->hdr.status;
        pOutMsg[3]  = pPkt->addrType;
        VOID memcpy(&(pOutMsg[4]), pPkt->devAddr, B_ADDR_LEN);
        pOutMsg[10] = BREAK_UINT32(pPkt->signCounter, 0);
        pOutMsg[11] = BREAK_UINT32(pPkt->signCounter, 1);
        pOutMsg[12] = BREAK_UINT32(pPkt->signCounter, 2);
        pOutMsg[13] = BREAK_UINT32(pPkt->signCounter, 3);
        pBuf = pOutMsg;
        msgLen = 14;
      }
      break;

    case GAP_PASSKEY_NEEDED_EVENT:
      {
        gapPasskeyNeededEvent_t *pPkt = (gapPasskeyNeededEvent_t *)pMsg;

        pOutMsg[0]  = LO_UINT16(HCI_EXT_GAP_PASSKEY_NEEDED_EVENT);
        pOutMsg[1]  = HI_UINT16(HCI_EXT_GAP_PASSKEY_NEEDED_EVENT);
        pOutMsg[2]  = pPkt->hdr.status;
        VOID memcpy(&(pOutMsg[3]), pPkt->deviceAddr, B_ADDR_LEN);
        pOutMsg[9] = LO_UINT16(pPkt->connectionHandle);
        pOutMsg[10] = HI_UINT16(pPkt->connectionHandle);
        pOutMsg[11] = pPkt->uiInputs;
        pOutMsg[12] = pPkt->uiOutputs;
        VOID memcpy(&(pOutMsg[13]), &pPkt->numComparison, 4);
        pBuf = pOutMsg;
        msgLen = 17;
      }
      break;

    case GAP_AUTHENTICATION_COMPLETE_EVENT:
      {
        gapAuthCompleteEvent_t *pPkt = (gapAuthCompleteEvent_t *)pMsg;

        msgLen = 107;

        pBuf = ICall_malloc(msgLen);
        if (pBuf)
        {
          uint8_t *buf = pBuf;

          *pAllocated = TRUE;

          VOID memset(buf, 0, msgLen);

          *buf++  = LO_UINT16(HCI_EXT_GAP_AUTH_COMPLETE_EVENT);
          *buf++  = HI_UINT16(HCI_EXT_GAP_AUTH_COMPLETE_EVENT);
          *buf++  = pPkt->hdr.status;
          *buf++  = LO_UINT16(pPkt->connectionHandle);
          *buf++  = HI_UINT16(pPkt->connectionHandle);
          *buf++  = pPkt->authState;

          if (pPkt->pSecurityInfo)
          {
            *buf++ = TRUE;
            *buf++ = pPkt->pSecurityInfo->keySize;
            VOID memcpy(buf, pPkt->pSecurityInfo->ltk, KEYLEN);
            buf += KEYLEN;
            *buf++ = LO_UINT16(pPkt->pSecurityInfo->div);
            *buf++ = HI_UINT16(pPkt->pSecurityInfo->div);
            VOID memcpy(buf, pPkt->pSecurityInfo->rand, B_RANDOM_NUM_SIZE);
            buf += B_RANDOM_NUM_SIZE;
          }
          else
          {
            // Skip securityInfo
            buf += 1 + KEYLEN + B_RANDOM_NUM_SIZE + 2 + 1;
          }

          if (pPkt->pDevSecInfo)
          {
            *buf++ = TRUE;
            *buf++ = pPkt->pDevSecInfo->keySize;
            VOID memcpy(buf, pPkt->pDevSecInfo->ltk, KEYLEN);
            buf += KEYLEN;
            *buf++ = LO_UINT16(pPkt->pDevSecInfo->div);
            *buf++ = HI_UINT16(pPkt->pDevSecInfo->div);
            VOID memcpy(buf, pPkt->pDevSecInfo->rand, B_RANDOM_NUM_SIZE);
            buf += B_RANDOM_NUM_SIZE;
          }
          else
          {
            // Skip securityInfo
            buf += 1 + KEYLEN + B_RANDOM_NUM_SIZE + 2 + 1;
          }

          if (pPkt->pIdentityInfo)
          {
            *buf++ = TRUE;
            VOID memcpy(buf, pPkt->pIdentityInfo->irk, KEYLEN);
            buf += KEYLEN;
            VOID memcpy(buf, pPkt->pIdentityInfo->bd_addr, B_ADDR_LEN);
            buf += B_ADDR_LEN;
            *buf++ = pPkt->pIdentityInfo->addrType;
          }
          else
          {
            // Skip identityInfo
            buf += KEYLEN + B_ADDR_LEN + 2;
          }

          if (pPkt->pSigningInfo)
          {
            *buf++ = TRUE;
            VOID memcpy(buf, pPkt->pSigningInfo->srk, KEYLEN);
            buf += KEYLEN;

            *buf++ = BREAK_UINT32(pPkt->pSigningInfo->signCounter, 0);
            *buf++ = BREAK_UINT32(pPkt->pSigningInfo->signCounter, 1);
            *buf++ = BREAK_UINT32(pPkt->pSigningInfo->signCounter, 2);
            *buf = BREAK_UINT32(pPkt->pSigningInfo->signCounter, 3);
          }
        }
        else
        {
          pOutMsg[0]  = LO_UINT16(HCI_EXT_GAP_AUTH_COMPLETE_EVENT);
          pOutMsg[1]  = HI_UINT16(HCI_EXT_GAP_AUTH_COMPLETE_EVENT);
          pOutMsg[2]  = bleMemAllocError;
          pOutMsg[3]  = LO_UINT16(pPkt->connectionHandle);
          pOutMsg[4]  = HI_UINT16(pPkt->connectionHandle);
          pBuf = pOutMsg;
          msgLen = 5;
        }
      }
      break;

    case GAP_BOND_COMPLETE_EVENT:
      {
        gapBondCompleteEvent_t *pPkt = (gapBondCompleteEvent_t *)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_BOND_COMPLETE_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_BOND_COMPLETE_EVENT);
        pOutMsg[2] = pPkt->hdr.status;
        pOutMsg[3] = LO_UINT16(pPkt->connectionHandle);
        pOutMsg[4] = HI_UINT16(pPkt->connectionHandle);
        pBuf = pOutMsg;
        msgLen = 5;
      }
      break;

    case GAP_PAIRING_REQ_EVENT:
      {
        gapPairingReqEvent_t *pPkt = (gapPairingReqEvent_t *)pMsg;
        uint8_t tmp = 0;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_PAIRING_REQ_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_PAIRING_REQ_EVENT);
        pOutMsg[2] = pPkt->hdr.status;
        pOutMsg[3] = LO_UINT16(pPkt->connectionHandle);
        pOutMsg[4] = HI_UINT16(pPkt->connectionHandle);
        pOutMsg[5] = pPkt->pairReq.ioCap;
        pOutMsg[6] = pPkt->pairReq.oobDataFlag;
        pOutMsg[7] = pPkt->pairReq.authReq;
        pOutMsg[8] = pPkt->pairReq.maxEncKeySize;

        tmp |= (pPkt->pairReq.keyDist.pEncKey) ? KEYDIST_SENC : 0;
        tmp |= (pPkt->pairReq.keyDist.pIdKey) ? KEYDIST_SID : 0;
        tmp |= (pPkt->pairReq.keyDist.pSign) ? KEYDIST_SSIGN : 0;
        tmp |= (pPkt->pairReq.keyDist.pLinkKey) ? KEYDIST_SLINK : 0;
        tmp |= (pPkt->pairReq.keyDist.cEncKey) ? KEYDIST_MENC : 0;
        tmp |= (pPkt->pairReq.keyDist.cIdKey) ? KEYDIST_MID : 0;
        tmp |= (pPkt->pairReq.keyDist.cSign) ? KEYDIST_MSIGN : 0;
        tmp |= (pPkt->pairReq.keyDist.cLinkKey) ? KEYDIST_MLINK : 0;
        pOutMsg[9] = tmp;

        pBuf = pOutMsg;
        msgLen = 10;
      }
      break;

    case GAP_PERIPHERAL_REQUESTED_SECURITY_EVENT:
      {
        gapPeripheralSecurityReqEvent_t *pPkt = (gapPeripheralSecurityReqEvent_t *)pMsg;

        pOutMsg[0]  = LO_UINT16(HCI_EXT_GAP_PERIPHERAL_REQUESTED_SECURITY_EVENT);
        pOutMsg[1]  = HI_UINT16(HCI_EXT_GAP_PERIPHERAL_REQUESTED_SECURITY_EVENT);
        pOutMsg[2]  = pPkt->hdr.status;
        pOutMsg[3]  = LO_UINT16(pPkt->connectionHandle);
        pOutMsg[4]  = HI_UINT16(pPkt->connectionHandle);
        VOID memcpy(&(pOutMsg[5]), pPkt->deviceAddr, B_ADDR_LEN);
        pOutMsg[11] = pPkt->authReq;
        pBuf = pOutMsg;
        msgLen = 12;
      }
      break;

    case GAP_CONNECTING_CANCELLED_EVENT:
      {
#ifdef BLE3_CMD
        if( !legacyConnCancel )
        {
#endif
          gapConnCancelledEvent_t *pPkt = (gapConnCancelledEvent_t *)pMsg;

          pOutMsg[0]  = LO_UINT16(HCI_EXT_GAP_CONNECTING_CANCELLED_EVENT);
          pOutMsg[1]  = HI_UINT16(HCI_EXT_GAP_CONNECTING_CANCELLED_EVENT);
          pOutMsg[2]  = pPkt->hdr.status;
          pBuf = pOutMsg;
          msgLen = 3;
#ifdef BLE3_CMD
        }
        else
        {
          // Send Link establish event with status "bleGAPConnNotAcceptable"

          pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_LINK_ESTABLISHED_EVENT);
          pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_LINK_ESTABLISHED_EVENT);
          pOutMsg[2] = bleGAPConnNotAcceptable;
          pOutMsg[3] = 0;
          memset(&(pOutMsg[4]), 0, B_ADDR_LEN);
          pOutMsg[10] = 0;
          pOutMsg[11] = 0;
          pOutMsg[12] = GAP_PROFILE_CENTRAL;
          pOutMsg[13] = 0;
          pOutMsg[14] = 0;
          pOutMsg[15] = 0;
          pOutMsg[16] = 0;
          pOutMsg[17] = 0;
          pOutMsg[18] = 0;
          pOutMsg[19] = 0;
          pBuf = pOutMsg;
          msgLen = 20;

          legacyConnCancel = 0;
        }
#endif
      }
      break;

    case GAP_BOND_LOST_EVENT:
      {
        gapBondLostEvent_t *pPkt = (gapBondLostEvent_t *)pMsg;

        pOutMsg[0]  = LO_UINT16(HCI_EXT_GAP_BOND_LOST_EVENT);
        pOutMsg[1]  = HI_UINT16(HCI_EXT_GAP_BOND_LOST_EVENT);
        pOutMsg[2]  = pPkt->hdr.status;
        pOutMsg[3]  = LO_UINT16(pPkt->connectionHandle);
        pOutMsg[4]  = HI_UINT16(pPkt->connectionHandle);
        VOID memcpy(&(pOutMsg[5]), pPkt->deviceAddr, B_ADDR_LEN);
        pBuf = pOutMsg;
        msgLen = 11;
      }
      break;

      case GAP_ADV_SET_PERIODIC_ADV_PARAMS_EVENT:
      {
        GapAdv_periodicAdvEvt_t *pPkt = (GapAdv_periodicAdvEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_ADV_SET_PERIODIC_ADV_PARAMS_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_ADV_SET_PERIODIC_ADV_PARAMS_EVENT);
        pOutMsg[2] = pPkt->status;
        pBuf = pOutMsg;
        msgLen = 3;

        break;
      }

      case GAP_ADV_SET_PERIODIC_ADV_DATA_EVENT:
      {
        GapAdv_periodicAdvEvt_t *pPkt = (GapAdv_periodicAdvEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_ADV_SET_PERIODIC_ADV_DATA_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_ADV_SET_PERIODIC_ADV_DATA_EVENT);
        pOutMsg[2] = pPkt->status;
        pBuf = pOutMsg;
        msgLen = 3;
        break;
      }

      case GAP_ADV_SET_PERIODIC_ADV_ENABLE_EVENT:
      {
        GapAdv_periodicAdvEvt_t *pPkt = (GapAdv_periodicAdvEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_ADV_SET_PERIODIC_ADV_ENABLE_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_ADV_SET_PERIODIC_ADV_ENABLE_EVENT);
        pOutMsg[2] = pPkt->status;
        pBuf = pOutMsg;
        msgLen = 3;
        break;
      }

      case GAP_SCAN_PERIODIC_ADV_SYNC_EST_EVENT:
      {
        GapScan_Evt_PeriodicAdvSyncEst_t *pEvt = (GapScan_Evt_PeriodicAdvSyncEst_t *)pMsg;

        if( pEvt != NULL )
        {
          pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_PERIODIC_SYNC_EST_EVENT);
          pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_PERIODIC_SYNC_EST_EVENT);
          pOutMsg[2] = pEvt->BLEEventCode;
          pOutMsg[3] = pEvt->status;
          pOutMsg[4] = LO_UINT16(pEvt->syncHandle);
          pOutMsg[5] = HI_UINT16(pEvt->syncHandle);
          pOutMsg[6] = pEvt->advSid;
          pOutMsg[7] = pEvt->advPhy;
          pOutMsg[8] = LO_UINT16(pEvt->periodicAdvInt);
          pOutMsg[9] = HI_UINT16(pEvt->periodicAdvInt);
          pOutMsg[10] = pEvt->advClockAccuracy;
          pOutMsg[11] = pEvt->advAddrType;
          MAP_osal_memcpy(&(pOutMsg[12]), pEvt->advAddress, B_ADDR_LEN);

          pBuf = pOutMsg;
          msgLen = 18;
        }
        break;
      }

      case GAP_SCAN_PERIODIC_ADV_SYNC_LOST_EVENT:
      {
        GapScan_PeriodicAdvSyncLostEvt_t *pEvt = (GapScan_PeriodicAdvSyncLostEvt_t *)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_PERIODIC_ADV_SYNC_LOST_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_PERIODIC_ADV_SYNC_LOST_EVENT);
        pOutMsg[2] = LO_UINT16(pEvt->syncHandle);
        pOutMsg[3] = HI_UINT16(pEvt->syncHandle);

        pBuf = pOutMsg;
        msgLen = 4;
        break;
      }

      case GAP_SCAN_PERIODIC_ADV_REPORT_EVENT:
      {
        GapScan_Evt_PeriodicAdvRpt_t *pEvt = (GapScan_Evt_PeriodicAdvRpt_t*)pMsg;
        uint8_t dataLen = pEvt->dataLen;
        uint16_t totalLen = sizeof(GapScan_Evt_PeriodicAdvRpt_t)-sizeof(((GapScan_Evt_PeriodicAdvRpt_t *)0)->pData) + dataLen;
        pLongMsg = (uint8_t*)MAP_osal_mem_alloc(totalLen);

        if(pLongMsg)
        {
          pLongMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_PERIODIC_ADV_REPORT_EVENT);
          pLongMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_PERIODIC_ADV_REPORT_EVENT);
          pLongMsg[2] = LO_UINT16(pEvt->syncHandle);
          pLongMsg[3] = HI_UINT16(pEvt->syncHandle);
          pLongMsg[4] = pEvt->txPower;
          pLongMsg[5] = pEvt->rssi;
          pLongMsg[6] = pEvt->cteType;
          pLongMsg[7] = pEvt->dataStatus;
          pLongMsg[8] = pEvt->dataLen;

          if ( (pEvt->pData != NULL) && (pEvt->dataLen > 0) )
          {
            MAP_osal_memcpy(&pLongMsg[9], pEvt->pData, pEvt->dataLen);
            MAP_osal_mem_free(pEvt->pData);
          }

          pBuf = pLongMsg;
          msgLen = 9 + pEvt->dataLen;
        }
        else
        {
          HCI_TL_sendSystemReport(HOST_TL_ID, LL_STATUS_ERROR_OUT_OF_HEAP, HCI_EXT_GAP_ADV_SCAN_EVENT);
        }
        break;
      }

      case GAP_SCAN_CREATE_SYNC_EVENT:
      {
        GapScan_PeriodicAdvEvt_t *pPkt = (GapScan_PeriodicAdvEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_CREATE_SYNC_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_CREATE_SYNC_EVENT);
        pOutMsg[2] = pPkt->status;
        pBuf = pOutMsg;
        msgLen = 3;
        break;
      }

      case GAP_SCAN_SYNC_CANCEL_EVENT:
      {
        GapScan_PeriodicAdvEvt_t *pPkt = (GapScan_PeriodicAdvEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_CREATE_SYNC_CANCEL_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_CREATE_SYNC_CANCEL_EVENT);
        pOutMsg[2] = pPkt->status;
        pBuf = pOutMsg;
        msgLen = 3;
        break;
      }

      case GAP_SCAN_TERMINATE_SYNC_EVENT:
      {
        GapScan_PeriodicAdvEvt_t *pPkt = (GapScan_PeriodicAdvEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_TERMINATE_SYNC_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_TERMINATE_SYNC_EVENT);
        pOutMsg[2] = pPkt->status;
        pBuf = pOutMsg;
        msgLen = 3;
        break;
      }

      case GAP_SCAN_PERIODIC_RECEIVE_EVENT:
      {
        GapScan_PeriodicAdvEvt_t *pPkt = (GapScan_PeriodicAdvEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_RECEIVE_PERIODIC_ADV_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_RECEIVE_PERIODIC_ADV_EVENT);
        pOutMsg[2] = pPkt->status;
        pBuf = pOutMsg;
        msgLen = 3;
        break;
      }

      case GAP_SCAN_ADD_DEVICE_ADV_LIST_EVENT:
      {
        GapScan_PeriodicAdvEvt_t *pPkt = (GapScan_PeriodicAdvEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_ADD_DEVICE_PERIODIC_ADV_LIST_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_ADD_DEVICE_PERIODIC_ADV_LIST_EVENT);
        pOutMsg[2] = pPkt->status;
        pBuf = pOutMsg;
        msgLen = 3;
        break;
      }

      case GAP_SCAN_REMOVE_DEVICE_ADV_LIST_EVENT:
      {
        GapScan_PeriodicAdvEvt_t *pPkt = (GapScan_PeriodicAdvEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_REMOVE_DEVICE_PERIODIC_ADV_LIST_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_REMOVE_DEVICE_PERIODIC_ADV_LIST_EVENT);
        pOutMsg[2] = pPkt->status;
        pBuf = pOutMsg;
        msgLen = 3;
        break;
      }

      case GAP_SCAN_CLEAR_ADV_LIST_EVENT:
      {
        GapScan_PeriodicAdvEvt_t *pPkt = (GapScan_PeriodicAdvEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_CLEAR_PERIODIC_ADV_LIST_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_CLEAR_PERIODIC_ADV_LIST_EVENT);
        pOutMsg[2] = pPkt->status;
        pBuf = pOutMsg;
        msgLen = 3;
        break;
      }

      case GAP_SCAN_READ_ADV_LIST_SIZE_EVENT:
      {
        GapScan_ReadAdvSizeListEvt_t *pPkt = (GapScan_ReadAdvSizeListEvt_t*)pMsg;

        pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_SCAN_READ_PERIODIC_ADV_LIST_SIZE_EVENT);
        pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_SCAN_READ_PERIODIC_ADV_LIST_SIZE_EVENT);
        pOutMsg[2] = pPkt->status;
        pOutMsg[3] = pPkt->listSize;

        pBuf = pOutMsg;
        msgLen = 4;
        break;
      }

#ifdef BLE3_CMD
#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )
      case GAP_ADV_DATA_UPDATE_DONE_EVENT:
      {
        msgLen = 0;

        if (advEventCntr == 2)
        {
          GapAdv_dataUpdateEvent_t *pPkt = (GapAdv_dataUpdateEvent_t *)pMsg;

          pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_ADV_DATA_UPDATE_DONE_EVENT);
          pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_ADV_DATA_UPDATE_DONE_EVENT);
          pOutMsg[2] = pPkt->hdr.status;
          pOutMsg[3] = pPkt->dataType;
          pBuf = pOutMsg;
          msgLen = 4;
        }
        else
        {
          advEventCntr++;
        }
        break;
      }

      case GAP_ADV_MAKE_DISCOVERABLE_DONE_EVENT:
      {
        if( makeDiscFlag )
        {
          // This event is a response to GapAdv_enable
          GapAv_setAdvStatusDoneEvent_t *pPkt = (GapAv_setAdvStatusDoneEvent_t*)pMsg;

          pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_MAKE_DISCOVERABLE_DONE_EVENT);
          pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_MAKE_DISCOVERABLE_DONE_EVENT);
          pOutMsg[2] = pPkt->hdr.status;
          pBuf = pOutMsg;
          msgLen = 3;

          makeDiscFlag = 0;
        }
        break;
      }

      case GAP_ADV_END_DISCOVERABLE_DONE_EVENT:
      {
        if( endDiscFlag )
        {
          // This event is a response to GapAdv_disable
          GapAv_setAdvStatusDoneEvent_t *pPkt = (GapAv_setAdvStatusDoneEvent_t*)pMsg;

          pOutMsg[0] = LO_UINT16(HCI_EXT_GAP_END_DISCOVERABLE_DONE_EVENT);
          pOutMsg[1] = HI_UINT16(HCI_EXT_GAP_END_DISCOVERABLE_DONE_EVENT);
          pOutMsg[2] = pPkt->hdr.status;
          pBuf = pOutMsg;
          msgLen = 3;

          endDiscFlag = 0;
        }
        break;
      }
#endif // HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG)
#endif // BLE3_CMD

    default:
      // Unknown command
      break;
  }

  *pMsgLen = msgLen;

  return(pBuf);
}

#if (HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG))
/*********************************************************************
 * @fn      processEventsSM
 *
 * @brief   Process an incoming SM Event messages.
 *
 * @param   pMsg       - packet to process
 * @param   pOutMsg    - outgoing message to be built
 * @param   pMsgLen    - length of outgoing message
 * @param   pAllocated - whether outgoing message is locally allocated
 *
 * @return  outgoing message
 */
static uint8_t *processEventsSM(smEventHdr_t *pMsg, uint8_t *pOutMsg,
                               uint8_t *pMsgLen, uint8_t *pAllocated)
{
  uint8_t msgLen = 0;
  uint8_t *pBuf = NULL;

  switch (pMsg->opcode)
  {
    case SM_ECC_KEYS_EVENT:
        {
          smEccKeysEvt_t *keys = (smEccKeysEvt_t *)pMsg;

          msgLen = (SM_ECC_KEY_LEN * 3) + 3;

          pBuf = ICall_malloc(msgLen);
          if (pBuf)
          {
            uint8_t *buf = pBuf;

            *buf++ = LO_UINT16(HCI_EXT_SM_GET_ECC_KEYS_EVENT);
            *buf++ = HI_UINT16(HCI_EXT_SM_GET_ECC_KEYS_EVENT);
            *buf++ = keys->hdr.status;
            memcpy(buf, keys->privateKey, SM_ECC_KEY_LEN);
            buf+=32;
            memcpy(buf, keys->publicKeyX, SM_ECC_KEY_LEN);
            buf+=32;
            memcpy(buf, keys->publicKeyY, SM_ECC_KEY_LEN);

            *pAllocated = TRUE;
          }
          else
          {
            keys->hdr.status = bleMemAllocError;
            msgLen = 0;
          }
        }
        break;

      case SM_DH_KEY_EVENT:
        {
          smDhKeyEvt_t *dhKey = (smDhKeyEvt_t *)pMsg;

          msgLen = SM_ECC_KEY_LEN + 3;

          pBuf = ICall_malloc(msgLen);
          if (pBuf)
          {
            uint8_t *buf = pBuf;
            *buf++ = LO_UINT16(HCI_EXT_SM_GET_DH_KEY_EVENT);
            *buf++ = HI_UINT16(HCI_EXT_SM_GET_DH_KEY_EVENT);
            *buf++ = dhKey->hdr.status;
            memcpy(buf, dhKey->dhKey, SM_ECC_KEY_LEN);

            *pAllocated = TRUE;
          }
          else
          {
            dhKey->hdr.status = bleMemAllocError;
            msgLen = 0;
          }
        }
        break;

      default:
        break;
  }

  *pMsgLen = msgLen;

  return(pBuf);
}


/*********************************************************************
 * @fn      processEventsL2CAP
 *
 * @brief   Process an incoming L2CAP Event messages.
 *
 * @param   pPkt - packet to process
 * @param   pOutMsg - outgoing message to be built
 * @param   pMsgLen - length of outgoing message
 *
 * @return  outgoing message
 */
static uint8_t *processEventsL2CAP(l2capSignalEvent_t *pPkt, uint8_t *pOutMsg, uint8_t *pMsgLen)
{
  uint8_t msgLen;

  // Build the message header first
  msgLen = buildHCIExtHeader(pOutMsg, (HCI_EXT_L2CAP_EVENT | pPkt->opcode),
                              pPkt->hdr.status, pPkt->connHandle);
#if defined(BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
  // Build Send SDU Done event regardless of status
  if (pPkt->opcode == L2CAP_SEND_SDU_DONE_EVT)
  {
    l2capSendSduDoneEvt_t *pSduEvt = &(pPkt->cmd.sendSduDoneEvt);

    pOutMsg[msgLen++] = LO_UINT16(pSduEvt->CID);
    pOutMsg[msgLen++] = HI_UINT16(pSduEvt->CID);

    pOutMsg[msgLen++] = LO_UINT16(pSduEvt->credits);
    pOutMsg[msgLen++] = HI_UINT16(pSduEvt->credits);

    pOutMsg[msgLen++] = LO_UINT16(pSduEvt->peerCID);
    pOutMsg[msgLen++] = HI_UINT16(pSduEvt->peerCID);

    pOutMsg[msgLen++] = LO_UINT16(pSduEvt->peerCredits);
    pOutMsg[msgLen++] = HI_UINT16(pSduEvt->peerCredits);

    pOutMsg[msgLen++] = LO_UINT16(pSduEvt->totalLen);
    pOutMsg[msgLen++] = HI_UINT16(pSduEvt->totalLen);

    pOutMsg[msgLen++] = LO_UINT16(pSduEvt->txLen);
    pOutMsg[msgLen++] = HI_UINT16(pSduEvt->txLen);
  }
  else
#endif //(BLE_V41_FEATURES & L2CAP_COC_CFG)
  if (pPkt->hdr.status == SUCCESS) // Build all other messages if status is success
  {
    switch (pPkt->opcode)
    {
      case L2CAP_CMD_REJECT:
        msgLen += L2CAP_BuildCmdReject(&pOutMsg[msgLen], (uint8_t *)&(pPkt->cmd.cmdReject));
        break;

      case L2CAP_PARAM_UPDATE_RSP:
        msgLen += L2CAP_BuildParamUpdateRsp(&pOutMsg[msgLen], (uint8_t *)&(pPkt->cmd.updateRsp));
        break;

      case L2CAP_INFO_RSP:
        msgLen += L2CAP_BuildInfoRsp(&pOutMsg[msgLen], (uint8_t *)&(pPkt->cmd.infoRsp));
        break;

#if defined(BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
      case L2CAP_CONNECT_IND:
        pOutMsg[msgLen++] = pPkt->id;
        msgLen += L2CAP_BuildConnectReq(&pOutMsg[msgLen], (uint8_t *)&(pPkt->cmd.connectReq));
        break;

      case L2CAP_CHANNEL_ESTABLISHED_EVT:
        {
          l2capChannelEstEvt_t *pEstEvt = &(pPkt->cmd.channelEstEvt);

          pOutMsg[msgLen++] = LO_UINT16(pEstEvt->result);
          pOutMsg[msgLen++] = HI_UINT16(pEstEvt->result);

          if (pEstEvt->result == L2CAP_CONN_SUCCESS)
          {
            msgLen += buildCoChannelInfo(pEstEvt->CID, &pEstEvt->info, &pOutMsg[msgLen]);
          }
        }
        break;

       case L2CAP_CHANNEL_TERMINATED_EVT:
        {
          l2capChannelTermEvt_t *pTermEvt = &(pPkt->cmd.channelTermEvt);

          pOutMsg[msgLen++] = LO_UINT16(pTermEvt->CID);
          pOutMsg[msgLen++] = HI_UINT16(pTermEvt->CID);

          pOutMsg[msgLen++] = LO_UINT16(pTermEvt->peerCID);
          pOutMsg[msgLen++] = HI_UINT16(pTermEvt->peerCID);

          pOutMsg[msgLen++] = LO_UINT16(pTermEvt->reason);
          pOutMsg[msgLen++] = HI_UINT16(pTermEvt->reason);
        }
        break;

      case L2CAP_OUT_OF_CREDIT_EVT:
      case L2CAP_PEER_CREDIT_THRESHOLD_EVT:
        {
          l2capCreditEvt_t *pCreditEvt = &(pPkt->cmd.creditEvt);

          pOutMsg[msgLen++] = LO_UINT16(pCreditEvt->CID);
          pOutMsg[msgLen++] = HI_UINT16(pCreditEvt->CID);

          pOutMsg[msgLen++] = LO_UINT16(pCreditEvt->peerCID);
          pOutMsg[msgLen++] = HI_UINT16(pCreditEvt->peerCID);

          pOutMsg[msgLen++] = LO_UINT16(pCreditEvt->credits);
          pOutMsg[msgLen++] = HI_UINT16(pCreditEvt->credits);
        }
        break;
#endif //(BLE_V41_FEATURES & L2CAP_COC_CFG)

      default:
        // Unknown command
        break;
    }
  }

  *pMsgLen = msgLen;

  return(pOutMsg);
}

/*********************************************************************
 * @fn      processDataL2CAP
 *
 * @brief   Process an incoming L2CAP Data message.
 *
 * @param   pPkt - packet to process
 * @param   pOutMsg - outgoing message to be built
 * @param   pMsgLen - length of outgoing message
 *
 * @return  outgoing message
 */
static uint8_t *processDataL2CAP(l2capDataEvent_t *pPkt, uint8_t *pOutMsg,
                                uint16_t *pMsgLen, uint8_t *pAllocated)
{
  uint8_t *pBuf;
  uint16_t msgLen = HCI_EXT_HDR_LEN + 2 + 2; // hdr + CID + packet length;
  uint8_t status = pPkt->hdr.status;

  *pAllocated = FALSE;

  msgLen += pPkt->pkt.len;
  if (msgLen > HCI_EXT_APP_OUT_BUF)
  {
    pBuf = ICall_malloc(msgLen);
    if (pBuf)
    {
      *pAllocated = TRUE;
    }
    else
    {
      pBuf = pOutMsg;
      msgLen -= pPkt->pkt.len;

      status = bleMemAllocError;
    }
  }
  else
  {
    pBuf = pOutMsg;
  }

  // Build the message header first
  VOID buildHCIExtHeader(pBuf, (HCI_EXT_L2CAP_EVENT | HCI_EXT_L2CAP_DATA),
                          status, pPkt->pkt.connHandle);
  // Add CID
  pBuf[HCI_EXT_HDR_LEN] = LO_UINT16(pPkt->pkt.CID);
  pBuf[HCI_EXT_HDR_LEN+1] = HI_UINT16(pPkt->pkt.CID);

  // Add data length
  pBuf[HCI_EXT_HDR_LEN+2] = LO_UINT16(pPkt->pkt.len);
  pBuf[HCI_EXT_HDR_LEN+3] = HI_UINT16(pPkt->pkt.len);

  // Add payload
  if (pPkt->pkt.pPayload != NULL)
  {
    if (status == SUCCESS)
    {
      VOID memcpy(&pBuf[HCI_EXT_HDR_LEN+4], pPkt->pkt.pPayload, pPkt->pkt.len);
    }

    // Received buffer is processed so it's safe to free it
    BM_free(pPkt->pkt.pPayload);
  }

  *pMsgLen = msgLen;

  return(pBuf);
}

/*********************************************************************
 * @fn      processEventsGATT
 *
 * @brief   Process an incoming GATT Event messages.
 *
 * @param   pPkt - packet to process
 * @param   pOutMsg - outgoing message to be built
 * @param   pMsgLen - length of outgoing message
 * @param   pAllocated - whether outgoing message is locally allocated
 *
 * @return  outgoing message
 */
static uint8_t *processEventsGATT(gattMsgEvent_t *pPkt, uint8_t *pOutMsg,
                                  uint16_t *pMsgLen, uint8_t *pAllocated)
{
  uint16_t msgLen = 0;
  uint8_t attHdrLen = 0, hdrLen = HCI_EXT_HDR_LEN + 1; // hdr + event length
  uint8_t *pBuf, *pPayload = NULL;
  uint8_t status = pPkt->hdr.status;

  *pAllocated = FALSE;

  if ((status == SUCCESS) || (status == blePending))
  {
    // Build the ATT header first
    switch (pPkt->method)
    {
      case ATT_ERROR_RSP:
        attHdrLen = ATT_BuildErrorRsp(&pOutMsg[hdrLen], (uint8_t *)&(pPkt->msg.errorRsp));
        break;

      case ATT_EXCHANGE_MTU_REQ:
      case ATT_MTU_UPDATED_EVENT:
        attHdrLen = ATT_BuildExchangeMTUReq(&pOutMsg[hdrLen], (uint8_t *)&(pPkt->msg.exchangeMTUReq));
        break;

      case ATT_EXCHANGE_MTU_RSP:
        attHdrLen = ATT_BuildExchangeMTURsp(&pOutMsg[hdrLen], (uint8_t *)&(pPkt->msg.exchangeMTURsp));
        break;

      case ATT_FIND_INFO_REQ:
        attHdrLen = ATT_BuildFindInfoReq(&pOutMsg[hdrLen], (uint8_t *)&(pPkt->msg.findInfoReq));
        break;

      case ATT_FIND_INFO_RSP:
        {
          attFindInfoRsp_t *pRsp = &pPkt->msg.findInfoRsp;

          attHdrLen = ATT_FIND_INFO_RSP_FIXED_SIZE;

          // Copy response header over
          msgLen = ATT_BuildFindInfoRsp(&pOutMsg[hdrLen], (uint8_t *)pRsp) - attHdrLen;
          pPayload = pRsp->pInfo;
        }
        break;

      case ATT_FIND_BY_TYPE_VALUE_REQ:
        {
          attFindByTypeValueReq_t *pReq = &pPkt->msg.findByTypeValueReq;

          attHdrLen = ATT_FIND_BY_TYPE_VALUE_REQ_FIXED_SIZE;

          // Copy request header over
          msgLen = ATT_BuildFindByTypeValueReq(&pOutMsg[hdrLen], (uint8_t *)pReq) - attHdrLen;
          pPayload = pReq->pValue;
        }
        break;

      case ATT_FIND_BY_TYPE_VALUE_RSP:
        {
          attFindByTypeValueRsp_t *pRsp = &pPkt->msg.findByTypeValueRsp;

          msgLen = ATT_BuildFindByTypeValueRsp(&pOutMsg[hdrLen], (uint8_t *)pRsp);
          pPayload = pRsp->pHandlesInfo;
        }
        break;

      case ATT_READ_BY_TYPE_REQ:
        attHdrLen = ATT_BuildReadByTypeReq(&pOutMsg[hdrLen], (uint8_t *)&(pPkt->msg.readByTypeReq));
        break;

      case ATT_READ_BY_TYPE_RSP:
        {
          attReadByTypeRsp_t *pRsp = &pPkt->msg.readByTypeRsp;

          attHdrLen = ATT_READ_BY_TYPE_RSP_FIXED_SIZE;

          // Copy request header over
          msgLen = ATT_BuildReadByTypeRsp(&pOutMsg[hdrLen], (uint8_t *)pRsp) - attHdrLen;
          pPayload = pRsp->pDataList;
        }
        break;

      case ATT_READ_REQ:
        attHdrLen = ATT_BuildReadReq(&pOutMsg[hdrLen], (uint8_t *)&(pPkt->msg.readReq));
        break;

      case ATT_READ_RSP:
        {
          attReadRsp_t *pRsp = &pPkt->msg.readRsp;

          msgLen = ATT_BuildReadRsp(&pOutMsg[hdrLen], (uint8_t *)pRsp);
          pPayload = pRsp->pValue;
        }
        break;

      case ATT_READ_BLOB_REQ:
        attHdrLen = ATT_BuildReadBlobReq(&pOutMsg[hdrLen], (uint8_t *)&(pPkt->msg.readBlobReq));
        break;

      case ATT_READ_BLOB_RSP:
        {
          attReadBlobRsp_t *pRsp = &pPkt->msg.readBlobRsp;

          msgLen = ATT_BuildReadBlobRsp(&pOutMsg[hdrLen], (uint8_t *)pRsp);
          pPayload = pRsp->pValue;
        }
        break;

      case ATT_READ_MULTI_REQ:
        {
          attReadMultiReq_t *pReq = &pPkt->msg.readMultiReq;

          msgLen = ATT_BuildReadMultiReq(&pOutMsg[hdrLen], (uint8_t *)pReq);
          pPayload = pReq->pHandles;
        }
        break;

      case ATT_READ_MULTI_RSP:
        {
          attReadMultiRsp_t *pRsp = &pPkt->msg.readMultiRsp;

          msgLen = ATT_BuildReadMultiRsp(&pOutMsg[hdrLen], (uint8_t *)pRsp);
          pPayload = pRsp->pValues;
        }
        break;

      case ATT_READ_BY_GRP_TYPE_REQ:
        attHdrLen = ATT_BuildReadByTypeReq(&pOutMsg[hdrLen], (uint8_t *)&(pPkt->msg.readByGrpTypeReq));
        break;

      case ATT_READ_BY_GRP_TYPE_RSP:
        {
          attReadByGrpTypeRsp_t *pRsp = &pPkt->msg.readByGrpTypeRsp;

          attHdrLen = ATT_READ_BY_GRP_TYPE_RSP_FIXED_SIZE;

          // Copy request header over
          msgLen = ATT_BuildReadByGrpTypeRsp(&pOutMsg[hdrLen], (uint8_t *)pRsp) - attHdrLen;
          pPayload = pRsp->pDataList;
        }
        break;

      case ATT_WRITE_REQ:
        {
          attWriteReq_t *pReq = &pPkt->msg.writeReq;

          pOutMsg[hdrLen] = pReq->sig;
          pOutMsg[hdrLen+1] = pReq->cmd;

          attHdrLen = ATT_WRITE_REQ_FIXED_SIZE;

          // Copy request header over
          msgLen = ATT_BuildWriteReq(&pOutMsg[hdrLen+2], (uint8_t *)pReq) - attHdrLen;
          pPayload = pReq->pValue;

          attHdrLen += 2; // sig + cmd
        }
        break;

      case ATT_PREPARE_WRITE_REQ:
        {
          attPrepareWriteReq_t *pReq = &pPkt->msg.prepareWriteReq;

          attHdrLen = ATT_PREPARE_WRITE_REQ_FIXED_SIZE;

          // Copy request header over
          msgLen = ATT_BuildPrepareWriteReq(&pOutMsg[hdrLen], (uint8_t *)pReq) - attHdrLen;
          pPayload = pReq->pValue;
        }
        break;

      case ATT_PREPARE_WRITE_RSP:
        {
          attPrepareWriteRsp_t *pRsp = &pPkt->msg.prepareWriteRsp;

          attHdrLen = ATT_PREPARE_WRITE_RSP_FIXED_SIZE;

          // Copy request header over
          msgLen = ATT_BuildPrepareWriteRsp(&pOutMsg[hdrLen], (uint8_t *)pRsp) - attHdrLen;
          pPayload = pRsp->pValue;
        }
        break;

      case ATT_EXECUTE_WRITE_REQ:
        attHdrLen = ATT_BuildExecuteWriteReq(&pOutMsg[hdrLen], (uint8_t *)&(pPkt->msg.executeWriteReq));
        break;

      case ATT_HANDLE_VALUE_IND:
      case ATT_HANDLE_VALUE_NOTI:
        {
          attHandleValueInd_t *pInd = &pPkt->msg.handleValueInd;

          attHdrLen = ATT_HANDLE_VALUE_IND_FIXED_SIZE;

          // Copy request header over
          msgLen = ATT_BuildHandleValueInd(&pOutMsg[hdrLen], (uint8_t *)pInd) - attHdrLen;
          pPayload = pInd->pValue;
        }
        break;

      case ATT_FLOW_CTRL_VIOLATED_EVENT:
        {
          attFlowCtrlViolatedEvt_t *pEvt = &pPkt->msg.flowCtrlEvt;

          pOutMsg[hdrLen]   = pEvt->opcode;
          pOutMsg[hdrLen+1] = pEvt->pendingOpcode;

          attHdrLen = 2;
        }
        break;

      default:
        // Unknown command
        break;
    }
  }

  // Event format: HCI Ext hdr + event len + ATT hdr + ATT PDU
  if ((hdrLen + attHdrLen + msgLen) > HCI_EXT_APP_OUT_BUF)
  {
    pBuf = ICall_malloc(hdrLen + attHdrLen + msgLen);
    if (pBuf)
    {
      *pAllocated = TRUE;

      // Copy the ATT header over
      if (attHdrLen > 0)
      {
        VOID memcpy(&pBuf[hdrLen], &pOutMsg[hdrLen], attHdrLen);
      }
    }
    else
    {
      pBuf = pOutMsg;
      msgLen = 0;

      status = bleMemAllocError;
    }
  }
  else
  {
    pBuf = pOutMsg;
  }

  // Build the message PDU
  if (pPayload != NULL)
  {
    if (msgLen > 0)
    {
      // Copy the message payload over
      VOID memcpy(&pBuf[hdrLen+attHdrLen], pPayload, msgLen);
    }

    // Free the payload buffer
    BM_free(pPayload);
  }

  // Build the message header
  VOID buildHCIExtHeader(pBuf, (HCI_EXT_ATT_EVENT | pPkt->method), status, pPkt->connHandle);

  // Add the event (PDU) length for GATT events for now!
  pBuf[HCI_EXT_HDR_LEN] = attHdrLen + msgLen;

  *pMsgLen = hdrLen + attHdrLen + msgLen;

  return(pBuf);
}

#ifdef BLE3_CMD
/*********************************************************************
 * @fn      BLE3ToAgama_setParam
 *
 * @brief   Translating BLE3 param ID to a set of Agama commands
 *
 * @param   id - Parameter ID
 * @param   value - Parameter value
 *
 * @return  SUCCESS, bleInvalidRange, INVALIDPARAMETER.
 */
status_t BLE3ToAgama_setParam( uint16_t id, uint16_t value )
{
  uint8_t stat = SUCCESS;
  switch(id)
  {
#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
    // Scan parameters
    case TGAP_CONN_SCAN_INT:
    case TGAP_CONN_HIGH_SCAN_INT:
    case TGAP_CONN_EST_SCAN_INT:
    {
      // Update the scan interval for the establish connection scanning
      stat = GapInit_setPhyParam(SCAN_PRIM_PHY_1M, INIT_PHYPARAM_SCAN_INTERVAL, value);
      break;
    }

    case TGAP_GEN_DISC_SCAN_INT:
    case TGAP_LIM_DISC_SCAN_INT:
    {
      uint16_t scanWin;
      uint16_t scanInt;
      GapScan_ScanType_t scanType;
      stat = GapScan_getPhyParams(SCAN_PRIM_PHY_1M, &scanType, &scanInt, &scanWin);
      if( stat == SUCCESS )
      {
        // Scan interval update
        stat = GapScan_setPhyParams(SCAN_PRIM_PHY_1M, scanType, value, scanWin);
      }
      break;
    }

    case TGAP_CONN_SCAN_WIND:
    case TGAP_CONN_EST_SCAN_WIND:
    case TGAP_CONN_HIGH_SCAN_WIND:
    {
      // Update the scan window for the establish connection scanning
      stat = GapInit_setPhyParam(SCAN_PRIM_PHY_1M, INIT_PHYPARAM_SCAN_WINDOW, value);
      break;
    }

    case TGAP_GEN_DISC_SCAN_WIND:
    case TGAP_LIM_DISC_SCAN_WIND:
    {
      uint16_t scanWin;
      uint16_t scanInt;
      GapScan_ScanType_t scanType;
      stat = GapScan_getPhyParams(SCAN_PRIM_PHY_1M, &scanType, &scanInt, &scanWin);
      if( stat == SUCCESS )
      {
        // Scan window update
        stat = GapScan_setPhyParams(SCAN_PRIM_PHY_1M, scanType, scanInt, value);
      }
      break;
    }

    // Scan duration update
    case TGAP_GEN_DISC_SCAN:
    case TGAP_LIM_DISC_SCAN:
    {
      scanDuration = value/10;
      break;
    }

    // Scan channels update
    case TGAP_SET_SCAN_CHAN:
    {
      stat = GAP_SetParamValue(GAP_PARAM_SET_SCAN_CHAN, value);
      break;
    }

    case TGAP_FILTER_ADV_REPORTS:
    {
      uint8_t fltValue = (uint8_t)value;
      stat = GapScan_setParam(SCAN_PARAM_FLT_DUP, &fltValue);
      break;
    }

    case TGAP_SCAN_RSP_RSSI_MIN:
    {
      int8_t rssiValue = (int8_t)value;
      stat = GapScan_setParam(SCAN_PARAM_FLT_MIN_RSSI, &rssiValue);
      break;
    }

    case TGAP_CONN_EST_INT_MIN:
    {
      stat = GapInit_setPhyParam(INIT_PHY_1M, INIT_PHYPARAM_CONN_INT_MIN, value);
      break;
    }

    case TGAP_CONN_EST_INT_MAX:
    {
      stat = GapInit_setPhyParam(INIT_PHY_1M, INIT_PHYPARAM_CONN_INT_MAX, value);
      break;
    }

        case TGAP_CONN_EST_SUPERV_TIMEOUT:
    {
      stat = GapInit_setPhyParam(INIT_PHY_1M, INIT_PHYPARAM_SUP_TIMEOUT, value);
      break;
    }

    case TGAP_CONN_EST_LATENCY:
    {
      stat = GapInit_setPhyParam(INIT_PHY_1M, INIT_PHYPARAM_CONN_LATENCY, value);
      break;
    }

    case TGAP_CONN_EST_MIN_CE_LEN:
    {
      stat = GapInit_setPhyParam(INIT_PHY_1M, INIT_PHYPARAM_MIN_CE_LEN, value);
      break;
    }

    case TGAP_CONN_EST_MAX_CE_LEN:
    {
      stat = GapInit_setPhyParam(INIT_PHY_1M, INIT_PHYPARAM_MAX_CE_LEN, value);
      break;
    }
#endif

#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )
    // Advertisement parameters
    // Update the minimum advertising interval
    case TGAP_GEN_DISC_ADV_INT_MIN:
    case TGAP_CONN_ADV_INT_MIN:
    case TGAP_FAST_INTERVAL_2_INT_MIN:
    case TGAP_LIM_DISC_ADV_INT_MIN:
    {
      if(advHandleLegacy == 0xFF)
      {
        GapAdv_params_t advParamLegacy = GAPADV_PARAMS_LEGACY_SCANN_CONN;
        stat = GapAdv_create(host_tl_advEvtCallback, &advParamLegacy, &advHandleLegacy);
      }

      if( stat != SUCCESS )
      {
        // Failed to create a valid advertising set
        return stat;
      }

      // There is a valid advertising set
      uint32_t advIntMin = (uint32_t)value;
      stat = GapAdv_setParam(advHandleLegacy, GAP_ADV_PARAM_PRIMARY_INTERVAL_MIN, &advIntMin);
      break;
    }

    // Update the maximum advertising interval
    case TGAP_LIM_DISC_ADV_INT_MAX:
    case TGAP_GEN_DISC_ADV_INT_MAX:
    case TGAP_CONN_ADV_INT_MAX:
    case TGAP_FAST_INTERVAL_2_INT_MAX:
    {
      if(advHandleLegacy == 0xFF)
      {
        GapAdv_params_t advParamLegacy = GAPADV_PARAMS_LEGACY_SCANN_CONN;
        stat = GapAdv_create(host_tl_advEvtCallback, &advParamLegacy, &advHandleLegacy);
      }

      if( stat != SUCCESS )
      {
        // Failed to create a valid advertising set
        return stat;
      }

      // There is a valid advertising set
      uint32_t advIntMax = (uint32_t)value;
      stat = GapAdv_setParam(advHandleLegacy, GAP_ADV_PARAM_PRIMARY_INTERVAL_MAX, &advIntMax);
      break;
    }

    case TGAP_GEN_DISC_ADV_MIN:
    case TGAP_LIM_ADV_TIMEOUT:
    {
      advDuration = value;
      break;
    }
#endif

    case TGAP_CONN_PARAM_TIMEOUT:
    {
      stat = GAP_SetParamValue(GAP_PARAM_CONN_PARAM_TIMEOUT, value);
      break;
    }

    case TGAP_REJECT_CONN_PARAMS:
    {
      stat = GAP_SetParamValue(GAP_PARAM_LINK_UPDATE_DECISION, value);
      break;
    }

    case TGAP_PRIVATE_ADDR_INT:
    {
      stat = GAP_SetParamValue(GAP_PARAM_PRIVATE_ADDR_INT, value);
      break;
    }

    case TGAP_SM_TIMEOUT:
    {
      stat = GAP_SetParamValue(GAP_PARAM_SM_TIMEOUT, value);
      break;
    }

    case TGAP_SM_MIN_KEY_LEN:
    {
      stat = GAP_SetParamValue(GAP_PARAM_SM_MIN_KEY_LEN, value);
      break;
    }

    case TGAP_SM_MAX_KEY_LEN:
    {
      stat = GAP_SetParamValue(GAP_PARAM_SM_MAX_KEY_LEN, value);
      break;
    }

    default:
      stat = SUCCESS;
      break;
  }

  return stat;
}

/*********************************************************************
 * @fn      getBLE3ToAgamaEventProp
 *
 * @brief   Translating BLE3 advertising properties to Agama advertising
 *          properties
 *
 * @param   eventType - BLE3 advertising properties
 *
 * @return  0xFFFF if the event properties wasn't found, else Agama
 *          event properties
 */
uint16_t getBLE3ToAgamaEventProp( uint8_t eventType)
{
  uint16_t eventProp = 0xFFFF;

  if( eventType == GAP_ADTYPE_ADV_IND )
  {
    eventProp = GAP_ADV_PROP_LEGACY | GAP_ADV_PROP_CONNECTABLE | GAP_ADV_PROP_SCANNABLE;
  }

  if( eventType == GAP_ADTYPE_ADV_HDC_DIRECT_IND )
  {
    eventProp = GAP_ADV_PROP_LEGACY | GAP_ADV_PROP_CONNECTABLE | GAP_ADV_PROP_DIRECTED | GAP_ADV_PROP_HDC;
  }

  if( eventType == GAP_ADTYPE_ADV_SCAN_IND )
  {
    eventProp = GAP_ADV_PROP_LEGACY | GAP_ADV_PROP_SCANNABLE;
  }

  if( eventType == GAP_ADTYPE_ADV_NONCONN_IND )
  {
    eventProp = GAP_ADV_PROP_LEGACY;
  }

  if( eventType == GAP_ADTYPE_ADV_LDC_DIRECT_IND )
  {
    eventProp = GAP_ADV_PROP_LEGACY | GAP_ADV_PROP_CONNECTABLE | GAP_ADV_PROP_DIRECTED;
  }

  return eventProp;
}

/*********************************************************************
 * @fn      getAgamaToBLE3EventProp
 *
 * @brief   Translating Agama advertising properties to BLE3 advertising
 *          properties
 *
 * @param   eventType - Agama advertising properties
 *
 * @return  0xFFFF if the event properties wasn't found, else BLE3
 *          event properties
 */
uint8_t getAgamaToBLE3EventProp( uint8_t eventType )
{
  uint8_t eventProp = 0xFF;

  if( eventType == GAP_AGAMA_BLE3_TYPE_ADV_IND )
  {
    eventProp = GAP_ADTYPE_ADV_IND;
  }

  if( eventType == GAP_AGAMA_BLE3_TYPE_ADV_HDC_DIRECT_IND )
  {
   eventProp = GAP_ADTYPE_ADV_HDC_DIRECT_IND;
  }

  if( eventType == GAP_AGAMA_BLE3_TYPE_ADV_SCAN_IND )
  {
   eventProp = GAP_ADTYPE_ADV_SCAN_IND;
  }

  if( eventType == GAP_AGAMA_BLE3_TYPE_ADV_NONCONN_IND )
  {
   eventProp = GAP_ADTYPE_ADV_NONCONN_IND;
  }

  if( eventType == GAP_AGAMA_BLE3_TYPE_ADV_LDC_DIRECT_IND )
  {
   eventProp = GAP_ADTYPE_ADV_LDC_DIRECT_IND;
  }

  return eventProp;
}

#endif // BLE3_CMD

#if !defined(GATT_DB_OFF_CHIP)
/*********************************************************************
 * @fn      processEventsGATTServ
 *
 * @brief   Process an incoming GATT Server Event messages.
 *
 * @param   pPkt - packet to process
 * @param   pMsg - outgoing message to be built
 * @param   pMsgLen - length of outgoing message
 *
 * @return  outgoing message
 */
static uint8_t *processEventsGATTServ(gattEventHdr_t *pPkt, uint8_t *pMsg, uint8_t *pMsgLen)
{
  uint8_t hdrLen = HCI_EXT_HDR_LEN + 1; // hdr + event length
  uint8_t msgLen = 0;
  uint8_t *pBuf = pMsg;

  if (pPkt->hdr.status == SUCCESS)
  {
    // Build the message first
    switch (pPkt->method)
    {
      case GATT_CLIENT_CHAR_CFG_UPDATED_EVENT:
        {
          gattClientCharCfgUpdatedEvent_t *pEvent = (gattClientCharCfgUpdatedEvent_t *)pPkt;

          // Attribute handle
          pMsg[hdrLen]   = LO_UINT16(pEvent->attrHandle);
          pMsg[hdrLen+1] = HI_UINT16(pEvent->attrHandle);

          // Attribute value
          pMsg[hdrLen+2] = LO_UINT16(pEvent->value);
          pMsg[hdrLen+3] = HI_UINT16(pEvent->value);

          msgLen = 4;
        }
        break;

      default:
        // Unknown command
        break;
    }
  }

  // Build the message header
  VOID buildHCIExtHeader(pBuf, (HCI_EXT_GATT_EVENT | pPkt->method), pPkt->hdr.status, pPkt->connHandle);

  // Add the event (PDU) length for GATT events for now!
  pBuf[HCI_EXT_HDR_LEN] = msgLen;

  *pMsgLen = hdrLen + msgLen;

  return(pBuf);
}
#endif // !GATT_DB_OFF_CHIP

/*********************************************************************
 * @fn      buildHCIExtHeader
 *
 * @brief   Build an HCI Extension header.
 *
 * @param   pBuf - header to be built
 * @param   event - event id
 * @param   status - event status
 * @param   connHandle - connection handle
 *
 * @return  header length
 */
static uint8_t buildHCIExtHeader(uint8_t *pBuf, uint16_t event, uint8_t status, uint16_t connHandle)
{
  pBuf[0] = LO_UINT16(event);
  pBuf[1] = HI_UINT16(event);
  pBuf[2] = status;
  pBuf[3] = LO_UINT16(connHandle);
  pBuf[4] = HI_UINT16(connHandle);

  return(HCI_EXT_HDR_LEN);
}
#endif /* (CENTRAL_CFG | PERIPHERAL_CFG)*/

#endif /* (PTM_MODE) */

#if defined(BLE_V41_FEATURES) && (BLE_V41_FEATURES & L2CAP_COC_CFG)
/*********************************************************************
  * @fn      l2capVerifySecCB
 *
 * @brief   Callback function to verify security when a Connection
 *          Request is received.
 *
 * @param   connHandle - connection handle request was received on
 * @param   id - identifier matches responses with requests
 * @param   pReq - received connection request
 *
 * @return  See L2CAP Connection Response: Result values
 */
static uint16_t l2capVerifySecCB(uint16_t connHandle, uint8_t id, l2capConnectReq_t *pReq)
{
  uint8_t msgLen = 0;

  // Build the message header first
  msgLen = buildHCIExtHeader(out_msg, (HCI_EXT_L2CAP_EVENT | L2CAP_CONNECT_IND),
                              SUCCESS, connHandle);

  out_msg[msgLen++] = id;
  msgLen += L2CAP_BuildConnectReq(&out_msg[msgLen], (uint8_t *)pReq);

  // Send out the Connection Request
  HCI_SendControllerToHostEvent(HCI_VE_EVENT_CODE,  msgLen, out_msg);

  return(L2CAP_CONN_PENDING_SEC_VERIFY);
}

/*********************************************************************
 * @fn      buildCoChannelInfo
 *
 * @brief   Build Connection Oriented Channel info.
 *
 * @param   CID - local CID
 * @param   pInfo - pointer to CoC info
 * @param   pRspBuf - buffer to copy CoC info into
 *
 * @return  length of data copied
 */
static uint8_t buildCoChannelInfo(uint16_t CID, l2capCoCInfo_t *pInfo, uint8_t *pRspBuf)
{
  uint8_t msgLen = 0;

  pRspBuf[msgLen++] = LO_UINT16(pInfo->psm);
  pRspBuf[msgLen++] = HI_UINT16(pInfo->psm);

  pRspBuf[msgLen++] = LO_UINT16(CID);
  pRspBuf[msgLen++] = HI_UINT16(CID);

  pRspBuf[msgLen++] = LO_UINT16(pInfo->mtu);
  pRspBuf[msgLen++] = HI_UINT16(pInfo->mtu);

  pRspBuf[msgLen++] = LO_UINT16(pInfo->mps);
  pRspBuf[msgLen++] = HI_UINT16(pInfo->mps);

  pRspBuf[msgLen++] = LO_UINT16(pInfo->credits);
  pRspBuf[msgLen++] = HI_UINT16(pInfo->credits);

  pRspBuf[msgLen++] = LO_UINT16(pInfo->peerCID);
  pRspBuf[msgLen++] = HI_UINT16(pInfo->peerCID);

  pRspBuf[msgLen++] = LO_UINT16(pInfo->peerMtu);
  pRspBuf[msgLen++] = HI_UINT16(pInfo->peerMtu);

  pRspBuf[msgLen++] = LO_UINT16(pInfo->peerMps);
  pRspBuf[msgLen++] = HI_UINT16(pInfo->peerMps);

  pRspBuf[msgLen++] = LO_UINT16(pInfo->peerCredits);
  pRspBuf[msgLen++] = HI_UINT16(pInfo->peerCredits);

  pRspBuf[msgLen++] = LO_UINT16(pInfo->peerCreditThreshold);
  pRspBuf[msgLen++] = HI_UINT16(pInfo->peerCreditThreshold);

  return(msgLen);
}

#endif //(BLE_V41_FEATURES & L2CAP_COC_CFG)

#if defined(GATT_DB_OFF_CHIP)
/*********************************************************************
 * @fn      addAttrRec
 *
 * @brief   Add attribute record to its service.
 *
 * @param   pServ - GATT service
 * @param   pUUID - attribute UUID
 * @param   len - length of UUID
 * @param   permissions - attribute permissions
 * @param   pTotalAttrs - total number of attributes
 * @param   pRspDataLen - response data length to be returned
 *
 * @return  status
 */
static uint8_t addAttrRec(gattService_t *pServ, uint8_t *pUUID, uint8_t len,
                         uint8_t permissions, uint16_t *pTotalAttrs, uint8_t *pRspDataLen)
{
  gattAttribute_t *pAttr = &(pServ->attrs[pServ->numAttrs]);
  uint8_t stat = SUCCESS;

  // Set up attribute record
  pAttr->type.uuid = findUUIDRec(pUUID, len);
  if (pAttr->type.uuid != NULL)
  {
    pAttr->type.len = len;
    pAttr->permissions = permissions;

    // Are all attributes added to the service yet?
    if (++pServ->numAttrs == *pTotalAttrs)
    {
      // Register the service with the GATT Server
      stat = GATT_RegisterService(pServ);
      if (stat == SUCCESS)
      {
        *pRspDataLen = 4;

        // Service startHandle
        uint16_t handle = pServ->attrs[0].handle;
        rspBuf[RSP_PAYLOAD_IDX] = LO_UINT16(handle);
        rspBuf[RSP_PAYLOAD_IDX+1] = HI_UINT16(handle);

        // Service endHandle
        handle = pServ->attrs[pServ->numAttrs-1].handle;
        rspBuf[RSP_PAYLOAD_IDX+2] = LO_UINT16(handle);
        rspBuf[RSP_PAYLOAD_IDX+3] = HI_UINT16(handle);

        // Service is registered with GATT; clear its info
        pServ->attrs = NULL;
        pServ->numAttrs = 0;
      }
      else
      {
        freeAttrRecs(pServ);
      }

      // We're done with this service
      *pTotalAttrs = 0;
    }
  }
  else
  {
    stat = INVALIDPARAMETER;
  }

  return(stat);
}

/*********************************************************************
 * @fn      freeAttrRecs
 *
 * @brief   Free attribute records. Also, free UUIDs that were
 *          allocated dynamically.
 *
 * @param   pServ - GATT service
 *
 * @return  none
 */
static void freeAttrRecs(gattService_t *pServ)
{
  if (pServ->attrs != NULL)
  {
    for (uint8_t i = 0; i < pServ->numAttrs; i++)
    {
      gattAttrType_t *pType = &pServ->attrs[i].type;
      if (pType->uuid != NULL)
      {
        if ((uint8_t*)GATT_FindUUIDRec((uint8_t *)pType->uuid, pType->len) == NULL)
        {
          // UUID was dynamically allocated; free it
          ICall_free((uint8_t *)pType->uuid);
        }
      }
    }

    ICall_free(pServ->attrs);

    pServ->attrs = NULL;
    pServ->numAttrs = 0;
  }
}

/*********************************************************************
 * @fn      findUUIDRec
 *
 * @brief   Find UUID record. If the record is not found, create one
 *          dynamically.
 *
 * @param   pUUID - UUID to look for
 * @param   len - length of UUID
 *
 * @return  UUID record
 */
static const uint8_t *findUUIDRec(uint8_t *pUUID, uint8_t len)
{
  const uint8_t *pUuid = (uint8_t*) GATT_FindUUIDRec(pUUID, len);
  if (pUuid == NULL)
  {
    // UUID not found; allocate space for it
    pUuid = ICall_malloc(len);
    if (pUuid != NULL)
    {
      VOID memcpy((uint8_t *)pUuid, pUUID, len);
    }
    else
    {
#ifdef EXT_HAL_ASSERT
      HAL_ASSERT(HAL_ASSERT_CAUSE_OUT_OF_MEMORY);
#else /* !EXT_HAL_ASSERT */
      HAL_ASSERT(FALSE);
#endif /* EXT_HAL_ASSERT */
    }
  }

  return(pUuid);
}
#endif // GATT_DB_OFF_CHIP
#endif // HOST_CONFIG
#endif // #endif /* (defined(HCI_TL_FULL) || defined(PTM_MODE)) */

/*********************************************************************
 * @fn      HCI_TL_create_conn
 *
 * @brief   Detailed description is provided in the declaration of the function
 *          in icall_hci_tl.h.
 */
llStatus_t HCI_TL_create_conn(uint16_t opcode, uint8_t* pHciParams)
{
    static aeCreateConnCmd_t HCI_TL_createConnParam;
    llStatus_t status;

    // Check if a legacy/extended command mixing is allowed
    if(OPT_checkLegacyHCICmdStatus(opcode))
    {
       status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
    }
    else
    {
        // Translate this legacy API to a call to the new AE API.
        HCI_TL_createConnParam.initFilterPolicy = pHciParams[4];
        HCI_TL_createConnParam.ownAddrType      = pHciParams[12];
        HCI_TL_createConnParam.peerAddrType     = pHciParams[5];
        memcpy(HCI_TL_createConnParam.peerAddr, &pHciParams[6], B_ADDR_LEN) ;
        HCI_TL_createConnParam.initPhys         = LL_PHY_1_MBPS;  //Forced
        HCI_TL_createConnParam.extInitParam[0].scanInterval = BUILD_UINT16(pHciParams[0], pHciParams[1]);
        HCI_TL_createConnParam.extInitParam[0].scanWindow   = BUILD_UINT16(pHciParams[2], pHciParams[3]);
        HCI_TL_createConnParam.extInitParam[0].connIntMin   = BUILD_UINT16(pHciParams[13], pHciParams[14]);
        HCI_TL_createConnParam.extInitParam[0].connIntMax   = BUILD_UINT16(pHciParams[15], pHciParams[16]);
        HCI_TL_createConnParam.extInitParam[0].connLatency  = BUILD_UINT16(pHciParams[17], pHciParams[18]);
        HCI_TL_createConnParam.extInitParam[0].connTimeout  = BUILD_UINT16(pHciParams[19], pHciParams[20]);
        HCI_TL_createConnParam.extInitParam[0].minLength    = BUILD_UINT16(pHciParams[21], pHciParams[22]);
        HCI_TL_createConnParam.extInitParam[0].maxLength    = BUILD_UINT16(pHciParams[23], pHciParams[24]);

        // Check that if random own address type configure as random identity,
        // random address configured before.
        if ( isValidRandomAddressForCreateConn(HCI_TL_createConnParam) == FALSE )
        {
            status = LL_STATUS_ERROR_BAD_PARAMETER;
        }
        else
        {
            status = LE_ExtCreateConn(&HCI_TL_createConnParam);
        }

     }
     return status;
}

/*********************************************************************
 * @fn      HCI_TL_set_scan_param
 *
 * @brief   Detailed description is provided in the declaration of the function
 *          in icall_hci_tl.h.
 */
llStatus_t HCI_TL_set_scan_param(uint16_t opcode, uint8_t* pHciParams, aeSetScanParamCmd_t*  hci_tl_cmdScanParams)
{
    llStatus_t status;

    // Check if a legacy/extended command mixing is allowed
    if(OPT_checkLegacyHCICmdStatus(opcode))
    {
        status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
    }
    else
    {
        // Translate this legacy API to a call to the new AE API.
        hci_tl_cmdScanParams->ownAddrType                  = pHciParams[5];
        hci_tl_cmdScanParams->scanFilterPolicy             = pHciParams[6];
        hci_tl_cmdScanParams->scanPhys                     = LL_PHY_1_MBPS;  //1mbps
        hci_tl_cmdScanParams->extScanParam[0].scanType     = pHciParams[0];
        hci_tl_cmdScanParams->extScanParam[0].scanInterval = BUILD_UINT16(pHciParams[1], pHciParams[2]);
        hci_tl_cmdScanParams->extScanParam[0].scanWindow   = BUILD_UINT16(pHciParams[3], pHciParams[4]);

        status = LE_SetExtScanParams(hci_tl_cmdScanParams);
    }
    return ( status );
}

/*********************************************************************
 * @fn      HCI_TL_set_ext_scan_param
 *
 * @brief   Detailed description is provided in the declaration of the function
 *          in icall_hci_tl.h.
 */
llStatus_t HCI_TL_set_ext_scan_param(uint16_t opcode, uint8_t* pHciParams, aeSetScanParamCmd_t*  hci_tl_cmdScanParams)
{
    // This information needs to be saved on behalf of the LL.
    // A linked List will be created to save this information.
    llStatus_t status;

    // Check if a legacy/extended command mixing is allowed
    if(OPT_checkLegacyHCICmdStatus(opcode))
    {
        status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
        return ( status );
    }

    // If set already exist, the parameters will be updated,
    // even if the call to  LE_SetExtAdvParams fails
    *hci_tl_cmdScanParams = *((aeSetScanParamCmd_t *)pHciParams);

    status = LE_SetExtScanParams(hci_tl_cmdScanParams);
    return ( status );
}

/*********************************************************************
 * @fn      HCI_TL_set_ext_scan_enable
 *
 * @brief   Detailed description is provided in the declaration of the function
 *          in icall_hci_tl.h.
 */
llStatus_t HCI_TL_set_ext_scan_enable(uint16_t opcode, uint8_t* pHciParams, aeEnableScanCmd_t* hci_tl_cmdScanEnable, aeSetScanParamCmd_t* hci_tl_cmdScanParams)
{
    llStatus_t status;

    // Check if a legacy/extended command mixing is allowed
    if(OPT_checkLegacyHCICmdStatus(opcode))
    {
        status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
        return ( status );
    }

    // Need to register the callback Function.
    LL_AE_RegCBack(LL_CBACK_EXT_ADV_REPORT,        (void *)MAP_HCI_AeScanCback);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_TIMEOUT,      (void *)MAP_HCI_AeScanCback);
#ifdef ENABLE_SCAN_CALLBACKS
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_START,        (void *)MAP_HCI_AeScanCback);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_END,          (void *)MAP_HCI_AeScanCback);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_WINDOW_END,   (void *)MAP_HCI_AeScanCback);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_INTERVAL_END, (void *)MAP_HCI_AeScanCback);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_DURATION_END, (void *)MAP_HCI_AeScanCback);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_PERIOD_END,   (void *)MAP_HCI_AeScanCback);
#endif // ENABLE_SCAN_CALLBACKS

    *hci_tl_cmdScanEnable = *((aeEnableScanCmd_t *)pHciParams);

    // Check if HCI_LE_SetRandomAddressCmd() function not called before
    // while the own address type is random.
    if ( hci_tl_isValidRandomAddressForScan(hci_tl_cmdScanParams) == FALSE )
    {
        status = LL_STATUS_ERROR_BAD_PARAMETER;
        return ( status );
    }

    // Check if the scan parameters were previously configured
    if (extScanInfo->pScanParam == NULL)
    {
        // Init to default scan params in case it was never configured before to enable to scan
        hci_tl_setDefaultScanParams(hci_tl_cmdScanParams);

        // Set the parameters
        LE_SetExtScanParams(hci_tl_cmdScanParams);
    }

    status = LE_SetExtScanEnable(hci_tl_cmdScanEnable);
    return ( status );
}

/*********************************************************************
 * @fn      HCI_TL_set_scan_enable
 *
 * @brief   Detailed description is provided in the declaration of the function
 *          in icall_hci_tl.h.
 */
llStatus_t HCI_TL_set_scan_enable(uint16_t opcode, uint8_t* pHciParams, aeEnableScanCmd_t* hci_tl_cmdScanEnable, aeSetScanParamCmd_t* hci_tl_cmdScanParams)
{
    llStatus_t status;
    // Check if a legacy/extended command mixing is allowed
    if(OPT_checkLegacyHCICmdStatus(opcode))
    {
        status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
        return ( status );
    }

    // Translate this legacy API to a call to the new AE API.
    // Need to register the callback Function.
    LL_AE_RegCBack(LL_CBACK_EXT_ADV_REPORT,        (void *)hci_tl_legacyScanCback);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_TIMEOUT,      (void *)NULL);
#ifdef ENABLE_SCAN_CALLBACKS
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_START,        (void *)NULL);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_END,          (void *)NULL);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_WINDOW_END,   (void *)NULL);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_INTERVAL_END, (void *)NULL);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_DURATION_END, (void *)NULL);
    LL_AE_RegCBack(LL_CBACK_EXT_SCAN_PERIOD_END,   (void *)NULL);
#endif // ENABLE_SCAN_CALLBACKS

    // Check if the scan parameters were previously configured
    if (extScanInfo->pScanParam == NULL)
    {
        // Init to default scan params in case it was never configured before to enable to scan
        hci_tl_setDefaultScanParams(hci_tl_cmdScanParams);

        // Set the parameters
        LE_SetExtScanParams(hci_tl_cmdScanParams);
    }

    // Copy the scan enable parameters
    hci_tl_cmdScanEnable->enable       = pHciParams[0];
    hci_tl_cmdScanEnable->dupFiltering = pHciParams[1];
    hci_tl_cmdScanEnable->duration     = 0;
    hci_tl_cmdScanEnable->period       = 0;

    // Check if HCI_LE_SetRandomAddressCmd() function not called before
    // while the own address type is random.
    if ( hci_tl_isValidRandomAddressForScan(hci_tl_cmdScanParams) == FALSE )
    {
        status = LL_STATUS_ERROR_BAD_PARAMETER;
        return ( status );
    }

    status = LE_SetExtScanEnable(hci_tl_cmdScanEnable);
    return ( status );
}

/*********************************************************************
 * @fn      HCI_TL_ext_create_conn
 *
 * @brief   Detailed description is provided in the declaration of the function
 *          in icall_hci_tl.h.
 */
llStatus_t HCI_TL_ext_create_conn(uint16_t opcode, uint8_t* pHciParams)
{
    llStatus_t status;
    static aeCreateConnCmd_t HCI_TL_createConnParam;

    // Check if a legacy/extended command mixing is allowed
    if(OPT_checkLegacyHCICmdStatus(opcode))
    {
        status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
    }
    else
    {
        HCI_TL_createConnParam = *((aeCreateConnCmd_t *)pHciParams);

        // Check that if random own address type configure as random identity,
        // random address configured before.
        if ( isValidRandomAddressForCreateConn(HCI_TL_createConnParam) == FALSE )
        {
            status = LL_STATUS_ERROR_BAD_PARAMETER;
        }
        else
        {
            status = LE_ExtCreateConn(&HCI_TL_createConnParam);
        }
    }

    return status;
}

/*********************************************************************
 * @fn      hci_tl_setDefaultScanParams
 *
 * @brief   Init the scan params to default values.
 *
 * @param   aeSetScanParamCmd_t *pcmdScanParams
 *
 * @return  none.
 */
static void hci_tl_setDefaultScanParams (aeSetScanParamCmd_t *pcmdScanParams)
{
    pcmdScanParams->ownAddrType                  = LL_DEV_ADDR_TYPE_PUBLIC_ID;
    pcmdScanParams->scanFilterPolicy              = LL_SCAN_AL_POLICY_ANY_ADV_PKTS;
    pcmdScanParams->scanPhys                     = LL_PHY_1_MBPS;
    pcmdScanParams->extScanParam[0].scanType     = LL_SCAN_ACTIVE;
    pcmdScanParams->extScanParam[0].scanInterval = 800; //500ms
    pcmdScanParams->extScanParam[0].scanWindow   = 800; //500ms
}

/*******************************************************************************
 * check that if the own address type for scan is random, HCI_LE_SetRandomAddressCmd was called.
 * If the own address Type is not random return TRUE.
 */
uint8 hci_tl_isValidRandomAddressForScan (aeSetScanParamCmd_t *cmdScanParams)
{
  // If the own address Type is not random return TRUE
  if ((cmdScanParams->ownAddrType != LL_DEV_ADDR_TYPE_RANDOM) &&
      (cmdScanParams->ownAddrType != LL_DEV_ADDR_TYPE_RANDOM_ID))
  {
    return TRUE;
  }

  // else, own address type is random. Check if random address was configured.
  return LL_IsRandomAddressConfigured();
}

/*********************************************************************
 * @fn      hci_tl_legacyScanCback
 *
 * @brief   Callback for the legacy advertising report send by LL.
 *          This callback is executed in SWI context
 *
 * @param   extAdvRpt - data of the Advertising report.
 *
 * @return  none
 */
static void hci_tl_legacyScanCback(uint8_t event, aeExtAdvRptEvt_t *extAdvRpt)
{
  if (HCI_TL_CallbackEvtProcessCB)
  {
    // Decide what to do with the pointer based on Event.
    hci_tl_ScanEvtCallback_t * evtCallback;
    evtCallback = (hci_tl_ScanEvtCallback_t *) ICall_malloc(sizeof(hci_tl_ScanEvtCallback_t));
    if(evtCallback)
    {
      switch(event)
      {
        case LL_CBACK_EXT_ADV_REPORT:
        {
          evtCallback->pData = extAdvRpt;
          break;
        }
        case LL_CBACK_EXT_SCAN_TIMEOUT:
        case LL_CBACK_EXT_SCAN_START:
        case LL_CBACK_EXT_SCAN_PERIOD_END:
        case LL_CBACK_EXT_SCAN_END:
        case LL_CBACK_EXT_SCAN_WINDOW_END:
        case LL_CBACK_EXT_SCAN_INTERVAL_END:
        case LL_CBACK_EXT_SCAN_DURATION_END:
        default:
        {
          evtCallback->pData = NULL; // Value of handle
          break;
        }
      }
      evtCallback->event = event;
      if(!HCI_TL_CallbackEvtProcessCB( (void*) evtCallback, (void*) hci_tl_legacyScanEventCallbackProcess))
      {
        // Not enough Heap...
        if ( event == LL_CBACK_EXT_ADV_REPORT )
        {
          if (extAdvRpt->pData)
          {
            ICall_free(extAdvRpt->pData);
          }
          ICall_free(extAdvRpt);
        }
        ICall_free(evtCallback);
        HCI_TL_sendSystemReport(HCI_TL_ID, LL_STATUS_ERROR_OUT_OF_HEAP, HCI_LE_EVENT_CODE);
      }
    }
    else
    {
      if ( event == LL_CBACK_EXT_ADV_REPORT )
      {
        if (extAdvRpt->pData)
        {
          ICall_free(extAdvRpt->pData);
        }
        ICall_free(extAdvRpt);
      }
      HCI_TL_sendSystemReport(HCI_TL_ID, LL_STATUS_ERROR_OUT_OF_HEAP, HCI_LE_EVENT_CODE);
    }
  }
}

/*********************************************************************
 * @fn      hci_tl_legacyScanEventCallbackProcess
 *
 * @brief   Process the legacy advertising report send by LL.
 *          This function will send the advertising report (per core spec).
 *
 * @param   extAdvRpt - data of the Advertising report.
 *
 * @return  none
 */
extern uint8* hciAllocAndPrepHciLeEvtPkt( uint8 **pData, uint8 hciLeEvtType,
                                   uint8 hciPktLen );
static void hci_tl_legacyScanEventCallbackProcess(hci_tl_ScanEvtCallback_t *evtCallback)
{
  uint8_t dataLen;
  uint8_t status = LL_STATUS_SUCCESS;
  if (evtCallback != NULL)
  {
    uint8_t event = evtCallback->event;

    switch(event)
    {
      case LL_CBACK_EXT_ADV_REPORT:
      {
          // Translate the report to the legacy report
          aeExtAdvRptEvt_t *extAdvRpt;
          extAdvRpt = (aeExtAdvRptEvt_t*) evtCallback->pData;
          uint8 *pEvt;
          // Pointer to data inside pEvt, that pointer point next slot to be filled
          uint8 *pData;

          // DEBUG CODE.
          // Add a Filter on RSSI to avoid being flooded and doomed by
          // all the crazy IOT devices in the world.
          // Uncomment the following to only report Adv with RSSI high enough
          // so they match teh board that are less than 1m from each other.
          //if (extAdvRpt->rssi < -37)
          //{
          //  // Reject this report...
          //  //Free the message and the payload
          //
          //}
          //else
          // END of Debug Code

          if ((extAdvRpt != NULL) && (extAdvRpt->subCode == HCI_BLE_EXTENDED_ADV_REPORT_EVENT))
          {
#ifdef QUAL_TEST
              if (extAdvRpt->directAddrType == AE_EXT_ADV_RPT_DIR_ADDR_TYPE_UNRESOLVED_RPA)
              {
                  // Got the Report, Map it to the LEGACY Directed Report Event...
                  pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                                     HCI_BLE_DIRECT_ADVERTISING_REPORT_EVENT,
                                                     HCI_ADV_DIRECTED_REPORT_EVENT_LEN);

                  if(pEvt)
                  {
                    *pData++ = extAdvRpt->numRpts;
                    *pData++ = 1; //Connectable directed legacy advertising
                    *pData++  = extAdvRpt->addrType;
                    memcpy(pData, extAdvRpt->addr, B_ADDR_LEN);
                    pData += B_ADDR_LEN;
                    *pData++ = LL_DEV_ADDR_TYPE_RANDOM;
                    memcpy(pData, extAdvRpt->directAddr, B_ADDR_LEN);
                    pData += B_ADDR_LEN;
                    *pData++ = extAdvRpt->rssi;

                    HCI_SendEventToHost( pEvt );
                  }
                  else
                  {
                    HCI_TL_sendSystemReport(HCI_TL_ID, LL_STATUS_ERROR_OUT_OF_HEAP, HCI_BLE_EXTENDED_ADV_REPORT_EVENT);
                    HCI_TL_sendMemoryReport(0x3);
                    break;
                  }
              }
              else
#endif
              {
                  // check if LE Meta-Events are enabled and this event is enabled
                  if ( MAP_HCI_CheckEventMaskLe(LE_EVT_ADV_REPORT_BIT) == 0 )
                  {
                    // the event mask is not set for this event
                    break;
                  }

                   // Got the Report, Map it to the LEGACY Report Event...
                  dataLen = extAdvRpt->dataLen;

                  pEvt = hciAllocAndPrepHciLeEvtPkt( &pData,
                                                     HCI_BLE_ADV_REPORT_EVENT,
                                                     HCI_ADV_REPORT_EVENT_LEN + dataLen );

                  if(pEvt)
                  {
                    *pData++  = extAdvRpt->numRpts;
                    switch (extAdvRpt->evtType)
                    {
                      case AE_EXT_ADV_RPT_EVT_TYPE_ADV_IND:
                      {
                        *pData = 0;
                        break;
                      }
                      case AE_EXT_ADV_RPT_EVT_TYPE_DIRECT_IND:
                      {
                        *pData = 1;
                        break;
                      }
                      case AE_EXT_ADV_RPT_EVT_TYPE_SCAN_IND:
                      {
                        *pData = 2;
                        break;
                      }
                      case AE_EXT_ADV_RPT_EVT_TYPE_NONCONN_IND:
                      {
                        *pData = 3;
                        break;
                      }
                      case AE_EXT_ADV_RPT_EVT_TYPE_SCAN_RSP_ADV_IND:
                      case AE_EXT_ADV_RPT_EVT_TYPE_SCAN_RSP_ADV_SCAN_IND:
                      case AE_EXT_ADV_RPT_EVT_TYPE_SCAN_RSP:
                      {
                        *pData = 4;
                        break;
                      }
                      default:
                      {
                        // Ignore any other event type....
                        ICall_freeMsg(pEvt);
                        status = LL_STATUS_ERROR_UNEXPECTED_PARAMETER;
                      }
                    }
                    if (status == LL_STATUS_SUCCESS)
                    {
                      /* Increase the pData after evtType field */
                      pData++;

                      *pData++  = extAdvRpt->addrType;
                      memcpy(pData, extAdvRpt->addr, B_ADDR_LEN);
                      pData += B_ADDR_LEN;
                      *pData++ = dataLen;

                      if (dataLen)
                      {
                        memcpy( pData, extAdvRpt->pData, dataLen);
                        pData += dataLen;
                      }
                      *pData = extAdvRpt->rssi;

                      HCI_SendEventToHost( pEvt );
                    }
                  }
                  else
                  {
                    HCI_TL_sendSystemReport(HCI_TL_ID, LL_STATUS_ERROR_OUT_OF_HEAP, HCI_BLE_EXTENDED_ADV_REPORT_EVENT);
                    HCI_TL_sendMemoryReport(0x3);
                    break;
                  }
              }
          }
          else
          {
              HCI_TL_sendSystemReport(HCI_TL_ID, LL_STATUS_ERROR_UNKNOWN_ADV_EVT_TYPE, HCI_BLE_EXTENDED_ADV_REPORT_EVENT);
          }

        break;
      }
      default:
      {
        //Unknow Event, just free the message.
        HCI_TL_sendSystemReport(HCI_TL_ID, LL_STATUS_ERROR_UNKNOWN_ADV_EVT_TYPE, HCI_EXT_LE_SCAN_EVENT);
        break;
      }
    } // end Switch


    //Free the message and the payload
    if(event == LL_CBACK_EXT_ADV_REPORT)
    {
        // Translate the report to the legacy report
        aeExtAdvRptEvt_t *extAdvRpt;
        extAdvRpt = (aeExtAdvRptEvt_t*) evtCallback->pData;
        if (extAdvRpt != NULL)
        {
          if (extAdvRpt->pData != NULL)
          {
            ICall_free(extAdvRpt->pData);
          }
          ICall_free(extAdvRpt);
        }
    }
    ICall_free(evtCallback);
  }
}

/*********************************************************************
 * @fn      processAdvExtraHCICmd
 *
 * @brief   Process all ADV HCI command that was not found in the predefined table
 *
 * @param   pMsg -     HCI Msg packet
 * @return  retVal     HCI Status
 */
uint8_t processAdvExtraHCICmd(hciPacket_t *pMsg)
{
    uint8_t retVal = HCI_SUCCESS;
    uint8_t packetType;
    uint16_t cmdOpCode;
    uint8_t *param; // pointer into HCI command parameters.

    // Retrieve packetType
    packetType = pMsg->pData[0];

    // Retrieve opcode
    cmdOpCode = BUILD_UINT16( pMsg->pData[1], pMsg->pData[2] );

    // Retrieve pointer to parameter
    if ( packetType == HCI_EXTENDED_CMD_PACKET )
    {
      param = &pMsg->pData[5];
    }
    else
    {
      param = &pMsg->pData[4];
    }

    switch(cmdOpCode)
    {
        case HCI_LE_READ_MAX_ADV_DATA_LENGTH:
        {
            uint16_t value;
            uint8_t res[3];

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                value = 0;
                res[0] = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                res[1] = LO_UINT16(value);
                res[2] = HI_UINT16(value);
                HCI_CommandCompleteEvent(cmdOpCode,
                                          sizeof(res),
                                          &res);
                break;
            }

            value = LE_ReadMaxAdvDataLen();
            res[0] = LL_STATUS_SUCCESS;
            res[1] = LO_UINT16(value);
            res[2] = HI_UINT16(value);
            HCI_CommandCompleteEvent(cmdOpCode,
                                      sizeof(res),
                                      &res);
            break;
        }
        case HCI_LE_READ_NUM_SUPPORTED_ADV_SETS:
        {
            uint8_t value;
            uint8_t res[2];

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                value = 0;
                res[0] = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                res[1] = value;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(res),
                                         &res);
                break;
            }

            value = LE_ReadNumSupportedAdvSets();
            res[0] = LL_STATUS_SUCCESS;
            res[1] = value;
            HCI_CommandCompleteEvent(cmdOpCode,
                                     sizeof(res),
                                     &res);
            break;
        }
        case HCI_LE_SET_ADV_SET_RANDOM_ADDRESS:
        {
            llStatus_t status;
            hci_tl_advSet_t *pAdvSet;

            pAdvSet = hci_tl_GetAdvSet(param[0]);
            if (pAdvSet)
            {
                pAdvSet->setAddrCmd = *((aeRandAddrCmd_t *)param);

                status = LE_SetAdvSetRandAddr(&pAdvSet->setAddrCmd);

                if (status == LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER)
                {
                    hci_tl_RemoveAdvSet(param[0]);
                }
            }
            else
            {
                status = LL_STATUS_ERROR_OUT_OF_HEAP;
            }

            HCI_CommandCompleteEvent(cmdOpCode,
                                     sizeof(status),
                                     &status);
            break;
        }
        case HCI_LE_SET_ADV_PARAM:
        {
            // Translate this legacy API to a call to the new AE API.
            // This information needs to be saved on behalf of the LL.
            // A linked List will be created to save this information.
            llStatus_t status;
            hci_tl_advSet_t *pAdvSet;
            aeSetParamRtn_t retParams;

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }

            // 1st check if there is not already an existing adv set with the same handle.
            pAdvSet = hci_tl_GetAdvSet(ADV_LEGACY_SET_HANDLE);
            if (pAdvSet)
            {
                // If set already exist, the parameters will be updated,
                // even if the call to  LE_SetExtAdvParams fails
                pAdvSet->advCmdParams.handle = ADV_LEGACY_SET_HANDLE;
                switch (param[4])
                {
                  case LL_ADV_CONNECTABLE_UNDIRECTED_EVT :
                    pAdvSet->advCmdParams.eventProps[0]   = LO_UINT16(AE_EVT_TYPE_CONN_SCAN_ADV_IND);
                    pAdvSet->advCmdParams.eventProps[1]   = HI_UINT16(AE_EVT_TYPE_CONN_SCAN_ADV_IND);
                    break;
                  case LL_ADV_CONNECTABLE_HDC_DIRECTED_EVT :
                    pAdvSet->advCmdParams.eventProps[0]   = LO_UINT16(AE_EVT_TYPE_CONN_DIR_HDC_ADV_DIRECT_IND);
                    pAdvSet->advCmdParams.eventProps[1]   = HI_UINT16(AE_EVT_TYPE_CONN_DIR_HDC_ADV_DIRECT_IND);
                    break;
                  case LL_ADV_SCANNABLE_UNDIRECTED_EVT :
                    pAdvSet->advCmdParams.eventProps[0]   = LO_UINT16(AE_EVT_TYPE_SCAN_UNDIR_ADV_SCAN_IND);
                    pAdvSet->advCmdParams.eventProps[1]   = HI_UINT16(AE_EVT_TYPE_SCAN_UNDIR_ADV_SCAN_IND);
                    break;
                  case LL_ADV_NONCONNECTABLE_UNDIRECTED_EVT :
                    pAdvSet->advCmdParams.eventProps[0]   = LO_UINT16(AE_EVT_TYPE_NONCONN_NONSCAN_ADV_NONCONN_IND);
                    pAdvSet->advCmdParams.eventProps[1]   = HI_UINT16(AE_EVT_TYPE_NONCONN_NONSCAN_ADV_NONCONN_IND);
                    break;
                  case LL_ADV_CONNECTABLE_LDC_DIRECTED_EVT :
                    pAdvSet->advCmdParams.eventProps[0]   = LO_UINT16(AE_EVT_TYPE_CONN_DIR_LDC_ADV_DIRECT_IND);
                    pAdvSet->advCmdParams.eventProps[1]   = HI_UINT16(AE_EVT_TYPE_CONN_DIR_LDC_ADV_DIRECT_IND);
                    break;
                  default:
                    status = LL_STATUS_ERROR_BAD_PARAMETER;
                    HCI_CommandCompleteEvent(cmdOpCode,
                                             sizeof(status),
                                             &status);
                    break;
                }

                pAdvSet->advCmdParams.primIntMin[0]   = param[0];
                pAdvSet->advCmdParams.primIntMin[1]   = param[1];
                pAdvSet->advCmdParams.primIntMin[2]   = 0;
                pAdvSet->advCmdParams.primIntMax[0]   = param[2];
                pAdvSet->advCmdParams.primIntMax[1]   = param[3];
                pAdvSet->advCmdParams.primIntMax[2]   = 0;
                pAdvSet->advCmdParams.primChanMap     = param[13];
                pAdvSet->advCmdParams.ownAddrType     = param[5];
                pAdvSet->advCmdParams.peerAddrType    = param[6];
                memcpy(pAdvSet->advCmdParams.peerAddr, &param[7], B_ADDR_LEN) ;
                pAdvSet->advCmdParams.filterPolicy    = param[14];
                pAdvSet->advCmdParams.txPower         = 127;            // Forced
                pAdvSet->advCmdParams.primPhy         = AE_PHY_1_MBPS;  // Forced to 1mbps
                pAdvSet->advCmdParams.secMaxSkip      = 0;              // N/A
                pAdvSet->advCmdParams.secPhy          = AE_PHY_1_MBPS;  // N/A
                pAdvSet->advCmdParams.sid             = 0;              // N/A
                pAdvSet->advCmdParams.notifyEnableFlags = 0;         // N/A

                status = LE_SetExtAdvParams(&pAdvSet->advCmdParams, &retParams);
                if (status != LL_STATUS_SUCCESS)
                {
                  // Remove the ADV set only if it is disabled
                  // If the set was already enabled it means the controller allocated
                  // its memory. We need to avoid removing it only from here.
                  // In addition, the set can keep running with the last vaid parameters.
                  if (pAdvSet->enableCmdParams.enable == LL_ADV_MODE_OFF)
                  {
                    hci_tl_RemoveAdvSet(ADV_LEGACY_SET_HANDLE);
                  }
                }
            }
            else
            {
                status = LL_STATUS_ERROR_OUT_OF_HEAP;
            }
            HCI_CommandCompleteEvent(cmdOpCode,
                                     sizeof(status),
                                     &status);
            break;
        }
        case HCI_LE_SET_ADV_DATA:
        {
            // Translate this legacy API to a call to the new AE API.
            hci_tl_advSet_t *pAdvSet;
            llStatus_t status;

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }

            if ( param[0] > LL_MAX_ADV_DATA_LEN )
            {
                status = LL_STATUS_ERROR_BAD_PARAMETER;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }

            // 1st check if there is not already an existing adv set with the same handle.
            pAdvSet = hci_tl_GetAdvSet(ADV_LEGACY_SET_HANDLE);

            if (pAdvSet)
            {
               // Legacy "set adv data" command might be sent before the "set adv parameters" command.
               // Support this case  by setting default parameters for legacy advertisement which
               // will be overriden by the "set adv parameters" later
               if (pAdvSet->advCmdParams.handle == ADV_INVALID_SET_HANDLE)
               {
                 aeSetParamRtn_t retParams;

                 // Init to default legacy params
                 hci_tl_InitAdvSetParams(pAdvSet);

                 // The parameters will be updated even if the call to  LE_SetExtAdvParams fails
                 // hci_tl_managedAEdata function will send the command complete event to host
                 LE_SetExtAdvParams(&pAdvSet->advCmdParams, &retParams);
               }

               // If set already exist, the parameters will be updated,
               // even if the call to  LE_SetExtAdvParams fails
               pAdvSet->advCmdData.handle    = ADV_LEGACY_SET_HANDLE;  //Forced
               pAdvSet->advCmdData.operation = AE_DATA_OP_COMPLETE;  //Forced
               pAdvSet->advCmdData.fragPref  = 0;                    //Forced
               pAdvSet->advCmdData.dataLen   = param[0];

               hci_tl_managedAEdata(cmdOpCode , &pAdvSet->advCmdData, &param[1]);
            }
            else
            {
                status = LL_STATUS_ERROR_OUT_OF_HEAP;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
            }
            break;
        }
        case HCI_LE_SET_SCAN_RSP_DATA:
        {
            // Translate this legacy to a call to the new AE API.
            hci_tl_advSet_t *pAdvSet;
            llStatus_t status;

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }

            if ( param[0] > LL_MAX_ADV_DATA_LEN )
            {
                status = LL_STATUS_ERROR_BAD_PARAMETER;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }
            // 1st check if there is not already an existing adv set with the same handle.
            pAdvSet = hci_tl_GetAdvSet(ADV_LEGACY_SET_HANDLE);

            if (pAdvSet)
            {
               // Legacy "set scan rsp data" command might be sent before the "set adv parameters" command.
               // Support this case  by setting default parameters for legacy advertisement which
               // will be overriden by the "set adv parameters" later
               if (pAdvSet->advCmdParams.handle == ADV_INVALID_SET_HANDLE)
               {
                 aeSetParamRtn_t retParams;

                 // Init to default legacy params
                 hci_tl_InitAdvSetParams(pAdvSet);

                 // The parameters will be updated even if the call to  LE_SetExtAdvParams fails
                 // hci_tl_managedAEdata function will send the command complete event to host
                 LE_SetExtAdvParams(&pAdvSet->advCmdParams, &retParams);
               }

               // If set already exist, the parameters will be updated,
               // even if the call to  LE_SetExtAdvParams fails
               pAdvSet->scanCmdData.handle    = ADV_LEGACY_SET_HANDLE;  //Forced
               pAdvSet->scanCmdData.operation = AE_DATA_OP_COMPLETE;  //Forced
               pAdvSet->scanCmdData.fragPref  = 0;                    //Forced
               pAdvSet->scanCmdData.dataLen   = param[0];

               hci_tl_managedAEdata(cmdOpCode, &pAdvSet->scanCmdData, &param[1]);
            }
            else
            {
                status = LL_STATUS_ERROR_OUT_OF_HEAP;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
            }
            break;
        }
        case HCI_LE_SET_ADV_ENABLE:
        {
            // Translate this legacy to a call to the new AE API.
            hci_tl_advSet_t *pAdvSet;
            llStatus_t status;

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }

            // Check number of Set to enable, only one supported by TL.
            pAdvSet = hci_tl_GetAdvSet(ADV_LEGACY_SET_HANDLE);

            if(pAdvSet)
            {
                LL_AE_RegCBack(LL_CBACK_ADV_START_AFTER_ENABLE, (void *)NULL);
                LL_AE_RegCBack(LL_CBACK_ADV_END_AFTER_DISABLE, (void *)NULL);
                LL_AE_RegCBack(LL_CBACK_ADV_START, (void *)NULL);
                LL_AE_RegCBack(LL_CBACK_ADV_END, (void *)NULL);
                LL_AE_RegCBack(LL_CBACK_EXT_SCAN_REQ_RECEIVED, (void *)NULL);
                LL_AE_RegCBack(LL_CBACK_ADV_SET_TERMINATED, (void *)NULL);
                LL_AE_RegCBack(LL_CBACK_EXT_ADV_DATA_TRUNCATED, (void *)NULL);
                LL_AE_RegCBack(LL_CBACK_OUT_OF_MEMORY, (void *)MAP_HCI_AeAdvCback);

                pAdvSet->enableCmdParams.enable    = param[0];
                pAdvSet->enableCmdParams.numSets   = 1;
                pAdvSet->enableCmdParams.handle    = ADV_LEGACY_SET_HANDLE;
                pAdvSet->enableCmdParams.duration  = 0;
                pAdvSet->enableCmdParams.maxEvents = 0;

                // Check if HCI_LE_SetRandomAddressCmd() function not called before
                // while the own address type is random.
                if ( hci_tl_isValidRandomAddressForAdv(pAdvSet) == FALSE )
                {
                    status = LL_STATUS_ERROR_BAD_PARAMETER;
                    HCI_CommandCompleteEvent(cmdOpCode,
                                             sizeof(status),
                                             &status);
                    break;
                }

                status = LE_SetExtAdvEnable(&pAdvSet->enableCmdParams);
                if (status == LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER)
                {
                    hci_tl_RemoveAdvSet(ADV_LEGACY_SET_HANDLE);
                }
            }
            else
            {
                status = LL_STATUS_ERROR_OUT_OF_HEAP;
            }
            HCI_CommandCompleteEvent(cmdOpCode,
                                     sizeof(status),
                                     &status);
            break;
        }
        // Extended Advertiser
        case HCI_LE_SET_EXT_ADV_PARAMETERS:
        {
            // This information needs to be saved on behalf of the LL.
            // A linked List will be created to save this information.
            hci_tl_advSet_t *pAdvSet;
            aeSetParamRtn_t retParams;
            int8_t rsp[2];

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                rsp[0] = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                rsp[1] = 0;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         2,
                                         rsp);
                break;
            }

            // 1st check if there is not already an existing adv set with the same handle.
            pAdvSet = hci_tl_GetAdvSet(param[0]);
            if (pAdvSet)
            {
                // If set already exist, parameters will be updated,
                // even if the call to  LE_SetExtAdvParams fails
                memcpy(&pAdvSet->advCmdParams, param, sizeof(aeSetParamCmd_t));
#ifndef CONTROLLER_ONLY
                // Deactivate zeroDelay, so the controller will add a delay to the advertise interval.
                pAdvSet->advCmdParams.zeroDelay = FALSE;
#endif

                rsp[0] = LE_SetExtAdvParams(&pAdvSet->advCmdParams, &retParams);
                rsp[1] = retParams.txPower;

                if (rsp[0] != LL_STATUS_SUCCESS)
                {
                  // Remove the ADV set only if it is disabled
                  // If the set was already enabled it means the controller allocated
                  // its memory. We need to avoid removing it only from here.
                  // In addition, the set can keep running with the last vaid parameters.
                  if (pAdvSet->enableCmdParams.enable == LL_ADV_MODE_OFF)
                  {
                    hci_tl_RemoveAdvSet(param[0]);
                  }
                }

            }
            else
            {
                rsp[0] = LL_STATUS_ERROR_OUT_OF_HEAP;
            }
            HCI_CommandCompleteEvent(cmdOpCode,
                                     2,
                                     rsp);
            break;
        }
        case HCI_LE_SET_EXT_ADV_ENABLE:
        {
            hci_tl_advSet_t *pAdvSet;
            llStatus_t status = LL_STATUS_SUCCESS;
            uint8 i;

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }

            // Enable/Disable all ADV sets
            if (param[1] == 0)
            {
                // ENABLE ALL ADV SETS
                if (param[0] == LL_ADV_MODE_ON)
                {
                    status = LL_STATUS_ERROR_BAD_PARAMETER;
                }
                // DISABLE ALL ADV SETS
                else
                {
                    // Point ot start of the ADV set list
                    pAdvSet = hci_tl_advSetList;

                    // Go over all ADV set list
                    while (pAdvSet != NULL)
                    {
                        // Set command parameters
                        pAdvSet->enableCmdParams.enable = LL_ADV_MODE_OFF;
                        pAdvSet->enableCmdParams.numSets = 1;
                        pAdvSet->enableCmdParams.handle = pAdvSet->handle;

                        // Check if HCI_LE_SetRandomAddressCmd() function not called before
                        // while the own address type is random.
                        if ( hci_tl_isValidRandomAddressForAdv(pAdvSet) == FALSE )
                        {
                            status = LL_STATUS_ERROR_BAD_PARAMETER;
                            HCI_CommandCompleteEvent(cmdOpCode,
                                                     sizeof(status),
                                                     &status);
                            break;
                        }

                        status = LE_SetExtAdvEnable(&pAdvSet->enableCmdParams);
                        if (status == LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER)
                        {
                            hci_tl_RemoveAdvSet(pAdvSet->handle);
                        }

                        // advance to next entry
                        pAdvSet = pAdvSet->next;
                    }
                }
            }
            else
            {
                // Enable/Disable ADV sets one by one
                for (i=0; i< param[1]; i++)
                {
                    pAdvSet = hci_tl_GetAdvSet(param[2 + (i * 4)]);

                    if(pAdvSet)
                    {
                        LL_AE_RegCBack(LL_CBACK_ADV_START_AFTER_ENABLE, (void *)MAP_HCI_AeAdvCback);
                        LL_AE_RegCBack(LL_CBACK_ADV_END_AFTER_DISABLE, (void *)MAP_HCI_AeAdvCback);
                        LL_AE_RegCBack(LL_CBACK_ADV_START, (void *)MAP_HCI_AeAdvCback);
                        LL_AE_RegCBack(LL_CBACK_ADV_END, (void *)MAP_HCI_AeAdvCback);
                        LL_AE_RegCBack(LL_CBACK_EXT_SCAN_REQ_RECEIVED, (void *)MAP_HCI_AeAdvCback);
                        LL_AE_RegCBack(LL_CBACK_ADV_SET_TERMINATED, (void *)MAP_HCI_AeAdvCback);
                        LL_AE_RegCBack(LL_CBACK_EXT_ADV_DATA_TRUNCATED, (void *)MAP_HCI_AeAdvCback);
                        LL_AE_RegCBack(LL_CBACK_OUT_OF_MEMORY, (void *)MAP_HCI_AeAdvCback);

                        // Set command parameters
                        pAdvSet->enableCmdParams.enable = param[0];
                        pAdvSet->enableCmdParams.numSets = 1;
                        pAdvSet->enableCmdParams.handle = param[2 + (i * 4)];
                        // For the duration calc we use byte 3 as LSB and byte 4 as MSB of param, therefore we need to perform
                        // 8 bits shift for the forth byte and bitwise or to the third byte.
                        pAdvSet->enableCmdParams.duration = (uint16)(param[4 + (i * 4)] << 8) | (param[3 + (i * 4)]);
                        pAdvSet->enableCmdParams.maxEvents = param[5 + (i * 4)];

                        // Check if HCI_LE_SetRandomAddressCmd() function not called before
                        // while the own address type is random.
                        if ( hci_tl_isValidRandomAddressForAdv(pAdvSet) == FALSE )
                        {
                            status = LL_STATUS_ERROR_BAD_PARAMETER;
                            HCI_CommandCompleteEvent(cmdOpCode,
                                                     sizeof(status),
                                                     &status);
                            break;
                        }

                        status = LE_SetExtAdvEnable(&pAdvSet->enableCmdParams);
                        if (status == LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER)
                        {
                            hci_tl_RemoveAdvSet(param[2 + (i * 4)]);
                        }
                    }
                    else
                    {
                        status = LL_STATUS_ERROR_OUT_OF_HEAP;
                    }
                }
            }

            HCI_CommandCompleteEvent(cmdOpCode,
                                      sizeof(status),
                                      &status);
            break;
        }
        case HCI_LE_REMOVE_ADV_SET:
        {
            llStatus_t status = LE_RemoveAdvSet(param[0]);

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }
            // Remove it from our linked list also
            if (status == LL_STATUS_SUCCESS)
            {
                hci_tl_RemoveAdvSet(param[0]);
            }
            HCI_CommandCompleteEvent(cmdOpCode,
                                      sizeof(status),
                                      &status);
            break;
        }
        case HCI_LE_CLEAR_ADV_SETS:
        {
            llStatus_t status = LE_ClearAdvSets();

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }
            if (status == LL_STATUS_SUCCESS)
            {
                OPT_hci_tl_ClearAdvSet();
            }
            HCI_CommandCompleteEvent(cmdOpCode,
                                     sizeof(status),
                                     &status);
            break;
        }
        case HCI_LE_SET_EXT_SCAN_RESPONSE_DATA:
        {
            hci_tl_advSet_t *pAdvSet;
            llStatus_t status;

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }

            pAdvSet = hci_tl_GetAdvSet(param[0]);
            if (pAdvSet)
            {
                // Extended ADV doesn't allow to send "set scan rsp data" command before "set adv params"
                if (pAdvSet->advCmdParams.handle == ADV_INVALID_SET_HANDLE)
                {
                   status = LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER;
                   HCI_CommandCompleteEvent(cmdOpCode,
                                            sizeof(status),
                                            &status);
                   hci_tl_RemoveAdvSet(param[0]);
                   break;
                }

                uint8_t idx = 0;
                pAdvSet->scanCmdData.handle    = param[idx++];
                pAdvSet->scanCmdData.operation = param[idx++];
                pAdvSet->scanCmdData.fragPref  = param[idx++];
                pAdvSet->scanCmdData.dataLen   = param[idx++];
                hci_tl_managedAEdata(cmdOpCode, &pAdvSet->scanCmdData, &param[idx]);
            }
            else
            {
                status = LL_STATUS_ERROR_OUT_OF_HEAP;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
            }
            break;
        }
        case HCI_LE_SET_EXT_ADV_DATA:
        {
            hci_tl_advSet_t *pAdvSet;
            llStatus_t status;

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }

            // 1st check if there is not already an existing adv set with the same handle.
            pAdvSet = hci_tl_GetAdvSet(param[0]);
            if (pAdvSet)
            {
               // Extended ADV doesn't allow to send "set adv data" command before "set adv params"
               if (pAdvSet->advCmdParams.handle == ADV_INVALID_SET_HANDLE)
               {
                  status = LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER;
                  HCI_CommandCompleteEvent(cmdOpCode,
                                           sizeof(status),
                                           &status);
                  hci_tl_RemoveAdvSet(param[0]);
                  break;
               }

               uint8_t idx = 0;
               // If set already exist, the parameters will be updated,
               // even if the call to  LE_SetExtAdvParams fails
               pAdvSet->advCmdData.handle    = param[idx++];
               pAdvSet->advCmdData.operation = param[idx++];
               pAdvSet->advCmdData.fragPref  = param[idx++];
               pAdvSet->advCmdData.dataLen   = param[idx++];
               hci_tl_managedAEdata(cmdOpCode, &pAdvSet->advCmdData, &param[idx]);
            }
            else
            {
                status = LL_STATUS_ERROR_OUT_OF_HEAP;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
            }
            break;
        }
        case HCI_EXT_LE_SET_EXT_ADV_DATA:
        {
            hci_tl_advSet_t *pAdvSet;
            llStatus_t status;

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }

            pAdvSet = hci_tl_GetAdvSet(param[0]);
            if (pAdvSet)
            {
                // Extended ADV doesn't allow to send "set adv data" command before "set adv params"
                if (pAdvSet->advCmdParams.handle == ADV_INVALID_SET_HANDLE)
                {
                   status = LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER;
                   HCI_CommandCompleteEvent(cmdOpCode,
                                            sizeof(status),
                                            &status);
                   hci_tl_RemoveAdvSet(param[0]);
                   break;
                }

                uint8_t idx = 0;
                uint16_t length;
                // If set already exist, the parameters will be updated,
                // even if the call to  LE_SetExtAdvParams fails
                pAdvSet->advCmdData.handle   = param[idx++];
                pAdvSet->advCmdData.fragPref = param[idx++];
                length = BUILD_UINT16(param[idx], param[idx+1]);
                idx+=2; // Skip the embedded Length Parameter
                pAdvSet->advCmdData.operation = AE_DATA_OP_COMPLETE;
                pAdvSet->advCmdData.dataLen   = length;
                // The real Data length of the buffer is contain in the first 2 bytes of the payload.
                hci_tl_managedAEdata(cmdOpCode, &pAdvSet->advCmdData, &param[idx]);
            }
            else
            {
                status = LL_STATUS_ERROR_OUT_OF_HEAP;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
            }
            break;
        }
        case HCI_EXT_LE_SET_EXT_SCAN_RESPONSE_DATA:
        {
            hci_tl_advSet_t *pAdvSet;
            llStatus_t status;

            // Check if a legacy/extended command mixing is allowed
            if(OPT_checkLegacyHCICmdStatus(cmdOpCode))
            {
                status = LL_STATUS_ERROR_COMMAND_DISALLOWED;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
                break;
            }

            pAdvSet = hci_tl_GetAdvSet(param[0]);
            if (pAdvSet)
            {
                // Extended ADV doesn't allow to send "set scan rsp data" command before "set adv params"
                if (pAdvSet->advCmdParams.handle == ADV_INVALID_SET_HANDLE)
                {
                   status = LL_STATUS_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER;
                   HCI_CommandCompleteEvent(cmdOpCode,
                                            sizeof(status),
                                            &status);
                   hci_tl_RemoveAdvSet(param[0]);
                   break;
                }

                uint8_t idx = 0;
                uint16_t length;
                // If set already exist, the parameters will be updated,
                // even if the call to  LE_SetExtAdvParams fails
                pAdvSet->scanCmdData.handle   = param[idx++];
                pAdvSet->scanCmdData.fragPref = param[idx++];
                length = BUILD_UINT16(param[idx], param[idx+1]);
                idx+=2; // Skip the embedded Length Parameter
                pAdvSet->scanCmdData.operation = AE_DATA_OP_COMPLETE;
                pAdvSet->scanCmdData.dataLen   = length;
                // The real Data length of the buffer is contain in the first 2 bytes of the payload.
                hci_tl_managedAEdata(cmdOpCode, &pAdvSet->scanCmdData, &param[idx]);
            }
            else
            {
                status = LL_STATUS_ERROR_OUT_OF_HEAP;
                HCI_CommandCompleteEvent(cmdOpCode,
                                         sizeof(status),
                                         &status);
            }
            break;
        }
        default:
        {
          retVal = HCI_ERROR_CODE_UNKNOWN_HCI_CMD;
        }
    }
    return retVal;
}

/*********************************************************************
 * @fn      processLegacyExtraHCICmd
 *
 * @brief   Process all legacy HCI command that was not found in the predefined table
 *
 * @param   pMsg -     HCI Msg packet
 * @return  retVal     HCI Status
 */
uint8_t processLegacyExtraHCICmd(hciPacket_t* pMsg)
{
    uint8_t retVal = HCI_SUCCESS;
    uint8_t packetType;
    uint16_t cmdOpCode;
    uint8_t *param; // pointer into HCI command parameters.

    // Retrieve packetType
    packetType = pMsg->pData[0];

    // Retrieve opcode
    cmdOpCode = BUILD_UINT16( pMsg->pData[1], pMsg->pData[2] );

    // Retrieve pointer to parameter
    if ( packetType == HCI_EXTENDED_CMD_PACKET )
    {
      param = &pMsg->pData[5];
    }
    else
    {
      param = &pMsg->pData[4];
    }


    // Else, Comand not found in processAdvExtraHCICmd, so continue...
    switch(cmdOpCode)
    {
        case HCI_RESET:
        {
            // Since HCI_ResetCmd is doing more than just
            // calling a LL function, it is being called directly.
            // The status will be returned directly through a
            // HCI_CommandCompleteEvent call performed by HCI_ResetCmd().
            legacyCmdStatusAdv = HCI_LEGACY_CMD_STATUS_UNDEFINED;
            legacyCmdStatusScan = HCI_LEGACY_CMD_STATUS_UNDEFINED;

            OPT_hci_tl_ClearAdvSet();

            HCI_ResetCmd();
            break;
        }

        // Scanner
        case HCI_LE_SET_SCAN_PARAM:
        {
            llStatus_t status;
            status = OPT_HCI_TL_set_scan_param(cmdOpCode, param, &hci_tl_cmdScanParams);
            HCI_CommandCompleteEvent(cmdOpCode,
                                     sizeof(status),
                                     &status);
            break;

          break;
        }
        case HCI_LE_SET_SCAN_ENABLE:
        {
            llStatus_t status;
            status = OPT_HCI_TL_set_scan_enable(cmdOpCode, param, &hci_tl_cmdScanEnable, &hci_tl_cmdScanParams);

            HCI_CommandCompleteEvent(cmdOpCode,
                                     sizeof(status),
                                     &status);
            break;
        }
        case HCI_LE_CREATE_CONNECTION:
        {
            llStatus_t status;
            status = OPT_HCI_TL_create_conn(cmdOpCode, param);
            HCI_CommandStatusEvent(status, cmdOpCode);
            break;
        }
        default:
        {
          retVal = HCI_ERROR_CODE_UNKNOWN_HCI_CMD;
        }
    }
    return retVal;
}
/*********************************************************************
 * @fn      hci_tl_ClearAdvSet
 *
 * @brief   remove a set from the AE linked list.
 *
 * @param   handle - handle of the set to remove.
 *
 * @return  none.
 */
void hci_tl_ClearAdvSet(void)
{
  hci_tl_advSet_t *pCurr = hci_tl_advSetList;

  while(pCurr)
  {
    // move head to next, if any
    hci_tl_advSetList = pCurr->next;

    if (pCurr->advCmdData.pData)
    {
        ICall_free(pCurr->advCmdData.pData);
    }

    if (pCurr->scanCmdData.pData)
    {
        ICall_free(pCurr->scanCmdData.pData);
    }

    // free what was at head
    ICall_free(pCurr);

    // point to next head, if any
    pCurr = hci_tl_advSetList;
  }

  hci_tl_advSetList = NULL;
}

/*******************************************************************************
 * check that if the own address type for ADV is random, HCI_LE_SetRandomAddressCmd was called.
 * If the own address Type is not random return TRUE.
 */
static uint8 hci_tl_isValidRandomAddressForAdv (hci_tl_advSet_t *pAdvSet)
{
  // If the own address Type is not random return TRUE
  if ((pAdvSet != NULL) &&
      (pAdvSet->advCmdParams.ownAddrType != LL_DEV_ADDR_TYPE_RANDOM) &&
      (pAdvSet->advCmdParams.ownAddrType != LL_DEV_ADDR_TYPE_RANDOM_ID))
  {
    return TRUE;
  }

  // else, own address type is random
  return LL_IsRandomAddressConfigured();
}

/*********************************************************************
 * @fn      HCI_TL_processHCICmd
 *
 * @brief   Detailed description is provided in the declaration of the function
 *          in icall_hci_tl.h.
 */
uint8_t HCI_TL_processHCICmd( hciPacket_t *pMsg )
{
  uint8_t status;

  status = HCI_CMD_Parser( pMsg->pData );

  if ( status == HCI_ERROR_CODE_UNKNOWN_HCI_CMD )
  {
    status = processExtraHCICmd(pMsg);
  }

  return ( status );
}

/*********************************************************************
 * @fn      HCI_TL_legacyProcessHCICmd
 *
 * @brief   Detailed description is provided in the declaration of the function
 *          in icall_hci_tl.h.
 */
uint8_t HCI_TL_legacyProcessHCICmd( hciPacket_t *pMsg )
{
  uint8_t status;

  status = processExtraHCICmd(pMsg);

  if ( status == HCI_ERROR_CODE_UNKNOWN_HCI_CMD )
  {
    status = HCI_CMD_Parser( pMsg->pData );
  }

  return ( status );
}

/*********************************************************************
*********************************************************************/
