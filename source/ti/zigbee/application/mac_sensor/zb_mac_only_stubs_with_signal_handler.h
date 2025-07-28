/*
 * Copyright (c) 2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* PURPOSE: Stubs (empty functions) to build MAC-only stack, but also with a populated signal handler. This is a copy of zb_mac_only_stubs.h with
 * the signal handler included. This is used in the mac_sensor application to allow the sensor to go to sleep when it can. We're able to make
 * this copy because zb_mac_only_stubs.h is only used in test files, so making this copy doesn't cause any issues with the ZBOSS tack.
*/

#ifndef ZB_MAC_ONLY_STUBS_WITH_SIGNAL_HANDLER_H
#define ZB_MAC_ONLY_STUBS_WITH_SIGNAL_HANDLER_H 1

#include <ti/log/Log.h>
#include "zb_mac.h"

/*! \addtogroup ZB_MAC */
/*! @{ */


/**
   \par How to use mac-only stubs

   That stubs are created to be able to compile mac-only stack.
   The problem is that MAC calls some callbacks by name (that are .confirm and
   .indication).

   Test which uses only MAC layer, must have defines for functions which test
   re-declares and then include this header file.
 */

void zb_nwk_init()
{
}


void zb_aps_init()
{
}

void zdo_secur_init()
{
}

void zb_zdo_init()
{
}

void secur_nwk_generate_keys()
{
}

zb_ret_t zb_nwk_secure_frame(zb_bufid_t src, zb_uint_t mac_hdr_size, zb_bufid_t dst)
 {
  ZVUNUSED(src);
  ZVUNUSED(dst);
  ZVUNUSED(mac_hdr_size);
  return 0;
}

zb_ret_t zb_aps_secure_frame(zb_bufid_t src, zb_uint_t mac_hdr_size, zb_bufid_t dst, zb_secur_frame_type_t frame_type)
{
  ZVUNUSED(src);
  ZVUNUSED(dst);
  ZVUNUSED(frame_type);
  ZVUNUSED(mac_hdr_size);
  return 0;
}

void zb_zdo_check_fails(zb_uint8_t param)
{
  if ( param )
  {
    zb_buf_free(param);
  }
}

