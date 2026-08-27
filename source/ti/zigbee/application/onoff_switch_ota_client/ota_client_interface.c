/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2026, Texas Instruments Incorporated
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

/*
 * Heartbeat Logs: fires every 30s to confirm device is alive during OTA silence windows.
 * ZBOSS runs its QNI retry timer silently (~72s) between SERVER_NOT_FOUND and the
 * next Query Next Image Request. Without this, the log looks frozen.
 *
 */
#define ENABLE_OTA_CLIENT_HEARTBEAT_LOGS (0U)

zb_uint8_t dl_ota_upgrade_init(zb_uint32_t image_size, zb_uint32_t image_version);
zb_ret_t dl_ota_upgrade_write_next_portion(zb_uint8_t *ptr, zb_uint32_t off, zb_uint8_t len);
zb_uint8_t dl_ota_upgrade_check_fw(zb_uint8_t param);
void dl_ota_upgrade_mark_fw_ok(void);
void dl_ota_upgrade_abort(void);
void dl_ota_upgrade_server_not_found(void);
void dl_device_reset_after_upgrade(zb_uint8_t param);

#if (ENABLE_OTA_CLIENT_HEARTBEAT_LOGS == 1)
/* Heartbeat: fires every 30s to confirm device is alive during OTA silence windows.
 * ZBOSS runs its QNI retry timer silently (~72s) between SERVER_NOT_FOUND and the
 * next Query Next Image Request. Without this, the log looks frozen.
 *
 */
static void ota_heartbeat(zb_uint8_t param)
{
  ZVUNUSED(param);
  zb_uint32_t file_ver = zb_zcl_ota_upgrade_get32(ZB_SWITCH_ENDPOINT,
                           ZB_ZCL_ATTR_OTA_UPGRADE_FILE_VERSION_ID);
  zb_uint32_t img_status = zb_zcl_ota_upgrade_get32(ZB_SWITCH_ENDPOINT,
                              ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_STATUS_ID);
  zb_uint32_t file_offset = zb_zcl_ota_upgrade_get32(ZB_SWITCH_ENDPOINT,
                               ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);

  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "[HEARTBEAT] alive  ota_status=%lu  file_version=%lu  file_offset=%lu"
    "  flash_is_open=%d  total=%lu",
    (unsigned long)img_status,
    (unsigned long)file_ver,
    (unsigned long)file_offset,
    g_dev_ctx.ota_ctx.flash_is_open,
    (unsigned long)g_dev_ctx.ota_ctx.total_image_size);

  ZB_SCHEDULE_APP_ALARM(ota_heartbeat, 0, 30 * ZB_TIME_ONE_SECOND);
}

#endif

/* ── wrapper just for logging around ZBOSS's init ── */
static void dl_ota_init_client_and_set_size(zb_uint8_t param)
{
  /* Read ZCL_ATTR_OTA_UPGRADE_FILE_VERSION_ID — this is the device's CURRENT
   * running version stored in the ZCL attribute table (loaded from NVRAM on boot).
   * Inside query_next_image_resp_handler, ZBOSS checks:
   *   if (response.file_version == this_attribute_value) → DENY (ZCL8 rule: same version)
   * If this prints 2 on a V1 device → NVRAM has stale V2 version → root cause of denial. */
  zb_uint32_t cur_file_ver = zb_zcl_ota_upgrade_get32(ZB_SWITCH_ENDPOINT,
                               ZB_ZCL_ATTR_OTA_UPGRADE_FILE_VERSION_ID);
  zb_uint32_t cur_downloaded_ver = zb_zcl_ota_upgrade_get32(ZB_SWITCH_ENDPOINT,
                                     ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID);

  Log_printf(LogModule_Zigbee_App, Log_VERBOSE,
    "[OTA] init_client FIRING param=%d  flash_is_open=%d",
    param, g_dev_ctx.ota_ctx.flash_is_open);
  Log_printf(LogModule_Zigbee_App, Log_VERBOSE,
    "[OTA] ZCL attrs: file_version=%lu  downloaded_file_version=%lu"
    "  (if file_version==server_version ZBOSS will DENY)",
    (unsigned long)cur_file_ver, (unsigned long)cur_downloaded_ver);
  Log_printf(LogModule_Zigbee_App, Log_VERBOSE,
    "[OTA] app ota_attr.file_version=0x%08lX  fw_version_ctx=0x%08lX",
    (unsigned long)g_dev_ctx.ota_attr.file_version,
    (unsigned long)g_dev_ctx.ota_ctx.fw_version);

  zb_zcl_ota_upgrade_init_client(param);

  Log_printf(LogModule_Zigbee_App, Log_VERBOSE,
    "[OTA] init_client DONE  — ZBOSS OTA discovery started (Match Desc sent)");
}

