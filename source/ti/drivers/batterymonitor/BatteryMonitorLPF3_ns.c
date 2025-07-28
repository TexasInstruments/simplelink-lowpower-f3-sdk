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
 *  ======== BatteryMonitorLPF3_ns.c ========
 *
 */

/* Includes */
#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/dpl/HwiP.h>

#include <ti/drivers/BatteryMonitor.h>
#include <ti/drivers/batterymonitor/BatteryMonitorLPF3.h>
#include <ti/drivers/batterymonitor/BatteryMonitorLPF3_veneers.h>
#include <ti/drivers/tfm/SecureCallback.h>

#include <ti/drivers/utils/Math.h>

/* Forward declarations */
static void BatteryMonitorLPF3_ns_walkNotifyList(uint16_t currentVoltage);
static void BatteryMonitorLPF3_ns_setNextThresholds(void);
static void BatteryMonitorLPF3_ns_batteryEventCb(uintptr_t arg);
static void BatteryMonitorLPF3_ns_updateThresholds(uint16_t thresholdHigh, uint16_t thresholdLow);

/* Globals */

/* Global list that stores all registered notifications */
static volatile List_List BatteryMonitorLPF3_ns_notificationList;

/* Current threshold values. These should always reflect the state of the
 * BatteryMonitor notification configured on the S side.
 */
static volatile uint16_t BatteryMonitorLPF3_ns_currentThresholdHigh = INVALID_VOLTAGE_MAX;
static volatile uint16_t BatteryMonitorLPF3_ns_currentThresholdLow  = INVALID_VOLTAGE_MIN;

/* SecureCallback object used to allow the S side to inform the NS side, when
 * one of the configured thresholds has been crossed.
 */
static SecureCallback_Object BatteryMonitorLPF3_ns_secureCallbackObject;

/*
 *  ======== BatteryMonitorLPF3_ns_updateThresholds ========
 */
static void BatteryMonitorLPF3_ns_updateThresholds(uint16_t thresholdHigh, uint16_t thresholdLow)
{
    bool thresholdsChanged = false;
    uint16_t newThresholdHigh;
    uint16_t newThresholdLow;
    if (thresholdLow > BatteryMonitorLPF3_ns_currentThresholdLow)
    {
        /* Set new low threshold if higher than the old one. */
        BatteryMonitorLPF3_ns_currentThresholdLow = thresholdLow;
        thresholdsChanged                         = true;
    }

    if (thresholdHigh < BatteryMonitorLPF3_ns_currentThresholdHigh)
    {
        /* Set new high threshold if lower than the old one. */
        BatteryMonitorLPF3_ns_currentThresholdHigh = thresholdHigh;
        thresholdsChanged                          = true;
    }

    if (thresholdsChanged)
    {
        /* Set new thresholds if they have changed. Cache thresholds to have a
         * defined order of volatile accesses.
         */
        newThresholdHigh = BatteryMonitorLPF3_ns_currentThresholdHigh;
        newThresholdLow  = BatteryMonitorLPF3_ns_currentThresholdLow;
        BatteryMonitorLPF3_setNsThresholds_veneer(newThresholdHigh, newThresholdLow);
    }
}

/*
 *  ======== BatteryMonitorLPF3_ns_batteryEventCb ========
 * BatteryMonitor notification from the S side indicating a high or low battery
 * event.
 */
static void BatteryMonitorLPF3_ns_batteryEventCb(uintptr_t arg)
{
    /* Unused parameter */
    (void)arg;

    /* Get the current voltage */
    uint16_t currentVoltage = BatteryMonitor_getVoltage();

    /* Cache old threshold values before clearing them below */
    uint16_t oldThresholdLow  = BatteryMonitorLPF3_ns_currentThresholdLow;
    uint16_t oldThresholdHigh = BatteryMonitorLPF3_ns_currentThresholdHigh;

    /* Clear the thresholds. This reflects that the notify object on the S
     * side was not re-registered. So essentially the current thresholds on
     * the S side are invalid. Any new thresholds will be set in either the
     * call to BatteryMonitorLPF3_ns_setNextThresholds() or
     * BatteryMonitorLPF3_ns_updateThresholds() below.
     */
    BatteryMonitorLPF3_ns_currentThresholdLow  = INVALID_VOLTAGE_MIN;
    BatteryMonitorLPF3_ns_currentThresholdHigh = INVALID_VOLTAGE_MAX;

    /* Only walk through notify list if the current voltage is actually above
     * the high threshold or below the low threshold.
     */
    if (currentVoltage <= oldThresholdLow || currentVoltage >= oldThresholdHigh)
    {
        /* Walk the notification list and issue any callbacks that have
         * triggered at the current voltage.
         */
        BatteryMonitorLPF3_ns_walkNotifyList(currentVoltage);

        /* Walk the queue another time to find and set the next set of
         * thresholds.
         */
        BatteryMonitorLPF3_ns_setNextThresholds();
    }
    else
    {
        /* Re-use the old thresholds. */
        BatteryMonitorLPF3_ns_updateThresholds(oldThresholdHigh, oldThresholdLow);
    }
}

