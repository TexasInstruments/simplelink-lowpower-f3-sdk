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
/* PURPOSE: OTA upgrade client application
*/

#define ZB_TRACE_FILE_ID 63256
#include "zboss_api.h"

#include "ota_client.h"

#if ! defined ZB_ROUTER_ROLE
#error define ZB_ROUTER_ROLE to compile zr tests
#endif

/* Used endpoint */
#define ENDPOINT  10

#define OTA_UPGRADE_TEST_FILE_VERSION       0x01010101
#define OTA_UPGRADE_TEST_FILE_VERSION_NEW   0x01020101

#define OTA_UPGRADE_TEST_MANUFACTURER       123

#define OTA_UPGRADE_TEST_IMAGE_TYPE         321

#define OTA_UPGRADE_TEST_IMAGE_SIZE         72

#define OTA_UPGRADE_TEST_CURRENT_TIME       0x12345678

#define OTA_UPGRADE_TEST_UPGRADE_TIME       0x12345978

#define OTA_UPGRADE_TEST_DATA_SIZE          32

/**
 * Global variables definitions
 */
zb_ieee_addr_t g_zr_addr = {0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
zb_ieee_addr_t g_zc_addr = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

/**
 * Declaring attributes for each cluster
 */

/* Basic cluster attributes */
/* ZCL version attribute */
zb_uint8_t g_attr_zcl_version  = ZB_ZCL_VERSION;
/* Power source attribute */
zb_uint8_t g_attr_power_source = ZB_ZCL_BASIC_POWER_SOURCE_UNKNOWN;

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST(basic_attr_list, &g_attr_zcl_version, &g_attr_power_source);

/* OTA Upgrade client cluster attributes */
/* UpgradeServerID attribute */
zb_ieee_addr_t upgrade_server = ZB_ZCL_OTA_UPGRADE_SERVER_DEF_VALUE;
/* FileOffset attribute */
zb_uint32_t file_offset = ZB_ZCL_OTA_UPGRADE_FILE_OFFSET_DEF_VALUE;
/* CurrentFileVersion attribute (custom data) */
zb_uint32_t file_version = OTA_UPGRADE_TEST_FILE_VERSION;
/* CurrentZigbeeStackVersion attribute */
zb_uint16_t stack_version = ZB_ZCL_OTA_UPGRADE_FILE_HEADER_STACK_PRO;
/* DownloadedFileVersion attribute */
zb_uint32_t downloaded_file_ver = ZB_ZCL_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_DEF_VALUE;
/* DownloadedZigbeeStackVersion attribute */
zb_uint16_t downloaded_stack_ver = ZB_ZCL_OTA_UPGRADE_DOWNLOADED_STACK_DEF_VALUE;
/* ImageUpgradeStatus attribute */
zb_uint8_t image_status = ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_DEF_VALUE;
/* Manufacturer ID attribute (custom data) */
zb_uint16_t manufacturer = OTA_UPGRADE_TEST_MANUFACTURER;
/* Image Type ID attribute (custom data) */
zb_uint16_t image_type = OTA_UPGRADE_TEST_IMAGE_TYPE;
/* MinimumBlockReque attribute */
zb_uint16_t min_block_reque = 0;
/* Image Stamp attribute */
zb_uint16_t image_stamp = ZB_ZCL_OTA_UPGRADE_IMAGE_STAMP_MIN_VALUE;
/* Server short address attribute */
zb_uint16_t server_addr;
/* Server endpoint attribute */
zb_uint8_t server_ep;

ZB_ZCL_DECLARE_OTA_UPGRADE_ATTRIB_LIST(ota_upgrade_attr_list,
    &upgrade_server, &file_offset, &file_version, &stack_version, &downloaded_file_ver,
    &downloaded_stack_ver, &image_status, &manufacturer, &image_type, &min_block_reque, &image_stamp,
    &server_addr, &server_ep, 0x0101, OTA_UPGRADE_TEST_DATA_SIZE, ZB_ZCL_OTA_UPGRADE_QUERY_TIMER_COUNT_DEF);

/* Declare cluster list for a device */
ZB_HA_DECLARE_OTA_UPGRADE_CLIENT_CLUSTER_LIST( ota_upgrade_client_clusters,
          basic_attr_list, ota_upgrade_attr_list);

/* Declare endpoint */
ZB_HA_DECLARE_OTA_UPGRADE_CLIENT_EP(ota_upgrade_client_ep, ENDPOINT, ota_upgrade_client_clusters);

/* Declare application's device context for single-endpoint device */
ZB_HA_DECLARE_OTA_UPGRADE_CLIENT_CTX(ota_upgrade_client_ctx, ota_upgrade_client_ep);

void test_device_cb(zb_uint8_t param)
{
  zb_zcl_device_callback_param_t *device_cb_param =
    ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);

  TRACE_MSG(TRACE_APP1, ">> test_device_cb param %hd id %hd", (FMT__H_H,
      param, device_cb_param->device_cb_id));

  device_cb_param->status = RET_OK;
  switch (device_cb_param->device_cb_id)
  {
    /* It's OTA Upgrade command */
    case ZB_ZCL_OTA_UPGRADE_VALUE_CB_ID:
    {
      zb_zcl_ota_upgrade_value_param_t *ota_upgrade_value = &(device_cb_param->cb_param.ota_value_param);

      switch (ota_upgrade_value->upgrade_status)
      {
        case ZB_ZCL_OTA_UPGRADE_STATUS_START:
          /* Start OTA upgrade. */
          if (image_status == ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL)
          {
            /* Accept image */
            ota_upgrade_value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
          }
          else
          {
            /* Another download is in progress, deny new image */
            ota_upgrade_value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_BUSY;
          }
          break;
        case ZB_ZCL_OTA_UPGRADE_STATUS_RECEIVE:
          /* Process image block. */
          ota_upgrade_value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
          break;
        case ZB_ZCL_OTA_UPGRADE_STATUS_CHECK:
          /* Downloading is finished, do additional checks if needed etc before Upgrade End Request. */
          ota_upgrade_value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
          break;
        case ZB_ZCL_OTA_UPGRADE_STATUS_APPLY:
          /* Upgrade End Resp is ok, ZCL checks for manufacturer, image type etc are ok.
             Last step before actual upgrade. */
          ota_upgrade_value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
          break;
        case ZB_ZCL_OTA_UPGRADE_STATUS_FINISH:
          /* It is time to upgrade FW. */
          break;
      }
    }
    break;

    default:
      device_cb_param->status = RET_ERROR;
      break;
  }

  TRACE_MSG(TRACE_APP1, "<< test_device_cb %hd", (FMT__H, device_cb_param->status));
}

