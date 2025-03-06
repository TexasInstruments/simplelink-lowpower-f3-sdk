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

#include "zboss_api.h"
#include "on_off_switch_ota_client.h"
#include "zb_osif_ota.h"
#include <ti/log/Log.h>


zb_uint8_t dl_ota_upgrade_init(zb_uint32_t image_size, zb_uint32_t image_version);
zb_ret_t dl_ota_upgrade_write_next_portion(zb_uint8_t *ptr, zb_uint32_t off, zb_uint8_t len);
zb_uint8_t dl_ota_upgrade_check_fw(zb_uint8_t param);
void dl_ota_upgrade_mark_fw_ok(void);
void dl_ota_upgrade_abort(void);
void dl_ota_upgrade_server_not_found(void);
void dl_device_reset_after_upgrade(zb_uint8_t param);

void dl_ota_start_upgrade(zb_uint8_t param)
{
  ZB_SCHEDULE_APP_ALARM(zb_zcl_ota_upgrade_init_client, param, 15*ZB_TIME_ONE_SECOND);

  Log_printf(LogModule_Zigbee_App, Log_INFO, "dl_ota_init_upgrade");
}

void dl_ota_upgrade_server_not_found(void)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "dl_ota_upgrade_server_not_found");
}

void dl_process_ota_upgrade_cb(zb_uint8_t param)
{
  zb_zcl_device_callback_param_t *device_cb_param =
          ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
  zb_zcl_ota_upgrade_value_param_t *value = &(device_cb_param->cb_param.ota_value_param);

  Log_printf(LogModule_Zigbee_App, Log_INFO, "dl_process_ota_upgrade_cb param %d", param);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "status %d", value->upgrade_status);

  switch (value->upgrade_status)
  {
    case ZB_ZCL_OTA_UPGRADE_STATUS_START:
      value->upgrade_status = dl_ota_upgrade_init(value->upgrade.start.file_length,
                                                  value->upgrade.start.file_version);
      break;

    case ZB_ZCL_OTA_UPGRADE_STATUS_RECEIVE:
      value->upgrade_status = dl_ota_upgrade_write_next_portion(value->upgrade.receive.block_data,
                                                                value->upgrade.receive.file_offset,
                                                                value->upgrade.receive.data_length);
      break;

    case ZB_ZCL_OTA_UPGRADE_STATUS_CHECK:
      value->upgrade_status = dl_ota_upgrade_check_fw(param);
      break;

    case ZB_ZCL_OTA_UPGRADE_STATUS_APPLY:
      dl_ota_upgrade_mark_fw_ok();
      value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
      break;

    case ZB_ZCL_OTA_UPGRADE_STATUS_FINISH:
      zb_zcl_ota_upgrade_file_upgraded(ZB_SWITCH_ENDPOINT);
      /* Do not reset immediately - lets finish ZCL pkts exchange etc */
      ZB_SCHEDULE_APP_ALARM(dl_device_reset_after_upgrade, 0, ZB_TIME_ONE_SECOND * 15);
      value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
      break;

    case ZB_ZCL_OTA_UPGRADE_STATUS_ABORT:
      dl_ota_upgrade_abort();
      value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
      break;

    case ZB_ZCL_OTA_UPGRADE_STATUS_SERVER_NOT_FOUND:
      dl_ota_upgrade_server_not_found();
      value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
      break;

    default:
      dl_ota_upgrade_abort();
      value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_ERROR;
      break;
  }

  Log_printf(LogModule_Zigbee_App, Log_INFO, "dl_process_ota_upgrade_cb result_status %d", value->upgrade_status);
}

void dl_device_reset_after_upgrade(zb_uint8_t param)
{

  Log_printf(LogModule_Zigbee_App, Log_INFO, "dl_ota_upgrade_check_fw");

  ZVUNUSED(param);

  zb_reset(0);
}

