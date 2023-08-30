/*
 * Copyright (c) 2021-2023, Texas Instruments Incorporated
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
 *  ======== LRFCC23X0.c ========
 */

#include <stdint.h>
#include <stdlib.h>

#include <ti/drivers/rcl/LRF.h>
#include <ti/drivers/rcl/RCL_Scheduler.h>
#include <ti/drivers/rcl/RCL_Command.h>
#include <ti/log/Log.h>
#include <ti/drivers/dpl/HwiP.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/txf_regs.h)
#include DeviceFamily_constructPath(driverlib/rxf_regs.h)
#include DeviceFamily_constructPath(driverlib/dbell_regs.h)
#include DeviceFamily_constructPath(driverlib/pbe_regs.h)
#include DeviceFamily_constructPath(driverlib/mdm_regs.h)
#include DeviceFamily_constructPath(driverlib/mdm32_regs.h)
#include DeviceFamily_constructPath(driverlib/rfe_regs.h)
#include DeviceFamily_constructPath(driverlib/rfe32_regs.h)
#include DeviceFamily_constructPath(driverlib/pbe_common_ram_regs.h)
#include DeviceFamily_constructPath(driverlib/rfe_common_ram_regs.h)

static uint32_t LRF_findFoff(int32_t frequencyOffset, uint32_t invSynthFreq);
static void LRF_programShape(const LRF_TxShape *txShape, uint32_t deviation, uint32_t invSynthFreq);
static uint32_t LRF_findLog2Bde1(uint32_t demmisc3);
static uint32_t LRF_programPQ(uint32_t pllM, uint32_t preDivRatio);
static void LRF_programCMixN(int32_t rxIntFrequency, uint32_t invSynthFreq);
static void LRF_applyNewClockEnable(void);
static void LRF_applyTrim(const LRF_TrimDef *trimDef, const LRF_SwConfig *swConfig);
static void LRF_updateTrim(const LRF_TrimDef *trimDef, const LRF_SwConfig *swConfig);
static void LRF_setTrimCommon(const LRF_TrimDef *trimDef, const LRF_SwConfig *swConfig);
static void LRF_setTemperatureTrim(const LRF_TrimDef *trimDef);
static void LRF_temperatureCompensateTrim(const LRF_TrimDef *trimDef);
static uint32_t LRF_findExtTrim1TrimAdjustment(uint32_t temperatureDiff, uint32_t tempThreshFactor, uint32_t maxAdjustment);
static int32_t LRF_findExtTrim0TrimAdjustment(int32_t temperature, int32_t tempCompFactor, int32_t offset);
static uint32_t LRF_scaleFreqWithHFXTOffset(uint32_t frequency);

uint32_t swParamList[sizeof(LRF_SwParam)/sizeof(uint32_t)];
const size_t swParamListSz = sizeof(LRF_SwParam);

static struct {
    const LRF_TOPsmImage   *pbeLoaded;
    const LRF_TOPsmImage   *mceLoaded;
    const LRF_TOPsmImage   *rfeLoaded;
    uint16_t                phyFeatures;
    uint16_t                clockEnable[LRF_NUM_CLK_ENA];
    LRF_TxPowerTable_Entry  currentTxPower;
    LRF_TxPowerTable_Entry  rawTxPower;
} lrfPhyState = {0};

/* Required time from enabling refsys to synth programming */
#define LRF_REFSYS_ENABLE_TIME RCL_SCHEDULER_SYSTIM_US(30)

LRF_SetupResult LRF_setupRadio(const LRF_Config *lrfConfig, uint16_t phyFeatures, LRF_RadioState lrfState)
{
    enum {
        trimNoUpdate,
        trimPartialUpdate,
        trimFullUpdate
    } trimUpdate = trimNoUpdate;
    LRF_SetupResult result = SetupResult_Ok;

    if (lrfPhyState.pbeLoaded != lrfConfig->pbeImage || lrfState < RadioState_ImagesLoaded)
    {
        result = LRF_loadImage(lrfConfig->pbeImage, PBE_RAM_BASE_ADDR);
        lrfPhyState.pbeLoaded = lrfConfig->pbeImage;
    }
    if ((result == SetupResult_Ok) &&
        (lrfPhyState.mceLoaded != lrfConfig->mceImage || lrfState < RadioState_ImagesLoaded))
    {
        result = LRF_loadImage(lrfConfig->mceImage, MCE_RAM_BASE_ADDR);
        lrfPhyState.mceLoaded = lrfConfig->mceImage;
    }
    if ((result == SetupResult_Ok) &&
        (lrfPhyState.rfeLoaded != lrfConfig->rfeImage || lrfState < RadioState_ImagesLoaded))
    {
        result = LRF_loadImage(lrfConfig->rfeImage, RFE_RAM_BASE_ADDR);
        lrfPhyState.rfeLoaded = lrfConfig->rfeImage;
    }

    if ((result == SetupResult_Ok) && (lrfConfig->regConfigList != NULL))
    {
        LRF_ApplySettingsBase includeBase;

        if (lrfState < RadioState_Configured)
        {
            includeBase = LRF_ApplySettings_IncludeBase;
            Log_printf(RclCore, Log_INFO3, "Performing full setup");
            trimUpdate = trimFullUpdate;
        }
        else
        {
            includeBase = LRF_ApplySettings_NoBase;
            if (phyFeatures != lrfPhyState.phyFeatures)
            {
                Log_printf(RclCore, Log_INFO3, "Changing PHY features");
                trimUpdate = trimPartialUpdate;
            }
        }
        if (includeBase == LRF_ApplySettings_IncludeBase || phyFeatures != lrfPhyState.phyFeatures)
        {
            LRF_ApplySettingsState settingsState;
            /* Initialize setup state */
            lrfPhyState.phyFeatures = phyFeatures;
            LRF_initSettingsState(&settingsState, includeBase, phyFeatures);
            for (uint32_t i = 0; i < lrfConfig->regConfigList->numEntries; i++)
            {
                LRF_ConfigWord *config = lrfConfig->regConfigList->entries[i];

                result = LRF_applySettings(config, &settingsState, LRF_SETTINGS_BUFFER_UNLIMITED);

                if (result != SetupResult_Ok)
                {
                    break;
                }
            }
        }
        /* Invalidate RSSI value to cover the case in which no RX has run before. */
        S_RFE_RSSI = LRF_RSSI_INVALID;
    }

    if (result == SetupResult_Ok)
    {
        LRF_SwParam *swParam = (LRF_SwParam *) swParamList;

        if (swParam->swConfig == NULL)
        {
            result = SetupResult_ErrorSwConfig;
        }
        else
        {
            if (trimUpdate == trimFullUpdate)
            {
                LRF_applyTrim(swParam->trimDef, swParam->swConfig);
            }
            else if (trimUpdate == trimPartialUpdate)
            {
                LRF_updateTrim(swParam->trimDef, swParam->swConfig);
            }
            else
            {
                LRF_setTemperatureTrim(swParam->trimDef);
            }
        }
    }

    return result;
}

bool LRF_imagesNeedUpdate(const LRF_Config *lrfConfig)
{
    return ((lrfPhyState.pbeLoaded != lrfConfig->pbeImage && lrfConfig->pbeImage != NULL) ||
            (lrfPhyState.mceLoaded != lrfConfig->mceImage && lrfConfig->mceImage != NULL) ||
            (lrfPhyState.rfeLoaded != lrfConfig->rfeImage && lrfConfig->rfeImage != NULL));
}

