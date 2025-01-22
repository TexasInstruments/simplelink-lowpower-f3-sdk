/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated
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
 *  ======== PowerCC27XX_lfoscCompensation.c ========
 */

#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC27XX.h>
#include <ti/drivers/Temperature.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/ckmd.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_pmctl.h)
#include DeviceFamily_constructPath(inc/hw_ckmd.h)

/* The amount of time in us to measure the LFOSC during a measurement */
#ifndef LFOSC_CALIBRATION_DURATION_US
    #define LFOSC_CALIBRATION_DURATION_US (1500)
#endif

/* The baseline ppm value used in the model to to calculate the calibration
 * interval in PowerCC27XX_setLfoscCompensationConfiguration()
 * TODO: Determine the correct value to use
 */
#ifndef LFOSC_BASELINE_PPM
    #define LFOSC_BASELINE_PPM (200)
#endif

/* The maximum uncompensated LFOSC error in ppm */
#define LFOSC_MAX_ABSOLUTE_PPM_ERROR (30000)
/* The jitter caused by SYSTIMER in us */
#define SYSTIMER_JITTER_USEC         (1)

static uint32_t PowerCC27XX_convertUsToLfPeriods(uint32_t us);
static void PowerCC27XX_setLfoscCompensationConfiguration(const PowerLPF3_LfoscCompensationProfile *profile,
                                                          int16_t currentTemperature);
static void PowerCC27XX_registerLfoscCompensationTemperatureNotification(int16_t currentTemperature);
static void PowerCC27XX_lfoscCompensationTemperatureNotifyFxn(int16_t currentTemperature,
                                                              int16_t thresholdTemperature,
                                                              uintptr_t clientArg,
                                                              Temperature_NotifyObj *notifyObject);

typedef enum
{
    PowerCC27XX_LFOSC_COMPENSATION_STATE_UNINITIALIZED = 0x0,
    PowerCC27XX_LFOSC_COMPENSATION_STATE_DISABLED      = 0x10,
    PowerCC27XX_LFOSC_COMPENSATION_STATE_ENABLED       = 0x30,
} PowerCC27XX_LfoscCompensationState;

/*! Object used for LFOSC Calibration */
static struct
{
    PowerCC27XX_LfoscCompensationState state;
    Temperature_NotifyObj tempNotifyObj;
    PowerLPF3_LfoscCompensationProfile currentProfile;
} PowerCC27XX_lfoscCompensationObj = {
    /* Ensure that the state is UNINITIALIZED by default */
    .state = PowerCC27XX_LFOSC_COMPENSATION_STATE_UNINITIALIZED,
};

/*
 *  ======== PowerCC27XX_convertUsToLfPeriods ========
 * Convert microseconds (us) to number of LF periods.
 */
static uint32_t PowerCC27XX_convertUsToLfPeriods(uint32_t us)
{
    /* The nominal LF oscillator frequency is 32768 Hz. To convert a duration in
     * microseconds to LF periods, the formula is:
     *
     * LF periods = (us * 32768) / 1000000
     *
     * Units: us * (LF period/s) / (us/s) = LF periods
     *
     * To ensure no overflow during the multiplication, us would need to be cast
     * to uint64_t. However, to avoid the complexity of 64-bit division, the
     * fraction (32768 / 1000000) is approximated as (2 / 61). This
     * approximation has an accuracy of ~0.06%.
     */
    return (2 * us) / 61;
}

/*
 *  ======== PowerCC27XX_registerLfoscCompensationTemperatureNotification ========
 * Register temperature notification based on the current temperature
 */
static void PowerCC27XX_registerLfoscCompensationTemperatureNotification(int16_t currentTemperature)
{
#if (CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX == INT16_MAX) || (CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MIN != INT16_MIN)
    #error "The current LFOSC Compensation implementation only works when there is no lower limit to the "middle" temperature range and when there is an upper limit."
#endif
    /* Register notification for when the temperature crosses
     * CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX
     */
    if (currentTemperature <= CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX)
    {
        Temperature_registerNotifyHigh(&PowerCC27XX_lfoscCompensationObj.tempNotifyObj,
                                       CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX,
                                       PowerCC27XX_lfoscCompensationTemperatureNotifyFxn,
                                       (uintptr_t)NULL);
    }
    else
    {
        Temperature_registerNotifyLow(&PowerCC27XX_lfoscCompensationObj.tempNotifyObj,
                                      CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX,
                                      PowerCC27XX_lfoscCompensationTemperatureNotifyFxn,
                                      (uintptr_t)NULL);
    }
}

