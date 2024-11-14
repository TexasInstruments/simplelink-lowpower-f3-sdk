/******************************************************************************

 @file  ull.c

 @brief This file contains the Micro Link Layer (uLL) API for the Micro
        BLE Stack.

        This API is mostly based on the Bluetooth Core Specification,
        V4.2, Vol. 6.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2024, Texas Instruments Incorporated

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

/*********************************************************************
 * INCLUDES
 */

#include <stdlib.h>
#include <string.h>
#include <icall.h>
#include <ll_common.h>
#include <bcomdef.h>

#include <port.h>
#include <uble.h>
#include <urcli.h>
#include <ull.h>

#include <ti/drivers/rcl/RCL_Scheduler.h>
#include <ti/drivers/rcl/RCL_Buffer.h>
#include <ti/drivers/rcl/LRFCC23X0.h>

#include <drivers/dpl/HwiP.h>
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/* uLL State */
#define ULL_STATE_STANDBY               0
#define ULL_STATE_ADVERTISING           1
#define ULL_STATE_SCANNING              2
#define ULL_STATE_MONITORING            3

/* uLL monitor rx status */
#define ULL_MONITOR_RX_SCHEDULED        1
#define ULL_MONITOR_RX_NO_RF_RESOURCE   2
#define ULL_MONITOR_RX_FAILED           3

/* uLL monitor modes */
#define ULL_MONITOR_MODE_START          1
#define ULL_MONITOR_MODE_RESCHEDULE     2

/* RCL command start time ramp up */
#define ULL_RX_RAMPUP_TIME_RAT          800
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
extern bStatus_t uble_buildAndPostEvt(ubleEvtDst_t evtDst, ubleEvt_t evt,
                                      uint8 *pMsg, uint16 len);

/*********************************************************************
 * EXTERNAL VARIABLES
 */

extern ubleParams_t ubleParams;

#if defined(FEATURE_MONITOR)
extern RCL_CommandStatus    urcliGenericRxHandle;
extern RCL_CmdBle5GenericRx urcliGenericRxCmd;
extern RCL_CtxGenericRx     urcliCtxGenericRx;
extern RCL_Handle           urcliHandle;

#endif
/*********************************************************************
 * LOCAL VARIABLES
 */

#if defined(FEATURE_MONITOR)
/* Monitor callbacks */
static pfnMonitorIndCB_t ull_notifyMonitorIndication;
static pfnMonitorCompCB_t ull_notifyMonitorComplete;

// RX Data
typedef struct
{
  RCL_MultiBuffer *dataBuffers[ULL_NUM_RX_MONITOR_ENTRIES];
  List_List        multiBuffers;
  List_List        finishedBuffers;
  uint16           length;                           // buffer length
} rxDataQ_t;

/** @data structure for the thread entity */
typedef struct
{
  /* Monitor Session ID */
  uint8_t       sessionId;
  /* Micro Link Layer State */
  uint8         state;
  /* Monitor status */
  uint8         monitorRxStatus;
  // Queue to handle CM RX Data
  rxDataQ_t     queue;
  // number of the rx entry done in the current session
  uint8         numRxSuccess;
} ull_CmData_t;

static ull_CmData_t gull_CmData =
{
  .state            = ULL_STATE_STANDBY,
  .monitorRxStatus  = ULL_MONITOR_RX_SCHEDULED,
  .queue            = {0},
  .numRxSuccess     = 0,
};

#endif /* FEATURE_MONITOR */

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

