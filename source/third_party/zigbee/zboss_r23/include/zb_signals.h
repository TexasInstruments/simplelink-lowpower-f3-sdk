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
/* PURPOSE: ZBOSS signal handlers - internals
*/

#ifndef ZB_SIGNALS_H
#define ZB_SIGNALS_H 1

#include "zb_common.h"

/*! @cond internals_doc */
/*! \addtogroup ZB_SIGNALS */
/*! @{ */

typedef struct zb_signals_globals_s
{
    zb_signal_handler_t signal_handlers[ZB_SIGNAL_HANDLERS_MAX];
    zb_uindex_t signal_handlers_count;
} zb_signals_globals_t;

/**
 * @brief Signal dispathcher. 
 * 
 * Shall be called by ZBOSS internally in order to propagate any signal to consumers.
 * 
 * @param param - reference to the buffer which contains signal. See @ref zb_get_app_signal.
 */
void zb_signal_dispatch(zb_uint8_t param);

#endif /* ZB_SIGNALS_H */
