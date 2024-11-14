/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * ZBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: MAC  layer main module
*/

#ifndef MAC_INTERNAL_INCLUDED
#define MAC_INTERNAL_INCLUDED 1

/** @cond DOXYGEN_MAC_SECTION */

/*! \addtogroup ZB_MAC */
/*! @{ */


#include "zb_common.h"
#include "zb_ringbuffer.h"
#include "zb_mac.h"

/* Table 85 MAC sublayer constants aUnitBackoffPeriod The number of symbols forming the basic time period
used by the CSMA-CA algorithm.
20 */
#define ZB_UINT_BACKOFF_PERIOD 20U
/* Table 22 PHY constants aTurnaroundTime RX-to-TX or TX-to-RX maximum turnaround time
(in symbol periods) (see 6.9.1 and 6.9.2) 12 */
#define ZB_TURNAROUND_TIME 12U
/* Table 23 PHY PIB attributes phySHRDuration  0x06 Integer 3, 7, 10, 40 The duration of the synchronization
header (SHR) in symbols for the current
PHY.

SHR = preamble + SFD.

Table 19 Preamble field length.
2400 2483.5 MHz O-QPSK 4 octets 8 symbols

Table 20 SFD field length
2400 2483.5 MHz O-QPSK 1 octet 2 symbols
*/
#define ZB_PHY_SHR_DURATION 10U /* 3,7,10,40 */
/* Table 23PHY PIB attributes phySymbolsPerOctet

6.5.2.2 Bit-to-symbol mapping
The 4 LSBs (b0, b1, b2, b3) of each octet shall map into one data symbol, and the 4 MSBs (b4, b5, b6, b7) of
each octet shall map into the next data symbol.
 */
#define ZB_PHY_SYMBOLS_PER_OCTET 2U /* 0.4, 1.6, 2, 8 */

/*
 For BO = 15, the unit period
will be aBaseSuperframe-
Duration.

So, it is in our intermal time units
*/
#define ZB_MAC_TRANSACTION_PERSISTENCE_TIME 0x01f4U

#define ZB_MAC_SECURITY_LEVEL 5U
#define ZB_MAC_KEY_ID_MODE    1U


/**
   Defines max scan duration
*/
#define ZB_MAX_SCAN_DURATION 14U


zb_ret_t zb_mac_check_security(zb_bufid_t data_buf);
zb_int8_t mac_check_pending_data(const zb_mac_mhr_t *mhr, zb_uint8_t index);
void zb_mac_indirect_data_rx_failed(zb_uint8_t status);
zb_ret_t zb_mac_call_comm_status_ind(zb_bufid_t pending_buf, zb_mac_status_t mac_status, zb_bufid_t buffer);
void mac_call_associate_confirm_fail(zb_uint8_t param, zb_mac_status_t cb_status);

/* It was mac_is_dup function, renamed it to create macro for compatibility reasons. */
zb_bool_t mac_is_dup_extended(zb_mac_mhr_t *mhr, zb_uint8_t *payload_ptr, zb_uint8_t len);

/* Checks all MAC commands for dups.
   Declared only for compatibility reasons. */
#define mac_is_dup(mhr, len)                            mac_is_dup_extended((mhr), NULL, (len))

/* Checks some MAC commands for dups, also checks interpan and GP packets.
   Supersedes `mac_is_dup` macro. */
#define mac_check_for_dup_using_payload(mhr, payload_ptr, len) mac_is_dup_extended((mhr), (payload_ptr), (len))

#if defined ZB_MAC_TESTING_MODE || defined DOXYGEN
/* Delay in ms before transmitting a packet (needed for MAC certification test) */
#define ZB_MAC_TESTING_FRAME_TX_DELAY 16000

/* Store PAN descriptors in a buffer (needed for MAC certification tests) */
void zb_mac_store_pan_desc(zb_bufid_t beacon_buf, zb_pan_descriptor_t *pan_desc);
#endif /* ZB_MAC_TESTING_MODE || DOXYGEN */

/**
   Time to wait before retransmit
 */
#define ZB_MAC_ACK_WAIT_DURATION() (ZB_MAC_PIB_ACK_WAIT_DURATION + ZB_RANDOM_JTR(ZB_MAC_PIB_ACK_WAIT_DURATION / 4 + 1))

/* receiving packet, suggested to be scheduled in check_int_status*/
/* switching to the next channel*/
void zb_mlme_scan_step(zb_uint8_t param);

void zb_handle_data_request_cmd_continue(zb_uint8_t pending_param);

