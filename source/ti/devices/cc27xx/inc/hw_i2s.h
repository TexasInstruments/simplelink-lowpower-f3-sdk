/******************************************************************************
*  Filename:       hw_i2s_h
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

#ifndef __HW_I2S_H__
#define __HW_I2S_H__

//*****************************************************************************
//
// This section defines the register offsets of
// I2S component
//
//*****************************************************************************
// This register configures the WCLK Source
#define I2S_O_WCLKSRC                                               0x00000000U

// This register configures DMA buffer size
#define I2S_O_DMACFG                                                0x00000004U

// This register configures the direction of data pins(AD0/AD1)
#define I2S_O_DIRCFG                                                0x00000008U

// This register configures the serial interface format
#define I2S_O_FMTCFG                                                0x0000000CU

// This register configures the word selection dit mask for data pin 0(AD0)
#define I2S_O_WMASK0                                                0x00000010U

// This register configures the word selection dit mask for data pin 1(AD1)
#define I2S_O_WMASK1                                                0x00000014U

// This register configures DMA input buffer next pointer
#define I2S_O_INPTRNXT                                              0x00000020U

// This register configures the DMA input buffer current pointer
#define I2S_O_INPTR                                                 0x00000024U

// This register configures DMA output buffer next pointer
#define I2S_O_OUTPTRNXT                                             0x00000028U

// This register configures DMA output buffer current pointer
#define I2S_O_OUTPTR                                                0x0000002CU

// This register controls the samplestamp generator.
#define I2S_O_STMPCTL                                               0x00000034U

// This register gives the captured XOSC counter value, capture channel 0
#define I2S_O_STMPXCNTCAPT0                                         0x00000038U

// The register gives the XOSC period value
#define I2S_O_STMPXPER                                              0x0000003CU

// This register gives the captured WCLK counter value, capture channel 0
#define I2S_O_STMPWCNTCAPT0                                         0x00000040U

// This register configures WCLK counter period value
#define I2S_O_STMPWPER                                              0x00000044U

// This register configures WCLK counter trigger value for input pins
#define I2S_O_STMPINTRIG                                            0x00000048U

// This register configures WCLK counter trigger value for output pins
#define I2S_O_STMPOUTTRIG                                           0x0000004CU

// This register confiures WCLK counter set operation
#define I2S_O_STMPWSET                                              0x00000050U

// This register configures WCLK counter add operation
#define I2S_O_STMPWADD                                              0x00000054U

// This register configures XOSC minimum period value
#define I2S_O_STMPXPERMIN                                           0x00000058U

// This register gives the current value of WLCK counter
#define I2S_O_STMPWCNT                                              0x0000005CU

// This register gives the current value XOSC counter
#define I2S_O_STMPXCNT                                              0x00000060U

// Interrupt Mask Register
#define I2S_O_IRQMASK                                               0x00000070U

// This registers gives the raw interrupt status
#define I2S_O_IRQFLAGS                                              0x00000074U

// Interrupt Set Register. This register can be used by software for
// diagnostics and safety checking purposes.
#define I2S_O_IRQSET                                                0x00000078U

// Interrupt clear register. This register allows software to clear interrupts.
#define I2S_O_IRQCLR                                                0x0000007CU

// This field configures MCLK division ratio
#define I2S_O_MCLKDIV                                               0x00000080U

// This field configures BCLK division ratio
#define I2S_O_BCLKDIV                                               0x00000084U

// This field configures WCLK division ratio
#define I2S_O_WCLKDIV                                               0x00000088U

// This register controls internal audio clock
#define I2S_O_CLKCTL                                                0x0000008CU

// Digital test bus control
#define I2S_O_DTB                                                   0x00000090U

//*****************************************************************************
//
// Register: I2S_O_WCLKSRC
//
//*****************************************************************************
// Field:     [2] WCLKINV
//
// This field Inverts WCLK source (pad or internal).
// ENUMs:
// INVERTED                 Source is inverted
// NOT_INTVERTED            Source is not intverted
#define I2S_WCLKSRC_WCLKINV                                         0x00000004U
#define I2S_WCLKSRC_WCLKINV_M                                       0x00000004U
#define I2S_WCLKSRC_WCLKINV_S                                                2U
#define I2S_WCLKSRC_WCLKINV_INVERTED                                0x00000004U
#define I2S_WCLKSRC_WCLKINV_NOT_INTVERTED                           0x00000000U

// Field:   [1:0] WBCLKSRC
//
// This field selects WCLK/BCLK source for I2S.
// ENUMs:
// RESERVED                 Not supported. Will give same WCLK as 'NONE'
//                          ('00')
// INT                      Internal WCLK generator, from module PRCM
// EXT                      External WCLK generator, from pad
// NONE                     None ('0')
#define I2S_WCLKSRC_WBCLKSRC_W                                               2U
#define I2S_WCLKSRC_WBCLKSRC_M                                      0x00000003U
#define I2S_WCLKSRC_WBCLKSRC_S                                               0U
#define I2S_WCLKSRC_WBCLKSRC_RESERVED                               0x00000003U
#define I2S_WCLKSRC_WBCLKSRC_INT                                    0x00000002U
#define I2S_WCLKSRC_WBCLKSRC_EXT                                    0x00000001U
#define I2S_WCLKSRC_WBCLKSRC_NONE                                   0x00000000U

//*****************************************************************************
//
// Register: I2S_O_DMACFG
//
//*****************************************************************************
// Field:   [7:0] ENDFRMIDX
//
// This field defines the length of the DMA buffer. Writing a non-zero value to
// this register field enables and initializes I2S.
// Note that before doing so, all other configuration must have been done, and
// INPTRNXT/OUTPTRNXT must have been loaded.
#define I2S_DMACFG_ENDFRMIDX_W                                               8U
#define I2S_DMACFG_ENDFRMIDX_M                                      0x000000FFU
#define I2S_DMACFG_ENDFRMIDX_S                                               0U

//*****************************************************************************
//
// Register: I2S_O_DIRCFG
//
//*****************************************************************************
// Field:   [5:4] AD1
//
// The field configures the AD1 data pin direction
// ENUMs:
// RESERVED                 Reserved
// OUT                      Output
// IN                       Input
// DIS                      Not in use (disabled)
#define I2S_DIRCFG_AD1_W                                                     2U
#define I2S_DIRCFG_AD1_M                                            0x00000030U
#define I2S_DIRCFG_AD1_S                                                     4U
#define I2S_DIRCFG_AD1_RESERVED                                     0x00000030U
#define I2S_DIRCFG_AD1_OUT                                          0x00000020U
#define I2S_DIRCFG_AD1_IN                                           0x00000010U
#define I2S_DIRCFG_AD1_DIS                                          0x00000000U

// Field:   [1:0] AD0
//
// The field configures the AD0 data pin direction
// ENUMs:
// RESERVED                 Reserved
// OUT                      Output
// IN                       Input
// DIS                      Not in use (disabled)
#define I2S_DIRCFG_AD0_W                                                     2U
#define I2S_DIRCFG_AD0_M                                            0x00000003U
#define I2S_DIRCFG_AD0_S                                                     0U
#define I2S_DIRCFG_AD0_RESERVED                                     0x00000003U
#define I2S_DIRCFG_AD0_OUT                                          0x00000002U
#define I2S_DIRCFG_AD0_IN                                           0x00000001U
#define I2S_DIRCFG_AD0_DIS                                          0x00000000U

//*****************************************************************************
//
// Register: I2S_O_FMTCFG
//
//*****************************************************************************
// Field:  [15:8] DATADLY
//
// This field configures the number of BCLK periods between a WCLK edge and MSB
// of the first word in a phase
//
// Note: When 0, MSB of the next word will be output in the idle period between
// LSB of the previous word and the start of the next word. Otherwise logical 0
// will be output until the data delay has expired.
// ENUMs:
// MAX                      Max(255) BCLK periods - RJF format
// TWO                      Two(Min) BCLK periods - RJF format
// ONE                      One BCLK periods - I2S and DSP formats
// ZERO                     Zero BCLK periods - LJF and DSP formats
#define I2S_FMTCFG_DATADLY_W                                                 8U
#define I2S_FMTCFG_DATADLY_M                                        0x0000FF00U
#define I2S_FMTCFG_DATADLY_S                                                 8U
#define I2S_FMTCFG_DATADLY_MAX                                      0x0000FF00U
#define I2S_FMTCFG_DATADLY_TWO                                      0x00000200U
#define I2S_FMTCFG_DATADLY_ONE                                      0x00000100U
#define I2S_FMTCFG_DATADLY_ZERO                                     0x00000000U

// Field:     [7] MEMLEN32
//
// This register configures the size of each word stored to or loaded from
// memory
// ENUMs:
// EN                       32-bit(one 32-bit access per sample)
// DIS                      16-bit (one 16 bit access per sample)
#define I2S_FMTCFG_MEMLEN32                                         0x00000080U
#define I2S_FMTCFG_MEMLEN32_M                                       0x00000080U
#define I2S_FMTCFG_MEMLEN32_S                                                7U
#define I2S_FMTCFG_MEMLEN32_EN                                      0x00000080U
#define I2S_FMTCFG_MEMLEN32_DIS                                     0x00000000U

// Field:     [6] SMPLEDGE
//
// This field configures the sample edge/ transfer edge of data (and WCLK) on
// BCLK
// ENUMs:
// POSEDGE                  Data is sampled on the positive edge and clocked
//                          out on the negative edge.
// NEGEDGE                  Data is sampled on the negative edge and clocked
//                          out on the positive edge.
#define I2S_FMTCFG_SMPLEDGE                                         0x00000040U
#define I2S_FMTCFG_SMPLEDGE_M                                       0x00000040U
#define I2S_FMTCFG_SMPLEDGE_S                                                6U
#define I2S_FMTCFG_SMPLEDGE_POSEDGE                                 0x00000040U
#define I2S_FMTCFG_SMPLEDGE_NEGEDGE                                 0x00000000U

// Field:     [5] DUALPHASE
//
// This field selects between dual-phase or single-phase format
// ENUMs:
// DUALPHASE                Dual-phase: I2S, LJF and RJF formats
// SINGLEPHASE              Single-phase: DSP format
#define I2S_FMTCFG_DUALPHASE                                        0x00000020U
#define I2S_FMTCFG_DUALPHASE_M                                      0x00000020U
#define I2S_FMTCFG_DUALPHASE_S                                               5U
#define I2S_FMTCFG_DUALPHASE_DUALPHASE                              0x00000020U
#define I2S_FMTCFG_DUALPHASE_SINGLEPHASE                            0x00000000U

// Field:   [4:0] WORDLEN
//
// Number of bits per word (8-24):
// In single-phase format, this is the exact number of bits per word.
// In dual-phase format, this is the maximum number of bits per word.
//
// Values below 8 and above 24 give undefined behavior. Data written to memory
// is always aligned to 16 or 24 bits as defined by MEMLEN32. Bit widths that
// differ from this alignment will either be truncated or zero padded.
#define I2S_FMTCFG_WORDLEN_W                                                 5U
#define I2S_FMTCFG_WORDLEN_M                                        0x0000001FU
#define I2S_FMTCFG_WORDLEN_S                                                 0U

//*****************************************************************************
//
// Register: I2S_O_WMASK0
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
#define I2S_WMASK0_MASK_W                                                    8U
#define I2S_WMASK0_MASK_M                                           0x000000FFU
#define I2S_WMASK0_MASK_S                                                    0U

//*****************************************************************************
//
// Register: I2S_O_WMASK1
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
#define I2S_WMASK1_MASK_W                                                    8U
#define I2S_WMASK1_MASK_M                                           0x000000FFU
#define I2S_WMASK1_MASK_S                                                    0U

//*****************************************************************************
//
// Register: I2S_O_INPTRNXT
//
//*****************************************************************************
// Field:  [31:0] PTR
//
// Pointer to the first byte in the next DMA input buffer.
//
// The read value equals the last written value until the currently used DMA
// input buffer is completed, and then becomes null when the last written value
// is transferred to the DMA controller to start on the next buffer. This event
// is signalized by IRQFLAGS.DMAIN.
//
// At startup, the value must be written once before and once after configuring
// the DMA buffer size in DMACFG.
//
// The next pointer must be written to this register while the DMA function
// uses the previously written pointer. If not written in time, IRQFLAGS.PTRERR
// will be raised and all input pins will be disabled.
#define I2S_INPTRNXT_PTR_W                                                  32U
#define I2S_INPTRNXT_PTR_M                                          0xFFFFFFFFU
#define I2S_INPTRNXT_PTR_S                                                   0U

//*****************************************************************************
//
// Register: I2S_O_INPTR
//
//*****************************************************************************
// Field:  [31:0] PTR
//
// Value of the DMA input buffer pointer currently used by the DMA controller.
// Incremented by 1 (byte) or 2 (word) for each AHB access.
#define I2S_INPTR_PTR_W                                                     32U
#define I2S_INPTR_PTR_M                                             0xFFFFFFFFU
#define I2S_INPTR_PTR_S                                                      0U

//*****************************************************************************
//
// Register: I2S_O_OUTPTRNXT
//
//*****************************************************************************
// Field:  [31:0] PTR
//
// Pointer to the first byte in the next DMA output buffer.
//
// The read value equals the last written value until the currently used DMA
// output buffer is completed, and then becomes null when the last written
// value is transferred to the DMA controller to start on the next buffer. This
// event is signalized by IRQFLAGS.DMAOUT.
//
// At startup, the value must be written once before and once after configuring
// the DMA buffer size in DMACFG. At this time, the first two samples will be
// fetched from memory.
//
// The next pointer must be written to this register while the DMA function
// uses the previously written pointer. If not written in time, IRQFLAGS.PTRERR
// will be raised and all output pins will be disabled.
#define I2S_OUTPTRNXT_PTR_W                                                 32U
#define I2S_OUTPTRNXT_PTR_M                                         0xFFFFFFFFU
#define I2S_OUTPTRNXT_PTR_S                                                  0U

//*****************************************************************************
//
// Register: I2S_O_OUTPTR
//
//*****************************************************************************
// Field:  [31:0] PTR
//
// Value of the DMA output buffer pointer currently used by the DMA controller
// Incremented by 1 (byte) or 2 (word) for each AHB access.
#define I2S_OUTPTR_PTR_W                                                    32U
#define I2S_OUTPTR_PTR_M                                            0xFFFFFFFFU
#define I2S_OUTPTR_PTR_S                                                     0U

//*****************************************************************************
//
// Register: I2S_O_STMPCTL
//
//*****************************************************************************
// Field:     [2] OUTRDY
//
// This field is low until the output pins are ready to be started by the
// samplestamp generator. When started (that is STMPOUTTRIG equals the WCLK
// counter) the bit goes back low.
#define I2S_STMPCTL_OUTRDY                                          0x00000004U
#define I2S_STMPCTL_OUTRDY_M                                        0x00000004U
#define I2S_STMPCTL_OUTRDY_S                                                 2U

// Field:     [1] INRDY
//
// This field is low until the input pins are ready to be started by the
// samplestamp generator. When started (that is STMPINTRIG equals the WCLK
// counter) the bit goes back low.
#define I2S_STMPCTL_INRDY                                           0x00000002U
#define I2S_STMPCTL_INRDY_M                                         0x00000002U
#define I2S_STMPCTL_INRDY_S                                                  1U

// Field:     [0] STMPEN
//
// This field configures the samplestamp generator. The samplestamp generator
// must only be enabled after it has been properly configured.
// When cleared, all samplestamp generator counters and capture values are
// cleared.
// ENUMs:
// EN                       Enable the samplestamp generator
// DIS                      Disable the samplestamp generator
#define I2S_STMPCTL_STMPEN                                          0x00000001U
#define I2S_STMPCTL_STMPEN_M                                        0x00000001U
#define I2S_STMPCTL_STMPEN_S                                                 0U
#define I2S_STMPCTL_STMPEN_EN                                       0x00000001U
#define I2S_STMPCTL_STMPEN_DIS                                      0x00000000U

//*****************************************************************************
//
// Register: I2S_O_STMPXCNTCAPT0
//
//*****************************************************************************
// Field:  [15:0] CAPTVALUE
//
// The value of the samplestamp XOSC counter (STMPXCNT.CURRVALUE) last time an
// event was pulsed. This number corresponds to the number of 24 MHz clock
// cycles since the last positive edge of the selected WCLK.
// The value is cleared when STMPCTL.STMPEN = 0.
// Note: Due to buffering and synchronization, WCLK is delayed by a small
// number of BCLK periods and clk periods.
// Note: When calculating the fractional part of the sample stamp, STMPXPER may
// be less than this bit field.
#define I2S_STMPXCNTCAPT0_CAPTVALUE_W                                       16U
#define I2S_STMPXCNTCAPT0_CAPTVALUE_M                               0x0000FFFFU
#define I2S_STMPXCNTCAPT0_CAPTVALUE_S                                        0U

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
// The value is cleared when STMPCTL.STMPEN = 0.
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
// The value of the samplestamp WCLK counter (STMPWCNT.CURRVALUE) last time an
// event was pulsed. This number corresponds to the number of positive WCLK
// edges since the samplestamp generator was enabled (not taking modification
// through STMPWADD/STMPWSET into account).
// The value is cleared when STMPCTL.STMPEN = 0.
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
// This field defines when STMPWCNT is to be reset so number of WCLK edges are
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
// Field:  [15:0] INSTARTWCNT
//
// This field configures the compare value used to start the incoming audio
// streams.
// This bit field shall equal the WCLK counter value during the WCLK period in
// which the first input word(s) are sampled and stored to memory (that is the
// sample at the start of the very first DMA input buffer).
//
// The value of this register takes effect when the following conditions are
// met:
// - One or more pins are configured as inputs in DIRCFG.
// - DMACFG has been configured for the correct buffer size, and at least 32
// BCLK cycle ticks have happened.
//
// Note: To avoid false triggers, this bit field should be set higher than
// STMPWPER.VALUE.
#define I2S_STMPINTRIG_INSTARTWCNT_W                                        16U
#define I2S_STMPINTRIG_INSTARTWCNT_M                                0x0000FFFFU
#define I2S_STMPINTRIG_INSTARTWCNT_S                                         0U

//*****************************************************************************
//
// Register: I2S_O_STMPOUTTRIG
//
//*****************************************************************************
// Field:  [15:0] OUTSTARTWCNT
//
// This field cofnigures the compare value used to start the outgoing audio
// streams.
//
// This bit field must equal the WCLK counter value during the WCLK period in
// which the first output word(s) read from memory are clocked out (that is the
// sample at the start of the very first DMA output buffer).
//
// The value of this register takes effect when the following conditions are
// met:
// - One or more pins are configured as outputs in DIRCFG.
// - DMACFG has been configured for the correct buffer size, and 32 BCLK cycle
// ticks have happened.
// - 2 samples have been preloaded from memory (examine the OUTPTR register if
// necessary).
// Note: The memory read access is only performed when required, that is
// channels 0/1 must be selected in WMASK0/WMASK1.
//
// Note: To avoid false triggers, this bit field should be set higher than
// STMPWPER.VALUE.
#define I2S_STMPOUTTRIG_OUTSTARTWCNT_W                                      16U
#define I2S_STMPOUTTRIG_OUTSTARTWCNT_M                              0x0000FFFFU
#define I2S_STMPOUTTRIG_OUTSTARTWCNT_S                                       0U

//*****************************************************************************
//
// Register: I2S_O_STMPWSET
//
//*****************************************************************************
// Field:  [15:0] VALUE
//
// Sets the running WCLK counter equal to the written value.
#define I2S_STMPWSET_VALUE_W                                                16U
#define I2S_STMPWSET_VALUE_M                                        0x0000FFFFU
#define I2S_STMPWSET_VALUE_S                                                 0U

//*****************************************************************************
//
// Register: I2S_O_STMPWADD
//
//*****************************************************************************
// Field:  [15:0] VALUEINC
//
// Adds the written value to the running WCLK counter. If a positive edge of
// WCLK occurs at the same time as the operation, this will be taken into
// account.
// To add a negative value, write "STMPWPER.VALUE - value".
//
#define I2S_STMPWADD_VALUEINC_W                                             16U
#define I2S_STMPWADD_VALUEINC_M                                     0x0000FFFFU
#define I2S_STMPWADD_VALUEINC_S                                              0U

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
// Field:  [15:0] CURRVALUE
//
// Current value of the WCLK counter
#define I2S_STMPWCNT_CURRVALUE_W                                            16U
#define I2S_STMPWCNT_CURRVALUE_M                                    0x0000FFFFU
#define I2S_STMPWCNT_CURRVALUE_S                                             0U

//*****************************************************************************
//
// Register: I2S_O_STMPXCNT
//
//*****************************************************************************
// Field:  [15:0] CURRVALUE
//
// Current value of the XOSC counter, latched when reading STMPWCNT.
#define I2S_STMPXCNT_CURRVALUE_W                                            16U
#define I2S_STMPXCNT_CURRVALUE_M                                    0x0000FFFFU
#define I2S_STMPXCNT_CURRVALUE_S                                             0U

//*****************************************************************************
//
// Register: I2S_O_IRQMASK
//
//*****************************************************************************
// Field:     [6] XCNTCAPT
//
// IRQFLAGS.XCNTCAPT interrupt mask
// ENUMs:
// EN                       Enable the interrupt mask
// DIS                      Disable the interrupt mask
#define I2S_IRQMASK_XCNTCAPT                                        0x00000040U
#define I2S_IRQMASK_XCNTCAPT_M                                      0x00000040U
#define I2S_IRQMASK_XCNTCAPT_S                                               6U
#define I2S_IRQMASK_XCNTCAPT_EN                                     0x00000040U
#define I2S_IRQMASK_XCNTCAPT_DIS                                    0x00000000U

// Field:     [5] DMAIN
//
// IRQFLAGS.DMAIN interrupt mask
// ENUMs:
// EN                       Enable the interrupt mask
// DIS                      Disable the interrupt mask
#define I2S_IRQMASK_DMAIN                                           0x00000020U
#define I2S_IRQMASK_DMAIN_M                                         0x00000020U
#define I2S_IRQMASK_DMAIN_S                                                  5U
#define I2S_IRQMASK_DMAIN_EN                                        0x00000020U
#define I2S_IRQMASK_DMAIN_DIS                                       0x00000000U

// Field:     [4] DMAOUT
//
// IRQFLAGS.DMAOUT interrupt mask
// ENUMs:
// EN                       Enable the interrupt mask
// DIS                      Disable the interrupt mask
#define I2S_IRQMASK_DMAOUT                                          0x00000010U
#define I2S_IRQMASK_DMAOUT_M                                        0x00000010U
#define I2S_IRQMASK_DMAOUT_S                                                 4U
#define I2S_IRQMASK_DMAOUT_EN                                       0x00000010U
#define I2S_IRQMASK_DMAOUT_DIS                                      0x00000000U

// Field:     [3] WCLKTIMEOUT
//
// IRQFLAGS.WCLKTIMEOUT interrupt mask
// ENUMs:
// EN                       Enable the interrupt mask
// DIS                      Disable the interrupt mask
#define I2S_IRQMASK_WCLKTIMEOUT                                     0x00000008U
#define I2S_IRQMASK_WCLKTIMEOUT_M                                   0x00000008U
#define I2S_IRQMASK_WCLKTIMEOUT_S                                            3U
#define I2S_IRQMASK_WCLKTIMEOUT_EN                                  0x00000008U
#define I2S_IRQMASK_WCLKTIMEOUT_DIS                                 0x00000000U

// Field:     [2] BUSERR
//
// IRQFLAGS.BUSERR interrupt mask
// ENUMs:
// EN                       Enable the interrupt mask
// DIS                      Disable the interrupt mask
#define I2S_IRQMASK_BUSERR                                          0x00000004U
#define I2S_IRQMASK_BUSERR_M                                        0x00000004U
#define I2S_IRQMASK_BUSERR_S                                                 2U
#define I2S_IRQMASK_BUSERR_EN                                       0x00000004U
#define I2S_IRQMASK_BUSERR_DIS                                      0x00000000U

// Field:     [1] WCLKERR
//
// IRQFLAGS.WCLKERR interrupt mask
// ENUMs:
// EN                       Enable the interrupt mask
// DIS                      Disable the interrupt mask
#define I2S_IRQMASK_WCLKERR                                         0x00000002U
#define I2S_IRQMASK_WCLKERR_M                                       0x00000002U
#define I2S_IRQMASK_WCLKERR_S                                                1U
#define I2S_IRQMASK_WCLKERR_EN                                      0x00000002U
#define I2S_IRQMASK_WCLKERR_DIS                                     0x00000000U

// Field:     [0] PTRERR
//
// IRQFLAGS.PTRERR interrupt mask.
// ENUMs:
// EN                       Enable the interrupt mask
// DIS                      Disable the interrupt mask
#define I2S_IRQMASK_PTRERR                                          0x00000001U
#define I2S_IRQMASK_PTRERR_M                                        0x00000001U
#define I2S_IRQMASK_PTRERR_S                                                 0U
#define I2S_IRQMASK_PTRERR_EN                                       0x00000001U
#define I2S_IRQMASK_PTRERR_DIS                                      0x00000000U

//*****************************************************************************
//
// Register: I2S_O_IRQFLAGS
//
//*****************************************************************************
// Field:     [6] XCNTCAPT
//
// This interrupt is set when xcnt counter is captured either by events or
// software.
// Needs to be cleared by software.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define I2S_IRQFLAGS_XCNTCAPT                                       0x00000040U
#define I2S_IRQFLAGS_XCNTCAPT_M                                     0x00000040U
#define I2S_IRQFLAGS_XCNTCAPT_S                                              6U
#define I2S_IRQFLAGS_XCNTCAPT_SET                                   0x00000040U
#define I2S_IRQFLAGS_XCNTCAPT_CLR                                   0x00000000U

// Field:     [5] DMAIN
//
// This interrupt is set when condition for this bit field event occurs (auto
// cleared when input pointer is updated - AIFINPTRNEXT), see description of
// AIFINPTRNEXT register for details.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define I2S_IRQFLAGS_DMAIN                                          0x00000020U
#define I2S_IRQFLAGS_DMAIN_M                                        0x00000020U
#define I2S_IRQFLAGS_DMAIN_S                                                 5U
#define I2S_IRQFLAGS_DMAIN_SET                                      0x00000020U
#define I2S_IRQFLAGS_DMAIN_CLR                                      0x00000000U

// Field:     [4] DMAOUT
//
// This interrupt is set when condition for this bit field event occurs (auto
// cleared when output pointer is updated - OUTPTRNXT), see description of
// OUTPTRNXT register for details
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define I2S_IRQFLAGS_DMAOUT                                         0x00000010U
#define I2S_IRQFLAGS_DMAOUT_M                                       0x00000010U
#define I2S_IRQFLAGS_DMAOUT_S                                                4U
#define I2S_IRQFLAGS_DMAOUT_SET                                     0x00000010U
#define I2S_IRQFLAGS_DMAOUT_CLR                                     0x00000000U

// Field:     [3] WCLKTIMEOUT
//
// Set when the sample stamp generator does not detect a positive WCLK edge for
//  65535 clk periods. This signalizes that the internal or external BCLK and
// WCLK generator source has been disabled.
//
// The bit is sticky and may only be cleared by software (by writing '1' to
// IRQCLR.WCLKTIMEOUT).
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define I2S_IRQFLAGS_WCLKTIMEOUT                                    0x00000008U
#define I2S_IRQFLAGS_WCLKTIMEOUT_M                                  0x00000008U
#define I2S_IRQFLAGS_WCLKTIMEOUT_S                                           3U
#define I2S_IRQFLAGS_WCLKTIMEOUT_SET                                0x00000008U
#define I2S_IRQFLAGS_WCLKTIMEOUT_CLR                                0x00000000U

// Field:     [2] BUSERR
//
// This interrupt set when a DMA operation is not completed in time (that is
// audio output buffer underflow, or audio input buffer overflow).
// This error requires a complete restart since word synchronization has been
// lost. The bit is sticky and may only be cleared by software (by writing '1'
// to IRQCLR.BUSERR).
//
// Note that DMA initiated transactions to illegal addresses will not trigger
// an interrupt. The response to such transactions is undefined.
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define I2S_IRQFLAGS_BUSERR                                         0x00000004U
#define I2S_IRQFLAGS_BUSERR_M                                       0x00000004U
#define I2S_IRQFLAGS_BUSERR_S                                                2U
#define I2S_IRQFLAGS_BUSERR_SET                                     0x00000004U
#define I2S_IRQFLAGS_BUSERR_CLR                                     0x00000000U

// Field:     [1] WCLKERR
//
// This interrupt is set when:
// - An unexpected WCLK edge occurs during the data delay period of a phase.
// Note unexpected WCLK edges during the word and idle periods of the phase are
// not detected.
// -  In dual-phase mode, when two WCLK edges are less than 4 BCLK cycles
// apart.
// -  In single-phase mode, when a WCLK pulse occurs before the last channel.
// This error requires a complete restart since word synchronization has been
// lost. The bit is sticky and may only be cleared by software (by writing '1'
// to IRQCLR.WCLKERR).
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define I2S_IRQFLAGS_WCLKERR                                        0x00000002U
#define I2S_IRQFLAGS_WCLKERR_M                                      0x00000002U
#define I2S_IRQFLAGS_WCLKERR_S                                               1U
#define I2S_IRQFLAGS_WCLKERR_SET                                    0x00000002U
#define I2S_IRQFLAGS_WCLKERR_CLR                                    0x00000000U

// Field:     [0] PTRERR
//
// This interrupt set when INPTRNXT or OUTPTRNXT has not been loaded with the
// next block address in time.
// This error requires a complete restart since word synchronization has been
// lost. The bit is sticky and may only be cleared by software (by writing '1'
// to IRQCLR.PTRERR).
// ENUMs:
// SET                      Interrupt occured
// CLR                      Interrupt did not occur
#define I2S_IRQFLAGS_PTRERR                                         0x00000001U
#define I2S_IRQFLAGS_PTRERR_M                                       0x00000001U
#define I2S_IRQFLAGS_PTRERR_S                                                0U
#define I2S_IRQFLAGS_PTRERR_SET                                     0x00000001U
#define I2S_IRQFLAGS_PTRERR_CLR                                     0x00000000U

//*****************************************************************************
//
// Register: I2S_O_IRQSET
//
//*****************************************************************************
// Field:     [6] XCNTCAPT
//
// This field sets the interrupt IRQFLAGS.XCNTCAPT (unless a auto clear
// criteria was given at the same time, in which the set will be ignored)
// ENUMs:
// SET                      Set interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQSET_XCNTCAPT                                         0x00000040U
#define I2S_IRQSET_XCNTCAPT_M                                       0x00000040U
#define I2S_IRQSET_XCNTCAPT_S                                                6U
#define I2S_IRQSET_XCNTCAPT_SET                                     0x00000040U
#define I2S_IRQSET_XCNTCAPT_NOEFF                                   0x00000000U

// Field:     [5] DMAIN
//
// This field sets the interrupt IRQFLAGS.DMAIN (unless a auto clear criteria
// was given at the same time, in which the set will be ignored)
// ENUMs:
// SET                      Set interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQSET_DMAIN                                            0x00000020U
#define I2S_IRQSET_DMAIN_M                                          0x00000020U
#define I2S_IRQSET_DMAIN_S                                                   5U
#define I2S_IRQSET_DMAIN_SET                                        0x00000020U
#define I2S_IRQSET_DMAIN_NOEFF                                      0x00000000U

// Field:     [4] DMAOUT
//
// This field sets the interrupt IRQFLAGS.DMAOUT (unless a auto clear criteria
// was given at the same time, in which the set will be ignored)
// ENUMs:
// SET                      Set interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQSET_DMAOUT                                           0x00000010U
#define I2S_IRQSET_DMAOUT_M                                         0x00000010U
#define I2S_IRQSET_DMAOUT_S                                                  4U
#define I2S_IRQSET_DMAOUT_SET                                       0x00000010U
#define I2S_IRQSET_DMAOUT_NOEFF                                     0x00000000U

// Field:     [3] WCLKTIMEOUT
//
// This field sets the interrupt IRQFLAGS.WCLKTIMEOUT(unless a auto clear
// criteria was given at the same time, in which the set will be ignored)
// ENUMs:
// SET                      Set interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQSET_WCLKTIMEOUT                                      0x00000008U
#define I2S_IRQSET_WCLKTIMEOUT_M                                    0x00000008U
#define I2S_IRQSET_WCLKTIMEOUT_S                                             3U
#define I2S_IRQSET_WCLKTIMEOUT_SET                                  0x00000008U
#define I2S_IRQSET_WCLKTIMEOUT_NOEFF                                0x00000000U

// Field:     [2] BUSERR
//
// 1: Sets the interrupt of IRQFLAGS.BUSERR(unless a auto clear criteria was
// given at the same time, in which the set will be ignored)
// ENUMs:
// SET                      Set interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQSET_BUSERR                                           0x00000004U
#define I2S_IRQSET_BUSERR_M                                         0x00000004U
#define I2S_IRQSET_BUSERR_S                                                  2U
#define I2S_IRQSET_BUSERR_SET                                       0x00000004U
#define I2S_IRQSET_BUSERR_NOEFF                                     0x00000000U

// Field:     [1] WCLKERR
//
// This field sets the interrupt IRQFLAGS.WCLKERR(unless a auto clear criteria
// was given at the same time, in which the set will be ignored)
// ENUMs:
// SET                      Set interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQSET_WCLKERR                                          0x00000002U
#define I2S_IRQSET_WCLKERR_M                                        0x00000002U
#define I2S_IRQSET_WCLKERR_S                                                 1U
#define I2S_IRQSET_WCLKERR_SET                                      0x00000002U
#define I2S_IRQSET_WCLKERR_NOEFF                                    0x00000000U

// Field:     [0] PTRERR
//
// This field sets the interrupt IRQFLAGS.PTRERR(unless a auto clear criteria
// was given at the same time, in which the set will be ignored)
// ENUMs:
// SET                      Set interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQSET_PTRERR                                           0x00000001U
#define I2S_IRQSET_PTRERR_M                                         0x00000001U
#define I2S_IRQSET_PTRERR_S                                                  0U
#define I2S_IRQSET_PTRERR_SET                                       0x00000001U
#define I2S_IRQSET_PTRERR_NOEFF                                     0x00000000U

//*****************************************************************************
//
// Register: I2S_O_IRQCLR
//
//*****************************************************************************
// Field:     [6] XCNTCAPT
//
// Ths field clears the interrupt IRQFLAGS.XCNTCAPT (unless a set criteria was
// given at the same time in which the clear will be ignored)
// ENUMs:
// CLR                      Clear interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQCLR_XCNTCAPT                                         0x00000040U
#define I2S_IRQCLR_XCNTCAPT_M                                       0x00000040U
#define I2S_IRQCLR_XCNTCAPT_S                                                6U
#define I2S_IRQCLR_XCNTCAPT_CLR                                     0x00000040U
#define I2S_IRQCLR_XCNTCAPT_NOEFF                                   0x00000000U

// Field:     [5] DMAIN
//
// This field clears the interrupt of IRQFLAGS.DMAIN (unless a set criteria was
// given at the same time in which the clear will be ignored)
// ENUMs:
// CLR                      Clear interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQCLR_DMAIN                                            0x00000020U
#define I2S_IRQCLR_DMAIN_M                                          0x00000020U
#define I2S_IRQCLR_DMAIN_S                                                   5U
#define I2S_IRQCLR_DMAIN_CLR                                        0x00000020U
#define I2S_IRQCLR_DMAIN_NOEFF                                      0x00000000U

// Field:     [4] DMAOUT
//
// This field clears the interrupt IRQFLAGS.DMAOUT (unless a set criteria was
// given at the same time in which the clear will be ignored)
// ENUMs:
// CLR                      Clear interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQCLR_DMAOUT                                           0x00000010U
#define I2S_IRQCLR_DMAOUT_M                                         0x00000010U
#define I2S_IRQCLR_DMAOUT_S                                                  4U
#define I2S_IRQCLR_DMAOUT_CLR                                       0x00000010U
#define I2S_IRQCLR_DMAOUT_NOEFF                                     0x00000000U

// Field:     [3] WCLKTIMEOUT
//
// 1: Clears the interrupt of IRQFLAGS.WCLKTIMEOUT (unless a set criteria was
// given at the same time in which the clear will be ignored)
// ENUMs:
// CLR                      Clear interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQCLR_WCLKTIMEOUT                                      0x00000008U
#define I2S_IRQCLR_WCLKTIMEOUT_M                                    0x00000008U
#define I2S_IRQCLR_WCLKTIMEOUT_S                                             3U
#define I2S_IRQCLR_WCLKTIMEOUT_CLR                                  0x00000008U
#define I2S_IRQCLR_WCLKTIMEOUT_NOEFF                                0x00000000U

// Field:     [2] BUSERR
//
// This field clears the interrupt IRQFLAGS.BUSERR (unless a set criteria was
// given at the same time in which the clear will be ignored)
// ENUMs:
// CLR                      Clear interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQCLR_BUSERR                                           0x00000004U
#define I2S_IRQCLR_BUSERR_M                                         0x00000004U
#define I2S_IRQCLR_BUSERR_S                                                  2U
#define I2S_IRQCLR_BUSERR_CLR                                       0x00000004U
#define I2S_IRQCLR_BUSERR_NOEFF                                     0x00000000U

// Field:     [1] WCLKERR
//
// This field clears the interrupt IRQFLAGS.WCLKERR (unless a set criteria was
// given at the same time in which the clear will be ignored)
// ENUMs:
// CLR                      Clear interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQCLR_WCLKERR                                          0x00000002U
#define I2S_IRQCLR_WCLKERR_M                                        0x00000002U
#define I2S_IRQCLR_WCLKERR_S                                                 1U
#define I2S_IRQCLR_WCLKERR_CLR                                      0x00000002U
#define I2S_IRQCLR_WCLKERR_NOEFF                                    0x00000000U

// Field:     [0] PTRERR
//
// This field clears the interrupt IRQFLAGS.PTRERR (unless a set criteria was
// given at the same time in which the clear will be ignored)
// ENUMs:
// CLR                      Clear interrupt
// NOEFF                    Writing 0 has no effect
#define I2S_IRQCLR_PTRERR                                           0x00000001U
#define I2S_IRQCLR_PTRERR_M                                         0x00000001U
#define I2S_IRQCLR_PTRERR_S                                                  0U
#define I2S_IRQCLR_PTRERR_CLR                                       0x00000001U
#define I2S_IRQCLR_PTRERR_NOEFF                                     0x00000000U

//*****************************************************************************
//
// Register: I2S_O_MCLKDIV
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
#define I2S_MCLKDIV_MDIV_W                                                  10U
#define I2S_MCLKDIV_MDIV_M                                          0x000003FFU
#define I2S_MCLKDIV_MDIV_S                                                   0U

//*****************************************************************************
//
// Register: I2S_O_BCLKDIV
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
// FMTCFG.SMPLEDGE = 0, the low phase of the clock is one MCUCLK period longer
// than the high phase.
//                                             If BDIV is odd and
// FMTCFG.SMPLEDGE = 1 , the high phase of the clock is one MCUCLK period
// longer than the low phase.
#define I2S_BCLKDIV_BDIV_W                                                  10U
#define I2S_BCLKDIV_BDIV_M                                          0x000003FFU
#define I2S_BCLKDIV_BDIV_S                                                   0U

//*****************************************************************************
//
// Register: I2S_O_WCLKDIV
//
//*****************************************************************************
// Field:  [15:0] WDIV
//
//    If CLKCTL.WCLKPHASE = 0, Single phase. WCLK is high one BCLK period and
// low WDIV[9:0] (unsigned, [1-1023]) BCLK periods.
//                                            WCLK = MCUCLK / BDIV*(WDIV[9:0] +
// 1) [Hz] MCUCLK upto 96MHz.
//                                            If [CLKCTL.WCLKPHASE.*] = 1, Dual
// phase. Each phase on WCLK (50% duty cycle) is WDIV[9:0] (unsigned,
// [1-1023]) BCLK periods.
//                                            **WCLK **= **MCUCLK **/
// BDIV*(2*WDIV[9:0]) [Hz]
//                                            If [CLKCTL.WCLKPHASE.*] = 2, User
// defined. WCLK is high WDIV[7:0] (unsigned, [1-255]) BCLK periods and low
// WDIV[15:8] (unsigned, [1-255]) BCLK periods.
//                                            WCLK = **MCUCLK **/
// (BDIV*(WDIV[7:0] + WDIV[15:8]) [Hz]
#define I2S_WCLKDIV_WDIV_W                                                  16U
#define I2S_WCLKDIV_WDIV_M                                          0x0000FFFFU
#define I2S_WCLKDIV_WDIV_S                                                   0U

