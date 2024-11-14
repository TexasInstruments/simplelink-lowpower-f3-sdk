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
/* PURPOSE: Declare ring buffer internals
*/

#ifndef ZB_RINGBUFFER_H
#define ZB_RINGBUFFER_H 1

/*! \cond internals_doc */
/**
   @addtogroup ZB_BASE
   @{
*/


/**
   \par Generic ring buffer macros
 */

/**
   Declares ring buffer for entries of given type and capacity.
   This is typedef, not variable declaration.

   @param type_name_prefix Prefix for names (like xxx_s, xxx_t)
   @param ent_type Type of the ring buffer entry
   @param capacity Ring buffer capacity
 */
#define ZB_RING_BUFFER_DECLARE(type_name_prefix, ent_type, capacity)  \
typedef struct type_name_prefix ## _s                                 \
{                                                                     \
  ent_type       ring_buf[capacity];                                   \
  zb_ushort_t    read_i;                                               \
  zb_ushort_t    write_i;                                              \
  zb_ushort_t    written;                                              \
} type_name_prefix ## _t

/**
 * Initializes the ring buffer internals.
 */
#define ZB_RING_BUFFER_INIT(rb) ( (rb)->read_i = (rb)->write_i = (rb)->written = 0)

/**
 * Returns the ring buffer capacity.
 *
 * @param rb Ring buffer pointer
 */
#define ZB_RING_BUFFER_CAPACITY(rb) ((sizeof((rb)->ring_buf) / sizeof((rb)->ring_buf[0])))

/**
 * Returns 1 if the ring buffer is full.
 *
 * @param rb Ring buffer pointer
 */
#define ZB_RING_BUFFER_IS_FULL(rb) ((zb_uint_t)(rb)->written >= ZB_RING_BUFFER_CAPACITY(rb))

/**
 * Returns 1 if the ring buffer is empty.
 *
 * @param rb Ring buffer pointer
 */
#define ZB_RING_BUFFER_IS_EMPTY(rb) ((rb)->written == 0)


/**
 * Returns free space available in the ring buffer.
 *
 * @param rb Ring buffer pointer
 */
#define ZB_RING_BUFFER_FREE_SPACE(rb) (ZB_RING_BUFFER_CAPACITY(rb) - (zb_uint_t)(rb)->written)

/**
 * Reserves slot in the ring buffer but do not update pointers.
 *
 * @param rb Ring buffer pointer
 * @return Pointer to the ring buffer entry or NULL if ring buffer is full
 */
#define ZB_RING_BUFFER_PUT_RESERVE(rb)          \
(                                               \
  ZB_RING_BUFFER_IS_FULL(rb) ? NULL             \
  : (rb)->ring_buf + (rb)->write_i              \
  )


/**
 * Puts to the ring buffer.
 * Gets free slot from the ring buffer, returns pointer to it.
 *
 * @param rb Ring buffer pointer
 * @return Nothing
 */
#define ZB_RING_BUFFER_FLUSH_PUT(rb)                                    \
(                                                                       \
  (rb)->written++,                                                      \
  (rb)->write_i = ((rb)->write_i + 1) % ZB_RING_BUFFER_CAPACITY(rb)     \
  )


/**
   Returns amount of data which can be put into ring buffer tail starting from write_i.

   @param rb Ring buffer pointer
   @param size Requested data size
 */
#define ZB_RING_BUFFER_LINEAR_PORTION(rb, size)         \
(                                                       \
  ZB_RING_BUFFER_CAPACITY(rb) - (rb)->write_i < size ?  \
  ZB_RING_BUFFER_CAPACITY(rb) - (rb)->write_i : size                    \
)

/**
   Batches put data into the ring buffer.

   To be used to copy from external buffer to the ring buffer.

   @param rb Ring buffer pointer
   @param data Data ptr
   @param size Requested data size
   @return Amount of data to be put
 */
#define ZB_RING_BUFFER_BATCH_PUT(rb, data, size)                \
  (                                                             \
                                                                \
    ZB_MEMCPY((rb)->ring_buf + (rb)->write_i, data,             \
              ZB_RING_BUFFER_LINEAR_PORTION(rb, size)),         \
    (rb)->written += ZB_RING_BUFFER_LINEAR_PORTION(rb, size),  \
    (rb)->write_i = ((rb)->write_i + ZB_RING_BUFFER_LINEAR_PORTION(rb, size)) % ZB_RING_BUFFER_CAPACITY(rb), \
    ZB_RING_BUFFER_LINEAR_PORTION(rb, size)                     \
)


