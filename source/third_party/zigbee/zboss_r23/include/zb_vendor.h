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

#ifndef ZB_VENDOR_H
#define ZB_VENDOR_H 1

#define DISABLE_AUTO_REBOOT_FOR_NSNG_CI
#define ZB_BDB_MODE
#define ZB_ZCL_ENABLE_DEFAULT_BASIC_PROCESSING
#define ZB_RANDOM_HARDWARE

#define ZB_MAC_INTERFACE_SINGLE
#define ZB_MAC_MONOLITHIC

#define ZB_DISTRIBUTED_SECURITY_ON

#define ZB_USE_SLEEP
#define ZB_USE_BUTTONS
#define ZB_USEALIAS

#define ZB_SECURITY_INSTALLCODES
#define APS_FRAGMENTATION
#define ZB_ALL_DEVICE_SUPPORT

#define ZB_CONFIGURABLE_RETRIES

#ifdef ZB_CONFIG_DEFAULT_KERNEL_DEFINITION

/* Default memory storage configuration - to be used if user does not include any of zb_mem_config_xxx.h */

#ifndef ZB_ED_ROLE
#define ZB_CONFIG_ROLE_ZC
#else
#define ZB_CONFIG_ROLE_ZED
#endif

#define ZB_CONFIG_OVERALL_NETWORK_SIZE 128

#define ZB_CONFIG_HIGH_TRAFFIC

#define ZB_CONFIG_APPLICATION_COMPLEX

#endif  /*  ZB_CONFIG_DEFAULT_KERNEL_DEFINITION */

/* ----------------- WWAH ---------------------- */
#define ZB_ZCL_SUPPORT_CLUSTER_WWAH
#define ZB_ZCL_ENABLE_WWAH_CLIENT
#define ZB_ZCL_ENABLE_WWAH_SERVER
#define ZB_APS_ENCRYPTION_PER_CLUSTER
#if (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER)
#define ZB_ENABLE_NWK_RETRANSMIT
#define ZB_BEACON_SURVEY
#define ZB_PARENT_CLASSIFICATION
#endif
/* ----------------- WWAH ---------------------- */

#define ZB_MAC_CONFIGURABLE_TX_POWER
#define ZB_BDB_ENABLE_FINDING_BINDING

#ifndef ZB_UART_BAUD_RATE
#define ZB_UART_BAUD_RATE 115200
#endif

#define ZB_MAC_DEFAULT_TX_POWER_24_GHZ        +5

#define ZB_LITE_NO_ZB_DIRECT
#define ZB_DISABLE_ZBD_SUPPORT_ON_TC

#define TC_SWAPOUT

#define ZB_ENABLE_ZGP
#define ZB_APS_USER_PAYLOAD

#endif /* ZB_VENDOR_H */
