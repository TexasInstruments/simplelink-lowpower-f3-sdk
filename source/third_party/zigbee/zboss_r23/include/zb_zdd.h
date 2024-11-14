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
/* PURPOSE: Zigbee Diret public API
*/
#ifndef __ZB_ZDD_H__
#define __ZB_ZDD_H__ 1

#include "zb_common.h"
#include "zboss_api_direct.h"

#define ZIGBEE_DIRECT_SECURITY_SERVICE_UUID             0xe3, 0x29, 0xb4, 0x99, 0x02, 0x6d, 0xe9, 0xbf, \
                                                        0x81, 0x44, 0x00, 0x00, 0xf4, 0x4a, 0x14, 0x29

#define ZIGBEE_DIRECT_SECURITY_PSK_P256_CHAR_UUID       0xe3, 0x29, 0xb4, 0x99, 0x02, 0x6d, 0xe9, 0xbf, \
                                                        0x81, 0x44, 0x03, 0x00, 0xf4, 0x4a, 0x14, 0x29

#define ZIGBEE_DIRECT_SECURITY_PSK_C25519_CHAR_UUID     0xe3, 0x29, 0xb4, 0x99, 0x02, 0x6d, 0xe9, 0xbf, \
                                                        0x81, 0x44, 0x01, 0x00, 0xf4, 0x4a, 0x14, 0x29

/* Used to write info packets to the dump */
#define ZIGBEE_DIRECT_SECURITY_DUMP_TRACE_CHAR_UUID     0xe3, 0x29, 0xb4, 0x99, 0x02, 0x6d, 0xe9, 0xbf, \
                                                        0x81, 0x44, 0xff, 0x00, 0xf4, 0x4a, 0x14, 0x29


#define ZIGBEE_DIRECT_TUNNEL_SERVICE_UUID               0x3f, 0x31, 0xd5, 0x8b, 0x37, 0xb2, 0x20, 0x81, \
                                                        0xf4, 0x45, 0x00, 0x00, 0xfd, 0x78, 0xd1, 0x8b

#define ZIGBEE_DIRECT_TUNNEL_CHAR_UUID                  0x3f, 0x31, 0xd5, 0x8b, 0x37, 0xb2, 0x20, 0x81, \
                                                        0xf4, 0x45, 0x01, 0x00, 0xfd, 0x78, 0xd1, 0x8b


#define ZIGBEE_DIRECT_COMM_SERVICE_UUID_SHORT           0xFFF7

#define ZIGBEE_DIRECT_COMM_SERVICE_UUID                 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, \
                                                        0x00, 0x10, 0x00, 0x00, 0xf7, 0xff, 0x00, 0x00

#define ZIGBEE_DIRECT_COMM_FORM_NWK_CHAR_UUID           0x61, 0x3a, 0x33, 0x27, 0x1c, 0x49, 0x63, 0xb1, \
                                                        0x1c, 0x42, 0x01, 0x00, 0x7d, 0x37, 0x72, 0x70

#define ZIGBEE_DIRECT_COMM_JOIN_CHAR_UUID               0x61, 0x3a, 0x33, 0x27, 0x1c, 0x49, 0x63, 0xb1, \
                                                        0x1c, 0x42, 0x02, 0x00, 0x7d, 0x37, 0x72, 0x70

#define ZIGBEE_DIRECT_COMM_PERMIT_JOIN_CHAR_UUID        0x61, 0x3a, 0x33, 0x27, 0x1c, 0x49, 0x63, 0xb1, \
                                                        0x1c, 0x42, 0x03, 0x00, 0x7d, 0x37, 0x72, 0x70

#define ZIGBEE_DIRECT_COMM_LEAVE_NWK_CHAR_UUID          0x61, 0x3a, 0x33, 0x27, 0x1c, 0x49, 0x63, 0xb1, \
                                                        0x1c, 0x42, 0x04, 0x00, 0x7d, 0x37, 0x72, 0x70

