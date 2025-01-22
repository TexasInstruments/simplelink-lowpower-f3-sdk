
/******************************************************************************
 *  Filename:       can.h
 *
 *  Description:    Defines and prototypes for the CAN peripheral.
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

#ifndef __CAN_H__
#define __CAN_H__

//*****************************************************************************
//
//! \addtogroup peripheral_group
//! @{
//! \addtogroup can_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_canfd.h"

//*****************************************************************************
//
// Flags that can be passed to CANSSSetControl and CANSSClearControl.
//
//*****************************************************************************
#define CANSS_CTRL_EXT_TIMESTAMP_EN (CANFD_MCANSS_CTRL_EXT_TS_CNTR_EN_M) //!< External Timestamp Counter Enable
#define CANSS_CTRL_AUTOWAKEUP_EN    (CANFD_MCANSS_CTRL_AUTOWAKEUP_M)     //!< Automatic Wakeup Enable
#define CANSS_CTRL_WAKEUP_REQ_EN    (CANFD_MCANSS_CTRL_WAKEUPREQEN_M)    //!< Wakeup Request Enable
#define CANSS_CTRL_DEBUG_SUSPEND_EN (CANFD_MCANSS_CTRL_DBGSUSP_FREE_M)   //!< Debug Suspend Enable

//*****************************************************************************
//
// Flags that can be returned by CANSSGetStatus.
//
//*****************************************************************************
#define CANSS_STATUS_ENABLE_FDOE   (CANFD_MCANSS_STAT_ENABLE_FDOE_M)   //!< CAN FD Operation Enabled
#define CANSS_STATUS_MEM_INIT_DONE (CANFD_MCANSS_STAT_MEM_INIT_DONE_M) //!< Memory Initialization Done

//*****************************************************************************
//
// Flags that can be passed to CANSSClearInt, CANSSEnableInt, and
// CANSSDisableInt or returned by CANSSGetIntStatus.
//
//*****************************************************************************
#define CANSS_INT_EXT_TIMESTAMP_OVERFLOW \
    (CANFD_MCANSS_IRS_EXT_TS_CNTR_OVFL_M) //!< External Timestamp Counter Overflow Interrupt.

//*****************************************************************************
//
// Flags that can be passed to CANSSSetEndOfInterrupt.
//
//*****************************************************************************
#define CANSS_EOI_EXT_TIMESTAMP 0U //!< End Of Interrupt for External Timestamp
#define CANSS_EOI_MCAN0         1U //!< End Of Interrupt for MCAN Interrupt Line 0
#define CANSS_EOI_MCAN1         2U //!< End Of Interrupt for MCAN Interrupt Line 1

//*****************************************************************************
//
// Interrupt line numbers that can be passed to CANSetIntMask, CANClearIntMask,
// CANGetIntStatus, and CANClearInt.
//
//*****************************************************************************
#define CAN_INT_LINE0 0U //!< Interrupt Line 0
#define CAN_INT_LINE1 1U //!< Interrupt Line 1

//*****************************************************************************
//
// Flags that can be passed to CANSetIntMask, CANClearIntMask, and CANClearInt
// or returned by CANGetIntStatus.
//
//*****************************************************************************
#define CAN_INT_DMA_DONE1      (CANFD_IMASK0_DMA_DONE1_M)      //!< DMA Done Channel 1 Event Interrupt
#define CAN_INT_DMA_DONE0      (CANFD_IMASK0_DMA_DONE0_M)      //!< DMA Done Channel 0 Event Interrupt
#define CAN_INT_FILTER_EVENT2  (CANFD_IMASK0_FE2_M)            //!< Rx Buffer Filter Event 2 Interrupt
#define CAN_INT_EXT_TS_OR_WAKE (CANFD_IMASK0_EXT_TS_OR_WAKE_M) //!< External Timestamp or Wake Event Interrupt
#define CAN_INT_DED            (CANFD_IMASK0_DED_M)            //!< Double Error Detected Event Interrupt
#define CAN_INT_SEC            (CANFD_IMASK0_SEC_M)            //!< Single Error Corrected Event Interrupt
#define CAN_INT_INTL1          (CANFD_IMASK0_INTL1_M)          //!< MCAN Interrupt Line 1 Event Interrupt
#define CAN_INT_INTL0          (CANFD_IMASK0_INTL0_M)          //!< MCAN Interrupt Line 0 Event Interrupt
#define CAN_INT_MASK           (0x0000000FU)                   //!< Mask of all interrupts

//*****************************************************************************
//
// Clock divider ratios that can be passed to CANSSSetClockDivider.
//
//*****************************************************************************
#define CANSS_CLK_DIVIDE_BY_1 0U //!< Divides input clock by 1
#define CANSS_CLK_DIVIDE_BY_2 1U //!< Divides input clock by 2
#define CANSS_CLK_DIVIDE_BY_4 2U //!< Divides input clock by 4

//*****************************************************************************
//
// Flags that can be passed to CANSSSetClkStopCtrl and CANSSClearClkStopCtrl.
//
//*****************************************************************************
#define CANSS_CLKCTRL_WAKEUP_GLITCH_FILTER_EN (CANFD_MCANSS_CLKCTL_WKUP_GLTFLT_EN_M) //!< Wakeup Glitch Filter Enable
#define CANSS_CLKCTRL_WAKEUP_INT_EN           (CANFD_MCANSS_CLKCTL_WAKEUP_INT_EN_M)  //!< Wakeup Interrupt Enable
#define CANSS_CLKCTRL_STOP_REQ                (CANFD_MCANSS_CLKCTL_STOPREQ_M)        //!< Clock Stop Request

//*****************************************************************************
//
// Flags that can be returned by CANSSGetClkStopCtrlStatus.
//
//*****************************************************************************
#define CANSS_CLKSTAT_CLKSTOP_REQ_OVR \
    (CANFD_MCANSS_CLKSTS_STOPREQ_HW_OVR_M) //!< Clock Stop Request Hardware Override (1 = Request cleared by HW)
#define CANSS_CLKSTAT_CLKSTOP_ACK (CANFD_MCANSS_CLKSTS_CLKSTOP_ACKSTS_M) //!< Clock Stop Acknowledge

//*****************************************************************************
//
//! \brief Sets the specified CAN Subsystem control feature flag(s).
//!
//! \param flags is a bit mask of the features to enable.
//! The parameter is the bitwise OR of any of the following values:
//! - \ref CANSS_CTRL_EXT_TIMESTAMP_EN : External Timestamp Counter Enable
//! - \ref CANSS_CTRL_AUTOWAKEUP_EN    : Automatic Wakeup Enable
//! - \ref CANSS_CTRL_WAKEUP_REQ_EN    : Wakeup Request Enable
//! - \ref CANSS_CTRL_DEBUG_SUSPEND_EN : Debug Suspend Enable
//!
//! \sa CANSSClearControl()
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSSSetControl(uint32_t flags)
{
    HWREG(CANFD_BASE + CANFD_O_MCANSS_CTRL) |= flags;
}

//*****************************************************************************
//
//! \brief Clears the specified CAN Subsystem control feature flag(s).
//!
//! \param flags is a bit mask of the features to disable.
//! The parameter is the bitwise OR of any of the following values:
//! - \ref CANSS_CTRL_EXT_TIMESTAMP_EN : External Timestamp Counter Enable
//! - \ref CANSS_CTRL_AUTOWAKEUP_EN    : Automatic Wakeup Enable
//! - \ref CANSS_CTRL_WAKEUP_REQ_EN    : Wakeup Request Enable
//! - \ref CANSS_CTRL_DEBUG_SUSPEND_EN : Debug Suspend Enable
//!
//! \sa CANSSSetControl()
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSSClearControl(uint32_t flags)
{
    HWREG(CANFD_BASE + CANFD_O_MCANSS_CTRL) &= (uint32_t)~flags;
}

//*****************************************************************************
//
//! \brief Gets the CAN Subsystem status.
//!
//! \note  After reset, SW must wait for 'Memory Initialization Done' before
//! the MCAN core is configured and activated.
//!
//! \return A bit mask of the CAN Subsystem status:
//! - \ref CANSS_STATUS_ENABLE_FDOE        : CAN FD Operation Enabled
//! - \ref CANSS_STATUS_MEM_INIT_DONE      : Memory Initialization Done
//
//*****************************************************************************
__STATIC_INLINE uint32_t CANSSGetStatus(void)
{
    return (HWREG(CANFD_BASE + CANFD_O_MCANSS_STAT));
}

//*****************************************************************************
//
//! \brief Gets the CAN Subsystem masked interrupt status.
//!
//! \return The CAN Subsystem masked interrupt status, enumerated as a bit field of:
//! - \ref CANSS_INT_EXT_TIMESTAMP_OVERFLOW : External Timestamp Counter Overflow.
//
//*****************************************************************************
__STATIC_INLINE uint32_t CANSSGetMaskedIntStatus(void)
{
    return (HWREG(CANFD_BASE + CANFD_O_MCANSS_IES));
}

//*****************************************************************************
//
//! \brief Gets the CAN Subsystem raw interrupt status.
//!
//! \return The CAN Subsystem raw interrupt status, enumerated as a bit field of:
//! - \ref CANSS_INT_EXT_TIMESTAMP_OVERFLOW : External Timestamp Counter Overflow.
//
//*****************************************************************************
__STATIC_INLINE uint32_t CANSSGetRawIntStatus(void)
{
    return (HWREG(CANFD_BASE + CANFD_O_MCANSS_IRS));
}

//*****************************************************************************
//
//! \brief Clears the specified CAN Subsytem interrupt enable(s).
//!
//! \param flags A bit mask of the CAN interrupts to clear:
//! The parameter is the bitwise OR of any of the following values:
//! - \ref CANSS_INT_EXT_TIMESTAMP_OVERFLOW : External Timestamp Counter Overflow
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSSClearInt(uint32_t flags)
{
    HWREG(CANFD_BASE + CANFD_O_MCANSS_ICS) = flags;
}

//*****************************************************************************
//
//! \brief Enables the specified CAN Subsytem interrupt(s).
//!
//! \param flags A bit mask of the CAN Subsytem interrupts:
//! The parameter is the bitwise OR of any of the following values:
//! - \ref CANSS_INT_EXT_TIMESTAMP_OVERFLOW : External Timestamp Counter Overflow
//!
//! \sa CANSSDisableInt()
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSSEnableInt(uint32_t flags)
{
    HWREG(CANFD_BASE + CANFD_O_MCANSS_IE) = flags;
}

//*****************************************************************************
//
//! \brief Disables the specified CAN Subsytem interrupt(s).
//!
//! \param flags A bit mask of the CAN Subsytem interrupts:
//! The parameter is the bitwise OR of any of the following values:
//! - \ref CANSS_INT_EXT_TIMESTAMP_OVERFLOW : External Timestamp Counter Overflow
//!
//! \sa CANSSEnableInt()
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSSDisableInt(uint32_t flags)
{
    HWREG(CANFD_BASE + CANFD_O_MCANSS_IECS) = flags;
}

//*****************************************************************************
//
//! \brief Sets the CAN Subsystem End Of Interrupt (EOI).
//!
//! \note  Setting the EOI will clear the associated interrupt. If the
//! unserviced interrupt counter is > 1, another interrupt is generated.
//! For MCAN Interrupts, this function should be called after clearing the flags
//! in MCAN.IR.
//!
//! \param eoi is one of the following:
//! - \ref CANSS_EOI_EXT_TIMESTAMP : End Of Interrupt for External Timestamp
//! - \ref CANSS_EOI_MCAN0         : End Of Interrupt for MCAN Interrupt Line 0
//! - \ref CANSS_EOI_MCAN1         : End Of Interrupt for MCAN Interrupt Line 1
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSSSetEndOfInt(uint32_t eoi)
{
    HWREG(CANFD_BASE + CANFD_O_MCANSS_EOI) = eoi;
}

//*****************************************************************************
//
//! \brief Sets the CAN Subsystem external timestamp prescaler.
//!
//! \param prescaler the external timestamp count rate is the host (system)
//! clock rate divided by this value, except in the case of 0. A zero value
//! will act identically to a value of 0x000001.
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSSSetExtTimestampPrescaler(uint32_t prescaler)
{
    HWREG(CANFD_BASE + CANFD_O_MCANSS_EXT_TS_PRESCALER) = prescaler;
}

//*****************************************************************************
//
//! \brief Gets the CAN Subsystem external timestamp unserviced interrupts counter.
//!
//! \return The number of external timestamp unserviced rollover interrupts.
//
//*****************************************************************************
__STATIC_INLINE uint32_t CANSSGetExtTSUnservicedIntCounter(void)
{
    return (HWREG(CANFD_BASE + CANFD_O_MCANSS_EXT_TS_UNSERVICED_INTR_CNTR));
}

//*****************************************************************************
//
//! \brief Sets the CAN peripheral interrupt mask.
//!
//! \param lineNum Interrupt Line to set the interrupt mask for:
//! - \ref CAN_INT_LINE0 : Interrupt Line 0
//! - \ref CAN_INT_LINE1 : Interrupt Line 1
//!
//! \param flags A bit mask of the CAN interrupts:
//! The parameter is the bitwise OR of any of the following values:
//! - \ref CAN_INT_DMA_DONE1      : DMA Done Channel 1 Event Interrupt
//! - \ref CAN_INT_DMA_DONE0      : DMA Done Channel 0 Event Interrupt
//! - \ref CAN_INT_FILTER_EVENT2  : Rx Buffer Filter Event 2 Interrupt
//! - \ref CAN_INT_EXT_TS_OR_WAKE : External Timestamp or Wake Event Interrupt
//! - \ref CAN_INT_DED            : Double Error Detected Event Interrupt
//! - \ref CAN_INT_SEC            : Single Error Corrected Event Interrupt
//! - \ref CAN_INT_INTL1          : MCAN Interrupt Line 1 Event Interrupt
//! - \ref CAN_INT_INTL0          : MCAN Interrupt Line 0 Event Interrupt
//! - \ref CAN_INT_MASK           : Mask of all of the above interrupts
//!
//! \sa CANClearIntMask()
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSetIntMask(uint32_t lineNum, uint32_t flags)
{
    uint32_t offset = (lineNum == CAN_INT_LINE0) ? CANFD_O_IMASK0 : CANFD_O_IMASK1;

    HWREG(CANFD_BASE + offset) |= flags;
}

//*****************************************************************************
//
//! \brief Clears the CAN peripheral interrupt mask.
//!
//! \param lineNum Interrupt Line to clear the interrupt mask for:
//! - \ref CAN_INT_LINE0 : Interrupt Line 0
//! - \ref CAN_INT_LINE1 : Interrupt Line 1
//!
//! \param flags A bit mask of the CAN interrupts:
//! The parameter is the bitwise OR of any of the following values:
//! - \ref CAN_INT_DMA_DONE1      : DMA Done Channel 1 Event Interrupt
//! - \ref CAN_INT_DMA_DONE0      : DMA Done Channel 0 Event Interrupt
//! - \ref CAN_INT_FILTER_EVENT2  : Rx Buffer Filter Event 2 Interrupt
//! - \ref CAN_INT_EXT_TS_OR_WAKE : External Timestamp or Wake Event Interrupt
//! - \ref CAN_INT_DED            : Double Error Detected Event Interrupt
//! - \ref CAN_INT_SEC            : Single Error Corrected Event Interrupt
//! - \ref CAN_INT_INTL1          : MCAN Interrupt Line 1 Event Interrupt
//! - \ref CAN_INT_INTL0          : MCAN Interrupt Line 0 Event Interrupt
//! - \ref CAN_INT_MASK           : Mask of all of the above interrupts
//!
//! \sa CANSetIntMask()
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANClearIntMask(uint8_t lineNum, uint32_t flags)
{
    uint32_t offset = (lineNum == CAN_INT_LINE0) ? CANFD_O_IMASK0 : CANFD_O_IMASK1;

    HWREG(CANFD_BASE + offset) &= (uint32_t)~flags;
}

//*****************************************************************************
//
//! \brief Gets the CAN masked peripheral interrupt status.
//!
//! \param lineNum is the interrupt line number to get the interrupt status for:
//! - \ref CAN_INT_LINE0 : Interrupt Line 0
//! - \ref CAN_INT_LINE1 : Interrupt Line 1
//!
//! \return The CAN masked interrupt status, enumerated as a bit field of:
//! - \ref CAN_INT_DMA_DONE1      : DMA Done Channel 1 Event Interrupt
//! - \ref CAN_INT_DMA_DONE0      : DMA Done Channel 0 Event Interrupt
//! - \ref CAN_INT_FILTER_EVENT2  : Rx Buffer Filter Event 2 Interrupt
//! - \ref CAN_INT_EXT_TS_OR_WAKE : External Timestamp or Wake Event Interrupt
//! - \ref CAN_INT_DED            : Double Error Detected Event Interrupt
//! - \ref CAN_INT_SEC            : Single Error Corrected Event Interrupt
//! - \ref CAN_INT_INTL1          : MCAN Interrupt Line 1 Event Interrupt
//! - \ref CAN_INT_INTL0          : MCAN Interrupt Line 0 Event Interrupt
//
//*****************************************************************************
__STATIC_INLINE uint32_t CANGetMaskedIntStatus(uint8_t lineNum)
{
    uint32_t offset = (lineNum == CAN_INT_LINE0) ? CANFD_O_MIS0 : CANFD_O_MIS1;
    return (HWREG(CANFD_BASE + offset));
}

//*****************************************************************************
//
//! \brief Gets the CAN raw peripheral interrupt status.
//!
//! \param lineNum is the interrupt line number to get the interrupt status for:
//! - \ref CAN_INT_LINE0 : Interrupt Line 0
//! - \ref CAN_INT_LINE1 : Interrupt Line 1
//!
//! \return The CAN raw interrupt status, enumerated as a bit field of:
//! - \ref CAN_INT_DMA_DONE1      : DMA Done Channel 1 Event Interrupt
//! - \ref CAN_INT_DMA_DONE0      : DMA Done Channel 0 Event Interrupt
//! - \ref CAN_INT_FILTER_EVENT2  : Rx Buffer Filter Event 2 Interrupt
//! - \ref CAN_INT_EXT_TS_OR_WAKE : External Timestamp or Wake Event Interrupt
//! - \ref CAN_INT_DED            : Double Error Detected Event Interrupt
//! - \ref CAN_INT_SEC            : Single Error Corrected Event Interrupt
//! - \ref CAN_INT_INTL1          : MCAN Interrupt Line 1 Event Interrupt
//! - \ref CAN_INT_INTL0          : MCAN Interrupt Line 0 Event Interrupt
//
//*****************************************************************************
__STATIC_INLINE uint32_t CANGetRawIntStatus(uint8_t lineNum)
{
    uint32_t offset = (lineNum == CAN_INT_LINE0) ? CANFD_O_RIS0 : CANFD_O_RIS1;
    return (HWREG(CANFD_BASE + offset));
}

//*****************************************************************************
//
//! \brief Clears the specified CAN peripheral interrupt(s).
//!
//! \note  For MCAN Interrupts, this function should be called before clearing
//! the flags in MCAN.IR.
//!
//! \param lineNum Interrupt Line to clear the interrupt mask for:
//! - \ref CAN_INT_LINE0 : Interrupt Line 0
//! - \ref CAN_INT_LINE1 : Interrupt Line 1
//!
//! \param flags A bit mask of the CAN interrupts to clear:
//! The parameter is the bitwise OR of any of the following values:
//! - \ref CAN_INT_DMA_DONE1      : DMA Done Channel 1 Event Interrupt
//! - \ref CAN_INT_DMA_DONE0      : DMA Done Channel 0 Event Interrupt
//! - \ref CAN_INT_FILTER_EVENT2  : Rx Buffer Filter Event 2 Interrupt
//! - \ref CAN_INT_EXT_TS_OR_WAKE : External Timestamp or Wake Event Interrupt
//! - \ref CAN_INT_DED            : Double Error Detected Event Interrupt
//! - \ref CAN_INT_SEC            : Single Error Corrected Event Interrupt
//! - \ref CAN_INT_INTL1          : MCAN Interrupt Line 1 Event Interrupt
//! - \ref CAN_INT_INTL0          : MCAN Interrupt Line 0 Event Interrupt
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANClearInt(uint8_t lineNum, uint32_t flags)
{
    uint32_t offset = (lineNum == CAN_INT_LINE0) ? CANFD_O_ICLR0 : CANFD_O_ICLR1;

    HWREG(CANFD_BASE + offset) = flags;
}

//*****************************************************************************
//
//! \brief Sets the CAN Subsystem clock divider ratio for the functional clock input.
//!
//! \note  The functional clock input frequency is 80 MHz.
//!
//! \param ratio is one of the following clock divider ratios:
//! - \ref CANSS_CLK_DIVIDE_BY_1 0U : Divides input clock by 1
//! - \ref CANSS_CLK_DIVIDE_BY_2 1U : Divides input clock by 2
//! - \ref CANSS_CLK_DIVIDE_BY_4 2U : Divides input clock by 4
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSSSetClockDivider(uint32_t ratio)
{
    HWREG(CANFD_BASE + CANFD_O_MCANSS_CLKDIV) = ratio;
}

//*****************************************************************************
//
//! \brief Sets the CAN Subsystem clock stop control feature flag(s).
//!
//! \param flags is a bit mask of the features to enable.
//! The parameter is the bitwise OR of any of the following values:
//! - \ref CANSS_CLKCTRL_WAKEUP_GLITCH_FILTER_EN : Wakeup Glitch Filter Enable
//! - \ref CANSS_CLKCTRL_WAKEUP_INT_EN           : Wakeup Interrupt Enable
//! - \ref CANSS_CLKCTRL_STOP_REQ                : Clock Stop Request
//!
//! \sa CANSSClearClkStopCtrl
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSSSetClkStopCtrl(uint32_t flags)
{
    HWREG(CANFD_BASE + CANFD_O_MCANSS_CLKCTL) |= flags;
}

//*****************************************************************************
//
//! \brief Clears the CAN Subsystem clock stop control feature flag(s).
//!
//! \param flags is a bit mask of the features to disable.
//! The parameter is the bitwise OR of any of the following values:
//! - \ref CANSS_CLKCTRL_WAKEUP_GLITCH_FILTER_EN : Wakeup Glitch Filter Enable
//! - \ref CANSS_CLKCTRL_WAKEUP_INT_EN           : Wakeup Interrupt Enable
//! - \ref CANSS_CLKCTRL_STOP_REQ                : Clock Stop Request
//!
//! \sa CANSSSetClkStopCtrl
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE void CANSSClearClkStopCtrl(uint32_t flags)
{
    HWREG(CANFD_BASE + CANFD_O_MCANSS_CLKCTL) &= (uint32_t)~flags;
}

//*****************************************************************************
//
//! \brief Gets the CAN Subsystem clock stop control status.
//!
//! \return The CAN Subsystem clock stop control status, enumerated as a bit field of:
//! - \ref CANSS_CLKSTAT_CLKSTOP_REQ_OVR : Clock Stop Request Hardware Override (1 = Request cleared by HW)
//! - \ref CANSS_CLKSTAT_CLKSTOP_ACK     : Clock Stop Acknowledge
//!
//! \return None
//
//*****************************************************************************
__STATIC_INLINE uint32_t CANSSGetClkStopCtrlStatus(void)
{
    return HWREG(CANFD_BASE + CANFD_O_MCANSS_CLKSTS);
}

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
//! Close the Doxygen group.
//! @}
//! @}
//
//*****************************************************************************

#endif // __CAN_H__
