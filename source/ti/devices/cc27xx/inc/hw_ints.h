/******************************************************************************
*  Filename:       hw_ints_h
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

#ifndef __HW_INTS_H__
#define __HW_INTS_H__

//*****************************************************************************
//
// The following are defines for the interrupt assignments.
//
//*****************************************************************************
#define INT_NMI_FAULT            2 // NMI Fault
#define INT_HARD_FAULT           3 // Hard Fault
#define INT_MEMMANAGE_FAULT      4 // Memory Management (MemManage) Fault
#define INT_BUS_FAULT            5 // Bus Fault
#define INT_USAGE_FAULT          6 // Usage Fault
#define INT_SVCALL              11 // Supervisor Call (SVCall)
#define INT_DEBUG               12 // Debug Monitor
#define INT_PENDSV              14 // Pending Service Call (PendSV)
#define INT_SYSTICK             15 // SysTick Interrupt from the System Timer
                                   // in NVIC.
#define INT_CPUIRQ0             16 // IRQ0: Configurable source controlled by
                                   // EVTSVT.CPUIRQ0SEL
#define INT_CPUIRQ1             17 // IRQ1: Configurable source controlled by
                                   // EVTSVT.CPUIRQ1SEL
#define INT_CPUIRQ2             18 // IRQ2: Configurable source controlled by
                                   // EVTSVT.CPUIRQ2SEL
#define INT_CPUIRQ3             19 // IRQ3: Configurable source controlled by
                                   // EVTSVT.CPUIRQ3SEL
#define INT_CPUIRQ4             20 // IRQ4: Configurable source controlled by
                                   // EVTSVT.CPUIRQ4SEL
#define INT_GPIO_COMB           21 // GPIO combined wake up interrupt,
                                   // interrupt flags can be found here
                                   // GPIO:MIS
#define INT_LRFD_IRQ0           22 // LRFD combined event, interrupt flags can
                                   // be found here LRFDDBELL:MIS0
#define INT_LRFD_IRQ1           23 // LRFD combined event, interrupt flags can
                                   // be found here LRFDDBELL:MIS1
#define INT_DMA_DONE_COMB       24 // DMA combined done interrupt,
                                   // corresponding flags can be found here
                                   // DMA:REQDONE
#define INT_AES_COMB            25 // AES accelerator combined interrupt
                                   // request, interrupt flags can be found
                                   // here AES:MIS
#define INT_SPI0_COMB           26 // SPI0 combined interrupt request,
                                   // interrupt flags can be found here
                                   // SPI0:MIS
#define INT_UART0_COMB          27 // UART0 combined interrupt, interrupt
                                   // flags are found here UART0:MIS
#define INT_I2C0_IRQ            28 // Interrupt event from I2C0, interrupt
                                   // flags can be found here I2C0:MIS
#define INT_LGPT0_COMB          29 // LGPT0 combined interrupt, interrupt
                                   // flags are found here LGPT0:MIS
#define INT_LGPT1_COMB          30 // LGPT1 combined interrupt, interrupt
                                   // flags are found here LGPT1:MIS
#define INT_ADC_COMB            31 // ADC combined interrupt request,
                                   // interrupt flags can be found here
                                   // ADC:MIS0
#define INT_CPUIRQ16            32 // IRQ16: Configurable source controlled by
                                   // EVTSVT.CPUIRQ16SEL
#define INT_CPUIRQ17            33 // IRQ17: Configurable source controlled by
                                   // EVTSVT.CPUIRQ17SEL
#define INT_LGPT2_COMB          34 // LGPT2 combined interrupt, interrupt
                                   // flags are found here LGPT2:MIS
#define INT_LGPT3_COMB          35 // LGPT3 combined interrupt, interrupt
                                   // flags are found here LGPT3:MIS
#define INT_I2S_IRQ             36 // I2S interrupt event, controlled by
                                   // I2S:IRQMASK
#define INT_CAN_IRQ             37 // MCAN interrupt event, interrupt flags
                                   // can be found here MCAN:MIS0
#define INT_UART1_COMB          38 // UART1 combined interrupt, interrupt
                                   // flags are found here UART1:MIS
#define INT_SPI1_COMB           39 // SPI1 combined interrupt request,
                                   // interrupt flags can be found here
                                   // SPI1:MIS
#define INT_VCE_IRQ             40 // VCE IRQ
#define INT_HSM_SEC_IRQ         41 // HSM Secure IRQ
#define INT_HSM_NONSEC_IRQ      42 // HSM Non-secure IRQ
#define INT_HSM_OTP_IRQ         43 // HSM OTP IRQ
#define INT_AON_PMU_COMB        44 // PMU combined interrupt request for
                                   // BATMON, interrupt flags can be found
                                   // here PMUD:EVENT
#define INT_AON_CKM_COMB        45 // CKMD combined interrupt request,
                                   // interrupt flags can be found here
                                   // CKMD:MIS
#define INT_AON_RTC_COMB        46 // AON_RTC event, controlled by the
                                   // RTC:IMASK setting
#define INT_AON_LPMCMP_IRQ      47 // AON LPCMP interrupt, controlled by
                                   // SYS0:LPCMPCFG
#define INT_AON_IOC_COMB        48 // IOC synchronous combined event,
                                   // controlled by IOC:EVTCFG
#define INT_SW0                 49 // Software Triggered Interrupt 0
#define INT_SW1                 50 // Software Triggered Interrupt 1

//*****************************************************************************
//
// The following are defines for number of interrupts and priority levels.
//
//*****************************************************************************
#define INT_PRI_LEVEL0          0x00000000 // Priority level 0 value
#define INT_PRI_LEVEL1          0x00000010 // Priority level 1 value
#define INT_PRI_LEVEL10         0x000000A0 // Priority level 2 value
#define INT_PRI_LEVEL11         0x000000B0 // Priority level 3 value
#define INT_PRI_LEVEL12         0x000000C0 // Priority level 4 value
#define INT_PRI_LEVEL13         0x000000D0 // Priority level 5 value
#define INT_PRI_LEVEL14         0x000000E0 // Priority level 6 value
#define INT_PRI_LEVEL15         0x000000F0 // Priority level 7 value
#define INT_PRI_LEVEL2          0x00000020 // Priority level 2 value
#define INT_PRI_LEVEL3          0x00000030 // Priority level 3 value
#define INT_PRI_LEVEL4          0x00000040 // Priority level 4 value
#define INT_PRI_LEVEL5          0x00000050 // Priority level 5 value
#define INT_PRI_LEVEL6          0x00000060 // Priority level 6 value
#define INT_PRI_LEVEL7          0x00000070 // Priority level 7 value
#define INT_PRI_LEVEL8          0x00000080 // Priority level 0 value
#define INT_PRI_LEVEL9          0x00000090 // Priority level 1 value
#define INT_PRIORITY_MASK       0x000000F0 // Priority level mask
#define NUM_INTERRUPTS          51 // Number of exceptions
#define NUM_PRIORITY            16 // Number of exception priority levels
#define NUM_PRIORITY_BITS        4 // Number of exception priority bits

#endif // __HW_INTS__