void dl_ota_start_upgrade(zb_uint8_t param)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "[OTA] dl_ota_start_upgrade: scheduling init in 15s param=%d", param);
  ZB_SCHEDULE_APP_ALARM(dl_ota_init_client_and_set_size, param, 15*ZB_TIME_ONE_SECOND);

#if (ENABLE_OTA_CLIENT_HEARTBEAT_LOGS == 1)
  /* Enable 30s heartbeat logs during silent ZBOSS windows */
  ZB_SCHEDULE_APP_ALARM(ota_heartbeat, 0, 30 * ZB_TIME_ONE_SECOND);
#endif
}

void dl_ota_upgrade_server_not_found(void)
{
  /* This fires when ZBOSS's OTA discovery times out.
   * Possible reasons:
   *   A) ZDO Match Desc found no OTA server → QNI Request never sent
   *   B) QNI Response arrived but ZBOSS denied it BEFORE calling STATUS_START:
   *      - response.file_version == device file_version attr (ZCL8 same-version rule)
   *      - ZB_ZCL_OTA_UPGRADE_GET_QUERY_NEXT_IMAGE_RES parsing failed
   *   C) QNI Response never arrived (coordinator not responding)
   * To distinguish: check PC tool — did it log MT_OTA_NEXT_IMG_REQ this run?
   * If NO  → reason A or C (QNI Request was never sent or coordinator down)
   * If YES → reason B (ZBOSS received response but denied it silently) */
  zb_uint32_t file_ver = zb_zcl_ota_upgrade_get32(ZB_SWITCH_ENDPOINT,
                           ZB_ZCL_ATTR_OTA_UPGRADE_FILE_VERSION_ID);
  zb_uint32_t dl_ver   = zb_zcl_ota_upgrade_get32(ZB_SWITCH_ENDPOINT,
                           ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID);

  Log_printf(LogModule_Zigbee_App, Log_WARNING,
    "[OTA] SERVER_NOT_FOUND  flash_is_open=%d  total=%lu  fw_ver_ctx=0x%08lX",
    g_dev_ctx.ota_ctx.flash_is_open,
    (unsigned long)g_dev_ctx.ota_ctx.total_image_size,
    (unsigned long)g_dev_ctx.ota_ctx.fw_version);
  Log_printf(LogModule_Zigbee_App, Log_WARNING,
    "[OTA] ZCL file_version_attr=%lu  downloaded_file_version_attr=%lu"
    "  app_file_version=%lu",
    (unsigned long)file_ver, (unsigned long)dl_ver,
    (unsigned long)g_dev_ctx.ota_attr.file_version);
}

