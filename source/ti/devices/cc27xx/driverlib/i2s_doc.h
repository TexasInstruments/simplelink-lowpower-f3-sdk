/******************************************************************************
 *  Filename:       i2s_doc.h
 *
 *  Copyright (c) 2023 Texas Instruments Incorporated
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
 *  3) Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
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
 *
 ******************************************************************************/
//! \addtogroup i2s_api
//! @{
//! \section sec_i2s Introduction
//!
//! The I2S API provides a set of functions for using the I2S module.
//! This module provides a standardized serial interface to transfer
//! audio samples from and to external audio devices such as a codec,
//! DAC, or ADC.
//!
//! The I2S module has the following features:
//!  - Audio clock signals are internally generated, or externally by another
//!    device.
//!  - One or two data pins which can be configured independently as
//!    input or output
//!  - Various data formats according to the settings of the module
//!  - Up to two channels per data pin for dual phase formats and up
//!    to eight channels per data pin for single phase formats
//!  - DMA with double-buffered pointers
//!  - Error detection for DMA and audio clock signal integrity
//!  - A sample stamp generator that allows maintaining constant audio latency
//!
//! The I2S module is configured through the functions \ref I2SConfigureFormat(),
//! \ref I2SConfigureFrame() and \ref I2SConfigureClocks().
//! If controller mode is used, then the internally generated clocks can be
//! enabled and disabled using \ref I2SEnableControllerClocks() and
//! \ref I2SDisableControllerClocks(), respectively.
//! Transfers are enabled using \ref I2SStart(). Transfers are disabled using
//! \ref I2SStop(). Please note that a specific procedure exists to disable
//! transfers without losing data (refer to \ref I2SStop()).
//!
//! Data are transmitted using the two double-buffered pointers.
//! For each interface, two registers are set with the address of the data to
//! transfer. These registers are named INPTR and INPTRNEXT for the input
//! interface and OUTPTR and OUTPTRNEXT for the output. When PTR is consumed,
//! the hardware copies the content of PTRNEXT into PTR and the next transfer
//! begins.
//! The address of the next value to write or to read in memory (i.e. to receive
//! or to send out) is set using \ref I2SSetInPointer() and
//! \ref I2SSetOutPointer(). The values stored in INPTRNEXT, OUTPTRNEXT, INPTR
//! and OUTPTR can be read using \ref I2SGetInPointerNext(),
//! \ref I2SGetOutPointerNext(), \ref I2SGetInPointer() and
//! \ref I2SGetOutPointer() functions.
//!
//! Interrupts can help the user to refresh pointers on time. Interrupts can also
//! be used to detect I2S errors. \ref I2SEnableInt() and \ref I2SDisableInt()
//! activate and deactivate interrupt(s). Interrupt status can be read through
//! \ref I2SIntStatus() and pending interrupt flags can be cleared by
//! \ref I2SClearInt() function.
//!
//! The sample stamps generator can be configured to start the emission or the
//! reception of the data at specified WCLK counter trigger values using
//! \ref I2SConfigureInSampleStampTrigger(),
//! \ref I2SConfigureOutSampleStampTrigger(), \ref I2SResetWclkCounter() and
//! \ref I2SConfigureWclkCounter(). The current sample stamp can be computed
//! using \ref I2SGetSampleStamp().
//! To finish, the sample stamps generator can be enabled and disabled using
//! the following functions: \ref I2SEnableSampleStamp() and
//! \ref I2SDisableSampleStamp().
//! The sample stamps generator must be enabled prior to any transfer.
//!
//! \section sec_i2s_api API
//! Functions to perform I2S configuration:
//!  - \ref I2SStart()
//!  - \ref I2SStop()
//!  - \ref I2SConfigureFormat()
//!  - \ref I2SConfigureFrame()
//!  - \ref I2SConfigureClocks()
//!  - \ref I2SEnableControllerClocks()
//!  - \ref I2SDisableControllerClocks()
//!
//! Functions to perform transfers:
//!  - \ref I2SSetInPointer()
//!  - \ref I2SSetOutPointer()
//!  - \ref I2SGetInPointer()
//!  - \ref I2SGetOutPointer()
//!  - \ref I2SGetInPointerNext()
//!  - \ref I2SGetOutPointerNext()
//!
//! Functions to handle interruptions:
//!  - \ref I2SEnableInt()
//!  - \ref I2SDisableInt()
//!  - \ref I2SIntStatus()
//!  - \ref I2SClearInt()
//!
//! Functions to handle sample stamps:
//!  - \ref I2SEnableSampleStamp()
//!  - \ref I2SDisableSampleStamp()
//!  - \ref I2SConfigureInSampleStampTrigger()
//!  - \ref I2SConfigureOutSampleStampTrigger()
//!  - \ref I2SGetSampleStamp()
//!  - \ref I2SConfigureWclkCounterPeriod()
//!  - \ref I2SConfigureWclkCounter()
//!  - \ref I2SResetWclkCounter()
//!
//! @}