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
/*  PURPOSE: Error codes
*/

#ifndef ZB_ERRORS_H
#define ZB_ERRORS_H 1

/**
 * @addtogroup base_types
 * @{
 */
/** @brief Return type for ZB functions returning execution status. @see ::RET_OK. */
typedef zb_int32_t zb_ret_t;
/** @} */

/*! @cond internals_doc */

/* categories */

#define ERROR_CATEGORY_INTERVAL 256

#define   ERROR_CATEGORY_GENERIC  0
#define   ERROR_CATEGORY_SYSTEM   1
#define   ERROR_CATEGORY_MAC      2
#define   ERROR_CATEGORY_NWK      3
#define   ERROR_CATEGORY_APS      4
#define   ERROR_CATEGORY_ZDO      5
#define   ERROR_CATEGORY_CBKE     6
#define   ERROR_CATEGORY_WATCHDOG 7
#define   ERROR_CATEGORY_SERIAL   8
#define   ERROR_CATEGORY_NVRAM    9
#define   ERROR_CATEGORY_MACSPLIT 10
#define   ERROR_CATEGORY_NCP      11
#define   ERROR_CATEGORY_EXTMAC   12

/* Let's always return 0 for RET_OK - in any category. */
#define ERROR_CODE(category, code) ((zb_ret_t)(((zb_ret_t)code) != 0 ? -(((category) * ERROR_CATEGORY_INTERVAL) + ((zb_ret_t)code)) : 0))
#define GENERIC_ERROR_CODE(code)   ERROR_CODE(ERROR_CATEGORY_GENERIC, code)
#define SYSTEM_ERROR_CODE(code)    ERROR_CODE(ERROR_CATEGORY_SYSTEM, code)

#define ERROR_GET_CATEGORY(err) (((-(zb_int_t)(err))) / ERROR_CATEGORY_INTERVAL)
#define ERROR_GET_CODE(err) ((-(zb_int_t)(err)) % ERROR_CATEGORY_INTERVAL)

/** @endcond */ /* internals_doc */

/**
 *  @addtogroup error_codes
 *  @{
 */

/**
 *  @brief Error codes for non-void stack functions.
 *  In general, function can return OK, BLOCKED or some error. Errors are negative.
 *
 *  Error can be "generic" or some additional error code.
 */

/* Most common return types: ok, generic error, BLOCKED, thread exit indication. */
#define  RET_OK              0
#define  RET_ERROR           ERROR_CODE(ERROR_CATEGORY_GENERIC, 1) /* -1 indeed */
#define  RET_BLOCKED         ERROR_CODE(ERROR_CATEGORY_GENERIC, 2)
#define  RET_EXIT            ERROR_CODE(ERROR_CATEGORY_GENERIC, 3)
#define  RET_BUSY            ERROR_CODE(ERROR_CATEGORY_GENERIC, 4)
#define  RET_EOF             ERROR_CODE(ERROR_CATEGORY_GENERIC, 5)
#define  RET_OUT_OF_RANGE    ERROR_CODE(ERROR_CATEGORY_GENERIC, 6)
#define  RET_EMPTY           ERROR_CODE(ERROR_CATEGORY_GENERIC, 7)
#define  RET_CANCELLED       ERROR_CODE(ERROR_CATEGORY_GENERIC, 8)