//*****************************************************************************
//
// Register: I2S_O_CLKCTL
//
//*****************************************************************************
// Field:     [3] MEN
//
// This field configures the MCLK generation
// ENUMs:
// EN                       Enable the generation
// DIS                      Disable the generation
#define I2S_CLKCTL_MEN                                              0x00000008U
#define I2S_CLKCTL_MEN_M                                            0x00000008U
#define I2S_CLKCTL_MEN_S                                                     3U
#define I2S_CLKCTL_MEN_EN                                           0x00000008U
#define I2S_CLKCTL_MEN_DIS                                          0x00000000U

// Field:   [2:1] WCLKPHASE
//
// The field configures how the WCLK division ratio is calculated and used to
// generate different duty cycles (See WCLKDIV.WDIV)
#define I2S_CLKCTL_WCLKPHASE_W                                               2U
#define I2S_CLKCTL_WCLKPHASE_M                                      0x00000006U
#define I2S_CLKCTL_WCLKPHASE_S                                               1U

// Field:     [0] WBEN
//
// This field configures WCLK/BCLK generation
// ENUMs:
// EN                       Enable the generation
// DIS                      Disables the generation
#define I2S_CLKCTL_WBEN                                             0x00000001U
#define I2S_CLKCTL_WBEN_M                                           0x00000001U
#define I2S_CLKCTL_WBEN_S                                                    0U
#define I2S_CLKCTL_WBEN_EN                                          0x00000001U
#define I2S_CLKCTL_WBEN_DIS                                         0x00000000U

//*****************************************************************************
//
// Register: I2S_O_DTB
//
//*****************************************************************************
// Field:   [1:0] SEL
//
// The field controls the select of DTB
// ENUMs:
// SEL_3                    {12{1'b0},aif_words_sampled, aif_word_loaded,
//                          aif_output_en, aif_input_en}
// SEL_2                    Samplestamp XCNT(16 bits)
// SEL_1                    Samplestamp WCNT(16 bits)
// SEL_0                    None is selected
#define I2S_DTB_SEL_W                                                        2U
#define I2S_DTB_SEL_M                                               0x00000003U
#define I2S_DTB_SEL_S                                                        0U
#define I2S_DTB_SEL_SEL_3                                           0x00000003U
#define I2S_DTB_SEL_SEL_2                                           0x00000002U
#define I2S_DTB_SEL_SEL_1                                           0x00000001U
#define I2S_DTB_SEL_SEL_0                                           0x00000000U


#endif // __I2S__
