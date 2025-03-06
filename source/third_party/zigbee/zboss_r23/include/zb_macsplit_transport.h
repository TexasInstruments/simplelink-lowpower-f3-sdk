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
/*  PURPOSE: MAC-Split API definitions
*/

#ifndef ZB_MACSPLIT_TRANSPORT_H
#define ZB_MACSPLIT_TRANSPORT_H 1

#include "zb_common.h"
#include "zb_types.h"
#include "zb_ringbuffer.h"
#include "zb_config.h"
#include "zb_macsplit_internal.h"
#include "zb_mac_transport.h"
#include "zb_ota_transport.h"

#ifdef ZB_MACSPLIT_SPINEL
#include "zb_hdlc.h"
#endif /* ZB_MACSPLIT_SPINEL */

/****************************** MAC-SPLIT TRANSPORT API ******************************/

/**
 * Type for callback which is used by upper layer for non-call packets handler establishment
 */
typedef void (*recv_data_cb_t)(zb_uint8_t);

/**
 * MAC-Split transport initializer,
 * should be called before sending any data
 *
 * @return nothing
 */
void zb_macsplit_transport_init(void);

/**
 * MAC-Split transport re-initializer,
 *
 * @return nothing
 */
void zb_macsplit_transport_reinit(void);

/**
 * MAC-Split transport de-initializer,
 *
 * @return nothing
 */
void zb_macsplit_transport_deinit(void);

#if defined ZB_MACSPLIT_TRACE_DUMP_TO_FILE
/**
 * Provides MAC-Split trace file path.
 *
 * Weak default implementation provided. Re-implement for custom logic.
 */
void zb_macsplit_trace_dump_file_path_get(char* file_name);

void zb_macsplit_trace_dump_file_init(void);
void zb_macsplit_trace_dump_file_deinit(void);
#endif /* defined ZB_MACSPLIT_TRACE_DUMP_TO_FILE */

/**
 * Function for non-call packets handler establishment,
 * callback can be set at any time,
 * if no callback is set then all non-call packets will be discarded
 *
 * @param cb - non-call packets handler
 *
 * @return nothing
 */
void zb_macsplit_set_cb_recv_data(recv_data_cb_t cb);

/**
 * Function for non-call packet sending,
 * on receipt such a packet, handler set by upper layer is invoked
 * with buffer as function parameter,
 * if no handler set, this packet is discarded
 *
 * @param param - bufid of buffer to be transmitted
 *
 * @return nothing
 */
void zb_macsplit_transport_send_data(zb_bufid_t param);

/**
 * Function for call packet sending,
 * on receipt of this packet, an appropriate handler is invoked (usually, MAC primitive)
 * with buffer as function parameter
 *
 * @param param - bufid of buffer to be transmitted
 * @param call_type - call type (for possible call types, see MAC-SPLIT TRANSPORT PROTOCOL section)
 *
 * @return nothing
 */
void zb_macsplit_transport_send_data_with_type(zb_bufid_t param, zb_transport_call_type_t call_type);


void zb_macsplit_send_ota_msg(zb_bufid_t param);

#ifdef ZB_MACSPLIT_DEVICE
/**
 * Type for callback which is used by upper layer for non-call packets handler establishment
 */
typedef zb_uint32_t (*get_mac_device_version_cb_t)();

/**
 * Function for getting device version from app layer
 * Be sure your app setup this cb before mac initialization
 * if no callback is set then default version 1.0.0.0 will be applied
 *
 */
void zb_macsplit_set_cb_dev_version(get_mac_device_version_cb_t cb);
#endif  /* ZB_MACSPLIT_DEVICE */

/**
 * Function for handling specific incoming call.
 *
 * @param param - ref to buffer to be handling
 *
 * @return nothing
 */
#if defined ZB_MACSPLIT_HOST
void zb_macsplit_transport_handle_data_confirm_call(zb_bufid_t param);
void zb_macsplit_transport_handle_set_get_confirm_call(zb_bufid_t param, zb_transport_call_type_t call_type);
void zb_macsplit_transport_handle_reset_confirm_call(zb_bufid_t param);
void zb_macsplit_transport_handle_start_confirm_call(zb_bufid_t param);
void zb_macsplit_transport_handle_scan_confirm_call(zb_bufid_t param);
void zb_macsplit_transport_handle_cancel_scan_confirm_call(zb_bufid_t param);
#ifdef ZB_MAC_TESTING_MODE
void zb_macsplit_transport_handle_purge_request_call(zb_bufid_t param);
void zb_macsplit_transport_handle_purge_confirm_call(zb_bufid_t param);
#endif /* ZB_MAC_TESTING_MODE */
#ifdef ZB_MAC_CONFIGURABLE_TX_POWER
void zb_macsplit_transport_handle_set_power_confirm_call(zb_bufid_t param);
void zb_macsplit_transport_handle_get_power_confirm_call(zb_bufid_t param);
#endif /* ZB_MAC_TESTING_MODE */
#endif /* ZB_MACSPLIT_HOST */

