/*
 * Copyright (c) 2018-2026, Texas Instruments Incorporated
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
 /*
 *  ======== rfe_ble_cs_ram_regs.h ========
*/

#ifndef __RFE_BLE_CS_RAM_REGS_H
#define __RFE_BLE_CS_RAM_REGS_H

//******************************************************************************
// REGISTER OFFSETS
//******************************************************************************
// Derived Constant Used to Calculate Loop Filter Ki for CS packet TX
#define RFE_BLE_CS_RAM_O_K2CSPKTTX                                   0x0000084CU

// Derived Constant Used to Calculate Loop Filter Ki for CS tone TX
#define RFE_BLE_CS_RAM_O_K2CSTNTX                                    0x0000084EU

// Derived Constant Used to Calculate Loop Filter Kp for CS packet TX
#define RFE_BLE_CS_RAM_O_K3CSPKTTX                                   0x00000850U

// Derived Constant Used to Calculate Loop Filter Kp for CS tone TX
#define RFE_BLE_CS_RAM_O_K3CSTNTX                                    0x00000852U

// Loop Filter Ki for CS packet TX
#define RFE_BLE_CS_RAM_O_LFKICSPKTTX                                 0x00000854U

// Loop Filter Kp for CS packet TX
#define RFE_BLE_CS_RAM_O_LFKPCSPKTTX                                 0x00000856U

// Loop Filter Ki for CS tone TX
#define RFE_BLE_CS_RAM_O_LFKICSTNTX                                  0x00000858U

// Loop Filter Kp for CS tone TX
#define RFE_BLE_CS_RAM_O_LFKPCSTNTX                                  0x0000085AU

//******************************************************************************
// Register: K2CSPKTTX
//******************************************************************************
// Field: [15:15] hpm
//
// High Precision Mode
#define RFE_BLE_CS_RAM_K2CSPKTTX_HPM                                     0x8000U
#define RFE_BLE_CS_RAM_K2CSPKTTX_HPM_M                                   0x8000U
#define RFE_BLE_CS_RAM_K2CSPKTTX_HPM_S                                       15U

// Field: [14:0] val
//
// Value
#define RFE_BLE_CS_RAM_K2CSPKTTX_VAL_W                                       15U
#define RFE_BLE_CS_RAM_K2CSPKTTX_VAL_M                                   0x7FFFU
#define RFE_BLE_CS_RAM_K2CSPKTTX_VAL_S                                        0U

//******************************************************************************
// Register: K2CSTNTX
//******************************************************************************
// Field: [15:15] hpm
//
// High Precision Mode
#define RFE_BLE_CS_RAM_K2CSTNTX_HPM                                      0x8000U
#define RFE_BLE_CS_RAM_K2CSTNTX_HPM_M                                    0x8000U
#define RFE_BLE_CS_RAM_K2CSTNTX_HPM_S                                        15U

// Field: [14:0] val
//
// Value
#define RFE_BLE_CS_RAM_K2CSTNTX_VAL_W                                        15U
#define RFE_BLE_CS_RAM_K2CSTNTX_VAL_M                                    0x7FFFU
#define RFE_BLE_CS_RAM_K2CSTNTX_VAL_S                                         0U

//******************************************************************************
// Register: K3CSPKTTX
//******************************************************************************
// Field: [15:0] val
//
// Value
#define RFE_BLE_CS_RAM_K3CSPKTTX_VAL_W                                       16U
#define RFE_BLE_CS_RAM_K3CSPKTTX_VAL_M                                   0xFFFFU
#define RFE_BLE_CS_RAM_K3CSPKTTX_VAL_S                                        0U

//******************************************************************************
// Register: K3CSTNTX
//******************************************************************************
// Field: [15:0] val
//
// Value
#define RFE_BLE_CS_RAM_K3CSTNTX_VAL_W                                        16U
#define RFE_BLE_CS_RAM_K3CSTNTX_VAL_M                                    0xFFFFU
#define RFE_BLE_CS_RAM_K3CSTNTX_VAL_S                                         0U

//******************************************************************************
// Register: LFKICSPKTTX
//******************************************************************************
// Field: [12:12] hpm
//
// High Precision Mode
#define RFE_BLE_CS_RAM_LFKICSPKTTX_HPM                                   0x1000U
#define RFE_BLE_CS_RAM_LFKICSPKTTX_HPM_M                                 0x1000U
#define RFE_BLE_CS_RAM_LFKICSPKTTX_HPM_S                                     12U

// Field: [11:0] ki
//
// KI written by RFE
#define RFE_BLE_CS_RAM_LFKICSPKTTX_KI_W                                      12U
#define RFE_BLE_CS_RAM_LFKICSPKTTX_KI_M                                  0x0FFFU
#define RFE_BLE_CS_RAM_LFKICSPKTTX_KI_S                                       0U

//******************************************************************************
// Register: LFKPCSPKTTX
//******************************************************************************
// Field: [14:0] kp
//
// KP written by RFE
#define RFE_BLE_CS_RAM_LFKPCSPKTTX_KP_W                                      15U
#define RFE_BLE_CS_RAM_LFKPCSPKTTX_KP_M                                  0x7FFFU
#define RFE_BLE_CS_RAM_LFKPCSPKTTX_KP_S                                       0U

//******************************************************************************
// Register: LFKICSTNTX
//******************************************************************************
// Field: [12:12] hpm
//
// High Precision Mode
#define RFE_BLE_CS_RAM_LFKICSTNTX_HPM                                    0x1000U
#define RFE_BLE_CS_RAM_LFKICSTNTX_HPM_M                                  0x1000U
#define RFE_BLE_CS_RAM_LFKICSTNTX_HPM_S                                      12U

// Field: [11:0] ki
//
// KI written by RFE
#define RFE_BLE_CS_RAM_LFKICSTNTX_KI_W                                       12U
#define RFE_BLE_CS_RAM_LFKICSTNTX_KI_M                                   0x0FFFU
#define RFE_BLE_CS_RAM_LFKICSTNTX_KI_S                                        0U

//******************************************************************************
// Register: LFKPCSTNTX
//******************************************************************************
// Field: [14:0] kp
//
// KP written by RFE
#define RFE_BLE_CS_RAM_LFKPCSTNTX_KP_W                                       15U
#define RFE_BLE_CS_RAM_LFKPCSTNTX_KP_M                                   0x7FFFU
#define RFE_BLE_CS_RAM_LFKPCSTNTX_KP_S                                        0U


#endif // __RFE_BLE_CS_RAM_REGS_H
