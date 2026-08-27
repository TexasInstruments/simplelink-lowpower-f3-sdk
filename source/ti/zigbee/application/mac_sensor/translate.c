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
#include <stdio.h> // for NULL
#include <ti/log/Log.h>
#include "zb_common.h"
#include "translate.h"

/******************************************************************************
 Constants and definitions
 *****************************************************************************/
#define USE_ZB_MLME_ASSOCIATE_INDICATION
#define USE_ZB_MLME_ASSOCIATE_CONFIRM
#define USE_ZB_MLME_BEACON_NOTIFY_INDICATION
#define USE_ZB_MLME_ORPHAN_INDICATION
#define USE_ZB_MLME_SCAN_CONFIRM
#define USE_ZB_MLME_START_CONFIRM
#define USE_ZB_MLME_POLL_CONFIRM
#define USE_ZB_MLME_COMM_STATUS_INDICATION
#define USE_ZB_MCPS_DATA_CONFIRM
#define USE_ZB_MCPS_DATA_INDICATION
#define USE_ZB_MLME_PURGE_CONFIRM
#define USE_ZB_MLME_RESET_CONFIRM
#include "zb_mac_only_stubs_with_signal_handler.h"

/******************************************************************************
 Local variables
 *****************************************************************************/
/*! MAC callback table, initialized to no callback table */
static zb_mac_callbacks_t *pMacCallbacks = (zb_mac_callbacks_t *) NULL;

/******************************************************************************
 Map all of these ZBOSS callbacks to their 15.4-equivalent headers
 so that the user doesn't have to do this on their own
 *****************************************************************************/

ZB_MLME_ASSOCIATE_INDICATION(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_associate_indication callback with buffer ID %d", param);
    if (pMacCallbacks->pAssocIndCb != NULL)
    {
        zb_mlme_associate_indication_t *ind = ZB_BUF_GET_PARAM(param, zb_mlme_associate_indication_t);
        pMacCallbacks->pAssocIndCb(ind);
    }
    zb_buf_free(param);
}

ZB_MLME_ASSOCIATE_CONFIRM(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_associate_confirm callback with buffer ID %d", param);
    if (pMacCallbacks->pAssocCnfCb != NULL)
    {
        zb_mlme_associate_confirm_t *conf = ZB_BUF_GET_PARAM(param, zb_mlme_associate_confirm_t);
        pMacCallbacks->pAssocCnfCb(conf);
    }
    zb_buf_free(param);
}

ZB_MLME_BEACON_NOTIFY_INDICATION(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_beacon_notify_indication callback with buffer ID %d", param);
    if (pMacCallbacks->pBeaconNotifyIndCb != NULL)
    {
        // Note the beacon notify indication parameters are at the beginning of the buffer, so use zb_buf_begin to retrieve the indication.
        zb_mac_beacon_notify_indication_t *ind = (zb_mac_beacon_notify_indication_t *)zb_buf_begin(param);
        pMacCallbacks->pBeaconNotifyIndCb(ind);
    }
    zb_buf_free(param);
}

ZB_MLME_ORPHAN_INDICATION(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_orphan_indication callback with buffer ID %d", param);
    if (pMacCallbacks->pOrphanIndCb != NULL)
    {
        zb_mac_orphan_ind_t *ind = (zb_mac_orphan_ind_t *)zb_buf_begin(param);
        pMacCallbacks->pOrphanIndCb(ind);
    }
    zb_buf_free(param);
}

ZB_MLME_SCAN_CONFIRM(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_scan_confirm callback with buffer ID %d", param);
    if (pMacCallbacks->pScanCnfCb != NULL)
    {
        // Note this confirmation uses ZB_BUF_GET_PARAM because zb_mlme_scan_step populates the confirmation from the tail.
        zb_mac_scan_confirm_t *conf = ZB_BUF_GET_PARAM(param, zb_mac_scan_confirm_t);
        pMacCallbacks->pScanCnfCb(conf);
    }
    zb_buf_free(param);
}

