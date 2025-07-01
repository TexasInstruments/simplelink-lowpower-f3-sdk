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

#include "zb_types.h"

#ifndef ZB_STACK_INFO_H
#define ZB_STACK_INFO_H 1

#define ZB_MAX_VERSION_LENGTH 10U

typedef ZB_PACKED_PRE struct zb_stack_information_s
{
  zb_uint8_t version[ZB_MAX_VERSION_LENGTH];
  zb_uint_t revision;
} ZB_PACKED_STRUCT 
zb_stack_information_t;

void zb_get_stack_information(zb_stack_information_t *stack_info);
#endif /* ZB_STACK_INFO_H */