void dl_process_ota_upgrade_cb(zb_uint8_t param)
{
  zb_zcl_device_callback_param_t *device_cb_param =
          ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
  zb_zcl_ota_upgrade_value_param_t *value = &(device_cb_param->cb_param.ota_value_param);

  Log_printf(LogModule_Zigbee_App, Log_VERBOSE,
    "[OTA] CB ENTER status=%d param=%d", value->upgrade_status, param);

  switch (value->upgrade_status)
  {
    /* ── ZBOSS accepted QNI Response and is starting download ── */
    case ZB_ZCL_OTA_UPGRADE_STATUS_START:
      Log_printf(LogModule_Zigbee_App, Log_VERBOSE,
        "[OTA] STATUS_START: file_length=%lu  file_version=0x%08lX",
        (unsigned long)value->upgrade.start.file_length,
        (unsigned long)value->upgrade.start.file_version);
      value->upgrade_status = dl_ota_upgrade_init(value->upgrade.start.file_length,
                                                  value->upgrade.start.file_version);
      Log_printf(LogModule_Zigbee_App, Log_VERBOSE,
        "[OTA] STATUS_START done  ret=%d  flash_is_open=%d  total=%lu",
        value->upgrade_status,
        g_dev_ctx.ota_ctx.flash_is_open,
        (unsigned long)g_dev_ctx.ota_ctx.total_image_size);
      break;

    /* ── Image Block Response: write a chunk to flash ── */
    case ZB_ZCL_OTA_UPGRADE_STATUS_RECEIVE:
    {
      zb_uint32_t off   = value->upgrade.receive.file_offset;
      zb_uint8_t  len   = value->upgrade.receive.data_length;
      zb_uint32_t total = g_dev_ctx.ota_ctx.total_image_size;

      /* log first block, every ~5%, and last block */
      if (total > 0 &&
          (off == 0 ||
           (off % (total / 20)) < (zb_uint32_t)len ||
           (off + len) >= total))
      {
        Log_printf(LogModule_Zigbee_App, Log_INFO,
          "[OTA] RECEIVE off=%lu len=%d  written=%lu/%lu (%lu%%)",
          (unsigned long)off, len,
          (unsigned long)(off + len), (unsigned long)total,
          (unsigned long)((off + len) * 100 / total));
      }

      value->upgrade_status = dl_ota_upgrade_write_next_portion(
        value->upgrade.receive.block_data, off, len);

      if (value->upgrade_status != ZB_ZCL_OTA_UPGRADE_STATUS_OK)
      {
        Log_printf(LogModule_Zigbee_App, Log_ERROR,
          "[OTA] RECEIVE WRITE FAILED  off=%lu len=%d ret=%d",
          (unsigned long)off, len, value->upgrade_status);
      }
      break;
    }

    /* ── All blocks received, verify integrity ── */
    case ZB_ZCL_OTA_UPGRADE_STATUS_CHECK:
      Log_printf(LogModule_Zigbee_App, Log_VERBOSE,
        "[OTA] STATUS_CHECK  total_image_size=%lu",
        (unsigned long)g_dev_ctx.ota_ctx.total_image_size);
      value->upgrade_status = dl_ota_upgrade_check_fw(param);
      Log_printf(LogModule_Zigbee_App, Log_VERBOSE,
        "[OTA] STATUS_CHECK ret=%d", value->upgrade_status);
      break;

    /* ── Integrity passed, apply (write BOOT_MAGIC) ── */
    case ZB_ZCL_OTA_UPGRADE_STATUS_APPLY:
      Log_printf(LogModule_Zigbee_App, Log_INFO,
        "[OTA] STATUS_APPLY  size=%lu  fw_version=0x%08lX",
        (unsigned long)g_dev_ctx.ota_ctx.total_image_size,
        (unsigned long)g_dev_ctx.ota_ctx.fw_version);
      dl_ota_upgrade_mark_fw_ok();
      value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
      break;

    /* ── Device should reset and boot new image ── */
    case ZB_ZCL_OTA_UPGRADE_STATUS_FINISH:
      Log_printf(LogModule_Zigbee_App, Log_INFO,
        "[OTA] STATUS_FINISH — OTA complete, resetting in 15s");
      zb_zcl_ota_upgrade_file_upgraded(ZB_SWITCH_ENDPOINT);
      ZB_SCHEDULE_APP_ALARM(dl_device_reset_after_upgrade, 0, ZB_TIME_ONE_SECOND * 15);
      value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
      break;

    /* ── Download aborted (error / server abort) ── */
    case ZB_ZCL_OTA_UPGRADE_STATUS_ABORT:
      Log_printf(LogModule_Zigbee_App, Log_WARNING,
        "[OTA] STATUS_ABORT  flash_is_open=%d  total_so_far=%lu",
        g_dev_ctx.ota_ctx.flash_is_open,
        (unsigned long)g_dev_ctx.ota_ctx.total_image_size);
      dl_ota_upgrade_abort();
      value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
      break;

    /* ── ZBOSS discovery timed out or server rejected ── */
    case ZB_ZCL_OTA_UPGRADE_STATUS_SERVER_NOT_FOUND:
      dl_ota_upgrade_server_not_found();
      value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
      break;

    default:
      Log_printf(LogModule_Zigbee_App, Log_WARNING,
        "[OTA] CB unknown status=%d → abort", value->upgrade_status);
      dl_ota_upgrade_abort();
      value->upgrade_status = ZB_ZCL_OTA_UPGRADE_STATUS_ERROR;
      break;
  }

  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "[OTA] CB EXIT  result=%d", value->upgrade_status);
}

void dl_device_reset_after_upgrade(zb_uint8_t param)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "[OTA] RESETTING device now");
  ZVUNUSED(param);
  zb_reset(0);
}

