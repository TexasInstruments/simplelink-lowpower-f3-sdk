/******************************************************************************

@file  BLEAppUtil_init.c

@brief This file contains the BLEAppUtil module initialization functions

Group: WCS, BTS
Target Device: cc23xx

******************************************************************************

 Copyright (c) 2022-2025, Texas Instruments Incorporated
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
#include "ti/ble/app_util/framework/bleapputil_api.h"
#include "ti/ble/app_util/framework/bleapputil_internal.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/
typedef void (*pfnBleStkAdvCB_t) (uint32_t event,
                                  GapAdv_data_t *pBuf,
                                  uint32_t *arg);
typedef void (*bleStk_pfnGapScanCB_t) (uint32_t event,
                                       GapScan_data_t *pBuf,
                                     uint32_t *arg);

/*********************************************************************
* GLOBAL VARIABLES
*/
BLEAppUtil_TheardEntity_t BLEAppUtil_theardEntity;

/*********************************************************************
* LOCAL VARIABLES
*/
// Entity ID globally used to check for source and/or destination of messages
static BLEAppUtil_entityId_t BLEAppUtilSelfEntity;

// BLEAppUtil parameters given in the init function
BLEAppUtil_GeneralParams_t *BLEAppUtilLocal_GeneralParams = NULL;
BLEAppUtil_PeriCentParams_t *BLEAppUtilLocal_PeriCentParams = NULL;

// Callback functions handlers
ErrorHandler_t errorHandlerCb;
BLEAppUtil_EventHandlersList_t *BLEAppUtilEventHandlersHead = NULL;

// GAP Bond Manager Callbacks
gapBondCBs_t BLEAppUtil_bondMgrCBs =
{
    BLEAppUtil_passcodeCB, // Passcode callback
    BLEAppUtil_pairStateCB // Pairing/Bonding state Callback
};

// Handover Serving Node CB
const handoverSNCBs_t BLEAppUtil_handoverSnCB =
{
     BLEAppUtil_HandoverSNCB
};

const handoverCNCBs_t BLEAppUtil_handoverCnCB =
{
     BLEAppUtil_HandoverCNCB
};

const csCBs_t BLEAppUtil_csCBs =
{
     BLEAppUtil_CsCB    // Returns @ref BLEAPPUTIL_EVT_CS_EVENT_CB event
};

// Connection Monitor CBs
const cmsCBs_t BLEAppUtil_cmsCBs =
{
     BLEAppUtil_CmsConnUpdateCB
};

const cmCBs_t BLEAppUtil_cmCBs =
{
     BLEAppUtil_CmReportCB,
     BLEAppUtil_CmConnStatusCB
};

pthread_mutex_t mutex;

/*********************************************************************
* LOCAL FUNCTIONS
*/
static bStatus_t BLEAppUtil_createQueue(void);
ICall_Errno BLEAppUtil_registerIcall(uint8_t *selfEntity, appCallback_t appCallback);
void BLEAppUtil_createStackTasks();
bStatus_t BLEAppUtil_initGap(uint8_t role,
                             ICall_EntityID appSelfEntity,
                             bleStk_pfnGapScanCB_t scanCallback,
                             uint16_t paramUpdateDecision);

#if defined ( GAP_BOND_MGR )
void BLEAppUtil_initGapBondParams(gapBondParams_t *pGapBondParams);
#endif // GAP_BOND_MGR
bStatus_t BLEAppUtil_initGapBond(gapBondParams_t *pGapBondParams, void *bleApp_bondMgrCBs);
bStatus_t BLEAppUtil_initGatt(uint8_t role, ICall_EntityID appSelfEntity, uint8_t *pAttDeviceName);
bStatus_t BLEAppUtil_initHostAdvSet(pfnBleStkAdvCB_t advCallback, uint8_t *advHandle,
                                GapAdv_eventMaskFlags_t eventMask,
                                GapAdv_params_t *advParams,
                                uint16_t advDataLen, uint8_t *advData,
                                uint16_t scanRespDataLen, uint8_t *scanRespData);
/*********************************************************************
 * EXTERN FUNCTIONS
*/

/*********************************************************************
* CALLBACKS
*/

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      BLEAppUtil_init
 *
 * @brief   Initiate the application (queue, event, stack)
 *
 * @param   errorHandler       - Error handler to register
 * @param   initDoneHandler    - This handler will be called when the
 *                               device initialization is done
 * @param   initGeneralParams  - General parameters needed to initialize
 *                               the BLE stack
 * @param   initPeriCentParams - Connection related parameters needed
 *                               to initialize the BLE stack
 *
 * @return  None
 */
