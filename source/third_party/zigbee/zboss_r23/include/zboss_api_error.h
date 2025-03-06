/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2022 DSR Corporation, Denver CO, USA.
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
/*  PURPOSE: ZBOSS error API header. Error handling API.
*/

#ifndef ZB_ZBOSS_API_ERROR_H
#define ZB_ZBOSS_API_ERROR_H 1

#define ZB_ERROR_SEVERITY_MINOR 0x01
#define ZB_ERROR_SEVERITY_MAJOR 0xFE
#define ZB_ERROR_SEVERITY_FATAL 0xFF

typedef struct zb_verify_additional_info_s {
  zb_uint16_t file_id;
  zb_int_t line_number;
  void *caller_additional_info;
} zb_verify_additional_info_t;

/**
 * @brief   Definition of error handler. It must be registered by application using
 *          @ref zb_error_register_app_handler .
 * 
 * @note    Severity @ref ZB_ERROR_SEVERITY_FATAL is excusive for errors raised by ZB_VERIFY
 *          or ZB_VERIFY_INFO
 * 
 * @note    If the registered callback is called with severity @ref ZB_ERROR_SEVERITY_FATAL ,
 *          the argument additional_info will point to a structure of type
 *          @ref zb_verify_additional_info_t , in which its parameter @ref caller_additional_info
 *          will point to the original additional info used by the caller in @ref  ZB_VERIFY_INFO
 *          (or NULL, in case of @ref ZB_VERIFY)
 * 
 * @param severity        severity level of the error
 * @param error_code      error code being raised
 * @param additional_info additional error-dependent info
 */
typedef zb_bool_t (*zb_error_handler_t)(zb_uint8_t severity,
                                        zb_ret_t error_code,
                                        void *additional_info);

typedef struct zb_error_indication_ctx_s
{
  zb_error_handler_t app_err_handler;
} zb_error_indication_ctx_t;

/**
 * Set application error handler.
 *
 * @param cb - an error handler to setAddr
 */
void zb_error_register_app_handler(zb_error_handler_t cb);

/**
  Verify: Raise error and interrupt application

  @param file_id          source file id
  @param line_number      line in the source
  @param error_code       error code to be used
  @param additional_info  any additional error-dependent info
*/
ZB_NORETURN void zb_verify(zb_uint16_t file_id, zb_int_t line_number, zb_ret_t error_code, void *additional_info);
/**
   Check for expression in runtime and call zb_verify() if it is false.

  @param condition        expression to check
  @param error_code       error code to be used if the expression is false
  @param additional_info  any additional error-dependent info
*/
#define ZB_VERIFY_INFO(condition, error_code, additional_info) do {if(!(condition)) { zb_verify(ZB_TRACE_FILE_ID, __LINE__, error_code, additional_info);} } while (0)

/**
  Check for expression in runtime and call zb_verify() if it is false.

  @param condition        expression to check
  @param error_code       error code to be used if the expression is false - @ref ERROR_CODE macro
*/
#define ZB_VERIFY(condition, error_code) ZB_VERIFY_INFO(condition, error_code, NULL)


/* ERROR_CATEGORY_MACSPLIT */
/* MAC-Split Radio Failure error indication code */
#define ZB_ERROR_MACSPLIT_RADIO_FAILURE 1u /* unexpected SoC reboot */
#define ZB_ERROR_MACSPLIT_RADIO_REBOOT  ZB_ERROR_MACSPLIT_RADIO_FAILURE
#define ZB_ERROR_MACSPLIT_RADIO_HANG    2u /* no ACK etc */

/* ERROR_CATEGORY_SYSTEM */
#define ZB_ERROR_ZBBUF_INVALID_PTR 1u

#endif /* ZB_ZBOSS_API_ERROR_H */
