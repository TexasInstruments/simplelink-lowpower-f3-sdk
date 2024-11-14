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
/*  PURPOSE: NCP Low level protocol definitions for the device side
*/
#ifndef NCP_LL_DEV_H
#define NCP_LL_DEV_H 1

#include "zb_common.h"

/** @cond DOXYGEN_INTERNAL_DOC */

/* LL protocol API for NCP (== inside ZBOSS) */

typedef void (*ncp_ll_packet_received_cb_t)(void  *data, zb_uint16_t len);
typedef void (*ncp_ll_tx_ready_cb_t)(void);
typedef void (*ncp_ll_next_event_notify_cb_t)(zb_uint32_t timeout);
typedef void (*ncp_ll_error_handler_cb_t)(zb_int32_t error);

void ncp_ll_proto_init(ncp_ll_packet_received_cb_t packet_received_cb, ncp_ll_tx_ready_cb_t tx_ready_cb, ncp_ll_error_handler_cb_t error_handler_cb);
zb_int_t ncp_ll_send_packet(void *data, zb_uint32_t len);
void ncp_ll_quant(zb_uint8_t unused);

#ifndef ZB_ALIEN_SCHEDULER_COLLABORATION_DISABLED
/** Registers the callback that is called when the NCP LL requires the next quantum
 *  of time.
 *
 * @param cb - the callback. If NULL is passed, previously registered callback (if any) will be unregistered.
 *
 * @return the previous callback or NULL if it was not set.
 *
 * @note The callback parameter is the number of milliseconds after which the ZBOSS scheduler should be called.
 * 0 means call as soon as possible,
 * ZBNCP_LL_TIMEOUT_INFINITE means no next call is required.
 */
ncp_ll_next_event_notify_cb_t ncp_ll_next_event_set_cb(ncp_ll_next_event_notify_cb_t cb);
#endif /* !ZB_ALIEN_SCHEDULER_COLLABORATION_DISABLED */

/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

#endif /* NCP_LL_DEV_H */
