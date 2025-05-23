/*
 * Copyright (c) 2018-2025, Texas Instruments Incorporated
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
 *  ======== rfe_cpll_ram_regs.h ========
*/

#ifndef __RFE_CPLL_RAM_REGS_H
#define __RFE_CPLL_RAM_REGS_H

//******************************************************************************
// REGISTER OFFSETS
//******************************************************************************
// Settling time per channel
#define RFE_CPLL_RAM_O_FREQ_SETTLE                                   0x00000004U

// Tone length per channel
#define RFE_CPLL_RAM_O_TONE_LENGTH                                   0x00000008U

// RX duration per channel
#define RFE_CPLL_RAM_O_RX_LENGTH                                     0x0000000CU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP0                                      0x00000E00U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP1                                      0x00000E02U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP2                                      0x00000E04U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP3                                      0x00000E06U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP4                                      0x00000E08U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP5                                      0x00000E0AU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP6                                      0x00000E0CU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP7                                      0x00000E0EU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP8                                      0x00000E10U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP9                                      0x00000E12U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP10                                     0x00000E14U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP11                                     0x00000E16U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP12                                     0x00000E18U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP13                                     0x00000E1AU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP14                                     0x00000E1CU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP15                                     0x00000E1EU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP16                                     0x00000E20U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP17                                     0x00000E22U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP18                                     0x00000E24U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP19                                     0x00000E26U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP20                                     0x00000E28U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP21                                     0x00000E2AU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP22                                     0x00000E2CU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP23                                     0x00000E2EU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP25                                     0x00000E30U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP26                                     0x00000E32U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP27                                     0x00000E34U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP28                                     0x00000E36U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP29                                     0x00000E38U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP30                                     0x00000E3AU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP31                                     0x00000E3CU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP32                                     0x00000E3EU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP33                                     0x00000E40U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP34                                     0x00000E42U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP35                                     0x00000E44U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP36                                     0x00000E46U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP37                                     0x00000E48U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP38                                     0x00000E4AU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP39                                     0x00000E4CU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP40                                     0x00000E4EU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP41                                     0x00000E50U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP42                                     0x00000E52U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP43                                     0x00000E54U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP44                                     0x00000E56U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP45                                     0x00000E58U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP46                                     0x00000E5AU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP47                                     0x00000E5CU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP48                                     0x00000E5EU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP49                                     0x00000E60U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP50                                     0x00000E62U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP51                                     0x00000E64U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP52                                     0x00000E66U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP53                                     0x00000E68U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP54                                     0x00000E6AU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP55                                     0x00000E6CU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP56                                     0x00000E6EU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP57                                     0x00000E70U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP58                                     0x00000E72U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP59                                     0x00000E74U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP60                                     0x00000E76U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP61                                     0x00000E78U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP62                                     0x00000E7AU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP63                                     0x00000E7CU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP65                                     0x00000E7EU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP66                                     0x00000E80U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP67                                     0x00000E82U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP68                                     0x00000E84U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP69                                     0x00000E86U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP70                                     0x00000E88U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP71                                     0x00000E8AU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP72                                     0x00000E8CU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP73                                     0x00000E8EU

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP74                                     0x00000E90U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP75                                     0x00000E92U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP76                                     0x00000E94U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP77                                     0x00000E96U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP78                                     0x00000E98U

// Channel in channelmap
#define RFE_CPLL_RAM_O_CHANMAP79                                     0x00000E9AU

//******************************************************************************
// Register: FREQ_SETTLE
//******************************************************************************
// Field: [15:0] val
//
// Unit is 16 MHz ticks
#define RFE_CPLL_RAM_FREQ_SETTLE_VAL_W                                       16U
#define RFE_CPLL_RAM_FREQ_SETTLE_VAL_M                                   0xFFFFU
#define RFE_CPLL_RAM_FREQ_SETTLE_VAL_S                                        0U

//******************************************************************************
// Register: TONE_LENGTH
//******************************************************************************
// Field: [15:0] val
//
// Unit is 16 MHz ticks
#define RFE_CPLL_RAM_TONE_LENGTH_VAL_W                                       16U
#define RFE_CPLL_RAM_TONE_LENGTH_VAL_M                                   0xFFFFU
#define RFE_CPLL_RAM_TONE_LENGTH_VAL_S                                        0U

//******************************************************************************
// Register: RX_LENGTH
//******************************************************************************
// Field: [15:0] val
//
// Unit is 16 MHz ticks
#define RFE_CPLL_RAM_RX_LENGTH_VAL_W                                         16U
#define RFE_CPLL_RAM_RX_LENGTH_VAL_M                                     0xFFFFU
#define RFE_CPLL_RAM_RX_LENGTH_VAL_S                                          0U

//******************************************************************************
// Register: CHANMAP0
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP0_FREQ_W                                         12U
#define RFE_CPLL_RAM_CHANMAP0_FREQ_M                                     0x0FFFU
#define RFE_CPLL_RAM_CHANMAP0_FREQ_S                                          0U

//******************************************************************************
// Register: CHANMAP1
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP1_FREQ_W                                         12U
#define RFE_CPLL_RAM_CHANMAP1_FREQ_M                                     0x0FFFU
#define RFE_CPLL_RAM_CHANMAP1_FREQ_S                                          0U

