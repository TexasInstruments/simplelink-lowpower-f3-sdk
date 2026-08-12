/******************************************************************************
*  Filename:       cc27xx.h
*
*  Description:    Collection of architecture definitions for CC27xx devices
*
// ##### LICENSE HEADER #####
*
******************************************************************************/
#ifndef __CC27XX_H__
#define __CC27XX_H__

/* IRQ numbers */
typedef enum IRQn
{
    NonMaskableInt_IRQn   = -14, /* 2  Non Maskable Interrupt */
    HardFault_IRQn        = -13, /* 3  Hard Fault Interrupt */
    MemoryManagement_IRQn = -12, /* 4  Memory Management Interrupt */
    BusFault_IRQn         = -11, /* 5  Bus Fault Interrupt */
    UsageFault_IRQn       = -10, /* 6  Usage Fault Interrupt */
    SecureFault_IRQn      = -9,  /* 7  Secure Fault Interrupt */
    SVCall_IRQn           = -5,  /* 11 SV Call Interrupt */
    DebugMonitor_IRQn     = -4,  /* 12 Debug Monitor Interrupt */
    PendSV_IRQn           = -2,  /* 14 Pend SV Interrupt */
    SysTick_IRQn          = -1,  /* 15 System Tick Interrupt */
    CPUIRQ0_IRQn          = 0,   /* 16 IRQ0: Configurable source controlled by
                                       EVTSVT.CPUIRQ0SEL */
    CPUIRQ1_IRQn          = 1,   /* 17 IRQ1: Configurable source controlled by
                                       EVTSVT.CPUIRQ1SEL */
    CPUIRQ2_IRQn          = 2,   /* 18 IRQ2: Configurable source controlled by
                                       EVTSVT.CPUIRQ2SEL */
    CPUIRQ3_IRQn          = 3,   /* 19 IRQ3: Configurable source controlled by
                                       EVTSVT.CPUIRQ3SEL */
    CPUIRQ4_IRQn          = 4,   /* 20 IRQ4: Configurable source controlled by
                                       EVTSVT.CPUIRQ4SEL */
    GPIO_COMB_IRQn        = 5,   /* 21 GPIO combined wake up interrupt,
                                       interrupt flags can be found here
                                       GPIO:MIS */
    LRFD_IRQ0_IRQn        = 6,   /* 22 LRFD combined event, interrupt flags can
                                       be found here LRFDDBELL:MIS0 */
    LRFD_IRQ1_IRQn        = 7,   /* 23 LRFD combined event, interrupt flags can
                                       be found here LRFDDBELL:MIS1 */
    DMA_DONE_COMB_IRQn    = 8,   /* 24 DMA combined done interrupt,
                                       corresponding flags can be found here
                                       DMA:REQDONE */
    AES_COMB_IRQn         = 9,   /* 25 AES accelerator combined interrupt
                                       request, interrupt flags can be found
                                       here AES:MIS */
    SPI0_COMB_IRQn        = 10,  /* 26 SPI0 combined interrupt request,
                                       interrupt flags can be found here
                                       SPI0:MIS */
    UART0_COMB_IRQn       = 11,  /* 27 UART0 combined interrupt, interrupt flags
                                       are found here UART0:MIS */
    I2C0_IRQ_IRQn         = 12,  /* 28 Interrupt event from I2C0, interrupt
                                       flags can be found here I2C0:MIS */
    LGPT0_COMB_IRQn       = 13,  /* 29 LGPT0 combined interrupt, interrupt flags
                                       are found here LGPT0:MIS */
    LGPT1_COMB_IRQn       = 14,  /* 30 LGPT1 combined interrupt, interrupt flags
                                       are found here LGPT1:MIS */
    ADC_COMB_IRQn         = 15,  /* 31 ADC combined interrupt request, interrupt
                                       flags can be found here ADC:MIS0 */
    CPUIRQ16_IRQn         = 16,  /* 32 IRQ16: Configurable source controlled by
                                       EVTSVT.CPUIRQ16SEL */
    CPUIRQ17_IRQn         = 17,  /* 33 IRQ17: Configurable source controlled by
                                       EVTSVT.CPUIRQ17SEL */
    LGPT2_COMB_IRQn       = 18,  /* 34 LGPT2 combined interrupt, interrupt flags
                                       are found here LGPT2:MIS */
    LGPT3_COMB_IRQn       = 19,  /* 35 LGPT3 combined interrupt, interrupt flags
                                       are found here LGPT3:MIS */
    I2S_IRQ_IRQn          = 20,  /* 36 I2S interrupt event, controlled by
                                       I2S:IRQMASK */
    CAN_IRQ_IRQn          = 21,  /* 37 MCAN interrupt event, interrupt flags can
                                       be found here MCAN:MIS0 */
    UART1_COMB_IRQn       = 22,  /* 38 UART1 combined interrupt, interrupt flags
                                       are found here UART1:MIS */
    SPI1_COMB_IRQn        = 23,  /* 39 SPI1 combined interrupt request,
                                       interrupt flags can be found here
                                       SPI1:MIS */
    APU_IRQ_IRQn          = 24,  /* 40 APU IRQ */
    HSM_SEC_IRQ_IRQn      = 25,  /* 41 HSM Secure IRQ */
    HSM_NONSEC_IRQ_IRQn   = 26,  /* 42 HSM Non-secure IRQ */
    HSM_OTP_IRQ_IRQn      = 27,  /* 43 HSM OTP IRQ */
    AON_PMU_COMB_IRQn     = 28,  /* 44 PMU combined interrupt request for
                                       BATMON, interrupt flags can be found here
                                       PMUD:EVENT */
    AON_CKM_COMB_IRQn     = 29,  /* 45 CKMD combined interrupt request,
                                       interrupt flags can be found here
                                       CKMD:MIS */
    AON_RTC_COMB_IRQn     = 30,  /* 46 AON_RTC event, controlled by the
                                       RTC:IMASK setting */
    AON_LPCMP_IRQ_IRQn    = 31,  /* 47 AON LPCMP interrupt, controlled by
                                       SYS0:LPCMPCFG */
    AON_IOC_COMB_IRQn     = 32,  /* 48 IOC synchronous combined event,
                                       controlled by IOC:EVTCFG */
    SW0_IRQn              = 33,  /* 49 Software Triggered Interrupt 0 */
    SW1_IRQn              = 34,  /* 50 Software Triggered Interrupt 1 */
} IRQn_Type;

/* Architecture-specific constants */
#define __SAUREGION_PRESENT    0x0001U /* SAU present */
#define __DSP_PRESENT          0x0001U /* DSP extension present */
#define __Vendor_SysTickConfig 0x0000U /* Set to 1 if different SysTick Config is used */
#define __CM33_REV             0x0001U /* Core revision */
#define __MPU_PRESENT          0x0001U /* MPU present or not */
#define __FPU_PRESENT          0x0001U /* FPU present or not */
#define __VTOR_PRESENT         0x0001U /* VTOR present */
#define __NVIC_PRIO_BITS       0x0004U /* 4 NVIC priority bits */

#endif // #ifndef __CC27XX_H__
