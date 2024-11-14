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
/* PURPOSE: Common functions for GPD commands
*/

#ifndef GBOSS_CMD_COMMON_H
#define GBOSS_CMD_COMMON_H 1

/*! \addtogroup nwk */
/*! @{ */


/**
 * Enum all ZGPD APP Command IDs.
 */
typedef enum gboss_cmd_ids_e {
/*  --------------------------------------------------------------------- | ----------------------------------------------------------------------------------- */
/*  GPD Command                                                           | Mapping to Zigbee                                                                   */
/*  --------------------------------------- | ---------- | -------------- | ------------------------- | ------------------------- | --------------------------- */
/*  Command                                   ID           Has payload?     Cluster ID                  Command ID                  Command Payload             */
/*  --------------------------------------- | ---------- | -------------- | ------------------------- | ------------------------- | --------------------------- */
    GBOSS_APP_CMD_ID_IDENTIFY               = 0x00,     /* Payloadless    | Identify                  | Identify                  | 0x003c                      */
    GBOSS_APP_CMD_ID_RECALL_SCENE0          = 0x10,     /* Payloadless    | Scenes                    | Recall Scene              | GroupID SceneID = 0         */
    GBOSS_APP_CMD_ID_RECALL_SCENE1          = 0x11,     /* Payloadless    | Scenes                    | Recall Scene              | GroupID SceneID = 1         */
    GBOSS_APP_CMD_ID_RECALL_SCENE2          = 0x12,     /* Payloadless    | Scenes                    | Recall Scene              | GroupID SceneID = 2         */
    GBOSS_APP_CMD_ID_RECALL_SCENE3          = 0x13,     /* Payloadless    | Scenes                    | Recall Scene              | GroupID SceneID = 3         */
    GBOSS_APP_CMD_ID_RECALL_SCENE4          = 0x14,     /* Payloadless    | Scenes                    | Recall Scene              | GroupID SceneID = 4         */
    GBOSS_APP_CMD_ID_RECALL_SCENE5          = 0x15,     /* Payloadless    | Scenes                    | Recall Scene              | GroupID SceneID = 5         */
    GBOSS_APP_CMD_ID_RECALL_SCENE6          = 0x16,     /* Payloadless    | Scenes                    | Recall Scene              | GroupID SceneID = 6         */
    GBOSS_APP_CMD_ID_RECALL_SCENE7          = 0x17,     /* Payloadless    | Scenes                    | Recall Scene              | GroupID SceneID = 7         */
    GBOSS_APP_CMD_ID_STORE_SCENE0           = 0x18,     /* Payloadless    | Scenes                    | Store Scene               | GroupID SceneID = 0         */
    GBOSS_APP_CMD_ID_STORE_SCENE1           = 0x19,     /* Payloadless    | Scenes                    | Store Scene               | GroupID SceneID = 1         */
    GBOSS_APP_CMD_ID_STORE_SCENE2           = 0x1a,     /* Payloadless    | Scenes                    | Store Scene               | GroupID SceneID = 2         */
    GBOSS_APP_CMD_ID_STORE_SCENE3           = 0x1b,     /* Payloadless    | Scenes                    | Store Scene               | GroupID SceneID = 3         */
    GBOSS_APP_CMD_ID_STORE_SCENE4           = 0x1c,     /* Payloadless    | Scenes                    | Store Scene               | GroupID SceneID = 4         */
    GBOSS_APP_CMD_ID_STORE_SCENE5           = 0x1d,     /* Payloadless    | Scenes                    | Store Scene               | GroupID SceneID = 5         */
    GBOSS_APP_CMD_ID_STORE_SCENE6           = 0x1e,     /* Payloadless    | Scenes                    | Store Scene               | GroupID SceneID = 6         */
    GBOSS_APP_CMD_ID_STORE_SCENE7           = 0x1f,     /* Payloadless    | Scenes                    | Store Scene               | GroupID SceneID = 7         */
    GBOSS_APP_CMD_ID_OFF                    = 0x20,     /* Payloadless    | On/Off                    | Off                       | N/A                         */
    GBOSS_APP_CMD_ID_ON                     = 0x21,     /* Payloadless    | On/Off                    | On                        | N/A                         */
    GBOSS_APP_CMD_ID_TOGGLE                 = 0x22,     /* Payloadless    | On/Off                    | Toggle                    | N/A                         */
    GBOSS_APP_CMD_ID_RELEASE                = 0x23,     /* Payloadless    | -                         | -                         | -                           */
    GBOSS_APP_CMD_ID_MOVE_UP                = 0x30,     /* Has payload    | Level Control             | Move Up                   |                             */
    GBOSS_APP_CMD_ID_MOVE_DOWN              = 0x31,     /* Has payload    | Level Control             | Move Down                 |                             */
    GBOSS_APP_CMD_ID_STEP_UP                = 0x32,     /* Has payload    | Level Control             | Step Up                   |                             */
    GBOSS_APP_CMD_ID_STEP_DOWN              = 0x33,     /* Has payload    | Level Control             | Step Down                 |                             */
    GBOSS_APP_CMD_ID_LEVEL_CONTROL_STOP     = 0x34,     /* Payloadless    | Level Control             | Stop                      | N/A                         */
    GBOSS_APP_CMD_ID_MOVE_UP_W_ON_OFF       = 0x35,     /* Has payload    | Level Control             | Move Up (with On/Off)     |                             */
    GBOSS_APP_CMD_ID_MOVE_DOWN_W_ON_OFF     = 0x36,     /* Has payload    | Level Control             | Move Down (with On/Off)   |                             */
    GBOSS_APP_CMD_ID_STEP_UP_W_ON_OFF       = 0x37,     /* Has payload    | Level Control             | Step Up (with On/Off)     |                             */
    GBOSS_APP_CMD_ID_STEP_DOWN_W_ON_OFF     = 0x38,     /* Has payload    | Level Control             | Step Down (with On/Off)   |                             */
    GBOSS_APP_CMD_ID_MOVE_HUE_STOP          = 0x40,     /* Payloadless    | Color Control             | Move Hue                  | Stop                        */
    GBOSS_APP_CMD_ID_MOVE_HUE_UP            = 0x41,     /* Has payload    | Color Control             | Move Hue Up               |                             */
    GBOSS_APP_CMD_ID_MOVE_HUE_DOWN          = 0x42,     /* Has payload    | Color Control             | Move Hue Down             |                             */
    GBOSS_APP_CMD_ID_STEP_HUE_UP            = 0x43,     /* Has payload    | Color Control             | Step Hue Up               |                             */
    GBOSS_APP_CMD_ID_STEP_HUE_DOWN          = 0x44,     /* Has payload    | Color Control             | Step Hue Down             |                             */
    GBOSS_APP_CMD_ID_MOVE_SATURATION_STOP   = 0x45,     /* Payloadless    | Color Control             | Move Saturation           | Stop                        */
    GBOSS_APP_CMD_ID_MOVE_SATURATION_UP     = 0x46,     /* Has payload    | Color Control             | Move Saturation Up        |                             */
    GBOSS_APP_CMD_ID_MOVE_SATURATION_DOWN   = 0x47,     /* Has payload    | Color Control             | Move Saturation Down      |                             */
    GBOSS_APP_CMD_ID_STEP_SATURATION_UP     = 0x48,     /* Has payload    | Color Control             | Step Saturation Up        |                             */
    GBOSS_APP_CMD_ID_STEP_SATURATION_DOWN   = 0x49,     /* Has payload    | Color Control             | Step Saturation Down      |                             */
    GBOSS_APP_CMD_ID_MOVE_COLOR             = 0x4a,     /* Has payload    | Color Control             | Move Color                |                             */
    GBOSS_APP_CMD_ID_STEP_COLOR             = 0x4b,     /* Has payload    | Color Control             | Step Color                |                             */
    GBOSS_APP_CMD_ID_LOCK_DOOR              = 0x50,     /* Payloadless    | Door Lock                 | Lock Door                 | N/A                         */
    GBOSS_APP_CMD_ID_UNLOCK_DOOR            = 0x51,     /* Payloadless    | Door Lock                 | Unlock Door               | N/A                         */
    GBOSS_APP_CMD_ID_PRESS_1_OF_1           = 0x60,     /* Payloadless    | Press 1 of 1              | N/A                       | N/A                         */
    GBOSS_APP_CMD_ID_RELEASE_1_OF_1         = 0x61,     /* Payloadless    | Release 1 of 1            | N/A                       | N/A                         */
    GBOSS_APP_CMD_ID_PRESS_1_OF_2           = 0x62,     /* Payloadless    | Press 1 of 2              | N/A                       | N/A                         */
    GBOSS_APP_CMD_ID_RELEASE_1_OF_2         = 0x63,     /* Payloadless    | Release 1 of 2            | N/A                       | N/A                         */
    GBOSS_APP_CMD_ID_PRESS_2_OF_2           = 0x64,     /* Payloadless    | Press 2 of 2              | N/A                       | N/A                         */
    GBOSS_APP_CMD_ID_RELEASE_2_OF_2         = 0x65,     /* Payloadless    | Release 2 of 2            | N/A                       | N/A                         */
    GBOSS_APP_CMD_ID_SHORT_PRESS_1_OF_1     = 0x66,     /* Payloadless    | Short press 1 of 1        | N/A                       | N/A                         */
    GBOSS_APP_CMD_ID_SHORT_PRESS_1_OF_2     = 0x67,     /* Payloadless    | Short press 1 of 2        | N/A                       | N/A                         */
    GBOSS_APP_CMD_ID_SHORT_PRESS_2_OF_2     = 0x68,     /* Payloadless    | Short press 2 of 2        | N/A                       | N/A                         */
    GBOSS_APP_CMD_ID_8BIT_VECTOR_PRESS      = 0x69,     /* Has payload    | 8-bit vector: press       |                           |                             */
    GBOSS_APP_CMD_ID_8BIT_VECTOR_RELEASE    = 0x6a,     /* Has payload    | 8-bit vector: release     |                           |                             */
    GBOSS_APP_CMD_ID_ATTR_REPORT            = 0xa0,     /* Has payload    | Copied from tr. GPD cmd   | ZCL Report attr. cmd      | Copied from tr. GPD cmd     */
    GBOSS_APP_CMD_ID_MANUF_ATTR_REPORT      = 0xa1,     /* Has payload    | Copied from tr. GPD cmd   | ZCL Report attr. cmd      | Copied from tr. GPD cmd     */
    GBOSS_APP_CMD_ID_MULTI_CL_REPORT        = 0xa2,     /* Has payload    | Copied from tr. GPD cmd   | ZCL Report attr. cmd      | Copied from tr. GPD cmd     */
    GBOSS_APP_CMD_ID_MANUF_MULTI_CL_REPORT  = 0xa3,     /* Has payload    | Copied from tr. GPD cmd   | ZCL Report attr. cmd      | Copied from tr. GPD cmd     */
    GBOSS_APP_CMD_ID_REQUEST_ATTR           = 0xa4,     /* Has payload    | Copied from tr. GPD cmd   | ZCL Read attr. cmd        | Copied from tr. GPD cmd     */
    GBOSS_APP_CMD_ID_READ_ATTR_RESPONSE     = 0xa5,     /* Has payload    | Copied from tr. GPD cmd   | ZCL Read attr. resp cmd   | Copied from tr. GPD cmd     */
    GBOSS_APP_CMD_ID_ZCL_TUNNELING          = 0xa6,     /* Has payload    | Copied from tr. GPD cmd   | Copied from tr. GPD cmd   | Copied from tr. GPD cmd     */
    GBOSS_APP_CMD_ID_COMPACT_ATTR_REPORT    = 0xa8,     /* Has payload    | Deriv. from tr. GPD cmd   | ZCL Report attr. cmd      | Deriv. from tr. GPD cmd     */
    GBOSS_APP_CMD_ID_ANY_GPD_SENSOR_CMD     = 0xaf,     /* Has payload    | Copied from tr. GPD cmd   | ZCL Report attr. cmd      | Copied from tr. GPD cmd     */
    
    /* Manufacturer-defined GPD commands (payload is manufacturer-specific) - 0xb0 to 0xbf */
    
    GBOSS_APP_CMD_ID_COMMISSIONING          = 0xe0,     /* Has payload    |                           |                           |                             */
    GBOSS_APP_CMD_ID_DECOMMISSIONING        = 0xe1,     /* Payloadless    |                           |                           |                             */
    GBOSS_APP_CMD_ID_SUCCESS                = 0xe2,     /* Payloadless    |                           |                           |                             */
    GBOSS_APP_CMD_ID_CHANNEL_REQUEST        = 0xe3,     /* Has payload    |                           |                           |                             */
    GBOSS_APP_CMD_ID_APPLICATION_DESCR      = 0xe4,     /* Has payload    |                           |                           |                             */

    GBOSS_APP_CMD_ID_COMMISSIONING_REPLY    = 0xf0,     /* Has payload    |                           |                           |                             */
    GBOSS_APP_CMD_ID_WRITE_ATTRIBUTES       = 0xf1,     /* Has payload    |                           |                           |                             */
    GBOSS_APP_CMD_ID_READ_ATTRIBUTES        = 0xf2,     /* Has payload    |                           |                           |                             */
    GBOSS_APP_CMD_ID_CHANNEL_CONFIGURATION  = 0xf3,     /* Has payload    |                           |                           |                             */

} gboss_cmd_ids_t;

