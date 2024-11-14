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
/*  PURPOSE: Public ZB Direct API
*/
#ifndef ZBOSS_API_DIRECT_H
#define ZBOSS_API_DIRECT_H 1

#include "zb_types.h"
#include "zb_direct_ble_platform.h"

/** \addtogroup ZB_DIRECT_COMMON_API
 * @{
 */

#define ZIGBEE_DIRECT_DEFAULT_PROXY_NAME "DSR ZDD" /**< Name of device. Will be included in the advertising data. */
#define ZBD_BLE_CONN_ID_INVALID     (zb_ble_conn_id_t)(-1)

typedef zb_uint16_t zbd_data_len_t;

/** @brief Return codes for callback functions */
typedef enum zbd_zdd_recv_callback_ret_e {
  ZBD_ZDD_RECV_RET_OK = 0,
  ZBD_ZDD_RECV_RET_ERROR,
  ZBD_ZDD_RECV_RET_DECRYPTION_FAIL,
  ZBD_ZDD_RECV_RET_UNAUTHORIZED,
  ZBD_ZDD_RECV_RET_PROCEDURE_ALREADY_IN_PROGRESS,
} zbd_zdd_recv_callback_ret_t;

/**
 * @brief Callback type that is being used on new data receiving
 *
 * @return BLE GATT Status code
 *
 * @note The callback is being called from user space (not from ISR)
 */
typedef zbd_zdd_recv_callback_ret_t (*zbd_zdd_recv_callback_t)(
    zb_ble_conn_id_t conn_id,
    zb_ble_characteristic_handle_t charact_handle,
    const zb_uint8_t* data,
    zbd_data_len_t len);

/**
 * @brief Callback type that is being called after data sending
 *
 * @note The callback is being called from user space (not from ISR)
 */
typedef void (*zbd_zdd_send_callback_t)(
    zb_ble_conn_id_t conn_id,
    zb_ble_characteristic_handle_t charact_handle,
    zb_bool_t success);

/**
 * @brief All other callback types like new BLE device connected, etc.
 *
 * @note The callback is being called from user space (not from ISR)
 */
typedef void (*zbd_zdd_simple_callback_t)(zb_ble_conn_id_t conn_id);

/**
 * @brief Callback type that is being used on peer subscribing or unsubscribing
 *
 * @note The callback is being called from BLE thread/ISR
 */
typedef void (*zbd_zdd_subscribe_callback_t)(
    zb_ble_conn_id_t conn_id,
    zb_ble_characteristic_handle_t charact_handle,
    zb_bool_t is_subscribed);

typedef enum zbd_zdd_data_request_callback_ret_e {
    ZBD_ZDD_DATA_REQUEST_RET_OK = 0,
    ZBD_ZDD_DATA_REQUEST_RET_ERROR_INVALID_PARAM,
    ZBD_ZDD_DATA_REQUEST_RET_ERROR_ENCRYPTION_FAIL,
    ZBD_ZDD_DATA_REQUEST_RET_ERROR_UNAUTHORIZED,
    ZBD_ZDD_DATA_REQUEST_RET_ERROR_PROCEDURE_ALREADY_IN_PROGRESS,
} zbd_zdd_data_request_callback_ret_t;

/**
 * @brief Callback used to write to the value of a characteristic right before
 * sending data to the peer that executed a read operation.
 *
 * @param [in]      conn_id        Connection ID
 * @param [in]      charact_handle Characteristic handle returned on creation
 * @param [out]     data           Pointer to the buffer where the result will be stored
 * @param [in]   len
 *                                  - <tt>[in]</tt> Size of the data buffer in bytes
 *                                  - <tt>[out]</tt> Length of the byte array written to the data buffer
 * @return zbd_zdd_data_request_callback_ret_t Return code from data request callback
 */
typedef zbd_zdd_data_request_callback_ret_t (*zbd_zdd_data_request_callback_t) (zb_ble_conn_id_t conn_id,
                                                                                zb_ble_characteristic_handle_t charact_handle,
                                                                                zb_uint8_t      *data,
                                                                                zbd_data_len_t  *len);

/* Characteristic properties */
typedef enum zbd_characteristic_properties_e
{
  ZBD_CHAR_PROP_READ      = 1 << 0,
  ZBD_CHAR_PROP_WRITE     = 1 << 1,
  ZBD_CHAR_PROP_INDICATE  = 1 << 2,
  ZBD_CHAR_PROP_NOTIFY    = 1 << 3
} zbd_characteristic_properties_t;

/**
 * @brief Create a BLE service with a custom 128-bit UUID
 *
 * @param uuid128 Pointer to 16-bytes array contatining desired UUID
 * @param [out] service_handle pointer to variable to store handle of created service in
 * @return RET_OK if service was successfully created, RET_ERROR otherwise
 */
zb_ret_t zbd_ble_create_service(const zb_uint8_t* uuid128, zb_ble_service_handle_t *service_handle);

/**
 * @brief Create a BLE service with a short 16-bit UUID
 *
 * @param uuid16 Desired 16-bit UUID
 * @param service_handle pointer to variable to store handle of created service in
 * @return RET_OK if service was successfully created, RET_ERROR otherwise
 */
zb_ret_t zbd_ble_create_service_short_uuid(zb_uint16_t uuid16, zb_ble_service_handle_t *service_handle);

