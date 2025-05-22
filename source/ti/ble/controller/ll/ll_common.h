/******************************************************************************

 @file  ll_common.h

 @brief This file contains the Link Layer (LL) types, constants,
        API's etc. for the Bluetooth Low Energy (ULE) Controller that are
        internally common to LL routines.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2025, Texas Instruments Incorporated

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


#ifndef LL_COMMON_H
#define LL_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */

#include <string.h>
#include <stdint.h>
#include <ti/drivers/rcl/RCL.h>
#include <ti/drivers/rcl/commands/ble5.h>
#include <ti/drivers/RNG.h>
#include "ti/drivers/utils/List.h"

#include "ti/ble/stack_util/osal/osal.h"
#include "ti/ble/controller/ll/ll.h"
#include "ti/ble/controller/ll/ll_scheduler.h"
#include "ti/ble/stack_util/health_toolkit/assert.h"

/*******************************************************************************
 * MACROS
 */

// Note: These macros assume the packet header has already been masked with
//       LL_DATA_PDU_HDR_LLID_MASK.
#define LL_DATA_PDU( pktHdr )            ((pktHdr) != LL_DATA_PDU_HDR_LLID_CONTROL_PKT)
#define LL_CTRL_PDU( pktHdr )            ((pktHdr) == LL_DATA_PDU_HDR_LLID_CONTROL_PKT)
#define LL_INVALID_LLID( pktHdr )        ((pktHdr) == LL_DATA_PDU_HDR_LLID_RESERVED)
//
#define LL_SCAN_REQ_PDU( pktHdr )        (((pktHdr) & 0x0FU) == LL_PKT_TYPE_SCAN_REQ)
#define LL_SCAN_RSP_PDU( pktHdr )        (((pktHdr) & 0x0FU) == LL_PKT_TYPE_SCAN_RSP)
#define LL_CONN_REQ_PDU( pktHdr )        (((pktHdr) & 0x0FU) == LL_PKT_TYPE_CONNECT_IND)
#define LL_ADV_IND_PDU( pktHdr )         (((pktHdr) & 0x0FU) == LL_PKT_TYPE_ADV_IND)
#define LL_ADV_DIRECT_IND_PDU( pktHdr )  (((pktHdr) & 0x0FU) == LL_PKT_TYPE_ADV_DIRECT_IND)
#define LL_ADV_NONCONN_IND_PDU( pktHdr ) (((pktHdr) & 0x0FU) == LL_PKT_TYPE_ADV_NONCONN_IND)
#define LL_ADV_SCAN_IND_PDU( pktHdr )    (((pktHdr) & 0x0FU) == LL_PKT_TYPE_ADV_SCAN_IND)
#define LL_ADV_EXT_IND_PDU( pktHdr )     (((pktHdr) & 0x0FU) == LL_PKT_TYPE_ADV_EXT_IND)
#define LL_AUX_PDU( pktHdr )             (((pktHdr) & 0x0FU) == LL_PKT_TYPE_GENERIC_AUX)
#define LL_AUX_CONN_RSP_PDU( pktHdr )    (((pktHdr) & 0x0FU) == LL_PKT_TYPE_AUX_CONNECT_RSP)
//
#define LL_ADV_PDU_TYPE( pktHdr )        ((pktHdr) & 0x0FU)
#define LL_LEGACY_ADV_PDU( pktHdr )      (LL_ADV_IND_PDU( (pktHdr) )          || \
                                          LL_ADV_DIRECT_IND_PDU( (pktHdr) )   || \
                                          LL_ADV_NONCONN_IND_PDU( (pktHdr) )  || \
                                          LL_SCAN_RSP_PDU( (pktHdr) )         || \
                                          LL_ADV_SCAN_IND_PDU( (pktHdr) ))

#define LL_ADV_HDR_GET_TX_ADD( hdr )     (((hdr) &  0x40) >> 6)
#define LL_ADV_HDR_GET_RX_ADD( hdr )     (((hdr) &  0x80) >> 7)
#define LL_ADV_HDR_SET_TX_ADD( hdr, v )  (((hdr) & ~0x40) | ((v) << 6))
#define LL_ADV_HDR_SET_RX_ADD( hdr, v )  (((hdr) & ~0x80) | ((v) << 7))
//
#define LL_ADV_HDR_GET_CHSEL( hdr )      (((hdr) &  0x20) >> 5)
#define LL_ADV_HDR_SET_CHSEL( hdr, v )   (((hdr) & ~0x20) | ((v) << 5))

#define LL_GET_PDU_HEADER(data, pad)      ( data + (pad - RCL_HEADER_BYTE) )

// Address types
// Check if the address type is RPA
#define LL_IS_ADDR_TYPE_RPA( type )       ( (type == LL_DEV_ADDR_TYPE_PUBLIC_ID) || \
                                            (type == LL_DEV_ADDR_TYPE_RANDOM_ID) )
// Check if the address type public/random identity address
#define LL_IS_ADDR_IDENTITY_TYPE( type )  ( (type == LL_DEV_ADDR_TYPE_PUBLIC)    || \
                                            (type == LL_DEV_ADDR_TYPE_RANDOM) )
// local ASSERT handler
// Note: Use HALNODEBUG to eliminate HAL assert handling (i.e. no assert).
// Note: If HALNODEBUG is not used, use ASSERT_RESET to reset system on assert.
//       Otherwise, evaluation board hazard lights are used.
// Note: Unused input parameter possible when HALNODEBUG; PC-Lint error 715.
#define LL_ASSERT(cond) HAL_ASSERT(cond)

// checks if RSSI is valid - returns boolean
#define LL_CHECK_RSSI_VALID( rssi )                                            \
          ((rssi) == LL_RF_RSSI_UNDEFINED || (rssi) == LL_RF_RSSI_INVALID)) ?  \
          TRUE                                                              :  \
          FALSE

// corrects RSSI if valid, otherwise returns not available
// Note: Input is uint8, output int8.
#define LL_CHECK_LAST_RSSI( rssi ) (rssi)
#define CHECK_CRITICAL_SECTION() (__get_BASEPRI() & 0x20 )

#define LL_CMP_BDADDR( dstPtr, srcPtr )                                        \
  ( ((dstPtr)[0] == (srcPtr)[0]) &&                                            \
    ((dstPtr)[1] == (srcPtr)[1]) &&                                            \
    ((dstPtr)[2] == (srcPtr)[2]) &&                                            \
    ((dstPtr)[3] == (srcPtr)[3]) &&                                            \
    ((dstPtr)[4] == (srcPtr)[4]) &&                                            \
    ((dstPtr)[5] == (srcPtr)[5]) )

/*
** Control Procedure Macros
*/
#define SET_FEATURE_FLAG( flags, flag ) ((flags) |= (flag))
#define TST_FEATURE_FLAG( flags, flag ) ((flags) & (flag))
#define CLR_FEATURE_FLAG( flags, flag ) ((flags) &= ~(flag))

#define ONLY_ONE_BIT_SET( x ) (((x) != 0) && !((x) & ((x)-1)))

// Special connection handles (currently only used in the controller for
// connection event reports)
#define LL_CONNHANDLE_ALL                  0xFFFD  //!< Terminates all links
#define LL_CONNHANDLE_LOOPBACK             0xFFFE  //!< Loopback connection handle, used to loopback a message
#define LL_CONNHANDLE_INVALID              0xFFFF  //!< Invalid connection handle, used for no connection handle

// Microsecond to/from RAT Tick Conversion
#define US_TO_RAT_TICKS( us )              ((us) << 2U)
#define RAT_TICKS_TO_US( rat )             ((rat) >> 2)

#define CONVERT_1US_TO_0_625MS( us )         ( us / 625 )     //!< Convert US to 0.625 ms
#define CONVERT_1_25MS_TO_0_625MS( ms )      ( ms << 1 )      //!< Convert 1.25 ms to 0.625 ms

/*******************************************************************************
 * CONSTANTS
 */

// Link Layer State
#define LL_STATE_IDLE                                  0x00
#define LL_STATE_ADV_UNDIRECTED                        0x01
#define LL_STATE_ADV_DIRECTED                          0x02
#define LL_STATE_ADV_SCANNABLE                         0x03
#define LL_STATE_ADV_NONCONN                           0x04
#define LL_STATE_SCAN                                  0x05
#define LL_STATE_INIT                                  0x06
#define LL_STATE_CONN_PERIPHERAL                       0x07
#define LL_STATE_CONN_CENTRAL                          0x08
#define LL_STATE_DIRECT_TEST_MODE_TX                   0x09
#define LL_STATE_DIRECT_TEST_MODE_RX                   0x0A
#define LL_STATE_MODEM_TEST_TX                         0x0B
#define LL_STATE_MODEM_TEST_RX                         0x0C
#define LL_STATE_MODEM_TEST_TX_FREQ_HOPPING            0x0D
#define LL_STATE_CONN_MONITOR                          0x0E
#define LL_STATE_CS_PRECAL                             0x0F
// Extended Advertising

// Pre release flag for the health check
#define LL_PRE_REALSE_INDICATION                       0xFF

// TEMP: CONSOLIDATE BACK INTO EXISTING STATES
#define LL_STATE_EXT_ADV                               0x11
#define LL_STATE_PERIODIC_ADV                          0x12
#define LL_STATE_PERIODIC_SCAN                         0x13

// Channel Assessment task - Adaptivity
#define LL_STATE_SDAA_CHANNEL_ASSESSMNT                0x14

// Hardware Failure Status
#define HW_FAIL_PAST_START_TRIG                        0x80
#define HW_FAIL_OUT_OF_MEMORY                          0x81
#define HW_FAIL_FW_INTERNAL_ERROR                      0x82
#define HW_FAIL_INVAILD_RF_COMMAND                     0x83
#define HW_FAIL_UNKNOWN_RF_STATUS                      0x84
#define HW_FAIL_UNEXPECTED_RF_STATUS                   0x85
#define HW_FAIL_UNKNOWN_LL_STATE                       0x86
#define HW_FAIL_FS_PROG_ERROR                          0x87
#define HW_FAIL_FS_FAIL_TO_START                       0x88
#define HW_FAIL_RF_INIT_ERROR                          0x89
#define HW_FAIL_PDU_SIZE_EXCEEDS_MTU                   0x8A
#define HW_FAIL_PKT_LEN_EXCEEDS_PDU_SIZE               0x8B
#define HW_FAIL_INADEQUATE_PKT_LEN                     0x8C
#define HW_FAIL_DISALLOWED_PHY_CHANGE                  0x8D
#define HW_FAIL_NO_TIMER_AVAILABLE                     0x8E
#define HW_FAIL_EXTENDED_AL_FAULT                      0x8F
#define HW_FAIL_NO_RAT_COMPARE_AVAILABLE               0x90
#define HW_FAIL_START_EXT_ADV_ERROR                    0x91
#define HW_FAIL_RF_DRIVER_ERROR                        0xE0
#define HW_FAIL_UNKNOWN_ERROR                          0xFF

/*
** Air Interface Packets
*/

// Packet Type Mask
#define LL_PKT_EVT_TYPE_MASK                           0x0F

// Advertising Channel PDUs
#define LL_PKT_TYPE_ADV_IND                            0U  // primary   - 1M
#define LL_PKT_TYPE_ADV_DIRECT_IND                     1U  // primary   - 1M
#define LL_PKT_TYPE_ADV_NONCONN_IND                    2U  // primary   - 1M
#define LL_PKT_TYPE_SCAN_REQ                           3U  // primary   - 1M
#define LL_PKT_TYPE_AUX_SCAN_REQ                       3U  // secondary - 1M/2M/Coded
#define LL_PKT_TYPE_SCAN_RSP                           4U  // primary   - 1M
#define LL_PKT_TYPE_CONNECT_IND                        5U  // primary   - 1M
#define LL_PKT_TYPE_AUX_CONNECT_IND                    5U  // secondary - 1M/2M/Coded
#define LL_PKT_TYPE_ADV_SCAN_IND                       6U  // primary   - 1M
#define LL_PKT_TYPE_GENERIC_AUX                        7U  // used for any secondary
#define LL_PKT_TYPE_ADV_EXT_IND                        7U  // secondary - 1M/Coded
#define LL_PKT_TYPE_AUX_ADV_IND                        7U  // secondary - 1M/2M/Coded
#define LL_PKT_TYPE_AUX_SCAN_RSP                       7U  // secondary - 1M/2M/Coded
#define LL_PKT_TYPE_AUX_SYNC_IND                       7U  // secondary - 1M/2M/Coded
#define LL_PKT_TYPE_AUX_CHAIN_IND                      7U  // secondary - 1M/2M/Coded
#define LL_PKT_TYPE_AUX_CONNECT_RSP                    8U  // secondary - 1M/2M/Coded
#define LL_PKT_TYPE_RESERVED                           9U

// Packet Related Information
#define LL_PKT_PREAMBLE_LEN                            1
#define LL_PKT_SYNCH_LEN                               4
#define LL_PKT_LLID_LEN                                1
#define LL_PKT_HDR_LEN                                 2
#define LL_PKT_MIC_LEN                                 4
#define LL_PKT_CRC_LEN                                 3
#define LL_PKT_DATAENTRY_DATA_OFFSET                   4

