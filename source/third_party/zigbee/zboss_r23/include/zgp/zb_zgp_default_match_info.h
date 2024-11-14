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
/* PURPOSE: Provides example of matching table declaration for ZGPS sink
*/

#ifndef ZB_ZGP_DEFAULT_MATCH_INFO_H
#define ZB_ZGP_DEFAULT_MATCH_INFO_H 1

#include "zb_common.h"

#ifdef ZB_ENABLE_ZGP_SINK
#include "zboss_api_zgp.h"

#ifdef ZB_ZGP_SINK_SUPPORT_LEGACY_MATCH_INFO

/** 
 * @ingroup zgp_sink
 * @file zb_zgp_default_match_info.h
 * Provides example of matching table declaration for ZGPS sink
 * @include zb_zgp_default_match_info.h
 */

/**
 * Default matching table.
 */
zgps_dev_match_rec_t g_zgps_match_tbl[] =
{
#ifdef ZGPS_MATCH_TBL_TEMPERATURE_SENSOR_ENTRY
  {
    ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
    ZB_ZCL_CLUSTER_CLIENT_ROLE,
    {ZB_GPDF_CMD_ATTR_REPORT, ZB_GPDF_CMD_MULTI_CLUSTER_ATTR_REPORT, 0},
    ZB_ZGP_TEMPERATURE_SENSOR_DEV_ID,
  },
#endif

};


/**
 * Mapping of ZGPD commands IDs to Zigbee ZCL commands IDs
 */
zgp_to_zb_cmd_mapping_t g_cmd_mapping[] =
{
#if defined ZGPS_MATCH_TBL_TEMPERATURE_SENSOR_ENTRY
    {ZB_GPDF_CMD_ATTR_REPORT, ZB_ZCL_CMD_REPORT_ATTRIB},
#endif
};


/**
 * Default matching info
 */
zb_zgps_match_info_t  g_default_zgps_match_info =
{
  ZB_ARRAY_SIZE(g_zgps_match_tbl),
  g_zgps_match_tbl,
  ZB_ARRAY_SIZE(g_cmd_mapping),
  g_cmd_mapping,
  0,
  NULL
};
#endif  /* ZB_ZGP_SINK_SUPPORT_LEGACY_MATCH_INFO */
#endif  /* ZB_ENABLE_ZGP_SINK */
#endif //ZB_ZGP_DEFAULT_MATCH_INFO_H
