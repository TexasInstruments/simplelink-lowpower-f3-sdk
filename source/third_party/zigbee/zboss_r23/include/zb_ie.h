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
/*  PURPOSE: Information Elements defined by 802.15.4-2015 (section 7.4)
*/

#ifndef ZB_IE_H
#define ZB_IE_H 1

/**
 * @name Possible values of element_id parameter of header IE
 * @anchor hie_element_id
 * @brief The list is far from complete, add here items as needed.
 */
/** @{ */
#define ZB_HIE_ELEMENT_VENDOR_SPECIFIC 0x00U
#define ZB_HIE_ELEMENT_HT1             0x7EU
#define ZB_HIE_ELEMENT_HT2             0x7FU
/** @} */

/**
 * @brief Type for possible values of element_id parameter of header IE.
 *
 * Holds one of @ref hie_element_id. Kept only for backward compatibility as
 * @ref hie_element_id were declared previously as enum. Can be removed in future releases.
 */
typedef zb_uint8_t zb_hie_element_id_t;

/**
 * @name Possible values of group_id parameter of payload IE
 * @anchor pie_group_id
 * @brief The list is complete as of 802.15.4-2015
 */
/** @{ */
#define ZB_PIE_GROUP_ESDU            0x00U
#define ZB_PIE_GROUP_MLME            0x01U
#define ZB_PIE_GROUP_VENDOR_SPECIFIC 0x02U
/* 0x03 - 0x0e:  reserved */
#define ZB_PIE_GROUP_TERMINATION     0x0FU
/** @} */

/**
 * @brief Type for possible values of element_id parameter of header IE.
 *
 * Holds one of @ref pie_group_id. Kept only for backward compatibility as
 * @ref pie_group_id were declared previously as enum. Can be removed in future releases.
 */
typedef zb_uint8_t zb_pie_group_id_t;

/*
 * Possible values of sub_id parameter of nested IE header
 * The list is far from complete, add here items as needed.
 */
typedef zb_uint8_t zb_nie_sub_id_t;
#define ZB_NIE_SUB_ID_EB_FILTER (0x1EU)

/*
 * Possible values of sub_id parameter of Zigbee payload IE header
 * The list is complete as of r22
 */
typedef zb_uint8_t zb_zigbee_pie_sub_id_t;

#define ZB_ZIGBEE_PIE_SUB_ID_REJOIN     (0x00U)
#define ZB_ZIGBEE_PIE_SUB_ID_TX_POWER   (0x01U)
#define ZB_ZIGBEE_PIE_SUB_ID_EB_PAYLOAD (0x02U)

/* These structures are intended for use while parsing IEs */

typedef struct zb_hie_header_s
{
  zb_uint8_t length;
  zb_hie_element_id_t element_id;
}
zb_hie_header_t;

typedef struct zb_pie_header_s
{
  zb_uint16_t length;
  zb_pie_group_id_t group_id;
}
zb_pie_header_t;

typedef struct zb_nie_header_s
{
  zb_uint16_t length;
  zb_nie_sub_id_t sub_id;
}
zb_nie_header_t;

typedef struct zb_zigbee_pie_header_s
{
  zb_uint8_t length;
  zb_zigbee_pie_sub_id_t sub_id;
}
zb_zigbee_pie_header_t;

/* Length definitions */

#define ZB_HIE_HEADER_LENGTH 2U
#define ZB_PIE_HEADER_LENGTH 2U
#define ZB_NIE_HEADER_LENGTH 2U
#define ZB_ZIGBEE_PIE_HEADER_LENGTH 2U

/* Setters */

/**
 * @brief Write HIE header into the specified memory location
 * The header has the following format (type = 0):
 * length (bits 0-6) | element_id (bits 7-14) | type (bits 15)
 *
 * @param ptr - pointer to memory location
 * @param element_id
 * @param length
 */

#define ZB_SET_HIE_HEADER(ptr, element_id, length)                                                 \
  {                                                                                                \
    (ptr)[ZB_PKT_16B_ZERO_BYTE] = ((length) & 0x7FU) | (((element_id) & 0x01U) << 7);              \
    (ptr)[ZB_PKT_16B_FIRST_BYTE] = ((element_id) >> 1) & 0x7FU;                                    \
  }

