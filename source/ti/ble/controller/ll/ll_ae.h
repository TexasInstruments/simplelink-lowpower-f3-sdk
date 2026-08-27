/******************************************************************************

 @file  ll_ae.h

 @brief This file contains the data structures and APIs for CC26xx
        RF Core Firmware Specification for Bluetooth Low Energy.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2026, Texas Instruments Incorporated

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

#ifndef LL_AE_H
#define LL_AE_H

/*******************************************************************************
 * INCLUDES
 */

#include <stdint.h>
#include <ti/drivers/RCL.h>
#include <ti/drivers/rcl/commands/ble5.h>
#include "ti/ble/controller/ll/ll_ble.h"
#include "ti/ble/controller/ll/ll_al.h"
#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_config.h"
#include "ti/ble/controller/ll/ll_tx_power.h"

/*******************************************************************************
 * HCI AE API
 */

/*******************************************************************************
 * CONSTANTS
 */

/*
** LE API Parameters
*/

// Extended Advertising Tx Power
#define AE_TX_POWER_NO_PREFERENCE                           127       // valid values: -127..+126

// RSSI not available indicator (per BLE Core Spec)
#define AE_RSSI_NOT_AVAILABLE                               127       // 0x7F indicates RSSI not available

// Maximum valid advertising handle value (BLE Core Spec range: 0x00 to 0xEF)
#define ADV_HANDLE_MAX                                      0xEF

// Extended Advertising Interval
#define AE_INTERVAL_MIN                                     32        // 20ms in 625us
#define AE_INTERVAL_MAX                                     768000    // 480,000ms in 625us (=480000/0.625).

// Extended Advertising Max Skip and Duration
#define AE_MAX_SKIP_DEFAULT                                 0
#define AE_DURATION_DISABLED                                0

// Extended Advertising Primary PHY
// TI Enhancement - Use as the MSB of the PHY parameter!
#define AE_PHY_NONE                                         0x00
#define AE_PHY_1_MBPS                                       0x01
#define AE_PHY_2_MBPS                                       0x02
#define AE_PHY_CODED                                        0x03
#define AE_PHY_CODED_S8                                     0x03
#define AE_PHY_CODED_S2                                     0x83
//
#define AE_PHY_CODED_SCHEME_MASK                            0x7F

// Secondary Advertising Max Skip
#define AE_MAX_AUX_SKIP_NONE                                0x00

// Advertising SID
#define AE_SID_0                                            0
#define AE_SID_1                                            1
#define AE_SID_2                                            2
#define AE_SID_3                                            3
#define AE_SID_4                                            4
#define AE_SID_5                                            5
#define AE_SID_6                                            6
#define AE_SID_7                                            7
#define AE_SID_8                                            8
#define AE_SID_9                                            9
#define AE_SID_10                                           10
#define AE_SID_11                                           11
#define AE_SID_12                                           12
#define AE_SID_13                                           13
#define AE_SID_14                                           14
#define AE_SID_15                                           15

// Advertising Scan Request Notification Flag
#define AE_NOTIFY_DISABLE_SCAN_REQUEST                      ~BV(0)
#define AE_NOTIFY_ENABLE_SCAN_REQUEST                        BV(0)

// Vendor Specific Notify Flags
#define AE_NOTIFY
#define AE_NOTIFY_DISABLE_ADV_SET_START                     ~BV(4)
#define AE_NOTIFY_ENABLE_ADV_SET_START                       BV(4)
#define AE_NOTIFY_DISABLE_ADV_START                         ~BV(5)
#define AE_NOTIFY_ENABLE_ADV_START                           BV(5)
#define AE_NOTIFY_DISABLE_ADV_END                           ~BV(6)
#define AE_NOTIFY_ENABLE_ADV_END                             BV(6)
#define AE_NOTIFY_DISABLE_ADV_SET_END                       ~BV(7)
#define AE_NOTIFY_ENABLE_ADV_SET_END                         BV(7)

// Advertising Halt Flags
#define AE_HALTED_FALSE                                     ~BV(0)
#define AE_HALTED_TRUE                                       BV(0)
#define AE_DISABLE_DISCONNECT_AUTH_FAIL                     ~BV(1)
#define AE_ENABLE_DISCONNECT_AUTH_FAIL                       BV(1)
/*
** Extended Advertiser
*/

#define AE_MAX_NUM_SID                                      16
#define AE_MAX_ADV_SETS                                     254
#define AE_MAX_ADV_SETS_TO_ENABLE_DISABLE                   63

#define AE_MAX_ADV_DATA_LEN                                 1650
#define AE_MAX_SCAN_RSP_DATA_LEN                            1650
#define AE_MAX_NUM_ADV_SETS                                 20

#define AE_MAX_ADV_PAYLOAD_LEN                              255
#define AE_DEFAULT_ADV_DATA_LEN                             AE_MAX_ADV_DATA_LEN
#define AE_DEFAULT_SCAN_RSP_DATA_LEN                        AE_MAX_SCAN_RSP_DATA_LEN
#define AE_DEFAULT_NUM_ADV_SETS                             AE_MAX_NUM_ADV_SETS

// values for Tx buffers
#define AE_NUM_TX_BUFFERS                                   3
#define PERIODIC_NUM_TX_BUFFERS                             2

// Advertising Event Properties Bit Number
// Byte 0
#define AE_PROP_CONNECTABLE_ADVERTISING                     0
#define AE_PROP_SCANNABLE_ADVERTISING                       1
#define AE_PROP_DIRECTED_ADVERTISING                        2
#define AE_PROP_HDC_DIRECTED_CONNECTABLE_ADVERTISING        3 // <=3.75ms Adv Interval
#define AE_PROP_LEGACY_PDU                                  4
#define AE_PROP_OMIT_ADVERTISERS_ADDRESS_FROM_ALL_PDUS      5 // anonymous advertising
#define AE_PROP_INCLUDE_TX_POWER_IN_EXTENDED_HEADER         6
#define AE_PROP_USE_DECISION_PDU                            7
// Byte 1
#define AE_PROP_INCLUDE_ADVA_IN_EXT_HEADER_DECISION_PDU     0
#define AE_PROP_INCLUDE_ADI_IN_EXT_HEADER_DECISION_PDU      1

// Advertising Event Properties
#define AE_PROPS_CONN_ADV                                   BV(AE_PROP_CONNECTABLE_ADVERTISING)
#define AE_PROPS_SCAN_ADV                                   BV(AE_PROP_SCANNABLE_ADVERTISING)
#define AE_PROPS_DIR_ADV                                    BV(AE_PROP_DIRECTED_ADVERTISING)
#define AE_PROPS_HDC_DIR_ADV                                BV(AE_PROP_HDC_DIRECTED_CONNECTABLE_ADVERTISING)
#define AE_PROPS_LEGACY                                     BV(AE_PROP_LEGACY_PDU)
#define AE_PROPS_OMIT_ADVA                                  BV(AE_PROP_OMIT_ADVERTISERS_ADDRESS_FROM_ALL_PDUS)
#define AE_PROPS_TX_POWER                                   BV(AE_PROP_INCLUDE_TX_POWER_IN_EXTENDED_HEADER)
#define AE_PROPS_DECISION_PDU                               BV(AE_PROP_USE_DECISION_PDU)
#define AE_PROPS_ADVA_DECISION_PDU                          BV(AE_PROP_INCLUDE_ADVA_IN_EXT_HEADER_DECISION_PDU)
#define AE_PROPS_ADI_DECISION_PDU                           BV(AE_PROP_INCLUDE_ADI_IN_EXT_HEADER_DECISION_PDU)

// Advertising Event Properties for Legacy PDUs
#define AE_EVT_TYPE_CONN_SCAN_ADV_IND                       AE_PROPS_LEGACY      | \
                                                            AE_PROPS_SCAN_ADV    | \
                                                            AE_PROPS_CONN_ADV    // 0x13: Supported if there's Advertising Data (31 bytes max)
#define AE_EVT_TYPE_CONN_DIR_LDC_ADV_DIRECT_IND             AE_PROPS_LEGACY      | \
                                                            AE_PROPS_DIR_ADV     | \
                                                            AE_PROPS_CONN_ADV    // 0x15: Not supported if there's Advertising Data
#define AE_EVT_TYPE_CONN_DIR_HDC_ADV_DIRECT_IND             AE_PROPS_LEGACY      | \
                                                            AE_PROPS_HDC_DIR_ADV | \
                                                            AE_PROPS_DIR_ADV     | \
                                                            AE_PROPS_CONN_ADV    // 0x1D: Not supported if there's Advertising Data
#define AE_EVT_TYPE_SCAN_UNDIR_ADV_SCAN_IND                 AE_PROPS_LEGACY      | \
                                                            AE_PROPS_SCAN_ADV    // 0x12: Supported if there's Advertising Data (31 bytes max)
#define AE_EVT_TYPE_NONCONN_NONSCAN_ADV_NONCONN_IND         AE_PROPS_LEGACY      // 0x10: Supported if there's Advertising Data (31 bytes max)

// Advertising Mode
#define AE_ADV_MODE_NONCONN_NONSCAN                         0
#define AE_ADV_MODE_CONNECTABLE                             1
#define AE_ADV_MODE_SCANNABLE                               2
#define AE_ADV_MODE_RESERVED                                3
//
#define ADV_MODE_MASK                                       0xC0
#define ADV_MODE_BIT_OFFSET                                 6

