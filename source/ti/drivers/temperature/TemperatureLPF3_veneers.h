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
/** ============================================================================
 *  @file       TemperatureLPF3_veneers.h
 *
 *  @brief      Secure Temperature API functions available from non-secure
 *              context through veneers.
 *
 *  @warning These APIs are only to be used by the NS Temperature driver, and
 *  should not be used by the application directly.
 *
 *  ============================================================================
 */

#ifndef ti_drivers_temperature_TemperatureLPF3_veneers__include
#define ti_drivers_temperature_TemperatureLPF3_veneers__include

#include <ti/drivers/tfm/SecureCallback.h>

/*! Veneer to inform the secure side about the non-secure side's SecureCallback handle.
 * Panics if the handle is invalid.
 */
void TemperatureLPF3_setSecureCallbackHandle_veneer(SecureCallback_Handle handle);

int16_t TemperatureLPF3_getTemperature_veneer(void);

void TemperatureLPF3_setNsThresholds_veneer(int16_t thresholdHigh, int16_t thresholdLow);

void TemperatureLPF3_enableTSDMonitoring_veneer(int16_t shutdownThreshold);

void TemperatureLPF3_disableTSDMonitoring_veneer(void);

void TemperatureLPF3_triggerThermalShutdown_veneer(void);

#endif /* ti_drivers_temperature_TemperatureLPF3_veneers__include */
