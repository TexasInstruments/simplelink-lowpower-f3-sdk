/* GreenBOSS Zigbee Green Power Device software protocol stack
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
 * www.dsr-iot.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * GreenBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: Internals if bi-directional commissioning
*/

#ifndef ZB_BIDIR_COMM_H
#define ZB_BIDIR_COMM_H 1

/*! \addtogroup comm_int */
/*! @{ */


#define ZGPD_COMM_NO_EVENT 0U
#define ZGPD_COMM_COMMISSIONING_EVENT 1U
#define ZGPD_COMM_TX_DONE_EVENT 2U
#define ZGPD_COMM_TIMER_EXPIRED_EVENT 3U
#define ZGPD_COMM_RX_DONE_EVENT 4U

/* Channel Request Frame, 10 bytes w/o MAC FCS */
#define ZGPD_FRAME_CHANNEL_REQ_LEN 10U

#define ZGPD_CHANNEL_REQ_FRAME_SEQ_NUMBER_OFFSET 2U
#define ZGPD_CHANNEL_REQ_FRAME_AUTOCOMM_OFFSET   7U
#define ZGPD_CHANNEL_REQ_FRAME_PAYLOAD_OFFSET    9U

#define ZGPD_FRAME_CHANNEL_CONF_LEN 10U

#define ZGPD_FRAME_MAC_FCF 0x0801
#define ZGPD_FRAME_MAC_PAN_ID 0xFFFF
#define ZGPD_FRAME_MAC_ADDR 0xFFFF

typedef zb_uint8_t zgpd_comm_event_t;

typedef struct zgpd_comm_channel_req_ctx_s
{
  zb_uint8_t next_channel;
  zb_uint8_t next_next_channel;
  zb_uint8_t seq_number;
} zgpd_comm_channel_req_ctx_t;

typedef struct zgpd_comm_tx_frame_ctx_s
{
  zb_uint8_t channels_per_attempt;
  zb_uint8_t channel_idx;
  zb_uint8_t ingroup_idx;
  zb_uint8_t op_channel;
  zb_uint8_t rx_on;
} zgpd_comm_tx_frame_ctx_t;

#define ZGPD_COMM_CMD_BUFFER_SIZE 64

typedef struct zgpd_commissioning_ctx_s {
  zb_uint8_t event;
  const gboss_device_config_t *dev_cfg;
  const gboss_tx_cfg_t *tx_cfg;
  zgpd_comm_channel_req_ctx_t channel_req;
  zgpd_comm_tx_frame_ctx_t tx;
  zb_uint8_t op_channel;
  zb_uint8_t cmd_buf[ZGPD_COMM_CMD_BUFFER_SIZE];
} zgpd_commissioning_ctx_t;

extern zgpd_commissioning_ctx_t gs_comm_ctx;

#define ZGPD_COMM_CTX (&(gs_comm_ctx))
#define ZGPD_SET_COMMISSIONING_EVENT(ev) (ZGPD_COMM_CTX->event = (ev))
#define ZGPD_GET_COMMISSIONING_EVENT() ZGPD_COMM_CTX->event

#define ZGPD_TX_CTX (&(gs_comm_ctx.tx))

/**
@}
*/

#endif /* ZB_BIDIR_COMM_H */
