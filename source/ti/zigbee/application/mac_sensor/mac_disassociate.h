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

#ifndef MAC_DISASSOCIATE_H
#define MAC_DISASSOCIATE_H 1

#include "zb_mac.h"

/*! Disassociate Reasons (in zb_mac.h, it says that the reasons are provided in the mac_status_e enum, but this enum doesn't exist, so I'm adding one here) */
typedef enum
{
    /*! The coordinator wishes the device to disassociate */
    DISASSOCIATE_COORD = 1,
    /*! The device itself wishes to disassociate */
    DISASSOCIATE_DEVICE = 2
} zb_disassociate_reason_t;

/* MLME disassociate request type (inspired by 15.4 stack version because ZBOSS never made one of their own) */
typedef ZB_PACKED_PRE struct zb_mlme_disassociate_request_s
{
    // /*! The security parameters for this message */
    // ApiMac_sec_t sec;
    /*! The addressing mode of the device to disassociate from */
    zb_uint8_t addrMode;
    /*! The address of the device with which to disassociate */
    zb_addr_u deviceAddress;
    /*! The PAN ID of the device */
    zb_uint16_t devicePanId;
    /*! The disassociate reason */
    zb_disassociate_reason_t disassociateReason;
    /*! Transmit Indirect */
    zb_bool_t txIndirect;
} ZB_PACKED_STRUCT
zb_mlme_disassociate_request_t;

/**
 *  @brief Handle MLME-DISASSOCIATION.request.
 *  @param param - reference to buffer, contain zb_mlme_disassociate_request_t parameters.
 */
void zb_mlme_disassociate_request(zb_uint8_t param);

#endif /* MAC_DISASSOCIATE */