/**
 * @brief Get current MTU size for particular BLE connection
 *
 * @param connection_id Connection ID
 * @return zb_uint8_t Negotiated BLE MTU size
 */
zb_uint8_t zbd_ble_get_curr_mtu(zb_ble_conn_id_t connection_id);

/**
 * @brief Request an indication sending to one peer
 *
 * @param conn_id ID of a connection where to send the indication to
 * @param charact_handle Characteristic handle returned on creation
 * @param data Pointer to a data block to be sent
 * @param len Size of the data
 * @return zb_ret_t
 *          - RET_OK on success
 *          - RET_BLOCKED if another indication is in progress now
 *          - RET_ERROR on internal stack error
 *
 * @note Keep in mind that the function doesn't actually send the data. It makes an
 * indication request instead and a real sending will be handled by a BLE stack a bit later.
 * This function doesn't copy the data to internal buffers. It stores the data pointer internally, so
 * you shouldn't change the data in the buffer until send_callback will be called.
 */
zb_ret_t zbd_ble_indicate(zb_ble_conn_id_t conn_id, zb_ble_characteristic_handle_t charact_handle, const zb_uint8_t* data, zbd_data_len_t len);

/**
 * @brief Request a notification for a specific subscribed peer
 *
 * @param conn_id ID of a connection where to send the notification to
 * @param charact_handle Characteristic handle returned on creation
 * @param data Pointer to a data block to be sent
 * @param len Size of the data
 * @return zb_ret_t
 *          - RET_ON on success
 *          - RET_ERROR on internal stack error
 *
 * @note Keep in mind that the function doesn't actually send the data. It makes a
 * notification request instead and a real sending will be handled by a BLE stack a bit later.
 * This function doesn't copy the data to internal buffers. It stores the data pointer internally, so
 * you shouldn't change the data in the buffer until notif_send_callback will be called.
 */
zb_ret_t zbd_ble_notify_peer(zb_ble_conn_id_t conn_id, zb_ble_characteristic_handle_t charact_handle, const zb_uint8_t* data, zbd_data_len_t len);

/**
 * @brief Set receiving callback for user defined characteristics
 *
 * @param callback A callback
 */
void zbd_ble_set_user_recv_callback(zbd_zdd_recv_callback_t callback);

/**
 * @brief Set data request callback for user defined characteristics
 *
 * @param callback A callback
 */
void zbd_ble_set_user_data_request_callback(zbd_zdd_data_request_callback_t callback);

/**
 * @brief Set sending callback for user defined characteristics
 *
 * @param callback A callback
 */
void zbd_ble_set_user_send_callback(zbd_zdd_send_callback_t callback);

/**
 * @brief Initialize ZigBee Direct functionality
 *
 * @param device_name BLE device name
 * @param identification_ep endpoint number to to perform ZBD identifiation on
 *
 * @return RET_OK if ZigBee Direct Proxy Device was initialized correctly, RET_ERROR otherwise
 */
zb_ret_t zbd_proxy_init(const zb_char_t* device_name, zb_uint8_t identification_ep);

/**
 * @brief Uninitialize ZDD
 */
void zbd_zdd_uninit(void);

/**
 * @brief Enable BLE interface of the device, saves state to NVRAM
 *
 * @return  RET_OK    interface was successfully enabled
 *          RET_BUSY  interface was already scheduled to disconnect, dismiss enabling
 *          RET_ERROR failure during interface enabling
 */
zb_ret_t zbd_proxy_enable_ble_interface(void);

/**
 * @brief Disable BLE interface of the device, saves state to NVRAM
 *
 * @param disconnect_from_peers whether or not shall ZDD perform a disconnection from every ZVD before disabling BLE interface
 *
 * @return  RET_OK    interface was successfully disabled
 *          RET_BUSY  interface was already scheduled to disconnect, dismiss disabling
 *          RET_ERROR failed to stop advertising
 */
zb_ret_t zbd_proxy_disable_ble_interface(zb_bool_t disconnect_from_peers);

/**
 * @brief Check if BLE interface is enabled or not
 *
 * @return ZB_TRUE if BLE interface is enabled, otherwise ZB_FALSE
 */
zb_bool_t zbd_proxy_is_ble_interface_enabled(void);

/**
 * @brief Retrieves ZDD's 6-bytes bluetooth address
 *
 * @param ble_address [OUT] pointer to 6-bytes array to store the address
 *
 * @return  RET_OK            address was successfully retrieved
 *          RET_ERROR         failed to retrieve address
 *          RET_UNINITIALIZED BLE stack was not initialized yet
 */
zb_ret_t zbd_proxy_ble_addr_get(zb_uint8_t* ble_address);

#ifndef ZB_MINIMAL_CONTEXT

/**
 * @brief Generates EUI-64 address from MAC address
 *
 * @param [in]  mac_address MAC address, byte array of length 6, position 0 is the MSB and
 *                          position 5 is the LSB
 * @param [out] eui_64      Generated EUI-64 address, byte array of length 8, position 0
 *                          is the LSB and position 7 is the MSB
 * @return zb_ret_t
 *          - RET_OK on success
 *          - RET_INVALID_PARAMETER if NULL pointers are used
 */