void BLEAppUtil_init(ErrorHandler_t errorHandler, StackInitDone_t initDoneHandler,
                     BLEAppUtil_GeneralParams_t *initGeneralParams,
                     BLEAppUtil_PeriCentParams_t *initPeriCentParams)
{
    // Register the application error handler
    errorHandlerCb = errorHandler;

    // Register the init done handler - will be called from the GAP_DEVICE_INIT_DONE_EVENT
    appInitDoneHandler = initDoneHandler;

    // Assign the BLEAppUtil parameters from the user to the local parameters
    BLEAppUtilLocal_GeneralParams = initGeneralParams;
    BLEAppUtilLocal_PeriCentParams = initPeriCentParams;

    // Create a message queue for message to be sent to BLEAppUtil
    BLEAppUtil_createQueue();

    // Create BLE stack task
    BLEAppUtil_createStackTasks();

    // Create local app task
    BLEAppUtil_createBLEAppUtilTask();

    // Construct a mutex that will be used by the following functions:
    // BLEAppUtil_registerEventHandler
    // BLEAppUtil_unRegisterEventHandler
    // BLEAppUtil_callEventHandler
    pthread_mutex_init(&mutex, NULL);
}

/*********************************************************************
 * @fn      BLEAppUtil_registerEventHandler
 *
 * @brief   Register application event handler
 *
 * @param   eventHandler - The handler to register
 *
 * @return  SUCCESS, FAILURE
 */
bStatus_t BLEAppUtil_registerEventHandler(BLEAppUtil_EventHandler_t *eventHandler)
{
    BLEAppUtil_EventHandlersList_t *newHandler;

    // Lock the Mutex
    pthread_mutex_lock(&mutex);

    // Allocate the new handler space
    newHandler = (BLEAppUtil_EventHandlersList_t *)BLEAppUtil_malloc(sizeof(BLEAppUtil_EventHandlersList_t));

    // If the allocation failed, return an error
    if(newHandler == NULL)
    {
        return FAILURE;
    }

    // Set the parameters of the new item
    newHandler->eventHandler = eventHandler;
    newHandler->next = NULL;

    // The head is NULL
    if(BLEAppUtilEventHandlersHead == NULL)
    {
        BLEAppUtilEventHandlersHead = newHandler;
    }
    else
    {
        // Add item to be the head of the list
        BLEAppUtil_EventHandlersList_t *iter = BLEAppUtilEventHandlersHead;

        // Iterate through the list to get to the last item
        while(iter->next != NULL)
        {
          iter = (BLEAppUtil_EventHandlersList_t *)iter->next;
        }

        // Add to the end of the list
        iter->next = (struct BLEAppUtil_EventHandlersList_t *)newHandler;
    }

    // Unlock the Mutex - item was added to the list
    pthread_mutex_unlock(&mutex);

    return SUCCESS;
}

/*********************************************************************
 * @fn      BLEAppUtil_unRegisterEventHandler
 *
 * @brief   Un-register application event handler
 *
 * @param   eventHandler - The handler to un-register
 *
 * @return  SUCCESS, INVALIDPARAMETER
 */
bStatus_t BLEAppUtil_unRegisterEventHandler(BLEAppUtil_EventHandler_t *eventHandler)
{
    BLEAppUtil_EventHandlersList_t *curr = BLEAppUtilEventHandlersHead;
    BLEAppUtil_EventHandlersList_t *prev = NULL;
    bStatus_t status = INVALIDPARAMETER;

    // Lock the Mutex
    pthread_mutex_lock(&mutex);

    // Go over the handlers list
    while(curr != NULL)
    {
        // The item containing the handler to un-register is found
        if(curr->eventHandler == eventHandler)
        {
            // The item is the head
            if(prev == NULL)
            {
                // Change the head to point the next item in the list
                BLEAppUtilEventHandlersHead = (BLEAppUtil_EventHandlersList_t *)curr->next;
            }
            // The item is not the head
            else
            {
                prev->next = curr->next;
            }

            // Free the item
            BLEAppUtil_free(curr);

            // Set the status to SUCCESS
            status = SUCCESS;
            break;
        }

        // Update the prev item to point the current item
        prev = curr;
        // Update the current item to point the next item
        curr = (BLEAppUtil_EventHandlersList_t *)prev->next;
    }

    // Unlock the Mutex - handler was removed from the list
    pthread_mutex_unlock(&mutex);

    return status;
}


/*********************************************************************
 * @fn      BLEAppUtil_invokeFunctionNoData
 *
 * @brief   This function receives a callback and data and switches
 *          the context in order to call the callback from the BLE
 *          App Util module context
 *
 * @param   callback - The callback to invoke from the BLE App Util
 *                     module context
 *
 * @return  SUCCESS, FAILURE
 */
