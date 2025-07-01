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

#include <ti/log/Log.h>

#include "on_off_switch_ota_client.h"

#include "ti_zigbee_config.h"
#include "zboss_api.h"
#include "zb_led_button.h"

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_fcfg.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)

/* for button handling */
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"

#ifdef ZB_CONFIGURABLE_MEM
#include "zb_mem_config_lprf3.h"
#endif

#if !defined ZB_ED_FUNC
#error define ZB_ED_ROLE to compile the tests
#endif

/****** Application variables declarations ******/
/* IEEE address of the device */
zb_bool_t cmd_in_progress = ZB_FALSE;
zb_bool_t perform_factory_reset = ZB_FALSE;

on_off_switch_ota_ctx_t g_dev_ctx;

/****** Application function declarations ******/
zb_uint8_t zcl_specific_cluster_cmd_handler(zb_uint8_t param);
void on_off_read_attr_resp_handler(zb_bufid_t cmd_buf);
void send_toggle_req(zb_uint8_t param);
void button_press_handler(zb_uint8_t param);
void device_interface_cb(zb_uint8_t param);

/****** Cluster declarations ******/
/* Switch config cluster attributes */

ZB_ZCL_DECLARE_ON_OFF_SWITCH_CONFIGURATION_ATTRIB_LIST(on_off_switch_config_attr_list,
                                                       &g_dev_ctx.on_off_sw_attr.attr_switch_type,
                                                       &g_dev_ctx.on_off_sw_attr.attr_switch_actions);
/* Basic cluster attributes */
ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(
  basic_attr_list,
  &g_dev_ctx.basic_attr.zcl_version,
  &g_dev_ctx.basic_attr.app_version,
  &g_dev_ctx.basic_attr.stack_version,
  &g_dev_ctx.basic_attr.hw_version,
  &g_dev_ctx.basic_attr.mf_name,
  &g_dev_ctx.basic_attr.model_id,
  &g_dev_ctx.basic_attr.date_code,
  &g_dev_ctx.basic_attr.power_source,
  &g_dev_ctx.basic_attr.location_id,
  &g_dev_ctx.basic_attr.ph_env,
  &g_dev_ctx.basic_attr.sw_build_id);

/* Identify cluster attributes */

ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(identify_attr_list,
  &g_dev_ctx.identify_attr.identify_time);

/* OTA cluster attributes */
ZB_ZCL_DECLARE_OTA_UPGRADE_ATTRIB_LIST(ota_upgrade_attr_list,
  &g_dev_ctx.ota_attr.upgrade_server,
  &g_dev_ctx.ota_attr.file_offset,
  &g_dev_ctx.ota_attr.file_version,
  &g_dev_ctx.ota_attr.stack_version,
  &g_dev_ctx.ota_attr.downloaded_file_ver,
  &g_dev_ctx.ota_attr.downloaded_stack_ver,
  &g_dev_ctx.ota_attr.image_status,
  &g_dev_ctx.ota_attr.manufacturer,
  &g_dev_ctx.ota_attr.image_type,
  &g_dev_ctx.ota_attr.min_block_reque,
  &g_dev_ctx.ota_attr.image_stamp,
  &g_dev_ctx.ota_attr.server_addr,
  &g_dev_ctx.ota_attr.server_ep,
  DL_INIT_OTA_HW_VERSION,
  DL_OTA_IMAGE_BLOCK_DATA_SIZE_MAX,
  DL_OTA_UPGRADE_QUERY_TIMER_COUNTER);

/* Declare cluster list for the device */
ZB_HA_DECLARE_ON_OFF_SWITCH_OTA_CLUSTER_LIST(on_off_switch_clusters,
                                                     on_off_switch_config_attr_list,
                                                     basic_attr_list,
                                                     identify_attr_list,
                                                     ota_upgrade_attr_list);

/* Declare endpoint */
ZB_HA_DECLARE_ON_OFF_SWITCH_EP(on_off_switch_ep, ZB_SWITCH_ENDPOINT, on_off_switch_clusters);
/* Declare application's device context for single-endpoint device */
ZB_HA_DECLARE_ON_OFF_SWITCH_CTX(on_off_switch_ctx, on_off_switch_ep);

