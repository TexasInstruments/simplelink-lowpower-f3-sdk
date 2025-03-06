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

#ifndef ZB_TI_F3_MAC_H
#define ZB_TI_F3_MAC_H 1

#include <stdint.h>
#include <stdbool.h>

/* Common forward declarations */
extern zb_ret_t mac_ti23xx_set_channel(uint8_t page, uint8_t channel_num);
extern void mac_ti23xx_24_set_tx_power(uint8_t tx_power_dbm);
extern void mac_ti23xx_trans_rec_pkt(uint8_t *buf);
extern void mac_ti23xx_trans_set_rx_on_off(uint32_t enable);
extern void mac_ti23xx_set_ieee_addr(const zb_ieee_addr_t addr);

typedef enum
{
   STATUS_TYPE_RX = 0,
   STATUS_TYPE_TX
} MacDataStatus;

typedef struct zb_transceiver_ctx_s
{
  uint8_t tx_int; /* Transmit Interrupt status */
  uint8_t rx_int; /* Number of received packets, non-zero correlates to positive interrupt */
  uint8_t csma_cnt; /* Current count for CSMA status */
  int8_t cca_rssi_threshold; /* RSSI limit (dBm) for energy based CCA */
  uint8_t channel_number; /* Current MAC Channel */
  int8_t cur_tx_power; /* Current Tx power */
  uint8_t txRetries; /* Number of packet retries for Green power frames or number of CSMA backoffs */
  uint8_t *payload;
  uint8_t payloadLen;
#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
  unsigned int tx_status;
  uint8_t interrupt_flag;
  uint8_t recv_buf_full;
  uint8_t ed_started;
  uint8_t current_wait_type;
  unsigned int failed_tx;
  /* CCA Retry variables */
  uint8_t be;
  uint8_t nb;
  uint8_t initCCA;
  /* ES: at platform mac level we have two sources of control RX on/off state:
     1st: command to ON/OFF from higher MAC level (mac_ti23xx_trans_set_rx_on_off())
     2nd: command from platform internally (mostly for CSMACA)
     that's why we have to track both states, rx_on is for real RX state,
     rx_cmd_on is for last command to RX on/off from stack */
  bool rx_on; /* actual state of a receiver */
  bool rx_cmd_on; /* command from mac level to turn RX ON/OFF */
  bool rx_paused;
  bool initialized;
  uint8_t na_start; /* starting id of a continuous region that is currently not available for write by the RF module*/
  uint8_t na_len; /* length of the region */
  uint8_t ti_rx_i;
  uint8_t pending_bit;
  uint8_t seq_num;
  uint8_t ackReq;
#endif // defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
}
zb_transceiver_ctx_t;

extern zb_transceiver_ctx_t transCtx;

/**
 * 6.5.3.2 Symbol rate
 * The 2450 MHz PHY symbol rate shall be 62.
 * 5 ksymbol/s 40 ppm.
 */
#ifndef ZB_SYMBOL_DURATION_USEC
#define ZB_SYMBOL_DURATION_USEC 16
#endif

/**
   I/o with wait. Do it when nothing to do more.

   Set RX_DATA_STATUS when received MAC data from NS.
   Set TX_DATA_STATUS when received TX status from NS.
 */
#define ZB_GO_IDLE()

/**
@addtogroup macll
@{
*/

/******************************* Common APIs ****************************/

/**
  Switch RX ON or OFF
*/
#define ZB_TRANSCEIVER_SET_RX_ON_OFF(_rx_on)           \
    mac_ti23xx_trans_set_rx_on_off(_rx_on)

/*************************************************************************/

#ifdef ZB_ZGPD_ROLE

extern void zb_mac_hw_init();
extern void mac_ti23xx_zp_send_frame(uint8_t *buf, uint8_t repetitions);
extern uint8_t mac_ti23xx_get_radio_data_status(uint8_t type);
extern void mac_ti23xx_clear_radio_data_status(uint8_t type);
extern void mac_ti23xx_clear_rx_queue(void);

#define TRANS_CTX() transCtx

#define ZB_TRANSCEIVER_START_CHANNEL_NUMBER 11U
#define ZB_TRANSCEIVER_MAX_CHANNEL_NUMBER   26U

/**
   Sends the ZGPD frame through the currently set channel.
   The defined function used on this macro should be aware
   that the buffer's first byte is the frame length
 */
#define ZB_TRANS_SEND_FRAME(buf, repetitions)          \
  mac_ti23xx_zp_send_frame(buf, repetitions);

/**
   Sets the working channel
 */
