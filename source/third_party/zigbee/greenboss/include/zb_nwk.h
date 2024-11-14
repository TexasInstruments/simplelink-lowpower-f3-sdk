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
/* PURPOSE: NWK layer ZGPD API
*/

#ifndef ZB_NWK_H
#define ZB_NWK_H 1

/*! \addtogroup nwk */
/*! @{ */


/** @brief ZGPD application ID. */
typedef enum zb_zgp_app_id_e
{
  ZB_ZGP_APP_ID_0000    = 0x00, /**< ApplicationID value 0b000 - usage of the SrcID */
  ZB_ZGP_APP_ID_0001    = 0x01, /**< ApplicationID value 0b001 - LPED */
  ZB_ZGP_APP_ID_0010    = 0x02, /**< ApplicationID value 0b010 - usage of the GPD IEEE address*/
  ZB_ZGP_APP_ID_INVALID = 0x07, /**< Invalid ApplicationID */
}
zb_zgp_app_id_t;

/**
 * @brief NWK frame type used with @ref ZB_ZGP_PROTOCOL_VERSION.
 * @see ZGP spec, A.1.4.1.2
 */
enum zb_gpdf_frame_type_e
{
  ZGP_FRAME_TYPE_DATA         = 0x00, /**< Data frame */
  ZGP_FRAME_TYPE_MAINTENANCE  = 0x01, /**< Maintenance frame */
  ZGP_FRAME_TYPE_RESERVED1    = 0x02, /**< Reserved */
  ZGP_FRAME_TYPE_RESERVED2    = 0x03  /**< Reserved */
};

/**
 * @brief ZGP security level type.
 * @see ZGP spec, A.1.5.3.2 */
enum zb_zgp_security_level_e
{
  ZB_ZGP_SEC_LEVEL_NO_SECURITY           = 0x00,  /**< No security */
  ZB_ZGP_SEC_LEVEL_REDUCED               = 0x01,  /**< 1LSB of frame counter and short (2B) MIC */
  ZB_ZGP_SEC_LEVEL_FULL_NO_ENC           = 0x02,  /**< Full (4B) frame counter and full (4B) MIC */
  ZB_ZGP_SEC_LEVEL_FULL_WITH_ENC         = 0x03,  /**< Encryption & full (4B) frame counter and
                                                       full (4B) MIC */
};

/* From ZGP spec, A. 1.4.1.4:
 * ZGPD ID value of 0x00000000 indicates unspecified.
 * ... 0xffffffff indicates all.
 */
#define ZB_ZGP_SRC_ID_UNSPECIFIED 0x00000000U
#define ZB_ZGP_SRC_ID_ALL         (zb_uint32_t)(~0u)

/** @brief Gets protocol version from the NWK header Frame Control field.

   @param fctl Frame Control Field of NWK header
*/
#define ZB_NWK_FRAMECTL_GET_PROTOCOL_VERSION(fctl) (((fctl)[ZB_PKT_16B_ZERO_BYTE] >> 2U) & 0xfU)

/********************************************************************/
/********** Get/set macros for individual bit sub-fields ************/
/********************************************************************/

#define ZB_GPDF_NFC_GET_NFC_EXT(frame_ctl) (((frame_ctl) & 0x80) >> 7)

#define ZB_GPDF_NFC_GET_FRAME_TYPE(frame_ctl) ((frame_ctl) & 0x03)

#define ZB_GPDF_NFC_SET_FRAME_TYPE(frame_ctl, frame_type) ((frame_ctl) |= ((frame_type) & 0x03))

#define ZB_GPDF_NFC_GET_AUTO_COMMISSIONING(frame_ctl) (((frame_ctl) >> 6) & 0x01)

#define ZB_GPDF_NFC_SET_AUTO_COMMISSIONING(frame_ctl, auto_comm) ((frame_ctl) |= ((!!(auto_comm)) << 6))

#define ZB_GPDF_EXT_NFC_GET_APP_ID(ext_fc) ((ext_fc) & 0x03)

#define ZB_GPDF_EXT_NFC_GET_SEC_LEVEL(ext_fc) (((ext_fc) >> 3) & 0x03)

#define ZB_GPDF_EXT_NFC_SET_SEC_LEVEL(ext_fc, lvl) ((ext_fc) |= (((lvl) & 0x03) << 3))

#define ZB_GPDF_EXT_NFC_GET_SEC_KEY(ext_fc) (((ext_fc) >> 5) & 0x01)

#define ZB_GPDF_EXT_NFC_SET_SEC_KEY(ext_fc, sec_key) ((ext_fc) |= ((!!(sec_key)) << 5))