bStatus_t BLEAppUtil_invokeFunctionNoData(InvokeFromBLEAppUtilContext_t callback)
{
    return BLEAppUtil_invokeFunction(callback, NULL);
}

/*********************************************************************
 * @fn      BLEAppUtil_invokeFunction
 *
 * @brief   This function receives a callback and data and switches
 *          the context in order to call the callback from the BLE
 *          App Util module context
 *
 * @param   callback - The callback to invoke from the BLE App Util
 *                     module context
 * @param   pData    - The data to provide the callback
 *
 * @return  SUCCESS, FAILURE
 */
bStatus_t BLEAppUtil_invokeFunction(InvokeFromBLEAppUtilContext_t callback, char* pData)
{
    BLEAppUtil_CallbackToInvoke_t *pDataMsg = NULL;

    // If the callback doesn't exist return an error
    if(callback == NULL)
    {
        return FAILURE;
    }

    // Create a BLEAppUtil_CallbackToInvoke_t and assign parameters
    pDataMsg = BLEAppUtil_malloc(sizeof(BLEAppUtil_CallbackToInvoke_t));

    // If the allocation failed, return an error
    if(pDataMsg == NULL)
    {
        if(pData != NULL)
        {
            BLEAppUtil_free(pData);
        }
        return FAILURE;
    }

    pDataMsg->callback = callback;
    pDataMsg->data     = pData;

    // Queue the event and data to switch context
    if (BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_CALL_IN_BLEAPPUTIL_CONTEXT, pDataMsg) != SUCCESS)
    {
        if(pData != NULL)
        {
            BLEAppUtil_free(pData);
        }
        BLEAppUtil_free(pDataMsg);
        return FAILURE;
    }
    return SUCCESS;
}

/*********************************************************************
* LOCAL FUNCTIONS
*/

/*********************************************************************
 * @fn      BLEAppUtil_stackRegister
 *
 * @brief   Register to stack messages callback
 *
 * @return  None
 */
