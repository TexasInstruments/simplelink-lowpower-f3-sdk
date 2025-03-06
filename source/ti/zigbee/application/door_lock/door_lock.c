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

/***** Trace related defines *****/
#define ZB_TRACE_FILE_ID 40123

/****** Application defines ******/
#define ZB_OUTPUT_ENDPOINT          6
#define ZB_OUTPUT_MAX_CMD_PAYLOAD_SIZE 2

#include <ti/log/Log.h>

#include "ti_zigbee_config.h"
#include "zboss_api.h"
#include "zb_led_button.h"
#include "zboss_api_error.h"
#include "zb_app_test_common.h"

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_fcfg.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)

/* for button handling */
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include <ti/drivers/dpl/ClockP.h>

#ifdef ZB_CONFIGURABLE_MEM
#include "zb_mem_config_lprf3.h"
#endif

/****** Application variables declarations ******/
#ifdef ZB_SHORT_ADDR
zb_uint16_t g_short_addr = ZB_SHORT_ADDR;
#endif // ZB_SHORT_ADDR
zb_uint16_t g_dst_addr;
zb_uint8_t g_addr_mode;
zb_uint8_t g_endpoint;
zb_bool_t perform_factory_reset = ZB_FALSE;

/****** Application function declarations ******/
/* Handler for specific ZCL commands */
zb_uint8_t zcl_specific_cluster_cmd_handler(zb_uint8_t param);
void test_device_interface_cb(zb_uint8_t param);
void button_press_handler(zb_uint8_t param);

/****** Cluster declarations ******/

/* Basic cluster attributes data */
zb_uint8_t g_attr_basic_zcl_version = ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_application_version = ZB_ZCL_BASIC_APPLICATION_VERSION_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_stack_version = ZB_ZCL_BASIC_STACK_VERSION_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_hw_version = ZB_ZCL_BASIC_HW_VERSION_DEFAULT_VALUE;
zb_char_t g_attr_basic_manufacturer_name[] = ZB_ZCL_BASIC_MANUFACTURER_NAME_DEFAULT_VALUE;
zb_char_t g_attr_basic_model_identifier[] = ZB_ZCL_BASIC_MODEL_IDENTIFIER_DEFAULT_VALUE;
zb_char_t g_attr_basic_date_code[] = ZB_ZCL_BASIC_DATE_CODE_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_power_source = ZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE;
zb_char_t g_attr_basic_location_description[] = ZB_ZCL_BASIC_LOCATION_DESCRIPTION_DEFAULT_VALUE;
zb_uint8_t g_attr_basic_physical_environment = ZB_ZCL_BASIC_PHYSICAL_ENVIRONMENT_DEFAULT_VALUE;
zb_char_t g_attr_sw_build_id[] = ZB_ZCL_BASIC_SW_BUILD_ID_DEFAULT_VALUE;
ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(basic_attr_list, &g_attr_basic_zcl_version, &g_attr_basic_application_version,
                                     &g_attr_basic_stack_version, &g_attr_basic_hw_version,
                                     &g_attr_basic_manufacturer_name, &g_attr_basic_model_identifier,
                                     &g_attr_basic_date_code, &g_attr_basic_power_source,
                                     &g_attr_basic_location_description, &g_attr_basic_physical_environment,
                                     &g_attr_sw_build_id);

/* Identify cluster attributes data */
zb_uint16_t g_attr_identify_identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;
/* Groups cluster attributes data */
zb_uint8_t g_attr_groups_name_support = 0;
/* Scenes cluster attributes data */
zb_uint8_t g_attr_scenes_scene_count = ZB_ZCL_SCENES_SCENE_COUNT_DEFAULT_VALUE;
zb_uint8_t g_attr_scenes_current_scene = ZB_ZCL_SCENES_CURRENT_SCENE_DEFAULT_VALUE;
zb_uint16_t g_attr_scenes_current_group = ZB_ZCL_SCENES_CURRENT_GROUP_DEFAULT_VALUE;
zb_uint8_t g_attr_scenes_scene_valid = ZB_ZCL_SCENES_SCENE_VALID_DEFAULT_VALUE;
zb_uint8_t g_attr_scenes_name_support = 0;
/* Door Lock cluster attributes data */
zb_uint8_t g_attr_door_lock_lock_state = 0;
zb_uint8_t g_attr_door_lock_lock_type = 0;
zb_bool_t g_attr_door_lock_actuator_enabled = ZB_FALSE;
ZB_ZCL_DECLARE_DOOR_LOCK_ATTRIB_LIST(door_lock_attr_list, &g_attr_door_lock_lock_state,
                                     &g_attr_door_lock_lock_type, &g_attr_door_lock_actuator_enabled);
ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(identify_attr_list, &g_attr_identify_identify_time);
ZB_ZCL_DECLARE_GROUPS_ATTRIB_LIST(groups_attr_list, &g_attr_groups_name_support);
ZB_ZCL_DECLARE_SCENES_ATTRIB_LIST(scenes_attr_list, &g_attr_scenes_scene_count, &g_attr_scenes_current_scene,
                                  &g_attr_scenes_current_group, &g_attr_scenes_scene_valid, &g_attr_scenes_name_support);