// Extended Header Flag Bits
#define EXTHDR_FLAG_ADVA_BIT                                0
#define EXTHDR_FLAG_TARGETA_BIT                             1
#define EXTHDR_FLAG_CTEINFO_BIT                             2
#define EXTHDR_FLAG_ADI_BIT                                 3
#define EXTHDR_FLAG_AUXPTR_BIT                              4
#define EXTHDR_FLAG_SYNCINFO_BIT                            5
#define EXTHDR_FLAG_TXPWR_BIT                               6
#define EXTHDR_FLAG_RFU2_BIT                                7
//
#define EXTHDR_INFO_SIZE                                    1
#define EXTHDR_FLAGS_SIZE                                   1
//
#define EXTHDR_FLAG_ADVA_SIZE                               6
#define EXTHDR_FLAG_TARGETA_SIZE                            6
#define EXTHDR_FLAG_CTEINFO_SIZE                            1
#define EXTHDR_FLAG_ADI_SIZE                                2
#define EXTHDR_FLAG_AUXPTR_SIZE                             3
#define EXTHDR_FLAG_SYNCINFO_SIZE                           18
#define EXTHDR_FLAG_TXPWR_SIZE                              1
#define EXTHDR_FLAG_RFU2_SIZE                               1
#define ACAD_CHAN_MAP_SIZE                                  9 // Size for periodic channel map update ACAD
//
#define EXTHDR_FLAG_ADVA                                    BV(EXTHDR_FLAG_ADVA_BIT)
#define EXTHDR_FLAG_TARGETA                                 BV(EXTHDR_FLAG_TARGETA_BIT)
#define EXTHDR_FLAG_CTEINFO                                 BV(EXTHDR_FLAG_CTEINFO_BIT)
#define EXTHDR_FLAG_ADI                                     BV(EXTHDR_FLAG_ADI_BIT)
#define EXTHDR_FLAG_AUXPTR                                  BV(EXTHDR_FLAG_AUXPTR_BIT)
#define EXTHDR_FLAG_SYNCINFO                                BV(EXTHDR_FLAG_SYNCINFO_BIT)
#define EXTHDR_FLAG_TXPWR                                   BV(EXTHDR_FLAG_TXPWR_BIT)
#define EXTHDR_FLAG_RFU2                                    BV(EXTHDR_FLAG_RFU2_BIT)
//
#define EXTHDR_MAX_LEN                                      63
#define EXTHDR_LEN_MASK                                     0x3F
#define EXTHDR_LEN_OFFSET                                   0
//
#define EXTHDR_DID_SIZE                                     12 // in bits
#define EXTHDR_DID_MASK                                     0x0FFF
//
#define EXTHDR_ACAD_CHANMAP_UPDATE_SIZE                    0x08
#define EXTHDR_ACAD_CHANMAP_UPDATE_TYPE                    0x28
//
#define EXTHDR_TOTAL_BUF_SIZE                               (EXTHDR_FLAGS_SIZE         +  \
                                                             EXTHDR_FLAG_ADVA_SIZE     +  \
                                                             EXTHDR_FLAG_TARGETA_SIZE  +  \
                                                             EXTHDR_FLAG_CTEINFO_SIZE  +  \
                                                             EXTHDR_FLAG_ADI_SIZE      +  \
                                                             EXTHDR_FLAG_AUXPTR_SIZE   +  \
                                                             EXTHDR_FLAG_SYNCINFO_SIZE +  \
                                                             EXTHDR_FLAG_TXPWR_SIZE)

#define PERIODIC_ADV_HDR_TOTAL_BUF_SIZE                     (EXTHDR_FLAG_CTEINFO_SIZE  +  \
                                                             EXTHDR_FLAG_AUXPTR_SIZE   +  \
                                                             EXTHDR_FLAG_TXPWR_SIZE)   +  \
                                                             ACAD_CHAN_MAP_SIZE

#define AE_EXT_HDR_ADV_TYPE_FIELD_SIZE                      1 // 6 bits extended header size and 2 bits for the adv type

// Periodic soft stop Time values - gracefulStopTime when using RCL
// Stop Time is Jitter with some grace
#define PER_SUCCESS_SOFTSTOPTIME_DEFAULT                    LL_JITTER_CORRECTION + RAT_TICKS_IN_625US

#define PER_DRIFT_RATIO                                     4                        // For every 1s add 50us drift

// Sync Info offsets
#define AE_SYNC_INFO_PACKET_OFFSET_WINDOW_OFFSET            0
#define AE_SYNC_INFO_INTERVAL_OFFSET                        2
#define AE_SYNC_INFO_CHANNEL_MAP_OFFSET                     4
#define AE_SYNC_INFO_SCA_OFFSET                             8
#define AE_SYNC_INFO_ACCESS_ADDR_OFFSET                     9
#define AE_SYNC_INFO_CRC_INIT_OFFSET                        13
#define AE_SYNC_INFO_PERIODIC_EVENT_COUNTER_OFFSET          16
#define AE_SYNC_INFO_SCA_BIT_OFFSET                         5
#define AE_SYNC_INFO_SCA_MASK                               0xE0

// Auxilary Offset Units
#define AE_AUX_OFFSET_UNITS_30_US                           0
#define AE_AUX_OFFSET_UNITS_300_US                          1

// Offset Adjust value - add 2.4576 seconds to the offset
// when the Offset Adjust field in syncInfo is set to 1
#define AE_AUX_OFFSET_ADJUST_US                             2457600    // 2.4576 seconds

//
#define AE_AUX_OFFSET_30_US_UNIT_VALUE                      30
#define AE_AUX_OFFSET_300_US_UNIT_VALUE                     300
#define AE_AUX_OFFSET_UNITS_VALUE_DIFF                      (AE_AUX_OFFSET_300_US_UNIT_VALUE - AE_AUX_OFFSET_30_US_UNIT_VALUE)
#define AE_AUX_OFFSET_LSB                                   1
#define AE_AUX_OFFSET_MSB                                   2
#define AE_AUX_OFFSET_UNITS_OFFSET                          7
#define AE_AUX_OFFSET_OFFSET                                8
#define AE_AUX_OFFSET_UNIT_CUTOFF_TIME                      0x0003BFC4 // 245,700 us
#define AE_AUX_OFFSET_SIZE                                  13         // in bits
#define AE_AUX_OFFSET_MASK                                  0x1FFF
#define AE_AUX_OFFSET_ADJUST_MASK                           0x4000
#define AE_AUX_OFFSET_AUTO_INSERT                           0

// Auxiliary Channel Index
#define AE_CHAN_INDEX_MASK                                  0x3FU
#define AE_IGNORE_BIT_MASK                                  0x40
#define AE_IGNORE_BIT_OFFSET                                6

// Auxiliary PHY mask
#define AE_PHY_MASK                                         0x7

// Auxiliary syncinfo only mask
#define AE_SYNCINFO_ONLY_MASK                               0x4

// Auxilary Clock Accuracy
#define AE_AUX_CA_51_500_PPM                                0
#define AE_AUX_CA_0_50_PPM                                  1

// Auxilary PHY
#define AE_AUX_1M_PHY                                       BLE5_1M_PHY
#define AE_AUX_2M_PHY                                       BLE5_2M_PHY
#define AE_AUX_CODED_PHY                                    BLE5_CODED_PHY

/*
** Common Extended Advertising Payload Format
*/
#define AE_EXT_HDR_LEN_INFO_SIZE                            1 // 6 bits of extended header length field
                                                              // and 2 bits for the advMode field
#define AE_EXT_HDR_FLAGS_SIZE                               1

/* Extended Advetising index */
#define AUX_CONN_RSP_PHY_INDEX      LL_PKT_HDR_LEN + AE_EXT_HDR_LEN_INFO_SIZE + \
                                    AE_EXT_HDR_FLAGS_SIZE + 2 * LL_DEVICE_ADDR_LEN

#define AE_AUX_ADVA_INDEX           LL_PKT_HDR_LEN + AE_EXT_HDR_LEN_INFO_SIZE + \
                                    AE_EXT_HDR_FLAGS_SIZE

#define AE_AUX_TARGETA_INDEX        AE_AUX_ADVA_INDEX + B_ADDR_LEN

#define CONN_IND_PEER_ADDR_TYPE_INDEX     0
#define CONN_IND_PEER_ADDR_INDEX          2
#define ADV_DATA_INDEX                    2
#define CONN_IND_AA_INDEX                 14
#define CONN_IND_CRC_INDEX                18
#define CONN_IND_WIN_OFFSET_INDEX         21
#define CONN_IND_WIN_SIZE_INDEX           22
#define CONN_IND_CONN_INTERVAL_INDEX      24
#define CONN_IND_PERI_LATENCY_INDEX       26
#define CONN_IND_CONN_TIMEOUT_INDEX       28
#define CONN_IND_CHAN_MAP_INDEX           30
#define CONN_IND_HOP_LEN_INDEX            35
#define CONN_IND_PEER_SCA_INDEX           35

// For AUX_CONN only
#define AUX_CONN_REQ_PHY_INDEX            36

/*
** Advertising and Scan Response Data
*/

// Operation
#define AE_DATA_OP_NEXT_FRAG                                0x00
#define AE_DATA_OP_FIRST_FRAG                               0x01
#define AE_DATA_OP_LAST_FRAG                                0x02
#define AE_DATA_OP_COMPLETE                                 0x03
#define AE_DATA_OP_UNCHANGED                                0x04
//
#define AE_DATA_OP_NO_DATA                                  0xFF

// Fragmentation Preference
#define AE_DATA_FRAG_PREF_CTRL_MAY_FRAG                     0
#define AE_DATA_FRAG_PREF_CTRL_MAY_NOT_FRAG                 1

