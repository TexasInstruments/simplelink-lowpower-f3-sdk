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
/* PURPOSE: Tests and debug macros
*/

#ifndef ZB_DEBUG_H
#define ZB_DEBUG_H 1

/** \addtogroup gbdebug

@{
@defgroup ZB_DEBUG Asserts
@defgroup ZB_TRACE Debug Trace
@}
*/

/*! \addtogroup ZB_DEBUG */
/*! @{ */



/**
   @brief Traces current location, aborts current program execution - with core dump if possible.

   @param caller_file Source file name
   @param caller_line Line in the source
*/
void zb_abort(char *caller_file, int caller_line);

#if defined DEBUG || defined USE_ASSERT



#ifndef ZB_BINARY_TRACE
/**
   @brief Asserts printing diagnostic and forcing a core dump.

   @param file_name Source file name
   @param line_number Line in the source
*/

void zb_assert(zb_char_t *file_name, zb_int_t line_number);
#define ZB_ASSERT(expr) {if(!(expr)) zb_assert(__FILE__, __LINE__);}

#else  /* !ZB_BINARY_TRACE */
/**
   Asserts printing diagnostic and forcing a core dump.

   @param file_id Source file id
   @param line_number Line in the source
*/
void zb_assert(zb_uint16_t file_id, zb_int_t line_number);
#define ZB_ASSERT(expr) {if(!(expr)) zb_assert(ZB_TRACE_FILE_ID, __LINE__);}

#endif  /* !ZB_BINARY_TRACE */


#define ZB_INLINE_ASSERT_SIMPLE(expr)  ((expr) ? 1 : (zb_abort(__FILE__, __LINE__), 1))

/**
   Assert which can be used inside an expression.

   @param expr Expression to check
   @return Always 1 (true)
 */
#define ZB_INLINE_ASSERT(expr)  ZB_INLINE_ASSERT_SIMPLE((expr)),

#else  /* release */


#define ZB_ASSERT(expr) ((void)0)

#define ZB_INLINE_ASSERT_SIMPLE(expr) -1
#define ZB_INLINE_ASSERT(expr)

#endif


/*
   \par Compile-time asserts
 */

/* Tricks to force preprocessor to substitute __LINE__ in macros */
#define ZB_ASSERT_CAT_(a, b) a##b
#define ZB_ASSERT_CAT(a, b) ZB_ASSERT_CAT_(a, b)

/**
   Checks condition at compile time in the code (not declaration block).

   @param expr Expression to check
 */

#define ZB_ASSERT_COMPILE_TIME(expr) ((void)(zb_int_t (*)[(expr) ? 1 : -1])0)

/**
   Checks condition at compile time in the code (not declaration block), returns 0.
   To be used inside expressions.

   @param expr Expression to check
 */
#define ZB_ASSERT_COMPILE_TIME_EXPR(expr) ((zb_int_t)((zb_int_t (*)[(expr) ? 1 : -1])0))

/**
   Checks condition at compile time in the declaration block.

   @param expr Expression to check
 */
#define ZB_ASSERT_COMPILE_DECL(expr) typedef zb_int_t ZB_ASSERT_CAT(assert, __LINE__)[(expr) ? 1 : -1]


/**
   Ensures, that size of type 'type' is not greater than 'limit'. If it is not,
   compilation is aborted.

   @param type Type name
   @param limit Size limit
 */
#define ZB_ASSERT_TYPE_SIZE_NOT_GREATER(type, limit) ZB_ASSERT_COMPILE_DECL(sizeof(type) <= (limit))

/**
   Ensures, that size of type 'type' is not less than 'limit'. If it is not,
   compilation is aborted.

   @param type Type name
   @param limit Size limit
 */
#define ZB_ASSERT_TYPE_SIZE_NOT_LESS(type, limit) ZB_ASSERT_COMPILE_DECL(sizeof(type) >= (limit))

/**
   Ensures, that size of type 'type' is equal to the 'limit'. If it is not,
   compilation is aborted.

   @param type Type name
   @param limit Size limit
 */
#define ZB_ASSERT_TYPE_SIZE_EQ(type, limit) ZB_ASSERT_COMPILE_DECL(sizeof(type) == (limit))


/*! @} */

#endif /* ZB_DEBUG_H */
