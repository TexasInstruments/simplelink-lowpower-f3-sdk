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
 *  ======== BatteryMonitorLPF3_veneers.c ========
 */
#include <stdint.h>

#include <third_party/tfm/secure_fw/include/security_defs.h> // __tz_c_veneer
#include <third_party/tfm/secure_fw/spm/include/utilities.h> // tfm_core_panic()

#include <ti/drivers/BatteryMonitor.h>
#include <ti/drivers/batterymonitor/BatteryMonitorLPF3.h>
#include <ti/drivers/batterymonitor/BatteryMonitorLPF3_veneers.h>
#include <ti/drivers/tfm/SecureCallback.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)

/* Declarations of functions in BatteryMonitorLPF3_s.c. */
extern void BatteryMonitorLPF3_s_setNsThresholds(uint16_t thresholdHigh, uint16_t thresholdLow);
extern void BatteryMonitorLPF3_s_setSecureCallbackHandle(SecureCallback_Handle handle);

/*
 *  ======== BatteryMonitorLPF3_setSecureCallbackHandle_veneer ========
 *
 * Veneer to inform the secure side about the non-secure side's SecureCallback
 * handle. Panics if the handle is invalid.
 */
__tz_c_veneer void BatteryMonitorLPF3_setSecureCallbackHandle_veneer(SecureCallback_Handle handle)
{
    /* Verify that the handle is pointing to NS RAM, to prevent pointing to
     * memory with read side effects.
     */
    if ((((uintptr_t)handle) < SRAM_NS_BASE) || (((uintptr_t)handle + sizeof(*handle)) > (SRAM_NS_BASE + SRAM_SIZE)))
    {
        tfm_core_panic();
    }

    BatteryMonitorLPF3_s_setSecureCallbackHandle(handle);
}

/*
 *  ======== BatteryMonitorLPF3_getVoltage_veneer ========
 */
__tz_c_veneer uint16_t BatteryMonitorLPF3_getVoltage_veneer(void)
{
    return BatteryMonitor_getVoltage();
}

/*
 *  ======== BatteryMonitorLPF3_setNsThresholds_veneer ========
 */
__tz_c_veneer void BatteryMonitorLPF3_setNsThresholds_veneer(uint16_t thresholdHigh, uint16_t thresholdLow)
{
#if BATMON_VOLTAGE_MIN != 0U
    #error This implementation of BatteryMonitorLPF3_setNsThresholds_veneer() \
           assumes that BATMON_VOLTAGE_MIN == 0.                              \
           The implementation must be changed if this is not the case.
#endif
    /* Verify that the thresholds are within the valid range. */
    if (/*(thresholdHigh < BATMON_VOLTAGE_MIN) || (thresholdLow < BATMON_VOLTAGE_MIN) || */
        (thresholdHigh > BATMON_VOLTAGE_MAX) || (thresholdLow > BATMON_VOLTAGE_MAX))
    {
        tfm_core_panic();
    }

    BatteryMonitorLPF3_s_setNsThresholds(thresholdHigh, thresholdLow);
}