void BLEAppUtil_stackRegister(void)
{
    ICall_Errno status;

    // ******************************************************************
    // NO STACK API CALLS CAN OCCUR BEFORE THIS CALL TO bleStack_register
    // ******************************************************************
    // Register the stack messages callback
    status = BLEAppUtil_registerIcall(&BLEAppUtilSelfEntity,
                                      BLEAppUtil_processStackMsgCB);
    if(status != SUCCESS)
    {
        // TODO: Call Error Handler
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_stackInit
 *
 * @brief   Initialize the BLE stack host using the parameters from @ref
 *          BLEAppUtil_GeneralParams_t and @ref BLEAppUtil_PeriCentParams_t,
 *          provided by @ref BLEAppUtil_init
 *
 * @return  None
 */
void BLEAppUtil_stackInit(void)
{
    bStatus_t status;

    // Init GAP
    status = BLEAppUtil_initGap(BLEAppUtilLocal_GeneralParams->profileRole,
                                BLEAppUtilSelfEntity, BLEAppUtil_scanCB,
                                BLEAppUtilLocal_PeriCentParams->connParamUpdateDecision);
    if(status != SUCCESS)
    {
        // TODO: Call Error Handler
    }

    // Init GapBond
    status = BLEAppUtil_initGapBond(BLEAppUtilLocal_PeriCentParams->gapBondParams,
                                    &BLEAppUtil_bondMgrCBs);
    if(status != SUCCESS)
    {
        // TODO: Call Error Handler
    }

    // Init GATT
    status = BLEAppUtil_initGatt(BLEAppUtilLocal_GeneralParams->profileRole,
                                 BLEAppUtilSelfEntity,
                                 BLEAppUtilLocal_GeneralParams->deviceNameAtt);
    if(status != SUCCESS)
    {
        // TODO: Call Error Handler
    }

    // Initialize GAP layer to receive GAP events
    status = GAP_DeviceInit(BLEAppUtilLocal_GeneralParams->profileRole,
                            BLEAppUtilSelfEntity,
                            BLEAppUtilLocal_GeneralParams->addressMode,
                            BLEAppUtilLocal_GeneralParams->pDeviceRandomAddress);
    if(status != SUCCESS)
    {
        // TODO: Call Error Handler
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_getSelfEntity
 *
 * @brief   Returns the self entity id needed when sending BLE stack
 *          commands
 *
 * @return  The entityId
 */
BLEAppUtil_entityId_t BLEAppUtil_getSelfEntity(void)
{
    return BLEAppUtilSelfEntity;
}

/*********************************************************************
 * @fn      BLEAppUtil_createQueue
 *
 * @brief   Create a message queue for message to be sent to BLEAppUtil
 *
 * @return  SUCCESS, FAILURE
 */
static bStatus_t BLEAppUtil_createQueue(void)
{
     struct mq_attr attr;

     attr.mq_flags = 0; // Parameter mq_flags is ignored when the queue is created with O_CREAT
     attr.mq_curmsgs = 0;
     attr.mq_maxmsg = 8;
     attr.mq_msgsize = sizeof(BLEAppUtil_appEvt_t);

     /* Create the message queue */
     BLEAppUtil_theardEntity.queueHandle = mq_open("BLEAppUtil_theardQueue", O_CREAT , 0, &attr);

     if (BLEAppUtil_theardEntity.queueHandle == (mqd_t)-1)
     {
         return FAILURE;
     }
     return SUCCESS;
}

/*********************************************************************
 * @fn      BLEAppUtil_registerIcall
 *
 * @brief   register callback in the ICALL for application events
 *
 * @param   selfEntity - return self entity id from the ICALL
 * @param   appCallback- The callback
 *
 * @return  SUCCESS or FAILURE.
 */
ICall_Errno BLEAppUtil_registerIcall(uint8_t *selfEntity, appCallback_t appCallback)
{
  ICall_EntityID   localSelfEntity;
  ICall_Errno status;
  // ******************************************************************
  // NO STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
  // ******************************************************************
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  status = ICall_registerAppCback(&localSelfEntity, appCallback);

  // Application should use the task entity ID
  *selfEntity = localSelfEntity;

  return status;
}

/*********************************************************************
 * @fn      BLEAppUtil_createStackTasks
 *
 * @brief   Create ICALL tasks
 *
 * @param   None
 *
 * @return  None
 */
void BLEAppUtil_createStackTasks()
{
    /* Initialize ICall module */
    ICall_init();

    /* Start tasks of external images - Priority 5 */
    ICall_createRemoteTasks();
}

/*********************************************************************
 * @fn      BLEAppUtil_initGap
 *
 * @brief   Init the GAP
 *
 * @param   role - the role of the application
 * @param   appSelfEntity - self entity id for the ICALL
 * @param   paramUpdateDecision- the param update configuration
 *
 * @return  SUCCESS ot FAILURE.
 */
bStatus_t BLEAppUtil_initGap(uint8_t role,
                             ICall_EntityID appSelfEntity,
                             bleStk_pfnGapScanCB_t scanCallback,
                             uint16_t paramUpdateDecision)
{
  bStatus_t status = SUCCESS;
#if defined( HOST_CONFIG ) && ( HOST_CONFIG & ( CENTRAL_CFG | PERIPHERAL_CFG ) )

  // Pass all parameter update requests to the app for it to decide
  GAP_SetParamValue(GAP_PARAM_LINK_UPDATE_DECISION, paramUpdateDecision);
#endif //#if defined( HOST_CONFIG ) && ( HOST_CONFIG & ( CENTRAL_CFG | PERIPHERAL_CFG ) )

#if defined( HOST_CONFIG ) && ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
  if(scanCallback != NULL)
  {
    // Register scan callback to process scanner events
    status = GapScan_registerCb(scanCallback, NULL);
    if (status != SUCCESS)
    {
      return status;
    }

    // Set Scanner Event Mask
    GapScan_setEventMask(GAP_EVT_SCAN_EVT_MASK);
  }
#endif //#if defined( HOST_CONFIG ) && ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )

  // Register with GAP for HCI/Host messages. This is needed to receive HCI
  // events. For more information, see the HCI section in the User's Guide:
  // http://software-dl.ti.com/lprf/ble5stack-latest/
  GAP_RegisterForMsgs(appSelfEntity);

  return status;
}

#if defined ( GAP_BOND_MGR )
/*********************************************************************
 * @fn      BLEAppUtil_initGapBondParams
 *
 * @brief   Set all GAP bond manager parameters
 *
 * @param   pGapBondParams - the configuration structure
 *
 * @return  SUCCESS ot FAILURE.
 */
void BLEAppUtil_initGapBondParams(gapBondParams_t *pGapBondParams)
{
    // Set Pairing Mode
    GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8_t), &pGapBondParams->pairMode);
    // Set MITM Protection
    GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8_t), &pGapBondParams->mitm);
    // Set IO Capabilities
    GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8_t), &pGapBondParams->ioCap);
    // Set Bonding
    GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8_t), &pGapBondParams->bonding);
    // Set Secure Connection Usage during Pairing
    GAPBondMgr_SetParameter(GAPBOND_SECURE_CONNECTION, sizeof(uint8_t), &pGapBondParams->secureConnection);
    // Set Authenticated Pairing Only mode
    GAPBondMgr_SetParameter(GAPBOND_AUTHEN_PAIRING_ONLY, sizeof(uint8_t), &pGapBondParams->authenPairingOnly);
    // Set Auto Acceptlist Sync
    GAPBondMgr_SetParameter(GAPBOND_AUTO_SYNC_AL, sizeof(uint8_t), &pGapBondParams->autoSyncAL);
    // Set ECC Key Regeneration Policy
    GAPBondMgr_SetParameter(GAPBOND_ECCKEY_REGEN_POLICY, sizeof(uint8_t), &pGapBondParams->eccReGenPolicy);
    // Set Key Size used in pairing
    GAPBondMgr_SetParameter(GAPBOND_KEYSIZE, sizeof(uint8_t), &pGapBondParams->KeySize);
    // Set LRU Bond Replacement Scheme
    GAPBondMgr_SetParameter(GAPBOND_LRU_BOND_REPLACEMENT, sizeof(uint8_t), &pGapBondParams->removeLRUBond);
    // Set Key Distribution list for pairing
    GAPBondMgr_SetParameter(GAPBOND_KEY_DIST_LIST, sizeof(uint8_t), &pGapBondParams->KeyDistList);
    // Set Secure Connection Debug Keys
    GAPBondMgr_SetParameter(GAPBOND_SC_HOST_DEBUG, sizeof(uint8_t), &pGapBondParams->eccDebugKeys);
    // Set the Erase bond While in Active Connection Flag
    GAPBondMgr_SetParameter(GAPBOND_ERASE_BOND_IN_CONN, sizeof(uint8_t), &pGapBondParams->eraseBondWhileInConn);
    // Set Same IRK Action
    GAPBondMgr_SetParameter(GAPBOND_SAME_IRK_OPTION, sizeof(uint8_t), &pGapBondParams->sameIrkAction);
}
#endif // GAP_BOND_MGR

