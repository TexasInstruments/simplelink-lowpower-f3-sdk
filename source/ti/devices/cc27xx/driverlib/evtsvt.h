/******************************************************************************
 *  Filename:       evtsvt.h
 *
 *  Description:    Prototypes and defines for the EVTSVT API.
 *
 *  Copyright (c) 2025 Texas Instruments Incorporated
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

#ifndef __EVTSVT_H__
#define __EVTSVT_H__

//*****************************************************************************
//
//! \addtogroup system_control_group
//! @{
//! \addtogroup evtsvt_api
//! @{
//
//*****************************************************************************

#include <stdint.h>
#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_evtsvt.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
// Event subscribers
//
//*****************************************************************************

#define EVTSVT_SUB_CPUIRQ0  EVTSVT_O_CPUIRQ0SEL  //!< Subscriber ID for CPUIRQ0
#define EVTSVT_SUB_CPUIRQ1  EVTSVT_O_CPUIRQ1SEL  //!< Subscriber ID for CPUIRQ1
#define EVTSVT_SUB_CPUIRQ2  EVTSVT_O_CPUIRQ2SEL  //!< Subscriber ID for CPUIRQ2
#define EVTSVT_SUB_CPUIRQ3  EVTSVT_O_CPUIRQ3SEL  //!< Subscriber ID for CPUIRQ3
#define EVTSVT_SUB_CPUIRQ4  EVTSVT_O_CPUIRQ4SEL  //!< Subscriber ID for CPUIRQ4
#define EVTSVT_SUB_CPUIRQ16 EVTSVT_O_CPUIRQ16SEL //!< Subscriber ID for CPUIRQ16
#define EVTSVT_SUB_CPUIRQ17 EVTSVT_O_CPUIRQ17SEL //!< Subscriber ID for CPUIRQ17
#define EVTSVT_SUB_SYSTIMC1 EVTSVT_O_SYSTIMC1SEL //!< Subscriber ID for SYSTIMC1
#define EVTSVT_SUB_SYSTIMC5 EVTSVT_O_SYSTIMC5SEL //!< Subscriber ID for SYSTIMC5
#define EVTSVT_SUB_ADCTRG   EVTSVT_O_ADCTRGSEL   //!< Subscriber ID for ADCTRG
#define EVTSVT_SUB_LGPTSYNC EVTSVT_O_LGPTSYNCSEL //!< Subscriber ID for LGPTSYNC
#define EVTSVT_SUB_LGPT0IN0 EVTSVT_O_LGPT0IN0SEL //!< Subscriber ID for LGPT0IN0
#define EVTSVT_SUB_LGPT0IN1 EVTSVT_O_LGPT0IN1SEL //!< Subscriber ID for LGPT0IN1
#define EVTSVT_SUB_LGPT0IN2 EVTSVT_O_LGPT0IN2SEL //!< Subscriber ID for LGPT0IN2
#define EVTSVT_SUB_LGPT0TEN EVTSVT_O_LGPT0TENSEL //!< Subscriber ID for LGPT0TEN
#define EVTSVT_SUB_LGPT1IN0 EVTSVT_O_LGPT1IN0SEL //!< Subscriber ID for LGPT1IN0
#define EVTSVT_SUB_LGPT1IN1 EVTSVT_O_LGPT1IN1SEL //!< Subscriber ID for LGPT1IN1
#define EVTSVT_SUB_LGPT1IN2 EVTSVT_O_LGPT1IN2SEL //!< Subscriber ID for LGPT1IN2
#define EVTSVT_SUB_LGPT1TEN EVTSVT_O_LGPT1TENSEL //!< Subscriber ID for LGPT1TEN
#define EVTSVT_SUB_LGPT2IN0 EVTSVT_O_LGPT2IN0SEL //!< Subscriber ID for LGPT2IN0
#define EVTSVT_SUB_LGPT2IN1 EVTSVT_O_LGPT2IN1SEL //!< Subscriber ID for LGPT2IN1
#define EVTSVT_SUB_LGPT2IN2 EVTSVT_O_LGPT2IN2SEL //!< Subscriber ID for LGPT2IN2
#define EVTSVT_SUB_LGPT2TEN EVTSVT_O_LGPT2TENSEL //!< Subscriber ID for LGPT2TEN
#define EVTSVT_SUB_LGPT3IN0 EVTSVT_O_LGPT3IN0SEL //!< Subscriber ID for LGPT3IN0
#define EVTSVT_SUB_LGPT3IN1 EVTSVT_O_LGPT3IN1SEL //!< Subscriber ID for LGPT3IN1
#define EVTSVT_SUB_LGPT3IN2 EVTSVT_O_LGPT3IN2SEL //!< Subscriber ID for LGPT3IN2
#define EVTSVT_SUB_LGPT3TEN EVTSVT_O_LGPT3TENSEL //!< Subscriber ID for LGPT3TEN
#define EVTSVT_SUB_I2SSTMP  EVTSVT_O_I2SSTMPSEL  //!< Subscriber ID for I2SSTMP

//*****************************************************************************
//
// DMA channels
//
//*****************************************************************************

#define EVTSVT_DMA_CH0  EVTSVT_O_DMACH0SEL  //!< DMA channel 0 (DCH)
#define EVTSVT_DMA_CH1  EVTSVT_O_DMACH1SEL  //!< DMA channel 1 (DCH)
#define EVTSVT_DMA_CH2  EVTSVT_O_DMACH2SEL  //!< DMA channel 2 (DCH)
#define EVTSVT_DMA_CH3  EVTSVT_O_DMACH3SEL  //!< DMA channel 3 (DCH)
#define EVTSVT_DMA_CH4  EVTSVT_O_DMACH4SEL  //!< DMA channel 4 (DCH)
#define EVTSVT_DMA_CH5  EVTSVT_O_DMACH5SEL  //!< DMA channel 5 (DCH)
#define EVTSVT_DMA_CH6  EVTSVT_O_DMACH6SEL  //!< DMA channel 6 (DCH)
#define EVTSVT_DMA_CH7  EVTSVT_O_DMACH7SEL  //!< DMA channel 7 (DCH)
#define EVTSVT_DMA_CH8  EVTSVT_O_DMACH8SEL  //!< DMA channel 8 (ECH)
#define EVTSVT_DMA_CH9  EVTSVT_O_DMACH9SEL  //!< DMA channel 9 (ECH)
#define EVTSVT_DMA_CH10 EVTSVT_O_DMACH10SEL //!< DMA channel 10 (ECH)
#define EVTSVT_DMA_CH11 EVTSVT_O_DMACH11SEL //!< DMA channel 11 (ECH)

//*****************************************************************************
//
// DMA triggers for use use with dedicated DMA channels (DCH) in
// EVTSVTConfigureDma(). Channels 0-7 are dedicated DMA channels.
//
//*****************************************************************************

#define EVTSVT_DMA_TRIG_UART0TXTRG EVTSVT_DMACH2SEL_IPID_UART0TXTRG //!< DMA trigger for UART0 TX
#define EVTSVT_DMA_TRIG_UART0RXTRG EVTSVT_DMACH3SEL_IPID_UART0RXTRG //!< DMA trigger for UART0 RX
#define EVTSVT_DMA_TRIG_UART1TXTRG EVTSVT_DMACH1SEL_IPID_UART1TXTRG //!< DMA trigger for UART1 TX
#define EVTSVT_DMA_TRIG_UART1RXTRG EVTSVT_DMACH0SEL_IPID_UART1RXTRG //!< DMA trigger for UART1 RX
#define EVTSVT_DMA_TRIG_SPI0TXTRG  EVTSVT_DMACH0SEL_IPID_SPI0TXTRG  //!< DMA trigger for SPI0 TX
#define EVTSVT_DMA_TRIG_SPI0RXTRG  EVTSVT_DMACH1SEL_IPID_SPI0RXTRG  //!< DMA trigger for SPI0 RX
#define EVTSVT_DMA_TRIG_SPI1TXTRG  EVTSVT_DMACH6SEL_IPID_SPI1TXTRG  //!< DMA trigger for SPI1 TX
#define EVTSVT_DMA_TRIG_SPI1RXTRG  EVTSVT_DMACH7SEL_IPID_SPI1RXTRG  //!< DMA trigger for SPI1 RX
#define EVTSVT_DMA_TRIG_LRFDTRG    EVTSVT_DMACH2SEL_IPID_LRFDTRG    //!< DMA trigger for LRFD
#define EVTSVT_DMA_TRIG_ADC0TRG    EVTSVT_DMACH3SEL_IPID_ADC0TRG    //!< DMA trigger for ADC0
#define EVTSVT_DMA_TRIG_LAESTRGA   EVTSVT_DMACH4SEL_IPID_LAESTRGA   //!< DMA trigger for LAES A
#define EVTSVT_DMA_TRIG_LAESTRGB   EVTSVT_DMACH5SEL_IPID_LAESTRGB   //!< DMA trigger for LAES B
#define EVTSVT_DMA_TRIG_CANTRGA    EVTSVT_DMACH6SEL_IPID_CANTRGA    //!< DMA trigger for CAN A
#define EVTSVT_DMA_TRIG_CANTRGB    EVTSVT_DMACH7SEL_IPID_CANTRGB    //!< DMA trigger for CAN B

//*****************************************************************************
//
// Event publishers for use with EVTSVTConfigureEvent() and
// EVTSVTConfigureDma(). Channels 8-11 are event publisher DMA channels (ECH).
//
//*****************************************************************************

#define EVTSVT_PUB_SYSTIM5       EVTSVT_CPUIRQ0SEL_PUBID_SYSTIM5       //!< Publisher ID for System Timer 5
#define EVTSVT_PUB_GPIO_EVT1     EVTSVT_CPUIRQ0SEL_PUBID_GPIO_EVT1     //!< Publisher ID for GPIO Event 1
#define EVTSVT_PUB_APU_IRQ       EVTSVT_CPUIRQ0SEL_PUBID_APU_IRQ       //!< Publisher ID for APU IRQ
#define EVTSVT_PUB_SPI1_COMB     EVTSVT_CPUIRQ0SEL_PUBID_SPI1_COMB     //!< Publisher ID for SPI1 Combined event
#define EVTSVT_PUB_CAN_EVT       EVTSVT_CPUIRQ0SEL_PUBID_CAN_EVT       //!< Publisher ID for CAN Event
#define EVTSVT_PUB_CAN_IRQ       EVTSVT_CPUIRQ0SEL_PUBID_CAN_IRQ       //!< Publisher ID for CAN IRQ event
#define EVTSVT_PUB_I2S_IRQ       EVTSVT_CPUIRQ0SEL_PUBID_I2S_IRQ       //!< Publisher ID for I2S IRQ event
#define EVTSVT_PUB_LGPT3_ADC     EVTSVT_CPUIRQ0SEL_PUBID_LGPT3_ADC     //!< Publisher ID for LGPT3 ADC event
#define EVTSVT_PUB_LGPT3_DMA     EVTSVT_CPUIRQ0SEL_PUBID_LGPT3_DMA     //!< Publisher ID for LGPT3 DMA event
#define EVTSVT_PUB_LGPT3_COMB    EVTSVT_CPUIRQ0SEL_PUBID_LGPT3_COMB    //!< Publisher ID for LGPT3 Combined event
#define EVTSVT_PUB_LGPT3C2       EVTSVT_CPUIRQ0SEL_PUBID_LGPT3C2       //!< Publisher ID for LGPT3 Channel 2 event
#define EVTSVT_PUB_LGPT3C1       EVTSVT_CPUIRQ0SEL_PUBID_LGPT3C1       //!< Publisher ID for LGPT3 Channel 1 event
#define EVTSVT_PUB_LGPT3C0       EVTSVT_CPUIRQ0SEL_PUBID_LGPT3C0       //!< Publisher ID for LGPT3 Channel 0 event
#define EVTSVT_PUB_LGPT2_ADC     EVTSVT_CPUIRQ0SEL_PUBID_LGPT2_ADC     //!< Publisher ID for LGPT2 ADC event
#define EVTSVT_PUB_LGPT2_DMA     EVTSVT_CPUIRQ0SEL_PUBID_LGPT2_DMA     //!< Publisher ID for LGPT2 DMA event
#define EVTSVT_PUB_LGPT2_COMB    EVTSVT_CPUIRQ0SEL_PUBID_LGPT2_COMB    //!< Publisher ID for LGPT2 Combined event
#define EVTSVT_PUB_LGPT2C2       EVTSVT_CPUIRQ0SEL_PUBID_LGPT2C2       //!< Publisher ID for LGPT2 Channel 2 event
#define EVTSVT_PUB_LGPT2C1       EVTSVT_CPUIRQ0SEL_PUBID_LGPT2C1       //!< Publisher ID for LGPT2 Channel 1 event
#define EVTSVT_PUB_LGPT2C0       EVTSVT_CPUIRQ0SEL_PUBID_LGPT2C0       //!< Publisher ID for LGPT2 Channel 0 event
#define EVTSVT_PUB_UART1_COMB    EVTSVT_CPUIRQ0SEL_PUBID_UART1_COMB    //!< Publisher ID for UART1 Combined event
#define EVTSVT_PUB_LRFD_EVT2     EVTSVT_CPUIRQ0SEL_PUBID_LRFD_EVT2     //!< Publisher ID for LRFD Event 2
#define EVTSVT_PUB_LRFD_EVT1     EVTSVT_CPUIRQ0SEL_PUBID_LRFD_EVT1     //!< Publisher ID for LRFD Event 1
#define EVTSVT_PUB_LRFD_EVT0     EVTSVT_CPUIRQ0SEL_PUBID_LRFD_EVT0     //!< Publisher ID for LRFD Event 0
#define EVTSVT_PUB_LGPT1_ADC     EVTSVT_CPUIRQ0SEL_PUBID_LGPT1_ADC     //!< Publisher ID for LGPT1 ADC event
#define EVTSVT_PUB_LGPT1_DMA     EVTSVT_CPUIRQ0SEL_PUBID_LGPT1_DMA     //!< Publisher ID for LGPT1 DMA event
#define EVTSVT_PUB_LGPT1C2       EVTSVT_CPUIRQ0SEL_PUBID_LGPT1C2       //!< Publisher ID for LGPT1 Channel 2 event
#define EVTSVT_PUB_LGPT1C1       EVTSVT_CPUIRQ0SEL_PUBID_LGPT1C1       //!< Publisher ID for LGPT1 Channel 1 event
#define EVTSVT_PUB_LGPT1C0       EVTSVT_CPUIRQ0SEL_PUBID_LGPT1C0       //!< Publisher ID for LGPT1 Channel 0 event
#define EVTSVT_PUB_LGPT0_ADC     EVTSVT_CPUIRQ0SEL_PUBID_LGPT0_ADC     //!< Publisher ID for LGPT0 ADC event
#define EVTSVT_PUB_LGPT0_DMA     EVTSVT_CPUIRQ0SEL_PUBID_LGPT0_DMA     //!< Publisher ID for LGPT0 DMA event
#define EVTSVT_PUB_LGPT0C2       EVTSVT_CPUIRQ0SEL_PUBID_LGPT0C2       //!< Publisher ID for LGPT0 Channel 2 event
#define EVTSVT_PUB_LGPT0C1       EVTSVT_CPUIRQ0SEL_PUBID_LGPT0C1       //!< Publisher ID for LGPT0 Channel 1 event
#define EVTSVT_PUB_LGPT0C0       EVTSVT_CPUIRQ0SEL_PUBID_LGPT0C0       //!< Publisher ID for LGPT0 Channel 0 event
#define EVTSVT_PUB_SYSTIM4       EVTSVT_CPUIRQ0SEL_PUBID_SYSTIM4       //!< Publisher ID for System Timer 4
#define EVTSVT_PUB_SYSTIM3       EVTSVT_CPUIRQ0SEL_PUBID_SYSTIM3       //!< Publisher ID for System Timer 3
#define EVTSVT_PUB_SYSTIM2       EVTSVT_CPUIRQ0SEL_PUBID_SYSTIM2       //!< Publisher ID for System Timer 2
#define EVTSVT_PUB_SYSTIM1       EVTSVT_CPUIRQ0SEL_PUBID_SYSTIM1       //!< Publisher ID for System Timer 1
#define EVTSVT_PUB_SYSTIM0       EVTSVT_CPUIRQ0SEL_PUBID_SYSTIM0       //!< Publisher ID for System Timer 0
#define EVTSVT_PUB_SYSTIM_LT     EVTSVT_CPUIRQ0SEL_PUBID_SYSTIM_LT     //!< Publisher ID for System Timer LT
#define EVTSVT_PUB_SYSTIM_HB     EVTSVT_CPUIRQ0SEL_PUBID_SYSTIM_HB     //!< Publisher ID for System Timer HB
#define EVTSVT_PUB_I2C0_IRQ      EVTSVT_CPUIRQ0SEL_PUBID_I2C0_IRQ      //!< Publisher ID for I2C0 IRQ
#define EVTSVT_PUB_UART0_COMB    EVTSVT_CPUIRQ0SEL_PUBID_UART0_COMB    //!< Publisher ID for UART0 Combined event
#define EVTSVT_PUB_AES_COMB      EVTSVT_CPUIRQ0SEL_PUBID_AES_COMB      //!< Publisher ID for AES Combined event
#define EVTSVT_PUB_DMA_ERR       EVTSVT_CPUIRQ0SEL_PUBID_DMA_ERR       //!< Publisher ID for DMA Error
#define EVTSVT_PUB_DMA_DONE_COMB EVTSVT_CPUIRQ0SEL_PUBID_DMA_DONE_COMB //!< Publisher ID for DMA Done Combined event
#define EVTSVT_PUB_LGPT1_COMB    EVTSVT_CPUIRQ0SEL_PUBID_LGPT1_COMB    //!< Publisher ID for LGPT1 Combined event
#define EVTSVT_PUB_LGPT0_COMB    EVTSVT_CPUIRQ0SEL_PUBID_LGPT0_COMB    //!< Publisher ID for LGPT0 Combined event
#define EVTSVT_PUB_ADC_EVT       EVTSVT_CPUIRQ0SEL_PUBID_ADC_EVT       //!< Publisher ID for ADC Event
#define EVTSVT_PUB_ADC_COMB      EVTSVT_CPUIRQ0SEL_PUBID_ADC_COMB      //!< Publisher ID for ADC Combined event
#define EVTSVT_PUB_SPI0_COMB     EVTSVT_CPUIRQ0SEL_PUBID_SPI0_COMB     //!< Publisher ID for SPI0 Combined event
#define EVTSVT_PUB_LRFD_IRQ2     EVTSVT_CPUIRQ0SEL_PUBID_LRFD_IRQ2     //!< Publisher ID for LRFD IRQ2
#define EVTSVT_PUB_LRFD_IRQ1     EVTSVT_CPUIRQ0SEL_PUBID_LRFD_IRQ1     //!< Publisher ID for LRFD IRQ1
#define EVTSVT_PUB_LRFD_IRQ0     EVTSVT_CPUIRQ0SEL_PUBID_LRFD_IRQ0     //!< Publisher ID for LRFD IRQ0
#define EVTSVT_PUB_FLASH_IRQ     EVTSVT_CPUIRQ0SEL_PUBID_FLASH_IRQ     //!< Publisher ID for Flash IRQ
#define EVTSVT_PUB_GPIO_EVT      EVTSVT_CPUIRQ0SEL_PUBID_GPIO_EVT      //!< Publisher ID for GPIO Event
#define EVTSVT_PUB_GPIO_COMB     EVTSVT_CPUIRQ0SEL_PUBID_GPIO_COMB     //!< Publisher ID for GPIO Combined event
#define EVTSVT_PUB_SYSTIM_COMB   EVTSVT_CPUIRQ0SEL_PUBID_SYSTIM_COMB   //!< Publisher ID for System Timer Combined event
#define EVTSVT_PUB_AON_IOC_COMB  EVTSVT_CPUIRQ0SEL_PUBID_AON_IOC_COMB  //!< Publisher ID for AON IOC Combined event
#define EVTSVT_PUB_AON_LPMCMP_IRQ \
    EVTSVT_CPUIRQ0SEL_PUBID_AON_LPMCMP_IRQ                           //!< Publisher ID for AON Low Power Comparator IRQ
#define EVTSVT_PUB_AON_DBG_COMB EVTSVT_CPUIRQ0SEL_PUBID_AON_DBG_COMB //!< Publisher ID for AON Debug Combined event
#define EVTSVT_PUB_AON_RTC_COMB EVTSVT_CPUIRQ0SEL_PUBID_AON_RTC_COMB //!< Publisher ID for AON RTC Combined event
#define EVTSVT_PUB_AON_CKM_COMB \
    EVTSVT_CPUIRQ0SEL_PUBID_AON_CKM_COMB //!< Event ID for AON Clock Management Combined event
#define EVTSVT_PUB_AON_PMU_COMB \
    EVTSVT_CPUIRQ0SEL_PUBID_AON_PMU_COMB             //!< Event ID for AON Power Management Combined event
#define EVTSVT_PUB_NONE EVTSVT_CPUIRQ0SEL_PUBID_NONE //!< No event (always inactive)

//*****************************************************************************
//
// Helper defines used for range checking
//
//*****************************************************************************
//! Maximum allowed IPID value, used for range checking
#define EVTSVT_IPID_MAX_VAL EVTSVT_DMA_TRIG_UART1RXTRG

//! Maximum allowed PUBID value, used for range checking
#define EVTSVT_PUBID_MAX_VAL EVTSVT_PUB_SYSTIM5

//! Maximum allowed DMA channel parameter value, used for range checking.
//! \note Channel address order is not always the same as the channel number.
#define EVTSVT_DMA_CH_MAX_VAL EVTSVT_DMA_CH9
//! Minimum allowed DMA channel parameter value, used for range checking
#define EVTSVT_DMA_CH_MIN_VAL EVTSVT_DMA_CH0

//! Maximum allowed subscriber parameter value, used for range checking.
#define EVTSVT_SUB_MAX_VAL EVTSVT_SUB_I2SSTMP
//! Minimum allowed subscriber parameter value, used for range checking.
#define EVTSVT_SUB_MIN_VAL EVTSVT_SUB_CPUIRQ0

//*****************************************************************************
//
// API Functions and prototypes
//
//*****************************************************************************

#ifndef DRIVERLIB_NS
//*****************************************************************************
//
// Below functions will only be compiled into the driverlib.a library and not
// the driverlib_ns.a library.
// Non-secure applications might be able to access some/all of these functions
// through veneers declared further below in the #else case.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Configure DMA channel for the given trigger/publisher.
//!
//! This function configures the DMA channel \c channel to be triggered by the
//! event publisher \c id. DMA channels 0-7 accept \c EVTSVT_DMA_TRIG_* defines,
//! while channels 8-11 accept \c EVTSVT_PUB_* defines.
//!
//! \note Channel 0-7: Not all channels support all DMA triggers, please refer
//! to the Technical Reference Manual for details.
//!
//! \note Channel 8-11: These channels support edge detection and this function
//! always enables it.
//!
//! \param channel is the DMA channel to configure. The parameter can be ONE of
//! the following values:
//! - \ref EVTSVT_DMA_CH0
//! - \ref EVTSVT_DMA_CH1
//! - \ref EVTSVT_DMA_CH2
//! - \ref EVTSVT_DMA_CH3
//! - \ref EVTSVT_DMA_CH4
//! - \ref EVTSVT_DMA_CH5
//! - \ref EVTSVT_DMA_CH6
//! - \ref EVTSVT_DMA_CH7
//! - \ref EVTSVT_DMA_CH8
//! - \ref EVTSVT_DMA_CH9
//! - \ref EVTSVT_DMA_CH10
//! - \ref EVTSVT_DMA_CH11
//!
//! \param id is the \c EVTSVT_DMA_TRIG_* or \c EVTSVT_PUB_* define to configure
//! the DMA channel with.
//!
//! For DMA channels 0-7, \c id must be ONE of the following values:
//! - \ref EVTSVT_DMA_TRIG_UART0TXTRG
//! - \ref EVTSVT_DMA_TRIG_UART0RXTRG
//! - \ref EVTSVT_DMA_TRIG_UART1TXTRG
//! - \ref EVTSVT_DMA_TRIG_UART1RXTRG
//! - \ref EVTSVT_DMA_TRIG_SPI0TXTRG
//! - \ref EVTSVT_DMA_TRIG_SPI0RXTRG
//! - \ref EVTSVT_DMA_TRIG_SPI1TXTRG
//! - \ref EVTSVT_DMA_TRIG_SPI1RXTRG
//! - \ref EVTSVT_DMA_TRIG_LRFDTRG
//! - \ref EVTSVT_DMA_TRIG_ADC0TRG
//! - \ref EVTSVT_DMA_TRIG_LAESTRGA
//! - \ref EVTSVT_DMA_TRIG_LAESTRGB
//! - \ref EVTSVT_DMA_TRIG_CANTRGA
//! - \ref EVTSVT_DMA_TRIG_CANTRGB
//!
//! For DMA channels 8-11, \c id must be ONE of the following values:
//! - \ref EVTSVT_PUB_SYSTIM5
//! - \ref EVTSVT_PUB_GPIO_EVT1
//! - \ref EVTSVT_PUB_APU_IRQ
//! - \ref EVTSVT_PUB_SPI1_COMB
//! - \ref EVTSVT_PUB_CAN_EVT
//! - \ref EVTSVT_PUB_CAN_IRQ
//! - \ref EVTSVT_PUB_I2S_IRQ
//! - \ref EVTSVT_PUB_LGPT3_ADC
//! - \ref EVTSVT_PUB_LGPT3_DMA
//! - \ref EVTSVT_PUB_LGPT3_COMB
//! - \ref EVTSVT_PUB_LGPT3C2
//! - \ref EVTSVT_PUB_LGPT3C1
//! - \ref EVTSVT_PUB_LGPT3C0
//! - \ref EVTSVT_PUB_LGPT2_ADC
//! - \ref EVTSVT_PUB_LGPT2_DMA
//! - \ref EVTSVT_PUB_LGPT2_COMB
//! - \ref EVTSVT_PUB_LGPT2C2
//! - \ref EVTSVT_PUB_LGPT2C1
//! - \ref EVTSVT_PUB_LGPT2C0
//! - \ref EVTSVT_PUB_UART1_COMB
//! - \ref EVTSVT_PUB_LRFD_EVT2
//! - \ref EVTSVT_PUB_LRFD_EVT1
//! - \ref EVTSVT_PUB_LRFD_EVT0
//! - \ref EVTSVT_PUB_LGPT1_ADC
//! - \ref EVTSVT_PUB_LGPT1_DMA
//! - \ref EVTSVT_PUB_LGPT1C2
//! - \ref EVTSVT_PUB_LGPT1C1
//! - \ref EVTSVT_PUB_LGPT1C0
//! - \ref EVTSVT_PUB_LGPT0_ADC
//! - \ref EVTSVT_PUB_LGPT0_DMA
//! - \ref EVTSVT_PUB_LGPT0C2
//! - \ref EVTSVT_PUB_LGPT0C1
//! - \ref EVTSVT_PUB_LGPT0C0
//! - \ref EVTSVT_PUB_SYSTIM4
//! - \ref EVTSVT_PUB_SYSTIM3
//! - \ref EVTSVT_PUB_SYSTIM2
//! - \ref EVTSVT_PUB_SYSTIM1
//! - \ref EVTSVT_PUB_SYSTIM0
//! - \ref EVTSVT_PUB_SYSTIM_LT
//! - \ref EVTSVT_PUB_SYSTIM_HB
//! - \ref EVTSVT_PUB_I2C0_IRQ
//! - \ref EVTSVT_PUB_UART0_COMB
//! - \ref EVTSVT_PUB_AES_COMB
//! - \ref EVTSVT_PUB_DMA_ERR
//! - \ref EVTSVT_PUB_DMA_DONE_COMB
//! - \ref EVTSVT_PUB_LGPT1_COMB
//! - \ref EVTSVT_PUB_LGPT0_COMB
//! - \ref EVTSVT_PUB_ADC_EVT
//! - \ref EVTSVT_PUB_ADC_COMB
//! - \ref EVTSVT_PUB_SPI0_COMB
//! - \ref EVTSVT_PUB_LRFD_IRQ2
//! - \ref EVTSVT_PUB_LRFD_IRQ1
//! - \ref EVTSVT_PUB_LRFD_IRQ0
//! - \ref EVTSVT_PUB_FLASH_IRQ
//! - \ref EVTSVT_PUB_GPIO_EVT
//! - \ref EVTSVT_PUB_GPIO_COMB
//! - \ref EVTSVT_PUB_SYSTIM_COMB
//! - \ref EVTSVT_PUB_AON_IOC_COMB
//! - \ref EVTSVT_PUB_AON_LPMCMP_IRQ
//! - \ref EVTSVT_PUB_AON_DBG_COMB
//! - \ref EVTSVT_PUB_AON_RTC_COMB
//! - \ref EVTSVT_PUB_AON_CKM_COMB
//! - \ref EVTSVT_PUB_AON_PMU_COMB
//! - \ref EVTSVT_PUB_NONE
//
//*****************************************************************************
extern void EVTSVTConfigureDma(uint32_t channel, uint32_t id);

//*****************************************************************************
//
//! \brief Configure event publisher to be input to a subscriber.
//!
//! This function configures the \c pubId event publisher to be input of
//! \c subscriber.
//!
//! \note Not all subscribers can select all publishers, please refer to the
//!   Technical Reference Manual for details.
//!
//! \param subscriber is the subscriber ID. The parameter must be ONE of the
//! following values:
//! - \ref EVTSVT_SUB_CPUIRQ0
//! - \ref EVTSVT_SUB_CPUIRQ1
//! - \ref EVTSVT_SUB_CPUIRQ2
//! - \ref EVTSVT_SUB_CPUIRQ3
//! - \ref EVTSVT_SUB_CPUIRQ4
//! - \ref EVTSVT_SUB_CPUIRQ16
//! - \ref EVTSVT_SUB_CPUIRQ17
//! - \ref EVTSVT_SUB_SYSTIMC1
//! - \ref EVTSVT_SUB_SYSTIMC5
//! - \ref EVTSVT_SUB_ADCTRG
//! - \ref EVTSVT_SUB_LGPTSYNC
//! - \ref EVTSVT_SUB_LGPT0IN0
//! - \ref EVTSVT_SUB_LGPT0IN1
//! - \ref EVTSVT_SUB_LGPT0IN2
//! - \ref EVTSVT_SUB_LGPT0TEN
//! - \ref EVTSVT_SUB_LGPT1IN0
//! - \ref EVTSVT_SUB_LGPT1IN1
//! - \ref EVTSVT_SUB_LGPT1IN2
//! - \ref EVTSVT_SUB_LGPT1TEN
//! - \ref EVTSVT_SUB_LGPT2IN0
//! - \ref EVTSVT_SUB_LGPT2IN1
//! - \ref EVTSVT_SUB_LGPT2IN2
//! - \ref EVTSVT_SUB_LGPT2TEN
//! - \ref EVTSVT_SUB_LGPT3IN0
//! - \ref EVTSVT_SUB_LGPT3IN1
//! - \ref EVTSVT_SUB_LGPT3IN2
//! - \ref EVTSVT_SUB_LGPT3TEN
//! - \ref EVTSVT_SUB_I2SSTMP
//!
//! \param pubId is the publisher ID. The parameter must be ONE of the following
//! values:
//! - \ref EVTSVT_PUB_SYSTIM5
//! - \ref EVTSVT_PUB_GPIO_EVT1
//! - \ref EVTSVT_PUB_APU_IRQ
//! - \ref EVTSVT_PUB_SPI1_COMB
//! - \ref EVTSVT_PUB_CAN_EVT
//! - \ref EVTSVT_PUB_CAN_IRQ
//! - \ref EVTSVT_PUB_I2S_IRQ
//! - \ref EVTSVT_PUB_LGPT3_ADC
//! - \ref EVTSVT_PUB_LGPT3_DMA
//! - \ref EVTSVT_PUB_LGPT3_COMB
//! - \ref EVTSVT_PUB_LGPT3C2
//! - \ref EVTSVT_PUB_LGPT3C1
//! - \ref EVTSVT_PUB_LGPT3C0
//! - \ref EVTSVT_PUB_LGPT2_ADC
//! - \ref EVTSVT_PUB_LGPT2_DMA
//! - \ref EVTSVT_PUB_LGPT2_COMB
//! - \ref EVTSVT_PUB_LGPT2C2
//! - \ref EVTSVT_PUB_LGPT2C1
//! - \ref EVTSVT_PUB_LGPT2C0
//! - \ref EVTSVT_PUB_UART1_COMB
//! - \ref EVTSVT_PUB_LRFD_EVT2
//! - \ref EVTSVT_PUB_LRFD_EVT1
//! - \ref EVTSVT_PUB_LRFD_EVT0
//! - \ref EVTSVT_PUB_LGPT1_ADC
//! - \ref EVTSVT_PUB_LGPT1_DMA
//! - \ref EVTSVT_PUB_LGPT1C2
//! - \ref EVTSVT_PUB_LGPT1C1
//! - \ref EVTSVT_PUB_LGPT1C0
//! - \ref EVTSVT_PUB_LGPT0_ADC
//! - \ref EVTSVT_PUB_LGPT0_DMA
//! - \ref EVTSVT_PUB_LGPT0C2
//! - \ref EVTSVT_PUB_LGPT0C1
//! - \ref EVTSVT_PUB_LGPT0C0
//! - \ref EVTSVT_PUB_SYSTIM4
//! - \ref EVTSVT_PUB_SYSTIM3
//! - \ref EVTSVT_PUB_SYSTIM2
//! - \ref EVTSVT_PUB_SYSTIM1
//! - \ref EVTSVT_PUB_SYSTIM0
//! - \ref EVTSVT_PUB_SYSTIM_LT
//! - \ref EVTSVT_PUB_SYSTIM_HB
//! - \ref EVTSVT_PUB_I2C0_IRQ
//! - \ref EVTSVT_PUB_UART0_COMB
//! - \ref EVTSVT_PUB_AES_COMB
//! - \ref EVTSVT_PUB_DMA_ERR
//! - \ref EVTSVT_PUB_DMA_DONE_COMB
//! - \ref EVTSVT_PUB_LGPT1_COMB
//! - \ref EVTSVT_PUB_LGPT0_COMB
//! - \ref EVTSVT_PUB_ADC_EVT
//! - \ref EVTSVT_PUB_ADC_COMB
//! - \ref EVTSVT_PUB_SPI0_COMB
//! - \ref EVTSVT_PUB_LRFD_IRQ2
//! - \ref EVTSVT_PUB_LRFD_IRQ1
//! - \ref EVTSVT_PUB_LRFD_IRQ0
//! - \ref EVTSVT_PUB_FLASH_IRQ
//! - \ref EVTSVT_PUB_GPIO_EVT
//! - \ref EVTSVT_PUB_GPIO_COMB
//! - \ref EVTSVT_PUB_SYSTIM_COMB
//! - \ref EVTSVT_PUB_AON_IOC_COMB
//! - \ref EVTSVT_PUB_AON_LPMCMP_IRQ
//! - \ref EVTSVT_PUB_AON_DBG_COMB
//! - \ref EVTSVT_PUB_AON_RTC_COMB
//! - \ref EVTSVT_PUB_AON_CKM_COMB
//! - \ref EVTSVT_PUB_AON_PMU_COMB
//! - \ref EVTSVT_PUB_NONE
//
//*****************************************************************************
extern void EVTSVTConfigureEvent(uint32_t subscriber, uint32_t pubId);

#else
//*****************************************************************************
//
// Secure-only API functions available from non-secure context through
// veneers.
//
// These functions must be linked in from from the secure veneer library.
//
//*****************************************************************************

extern void EVTSVTConfigureDma_veneer(uint32_t channel, uint32_t id);
    #define EVTSVTConfigureDma   EVTSVTConfigureDma_veneer

extern void EVTSVTConfigureEvent_veneer(uint32_t subscriber, uint32_t pubId);
    #define EVTSVTConfigureEvent EVTSVTConfigureEvent_veneer
#endif

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

#endif //  __EVTSVT_H__
