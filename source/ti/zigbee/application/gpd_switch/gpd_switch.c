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

#include <ti/log/Log.h>

#include "gboss_api.h"
#include "ti_zigbee_config.h"

#define COMM_STEPS_LIMIT 100

typedef enum gboss_bidir_battery_onoff_buttons_e {
  BUTTON_COMMISSIONING  = 0,
  BUTTON_ON             = 1,
  BUTTON_TOGGLE         = 2,
  BUTTON_OFF            = 3,
  BUTTONS_NUM           = 4,
} gboss_bidir_battery_onoff_buttons_t;

static const zb_uint8_t gs_commands[] = {
  GBOSS_APP_CMD_ID_OFF,
  GBOSS_APP_CMD_ID_ON,
  GBOSS_APP_CMD_ID_TOGGLE
};

static const zb_uint16_t gs_clusters_cli[] = {
  ZB_ZCL_CLUSTER_ID_BASIC,
  ZB_ZCL_CLUSTER_ID_IDENTIFY,
  ZB_ZCL_CLUSTER_ID_SCENES,
  ZB_ZCL_CLUSTER_ID_ON_OFF,
  ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL
};

static const zb_uint16_t gs_clusters_srv[] = {
  ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
  ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT
};

static const gboss_device_config_t g_device_config = {
  .manufacturer                     = 0x1234,
  .model                            = 0x0001,
  .src_addr                         = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88},
  .src_id                           = GPD_SRC_ID,
  .application_id                   = GPD_APP_ID_TYPE,
  .endpoint                         = 0x55,
  .device_id                        = GBOSS_ON_OFF_SWITCH_DEV_ID,
  .comm_options                     = {
    .mac_sequence_number_capability     = GBOSS_MAC_SEQ_NUM_INCREMENTAL,
    .rx_on_capability                   = ZB_FALSE, /* receiving capabilities in operational mode. */
    .application_info_present           = ZB_TRUE,
    .pan_id_request                     = ZB_FALSE,
    .gp_security_key_request            = ZB_FALSE,
    .fixed_location                     = ZB_FALSE,
    .extended_options_present           = ZB_TRUE},
  .comm_extended_options            = {
    .security_level_capabilities        = SECURITY_LEVEL,
    .key_type                           = SECURITY_KEY_TYPE,
    .gpd_key_present                    = ZB_TRUE,
    .gpd_key_encryption                 = ZB_TRUE,
    .gpd_outgoing_counter_present       = ZB_TRUE},
  .comm_app_info = {
    .manuf_id_present                   = ZB_TRUE,
    .model_id_present                   = ZB_TRUE,
    .gpd_commands_present               = ZB_TRUE,
    .cluster_list_present               = ZB_TRUE,
    .switch_info_present                = ZB_FALSE,
    .gpd_app_descr_command_follows      = ZB_FALSE},
  .cmd_list_count                   = ZB_ARRAY_SIZE(gs_commands),
  .cmd_list                         = gs_commands,
  .cluster_srv_cnt                  = ZB_ARRAY_SIZE(gs_clusters_srv),
  .cluster_list_srv                 = gs_clusters_srv,
  .cluster_cli_cnt                  = ZB_ARRAY_SIZE(gs_clusters_cli),
  .cluster_list_cli                 = gs_clusters_cli
};

//! A preinstalled OOB security key
static const zb_uint8_t g_gboss_encryption_key[] = SECURITY_KEY;

static const zb_uint8_t g_gboss_channels[] = CHANNEL_LIST;

static const gboss_tx_cfg_t g_tx_cfg = {
  .channels           = g_gboss_channels,
  .channel_count      = ZB_ARRAY_SIZE(g_gboss_channels),
  .channel_group_size = 3, // Channel list is split into groups when commissioning.
                           // Each button press will send the next group,
                           // until it loops back to the first group.
  .repetitions        = GPDF_REPETITIONS
};