/*********************************************************************
 * @fn      BLEAppUtil_initGapBond
 *
 * @brief   Init GAP bond manager
 *
 * @param   pGapBondParams - the configuration structure
 * @param   bleApp_bondMgrCBs - the pairing and passcode callbacks
 *
 * @return  SUCCESS ot FAILURE.
 */
bStatus_t BLEAppUtil_initGapBond(gapBondParams_t *pGapBondParams, void *bleApp_bondMgrCBs)
{
#if defined ( GAP_BOND_MGR )
  BLEAppUtil_initGapBondParams(pGapBondParams);

  if (bleApp_bondMgrCBs != NULL)
  {
    // Start Bond Manager and register callback
    VOID GAPBondMgr_Register((gapBondCBs_t *)bleApp_bondMgrCBs);
  }
#endif // GAP_BOND_MGR
  return SUCCESS;
}

/*********************************************************************
 * @fn      BLEAppUtil_initGatt
 *
 * @brief   Init the GATT
 *
 * @param   role - the role of the application
 * @param   appSelfEntity - self entity id for the ICALL
 * @param   pAttDeviceName- the device name
 *
 * @return  SUCCESS ot FAILURE.
 */
bStatus_t BLEAppUtil_initGatt(uint8_t role, ICall_EntityID appSelfEntity, uint8_t *pAttDeviceName)
{
    bStatus_t status = SUCCESS;

  // Set the Device Name characteristic in the GAP GATT Service
  // For more information, see the section in the User's Guide:
  // http://software-dl.ti.com/lprf/ble5stack-latest/
    status = GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, (void *)pAttDeviceName);
    if (status != SUCCESS)
    {
      return status;
    }

  // Initialize GATT attributes
  GGS_AddService(GATT_ALL_SERVICES);           // GAP GATT Service
  GATTServApp_AddService(GATT_ALL_SERVICES);   // GATT Service

  // Register for GATT local events and ATT Responses pending for transmission
  GATT_RegisterForMsgs(appSelfEntity);

  if (role & (GAP_PROFILE_PERIPHERAL | GAP_PROFILE_BROADCASTER))
  // Set default values for Data Length Extension
  // Extended Data Length Feature is already enabled by default
  {
    // Set initial values to maximum
    #define BLEAPP_SUGGESTED_PDU_SIZE 251
    #define BLEAPP_SUGGESTED_TX_TIME 2120 //default is 328us(TX)

    // This API is documented in hci.h
    // See the LE Data Length Extension section in the BLE5-Stack User's Guide for information on using this command:
    // http://software-dl.ti.com/lprf/ble5stack-latest/
    HCI_LE_WriteSuggestedDefaultDataLenCmd(BLEAPP_SUGGESTED_PDU_SIZE, BLEAPP_SUGGESTED_TX_TIME);
  }
  if (role & (GAP_PROFILE_CENTRAL | GAP_PROFILE_OBSERVER))
  //Set default values for Data Length Extension
  //Extended Data Length Feature is already enabled by default
  //in build_config.opt in stack project.
  {
    //Change initial values of RX/TX PDU and Time, RX is set to max. by default(251 octets, 2120us)
    #define APP_SUGGESTED_RX_PDU_SIZE 251     //default is 251 octets(RX)
    #define APP_SUGGESTED_RX_TIME     17000   //default is 17000us(RX)
    #define APP_SUGGESTED_TX_PDU_SIZE 27      //default is 27 octets(TX)
    #define APP_SUGGESTED_TX_TIME     328     //default is 328us(TX)

    //This API is documented in hci.h
    //See the LE Data Length Extension section in the BLE5-Stack User's Guide for information on using this command:
    //http://software-dl.ti.com/lprf/ble5stack-latest/
    HCI_EXT_SetMaxDataLenCmd(APP_SUGGESTED_TX_PDU_SIZE, APP_SUGGESTED_TX_TIME, APP_SUGGESTED_RX_PDU_SIZE, APP_SUGGESTED_RX_TIME);
  }

  if (role & (GAP_PROFILE_PERIPHERAL | GAP_PROFILE_CENTRAL))
  {
      // Initialize GATT Client
      GATT_InitClient();
  }

  if (role & (GAP_PROFILE_PERIPHERAL | GAP_PROFILE_CENTRAL))
  {
      // Register to receive incoming ATT Indications/Notifications
      GATT_RegisterForInd(appSelfEntity);
  }

  return status;
}

