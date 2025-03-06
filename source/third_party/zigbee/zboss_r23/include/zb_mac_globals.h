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
/* PURPOSE: MAC layer globals definition
*/

#ifndef ZB_MAC_GLOBALS_H
#define ZB_MAC_GLOBALS_H 1

/*! @cond internals_doc */
/*! \addtogroup ZB_MAC */
/*! @{ */

#include "zb_bufpool.h"
#include "zb_mac.h"
#include "zb_list_macros.h"
#include "zboss_api_core.h"     /* for zb_mac_pending_data_t */

#ifdef ZB_COMPILE_MAC_MONOLITHIC

#ifdef ZB_PHY_TESTING_MODE
#include "zboss_api_mac.h"
#endif /* ZB_PHY_TESTING_MODE */

#if defined(ZB_ALIEN_MAC)
#error "When using alien MAC can't have access to our MAC globals!"
#endif

#define MAC_N_DUPS 12U

#ifdef ZB_MAC_SECURITY
typedef struct zb_mac_device_table_s
{
  zb_ieee_addr_t long_address;
  zb_uint16_t    short_address;
  zb_uint32_t    frame_counter;
  zb_uint16_t    pan_id;
}
zb_mac_device_table_t;
#endif

/* 08/21/2017: MM: CR: MEDIUM: Don't pass last_update_id in this request. It
 * should be stored at ZDO level
 *
 * TBD: Also, I haven't found usage of update_id
 *
 * Naming of "MAC_JOINING_LIST_SIZE_LIMIT"... better to use "ZB_MAC_JOINING_LIST_MAX_SIZE"
 */

#if defined ZB_JOINING_LIST_SUPPORT
/* ieee joining list with necessary attributes */
typedef ZB_PACKED_PRE struct zb_mac_ieee_joining_list_s
{
  zb_uint8_t length;
  zb_ieee_addr_t items[MAC_JOINING_LIST_SIZE_LIMIT];
} ZB_PACKED_STRUCT zb_mac_ieee_joining_list_t;
#endif /* ZB_JOINING_LIST_SUPPORT */

#if defined ZB_MAC_DUTY_CYCLE_MONITORING
ZB_RING_BUFFER_DECLARE(zb_mac_duty_cycle_bucket, zb_uint32_t, ZB_MAC_DUTY_CYCLE_BUCKETS);
#endif  /* ZB_MAC_DUTY_CYCLE_MONITORING */

#if defined ZB_MAC_POWER_CONTROL
typedef struct zb_mac_power_ctrl_info_tbl_s
{
  zb_uint8_t n_used;            /* Number of used entries */
  zb_mac_power_ctrl_info_tbl_ent_t tbl[ZB_MAC_POWER_CONTROL_INFO_TABLE_SIZE];
} zb_mac_power_ctrl_info_tbl_t;
#endif  /* ZB_MAC_POWER_CONTROL */

/**
   MAC PIB
 */
