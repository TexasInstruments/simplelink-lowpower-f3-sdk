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
/*  PURPOSE: MAC-Split internals
*/

#ifndef ZB_MACSPLIT_INTERNAL_H
#define ZB_MACSPLIT_INTERNAL_H 1

#include "zb_types.h"
#include "zb_stack_info.h"
#if defined ZB_MACSPLIT_HOST && defined ZB_MAC_API_TRACE_PRIMITIVES
#include "mac_internal.h"
#endif /* ZB_MACSPLIT_HOST && ZB_MAC_API_TRACE_PRIMITIVES */

/* MAC-Split protocol version that provides an ability for developer to track changes */
/* is changed only when the back compatibility is broken */
#define ZB_MACSPLIT_API_VERSION_MAJOR 1u
/* is changed every time when the protocol is extended with some new API calls */
#define ZB_MACSPLIT_API_VERSION_MINOR 1u

/**
 * Type for call types, see MAC-Split TRANSPORT PROTOCOL section for possible call types
 */
typedef zb_uint16_t zb_transport_call_type_t;

#define ZB_TRANSPORT_CALL_TYPE_NO_TYPE   0xBBU
/* 0xFFU may be interpreted as ZB_MAC_TRANSPORT_TYPE_OTA_PROTOCOL. See hacks in MAC-Split transport logic */
#define ZB_TRANSPORT_CALL_TYPE_OTA       0xFFU


typedef enum zb_transport_host_calls_e
{
  ZB_TRANSPORT_CALL_TYPE_HOST_RESET                                    = 0x01, /* it is not used now */
  ZB_TRANSPORT_CALL_TYPE_HOST_MCPS_DATA_REQUEST                        = 0x02,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_GET_REQUEST                         = 0x03,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_SET_REQUEST                         = 0x04,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_RESET_REQUEST                       = 0x05,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_SCAN_REQUEST                        = 0x06,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_ASSOCIATE_REQUEST                   = 0x07,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_ASSOCIATE_RESPONSE                  = 0x08,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_POLL_REQUEST                        = 0x09,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_ORPHAN_RESPONSE                     = 0x0A,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_START_REQUEST                       = 0x0B,
  ZB_TRANSPORT_CALL_TYPE_HOST_SEND_EMPTY_FRAME                         = 0x0C,
  ZB_TRANSPORT_CALL_TYPE_HOST_MCPS_PURGE_INDIRECT_QUEUE_REQUEST        = 0x0D,
  ZB_TRANSPORT_CALL_TYPE_HOST_MACSPLIT_GET_VERSION_REQUEST             = 0x0E,
  ZB_TRANSPORT_CALL_TYPE_HOST_SEND_MAC_TX_POWER                        = 0x0F,  /* send TX powers array from Host to Radio */
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_ADD_VISIBLE_LONG                     = 0x10,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_ADD_INVISIBLE_SHORT                  = 0x11,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_REMOVE_INVISIBLE_SHORT               = 0x12,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_CLEAR_FILTERS                        = 0x13,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_SYNC_ADDRESS_UPDATE                  = 0x14,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_SEND_BEACON_REQUEST                  = 0x15,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_SEND_ENHANCED_BEACON_REQUEST         = 0x16,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_CONFIGURE_FOR_SENDING                = 0x17,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_SET_POWER_INFORMATION_TABLE_REQUEST = 0x18,
  ZB_TRANSPORT_CALL_TYPE_HOST_PHY_TESTING_MODE_REQUEST                 = 0x19,
  ZB_TRANSPORT_CALL_TYPE_HOST_PHY_TESTING_MODE_STATISTICS_REQUEST      = 0x1A,
  ZB_TRANSPORT_CALL_TYPE_HOST_MLME_PURGE_REQUEST                       = 0x1B,
  ZB_TRANSPORT_CALL_TYPE_HOST_PLME_CCA_REQUEST                         = 0x1C,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_CANCEL_SCAN                          = 0x1D,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_SET_TX_POWER                         = 0x1E,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_GET_TX_POWER                         = 0x1F,
  ZB_TRANSPORT_CALL_TYPE_HOST_BLOCK_NO_ADDR_FRAMES                     = 0x20,
  ZB_TRANSPORT_CALL_TYPE_HOST_MAC_SEND_BEACON                          = 0x21
} zb_transport_host_calls_t;