#define ZB_TRANS_ZGPD_SET_CHANNEL(channel)           \
  mac_ti23xx_set_channel(0, channel);

/**
   Sets the TX power on MAC hardware.
 */
#define ZB_TRANS_ZGPD_SET_TX_POWER(power)           \
  mac_ti23xx_24_set_tx_power(power);

/**
   Clear RX fifo at HW.

   To be used if we got RX when it must not happen
 */
#define ZB_TRANS_CLR_RX_FIFO() mac_ti23xx_clear_rx_queue()

/**
   Set NS MAC address from passed ieee_addr parameter
 */
#define ZB_TRANSCEIVER_UPDATE_IEEE_ADDR(ieee_addr)           \
  mac_ti23xx_set_ieee_addr(ieee_addr)


/* RX status set when full data packet is received */
/**
   Clear "rx done" flag. External app should not control this flags internal state
 */
#define ZB_CLEAR_RX_DATA_STATUS()

/**
   Get "rx done" flag
 */
#define ZB_GET_RX_DATA_STATUS() mac_ti23xx_get_radio_data_status(STATUS_TYPE_RX)

/* TX status set when full data packet is sent */
/**
   Clear "tx done" flag
 */
#define ZB_CLEAR_TX_DATA_STATUS() mac_ti23xx_clear_radio_data_status(STATUS_TYPE_TX)

/**
   Get "tx done" flag
 */
#define ZB_GET_TX_DATA_STATUS() mac_ti23xx_get_radio_data_status(STATUS_TYPE_TX)

/**
   Synchronous recv.
   Imitation of read from FIFO in case of a real transceiver (in real transceiver
   we got rx int, so packet is received already and we must read it from
   FIFO).
   In case of NS NG packet is read already, so this is just data copy.
*/
#define ZB_TRANS_RECV_PACKET(buf)           \
 mac_ti23xx_trans_rec_pkt(buf)

#else // defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
#include <ti/zigbee/osif/include/zb_hal_utils.h>

extern zb_int8_t mac_ti23xx_24_get_tx_power(void);
extern void mac_ti23xx_radio_init(uint8_t enable);
extern void mac_ti23xx_send_packet(uint8_t mhr_len, zb_uint8_t buf, uint8_t wait_type);
extern void mac_ti23xx_resend_packet(zb_uint8_t wait_type, zb_time_t tx_delay);
extern void mac_ti23xx_abort_tx(void);
extern void mac_ti23xx_set_promiscuous_mode(zb_uint8_t promiscuous_mode);
extern zb_ret_t mac_ti23xx_perform_cca(zb_int8_t *rssi);
extern zb_time_t mac_ti23xx_tx_time_get(void);
extern zb_uint8_t mac_ti23xx_src_match_add_short_addr(zb_uint8_t index, zb_uint16_t short_addr);
extern zb_uint8_t mac_ti23xx_src_match_delete_short_addr(zb_uint8_t index);
extern void mac_ti23xx_src_match_tbl_drop(void);
extern void mac_ti23xx_src_match_short_set_pending_bit(zb_uint16_t short_addr, zb_bool_t pending);
extern zb_bool_t mac_ti23xx_buffer_available(uint8_t ref_new);
extern void mac_ti23xx_enable_rx(void);
extern void mac_ti23xx_24_get_tx_power_ptr(zb_int8_t *tx_power_dbm);
extern void update_rx_panconfig(zb_uint16_t pan_id);
extern zb_int8_t mac_ti23xx_get_sync_rssi(void);
extern void mac_ti23xx_set_cca_rssi_threshold(zb_int8_t new_rssi_threshold);
extern void mac_ti23xx_get_cca_rssi_threshold_ptr(zb_int8_t* rssi_threshold);

#include <ti/drivers/rcl/RCL.h>
#include <ti/drivers/rcl/LRF.h>
#include <ti/drivers/rcl/RCL_Scheduler.h>
#include <ti/drivers/rcl/commands/ieee.h>


/**
   Definitions for extra information size in RX fifo packet. Packet format:
   1 byte packet length + N bytes of RX fifo data + M bytes of extra information
   (LQI, RSSI, at some platforms Frame timer, Superframe counter etc)
*/
#define ZB_MAC_PACKET_LENGTH_SIZE 1U
#define ZB_MAC_EXTRA_DATA_SIZE    2U /* rssi & lqi - see ns_pkt_trailer_t */

/**
   Tail size for mac packet
*/
#define ZB_TAIL_SIZE_FOR_RECEIVED_MAC_FRAME (ZB_MAC_EXTRA_DATA_SIZE)


/******************************* 2.4 GHz part ****************************/