zb_uint8_t dl_ota_upgrade_init(zb_uint32_t image_size,
                               zb_uint32_t image_version)
{
  zb_uint8_t ret = ZB_ZCL_OTA_UPGRADE_STATUS_OK;

  if (g_dev_ctx.ota_ctx.flash_dev)
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "OTA is already in progress (is it ever possible?)");
    ret = ZB_ZCL_OTA_UPGRADE_STATUS_ERROR;
  }
  else if (!zb_osif_ota_fw_size_ok(image_size))
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO, "bad file length %d", image_size);
    ret = ZB_ZCL_OTA_UPGRADE_STATUS_ERROR;
  }
  else
  {
    /*
     * OTA server sends OTA file with OTA header at begin.
     * We support here trivial OTA file with only single image. We are not
     * interested on OTA header - will skip it.
     */
    #ifdef OTA_OFFCHIP
    g_dev_ctx.ota_ctx.flash_is_open = zb_osif_ota_open_storage();
    #endif // OTA_OFFCHIP

    g_dev_ctx.ota_ctx.total_image_size = image_size;
    g_dev_ctx.ota_ctx.fw_version = image_version;
    zb_osif_ota_mark_fw_absent();
    /* Simplify our life: sync erase space for entire FW.
       Alternetively can erase by portions in dl_ota_upgrade_write_next_portion().
     */
    zb_osif_ota_erase_fw(g_dev_ctx.ota_ctx.flash_dev, 0, g_dev_ctx.ota_ctx.total_image_size);
    ret = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
  }
  return ret;
}


zb_ret_t dl_ota_upgrade_write_next_portion(zb_uint8_t *ptr, zb_uint32_t off, zb_uint8_t len)
{
  if (zb_osif_ota_write(g_dev_ctx.ota_ctx.flash_dev, ptr, off, len, g_dev_ctx.ota_ctx.total_image_size) != ZB_ZCL_STATUS_SUCCESS)
  {
    return ZB_ZCL_OTA_UPGRADE_STATUS_ERROR;
  }

  return ZB_ZCL_OTA_UPGRADE_STATUS_OK;
}

zb_uint8_t dl_ota_upgrade_check_fw(zb_uint8_t param)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "dl_ota_upgrade_check_fw");

  if (zb_osif_ota_verify_integrity_async(g_dev_ctx.ota_ctx.flash_dev, g_dev_ctx.ota_ctx.total_image_size))
  {
    g_dev_ctx.ota_ctx.param = param;
    return ZB_ZCL_OTA_UPGRADE_STATUS_BUSY;
  }

  return ZB_ZCL_OTA_UPGRADE_STATUS_OK;
}

void zb_osif_ota_verify_integrity_done(zb_uint8_t integrity_is_ok)
{
  zb_zcl_ota_upgrade_send_upgrade_end_req(g_dev_ctx.ota_ctx.param,
    (integrity_is_ok == ZB_TRUE) ? ZB_ZCL_OTA_UPGRADE_STATUS_OK : ZB_ZCL_OTA_UPGRADE_STATUS_ERROR);
}

void dl_ota_upgrade_mark_fw_ok(void)
{
  zb_osif_ota_mark_fw_ready(g_dev_ctx.ota_ctx.flash_dev, g_dev_ctx.ota_ctx.total_image_size, g_dev_ctx.ota_ctx.fw_version);
  zb_osif_ota_close_storage(g_dev_ctx.ota_ctx.flash_dev);
  #ifdef OTA_OFFCHIP
  g_dev_ctx.ota_ctx.flash_is_open = false;
  #endif // OTA_OFFCHIP
  g_dev_ctx.ota_ctx.flash_dev = NULL;
}

void dl_ota_upgrade_abort(void)
{
  zb_osif_ota_close_storage(g_dev_ctx.ota_ctx.flash_dev);
  #ifdef OTA_OFFCHIP
  g_dev_ctx.ota_ctx.flash_is_open = false;
  #endif // OTA_OFFCHIP
  g_dev_ctx.ota_ctx.flash_dev = NULL;
}