zb_ret_t zbd_generate_eui64_from_mac_address(const zb_uint8_t *mac_address, zb_ieee_addr_t eui_64);

#endif /* !ZB_MINIMAL_CONTEXT */

/**
 * @brief Establishes Trusted Link between ZVD and connected ZDD
 * The function is intended to initiate ZVD TC and ZDD commissioning process
 * (ZVD TC sends Network Commissioning Request and waits for Network Commissioning Response)
 *
 * @param buffer buffer id to establish trusted link
 * @param conn_id connection id between ZVD and ZDD
 */
void zbd_zvd_establish_trusted_link(zb_uint8_t buffer, zb_uint16_t conn_id);

/**
 * @brief Retrieves the BLE device address.
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
 * ZVD API
 */

/**
 * @brief This struct represents a remote Zigbee Direct Device to the ZVD API
 */
typedef struct zb_peripheral_zdd_t {
    zb_int16_t                          rssi;
    zb_uint16_t                         pan_id;
    zb_uint16_t                         nwk_addr;
    zb_uint8_t                          mac_addr[6];
    zb_uint8_t                          ieee_addr[8];
    char                                name[64];
    zb_uint8_t                          rssi_valid;
    zb_uint8_t                          connected;
    zb_uint8_t                          ble_adv_version  : 4;
    zb_uint8_t                          capability_flags : 4;
} ZB_PERIPHERAL_ZDD_T;


typedef struct zbd_network_status_s {
    zb_uint8_t join_status : 3;
    zb_uint8_t opened : 1;
    zb_uint8_t nwk_centralized : 1;
    zb_uint8_t : 3;
} zbd_network_status_t;


/* Must fit 1 byte */
ZB_ASSERT_COMPILE_DECL(sizeof(zbd_network_status_t) == sizeof(zb_uint8_t));



/* Domains for Status Code TLV (7.7.2.4.6) */
typedef enum zbd_status_domain_e
{
  ZBD_STATUS_DOMAIN_GENERAL         = 0x00,
  ZBD_STATUS_DOMAIN_FORM            = 0x01,
  ZBD_STATUS_DOMAIN_JOIN            = 0x02,
  ZBD_STATUS_DOMAIN_PERMIT_JOIN     = 0x03,
  ZBD_STATUS_DOMAIN_LEAVE           = 0x04,
  ZBD_STATUS_DOMAIN_MANAGE_JOINERS  = 0x05,
  ZBD_STATUS_DOMAIN_IDENTIFY        = 0x06,
  ZBD_STATUS_DOMAIN_FINDING_BINDING = 0x07,
  ZBD_STATUS_DOMAIN_MAX             = 0x08
} zbd_status_domain_t;

/* Codes for Status Code TLV (7.7.2.4.6) */
typedef enum zbd_status_code_value_e
{
  ZBD_STATUS_CODE_SUCCESS = 0,
  ZBD_STATUS_CODE_FAIL    = 1,
  ZBD_STATUS_CODE_MAX     = 2
  /** TODO: add more detailed codes, as described in 7.7.2.4.6. Status Code TLV */
} zbd_status_code_value_t;


/* Structure for Status Code TLV payload */
typedef ZB_PACKED_PRE struct zbd_status_code_s
{
  zb_uint8_t     domain;
  zb_uint8_t     code;
} ZB_PACKED_STRUCT zbd_status_code_t;

#define MAX_NWK_CHANNEL_LIST_PAGES  20

typedef struct zbd_nwk_channel_list_s {
    zb_uint8_t  page_count;                         //!< Channel page count in channel list
    zb_uint32_t pages[MAX_NWK_CHANNEL_LIST_PAGES];  //!< Channel pages array
} zbd_nwk_channel_list_t;


/**
 * @brief Commissioning Service request parameters
 */
