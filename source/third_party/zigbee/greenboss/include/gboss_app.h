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
/* PURPOSE: GP APP layer API
*/

#ifndef GBOSS_APP_H
#define GBOSS_APP_H 1

/**
@addtogroup appconf
@{
*/

/**
 * @brief GP device MAC sequence number capabilities enum.
 * 
 */
typedef enum gboss_mac_sequence_number_capabilities_e {
    GBOSS_MAC_SEQ_NUM_RANDOM = 0,                           //!< GPD uses random MAC sequence number
    GBOSS_MAC_SEQ_NUM_INCREMENTAL = 1                       //!< GPD uses incremental MAC sequence number
} gboss_mac_sequence_number_capabilities_t;

/**
 * @brief GPD Commissioning - Options field descriptor.
 * 
  * @snippet zgpd_unidir_onoff.c device_config
 */
typedef struct gboss_comm_options_s {
    zb_uint8_t mac_sequence_number_capability   : 1;        //!< See @ref gboss_mac_sequence_number_capabilities_t
    zb_uint8_t rx_on_capability                 : 1;        //!< 1 = GPD can receive data in operational mode, 0 otherwise
    zb_uint8_t application_info_present         : 1;        //!< 1 = Application info field is present in comm. packet, 0 otherwise
    zb_uint8_t reserved                         : 1;        //!< Reserved
    zb_uint8_t pan_id_request                   : 1;        //!< 1 = GPD requests to receive PAN ID value from NWK, 0 otherwise
    zb_uint8_t gp_security_key_request          : 1;        //!< 1 = GPD requests to receive GP sec. key, 0 otherwise
    zb_uint8_t fixed_location                   : 1;        //!< 1 = GPD is not expected to change its position on the NWK, 0 otherwise
    zb_uint8_t extended_options_present         : 1;        //!< 1 = Extended options field is present in comm. packet, 0 otherwise
} gboss_comm_options_t;

/**
 * @brief GPD Commissioning - Security key capabilities.
 * 
 */
typedef enum gboss_security_key_capabilities_e {
    GBOSS_SECURITY_KEY_CAP_L0 = 0,                          //!< No security
    GBOSS_SECURITY_KEY_CAP_L2 = 2,                          //!< 4B frame counter and 4B MIC only
    GBOSS_SECURITY_KEY_CAP_L3 = 3                           //!< Encryption & 4B frame counter and 4B MIC
} gboss_security_key_capabilities_t;

/**
 * @brief GPD Commissioning - Key types.
 * 
 */
typedef enum gboss_key_types_e {
    GBOSS_KEY_TYPE_NO_KEY                           = 0,    //!< No protection for GPDF communication
    GBOSS_KEY_TYPE_ZB_NWK_KEY                       = 1,    //!< GPD communication is secured with Zigbee NWK key
    GBOSS_KEY_TYPE_GPD_GROUP_KEY                    = 2,    //!< Group key is shared between GPDs and GP infrastructure devices
    GBOSS_KEY_TYPE_NWK_KEY_DERIVED_GPD_GROUP_KEY    = 3,    //!< Group key is shared between GPDs and GP infrastructure devices, derived from ZB NWK key
    GBOSS_KEY_TYPE_INDIVIDUAL_PRE_CONFIG_KEY        = 4,    //!< GPD is pre-configured with a security key
    GBOSS_KEY_TYPE_DERIVED_INDIVIDUAL_GROUP_KEY     = 7     //!< An individual key is derived from the GPD independent group key (0x010) used by a particular network
} gboss_key_types_t;

/**
 * @brief GPD Commissioning - Extended options field descriptor.
 * 
  * @snippet zgpd_unidir_onoff.c device_config
 */
