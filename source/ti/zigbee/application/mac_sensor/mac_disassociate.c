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
#include <ti/log/Log.h>

#include "zb_common.h"
#include "translate.h"
#include "mac_disassociate.h"

#ifdef ZB_COMPILE_MAC_MONOLITHIC

#include "zb_scheduler.h"
#include "zb_mac.h"
#include "zb_mac_globals.h"
#include "mac_internal.h"
#include "zb_mac_transport.h"
#include "zb_secur.h"

void zb_direct_mlme_disassociate_req_tx_done(zb_uint8_t param);
void zb_handle_mlme_disassociate_req(zb_uint8_t param);

void zb_mac_send_disassociate_conf(zb_bufid_t param, zb_mac_status_t status, zb_uint8_t cut_mhr)
{
    // Dereference because we're going to be using this exact same buffer to populate the confirmation. Need a read-only copy of the
    // request parameters since it will be used as a reference in the confirmation.
    zb_mlme_disassociate_request_t disassoc_req_params = *ZB_BUF_GET_PARAM(param, zb_mlme_disassociate_request_t);
    zb_mac_disassociate_confirm_t *disassoc_confirm_params;

    zb_mac_mhr_t mhr = {0};

    Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_mac_send_disassociate_conf: param %d, status %d", param, status);

    // Remove the MAC header from this buffer.
    if (cut_mhr)
    {
        (void)zb_buf_cut_left(param, zb_parse_mhr(&mhr, param));
    }
    // Now populate the confirmation, then pass it up to the translation layer.
    disassoc_confirm_params = ZB_BUF_GET_PARAM(param, zb_mac_disassociate_confirm_t);
    disassoc_confirm_params->status = status;
    // The address mode of the device that has either requested disassociation or was instructed by its coordinator to disassociate.
    disassoc_confirm_params->device_addr_mode = disassoc_req_params.addrMode;
    disassoc_confirm_params->device_pan_id = disassoc_req_params.devicePanId;
    // Copy address based on union
    if (disassoc_req_params.addrMode == ZB_ADDR_64BIT_DEV)
    {
        ZB_IEEE_ADDR_COPY(disassoc_confirm_params->device_addr.addr_long, disassoc_req_params.deviceAddress.addr_long);
    }
    else
    {
        disassoc_confirm_params->device_addr.addr_short = disassoc_req_params.deviceAddress.addr_short;
    }
    ZB_SCHEDULE_CALLBACK(zb_mlme_disassociate_confirm, param);
}

/**
  7.1.4.1 MLME-DISASSOCIATE.request

  Just call zb_handle_mlme_disassociate_req via tx q
*/
void zb_mlme_disassociate_request(zb_uint8_t param)
{
    zb_mlme_disassociate_request_t *disassoc_req = ZB_BUF_GET_PARAM(param, zb_mlme_disassociate_request_t);

    ZB_ASSERT(param);

    // Note that the TX indirect field will always be false in the disassociation request, as is the case in 15.4.
    /* If case of indirect tx do not need to wait for tx complete before put
     * into indirect q. */
    if (disassoc_req->txIndirect)
    {
        Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_mlme_disassociate_request: indirect tx param %d", param);
        zb_handle_mlme_disassociate_req(param);
    }
    else
    {
        zb_bool_t success = ZB_FALSE;

        if (MAC_CTX().flags.mlme_scan_in_progress == 0U)
        {
            success = (ZB_SCHEDULE_TX_CB(zb_handle_mlme_disassociate_req, param) == RET_OK);
        }

        if (!success)
        {
            // If we couldn't schedule the TX callback, it means the TX queue is full. Send a confirmation with a failed status,
            // and don't set the cut MAC header argument to true because the buffer never had one at this point.
            zb_mac_send_disassociate_conf(param, MAC_TRANSACTION_OVERFLOW, ZB_FALSE);
        }
    }
}

/**
   handle mac disassociate request, caller side

   That routine is called via tx q
 */