/**
 * @brief Call ZB_SET_HIE_HEADER and increment ptr by HIE header length
 * @see ZB_SET_HIE_HEADER
 */
#define ZB_SET_NEXT_HIE_HEADER(ptr, element_id, length)	\
{							\
  ZB_SET_HIE_HEADER((ptr), (element_id), (length));	\
  (ptr) += ZB_HIE_HEADER_LENGTH;			\
}

/**
 * @brief Write PIE header into the specified memory location
 * The header has the following format (type = 1):
 * length (bits 0-10) | group_id (bits 11-14) | type (bits 15)
 *
 * @param ptr - pointer to memory location
 * @param group_id
 * @param length
 */
#define ZB_SET_PIE_HEADER(ptr, group_id, length)                                                     \
  {                                                                                                  \
    (ptr)[ZB_PKT_16B_ZERO_BYTE] = (length) & 0xFFU;                                                  \
    (ptr)[ZB_PKT_16B_FIRST_BYTE] = (((length) >> 8U) & 0x7U) | (((group_id) & 0x0FU) << 3U) | 0x80U;   \
  }

/**
 * @brief Call ZB_SET_PIE_HEADER and increment ptr by PIE header length
 * @see ZB_SET_PIE_HEADER
 */
#define ZB_SET_NEXT_PIE_HEADER(ptr, group_id, length)                                              \
  {                                                                                                \
    ZB_SET_PIE_HEADER((ptr), (group_id), (length));                                                \
    (ptr) += ZB_PIE_HEADER_LENGTH;                                                                 \
  }

/**
 * @brief Write short NIE header into the specified memory location
 * The header has the following format (type = 0):
 * length (bits 0-7) | sub_id (bits 8-14) | type (bits 15)
 *
 * @param ptr - pointer to memory location
 * @param sub_id
 * @param length
 */
#define ZB_SET_SHORT_NIE_HEADER(ptr, sub_id, length)		\
{								\
  (ptr)[ZB_PKT_16B_ZERO_BYTE] = (length) & 0xFFU;		\
  (ptr)[ZB_PKT_16B_FIRST_BYTE] = (sub_id) & 0x7FU;		\
}

/**
 * @brief Call ZB_SET_SHORT_NIE_HEADER and increment ptr by
 *  short NIE header length
 * @see ZB_SET_SHORT_NIE_HEADER
 */
#define ZB_SET_NEXT_SHORT_NIE_HEADER(ptr, group_id, length)	\
{							\
  ZB_SET_SHORT_NIE_HEADER((ptr), (group_id), (length));	\
  (ptr) += ZB_NIE_HEADER_LENGTH;			\
}

/**
 * @brief Write long NIE header into the specified memory location
 * The header has the following format (type = 1):
 * length (bits 0-10) | sub_id (bits 11-14) | type (bits 15)
 *
 * @param ptr - pointer to memory location
 * @param sub_id
 * @param length
 */
#define ZB_SET_LONG_NIE_HEADER(ptr, sub_id, length)		\
{						 		\
 (ptr)[ZB_PKT_16B_ZERO_BYTE] = (length) & 0xFF;			\
 (ptr)[ZB_PKT_16B_FIRST_BYTE] = (((length) >> 8) & 0x7)		\
   | (((sub_id) & 0xF) << 3) | 0x80;				\
}

/**
 * @brief Call ZB_SET_LONG_NIE_HEADER and increment ptr by
 *  long NIE header length
 * @see ZB_SET_LONG_NIE_HEADER
 */
#define ZB_SET_NEXT_LONG_NIE_HEADER(ptr, group_id, length)	\
{							\
  ZB_SET_LONG_NIE_HEADER((ptr), (group_id), (length));	\
  (ptr) += ZB_NIE_HEADER_LENGTH;			\
}

/**
 * @brief Write Zigbee Payload Nested IE header into the
 * specified memory location.
 * The header has the following format:
 * length (bits 0-5) | sub_id (bits 6-15)
 *
 * @param ptr - pointer to memory location
 * @param sub_id
 * @param length
 */
#define ZB_SET_ZIGBEE_PIE_HEADER(ptr, sub_id, length)			\
{									\
  (ptr)[ZB_PKT_16B_ZERO_BYTE] = ((length) & 0x3FU)			\
    | (((sub_id) & 0x3U) << 6);						\
  (ptr)[ZB_PKT_16B_FIRST_BYTE] = ((sub_id) >> 2) & 0xFFU;		\
}

