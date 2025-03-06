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
/* PURPOSE: Common include file for ZigBee
*/
#ifndef ZB_COMMON_H
#define ZB_COMMON_H 1

#include "zb_config.h"

#ifdef ZB_MINIMAL_CONTEXT

#include "zb_g_context_min.h"

#else

#include "zboss_api.h"
#include "zb_types.h"
#include "zb_errors.h"
#include "zb_diag.h"
#include "zb_debug.h"
#include "zb_g_context.h"
#include "zb_trace.h"
#include "zb_hash.h"
#ifdef ZB_TH_ENABLED
#include "zb_th_internal.h"
#else
#include "zb_th_dummy.h"
#endif /* ZB_TH_ENABLED */
#include "zb_common_u.h"

/**
   This feature is not used in regular deliveries and not supported by ZOI official platforms.
   Refer to zboss_api.h for more info.
*/
#ifdef ZB_EXTENDED_VERSION_INFO
#include "zb_sdk_version.h"
#endif

#include "zb_nvram.h"

#include "zb_error_indication.h"

/*! \addtogroup init_api */
/*! @{ */

void zb_globals_init(void);

#if defined ENABLE_USB_SERIAL_IMITATOR
#define USB_SERIAL_IMITATOR_RX_PIPE "usbc_rx"
#define USB_SERIAL_IMITATOR_TX_PIPE "usbc_tx"
#endif /* defined ENABLE_USB_SERIAL_IMITATOR */

/*! @} */

/*! \internal \addtogroup ZB_BASE */
/*! @{ */

/**
   Set Informational Bases defaults.

   @param rx_pipe - rx pipe name (for Unix) or node number (for ns build
                            in 8051 simulator)
 */
void zb_ib_set_defaults(zb_char_t *rx_pipe);

enum zb_dev_specific_e
{
  /*! Standard device */
  ZB_STANDARD_SPECIFIC_DEVICE = 0x00,
  /*! Manufactured device */
  ZB_MANUFACTURED_SPECIFIC_DEVICE = 0x01
};

/* Definitions for multiple tests in the same FW image / same Linux binary */
#ifdef ZB_MULTI_TEST
extern zb_int_t g_argc;
extern char** g_argv;
#define ZB_ARGV g_argv
#define ZB_ARGC g_argc
#undef MAIN
#undef ARGV_UNUSED
#define ARGV_UNUSED
#undef MAIN_RETURN
#define MAIN_RETURN(x)
#define TN_CAT0(x,y) x ## y
#define TN_CAT(x,y) TN_CAT0(x,y)
#define MAIN void TN_CAT(ZB_TEST_NAME,_main)
#define ZB_ZDO_STARTUP_COMPLETE void TN_CAT(ZB_TEST_NAME,_zb_zdo_startup_complete)
#define ZGPD_STARTUP_COMPLETE void TN_CAT(ZB_TEST_NAME,_zgpd_startup_complete)
#ifdef ZB_INIT_HAS_ARGS
#define TO_STR(s) #s
#define MACRO_TO_STR(s) TO_STR(s)
/* Stringizing in C
 * When a macro parameter is used with a leading ‘#’,
 * the preprocessor replaces it with the literal text
 * of the actual argument, converted to a string constant
 */
#define ZB_TEST_NAME_STR MACRO_TO_STR(ZB_TEST_NAME)
#undef ZB_INIT
/* ZB_TEST_NAME_STR contains a value of the ZB_TEST_NAME macro as a const string
 * sizeof() of a const string returns a string size taking into account the null-symbol
 */
#define ZB_INIT(trace_comment)         \
  {                                    \
    zb_char_t buf[64];                 \
    ZVUNUSED(trace_comment);           \
    ZB_MEMCPY(buf, ZB_TEST_NAME_STR, sizeof(ZB_TEST_NAME_STR)); \
    ZB_CHECK_LIBRARY();                \
    zb_init(buf);                      \
    zb_diag_init();                    \
  }
#endif /* ZB_INIT_HAS_ARGS */

