/*
 * Copyright (c) 2019-2023 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== custom.syscfg.js ========
 */

"use strict";

// Common utility functions
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");

// Other dependencies
const RfDesign = Common.getScript("rfdesign");
const DeviceInfo = Common.getScript("device_info.js");
const Docs = Common.getScript("radioconfig_docs.js");

// Manage PHY support
const hasProp = Common.HAS_PROP;
const hasBle = Common.HAS_BLE;
const hasIeee = Common.HAS_IEEE_15_4;

// Multi-stack validation module
const msValidationModule = "/ti/common/multi_stack_validate";
let msValidationPresent = true;
try {
    system.getScript(msValidationModule);
}
catch (err) {
    msValidationPresent = false;
}

// PHY information, grouped by protocol/frequency band
const PhyInfo = {
    ble: {
        displayName: "BLE",
        phyGroup: Common.PHY_BLE,
        settings: []
    },
    ieee: {
        displayName: "IEEE 802.15.4",
        phyGroup: Common.PHY_IEEE_15_4,
        settings: []
    }
};

if (Common.HAS_PROP) {
    PhyInfo.prop1 = {
        displayName: "Proprietary (169 MHz)",
        phyGroup: Common.PHY_PROP,
        settings: []
    };
    PhyInfo.prop4 = {
        displayName: "Proprietary (420 - 527 MHz)",
        phyGroup: Common.PHY_PROP,
        settings: []
    };
    PhyInfo.prop8 = {
        displayName: "Proprietary (779 - 930 MHz)",
        phyGroup: Common.PHY_PROP,
        settings: []
    };
}

if (Common.HAS_24G_PROP) {
    PhyInfo.prop2 = {
        displayName: "Proprietary (2400 - 2480 MHz)",
        phyGroup: Common.PHY_PROP,
        settings: []
    };
}

// Add PHY settings depending on device name
if (hasProp) {
    const configs = DeviceInfo.getConfiguration(Common.PHY_PROP).configs;

    // PHY types are in positions 1 to 4 in the config array
    for (let i = 1; i <= 4; i++) {
        const cfg = configs[i];
        if (cfg.name === "phyType868" && "prop8" in PhyInfo) {
            PhyInfo.prop8.settings = cfg.options;
        }
        if (cfg.name === "phyType433" && "prop4" in PhyInfo) {
            PhyInfo.prop4.settings = cfg.options;
        }
        if (cfg.name === "phyType169" && "prop1" in PhyInfo) {
            PhyInfo.prop1.settings = cfg.options;
        }
        if (cfg.name === "phyType2400" && "prop2" in PhyInfo) {
            PhyInfo.prop2.settings = cfg.options;
        }
    }
}

if (hasBle) {
    const configs = DeviceInfo.getConfiguration(Common.PHY_BLE).configs;
    const options = configs[0].options;
    const settings = getSettingsOptions(options);
    PhyInfo.ble.settings5dbm = settings.pa5;
    PhyInfo.ble.settings10dbm = settings.pa10;
    PhyInfo.ble.settings = settings.pa5.concat(settings.pa10);
}

if (hasIeee) {
    const configs = DeviceInfo.getConfiguration(Common.PHY_IEEE_15_4).configs;
    const options = configs[0].options;
    const settings = getSettingsOptions(options);
    PhyInfo.ieee.settings5dbm = settings.pa5;
    PhyInfo.ieee.settings10dbm = settings.pa10;
    PhyInfo.ieee.settings = settings.pa5.concat(settings.pa10);
}

// Create configurables with checkbox options list
const config = [];

_.each(PhyInfo, (pi, phyGroup) => {
    const opts = pi.settings;

    if (opts.length > 0) {
        config.push({
            name: phyGroup,
            displayName: pi.displayName,
            description: "Select PHY settings to be included in the generated code",
            placeholder: "No " + pi.displayName + " PHY selected",
            minSelections: 0,
            options: opts,
            getDisabledOptions: generateDisabledOptions(phyGroup),
            default: []
        });
    }
});

/*
 *  ======== generateDisabledOptions ========
 *  Determine what settings are to be disabled for the current board
 *
 *  @param phyGroup - PHY group (frequency band/protocol)
 *
 *  @returns - Array containing settings to be disabled
 */