/*  IEEE std. 802.15.4-2011
 *
 * 10.3.4 Receiver sensitivity
 * Under the conditions specified in 8.1.7, a compliant device shall be capable of achieving a
 * receiver sensitivity of -85 dBm or better.
 *
 * 8.2.7 Clear channel assessment (CCA)
 * The CCA parameters are subject to the following criteria:
 * a) The ED threshold shall correspond to a received signal power of at most 10 dB greater than the
 * specified receiver sensitivity for that PHY.
 */
#define ZB_MAC_SHORT_MATCH_NOT_FOUND 0xFFU

/* However:
 * Refer to ZigBee doc 14-0332-01 "ZigBee IEEE 802.15.4 Test Spec"
 * Test case TP/154/PHY24/RECEIVER-07
 * ... CCA Mode 1 shall report a busy medium upon detecting any energy above the ED threshold specified in the table below:
 * 2.4 GHz O-QPSK = -68dBm
 *
 * ...
 * Lets follow the requirement of test specification document
 */
#define ZB_MAC_CCA_THRESHOLD -68

/* src index (1 byte) + rssi (1 byte) + timestamp (4 bytes) */
#define ZB_MAC_BUFFER_EXTRA_SPACE (1U + 1U + 4U)

#define ZB_MAC_SHORT_MATCH_LIST_SIZE ZB_NEIGHBOR_TABLE_SIZE

#if defined ZB_CONFIG_DEFAULT_KERNEL_DEFINITION

/**
 Kernel's default buffers. Define weak global variables.
*/
#define ZB_KERNEL_POST
#define ZB_KERNEL_PRE  __attribute__ ((weak))

#if ZB_CONFIG_OVERALL_NETWORK_SIZE > RCL_CMD_IEEE_SOURCE_MATCH_TABLE_SHORT_MAX_LEN
#define ZB_CONFIG_MAC_SHORT_MATCH_LIST_SIZE ((offsetof(RCL_CmdIeee_SourceMatchingTableShort, shortEntry) / sizeof(uint32_t)) + RCL_CMD_IEEE_SOURCE_MATCH_TABLE_SHORT_MAX_LEN)
#else
#define ZB_CONFIG_MAC_SHORT_MATCH_LIST_SIZE ((offsetof(RCL_CmdIeee_SourceMatchingTableShort, shortEntry) / sizeof(uint32_t)) + ZB_CONFIG_OVERALL_NETWORK_SIZE)
#endif // ZB_CONFIG_OVERALL_NETWORK_SIZE > RCL_CMD_IEEE_SOURCE_MATCH_TABLE_SHORT_MAX_LEN

ZB_KERNEL_PRE uint32_t gc_srcMatchTableBuffer[ZB_CONFIG_MAC_SHORT_MATCH_LIST_SIZE] ZB_KERNEL_POST = { 0 };
ZB_KERNEL_PRE zb_uint8_t gc_zb_mac_short_match_list_size ZB_KERNEL_POST = ZB_CONFIG_MAC_SHORT_MATCH_LIST_SIZE;
ZB_KERNEL_PRE uint8_t gc_neighborToSrcMatchTable[ZB_CONFIG_MAC_SHORT_MATCH_LIST_SIZE] ZB_KERNEL_POST;

#endif  /* ZB_CONFIG_DEFAULT_KERNEL_DEFINITION */

/*************************************************************************/

/******************************** Common part ****************************/
#define MAC_TIMER_QUANT_US 4
#define ZB_TRANS_GET_TX_TIMESTAMP() osif_transceiver_time_get()

#define ZB_MAC_SET_TRANS_INT() (TRANS_CTX().interrupt_flag = 1)
#define ZB_MAC_CLEAR_TRANS_INT() (TRANS_CTX().interrupt_flag = 0)

#ifndef ZB_BOOTLOADER
#define ZB_MAC_GET_TRANS_INT_FLAG() (TRANS_CTX().interrupt_flag)
#else
#define ZB_MAC_GET_TRANS_INT_FLAG() ZB_TRUE
#endif /* !ZB_BOOTLOADER */

#define ZB_MAC_GET_RX_INT_STATUS_BIT() TRANS_CTX().rx_int
#define ZB_MAC_GET_TX_INT_STATUS_BIT() TRANS_CTX().tx_int

#define ZB_MAC_SET_RX_INT_STATUS_BIT() TRANS_CTX().rx_int = 1
#define ZB_MAC_SET_TX_INT_STATUS_BIT() TRANS_CTX().tx_int = 1

