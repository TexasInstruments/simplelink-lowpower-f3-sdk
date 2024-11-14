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
/* PURPOSE: HA global definitions
*/

#ifndef ZB_HA_H
#define ZB_HA_H 1

#ifdef ZB_ENABLE_HA
#include "ha/zb_ha_config.h"
#include "ha/zb_ha_sas.h"

#include "ha/zb_ha_door_lock.h"
#include "ha/zb_ha_door_lock_controller.h"
#include "ha/zb_ha_on_off_output.h"
#include "ha/zb_ha_on_off_switch.h"
#include "ha/zb_ha_simple_sensor.h"
#include "ha/zb_ha_combined_interface.h"
#include "ha/zb_ha_scene_selector.h"
#include "ha/zb_ha_configuration_tool.h"
#include "ha/zb_ha_mains_power_outlet.h"
#include "ha/zb_ha_range_extender.h"
#include "ha/zb_ha_level_control_switch.h"
#include "ha/zb_ha_level_controllable_output.h"
#include "ha/zb_ha_window_covering.h"
#include "ha/zb_ha_window_covering_controller.h"
#include "ha/zb_ha_shade.h"
#include "ha/zb_ha_shade_controller.h"

#include "ha/zb_ha_temperature_sensor.h"

#include "ha/zb_ha_ias_control_indicating_equipment.h"
#include "ha/zb_ha_ias_ancillary_control_equipment.h"
#include "ha/zb_ha_ias_zone.h"
#include "ha/zb_ha_ias_warning_device.h"

#include "ha/zb_ha_custom_attr.h"
#include "ha/zb_ha_dimmable_light.h"
#include "ha/zb_ha_dimmer_switch.h"
#include "ha/zb_ha_smart_plug.h"
#include "ha/zb_ha_thermostat.h"

#include "ha/zb_ha_test_device.h"

#include "ha/zb_ha_erl_device_interface.h"
#include "ha/zb_ha_erl_gw_device.h"
#endif
#endif /* ZB_HA_H */