// Payload sizes
// Note: For control packets, this is the Opcode + CtrData.
#define LL_CTRL_PAYLOAD_LEN_UNDEFIEND                  0
#define LL_CONN_UPDATE_IND_PAYLOAD_LEN                 12
#define LL_CHAN_MAP_IND_PAYLOAD_LEN                    8
#define LL_TERM_IND_PAYLOAD_LEN                        2
#define LL_ENC_REQ_PAYLOAD_LEN                         23
#define LL_ENC_RSP_PAYLOAD_LEN                         13
#define LL_START_ENC_REQ_PAYLOAD_LEN                   1
#define LL_START_ENC_RSP_PAYLOAD_LEN                   1
#define LL_UNKNOWN_RSP_PAYLOAD_LEN                     2
#define LL_FEATURE_REQ_PAYLOAD_LEN                     9
#define LL_FEATURE_RSP_PAYLOAD_LEN                     9
#define LL_PAUSE_ENC_REQ_PAYLOAD_LEN                   1
#define LL_PAUSE_ENC_RSP_PAYLOAD_LEN                   1
#define LL_VERSION_IND_PAYLOAD_LEN                     6
#define LL_REJECT_IND_PAYLOAD_LEN                      2
#define LL_PERIPHERAL_FEATURE_REQ_PAYLOAD_LEN          9
#define LL_CONN_PARAM_REQ_PAYLOAD_LEN                  24
#define LL_CONN_PARAM_RSP_PAYLOAD_LEN                  24
#define LL_REJECT_EXT_IND_PAYLOAD_LEN                  3
#define LL_PING_REQ_PAYLOAD_LEN                        1
#define LL_PING_RSP_PAYLOAD_LEN                        1
#define LL_LENGTH_REQ_PAYLOAD_LEN                      9
#define LL_LENGTH_RSP_PAYLOAD_LEN                      9
#define LL_PHY_REQ_PAYLOAD_LEN                         3
#define LL_PHY_RSP_PAYLOAD_LEN                         3
#define LL_PHY_PHY_UPDATE_IND_PAYLOAD_LEN              5
#define LL_MIN_USED_CHANNELS_IND_LEN                   3
#define LL_CTE_REQ_PAYLOAD_LEN                         2
#define LL_CTE_RSP_PAYLOAD_LEN                         1
#define LL_PERIODIC_SYNC_IND_PAYLOAD_LEN               LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_CLOCK_ACCURACY_REQ_PAYLOAD_LEN              LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_CLOCK_ACCURACY_RSP_PAYLOAD_LEN              LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_CIS_REQ_PAYLOAD_LEN                         LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_CIS_RSP_PAYLOAD_LEN                         LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_CIS_IND_PAYLOAD_LEN                         LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_CIS_TERMINATE_IND_PAYLOAD_LEN               LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_POWER_CONTROL_REQ_PAYLOAD_LEN               4
#define LL_POWER_CONTROL_RSP_PAYLOAD_LEN               5
#define LL_POWER_CONTROL_IND_PAYLOAD_LEN               5
#define LL_SUBRATE_REQ_PAYLOAD_LEN                     LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_SUBRATE_IND_PAYLOAD_LEN                     LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_CHANNEL_REPORTING_IND_PAYLOAD_LEN           LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_CHANNEL_STATUS_IND_PAYLOAD_LEN              LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_PERIODIC_SYNC_WR_IND_PAYLOAD_LEN            LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_FEATURE_EXT_REQ_PAYLOAD_LEN                 LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_FEATURE_EXT_RSP_PAYLOAD_LEN                 LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_CS_CHANNEL_MAP_IND_PL_LEN                   11
#define LL_CS_FAE_RSP_PL_LEN                           73
#define LL_CS_FAE_REQ_PL_LEN                           1
#define LL_CS_TERMINATE_REQ_PL_LEN                     5
#define LL_CS_TERMINATE_RSP_PL_LEN                     5
#define LL_CS_IND_PL_LEN                               19
#define LL_CS_RSP_PL_LEN                               22
#define LL_CS_REQ_PL_LEN                               29
#define LL_CS_CONFIG_RSP_PL_LEN                        2
#define LL_CS_CONFIG_REQ_PL_LEN                        28
#define LL_CS_CAPABILITIES_RSP_PAYLOAD_LEN             26
#define LL_CS_CAPABILITIES_REQ_PAYLOAD_LEN             26
#define LL_CS_SEC_RSP_PL_LEN                           21
#define LL_CS_SEC_REQ_PL_LEN                           21
#define LL_FRAME_SPACE_REQ_PAYLOAD_LEN                 LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_FRAME_SPACE_RSP_PAYLOAD_LEN                 LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_UNSUPPORTED_PAYLOAD_LEN                     LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_TERMINATE_RX_WAIT_FOR_TX_ACK_PAYLOAD_LEN    LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_DUMMY_PLACE_HOLDER_TRANSMIT_PAYLOAD_LEN     LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_DUMMY_PLACE_HOLDER_TX_PENDING_PAYLOAD_LEN   LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_DUMMY_PLACE_HOLDER_RECEIVE_PAYLOAD_LEN      LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_UNDEFINED_PKT_PAYLOAD_LEN                   LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_INVALID_OPCODE_PAYLOAD_LEN                  LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_REVERVED1_PAYLOAD_LEN                       LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_REVERVED2_PAYLOAD_LEN                       LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_REVERVED3_PAYLOAD_LEN                       LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_REVERVED4_PAYLOAD_LEN                       LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_REVERVED5_PAYLOAD_LEN                       LL_CTRL_PAYLOAD_LEN_UNDEFIEND
#define LL_REVERVED6_PAYLOAD_LEN                       LL_CTRL_PAYLOAD_LEN_UNDEFIEND

// Set to one byte larger than the largest control packet length
#define LL_INVALID_CTRL_LEN                            74

// Miscellaneous fields, in bytes
#define LL_CONNECT_IND_LL_DATA_LEN                     22
#define LL_CONNECT_IND_PKT_LEN                         34
#define LL_NUM_BYTES_FOR_CHAN_MAP                      5   //(LL_MAX_NUM_ADV_CHAN+LL_MAX_NUM_DATA_CHAN)/sizeof(uint8)

// Terminate Indication Control Packet
// Note: MD bit is set to 1 to force sending this packet with MD=1 even though
//       the rest of the TX FIFO is empty and the MD configuration is set to
//       use automatic MD insertion based on FIFO contents.
// Note: The bytes are listed in little endian and read right-to-left:
//       0x13: MD=1, LLID=3 (LL control packet)
//       0x02: Length=1 plus one since NR will decrement the length
//       0x02: CtrlType=2 (TERMINATE_IND)
#define LL_TERM_IND_PKT_HDR                            ((1 << LL_DATA_PDU_HDR_MD_BIT) | LL_DATA_PDU_HDR_LLID_CONTROL_PKT)

// Max number of sequential NACKS before closing a connection event
#define LL_MAX_NUM_RX_NACKS_ALLOWED                    4

// Max number of retries to init the RNG before we fail
#define LL_MAX_INIT_RNG_RETRIES                        10U

// Control procedure timeout in coarse timer ticks
#define LL_MAX_CTRL_PROC_TIMEOUT                       64000 // 40s

// Authenticated payload timeout
#define LL_APTO_DEFAULT_VALUE                          30000 // 30s in ms

// Connection related timing
#define LL_CONNECTION_T_IFS                            150   // in us
#define LL_CONNECTION_SLOT_TIME                        625   // in us

// Max future number of events for an update to parameters or data channel
#define LL_MAX_UPDATE_COUNT_RANGE                      32767

// Connection Setup
#define LL_WINDOW_SIZE                                 2  // 2.5ms in 1.25ms ticks
#define LL_WINDOW_OFFSET                               0  // 1.25ms + 0
#define LL_LINK_SETUP_TIMEOUT                          5  // 6 connection intervals (i.e. 0..5)
#define LL_TRANSMIT_WIN_DELAY_LEGACY                   2  // in 625us units ~ 1.25ms
//
#define LL_TRANSMIT_WIN_DELAY_AE_UNCODED               4  // in 625us units ~ 2.5ms
#define LL_TRANSMIT_WIN_DELAY_LEGACY_AE_CODED          6  // in 625us units ~ 3.75 ms

// Adv PDU Header Fields
#define LL_ADV_PDU_HDR_TXADDR                          6
#define LL_ADV_PDU_HDR_RXADDR                          7
//
#define LL_ADV_PDU_HDR_LEN_MASK                        0x3F

// Data PDU Header Fields
#define LL_DATA_PDU_HDR_LLID_MASK                      0x03
//
#define LL_DATA_PDU_HDR_LLID_RESERVED                  0
#define LL_DATA_PDU_HDR_LLID_DATA_PKT_NEXT             1
#define LL_DATA_PDU_HDR_LLID_DATA_PKT_FIRST            2
#define LL_DATA_PDU_HDR_LLID_CONTROL_PKT               3
//
#define LL_DATA_PDU_HDR_NESN_BIT                       2
#define LL_DATA_PDU_HDR_SN_BIT                         3
#define LL_DATA_PDU_HDR_MD_BIT                         4
#define LL_DATA_PDU_HDR_CP_BIT                         5
#define LL_DATA_PDU_HDR_NESN_MASK                      0x04
#define LL_DATA_PDU_HDR_SN_MASK                        0x08
#define LL_DATA_PDU_HDR_MD_MASK                        0x10
#define LL_DATA_PDU_HDR_LEN_MASK                       0x1F

// CTE Info packet fields
#define LL_CTE_INFO_TIME_MASK                          0x1F
#define LL_CTE_INFO_TYPE_MASK                          0xC0
#define LL_CTE_INFO_TYPE_OFFSET                        6     // CTE time (5 bits) + RFU (1 bit)

// Data PDU overhead due to HCI packet type(1), connId(2), and length(2)
// Note: This is temporary until replace by BM alloc/free.
#define LL_DATA_HCI_OVERHEAD_LENGTH                    5

