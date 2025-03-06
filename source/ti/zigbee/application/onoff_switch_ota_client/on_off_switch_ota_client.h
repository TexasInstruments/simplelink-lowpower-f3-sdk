/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

#ifndef ON_OFF_SWITCH_OTA_H
#define ON_OFF_SWITCH_OTA_H 1

#include "zboss_api.h"

#define ZB_SWITCH_ENDPOINT          10

/* OTA Manufacturer code */
#define DL_DEVICE_MANUFACTURER_CODE  0x1234
/* ZBOSS SDK for Smart plug application - from zb_ver_sdk_type.h */
#define ZBOSS_SDK_SMART_PLUG_MAJOR 3

/* Basic cluster attributes data */
#define DL_INIT_BASIC_STACK_VERSION     ZBOSS_MAJOR

#define DL_INIT_BASIC_APP_VERSION       1

/* OTA Upgrade client cluster attributes data */
#define DL_INIT_OTA_FILE_VERSION    \
  ( ((zb_uint32_t)ZBOSS_MAJOR) | ((zb_uint32_t)ZBOSS_MINOR << 8) | ((zb_uint32_t)ZBOSS_SDK_SMART_PLUG_MAJOR << 16) | ((zb_uint32_t)DL_INIT_BASIC_APP_VERSION << 24) )
#define DL_INIT_OTA_HW_VERSION          2
#define DL_INIT_OTA_MANUFACTURER        DL_DEVICE_MANUFACTURER_CODE

#define DL_INIT_OTA_IMAGE_TYPE          0x0012

#define DL_INIT_OTA_MIN_BLOCK_REQUE             10
#define DL_INIT_OTA_IMAGE_STAMP         ZB_ZCL_OTA_UPGRADE_IMAGE_STAMP_MIN_VALUE
#define DL_OTA_IMAGE_BLOCK_DATA_SIZE_MAX        32
#define DL_OTA_UPGRADE_SERVER                   { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa }
#define DL_OTA_UPGRADE_QUERY_TIMER_COUNTER      (12*60)

#define DL_OTA_UPGRADE_HASH_LENGTH              16
#define DL_OTA_DEVICE_RESET_TIMEOUT             30*ZB_TIME_ONE_SECOND


/*!
  @brief Declare HA cluster list for On Off Switch OTA Device
  @param cluster_list_name - cluster list variable name
  @param on_off_switch_config_attr_list - attribute list for On Off Switch
  @param basic_attr_list - attribute list for Basic cluster
  @param identify_attr_list - attribute list for Identify cluster
  @param ota_upgrade_attr_list - attribute list for OTA cluster
*/

#define ZB_HA_DECLARE_ON_OFF_SWITCH_OTA_CLUSTER_LIST(                   \
  cluster_list_name,                                                    \
  on_off_switch_config_attr_list,                                       \
  basic_attr_list,                                                      \
  identify_attr_list,                                                   \
  ota_upgrade_attr_list                                                 \
)                                                                       \
  zb_zcl_cluster_desc_t cluster_list_name[] =                           \
{                                                                       \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_ON_OFF_SWITCH_CONFIG,                             \
    ZB_ZCL_ARRAY_SIZE(on_off_switch_config_attr_list, zb_zcl_attr_t),   \
    (on_off_switch_config_attr_list),                                   \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  ),                                                                    \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_BASIC,                                            \
    ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),                  \
    (basic_attr_list),                                                  \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  ),                                                                    \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_ALARMS,                                           \
    0,                                                                  \
    NULL,                                                               \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  ),                                                                    \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_IDENTIFY,                                         \
    ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),               \
    (identify_attr_list),                                               \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  ),                                                                    \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_ON_OFF,                                           \
    0,                                                                  \
    NULL,                                                               \
    ZB_ZCL_CLUSTER_CLIENT_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  ),                                                                    \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_SCENES,                                           \
    0,                                                                  \
    NULL,                                                               \
    ZB_ZCL_CLUSTER_CLIENT_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  ),                                                                    \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_IDENTIFY,                                         \
    0,                                                                  \
    NULL,                                                               \
    ZB_ZCL_CLUSTER_CLIENT_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  ),                                                                    \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_GROUPS,                                           \
    0,                                                                  \
    NULL,                                                               \
    ZB_ZCL_CLUSTER_CLIENT_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  ),                                                                    \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_BASIC,                                            \
    0,                                                                  \
    NULL,                                                               \
    ZB_ZCL_CLUSTER_CLIENT_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  ),                                                                    \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,                                      \
    ZB_ZCL_ARRAY_SIZE(ota_upgrade_attr_list, zb_zcl_attr_t),            \
    (ota_upgrade_attr_list),                                            \
    ZB_ZCL_CLUSTER_CLIENT_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  ),                                                                    \
  ZB_ZCL_CLUSTER_DESC(                                                  \
    ZB_ZCL_CLUSTER_ID_TIME,                                             \
    0,                                                                  \
    NULL,                                                               \
    ZB_ZCL_CLUSTER_CLIENT_ROLE,                                         \
    ZB_ZCL_MANUF_CODE_INVALID                                           \
  )                                                                     \
}