typedef enum zb_transport_device_calls_e
{
  ZB_TRANSPORT_CALL_TYPE_DEVICE_BOOT                              = 0x00,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MCPS_DATA_INDICATION              = 0x01,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MCPS_DATA_CONFIRM                 = 0x02,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_GET_CONFIRM                  = 0x03,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_SET_CONFIRM                  = 0x04,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_RESET_CONFIRM                = 0x05,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_SCAN_CONFIRM                 = 0x06,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_ASSOCIATE_CONFIRM            = 0x07,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_ASSOCIATE_INDICATION         = 0x08,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_POLL_CONFIRM                 = 0x09,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_ORPHAN_INDICATION            = 0x0A,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_START_CONFIRM                = 0x0B,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MCPS_POLL_INDICATION              = 0x0C,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_BEACON_NOTIFY_INDICATION     = 0x0D,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_COMM_STATUS_INDICATION       = 0x0E,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_GP_MCPS_DATA_INDICATION           = 0x0F,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MCPS_PURGE_INDIRECT_QUEUE_CONFIRM = 0x10,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_DUTY_CYCLE_MODE_INDICATION   = 0x11,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_GET_POWER_INFO_TABLE_CONFIRM = 0x12,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_SET_POWER_INFO_TABLE_CONFIRM = 0x13,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MACSPLIT_GET_VERSION_CONFIRM      = 0x14,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_PHY_TESTING_MODE_NOTIFICATION     = 0x15,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MLME_PURGE_CONFIRM                = 0x16,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_PLME_CCA_CONFIRM                  = 0x17,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MAC_CANCEL_SCAN_CONFIRM           = 0x18,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MAC_SET_TX_POWER_CONFIRM          = 0x19,
  ZB_TRANSPORT_CALL_TYPE_DEVICE_MAC_GET_TX_POWER_CONFIRM          = 0x1A,
} zb_transport_device_calls_t;

typedef ZB_PACKED_PRE struct zb_macsplit_sync_ieee_addr_update_s
{
  zb_ieee_addr_t ieee_address;
  zb_uint16_t short_address;
  zb_bool_t lock;
} ZB_PACKED_STRUCT
zb_macsplit_sync_ieee_addr_update_t;

typedef ZB_PACKED_PRE struct zb_macsplit_add_visible_device_s
{
  zb_ieee_addr_t ieee_address;
  zb_uint16_t short_address;
} ZB_PACKED_STRUCT
zb_macsplit_add_visible_device_t;

/* Most of zb_mcps_data_confirm_params_t's fields are the same as in zb_mcps_data_req_params_t
 * Let's omit them to reduce amount of data sent between Host and Mac
 * */
typedef ZB_PACKED_PRE struct zb_macsplit_data_confirm_params_s
{
#if defined ZB_MAC_TESTING_MODE
  zb_time_t timestamp;      /**< Timestamp of TX done */
#endif
} ZB_PACKED_STRUCT
zb_macsplit_data_confirm_params_t;

typedef void (*zb_device_stack_information_callback)(zb_stack_information_t*);

/**
 * Get stack information from device.
 *
 * @param cb - called when device send information about it's stack
 *
 * @return nothing
 */
void zb_macsplit_get_device_stack_information(zb_device_stack_information_callback cb);

/**
 * Function for handling outcoming call.
 *
 * @param ref - ref to buffer to be handling
 *
 * @return nothing
 */