/**
 * Enumeration for transport types
 */
typedef enum
{
  ZB_MACSPLIT_TRANSPORT_TYPE_SPI = 0,
  ZB_MACSPLIT_TRANSPORT_TYPE_SERIAL,
  ZB_MACSPLIT_TRANSPORT_TYPE_USERIAL,
  ZB_MACSPLIT_TRANSPORT_TYPE_MUX,
  ZB_MACSPLIT_TRANSPORT_TYPES_N
}
zb_macsplit_transport_type_e;

/****************************** MAC-Split TRANSPORT CONFIG ******************************/
/* ES: ZB_MACSPLIT_USE_IO_BUFFERS is disabled for a few platforms actually */
#if !defined ZB_MACSPLIT_DISABLE_IO_BUFFERS
#define ZB_MACSPLIT_USE_IO_BUFFERS
#endif

#if defined ZB_MACSPLIT_USE_IO_BUFFERS

void zb_macsplit_transport_recv_bytes(void);

#ifndef ZB_MACSPLIT_TRANSPORT_BUFFER_CAPACITY
#define ZB_MACSPLIT_TRANSPORT_BUFFER_CAPACITY 512U
#endif /* ZB_MACSPLIT_TRANSPORT_BUFFER_CAPACITY */
ZB_RING_BUFFER_DECLARE(zb_macsplit_transport_buffer, zb_uint8_t, ZB_MACSPLIT_TRANSPORT_BUFFER_CAPACITY);
#endif /* defined ZB_MACSPLIT_USE_RX_BUFFER */

#define ZB_MACSPLIT_TRANSPORT_TX_QUEUE_SIZE   ZB_IOBUF_POOL_SIZE
#ifndef ZB_CONFIGURABLE_MEM
  /* static memory configuration */
  ZB_RING_BUFFER_DECLARE(zb_macsplit_transport_tx_queue, zb_uint8_t, ZB_MACSPLIT_TRANSPORT_TX_QUEUE_SIZE);
#endif

#define ZB_CRC16_INITIAL_VALUE 0U
#define ZB_CRC8_INITIAL_VALUE  0U

#if defined ZB_PLATFORM_LINUX
  #define TRANSPORT_BUFFER_SIZE 1024U
  ZB_RING_BUFFER_DECLARE(zb_transport_buffer, zb_uint8_t, TRANSPORT_BUFFER_SIZE);

  /* name of file where trace and dump from device is stored */
  #define DUMP_FILENAME "device.dump"
#endif /* defined ZB_PLATFORM_LINUX */


/****************************** MAC-SPLIT TRANSPORT PROTOCOL ******************************/

/**
 * Description of protocol for MAC-Split architecture
 *
 * This protocol is used for transmitting stack calls between host and device,
 * transmitting dump and trace from device to host.
 *
 * Packet structure:
 *   header:
 *     length of the packet: 1 byte - length includes header and body, doesn't include signature
 *     type of the packet: 1 byte - dump, trace or MAC-Split data
 *     flags: 1 byte - is_ack, should_retransmit, packet number and call type  | in case of non-MAC-Split packet
 *     crc: 1 byte - crc, if MAC-Split data                                    | these 2 bytes are used for time field
 *   body:
 *     data : ZB_TRANSPORT_DATA_SIZE bytes
 *     crc: 2 bytes
 *
 */
#ifdef ZB_TH_ENABLED
#define ZB_MACSPLIT_RECV_DATA_TIMEOUT    400U    /* in msec. */
#else
#define ZB_MACSPLIT_RECV_DATA_TIMEOUT    200U    /* in msec. Increase to 200. */
#endif
/**
 *  Retransmit timeout (in milliseconds)
 *
 *  Attention: retransmit timeout is related to maximum count of retransmit
 *  attempts. If you decrease retransmit timeout you should increase maximum
 *  count of retransmit attempts.
 */
#define ZB_MACSPLIT_RETRANSMIT_TIMEOUT_MS   500U