static void LRF_applyTrim(const LRF_TrimDef *trimDef, const LRF_SwConfig *swConfig)
{
    if (trimDef != NULL)
    {
        S_RFE_PA0        |= trimDef->trim0.pa0;
        S_RFE_ATSTREFH   |= trimDef->trim0.atstRefH;

        S_RFE_LNA        |= trimDef->trim1.lna;
        S_RFE_IFAMPRFLDO |= trimDef->trim1.ifampRfLdo;

        S_RFE_DCOLDO0    |= trimDef->trim2.dcoLdo0;
        S_RFE_IFADCALDO  |= trimDef->trim2.ifadcAldo;
        S_RFE_IFADCDLDO  |= trimDef->trim2.ifadcDldo;

        /* DEMIQMC0 has no fields not to be trimmed */
        S_MDM_DEMIQMC0   =  trimDef->trim4.demIQMC0;

        /* Write trim to shadow registers */
        S_RFE_COMMON_RAM_IFAMPRFLDODEFAULT = S_RFE_IFAMPRFLDO & RFE_IFAMPRFLDO_TRIM_BM;

        LRF_setTrimCommon(trimDef, swConfig);
        LRF_temperatureCompensateTrim(trimDef);
    }
}

static void LRF_updateTrim(const LRF_TrimDef *trimDef, const LRF_SwConfig *swConfig)
{
    if (trimDef != NULL)
    {
        /* Remove trim fields from registers with BW variations */
        S_RFE_IFADCQUANT    &= ~(RFE_IFADCQUANT_QUANTTHR_BM);
        S_RFE_IFADC0        &= ~(RFE_IFADC0_AAFCAP_BM | RFE_IFADC0_INT2ADJ_BM | RFE_IFADC0_DITHEREN_BM | RFE_IFADC0_DITHERTRIM_BM);
        S_RFE_IFADC1        &= ~(RFE_IFADC1_TRIM_BM | RFE_IFADC1_NRZ_BM);
        S_RFE_IFADCLF       = 0; /* All fields are trimmed so everything needs to be cleared */
        S_RFE_IFAMPRFLDO    &= ~RFE_IFAMPRFLDO_AAFCAP_BM;

        LRF_setTrimCommon(trimDef, swConfig);
        LRF_setTemperatureTrim(trimDef);
    }
}

static void LRF_setTrimCommon(const LRF_TrimDef *trimDef, const LRF_SwConfig *swConfig)
{
    uint8_t bwIndex = 0;
    uint8_t bwIndexDither = 0;
    uint8_t revision = trimDef->revision;
    if (revision >= LRF_TRIM_MIN_VERSION_FULL_FEATURES)
    {
        bwIndex = swConfig->bwIndex;
        bwIndexDither = swConfig->bwIndexDither;
    }
    S_RFE_IFADCQUANT |= trimDef->trimVariant[bwIndex].ifadcQuant;
    S_RFE_IFADC0     |= trimDef->trimVariant[bwIndex].ifadc0;
    S_RFE_IFADC1     |= trimDef->trimVariant[bwIndex].ifadc1;
    S_RFE_IFADCLF    |= trimDef->trimVariant[bwIndex].ifadclf;

    if (revision >= LRF_TRIM_MIN_VERSION_FULL_FEATURES)
    {
        /* Set AAFCAP according to BW */
        S_RFE_IFAMPRFLDO |= trimDef->trim4.ifamprfldo[bwIndex];

        if (bwIndexDither != bwIndex)
        {
            /* Use different setting of dither settings compared to the rest of the IFADC0 register. */
            S_RFE_IFADC0 = (S_RFE_IFADC0 & ~(RFE_IFADC0_DITHEREN_BM | RFE_IFADC0_DITHERTRIM_BM)) |
                (trimDef->trimVariant[bwIndexDither].ifadc0 & (RFE_IFADC0_DITHEREN_BM | RFE_IFADC0_DITHERTRIM_BM));
        }
    }
}

static void LRF_setTemperatureTrim(const LRF_TrimDef *trimDef)
{
    if (trimDef != NULL)
    {
        /* Remove trim fields from registers with temperature compensation */
        S_RFE_COMMON_RAM_DIVLDOF    &= ~RFE_COMMON_RAM_DIVLDOF_VOUTTRIM_BM;
        S_RFE_COMMON_RAM_DIVLDOI    &= ~RFE_COMMON_RAM_DIVLDOI_VOUTTRIM_BM;
        S_RFE_TDCLDO                &= ~RFE_TDCLDO_VOUTTRIM_BM;
        S_RFE_DCO                   &= ~RFE_DCO_TAILRESTRIM_BM;

        LRF_temperatureCompensateTrim(trimDef);
    }
}

/* Number of shifts in temperature compensation for fields in lrfdrfeExtTrim1 */
#define LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP 4U
/* Adjustment per step of DIVLDO at lowest temperature */
#define LRF_DIVLDO_LOW_TEMP_ADJ_FACTOR 10U
/* Adjustment per step of DIVLDO at highest temperature */
#define LRF_DIVLDO_HIGH_TEMP_ADJ_FACTOR 10U
/* Adjustment per step of TDCLDO at lowest temperature */
#define LRF_TDCLDO_LOW_TEMP_ADJ_FACTOR 10U
/* Adjustment per step of TDCLDO at highest temperature */
#define LRF_TDCLDO_HIGH_TEMP_ADJ_FACTOR 10U
/* Adjustment per step of RTRIM at lowest temperature */
#define LRF_RTRIM_LOW_TEMP_ADJ_FACTOR 1U
/* Adjustment per step of RTRIM at highest temperature */
#define LRF_RTRIM_HIGH_TEMP_ADJ_FACTOR 1U
/* Maximum allowed value (saturation value) for RTRIM */
#define LRF_RTRIM_MAX 12U

/* Number of shifts in temperature compensation for fields in lrfdrfeExtTrim0 */
#define LRF_EXTTRIM0_TEMPERATURE_SCALE_EXP 7

/* Lowest temperature supported */
#define LRF_TEMPERATURE_MIN (-40)
/* Highest temperature supported */
#define LRF_TEMPERATURE_MAX 125
/* Nominal temperature for offset definitions */
#define LRF_TEMPERATURE_NOM 25

/* Bit masks and positions in SPARE0 and SPARE1 */
/* RFE_SPARE0: Fast AGC only */
#define RFE_SPARE0_LOW_GAIN_BM  0x000F
#define RFE_SPARE0_LOW_GAIN     0
#define RFE_SPARE0_HIGH_GAIN_BM 0x00F0
#define RFE_SPARE0_HIGH_GAIN    4
/* RFE_SPARE1: Fast AGC: Threshold. Standard AGC: Magnitude */
/* These fields have the same position in the register */
#define RFE_SPARE1_AGC_VALUE_BM 0x000FF
#define RFE_SPARE1_AGC_VALUE    0