void zb_mcps_data_confirm(zb_bufid_t param);
void zb_mcps_data_indication(zb_bufid_t param);
void zb_mcps_poll_indication(zb_bufid_t param);
void zb_mlme_get_confirm(zb_bufid_t param);
void zb_mlme_set_confirm(zb_bufid_t param);
void zb_mlme_reset_confirm(zb_bufid_t param);
void zb_mlme_scan_confirm(zb_bufid_t param);
void zb_mlme_beacon_notify_indication(zb_bufid_t param);
void zb_mlme_associate_indication(zb_bufid_t param);
void zb_mlme_associate_confirm(zb_bufid_t param);
void zb_mlme_comm_status_indication(zb_bufid_t param);
void zb_mlme_orphan_indication(zb_bufid_t param);
void zb_mcps_data_request_macsplit(zb_bufid_t param);
void zb_mlme_get_request_macsplit(zb_bufid_t param);
void zb_mlme_set_request_macsplit(zb_bufid_t param);
void zb_mlme_reset_request_macsplit(zb_bufid_t param);
void zb_mlme_scan_request_macsplit(zb_bufid_t param);
void zb_mlme_associate_request_macsplit(zb_bufid_t param);
void zb_mlme_associate_response_macsplit(zb_bufid_t param);
void zb_mlme_poll_request_macsplit(zb_bufid_t param);
void zb_mlme_orphan_response_macsplit(zb_uint8_t param);
void zb_mlme_start_request_macsplit(zb_uint8_t param);
void zb_mcps_purge_indirect_queue_request_macsplit(zb_bufid_t param);
void zb_macsplit_transport_boot_indication_call_timeout(zb_bufid_t param);
void macsplit_indicate_boot(void);
zb_bool_t zb_handle_boot_indication_call(zb_bufid_t param);
void zb_mlme_dev_send_mac_tx_power(zb_bufid_t param);
void zb_macsplit_transport_handle_device_boot_call(zb_bufid_t param);
void zb_macsplit_get_device_version_request(zb_bufid_t param);
void zb_macsplit_get_device_version_confirm(zb_bufid_t param);
void zb_macsplit_send_device_stack_information(zb_bufid_t param);
void zb_mac_cancel_scan_macsplit(zb_bufid_t param);

#if !defined(ZB_MACSPLIT_HOST) || (!defined(ZB_MAC_INTERFACE_SINGLE) && defined(ZB_MAC_CONFIGURABLE_TX_POWER))
/* Prototypes for the enabled ZB_MAC_INTERFACE_SINGLE defined in the public API */
void zb_set_tx_power_async_macsplit(zb_bufid_t param);
void zb_get_tx_power_async_macsplit(zb_bufid_t param);
#endif /* #ifdef ZB_MAC_CONFIGURABLE_TX_POWER */

#if defined(ZB_MAC_INTERFACE_SINGLE) && defined(ZB_MACSPLIT_HOST)
/* Redefine MAC API function in the case if MAC-Split is
   the only interface (needed for MAC certification tests).
 */
#define zb_mcps_data_request                  zb_mcps_data_request_macsplit
#define zb_mlme_get_request                   zb_mlme_get_request_macsplit
#define zb_mlme_set_request                   zb_mlme_set_request_macsplit
#define zb_mlme_reset_request                 zb_mlme_reset_request_macsplit
#define zb_mlme_scan_request                  zb_mlme_scan_request_macsplit
#define zb_mlme_associate_request             zb_mlme_associate_request_macsplit
#define zb_mlme_associate_response            zb_mlme_associate_response_macsplit
#define zb_mlme_poll_request                  zb_mlme_poll_request_macsplit
#define zb_mlme_orphan_response               zb_mlme_orphan_response_macsplit
#define zb_mlme_start_request                 zb_mlme_start_request_macsplit
#define zb_mcps_purge_indirect_queue_request  zb_mcps_purge_indirect_queue_request_macsplit
#define zb_mac_cancel_scan                    zb_mac_cancel_scan_macsplit
#endif /* defined(ZB_MAC_INTERFACE_SINGLE) && defined(ZB_MACSPLIT_HOST) */

/**
 * Function for handling incoming call.
 *
 * @param ref - ref to buffer to be handling
 * @param call_type - incoming call type
 *
 * @return nothing
 */
