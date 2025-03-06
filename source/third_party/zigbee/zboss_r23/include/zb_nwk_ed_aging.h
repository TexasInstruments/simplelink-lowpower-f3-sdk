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
/*  PURPOSE: NWK end device aging management
*/
#ifndef ZB_NWK_ED_AGING_H
#define ZB_NWK_ED_AGING_H 1

/**
 * @name NWK request timeout status
 * @anchor requested_timeout_status
 */
/** @{ */
#define ED_AGING_SUCCESS         0U
#define ED_AGING_INCORRECT_VALUE 1U
/** @} */

/**
 * @brief Type for NWK request timeout status.
 *
 * Holds one of @ref requested_timeout_status. Kept only for backward compatibility as
 * @ref requested_timeout_status were declared previously as enum. Can be removed in future
 * releases.
 */
typedef zb_uint8_t nwk_requested_timeout_status_t;

/*
  Power negotiation feature to be enabled/disabled.
  It's included in 'parent_info' bitmask (3rd LSB => .... .X..).
*/
#define POWER_NEGOTIATION_SUPPORT 0x04U

/*
  nwk_parent_information is filled in response to ED timeout req.
  Non-zero nwk_parent_information means we work with >= r21 parent with ED aging ON.
 */
#define ZB_GET_PARENT_INFO() ZB_NIB().nwk_parent_information
#define ZB_SET_PARENT_INFO(pinf) ZB_NIB().nwk_parent_information = (pinf)

#define ZB_GET_KEEPALIVE_MODE() ZB_NIB().nwk_keepalive_modes
#define ZB_SET_KEEPALIVE_MODE(mode) ZB_NIB().nwk_keepalive_modes = (mode)

#define ZB_GET_ED_TIMEOUT() ZB_NIB().nwk_ed_timeout_default
#define ZB_GET_KEEPALIVE_TIMEOUT() ZB_NIB().nwk_ed_keepalive_timeout

#define NWK_ED_DEVICE_TIMEOUT_DEFAULT 8U /*This is an index into. It indicates the default timeout in minutes for any end device that does not negotiate a different timeout value.*/

#define NWK_ED_DEVICE_CONFIG_DEFAULT 0x00U /*At this time there are no enumerated bits in the configuration field. Devices adhering to this standard shall set the field to 0.*/

#define ZB_NWK_ED_TIMEOUT_RESP_FAILURE_TMO (7U * ZB_TIME_ONE_SECOND)

/**
   End device timeout request command
*/
typedef ZB_PACKED_PRE struct zb_nwk_ed_timeout_request_s
{
  zb_uint8_t request_timeout;         /**< Request timeout enumeration @see @ref nwk_requested_timeout */
  zb_uint8_t ed_config;               /**< End Device Configuration */
/*NOTE: This is a bitmask indicating the end device's requested
 * configuration. At this time there are no enumerated bits in the configuration
 * field. Devices adhering to this standard shall set the field to 0. To allow
 * for future compatibility this field is left in place. Devices that receive
 * the End Device Timeout Request message with an End Device Configuration field
 * set to anything other than 0 shall reject the message. The receiving device
 * shall send an End Device Timeout Response command with a status of 0x01 (INCORRECT_VALUE).*/

} ZB_PACKED_STRUCT zb_nwk_ed_timeout_request_t;

/**
   End device timeout response command
*/
typedef ZB_PACKED_PRE struct zb_nwk_ed_timeout_response_s
{
  zb_uint8_t status;                    /**< The success or failure result of
                                         * the  previously received End Device
                                         * Timeout Request command. */
  zb_uint8_t parent_info;               /**< Parent routers support information
                                         * to the child device. */
} ZB_PACKED_STRUCT
zb_nwk_ed_timeout_response_t;

/**
 * @name ED keepalive status
 * @anchor keepalive_status_internal
 *
 * Note: These values were members of `enum nwk_keepalive_status_internal_e` type but were converted
 * to a set of macros due to MISRA violations.
 */
/** @{ */
#define ED_KEEPALIVE_SUCCESS     0U
#define ED_KEEPALIVE_REQ_FAILED  1U
#define ED_KEEPALIVE_RESP_FAILED 2U
/** @} */

typedef struct zb_nwk_ed_timeout_response_param_s
{
  zb_uint8_t status;                    /**< General internal status of the
                                         * Keepalive procedure */
  zb_uint8_t resp_status;               /**< The success or failure result of
                                         * the  previously received End Device
                                         * Timeout Request command. */
  zb_uint8_t parent_info;               /**< Parent routers support information
                                         * to the child device. */

} zb_nwk_ed_timeout_response_param_t;

void zb_init_ed_aging(zb_neighbor_tbl_ent_t *nbt, zb_uint8_t timeout, zb_bool_t run_aging);

void zb_update_ed_aging(void);

void zb_stop_ed_aging(void);

void zb_nwk_ed_aging_timeout(zb_uint8_t param);

/* Convert timeout value from index to seconds */
zb_uint32_t zb_convert_timeout_value(zb_uint8_t timeout);

#ifdef ZB_MAC_PENDING_BIT_SOURCE_MATCHING
void zb_nwk_src_match_add(zb_uint8_t param, zb_uint16_t addr_ref);
void zb_nwk_src_match_add_ent(zb_uint8_t param, zb_address_ieee_ref_t addr_ref, zb_callback_t cb);
void zb_nwk_src_match_delete(zb_uint8_t param, zb_uint16_t ieee_ref);
void zb_nwk_src_match_drop(zb_uint8_t param);
void zb_nwk_src_match_restore(zb_uint8_t param);
void zb_nwk_src_match_mac_child_update(zb_uint8_t param, zb_uint16_t addr_ref);
#endif  /* ZB_MAC_PENDING_BIT_SOURCE_MATCHING */

void zb_nwk_ed_timeout_resp_recv_fail_trig(zb_uint8_t unused);

#endif /* ZB_NWK_ED_AGING_H */