// Data PDU Control Packet Types - Vol 6, Part B, Section 2.4.2
#define LL_CTRL_CONNECTION_UPDATE_IND                 0x00 // C
#define LL_CTRL_CHANNEL_MAP_IND                       0x01 // C
#define LL_CTRL_TERMINATE_IND                         0x02 // C, P
#define LL_CTRL_ENC_REQ                               0x03 // C
#define LL_CTRL_ENC_RSP                               0x04 //  , P
#define LL_CTRL_START_ENC_REQ                         0x05 //  , P
#define LL_CTRL_START_ENC_RSP                         0x06 // C, P
#define LL_CTRL_UNKNOWN_RSP                           0x07 // C, P
#define LL_CTRL_FEATURE_REQ                           0x08 // C
#define LL_CTRL_FEATURE_RSP                           0x09 // C, P
#define LL_CTRL_PAUSE_ENC_REQ                         0x0A // C
#define LL_CTRL_PAUSE_ENC_RSP                         0x0B //  , P
#define LL_CTRL_VERSION_IND                           0x0C // C, P
#define LL_CTRL_REJECT_IND                            0x0D //  , P
#define LL_CTRL_PERIPHERAL_FEATURE_REQ                0x0E //  , P
#define LL_CTRL_CONNECTION_PARAM_REQ                  0x0F // C, P
#define LL_CTRL_CONNECTION_PARAM_RSP                  0x10 //  , P
#define LL_CTRL_REJECT_EXT_IND                        0x11 // C, P
#define LL_CTRL_PING_REQ                              0x12 // C, P
#define LL_CTRL_PING_RSP                              0x13 // C, P
#define LL_CTRL_LENGTH_REQ                            0x14 // C, P
#define LL_CTRL_LENGTH_RSP                            0x15 // C, P
#define LL_CTRL_PHY_REQ                               0x16 // C, P
#define LL_CTRL_PHY_RSP                               0x17 //  , P
#define LL_CTRL_PHY_UPDATE_IND                        0x18 // C
#define LL_CTRL_MIN_USED_CHANNELS_IND                 0x19 //  , P
#define LL_CTRL_CTE_REQ                               0x1A // C, P
#define LL_CTRL_CTE_RSP                               0x1B // C, P
#define LL_CTRL_PERIODIC_SYNC_IND                     0x1C // Unsupported yet
#define LL_CTRL_CLOCK_ACCURACY_REQ                    0x1D // Unsupported yet
#define LL_CTRL_CLOCK_ACCURACY_RSP                    0x1E // Unsupported yet
#define LL_CTRL_CIS_REQ                               0x1F // Unsupported yet
#define LL_CTRL_CIS_RSP                               0x20 // Unsupported yet
#define LL_CTRL_CIS_IND                               0x21 // Unsupported yet
#define LL_CTRL_CIS_TERMINATE_IND                     0x22 // Unsupported yet
#define LL_CTRL_POWER_CONTROL_REQ                     0x23 // C, P
#define LL_CTRL_POWER_CONTROL_RSP                     0x24 // C, P
#define LL_CTRL_POWER_CONTROL_IND                     0x25 // C, P
#define LL_CTRL_SUBRATE_REQ                           0x26 // Unsupported yet
#define LL_CTRL_SUBRATE_IND                           0x27 // Unsupported yet
#define LL_CTRL_CHANNEL_REPORTING_IND                 0x28 // Unsupported yet
#define LL_CTRL_CHANNEL_STATUS_IND                    0x29 // Unsupported yet
#define LL_CTRL_PERIODIC_SYNC_WR_IND                  0x2A // Unsupported yet
#define LL_CTRL_FEATURE_EXT_REQ                       0x2B // Unsupported yet
#define LL_CTRL_FEATURE_EXT_RSP                       0x2C // Unsupported yet
#define LL_CTRL_CS_SEC_RSP                            0x2DU //  , P
#define LL_CTRL_CS_CAPABILITIES_REQ                   0x2EU // C, P
#define LL_CTRL_CS_CAPABILITIES_RSP                   0x2FU // C, P
#define LL_CTRL_CS_CONFIG_REQ                         0x30U // C, P
#define LL_CTRL_CS_CONFIG_RSP                         0x31U // C, P
#define LL_CTRL_CS_REQ                                0x32U // C, P
#define LL_CTRL_CS_RSP                                0x33U //  , P
#define LL_CTRL_CS_IND                                0x34U // C
#define LL_CTRL_CS_TERMINATE_REQ                      0x35U // C
#define LL_CTRL_CS_FAE_REQ                            0x36U // C, P
#define LL_CTRL_CS_FAE_RSP                            0x37U // C, P
#define LL_CTRL_CS_CHANNEL_MAP_IND                    0x38U // C
#define LL_CTRL_CS_SEC_REQ                            0x39U // C
#define LL_CTRL_CS_TERMINATE_RSP                      0x3AU // C, P
#define LL_CTRL_FRAME_SPACE_REQ                       0x3B  // Unsupported yet
#define LL_CTRL_FRAME_SPACE_RSP                       0x3C  // Unsupported yet
/* 0x3D to 0xF0 - has no definition yet at current BLE Core version */
/* 0xF0 to 0xFB - reserved for specification development purposes   */
#define LL_CTRL_TERMINATE_RX_WAIT_FOR_TX_ACK          0xF0
// The LL_CTRL_DUMMY_PLACE_HOLDER_TRANSMIT type is used as a place holder at the
// Head of the control packet queue so that other control packets cannot
// Interleave a currently executing control procedure. This entry would then be
// Replaced with the appropriate control packet at a subsequent time. Any new
// Control packets would remain queued behind until the current control
// procedure completes.
#define LL_CTRL_DUMMY_PLACE_HOLDER_TRANSMIT           0xF1
// The LL_CTRL_DUMMY_PLACE_HOLDER_TX_PENDING type is used as a place holder for
// A control packet that has already been queued for Tx, but due to a collision
// Caused by a received packet, must be dequeued and restored at a later time
// Without being re-queued on the Tx FIFO.
#define LL_CTRL_DUMMY_PLACE_HOLDER_TX_PENDING         0xF2
// The LL_CTRL_DUMMY_PLACE_HOLDER_RECEIVE type is used to delay the processing
// Of a received control packet.
#define LL_CTRL_DUMMY_PLACE_HOLDER_RECEIVE            0xF3
#define LL_CTRL_UNDEFINED_PKT                         0xF4  // Use to indentify undefined control packets type
#define LL_CTRL_INVALID_OPCODE                        0xF5  // Use to indentify invalid opcode control packets
#define LL_CTRL_REVERVED1                             0xF6  // Reserved for specification development purposes
#define LL_CTRL_REVERVED2                             0xF7  // Reserved for specification development purposes
#define LL_CTRL_REVERVED3                             0xF8  // Reserved for specification development purposes
#define LL_CTRL_REVERVED4                             0xF9  // Reserved for specification development purposes
#define LL_CTRL_REVERVED5                             0xFA  // Reserved for specification development purposes
#define LL_CTRL_REVERVED6                             0xFB  // Reserved for specification development purposes
/* From 0xFC until 0xFF - has no definition at BLE Core 6.0 version */

#define NUM_OF_CTRL_PKT                                255 // (256 possible types, 12 Reserved for specification development purposes)

#define LL_CTRL_BLE_LOG_STRINGS_MAX                    27
extern char *llCtrl_BleLogStrings[];

// There is only supposed to be at most one control procedure pending, but some
// extra space is allocated here just in case some queueing is required.
#define LL_MAX_NUM_CTRL_PROC_PKTS                      10

// Control Procedure Actions
#define LL_CTRL_PROC_STATUS_SUCCESS                    0U
#define LL_CTRL_PROC_STATUS_TERMINATE                  1
#define LL_CTRL_PROC_STATUS_NOT_PROCESSED              2

// Setup Next Peripheral Procedure Actions
#define LL_SETUP_NEXT_LINK_STATUS_SUCCESS              0
#define LL_SETUP_NEXT_LINK_STATUS_TERMINATE            1

// Receive Flow Control
#define LL_RX_FLOW_CONTROL_DISABLED                    0
#define LL_RX_FLOW_CONTROL_ENABLED                     1

// Advertisement Channels
#define LL_ADV_BASE_CHAN                               37
//
#define LL_SCAN_ADV_CHAN_37                            (LL_ADV_BASE_CHAN+0)
#define LL_SCAN_ADV_CHAN_38                            (LL_ADV_BASE_CHAN+1)
#define LL_SCAN_ADV_CHAN_39                            (LL_ADV_BASE_CHAN+2)

// Advertiser Synchronization Word
#define ADV_SYNCH_WORD                                 0x8E89BED6  // Adv channel sync
#define ADV_CRC_INIT_VALUE                             0x00555555  // not needed; handled by NR hardware automatically

// Connection Related
#define LL_INVALID_CONNECTION_ID                       0xFFU
#define LL_RESERVED_CONNECTION_ID                      0x0F00

// Feature Response Flag
#define LL_FEATURE_RSP_INIT                            0
#define LL_FEATURE_RSP_PENDING                         1
#define LL_FEATURE_RSP_DONE                            2
#define LL_FEATURE_RSP_FAILED                          3

// Encryption Related
#define LL_ENC_RAND_LEN                                8
#define LL_ENC_EDIV_LEN                                2
#define LL_ENC_LTK_LEN                                 16
#define LL_ENC_IV_M_LEN                                4
#define LL_ENC_IV_S_LEN                                4
#define LL_ENC_IV_LINK_LEN                             4
#define LL_ENC_IV_LEN                                  (LL_ENC_IV_M_LEN + LL_ENC_IV_S_LEN)
#define LL_ENC_SKD_M_LEN                               8
#define LL_ENC_SKD_S_LEN                               8
#define LL_ENC_SKD_LINK_LEN                            8
#define LL_ENC_SKD_LEN                                 (LL_ENC_SKD_M_LEN + LL_ENC_SKD_S_LEN)
#define LL_ENC_SK_LEN                                  16
#define LL_ENC_NONCE_LEN                               13
#define LL_ENC_NONCE_IV_OFFSET                         5
#define LL_ENC_MIC_LEN                                 LL_PKT_MIC_LEN
//
#define LL_ENC_IV_M_OFFSET                             LL_ENC_IV_S_LEN
#define LL_ENC_IV_S_OFFSET                             0
#define LL_ENC_SKD_M_OFFSET                            LL_ENC_SKD_S_LEN
#define LL_ENC_SKD_S_OFFSET                            0
//
#define LL_ENC_BLOCK_LEN                               16
#define LL_ENC_CCM_BLOCK_LEN                           LL_ENC_BLOCK_LEN
#define LL_ENC_BLOCK_B0_FLAGS                          0x49
#define LL_ENC_BLOCK_A0_FLAGS                          0x01

// Clock Accuracy
#define LL_CA_20_PPM                                   20
#define LL_CA_40_PPM                                   40
#define LL_CA_50_PPM                                   50
#define LL_CA_100_PPM                                  100
#define LL_CA_500_PPM                                  500

// Default SCA
#define LL_SCA_CENTRAL_DEFAULT                         5 // 50ppm (ordinal)
#define LL_SCA_PERIPHERAL_DEFAULT                      LL_CA_40_PPM
#define LL_SCA_PERIPHERAL_DEFAULT_LFOSC                LL_CA_500_PPM

// TX Output Power Related
#define LL_TX_POWER_INVALID                            -128

// Direct Test Mode Related
#define LL_DIRECT_TEST_SYNCH_WORD                      0x71764129
#define LL_DTM_MAX_PAYLOAD_LEN                         37

/*
** LL RF Frequencies
*/
// BLE RF Values
#define LL_FIRST_RF_CHAN_FREQ                          2402      // MHz
#define LL_LAST_RF_CHAN_FREQ                           (LL_FIRST_RF_CHAN_FREQ+(2*LL_LAST_RF_CHAN))
//
// The BLE RF interface uses a 8 bit channel field to specify the RF channel:
//  0 ..  39: BLE Advertising/Data Channels
// 40 ..  59: Reserved
// 60 .. 207: Customer Frequency given by 2300+channel MHz
//       255: Use existing frequency.
//
#define LL_FIRST_RF_CHAN_FREQ_OFFSET                   2300
#define LL_FIRST_RF_CHAN_ADJ                           (LL_FIRST_RF_CHAN_FREQ - LL_FIRST_RF_CHAN_FREQ_OFFSET)
#define LL_LAST_RF_CHAN_ADJ                            (LL_LAST_RF_CHAN_FREQ - LL_FIRST_RF_CHAN_FREQ_OFFSET)

//defines for converter channel from BLE to RF channel
#define LL_RF_FREQ_HOP                                  2        // hop between 2 consecutive channels (in MHZ)
#define LL_BLE_CHANNEL_11                               11       // BLE channel 11.
// number of adv channels
#define LL_ONE_ADV_CHANNEL                              1
#define LL_TWO_ADV_CHANNEL                              2

/*
** FCFG and CCFG Offsets, and some Miscellaneous
*/

#define RCOSC_LF_SCA                                   1500      // possible worst case drift in PPM

// values for pendingParamUpdate
#define PARAM_UPDATE_NOT_PENDING                       0
#define PARAM_UPDATE_PENDING                           1
#define PARAM_UPDATE_APPLIED                           2

#define PHY_UPDATE_APPLIED                             2
#define CHANNEL_MAP_UPDATE_APPLIED                     2

// Update State Values for Peripheral Latency
#define UPDATE_PL_OKAY                                 0
#define UPDATE_RX_CTRL_ACK_PENDING                     1
#define UPDATE_NEW_TRANS_PENDING                       2

/*
** Bluetooth LE Feature Support
** Core Specification, Vol. 6, Part B, Section 4.6
*/

// Feature Set Related
#define LL_MAX_FEATURE_SET_SIZE                        8         // in bytes
//
#define LL_FEATURE_NONE                                0x00