/*********************************************************************
 * @fn      BLEAppUtil_initHostAdvSet
 *
 * @brief   Initialize and starts advertise set (legacy or extended)
 *
 * @param advCallback     - callback for advertising progress states
 * @param advHandle       - return the created advertising handle
 * @param eventMask       - A bitfield to enable / disable events returned to the
 *                          per-advertising set callback function (@ref pfnGapCB_t ).
 *                          See @ref GapAdv_eventMaskFlags_t
 * @param advParams       - pointer to structure of adversing parameters
 * @param advData         - pointer to array containing the advertise data
 * @param advDataLen      - length (in bytes) of advData
 * @param scanRespData    - pointer to array containing the scan response data
 * @param scanRespDataLen - length (in bytes) of scanRespDataLen
 *
 * @return SUCCESS upon successful initialization,
 *         else, relevant error code upon failure
 */
bStatus_t BLEAppUtil_initHostAdvSet(pfnBleStkAdvCB_t advCallback, uint8_t *advHandle,
                                   GapAdv_eventMaskFlags_t eventMask,
                                   GapAdv_params_t *advParams,
                                   uint16_t advDataLen ,uint8_t *advData,
                                   uint16_t scanRespDataLen, uint8_t *scanRespData)
{
  bStatus_t status;

  status = GapAdv_create((pfnGapCB_t)advCallback, advParams, advHandle);
  if (status != SUCCESS)
  {
    return status;
  }

  if (advData != NULL)
  {
    // Load advertising data for set that is statically allocated by the app
    status = GapAdv_loadByHandle(*advHandle, GAP_ADV_DATA_TYPE_ADV, advDataLen, advData);
    if (status != SUCCESS)
    {
      return status;
    }
  }

  // Load scan response data for set that is statically allocated by the app
  if (scanRespData != NULL)
  {
    status = GapAdv_loadByHandle(*advHandle, GAP_ADV_DATA_TYPE_SCAN_RSP, scanRespDataLen, scanRespData);
    if (status != SUCCESS)
    {
      return status;
    }
  }

  // Set event mask for set
  status = GapAdv_setEventMask(*advHandle, eventMask);
  if (status != SUCCESS)
  {
    return status;
  }

  return status;
}

/////////////////////////////////////////////////////////////////////////
// Host Functions Encapsulation
/////////////////////////////////////////////////////////////////////////

bStatus_t BLEAppUtil_initAdvSet(uint8 *advHandle, const BLEAppUtil_AdvInit_t *advInitInfo)
{
    return BLEAppUtil_initHostAdvSet(BLEAppUtil_advCB, advHandle, GAP_ADV_EVT_MASK_ALL,
                                 advInitInfo->advParam, advInitInfo->advDataLen ,
                                 advInitInfo->advData, advInitInfo->scanRespDataLen,
                                 advInitInfo->scanRespData);
}

bStatus_t BLEAppUtil_advStart(uint8 handle, const BLEAppUtil_AdvStart_t *advStartInfo)
{
    return GapAdv_enable(handle, advStartInfo->enableOptions ,
                         advStartInfo->durationOrMaxEvents);
}

