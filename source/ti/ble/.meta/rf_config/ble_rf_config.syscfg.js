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

// Get the rfDesign options according to the device
const rfDesignOptions = getRfDesignOptions(system.deviceData.deviceId);

const config = {
    name: "bleRadioConfig",
    displayName: "Radio",
    description: "Configure BLE Radio Settings",
    config: [
        {
            name: "rfDesign",
            displayName: "Based On RF Design",
            description: "Select which RF Design to use as a template",
            options: rfDesignOptions,
            default: rfDesignOptions ? rfDesignOptions[0].name : ""
        },
        {
            // RF Front End Settings
            // Note: The use of these values completely depends on how the PCB is laid out.
            //       Please see Device Package and Evaluation Module (EM) Board below.
            name: "frontEndMode",
            displayName: "Front End Mode",
            default: "RF_FE_DIFFERENTIAL",
            deprecated: true,
            options: [
                {
                    displayName: "Differential",
                    name: "RF_FE_DIFFERENTIAL"
                },
                {
                    displayName: "Single Ended RFP",
                    name: "RF_FE_SINGLE_ENDED_RFP"
                },
                {
                    displayName: "Single Ended RFN",
                    name: "RF_FE_SINGLE_ENDED_RFN"
                },
                {
                    displayName: "Antenna Diversity RFP First",
                    name: "RF_FE_ANT_DIVERSITY_RFP_FIRST"
                },
                {
                    displayName: "Antenna Diversity RFN First",
                    name: "RF_FE_ANT_DIVERSITY_RFN_FIRST"
                },
                {
                    displayName: "Single Ended RFP External Pins",
                    name: "RF_FE_SINGLE_ENDED_RFP_EXT_PINS"
                },
                {
                    displayName: "Single Ended RFN External Pins",
                    name: "RF_FE_SINGLE_ENDED_RFN_EXT_PINS"
                }
            ]
        },
        {
            name: "biasConfiguration",
            displayName: "Bias Configuration",
            default: "RF_FE_INT_BIAS",
            deprecated: true,
            options: [
                {
                    displayName: "Internal BIAS",
                    name: "RF_FE_INT_BIAS"
                },
                {
                    displayName: "External BIAS",
                    name: "RF_FE_EXT_BIAS"
                }
            ]
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
        },
        {
            name: "thorPg",
            displayName: "Thor PG version",
            default: 2,
            options: [
                {
                    displayName: "1",
                    name: 1
                },
                {
                    displayName: "2",
                    name: 2
                }
            ],
            description: "Thor pg version",
            hidden: true
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
    let currentOptions = [];
    // Get the device PA table levels
    const txPowerTableType = Common.getRadioScript(rfDesign,
                             system.deviceData.deviceId).radioConfigParams.paExport;
    let isHighPA = txPowerTableType == "combined"? true : false;

    if(isHighPA)
    {
        // Get the options list from the rfDesign module
        currentOptions = RfDesign.getTxPowerOptions(frequency, isHighPA);
    }

    currentOptions = currentOptions.concat(RfDesign.getTxPowerOptions(frequency, false));


    return currentOptions;
}
/*
 * ======== getRfDesignOptions ========
 * Generates an array of SRFStudio compatible rfDesign options based on device
 *
 * @param deviceId - device being used
 *
 * @returns Array - Array of rfDesign options, if the device isn't supported,
 *                  returns null
 */
function getRfDesignOptions(deviceId)
{
    let newRfDesignOptions = null;
    if(deviceId === "CC2340R2RGE")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2340R5_RGE_4X4_IS24"}]
    }
    else if(deviceId === "CC2340R5RKP")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2340R5"}];
    }
    else if(deviceId === "CC2340R53RKP")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2340R5"}];
    }
    else if(deviceId === "CC2340R53RHBQ1")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2340R53_Q1"}];
    }
    else if(deviceId === "CC2340R53YBG")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2340R53_WCSP"}];
    }
    else if(deviceId === "CC2340R5RGE")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2340R5"}]
    }
    else if(deviceId === "CC2340R22RKP")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2340R5"}]
    }
    else if(deviceId === "CC2340R5RHB")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2340R5_Q1"}]
    }
    else if(deviceId === "CC2745R10RHAQ1")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2745R10_Q1"}]
    }
    else if(deviceId === "CC2744R7RHAQ1")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2745R10_Q1"}]
    }
    else if(deviceId === "CC2745P10RHAQ1")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2745R10_Q1"}]
    }
    else if(deviceId === "CC2755P105RHA")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2745R10_Q1"}]
    }
    else if(deviceId === "CC2755R105RHA")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2745R10_Q1"}]
    }
    else if(deviceId === "CC2745R7RHAQ1")
    {
        newRfDesignOptions = [{name: "LP_EM_CC2745R10_Q1"}]
    }
    return(newRfDesignOptions);
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
    const radioSettings = Common.getRadioScript(inst.rfDesign,system.deviceData.deviceId).radioConfigParams;
    const adcNoiseSettings = Common.getRadioScript(inst.rfDesign,system.deviceData.deviceId).adcNoiseParams;
    const bleCsHpSettings = Common.getRadioScript(inst.rfDesign,system.deviceData.deviceId).bleCsHpParams;
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