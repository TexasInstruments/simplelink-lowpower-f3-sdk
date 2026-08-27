/*
 * Copyright (c) 2018-2025 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble_controller.syscfg.js ========
 *  Standalone BLE Controller (link-layer) module.
 *  Configures controller-only feature flags; does not generate ble_config.c/h.
 */

"use strict";

// Get BLE Features Script
const bleFeaturesScript = system.getScript("/ti/ble/ble_features/ble_features");

// Get General and RF stack config groups
const connectionResourcesScript = system.getScript("/ti/ble/general/ble_connection_resources");
const rfScript         = system.getScript("/ti/ble/rf_config/ble_rf_config");
const advSetScript     = system.getScript("/ti/ble/advanced_settings/ble_advanced_settings");
const driverDepsScript = system.getScript("/ti/ble/ble_dependencies");
const csScript         = system.getScript("/ti/ble/channel_sounding/ble_cs");

//static implementation of the BLE Controller module
const moduleStatic = {

    config: [
        {
            name: "genLibs",
            displayName: "Generate BLE Libraries",
            default: true,
            hidden: true,
            description: "Configures genLibs usage for local libraries. Always hidden",
            onChange: advSetScript.onGenLibsChange
        },
        {
            name: "ctrlConfig",
            displayName: "Controller Configuration",
            description: "The BLE link-layer capabilities to compile in",
            longDescription: `Selects which link-layer capabilities are included in the controller build.\\n
Role | Description
--- | ---
Broadcaster | Advertise without accepting connections (non-connectable advertising)
Peripheral  | Advertise and accept connections (connectable advertising)
Observer    | Scan for advertisements
Central     | Initiate connections`,
            default: ["ADV_NCONN_CFG", "ADV_CONN_CFG", "SCAN_CFG", "INIT_CFG"],
            minSelections: 1,
            onChange: onCtrlConfigChange,
            options: [
                { displayName: "Broadcaster", name: "ADV_NCONN_CFG" },
                { displayName: "Peripheral",  name: "ADV_CONN_CFG"  },
                { displayName: "Observer",    name: "SCAN_CFG"      },
                { displayName: "Central",     name: "INIT_CFG"      }
            ]
        },
        bleFeaturesScript.controllerFeaturesConfig,
        {
            name: "adaptivityConfig",
            displayName: "Adaptivity Configuration",
            config: bleFeaturesScript.adaptivityConfigItems
        },
        {
            name: "channelSoundingConfig",
            displayName: "Channel Sounding Configuration",
            config: csScript.csControllerConfigItems
        },
        rfScript.rfStackConfig,
        connectionResourcesScript.config,
        advSetScript.ctrlConfig,
    ],

    dependencies: driverDepsScript.ccfgDependencies,

    moduleInstances: moduleInstances,
    modules: modules,
    validate: validate
};

/*
 *  ======== onCtrlConfigChange ========
 * Show centralGuardTime only when Initiating (Central) capability is selected.
 * @param inst  - Module instance containing the config that changed
 * @param ui    - The User Interface object
 */
function onCtrlConfigChange(inst, ui)
{
    ui.centralGuardTime.hidden = !inst.ctrlConfig.includes("INIT_CFG");
}

/*
 * ======== moduleInstances ========
 * Returns sub-module instances required by this module (RF radio config).
 *
 * @param inst  - Module instance
 * @returns     - Array of sub-module instance descriptors
 */
function moduleInstances(inst)
{
    return [
        ...rfScript.moduleInstances(inst),
        ...advSetScript.moduleInstances(inst),
        ...driverDepsScript.moduleInstances
    ];
}

/*
 * ======== modules ========
 * Returns static sub-modules required by this module.
 *
 * @param inst  - Module instance
 * @returns     - Array of static dependency module descriptors
 */
function modules(inst)
{
    let dependencyModule = [
        ...advSetScript.modules(inst),
        ...driverDepsScript.modules
    ];

    dependencyModule.push({
        name        : "multiStack",
        displayName : "Multi-Stack Validation",
        moduleName  : "/ti/common/multi_stack_validate",
        hidden      : true
    });

    return dependencyModule;
}

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - BLE Controller instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    connectionResourcesScript.validate(inst, validation);
    rfScript.validate(inst, validation);
    advSetScript.validate(inst, validation);
    bleFeaturesScript.validate(inst, validation);
    csScript.validate(inst, validation);

    // BLE requires the RNG driver to operate in Blocking mode
    const rngMod = system.modules["/ti/drivers/RNG"];
    if(rngMod && rngMod.$static.rngReturnBehavior !== "RNG_RETURN_BEHAVIOR_BLOCKING")
    {
        validation.logError("BLE requires RNG Return Behavior to be set to Blocking", inst);
    }
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
    let GenLibs = system.getScript("/ti/utils/build/GenLibs.syscfg.js");
    let libs = [];
    let toolchain = GenLibs.getToolchainDir();

    if(inst.$static.genLibs)
    {
        if(system.deviceData.deviceId.match(/CC23/))
        {
            libs.push(`ti/ble/lib/CC2340R5/ControllerLib/lib/${toolchain}/m0p/ControllerLib.a`);
        }
        else if(system.deviceData.deviceId.match(/CC27/))
        {
            libs.push(`ti/ble/lib/CC27XXX10/ControllerLib/lib/${toolchain}/m33f/ControllerLib.a`);
        }
    }

    libs.push(...csScript.getLibs(inst));

    const linkOpts = {
        name: "/ti/ble/ble_controller",
        deps: [],
        libs: libs
    };

    return(linkOpts);
}

/*
 *  ======== getOpts ========
 * Returns compiler defines for BLE Controller: general stack, RF, and
 * controller feature flags.
 */
function getOpts(mod)
{
    const inst = mod.$static;
    // CTRL_CONFIG must come first as it is the primary controller capability define.
    return [
        "-DCTRL_CONFIG=" + inst.ctrlConfig.join("+"),
        "-DHCI_TL_FULL",
        "-DCONTROLLER_ONLY",
        ...connectionResourcesScript.getOpts(mod),
        ...rfScript.getOpts(mod),
        ...advSetScript.getOpts(mod),
        ...bleFeaturesScript.getControllerOpts(mod),
        ...csScript.getStackOpts(mod)
    ];
}

/*
 *  ======== bleControllerModule ========
 *  Define the BLE Controller module properties and methods
 */
const bleControllerModule = {
    displayName: "BLE Controller Only",
    description: "Configure BLE Controller (link-layer) features",
    moduleStatic: moduleStatic,
    templates: {
        "/ti/utils/build/GenLibs.cmd.xdt": {
            modName: "/ti/ble/ble_controller",
            getLibs: getLibs
        },
        "/ti/utils/build/GenOpts.opt.xdt": {
            modName: "/ti/ble/ble_controller",
            getOpts: getOpts
        }
    }
};

/*
 *  ======== exports ========
 *  Export the BLE Controller module
 */
exports = bleControllerModule;