#define ZB_GPDF_EXT_NFC_GET_RX_AFTER_TX(ext_fc) (((ext_fc) >> 6) & 0x01)

#define ZB_GPDF_EXT_NFC_SET_RX_AFTER_TX(ext_fc, rx_after_tx) ((ext_fc) |= ((!!(rx_after_tx))<<6))

#define ZB_GPDF_EXT_NFC_CLR_RX_AFTER_TX(ext_fc) (ext_fc) &= ~(1<<6)

#define ZB_GPDF_EXT_NFC_GET_DIRECTION(ext_fc) (((ext_fc) & 0x80) >> 7)

#define ZB_GPDF_EXT_NFC_SET_DIRECTION(ext_fc, dir) ((ext_fc) |= ((!!(dir))<<7))

#define ZGPD_SRC_ID_SIZE(app_id, frame_type) \
  (((app_id == ZB_ZGP_APP_ID_0000) && (frame_type == ZGP_FRAME_TYPE_DATA)) ? 4 : 0)


/** @brief Zigbee protocol version for GPDF frames. */
#define ZB_ZGP_PROTOCOL_VERSION    0x03U

#define ZB_COMM_MAX_PACKET_SIZE     120


/**
 * @brief ZGPD address.
 *
 * ZGPD is identified in network whether by SrcId or its IEEE address. */
typedef ZB_PACKED_PRE union zb_zgpd_addr_u
{
  zb_uint32_t     src_id;    /**< ZGPD SrcId @see ZGP spec, A.1.4.1.4 */
  zb_ieee_addr_t  ieee_addr; /**< ZGPD IEEE address */
} ZB_PACKED_STRUCT
zb_zgpd_addr_t;

/**
 * @brief ZGPD identification info (ZGPD ID).
 *
 * ZGPD is identified by SrcId if ApplicationID is @ref ZB_ZGP_APP_ID_0000.
 * Otherwise, ZGPD is identified by its IEEE address. */
typedef ZB_PACKED_PRE struct zb_zgpd_id_s
{
  zb_uint8_t         app_id;  /**< One of the @ref zb_zgp_app_id_t values */
  zb_uint8_t         endpoint;/**< Identifier of the GPD endpoint, which jointly with the GPD IEEE address identifies a unique logical GPD device.*/
  zb_zgpd_addr_t     addr;    /**< ZGPD SrcId or IEEE address */
} ZB_PACKED_STRUCT
zb_zgpd_id_t;

/**
 * @brief Parsed values of GPDF frame.
 *
 * Structure contains GPDF information that is needed by commissioning.
 */
typedef ZB_PACKED_PRE struct zb_gpdf_info_s
{
  zb_gpdf_mac_addr_flds_t   mac_addr_flds;      /**< MAC addressing fields */
  zb_uint8_t                mac_addr_flds_len;  /**< Length of data in MAC addressing fields */
  zb_uint8_t                mac_seq_num;        /**< MAC sequence number */
  zb_uint8_t                nwk_frame_ctl;      /**< NWK frame control */
  zb_uint8_t                nwk_ext_frame_ctl;  /**< Extended NWK frame control */
  zb_zgpd_id_t              zgpd_id;            /**< ZGPD ID */
  zb_uint32_t               sec_frame_counter;  /**< Security frame counter */
  zb_uint8_t                zgpd_cmd_id;        /**< ZGPD command ID */
  zb_uint8_t                nwk_hdr_len;        /**< Length of the GPDF NWK header */
  zb_uint8_t                status;             /**< 'Status' for GP-DATA.indication @see  */
  zb_uint8_t                key[16];            /**< Key to decrypt the frame */
  zb_uint8_t                key_type;           /**< Type of key to be used for
                                                 that frame @see zb_zgp_security_key_type_e */
  zb_uint8_t                mic[4];
}
ZB_PACKED_STRUCT zb_gpdf_info_t;

/**
 * @brief ZGPD Commissioning reply parameters.
 * @see ZGP spec, A.4.2.1.1
 */
typedef struct zb_gpdf_comm_reply_s
{
  zb_uint8_t     options;                       /**< Options */
  zb_uint16_t    pan_id;                        /**< Pan ID if requested */
  zb_uint8_t     security_key[16];              /**< ZGPD key */
  zb_uint32_t    key_mic;                       /**< ZGPD key MIC */
  zb_uint32_t    frame_counter;                 /**< ZGPD key encryption counter */
}
zgpd_comm_reply_t;