// "Send to Peer" column description:
// "Y" indicates that the bit shall be set correctly when sent to the peer.
// "O" indicates that the bit shall either be zero or set correctly when sent to the
//      peer. The peer device shall ignore the value of this bit.
// "N" indicates that the bit shall be set to zero when sent to the peer.
// If a bit does not have "Y" for "Send to Peer", it shall not be used to determine
// whether a peer device supports any associated procedure.
//                                                                   // "Send to Peer"
// Byte 0
#define LL_FEATURE_ENCRYPTION                          0x01          //     "Y"
#define LL_FEATURE_CONN_PARAMS_REQ                     0x02          //     "Y"
#define LL_FEATURE_REJECT_EXT_IND                      0x04          //     "Y"
#define LL_FEATURE_SLV_FEATURES_EXCHANGE               0x08          //     "Y"
#define LL_FEATURE_PING                                0x10          //     "O"
#define LL_FEATURE_DATA_PACKET_LENGTH_EXTENSION        0x20          //     "Y"
#define LL_FEATURE_PRIVACY                             0x40          //     "O"
#define LL_FEATURE_EXTENDED_SCANNER_FILTER_POLICIES    0x80          //     "O"
// Byte 1
#define LL_FEATURE_2M_PHY                              0x01          //     "Y"
#define LL_FEATURE_STABLE_MODULATION_INDEX_TX          0x02          //     "Y"
#define LL_FEATURE_STABLE_MODULATION_INDEX_RX          0x04          //     "Y"
#define LL_FEATURE_CODED_PHY                           0x08          //     "Y"
#define LL_FEATURE_EXTENDED_ADVERTISING                0x10          //     "O"
#define LL_FEATURE_PERIODIC_ADVERTISING                0x20          //     "O"
#define LL_FEATURE_CHAN_ALGO_2                         0x40          //     "Y"
#define LL_FEATURE_LE_POWER_CLASS_1                    0x80          //     "Y"
// Byte 2
#define LL_FEATURE_MINIMUM_NUMBER_OF_USED_CHANNELS     0x01          //     "Y"
#define LL_FEATURE_CONNECTION_CTE_REQUEST              0x02          //     "Y"     // support CTE request procedure as initiator
#define LL_FEATURE_CONNECTION_CTE_RESPONSE             0x04          //     "Y"     // support CTE request procedure as responder
#define LL_FEATURE_CONNECTIONLESS_CTE_TRANSMITTER      0x08          //     "O"
#define LL_FEATURE_CONNECTIONLESS_CTE_RECEIVER         0x10          //     "O"
#define LL_FEATURE_ANTENNA_SWITCHING_DURING_CTE_TX     0x20          //     "O"     // support LL_FEATURE_RECEIVING_CTE and switching antennas for AoD
#define LL_FEATURE_ANTENNA_SWITCHING_DURING_CTE_RX     0x40          //     "O"     // support LL_FEATURE_RECEIVING_CTE and switching antennas for AoA
#define LL_FEATURE_RECEIVING_CTE                       0x80          //     "Y"     // support receiving CTE in data PDUs and IQ sampling
// Byte 3
#define LL_FEATURE_PERIODIC_ADV_SYNC_TRANSFER_SEND     0x01          //     "Y"
#define LL_FEATURE_PERIODIC_ADV_SYNC_TRANSFER_RECV     0x02          //     "Y"
#define LL_FEATURE_SLEEP_CLOCK_ACCURACY_UPDATES        0x04          //     "Y"
#define LL_FEATURE_REMOTE_PUBLIC_KEY_VALIDATION        0x08          //     "N"
#define LL_FEATURE_CONNECTED_ISOCHROOUS_STREAM_CENTRAL 0x10          //     "Y"
#define LL_FEATURE_CONNECTED_ISOCHROOUS_STREAM_PERIPHERAL 0x20       //     "Y"
#define LL_FEATURE_ISOCHRONOUS_BROADCASTER             0x40          //     "Y"
#define LL_FEATURE_SYNCRONIZED_RECEIVER                0x80          //     "Y"
// Byte 4
#define LL_FEATURE_CONNECTED_ISOCHROOUS_STREAM_HOST    0x01          //     "Y"
#define LL_FEATURE_LE_PWR_CTRL_REQ                     0x02          //     "Y"
#define LL_FEATURE_LE_PWR_CTRL_REQ_2                   0x04          //     "Y"
#define LL_FEATURE_LE_PATH_LOSS_MONITORING             0x08          //     "Y"
#define LL_FEATURE_PERIODIC_ADVERTISING_ADI_SUPPORT    0x10          //     "O"
#define LL_FEATURE_CONNECTION_SUBRATING                0x20          //     "Y"
#define LL_FEATURE_CONNECTION_SUBRAING_HOST_SUPPORT    0x40          //     "Y"
#define LL_FEATURE_CHANNEL_CLASSIFICATION              0x80          //     "Y"
// Byte 5
#define LL_FEATURE_RESERVED0                           0x01
#define LL_FEATURE_RESERVED1                           0x02
#define LL_FEATURE_RESERVED2                           0x04
#define LL_FEATURE_RESERVED3                           0x08
#define LL_FEATURE_RESERVED4                           0x10
#define LL_FEATURE_RESERVED5                           0x20
#define LL_FEATURE_CS                                  0x40
#define LL_FEATURE_CS_HOST                             0x80
// Byte 6
#define LL_FEATURE_RESERVED0                           0x01
#define LL_FEATURE_RESERVED1                           0x02
#define LL_FEATURE_RESERVED2                           0x04
#define LL_FEATURE_RESERVED3                           0x08
#define LL_FEATURE_RESERVED4                           0x10
#define LL_FEATURE_RESERVED5                           0x20
#define LL_FEATURE_RESERVED6                           0x40
#define LL_FEATURE_RESERVED7                           0x80
// Byte 7
#define LL_FEATURE_RESERVED0                           0x01
#define LL_FEATURE_RESERVED1                           0x02
#define LL_FEATURE_RESERVED2                           0x04
#define LL_FEATURE_RESERVED3                           0x08
#define LL_FEATURE_RESERVED4                           0x10
#define LL_FEATURE_RESERVED5                           0x20
#define LL_FEATURE_RESERVED6                           0x40
#define LL_FEATURE_RESERVED7                           0x80

// Feature Set Masks
// Note: BLE V5.0, Vol.6, Part B, Section 4.6, Table 4.4 indicates which bits
//       are valid between Controllers. Invalid bits are to be ignored upon
//       receipt from the peer Controller. These masks are logically "OR'ed"
//       with the peer's feature set upon exchange to ensure our Controller's
//       behavior remains unaffected by them.
#define LL_FEATURE_MASK_BYTE0                          (LL_FEATURE_PING                               |  \
                                                        LL_FEATURE_PRIVACY                            |  \
                                                        LL_FEATURE_EXTENDED_SCANNER_FILTER_POLICIES)
#define LL_FEATURE_MASK_BYTE1                          (LL_FEATURE_EXTENDED_ADVERTISING               |  \
                                                        LL_FEATURE_PERIODIC_ADVERTISING)
#define LL_FEATURE_MASK_BYTE2                          (LL_FEATURE_CONNECTIONLESS_CTE_TRANSMITTER     |  \
                                                        LL_FEATURE_CONNECTIONLESS_CTE_RECEIVER        |  \
                                                        LL_FEATURE_ANTENNA_SWITCHING_DURING_CTE_TX    |  \
                                                        LL_FEATURE_ANTENNA_SWITCHING_DURING_CTE_RX)
#define LL_FEATURE_MASK_BYTE3                          LL_FEATURE_REMOTE_PUBLIC_KEY_VALIDATION
#define LL_FEATURE_MASK_BYTE4                          (LL_FEATURE_PERIODIC_ADVERTISING_ADI_SUPPORT   |  \
                                                       LL_FEATURE_LE_PWR_CTRL_REQ            |  \
                                                       LL_FEATURE_LE_PWR_CTRL_REQ_2)
#define LL_FEATURE_MASK_BYTE5                          LL_FEATURE_NONE
#define LL_FEATURE_MASK_BYTE6                          LL_FEATURE_NONE
#define LL_FEATURE_MASK_BYTE7                          LL_FEATURE_NONE

// Feature set byte loaction
#define LL_FEATURE_BYTE_0                             0x0
#define LL_FEATURE_BYTE_1                             0x1
#define LL_FEATURE_BYTE_2                             0x2
#define LL_FEATURE_BYTE_3                             0x3
#define LL_FEATURE_BYTE_4                             0x4
#define LL_FEATURE_BYTE_5                             0x5
#define LL_FEATURE_BYTE_6                             0x6
#define LL_FEATURE_BYTE_7                             0x7

// Rx Ifs Timeout
#define LL_RF_RX_IFS_TIMEOUT                           0x10A6  // halfword write rxIfsTimeout
#define LL_RF_RX_IFS_DEFAULT_VAL                       0x03C0
#define LL_RF_RX_IFS_TIMEOUT_VAL                       0x02AD

//
// V4.2 - Extended Data length
//
#define LL_RF_DATA_LEN_WRITE_REG                       0x10A2  // halfword write dataLenMask and maxDataLen
#define LL_RF_DATA_LEN_DEFAULT_VAL                     0x1F1F
#define LL_RF_DATA_LEN_MAX_VAL                         0xFFFF
#define LL_RF_ADV_LEN_WRITE_REG                        0x10A4  // halfword write advLenMask and maxAdvLen
#define LL_RF_ADV_LEN_DEFAULT_VAL                      0x253F
#define LL_RF_ADV_LEN_MAX_VAL                          0xFFFF

//
// V5.0 - 2 Mbps and Coded PHY
//
#define LL_PHY_NUMBER_OF_PHYS                          3
#define LL_PHY_BASE_PHY                                LL_PHY_1_MBPS
#define LL_PHY_SLOWEST_PHY                             LL_PHY_1_MBPS //LL_PHY_CODED
#define LL_PHY_FASTEST_PHY                             LL_PHY_2_MBPS
#define LL_PHY_SUPPORTED_PHYS                          (LL_PHY_1_MBPS | LL_PHY_2_MBPS | LL_PHY_CODED)

//
// Control Procedure Flags
//

// Common Control Procedure Flags
#define CLEAR_ALL_FLAGS                                0x00
#define REJECT_EXT_IND_RECEIVED                        0x01
#define UNKNOWN_RSP_RECEIVED                           0x02
#define HOST_INITIATED                                 0x04
#define NOTIFY_HOST                                    0x08

// Use MIC In Payload
#define MIC_NOT_ENABLED                                0
#define MIC_ENABLED                                    1

// V4.1 - Connection Parameter Request Control Procedure Specific Flags
// ALT: REPLACE WITH ONE BYTE, USE MACROS
//uint8 connParamFlags;
//#define CONN_PARAM_CLEAR_ALL_FLAGS                   0x00
//#define CONN_PARAM_UNKNOWN_RSP_RECEIVED_FLAG         0x01
//#define CONN_PARAM_REJECT_EXT_IND_RECEIVED_FLAG      0x02
//#define CONN_PARAM_HOST_INITIATED_FLAG               0x04
//#define CONN_PARAM_REQ_RECEIVED_FLAG                 0x08
//#define CONN_PARAM_RSP_RECEIVED_FLAG                 0x10
//#define CONN_PARAM_UPDATE_ACTIVE_FLAG                0x20

// V4.2 - Extended Data Length Control Procedure Specific Flags
#define DISABLE_LEN_REQUEST                            0x10
#define LEN_RSP_RECEIVED                               0x20
#define UPDATE_LEN_RECEIVED                            0x40
#define REPLACE_RX_BUFFERS                             0x80

// V4.2 - Privacy V1.2 Control Procedure Specific Flags

// V4.2 - Secure Connections Control Procedure Specific Flags

// V5.0 - 2M and Coded PHY Control Procedure Specific Flags
#define DISABLE_PHY_REQUEST                            0x10
#define PHY_RSP_RECEIVED                               0x20
#define UPDATE_PHY_RECEIVED                            0x40

/*
** Miscellaneous
*/
#define BITS_PER_BYTE                                  8
#define BYTES_PER_WORD                                 4

// HCI Connection Complete Roles
#define HCI_EVT_CENTRAL_ROLE                           0
#define HCI_EVT_PERIPHERAL_ROLE                        1

// Channel Selection Algorithm
#define LL_CHANNEL_SELECT_ALGO_1                       0
#define LL_CHANNEL_SELECT_ALGO_2                       1

// TX Packet Last Packet Flag Values
// The RFHAL Data Entry specification allots four bits in the Config field for
// Partial Reads. Since BLE only uses Pointer Entries, these bits are ignored
// by the radio, and thus are not used. One bit can therefore be used to
// indicate if a completed fragmented packet is the last packet or not, so that
// packet completion can know when to send the Number of Completed Packets
// event to the Host.
//
// Warning: These bits are owned by radio firmware developers, who can change
//          the definition of the Config field, although unlikely in view of
//          keeping the spec backward compatible.
//
#define DATA_ENTRY_NOT_LAST_PACKET                     0x00
#define DATA_ENTRY_LAST_PACKET                         0x10

// BLE5 PHYs
#define BLE5_1M_PHY                                    0
#define BLE5_2M_PHY                                    1
#define BLE5_CODED_PHY                                 2U
#define BLE5_RESERVED_PHY                              3
// Rx Status
#define BLE5_S8_PHY                                    2
#define BLE5_S2_PHY                                    3
//
#define BLE5_CODING_NONE                               0
#define BLE5_CODED_S8_DEFAULT                          0
#define BLE5_CODED_S2_DEFAULT                          1
#define BLE5_CODED_S8_DO_NOT_USE_LAST_PKT              0
#define BLE5_CODED_S8_USE_LAST_PKT                     1
#define BLE5_USE_DEFAULT_RATE_FOR_EMPTY_PKTS           0
#define BLE5_USE_CODDED_S8_FOR_EMPTY_PKTS              1
#define BLE5_USE_DEFAULT_RATE_FOR_RETRAN_PKTS          0
#define BLE5_USE_CODDED_S8_FOR_RETRAN_PKTS             1
//
#define BLE5_CODED_S2_SCHEME                           2
#define BLE5_CODED_S8_SCHEME                           8
//
#define BLE5_CODED_S8_PHY                              (BLE5_CODED_PHY | (BLE5_CODED_S8_DEFAULT << 2))
#define BLE5_CODED_S2_PHY                              (BLE5_CODED_PHY | (BLE5_CODED_S2_DEFAULT << 2))
//
#define BLE5_PHY_MASK                                  0x03U

// Connection Event Statuses
#define LL_CONN_EVT_STAT_SUCCESS                       0U
#define LL_CONN_EVT_STAT_CRC_ERROR                     1U
#define LL_CONN_EVT_STAT_MISSED                        2U

// RF FW write param command type
#define RFC_FWPAR_ADDRESS_TYPE_BYTE                    (0x03)
#define RFC_FWPAR_ADDRESS_TYPE_DWORD                   (0x00)

// RF FW write param command CTE address
#define RFC_FWPAR_CTE_CONFIG                           (187)   // 1 byte
#define RFC_FWPAR_CTE_SAMPLING_CONFIG                  (188)   // 1 byte
#define RFC_FWPAR_CTE_OFFSET                           (189)   // 1 byte
#define RFC_FWPAR_CTE_ANT_SWITCH                       (208)   // 4 bytes
#define RFC_FWPAR_CTE_AUTO_COPY                        (212)   // 4 bytes
#define RFC_FWPAR_CTE_INFO_TX_TEST                     (159)   // 1 byte

// RF param command CTE offset
#define RFC_CTE_CONFIG_OFFSET                          (2)
#define RFC_CTE_SAMPLING_CONFIG_OFFSET                 (3)
#define RFC_CTE_ANT_SWITCH_OFFSET                      (5)
#define RFC_CTE_AUTO_COPY_OFFSET                       (6)