/*
 *  ======== PowerCC27XX_lfoscCompensationTemperatureNotifyFxn ========
 */
static void PowerCC27XX_lfoscCompensationTemperatureNotifyFxn(int16_t currentTemperature,
                                                              int16_t thresholdTemperature,
                                                              uintptr_t clientArg,
                                                              Temperature_NotifyObj *notifyObject)
{
    uintptr_t key;

    /* Ensure that state isn't asynchronously changed after checking the state */
    key = HwiP_disable();

    /* If LFOSC compensation has been disabled asynchronously during execution
     * of this callback then do not update the calibration interval and
     * re-register the notification object.
     */
    if (PowerCC27XX_lfoscCompensationObj.state == PowerCC27XX_LFOSC_COMPENSATION_STATE_ENABLED)
    {
        /* Update the LFOSC compensation configuration based on the new
         * temperature.
         */
        PowerCC27XX_setLfoscCompensationConfiguration(&PowerCC27XX_lfoscCompensationObj.currentProfile,
                                                      currentTemperature);
    }
    HwiP_restore(key);
}

/*
 *  ======== PowerCC27XX_setLfoscCompensationConfiguration ========
 * This function will based on the specified profile and temperature compute the
 * calibration interval and update the PMCTL.LFCAL.PER field. If the calculated
 * value is bigger that the maximum value allowed in the PMCTL.LFCAL.PER field,
 * then the maximum allowed value will be used instead.
 *
 * The registered temperature notification will be updated to ensure the
 * configured calibration interval matches the configured temperature
 * notifications.
 */