/********************* Declare device **************************/
ZB_HA_DECLARE_DOOR_LOCK_CLUSTER_LIST(door_lock_clusters,
                                     door_lock_attr_list,
                                     basic_attr_list, identify_attr_list,
                                     groups_attr_list, scenes_attr_list);
ZB_HA_DECLARE_DOOR_LOCK_EP(door_lock_ep, ZB_OUTPUT_ENDPOINT, door_lock_clusters);
ZB_HA_DECLARE_DOOR_LOCK_CTX(device_ctx, door_lock_ep);

static zb_bool_t error_ind_handler(zb_uint8_t severity,
                                   zb_ret_t error_code,
                                   void *additional_info);

MAIN()
{
  ARGV_UNUSED;

  /* Trace disable */
  ZB_SET_TRACE_OFF();
  /* Traffic dump disable */
  ZB_SET_TRAF_DUMP_OFF();

  /* Global ZBOSS initialization */
  ZB_INIT("door_lock");

  #ifdef ZB_SHORT_ADDR
  // Use the pre-defined short address, and then get the long address out of that
  // extract each of the two bytes from the short address to use as the last two bytes for the long address
  zb_uint8_t short_addr_byte1 = (g_short_addr & 0xFF00) >> 8;
  zb_uint8_t short_addr_byte2 = (g_short_addr & 0x00FF);
  zb_ieee_addr_t g_long_addr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, short_addr_byte1, short_addr_byte2};
  ZB_PIBCACHE_NETWORK_ADDRESS() = g_short_addr;
  zb_set_long_address(g_long_addr);
  #else
  /* Set the device's long address to the IEEE address pulling from the FCFG of the device */
  zb_ieee_addr_t ieee_mac_addr;
  ZB_MEMCPY(ieee_mac_addr, fcfg->deviceInfo.macAddr, 8);
  zb_set_long_address(ieee_mac_addr);
  #endif // ZB_SHORT_ADDR

#ifdef ZB_COORDINATOR_ROLE
  /* Set up defaults for the commissioning */
  zb_set_network_coordinator_role(DEFAULT_CHANLIST);
#ifdef DEFAULT_NWK_KEY
  zb_uint8_t nwk_key[16] = DEFAULT_NWK_KEY;
  zb_secur_setup_nwk_key(nwk_key, 0);
#endif //DEFAULT_NWK_KEY
  zb_set_max_children(MAX_CHILDREN);

#elif defined ZB_ROUTER_ROLE && !defined ZB_COORDINATOR_ROLE

  zb_set_network_router_role(DEFAULT_CHANLIST);
  zb_set_max_children(MAX_CHILDREN);

#elif defined ZB_ED_ROLE
  /* Set up defaults for the commissioning */
  zb_set_network_ed_role(DEFAULT_CHANLIST);

  /* Set end-device configuration parameters */
  zb_set_ed_timeout(ED_TIMEOUT_VALUE);
  zb_set_rx_on_when_idle(ED_RX_ALWAYS_ON);
#if ( ED_RX_ALWAYS_ON == ZB_FALSE )
  zb_set_keepalive_timeout(ZB_MILLISECONDS_TO_BEACON_INTERVAL(ED_POLL_RATE));
  // Disable turbo poll feature
  zb_zdo_pim_permit_turbo_poll(ZB_FALSE);
  zb_zdo_pim_set_long_poll_interval(ED_POLL_RATE);
#endif //ED_RX_ALWAYS_ON
#endif //ZB_ED_ROLE

  zb_set_nvram_erase_at_start(ZB_TRUE);

#ifdef ZB_ENABLE_PTA
  zb_enable_pta(0);
