/******************************************************************************
*  Filename:       hw_tcm_h
******************************************************************************
*  Copyright (c) 2021 Texas Instruments Incorporated. All rights reserved.
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
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define TCM_DESC_MODID_W                                                    16U
#define TCM_DESC_MODID_M                                            0xFFFF0000U
#define TCM_DESC_MODID_S                                                    16U
#define TCM_DESC_MODID_MAXIMUM                                      0xFFFF0000U
#define TCM_DESC_MODID_MINIMUM                                      0x00000000U

// Field: [15:12] STDIPOFF
//
// Standard IP registers offset. Value 0 indicates Standard IP registers are
// not present. Any other value between 1 to 15 indicates standard IP registers
// start from address offset 64 * STDIPOFF from base address.
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define TCM_DESC_STDIPOFF_W                                                  4U
#define TCM_DESC_STDIPOFF_M                                         0x0000F000U
#define TCM_DESC_STDIPOFF_S                                                 12U
#define TCM_DESC_STDIPOFF_MAXIMUM                                   0x0000F000U
#define TCM_DESC_STDIPOFF_MINIMUM                                   0x00000000U

// Field:  [11:8] INSTIDX
//
// Instance Index within the device. This will be a parameter to the RTL for
// modules that can have multiple instances.
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define TCM_DESC_INSTIDX_W                                                   4U
#define TCM_DESC_INSTIDX_M                                          0x00000F00U
#define TCM_DESC_INSTIDX_S                                                   8U
#define TCM_DESC_INSTIDX_MAXIMUM                                    0x00000F00U
#define TCM_DESC_INSTIDX_MINIMUM                                    0x00000000U

// Field:   [7:4] MAJREV
//
// Major rev of the IP
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define TCM_DESC_MAJREV_W                                                    4U
#define TCM_DESC_MAJREV_M                                           0x000000F0U
#define TCM_DESC_MAJREV_S                                                    4U
#define TCM_DESC_MAJREV_MAXIMUM                                     0x000000F0U
#define TCM_DESC_MAJREV_MINIMUM                                     0x00000000U

// Field:   [3:0] MINREV
//
// Minor rev of the IP
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define TCM_DESC_MINREV_W                                                    4U
#define TCM_DESC_MINREV_M                                           0x0000000FU
#define TCM_DESC_MINREV_S                                                    0U
#define TCM_DESC_MINREV_MAXIMUM                                     0x0000000FU
#define TCM_DESC_MINREV_MINIMUM                                     0x00000000U

//*****************************************************************************
//
// Register: TCM_O_DESCEX
//
//*****************************************************************************
// Field:   [8:0] GSKCNT
//
// Number of gaskets
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define TCM_DESCEX_GSKCNT_W                                                  9U
#define TCM_DESCEX_GSKCNT_M                                         0x000001FFU
#define TCM_DESCEX_GSKCNT_S                                                  0U
#define TCM_DESCEX_GSKCNT_MAXIMUM                                   0x00000200U
#define TCM_DESCEX_GSKCNT_MINIMUM                                   0x00000001U

//*****************************************************************************
//
// Register: TCM_O_REGWEN
//
//*****************************************************************************
// Field:    [29] PAR
//
// Parity bit
// ENUMs:
// RESET                    Reset
// SET
#define TCM_REGWEN_PAR                                              0x20000000U
#define TCM_REGWEN_PAR_M                                            0x20000000U
#define TCM_REGWEN_PAR_S                                                    29U
#define TCM_REGWEN_PAR_RESET                                        0x20000000U
#define TCM_REGWEN_PAR_SET                                          0x00000000U

// Field:     [0] WEN
//
// Write enable
// ENUMs:
// DISABLE                  Highest possible value
// ENABLE                   Smallest value
#define TCM_REGWEN_WEN                                              0x00000001U
#define TCM_REGWEN_WEN_M                                            0x00000001U
#define TCM_REGWEN_WEN_S                                                     0U
#define TCM_REGWEN_WEN_DISABLE                                      0x00000001U
#define TCM_REGWEN_WEN_ENABLE                                       0x00000000U

//*****************************************************************************
//
// Register: TCM_O_WMCFG
//
//*****************************************************************************
// Field: [31:29] PAR
//
// Parity bits. Bit 28 stores the odd parity for bits 0 to 7. Bit 29 stores the
// odd parity of bits 8 to 15. Bit 30 stores the odd parity of bits 23 to 16.
// ENUMs:
// VAL7                     Value 7
// VAL6                     Value 6
// VAL5                     Value 5
// VAL4                     Value 4
// VAL3                     Value 3
// VAL2                     Value 2
// VAL1                     Value 1
// VAL0                     Value 0
#define TCM_WMCFG_PAR_W                                                      3U
#define TCM_WMCFG_PAR_M                                             0xE0000000U
#define TCM_WMCFG_PAR_S                                                     29U
#define TCM_WMCFG_PAR_VAL7                                          0xE0000000U
#define TCM_WMCFG_PAR_VAL6                                          0xC0000000U
#define TCM_WMCFG_PAR_VAL5                                          0xA0000000U
#define TCM_WMCFG_PAR_VAL4                                          0x80000000U
#define TCM_WMCFG_PAR_VAL3                                          0x60000000U
#define TCM_WMCFG_PAR_VAL2                                          0x40000000U
#define TCM_WMCFG_PAR_VAL1                                          0x20000000U
#define TCM_WMCFG_PAR_VAL0                                          0x00000000U

// Field: [22:12] SRAMWM
//
// SRAM Watermark
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define TCM_WMCFG_SRAMWM_W                                                  11U
#define TCM_WMCFG_SRAMWM_M                                          0x007FF000U
#define TCM_WMCFG_SRAMWM_S                                                  12U
#define TCM_WMCFG_SRAMWM_MAXIMUM                                    0x001FF000U
#define TCM_WMCFG_SRAMWM_MINIMUM                                    0x00000000U

// Field:   [4:0] VIMSWM
//
// VIMS Watermark
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define TCM_WMCFG_VIMSWM_W                                                   5U
#define TCM_WMCFG_VIMSWM_M                                          0x0000001FU
#define TCM_WMCFG_VIMSWM_S                                                   0U
#define TCM_WMCFG_VIMSWM_MAXIMUM                                    0x000001FFU
#define TCM_WMCFG_VIMSWM_MINIMUM                                    0x00000000U

//*****************************************************************************
//
// Register: TCM_O_GSKEN0
//
//*****************************************************************************
// Field: [31:29] PAR
//
// Parity bits. Bit 29 stores the odd parity for bits 0 to 7. Bit 30 stores the
// odd parity of bits 8 to 15. Bit 31 stores the odd parity of bits 23 to 16.
// ENUMs:
// VAL7                     Value 7
// VAL6                     Value 6
// VAL5                     Value 5
// VAL4                     Value 4
// VAL3                     Value 3
// VAL2                     Value 2
// VAL1                     Value 1
// VAL0                     Value 0
#define TCM_GSKEN0_PAR_W                                                     3U
#define TCM_GSKEN0_PAR_M                                            0xE0000000U
#define TCM_GSKEN0_PAR_S                                                    29U
#define TCM_GSKEN0_PAR_VAL7                                         0xE0000000U
#define TCM_GSKEN0_PAR_VAL6                                         0xC0000000U
#define TCM_GSKEN0_PAR_VAL5                                         0xA0000000U
#define TCM_GSKEN0_PAR_VAL4                                         0x80000000U
#define TCM_GSKEN0_PAR_VAL3                                         0x60000000U
#define TCM_GSKEN0_PAR_VAL2                                         0x40000000U
#define TCM_GSKEN0_PAR_VAL1                                         0x20000000U
#define TCM_GSKEN0_PAR_VAL0                                         0x00000000U

// Field:   [9:0] EN
//
// Gasket enable. The gasket bit to IP mapping is given as follows :-
// [0]     =   DMA initiator AHB gasket enable
// [1]     =   I2S initiator AHB gasket enable
// [2]     =   HSM initiator AHB gasket enable
// [3]     =   Radio target AHB gasket enable
// [4]     =   AES target AHB gasket enable
// [5]     =   I2S target AHB gasket enable
// [6]     =   PDM target AHB gasket enable
// [7]     =   AFA target AHB gasket enable
// [8]     =   DMA target AHB gasket enable
// [9]     =   CANFD target AHB gasket enable
// [10]   =   VCE target AHB gasket enable
// [11]   =   VCERAM target AHB gasket enable
// [12]   =   GPIO target APB gasket enable
// [13]   =   SYSTIMER target APB gasket enable
// [14]   =   UART0 target APB gasket enable
// [15]   =   UART1 target APB gasket enable
// [16]   =   SPI0 target APB gasket enable
// [17]   =   SPI1 target APB gasket enable
// [18]   =   I2C0 target APB gasket enable
// [19]   =   EVTSVT target APB gasket enable
// [20]   =   ADC target APB gasket enable
// [21]   =   MICADC target APB gasket enable
// [22]   =   MICPGA target APB gasket enable
// [23]   =   CLKCTRL target APB gasket enable
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define TCM_GSKEN0_EN_W                                                     10U
#define TCM_GSKEN0_EN_M                                             0x000003FFU
#define TCM_GSKEN0_EN_S                                                      0U
#define TCM_GSKEN0_EN_MAXIMUM                                       0x00FFFFFFU
#define TCM_GSKEN0_EN_MINIMUM                                       0x00000000U

//*****************************************************************************
//
// Register: TCM_O_GSKEN1
//
//*****************************************************************************
// Field: [31:29] PAR
//
// Parity bits. Bit 29 stores the odd parity for bits 0 to 7. Bit 30 stores the
// odd parity of bits 8 to 15. Bit 31 stores the odd parity of bits 23 to 16.
// ENUMs:
// VAL7                     Value 7
// VAL6                     Value 6
// VAL5                     Value 5
// VAL4                     Value 4
// VAL3                     Value 3
// VAL2                     Value 2
// VAL1                     Value 1
// VAL0                     Value 0
#define TCM_GSKEN1_PAR_W                                                     3U
#define TCM_GSKEN1_PAR_M                                            0xE0000000U
#define TCM_GSKEN1_PAR_S                                                    29U
#define TCM_GSKEN1_PAR_VAL7                                         0xE0000000U
#define TCM_GSKEN1_PAR_VAL6                                         0xC0000000U
#define TCM_GSKEN1_PAR_VAL5                                         0xA0000000U
#define TCM_GSKEN1_PAR_VAL4                                         0x80000000U
#define TCM_GSKEN1_PAR_VAL3                                         0x60000000U
#define TCM_GSKEN1_PAR_VAL2                                         0x40000000U
#define TCM_GSKEN1_PAR_VAL1                                         0x20000000U
#define TCM_GSKEN1_PAR_VAL0                                         0x00000000U

// Field:  [23:0] EN
//
// Gasket enable. The gasket bit to IP mapping is given as follows :-
// [0]     =   LGPT target APB gasket enable
// [1]     =   NOWRAPPER target APB gasket enable
// [2]     =   VIMS target APB gasket enable
// [3]     =   HSM target APB gasket enable
// [4]     =   PMC target APB gasket enable
// [5]     =   CKMDIG target APB gasket enable
// [6]     =   RTC target APB gasket enable
// [7]     =   IOC target APB gasket enable
// [8]     =   SYS0 target APB gasket enable
// [9]     =   EVTULL target APB gasket enable
// [10]   =   PMUDIG target APB gasket enable
// [11]   =   DEBUGSS target APB gasket enable
// [12]   =   HSM mailbox target APB gasket enable
// [13]   =   HSM mailbox target APB gasket enable
// [14]   =   HSM mailbox target APB gasket enable
// [15]   =   HSM mailbox target APB gasket enable
// [16]   =   HSM config target APB gasket enable
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define TCM_GSKEN1_EN_W                                                     24U
#define TCM_GSKEN1_EN_M                                             0x00FFFFFFU
#define TCM_GSKEN1_EN_S                                                      0U
#define TCM_GSKEN1_EN_MAXIMUM                                       0x00FFFFFFU
#define TCM_GSKEN1_EN_MINIMUM                                       0x00000000U


#endif // __TCM__
