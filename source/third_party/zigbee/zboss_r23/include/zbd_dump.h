/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
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

#ifndef ZBD_DUMP_H
#define ZBD_DUMP_H

#include "zb_common.h"
#include "zb_direct_common.h"

typedef enum zb_dump_info_e
{
  /** 
   * Clear current used key
   */
  ZB_DUMP_INFO_CCM_KEY_DELETE,
  /**
   * Replace current key with a new one
   * <key> || <zdd addr> || <zvd addr> 
   */
  ZB_DUMP_INFO_CCM_KEY_SET,
  /**
   * Specify, if encryption is needed or not
   * <1|0> 
   */
  ZB_DUMP_INFO_ENCRYPTION_STATUS
} zb_dump_info_t;

#ifdef ZB_TRAFFIC_DUMP_ON

/**
 * @brief Initialize dump (put some information packets, needed in advance)
 */
void zb_ble_dump_init(void);

/**
 * @brief Add info about characteristic to the dump
 * (have to be called before dumping from that characteristic)
 *
 * @param handle        characteristic handle
 * @param uuid          pointer to characteristics uuid in BE (either uuid16 or uuid128)
 * @param uuid_is_short indicates whether uuid is uuid16 or uuid128
 */
void zb_ble_traffic_dump_char_info(zb_ble_characteristic_handle_t handle,
                                   const zb_uint8_t *uuid,
                                   zb_bool_t uuid_is_short);

/**
 * @brief Dump BLE traffic
 * Encapsulates traffic into BLE packets
 *
 * @param data          traffic to be dumped
 * @param length        length of the payload
 * @param handle        handle of a characteristic, performing dump
 * @param is_outgoing   shall be ZB_TRUE for outgoing packets and ZB_FALSE for incoming
 * @param ble_iface_id  mac interface id
 */
void zb_ble_traffic_dump(const void *data,
                         zb_uint8_t length,
                         zb_ble_characteristic_handle_t handle,
                         zb_bool_t is_outgoing);

/**
 * @brief Put a special info packet for dissector
 * 
 * @param info    type of packet @see zb_dump_info_e
 * @param data    data to be written to the packet
 * @param length  number of bytes in the packet
 */
void zb_ble_info_dump(zb_dump_info_t info,
                      const void* data,
                      zb_uint8_t length);

/**
 * @brief Set decryption key for the following ZBD traffic
 * 
 * @param key               session key
 * @param local_ieee_addr   IEEE address of current device
 * @param remote_ieee_addr  IEEE address of the communicated device
 */
void zbd_dump_key_set(zb_uint8_t key[ZB_CCM_KEY_SIZE], zb_uint8_t local_ieee_addr[8], zb_uint8_t remote_ieee_addr[8]);

/**
 * @brief Report, if following traffic is encrypted or not
 * 
 * @param is_encrypted  encryption status (encryption is on if ZB_TRUE, off otherwise)
 */
void zbd_dump_encryption_status(zb_bool_t is_encrypted);

#else

#define zb_ble_dump_init(...)
#define zb_ble_traffic_dump_char_info(...)
#define zb_ble_traffic_dump(...)
#define zb_ble_info_dump(...)
#define zbd_dump_key_set(...)
#define zbd_dump_encryption_status(...)

#endif /* ZB_TRAFFIC_DUMP_ON */

#define zbd_dump_key_clear() zb_ble_info_dump(ZB_DUMP_INFO_CCM_KEY_DELETE, NULL, 0)
#define zbd_dump_encryption_on()  zbd_dump_encryption_status(ZB_TRUE)
#define zbd_dump_encryption_off() zbd_dump_encryption_status(ZB_FALSE)

#endif /* ZBD_DUMP_H */
