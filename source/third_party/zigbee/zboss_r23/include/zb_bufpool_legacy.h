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
/* PURPOSE: Packet buffers pool - internal legacy API
*/

#ifndef ZB_BUFPOOL_LEGACY_H
#define ZB_BUFPOOL_LEGACY_H 1

#include "zb_config.h"
#include "zboss_api_core.h"

#if defined ZB_LEGACY_BUFS

/*! \addtogroup buf */
/*! @{ */

/**
 * Additional settings for case when we use std malloc's
 */

#ifdef ZB_USE_STD_MALLOC
void print_buffs_list(void); /* Only for debug */
#define ZB_USE_MEMSET_FOR_NEW_BUFS
#define ZB_USE_ALLOC_FOR_PAYLOAD_DATA
#endif

/* Only for debug */
#ifdef ZB_USE_STD_MALLOC
#define TRACE_BUFFERS_LIST() \
{ \
  TRACE_MSG(TRACE_COMMON1, "###############################################", (FMT__0)); \
  print_buffs_list(); \
  TRACE_MSG(TRACE_COMMON1, "###############################################", (FMT__0)); \
}
#else
#define TRACE_BUFFERS_LIST()
#endif

#ifdef ZB_USE_STD_MALLOC
#define ZB_MALLOC(size_, type_)  (type_ *)malloc(size_*sizeof(type_))
#define ZB_FREE(ptr_)            free(ptr_); ptr_=NULL;
#else
#define ZB_MALLOC(size_, type_)  NULL
#define ZB_FREE(ptr_)
#endif

//#define ZB_UNDEFINED_BUFFER (zb_uint8_t)(-1)


/* check if input(output) buffer available */
#define ZB_IN_BUF_AVAILABLE(reserve) \
  (ZG->bpool.bufs_allocated[1] < ZB_BUFS_LIMIT - ZB_BUFS_HI_PRIOR_RESERVE - (reserve))
#define ZB_OUT_BUF_AVAILABLE(reserve) \
    (ZG->bpool.bufs_allocated[0] < ZB_BUFS_LIMIT - ZB_BUFS_HI_PRIOR_RESERVE - (reserve))
#define ZB_OUT_N_BUFS_AVAILABLE(num) (ZB_BUFS_LIMIT - ZG->bpool.bufs_allocated[0] - ZB_BUFS_HI_PRIOR_RESERVE >= (num))

#ifdef ZB_BIGBUF
#define ZB_BUF_IS_BUSY_SM(param) (ZG->bpool.buf_in_use[(param) / 8U] & (1U<<(param % 8U)))
#define ZB_BUF_IS_BUSY_BIG(param) (ZG->bpool.big_buf_in_use[(ZB_BIGBUF_ID_FROM_REF(param)) / 8U] & (1U<<((ZB_BIGBUF_ID_FROM_REF(param)) % 8U)))
#define ZB_BUF_IS_BUSY(param) ((ZB_IS_BIGBUF(param))?ZB_BUF_CHECK_BUSY_BIG(param):ZB_BUF_CHECK_BUSY_SM(param))
#else
#define ZB_BUF_IS_BUSY(param) (ZG->bpool.buf_in_use[(param) / 8U] & (1U<<((param) % 8U)))
#endif

#define ZB_BUF_GET_FREE_SIZE(zbbuf) (zb_uint_t)(ZB_BUF_IS_BUSY_INLINE(ZB_REF_FROM_BUF(zbbuf)) ZB_IO_BUF_SIZE - zb_buf_len(zbbuf))

/**
   Return pointer to data stored in buffer by buffer reference.
 */
#define ZB_BUF_BEGIN_BY_REF(_ref) ZB_BUF_BEGIN(ZB_BUF_FROM_REF(_ref))


zb_uint8_t *zb_bpool_get_reserved_area(zb_uint_t *reserve_bytes_p);

void zb_bpool_put_reserve_back(void);

#ifdef ZB_DEBUG_BUFFERS_EXT
  void zb_trace_bufs_usage(void);
  #define ZB_TRACE_BUFS_USAGE() zb_trace_bufs_usage()
#else
  #define ZB_TRACE_BUFS_USAGE()
#endif