#define ZB_MAC_CLEAR_RX_INT_STATUS_BIT() TRANS_CTX().rx_int = 0
#define ZB_MAC_CLEAR_TX_INT_STATUS_BIT() (TRANS_CTX().tx_int = TRANS_CTX().tx_status = 0)

#define ZB_TRANS_CHECK_CHANNEL_BUSY_ERROR() (TRANS_CTX().tx_status == ZB_TRANS_CHANNEL_BUSY_ERROR)
#define ZB_TRANS_CHECK_TX_RETRY_COUNT_EXCEEDED_ERROR() 0U
#define ZB_TRANS_CHECK_TX_LBT_TO_ERROR() (TRANS_CTX().tx_status == ZB_TRANS_TX_LBT_TO)
#define ZB_TRANS_CHECK_NO_ACK() (TRANS_CTX().tx_status == ZB_TRANS_NO_ACK)

#define ZB_TRANSCEIVER_SET_COORD_SHORT_ADDR(addr)
#define ZB_TRANSCEIVER_SET_COORD_EXT_ADDR(addr)
#define ZB_TRANSCEIVER_SET_PAN_COORDINATOR(pan_coord)

#define ZB_TRANS_CUT_SPECIFIC_HEADER(buf) // TODO: Add definition -- No Agama Support

/* Have MAC layer check transmit status */
//#define ZB_IS_TRANSPORT_BUSY() ZB_FALSE

#define ZB_TRANSCEIVER_INIT_RADIO() mac_ti23xx_radio_init(true)

#define ZB_TRANSCEIVER_DEINIT_RADIO() mac_ti23xx_radio_init(false)

#define ZB_MAC_TX_ABORT() mac_ti23xx_abort_tx()

#define ZB_TRANSCEIVER_UPDATE_SHORT_ADDR() update_rx_panconfig(MAC_PIB().mac_pan_id)

/* really need this function: sometimes we set panid -1 keeping valid MAC_PIB().mac_pan_id */
#define ZB_TRANSCEIVER_SET_PAN_ID(pan_id) update_rx_panconfig(pan_id)

#define ZB_TRANSCEIVER_UPDATE_PAN_ID() ZB_TRANSCEIVER_SET_PAN_ID(MAC_PIB().mac_pan_id)

#define ZB_TRANSCEIVER_GET_ENERGY_LEVEL(x) mac_ti23xx_get_energy_level(x)

#define ZB_TRANSCEIVER_START_GET_RSSI(_scan_duration_bi) mac_ti23xx_start_ed_scan(_scan_duration_bi)

#define ZB_TRANSCEIVER_SET_CHANNEL(page, channel_number) mac_ti23xx_set_channel(page, ZB_PAGES_REMAP_LOGICAL_CHANNEL(page, channel_number))

#define ZB_TRANSCEIVER_GET_SYNC_RSSI() mac_ti23xx_get_sync_rssi()

#define ZB_TRANSCEIVER_UPDATE_LONGMAC() ZB_TRANSCEIVER_SET_PAN_ID(MAC_PIB().mac_pan_id)

#define ZB_TRANS_SEND_FRAME(header_length, buf, wait_type)                     \
{                                                                              \
  ZB_DUMP_OUTGOING_DATA(buf);                                                  \
  mac_ti23xx_send_packet(header_length, buf, wait_type);                       \
}

/*
 *  In general, a separate implementation for REPEAT_SEND_FRAME is not required -
 *  SEND_FRAME will do the same.
 *  REPEAT_SEND_FRAME has only been implemented for 2.4GHz because of GP support,
 *  we can use it without complicated macros when ZB_SUBGHZ_BAND_ENABLED is not defined.
 */
#define ZB_TRANS_REPEAT_SEND_FRAME(header_length, buf, wait_type)       \
{                                                                       \
  ZB_DUMP_OUTGOING_DATA(buf);                                           \
  mac_ti23xx_send_packet(header_length, buf, wait_type);                \
}

#define ZB_TRANSCEIVER_GET_RX_ON_OFF() mac_ti23xx_get_rx_on_off()

/* Perform single CCA attempt
 * Used in LBT and TP_154_PHY24_RECEIVER_07 test */
#define ZB_TRANSCEIVER_PERFORM_CCA(rssi) mac_ti23xx_perform_cca(rssi)

/* [VK]: sticky pending bit is not supported longer. */
#define ZB_MAC_TRANS_CLEAR_PENDING_BIT()
#define ZB_MAC_TRANS_SET_PENDING_BIT()
#define ZB_MAC_TRANS_PENDING_BIT() 0

#define ZB_TRANSCEIVER_SET_TX_POWER(tx_power_dbm) mac_ti23xx_24_set_tx_power(tx_power_dbm)

