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

/* Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <ti/drivers/dpl/HwiP.h>

#include <ti/drivers/Temperature.h>
#include <ti/drivers/temperature/TemperatureLPF3.h>
#include <ti/drivers/temperature/TemperatureLPF3_veneers.h>
#include <ti/drivers/tfm/SecureCallback.h>

#include <ti/drivers/utils/Math.h>

/* Forward declarations */
static void TemperatureLPF3_ns_walkNotifyList(void);
static void TemperatureLPF3_ns_setNextThresholds(void);
static void TemperatureLPF3_ns_temperatureEventCb(uintptr_t arg);
static void TemperatureLPF3_ns_updateThresholds(int16_t thresholdHigh, int16_t thresholdLow);

/* Globals */

/* Global list that stores all registered notifications */
volatile static List_List TemperatureLPF3_ns_notificationList;

/* Current threshold values. These should always reflect the state of the
 * Temperature notification configured on the S side.
 */
static volatile int16_t TemperatureLPF3_ns_currentThresholdHigh = INVALID_TEMPERATURE_MAX;
static volatile int16_t TemperatureLPF3_ns_currentThresholdLow  = INVALID_TEMPERATURE_MIN;

/* SecureCallback object used to allow the S side to inform the NS side, when
 * one of the configured thresholds has been crossed.
 */
static SecureCallback_Object TemperatureLPF3_ns_secureCallbackObject;

/*
 *  ======== TemperatureLPF3_ns_updateThresholds ========
 */
static void TemperatureLPF3_ns_updateThresholds(int16_t thresholdHigh, int16_t thresholdLow)
{
    bool thresholdsChanged = false;
    int16_t newThresholdHigh;
    int16_t newThresholdLow;
    if (thresholdLow > TemperatureLPF3_ns_currentThresholdLow)
    {
        /* Set new low threshold if higher than the old one. */
        TemperatureLPF3_ns_currentThresholdLow = thresholdLow;
        thresholdsChanged                      = true;
    }

    if (thresholdHigh < TemperatureLPF3_ns_currentThresholdHigh)
    {
        /* Set new high threshold if lower than the old one. */
        TemperatureLPF3_ns_currentThresholdHigh = thresholdHigh;
        thresholdsChanged                       = true;
    }

    if (thresholdsChanged)
    {
        /* Set new thresholds if they have changed. Cache thresholds to have a
         * defined order of volatile accesses.
         */
        newThresholdHigh = TemperatureLPF3_ns_currentThresholdHigh;
        newThresholdLow  = TemperatureLPF3_ns_currentThresholdLow;
        TemperatureLPF3_setNsThresholds_veneer(newThresholdHigh, newThresholdLow);
    }
}

/*
 *  ======== TemperatureLPF3_ns_temperatureEventCb ========
 *
 * Temperature notification from the S side indicating a high or low temperature
 * event.
 */
static void TemperatureLPF3_ns_temperatureEventCb(uintptr_t arg)
{
    /* Unused parameter */
    (void)arg;

    /* Clear the thresholds. This reflects that the notify object on the S
     * side was not re-registered. So essentially the current thresholds on
     * the S side are invalid. Any new thresholds will be set in the call to
     * TemperatureLPF3_ns_setNextThresholds() below.
     */
    TemperatureLPF3_ns_currentThresholdLow  = INVALID_TEMPERATURE_MIN;
    TemperatureLPF3_ns_currentThresholdHigh = INVALID_TEMPERATURE_MAX;

    /* Walk the notification list and issue any callbacks that have triggered
     * at the current temperature.
     */
    TemperatureLPF3_ns_walkNotifyList();

    /* Walk the queue another time to find and set the next set of thresholds.
     * This is faster than making even one extra access to AON_BATMON.
     */
    TemperatureLPF3_ns_setNextThresholds();
}

/*
 *  ======== Temperature_init ========
 */
void Temperature_init(void)
{
    /* Static variable to track if the driver has been initialized */
    static bool isInitialized = false;

    uintptr_t key;
    SecureCallback_Handle secureCallbackHandle;

    key = HwiP_disable();

    if (isInitialized == false)
    {
        /* Initialize the SecureCallback object and inform the S side about it. */
        secureCallbackHandle = SecureCallback_construct(&TemperatureLPF3_ns_secureCallbackObject,
                                                        TemperatureLPF3_ns_temperatureEventCb,
                                                        0);
        TemperatureLPF3_setSecureCallbackHandle_veneer(secureCallbackHandle);

        isInitialized = true;
    }

    HwiP_restore(key);
}