static void LRF_temperatureCompensateTrim(const LRF_TrimDef *trimDef)
{
    uint32_t divLdoTempOffset = 0;
    uint32_t tdcLdoTempOffset = 0;
    uint32_t rtrimTempOffset = 0;
    int32_t rssiTempOffset = 0;
    int32_t agcValOffset = 0;
    int32_t agcHighGainOffset = 0;
    int32_t agcLowGainOffset = 0;

    if (trimDef->revision >= LRF_TRIM_MIN_VERSION_FULL_FEATURES)
    {
        int32_t temperature = hal_get_temperature();

        LRF_Trim_tempLdoRtrim tempLdoRtrim = trimDef->trim3.fields.lrfdrfeExtTrim1.tempLdoRtrim;

        int32_t tempThreshLow = LRF_TEMPERATURE_MIN + tempLdoRtrim.tThrl * (1 << LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP);
        int32_t tempThreshHigh = LRF_TEMPERATURE_MAX - tempLdoRtrim.tThrh * (1 << LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP);

        /* Adjust values for extreme temperatures */
        if (temperature < tempThreshLow)
        {
            uint32_t temperatureDiff = tempThreshLow - temperature;
            divLdoTempOffset = LRF_findExtTrim1TrimAdjustment(temperatureDiff, tempLdoRtrim.tThrl,
                LRF_DIVLDO_LOW_TEMP_ADJ_FACTOR * tempLdoRtrim.divLdoMinOffset);
            tdcLdoTempOffset = LRF_findExtTrim1TrimAdjustment(temperatureDiff, tempLdoRtrim.tThrl,
                LRF_TDCLDO_LOW_TEMP_ADJ_FACTOR * tempLdoRtrim.tdcLdoMinOffset);
            rtrimTempOffset = LRF_findExtTrim1TrimAdjustment(temperatureDiff, tempLdoRtrim.tThrl,
                LRF_RTRIM_LOW_TEMP_ADJ_FACTOR * tempLdoRtrim.rtrimMinOffset);
        }
        else if (temperature > tempThreshHigh)
        {
            uint32_t temperatureDiff = temperature - tempThreshHigh;
            divLdoTempOffset = LRF_findExtTrim1TrimAdjustment(temperatureDiff, tempLdoRtrim.tThrh,
                LRF_DIVLDO_HIGH_TEMP_ADJ_FACTOR * tempLdoRtrim.divLdoMaxOffset);
            tdcLdoTempOffset = LRF_findExtTrim1TrimAdjustment(temperatureDiff, tempLdoRtrim.tThrh,
                LRF_TDCLDO_HIGH_TEMP_ADJ_FACTOR * tempLdoRtrim.tdcLdoMaxOffset);
            rtrimTempOffset = LRF_findExtTrim1TrimAdjustment(temperatureDiff, tempLdoRtrim.tThrh,
                LRF_RTRIM_HIGH_TEMP_ADJ_FACTOR * tempLdoRtrim.rtrimMaxOffset);
        }

        rssiTempOffset = LRF_findExtTrim0TrimAdjustment(temperature, trimDef->trim3.fields.lrfdrfeExtTrim0.rssiTcomp, 0);

        if (((S_RFE_COMMON_RAM_AGCINFO & RFE_COMMON_RAM_AGCINFO_MODE_BM) >> RFE_COMMON_RAM_AGCINFO_MODE) == RFE_COMMON_RAM_AGCINFO_MODE_FAST)
        {
            /* Fast AGC */
            agcValOffset =LRF_findExtTrim0TrimAdjustment(temperature, trimDef->trim3.fields.lrfdrfeExtTrim0.agcThrTcomp,
                                                         trimDef->trim3.fields.lrfdrfeExtTrim0.agcThrOffset);
            agcHighGainOffset = trimDef->trim3.fields.lrfdrfeExtTrim0.highGainOffset;
            agcLowGainOffset = trimDef->trim3.fields.lrfdrfeExtTrim0.lowGainOffset;
        }
        else
        {
            /* Standard AGC */
            agcValOffset = LRF_findExtTrim0TrimAdjustment(temperature, trimDef->trim3.fields.lrfdrfeExtTrim0.magnTcomp,
                                                          trimDef->trim3.fields.lrfdrfeExtTrim0.magnOffset);
        }
    }

    uint32_t divLdoVoutTrim = trimDef->trim1.fields.divLdo.voutTrim;

    /* Most significant bit is inverted */
    divLdoVoutTrim ^= 0x40;

    divLdoVoutTrim += divLdoTempOffset;

    /* Saturate at maximum value */
    if (divLdoVoutTrim > RFE_DIVLDO_VOUTTRIM_ONES)
    {
        divLdoVoutTrim = RFE_DIVLDO_VOUTTRIM_ONES;
    }
    /* Write back with most signigicant bit inverted back */
    S_RFE_COMMON_RAM_DIVLDOF |= ((divLdoVoutTrim ^ 0x40) << RFE_COMMON_RAM_DIVLDOF_VOUTTRIM);

    /* Add offset to initial value */
    divLdoVoutTrim += S_RFE_COMMON_RAM_DIVLDOIOFF;

    /* Saturate at maximum value */
    if (divLdoVoutTrim > RFE_DIVLDO_VOUTTRIM_ONES)
    {
        divLdoVoutTrim = RFE_DIVLDO_VOUTTRIM_ONES;
    }
    /* Write back with most signigicant bit inverted back */
    S_RFE_COMMON_RAM_DIVLDOI |= ((divLdoVoutTrim ^ 0x40) << RFE_COMMON_RAM_DIVLDOI_VOUTTRIM);

    uint32_t tdcLdoVoutTrim = trimDef->trim1.fields.tdcLdo.voutTrim;

    if (tdcLdoTempOffset > 0)
    {
        /* Most significant bit is inverted */
        tdcLdoVoutTrim ^= 0x40;

        tdcLdoVoutTrim += tdcLdoTempOffset;

        /* Saturate at maximum value */
        if (tdcLdoVoutTrim > RFE_TDCLDO_VOUTTRIM_ONES)
        {
            tdcLdoVoutTrim = RFE_TDCLDO_VOUTTRIM_ONES;
        }

        /* Invert back */
        tdcLdoVoutTrim ^= 0x40;
    }
    /* Write back */
    S_RFE_TDCLDO |= (tdcLdoVoutTrim << RFE_TDCLDO_VOUTTRIM);

    uint32_t rtrim = trimDef->trim2.fields.dco.tailresTrim;

    /* Add offset from temperature compensation */
    rtrim += rtrimTempOffset;

    /* Add offset from PHY */
    rtrim += S_RFE_COMMON_RAM_RTRIMOFF;

    /* Saturate */
    if (rtrim > LRF_RTRIM_MAX)
    {
        rtrim = LRF_RTRIM_MAX;
    }

    /* Ensure it is not smaller than minimum from PHY */
    uint32_t minRtrim = S_RFE_COMMON_RAM_RTRIMMIN;
    if (rtrim < minRtrim)
    {
        rtrim = minRtrim;
    }

    /* Write into register */
    S_RFE_DCO |= rtrim << RFE_DCO_TAILRESTRIM;

    /* Get RSSI offset from FCFG */
    int32_t rssiOffset = trimDef->trim4.rssiOffset;

    /* RCL-335: Some devices (State D) have an error in the programmed RSSI offset */
    if (trimDef->revision == LRF_TRIM_VERSION_RSSIOFFSET_ISSUE)
    {
        if (rssiOffset <= LRF_TRIM_LIMIT_RSSIOFFSET_ISSUE)
        {
            rssiOffset += LRF_TRIM_CORRECTION_RSSIOFFSET_ISSUE;
        }
    }

    /* Apply temperature compensation */
    rssiOffset += rssiTempOffset;

    /* Apply PHY specific offset */
    rssiOffset += S_RFE_COMMON_RAM_PHYRSSIOFFSET;

    /* Store in HW register */
    S_RFE_RSSIOFFSET = rssiOffset;

    if (((S_RFE_COMMON_RAM_AGCINFO & RFE_COMMON_RAM_AGCINFO_MODE_BM) >> RFE_COMMON_RAM_AGCINFO_MODE) == RFE_COMMON_RAM_AGCINFO_MODE_FAST)
    {
        uint32_t spare0Val = S_RFE_COMMON_RAM_SPARE0SHADOW;
        if (agcHighGainOffset != 0 || agcLowGainOffset != 0)
        {
            int32_t lowGain = (spare0Val & RFE_SPARE0_LOW_GAIN_BM) >> RFE_SPARE0_LOW_GAIN;
            int32_t highGain = (spare0Val & RFE_SPARE0_HIGH_GAIN_BM) >> RFE_SPARE0_HIGH_GAIN;
            if (agcLowGainOffset != 0)
            {
                lowGain += agcLowGainOffset;
                if (lowGain < 0)
                {
                    lowGain = 0;
                }
                if (lowGain > (RFE_SPARE0_LOW_GAIN_BM >> RFE_SPARE0_LOW_GAIN))
                {
                    lowGain = (RFE_SPARE0_LOW_GAIN_BM >> RFE_SPARE0_LOW_GAIN);
                }
            }
            if (agcHighGainOffset != 0)
            {
                highGain += agcHighGainOffset;
                if (highGain < 0)
                {
                    highGain = 0;
                }
                if (highGain > (RFE_SPARE0_HIGH_GAIN_BM >> RFE_SPARE0_HIGH_GAIN))
                {
                    highGain = (RFE_SPARE0_HIGH_GAIN_BM >> RFE_SPARE0_HIGH_GAIN);
                }
            }
            spare0Val = (spare0Val & ~(RFE_SPARE0_LOW_GAIN_BM | RFE_SPARE0_HIGH_GAIN_BM)) |
                (lowGain << RFE_SPARE0_LOW_GAIN) | (highGain << RFE_SPARE0_HIGH_GAIN);
        }
        S_RFE_SPARE0 = spare0Val;
    }

    uint32_t spare1Val = S_RFE_COMMON_RAM_SPARE1SHADOW;
    if (agcValOffset != 0)
    {
        int32_t agcVal = (spare1Val & RFE_SPARE1_AGC_VALUE_BM) >> RFE_SPARE1_AGC_VALUE;
        agcVal += agcValOffset;
        if (agcVal < 0)
        {
            agcVal = 0;
        }
        if (agcVal > (RFE_SPARE1_AGC_VALUE_BM >> RFE_SPARE1_AGC_VALUE))
        {
            agcVal = (RFE_SPARE1_AGC_VALUE_BM >> RFE_SPARE1_AGC_VALUE);
        }
        spare1Val = (spare1Val & ~RFE_SPARE1_AGC_VALUE_BM) | (agcVal << RFE_SPARE1_AGC_VALUE);
    }
    S_RFE_SPARE1 = spare1Val;
}