typedef struct zb_mac_pib_s
{
  zb_uint16_t             mac_coord_short_address;       /*!< The 16-bit short address assigned to the coordinator
                                                           through which the device is associated. */
  zb_uint16_t             mac_pan_id;                    /*!< The 16-bit Pan ID on which
                                                           the device is operating. If this value is 0xffff,
                                                           the device is not associated. */
  zb_uint16_t             mac_short_address;             /*!< The 16-bit address that the device uses
                                                           to communicate in the PAN. */
  zb_uint16_t             mac_superframe_order;          /*!< The length of the active portion of the outgoing
                                                           superframe, including the beacon frame. */
  zb_time_t               mac_max_frame_total_wait_time; /*!< The maximum number of CAP symbols in a beaconenabled
                                                           PAN, or symbols in a nonbeacon-enabled PAN, to wait
                                                           either for a frame intended as a response to a
                                                           data request frame or for a broadcast frame following a
                                                           beacon with the Frame Pending subfield set to one. */
#ifdef ZB_CONFIGURABLE_MAC_PIB
  zb_time_t               mac_ack_wait_duration;         /*!< The maximum number of symbols to wait for an
                                                           acknowledgment frame to arrive following a
                                                           transmitted data frame.
                                                           The commencement time is described in 7.5.6.4.2.*/
  zb_uint16_t             mac_transaction_persistence_time; /*!< The maximum time (in unit periods) that
                                                              a transaction is stored by a coordinator and
                                                              indicated in its beacon. */
  zb_uint8_t              mac_rx_on_when_idle;           /*!< Indication of whether the MAC sublayer is to enable
                                                           its receiver during idle periods. */
  zb_uint8_t              mac_dsn;                       /*!< The sequence number added to the transmitted data
                                                           or MAC command frame. */
  zb_uint8_t              mac_max_frame_retries;         /*!< The maximum number of retries allowed after a
                                                           transmission failure. */
  zb_uint8_t              mac_response_wait_time;        /*!< The maximum time, in multiples of aBaseSuperframeDuration,
                                                           a device shall wait for a response command frame to
                                                           be available following a request command frame. */
  zb_uint8_t              mac_association_permit;        /*!< Indication of whether a coordinator is currently
                                                           allowing association. A value of TRUE indicates
                                                           that association is permitted. */
  zb_uint8_t              mac_auto_request;              /*!< Indication of whether a device automatically
                                                           sends a data request command if its address
                                                           is listed in the beacon frame.
                                                           indication primitive (see 7.1.5.1.2). */
  zb_uint8_t              mac_batt_life_ext;             /*!< Indication of whether BLE, through the reduction of
                                                           coordinator receiver operation time during the CAP,
                                                           is enabled. Also, see 7.5.1.4 for an explanation. */
  zb_mac_beacon_payload_t mac_beacon_payload;            /*!< The contents of the beacon payload. */
  zb_uint8_t              mac_beacon_payload_length;     /*!< The length, in octets, of the beacon payload. */
  zb_uint8_t              mac_beacon_order;              /*!< Specification of how often the coordinator
                                                           transmits its beacon. */
  zb_uint8_t              mac_bsn;                       /*!< The sequence number added to the transmitted
                                                           beacon frame. */
#if defined ZB_ENHANCED_BEACON_SUPPORT
  zb_uint8_t              mac_ebsn;                      /*!< The sequence number added to the transmitted
                                                           enhanced beacon frame. */
#endif /* ZB_ENHANCED_BEACON_SUPPORT */
  zb_ieee_addr_t          mac_coord_extended_address;    /*!< The 64-bit address of the coordinator
                                                           through which the device is associated. */
/* next parameters are also unused */
  zb_uint16_t             mac_base_superframe_duration;  /*!< The number of symbols forming a superframe when
                                                           the superframe order is equal to 0. */
  zb_uint8_t              mac_max_csma_backoffs;         /*!< The maximum number of backoffs the CSMA-CA algorithm
                                                           will attempt before declaring a channel access failure. */
  zb_uint8_t              mac_batt_life_ext_periods;     /*!< In BLE mode, the number of backoff periods
                                                           during which the receiver is enabled after
                                                           the IFS following a beacon. */
  zb_uint8_t              mac_gts_permit;                /*!< TRUE if the PAN coordinator is to accept
                                                           GTS requests. FALSE otherwise. */
  zb_uint8_t              mac_min_be;                    /*!< *The minimum value of the backoff exponent (BE)
                                                           in the CSMA-CA algorithm. See 7.5.1.4
                                                           for a detailed explanation */
#endif
  zb_uint8_t              phy_current_page;              /*!< */

  zb_uint8_t              phy_ephemeral_page;              /*!< */

  zb_uint8_t              phy_current_channel;           /*!< Index of current physical channel */

  zb_uint8_t              phy_pending_channel;           /*!< Index of pending physical channel */

  zb_uint8_t              tmp_channel;                   /*!< The device is working on the temp channel physically in case it isn't equal to 0xFF  */

  zb_ieee_addr_t          mac_extended_address;          /*!< The 64-bit (IEEE) address assigned to the device. */
#ifdef ZB_MAC_SECURITY
  zb_mac_device_table_t   mac_device_table[MAC_DEVICE_TABLE_SIZE];
  zb_uint8_t              mac_device_table_entries;
  zb_uint32_t             mac_frame_counter;
  zb_uint8_t              mac_key[16];
#endif
  zb_uint8_t              mac_pan_coordinator;              /*!< Custom field -
                                                             * PANCoordinator
                                                             * field of Beacon frame */
#ifdef ZB_MAC_TESTING_MODE
  zb_uint8_t              mac_no_auto_ack;                  /*!< Disable/enable auto ack on platform */
#endif
#ifdef ZB_PROMISCUOUS_MODE
  zb_callback_t           mac_promiscuous_mode_cb;  /*!< alternative data indication   */
#endif
#if defined(ZB_CONFIGURABLE_MAC_PIB) || defined(ZB_PROMISCUOUS_MODE)
  zb_uint8_t              mac_promiscuous_mode;          /*!< Indication of whether the MAC sublayer is in a
                                                              promiscuous (receive all) mode. */
#endif
#if defined ZB_JOINING_LIST_SUPPORT
  zb_mac_ieee_joining_list_t mac_ieee_joining_list;      /*<!  List of 64-bit IEEE
							                                                 addresses permitted to join.*/

  zb_uint8_t                 mac_joining_policy;            /*<! Mode of join */

  zb_uint32_t                mac_ieee_expiry_interval;      /*<! Timer before clearing mac_joining_ieee_list, in min. */
#endif /* ZB_JOINING_LIST_SUPPORT */

#if defined ZB_MAC_DUTY_CYCLE_MONITORING
  zb_uint32_t                mib_duty_cycle_limited_threshold;
  zb_uint32_t                mib_duty_cycle_critical_threshold;
  zb_uint32_t                mib_duty_cycle_regulated;
  zb_uint32_t                mib_duty_cycle_used;
  zb_mac_duty_cycle_bucket_t mib_duty_cycle_bucket;
  zb_uint8_t                 mib_duty_cycle_status;
#endif  /* ZB_MAC_DUTY_CYCLE_MONITORING */

#if defined ZB_MAC_POWER_CONTROL
  zb_mac_power_ctrl_info_tbl_t power_info_table;
#endif  /* ZB_MAC_POWER_CONTROL */

#if defined ZB_ENABLE_ZGP_DIRECT
  zb_uint8_t            zgp_skip_all_packets;
#endif

  zb_uint8_t            ignore_beacon_reqs;
#if defined ZB_MAC_PIB_CONFIGURABLE_BEACON_JITTER
  zb_uint8_t            mac_beacon_jitter;
#endif

#ifdef ZB_TRANSCEIVER_SET_PTA_OPTIONS
  zb_uint8_t pta_options_len;
  zb_uint8_t pta_options[ZB_MAC_MAX_PTA_OPTIONS_LEN];
  zb_uint8_t pta_tx_prio_escal;
  zb_uint8_t pta_rx_prio_escal;
#endif
#ifdef ZB_TRANSCEIVER_SET_PTA_STATE
  zb_uint8_t pta_state;
#endif
#ifdef ZB_TRANSCEIVER_SET_PTA_PRIORITY
  zb_uint8_t pta_priority;
  zb_uint8_t pta_tx_priority;
  zb_uint8_t pta_rx_priority;
#endif
#ifdef ZB_MAC_COEX_CONTROL
  /* Note: that setting is not compatibe with *PTA* group of defines and
   * flags. It is introdcued for another (already not supported) platform. */
  zb_uint32_t coex_shutdown_duration;
#endif

}
zb_mac_pib_t;