bStatus_t BLEAppUtil_advStop(uint8 handle)
{
    return GapAdv_disable(handle);
}


bStatus_t BLEAppUtil_scanInit(const BLEAppUtil_ScanInit_t *scanInitInfo)
{
    bStatus_t status;

    // Set Scan PHY parameters
    status = GapScan_setPhyParams(scanInitInfo->primPhy,
                                  scanInitInfo->scanType,
                                  scanInitInfo->scanInterval,
                                  scanInitInfo->scanWindow);
    if (status != SUCCESS)
    {
        return status;
    }

    // Set scan parameters
    status = GapScan_setParam(SCAN_PARAM_RPT_FIELDS, &scanInitInfo->advReportFields);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapScan_setParam(SCAN_PARAM_PRIM_PHYS, &scanInitInfo->scanPhys);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapScan_setParam(SCAN_PARAM_FLT_POLICY, &scanInitInfo->fltPolicy);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapScan_setParam(SCAN_PARAM_FLT_PDU_TYPE, &scanInitInfo->fltPduType);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapScan_setParam(SCAN_PARAM_FLT_MIN_RSSI, &scanInitInfo->fltMinRssi);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapScan_setParam(SCAN_PARAM_FLT_DISC_MODE, &scanInitInfo->fltDiscMode);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapScan_setParam(SCAN_PARAM_FLT_DUP, &scanInitInfo->fltDup);

    return status;
}

bStatus_t BLEAppUtil_scanStart(const BLEAppUtil_ScanStart_t *scanStartInfo)
{
    return GapScan_enable(scanStartInfo->scanPeriod, scanStartInfo->scanDuration, scanStartInfo->maxNumReport);
}

bStatus_t BLEAppUtil_scanStop(void)
{
    return GapScan_disable();
}

bStatus_t BLEAppUtil_setConnParams(const BLEAppUtil_ConnParams_t *connParams)
{
    bStatus_t status;

    status = GapInit_setPhyParam(connParams->initPhys, INIT_PHYPARAM_SCAN_INTERVAL, connParams->scanInterval);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapInit_setPhyParam(connParams->initPhys, INIT_PHYPARAM_SCAN_WINDOW, connParams->scanWindow);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapInit_setPhyParam(connParams->initPhys, INIT_PHYPARAM_CONN_INT_MIN, connParams->minConnInterval);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapInit_setPhyParam(connParams->initPhys, INIT_PHYPARAM_CONN_INT_MAX, connParams->maxConnInterval);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapInit_setPhyParam(connParams->initPhys, INIT_PHYPARAM_CONN_LATENCY, connParams->connLatency);
    if(status != SUCCESS)
    {
        return(status);
    }
    status = GapInit_setPhyParam(connParams->initPhys, INIT_PHYPARAM_SUP_TIMEOUT, connParams->supTimeout);
    if(status != SUCCESS)
    {
        return(status);
    }

    return SUCCESS;
}

bStatus_t BLEAppUtil_connect(BLEAppUtil_ConnectParams_t *connParams)
{
    return GapInit_connect(connParams->peerAddrType, connParams->pPeerAddress,
                           connParams->phys, connParams->timeout);
}

bStatus_t BLEAppUtil_cancelConnect()
{
    return GapInit_cancelConnect();
}

bStatus_t BLEAppUtil_disconnect(uint16 connHandle)
{
  return GAP_TerminateLinkReq(connHandle, HCI_DISCONNECT_REMOTE_USER_TERM);
}

GAP_Addr_Modes_t BLEAppUtil_getDevAddrMode(void)
{
  return BLEAppUtilLocal_GeneralParams->addressMode;
}

bStatus_t BLEAppUtil_setConnPhy(BLEAppUtil_ConnPhyParams_t *phyParams)
{
    // Set Phy Preference on the current connection. Apply the same value
    // for RX and TX. For more information, see the LE 2M PHY section in the User's Guide:
    // http://software-dl.ti.com/lprf/ble5stack-latest/
    return HCI_LE_SetPhyCmd(phyParams->connHandle, phyParams->allPhys, phyParams->txPhy, phyParams->rxPhy, phyParams->phyOpts);
}

bStatus_t BLEAppUtil_registerConnNotifHandler(uint16_t connHandle, GAP_CB_Event_e eventType)
{
    return Gap_RegisterConnEventCb(BLEAppUtil_connEventCB, GAP_CB_REGISTER, eventType, connHandle);
}
bStatus_t BLEAppUtil_unRegisterConnNotifHandler()
{
    return Gap_RegisterConnEventCb(BLEAppUtil_connEventCB, GAP_CB_UNREGISTER, GAP_CB_CONN_EVENT_ALL, LINKDB_CONNHANDLE_ALL);
}