#if defined(FEATURE_MONITOR)
/*******************************************************************************
 * @fn          ull_setupMonitorDataEntryQueue
 *
 * @brief       This routine is used to setup a RX Buffers for the RCL.
 *
 * input parameters
 *
 * @param
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void ull_setupMonitorDataEntryQueue( void )
{
  // set the receiving buffer size
  gull_CmData.queue.length = sizeof(RCL_MultiBuffer) + sizeof(RCL_Buffer_DataEntry) +
                       RCL_BUFFER_MAX_HEADER_PAD_BYTES +
                       ULL_BLE_ALIGNED_BUFFER_SIZE  + LL_PKT_MIC_LEN;

  /* Prepare list of RX buffers */
  List_clearList(&gull_CmData.queue.multiBuffers);
  /* Prepare list of RX buffers that are done */
  List_clearList(&gull_CmData.queue.finishedBuffers);

  // set the Scan receive buffers
  for (uint8 i = 0; i < ULL_NUM_RX_MONITOR_ENTRIES; i++)
  {
    volatile port_key_t key;

    key = port_enterCS_HW();
    gull_CmData.queue.dataBuffers[i] = (RCL_MultiBuffer *) malloc( gull_CmData.queue.length );
    port_exitCS_HW(key);

    if (gull_CmData.queue.dataBuffers[i] != NULL)
    {
      RCL_MultiBuffer_init(gull_CmData.queue.dataBuffers[i],  gull_CmData.queue.length );
      RCL_MultiBuffer_put(&gull_CmData.queue.multiBuffers, gull_CmData.queue.dataBuffers[i]);
    }
    else
    {
      return;
    }
  }

  urcliCtxGenericRx.rxBuffers.head = gull_CmData.queue.multiBuffers.head;
  urcliCtxGenericRx.rxBuffers.tail = gull_CmData.queue.multiBuffers.tail;

  return ;
}
#endif /* FEATURE_MONITOR */
/*******************************************************************************
 * @fn          ull_ClearRxDataEntry API
 *
 * @brief       This function is used to clear the RX data entry so that the radio
 *              can once again use it. It should be called after the user has processed the
 *              data entry.
 *
 * input parameters
 *
 * @param       rxDataQ_t - Pointer to data queue.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */

void ull_ClearRxDataEntry( rxDataQ_t *pQueue)
{
  RCL_MultiBuffer *multiBuffer;

  while ((multiBuffer = RCL_MultiBuffer_get(&pQueue->finishedBuffers)) != NULL)
  {
    RCL_MultiBuffer_clear(multiBuffer);
    RCL_MultiBuffer_put(&pQueue->multiBuffers, multiBuffer);
  }

  // Prepare list of RX buffers that are done
  List_clearList(&pQueue->finishedBuffers);

}

#if defined(FEATURE_MONITOR)

/*******************************************************************************
 * @fn          ull_flushAllDataEntry
 *
 * @brief       This function is used to mark the all System data entry on a
 *              data entry queue as Pending so that the radio can once again
 *              use all available data entry queue. It should be called after
 *              the user has processed after the Rx buffer full is reported.
 *              NOTE: This function should not be used by the application.
 *
 * input parameters
 *
 * @param       dataEntryQueue_t - Pointer to data entry queue.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static void ull_flushAllDataEntry( rxDataQ_t *pQueue )
{

  while (RCL_MultiBuffer_RxEntry_get(&pQueue->multiBuffers, &pQueue->finishedBuffers) != NULL)
  {
  }

  ull_ClearRxDataEntry(pQueue);

  urcliCtxGenericRx.rxBuffers.head = gull_CmData.queue.multiBuffers.head;
  urcliCtxGenericRx.rxBuffers.tail = gull_CmData.queue.multiBuffers.tail;

}

#endif /* FEATURE_MONITOR */

/*********************************************************************
 * CALLBACKS
 */




#if defined(FEATURE_MONITOR)
/*********************************************************************
 * @fn      ull_monitorDoneCb
 *
 * @brief   Callback function to be invoked by RF driver
 *
 * @param   rfHandle - RF client handle
 *
 * @param   cmdHandle - RF command handle
 *
 * @param   events - RF events
 *
 * @return  none
 */

