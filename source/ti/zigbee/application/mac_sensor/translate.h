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

#ifndef TRANSLATE_H
#define TRANSLATE_H 1

#include "zb_mac.h"

/*! MCPS data indication type (inspired by 15.4 stack version) */
typedef ZB_PACKED_PRE struct zb_mcps_data_indication_s
{
    /*! The addressing mode of the source device */
    zb_uint8_t srcAddrMode;
    /*! The address of the sending device */
    zb_addr_u srcAddr;
    /*! The addressing mode of the destination device */
    zb_uint8_t dstAddrMode;
    /*! The address of the destination device */
    zb_addr_u dstAddr;
    /*! The time, in radio ticks, at which the data were received */
    zb_time_t timestamp;
    /*! The PAN ID of the sending device */
    zb_uint16_t srcPanId;
    /*! The PAN ID of the destination device */
    zb_uint16_t dstPanId;
    /*! The link quality of the received data frame */
    zb_uint8_t mpduLinkQuality;
    /*! The received RF power in units dBm */
    zb_int8_t rssi;
    /*! The data sequence number of the received frame */
    zb_uint8_t dsn;
    /*! Frame counter value of the received data frame (if used) */
    zb_uint32_t frameCntr;
    /*! Security Parameters */
#ifdef ZB_MAC_SECURITY
    zb_uint8_t      securityLevel;
    zb_uint8_t      keyIdMode;
    zb_uint8_t      keySource[8];
    zb_uint8_t      keyIndex;
#endif
    /*! Data Buffer */
    zb_uint8_t* msdu;
    /*! Length of data buffer (adding in here instead of creating another structure for just the buffer and length) */
    zb_uint16_t msduLength;
} ZB_PACKED_STRUCT
zb_mcps_data_indication_t;

/*!
 Associate Indication Callback function pointer prototype
 */
typedef void (*zb_mlme_associate_indication_cb_t)(zb_mlme_associate_indication_t *pAssocInd);

/*!
 Assocate Confirmation Callback function pointer prototype
 */
typedef void (*zb_mlme_associate_confirm_cb_t)(zb_mlme_associate_confirm_t *pAssocCnf);

/*!
 Disassociate Indication Callback function pointer prototype
 */
typedef void (*zb_mac_disassociate_indication_cb_t)(zb_mac_disassociate_indication_t *pDisassociateInd);

/*!
 Disassociate Confirm Callback function pointer prototype
 */
typedef void (*zb_mac_disassociate_confirm_cb_t)(zb_mac_disassociate_confirm_t *pDisassociateCnf);

/*!
 Beacon Notify Indication Callback function pointer prototype
 */
typedef void (*zb_mac_beacon_notify_indication_cb_t)(zb_mac_beacon_notify_indication_t *pBeaconNotifyInd);

/*!
 Orphan Indication Callback function pointer prototype
 */
typedef void (*zb_mac_orphan_ind_cb_t)(zb_mac_orphan_ind_t *pOrphanInd);

/*!
 Scan Confirmation Callback function pointer prototype
 */
typedef void (*zb_mac_scan_confirm_cb_t)(zb_mac_scan_confirm_t *pScanCnf);

/*!
 Start Confirmation Callback function pointer prototype
 */
typedef void (*zb_mac_start_confirm_cb_t)(zb_ret_t status);

/*!
 Poll Confirm Callback function pointer prototype
 */
typedef void (*zb_mac_poll_confirm_cb_t)(zb_ret_t status);

/*!
 Comm Status Indication Callback function pointer prototype
 */
typedef void (*zb_mlme_comm_status_indication_cb_t)(zb_mlme_comm_status_indication_t *pCommStatus);

/*!
 Data Confirmation Callback function pointer prototype
 */
typedef void (*zb_mcps_data_confirm_cb_t)(zb_mcps_data_confirm_params_t *pDataCnf);

/*!
 Data Indication Callback function pointer prototype
 */
typedef void (*zb_mcps_data_indication_cb_t)(zb_mcps_data_indication_t *pDataInd);

/*!
 Purge Confirmation Callback function pointer prototype
 */
typedef void (*zb_mac_purge_confirm_cb_t)(zb_mac_purge_confirm_t *pPurgeCnf);

/*!
 Reset Confirmation Callback function pointer prototype
 */
typedef void (*zb_mlme_reset_confirm_cb_t)(zb_mlme_reset_confirm_t *pPurgeCnf);


/*!
 Structure containing all the MAC callbacks (indications).
 To receive the confirmation or indication fill in the
 associated callback with a pointer to the function that
 will handle that callback.  To ignore a callback
 set that function pointer to NULL.
 */
typedef struct _apimac_callbacks
{
    /*! Associate Indicated callback */
    zb_mlme_associate_indication_cb_t pAssocIndCb;
    /*! Associate Confirmation callback */
    zb_mlme_associate_confirm_cb_t pAssocCnfCb;
    /*! Disassociate Indication callback */
    zb_mac_disassociate_indication_cb_t pDisassociateIndCb;
    /*! Disassociate Confirmation callback */
    zb_mac_disassociate_confirm_cb_t pDisassociateCnfCb;
    /*! Beacon Notify Indication callback */
    zb_mac_beacon_notify_indication_cb_t pBeaconNotifyIndCb;
    /*! Orphan Indication callback */
    zb_mac_orphan_ind_cb_t pOrphanIndCb;
    /*! Scan Confirmation callback */
    zb_mac_scan_confirm_cb_t pScanCnfCb;
    /*! Start Confirmation callback */
    zb_mac_start_confirm_cb_t pStartCnfCb;
    /*! Poll Confirm callback */
    zb_mac_poll_confirm_cb_t pPollCnfCb;
    /*! Comm Status Indication callback */
    zb_mlme_comm_status_indication_cb_t pCommStatusCb;
    /*! Data Confirmation callback */
    zb_mcps_data_confirm_cb_t pDataCnfCb;
    /*! Data Indication callback */
    zb_mcps_data_indication_cb_t pDataIndCb;
    /*! Purge Confirm callback */
    zb_mac_purge_confirm_cb_t pPurgeCnfCb;
    /*! Reset Confirmation callback */
    zb_mlme_reset_confirm_cb_t pResetCnfCb;
} zb_mac_callbacks_t;

/******************************************************************************
 Function Prototypes
 *****************************************************************************/

/*!
 * @brief       Register for MAC callbacks.
 *
 * @param       pCallbacks - pointer to callback structure
 */
extern void register_callbacks(zb_mac_callbacks_t *pCallbacks);

/*!
 * @brief      Get the PAN ID of the device from MAC PIB.
 */
zb_uint16_t getPibPanIdSynchronous(void);


#ifdef ZB_MAC_SECURITY
/*!
 * @brief      Get the frame counter of the device from MAC PIB.
 */
zb_uint32_t getPibFrameCounterSynchronous(void);

/*!
 * @brief      Set the MAC PIB frame counter value.
 * @param      frameCounter - the frame counter value to set the PIB attribute to.
 */
void setPibFrameCounterSynchronous(zb_uint32_t frameCounter);
#endif /* ZB_MAC_SECURITY */

/**
 *  @brief MLME-DISASSOCIATE.confirm primitive
 *  @param param - reference to buffer.
 */
void zb_mlme_disassociate_confirm(zb_uint8_t param);

#endif // TRANSLATE