/*********************************************************************
 * @fn      BLEAppUtil_paramUpdateRsp
 *
 * @brief   This function prepares an answer for connection parameters
 *          changing request, according to the decision made by the application,
 *          And then call for gap function with the response.
 *
 * @param   pReq - Pointer to master request
 *          accept - Application decision.
 *
 * @return  status
 */
bStatus_t BLEAppUtil_paramUpdateReq(gapUpdateLinkParamReq_t *pReq)
{
    bStatus_t status = FAILURE;
    linkDBInfo_t linkInfo;

    // Verify that the connection is active and send the param update request
    if (linkDB_GetInfo(pReq->connectionHandle, &linkInfo) == SUCCESS)
    {
        status = GAP_UpdateLinkParamReq(pReq);
    }

    return status;
}

/*********************************************************************
 * @fn      BLEAppUtil_paramUpdateRsp
 *
 * @brief   This function prepares an answer for connection parameters
 *          changing request, according to the decision made by the application,
 *          And then call for gap function with the response.
 *
 * @param   pReq - Pointer to central request
 *          accept - Application decision.
 *
 * @return  SUCCESS, INVALIDPARAMETER
 */
bStatus_t BLEAppUtil_paramUpdateRsp(gapUpdateLinkParamReqEvent_t *pReq, uint8 accept)
{
    bStatus_t status = SUCCESS;
    gapUpdateLinkParamReqReply_t rsp;

    rsp.connectionHandle = pReq->req.connectionHandle;
    rsp.signalIdentifier = pReq->req.signalIdentifier;

    if(accept)
    {
        rsp.intervalMin = pReq->req.intervalMin;
        rsp.intervalMax = pReq->req.intervalMax;
        rsp.connLatency = pReq->req.connLatency;
        rsp.connTimeout = pReq->req.connTimeout;
        rsp.accepted = TRUE;
    }
    else
    {
        rsp.accepted = FALSE;
    }
    status = GAP_UpdateLinkParamReqReply(&rsp);
    // Send Reply and return

    return (status);
}

/*********************************************************************
 * @fn      BLEAppUtil_registerSNCB
 *
 * @brief   Register the serving node callback
 *
 * @param   None
 *
 * @return  SUCCESS, FAILURE
 */
bStatus_t BLEAppUtil_registerSNCB(void)
{
    return Handover_RegisterSNCBs(&BLEAppUtil_handoverSnCB);
}

/*********************************************************************
 * @fn      BLEAppUtil_registerCNCB
 *
 * @brief   Register the serving node callback
 *
 * @param   None
 *
 * @return  SUCCESS, FAILURE
 */
bStatus_t BLEAppUtil_registerCNCB(void)
{
    return Handover_RegisterCNCBs(&BLEAppUtil_handoverCnCB);
}

/*********************************************************************
 * @fn      BLEAppUtil_registerCsCB
 *
 * @brief   Register the Channel Sounding callback
 *
 * @param   None
 *
 * @return  SUCCESS, FAILURE
 */
bStatus_t BLEAppUtil_registerCsCB(void)
{
    return CS_RegisterCB(&BLEAppUtil_csCBs);
}

/*********************************************************************
 * @fn      BLEAppUtil_registerCMSCBs
 *
 * @brief   Register to connection monitor callbacks Connection Monitor
 *          Serving.
 * @param   None
 *
 * @return  SUCCESS, FAILURE
 */
bStatus_t BLEAppUtil_registerCMSCBs(void)
{
    return CMS_RegisterCBs(&BLEAppUtil_cmsCBs);
}

/*********************************************************************
 * @fn      BLEAppUtil_registerCMCBs
 *
 * @brief   Register to connection monitor callbacks Connection Monitor
 *          Role.
 *
 * @param   None
 *
 * @return  SUCCESS, FAILURE
 */
bStatus_t BLEAppUtil_registerCMCBs(void)
{
    return CM_RegisterCBs(&BLEAppUtil_cmCBs);
}

/*********************************************************************
 * @fn      BLEAppUtil_isbufset
 *
 * @brief   Is all of the array elements set to a value
 *
 * @param   buf - buffer to check
 * @param   val - value to check each array element for
 * @param   len - length to check
 *
 * @return  TRUE if all "val"
 *          FALSE otherwise
 */
bool BLEAppUtil_isbufset( uint8_t *buf, uint8_t val, uint8_t len )
{
  uint8_t x;

  if ( buf == NULL )
  {
    return ( FALSE );
  }

  for ( x = 0; x < len; x++ )
  {
    // Check for non-initialized value
    if ( buf[x] != val )
    {
      return ( FALSE );
    }
  }
  return ( TRUE );
}