/**
 * @brief Call ZB_SET_ZIGBEE_PIE_HEADER and increment ptr by
 *  Zigbee PIE header length
 * @see ZB_SET_ZIGBEE_PIE_HEADER
 */
#define ZB_SET_NEXT_ZIGBEE_PIE_HEADER(ptr, sub_id, length)	\
{								\
  ZB_SET_ZIGBEE_PIE_HEADER((ptr), (sub_id), (length));		\
  (ptr) += ZB_ZIGBEE_PIE_HEADER_LENGTH;				\
}

/* Getters */

#define ZB_IE_HEADER_GET_TYPE(ptr) (((ptr)[ZB_PKT_16B_FIRST_BYTE] & 0x80U) >> 7U)
#define ZB_IE_HEADER_TYPE_HIE 0U
#define ZB_IE_HEADER_TYPE_PIE 1U

/*
 * HIE header structure:
 * bits 0-6 (length) | bits 7-14 (element_id) | bit 15 (type)
 * type = 0
 **/
#define ZB_HIE_GET_LENGTH(ptr) ((ptr)[ZB_PKT_16B_ZERO_BYTE] & 0x7FU)
#define ZB_HIE_GET_ELEMENT_ID(ptr) \
  ((zb_hie_element_id_t)(((ptr)[ZB_PKT_16B_ZERO_BYTE] >> 7U) | ((ptr)[ZB_PKT_16B_FIRST_BYTE] << 1U)))

/**
 * @brief Read HIE header located at ptr address into zb_hie_header_t structure
 * @param ptr - pointer to HIE location
 * @param hie_hdr_ptr - pointer to zb_hie_header_t structure
 */
#define ZB_GET_HIE_HEADER(ptr, hie_hdr_ptr)				\
  (hie_hdr_ptr)->length = ZB_HIE_GET_LENGTH(ptr),		\
  (hie_hdr_ptr)->element_id = ZB_HIE_GET_ELEMENT_ID(ptr)

/**
 * @brief Call ZB_GET_HIE_HEADER and increment ptr by
 *  HIE header length
 * @see ZB_GET_HIE_HEADER
 */
#define ZB_GET_NEXT_HIE_HEADER(ptr, hie_hdr_ptr)		\
{								\
  ZB_GET_HIE_HEADER((ptr), (hie_hdr_ptr));			\
  (ptr) += ZB_HIE_HEADER_LENGTH;				\
}

/*
 * PIE header structure:
 * bits 0-10 (length) | bits 11-14 (group_id) | bit 15 (type)
 * type = 1
 */

#define ZB_PAYLOAD_PIE_HEADER_GET_LENGTH(ptr) \
  ((zb_uint16_t)(ptr)[ZB_PKT_16B_ZERO_BYTE] | ((((zb_uint16_t)(ptr)[ZB_PKT_16B_FIRST_BYTE] & 0x7U)) << 8U))
#define ZB_PAYLOAD_PIE_HEADER_GET_GROUP_ID(ptr) (((ptr)[ZB_PKT_16B_FIRST_BYTE] & 0x78U) >> 3U)

/**
 * @brief Read PIE header located at ptr address into zb_pie_header_t structure
 * @param ptr - pointer to PIE location
 * @param pie_hdr_ptr - pointer to zb_pie_header_t structure
 */
#define ZB_GET_PIE_HEADER(ptr, pie_hdr_ptr)			\
  (pie_hdr_ptr)->length = ZB_PAYLOAD_PIE_HEADER_GET_LENGTH(ptr),	\
  (pie_hdr_ptr)->group_id = (zb_pie_group_id_t)ZB_PAYLOAD_PIE_HEADER_GET_GROUP_ID(ptr)

/**
 * @brief Call ZB_GET_PIE_HEADER and increment ptr by
 *  PIE header length
 * @see ZB_GET_PIE_HEADER
 */
#define ZB_GET_NEXT_PIE_HEADER(ptr, pie_hdr_ptr)		\
{								\
  ZB_GET_PIE_HEADER((ptr), (pie_hdr_ptr));			\
  (ptr) += ZB_PIE_HEADER_LENGTH;				\
}

