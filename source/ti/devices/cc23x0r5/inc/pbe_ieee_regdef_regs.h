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
 *  ======== pbe_ieee_regdef_regs.h ========
*/

#ifndef __PBE_IEEE_REGDEF_REGS_H
#define __PBE_IEEE_REGDEF_REGS_H

//******************************************************************************
// REGISTER OFFSETS
//******************************************************************************
// Packet Building Engine (PBE) interrupt generation register
#define PBE_IEEE_REGDEF_O_IRQ                                        0x00000010U

// PBE API Operation Register
#define PBE_IEEE_REGDEF_O_API                                        0x00000030U

//******************************************************************************
// Register: IRQ
//******************************************************************************
// Field: [15:15] operror
//
// Software defined interrupt
#define PBE_IEEE_REGDEF_IRQ_OPERROR                                  0x00008000U
#define PBE_IEEE_REGDEF_IRQ_OPERROR_M                                0x00008000U
#define PBE_IEEE_REGDEF_IRQ_OPERROR_S                                        15U
#define PBE_IEEE_REGDEF_IRQ_OPERROR_OFF                              0x00000000U
#define PBE_IEEE_REGDEF_IRQ_OPERROR_ACTIVE                           0x00008000U

// Field: [14:14] unused14
//
// Software defined interrupt
#define PBE_IEEE_REGDEF_IRQ_UNUSED14                                 0x00004000U
#define PBE_IEEE_REGDEF_IRQ_UNUSED14_M                               0x00004000U
#define PBE_IEEE_REGDEF_IRQ_UNUSED14_S                                       14U
#define PBE_IEEE_REGDEF_IRQ_UNUSED14_OFF                             0x00000000U
#define PBE_IEEE_REGDEF_IRQ_UNUSED14_ACTIVE                          0x00004000U

// Field: [13:13] txdone
//
// Packet transmitted. MCU shall deallocate or retry.
#define PBE_IEEE_REGDEF_IRQ_TXDONE                                   0x00002000U
#define PBE_IEEE_REGDEF_IRQ_TXDONE_M                                 0x00002000U
#define PBE_IEEE_REGDEF_IRQ_TXDONE_S                                         13U
#define PBE_IEEE_REGDEF_IRQ_TXDONE_OFF                               0x00000000U
#define PBE_IEEE_REGDEF_IRQ_TXDONE_ACTIVE                            0x00002000U

// Field: [12:12] txack
//
// ACK transmitted
#define PBE_IEEE_REGDEF_IRQ_TXACK                                    0x00001000U
#define PBE_IEEE_REGDEF_IRQ_TXACK_M                                  0x00001000U
#define PBE_IEEE_REGDEF_IRQ_TXACK_S                                          12U
#define PBE_IEEE_REGDEF_IRQ_TXACK_OFF                                0x00000000U
#define PBE_IEEE_REGDEF_IRQ_TXACK_ACTIVE                             0x00001000U

// Field: [11:11] unused11
//
// Software defined interrupt
#define PBE_IEEE_REGDEF_IRQ_UNUSED11                                 0x00000800U
#define PBE_IEEE_REGDEF_IRQ_UNUSED11_M                               0x00000800U
#define PBE_IEEE_REGDEF_IRQ_UNUSED11_S                                       11U
#define PBE_IEEE_REGDEF_IRQ_UNUSED11_OFF                             0x00000000U
#define PBE_IEEE_REGDEF_IRQ_UNUSED11_ACTIVE                          0x00000800U

// Field: [10:10] unused10
//
// Software defined interrupt
#define PBE_IEEE_REGDEF_IRQ_UNUSED10                                 0x00000400U
#define PBE_IEEE_REGDEF_IRQ_UNUSED10_M                               0x00000400U
#define PBE_IEEE_REGDEF_IRQ_UNUSED10_S                                       10U
#define PBE_IEEE_REGDEF_IRQ_UNUSED10_OFF                             0x00000000U
#define PBE_IEEE_REGDEF_IRQ_UNUSED10_ACTIVE                          0x00000400U

// Field: [9:9] ackabort
//
// Ack frame, to be transmitted, was cancelled.
#define PBE_IEEE_REGDEF_IRQ_ACKABORT                                 0x00000200U
#define PBE_IEEE_REGDEF_IRQ_ACKABORT_M                               0x00000200U
#define PBE_IEEE_REGDEF_IRQ_ACKABORT_S                                        9U
#define PBE_IEEE_REGDEF_IRQ_ACKABORT_OFF                             0x00000000U
#define PBE_IEEE_REGDEF_IRQ_ACKABORT_ACTIVE                          0x00000200U

// Field: [8:8] rxok
//
// Packet received with CRC OK and not to be ignored by the MCU
#define PBE_IEEE_REGDEF_IRQ_RXOK                                     0x00000100U
#define PBE_IEEE_REGDEF_IRQ_RXOK_M                                   0x00000100U
#define PBE_IEEE_REGDEF_IRQ_RXOK_S                                            8U
#define PBE_IEEE_REGDEF_IRQ_RXOK_OFF                                 0x00000000U
#define PBE_IEEE_REGDEF_IRQ_RXOK_ACTIVE                              0x00000100U