void ull_monitorDoneCb(RCL_Command *cmd, LRF_Events lrfEvents, RCL_Events rclEvents)
{
  if (rclEvents.rxEntryAvail)
  {
    if (gull_CmData.monitorRxStatus == ULL_MONITOR_RX_SCHEDULED)
    {
      /* Radio has received a packet. Process it and eventually post the result to the monitor app task */
      uble_buildAndPostEvt(UBLE_EVTDST_LL, ULL_EVT_MONITOR_RX_SUCCESS, NULL, 0);
      gull_CmData.numRxSuccess++;
    }
  }

  /* rxEntryAvail and lastCmdDone can be set at the same time */
  if (rclEvents.lastCmdDone)
  {
    if (gull_CmData.monitorRxStatus == ULL_MONITOR_RX_SCHEDULED)
    {
      switch(urcliGenericRxCmd.common.status)
      {
        case RCL_CommandStatus_Finished:
        case RCL_CommandStatus_Idle:
        case RCL_CommandStatus_RxTimeout:
        case RCL_CommandStatus_NoSync:
        case RCL_CommandStatus_RxErr:
        case RCL_CommandStatus_HardStopApi:
        case RCL_CommandStatus_HardStopTimeout:
        case RCL_CommandStatus_HardStopScheduling:
        case RCL_CommandStatus_GracefulStopApi:
        case RCL_CommandStatus_GracefulStopTimeout:
        case RCL_CommandStatus_GracefulStopScheduling:
        case RCL_CommandStatus_Error_StartTooLate:
        case RCL_CommandStatus_Error_AlreadySubmitted:
          /* Operation finished normally or past start trigger.
           * Clear the radio buffers and schedule the next command */
          uble_buildAndPostEvt(UBLE_EVTDST_LL, ULL_EVT_MONITOR_RX_WINDOW_COMPLETE, NULL, 0);
          break;

        case RCL_CommandStatus_Error_RxFifo:
          /* No available Rx buffer */
          uble_buildAndPostEvt(UBLE_EVTDST_LL, ULL_EVT_MONITOR_RX_BUF_FULL, NULL, 0);
          break;

        default:
          /* Do not allow further monitoring scans */
          gull_CmData.state = ULL_STATE_STANDBY;

          /* Synth error or other fatal errors */
          uble_buildAndPostEvt(UBLE_EVTDST_LL, ULL_EVT_MONITOR_RX_FAILED, NULL, 0);
          break;
      }
    }
  }
}

#endif /* FEATURE_MONITOR */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn     ull_init
 *
 * @brief  Initialization function for the Micro Link Layer.
 *
 * @param  none
 *
 * @return SUCCESS or FAILURE
 */
bStatus_t ull_init(void)
{
  if (gull_CmData.state != ULL_STATE_STANDBY)
  {
    /* Cannot re-initialize if running */
    return FAILURE;
  }

  /* Initialize RF Interface */
  if (urcli_init() != SUCCESS)
  {
    return FAILURE;
  }


  return SUCCESS;
}


#if defined(FEATURE_MONITOR)
/*********************************************************************
 * @fn     ull_monitorRegisterCB
 *
 * @brief  Register callbacks supposed to be called by Monitor.
 *
 * @param  pfnMonitorIndicationCB - callback to nofity the application that a
 *                                  packet has been received.
 * @param  pfnMonitorCompleteCB - callback to post-process monitor duration complete
 *
 * @return  none
 */
void ull_monitorRegisterCB(pfnMonitorIndCB_t pfnMonitorIndicationCB,
                           pfnMonitorCompCB_t pfnMonitorCompleteCB)
{
  ull_notifyMonitorIndication = pfnMonitorIndicationCB;
  ull_notifyMonitorComplete = pfnMonitorCompleteCB;
}
#endif /* FEATURE_MONITOR */

/*********************************************************************
 * LOCAL FUNCTIONS
 */



#if defined(FEATURE_MONITOR)
/*********************************************************************
 * @fn      ull_monitorSchedule
 *
 * @brief   Update Generic RX command with latest parameters and schedule
 *          the radio operation
 *
 * @param   mode   -  if ULL_MONITOR_MODE_START, start the scanning and schedule
 *                    the operation immediately. ULL_MONITOR_MODE_RESCHEDULE,
 *                    reschedules the operation at the same previous schedule.
 *
 * @return  SUCCESS - the radio operation has been successfully scheduled
 *          FAILURE - failed to schedule the radio operation due to RF driver
 *                    error
 */
