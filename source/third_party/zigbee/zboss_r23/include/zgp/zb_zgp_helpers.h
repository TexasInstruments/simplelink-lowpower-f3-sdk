/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: ZGPD helper header for LCGW.
*/

#include "zb_common.h"
#include "zboss_api_zgp.h"

#ifdef ZB_ENABLE_ZGP_SINK

void zb_zgps_read_attrs_command(
  zb_uint8_t param, zb_zgpd_id_t *dev, zb_uint16_t cluster_id,
  zb_uint16_t *attrs, zb_uint8_t attrs_len);

void zb_zgps_write_attrs_command(
  zb_uint8_t param, zb_zgpd_id_t *zgpd_id, zb_uint16_t cluster_id,
  zb_uint8_t attrs_size, zb_gpdf_attr_write_fld_t *attrs);

#endif  /* ZB_ENABLE_ZGP_SINK */