zb_uint8_t dl_ota_upgrade_init(zb_uint32_t image_size, zb_uint32_t image_version)
{
  zb_uint8_t ret = ZB_ZCL_OTA_UPGRADE_STATUS_OK;

  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "[OTA] upgrade_init ENTER  image_size=%lu  version=0x%08lX  flash_is_open=%d",
    (unsigned long)image_size, (unsigned long)image_version,
    g_dev_ctx.ota_ctx.flash_is_open);

  if (g_dev_ctx.ota_ctx.flash_dev)
  {
    Log_printf(LogModule_Zigbee_App, Log_WARNING,
      "[OTA] upgrade_init: already in progress — aborting previous session");
    dl_ota_upgrade_abort();
  }

  if (!zb_osif_ota_fw_size_ok(image_size))
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR,
      "[OTA] upgrade_init: fw_size_ok FAILED  image_size=%lu", (unsigned long)image_size);
    ret = ZB_ZCL_OTA_UPGRADE_STATUS_ERROR;
  }
  else
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO,
      "[OTA] upgrade_init: fw_size_ok PASSED — opening storage");
    /* flash_dev = open_storage() stores ZB_TRUE=(void*)1 as sentinel.
     * Not a real pointer — ZVUNUSED(dev) in all OSIF functions.
     * NULL = closed, (void*)1 = open. Original author's design. */
    g_dev_ctx.ota_ctx.flash_dev = (void *)zb_osif_ota_open_storage();
    g_dev_ctx.ota_ctx.total_image_size = image_size;
    g_dev_ctx.ota_ctx.fw_version = image_version;
    zb_osif_ota_mark_fw_absent();
    Log_printf(LogModule_Zigbee_App, Log_INFO,
      "[OTA] upgrade_init: erasing secondary slot  size=%lu", (unsigned long)image_size);
    zb_osif_ota_erase_fw(g_dev_ctx.ota_ctx.flash_dev, 0, g_dev_ctx.ota_ctx.total_image_size);
    Log_printf(LogModule_Zigbee_App, Log_INFO,
      "[OTA] upgrade_init: erase done  flash_is_open=%d",
      g_dev_ctx.ota_ctx.flash_is_open);
    ret = ZB_ZCL_OTA_UPGRADE_STATUS_OK;
  }

  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "[OTA] upgrade_init EXIT  ret=%d", ret);
  return ret;
}

zb_ret_t dl_ota_upgrade_write_next_portion(zb_uint8_t *ptr, zb_uint32_t off, zb_uint8_t len)
{
  zb_uint8_t rc = zb_osif_ota_write(g_dev_ctx.ota_ctx.flash_dev, ptr, off, len,
                                     g_dev_ctx.ota_ctx.total_image_size);
  if (rc != ZB_ZCL_STATUS_SUCCESS)
  {
    Log_printf(LogModule_Zigbee_App, Log_ERROR,
      "[OTA] write_portion: FAILED  off=%lu len=%d rc=%d",
      (unsigned long)off, len, rc);
    return ZB_ZCL_OTA_UPGRADE_STATUS_ERROR;
  }

  return ZB_ZCL_OTA_UPGRADE_STATUS_OK;
}

zb_uint8_t dl_ota_upgrade_check_fw(zb_uint8_t param)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "[OTA] check_fw  total=%lu", (unsigned long)g_dev_ctx.ota_ctx.total_image_size);

  if (zb_osif_ota_verify_integrity_async(g_dev_ctx.ota_ctx.flash_dev,
                                          g_dev_ctx.ota_ctx.total_image_size))
  {
    Log_printf(LogModule_Zigbee_App, Log_INFO,
      "[OTA] check_fw: async verify started — returning BUSY");
    g_dev_ctx.ota_ctx.param = param;
    return ZB_ZCL_OTA_UPGRADE_STATUS_BUSY;
  }

  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "[OTA] check_fw: verify done inline — returning OK");
  return ZB_ZCL_OTA_UPGRADE_STATUS_OK;
}

void zb_osif_ota_verify_integrity_done(zb_uint8_t integrity_is_ok)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "[OTA] verify_integrity_done  ok=%d", integrity_is_ok);
  zb_zcl_ota_upgrade_send_upgrade_end_req(g_dev_ctx.ota_ctx.param,
    (integrity_is_ok == ZB_TRUE) ? ZB_ZCL_OTA_UPGRADE_STATUS_OK
                                  : ZB_ZCL_OTA_UPGRADE_STATUS_ERROR);
}

void dl_ota_upgrade_mark_fw_ok(void)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "[OTA] mark_fw_ok: calling mark_fw_ready  size=%lu  ver=0x%08lX",
    (unsigned long)g_dev_ctx.ota_ctx.total_image_size,
    (unsigned long)g_dev_ctx.ota_ctx.fw_version);
  zb_osif_ota_mark_fw_ready(g_dev_ctx.ota_ctx.flash_dev,
                             g_dev_ctx.ota_ctx.total_image_size,
                             g_dev_ctx.ota_ctx.fw_version);
  zb_osif_ota_close_storage(g_dev_ctx.ota_ctx.flash_dev);
  g_dev_ctx.ota_ctx.flash_is_open = false;
  g_dev_ctx.ota_ctx.flash_dev = NULL;
  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "[OTA] mark_fw_ok: storage closed");
}

void dl_ota_upgrade_abort(void)
{
  Log_printf(LogModule_Zigbee_App, Log_WARNING,
    "[OTA] upgrade_abort  flash_is_open=%d", g_dev_ctx.ota_ctx.flash_is_open);
  zb_osif_ota_close_storage(g_dev_ctx.ota_ctx.flash_dev);
  g_dev_ctx.ota_ctx.flash_is_open = false;
  g_dev_ctx.ota_ctx.flash_dev = NULL;
}