// Length
#define AE_MAX_ADV_DATA_PARAM_LEN                           251
#define AE_DATA_LEN_IN_PAYLOAD_SIZE                         2     // in bytes

/*
** Extended Scanner
*/

#define AE_EXT_SCAN_MIN_TIME                                4      // in 625us = 2.5ms
#define AE_EXT_SCAN_MAX_TIME                                0xFFFF // in 625us units = 40.959375s

// Max adv sets to store their scan report state in parallel
#define EXT_SCAN_STATE_LIST_MAX_ENTRIES                     6

#define AE_EXT_SCAN_DURATION_INFINITE                       0
#define AE_EXT_SCAN_MIN_DURATION                            1      // in 10ms units = 10ms
#define AE_EXT_SCAN_MAX_DURATION                            0xFFFF // in 10ms units = 655.35s

#define AE_EXT_SCAN_PERIOD_DISABLED                         0
#define AE_EXT_SCAN_MIN_PERIOD                              1      // in 1.28s units = 1.28s
#define AE_EXT_SCAN_MAX_PERIOD                              0xFFFF // in 1.28s units = 83,884.8s

// Extended Advertising Report Event Type
#define AE_EVT_TYPE_CONNECTABLE_ADVERTISING                 0
#define AE_EVT_TYPE_SCANNABLE_ADVERTISING                   1
#define AE_EVT_TYPE_DIRECTED_ADVERTISING                    2
#define AE_EVT_TYPE_SCAN_RESPONSE                           3
#define AE_EVT_TYPE_LEGACY_PDU                              4
//
#define AE_EVT_TYPE_CONN_ADV                                BV(AE_EVT_TYPE_CONNECTABLE_ADVERTISING)
#define AE_EVT_TYPE_SCAN_ADV                                BV(AE_EVT_TYPE_SCANNABLE_ADVERTISING)
#define AE_EVT_TYPE_DIR_ADV                                 BV(AE_EVT_TYPE_DIRECTED_ADVERTISING)
#define AE_EVT_TYPE_SCAN_RSP                                BV(AE_EVT_TYPE_SCAN_RESPONSE)
#define AE_EVT_TYPE_LEGACY                                  BV(AE_EVT_TYPE_LEGACY_PDU)

// SID filtering
#define AE_EXT_SID_FILTERING_ENTRY_EMPTY                    -1
#define AE_EXT_SID_FILTERING_TIMEOUT                        RAT_TICKS_IN_1S * 2;

// Advertising Event Types for Legacy PDUs
#define AE_EXT_ADV_RPT_EVT_TYPE_ADV_IND                     AE_EVT_TYPE_LEGACY    | \
                                                            AE_EVT_TYPE_SCAN_ADV  | \
                                                            AE_EVT_TYPE_CONN_ADV  // 0x13
#define AE_EXT_ADV_RPT_EVT_TYPE_DIRECT_IND                  AE_EVT_TYPE_LEGACY    | \
                                                            AE_EVT_TYPE_DIR_ADV   | \
                                                            AE_EVT_TYPE_CONN_ADV  // 0x15
#define AE_EXT_ADV_RPT_EVT_TYPE_SCAN_IND                    AE_EVT_TYPE_LEGACY    | \
                                                            AE_EVT_TYPE_SCAN_ADV  // 0x12
#define AE_EXT_ADV_RPT_EVT_TYPE_NONCONN_IND                 AE_EVT_TYPE_LEGACY    // 0x10
#define AE_EXT_ADV_RPT_EVT_TYPE_SCAN_RSP_ADV_IND            AE_EVT_TYPE_LEGACY    | \
                                                            AE_EVT_TYPE_SCAN_RSP  | \
                                                            AE_EVT_TYPE_SCAN_ADV  | \
                                                            AE_EVT_TYPE_CONN_ADV  // 0x1B
#define AE_EXT_ADV_RPT_EVT_TYPE_SCAN_RSP_ADV_SCAN_IND       AE_EVT_TYPE_LEGACY    | \
                                                            AE_EVT_TYPE_SCAN_RSP  | \
                                                            AE_EVT_TYPE_SCAN_ADV  // 0x1A
// special error case
#define AE_EXT_ADV_RPT_EVT_TYPE_SCAN_RSP                    AE_EVT_TYPE_LEGACY    | \
                                                            AE_EVT_TYPE_SCAN_RSP  // 0x18

#define AE_EXT_ADV_RPT_DIR_ADDR_TYPE_UNRESOLVED_RPA         0xFE

//
#define AE_EVT_TYPE_COMPLETE_MASK                           (~(3 << 5))
#define AE_EVT_TYPE_COMPLETE                                (0 << 5)
#define AE_EVT_TYPE_INCOMPLETE_MORE_TO_COME                 (1 << 5)
#define AE_EVT_TYPE_INCOMPLETE_NO_MORE_TO_COME              (2 << 5)
#define AE_EVT_TYPE_RFU                                     (3 << 5)
//
#define AE_EVNT_INTERNAL_TYPE_PREV_TRUNCATED                (1 << 7)

// LE Extended Advertising Report Event

#define AE_NO_ADDRESS_PROVIDED                              0xFF

/*
** LL API Parameters
*/

// flags to indicate whether to allocate an Adv Set if not found, or return it
#define LE_SEARCH_ADV_SET                                   0
#define LE_ALLOCATE_ADV_SET                                 1

#define LE_COUNT_ALL_ADV_SETS                               0
#define LE_COUNT_ENABLED_ADV_SETS                           1

// Parameters to indicate if common data set operation is for Adv, Scan Response or update during Adv
#define LE_AE_EXT_DATA_CMD_ADV                              0
#define LE_AE_EXT_DATA_CMD_SCAN_RSP                         1

// Indicate that the adv data was not changed during advertising
#define EXT_DATA_NO_UPDATE_DURING_ADV                       0xFF

/*
** Callbacks
*/

// TEMP: THIS IS HCI TERRITORY, BUT EASIER FOR NOW IF DONE IN LL.
#define AE_SCAN_HCI_BLE_EXTENDED_ADV_REPORT_EVENT           0x0D      //!< Extended Adv Report
#define AE_ADV_HCI_BLE_ADV_SET_TERMINATED_EVENT             0x12      //!< LE Advertising Set Terminated Event
#define AE_ADV_HCI_BLE_SCAN_REQUEST_RECEIVED_EVENT          0x13      //!< Scan Request Received

// timing constants
// ALT: Make these values configurable from ll_config? Allow user to set?
#define AE_T_IFS_US                                         150
#define AE_T_MAFS_IN_US                                     300
#define AE_MIN_T_MAFS_IN_US                                 310
#define AE_MIN_OFFSET_UNIT_IN_US                             30
#define AE_MAX_OFFSET_UNIT_IN_US                            300
#define AE_1M_OR_CODED_TO_2M_TIME_COMPENSATION_IN_TICKS     7

// constants which are used to determine the time of adv over the air.
#define LEGACY_ADV_MAX_TIME_CONSUME                        3500
#define AE_CONSUME_OVERHEAD                                1500
#define AE_NUM_BYTES_OVERHAED_27_BYTES                       27
#define AE_SWITCH_TIME                                      420
// adv sorted list node start time error code
#define AE_INVALID_START_TIME                               0

// TEMP: Define substitute for StartSynthToRatOffset, a radio parameter(?).
#define START_SYNTH_TO_RAT_OFFSET                         166

// scan timeout flags to track duration/period
#define AE_SCAN_DURATION_TIMEOUT                            0
#define AE_SCAN_PERIOD_TIMEOUT                              1

// scan start state
#define AE_SCAN_START_STATE_FIRST                           0  // New period
#define AE_SCAN_START_STATE_SECOND                          1  // Invoke interval callback
#define AE_SCAN_START_STATE_NEXT                            2  // End of a period

// Action on scan state list
#define EXT_SCAN_STATE_LIST_ADD                             0
#define EXT_SCAN_STATE_LIST_FIND                            1
#define EXT_SCAN_STATE_LIST_REMOVE                          2
#define EXT_SCAN_STATE_LIST_CLEAR_ALL                       3

// ACAD
#define ACAD_SIZE_ZERO                                      0

// Scanner Extended Advertisement Report Packet States
#define WAIT_FOR_ADV_EXT_IND                                0
#define WAIT_FOR_AUX_ADV_IND                                1
#define WAIT_FOR_AUX_CHAIN_IND                              2
#define WAIT_FOR_AUX_SCAN_RSP                               3
#define WAIT_FOR_AUX_SYNC_RSP                               4

// RAT Channels
// Note: The Controller will always use Channel 0.
#define RF_RAT_CHAN_0                                       0
#define RF_RAT_CHAN_1                                       1
#define RF_RAT_CHAN_2                                       2

// Periodic scanner state
#define PERIODIC_SCAN_STATE_IDLE                            0
#define PERIODIC_SCAN_STATE_SYNCING_PENDING                 1
#define PERIODIC_SCAN_STATE_SYNCING_ACTIVE                  2
#define PERIODIC_SCAN_STATE_SYNCED                          3