/**
 * @brief Determine the type of Nested IE header that starts at ptr
 * @param ptr - location of Nested IE header
 * @return ZB_NIE_HEADER_TYPE_LONG or ZB_NIE_HEADER_TYPE_SHORT
 **/
#define ZB_GET_NIE_HEADER_TYPE(ptr) (((ptr)[ZB_PKT_16B_FIRST_BYTE] & 0x80U) >> 7U)
#define ZB_NIE_HEADER_TYPE_LONG 1U
#define ZB_NIE_HEADER_TYPE_SHORT 0U

/*
 * Nested IE short header structure:
 * bits 0-7 (length) | bits  8-14 (sub_id) | bit 15 (type)
 * type = 0 for short
 */

#define ZB_NIE_SHORT_HEADER_GET_LENGTH(ptr) ((zb_uint16_t)(ptr)[ZB_PKT_16B_ZERO_BYTE])
#define ZB_NIE_SHORT_HEADER_GET_SUB_ID(ptr)  ((zb_nie_sub_id_t)((ptr)[ZB_PKT_16B_FIRST_BYTE] & 0x7FU))

/*
 * Nested IE long header structure:
 * bits 0-10 (length) | bits 11-14 (sub_id) | bit 15 (type, equals 1)
 */

#define ZB_NIE_LONG_HEADER_GET_LENGTH(ptr) \
  ((zb_uint16_t)(ptr)[ZB_PKT_16B_ZERO_BYTE] | (((zb_uint16_t)(ptr)[ZB_PKT_16B_FIRST_BYTE] & 0x7U) << 8U))
#define ZB_NIE_LONG_HEADER_GET_SUB_ID(ptr) \
  ((zb_nie_sub_id_t)(((ptr)[ZB_PKT_16B_FIRST_BYTE] & 0x78U) >> 3U))


/**
 * @brief Read NIE header located at ptr address into zb_nie_header_short_t structure
 * This macro handles cases of long and short automatically.
 *
 * @param ptr - pointer to NIE location
 * @param nie_hdr_ptr - pointer to zb_nie_header_short_t structure
 */
#define ZB_GET_NIE_HEADER(ptr, nie_hdr_ptr)			\
{								\
  if (ZB_GET_NIE_HEADER_TYPE(ptr) == ZB_NIE_HEADER_TYPE_LONG)	\
  {								\
    (nie_hdr_ptr)->length = ZB_NIE_LONG_HEADER_GET_LENGTH(ptr);	\
    (nie_hdr_ptr)->sub_id = ZB_NIE_LONG_HEADER_GET_SUB_ID(ptr);	\
  }								\
  else								\
  {								\
    (nie_hdr_ptr)->length = ZB_NIE_SHORT_HEADER_GET_LENGTH(ptr); \
    (nie_hdr_ptr)->sub_id = ZB_NIE_SHORT_HEADER_GET_SUB_ID(ptr); \
  }								\
}

/**
 * @brief Call ZB_GET_NIE_HEADER and increment ptr by
 *  NIE header length
 * @see ZB_GET_PIE_HEADER
 */
#define ZB_GET_NEXT_NIE_HEADER(ptr, nie_hdr_ptr)		\
{								\
  ZB_GET_NIE_HEADER((ptr), (nie_hdr_ptr));			\
  (ptr) += ZB_NIE_HEADER_LENGTH;				\
}

/*
 * Zigbee Payload IE header structure:
 * bits 0-5 (length) |  bits 6-15 bit (sub_id)
 */

#define ZB_ZIGBEE_PIE_HEADER_GET_LENGTH(ptr) ((ptr)[ZB_PKT_16B_ZERO_BYTE] & 0x3FU)
#define ZB_ZIGBEE_PIE_HEADER_GET_SUB_ID(ptr) \
  ((zb_zigbee_pie_sub_id_t)(((ptr)[ZB_PKT_16B_ZERO_BYTE] >> 6U) | ((ptr)[ZB_PKT_16B_FIRST_BYTE] << 8U)))

/**
 * @brief Read Zigbee PIE header located at ptr address into zb_zigbee_pie_header_t structure
 * @param ptr - pointer to Zigbee PIE location
 * @param hdr_ptr - pointer to zb_zigbee_pie_header_t structure
 */
