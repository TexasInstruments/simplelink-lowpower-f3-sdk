/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated
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

 #ifndef MAC_ACTIVITY_H
 #define MAC_ACTIVITY_H
 
 /*******************************************************************************
  * INCLUDES
  */
 #include <stdint.h>
 #include <stdbool.h>
 /*******************************************************************************
  * CONSTANTS
  */
 /* MAC Activity priorities */
 typedef enum {
     MAC_ACTIVITY_PRI_NORMAL_INDEX = 0,
     MAC_ACTIVITY_PRI_HIGH_INDEX,
     MAC_ACTIVITY_PRI_URGENT_INDEX,
     NUM_MAC_ACTIVITY_PRI
 } macActivityPriority_t;
 
 #define MAC_ACTIVITY_THRESHOLD_PRI_NORMAL (1)
 #define MAC_ACTIVITY_THRESHOLD_PRI_HIGH   (2)
 #define MAC_ACTIVITY_THRESHOLD_PRI_URGENT (3)
 
 /* Priority threshold for RX BEACON Activity */
 #define MAC_ACTIVITY_THRESHOLD_REM_BEACON_NORMAL_PRI (3)
 #define MAC_ACTIVITY_THRESHOLD_REM_BEACON_HIGH_PRI (2)
 
 /* Default increment for preemption tracking */
 #define MAC_ACTIVITY_DEFAULT_INC (1)
 #define MAC_ACTIVITY_INCREMENT_FAILURE_COUNT false
 #define MAC_ACTIVITY_RESET_FAILURE_COUNT true
 
 /* MAC Activity types */
 typedef enum {
     MAC_ACTIVITY_TX_LINK_EST = 0x0001, /* Link establishment transmit activity */
     MAC_ACTIVITY_TX_BEACON,  /* Transmit beacon activity */
     MAC_ACTIVITY_TX_DATA,    /* Transmit data activity */
     MAC_ACTIVITY_RX_SCAN,    /* Receive scan activity */
     MAC_ACTIVITY_RX_POLLING, /* Polling activity */
     MAC_ACTIVITY_RX_IDLE,    /* Idle receive activity */
     NUM_MAC_ACTIVITY         /* Total number of activities */
 } macActivity_t;
 
 
 typedef struct {
     uint8_t numMissedLinkEstTxFrames; /* Number of missed link establishment packets. */
     uint8_t numMissedBeaconTxFrames; /* Number of missed beacon transmissions. */
     uint8_t numMissedDataTxFrames; /* Number of missed data transmissions. */
     uint8_t numRxScanAbort; /* Number of aborted scans. */
     uint8_t numRxPollAbort; /* Number of poll aborts. */
     macActivity_t txActivity; /* Current activity associated with the Transmitter */
     macActivity_t rxActivity; /* Current activity associated with the Receiver */
 } macActivityData_t;
 
 /* Activity tracking module function pointer typedef's. */
 typedef void (*setActivityTrackingTx_t)(bool resetCount);
 typedef void (*setActivityTrackingRx_t)(bool resetCount);
 typedef uint32_t (*getActivityPriorityTx_t)(void);
 typedef uint32_t (*getActivityPriorityRx_t)(void);
 typedef void (*setActivityTx_t)(uint8_t commandId);
 typedef void (*setActivityRx_t)(bool isPolling, bool isScanning);
 typedef macActivity_t (*getActivityTx_t)(void);
 typedef macActivity_t (*getActivityRx_t)(void);
 typedef bool (*getCoexActivityTx_t)(void);
 typedef bool (*getCoexActivityRx_t)(void);
 
 /* Activity tracking object type definition */
 typedef const struct _activityObject
 {
     setActivityTrackingTx_t pSetActivityTrackingTxFn;
     setActivityTrackingRx_t pSetActivityTrackingRxFn;
     getActivityPriorityTx_t pGetActivityPriorityTxFn;
     getActivityPriorityRx_t pGetActivityPriorityRxFn;
     setActivityTx_t pSetActivityTxFn;
     setActivityRx_t pSetActivityRxFn;
     getActivityTx_t pGetActivityTxFn;
     getActivityRx_t pGetActivityRxFn;
 } activityObject_t;
 
 
 /* ------------------------------------------------------------------------------------------------
  *                                          Macros
  * ------------------------------------------------------------------------------------------------
  */
 /* MAC Activity table priority MACRO */
 #define CALC_ACTIVITY_PRIORITY(ACTIVITY, PRIORITY) ((ACTIVITY << 16) | PRIORITY)
 
 /*******************************************************************************
  * LOCAL VARIABLES
  */
 
 /*******************************************************************************
  * GLOBAL VARIABLES
  */
 extern activityObject_t activityObject;
 
 /*******************************************************************************
  * APIs
  */
 
 /*******************************************************************************
  */
 
 #endif /* MAC_ACTIVITY_H */