/* PIB access macros */

/* FIXME: do we really need that macros? TODO: remove it and access variables in MAC code. Never access it in the code above MAC. */

/**
   Get mac Pan ID
 */
#define ZB_PIB_SHORT_PAN_ID() MAC_PIB().mac_pan_id
/**
   Get mac short address
 */
#define ZB_PIB_SHORT_ADDRESS() MAC_PIB().mac_short_address
/**
   Get mac extended address
 */
#define ZB_PIB_EXTENDED_ADDRESS() MAC_PIB().mac_extended_address
/**
   Get mac coord short address
 */
#define ZB_PIB_COORD_SHORT_ADDRESS() MAC_PIB().mac_coord_short_address

/**
   Get mac rx on when idle
 */
#define ZB_PIB_RX_ON_WHEN_IDLE() MAC_PIB().mac_rx_on_when_idle

/**
   Get mac DSN
 */
#define ZB_MAC_DSN() MAC_PIB().mac_dsn

/**
   Get mac Pan BSN
 */
#define ZB_MAC_BSN() MAC_PIB().mac_bsn

#if defined ZB_ENHANCED_BEACON_SUPPORT

/**
   Get mac pan EBSN
 */
#define ZB_MAC_EBSN() MAC_PIB().mac_ebsn

#endif /* ZB_ENHANCED_BEACON_SUPPORT */

