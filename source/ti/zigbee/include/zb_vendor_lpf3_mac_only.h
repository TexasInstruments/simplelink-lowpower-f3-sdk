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

#define ZB_USE_BUTTONS
#define ZB_USE_SLEEP
#define ZB_MAC_ONLY_STACK
#define ZB_MAC_INTERFACE_SINGLE
#define ZB_MAC_TESTING_MODE

#define ZB_CONFIGURABLE_MAC_PIB
#define ZB_OPTIONAL_MAC_FEATURES

#define ZB_MAC_DEFAULT_TX_POWER_24_GHZ        0
#define ZB_MAC_CONFIGURABLE_TX_POWER

#endif /* ZB_VENDOR_LPF3_H */