bStatus_t ull_monitorSchedule(uint8 mode)
{
  RCL_StopType stopType = RCL_StopType_Graceful;
  ull_flushAllDataEntry( &gull_CmData.queue );
  gull_CmData.numRxSuccess = 0;

  if (mode == ULL_MONITOR_MODE_START)
  {
    urcliGenericRxCmd.common.scheduling = RCL_Schedule_AbsTime;
    urcliGenericRxCmd.common.timing.absStartTime = ubleParams.startTime /* + LL_SCHED_START_IMMED_PAD*/;

    if (ubleParams.monitorDuration > 0)
    {
      urcliGenericRxCmd.common.timing.relGracefulStopTime = ubleParams.monitorDuration * BLE_TO_RAT;
    }
    else
    {
      /* Continuous monitoring */
      urcliGenericRxCmd.common.timing.relGracefulStopTime = 0;
      urcliGenericRxCmd.common.timing.relHardStopTime = 0;
    }
  }
  else
  {
    /* ULL_MONITOR_MODE_RESCHEDULE */
    urcliGenericRxCmd.common.scheduling = RCL_Schedule_Now;
    urcliGenericRxCmd.common.timing.absStartTime = 0;

    if (ubleParams.monitorDuration > 0)
    {
      if (urcliGenericRxCmd.common.timing.relGracefulStopTime > ubleParams.monitorDuration * BLE_TO_RAT)
      {
        /* shorten the end time */
          urcliGenericRxCmd.common.timing.relGracefulStopTime -= ubleParams.monitorDuration * BLE_TO_RAT;
      }
      else
      {
        /* past the end time */
        stopType = RCL_StopType_Hard; /* End ASAP */
      }
    }
    else
    {
      /* Continuous monitoring */
      urcliGenericRxCmd.common.timing.relGracefulStopTime = 0;
      urcliGenericRxCmd.common.timing.relHardStopTime = 0;
    }
  }

  /* clear command status values */
  urcliGenericRxCmd.common.status = RCL_CommandStatus_Idle;

  /* Note: cannot do wildcard access address */
  urcliGenericRxCmd.ctx->accessAddress = ubleParams.accessAddr;

  /* Note: for crc check and rssi readings to work we must set a valid crcInit value */
  urcliGenericRxCmd.ctx->crcInit = ubleParams.crcInit;

  urcliGenericRxCmd.channel = ubleParams.monitorChan;

  urcliGenericRxCmd.common.runtime.callback = ull_monitorDoneCb;
  urcliGenericRxCmd.common.runtime.lrfCallbackMask.value = LRF_EventRxOk.value;
  urcliGenericRxCmd.common.runtime.rclCallbackMask.value = RCL_EventLastCmdDone.value |
                                                           RCL_EventRxEntryAvail.value |
                                                           RCL_EventRxBufferFinished.value |
                                                           RCL_EventGracefulStop.value |
                                                           RCL_EventHardStop.value |
                                                           RCL_EventStartRejected.value;

  // Check if the start time requested is too close to the current time or it is in the past,
  // if it meet the condition => ask the stack to schedule the next request
  // NOTE - if we didn't take into consideration the ULL_RX_RAMPUP_TIME_RAT, the RCL_Command_submit will return error code.
  if (uble_timeCompare(ull_getCurrentTime(), ubleParams.startTime - ULL_RX_RAMPUP_TIME_RAT))
  {
     uble_buildAndPostEvt(UBLE_EVTDST_LL, ULL_EVT_MONITOR_CONTINUE, NULL, 0);
     return FAILURE;
  }

  // post the command
  urcliGenericRxHandle = RCL_Command_submit(urcliHandle, (RCL_Command_Handle)&urcliGenericRxCmd);

  if (stopType != RCL_StopType_Graceful)
  {
    RCL_Command_stop(&urcliGenericRxCmd, stopType);
  }

  if(urcliGenericRxHandle > RCL_CommandStatus_Error)
  {
    uble_buildAndPostEvt(UBLE_EVTDST_LL, ULL_EVT_MONITOR_CONTINUE, NULL, 0);
  }

  /* Save local session Id */
  gull_CmData.sessionId = ubleParams.monitorHandle;

  gull_CmData.monitorRxStatus = ULL_MONITOR_RX_SCHEDULED;

  return SUCCESS;
}

