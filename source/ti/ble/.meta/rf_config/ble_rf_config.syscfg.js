/*
 * Copyright (c) 2018 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble_rf_config.syscfg.js ========
 */

"use strict";

// Get rf_config long descriptions
const Docs = system.getScript("/ti/ble/rf_config/ble_rf_config_docs.js");

// Get common Script
const Common = system.getScript("/ti/ble/ble_common.js");

// RadioConfig module scripts
const CommonRadioConfig = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");
const RfDesign = CommonRadioConfig.getScript("rfdesign");

const config = {
    name: "bleRadioConfig",
    displayName: "Radio",
    description: "Configure BLE Radio Settings",
    config: [
        {
            name: "rfDesign",
            displayName: "Based On RF Design",
            description: "Select which RF Design to use as a template",
            default: Common.getLaunchPadName(),
        },
        {
            name: "defaultTxPower",
            displayName: "Default Tx Power Value",
            deprecated: true,
            default: "0",
        },
        {
            name: "defaultTxPowerValue",
            displayName: "Default Tx Power Value",
            default: "0",
            description: "This is the Tx Power value the BLE stack will use",
            options: (inst) => { return getPaTableValues(inst.rfDesign); }
        }
    ]
}

/*
 * ======== getPaTableValues ========
 * Returns the tx power values options for the current device
 *
 * @param rfDesign - the selected device
 *
 * @returns - a list with the valid pa levels from the tableOptions
 */
function getPaTableValues(rfDesign)
{
    const frequency = 2400;

    return RfDesign.getTxPowerOptions(frequency, false);
}

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - BLE instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    // Validate the value is part of the options
    let validOptions = inst.$module.$configByName.defaultTxPowerValue.options(inst);
    let isValid = validOptions.find((option) => {
        if(option.name == inst.defaultTxPowerValue)
            return true;
        });

    if(!isValid)
    {
        validation.logError("Selected option is invalid, please select a valid option", inst, "defaultTxPowerValue");
    }

    // Get the RF Design module
    const rfDesign = system.modules["/ti/devices/radioconfig/rfdesign"].$static;

    // inst.rfDesign configurable value should always be equal to
    // the value of the rfDesign configurable in rfdesign module
    if(inst.rfDesign !== rfDesign.rfDesign)
    {
        validation.logError(`Must match ${system.getReference(rfDesign,
            "rfDesign")} in the RF Design Module`, inst, "rfDesign");
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
    const dependencyModule = [];
    let args;
    let adcNoiseArgs;
    let modulePath;
    let bleCsHpArgs;

    // Get the board default rf settings
    const radioSettings = {};
    const adcNoiseSettings = {
        symGenMethod: "Custom",
        phyAbbr: true
    };
    const bleCsHpSettings = {

        symGenMethod: "Custom",
        phyAbbr: true
    };
    const csSupported = Common.isChannelSoundingSupported();

    modulePath = "/ti/devices/radioconfig/phy_groups/ble";
    args = {
        $name: "RF_BLE_Setting",
        phyType: "ble",
        codeExportConfig: radioSettings,
        paramVisibility: false,
        permission: "ReadOnly"
    }
    adcNoiseArgs = {
        $name: "Adc_noise",
        phyType: "adc_noise",
        codeExportConfig: adcNoiseSettings,
        paramVisibility: false,
        permission: "ReadOnly"
    }
    bleCsHpArgs = {
        $name: "Ble_CS",
        phyType: "ble_cs_hp",
        codeExportConfig: bleCsHpSettings,
        paramVisibility: false,
        permission: "ReadOnly"
    }

    dependencyModule.push({
        name: "radioConfig",
        group: "bleRadioConfig",
        displayName: "BLE Radio Configuration",
        moduleName: modulePath,
        collapsed: true,
        args: args
    });

    dependencyModule.push({
        name: "adcNoiseConfig",
        group: "bleRadioConfig",
        displayName: "ADC Noise Configuration",
        moduleName: modulePath,
        collapsed: true,
        args: adcNoiseArgs
    });

    if(csSupported)
    {
        dependencyModule.push({
            name: "bleCsConfig",
            group: "bleRadioConfig",
            displayName: "BLE Channel Sounding Configuration",
            moduleName: modulePath,
            collapsed: true,
            args: bleCsHpArgs
        });
    }

    return(dependencyModule);
}

/*
 *  ======== exports ========
 *  Export the BLE RF Settings Configuration
 */
exports = {
    config: config,
    validate: validate,
    moduleInstances: moduleInstances
};