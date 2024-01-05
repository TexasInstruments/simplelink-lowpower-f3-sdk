/******************************************************************************
*  Filename:       hw_hsmcrypto_h
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

#ifndef __HW_HSMCRYPTO_H__
#define __HW_HSMCRYPTO_H__

//*****************************************************************************
//
// This section defines the register offsets of
// HSMCRYPTO component
//
//*****************************************************************************
// Input Mailbox 1
#define HSMCRYPTO_O_MBX1IN                                          0x00000000U

// Output Mailbox 1
#define HSMCRYPTO_O_MBX1OUT                                         0x00000000U

// Input Mailbox 2
#define HSMCRYPTO_O_MBX2IN                                          0x00000400U

// Output Mailbox 2
#define HSMCRYPTO_O_MBX2OUT                                         0x00000400U

// AIC Polarity Control Register
#define HSMCRYPTO_O_AICPOLCTRL                                      0x00003E00U

// AIC Type Control Register
#define HSMCRYPTO_O_AICTYPECTRL                                     0x00003E04U

// AIC Enable Control Register
#define HSMCRYPTO_O_AICENABLECTRL                                   0x00003E08U

// AIC Raw Source Status Register
#define HSMCRYPTO_O_AICRAWSTAT                                      0x00003E0CU

// AIC Enable Set Registers
#define HSMCRYPTO_O_AICENABLESET                                    0x00003E0CU

// AIC Enabled Status Register
#define HSMCRYPTO_O_AICENABLEDSTAT                                  0x00003E10U

// AIC Acknowledge Register
#define HSMCRYPTO_O_AICACK                                          0x00003E10U

// AIC Enable Clear Register
#define HSMCRYPTO_O_AICENABLECLR                                    0x00003E14U

// AIC Options Register
#define HSMCRYPTO_O_AICOPTIONS                                      0x00003E18U

// AIC Version Register
#define HSMCRYPTO_O_AICVERSION                                      0x00003E1CU

// Mailbox Status Register
#define HSMCRYPTO_O_MBXSTAT                                         0x00003F00U

// Mailbox Control Register
#define HSMCRYPTO_O_MBXCTRL                                         0x00003F00U

// Raw (unmasked) Mailbox Status Register
#define HSMCRYPTO_O_MBXRAWSTAT                                      0x00003F04U

// Mailbox Reset Register
#define HSMCRYPTO_O_MBXRESET                                        0x00003F04U

// Mailbox Status - linked Host IDs Register
#define HSMCRYPTO_O_MBXLINKID                                       0x00003F08U

// Mailbox Status - output Host IDs Register
#define HSMCRYPTO_O_MBXOUTID                                        0x00003F0CU

// Host/Mailbox1-4  lockout control Register
#define HSMCRYPTO_O_MBXLOCKOUT                                      0x00003F10U

// Module Status Register
#define HSMCRYPTO_O_MODULESTATUS                                    0x00003FE0U

// Configured options(2)
#define HSMCRYPTO_O_OPTIONS2                                        0x00003FF4U

// Configured options(1)
#define HSMCRYPTO_O_OPTIONS                                         0x00003FF8U

// Version register
#define HSMCRYPTO_O_VERSION                                         0x00003FFCU

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBX1IN
//
//*****************************************************************************
// Field:  [31:0] MEM
//
#define HSMCRYPTO_MBX1IN_MEM_W                                              32U
#define HSMCRYPTO_MBX1IN_MEM_M                                      0xFFFFFFFFU
#define HSMCRYPTO_MBX1IN_MEM_S                                               0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBX1OUT
//
//*****************************************************************************
// Field:  [31:0] MEM
//
#define HSMCRYPTO_MBX1OUT_MEM_W                                             32U
#define HSMCRYPTO_MBX1OUT_MEM_M                                     0xFFFFFFFFU
#define HSMCRYPTO_MBX1OUT_MEM_S                                              0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBX2IN
//
//*****************************************************************************
// Field:  [31:0] MEM
//
#define HSMCRYPTO_MBX2IN_MEM_W                                              32U
#define HSMCRYPTO_MBX2IN_MEM_M                                      0xFFFFFFFFU
#define HSMCRYPTO_MBX2IN_MEM_S                                               0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBX2OUT
//
//*****************************************************************************
// Field:  [31:0] MEM
//
#define HSMCRYPTO_MBX2OUT_MEM_W                                             32U
#define HSMCRYPTO_MBX2OUT_MEM_M                                     0xFFFFFFFFU
#define HSMCRYPTO_MBX2OUT_MEM_S                                              0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICPOLCTRL
//
//*****************************************************************************
// Field:     [4] POLCTRL4
//
// Individual polarity (high level/rising edge or low level/falling edge)
// control bits per interrupt input: 0b = Low level/falling edge 1b = High
// level/rising edge.
#define HSMCRYPTO_AICPOLCTRL_POLCTRL4                               0x00000010U
#define HSMCRYPTO_AICPOLCTRL_POLCTRL4_M                             0x00000010U
#define HSMCRYPTO_AICPOLCTRL_POLCTRL4_S                                      4U

// Field:     [3] POLCTRL3
//
// Individual polarity (high level/rising edge or low level/falling edge)
// control bits per interrupt input: 0b = Low level/falling edge 1b = High
// level/rising edge.
#define HSMCRYPTO_AICPOLCTRL_POLCTRL3                               0x00000008U
#define HSMCRYPTO_AICPOLCTRL_POLCTRL3_M                             0x00000008U
#define HSMCRYPTO_AICPOLCTRL_POLCTRL3_S                                      3U

// Field:     [2] POLCTRL2
//
// Individual polarity (high level/rising edge or low level/falling edge)
// control bits per interrupt input: 0b = Low level/falling edge 1b = High
// level/rising edge.
#define HSMCRYPTO_AICPOLCTRL_POLCTRL2                               0x00000004U
#define HSMCRYPTO_AICPOLCTRL_POLCTRL2_M                             0x00000004U
#define HSMCRYPTO_AICPOLCTRL_POLCTRL2_S                                      2U

// Field:     [1] POLCTRL1
//
// Individual polarity (high level/rising edge or low level/falling edge)
// control bits per interrupt input: 0b = Low level/falling edge 1b = High
// level/rising edge.
#define HSMCRYPTO_AICPOLCTRL_POLCTRL1                               0x00000002U
#define HSMCRYPTO_AICPOLCTRL_POLCTRL1_M                             0x00000002U
#define HSMCRYPTO_AICPOLCTRL_POLCTRL1_S                                      1U

// Field:     [0] POLCTRL0
//
// Individual polarity (high level/rising edge or low level/falling edge)
// control bits per interrupt input: 0b = Low level/falling edge 1b = High
// level/rising edge.
#define HSMCRYPTO_AICPOLCTRL_POLCTRL0                               0x00000001U
#define HSMCRYPTO_AICPOLCTRL_POLCTRL0_M                             0x00000001U
#define HSMCRYPTO_AICPOLCTRL_POLCTRL0_S                                      0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICTYPECTRL
//
//*****************************************************************************
// Field:     [4] TYPCTRL4
//
// Signal type (level or edge) control bits for each interrupt input: 0b =
// level (the interrupt source level determines the raw status). 1b = edge (the
// interrupt source is connected to an edge detector and the edge detector
// output determines the raw status).
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL4                              0x00000010U
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL4_M                            0x00000010U
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL4_S                                     4U

// Field:     [3] TYPCTRL3
//
// Signal type (level or edge) control bits for each interrupt input: 0b =
// level (the interrupt source level determines the raw status). 1b = edge (the
// interrupt source is connected to an edge detector and the edge detector
// output determines the raw status).
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL3                              0x00000008U
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL3_M                            0x00000008U
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL3_S                                     3U

// Field:     [2] TYPCTRL2
//
// Signal type (level or edge) control bits for each interrupt input: 0b =
// level (the interrupt source level determines the raw status). 1b = edge (the
// interrupt source is connected to an edge detector and the edge detector
// output determines the raw status).
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL2                              0x00000004U
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL2_M                            0x00000004U
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL2_S                                     2U

// Field:     [1] TYPCTRL1
//
// Signal type (level or edge) control bits for each interrupt input: 0b =
// level (the interrupt source level determines the raw status). 1b = edge (the
// interrupt source is connected to an edge detector and the edge detector
// output determines the raw status).
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL1                              0x00000002U
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL1_M                            0x00000002U
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL1_S                                     1U

// Field:     [0] TYPCTRL
//
// Signal type (level or edge) control bits for each interrupt input: 0b =
// level (the interrupt source level determines the raw status). 1b = edge (the
// interrupt source is connected to an edge detector and the edge detector
// output determines the raw status).
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL                               0x00000001U
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL_M                             0x00000001U
#define HSMCRYPTO_AICTYPECTRL_TYPCTRL_S                                      0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICENABLECTRL
//
//*****************************************************************************
// Field:   [4:0] ENCTRL
//
// Individual enable control bits per interrupt input: 0b = Disabled. 1b =
// Enabled
#define HSMCRYPTO_AICENABLECTRL_ENCTRL_W                                     5U
#define HSMCRYPTO_AICENABLECTRL_ENCTRL_M                            0x0000001FU
#define HSMCRYPTO_AICENABLECTRL_ENCTRL_S                                     0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICRAWSTAT
//
//*****************************************************************************
// Field:     [4] RAWSTAT4
//
// Individual interrupt status bit before masking with enable_ctrl_r[4]
// (programmable, reset to 'low level' mode): 0b = Inactive, 1b = Pending.
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT4                               0x00000010U
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT4_M                             0x00000010U
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT4_S                                      4U

// Field:     [3] RAWSTAT3
//
// Individual interrupt status bit before masking with enable_ctrl_r[3]
// (programmable, reset to 'low level' mode): 0b = Inactive, 1b = Pending.
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT3                               0x00000008U
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT3_M                             0x00000008U
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT3_S                                      3U

// Field:     [2] RAWSTAT2
//
// Individual interrupt status bit before masking with enable_ctrl_r[2]
// (programmable, reset to 'low level' mode): 0b = Inactive, 1b = Pending.
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT2                               0x00000004U
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT2_M                             0x00000004U
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT2_S                                      2U

// Field:     [1] RAWSTAT1
//
// Individual interrupt status bit before masking with enable_ctrl_r[1]
// (programmable, reset to 'low level' mode): 0b = Inactive, 1b = Pending.
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT1                               0x00000002U
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT1_M                             0x00000002U
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT1_S                                      1U

// Field:     [0] RAWSTAT0
//
// Individual interrupt status bit before masking with enable_ctrl_r[0]
// (programmable, reset to 'low level' mode): 0b = Inactive, 1b = Pending.
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT0                               0x00000001U
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT0_M                             0x00000001U
#define HSMCRYPTO_AICRAWSTAT_RAWSTAT0_S                                      0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICENABLESET
//
//*****************************************************************************
// Field:   [4:0] ENSET
//
// Individual interrupt enable bits per interrupt input: 0b = No effect. 1b =
// Set the corresponding bit in the AICENABLECTRL register, enabling the
// interrupt. After writing a 1b, there is no need to write a 0b.
#define HSMCRYPTO_AICENABLESET_ENSET_W                                       5U
#define HSMCRYPTO_AICENABLESET_ENSET_M                              0x0000001FU
#define HSMCRYPTO_AICENABLESET_ENSET_S                                       0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICENABLEDSTAT
//
//*****************************************************************************
// Field:   [4:0] ENSTAT
//
// These bits reflect the status of the interrupts after polarity control and
// optional edge detection, gated with bits in AICENABLECTRL register: 0b =
// Inactive. 1b = Pending.
#define HSMCRYPTO_AICENABLEDSTAT_ENSTAT_W                                    5U
#define HSMCRYPTO_AICENABLEDSTAT_ENSTAT_M                           0x0000001FU
#define HSMCRYPTO_AICENABLEDSTAT_ENSTAT_S                                    0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICACK
//
//*****************************************************************************
// Field:     [4] ACK4
//
// Used to clear edge-detect interrupts: 0b = No effect. 1b = Acknowledge the
// interrupt signal, clears the edge detector and status bit when in edge
// detect mode (bit [4] of TYPE_CTRL == 1b). After writing a 1b, there is no
// need to write a 0b.
#define HSMCRYPTO_AICACK_ACK4                                       0x00000010U
#define HSMCRYPTO_AICACK_ACK4_M                                     0x00000010U
#define HSMCRYPTO_AICACK_ACK4_S                                              4U

// Field:     [3] ACK3
//
// Used to clear edge-detect interrupts: 0b = No effect. 1b = Acknowledge the
// interrupt signal, clears the edge detector and status bit when in edge
// detect mode (bit [3] of TYPE_CTRL == 1b). After writing a 1b, there is no
// need to write a 0b.
#define HSMCRYPTO_AICACK_ACK3                                       0x00000008U
#define HSMCRYPTO_AICACK_ACK3_M                                     0x00000008U
#define HSMCRYPTO_AICACK_ACK3_S                                              3U

// Field:     [2] ACK2
//
// Used to clear edge-detect interrupts: 0b = No effect. 1b = Acknowledge the
// interrupt signal, clears the edge detector and status bit when in edge
// detect mode (bit [2] of TYPE_CTRL == 1b). After writing a 1b, there is no
// need to write a 0b.
#define HSMCRYPTO_AICACK_ACK2                                       0x00000004U
#define HSMCRYPTO_AICACK_ACK2_M                                     0x00000004U
#define HSMCRYPTO_AICACK_ACK2_S                                              2U

// Field:     [1] ACK1
//
// Used to clear edge-detect interrupts: 0b = No effect. 1b = Acknowledge the
// interrupt signal, clears the edge detector and status bit when in edge
// detect mode (bit [1] of TYPE_CTRL == 1b). After writing a 1b, there is no
// need to write a 0b.
#define HSMCRYPTO_AICACK_ACK1                                       0x00000002U
#define HSMCRYPTO_AICACK_ACK1_M                                     0x00000002U
#define HSMCRYPTO_AICACK_ACK1_S                                              1U

// Field:     [0] ACK0
//
// Used to clear edge-detect interrupts: 0b = No effect. 1b = Acknowledge the
// interrupt signal, clears the edge detector and status bit when in edge
// detect mode (bit [0] of TYPE_CTRL == 1b). After writing a 1b, there is no
// need to write a 0b.
#define HSMCRYPTO_AICACK_ACK0                                       0x00000001U
#define HSMCRYPTO_AICACK_ACK0_M                                     0x00000001U
#define HSMCRYPTO_AICACK_ACK0_S                                              0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICENABLECLR
//
//*****************************************************************************
// Field:   [4:0] ENCLR
//
// Individual interrupt disable bits per interrupt input: 0b = No effect. 1b =
// Clear the corresponding bit in the AICENABLECTRL register, disabling the
// interrupt. After writing a 1b, there is no need to write a 0b.
#define HSMCRYPTO_AICENABLECLR_ENCLR_W                                       5U
#define HSMCRYPTO_AICENABLECLR_ENCLR_M                              0x0000001FU
#define HSMCRYPTO_AICENABLECLR_ENCLR_S                                       0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICOPTIONS
//
//*****************************************************************************
// Field:     [8] MINIREGMAP
//
// Mini register map.
#define HSMCRYPTO_AICOPTIONS_MINIREGMAP                             0x00000100U
#define HSMCRYPTO_AICOPTIONS_MINIREGMAP_M                           0x00000100U
#define HSMCRYPTO_AICOPTIONS_MINIREGMAP_S                                    8U

// Field:     [7] EXTREGMAP
//
// Extended register map.
#define HSMCRYPTO_AICOPTIONS_EXTREGMAP                              0x00000080U
#define HSMCRYPTO_AICOPTIONS_EXTREGMAP_M                            0x00000080U
#define HSMCRYPTO_AICOPTIONS_EXTREGMAP_S                                     7U

// Field:   [5:0] INPUTS
//
// The number of interrupt request inputs.
#define HSMCRYPTO_AICOPTIONS_INPUTS_W                                        6U
#define HSMCRYPTO_AICOPTIONS_INPUTS_M                               0x0000003FU
#define HSMCRYPTO_AICOPTIONS_INPUTS_S                                        0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICVERSION
//
//*****************************************************************************
// Field: [27:24] MAJORVER
//
// These bits encode the major version number for the AIC module.
#define HSMCRYPTO_AICVERSION_MAJORVER_W                                      4U
#define HSMCRYPTO_AICVERSION_MAJORVER_M                             0x0F000000U
#define HSMCRYPTO_AICVERSION_MAJORVER_S                                     24U

// Field: [23:20] MINORVER
//
// These bits encode the minor version number for the AIC module.
#define HSMCRYPTO_AICVERSION_MINORVER_W                                      4U
#define HSMCRYPTO_AICVERSION_MINORVER_M                             0x00F00000U
#define HSMCRYPTO_AICVERSION_MINORVER_S                                     20U

// Field: [19:16] PATCHLEVEL
//
// These bits encode the hardware patch level for the AIC module, starting at
// value 0 on the first release.
#define HSMCRYPTO_AICVERSION_PATCHLEVEL_W                                    4U
#define HSMCRYPTO_AICVERSION_PATCHLEVEL_M                           0x000F0000U
#define HSMCRYPTO_AICVERSION_PATCHLEVEL_S                                   16U

// Field:  [15:8] AICNUMBERCMPL
//
// These bits simply contain the complement of bits [7:0], used by a driver to
// ascertain that this version register is indeed read.
#define HSMCRYPTO_AICVERSION_AICNUMBERCMPL_W                                 8U
#define HSMCRYPTO_AICVERSION_AICNUMBERCMPL_M                        0x0000FF00U
#define HSMCRYPTO_AICVERSION_AICNUMBERCMPL_S                                 8U

// Field:   [7:0] AICNUMBER
//
// These bits encode the AIC number.
#define HSMCRYPTO_AICVERSION_AICNUMBER_W                                     8U
#define HSMCRYPTO_AICVERSION_AICNUMBER_M                            0x000000FFU
#define HSMCRYPTO_AICVERSION_AICNUMBER_S                                     0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBXSTAT
//
//*****************************************************************************
// Field:     [7] MBX2AVAILABLE
//
// (set)-Input Mailbox is linked to a Host or is filled, (Clear)- Input Mailbox
// is available for linking by this Host
#define HSMCRYPTO_MBXSTAT_MBX2AVAILABLE                             0x00000080U
#define HSMCRYPTO_MBXSTAT_MBX2AVAILABLE_M                           0x00000080U
#define HSMCRYPTO_MBXSTAT_MBX2AVAILABLE_S                                    7U

// Field:     [6] MBX2LINKED
//
// (set)-This Host is linked to Mailbox, (Clear)-This Host is not linked to
// Mailbox
#define HSMCRYPTO_MBXSTAT_MBX2LINKED                                0x00000040U
#define HSMCRYPTO_MBXSTAT_MBX2LINKED_M                              0x00000040U
#define HSMCRYPTO_MBXSTAT_MBX2LINKED_S                                       6U

// Field:     [5] MBX2OUTFULL
//
// (set)-Output Mailbox contains an output token, (Clear)-Output Mailbox is
// empty
#define HSMCRYPTO_MBXSTAT_MBX2OUTFULL                               0x00000020U
#define HSMCRYPTO_MBXSTAT_MBX2OUTFULL_M                             0x00000020U
#define HSMCRYPTO_MBXSTAT_MBX2OUTFULL_S                                      5U

// Field:     [4] MBX2INFULL
//
// (set)-Input Mailbox contains a token that is handed over to Internal
// Controller, (Clear)-Input Mailbox is ready to rceive new token
#define HSMCRYPTO_MBXSTAT_MBX2INFULL                                0x00000010U
#define HSMCRYPTO_MBXSTAT_MBX2INFULL_M                              0x00000010U
#define HSMCRYPTO_MBXSTAT_MBX2INFULL_S                                       4U

// Field:     [3] MBX1AVAILABLE
//
// (set)-Input Mailbox is linked to a Host or is filled, (Clear)- Input Mailbox
// is available for linking by this Host
#define HSMCRYPTO_MBXSTAT_MBX1AVAILABLE                             0x00000008U
#define HSMCRYPTO_MBXSTAT_MBX1AVAILABLE_M                           0x00000008U
#define HSMCRYPTO_MBXSTAT_MBX1AVAILABLE_S                                    3U

// Field:     [2] MBX1LINKED
//
// (set)-This Host is linked to Mailbox, (Clear)-This Host is not linked to
// Mailbox
#define HSMCRYPTO_MBXSTAT_MBX1LINKED                                0x00000004U
#define HSMCRYPTO_MBXSTAT_MBX1LINKED_M                              0x00000004U
#define HSMCRYPTO_MBXSTAT_MBX1LINKED_S                                       2U

// Field:     [1] MBX1OUTFULL
//
// (set)-Output Mailbox contains an output token, (Clear)-Output Mailbox is
// empty
#define HSMCRYPTO_MBXSTAT_MBX1OUTFULL                               0x00000002U
#define HSMCRYPTO_MBXSTAT_MBX1OUTFULL_M                             0x00000002U
#define HSMCRYPTO_MBXSTAT_MBX1OUTFULL_S                                      1U

// Field:     [0] MBX1INFULL
//
// (set)-Input Mailbox contains a token that is handed over to Internal
// Controller, (Clear)-Input Mailbox is ready to rceive new token
#define HSMCRYPTO_MBXSTAT_MBX1INFULL                                0x00000001U
#define HSMCRYPTO_MBXSTAT_MBX1INFULL_M                              0x00000001U
#define HSMCRYPTO_MBXSTAT_MBX1INFULL_S                                       0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBXCTRL
//
//*****************************************************************************
// Field:     [7] MBX2UNLINK
//
// Set only - Writing a 1b here unlinks the MAilbox from this HOST and clears
// the mbx_linked bit in MBXSTAT
#define HSMCRYPTO_MBXCTRL_MBX2UNLINK                                0x00000080U
#define HSMCRYPTO_MBXCTRL_MBX2UNLINK_M                              0x00000080U
#define HSMCRYPTO_MBXCTRL_MBX2UNLINK_S                                       7U

// Field:     [6] MBX2LINK
//
// Writing a 1b here links Mailbox to this Host - only if Mailbox not filled
// and not linked to another host
#define HSMCRYPTO_MBXCTRL_MBX2LINK                                  0x00000040U
#define HSMCRYPTO_MBXCTRL_MBX2LINK_M                                0x00000040U
#define HSMCRYPTO_MBXCTRL_MBX2LINK_S                                         6U

// Field:     [5] MBX2OUTEMPTY
//
// Set only - The Host for whom the token in Output Mailbox is meant can clear
// the mbx_out_full bit in MBXSTAT by writing 1b here.
#define HSMCRYPTO_MBXCTRL_MBX2OUTEMPTY                              0x00000020U
#define HSMCRYPTO_MBXCTRL_MBX2OUTEMPTY_M                            0x00000020U
#define HSMCRYPTO_MBXCTRL_MBX2OUTEMPTY_S                                     5U

// Field:     [4] MBX2INFULL
//
// Set only - The Host linked to mailbox can set the mbx_in_full bit in MBXSTAT
// by writing 1b here.
#define HSMCRYPTO_MBXCTRL_MBX2INFULL                                0x00000010U
#define HSMCRYPTO_MBXCTRL_MBX2INFULL_M                              0x00000010U
#define HSMCRYPTO_MBXCTRL_MBX2INFULL_S                                       4U

// Field:     [3] MBX1UNLINK
//
// Set only - Writing a 1b here unlinks the MAilbox from this HOST and clears
// the mbx_linked bit in MBXSTAT
#define HSMCRYPTO_MBXCTRL_MBX1UNLINK                                0x00000008U
#define HSMCRYPTO_MBXCTRL_MBX1UNLINK_M                              0x00000008U
#define HSMCRYPTO_MBXCTRL_MBX1UNLINK_S                                       3U

// Field:     [2] MBX1LINK
//
// Writing a 1b here links Mailbox to this Host - only if Mailbox not filled
// and not linked to another host
#define HSMCRYPTO_MBXCTRL_MBX1LINK                                  0x00000004U
#define HSMCRYPTO_MBXCTRL_MBX1LINK_M                                0x00000004U
#define HSMCRYPTO_MBXCTRL_MBX1LINK_S                                         2U

// Field:     [1] MBX1OUTEMPTY
//
// Set only - The Host for whom the token in Output Mailbox is meant can clear
// the mbx_out_full bit in MBXSTAT by writing 1b here.
#define HSMCRYPTO_MBXCTRL_MBX1OUTEMPTY                              0x00000002U
#define HSMCRYPTO_MBXCTRL_MBX1OUTEMPTY_M                            0x00000002U
#define HSMCRYPTO_MBXCTRL_MBX1OUTEMPTY_S                                     1U

// Field:     [0] MBX1INFULL
//
// Set only - The Host linked to mailbox can set the mbx_in_full bit in MBXSTAT
// by writing 1b here.
#define HSMCRYPTO_MBXCTRL_MBX1INFULL                                0x00000001U
#define HSMCRYPTO_MBXCTRL_MBX1INFULL_M                              0x00000001U
#define HSMCRYPTO_MBXCTRL_MBX1INFULL_S                                       0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBXRAWSTAT
//
//*****************************************************************************
// Field:     [6] MBX2LINKED
//
// (set)-This Host is linked to Mailbox mbx%d , (Clear)-This Host is not linked
// to  Mailbox mbx%d
#define HSMCRYPTO_MBXRAWSTAT_MBX2LINKED                             0x00000040U
#define HSMCRYPTO_MBXRAWSTAT_MBX2LINKED_M                           0x00000040U
#define HSMCRYPTO_MBXRAWSTAT_MBX2LINKED_S                                    6U

// Field:     [5] MBX2OUTFULL
//
// (set)-Output Mailbox contains an output token, (Clear)-Output Mailbox mbx%d
// is empty
#define HSMCRYPTO_MBXRAWSTAT_MBX2OUTFULL                            0x00000020U
#define HSMCRYPTO_MBXRAWSTAT_MBX2OUTFULL_M                          0x00000020U
#define HSMCRYPTO_MBXRAWSTAT_MBX2OUTFULL_S                                   5U

// Field:     [4] MBX2INFULL
//
// (set)-Input Mailbox contains a token that is handed over to Internal
// Controller, (Clear)-Input Mailbox mbx%d is ready to rceive new token
#define HSMCRYPTO_MBXRAWSTAT_MBX2INFULL                             0x00000010U
#define HSMCRYPTO_MBXRAWSTAT_MBX2INFULL_M                           0x00000010U
#define HSMCRYPTO_MBXRAWSTAT_MBX2INFULL_S                                    4U

// Field:     [2] MBX1LINKED
//
// (set)-This Host is linked to Mailbox mbx%d , (Clear)-This Host is not linked
// to  Mailbox mbx%d
#define HSMCRYPTO_MBXRAWSTAT_MBX1LINKED                             0x00000004U
#define HSMCRYPTO_MBXRAWSTAT_MBX1LINKED_M                           0x00000004U
#define HSMCRYPTO_MBXRAWSTAT_MBX1LINKED_S                                    2U

// Field:     [1] MBX1OUTFULL
//
// (set)-Output Mailbox contains an output token, (Clear)-Output Mailbox mbx%d
// is empty
#define HSMCRYPTO_MBXRAWSTAT_MBX1OUTFULL                            0x00000002U
#define HSMCRYPTO_MBXRAWSTAT_MBX1OUTFULL_M                          0x00000002U
#define HSMCRYPTO_MBXRAWSTAT_MBX1OUTFULL_S                                   1U

// Field:     [0] MBX1INFULL
//
// (set)-Input Mailbox contains a token that is handed over to Internal
// Controller, (Clear)-Input Mailbox mbx%d is ready to rceive new token
#define HSMCRYPTO_MBXRAWSTAT_MBX1INFULL                             0x00000001U
#define HSMCRYPTO_MBXRAWSTAT_MBX1INFULL_M                           0x00000001U
#define HSMCRYPTO_MBXRAWSTAT_MBX1INFULL_S                                    0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBXRESET
//
//*****************************************************************************
// Field:     [7] MBX2UNLINK
//
// Set only - Master Host can unlink mbx from it's current Host by writing 1b
// here.
#define HSMCRYPTO_MBXRESET_MBX2UNLINK                               0x00000080U
#define HSMCRYPTO_MBXRESET_MBX2UNLINK_M                             0x00000080U
#define HSMCRYPTO_MBXRESET_MBX2UNLINK_S                                      7U

// Field:     [5] MBX2OUTEMPTY
//
// Set only - Master Host can clear mbx_out_full bit in MBXSTAT by writing 1b
// here.
#define HSMCRYPTO_MBXRESET_MBX2OUTEMPTY                             0x00000020U
#define HSMCRYPTO_MBXRESET_MBX2OUTEMPTY_M                           0x00000020U
#define HSMCRYPTO_MBXRESET_MBX2OUTEMPTY_S                                    5U

// Field:     [3] MBX1UNLINK
//
// Set only - Master Host can unlink mbx from it's current Host by writing 1b
// here.
#define HSMCRYPTO_MBXRESET_MBX1UNLINK                               0x00000008U
#define HSMCRYPTO_MBXRESET_MBX1UNLINK_M                             0x00000008U
#define HSMCRYPTO_MBXRESET_MBX1UNLINK_S                                      3U

// Field:     [1] MBX1OUTEMPTY
//
// Set only - Master Host can clear mbx_out_full bit in MBXSTAT by writing 1b
// here.
#define HSMCRYPTO_MBXRESET_MBX1OUTEMPTY                             0x00000002U
#define HSMCRYPTO_MBXRESET_MBX1OUTEMPTY_M                           0x00000002U
#define HSMCRYPTO_MBXRESET_MBX1OUTEMPTY_S                                    1U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBXLINKID
//
//*****************************************************************************
// Field:     [7] MBX2PROTACC
//
// 0: Mailbox 2 can be accessed by the Host using protected or non-protected
// access. 1: Mailbox 2 is only accessible if the Host uses protected access.
#define HSMCRYPTO_MBXLINKID_MBX2PROTACC                             0x00000080U
#define HSMCRYPTO_MBXLINKID_MBX2PROTACC_M                           0x00000080U
#define HSMCRYPTO_MBXLINKID_MBX2PROTACC_S                                    7U

// Field:   [6:4] MBX2LINK_id
//
// Bits[1:0]Host cpu_id of the Host linked to the Mailbox, bit[2] Set - Mailbox
// is only accessible if Host uses protected access, Clear - Mailbox accessed
// with protected of non-protected access
#define HSMCRYPTO_MBXLINKID_MBX2LINK_ID_W                                    3U
#define HSMCRYPTO_MBXLINKID_MBX2LINK_ID_M                           0x00000070U
#define HSMCRYPTO_MBXLINKID_MBX2LINK_ID_S                                    4U

// Field:     [3] MBX1PROTACC
//
// 0: Mailbox 1 can be accessed by the Host using protected or non-protected
// access. 1: Mailbox 1 is only accessible if the Host uses protected access.
#define HSMCRYPTO_MBXLINKID_MBX1PROTACC                             0x00000008U
#define HSMCRYPTO_MBXLINKID_MBX1PROTACC_M                           0x00000008U
#define HSMCRYPTO_MBXLINKID_MBX1PROTACC_S                                    3U

// Field:   [2:0] MBX1LINK_id
//
// Bits[1:0]Host cpu_id of the Host linked to the Mailbox, bit[2] Set - Mailbox
// is only accessible if Host uses protected access, Clear - Mailbox accessed
// with protected of non-protected access
#define HSMCRYPTO_MBXLINKID_MBX1LINK_ID_W                                    3U
#define HSMCRYPTO_MBXLINKID_MBX1LINK_ID_M                           0x00000007U
#define HSMCRYPTO_MBXLINKID_MBX1LINK_ID_S                                    0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBXOUTID
//
//*****************************************************************************
// Field:     [7] MBX2PROTACC
//
// 0: Output mailbox 2 can be accessed by the Host using protected or
// non-protected access. 1: Output mailbox 2 is only accessible if the Host
// uses protected access.
#define HSMCRYPTO_MBXOUTID_MBX2PROTACC                              0x00000080U
#define HSMCRYPTO_MBXOUTID_MBX2PROTACC_M                            0x00000080U
#define HSMCRYPTO_MBXOUTID_MBX2PROTACC_S                                     7U

// Field:   [6:4] MBX2OUTID
//
// Bit[1,0] Host cpu_id of the Host allowed to read a result from the Mailbox,
// bit[2] Set - Out Mailbox is only accessible if Host uses protected access,
// Clear - Output Mailbox accessed with protected of non-protected access
#define HSMCRYPTO_MBXOUTID_MBX2OUTID_W                                       3U
#define HSMCRYPTO_MBXOUTID_MBX2OUTID_M                              0x00000070U
#define HSMCRYPTO_MBXOUTID_MBX2OUTID_S                                       4U

// Field:     [3] MBX1PROTACC
//
// 0: Output mailbox 1 can be accessed by the Host using protected or
// non-protected access. 1: Output mailbox 1 is only accessible if the Host
// uses protected access.
#define HSMCRYPTO_MBXOUTID_MBX1PROTACC                              0x00000008U
#define HSMCRYPTO_MBXOUTID_MBX1PROTACC_M                            0x00000008U
#define HSMCRYPTO_MBXOUTID_MBX1PROTACC_S                                     3U

// Field:   [2:0] MBX1OUTID
//
// Bit[1,0] Host cpu_id of the Host allowed to read a result from the Mailbox,
// bit[2] Set - Out Mailbox is only accessible if Host uses protected access,
// Clear - Output Mailbox accessed with protected of non-protected access
#define HSMCRYPTO_MBXOUTID_MBX1OUTID_W                                       3U
#define HSMCRYPTO_MBXOUTID_MBX1OUTID_M                              0x00000007U
#define HSMCRYPTO_MBXOUTID_MBX1OUTID_S                                       0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBXLOCKOUT
//
//*****************************************************************************
// Field:  [15:8] MBX2LOCKOUT
//
// Bit map indicates which Hosts are blocked from accessing mailbox
#define HSMCRYPTO_MBXLOCKOUT_MBX2LOCKOUT_W                                   8U
#define HSMCRYPTO_MBXLOCKOUT_MBX2LOCKOUT_M                          0x0000FF00U
#define HSMCRYPTO_MBXLOCKOUT_MBX2LOCKOUT_S                                   8U

// Field:   [7:0] MBX1LOCKOUT
//
// Bit map indicates which Hosts are blocked from accessing mailbox
#define HSMCRYPTO_MBXLOCKOUT_MBX1LOCKOUT_W                                   8U
#define HSMCRYPTO_MBXLOCKOUT_MBX1LOCKOUT_M                          0x000000FFU
#define HSMCRYPTO_MBXLOCKOUT_MBX1LOCKOUT_S                                   0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MODULESTATUS
//
//*****************************************************************************
// Field:    [31] FATALERROR
//
// Read-Only. Set if fatal error occured
#define HSMCRYPTO_MODULESTATUS_FATALERROR                           0x80000000U
#define HSMCRYPTO_MODULESTATUS_FATALERROR_M                         0x80000000U
#define HSMCRYPTO_MODULESTATUS_FATALERROR_S                                 31U

// Field:    [23] FWIMGACCEPTED
//
// Read-Only. Set if firmware is to be executed
#define HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED                        0x00800000U
#define HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED_M                      0x00800000U
#define HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED_S                              23U

// Field:    [22] FWIMGCHKDONE
//
// Read-Only. Set if firmware checks complete
#define HSMCRYPTO_MODULESTATUS_FWIMGCHKDONE                         0x00400000U
#define HSMCRYPTO_MODULESTATUS_FWIMGCHKDONE_M                       0x00400000U
#define HSMCRYPTO_MODULESTATUS_FWIMGCHKDONE_S                               22U

// Field:    [10] CRC24ERROR
//
// Read-Only. Set if CRC on ProgramROM is fails
#define HSMCRYPTO_MODULESTATUS_CRC24ERROR                           0x00000400U
#define HSMCRYPTO_MODULESTATUS_CRC24ERROR_M                         0x00000400U
#define HSMCRYPTO_MODULESTATUS_CRC24ERROR_S                                 10U

// Field:     [9] CRC24OK
//
// Read-Only. Set if CRC on ProgramROM is passes
#define HSMCRYPTO_MODULESTATUS_CRC24OK                              0x00000200U
#define HSMCRYPTO_MODULESTATUS_CRC24OK_M                            0x00000200U
#define HSMCRYPTO_MODULESTATUS_CRC24OK_S                                     9U

// Field:     [8] CRC24BUSY
//
// Read-Only. Set if CRC on ProgramROM is busy
#define HSMCRYPTO_MODULESTATUS_CRC24BUSY                            0x00000100U
#define HSMCRYPTO_MODULESTATUS_CRC24BUSY_M                          0x00000100U
#define HSMCRYPTO_MODULESTATUS_CRC24BUSY_S                                   8U

// Field:     [1] NONFIPSMODE
//
// Read-Only. Set if VaultIP is in non-FIPS mode
#define HSMCRYPTO_MODULESTATUS_NONFIPSMODE                          0x00000002U
#define HSMCRYPTO_MODULESTATUS_NONFIPSMODE_M                        0x00000002U
#define HSMCRYPTO_MODULESTATUS_NONFIPSMODE_S                                 1U

// Field:     [0] FIPSMODE
//
// Read-Only. Set if VaultIP is in FIPS mode
#define HSMCRYPTO_MODULESTATUS_FIPSMODE                             0x00000001U
#define HSMCRYPTO_MODULESTATUS_FIPSMODE_M                           0x00000001U
#define HSMCRYPTO_MODULESTATUS_FIPSMODE_S                                    0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_OPTIONS2
//
//*****************************************************************************
// Field:    [25] ADDCE10
//
// Set - an additional crypto engine is available in hardware as custom
// engine10
#define HSMCRYPTO_OPTIONS2_ADDCE10                                  0x02000000U
#define HSMCRYPTO_OPTIONS2_ADDCE10_M                                0x02000000U
#define HSMCRYPTO_OPTIONS2_ADDCE10_S                                        25U

// Field:    [24] ADDCE9
//
// Set - an additional crypto engine is available in hardware as custom engine9
#define HSMCRYPTO_OPTIONS2_ADDCE9                                   0x01000000U
#define HSMCRYPTO_OPTIONS2_ADDCE9_M                                 0x01000000U
#define HSMCRYPTO_OPTIONS2_ADDCE9_S                                         24U

// Field:    [23] ADDCE8
//
// Set - an additional crypto engine is available in hardware as custom engine8
#define HSMCRYPTO_OPTIONS2_ADDCE8                                   0x00800000U
#define HSMCRYPTO_OPTIONS2_ADDCE8_M                                 0x00800000U
#define HSMCRYPTO_OPTIONS2_ADDCE8_S                                         23U

// Field:    [22] ADDCE7
//
// Set - an additional crypto engine is available in hardware as custom engine7
#define HSMCRYPTO_OPTIONS2_ADDCE7                                   0x00400000U
#define HSMCRYPTO_OPTIONS2_ADDCE7_M                                 0x00400000U
#define HSMCRYPTO_OPTIONS2_ADDCE7_S                                         22U

// Field:    [21] ADDCE6
//
// Set - an additional crypto engine is available in hardware as custom engine6
#define HSMCRYPTO_OPTIONS2_ADDCE6                                   0x00200000U
#define HSMCRYPTO_OPTIONS2_ADDCE6_M                                 0x00200000U
#define HSMCRYPTO_OPTIONS2_ADDCE6_S                                         21U

// Field:    [20] ADDCE5
//
// Set - an additional crypto engine is available in hardware as custom engine5
#define HSMCRYPTO_OPTIONS2_ADDCE5                                   0x00100000U
#define HSMCRYPTO_OPTIONS2_ADDCE5_M                                 0x00100000U
#define HSMCRYPTO_OPTIONS2_ADDCE5_S                                         20U

// Field:    [19] ADDCE4
//
// Set - an additional crypto engine is available in hardware as custom engine4
#define HSMCRYPTO_OPTIONS2_ADDCE4                                   0x00080000U
#define HSMCRYPTO_OPTIONS2_ADDCE4_M                                 0x00080000U
#define HSMCRYPTO_OPTIONS2_ADDCE4_S                                         19U

// Field:    [18] ADDCE3
//
// Set - an additional crypto engine is available in hardware as custom engine3
#define HSMCRYPTO_OPTIONS2_ADDCE3                                   0x00040000U
#define HSMCRYPTO_OPTIONS2_ADDCE3_M                                 0x00040000U
#define HSMCRYPTO_OPTIONS2_ADDCE3_S                                         18U

// Field:    [17] ADDCE2
//
// Set - an additional crypto engine is available in hardware as custom engine2
#define HSMCRYPTO_OPTIONS2_ADDCE2                                   0x00020000U
#define HSMCRYPTO_OPTIONS2_ADDCE2_M                                 0x00020000U
#define HSMCRYPTO_OPTIONS2_ADDCE2_S                                         17U

// Field:    [16] ADDCE1
//
// Set - an additional crypto engine is available in hardware as custom engine1
#define HSMCRYPTO_OPTIONS2_ADDCE1                                   0x00010000U
#define HSMCRYPTO_OPTIONS2_ADDCE1_M                                 0x00010000U
#define HSMCRYPTO_OPTIONS2_ADDCE1_S                                         16U

// Field:    [12] BUSIFC
//
// Bus interface type, for both Master and Slave: 0b = 32-bit AHB, 1b = 32-bit
// AXI
#define HSMCRYPTO_OPTIONS2_BUSIFC                                   0x00001000U
#define HSMCRYPTO_OPTIONS2_BUSIFC_M                                 0x00001000U
#define HSMCRYPTO_OPTIONS2_BUSIFC_S                                         12U

// Field:     [9] PROGRAMRAM
//
// 1b = downloadable RAM based firmware program memory. 0b = ROM only firmware
// program memory.
#define HSMCRYPTO_OPTIONS2_PROGRAMRAM                               0x00000200U
#define HSMCRYPTO_OPTIONS2_PROGRAMRAM_M                             0x00000200U
#define HSMCRYPTO_OPTIONS2_PROGRAMRAM_S                                      9U

// Field:     [8] CCPU
//
// C capable local cpu available
#define HSMCRYPTO_OPTIONS2_CCPU                                     0x00000100U
#define HSMCRYPTO_OPTIONS2_CCPU_M                                   0x00000100U
#define HSMCRYPTO_OPTIONS2_CCPU_S                                            8U

// Field:     [5] PKCP
//
// PKCP Engine available
#define HSMCRYPTO_OPTIONS2_PKCP                                     0x00000020U
#define HSMCRYPTO_OPTIONS2_PKCP_M                                   0x00000020U
#define HSMCRYPTO_OPTIONS2_PKCP_S                                            5U

// Field:     [4] CRC
//
// CRC calculation available
#define HSMCRYPTO_OPTIONS2_CRC                                      0x00000010U
#define HSMCRYPTO_OPTIONS2_CRC_M                                    0x00000010U
#define HSMCRYPTO_OPTIONS2_CRC_S                                             4U

// Field:     [3] TRNG
//
// Set - TRNG engine available
#define HSMCRYPTO_OPTIONS2_TRNG                                     0x00000008U
#define HSMCRYPTO_OPTIONS2_TRNG_M                                   0x00000008U
#define HSMCRYPTO_OPTIONS2_TRNG_S                                            3U

// Field:     [2] SHA
//
// Set - SHA1/SHA2 combination core available
#define HSMCRYPTO_OPTIONS2_SHA                                      0x00000004U
#define HSMCRYPTO_OPTIONS2_SHA_M                                    0x00000004U
#define HSMCRYPTO_OPTIONS2_SHA_S                                             2U

// Field:     [0] DESAES
//
// Set - (3)DES/AES combination crypto core available
#define HSMCRYPTO_OPTIONS2_DESAES                                   0x00000001U
#define HSMCRYPTO_OPTIONS2_DESAES_M                                 0x00000001U
#define HSMCRYPTO_OPTIONS2_DESAES_S                                          0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_OPTIONS
//
//*****************************************************************************
// Field: [31:24] HOSTIDSEC
//
// Bits to indicate which of the 8 possible cpu_id codes on the bus interface
// are active Hosts with secure access
#define HSMCRYPTO_OPTIONS_HOSTIDSEC_W                                        8U
#define HSMCRYPTO_OPTIONS_HOSTIDSEC_M                               0xFF000000U
#define HSMCRYPTO_OPTIONS_HOSTIDSEC_S                                       24U

// Field:    [23] MYIDSEC
//
// Indicates the current protection bit values of the Host actually reading the
// register
#define HSMCRYPTO_OPTIONS_MYIDSEC                                   0x00800000U
#define HSMCRYPTO_OPTIONS_MYIDSEC_M                                 0x00800000U
#define HSMCRYPTO_OPTIONS_MYIDSEC_S                                         23U

// Field: [22:20] MYID
//
// Slave & Master interface support protection bit (secure/non-secure) accesses
#define HSMCRYPTO_OPTIONS_MYID_W                                             3U
#define HSMCRYPTO_OPTIONS_MYID_M                                    0x00700000U
#define HSMCRYPTO_OPTIONS_MYID_S                                            20U

// Field: [18:16] MASTERID
//
// Value of the cpu_id that designates the Master Host
#define HSMCRYPTO_OPTIONS_MASTERID_W                                         3U
#define HSMCRYPTO_OPTIONS_MASTERID_M                                0x00070000U
#define HSMCRYPTO_OPTIONS_MASTERID_S                                        16U

// Field:  [15:8] HOSTID
//
// Bits to indicate which of the 8 possible cpu_id codes on the bus interface
// are active
#define HSMCRYPTO_OPTIONS_HOSTID_W                                           8U
#define HSMCRYPTO_OPTIONS_HOSTID_M                                  0x0000FF00U
#define HSMCRYPTO_OPTIONS_HOSTID_S                                           8U

// Field:   [5:4] MBXSIZE
//
// Implemented size of Mailbox pairs - 00b-128bytes, 01b-256bytes,
// 10b-512bytes, 11b-1Kbyte
#define HSMCRYPTO_OPTIONS_MBXSIZE_W                                          2U
#define HSMCRYPTO_OPTIONS_MBXSIZE_M                                 0x00000030U
#define HSMCRYPTO_OPTIONS_MBXSIZE_S                                          4U

// Field:   [3:0] MAILBOXES
//
// Number of Input/Output Mailbox pairs
#define HSMCRYPTO_OPTIONS_MAILBOXES_W                                        4U
#define HSMCRYPTO_OPTIONS_MAILBOXES_M                               0x0000000FU
#define HSMCRYPTO_OPTIONS_MAILBOXES_S                                        0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_VERSION
//
//*****************************************************************************
// Field: [27:24] MAJORVER
//
// Major Version release number for this module
#define HSMCRYPTO_VERSION_MAJORVER_W                                         4U
#define HSMCRYPTO_VERSION_MAJORVER_M                                0x0F000000U
#define HSMCRYPTO_VERSION_MAJORVER_S                                        24U

// Field: [23:20] MINORVER
//
// Minor Version release number for this module
#define HSMCRYPTO_VERSION_MINORVER_W                                         4U
#define HSMCRYPTO_VERSION_MINORVER_M                                0x00F00000U
#define HSMCRYPTO_VERSION_MINORVER_S                                        20U

// Field: [19:16] PATCHLEVEL
//
// Hardware Patch Level for this module
#define HSMCRYPTO_VERSION_PATCHLEVEL_W                                       4U
#define HSMCRYPTO_VERSION_PATCHLEVEL_M                              0x000F0000U
#define HSMCRYPTO_VERSION_PATCHLEVEL_S                                      16U

// Field:  [15:8] NUMBERCMPL
//
// Bit by Bit compliment of IP Number
#define HSMCRYPTO_VERSION_NUMBERCMPL_W                                       8U
#define HSMCRYPTO_VERSION_NUMBERCMPL_M                              0x0000FF00U
#define HSMCRYPTO_VERSION_NUMBERCMPL_S                                       8U

// Field:   [7:0] NUMBER
//
// IP number
#define HSMCRYPTO_VERSION_NUMBER_W                                           8U
#define HSMCRYPTO_VERSION_NUMBER_M                                  0x000000FFU
#define HSMCRYPTO_VERSION_NUMBER_S                                           0U


#endif // __HSMCRYPTO__