typedef struct gboss_comm_ext_options_s {
    zb_uint8_t security_level_capabilities      : 2;        //!< See @ref gboss_security_key_capabilities_t
    zb_uint8_t key_type                         : 3;        //!< See @ref gboss_key_types_t
    zb_uint8_t gpd_key_present                  : 1;        //!< 1 = GPD key field is present, 0 otherwise
    zb_uint8_t gpd_key_encryption               : 1;        //!< 1 = GPD key is (or it can be, if not present) encrypted, 0 otherwise
    zb_uint8_t gpd_outgoing_counter_present     : 1;        //!< 1 = GPD outgoing counter is present, 0 otherwise
} gboss_comm_ext_options_t;

/**
 * @brief GPD Commissioning - Application information descriptor.
 * 
  * @snippet zgpd_unidir_onoff.c device_config
 */
typedef struct gboss_comm_app_info_s {
    zb_uint8_t manuf_id_present                 : 1;        //!< Manufacturer ID field is present
    zb_uint8_t model_id_present                 : 1;        //!< Model ID field is present
    zb_uint8_t gpd_commands_present             : 1;        //!< Number of GPD commands and GPD CommandID list fields are present
    zb_uint8_t cluster_list_present             : 1;        //!< Cluster list field is present
    zb_uint8_t switch_info_present              : 1;        //!< Switch information field is present
    zb_uint8_t gpd_app_descr_command_follows    : 1;        //!< After the current Commissioning GPDF, the GPD Application Description command (0xE4) will follow
    zb_uint8_t reserved                         : 2;        //!< Reserved
} gboss_comm_app_info_t;

/**
 * @brief GPD Application IDs
 * 
 */
typedef enum gboss_app_ids_e {
    GBOSS_APP_ID_0  = 0,                                    //!< Uses SrcID
    GBOSS_APP_ID_2  = 2                                     //!< Uses GPD IEEE address
} gboss_app_ids_t;

/**
 * @brief Possible ZGPD device identifiers.
 * @see ZGP spec, A.4.3 */
typedef enum gboss_dev_id_e {
  GBOSS_SIMPLE_GEN_1_STATE_SWITCH_DEV_ID        = 0x00,     /**< Simple Generic 1-state ZGP Switch */
  GBOSS_SIMPLE_GEN_2_STATE_SWITCH_DEV_ID        = 0x01,     /**< Simple Generic 2-state ZGP Switch */
  GBOSS_ON_OFF_SWITCH_DEV_ID                    = 0x02,     /**< ZGP On/Off Switch */
  GBOSS_LEVEL_CONTROL_SWITCH_DEV_ID             = 0x03,     /**< ZGP Level Control Switch */
#define ZB_ZGP_LVL_CTRL_SWITCH_DEV_ID GBOSS_LEVEL_CONTROL_SWITCH_DEV_ID
  GBOSS_SIMPLE_SENSOR_DEV_ID                    = 0x04,     /**< ZGP Simple Sensor */
  GBOSS_ADVANCED_GEN_1_STATE_SWITCH_DEV_ID      = 0x05,     /**< Advanced Generic 1-state ZGP Switch */
  GBOSS_ADVANCED_GEN_2_STATE_SWITCH_DEV_ID      = 0x06,     /**< Advanced Generic 2-state ZGP Switch */
  GBOSS_GEN_8_CONT_SWITCH_DEV_ID                = 0x07,     /**< Generic 8-contact ZGP Switch */
  GBOSS_COLOR_DIMMER_SWITCH_DEV_ID              = 0x10,     /**< Color Dimmer ZGP Switch */
  GBOSS_LIGHT_SENSOR_DEV_ID                     = 0x11,     /**< ZGP Light Sensor */
  GBOSS_OCCUPANCY_SENSOR_DEV_ID                 = 0x12,     /**< ZGP Occupancy Sensor */
  GBOSS_DOOR_LOCK_CONTROLLER_DEV_ID             = 0x20,     /**< ZGP Door Lock Controller */
  GBOSS_TEMPERATURE_SENSOR_DEV_ID               = 0x30,     /**< ZGP Temperature Sensor */
  GBOSS_PRESSURE_SENSOR_DEV_ID                  = 0x31,     /**< ZGP Pressure Sensor */
  GBOSS_FLOW_SENSOR_DEV_ID                      = 0x32,     /**< ZGP Flow Sensor */
  GBOSS_ENVIRONMENT_SENSOR_DEV_ID               = 0x33,     /**< ZGP Temperature + Humidity Sensor */
  GBOSS_MANUF_SPECIFIC_DEV_ID                   = 0xfe,     /**< Manufactures-specific; 2
                                                             * more fields in the
                                                             * Commissioning frame. See 4.1
                                                             * Manufacturer Specific Green
                                                             * Power Device Type Use case
                                                             * in Green Power Device
                                                             * Manufacturer Specific Device & Command
                                                             * Definition Proposal
                                                             */
  GBOSS_UNDEFINED_DEV_ID                        = 0xff,     /**< Undefined device type */
} gboss_dev_id_t;