/**
 *  Retransmit timeout (in milliseconds) when transport is busy
 *
 *  Attention: retransmit timeout is related to maximum count of retransmit
 *  attempts. If you decrease retransmit timeout you should increase maximum
 *  count of retransmit attempts.
 */
#define ZB_MACSPLIT_RETRANSMIT_TIMEOUT_TRANSPORT_BUSY_MS   50U

/**
 *  Maximum count of retransmit attempts
 *
 *  Attention: set bigger count of retransmit attempts for the Host side to be
 *  sure that there is real OOM state (or buffers leakage) on the Radio side
 */
#if defined ZB_MACSPLIT_HOST
#define ZB_MACSPLIT_RETRANSMIT_MAX_COUNT 12U
#else
#define ZB_MACSPLIT_RETRANSMIT_MAX_COUNT 12U
#endif /* ZB_MACSPLIT_HOST */

#if defined ZB_MACSPLIT_HOST
#define ZB_MACSPLIT_CONFIRM_TIMEOUT      10000U  /* in msec */
#endif /* ZB_MACSPLIT_HOST */

#if defined ZB_MACSPLIT_DEVICE
#define ZB_MACSPLIT_BOOT_INDICATION_RETRANSMIT_TIMEOUT_MS  ((ZB_MACSPLIT_RETRANSMIT_MAX_COUNT + 1) * ZB_MACSPLIT_RETRANSMIT_TIMEOUT_MS)
#endif  /* ZB_MACSPLIT_DEVICE */

#define SIGNATURE_SIZE                   2U
#define SIGNATURE_FIRST_BYTE             0xDEU
#define SIGNATURE_SECOND_BYTE            0xADU

#define ZB_TRANSPORT_MAX_PACKET_NUMBER   3U
#define ZB_TRANSPORT_MIN_PACKET_NUMBER   0U

#ifndef ZB_MACSPLIT_SPINEL
#define ZB_TRANSPORT_BODY_CRC_SIZE       2U
#else
#define ZB_TRANSPORT_BODY_CRC_SIZE       0U
#endif
/* TODO: adjust size */
#define ZB_TRANSPORT_DATA_SIZE           200U
#define ZB_TRANSPORT_PKT_MAX_SIZE \
  (SIGNATURE_SIZE + sizeof(zb_macsplit_packet_t))

#ifndef ZB_MACSPLIT_TRACE_BUF_SIZE
#define ZB_MACSPLIT_TRACE_BUF_SIZE ZB_TRANSPORT_PKT_MAX_SIZE
#endif /* ZB_MACSPLIT_TRACE_BUF_SIZE */

/* Note: ZB_MACSPLIT_TX_WIN_SIZE = 3 was initially supposed, but now does not work due to packet length limited by 2 bits. That's enough only for windows 2,
   otherwise it's impossible to distinguish a duplicate from a hole. E.g. Host received packets #0, #1, #2, acked packets #0, #1 and #2 and now waits for packet #3.
   If packet #0 is received instead of expected one, Host can't say whether it missed packet #3 and should send a NACK #3 or it was a duplicate of previous packet. */
#ifndef ZB_MACSPLIT_TX_WIN_SIZE
#define ZB_MACSPLIT_TX_WIN_SIZE 2u
#endif /* !ZB_MACSPLIT_TX_WIN_SIZE */

#if ((ZB_MACSPLIT_TX_WIN_SIZE) < 1U || (ZB_MACSPLIT_TX_WIN_SIZE) > 2U)
#error "ZB_MACSPLIT_TX_WIN_SIZE has an invalid value! Only 1 and 2 are allowed"
#endif /* ZB_MACSPLIT_TX_WIN_SIZE < 1U || ZB_MACSPLIT_TX_WIN_SIZE > 2U */

#define ZB_MACSPLIT_INVALID_PKT_NUM 0xffu

#define ZB_MACSPLIT_MAX_PACKET_NUMBER 4u /* size 4 because packet number size is 2 bits */

/* 08/22/2018 EE CR:MINOR Better put trace file name size and names into the vendor file. */
#define ZB_TRACE_DUMP_FILE_PATH_SIZE     30U

typedef ZB_PACKED_PRE struct zb_transport_flags_s
{
  zb_bitfield_t               is_ack            : 1;
  zb_bitfield_t               should_retransmit : 1;
  zb_bitfield_t               packet_number     : 2;
  zb_bitfield_t               ack_number        : 2;
  zb_bitfield_t               first_frag        : 1;
  zb_bitfield_t               last_frag         : 1;
} ZB_PACKED_STRUCT
zb_transport_flags_t;