void off_network_attention(zb_uint8_t param)
{
  ZVUNUSED(param);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "off_network_attention");
  zb_osif_led_toggle(1);

  ZB_SCHEDULE_APP_ALARM(off_network_attention, 0, 1 * ZB_TIME_ONE_SECOND);
}


void my_main_loop()
{
  while (1)
  {
    /* ... User code ... */
    zboss_main_loop_iteration();
    /* ... User code ... */
  }
}

MAIN()
{
  ARGV_UNUSED;


  g_dev_ctx.on_off_sw_attr.attr_switch_type = ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_TOGGLE;
  g_dev_ctx.on_off_sw_attr.attr_switch_actions = ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_DEFAULT_VALUE;
  g_dev_ctx.basic_attr.zcl_version = ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE;
  g_dev_ctx.basic_attr.power_source = ZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE;
  g_dev_ctx.identify_attr.identify_time = 0;

  g_dev_ctx.ota_attr.manufacturer = 0xBEBE;
  g_dev_ctx.ota_attr.image_type = 0x2340;
  g_dev_ctx.ota_attr.file_version = 0x00000001;

  /* Global ZBOSS initialization */
  ZB_INIT("on_off_switch");

  #ifdef ZB_LONG_ADDR
  // use the address that the customer set in the pre-defined symbols tab
  zb_ieee_addr_t g_long_addr = ZB_LONG_ADDR;
  zb_set_long_address(g_long_addr);
  #else
  /* Set the device's long address to the IEEE address pulling from the FCFG of the device */
  zb_ieee_addr_t ieee_mac_addr;
  ZB_MEMCPY(ieee_mac_addr, fcfg->deviceInfo.macAddr, 8);
  zb_set_long_address(ieee_mac_addr);
  #endif // ZB_LONG_ADDR

#ifdef ZB_COORDINATOR_ROLE
  zb_set_network_coordinator_role(DEFAULT_CHANLIST);
#ifdef DEFAULT_NWK_KEY
  zb_uint8_t nwk_key[16] = DEFAULT_NWK_KEY;
  zb_secur_setup_nwk_key(nwk_key, 0);
#endif //DEFAULT_NWK_KEY

#ifdef ZBOSS_REV23
  zb_nwk_set_max_ed_capacity(MAX_ED_CAPACITY);
#endif //ZBOSS_REV23

#elif defined ZB_ROUTER_ROLE && !defined ZB_COORDINATOR_ROLE
  zb_set_network_router_role(DEFAULT_CHANLIST);

#ifdef ZBOSS_REV23
  zb_nwk_set_max_ed_capacity(MAX_ED_CAPACITY);
#endif //ZBOSS_REV23

#elif defined ZB_ED_ROLE
  zb_set_network_ed_role(DEFAULT_CHANLIST);

  /* Set end-device configuration parameters */
  zb_set_ed_timeout(ED_TIMEOUT_VALUE);
  zb_set_rx_on_when_idle(ED_RX_ALWAYS_ON);
#if ( ED_RX_ALWAYS_ON == ZB_FALSE )
  zb_set_keepalive_timeout(ZB_MILLISECONDS_TO_BEACON_INTERVAL(ED_POLL_RATE));
#ifdef DISABLE_TURBO_POLL
  // Disable turbo poll feature
  zb_zdo_pim_permit_turbo_poll(ZB_FALSE);
  zb_zdo_pim_set_long_poll_interval(ED_POLL_RATE);
#endif // DISABLE_TURBO_POLL
#endif // ED_RX_ALWAYS_ON
#endif //ZB_ED_ROLE

  zb_set_nvram_erase_at_start(ZB_FALSE);

  /* Register device ZCL context */
  ZB_AF_REGISTER_DEVICE_CTX(&on_off_switch_ctx);
  /* Register cluster commands handler for a specific endpoint */
  ZB_AF_SET_ENDPOINT_HANDLER(ZB_SWITCH_ENDPOINT, zcl_specific_cluster_cmd_handler);

  /********** SP device configuration **********/
  ZB_ZCL_REGISTER_DEVICE_CB(device_interface_cb);

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
      Log_printf(LogModule_Zigbee_App, Log_INFO, "perform factory reset");
    }

    zb_osif_led_button_init();
    ZB_SCHEDULE_APP_ALARM(off_network_attention, 0, 1 * ZB_TIME_ONE_SECOND);
    
    /* Call the application-specific main loop */
    my_main_loop();
  }

  MAIN_RETURN(0);
}

