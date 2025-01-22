/******************************************************************************
*  Filename:       hw_sramctrl_h
******************************************************************************
*  Copyright (c) 2024 Texas Instruments Incorporated. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*
*  1) Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*
*  2) Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
*
*  3) Neither the name of the copyright holder nor the names of its contributors
*     may be used to endorse or promote products derived from this software
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
*  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
*  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
*  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
*  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
*  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#ifndef __HW_SRAMCTRL_H__
#define __HW_SRAMCTRL_H__

//*****************************************************************************
//
// This section defines the register offsets of
// SRAMCTRL component
//
//*****************************************************************************
#define SRAMCTRL_O_DESC                                             0x00000000U

#define SRAMCTRL_O_DTB                                              0x00000064U

#define SRAMCTRL_O_CFG                                              0x00000100U

#define SRAMCTRL_O_PARDBG                                           0x00000110U

#define SRAMCTRL_O_PARERR                                           0x00000114U

//*****************************************************************************
//
// Register: SRAMCTRL_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module identifier
#define SRAMCTRL_DESC_MODID_W                                               16U
#define SRAMCTRL_DESC_MODID_M                                       0xFFFF0000U
#define SRAMCTRL_DESC_MODID_S                                               16U

// Field: [15:12] STDIPOFF
//
// 64 B Standard IP MMR block<br>0: STDIP MMRs do not exist<br>1:15: These MMRs
// begin at offset 64*STDIPOFF from IP base address
#define SRAMCTRL_DESC_STDIPOFF_W                                             4U
#define SRAMCTRL_DESC_STDIPOFF_M                                    0x0000F000U
#define SRAMCTRL_DESC_STDIPOFF_S                                            12U

// Field:  [11:8] INSTIDX
//
// IP Instance number
#define SRAMCTRL_DESC_INSTIDX_W                                              4U
#define SRAMCTRL_DESC_INSTIDX_M                                     0x00000F00U
#define SRAMCTRL_DESC_INSTIDX_S                                              8U

// Field:   [7:4] MAJREV
//
// Major revision
#define SRAMCTRL_DESC_MAJREV_W                                               4U
#define SRAMCTRL_DESC_MAJREV_M                                      0x000000F0U
#define SRAMCTRL_DESC_MAJREV_S                                               4U

// Field:   [3:0] MINREV
//
// Minor revision
#define SRAMCTRL_DESC_MINREV_W                                               4U
#define SRAMCTRL_DESC_MINREV_M                                      0x0000000FU
#define SRAMCTRL_DESC_MINREV_S                                               0U

//*****************************************************************************
//
// Register: SRAMCTRL_O_DTB
//
//*****************************************************************************
// Field:   [1:0] SEL
//
// DTB Selection
#define SRAMCTRL_DTB_SEL_W                                                   2U
#define SRAMCTRL_DTB_SEL_M                                          0x00000003U
#define SRAMCTRL_DTB_SEL_S                                                   0U

//*****************************************************************************
//
// Register: SRAMCTRL_O_CFG
//
//*****************************************************************************
// Field:     [8] PARDBGEN
//
// SRAM Parity Debug Enable.
// ENUMs:
// EN                       Enable Parity Debug. An address offset can be
//                          written to PARDBG.ADDR and parity errors will
//                          be generated on reads from within this offset
// DIS                      Disable Parity Debug. Normal operation
#define SRAMCTRL_CFG_PARDBGEN                                       0x00000100U
#define SRAMCTRL_CFG_PARDBGEN_M                                     0x00000100U
#define SRAMCTRL_CFG_PARDBGEN_S                                              8U
#define SRAMCTRL_CFG_PARDBGEN_EN                                    0x00000100U
#define SRAMCTRL_CFG_PARDBGEN_DIS                                   0x00000000U

// Field:     [0] PAREN
//
// SRAM Parity Enable.
// ENUMs:
// EN                       Enable Parity
// DIS                      Disable Parity
#define SRAMCTRL_CFG_PAREN                                          0x00000001U
#define SRAMCTRL_CFG_PAREN_M                                        0x00000001U
#define SRAMCTRL_CFG_PAREN_S                                                 0U
#define SRAMCTRL_CFG_PAREN_EN                                       0x00000001U
#define SRAMCTRL_CFG_PAREN_DIS                                      0x00000000U

//*****************************************************************************
//
// Register: SRAMCTRL_O_PARDBG
//
//*****************************************************************************
// Field:  [19:2] ADDR
//
// Debug Parity Error Address Offset.##br##When CFG.PARDBGEN is 1, this field
// is used to set a parity debug address offset. Writes within this address
// offset will force incorrect parity bits to be stored together with the data
// written. The following reads within this same address offset will thus
// result in parity errors to be generated.
#define SRAMCTRL_PARDBG_ADDR_W                                              18U
#define SRAMCTRL_PARDBG_ADDR_M                                      0x000FFFFCU
#define SRAMCTRL_PARDBG_ADDR_S                                               2U

//*****************************************************************************
//
// Register: SRAMCTRL_O_PARERR
//
//*****************************************************************************
// Field:  [31:0] ADDR
//
// Parity Error Address Offset.##br##This holds the address offset that first
// generated the parity error and an interrupt is generated. ##br##This
// register is 'Clear-On-Read'
#define SRAMCTRL_PARERR_ADDR_W                                              32U
#define SRAMCTRL_PARERR_ADDR_M                                      0xFFFFFFFFU
#define SRAMCTRL_PARERR_ADDR_S                                               0U


#endif // __SRAMCTRL__