#ifdef ZB_MAC_PIB_CONFIGURABLE_BEACON_JITTER
#define ZB_PIB_BEACON_JITTER() MAC_PIB().mac_beacon_jitter
#else
#ifdef ZB_SEND_BEACON_AFTER_RANDOM_DELAY
#define ZB_PIB_BEACON_JITTER() ZB_MAC_HANDLE_BEACON_REQ_HI_TMO
#else
#define ZB_PIB_BEACON_JITTER() 0
#endif /* ZB_SEND_BEACON_AFTER_RANDOM_DELAY */
#endif /* ZB_MAC_PIB_CONFIGURABLE_BEACON_JITTER */

#ifdef ZB_ENABLE_ZGP_DIRECT
#define ZB_MAC_GET_SKIP_GPDF() MAC_PIB().zgp_skip_all_packets
#else
#define ZB_MAC_GET_SKIP_GPDF() ZB_TRUE
#endif

/**
   Increment mac Pan DSN
 */
#ifdef ZB_ZGPD_ROLE
/* Function to generate random mac_dsn if zgpd device need it, implementation placed in zb_zgpd.c */
zb_uint8_t zb_zgpd_mac_num_gen();
#define ZB_INC_MAC_DSN() (zb_zgpd_mac_num_gen())
#else
#define ZB_INC_MAC_DSN() (MAC_PIB().mac_dsn++)
#endif
/**
   Increment mac Pan BSN
 */
#define ZB_INC_MAC_BSN() (MAC_PIB().mac_bsn++)

#if defined ZB_ENHANCED_BEACON_SUPPORT

/**
   Increment mac pan EBSN
 */
#define ZB_INC_MAC_EBSN() (MAC_PIB().mac_ebsn++)

#endif /* ZB_ENHANCED_BEACON_SUPPORT */

/**
   Get mac beacon payload
 */
#define ZB_PIB_BEACON_PAYLOAD() MAC_PIB().mac_beacon_payload

#ifndef ZB_RADIO_INT_DISABLE
#define ZB_RADIO_INT_DISABLE()  ZB_OSIF_GLOBAL_LOCK()
#endif /* ZB_RADIO_INT_DISABLE */

#ifndef ZB_RADIO_INT_ENABLE
#define ZB_RADIO_INT_ENABLE()   ZB_OSIF_GLOBAL_UNLOCK()
#endif /* ZB_RADIO_INT_ENABLE */

#define ZB_SET_MAC_STATUS(status) (MAC_CTX().mac_status = (status))

/* 'MAC_CTX().mac_status + 0' is used in the following macro to prevent any assignments to
 * 'MAC_CTX().mac_status' */
#define ZB_GET_MAC_STATUS() (MAC_CTX().mac_status + 0U)


#ifdef ZB_MANUAL_ACK

#define ZB_MAC_INVALID_DSN 0x1FFU
#define ZB_MAC_GET_ACK_NEEDED()  (MAC_ICTX().ack_dsn != ZB_MAC_INVALID_DSN)

#ifndef ZB_TRANSCEIVER_ACK_ACCEPT
#define ZB_TRANSCEIVER_ACK_ACCEPT(x) x
#endif
#define ZB_MAC_CLEAR_ACK_NEEDED() ZB_RADIO_INT_DISABLE(); MAC_ICTX().ack_dsn = ZB_MAC_INVALID_DSN; ZB_RADIO_INT_ENABLE()

#define ZB_MAC_CLR_ACK_RECEIVED() (MAC_ICTX().ack_received = 0U)
#define ZB_MAC_SET_ACK_RECEIVED(pend) (MAC_ICTX().ack_received = (1U | (((zb_uint_t)ZB_B2U((pend) != 0U)) << 1U)))
#define ZB_MAC_GET_ACK_RECEIVED() (MAC_ICTX().ack_received)
#define ZB_MAC_GET_ACK_RECEIVED_PEND() (MAC_ICTX().ack_received & (~1U))

#define ZB_MAC_GET_ACK_TIMED_OUT()   (MAC_CTX().flags.mac_ack_timed_out)
#define ZB_MAC_SET_ACK_TIMED_OUT()   (MAC_CTX().flags.mac_ack_timed_out = ZB_TRUE)
#define ZB_MAC_CLEAR_ACK_TIMED_OUT() (MAC_CTX().flags.mac_ack_timed_out = ZB_FALSE)

#else  /* ZB_MANUAL_ACK */