/* Represent 1/3 with the approximation LRF_ONE_THIRD_MANTISSA * 2^(-LRF_ONE_THIRD_NEG_EXP) */
#define LRF_ONE_THIRD_MANTISSA 21845U /* (round(1/3 * 2^16)) */
#define LRF_ONE_THIRD_NEG_EXP  16
/* Calculate temperature compensation for the fields in lrfdrfeExtTrim1 */
/* temperatureDiff: absolute difference from calculated temperature threshold */
/* tempThreshFactor: Temperature Threshold field as defined in the trim spec:
   The temperature threshold is given by (125 - tempThreshFactor * 2^k) for high temperatures and
   (-40C + tempThreshFactor * 2^k) for low temperatures, where k = LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP. */
/* maxAdjustment: The adjustment to apply at the extreme temperature */
/* Return: Adjustment to add to value */
static uint32_t LRF_findExtTrim1TrimAdjustment(uint32_t temperatureDiff, uint32_t tempThreshFactor, uint32_t maxAdjustment)
{
    uint32_t adjustment;
    /* Calculate adjustment = round((temperatureDiff * maxAdjustment) / (tempThreshFactor * 2^LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP)) */
    switch (tempThreshFactor)
    {
        case 0:
        default:
            /* tempThreshFactor = 0:
               No temperatures will be in the range for adjustment */
            adjustment = 0;
            break;
        case 1:
            /* tempThreshFactor = 1:
               adjustment = round((temperatureDiff * maxAdjustment) / (1 * 2^LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP)) */
            adjustment = ((temperatureDiff * maxAdjustment) + (1 << (LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP - 1))) >> LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP;
            break;
        case 2:
            /* tempThreshFactor = 2:
               adjustment = round((temperatureDiff * maxAdjustment) / (2 * 2^LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP)) */
            adjustment = ((temperatureDiff * maxAdjustment) + (1 << LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP)) >> (LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP + 1);
            break;
        case 3:
            /* tempThreshFactor = 3:
               adjustment = round((temperatureDiff * maxAdjustment) / (3 * 2^LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP))
               Use approximation with multiplication to avoid performing division */
            adjustment = ((temperatureDiff * maxAdjustment * LRF_ONE_THIRD_MANTISSA) + (1 << (LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP + LRF_ONE_THIRD_NEG_EXP - 1)))
                >> (LRF_EXTTRIM1_TEMPERATURE_SCALE_EXP + LRF_ONE_THIRD_NEG_EXP);
            break;
    }
    return adjustment;
}

/* Calculate temperature compensation for the fields in lrfdrfeExtTrim0 */
/* temperature: temperature (degrees C) */
/* tempCompFactor: Temperature compensation coefficient used to find offset from the formula ((temperature - 25) * tempCompFactor) / 128 */
/* offset: Absolute offset to apply independent of temperature  */
/* Return: Adjustment to add to value */
static int32_t LRF_findExtTrim0TrimAdjustment(int32_t temperature, int32_t tempCompFactor, int32_t offset)
{
    return (((temperature - LRF_TEMPERATURE_NOM) * tempCompFactor) >> LRF_EXTTRIM0_TEMPERATURE_SCALE_EXP) + offset;
}

void LRF_enable(void)
{
    /* Set MSGBOX register to 0 */
    S_PBE_COMMON_RAM_MSGBOX = 0;

    /* Initialize and enable PBE TOPsm and FIFO */
    S_PBE_INIT   = ((1 << PBE_INIT_MDMF)   |
                    (1 << PBE_INIT_TOPSM)      );
    S_PBE_ENABLE = ((1 << PBE_ENABLE_MDMF) |
                    (1 << PBE_ENABLE_TOPSM)    );

    /* Initialize and enable MCE TOPsm and FIFO */
    S_MDM_INIT   = ((1 << MDM_INIT_TXRXFIFO)  |
                    (1 << MDM_INIT_TOPSM)      );
    S_MDM_ENABLE = ((1 << MDM_ENABLE_TXRXFIFO)|
                    (1 << MDM_ENABLE_TOPSM)    );

    /* Initialize and enable RFE TOPsm */
    S_RFE_INIT   = (1 << RFE_INIT_TOPSM);
    S_RFE_ENABLE = (1 << RFE_ENABLE_TOPSM);
}

void LRF_disable(void)
{
    /* Request PBE powerdown */
    S_PBE_PDREQ = PBE_PDREQ_TOPSMPDREQ_BM;
    /* Disable all PBE modules */
    S_PBE_ENABLE = 0;
    /* Stop powerdown request */
    S_PBE_PDREQ = 0;

    /* Request MCE powerdown */
    S_MDM_PDREQ = MDM_PDREQ_TOPSMPDREQ_BM;
    /* Disable all MDM modules */
    S_MDM_ENABLE = 0;
    /* Stop powerdown request */
    S_MDM_PDREQ = 0;

    /* Request RFE powerdown */
    S_RFE_PDREQ = RFE_PDREQ_TOPSMPDREQ_BM;
    /* Disable all RFE modules */
    S_RFE_ENABLE = 0;
    /* Stop powerdown request */
    S_RFE_PDREQ = 0;
}

void LRF_waitForTopsmReady(void)
{
    /* Make sure PBE is finished booting */
    /* This poll should be quick as long as the TOPsms have been reset and enabled */
    while (S_PBE_COMMON_RAM_MSGBOX == 0)
    {
    }
}