zb_bool_t mac_pending_queue_is_empty(void);

void zb_mac_reinit_pib(void);

#ifndef ZB_AUTO_ACK_N_RETX
/**
  Function is called on acknowledge timeout
  @param param is unused
 */
void zb_mac_ack_timeout(zb_uint8_t param);
#endif /* ZB_AUTO_ACK_N_RETX */

/**
  Function is called on scan timeout
  @param param is unused
 */
void zb_mac_scan_timeout(zb_uint8_t param);

/**
  Function is called on transaction (pending data) timeout
  @param param is pending data index
 */
void zb_mac_pending_data_timeout(zb_uint8_t param);

/**
  Accept data request function
  @param param - reference to buffer.
 */
void zb_accept_data_request_cmd(zb_uint8_t param);

/**
   Coordinator side: get request command, say ACK to end device,
   signal to high level with associate.indication.
   Also drop association if an association response is provided in theindirect queue.
   @param param - reference to buffer.
*/
void zb_process_ass_request_cmd(zb_uint8_t param);

#ifdef ZB_OPTIONAL_MAC_FEATURES
/**
  Sends orphan notification command, mac spec 7.3.6
  @param param is unused
*/
void zb_orphan_notification_command(zb_uint8_t param);
#endif /* ZB_OPTIONAL_MAC_FEATURES */


/**
  sends association request command
  @return RET_OK, RET_ERROR
*/
void zb_mlme_send_association_req_cmd(zb_uint8_t param);

#ifndef ZB_AUTO_ACK_TX
/**
  sends acknowledgement
  @param ack_dsn - frame sequence number to acknowledge
*/

void zb_mac_send_ack(zb_uint8_t ack_dsn);
#endif /* ZB_AUTO_ACK_TX */

zb_bool_t mac_can_accept_frame(zb_bufid_t buf);

/**
  Sends frame to the transmitter with check for manual acknowledge.
  param hdr_len - frame header size
  param buf - data buffer
  return RET_OK, RET_ERROR
*/
void zb_mac_send_frame(zb_bufid_t buf, zb_uint8_t mhr_len);
/**
  gets pending data from a coordinator (get data indirectly)
  @param params - parameters for data request command
  @return RET_OK, RET_ERROR, RET_BLOCKED, RET_PENDING
  RET_PENDING is returned if there is more data available on
  coordinator

   @b Example
@code
    zb_mlme_data_req_params_t data_req_cmd_params;
    TRACE_MSG(TRACE_MAC3, "RESP_TIMEOUT", (FMT__0));
    data_req_cmd_params.src_addr_mode = ZB_ADDR_64BIT_DEV;
    ZB_MEMCPY(&data_req_cmd_params.src_addr, &MAC_PIB().mac_extended_address, sizeof(zb_ieee_addr_t));
    data_req_cmd_params.dst_addr_mode = params->coord_addr_mode;
    ZB_MEMCPY(&data_req_cmd_params.dst_addr, &params->coord_addr, sizeof(zb_addr_u));
    data_req_cmd_params.cb_type = MAC_ASS_CONFIRM_CALLBACK;
    ZB_MAC_SET_ASS_REQUEST();
    zb_ret_t ret = zb_mac_get_indirect_data(&data_req_cmd_params);
@endcode
*/
void zb_mac_get_indirect_data(zb_mlme_data_req_params_t *params);

/**
  Puts data to pending queue. It is used for indirect
  transmission. Coordinator side
  @return RET_PENDING on success, RET_ERROR on error

   @b Example
@code
 zb_ret_t ret;
 if (data_req_params->tx_options & MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT)
  {
    zb_mac_pending_data_t pend_data;
    pend_data.pending_data = data_req;
    pend_data.dst_addr_mode = data_req_params->dst_addr_mode;
    ZB_MEMCPY(&pend_data.dst_addr, &data_req_params->dst_addr, sizeof(zb_addr_u));

    ret = zb_mac_put_data_to_pending_queue(&pend_data);
  }
  @endcode
 */
zb_ret_t zb_mac_put_data_to_pending_queue(zb_mac_pending_data_t *pend_data) ZB_SDCC_BANKED;

/**
   Handle mac data request, caller side
   @return RET_OK, RET_ERROR, RET_BLOCKED
 */
void zb_handle_mcps_data_req(zb_uint8_t param);

/**
   Handles data frame. Now it is a callback.
 */
void zb_handle_data_frame(zb_uint8_t param);