typedef ZB_PACKED_PRE struct zb_macsplit_transport_hdr_s
{
  zb_uint8_t           len;
  zb_uint8_t           type;
  zb_transport_flags_t flags;
  zb_uint8_t           crc;
} ZB_PACKED_STRUCT
zb_macsplit_transport_hdr_t;

/* Ensure that transport headers are always of the same size */
ZB_ASSERT_COMPILE_DECL((sizeof(zb_macsplit_transport_hdr_t) == sizeof(zb_mac_transport_hdr_t)));

typedef ZB_PACKED_PRE struct zb_macsplit_transport_body_s
{
  zb_uint8_t msdu_handle;
  zb_uint16_t call_type;
  zb_uint8_t data[ZB_TRANSPORT_DATA_SIZE];
#ifndef ZB_MACSPLIT_SPINEL
  /* Spinel has its own crc, so that one is not necessary */
  zb_uint16_t crc;
#endif
} ZB_PACKED_STRUCT
  zb_macsplit_transport_body_t;

typedef struct zb_macsplit_packet_s
{
  zb_macsplit_transport_hdr_t hdr;
  zb_macsplit_transport_body_t body;
}
  zb_macsplit_packet_t;

/**
   Serialized buffer header.
 */
typedef ZB_PACKED_PRE struct zb_buf_ser_hdr_s
{
  zb_uint8_t data_len;
  zb_uint8_t tail_len;          /*!< full tail length. real transferred can be less due to zeroes packing  */
  zb_uint8_t status;
  zb_bitfield_t trailing_zeroes:5;
#define ZB_BUF_SER_MAX_TRAILING_ZEROES ((1u<<5u)-1u)
  zb_bitfield_t encr_flags:3;
} ZB_PACKED_STRUCT zb_buf_ser_hdr_t;

typedef enum zb_transport_state_e
{
  /* signature */
  RECEIVING_SIGNATURE,

  /* header/common */
  RECEIVING_LENGTH,
  RECEIVING_TYPE,

  /* header/mac_data */
  RECEIVING_FLAGS,
  RECEIVING_HDR_CRC,

  /* header/trace */
  RECEIVING_TIME,

  /* body/mac_data */
  RECEIVING_MSDU_HANDLE,
  RECEIVING_CALL_TYPE,


  /* body/common */
  RECEIVING_BODY_DATA,
  RECEIVING_BODY_CRC

} zb_transport_state_t;

#define ZB_RECV_CFM_NBYTES ((ZB_IOBUF_POOL_SIZE + 7U) / 8U)

typedef struct zb_macsplit_transport_specific_s
{
#ifndef ZB_CONFIGURABLE_MEM

#if defined ZB_MACSPLIT_DEVICE
  zb_uint8_t msdu_handles[ZB_N_BUF_IDS];  /*!< store msdu_handle from host */
#else
  zb_callback_t confirm_cb[ZB_N_BUF_IDS]; /*!< store callbacks by host */
  zb_uint8_t    recv_cfm[ZB_RECV_CFM_NBYTES]; /*!< store received confirm packets buf id */
#endif /* ZB_MACSPLIT_DEVICE */

#else /* ZB_CONFIGURABLE_MEM */

#if defined ZB_MACSPLIT_DEVICE
  zb_uint8_t* msdu_handles;
#else
  zb_callback_t* confirm_cb;
  zb_uint8_t   * recv_cfm;
#endif /* ZB_MACSPLIT_DEVICE */

#endif /* ZB_CONFIGURABLE_MEM */

#ifndef ZB_MACSPLIT_DEVICE
#endif

} zb_macsplit_transport_specific_t;

#if defined ZB_MACSPLIT_HOST
typedef zb_ret_t (*zb_macsplit_device_trace_cb_t)(zb_uint8_t *buf, zb_uint8_t len);
void zb_macsplit_set_device_trace_cb(zb_macsplit_device_trace_cb_t cb);
#endif

#ifdef ZB_MACSPLIT_HANDLE_DATA_BY_APP
typedef void (*zb_macsplit_handle_data_by_app)(zb_uint8_t byte);
#endif