uint32_t LRF_prepareRxFifo(void)
{
    uint32_t fifoSize;
    /* Reset RXFIFO */
    S_PBE_FCMD = PBE_FCMD_DATA_RXFIFO_RESET;
    /* Set up RXFIFO without auto commit or deallocate */
    S_PBE_FCFG0 &= ~(PBE_FCFG0_RXADEAL_BM | PBE_FCFG0_RXACOM_BM);
    /* Read writable bytes, which is the FIFO size */
    fifoSize = S_PBE_RXFWRITABLE;
    /* Write SRP to 0. This sets no available space for writing; to be updated by
       calling LRF_setRxFifoEffSz() */
    S_PBE_RXFSRP = 0;

    return fifoSize;
}

uint32_t LRF_prepareTxFifo(void)
{
    /* Reset TXFIFO */
    S_PBE_FCMD = PBE_FCMD_DATA_TXFIFO_RESET;
    /* Set up TXFIFO with auto commit, without auto deallocate */
    uint32_t fcfg0 = S_PBE_FCFG0;
    fcfg0 &= ~PBE_FCFG0_TXADEAL_BM;
    fcfg0 |=  PBE_FCFG0_TXACOM_BM;
    S_PBE_FCFG0 = fcfg0;
    /* Return writable bytes, which is the FIFO size */
    return S_PBE_TXFWRITABLE;
}


uint32_t LRF_peekRxFifo(int32_t offset)
{
    int32_t index = S_PBE_RXFRP + offset;
    int32_t fifosz = ((S_PBE_FCFG4 & PBE_FCFG4_RXSIZE_BM) >> PBE_FCFG4_RXSIZE) << 2;
    if (index >= fifosz)
    {
        index -= fifosz;
    }
    return *(uint32_t *) (BUF_RAM_BASE_ADDR + (S_PBE_FCFG3 << 2) + index);
}



uint32_t LRF_peekTxFifo(int32_t offset)
{
    int32_t index = S_PBE_TXFRP + offset;
    int32_t fifosz = ((S_PBE_FCFG2 & PBE_FCFG2_TXSIZE_BM) >> PBE_FCFG2_TXSIZE) << 2;
    if (index >= fifosz)
    {
        index -= fifosz;
    }
    return *(uint32_t *) (BUF_RAM_BASE_ADDR + (S_PBE_FCFG1 << 2) + index);
}

uint8_t *LRF_getTxFifoWrAddr(int32_t offset)
{
    int32_t index = S_PBE_TXFWP + offset;
    int32_t fifosz = ((S_PBE_FCFG2 & PBE_FCFG2_TXSIZE_BM) >> PBE_FCFG2_TXSIZE) << 2;
    if (index >= fifosz)
    {
        index -= fifosz;
    }
    return (uint8_t *) (BUF_RAM_BASE_ADDR + (S_PBE_FCFG1 << 2) + index);
}

void LRF_skipTxFifoWords(uint32_t wordLength)
{
    int32_t index = S_PBE_TXFRP + (wordLength * 4);
    int32_t fifosz = ((S_PBE_FCFG2 & PBE_FCFG2_TXSIZE_BM) >> PBE_FCFG2_TXSIZE) << 2;
    if (index >= fifosz)
    {
        index -= fifosz;
    }
    S_PBE_TXFRP = index;
}

void LRF_discardRxFifoWords(uint32_t wordLength)
{
    int32_t index = S_PBE_RXFRP + (wordLength * 4);
    int32_t fifosz = ((S_PBE_FCFG4 & PBE_FCFG4_RXSIZE_BM) >> PBE_FCFG4_RXSIZE) << 2;
    if (index >= fifosz)
    {
        index -= fifosz;
    }
    S_PBE_RXFRP = index;
    /* Deallocate RX FIFO to free up the space */
    S_PBE_FCMD = PBE_FCMD_DATA_RXFIFO_DEALLOC;
}

void LRF_readRxFifoWords(uint32_t *data32, uint32_t wordLength)
{
    for (uint32_t i = 0; i < wordLength; i++) {
        *data32++ = S_RXF_RXD;
    }
}

void LRF_writeTxFifoWords(const uint32_t *data32, uint32_t wordLength)
{
    for (uint32_t i = 0; i < wordLength; i++) {
        S_TXF_TXD = *data32++;
    }
}


void LRF_setRxFifoEffSz(uint32_t maxSz)
{
    uint32_t fifoSz = ((S_PBE_FCFG4 & PBE_FCFG4_RXSIZE_BM) >> PBE_FCFG4_RXSIZE) << 2;
    if (maxSz >= fifoSz)
    {
        /* Deallocate RX FIFO to get no additional FIFO restriction */
        S_PBE_FCMD = PBE_FCMD_DATA_RXFIFO_DEALLOC;
    }
    else {
        /* A limit of 0 can't be achieved through manipulating SRP. For all practical purposes, a limit of 1 has the same effect. */
        if (maxSz == 0)
        {
            maxSz = 1;
        }

        uint32_t newSrp = S_PBE_RXFRP + maxSz;
        if (newSrp >= fifoSz)
        {
            newSrp -= fifoSz;
        }
        /* The new SRP value will always be a write forward in the FIFO */
        /* Do not write if the value is the same as before, as this will be
           interpreted by the FIFO HW as going a full round forward */
        if (S_PBE_RXFSRP != newSrp)
        {
            S_PBE_RXFSRP = newSrp;
        }
    }
}

/* (FXTALINVL + (FXTALINVH << 16)) = round(2^67/48e6) */
#define FXTALINVL 0x00001E52U
#define FXTALINVH 0x02CBD3F0U
/* (fXtalInv.word[0] + (fXtalInv.word[1] << 16)) = round(2^67/fXtal), where fXtal is clock frequency in Hz */
/* Only 16 bits are used of word[0] */
LRF_DoubleWord fXtalInv =
{
    .word = {FXTALINVL, FXTALINVH},
};

static uint32_t LRF_findPllM(uint32_t frequency, uint32_t prediv)
{
    /* Find 2^51 / fref = 2^51 * prediv / fxtal */
    uint32_t frefInv = ((fXtalInv.word[0] * prediv) >> 16) + (fXtalInv.word[1] * prediv);

    uint32_t pllM;
    /* Find pllM = frequency / fref, encoded as <12.18u> */
    /* First, find pllM = frequency / fref * 2^(51-32) */
    pllM = (frefInv >> 16) * (frequency >> 16);
    uint32_t tmpPllM;
    tmpPllM = ((frefInv >> 16) * (frequency & 0xFFFFU)) >> 1;
    tmpPllM += ((frefInv & 0xFFFFU) * (frequency >> 16)) >> 1;
    tmpPllM += (1U << 14);
    tmpPllM >>= 15;
    pllM += tmpPllM;

    /* Divide by 2 with rounding to get pllM = frequency / fref * 2^18 */
    pllM += 1;
    pllM >>= 1;

    return pllM;
}

static uint32_t countLeadingZeros(uint16_t value)
{
   int numZeros = 0;
   if (value >= 0x0100) {
      value >>= 8;
   }
   else {
      numZeros += 8;
   }
   if (value >= 0x10) {
      value >>= 4;
   }
   else {
      numZeros += 4;
   }
   if (value >= 0x04) {
      value >>= 2;
   }
   else {
      numZeros += 2;
   }
   if (value >= 0x02) {
       /* No need to shift down value since this is the last step */
   }
   else {
      numZeros += 1;
   }
   return numZeros;
}

