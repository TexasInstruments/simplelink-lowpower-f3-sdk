/******************************************************************************

@file  BLEAppUtil_stack_callbacks.c

@brief This file contains the BLEAppUtil module stack callback functions

Group: WCS, BTS
$Target Device: DEVICES $

******************************************************************************
$License: BSD3 2022 $
******************************************************************************
$Release Name: PACKAGE NAME $
$Release Date: PACKAGE RELEASE DATE $
*****************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include <stdarg.h>
#include <ti/bleapp/ble_app_util/inc/bleapputil_api.h>
#include <ti/bleapp/ble_app_util/inc/bleapputil_internal.h>

/*********************************************************************
 * MACROS
 */
#define BLEAPPUTIL_PAIR_STATE_TABLE_SIZE 7

/*********************************************************************
* CONSTANTS
*/
// The following look up table is used to convert PairState events
// received from the BLE stack to BLEAppUtil PairState events
const uint8_t pairStateLookupTable[BLEAPPUTIL_PAIR_STATE_TABLE_SIZE] =
{
    BLEAPPUTIL_PAIRING_STATE_STARTED,
    BLEAPPUTIL_PAIRING_STATE_COMPLETE,
    BLEAPPUTIL_PAIRING_STATE_ENCRYPTED,
    BLEAPPUTIL_PAIRING_STATE_BOND_SAVED,
    BLEAPPUTIL_PAIRING_STATE_CAR_READ,
    BLEAPPUTIL_PAIRING_STATE_RPAO_READ,
    BLEAPPUTIL_GENERATE_ECC_DONE
};

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* GLOBAL VARIABLES
*/

/*********************************************************************
* LOCAL VARIABLES
*/

/*********************************************************************
* CALLBACKS
*/

/*********************************************************************
 * @fn      BLEAppUtil_processStackMsgCB
 *
 * @brief   BLE stack callback
 *
 * @param   event    - todo - remove the event from BLEAppUtil_processStackMsgCB
 * @param   pMessage - message from ble stack
 *
 * @return  None
 */
uint8_t BLEAppUtil_processStackMsgCB(uint8_t event, uint8_t *pMessage)
{
  // ignore the event
  // Enqueue the msg in order to be excuted in the application context
  if (BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_STACK_CALLBACK, pMessage) != SUCCESS)
  {
      BLEAppUtil_free(pMessage);
  }

  // Not safe to dealloc, the application BleAppUtil module will free the msg
  return FALSE;
}

/*********************************************************************
 * @fn      BLEAppUtil_pairStateCB
 *
 * @brief   Pairing state callback
 *
 * @param   connHandle - connection handle of the peer
 * @param   state      - state of the pairing process
 * @param   status     - status of the pairing process
 *
 * @return  None
 */
void BLEAppUtil_pairStateCB(uint16_t connHandle, uint8_t state, uint8_t status)
{
    BLEAppUtil_PairStateData_t *pData = BLEAppUtil_malloc(sizeof(BLEAppUtil_PairStateData_t));

  // Allocate space for the event data
  if (pData)
  {
    pData->connHandle = connHandle;
    pData->state = pairStateLookupTable[state];
    pData->status = status;

    // Queue the event
    if (BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_PAIRING_STATE_CB, pData) != SUCCESS)
    {
        BLEAppUtil_free(pData);
    }
  }
}

/*********************************************************************
 * @fn      BLEAppUtil_passcodeCB
 *
 * @brief   PassCode callback
 *
 * @param   *pDeviceAddr  - pointer to the peer address
 * @param   connHandle    - connection handle of the pairing peer
 * @param   uiInputs      - TRUE if the local device should accept a
 *                          passcode input
 * @param   uiOutputs     - TRUE if the local device should display
 *                          the passcode
 * @param   numComparison - Is zero until the passcode pairing is
 *                          complete. After that, it is the code that
 *                          should be displayed for numeric comparison pairing
 *
 * @return  None
 */
void BLEAppUtil_passcodeCB(uint8_t *pDeviceAddr,
                           uint16_t connHandle,
                           uint8_t uiInputs,
                           uint8_t uiOutputs,
                           uint32_t numComparison)
{
    BLEAppUtil_PasscodeData_t *pData = BLEAppUtil_malloc(sizeof(BLEAppUtil_PasscodeData_t));

    // Allocate space for the passcode event.
    if (pData)
    {
        pData->connHandle = connHandle;
        memcpy(pData->deviceAddr, pDeviceAddr, B_ADDR_LEN);
        pData->uiInputs = uiInputs;
        pData->uiOutputs = uiOutputs;
        pData->numComparison = numComparison;

        // Enqueue the event.
        if (BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_PASSCODE_NEEDED_CB, pData) != SUCCESS)
        {
            BLEAppUtil_free(pData);
        }
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_connEventCB
 *
 * @brief   Connection event callback
 *
 * @param   *pReport - pointer to connection event report
 *
 * @return  None
 */
void BLEAppUtil_connEventCB(Gap_ConnEventRpt_t *pReport)
{
    // Enqueue the event msg
    if ( BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_CONN_EVENT_CB, pReport) != SUCCESS)
    {
        BLEAppUtil_free(pReport);
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_scanCB
 *
 * @brief   Scan callback
 *
 * @param   event - The event to handle
 * @param   *pBuf - Data potentially accompanying event
 * @param   *arg  - Custom application argument that can be return through
 *                  this callback
 *
 * @return  None
 */
void BLEAppUtil_scanCB(uint32_t event, GapScan_data_t *pBuf, uint32_t *arg)
{
    BLEAppUtil_ScanEventData_t *pData = BLEAppUtil_malloc(sizeof(BLEAppUtil_ScanEventData_t));

    if (pData)
    {
        pData->event = event;
        pData->pBuf = pBuf;
        pData->arg = arg;

        // Enqueue the event
        if (BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_SCAN_CB_EVENT, pData) != SUCCESS)
        {
            BLEAppUtil_free(pData);
        }
    }
}

/*********************************************************************
 * @fn      BLEAppUtil_advCB
 *
 * @brief   Advertise callback
 *
 * @param   event - The event to handle
 * @param   *pBuf - Data potentially accompanying event
 * @param   *arg  - Custom application argument that can be return through
 *                  this callback
 *
 * @return  none
 */
void BLEAppUtil_advCB(uint32_t event, GapAdv_data_t *pBuf, uint32_t *arg)
{
    BLEAppUtil_AdvEventData_t *pData = BLEAppUtil_malloc(sizeof(BLEAppUtil_AdvEventData_t));

    if (pData)
    {
        pData->event = event;
        pData->pBuf = pBuf;
        pData->arg = arg;

        // Enqueue the event
        if (BLEAppUtil_enqueueMsg(BLEAPPUTIL_EVT_ADV_CB_EVENT, pData) != SUCCESS)
        {
            BLEAppUtil_free(pData);
        }
    }
}
