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

#ifndef ZB_PLATFORM_CONFIG_H
#define ZB_PLATFORM_CONFIG_H 1
#include "stdint.h"

#define GP_MAC_PKT_SIZE 256U

#define GP_MAC_SEND_REPEAT_MAX  4U

#define GP_MAC_RX_QUEUE_CAP     5U

extern uint8_t zb_mac_default_tx_power;
#define ZB_MAC_DEFAULT_TX_POWER     zb_mac_default_tx_power

/* trace */
#ifdef ZB_TRACE_LEVEL
#define ZB_TRACE_OVER_USART
#define ZB_SERIAL_FOR_TRACE
#define ZB_HAVE_SERIAL
#define ZB_HAVE_SERIAL_SINGLE
#define ZB_TRACE_TO_PORT
#define ZB_BINARY_TRACE
#define ZB_TRACE_FROM_INTR
#define ZB_TRAFFIC_DUMP_ON
#else
#ifdef USE_ASSERT
/* If trace is disabled but we use asserts we still need
   to enable ZB_BINARY_TRACE to use asserts with TRACE_FILE_ID
   instead of actual paths to files. Also, to decrease footprint
   in this case it's better to disable assert indication as well.
 */
#define ZB_BINARY_TRACE
#define ZB_DISABLE_ASSERT_INDICATION
#endif
#endif  /* ZB_TRACE_LEVEL */

#ifdef ZB_HAVE_SERIAL
#ifndef ZB_UART_BAUD_RATE
#define ZB_UART_BAUD_RATE 115200U
#endif
#endif /* ZB_HAVE_SERIAL */

#if defined(ZB_ZGPD_ROLE)
/* number of Flash pages reserved for MAC sequence storage */
#define ZB_MSS_PAGE_COUNT  40U
#endif //ZB_ZGPD_ROLE

/*
 * Do not drop a packet if it is received by the radio and pushed onto the stack.
 * This breaks, in particular, fragmented transmission
 */
#define xZB_MAC_DROP_PKT_WHEN_RX_OFF
/* May use HW AES */
#define ZB_HW_ZB_AES128
#define ZB_HW_ZB_AES128_DEC
#define ZB_HW_SHA_256
#define ZB_HW_ECDH_P256_PUBLIC_KEY
#define ZB_HW_ECDH_P256_SHARED_SECRET
#define ZB_HW_HMAC_SHA_256
#define ZB_MAC_RADIO_CANT_WAKEUP_MCU

#if defined ZB_USE_SLEEP && defined NCP_MODE
#define ZB_OSIF_NCP_TRANSPORT_PREPARE_TO_SLEEP() zb_osif_spi_prepare_to_sleep()
#endif

/* Do not reboot at oom: assert actually does not reboot there */
#define xZB_OOM_REBOOT

#ifndef ZB_IOBUF_POOL_SIZE
#ifdef ZB_MEMORY_COMPACT
#define ZB_IOBUF_POOL_SIZE  20U
#define ZB_SCHEDULER_Q_SIZE 20U
#else
#define ZB_IOBUF_POOL_SIZE  40U
#define ZB_SCHEDULER_Q_SIZE 64U
#endif
#endif

#define ZB_MANUAL_ACK
#define ZB_AUTO_ACK_TX
#define ZB_MAC_AUTO_ACK_RECV
#define ZB_MAC_SINGLE_PACKET_IN_FIFO

#define ZB_MAC_RX_QUEUE_CAP (5U)

#define ZB_MAC_SYNC_RSSI

#if !defined ZB_BOOTLOADER && defined DEBUG
#define ZB_USE_BUTTONS
#define ZB_N_LEDS 2U
#define ZB_N_BUTTONS 2U
#endif /* !ZB_BOOTLOADER && DEBUG */

#define ZB_MAC_PENDING_BIT_SOURCE_MATCHING
#define ZB_MAC_HARDWARE_PB_MATCHING
#define ZB_MAC_SWITCHABLE_PB_MATCHING
#ifndef ZB_CONFIGURABLE_MEM
#define ZB_CHILD_HASH_TABLE_SIZE (((ZB_IEEE_ADDR_TABLE_SIZE + ZB_IEEE_ADDR_TABLE_SIZE / 3U) + 31U) / 32U * 32U)
#endif //ZB_CONFIGURABLE_MEM

#define ZB_MANUAL_ADDR_FILTER
/* #define MAC_RADIO_TX_WATCHDOG_ALARM ZB_MILLISECONDS_TO_BEACON_INTERVAL(500) */

/*
 * Uncomment when implemented GP
 */
/* #define MAC_AUTO_DELAY_IN_MAC_GP_SEND */

#if defined ZB_MAC_STICKY_PENDING_BIT
#error "Sticky Pending Bit is not supported longer! Use ZB_MAC_SOFTWARE_PB_MATCHING or ZB_MAC_HARDWARE_PB_MATCHING or ZB_MAC_SWITCHABLE_PB_MATCHING"
#endif /* ZB_MAC_STICKY_PENDING_BIT */


#define ZB_LITTLE_ENDIAN
#define ZB_NEED_ALIGN
#define ZB_NO_SYSTEST_SUPPORT
#define ZB_USE_NVRAM
#define ZB_SOFT_SECURITY

#define ZB_ENABLE_HA
#define ZB_BDB_MODE
/* #define ZB_DISTRIBUTED_SECURITY_ON */

/* our MAC */
#define ZB_MAC_CONFIGURABLE_TX_POWER
#define ZB_AUTO_ACK_TX

#endif /* ZB_PLATFORM_CONFIG_H */
