/******************************************************************************
*  Filename:       hw_hsmcrypto_h
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

#ifndef __HW_HSMCRYPTO_H__
#define __HW_HSMCRYPTO_H__

//*****************************************************************************
//
// This section defines the register offsets of
// HSMCRYPTO component
//
//*****************************************************************************
// Input Mailbox 1
#define HSMCRYPTO_O_MB1IN                                           0x00000000U

// Output Mailbox 1
#define HSMCRYPTO_O_MB1OUT                                          0x00000000U

// Input Mailbox 2
#define HSMCRYPTO_O_MB2IN                                           0x00000400U

// Output Mailbox 2
#define HSMCRYPTO_O_MB2OUT                                          0x00000400U

// AIC Polarity Control Register
#define HSMCRYPTO_O_AICPOL                                          0x00003E00U

// AIC Type Control Register
#define HSMCRYPTO_O_AICTYPE                                         0x00003E04U

// AIC Enable Control Register
#define HSMCRYPTO_O_AICEN                                           0x00003E08U

// AIC Raw Source Status Register
#define HSMCRYPTO_O_AICRSTA                                         0x00003E0CU

// AIC Enable Set Registers
#define HSMCRYPTO_O_AICENSET                                        0x00003E0CU

// AIC Enabled Status Register
#define HSMCRYPTO_O_AICENSTA                                        0x00003E10U

// AIC Acknowledge Register
#define HSMCRYPTO_O_AICACK                                          0x00003E10U

// AIC Enable Clear Register
#define HSMCRYPTO_O_AICENCLR                                        0x00003E14U

// AIC Options Register
#define HSMCRYPTO_O_AICOPT                                          0x00003E18U

// AIC Version Register
#define HSMCRYPTO_O_AICVER                                          0x00003E1CU

// Mailbox Status Register
#define HSMCRYPTO_O_MBSTA                                           0x00003F00U

// Mailbox Control Register
#define HSMCRYPTO_O_MBCTL                                           0x00003F00U

// Raw Mailbox Status Register
#define HSMCRYPTO_O_MBRSTA                                          0x00003F04U

// Mailbox Reset Register. Only Master Host can write into this register
#define HSMCRYPTO_O_MBRST                                           0x00003F04U

// Mailbox Status - linked Host IDs Register
#define HSMCRYPTO_O_MBLNKID                                         0x00003F08U

// Mailbox Status - output Host IDs Register
#define HSMCRYPTO_O_MBOUTID                                         0x00003F0CU

// Host or Mailbox lockout control Register
#define HSMCRYPTO_O_MBLCKOUT                                        0x00003F10U

// Module Status Register
#define HSMCRYPTO_O_MODSTA                                          0x00003FE0U

// Configured options(2)
#define HSMCRYPTO_O_OPTIONS2                                        0x00003FF4U

// Configured options(1)
#define HSMCRYPTO_O_OPTIONS                                         0x00003FF8U

// Version register
#define HSMCRYPTO_O_VERSION                                         0x00003FFCU

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MB1IN
//
//*****************************************************************************
// Field:  [31:0] MEM
//
// Input Mailbox Memory
#define HSMCRYPTO_MB1IN_MEM_W                                               32U
#define HSMCRYPTO_MB1IN_MEM_M                                       0xFFFFFFFFU
#define HSMCRYPTO_MB1IN_MEM_S                                                0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MB1OUT
//
//*****************************************************************************
// Field:  [31:0] MEM
//
// Output Mailbox Memory
#define HSMCRYPTO_MB1OUT_MEM_W                                              32U
#define HSMCRYPTO_MB1OUT_MEM_M                                      0xFFFFFFFFU
#define HSMCRYPTO_MB1OUT_MEM_S                                               0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MB2IN
//
//*****************************************************************************
// Field:  [31:0] MEM
//
// Input Mailbox Memory
#define HSMCRYPTO_MB2IN_MEM_W                                               32U
#define HSMCRYPTO_MB2IN_MEM_M                                       0xFFFFFFFFU
#define HSMCRYPTO_MB2IN_MEM_S                                                0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MB2OUT
//
//*****************************************************************************
// Field:  [31:0] MEM
//
// Output Mailbox Memory
#define HSMCRYPTO_MB2OUT_MEM_W                                              32U
#define HSMCRYPTO_MB2OUT_MEM_M                                      0xFFFFFFFFU
#define HSMCRYPTO_MB2OUT_MEM_S                                               0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICPOL
//
//*****************************************************************************
// Field:     [4] MBLNKABL
//
// Mailbox Linkable Interrupt Polarity
// ENUMs:
// HIGHRISE                 High level/rising edge
// LOWFALL                  Low level/falling edge
#define HSMCRYPTO_AICPOL_MBLNKABL                                   0x00000010U
#define HSMCRYPTO_AICPOL_MBLNKABL_M                                 0x00000010U
#define HSMCRYPTO_AICPOL_MBLNKABL_S                                          4U
#define HSMCRYPTO_AICPOL_MBLNKABL_HIGHRISE                          0x00000010U
#define HSMCRYPTO_AICPOL_MBLNKABL_LOWFALL                           0x00000000U

// Field:     [3] MB2DONE
//
// Mailbox 2 Token Done Interrupt Polarity
// ENUMs:
// HIGHRISE                 High level/rising edge
// LOWFALL                  Low level/falling edge
#define HSMCRYPTO_AICPOL_MB2DONE                                    0x00000008U
#define HSMCRYPTO_AICPOL_MB2DONE_M                                  0x00000008U
#define HSMCRYPTO_AICPOL_MB2DONE_S                                           3U
#define HSMCRYPTO_AICPOL_MB2DONE_HIGHRISE                           0x00000008U
#define HSMCRYPTO_AICPOL_MB2DONE_LOWFALL                            0x00000000U

// Field:     [2] MB2FREE
//
// Mailbox 2 Free Interrupt Polarity
// ENUMs:
// HIGHRISE                 High level/rising edge
// LOWFALL                  Low level/falling edge
#define HSMCRYPTO_AICPOL_MB2FREE                                    0x00000004U
#define HSMCRYPTO_AICPOL_MB2FREE_M                                  0x00000004U
#define HSMCRYPTO_AICPOL_MB2FREE_S                                           2U
#define HSMCRYPTO_AICPOL_MB2FREE_HIGHRISE                           0x00000004U
#define HSMCRYPTO_AICPOL_MB2FREE_LOWFALL                            0x00000000U

// Field:     [1] MB1DONE
//
// Mailbox 1 Token done Interrupt Polarity
// ENUMs:
// HIGHRISE                 High level/rising edge
// LOWFALL                  Low level/falling edge
#define HSMCRYPTO_AICPOL_MB1DONE                                    0x00000002U
#define HSMCRYPTO_AICPOL_MB1DONE_M                                  0x00000002U
#define HSMCRYPTO_AICPOL_MB1DONE_S                                           1U
#define HSMCRYPTO_AICPOL_MB1DONE_HIGHRISE                           0x00000002U
#define HSMCRYPTO_AICPOL_MB1DONE_LOWFALL                            0x00000000U

// Field:     [0] MB1FREE
//
// Mailbox 1 Free Interrupt Polarity
// ENUMs:
// HIGHRISE                 High level/rising edge
// LOWFALL                  Low level/falling edge
#define HSMCRYPTO_AICPOL_MB1FREE                                    0x00000001U
#define HSMCRYPTO_AICPOL_MB1FREE_M                                  0x00000001U
#define HSMCRYPTO_AICPOL_MB1FREE_S                                           0U
#define HSMCRYPTO_AICPOL_MB1FREE_HIGHRISE                           0x00000001U
#define HSMCRYPTO_AICPOL_MB1FREE_LOWFALL                            0x00000000U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICTYPE
//
//*****************************************************************************
// Field:     [4] MBLNKABL
//
// Mailbox Linkable Interrupt Type
// ENUMs:
// EDG                      Edge
// LVL                      Level
#define HSMCRYPTO_AICTYPE_MBLNKABL                                  0x00000010U
#define HSMCRYPTO_AICTYPE_MBLNKABL_M                                0x00000010U
#define HSMCRYPTO_AICTYPE_MBLNKABL_S                                         4U
#define HSMCRYPTO_AICTYPE_MBLNKABL_EDG                              0x00000010U
#define HSMCRYPTO_AICTYPE_MBLNKABL_LVL                              0x00000000U

// Field:     [3] MB2DONE
//
// Mailbox 2 Token Done Interrupt Type
// ENUMs:
// EDG                      Edge
// LEVEL                    Level
#define HSMCRYPTO_AICTYPE_MB2DONE                                   0x00000008U
#define HSMCRYPTO_AICTYPE_MB2DONE_M                                 0x00000008U
#define HSMCRYPTO_AICTYPE_MB2DONE_S                                          3U
#define HSMCRYPTO_AICTYPE_MB2DONE_EDG                               0x00000008U
#define HSMCRYPTO_AICTYPE_MB2DONE_LEVEL                             0x00000000U

// Field:     [2] MB2FREE
//
// Mailbox 2 Free Interrupt Type
// ENUMs:
// EDG                      Edge
// LVL                      Level
#define HSMCRYPTO_AICTYPE_MB2FREE                                   0x00000004U
#define HSMCRYPTO_AICTYPE_MB2FREE_M                                 0x00000004U
#define HSMCRYPTO_AICTYPE_MB2FREE_S                                          2U
#define HSMCRYPTO_AICTYPE_MB2FREE_EDG                               0x00000004U
#define HSMCRYPTO_AICTYPE_MB2FREE_LVL                               0x00000000U

// Field:     [1] MB1DONE
//
// Mailbox 1 Token Done Interrupt Type
// ENUMs:
// EDG                      Edge
// LVL                      Level
#define HSMCRYPTO_AICTYPE_MB1DONE                                   0x00000002U
#define HSMCRYPTO_AICTYPE_MB1DONE_M                                 0x00000002U
#define HSMCRYPTO_AICTYPE_MB1DONE_S                                          1U
#define HSMCRYPTO_AICTYPE_MB1DONE_EDG                               0x00000002U
#define HSMCRYPTO_AICTYPE_MB1DONE_LVL                               0x00000000U

// Field:     [0] MB1FREE
//
// Mailbox 1 Free Interrupt Type
// ENUMs:
// EDG                      Edge
// LVL                      Level
#define HSMCRYPTO_AICTYPE_MB1FREE                                   0x00000001U
#define HSMCRYPTO_AICTYPE_MB1FREE_M                                 0x00000001U
#define HSMCRYPTO_AICTYPE_MB1FREE_S                                          0U
#define HSMCRYPTO_AICTYPE_MB1FREE_EDG                               0x00000001U
#define HSMCRYPTO_AICTYPE_MB1FREE_LVL                               0x00000000U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICEN
//
//*****************************************************************************
// Field:     [4] MBLNKABL
//
// Mailbox Linkable Interrupt Enable. Indicates one or more mailbox can be
// linked
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define HSMCRYPTO_AICEN_MBLNKABL                                    0x00000010U
#define HSMCRYPTO_AICEN_MBLNKABL_M                                  0x00000010U
#define HSMCRYPTO_AICEN_MBLNKABL_S                                           4U
#define HSMCRYPTO_AICEN_MBLNKABL_EN                                 0x00000010U
#define HSMCRYPTO_AICEN_MBLNKABL_DIS                                0x00000000U

// Field:     [3] MB2DONE
//
// Mailbox 2 Token Done Interrupt Enable
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define HSMCRYPTO_AICEN_MB2DONE                                     0x00000008U
#define HSMCRYPTO_AICEN_MB2DONE_M                                   0x00000008U
#define HSMCRYPTO_AICEN_MB2DONE_S                                            3U
#define HSMCRYPTO_AICEN_MB2DONE_EN                                  0x00000008U
#define HSMCRYPTO_AICEN_MB2DONE_DIS                                 0x00000000U

// Field:     [2] MB2FREE
//
// Mailbox 2 Free Interrupt Enable
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define HSMCRYPTO_AICEN_MB2FREE                                     0x00000004U
#define HSMCRYPTO_AICEN_MB2FREE_M                                   0x00000004U
#define HSMCRYPTO_AICEN_MB2FREE_S                                            2U
#define HSMCRYPTO_AICEN_MB2FREE_EN                                  0x00000004U
#define HSMCRYPTO_AICEN_MB2FREE_DIS                                 0x00000000U

// Field:     [1] MB1DONE
//
// Mailbox 1 Token done Interrupt Enable
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define HSMCRYPTO_AICEN_MB1DONE                                     0x00000002U
#define HSMCRYPTO_AICEN_MB1DONE_M                                   0x00000002U
#define HSMCRYPTO_AICEN_MB1DONE_S                                            1U
#define HSMCRYPTO_AICEN_MB1DONE_EN                                  0x00000002U
#define HSMCRYPTO_AICEN_MB1DONE_DIS                                 0x00000000U

// Field:     [0] MB1FREE
//
// Mailbox 1 Free Interrupt Enable
// ENUMs:
// EN                       Enable
// DIS                      Disable
#define HSMCRYPTO_AICEN_MB1FREE                                     0x00000001U
#define HSMCRYPTO_AICEN_MB1FREE_M                                   0x00000001U
#define HSMCRYPTO_AICEN_MB1FREE_S                                            0U
#define HSMCRYPTO_AICEN_MB1FREE_EN                                  0x00000001U
#define HSMCRYPTO_AICEN_MB1FREE_DIS                                 0x00000000U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICRSTA
//
//*****************************************************************************
// Field:     [4] MBLNKABL
//
// Mailbox Linkable Interrupt Status. Indicates one or more mailbox can be
// linked
#define HSMCRYPTO_AICRSTA_MBLNKABL                                  0x00000010U
#define HSMCRYPTO_AICRSTA_MBLNKABL_M                                0x00000010U
#define HSMCRYPTO_AICRSTA_MBLNKABL_S                                         4U

// Field:     [3] MB2DONE
//
// Mailbox 2 Token Done Interrupt Status
#define HSMCRYPTO_AICRSTA_MB2DONE                                   0x00000008U
#define HSMCRYPTO_AICRSTA_MB2DONE_M                                 0x00000008U
#define HSMCRYPTO_AICRSTA_MB2DONE_S                                          3U

// Field:     [2] MB2FREE
//
// Mailbox 2 Free Interrupt Status
#define HSMCRYPTO_AICRSTA_MB2FREE                                   0x00000004U
#define HSMCRYPTO_AICRSTA_MB2FREE_M                                 0x00000004U
#define HSMCRYPTO_AICRSTA_MB2FREE_S                                          2U

// Field:     [1] MB1DONE
//
// Mailbox 1 Token done Interrupt Status
#define HSMCRYPTO_AICRSTA_MB1DONE                                   0x00000002U
#define HSMCRYPTO_AICRSTA_MB1DONE_M                                 0x00000002U
#define HSMCRYPTO_AICRSTA_MB1DONE_S                                          1U

// Field:     [0] MB1FREE
//
// Mailbox 1 Free Interrupt Status
#define HSMCRYPTO_AICRSTA_MB1FREE                                   0x00000001U
#define HSMCRYPTO_AICRSTA_MB1FREE_M                                 0x00000001U
#define HSMCRYPTO_AICRSTA_MB1FREE_S                                          0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICENSET
//
//*****************************************************************************
// Field:     [4] MBLNKABL
//
// Mailbox Linkable Interrupt Enable Set
// ENUMs:
// SET                      Set Enable
#define HSMCRYPTO_AICENSET_MBLNKABL                                 0x00000010U
#define HSMCRYPTO_AICENSET_MBLNKABL_M                               0x00000010U
#define HSMCRYPTO_AICENSET_MBLNKABL_S                                        4U
#define HSMCRYPTO_AICENSET_MBLNKABL_SET                             0x00000010U

// Field:     [3] MB2DONE
//
// MailBox 2 Token Done Interrupt Enable Set
// ENUMs:
// SET                      Set Enable
#define HSMCRYPTO_AICENSET_MB2DONE                                  0x00000008U
#define HSMCRYPTO_AICENSET_MB2DONE_M                                0x00000008U
#define HSMCRYPTO_AICENSET_MB2DONE_S                                         3U
#define HSMCRYPTO_AICENSET_MB2DONE_SET                              0x00000008U

// Field:     [2] MB2FREE
//
// MailBox 2 Free Interrupt Enable Set
// ENUMs:
// SET                      Set Enable
#define HSMCRYPTO_AICENSET_MB2FREE                                  0x00000004U
#define HSMCRYPTO_AICENSET_MB2FREE_M                                0x00000004U
#define HSMCRYPTO_AICENSET_MB2FREE_S                                         2U
#define HSMCRYPTO_AICENSET_MB2FREE_SET                              0x00000004U

// Field:     [1] MB1DONE
//
// MailBox 1 Token Done Interrupt Enable Set
// ENUMs:
// SET                      Set Enable
#define HSMCRYPTO_AICENSET_MB1DONE                                  0x00000002U
#define HSMCRYPTO_AICENSET_MB1DONE_M                                0x00000002U
#define HSMCRYPTO_AICENSET_MB1DONE_S                                         1U
#define HSMCRYPTO_AICENSET_MB1DONE_SET                              0x00000002U

// Field:     [0] MB1FREE
//
// MailBox 1 Free Interrupt Enable Set
// ENUMs:
// SET                      Set Enable
#define HSMCRYPTO_AICENSET_MB1FREE                                  0x00000001U
#define HSMCRYPTO_AICENSET_MB1FREE_M                                0x00000001U
#define HSMCRYPTO_AICENSET_MB1FREE_S                                         0U
#define HSMCRYPTO_AICENSET_MB1FREE_SET                              0x00000001U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICENSTA
//
//*****************************************************************************
// Field:     [4] MBLNKABL
//
// Mailbox Linkable Interrupt Enable Status
#define HSMCRYPTO_AICENSTA_MBLNKABL                                 0x00000010U
#define HSMCRYPTO_AICENSTA_MBLNKABL_M                               0x00000010U
#define HSMCRYPTO_AICENSTA_MBLNKABL_S                                        4U

// Field:     [3] MB2DONE
//
// Mailbox 2 Done Interrupt Enable Status
#define HSMCRYPTO_AICENSTA_MB2DONE                                  0x00000008U
#define HSMCRYPTO_AICENSTA_MB2DONE_M                                0x00000008U
#define HSMCRYPTO_AICENSTA_MB2DONE_S                                         3U

// Field:     [2] MB2FREE
//
// MailBox 2 Free Interrupt Enable Status
#define HSMCRYPTO_AICENSTA_MB2FREE                                  0x00000004U
#define HSMCRYPTO_AICENSTA_MB2FREE_M                                0x00000004U
#define HSMCRYPTO_AICENSTA_MB2FREE_S                                         2U

// Field:     [1] MB1DONE
//
// MailBox 1 Token Done Interrupt Enable Status
#define HSMCRYPTO_AICENSTA_MB1DONE                                  0x00000002U
#define HSMCRYPTO_AICENSTA_MB1DONE_M                                0x00000002U
#define HSMCRYPTO_AICENSTA_MB1DONE_S                                         1U

// Field:     [0] MB1FREE
//
// Mailbox 1 Free Interrupt Enable Status
#define HSMCRYPTO_AICENSTA_MB1FREE                                  0x00000001U
#define HSMCRYPTO_AICENSTA_MB1FREE_M                                0x00000001U
#define HSMCRYPTO_AICENSTA_MB1FREE_S                                         0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICACK
//
//*****************************************************************************
// Field:     [4] MBLNKABL
//
// Mailbox Linkable Interrupt Acknowledge
// ENUMs:
// ACK                      Interrupt Ack
#define HSMCRYPTO_AICACK_MBLNKABL                                   0x00000010U
#define HSMCRYPTO_AICACK_MBLNKABL_M                                 0x00000010U
#define HSMCRYPTO_AICACK_MBLNKABL_S                                          4U
#define HSMCRYPTO_AICACK_MBLNKABL_ACK                               0x00000010U

// Field:     [3] MB2DONE
//
// Mailbox 2 Done Interrupt Acknowledge
// ENUMs:
// ACK                      Interrupt Ack
#define HSMCRYPTO_AICACK_MB2DONE                                    0x00000008U
#define HSMCRYPTO_AICACK_MB2DONE_M                                  0x00000008U
#define HSMCRYPTO_AICACK_MB2DONE_S                                           3U
#define HSMCRYPTO_AICACK_MB2DONE_ACK                                0x00000008U

// Field:     [2] MB2FREE
//
// MailBox 2 Free Interrupt Acknowledge
// ENUMs:
// ACK                      Interrupt Ack
#define HSMCRYPTO_AICACK_MB2FREE                                    0x00000004U
#define HSMCRYPTO_AICACK_MB2FREE_M                                  0x00000004U
#define HSMCRYPTO_AICACK_MB2FREE_S                                           2U
#define HSMCRYPTO_AICACK_MB2FREE_ACK                                0x00000004U

// Field:     [1] MB1DONE
//
// MailBox 1 Token Done Interrupt Acknowledge
// ENUMs:
// ACK                      Interrupt Ack
#define HSMCRYPTO_AICACK_MB1DONE                                    0x00000002U
#define HSMCRYPTO_AICACK_MB1DONE_M                                  0x00000002U
#define HSMCRYPTO_AICACK_MB1DONE_S                                           1U
#define HSMCRYPTO_AICACK_MB1DONE_ACK                                0x00000002U

// Field:     [0] MB1FREE
//
// Mailbox 1 Free Interrupt Acknowledge
// ENUMs:
// ACK                      Interrupt Ack
#define HSMCRYPTO_AICACK_MB1FREE                                    0x00000001U
#define HSMCRYPTO_AICACK_MB1FREE_M                                  0x00000001U
#define HSMCRYPTO_AICACK_MB1FREE_S                                           0U
#define HSMCRYPTO_AICACK_MB1FREE_ACK                                0x00000001U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICENCLR
//
//*****************************************************************************
// Field:     [4] MBLNKABL
//
// Mailbox Linkable Interrupt Enable Clear
// ENUMs:
// CLR                      Clear Enable
#define HSMCRYPTO_AICENCLR_MBLNKABL                                 0x00000010U
#define HSMCRYPTO_AICENCLR_MBLNKABL_M                               0x00000010U
#define HSMCRYPTO_AICENCLR_MBLNKABL_S                                        4U
#define HSMCRYPTO_AICENCLR_MBLNKABL_CLR                             0x00000010U

// Field:     [3] MB2DONE
//
// Mailbox 2 Done Interrupt Enable Clear
// ENUMs:
// CLR                      Clear Enable
#define HSMCRYPTO_AICENCLR_MB2DONE                                  0x00000008U
#define HSMCRYPTO_AICENCLR_MB2DONE_M                                0x00000008U
#define HSMCRYPTO_AICENCLR_MB2DONE_S                                         3U
#define HSMCRYPTO_AICENCLR_MB2DONE_CLR                              0x00000008U

// Field:     [2] MB2FREE
//
// MailBox 2 Free Interrupt Enable Clear
// ENUMs:
// CLR                      Clear Enable
#define HSMCRYPTO_AICENCLR_MB2FREE                                  0x00000004U
#define HSMCRYPTO_AICENCLR_MB2FREE_M                                0x00000004U
#define HSMCRYPTO_AICENCLR_MB2FREE_S                                         2U
#define HSMCRYPTO_AICENCLR_MB2FREE_CLR                              0x00000004U

// Field:     [1] MB1DONE
//
// MailBox 1 Token Done Interrupt Enable Clear
// ENUMs:
// CLR                      Clear Enable
#define HSMCRYPTO_AICENCLR_MB1DONE                                  0x00000002U
#define HSMCRYPTO_AICENCLR_MB1DONE_M                                0x00000002U
#define HSMCRYPTO_AICENCLR_MB1DONE_S                                         1U
#define HSMCRYPTO_AICENCLR_MB1DONE_CLR                              0x00000002U

// Field:     [0] MB1FREE
//
// Mailbox 1 Free Interrupt Enable Clear
// ENUMs:
// CLR                      Clear Enable
#define HSMCRYPTO_AICENCLR_MB1FREE                                  0x00000001U
#define HSMCRYPTO_AICENCLR_MB1FREE_M                                0x00000001U
#define HSMCRYPTO_AICENCLR_MB1FREE_S                                         0U
#define HSMCRYPTO_AICENCLR_MB1FREE_CLR                              0x00000001U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICOPT
//
//*****************************************************************************
// Field:     [8] MINRMAP
//
// Mini register map.
#define HSMCRYPTO_AICOPT_MINRMAP                                    0x00000100U
#define HSMCRYPTO_AICOPT_MINRMAP_M                                  0x00000100U
#define HSMCRYPTO_AICOPT_MINRMAP_S                                           8U

// Field:     [7] EXTRMAP
//
// Extended register map.
#define HSMCRYPTO_AICOPT_EXTRMAP                                    0x00000080U
#define HSMCRYPTO_AICOPT_EXTRMAP_M                                  0x00000080U
#define HSMCRYPTO_AICOPT_EXTRMAP_S                                           7U

// Field:   [5:0] INPUTS
//
// The number of interrupt request inputs.
#define HSMCRYPTO_AICOPT_INPUTS_W                                            6U
#define HSMCRYPTO_AICOPT_INPUTS_M                                   0x0000003FU
#define HSMCRYPTO_AICOPT_INPUTS_S                                            0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_AICVER
//
//*****************************************************************************
// Field: [27:24] MAJORVER
//
// These bits encode the major version number for the AIC module.
#define HSMCRYPTO_AICVER_MAJORVER_W                                          4U
#define HSMCRYPTO_AICVER_MAJORVER_M                                 0x0F000000U
#define HSMCRYPTO_AICVER_MAJORVER_S                                         24U

// Field: [23:20] MINORVER
//
// These bits encode the minor version number for the AIC module.
#define HSMCRYPTO_AICVER_MINORVER_W                                          4U
#define HSMCRYPTO_AICVER_MINORVER_M                                 0x00F00000U
#define HSMCRYPTO_AICVER_MINORVER_S                                         20U

// Field: [19:16] PATCHLVL
//
// These bits encode the hardware patch level for the AIC module, starting at
// value 0 on the first release.
#define HSMCRYPTO_AICVER_PATCHLVL_W                                          4U
#define HSMCRYPTO_AICVER_PATCHLVL_M                                 0x000F0000U
#define HSMCRYPTO_AICVER_PATCHLVL_S                                         16U

// Field:  [15:8] NUMCMPL
//
// These bits simply contain the complement of bits [7:0], used by a driver to
// ascertain that this version register is indeed read.
#define HSMCRYPTO_AICVER_NUMCMPL_W                                           8U
#define HSMCRYPTO_AICVER_NUMCMPL_M                                  0x0000FF00U
#define HSMCRYPTO_AICVER_NUMCMPL_S                                           8U

// Field:   [7:0] NUM
//
// These bits encode the AIC number.
#define HSMCRYPTO_AICVER_NUM_W                                               8U
#define HSMCRYPTO_AICVER_NUM_M                                      0x000000FFU
#define HSMCRYPTO_AICVER_NUM_S                                               0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBSTA
//
//*****************************************************************************
// Field:     [7] MB2AVAIL
//
// Input Mailbox 2 available status
// ENUMs:
// AVAIL                    Input Mailbox is available for linking by this
//                          Host
// NAVAIL                   Input Mailbox is linked to a Host or is filled
#define HSMCRYPTO_MBSTA_MB2AVAIL                                    0x00000080U
#define HSMCRYPTO_MBSTA_MB2AVAIL_M                                  0x00000080U
#define HSMCRYPTO_MBSTA_MB2AVAIL_S                                           7U
#define HSMCRYPTO_MBSTA_MB2AVAIL_AVAIL                              0x00000080U
#define HSMCRYPTO_MBSTA_MB2AVAIL_NAVAIL                             0x00000000U

// Field:     [6] MB2LNKD
//
// Mailbox 2 Link Status
// ENUMs:
// LNKD                     This Host is linked to Mailbox
// NLNKD                    This Host is not linked to Mailbox
#define HSMCRYPTO_MBSTA_MB2LNKD                                     0x00000040U
#define HSMCRYPTO_MBSTA_MB2LNKD_M                                   0x00000040U
#define HSMCRYPTO_MBSTA_MB2LNKD_S                                            6U
#define HSMCRYPTO_MBSTA_MB2LNKD_LNKD                                0x00000040U
#define HSMCRYPTO_MBSTA_MB2LNKD_NLNKD                               0x00000000U

// Field:     [5] MB2OUT
//
// Output Mailbox 2 Status
// ENUMs:
// FULL                     Mailbox is Full
// EMTY                     Mailbox is Empty
#define HSMCRYPTO_MBSTA_MB2OUT                                      0x00000020U
#define HSMCRYPTO_MBSTA_MB2OUT_M                                    0x00000020U
#define HSMCRYPTO_MBSTA_MB2OUT_S                                             5U
#define HSMCRYPTO_MBSTA_MB2OUT_FULL                                 0x00000020U
#define HSMCRYPTO_MBSTA_MB2OUT_EMTY                                 0x00000000U

// Field:     [4] MB2IN
//
// Input Mailbox 2 Status
// ENUMs:
// FULL                     Mailbox is Full
// EMTY                     Mailbox is Empty
#define HSMCRYPTO_MBSTA_MB2IN                                       0x00000010U
#define HSMCRYPTO_MBSTA_MB2IN_M                                     0x00000010U
#define HSMCRYPTO_MBSTA_MB2IN_S                                              4U
#define HSMCRYPTO_MBSTA_MB2IN_FULL                                  0x00000010U
#define HSMCRYPTO_MBSTA_MB2IN_EMTY                                  0x00000000U

// Field:     [3] MB1AVAIL
//
// Input Mailbox 1 available status
// ENUMs:
// AVAIL                    Input Mailbox is available for linking by this
//                          Host
// NAVAIL                   Input Mailbox is linked to a Host or is filled
#define HSMCRYPTO_MBSTA_MB1AVAIL                                    0x00000008U
#define HSMCRYPTO_MBSTA_MB1AVAIL_M                                  0x00000008U
#define HSMCRYPTO_MBSTA_MB1AVAIL_S                                           3U
#define HSMCRYPTO_MBSTA_MB1AVAIL_AVAIL                              0x00000008U
#define HSMCRYPTO_MBSTA_MB1AVAIL_NAVAIL                             0x00000000U

// Field:     [2] MB1LNKD
//
// Mailbox 1 Link Status
// ENUMs:
// LNKD                     This Host is linked to Mailbox
// NLNKD                    This Host is not linked to Mailbox
#define HSMCRYPTO_MBSTA_MB1LNKD                                     0x00000004U
#define HSMCRYPTO_MBSTA_MB1LNKD_M                                   0x00000004U
#define HSMCRYPTO_MBSTA_MB1LNKD_S                                            2U
#define HSMCRYPTO_MBSTA_MB1LNKD_LNKD                                0x00000004U
#define HSMCRYPTO_MBSTA_MB1LNKD_NLNKD                               0x00000000U

// Field:     [1] MB1OUT
//
// Output Mailbox 1 Status
// ENUMs:
// FULL                     Mailbox is Full
// EMTY                     Mailbox is Empty
#define HSMCRYPTO_MBSTA_MB1OUT                                      0x00000002U
#define HSMCRYPTO_MBSTA_MB1OUT_M                                    0x00000002U
#define HSMCRYPTO_MBSTA_MB1OUT_S                                             1U
#define HSMCRYPTO_MBSTA_MB1OUT_FULL                                 0x00000002U
#define HSMCRYPTO_MBSTA_MB1OUT_EMTY                                 0x00000000U

// Field:     [0] MB1IN
//
// Input Mailbox 1 Status
// ENUMs:
// FULL                     Mailbox is Full
// EMTY                     Mailbox is Empty
#define HSMCRYPTO_MBSTA_MB1IN                                       0x00000001U
#define HSMCRYPTO_MBSTA_MB1IN_M                                     0x00000001U
#define HSMCRYPTO_MBSTA_MB1IN_S                                              0U
#define HSMCRYPTO_MBSTA_MB1IN_FULL                                  0x00000001U
#define HSMCRYPTO_MBSTA_MB1IN_EMTY                                  0x00000000U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBCTL
//
//*****************************************************************************
// Field:     [7] MB2ULNK
//
// Unlink the Mailbox from this host
// ENUMs:
// UNLNK                    unlink Mailbox 2
#define HSMCRYPTO_MBCTL_MB2ULNK                                     0x00000080U
#define HSMCRYPTO_MBCTL_MB2ULNK_M                                   0x00000080U
#define HSMCRYPTO_MBCTL_MB2ULNK_S                                            7U
#define HSMCRYPTO_MBCTL_MB2ULNK_UNLNK                               0x00000080U

// Field:     [6] MB2LNK
//
// Link Mailbox to this Host. Host can link to a mailbox only if it is not
// filled and not linked to another host
// ENUMs:
// LNK                      Link Mailbox 2
#define HSMCRYPTO_MBCTL_MB2LNK                                      0x00000040U
#define HSMCRYPTO_MBCTL_MB2LNK_M                                    0x00000040U
#define HSMCRYPTO_MBCTL_MB2LNK_S                                             6U
#define HSMCRYPTO_MBCTL_MB2LNK_LNK                                  0x00000040U

// Field:     [5] MB2OUT
//
// The Host for whom the token is in Output Mailbox 1 can clear the status
// ENUMs:
// EMTY                     Set output mailbox empty
#define HSMCRYPTO_MBCTL_MB2OUT                                      0x00000020U
#define HSMCRYPTO_MBCTL_MB2OUT_M                                    0x00000020U
#define HSMCRYPTO_MBCTL_MB2OUT_S                                             5U
#define HSMCRYPTO_MBCTL_MB2OUT_EMTY                                 0x00000020U

// Field:     [4] MB2IN
//
// The Host linked to input mailbox 2 can set after placing a token into Input
// Mailbox 2
// ENUMs:
// FULL                     Set Input Mailbox full
#define HSMCRYPTO_MBCTL_MB2IN                                       0x00000010U
#define HSMCRYPTO_MBCTL_MB2IN_M                                     0x00000010U
#define HSMCRYPTO_MBCTL_MB2IN_S                                              4U
#define HSMCRYPTO_MBCTL_MB2IN_FULL                                  0x00000010U

// Field:     [3] MB1UNLNK
//
// Unlink the Mailbox from this host
// ENUMs:
// UNLNK                    Unlink Mailbox 1
#define HSMCRYPTO_MBCTL_MB1UNLNK                                    0x00000008U
#define HSMCRYPTO_MBCTL_MB1UNLNK_M                                  0x00000008U
#define HSMCRYPTO_MBCTL_MB1UNLNK_S                                           3U
#define HSMCRYPTO_MBCTL_MB1UNLNK_UNLNK                              0x00000008U

// Field:     [2] MB1LNK
//
// Link Mailbox to this Host. Host can link to a mailbox only if it is not
// filled and not linked to another host
// ENUMs:
// LNK                      Link Mailbox 1
#define HSMCRYPTO_MBCTL_MB1LNK                                      0x00000004U
#define HSMCRYPTO_MBCTL_MB1LNK_M                                    0x00000004U
#define HSMCRYPTO_MBCTL_MB1LNK_S                                             2U
#define HSMCRYPTO_MBCTL_MB1LNK_LNK                                  0x00000004U

// Field:     [1] MB1OUT
//
// The Host for whom the token is in Output Mailbox 1 can clear the status
// ENUMs:
// EMTY                     Set output mailbox empty
#define HSMCRYPTO_MBCTL_MB1OUT                                      0x00000002U
#define HSMCRYPTO_MBCTL_MB1OUT_M                                    0x00000002U
#define HSMCRYPTO_MBCTL_MB1OUT_S                                             1U
#define HSMCRYPTO_MBCTL_MB1OUT_EMTY                                 0x00000002U

// Field:     [0] MB1IN
//
// The Host linked to input mailbox 1 can set after placing a token into Input
// Mailbox 1
// ENUMs:
// FULL                     Set Input Mailbox full
#define HSMCRYPTO_MBCTL_MB1IN                                       0x00000001U
#define HSMCRYPTO_MBCTL_MB1IN_M                                     0x00000001U
#define HSMCRYPTO_MBCTL_MB1IN_S                                              0U
#define HSMCRYPTO_MBCTL_MB1IN_FULL                                  0x00000001U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBRSTA
//
//*****************************************************************************
// Field:     [6] MB2LNKD
//
// Mailbox 2 Linked Raw Status
// ENUMs:
// LNKD                     This Host is linked to Mailbox
// NLNKD                    This Host is not linked to Mailbox
#define HSMCRYPTO_MBRSTA_MB2LNKD                                    0x00000040U
#define HSMCRYPTO_MBRSTA_MB2LNKD_M                                  0x00000040U
#define HSMCRYPTO_MBRSTA_MB2LNKD_S                                           6U
#define HSMCRYPTO_MBRSTA_MB2LNKD_LNKD                               0x00000040U
#define HSMCRYPTO_MBRSTA_MB2LNKD_NLNKD                              0x00000000U

// Field:     [5] MB2OUT
//
// Output Mailbox 2 Raw Status
// ENUMs:
// FULL                     Mailbox is Full
// EMTY                     Mailbox is Empty
#define HSMCRYPTO_MBRSTA_MB2OUT                                     0x00000020U
#define HSMCRYPTO_MBRSTA_MB2OUT_M                                   0x00000020U
#define HSMCRYPTO_MBRSTA_MB2OUT_S                                            5U
#define HSMCRYPTO_MBRSTA_MB2OUT_FULL                                0x00000020U
#define HSMCRYPTO_MBRSTA_MB2OUT_EMTY                                0x00000000U

// Field:     [4] MB2IN
//
// Input Mailbox 2 Raw Status
// ENUMs:
// FULL                     Mailbox is Full
// EMTY                     Mailbox is Empty
#define HSMCRYPTO_MBRSTA_MB2IN                                      0x00000010U
#define HSMCRYPTO_MBRSTA_MB2IN_M                                    0x00000010U
#define HSMCRYPTO_MBRSTA_MB2IN_S                                             4U
#define HSMCRYPTO_MBRSTA_MB2IN_FULL                                 0x00000010U
#define HSMCRYPTO_MBRSTA_MB2IN_EMTY                                 0x00000000U

// Field:     [2] MB1LNKD
//
// Mailbox 1 Linked Raw Status
// ENUMs:
// LNKD                     This Host is linked to Mailbox
// NLNKD                    This Host is not linked to Mailbox
#define HSMCRYPTO_MBRSTA_MB1LNKD                                    0x00000004U
#define HSMCRYPTO_MBRSTA_MB1LNKD_M                                  0x00000004U
#define HSMCRYPTO_MBRSTA_MB1LNKD_S                                           2U
#define HSMCRYPTO_MBRSTA_MB1LNKD_LNKD                               0x00000004U
#define HSMCRYPTO_MBRSTA_MB1LNKD_NLNKD                              0x00000000U

// Field:     [1] MB1OUT
//
// Output Mailbox 1 Raw Status
// ENUMs:
// FULL                     Mailbox is Full
// EMTY                     Mailbox is Empty
#define HSMCRYPTO_MBRSTA_MB1OUT                                     0x00000002U
#define HSMCRYPTO_MBRSTA_MB1OUT_M                                   0x00000002U
#define HSMCRYPTO_MBRSTA_MB1OUT_S                                            1U
#define HSMCRYPTO_MBRSTA_MB1OUT_FULL                                0x00000002U
#define HSMCRYPTO_MBRSTA_MB1OUT_EMTY                                0x00000000U

// Field:     [0] MB1IN
//
// Input Mailbox 1 Raw Status
// ENUMs:
// FULL                     Mailbox is Full
// EMTY                     Mailbox is Empty
#define HSMCRYPTO_MBRSTA_MB1IN                                      0x00000001U
#define HSMCRYPTO_MBRSTA_MB1IN_M                                    0x00000001U
#define HSMCRYPTO_MBRSTA_MB1IN_S                                             0U
#define HSMCRYPTO_MBRSTA_MB1IN_FULL                                 0x00000001U
#define HSMCRYPTO_MBRSTA_MB1IN_EMTY                                 0x00000000U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBRST
//
//*****************************************************************************
// Field:     [7] MB2ULNK
//
// Set only - Master Host can unlink mbx from current Host by writing 1b here.
// ENUMs:
// UNLNK                    Unlink Mailbox
#define HSMCRYPTO_MBRST_MB2ULNK                                     0x00000080U
#define HSMCRYPTO_MBRST_MB2ULNK_M                                   0x00000080U
#define HSMCRYPTO_MBRST_MB2ULNK_S                                            7U
#define HSMCRYPTO_MBRST_MB2ULNK_UNLNK                               0x00000080U

// Field:     [5] MB2OUT
//
// Set only - Master Host can clear mbx_out_full bit in MBSTA by writing 1b
// here.
// ENUMs:
// EMTY                     Set Mailbox Empty
#define HSMCRYPTO_MBRST_MB2OUT                                      0x00000020U
#define HSMCRYPTO_MBRST_MB2OUT_M                                    0x00000020U
#define HSMCRYPTO_MBRST_MB2OUT_S                                             5U
#define HSMCRYPTO_MBRST_MB2OUT_EMTY                                 0x00000020U

// Field:     [3] MB1UNLNK
//
// Set only - Master Host can unlink mbx from current Host by writing 1b here.
// ENUMs:
// UNLNK                    Unlink Mailbox
#define HSMCRYPTO_MBRST_MB1UNLNK                                    0x00000008U
#define HSMCRYPTO_MBRST_MB1UNLNK_M                                  0x00000008U
#define HSMCRYPTO_MBRST_MB1UNLNK_S                                           3U
#define HSMCRYPTO_MBRST_MB1UNLNK_UNLNK                              0x00000008U

// Field:     [1] MB1OUT
//
// Set only - Master Host can clear mbx_out_full bit in MBSTA by writing 1b
// here.
// ENUMs:
// EMTY                     Set Mailbox Empty
#define HSMCRYPTO_MBRST_MB1OUT                                      0x00000002U
#define HSMCRYPTO_MBRST_MB1OUT_M                                    0x00000002U
#define HSMCRYPTO_MBRST_MB1OUT_S                                             1U
#define HSMCRYPTO_MBRST_MB1OUT_EMTY                                 0x00000002U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBLNKID
//
//*****************************************************************************
// Field:     [7] MB2PACC
//
// 0: Mailbox 2 can be accessed by the Host using protected or non-protected
// access. 1: Mailbox 2 is only accessible if the Host uses protected access.
#define HSMCRYPTO_MBLNKID_MB2PACC                                   0x00000080U
#define HSMCRYPTO_MBLNKID_MB2PACC_M                                 0x00000080U
#define HSMCRYPTO_MBLNKID_MB2PACC_S                                          7U

// Field:   [6:4] MB2LNKID
//
// Host cpu_id of the Host linked to the Mailbox 2
#define HSMCRYPTO_MBLNKID_MB2LNKID_W                                         3U
#define HSMCRYPTO_MBLNKID_MB2LNKID_M                                0x00000070U
#define HSMCRYPTO_MBLNKID_MB2LNKID_S                                         4U

// Field:     [3] MB1PACC
//
// 0: Mailbox 1 can be accessed by the Host using protected or non-protected
// access. 1: Mailbox 1 is only accessible if the Host uses protected access.
#define HSMCRYPTO_MBLNKID_MB1PACC                                   0x00000008U
#define HSMCRYPTO_MBLNKID_MB1PACC_M                                 0x00000008U
#define HSMCRYPTO_MBLNKID_MB1PACC_S                                          3U

// Field:   [2:0] MB1LNKID
//
// Host cpu_id of the Host linked to the Mailbox 1
#define HSMCRYPTO_MBLNKID_MB1LNKID_W                                         3U
#define HSMCRYPTO_MBLNKID_MB1LNKID_M                                0x00000007U
#define HSMCRYPTO_MBLNKID_MB1LNKID_S                                         0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBOUTID
//
//*****************************************************************************
// Field:     [7] MB2PACC
//
// 0: Output mailbox 2 can be accessed by the Host using protected or
// non-protected access. 1: Output mailbox 2 is only accessible if the Host
// uses protected access.
#define HSMCRYPTO_MBOUTID_MB2PACC                                   0x00000080U
#define HSMCRYPTO_MBOUTID_MB2PACC_M                                 0x00000080U
#define HSMCRYPTO_MBOUTID_MB2PACC_S                                          7U

// Field:   [6:4] MB2ID
//
// Host cpu_id of the Host allowed to read a result from the Mailbox 2
#define HSMCRYPTO_MBOUTID_MB2ID_W                                            3U
#define HSMCRYPTO_MBOUTID_MB2ID_M                                   0x00000070U
#define HSMCRYPTO_MBOUTID_MB2ID_S                                            4U

// Field:     [3] MB1PACC
//
// 0: Output mailbox 1 can be accessed by the Host using protected or
// non-protected access. 1: Output mailbox 1 is only accessible if the Host
// uses protected access.
#define HSMCRYPTO_MBOUTID_MB1PACC                                   0x00000008U
#define HSMCRYPTO_MBOUTID_MB1PACC_M                                 0x00000008U
#define HSMCRYPTO_MBOUTID_MB1PACC_S                                          3U

// Field:   [2:0] MB1ID
//
// HostID of the Host allowed to read a result from the Mailbox 1
#define HSMCRYPTO_MBOUTID_MB1ID_W                                            3U
#define HSMCRYPTO_MBOUTID_MB1ID_M                                   0x00000007U
#define HSMCRYPTO_MBOUTID_MB1ID_S                                            0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MBLCKOUT
//
//*****************************************************************************
// Field:  [15:8] MB2LKOUT
//
// Bit map indicates which Hosts are blocked from accessing mailbox 2
#define HSMCRYPTO_MBLCKOUT_MB2LKOUT_W                                        8U
#define HSMCRYPTO_MBLCKOUT_MB2LKOUT_M                               0x0000FF00U
#define HSMCRYPTO_MBLCKOUT_MB2LKOUT_S                                        8U

// Field:   [7:0] MB1LKOUT
//
// Bit map indicates which Hosts are blocked from accessing mailbox 1
#define HSMCRYPTO_MBLCKOUT_MB1LKOUT_W                                        8U
#define HSMCRYPTO_MBLCKOUT_MB1LKOUT_M                               0x000000FFU
#define HSMCRYPTO_MBLCKOUT_MB1LKOUT_S                                        0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_MODSTA
//
//*****************************************************************************
// Field:    [31] FATAL
//
// Set if fatal error occured
#define HSMCRYPTO_MODSTA_FATAL                                      0x80000000U
#define HSMCRYPTO_MODSTA_FATAL_M                                    0x80000000U
#define HSMCRYPTO_MODSTA_FATAL_S                                            31U

// Field:    [23] FWACPTD
//
// Set if firmware is to be executed
#define HSMCRYPTO_MODSTA_FWACPTD                                    0x00800000U
#define HSMCRYPTO_MODSTA_FWACPTD_M                                  0x00800000U
#define HSMCRYPTO_MODSTA_FWACPTD_S                                          23U

// Field:    [22] FWCKDONE
//
// Set if firmware checks complete
#define HSMCRYPTO_MODSTA_FWCKDONE                                   0x00400000U
#define HSMCRYPTO_MODSTA_FWCKDONE_M                                 0x00400000U
#define HSMCRYPTO_MODSTA_FWCKDONE_S                                         22U

// Field:    [10] CRC24ERR
//
// Set if CRC on ProgramROM is fails
#define HSMCRYPTO_MODSTA_CRC24ERR                                   0x00000400U
#define HSMCRYPTO_MODSTA_CRC24ERR_M                                 0x00000400U
#define HSMCRYPTO_MODSTA_CRC24ERR_S                                         10U

// Field:     [9] CRC24OK
//
// Set if CRC on ProgramROM is passes
#define HSMCRYPTO_MODSTA_CRC24OK                                    0x00000200U
#define HSMCRYPTO_MODSTA_CRC24OK_M                                  0x00000200U
#define HSMCRYPTO_MODSTA_CRC24OK_S                                           9U

// Field:     [8] CRC24BSY
//
// Set if CRC on ProgramROM is busy
#define HSMCRYPTO_MODSTA_CRC24BSY                                   0x00000100U
#define HSMCRYPTO_MODSTA_CRC24BSY_M                                 0x00000100U
#define HSMCRYPTO_MODSTA_CRC24BSY_S                                          8U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_OPTIONS2
//
//*****************************************************************************
// Field:    [25] ADDCE10
//
// An additional crypto engine is available in hardware as custom engine10
#define HSMCRYPTO_OPTIONS2_ADDCE10                                  0x02000000U
#define HSMCRYPTO_OPTIONS2_ADDCE10_M                                0x02000000U
#define HSMCRYPTO_OPTIONS2_ADDCE10_S                                        25U

// Field:    [24] ADDCE9
//
// An additional crypto engine is available in hardware as custom engine9
#define HSMCRYPTO_OPTIONS2_ADDCE9                                   0x01000000U
#define HSMCRYPTO_OPTIONS2_ADDCE9_M                                 0x01000000U
#define HSMCRYPTO_OPTIONS2_ADDCE9_S                                         24U

// Field:    [23] ADDCE8
//
// An additional crypto engine is available in hardware as custom engine8
#define HSMCRYPTO_OPTIONS2_ADDCE8                                   0x00800000U
#define HSMCRYPTO_OPTIONS2_ADDCE8_M                                 0x00800000U
#define HSMCRYPTO_OPTIONS2_ADDCE8_S                                         23U

// Field:    [22] ADDCE7
//
// An additional crypto engine is available in hardware as custom engine7
#define HSMCRYPTO_OPTIONS2_ADDCE7                                   0x00400000U
#define HSMCRYPTO_OPTIONS2_ADDCE7_M                                 0x00400000U
#define HSMCRYPTO_OPTIONS2_ADDCE7_S                                         22U

// Field:    [21] ADDCE6
//
// An additional crypto engine is available in hardware as custom engine6
#define HSMCRYPTO_OPTIONS2_ADDCE6                                   0x00200000U
#define HSMCRYPTO_OPTIONS2_ADDCE6_M                                 0x00200000U
#define HSMCRYPTO_OPTIONS2_ADDCE6_S                                         21U

// Field:    [20] ADDCE5
//
// An additional crypto engine is available in hardware as custom engine5
#define HSMCRYPTO_OPTIONS2_ADDCE5                                   0x00100000U
#define HSMCRYPTO_OPTIONS2_ADDCE5_M                                 0x00100000U
#define HSMCRYPTO_OPTIONS2_ADDCE5_S                                         20U

// Field:    [19] ADDCE4
//
// An additional crypto engine is available in hardware as custom engine4
#define HSMCRYPTO_OPTIONS2_ADDCE4                                   0x00080000U
#define HSMCRYPTO_OPTIONS2_ADDCE4_M                                 0x00080000U
#define HSMCRYPTO_OPTIONS2_ADDCE4_S                                         19U

// Field:    [18] ADDCE3
//
// An additional crypto engine is available in hardware as custom engine3
#define HSMCRYPTO_OPTIONS2_ADDCE3                                   0x00040000U
#define HSMCRYPTO_OPTIONS2_ADDCE3_M                                 0x00040000U
#define HSMCRYPTO_OPTIONS2_ADDCE3_S                                         18U

// Field:    [17] ADDCE2
//
// An additional crypto engine is available in hardware as custom engine2
#define HSMCRYPTO_OPTIONS2_ADDCE2                                   0x00020000U
#define HSMCRYPTO_OPTIONS2_ADDCE2_M                                 0x00020000U
#define HSMCRYPTO_OPTIONS2_ADDCE2_S                                         17U

// Field:    [16] ADDCE1
//
// An additional crypto engine is available in hardware as custom engine1
#define HSMCRYPTO_OPTIONS2_ADDCE1                                   0x00010000U
#define HSMCRYPTO_OPTIONS2_ADDCE1_M                                 0x00010000U
#define HSMCRYPTO_OPTIONS2_ADDCE1_S                                         16U

// Field:    [12] BUSIFC
//
// Bus interface type, 0b = 32-bit AHB, 1b = 32-bit AXI
#define HSMCRYPTO_OPTIONS2_BUSIFC                                   0x00001000U
#define HSMCRYPTO_OPTIONS2_BUSIFC_M                                 0x00001000U
#define HSMCRYPTO_OPTIONS2_BUSIFC_S                                         12U

// Field:     [9] PROGRAM
//
// Downloadable RAM based firmware program memory.
#define HSMCRYPTO_OPTIONS2_PROGRAM                                  0x00000200U
#define HSMCRYPTO_OPTIONS2_PROGRAM_M                                0x00000200U
#define HSMCRYPTO_OPTIONS2_PROGRAM_S                                         9U

// Field:     [8] CCPU
//
// C capable local cpu available
#define HSMCRYPTO_OPTIONS2_CCPU                                     0x00000100U
#define HSMCRYPTO_OPTIONS2_CCPU_M                                   0x00000100U
#define HSMCRYPTO_OPTIONS2_CCPU_S                                            8U

// Field:     [6] CRNG
//
// CRNG engine available
#define HSMCRYPTO_OPTIONS2_CRNG                                     0x00000040U
#define HSMCRYPTO_OPTIONS2_CRNG_M                                   0x00000040U
#define HSMCRYPTO_OPTIONS2_CRNG_S                                            6U

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
// TRNG engine available
#define HSMCRYPTO_OPTIONS2_TRNG                                     0x00000008U
#define HSMCRYPTO_OPTIONS2_TRNG_M                                   0x00000008U
#define HSMCRYPTO_OPTIONS2_TRNG_S                                            3U

// Field:     [2] SHA
//
// SHA1/SHA2 combination core available
#define HSMCRYPTO_OPTIONS2_SHA                                      0x00000004U
#define HSMCRYPTO_OPTIONS2_SHA_M                                    0x00000004U
#define HSMCRYPTO_OPTIONS2_SHA_S                                             2U

// Field:     [0] DESAES
//
// DES/AES combination crypto core available
#define HSMCRYPTO_OPTIONS2_DESAES                                   0x00000001U
#define HSMCRYPTO_OPTIONS2_DESAES_M                                 0x00000001U
#define HSMCRYPTO_OPTIONS2_DESAES_S                                          0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_OPTIONS
//
//*****************************************************************************
// Field: [31:24] SHOST
//
// Bits to indicate which of the 8 possible HOSTID codes on the bus interface
// are active Hosts with secure access
#define HSMCRYPTO_OPTIONS_SHOST_W                                            8U
#define HSMCRYPTO_OPTIONS_SHOST_M                                   0xFF000000U
#define HSMCRYPTO_OPTIONS_SHOST_S                                           24U

// Field:    [23] MYIDSEC
//
// Indicates the current protection bit values of the Host actually reading the
// register
#define HSMCRYPTO_OPTIONS_MYIDSEC                                   0x00800000U
#define HSMCRYPTO_OPTIONS_MYIDSEC_M                                 0x00800000U
#define HSMCRYPTO_OPTIONS_MYIDSEC_S                                         23U

// Field: [22:20] MYID
//
// Host ID code for the Host that is reading this register
#define HSMCRYPTO_OPTIONS_MYID_W                                             3U
#define HSMCRYPTO_OPTIONS_MYID_M                                    0x00700000U
#define HSMCRYPTO_OPTIONS_MYID_S                                            20U

// Field: [18:16] MASTERID
//
// Value of the HOSTID that designates the Master Host
#define HSMCRYPTO_OPTIONS_MASTERID_W                                         3U
#define HSMCRYPTO_OPTIONS_MASTERID_M                                0x00070000U
#define HSMCRYPTO_OPTIONS_MASTERID_S                                        16U

// Field:  [15:8] HOSTID
//
// Bits to indicate which of the 8 possible HOSTID codes on the bus interface
// are active
#define HSMCRYPTO_OPTIONS_HOSTID_W                                           8U
#define HSMCRYPTO_OPTIONS_HOSTID_M                                  0x0000FF00U
#define HSMCRYPTO_OPTIONS_HOSTID_S                                           8U

// Field:   [5:4] MBSIZE
//
// Mailbox pair Size
// ENUMs:
// MB1024                   1024 Bytes
// MB512                    512 Bytes
// MB256                    256 Bytes
// MB128                    128 Bytes
#define HSMCRYPTO_OPTIONS_MBSIZE_W                                           2U
#define HSMCRYPTO_OPTIONS_MBSIZE_M                                  0x00000030U
#define HSMCRYPTO_OPTIONS_MBSIZE_S                                           4U
#define HSMCRYPTO_OPTIONS_MBSIZE_MB1024                             0x00000030U
#define HSMCRYPTO_OPTIONS_MBSIZE_MB512                              0x00000020U
#define HSMCRYPTO_OPTIONS_MBSIZE_MB256                              0x00000010U
#define HSMCRYPTO_OPTIONS_MBSIZE_MB128                              0x00000000U

// Field:   [3:0] NMB
//
// Number of Input/Output Mailbox pairs
#define HSMCRYPTO_OPTIONS_NMB_W                                              4U
#define HSMCRYPTO_OPTIONS_NMB_M                                     0x0000000FU
#define HSMCRYPTO_OPTIONS_NMB_S                                              0U

//*****************************************************************************
//
// Register: HSMCRYPTO_O_VERSION
//
//*****************************************************************************
// Field: [27:24] MAJOR
//
// Major Version release number for this module
#define HSMCRYPTO_VERSION_MAJOR_W                                            4U
#define HSMCRYPTO_VERSION_MAJOR_M                                   0x0F000000U
#define HSMCRYPTO_VERSION_MAJOR_S                                           24U

// Field: [23:20] MINOR
//
// Minor Version release number for this module
#define HSMCRYPTO_VERSION_MINOR_W                                            4U
#define HSMCRYPTO_VERSION_MINOR_M                                   0x00F00000U
#define HSMCRYPTO_VERSION_MINOR_S                                           20U

// Field: [19:16] PATCHLVL
//
// Hardware Patch Level for this module
#define HSMCRYPTO_VERSION_PATCHLVL_W                                         4U
#define HSMCRYPTO_VERSION_PATCHLVL_M                                0x000F0000U
#define HSMCRYPTO_VERSION_PATCHLVL_S                                        16U

// Field:  [15:8] NUMCMPL
//
// Bit by Bit compliment of IP Number
#define HSMCRYPTO_VERSION_NUMCMPL_W                                          8U
#define HSMCRYPTO_VERSION_NUMCMPL_M                                 0x0000FF00U
#define HSMCRYPTO_VERSION_NUMCMPL_S                                          8U

// Field:   [7:0] NUM
//
// IP number
#define HSMCRYPTO_VERSION_NUM_W                                              8U
#define HSMCRYPTO_VERSION_NUM_M                                     0x000000FFU
#define HSMCRYPTO_VERSION_NUM_S                                              0U


#endif // __HSMCRYPTO__
