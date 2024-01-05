/******************************************************************************
*  Filename:       hw_i2s_h
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

#ifndef __HW_I2S_H__
#define __HW_I2S_H__

//*****************************************************************************
//
// This section defines the register offsets of
// I2S component
//
//*****************************************************************************
// WCLK Source Selection
#define I2S_O_AIFWCLKSRC                                            0x00000000U

// DMA Buffer Size Configuration
#define I2S_O_AIFDMACFG                                             0x00000004U

// Pin Direction
#define I2S_O_AIFDIRCFG                                             0x00000008U

// Serial Interface Format Configuration
#define I2S_O_AIFFMTCFG                                             0x0000000CU

// Word Selection Bit Mask for Pin 0
#define I2S_O_AIFWMASK0                                             0x00000010U

// Word Selection Bit Mask for Pin 1
#define I2S_O_AIFWMASK1                                             0x00000014U

// DMA Input Buffer Next Pointer
#define I2S_O_AIFINPTRNEXT                                          0x00000020U

// DMA Input Buffer Current Pointer
#define I2S_O_AIFINPTR                                              0x00000024U

// DMA Output Buffer Next Pointer
#define I2S_O_AIFOUTPTRNEXT                                         0x00000028U

// DMA Output Buffer Current Pointer
#define I2S_O_AIFOUTPTR                                             0x0000002CU

// Samplestamp Generator Control Register
#define I2S_O_STMPCTL                                               0x00000034U

// Captured XOSC Counter Value, Capture Channel 0
#define I2S_O_STMPXCNTCAPT0                                         0x00000038U

// XOSC Period Value
#define I2S_O_STMPXPER                                              0x0000003CU

// Captured WCLK Counter Value, Capture Channel 0
#define I2S_O_STMPWCNTCAPT0                                         0x00000040U

// WCLK Counter Period Value
#define I2S_O_STMPWPER                                              0x00000044U

// WCLK Counter Trigger Value for Input Pins
#define I2S_O_STMPINTRIG                                            0x00000048U

// WCLK Counter Trigger Value for Output Pins
#define I2S_O_STMPOUTTRIG                                           0x0000004CU

// WCLK Counter Set Operation
#define I2S_O_STMPWSET                                              0x00000050U

// WCLK Counter Add Operation
#define I2S_O_STMPWADD                                              0x00000054U

// XOSC Minimum Period Value
#define I2S_O_STMPXPERMIN                                           0x00000058U

// Current Value of WCNT
#define I2S_O_STMPWCNT                                              0x0000005CU

// Current Value of XCNT
#define I2S_O_STMPXCNT                                              0x00000060U

// Interrupt Mask Register
#define I2S_O_IRQMASK                                               0x00000070U

// Raw Interrupt Status Register
#define I2S_O_IRQFLAGS                                              0x00000074U

// Interrupt Set Register
#define I2S_O_IRQSET                                                0x00000078U

// Interrupt Clear Register
#define I2S_O_IRQCLR                                                0x0000007CU

// MCLK Division Ratio
#define I2S_O_AIFMCLKDIV                                            0x00000080U

// BCLK Division Ratio
#define I2S_O_AIFBCLKDIV                                            0x00000084U

// WCLK Division Ratio
#define I2S_O_AIFWCLKDIV                                            0x00000088U

// Internal Audio Clock Control
#define I2S_O_AIFCLKCTL                                             0x0000008CU

// Digital test bus control register. This register can be used to bring out IP
// internal signals to the pads for observation. 16 signals can be observed per
// select value.
#define I2S_O_DTB                                                   0x00000090U

//*****************************************************************************
//
// Register: I2S_O_AIFWCLKSRC
//
//*****************************************************************************
// Field:     [2] WCLK_INV
//
// Inverts WCLK source (pad or internal) when set.
//
// 0: Not inverted
// 1: Inverted
#define I2S_AIFWCLKSRC_WCLK_INV                                     0x00000004U
#define I2S_AIFWCLKSRC_WCLK_INV_M                                   0x00000004U
#define I2S_AIFWCLKSRC_WCLK_INV_S                                            2U

// Field:   [1:0] WCLK_BCLK_SRC
//
// Selects WCLK/BCLK source for AIF.
// ENUMs:
// RESERVED                 Not supported. Will give same WCLK as 'NONE'
//                          ('00')
// INT                      Internal WCLK / BCLK generator, from internal
//                          clock generator module
// EXT                      External WCLK / BCLK generator, from pad
// NONE                     None ('0')
#define I2S_AIFWCLKSRC_WCLK_BCLK_SRC_W                                       2U
#define I2S_AIFWCLKSRC_WCLK_BCLK_SRC_M                              0x00000003U
#define I2S_AIFWCLKSRC_WCLK_BCLK_SRC_S                                       0U
#define I2S_AIFWCLKSRC_WCLK_BCLK_SRC_RESERVED                       0x00000003U
#define I2S_AIFWCLKSRC_WCLK_BCLK_SRC_INT                            0x00000002U
#define I2S_AIFWCLKSRC_WCLK_BCLK_SRC_EXT                            0x00000001U
#define I2S_AIFWCLKSRC_WCLK_BCLK_SRC_NONE                           0x00000000U

//*****************************************************************************
//
// Register: I2S_O_AIFDMACFG
//
//*****************************************************************************
// Field:   [7:0] END_FRAME_IDX
//
// Defines the length of the DMA buffer. Writing a non-zero value to this
// register field enables and initializes AIF. Note that before doing so, all
// other configuration must have been done, and AIFINPTRNEXT/AIFOUTPTRNEXT must
// have been loaded.
#define I2S_AIFDMACFG_END_FRAME_IDX_W                                        8U
#define I2S_AIFDMACFG_END_FRAME_IDX_M                               0x000000FFU
#define I2S_AIFDMACFG_END_FRAME_IDX_S                                        0U

//*****************************************************************************
//
// Register: I2S_O_AIFDIRCFG
//
//*****************************************************************************
// Field:   [5:4] AD1
//
// Configures the AD1 audio data pin usage:
//
// 0x3: Reserved
// ENUMs:
// OUT                      Output mode
// IN                       Input mode
// DIS                      Not in use (disabled)
#define I2S_AIFDIRCFG_AD1_W                                                  2U
#define I2S_AIFDIRCFG_AD1_M                                         0x00000030U
#define I2S_AIFDIRCFG_AD1_S                                                  4U
#define I2S_AIFDIRCFG_AD1_OUT                                       0x00000020U
#define I2S_AIFDIRCFG_AD1_IN                                        0x00000010U
#define I2S_AIFDIRCFG_AD1_DIS                                       0x00000000U

// Field:   [1:0] AD0
//
// Configures the AD0 audio data pin usage:
//
// 0x3: Reserved
// ENUMs:
// OUT                      Output mode
// IN                       Input mode
// DIS                      Not in use (disabled)
#define I2S_AIFDIRCFG_AD0_W                                                  2U
#define I2S_AIFDIRCFG_AD0_M                                         0x00000003U
#define I2S_AIFDIRCFG_AD0_S                                                  0U
#define I2S_AIFDIRCFG_AD0_OUT                                       0x00000002U
#define I2S_AIFDIRCFG_AD0_IN                                        0x00000001U
#define I2S_AIFDIRCFG_AD0_DIS                                       0x00000000U

//*****************************************************************************
//
// Register: I2S_O_AIFFMTCFG
//
//*****************************************************************************
// Field:  [15:8] DATA_DELAY
//
// The number of BCLK periods between a WCLK edge and MSB of the first word in
// a phase:
//
// 0x00: LJF and DSP format
// 0x01: I2S and DSP format
// 0x02: RJF format
// ...
// 0xFF: RJF format
//
// Note: When 0, MSB of the next word will be output in the idle period between
// LSB of the previous word and the start of the next word. Otherwise logical 0
// will be output until the data delay has expired.
#define I2S_AIFFMTCFG_DATA_DELAY_W                                           8U
#define I2S_AIFFMTCFG_DATA_DELAY_M                                  0x0000FF00U
#define I2S_AIFFMTCFG_DATA_DELAY_S                                           8U

// Field:     [7] MEM_LEN_32
//
// The size of each word stored to or loaded from memory:
// ENUMs:
// _32BIT                   32-bit(one 32-bit access per sample)
// _16BIT                   16-bit (one 16 bit access per sample)
#define I2S_AIFFMTCFG_MEM_LEN_32                                    0x00000080U
#define I2S_AIFFMTCFG_MEM_LEN_32_M                                  0x00000080U
#define I2S_AIFFMTCFG_MEM_LEN_32_S                                           7U
#define I2S_AIFFMTCFG_MEM_LEN_32__32BIT                             0x00000080U
#define I2S_AIFFMTCFG_MEM_LEN_32__16BIT                             0x00000000U

// Field:     [6] SMPL_EDGE
//
// On the serial audio interface, data (and wclk) is sampled and clocked out on
// opposite edges of BCLK.
// ENUMs:
// POS                      Data is sampled on the positive edge and clocked
//                          out on the negative edge.
// NEG                      Data is sampled on the negative edge and clocked
//                          out on the positive edge.
#define I2S_AIFFMTCFG_SMPL_EDGE                                     0x00000040U
#define I2S_AIFFMTCFG_SMPL_EDGE_M                                   0x00000040U
#define I2S_AIFFMTCFG_SMPL_EDGE_S                                            6U
#define I2S_AIFFMTCFG_SMPL_EDGE_POS                                 0x00000040U
#define I2S_AIFFMTCFG_SMPL_EDGE_NEG                                 0x00000000U

// Field:     [5] DUAL_PHASE
//
// Selects dual- or single-phase format.
//
// 0: Single-phase: DSP format
// 1: Dual-phase: I2S, LJF and RJF formats
#define I2S_AIFFMTCFG_DUAL_PHASE                                    0x00000020U
#define I2S_AIFFMTCFG_DUAL_PHASE_M                                  0x00000020U
#define I2S_AIFFMTCFG_DUAL_PHASE_S                                           5U

// Field:   [4:0] WORD_LEN
//
// Number of bits per word (8-24):
// In single-phase format, this is the exact number of bits per word.
// In dual-phase format, this is the maximum number of bits per word.
//
// Values below 8 and above 24 give undefined behavior. Data written to memory
// is always aligned to 16 or 24 bits as defined by MEM_LEN_24. Bit widths that
// differ from this alignment will either be truncated or zero padded.
#define I2S_AIFFMTCFG_WORD_LEN_W                                             5U
#define I2S_AIFFMTCFG_WORD_LEN_M                                    0x0000001FU
#define I2S_AIFFMTCFG_WORD_LEN_S                                             0U

//*****************************************************************************
//
// Register: I2S_O_AIFWMASK0
//
//*****************************************************************************
// Field:   [7:0] MASK
//
// Bit-mask indicating valid channels in a frame on AD0.
//
// In single-phase mode, each bit represents one channel, starting with LSB for
// the first word in the frame. A frame can contain up to 8 channels.  Channels
// that are not included in the mask will not be sampled and stored in memory,
// and clocked out as '0'.
//
// In dual-phase mode, only the two LSBs are considered. For a stereo
// configuration, set both bits. For a mono configuration, set bit 0 only. In
// mono mode, only channel 0 will be sampled and stored to memory, and channel
// 0 will be repeated when clocked out.
//
// In mono mode, only channel 0 will be sampled and stored to memory, and
// channel 0 will be repeated in the second phase when clocked out.
//
// If all bits are zero, no input words will be stored to memory, and the
// output data lines will be constant '0'. This can be utilized when PWM debug
// output is desired without any actively used output pins.
#define I2S_AIFWMASK0_MASK_W                                                 8U
#define I2S_AIFWMASK0_MASK_M                                        0x000000FFU
#define I2S_AIFWMASK0_MASK_S                                                 0U

//*****************************************************************************
//
// Register: I2S_O_AIFWMASK1
//
//*****************************************************************************
// Field:   [7:0] MASK
//
// Bit-mask indicating valid channels in a frame on AD1.
//
// In single-phase mode, each bit represents one channel, starting with LSB for
// the first word in the frame. A frame can contain up to 8 channels.  Channels
// that are not included in the mask will not be sampled and stored in memory,
// and clocked out as '0'.
//
// In dual-phase mode, only the two LSBs are considered. For a stereo
// configuration, set both bits. For a mono configuration, set bit 0 only. In
// mono mode, only channel 0 will be sampled and stored to memory, and channel
// 0 will be repeated when clocked out.
//
// In mono mode, only channel 0 will be sampled and stored to memory, and
// channel 0 will be repeated in the second phase when clocked out.
//
// If all bits are zero, no input words will be stored to memory, and the
// output data lines will be constant '0'. This can be utilized when PWM debug
// output is desired without any actively used output pins.
#define I2S_AIFWMASK1_MASK_W                                                 8U
#define I2S_AIFWMASK1_MASK_M                                        0x000000FFU
#define I2S_AIFWMASK1_MASK_S                                                 0U

//*****************************************************************************
//
// Register: I2S_O_AIFINPTRNEXT
//
//*****************************************************************************
// Field:  [31:0] PTR
//
// Pointer to the first byte in the next DMA input buffer.
//
// The read value equals the last written value until the currently used DMA
// input buffer is completed, and then becomes null when the last written value
// is transferred to the DMA controller to start on the next buffer. This event
// is signalized by IRQFLAGS.AIF_DMA_IN.
//
// At startup, the value must be written once before and once after configuring
// the DMA buffer size in AIFDMACFG.
//
// The next pointer must be written to this register while the DMA function
// uses the previously written pointer. If not written in time,
// IRQFLAGS.PTR_ERR will be raised and all input pins will be disabled.
#define I2S_AIFINPTRNEXT_PTR_W                                              32U
#define I2S_AIFINPTRNEXT_PTR_M                                      0xFFFFFFFFU
#define I2S_AIFINPTRNEXT_PTR_S                                               0U

//*****************************************************************************
//
// Register: I2S_O_AIFINPTR
//
//*****************************************************************************
// Field:  [31:0] PTR
//
// Value of the DMA input buffer pointer currently used by the DMA controller.
// Incremented by 1 (byte) or 2 (word) for each AHB access.
#define I2S_AIFINPTR_PTR_W                                                  32U
#define I2S_AIFINPTR_PTR_M                                          0xFFFFFFFFU
#define I2S_AIFINPTR_PTR_S                                                   0U

//*****************************************************************************
//
// Register: I2S_O_AIFOUTPTRNEXT
//
//*****************************************************************************
// Field:  [31:0] PTR
//
// Pointer to the first byte in the next DMA output buffer.
//
// The read value equals the last written value until the currently used DMA
// output buffer is completed, and then becomes null when the last written
// value is transferred to the DMA controller to start on the next buffer. This
// event is signalized by IRQFLAGS.AIF_DMA_OUT.
//
// At startup, the value must be written once before and once after configuring
// the DMA buffer size in AIFDMACFG. At this time, the first two samples will
// be fetched from memory.
//
// The next pointer must be written to this register while the DMA function
// uses the previously written pointer. If not written in time,
// IRQFLAGS.PTR_ERR will be raised and all output pins will be disabled.
#define I2S_AIFOUTPTRNEXT_PTR_W                                             32U
#define I2S_AIFOUTPTRNEXT_PTR_M                                     0xFFFFFFFFU
#define I2S_AIFOUTPTRNEXT_PTR_S                                              0U

//*****************************************************************************
//
// Register: I2S_O_AIFOUTPTR
//
//*****************************************************************************
// Field:  [31:0] PTR
//
// Value of the DMA output buffer pointer currently used by the DMA controller
// Incremented by 1 (byte) or 2 (word) for each AHB access.
#define I2S_AIFOUTPTR_PTR_W                                                 32U
#define I2S_AIFOUTPTR_PTR_M                                         0xFFFFFFFFU
#define I2S_AIFOUTPTR_PTR_S                                                  0U

//*****************************************************************************
//
// Register: I2S_O_STMPCTL
//
//*****************************************************************************
// Field:     [2] OUT_RDY
//
// Low until the output pins are ready to be started by the samplestamp
// generator. When started (that is STMPOUTTRIG equals the WCLK counter) the
// bit goes back low.
#define I2S_STMPCTL_OUT_RDY                                         0x00000004U
#define I2S_STMPCTL_OUT_RDY_M                                       0x00000004U
#define I2S_STMPCTL_OUT_RDY_S                                                2U

// Field:     [1] IN_RDY
//
// Low until the input pins are ready to be started by the samplestamp
// generator. When started (that is STMPINTRIG equals the WCLK counter) the bit
// goes back low.
#define I2S_STMPCTL_IN_RDY                                          0x00000002U
#define I2S_STMPCTL_IN_RDY_M                                        0x00000002U
#define I2S_STMPCTL_IN_RDY_S                                                 1U

// Field:     [0] STMP_EN
//
// Enables the samplestamp generator. The samplestamp generator must only be
// enabled after it has been properly configured.
// When cleared, all samplestamp generator counters and capture values are
// cleared.
#define I2S_STMPCTL_STMP_EN                                         0x00000001U
#define I2S_STMPCTL_STMP_EN_M                                       0x00000001U
#define I2S_STMPCTL_STMP_EN_S                                                0U

//*****************************************************************************
//
// Register: I2S_O_STMPXCNTCAPT0
//
//*****************************************************************************
// Field:  [15:0] CAPT_VALUE
//
// The value of the samplestamp XOSC counter (STMPXCNT.CURR_VALUE) last time an
// event was pulsed (event source selected in [EVENT.I2SSTMPSEL0.EV] for
// channel 0). This number corresponds to the number of 24 MHz clock cycles
// since the last positive edge of the selected WCLK.
// The value is cleared when STMPCTL.STMP_EN = 0.
// Note: Due to buffering and synchronization, WCLK is delayed by a small
// number of BCLK periods and clk periods.
// Note: When calculating the fractional part of the sample stamp, STMPXPER may
// be less than this bit field.
#define I2S_STMPXCNTCAPT0_CAPT_VALUE_W                                      16U
#define I2S_STMPXCNTCAPT0_CAPT_VALUE_M                              0x0000FFFFU
#define I2S_STMPXCNTCAPT0_CAPT_VALUE_S                                       0U

//*****************************************************************************
//
// Register: I2S_O_STMPXPER
//
//*****************************************************************************
// Field:  [15:0] VALUE
//
// The number of 24 MHz clock cycles in the previous WCLK period (that is -
// the next value of the XOSC counter at the positive WCLK edge, had it not
// been reset to 0).
// The value is cleared when STMPCTL.STMP_EN = 0.
#define I2S_STMPXPER_VALUE_W                                                16U
#define I2S_STMPXPER_VALUE_M                                        0x0000FFFFU
#define I2S_STMPXPER_VALUE_S                                                 0U

//*****************************************************************************
//
// Register: I2S_O_STMPWCNTCAPT0
//
//*****************************************************************************
// Field:  [15:0] CAPT_VALUE
//
// The value of the samplestamp WCLK counter (STMPWCNT.CURR_VALUE) last time an
// event was pulsed (event source selected in EVENT:I2SSTMPSEL0.EV for channel
// 0). This number corresponds to the number of positive WCLK edges since the
// samplestamp generator was enabled (not taking modification through
// STMPWADD/STMPWSET into account).
// The value is cleared when STMPCTL.STMP_EN = 0.
#define I2S_STMPWCNTCAPT0_CAPT_VALUE_W                                      16U
#define I2S_STMPWCNTCAPT0_CAPT_VALUE_M                              0x0000FFFFU
#define I2S_STMPWCNTCAPT0_CAPT_VALUE_S                                       0U

//*****************************************************************************
//
// Register: I2S_O_STMPWPER
//
//*****************************************************************************
// Field:  [15:0] VALUE
//
// Used to define when STMPWCNT is to be reset so number of WCLK edges are
// found for the size of the sample buffer. This is thus a modulo value for the
// WCLK counter. This number must correspond to the size of the sample buffer
// used by the system (that is the index of the last sample plus 1).
#define I2S_STMPWPER_VALUE_W                                                16U
#define I2S_STMPWPER_VALUE_M                                        0x0000FFFFU
#define I2S_STMPWPER_VALUE_S                                                 0U

//*****************************************************************************
//
// Register: I2S_O_STMPINTRIG
//
//*****************************************************************************
// Field:  [15:0] IN_START_WCNT
//
// Compare value used to start the incoming audio streams.
// This bit field shall equal the WCLK counter value during the WCLK period in
// which the first input word(s) are sampled and stored to memory (that is the
// sample at the start of the very first DMA input buffer).
//
// The value of this register takes effect when the following conditions are
// met:
// - One or more pins are configured as inputs in AIFDIRCFG.
// - AIFDMACFG has been configured for the correct buffer size, and at least 32
// BCLK cycle ticks have happened.
//
// Note: To avoid false triggers, this bit field should be set higher than
// STMPWPER.VALUE.
#define I2S_STMPINTRIG_IN_START_WCNT_W                                      16U
#define I2S_STMPINTRIG_IN_START_WCNT_M                              0x0000FFFFU
#define I2S_STMPINTRIG_IN_START_WCNT_S                                       0U

//*****************************************************************************
//
// Register: I2S_O_STMPOUTTRIG
//
//*****************************************************************************
// Field:  [15:0] OUT_START_WCNT
//
// Compare value used to start the outgoing audio streams.
//
// This bit field must equal the WCLK counter value during the WCLK period in
// which the first output word(s) read from memory are clocked out (that is the
// sample at the start of the very first DMA output buffer).
//
// The value of this register takes effect when the following conditions are
// met:
// - One or more pins are configured as outputs in AIFDIRCFG.
// - AIFDMACFG has been configured for the correct buffer size, and 32 BCLK
// cycle ticks have happened.
// - 2 samples have been preloaded from memory (examine the AIFOUTPTR register
// if necessary).
// Note: The memory read access is only performed when required, that is
// channels 0/1 must be selected in AIFWMASK0/AIFWMASK1.
//
// Note: To avoid false triggers, this bit field should be set higher than
// STMPWPER.VALUE.
#define I2S_STMPOUTTRIG_OUT_START_WCNT_W                                    16U
#define I2S_STMPOUTTRIG_OUT_START_WCNT_M                            0x0000FFFFU
#define I2S_STMPOUTTRIG_OUT_START_WCNT_S                                     0U

//*****************************************************************************
//
// Register: I2S_O_STMPWSET
//
//*****************************************************************************
// Field:  [15:0] VALUE
//
// WCLK counter modification: Sets the running WCLK counter equal to the
// written value.
#define I2S_STMPWSET_VALUE_W                                                16U
#define I2S_STMPWSET_VALUE_M                                        0x0000FFFFU
#define I2S_STMPWSET_VALUE_S                                                 0U

//*****************************************************************************
//
// Register: I2S_O_STMPWADD
//
//*****************************************************************************
// Field:  [15:0] VALUE_INC
//
// WCLK counter modification: Adds the written value to the running WCLK
// counter. If a positive edge of WCLK occurs at the same time as the
// operation, this will be taken into account.
// To add a negative value, write "STMPWPER.VALUE - value".
//
#define I2S_STMPWADD_VALUE_INC_W                                            16U
#define I2S_STMPWADD_VALUE_INC_M                                    0x0000FFFFU
#define I2S_STMPWADD_VALUE_INC_S                                             0U

//*****************************************************************************
//
// Register: I2S_O_STMPXPERMIN
//
//*****************************************************************************
// Field:  [15:0] VALUE
//
// Each time STMPXPER is updated, the value is also loaded into this register,
// provided that the value is smaller than the current value in this register.
// When written, the register is reset to 0xFFFF (65535), regardless of the
// value written.
// The minimum value can be used to detect extra WCLK pulses (this registers
// value will be significantly smaller than STMPXPER.VALUE).
#define I2S_STMPXPERMIN_VALUE_W                                             16U
#define I2S_STMPXPERMIN_VALUE_M                                     0x0000FFFFU
#define I2S_STMPXPERMIN_VALUE_S                                              0U

//*****************************************************************************
//
// Register: I2S_O_STMPWCNT
//
//*****************************************************************************
// Field:  [15:0] CURR_VALUE
//
// Current value of the WCLK counter
#define I2S_STMPWCNT_CURR_VALUE_W                                           16U
#define I2S_STMPWCNT_CURR_VALUE_M                                   0x0000FFFFU
#define I2S_STMPWCNT_CURR_VALUE_S                                            0U

//*****************************************************************************
//
// Register: I2S_O_STMPXCNT
//
//*****************************************************************************
// Field:  [15:0] CURR_VALUE
//
// Current value of the XOSC counter, latched when reading STMPWCNT.
#define I2S_STMPXCNT_CURR_VALUE_W                                           16U
#define I2S_STMPXCNT_CURR_VALUE_M                                   0x0000FFFFU
#define I2S_STMPXCNT_CURR_VALUE_S                                            0U

//*****************************************************************************
//
// Register: I2S_O_IRQMASK
//
//*****************************************************************************
// Field:     [6] XCNT_CAPTURE
//
// IRQFLAGS.XCNT_CAPTURE interrupt mask
//
// 0: Disable
// 1: Enable
#define I2S_IRQMASK_XCNT_CAPTURE                                    0x00000040U
#define I2S_IRQMASK_XCNT_CAPTURE_M                                  0x00000040U
#define I2S_IRQMASK_XCNT_CAPTURE_S                                           6U

// Field:     [5] AIF_DMA_IN
//
// IRQFLAGS.AIF_DMA_IN interrupt mask
//
// 0: Disable
// 1: Enable
#define I2S_IRQMASK_AIF_DMA_IN                                      0x00000020U
#define I2S_IRQMASK_AIF_DMA_IN_M                                    0x00000020U
#define I2S_IRQMASK_AIF_DMA_IN_S                                             5U

// Field:     [4] AIF_DMA_OUT
//
// IRQFLAGS.AIF_DMA_OUT interrupt mask
//
// 0: Disable
// 1: Enable
#define I2S_IRQMASK_AIF_DMA_OUT                                     0x00000010U
#define I2S_IRQMASK_AIF_DMA_OUT_M                                   0x00000010U
#define I2S_IRQMASK_AIF_DMA_OUT_S                                            4U

// Field:     [3] WCLK_TIMEOUT
//
// IRQFLAGS.WCLK_TIMEOUT interrupt mask
//
// 0: Disable
// 1: Enable
#define I2S_IRQMASK_WCLK_TIMEOUT                                    0x00000008U
#define I2S_IRQMASK_WCLK_TIMEOUT_M                                  0x00000008U
#define I2S_IRQMASK_WCLK_TIMEOUT_S                                           3U

// Field:     [2] BUS_ERR
//
// IRQFLAGS.BUS_ERR interrupt mask
//
// 0: Disable
// 1: Enable
#define I2S_IRQMASK_BUS_ERR                                         0x00000004U
#define I2S_IRQMASK_BUS_ERR_M                                       0x00000004U
#define I2S_IRQMASK_BUS_ERR_S                                                2U

// Field:     [1] WCLK_ERR
//
// IRQFLAGS.WCLK_ERR interrupt mask
//
// 0: Disable
// 1: Enable
#define I2S_IRQMASK_WCLK_ERR                                        0x00000002U
#define I2S_IRQMASK_WCLK_ERR_M                                      0x00000002U
#define I2S_IRQMASK_WCLK_ERR_S                                               1U

// Field:     [0] PTR_ERR
//
// IRQFLAGS.PTR_ERR interrupt mask.
//
// 0: Disable
// 1: Enable
#define I2S_IRQMASK_PTR_ERR                                         0x00000001U
#define I2S_IRQMASK_PTR_ERR_M                                       0x00000001U
#define I2S_IRQMASK_PTR_ERR_S                                                0U

//*****************************************************************************
//
// Register: I2S_O_IRQFLAGS
//
//*****************************************************************************
// Field:     [6] XCNT_CAPTURE
//
// Will be set when MCUCLK counter is captured based upon events.
//                   Can only be cleared by software.
#define I2S_IRQFLAGS_XCNT_CAPTURE                                   0x00000040U
#define I2S_IRQFLAGS_XCNT_CAPTURE_M                                 0x00000040U
#define I2S_IRQFLAGS_XCNT_CAPTURE_S                                          6U

// Field:     [5] AIF_DMA_IN
//
// Set when condition for this bit field event occurs (auto cleared when input
// pointer is updated - AIFINPTRNEXT), see description of AIFINPTRNEXT register
// for details.
#define I2S_IRQFLAGS_AIF_DMA_IN                                     0x00000020U
#define I2S_IRQFLAGS_AIF_DMA_IN_M                                   0x00000020U
#define I2S_IRQFLAGS_AIF_DMA_IN_S                                            5U

// Field:     [4] AIF_DMA_OUT
//
// Set when condition for this bit field event occurs (auto cleared when output
// pointer is updated - AIFOUTPTRNEXT), see description of AIFOUTPTRNEXT
// register for details
#define I2S_IRQFLAGS_AIF_DMA_OUT                                    0x00000010U
#define I2S_IRQFLAGS_AIF_DMA_OUT_M                                  0x00000010U
#define I2S_IRQFLAGS_AIF_DMA_OUT_S                                           4U

// Field:     [3] WCLK_TIMEOUT
//
// Set when the sample stamp generator does not detect a positive WCLK edge for
//  65535 clk periods. This signalizes that the internal or external BCLK and
// WCLK generator source has been disabled.
//
// The bit is sticky and may only be cleared by software (by writing '1' to
// IRQCLR.WCLK_TIMEOUT).
#define I2S_IRQFLAGS_WCLK_TIMEOUT                                   0x00000008U
#define I2S_IRQFLAGS_WCLK_TIMEOUT_M                                 0x00000008U
#define I2S_IRQFLAGS_WCLK_TIMEOUT_S                                          3U

// Field:     [2] BUS_ERR
//
// Set when a DMA operation is not completed in time (that is audio output
// buffer underflow, or audio input buffer overflow).
// This error requires a complete restart since word synchronization has been
// lost. The bit is sticky and may only be cleared by software (by writing '1'
// to IRQCLR.BUS_ERR).
//
// Note that DMA initiated transactions to illegal addresses will not trigger
// an interrupt. The response to such transactions is undefined.
#define I2S_IRQFLAGS_BUS_ERR                                        0x00000004U
#define I2S_IRQFLAGS_BUS_ERR_M                                      0x00000004U
#define I2S_IRQFLAGS_BUS_ERR_S                                               2U

// Field:     [1] WCLK_ERR
//
// Set when:
// - An unexpected WCLK edge occurs during the data delay period of a phase.
// Note unexpected WCLK edges during the word and idle periods of the phase are
// not detected.
// -  In dual-phase mode, when two WCLK edges are less than 4 BCLK cycles
// apart.
// -  In single-phase mode, when a WCLK pulse occurs before the last channel.
// This error requires a complete restart since word synchronization has been
// lost. The bit is sticky and may only be cleared by software (by writing '1'
// to IRQCLR.WCLK_ERR).
#define I2S_IRQFLAGS_WCLK_ERR                                       0x00000002U
#define I2S_IRQFLAGS_WCLK_ERR_M                                     0x00000002U
#define I2S_IRQFLAGS_WCLK_ERR_S                                              1U

// Field:     [0] PTR_ERR
//
// Set when AIFINPTRNEXT or AIFOUTPTRNEXT has not been loaded with the next
// block address in time.
// This error requires a complete restart since word synchronization has been
// lost. The bit is sticky and may only be cleared by software (by writing '1'
// to IRQCLR.PTR_ERR).
#define I2S_IRQFLAGS_PTR_ERR                                        0x00000001U
#define I2S_IRQFLAGS_PTR_ERR_M                                      0x00000001U
#define I2S_IRQFLAGS_PTR_ERR_S                                               0U

//*****************************************************************************
//
// Register: I2S_O_IRQSET
//
//*****************************************************************************
// Field:     [6] XCNT_CAPTURE
//
// 1: Sets the interrupt of IRQFLAGS.XCNT_CAPTURE (unless a auto clear criteria
// was given at the same time, in which the set will be ignored)
#define I2S_IRQSET_XCNT_CAPTURE                                     0x00000040U
#define I2S_IRQSET_XCNT_CAPTURE_M                                   0x00000040U
#define I2S_IRQSET_XCNT_CAPTURE_S                                            6U

// Field:     [5] AIF_DMA_IN
//
// 1: Sets the interrupt of IRQFLAGS.AIF_DMA_IN (unless a auto clear criteria
// was given at the same time, in which the set will be ignored)
#define I2S_IRQSET_AIF_DMA_IN                                       0x00000020U
#define I2S_IRQSET_AIF_DMA_IN_M                                     0x00000020U
#define I2S_IRQSET_AIF_DMA_IN_S                                              5U

// Field:     [4] AIF_DMA_OUT
//
// 1: Sets the interrupt of IRQFLAGS.AIF_DMA_OUT (unless a auto clear criteria
// was given at the same time, in which the set will be ignored)
#define I2S_IRQSET_AIF_DMA_OUT                                      0x00000010U
#define I2S_IRQSET_AIF_DMA_OUT_M                                    0x00000010U
#define I2S_IRQSET_AIF_DMA_OUT_S                                             4U

// Field:     [3] WCLK_TIMEOUT
//
// 1: Sets the interrupt of IRQFLAGS.WCLK_TIMEOUT
#define I2S_IRQSET_WCLK_TIMEOUT                                     0x00000008U
#define I2S_IRQSET_WCLK_TIMEOUT_M                                   0x00000008U
#define I2S_IRQSET_WCLK_TIMEOUT_S                                            3U

// Field:     [2] BUS_ERR
//
// 1: Sets the interrupt of IRQFLAGS.BUS_ERR
#define I2S_IRQSET_BUS_ERR                                          0x00000004U
#define I2S_IRQSET_BUS_ERR_M                                        0x00000004U
#define I2S_IRQSET_BUS_ERR_S                                                 2U

// Field:     [1] WCLK_ERR
//
// 1: Sets the interrupt of IRQFLAGS.WCLK_ERR
#define I2S_IRQSET_WCLK_ERR                                         0x00000002U
#define I2S_IRQSET_WCLK_ERR_M                                       0x00000002U
#define I2S_IRQSET_WCLK_ERR_S                                                1U

// Field:     [0] PTR_ERR
//
// 1: Sets the interrupt of IRQFLAGS.PTR_ERR
#define I2S_IRQSET_PTR_ERR                                          0x00000001U
#define I2S_IRQSET_PTR_ERR_M                                        0x00000001U
#define I2S_IRQSET_PTR_ERR_S                                                 0U

//*****************************************************************************
//
// Register: I2S_O_IRQCLR
//
//*****************************************************************************
// Field:     [6] XCNT_CAPTURE
//
// 1: Clears the interrupt of IRQFLAGS.XCNT_CAPTURE (unless a set criteria was
// given at the same time in which the clear will be ignored)
#define I2S_IRQCLR_XCNT_CAPTURE                                     0x00000040U
#define I2S_IRQCLR_XCNT_CAPTURE_M                                   0x00000040U
#define I2S_IRQCLR_XCNT_CAPTURE_S                                            6U

// Field:     [5] AIF_DMA_IN
//
// 1: Clears the interrupt of IRQFLAGS.AIF_DMA_IN (unless a set criteria was
// given at the same time in which the clear will be ignored)
#define I2S_IRQCLR_AIF_DMA_IN                                       0x00000020U
#define I2S_IRQCLR_AIF_DMA_IN_M                                     0x00000020U
#define I2S_IRQCLR_AIF_DMA_IN_S                                              5U

// Field:     [4] AIF_DMA_OUT
//
// 1: Clears the interrupt of IRQFLAGS.AIF_DMA_OUT (unless a set criteria was
// given at the same time in which the clear will be ignored)
#define I2S_IRQCLR_AIF_DMA_OUT                                      0x00000010U
#define I2S_IRQCLR_AIF_DMA_OUT_M                                    0x00000010U
#define I2S_IRQCLR_AIF_DMA_OUT_S                                             4U

// Field:     [3] WCLK_TIMEOUT
//
// 1: Clears the interrupt of IRQFLAGS.WCLK_TIMEOUT (unless a set criteria was
// given at the same time in which the clear will be ignored)
#define I2S_IRQCLR_WCLK_TIMEOUT                                     0x00000008U
#define I2S_IRQCLR_WCLK_TIMEOUT_M                                   0x00000008U
#define I2S_IRQCLR_WCLK_TIMEOUT_S                                            3U

// Field:     [2] BUS_ERR
//
// 1: Clears the interrupt of IRQFLAGS.BUS_ERR (unless a set criteria was given
// at the same time in which the clear will be ignored)
#define I2S_IRQCLR_BUS_ERR                                          0x00000004U
#define I2S_IRQCLR_BUS_ERR_M                                        0x00000004U
#define I2S_IRQCLR_BUS_ERR_S                                                 2U

// Field:     [1] WCLK_ERR
//
// 1: Clears the interrupt of IRQFLAGS.WCLK_ERR (unless a set criteria was
// given at the same time in which the clear will be ignored)
#define I2S_IRQCLR_WCLK_ERR                                         0x00000002U
#define I2S_IRQCLR_WCLK_ERR_M                                       0x00000002U
#define I2S_IRQCLR_WCLK_ERR_S                                                1U

// Field:     [0] PTR_ERR
//
// 1: Clears the interrupt of IRQFLAGS.PTR_ERR (unless a set criteria was given
// at the same time in which the clear will be ignored)
#define I2S_IRQCLR_PTR_ERR                                          0x00000001U
#define I2S_IRQCLR_PTR_ERR_M                                        0x00000001U
#define I2S_IRQCLR_PTR_ERR_S                                                 0U

//*****************************************************************************
//
// Register: I2S_O_AIFMCLKDIV
//
//*****************************************************************************
// Field:   [9:0] MDIV
//
//     An unsigned factor of the division ratio used to generate MCLK [2-1024]:
//                                             MCLK = MCUCLK/MDIV[Hz] MCUCLK is
// upto 96MHz.
//                                             A value of 0 is interpreted as
// 1024.
//                                             A value of 1 is invalid.
//                                             If MDIV is odd the low phase of
// the clock is one MCUCLK period longer than the high phase.
#define I2S_AIFMCLKDIV_MDIV_W                                               10U
#define I2S_AIFMCLKDIV_MDIV_M                                       0x000003FFU
#define I2S_AIFMCLKDIV_MDIV_S                                                0U

//*****************************************************************************
//
// Register: I2S_O_AIFBCLKDIV
//
//*****************************************************************************
// Field:   [9:0] BDIV
//
//     An unsigned factor of the division ratio used to generate BCLK [2-1024]:
//                                             BCLK = MCUCLK/BDIV[Hz] MCUCLK
// can be upto 96MHz.
//                                             A value of 0 is interpreted as
// 1024.
//                                             A value of 1 is invalid.
//                                             If BDIV is odd and
// [AIFCLKCTL.SMPL_ON_POSEDGE.*] = 0, the low phase of the clock is one MCUCLK
// period longer than the high phase.
//                                             If BDIV is odd and
// [AIFCLKCTL.SMPL_ON_POSEDGE.*] = 1 , the high phase of the clock is one
// MCUCLK period longer than the low phase.
#define I2S_AIFBCLKDIV_BDIV_W                                               10U
#define I2S_AIFBCLKDIV_BDIV_M                                       0x000003FFU
#define I2S_AIFBCLKDIV_BDIV_S                                                0U

//*****************************************************************************
//
// Register: I2S_O_AIFWCLKDIV
//
//*****************************************************************************
// Field:  [15:0] WDIV
//
//    If [AIFCLKCTL.WCLK_PHASE.*] = 0, Single phase. WCLK is high
// one BCLK period and low WDIV[9:0] (unsigned, [1-1023]) BCLK periods.
//                                            WCLK = MCUCLK / BDIV*(WDIV[9:0] +
// 1) [Hz] MCUCLK upto 96MHz.
//                                            If [AIFCLKCTL.WCLK_PHASE.*] = 1,
// Dual phase. Each phase on WCLK (50% duty cycle) is WDIV[9:0] (unsigned,
// [1-1023]) BCLK periods.
//                                            **WCLK **= **MCUCLK **/
// BDIV*(2*WDIV[9:0]) [Hz]
//                                            If [AIFCLKCTL.WCLK_PHASE.*] = 2,
// User defined. WCLK is high WDIV[7:0] (unsigned, [1-255]) BCLK periods and
// low WDIV[15:8] (unsigned, [1-255]) BCLK periods.
//                                            WCLK = **MCUCLK **/
// (BDIV*(WDIV[7:0] + WDIV[15:8]) [Hz]
#define I2S_AIFWCLKDIV_WDIV_W                                               16U
#define I2S_AIFWCLKDIV_WDIV_M                                       0x0000FFFFU
#define I2S_AIFWCLKDIV_WDIV_S                                                0U