#ifndef USE_ZB_MLME_RESET_CONFIRM
void zb_mlme_reset_confirm(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif


#ifndef USE_ZB_MLME_SET_CONFIRM
void zb_mlme_set_confirm(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif


#ifndef USE_ZB_MLME_START_CONFIRM
void zb_mlme_start_confirm(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif


#ifndef USE_ZB_MCPS_DATA_CONFIRM
void zb_mcps_data_confirm(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifndef USE_ZB_MLME_BEACON_NOTIFY_INDICATION
void zb_mlme_beacon_notify_indication(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifndef USE_ZB_MLME_POLL_CONFIRM
void zb_mlme_poll_confirm(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifndef USE_ZB_MLME_ORPHAN_INDICATION
void zb_mlme_orphan_indication(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifndef USE_ZB_MLME_ASSOCIATE_CONFIRM
void zb_mlme_associate_confirm(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifndef USE_ZB_MLME_ASSOCIATE_INDICATION
void zb_mlme_associate_indication(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifndef USE_ZB_MLME_SCAN_CONFIRM
void zb_mlme_scan_confirm(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifndef USE_ZB_MCPS_DATA_INDICATION
void zb_mcps_data_indication(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifndef USE_ZB_MCPS_POLL_INDICATION
void zb_mcps_poll_indication(zb_uint8_t param)
{
#if defined ZB_MAC_TESTING_MODE
  if (zb_buf_get_status(param) != MAC_NO_DATA)
  {
    /* For mac certification tests need to send empty mac frame */
    zb_mac_resp_by_empty_frame(param);
  }
  else
#endif /* ZB_MAC_TESTING_MODE */
  {
    zb_buf_free(param);
  }
}
#endif


#ifndef USE_ZB_MLME_COMM_STATUS_INDICATION
void zb_mlme_comm_status_indication(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif


#ifndef USE_ZB_MLME_PURGE_CONFIRM
void zb_mlme_purge_confirm(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifndef USE_ZB_MLME_DUTY_CYCLE_MODE_INDICATION
void zb_mlme_duty_cycle_mode_indication(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifndef USE_ZB_PLME_CCA_CONFIRM
void zb_plme_cca_confirm(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

void zb_aps_secur_init()
{
}

void zb_nwk_neighbor_init()
{
}

void zb_nwk_nib_init(zb_bool_t is_first)
{
  (void)is_first;
}

void zb_check_oom_status(zb_uint8_t param)
{
  ZVUNUSED(param);
}

void zb_check_and_handle_oom(zb_time_t t)
{
  ZVUNUSED(t);
}

void zb_mlme_get_power_info_table_confirm(zb_uint8_t param)
{
  ZVUNUSED(param);
}

void zb_mlme_set_power_info_table_confirm(zb_uint8_t param)
{
  ZVUNUSED(param);
}

void zb_zse_init(void)
{
}

void zb_mcps_purge_indirect_queue_confirm(zb_uint8_t param)
{
  ZVUNUSED(param);
}

#ifdef ZB_ENABLE_ZCL
void zb_zcl_init(void)
{

}
#endif

#ifdef ZB_ENABLE_ZLL
void zll_init(void)
{
}
#endif

#ifdef ZB_ENABLE_ZGP
void zb_zgp_init(void)
{
}

zb_uint16_t zb_zgp_ctx_size(void)
{
  return 0U;
}
#endif

#if defined(ZB_ENABLE_ZGP_DIRECT) && !defined(ZB_ZGPD_ROLE)
void zb_gp_mcps_data_indication(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif

#ifdef ZB_PHY_TESTING_MODE
void zb_mac_phy_testing_mode_notification(zb_bufid_t param)
{
  zb_buf_free(param);
}
#endif

void zb_nwk_unlock_in(zb_uint8_t param)
{
  ZVUNUSED(param);
}

void zboss_start_continue(void)
{
}

void zboss_signal_handler(zb_uint8_t param)
{
  zb_zdo_app_signal_hdr_t *sg_p = NULL;
  zb_zdo_app_signal_type_t sig = zb_get_app_signal(param, &sg_p);

  if (ZB_GET_APP_SIGNAL_STATUS(param) == 0)
  {
    switch(sig)
    {
      case ZB_COMMON_SIGNAL_CAN_SLEEP:
      {
#ifdef ZB_USE_SLEEP
        zb_sleep_now();
#endif
        break;
      }

      default:
        Log_printf(LogModule_Zigbee_App, Log_WARNING, "zboss_signal_handler: Unknown signal %d, do nothing", sig);
    }
  }
  else
  {
    switch (sig)
    {
      default:
        Log_printf(LogModule_Zigbee_App, Log_WARNING, "zboss_signal_handler: Unknown signal %d with error status, do nothing", sig);
        break;
    }
  }

  if (param)
  {
    zb_buf_free(param);
  }
}

#ifdef ZDO_DIAGNOSTICS
void zdo_diagnostics_inc(zdo_diagnostics_counter_id_t counter_id)
{
  ZVUNUSED(counter_id);
}
#endif

#ifdef ZB_CERTIFICATION_HACKS
/* Called from mac_cr_coordinator.c:zb_handle_beacon_req(..) */
void zb_nwk_update_beacon_payload(zb_uint8_t param)
{
  zb_buf_free(param);
}
#endif /* ZB_CERTIFICATION_HACKS */

zb_bool_t zb_is_device_zed(void)
{
  return ZB_FALSE;
}

void zb_mac_cancel_scan_response(zb_bufid_t buf)
{
  if (buf)
  {
    zb_buf_free(buf);
  }
}


#ifdef ZB_MAC_CONFIGURABLE_TX_POWER
void zb_mac_set_tx_power_async_confirm(zb_bufid_t param)
{
  if (param)
  {
    zb_buf_free(param);
  }
}

void zb_mac_get_tx_power_async_confirm(zb_bufid_t param)
{
  if (param)
  {
    zb_buf_free(param);
  }
}
#endif  /* ZB_MAC_CONFIGURABLE_TX_POWER */
#endif /* ZB_MAC_ONLY_STUBS_WITH_SIGNAL_HANDLER_H */

/*! @}  */
