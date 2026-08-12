/*
 * Copyright (c) 2024-2025, Texas Instruments Incorporated
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
 *  @file       PowerCC27XX_veneers.h
 *
 *  @brief      Secure Power API functions available from non-secure context
 *              through veneers.
 *
 *  @warning These APIs are only to be used by the NS Power driver, and should
 *  not be used by the application directly.
 *
 *  ============================================================================
 */

#ifndef ti_drivers_power_PowerCC27XX_veneers_
#define ti_drivers_power_PowerCC27XX_veneers_

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC27XX.h>
#include <ti/drivers/tfm/SecureCallback.h>

int_fast16_t Power_getDependencyCount_veneer(Power_Resource resourceId);

PowerLPF3_ResetReason PowerLPF3_getResetReason_veneer(void);

int_fast16_t PowerCC27XX_setNonSecureConstraintPointers_veneer(
    volatile const uint32_t *constraintMask,
    volatile const uint8_t (*const constraintCounts)[PowerCC27XX_NUMCONSTRAINTS]);

int_fast16_t Power_setDependency_veneer(Power_Resource resourceId);

int_fast16_t Power_releaseDependency_veneer(Power_Resource resourceId);

int_fast16_t Power_shutdown_veneer(uint_fast16_t shutdownState, uint_fast32_t shutdownTime);

void PowerLPF3_releaseLatches_veneer(void);

void Power_reset_veneer(void);

void PowerCC27XX_doWFI_veneer(void);

int_fast16_t PowerLPF3_startAFOSC_veneer(PowerLPF3_AfoscFreq frequency);

void PowerLPF3_stopAFOSC_veneer(void);

uint_fast16_t PowerCC27XX_getCurrentEventsMask_veneer(void);

/*! Veneer to inform the secure side about the non-secure side's SecureCallback handle.
 * Panics if the handle is invalid.
 */
void PowerCC27XX_setNonSecureNotificationHandle_veneer(SecureCallback_Handle handle);

int_fast16_t PowerLPF3_sleep_veneer(uint32_t nextEventTimeUs);

#endif /* ti_drivers_power_PowerCC27XX_veneers_ */