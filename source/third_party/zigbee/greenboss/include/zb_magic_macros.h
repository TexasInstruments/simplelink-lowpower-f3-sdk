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
/* PURPOSE: Utility macros */

#ifndef ZB_MAGIC_MACROS_H
#define ZB_MAGIC_MACROS_H 1

#if defined(__GNUC__)
#define MAGIC_CLZ(x) __builtin_clz(x)
#elif _MSC_VER > 0
#define MAGIC_CLZ(x) __lzcnt(x);
#elif defined(__ICCARM__)
#include "intrinsics.h"
#define MAGIC_CLZ(x) __CLZ(x)
#else
#error Implement MAGIC_CLZ elsewhere!
#endif



#endif
