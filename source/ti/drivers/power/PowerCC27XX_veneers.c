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
/*
 *  ======== PowerCC27XX_veneers.c ========
 */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC27XX.h>
#include <ti/drivers/power/PowerCC27XX_s.h>
#include <ti/drivers/power/PowerCC27XX_veneers.h>
#include <ti/drivers/tfm/SecureCallback.h>

#include <third_party/tfm/secure_fw/include/security_defs.h> // __tz_c_veneer
#include <third_party/tfm/secure_fw/spm/include/utilities.h> // tfm_core_panic()

extern int_fast16_t PowerCC27XX_s_getDependencyCount(Power_Resource resourceId, PowerCC27XX_SecurityState secState);
extern int_fast16_t PowerCC27XX_s_setDependency(Power_Resource resourceId, PowerCC27XX_SecurityState secState);
extern int_fast16_t PowerCC27XX_s_releaseDependency(Power_Resource resourceId, PowerCC27XX_SecurityState secState);
extern int_fast16_t PowerCC27XX_s_setNonSecureConstraintPointers(
    volatile const uint32_t *constraintMask,
    volatile const uint8_t (*const constraintCounts)[PowerCC27XX_NUMCONSTRAINTS]);
extern bool PowerCC27XX_isValidResourceId(Power_Resource resourceId);
extern uint_fast16_t PowerCC27XX_s_getCurrentEventsMask(void);
extern int_fast16_t PowerCC27XX_s_setNonSecureNotificationHandle(SecureCallback_Handle handle);

extern const uint32_t *constraintMask_ns;
extern const uint8_t (*constraintCounts_ns)[PowerCC27XX_NUMCONSTRAINTS];

__tz_c_veneer int_fast16_t Power_getDependencyCount_veneer(Power_Resource resourceId)
{
    int_fast16_t result = Power_EINVALIDINPUT;

    if (PowerCC27XX_isValidResourceId(resourceId))
    {
        /* The generic Power_getDependencyCount() API CANNOT be used, since this
         * would leak secure info to the non-secure side since the generic
         * Power_getDependencyCount() API combines secure and non-secure counts.
         * Therefore the PowerCC27XX_s_getDependencyCount() API must be used and
         * the PowerCC27XX_NONSECURE security state must be used.
         */
        result = PowerCC27XX_s_getDependencyCount(resourceId, PowerCC27XX_NONSECURE);
    }

    return result;
}

__tz_c_veneer PowerLPF3_ResetReason PowerLPF3_getResetReason_veneer(void)
{
    return PowerLPF3_getResetReason();
}

__tz_c_veneer int_fast16_t PowerCC27XX_setNonSecureConstraintPointers_veneer(
    volatile const uint32_t *constraintMask,
    volatile const uint8_t (*const constraintCounts)[PowerCC27XX_NUMCONSTRAINTS])
{
    int_fast16_t result = Power_EINVALIDPOINTER;
    /* Verify that the pointers are pointing to NS RAM, to prevent pointing to
     * memory with read side effects.
     */
    if ((((uintptr_t)constraintMask) >= SRAM_NS_BASE) &&
        (((uintptr_t)constraintMask + sizeof(*constraintMask)) <= (SRAM_NS_BASE + SRAM_SIZE)) &&
        (((uintptr_t)*constraintCounts) >= SRAM_NS_BASE) &&
        ((((uintptr_t)*constraintCounts) + sizeof(*constraintCounts)) <= (SRAM_NS_BASE + SRAM_SIZE)))
    {
        result = PowerCC27XX_s_setNonSecureConstraintPointers(constraintMask, constraintCounts);
    }

    return result;
}

__tz_c_veneer int_fast16_t Power_setDependency_veneer(Power_Resource resourceId)
{
    int_fast16_t result = Power_EINVALIDINPUT;

    if (PowerCC27XX_isValidResourceId(resourceId))
    {
        /* The generic Power_setDependency() API CANNOT be used, since this
         * would alter the secure dependencies, which the non-secure app is not
         * allowed to alter.
         * Therefore the PowerCC27XX_s_setDependency() API must be used and the
         * PowerCC27XX_NONSECURE security state must be used.
         * This way the non-secure app cannot modify the secure dependency
         * count.
         */
        result = PowerCC27XX_s_setDependency(resourceId, PowerCC27XX_NONSECURE);
    }

    return result;
}

__tz_c_veneer int_fast16_t Power_releaseDependency_veneer(Power_Resource resourceId)
{
    int_fast16_t result = Power_EINVALIDINPUT;

    if (PowerCC27XX_isValidResourceId(resourceId))
    {
        /* The generic Power_releaseDependency() API CANNOT be used, since this
         * would alter the secure dependencies, which the non-secure app is not
         * allowed to alter.
         * Therefore the PowerCC27XX_s_releaseDependency() API must be used and
         * the PowerCC27XX_NONSECURE security state must be used.
         * This way the non-secure app cannot modify the secure dependency
         * count.
         */
        result = PowerCC27XX_s_releaseDependency(resourceId, PowerCC27XX_NONSECURE);
    }

    return result;
}

__tz_c_veneer void PowerLPF3_releaseLatches_veneer(void)
{
    PowerLPF3_releaseLatches();
}

__tz_c_veneer int_fast16_t Power_shutdown_veneer(uint_fast16_t shutdownState, uint_fast32_t shutdownTime)
{
    return Power_shutdown(shutdownState, shutdownTime);
}

__tz_c_veneer void Power_reset_veneer(void)
{
    Power_reset();
}

__tz_c_veneer void PowerCC27XX_doWFI_veneer(void)
{
    PowerCC27XX_doWFI();
}

__tz_c_veneer int_fast16_t PowerLPF3_startAFOSC_veneer(PowerLPF3_AfoscFreq frequency)
{
    return PowerLPF3_startAFOSC(frequency);
}

__tz_c_veneer void PowerLPF3_stopAFOSC_veneer(void)
{
    PowerLPF3_stopAFOSC();
}

__tz_c_veneer uint_fast16_t PowerCC27XX_getCurrentEventsMask_veneer(void)
{
    return PowerCC27XX_s_getCurrentEventsMask();
}

__tz_c_veneer void PowerCC27XX_setNonSecureNotificationHandle_veneer(SecureCallback_Handle handle)
{
    /* Verify that the handle is pointing to NS RAM, to prevent pointing to
     * memory with read side effects.
     */
    if ((((uintptr_t)handle) < SRAM_NS_BASE) || (((uintptr_t)handle + sizeof(*handle)) > (SRAM_NS_BASE + SRAM_SIZE)))
    {
        tfm_core_panic();
    }

    /* Ignore return value since it never returns an error. */
    (void)PowerCC27XX_s_setNonSecureNotificationHandle(handle);
}

__tz_c_veneer int_fast16_t PowerLPF3_sleep_veneer(uint32_t nextEventTimeUs)
{
    return PowerLPF3_sleep(nextEventTimeUs);
}