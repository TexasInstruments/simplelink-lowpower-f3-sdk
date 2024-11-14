/*
* Copyright (c) 2021, Nordic Semiconductor ASA
*
* All rights reserved.
*
* Commercial Usage
* Licensees holding valid Commercial licenses may use
* this file in accordance with the Commercial License
* Agreement provided with the Software or, alternatively, in accordance
* with the terms contained in a written agreement relevant to the usage of the file.
*/
/*  PURPOSE: NCP Dev API declarations
*/
#ifndef NCP_DEV_API_H
#define NCP_DEV_API_H 1

#include "zb_types.h"
#include "ncp_common_api.h"

/** @cond DOXYGEN_NCP_SECTION */

/** @addtogroup ncp_dev_api
  *  @{
  */

/** Return code which is raised if @ref zb_ncp_custom_response will be called later*/
#define NCP_RET_LATER (255U)


/** The custom request callback.
 *
 * @param param - the buffer with request data from host and tsn (@ref zb_uint8_t) as a parameter.
 *                It's freed by the stack afterwards.
 *
 * @return The length of a response returned by @ref zb_ncp_custom_response if it is called within callback.
 *         @ref NCP_RET_LATER if @ref zb_ncp_custom_response is called later.
 *
 * @note If returned length is equal to 0, the response is generated and sent automatically.
 */
typedef zb_uint16_t (*zb_ncp_custom_request_cb_t)(zb_uint8_t param);

/** Registers a callback, that is called once the zb_ncp_custom_request is called on the host side
 *  and the corresponding NCP command is received.
 *
 * @param cb - the callback. If NULL is passed, previously registered callback (if any) will be unregistered.
 */
void zb_ncp_custom_register_request_cb(zb_ncp_custom_request_cb_t cb);

/** Sends the indication to the Host.
 *
 * @param param - the ZBOSS buffer with payload. It's freed by the stack.
 */
void zb_ncp_custom_indication(zb_uint8_t param);

/** Sends a custom response.
 *
 * @param param - the ZBOSS buffer with response payload and @ref ncp_hl_custom_resp_t as a parameter.
 *                It's freed by the stack.
 *
 * @return Response length
 *
 * @note Should be called within @ref zb_ncp_custom_request_cb_t unless @ref NCP_RET_LATER is not returned from callback.
 */
zb_uint16_t zb_ncp_custom_response(zb_uint8_t param);

/** @} */ /* ncp_dev_api */

/** @endcond */ /* DOXYGEN_NCP_SECTION */

#endif /* NCP_DEV_API_H */