function generateDisabledOptions(phyGroup) {
    return (inst) => {
        const pi = PhyInfo[phyGroup];
        const opts = [];
        if ("settings10dbm" in pi && !RfDesign.has10dBmPA()) {
            pi.settings10dbm.forEach((opt) => {
                opts.push({
                    name: opt.name,
                    reason: "Setting valid for 10 dBm PA only"
                });
            });
        }
        else if ("settings5dbm" in pi && RfDesign.has10dBmPA()) {
            // If no 10 dBm existing setting exists, allow the 5 dBm setting to be used
            // NB! IEEE 15.4 for CC2674P10
            if ("settings10dbm" in pi && pi.settings10dbm.length === 0) {
                return [];
            }
            pi.settings5dbm.forEach((opt) => {
                opts.push({
                    name: opt.name,
                    reason: "Setting not valid for 10 dBm PA"
                });
            });
        }
        return opts;
    };
}

/*
 *  ======== getSettingsOptions ========
 *  Get a list of 5 dBm and 10 dBm settings
 *
 *  @param config - list of PHY settings
 *
 *  @returns - PHY options grouped by PA
 */
function getSettingsOptions(options) {
    const ret = {
        pa5: [],
        pa10: []
    };
    options.forEach((opt) => {
        if (opt.name.includes("p10")) {
            ret.pa10.push(opt);
        }
        else {
            ret.pa5.push(opt);
        }
    });
    return ret;
}

/*
 *  ======== addRfSettingDependency ========
 *  Creates an RF setting dependency module
 *
 * @param phyGroup - PROP, BLE, IEEE 802.15.4
 * @param phyDef  - PHY definition name (e.g. )
 * @param displayName  - PHY name to display
 *
 * @returns dictionary - containing a single RF setting dependency module
 */
function addRfSettingDependency(phyGroup, phyDef, displayName) {
    const radioConfigArgs = {
        // Set parent NOT to be a protocol stack
        parent: "Custom"
    };

    // Set up module names and arguments
    if (phyGroup === Common.PHY_PROP) {
        const freqBand = Common.getFrequencyBand(phyGroup, phyDef);
        const fbStr = freqBand.toString();

        radioConfigArgs.freqBand = fbStr;

        // PHY type configs for proprietary are suffixed with frequency band
        // e.g. "phyType868", "phyType433", "phyType169", "phyType2400"
        const ptCfg = "phyType" + fbStr;
        radioConfigArgs[ptCfg] = phyDef;
    }
    else {
        radioConfigArgs.phyType = phyDef;
    }
    // Force an update of the permissions configurable
    radioConfigArgs.permission = "ReadWrite"; // NB! Must be last argument

    return ({
        name: "radioConfig" + phyDef,
        displayName: displayName,
        moduleName: Common.phyGroupPath + phyGroup,
        collapsed: true,
        args: radioConfigArgs
    });
}

/*
 *  ======== moduleInstances ========
 *  Determines what modules are added as non-static sub-modules
 *
 *  @param inst  - Module instance containing the config that changed
 *  @returns     - Array containing dependency modules
 */
function moduleInstances(inst) {
    const dependencyModule = [];
    // Iterate each PHY group
    _.each(PhyInfo, (pi, key) => {
        const phyGroup = pi.phyGroup;
        const selected = inst[key];
        // Iterate each PHY setting in the group
        _.each(pi.settings, (phy) => {
            // Add module if selected
            if (selected.includes(phy.name)) {
                dependencyModule.push(addRfSettingDependency(phyGroup, phy.name, phy.displayName));
            }
        });
    });
    return dependencyModule;
}

/*
 *  ======== modules ========
 *  Determines what modules are added as static sub-modules
 *
 *  @returns - Array containing dependency modules
 */
function modules() {
    // Pull in Multi-Stack validation module
    if (msValidationPresent) {
        return [
            {
                name: "multiStack",
                displayName: "Multi-Stack Validation",
                moduleName: msValidationModule,
                hidden: true
            }
        ];
    }
    return [];
}

/*
 *  ======== moduleStatic ========
 *  Define the Custom RF module as static
 */
const moduleStatic = {
    config: config,
    modules: modules,
    moduleInstances: moduleInstances
};

/*
 *  ======== module ========
 *  Define the Custom module properties and methods
 */
const module = {
    displayName: "Custom",
    description: "Custom Radio Configuration",
    longDescription: Docs.customDescription,
    moduleStatic: moduleStatic
};

exports = module;