MAIN()
{
  ARGV_UNUSED;

  /* Trace disable */
  /*  ZB_SET_TRACE_OFF(); */
  /* Traffic dump enable */
  ZB_SET_TRAF_DUMP_ON();

  /* Global ZBOSS initialization */
  ZB_INIT("ota_simple_download_zr");

  /* Set up defaults for the commissioning */
  zb_set_long_address(g_zr_addr);
  zb_set_network_router_role(1l<<21);
  zb_set_nvram_erase_at_start(ZB_FALSE);

  /* Register device ZCL context */
  ZB_AF_REGISTER_DEVICE_CTX(&ota_upgrade_client_ctx);
  /* Set Device user application callback */
  ZB_ZCL_REGISTER_DEVICE_CB(test_device_cb);

  /* Initiate the stack start with starting the commissioning */
  if (zboss_start() != RET_OK)
  {
    TRACE_MSG(TRACE_ERROR, "zboss_start failed", (FMT__0));
  }
  else
  {
    /* Call the main loop */
    zboss_main_loop();
  }

  /* Deinitialize trace */
  TRACE_DEINIT();

  MAIN_RETURN(0);
}

/* Callback to handle the stack events */
void zboss_signal_handler(zb_uint8_t param)
{
  zb_zdo_app_signal_type_t sig = zb_get_app_signal(param, NULL);

  TRACE_MSG(TRACE_APP1, ">> zboss_signal_handler %h", (FMT__H, param));

  if (ZB_GET_APP_SIGNAL_STATUS(param) == 0)
  {
    switch(sig)
    {
      case ZB_ZDO_SIGNAL_DEFAULT_START:
      case ZB_BDB_SIGNAL_DEVICE_FIRST_START:
      case ZB_BDB_SIGNAL_DEVICE_REBOOT:
        TRACE_MSG(TRACE_APP1, "Device STARTED OK", (FMT__0));
        /* Initialize OTA Upgrade cluster */
        ZB_SCHEDULE_APP_ALARM(zb_zcl_ota_upgrade_init_client, param, ZB_MILLISECONDS_TO_BEACON_INTERVAL(2*1000));
        param = 0;
        break;

      case ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY:
        TRACE_MSG(TRACE_APP1, "Production configuration block is ready", (FMT__0));
        break;

      default:
        TRACE_MSG(TRACE_APP1, "Unknown signal %hd", (FMT__H, sig));
    }
  }
  else if (sig == ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY)
  {
    TRACE_MSG(TRACE_APP1, "Production config is not present or invalid", (FMT__0));
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Device started FAILED status %d", (FMT__D, ZB_GET_APP_SIGNAL_STATUS(param)));
  }

  /* Free the buffer if it is not used */
  if (param)
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_APP1, "<< zboss_signal_handler", (FMT__0));
}
