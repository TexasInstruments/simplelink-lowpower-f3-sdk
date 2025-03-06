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
/* PURPOSE: Tests and debug macros
*/
#ifndef ZB_DEBUG_H
#define ZB_DEBUG_H 1

/*! \addtogroup ZB_ASSERT_DYNAMIC */
/*! @{ */

#if (defined USE_ASSERT)
/**
   Trace current location, abort current program execution, with core dump if possible.

   @param caller_file - source file name
   @param caller_line - line in the source
 */
void zb_abort(char *caller_file, int caller_line);

#ifndef ZB_BINARY_TRACE
/** @cond internals_doc */
/**
   Assert: print diagnostic and force core dump

   @param file_name - source file name
   @param line_number - line in the source
*/
ZB_NORETURN void zb_assert(const zb_char_t *file_name, zb_int_t line_number);
/** @endcond */
/**
   Check for expression in runtime and call zb_assert() if it is false.

   Compiled to nothing if USE_ASSERT is not defined.

   @param expr expression to check
*/
#define ZB_ASSERT(expr) do {if(!(expr)){zb_assert(__FILE__, __LINE__);}} while (0)
/** @cond internals_doc */
#define ZB_INLINE_ASSERT_SIMPLE(expr)  ((expr) ? 1 : (zb_assert(__FILE__, __LINE__), 1))
/** @endcond */

#else  /* !ZB_BINARY_TRACE */
/**
   Assert: print diagnostic and force core dump

   @param file_id - source file id
   @param line_number - line in the source
*/
ZB_NORETURN void zb_assert(zb_uint16_t file_id, zb_int_t line_number);
/**
   Check for expression in runtime and call zb_assert() if it is false.

   Compiled to nothing if USE_ASSERT is not defined.

   @param expr expression to check
*/
#define ZB_ASSERT(expr) do {if(!(expr)) { zb_assert(ZB_TRACE_FILE_ID, __LINE__);} } while (0)
/** @cond internals_doc */
#define ZB_INLINE_ASSERT_SIMPLE(expr)  ((expr) ? 1 : (zb_assert(ZB_TRACE_FILE_ID, __LINE__), 1))
/** @endcond */

#endif  /* !ZB_BINARY_TRACE */

/** @cond internals_doc */
void lwip_zb_assert(zb_uint16_t file_id, zb_int_t line_number);
/** @endcond */

/**
   Assert which can be used inside an expression.

   @param expr - expression to check.
   @return always 1 (true)
 */
#define ZB_INLINE_ASSERT(expr)  ZB_INLINE_ASSERT_SIMPLE((expr)),

#else  /* release */

#define ZB_ASSERT(expr) ((void)(expr))
#define ZB_INLINE_ASSERT_SIMPLE(expr) -1
#define ZB_INLINE_ASSERT(expr)

#endif

/*! @} */

/** \addtogroup ZB_ASSERT_STATIC */
/** @{ */

/** @cond internals_doc */
/* Tricks to force preprocessor to substitute __LINE__ in macros */
#define ZB_ASSERT_CAT_(a, b) a##_##b
#define ZB_ASSERT_CAT(a, b) ZB_ASSERT_CAT_(a, b)
/** @endcond */

/**
   Check condition at compile time in the code (not declaration block)

   @param expr - expression to check.
 */
#if defined SDCC || defined KEIL
#define ZB_ASSERT_COMPILE_TIME(expr)
#else
#define ZB_ASSERT_COMPILE_TIME(expr) ((void)(zb_int_t (*)[(expr) ? 1 : -1])0)
#endif
/**
   Check condition at compile time in the code (not declaration block), return 0
   To be used inside an expressions.

   @param expr - expression to check.
 */
#define ZB_ASSERT_COMPILE_TIME_EXPR(expr) ((zb_int_t)((zb_int_t (*)[(expr) ? 1 : -1])0))

/**
   Check condition at compile time in the declaration block

   @param expr - expression to check.
 */
#ifdef __cplusplus
#define ZB_ASSERT_COMPILE_DECL(expr) __extension__ static_assert(expr, "Assert at line __LINE__")
#elif (defined __GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
#define ZB_ASSERT_COMPILE_DECL(expr) __extension__ _Static_assert(expr, "Assert at line __LINE__")
#elif defined(__COUNTER__)
#define ZB_ASSERT_COMPILE_DECL(expr) typedef zb_int_t ZB_ASSERT_CAT(assert, ZB_ASSERT_CAT(__LINE__, __COUNTER__))[(expr) ? 1 : -1]
#else
#define ZB_ASSERT_COMPILE_DECL(expr)
#endif