ZB_MLME_START_CONFIRM(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_start_confirm callback with buffer ID %d", param);
    if (pMacCallbacks->pStartCnfCb != NULL)
    {
        zb_ret_t status = zb_buf_get_status(param);
        pMacCallbacks->pStartCnfCb(status);
    }
    zb_buf_free(param);
}

ZB_MLME_POLL_CONFIRM(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_poll_confirm callback with buffer ID %d", param);
    if (pMacCallbacks->pPollCnfCb != NULL)
    {
        zb_ret_t status = zb_buf_get_status(param);
        Log_printf(LogModule_Zigbee_App, Log_INFO, "translate.c: Status of poll: %x", status);
        pMacCallbacks->pPollCnfCb(status);
    }
    zb_buf_free(param);
}

ZB_MLME_COMM_STATUS_INDICATION(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_comm_status_indication callback with buffer ID %d", param);
    if (pMacCallbacks->pCommStatusCb != NULL)
    {
        zb_mlme_comm_status_indication_t *ind = ZB_BUF_GET_PARAM(param, zb_mlme_comm_status_indication_t);
        pMacCallbacks->pCommStatusCb(ind);
    }
    zb_buf_free(param);
}

ZB_MCPS_DATA_CONFIRM(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mcps_data_confirm callback with buffer ID %d", param);
    if (pMacCallbacks->pDataCnfCb != NULL)
    {
        zb_mcps_data_confirm_params_t *conf = ZB_BUF_GET_PARAM(param, zb_mcps_data_confirm_params_t);
        pMacCallbacks->pDataCnfCb(conf);
    }
    zb_buf_free(param);
}

ZB_MCPS_DATA_INDICATION(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mcps_data_indication callback with buffer ID %d", param);
    if (pMacCallbacks->pDataIndCb != NULL)
    {
        zb_mcps_data_indication_param_t *ind = ZB_BUF_GET_PARAM(param, zb_mcps_data_indication_param_t);
        // Extract fields of the data indication (inspired from zb_mac_api_trace_data_indication)
        zb_mac_mhr_t mhr;
        zb_uint8_t mhr_size = zb_parse_mhr(&mhr, param);

        // Place all the key fields into the new data indication structure (this will hold all the necessary information, note that the
        // zb_mcps_data_indication_param_t structure that ZBOSS provides doesn't have these fields)
        zb_mcps_data_indication_t data_ind;
        data_ind.srcAddrMode = ZB_FCF_GET_SRC_ADDRESSING_MODE(mhr.frame_control);
        if (data_ind.srcAddrMode == ZB_ADDR_16BIT_DEV_OR_BROADCAST)
        {
            data_ind.srcAddr.addr_short = mhr.src_addr.addr_short;
        }
        else
        {
            ZB_IEEE_ADDR_COPY(data_ind.srcAddr.addr_long, mhr.src_addr.addr_long);
        }
        data_ind.srcPanId = mhr.src_pan_id;
        data_ind.dstAddrMode = ZB_FCF_GET_DST_ADDRESSING_MODE(mhr.frame_control);
        if (data_ind.dstAddrMode == ZB_ADDR_16BIT_DEV_OR_BROADCAST)
        {
            data_ind.dstAddr.addr_short = mhr.dst_addr.addr_short;
        }
        else
        {
            ZB_IEEE_ADDR_COPY(data_ind.dstAddr.addr_long, mhr.dst_addr.addr_long);
        }
        data_ind.dstPanId = mhr.dst_pan_id;
        data_ind.timestamp = ind->timestamp;
        data_ind.mpduLinkQuality = ZB_MAC_GET_LQI(param);
        data_ind.rssi = ZB_MAC_GET_RSSI(param);
        data_ind.dsn = mhr.seq_number;
        data_ind.msduLength = zb_buf_len(param) - mhr_size - ZB_TAIL_SIZE_FOR_RECEIVED_MAC_FRAME;
        data_ind.msdu = (zb_uint8_t*)zb_buf_begin(param) + mhr_size;

        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: src_pan_id: %x", data_ind.srcPanId);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: dst_pan_id: %x", data_ind.dstPanId);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: src_addr_short: %x", data_ind.srcAddr.addr_short);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: dst_addr_short: %x", data_ind.dstAddr.addr_short);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: MSDU length = %d", data_ind.msduLength);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: timestamp: %d", data_ind.timestamp);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: sequence number: %d", data_ind.dsn);

        // Populate the security fields if MAC security is used