//******************************************************************************
// Register: CHANMAP2
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP2_FREQ_W                                         12U
#define RFE_CPLL_RAM_CHANMAP2_FREQ_M                                     0x0FFFU
#define RFE_CPLL_RAM_CHANMAP2_FREQ_S                                          0U

//******************************************************************************
// Register: CHANMAP3
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP3_FREQ_W                                         12U
#define RFE_CPLL_RAM_CHANMAP3_FREQ_M                                     0x0FFFU
#define RFE_CPLL_RAM_CHANMAP3_FREQ_S                                          0U

//******************************************************************************
// Register: CHANMAP4
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP4_FREQ_W                                         12U
#define RFE_CPLL_RAM_CHANMAP4_FREQ_M                                     0x0FFFU
#define RFE_CPLL_RAM_CHANMAP4_FREQ_S                                          0U

//******************************************************************************
// Register: CHANMAP5
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP5_FREQ_W                                         12U
#define RFE_CPLL_RAM_CHANMAP5_FREQ_M                                     0x0FFFU
#define RFE_CPLL_RAM_CHANMAP5_FREQ_S                                          0U

//******************************************************************************
// Register: CHANMAP6
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP6_FREQ_W                                         12U
#define RFE_CPLL_RAM_CHANMAP6_FREQ_M                                     0x0FFFU
#define RFE_CPLL_RAM_CHANMAP6_FREQ_S                                          0U

//******************************************************************************
// Register: CHANMAP7
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP7_FREQ_W                                         12U
#define RFE_CPLL_RAM_CHANMAP7_FREQ_M                                     0x0FFFU
#define RFE_CPLL_RAM_CHANMAP7_FREQ_S                                          0U

//******************************************************************************
// Register: CHANMAP8
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP8_FREQ_W                                         12U
#define RFE_CPLL_RAM_CHANMAP8_FREQ_M                                     0x0FFFU
#define RFE_CPLL_RAM_CHANMAP8_FREQ_S                                          0U

//******************************************************************************
// Register: CHANMAP9
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP9_FREQ_W                                         12U
#define RFE_CPLL_RAM_CHANMAP9_FREQ_M                                     0x0FFFU
#define RFE_CPLL_RAM_CHANMAP9_FREQ_S                                          0U

//******************************************************************************
// Register: CHANMAP10
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP10_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP10_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP10_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP11
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP11_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP11_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP11_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP12
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP12_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP12_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP12_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP13
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP13_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP13_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP13_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP14
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP14_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP14_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP14_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP15
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP15_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP15_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP15_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP16
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP16_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP16_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP16_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP17
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP17_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP17_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP17_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP18
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP18_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP18_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP18_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP19
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP19_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP19_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP19_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP20
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP20_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP20_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP20_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP21
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP21_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP21_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP21_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP22
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP22_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP22_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP22_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP23
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP23_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP23_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP23_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP25
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP25_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP25_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP25_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP26
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP26_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP26_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP26_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP27
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP27_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP27_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP27_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP28
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP28_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP28_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP28_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP29
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP29_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP29_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP29_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP30
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP30_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP30_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP30_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP31
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP31_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP31_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP31_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP32
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP32_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP32_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP32_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP33
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP33_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP33_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP33_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP34
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP34_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP34_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP34_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP35
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP35_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP35_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP35_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP36
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP36_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP36_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP36_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP37
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP37_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP37_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP37_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP38
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP38_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP38_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP38_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP39
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP39_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP39_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP39_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP40
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP40_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP40_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP40_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP41
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP41_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP41_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP41_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP42
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP42_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP42_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP42_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP43
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP43_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP43_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP43_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP44
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP44_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP44_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP44_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP45
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP45_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP45_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP45_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP46
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP46_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP46_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP46_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP47
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP47_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP47_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP47_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP48
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP48_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP48_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP48_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP49
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP49_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP49_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP49_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP50
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP50_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP50_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP50_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP51
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP51_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP51_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP51_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP52
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP52_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP52_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP52_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP53
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP53_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP53_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP53_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP54
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP54_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP54_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP54_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP55
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP55_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP55_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP55_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP56
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP56_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP56_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP56_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP57
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP57_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP57_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP57_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP58
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP58_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP58_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP58_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP59
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP59_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP59_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP59_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP60
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP60_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP60_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP60_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP61
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP61_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP61_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP61_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP62
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP62_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP62_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP62_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP63
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP63_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP63_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP63_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP65
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP65_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP65_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP65_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP66
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP66_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP66_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP66_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP67
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP67_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP67_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP67_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP68
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP68_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP68_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP68_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP69
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP69_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP69_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP69_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP70
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP70_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP70_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP70_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP71
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP71_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP71_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP71_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP72
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP72_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP72_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP72_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP73
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP73_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP73_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP73_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP74
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP74_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP74_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP74_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP75
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP75_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP75_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP75_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP76
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP76_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP76_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP76_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP77
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP77_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP77_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP77_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP78
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP78_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP78_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP78_FREQ_S                                         0U

//******************************************************************************
// Register: CHANMAP79
//******************************************************************************
// Field: [11:0] freq
//
// Channel frequency in MHz
#define RFE_CPLL_RAM_CHANMAP79_FREQ_W                                        12U
#define RFE_CPLL_RAM_CHANMAP79_FREQ_M                                    0x0FFFU
#define RFE_CPLL_RAM_CHANMAP79_FREQ_S                                         0U


#endif // __RFE_CPLL_RAM_REGS_H