void zb_handle_mlme_disassociate_req(zb_uint8_t param)
{
    zb_uint8_t *ptr;
    zb_mac_mhr_t mhr = {0};
    zb_ret_t ret = RET_OK;
    zb_uint8_t mhr_len;
    zb_uint8_t packet_length; // Should be the length of the MAC header plus two additional payload bytes (command ID and disassociation reason)
    zb_mlme_disassociate_request_t *disassoc_req_params;

    disassoc_req_params = ZB_BUF_GET_PARAM(param, zb_mlme_disassociate_request_t);

    ZB_SET_MAC_STATUS(MAC_SUCCESS);

    // Without security, this should be 15 bytes
    mhr_len = zb_mac_calculate_mhr_length(
        ZB_ADDR_64BIT_DEV,
        disassoc_req_params->addrMode,
        /* This last field should always be true. There shouldn't be a use case where the sensor and collector are interacting via different networks. */
        (disassoc_req_params->devicePanId == MAC_PIB().mac_pan_id));
    
    Log_printf(LogModule_Zigbee_App, Log_DEBUG, "zb_handle_mlme_disassociate_req: Header size is %d bytes", mhr_len);

    /* In the current implementation only DATA can be encrypted, so it is ok to
    * analyze security only here. */
// #ifdef ZB_MAC_SECURITY
//     if (disassoc_req_params->security_level)
//     {
//         /* currently hard-code security level 5 and key_id_mode 3 */
//         mhr_len += MAC_SECUR_LEV5_KEYID3_AUX_HDR_SIZE; /* control(1), frame counter(4), key id (9) */
//         // Need to OR the flag correctly in order to actually encrypt the MAC data request packet. This means to retrieve
//         // header of the buffer ID and then set the flag.
//         zb_buf_flags_or(disassoc_req, ZB_BUF_SECUR_MAC_ENCR);

//         if (MAC_PIB().mac_frame_counter == (zb_uint32_t)~0)
//         {
//             ret = RET_ERROR;
//             ZB_SET_MAC_STATUS(MAC_COUNTER_ERROR);
//             return ret;
//         }
//     }
// #endif

    packet_length = mhr_len + (sizeof(zb_uint8_t) * 2U);
    ptr = zb_buf_initial_alloc(MAC_CTX().operation_buf, packet_length);
    ZB_ASSERT(ptr);
    ZB_BZERO(ptr, packet_length);

    /*
    7.3.3 Disassociation notification command
    - fill frame control:
    - source addr mode = ZB_ADDR_64BIT_DEV
    - dst addr mode = req.coord_addr_mode
    - frame pending = 0
    - ack request = 1
    - fill MHR
    - dst pan id = req.coord_pan_id
    - dst address = req.coord_addr
    - src addr = device extended address
    - command id = MAC_FRAME_COMMAND (because this is a command to disassociate)
    - disassoc reason = req.disassoc_reason
    */

    /* Fill Frame Control then call zb_mac_fill_mhr()
    mac spec  7.2.1.1 Frame Control field
    | Frame Type | Security En | Frame Pending | Ack.Request | PAN ID Compress | Reserved | Dest.Addr.Mode | Frame Ver | Src.Addr.gMode |
    */
    ZB_BZERO2(mhr.frame_control);
    // We are sending a disassociation request, which is a command frame.
    ZB_FCF_SET_FRAME_TYPE(mhr.frame_control, MAC_FRAME_COMMAND);

    /* security enable is 0 */
    /* frame pending is 0 */

    // Taking the following from what I saw over the air with F2 collector-sensor interaction
    ZB_FCF_SET_ACK_REQUEST_BIT(mhr.frame_control, 1U);
    ZB_FCF_SET_PANID_COMPRESSION_BIT(mhr.frame_control, 1U);
    ZB_FCF_SET_DST_ADDRESSING_MODE(mhr.frame_control, disassoc_req_params->addrMode);
    ZB_FCF_SET_SRC_ADDRESSING_MODE(mhr.frame_control, ZB_ADDR_64BIT_DEV);

    /* 7.2.3 Frame compatibility: All unsecured frames specified in this
        standard are compatible with unsecured frames compliant with IEEE Std 802.15.4-2003 */
    ZB_FCF_SET_FRAME_VERSION(mhr.frame_control, MAC_FRAME_IEEE_802_15_4_2003);

// #ifdef ZB_MAC_SECURITY
//     if (disassoc_req_params->security_level)
//     {
//         ZB_FCF_SET_SECURITY_BIT(mhr.frame_control, 1);
//         /* frame security compatible with 2006 */
//         ZB_FCF_SET_FRAME_VERSION(mhr.frame_control, MAC_FRAME_IEEE_802_15_4);
//     }
// #endif

    /* mac spec 7.5.6.1 Transmission */
    mhr.seq_number = ZB_MAC_DSN();
    ZB_INC_MAC_DSN();

    // Taking what I saw over the air with F2 collector-sensor interaction
    mhr.dst_pan_id = disassoc_req_params->devicePanId;
    mhr.dst_addr.addr_short = disassoc_req_params->deviceAddress.addr_short;
    mhr.src_pan_id = MAC_PIB().mac_pan_id;
    ZB_IEEE_ADDR_COPY(&mhr.src_addr.addr_long, MAC_PIB().mac_extended_address);

    zb_mac_fill_mhr(ptr, &mhr);

// #ifdef ZB_MAC_SECURITY
//     if (disassoc_req_params->security_level)
//     {
//         /* fill Aux security header */
//         ptr += (mhr_len - MAC_SECUR_LEV5_KEYID3_AUX_HDR_SIZE);
//         /* security control: always level 5, key id mode 3 */
//         *ptr = ZB_MAC_SECURITY_LEVEL | (ZB_MAC_KEY_ID_MODE << 3);
//         ptr++;
//         /* frame counter */
//         ZB_HTOLE32(ptr, &MAC_PIB().mac_frame_counter);
//         MAC_PIB().mac_frame_counter++;
//         ptr += 4;
//         /* key source */
//         ZB_MEMCPY(ptr, disassoc_req_params->key_source, 8);
//         ptr += 8;
//         /* key identifier */
//         *ptr = disassoc_req_params->key_index;
//     }
// #else
    // Because there's nothing else in the header that needs to be filled
    ptr += mhr_len;
// #endif /* ZB_MAC_SECURITY */

    /* | MHR | Command frame id 1 byte | Capability info 1 byte | */
    // After adding in the MAC header, all that's left in the payload is the command frame ID (disassociation request) and the disassociation reason. Both are one byte.
    *ptr = MAC_CMD_DISASSOCIATION_NOTIFICATION;
    ptr++;
    *ptr = disassoc_req_params->disassociateReason;

// #ifdef ZB_MAC_SECURITY
//     if (ret == RET_OK)
//     {
//         ret = zb_mac_secure_frame(param, mhr_len, GET_MONOLITHIC_MAC_ENCRYPTION_BUF());
//         if (ret != RET_OK)
//         {
//             Log_printf(LogModule_Zigbee_App, Log_ERROR, "zb_mac_secure_frame() failed with ret = %d", ret);
//             ZB_SET_MAC_STATUS(MAC_SECURITY_ERROR);
//         }
//     }
// #endif /* ZB_MAC_SECURITY */

    if (ret == RET_OK)
    {
        // Function to call once the TX is complete
        MAC_CTX().tx_wait_cb = zb_direct_mlme_disassociate_req_tx_done;
        MAC_CTX().tx_wait_cb_arg = param;

        // if (ZB_BIT_IS_SET(zb_buf_flags_get(param), ZB_BUF_SECUR_ALL_ENCR))
        // {
        //     Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_handle_mlme_disassocation_req: Sending secure disassociation request with param %d", GET_MONOLITHIC_MAC_ENCRYPTION_BUF());
        //     zb_mac_send_frame(GET_MONOLITHIC_MAC_ENCRYPTION_BUF(), mhr_len);
        // }
        // else
        // {
            Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_handle_mlme_disassocation_req: Sending disassociation request with param %d and reason %d", param, disassoc_req_params->disassociateReason);
            zb_mac_send_frame(MAC_CTX().operation_buf, mhr_len);
        // }
    }
    else if (ret == RET_PENDING)
    {
        /* Do nothing for RET_PENDING error code. The packet is put to pending queue. */
        TRACE_MSG(TRACE_MAC3, "RET_PENDING for param %hd", (FMT__H, param));
    }
    else
    {
        ZB_ASSERT(ZB_GET_MAC_STATUS() != MAC_SUCCESS);
        zb_direct_mlme_disassociate_req_tx_done(param);
    }
}

/**
   Callback called when the direct disassociation request transmission is done.
 */
void zb_direct_mlme_disassociate_req_tx_done(zb_uint8_t param)
{
    // This buffer should eventually get freed at some point?
    Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_direct_mlme_disassociate_req_tx_done: param %d, status %d", param, ZB_GET_MAC_STATUS());

    // The status will be set with ZB_SET_MAC_STATUS in mac.c. Set the cut MAC header argument to true because it will be present in the buffer by this point.
    zb_mac_send_disassociate_conf(param, ZB_GET_MAC_STATUS(), ZB_TRUE);
}

#endif /* ZB_COMPILE_MAC_MONOLITHIC */