typedef struct zbd_zvd_comm_service_params_t {
    zb_uint32_t                         param_mask;                 //!< Parameter mask (used bits 0 to 16) - indicates what parameters of the request should be considered - ::zbd_zvd_comm_service_param_bitmasks_t
    zb_uint8_t                          ext_pan_id[8];              //!< [00] Specifies the Zigbee network's extended PAN ID - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_TZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_EXT_PAN_ID
    zb_uint16_t                         short_pan_id;               //!< [01] Specifies the Zigbee network's short PAN ID - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_SHORT_PAN_ID
    zbd_nwk_channel_list_t              nwk_channel_list;           //!< [02] Network Channel List - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_NWK_CHANNEL
    zb_uint8_t                          nwk_key[16];                //!< [03] Specifies the Zigbee Network Key - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_NWK_KEY
    zb_uint8_t                          link_key[16];               //!< [04] Specifies the Zigbee Link Key - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_LINK_KEY
    zb_nwk_device_type_t                device_type;                //!< [05] Specifies the ZDD's Zigbee device type - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_DEVICE_TYPE
    zb_uint16_t                         nwk_addr;                   //!< [06] Specifies the ZDD's 16-bit Zigbee network address - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_NWK_ADDR
    zb_uint8_t                          joining_method;             //!< [07] Specifies the method the ZDD is instructed to use to join the network - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_JOINING_METHOD
    zb_uint8_t                          ieee_addr[8];               //!< [08] The 64-bit IEEE address of the ZDD - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_IEEE_ADDR
    zb_uint8_t                          tc_addr[8];                 //!< [09] The 64-bit IEEE address of the Trust Center - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_TC_ADDR
    zbd_network_status_t                nwk_status_map;             //!< [10] Contains a bitmap that indicates the ZDD's network status - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_NWK_STATUS_MAP
    zb_uint8_t                          nwk_update_id;              //!< [11] Current value of the nwkUpdateId attribute of the Zigbee NIB - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_NWK_UPDATE_ID
    zb_uint8_t                          nwk_active_key_seq_num;     //!< [12] Current value of the nwkActiveKeySeqNumber attribute of the Zigbee NIB - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_NWK_ACTIVE_KEY_SEQ_NUM
    zb_uint8_t                          admin_key[16];              //!< [13] Specifies the 128-bit Admin Key - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_ADMIN_KEY
    zbd_status_code_t                   status_code;                //!< [14] Status codes as described in section 7.7.2.4.6 - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_STATUS_CODE , ::zbd_status_code_s
    zb_uint8_t                          manage_joiners_cmd;         //!< [15] Manage Joiners Command, as described in section 7.7.2.4.7 - ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_MANAGE_JOINERS_CMD , see zbd_manage_joiners_cmd_e
    zb_uint8_t                          manage_joiners_ieee[8];     //!< [16] The 64-bit IEEE address of the joiner ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_MANAGE_JOINERS_IEEE , see zbd_manage_joiners_cmd_e
    zb_uint8_t                          manage_joiners_lnk_key[16]; //!< [17] Specifies the Zigbee Link Key associated with the joiner ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T::ZBD_ZVD_COMM_SERVICE_PARAM_MANAGE_JOINERS_LNK_KEY , see zbd_manage_joiners_cmd_e
    zb_uint8_t                          link_key_flags_mask;        //!< [18] Link Key flags ::zbd_aps_link_key_flags_mask_t
} ZBD_ZVD_COMM_SERVICE_PARAMS_T;

/**
 * @brief Operation error codes
 */
typedef enum zbd_zvd_operation_error_t {
    ZBD_ZVD_OPERATION_OK                                    =  0,
    ZBD_ZVD_OPERATION_ERROR_READ                            = -1,
    ZBD_ZVD_OPERATION_ERROR_WRITE                           = -2,
    ZBD_ZVD_OPERATION_ERROR_UNKNOWN                         = -3,
} ZBD_ZVD_OPERATION_ERROR_T;

/**
 * @brief Commissioning Service parameter masks
 */
typedef enum zbd_zvd_comm_service_param_bitmasks_t {
    ZBD_ZVD_COMM_SERVICE_PARAM_EXT_PAN_ID                   = (1 << 0),
    ZBD_ZVD_COMM_SERVICE_PARAM_SHORT_PAN_ID                 = (1 << 1),
    ZBD_ZVD_COMM_SERVICE_PARAM_NWK_CHANNEL                  = (1 << 2),
    ZBD_ZVD_COMM_SERVICE_PARAM_NWK_KEY                      = (1 << 3),
    ZBD_ZVD_COMM_SERVICE_PARAM_LINK_KEY                     = (1 << 4),
    ZBD_ZVD_COMM_SERVICE_PARAM_DEVICE_TYPE                  = (1 << 5),
    ZBD_ZVD_COMM_SERVICE_PARAM_NWK_ADDR                     = (1 << 6),
    ZBD_ZVD_COMM_SERVICE_PARAM_JOINING_METHOD               = (1 << 7),
    ZBD_ZVD_COMM_SERVICE_PARAM_IEEE_ADDR                    = (1 << 8),
    ZBD_ZVD_COMM_SERVICE_PARAM_TC_ADDR                      = (1 << 9),
    ZBD_ZVD_COMM_SERVICE_PARAM_NWK_STATUS_MAP               = (1 << 10),
    ZBD_ZVD_COMM_SERVICE_PARAM_NWK_UPDATE_ID                = (1 << 11),
    ZBD_ZVD_COMM_SERVICE_PARAM_NWK_ACTIVE_KEY_SEQ_NUM       = (1 << 12),
    ZBD_ZVD_COMM_SERVICE_PARAM_ADMIN_KEY                    = (1 << 13),
    ZBD_ZVD_COMM_SERVICE_PARAM_STATUS_CODE                  = (1 << 14),
    ZBD_ZVD_COMM_SERVICE_PARAM_MANAGE_JOINERS_CMD           = (1 << 15),
    ZBD_ZVD_COMM_SERVICE_PARAM_MANAGE_JOINERS_IEEE          = (1 << 16),
    ZBD_ZVD_COMM_SERVICE_PARAM_MANAGE_JOINERS_LNK_KEY       = (1 << 17)
} ZBD_ZVD_COMM_SERVICE_PARAM_BITMASKS_T;


/* 7.7.2.4.5. Network Status Map TLV / Table 14 */
typedef enum joined_status_e {
    JOINED_STATUS_NOT_COMMISSIONED          = 0,
    JOINED_STATUS_COMMISSIONING_IN_PROGRESS = 1,
    JOINED_STATUS_COMMISSIONED              = 2
} joined_status_t;

