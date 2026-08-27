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
 *  ======== TemperatureLPF3_s.c ========
 *
 */

/* Includes */
#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/dpl/HwiP.h>

#include <ti/drivers/Temperature.h>
#include <ti/drivers/temperature/TemperatureLPF3.h>
#include <ti/drivers/tfm/SecureCallback.h>
#include <ti/drivers/tfm/SecureCallback_s.h>

/* Forward declarations */
void TemperatureLPF3_s_setNsThresholds(int16_t thresholdHigh, int16_t thresholdLow);
void TemperatureLPF3_s_setSecureCallbackHandle(SecureCallback_Handle handle);

/* The SecureCallback handle used for informing the NS side that a NS threshold
 * has been crossed. This is set by the non-secure side through a veneer.
 */
static volatile SecureCallback_Handle TemperatureLPF3_s_secureCallbackHandle = NULL;

/*
 *  ======== TemperatureLPF3_s_nsNotifyFxn ========
 *
 * Callback function used when a notification has been registered for
 * nsNotifyObject.
 */
static void TemperatureLPF3_s_nsNotifyFxn(int16_t currentTemperature,
                                          int16_t thresholdTemperature,
                                          uintptr_t clientArg,
                                          Temperature_NotifyObj *notifyObject)
{
    /* No parameters are used. */
    (void)currentTemperature;
    (void)thresholdTemperature;
    (void)clientArg;
    (void)notifyObject;

    if (TemperatureLPF3_s_secureCallbackHandle != NULL)
    {
        /* Notify the NS side that its temperature threshold(s) has been
         * crossed. It is up to the NS side to re-register the notification, by
         * setting new thresholds using BatteryMonitorLPF3_s_setNsThresholds()
         * through a veneer.
         */
        SecureCallback_s_post(TemperatureLPF3_s_secureCallbackHandle);
    }
}

/*
 *  ======== TemperatureLPF3_s_setNsThresholds ========
 *
 * API to let the NS side set the thresholds for when it wants a notification
 * through the SecureCallback. It is implemented using a normal range
 * notification, where the callback function will post the SecureCallback.
 */
void TemperatureLPF3_s_setNsThresholds(int16_t thresholdHigh, int16_t thresholdLow)
{
    /* Notify object to handle notifications for the non-secure side.
     * Note, this is static since the object cannot be placed on the stack since
     * the Temperature driver is using the object after this function returns.
     * In addition, it is static since the same object should be used for each
     * call to this function.
     */
    static volatile Temperature_NotifyObj nsNotifyObject;

    /* Disable interrupts to ensure unregister + register is atomic */
    uintptr_t key = HwiP_disable();

    if (nsNotifyObject.isRegistered == true)
    {
        /* Unregister NS notification, if it has already been registered */
        (void)Temperature_unregisterNotify((Temperature_NotifyObj *)&nsNotifyObject);
    }

    if ((thresholdLow != INVALID_TEMPERATURE_MIN) || (thresholdHigh != INVALID_TEMPERATURE_MAX))
    {
        /* Register range notification, if any threshold is set. */
        (void)Temperature_registerNotifyRange((Temperature_NotifyObj *)&nsNotifyObject,
                                              thresholdHigh,
                                              thresholdLow,
                                              TemperatureLPF3_s_nsNotifyFxn,
                                              0);
    }

    HwiP_restore(key);
}

/*
 *  ======== TemperatureLPF3_s_setSecureCallbackHandle ========
 *
 * API to let the NS side set the SecureCallback handle. This is available
 * through a veneer.
 */
void TemperatureLPF3_s_setSecureCallbackHandle(SecureCallback_Handle handle)
{
    /* Set SecureCallback handle. No need for validation, since it is done in
     * the veneer.
     */
    TemperatureLPF3_s_secureCallbackHandle = handle;
}