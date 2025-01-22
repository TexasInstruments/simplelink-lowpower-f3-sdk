/******************************************************************************

 @file  ll_handover.h

 @brief This file contains the data structures and APIs for handling
        Bluetooth Low Energy handover process

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
 *
 * WARNING!!!
 *
 * THE API'S FOUND IN THIS FILE ARE FOR INTERNAL STACK USE ONLY!
 * FUNCTIONS SHOULD NOT BE CALLED DIRECTLY FROM APPLICATIONS, AND ANY
 * CALLS TO THESE FUNCTIONS FROM OUTSIDE OF THE STACK MAY RESULT IN
 * UNEXPECTED BEHAVIOR.
 *
 */

#ifndef LL_HANDOVER_H
#define LL_HANDOVER_H

/*******************************************************************************
 * INCLUDES
 */
#include "hal_types.h"
#include "ll_common.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */
// Invalid Handover dats buffer size
#define INVALID_DATA_SIZE 0xFFFFFFFFU

// The size of the controller's data
#define LL_HANDOVER_CONTROLLER_DATA_SIZE 241U

/*******************************************************************************
 * TYPEDEFS
 */
// The controller's Handover data structure
typedef struct
{
  /* Link Command Parameters */
  uint16 phyFeatures;                            // PHY currently used for the connection
  uint16 seqStat;                                // SN/NESN saved in the command
  uint8  fraction:1;                             // TX power fraction value
  int8_t dBm:7;                                  // TX power dbm value
  /* General Connection Data */
  uint8  firstPacket;                            // flag to indicate when the first packet has been received
  uint16 currentEvent;                           // current event number
  uint8  nextChan;                               // The unmapped channel for the next active connection event
  uint8  reserved;                               // Reserved
  uint16 scaFactor;                              // SCA factor for timer drift calculation
  uint32 timeToHandoverEvent;                    // The time left to the next event
  uint16 expirationEvent;                        // event at which the LSTO has expired
  uint16 expirationValue;                        // number of events to a LSTO expiration
  uint32 lastTimeoutTime;                        // Last RX window timeout time used
  uint32 timerDrift;                             // saved timer drift adjustment to avoid recal
  uint32 accessAddr;                             // saved synchronization word to be used by Peripheral
  uint32 crcInit;                                // connection CRC initialization value (24 bits)
  uint16 peripheralLatency;                      // current peripheral latency; 0 means inactive
  uint8  sleepClkAccuracy;                       // peer's sleep clock accuracy; used by own device to determine timer drift
  /* Connection Parameters */
  uint8  winSize;                                // window size
  uint16 winOffset;                              // window offset
  uint16 connInterval;                           // connection interval
  uint16 curParamPeriLatency;                    // number of connection events the peripheral can ignore
  uint16 connTimeout;                            // supervision connection timeout
  /* Channel Map */
  uint8  currentChan;                            // the current unmapped channel for the completed connection event
  uint8  currentMappedChan;                      // Currently used mapped channel
  uint8  numUsedChans;                           // count of the number of usable data channels
  uint8  hopLength;                              // used for finding next data channel at next connection event
  uint8  chanMapTable[ LL_MAX_NUM_DATA_CHAN ];   // current chan map table that is in use for this connection //37 bytes
  uint8  chanMap[ LL_NUM_BYTES_FOR_CHAN_MAP ];   // bit map corresponding to the data channels 0..39 //5 Bytes
  /* Encryption Information */
  uint8  encEnabled;                             // flag to indicate that encryption is enabled for this connection
  uint8  IV[ LL_ENC_IV_LEN ];                    // combined master and slave IV values concatenated  //8 Bytes
  uint8  SKD [ LL_ENC_SKD_LEN ];                 // combined master and slave SKD values concatenated  // 16 bytes
  uint8  RAND[ LL_ENC_RAND_LEN ];                // random vector from Master // 8 Bytes
  uint8  EDIV[ LL_ENC_EDIV_LEN ];                // encrypted diversifier from Master  // 2 bytes
  uint8  nonce[ LL_ENC_NONCE_LEN ];              // current nonce with current IV value  13 Bytes
  uint8  SK[ LL_ENC_SK_LEN ];                    // session key derived from LTK and SKD  //16 bytes
  uint8  LTK[ LL_ENC_LTK_LEN ];                  // Long Term Key from Host  // 16 Bytes
  uint32 txPktCount;                             // used for nonce formation during encryption
  uint32 rxPktCount;                             // used for nonce formation during encryption
  /* Feature Set */
  uint8  featureSet[ LL_MAX_FEATURE_SET_SIZE ];  // The combination (logical AND) of this device's feature set and the peer's device feature set // 8 Bytes
  uint8  featureRspRcved;                        // indicates that the peer feature set was received (in Feature Request PDU)
  /* Version Information */
  uint8 peerInfoValid;                           // flag to indicate the peer's version information is valid
  uint8 hostRequest;                             // flag to indicate the host has requested the peer's version information
  uint8 verInfoSent;                             // flag to indicate this device's version information has been sent
  uint16 comId;                                  // company identifier
  uint16 subverNum;                              // implementation version
  uint8  verNum;                                 // controller spec version
  /* Peer Address Information */
  uint8  peerAddr[ LL_DEVICE_ADDR_LEN ];         // Peer address //6
  uint8  peerAddrType;                           // Peer address type
  /* Central Contribution */
  uint16 mstSCA;                                 // Central's portion of connection SCA
  /* QoS */
  uint8  connPriority;                           // connection priority
  /* Authenticated Payload Timeout  */
  uint8  numAptoExp;                             // number of 1/2 APTO expiration
  uint32 aptoValue;                              // APTO value, in ms
  /* Length Information */
  uint16 connMaxTxOctets;                        // max payload bytes sent by this device
  uint16 connMaxRxOctets;                        // max payload bytes this device can receive
  uint16 connMaxTxTime;                          // max time (us) this device will take to transmit payload
  uint16 connMaxRxTime;                          // max time (us) this device can take to receive payload
  uint16 connRemoteMaxTxOctets;                  // max payload bytes peer device will send
  uint16 connRemoteMaxRxOctets;                  // max payload bytes peer can receive
  uint16 connRemoteMaxTxTime;                    // max time (us) peer will take to transmit
  uint16 connRemoteMaxRxTime;                    // max time (us) peer can take to receive
  uint16 connIntervalPortionAvail;               // the current connection interval - C (see spec)
  uint8  lenFlags;                               // control procedure flags
  uint8  connSlowestPhy;                         // slowest PHY limits connRemoteMaxTxTime
  /* PHY Information */
  uint8  curPhy;                                 // current PHY
  uint8  phyFlags;                               // control procedure flags
  uint8  phyPreference;                          // based on set PHY
  uint8  phyOpts;                                // Coded phy options
  uint8  chanSelAlgo;                            // Channel selection algorithm
} handoverDataFull_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

#endif /* LL_HANDOVER_H */