#define ZIGBEE_DIRECT_COMM_STATUS_CHAR_UUID             0x61, 0x3a, 0x33, 0x27, 0x1c, 0x49, 0x63, 0xb1, \
                                                        0x1c, 0x42, 0x05, 0x00, 0x7d, 0x37, 0x72, 0x70

#define ZIGBEE_DIRECT_COMM_MANAGE_JOINERS_CHAR_UUID     0x61, 0x3a, 0x33, 0x27, 0x1c, 0x49, 0x63, 0xb1, \
                                                        0x1c, 0x42, 0x06, 0x00, 0x7d, 0x37, 0x72, 0x70

#define ZIGBEE_DIRECT_COMM_IDENTIFICATION_UUID          0x61, 0x3a, 0x33, 0x27, 0x1c, 0x49, 0x63, 0xb1, \
                                                        0x1c, 0x42, 0x07, 0x00, 0x7d, 0x37, 0x72, 0x70

#define ZIGBEE_DIRECT_COMM_FINDING_BINDING_UUID         0x61, 0x3a, 0x33, 0x27, 0x1c, 0x49, 0x63, 0xb1, \
                                                        0x1c, 0x42, 0x08, 0x00, 0x7d, 0x37, 0x72, 0x70

#define ZIGBEE_DIRECT_ZDD_MAX_CONNECTIONS               1
#define ZIGBEE_DIRECT_MAX_CUSTOM_UUID                   13

#define ZIGBEE_DIRECT_DEFAULT_ADV_INTERVAL         64                                      /**< The advertising interval (in units of 0.625 ms; this value corresponds to 40 ms). */
#define ZIGBEE_DIRECT_DEFAULT__ADV_DURATION        BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED   /**< The advertising time-out (in units of seconds). When set to 0, we will never time out. */
#define ZIGBEE_DIRECT_DEFAULT_MIN_CONN_INTERVAL    MSEC_TO_UNITS(100, UNIT_1_25_MS)        /**< Minimum acceptable connection interval (0.5 seconds). */
#define ZIGBEE_DIRECT_DEFAULT_MAX_CONN_INTERVAL    MSEC_TO_UNITS(200, UNIT_1_25_MS)        /**< Maximum acceptable connection interval (1 second). */
#define ZIGBEE_DIRECT_DEFAULT_SLAVE_LATENCY        0                                       /**< Slave latency. */
#define ZIGBEE_DIRECT_DEFAULT_CONN_SUP_TIMEOUT     MSEC_TO_UNITS(4000, UNIT_10_MS)         /**< Connection supervisory time-out (4 seconds). */
#define ZB_DIRECT_DEFAULT_CHANNEL_MASK             ((zb_uint32_t)1U << 21)

/**
 * @brief Set NewZddProvisioningTimeout seconds (@see 6.2.2. Un-provisioned ZDD)
 *
 * @param timeout_seconds time in seconds before ZDD goes BLE-off mode
 */
void zbd_proxy_set_zdd_provisioning_timeout(zb_uint32_t timeout_seconds);

/**
 * @brief Start BLE advertising
 *
 * @param timeout_s time is seconds, for which advertising shall be enabled. If equal to 0, advertising is not limited
 *
 * @return  RET_OK    advertising successfully started
 *          RET_ERROR failed to start advertising
 */
zb_ret_t zbd_proxy_advert_start(zb_uint16_t timeout_s);

/**
 * @brief Stop BLE advertising
 *
 * @return  RET_OK    advertising successfully stopped
 *          RET_ERROR failed to stop advertising
 */
zb_ret_t zbd_proxy_advert_stop();

/**
 * @brief Stop BLE advertising
 *
 * @param pan_id    which short PAN ID shall be shown in BLE advertisement
 * @param nwk_addr  which short NWK address shall be shown in BLE advertisement
 *
 * @return  RET_OK    advertising successfully updated
 *          RET_ERROR failed to update advertising
 */
zb_ret_t zbd_proxy_advert_update(zb_uint16_t pan_id, zb_uint16_t nwk_addr);