/*********************************************************************
 * @fn      ull_monitorStart
 *
 * @brief   Enter ULL_STATE_MONITORING
 *
 * @param   channel - monitoring scan channel
 *
 * @return  SUCCESS - Successfully entered ULL_STATE_MONITORING
 *          FAILURE - Failed to enter ULL_STATE_MONITORING
 */
bStatus_t ull_monitorStart(uint8_t channel)
{
  bStatus_t status = FAILURE;

  /* Update scan parameters */
  ubleParams.monitorChan = channel;

  gull_CmData.state = ULL_STATE_MONITORING;

  /* Post an ul event */
  status = uble_buildAndPostEvt(UBLE_EVTDST_LL, ULL_EVT_MONITOR_RX_STARTED, NULL, 0);

  return (status);
}

/*********************************************************************
 * @fn      ull_monitorStop
 *
 * @brief   Exit ULL_STATE_MONITORING
 *
 * @param   none
 *
 * @return  none
 */
void ull_monitorStop(void)
{
  if (gull_CmData.state == ULL_STATE_MONITORING)
  {
    gull_CmData.state = ULL_STATE_STANDBY;

    /* Cancel or stop generic Rx command */
    if (RCL_CommandStatus_Idle != urcliGenericRxHandle)
    {
        /* flush RF commands */
        RCL_Command_stop(&urcliGenericRxCmd, RCL_StopType_Graceful);
    }
  }
}

/*******************************************************************************
 * @fn          ull_getPDU
 *
 * @brief       This function is used to retrieve the Rx PDU.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       len      - Pointer to payload length.
 * @param       pPayload  - Pointer to payload.
 *
 * @return      None.
 */
void ull_getPDU( uint8 *len, uint8 *pPayload, RCL_Buffer_DataEntry *pDataEntry )
{
  int8_t rssi = RCL_BLE5_getRxRssi(pDataEntry);
  uint32_t timestamp = RCL_BLE5_getRxTimestamp(pDataEntry);

  /* read length, add postfix length, exclude CRC length  */
  *len = ULL_SUFFIX_RSSI_SIZE + ULL_SUFFIX_TIMESTAMP_SIZE + DUMMY_PAYLOAD_SIZE;

  /* We build the following payload:
     Note: We keep the dummy in order to be aligned with Agama implementation
   * | RSSI   | Dummy  | TimeStamp |
   * | 1 byte | 1 byte | 4 bytes   |
   */

  /* The memcpy is used here to avoid memory alignment issues (ie in FREERTOS) */
  memcpy(pPayload, &rssi, ULL_SUFFIX_RSSI_SIZE);
  memcpy(pPayload + ULL_SUFFIX_RSSI_SIZE + DUMMY_PAYLOAD_SIZE, &timestamp, ULL_SUFFIX_TIMESTAMP_SIZE);
}

/*********************************************************************
 * @fn      ull_rxEntryDoneCback
 *
 * @brief   Callback to send the received packet up.
 *
 * @param   none
 *
 * @return  none
 */
void ull_rxEntryDoneCback(void)
{
  uint8 dataLen;
  volatile uint32_t keyHwi;

  keyHwi = HwiP_disable();
  bool isEmpty = RCL_MultiBuffer_RxEntry_isEmpty(&gull_CmData.queue.multiBuffers);
  HwiP_restore(keyHwi);
  if (isEmpty)
  {
    /* nothing to do here */
    return;
  }

  RCL_Buffer_DataEntry *pDataEntry = NULL;
  keyHwi = HwiP_disable();
  uint8 *pktInfo = malloc( ULL_SUFFIX_MAX_SIZE);
  HwiP_restore(keyHwi);

  if (NULL == pktInfo)
  {
      return;
  }

  /* get pointer to packet */
  keyHwi = HwiP_disable();
  pDataEntry = RCL_MultiBuffer_RxEntry_get(&gull_CmData.queue.multiBuffers, &gull_CmData.queue.finishedBuffers);
  HwiP_restore(keyHwi);

  if ( NULL != pDataEntry)
  {
    /* process RX FIFO data */
    ull_getPDU( &dataLen, pktInfo, pDataEntry);
    if (dataLen != 0)
    {
      if (ull_notifyMonitorIndication)
      {

        ull_notifyMonitorIndication( SUCCESS, gull_CmData.sessionId, dataLen, pktInfo );
      }
    }
    else
    {
      keyHwi = HwiP_disable();
      free(pktInfo);
      HwiP_restore(keyHwi);
    }
  }
  else
  {
    keyHwi = HwiP_disable();
    free(pktInfo);
    HwiP_restore(keyHwi);
  }

  return;
}
#endif /* FEATURE_MONITOR */
/*********************************************************************
 * @fn      uble_processLLMsg
 *
 * @brief   Process event messages sent from Micro BLE Stack to uLL through
 *          the application's Stack Event Proxy and uble_processMsg().
 *
 * @param   pEvtMsg - Pointer to the Micro BLE Stack event message destined to
 *                    uLL.
 *
 * @return  none
 */
