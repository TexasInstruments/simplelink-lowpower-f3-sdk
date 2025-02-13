/*
 * Copyright (c) 2022-2024, Texas Instruments Incorporated
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

#ifndef ti_drivers_RCL_Profiling_h__include
#define ti_drivers_RCL_Profiling_h__include

typedef enum RCL_ProfilingEvent_e {
    RCL_ProfilingEvent_PreprocStart = 1,   /*!< Radio operation preprocessing has started */
    RCL_ProfilingEvent_PreprocStop,        /*!< Radio operation preprocessing has finalized */
    RCL_ProfilingEvent_PostprocStart,      /*!< Radio operation postprocessing has started */
    RCL_ProfilingEvent_PostprocStop,       /*!< Radio operation postprocessing has finalized */
    RCL_ProfilingEvent_CommitPktStart,     /*!< LRF has notified RCL of received packet with LRF event rxOk */
    RCL_ProfilingEvent_CommitPktEnd,       /*!< RCL committed packet to multibuffer and notified Stack with event rxEntryAvailable */
    RCL_ProfilingEvent_ProcessAuxPtrStart, /*!< ADV_EXT_IND received and committed to multibuffer */
    RCL_ProfilingEvent_ProcessAuxPtrEnd,   /*!< AuxPtr has been processed and a new radio operation has been scheduled on a secondary channel*/
    RCL_ProfilingEvent_PhySwitchStart,     /*!< Phy switch has been requested by command handler */
    RCL_ProfilingEvent_PhySwitchEnd,       /*!< Phy switch has succeeded */
} RCL_ProfilingEvent;

extern void __attribute__((weak)) RCL_Profiling_eventHook(RCL_ProfilingEvent event);

#endif /* ti_drivers_RCL_Profiling_h__include */