// RF force clock command parameters
#define RFC_FORCE_CLK_DIS_RAM                          (0x0000)
#define RFC_FORCE_CLK_ENA_RAM_MCE                      (0x0010)
#define RFC_FORCE_CLK_ENA_RAM_RFE                      (0x0040)

// extFeatureMask in llConnExtraParams_t
#define EXT_FEATURE_DISCONNECT_ENABLE                   BV(0)
#define EXT_FEATURE_DISCONNECT_DISABLE                 ~BV(0)

// DMM Policy invalid index
#define DMM_POLICY_INVALID_INDEX                       (0xFFFFFFFF)
#define DMM_POLICY_MAX_REPEAT_PRIORITIES               (2)

// Coex type
#define COEX_TYPE_1_WIRE_GRANT                         (0)
#define COEX_TYPE_1_WIRE_REQUEST                       (1)
#define COEX_TYPE_3_WIRE                               (2)

// CTE Samples task ID
#define CTE_TASK_ID_NONE                               (0)
#define CTE_TASK_ID_CONNECTION                         (1)
#define CTE_TASK_ID_CONNECTIONLESS                     (2)
#define CTE_TASK_ID_TEST                               (3)

// Timesync events
// The following events are used for connEvent registration.
// This is a clone of the stuct GAP_CB_Event_e in gap.h
// It's a bitmask event
#define LL_CONN_EVT_INVALID_TYPE                       0x00
#define LL_CONN_EVT_CONN_ESTABLISHED                   0x01
#define LL_CONN_EVT_PHY_UPDATE                         0x02
#define LL_CONN_EVT_ALL                                0xFF

// Health check status
#define LL_HEALTH_CHECK_SUCCESS         0
#define LL_HEALTH_CHECK_CONN_FAILURE    -1
#define LL_HEALTH_CHECK_SCAN_FAILURE    -2
#define LL_HEALTH_CHECK_INIT_FAILURE    -3
#define LL_HEALTH_CHECK_ADV_FAILURE     -4
#define LL_HEALTH_CHECK_PRE_RELEASE     -5
#define LL_HEALTH_CHECK_INVALID_RPA     -6
#define LL_HEALTH_CHECK_AES_FAILURE     -7

// Health check default time threshold
#define LL_HEALTH_CHECK_CONN_DEFAULT_THRESHOLD  (RAT_TICKS_IN_32S)           //32 sec - max LSTO
#define LL_HEALTH_CHECK_SCAN_DEFAULT_THRESHOLD  (40000 * RAT_TICKS_IN_1MS)   //40 sec max scan interval
#define LL_HEALTH_CHECK_INIT_DEFAULT_THRESHOLD  (40000 * RAT_TICKS_IN_1MS)   //40 sec max scan interval
#define LL_HEALTH_CHECK_ADV_DEFAULT_THRESHOLD   (10000 * RAT_TICKS_IN_1MS)   //10 sec max adv interval
#define LL_HEALTH_CHECK_AES_DEFAULT_THRESHOLD   5                            //5 AES engine failures

/*
 ** Connection indication's defines
 */
#define LL_CONN_IND_HEADER_OFFSET            0     // 0-1   (2 octets)
#define LL_CONN_IND_INITIATOR_ADDRESS_OFFSET 2     // 2-7   (6 octets)
#define LL_CONN_IND_ACCESS_ADDRESS_OFFSET    14    // 14-17 (4 octets)
#define LL_CONN_IND_TRANSMIT_WINDOW_OFFSET   22    // 22-23 (2 octets)
#define LL_CONN_IND_INTERVAL_OFFSET          24    // 24-25 (2 octets)
#define LL_CONN_IND_LATENCY_OFFSET           26    // 26-27 (2 octets)
#define LL_CONN_IND_TIMEOUT_OFFSET           28    // 28-29 (2 octets)
#define LL_CONN_IND_CHANNEL_MAP_OFFSET       30    // 30-34 (5 octets)

#define LL_MAX_SUPERVISION_TIMEOUT           0x0C80
#define LL_MIN_SUPERVISION_TIMEOUT           0x000A

#define LL_RCL_PKT_NUM_PAD_BTYES             3
#define LL_RCL_PKT_HDR_LEN                   2

#define LL_MAX_HCI_EVENT_LEN                 244

// Constans for llRclUpdateFilterList
#define LL_RCL_DYNAMIC_FL                    1U
#define LL_RCL_DUPLICATE_FL_ADD              2U
#define LL_RCL_DUPLICATE_FL_IGNORE           3U

// Optional status for update RCL filter list
typedef enum {
    LL_RCL_UPDATE_ENTRY_SUCCESS   = 0, // Filter list Entry update was successful
    LL_RCL_ENTRY_ALREADY_IGNORED  = 1, // Entry was already ignored
    LL_RCL_UPDATE_ENTRY_FAILURE   = 2  // Entry update failed
} llRclUpdateListStatus_t;

typedef enum {
    LL_LEGACY_CONN  = 0U,
    LL_EXT_CONN     = 1U
} llConnType;
/*******************************************************************************
 * TYPEDEFS
 */

/*
** Data PDU Control Packets
**
** The following structures are used to represent the various types of control
** packets. They are only used for recasting a data buffer for convenient
** field access
*/

// Connection Parameters
typedef struct
{
  uint8  winSize;                                    // window size
  uint16 winOffset;                                  // window offset
  uint16 connInterval;                               // connection interval
  uint16 peripheralLatency;                          // number of connection events the peripheral can ignore
  uint16 connTimeout;                                // supervision connection timeout
} connParam_t;

// Flags for Connection Parameters Request Control Procedure
typedef struct
{
  uint8 hostInitiated;                               // flag to indicate the Host initiated the Update
  uint8 unknownRspRcved;                             // flag to indicate Unknown Response received
  uint8 rejectIndExtRcved;                           // flag to indicate Reject Ind Extended received
  uint8 connParamReqRcved;                           // flag to indicate Connection Parameter Request received
  //uint8 connParamRspRcved;                           // flag to indicate Connection Parameter Response received
  uint8 connUpdateActive;                            // flag to indicate a Connection Update Parameter or Update Channel procedure active
} connParamFlags_t;

// Connection Parameters Request or Response
typedef struct
{
  uint16 intervalMin;                                // lower connection interval limit
  uint16 intervalMax;                                // upper connection interval limit
  uint16 latency;                                    // peripheral latency
  uint16 timeout;                                    // connection timeout
  uint8  periodicity;                                // preferred periodicity
  uint16 refConnEvtCount;                            // reference connection event count
  uint16 offset0;                                    // offset 0
  uint16 offset1;                                    // offset 1
  uint16 offset2;                                    // offset 2
  uint16 offset3;                                    // offset 3
  uint16 offset4;                                    // offset 4
  uint16 offset5;                                    // offset 5
} connParamReq_t;

// Channel Map
typedef struct
{
  uint8 chanMap[ LL_NUM_BYTES_FOR_CHAN_MAP ];        // bit map corresponding to the data channels 0..39
} chanMap_t;

// Encryption Request
typedef struct
{
  uint8 RAND[LL_ENC_RAND_LEN];                       // random vector from Central
  uint8 EDIV[LL_ENC_EDIV_LEN];                       // encrypted diversifier from Central
  uint8 SKDm[LL_ENC_SKD_M_LEN];                      // central SKD values concatenated
  uint8 IVm[LL_ENC_IV_M_LEN];                        // central IV values concatenated
} encReq_t;

// Encryption Response
typedef struct
{
  uint8 SKDs[LL_ENC_SKD_S_LEN];                      // peripheral SKD values concatenated
  uint8 IVs[LL_ENC_IV_S_LEN];                        // peripheral IV values concatenated
} encRsp_t;

// Unknown Response
typedef struct
{
  uint8 unknownType;                                 // control type of the control
} unknownRsp_t;                                      // packet that caused was unknown

// Feature Request
typedef struct
{
  uint8 featureSet[ LL_MAX_FEATURE_SET_SIZE ];       // features that are used or not
} featureReq_t;

// Feature Response
typedef struct
{
  uint8 featureSet[ LL_MAX_FEATURE_SET_SIZE ];       // features that are used or not
} featureRsp_t;

// Version Information
typedef struct
{
  uint8  verNum;                                     // controller spec version
  uint16 comId;                                      // company identifier
  uint16 subverNum;                                  // implementation version
} verInfo_t;

/*
** Connection Data
**
** The following structures are used to hold the data needed for a LL
** connection.
*/

// Encryption
typedef struct
{
  // Note: IV and SKD provide enough room for the full IV and SKD. When the
  //       Central and Peripheral values are provided, the result is one combined
  //       (concatenated) value.
  uint8  IV[ LL_ENC_IV_LEN ];                        // combined central and peripheral IV values concatenated
  uint8  SKD [ LL_ENC_SKD_LEN ];                     // combined central and peripheral SKD values concatenated
  uint8  RAND[ LL_ENC_RAND_LEN ];                    // random vector from Central
  uint8  EDIV[ LL_ENC_EDIV_LEN ];                    // encrypted diversifier from Central
  uint8  reserved[2];
  uint8  nonce[ LL_ENC_NONCE_LEN ];                  // current nonce with current IV value
  uint8  reserved2[3];
  uint8  SK[ LL_ENC_SK_LEN ];                        // session key derived from LTK and SKD
  uint8  LTK[ LL_ENC_LTK_LEN ];                      // Long Term Key from Host
  uint8  SKValid;                                    // flag that indicates the Session Key is valid
  uint8  LTKValid;                                   // Long Term Key is valid
  uint32 txPktCount;                                 // used for nonce formation during encryption (Note: 39 bits!)??
  uint32 rxPktCount;                                 // used for nonce formation during encryption (Note: 39 bits!)??
  // ALT: Could use one variable with one bit for each state.
  uint8  encRestart;                                 // flag to indicate if an encryption key change took place
  uint8  encRejectErrCode;                           // error code for rejecting encryption request
  uint8  startEncRspRcved;                           // flag to indicate the Start Request has been responded to
  uint8  pauseEncRspRcved;                           // flag to indicate the Pause Request has been responded to
  uint8  encReqRcved;                                // flag to indicate an Enc Req was received in a Enc Pause procedure
  uint8  encInProgress;                              // flag used to prevent a enc control procedure while one is already running

  uint8  startEncReqRcved;                           // flag to indicate the Start Request has been responded to
  uint8  rejectIndRcved;                             // flag to indicate the Start Encryption needs to be aborted
} encInfo_t;

// Feature Set Data
// This structure has a single global instance per device (called deviceFeatureSet)
// and one instance per each connection (part of connPtr, called featureSetInfo).
typedef struct
{
  uint8 featureRspRcved;                             // indicates that the peer feature set was received (in Feature Request PDU)
  uint8 featureSet[ LL_MAX_FEATURE_SET_SIZE ];       // in deviceFeatureSet, this indicates the features list, supported by this device
                                                     // in featureSetInfo, this is the active feature set - the combination (logical AND)
                                                     //                    of this device's feature set and the peer's device feature set.
  uint8 featureSetMask[ LL_MAX_FEATURE_SET_SIZE ];   // in deviceFeatureSet, this is the mask for peer's features, which we shall ignore when
                                                     //                    calculating active feature set.
                                                     // in featureSetInfo, this is the peer's device feature set.
                                                     //                    Remote feature procedure should to be called for this to be valid;
                                                     //                    doesn't matter who initiates the procedure - the central or the peripheral.
                                                     //                    Valid in case featureRspRcved is true.
} featureSet_t;

// Connection Termination
typedef struct
{
  uint8 connId;                                      // connection ID
  uint8 termIndRcvd;                                 // indicates a TERMINATE_IND was received
  uint8 reason;                                      // reason code to return to Host when connection finally ends
} termInfo_t;

// Version Information Exchange
typedef struct
{
  uint8 peerInfoValid;                               // flag to indicate the peer's version information is valid
  uint8 hostRequest;                                 // flag to indicate the host has requested the peer's version information
  uint8 verInfoSent;                                 // flag to indicate this device's version information has been sent
} verExchange_t;

// Reject Indication Extended
typedef struct
{
  uint8              rejectOpcode;                                // opcode that was rejected
  uint8              errorCode;                                   // error code for rejection
} rejectIndExt_t;

// Control Procedure Information
typedef struct
{
  uint8               ctrlPktActive;                              // control packet at head of queue has been queued for Tx
  uint8               ctrlPkts[ LL_MAX_NUM_CTRL_PROC_PKTS ];      // queue of control packets to be processed
  uint8               ctrlPktCount;                               // number of queued control packets
  uint16              ctrlTimeoutVal;                             // timeout in CI events for control procedure for this connection
  uint16              ctrlTimeout;                                // timeout counter in CI events for control procedure
  uint8               ctrlPktPending;                             // type of received control packet whose processing has been delayed
} ctrlPktInfo_t;

// Packet Error Rate Information - General
typedef struct
{
  uint16 numPkts;                                    // total number of packets received
  uint16 numCrcErr;                                  // number of packets with CRC error
  uint16 numEvents;                                  // number of connection events
  uint16 numMissedEvts;                              // number of missed connection events
} perInfo_t;