/* List of MAC primitives' callbacks */
#define ZB_MLME_ASSOCIATE_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mlme_associate_indication)
#define ZB_MLME_ASSOCIATE_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_associate_confirm)
#define ZB_MLME_BEACON_NOTIFY_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mlme_beacon_notify_indication)
#define ZB_MLME_COMM_STATUS_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mlme_comm_status_indication)
#define ZB_MLME_ORPHAN_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mlme_orphan_indication)
#define ZB_MLME_RESET_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_reset_confirm)
#define ZB_MLME_SCAN_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_scan_confirm)
#define ZB_MLME_START_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_start_confirm)
#define ZB_MLME_POLL_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_poll_confirm)
#define ZB_MLME_PURGE_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mlme_purge_confirm)
#define ZB_MCPS_DATA_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mcps_data_indication)
#define ZB_MCPS_DATA_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_mcps_data_confirm)
#define ZB_MLME_DUTY_CYCLE_MODE_INDICATION void TN_CAT(ZB_TEST_NAME,_zb_mlme_duty_cycle_mode_indication)
#define ZB_PLME_CCA_CONFIRM void TN_CAT(ZB_TEST_NAME,_zb_plme_cca_confirm)

#else  /* ZB_MULTI_TEST */

#define ZB_ZDO_STARTUP_COMPLETE void zb_zdo_startup_complete
#define ZGPD_STARTUP_COMPLETE void zgpd_startup_complete

  /* List of MAC primitives' callbacks */
#define ZB_MLME_ASSOCIATE_INDICATION void zb_mlme_associate_indication
#define ZB_MLME_ASSOCIATE_CONFIRM void zb_mlme_associate_confirm
#define ZB_MLME_BEACON_NOTIFY_INDICATION void zb_mlme_beacon_notify_indication
#define ZB_MLME_COMM_STATUS_INDICATION void zb_mlme_comm_status_indication
#define ZB_MLME_ORPHAN_INDICATION void zb_mlme_orphan_indication
#define ZB_MLME_RESET_CONFIRM void zb_mlme_reset_confirm
#define ZB_MLME_SCAN_CONFIRM void zb_mlme_scan_confirm
#define ZB_MLME_START_CONFIRM void zb_mlme_start_confirm
#define ZB_MLME_POLL_CONFIRM void zb_mlme_poll_confirm
#define ZB_MLME_PURGE_CONFIRM void zb_mlme_purge_confirm
#define ZB_MCPS_DATA_INDICATION void zb_mcps_data_indication
#define ZB_MCPS_DATA_CONFIRM void zb_mcps_data_confirm
#define ZB_MLME_DUTY_CYCLE_MODE_INDICATION void zb_mlme_duty_cycle_mode_indication
#define ZB_PLME_CCA_CONFIRM void zb_plme_cca_confirm

#define ZB_ARGV argv
#define ZB_ARGC argc

#endif  /* ZB_MULTI_TEST */

/**
 * @brief Implements "if (a) then (b)" logical operation.
 *
 * @b Example:
 * Instead of:
 * @code
 * {
 *   if(ret == RET_OK)
 *     ZB_ASSERT(*p != NULL);
 * }
 * @endcode
 * It is possible to write as:
 * @code
 * {
 *   ZB_ASSERT(ZB_THEREFORE(ret == RET_OK, *p != NULL));
 * }
 *  @endcode
 * This approach avoid dead code when ZB_ASSERT is not defined
 */
#define ZB_THEREFORE(a, b) (!(a) || (b))

/**
 * @brief Assert using #ZB_THEREFORE macro
 */
#define ZB_ASSERT_IF(cond, assert) ZB_ASSERT(ZB_THEREFORE((cond), (assert)))

#ifndef ZB_CONFIGURABLE_MEM
#define ZB_RESYNC_CFG_MEM()
#else /* ZB_CONFIGURABLE_MEM */
void zb_assign_global_pointers(void);
void zb_init_configurable_mem(int clear);
#define ZB_RESYNC_CFG_MEM() zb_init_configurable_mem(0)
#endif /* ZB_CONFIGURABLE_MEM */