#define ZB_GET_ZIGBEE_PIE_HEADER(ptr, hdr_ptr)			                       \
{									                                           \
  (hdr_ptr)->length = ZB_ZIGBEE_PIE_HEADER_GET_LENGTH(ptr);		               \
  (hdr_ptr)->sub_id = ZB_ZIGBEE_PIE_HEADER_GET_SUB_ID(ptr); \
}

/**
 * @brief Call ZB_GET_ZIGBEE_PIE_HEADER and increment ptr by
 *  Zigbee PIE header length
 * @see ZB_GET_ZIGBEE_PIE_HEADER
 */
#define ZB_GET_NEXT_ZIGBEE_PIE_HEADER(ptr, hdr_ptr)			\
{								\
  ZB_GET_ZIGBEE_PIE_HEADER((ptr), (hdr_ptr));			\
  (ptr) += ZB_ZIGBEE_PIE_HEADER_LENGTH;				\
}

/* Specific IE functions, add more as needed */

#define ZB_PIE_VENDOR_OUI_LENGTH 3U
#define ZB_PIE_VENDOR_HEADER_LENGTH (ZB_PIE_HEADER_LENGTH + ZB_PIE_VENDOR_OUI_LENGTH)

/**
 * @brief Write Zigbee PIE Vendor Specific header at ptr address
 * Inserts corresponding PIE header and Zigbee OUI:
 * ZB_PIE_VENDOR_HEADER_LENGTH bytes in total
 *
 * @param ptr - location to write into
 * @param len - length of additional payload
 */
#define ZB_SET_PIE_ZIGBEE_VENDOR_HEADER(ptr, len)			\
{									\
  ZB_SET_PIE_HEADER((ptr), ZB_PIE_GROUP_VENDOR_SPECIFIC, (len) + 3U);	\
  (ptr)[ZB_PIE_HEADER_LENGTH] = 0x1BU;					\
  (ptr)[ZB_PIE_HEADER_LENGTH + 1U] = 0x19U;				\
  (ptr)[ZB_PIE_HEADER_LENGTH + 2U] = 0x4AU;				\
}

/**
 * @brief Call ZB_SET_PIE_ZIGBEE_VENDOR_HEADER and increment ptr by
 *  PIE Zigbee vendor header length
 * @see ZB_SET_PIE_ZIGBEE_VENDOR_HEADER
 */
#define ZB_SET_NEXT_PIE_ZIGBEE_VENDOR_HEADER(ptr, len)			\
{									\
  ZB_SET_PIE_ZIGBEE_VENDOR_HEADER((ptr), (len));				\
  (ptr) += ZB_PIE_VENDOR_HEADER_LENGTH;					\
}

#define ZB_IE_CHECK_ZIGBEE_VENDOR(ptr)			\
  ((ptr)[0] == 0x1BU && (ptr)[1] == 0x19U && (ptr)[2] == 0x4AU)

#define ZB_TX_POWER_IE_DESCRIPTOR_LEN (ZB_ZIGBEE_PIE_HEADER_LENGTH + 1U)

/**
 * @brief Write Sub-IE TX Power descriptor at ptr address
 * Inserts corresponding Zigbee PIE header and power payload:
 * ZB_TX_POWER_IE_DESCRIPTOR_LEN bytes in total
 *
 * @param ptr - location to write into
 * @param len - length of additional payload
 */
#define ZB_SET_TX_POWER_IE_DESCRIPTOR(ptr, tx_power)			\
{									\
  ZB_SET_ZIGBEE_PIE_HEADER(ptr, ZB_ZIGBEE_PIE_SUB_ID_TX_POWER, 1U);	\
  (ptr)[ZB_ZIGBEE_PIE_HEADER_LENGTH] = (tx_power) & 0xFFU;		\
}

/**
 * @brief Call ZB_SET_TX_POWER_IE_DESCRIPTOR and increment ptr by
 *  Sub-IE TX Power descriptor length
 * @see ZB_SET_TX_POWER_IE_DESCRIPTOR
 */
#define ZB_SET_NEXT_TX_POWER_IE_DESCRIPTOR(ptr, tx_power)		\
{									\
  ZB_SET_TX_POWER_IE_DESCRIPTOR((ptr), (tx_power));			\
  (ptr) += ZB_TX_POWER_IE_DESCRIPTOR_LEN;				\
}