/**
   Checks that beacon request can be handled.
   This check occurs two times because stack may use delayed processing using ZB_PIB_BEACON_JITTER.
   Two checks are needed to verify that beacon still may be sent after timeout.
   @return ZB_TRUE if beacon may be sent, ZB_FALSE otherwise.
*/
zb_bool_t zb_can_handle_beacon_req(void);

/**
   Handles beacon request
   @return RET_OK, RET_ERROR
 */
void zb_handle_beacon_req(zb_uint8_t param);

void zb_handle_beacon_req_alarm(zb_uint8_t param);

#if defined ZB_ENHANCED_BEACON_SUPPORT

/**
   Handles enhanced beacon request
 */
void zb_handle_enhanced_beacon_req(zb_uint8_t param);

void zb_handle_enhanced_beacon_req_alarm(zb_uint8_t param);

#endif /* ZB_ENHANCED_BEACON_SUPPORT */

/*
  Sends coordinator realignment command
  @param is_broadcast - if TRUE, cmd is broadcast over PAN, if FALSE
  cmd is directed to orphaned device
  @param orphaned_dev_ext_addr - orphaned device extended address
  @param orphaned_dev_short_addr - orphaned device short address,
  maybe set to 0xFFFE

  @return RET_OK if ok, error code on error
*/
zb_ret_t zb_tx_coord_realignment_cmd(zb_bool_t is_broadcast,
                                     zb_ieee_addr_t orphaned_dev_ext_addr,
                                     zb_uint16_t orphaned_dev_short_addr,
                                     zb_uint16_t pan_id,
                                     zb_uint8_t logical_channel,
                                     zb_uint8_t channel_page);

zb_bool_t mac_is_frame_visible(const zb_mac_mhr_t *mhr);

#if defined ZB_LIMIT_VISIBILITY
void zb_mac_visibility_init(void);
#endif

void mac_handle_associate_resp(zb_uint8_t param, zb_mac_mhr_t *mhr, const zb_uint8_t *cmd_ptr);

#ifdef ZB_ENABLE_ZGP_DIRECT
void zb_mac_send_zgpd_frame(zb_uint8_t param);
#endif /* ZB_ENABLE_ZGP_DIRECT */

zb_ret_t zb_mcps_data_request_fill_hdr(zb_bufid_t data_req);

#ifdef ZB_OPTIONAL_MAC_FEATURES
void zb_handle_coord_realignment_cmd(zb_uint8_t param);
zb_ret_t zb_realign_pan(zb_uint8_t param);
#endif /* ZB_OPTIONAL_MAC_FEATURES */

/**
   Synchronizes transceiver state with ZB_PIB_RX_ON_WHEN_IDLE() values.

   IMPORTANT: must not be called when there is a possibility of an ongoing transmission.
   The safest way is to schedule the function via tx_q.
 */
void zb_mac_sync_rx_on_off_with_pib(zb_uint8_t unused);

/**
   Call MCPS-POLL.indication primitive via callback

   @param param - buffer parameter
   @param mhr - parsed MAC header

   @return noting
*/
void zb_mac_call_mcps_poll_indication(zb_uint8_t param, zb_mac_mhr_t *mhr);


/* macResponseWaitTime, in aBaseSuperframeDuration (our time units) */
#define ZB_MAC_PIB_RESPONSE_WAIT_TIME_CONST (ZB_MAC_RESPONSE_WAIT_TIME)
/* See 7.4.2 MAC PIB attributes */
/* Round to ZB_ABASE_SUPERFRAME_DURATION (at least 1) */
#define ZB_MAC_PIB_ACK_WAIT_DURATION_HW_2_4_GHZ_SYMBOLS 0x51U /* from 802.15.4 spec */
#define ZB_MAC_PIB_ACK_WAIT_DURATION_HW_2_4_GHZ_USEC (ZB_MAC_PIB_ACK_WAIT_DURATION_HW_2_4_GHZ_SYMBOLS * ZB_SYMBOL_DURATION_USEC)

#if defined ZB_SUBGHZ_BAND_ENABLED

#define ZB_SUB_GHZ_SYMBOL_DURATION_USEC \
  (ZB_LOGICAL_PAGE_IS_SUB_GHZ_NA_FSK(MAC_PIB().phy_current_page) ? ZB_NA_FSK_SYMBOL_DURATION_USEC : ZB_GB_EU_FSK_SYMBOL_DURATION_USEC)