typedef struct zb_macsplit_transport_context_s
{
  zb_uint8_t iface_id;
#if defined ZB_MACSPLIT_USE_IO_BUFFERS
  ZB_VOLATILE zb_macsplit_transport_buffer_t   rx_buffer;   /*!< buffer for incoming data used by osif layer */
  ZB_VOLATILE zb_macsplit_transport_buffer_t   tx_buffer;   /*!< buffer for outcoming data used by osif layer */
#endif
#if defined ZB_MACSPLIT_DEVICE && (defined ZB_SERIAL_FOR_TRACE || defined ZB_TRACE_OVER_USART || defined ZB_DUMP_OVER_MACSPLIT)
  zb_uint8_t                       trace_buffer[ZB_MACSPLIT_TRACE_BUF_SIZE]; /*!< inner buffer for sending trace to HW */
  zb_short_t                       trace_buffer_data;
#endif
  zb_uint8_t                       tx_inner_buffer[ZB_TRANSPORT_PKT_MAX_SIZE]; /*!< inner buffer for sending data to HW driver */
  zb_uint8_t                       free_param_table[ZB_MACSPLIT_MAX_PACKET_NUMBER];
#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t                       tx_calls_table[ZB_N_BUF_IDS];         /*!< translation table buffer to call-type */
  zb_macsplit_transport_tx_queue_t tx_queue;                /*!< queue for outcoming calls */
#else
  /* If configurable memory, no buffer allocate here. Instead assign a pointer in zb_init_configurable_mem(). */
  zb_byte_array_t  *tx_queue;
  zb_uint8_t       *tx_calls_table;
#endif
  recv_data_cb_t                   recv_data_cb;            /*!< callback for non-call packet handler */
  zb_callback_t                    ethernet_cb;             /*!< just to compile, used in zb_scheduler.c */
  zb_uint16_t                      received_bytes;          /*!< number of bytes received in current FSM state */
  zb_transport_state_t             transport_state;         /*!< current FSM state */
  zb_macsplit_packet_t             rx_pkt;                  /*!< last (or is currently being) received packet */
  zb_macsplit_packet_t             tx_pkt;                  /*!< last (or is currently being) sent packet */
  zb_macsplit_transport_hdr_t      ack_pkt;                 /*!< ack packet */
  zb_uint8_t                       ack_to_tx;               /*!< ack# to be transmitted, ZB_MACSPLIT_INVALID_PKT_NUM if no ack to tx */
  zb_uint8_t                       nack_to_tx;              /*!< nack# to be transmitted, ZB_MACSPLIT_INVALID_PKT_NUM if no nack to tx */
  zb_uint8_t                       tx_win_used;             /*!< number of sent unacket packets  */
  zb_uint8_t                       tx_win_size;             /*!< window size - # of packets to be able to send unacket. Presently can be 1 or 2  */
  zb_uint8_t                       tx_win[ZB_MACSPLIT_MAX_PACKET_NUMBER]; /*!< packet ids in tx win */
  zb_bool_t                        is_waiting_for_ack;      /*!< if an ack to sent packet was received  */
  zb_bool_t                        is_tx_in_progress;       /*!< if transmission in progress  */
  zb_uint8_t                       curr_pkt_number;         /*!< number of next outcoming packet */
  zb_uint8_t                       next_rx_pkt_number;      /*!< number of the packet to be received */
  zb_bool_t                        boot_in_progress;
  zb_uint32_t                      boot_random;             /*!< Boot indication random value - helps to detect dups */

  zb_macsplit_transport_type_e     transport_type;          /*!< transport type: serial or spi, should be set before init */
  zb_uint8_t                       retransmit_count;        /*!< number of packet retransmit */
  zb_macsplit_transport_specific_t specific_ctx;            /*!< specific context for Host or Device */

  zb_ota_protocol_context_t ota_context;
#if defined ZB_MACSPLIT_TRACE_DUMP_TO_FILE
  zb_osif_file_t                   *trace_file;             /*!< dump file for trace packets */
#endif
#if defined ZB_MACSPLIT_HOST
#ifdef ZB_MACSPLIT_HANDLE_DATA_BY_APP
  zb_uint8_t                       handle_data_by_app;
  zb_uint8_t                       handle_data_by_app_after_last_ack;
  zb_macsplit_handle_data_by_app   handle_data_by_app_cb;
#endif
#endif

  zb_bufid_t                         operation_buf;       /*!< Buffer for future use (some internal data etc) */
#ifdef ZB_MAC_CONFIGURABLE_TX_POWER
  zb_int8_t                        mac_tx_power[ZB_CHANNEL_PAGES_NUM][ZB_PROD_CFG_MAC_TX_POWER_CHANNEL_N];
  zb_bool_t                        tx_power_exist;
#endif
#if defined ZB_MACSPLIT_HOST
  zb_macsplit_device_trace_cb_t    device_trace_cb;
  zb_bufid_t                       device_boot_ind_param;
  zb_bufid_t                       skip_startup_signal_param;
#endif

#if defined ZB_MACSPLIT_HOST && defined ZB_LIMIT_VISIBILITY
  /* MAC_CTX().visible_addresses host cache table.
   *
   * It allows to:
   * 1) Check IEEE address visibility directly in the host
   * 2) Synchronize MAC_CTX().visible_short_addresses table for visible devices only
   */
  zb_uint8_t n_visible_addr;
  zb_ieee_addr_t visible_addresses[ZB_N_VISIBLE_ADDRESSES];
#endif
#ifdef ZB_MACSPLIT_SPINEL
  zb_uint8_t hdlc_buf[ZB_TRANSPORT_PKT_MAX_SIZE];
  zb_hdlc_ctx_t hdlc;
#endif
  zb_bufid_t last_data_req_buf;

#ifdef ZB_MACSPLIT_ENABLE_TX_THROTTLE
  zb_bool_t tx_throttle; /*!< true, if tx is already scheduled (for non-NACK) */
#endif /* ZB_MACSPLIT_ENABLE_TX_THROTTLE */
} zb_macsplit_transport_context_t;