/**
 * @brief Main configuration constants of a GP device.
 *
  * @snippet zgpd_unidir_onoff.c device_config
 */
typedef struct gboss_device_config_s {
    zb_uint16_t                 manufacturer;           //!< Manufacturer id
    zb_uint16_t                 model;                  //!< Model id
    zb_ieee_addr_t              src_addr;               //!< 64-bit source address for ApplicationID = 0b010 mode
    zb_uint32_t                 src_id;                 //!< 32-bit device identifier for the GP NWK layer
    zb_uint8_t                  application_id;         //!< ApplicationID value. Only 0 and 2 values are supported now - @ref gboss_app_ids_t
    zb_uint8_t                  endpoint;               //!< Endpoint id if ApplicationID = 0b010
    zb_uint8_t                  device_id;              //!< 8-bit device id used in a commissioning command - @ref gboss_dev_id_t
    gboss_comm_options_t        comm_options;           //!< Commissioning options
    gboss_comm_ext_options_t    comm_extended_options;  //!< Commissioning extended options
    gboss_comm_app_info_t       comm_app_info;          //!< Commissioning application info
    zb_uint8_t                  cmd_list_count;         //!< Number of items in the cmd_list array
    const zb_uint8_t            *cmd_list;              //!< Pointer to the cmd array or NULL
    zb_uint8_t                  cluster_srv_cnt;        //!< Number of supported server clusters
    const zb_uint16_t           *cluster_list_srv;      //!< List of supported server clusters or NULL
    zb_uint8_t                  cluster_cli_cnt;        //!< Number of supported client clusters
    const zb_uint16_t           *cluster_list_cli;      //!< List of supported client clusters or NULL
} gboss_device_config_t;


/**
 * @brief TX configuration outlining how a transmission should proceed concerning the channels to be used and the number of repetitions.

 The channel array describes list of channels that are used for frames transmission in unidirectional commissioning and frames transmission and reception in bidirectional commissioning. 
 The channel array is logically divided by chunks of "channel_group_size". 
 
 Example: If "channels" points to channel_list as follows zb_uint8_t channel_list[9] = [11, 12, 13, 14, 15, 16, 17, 18, 19] and channel_group_size equals to 3,
 it will give three logical groups of channels: 
 - group #0: channels 11, 12, 13
 - group #1: channels 14, 15, 16
 - group #2: channels 17, 18, 19

 The channel array size must be a multiple of channel_group_size. Group size determines channels that are used in one commissioning attempt. 
 The interpretation of how channels are used differs depending on the device state 
 (commissioned or commissioning in progress) and commissioning type (unidirectional or bidirectional).
 
 Unidirectional commissioning or application command TX time - repeat "repetition" times executes the following procedure:
 - Get channel from the channels array,
 - Change channel,
 - Send a frame,
 - Get next channel from the channels array.

 Bidirectional commissioning:
 - Number of repetitions is a GPFS size (number of consequent GP frames to be sent in one TX attempt),
 - Channel_group_size is a number of channels used in "Channel request" commissioning phase.

 Bidirectional commissioning after channel is established:
 - Number of repetitions is a GPFS size,
 - TX on a single channel stored in NVM.

 *
 * @snippet zgpd_unidir_onoff.c tx_config
 */
