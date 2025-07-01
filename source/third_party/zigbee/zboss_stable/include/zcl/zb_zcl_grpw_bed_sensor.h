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
/* PURPOSE: Bed Sensor for GreenPower cluster
*/

#ifndef ZB_ZCL_GRPW_BED_SENSOR_H
#define ZB_ZCL_GRPW_BED_SENSOR_H 1

#define ZB_ZCL_CLUSTER_ID_GRPW_BED_SENSOR  0xfd00

enum zb_zcl_grpw_bed_sensor_attr_e
{
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_STATUS_ID                   = 0x0000,
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_CALC_DATA_0_ID              = 0x0010,
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_CALC_DATA_1_ID              = 0x0011,
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_SENSITIVITY_SETTING         = 0x0020,
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_CALC_DATA_SEND_INTERVAL     = 0x0021,
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_CALC_DATA_SEND_MODE         = 0x0022,
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_EXIT_DETECT_ADD_DELAY       = 0x0023,
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_OCCUPANCY_DETECT_ALG_SEL    = 0x0024,
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_TYPE                        = 0x0025,
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_STATUS_REPORT_INTERVAL      = 0x0026,
  ZB_ZCL_ATTR_GRPW_BED_SENSOR_COMMANDS_FOR_SENSING_MODULE = 0x0030,
  ZB_ZCL_ATTR_GRPW_BED_PENDING_CONFIGURATION              = 0x00F0,
};

#define ZB_ZCL_CLUSTER_ID_GRPW_BED_SENSOR_SERVER_ROLE_INIT (zb_zcl_cluster_init_t)NULL
#define ZB_ZCL_CLUSTER_ID_GRPW_BED_SENSOR_CLIENT_ROLE_INIT (zb_zcl_cluster_init_t)NULL

#endif /* ZB_ZCL_GRPW_BED_SENSOR_H */