#define ZB_MAC_PIB_ACK_WAIT_DURATION_HW_SUB_GHZ_SYMBOLS            \
  (ZB_LOGICAL_PAGE_IS_SUB_GHZ_NA_FSK(MAC_PIB().phy_current_page) ? \
   ZB_MAC_PIB_ACK_WAIT_DURATION_HW_NA_FSK_SYMBOLS : ZB_MAC_PIB_ACK_WAIT_DURATION_HW_GB_EU_FSK_SYMBOLS)

#define ZB_MAC_PIB_ACK_WAIT_DURATION_HW_SUB_GHZ_USEC (ZB_MAC_PIB_ACK_WAIT_DURATION_HW_SUB_GHZ_SYMBOLS * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)

#define ZB_MAC_EU_FSK_SHR_LEN_SYMBOLS \
  (ZB_MAC_SUB_GHZ_SHR_LEN_BYTES * ZB_SUB_GHZ_PHY_SYMBOLS_PER_OCTET)

#define ZB_MAC_EU_FSK_PHR_LEN_SYMBOLS \
  (ZB_MAC_SUB_GHZ_PHR_LEN_BYTES * ZB_SUB_GHZ_PHY_SYMBOLS_PER_OCTET)

/*! The minimum permitted off time between a device's own transmissions in microseconds. */
#define ZB_MAC_LBT_TX_MIN_OFF_USEC    (ZB_MAC_LBT_TX_MIN_OFF_SYMBOLS * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)
/*! The minimum permitted off time between a device's own transmissions in ms.*/
#define ZB_MAC_LBT_TX_MIN_OFF_MS      (ZB_MAC_LBT_TX_MIN_OFF_USEC / 1000UL)

/*! The maximum permitted off time between a device's own transmissions in microseconds. */
#define ZB_MAC_LBT_TX_MAX_PKT_USEC    (ZB_MAC_LBT_TX_MAX_PKT_SYMBOLS * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)
/*! The maximum permitted off time between a device's own transmissions in ms.*/
#define ZB_MAC_LBT_TX_MAX_PKT_MS      (ZB_MAC_LBT_TX_MAX_PKT_USEC / 1000U)

/*! The minimum duration a channel should be free */
#define ZB_MAC_LBT_MIN_FREE_SYMBOLS \
    (ZB_LOGICAL_PAGE_IS_SUB_GHZ_NA_FSK(MAC_PIB().phy_current_page) ? ZB_MAC_NA_FSK_LBT_MIN_FREE_SYMBOLS : ZB_MAC_GB_EU_FSK_LBT_MIN_FREE_SYMBOLS)
/*! The minimum duration a channel should be free in microseconds */
#define ZB_MAC_LBT_MIN_FREE_USEC ((zb_time_t)ZB_MAC_LBT_MIN_FREE_SYMBOLS * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)
/*! The minimum duration a channel should be free in ms*/
#define ZB_MAC_LBT_MIN_FREE_MS   (ZB_MAC_LBT_MIN_FREE_USEC / 1000U)

/*! The maximum period of the backoff */
#define ZB_MAC_LBT_MAX_RANDOM_SYMBOLS \
    (ZB_LOGICAL_PAGE_IS_SUB_GHZ_NA_FSK(MAC_PIB().phy_current_page) ? ZB_MAC_NA_FSK_LBT_MAX_RANDOM_SYMBOLS : ZB_MAC_GB_EU_FSK_LBT_MAX_RANDOM_SYMBOLS)
/*! The maximum period of the backoff in microseconds */
#define ZB_MAC_LBT_MAX_RANDOM_USEC    (ZB_MAC_LBT_MAX_RANDOM_SYMBOLS * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)
/*! The maximum period of the backoff in ms */
#define ZB_MAC_LBT_MAX_RANDOM_MS      (ZB_MAC_LBT_MAX_RANDOM_USEC / 1000U)

/*! The minimum period of the backoff */
#define ZB_MAC_LBT_MIN_RANDOM_SYMBOLS \
    (ZB_LOGICAL_PAGE_IS_SUB_GHZ_NA_FSK(MAC_PIB().phy_current_page) ? ZB_MAC_NA_FSK_LBT_MIN_RANDOM_SYMBOLS : ZB_MAC_GB_EU_FSK_LBT_MIN_RANDOM_SYMBOLS)
/*! The minimum period of the backoff in microseconds */
#define ZB_MAC_LBT_MIN_RANDOM_USEC    (ZB_MAC_LBT_MIN_RANDOM_SYMBOLS * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)
/*! The minimum period of the backoff in ms */
#define ZB_MAC_LBT_MIN_RANDOM_MS      (ZB_MAC_LBT_MIN_RANDOM_USEC / 1000U)