void zb_macsplit_handle_call(zb_bufid_t param, zb_transport_call_type_t call_type);

zb_bool_t zb_macsplit_call_is_conf(zb_transport_call_type_t call_type);

#if defined ZB_MACSPLIT_DEVICE
void zb_macsplit_transport_put_trace_bytes(const zb_uint8_t *buf, zb_short_t len);
void zb_macsplit_transport_send_trace(zb_uint8_t *buf, zb_short_t len);
void zb_macsplit_transport_flush_trace(void);
#endif

#if defined ZB_MACSPLIT_HOST
zb_ret_t zb_macsplit_transport_secure_frame(zb_bufid_t param);
void zb_macsplit_host_set_skip_startup_signal(zb_bufid_t param);
/**
 * @brief Tries to send ZB_MACSPLIT_DEVICE_BOOT and ZB_ZDO_SIGNAL_SKIP_STARTUP signals
 *          if ZBOSS DOESN'T start automatically.
 *        Send these signals only after initialization completed on both host and SoC.
 *        Need it to exclude race between these signals.
 *        Now, ZB_MACSPLIT_DEVICE_BOOT always should be sent first.
 *
 *        If ZBOSS starts automatically, sends ZB_MACSPLIT_DEVICE_BOOT unconditionally.
 * @param param unused
 */
void zb_macsplit_host_try_send_dev_boot_sig_and_skip_startup_sig(zb_bufid_t param);
#endif /* ZB_MACSPLIT_HOST */

#ifdef ZB_TRACE_LEVEL
void zb_macsplit_transport_handle_trace_packet(void);
#endif

#if defined ZB_TRAFFIC_DUMP_ON && defined ZB_DUMP_OVER_MACSPLIT
void zb_macsplit_transport_handle_dump_packet(void);
#endif

void zb_macsplit_transport_init_io(void);
void zb_macsplit_transport_deinit_io(void);
zb_bool_t zb_macsplit_transport_is_open(void);

zb_uint8_t zb_macsplit_host_tx_data_hook(zb_uint8_t param, zb_bool_t *free_buf);
zb_uint8_t zb_macsplit_device_tx_data_hook(zb_uint8_t param, zb_bool_t *free_buf);

void zb_macsplit_transport_start_recv_packet_timer(void);
zb_bool_t zb_macsplit_transport_needs_confirm_timer(zb_transport_call_type_t call_type, zb_bufid_t param);
void zb_macsplit_transport_start_confirm_timer(zb_bufid_t param);
void zb_macsplit_transport_start_set_power_confirm_timer(zb_bufid_t param);
void zb_macsplit_transport_start_get_power_confirm_timer(zb_bufid_t param);

void zb_macsplit_transport_send_not_ack(zb_uint8_t packet_number);

void zb_macsplit_transport_stop_recv_packet_timer(void);
void zb_macsplit_transport_stop_wait_for_ack_timer(zb_uint8_t num);
void zb_macsplit_transport_stop_busy_timer(void);

void zb_macsplit_transport_handle_packet(void);

void zb_macsplit_transport_recv_byte(zb_uint8_t byte);

void zb_macsplit_spinel_recv_pkt(zb_uint8_t *ppkt, zb_uint_t pkt_len);

void zb_macsplit_trace_dump_to_file(zb_uint8_t *rx_buf, zb_uint8_t len);

#define GET_MACSPLIT_MAC_ENCRYPTION_BUF() SEC_CTX_ENCRYPTION_BUF(GET_MACSPLIT_IFACE_ID())
#define GET_MACSPLIT_MAC_ENCRYPTION_BUF2() SEC_CTX_ENCRYPTION_BUF2(GET_MACSPLIT_IFACE_ID())

void zb_macsplit_transport_confirm_timeout(zb_bufid_t param);

void macsplit_host_boot(void);
void macsplit_dev_boot(void);

#endif /* ZB_MACSPLIT_INTERNAL_H */
