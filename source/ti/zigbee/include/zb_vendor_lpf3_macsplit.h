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
/*  PURPOSE: Vendor configuration for Nordic nRF52840 MAC split SoC for Nordic devboard

 NOTE: This file configures behavior and some size constants when
 compiling ZBOSS libraries. If some definition changed here, ZBOSS
 libraries must be recompiled. Changing this file contents without
 recompiling ZBOSS libraries causes memory structures inconsistence.

 DO NOT CHANGE THAT FILE INCLUDED INTO BINARY SDK BUILD!
*/
#ifndef ZB_VENDOR_LPF3_H
#define ZB_VENDOR_LPF3_H 1

#define ZB_MACSPLIT_DEVICE
#define ZB_MACSPLIT_TRANSPORT_SERIAL
#define ZB_MACSPLIT_NO_HARD_RESET
/* Indeed ZB_MACSPLIT_USE_IO_BUFFERS used nearly always */
#define ZB_MACSPLIT_USE_IO_BUFFERS
#define ZB_HAVE_SERIAL
#define ZB_HAVE_SERIAL_SINGLE

#define MACSPLIT_APP_REVISION 1

#define APP_ONLY_NVRAM

#define ZB_SEND_BEACON_IMMEDIATELY
#define ZB_ENABLE_ZGP_DIRECT

#define ZB_CB_QUANT 8
#define ZB_IOBUF_POOL_SIZE 60
#define ZB_SCHEDULER_Q_SIZE 225

/* Macsplit SoC does not use prod cfg indeed, but that define is necessary for constants definition */
#define ZB_PRODUCTION_CONFIG
#define ZB_MAC_CONFIGURABLE_TX_POWER

/* ZBOSS trace over Segger RTT */
/* #define ZB_TRACE_OVER_JTAG */
//#define ZB_TRACE_OVER_MACSPLIT

/* Reduce MCPS Poll Indication calls from MAC layer */
/* Can't implement it with new Nordic driver: ther do not expose a function to seek in their children table.
   TODO: rethink and implement!!
 */
#define xZB_MAC_POLL_INDICATION_CALLS_REDUCED
#ifdef ZB_MAC_POLL_INDICATION_CALLS_REDUCED
/*
  It's relates with ZB_CHILD_HASH_TABLE_SIZE for ZB_MAC_SOFTWARE_PB_MATCHING
  Do not forget to align this value with host side
 */
#define ZB_NEIGHBOR_TABLE_SIZE 100
#endif  /* ZB_MAC_POLL_INDICATION_CALLS_REDUCED */

/* Support external PA/LNA */


#endif /* ZB_VENDOR_LPF3_H */
