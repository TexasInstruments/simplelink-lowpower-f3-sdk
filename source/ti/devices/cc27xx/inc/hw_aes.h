/******************************************************************************
*  Filename:       hw_aes_h
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

#ifndef __HW_AES_H__
#define __HW_AES_H__

//*****************************************************************************
//
// This section defines the register offsets of
// AES component
//
//*****************************************************************************
// Description
#define AES_O_DESC                                                  0x00000000U

// Trigger
#define AES_O_TRG                                                   0x00000010U

// Abort
#define AES_O_ABORT                                                 0x00000014U

// Clear
#define AES_O_CLR                                                   0x00000018U

// Status
#define AES_O_STA                                                   0x0000001CU

// Direct Memory Access
#define AES_O_DMA                                                   0x00000020U

// DMA Channel A Read Write
#define AES_O_DMACHA                                                0x00000024U

// DMA Channel B Read Write
#define AES_O_DMACHB                                                0x00000028U

// Automatic Configuration
#define AES_O_AUTOCFG                                               0x0000002CU

// Key Word 0
#define AES_O_KEY0                                                  0x00000050U

// Key Word 1
#define AES_O_KEY1                                                  0x00000054U

// Key Word 2
#define AES_O_KEY2                                                  0x00000058U

// Key Word 3
#define AES_O_KEY3                                                  0x0000005CU

// Text Word 0
#define AES_O_TXT0                                                  0x00000070U

// Text Word 1
#define AES_O_TXT1                                                  0x00000074U

// Text Word 2
#define AES_O_TXT2                                                  0x00000078U

// Text Word 3
#define AES_O_TXT3                                                  0x0000007CU

// Text Word 0 XOR
#define AES_O_TXTX0                                                 0x00000080U

// Text Word 1 XOR
#define AES_O_TXTX1                                                 0x00000084U

// Text Word 2 XOR
#define AES_O_TXTX2                                                 0x00000088U

// Text Word 3 XOR
#define AES_O_TXTX3                                                 0x0000008CU

// Buffer Word 0
#define AES_O_BUF0                                                  0x00000090U

// Buffer Word 1
#define AES_O_BUF1                                                  0x00000094U

// Buffer Word 2
#define AES_O_BUF2                                                  0x00000098U

// Buffer Word 3
#define AES_O_BUF3                                                  0x0000009CU

// Text Word 0 XOR Buffer Word 0
#define AES_O_TXTXBUF0                                              0x000000A0U

// Text Word 1 XOR Buffer Word 1
#define AES_O_TXTXBUF1                                              0x000000A4U

// Text Word 2 XOR Buffer Word 2
#define AES_O_TXTXBUF2                                              0x000000A8U

// Text Word 3 XOR Buffer Word3
#define AES_O_TXTXBUF3                                              0x000000ACU

// Interrupt Mask
#define AES_O_IMASK                                                 0x00000104U

// Raw Interrupt Status
#define AES_O_RIS                                                   0x00000108U

// Masked Interrupt Status
#define AES_O_MIS                                                   0x0000010CU

// Interrupt Set
#define AES_O_ISET                                                  0x00000110U

// Interrupt Clear
#define AES_O_ICLR                                                  0x00000114U

// Interrupt Mask Set
#define AES_O_IMSET                                                 0x00000118U

// Interrupt Mask Clear
#define AES_O_IMCLR                                                 0x0000011CU

//*****************************************************************************
//
// Register: AES_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module Identifier
#define AES_DESC_MODID_W                                                    16U
#define AES_DESC_MODID_M                                            0xFFFF0000U
#define AES_DESC_MODID_S                                                    16U

// Field: [15:12] STDIPOFF
//
// 64 B Standard IP MMR Block
//
// 0: These MMRs do not exist
// 1:15: These MMRs begin at offset 64*STDIPOFF from IP base address
#define AES_DESC_STDIPOFF_W                                                  4U
#define AES_DESC_STDIPOFF_M                                         0x0000F000U
#define AES_DESC_STDIPOFF_S                                                 12U

// Field:  [11:8] INSTIDX
//
// Instance Number
#define AES_DESC_INSTIDX_W                                                   4U
#define AES_DESC_INSTIDX_M                                          0x00000F00U
#define AES_DESC_INSTIDX_S                                                   8U

// Field:   [7:4] MAJREV
//
// Major Revision
#define AES_DESC_MAJREV_W                                                    4U
#define AES_DESC_MAJREV_M                                           0x000000F0U
#define AES_DESC_MAJREV_S                                                    4U

// Field:   [3:0] MINREV
//
// Minor Revision
#define AES_DESC_MINREV_W                                                    4U
#define AES_DESC_MINREV_M                                           0x0000000FU
#define AES_DESC_MINREV_S                                                    0U

//*****************************************************************************
//
// Register: AES_O_TRG
//
//*****************************************************************************
// Field:     [3] DMACHA
//
// Write 1 to this field to manually trigger channel A request.
#define AES_TRG_DMACHA                                              0x00000008U
#define AES_TRG_DMACHA_M                                            0x00000008U
#define AES_TRG_DMACHA_S                                                     3U

// Field:     [2] DMACHB
//
// Write 1 to this field to manually trigger channel B request.
#define AES_TRG_DMACHB                                              0x00000004U
#define AES_TRG_DMACHB_M                                            0x00000004U
#define AES_TRG_DMACHB_S                                                     2U

// Field:   [1:0] ECBOP
//
// Electronic Codebook (ECB) Operation
//
// Write an enumerated value to this field when STA.STATE = IDLE to manually
// trigger an ECB encryption. If condition is not met, the trigger is ignored.
// Non-enumerated values are ignored.
// ENUMs:
// TXTXBUF                  TXT = ECB(KEY, TXT XOR BUF)
// BUF                      TXT = ECB(KEY,BUF)
// TXT                      TXT = ECB(KEY,TXT)
#define AES_TRG_ECBOP_W                                                      2U
#define AES_TRG_ECBOP_M                                             0x00000003U
#define AES_TRG_ECBOP_S                                                      0U
#define AES_TRG_ECBOP_TXTXBUF                                       0x00000003U
#define AES_TRG_ECBOP_BUF                                           0x00000002U
#define AES_TRG_ECBOP_TXT                                           0x00000001U

//*****************************************************************************
//
// Register: AES_O_ABORT
//
//*****************************************************************************
// Field:     [0] ECB
//
// Electronic Codebook
//
// Write 1 to this field to abort an ongoing ECB encryption. An abort will
// clear TXT, BUF, DMA, AUTOCFG registers
#define AES_ABORT_ECB                                               0x00000001U
#define AES_ABORT_ECB_M                                             0x00000001U
#define AES_ABORT_ECB_S                                                      0U

//*****************************************************************************
//
// Register: AES_O_CLR
//
//*****************************************************************************
// Field:     [1] TXT
//
// Write 1 to this field to clear TXT.
#define AES_CLR_TXT                                                 0x00000002U
#define AES_CLR_TXT_M                                               0x00000002U
#define AES_CLR_TXT_S                                                        1U

// Field:     [0] BUF
//
// Write 1 to this field to clear BUF.
#define AES_CLR_BUF                                                 0x00000001U
#define AES_CLR_BUF_M                                               0x00000001U
#define AES_CLR_BUF_S                                                        0U

//*****************************************************************************
//
// Register: AES_O_STA
//
//*****************************************************************************
// Field:   [4:3] KEYMST
//
// This field gives the status of which master has written KEY MMR's.
// ENUMs:
// HSM                      Value of Keys written by HSM
// CORE                     Value of Key MMRs written by CM33
#define AES_STA_KEYMST_W                                                     2U
#define AES_STA_KEYMST_M                                            0x00000018U
#define AES_STA_KEYMST_S                                                     3U
#define AES_STA_KEYMST_HSM                                          0x00000018U
#define AES_STA_KEYMST_CORE                                         0x00000000U

// Field:     [2] KEYSTATE
//
// This field gives status of valid data in KEY MMR's
// ENUMs:
// VALID                    KEY MMR's are written with valid data
// EMPTY                    KEY MMRs are empty or partially written
#define AES_STA_KEYSTATE                                            0x00000004U
#define AES_STA_KEYSTATE_M                                          0x00000004U
#define AES_STA_KEYSTATE_S                                                   2U
#define AES_STA_KEYSTATE_VALID                                      0x00000004U
#define AES_STA_KEYSTATE_EMPTY                                      0x00000000U

// Field:     [1] BUFSTA
//
//  BUF Status
//
// Field gives the status of BUF, indicating EMPTY or FULL, when AUTOCFG.TRGECB
// = WRBUF3.
// If AUTOCFG.TRGECB != WRBUF3, then STA.BUFSTA will hold the value 0.
// Note : Useful for CBC-MAC
// ENUMs:
// FULL                     Data stored in BUF is not yet consumed by the AES
//                          engine. Next block of data cannot be written
//                          into BUF until STA.STATE = IDLE.
// EMPTY                    Data stored in BUF is already consumed by the AES
//                          engine and next block of data can be written in
//                          BUF
#define AES_STA_BUFSTA                                              0x00000002U
#define AES_STA_BUFSTA_M                                            0x00000002U
#define AES_STA_BUFSTA_S                                                     1U
#define AES_STA_BUFSTA_FULL                                         0x00000002U
#define AES_STA_BUFSTA_EMPTY                                        0x00000000U

// Field:     [0] STATE
//
// State
//
// Field gives the state of the ECB encryption engine.
// ENUMs:
// BUSY                     ECB encryption active.
// IDLE                     ECB is IDLE.
#define AES_STA_STATE                                               0x00000001U
#define AES_STA_STATE_M                                             0x00000001U
#define AES_STA_STATE_S                                                      0U
#define AES_STA_STATE_BUSY                                          0x00000001U
#define AES_STA_STATE_IDLE                                          0x00000000U

//*****************************************************************************
//
// Register: AES_O_DMA
//
//*****************************************************************************
// Field: [19:16] DONEACT
//
// Done Action
//
// This field determines the side effects of DMA done. It is allowed to
// configure this field with an OR-combination of supported enums, with the
// exception that GATE_TRGECB_ON_CHA and GATE_TRGECB_ON_CHA_DEL shall be
// mutually exclusive
// ENUMs:
// CLR_TXT_ON_CHB           DMA channel B done event clears TXT0 thru TXT3 if
//                          STA.STATE = IDLE. Event is ignored if condition
//                          is not met.
// CLR_TXT_ON_CHA           DMA channel A done event clears TXT0 thru TXT3 if
//                          STA.STATE = IDLE. Event is ignored if condition
//                          is not met.
// GATE_TRGECB_ON_CHA_DEL   Due to the pipelining of BUF writes, in certain
//                          modes, DMA CHA Done appears before the last but
//                          one ECB has completed. Setting this bit, will
//                          gate the triggers defined in AUTOCFG.TRGECB
//                          only after the last write by CHA is consumed by
//                          AES FSM. Used in ECB,CBC,CBC-MAC modes (having
//                          multiple blocks encryption) to avoid spurious
//                          ECB triggered on last read by CHB. For single
//                          mode encryption, DMA.GATE_TRGECB_ON_CHA should
//                          be used.
// GATE_TRGECB_ON_CHA       Triggers defined in AUTOCFG.TRGECB are gated when
//                          RIS.CHADONE  = SET.
// DIS                      DMA done has no side effect.
#define AES_DMA_DONEACT_W                                                    4U
#define AES_DMA_DONEACT_M                                           0x000F0000U
#define AES_DMA_DONEACT_S                                                   16U
#define AES_DMA_DONEACT_CLR_TXT_ON_CHB                              0x00080000U
#define AES_DMA_DONEACT_CLR_TXT_ON_CHA                              0x00040000U
#define AES_DMA_DONEACT_GATE_TRGECB_ON_CHA_DEL                      0x00020000U
#define AES_DMA_DONEACT_GATE_TRGECB_ON_CHA                          0x00010000U
#define AES_DMA_DONEACT_DIS                                         0x00000000U

// Field: [13:12] ADRCHB
//
// Channel B Read Write Address
//
// The DMA accesses DMACHB to read or write contents of TXT and BUF as a
// response to a burst request. This field specifes the start address of the
// first DMA transfer that follows the burst request. The internal address gets
// incremented automatically for subsequent accesses. The DMA shall transfer
// 8-bit, 16-bit, or 32-bit words, and shall always complete a 16-byte transfer
// before re-arbitration.
// ENUMs:
// TXTXBUF0                 Start address is TXTXBUF0.
// BUF0                     Start address is BUF0.
// TXTX0                    Start address is TXTX0.
// TXT0                     Start address is TXT0.
#define AES_DMA_ADRCHB_W                                                     2U
#define AES_DMA_ADRCHB_M                                            0x00003000U
#define AES_DMA_ADRCHB_S                                                    12U
#define AES_DMA_ADRCHB_TXTXBUF0                                     0x00003000U
#define AES_DMA_ADRCHB_BUF0                                         0x00002000U
#define AES_DMA_ADRCHB_TXTX0                                        0x00001000U
#define AES_DMA_ADRCHB_TXT0                                         0x00000000U

// Field:  [10:8] TRGCHB
//
// Channel B Trigger
//
// Select the condition that trigger DMA channel B request. Non-enumerated
// values are not supported and ignored.
// ENUMs:
// RDTXT3                   Reads of TXT3, or TXTXBUF3 generates request.
// WRTXT3                   Writes to TXT3, TXTX3, or TXTXBUF3 generates
//                          request.
// ECBDONE                  Completion of ECB encryption triggers request.
// ECBSTART                 Start of ECB encryption triggers request.
// DIS                      DMA requests are disabled.
#define AES_DMA_TRGCHB_W                                                     3U
#define AES_DMA_TRGCHB_M                                            0x00000700U
#define AES_DMA_TRGCHB_S                                                     8U
#define AES_DMA_TRGCHB_RDTXT3                                       0x00000400U
#define AES_DMA_TRGCHB_WRTXT3                                       0x00000300U
#define AES_DMA_TRGCHB_ECBDONE                                      0x00000200U
#define AES_DMA_TRGCHB_ECBSTART                                     0x00000100U
#define AES_DMA_TRGCHB_DIS                                          0x00000000U

// Field:   [5:4] ADRCHA
//
// Channel A Read Write Address
//
// The DMA accesses DMACHA to read or write contents of TXT and BUF as a
// response to a burst request. This field specifes the start address of the
// first DMA transfer that follows the burst request. The internal address gets
// incremented automatically for subsequent accesses. The DMA shall transfer
// 8-bit, 16-bit, or 32-bit words, and shall always complete a 16-byte transfer
// before re-arbitration.
// ENUMs:
// TXTXBUF0                 Start address is TXTXBUF0.
// BUF0                     Start address is BUF0.
// TXTX0                    Start address is TXTX0.
// TXT0                     Start address is TXT0.
#define AES_DMA_ADRCHA_W                                                     2U
#define AES_DMA_ADRCHA_M                                            0x00000030U
#define AES_DMA_ADRCHA_S                                                     4U
#define AES_DMA_ADRCHA_TXTXBUF0                                     0x00000030U
#define AES_DMA_ADRCHA_BUF0                                         0x00000020U
#define AES_DMA_ADRCHA_TXTX0                                        0x00000010U
#define AES_DMA_ADRCHA_TXT0                                         0x00000000U

// Field:   [2:0] TRGCHA
//
// Channel A Trigger
//
// Select the condition that trigger DMA channel A request. Non-enumerated
// values are not supported and ignored.
// ENUMs:
// RDTXT3                   Reads of TXT3 or TXTXBUF3 trigger request.
// WRTXT3                   Writes to TXT3 or TXTX3 trigger request.
// ECBDONE                  Completion of ECB encryption trigger request.
// ECBSTART                 Start of ECB encryption trigger request.
// DIS                      DMA requests are disabled.
#define AES_DMA_TRGCHA_W                                                     3U
#define AES_DMA_TRGCHA_M                                            0x00000007U
#define AES_DMA_TRGCHA_S                                                     0U
#define AES_DMA_TRGCHA_RDTXT3                                       0x00000004U
#define AES_DMA_TRGCHA_WRTXT3                                       0x00000003U
#define AES_DMA_TRGCHA_ECBDONE                                      0x00000002U
#define AES_DMA_TRGCHA_ECBSTART                                     0x00000001U
#define AES_DMA_TRGCHA_DIS                                          0x00000000U

//*****************************************************************************
//
// Register: AES_O_DMACHA
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Value
#define AES_DMACHA_VAL_W                                                    32U
#define AES_DMACHA_VAL_M                                            0xFFFFFFFFU
#define AES_DMACHA_VAL_S                                                     0U

//*****************************************************************************
//
// Register: AES_O_DMACHB
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Value
#define AES_DMACHB_VAL_W                                                    32U
#define AES_DMACHB_VAL_M                                            0xFFFFFFFFU
#define AES_DMACHB_VAL_S                                                     0U

//*****************************************************************************
//
// Register: AES_O_AUTOCFG
//
//*****************************************************************************
// Field:    [28] CHBDONECLR
//
// This field enable auto-clear of RIS.CHBDONE interrupt on read/write of
// TXT3/BUF3/TXTX3/TXTXBUF3 .
//
// ENUMs:
// EN                       Enable auto-clear of  RIS.CHBDONE interrupt
// DIS                      Disable auto-clear of  RIS.CHBDONE interrupt
#define AES_AUTOCFG_CHBDONECLR                                      0x10000000U
#define AES_AUTOCFG_CHBDONECLR_M                                    0x10000000U
#define AES_AUTOCFG_CHBDONECLR_S                                            28U
#define AES_AUTOCFG_CHBDONECLR_EN                                   0x10000000U
#define AES_AUTOCFG_CHBDONECLR_DIS                                  0x00000000U

// Field:    [27] CHADONECLR
//
// This field enables auto-clear of RIS.CHADONE interrupt on read/write of
// TXT3/BUF3/TXTX3/TXTXBUF3 .
//
// ENUMs:
// EN                       Enable auto-clear of  RIS.CHADONE interrupt
// DIS                      Disable auto-clear of  RIS.CHADONE interrupt
#define AES_AUTOCFG_CHADONECLR                                      0x08000000U
#define AES_AUTOCFG_CHADONECLR_M                                    0x08000000U
#define AES_AUTOCFG_CHADONECLR_S                                            27U
#define AES_AUTOCFG_CHADONECLR_EN                                   0x08000000U
#define AES_AUTOCFG_CHADONECLR_DIS                                  0x00000000U

// Field:    [26] ECBSTARTCLR
//
// This field enables auto-clear of RIS.ECBSTART interrupt on read/write of
// TXT3/BUF3/TXTX3/TXTXBUF3 .
// ENUMs:
// EN                       Enable auto-clear of  RIS.ECBSTART interrupt
// DIS                      Disable auto-clear of  RIS.ECBSTART interrupt
#define AES_AUTOCFG_ECBSTARTCLR                                     0x04000000U
#define AES_AUTOCFG_ECBSTARTCLR_M                                   0x04000000U
#define AES_AUTOCFG_ECBSTARTCLR_S                                           26U
#define AES_AUTOCFG_ECBSTARTCLR_EN                                  0x04000000U
#define AES_AUTOCFG_ECBSTARTCLR_DIS                                 0x00000000U

// Field:    [25] ECBDONECLR
//
// This field enables auto-clear of RIS.ECBDONE interrupt on read/write of
// TXT3/BUF3/TXTX3/TXTXBUF3 .
//
// ENUMs:
// EN                       Enable auto-clear of  RIS.ECBDONE interrupt
// DIS                      Disable auto-clear of RIS.ECBDONE interrupt
#define AES_AUTOCFG_ECBDONECLR                                      0x02000000U
#define AES_AUTOCFG_ECBDONECLR_M                                    0x02000000U
#define AES_AUTOCFG_ECBDONECLR_S                                            25U
#define AES_AUTOCFG_ECBDONECLR_EN                                   0x02000000U
#define AES_AUTOCFG_ECBDONECLR_DIS                                  0x00000000U

// Field:    [24] BUSHALT
//
// Bus Halt
//
// This field decides if bus halts on access to KEY, TXT, BUF, TXTX and TXTXBUF
// when STA.STATE = BUSY.
// ENUMs:
// EN                       Enable bus halt
//
//                          When STA.STATE = BUSY,
//                          access to KEY, TXT, TXTX, TXTXBUF halt the bus
//                          until STA.STATE = IDLE.
//                          When STA.STATE = BUSY and
//                          if either STA.BUFSTA = FULL or AUTOCFG.CTRSIZE
//                          != DISABLE, access to BUF halts the bus until
//                          STA.STATE = IDLE.
// DIS                      Disable bus halt
//
//                          When STA.STATE = BUSY,
//                          writes to KEY, TXT, TXTX are ignored, reads
//                          from  TXT, TXTXBUF return zero.
//                          When STA.STATE = BUSY and
//                          if either STA.BUFSTA = FULL or AUTOCFG.CTRSIZE
//                          != DISABLE, writes to BUF are ignored, reads
//                          return zero.
#define AES_AUTOCFG_BUSHALT                                         0x01000000U
#define AES_AUTOCFG_BUSHALT_M                                       0x01000000U
#define AES_AUTOCFG_BUSHALT_S                                               24U
#define AES_AUTOCFG_BUSHALT_EN                                      0x01000000U
#define AES_AUTOCFG_BUSHALT_DIS                                     0x00000000U

// Field: [21:19] CTRSIZE
//
// Configures  size of counter as either 8,16,32,64 or 128
// Non-enumerated values are not supported and ignored
// ENUMs:
// CTR128                   Configures counter size as 128 bit
// CTR64                    Configures counter size as 64 bit
// CTR32                    Configures counter size as 32 bit
// CTR16                    Configures counter size as 16 bit
// CTR8                     Configures counter size as 8 bit
// DIS                      Disable CTR operation
#define AES_AUTOCFG_CTRSIZE_W                                                3U
#define AES_AUTOCFG_CTRSIZE_M                                       0x00380000U
#define AES_AUTOCFG_CTRSIZE_S                                               19U
#define AES_AUTOCFG_CTRSIZE_CTR128                                  0x00280000U
#define AES_AUTOCFG_CTRSIZE_CTR64                                   0x00200000U
#define AES_AUTOCFG_CTRSIZE_CTR32                                   0x00180000U
#define AES_AUTOCFG_CTRSIZE_CTR16                                   0x00100000U
#define AES_AUTOCFG_CTRSIZE_CTR8                                    0x00080000U
#define AES_AUTOCFG_CTRSIZE_DIS                                     0x00000000U

// Field:    [18] CTRALIGN
//
// ENUMs:
// RIGHTALIGN               Indicates right aligned counter
//                          Not applicable when
//                          counter size is 128bit
//                          For 128 bit counter, all
//                          octets will be considered
//
//                          If right aligned, octet
//                          8-15 will be considered based on endianness and
//                          counter size
// LEFTALIGN                Indicates Left Aligned Counter
//                          Not applicable for 128
//                          bit counter size.
//                          For 128 bit counter, all
//                          octets will be considered
//
//                          When left aligned,,octet
//                          0-7 will be considered , based on counter size
//                          and endianness
#define AES_AUTOCFG_CTRALIGN                                        0x00040000U
#define AES_AUTOCFG_CTRALIGN_M                                      0x00040000U
#define AES_AUTOCFG_CTRALIGN_S                                              18U
#define AES_AUTOCFG_CTRALIGN_RIGHTALIGN                             0x00040000U
#define AES_AUTOCFG_CTRALIGN_LEFTALIGN                              0x00000000U

// Field:    [17] CTRENDIAN
//
// Counter Endianness
//
// Specifies Endianness of counter
// ENUMs:
// BIGENDIAN                Specifies Big Endian Counter
//                          Carry will flow from
//                          octet 'n' to octet 'n-1'
// LITTLEENDIAN             Specifies Little Endian Counter.
//                          Carry will flow from
//                          octet 'n' to octet 'n+1'
#define AES_AUTOCFG_CTRENDIAN                                       0x00020000U
#define AES_AUTOCFG_CTRENDIAN_M                                     0x00020000U
#define AES_AUTOCFG_CTRENDIAN_S                                             17U
#define AES_AUTOCFG_CTRENDIAN_BIGENDIAN                             0x00020000U
#define AES_AUTOCFG_CTRENDIAN_LITTLEENDIAN                          0x00000000U

// Field:   [9:8] TRGTXT
//
// Trigger for TXT
//
// This field determines if and when HW automatically XORs BUF into TXT.
// Non-enumerated values are not supported and ignored. It is allowed to
// configure this field with an OR-combination of supported enums.
// ENUMs:
// RDTXTXBUF3               HW XORs content of BUF into TXT upon read of
//                          TXTXBUF3.
// RDTXT3                   HW XORs content of BUF into TXT upon read of TXT3.
// DIS                      No HW update of TXT
#define AES_AUTOCFG_TRGTXT_W                                                 2U
#define AES_AUTOCFG_TRGTXT_M                                        0x00000300U
#define AES_AUTOCFG_TRGTXT_S                                                 8U
#define AES_AUTOCFG_TRGTXT_RDTXTXBUF3                               0x00000200U
#define AES_AUTOCFG_TRGTXT_RDTXT3                                   0x00000100U
#define AES_AUTOCFG_TRGTXT_DIS                                      0x00000000U

// Field:   [5:4] ECBSRC
//
// Electronic Codebook Source
//
// This field specifies the data source to HW-triggered ECB encryptions.
// Non-enumerated values are not supported and ignored.
// ENUMs:
// TXTXBUF                  TXT = ECB(KEY, TXT XOR BUF)
// BUF                      TXT = ECB(KEY,BUF)
// TXT                      TXT = ECB(KEY,TXT)
#define AES_AUTOCFG_ECBSRC_W                                                 2U
#define AES_AUTOCFG_ECBSRC_M                                        0x00000030U
#define AES_AUTOCFG_ECBSRC_S                                                 4U
#define AES_AUTOCFG_ECBSRC_TXTXBUF                                  0x00000030U
#define AES_AUTOCFG_ECBSRC_BUF                                      0x00000020U
#define AES_AUTOCFG_ECBSRC_TXT                                      0x00000010U

// Field:   [3:0] TRGECB
//
// Trigger Electronic Codebook
//
// This field specifies one or more actions that indirectly trigger operation
// selected in ECB.
// It is allowed to configure this field with an OR-combination of supported
// enums.
// ENUMs:
// WRBUF3S                  Write to BUF3 schedules to trigger single action
//                          once STA.STATE is or becomes IDLE. No action
//                          gets triggered unless condition is met.
//                          Subsequent writes does not trigger any action
//                          unless you write this setting again to this
//                          field.
// WRBUF3                   When AUTOCFG.CTRSIZE = DIS, all writes to BUF3
//                          schedule to trigger action once STA.STATE is or
//                          becomes IDLE. No action gets triggered unless
//                          condition is met.
// RDTXT3                   When STA.STATE = IDLE, all reads of TXT3 or
//                          TXTXBUF3 trigger action. No action gets
//                          triggered unless condition is met.
// WRTXT3                   When STA.STATE = IDLE, all writes to TXT3 or TXTX3
//                          trigger action. No action gets triggered unless
//                          condition is met.
// DIS                      No user action indirectly trigger ECB.
#define AES_AUTOCFG_TRGECB_W                                                 4U
#define AES_AUTOCFG_TRGECB_M                                        0x0000000FU
#define AES_AUTOCFG_TRGECB_S                                                 0U
#define AES_AUTOCFG_TRGECB_WRBUF3S                                  0x00000008U
#define AES_AUTOCFG_TRGECB_WRBUF3                                   0x00000004U
#define AES_AUTOCFG_TRGECB_RDTXT3                                   0x00000002U
#define AES_AUTOCFG_TRGECB_WRTXT3                                   0x00000001U
#define AES_AUTOCFG_TRGECB_DIS                                      0x00000000U

//*****************************************************************************
//
// Register: AES_O_KEY0
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// Value of KEY[31:0].
#define AES_KEY0_VAL_W                                                      32U
#define AES_KEY0_VAL_M                                              0xFFFFFFFFU
#define AES_KEY0_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_KEY1
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// KEY[63:32]
#define AES_KEY1_VAL_W                                                      32U
#define AES_KEY1_VAL_M                                              0xFFFFFFFFU
#define AES_KEY1_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_KEY2
//
//*****************************************************************************
// Field:  [31:0] VAL
//
//  KEY[95:64]
#define AES_KEY2_VAL_W                                                      32U
#define AES_KEY2_VAL_M                                              0xFFFFFFFFU
#define AES_KEY2_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_KEY3
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// KEY[127:96]
#define AES_KEY3_VAL_W                                                      32U
#define AES_KEY3_VAL_M                                              0xFFFFFFFFU
#define AES_KEY3_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_TXT0
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// TXT[31:0]
#define AES_TXT0_VAL_W                                                      32U
#define AES_TXT0_VAL_M                                              0xFFFFFFFFU
#define AES_TXT0_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_TXT1
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// TXT[63:32]
#define AES_TXT1_VAL_W                                                      32U
#define AES_TXT1_VAL_M                                              0xFFFFFFFFU
#define AES_TXT1_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_TXT2
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// TXT[95:64]
#define AES_TXT2_VAL_W                                                      32U
#define AES_TXT2_VAL_M                                              0xFFFFFFFFU
#define AES_TXT2_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_TXT3
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// TXT[127:96]
//
// AUTOCFG.TRGECB decides if a write to or a read of this field trigger an
// encryption.
#define AES_TXT3_VAL_W                                                      32U
#define AES_TXT3_VAL_M                                              0xFFFFFFFFU
#define AES_TXT3_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_TXTX0
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// TXT0.VAL = VAL XOR TXT0.VAL
#define AES_TXTX0_VAL_W                                                     32U
#define AES_TXTX0_VAL_M                                             0xFFFFFFFFU
#define AES_TXTX0_VAL_S                                                      0U

//*****************************************************************************
//
// Register: AES_O_TXTX1
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// TXT1.VAL = VAL XOR TXT1.VAL
#define AES_TXTX1_VAL_W                                                     32U
#define AES_TXTX1_VAL_M                                             0xFFFFFFFFU
#define AES_TXTX1_VAL_S                                                      0U

//*****************************************************************************
//
// Register: AES_O_TXTX2
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// TXT2.VAL = VAL XOR TXT2.VAL
#define AES_TXTX2_VAL_W                                                     32U
#define AES_TXTX2_VAL_M                                             0xFFFFFFFFU
#define AES_TXTX2_VAL_S                                                      0U

//*****************************************************************************
//
// Register: AES_O_TXTX3
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// TXT3.VAL = VAL XOR TXT3.VAL
#define AES_TXTX3_VAL_W                                                     32U
#define AES_TXTX3_VAL_M                                             0xFFFFFFFFU
#define AES_TXTX3_VAL_S                                                      0U

//*****************************************************************************
//
// Register: AES_O_BUF0
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// BUF[31:0]
#define AES_BUF0_VAL_W                                                      32U
#define AES_BUF0_VAL_M                                              0xFFFFFFFFU
#define AES_BUF0_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_BUF1
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// BUF[63:32]
#define AES_BUF1_VAL_W                                                      32U
#define AES_BUF1_VAL_M                                              0xFFFFFFFFU
#define AES_BUF1_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_BUF2
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// BUF[95:64]
#define AES_BUF2_VAL_W                                                      32U
#define AES_BUF2_VAL_M                                              0xFFFFFFFFU
#define AES_BUF2_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_BUF3
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// BUF[127:96]
#define AES_BUF3_VAL_W                                                      32U
#define AES_BUF3_VAL_M                                              0xFFFFFFFFU
#define AES_BUF3_VAL_S                                                       0U

//*****************************************************************************
//
// Register: AES_O_TXTXBUF0
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// VAL = TXT0.VAL XOR BUF0.VAL
#define AES_TXTXBUF0_VAL_W                                                  32U
#define AES_TXTXBUF0_VAL_M                                          0xFFFFFFFFU
#define AES_TXTXBUF0_VAL_S                                                   0U

//*****************************************************************************
//
// Register: AES_O_TXTXBUF1
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// VAL = TXT1.VAL XOR BUF1.VAL
#define AES_TXTXBUF1_VAL_W                                                  32U
#define AES_TXTXBUF1_VAL_M                                          0xFFFFFFFFU
#define AES_TXTXBUF1_VAL_S                                                   0U

//*****************************************************************************
//
// Register: AES_O_TXTXBUF2
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// VAL = TXT2.VAL XOR BUF2.VAL
#define AES_TXTXBUF2_VAL_W                                                  32U
#define AES_TXTXBUF2_VAL_M                                          0xFFFFFFFFU
#define AES_TXTXBUF2_VAL_S                                                   0U

//*****************************************************************************
//
// Register: AES_O_TXTXBUF3
//
//*****************************************************************************
// Field:  [31:0] VAL
//
// VAL = TXT3.VAL XOR BUF3.VAL
#define AES_TXTXBUF3_VAL_W                                                  32U
#define AES_TXTXBUF3_VAL_M                                          0xFFFFFFFFU
#define AES_TXTXBUF3_VAL_S                                                   0U

//*****************************************************************************
//
// Register: AES_O_IMASK
//
//*****************************************************************************
// Field:     [3] CHBDONE
//
// DMA Channel B Done Interrupt
// ENUMs:
// EN
// DIS
#define AES_IMASK_CHBDONE                                           0x00000008U
#define AES_IMASK_CHBDONE_M                                         0x00000008U
#define AES_IMASK_CHBDONE_S                                                  3U
#define AES_IMASK_CHBDONE_EN                                        0x00000008U
#define AES_IMASK_CHBDONE_DIS                                       0x00000000U

// Field:     [2] CHADONE
//
// DMA Channel A Done Interrupt
// ENUMs:
// EN
// DIS
#define AES_IMASK_CHADONE                                           0x00000004U
#define AES_IMASK_CHADONE_M                                         0x00000004U
#define AES_IMASK_CHADONE_S                                                  2U
#define AES_IMASK_CHADONE_EN                                        0x00000004U
#define AES_IMASK_CHADONE_DIS                                       0x00000000U

// Field:     [1] ECBSTART
//
// ECB Start Interrupt
// ENUMs:
// EN
// DIS
#define AES_IMASK_ECBSTART                                          0x00000002U
#define AES_IMASK_ECBSTART_M                                        0x00000002U
#define AES_IMASK_ECBSTART_S                                                 1U
#define AES_IMASK_ECBSTART_EN                                       0x00000002U
#define AES_IMASK_ECBSTART_DIS                                      0x00000000U

// Field:     [0] ECBDONE
//
// ECB Done Interrupt
// ENUMs:
// EN
// DIS
#define AES_IMASK_ECBDONE                                           0x00000001U
#define AES_IMASK_ECBDONE_M                                         0x00000001U
#define AES_IMASK_ECBDONE_S                                                  0U
#define AES_IMASK_ECBDONE_EN                                        0x00000001U
#define AES_IMASK_ECBDONE_DIS                                       0x00000000U

//*****************************************************************************
//
// Register: AES_O_RIS
//
//*****************************************************************************
// Field:     [3] CHBDONE
//
// DMA Channel B Done Interrupt
// ENUMs:
// SET
// CLR
#define AES_RIS_CHBDONE                                             0x00000008U
#define AES_RIS_CHBDONE_M                                           0x00000008U
#define AES_RIS_CHBDONE_S                                                    3U
#define AES_RIS_CHBDONE_SET                                         0x00000008U
#define AES_RIS_CHBDONE_CLR                                         0x00000000U

// Field:     [2] CHADONE
//
// DMA Channel A Done Interrupt
// ENUMs:
// SET
// CLR
#define AES_RIS_CHADONE                                             0x00000004U
#define AES_RIS_CHADONE_M                                           0x00000004U
#define AES_RIS_CHADONE_S                                                    2U
#define AES_RIS_CHADONE_SET                                         0x00000004U
#define AES_RIS_CHADONE_CLR                                         0x00000000U

// Field:     [1] ECBSTART
//
// ECB Start Interrupt
// ENUMs:
// SET
// CLR
#define AES_RIS_ECBSTART                                            0x00000002U
#define AES_RIS_ECBSTART_M                                          0x00000002U
#define AES_RIS_ECBSTART_S                                                   1U
#define AES_RIS_ECBSTART_SET                                        0x00000002U
#define AES_RIS_ECBSTART_CLR                                        0x00000000U

// Field:     [0] ECBDONE
//
// ECB Done Interrupt
// ENUMs:
// SET
// CLR
#define AES_RIS_ECBDONE                                             0x00000001U
#define AES_RIS_ECBDONE_M                                           0x00000001U
#define AES_RIS_ECBDONE_S                                                    0U
#define AES_RIS_ECBDONE_SET                                         0x00000001U
#define AES_RIS_ECBDONE_CLR                                         0x00000000U

//*****************************************************************************
//
// Register: AES_O_MIS
//
//*****************************************************************************
// Field:     [3] CHBDONE
//
// DMA Channel B Done Interrupt
// ENUMs:
// SET
// CLR
#define AES_MIS_CHBDONE                                             0x00000008U
#define AES_MIS_CHBDONE_M                                           0x00000008U
#define AES_MIS_CHBDONE_S                                                    3U
#define AES_MIS_CHBDONE_SET                                         0x00000008U
#define AES_MIS_CHBDONE_CLR                                         0x00000000U

// Field:     [2] CHADONE
//
// DMA Channel A Done Interrupt
// ENUMs:
// SET
// CLR
#define AES_MIS_CHADONE                                             0x00000004U
#define AES_MIS_CHADONE_M                                           0x00000004U
#define AES_MIS_CHADONE_S                                                    2U
#define AES_MIS_CHADONE_SET                                         0x00000004U
#define AES_MIS_CHADONE_CLR                                         0x00000000U

// Field:     [1] ECBSTART
//
// ECB Start Interrupt
// ENUMs:
// SET
// CLR
#define AES_MIS_ECBSTART                                            0x00000002U
#define AES_MIS_ECBSTART_M                                          0x00000002U
#define AES_MIS_ECBSTART_S                                                   1U
#define AES_MIS_ECBSTART_SET                                        0x00000002U
#define AES_MIS_ECBSTART_CLR                                        0x00000000U

// Field:     [0] ECBDONE
//
// ECB Done Interrupt
// ENUMs:
// SET
// CLR
#define AES_MIS_ECBDONE                                             0x00000001U
#define AES_MIS_ECBDONE_M                                           0x00000001U
#define AES_MIS_ECBDONE_S                                                    0U
#define AES_MIS_ECBDONE_SET                                         0x00000001U
#define AES_MIS_ECBDONE_CLR                                         0x00000000U

//*****************************************************************************
//
// Register: AES_O_ISET
//
//*****************************************************************************
// Field:     [3] CHBDONE
//
// DMA Channel B Done Interrupt
// ENUMs:
// SET
// NO_EFFECT
#define AES_ISET_CHBDONE                                            0x00000008U
#define AES_ISET_CHBDONE_M                                          0x00000008U
#define AES_ISET_CHBDONE_S                                                   3U
#define AES_ISET_CHBDONE_SET                                        0x00000008U
#define AES_ISET_CHBDONE_NO_EFFECT                                  0x00000000U

// Field:     [2] CHADONE
//
// DMA Channel A Done Interrupt
// ENUMs:
// SET
// NO_EFFECT
#define AES_ISET_CHADONE                                            0x00000004U
#define AES_ISET_CHADONE_M                                          0x00000004U
#define AES_ISET_CHADONE_S                                                   2U
#define AES_ISET_CHADONE_SET                                        0x00000004U
#define AES_ISET_CHADONE_NO_EFFECT                                  0x00000000U

// Field:     [1] ECBSTART
//
// ECB Start Interrupt
// ENUMs:
// SET
// NO_EFFECT
#define AES_ISET_ECBSTART                                           0x00000002U
#define AES_ISET_ECBSTART_M                                         0x00000002U
#define AES_ISET_ECBSTART_S                                                  1U
#define AES_ISET_ECBSTART_SET                                       0x00000002U
#define AES_ISET_ECBSTART_NO_EFFECT                                 0x00000000U

// Field:     [0] ECBDONE
//
// ECB Done Interrupt
// ENUMs:
// SET
// NO_EFFECT
#define AES_ISET_ECBDONE                                            0x00000001U
#define AES_ISET_ECBDONE_M                                          0x00000001U
#define AES_ISET_ECBDONE_S                                                   0U
#define AES_ISET_ECBDONE_SET                                        0x00000001U
#define AES_ISET_ECBDONE_NO_EFFECT                                  0x00000000U

//*****************************************************************************
//
// Register: AES_O_ICLR
//
//*****************************************************************************
// Field:     [3] CHBDONE
//
// DMA Channel B Done Interrupt
// ENUMs:
// CLR
// NO_EFFECT
#define AES_ICLR_CHBDONE                                            0x00000008U
#define AES_ICLR_CHBDONE_M                                          0x00000008U
#define AES_ICLR_CHBDONE_S                                                   3U
#define AES_ICLR_CHBDONE_CLR                                        0x00000008U
#define AES_ICLR_CHBDONE_NO_EFFECT                                  0x00000000U

// Field:     [2] CHADONE
//
// DMA Channel A Done Interrupt
// ENUMs:
// CLR
// NO_EFFECT
#define AES_ICLR_CHADONE                                            0x00000004U
#define AES_ICLR_CHADONE_M                                          0x00000004U
#define AES_ICLR_CHADONE_S                                                   2U
#define AES_ICLR_CHADONE_CLR                                        0x00000004U
#define AES_ICLR_CHADONE_NO_EFFECT                                  0x00000000U

// Field:     [1] ECBSTART
//
// ECB Start Interrupt
// ENUMs:
// CLR
// NO_EFFECT
#define AES_ICLR_ECBSTART                                           0x00000002U
#define AES_ICLR_ECBSTART_M                                         0x00000002U
#define AES_ICLR_ECBSTART_S                                                  1U
#define AES_ICLR_ECBSTART_CLR                                       0x00000002U
#define AES_ICLR_ECBSTART_NO_EFFECT                                 0x00000000U

// Field:     [0] ECBDONE
//
// ECB Done Interrupt
// ENUMs:
// CLR
// NO_EFFECT
#define AES_ICLR_ECBDONE                                            0x00000001U
#define AES_ICLR_ECBDONE_M                                          0x00000001U
#define AES_ICLR_ECBDONE_S                                                   0U
#define AES_ICLR_ECBDONE_CLR                                        0x00000001U
#define AES_ICLR_ECBDONE_NO_EFFECT                                  0x00000000U

//*****************************************************************************
//
// Register: AES_O_IMSET
//
//*****************************************************************************
// Field:     [3] CHBDONE
//
// DMA Channel B Done Interrupt
// ENUMs:
// SET
// NO_EFFECT
#define AES_IMSET_CHBDONE                                           0x00000008U
#define AES_IMSET_CHBDONE_M                                         0x00000008U
#define AES_IMSET_CHBDONE_S                                                  3U
#define AES_IMSET_CHBDONE_SET                                       0x00000008U
#define AES_IMSET_CHBDONE_NO_EFFECT                                 0x00000000U

// Field:     [2] CHADONE
//
// DMA Channel A Done Interrupt
// ENUMs:
// SET
// NO_EFFECT
#define AES_IMSET_CHADONE                                           0x00000004U
#define AES_IMSET_CHADONE_M                                         0x00000004U
#define AES_IMSET_CHADONE_S                                                  2U
#define AES_IMSET_CHADONE_SET                                       0x00000004U
#define AES_IMSET_CHADONE_NO_EFFECT                                 0x00000000U

// Field:     [1] ECBSTART
//
// ECB Start Interrupt
// ENUMs:
// SET
// NO_EFFECT
#define AES_IMSET_ECBSTART                                          0x00000002U
#define AES_IMSET_ECBSTART_M                                        0x00000002U
#define AES_IMSET_ECBSTART_S                                                 1U
#define AES_IMSET_ECBSTART_SET                                      0x00000002U
#define AES_IMSET_ECBSTART_NO_EFFECT                                0x00000000U

// Field:     [0] ECBDONE
//
// ECB Done Interrupt
// ENUMs:
// SET
// NO_EFFECT
#define AES_IMSET_ECBDONE                                           0x00000001U
#define AES_IMSET_ECBDONE_M                                         0x00000001U
#define AES_IMSET_ECBDONE_S                                                  0U
#define AES_IMSET_ECBDONE_SET                                       0x00000001U
#define AES_IMSET_ECBDONE_NO_EFFECT                                 0x00000000U

//*****************************************************************************
//
// Register: AES_O_IMCLR
//
//*****************************************************************************
// Field:     [3] CHBDONE
//
// DMA Channel B Done Interrupt
// ENUMs:
// CLR
// NO_EFFECT
#define AES_IMCLR_CHBDONE                                           0x00000008U
#define AES_IMCLR_CHBDONE_M                                         0x00000008U
#define AES_IMCLR_CHBDONE_S                                                  3U
#define AES_IMCLR_CHBDONE_CLR                                       0x00000008U
#define AES_IMCLR_CHBDONE_NO_EFFECT                                 0x00000000U

// Field:     [2] CHADONE
//
// DMA Channel A Done Interrupt
// ENUMs:
// CLR
// NO_EFFECT
#define AES_IMCLR_CHADONE                                           0x00000004U
#define AES_IMCLR_CHADONE_M                                         0x00000004U
#define AES_IMCLR_CHADONE_S                                                  2U
#define AES_IMCLR_CHADONE_CLR                                       0x00000004U
#define AES_IMCLR_CHADONE_NO_EFFECT                                 0x00000000U

// Field:     [1] ECBSTART
//
// ECB Start Interrupt
// ENUMs:
// CLR
// NO_EFFECT
#define AES_IMCLR_ECBSTART                                          0x00000002U
#define AES_IMCLR_ECBSTART_M                                        0x00000002U
#define AES_IMCLR_ECBSTART_S                                                 1U
#define AES_IMCLR_ECBSTART_CLR                                      0x00000002U
#define AES_IMCLR_ECBSTART_NO_EFFECT                                0x00000000U

// Field:     [0] ECBDONE
//
// ECB Done Interrupt
// ENUMs:
// CLR
// NO_EFFECT
#define AES_IMCLR_ECBDONE                                           0x00000001U
#define AES_IMCLR_ECBDONE_M                                         0x00000001U
#define AES_IMCLR_ECBDONE_S                                                  0U
#define AES_IMCLR_ECBDONE_CLR                                       0x00000001U
#define AES_IMCLR_ECBDONE_NO_EFFECT                                 0x00000000U


#endif // __AES__