// RX Statistics Information - General
typedef struct
{
  uint16 numRxOk;                                    // number of okay Rx pkts
  uint16 numRxCtrl;                                  // number of okay Rx ctrl pkts
  uint16 numRxCtrlAck;                               // number of okay Rx ctrl pkts Acked
  uint16 numRxCrcErr;                                // number of not okay Rx pkts
  uint16 numRxIgnored;                               // number of okay Rx pkts ignored
  uint16 numRxEmpty;                                 // number of okay Rx pkts with no payload
  uint16 numRxBufFull;                               // number of pkts discarded
} rxStats_t;

// TX Statistics Information - General
typedef struct
{
  uint16 numTx;                                      // number of Tx pkts
  uint16 numTxAck;                                   // number of Tx pkts Acked
  uint16 numTxCtrl;                                  // number of Tx ctrl pkts
  uint16 numTxCtrlAck;                               // number of Tx ctrl pkts Acked
  uint16 numTxCtrlAckAck;                            // number of Tx ctrl pkts Acked that were Acked
  uint16 numTxRetrans;                               // number of retransmissions
  uint16 numTxEntryDone;                             // number of pkts on Tx queue that are finished
} txStats_t;

// TX Data
typedef struct txData_t
{
  struct txData_t *pNext;                            // pointer to next Tx data entry on queue
  uint8            fragFlag;                         // packet boundary flag
  uint16           len;                              // data length
  uint8           *pData;                            // pointer to data payload
} txData_t;

// Data Packet Queue
typedef struct
{
  txData_t *head;                                    // pointer to the head of the data queue
  txData_t *tail;                                    // pointer to the tail of the data queue
  uint8     numEntries;                              // number of data queue entries
} llDataQ_t;

// Peer Device Information
typedef struct
{
  uint8     peerAddr[ LL_DEVICE_ADDR_LEN ];          // peer device address
  uint8     peerAddrType;                            // peer device address type of public or random
  uint8     reserved;
} peerInfo_t;

// PHY Information
typedef struct
{
  uint8 curPhy;                                      // current PHY
  uint8 updatePhy;                                   // PHY to update to
  uint8 phyFlags;                                    // control procedure flags
  uint8 phyPreference;                               // based on set PHY
  uint8 phyOpts;                                     // Coded phy options
} phyInfo_t;

// Length Information
typedef struct
{
  uint16 connMaxRxOctets;                            // max payload bytes this device can receive
  uint16 connMaxRxTime;                              // max time (us) this device can take to receive payload
  uint16 connMaxTxOctets;                            // max payload bytes sent by this device
  uint16 connMaxTxTime;                              // max time (us) this device will take to transmit payload
  uint16 connRemoteMaxTxOctets;                      // max payload bytes peer device will send
  uint16 connRemoteMaxRxOctets;                      // max payload bytes peer can receive
  uint16 connRemoteMaxTxTime;                        // max time (us) peer will take to transmit
  uint16 connRemoteMaxRxTime;                        // max time (us) peer can take to receive
  uint16 connEffectiveMaxTxOctets;                   // lesser of connMaxTxOctets and connRemoteMaxRxOctets
  uint16 connEffectiveMaxRxOctets;                   // lesser of connMaxRxOctets and connRemoteMaxTxOctets
  uint16 connEffectiveMaxTxTime;                     // lesser of connMaxTxTime and connRemoteMaxRxTime
  uint16 connEffectiveMaxRxTime;                     // lesser of connMaxRxTime and connRemoteMaxTxTime
  //
  uint16 connIntervalPortionAvail;                   // the current connection interval - C (see spec)
  uint16 connEffectiveMaxTxTimeAvail;                // lesser of connEffectiveMaxTxTimeUncoded and connIntervalPortionAvail
  uint16 connEffectiveMaxTxTimeCoded;                // greater of 2704 and connEffectiveMaxTxTimeAvailable
  uint16 connEffectiveMaxTxTimeUncoded;              // lesser of connMaxTxTime and connremoteMaxRxTime
  uint16 connEffectiveMaxRxTimeCoded;                // greater of 2704 and connEffectiveMaxRxTimeUncoded
  uint16 connEffectiveMaxRxTimeUncoded;              // lesser of connMaxRxTime and connRemoteMaxTxTime
  uint8  lenFlags;                                   // control procedure flags
  //
  uint16 connActualMaxTxOctets;                      // max paylod based on max Octets and max Time
  uint8  connSlowestPhy;                             // slowest PHY limits connRemoteMaxTxTime
} lenInfo_t;

// Privacy 1.2
typedef struct
{
  uint8  addrResolution;
  uint32 rpaTimeout;
} privInfo_t;

// Connection Data Type
// Note: Needed for forward reference to llConnState_t from chSelAlgo_t.
typedef struct llConn_t llConnState_t;

// Data Channel Algorithm Function
typedef uint8 (*chSelAlgo_t)(llConnState_t *);

// Connection event report. Note that this is passed by reference through
// the host callback so the sizes of each element need to match Gap_ConnEventRpt_t
typedef struct
{
  uint8_t  status;   // status of connection event
  uint16_t handle;   // connection handle
  uint8_t  channel;  // BLE RF channel index (0-39)
  uint8_t  phy;      // PHY of connection event
  int8_t   lastRssi; // RSSI of last packet received
  // Number of packets received for this connection event.
  uint16_t packets;
  // Total number of CRC errors for this connection.
  uint16_t errors;
  uint16_t nextTaskType; // Type of next BLE task
  uint32_t nextTaskTime; // time to next BLE task
  uint16_t eventCounter; // event Counter
  uint32_t timeStamp;    // timestamp (anchor point)
  uint8_t  eventType;    // event type of the connection report
} connEvtRpt_t;

// Callback function pointer type for Connection Event notifications
typedef void (*llConnEvtCB_t)
(
  connEvtRpt_t *pReport  // Connection Event Report
);

// Connection event callback reporting information
typedef struct
{
  llConnEvtCB_t cb;  // Host callback
  /// Connection handle to send connection events for. If 0xFFFF, send all.
  uint16_t handle;
  uint8_t  eventType;
} llConnEvtNotice_t;

// Extended Feature Mask
// | 7..1 |    0                                         |
// |  N/A |  RPA not resolved disconnect with reason 0x05|
typedef uint8 extFeatureMask_t;

// Receive data buffer info
typedef struct
{
  uint16            size;
  uint16            pduSize;
  uint16            pduCid;
  uint8             state;
  uint8             *pEntry;
} llRxData_t;

// Connection Data
struct llConn_t
{
  taskInfo_t       *llTask;                             // pointer to associated task block
  // General Connection Data
  uint8             allocConn;                          // flag to indicate if this connection is allocated
  uint8             activeConn;                         // flag to indicate if this connection is active
  uint8             connId;                             // connection ID
  uint16            currentEvent;                       // current event number
  uint16            nextEvent;                          // next active event number
  uint16            expirationEvent;                    // event at which the LSTO has expired
  uint16            expirationValue;                    // number of events to a LSTO expiration
  uint8             firstPacket;                        // flag to indicate when the first packet has been received
  uint16            scaFactor;                          // SCA factor for timer drift calculation
  uint32            timerDrift;                         // saved timer drift adjustment to avoid recalc
  // Connection Parameters
  uint32            lastTimeToNextEvt;                  // the time to next event from the previous connection event
  uint8             peripheralLatencyAllowed;           // flag to indicate peripheral latency is permitted
  uint16            peripheralLatency;                  // current peripheral latency; 0 means inactive
  uint8             lastPeripheralLatency;              // last peripheral latency value used
  uint16            peripheralLatencyValue;             // current peripheral latency value (when enabled)
  uint32            accessAddr;                         // saved synchronization word to be used by Peripheral
  uint32            crcInit;                            // connection CRC initialization value (24 bits)
  uint8             sleepClkAccuracy;                   // peer's sleep clock accuracy; used by own device to determine timer drift
  connParam_t       curParam;                           // current connection parameters
  // Channel Map
  uint8             nextChan;                           // the unmapped channel for the next active connection event
  uint8             currentChan;                        // the current unmapped channel for the completed connection event
  uint8             currentMappedChan;                  // Currently used mapped channel
  uint8             numUsedChans;                       // count of the number of usable data channels
  uint8             hopLength;                          // used for finding next data channel at next connection event
  uint8             chanMapTable[LL_MAX_NUM_DATA_CHAN]; // current chan map table that is in use for this connection
  chanMap_t         curChanMap;
  // TX Related
  uint8             txDataEnabled;                      // flag that indicates whether data output is allowed
  void             *pTxDataEntryQ;
  // RX Related
  uint8             rxDataEnabled;                      // flag that indicates whether data input is allowed
  void             *pRxDataEntryQ;
  uint8             lastRssi;                           // last data packet RSSI received on this connection
  // Control Packet Information
  ctrlPktInfo_t     ctrlPktInfo;                        // information for control procedure processing
  // Parameter Update Control Procedure
  uint8             pendingParamUpdate;                 // flag to indicate connection parameter update is pending
  uint16            paramUpdateEvent;                   // event count to indicate when to apply pending param update
  connParam_t       paramUpdate;                        // update parameters
  // Channel Map Update Control Procedure
  uint8             pendingChanUpdate;                  // flag to indicate connection channel map update is pending
  uint16            chanMapUpdateEvent;                 // event count to indicate when to apply pending chan map update
  // Encryption Data Control Procedure
  uint8             encEnabled;                         // flag to indicate that encryption is enabled for this connection
  encInfo_t         encInfo;                            // structure that holds encryption related data
  // Feature Set
  featureSet_t      featureSetInfo;                     // feature set for this connection
  // Version Information
  verExchange_t     verExchange;                        // version information exchange
  verInfo_t         verInfo;                            // peer version information
  // Termination Control Procedure
  termInfo_t        termInfo;                           // structure that holds connection termination data
  // Unknnown Control Packet
  uint8             unknownCtrlType;                    // value of unknown control type
  // Packet Error Rate
  perInfo_t         perInfo;                            // PER
  perByChan_t      *perInfoByChan;                      // PER by Channel
  // Rx and Tx Statistics
  rxStats_t         rxStats;                            // RX statistics
  txStats_t         txStats;                            // TX statistics
  // Peer Address
  // Note: Address must start on word boundary!
  peerInfo_t        peerInfo;                           // peer device address and address type
  // Connection Event Notification
  uint16            taskID;                             // user task ID to send task event
  uint16            taskEvent;                          // user event to send at end of connection event

  // TEMP: THIS BELONGS IN taskInfo_t, BUT THEN ANOTHER MALLOC IS NEEDED.
  uint32            lastTimeoutTime;

  uint8             updateSLPending;                    // flag to monitor Central confirmation of Peripheral's ACK for update

  // save off central contribution
  uint16            mstSCA;                             // Central's portion of connection SCA

  // Authenticated Payload Timeout
  uint32            aptoValue;                          // APTO value, in ms
  uint8             pingReqPending;                     // flag to indicate PING Request control procedure in progress
  uint8             numAptoExp;                         // number of 1/2 APTO expirations
  uint8             aptoTimerId;                        // cbTimer timer ID needed to stop the timer

  // Connection Parameter Control Procedure
  connParamFlags_t  connParamReqFlags;                  // flags for handling connection parameter request control procedure
  connParamReq_t    connParams;                         // connection parameters for Request and Response packets
  rejectIndExt_t    rejectIndExt;                       // Reject Indication Extended Sent

  uint16            numEventsLeft;                      // events left before LSTO expiration
  uint16            prevConnInterval;                   // saved curParam CI before overwritten by updateParam CI

  // V4.2
  uint8             pendingLenUpdate;
  lenInfo_t         lenInfo;

  // V5.0
  uint8             pendingPhyUpdate;                   // flag to indicate a PHY update is pending
  uint16            phyUpdateEvent;                     // instant event for PHY update
  phyInfo_t         phyInfo;                            // PHY info for update
  uint8             phyUpdateSentOrReceivedInd ;        // indicates that there was a phy update sent or recieved

  chSelAlgo_t       pChSelAlgo;                         // function for data channel algorithm

  extFeatureMask_t  extFeatureMask;                     // features can highjack this bitmap which is connection oriented
  llRxData_t        rxData;                             // Receive data info
  uint8             connPriority;                       // connection priority
  uint16            connMissCount;                      // connection miss count
  uint8             connMinTimeExternalUpdateInd:1;     // connection external update indication for the minimum connection time.
  uint8             connMaxTimeExternalUpdateInd:1;     // connection external update indication for the maximum connection time.
  uint32            connMinTimeLength:31;               // connection minimum time length
  uint32            connMaxTimeLength:31;               // connection maximum time length
  /* Starvation Handling */
  uint8             StarvationMode:1;                   // connection starvation mode on/off
  uint8             numLSTORetries:3;                   // connection number of retries in LSTO state
  uint8             paramUpdateNotifyHost:1;            // indicates that there was a param update with param change in connInterval, connTimeout or peripheralLatency
  uint8             procInitiator:1;                    // indicates that this device has sent the req (initaite the procedure)
  uint8             estWithHandover:1;                  // TRUE indicated this connection formed using a connection handover procedure, else FALSE
  uint8             handoverInProg:1;                   // TRUE indicates handover is in progress
  uint8             estWithCm:1;                        // TRUE indicated this connection formed using a connection monitor procedure, else FALSE
  uint8_t           txBurstRatio;                       // Central only: if used, TxBurst Ratio will be determine by this value
  uint8             ownAddrType;                        // Own device address type - used for dual advertise sets with different types.