#define ZB_GPDF_COMM_REPLY_PAN_ID_PRESENT(options) ((options) & 0x01U)
#define ZB_GPDF_COMM_REPLY_SEC_KEY_PRESENT(options) (((options) >> 1U) & 0x01U)
#define ZB_GPDF_COMM_REPLY_SEC_KEY_ENCRYPTED(options) (((options) >> 2U) & 0x01U)
#define ZB_GPDF_COMM_REPLY_SEC_LEVEL(options) (((options) >> 3U) & 0x03U)
#define ZB_GPDF_COMM_REPLY_SEC_KEY_TYPE(options) (((options) >> 5U) & 0x07U)


/**
 * @brief Prepares NWK header in the packet to be sent.
 *
 * @param buffer Pointer to the area where to put the NWK header
 * @param device_config Device configuration block
 * @param security_level Expected message security level
 * @param seq_number Packet sequential number
 * @param rx_after_tx RX after TX bit value
 * 
 * @return Size of NWK header
*/
zb_uint8_t zb_nwk_frame_prepare(
    zb_uint8_t* buffer,
    const gboss_device_config_t *device_config,
    zb_uint8_t security_level,
    zb_uint32_t seq_number,
    zb_uint8_t rx_after_tx);

/**
 * @brief Sends one data packet over the air.
 *
 * The function sends an application layer payload. It automatically adds
 * all necessary low-level headers and performs encryption if necessary.
 *
 * Once a packet is built in the internal memory it can be sent several times
 * over multiple channels if necessary.
 *
 * @param device_config GPD configuration block
 * @param security_level Levels 0, 2 and 3 are supported now - @ref gboss_security_key_capabilities_t
 * @param seq_number Packet's sequence number
 * @param data Pointer to the APP layer data payload
 * @param data_size Size of the data
 * @param channels Pointer to the list of the channels to be used for transmitting
 * @param channel_count Number of items in the channels list
 * @param repetitions Number of sending repetitions for every channel
 * 
 * @return zb_ret_t RET_OK if the packet was successfully sent, RET_ERROR
 * otherwise
 * 
 * @todo Validate inputs, check if MAC really sent it, return RET_ERROR if not
 */
zb_ret_t zgpd_pkt_send_unidir(
    const gboss_device_config_t* device_config,
    zb_uint8_t security_level,
    zb_uint32_t seq_number,
    const zb_uint8_t* data,
    zb_uint8_t data_size,
    const zb_uint8_t * channels,
    zb_uint8_t channel_count,
    zb_uint8_t repetitions);

/**
 * @brief Sends one data packet over the air with RX after TX bitfield control.
 *
 * The function sends an application layer payload. It automatically adds
 * all necessary low-level headers and performs encryption if necessary.
 *
 * Once a packet is built in the internal memory it can be sent several times
 * over multiple channels if necessary. 
 *
 * @param device_config GPD configuration block
 * @param security_level Levels 0, 2 and 3 are supported now - @ref gboss_security_key_capabilities_t
 * @param seq_number Packet's sequence number
 * @param data Pointer to the APP layer data payload
 * @param data_size Size of the data
 * @param channel Channel to be used
 * @param dst_pan_id Destination PAN ID
 * @param repetitions Number of sending repetitions
 * @param rx_after_tx Enable RX after TX bitfield of NWK Extended header
 * 
 * @return zb_ret_t RET_OK if the packet was successfully sent, RET_ERROR
 * otherwise
 * 
 * @todo Validate inputs, check if MAC really sent it, return RET_ERROR if not
 */
zb_ret_t zgpd_pkt_send_bidir(const gboss_device_config_t* device_config,
                             zb_uint8_t security_level,
                             zb_uint32_t seq_number,
                             const zb_uint8_t* data,
                             zb_uint8_t data_size,
                             zb_uint8_t channel,
                             zb_uint16_t dst_pan_id,
                             zb_uint8_t repetitions,
                             zb_uint8_t rx_after_tx);


/**
 * @brief Parses ZGP Stub NWK header of GPDF.
 *
 * @param gpdf      [in]   Pointer to the beginning of ZGP Stub NWK header
 * @param gpdf_len  [in]   Length from beginning of ZGP Stub NWK header to the end of MIC field
 * @param gpdf_info [out]  GPDF info structure
 *
 * @return Number of bytes parsed in ZGP Stub NWK header if parsing successful \n
 *         0 if parsing has failed
 * @see ZGP spec, A.1.4.1
 */
zb_uint8_t zgpd_parse_gpdf_nwk_hdr(zb_uint8_t *gpdf,
                                   zb_uint8_t gpdf_len,
                                   zb_gpdf_info_t *gpdf_info);



/*! @} */

#endif
