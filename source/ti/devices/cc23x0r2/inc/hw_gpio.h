/******************************************************************************
*  Filename:       hw_gpio_h
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

#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__

//*****************************************************************************
//
// This section defines the register offsets of
// GPIO component
//
//*****************************************************************************
// Module Description
#define GPIO_O_DESC                                                 0x00000000U

// Module Description Extended
#define GPIO_O_DESCEX                                               0x00000004U

// Interrupt Mask
#define GPIO_O_IMASK                                                0x00000044U

// Raw interrupt status
#define GPIO_O_RIS                                                  0x0000004CU

// Masked interrupt status
#define GPIO_O_MIS                                                  0x00000054U

// Interrupt set
#define GPIO_O_ISET                                                 0x0000005CU

// Interrupt clear
#define GPIO_O_ICLR                                                 0x00000064U

// Interrupt mask set
#define GPIO_O_IMSET                                                0x0000006CU

// Interrupt mask clear
#define GPIO_O_IMCLR                                                0x00000074U

// Digital Test Bus
#define GPIO_O_DTB                                                  0x0000007CU

// Alias for Data out 3 to 0
#define GPIO_O_DOUT3_0                                              0x00000100U

// Alias for Data out 7 to 4
#define GPIO_O_DOUT7_4                                              0x00000104U

// Alias for Data out 11 to 8
#define GPIO_O_DOUT11_8                                             0x00000108U

// Alias for Data out 15 to 12
#define GPIO_O_DOUT15_12                                            0x0000010CU

// Alias for Data out 19 to 16
#define GPIO_O_DOUT19_16                                            0x00000110U

// Alias for Data out 23 to 20
#define GPIO_O_DOUT23_20                                            0x00000114U

// Alias for Data out 27 to 24
#define GPIO_O_DOUT27_24                                            0x00000118U

// Data out 31 to 0
#define GPIO_O_DOUT31_0                                             0x00000200U

// Data out set 31 to 0
#define GPIO_O_DOUTSET31_0                                          0x00000210U

// Data out clear 31 to 0
#define GPIO_O_DOUTCLR31_0                                          0x00000220U

// Data out toggle 31 to 0
#define GPIO_O_DOUTTGL31_0                                          0x00000230U

// Alias for Data out toggle 3 to 0
#define GPIO_O_DOUTTGL3_0                                           0x00000300U

// Alias for Data out toggle 7 to 4
#define GPIO_O_DOUTTGL7_4                                           0x00000304U

// Alias for Data out toggle 11 to 8
#define GPIO_O_DOUTTGL11_8                                          0x00000308U

// Alias for Data out toggle 15 to 12
#define GPIO_O_DOUTTGL15_12                                         0x0000030CU

// Alias for Data out toggle 19 to 16
#define GPIO_O_DOUTTGL19_16                                         0x00000310U

// Alias for Data out toggle 23 to 20
#define GPIO_O_DOUTTGL23_20                                         0x00000314U

// Alias for Data out toggle 27 to 24
#define GPIO_O_DOUTTGL27_24                                         0x00000318U

// Alias for Data out enable 3 to 0
#define GPIO_O_DOE3_0                                               0x00000400U

// Alias for Data out enable 7 to 4
#define GPIO_O_DOE7_4                                               0x00000404U

// Alias for Data out enable 11 to 8
#define GPIO_O_DOE11_8                                              0x00000408U

// Alias for Data out enable 15 to 12
#define GPIO_O_DOE15_12                                             0x0000040CU

// Alias for Data out enable 19 to 16
#define GPIO_O_DOE19_16                                             0x00000410U

// Alias for Data out enable 23 to 20
#define GPIO_O_DOE23_20                                             0x00000414U

// Alias for Data out enable 27 to 24
#define GPIO_O_DOE27_24                                             0x00000418U

// Data out enable 31 to 0
#define GPIO_O_DOE31_0                                              0x00000500U

// Data out enable set 31 to 0
#define GPIO_O_DOESET31_0                                           0x00000510U

// Data out enable clear 31 to 0
#define GPIO_O_DOECLR31_0                                           0x00000520U

// Data out enable toggle 31 to 0
#define GPIO_O_DOETGL31_0                                           0x00000530U

// Alias for Data input 3 to 0
#define GPIO_O_DIN3_0                                               0x00000600U

// Alias for Data input 7 to 4
#define GPIO_O_DIN7_4                                               0x00000604U

// Alias for Data input 11 to 8
#define GPIO_O_DIN11_8                                              0x00000608U

// Alias for Data input 15 to 12
#define GPIO_O_DIN15_12                                             0x0000060CU

// Alias for Data input 19 to 16
#define GPIO_O_DIN19_16                                             0x00000610U

// Alias for Data input 23 to 20
#define GPIO_O_DIN23_20                                             0x00000614U

// Alias for Data input 27 to 24
#define GPIO_O_DIN27_24                                             0x00000618U

// Data input 31 to 0
#define GPIO_O_DIN31_0                                              0x00000700U

// Event configuration
#define GPIO_O_EVTCFG                                               0x00000800U

//*****************************************************************************
//
// Register: GPIO_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module identifier
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define GPIO_DESC_MODID_W                                                   16U
#define GPIO_DESC_MODID_M                                           0xFFFF0000U
#define GPIO_DESC_MODID_S                                                   16U
#define GPIO_DESC_MODID_MAXIMUM                                     0xFFFF0000U
#define GPIO_DESC_MODID_MINIMUM                                     0x00000000U

// Field: [15:12] STDIPOFF
//
// 0: STDIP MMRs do not exist
// 1:15: These MMRs begin at offset 64*STDIPOFF from IP base address
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define GPIO_DESC_STDIPOFF_W                                                 4U
#define GPIO_DESC_STDIPOFF_M                                        0x0000F000U
#define GPIO_DESC_STDIPOFF_S                                                12U
#define GPIO_DESC_STDIPOFF_MAXIMUM                                  0x0000F000U
#define GPIO_DESC_STDIPOFF_MINIMUM                                  0x00000000U

// Field:  [11:8] INSTIDX
//
// If multiple instances of IP exists in SOC, this field can identify the
// instance number 0-15
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define GPIO_DESC_INSTIDX_W                                                  4U
#define GPIO_DESC_INSTIDX_M                                         0x00000F00U
#define GPIO_DESC_INSTIDX_S                                                  8U
#define GPIO_DESC_INSTIDX_MAXIMUM                                   0x00000F00U
#define GPIO_DESC_INSTIDX_MINIMUM                                   0x00000000U

// Field:   [7:4] MAJREV
//
// Major revision version of IP
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define GPIO_DESC_MAJREV_W                                                   4U
#define GPIO_DESC_MAJREV_M                                          0x000000F0U
#define GPIO_DESC_MAJREV_S                                                   4U
#define GPIO_DESC_MAJREV_MAXIMUM                                    0x000000F0U
#define GPIO_DESC_MAJREV_MINIMUM                                    0x00000000U

// Field:   [3:0] MINREV
//
// Minor revision version of IP
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define GPIO_DESC_MINREV_W                                                   4U
#define GPIO_DESC_MINREV_M                                          0x0000000FU
#define GPIO_DESC_MINREV_S                                                   0U
#define GPIO_DESC_MINREV_MAXIMUM                                    0x0000000FU
#define GPIO_DESC_MINREV_MINIMUM                                    0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DESCEX
//
//*****************************************************************************
// Field:   [5:0] NUMDIO
//
// This provides the total number of DIOs supported by GPIO. The number of DIOs
// supprted is NUMDIO + 1
// ENUMs:
// MAXIMUM                  Highest possible value
// MINIMUM                  Smallest value
#define GPIO_DESCEX_NUMDIO_W                                                 6U
#define GPIO_DESCEX_NUMDIO_M                                        0x0000003FU
#define GPIO_DESCEX_NUMDIO_S                                                 0U
#define GPIO_DESCEX_NUMDIO_MAXIMUM                                  0x0000003FU
#define GPIO_DESCEX_NUMDIO_MINIMUM                                  0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_IMASK
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Interrupt mask for DIO24
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO24                                            0x01000000U
#define GPIO_IMASK_DIO24_M                                          0x01000000U
#define GPIO_IMASK_DIO24_S                                                  24U
#define GPIO_IMASK_DIO24_SET                                        0x01000000U
#define GPIO_IMASK_DIO24_CLR                                        0x00000000U

// Field:    [21] DIO21
//
// Interrupt mask for DIO21
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO21                                            0x00200000U
#define GPIO_IMASK_DIO21_M                                          0x00200000U
#define GPIO_IMASK_DIO21_S                                                  21U
#define GPIO_IMASK_DIO21_SET                                        0x00200000U
#define GPIO_IMASK_DIO21_CLR                                        0x00000000U

// Field:    [20] DIO20
//
// Interrupt mask for DIO20
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO20                                            0x00100000U
#define GPIO_IMASK_DIO20_M                                          0x00100000U
#define GPIO_IMASK_DIO20_S                                                  20U
#define GPIO_IMASK_DIO20_SET                                        0x00100000U
#define GPIO_IMASK_DIO20_CLR                                        0x00000000U

// Field:    [17] DIO17
//
// Interrupt mask for DIO17
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO17                                            0x00020000U
#define GPIO_IMASK_DIO17_M                                          0x00020000U
#define GPIO_IMASK_DIO17_S                                                  17U
#define GPIO_IMASK_DIO17_SET                                        0x00020000U
#define GPIO_IMASK_DIO17_CLR                                        0x00000000U

// Field:    [16] DIO16
//
// Interrupt mask for DIO16
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO16                                            0x00010000U
#define GPIO_IMASK_DIO16_M                                          0x00010000U
#define GPIO_IMASK_DIO16_S                                                  16U
#define GPIO_IMASK_DIO16_SET                                        0x00010000U
#define GPIO_IMASK_DIO16_CLR                                        0x00000000U

// Field:    [13] DIO13
//
// Interrupt mask for DIO13
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO13                                            0x00002000U
#define GPIO_IMASK_DIO13_M                                          0x00002000U
#define GPIO_IMASK_DIO13_S                                                  13U
#define GPIO_IMASK_DIO13_SET                                        0x00002000U
#define GPIO_IMASK_DIO13_CLR                                        0x00000000U

// Field:    [12] DIO12
//
// Interrupt mask for DIO12
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO12                                            0x00001000U
#define GPIO_IMASK_DIO12_M                                          0x00001000U
#define GPIO_IMASK_DIO12_S                                                  12U
#define GPIO_IMASK_DIO12_SET                                        0x00001000U
#define GPIO_IMASK_DIO12_CLR                                        0x00000000U

// Field:    [11] DIO11
//
// Interrupt mask for DIO11
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO11                                            0x00000800U
#define GPIO_IMASK_DIO11_M                                          0x00000800U
#define GPIO_IMASK_DIO11_S                                                  11U
#define GPIO_IMASK_DIO11_SET                                        0x00000800U
#define GPIO_IMASK_DIO11_CLR                                        0x00000000U

// Field:     [8] DIO8
//
// Interrupt mask for DIO8
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO8                                             0x00000100U
#define GPIO_IMASK_DIO8_M                                           0x00000100U
#define GPIO_IMASK_DIO8_S                                                    8U
#define GPIO_IMASK_DIO8_SET                                         0x00000100U
#define GPIO_IMASK_DIO8_CLR                                         0x00000000U

// Field:     [6] DIO6
//
// Interrupt mask for DIO6
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO6                                             0x00000040U
#define GPIO_IMASK_DIO6_M                                           0x00000040U
#define GPIO_IMASK_DIO6_S                                                    6U
#define GPIO_IMASK_DIO6_SET                                         0x00000040U
#define GPIO_IMASK_DIO6_CLR                                         0x00000000U

// Field:     [4] DIO4
//
// Interrupt mask for DIO4
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO4                                             0x00000010U
#define GPIO_IMASK_DIO4_M                                           0x00000010U
#define GPIO_IMASK_DIO4_S                                                    4U
#define GPIO_IMASK_DIO4_SET                                         0x00000010U
#define GPIO_IMASK_DIO4_CLR                                         0x00000000U

// Field:     [3] DIO3
//
// Interrupt mask for DIO3
// ENUMs:
// SET                      Set Interrrupt Mask
// CLR                      Clear Interrupt Mask
#define GPIO_IMASK_DIO3                                             0x00000008U
#define GPIO_IMASK_DIO3_M                                           0x00000008U
#define GPIO_IMASK_DIO3_S                                                    3U
#define GPIO_IMASK_DIO3_SET                                         0x00000008U
#define GPIO_IMASK_DIO3_CLR                                         0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_RIS
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Raw interrupt flag for DIO24
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO24                                              0x01000000U
#define GPIO_RIS_DIO24_M                                            0x01000000U
#define GPIO_RIS_DIO24_S                                                    24U
#define GPIO_RIS_DIO24_SET                                          0x01000000U
#define GPIO_RIS_DIO24_CLR                                          0x00000000U

// Field:    [21] DIO21
//
// Raw interrupt flag for DIO21
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO21                                              0x00200000U
#define GPIO_RIS_DIO21_M                                            0x00200000U
#define GPIO_RIS_DIO21_S                                                    21U
#define GPIO_RIS_DIO21_SET                                          0x00200000U
#define GPIO_RIS_DIO21_CLR                                          0x00000000U

// Field:    [20] DIO20
//
// Raw interrupt flag for DIO20
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO20                                              0x00100000U
#define GPIO_RIS_DIO20_M                                            0x00100000U
#define GPIO_RIS_DIO20_S                                                    20U
#define GPIO_RIS_DIO20_SET                                          0x00100000U
#define GPIO_RIS_DIO20_CLR                                          0x00000000U

// Field:    [17] DIO17
//
// Raw interrupt flag for DIO17
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO17                                              0x00020000U
#define GPIO_RIS_DIO17_M                                            0x00020000U
#define GPIO_RIS_DIO17_S                                                    17U
#define GPIO_RIS_DIO17_SET                                          0x00020000U
#define GPIO_RIS_DIO17_CLR                                          0x00000000U

// Field:    [16] DIO16
//
// Raw interrupt flag for DIO16
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO16                                              0x00010000U
#define GPIO_RIS_DIO16_M                                            0x00010000U
#define GPIO_RIS_DIO16_S                                                    16U
#define GPIO_RIS_DIO16_SET                                          0x00010000U
#define GPIO_RIS_DIO16_CLR                                          0x00000000U

// Field:    [13] DIO13
//
// Raw interrupt flag for DIO13
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO13                                              0x00002000U
#define GPIO_RIS_DIO13_M                                            0x00002000U
#define GPIO_RIS_DIO13_S                                                    13U
#define GPIO_RIS_DIO13_SET                                          0x00002000U
#define GPIO_RIS_DIO13_CLR                                          0x00000000U

// Field:    [12] DIO12
//
// Raw interrupt flag for DIO12
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO12                                              0x00001000U
#define GPIO_RIS_DIO12_M                                            0x00001000U
#define GPIO_RIS_DIO12_S                                                    12U
#define GPIO_RIS_DIO12_SET                                          0x00001000U
#define GPIO_RIS_DIO12_CLR                                          0x00000000U

// Field:    [11] DIO11
//
// Raw interrupt flag for DIO11
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO11                                              0x00000800U
#define GPIO_RIS_DIO11_M                                            0x00000800U
#define GPIO_RIS_DIO11_S                                                    11U
#define GPIO_RIS_DIO11_SET                                          0x00000800U
#define GPIO_RIS_DIO11_CLR                                          0x00000000U

// Field:     [8] DIO8
//
// Raw interrupt flag for DIO8
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO8                                               0x00000100U
#define GPIO_RIS_DIO8_M                                             0x00000100U
#define GPIO_RIS_DIO8_S                                                      8U
#define GPIO_RIS_DIO8_SET                                           0x00000100U
#define GPIO_RIS_DIO8_CLR                                           0x00000000U

// Field:     [6] DIO6
//
// Raw interrupt flag for DIO6
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO6                                               0x00000040U
#define GPIO_RIS_DIO6_M                                             0x00000040U
#define GPIO_RIS_DIO6_S                                                      6U
#define GPIO_RIS_DIO6_SET                                           0x00000040U
#define GPIO_RIS_DIO6_CLR                                           0x00000000U

// Field:     [4] DIO4
//
// Raw interrupt flag for DIO4
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO4                                               0x00000010U
#define GPIO_RIS_DIO4_M                                             0x00000010U
#define GPIO_RIS_DIO4_S                                                      4U
#define GPIO_RIS_DIO4_SET                                           0x00000010U
#define GPIO_RIS_DIO4_CLR                                           0x00000000U

// Field:     [3] DIO3
//
// Raw interrupt flag for DIO3
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_RIS_DIO3                                               0x00000008U
#define GPIO_RIS_DIO3_M                                             0x00000008U
#define GPIO_RIS_DIO3_S                                                      3U
#define GPIO_RIS_DIO3_SET                                           0x00000008U
#define GPIO_RIS_DIO3_CLR                                           0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_MIS
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Masked interrupt flag for DIO24
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO24                                              0x01000000U
#define GPIO_MIS_DIO24_M                                            0x01000000U
#define GPIO_MIS_DIO24_S                                                    24U
#define GPIO_MIS_DIO24_SET                                          0x01000000U
#define GPIO_MIS_DIO24_CLR                                          0x00000000U

// Field:    [21] DIO21
//
// Masked interrupt flag for DIO21
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO21                                              0x00200000U
#define GPIO_MIS_DIO21_M                                            0x00200000U
#define GPIO_MIS_DIO21_S                                                    21U
#define GPIO_MIS_DIO21_SET                                          0x00200000U
#define GPIO_MIS_DIO21_CLR                                          0x00000000U

// Field:    [20] DIO20
//
// Masked interrupt flag for DIO20
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO20                                              0x00100000U
#define GPIO_MIS_DIO20_M                                            0x00100000U
#define GPIO_MIS_DIO20_S                                                    20U
#define GPIO_MIS_DIO20_SET                                          0x00100000U
#define GPIO_MIS_DIO20_CLR                                          0x00000000U

// Field:    [17] DIO17
//
// Masked interrupt flag for DIO17
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO17                                              0x00020000U
#define GPIO_MIS_DIO17_M                                            0x00020000U
#define GPIO_MIS_DIO17_S                                                    17U
#define GPIO_MIS_DIO17_SET                                          0x00020000U
#define GPIO_MIS_DIO17_CLR                                          0x00000000U

// Field:    [16] DIO16
//
// Masked interrupt flag for DIO16
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO16                                              0x00010000U
#define GPIO_MIS_DIO16_M                                            0x00010000U
#define GPIO_MIS_DIO16_S                                                    16U
#define GPIO_MIS_DIO16_SET                                          0x00010000U
#define GPIO_MIS_DIO16_CLR                                          0x00000000U

// Field:    [13] DIO13
//
// Masked interrupt flag for DIO13
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO13                                              0x00002000U
#define GPIO_MIS_DIO13_M                                            0x00002000U
#define GPIO_MIS_DIO13_S                                                    13U
#define GPIO_MIS_DIO13_SET                                          0x00002000U
#define GPIO_MIS_DIO13_CLR                                          0x00000000U

// Field:    [12] DIO12
//
// Masked interrupt flag for DIO12
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO12                                              0x00001000U
#define GPIO_MIS_DIO12_M                                            0x00001000U
#define GPIO_MIS_DIO12_S                                                    12U
#define GPIO_MIS_DIO12_SET                                          0x00001000U
#define GPIO_MIS_DIO12_CLR                                          0x00000000U

// Field:    [11] DIO11
//
// Masked interrupt flag for DIO11
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO11                                              0x00000800U
#define GPIO_MIS_DIO11_M                                            0x00000800U
#define GPIO_MIS_DIO11_S                                                    11U
#define GPIO_MIS_DIO11_SET                                          0x00000800U
#define GPIO_MIS_DIO11_CLR                                          0x00000000U

// Field:     [8] DIO8
//
// Masked interrupt flag for DIO8
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO8                                               0x00000100U
#define GPIO_MIS_DIO8_M                                             0x00000100U
#define GPIO_MIS_DIO8_S                                                      8U
#define GPIO_MIS_DIO8_SET                                           0x00000100U
#define GPIO_MIS_DIO8_CLR                                           0x00000000U

// Field:     [6] DIO6
//
// Masked interrupt flag for DIO6
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO6                                               0x00000040U
#define GPIO_MIS_DIO6_M                                             0x00000040U
#define GPIO_MIS_DIO6_S                                                      6U
#define GPIO_MIS_DIO6_SET                                           0x00000040U
#define GPIO_MIS_DIO6_CLR                                           0x00000000U

// Field:     [4] DIO4
//
// Masked interrupt flag for DIO4
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO4                                               0x00000010U
#define GPIO_MIS_DIO4_M                                             0x00000010U
#define GPIO_MIS_DIO4_S                                                      4U
#define GPIO_MIS_DIO4_SET                                           0x00000010U
#define GPIO_MIS_DIO4_CLR                                           0x00000000U

// Field:     [3] DIO3
//
// Masked interrupt flag for DIO3
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define GPIO_MIS_DIO3                                               0x00000008U
#define GPIO_MIS_DIO3_M                                             0x00000008U
#define GPIO_MIS_DIO3_S                                                      3U
#define GPIO_MIS_DIO3_SET                                           0x00000008U
#define GPIO_MIS_DIO3_CLR                                           0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_ISET
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Set DIO24 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO24                                             0x01000000U
#define GPIO_ISET_DIO24_M                                           0x01000000U
#define GPIO_ISET_DIO24_S                                                   24U
#define GPIO_ISET_DIO24_SET                                         0x01000000U
#define GPIO_ISET_DIO24_NO_EFFECT                                   0x00000000U

// Field:    [21] DIO21
//
// Set DIO21 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO21                                             0x00200000U
#define GPIO_ISET_DIO21_M                                           0x00200000U
#define GPIO_ISET_DIO21_S                                                   21U
#define GPIO_ISET_DIO21_SET                                         0x00200000U
#define GPIO_ISET_DIO21_NO_EFFECT                                   0x00000000U

// Field:    [20] DIO20
//
// Set DIO20 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO20                                             0x00100000U
#define GPIO_ISET_DIO20_M                                           0x00100000U
#define GPIO_ISET_DIO20_S                                                   20U
#define GPIO_ISET_DIO20_SET                                         0x00100000U
#define GPIO_ISET_DIO20_NO_EFFECT                                   0x00000000U

// Field:    [17] DIO17
//
// Set DIO17 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO17                                             0x00020000U
#define GPIO_ISET_DIO17_M                                           0x00020000U
#define GPIO_ISET_DIO17_S                                                   17U
#define GPIO_ISET_DIO17_SET                                         0x00020000U
#define GPIO_ISET_DIO17_NO_EFFECT                                   0x00000000U

// Field:    [16] DIO16
//
// Set DIO16 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO16                                             0x00010000U
#define GPIO_ISET_DIO16_M                                           0x00010000U
#define GPIO_ISET_DIO16_S                                                   16U
#define GPIO_ISET_DIO16_SET                                         0x00010000U
#define GPIO_ISET_DIO16_NO_EFFECT                                   0x00000000U

// Field:    [13] DIO13
//
// Set DIO13 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO13                                             0x00002000U
#define GPIO_ISET_DIO13_M                                           0x00002000U
#define GPIO_ISET_DIO13_S                                                   13U
#define GPIO_ISET_DIO13_SET                                         0x00002000U
#define GPIO_ISET_DIO13_NO_EFFECT                                   0x00000000U

// Field:    [12] DIO12
//
// Set DIO12 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO12                                             0x00001000U
#define GPIO_ISET_DIO12_M                                           0x00001000U
#define GPIO_ISET_DIO12_S                                                   12U
#define GPIO_ISET_DIO12_SET                                         0x00001000U
#define GPIO_ISET_DIO12_NO_EFFECT                                   0x00000000U

// Field:    [11] DIO11
//
// Set DIO11 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO11                                             0x00000800U
#define GPIO_ISET_DIO11_M                                           0x00000800U
#define GPIO_ISET_DIO11_S                                                   11U
#define GPIO_ISET_DIO11_SET                                         0x00000800U
#define GPIO_ISET_DIO11_NO_EFFECT                                   0x00000000U

// Field:     [8] DIO8
//
// Set DIO8 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO8                                              0x00000100U
#define GPIO_ISET_DIO8_M                                            0x00000100U
#define GPIO_ISET_DIO8_S                                                     8U
#define GPIO_ISET_DIO8_SET                                          0x00000100U
#define GPIO_ISET_DIO8_NO_EFFECT                                    0x00000000U

// Field:     [6] DIO6
//
// Set DIO6 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO6                                              0x00000040U
#define GPIO_ISET_DIO6_M                                            0x00000040U
#define GPIO_ISET_DIO6_S                                                     6U
#define GPIO_ISET_DIO6_SET                                          0x00000040U
#define GPIO_ISET_DIO6_NO_EFFECT                                    0x00000000U

// Field:     [4] DIO4
//
// Set DIO4 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO4                                              0x00000010U
#define GPIO_ISET_DIO4_M                                            0x00000010U
#define GPIO_ISET_DIO4_S                                                     4U
#define GPIO_ISET_DIO4_SET                                          0x00000010U
#define GPIO_ISET_DIO4_NO_EFFECT                                    0x00000000U

// Field:     [3] DIO3
//
// Set DIO3 in RIS
// ENUMs:
// SET                      Set Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ISET_DIO3                                              0x00000008U
#define GPIO_ISET_DIO3_M                                            0x00000008U
#define GPIO_ISET_DIO3_S                                                     3U
#define GPIO_ISET_DIO3_SET                                          0x00000008U
#define GPIO_ISET_DIO3_NO_EFFECT                                    0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_ICLR
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Clears DIO24 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO24                                             0x01000000U
#define GPIO_ICLR_DIO24_M                                           0x01000000U
#define GPIO_ICLR_DIO24_S                                                   24U
#define GPIO_ICLR_DIO24_CLR                                         0x01000000U
#define GPIO_ICLR_DIO24_NO_EFFECT                                   0x00000000U

// Field:    [21] DIO21
//
// Clears DIO21 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO21                                             0x00200000U
#define GPIO_ICLR_DIO21_M                                           0x00200000U
#define GPIO_ICLR_DIO21_S                                                   21U
#define GPIO_ICLR_DIO21_CLR                                         0x00200000U
#define GPIO_ICLR_DIO21_NO_EFFECT                                   0x00000000U

// Field:    [20] DIO20
//
// Clears DIO20 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO20                                             0x00100000U
#define GPIO_ICLR_DIO20_M                                           0x00100000U
#define GPIO_ICLR_DIO20_S                                                   20U
#define GPIO_ICLR_DIO20_CLR                                         0x00100000U
#define GPIO_ICLR_DIO20_NO_EFFECT                                   0x00000000U

// Field:    [17] DIO17
//
// Clears DIO17 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO17                                             0x00020000U
#define GPIO_ICLR_DIO17_M                                           0x00020000U
#define GPIO_ICLR_DIO17_S                                                   17U
#define GPIO_ICLR_DIO17_CLR                                         0x00020000U
#define GPIO_ICLR_DIO17_NO_EFFECT                                   0x00000000U

// Field:    [16] DIO16
//
// Clears DIO16 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO16                                             0x00010000U
#define GPIO_ICLR_DIO16_M                                           0x00010000U
#define GPIO_ICLR_DIO16_S                                                   16U
#define GPIO_ICLR_DIO16_CLR                                         0x00010000U
#define GPIO_ICLR_DIO16_NO_EFFECT                                   0x00000000U

// Field:    [13] DIO13
//
// Clears DIO13 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO13                                             0x00002000U
#define GPIO_ICLR_DIO13_M                                           0x00002000U
#define GPIO_ICLR_DIO13_S                                                   13U
#define GPIO_ICLR_DIO13_CLR                                         0x00002000U
#define GPIO_ICLR_DIO13_NO_EFFECT                                   0x00000000U

// Field:    [12] DIO12
//
// Clears DIO12 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO12                                             0x00001000U
#define GPIO_ICLR_DIO12_M                                           0x00001000U
#define GPIO_ICLR_DIO12_S                                                   12U
#define GPIO_ICLR_DIO12_CLR                                         0x00001000U
#define GPIO_ICLR_DIO12_NO_EFFECT                                   0x00000000U

// Field:    [11] DIO11
//
// Clears DIO11 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO11                                             0x00000800U
#define GPIO_ICLR_DIO11_M                                           0x00000800U
#define GPIO_ICLR_DIO11_S                                                   11U
#define GPIO_ICLR_DIO11_CLR                                         0x00000800U
#define GPIO_ICLR_DIO11_NO_EFFECT                                   0x00000000U

// Field:     [8] DIO8
//
// Clears DIO8 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO8                                              0x00000100U
#define GPIO_ICLR_DIO8_M                                            0x00000100U
#define GPIO_ICLR_DIO8_S                                                     8U
#define GPIO_ICLR_DIO8_CLR                                          0x00000100U
#define GPIO_ICLR_DIO8_NO_EFFECT                                    0x00000000U

// Field:     [6] DIO6
//
// Clears DIO6 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO6                                              0x00000040U
#define GPIO_ICLR_DIO6_M                                            0x00000040U
#define GPIO_ICLR_DIO6_S                                                     6U
#define GPIO_ICLR_DIO6_CLR                                          0x00000040U
#define GPIO_ICLR_DIO6_NO_EFFECT                                    0x00000000U

// Field:     [4] DIO4
//
// Clears DIO4 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO4                                              0x00000010U
#define GPIO_ICLR_DIO4_M                                            0x00000010U
#define GPIO_ICLR_DIO4_S                                                     4U
#define GPIO_ICLR_DIO4_CLR                                          0x00000010U
#define GPIO_ICLR_DIO4_NO_EFFECT                                    0x00000000U

// Field:     [3] DIO3
//
// Clears DIO3 in RIS
// ENUMs:
// CLR                      Clear Interrupt
// NO_EFFECT                Writing 0 has no effect
#define GPIO_ICLR_DIO3                                              0x00000008U
#define GPIO_ICLR_DIO3_M                                            0x00000008U
#define GPIO_ICLR_DIO3_S                                                     3U
#define GPIO_ICLR_DIO3_CLR                                          0x00000008U
#define GPIO_ICLR_DIO3_NO_EFFECT                                    0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_IMSET
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Sets DIO24 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO24                                            0x01000000U
#define GPIO_IMSET_DIO24_M                                          0x01000000U
#define GPIO_IMSET_DIO24_S                                                  24U
#define GPIO_IMSET_DIO24_SET                                        0x01000000U
#define GPIO_IMSET_DIO24_NO_EFFECT                                  0x00000000U

// Field:    [21] DIO21
//
// Sets DIO21 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO21                                            0x00200000U
#define GPIO_IMSET_DIO21_M                                          0x00200000U
#define GPIO_IMSET_DIO21_S                                                  21U
#define GPIO_IMSET_DIO21_SET                                        0x00200000U
#define GPIO_IMSET_DIO21_NO_EFFECT                                  0x00000000U

// Field:    [20] DIO20
//
// Sets DIO20 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO20                                            0x00100000U
#define GPIO_IMSET_DIO20_M                                          0x00100000U
#define GPIO_IMSET_DIO20_S                                                  20U
#define GPIO_IMSET_DIO20_SET                                        0x00100000U
#define GPIO_IMSET_DIO20_NO_EFFECT                                  0x00000000U

// Field:    [17] DIO17
//
// Sets DIO17 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO17                                            0x00020000U
#define GPIO_IMSET_DIO17_M                                          0x00020000U
#define GPIO_IMSET_DIO17_S                                                  17U
#define GPIO_IMSET_DIO17_SET                                        0x00020000U
#define GPIO_IMSET_DIO17_NO_EFFECT                                  0x00000000U

// Field:    [16] DIO16
//
// Sets DIO16 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO16                                            0x00010000U
#define GPIO_IMSET_DIO16_M                                          0x00010000U
#define GPIO_IMSET_DIO16_S                                                  16U
#define GPIO_IMSET_DIO16_SET                                        0x00010000U
#define GPIO_IMSET_DIO16_NO_EFFECT                                  0x00000000U

// Field:    [13] DIO13
//
// Sets DIO13 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO13                                            0x00002000U
#define GPIO_IMSET_DIO13_M                                          0x00002000U
#define GPIO_IMSET_DIO13_S                                                  13U
#define GPIO_IMSET_DIO13_SET                                        0x00002000U
#define GPIO_IMSET_DIO13_NO_EFFECT                                  0x00000000U

// Field:    [12] DIO12
//
// Sets DIO12 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO12                                            0x00001000U
#define GPIO_IMSET_DIO12_M                                          0x00001000U
#define GPIO_IMSET_DIO12_S                                                  12U
#define GPIO_IMSET_DIO12_SET                                        0x00001000U
#define GPIO_IMSET_DIO12_NO_EFFECT                                  0x00000000U

// Field:    [11] DIO11
//
// Sets DIO11 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO11                                            0x00000800U
#define GPIO_IMSET_DIO11_M                                          0x00000800U
#define GPIO_IMSET_DIO11_S                                                  11U
#define GPIO_IMSET_DIO11_SET                                        0x00000800U
#define GPIO_IMSET_DIO11_NO_EFFECT                                  0x00000000U

// Field:     [8] DIO8
//
// Sets DIO8 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO8                                             0x00000100U
#define GPIO_IMSET_DIO8_M                                           0x00000100U
#define GPIO_IMSET_DIO8_S                                                    8U
#define GPIO_IMSET_DIO8_SET                                         0x00000100U
#define GPIO_IMSET_DIO8_NO_EFFECT                                   0x00000000U

// Field:     [6] DIO6
//
// Sets DIO6 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO6                                             0x00000040U
#define GPIO_IMSET_DIO6_M                                           0x00000040U
#define GPIO_IMSET_DIO6_S                                                    6U
#define GPIO_IMSET_DIO6_SET                                         0x00000040U
#define GPIO_IMSET_DIO6_NO_EFFECT                                   0x00000000U

// Field:     [4] DIO4
//
// Sets DIO4 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO4                                             0x00000010U
#define GPIO_IMSET_DIO4_M                                           0x00000010U
#define GPIO_IMSET_DIO4_S                                                    4U
#define GPIO_IMSET_DIO4_SET                                         0x00000010U
#define GPIO_IMSET_DIO4_NO_EFFECT                                   0x00000000U

// Field:     [3] DIO3
//
// Sets DIO3 in IMASK
// ENUMs:
// SET                      Set interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMSET_DIO3                                             0x00000008U
#define GPIO_IMSET_DIO3_M                                           0x00000008U
#define GPIO_IMSET_DIO3_S                                                    3U
#define GPIO_IMSET_DIO3_SET                                         0x00000008U
#define GPIO_IMSET_DIO3_NO_EFFECT                                   0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_IMCLR
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Clears DIO24 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO24                                            0x01000000U
#define GPIO_IMCLR_DIO24_M                                          0x01000000U
#define GPIO_IMCLR_DIO24_S                                                  24U
#define GPIO_IMCLR_DIO24_CLR                                        0x01000000U
#define GPIO_IMCLR_DIO24_NO_EFFECT                                  0x00000000U

// Field:    [21] DIO21
//
// Clears DIO21 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO21                                            0x00200000U
#define GPIO_IMCLR_DIO21_M                                          0x00200000U
#define GPIO_IMCLR_DIO21_S                                                  21U
#define GPIO_IMCLR_DIO21_CLR                                        0x00200000U
#define GPIO_IMCLR_DIO21_NO_EFFECT                                  0x00000000U

// Field:    [20] DIO20
//
// Clears DIO20 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO20                                            0x00100000U
#define GPIO_IMCLR_DIO20_M                                          0x00100000U
#define GPIO_IMCLR_DIO20_S                                                  20U
#define GPIO_IMCLR_DIO20_CLR                                        0x00100000U
#define GPIO_IMCLR_DIO20_NO_EFFECT                                  0x00000000U

// Field:    [17] DIO17
//
// Clears DIO17 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO17                                            0x00020000U
#define GPIO_IMCLR_DIO17_M                                          0x00020000U
#define GPIO_IMCLR_DIO17_S                                                  17U
#define GPIO_IMCLR_DIO17_CLR                                        0x00020000U
#define GPIO_IMCLR_DIO17_NO_EFFECT                                  0x00000000U

// Field:    [16] DIO16
//
// Clears DIO16 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO16                                            0x00010000U
#define GPIO_IMCLR_DIO16_M                                          0x00010000U
#define GPIO_IMCLR_DIO16_S                                                  16U
#define GPIO_IMCLR_DIO16_CLR                                        0x00010000U
#define GPIO_IMCLR_DIO16_NO_EFFECT                                  0x00000000U

// Field:    [13] DIO13
//
// Clears DIO13 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO13                                            0x00002000U
#define GPIO_IMCLR_DIO13_M                                          0x00002000U
#define GPIO_IMCLR_DIO13_S                                                  13U
#define GPIO_IMCLR_DIO13_CLR                                        0x00002000U
#define GPIO_IMCLR_DIO13_NO_EFFECT                                  0x00000000U

// Field:    [12] DIO12
//
// Clears DIO12 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO12                                            0x00001000U
#define GPIO_IMCLR_DIO12_M                                          0x00001000U
#define GPIO_IMCLR_DIO12_S                                                  12U
#define GPIO_IMCLR_DIO12_CLR                                        0x00001000U
#define GPIO_IMCLR_DIO12_NO_EFFECT                                  0x00000000U

// Field:    [11] DIO11
//
// Clears DIO11 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO11                                            0x00000800U
#define GPIO_IMCLR_DIO11_M                                          0x00000800U
#define GPIO_IMCLR_DIO11_S                                                  11U
#define GPIO_IMCLR_DIO11_CLR                                        0x00000800U
#define GPIO_IMCLR_DIO11_NO_EFFECT                                  0x00000000U

// Field:     [8] DIO8
//
// Clears DIO8 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO8                                             0x00000100U
#define GPIO_IMCLR_DIO8_M                                           0x00000100U
#define GPIO_IMCLR_DIO8_S                                                    8U
#define GPIO_IMCLR_DIO8_CLR                                         0x00000100U
#define GPIO_IMCLR_DIO8_NO_EFFECT                                   0x00000000U

// Field:     [6] DIO6
//
// Clears DIO6 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO6                                             0x00000040U
#define GPIO_IMCLR_DIO6_M                                           0x00000040U
#define GPIO_IMCLR_DIO6_S                                                    6U
#define GPIO_IMCLR_DIO6_CLR                                         0x00000040U
#define GPIO_IMCLR_DIO6_NO_EFFECT                                   0x00000000U

// Field:     [4] DIO4
//
// Clears DIO4 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO4                                             0x00000010U
#define GPIO_IMCLR_DIO4_M                                           0x00000010U
#define GPIO_IMCLR_DIO4_S                                                    4U
#define GPIO_IMCLR_DIO4_CLR                                         0x00000010U
#define GPIO_IMCLR_DIO4_NO_EFFECT                                   0x00000000U

// Field:     [3] DIO3
//
// Clears DIO3 in IMASK
// ENUMs:
// CLR                      Clear interrupt mask
// NO_EFFECT                Writing 0 has no effect
#define GPIO_IMCLR_DIO3                                             0x00000008U
#define GPIO_IMCLR_DIO3_M                                           0x00000008U
#define GPIO_IMCLR_DIO3_S                                                    3U
#define GPIO_IMCLR_DIO3_CLR                                         0x00000008U
#define GPIO_IMCLR_DIO3_NO_EFFECT                                   0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DTB
//
//*****************************************************************************
// Field:   [2:0] SEL
//
// DTB MUX select pin value
// ENUMs:
// GRP7                     Selects test group 7
// GRP6                     Selects test group 6
// GRP5                     Selects test group 5
// GRP4                     Selects test group 4
// GRP3                     Selects test group 3
// GRP2                     Selects test group 2
// GRP1                     Selects test group 1
// DISABLE                  DTB output from peripheral is 0x0.
#define GPIO_DTB_SEL_W                                                       3U
#define GPIO_DTB_SEL_M                                              0x00000007U
#define GPIO_DTB_SEL_S                                                       0U
#define GPIO_DTB_SEL_GRP7                                           0x00000007U
#define GPIO_DTB_SEL_GRP6                                           0x00000006U
#define GPIO_DTB_SEL_GRP5                                           0x00000005U
#define GPIO_DTB_SEL_GRP4                                           0x00000004U
#define GPIO_DTB_SEL_GRP3                                           0x00000003U
#define GPIO_DTB_SEL_GRP2                                           0x00000002U
#define GPIO_DTB_SEL_GRP1                                           0x00000001U
#define GPIO_DTB_SEL_DISABLE                                        0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUT3_0
//
//*****************************************************************************
// Field:    [24] DIO3
//
// Data output for DIO3
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT3_0_DIO3                                           0x01000000U
#define GPIO_DOUT3_0_DIO3_M                                         0x01000000U
#define GPIO_DOUT3_0_DIO3_S                                                 24U
#define GPIO_DOUT3_0_DIO3_ONE                                       0x01000000U
#define GPIO_DOUT3_0_DIO3_ZERO                                      0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUT7_4
//
//*****************************************************************************
// Field:    [16] DIO6
//
// Data output for DIO6
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT7_4_DIO6                                           0x00010000U
#define GPIO_DOUT7_4_DIO6_M                                         0x00010000U
#define GPIO_DOUT7_4_DIO6_S                                                 16U
#define GPIO_DOUT7_4_DIO6_ONE                                       0x00010000U
#define GPIO_DOUT7_4_DIO6_ZERO                                      0x00000000U

// Field:     [0] DIO4
//
// Data output for DIO4
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT7_4_DIO4                                           0x00000001U
#define GPIO_DOUT7_4_DIO4_M                                         0x00000001U
#define GPIO_DOUT7_4_DIO4_S                                                  0U
#define GPIO_DOUT7_4_DIO4_ONE                                       0x00000001U
#define GPIO_DOUT7_4_DIO4_ZERO                                      0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUT11_8
//
//*****************************************************************************
// Field:    [24] DIO11
//
// Data output for DIO11
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT11_8_DIO11                                         0x01000000U
#define GPIO_DOUT11_8_DIO11_M                                       0x01000000U
#define GPIO_DOUT11_8_DIO11_S                                               24U
#define GPIO_DOUT11_8_DIO11_ONE                                     0x01000000U
#define GPIO_DOUT11_8_DIO11_ZERO                                    0x00000000U

// Field:     [0] DIO8
//
// Data output for DIO8
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT11_8_DIO8                                          0x00000001U
#define GPIO_DOUT11_8_DIO8_M                                        0x00000001U
#define GPIO_DOUT11_8_DIO8_S                                                 0U
#define GPIO_DOUT11_8_DIO8_ONE                                      0x00000001U
#define GPIO_DOUT11_8_DIO8_ZERO                                     0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUT15_12
//
//*****************************************************************************
// Field:     [8] DIO13
//
// Data output for DIO13
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT15_12_DIO13                                        0x00000100U
#define GPIO_DOUT15_12_DIO13_M                                      0x00000100U
#define GPIO_DOUT15_12_DIO13_S                                               8U
#define GPIO_DOUT15_12_DIO13_ONE                                    0x00000100U
#define GPIO_DOUT15_12_DIO13_ZERO                                   0x00000000U

// Field:     [0] DIO12
//
// Data output for DIO12
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT15_12_DIO12                                        0x00000001U
#define GPIO_DOUT15_12_DIO12_M                                      0x00000001U
#define GPIO_DOUT15_12_DIO12_S                                               0U
#define GPIO_DOUT15_12_DIO12_ONE                                    0x00000001U
#define GPIO_DOUT15_12_DIO12_ZERO                                   0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUT19_16
//
//*****************************************************************************
// Field:     [8] DIO17
//
// Data output for DIO17
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT19_16_DIO17                                        0x00000100U
#define GPIO_DOUT19_16_DIO17_M                                      0x00000100U
#define GPIO_DOUT19_16_DIO17_S                                               8U
#define GPIO_DOUT19_16_DIO17_ONE                                    0x00000100U
#define GPIO_DOUT19_16_DIO17_ZERO                                   0x00000000U

// Field:     [0] DIO16
//
// Data output for DIO16
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT19_16_DIO16                                        0x00000001U
#define GPIO_DOUT19_16_DIO16_M                                      0x00000001U
#define GPIO_DOUT19_16_DIO16_S                                               0U
#define GPIO_DOUT19_16_DIO16_ONE                                    0x00000001U
#define GPIO_DOUT19_16_DIO16_ZERO                                   0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUT23_20
//
//*****************************************************************************
// Field:     [8] DIO21
//
// Data output for DIO21
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT23_20_DIO21                                        0x00000100U
#define GPIO_DOUT23_20_DIO21_M                                      0x00000100U
#define GPIO_DOUT23_20_DIO21_S                                               8U
#define GPIO_DOUT23_20_DIO21_ONE                                    0x00000100U
#define GPIO_DOUT23_20_DIO21_ZERO                                   0x00000000U

// Field:     [0] DIO20
//
// Data output for DIO20
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT23_20_DIO20                                        0x00000001U
#define GPIO_DOUT23_20_DIO20_M                                      0x00000001U
#define GPIO_DOUT23_20_DIO20_S                                               0U
#define GPIO_DOUT23_20_DIO20_ONE                                    0x00000001U
#define GPIO_DOUT23_20_DIO20_ZERO                                   0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUT27_24
//
//*****************************************************************************
// Field:     [0] DIO24
//
// Data output for DIO24
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT27_24_DIO24                                        0x00000001U
#define GPIO_DOUT27_24_DIO24_M                                      0x00000001U
#define GPIO_DOUT27_24_DIO24_S                                               0U
#define GPIO_DOUT27_24_DIO24_ONE                                    0x00000001U
#define GPIO_DOUT27_24_DIO24_ZERO                                   0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUT31_0
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Data output for DIO24
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO24                                         0x01000000U
#define GPIO_DOUT31_0_DIO24_M                                       0x01000000U
#define GPIO_DOUT31_0_DIO24_S                                               24U
#define GPIO_DOUT31_0_DIO24_ONE                                     0x01000000U
#define GPIO_DOUT31_0_DIO24_ZERO                                    0x00000000U

// Field:    [21] DIO21
//
// Data output for DIO21
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO21                                         0x00200000U
#define GPIO_DOUT31_0_DIO21_M                                       0x00200000U
#define GPIO_DOUT31_0_DIO21_S                                               21U
#define GPIO_DOUT31_0_DIO21_ONE                                     0x00200000U
#define GPIO_DOUT31_0_DIO21_ZERO                                    0x00000000U

// Field:    [20] DIO20
//
// Data output for DIO20
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO20                                         0x00100000U
#define GPIO_DOUT31_0_DIO20_M                                       0x00100000U
#define GPIO_DOUT31_0_DIO20_S                                               20U
#define GPIO_DOUT31_0_DIO20_ONE                                     0x00100000U
#define GPIO_DOUT31_0_DIO20_ZERO                                    0x00000000U

// Field:    [17] DIO17
//
// Data output for DIO17
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO17                                         0x00020000U
#define GPIO_DOUT31_0_DIO17_M                                       0x00020000U
#define GPIO_DOUT31_0_DIO17_S                                               17U
#define GPIO_DOUT31_0_DIO17_ONE                                     0x00020000U
#define GPIO_DOUT31_0_DIO17_ZERO                                    0x00000000U

// Field:    [16] DIO16
//
// Data output for DIO16
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO16                                         0x00010000U
#define GPIO_DOUT31_0_DIO16_M                                       0x00010000U
#define GPIO_DOUT31_0_DIO16_S                                               16U
#define GPIO_DOUT31_0_DIO16_ONE                                     0x00010000U
#define GPIO_DOUT31_0_DIO16_ZERO                                    0x00000000U

// Field:    [13] DIO13
//
// Data output for DIO13
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO13                                         0x00002000U
#define GPIO_DOUT31_0_DIO13_M                                       0x00002000U
#define GPIO_DOUT31_0_DIO13_S                                               13U
#define GPIO_DOUT31_0_DIO13_ONE                                     0x00002000U
#define GPIO_DOUT31_0_DIO13_ZERO                                    0x00000000U

// Field:    [12] DIO12
//
// Data output for DIO12
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO12                                         0x00001000U
#define GPIO_DOUT31_0_DIO12_M                                       0x00001000U
#define GPIO_DOUT31_0_DIO12_S                                               12U
#define GPIO_DOUT31_0_DIO12_ONE                                     0x00001000U
#define GPIO_DOUT31_0_DIO12_ZERO                                    0x00000000U

// Field:    [11] DIO11
//
// Data output for DIO11
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO11                                         0x00000800U
#define GPIO_DOUT31_0_DIO11_M                                       0x00000800U
#define GPIO_DOUT31_0_DIO11_S                                               11U
#define GPIO_DOUT31_0_DIO11_ONE                                     0x00000800U
#define GPIO_DOUT31_0_DIO11_ZERO                                    0x00000000U

// Field:     [8] DIO8
//
// Data output for DIO8
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO8                                          0x00000100U
#define GPIO_DOUT31_0_DIO8_M                                        0x00000100U
#define GPIO_DOUT31_0_DIO8_S                                                 8U
#define GPIO_DOUT31_0_DIO8_ONE                                      0x00000100U
#define GPIO_DOUT31_0_DIO8_ZERO                                     0x00000000U

// Field:     [6] DIO6
//
// Data output for DIO6
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO6                                          0x00000040U
#define GPIO_DOUT31_0_DIO6_M                                        0x00000040U
#define GPIO_DOUT31_0_DIO6_S                                                 6U
#define GPIO_DOUT31_0_DIO6_ONE                                      0x00000040U
#define GPIO_DOUT31_0_DIO6_ZERO                                     0x00000000U

// Field:     [4] DIO4
//
// Data output for DIO4
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO4                                          0x00000010U
#define GPIO_DOUT31_0_DIO4_M                                        0x00000010U
#define GPIO_DOUT31_0_DIO4_S                                                 4U
#define GPIO_DOUT31_0_DIO4_ONE                                      0x00000010U
#define GPIO_DOUT31_0_DIO4_ZERO                                     0x00000000U

// Field:     [3] DIO3
//
// Data output for DIO3
// ENUMs:
// ONE                      Output is set to 1
// ZERO                     Output is set to 0
#define GPIO_DOUT31_0_DIO3                                          0x00000008U
#define GPIO_DOUT31_0_DIO3_M                                        0x00000008U
#define GPIO_DOUT31_0_DIO3_S                                                 3U
#define GPIO_DOUT31_0_DIO3_ONE                                      0x00000008U
#define GPIO_DOUT31_0_DIO3_ZERO                                     0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUTSET31_0
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Set bit DIO24 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO24                                      0x01000000U
#define GPIO_DOUTSET31_0_DIO24_M                                    0x01000000U
#define GPIO_DOUTSET31_0_DIO24_S                                            24U
#define GPIO_DOUTSET31_0_DIO24_SET                                  0x01000000U
#define GPIO_DOUTSET31_0_DIO24_NO_EFFECT                            0x00000000U

// Field:    [21] DIO21
//
// Set bit DIO21 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO21                                      0x00200000U
#define GPIO_DOUTSET31_0_DIO21_M                                    0x00200000U
#define GPIO_DOUTSET31_0_DIO21_S                                            21U
#define GPIO_DOUTSET31_0_DIO21_SET                                  0x00200000U
#define GPIO_DOUTSET31_0_DIO21_NO_EFFECT                            0x00000000U

// Field:    [20] DIO20
//
// Set bit DIO20 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO20                                      0x00100000U
#define GPIO_DOUTSET31_0_DIO20_M                                    0x00100000U
#define GPIO_DOUTSET31_0_DIO20_S                                            20U
#define GPIO_DOUTSET31_0_DIO20_SET                                  0x00100000U
#define GPIO_DOUTSET31_0_DIO20_NO_EFFECT                            0x00000000U

// Field:    [17] DIO17
//
// Set bit DIO17 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO17                                      0x00020000U
#define GPIO_DOUTSET31_0_DIO17_M                                    0x00020000U
#define GPIO_DOUTSET31_0_DIO17_S                                            17U
#define GPIO_DOUTSET31_0_DIO17_SET                                  0x00020000U
#define GPIO_DOUTSET31_0_DIO17_NO_EFFECT                            0x00000000U

// Field:    [16] DIO16
//
// Set bit DIO16 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO16                                      0x00010000U
#define GPIO_DOUTSET31_0_DIO16_M                                    0x00010000U
#define GPIO_DOUTSET31_0_DIO16_S                                            16U
#define GPIO_DOUTSET31_0_DIO16_SET                                  0x00010000U
#define GPIO_DOUTSET31_0_DIO16_NO_EFFECT                            0x00000000U

// Field:    [13] DIO13
//
// Set bit DIO13 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO13                                      0x00002000U
#define GPIO_DOUTSET31_0_DIO13_M                                    0x00002000U
#define GPIO_DOUTSET31_0_DIO13_S                                            13U
#define GPIO_DOUTSET31_0_DIO13_SET                                  0x00002000U
#define GPIO_DOUTSET31_0_DIO13_NO_EFFECT                            0x00000000U

// Field:    [12] DIO12
//
// Set bit DIO12 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO12                                      0x00001000U
#define GPIO_DOUTSET31_0_DIO12_M                                    0x00001000U
#define GPIO_DOUTSET31_0_DIO12_S                                            12U
#define GPIO_DOUTSET31_0_DIO12_SET                                  0x00001000U
#define GPIO_DOUTSET31_0_DIO12_NO_EFFECT                            0x00000000U

// Field:    [11] DIO11
//
// Set bit DIO11 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO11                                      0x00000800U
#define GPIO_DOUTSET31_0_DIO11_M                                    0x00000800U
#define GPIO_DOUTSET31_0_DIO11_S                                            11U
#define GPIO_DOUTSET31_0_DIO11_SET                                  0x00000800U
#define GPIO_DOUTSET31_0_DIO11_NO_EFFECT                            0x00000000U

// Field:     [8] DIO8
//
// Set bit DIO8 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO8                                       0x00000100U
#define GPIO_DOUTSET31_0_DIO8_M                                     0x00000100U
#define GPIO_DOUTSET31_0_DIO8_S                                              8U
#define GPIO_DOUTSET31_0_DIO8_SET                                   0x00000100U
#define GPIO_DOUTSET31_0_DIO8_NO_EFFECT                             0x00000000U

// Field:     [6] DIO6
//
// Set bit DIO6 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO6                                       0x00000040U
#define GPIO_DOUTSET31_0_DIO6_M                                     0x00000040U
#define GPIO_DOUTSET31_0_DIO6_S                                              6U
#define GPIO_DOUTSET31_0_DIO6_SET                                   0x00000040U
#define GPIO_DOUTSET31_0_DIO6_NO_EFFECT                             0x00000000U

// Field:     [4] DIO4
//
// Set bit DIO4 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO4                                       0x00000010U
#define GPIO_DOUTSET31_0_DIO4_M                                     0x00000010U
#define GPIO_DOUTSET31_0_DIO4_S                                              4U
#define GPIO_DOUTSET31_0_DIO4_SET                                   0x00000010U
#define GPIO_DOUTSET31_0_DIO4_NO_EFFECT                             0x00000000U

// Field:     [3] DIO3
//
// Set bit DIO3 in DOUT31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOUTSET31_0_DIO3                                       0x00000008U
#define GPIO_DOUTSET31_0_DIO3_M                                     0x00000008U
#define GPIO_DOUTSET31_0_DIO3_S                                              3U
#define GPIO_DOUTSET31_0_DIO3_SET                                   0x00000008U
#define GPIO_DOUTSET31_0_DIO3_NO_EFFECT                             0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUTCLR31_0
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Clear bit DIO24 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO24                                      0x01000000U
#define GPIO_DOUTCLR31_0_DIO24_M                                    0x01000000U
#define GPIO_DOUTCLR31_0_DIO24_S                                            24U
#define GPIO_DOUTCLR31_0_DIO24_CLR                                  0x01000000U
#define GPIO_DOUTCLR31_0_DIO24_NO_EFFECT                            0x00000000U

// Field:    [21] DIO21
//
// Clear bit DIO21 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO21                                      0x00200000U
#define GPIO_DOUTCLR31_0_DIO21_M                                    0x00200000U
#define GPIO_DOUTCLR31_0_DIO21_S                                            21U
#define GPIO_DOUTCLR31_0_DIO21_CLR                                  0x00200000U
#define GPIO_DOUTCLR31_0_DIO21_NO_EFFECT                            0x00000000U

// Field:    [20] DIO20
//
// Clear bit DIO20 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO20                                      0x00100000U
#define GPIO_DOUTCLR31_0_DIO20_M                                    0x00100000U
#define GPIO_DOUTCLR31_0_DIO20_S                                            20U
#define GPIO_DOUTCLR31_0_DIO20_CLR                                  0x00100000U
#define GPIO_DOUTCLR31_0_DIO20_NO_EFFECT                            0x00000000U

// Field:    [17] DIO17
//
// Clear bit DIO17 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO17                                      0x00020000U
#define GPIO_DOUTCLR31_0_DIO17_M                                    0x00020000U
#define GPIO_DOUTCLR31_0_DIO17_S                                            17U
#define GPIO_DOUTCLR31_0_DIO17_CLR                                  0x00020000U
#define GPIO_DOUTCLR31_0_DIO17_NO_EFFECT                            0x00000000U

// Field:    [16] DIO16
//
// Clear bit DIO16 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO16                                      0x00010000U
#define GPIO_DOUTCLR31_0_DIO16_M                                    0x00010000U
#define GPIO_DOUTCLR31_0_DIO16_S                                            16U
#define GPIO_DOUTCLR31_0_DIO16_CLR                                  0x00010000U
#define GPIO_DOUTCLR31_0_DIO16_NO_EFFECT                            0x00000000U

// Field:    [13] DIO13
//
// Clear bit DIO13 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO13                                      0x00002000U
#define GPIO_DOUTCLR31_0_DIO13_M                                    0x00002000U
#define GPIO_DOUTCLR31_0_DIO13_S                                            13U
#define GPIO_DOUTCLR31_0_DIO13_CLR                                  0x00002000U
#define GPIO_DOUTCLR31_0_DIO13_NO_EFFECT                            0x00000000U

// Field:    [12] DIO12
//
// Clear bit DIO12 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO12                                      0x00001000U
#define GPIO_DOUTCLR31_0_DIO12_M                                    0x00001000U
#define GPIO_DOUTCLR31_0_DIO12_S                                            12U
#define GPIO_DOUTCLR31_0_DIO12_CLR                                  0x00001000U
#define GPIO_DOUTCLR31_0_DIO12_NO_EFFECT                            0x00000000U

// Field:    [11] DIO11
//
// Clear bit DIO11 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO11                                      0x00000800U
#define GPIO_DOUTCLR31_0_DIO11_M                                    0x00000800U
#define GPIO_DOUTCLR31_0_DIO11_S                                            11U
#define GPIO_DOUTCLR31_0_DIO11_CLR                                  0x00000800U
#define GPIO_DOUTCLR31_0_DIO11_NO_EFFECT                            0x00000000U

// Field:     [8] DIO8
//
// Clear bit DIO8 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO8                                       0x00000100U
#define GPIO_DOUTCLR31_0_DIO8_M                                     0x00000100U
#define GPIO_DOUTCLR31_0_DIO8_S                                              8U
#define GPIO_DOUTCLR31_0_DIO8_CLR                                   0x00000100U
#define GPIO_DOUTCLR31_0_DIO8_NO_EFFECT                             0x00000000U

// Field:     [6] DIO6
//
// Clear bit DIO6 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO6                                       0x00000040U
#define GPIO_DOUTCLR31_0_DIO6_M                                     0x00000040U
#define GPIO_DOUTCLR31_0_DIO6_S                                              6U
#define GPIO_DOUTCLR31_0_DIO6_CLR                                   0x00000040U
#define GPIO_DOUTCLR31_0_DIO6_NO_EFFECT                             0x00000000U

// Field:     [4] DIO4
//
// Clear bit DIO4 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO4                                       0x00000010U
#define GPIO_DOUTCLR31_0_DIO4_M                                     0x00000010U
#define GPIO_DOUTCLR31_0_DIO4_S                                              4U
#define GPIO_DOUTCLR31_0_DIO4_CLR                                   0x00000010U
#define GPIO_DOUTCLR31_0_DIO4_NO_EFFECT                             0x00000000U

// Field:     [3] DIO3
//
// Clear bit DIO3 in DOUT31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOUTCLR31_0_DIO3                                       0x00000008U
#define GPIO_DOUTCLR31_0_DIO3_M                                     0x00000008U
#define GPIO_DOUTCLR31_0_DIO3_S                                              3U
#define GPIO_DOUTCLR31_0_DIO3_CLR                                   0x00000008U
#define GPIO_DOUTCLR31_0_DIO3_NO_EFFECT                             0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUTTGL31_0
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Toggles bit DIO24 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO24                                      0x01000000U
#define GPIO_DOUTTGL31_0_DIO24_M                                    0x01000000U
#define GPIO_DOUTTGL31_0_DIO24_S                                            24U
#define GPIO_DOUTTGL31_0_DIO24_TOGGLE                               0x01000000U
#define GPIO_DOUTTGL31_0_DIO24_NO_EFFECT                            0x00000000U

// Field:    [21] DIO21
//
// Toggles bit DIO21 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO21                                      0x00200000U
#define GPIO_DOUTTGL31_0_DIO21_M                                    0x00200000U
#define GPIO_DOUTTGL31_0_DIO21_S                                            21U
#define GPIO_DOUTTGL31_0_DIO21_TOGGLE                               0x00200000U
#define GPIO_DOUTTGL31_0_DIO21_NO_EFFECT                            0x00000000U

// Field:    [20] DIO20
//
// Toggles bit DIO20 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO20                                      0x00100000U
#define GPIO_DOUTTGL31_0_DIO20_M                                    0x00100000U
#define GPIO_DOUTTGL31_0_DIO20_S                                            20U
#define GPIO_DOUTTGL31_0_DIO20_TOGGLE                               0x00100000U
#define GPIO_DOUTTGL31_0_DIO20_NO_EFFECT                            0x00000000U

// Field:    [17] DIO17
//
// Toggles bit DIO17 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO17                                      0x00020000U
#define GPIO_DOUTTGL31_0_DIO17_M                                    0x00020000U
#define GPIO_DOUTTGL31_0_DIO17_S                                            17U
#define GPIO_DOUTTGL31_0_DIO17_TOGGLE                               0x00020000U
#define GPIO_DOUTTGL31_0_DIO17_NO_EFFECT                            0x00000000U

// Field:    [16] DIO16
//
// Toggles bit DIO16 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO16                                      0x00010000U
#define GPIO_DOUTTGL31_0_DIO16_M                                    0x00010000U
#define GPIO_DOUTTGL31_0_DIO16_S                                            16U
#define GPIO_DOUTTGL31_0_DIO16_TOGGLE                               0x00010000U
#define GPIO_DOUTTGL31_0_DIO16_NO_EFFECT                            0x00000000U

// Field:    [13] DIO13
//
// Toggles bit DIO13 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO13                                      0x00002000U
#define GPIO_DOUTTGL31_0_DIO13_M                                    0x00002000U
#define GPIO_DOUTTGL31_0_DIO13_S                                            13U
#define GPIO_DOUTTGL31_0_DIO13_TOGGLE                               0x00002000U
#define GPIO_DOUTTGL31_0_DIO13_NO_EFFECT                            0x00000000U

// Field:    [12] DIO12
//
// Toggles bit DIO12 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO12                                      0x00001000U
#define GPIO_DOUTTGL31_0_DIO12_M                                    0x00001000U
#define GPIO_DOUTTGL31_0_DIO12_S                                            12U
#define GPIO_DOUTTGL31_0_DIO12_TOGGLE                               0x00001000U
#define GPIO_DOUTTGL31_0_DIO12_NO_EFFECT                            0x00000000U

// Field:    [11] DIO11
//
// Toggles bit DIO11 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO11                                      0x00000800U
#define GPIO_DOUTTGL31_0_DIO11_M                                    0x00000800U
#define GPIO_DOUTTGL31_0_DIO11_S                                            11U
#define GPIO_DOUTTGL31_0_DIO11_TOGGLE                               0x00000800U
#define GPIO_DOUTTGL31_0_DIO11_NO_EFFECT                            0x00000000U

// Field:     [8] DIO8
//
// Toggles bit DIO8 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO8                                       0x00000100U
#define GPIO_DOUTTGL31_0_DIO8_M                                     0x00000100U
#define GPIO_DOUTTGL31_0_DIO8_S                                              8U
#define GPIO_DOUTTGL31_0_DIO8_TOGGLE                                0x00000100U
#define GPIO_DOUTTGL31_0_DIO8_NO_EFFECT                             0x00000000U

// Field:     [6] DIO6
//
// Toggles bit DIO6 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO6                                       0x00000040U
#define GPIO_DOUTTGL31_0_DIO6_M                                     0x00000040U
#define GPIO_DOUTTGL31_0_DIO6_S                                              6U
#define GPIO_DOUTTGL31_0_DIO6_TOGGLE                                0x00000040U
#define GPIO_DOUTTGL31_0_DIO6_NO_EFFECT                             0x00000000U

// Field:     [4] DIO4
//
// Toggles bit DIO4 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO4                                       0x00000010U
#define GPIO_DOUTTGL31_0_DIO4_M                                     0x00000010U
#define GPIO_DOUTTGL31_0_DIO4_S                                              4U
#define GPIO_DOUTTGL31_0_DIO4_TOGGLE                                0x00000010U
#define GPIO_DOUTTGL31_0_DIO4_NO_EFFECT                             0x00000000U

// Field:     [3] DIO3
//
// Toggles bit DIO3 in DOUT31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL31_0_DIO3                                       0x00000008U
#define GPIO_DOUTTGL31_0_DIO3_M                                     0x00000008U
#define GPIO_DOUTTGL31_0_DIO3_S                                              3U
#define GPIO_DOUTTGL31_0_DIO3_TOGGLE                                0x00000008U
#define GPIO_DOUTTGL31_0_DIO3_NO_EFFECT                             0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUTTGL3_0
//
//*****************************************************************************
// Field:    [24] DIO3
//
// Data output toggle for DIO3
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL3_0_DIO3                                        0x01000000U
#define GPIO_DOUTTGL3_0_DIO3_M                                      0x01000000U
#define GPIO_DOUTTGL3_0_DIO3_S                                              24U
#define GPIO_DOUTTGL3_0_DIO3_TOGGLE                                 0x01000000U
#define GPIO_DOUTTGL3_0_DIO3_NO_EFFECT                              0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUTTGL7_4
//
//*****************************************************************************
// Field:    [16] DIO6
//
// Data output toggle for DIO6
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL7_4_DIO6                                        0x00010000U
#define GPIO_DOUTTGL7_4_DIO6_M                                      0x00010000U
#define GPIO_DOUTTGL7_4_DIO6_S                                              16U
#define GPIO_DOUTTGL7_4_DIO6_TOGGLE                                 0x00010000U
#define GPIO_DOUTTGL7_4_DIO6_NO_EFFECT                              0x00000000U

// Field:     [0] DIO4
//
// Data output toggle for DIO4
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL7_4_DIO4                                        0x00000001U
#define GPIO_DOUTTGL7_4_DIO4_M                                      0x00000001U
#define GPIO_DOUTTGL7_4_DIO4_S                                               0U
#define GPIO_DOUTTGL7_4_DIO4_TOGGLE                                 0x00000001U
#define GPIO_DOUTTGL7_4_DIO4_NO_EFFECT                              0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUTTGL11_8
//
//*****************************************************************************
// Field:    [24] DIO11
//
// Data output toggle for DIO11
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL11_8_DIO11                                      0x01000000U
#define GPIO_DOUTTGL11_8_DIO11_M                                    0x01000000U
#define GPIO_DOUTTGL11_8_DIO11_S                                            24U
#define GPIO_DOUTTGL11_8_DIO11_TOGGLE                               0x01000000U
#define GPIO_DOUTTGL11_8_DIO11_NO_EFFECT                            0x00000000U

// Field:     [0] DIO8
//
// Data output toggle for DIO8
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL11_8_DIO8                                       0x00000001U
#define GPIO_DOUTTGL11_8_DIO8_M                                     0x00000001U
#define GPIO_DOUTTGL11_8_DIO8_S                                              0U
#define GPIO_DOUTTGL11_8_DIO8_TOGGLE                                0x00000001U
#define GPIO_DOUTTGL11_8_DIO8_NO_EFFECT                             0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUTTGL15_12
//
//*****************************************************************************
// Field:     [8] DIO13
//
// Data output toggle for DIO13
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL15_12_DIO13                                     0x00000100U
#define GPIO_DOUTTGL15_12_DIO13_M                                   0x00000100U
#define GPIO_DOUTTGL15_12_DIO13_S                                            8U
#define GPIO_DOUTTGL15_12_DIO13_TOGGLE                              0x00000100U
#define GPIO_DOUTTGL15_12_DIO13_NO_EFFECT                           0x00000000U

// Field:     [0] DIO12
//
// Data output toggle for DIO12
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL15_12_DIO12                                     0x00000001U
#define GPIO_DOUTTGL15_12_DIO12_M                                   0x00000001U
#define GPIO_DOUTTGL15_12_DIO12_S                                            0U
#define GPIO_DOUTTGL15_12_DIO12_TOGGLE                              0x00000001U
#define GPIO_DOUTTGL15_12_DIO12_NO_EFFECT                           0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUTTGL19_16
//
//*****************************************************************************
// Field:     [8] DIO17
//
// Data output toggle for DIO17
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL19_16_DIO17                                     0x00000100U
#define GPIO_DOUTTGL19_16_DIO17_M                                   0x00000100U
#define GPIO_DOUTTGL19_16_DIO17_S                                            8U
#define GPIO_DOUTTGL19_16_DIO17_TOGGLE                              0x00000100U
#define GPIO_DOUTTGL19_16_DIO17_NO_EFFECT                           0x00000000U

// Field:     [0] DIO16
//
// Data output toggle for DIO16
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL19_16_DIO16                                     0x00000001U
#define GPIO_DOUTTGL19_16_DIO16_M                                   0x00000001U
#define GPIO_DOUTTGL19_16_DIO16_S                                            0U
#define GPIO_DOUTTGL19_16_DIO16_TOGGLE                              0x00000001U
#define GPIO_DOUTTGL19_16_DIO16_NO_EFFECT                           0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUTTGL23_20
//
//*****************************************************************************
// Field:     [8] DIO21
//
// Data output toggle for DIO21
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL23_20_DIO21                                     0x00000100U
#define GPIO_DOUTTGL23_20_DIO21_M                                   0x00000100U
#define GPIO_DOUTTGL23_20_DIO21_S                                            8U
#define GPIO_DOUTTGL23_20_DIO21_TOGGLE                              0x00000100U
#define GPIO_DOUTTGL23_20_DIO21_NO_EFFECT                           0x00000000U

// Field:     [0] DIO20
//
// Data output toggle for DIO20
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL23_20_DIO20                                     0x00000001U
#define GPIO_DOUTTGL23_20_DIO20_M                                   0x00000001U
#define GPIO_DOUTTGL23_20_DIO20_S                                            0U
#define GPIO_DOUTTGL23_20_DIO20_TOGGLE                              0x00000001U
#define GPIO_DOUTTGL23_20_DIO20_NO_EFFECT                           0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOUTTGL27_24
//
//*****************************************************************************
// Field:     [0] DIO24
//
// Data output toggle for DIO24
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOUTTGL27_24_DIO24                                     0x00000001U
#define GPIO_DOUTTGL27_24_DIO24_M                                   0x00000001U
#define GPIO_DOUTTGL27_24_DIO24_S                                            0U
#define GPIO_DOUTTGL27_24_DIO24_TOGGLE                              0x00000001U
#define GPIO_DOUTTGL27_24_DIO24_NO_EFFECT                           0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOE3_0
//
//*****************************************************************************
// Field:    [24] DIO3
//
// Data output enable for DIO3
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE3_0_DIO3                                            0x01000000U
#define GPIO_DOE3_0_DIO3_M                                          0x01000000U
#define GPIO_DOE3_0_DIO3_S                                                  24U
#define GPIO_DOE3_0_DIO3_ENABLE                                     0x01000000U
#define GPIO_DOE3_0_DIO3_DISABLE                                    0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOE7_4
//
//*****************************************************************************
// Field:    [16] DIO6
//
// Data output enable for DIO6
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE7_4_DIO6                                            0x00010000U
#define GPIO_DOE7_4_DIO6_M                                          0x00010000U
#define GPIO_DOE7_4_DIO6_S                                                  16U
#define GPIO_DOE7_4_DIO6_ENABLE                                     0x00010000U
#define GPIO_DOE7_4_DIO6_DISABLE                                    0x00000000U

// Field:     [0] DIO4
//
// Data output enable for DIO4
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE7_4_DIO4                                            0x00000001U
#define GPIO_DOE7_4_DIO4_M                                          0x00000001U
#define GPIO_DOE7_4_DIO4_S                                                   0U
#define GPIO_DOE7_4_DIO4_ENABLE                                     0x00000001U
#define GPIO_DOE7_4_DIO4_DISABLE                                    0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOE11_8
//
//*****************************************************************************
// Field:    [24] DIO11
//
// Data output enable for DIO11
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE11_8_DIO11                                          0x01000000U
#define GPIO_DOE11_8_DIO11_M                                        0x01000000U
#define GPIO_DOE11_8_DIO11_S                                                24U
#define GPIO_DOE11_8_DIO11_ENABLE                                   0x01000000U
#define GPIO_DOE11_8_DIO11_DISABLE                                  0x00000000U

// Field:     [0] DIO8
//
// Data output enable for DIO8
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE11_8_DIO8                                           0x00000001U
#define GPIO_DOE11_8_DIO8_M                                         0x00000001U
#define GPIO_DOE11_8_DIO8_S                                                  0U
#define GPIO_DOE11_8_DIO8_ENABLE                                    0x00000001U
#define GPIO_DOE11_8_DIO8_DISABLE                                   0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOE15_12
//
//*****************************************************************************
// Field:     [8] DIO13
//
// Data output enable for DIO13
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE15_12_DIO13                                         0x00000100U
#define GPIO_DOE15_12_DIO13_M                                       0x00000100U
#define GPIO_DOE15_12_DIO13_S                                                8U
#define GPIO_DOE15_12_DIO13_ENABLE                                  0x00000100U
#define GPIO_DOE15_12_DIO13_DISABLE                                 0x00000000U

// Field:     [0] DIO12
//
// Data output enable for DIO12
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE15_12_DIO12                                         0x00000001U
#define GPIO_DOE15_12_DIO12_M                                       0x00000001U
#define GPIO_DOE15_12_DIO12_S                                                0U
#define GPIO_DOE15_12_DIO12_ENABLE                                  0x00000001U
#define GPIO_DOE15_12_DIO12_DISABLE                                 0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOE19_16
//
//*****************************************************************************
// Field:     [8] DIO17
//
// Data output enable for DIO17
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE19_16_DIO17                                         0x00000100U
#define GPIO_DOE19_16_DIO17_M                                       0x00000100U
#define GPIO_DOE19_16_DIO17_S                                                8U
#define GPIO_DOE19_16_DIO17_ENABLE                                  0x00000100U
#define GPIO_DOE19_16_DIO17_DISABLE                                 0x00000000U

// Field:     [0] DIO16
//
// Data output enable for DIO16
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE19_16_DIO16                                         0x00000001U
#define GPIO_DOE19_16_DIO16_M                                       0x00000001U
#define GPIO_DOE19_16_DIO16_S                                                0U
#define GPIO_DOE19_16_DIO16_ENABLE                                  0x00000001U
#define GPIO_DOE19_16_DIO16_DISABLE                                 0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOE23_20
//
//*****************************************************************************
// Field:     [8] DIO21
//
// Data output enable for DIO21
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE23_20_DIO21                                         0x00000100U
#define GPIO_DOE23_20_DIO21_M                                       0x00000100U
#define GPIO_DOE23_20_DIO21_S                                                8U
#define GPIO_DOE23_20_DIO21_ENABLE                                  0x00000100U
#define GPIO_DOE23_20_DIO21_DISABLE                                 0x00000000U

// Field:     [0] DIO20
//
// Data output enable for DIO20
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE23_20_DIO20                                         0x00000001U
#define GPIO_DOE23_20_DIO20_M                                       0x00000001U
#define GPIO_DOE23_20_DIO20_S                                                0U
#define GPIO_DOE23_20_DIO20_ENABLE                                  0x00000001U
#define GPIO_DOE23_20_DIO20_DISABLE                                 0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOE27_24
//
//*****************************************************************************
// Field:     [0] DIO24
//
// Data output enable for DIO24
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE27_24_DIO24                                         0x00000001U
#define GPIO_DOE27_24_DIO24_M                                       0x00000001U
#define GPIO_DOE27_24_DIO24_S                                                0U
#define GPIO_DOE27_24_DIO24_ENABLE                                  0x00000001U
#define GPIO_DOE27_24_DIO24_DISABLE                                 0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOE31_0
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Data output enable for DIO24
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO24                                          0x01000000U
#define GPIO_DOE31_0_DIO24_M                                        0x01000000U
#define GPIO_DOE31_0_DIO24_S                                                24U
#define GPIO_DOE31_0_DIO24_ENABLE                                   0x01000000U
#define GPIO_DOE31_0_DIO24_DISABLE                                  0x00000000U

// Field:    [21] DIO21
//
// Data output enable for DIO21
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO21                                          0x00200000U
#define GPIO_DOE31_0_DIO21_M                                        0x00200000U
#define GPIO_DOE31_0_DIO21_S                                                21U
#define GPIO_DOE31_0_DIO21_ENABLE                                   0x00200000U
#define GPIO_DOE31_0_DIO21_DISABLE                                  0x00000000U

// Field:    [20] DIO20
//
// Data output enable for DIO20
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO20                                          0x00100000U
#define GPIO_DOE31_0_DIO20_M                                        0x00100000U
#define GPIO_DOE31_0_DIO20_S                                                20U
#define GPIO_DOE31_0_DIO20_ENABLE                                   0x00100000U
#define GPIO_DOE31_0_DIO20_DISABLE                                  0x00000000U

// Field:    [17] DIO17
//
// Data output enable for DIO17
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO17                                          0x00020000U
#define GPIO_DOE31_0_DIO17_M                                        0x00020000U
#define GPIO_DOE31_0_DIO17_S                                                17U
#define GPIO_DOE31_0_DIO17_ENABLE                                   0x00020000U
#define GPIO_DOE31_0_DIO17_DISABLE                                  0x00000000U

// Field:    [16] DIO16
//
// Data output enable for DIO16
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO16                                          0x00010000U
#define GPIO_DOE31_0_DIO16_M                                        0x00010000U
#define GPIO_DOE31_0_DIO16_S                                                16U
#define GPIO_DOE31_0_DIO16_ENABLE                                   0x00010000U
#define GPIO_DOE31_0_DIO16_DISABLE                                  0x00000000U

// Field:    [13] DIO13
//
// Data output enable for DIO13
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO13                                          0x00002000U
#define GPIO_DOE31_0_DIO13_M                                        0x00002000U
#define GPIO_DOE31_0_DIO13_S                                                13U
#define GPIO_DOE31_0_DIO13_ENABLE                                   0x00002000U
#define GPIO_DOE31_0_DIO13_DISABLE                                  0x00000000U

// Field:    [12] DIO12
//
// Data output enable for DIO12
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO12                                          0x00001000U
#define GPIO_DOE31_0_DIO12_M                                        0x00001000U
#define GPIO_DOE31_0_DIO12_S                                                12U
#define GPIO_DOE31_0_DIO12_ENABLE                                   0x00001000U
#define GPIO_DOE31_0_DIO12_DISABLE                                  0x00000000U

// Field:    [11] DIO11
//
// Data output enable for DIO11
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO11                                          0x00000800U
#define GPIO_DOE31_0_DIO11_M                                        0x00000800U
#define GPIO_DOE31_0_DIO11_S                                                11U
#define GPIO_DOE31_0_DIO11_ENABLE                                   0x00000800U
#define GPIO_DOE31_0_DIO11_DISABLE                                  0x00000000U

// Field:     [8] DIO8
//
// Data output enable for DIO8
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO8                                           0x00000100U
#define GPIO_DOE31_0_DIO8_M                                         0x00000100U
#define GPIO_DOE31_0_DIO8_S                                                  8U
#define GPIO_DOE31_0_DIO8_ENABLE                                    0x00000100U
#define GPIO_DOE31_0_DIO8_DISABLE                                   0x00000000U

// Field:     [6] DIO6
//
// Data output enable for DIO6
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO6                                           0x00000040U
#define GPIO_DOE31_0_DIO6_M                                         0x00000040U
#define GPIO_DOE31_0_DIO6_S                                                  6U
#define GPIO_DOE31_0_DIO6_ENABLE                                    0x00000040U
#define GPIO_DOE31_0_DIO6_DISABLE                                   0x00000000U

// Field:     [4] DIO4
//
// Data output enable for DIO4
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO4                                           0x00000010U
#define GPIO_DOE31_0_DIO4_M                                         0x00000010U
#define GPIO_DOE31_0_DIO4_S                                                  4U
#define GPIO_DOE31_0_DIO4_ENABLE                                    0x00000010U
#define GPIO_DOE31_0_DIO4_DISABLE                                   0x00000000U

// Field:     [3] DIO3
//
// Data output enable for DIO3
// ENUMs:
// ENABLE                   Output enabled
// DISABLE                  Output disabled
#define GPIO_DOE31_0_DIO3                                           0x00000008U
#define GPIO_DOE31_0_DIO3_M                                         0x00000008U
#define GPIO_DOE31_0_DIO3_S                                                  3U
#define GPIO_DOE31_0_DIO3_ENABLE                                    0x00000008U
#define GPIO_DOE31_0_DIO3_DISABLE                                   0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOESET31_0
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Set bit DIO24 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO24                                       0x01000000U
#define GPIO_DOESET31_0_DIO24_M                                     0x01000000U
#define GPIO_DOESET31_0_DIO24_S                                             24U
#define GPIO_DOESET31_0_DIO24_SET                                   0x01000000U
#define GPIO_DOESET31_0_DIO24_NO_EFFECT                             0x00000000U

// Field:    [21] DIO21
//
// Set bit DIO21 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO21                                       0x00200000U
#define GPIO_DOESET31_0_DIO21_M                                     0x00200000U
#define GPIO_DOESET31_0_DIO21_S                                             21U
#define GPIO_DOESET31_0_DIO21_SET                                   0x00200000U
#define GPIO_DOESET31_0_DIO21_NO_EFFECT                             0x00000000U

// Field:    [20] DIO20
//
// Set bit DIO20 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO20                                       0x00100000U
#define GPIO_DOESET31_0_DIO20_M                                     0x00100000U
#define GPIO_DOESET31_0_DIO20_S                                             20U
#define GPIO_DOESET31_0_DIO20_SET                                   0x00100000U
#define GPIO_DOESET31_0_DIO20_NO_EFFECT                             0x00000000U

// Field:    [17] DIO17
//
// Set bit DIO17 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO17                                       0x00020000U
#define GPIO_DOESET31_0_DIO17_M                                     0x00020000U
#define GPIO_DOESET31_0_DIO17_S                                             17U
#define GPIO_DOESET31_0_DIO17_SET                                   0x00020000U
#define GPIO_DOESET31_0_DIO17_NO_EFFECT                             0x00000000U

// Field:    [16] DIO16
//
// Set bit DIO16 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO16                                       0x00010000U
#define GPIO_DOESET31_0_DIO16_M                                     0x00010000U
#define GPIO_DOESET31_0_DIO16_S                                             16U
#define GPIO_DOESET31_0_DIO16_SET                                   0x00010000U
#define GPIO_DOESET31_0_DIO16_NO_EFFECT                             0x00000000U

// Field:    [13] DIO13
//
// Set bit DIO13 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO13                                       0x00002000U
#define GPIO_DOESET31_0_DIO13_M                                     0x00002000U
#define GPIO_DOESET31_0_DIO13_S                                             13U
#define GPIO_DOESET31_0_DIO13_SET                                   0x00002000U
#define GPIO_DOESET31_0_DIO13_NO_EFFECT                             0x00000000U

// Field:    [12] DIO12
//
// Set bit DIO12 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO12                                       0x00001000U
#define GPIO_DOESET31_0_DIO12_M                                     0x00001000U
#define GPIO_DOESET31_0_DIO12_S                                             12U
#define GPIO_DOESET31_0_DIO12_SET                                   0x00001000U
#define GPIO_DOESET31_0_DIO12_NO_EFFECT                             0x00000000U

// Field:    [11] DIO11
//
// Set bit DIO11 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO11                                       0x00000800U
#define GPIO_DOESET31_0_DIO11_M                                     0x00000800U
#define GPIO_DOESET31_0_DIO11_S                                             11U
#define GPIO_DOESET31_0_DIO11_SET                                   0x00000800U
#define GPIO_DOESET31_0_DIO11_NO_EFFECT                             0x00000000U

// Field:     [8] DIO8
//
// Set bit DIO8 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO8                                        0x00000100U
#define GPIO_DOESET31_0_DIO8_M                                      0x00000100U
#define GPIO_DOESET31_0_DIO8_S                                               8U
#define GPIO_DOESET31_0_DIO8_SET                                    0x00000100U
#define GPIO_DOESET31_0_DIO8_NO_EFFECT                              0x00000000U

// Field:     [6] DIO6
//
// Set bit DIO6 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO6                                        0x00000040U
#define GPIO_DOESET31_0_DIO6_M                                      0x00000040U
#define GPIO_DOESET31_0_DIO6_S                                               6U
#define GPIO_DOESET31_0_DIO6_SET                                    0x00000040U
#define GPIO_DOESET31_0_DIO6_NO_EFFECT                              0x00000000U

// Field:     [4] DIO4
//
// Set bit DIO4 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO4                                        0x00000010U
#define GPIO_DOESET31_0_DIO4_M                                      0x00000010U
#define GPIO_DOESET31_0_DIO4_S                                               4U
#define GPIO_DOESET31_0_DIO4_SET                                    0x00000010U
#define GPIO_DOESET31_0_DIO4_NO_EFFECT                              0x00000000U

// Field:     [3] DIO3
//
// Set bit DIO3 in DOE31_0
// ENUMs:
// SET
// NO_EFFECT                No effect
#define GPIO_DOESET31_0_DIO3                                        0x00000008U
#define GPIO_DOESET31_0_DIO3_M                                      0x00000008U
#define GPIO_DOESET31_0_DIO3_S                                               3U
#define GPIO_DOESET31_0_DIO3_SET                                    0x00000008U
#define GPIO_DOESET31_0_DIO3_NO_EFFECT                              0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOECLR31_0
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Clears bit DIO24 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO24                                       0x01000000U
#define GPIO_DOECLR31_0_DIO24_M                                     0x01000000U
#define GPIO_DOECLR31_0_DIO24_S                                             24U
#define GPIO_DOECLR31_0_DIO24_CLR                                   0x01000000U
#define GPIO_DOECLR31_0_DIO24_NO_EFFECT                             0x00000000U

// Field:    [21] DIO21
//
// Clears bit DIO21 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO21                                       0x00200000U
#define GPIO_DOECLR31_0_DIO21_M                                     0x00200000U
#define GPIO_DOECLR31_0_DIO21_S                                             21U
#define GPIO_DOECLR31_0_DIO21_CLR                                   0x00200000U
#define GPIO_DOECLR31_0_DIO21_NO_EFFECT                             0x00000000U

// Field:    [20] DIO20
//
// Clears bit DIO20 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO20                                       0x00100000U
#define GPIO_DOECLR31_0_DIO20_M                                     0x00100000U
#define GPIO_DOECLR31_0_DIO20_S                                             20U
#define GPIO_DOECLR31_0_DIO20_CLR                                   0x00100000U
#define GPIO_DOECLR31_0_DIO20_NO_EFFECT                             0x00000000U

// Field:    [17] DIO17
//
// Clears bit DIO17 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO17                                       0x00020000U
#define GPIO_DOECLR31_0_DIO17_M                                     0x00020000U
#define GPIO_DOECLR31_0_DIO17_S                                             17U
#define GPIO_DOECLR31_0_DIO17_CLR                                   0x00020000U
#define GPIO_DOECLR31_0_DIO17_NO_EFFECT                             0x00000000U

// Field:    [16] DIO16
//
// Clears bit DIO16 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO16                                       0x00010000U
#define GPIO_DOECLR31_0_DIO16_M                                     0x00010000U
#define GPIO_DOECLR31_0_DIO16_S                                             16U
#define GPIO_DOECLR31_0_DIO16_CLR                                   0x00010000U
#define GPIO_DOECLR31_0_DIO16_NO_EFFECT                             0x00000000U

// Field:    [13] DIO13
//
// Clears bit DIO13 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO13                                       0x00002000U
#define GPIO_DOECLR31_0_DIO13_M                                     0x00002000U
#define GPIO_DOECLR31_0_DIO13_S                                             13U
#define GPIO_DOECLR31_0_DIO13_CLR                                   0x00002000U
#define GPIO_DOECLR31_0_DIO13_NO_EFFECT                             0x00000000U

// Field:    [12] DIO12
//
// Clears bit DIO12 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO12                                       0x00001000U
#define GPIO_DOECLR31_0_DIO12_M                                     0x00001000U
#define GPIO_DOECLR31_0_DIO12_S                                             12U
#define GPIO_DOECLR31_0_DIO12_CLR                                   0x00001000U
#define GPIO_DOECLR31_0_DIO12_NO_EFFECT                             0x00000000U

// Field:    [11] DIO11
//
// Clears bit DIO11 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO11                                       0x00000800U
#define GPIO_DOECLR31_0_DIO11_M                                     0x00000800U
#define GPIO_DOECLR31_0_DIO11_S                                             11U
#define GPIO_DOECLR31_0_DIO11_CLR                                   0x00000800U
#define GPIO_DOECLR31_0_DIO11_NO_EFFECT                             0x00000000U

// Field:     [8] DIO8
//
// Clears bit DIO8 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO8                                        0x00000100U
#define GPIO_DOECLR31_0_DIO8_M                                      0x00000100U
#define GPIO_DOECLR31_0_DIO8_S                                               8U
#define GPIO_DOECLR31_0_DIO8_CLR                                    0x00000100U
#define GPIO_DOECLR31_0_DIO8_NO_EFFECT                              0x00000000U

// Field:     [6] DIO6
//
// Clears bit DIO6 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO6                                        0x00000040U
#define GPIO_DOECLR31_0_DIO6_M                                      0x00000040U
#define GPIO_DOECLR31_0_DIO6_S                                               6U
#define GPIO_DOECLR31_0_DIO6_CLR                                    0x00000040U
#define GPIO_DOECLR31_0_DIO6_NO_EFFECT                              0x00000000U

// Field:     [4] DIO4
//
// Clears bit DIO4 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO4                                        0x00000010U
#define GPIO_DOECLR31_0_DIO4_M                                      0x00000010U
#define GPIO_DOECLR31_0_DIO4_S                                               4U
#define GPIO_DOECLR31_0_DIO4_CLR                                    0x00000010U
#define GPIO_DOECLR31_0_DIO4_NO_EFFECT                              0x00000000U

// Field:     [3] DIO3
//
// Clears bit DIO3 in DOE31_0
// ENUMs:
// CLR                      Clear
// NO_EFFECT                No effect
#define GPIO_DOECLR31_0_DIO3                                        0x00000008U
#define GPIO_DOECLR31_0_DIO3_M                                      0x00000008U
#define GPIO_DOECLR31_0_DIO3_S                                               3U
#define GPIO_DOECLR31_0_DIO3_CLR                                    0x00000008U
#define GPIO_DOECLR31_0_DIO3_NO_EFFECT                              0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DOETGL31_0
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Toggles bit DIO24 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO24                                       0x01000000U
#define GPIO_DOETGL31_0_DIO24_M                                     0x01000000U
#define GPIO_DOETGL31_0_DIO24_S                                             24U
#define GPIO_DOETGL31_0_DIO24_TOGGLE                                0x01000000U
#define GPIO_DOETGL31_0_DIO24_NO_EFFECT                             0x00000000U

// Field:    [21] DIO21
//
// Toggles bit DIO21 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO21                                       0x00200000U
#define GPIO_DOETGL31_0_DIO21_M                                     0x00200000U
#define GPIO_DOETGL31_0_DIO21_S                                             21U
#define GPIO_DOETGL31_0_DIO21_TOGGLE                                0x00200000U
#define GPIO_DOETGL31_0_DIO21_NO_EFFECT                             0x00000000U

// Field:    [20] DIO20
//
// Toggles bit DIO20 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO20                                       0x00100000U
#define GPIO_DOETGL31_0_DIO20_M                                     0x00100000U
#define GPIO_DOETGL31_0_DIO20_S                                             20U
#define GPIO_DOETGL31_0_DIO20_TOGGLE                                0x00100000U
#define GPIO_DOETGL31_0_DIO20_NO_EFFECT                             0x00000000U

// Field:    [17] DIO17
//
// Toggles bit DIO17 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO17                                       0x00020000U
#define GPIO_DOETGL31_0_DIO17_M                                     0x00020000U
#define GPIO_DOETGL31_0_DIO17_S                                             17U
#define GPIO_DOETGL31_0_DIO17_TOGGLE                                0x00020000U
#define GPIO_DOETGL31_0_DIO17_NO_EFFECT                             0x00000000U

// Field:    [16] DIO16
//
// Toggles bit DIO16 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO16                                       0x00010000U
#define GPIO_DOETGL31_0_DIO16_M                                     0x00010000U
#define GPIO_DOETGL31_0_DIO16_S                                             16U
#define GPIO_DOETGL31_0_DIO16_TOGGLE                                0x00010000U
#define GPIO_DOETGL31_0_DIO16_NO_EFFECT                             0x00000000U

// Field:    [13] DIO13
//
// Toggles bit DIO13 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO13                                       0x00002000U
#define GPIO_DOETGL31_0_DIO13_M                                     0x00002000U
#define GPIO_DOETGL31_0_DIO13_S                                             13U
#define GPIO_DOETGL31_0_DIO13_TOGGLE                                0x00002000U
#define GPIO_DOETGL31_0_DIO13_NO_EFFECT                             0x00000000U

// Field:    [12] DIO12
//
// Toggles bit DIO12 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO12                                       0x00001000U
#define GPIO_DOETGL31_0_DIO12_M                                     0x00001000U
#define GPIO_DOETGL31_0_DIO12_S                                             12U
#define GPIO_DOETGL31_0_DIO12_TOGGLE                                0x00001000U
#define GPIO_DOETGL31_0_DIO12_NO_EFFECT                             0x00000000U

// Field:    [11] DIO11
//
// Toggles bit DIO11 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO11                                       0x00000800U
#define GPIO_DOETGL31_0_DIO11_M                                     0x00000800U
#define GPIO_DOETGL31_0_DIO11_S                                             11U
#define GPIO_DOETGL31_0_DIO11_TOGGLE                                0x00000800U
#define GPIO_DOETGL31_0_DIO11_NO_EFFECT                             0x00000000U

// Field:     [8] DIO8
//
// Toggles bit DIO8 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO8                                        0x00000100U
#define GPIO_DOETGL31_0_DIO8_M                                      0x00000100U
#define GPIO_DOETGL31_0_DIO8_S                                               8U
#define GPIO_DOETGL31_0_DIO8_TOGGLE                                 0x00000100U
#define GPIO_DOETGL31_0_DIO8_NO_EFFECT                              0x00000000U

// Field:     [6] DIO6
//
// Toggles bit DIO6 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO6                                        0x00000040U
#define GPIO_DOETGL31_0_DIO6_M                                      0x00000040U
#define GPIO_DOETGL31_0_DIO6_S                                               6U
#define GPIO_DOETGL31_0_DIO6_TOGGLE                                 0x00000040U
#define GPIO_DOETGL31_0_DIO6_NO_EFFECT                              0x00000000U

// Field:     [4] DIO4
//
// Toggles bit DIO4 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO4                                        0x00000010U
#define GPIO_DOETGL31_0_DIO4_M                                      0x00000010U
#define GPIO_DOETGL31_0_DIO4_S                                               4U
#define GPIO_DOETGL31_0_DIO4_TOGGLE                                 0x00000010U
#define GPIO_DOETGL31_0_DIO4_NO_EFFECT                              0x00000000U

// Field:     [3] DIO3
//
// Toggles bit DIO3 in DOE31_0
// ENUMs:
// TOGGLE                   Toggle
// NO_EFFECT                No effect
#define GPIO_DOETGL31_0_DIO3                                        0x00000008U
#define GPIO_DOETGL31_0_DIO3_M                                      0x00000008U
#define GPIO_DOETGL31_0_DIO3_S                                               3U
#define GPIO_DOETGL31_0_DIO3_TOGGLE                                 0x00000008U
#define GPIO_DOETGL31_0_DIO3_NO_EFFECT                              0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DIN3_0
//
//*****************************************************************************
// Field:    [24] DIO3
//
// Data input from DIO3
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN3_0_DIO3                                            0x01000000U
#define GPIO_DIN3_0_DIO3_M                                          0x01000000U
#define GPIO_DIN3_0_DIO3_S                                                  24U
#define GPIO_DIN3_0_DIO3_ONE                                        0x01000000U
#define GPIO_DIN3_0_DIO3_ZERO                                       0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DIN7_4
//
//*****************************************************************************
// Field:    [16] DIO6
//
// Data input from DIO6
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN7_4_DIO6                                            0x00010000U
#define GPIO_DIN7_4_DIO6_M                                          0x00010000U
#define GPIO_DIN7_4_DIO6_S                                                  16U
#define GPIO_DIN7_4_DIO6_ONE                                        0x00010000U
#define GPIO_DIN7_4_DIO6_ZERO                                       0x00000000U

// Field:     [0] DIO4
//
// Data input from DIO4
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN7_4_DIO4                                            0x00000001U
#define GPIO_DIN7_4_DIO4_M                                          0x00000001U
#define GPIO_DIN7_4_DIO4_S                                                   0U
#define GPIO_DIN7_4_DIO4_ONE                                        0x00000001U
#define GPIO_DIN7_4_DIO4_ZERO                                       0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DIN11_8
//
//*****************************************************************************
// Field:    [24] DIO11
//
// Data input from DIO11
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN11_8_DIO11                                          0x01000000U
#define GPIO_DIN11_8_DIO11_M                                        0x01000000U
#define GPIO_DIN11_8_DIO11_S                                                24U
#define GPIO_DIN11_8_DIO11_ONE                                      0x01000000U
#define GPIO_DIN11_8_DIO11_ZERO                                     0x00000000U

// Field:     [0] DIO8
//
// Data input from DIO8
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN11_8_DIO8                                           0x00000001U
#define GPIO_DIN11_8_DIO8_M                                         0x00000001U
#define GPIO_DIN11_8_DIO8_S                                                  0U
#define GPIO_DIN11_8_DIO8_ONE                                       0x00000001U
#define GPIO_DIN11_8_DIO8_ZERO                                      0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DIN15_12
//
//*****************************************************************************
// Field:     [8] DIO13
//
// Data input from DIO13
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN15_12_DIO13                                         0x00000100U
#define GPIO_DIN15_12_DIO13_M                                       0x00000100U
#define GPIO_DIN15_12_DIO13_S                                                8U
#define GPIO_DIN15_12_DIO13_ONE                                     0x00000100U
#define GPIO_DIN15_12_DIO13_ZERO                                    0x00000000U

// Field:     [0] DIO12
//
// Data input from DIO12
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN15_12_DIO12                                         0x00000001U
#define GPIO_DIN15_12_DIO12_M                                       0x00000001U
#define GPIO_DIN15_12_DIO12_S                                                0U
#define GPIO_DIN15_12_DIO12_ONE                                     0x00000001U
#define GPIO_DIN15_12_DIO12_ZERO                                    0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DIN19_16
//
//*****************************************************************************
// Field:     [8] DIO17
//
// Data input from DIO17
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN19_16_DIO17                                         0x00000100U
#define GPIO_DIN19_16_DIO17_M                                       0x00000100U
#define GPIO_DIN19_16_DIO17_S                                                8U
#define GPIO_DIN19_16_DIO17_ONE                                     0x00000100U
#define GPIO_DIN19_16_DIO17_ZERO                                    0x00000000U

// Field:     [0] DIO16
//
// Data input from DIO16
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN19_16_DIO16                                         0x00000001U
#define GPIO_DIN19_16_DIO16_M                                       0x00000001U
#define GPIO_DIN19_16_DIO16_S                                                0U
#define GPIO_DIN19_16_DIO16_ONE                                     0x00000001U
#define GPIO_DIN19_16_DIO16_ZERO                                    0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DIN23_20
//
//*****************************************************************************
// Field:     [8] DIO21
//
// Data input from DIO21
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN23_20_DIO21                                         0x00000100U
#define GPIO_DIN23_20_DIO21_M                                       0x00000100U
#define GPIO_DIN23_20_DIO21_S                                                8U
#define GPIO_DIN23_20_DIO21_ONE                                     0x00000100U
#define GPIO_DIN23_20_DIO21_ZERO                                    0x00000000U

// Field:     [0] DIO20
//
// Data input from DIO20
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN23_20_DIO20                                         0x00000001U
#define GPIO_DIN23_20_DIO20_M                                       0x00000001U
#define GPIO_DIN23_20_DIO20_S                                                0U
#define GPIO_DIN23_20_DIO20_ONE                                     0x00000001U
#define GPIO_DIN23_20_DIO20_ZERO                                    0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DIN27_24
//
//*****************************************************************************
// Field:     [0] DIO24
//
// Data input from DIO24
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN27_24_DIO24                                         0x00000001U
#define GPIO_DIN27_24_DIO24_M                                       0x00000001U
#define GPIO_DIN27_24_DIO24_S                                                0U
#define GPIO_DIN27_24_DIO24_ONE                                     0x00000001U
#define GPIO_DIN27_24_DIO24_ZERO                                    0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_DIN31_0
//
//*****************************************************************************
// Field:    [24] DIO24
//
// Data input from DIO24
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO24                                          0x01000000U
#define GPIO_DIN31_0_DIO24_M                                        0x01000000U
#define GPIO_DIN31_0_DIO24_S                                                24U
#define GPIO_DIN31_0_DIO24_ONE                                      0x01000000U
#define GPIO_DIN31_0_DIO24_ZERO                                     0x00000000U

// Field:    [21] DIO21
//
// Data input from DIO21
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO21                                          0x00200000U
#define GPIO_DIN31_0_DIO21_M                                        0x00200000U
#define GPIO_DIN31_0_DIO21_S                                                21U
#define GPIO_DIN31_0_DIO21_ONE                                      0x00200000U
#define GPIO_DIN31_0_DIO21_ZERO                                     0x00000000U

// Field:    [20] DIO20
//
// Data input from DIO20
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO20                                          0x00100000U
#define GPIO_DIN31_0_DIO20_M                                        0x00100000U
#define GPIO_DIN31_0_DIO20_S                                                20U
#define GPIO_DIN31_0_DIO20_ONE                                      0x00100000U
#define GPIO_DIN31_0_DIO20_ZERO                                     0x00000000U

// Field:    [17] DIO17
//
// Data input from DIO17
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO17                                          0x00020000U
#define GPIO_DIN31_0_DIO17_M                                        0x00020000U
#define GPIO_DIN31_0_DIO17_S                                                17U
#define GPIO_DIN31_0_DIO17_ONE                                      0x00020000U
#define GPIO_DIN31_0_DIO17_ZERO                                     0x00000000U

// Field:    [16] DIO16
//
// Data input from DIO16
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO16                                          0x00010000U
#define GPIO_DIN31_0_DIO16_M                                        0x00010000U
#define GPIO_DIN31_0_DIO16_S                                                16U
#define GPIO_DIN31_0_DIO16_ONE                                      0x00010000U
#define GPIO_DIN31_0_DIO16_ZERO                                     0x00000000U

// Field:    [13] DIO13
//
// Data input from DIO13
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO13                                          0x00002000U
#define GPIO_DIN31_0_DIO13_M                                        0x00002000U
#define GPIO_DIN31_0_DIO13_S                                                13U
#define GPIO_DIN31_0_DIO13_ONE                                      0x00002000U
#define GPIO_DIN31_0_DIO13_ZERO                                     0x00000000U

// Field:    [12] DIO12
//
// Data input from DIO12
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO12                                          0x00001000U
#define GPIO_DIN31_0_DIO12_M                                        0x00001000U
#define GPIO_DIN31_0_DIO12_S                                                12U
#define GPIO_DIN31_0_DIO12_ONE                                      0x00001000U
#define GPIO_DIN31_0_DIO12_ZERO                                     0x00000000U

// Field:    [11] DIO11
//
// Data input from DIO11
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO11                                          0x00000800U
#define GPIO_DIN31_0_DIO11_M                                        0x00000800U
#define GPIO_DIN31_0_DIO11_S                                                11U
#define GPIO_DIN31_0_DIO11_ONE                                      0x00000800U
#define GPIO_DIN31_0_DIO11_ZERO                                     0x00000000U

// Field:     [8] DIO8
//
// Data input from DIO8
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO8                                           0x00000100U
#define GPIO_DIN31_0_DIO8_M                                         0x00000100U
#define GPIO_DIN31_0_DIO8_S                                                  8U
#define GPIO_DIN31_0_DIO8_ONE                                       0x00000100U
#define GPIO_DIN31_0_DIO8_ZERO                                      0x00000000U

// Field:     [6] DIO6
//
// Data input from DIO6
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO6                                           0x00000040U
#define GPIO_DIN31_0_DIO6_M                                         0x00000040U
#define GPIO_DIN31_0_DIO6_S                                                  6U
#define GPIO_DIN31_0_DIO6_ONE                                       0x00000040U
#define GPIO_DIN31_0_DIO6_ZERO                                      0x00000000U

// Field:     [4] DIO4
//
// Data input from DIO4
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO4                                           0x00000010U
#define GPIO_DIN31_0_DIO4_M                                         0x00000010U
#define GPIO_DIN31_0_DIO4_S                                                  4U
#define GPIO_DIN31_0_DIO4_ONE                                       0x00000010U
#define GPIO_DIN31_0_DIO4_ZERO                                      0x00000000U

// Field:     [3] DIO3
//
// Data input from DIO3
// ENUMs:
// ONE                      Input value is 1
// ZERO                     Input value is 0
#define GPIO_DIN31_0_DIO3                                           0x00000008U
#define GPIO_DIN31_0_DIO3_M                                         0x00000008U
#define GPIO_DIN31_0_DIO3_S                                                  3U
#define GPIO_DIN31_0_DIO3_ONE                                       0x00000008U
#define GPIO_DIN31_0_DIO3_ZERO                                      0x00000000U

//*****************************************************************************
//
// Register: GPIO_O_EVTCFG
//
//*****************************************************************************
// Field:     [8] EVTEN
//
// Enables GPIO to publish edge qualified selected DIO event on SVT event
// fabric.
// Design note: The edge detector flop is cleared automatically for the
// selected DIO once the event is published.
// ENUMs:
// ENABLE                   Enable
// DISABLE                  Disable
#define GPIO_EVTCFG_EVTEN                                           0x00000100U
#define GPIO_EVTCFG_EVTEN_M                                         0x00000100U
#define GPIO_EVTCFG_EVTEN_S                                                  8U
#define GPIO_EVTCFG_EVTEN_ENABLE                                    0x00000100U
#define GPIO_EVTCFG_EVTEN_DISABLE                                   0x00000000U

// Field:   [5:0] DIOSEL
//
// This is used to select DIO for event generation. For example, DIOSEL = 0x0
// selects DIO0 and DIOSEL = 0x8 selects DIO8.
// ENUMs:
// MAXIMUM                  Maximum value
// MINIMUM                  Minimum value
#define GPIO_EVTCFG_DIOSEL_W                                                 6U
#define GPIO_EVTCFG_DIOSEL_M                                        0x0000003FU
#define GPIO_EVTCFG_DIOSEL_S                                                 0U
#define GPIO_EVTCFG_DIOSEL_MAXIMUM                                  0x0000003FU
#define GPIO_EVTCFG_DIOSEL_MINIMUM                                  0x00000000U


#endif // __GPIO__
