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
/* PURPOSE: OTA upgrade server application
*/

#define ZB_TRACE_FILE_ID 63255
#include "zboss_api.h"

#include "ota_upgrade_server.h"
#include "ota_upgrade_test_common.h"

#if ! defined ZB_COORDINATOR_ROLE
#error define ZB_COORDINATOR_ROLE to compile zc tests
#endif

/* Used endpoint */
#define ENDPOINT  5

/* OTA upgrade data context */
typedef ZB_PACKED_PRE struct ota_upgrade_test_file_s
{
  zb_zcl_ota_upgrade_file_header_t head;
  zb_uint8_t arr[16];

} ZB_PACKED_STRUCT ota_upgrade_test_file_t;

/**
 * Global variables definitions
 */
zb_ieee_addr_t g_zc_addr = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
zb_uint8_t g_key[16] = { 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0, 0, 0, 0, 0, 0, 0, 0};

zb_uint16_t dst_addr;
zb_ieee_addr_t g_zr_addr = {0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};

/* OTA upgrade data */
ota_upgrade_test_file_t ota_file =
{
  {
    ZB_ZCL_OTA_UPGRADE_FILE_HEADER_FILE_ID,         // OTA upgrade file identifier
    ZB_ZCL_OTA_UPGRADE_FILE_HEADER_FILE_VERSION,    // OTA Header version
    0x38,                                           // OTA Header length
    0x10,                                           // OTA Header Field control
    OTA_UPGRADE_TEST_MANUFACTURER,                  // Manufacturer code
    OTA_UPGRADE_TEST_IMAGE_TYPE,                    // Image type
    OTA_UPGRADE_TEST_FILE_VERSION_NEW,              // File version
    ZB_ZCL_OTA_UPGRADE_FILE_HEADER_STACK_PRO,       // Zigbee Stack version
    /* OTA Header string */
    {
      0x54, 0x68, 0x65, 0x20, 0x6c, 0x61, 0x74, 0x65,   0x73, 0x74, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x67,
      0x72, 0x65, 0x61, 0x74, 0x65, 0x73, 0x74, 0x20,   0x75, 0x70, 0x67, 0x72, 0x61, 0x64, 0x65, 0x2e,
    },
    OTA_UPGRADE_TEST_IMAGE_SIZE,                    // Total Image size (including header)
  },
  {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
  }
};

/**
 * Declaring attributes for each cluster
 */

/* Basic cluster attributes */
zb_uint8_t g_attr_zcl_version  = ZB_ZCL_VERSION;                     /* ZCL version attribute */
zb_uint8_t g_attr_power_source = ZB_ZCL_BASIC_POWER_SOURCE_UNKNOWN;  /* Power source attribute */

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST(basic_attr_list, &g_attr_zcl_version, &g_attr_power_source);

/* OTA Upgrade server cluster attributes */
zb_uint8_t query_jitter = ZB_ZCL_OTA_UPGRADE_QUERY_JITTER_MAX_VALUE; /* QueryJitter value */
zb_uint32_t current_time = OTA_UPGRADE_TEST_CURRENT_TIME;            /* CurrentTime */

ZB_ZCL_DECLARE_OTA_UPGRADE_ATTRIB_LIST_SERVER(ota_upgrade_attr_list, &query_jitter, &current_time, 1);

/* Declare cluster list for a device */
ZB_HA_DECLARE_OTA_UPGRADE_SERVER_CLUSTER_LIST(ota_upgrade_server_clusters,
          basic_attr_list, ota_upgrade_attr_list);

/* Declare endpoint */
ZB_HA_DECLARE_OTA_UPGRADE_SERVER_EP(ota_upgrade_server_ep, ENDPOINT, ota_upgrade_server_clusters);

/* Declare application's device context for single-endpoint device */
ZB_HA_DECLARE_OTA_UPGRADE_SERVER_CTX(ota_upgrade_server_ctx, ota_upgrade_server_ep);

