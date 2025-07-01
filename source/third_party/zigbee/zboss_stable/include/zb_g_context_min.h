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
/* PURPOSE: Global context definition - minimal case - scheduler + pool
*/

#ifndef ZB_G_CONTEXT_MIN_H
#define ZB_G_CONTEXT_MIN_H 1

/*! @cond internals_doc */
/*! \addtogroup ZB_BASE */
/*! @{ */

#include "zb_config.h"
#include "zb_types.h"
#include "zb_errors.h"

#include "zb_osif.h"

/**
  \par Define 'global minimal context' - A common minimal global data structure.

  A minimal data structure based on global context to reduce the dependencies of
  this context, therefore avoiding MISRA violations.

  This implementation is mainly used by applications without zigbee features,
  like bootloaders.
 */

struct zb_globals_s;
typedef struct zb_globals_s zb_globals_t;

struct zb_intr_globals_s;
typedef ZB_VOLATILE struct zb_intr_globals_s zb_intr_globals_t;

extern zb_globals_t g_zb;
extern zb_intr_globals_t g_izb;

/**
   Macro to access globals
 */
/* Hope compiler can optimize &g_zb-> to g_zb. */
#define ZG (&g_zb)
#define ZIG (&g_izb)

/*
  Per-subsystem globals files are named like zb_xxx_globals.h and included here.
 */
#include "zb_scheduler.h"
#include "zb_bufpool_globals.h"
#include "zb_time.h"

/**
   Global data area for data not to be accessed from interrupt handlers
*/
struct zb_globals_s
{
  zb_sched_globals_t      sched;    /*!< Global schedule context */
  zb_buf_pool_t           bpool;    /*!< Global buffer pool context */
};

/**
   Global data area for data to be accessed from interrupt handlers
 */
struct zb_intr_globals_s
{
  zb_timer_t  time;
};

#define ZB_TIMER_CTX() g_izb.time

/*! @} */
/*! @endcond */

#endif /* ZB_G_CONTEXT_MIN_H */