zb_ret_t gboss_payloadless_cmd(zb_uint8_t cmd,
                               const gboss_device_config_t* device_config,
                               const gboss_tx_cfg_t *tx_cfg);

zb_ret_t gboss_cmd_move(zb_uint8_t cmd,
                        zb_uint8_t rate,
                        zb_bool_t rate_informed,
                        const gboss_device_config_t* device_config,
                        const gboss_tx_cfg_t *tx_cfg);

zb_ret_t gboss_cmd_step(zb_uint8_t cmd,
                        zb_uint8_t step_size,
                        zb_uint16_t tr_time,
                        zb_bool_t tr_time_informed,
                        const gboss_device_config_t* device_config,
                        const gboss_tx_cfg_t *tx_cfg);

zb_ret_t gboss_cmd_move_color(zb_uint8_t cmd,
                              zb_uint16_t rate_x,
                              zb_uint16_t rate_y,
                              const gboss_device_config_t* device_config,
                              const gboss_tx_cfg_t *tx_cfg);

zb_ret_t gboss_cmd_step_color(zb_uint8_t cmd,
                              zb_uint16_t step_x,
                              zb_uint16_t step_y,
                              zb_uint16_t tr_time,
                              zb_bool_t tr_time_informed,
                              const gboss_device_config_t* device_config,
                              const gboss_tx_cfg_t *tx_cfg);

