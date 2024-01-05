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
 *  ======== Radio Configuration ========
 *  Radio configuration support.
 *
 *  Code used by to all RF Settings (Proprietary, BLE, TI 15.4 ...)
 */

"use strict";

// Common utility functions
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");

// Other dependencies
const DeviceInfo = Common.getScript("device_info.js");
const PhyHandler = Common.getPhyHandler();
const RfDesign = Common.getScript("rfdesign");

// Which device interface to use (RFD or RCL)
const UseRfDriver = Common.isPlatformRFD();

// Sub-templates
const templatePath = Common.basePath + "templates/";
let subTempH;
let subTempC;

if (UseRfDriver) {
    subTempH = templatePath + "phygroup_rfd.h.xdt";
    subTempC = templatePath + "phygroup_rfd.c.xdt";
}
else {
    subTempH = templatePath + "phygroup_rcl.h.xdt";
    subTempC = templatePath + "phygroup_rcl.c.xdt";
}

// Include paths for use in C-file.
const RfInclude = "#include \"ti_radio_config.h\"\n";

/**
*  ======== generateIncludesH ========
*  Generated include directives for use in the header file (*.h)
*
*  @param modNames - Names of all modules in the SDK (as path names)
*/
function generateIncludesH(modNames) {
    const fileMap = {};

    // Collect include files, use map to avoid duplicates
    for (const modPath of modNames) {
        const res = modPath.match("/radioconfig/phy_groups/(.*)$");
        if (res !== null) {
            // RadioConfig module
            const module = system.modules[modPath];
            const phyGroup = res[1];
            const instances = module.$instances;

            // Iterate PHY instances
            for (const inst of instances) {
                const phyName = Common.getPhyType(inst);
                const ch = PhyHandler.get(phyGroup, phyName);

                // Iterate include files
                const fileInfo = ch.getIncludeFiles();
                for (const fi of fileInfo) {
                    const name = fi.name;
                    const type = fi.type;
                    fileMap[name] = type;
                }
            }
        }
    }

    // Generate the code
    let code = "";

    for (const [name, type] of Object.entries(fileMap)) {
        if (type === "common") {
            code += `#include <${name}>\n`;
        }
        else {
            code += `#include DeviceFamily_constructPath(${name})\n`;
        }
    }
    return code;
}

/**
*  ======== generateIncludesC ========
*  Generated include directives for use in the implementation file (*.c)
*
*/
function generateIncludesC() {
    // Common includes (for compatibility with SmartRF Studio)
    return RfInclude;
}

/*
 *  ======== reloadInstanceFromPhy ========
 *  Update all configurables when the frequency band or PHY type has changed
 *
 *  @param inst - module instance
 *  @param ui - module UI state
 *  @phyName - currently selected setting (PHY)
 *  @phyGroup - currently used PHY group (BLE, PROP, IEEE..)
 *  @preserve - list of configurables to preserve the value of
 */
function reloadInstanceFromPhy(inst, ui, phyName, phyGroup, preserve) {
    const cmdHandler = PhyHandler.get(phyGroup, phyName, inst.settingGroup === "Selectivity");
    const rfData = cmdHandler.getRfData();
    _.each(rfData, (value, key) => {
        // Do NOT refresh preserved configurables
        if (preserve.includes(key)) {
            return;
        }
        if (key in inst) {
            inst[key] = value;
        }
    });

    if ("highPA" in inst) {
        updateTxPowerVisibility(inst, ui);

        const settingMap = DeviceInfo.getSettingMap(phyGroup);
        const settingInfo = _.find(settingMap, (s) => s.name === phyName);
        let hidden = false;

        if ("options" in settingInfo) {
            if (settingInfo.options.includes("disable_high_pa")) {
                hidden = true;
            }
        }
        if (inst.freqBand === "433") {
            hidden = true;
        }
        ui.highPA.hidden = hidden;
    }
}

/*
 *  ======== validatePhyProperty ========
 *  Check that PHY properties are supported by RF commands.
 *
 *  @param inst - module instance
 *  @param validation - validation object
 *  @phyType - currently selected setting (PHY)
 *  @phyGroup - currently used PHY group
 */