// Field: [7:7] rxfovfl
//
// Packet received which did not fit in the RX FIFO and was not to be discarded. 
#define PBE_IEEE_REGDEF_IRQ_RXFOVFL                                  0x00000080U
#define PBE_IEEE_REGDEF_IRQ_RXFOVFL_M                                0x00000080U
#define PBE_IEEE_REGDEF_IRQ_RXFOVFL_S                                         7U
#define PBE_IEEE_REGDEF_IRQ_RXFOVFL_OFF                              0x00000000U
#define PBE_IEEE_REGDEF_IRQ_RXFOVFL_ACTIVE                           0x00000080U

// Field: [6:6] ackdone
//
// ACK event that is raised on successful or not ACK receptions  
#define PBE_IEEE_REGDEF_IRQ_ACKDONE                                  0x00000040U
#define PBE_IEEE_REGDEF_IRQ_ACKDONE_M                                0x00000040U
#define PBE_IEEE_REGDEF_IRQ_ACKDONE_S                                         6U
#define PBE_IEEE_REGDEF_IRQ_ACKDONE_OFF                              0x00000000U
#define PBE_IEEE_REGDEF_IRQ_ACKDONE_ACTIVE                           0x00000040U

// Field: [5:5] rxign
//
// Packet received with correct CRC, but may be ignored by MCU
#define PBE_IEEE_REGDEF_IRQ_RXIGN                                    0x00000020U
#define PBE_IEEE_REGDEF_IRQ_RXIGN_M                                  0x00000020U
#define PBE_IEEE_REGDEF_IRQ_RXIGN_S                                           5U
#define PBE_IEEE_REGDEF_IRQ_RXIGN_OFF                                0x00000000U
#define PBE_IEEE_REGDEF_IRQ_RXIGN_ACTIVE                             0x00000020U

// Field: [4:4] rxnok
//
// Packet received with CRC error, no data may be trusted
#define PBE_IEEE_REGDEF_IRQ_RXNOK                                    0x00000010U
#define PBE_IEEE_REGDEF_IRQ_RXNOK_M                                  0x00000010U
#define PBE_IEEE_REGDEF_IRQ_RXNOK_S                                           4U
#define PBE_IEEE_REGDEF_IRQ_RXNOK_OFF                                0x00000000U
#define PBE_IEEE_REGDEF_IRQ_RXNOK_ACTIVE                             0x00000010U

// Field: [3:3] prenhack
//
// Packet received is frame version 2 and AR in the FCF is set, inform MCU to start preparing Enh-Ack
#define PBE_IEEE_REGDEF_IRQ_PRENHACK                                 0x00000008U
#define PBE_IEEE_REGDEF_IRQ_PRENHACK_M                               0x00000008U
#define PBE_IEEE_REGDEF_IRQ_PRENHACK_S                                        3U
#define PBE_IEEE_REGDEF_IRQ_PRENHACK_OFF                             0x00000000U
#define PBE_IEEE_REGDEF_IRQ_PRENHACK_ACTIVE                          0x00000008U

// Field: [2:2] ffok
//
// Packet received had a successful frame filtering 
#define PBE_IEEE_REGDEF_IRQ_FFOK                                     0x00000004U
#define PBE_IEEE_REGDEF_IRQ_FFOK_M                                   0x00000004U
#define PBE_IEEE_REGDEF_IRQ_FFOK_S                                            2U
#define PBE_IEEE_REGDEF_IRQ_FFOK_OFF                                 0x00000000U
#define PBE_IEEE_REGDEF_IRQ_FFOK_ACTIVE                              0x00000004U

// Field: [1:1] pingrsp
//
// PBE responds to PBEOP_PING, basic test to see if the battery is connected.
#define PBE_IEEE_REGDEF_IRQ_PINGRSP                                  0x00000002U
#define PBE_IEEE_REGDEF_IRQ_PINGRSP_M                                0x00000002U
#define PBE_IEEE_REGDEF_IRQ_PINGRSP_S                                         1U
#define PBE_IEEE_REGDEF_IRQ_PINGRSP_OFF                              0x00000000U
#define PBE_IEEE_REGDEF_IRQ_PINGRSP_ACTIVE                           0x00000002U

// Field: [0:0] opdone
//
// The operation commanded on PBEAPI has completed succesfully.
#define PBE_IEEE_REGDEF_IRQ_OPDONE                                   0x00000001U
#define PBE_IEEE_REGDEF_IRQ_OPDONE_M                                 0x00000001U
#define PBE_IEEE_REGDEF_IRQ_OPDONE_S                                          0U
#define PBE_IEEE_REGDEF_IRQ_OPDONE_OFF                               0x00000000U
#define PBE_IEEE_REGDEF_IRQ_OPDONE_ACTIVE                            0x00000001U

//******************************************************************************
// Register: API
//******************************************************************************
// Field: [4:0] op
//
// PBE Operation
#define PBE_IEEE_REGDEF_API_OP_W                                              5U
#define PBE_IEEE_REGDEF_API_OP_M                                     0x0000001FU
#define PBE_IEEE_REGDEF_API_OP_S                                              0U
#define PBE_IEEE_REGDEF_API_OP_PING                                  0x00000000U
#define PBE_IEEE_REGDEF_API_OP_STOP                                  0x00000001U
#define PBE_IEEE_REGDEF_API_OP_EOPSTOP                               0x00000002U
#define PBE_IEEE_REGDEF_API_OP_TX                                    0x0000001CU
#define PBE_IEEE_REGDEF_API_OP_RX                                    0x0000001DU
#define PBE_IEEE_REGDEF_API_OP_ENHACKSTOP                            0x0000001EU


#endif // __PBE_IEEE_REGDEF_REGS_H
