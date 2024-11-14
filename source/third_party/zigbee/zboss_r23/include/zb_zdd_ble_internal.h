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
/* PURPOSE: Zigbee Diret OSIF layer
*/
#ifndef __ZB_ZDD_BLE_INTERNAL_H__
#define __ZB_ZDD_BLE_INTERNAL_H__ 1

#ifdef ZB_DIRECT_ENABLED

#include "zb_common.h"
#include "zb_direct_common.h"

#include "zb_zdd.h"

/**
 * @brief Initilaize a BLE stack
 *
 * @param device_name Zero-terminated ASCII string representing device's name
 *
 * @return RET_OK if BLE was initialized, RET_ERROR otherwise
 */
zb_ret_t zbd_ble_init(const zb_char_t* device_name);

/**
 * @brief Enable BLE interface
 *
 * @return zb_ret_t
 *          - RET_OK on success
 *          - RET_UNINITIALIZED if BLE stack is not initialized yet
 *          - RET_ERROR on internal stack error
 */
zb_ret_t zbd_ble_interface_enable(void);

/**
 * @brief Disable BLE interface
 *
 * @return zb_ret_t
 *          - RET_OK on success
 *          - RET_UNINITIALIZED if BLE stack is not initialized yet
 *          - RET_ERROR on internal stack error
 */
zb_ret_t zbd_ble_interface_disable(void);

/**
 * @brief Create a BLE new characteristic in a service
 *
 * @param service Pointer to a service descriptor returned earlier by @zbd_osif_ble_create_service
 * @param uuid128 Desired 128-bit UUID of the characteristic
 * @param buffer Pointer to a buffer for data receiving. If NULL, memory will be allocated by the stack internally
 * @param size Size of receive buffer or size of memory to be reserved by a BLE stack internally
 * @param read TRUE if the characteristic should be readable by a peer
 * @param write TRUE if the characteristic should be writable by a peer
 * @param indication TRUE if the characteristic should support an indication to a peer
 * @param notification TRUE if the characteristic should support a notification to a peer
 * @param description ASCII string with human readable characteristic name
 * @param charact_handle A handle of the new characteristic to be used as reference further
 * @param is_user_defined Is characteristic defined by user (ZB_TRUE) or is part of ZBDirect spec (ZB_FALSE)
 * @return zb_ret_t Error code. For now only RET_OK can be returned.
 */
zb_ret_t zbd_ble_create_characteristic(zb_ble_service_handle_t service,
                                       const zb_uint8_t *uuid128,
                                       zb_uint8_t *buffer,
                                       zb_uint16_t size,
                                       zbd_characteristic_properties_t properties,
                                       const char *description,
                                       zb_ble_characteristic_handle_t *charact_handle,
                                       zb_bool_t is_user_defined);

/**
 * @brief Create a BLE new characteristic in a service
 *
 * @param service Pointer to a service descriptor returned earlier by @zbd_osif_ble_create_service
 * @param uuid16 Desired 16-bit UUID of the characteristic
 * @param buffer Pointer to a buffer for data receiving. If NULL, memory will be allocated by the stack internally
 * @param size Size of receive buffer or size of memory to be reserved by a BLE stack internally
 * @param read TRUE if the characteristic should be readable by a peer
 * @param write TRUE if the characteristic should be writable by a peer
 * @param indication TRUE if the characteristic should support an indication to a peer
 * @param notification TRUE if the characteristic should support a notification to a peer
 * @param description ASCII string with human readable characteristic name
 * @param charact_handle A handle of the new characteristic to be used as reference further
 * @param is_user_defined Is characteristic defined by user (ZB_TRUE) or is part of ZBDirect spec (ZB_FALSE)
 * @return zb_ret_t Error code. For now only RET_OK can be returned.
 */
zb_ret_t zbd_ble_create_characteristic_short_id(zb_ble_service_handle_t service,
                                                zb_uint16_t uuid16,
                                                zb_uint8_t *buffer,
                                                zb_uint16_t size,
                                                zbd_characteristic_properties_t properties,
                                                const char *description,
                                                zb_ble_characteristic_handle_t *charact_handle,
                                                zb_bool_t is_user_defined);

/**
 * @brief Set receiving callback
 *
 * @param charact_handle Characteristic handle which the callback is being set for
 * @param callback A callback
 */
void zbd_ble_set_recv_callback(zb_ble_characteristic_handle_t charact_handle, zbd_zdd_recv_callback_t callback);

/**
 * @brief Set subscription callback
 *
 * @param charact_handle Characteristic handle which the callback is being set for
 * @param callback A callback
 */
void zbd_ble_set_subscribe_callback(zb_ble_characteristic_handle_t charact_handle, zbd_zdd_subscribe_callback_t callback);

/**
 * @brief Set sending callback
 *
 * @param charact_handle Characteristic handle which the callback is being set for
 * @param callback A callback
 */
void zbd_ble_set_send_callback(zb_ble_characteristic_handle_t charact_handle, zbd_zdd_send_callback_t callback);

/**
 * @brief Set a callback for new BLE connect handling
 *
 * @param callback A callback
 */
void zbd_ble_set_connect_callback(zbd_zdd_simple_callback_t callback);

/**
 * @brief Set a callback for new BLE disconnect handling
 *
 * @param callback  A callback
 */
void zbd_ble_set_disconnect_callback(zbd_zdd_simple_callback_t callback);

/**
 * @brief Set a callback for end-of-notification handling
 *
 * @param callback  A callback
 */
void zbd_ble_set_notif_sent_callback(zbd_zdd_simple_callback_t callback);

/**
 * @brief Set data request callback
 *
 * @param charact_handle Characteristic handle which the callback is being set for
 * @param callback A callback
 */
void zbd_ble_set_data_request_callback(zb_ble_characteristic_handle_t charact_handle, zbd_zdd_data_request_callback_t callback);

/**
 * @brief Disconnects from peer
 *
 * @param conn_id Connection ID
 * @return zb_ret_t
 *          - RET_ON on success
 *          - RET_ERROR on internal stack error
 */
zb_ret_t zbd_ble_disconnect(zb_ble_conn_id_t conn_id);

/**
 * @brief Initialize BLE advertizing engine
 *
 */
void zbd_ble_advert_init(void);

/**
 * @brief Update ZDD advertizing data
 *
 * @param pan_id Current ZDD's PAN id or 0xFFFF if not joined
 * @param nwk_addr Current ZDD's short NWK address or 0xFFFF if not joined
 * @return zb_ret_t
 *          - RET_ON on success
 *          - RET_ERROR on internal stack error
 */
zb_ret_t zbd_ble_advert_update(zb_uint16_t pan_id, zb_uint16_t nwk_addr, zbd_zdd_advert_flags_t flags);

/**
 * @brief Start an advertisement
 *
 * @return zb_ret_t
 *          - RET_ON on success
 *          - RET_ERROR on internal stack error
 */
zb_ret_t zbd_ble_advert_start(void);

/**
 * @brief Stop an advertisement
 *
 * @return zb_ret_t
 *          - RET_ON on success
 *          - RET_ERROR on internal stack error
 */
zb_ret_t zbd_ble_advert_stop(void);

/**
 * @brief Perform necessary deinitialization for BLE
 */
void zbd_zdd_ble_uninit(void);

#endif /* ZB_DIRECT_ENABLED */

#endif //__ZB_ZDD_BLE_INTERNAL_H__
