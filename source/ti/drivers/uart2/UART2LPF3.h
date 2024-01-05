/*
 * Copyright (c) 2021-2023, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       UART2LPF3.h
 *
 *  @brief      UART driver implementation for a Low Power F3 device UART
 *              controller
 *
 *  The UART header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/UART2.h> #include <ti/drivers/uart2/UART2LPF3.h>
 *  @endcode
 *
 *  Refer to @ref UART2.h for a complete description of APIs and examples of
 *  use.
 *
 *  ============================================================================
 */

#ifndef ti_drivers_uart2_UART2LPF3__include
#define ti_drivers_uart2_UART2LPF3__include

#include <stddef.h>
#include <stdint.h>

#include <stdint.h>
#include <stdbool.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(driverlib/udma.h)

#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/UART2.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! Size of the TX and RX FIFOs is 8 items */
#define UART2LPF3_FIFO_SIZE 8

/*!
 *  @brief    UART TXD/RXD encoding/decoding scheme
 *
 *  Encoding scheme for data transmitted/received on TXD/RXD pins.
 *  The data can either be encoded as standard UART, or as an IrDA-compliant
 *  Serial Infrared stream, where the width of each pulse is 3/16th of the bit period.
 *  A low-power SIR encoding is also available, where the pulse width can be configured.
 */
typedef enum
{
    UART2LPF3_CODING_UART   = 0, /*!< TXD/RXD coded as UART */
    UART2LPF3_CODING_SIR    = 1, /*!< TXD/RXD coded as Serial Infrared */
    UART2LPF3_CODING_SIR_LP = 2, /*!< TXD/RXD coded as Low-Power Serial Infrared */
} UART2LPF3_CodingScheme;

/*!
 *  @brief      UART2LPF3 Hardware attributes
 *
 *  The fields, baseAddr and intNum are used by driverlib
 *  APIs and therefore must be populated by
 *  driverlib macro definitions. These definitions are found under the
 *  device family in:
 *      - inc/hw_memmap.h
 *      - inc/hw_ints.h
 *      - driverlib/uart.h
 *
 *  intPriority is the UART peripheral's interrupt priority, as defined by the
 *  underlying OS.  It is passed unmodified to the underlying OS's interrupt
 *  handler creation code, so you need to refer to the OS documentation
 *  for usage.  For example, for SYS/BIOS applications, refer to the
 *  ti.sysbios.family.arm.m3.Hwi documentation for SYS/BIOS usage of
 *  interrupt priorities.  If the driver uses the ti.dpl interface
 *  instead of making OS calls directly, then the HwiP port handles the
 *  interrupt priority in an OS specific way.  In the case of the SYS/BIOS
 *  port, intPriority is passed unmodified to Hwi_create().
 *
 *  The Low Power F3 devices use either three or two priority bits,
 *  depending on the device. That means ~0 has the same effect as (7 << 5)
 *  or (3 << 6), respectively.
 *
 *  Setting the priority to 0 is not supported by this driver.  HWI's with
 *  priority 0 ignore the HWI dispatcher to support zero-latency interrupts,
 *  thus invalidating the critical sections in this driver.
 *
 *  A sample structure is shown below:
 *  @code
 *  const UART2LPF3_HWAttrs UART2LPF3_hwAttrs[] = {
 *      {
 *           .baseAddr           = UART0_BASE,
 *           .intNum             = INT_UART0_COMB,
 *           .intPriority        = (~0),
 *           .rxPin              = CONFIG_GPIO_UART2_0_RX,
 *           .txPin              = CONFIG_GPIO_UART2_0_TX,
 *           .ctsPin             = GPIO_INVALID_INDEX,
 *           .rtsPin             = GPIO_INVALID_INDEX,
 *           .flowControl        = UART2_FLOWCTRL_NONE,
 *           .txPinMux           = GPIO_MUX_PORTCFG_PFUNC3,
 *           .rxPinMux           = GPIO_MUX_PORTCFG_PFUNC3,
 *           .ctsPinMux          = GPIO_MUX_GPIO_INTERNAL,
 *           .rtsPinMux          = GPIO_MUX_GPIO_INTERNAL,
 *           .rxChannelMask      = UDMA_CHANNEL_0_M,
 *           .txChannelMask      = UDMA_CHANNEL_1_M,
 *           .rxChannelEvtMux    = EVTSVT_DMACH0SEL_IPID_UART0RXTRG,
 *           .txChannelEvtMux    = EVTSVT_DMACH1SEL_IPID_UART0TXTRG,
 *           .codingScheme       = UART2LPF3_CODING_UART,
 *           .concatenateFIFO    = false,
 *      },
 *  };
 *  @endcode
 *
 *  To enable flow control, the .ctsPin and/or .rtsPin must be assigned.
 *  In addition, .flowControl must be set to UART2_FLOWCTRL_HARDWARE.
 */

typedef struct
{
    UART2_BASE_HWATTRS
    volatile uDMAControlTableEntry *dmaTxTableEntryPri; /*!< uDMA controlTable primary TX entry */
    volatile uDMAControlTableEntry *dmaRxTableEntryPri; /*!< uDMA controlTable primary RX entry */
    uint32_t txChannelMask;                             /*!< Mask for uDMA TX channel number (1 << channel number) */
    uint32_t rxChannelMask;                             /*!< Mask for uDMA RX channel number (1 << channel number) */
    int32_t txPinMux;                                   /*!< TX PIN mux value */
    int32_t rxPinMux;                                   /*!< RX PIN mux value */
    int32_t ctsPinMux;                                  /*!< CTS PIN mux value for flow control */
    int32_t rtsPinMux;                                  /*!< RTS PIN mux value for flow control */
    PowerLPF3_Resource powerID;                         /*!< Power driver ID for this UART instance */
    UART2LPF3_CodingScheme codingScheme;                /*!< UART TXD/RXD encoding/decoding scheme */
    uint8_t irLPClkDivider;                             /*!< Clock divider for IR Low-Power mode */
    uint8_t txChannelEvtMux;                            /*!< DMA Mux ID for this UART TX channel */
    uint8_t rxChannelEvtMux;                            /*!< DMA Mux ID for this UART RX channel */
    bool concatenateFIFO;                               /*!< Concatenate RX and TX FIFO if write-only */
} UART2LPF3_HWAttrs;

/*!
 *  @brief      UART2LPF3 Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    UART2_BASE_OBJECT
    Power_NotifyObj preNotify;  /*!< For configuring IO pins before entering standby */
    Power_NotifyObj postNotify; /*!< For configuring IO pins after returning from standby */
} UART2LPF3_Object, *UART2LPF3_Handle;

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_uart2_UART2LPF3__include */