typedef enum zbd_characteristic_tag_e {
    ZBD_SECURITY_P256_CHAR,
    ZBD_SECURITY_C25519_CHAR,
    ZBD_COMM_NWK_FORM_CHAR,
    ZBD_COMM_NWK_JOIN_CHAR,
    ZBD_COMM_NWK_PERMIT_JOIN_CHAR,
    ZBD_COMM_NWK_LEAVE_CHAR,
    ZBD_COMM_NWK_STATUS_CHAR,
    ZBD_COMM_MANAGE_JOINERS_CHAR,
    ZBD_COMM_IDENTIFICATION_CHAR,
    ZBD_COMM_FINDING_BINDING_CHAR,
    ZBD_TUNNEL_CHAR,
    ZBD_DUMP_CHAR,
    ZBD_USER_CHAR,
    ZBD_CHAR_LAST
} zbd_characteristic_tag_t;

typedef enum zbd_device_type_e {
    ZBD_DEVICE_TYPE_COORDINATOR = 0,
    ZBD_DEVICE_TYPE_ROUTER,
    ZBD_DEVICE_TYPE_ED
} zbd_device_type_t;

/* Joining Method TLV (7.7.2.4.4) */
typedef enum zbd_joining_method_e {
    ZBD_JOINING_METHOD_MAC_ASSOCIATION = 0,
    ZBD_JOINING_METHOD_REJOIN          = 1,
    ZBD_JOINING_METHOD_OUT_OF_BAND    = 2,
    ZBD_JOINING_METHOD_RESERVED_MAX = 3
} zbd_joining_method_t;

typedef enum zbd_manage_joiners_cmd_e {
    ZBD_MANAGE_JOINERS_CMD_DROP_ALL_JOINERS = 0x00,
    ZBD_MANAGE_JOINERS_CMD_ADD_JOINER       = 0x01,
    ZBD_MANAGE_JOINERS_CMD_REMOVE_JOINER    = 0x02
} zbd_manage_joiners_cmd_t;


/**
 * @brief Callback type used when ZDD notifies a commissioning service status update
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  params          ZVD Commissioning Service parameters
 */
typedef void (*zbd_zvd_comm_status_update_callback_t)(zb_ble_conn_id_t conn_id, const ZBD_ZVD_COMM_SERVICE_PARAMS_T *params);

/**
 * @brief Callback type used when a ZDD operation (characteristic read or write) returns with an error code
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  operation_error Operation error code
 */
typedef void (*zbd_zvd_operation_return_callback_t)(zb_ble_conn_id_t conn_id, ZBD_ZVD_OPERATION_ERROR_T operation_error);

/**
 * @brief Callback type used when the secure session establishment between ZVD and ZDD is done
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  success         true if the secure session was successfully established; false otherwise
 * @param [in]  zdd_ieee_addr   zdd ieee address
 */
typedef void (*zbd_zvd_security_establish_secure_session_callback_t)(zb_ble_conn_id_t conn_id, zb_bool_t success, const zb_uint8_t *zdd_ieee_addr);



/**
 * @brief   Callback type that is being used when there is an update to the discovered peripheral list
 *
 */
typedef void (*zbd_zvd_periph_list_updated_callback_t)                                 (void);

/**
 * @brief   All other callback types
 *
 * @param [in]  conn_id     Connection ID
 */
typedef void (*zbd_zvd_simple_callback_t)                                              (zb_ble_conn_id_t     conn_id);

/**
 * @brief   Set a callback to be called when the discovered peripheral list is updated
 *
 * @param   callback        A callback
 */
void zbd_zvd_ble_set_periph_list_updated_callback(zbd_zvd_periph_list_updated_callback_t callback);

/**
 * @brief   Set a callback to be called when a new connection to a peer must be handled
 *
 * @param   callback        A callback
 */
void zbd_zvd_ble_set_connect_callback(zbd_zvd_simple_callback_t callback);

/**
 * @brief   Set a callback to be called when a peer is disconnected
 *
 * @param   callback        A callback
 */
void zbd_zvd_ble_set_disconnect_callback(zbd_zvd_simple_callback_t callback);

/**
 * @brief   Set a callback to be called when a ZDD services have been resolved and can be used by ZVD
 *
 * @param   callback        A callback
 */
void zbd_zvd_ble_set_services_resolved_callback(zbd_zvd_simple_callback_t callback);


/**
 * @brief   Initialize ZVD, initializing BLE stack as a Central and ZigBee stack
 *
 * @param   device_name         Zero-terminated ASCII string representing device's name
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_init(const zb_char_t* device_name);

/**
 * @brief   Starts discovering ZDD devices in range.
 *
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_discover_zdd(void);

/**
 * @brief   Stops discovering ZDD devices in range.
 *
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_stop_discovery(void);

/**
 * @brief   Get the updated list of discovered peripherals
 *
 * @param   list            Updated list of discovered peripherals
 * @param   max_count
 *                              - <tt>[in]</tt> Size of the list
 *                              - <tt>[out]</tt> Length of the returned list
 *
 */
zb_uint32_t zbd_zvd_get_discovered_peripherals_list(ZB_PERIPHERAL_ZDD_T *list, zb_uint32_t max_count);

