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
/* PURPOSE: Stub APS API
*/

#ifndef ZB_APS_INTERPAN_H
#define ZB_APS_INTERPAN_H 1

#include "zboss_api_aps_interpan.h"

/** @addtogroup aps_api
  * @{
  */


typedef ZB_PACKED_PRE struct zb_aps_intrp_data_header_s
{
  zb_uint16_t nwk_fcf;          /*!< */
  zb_uint8_t aps_fcf;           /*!< */
  zb_uint16_t group_addr;       /*!< */
  zb_uint16_t cluster_id;       /*!< */
  zb_uint16_t profile_id;       /*!< */
} ZB_PACKED_STRUCT zb_aps_intrp_data_header_t;

/** @brief Additional NWK frame type for inter-PAN exchange.
  * @see SE spec, clause B.4.
  */
#define ZB_NWK_FRAME_TYPE_INTER_PAN 0x0003U

/** @brief NWK FCF value for inter-PAN exchange.
  * @see SE spec, clause B.4.
  */
#define ZB_NWK_INTERPAN_FCF_VALUE ((ZB_PROTOCOL_VERSION << 2U) | ZB_NWK_FRAME_TYPE_INTER_PAN)

/** @brief Additional APS frame type for inter-PAN exchange.
  * @see SE spec, clause B.4.
  */
#define ZB_APS_FRAME_INTER_PAN  0x0003U

/** @brief Evaluate Stub-APS header size.
  * @param is_group_addressing "group addressing is being used" flag.
  * @return Stub-APS header size.
  */
#define ZB_INTRP_HEADER_SIZE(is_group_addressing)                       \
  (   sizeof(zb_uint16_t)       /* NWK FCF */                           \
    + sizeof(zb_uint8_t)        /* APS FCF */                           \
                                /* Group address if present */          \
    + ((is_group_addressing) ? sizeof(zb_uint16_t) : 0)                 \
    + 2 * sizeof(zb_uint16_t)   /* Cluster and profile identifiers */ )

/** @brief Parsed Stub-APS header representation.
  * @attention Structure is aligned, so shan't be used in the buffer.
  */
typedef struct zb_intrp_hdr_s
{
  zb_uint16_t nwk_fcf;
  zb_uint16_t group_addr;
  zb_uint16_t cluster_id;
  zb_uint16_t profile_id;
  zb_uint8_t aps_fcf;
} zb_intrp_hdr_t;

#if defined ZB_ENABLE_INTER_PAN_EXCHANGE
/** @brief Parses Stub-APS header. */
void zb_parse_intrp_hdr(zb_intrp_hdr_t* header, zb_bufid_t  buffer);
#endif /* ZB_ENABLE_INTER_PAN_EXCHANGE */

#if defined ZB_ENABLE_INTER_PAN_EXCHANGE
/** @brief Fill INTRP-DATA.indication parameters.
 *
  * @param param - packet data buffer reference (index).
  * @param mac_hdr - parsed MAC header
  * @param lqi - LQI
  * @param rssi - RSSI
  *
  * @note Doesn't cut Stub-APS header.
  */
void zb_intrp_data_frame_indication(zb_uint8_t param, zb_mac_mhr_t *mac_hdr, zb_uint8_t lqi, zb_int8_t rssi);
#endif /* ZB_ENABLE_INTER_PAN_EXCHANGE */

/** @brief INTRP-DATA.confirm representation structure.
  * @note Status field is being passed in the buffer header.
  * @see SE spec, subclause B.3.2.
  */
typedef struct zb_intrp_data_confirm_s
{
  /** An integer handle associated with the transmitted frame. */
  zb_uint8_t asdu_handle;
} zb_intrp_data_confirm_t;

#if defined ZB_ENABLE_INTER_PAN_EXCHANGE
/** @brief Fill INTRP-DATA.confirm parameters.
  * @param param - packet data buffer reference (index).
  * @note Buffer's parameter contains @ref zb_intrp_data_confirm_t instance, and header contains an
  * INTRP-DATA.confirm.Status value. Buffer truncated to contain INTRRP-DATA payload.
  */
void zb_intrp_data_frame_confirm(zb_uint8_t param);

#endif /* ZB_ENABLE_INTER_PAN_EXCHANGE */
/** @} */

#ifdef ZB_ENABLE_INTER_PAN_NON_DEFAULT_CHANNEL
#define ZB_APS_INTRP_MCHAN_FUNC_SELECTOR() interpan_mchan_selector

/* Structure to store pointers to interpan multichannel functions */
typedef struct zb_aps_inter_pan_mchan_func_selector_s {
  zb_ret_t (* put_aps_packet_to_queue)(zb_uint8_t param);
  zb_ret_t (* put_nwk_packet_to_queue)(zb_uint8_t param);
  zb_ret_t (* intrp_mchan_data_req_confirmed)(zb_bufid_t buffer);
  zb_ret_t (* intrp_data_req_with_chan_change)(zb_bufid_t buffer, zb_channel_page_t channel_page_mask, zb_uint32_t chan_wait_ms, zb_callback_t cb);
  void (* af_interpan_set_data_indication)(zb_af_inter_pan_handler_t cb);
} zb_aps_inter_pan_mchan_func_selector_t;

extern zb_aps_inter_pan_mchan_func_selector_t interpan_mchan_selector;
#endif /* defined ZB_ENABLE_INTER_PAN_NON_DEFAULT_CHANNEL */

#endif /* ZB_APS_INTERPAN_H */