/**
 * Flushes after putting more then 1 element to the ring buffer.
 *
 * Does not check for write_i overflow!
 *
 * @param rb Ring buffer pointer
 * @param size Number of items put
 * @return Nothing
 */
#define ZB_RING_BUFFER_FLUSH_BATCH_PUT(rb, size)                        \
(                                                                       \
  (rb)->written += size,                                                \
  (rb)->write_i += size                                                 \
  )


/**
 * Puts value to the ring buffer.
 *
 * @param rb Ring buffer pointer
 * @param value Value to put to the ring buffer
 * @return Nothing
 */
#define ZB_RING_BUFFER_PUT(rb, value)                                   \
(                                                                       \
  (rb)->ring_buf[(rb)->write_i] = (value),                              \
  (rb)->written++,                                                      \
  (rb)->write_i = ((rb)->write_i + 1) % ZB_RING_BUFFER_CAPACITY(rb)     \
  )


/**
 * Puts value to the ring buffer using memcpy.
 *
 * @param rb Ring buffer pointer
 * @param value_ptr Pointer to value to put to the ring buffer
 * @return Nothing
 */
#define ZB_RING_BUFFER_PUT_PTR(rb, value_ptr)                           \
(                                                                       \
  ZB_MEMCPY(&((rb)->ring_buf[(rb)->write_i]), (value_ptr), sizeof((rb)->ring_buf[0])), \
  (rb)->written++,                                                      \
  (rb)->write_i = ((rb)->write_i + 1) % ZB_RING_BUFFER_CAPACITY(rb)     \
  )


/**
 * Reinserts last value into the ring buffer using memcpy.
 *
 * @param rb Ring buffer pointer
 * @param value_ptr Pointer to value to put to the ring buffer
 * @return Nothing
 */
#define ZB_RING_BUFFER_PUT_REUSE_LAST(rb, value_ptr)                    \
(                                                                       \
  ZB_MEMCPY(&((rb)->ring_buf[((rb)->write_i ? (rb)->write_i - 1 : (rb)->write_i + ZB_RING_BUFFER_CAPACITY(rb) - 1)]), \
           (value_ptr),                                                 \
           sizeof((rb)->ring_buf[0]))																		\
	)


/**
 * Gets entry from the ring buffer read pointer position.
 *
 * @param rb Ring buffer pointer
 *
 * @return Pointer to the ring buffer entry or NULL if it is empty
 */
#define ZB_RING_BUFFER_PEEK(rb)                 \
(                                               \
  ZB_RING_BUFFER_IS_EMPTY(rb) ? NULL            \
  : (rb)->ring_buf + (rb)->read_i               \
  )

/**
 * Gets entry from the ring buffer read pointer position.
 *
 * @param rb Ring buffer pointer
 *
 * @return Pointer to the ring buffer entry
 */
#define ZB_RING_BUFFER_GET(rb)                 \
(                                               \
  (rb)->ring_buf + (rb)->read_i               \
  )

/**
 * Moves ring buffer read pointer.
 *
 * To be used after ZB_RING_BUFFER_PEEK().
 * Note that this macro does not check for empty ring buffer.
 *
 * @param rb Ring buffer pointer
 * @return Nothing
 */
#define ZB_RING_BUFFER_FLUSH_GET(rb)                                    \
(                                                                       \
  (rb)->written--,                                                      \
  ((rb)->read_i = ((rb)->read_i + 1) % ZB_RING_BUFFER_CAPACITY(rb))     \
  )


/**
 * Gets entries from the ring buffer read pointer position which can be got at once.
 *
 * @param rb Ring buffer pointer
 * @param size (Out) number of entries which can be got
 *
 * @return Pointer to the ring buffer entry
 */
#define ZB_RING_BUFFER_GET_BATCH(rb, size)                              \
  (                                                                     \
    size = ((rb)->written <= (ZB_RING_BUFFER_CAPACITY(rb) - (rb)->read_i) \
            ?                                                           \
            (rb)->written                                               \
            :                                                           \
            (ZB_RING_BUFFER_CAPACITY(rb) - (rb)->read_i)),              \
    (rb)->ring_buf + (rb)->read_i                                       \
  )                                                                     \