/*
 *  ======== BatteryMonitor_init ========
 */
void BatteryMonitor_init(void)
{
    /* Static variable to track if the driver has been initialized */
    static bool isInitialized = false;

    uintptr_t key;
    SecureCallback_Handle secureCallbackHandle;

    key = HwiP_disable();

    if (isInitialized == false)
    {
        /* Initialize the SecureCallback object and inform the S side about it. */
        secureCallbackHandle = SecureCallback_construct(&BatteryMonitorLPF3_ns_secureCallbackObject,
                                                        BatteryMonitorLPF3_ns_batteryEventCb,
                                                        0);
        BatteryMonitorLPF3_setSecureCallbackHandle_veneer(secureCallbackHandle);

        isInitialized = true;
    }

    HwiP_restore(key);
}

/*
 *  ======== BatteryMonitor_getVoltage ========
 */
uint16_t BatteryMonitor_getVoltage(void)
{
    return BatteryMonitorLPF3_getVoltage_veneer();
}

/*
 * =============================================================================
 * The implementation of below functions should be kept the same as the
 * functions with the same names (except for the module prefix) in
 * BatteryMonitorLPF3.c
 * =============================================================================
 */

/*
 *  ======== BatteryMonitorLPF3_ns_setNextThresholds ========
 */
static void BatteryMonitorLPF3_ns_setNextThresholds(void)
{
    List_Elem *notifyLink;
    uint16_t nextThresholdHigh = INVALID_VOLTAGE_MAX;
    uint16_t nextThresholdLow  = INVALID_VOLTAGE_MIN;
    uintptr_t key;

    key = HwiP_disable();

    /* Starting with the head of the list, keep track of the smallest high
     * threshold and largest low threshold.
     */
    notifyLink = List_head((List_List *)&BatteryMonitorLPF3_ns_notificationList);

    while (notifyLink != NULL)
    {
        BatteryMonitor_NotifyObj *notifyObject = (BatteryMonitor_NotifyObj *)notifyLink;

        nextThresholdHigh = Math_MIN(nextThresholdHigh, notifyObject->thresholdHigh);
        nextThresholdLow  = Math_MAX(nextThresholdLow, notifyObject->thresholdLow);

        notifyLink = List_next(notifyLink);
    }

    /* Now that we have found the next upper and lower thresholds, set them.
     * These could be INVALID_VOLTAGE_MAX and/or INVALID_VOLTAGE_MIN
     * if the list is empty or only high/low notifications were registered.
     */
    BatteryMonitorLPF3_ns_updateThresholds(nextThresholdHigh, nextThresholdLow);

    HwiP_restore(key);
}

/*
 *  ======== BatteryMonitorLPF3_ns_walkNotifyList ========
 */
static void BatteryMonitorLPF3_ns_walkNotifyList(uint16_t currentVoltage)
{
    List_Elem *notifyLink = List_head((List_List *)&BatteryMonitorLPF3_ns_notificationList);

    /* If the notification list is empty, the head pointer will be
     * NULL and the while loop will never execute the statement.
     */
    while (notifyLink != NULL)
    {
        BatteryMonitor_NotifyObj *notifyObject = (BatteryMonitor_NotifyObj *)notifyLink;

        /* Buffer the next link in case the notification triggers.
         * Without buffering, we might skip list entries if the
         * notifyObject is freed or re-registered and the notifyObject->link.next
         * pointer is altered.
         */
        List_Elem *notifyLinkNext = List_next(notifyLink);

        /* If the current voltage is below this notification's low
         * threshold or above its high threshold, remove it from the list and
         * call the callback fxn
         */
        if ((currentVoltage <= notifyObject->thresholdLow) || (currentVoltage >= notifyObject->thresholdHigh))
        {

            /* Choose the threshold to provide to the notifyFxn based on the
             * thresholds and the current voltage.
             */
            uint16_t threshold = (currentVoltage <= notifyObject->thresholdLow) ? notifyObject->thresholdLow
                                                                                : notifyObject->thresholdHigh;

            List_remove((List_List *)&BatteryMonitorLPF3_ns_notificationList, notifyLink);
            notifyObject->isRegistered = false;

            notifyObject->notifyFxn(currentVoltage, threshold, notifyObject->clientArg, notifyObject);
        }

        notifyLink = notifyLinkNext;
    }
}

