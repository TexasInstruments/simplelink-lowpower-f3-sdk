/* GreenBOSS Zigbee Green Power Device software protocol stack
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
 * www.dsr-iot.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 * This is unpublished proprietary source code of DSR Corporation
 * The copyright notice does not evidence any actual or intended
 * publication of such source code.
 *
 * GreenBOSS is a registered trademark of Data Storage Research LLC d/b/a DSR
 * Corporation
 *
 * Commercial Usage
 * Licensees holding valid DSR Commercial licenses may use
 * this file in accordance with the DSR Commercial License
 * Agreement provided with the Software or, alternatively, in accordance
 * with the terms contained in a written agreement between you and
 * DSR.
 */
/* PURPOSE: Packet buffers pool
*/

#ifndef ZB_BUFPOOL_H
#define ZB_BUFPOOL_H 1

#define ZB_BUF_BEGIN(a) (&(a)[1])
#define ZB_BUF_LEN(a)   ((a)[0])
#define ZB_BUF_SET_MAX_LEN(a)   ((a)[0] = sizeof((a)) - 1)

#endif