#define  RET_INVALID_PARAMETER_1                 ERROR_CODE(ERROR_CATEGORY_GENERIC, 10)
#define  RET_INVALID_PARAMETER_2                 ERROR_CODE(ERROR_CATEGORY_GENERIC, 11)
#define  RET_INVALID_PARAMETER_3                 ERROR_CODE(ERROR_CATEGORY_GENERIC, 12)
#define  RET_INVALID_PARAMETER_4                 ERROR_CODE(ERROR_CATEGORY_GENERIC, 13)
#define  RET_INVALID_PARAMETER_6                 ERROR_CODE(ERROR_CATEGORY_GENERIC, 15)
#define  RET_PENDING                             ERROR_CODE(ERROR_CATEGORY_GENERIC, 21)
#define  RET_NO_MEMORY                           ERROR_CODE(ERROR_CATEGORY_GENERIC, 22)
#define  RET_INVALID_PARAMETER                   ERROR_CODE(ERROR_CATEGORY_GENERIC, 23)
#define  RET_OPERATION_FAILED                    ERROR_CODE(ERROR_CATEGORY_GENERIC, 24)
#define  RET_BUFFER_TOO_SMALL                    ERROR_CODE(ERROR_CATEGORY_GENERIC, 25)
#define  RET_END_OF_LIST                         ERROR_CODE(ERROR_CATEGORY_GENERIC, 26)
#define  RET_ALREADY_EXISTS                      ERROR_CODE(ERROR_CATEGORY_GENERIC, 27)
#define  RET_NOT_FOUND                           ERROR_CODE(ERROR_CATEGORY_GENERIC, 28)
#define  RET_OVERFLOW                            ERROR_CODE(ERROR_CATEGORY_GENERIC, 29)
#define  RET_TIMEOUT                             ERROR_CODE(ERROR_CATEGORY_GENERIC, 30)
#define  RET_NOT_IMPLEMENTED                     ERROR_CODE(ERROR_CATEGORY_GENERIC, 31)
#define  RET_NO_RESOURCES                        ERROR_CODE(ERROR_CATEGORY_GENERIC, 32)
#define  RET_UNINITIALIZED                       ERROR_CODE(ERROR_CATEGORY_GENERIC, 33)
#define  RET_INVALID_STATE                       ERROR_CODE(ERROR_CATEGORY_GENERIC, 35)
#define  RET_CONNECTION_FAILED                   ERROR_CODE(ERROR_CATEGORY_GENERIC, 37)
#define  RET_CONNECTION_LOST                     ERROR_CODE(ERROR_CATEGORY_GENERIC, 38)
#define  RET_UNAUTHORIZED                        ERROR_CODE(ERROR_CATEGORY_GENERIC, 40)
#define  RET_CONFLICT                            ERROR_CODE(ERROR_CATEGORY_GENERIC, 41)
#define  RET_INVALID_FORMAT                      ERROR_CODE(ERROR_CATEGORY_GENERIC, 42)
#define  RET_NO_MATCH                            ERROR_CODE(ERROR_CATEGORY_GENERIC, 43)
#define  RET_PROTOCOL_ERROR                      ERROR_CODE(ERROR_CATEGORY_GENERIC, 44)
#define  RET_VERSION                             ERROR_CODE(ERROR_CATEGORY_GENERIC, 45)
#define  RET_FILE_NOT_FOUND                      ERROR_CODE(ERROR_CATEGORY_GENERIC, 48)
#define  RET_CONVERSION_ERROR                    ERROR_CODE(ERROR_CATEGORY_GENERIC, 50)
#define  RET_FILE_CORRUPTED                      ERROR_CODE(ERROR_CATEGORY_GENERIC, 56)
#define  RET_PAGE_NOT_FOUND                      ERROR_CODE(ERROR_CATEGORY_GENERIC, 57)
#define  RET_ILLEGAL_REQUEST                     ERROR_CODE(ERROR_CATEGORY_GENERIC, 62)
#define  RET_INVALID_GROUP                       ERROR_CODE(ERROR_CATEGORY_GENERIC, 64)
#define  RET_TABLE_FULL                          ERROR_CODE(ERROR_CATEGORY_GENERIC, 65)
#define  RET_IGNORE                              ERROR_CODE(ERROR_CATEGORY_GENERIC, 69)
#define  RET_AGAIN                               ERROR_CODE(ERROR_CATEGORY_GENERIC, 70)
#define  RET_DEVICE_NOT_FOUND                    ERROR_CODE(ERROR_CATEGORY_GENERIC, 71)
#define  RET_OBSOLETE                            ERROR_CODE(ERROR_CATEGORY_GENERIC, 72)
#define  RET_INTERRUPTED                         ERROR_CODE(ERROR_CATEGORY_GENERIC, 73)
#define  RET_NULL_POINTER                        ERROR_CODE(ERROR_CATEGORY_GENERIC, 74)

/** @} */ /* Error codes */

#endif /* ZB_ERRORS_H */