#if !defined ZB_LEGACY_BUFS
#ifdef zb_buf_t
#undef zb_buf_t
#undef ZB_BUF_ALLOC_LEFT
#undef ZB_BUF_ALLOC_RIGHT
#undef ZB_BUF_BEGIN_FROM_REF
#undef ZB_BUF_BEGIN
#undef ZB_BUF_CLEAR_PARAM
#undef ZB_BUF_COPY
#undef ZB_BUF_CUT_LEFT2
#undef ZB_BUF_CUT_LEFT
#undef ZB_BUF_CUT_RIGHT
#undef ZB_BUF_FROM_REF
#undef ZB_BUF_GET_ARRAY_PTR
#undef ZB_BUF_INITIAL_ALLOC
#undef ZB_BUF_LEN_BY_REF
#undef ZB_BUF_LEN
#undef ZB_BUF_OFFSET
#undef ZB_BUF_REUSE
#undef ZB_FREE_BUF
#undef ZB_FREE_BUF_BY_REF
#undef ZB_GET_BUF_PARAM
#undef ZB_GET_BUF_TAIL
#undef ZB_GET_IN_BUF
#undef ZB_GET_IN_BUF_DELAYED
#undef ZB_GET_OUT_BUF
#undef ZB_GET_OUT_BUF_DELAYED2
#undef ZB_GET_OUT_BUF_DELAYED
#undef ZB_REF_FROM_BUF
#undef ZB_REF_FROM_BUF_PTR
#undef ZB_SET_BUF_PARAM_PTR
#undef ZB_SET_BUF_PARAM
#undef ZB_SWITCH_BUF

#endif /*zb_buf_t */

#define ZB_BUF_ALLOC_LEFT(zbbuf, size, ptr)           No old buf API
#define ZB_BUF_ALLOC_RIGHT(zbbuf, size, ptr)          No old buf API
#define ZB_BUF_BEGIN_FROM_REF(_ref)                   No old buf API
#define ZB_BUF_BEGIN(zbbuf)                           No old buf API
#define ZB_BUF_CLEAR_PARAM(zbbuf)                     No old buf API
#define ZB_BUF_COPY(dst_buf, src_buf)                 No old buf API
#define ZB_BUF_CUT_LEFT2(zbbuf, size)                 No old buf API
#define ZB_BUF_CUT_LEFT(zbbuf, size, ptr)             No old buf API
#define ZB_BUF_CUT_RIGHT(zbbuf, size)                 No old buf API
#define ZB_BUF_FROM_REF(ref)                          No old buf API
#define ZB_BUF_GET_ARRAY_PTR(zbbuf)                   No old buf API
#define ZB_BUF_INITIAL_ALLOC(zbbuf, size, ptr)        No old buf API
#define ZB_BUF_LEN_BY_REF(param)                      No old buf API
#define ZB_BUF_LEN(zbbuf)                             No old buf API
#define ZB_BUF_OFFSET(zbbuf)                          No old buf API
#define ZB_BUF_REUSE(zbbuf)                           No old buf API
#define ZB_FREE_BUF(buf)                              No old buf API
#define ZB_FREE_BUF_BY_REF(param)                     No old buf API
#define ZB_GET_BUF_PARAM(zbbuf, type)                 No old buf API
#define ZB_GET_BUF_TAIL(zbbuf, size)                  No old buf API
#define ZB_GET_IN_BUF()                               No old buf API
#define ZB_GET_IN_BUF_DELAYED(callback)               No old buf API
#define ZB_GET_OUT_BUF()                              No old buf API
#define ZB_GET_OUT_BUF_DELAYED2(callback, user_param) No old buf API
#define ZB_GET_OUT_BUF_DELAYED(callback)              No old buf API
#define ZB_REF_FROM_BUF(buf)                          No old buf API
#define ZB_REF_FROM_BUF_PTR(ptr)                      No old buf API
#define ZB_SET_BUF_PARAM_PTR(zbbuf, param, type)      No old buf API
#define ZB_SET_BUF_PARAM(zbbuf, param, type)          No old buf API
#define ZB_SWITCH_BUF(buf, to_in)                     No old buf API
#endif /* !defined ZB_LEGACY_BUFS */

/*
 * Static compile time assertion to make sure that 'zb_bool_t' has the size
 * of exactly one byte, regardless of the used platform.
 */
ZB_ASSERT_TYPE_SIZE_NOT_GREATER(zb_bool_t, 1);

#endif /* ZB_MINIMAL_CONTEXT */

#endif /* ZB_COMMON_H */
