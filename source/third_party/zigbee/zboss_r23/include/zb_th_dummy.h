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
/*  PURPOSE: Stub macro for build without TH
*/

#ifndef ZB_TH_INTERNAL_H
#define ZB_TH_INTERNAL_H 1

#ifndef ZB_TH_ENABLED

#define ZB_TH_PUSH_PACKET(primitive_type, call_type, bufid)
#define ZB_TH_PUSH_PACKET_TAIL(primitive_type, call_type, bufid, size)
#define ZB_TH_PUSH_NLDE_DATA_CONFIRM(bufid)
#define ZB_TH_PUSH_APSDE_DATA_CONFIRM(bufid)

#endif /* ZB_TH_ENABLED */
#endif /* ZB_TH_INTERNAL_H */