zb_ret_t gboss_cmd_attr_report(zb_uint16_t zb_cluster_id,
                               zb_uint32_t attr_count,
                               zb_zcl_attr_t *attr_reports,
                               const gboss_device_config_t* device_config,
                               const gboss_tx_cfg_t *tx_cfg);

zb_ret_t gboss_cmd_attr_report_rx(zb_uint16_t zb_cluster_id,
                                  zb_uint32_t attr_count,
                                  zb_zcl_attr_t *attr_reports,
                                  const gboss_device_config_t* device_config,
                                  const gboss_tx_cfg_t *tx_cfg,
                                  zb_uint8_t rx_after_tx);

zb_ret_t gboss_cmd_attr_read_response(zb_uint16_t zb_cluster_id,
                               zb_uint8_t *attr_ids_buf,
                               zb_size_t attr_ids_len,
                               zb_uint32_t attr_count,
                               zb_zcl_attr_t *attrs,
                               const gboss_device_config_t* device_config,
                               const gboss_tx_cfg_t *tx_cfg);

zb_ret_t gboss_cmd_attr_request(zb_uint16_t zb_cluster_id,
                               zb_uint32_t attr_count,
                               zb_zcl_attr_t *attr_reports,
                               const gboss_device_config_t* device_config,
                               const gboss_tx_cfg_t *tx_cfg);