// Margin time is rf and controller process time in rat ticks (empirical calculation)
#define PERIODIC_ADV_MARGIN_TIME_RAT_TICKS                  2400 + RAT_TICKS_FOR_SCHED_PROCESS_TIME // 800 usec
#define PERIODIC_SCAN_MARGIN_TIME_RAT_TICKS                 1600 + RAT_TICKS_FOR_SCHED_PROCESS_TIME // 600 usec
// NOTE: EXT_SCAN/INIT_MARGIN are used only by llCheckRfCmdPreemption (task OTA duration
// overhead). Scheduling gate margins in llUpdateScannSecTaskTimeGap and
// llUpdateTimeGapForInitiator use LL_SCHED_OVERHEAD (ll_scheduler.h) instead.
#define EXT_SCAN_MARGIN_TIME_RAT_TICKS                      2400 // 600 usec
#define EXT_INIT_MARGIN_TIME_RAT_TICKS                      2400 // 600 usec

// Optional status for process packet in scanner role
typedef enum {
  LL_PROCESS_STATUS_SUCCESS = 0x00U,              // Indicates successful processing
  LL_PROCESS_STATUS_INVALID_PARAMS = 0x01U,       // Indicates invalid parameters were provided
  LL_PROCESS_NO_CALLBACK_NO_PERIODIC = 0x02U,     // No registered callback and not in a periodic process
  LL_PROCESS_DROP_INVALID_OWN_ADDR = 0x03U,       // Indicates an invalid own address was detected
  LL_PROCESS_DROP_DUP_FILTER = 0x04U,             // Indicates that the packet should be dropped due to duplicate filter being enabled
  LL_PROCESS_DROP_PRIVACY = 0x05U,                // Indicates that the packet failed privacy checks
  LL_PROCESS_MEM_CAPACITY_EXCEEDED = 0x06U,       // Indicates memory capacity was exceeded
  LL_PROCESS_UNEXPECTED_SCAN_STATE = 0x07U,       // Indicates an unexpected scan state was encountered
  LL_PROCESS_INVALID_SECONDARY_PHY = 0x08U,       // Indicates an invalid secondary PHY was detected
  LL_PROCESS_DROP_UNSUPPORTED_AE = 0x09U,         // Indicates that the packet contains unsupported advertising extensions
  LL_PROCESS_DROP_UNKNOWN_SID = 0x0AU,            // Indicates that the packet contains an unknown SID
  LL_PROCESS_DROP_WAIT_AUX = 0x0BU                // Indicates to drop and wait for auxiliary advertising indication
} llProcessEntryStatus;

/*******************************************************************************
 * @fn          LE Extended Advertising Start After Enable Event Callback
 *
 * @brief       This callback is used to handle the Advertising Start After
 *              Enable event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - Pointer to the advertisement set handle for
 *                        this event, specified by uint8.
 *
 *                        Note: The pointer pParams MUST NOT be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_ADV_START_AFTER_ENABLE                     0

/*******************************************************************************
 * @fn          LE Extended Advertising End After Disable Event Callback
 *
 * @brief       This callback is used to handle the Advertising End After
 *              Disable event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - Pointer to the advertisement set handle for
 *                        this event, specified by uint8.
 *
 *                        Note: The pointer pParams MUST NOT be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_ADV_END_AFTER_DISABLE                      1

/*******************************************************************************
 * @fn          LE Extended Advertising Start Event Callback
 *
 * @brief       This callback is used to handle the Advertising Start event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - Pointer to the advertisement set handle for
 *                        this event, specified by uint8.
 *
 *                        Note: The pointer pParams MUST NOT be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_ADV_START                                  2

/*******************************************************************************
 * @fn          LE Extended Advertising End Event Callback
 *
 * @brief       This callback is used to handle the Advertising End event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - Pointer to the advertisement set handle for
 *                        this event, specified by uint8.
 *
 *                        Note: The pointer pParams MUST NOT be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_ADV_END                                    3

/*******************************************************************************
 * @fn          LE Extended Advertising Report Event Callback
 *
 * @brief       This callback is used to handle the Extended Advertising
 *              Report event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - Pointer aeExtAdvRptEvt to the advertising report
 *                        event data specified by aeExtAdvRptEvt_t.
 *
 *                        Note: The pointer pParams MUST be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_EXT_ADV_REPORT                             4

/*******************************************************************************
 * @fn          LE Advertising Set Terminated Callback
 *
 * @brief       This callback is used to handle the LE Advertising Set
 *              Terminated event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - Pointer aeAdvSetTerm to the advertising set terminated
 *                        event data specified by aeAdvSetTerm_t.
 *
 *                        Note: The pointer pParams MUST be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_ADV_SET_TERMINATED                         5

/*******************************************************************************
 * @fn          LE Extended Scan Request Received Event Callback
 *
 * @brief       This callback is used to handle the Extended Scan Request
 *              Received event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - Pointer aeScanReqReceived to the scan request received
 *                        event, data specified by aeScanReqReceived_t.
 *
 *                        Note: The pointer pParams MUST be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_EXT_SCAN_REQ_RECEIVED                      6

/*******************************************************************************
 * @fn          LE Extended Scan Timeout Event Callback
 *
 * @brief       This callback is used to handle the Extended Scan Timeout
 *              event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - NULL.
 *
 *                        Note: No parameter to be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_EXT_SCAN_TIMEOUT                           7

/*******************************************************************************
 * @fn          LE Extended Scan Start Event Callback
 *
 * @brief       This callback is used to handle the Extended Scan Start event.
 *              This event occurs only once when the Scan starts after it is
 *              enabled.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - NULL.
 *
 *                        Note: No parameter to be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_EXT_SCAN_START                             8

/*******************************************************************************
 * @fn          LE Extended Scan End Event Callback
 *
 * @brief       This callback is used to handle the Extended Scan End event.
 *              This event occurs only once when the Scan ends after it is
 *              disabled.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - NULL.
 *
 *                        Note: No parameter to be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_EXT_SCAN_END                               9

/*******************************************************************************
 * @fn          LE Extended Scan Window End Event Callback
 *
 * @brief       This callback is used to handle the Extended Scan Window End
 *              event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - NULL.
 *
 *                        Note: No parameter to be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_EXT_SCAN_WINDOW_END                        10

/*******************************************************************************
 * @fn          LE Extended Scan Interval End Event Callback
 *
 * @brief       This callback is used to handle the Extended Scan Interval End
 *              event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - NULL.
 *
 *                        Note: No parameter to be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_EXT_SCAN_INTERVAL_END                      11

/*******************************************************************************
 * @fn          LE Extended Scan Duration End Event Callback
 *
 * @brief       This callback is used to handle the Extended Scan Duration End
 *              event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - NULL.
 *
 *                        Note: No parameter to be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_EXT_SCAN_DURATION_END                      12

/*******************************************************************************
 * @fn          LE Extended Scan Period End Event Callback
 *
 * @brief       This callback is used to handle the Extended Scan Period End
 *              event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - NULL.
 *
 *                        Note: No parameter to be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_EXT_SCAN_PERIOD_END                        13

/*******************************************************************************
 * @fn          LE Out of Memory Event Callback
 *
 * @brief       This callback is used to handle an out of heap memory event.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - NULL.
 *
 *                        Note: No parameter to be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_OUT_OF_MEMORY                              14

/*******************************************************************************
 * @fn          LE Extended Advertisement Data Truncated Event Callback
 *
 * @brief       This callback is used to inform the Host that the Extended
 *              Advertising Data was truncated. This can happen when there's
 *              a connectable extended advertisement, and the the data has
 *              been set to a length that exceeds the max available data
 *              possible in the AUX_ADV_IND.
 *
 * @prototype   void <function name>( uint8 eventId, (void *)pParams );
 *
 * input parameters
 *
 * @param       eventId - Specified by this define.
 * @param       pParams - Pointer truncData to the extended advertisement data
 *                        truncated event, data specified by aeAdvTrucData_t.
 *
 *                        Note: The pointer pParams MUST NOT be freed!
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
#define LL_CBACK_EXT_ADV_DATA_TRUNCATED                     15

//
// Register callback with this index clears the callback tables.
//
#define LL_CBACK_CLEAR                                      0xFF

/*******************************************************************************
 * MACROS
 */

// Advertising Event Properties
#define TST_AE_PROPS_CONN( advEvtProps )               ((advEvtProps)[0] & AE_PROPS_CONN_ADV)
#define TST_AE_PROPS_SCAN( advEvtProps )               ((advEvtProps)[0] & AE_PROPS_SCAN_ADV)
#define TST_AE_PROPS_DIR( advEvtProps )                ((advEvtProps)[0] & AE_PROPS_DIR_ADV)
#define TST_AE_PROPS_HDC_DIR( advEvtProps )            ((advEvtProps)[0] & AE_PROPS_HDC_DIR_ADV)
#define TST_AE_PROPS_LEGACY( advEvtProps )             ((advEvtProps)[0] & AE_PROPS_LEGACY)
#define TST_AE_PROPS_OMIT_ADVA( advEvtProps )          ((advEvtProps)[0] & AE_PROPS_OMIT_ADVA)
#define TST_AE_PROPS_TX_PWR( advEvtProps )             ((advEvtProps)[0] & AE_PROPS_TX_POWER)
#define TST_AE_PROPS_DECISION_PDU( advEvtProps )       ((advEvtProps)[0] & AE_PROPS_DECISION_PDU)
#define TST_AE_PROPS_ADVA_DECISION_PDU( advEvtProps )  ((advEvtProps)[1] & AE_PROPS_ADVA_DECISION_PDU)
#define TST_AE_PROPS_ADI_DECISION_PDU( advEvtProps )   ((advEvtProps)[1] & AE_PROPS_ADI_DECISION_PDU)

// Common Extended Advertising Payload Format
#define LL_ADV_MODE( advHdr )                 ((advHdr) >> 6)