static uint32_t LRF_findCalM(uint32_t frequency, uint32_t prediv)
{
    /* Find 2^47 / fref = 2^47 * prediv / fxtal */
    uint32_t frefInv = (fXtalInv.word[1] >> 4) * prediv;
    /* Round to 2^31 * prediv / fxtal */
    frefInv += 1 << 15;
    frefInv >>= 16;

    uint32_t calM;
    /* Find calM = frequency / fref (no fractional bits) */
    /* First, find calM = frequency / fref * 2^(31-15)) */
    calM = frefInv * ((frequency + (1 << 14)) >> 15);

    /* Divide by 2^16 with rounding to get calM = frequency / fref */
    calM += 1 << 15;
    calM >>= 16;

    return calM;
}

/* invSynthFreq = 2^47 / synthFrequency */
static uint32_t LRF_findFoff(int32_t frequencyOffset, uint32_t invSynthFreq)
{
    uint32_t absFrequencyOffset;
    int32_t fOffRes;
    if (frequencyOffset == 0)
    {
        return 0;
    }
    else {
        if (frequencyOffset < 0)
        {
            absFrequencyOffset = -frequencyOffset;
        }
        else
        {
            absFrequencyOffset = frequencyOffset;
        }
        /* Calculate 2^41 * abs(frequencyOffset) / synthFrequency */
        absFrequencyOffset = (absFrequencyOffset + (1U << 5)) >> 6;
        absFrequencyOffset *= invSynthFreq;
        /* Round to 2^21 * abs(frequencyOffset) / synthFrequency */
        absFrequencyOffset = (absFrequencyOffset + (1U << 19)) >> 20;
        /* Re-intruduce sign */
        if (frequencyOffset < 0)
        {
            fOffRes = -absFrequencyOffset;
        }
        else
        {
            fOffRes = absFrequencyOffset;
        }

        return (((uint32_t)fOffRes) & RFE_MOD1_FOFF_BM);
    }
}

#define NUM_TX_FILTER_TAPS 24
/* deviation in Hz */
/* invSynthFreq = 2^51 / synthFrequency */
static void LRF_programShape(const LRF_TxShape *txShape, uint32_t deviation, uint32_t invSynthFreq)
{
    /* If txShape is NULL, do not program shape, but instead leave the values programmed as part of setup */
    if (txShape != NULL)
    {
        union {
            uint8_t  b[NUM_TX_FILTER_TAPS];
            uint32_t w[NUM_TX_FILTER_TAPS/4];
        } filterCoeff;
        /* Find deviation * 2^29/fs * 2^10 */
        uint32_t deviationFactor1 = ((deviation >> 12) * invSynthFreq) +
            (((deviation & 0x0FFFU) * invSynthFreq) >> 12);
        /* Find deviation * 2^29/fs * scale / 2^16 */
        uint32_t scale = txShape->scale;
        uint32_t deviationFactor2 = ((((deviationFactor1 >> 15) * scale) >> 1) +
                                     (((deviationFactor1 & 0x7FFF) * scale) >> 16) + (1 << 4)) >> 5;
        /* Find shapeGain and scaling */
        int32_t shapeGain = 8 - countLeadingZeros(deviationFactor2 >> 11);
        if (shapeGain < 0)
        {
            shapeGain = 0;
        }
        uint32_t startCoeff = NUM_TX_FILTER_TAPS - txShape->numCoeff;
        for (uint32_t i = 0; i < startCoeff; i++)
        {
            filterCoeff.b[i] = 0;
        }
        for (uint32_t i = 0; i < NUM_TX_FILTER_TAPS - startCoeff; i++)
        {
            filterCoeff.b[i + startCoeff] =
                ((deviationFactor2 * txShape->coeff[i]) + (1 << (18 + shapeGain))) >> (19 + shapeGain);
        }

        for (int i = 0; i <  NUM_TX_FILTER_TAPS / 4; i++)
        {
            *(SP_RFE32_DTX1_DTX0 + i) = filterCoeff.w[i];
        }
        if (shapeGain > 3)
        {
            /* TODO: Scale by adjusting the symbol mapping */
            shapeGain = 3;
        }
        S_RFE_MOD0 = (S_RFE_MOD0 & ~RFE_MOD0_SHPGAIN_BM) | (shapeGain << RFE_MOD0_SHPGAIN);
    }
}

static uint32_t LRF_findLog2Bde1(uint32_t demmisc3)
{
    uint32_t log2Bde1;
    if ((demmisc3 & MDM_DEMMISC3_BDE1FILTMODE_BM) != 0)
    {
        log2Bde1 = 0;
    }
    else
    {
        log2Bde1 = (demmisc3 & MDM_DEMMISC3_BDE1NUMSTAGES_BM) >> MDM_DEMMISC3_BDE1NUMSTAGES;
    }
    return log2Bde1;
}

/* Calculate P as rateWord * preDivRatio * bde1 * bde2 * pdifDecim * 9 * 2^4
   Calculate Q as pllM * pre
   Normalize Q to 28 bits and do the same normalization to P */
/* Multiplication factor from P formula */
#define P_FACTOR 9
/* Shift from P formula */
#define P_SHIFT 4
/* Right shift of PLL M to allow 32-bit calculation */
#define Q_MAGN_SHIFT 6
/* Number of bits in P and Q */
#define FRAC_NUM_BITS 28
/* Number of extra bits in an uint32_t compared to the P and Q HW registers */
#define FRAC_EXTRA_BITS (32 - FRAC_NUM_BITS)
static uint32_t LRF_programPQ(uint32_t pllM, uint32_t preDivRatio)
{
    bool roundingError = false;
    uint32_t rateWord = (S_MDM_BAUD << 5);
    rateWord |= ((S_MDM_BAUDPRE & MDM_BAUDPRE_EXTRATEWORD_BM) >> MDM_BAUDPRE_EXTRATEWORD);
    uint32_t pre = (S_MDM_BAUDPRE & MDM_BAUDPRE_PRESCALER_BM);
    uint32_t demmisc3 = S_MDM_DEMMISC3;
    uint32_t log2Bde1 = LRF_findLog2Bde1(demmisc3);

    uint32_t bde2 = (demmisc3 & MDM_DEMMISC3_BDE2DECRATIO_BM) >> MDM_DEMMISC3_BDE2DECRATIO;
    uint32_t log2PdifDecim = (demmisc3 & MDM_DEMMISC3_PDIFDECIM_BM) >> MDM_DEMMISC3_PDIFDECIM;
    int32_t leftShiftP;

    leftShiftP = log2Bde1 + log2PdifDecim + P_SHIFT;

    uint32_t demFracP = rateWord * preDivRatio * bde2;
    if (demFracP > (uint32_t)((1ULL << 32) / P_FACTOR))
    {
        if ((demFracP & 1) != 0)
        {
            roundingError = true;
        }
        demFracP >>= 1;
        leftShiftP -= 1;
    }
    demFracP *= P_FACTOR;

    /* Preliminary calculation to find scaling factor - round PLLM upwards to ensure no overflow
       in final calculation */
    uint32_t demFracQ = ((pllM + ((1 << Q_MAGN_SHIFT) - 1)) >> Q_MAGN_SHIFT) * pre;
    uint32_t num0Q = countLeadingZeros(demFracQ >> 16);

    int32_t pllMShift = Q_MAGN_SHIFT + FRAC_EXTRA_BITS - num0Q;
    uint32_t pllMRounded;
    if (pllMShift <= 0)
    {
        pllMRounded = pllM;
        demFracQ = pllM * pre;
        int32_t leftShiftQ = num0Q - FRAC_EXTRA_BITS;
        leftShiftP += leftShiftQ;
        /* leftShiftQ is sure to be positive since pllMShift <= 0 */
        demFracQ <<= leftShiftQ;
    }
    else
    {
        /* Scale PLLM to allow Q to fit */
        pllMRounded = (pllM + (1U << (pllMShift - 1))) >> pllMShift;
        demFracQ = pllMRounded * pre;
        /* Multiply PLLM back (rounding is now applied) */
        pllMRounded <<= pllMShift;
        leftShiftP -= pllMShift;
    }

    if (leftShiftP >= 0)
    {
        demFracP <<= leftShiftP;
    }
    else
    {
        /* Check if right shift of P introduces rounding error */
        if ((demFracP & ((1 << -leftShiftP) - 1)) != 0)
        {
            roundingError = true;
        }
        demFracP >>= -leftShiftP;
    }

    if (demFracP >= demFracQ)
    {
        Log_printf(RclCore, Log_ERROR, "Error: resampler fraction greater than 1; demodulator will not work");
    }
    if (roundingError)
    {
        Log_printf(RclCore, Log_WARN, "Rounding error in fractional resampler");
    }
    if (pllMRounded != pllM)
    {
        Log_printf(RclCore, Log_INFO5, "PLLM rounded from %08X to %08X to fit in fractional resampler", pllM, pllMRounded);
    }

    S_MDM32_DEMFRAC1_DEMFRAC0 = demFracP;
    S_MDM32_DEMFRAC3_DEMFRAC2 = demFracQ;

    return pllMRounded;
}