/**
   Ensures, that size of type `type' is not greater than `limit'.  If it is not,
   compilation is aborted.

   @param type - type name
   @param limit - size limit
 */
#define ZB_ASSERT_TYPE_SIZE_NOT_GREATER(type, limit) ZB_ASSERT_COMPILE_DECL(sizeof(type) <= (limit))

/**
   Ensures, that size of type `type' is not less than `limit'.  If it is not,
   compilation is aborted.

   @param type - type name
   @param limit - size limit
 */
#define ZB_ASSERT_TYPE_SIZE_NOT_LESS(type, limit) ZB_ASSERT_COMPILE_DECL(sizeof(type) >= (limit))

/**
   Ensures, that size of type `type' is equal to the `limit'.  If it is not,
   compilation is aborted.

   @param type - type name
   @param limit - size limit
 */
#define ZB_ASSERT_TYPE_SIZE_EQ(type, limit) ZB_ASSERT_COMPILE_DECL(sizeof(type) == (limit))

/*! @} */

#ifdef ZB_ARRAYS_CHECK
/**
   Address all array elements. Use this macro to test that array really inited
   by valgrind in Linux
 */
#define ZB_CHK_ARR(arr, len)                       \
  do                                               \
  {                                                \
  zb_ushort_t _ii;                                 \
  zb_ushort_t n = 0;                               \
  for (_ii = 0 ; _ii < (len) ; ++_ii)              \
  {                                                \
    if ((arr)[_ii] < 2)                            \
    {                                              \
      n++;                                         \
    }                                              \
    else                                           \
    {                                              \
      n--;                                         \
    }                                              \
  }                                                \
  }                                                \
while (0)

#else
#define ZB_CHK_ARR(arr, len)
#endif  /* ZB_ARRAYS_CHECK */

#if !defined ZB_NS_BUILD && defined ZB8051 && defined C8051F120
#define ZB_P3_ON() LED1 = 0
#define ZB_P3_OFF() LED1 = 1
#define ZB_P4_ON() LED2 = 0
#define ZB_P4_OFF() LED2 = 1
#else
#define ZB_P3_ON()
#define ZB_P3_OFF()
#define ZB_P4_ON()
#define ZB_P4_OFF()
#endif

#if defined(ZB_TRACE_LEVEL) && defined(ZB_TRACE_TRAFFIC)
/**
   Trace array of bytes

   @param buf - array of byte
   @param len - size of array
 */
void dump_traf(const zb_uint8_t *buf, zb_ushort_t len);
void dump_usb_traf(zb_uint8_t *buf, zb_ushort_t len);
void zb_debug_trace_buffer(zb_uint8_t buf_id, zb_uint16_t label);
void zb_debug_trace_nwk_header(const zb_uint8_t *payload_ptr, zb_uint16_t label);
#else
#define dump_traf(buf, len) {ZVUNUSED(buf); ZVUNUSED(len);}
#define dump_usb_traf(buf, len) {ZVUNUSED(buf); ZVUNUSED(len);}
#define zb_debug_trace_buffer(buf_id, label) {ZVUNUSED(buf_id); ZVUNUSED(label);}
#define zb_debug_trace_nwk_header(payload_ptr, label) {ZVUNUSED(payload_ptr); ZVUNUSED(label);}
#endif /* ZB_TRACE_LEVEL && ZB_TRACE_TRAFFIC */

#if (defined ZB_MAC_TESTING_MODE) && (defined ZB_TRACE_TRAFFIC)
#define DUMP_TRAF(cmt, buf, len, total) TRACE_MSG(TRACE_MAC3, #cmt, (FMT__0)); dump_traf(buf, len)
#else
#define DUMP_TRAF(comment, buf, len, total)
#endif /* ZB_MAC_TESTING_MODE && ZB_TRACE_TRAFFIC */

#ifdef DEBUG
void dump_hex_data(zb_uint_t trace_mask, zb_uint8_t trace_level, const zb_uint8_t *buf, zb_ushort_t len);

void trace_hex_data_func(const zb_uint8_t *ptr, zb_short_t size, zb_bool_t format);
#define trace_8hex_per_line(ptr, size) trace_hex_data_func((ptr), (size), ZB_FALSE)
#define trace_16hex_per_line(ptr, size) trace_hex_data_func((ptr), (size), ZB_TRUE)
#else
#define dump_hex_data(trace_mask, trace_level, buf, len)

#define trace_8hex_per_line(ptr, size)
#define trace_16hex_per_line(ptr, size)
#endif  /* DEBUG */

#endif /* ZB_DEBUG_H */