/*
 *  ======== Temperature_getTemperature ========
 */
int16_t Temperature_getTemperature(void)
{
    return TemperatureLPF3_getTemperature_veneer();
}

/*
 *  ======== TemperatureLPF3_enableTSDMonitoring ========
 */
void TemperatureLPF3_enableTSDMonitoring(int16_t shutdownThreshold)
{
    TemperatureLPF3_enableTSDMonitoring_veneer(shutdownThreshold);
}

/*
 *  ======== TemperatureLPF3_disableTSDMonitoring ========
 */
void TemperatureLPF3_disableTSDMonitoring(void)
{
    TemperatureLPF3_disableTSDMonitoring_veneer();
}

/*
 *  ======== TemperatureLPF3_triggerThermalShutdown ========
 */
void TemperatureLPF3_triggerThermalShutdown(void)
{
    TemperatureLPF3_triggerThermalShutdown_veneer();
}

/*
 * =============================================================================
 * The implementation of below functions should be kept the same as the
 * functions with the same names (except for the module prefix) in
 * TemperatureLPF3.c
 * =============================================================================
 */

/*
 *  ======== TemperatureLPF3_ns_setNextThresholds ========
 */
static void TemperatureLPF3_ns_setNextThresholds(void)
{
    List_Elem *notifyLink;
    int16_t nextThresholdHigh = INVALID_TEMPERATURE_MAX;
    int16_t nextThresholdLow  = INVALID_TEMPERATURE_MIN;
    uintptr_t key;

    key = HwiP_disable();

    /* Starting with the head of the list, keep track of the smallest high
     * threshold and largest low threshold.
     */
    notifyLink = List_head((List_List *)&TemperatureLPF3_ns_notificationList);

    while (notifyLink != NULL)
    {
        Temperature_NotifyObj *notifyObject = (Temperature_NotifyObj *)notifyLink;

        nextThresholdHigh = Math_MIN(nextThresholdHigh, notifyObject->thresholdHigh);
        nextThresholdLow  = Math_MAX(nextThresholdLow, notifyObject->thresholdLow);

        notifyLink = List_next(notifyLink);
    }

    /* Now that we have found the next upper and lower thresholds, set them.
     * These could be INVALID_TEMPERATURE_MAX and/or INVALID_TEMPERATURE_MIN
     * if the list is empty or only high/low notifications were registered.
     */
    TemperatureLPF3_ns_updateThresholds(nextThresholdHigh, nextThresholdLow);

    HwiP_restore(key);
}

/*
 *  ======== TemperatureLPF3_ns_walkNotifyList ========
 */
static void TemperatureLPF3_ns_walkNotifyList(void)
{
    List_Elem *notifyLink      = List_head((List_List *)&TemperatureLPF3_ns_notificationList);
    int16_t currentTemperature = Temperature_getTemperature();

    /* If the notification list is empty, the head pointer will be
     * NULL and the while loop will never execute the statement.
     */
    while (notifyLink != NULL)
    {
        Temperature_NotifyObj *notifyObject = (Temperature_NotifyObj *)notifyLink;

        /* Buffer the next link in case the notification triggers.
         * Without buffering, we might skip list entries if the
         * notifyObject is freed or reregistered and the notifyObject->link.next
         * pointer is altered.
         */
        List_Elem *notifyLinkNext = List_next(notifyLink);

        /* If the current temperature is below this notification's low
         * threshold or above its high threshold, remove it from the list and
         * call the callback fxn
         */
        if ((currentTemperature <= notifyObject->thresholdLow) || (currentTemperature >= notifyObject->thresholdHigh))
        {

            /* Choose the threshold to provide to the notifyFxn based on the
             * thresholds and the current temperature.
             */
            int16_t threshold = (currentTemperature <= notifyObject->thresholdLow) ? notifyObject->thresholdLow
                                                                                   : notifyObject->thresholdHigh;

            List_remove((List_List *)&TemperatureLPF3_ns_notificationList, notifyLink);
            notifyObject->isRegistered = false;

            notifyObject->notifyFxn(currentTemperature, threshold, notifyObject->clientArg, notifyObject);
        }

        notifyLink = notifyLinkNext;
    }
}