/* invSynthFreq = 2^47 / synthFrequency */
static void LRF_programCMixN(int32_t rxIntFrequency, uint32_t invSynthFreq)
{
    /* Calculate n = f_if/f_pll*24*12*bde1*1024
                   = f_if/f_pll * 2^(15 + log2Bde1) * 9 */
    uint32_t absRxIntFrequency;
    if (rxIntFrequency < 0)
    {
        absRxIntFrequency = -rxIntFrequency;
    }
    else
    {
        absRxIntFrequency = rxIntFrequency;
    }

    absRxIntFrequency = (absRxIntFrequency + (1 << 5)) >> 6;
    /* Find 2^41 * abs(rxIntFrequency) / synthFrequency */
    uint32_t cMixN = absRxIntFrequency * invSynthFreq;
    /* Find 2^37 * abs(rxIntFrequency) / synthFrequency * 9 */
    cMixN = ((cMixN + (1 << 3)) >> 4) * 9;

    /* Find 2^(15 + log2Bde1) * abs(rxIntFrequency) / synthFrequency * 9 */
    uint32_t rightShift = (37 - 15) - LRF_findLog2Bde1(S_MDM_DEMMISC3);
    cMixN = (cMixN + (1 << (rightShift - 1))) >> rightShift;

    int32_t signedCMixN;
    /* Use inverse sign */
    if (rxIntFrequency > 0)
    {
        signedCMixN = -cMixN;
    }
    else
    {
        signedCMixN = cMixN;
    }

    cMixN = (uint32_t)(signedCMixN & MDM_DEMMISC0_CMIXN_BM);
    S_MDM_DEMMISC0 = cMixN;
    S_MDM_SPARE3 = cMixN;
}

void LRF_programFrequency(uint32_t frequency, bool tx)
{
    uint32_t synthFrequency;
    LRF_SwParam *swParam = (LRF_SwParam *) swParamList;
    const LRF_SwConfig *swConfig = swParam->swConfig;

    /* Find frequency corrected for IF. Normally, the result will be the same for tx and rx, and
       that is a prerequisite for switching without recalibration. */
    if (tx)
    {
        synthFrequency = frequency - swConfig->txFrequencyOffset;
    }
    else
    {
        synthFrequency = frequency - swConfig->rxFrequencyOffset
            - swConfig->rxIntFrequency;
    }

    /* Compensate desired frequency for temperature-dependent offset in HFXT, if any */
    synthFrequency = LRF_scaleFreqWithHFXTOffset(synthFrequency);

    /* Frequency divided by 2^16, rounded */
    uint32_t frequencyDiv2_16 = (synthFrequency + (1 << 15)) >> 16;

    /* Start calculating 2^47/frequency (approximated as 2^31/(synthFrequency/2^16)) */
    S_RFE32_DIVIDEND = 1U << 31;
    S_RFE32_DIVISOR = frequencyDiv2_16;

    /* Write approximate freuency to RFE */
    S_RFE_COMMON_RAM_K5 = frequencyDiv2_16;

    /* Find setting for coarse and mid calibration */
    uint32_t precalSetting = S_RFE32_PRE3_PRE2;
    uint32_t coarsePrecal = (precalSetting & RFE32_PRE3_PRE2_CRSCALDIV_BM) >> RFE32_PRE3_PRE2_CRSCALDIV;
    uint32_t midPrecal = (precalSetting &
                          (RFE32_PRE3_PRE2_MIDCALDIVMSB_BM | RFE32_PRE3_PRE2_MIDCALDIVLSB_BM))
        >> RFE_PRE2_MIDCALDIVLSB;

    uint32_t calMCoarse = LRF_findCalM(synthFrequency, coarsePrecal);
    uint32_t calMMid;
    if (coarsePrecal == midPrecal)
    {
        calMMid = calMCoarse;
    }
    else
    {
        calMMid = LRF_findCalM(synthFrequency, midPrecal);
    }
    S_RFE32_CALMMID_CALMCRS = (calMCoarse << RFE32_CALMMID_CALMCRS_CALMCRS_VAL) |
        (calMMid << RFE32_CALMMID_CALMCRS_CALMMID_VAL);

    precalSetting = S_RFE32_PRE1_PRE0;
    uint32_t precal0 = (precalSetting & RFE32_PRE1_PRE0_PLLDIV0_BM) >> RFE32_PRE1_PRE0_PLLDIV0;
    uint32_t precal1 = (precalSetting & RFE32_PRE1_PRE0_PLLDIV1_BM) >> RFE32_PRE1_PRE0_PLLDIV1;

    uint32_t pllM1 = LRF_findPllM(synthFrequency, precal1);

    pllM1 =  LRF_programPQ(pllM1, precal1);

    uint32_t pllM0;
    if (precal1 == precal0)
    {
        pllM0 = pllM1;
    }
    else
    {
        pllM0 = LRF_findPllM(synthFrequency, precal0);
    }

    S_RFE32_PLLM0 = (pllM0 << RFE32_PLLM0_VAL);
    S_RFE32_PLLM1 = (pllM1 << RFE32_PLLM1_VAL);

    /* Read out division result to find invSynthFreq */
    while ((S_RFE_DIVSTA & RFE_DIVSTA_STAT_BM) != 0)
    {
    }
    uint32_t invSynthFreq = S_RFE32_QUOTIENT;

    /* Calculate intermediate frequencies */
    S_RFE_COMMON_RAM_RXIF = LRF_findFoff(swConfig->rxFrequencyOffset, invSynthFreq);
    S_RFE_COMMON_RAM_TXIF = LRF_findFoff(swConfig->txFrequencyOffset, invSynthFreq);

    /* Calculate CMIXN */
    LRF_programCMixN(swConfig->rxIntFrequency, invSynthFreq);

    LRF_programShape(swConfig->txShape, swConfig->modFrequencyDeviation,
                     invSynthFreq << 4);
}

uint32_t LRF_enableSynthRefsys(void)
{
    uint32_t earliestStartTime;

    /* Enable REFSYS if not already done. If it is enabled now, we need to make sure that start
       time is late enough */
    uint32_t atstref = S_RFE32_ATSTREF;
    if ((atstref & RFE32_ATSTREF_BIAS_BM) == 0)
    {
        /* Bias not already enabled - enable it now */
        S_RFE32_ATSTREF = atstref | RFE32_ATSTREF_BIAS_BM;
        /* Set earliest start time of synth to some later time */
        earliestStartTime = LRF_REFSYS_ENABLE_TIME;
    }
    else
    {
        /* No restriction on start time */
        earliestStartTime = 0;
    }
    /* Add current time */
    earliestStartTime += RCL_Scheduler_getCurrentTime();

    return earliestStartTime;
}