/* define to 0 to produce conditions like if (0) and exclude conditional compilation */
#define ZB_MAC_GET_ACK_NEEDED() ZB_FALSE
#define ZB_MAC_GET_ACK_TIMED_OUT() ZB_FALSE
#define ZB_MAC_CLEAR_ACK_NEEDED()
#define ZB_MAC_SET_ACK_NEEDED()
#define ZB_MAC_SET_ACK_TIMED_OUT()
#define ZB_MAC_CLEAR_ACK_TIMED_OUT()

#define ZB_MAC_CLR_ACK_RECEIVED()
#define ZB_MAC_SET_ACK_RECEIVED(pend)
#define ZB_MAC_GET_ACK_RECEIVED() ZB_FALSE
#define ZB_MAC_GET_ACK_RECEIVED_PEND() ZB_FALSE

#endif  /* ZB_MANUAL_ACK */


#ifndef ZB_MAC_TESTING_MODE
#define ZB_ACTIVE_SCAN_MAX_PAN_DESC_COUNT 2U
#else
#ifdef ZB_MACSPLIT
/* It's not possible to transfer more PAN descriptors at once over MAC-Split */
#define ZB_ACTIVE_SCAN_MAX_PAN_DESC_COUNT 6U
#else
#define ZB_ACTIVE_SCAN_MAX_PAN_DESC_COUNT 10U
#endif /* ZB_MACSPLIT */
#endif /* ZB_MAC_TESTING_MODE */

#define ZB_MAC_TX_POWER_INVALID_DBM    127

#ifndef ZB_CONFIGURABLE_MEM
/**
   pending transactions queue size is implementation-dependent but
   must be at least one
*/
#ifndef ZB_MAC_PENDING_QUEUE_SIZE
#define ZB_MAC_PENDING_QUEUE_SIZE (ZB_IOBUF_POOL_SIZE / 4U)
#endif
#endif

  /* Pending bitmap size. Each bit refers to corresponding element in child hash table */
#define ZB_PENDING_BITMAP_SIZE ((ZB_CHILD_HASH_TABLE_SIZE + 31U) / 32U)

/* Receive queue routines */
#ifdef ZB_MAC_RX_QUEUE_CAP

/* main ring buffer, that contains whole packets itself */
ZB_RING_BUFFER_DECLARE(zb_rx_queue, zb_uint8_t, ZB_MAC_RX_QUEUE_CAP);

#endif  /* ZB_USE_RX_QUEUE */


#define POLL_INDICATION_SHORT_ADDR 2U /*16-bit short address*/
#define POLL_INDICATION_EXT_ADDR   3U /*64-bit extended address*/

/**
   Data structure to store dups info
*/
typedef struct zb_mac_dups_s
{
  zb_mac_mhr_t mhr;
  zb_uint8_t   len;
  zb_time_t    expire;
  zb_uint16_t  nwk_fcf;
} zb_mac_dups_t;


#ifdef ZB_PHY_TESTING_MODE
typedef ZB_PACKED_PRE struct zb_mac_phy_testing_ctx_s
{
  /*
   * Test state activated.
   * Set to true when a device receives the first test request, validates its
   * parameters, and the test started. Once done, the active field can't be
   * switched to the normal mode (i.e. put back to false).
   */
  zb_bool_t active;
  /* Remaining TX/RX iterations count */
  zb_uint16_t remaining_pkt_counter;
  /* MAC Retries counter */
  zb_uint8_t retry_counter;
  /* Valid rx packets counter */
  zb_uint32_t rx_counter;
  /* Last received MAC ACK DSN (to filter out MAC dups) */
  zb_uint16_t ack_dsn;
  /*
   * Requested In/Out buffer with
   * 1) Zigbee header
   * 2) Pattern payload
   * 3) zb_phy_testing_request_t parameter
   */
  zb_bufid_t buf;
  /*
   * Current test mode can be:
   *   a) set to default values
   *     (current_test_mode == ZB_PHY_TESTING_STOP and active == false)
   *   b) executed
   *     (current_test_mode set and active == true)
   *   c) stopped by the host application
   *     (current_test_mode == ZB_PHY_TESTING_STOP and active == true)
   *   d) automatically stopped by reaching test end
   *     (current_test_mode == ZB_PHY_TESTING_STOP and active == true)
   */
  zb_phy_testing_mode_t current_test_mode;
  zb_phy_testing_state_t state_machine_iteration;
  /* Total number of performed iterations */
  zb_uint8_t performed_counter;
  /* Total number of channel busy reports by CCA */
  zb_uint8_t cca_busy_counter;
  /* List of RSSI measurements during a test*/
  zb_uint8_t rssi_list[ZB_PHY_TESTING_MAX_CCA_OR_ED_SCAN_COUNT];
  /* Dump every received packet to host trace */
  zb_bool_t send_rx_packets_to_host;
} ZB_PACKED_STRUCT
zb_mac_phy_testing_ctx_t;
#endif /* ZB_PHY_TESTING_MODE*/