typedef struct gboss_tx_cfg_s {
    const zb_uint8_t * channels;        //!< The pointer to array of channels to be used for transmitting
    zb_uint8_t channel_count;           //!< Number of items in the channels array
    zb_uint8_t channel_group_size;      //!< Size of the group of channels
    zb_uint8_t repetitions;             //!< Number of sending repetitions for every channel
} gboss_tx_cfg_t;

/**
 * @brief Initializes the ZGPD stack.
 *
 * @param key Pointer to a 16-bytes OOB key or NULL if not used
 * @param device_config GPD configuration block
 *
 * @snippet zgpd_unidir_onoff.c key
 * @snippet zgpd_unidir_onoff.c device_config
 * @snippet zgpd_unidir_onoff.c tx_config
 * @snippet zgpd_unidir_onoff.c app_init
 *
 */
void gboss_app_init(const zb_uint8_t* key,
                    const gboss_device_config_t* device_config);


/**
   Initializes the platform layer.

   To be called before gboss_app_init().

   @snippet zgpd_unidir_onoff.c app_init
*/
void gboss_platform_init(ARGC_ARGV_PROTO);


/**
   Waits until any button is pressed.

   To be called before gboss_test_get_button_mask() for
   bidirectional battery powered device.
  */
void gboss_wait_for_button(void);

/**
   Gets bitmask of buttons pressed on the device boot.

   @return Bit mask of pressed buttons

   @snippet zgpd_unidir_onoff.c app_init
 */
zb_uint32_t gboss_test_get_button_mask(void);


/**
   Informs platform layer about number of supported buttons.

   @param num Number of supported buttons in application
  */
void gboss_set_buttons_max_num(zb_uint8_t num);


/**
@}
*/


/*! \addtogroup nwk */
/*! @{ */

/**
 * @brief Sends one data packet over the air when GPD is already commissioned.
 *
 * The function sends an application layer payload. It automatically adds
 * all necessary low-level headers and performs encryption if necessary.
 *
 * Once a packet built in the internal memory it can be sent several times
 * over multiple channels if necessary. 
 *
 * @param device_config GPD configuration block
 * @param tx_cfg GPD TX configuration block
 * @param data Pointer to the APP layer data payload
 * @param data_size Size of the data
 * @param data_size Size of the data
 * 
 * @return zb_ret_t RET_OK if the packet was successfully sent, RET_ERROR
 * otherwise
 * 
 * @todo Validate inputs, check if MAC really sent it, return RET_ERROR if not
 */
zb_ret_t gboss_app_send(const gboss_device_config_t* device_config,
                        const gboss_tx_cfg_t *tx_cfg,
                        const zb_uint8_t* data,
                        zb_uint8_t data_size);

/**
 * @brief Sends one data packet over the air when GPD is already commissioned.
 *
 * The function sends an application layer payload. It automatically adds
 * all necessary low-level headers and performs encryption if necessary.
 *
 * Once a packet built in the internal memory it can be sent several times
 * over multiple channels if necessary. 
 *
 * @param device_config GPD configuration block
 * @param tx_cfg GPD TX configuration block
 * @param data Pointer to the APP layer data payload
 * @param data_size Size of the data
 * @param rx_after_tx Enable RX after TX bitfield of NWK Extended header for bidir
 * 
 * @return zb_ret_t RET_OK if the packet was successfully sent, RET_ERROR
 * otherwise
 * 
 * @todo Validate inputs, check if MAC really sent it, return RET_ERROR if not
 */
zb_ret_t gboss_app_send_rx(const gboss_device_config_t* device_config,
                           const gboss_tx_cfg_t *tx_cfg,
                           const zb_uint8_t* data,
                           zb_uint8_t data_size,
                           zb_uint8_t rx_after_tx);

/*! @} */


#endif /* GBOSS_APP_H */