/* EB Filter IE flags, see 7.4.4.6 of 802.15.4-2015 */
#define ZB_EB_FILTER_IE_PERMIT_JOINING_ON       0x1U
#define ZB_EB_FILTER_IE_INCLUDE_LQI             0x2U
#define ZB_EB_FILTER_IE_INCLUDE_PERCENT_FILTER  0x4U

#define ZB_EB_FILTER_HAS_FLAG(mask, flag) (((mask) & (flag)) > 0U)

/**
 * @brief Checks if HIE with HT1 element id is located at ptr address
 *
 * @param ptr - pointer to PIE location
 * @return 1 if the condition is true, 0 otherwise
 */
#define ZB_CHECK_IS_HT1(ptr)				\
  (ZB_IE_HEADER_GET_TYPE(ptr) == ZB_IE_HEADER_TYPE_HIE	\
   && ZB_HIE_GET_LENGTH(ptr) == 0U			\
   && ZB_HIE_GET_ELEMENT_ID(ptr) == ZB_HIE_ELEMENT_HT1)	\


/**
 * @brief Skip header IEs starting at position ptr until HT1 or HT2 is found
 *  or max_len bytes skipped.
 * On success, either all bytes are skipped or HT* information element can be
 *  found at (ptr + skipped_bytes) location
 *
 * @param ptr - pointer to the first HIE
 * @param max_len - maximum number of bytes to skip
 * @param skipped_bytes_ptr - number of skipped bytes will be stored here
 * @return ZB_TRUE if no errors, ZB_FALSE when given corrupted packet
 */
#define ZB_SKIP_HIE_TILL_HT(ptr, max_len, skipped_bytes_ptr)	\
  zb_skip_hie_till_ht((ptr), (max_len), (skipped_bytes_ptr))


/**
 * @brief Skip all IEs starting at position ptr.
 * On success, either all bytes are skipped or HIE-HT2 /
 * PIE-TERMINATION is found (and also skipped).
 * Data payload starts at (ptr + skipped_bytes)
 *
 * @param ptr - pointer to the some IE
 * @param max_len - maximum number of bytes to skip
 * @param skipped_bytes_ptr - number of skipped bytes will be stored here
 * @return ZB_TRUE if no errors, ZB_FALSE when given corrupted packet
 */
#define ZB_SKIP_ALL_IE(ptr, max_len, skipped_bytes_ptr) \
  zb_skip_all_ie((ptr), (max_len), (skipped_bytes_ptr))

/* IE termination (see 7.4.1 of 802.15.4) helpers */

/**
 * @brief Helps to determine whether it is required to put HT1 after header IEs
 */
#define ZB_IS_HT1_REQUIRED(hie_present, pie_present, payload_present) \
  (pie_present)

/**
 * @brief Helps to determine whether it is required to put HT2 after header IEs
 */
#define ZB_IS_HT2_REQUIRED(hie_present, pie_present, payload_present) \
  ((hie_present) && (!(pie_present)) && (payload_present))

/**
 * @brief Helps to determine whether it is required to put HT* after header IEs
 */