static void PowerCC27XX_setLfoscCompensationConfiguration(const PowerLPF3_LfoscCompensationProfile *profile,
                                                          int16_t currentTemperature)
{
    uint16_t ppmPerC;                          /* Worst case ppm/C */
    uint64_t tempPpmPerSystemWakeup;           /* Worst case ppm per wakeup period caused by temperature changes */
    uint16_t ppmPerRtn;                        /* Worst case ppm for one RTN event */
    uint16_t baselinePpm;                      /* The baseline ppm used in the model below */
    uint32_t ppmPerLfoscJitter;                /* Additional required ppm due to lfosc jitter */
    uint16_t lfsocMaxWakeupJitterUsec;         /* The maximum LFSOC jitter per wakeup in us */
    uint32_t ppmRequirement;                   /* The required ppm */
    uint32_t n;                                /* Number of calibrations per system wakeup */
    uint32_t calibrationIntervalUs;            /* The calculated calibration interval in us */
    uint32_t calibrationInterval256LfPeriods;  /* The calculated calibration interval in 256 LF periods */
    uint32_t lfcalReg;                         /* Temperature variable used when writing to the PMCTL.LFCAL register */
    uint32_t maxMeasurementPeriod256LfPeriods; /* Max value supported by the PMCTL.LFCAL.PER field */
    uintptr_t key;                             /* Key returned by HwiP_disable() */

    /* Read FCFG values */
#if (CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX == INT16_MAX) || (CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MIN != INT16_MIN)
    #error "The current LFOSC Compensation implementation only works when there is no lower limit to the "middle" temperature range and when there is an upper limit."
#endif
    if (currentTemperature <= CKMD_LFOSC_MID_TEMP_COEFFICIENT_RANGE_MAX)
    {
        ppmPerC = CKMDGetLfoscMidTempCoefficientPpmPerC();
    }
    else
    {
        ppmPerC = CKMDGetLfoscExtTempCoefficientPpmPerC();
    }

    /* Multiply the PPM per RTN event by 2 to compensate for the simplified
     * model below (ppm_rtn(n)  = ppmPerRtn/n).
     */
    ppmPerRtn   = CKMDGetLfoscRtnPpm() * 2;
    baselinePpm = LFOSC_BASELINE_PPM;

    /* Calculate ppm contribution due to temperature change over the entire
     * system wakeup period.
     * Units for the statement ppmPerC * profile->temperatureGradientMilliCelciusPerSec *
     * profile->systemWakeupIntervalUsec / 2: (ppm/C) * (mC/s) * (us/(system wakeup)) = 10^(-9) * (ppm/(system wakeup))
     * To convert this unit to ppm/(system wakeup) the result is divided by
     * 10^9.
     *
     * The product ppmPerC * profile->temperatureGradientMilliCelciusPerSec * profile->systemWakeupIntervalUsec
     * is using the types uint16_t, uint16_t and uint32_t respectively. The
     * result of the product will fit in uint64_t.
     *
     * For the final result to fit in uint32_t, the following inequality must be satisfied:
     * 0x100000000 > ppmPerC * profile->temperatureGradientMilliCelciusPerSec * profile->systemWakeupIntervalUsec / (2 *
     * 1000000000)
     *
     * 0x100000000 * (2 * 1000000000) > ppmPerC * profile->temperatureGradientMilliCelciusPerSec *
     * profile->systemWakeupIntervalUsec
     *
     * If assuming profile->systemWakeupIntervalUsec and
     * profile->temperatureGradientMilliCelciusPerSec can take any value, then
     * the inequality can be written as below:
     * 0x100000000 * (2 * 1000000000) > ppmPerC * 0xFFFF * 0xFFFFFFFF
     * 0x100000000 * (2 * 1000000000) / (0xFFFF * 0xFFFFFFFF) > ppmPerC
     * ppmPerC < 30518
     *
     * No devices has such a bad ppm/C, so the result will fit in uint32_t
     */
    tempPpmPerSystemWakeup = (uint64_t)ppmPerC * profile->temperatureGradientMilliCelsiusPerSec *
                             profile->systemWakeupIntervalUsec / (2 * 1000000000);

    /* Calculate ppm contribution to required ppm due to LFOSC jitter (rounded
     * down).
     * Make sure that lfsocMaxWakeupJitterUsec is non negative.
     * Units for the statement lfsocMaxWakeupJitterUsec / profile->systemWakeupIntervalUsec:
     * (us/system wakeup) / (us/(system wakeup)) = unitless
     * To convert this unit to ppm the result is multiplied by 10^6.
     *
     * The allowed jitter should always be lower than the wakeup interval, hence
     * the division lfsocMaxWakeupJitterUsec / profile->systemWakeupIntervalUsec
     * will fit in uint32_t.
     */
    lfsocMaxWakeupJitterUsec = (profile->maxAllowedJitterUsec < SYSTIMER_JITTER_USEC)
                                   ? 0
                                   : profile->maxAllowedJitterUsec - SYSTIMER_JITTER_USEC;
    ppmPerLfoscJitter        = ((uint32_t)lfsocMaxWakeupJitterUsec * 1000000) / (profile->systemWakeupIntervalUsec);

    /* Relax ppm requirement based on the maximum allowed jitter */
    ppmRequirement = profile->ppmRequirement + ppmPerLfoscJitter;

    /* The interval between two calibration wakeups is:
     * T_wakeup(n) = T_system/n
     * where T_system is the system wakeup interval in us and n is the number
     * of calibrations per system wakeup (including the system wakeup).
     * A model of the frequency error as a function of n is constructed with the
     * following error sources:
     * ppm_temp(n) = tempPpmPerSystemWakeup/n
     * ppm_rtn(n)  = ppmPerRtn/n (it is assumed there is only one RTN event per system wakeup)
     * ppm_rest(n) = baselinePpm
     *
     * The total error of this model is then:
     * ppm_total(n) = ppm_temp(n) + ppm_rtn(n) + ppm_rest(n)
     *
     * Requirement:
     * ppm_total(n) <= ppmRequirement
     * ppm_temp(n) + ppm_rtn(n) + ppm_rest(n) <= ppmRequirement
     * (tempPpmPerSystemWakeup/n) + (ppmPerRtn/n) + baselinePpm <= ppmRequirement, solve for n
     * n => (tempPpmPerSystemWakeup + ppmPerRtn)/(ppmRequirement - baselinePpm)
     *
     * The lowest value of n that satisfy this requirement is chosen by rounding
     * the right side of the inequality up to the nearest integer:
     * n = ceil((tempPpmPerSystemWakeup + ppmPerRtn)/(ppmRequirement - baselinePpm))
     *
     * This ceil function is implemented using integer arithmetic:
     * n = (tempPpmPerSystemWakeup + ppmPerRtn + (ppmRequirement - baselinePpm) - 1)/(ppmRequirement - baselinePpm)
     */
    n = (tempPpmPerSystemWakeup + ppmPerRtn + (ppmRequirement - baselinePpm) - 1) / (ppmRequirement - baselinePpm);

    /* Calculate calibration interval. Round up and add worst case LFOSC
     * inaccuracy to ensure the system does not wake up to perform an extra
     * calibration just before the system wakeup.
     */
    calibrationIntervalUs = (profile->systemWakeupIntervalUsec + (n - 1)) / n;
    calibrationIntervalUs = (uint64_t)calibrationIntervalUs * (1000000 + LFOSC_MAX_ABSOLUTE_PPM_ERROR) / 1000000;

    /* Convert calibration interval to number of 256 LF periods (rounded up) */
    calibrationInterval256LfPeriods = (PowerCC27XX_convertUsToLfPeriods(calibrationIntervalUs) + (256 - 1)) / 256;

    /* Cap calibration interval to the maximum value supported by the hardware */
    maxMeasurementPeriod256LfPeriods = (PMCTL_LFCAL_PER_MAX >> PMCTL_LFCAL_PER_S);
    calibrationInterval256LfPeriods  = (calibrationInterval256LfPeriods > maxMeasurementPeriod256LfPeriods)
                                           ? maxMeasurementPeriod256LfPeriods
                                           : calibrationInterval256LfPeriods;

    /* Protect read-modify-write and ensure the configured temperature
     * notifications matches the new calibration interval.
     */
    key = HwiP_disable();

    /* Set measurement period of LFCAL hardware */
    lfcalReg = HWREG(PMCTL_BASE + PMCTL_O_LFCAL);
    lfcalReg &= ~PMCTL_LFCAL_PER_M;
    lfcalReg |= calibrationInterval256LfPeriods << PMCTL_LFCAL_PER_S;
    HWREG(PMCTL_BASE + PMCTL_O_LFCAL) = lfcalReg;

    /* Register temperature notification based on the temperature used to
     * calculate the new calibration interval
     */
    PowerCC27XX_registerLfoscCompensationTemperatureNotification(currentTemperature);
    HwiP_restore(key);
}