static zb_bool_t finding_binding_cb(zb_int16_t status,
                                    zb_ieee_addr_t addr,
                                    zb_uint8_t ep,
                                    zb_uint16_t cluster)
{
  /* Unused without trace. */
  ZVUNUSED(status);
  ZVUNUSED(addr);
  ZVUNUSED(ep);
  ZVUNUSED(cluster);

  Log_printf(LogModule_Zigbee_App, Log_INFO, "finding_binding_cb status %d addr %x ep %d cluster %d",
             status, ((zb_uint32_t *)addr)[0], ep, cluster);
  return ZB_TRUE;
}

zb_uint8_t zcl_specific_cluster_cmd_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_bool_t unknown_cmd_received = ZB_TRUE;

  Log_printf(LogModule_Zigbee_App, Log_INFO, "> zcl_specific_cluster_cmd_handler %i", param);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "payload size: %i", zb_buf_len(param));

  if (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_CLI)
  {
    if (cmd_info->cmd_id == ZB_ZCL_CMD_DEFAULT_RESP)
    {
      unknown_cmd_received = ZB_FALSE;

      cmd_in_progress = ZB_FALSE;

      zb_buf_free(param);
    }
  }

  Log_printf(LogModule_Zigbee_App, Log_INFO, "< zcl_specific_cluster_cmd_handler %i", param);
  return ! unknown_cmd_received;
}

void send_toggle_req(zb_uint8_t param)
{
  zb_uint16_t addr = 0;

  ZB_ASSERT(param);

  if (ZB_JOINED()  && !cmd_in_progress)
  {
    cmd_in_progress = ZB_TRUE;
    Log_printf(LogModule_Zigbee_App, Log_INFO, "send_toggle_req %d - send toggle", param);

    /* Destination address and endpoint are unknown; command will be sent via binding */
    ZB_ZCL_ON_OFF_SEND_TOGGLE_REQ(
      param,
      addr,
      ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT,
      0,
      ZB_SWITCH_ENDPOINT,
      ZB_AF_HA_PROFILE_ID,
      ZB_FALSE, NULL);
  }
  else
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "send_toggle_req %d - not joined", param);
    zb_buf_free(param);
  }
}

void restart_commissioning(zb_uint8_t param)
{
  ZVUNUSED(param);
  bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);
}

void button_press_handler(zb_uint8_t param)
{
  if (!param)
  {
    /* Button is pressed, gets buffer for outgoing command */
    zb_buf_get_out_delayed(button_press_handler);
  }
  else
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "button_press_handler %d", param);
    send_toggle_req(param);
#ifndef ZB_USE_BUTTONS
    /* Do not have buttons in simulator - just start periodic on/off sending */
    ZB_SCHEDULE_APP_ALARM(button_press_handler, 0, 7 * ZB_TIME_ONE_SECOND);
#endif
  }
}

void permit_joining_cb(zb_uint8_t param)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "permit joining done");
  zb_buf_free(param);
}

void start_finding_binding(zb_uint8_t param)
{
  ZVUNUSED(param);

  Log_printf(LogModule_Zigbee_App, Log_INFO, "Successful steering, start f&b initiator");
  zb_bdb_finding_binding_initiator(ZB_SWITCH_ENDPOINT, finding_binding_cb);
}