function validatePhyProperty(inst, validation, phyType, phyGroup) {
    const phyHandler = PhyHandler.get(phyGroup, phyType);
    const rfData = phyHandler.getRfData();

    if (UseRfDriver) {
        // RF Driver device interface
        const usedCmds = phyHandler.getUsedCommands(inst, "actual");

        for (const phyProp in rfData) {
            if (!("highPA" in inst && phyProp.includes("txPower"))) {
                if (!phyHandler.isPhyPropSupported(phyProp, usedCmds)) {
                    Common.logWarning(validation,
                        inst,
                        phyProp,
                        "This PHY property has no effect on the exported code "
                        + "(no corresponding RF command selected).");
                }
            }
        }
    }
    else {
        // RCL device interfaces
        for (const phyProp in rfData) {
            if (!phyHandler.isPhyPropSupported(inst, phyProp, "actual")) {
                const ui = inst.$uiState;
                if (!ui[phyProp].hidden) {
                    Common.logWarning(validation,
                        inst,
                        phyProp,
                        "This PHY property has no effect on the exported code "
                        + "(no corresponding RCL command selected).");
                }
            }
        }
    }
}

/*
 *  ======== validateFrontendSettings ========
 *  Check that RF parameters are supported by dependant RF commands
 *
 *  @param inst - module instance
 *  @param validation - validation object
 *  @phyGroup - currently used PHY group
 */
function validateFrontendSettings(inst, validation, phyGroup) {
    let freqBand = 2400;
    let cfgName = "fe24g";
    if ("freqBand" in inst) {
        freqBand = parseInt(inst.freqBand);
        if (freqBand !== 2400) {
            cfgName = "feSub1g";
        }
    }
    const id = RfDesign.getFrontEnd(freqBand);
    const fe = DeviceInfo.getFrontendSettings(phyGroup, id);
    if (fe === null) {
        const rfinst = system.modules["/ti/devices/radioconfig/rfdesign"].$static;
        Common.logError(validation,
            rfinst,
            cfgName,
            "This frontend setting is not supported for the current RF Design");
    }
}

/*
 *  ======== updateTxPowerVisibility ========
 *  Update the visibility of txPower configurables
 *
 *  @param inst  - module instance
 *  @param ui - module UI state
 */
function updateTxPowerVisibility(inst, ui) {
    let prop2400 = false;
    let freqBand = "2400";

    if ("freqBand" in inst) {
        // This is a proprietary setting
        freqBand = inst.freqBand;
        prop2400 = freqBand === "2400";
    }
    const fbLow = freqBand === "433" || freqBand === "169";

    // Visibility of power tables
    ui.txPower.hidden = inst.highPA || fbLow || prop2400;
    ui.txPowerHi.hidden = !inst.highPA || fbLow || prop2400;

    if ("phyType433" in inst) {
        const otherFreqband = freqBand !== "433";
        ui.txPower433.hidden = inst.highPA || otherFreqband;
        if ("txPower433Hi" in inst) {
            ui.txPower433Hi.hidden = true;
        }
    }
}

/**
 *  ======== getPaUsage ========
 *  Return the PA symbols used by this PHY setting
 *
 *  @inst - PHY setting instance
 */
function getPaUsage(inst) {
    const ce = inst.codeExportConfig;
    const paExport = ce.paExport;

    if (paExport === "none") {
        return null;
    }

    const paUsage = {};
    const rfMod = system.modules["/ti/devices/radioconfig/rfdesign"].$static;

    let fb = 2400; // For BLE and IEEE 802.15.4
    let paHi = "20";
    let paTable = RfDesign.getPaTable("2400", false);

    if ("freqBand" in inst) {
        fb = parseInt(inst.freqBand);
        if (fb < 1000) {
            paTable = RfDesign.getPaTable(fb, false);
        }
    }
    let pa = RfDesign.getPaTableSuffix(paTable);

    const hasHighPA = rfMod.pa20 !== "none";
    if (hasHighPA) {
        paTable = RfDesign.getPaTable(fb, true);
    }
    const useHighPA = hasHighPA && "highPA" in inst && inst.highPA;

    switch (paExport) {
    case "active":
        if (useHighPA) {
            pa = RfDesign.getPaTableSuffix(paTable);
        }
        break;
    case "combined":
    case "dual":
        if (useHighPA) {
            paHi = RfDesign.getPaTableSuffix(paTable);
        }
        break;
    default:
        break;
    }

    if (paExport === "dual") {
        let code = RfDesign.genPaTableName(fb, pa, null);
        paUsage.code = "#define " + ce.txPower + "TxStd " + code + "\n";
        paUsage.size = "#define " + ce.txPowerSize + "_TXSTD " + code.toUpperCase() + "\n";

        code = RfDesign.genPaTableName(fb, paHi, null);
        paUsage.code += "#define " + ce.txPower + "Tx20 " + code + "\n";
        paUsage.size += "#define " + ce.txPowerSize + "_TX20 " + code.toUpperCase() + "\n";
    }
    else {
        const code = RfDesign.genPaTableName(fb, pa, useHighPA ? paHi : null);
        paUsage.code = "#define " + ce.txPower + " " + code + "\n";
        paUsage.size = "#define " + ce.txPowerSize + " " + code.toUpperCase() + "_SIZE\n";
    }
    return paUsage;
}