/*! The granularity in the random backoff */
#define ZB_MAC_LBT_GRANULARITY_SYMBOLS \
    (ZB_LOGICAL_PAGE_IS_SUB_GHZ_NA_FSK(MAC_PIB().phy_current_page) ? ZB_MAC_NA_FSK_LBT_GRANULARITY_SYMBOLS : ZB_MAC_GB_EU_FSK_LBT_GRANULARITY_SYMBOLS)
/*! The granularity in the random backoff in microseconds */
#define ZB_MAC_LBT_GRANULARITY_USEC    ((zb_time_t)ZB_MAC_LBT_GRANULARITY_SYMBOLS * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)

/*!
*   The minimum pause before acknowledging a received packet in microseconds.
*   This is to allow a transmitting device to change from
*   transmit to receive mode. Starting an ACK before this time
*   may result in the transmitter missing the ACK.
*/
#define ZB_MAC_LBT_ACK_WINDOW_START_SYMBOLS \
    (ZB_LOGICAL_PAGE_IS_SUB_GHZ_NA_FSK(MAC_PIB().phy_current_page) ? ZB_MAC_NA_FSK_LBT_ACK_WINDOW_START_SYMBOLS : ZB_MAC_GB_EU_FSK_LBT_ACK_WINDOW_START_SYMBOLS)
#define ZB_MAC_LBT_ACK_WINDOW_START_USEC \
  (ZB_MAC_LBT_ACK_WINDOW_START_SYMBOLS * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)

/*!
*   The maximum wait time in microseconds before acknowledging a received
*   packet (includes @ref ZB_MAC_LBT_ACK_WINDOW_START_SYMBOLS).
*   This time MUST be shorter than @ref ZB_MAC_LBT_MIN_FREE_SYMBOLS otherwise other
*   devices could interpret the quiet as an opportunity to transmit.
*/
#define ZB_MAC_LBT_ACK_WINDOW_SYMBOLS \
    (ZB_LOGICAL_PAGE_IS_SUB_GHZ_NA_FSK(MAC_PIB().phy_current_page) ? ZB_MAC_NA_FSK_LBT_ACK_WINDOW_SYMBOLS : ZB_MAC_GB_EU_FSK_LBT_ACK_WINDOW_SYMBOLS)
#define ZB_MAC_LBT_ACK_WINDOW_USEC \
  (ZB_MAC_LBT_ACK_WINDOW_SYMBOLS * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)

/*!
*   The maximum wait time in ms before acknowledging a received
*   packet (includes @ref ZB_MAC_LBT_ACK_WINDOW_START_SYMBOLS).
*   This time MUST be shorter than @ref ZB_MAC_LBT_MIN_FREE_SYMBOLS otherwise other
*   devices could interpret the quiet as an opportunity to transmit.
*/
#define ZB_MAC_LBT_ACK_WINDOW_MS      (ZB_MAC_LBT_ACK_WINDOW_USEC / 1000U)
/*!
*   Time before aborting LBT if it cannot find a free slot in microseconds.
*   This value should be set to at least
*   [@ref ZB_MAC_LBT_MIN_FREE_SYMBOLS  + @ref ZB_MAC_LBT_MAX_TX_RETRIES * (@ref ZB_MAC_LBT_MIN_FREE_SYMBOLS + @ref ZB_MAC_LBT_MAX_RANDOM_SYMBOLS) + @ref ZB_MAC_LBT_TX_RX_SWITCH_TIME_SYMBOLS )]
*   to ensure that all re-tries can occur.
*/
#define ZB_MAC_LBT_TIMEOUT_SYMBOLS \
    (ZB_LOGICAL_PAGE_IS_SUB_GHZ_NA_FSK(MAC_PIB().phy_current_page) ? ZB_MAC_NA_FSK_LBT_TIMEOUT_SYMBOLS : ZB_MAC_GB_EU_FSK_LBT_TIMEOUT_SYMBOLS)
#define ZB_MAC_LBT_TIMEOUT_USEC \
  (ZB_MAC_LBT_TIMEOUT_SYMBOLS * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)

/*!
*   Time before aborting LBT if it cannot find a free slot in ms.
*   This value should be set to at least
*   [@ref ZB_MAC_LBT_MIN_FREE_SYMBOLS  + @ref ZB_MAC_LBT_MAX_TX_RETRIES * (@ref ZB_MAC_LBT_MIN_FREE_SYMBOLS + @ref ZB_MAC_LBT_MAX_RANDOM_SYMBOLS) + @ref ZB_MAC_LBT_TX_RX_SWITCH_TIME_SYMBOLS )]
*   to ensure that all re-tries can occur.
*/
#define ZB_MAC_LBT_TIMEOUT_MS      (ZB_MAC_LBT_TIMEOUT_USEC / 1000U)