/**
@}
*/

/*! \addtogroup bidirCmdHandle */
/*! @{ */

/**
   Parse the header of the received command

   @param buf - packet payload
   @param length - payload length
   @param option - option field from the header
   @param manufactured_id - manufactured id from the header
   @param hdr_len - header length

   @return RET_OK or RET_INVALID_PARAMETER if header is invalid
 */
zb_ret_t gboss_handle_header_attr_cmd(zb_uint8_t *buf,
                                      zb_size_t length,
                                      zb_uint8_t *option,
                                      zb_uint16_t *manufactured_id,
                                      zb_size_t *hdr_len);


/**
   Parse single attribute from the read/write attribute command

   @param buf - packet payload at the position of the attribute to be parsed
   @param length - the rest of the payload length
   @param cluster_id - cluster id from the payload
   @param payload_length - cluster-specific payload length

   @return number of bytes parsed
 */
zb_size_t gboss_handle_cluster_attr_cmd(zb_uint8_t *buf,
                                        zb_size_t length,
                                        zb_uint16_t *cluster_id,
                                        zb_uint8_t *payload_length);

/**
   Parse Write Attributes command

   @param buf - packet payload rest
   @param length - the rest of the payload length
   @param attr_count - number of attributes to be parsed
   @param attr_reports - filled array of attributes

   @return RET_OK or RET_INVALID_PARAMETER if packet is invalid
 */
zb_ret_t gboss_handle_cluster_attr_write(zb_uint8_t *buf,
                                         zb_size_t length,
                                         zb_uint32_t attr_count,
                                         zb_zcl_attr_t *attr_reports);

/**
@}
*/


#endif /* GBOSS_CMD_COMMON_H */