// Advertising Event Properties
#define SET_AE_PROPS_FLAG( flags, flag ) ((flags) |= (flag))
#define TST_AE_PROPS_FLAG( flags, flag ) ((flags) & (flag))
#define CLR_AE_PROPS_FLAG( flags, flag ) ((flags) &= ~(flag))

// Extended Header Flags
#define SET_EXTHDR_FLAG( flags, flag ) ((flags) |= (flag))
#define TST_EXTHDR_FLAG( flags, flag ) ((flags) & (flag))
#define CLR_EXTHDR_FLAG( flags, flag ) ((flags) &= ~(flag))

// Extended Header Length and Advertising Mode
#define SET_EXTHDR_LEN( hdr, len ) ( (hdr) = ((hdr) & ~EXTHDR_LEN_MASK) |      \
                                             ((len) &  EXTHDR_LEN_MASK))

#define GET_EXT_HDR_INFO( len, mode ) (((mode) << ADV_MODE_BIT_OFFSET) |       \
                                       ((len) & EXTHDR_LEN_MASK))

#define GET_EXT_HDR_LEN( hdr ) ((hdr) & EXTHDR_LEN_MASK)

#define GET_ADV_MODE( hdr ) ((hdr) >> ADV_MODE_BIT_OFFSET)

#define SET_ADV_MODE( hdr, mode ) ((hdr) = ((hdr) & ~ADV_MODE_MASK) |          \
                                           ((mode) << ADV_MODE_BIT_OFFSET) )

#define CLR_ADV_MODE( hdr ) ((hdr) &= ~ADV_MODE_MASK )

// Advertising Data Information Field
#define GET_SID( adi ) ((adi) >> EXTHDR_DID_SIZE)
#define GET_DID( adi ) ((adi) & EXTHDR_DID_MASK)

//
#define SET_EXT_ADV_HDR_CFG_SKIP_ADVA( c )                                     \
  (c) |= BV(0)

#define SET_EXT_ADV_HDR_CFG_SKIP_TGTA( c )                                     \
  (c) |= BV(1)

#define CLR_EXT_ADV_HDR_CFG_SKIP_TGTA( c )                                     \
  (c) &= ~BV(1)

#define SETVAR_EXT_ADV_HDR_CFG_DEV_ADDR_TYPE( c, v )                           \
  (c) = ((c) & ~BV(2)) | (((v) & 0x01) << 2)

#define SETVAR_EXT_ADV_HDR_CFG_TGT_ADDR_TYPE( c, v )                           \
  (c) = ((c) & ~BV(3)) | (((v) & 0x01) << 3)

// Periodic Scan options
#define GET_PERIODIC_SCAN_OPTIONS_LIST_USE(c)       (((c) >> 0) & 0x01)
#define GET_PERIODIC_SCAN_OPTIONS_REPORT_DISABLE(c) (((c) >> 1) & 0x01)

//Periodic Scan Sync CTE types
#define GET_PERIODIC_CTE_TYPE_SYNC_NO_AOA(c)       (((c) >> 0) & 0x01)
#define GET_PERIODIC_CTE_TYPE_SYNC_NO_1U_AOD(c)    (((c) >> 1) & 0x01)
#define GET_PERIODIC_CTE_TYPE_SYNC_NO_2U_AOD(c)    (((c) >> 2) & 0x01)
#define GET_PERIODIC_CTE_TYPE_SYNC_NO_TYPE_3(c)    (((c) >> 3) & 0x01)
#define GET_PERIODIC_CTE_TYPE_SYNC_ONLY_CTE(c)     (((c) >> 4) & 0x01)

// Ignore Bit for Scan Optimization
#define GET_IGNORE_BIT(pkt)                        (((pkt) & AE_IGNORE_BIT_MASK) >> AE_IGNORE_BIT_OFFSET)
#define GET_CHANNEL_IDX(pkt)                       ( (pkt) & AE_CHAN_INDEX_MASK)

/*******************************************************************************
 * TYPEDEFS
 */


////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

//
// LE Set Advertising Set Random Address Command
//
PACKED_TYPEDEF_STRUCT
{
  uint8 handle;
  uint8 randAddr[B_ADDR_LEN];
} aeRandAddrCmd_t;

//
// LE Set Extended Advertising Parameters Command
//
PACKED_TYPEDEF_STRUCT
{
  // Command Parameters
  uint8 handle;
  uint8 eventProps[2];
  uint8 primIntMin[3];
  uint8 primIntMax[3];
  uint8 primChanMap;
  uint8 ownAddrType;
  uint8 peerAddrType;
  uint8 peerAddr[B_ADDR_LEN];               // must be word aligned!
  uint8 filterPolicy;
  int8  txPower;
  uint8 primPhy;
  uint8 secMaxSkip;
  uint8 secPhy;
  uint8 sid;
  uint8 notifyEnableFlags;                  // scan request notification
#ifndef CONTROLLER_ONLY
  uint8 zeroDelay;                          // not part of BLE SIG HCI command
#endif
} aeSetParamCmd_t;

//
// LE Set Extended Advertising Parameters Return Parameters
//
PACKED_TYPEDEF_STRUCT
{
  int8 txPower;
} aeSetParamRtn_t;

//
// LE Set Extended Advertising and Scan Response Data Command
//
PACKED_TYPEDEF_STRUCT
{
  // Command Parameters
  uint8  handle;
  uint8  operation;                         // AE_DATA_OP_COMPLETE or AE_DATA_OP_UNCHANGED
  uint8  fragPref;                          // AE_DATA_FRAG_PREF_CTRL_MAY_FRAG or AE_DATA_FRAG_PREF_CTRL_MAY_NOT_FRAG
  uint16 dataLen;                           // length of Adv or Scan Rsp data
  uint8 *pData;                             // ptr to Host data, or NULL
} aeSetDataCmd_t;

//
// LE Set Extended Advertising Enable Command
//
PACKED_TYPEDEF_STRUCT
{
  // Command Parameters
  uint8  enable;
  uint8  numSets;
  uint8  handle;
  uint16 duration;                           // advertising duration
  uint8  maxEvents;                          // number of events allowed; 0=infinite
} aeEnableCmd_t;

//
// LE Set Extended Scan Parameters Command
//
PACKED_TYPEDEF_STRUCT
{
  uint8  scanType;                          // active or passive
  uint16 scanInterval;                      // 2.5ms..40.959375s (625us units)
  uint16 scanWindow;                        // 2.5ms..40.959375s (625us units)
} extScan_t;

PACKED_TYPEDEF_STRUCT
{
  uint8     ownAddrType;
  uint8     scanFilterPolicy;
  uint8     scanPhys;                       // bit 0: 1M, bit 2: Coded
  extScan_t extScanParam[2];
} aeSetScanParamCmd_t;

//
// LE Set Extended Scan Enable Command
//
PACKED_TYPEDEF_STRUCT
{
  uint8  enable;
  uint8  dupFiltering;
  uint16 duration;                          // 10ms..655.35s (10ms units)
  uint16 period;                            // 1.28s..83,884s (1.28s units)
} aeEnableScanCmd_t;

//
// LE Extended Create Connection Command
//
PACKED_TYPEDEF_STRUCT
{
  uint16 scanInterval;                      // 2.5ms..40.959375s (625us units)
  uint16 scanWindow;                        // 2.5ms..40.959375s (625us units)
  uint16 connIntMin;                        // 7.5ms..4s (1.25ms units)
  uint16 connIntMax;                        // 7.5ms..4s (1.25ms units)
  uint16 connLatency;                       // 0..499
  uint16 connTimeout;                       // 100ms..32s (10ms units)
  uint16 minLength;                         // 0ms..40.959375s (625us units)
  uint16 maxLength;                         // 0ms..40.959375s (625us units)
} extInit_t;

PACKED_TYPEDEF_STRUCT
{
  uint8     initFilterPolicy;
  uint8     ownAddrType;
  uint8     peerAddrType;
  uint8     peerAddr[B_ADDR_LEN];
  uint8     initPhys;                       // bit 0: 1M, bit 1: 2M, bit 2: Coded
  extInit_t extInitParam[LL_PHY_NUMBER_OF_PHYS];
} aeCreateConnCmd_t;

//
// LE Extended Advertising Report Event
//
PACKED_TYPEDEF_STRUCT
{
  uint8  subCode;                           // HCI_BLE_EXTENDED_ADV_REPORT_EVENT
  uint8  numRpts;                           // only a value of 1 is supported
  uint16 evtType;
  uint8  addrType;
  uint8  addr[B_ADDR_LEN];
  uint8  primPhy;                           // 0: 1M, 3: Coded
  uint8  secPhy;                            // 0: 1M, 2: 2M, 3: Coded
  uint8  advSid;                            // 0..15
  int8   txPower;                           // -127..126, or 127: no Tx Power
  int8   rssi;                              // -127..126, or 127: no RSSI
  uint16 periodicAdvInt;                    // 7.5ms..81,918.75s (1.25ms units), 0=No periodic advertising
  uint8  directAddrType;
  uint8  directAddr[B_ADDR_LEN];
  uint8  dataLen;                           // 0..229 bytes
  uint8  *pData;
  // Vendor Specific Information
  uint16 advDid;
} aeExtAdvRptEvt_t;

//
// LE Advertising Set Terminated Event
//
PACKED_TYPEDEF_STRUCT
{
  uint8  subCode;                           // HCI_BLE_ADV_SET_TERMINATED_EVENT
  uint8  status;
  uint8  handle;
  uint16 connHandle;
  uint8  numCompAdvEvts;
} aeAdvSetTerm_t;