/**
 * @brief   Connects to a designated ZDD in range using its BLE address
 *
 * @param   ble_address         Pointer to 6-bytes array of the ZDD BLE address
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_connect(const zb_uint8_t* ble_address);

/**
 * @brief   Disconnects from ZDD, if it is already connected to one
 *
 * @param [in]  conn_id         Connection ID
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_disconnect(zb_ble_conn_id_t conn_id);

/**
 * @brief Set a callback to be called when ZVD connects to a ZDD
 *
 * @param [in]  callback        A callback
 */
void zbd_zvd_set_connect_callback(zbd_zvd_simple_callback_t callback);

/**
 * @brief Set a callback to be called when ZDD's services are resolved
 *
 * @param [in]  callback        A callback
 */
void zbd_zvd_set_services_resolved_callback(zbd_zvd_simple_callback_t callback);

/**
 * @brief Set a callback to be called when ZVD disconnects from a ZDD
 *
 * @param [in]  callback        A callback
 */
void zbd_zvd_set_disconnect_callback(zbd_zvd_simple_callback_t callback);

/**
 * @brief Set a callback to be called when the list of ZDD peripherals in range has been updated
 *
 * @param [in]  callback        A callback
 */
void zbd_zvd_set_zdd_list_updated_callback(zbd_zvd_periph_list_updated_callback_t callback);

/**
 * @brief Uninitialize ZVD
 */
void zbd_zvd_uninit(void);


/* --- Commissioning service ------------------------------------------------------------------- */

/**
 * @brief   Get currently cached commissioning service parameters corresponding to ZDD at specified connection ID
 *
 * @param   [in]    conn_id Connection ID
 *
 * @return Pointer to a structure with commissioning service parameters or NULL, if conn_id is not valid
 */
const ZBD_ZVD_COMM_SERVICE_PARAMS_T * zbd_zvd_comm_get_params(zb_ble_conn_id_t conn_id);


/**
 * @brief   Request ZDD to form a Zigbee network
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  request         Commissioning service request parameters
 *                                  - Required parameters:
 *                                      - admin_key (only for centralized == false)
 *                                      - tc_addr (only for centralized == false)
 *                                  - Optional parameters:
 *                                      - ext_pan_id
 *                                      - short_pan_id
 *                                      - nwk_channel
 *                                      - nwk_key
 *                                      - link_key
 *                                      - nwk_addr
 *                                      - nwk_update_id
 *                                      - nwk_active_key_seq_num
 *                                  - Invalid parameters:
 *                                      - device_type
 *                                      - joining_method
 *                                      - ieee_addr
 *                                      - nwk_status_map
 *                                      - status_code
 * @param [in]  centralized     Indicates if the requested Zigbee network should have centralized or distributed security
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_form_network(zb_ble_conn_id_t conn_id, const ZBD_ZVD_COMM_SERVICE_PARAMS_T *request, zb_bool_t centralized);

/**
 * @brief   Request ZDD to permit/prohibit joining of other devices in the ZigBee network
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  permit_duration Time, in seconds, during which ZigBee router or coordinator will allow associations.
 *                                  - 0x00: closes the network
 *                                  - 0x01 to 0xFE: opens the network with the specified time limit (in seconds)
 *                                  - 0xFF: same value as 0xFE
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_permit_joining(zb_ble_conn_id_t conn_id, zb_uint8_t permit_duration);

/**
 * @brief   Request ZDD to join a ZigBee network via MAC association
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  request         Commissioning service request parameters
 *                                  - Required parameters:
 *                                      - joining_method
 *                                  - Optional parameters:
 *                                      - admin_key
 *                                      - nwk_channel
 *                                      - ext_pan_id
 *                                      - short_pan_id
 *                                      - link_key
 *                                      - tc_addr
 *                                      - nwk_addr
 *                                  - Invalid parameters:
 *                                      - nwk_key
 *                                      - nwk_update_id
 *                                      - nwk_active_key_seq_num
 *                                      - device_type
 *                                      - ieee_addr
 *                                      - nwk_status_map
 *                                      - status_code
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_join_network_mac_association(zb_ble_conn_id_t conn_id, const ZBD_ZVD_COMM_SERVICE_PARAMS_T *request);

/**
 * @brief   Request ZDD to join a ZigBee network via NWK rejoin with key
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  request         Commissioning service request parameters
 *                                  - Required parameters:
 *                                      - joining_method
 *                                  - Optional parameters:
 *                                      - admin_key
 *                                      - nwk_channel
 *                                      - short_pan_id
 *                                      - tc_addr
 *                                  - Invalid parameters:
 *                                      - ext_pan_id
 *                                      - link_key
 *                                      - nwk_addr
 *                                      - nwk_key
 *                                      - nwk_update_id
 *                                      - nwk_active_key_seq_num
 *                                      - device_type
 *                                      - ieee_addr
 *                                      - nwk_status_map
 *                                      - status_code
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_join_network_nwk_rejoin_with_key(zb_ble_conn_id_t conn_id, const ZBD_ZVD_COMM_SERVICE_PARAMS_T *request);

/**
 * @brief   Request ZDD to join a ZigBee network via out-of-band commissioning
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  request         Commissioning service request parameters
 *                                  - Required parameters:
 *                                      - joining_method
 *                                      - ext_pan_id
 *                                      - short_pan_id
 *                                      - nwk_channel
 *                                      - nwk_key
 *                                  - Optional parameters:
 *                                      - admin_key
 *                                      - link_key
 *                                      - tc_addr
 *                                      - nwk_addr
 *                                      - nwk_update_id
 *                                      - nwk_active_key_seq_num
 *                                  - Invalid parameters:
 *                                      - device_type
 *                                      - ieee_addr
 *                                      - nwk_status_map
 *                                      - status_code
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_join_network_out_of_band_comm(zb_ble_conn_id_t conn_id, const ZBD_ZVD_COMM_SERVICE_PARAMS_T *request);

/**
 * @brief   Request ZDD to leave the network it is currently connected to
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  remove_children Set this to TRUE to ask ZDD to also remove its child devices, if any. Otherwise, set to FALSE
 * @param [in]  rejoin          Set this parameter to TRUE to ask ZDD to rejoin the network. Otherwise, set to FALSE
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_leave_network(zb_ble_conn_id_t conn_id, zb_bool_t remove_children, zb_bool_t rejoin);

/**
 * @brief   Reads Commissioning Status value. The result will be returned via callback that was registered
 *          with @ref zbd_zvd_comm_set_status_update_callback
 *
 * @param [in]  conn_id         Connection ID
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_read_commissioning_status(zb_ble_conn_id_t conn_id);

/**
 * @brief Set a callback to be called when ZDD notifies a commissioning service status update
 *
 * @param [in]  callback        A callback
 */