/*
 *  ======== BatteryMonitor_registerNotifyHigh ========
 */
int_fast16_t BatteryMonitor_registerNotifyHigh(BatteryMonitor_NotifyObj *notifyObject,
                                               uint16_t thresholdHigh,
                                               BatteryMonitor_NotifyFxn notifyFxn,
                                               uintptr_t clientArg)
{
    uintptr_t key;

    key = HwiP_disable();

    notifyObject->thresholdHigh = thresholdHigh;
    notifyObject->thresholdLow  = INVALID_VOLTAGE_MIN;
    notifyObject->notifyFxn     = notifyFxn;
    notifyObject->clientArg     = clientArg;

    if (notifyObject->isRegistered == false)
    {
        /* Add the notification to the end of the list.
         * There is the implicit assumption that the notification is not already
         * in the list. Otherwise the list linkage will be corrupted.
         */
        List_put((List_List *)&BatteryMonitorLPF3_ns_notificationList, &notifyObject->link);

        notifyObject->isRegistered = true;
    }

    BatteryMonitorLPF3_ns_updateThresholds(notifyObject->thresholdHigh, notifyObject->thresholdLow);

    HwiP_restore(key);

    return BatteryMonitor_STATUS_SUCCESS;
}

/*
 *  ======== BatteryMonitor_registerNotifyLow ========
 */
int_fast16_t BatteryMonitor_registerNotifyLow(BatteryMonitor_NotifyObj *notifyObject,
                                              uint16_t thresholdLow,
                                              BatteryMonitor_NotifyFxn notifyFxn,
                                              uintptr_t clientArg)
{
    uintptr_t key;

    key = HwiP_disable();

    notifyObject->thresholdHigh = INVALID_VOLTAGE_MAX;
    notifyObject->thresholdLow  = thresholdLow;
    notifyObject->notifyFxn     = notifyFxn;
    notifyObject->clientArg     = clientArg;

    if (notifyObject->isRegistered == false)
    {
        /* Add the notification to the end of the list.
         * There is the implicit assumption that the notification is not already
         * in the list. Otherwise the list linkage will be corrupted.
         */
        List_put((List_List *)&BatteryMonitorLPF3_ns_notificationList, &notifyObject->link);

        notifyObject->isRegistered = true;
    }

    BatteryMonitorLPF3_ns_updateThresholds(notifyObject->thresholdHigh, notifyObject->thresholdLow);

    HwiP_restore(key);

    return BatteryMonitor_STATUS_SUCCESS;
}

/*
 *  ======== BatteryMonitor_registerNotifyRange ========
 */
int_fast16_t BatteryMonitor_registerNotifyRange(BatteryMonitor_NotifyObj *notifyObject,
                                                uint16_t thresholdHigh,
                                                uint16_t thresholdLow,
                                                BatteryMonitor_NotifyFxn notifyFxn,
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
        List_put((List_List *)&BatteryMonitorLPF3_ns_notificationList, &notifyObject->link);

        notifyObject->isRegistered = true;
    }

    BatteryMonitorLPF3_ns_updateThresholds(notifyObject->thresholdHigh, notifyObject->thresholdLow);

    HwiP_restore(key);

    return BatteryMonitor_STATUS_SUCCESS;
}

/*
 *  ======== BatteryMonitor_unregisterNotify ========
 */
int_fast16_t BatteryMonitor_unregisterNotify(BatteryMonitor_NotifyObj *notifyObject)
{
    uintptr_t key;

    key = HwiP_disable();

    if (notifyObject->isRegistered == true)
    {
        /* Remove the notification from the list */
        List_remove((List_List *)&BatteryMonitorLPF3_ns_notificationList, &(notifyObject->link));

        notifyObject->isRegistered = false;
    }

    /* Find the next set of thresholds and update the registers */
    BatteryMonitorLPF3_ns_setNextThresholds();

    HwiP_restore(key);

    return BatteryMonitor_STATUS_SUCCESS;
}