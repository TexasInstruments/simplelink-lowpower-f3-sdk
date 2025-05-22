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
 *  ======== ble_cs.syscfg.js ========
 */

"use strict";

// Get general long descriptions
const Docs = system.getScript("/ti/ble/channel_sounding/ble_cs_docs.js");

// Get common Script
const Common = system.getScript("/ti/ble/ble_common.js");

const config = {
    name: "channelSoundingConfig",
    displayName: "Channel Sounding Configuration",
    description: "Configure BLE Channel Sounding Settings",
    config: [
        {
            name: "hideChannelSoundingGroup",
            default: true,
            hidden: true
        },
        {
            name: "numAntennas",
            displayName: "Number of Antennas",
            default: 1,
            hidden: true,
            description: "Set the number of antennas",
            longDescription: "This setting allows you to specify the number of antennas of the device.",
            options: [
                { name: 1, displayName: "1 Antenna" },
                { name: 2, displayName: "2 Antennas" },
                { name: 3, displayName: "3 Antennas" },
                { name: 4, displayName: "4 Antennas" }
            ],
            onChange: onNumAntennasChange,
        },
        {
            name: "antennasMuxValues",
            displayName: "Antennas Muxing Bitmap",
            displayFormat: "hex",
            default: 0x0,
            hidden: true,
            description: "Set the antennas muxing bitmap",
            longDescription: Docs.antennasMuxValuesLongDescription
        },
        {
            name: "csStepsResultsFormat",
            displayName: "CS Steps Results Format",
            default: 1,
            hidden: true,
            options: [
                { name: 0, displayName: "HCI" },
                { name: 1, displayName: "Custom" }
            ],
            description: "Format for Channel Sounding steps results",
        },
        {
            name: "csMeasureDistance",
            displayName: "Channel Sounding Measure Distance",
            hidden: true,
            default: false,
            description: "Enable Channel Sounding Measure Distance feature",
            onChange: onCsMeasureDistanceChange,
        },
        {
            name: "csMeasureResultsMode",
            displayName: "Channel Sounding Results Mode",
            default: 0,
            hidden: true,
            options: [
                { name: 0, displayName: "Distance" },
                { name: 1, displayName: "Distance + Raw Results" }
            ],
            description: "Determines the results mode for Channel Sounding - raise only distance events or both distance and raw results events",
        }
    ]
};

/*
 *  ======== onNumAntennasChange ========
 * Handles the change of number of antennas.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onNumAntennasChange(inst, ui)
{
    // Set the antennas muxing values to be 0 by default
    inst.antennasMuxValues = 0x0;

    // Show the muxing value option only when the number of antennas is greater than 1
    ui.antennasMuxValues.hidden = inst.numAntennas === 1;
}

/*
 *  ======== onCsMeasureDistanceChange ========
 * Handles the change of Measure Distance checkbox
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onCsMeasureDistanceChange(inst, ui)
{
    // Set the CS Measure Distance Results Mode to 'Distance' by default
    inst.csMeasureResultsMode = 0;

    // Show the CS Measure Distance Results Mode option only when the Measure Distance is enabled
    ui.csMeasureResultsMode.hidden = inst.csMeasureDistance === false;
}

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - General instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    // Throw a "Preview" warning to Channel Sounding when enabled
    if (inst.channelSounding)
    {
        validation.logWarning("Channel Sounding preview (Not yet certified)", inst, "channelSounding");
    }

    if(inst.numAntennas > 1)
    {
        validation.logWarning("In order to enable multiple antennas, please make sure to enable PBEGPO2 and PBEGPO3 inside TI Drivers -> RCL Observables -> signals", inst, "numAntennas");

        // Throw an error if antennas muxing values has been set to 0xFF (0xFF means no configuration)
        if(inst.antennasMuxValues < 0x0 || inst.antennasMuxValues >= 0xFF)
        {
            validation.logError("Antennas muxing values bitmap should be between 0x0 to 0xFC", inst, "antennasMuxValues");
        }
    }
}

/*
 *  ======== moduleInstances ========
 *  Determines what modules are added as non-static submodules
 *
 *  @param inst  - Module instance containing the config that changed
 *  @returns     - Array containing dependency modules
 */
function moduleInstances(inst)
{
    let dependencyModule = [];

    // Add ti/driver/APU module only if channelSounding is enabled
    // and csMeasureDistance is enabled
    if (inst.channelSounding && inst.csMeasureDistance)
    {
        dependencyModule.push(
        {
            name            : 'APULPF3',
            displayName     : 'APULPF3',
            moduleName      : '/ti/drivers/APU',
            group           : "channelSoundingConfig",
            collapsed       : true,
            hidden          : true
        });
    }

    return(dependencyModule);
}

/*
 *  ======== getOpts ========
 */
function getOpts(mod)
{
    let inst = mod.$static;
    let result = [];

    // Add csMeasureDistance related defines only if channelSounding is enabled
    // and csMeasureDistance is enabled
    if (inst.channelSounding)
    {
        inst.numAntennas && result.push("-DNUM_ANTENNAS="+inst.numAntennas);
        (inst.numAntennas > 1) && (inst.antennasMuxValues >= 0) && result.push("-DANT_MUX_VALUES=0x" + inst.antennasMuxValues.toString(16).toUpperCase());

        if(inst.csMeasureDistance)
        {
            if(inst.csMeasureResultsMode == 0)
                (inst.csMeasureDistance == true) && result.push("-DCS_MEASURE_DISTANCE=0");
            else
                (inst.csMeasureDistance == true) && result.push("-DCS_MEASURE_DISTANCE=1");
        }
    }

    return result;
}

/*
 * ======== getLibs ========
 * Contribute libraries to linker command file
 *
 * @param inst  - Module instance containing the config that changed
 * @returns     - Object containing the name of component, array of dependent
 *                components, and array of library names
 */
function getLibs(inst)
{
    let libs = [];

    // Add APU lib only if channelSounding is enabled
    // and csMeasureDistance is enabled
    if (inst.$static.channelSounding && inst.$static.csMeasureDistance)
    {
        let GenLibs = system.getScript("/ti/utils/build/GenLibs.syscfg.js");
        let toolchain = GenLibs.getToolchainDir();
        const devFamily = Common.device2DeviceFamily(system.deviceData.deviceId);

        if(devFamily == "DeviceFamily_CC27XX")
        {
            libs.push(`ti/ble/app_util/cs_ranging/lib/${toolchain}/m33f/blecsranging_cc27xx.a`);
        }
    }

    return(libs);
}

/*
 *  ======== exports ========
 *  Export the BLE General Configuration module
 */
exports = {
    config: config,
    validate: validate,
    moduleInstances: moduleInstances,
    getOpts: getOpts,
    getLibs: getLibs
};