/*
 *  ======== PowerLPF3_initLfoscCompensation ========
 */
void PowerLPF3_initLfoscCompensation(void)
{
    uintptr_t key;

    /* Enter critical section to make the function reentrant */
    key = HwiP_disable();

    /* Only initialize LFOSC compensation if it is not already initialized */
    if (PowerCC27XX_lfoscCompensationObj.state == PowerCC27XX_LFOSC_COMPENSATION_STATE_UNINITIALIZED)
    {
        /* Initialize temperature driver */
        Temperature_init();

        /* Convert measurement time to LF periods and cap value to the maximum
         * value supported by the hardware.
         */
        uint32_t measurementTimeLfPeriods    = PowerCC27XX_convertUsToLfPeriods(LFOSC_CALIBRATION_DURATION_US);
        uint32_t maxMeasurementTimeLfPeriods = (PMCTL_LFCAL_MEAS_MAX >> PMCTL_LFCAL_MEAS_S);
        measurementTimeLfPeriods             = (measurementTimeLfPeriods > maxMeasurementTimeLfPeriods)
                                                   ? maxMeasurementTimeLfPeriods
                                                   : measurementTimeLfPeriods;

        /* Set measurement time of LFCAL hardware */
        uint32_t lfcalReg = HWREG(PMCTL_BASE + PMCTL_O_LFCAL);
        lfcalReg &= ~PMCTL_LFCAL_MEAS_M;
        lfcalReg |= measurementTimeLfPeriods << PMCTL_LFCAL_MEAS_S;
        HWREG(PMCTL_BASE + PMCTL_O_LFCAL) = lfcalReg;

        /* Mark LFOSC compensation as initialized (disabled) */
        PowerCC27XX_lfoscCompensationObj.state = PowerCC27XX_LFOSC_COMPENSATION_STATE_DISABLED;
    }
    HwiP_restore(key);
}

