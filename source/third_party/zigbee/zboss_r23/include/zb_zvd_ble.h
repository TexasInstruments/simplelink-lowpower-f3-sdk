/* ZBOSS Zigbee software protocol stack
 *
 * Copyright (c) 2012-2021 DSR Corporation, Denver CO, USA.
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
/**
 * PURPOSE: Zigbee Diret ZVD platform-independent API for BLE.
 */

#ifndef ZB_ZVD_BLE_H
#define ZB_ZVD_BLE_H

#include "zb_common.h"
#include "zb_direct_common.h"
#include "zboss_api_direct.h"

#define ZBD_BLE_CONN_ID_INVALID     (zb_ble_conn_id_t)(-1)

typedef enum zbd_zvd_data_received_callback_ret_e {
    ZBD_ZVD_RECV_RET_OK,
    ZBD_ZVD_RECV_RET_BUSY,
    ZBD_ZVD_RECV_RET_ERROR,
} zbd_zvd_data_received_callback_ret_t;


/**
 * @brief A structure to pass discovered characteristics to ZVD
 */
typedef struct zb_zvd_characteristic_s
{
  /* characteristic handle, used for ZVD-platform communication */
  zb_ble_characteristic_handle_t handle;
  /* Characteristic tag, used by ZVD to identify purpose of the characteristic */
  zbd_characteristic_tag_t tag;
  /* Characteristic UUID */
  const zb_char_t* uuid;
} zb_zvd_characteristic_t;

/**
 * @brief   Callback type that is being used for data receiving
 *
 * @param [in]  conn_id     Connection ID
 * @param [in]  handle      characteristic identifier (handle)
 * @param [in]  data        Pointer to the buffer where the received byte array is loaded
 * @param [in]  len         Length of the received byte array
 * @return BLE GATT Status code
 */
typedef zbd_zvd_data_received_callback_ret_t (*zbd_zvd_data_received_callback_t)       (zb_ble_conn_id_t                 conn_id,
                                                                                        zb_ble_characteristic_handle_t  handle,
                                                                                        const zb_uint8_t                *data,
                                                                                        zbd_data_len_t                  len,
                                                                                        zb_ret_t                        return_code);

/**
 * @brief   Callback type that is being used after data has been sent
 *
 * @param [in]  conn_id     Connection ID
 * @param [in]  handle      characteristic handle
 */
typedef void (*zbd_zvd_data_sent_callback_t)                                           (zb_ble_conn_id_t                 conn_id,
                                                                                        zb_ble_characteristic_handle_t  handle,
                                                                                        zb_ret_t                        return_code);

/**
 * @brief   Callback for filtering characteristics
 *
 * @param   characteristic  pointer to input characteristic descriptor
 *
 * @return  ZB_TRUE if characteristic shall be presented in filtered result, ZB_FALSE otherwise
 */
typedef zb_bool_t (*zbd_characteristic_filter_t)                                        (const zb_zvd_characteristic_t* characteristic);

/**
 * @brief   Set characteristic value read callback
 *
 * @param   handle          Characteristic handle which the callback is being set for
 * @param   callback        A callback
 */
void zbd_zvd_ble_set_charact_read_callback(zb_ble_characteristic_handle_t handle, zbd_zvd_data_received_callback_t callback);

/**
 * @brief   Set characteristic write callback
 *
 * @param   handle          Characteristic handle which the callback is being set for
 * @param   callback        A callback
 */
void zbd_zvd_ble_set_charact_write_callback(zb_ble_characteristic_handle_t handle, zbd_zvd_data_sent_callback_t callback);

/**
 * @brief   Initialize BLE stack
 *
 * @param   device_name     Zero-terminated ASCII string representing device's name
 *
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_init(const zb_char_t *device_name);

/**
 * @brief Retrieves the BLE device address
 *
 * @param ble_address   Pointer to the buffer where the address will be stored (6-bytes array),
 *                      MSB at position 0, LSB at position 5
 * @return zb_ret_t
 *          - RET_OK on success
 *          - RET_UNINITIALIZED if BLE stack is not initialized yet
 *          - RET_ERROR on internal stack error
 */
zb_ret_t zbd_ble_addr_get(zb_uint8_t* ble_address);

/**
 * @brief   Start discovering peripherals in range
 *
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_start_discovering_peripherals(void);

/**
 * @brief   Stop discovering peripherals
 *
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_stop_discovering_peripherals(void);

/**
 * @brief   Get the updated list of discovered peripherals
 *
 * @param   list            Updated list of discovered peripherals
 * @param   count
 *                              - <tt>[in]</tt> Size of the list
 *                              - <tt>[out]</tt> Length of the returned list
 *
 */
zb_uint32_t zbd_zvd_ble_get_discovered_peripherals_list(ZB_PERIPHERAL_ZDD_T *list, zb_uint32_t max_count);