//*****************************************************************************
//
// Register: I2S_O_AIFCLKCTL
//
//*****************************************************************************
// Field:     [3] M_EN
//
// 0: MCLK internal geneartion disabled, 1: MCLK internal generation enabled
#define I2S_AIFCLKCTL_M_EN                                          0x00000008U
#define I2S_AIFCLKCTL_M_EN_M                                        0x00000008U
#define I2S_AIFCLKCTL_M_EN_S                                                 3U

// Field:   [2:1] WCLK_PHASE
//
// Decides how the WCLK division ratio is calculated and used to generate
// different duty cycles (See [AIFWCLKDIV.WDIV.*]).
#define I2S_AIFCLKCTL_WCLK_PHASE_W                                           2U
#define I2S_AIFCLKCTL_WCLK_PHASE_M                                  0x00000006U
#define I2S_AIFCLKCTL_WCLK_PHASE_S                                           1U

// Field:     [0] WB_EN
//
// 0: WCLK/BCLK internal geneartion disabled, 1: WCLK/BCLK internal generation
// enabled
#define I2S_AIFCLKCTL_WB_EN                                         0x00000001U
#define I2S_AIFCLKCTL_WB_EN_M                                       0x00000001U
#define I2S_AIFCLKCTL_WB_EN_S                                                0U

//*****************************************************************************
//
// Register: I2S_O_DTB
//
//*****************************************************************************
// Field:   [1:0] SEL
//
//      0: DTB output will be zero
//                                              1: Word clock edge counter's
// current value
//                                              2: Current value of MCUCLK
// clock counter.
//                                              3: DTB[0] => Input format
// control is enabled
//                                                 DTB[1] => output format
// control is enabled
//                                                 DTB[2] => Word loaded into
// serializer from DMA
//                                                 DTB[3] => Word sampled into
// DMA buffer from deserializer
//                                                 DTB[15:4] => All zero
#define I2S_DTB_SEL_W                                                        2U
#define I2S_DTB_SEL_M                                               0x00000003U
#define I2S_DTB_SEL_S                                                        0U


#endif // __I2S__