//
// LE Scan Request Received Event
//
PACKED_TYPEDEF_STRUCT
{
  uint8  subCode;                           // HCI_BLE_SCAN_REQUEST_RECEIVED_EVENT
  uint8  handle;
  uint8  scanAddrType;
  uint8  scanAddr[B_ADDR_LEN];
  uint8  channel;
  int8   rssi;
} aeScanReqReceived_t;

// Extended Scanner Report state
typedef struct
{
  uint32 time;                   // time expected for AUX packet
  uint8  sid;                    // set ID - use as key list
  uint8  state;                  // scan state
  uint8  lastPrimPhy;            // keep PHY on primary channel
  uint8  lastScanRsp;            // indicate chain after AUX or after SCAN RSP
  uint8  directed;               // Indicate if received adv is directed or not
  uint8  advAddr[B_ADDR_LEN];    // advertising address
  uint8  addrType;               // address type given by the advertiser
  uint8  directAddr[B_ADDR_LEN]; // direct address (TargetA) for directed advertising
  uint8  directAddrType;         // direct address type
}extScanReportState_t;

//
// LE Scan Timeout Event
//
PACKED_TYPEDEF_STRUCT
{
  uint8  subCode;                           // HCI_BLE_SCAN_TIMEOUT_EVENT
} aeScanTimeout_t;

//
// Vendor Specific LE Advertising Set Adv Truncated Data Event
//
PACKED_TYPEDEF_STRUCT
{
  uint8   handle;
  uint16  advDataLen;
  uint8   availAdvDataLen;
} aeAdvTrucData_t;

//
// Register Callback Command
//
PACKED_TYPEDEF_STRUCT
{
  uint8 cBackID;
  void *pCBack;
} aeCBack_t;

/*
** BLE Input Command Parameter Structures
*/

// Legacy advertising packet struct
typedef struct
{
  List_Elem            elem;
  union
  {
      RCL_BufferState  state;                                  ///< Buffer state
      uint32_t         dummy;
  };
  uint16_t             length;                                 ///< Number of bytes in buffer after the length field
  uint8_t              numPad;                                 ///< Number of pad bytes before start of the packet
  uint8_t              pad0;                                   ///< First pad byte, or first byte of the packet if numPad == 0
  uint8_t              pad1;
  uint8_t              pad2;
  uint8_t              header;
  uint8_t              payloadLen;
  uint8_t              advA[ LL_DEVICE_ADDR_LEN ];
  union
  {
    uint8_t            targetA[ LL_DEVICE_ADDR_LEN ];
    uint8_t            advData[ LL_MAX_ADV_DATA_LEN ];
    uint8_t            scanRspData[ LL_MAX_SCAN_DATA_LEN  ];
  };
} aeLegacyPacket;

// Number of bytes between the aeLegacyPacket.length field and the first byte
// of the BLE PDU payload (advA). The RCL DMA transfers exactly 'length' bytes
// starting from the byte immediately after the 'length' field, so:
//   length = AE_LEGACY_PKT_LEN_OVERHEAD + payloadLen
// The overhead accounts for the six bytes that precede the payload in the
// buffer: numPad(1) + pad0(1) + pad1(1) + pad2(1) + header(1) + payloadLen(1)
#define AE_LEGACY_PKT_LEN_OVERHEAD  6

// Common Extended Packet Entry Format
typedef struct
{
  uint8         extHdrInfo;            // W:  advMode(7..6), length(5..0)
  uint8         extHdrFlags;           // W:  ext hdr flags per spec
  uint8         acadLen;               // W:  length of ACAD data
  uint8         extHdrConfig;          // W:  ext hdr configuration
  uint8         advDataLen;            // W:  size of Adv data
  uint8        *pExtHeader;            // W:  ptr to buffer with ext hdr
  uint8        *pAdvData;              // W:  ptr to adv data
} comExtPktFormat_t;

// Extended advertising packet struct
typedef struct
{
  List_Elem          __elem__;
  RCL_BufferState    state;                                   ///< Buffer state
  uint16             length   __attribute__ ((aligned (4)));  ///< Number of bytes in buffer after the length field
  uint8              numPad;                                  ///< Number of pad bytes before start of the packet
  uint8              pad0;                                    ///< First pad byte, or first byte of the packet if numPad == 0
  uint8              pad1;
  uint8              pad2;
  uint8              header;                                  ///< contain the packet type (EXT_IND/AUX_ADV...), chSel, TX/RX address type if needed
  uint8              payloadLen;                              ///< Packet payload length, include the header length and data length
  uint8              extHdrLen:6;                             ///< Extended header length
  uint8              advType:2;                               ///< NC_NS/Connectable...
  uint8              data[ LL_MAX_EXT_DATA_LEN ];             ///< The maximum is 254 and it depends on the header len
} aePacket;

/*
 * Extended advertising command struct
 * This structure holds 3 TX buffers. These buffer are used for building
 * the extended advertising packets the RCL will transmit.
 * There will be a rotation between the three buffers depending on the
 * chain length
 */
typedef struct
{
  RCL_CmdBle5Advertiser    advCmd;
  RCL_CtxAdvertiser        advParam;
  RCL_StatsAdvScanInit     advOutput;
  aePacket                 txBuffer[AE_NUM_TX_BUFFERS];
  comExtPktFormat_t        comPkt;
  uint16                   auxPhyFeature;
  uint8                    extHdr[EXTHDR_TOTAL_BUF_SIZE];
  uint8                    buffNo:2;  /// < Marks the next txBuffer that should be used
  uint8                    rfu:6;
} aeRf_t;

// Legacy advertising command struct
typedef struct
{
  RCL_CmdBle5Advertiser advCmd;
  RCL_CtxAdvertiser     advParam;
  RCL_StatsAdvScanInit  advOutput;
  aeLegacyPacket        advPacket;
  aeLegacyPacket        scanRspPacket;
} aeLegacyRf_t;

typedef union
{
  aeLegacyRf_t aeRfLegacyCmd;
  aeRf_t       aeRfCmd;
} aeRfCmdSize_t;



/*
** Controller Extended Advertising Set
*/

typedef struct advSet_t advSet_t;

// Extended Advertising Set Entry
struct advSet_t
{
  // Controller Internal
  advSet_t       *next;                           // ptr to next handle in list, if any
  taskInfo_t     *llTask;                         // pointer to associated BLE task block
  uint8           advMode;                        // flag to indicate if currently advertising
  uint8           paramValid;                     // flag to indicate parameters are valid
  uint8           connId;                         // allocated connection ID
  uint8           firstAdvEvt;                    // flag indicating first advertisement event
  uint8           maxAdvEvts;                     // maximum number of AE events, or continuous
  uint8           advHalted;                      // in order to save memory (avoid patching) advHalted was transformed from boolean to bitmap
                                                  // | reserved | disconnect, reason 0x05 | radio was halted |
                                                  // |   2..7   |          1              |         0        |
  uint8           advEvtType;                     // legacy Adv Event Type based on properties
  uint32          advStartTime;                   // start time of advertisement event
  // Primary Channel - ADV_EXT_IND
  uint8           numPrimChans;                   // number of primary adv channels
  uint8           firstPrimChan;                  // first primary channel

  uint8           extHdrInfo;                     // Adv Mode (7..6), Ext Hdr Len (5..0)
  uint8           extHdrFlags;                    // extended header flags
  uint8           acadLen;                        // length of ACAD data
  uint8           extHdrSize;                     // extended header length
  uint16          otaTimeExtAdv;                  // total OTA time in us per length and PHY
  uint16          otaTimeAdjust;                  // OTA time of one length + interpackt time (us)
  // Secondary Channel - AUX_ADV_IND, AUX_CHAIN_IND, AUX_SCAN_RSP, AUX_SYNC_IND
  uint8           auxHdrInfo;                     // Adv Mode (7..6), Ext Hdr Len (5..0)
  uint8           auxHdrFlags;                    // secondary channel header flags
  uint8           auxExtHdrSize;                  // aux extended header length
  uint8           auxChanIndex;                   // aux channel index
  uint16          auxChanCounter;                 // channelization algo 2 counter
  uint16          otaTimeAuxAdv;                  // total OTA time in us per length and PHY
  uint16          otaTimeAuxAdvScan;              // total OTA time in us per length and PHY for first Scannable Aux
  uint8           minTimeAdj;                     // total OTA time modulo min offset

  // Common Packet
  int8            txPowerValueDbm;                // tx power value in dBm
  uint16          scaValue;                       // Peripheral SCA in PPM
  uint8          *pOwnRandAddr;                   // Host defined random address (for LE_SetAdvSetRandAddr)
  // Note: Address must start on word boundary!
  uint8           ownAddr[ LL_DEVICE_ADDR_LEN ];  // own device address
  uint32          durationExpireTime;             // duration end time in RF ticks
  // Note: Address must start on word boundary!
  uint8           peerAddr[ LL_DEVICE_ADDR_LEN ]; // peer device address
  uint8           ownAddrType;                    // own device address type of public or random
  uint8           peerAddrType;                   // peer device address type of public or random
  // Advertising Data
  uint16          adi;                            // Advertising Information (ADI)
  uint8           maxAvailData;                   // max space available for adv data in pkt

  uint8           fragLen;                        // length of fragment
  uint8           lastFragLen;                    // length of last fragment
  uint8           numFrags;                       // number of advertising data fragments