#define ZB_IS_HT_REQUIRED(hie_present, pie_present, payload_present) \
  (ZB_IS_HT1_REQUIRED((hie_present), (pie_present), (payload_present)) || \
  (ZB_IS_HT2_REQUIRED((hie_present), (pie_present), (payload_present))

/**
 * @brief Helps to determine whether it is required to put HT2 after header IEs
 */
#define ZB_IS_PT_REQUIRED(hie_present, pie_present, payload_present) \
  (pie_present)

/* TODO: now it says payload termination is required even when it is optional. is it ok? */

/**
 * @brief Returns the number of bytes that must be occupied by termination headers
 *  given the presence of HIEs, PIEs and data payload
 */
#define ZB_CALCULATE_TERMINATION_IES_LENGTH(hie_present, pie_present, payload_present) \
  (ZB_IS_PT_REQUIRED((hie_present), (pie_present), (payload_present)) ? ZB_HIE_HEADER_LENGTH : 0U) + \
  (ZB_IS_PT_REQUIRED((hie_present), (pie_present), (payload_present)) ? ZB_PIE_HEADER_LENGTH : 0U)

/**
 * @brief Puts HT* header (HT1 or HT2) if required and moves ptr accordingly
 * Must be called after all HIEs are written
 * hie_present, pie_present and payload_present are boolean flags indicating
 * existence of corresponding packet parts
 */

#define ZB_SET_NEXT_HT_IF_REQUIRED(ptr, hie_present, pie_present, payload_present)                 \
  {                                                                                                \
    if (ZB_IS_HT1_REQUIRED((hie_present), (pie_present), (payload_present)))                       \
    {                                                                                              \
      ZB_SET_NEXT_HIE_HEADER((ptr), ZB_HIE_ELEMENT_HT1, 0U);                                       \
    }                                                                                              \
    else if (ZB_IS_HT2_REQUIRED((hie_present), (pie_present), (payload_present)))                  \
    {                                                                                              \
      ZB_SET_NEXT_HIE_HEADER((ptr), ZB_HIE_ELEMENT_HT2, 0U);                                       \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      /* MISRA rule 15.7 requires empty 'else' branch. */                                          \
    }                                                                                              \
  }

/**
 * @brief Puts payload termination header if required and moves ptr accordingly
 * Must be called after all PIEs are written
 */
#define ZB_SET_NEXT_PT_IF_REQUIRED(ptr, hie_present, pie_present, payload_present)                 \
  {                                                                                                \
    if (ZB_IS_PT_REQUIRED((hie_present), (pie_present), (payload_present)))                        \
    {                                                                                              \
      ZB_SET_NEXT_PIE_HEADER((ptr), ZB_PIE_GROUP_TERMINATION, 0U);                                 \
    }                                                                                              \
  }

/* R22 parsing helpers */


/* parsed filter EB IE */
typedef struct zb_eb_filter_ie_s
{
  zb_uint8_t mask;
} zb_eb_filter_ie_t;

/* parsed TX power IE*/
typedef struct zb_tx_power_ie_s
{
  zb_int8_t tx_power_value;
} zb_tx_power_ie_t;

/* parsed MLME PIE*/
typedef struct zb_mlme_pie_parsed_s
{
  zb_bool_t eb_filter_set;
  zb_eb_filter_ie_t eb_filter;
} zb_mlme_pie_parsed_t;

/* parsed Zigbee EB Payload IE */
typedef struct zb_eb_payload_ie_parsed_s
{
  zb_uint8_t *beacon_payload_ptr;
  zb_uint8_t beacon_payload_len;
  zb_uint16_t superframe_spec;
  zb_uint16_t source_short_addr;
} zb_eb_payload_ie_parsed_t;

/* parsed Zigbee Rejoin IE */
typedef struct zb_rejoin_ie_s
{
  zb_ext_pan_id_t extended_pan_id;
  zb_uint16_t src_short_addr;
} zb_rejoin_ie_t;

/* parsed Vendor Specific PIE (Zigbee) */
typedef struct zb_zigbee_vendor_pie_parsed_s
{
  zb_bool_t eb_payload_set;
  zb_eb_payload_ie_parsed_t eb_payload;
  zb_bool_t rejoin_desc_set;
  zb_rejoin_ie_t rejoin_desc;
  zb_bool_t tx_power_set;
  zb_tx_power_ie_t tx_power;
} zb_zigbee_vendor_pie_parsed_t;


#if defined ZB_ENHANCED_BEACON_SUPPORT
zb_bool_t zb_skip_all_ie(zb_uint8_t *ptr,  zb_uint32_t max_len, zb_uint32_t *skipped_bytes);

zb_bool_t zb_skip_hie_till_ht(zb_uint8_t *ptr, zb_uint32_t max_len, zb_uint32_t *skipped_bytes);

zb_ret_t zb_get_enhaced_beacon_zigbee_payload(zb_mac_beacon_notify_indication_t *ind,
                                              zb_mac_beacon_payload_t **beacon_payload,
                                              zb_uint8_t *beacon_payload_len);


/*
 * @brief Function to parse Zigbee vendor PIE starting at ptr
 */
zb_bool_t zb_parse_zigbee_vendor_pie(zb_uint8_t *ptr, zb_uint32_t len,
				     zb_zigbee_vendor_pie_parsed_t *parsed);

/*
 * @brief Function to parse mlme PIE starting at ptr
 */
zb_bool_t zb_parse_mlme_pie(zb_uint8_t *ptr, zb_uint32_t len,
			    zb_mlme_pie_parsed_t *parsed);
#endif /* ZB_ENHANCED_BEACON_SUPPORT */

#endif