/*
 * It is not possible to define association states as anonymous enum because MISRA treats anonymous
 * values as essentially signed. However, association state is stored in zb_mac_ctx_s::ass_state
 * which is unsigned.
 */
#define ZB_MAC_ASS_STATE_NONE           0U
#define ZB_MAC_ASS_STATE_REQ_SENDING    1U
#define ZB_MAC_ASS_STATE_REQ_SENT       2U
#define ZB_MAC_ASS_STATE_POLLING        3U
#define ZB_MAC_ASS_STATE_POLL_FAILED    4U
#define ZB_MAC_ASS_STATE_POLL_RETRY     5U

/**
   MAC global context
 */
typedef struct zb_mac_ctx_s
{
  zb_bufid_t              pending_buf;            /* zb_bufid_t  for handling any service requests */
  zb_callback_t         tx_wait_cb;             /*!< */
  zb_uint8_t            tx_wait_cb_arg;         /*!< */

#ifdef ZB_MAC_RX_QUEUE_CAP
  zb_rx_queue_t         mac_rx_queue;
#endif
  zb_bufid_t              operation_buf;          /*!< Buffer with command data */
#ifndef ZB_AUTO_ACK_TX
  zb_bufid_t              ack_tx_buf; /*!< buffer used to send ACKs  */
#endif
  /* A separate buffer must be used for receiving data, use no set
   * recv buffer before sending address while reading registers; if
   * recv buffer is not set when byte is received, data will be lost */

  zb_bufid_t              retx_buf;
  zb_uint8_t            retx_len;

  zb_mac_status_t       mac_status; /*!< MAC status of the last operation.
                                     * @note If operation is successful, status is not updated! */

  struct
  {
    zb_bitbool_t      poll_inprogress:1;            /*!< poll request is in progress */
    zb_bitbool_t      in_pending_data:1;            /*!< data pending from the parent. Can't
                                       * go asleep */

    zb_bitfield_t  ass_state:3;  /*!< State of association process */
    zb_bitfield_t  ass_indir_retries:2; /*!< Number of data req attempt during association */
#define ZB_MAC_INDIRECT_IN_PROGRESS() (MAC_CTX().flags.poll_inprogress || MAC_CTX().flags.ass_state == ZB_MAC_ASS_STATE_POLLING)

    zb_bitbool_t      mac_ack_timed_out:1;          /*!< ACK wait timed out  */

    zb_bitbool_t      tx_q_busy:1;                  /*!< tx is busy or no ack, can't send next data frame  */
    zb_bitbool_t      tx_radio_busy:1;              /*!< transmitter is busy: can't send neither
                                                     * data nor ack */
#if ! defined ZB_IS_TRANSPORT_BUSY
#define ZB_IS_TRANSPORT_BUSY() MAC_CTX().flags.tx_radio_busy
#endif /* ! defined ZB_IS_TRANSPORT_BUSY */
    zb_bitbool_t      tx_ok_to_send:1;              /*!< it is ok to send now (used for debug only)  */
    zb_bitbool_t      pending_bit:1;                /*!< pending bit in the to be send by
                                     * manual ack  */
    zb_bitbool_t      transmitting_ack:1;           /*!< if 1, ACK TX is now in progress */
    zb_bitbool_t      mlme_scan_in_progress:1;      /*!< 1 if active, ed or orphan scan
                                              * is in progress */
    zb_bitbool_t      mlme_scan_cancelled:1;      /*!< 1 if active, ed or orphan scan
                                                     * is cancelled */
    zb_bitbool_t      active_scan_beacon_found:1;   /*!< at least 1 beacon found
                                                * during active scan  */
    zb_bitbool_t      scan_timeout:1;               /*!< scan process timed out  */

    zb_bitbool_t      got_realignment:1;
    zb_bitbool_t      ed_next_pass:1;
    zb_bitbool_t      scan_ch_change_failed:1;
#if defined ZB_SUB_GHZ_LBT
    zb_bitbool_t      lbt_radio_busy:1;
#endif
#if defined ZB_MAC_SYNC_RSSI
    zb_bitbool_t      sync_rssi_in_progess:1;
#endif
#if defined ZB_MAC_POWER_CONTROL
    zb_bitbool_t      power_update_locked:1;
#endif     /* ZB_MAC_POWER_CONTROL */
  } flags;                      /*!< flags to store internal state */

  zb_uint8_t retry_counter;     /*!< packet re-send counter */

  /* Scan data */
  zb_uint32_t unscanned_channels;   /*!< bits array*/
  zb_uint8_t  save_page;
  zb_uint8_t  save_channel;
  zb_uint8_t  scan_type;
  zb_time_t   scan_timeout;
  zb_uint8_t  scan_iface_id;
#ifdef ZB_MAC_TESTING_MODE
  struct
  {
    zb_uint8_t buf_param;
    zb_uint8_t stop_scan;
  }
  active_scan;
#endif

#ifndef ZB_ED_ROLE
#ifndef ZB_CONFIGURABLE_MEM
#ifdef ZB_MAC_SOFTWARE_PB_MATCHING
  /* Hash table of rfd child devices with open addressing
   * Each element contains short address of a device
   * 0x0000 means absence of element in table */
  zb_uint16_t child_hash_table[ZB_CHILD_HASH_TABLE_SIZE];
  /* Each bit refers to corresponding child_hash_table element. */
  zb_uint32_t pending_bitmap[ZB_PENDING_BITMAP_SIZE];
#ifdef ZB_MAC_POLL_INDICATION_CALLS_REDUCED
  /* The last MCPS.Poll-Indication call timestamp for a child (in Beacon intervals) */
  zb_time_t poll_timestamp_table[ZB_CHILD_HASH_TABLE_SIZE];
#endif /* ZB_MAC_POLL_INDICATION_CALLS_REDUCED */
#endif  /* ZB_MAC_SOFTWARE_PB_MATCHING */
#else
#ifdef ZB_MAC_SOFTWARE_PB_MATCHING
  zb_uint16_t *child_hash_table;
  zb_uint32_t *pending_bitmap;
#ifdef ZB_MAC_POLL_INDICATION_CALLS_REDUCED
  zb_time_t *poll_timestamp_table;
#endif /* ZB_MAC_POLL_INDICATION_CALLS_REDUCED */
#endif /* ZB_MAC_SOFTWARE_PB_MATCHING */
#endif /* !ZB_CONFIGURABLE_MEM */

#if defined ZB_MAC_SWITCHABLE_PB_MATCHING
  zb_uint8_t src_matching_type;
#endif /* ZB_MAC_SWITCHABLE_PB_MATCHING */

#endif /* !ZB_ED_ROLE */

/*  void (*beacon_sent)(void); */  /*Not needed by now but seems to be useful for certification tests*/

  zb_callback_t ethernet_cb;

  zb_mac_dups_t dups[MAC_N_DUPS];

#if defined ZB_MAC_TESTING_MODE
  zb_time_t tx_timestamp;

  struct
  {
    zb_bitfield_t allow_empty_beacon_payload:1;   /* Allow sending/receiving empty Beacon payload */
    zb_bitfield_t allow_sending_empty_frames:1;   /* Allow responding with empty frames to indirect transmission */
    zb_bitfield_t reset_init_only_radio:1;        /* Only init radio during
                                                   * MLME-RESET.request (for TP_154_MAC_WARM_START_01) */
    zb_bitfield_t lbt_radio_busy_disabled:1 ;     /* Disable blocking radio by LBT while sending frame
                                                   * TP/154/MAC/CHANNEL-ACCESS-04 test procedure 2 */
    zb_bitfield_t ack_wait_disabled:1;            /* MAC ack waiting and retransmissions disabled for
                                                   * TP/154/MAC/CHANNEL-ACCESS-03 test*/
    zb_bitfield_t keep_pkt_in_pending_queue:1;    /* Don't send packet from pending queue even if got data_req */
    zb_bitfield_t reserved: 2;                    /* padding */
  } cert_hacks;
#endif
  zb_time_t poll_rate; /*!< MAC poll rate */

#if defined ZB_MAC_SYNC_RSSI
  zb_int8_t max_measured_sync_rssi;
#endif
  zb_int8_t default_tx_power;           /* Default TX power used for normal
                                         * operations, except MAC Power control overrides*/
#ifdef ZB_MAC_CONFIGURABLE_TX_POWER
  zb_tx_power_provider_t tx_power_provider;

  struct
  {
    zb_bool_t initialized;
    zb_int8_t page0[ZB_PAGE0_2_4_GHZ_CHANNEL_TO - ZB_PAGE0_2_4_GHZ_CHANNEL_FROM + 1U];
#if defined ZB_SUBGHZ_BAND_ENABLED
    zb_int8_t page23[ZB_PAGE23_SUB_GHZ_CHANNEL_TO - ZB_PAGE23_SUB_GHZ_CHANNEL_FROM + 1U];
    zb_int8_t page24[ZB_PAGE24_SUB_GHZ_CHANNEL_TO - ZB_PAGE24_SUB_GHZ_CHANNEL_FROM + 1U];
    zb_int8_t page25_26_27[ZB_PAGE27_SUB_GHZ_CHANNEL_TO - ZB_PAGE25_SUB_GHZ_CHANNEL_FROM + 1U];
    zb_int8_t page28_29_30[ZB_PAGES_28_29_30_SUG_GHZ_CHANNEL_TO - ZB_PAGES_28_29_30_SUG_GHZ_CHANNEL_FROM + 1U];
    zb_int8_t page31[ZB_PAGE31_SUB_GHZ_CHANNEL_TO - ZB_PAGE31_SUB_GHZ_CHANNEL_FROM + 1U];
#endif
  } channel_pages;

#endif

#if defined ZB_MAC_DIAGNOSTICS
  zb_mac_diagnostic_ctx_t    diagnostic_ctx;
#endif
#ifdef ZB_PHY_TESTING_MODE
 zb_mac_phy_testing_ctx_t phy_testing_ctx;
#endif
} zb_mac_ctx_t;