  aeSetDataCmd_t  *pPendingAdvData;               // pointer to the ext data that is pending while adv is on
  aeSetDataCmd_t  *pPendingScanRspData;           // pointer to the scan rsp data that is pending while adv is on
  uint16          dataLen;                        // original length of data
  uint8          *pData;                          // pointer to raw data
  // ISR Related
  uint8           txCount;                        // track number of Tx Done interrupts
  uint8           priority;                       // Extended Advertise Priority.
  // RF Related
  // Note: Pointer to aeRf_t for AE, and aeLegacyRf_t for AE Legacy.
  uint8          *pRfCmds;                        // ptr to allocated RF command memory
  //
  // Host Extended Advertising Parameters
  aeSetParamCmd_t *pAdvParam;                     // ptr to Host provided params
  aeSetDataCmd_t  *pAdvData;                      // ptr to Host provided adv data params
  aeSetDataCmd_t  *pScanRspData;                  // ptr to Host provided scan response data params
  aeEnableCmd_t   *pEnable;                       // ptr to Host provided enable params

  uint8           actualOwnAddrType;              // The original address type
};

typedef struct sortedAdv_t sortedAdv_t;
// Structure to represent each node in Adv sorted list
struct sortedAdv_t
{
  advSet_t    *AdvEntry;
  uint16       timeConsume;
  uint8        consecutiveMisses;             // times ADV was skipped by connection collision
  uint32       timeScheduled;                 // last successful scheduled start time
  sortedAdv_t *next;
};

// Scan Response filter by sid
typedef struct
{
  uint32      scanStartTimeStamp;                           // timeStamp of the begining of Scanning
  uint32      timeOut;                                      // when timeout reached, sid table will be reset
  int8        sidTable[ EXT_SCAN_STATE_LIST_MAX_ENTRIES ];  // known sid list, none value is -1
} extSidScanRspFilter;

// Extended Scanner Information
typedef struct
{
  taskInfo_t *llTask;                               // pointer to associated task block
  // Note: Address must start on word boundary!
  uint8        ownAddr[ LL_DEVICE_ADDR_LEN ];       // own device address
  uint8        ownAddrType;                         // own device address type of public or random
  uint8        ownAddrRclCtx[ LL_DEVICE_ADDR_LEN ]; // own device address. used for processing directed advertising
                                                    // with a resolveable TargetA which is not the same as our latest RPA
                                                    // When set to zero address, it shall not be used.
  uint8        paramValid;                          // flag to indicate parameters are valid
  uint8        scanMode;                            // flag to indicate if currently scanning
  //
  uint32_t     scanStartTime;                       // absolute start time of the current scan window
  uint32_t     scanEndTime;                         // absolute deadline of the current scan window
  uint32       scanPeriodLeft;                      // Period - Duration
  uint8        scanStartState;                      // flag to indicate start state
  uint8        timingFlag;                          // flag to indicate duration or period end
  uint8        lastLegacyAdv;                       // used to track last received legacy Adv PDU
  //
  // Host Extended Scanner Parameters
  aeSetScanParamCmd_t scanParam;                    // Host provided scan copied params
  aeEnableScanCmd_t   scanEnable;                   // Host provided enable scan copied params
  //
  // Extended Scan Priority
  uint8                priority;                    // Extended Scan Priority as a Secondary Task.
  //
  // filter scanRsp SID table
  extSidScanRspFilter  extScanRspSidFltr;          // filter scanRsp by their SID value
} extScanInfo_t;

// Extended Initiator Event Information
typedef struct
{
  taskInfo_t *llTask;                         // pointer to associated task block
  // Note: Address must start on word boundary!
  uint8       ownAddr[ LL_DEVICE_ADDR_LEN ];  // own device address
  uint8       ownAddrType;                    // own device address type of public or random
  uint8       scanMode;                       // flag to indicate if currently scanning
  uint32      initStartTime;                  // start time of initiator event
  uint8       connId;                         // allocated connection ID
  uint8       scaValue;                       // Central SCA as an ordinal value for PPM
  //
  // Host Extended Initiator Parameters
  aeCreateConnCmd_t createConn;               // Host provided connection copied params.
  //
  // Extender Initiator Priority
  uint8             priority;                // Extender Initiator Priority as a Secondary Task.
} extInitInfo_t;

// Callback Table
// WARNING: Do not rearrange the order of these pointers without changing their
//          corresponding indices!
typedef struct
{
  union
  {
    uint8  numCBack;
    uint32 reserved;
  };
  void *pCBackAdvStartAfterEnable;    // LL_CBACK_ADV_START_AFTER_ENABLE
  void *pCBackAdvEndAfterDisble;      // LL_CBACK_ADV_END_AFTER_DISABLE
  void *pCBackAdvStart;               // LL_CBACK_ADV_START
  void *pCBackAdvEnd;                 // LL_CBACK_ADV_END
  //
  void *pCBackExtAdvReport;           // LL_CBACK_EXT_ADV_REPORT
  void *pCBackAdvSetTerminated;       // LL_CBACK_ADV_SET_TERMINATED
  void *pCBackExtScanReqReceived;     // LL_CBACK_EXT_SCAN_REQ_RECEIVED
  void *pCBackExtScanTimeout;         // LL_CBACK_EXT_SCAN_TIMEOUT
  //
  void *pCBackExtScanStart;           // LL_CBACK_EXT_SCAN_START
  void *pCBackExtScanEnd;             // LL_CBACK_EXT_SCAN_END
  void *pCBackExtScanWindowEnd;       // LL_CBACK_EXT_SCAN_WINDOW_END
  void *pCBackExtScanIntervalEnd;     // LL_CBACK_EXT_SCAN_INTERVAL_END
  void *pCBackExtDurationEnd;         // LL_CBACK_EXT_SCAN_DURATION_END
  void *pCBackExtPeriodEnd;           // LL_CBACK_EXT_SCAN_PERIOD_END
  //
  void *pCBackOutOfMemory;            // LL_CBACK_OUT_OF_MEMORY
  void *pCBackAdvDataTruncated;       // LL_CBACK_EXT_ADV_DATA_TRUNCATED
  //
} aeCBackTbl_t;

// Generic Callback Function Type
typedef void (*pCBack_t)(uint8 cBackID, void *pParams);

// Auxiliary struct to parse extended advertising PDUs info.
typedef struct llExtAdvPDUInfo_t
{
  uint8  *pExtHeader;        // Pointer to the extended header
  uint8  *pSyncInfo;         // Pointer to the synchronization information
  uint8  *pACAD;             // Pointer to the ACAD, null if there is no ACAD.
  uint8  extHdrFlgs;         // Extended header flags
  uint8  extHeaderLength;    // Length of the extended header
  uint8  currPhy;            // Current PHY
} llExtAdvPDUInfo;

// Auxiliary struct to parse legacy and extended advertising PDUs
// This structure holds information about the advertising PDU, including
// addresses, data, headers, and other relevant fields.
typedef struct llAdvPDUInfo_t
{
  llExtAdvPDUInfo extAdvInfo;     // Pointer to the extended advertising
                                  // information
  uint8   *pAdvA;                 // Pointer to the advertiser address
  uint8   *pTargetA;              // Pointer to the target address
  uint8   *pAdvData;              // Pointer to the advertising data
  uint8   *pPayload;              // Pointer to the payload
  uint8   advAType;               // Type of the advertiser address
  uint8   advAIsResolved;         // Indicates if advA is resolved
  uint8   targetAType;            // Type of the target address
  uint8   dataLen;                // Length of the advertising data
  uint8   header;                 // Header of the PDU
  uint8   packetlength;           // Length of the packet
  uint8   evtType;                // Type of the event
  uint8   channelIndex;           // Index of the channel
} llAdvPDUInfo;

// This struct includes the status of processing scan RX entry.
// It indicates whether to send the report or process the sync info for
// periodic advertising.
typedef struct llScanRxEntryStatus_t{
  uint8 checkSyncInfo;                // Indicate if whether to process the
                                      // sync info for periodic advertising
  uint8 sendReport;                   // Indicate if a report should be sent
                                      // to the host.
  llProcessEntryStatus processStatus; // Indicate the process status

} llScanRxEntryStatus;

/*******************************************************************************
 * LOCAL VARIABLES
*/

/*******************************************************************************
 * GLOBAL VARIABLES
 */

extern aeCBackTbl_t    aeCBackTbl;


extern advSet_t       *advSetList;
extern uint8           aeCurHandle;
extern uint8           aeCurAdvEnableHandle;
extern uint8           aeCurConnHandle;
extern uint8           aePeripheralSCA;

extern extScanInfo_t  *extScanInfo;

extern uint8           extScanIndex;
extern uint16          extScanNumMissed;
extern uint8           extScanPriority;
//

extern RCL_CmdBle5Scanner   extScanCmd;
extern RCL_CtxScanInit      extScanParam;
extern RCL_StatsAdvScanInit extScanOutput;

extern uint8           extScanChanMap;

extern extInitInfo_t  *extInitInfo;
extern uint8           extInitIndex;

extern RCL_CmdBle5Initiator  extInitCmd;
extern RCL_CtxScanInit       extInitParam;
extern RCL_StatsAdvScanInit  extInitOutput;

/*******************************************************************************
 * HCI AE API
 */

