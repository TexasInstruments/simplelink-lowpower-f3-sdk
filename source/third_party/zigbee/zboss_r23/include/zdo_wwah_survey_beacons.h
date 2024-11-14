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
/* PURPOSE:
*/

#ifndef ZDO_WWAH_SURVEY_BEACONS_H
#define ZDO_WWAH_SURVEY_BEACONS_H 1

#include "zb_config.h"
#include "zb_common.h"
#include "zdo_wwah_stubs.h"

typedef ZB_PACKED_PRE struct zb_zdo_beacon_survey_configuration_t
{
  zb_nlme_beacon_survey_scan_request_t params;
}
ZB_PACKED_STRUCT zb_zdo_beacon_survey_configuration_t;

/* 2019-02-26: CR:MINOR TODO:Split WWAH-specific and non-WWAH-specific parts of Survey Beacons. */

/** @cond DOXYGEN_ZCL_SECTION */

/**
 *  @addtogroup ZB_ZCL_WWAH
 *  @{
 */

/**
 *  @addtogroup ZDO_WWAH_SURVEY_BEACONS
 *  @{
 */

/**
 *  @brief Start Survey Beacons procedure
 *
 *  @param param - reference to a buffer which contain
 *         @zb_zdo_survey_beacons_params_t
 *
 *  @return @RET_ERROR if failed;
 *          @RET_BUSY otherwise - in this case stack doesn't free
 *          the buffer with Survey Beacons command, because this buffer
 *          will be reused to send Survey Beacons Response command.
 */
zb_ret_t zdo_wwah_start_survey_beacons(zb_uint8_t param);

/**
 *  @brief Process a received beacons and put information from it
 *         into the 'Survey Beacons Response' buffer
 *
 *  @param beacon_noti_ind_ptr - pointer to the @zb_mac_beacon_notify_indication_t
 *  @param beacon_payload_ptr - pointer to the @zb_mac_beacon_payload_t
 */
void zdo_wwah_process_beacon_info(
  zb_mac_beacon_notify_indication_t *beacon_noti_ind_ptr,
  zb_mac_beacon_payload_t *beacon_payload_ptr);

/**
 *  @brief Send 'Survey Beacons Response' command
 *
 */
void zdo_send_survey_beacons_response(zb_uint8_t param);


void zb_tlv_put_potential_parents(zb_uint8_t param,
                                  zb_uint16_t current_parent,
                                  zb_uint8_t potent_parents_cnt,
                                  zb_zcl_wwah_beacon_survey_t *potent_parents_list);


void zb_zdo_beacon_survey_put_configuration_tlv (zb_uint8_t param,
                                                 zb_uint8_t channel_page_cnt,
                                                 zb_uint32_t *channel_page_list,
                                                 zb_uint8_t conf_mask);


void zb_zdo_beacon_survey_put_results_tlv (zb_uint8_t param,
                                           zb_uint8_t total_beacon_surv,
                                           zb_uint8_t num_cur_zbn,
                                           zb_uint8_t num_potent_parents_cur_zbn,
                                           zb_uint8_t num_other_zbn);


zb_ret_t zb_zdo_beacon_survey_process_configuration_tlv (zb_uint8_t *tlv_ptr,
                                                         zb_uint8_t tlv_data_len,
                                                         zb_uint8_t *channel_page_cnt,
                                                         zb_uint32_t *channel_page_list,
                                                         zb_uint8_t *conf_mask);

zb_ret_t zb_tlv_parse_beacon_survey_results_tlv(zb_uint8_t *tlv_ptr,
                                                zb_uint8_t tlv_data_len,
                                                zb_zdo_beacon_survey_results_t *results);

zb_ret_t zb_tlv_parse_beacon_survey_parents_tlv(zb_uint8_t *tlv_ptr,
                                                zb_uint8_t tlv_data_len,
                                                zb_zdo_beacon_survey_potential_parents_t *parents);

zb_ret_t zb_tlv_parse_panid_conflict_report_tlv(zb_uint8_t *tlv_ptr,
                                                zb_uint8_t tlv_data_len,
                                                zb_uint16_t *panid_conflict_count);





/** @} */ /* @addtogroup ZDO_WWAH_SURVEY_BEACONS */

/** @} */ /* @addtogroup ZB_ZCL_WWAH */

/** @endcond */ /* DOXYGEN_ZCL_SECTION */

#endif /* ZDO_WWAH_SURVEY_BEACONS_H */
