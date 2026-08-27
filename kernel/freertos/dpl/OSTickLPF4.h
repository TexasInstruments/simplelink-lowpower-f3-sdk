/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
 *  ======== OSTickLPF4.h ========
 */

#ifndef kernel_freertos_dpl_OSTickLPF4__include
#define kernel_freertos_dpl_OSTickLPF4__include

#include <ti/devices/DeviceFamily.h>

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC283X)
    /*! The event subscriber for the OS Tick SysTimer channel. This should match
     *  #OSTickLPF4_TICK_INT.
     */
    #define OSTickLPF4_TICK_EVTSVT_SUB (EVTSVT_SUB_CPUIRQ6)

    /*! The interrupt number for the OS Tick SysTimer channel. This should match
     *  #OSTickLPF4_TICK_EVTSVT_SUB.
     */
    #define OSTickLPF4_TICK_INT (INT_CPUIRQ6)

    /*! The event publisher for the OS Tick SysTimer channel. This should match
     *  #OSTickLPF4_TICK_SYSTIM_CHANNEL.
     */
    #define OSTickLPF4_TICK_EVTSVT_PUB (EVTSVT_PUB_SYSTIM0)

    /*! The OS Tick SysTimer channel number. This should match
     *  #OSTickLPF4_TICK_EVTSVT_PUB.
     */
    #define OSTickLPF4_TICK_SYSTIM_CHANNEL (0U)
#else
    #error "Unsupported Device Family"
#endif

#endif /* kernel_freertos_dpl_OSTickLPF4__include */