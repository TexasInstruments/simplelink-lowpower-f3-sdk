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
/* PURPOSE: Mac layer API
*/

#ifndef ZB_MAC_H
#define ZB_MAC_H 1

/*! \addtogroup mac */
/*! @{ */


/**
 * @brief MAC addressing fields of GPDF as they are transmitted over the air.
 *
 * This structure is used during security processing for
 * security level @ref ZB_ZGP_SEC_LEVEL_REDUCED.
 *
 * From ZGP spec, A.1.5.4.2 "Initialization":
 * Header = MAC sequence number || MAC addressing fields || NWK Frame Control || Extended NWK
 * Frame Control || SrcID.
 *
 * During the encryption process by ZGP or ZGPD, the MAC addressing fields of the packet remain not ready.
 * They will be calculated only on the MAC layer. This is a problem and the following trick is implemented:
 * MAC addressing fields are filled at ZGP level.
 *
 * In case of handling incoming frames, MAC addressing frame values are copied from the original frame.
 *
 * This union contains MAC addressing fields of GPDF as they are broadcasted over the air.
 */
typedef ZB_PACKED_PRE union zb_gpdf_mac_addr_flds_u
{
  ZB_PACKED_PRE struct zgp_mac_addr_flds_short_s
  {
    zb_uint16_t  dst_addr;
    zb_uint16_t  dst_pan_id;
  }
  ZB_PACKED_STRUCT s;

  ZB_PACKED_PRE struct zgp_mac_addr_flds_long_s
  {
    zb_ieee_addr_t  addr;
  }
  ZB_PACKED_STRUCT l;

  ZB_PACKED_PRE struct zgp_mac_addr_flds_combined_s
  {
    zb_uint16_t     dst_addr;
    zb_uint16_t     dst_pan_id;
    zb_ieee_addr_t  src_addr;
  }
  ZB_PACKED_STRUCT comb;

  zb_uint8_t in[12];         /**< MAC addressing fields of incoming frame */
}
zb_gpdf_mac_addr_flds_t;


/* MAC TX statuses */
#define MAC_NO_ACK       0
#define MAC_ACK_RECEIVED 1

/** @{ */
/** 802.15: 0x00 = no address (addressing fields omitted, see 7.2.1.1.8). */
#define ZB_ADDR_NO_ADDR                  0x00U
/** 802.15: 0x01 = reserved.
 *  NWK:    0x01 = 16-bit multicast group address. */
#define ZB_ADDR_16BIT_MULTICAST          0x01U
/** APS:    0x01 = 16-bit group address for DstAddress. */
#define ZB_ADDR_16BIT_GROUP  ZB_ADDR_16BIT_MULTICAST
/** 802.15: 0x02 = 16-bit short address.
 *  NWK:    0x02 = 16-bit network address of a device or a 16-bit broadcast address. */
#define ZB_ADDR_16BIT_DEV_OR_BROADCAST   0x02U
/** 802.15: 0x03 = 64-bit extended address. */
#define ZB_ADDR_64BIT_DEV                0x03U
/** @} */

/**
   Broadcast Pan ID value
*/
#define ZB_BROADCAST_PAN_ID 0xFFFFU

/**
   Parser MAC header.

   Mac spec 7.2.1 General MAC frame format.
   @note Actually MHR is of variable length and this structure can't
   be used for physical data i/o, it is used only for function calls.
*/
typedef struct zb_mac_mhr_s
{
  zb_addr_u dst_addr;     /**< */
  zb_addr_u src_addr;     /**< */
  zb_uint16_t src_pan_id;       /**< */
  zb_uint16_t dst_pan_id;       /**< */
  zb_uint8_t frame_control[2];  /**< */
  zb_uint8_t seq_number;        /**< */
  zb_uint8_t mhr_len;        /**< */
}
zb_mac_mhr_t;

