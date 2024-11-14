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
/* PURPOSE: Common include file for ZigBee
*/

#ifndef ZB_NVM_H
#define ZB_NVM_H 1

/**
   @addtogroup comm_int
   @{
 */

#define ZB_NVM_BIT_SHIFT(bit_number)  (1UL << ((ZB_FLASH_WORD_SIZE_BYTES * 8u - 1u) - bit_number))

/**
  Commissioning page structure in NVM.

  A single flash page is used to store the commissioning information of the following available types:
  - Unidirectional,
  - Bidirectional battery-less,
  - Bidirectional battery-powered.

  The structure of the commissioning page is consistent across all types of commissioning, even if certain parts are unused for a specific commissioning type.

  The page consists of a fixed size part and the rest of the page is used to store index in the current channels array.

  To forget commissioning info (== decommission) just erase the page.
  It is supposed that the flash is erased to ~0.

  All data structures in the commissioning page are word-aligned. Use 32 bits even where 8 or 16 bits are enough.
 */
typedef struct zb_comm_page_header_s
{
  /**
     For unidirectional - index in the tx_cfg->channels[] after commissioning completeness, ~0 if not commissioned.

     For bidirectional - channel number (not index!) when channel is configured by receiving Channel Config.
   */
  zb_uint32_t channel_index;
  /**
     If bidir_comm_stage_done[i] is set to the value other than ~0, than bidirectional commissioning stage is successfully passed.
     Used for battery-less bidirectional commissioning, ignored in other cases.
   */
  zb_uint32_t bidir_comm_stage_done[ZGPD_COMM_N_STAGES];
  /**
     GPD key received for GPCB, if GPD is configured to receive a key. Ignored in all other cases.
   */
  zb_uint8_t gpd_key[ZB_SECURITY_KEY_LEN];

  /**
     Type of GPD key received.
   */
  zb_uint32_t key_type;

  /**
     PAN ID saved as 32bit unsigned integer, only lower 16bits are used.
  */
  zb_uint32_t pan_id;

  /**
     Channels index bitmap (first word). The bitmap resides up to end of the page.
   */
  zb_uint32_t channel_idx_bm_start[1];
} zb_comm_page_header_t;


/**
 * @brief Retrieves next MAC sequential number for a packet to be sent
 * and saves it to non-volatile memory.
 *
 * The function provides generation of a very long sequence of the
 * incrementing numbers. Due to limitations of
 * energy and flash memory characteristics the own data structure is
 * used to minimize memory erasing.
 *
 * @note This function performs write operation in non-volatile memory.
 * Once in 1024 attempts one memory sector has to be erased.
 * In this case next MAC sequence number will be returned, however,
 * there might not be enough power to send a packet over the air.
 *
 * @return zb_uint32_t generated sequence number
 */
zb_uint32_t zgpd_get_seq_number(void);


/**
 * @brief Looks up for the next MAC sequential number for a packet
 * to be sent without persisting to non-volatile memory.
 *
 * The function provides generation of a very long sequence of the 
 * incrementing numbers. Due to limitations of
 * energy and flash memory characteristics own data structure
 * used to minimize memory erasing.
 *
 * @return zb_uint32_t generated sequence number
 */
zb_uint32_t zgpd_lookup_seq_number(void);


/**
@}
*/

void zb_commission_nvm_restart(void);
void zb_commission_nvm_load(void);
zb_comm_page_header_t *zb_commission_nvm_get(void);
zb_uint32_t zb_commission_nvm_get_channel_index(void);
void zb_commission_nvm_get_gpd_key(zb_uint8_t *key);
zb_uint8_t zb_commission_nvm_get_key_type(void);
zb_uint16_t zb_commission_nvm_get_pan_id(void);
void zb_commission_nvm_save_channel(zb_uint8_t channel_idx);
void zb_commission_nvm_save_stage(zb_uint8_t stage);
void zb_commission_nvm_save_key(zb_uint8_t *key);
void zb_commission_nvm_save_key_type(zb_uint8_t key_type);
void zb_commission_nvm_save_pan_id(zb_uint16_t pan_id);
zb_uint32_t zb_commission_nvm_chan_bm_start(void);
zb_uint32_t zb_commission_nvm_chan_bm_end(void);

zb_int16_t zb_commission_nvm_get_next_index(void);
zb_int16_t zb_search_next_bit(zb_uint32_t offset_first, zb_uint32_t offset_last);

/**
 * @brief Reads a word from the given address, modifies it via bitwise AND and writes the word back.
 *
 * The function takes a word as an argument, usually it's 32 bit size. Some platforms doesn't allow
 * changing bit state from 0 to 1 in Flash memory without erasing whole sector or page,
 * bitwise AND prevents such types of operations, if old value was 0 it will remain the same after modification.
 *
 * @param address Offset in the NVRAM array
 * @param buf     Buffer containing data (a word) for writing
 * @param len     Count bytes for write data (a word size)
 *
 * @return RET_OK if success or code error
 */
zb_ret_t zb_nvm_word_read_modify_write(zb_uint32_t address, zb_uint8_t *buf, zb_uint16_t len);

#endif  /* ZB_NVM_H */