MAIN()
{
  /* Variable where a mask of pressed buttons will be stored to */
  zb_uint32_t button_mask = 0;
  zb_uint32_t button_id = 0;
  zb_uint_t comm_steps = 0;

  ARGV_UNUSED;

  /* Platform-specific initialization */
  gboss_platform_init(ARGC_ARGV);

  /* ZPGD stack init */
  gboss_app_init(g_gboss_encryption_key, &g_device_config);

#if ( UNIDIR_DEVICE == ZB_FALSE )
  gboss_bidir_operational_init(&g_device_config, &g_tx_cfg);

#if ( BATTERYLESS_DEVICE == ZB_TRUE )
  gboss_bidir_batteryless_mode();
#else
  gboss_bidir_battery_mode();
#endif // BATTERYLESS_DEVICE == ZB_TRUE

#elif ( BATTERYLESS_DEVICE == ZB_FALSE )
    // Enable button decounce if unidirectional and not a batteryless device
    gboss_button_enable_debounce();
#endif // UNIDIR_DEVICE == ZB_FALSE

  /* Inform platform layer about buttons number */
  gboss_set_buttons_max_num(BUTTONS_NUM);

#if (BATTERYLESS_DEVICE == ZB_FALSE)
  while (1)
#endif // BATTERYLESS_DEVICE == ZB_FALSE
  {
    /* Get mask of buttons pressed at power ON time - if multiple are pressed
       simultaneously, only the higher index is considered */
    gboss_wait_for_button();
    button_mask = gboss_test_get_button_mask();
    button_id = 31 - MAGIC_CLZ(button_mask);

    switch (button_id)
    {
    case BUTTON_COMMISSIONING:

#if ( UNIDIR_DEVICE == ZB_TRUE )
      gboss_unidir_commissioning_step(&g_device_config, &g_tx_cfg);
#else

#if ( BATTERYLESS_DEVICE == ZB_TRUE )
      if (!gboss_bidir_is_commissionned())
      {
        /* In bidir batteryless we can do only one commissioning iteration a time, then die */
        gboss_bidir_commissioning_step(&g_device_config, &g_tx_cfg);
      }
#else // Battery Bidirectional Device
      /* Commissioning button pressed when alredy commissioned - let's decommission this device */
      if (gboss_bidir_is_commissionned())
      {
        gboss_bidir_decommission(&g_device_config, &g_tx_cfg);
        break;
      }

      /** [bidir_step] */
      /* In contrast to batteryless bidir commissioning, here we can do multiple
       * iterations until commissioning complete. */
      while (!gboss_bidir_is_commissionned())
      {
        gboss_bidir_commissioning_step(&g_device_config, &g_tx_cfg);

        /* If can't commission, do not loop there forever */
        comm_steps++;
        if (comm_steps > COMM_STEPS_LIMIT)
        {
          break;
        }
      }
      comm_steps = 0;
      /** [bidir_step] */

      if (!gboss_bidir_is_commissionned())
      {
        /* If could not commission, go to deep sleep to save the battery */
        continue;
      }
#endif // BATTERYLESS_DEVICE == ZB_TRUE
#endif // UNIDIR_DEVICE == ZB_TRUE
      break;

    case BUTTON_ON:
      /* Button 1 pressed - send ON */
      GBOSS_CMD_ON(&g_device_config, &g_tx_cfg);
      /** [button2] */
      break;

    case BUTTON_TOGGLE:
      /* Button 2 pressed - send TOGGLE */
      GBOSS_CMD_TOGGLE(&g_device_config, &g_tx_cfg);
      break;

    case BUTTON_OFF:
      /* Button 3 pressed - send OFF */
      GBOSS_CMD_OFF(&g_device_config, &g_tx_cfg);
      break;

    default:
      break;
    }
#if ( BATTERYLESS_DEVICE == ZB_TRUE )
    osif_deep_sleep();
#endif // BATTERYLESS_DEVICE == ZB_TRUE
  }
}