/**
  MAC layer globals.

  Now separated from the whole stack globals to be able to use alien MAC with
  the stack.

  @note MAC PIB and mac ctx are accessible only from the MAC layer.
 */
typedef struct zb_mac_globals_s
{
  zb_uint8_t iface_id;          /*!< Monolithic MAC interface id */
  zb_mac_pib_t    pib;          /*!< MAC_PIB */
  zb_mac_ctx_t    mac_ctx;      /*!< MAC_CTX */
} zb_mac_globals_t;


typedef struct zb_intr_mac_globals_s
{
  zb_transceiver_ctx_t transceiver; /*!< */

  /* Data to share between transceiver-specific logic and MAC. Can be accessed
   * from interrupt handlers. */

  zb_uint_t ack_dsn; /*!< dsn value, is used to check received
                      * acknowledgement. not uint8 but uint to speedup access
                      * from int handler */
  zb_uint_t ack_received;       /*!< if 0, no ack received. else, check for
                                 * pending bit also  */
} zb_intr_mac_globals_t;

#ifndef ZB_BOOTLOADER
extern volatile zb_intr_mac_globals_t g_imac;
#endif /* !ZB_BOOTLOADER */

extern zb_mac_globals_t g_mac;

#define GET_MONOLITHIC_MAC_IFACE_ID() (g_mac.iface_id)
#define GET_MONOLITHIC_MAC_ENCRYPTION_BUF() SEC_CTX_ENCRYPTION_BUF(GET_MONOLITHIC_MAC_IFACE_ID())

/**
   Get MAC context
 */
#define MAC_CTX() (g_mac.mac_ctx)

/**
   Get MAC interrupt & transceiver context
 */
#define MAC_ICTX() (g_imac)

/**
   Get MAC PIB
 */
#define MAC_PIB() g_mac.pib

#define TRANS_CTX() (g_imac.transceiver)

#ifdef ZB_PHY_TESTING_MODE
#define PHY_TEST_CTX() (g_mac.mac_ctx.phy_testing_ctx)
#endif

#ifndef ZB_GET_HW_LONG_ADDR
#define ZB_GET_HW_LONG_ADDR(a) ZB_BZERO(a, sizeof(zb_ieee_addr_t))
#endif

#endif /* ZB_COMPILE_MAC_MONOLITHIC */

/*! @} */
/*! @endcond */

#endif /* ZB_MAC_GLOBALS_H */
