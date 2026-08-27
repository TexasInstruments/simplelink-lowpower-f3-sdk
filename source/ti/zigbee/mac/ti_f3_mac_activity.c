/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2026, Texas Instruments Incorporated
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

#include <ti_f3_mac_activity.h>

#include "zb_common.h"

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/* Activity tracking variables, updated within ISR and MAC Task context. */
static volatile macActivityData_t activityData = {0};


/*******************************************************************************
 * GLOBAL VARIABLES
 */

/* ------------------------------------------------------------------------------------------------
 *                                        Local Functions
 * ------------------------------------------------------------------------------------------------
 */
static void setActivityTrackingTx(bool resetCount);
static void setActivityTrackingRx(bool resetCount);
static uint32_t getActivityPriorityTx(void);
static uint32_t getActivityPriorityRx(void);
static void setActivityTx(uint8_t commandId);
static void setActivityRx(bool isPolling, bool isScanning);
static macActivity_t getActivityTx(void);
static macActivity_t getActivityRx(void);


const activityObject_t activityObject = {
    .pSetActivityTrackingTxFn = setActivityTrackingTx,
    .pSetActivityTrackingRxFn = setActivityTrackingRx,
    .pGetActivityPriorityTxFn = getActivityPriorityTx,
    .pGetActivityPriorityRxFn = getActivityPriorityRx,
    .pSetActivityTxFn = setActivityTx,
    .pSetActivityRxFn = setActivityRx,
    .pGetActivityTxFn = getActivityTx,
    .pGetActivityRxFn = getActivityRx
};

/*******************************************************************************
 * @fn          setActivityTrackingTx
 *
 * @brief       This call is used by the LMAC to update the Transmit activity
 *              tracking metrics.
 *
 * input parameters
 *
 * @param       resetCount - Whether to reset activity tracking count.
 *                           True to reset, False to increment count.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static void setActivityTrackingTx(bool resetCount)
{
    switch (activityData.txActivity)
    {
        case MAC_ACTIVITY_TX_LINK_EST:
            activityData.numMissedLinkEstTxFrames = resetCount ? 0 : activityData.numMissedLinkEstTxFrames + MAC_ACTIVITY_DEFAULT_INC;
            break;
        case MAC_ACTIVITY_TX_BEACON:
            activityData.numMissedBeaconTxFrames = resetCount ? 0 : activityData.numMissedBeaconTxFrames + MAC_ACTIVITY_DEFAULT_INC;
            break;
        case MAC_ACTIVITY_TX_DATA:
            activityData.numMissedDataTxFrames = resetCount ? 0 : activityData.numMissedDataTxFrames + MAC_ACTIVITY_DEFAULT_INC;
            break;
        default:
            break;
    }
}

/*******************************************************************************
 * @fn          setActivityTrackingRx
 *
 * @brief       This call is used by the LMAC to update the Receive activity
 *              tracking metrics.
 *
 * input parameters
 *
 * @param       resetCount - Whether to reset activity tracking count.
 *                           True to reset, False to increment count.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static void setActivityTrackingRx(bool resetCount)
{
    switch (activityData.rxActivity)
    {
        case MAC_ACTIVITY_RX_SCAN:
            activityData.numRxScanAbort = resetCount ? 0 : activityData.numRxScanAbort + MAC_ACTIVITY_DEFAULT_INC;
            break;
        case MAC_ACTIVITY_RX_POLLING:
            activityData.numRxPollAbort = resetCount ? 0 : activityData.numRxPollAbort + MAC_ACTIVITY_DEFAULT_INC;
            break;
        default:
            break;
    }
}

/*******************************************************************************
 * @fn          getActivityPriorityTx
 *
 * @brief       This call is used by the LMAC to calculate the activity priority
 *              index for the current activity.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static uint32_t getActivityPriorityTx(void)
{
    uint32_t currPri;

    switch(activityData.txActivity)
    {
        case MAC_ACTIVITY_TX_LINK_EST:
            currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_TX_LINK_EST, MAC_ACTIVITY_PRI_URGENT_INDEX);
            break;

        case MAC_ACTIVITY_TX_BEACON:
            currPri = (activityData.numMissedBeaconTxFrames == 0) ?
                      CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_TX_BEACON, MAC_ACTIVITY_PRI_HIGH_INDEX) :
                      CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_TX_BEACON, MAC_ACTIVITY_PRI_URGENT_INDEX);
            break;

        case MAC_ACTIVITY_TX_DATA:
            if (activityData.numMissedDataTxFrames < MAC_ACTIVITY_THRESHOLD_PRI_NORMAL)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_TX_DATA, MAC_ACTIVITY_PRI_NORMAL_INDEX);
            }
            else if (activityData.numMissedDataTxFrames < MAC_ACTIVITY_THRESHOLD_PRI_HIGH)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_TX_DATA, MAC_ACTIVITY_PRI_HIGH_INDEX);
            }
            else
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_TX_DATA, MAC_ACTIVITY_PRI_URGENT_INDEX);
            }
            break;

        default:
            currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_TX_DATA, MAC_ACTIVITY_PRI_NORMAL_INDEX);
            break;
    }
    return currPri;
}

/*******************************************************************************
 * @fn          getActivityPriorityRx
 *
 * @brief       This call is used by the LMAC to calculate the activity priority
 *              index for the current activity.
 *              Note: The priority calculation will is based on a number of missed
 *              events. For most activities the priority will increase incrementally
 *              until the max priority is reached. This is synonymous to the amount
 *              of missed events.
 *              Ex) Every missed abort shall increase the priority, up to the maximum.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static uint32_t getActivityPriorityRx(void)
{
    uint32_t currPri;

    switch (activityData.rxActivity)
    {
        case MAC_ACTIVITY_RX_SCAN:
            if (activityData.numRxScanAbort < MAC_ACTIVITY_THRESHOLD_PRI_NORMAL)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_RX_SCAN, MAC_ACTIVITY_PRI_NORMAL_INDEX);
            }
            else if (activityData.numRxScanAbort < MAC_ACTIVITY_THRESHOLD_PRI_HIGH)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_RX_SCAN, MAC_ACTIVITY_PRI_HIGH_INDEX);
            }
            else
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_RX_SCAN, MAC_ACTIVITY_PRI_URGENT_INDEX);
            }
            break;

        case MAC_ACTIVITY_RX_POLLING:
            if (activityData.numRxPollAbort < MAC_ACTIVITY_THRESHOLD_PRI_NORMAL)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_RX_POLLING, MAC_ACTIVITY_PRI_NORMAL_INDEX);
            }
            else if (activityData.numRxPollAbort < MAC_ACTIVITY_THRESHOLD_PRI_HIGH)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_RX_POLLING, MAC_ACTIVITY_PRI_HIGH_INDEX);
            }
            else
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_RX_POLLING, MAC_ACTIVITY_PRI_URGENT_INDEX);
            }
            break;

        default:
            currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_RX_IDLE, MAC_ACTIVITY_PRI_NORMAL_INDEX);
            break;
    }

    return currPri;
}

/*******************************************************************************
 * @fn          setActivityTx
 *
 * @brief       This call is used by the LMAC to set the transmit activity.
 *
 * input parameters
 *
 * @param       commandId - Command ID of the transmit packet.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      macActivity_t Current activity.
 */