#endif

  /* Register device ZCL context */
  ZB_AF_REGISTER_DEVICE_CTX(&device_ctx);
  ZB_AF_SET_ENDPOINT_HANDLER(ZB_OUTPUT_ENDPOINT, zcl_specific_cluster_cmd_handler);
  ZB_ZCL_REGISTER_DEVICE_CB(test_device_interface_cb);

#ifdef ZB_USE_BUTTONS
  zb_button_register_handler(0, 0, button_press_handler);
#endif

  zb_error_register_app_handler(error_ind_handler);

  /* Initiate the stack start without starting the commissioning */
  if (zboss_start_no_autostart() != RET_OK)
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "zboss_start failed");
  }
  else
  {
    GPIO_setConfig(CONFIG_GPIO_BTN1, GPIO_CFG_IN_PU);
    GPIO_setConfig(CONFIG_GPIO_BTN2, GPIO_CFG_IN_PU);
    // if either button 1 or button 2 gets pressed
    zb_bool_t sideButtonPressed = ((GPIO_read((zb_uint8_t)CONFIG_GPIO_BTN1) == 0U) || (GPIO_read((zb_uint8_t)CONFIG_GPIO_BTN2) == 0U));
    // then perform a factory reset
    if (sideButtonPressed)
    {
      perform_factory_reset = ZB_TRUE;
      Log_printf(LogModule_Zigbee_App, Log_INFO, "performing factory reset");
    }
    while (1)
    {
      zboss_main_loop_iteration();
#ifdef SILK_UART_LOGGING
      // Idle task needs time to execute to flush logs
      ClockP_usleep(500);
#endif //SILK_UART_LOGGING
    }
  }

  /* Deinitialize trace */
  TRACE_DEINIT();

  MAIN_RETURN(0);
}


static zb_bool_t error_ind_handler(zb_uint8_t severity,
                                    zb_ret_t error_code,
                                    void *additional_info)
{
  zb_bool_t ret = ZB_FALSE;
  ZVUNUSED(additional_info);
  /* Unused without trace. */
  ZVUNUSED(severity);

  Log_printf(LogModule_Zigbee_App, Log_ERROR, "error_ind_handler severity %hd error_code %d", severity, error_code);

  if (error_code == ERROR_CODE(ERROR_CATEGORY_MACSPLIT, ZB_ERROR_MACSPLIT_RADIO_HANG))
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "Fatal macsplit error");
    ret = ZB_TRUE;
    /* return TRUE to prevent default error handling by the stack */
  }
  if (error_code == ERROR_CODE(ERROR_CATEGORY_MACSPLIT, ZB_ERROR_MACSPLIT_RADIO_REBOOT))
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "macsplit radio reboot");
    ret = ZB_TRUE;
  }
  Log_printf(LogModule_Zigbee_App, Log_ERROR, "error_ind_handler ret %d", ret);
  return ret;
}


void test_device_interface_cb(zb_uint8_t param)
{
  zb_zcl_device_callback_param_t *device_cb_param =
    ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "> test_device_interface_cb param %d id 0x%x",
             param, device_cb_param->device_cb_id);
  device_cb_param->status = RET_OK;
  switch (device_cb_param->device_cb_id)
  {
    case ZB_ZCL_DOOR_LOCK_LOCK_DOOR_CB_ID:
    {
      zb_uint8_t lock_state = ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_LOCKED;
      Log_printf(LogModule_Zigbee_App, Log_INFO, "Lock the door");
      ZVUNUSED(zb_zcl_set_attr_val(ZB_OUTPUT_ENDPOINT,
                                   ZB_ZCL_CLUSTER_ID_DOOR_LOCK,
                                   ZB_ZCL_CLUSTER_SERVER_ROLE,
                                   ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_ID,
                                   &lock_state,
                                   ZB_FALSE));
      zb_osif_led_on(0);
    }
    break;
    case ZB_ZCL_DOOR_LOCK_UNLOCK_DOOR_CB_ID:
    {
      zb_uint8_t lock_state = ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_UNLOCKED;
      Log_printf(LogModule_Zigbee_App, Log_INFO, "Unlock the door");
      ZVUNUSED(zb_zcl_set_attr_val(ZB_OUTPUT_ENDPOINT,
                                   ZB_ZCL_CLUSTER_ID_DOOR_LOCK,
                                   ZB_ZCL_CLUSTER_SERVER_ROLE,
                                   ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_ID,
                                   &lock_state,
                                   ZB_FALSE));
      zb_osif_led_off(0);
    }
    break;
    default:
      device_cb_param->status = RET_ERROR;
      break;
  }
  Log_printf(LogModule_Zigbee_App, Log_INFO, "< test_device_interface_cb %d", device_cb_param->status);
}

