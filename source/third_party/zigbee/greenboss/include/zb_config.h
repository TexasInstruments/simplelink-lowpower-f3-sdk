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
/* PURPOSE: Configuration file: configurations and definitions,
constants etc.
*/

#ifndef ZB_CONFIG_H
#define ZB_CONFIG_H 1

#include "zb_config_platform.h"

/**
 * @addtogroup buildconfig
 * @{
 */

#if defined(__APPLE__) && defined(__MACH__)
#ifndef MACOSX
#define MACOSX
#endif /* MACOSX */
#endif /* __APPLE__ && __MACH__ */

/************************************************/

#define ZB_LITTLE_ENDIAN

/**
   Name for trace off switch file

   If a file with this name exists in the current directory, switch off both trace
   and traffic dump.
*/
#define ZB_TRACE_SWITCH_OFF_FILE_NAME "trace_off"

#define ZGPD_MAX_PACKET_SIZE           128
#define ZGPD_MAX_ATTR_REPORT_DATA_SIZE 100

#define ZGPD_MIN_CHANNEL_NUM           11
#define ZGPD_MAX_CHANNEL_NUM           26

#define ZB_SECURITY_KEY_LEN            16

#define ZB_RANDOM_SEQ_NUMBER

#define ZB_USE_NVRAM
#define ZB_SOFTWARE_FCS

#ifndef ZGPD_COMM_CHANNEL_REQ_WAIT_MS
#define ZGPD_COMM_CHANNEL_REQ_WAIT_MS 200U
#endif

#ifndef ZGPD_COMM_COMMISSIONING_WAIT_MS
#define ZGPD_COMM_COMMISSIONING_WAIT_MS 200U
#endif

#ifndef ZGPD_COMM_SUCCESS_WAIT_MS
#define ZGPD_COMM_SUCCESS_WAIT_MS 50U
#endif 

#ifndef ZGPD_COMM_OPERATIONAL_WAIT_MS
#define ZGPD_COMM_OPERATIONAL_WAIT_MS 50U
#endif

#ifndef ZGPD_RX_OFFSET_MS
#define ZGPD_RX_OFFSET_MS 20U
#endif

#ifndef ZGPD_RX_WAIT_MS
#define ZGPD_RX_WAIT_MS 5U
#endif

#ifndef ZGPD_MIN_RX_WINDOW_MS
#define ZGPD_MIN_RX_WINDOW_MS 20U
#endif

#ifndef ZB_MAC_DEFAULT_TX_POWER
#define ZB_MAC_DEFAULT_TX_POWER 5
#endif

/** @cond DOXYGEN_INTERNAL_DOC */
/*
  If enabled, real int24 and uint48 types will work. Else, int24 and uint48 is mapped to
  int32 and uint32.
  See include/zb_uint48.h.
*/
/** Enable support for int24 and uint48 data types */
#ifndef ZB_NO_UINT24_48_SUPPORT
#define ZB_UINT24_48_SUPPORT
#endif /* !ZB_NO_UINT24_48_SUPPORT */
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/**
   NVM, commissioning page number
 */
#define ZB_COMMISSIONING_PAGE       0
/**
   MVM, MAC sequence first page number
 */
#define ZB_MSS_FIRST_PAGE           1
/**
   Number of pages used for MAC sequence storage
 */
#ifndef ZB_MSS_PAGE_COUNT
#define ZB_MSS_PAGE_COUNT           10
#endif /* ZB_MSS_PAGE_COUNT */

/** @} */

#endif /* ZB_CONFIG_H */