extern llStatus_t    LE_SetAdvSetRandAddr( aeRandAddrCmd_t * );
extern llStatus_t    LE_SetExtAdvParams( aeSetParamCmd_t *, aeSetParamRtn_t * );
void                 LE_SetExtAdvParamsAdvConn( aeSetParamCmd_t *pCmdParams, advSet_t *pAdvSet );
extern llStatus_t    LE_SetExtAdvData( aeSetDataCmd_t * );
extern llStatus_t    LE_SetExtScanRspData( aeSetDataCmd_t * );
extern llStatus_t    LE_SetExtAdvEnable( aeEnableCmd_t * );
llStatus_t           LE_checkAdvConnStatus( advSet_t *pAdvSet, aeEnableCmd_t *pCmdParams );
extern llStatus_t    LE_RemoveAdvSet( uint8 );
extern llStatus_t    LE_ClearAdvSets( void );
extern uint16        LE_ReadMaxAdvDataLen( void );
extern uint8         LE_ReadNumSupportedAdvSets( void );
extern llStatus_t    LE_AE_SetData( aeSetDataCmd_t *, uint8 );
extern llStatus_t    LE_SetExtScanParams( aeSetScanParamCmd_t * );
extern llStatus_t    LE_SetExtScanEnable( aeEnableScanCmd_t * );
extern llStatus_t    LE_ExtCreateConn( aeCreateConnCmd_t * );

/*******************************************************************************
 * LL Scan Helper Functions
 */

/**
 * @brief Validates that if the own address type for scan is random,
 *        HCI_LE_SetRandomAddressCmd was called. If the own address type
 *        is not random, returns TRUE.
 *
 * @param cmdScanParams - Pointer to scan parameters.
 *
 * @return TRUE if valid, FALSE otherwise.
 */
extern uint8 LL_IsValidRandomAddressForScan( aeSetScanParamCmd_t *cmdScanParams );

/**
 * @brief Validates that if the own address type for connection is random,
 *        HCI_LE_SetRandomAddressCmd was called. If the own address type
 *        is not random, returns TRUE.
 *
 * @param cmdConnParams - Pointer to connection parameters.
 *
 * @return TRUE if valid, FALSE otherwise.
 */
extern uint8 LL_IsValidRandomAddressForConnection( aeCreateConnCmd_t *cmdConnParams );

/**
 * @brief Initializes scan parameters to default values.
 *        Default: Public address, 500ms interval/window, active scan on 1M PHY.
 *
 * @param pcmdScanParams - Pointer to scan parameters structure to initialize.
 *
 * @return None.
 */
extern void LL_SetDefaultScanParams( aeSetScanParamCmd_t *pcmdScanParams );

/*******************************************************************************
 * LL AE API
 */
extern llStatus_t    LL_AE_Init( void );
extern llStatus_t    LL_AE_RegCBack( uint8 cBackId, void * );
llStatus_t           LL_AE_RegMultipleCBacks( uint8_t numCBacks, uint8_t *pCBackIds, void **pCBacks, llStatus_t *pRegStatus );
extern advSet_t     *LL_GetAdvSet( uint8, uint8 );
uint8_t              LL_AE_GetNumActiveAdvSets( void );
void                 LL_AE_SetNumActiveAdvSets( uint8_t numActiveSets );
advSet_t*            LL_AE_GetNextAdvSet( void );
uint32_t             LL_AE_GetLastTimeScheduled( void );
uint32_t             LL_AE_GetNextAdvSetTimeConsume( void );
advSet_t*            LL_SearchAdvSet( uint8_t handle );
llStatus_t           LL_ClearAdvSets( void );
extern uint8         LL_CountAdvSets( uint8 );
extern void          LL_DisableAdvSets( void );
extern void          LL_AE_SetPendingData( advSet_t * );
uint8                LL_GetNextChannelExtAdv(void);
bool                 LL_CheckIfNextAdvIsExtended(void);
void                 LL_AE_GetTxUsageParams(llTxUsageParams_t *pExtAdvTxParams);

extern llStatus_t    llSetExtendedAdvParams( advSet_t *, aeSetParamCmd_t * );
uint8_t                llParseExtAdvPduInfo( llAdvPDUInfo *pAdvInfo, RCL_Buffer_DataEntry* const  pDataEntry);
extern void          llSetRfCmdPreemptionParams( uint32 );
llStatus_t           llValidateAEAdvDataLength( aeSetDataCmd_t* pCmdParams, advSet_t* pAdvSet );
extern uint8         llGetRfCmdPreemptionEnable( void );

/*******************************************************************************
 * LL Internal API
 */
extern sortedAdv_t  *llGetAdvSortedEntry( advSet_t * );
extern void          llUpdateSortedAdvList( void );
extern void          llSetAETimeConsume( sortedAdv_t *aeNode );
uint16_t             llCalculateTotalExtAdvTime( sortedAdv_t *aeNode );
extern sortedAdv_t  *llDetachNode( sortedAdv_t *aeNode );
extern llStatus_t    llAddAdvSortedEntry( advSet_t *pAdvSet, sortedAdv_t** newNode, uint32 *pAdvStartTime );
extern void          llRemoveAdvSortedEntry( advSet_t *pAdvSet );
extern void          llAllocRfMem( advSet_t * );
extern llStatus_t    llBuildExtAdvPacket(aePacket *pPkt, comExtPktFormat_t *comPkt, uint8_t pktType, uint8_t payloadLen, uint8_t peerAddrType, uint8_t ownAddrType);
extern llStatus_t    llAddExtAdvPacketToTx(advSet_t *pAdvSet, uint8_t pktType, uint8_t payloadLen);
extern llStatus_t    llupdateAuxHdrPacket(advSet_t *pAdvSet);
uint8_t              llCheckAdvEvtType( uint8_t advEvtType);
void                 llResetExtScanStateMachine( uint8_t* scanState );
void                 llStopCurrentScan( void );
extern llStatus_t    llSetupExtAdvLegacy( advSet_t * );
void                 llupdateAdvCmdForHDC( advSet_t *pAdvSet, aeLegacyRf_t *pRf );
extern llStatus_t    llGetNextOrPreviousExtScanChannelIndex( uint8 );
extern uint8         llGetFirstExtScanChannelIndex( void );
extern llStatus_t    llSetupExtScan( uint32_t scanStartTime );
extern void          llSetupExtInit( uint8 );
extern void          llSetupExtData( advSet_t * );
extern void          llSetupExtHdr( advSet_t *, uint8, uint16 );
extern uint8         llGetExtHdrLen( uint8, uint8);
extern uint8         llNextChanIndex( uint16 );
extern void          llExtAdvCBack( uint8, void * );
extern uint8         llCheckCBack( uint8 );
extern void          llEndExtAdvTask( advSet_t * );
extern void          llEndExtScanTask( void );
extern void          llEndExtInitTask( void );
extern uint8         llGetRandChannelMapIndex( uint8 );
llStatus_t           llValidateAEAdvDataLen( aeSetDataCmd_t *pCmdParams );
llStatus_t           llValidateScanRspDataForAE( aeEnableCmd_t* pCmdParams, advSet_t* pAdvSet);
llStatus_t           llSetupExtendedAdvertising( advSet_t* pAdvSet );

extern uint32        llEstimatePeriodicAdvOtaTime(uint16 , uint8 , uint8 , uint8 , uint8 );

extern void          llSetPeriodicChanMap( llPeriodicChanMap_t *, uint8 * );
extern uint8         llSetNextPeriodicAdvChan( llPeriodicChanMap_t *, uint32 , uint16  );

extern extScanReportState_t *llManageExtScanStateList(uint8 , uint8 , uint32 , uint8 );
extern void          llClearExtScanReportState(void);
extern uint8         llCompareSecondaryPrimaryTasksQoSParam( uint8 , taskInfo_t *, llConnState_t * );
uint8_t              llCheckScanPriority( uint16_t taskID, uint8_t connPriority );
extern uint32        llGetSecondaryTaskEndTime( taskInfo_t *, uint32 , llConnState_t *);
extern uint8         llCheckRfCmdPreemption( uint32, uint8 );
extern void          llSetRestPrimaryChannels( advSet_t * );
extern void          llTermExtAdv( advSet_t *, uint8  );
uint8_t              llCheckAdvEventType( advSet_t * pAdvSet );


// RF Post Processing
extern void          llExtAdv_PostProcess( void );
extern void          llExtScan_PostProcess( void );
extern void          llExtInit_PostProcess( void );
extern void          LL_rclScanRxEntryDone( void );

// RF Event Processing
extern void          llHandleScanRxEntry( RCL_Buffer_DataEntry *pDataEntry );
extern void          llSendAdvSetTermEvent( advSet_t *, uint8, uint8 );
extern void          llSendAdvSetEndEvent( advSet_t * );
extern llStatus_t    llStartDurationTimer( uint16, uint32 );
// Scheduler Related
extern void         *llFindNextAdvSet( void );
extern void         *llFindNextSecCmd( taskInfo_t *llTask );

extern uint8 llAddExtAlAndSetIgnBit(aeExtAdvRptEvt_t *extAdvRpt, uint8 ignoreBit);
extern void llSetRxCfg(void);
uint32_t llReturnCurrentPeriodicStartTime();
uint16   llReturnMinConnInterval( aeCreateConnCmd_t *pCmdParams);
uint32_t llExtAdvTxTime(advSet_t* pAdvSet, uint8_t primPhy, uint8_t secPhy);
uint32_t llEstimateAuxOtaTime(advSet_t * pAdvSet, uint8_t secPhy);
void     llSetExtHdrFlags( aeSetDataCmd_t *pCmdParams );
uint8_t  llIsInitPriorityValid(uint16_t taskID, uint8_t priority, uint8_t connPriority);
bool     LL_AeEnable( void );

/*******************************************************************************
 */

#endif /* LL_AE_H */