  /* Power control data pointer*/
  void              *pPowerControlConnInfo;             // Power control connection information
};

// Per BLE LL Connection
typedef struct
{
  uint8         numLLConns;                          // number of allocated connections
  uint8         numActiveConns;                      // number of allocated connections that are active
  uint8         currentConn;                         // the LL connection currently in use
  uint8         nextConn;                            // the next LL connection
  llConnState_t *llConnection;                       // connection state information
} llConns_t;

// Direct Test Mode
typedef struct
{
  uint8       rfChan;                                // 0..39
  uint8       packetLen;                             // 0..39 bytes
  uint8       packetType;                            // data pattern
  uint16      numPackets;                            // number of packets received
  uint16      numRxCrcNOK;                           // number of packets received with CRC error
  uint8       lastRssi;                              // RSSI of last packet received
  uint16      txPktCnt;                              // number of tx packets to transmit
} dtmInfo_t;

// RF Patch Compensation
typedef struct
{
  int16       rfTxPathCompParam;                     // RF Tx Path Compensation Parameter
  int16       rfRxPathCompParam;                     // RF Rx Path Compensation Parameter
  int8        rfTxPathCompVal;                       // RF Tx Path Compensation Value (floor of parameter)
  int8        rfRxPathCompVal;                       // RF Rx Path Compensation Value (floor of parameter)
} rfPathComp_t;

// Build Revision
typedef struct
{
  uint16      userRevNum;                            // user revision number
} buildInfo_t;

// Size Info (for dynamic allocation)
typedef struct
{
  uint8  sizeOfTaskInfo;
  uint16 sizeOfLlConnState;
  uint8  sizeOfAdvInfo;
  uint8  sizeOfScanInfo;
  uint8  sizeOfInitInfo;
  uint8  sizeOfAeCBackTbl;
  uint8  sizeOfAeRfCmdSize;
  uint8  sizeOfAeAdvSetTerm;
  uint8  sizeOfAeExtAdvRptEvt;
  uint8  sizeOfDtmInfo;
  uint8  sizeOfConnEvtRpt;
} sizeInfo_t;

// Multiple Antennas parameters
typedef struct
{
  /* Number of antennas. max value - 4 */
  uint8_t numAntennas;

  /*
  * Antenna selection multiplexer values bitmap
  * Stores the values for antenna selection, 2 bits for each antenna.
  * MSB: [4th order, 3rd order, 2nd order, 1st order] :LSB
  */
  uint8_t antennasMuxValues;

  /* The default antenna mux value to be used for common BLE.
   * Should be set to a range of 0x0 to 0x3 (up to 4 antennas).
   */
  uint8_t defaultAntennaMux;
} antennasInfo_t;

// @brief This struct contains the parameters required to compute the TX usage
//        for a BLE channel.
typedef struct
{
  uint32_t totalTxTime;  // Total time spent in Tx on this channel in us
  int8_t txPower;        // Transmit power in dBm
  uint8_t channel;       // BLE channel
} llTxUsageParams_t;

// Link Layer Test Mode
#ifdef LL_TEST_MODE

#define LL_TEST_MODE_TP_CON_MAS_BV_19                0
#define LL_TEST_MODE_TP_CON_MAS_BV_26                1
#define LL_TEST_MODE_TP_CON_MAS_BV_28                2
#define LL_TEST_MODE_TP_CON_MAS_BV_31_1              3
#define LL_TEST_MODE_TP_CON_MAS_BV_31_2              4
#define LL_TEST_MODE_TP_CON_MAS_BV_31_3              5
#define LL_TEST_MODE_TP_CON_MAS_BV_32                6
#define LL_TEST_MODE_TP_CON_MAS_BV_33                7
#define LL_TEST_MODE_TP_CON_MAS_BI_02                8
#define LL_TEST_MODE_TP_CON_MAS_BI_04                9
#define LL_TEST_MODE_TP_CON_MAS_BI_06                10
#define LL_TEST_MODE_TP_CON_SLA_BV_26                11
#define LL_TEST_MODE_TP_CON_SLA_BV_28                12
#define LL_TEST_MODE_TP_CON_SLA_BV_30_1              13
#define LL_TEST_MODE_TP_CON_SLA_BV_30_2              14
#define LL_TEST_MODE_TP_CON_SLA_BV_30_3              15
#define LL_TEST_MODE_TP_CON_SLA_BV_31                16
#define LL_TEST_MODE_TP_CON_SLA_BV_32                17
#define LL_TEST_MODE_TP_CON_SLA_BV_33                18
#define LL_TEST_MODE_TP_CON_SLA_BV_34                19
#define LL_TEST_MODE_TP_CON_SLA_BI_02                20
#define LL_TEST_MODE_TP_CON_SLA_BI_04                21
#define LL_TEST_MODE_TP_CON_SLA_BI_05                22
#define LL_TEST_MODE_TP_CON_SLA_BI_06                23
#define LL_TEST_MODE_TP_CON_SLA_BI_08                24
#define LL_TEST_MODE_TP_SEC_MAS_BV_08                25
#define LL_TEST_MODE_TP_SEC_SLA_BV_08                26
#define LL_TEST_MODE_TP_ENC_ADV_BI_02                27
#define LL_TEST_MODE_TP_TIM_SLA_BV_05                28
#define LL_TEST_MODE_TP_SEC_MAS_BV_14                29
#define LL_TEST_MODE_TP_SEC_SLA_BI_05                30
#define LL_TEST_MODE_TP_SEC_MAS_BI_07                31
#define LL_TEST_MODE_TP_SEC_MAS_BV_12                32
#define LL_TEST_MODE_TP_SEC_MAS_BV_13                33
#define LL_TEST_MODE_TP_SEC_MAS_BV_04                34
#define LL_TEST_MODE_TP_SEC_MAS_BI_09                35
#define LL_TEST_MODE_TP_SEC_MAS_BI_05                36
#define LL_TEST_MODE_TP_SEC_MAS_BI_04                37
#define LL_TEST_MODE_TP_SEC_MAS_BI_06                38
// V5.0
#define LL_TEST_MODE_TP_CON_SLA_BV_05                50
#define LL_TEST_MODE_TP_CON_SLA_BV_06                51
#define LL_TEST_MODE_TP_CON_SLA_BV_45                52
#define LL_TEST_MODE_TP_CON_SLA_BV_51                53
#define LL_TEST_MODE_TP_CON_MAS_BV_46                54
#define LL_TEST_MODE_TP_CON_SLA_BI_09                55
#define LL_TEST_MODE_TP_SEC_MAS_BV_03                56
// ESR11
#define LL_TEST_MODE_TP_PAC_SLA_BV01                 57
#define LL_TEST_MODE_TP_PAC_MAS_BV01                 58
#define LL_TEST_MODE_TP_PAC_SLA_BI01                 59
#define LL_TEST_MODE_TP_PAC_MAS_BI01                 60
#define LL_TEST_MODE_TP_SEC_MAS_BV05                 66
//
#define LL_TEST_MODE_TP_CON_MAS_BV03                 61
#define LL_TEST_MODE_TP_CON_MAS_BV04                 62
#define LL_TEST_MODE_TP_CON_MAS_BV05                 63
#define LL_TEST_MODE_TP_CON_SLA_BV04                 64
#define LL_TEST_MODE_TP_CON_SLA_BV05                 65
// TS V5.0.2
#define LL_TEST_MODE_TP_CON_SLA_BV59                 66
// TS V5.0.3
#define LL_TEST_MODE_TP_CON_SLA_BV69                 67
#define LL_TEST_MODE_TP_CON_MAS_BV65                 68
#define LL_TEST_MODE_TP_CON_INI_BV03                 69
#define LL_TEST_MODE_TP_DDI_SCN_BV36                 70
// TS V5.3
#define LL_TEST_MODE_TP_CON_ADV_BI_01                71
#define LL_TEST_MODE_TP_CON_ADV_BI_02                72
#define LL_TEST_MODE_TP_ENC_INI_BI_01                80
#define LL_TEST_MODE_TP_CON_INI_BI_02                81
#define LL_TEST_MODE_TP_HCI_CM_BV_04                 82
#define LL_TEST_MODE_TP_CON_MAS_BI_07                83
// Tickets
#define LL_TEST_MODE_JIRA_220                        200
#define LL_TEST_MODE_MISSED_SLV_EVT                  201
#define LL_TEST_MODE_JIRA_2756                       202
#define LL_TEST_MODE_JIRA_3478                       203
#define LL_TEST_MODE_JIRA_3646                       204
#define LL_TEST_MODE_JIRA_4785                       205
#define LL_TEST_MODE_INVALID                         0xFF

typedef struct
{
  uint8  testCase;                                   // Core Test Spec Test Case
  uint32 counter;                                    // General purpose counter.
} llTestMode_t;

#endif // LL_TEST_MODE

// Invalid System Boot Message
#define INVALID_SYSBOOTMSG (uint8 *)0xFFFFFFFF

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

// Device Addresses
extern uint8 ownPublicAddr[];
extern uint8 ownRandomAddr[];
//
extern uint8         LL_TaskID;                       // link layer task ID
extern uint8         llState;                         // link layer state
extern llConns_t     llConns;                         // link layer connection table
extern verInfo_t     verInfo;                         // own version information
extern dtmInfo_t     *dtmInfo;                        // direct test mode data
extern sizeInfo_t    sizeInfo;                        // size info of various data structs
extern buildInfo_t   buildInfo;                       // build revision data
extern featureSet_t  deviceFeatureSet;                // device feature set
extern rfPathComp_t *pRfPathComp;                     // RF Tx Path Compensation data
extern uint16        taskEndStatus;                   // radio task end status
extern int8          rssiCorrection;                  // RSSI correction
extern uint8         onePktPerEvt;                    // one packet per event enable flag
extern uint8         fastTxRespTime;                  // fast TX response time enable flag
extern uint8         rxFifoFlowCtrl;                  // flag to indicate if RX Flow Control is enabled
extern uint8         slOverride;                      // flag for user suspension of SL
extern uint8         numComplPkts;                    // number of completed Tx buffers
extern uint8         numComplPktsLimit;               // minimum number of completed Tx buffers before event
extern uint8         numComplPktsFlush;               // flag to indicate send number of completed buffers at end of event

// V4.1 - LL Topology
extern uint8 *activeConns;

// System Boot Message
extern uint8 *SysBootMsg;

// V4.2 - Extended Data Length
extern uint16 connInitialMaxTxOctets;
extern uint16 connInitialMaxTxTime;
extern uint16 connInitialMaxTxTimeUncoded;
extern uint16 connInitialMaxTxTimeCoded;
//
extern uint16 supportedMaxTxOctets;
extern uint16 supportedMaxTxTime;
extern uint16 supportedMaxRxOctets;
extern uint16 supportedMaxRxTime;

#ifdef LL_TEST_MODE
extern uint16 invalidRxOctets;
extern uint16 invalidRxTime;
extern uint16 invalidTxOctets;
extern uint16 invalidTxTime;
#endif

// V5.0 - 2M and Coded PHY
extern uint8 defaultPhy;

#ifdef LL_TEST_MODE
extern llTestMode_t llTestMode;                      // LL Test Mode test cases
//
extern volatile uint8 firstTx;
extern volatile uint8 timSlvBv05Done;
extern volatile uint8 numSets;
extern volatile uint8 numTxPkts;
extern volatile uint8 nomCI;
extern volatile uint8 numTxEvts;
extern volatile uint8 setFailed;
extern volatile uint8 numFailedSets;
extern volatile uint8 numFailedTx;
#endif // LL_TEST_MODE

// Host Connection Event Notice Callback
extern llConnEvtNotice_t llConnEvtNotice;

// TRNG handle
extern RNG_Handle trngHandle;

// QOS PARAMETERS
//***************
// Qos default parameters
extern uint8  qosDefaultPriorityConnParameter;
extern uint8  qosDefaultPriorityAdvParameter;
extern uint8  qosDefaultPriorityScnParameter;
extern uint8  qosDefaultPriorityInitParameter;
extern uint8  qosDefaultPriorityPerAdvParameter;
extern uint8  qosDefaultPriorityPerScnParameter;
extern uint8  defaultChannelMap[LL_NUM_BYTES_FOR_CHAN_MAP];

extern const uint8 ctrlPktLenTable[NUM_OF_CTRL_PKT];

/*******************************************************************************
 * FUNCTIONS
 */

// Taskend Jump Tables
extern void (*taskEndAction)( void );
/*
** Link Layer Common
*/
uint8                llHaltRadio( uint32 );

uint16               llGetCsConnTaskID( void );
void                 llGetTimeToStableXOSC( void );
uint16               llBleToRfChannel(uint8);
//
// Control Procedure Setup

uint8_t              llSetupCtrlPkt( llConnState_t *connPtr, uint8_t ctrlPkt );
void                 llBuildCtrlPktPeri( llConnState_t *connPtr, uint8 *pData, uint8_t ctrlPkt );
void                 llBuildCtrlPktCent( llConnState_t *connPtr,uint8_t *pData, uint8_t ctrlPkt );
void                 llPostSetupCtrlPktPeri( llConnState_t *connPtr, uint8_t ctrlPkt );
void                 llPostSetupCtrlPktCent( llConnState_t *connPtr, uint8_t ctrlPkt );
//
void                 llSetupDataEntry( RCL_Buffer_TxBuffer *dataEntry, uint8 cmdLen, uint8 encEnabled ); // TODO add C, P thing