static void handle_diag_data_resp(zb_bufid_t buf)
{
  zb_zdo_get_diag_data_resp_params_t *resp_params;

  resp_params = ZB_BUF_GET_PARAM(buf, zb_zdo_get_diag_data_resp_params_t);

  ZVUNUSED(resp_params);

  Log_printf(LogModule_Zigbee_App, Log_INFO, "handle_diag_data_resp, status: %d, addr: 0x%x, lqi: %d, rssi: %d",
             resp_params->status, resp_params->short_address,
             resp_params->lqi, resp_params->rssi);

  zb_buf_free(buf);
}

static void send_diag_data_req(zb_uint16_t short_address)
{
  zb_zdo_get_diag_data_req_params_t *req;
  zb_bufid_t buf;

  buf = zb_buf_get_out();
  if (buf != ZB_BUF_INVALID)
  {
    req = ZB_BUF_GET_PARAM(buf, zb_zdo_get_diag_data_req_params_t);
    ZB_BZERO(req, sizeof(*req));

    req->short_address = short_address;
    zb_zdo_get_diag_data_async(buf, handle_diag_data_resp);
  }
  else
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "Failed to get a buffer");
  }
}

zb_uint8_t zcl_specific_cluster_cmd_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;

  Log_printf(LogModule_Zigbee_App, Log_INFO, "> zcl_specific_cluster_cmd_handler");

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  g_dst_addr = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr;
  g_endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint;
  g_addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;

  ZB_ZCL_DEBUG_DUMP_HEADER(&cmd_info);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "payload size: %i", zb_buf_len(param));

  send_diag_data_req(g_dst_addr);

  if (cmd_info.cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_CLI)
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR, "Unsupported \"from server\" command direction");
  }

  Log_printf(LogModule_Zigbee_App, Log_INFO, "< zcl_specific_cluster_cmd_handler");
  return ZB_FALSE;
}

void button_press_handler(zb_uint8_t param)
{
  ZVUNUSED(param);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "button is pressed, do nothing");
}

/* Callback to handle the stack events */
void zboss_signal_handler(zb_uint8_t param)
{
  zb_zdo_app_signal_type_t sig = zb_get_app_signal(param, NULL);

  if (ZB_GET_APP_SIGNAL_STATUS(param) == 0)
  {
    switch(sig)
    {
#ifndef ZB_MACSPLIT_HOST
      case ZB_ZDO_SIGNAL_SKIP_STARTUP:
#else
      case ZB_MACSPLIT_DEVICE_BOOT:
#endif /* ZB_MACSPLIT_HOST */

#ifdef TEST_USE_INSTALLCODE
        zb_secur_ic_str_add(g_ed_addr, g_installcode, NULL);
#endif
        zboss_start_continue();
        break;

      case ZB_BDB_SIGNAL_DEVICE_FIRST_START:
      case ZB_BDB_SIGNAL_DEVICE_REBOOT:
      {
        zb_nwk_device_type_t device_type = ZB_NWK_DEVICE_TYPE_NONE;
        device_type = zb_get_device_type();
        ZVUNUSED(device_type);
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Device (%d) STARTED OK", device_type);
        if (perform_factory_reset)
        {
          Log_printf(LogModule_Zigbee_App, Log_INFO, "Performing a factory reset.");
          zb_bdb_reset_via_local_action(0);
          perform_factory_reset = ZB_FALSE;
        }
        bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);
        break;
      }

      case ZB_BDB_SIGNAL_STEERING:
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Successful steering, start f&b target");
        ZB_SCHEDULE_APP_ALARM(zb_bdb_finding_binding_target, ZB_OUTPUT_ENDPOINT, 3 * ZB_TIME_ONE_SECOND);
        break;

      default:
        Log_printf(LogModule_Zigbee_App, Log_WARNING, "Unknown signal %d", (zb_uint16_t)sig);
    }
  }
  else if (sig == ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Production config is not present or invalid");
  }
  else
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "Device started FAILED status %d sig %d", ZB_GET_APP_SIGNAL_STATUS(param), sig);
    bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);
  }

  /* Free the buffer if it is not used */
  if (param)
  {
    zb_buf_free(param);
  }
}