/*! LBT acknowledgement length for SUB GHZ */
#define ZB_MAC_LBT_SUB_GHZ_ACK_LEN_SYMBOLS (ZB_MAC_ACK_LEN_BYTES * ZB_SUB_GHZ_PHY_SYMBOLS_PER_OCTET)

/*! The period over which the duty cycle is calculated in seconds */
#define ZB_MAC_DUTY_CYCLE_MEASUREMENT_PERIOD_SEC \
 (ZB_MAC_DUTY_CYCLE_MEASUREMENT_PERIOD_SYMBOLS / 1000000U * ZB_SUB_GHZ_SYMBOL_DURATION_USEC)

/*!
 *   Duty cycle monitoring period
 */
#define ZB_MAC_DUTY_CYCLE_TIME_PERIOD_SEC \
  (ZB_MAC_DUTY_CYCLE_MEASUREMENT_PERIOD_SEC / (ZB_MAC_DUTY_CYCLE_BUCKETS - 1U))

#endif /* ZB_SUBGHZ_BAND_ENABLED */

//#define ZB_MAC_PIB_ACK_WAIT_DURATION_CONST (((ZB_UINT_BACKOFF_PERIOD + ZB_TURNAROUND_TIME + ZB_PHY_SHR_DURATION + (zb_uint16_t)(6 * ZB_PHY_SYMBOLS_PER_OCTET)) + ZB_ABASE_SUPERFRAME_DURATION - 1) / ZB_ABASE_SUPERFRAME_DURATION)
/* If follow spec, have less that retransmit 1 timeout. But, in ZGPD flooding
 * stress test frequently got unnecessary retransmits. Tune retransmit timeout
 * to exclude it.
 * In ZB 2003 macAckWaitDuration 54 or 120 symbols == 896us or 1920us
 */
#ifndef ZB_NSNG
#define ZB_MAC_PIB_ACK_WAIT_DURATION_CONST 2U
#define ZB_MAC_PIB_ACK_WAIT_DURATION_CONST_EU_FSK 2U

#define ZB_MAC_PIB_MAX_FRAME_TOTAL_WAIT_TIME_CONST_SUB_GHZ (ZB_MAX_FRAME_TOTAL_WAIT_TIME_SUB_GHZ)
#else
/*
  In Linux nsng (2.4 GHz), ACK is usually received 1-2 ms after pkt sent.
  Old value (8 BI ~124 ms) seems too large because it exceeds frame wait time for 2.4.
  Set ACK wait wait duration to 2 BI to guarantee that device will wait for ACK at least for 1 BI.
  Keep SubGhz constant as is (don't know, if 2 BI value ever been tested anywhere except NS).
 */
#define ZB_MAC_PIB_ACK_WAIT_DURATION_CONST 2U
#define ZB_MAC_PIB_ACK_WAIT_DURATION_CONST_EU_FSK 9U

#define ZB_MAC_PIB_MAX_FRAME_TOTAL_WAIT_TIME_CONST_SUB_GHZ ((ZB_MAX_FRAME_TOTAL_WAIT_TIME_SUB_GHZ) * 2)
#endif

#define ZB_MAC_PIB_MAX_FRAME_TOTAL_WAIT_TIME   MAC_PIB().mac_max_frame_total_wait_time

#define ZB_MAC_PIB_TRANSACTION_PERSISTENCE_TIME_CONST ZB_MAC_TRANSACTION_PERSISTENCE_TIME

#define ZB_MAC_PIB_MAX_FRAME_TOTAL_WAIT_TIME_CONST_2_4_GHZ (ZB_MAX_FRAME_TOTAL_WAIT_TIME_2_4_GHZ)


/* supporting MLME_SET.request for 802.15.4 */
#ifndef ZB_CONFIGURABLE_MAC_PIB

#define ZB_MAC_PIB_RESPONSE_WAIT_TIME ZB_MAC_PIB_RESPONSE_WAIT_TIME_CONST
#define ZB_MAC_PIB_ACK_WAIT_DURATION ZB_MAC_PIB_ACK_WAIT_DURATION_CONST