void uble_processLLMsg(ubleEvtMsg_t *pEvtMsg)
{
  switch (pEvtMsg->hdr.evt)
  {
#if defined(FEATURE_MONITOR)
  case ULL_EVT_MONITOR_RX_SUCCESS:
    /* Monitoring scan Rx has received a packet */
    ull_rxEntryDoneCback();
    break;

  case ULL_EVT_MONITOR_RX_FAILED:
    /* Monitoring scan rx failure. This is caused by radio failures,
     * not preemption eventhough the gull_CmData.monitorRxStatus is the same.
     */
    gull_CmData.monitorRxStatus = ULL_MONITOR_RX_NO_RF_RESOURCE;
    if (ull_notifyMonitorComplete)
    {
      ull_notifyMonitorComplete(bleNoResources, gull_CmData.sessionId);
    }
    break;

  case ULL_EVT_MONITOR_RX_BUF_FULL:
    /* Monitoring scan Rx buffer full */
    gull_CmData.monitorRxStatus = ULL_MONITOR_RX_NO_RF_RESOURCE;
    if (ull_notifyMonitorIndication)
    {
       ull_notifyMonitorIndication(MSG_BUFFER_NOT_AVAIL, gull_CmData.sessionId, 0, NULL);
    }
    /* flush RX queue data entries */
    ull_flushAllDataEntry( &gull_CmData.queue );

    /* reschedule the monitoring scan */
    ull_monitorSchedule(ULL_MONITOR_MODE_RESCHEDULE);
    break;

  case ULL_EVT_MONITOR_RX_WINDOW_COMPLETE:
  {
    /* Monitoring scan rx window complete. */
    if (ull_notifyMonitorComplete)
    {
      if (gull_CmData.numRxSuccess == 2)
      {
        ull_notifyMonitorComplete(MONITOR_SUCCESS, gull_CmData.sessionId);
      }
      else
      {
        ull_notifyMonitorComplete(MONITOR_UNSTABLE, gull_CmData.sessionId);
      }
    }
  }
  break;

  case ULL_EVT_MONITOR_RX_RADIO_AVAILABLE:
    /* Rf radio resource available. This is caused by PHY preemption.
     */
      /* reschedule the monitoring scan */
      ull_monitorSchedule(ULL_MONITOR_MODE_RESCHEDULE);
    break;

  case ULL_EVT_MONITOR_RX_STARTED:
      gull_CmData.monitorRxStatus = ULL_MONITOR_RX_SCHEDULED;
      ull_monitorSchedule(ULL_MONITOR_MODE_START);
    break;

  case ULL_EVT_MONITOR_CONTINUE:
  {
    if (ull_notifyMonitorComplete)
    {
      ull_notifyMonitorComplete(MONITOR_CONTINUE, gull_CmData.sessionId);
    }
  }
      break;
#endif /* FEATURE_MONITOR */

  default:
    break;
  }
}

uint32_t ull_getCurrentTime()
{
  return( RCL_Scheduler_getCurrentTime() );
}

uint32_t ull_convertRatTicksToUs(uint32_t time)
{
  return( RCL_SCHEDULER_SYSTIM_US(time) );
}

/*********************************************************************
*********************************************************************/
