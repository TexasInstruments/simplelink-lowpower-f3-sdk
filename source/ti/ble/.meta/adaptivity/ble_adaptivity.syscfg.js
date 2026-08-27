/*
 * Copyright (c) 2025 Texas Instruments Incorporated - http://www.ti.com
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
 *
 */

/*
 *  ======== ble_adaptivity.syscfg.js ========
 */

"use strict";

const Docs = system.getScript("/ti/ble/adaptivity/ble_adaptivity_docs.js");

const adaptivityConfigItems = [
    {
        name: "AdaptivityMode",
        displayName: "Utilization Based Mode",
        default: "SDAA_ADAPTIVE_MODE",
        longDescription: Docs.AdaptivityModeLongDescription,
        onChange: onAdaptivityConfigurationChange,
        hidden: true,
        options: [
            {
                displayName: "Adaptive Mode",
                name: "SDAA_ADAPTIVE_MODE",
                description: "Assesses the TX channel when TX usage exceeds 10% of the maximum TX usage."
            },
            {
                displayName: "Fixed Mode",
                name: "SDAA_FIXED_MODE",
                description: "In fixed mode, adaptivity is always enabled whenever the TX power is above 10dBm"
            }
        ],
    },
    {
        name: "utilizationFactor",
        displayName: "Utilization Factor (%)",
        default: 10,
        hidden: true,
    },
    {
        name: "externalTxPowerGain",
        displayName: "External TX Power Gain (dBm)",
        longDescription: Docs.externalTxPowerGainLongDescription,
        default: 0,
        hidden: true,
    },
    {
        name: "externalRxPowerGain",
        displayName: "External RX Power Gain (dBm)",
        longDescription: Docs.externalRxPowerGainLongDescription,
        default: 0,
        hidden: true,
    },
    {
        name: "fixedObservTime",
        displayName: "Fixed Observation Time",
        longDescription: Docs.fixedObservTimeLongDescription,
        default: true,
        hidden: true,
        onChange: onDynamicObservTimeChange
    },
    {
        name: "observationTime",
        displayName: "Observation Time Value ( ms )",
        longDescription: Docs.observationTimeLongDescription,
        default: 1,
        hidden: true,
    },
    {
        name: "rxWindowDuration",
        displayName: "Rx Window Duration",
        longDescription: Docs.rxWindowDurationLongDescription,
        default: 12,
        hidden: true,
    },
    {
        name: "blockingChannelTime",
        displayName: "Blocking Channel Time (sec)",
        longDescription: Docs.blockingChannelTimeLongDescription,
        default: 1,
        hidden: true,
    },
];

/*
 *  ======== onAdaptivityConfigurationChange ========
 * Show or hide utilizationFactor based on selected adaptivity mode.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onAdaptivityConfigurationChange(inst, ui)
{
    if (inst.AdaptivityMode == "SDAA_ADAPTIVE_MODE")
    {
        ui.utilizationFactor.hidden = false;
    }
    else if (inst.AdaptivityMode == "SDAA_FIXED_MODE")
    {
        ui.utilizationFactor.hidden = true;
        inst.utilizationFactor = 0;
    }
}

/*
 *  ======== onDynamicObservTimeChange ========
 * Show or hide observationTime based on fixedObservTime toggle.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onDynamicObservTimeChange(inst, ui)
{
    ui.observationTime.hidden = !inst.fixedObservTime;
}

/*
 * ======== validate ========
 * Validate adaptivity configuration
 *
 * @param inst       - Module instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    if (inst.externalTxPowerGain < -20 || inst.externalTxPowerGain > 20)
    {
        validation.logError("The value must be between -20 and 20 dBm.", inst, "externalTxPowerGain");
    }

    if (inst.externalRxPowerGain < -20 || inst.externalRxPowerGain > 20)
    {
        validation.logError("The value must be between -20 and 20 Gdb.", inst, "externalRxPowerGain");
    }

    if(inst.rxWindowDuration < 12 || inst.rxWindowDuration > 100)
    {
        validation.logError("The value must be >=12 or <=100", inst, "rxWindowDuration");
    }

    if(inst.utilizationFactor < 0 || inst.utilizationFactor > 100)
    {
        validation.logError("The value must be >=0 or <=100", inst, "utilizationFactor");
    }

    if(inst.blockingChannelTime < 0 || inst.blockingChannelTime > 255)
    {
        validation.logError("The value must be >=0 or <=255", inst, "blockingChannelTime");
    }

    if(inst.observationTime < 1 || inst.observationTime > 3000)
    {
        validation.logError("The value must be >=1 or <=3000", inst, "observationTime");
    }
}

/*
 *  ======== getOpts ========
 * Returns compiler defines for adaptivity features.
 * Called only when inst.adaptivity is true.
 */
function getOpts(mod)
{
    const inst = mod.$static;
    let result = [];

    if(inst.adaptivity)
    {
        result.push("-DSDAA_ENABLE");
        result.push("-D" + inst.AdaptivityMode);
        result.push("-DSDAA_MAX_THRESHOLD=" + inst.utilizationFactor);
        result.push("-DSDAA_TX_POWER_GAIN=" + inst.externalTxPowerGain);
        result.push("-DSDAA_RX_POWER_GAIN=" + inst.externalRxPowerGain);
        result.push("-DSDAA_RX_WINDOW_DURATION=" + inst.rxWindowDuration);
        result.push("-DSDAA_MAX_BLOCKED_CHANNEL_TIME=" + inst.blockingChannelTime);
        result.push("-DSDAA_CONST_OBSERV_TIME=" + (inst.fixedObservTime ? 1 : 0));
        result.push("-DSDAA_OBSERVATION_TIME=" + inst.observationTime);
    }

    return result;
}

exports = {
    adaptivityConfigItems: adaptivityConfigItems,
    validate: validate,
    getOpts: getOpts,
};