#ifdef ZB_MAC_SECURITY
        data_ind.frameCntr = mhr.frame_counter;
        data_ind.securityLevel = mhr.security_level;
        data_ind.keyIdMode = mhr.key_id_mode;
        ZB_MEMCPY(data_ind.keySource, mhr.key_source, sizeof(mhr.key_source));
        data_ind.keyIndex = mhr.key_index;
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: security_level: %d", mhr.security_level);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: key_id_mode: %d", mhr.key_id_mode);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: key_source: %x %x", mhr.key_source[0], mhr.key_source[1]);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: key_source: %x %x", mhr.key_source[2], mhr.key_source[3]);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: key_source: %x %x", mhr.key_source[4], mhr.key_source[5]);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: key_source: %x %x", mhr.key_source[6], mhr.key_source[7]);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: key_index: %d", mhr.key_index);
        Log_printf(LogModule_Zigbee_App, Log_DEBUG, "translate.c: frame_counter: %x", mhr.frame_counter);
#endif
        pMacCallbacks->pDataIndCb(&data_ind);
    }
    zb_buf_free(param);
}

ZB_MLME_PURGE_CONFIRM(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_purge_confirm callback with buffer ID %d", param);
    if (pMacCallbacks->pPurgeCnfCb != NULL)
    {
        zb_mac_purge_confirm_t *conf = ZB_BUF_GET_PARAM(param, zb_mac_purge_confirm_t);
        pMacCallbacks->pPurgeCnfCb(conf);
    }
    zb_buf_free(param);
}

ZB_MLME_RESET_CONFIRM(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_reset_confirm callback with buffer ID %d", param);
    if (pMacCallbacks->pResetCnfCb != NULL)
    {
        zb_mlme_reset_confirm_t *conf = ZB_BUF_GET_PARAM(param, zb_mlme_reset_confirm_t);
        pMacCallbacks->pResetCnfCb(conf);
    }
    zb_buf_free(param);
}

/******************************************************************************
 Public Functions
 *****************************************************************************/
void register_callbacks(zb_mac_callbacks_t *pCallbacks)
{
    // Save the application's callback table
    pMacCallbacks = pCallbacks;
}

zb_uint16_t getPibPanIdSynchronous(void)
{
    return MAC_PIB().mac_pan_id;
}

#ifdef ZB_MAC_SECURITY
zb_uint32_t getPibFrameCounterSynchronous(void)
{
    return MAC_PIB().mac_frame_counter;
}

void setPibFrameCounterSynchronous(zb_uint32_t frameCounter)
{
    MAC_PIB().mac_frame_counter = frameCounter;
}
#endif /* ZB_MAC_SECURITY */

void zb_mlme_disassociate_confirm(zb_uint8_t param)
{
    Log_printf(LogModule_Zigbee_App, Log_VERBOSE, "translate.c: Entering zb_mlme_disassociate_confirm with buffer ID %d", param);
    if (pMacCallbacks->pDisassociateCnfCb != NULL)
    {
        zb_mac_disassociate_confirm_t *conf = ZB_BUF_GET_PARAM(param, zb_mac_disassociate_confirm_t);
        pMacCallbacks->pDisassociateCnfCb(conf);
    }
    zb_buf_free(param);
}