void LRF_disableSynthRefsys(void)
{
    S_RFE32_ATSTREF &= ~RFE32_ATSTREF_BIAS_BM;
}


void LRF_setClockEnable(uint16_t mask, uint8_t entryNumber)
{
    if (entryNumber < LRF_NUM_CLK_ENA)
    {
        lrfPhyState.clockEnable[entryNumber] |= mask;
    }
    LRF_applyNewClockEnable();
}

void LRF_clearClockEnable(uint16_t mask, uint8_t entryNumber)
{
    if (entryNumber < LRF_NUM_CLK_ENA)
    {
        lrfPhyState.clockEnable[entryNumber] &= ~mask;
    }
    LRF_applyNewClockEnable();
}

static void LRF_applyNewClockEnable(void)
{
    uint16_t clkEna = 0;
    uintptr_t key = HwiP_disable();
    for (int i = 0; i < LRF_NUM_CLK_ENA; i++)
    {
        clkEna |= lrfPhyState.clockEnable[i];
    }

    hal_set_clkctl(clkEna);
    HwiP_restore(key);
}

void LRF_rclEnableRadioClocks(void)
{
    LRF_setClockEnable(DBELL_CLKCTL_BUFRAM_BM |
        DBELL_CLKCTL_DSBRAM_BM |
        DBELL_CLKCTL_RFERAM_BM |
        DBELL_CLKCTL_MCERAM_BM |
        DBELL_CLKCTL_PBERAM_BM |
        DBELL_CLKCTL_RFE_BM |
        DBELL_CLKCTL_MDM_BM |
        DBELL_CLKCTL_PBE_BM,
        LRF_CLK_ENA_RCL);
}

void LRF_rclDisableRadioClocks(void)
{
    LRF_clearClockEnable(DBELL_CLKCTL_BUFRAM_BM |
        DBELL_CLKCTL_DSBRAM_BM |
        DBELL_CLKCTL_RFERAM_BM |
        DBELL_CLKCTL_MCERAM_BM |
        DBELL_CLKCTL_PBERAM_BM |
        DBELL_CLKCTL_RFE_BM |
        DBELL_CLKCTL_MDM_BM |
        DBELL_CLKCTL_PBE_BM,
        LRF_CLK_ENA_RCL);
}

int8_t LRF_readRssi(void)
{
    return (int8_t)(S_RFE_RSSI & RFE_RSSI_VAL_BM);
}

void LRF_setRawTxPower(uint32_t value, uint32_t temperatureCoefficient)
{
    lrfPhyState.rawTxPower.value.rawValue = value;
    lrfPhyState.rawTxPower.tempCoeff = temperatureCoefficient;
    lrfPhyState.rawTxPower.power = LRF_TxPower_Use_Raw;
}

LRF_TxPowerTable_Entry LRF_getRawTxPower(void)
{
    if (lrfPhyState.rawTxPower.power.rawValue == LRF_TxPower_Use_Raw.rawValue)
    {
        /* Raw TX power has been set */
        return lrfPhyState.rawTxPower;
    }
    else
    {
        /* Error: Raw TX power was never set */
        return LRF_TxPowerEntry_INVALID_VALUE;
    }
}

/* Avoid IB = 0 as it effectively turns the PA off */
#define RFE_PA0_IB_MIN_USED 1

void LRF_programTemperatureCompensatedTxPower(void)
{
    LRF_TxPowerTable_Entry txPowerEntry = lrfPhyState.currentTxPower;
    uint8_t tempCoeff = txPowerEntry.tempCoeff;
    if (tempCoeff != 0)
    {
        int32_t ib = txPowerEntry.value.ib;
        int32_t temperature = hal_get_temperature();
        /* Linear adjustment of IB field as a function of temperature, scaled
         * by the coefficient for the given setting */
        ib += ((temperature - LRF_TXPOWER_REFERENCE_TEMPERATURE) * (int32_t) tempCoeff)
            / LRF_TXPOWER_TEMPERATURE_SCALING;
        /* Saturate IB */
        if (ib < (int32_t) RFE_PA0_IB_MIN_USED)
        {
            ib = RFE_PA0_IB_MIN_USED;
        }
        if (ib > (int32_t) RFE_PA0_IB_MAX)
        {
            ib = RFE_PA0_IB_MAX;
        }
        txPowerEntry.value.ib = ib;
    }
    /* Program into RFE shadow register for PA power */
    S_RFE_SPARE5 = txPowerEntry.value.rawValue;
}

LRF_TxPowerResult LRF_programTxPower(LRF_TxPowerTable_Index powerLevel)
{
    if (powerLevel.rawValue != LRF_TxPower_None.rawValue)
    {
        LRF_SwParam *swParam = (LRF_SwParam *) swParamList;
        LRF_TxPowerTable_Entry txPowerEntry = LRF_TxPowerTable_findValue(swParam->txPowerTable, powerLevel);
        if (txPowerEntry.value.rawValue != LRF_TxPowerTable_INVALID_VALUE.rawValue)
        {
            lrfPhyState.currentTxPower = txPowerEntry;
            LRF_programTemperatureCompensatedTxPower();
        }
        else
        {
            return TxPowerResult_Error;
        }
    }
    return TxPowerResult_Ok;
}

static uint32_t LRF_scaleFreqWithHFXTOffset(uint32_t frequency)
{
    /* Get HFXT ratio from HFTRACKCTL register. This will have been
     * updated by the power driver if compensation is enabled and the temperature has drifted beyond the threshold.
     */
    uint32_t ratio = hal_get_hfxt_ratio();

    /* If temperature compensation is disabled, or temperature has not drifted,
     * the ratio will have its reset-value of 0x400000. In this case, do not perform scaling of input frequency
     * to save computational cost.
     * Rationale:
     * ratio = 24 MHz / (2 * HFXT_freq) * 2^24 ==> HFXT_freq = 24 MHz / ratio * 2^23
     * (ref: CKMD.HFTRACKCTL.RATIO register description)
     * Nominal HFXT frequency is 48 MHz
     *
     * frequency_out = frequency_in * HFXT_nominal_freq / HFXT_freq
     *               = frequency_in * 48 MHz / (24 MHz / ratio * 2^23)
     * frequency_out = frequency_in * ratio * 2^-22
     *
     * The method below is a computationally cost-effective way to calculate the scaled result.
     * Instead of performing 64-bit multiplication and shifting, the multiplier and multiplicand are divided into
     * half-words which are multiplied, added, and shifted appropriately.
     */
    if (ratio != hal_get_hfxt_ratio_default())
    {
        uint32_t ah = frequency >> 16;      /* Multiplier high half-word */
        uint32_t al = frequency & 0xFFFF;   /* Multiplier low half-word */

        uint32_t bh = ratio >> 16;          /* Multiplicand high half-word */
        uint32_t bl = ratio & 0xFFFF;       /* Multiplicand low half-word */

        /* Perform standard long multiplication where each "digit" is a half-word
         * https://en.wikipedia.org/wiki/Multiplication_algorithm
         * The rounding error will be maximum 1 Hz in this calculation.
         * frequency * ratio >> 22 = [ah al] * [bh bl] >> 22
         * [ah al] * [bh bl] >> 22 = ([bl * al] + (([bl * ah] + [bh * al]) << 16) + ([bh * ah]) << 32) >> 22
         *                         = (([bl * ah] + [bh * al]) >> 6) + ([bh * ah]) << 10)
         */
        frequency = ((bl*ah + bh*al + ((bl*al) >> 16)) >> 6) + ((bh*ah) << 10);
    }

    return frequency;
}