/* attributes of Basic cluster */
typedef struct device_basic_attr_s
{
  zb_uint8_t zcl_version;
  zb_uint8_t app_version;
  zb_uint8_t stack_version;
  zb_uint8_t hw_version;
  zb_char_t mf_name[32];
  zb_char_t model_id[32];
  zb_char_t date_code[16];
  zb_uint8_t power_source;
  zb_char_t location_id[5];
  zb_uint8_t ph_env;
  zb_char_t sw_build_id[3];
} device_basic_attr_t;

/* attributes of Identify cluster */
typedef struct device_identify_attr_s
{
  zb_uint16_t identify_time;
} device_identify_attr_t;

/* OTA Upgrade client cluster attributes data */
typedef struct device_ota_attr_s
{
  zb_ieee_addr_t upgrade_server;
  zb_uint32_t file_offset;
  zb_uint32_t file_version;
  zb_uint16_t stack_version;
  zb_uint32_t downloaded_file_ver;
  zb_uint16_t downloaded_stack_ver;
  zb_uint8_t image_status;
  zb_uint16_t manufacturer;
  zb_uint16_t image_type;
  zb_uint16_t min_block_reque;
  zb_uint16_t image_stamp;
  zb_uint16_t server_addr;
  zb_uint8_t server_ep;
} device_ota_attr_t;

typedef struct ota_upgrade_ctx_s
{
  zb_uint32_t total_image_size;
  zb_uint32_t addr_to_erase;
  zb_uint32_t address;          /* Supposed to be constant value, init
                                 * on OTA Upgrade start  */
  void       *flash_dev; //! Probably not needed
#ifdef OTA_OFFCHIP
  zb_bool_t flash_is_open;
#endif // OTA_OFFCHIP

  zb_uint32_t fw_version;
  zb_uint8_t param;     // buffer, contain process command (if scheduling process)
  zb_bool_t is_started_manually;
#ifndef ZB_USE_OSIF_OTA_ROUTINES
  zb_uint8_t fw_image_portion[DL_OTA_IMAGE_BLOCK_DATA_SIZE_MAX * 2];
  zb_uint32_t fw_image_portion_size;
  zb_uint32_t file_length;        /*!< OTA file length got from next_image_resp  */

  zb_uint32_t hash_addr;
  zb_uint8_t hash[DL_OTA_UPGRADE_HASH_LENGTH];
  zb_bool_t hash16_calc_ongoing;
#endif

  zb_uint8_t  server_endpoint;  /**< The EndPoint of the OTA server */
  zb_uint16_t server_nwk_addr; /*!< Short address of the OTA server */
} ota_upgrade_ctx_t;

typedef struct on_off_switch_attr_s
{
  zb_uint8_t attr_switch_type;
  zb_uint8_t attr_switch_actions;
} on_off_switch_attr_t;

typedef struct on_off_switch_ota_ctx_s
{
  on_off_switch_attr_t on_off_sw_attr;
  device_basic_attr_t basic_attr;
  device_identify_attr_t identify_attr;
  device_ota_attr_t ota_attr;
  ota_upgrade_ctx_t ota_ctx;
} on_off_switch_ota_ctx_t;

extern on_off_switch_ota_ctx_t g_dev_ctx;

/*** OTA API ***/
void dl_process_ota_upgrade_cb(zb_uint8_t param);
void dl_ota_start_upgrade(zb_uint8_t param);


#endif /* ON_OFF_SWITCH_OTA_H */