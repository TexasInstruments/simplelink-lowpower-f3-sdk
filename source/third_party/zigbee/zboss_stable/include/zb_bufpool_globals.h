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
/* PURPOSE: Buffer pool globals declaration
*/

#ifndef ZB_BUFPOOL_GLOBALS_H
#define ZB_BUFPOOL_GLOBALS_H 1

/*! @cond internals_doc */
/*! \addtogroup ZB_BASE */
/*! @{ */

#include "zboss_api_buf_legacy.h"
#include "zb_bufpool.h"

#ifdef ZB_BIGBUF
#define ZB_BUF_SET_BUSY_BIG(param) ZB_ASSERT(!ZB_BUF_CHECK_BUSY_BIG(param));ZG->bpool.big_buf_in_use[(ZB_BIGBUF_ID_FROM_REF(param)) / 8U] |= (1U<<((ZB_BIGBUF_ID_FROM_REF(param)) % 8U))
#define ZB_BUF_CLEAR_BUSY_BIG(param) ZB_ASSERT(ZB_BUF_CHECK_BUSY_BIG(param)); ZG->bpool.big_buf_in_use[(ZB_BIGBUF_ID_FROM_REF(param)) / 8U] &= ~(1U<<((ZB_BIGBUF_ID_FROM_REF(param)) % 8U))
#endif


#ifdef ZB_BUF_SHIELD

#define ZB_BUF_HDR_SIGNATURE 0xbbU
#define ZB_BUF_TAIL_SIGNATURE 0xbeU

#ifdef ZB_BIGBUF

#define ZB_BUF_SET_BUSY_SM(param) ZB_ASSERT(!ZB_BUF_IS_BUSY(param)); ZG->bpool.buf_in_use[(param) / 8U] |= (1U<<((param) % 8U))
#define ZB_BUF_SET_BUSY(param) ((ZB_IS_BIGBUF(param))?ZB_BUF_SET_BUSY_BIG(param):ZB_BUF_SET_BUSY(param))

#define ZB_BUF_CLEAR_BUSY_SM(param) ZB_ASSERT(ZB_BUF_IS_BUSY(param)); ZG->bpool.buf_in_use[(param) / 8U] &= ~(1U<<((param) % 8U))
#define ZB_BUF_CLEAR_BUSY(param) ((ZB_IS_BIGBUF(param))?ZB_BUF_CLEAR_BUSY_BIG(param):ZB_BUF_CLEAR_BUSY(param))

#else

#define ZB_BUF_SET_BUSY(param) \
ZB_ASSERT(!ZB_BUF_IS_BUSY(param));\
ZG->bpool.buf_in_use[(param) / 8U] |= (1U<<((param) % 8U))
#define ZB_BUF_CLEAR_BUSY(param) \
ZB_ASSERT(ZB_BUF_IS_BUSY(param));\
ZG->bpool.buf_in_use[(param) / 8U] &= ~(1U<<((param) % 8U))

#endif

#else

#define ZB_BUF_SET_BUSY(param)

#define ZB_BUF_CLEAR_BUSY(param)

#endif




/**
  Buffers pool
 */
typedef struct zb_buf_pool_s
{
#ifdef ZB_USE_STD_MALLOC
  zb_buf_t *pool[ZB_IOBUF_POOL_SIZE];   /*!< Buffers pointers array*/
#else

#ifndef ZB_CONFIGURABLE_MEM
  zb_buf_ent_t pool[ZB_IOBUF_POOL_SIZE];    /*!< Buffers array (static memory configuration case)*/
#else
  zb_buf_ent_t *pool;               /*!< Pointer to buffers array allocated elsewhere. @see configurable_mem_internals  */
#endif

#ifdef ZB_LEGACY_BUFS
  zb_buf_t ZB_XDATA *head;                /*!< First free buffer*/
#endif

#endif /* else #ifdef ZB_USE_STD_MALLOC */
#ifdef ZB_BIGBUF
  zb_bigbuf_t bpool[ZB_BIGBUF_POOL_SIZE/*ZB_IOBUF_POOL_SIZE*/];
  zb_uint8_t bigbufs_allocated;
  //zb_uint8_t bigbufs_allocated[2];           /*!< Count used buffers for in/out packet */
#endif
  zb_uint_t  reserved;          /*!< reserved bytes at buf pool tail */
  zb_uint8_t bufs_allocated[2];           /*!< Count used buffers for in/out packet */
  zb_bool_t mac_rx_need_buf;              /*!< Flag 'buffer need for mac read cmd'.
                                              Mac read cmd is first in queue to buffer always' */

#ifndef ZB_CONFIGURABLE_MEM
#define ZB_BUF_POOL_BITMAP_SIZE ((ZB_IOBUF_POOL_SIZE + 7U) / 8U)
  zb_uint8_t busy_bufs_bitmap[ZB_BUF_POOL_BITMAP_SIZE]; /* Bitmap storing the state of each
                                                         *  buffer:
                                                         * '1' - currently allocated
                                                         * '0' - currently free
                                                         * It is different from buf_in_use since
                                                         * 1 allocated buffer may occupy the span
                                                         * of multiple actual buffers.
                                                         */
#else
  zb_uint8_t *busy_bufs_bitmap;
#endif

#ifdef ZB_BUF_SHIELD
#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t buf_in_use[(ZB_IOBUF_POOL_SIZE + 7)/8]; /*!< bitmap of allocated
                                                       buffers */
#else
  zb_uint8_t *buf_in_use;
#endif
#endif
} zb_buf_pool_t;


/*! @} */
/*! @endcond */

#endif /* ZB_BUFPOOL_GLOBALS_H */