/**
   Gets source addressing mode subfield in frame control field (FCF).
   Retrieves value from @ref address_modes.

   @param p_fcf Pointer to 16bit FCF field
   
*/

#define ZB_FCF_GET_DST_ADDRESSING_MODE( p_fcf )  (((( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_FIRST_BYTE] ) & 0x0CU) ) >> 2U )

/**
   Gets source addressing mode subfield in frame control field (FCF).
   Retrieves value from @ref address_modes.

   @param p_fcf Pointer to 16bit FCF field.
*/

#define ZB_FCF_GET_SRC_ADDRESSING_MODE( p_fcf )  ( (( ( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_FIRST_BYTE] ) & 0xC0U )) >> 6U )

/**
   Gets Pan ID compression bit subfield in frame control field (FCF).
   Retrieved value can be 0 or 1.

   @param p_fcf Pointer to 16bit FCF field.
*/

#define ZB_FCF_GET_PANID_COMPRESSION_BIT( p_fcf ) (( ( ( ( const zb_uint8_t* ) ( p_fcf ) )[ZB_PKT_16B_ZERO_BYTE] ) & 0x40U ))

/**
   Inits ZGPD MAC HW.

   @note Typically the function shouldn't be called by user application
   because it is called internally by \ref gboss_app_init().

 */
void zb_mac_init();

/**
   Inits ZGPD MAC IEEE address.

   @note Typically the function shouldn't be called by user application
   because it is called internally by \ref gboss_app_init().

   @param ieee_addr IEEE address of GP device
 */
void zb_mac_set_ieee_addr(const zb_ieee_addr_t ieee_addr);

/**
 * @brief Prepares a packet to be sent.
 *
 * The function sets some fields in the MAC header of the packet.
 * Should be called before \ref zb_mac_send() calling.
 * 
 * @note This function simplifies the selection of MAC header fields for
 * GPD, aiming to save resources.
 *
 * @param buffer Pointer to the start of MAC header area
 * @param device_config GPD configuration block
 * @param sequence_number Packet sequential number
 * @param dst_pan_id Destination PAN ID
 * @return Size of MAC header
 */
zb_uint8_t zb_mac_frame_prepare(zb_uint8_t* buffer, const gboss_device_config_t* device_config, zb_uint8_t sequence_number, zb_uint16_t dst_pan_id);

/**
   @brief Sends ZGPD frame through one channel.

   @note The channel to be used for sending should be previously set
   by the \ref zb_mac_set_tx_channel.

 * @note First byte of the buffer is a length of the data. It is not
 * sent actually.
 *
   Sends one frame. It is supposed that MAC sequence number was added earlier.

   @param buf Buffer to be sent. The first byte in the buffer is a packet length
   @param repetitions Number of packets in GPDFS
 */
void zb_mac_send(zb_uint8_t* buf, zb_uint8_t repetitions);


/**
   Blocks waiting for TX complete.
 */
void zb_mac_sync_wait_tx_complete(void);

/**
   @brief Sets the TX power on the transmitter (in dBm).

   @param power Expected power in dBm
 */
void zb_mac_set_tx_power(zb_int8_t power);

/**
 * @brief Sets RF channel to be used.
 *
 * @param channel Channel number in range 11..26 according to 802.15.4 specification
 */
void zb_mac_set_tx_channel(zb_int8_t channel);

/**
   @brief Parses packed mhr header, fills mhr structure.

   @param mhr Out pointer to mhr structure
   @param ptr Pointer to packed mhr header buffer
   @return Packed mhr buffer length
*/
zb_uint8_t zb_parse_mhr(zb_mac_mhr_t *mhr, zb_uint8_t *buf);

#ifdef ZB_SOFTWARE_FCS
/**
   Adds FCS to the MAC packet (for platforms where radio does not support HW FCS).
 */
void zb_mac_fcs_add(zb_uint8_t *buf);
#endif
  
/*! @} */

#endif
