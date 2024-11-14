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
/*  PURPOSE: API shared for host and dev
*/
#ifndef NCP_COMMON_API_H
#define NCP_COMMON_API_H 1

#include "zb_types.h"
#include "zb_errors.h"

/** @cond DOXYGEN_NCP_SECTION */

/** @addtogroup ncp_common_api
 * @{
 */

/**
 * @brief It is used to create and read a response sent by @ref zb_ncp_custom_response
 */
typedef ZB_PACKED_PRE struct ncp_hl_custom_resp_s
{
  zb_ret_t status;
  zb_uint8_t tsn;
} ZB_PACKED_STRUCT ncp_hl_custom_resp_t;



/** @} */ /* ncp_common_api */

/** @endcond */ /* DOXYGEN_NCP_SECTION */

#endif /* NCP_COMMON_API_H */