void zbd_zvd_comm_set_status_update_callback(zbd_zvd_comm_status_update_callback_t callback);

/**
 * @brief Set a callback to be called when an operation (characteristic read/write) returns
 *
 * @param [in]  callback        A callback
 */
void zbd_zvd_set_operation_return_callback(zbd_zvd_operation_return_callback_t callback);

/**
 * @brief   Request ZDD to drop all joiners provisional link keys from its database
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  request         Commissioning service request parameters
 *                                  - Required parameters:
 *                                      - manage_joiners_cmd
 *                                  - Invalid parameters:
 *                                      - ieee_addr
 *                                      - link_key
 *                                      - device_type
 *                                      - nwk_status_map
 *                                      - status_code
 *                                      - joining_method
 *                                      - ext_pan_id
 *                                      - short_pan_id
 *                                      - nwk_channel
 *                                      - nwk_key
 *                                      - admin_key
 *                                      - link_key
 *                                      - tc_addr
 *                                      - nwk_addr
 *                                      - nwk_update_id
 *                                      - nwk_active_key_seq_num
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_drop_all_joiners(zb_ble_conn_id_t conn_id, const ZBD_ZVD_COMM_SERVICE_PARAMS_T *request);

/**
 * @brief   Request ZDD to add a joiner provisional link key to its database (indexed by IEEE address)
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  request         Commissioning service request parameters
 *                                  - Required parameters:
 *                                      - manage_joiners_cmd
 *                                      - ieee_addr
 *                                      - link_key
 *                                  - Invalid parameters:
 *                                      - device_type
 *                                      - nwk_status_map
 *                                      - status_code
 *                                      - joining_method
 *                                      - ext_pan_id
 *                                      - short_pan_id
 *                                      - nwk_channel
 *                                      - nwk_key
 *                                      - admin_key
 *                                      - link_key
 *                                      - tc_addr
 *                                      - nwk_addr
 *                                      - nwk_update_id
 *                                      - nwk_active_key_seq_num
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_add_joiner(zb_ble_conn_id_t conn_id, const ZBD_ZVD_COMM_SERVICE_PARAMS_T *request);

/**
 * @brief   Request ZDD to remove a joiner provisional link key from its database (indexed by IEEE address)
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  request         Commissioning service request parameters
 *                                  - Required parameters:
 *                                      - manage_joiners_cmd
 *                                      - ieee_addr
 *                                  - Invalid parameters:
 *                                      - link_key
 *                                      - device_type
 *                                      - nwk_status_map
 *                                      - status_code
 *                                      - joining_method
 *                                      - ext_pan_id
 *                                      - short_pan_id
 *                                      - nwk_channel
 *                                      - nwk_key
 *                                      - admin_key
 *                                      - link_key
 *                                      - tc_addr
 *                                      - nwk_addr
 *                                      - nwk_update_id
 *                                      - nwk_active_key_seq_num
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_remove_joiner(zb_ble_conn_id_t conn_id, const ZBD_ZVD_COMM_SERVICE_PARAMS_T *request);

/**
 * @brief   Generates a 16-byte provisional link key for a joiner ZDD
 *
 * @param [out] prov_link_key   Generated joiner's provisional link key, a 16-byte array
 * @param [in]  length          Length of the output buffer pointed by prov_link_key
 * @return zb_ret_t
 *              - RET_OK on success
 *              - RET_INVALID_PARAMETER if prov_link_key is NULL or if length is not 16
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_generate_joiner_prov_link_key(zb_uint8_t *prov_link_key, zb_uint32_t length);

/**
 * @brief   Sends ZDD a signal to start/stop identifying itself
 *
 * @param [in]  conn_id Connection ID
 * @param [in]  time    Time to identify. If equal to zero then identification stops
 * @return zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_identify(zb_ble_conn_id_t conn_id, zb_uint16_t time);

/**
 * @brief   Request ZDD to start finding & binding procedure
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  endpoint        Initiator/Target endpoint number
 * @param [in]  initiator       Set this parameter to TRUE to request ZDD to start Finding & Binding
 *                              as the initiator endpoint; or FALSE to start as the target endpoint
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 */
zb_ret_t zbd_zvd_comm_start_finding_and_binding(zb_ble_conn_id_t conn_id, zb_uint8_t endpoint, zb_bool_t initiator);