#define ZB_TRANSCEIVER_GET_TX_POWER(tx_power_dbm) mac_ti23xx_24_get_tx_power_ptr(tx_power_dbm)

#define ZB_TRANS_RECV_PACKET(buf) (ZB_FALSE)

#define ZB_TRANSCEIVER_SET_PROMISCUOUS(promiscuous_mode) \
  mac_ti23xx_set_promiscuous_mode(promiscuous_mode)

/**
   Get CCA RSSI threshold value
   @param rssi - pointer to buffer
*/
#define ZB_MAC_GET_CCA_RSSI_THRESHOLD(rssi) mac_ti23xx_get_cca_rssi_threshold_ptr(rssi)

/**
   Set CCA RSSI threshold  value
   @param rssi - RSSI value
*/
#define ZB_MAC_SET_CCA_RSSI_THRESHOLD(rssi) mac_ti23xx_set_cca_rssi_threshold(rssi)

/*******************************************************/


/************* Unused/Not Implemented part *************/

#define ZB_MAC_TRANSCEIVER_GO_IDLE()
#define ZB_MAC_TRANSCEIVER_RESTORE()

#define ZB_RADIO_INT_DISABLE() ZB_OSIF_GLOBAL_LOCK()
#define ZB_RADIO_INT_ENABLE()  ZB_OSIF_GLOBAL_UNLOCK()

/* No need rx flush */
#define ZB_MAC_RX_FLUSH()
#define ZB_MAC_READ_INT_STATUS_REG() /* already done by int handler */

/* ACK is always filtered. Use explicit command to wait for ACK. */
#define ZB_TRANSCEIVER_ACK_ACCEPT(enable_ack)

/* TODO: implement for PHY testing */
#define ZB_TRANS_TX_CARRIER_DATA(channel, timeout_bi) ((void)0) /* Start continuous transmission.
                                                                 * Used in TP_154_PHY24_TRANSMIT_02 test.
                                                                 */

#define ZB_TRANSCEIVER_ENABLE_AUTO_ACK()

/*****************************************************/

#if defined ZB_MAC_PENDING_BIT_SOURCE_MATCHING

#if defined ZB_MAC_HARDWARE_PB_MATCHING

#define ZB_TRANSCEIVER_SRC_MATCH_ADD_SHORT_ADDR(index, short_addr) \
  mac_ti23xx_src_match_add_short_addr(index, short_addr)

#define ZB_TRANSCEIVER_SRC_MATCH_DELETE_SHORT_ADDR(index, short_addr) \
  mac_ti23xx_src_match_delete_short_addr(index)

#define ZB_TRANSCEIVER_SRC_MATCH_TBL_DROP() \
  mac_ti23xx_src_match_tbl_drop()

#define ZB_TRANSCEIVER_SRC_MATCH_SHORT_SET_PENDING_BIT(short_addr, pending) \
  mac_ti23xx_src_match_short_set_pending_bit(short_addr, pending)

/* N/A */
#define ZB_TRANSCEIVER_SRC_MATCH_ADD_IEEE_ADDR(index, ieee_addr) (0U)
#define ZB_TRANSCEIVER_SRC_MATCH_DELETE_IEEE_ADDR(index, ieee_addr) (0U)

#else /* ZB_MAC_SOFTWARE_PB_MATCHING only */
#define ZB_TRANSCEIVER_SRC_MATCH_ADD_SHORT_ADDR(index, short_addr) (0U)
#define ZB_TRANSCEIVER_SRC_MATCH_DELETE_SHORT_ADDR(index, short_addr) (0U)
#define ZB_TRANSCEIVER_SRC_MATCH_TBL_DROP()
#define ZB_TRANSCEIVER_SRC_MATCH_SHORT_SET_PENDING_BIT(short_addr, pending)
#define ZB_TRANSCEIVER_SRC_MATCH_ADD_IEEE_ADDR(index, ieee_addr) (0U)
#define ZB_TRANSCEIVER_SRC_MATCH_DELETE_IEEE_ADDR(index, ieee_addr) (0U)

#endif /* ZB_MAC_HARDWARE_PB_MATCHING */

#endif /* ZB_MAC_PENDING_BIT_SOURCE_MATCHING */

/**
  Called by mac.c after a buffer has been processed
 */
#define MAC_Q_BUFFER_AVAILABLE(ref_new) mac_ti23xx_buffer_available(ref_new)

#define MAC_Q_ENA_RX() mac_ti23xx_enable_rx()

#endif // ZB_ZGPD_ROLE

/**
@}
*/

#endif /* ZB_TI_F3_MAC_H */