/**
 * Moves ring buffer read pointer for more than 1 element.
 *
 * Note that this macro does not check for empty ring buffer.
 *
 * @param rb Ring buffer pointer
 * @param size Number of elements to be marked as read
 * @return Nothing
 */
#define ZB_RING_BUFFER_FLUSH_GET_BATCH(rb, size)                        \
(                                                                       \
  (rb)->written -= size,                                                      \
  ((rb)->read_i = ((rb)->read_i + size) % ZB_RING_BUFFER_CAPACITY(rb))     \
  )


/**
 * Returns number of used entries.
 *
 * @param rb Ring buffer pointer
 */

#define ZB_RING_BUFFER_USED_SPACE(rb)                                   \
(                                                                       \
  (rb)->written                                                         \
  )

/* Antropov 19/11/13 CR:  begin */
#define ZB_RING_BUFFER_SEARCH_GET(_rb, _i)                      \
(                                                               \
  ZB_RING_BUFFER_IS_EMPTY(_rb) ? NULL                           \
  : (_rb)->ring_buf + ((_rb)->read_i +(_i)) % ZB_RING_BUFFER_CAPACITY(_rb)      \
  )
/* Antropov 19/11/13 CR:  end */

/**
 * @see ZB_RING_BUFFER_IS_EMPTY
 */
#define ZB_BYTE_ARRAY_IS_EMPTY(rb) ZB_RING_BUFFER_IS_EMPTY(rb)

/**
 * @see ZB_RING_BUFFER_PEEK
 */
#define ZB_BYTE_ARRAY_PEEK(rb) ZB_RING_BUFFER_PEEK(rb)

/**
 * Returns 1 if ring buffer is full.
 *
 * @param rb Ring buffer pointer
 * @param cap Ring buffer capacity (number of bytes)
 */
#define ZB_BYTE_ARRAY_IS_FULL(rb, cap) ((zb_uint_t)(rb)->written >= cap)

/**
 * Moves ring buffer read pointer.
 *
 * To be used after ZB_BYTE_ARRAY_PEEK().
 * Note that this macro does not check for empty ring buffer.
 *
 * @param rb Ring buffer pointer
 * @param cap Ring buffer capacity (number of bytes)
 * @return Nothing
 */
#define ZB_BYTE_ARRAY_FLUSH_GET(rb, cap)      \
(                                            \
  (rb)->written--,                           \
  ((rb)->read_i = ((rb)->read_i + 1) % cap)  \
  )

/**
 * Puts value to the ring buffer.
 *
 * @param rb Ring buffer pointer
 * @param value Value to put to the ring buffer
 * @param cap Ring buffer capacity (number of bytes)
 * @return Nothing
 */
#define ZB_BYTE_ARRAY_PUT(rb, value, cap)       \
(                                              \
  (rb)->ring_buf[(rb)->write_i] = (value),     \
  (rb)->written++,                             \
  (rb)->write_i = ((rb)->write_i + 1) % cap    \
  )

/**
 * Gets entries from the ring buffer read pointer position which can be get at once
 *
 * @param rb Ring buffer pointer.
 * @param size (out) number of entries which can be got
 * @param cap Ring buffer capacity (number of bytes)
 *
 * @return Pointer to the ring buffer entry
 */
#define ZB_BYTE_ARRAY_GET_BATCH(rb, size, cap)                              \
  (                                                                     \
    size = ((rb)->written <= (cap - (rb)->read_i)  \
            ?                                                           \
            (rb)->written                                               \
            :                                                           \
            (cap - (rb)->read_i)),                 \
    (rb)->ring_buf + (rb)->read_i                                       \
  )

/**
 * Moves ring buffer read pointer for more than 1 element.
 *
 * This macro does not check for an empty ring buffer.
 *
 * @param rb Ring buffer pointer.
 * @param size Number of elements to mark as read
 * @param cap Ring buffer capacity (number of bytes)
 * @return Nothing
 */
#define ZB_BYTE_ARRAY_FLUSH_GET_BATCH(rb, size, cap)                        \
(                                                                       \
  (rb)->written -= size,                                                      \
    ((rb)->read_i = ((rb)->read_i + size) % cap)     \
  )

/*! @} */
/*! \endcond */

#endif /* ZB_RINGBUFFER_H */