/**
 * @brief Checks, ZDD is provisioned to Zigbee network
 *
 * @return ZB_TRUE if ZDD is provisioned, otherwise ZB_FALSE
 */
zb_bool_t zbd_is_provisioned(void);

/**
 * @brief Checks, if ZDD is currently joined to Zigbee network
 *
 * @return ZB_TRUE if ZDD is joined, otherwise ZB_FALSE
 *
 * @note  difference with @ref zbd_is_provisioned is that @ref zbd_is_provisioned will return ZB_TRUE
 *        if there is some network action in progress, @ref zbd_is_joined will not
 */
zb_bool_t zbd_is_joined(void);


/**
 * @brief Create a BLE characteristic in a service
 *
 * @param service A service descriptor returned earlier by @zbd_osif_ble_create_service
 * @param uuid128 Desired 128-bit UUID of the characteristic
 * @param buffer Pointer to a buffer for data receiving. If NULL, memory will be allocated by the stack internally
 * @param size Size of receive buffer or size of memory to be reserved by a BLE stack internally
 * @param read TRUE if the characteristic should be readable by a peer
 * @param write TRUE if the characteristic should be writable by a peer
 * @param indication TRUE if the characteristic should support an indication to a peer
 * @param notification TRUE if the characteristic should support a notification to a peer
 * @param description ASCII string with human readable characteristic name
 * @param charact_handle A handle of the new characteristic to be used as reference further
 * @return zb_ret_t Error code. For now only RET_OK can be returned.
 */
zb_ret_t zb_zdd_create_user_characteristic(zb_ble_service_handle_t service,
                                           const zb_uint8_t *uuid128,
                                           zb_uint8_t *buffer,
                                           zb_uint16_t size,
                                           zbd_characteristic_properties_t properties,
                                           const char *description,
                                           zb_ble_characteristic_handle_t *charact_handle);

/**
 * @brief Create a BLE new characteristic in a service
 *
 * @param service A service descriptor returned earlier by @zbd_osif_ble_create_service
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
zb_ret_t zb_zdd_create_user_characteristic_short_id(zb_ble_service_handle_t service,
                                                    zb_uint16_t uuid16,
                                                    zb_uint8_t *buffer,
                                                    zb_uint16_t size,
                                                    zbd_characteristic_properties_t properties,
                                                    const char *description,
                                                    zb_ble_characteristic_handle_t *charact_handle);

/**
 * @brief Set user's callback for new BLE connect handling
 *
 * @param callback A callback
 */
void zbd_ble_set_user_connect_callback(zbd_zdd_simple_callback_t callback);

/**
 * @brief Set user's callback for new BLE disconnect handling
 *
 * @param callback  A callback
 */
void zbd_ble_set_user_disconnect_callback(zbd_zdd_simple_callback_t callback);

zb_ret_t zbd_indicate_tunnel(zb_bufid_t req_payload_buf, zb_ieee_addr_t dst_addr);

/**
 * @brief Perform zigbee network opening/closing (locally and network-wide)
 */
void zbd_permit_join_request(zb_uint16_t time);


/**
 * @brief Set if joiners shall be restricted to those only,
 * which were introduced via Manage Joiners
 *
 * @param restrict_joiners  if ZB_TRUE, ZDD will not authorize Joiners,
 *                          which security materials were not provided
 *                          via Manage Joiners characteristic
 */
void zbd_zdd_restrict_joiners(zb_bool_t restrict_joiners);

void zb_zdd_set_admin_key(const zb_uint8_t admin_key[ZB_CCM_KEY_SIZE]);

/**
 * @brief Set a ZDD BLE address
 *
 * @param ble_address Pointer to six-byte BLE address
 * @param is_public Flag shows whether the address is public or static random
 *
 * @return RET_OK if address was set, RET_ERROR otherwise
 */
zb_ret_t zbd_proxy_ble_addr_set(const zb_uint8_t* ble_address, zb_bool_t is_public);

#endif //__ZB_ZDD_H__