typedef ZB_PACKED_PRE struct macsplit_device_ver_s
{
  zb_uint32_t    random_value;     /*!< Is used to detect boot indication dups. */
  zb_ieee_addr_t extended_address; /*!< The 64-bit (IEEE) address assigned to the device. */
  zb_uint32_t    val;
} ZB_PACKED_STRUCT macsplit_device_ver_t;

extern zb_macsplit_transport_context_t g_macsplit;

#define MACSPLIT_CTX() g_macsplit

#ifndef ZB_CONFIGURABLE_MEM
#define MACSPLIT_TX_Q() (&(MACSPLIT_CTX().tx_queue))
#else
#define MACSPLIT_TX_Q() (MACSPLIT_CTX().tx_queue)
#endif

#define MACSPLIT_OTA_CTX() MACSPLIT_CTX().ota_context

#define GET_MACSPLIT_IFACE_ID() (MACSPLIT_CTX().iface_id)

#ifdef ZB_PLATFORM_LINUX
#include "zb_macsplit_transport_linux.h"
#elif defined ZB_WINDOWS
#include "zb_macsplit_transport_windows.h"
#elif defined ZB_PLATFORM_SOC
#include "zb_macsplit_transport_soc_host.h"  //reserve for MAC-Split host running on SOC
#endif

#ifdef ZB_MACSPLIT_HANDLE_DATA_BY_APP
void zb_mac_transport_handle_data_by_app_set(zb_uint8_t flag, zb_uint8_t after_ack);
void zb_mac_transport_handle_data_by_app_set_cb(zb_macsplit_handle_data_by_app cb);
int zb_mac_transport_get_fd();
#endif

zb_ret_t zb_macsplit_transport_transfer_packet(zb_macsplit_packet_t* pkt, zb_uint8_t len, zb_bool_t retransmit);
zb_uint8_t zb_macsplit_transport_transfer_packet_preparation(zb_macsplit_packet_t* pkt);

void zb_macsplit_transport_got_boot_ind(void);

zb_uint8_t zb_macsplit_pkt_payload_size(zb_macsplit_packet_t *pkt);
zb_bool_t zb_macsplit_transport_is_body_present(zb_macsplit_packet_t* pkt);
void zb_macsplit_transport_dump_transport_hdr(zb_macsplit_packet_t* pkt, zb_uint8_t tx);

void mac_sync_address_update(zb_ieee_addr_t ieee_address, zb_uint16_t short_address, zb_bool_t lock);

zb_ret_t zb_macsplit_try_merge_pkts(const zb_uint8_t *npkt, zb_uint16_t nsize,
                                    zb_uint8_t *opkt, zb_uint16_t osize,
                                    zb_uint8_t **rpkt, zb_uint16_t *rsize);

void macsplit_ack_by_conf_param(zb_bufid_t param);
zb_uint8_t macsplit_pkt_number_add(zb_uint8_t num, zb_int8_t inc);

zb_uint8_t macsplit_pkt_number_add(zb_uint8_t num, zb_int8_t inc);

#endif /* ZB_MACSPLIT_TRANSPORT_H */