/**
   Get buffer from the buffers list.

   If no buffers available, does not block.

   @param is_in - ZB_TRUE if want IN buffer
   @param priority - 0 - low priority. Bigger number - higher priority.
   @return pointer to the buffer or NULL if no buffer available.

 */
zb_buf_t *zb_get_buf(
#ifdef ZB_DEBUG_BUFFERS
    zb_uint16_t from_file, zb_uint16_t from_line,
#endif
    zb_uint8_t is_in, zb_uint8_t priority);

#ifdef ZB_DEBUG_BUFFERS
#define ZB_GET_BUF(is_in, priority) zb_get_buf(ZB_TRACE_FILE_ID, __LINE__, is_in, priority)
#else
#define ZB_GET_BUF(is_in, priority) zb_get_buf(is_in, priority)
#endif

/**
   Get IN or OUT buffer from the buffers list, depending on buffers pool load

   If no buffers available, does not block.
   To be called from the main loop routine.

   @return pointer to the buffer.

   @snippet nwk_leave/zdo_start_zr.c zb_get_out_buf

 */
zb_buf_t *zb_get_any_buf(
#ifdef ZB_DEBUG_BUFFERS
    zb_uint16_t from_file, zb_uint16_t from_line
#else
    void
#endif
    );

#ifdef ZB_DEBUG_BUFFERS
#define ZB_GET_ANY_BUF() zb_get_any_buf(ZB_TRACE_FILE_ID, __LINE__)
#else
#define ZB_GET_ANY_BUF() zb_get_any_buf()
#endif

/** Calculate reference (index) of the buffer. */
#ifdef ZB_USE_STD_MALLOC
zb_uint8_t get_buf_ref(zb_buf_t *buf);
#endif

/**
 *  @brief Allocate in buffer, call a callback with extra user parameter parameters when the buffer is available.
 *  If buffer is available, schedules callback for execution immediately. If no buffers available now,
 *  schedule callback later, when buffer will be available.
 *  @param callback to call.
 *  @param user data - will be passed to callback
 * @return RET_OK or error code.
 *
 *  @code
 *      ZB_GET_IN_BUF_DELAYED2(new_buffer_allocated, user_data);
 *  @endcode
 */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_GET_IN_BUF_DELAYED2(callback, user_param) zb_get_buf_delayed_2param(ZB_TRACE_FILE_ID, __LINE__, callback, ZB_IN_BUFFER, ZB_TRUE, user_param)
#else
#define ZB_GET_IN_BUF_DELAYED2(callback, user_param) zb_get_buf_delayed_2param(callback, ZB_IN_BUFFER, ZB_TRUE, user_param)
#endif

void *zb_buf_initial_alloc_in(zb_buf_t *zbbuf, zb_uint8_t size);

/*! @} */

#if 0
/**
 * Print data from buffer by reference or by pointer
 */
/*  note: all that trace via macro does not work in case of binary trace! Indeed,
 * it works in Linux only. */
#define PRINT_BUF_DATA_BY_REF( _p )      DUMP_TRAF("ZB BUFFER DATA: ", ZB_BUF_FROM_REF(_p)->buf, ZB_IO_BUF_SIZE, 0)
#define PRINT_BUF_PAYLOAD_BY_REF( _p )   DUMP_TRAF("ZB BUFFER PAYLOAD: ", zb_buf_begin(ZB_BUF_FROM_REF(_p)), zb_buf_len(ZB_BUF_FROM_REF(_p)), 0)

#define PRINT_BUF_DATA_BY_PTR( _p )      DUMP_TRAF("ZB BUFFER DATA: ", _p->buf, ZB_IO_BUF_SIZE, 0)
#define PRINT_BUF_PAYLOAD_BY_PTR( _p )   DUMP_TRAF("ZB BUFFER PAYLOAD: ", zb_buf_begin(_p), zb_buf_len(_p), 0)
#endif

void zb_buf_oom_trace_leg(void);

zb_bool_t zb_buf_memory_low_leg(void);
#ifdef ZB_REDUCE_NWK_LOAD_ON_LOW_MEMORY
zb_bool_t zb_buf_memory_close_to_low_leg(void);
#endif

#endif  /* ZB_LEGACY_BUFS */

#endif /* ZB_BUFPOOL_LEGACY_H */