#define ZB_MAC_PIB_ACK_WAIT_DURATION_SYMBOLS (ZB_UINT_BACKOFF_PERIOD + ZB_TURNAROUND_TIME + ZB_PHY_SHR_DURATION + (zb_uint16_t)(6 * ZB_PHY_SYMBOLS_PER_OCTET))
#define ZB_MAC_PIB_ACK_WAIT_DURATION_USEC (ZB_MAC_PIB_ACK_WAIT_DURATION_SYMBOLS * ZB_SYMBOL_DURATION_USEC)

#define ZB_MAC_PIB_TRANSACTION_PERSISTENCE_TIME ZB_MAC_PIB_TRANSACTION_PERSISTENCE_TIME_CONST

#define ZB_MAC_PIB_MAX_FRAME_RETRIES ZB_MAC_MAX_FRAME_RETRIES
#else
#define ZB_MAC_PIB_RESPONSE_WAIT_TIME           MAC_PIB().mac_response_wait_time
#define ZB_MAC_PIB_ACK_WAIT_DURATION            MAC_PIB().mac_ack_wait_duration
#define ZB_MAC_PIB_TRANSACTION_PERSISTENCE_TIME MAC_PIB().mac_transaction_persistence_time
#define ZB_MAC_PIB_MAX_FRAME_RETRIES            MAC_PIB().mac_max_frame_retries
#define ZB_MAC_PIB_ACK_WAIT_DURATION_USEC       (ZB_MAC_PIB_ACK_WAIT_DURATION * ZB_SYMBOL_DURATION_USEC)

#endif  /* ZB_CONFIGURABLE_MAC_PIB */

#if defined ZB_TRAFFIC_DUMP_ON && !defined ZB_ALIEN_MAC
void zb_mac_dump_mac_ack_iface(zb_bool_t is_out, zb_uint8_t data_pending, zb_uint8_t dsn, zb_uint8_t interface_type, zb_uint8_t interface_id);
#endif /* ZB_TRAFFIC_DUMP_ON && !ZB_ALIEN_MAC */

#ifdef ZB_MAC_PENDING_BIT_SOURCE_MATCHING
zb_mac_status_t zb_mac_src_match_process_cmd(zb_mac_src_match_params_t *match_params);
#endif

zb_int8_t zb_mac_lbt_rssi_threshold_by_page(zb_uint8_t logical_page);

#define ZB_MAC_LBT_RSSI_THRESHOLD_BY_PAGE(logical_page) zb_mac_lbt_rssi_threshold_by_page(logical_page)

/*
 * The return/error codes for all LBT routines.
 * See D.11.2.4 of r22 spec
 */
typedef enum zb_mac_lbt_ret_e
{
  ZB_MAC_LBT_RET_OK      = 0,  /* LBTrcOk: success */
  ZB_MAC_LBT_RET_BUSY    = 1,  /* LBTrcLBTBsy: timeout */
  ZB_MAC_LBT_RET_LBT_MAX = 2,  /* LBTrcLBTMax: retries limit reached */
  ZB_MAC_LBT_RET_TX_MAX  = 3,  /* LBTrcTxMax: maximum TX retries limit reached */
  ZB_MAC_LBT_RET_TX_TO   = 4,  /* LBTrcTxTo: Tx timeout */
  ZB_MAC_LBT_RET_RX_TO   = 5,  /* LBTrcTxTo: Tx timeout */
  ZB_MAC_LBT_RET_ACK     = 6,  /* LBTrcAck: Valid ACK received */
  ZB_MAC_LBT_RET_ACK_TO  = 7,  /* LBTrcAckTo: Ack transmission failed */
  ZB_MAC_LBT_RET_RX_INV  = 8,  /* LBTrcRxInv: Invalid message received */
  ZB_MAC_LBT_RET_TX_TO_PKT = 99, /* LBTrcTxToPkt: Single transmission exceeded */
  ZB_MAC_LBT_RET_TX_TO_MAX = 100 /* LBTrcTxToMax: Transmission have exceeded
                                  * the maximum rate in the last hour */
} zb_mac_lbt_ret_t;

#if defined ZB_SUB_GHZ_LBT && !defined ZB_MACSPLIT_HOST
void zb_mac_lbt_tx(zb_uint8_t mhr_len, zb_bufid_t buf, zb_uint8_t wait_type);

zb_mac_lbt_ret_t zb_mac_lbt_send_ack(zb_uint8_t ack_dsn, zb_time_t frame_timestamp);
#endif /* ZB_SUB_GHZ_LBT && !ZB_MACSPLIT_HOST */

#if defined ZB_MAC_DUTY_CYCLE_MONITORING && !defined ZB_MACSPLIT_HOST