/**
 * @brief   Connect to a peripheral
 *
 * @param   address         Pointer to 6-bytes array of the peripheral device address
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_connect(const zb_uint8_t *address);

/**
 * @brief   Disconnect from a peripheral
 *
 * @param   conn_id         ID of the connection, which is intended to be closed
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_disconnect(zb_ble_conn_id_t conn_id);

/**
 * @brief   Get all characteristics from the connected peripheral
 *
 * @param   conn_id         ID of the connection to the peripheral
 * @param   filter          Filter callback
 * @param   charact_list    List of characteristic tags - @ref zbd_characteristic_tag_t
 * @param   count
 *                              - <tt>[in]</tt> Size of the list
 *                              - <tt>[out]</tt> Length of the returned list
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_get_characteristics_list_by_filter(zb_ble_conn_id_t conn_id, zbd_characteristic_filter_t filter, zb_zvd_characteristic_t *charact_list, zb_uint32_t *count);

/**
 * @brief   Subscribes for notifications of a characteristic from a peripheral
 *
 * @param   conn_id         ID of the connection to the peripheral
 * @param   handle          Characteristic TAG which notifications are being subscribed for
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_notification_subscribe(zb_ble_conn_id_t conn_id, zb_ble_characteristic_handle_t handle);

/**
 * @brief   Unsubscribes for notifications of a characteristic from a peripheral
 *
 * @param   conn_id         ID of the connection to the peripheral
 * @param   handle          Characteristic TAG which notifications are being unsubscribed
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_notification_unsubscribe(zb_ble_conn_id_t conn_id, zb_ble_characteristic_handle_t handle);

/**
 * @brief   Subscribes for indications of a characteristic from a peripheral
 *
 * @param   conn_id         ID of the connection to the peripheral
 * @param   handle          Characteristic TAG which indications are being subscribed for
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_indication_subscribe(zb_ble_conn_id_t conn_id, zb_ble_characteristic_handle_t handle);

/**
 * @brief   Unsubscribes for indications of a characteristic from a peripheral
 *
 * @param   conn_id         ID of the connection to the peripheral
 * @param   handle          Characteristic TAG which indications are being unsubscribed
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_indication_unsubscribe(zb_ble_conn_id_t conn_id, zb_ble_characteristic_handle_t handle);

/**
 * @brief   Request to write a characteristic in a peripheral
 *
 * @param   conn_id         ID of the connection to the peripheral
 * @param   handle          Characteristic TAG which is being requested to write
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_charact_write(zb_ble_conn_id_t conn_id, zb_ble_characteristic_handle_t handle, const zb_uint8_t *data, zbd_data_len_t len);

/**
 * @brief   Request to read a characteristic from a peripheral
 *
 * @param   conn_id         ID of the connection to the peripheral
 * @param   handle          Characteristic TAG which is being requested to read
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_charact_read(zb_ble_conn_id_t conn_id, zb_ble_characteristic_handle_t handle);

/**
 * @brief   Request to read a characteristic descriptor from a peripheral
 *
 * @param   conn_id         ID of the connection to the peripheral
 * @param   handle          Characteristic TAG which descriptor is being requested to read
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_descr_read(zb_ble_conn_id_t conn_id, zb_ble_characteristic_handle_t handle);

/**
 * @brief   Set the Data Length Extension for a connection with a peripheral
 *
 * @param   conn_id         ID of the connection to the peripheral
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_set_dle_size(zb_ble_conn_id_t conn_id, zb_uint32_t dle_size);

/**
 * @brief   Set the Connection Interval for a connection with a peripheral
 *
 * @param   conn_id         ID of the connection to the peripheral
 * @param   interval        Connection interval
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_ble_set_connection_interval(zb_ble_conn_id_t conn_id, zb_uint32_t interval);

/**
 * @brief Get ZDD's PAN ID as it was stated in its advertisement at the moment of connection
 *
 * @param [in]  conn_id Connection id
 * @param [out] pan_id  Pointer to variable for PAN ID to be saved to
 *
 * @return Short PAN ID @warning only valid at the moment of connection callback call
 */
zb_ret_t zbd_zvd_ble_get_initial_pan_id(zb_ble_conn_id_t conn_id, zb_uint16_t* pan_id);

/**
 * @brief Get ZDD's NWK Address as it was stated in its advertisement at the moment of connection
 *
 * @param [in]  conn_id   Connection id
 * @param [out] nwk_addr  Pointer to variable for NWK Address to be saved to
 *
 * @return NWK Address @warning only valid at the moment of connection callback call
 */
zb_ret_t zbd_zvd_ble_get_initial_nwk_addr(zb_ble_conn_id_t conn_id, zb_uint16_t* nwk_addr);

/**
 * @brief Perform necessary deinitialization for BLE
 */
void zbd_zvd_ble_uninit(void);

#endif /* ZB_ZVD_BLE_H */