/* Insert upgrade OTA file into Server upgrade table */
void insert_ota_file(zb_uint8_t param)
{
  zb_ret_t ret;

  TRACE_MSG(TRACE_APP1, ">> insert_ota_file %hd", (FMT__H, param));

  ZB_ZCL_OTA_UPGRADE_INSERT_FILE(param, ENDPOINT, 0, (zb_uint8_t*)(&ota_file), OTA_UPGRADE_TEST_UPGRADE_TIME, ZB_TRUE, ret);
  ZB_ASSERT(ret == RET_OK);

  TRACE_MSG(TRACE_APP1, "<< insert_ota_file", (FMT__0));
}

/* This cb is called on next image block request
   Parameters:
   - index - index of the requested image in the image table
   - zcl_hdr - ZCL hdr of the request packet
   - offset - requested offset for the file
   - size - requested block size
   - data - OUT param, pointer to the requested block

   Possible return values:
   - RET_OK - if data set to the requested block
   - RET_ERROR - if the requested block is unavailable
   - RET_BLOCKED - if the application can't serve the request on the spot, but will send
   a response later. See zb_zcl_ota_upgrade_send_image_block_response() for further details.
*/
zb_ret_t next_data_ind_cb(zb_uint8_t index,
                          zb_zcl_parsed_hdr_t* zcl_hdr,
                          zb_uint32_t offset,
                          zb_uint8_t size,
                          zb_uint8_t** data)
{
  ZVUNUSED(index);
  ZVUNUSED(zcl_hdr);
  ZVUNUSED(size);
  *data = ((zb_uint8_t *)&ota_file + offset);
  return RET_OK;
}

MAIN()
{
  ARGV_UNUSED;

  /* Trace disable */
  /* ZB_SET_TRACE_OFF(); */
  /* Traffic dump enable */
  ZB_SET_TRAF_DUMP_ON();

  /* Global ZBOSS initialization */
  ZB_INIT("ota_simple_download_zc");

  /* Set up defaults for the commissioning */
  zb_set_long_address(g_zc_addr);
  zb_set_network_coordinator_role(1l<<21);
  zb_set_nvram_erase_at_start(ZB_FALSE);
  zb_secur_setup_nwk_key(g_key, 0);

  /* Register device ZCL context */
  ZB_AF_REGISTER_DEVICE_CTX(&ota_upgrade_server_ctx);

  /* Initialize OTA Upgrade cluster */
  zb_zcl_ota_upgrade_init_server(ENDPOINT, next_data_ind_cb);

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
  /* Get application signal from the buffer */
  zb_zdo_app_signal_hdr_t *sg_p = NULL;
  zb_zdo_app_signal_type_t sig = zb_get_app_signal(param, &sg_p);

  TRACE_MSG(TRACE_APP1, ">> zboss_signal_handler %h", (FMT__H, param));

  if (ZB_GET_APP_SIGNAL_STATUS(param) == 0)
  {
    switch(sig)
    {
#ifdef DEBUG
      case ZB_DEBUG_SIGNAL_TCLK_READY:
      {
        zb_debug_signal_tclk_ready_params_t *params = ZB_ZDO_SIGNAL_GET_PARAMS(sg_p, zb_debug_signal_tclk_ready_params_t);
        zb_debug_broadcast_aps_key(params->long_addr);
      }
      break;
#endif
      case ZB_ZDO_SIGNAL_DEFAULT_START:
      case ZB_BDB_SIGNAL_DEVICE_FIRST_START:
      case ZB_BDB_SIGNAL_DEVICE_REBOOT:
        TRACE_MSG(TRACE_APP1, "Device STARTED OK", (FMT__0));
        /* Start steering */
        bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);
        /* Register upgrade OTA file */
        ZB_SCHEDULE_APP_ALARM(insert_ota_file, param, ZB_MILLISECONDS_TO_BEACON_INTERVAL(20*1000));
        param = 0;
        break;

      default:
        TRACE_MSG(TRACE_APP1, "Unknown signal", (FMT__0));
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