void zb_mac_duty_cycle_mib_init(void);

void zb_mac_duty_cycle_check_mode(void);

void zb_mac_duty_cycle_bump_buckets(void);

void zb_mac_duty_cycle_accumulate_time(zb_uint32_t tx_symbols, zb_bool_t check_mode);

void zb_mac_duty_cycle_update_regulated(zb_uint8_t page);

zb_bool_t zb_mac_duty_cycle_is_limit_exceeded(zb_uint32_t tx_symbols);

void zb_mac_duty_cycle_periodic(zb_uint8_t unused);
#endif /* ZB_MAC_DUTY_CYCLE_MONITORING && !ZB_MACSPLIT_HOST */

zb_ret_t zb_mac_change_channel(zb_uint8_t logical_page, zb_uint8_t logical_channel);
zb_ret_t zb_mac_temp_channel_set(zb_uint8_t logical_channel);
zb_bool_t zb_mac_temp_channel_is_set(void);

void zb_mac_set_tx_power(zb_int8_t new_power);
zb_int8_t zb_mac_get_tx_power(void);

/* MAC power control */

#if defined ZB_MAC_POWER_CONTROL

#define ZB_MAC_POWER_CONTROL_INVALID_POWER_VALUE 0x7F
#define ZB_MAC_POWER_CONTROL_LOCK_POWER_APPLY() (MAC_CTX().flags.power_update_locked = ZB_TRUE)
#define ZB_MAC_POWER_CONTROL_UNLOCK_POWER_APPLY() (MAC_CTX().flags.power_update_locked = ZB_FALSE)
#define ZB_MAC_POWER_CONTROL_IS_POWER_APPLY_LOCKED() (MAC_CTX().flags.power_update_locked)

zb_ret_t zb_mac_power_ctrl_update_ent_by_ieee(zb_ieee_addr_t ieee_addr,
                                              zb_int8_t rx_power,
                                              zb_int8_t rssi,
                                              zb_bool_t create);

zb_ret_t zb_mac_power_ctrl_update_ent(zb_ieee_addr_t ieee_addr,
                                      zb_uint16_t short_addr,
                                      zb_int8_t rx_power,
                                      zb_int8_t rssi,
                                      zb_bool_t create);

/* [VK]: Functions were renamed to avoid MISRAC2012-Rule-5.1
 * The external identifier 'some_name' clashes with other
 * identifier(s) in the first 31 characters 1 time(s).
 */
zb_ret_t zb_mac_pwr_ctrl_tbl_upd_rssi_short(zb_uint16_t short_addr,
                                            zb_int8_t rssi);

zb_ret_t zb_mac_pwr_ctrl_tbl_upd_rssi_ieee(zb_ieee_addr_t ieee_addr,
                                           zb_int8_t rssi);

zb_ret_t zb_mac_pwr_ctrl_get_tx_pwr_ieee(zb_ieee_addr_t ieee_addr,
                                         zb_int8_t *tx_power);

zb_ret_t zb_mac_pwr_ctrl_get_tx_pwr_short(zb_uint16_t short_addr,
                                          zb_int8_t *tx_power);

zb_ret_t zb_mac_pwr_ctrl_apply_tx_pwr_short(zb_uint16_t short_addr);

zb_ret_t zb_mac_pwr_ctrl_apply_tx_pwr_ieee(zb_ieee_addr_t ieee_addr);

void zb_mac_power_ctrl_restore_tx_power(void);

#endif  /* ZB_MAC_POWER_CONTROL */


void zb_mac_send_data_conf(zb_bufid_t param, zb_mac_status_t status, zb_bool_t cut_mhr);

enum zb_radio_stats_events_e
{
  RADIO_STAT_QUEUE_ITEM_FREE,
  RADIO_STAT_ITERATION
};

#ifndef ZB_RADIO_STATISTIC_ENABLE
#ifndef ZB_RADIO_PROCESS_STATISTIC
#define ZB_RADIO_PROCESS_STATISTIC(e)
#endif
#endif

/*
 * Shutdown other radios during critical operations
 * for the specified period of time (in msec).
 * Note: it can be used within MAC layer only,
 * it's not an API for upper layers.
 */
#ifndef ZB_TRANSCEIVER_COEX_SHUTDOWN
#define ZB_TRANSCEIVER_COEX_SHUTDOWN(duration)
#endif

#define ZB_MAC_ENERGY_LEVEL_INVALID 255u

/*! @} */
/** @endcond */

#endif  /* MAC_INTERNAL_INCLUDED */
