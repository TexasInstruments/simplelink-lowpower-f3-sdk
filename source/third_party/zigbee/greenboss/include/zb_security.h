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
/* PURPOSE: GP NWK layer security API
*/

#ifndef ZB_SECURITY_H
#define ZB_SECURITY_H 1

#define ZB_SECURITY_KEY_LEN             16
#define ZB_SECURITY_NONCE_LEN           13
#define ZB_SECURITY_CCM_M               4
#define ZB_SECURITY_CCM_L               2

/** \addtogroup secur
*/
/*! @{ */

/**
 * @brief Initializes the security module.
 *
   @note Typically the function shouldn't be called by user application
   because it is called internally by \ref gboss_app_init().
 *
 * @note The function should be called before any
 * encoding operation.
 *
 * @param key 16-bytes encryption key to be used for security operations
 * @param key_type GPD key type used for GPD frames encryption
 * @param application_id Application ID, which defines the GPDF frame format
 * @param src_id GPD source ID
 * @param src_addr 64-bit source address
 */
void zb_security_init(
  const zb_uint8_t* key,
  zb_uint8_t key_type,
  zb_uint8_t application_id,
  zb_uint32_t src_id,
  const zb_ieee_addr_t src_addr);

/**
 * @brief Encrypts the user packet using a key selected earlier.
 *
 * According to Zigbee CCM encryption standard the data
 * to be encrypted consists of two parts: "a" and "m" strings.
 *
 * @param header "a" string according to CCM description
 * @param payload "m" string according to CCM description
 * @param header_size Length of the "a" part
 * @param m_payload_size Length of the "m" part
 * @param securiy_level Levels 2 are 3 valid
 * @param seq_number Packet sequential number
 * @param result Buffer where the result should be stored to
 * @return zb_uint32_t MIC encryption value
 */
zb_uint32_t zb_security_encode(
    const zb_uint8_t* header,
    const zb_uint8_t* payload,
    zb_uint8_t header_size,
    zb_uint8_t payload_size,
    zb_uint8_t security_level,
    zb_uint32_t seq_number,
    zb_uint8_t* result);

/**
 * @brief Decrypts the user packet using a key selected earlier.
 *
 * According to Zigbee CCM encryption standard the data
 * to be decrypted consists of two parts: "a" and "m" strings.
 *
 * @param app_id GP device Application ID
 * @param src_id GP device's source id (for AppID = 0b000)
 * @param ieee_addr GP device's IEEE address (for AppID = 0b010)
 * @param header "a" string according to CCM description
 * @param payload "m" string according to CCM description
 * @param header_size Length of the "a" part
 * @param m_payload_size Length of the "m" part
 * @param seq_number Packet sequential number
 * @param mic Income Message Integrity Code
 * @param result Buffer where the result should be stored to
 * @return zb_ret_t Decode status
 */
zb_ret_t zb_security_decode(
    zb_uint8_t app_id,
    zb_uint32_t src_id,
    const zb_ieee_addr_t ieee_addr,
    const zb_uint8_t* header,
    const zb_uint8_t* payload,
    zb_uint8_t header_size,
    zb_uint8_t payload_size,
    zb_uint32_t seq_number,
    zb_uint32_t mic,
    zb_uint8_t* result);

/**
 * @brief Returns current encryption key.
 *
 * @return const zb_uint8_t* pointer to the encryption key
 */
const zb_uint8_t* zb_security_get_key();

/**
 * @brief Sets current encryption key.
 * @note Function doesn't perform copy of the passed key and refers to the key by pointer.
 *
 * @param const zb_uint8_t* pointer to the encryption key
 */
void zb_security_set_key(const zb_uint8_t* key);

/**
 * @brief Returns type of the current encryption key.
 *
 * @return GP device encryption key type
 */
zb_uint8_t zb_security_get_key_type();


/**
 * @brief Sets type of the current encryption key.
 *
 * @param key_type GP device encryption key type
 */
void zb_security_set_key_type(zb_uint8_t key_type);

/**
 * @brief Sets the initial encryption key that was used at startup.
 * @ref zb_security_init
 * 
 */
void zb_security_set_initial_key(void);

/**
 * @brief Sets the initial key type that was used at startup.
 * @ref zb_security_init
 * 
 */
void zb_security_set_initial_key_type(void);


/**
 * @brief Performs personal key encryption for commissioning.
 *
 * Encryption of commissioning command significantly differs from ordinary
 * packet security. In the commissioning packet only the key should be
 * encrypted instead of full data packet.
 *
 * @param key_tobe_encoded Device's key to be encoded
 * @param app_id GP device Application ID
 * @param src_id GP device's source id (for AppID = 0b000)
 * @param ieee_addr GP device's IEEE address (for AppID = 0b010)
 * @param result_key Buffer where the encoded key should be stored to
 * @return zb_uint32_t MIC encryption value
 */
zb_uint32_t zb_security_encode_key_for_commiss(
    const zb_uint8_t *key_tobe_encoded,
    zb_uint8_t app_id,
    zb_uint32_t src_id,
    const zb_ieee_addr_t ieee_addr,
    zb_uint8_t *result_key);


/**
 * @brief Performs GPD key decryption from commissioning reply frame.
 *
 * Decryption of GPD key significantly differs from ordinary
 * packet security. In the commissioning reply packet only the key 
 * should be decrypted instead of full data packet.
 *
 * @param app_id GP device Application ID
 * @param src_id GP device's source id (for AppID = 0b000)
 * @param ieee_addr GP device's IEEE address (for AppID = 0b010)
 * @param encrypted_key Pointer to key passed in Commissioning Reply frame
 * @param frame_counter Received Commissioning Reply frame counter
 * @param mic Commissioning Reply Message Integrity Code 
 * @param plain_key Buffer where the encoded key should be stored to
 * @return zb_ret_t Key decryption and authentication status
 */
zb_ret_t zgpd_decrypt_n_auth_gpd_key(
    zb_uint8_t app_id,
    zb_uint32_t src_id,
    const zb_ieee_addr_t ieee_addr,
    zb_uint8_t *encrypted_key,
    zb_uint32_t frame_counter,
    zb_uint32_t mic,
    zb_uint8_t *plain_key);

/**
 * @brief Makes one AES128 encryption cycle.
 *
 * @param key Key to be used for encryption
 * @param msg Message to be processed
 * @param c Buffer for the result
 */
#ifdef NEED_SOFT_AES128
void zb_aes128(const zb_uint8_t *key, zb_uint8_t *msg, zb_uint8_t *c);
#else
#define zb_aes128 zb_osif_aes128
#endif /* NEED_SOFT_AES128 */

/*! @} */

#endif