/*
 *  ======== PowerLPF3_setLfoscCompensationProfile ========
 */
void PowerLPF3_setLfoscCompensationProfile(const PowerLPF3_LfoscCompensationProfile *profile)
{
    uintptr_t key;
    int16_t currentTemperature = Temperature_getTemperature();

    /* Enter critical section to make the function reentrant */
    key = HwiP_disable();

    /* Set the current profile */
    PowerCC27XX_lfoscCompensationObj.currentProfile = *profile;

    /* Only update the LFOSC compensation configuration, if compensation is
     * enabled.
     * This is done to prevent
     * PowerCC27XX_setLfoscCompensationConfiguration() from registering
     * temperature notifications while compensation is disabled.
     * If compensation is not enabled, the configuration will be set when
     * enabling compensation.
     */
    if (PowerCC27XX_lfoscCompensationObj.state == PowerCC27XX_LFOSC_COMPENSATION_STATE_ENABLED)
    {
        /* Update the LFOSC compensation configuration based on the new profile
         * and the current temperature.
         */
        PowerCC27XX_setLfoscCompensationConfiguration(&PowerCC27XX_lfoscCompensationObj.currentProfile,
                                                      currentTemperature);
    }

    HwiP_restore(key);
}

/*
 *  ======== PowerLPF3_enableLfoscCompensation ========
 */
void PowerLPF3_enableLfoscCompensation(void)
{
    uintptr_t key;

    /* Enter critical section to make the function reentrant */
    key = HwiP_disable();

    /* Only enable compensation if it is currently disabled */
    if (PowerCC27XX_lfoscCompensationObj.state == PowerCC27XX_LFOSC_COMPENSATION_STATE_DISABLED)
    {
        int16_t currentTemperature = Temperature_getTemperature();

        /* Set the LFOSC compensation configuration based on the current
         * temperature and the current profile.
         */
        PowerCC27XX_setLfoscCompensationConfiguration(&PowerCC27XX_lfoscCompensationObj.currentProfile,
                                                      currentTemperature);

        /* Enable LFCAL hardware */
        HWREG(PMCTL_BASE + PMCTL_O_HFXTCTL) |= PMCTL_HFXTCTL_LFCAL_EN;

        /* Update state */
        PowerCC27XX_lfoscCompensationObj.state = PowerCC27XX_LFOSC_COMPENSATION_STATE_ENABLED;
    }
    HwiP_restore(key);
}

/*
 *  ======== PowerLPF3_disableLfoscCompensation ========
 */
void PowerLPF3_disableLfoscCompensation(void)
{
    uintptr_t key;

    /* Enter critical section to make the function reentrant and to protect
     * read-modify-write of PMCTL_O_HFXTCTL.
     */
    key = HwiP_disable();

    /* Only disable compensation if it is currently enabled */
    if (PowerCC27XX_lfoscCompensationObj.state == PowerCC27XX_LFOSC_COMPENSATION_STATE_ENABLED)
    {
        /* Disable LFCAL hardware */
        HWREG(PMCTL_BASE + PMCTL_O_HFXTCTL) &= ~PMCTL_HFXTCTL_LFCAL_EN;

        /* Unregister temperature notifications */
        Temperature_unregisterNotify(&PowerCC27XX_lfoscCompensationObj.tempNotifyObj);

        /* Change state to DISABLED */
        PowerCC27XX_lfoscCompensationObj.state = PowerCC27XX_LFOSC_COMPENSATION_STATE_DISABLED;
    }
    HwiP_restore(key);
}