static void setActivityTx(uint8_t commandId)
{
    macActivity_t currActivity = MAC_ACTIVITY_TX_DATA;

    switch (commandId)
    {
        case MAC_CMD_ASSOCIATION_REQUEST:
        case MAC_CMD_ASSOCIATION_RESPONSE:
        case MAC_CMD_DISASSOCIATION_NOTIFICATION:
            currActivity = MAC_ACTIVITY_TX_LINK_EST;
            break;

        case MAC_CMD_BEACON_REQUEST:
            currActivity = MAC_ACTIVITY_TX_BEACON;
            break;

        case MAC_CMD_DATA_REQUEST:
        case MAC_CMD_ORPHAN_NOTIFICATION:
        case MAC_CMD_COORDINATOR_REALIGNMENT:
        case MAC_CMD_PAN_ID_CONFLICT_NOTIFICATION:
            currActivity = MAC_ACTIVITY_TX_DATA;
            break;

        default:
            currActivity = MAC_ACTIVITY_TX_DATA;
            break;
    }

    activityData.txActivity = currActivity;
}

/*******************************************************************************
 * @fn          setActivityRx
 *
 * @brief       This call is used by the LMAC to set the receive activity.
 *
 * input parameters
 *
 * @param       isPolling - Boolean indicating if polling is active.
 *
 * @param       isScanning - Boolean indicating if scanning is active.
 *
 * output parameters
 *
 * @param       None.
 *
 */
static void setActivityRx(bool isPolling, bool isScanning)
{
    macActivity_t currActivity = MAC_ACTIVITY_RX_IDLE;

    if (isScanning)
    {
        currActivity = MAC_ACTIVITY_RX_SCAN;
    }
    else if (isPolling)
    {
        currActivity = MAC_ACTIVITY_RX_POLLING;
    }

    activityData.rxActivity = currActivity;
}


/*******************************************************************************
 * @fn          getActivityTx
 *
 * @brief       This call is used by the LMAC to obtain the current transmit activity.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      macActivity_t Current activity.
 */
static macActivity_t getActivityTx(void)
{
    return activityData.txActivity;
}

/*******************************************************************************
 * @fn          getActivityRx
 *
 * @brief       This call is used by the LMAC to obtain the current receive activity.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 */
static macActivity_t getActivityRx(void)
{
    return activityData.rxActivity;
}

/********************************************************************************/