// Control Procedure Management
void                 llEnqueueCtrlPkt( llConnState_t *, uint8 );
void                 llDequeueCtrlPkt( llConnState_t * );
void                 llReplaceCtrlPkt( llConnState_t *, uint8, uint8);
void                 llSendReject( llConnState_t *, uint8, uint8 );
void                 llSendRejectInd( llConnState_t * connPtr, uint8_t errorCode );

uint8                llPendingUpdateParam( void );
void                 llInitFeatureSet( void );
void                 llRemoveFromFeatureSet( uint8 byte, uint8 feature );
void                 llConvertCtrlProcTimeoutToEvent( llConnState_t * );
uint8                llVerifyConnParamReqParams( uint16, uint16, uint16, uint8, uint16, uint16 *);
uint8                llValidateConnParams( llConnState_t *, uint16, uint16, uint16, uint16, uint16, uint8, uint16, uint16 *);
void                 llApplyParamUpdate( llConnState_t * );
void                 llRemoveFeaturesForSendToPeer ( uint8 * );

// Data Channel Management
void                 llProcessChanMap( llConnState_t *, uint8 * );
uint8                llGetNextDataChan( llConnState_t *, uint16 );
uint8                llGetNextDataChanAlgo1( llConnState_t * );
uint8                llGetNextDataChanAlgo2( llConnState_t * );
uint8                llReverseBits( uint8 );
uint16               llGenPrnE( uint16, uint16 );
void                 llSetNextDataChan( llConnState_t * );
uint8                llAtLeastTwoChans( uint8 * );
uint8                llGetSlowestPhy( uint8 );

// Connection Management
llConnState_t        *llAllocConnId( void );
void                 llReleaseConnId( llConnState_t * );
void                 llReleaseAllConnId( void );
uint16               llGetMinCI( uint16  );
uint16               llFindNextActiveConnId( uint16 );
uint8                llGetNextConn( void );
uint16               llGetLstoNumOfEventsLeftMargin( uint16 );
uint8_t              llSetStarvationMode( uint16_t , uint8_t );
void                 llRealignConn( llConnState_t *, uint32 );
void                 llSortActiveConns( uint8 *, uint8 );
void                 llShellSortActiveConns(uint8 *activeConns, uint8 numActiveConns);
void                 llConnCleanup( llConnState_t * );
void                 llConnClearRxHeadTail( uint16_t connHandle );
void                 llConnTerminate( llConnState_t *, uint8  );
uint8                llConnExists( uint8 *, uint8 );
uint32               llGenerateCRC( void );
uint8                llEventInRange( uint16 , uint16 , uint16  );
uint16               llEventDelta( uint16 , uint16  );
void                 llConvertLstoToEvent( llConnState_t *, connParam_t * );
uint8                llAdjustForMissedEvent( llConnState_t *, uint32  );
void                 llAlignToNextEvent( llConnState_t *connPtr );
void                 llGetAdvChanPDU( uint8 *, uint8 *, uint8 *, uint8 *, uint8 *, int8 * );
void                 llSecTaskInitiatorHandle( taskInfo_t* secTask, RCL_Command* secCmd, llConnState_t* nextConnPtr, uint32_t* timeGap, uint32_t curTime );
void                 llUpdateTimeGapForInitiator( uint32_t* timeGap );
void                 llUpdateTimeGapForScanWindow( taskInfo_t* secTask, llConnState_t* nextConnPtr, RCL_Command* secCmd, uint32_t* timeGap, uint32_t curTime );
void                 LL_GetConnTxUsageParams( llTxUsageParams_t *pConnTxParams );

// Access Address
uint32               llGenerateValidAccessAddr( void );
uint8                llValidAccessAddr( uint32 );
uint8                llGtSixConsecZerosOrOnes( uint32 );
uint8                llLSBPreamSimilar (uint32);
uint8                llEqSynchWord( uint32 );
uint8                llOneBitSynchWordDiffer( uint32 );
uint8                llEqualBytes( uint32 );
uint8                llGtTwentyFourTransitions( uint32 );
uint8                llLtTwoChangesInLastSixBits( uint32 );
uint8                llEqAlreadyValidAddr( uint32  );
uint8                llLtThreeOnesInLsb( uint32 );
uint8                llGtElevenTransitionsInLsh( uint32 );
// Data Management
uint8                llEnqueueDataQ( llDataQ_t *, txData_t * );
uint8                llEnqueueHeadDataQ( llDataQ_t *, txData_t * );
uint8                llDequeueDataQ( llDataQ_t *, txData_t ** );
uint8                llDataQFull( llDataQ_t * );
uint8                llDataQEmpty( llDataQ_t * );
void                 llProcessTxData( void );
uint8                llWriteTxData( llConnState_t *, uint8 *, uint8 , uint8, uint8 );
void                 llCombinePDU( uint16, uint8 *, uint16, uint8 );
uint8                llFragmentPDU( llConnState_t *, uint8 *, uint16 );
uint8                *llMemCopySrc( uint8 *, uint8 *, uint8 );
uint8                *llMemCopyDst( uint8 *, uint8 *, uint8 );
void                 llUpdateRxBuffersForActiveConnections(List_List *rxBuffers);
void                 llCheckRxBuffers( llConnState_t *connPtr );
void                 llReplaceRxBuffers( llConnState_t * );
uint16               llTime2Octets( uint8, uint8, uint16, uint8 );
uint32               llOctets2Time( uint8, uint8, uint16, uint8 );
uint16               llSetCodedMaxTxTime( llConnState_t * );
uint8                llVerifyCodedConnInterval( llConnState_t *, uint16 );
llConnState_t        *llDataGetConnPtr( uint8 );
uint32_t             llGetTxQueueTotalTime(List_List   *txBuffer,
                                           uint8_t     phy,
                                           uint8_t     coding,
                                           bool        encEnabled);

llStatus_t           llDynamicAlloc( void );
llStatus_t           llDynamicAlloc_connectable( void );
void                 llDynamicFree( void );
void                 llDynamicFree_connectable( void );
llStatus_t           llAllocExtInitInfo( void );
llStatus_t           llAllocExtScanInfo( void );

// Failure Management
void                 llHardwareError( uint8 );

// Advertising Task End Cause
void                 llDirAdv_TaskEnd( void );
void                 llAdv_TaskConnect( void );
void                 llAdv_TaskAbort( void );

// Scanner Task End Cause
void                 llScan_TaskEnd( void );
void                 llProcessScanRxFIFO( uint8 scanStatus );

// Initiator Task End Cause
void                 llInit_TaskConnect( void );
void                 llExtInit_ResolveConnRsp( void );
void                 llSetPeerAddress( uint8_t* advPkt, llConnState_t *connPtr );

// Central Task End Cause
void                 llCentral_TaskEnd( void );
uint8                llProcessCentralControlProcedures( llConnState_t *connPtr );
uint8                llSetupNextCentralEvent( void );

// Peripheral Task End Cause
void                 llPeripheral_TaskEnd( void );
uint8                llSetupNextPeripheralEvent( void );
uint8                llProcessPeripheralControlProcedures( llConnState_t * );
uint8                llCheckForLstoDuringSL( llConnState_t * );
uint8                llCheckPeripheralTerminate( uint8 );
void                 llPeriUpdateConnParamsByInitiatorPhy(llConnState_t *connPtr, llConnType connType, uint8 connReqPhy);
void                 llPeriSetConnAddr(llConnState_t *connPtr, uint8 ownAddrType, const uint8 *pConnReqData);
void                 llPeriSetConnSyncParams(llConnState_t *connPtr, const uint8 *pConnReqData);
void                 llPeriSetConnSCA(llConnState_t *connPtvor, uint16 periSCAValue, const uint8 *pData);
void                 llPeriSetConnChannelParams(llConnState_t *connPtr, llConnType connType, const uint8* pConnReqData);
void                 llPeriSetTimeToNextEvent(llConnState_t *connPtr, llConnType connType, uint8 secPhy);
void                 llPeriSetupConnWindow(llConnState_t *connPtr);
void                 llPeriHandleFeatureExchange(llConnState_t *connPtr);
void                 llSetupConnCmd(llConnState_t *connPtr, uint8 isPeripheral);
void                 llConnSetupBuffers(llConnState_t *connPtr);
uint8_t              llAllocConnection(llConnState_t **connPtr, uint8_t connRole);
bool                 ll_AdvConnIsEnable( void );

// Error Related End Cause
void                 llTaskError( void );

// Accept List Related
llStatus_t           llCheckAcceptListUsage( void );
uint8_t              llChackInitiatorUseAcceptList( void );
uint8_t              llChackScannerUseAcceptList( void );
// Timer Related Management
void                 llCBTimer_AptoExpiredCback( uint8 * );

// Connection Event Notice
void                 llRegisterConnEvtCallback( llConnEvtCB_t cb, uint8_t eventType, uint16_t connHandle );
void                 llSendConnEvtCallback( uint8 connEvtStatus, uint16 numPkts, llConnState_t *connPtr );

// LL Process Event functions
void                 llProcessScanTimeout( void );
void                 llProcessCentralConnectionCreated( void );
void                 llProcessPeripheralConnectionCreated( void );
void                 llProcessAdvAddrResolutionTimeout( void );
void                 llProcessPeriConnectionEstablishFailed( uint8_t reason );

uint8                llConvertBlePhyToLlPhy(uint8 blePhy, uint8 *llPhy, uint8 *llPhyCodedOpt);
uint8                llConvertLlPhyToBlePhy(uint8 llPhy, uint8 *blePhy);
uint8                llConvertLlPhyOptToBlePhyOpt(uint8 llPhyOpt, uint8 *blePhyOpt);
void                 llConvertAePhyToBlePhy(uint8 llPhy, uint8 *blePhy);

uint8                llSetPhy(llConnState_t *connPtr, uint8 rxPhy);
void                 llSetRangeDelay(llConnState_t *connPtr);

uint16               llPhyToPhyFeatures(uint8 primPhy, uint8 secPhy);
uint16               llAuxPhyFeatures(uint8 secPhy);

// RCL- filterlist update
llRclUpdateListStatus_t llRclUpdateFilterList( uint8 operation,
                                               uint8 peerAddrType,
                                               uint8 *peerAddr );

uint8                RfBleDpl_setConnPhy(uint8 connId, uint8 phy, uint8 phyOpts);
uint8                RfBleDpl_setAdvPhy(void *pRfCmd, uint8 primPhy, uint8 secPhy);
void                 RfBleDpl_setRangeDelay(uint8 connId, uint8 rangeDelay);

#ifdef BLE_HEALTH
// Health Toolkit api
uint8_t llDbgInf_addSchedRec(taskInfo_t * const llTask);
uint8_t llDbgInf_addConnTerm(uint16_t connHandle, uint8_t reasonCode);
#endif //BLE_HEALTH

// Health check api
void llHealthUpdateWrapperForOsal(void);
int8_t llHealthCheck(void);
uint8_t llCheckInitHealth(volatile uint32_t* initTime, volatile uint32_t* currentTime);
void llHealthSetThreshold(uint32_t connTime,uint32_t scanTime,uint32_t initTime,uint32_t advTime);
void llHealthSetAESThreshold(uint32_t aesThreshold);
// Health check internal
void    llHealthUpdate(uint8_t state);
uint8_t llCheckConnHealth(uint32_t* currentTime, volatile uint32_t* connTime);
uint8_t llCheckScanHealth(uint32_t* currentTime, volatile uint32_t* scanTime);
uint8_t llCheckAdvHealth(uint32_t* currentTime, volatile uint32_t* advTime);
bool    ll_healthCheckIsEnable( void );

void llCreateCommonFeatureSet( llConnState_t *connPtr, uint8 *pBuf );
void LL_rclAdvTxFinished( void );

// Tx queue api
uint8 llQueryTxQueue(uint32 addr);

// Connection Ind
uint8 llValidateConnectIndPkt( uint8 * );

// Check if there is a control procedure with instant active for a specific connection
uint8 llCheckConnInstant(llConnState_t *connPtr);

// Removes the handover connection from activeConns list
void llRemoveHandoverConn(uint8_t *activeConnsArray, uint8_t numActiveConns);

// Check if there is a control procedure waiting
uint8_t llConnIsActiveCtrlProcedure( llConnState_t *connPtr );

// Check if the TX buffer is empty
uint8_t llConnIsTxBuffEmpty( llConnState_t *connPtr );

// Check if the RX buffer is idle
uint8_t llConnIsRxBuffIdle( const llConnState_t *connPtr);

// Tx Burst API's
void llConnStartTxBurst(llConnState_t *connPtr, uint8_t txBurstRatio);
bool llConnUseTxBurst(llConnState_t *connPtr, uint8_t txBurstRatio);
void llConnEndTxBurst(llConnState_t *connPtr);

// Get the LTK of a given connection handle
uint8_t LL_GetLtk(uint16_t connHandle, uint8_t *pLtk);

void llConnSetRejectIndExt(llConnState_t *connPtr, uint8 rejectOpcode, uint8 errorCode);

#ifdef __cplusplus
}
#endif

#endif /* LL_COMMON_H */
