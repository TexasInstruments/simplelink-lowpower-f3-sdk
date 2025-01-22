/******************************************************************************
*  Filename:       hw_tcm_h
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

#ifndef __HW_TCM_H__
#define __HW_TCM_H__

//*****************************************************************************
//
// This section defines the register offsets of
// TCM component
//
//*****************************************************************************
// Module Description
#define TCM_O_DESC                                                  0x00000000U

// Extended Module Description
#define TCM_O_DESCEX                                                0x00000004U

// Register Write Enable
#define TCM_O_REGWEN                                                0x00000008U

// VIMS and SRAM watermark configurations
#define TCM_O_WMCFG                                                 0x0000000CU

// Gasket configuration
#define TCM_O_GSKEN0                                                0x00000010U

// Gasket configuration
#define TCM_O_GSKEN1                                                0x00000014U

//*****************************************************************************
//
// Register: TCM_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module identification contains a unique peripheral identification number.
#define TCM_DESC_MODID_W                                                    16U
#define TCM_DESC_MODID_M                                            0xFFFF0000U
#define TCM_DESC_MODID_S                                                    16U

// Field: [15:12] STDIPOFF
//
// Standard IP registers offset. Value 0 indicates Standard IP registers are
// not present. Any other value between 1 to 15 indicates standard IP registers
// start from address offset 64 * STDIPOFF from base address.
#define TCM_DESC_STDIPOFF_W                                                  4U
#define TCM_DESC_STDIPOFF_M                                         0x0000F000U
#define TCM_DESC_STDIPOFF_S                                                 12U

// Field:  [11:8] INSTIDX
//
// Instance Index within the device. This will be a parameter to the RTL for
// modules that can have multiple instances.
#define TCM_DESC_INSTIDX_W                                                   4U
#define TCM_DESC_INSTIDX_M                                          0x00000F00U
#define TCM_DESC_INSTIDX_S                                                   8U

// Field:   [7:4] MAJREV
//
// Major rev of the IP
#define TCM_DESC_MAJREV_W                                                    4U
#define TCM_DESC_MAJREV_M                                           0x000000F0U
#define TCM_DESC_MAJREV_S                                                    4U

// Field:   [3:0] MINREV
//
// Minor rev of the IP
#define TCM_DESC_MINREV_W                                                    4U
#define TCM_DESC_MINREV_M                                           0x0000000FU
#define TCM_DESC_MINREV_S                                                    0U

//*****************************************************************************
//
// Register: TCM_O_DESCEX
//
//*****************************************************************************
// Field:   [8:0] GSKCNT
//
// Number of gaskets
#define TCM_DESCEX_GSKCNT_W                                                  9U
#define TCM_DESCEX_GSKCNT_M                                         0x000001FFU
#define TCM_DESCEX_GSKCNT_S                                                  0U

//*****************************************************************************
//
// Register: TCM_O_REGWEN
//
//*****************************************************************************
// Field: [30:29] PAR
//
// Parity bit. Bit 29 stores the odd parity for bits 0 to 7. Bit 30 stores the
// odd parity of bits 8 to 15.
// PAR[29] = ~WEN
// PAR[30] = ~SYNC
#define TCM_REGWEN_PAR_W                                                     2U
#define TCM_REGWEN_PAR_M                                            0x60000000U
#define TCM_REGWEN_PAR_S                                                    29U

// Field:     [8] SYNC
//
// Program this bit to 0 before writing to WMCFG or any of the gasket enable
// MMR i.e. GSKEN0, GSKEN1 etc.
// Once the MMR writes are done, program this bit to 1 again.
// Then WEN can be programmed 1 to lock the configuration.
//
// Note:
// Do not toggle SYNC and WEN in the same cycle
// ENUMs:
// RST                      Reset
// SET
#define TCM_REGWEN_SYNC                                             0x00000100U
#define TCM_REGWEN_SYNC_M                                           0x00000100U
#define TCM_REGWEN_SYNC_S                                                    8U
#define TCM_REGWEN_SYNC_RST                                         0x00000100U
#define TCM_REGWEN_SYNC_SET                                         0x00000000U

// Field:     [0] WEN
//
// Write enable
// ENUMs:
// DIS                      Disable writes
// EN                       Enable writes
#define TCM_REGWEN_WEN                                              0x00000001U
#define TCM_REGWEN_WEN_M                                            0x00000001U
#define TCM_REGWEN_WEN_S                                                     0U
#define TCM_REGWEN_WEN_DIS                                          0x00000001U
#define TCM_REGWEN_WEN_EN                                           0x00000000U

//*****************************************************************************
//
// Register: TCM_O_WMCFG
//
//*****************************************************************************
// Field: [31:29] PAR
//
// Parity bits. Bit 29 stores the odd parity for bits 0 to 7. Bit 30 stores the
// odd parity of bits 8 to 15. Bit 31 stores the odd parity of bits 23 to 16.
// PAR[29] = ~XOR(VIMSWM[7:0])
// PAR[30] = ~XOR({[VIMSWM[9:8]], SRAMWM[15:12]})
// PAR[31] = ~XOR(SRAMWM[21:16])
#define TCM_WMCFG_PAR_W                                                      3U
#define TCM_WMCFG_PAR_M                                             0xE0000000U
#define TCM_WMCFG_PAR_S                                                     29U

// Field: [19:12] SRAMWM
//
// SRAM Watermark.
// The address from (SRAM Base Address) to (SRAM Base Address + (SRAMWM +
// 1)<<10 - 1) is considered secure.
// The address from (SRAM Base Address + (SRAMWM + 1) <<10) to (SRAM Last
// Address) is considered Non-secure.
#define TCM_WMCFG_SRAMWM_W                                                   8U
#define TCM_WMCFG_SRAMWM_M                                          0x000FF000U
#define TCM_WMCFG_SRAMWM_S                                                  12U

// Field:   [6:0] VIMSWM
//
// 	VIMS Watermark
// The address from (Flash Main Base Address) to (Flash Main Base Address +
// (VIMSWM + 1)<<13 - 1) is considered secure.
// The address from (Flash Main Base Address + (VIMSWM + 1)<<13) to (Flash Main
// Last Address) is considered Non-secure.
#define TCM_WMCFG_VIMSWM_W                                                   7U
#define TCM_WMCFG_VIMSWM_M                                          0x0000007FU
#define TCM_WMCFG_VIMSWM_S                                                   0U

//*****************************************************************************
//
// Register: TCM_O_GSKEN0
//
//*****************************************************************************
// Field: [31:29] PAR
//
// Parity bits. Bit 29 stores the odd parity for bits 0 to 7. Bit 30 stores the
// odd parity of bits 8 to 15. Bit 31 stores the odd parity of bits 23 to 16.
// PAR[29] = ~XOR(GSKEN0[7:0])
// PAR[30] = ~XOR(GSKEN0[15:8])
// PAR[31] = ~XOR(GSKEN0[23:16])
#define TCM_GSKEN0_PAR_W                                                     3U
#define TCM_GSKEN0_PAR_M                                            0xE0000000U
#define TCM_GSKEN0_PAR_S                                                    29U

// Field:  [23:0] EN
//
// Gasket enable. The gasket bit to IP mapping is given as follows :-
// [0]     =   DMA initiator gasket enable
// [1]     =   I2S initiator gasket enable
// [2]     =   HSM initiator gasket enable
// [3]     =   Radio target gasket enable
// [4]     =   AES target gasket enable
// [5]     =   I2S target gasket enable
// [6]     =   PDM target gasket enable
// [7]     =   AFA target gasket enable
// [8]     =   DMA target gasket enable
// [9]     =   CANFD target gasket enable
// [10]   =   VCE target gasket enable
// [11]   =   VCERAM target gasket enable
// [12]   =   GPIO target gasket enable
// [13]   =   SYSTIMER target gasket enable
// [14]   =   UART0 target gasket enable
// [15]   =   UART1 target gasket enable
// [16]   =   SPI0 target gasket enable
// [17]   =   SPI1 target gasket enable
// [18]   =   I2C0 target gasket enable
// [19]   =   EVTSVT target gasket enable
// [20]   =   ADC target gasket enable
// [21]   =   MICADC target gasket enable
// [22]   =   MICPGA target gasket enable
// [23]   =   CLKCTRL target gasket enable
#define TCM_GSKEN0_EN_W                                                     24U
#define TCM_GSKEN0_EN_M                                             0x00FFFFFFU
#define TCM_GSKEN0_EN_S                                                      0U

//*****************************************************************************
//
// Register: TCM_O_GSKEN1
//
//*****************************************************************************
// Field: [31:29] PAR
//
// Parity bits. Bit 29 stores the odd parity for bits 0 to 7. Bit 30 stores the
// odd parity of bits 8 to 15. Bit 31 stores the odd parity of bits 23 to 16.
// PAR[29] = ~XOR(GSKEN1[7:0])
// PAR[30] = ~XOR(GSKEN1[15:8])
// PAR[31] = ~XOR(GSKEN1[23:16])
#define TCM_GSKEN1_PAR_W                                                     3U
#define TCM_GSKEN1_PAR_M                                            0xE0000000U
#define TCM_GSKEN1_PAR_S                                                    29U

// Field:  [16:0] EN
//
// Gasket enable. The gasket bit to IP mapping is given as follows :-
// [0]     =   LGPT target gasket enable
// [1]     =   NOWRAPPER target gasket enable
// [2]     =   VIMS target gasket enable
// [3]     =   HSM target gasket enable
// [4]     =   PMC target gasket enable
// [5]     =   CKMDIG target gasket enable
// [6]     =   RTC target gasket enable
// [7]     =   IOC target gasket enable
// [8]     =   SYS0 target gasket enable
// [9]     =   EVTULL target gasket enable
// [10]   =   PMUDIG target gasket enable
// [11]   =   DEBUGSS target gasket enable
// [12]   =   HSM mailbox 1 target gasket enable
// [13]   =   HSM mailbox 2 target gasket enable
// [14]   =   HSM mailbox 3 target gasket enable
// [15]   =   HSM mailbox 4 target gasket enable
// [16]   =   HSM config target gasket enable
#define TCM_GSKEN1_EN_W                                                     17U
#define TCM_GSKEN1_EN_M                                             0x0001FFFFU
#define TCM_GSKEN1_EN_S                                                      0U


#endif // __TCM__