/*
 *  ======== Temperature_registerNotifyHigh ========
 */
int_fast16_t Temperature_registerNotifyHigh(Temperature_NotifyObj *notifyObject,
                                            int16_t thresholdHigh,
                                            Temperature_NotifyFxn notifyFxn,
                                            uintptr_t clientArg)
{
    uintptr_t key;

    key = HwiP_disable();

    notifyObject->thresholdHigh = thresholdHigh;
    notifyObject->thresholdLow  = INVALID_TEMPERATURE_MIN;
    notifyObject->notifyFxn     = notifyFxn;
    notifyObject->clientArg     = clientArg;

    if (notifyObject->isRegistered == false)
    {
        /* Add the notification to the end of the list.
         * There is the implicit assumption that the notification is not already
         * in the list. Otherwise the list linkage will be corrupted.
         */
        List_put((List_List *)&TemperatureLPF3_ns_notificationList, &notifyObject->link);

        notifyObject->isRegistered = true;
    }

    TemperatureLPF3_ns_updateThresholds(notifyObject->thresholdHigh, notifyObject->thresholdLow);

    HwiP_restore(key);

    return Temperature_STATUS_SUCCESS;
}

/*
 *  ======== Temperature_registerNotifyLow ========
 */
int_fast16_t Temperature_registerNotifyLow(Temperature_NotifyObj *notifyObject,
                                           int16_t thresholdLow,
                                           Temperature_NotifyFxn notifyFxn,
                                           uintptr_t clientArg)
{
    uintptr_t key;

    key = HwiP_disable();

    notifyObject->thresholdHigh = INVALID_TEMPERATURE_MAX;
    notifyObject->thresholdLow  = thresholdLow;
    notifyObject->notifyFxn     = notifyFxn;
    notifyObject->clientArg     = clientArg;

    if (notifyObject->isRegistered == false)
    {
        /* Add the notification to the end of the list.
         * There is the implicit assumption that the notification is not already
         * in the list. Otherwise the list linkage will be corrupted.
         */
        List_put((List_List *)&TemperatureLPF3_ns_notificationList, &notifyObject->link);

        notifyObject->isRegistered = true;
    }

    TemperatureLPF3_ns_updateThresholds(notifyObject->thresholdHigh, notifyObject->thresholdLow);

    HwiP_restore(key);

    return Temperature_STATUS_SUCCESS;
}

/*
 *  ======== Temperature_registerNotifyRange ========
 */
int_fast16_t Temperature_registerNotifyRange(Temperature_NotifyObj *notifyObject,
                                             int16_t thresholdHigh,
                                             int16_t thresholdLow,
                                             Temperature_NotifyFxn notifyFxn,
                                             uintptr_t clientArg)
{
    uintptr_t key;

    key = HwiP_disable();

    notifyObject->thresholdHigh = thresholdHigh;
    notifyObject->thresholdLow  = thresholdLow;
    notifyObject->notifyFxn     = notifyFxn;
    notifyObject->clientArg     = clientArg;

    if (notifyObject->isRegistered == false)
    {
        /* Add the notification to the end of the list.
         * There is the implicit assumption that the notification is not already
         * in the list. Otherwise the list linkage will be corrupted.
         */
        List_put((List_List *)&TemperatureLPF3_ns_notificationList, &notifyObject->link);

        notifyObject->isRegistered = true;
    }

    TemperatureLPF3_ns_updateThresholds(notifyObject->thresholdHigh, notifyObject->thresholdLow);

    HwiP_restore(key);

    return Temperature_STATUS_SUCCESS;
}

/*
 *  ======== Temperature_unregisterNotify ========
 */
int_fast16_t Temperature_unregisterNotify(Temperature_NotifyObj *notifyObject)
{
    uintptr_t key;

    key = HwiP_disable();

    if (notifyObject->isRegistered == true)
    {
        /* Remove the notification from the list */
        List_remove((List_List *)&TemperatureLPF3_ns_notificationList, &(notifyObject->link));

        notifyObject->isRegistered = false;
    }

    /* Find the next set of thresholds and update the registers */
    TemperatureLPF3_ns_setNextThresholds();

    HwiP_restore(key);

    return Temperature_STATUS_SUCCESS;
}