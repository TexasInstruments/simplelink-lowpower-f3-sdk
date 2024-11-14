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
/* PURPOSE: Error indication globals definition
*/

#ifndef ZB_ERROR_INDICATION_H
#define ZB_ERROR_INDICATION_H 1

#include "zb_types.h"
#include "zb_errors.h"
#include "zboss_api_error.h"

/**
 * Raise an error. Use ZB_ERROR_RAISE macro instead of this function.
 *
 * @param severity - a severity level of the error
 * @param err_code - an error code (@see zb_ret_t), consists of an error category and code,
 *                   @see ERROR_CODE.
 * @param additional_info - any additional error-dependent info
 */
void zb_error_raise(zb_uint8_t severity, zb_ret_t err_code, void *additional_info);

#ifdef HAVE_TOP_LEVEL_ERROR_HANDLER
/**
 * Top-level handler which is called before an application handler
 * if HAVE_TOP_LEVEL_ERROR_HANDLER is defined.
 */
zb_bool_t zb_error_top_level_handler(zb_uint8_t severity, zb_ret_t err_code, void *additional_info);
#endif /* HAVE_TOP_LEVEL_ERROR_HANDLER */

#define ZB_ERROR_RAISE(severity, err_code, additional_info) \
  zb_error_raise((severity), (err_code), (additional_info))

#endif  /* ZB_ERROR_INDICATION_H */