/* --- Tunnel service -------------------------------------------------------------------------- */


/**
 * @brief   Transport ZigBee Network Packet Data Units (NPDUs) over BLE from ZVD to ZDD
 *
 * @param [in]  conn_id         Connection ID
 * @param [in]  payload         Tunnel service payload. Formed by one or more NPDUs to be sent to
 * the connected ZDD device. Each NPDU will be encoded in a separated TLV.
 * @param [in]  payload_length  Tunnel service payload length.
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 * @note    The length of the TLV must fit the BLE connection's negotiated MTU size
 */
zb_ret_t zbd_zvd_tunnel_send_npdu(zb_ble_conn_id_t conn_id, zb_uint8_t *payload, zb_uint8_t payload_length);

/* --- Security service ------------------------------------------------------------------------ */

/**
 * @brief   Starts a secure session with the connected ZDD
 *
 * @param [in]  conn_id         Connection ID
 * @return  zb_ret_t
 *              - RET_OK on success
 *              - RET_ERROR on internal stack error
 *
 * @note    This function will be used to start a secure session with the connected ZDD. Basically it will send an SE1
 *          message with the passed parameters. The expected SE2 message from ZDD should then be received via indication,
 *          then the SE3 should be sent, and the SE4 should be received by another indication
 */
zb_ret_t zbd_zvd_establish_secure_session(zb_ble_conn_id_t conn_id);


/**
 * @brief Set a callback to be called when the secure session establishment between ZVD and ZDD is done
 *
 * @param [in]  callback        A callback
 */
void zbd_zvd_security_set_secure_session_established_callback(zbd_zvd_security_establish_secure_session_callback_t callback);


/**
 * @brief Set a preshared key to be used during session establishment with zdd
 *
 * @param conn_id         zdd connection to set the key for
 * @param psk_type        type of preshared key used @see zb_tlv_direct_psk_secrets_e
 * @param key_neg_method  key negotiation method used @see zb_tlv_direct_key_negotiation_methods_e
 * @param psk             the psk to be used during next session establishment
 *                        (if NULL is passed, ZigBeeAlliance18 key is being used)
 * @param length          length of the psk in bytes
 *
 * @returns RET_OK if PSK was set, RET_ERROR otherwise
 */
zb_ret_t zbd_zvd_security_set_negotiation_method(zb_ble_conn_id_t conn_id, zb_tlv_psk_secrets_t psk_type, zb_tlv_key_negotiation_methods_t key_neg_method, const zb_uint8_t* psk, zb_uint8_t length);


/**
 * @brief Set a Network Key Sequence Number, which will be included in the SE1 in case of basic authorization
 *
 * @param conn_id         zdd connection to set the key for
 * @param nwk_key_seq_num network key sequence number
 *
 * @returns RET_OK if Network Key Sequence Number was set, RET_ERROR otherwise
 */
zb_ret_t zbd_zvd_security_set_nwk_key_seq_num(zb_ble_conn_id_t conn_id, zb_uint8_t nwk_key_seq_num);


/**
 * @brief Generate administrative access key (does not apply as current PSK method)
 *
 * @param tclk      ZDD's trust center link key
 * @param admin_key a buffer of 16 bytes to store admin key in
 */
zb_ret_t zbd_zvd_security_generate_admin_key(const zb_uint8_t tclk[ZB_CCM_KEY_SIZE], zb_uint8_t admin_key[16]);

/**
 * @brief Generate basic access key (does not apply as current PSK method)
 *
 * @param nwk_key   ZDD's trust center link key
 * @param basic_key a buffer of 16 bytes to store admin key in
 */
zb_ret_t zbd_zvd_security_generate_basic_key(const zb_uint8_t nwk_key[ZB_CCM_KEY_SIZE], zb_uint8_t basic_key[16]);

extern const char * str_comm_joined_status(joined_status_t joined_status);
extern const char * str_comm_joining_method(zbd_joining_method_t joining_method);
extern const char * str_comm_device_type(zbd_device_type_t device_type);
extern const char * str_comm_manage_joiners_cmd(zbd_manage_joiners_cmd_t cmd);

/* According to Zigbee Direct spec 7.7.2.4.7. Link Key TLV, the Link Key TLV Flags field has following structure */

/**
 * @name ZBD APS Link Key Flags Mask
 * @anchor zbd_aps_link_key_flags_mask_t
 * @brief ZBD APS Link Key Flags Mask
 */
/** @{ */
#define ZBD_APS_LINK_KEY_FLAGS_MASK_UNIQUE 0b00000001U
#define ZBD_APS_LINK_KEY_FLAGS_MASK_PROVISIONAL 0b00000010U
/** @} */

/**
 * @brief Type for ZBD APS Link Key Flags Mask.
 *
 * @ref zbd_aps_link_key_flags_mask_t were declared previously as enum
 */
typedef zb_uint8_t zbd_aps_link_key_flags_mask_t;

/** @} addtogroup ZB_DIRECT_COMMON_API */

#endif /*ZBOSS_API_DIRECT_H*/