/*
 *  ======== highPaOnChange ========
 *  On change handler for highPA configurable
 *
 *  @param inst  - module instance
 *  @param ui - module UI state
 */
function highPaOnChange(inst, ui) {
    // Update visibility of TX power configurables
    if ("phyType" in inst) {
        ui.txPower.hidden = inst.highPA;
        ui.txPowerHi.hidden = !inst.highPA;
    }
    if ("phyType868" in inst) {
        ui.txPower.hidden = inst.highPA || ui.phyType868.hidden;
        ui.txPowerHi.hidden = !inst.highPA || ui.phyType868.hidden;
    }
    if ("phyType433" in inst) {
        ui.txPower433.hidden = inst.highPA || ui.phyType433.hidden;
        if ("txPower433Hi" in inst) {
            ui.txPower433Hi.hidden = true;
        }
    }
}

/*
 *  ======== pruneConfig ========
 *  Removed option list members that are not accepted by SysConfig.
 *
 *  @param config - configurable array to be modified
 */
function pruneConfig(config) {
    _.each(config, (item) => {
        deleteKeys(item, "key");
        deleteKeys(item, "info");

        if (_.has(item, "config")) {
            _.each(item.config, (subItem) => {
                deleteKeys(subItem, "key");
                deleteKeys(subItem, "info");
            });
        }
    });

    function deleteKeys(item, key) {
        if (_.has(item, "options")) {
            _.each(item.options, (opt) => {
                delete opt[key];
            });
        }
    }
}

/*
 *  ======== moduleInstances ========
 *  Determines what modules are added as sub-modules
 *
 *  @param inst  - Module instance containing the config that changed
 *  @returns     - Array containing the dependency modules
 */
function moduleInstances(inst) {
    // Controls visibility/access of code export config
    const noAccess = inst.permission === "ReadOnly";
    const phyType = Common.getPhyType(inst);
    const phyGroup = Common.getPhyGroup(inst);

    return [
        {
            name: "codeExportConfig",
            displayName: "Code Export Configuration",
            moduleName: Common.basePath + "code_export_param",
            collapsed: true,
            readOnly: noAccess,
            hidden: noAccess,
            args: {
                phyType: phyType,
                phyGroup: phyGroup
            }
        }
    ];
}

/*
 *  ======== modules ========
 *  Determines what modules are added as static sub-modules
 *
 *  @returns     - Array containing static dependency modules
 */
function modules() {
    const rfDesignMod = {
        name: "rfDesign",
        displayName: "RF Frontend",
        moduleName: "/ti/devices/radioconfig/rfdesign",
        hidden: false
    };

    if (Common.isPlatformRFD()) {
        return [{
            name: "RF",
            displayName: "RF Driver",
            moduleName: "/ti/drivers/RF",
            collapsed: true
        },
        rfDesignMod
        ];
    }
    return [
        rfDesignMod
    ];
}

exports = {
    generateIncludesH: generateIncludesH,
    generateIncludesC: generateIncludesC,
    reloadInstanceFromPhy: reloadInstanceFromPhy,
    highPaOnChange: highPaOnChange,
    pruneConfig: pruneConfig,
    getPaUsage: getPaUsage,
    validatePhyProperty: validatePhyProperty,
    validateFrontendSettings: validateFrontendSettings,
    modules: modules,
    moduleInstances: moduleInstances,
    templates: {
        "/ti/devices/radioconfig/templates/rf_settings.c.xdt": subTempC,
        "/ti/devices/radioconfig/templates/rf_settings.h.xdt": subTempH
    }
};