void zboss_signal_handler(zb_uint8_t param)
{
  zb_zdo_app_signal_hdr_t *sg_p = NULL;
  zb_zdo_app_signal_type_t sig = zb_get_app_signal(param, &sg_p);
  zb_bufid_t buf;
  zb_bufid_t req_buf = 0;
  zb_zdo_mgmt_permit_joining_req_param_t *req_param;

  if (ZB_GET_APP_SIGNAL_STATUS(param) == 0)
  {
    switch(sig)
    {
      case ZB_ZDO_SIGNAL_SKIP_STARTUP:
#ifndef ZB_MACSPLIT_HOST
        Log_printf(LogModule_Zigbee_App, Log_INFO, "ZB_ZDO_SIGNAL_SKIP_STARTUP: boot, not started yet");
        zboss_start_continue();
#endif /* ZB_MACSPLIT_HOST */
        break;

#ifdef ZB_MACSPLIT_HOST
      case ZB_MACSPLIT_DEVICE_BOOT:
        Log_printf(LogModule_Zigbee_App, Log_INFO, "ZB_MACSPLIT_DEVICE_BOOT: boot, not started yet");
        zboss_start_continue();
        break;
#endif /* ZB_MACSPLIT_HOST */
      case ZB_BDB_SIGNAL_DEVICE_FIRST_START:
        Log_printf(LogModule_Zigbee_App, Log_INFO, "FIRST_START: start steering");
        if (perform_factory_reset)
        {
          // passing in 0 as the parameter means that a buffer will be allocated automatically for the reset
          zb_bdb_reset_via_local_action(0);
          perform_factory_reset = ZB_FALSE;
        }
        bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);

        buf = zb_buf_get_out();
        if (!buf)
        {
          Log_printf(LogModule_Zigbee_App, Log_WARNING, "no buffer available");
          break;
        }

        req_param = ZB_BUF_GET_PARAM(buf, zb_zdo_mgmt_permit_joining_req_param_t);

        req_param->dest_addr = 0xfffc;
        req_param->permit_duration = 0;
        req_param->tc_significance = 1;

        zb_zdo_mgmt_permit_joining_req(buf, permit_joining_cb);
  #ifdef ZB_USE_BUTTONS
        zb_button_register_handler(0, 0, button_press_handler);
  #endif
        break;
      case ZB_BDB_SIGNAL_DEVICE_REBOOT:
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Device RESTARTED OK");
        if (perform_factory_reset)
        {
          Log_printf(LogModule_Zigbee_App, Log_INFO, "Performing a factory reset.");
          zb_bdb_reset_via_local_action(0);
          perform_factory_reset = ZB_FALSE;
        }
        else
        {
          ZB_SCHEDULE_APP_ALARM_CANCEL(off_network_attention, ZB_ALARM_ANY_PARAM);
          zb_osif_led_off(1);

          buf = zb_buf_get_out();
          if (!buf)
          {
            zb_buf_get_out_delayed(dl_ota_start_upgrade);
          }
          else
          {
            dl_ota_start_upgrade(buf);
          }
        }
#ifdef ZB_USE_BUTTONS
      zb_button_register_handler(0, 0, button_press_handler);
#endif
        break;
#ifdef ZB_COORDINATOR_ROLE
      case ZB_ZDO_SIGNAL_DEVICE_ANNCE:
#else
#ifdef ZBOSS_REV23
      case ZB_SIGNAL_JOIN_DONE:
#endif // ZBOSS_REV23
      case ZB_BDB_SIGNAL_TC_REJOIN_DONE:
        Log_printf(LogModule_Zigbee_App, Log_INFO, "TC rejoin is completed successfully");
      case ZB_BDB_SIGNAL_STEERING:
#endif
      {
        zb_nwk_device_type_t device_type = ZB_NWK_DEVICE_TYPE_NONE;
        device_type = zb_get_device_type();
        ZVUNUSED(device_type);
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Device (%d) STARTED OK", device_type);
        ZB_SCHEDULE_APP_ALARM(start_finding_binding, 0, 3 * ZB_TIME_ONE_SECOND);
        ZB_SCHEDULE_APP_ALARM_CANCEL(off_network_attention, ZB_ALARM_ANY_PARAM);
#ifdef ZB_USE_BUTTONS
        zb_button_register_handler(0, 0, button_press_handler);
#endif /* ZB_USE_BUTTONS */

        buf = zb_buf_get_out();
        if (!buf)
        {
          zb_buf_get_out_delayed(dl_ota_start_upgrade);
        }
        else
        {
          dl_ota_start_upgrade(buf);
        }

        break;
      }

      case ZB_BDB_SIGNAL_FINDING_AND_BINDING_INITIATOR_FINISHED:
      {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Finding&binding done");
#ifndef ZB_USE_BUTTONS
        /* Do not have buttons in simulator - just start periodic on/off sending */
        cmd_in_progress = ZB_FALSE;
        ZB_SCHEDULE_APP_ALARM_CANCEL(button_press_handler, ZB_ALARM_ANY_PARAM);
        ZB_SCHEDULE_APP_ALARM(button_press_handler, 0, 7 * ZB_TIME_ONE_SECOND);
#endif
      }
      break;

      case ZB_ZDO_SIGNAL_LEAVE:
      break;
      case ZB_COMMON_SIGNAL_CAN_SLEEP:
      {
#ifdef ZB_USE_SLEEP
        zb_sleep_now();
#endif
        break;
      }
      case ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY:
      {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Production config is ready");
        break;
      }

      default:
        Log_printf(LogModule_Zigbee_App, Log_WARNING, "Unknown signal %d, do nothing", sig);
    }
  }
  else
  {
    switch (sig)
    {
      case ZB_BDB_SIGNAL_DEVICE_FIRST_START:
        Log_printf(LogModule_Zigbee_App, Log_WARNING, "Device can not find any network on start, so try to perform network steering");
        ZB_SCHEDULE_APP_ALARM(restart_commissioning, 0, 10 * ZB_TIME_ONE_SECOND);
        break; /* ZB_BDB_SIGNAL_DEVICE_FIRST_START */

      case ZB_BDB_SIGNAL_DEVICE_REBOOT:
        Log_printf(LogModule_Zigbee_App, Log_WARNING, "Device can not find any network on restart");

        if (zb_bdb_is_factory_new())
        {
          /* Device tried to perform TC rejoin after reboot and lost its authentication flag.
           * Do nothing here and wait for ZB_BDB_SIGNAL_TC_REJOIN_DONE to handle TC rejoin error */
          Log_printf(LogModule_Zigbee_App, Log_WARNING, "Device lost authentication flag");
        }
        else
        {
          /* Device tried to perform secure rejoin, but didn't found any networks or can't decrypt Rejoin Response
           * (it is possible when Trust Center changes network key when ZED is powered off) */
          Log_printf(LogModule_Zigbee_App, Log_WARNING, "Device is still authenticated, try to perform TC rejoin");
          ZB_SCHEDULE_APP_ALARM(zb_bdb_initiate_tc_rejoin, 0, ZB_TIME_ONE_SECOND);
        }
        break; /* ZB_BDB_SIGNAL_DEVICE_REBOOT */

      case ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY:
        Log_printf(LogModule_Zigbee_App, Log_INFO, "Production config is not present or invalid");
        break; /* ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY */

      case ZB_BDB_SIGNAL_TC_REJOIN_DONE:
        Log_printf(LogModule_Zigbee_App, Log_WARNING, "TC rejoin failed, so try it again with interval");

        ZB_SCHEDULE_APP_ALARM(zb_bdb_initiate_tc_rejoin, 0, 3 * ZB_TIME_ONE_SECOND);
        break; /* ZB_BDB_SIGNAL_TC_REJOIN_DONE */

      case ZB_BDB_SIGNAL_STEERING:
        Log_printf(LogModule_Zigbee_App, Log_WARNING, "Steering failed, retrying again in 10 seconds");
        ZB_SCHEDULE_APP_ALARM(restart_commissioning, 0, 10 * ZB_TIME_ONE_SECOND);
        break; /* ZB_BDB_SIGNAL_STEERING */

      default:
        Log_printf(LogModule_Zigbee_App, Log_WARNING, "Unknown signal %hd with error status, do nothing", sig);
        break;
    }
  }

  if (param)
  {
    zb_buf_free(param);
  }
}

void device_interface_cb(zb_uint8_t param)
{
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t mask;
  zb_zcl_device_callback_param_t *device_cb_param =
          ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
  const zb_zcl_parsed_hdr_t *in_cmd_info = ZB_ZCL_DEVICE_CMD_PARAM_CMD_INFO(param);

  Log_printf(LogModule_Zigbee_App, Log_INFO, "device_interface_cb param %d id %d",
             param, device_cb_param->device_cb_id);
  /* WARNING: Default status rewritten */
  device_cb_param->status = RET_OK;

  switch (device_cb_param->device_cb_id)
  {

    case ZB_ZCL_OTA_UPGRADE_VALUE_CB_ID:
      dl_process_ota_upgrade_cb(param);
      break;

    default:
      device_cb_param->status = RET_ERROR;
      break;
  }

  Log_printf(LogModule_Zigbee_App, Log_INFO, "device_interface_cb %d", device_cb_param->status);
}
