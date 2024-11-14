/*
 * Copyright (c) 2022-2023 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== code_export_param.syscfg.js ========
 *  Module to configure code generation
 */

"use strict";

// Common utility functions
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");

// Other dependencies
const DevInfo = Common.getScript("device_info.js");
const PhyHandler = Common.getPhyHandler();

// Documentation
const CodeExportDocs = Common.getScript("code_export_param_docs.js");

// Path to external applications (relative to location of SDK product file)
const ExtPath = "./../source/ti/devices/radioconfig/.meta/ext_app/";

// Protocol support
const HasProp = Common.HAS_PROP;
const HasBle = Common.HAS_BLE;
const Has154 = Common.HAS_IEEE_15_4;

// Platform support
const isRFD = Common.isPlatformRFD();
const isRCL = !isRFD;

// Custom RF patch only on Windows
const IsWindows = system.getOS() === "win";

// Storage for generated variable names, used by code generation template
let SymbolNames = {
    txPower: "txPowerTable",
    txPowerSize: "TX_POWER_TABLE_SIZE",
    rfMode: "RF_prop",
    overrides: "pOverrides"
};

// List of RF commands that can be individually configured
const RfCommandNames = {};

// List of commands, indexed by PHY group ("ble", "prop" or "ieee_15_4")
// Populated by createCommandList()
const CmdMap = {};

// Storage for command lists that has been changed by the user, overrides default command list for PHY
let CustomCmdList = [];

// Load pre-generated configurables
const CeCfg = DevInfo.getCodeExportConfig();
const Config = CeCfg.config;
let CmdOffset;
if (isRFD) {
    SymbolNames = {...SymbolNames, ...CeCfg.symbols};
    CmdOffset = 11;
}
else {
    SymbolNames = {
        ...SymbolNames, ...CeCfg.symbols.ble, ...CeCfg.symbols.prop, ...CeCfg.symbols.ieee_15_4
    };
    CmdOffset = 7;
}

// Symbols that need a post-fix appended
const NeedPostfix = Object.keys(SymbolNames);

// Add RF command configurables
let CmdConfig = CeCfg.commands.common;

if (HasProp) {
    CmdConfig = [...CmdConfig, ...CeCfg.commands.prop];
    createCommandList(Common.PHY_PROP);
}

if (HasBle) {
    CmdConfig = [...CmdConfig, ...CeCfg.commands.ble];
    createCommandList(Common.PHY_BLE);
}

if (Has154) {
    CmdConfig = [...CmdConfig, ...CeCfg.commands.ieee_15_4];
    createCommandList(Common.PHY_IEEE_15_4);
}

// Add RF Command configurables (config group)
Config[CmdOffset].config = [...Config[CmdOffset].config, ...CmdConfig];

// Add documentation to main commands
Common.initLongDescription(Config, CodeExportDocs.codeExportDocs);

// Set up call-backs etc
initConfigurables();

/**
 *  ======== initConfigurables ========
 *  Initialize configurables that are not completed in pre-processing script.
 *
 */
function initConfigurables() {
    for (const item of Config) {
        switch (item.name) {
        case "symGenMethod":
            item.onChange = onSymGenMethodChanged;
            break;
        case "phyAbbr":
            item.onChange = onPhyAbbrChanged;
            break;
        case "phyGroup":
            item.onChange = onPhyGroupChanged;
            break;
        case "phyType":
            item.onChange = onPhyTypeChanged;
            break;
        default:
            break;
        }

        // Configurable groups
        if ("config" in item) {
            for (const subItem of item.config) {
                switch (subItem.name) {
                case "patchUpdate":
                    subItem.onLaunch = onPatchUpdateLaunch;
                    subItem.onComplete = onPatchUpdateComplete;
                    subItem.hidden = !IsWindows;
                    break;
                case "cpePatch":
                    subItem.onChange = onPatchChanged;
                    subItem.options = (inst) => getPatchOptions("cpe");
                    break;
                case "mcePath":
                    subItem.onChange = onPatchChanged;
                    subItem.options = (inst) => getPatchOptions("mce");
                    break;
                case "rfePatch":
                    subItem.onChange = onPatchChanged;
                    subItem.options = (inst) => getPatchOptions("rfe");
                    break;
                case "cmdPrefix":
                case "cmdFormat":
                    subItem.onChange = onCmdFormatChanged;
                    break;
                case "rfMode":
                case "txPower":
                case "txPowerSize":
                case "overrides":
                    subItem.default = SymbolNames[subItem.name];
                    break;
                default:
                    if (subItem.name.includes("cmdList")) {
                        subItem.onChange = onCmdListChanged;
                    }
                    break;
                }
            }
        }
    }
}

/**
 * ======== createCommandList ========
 * Create list of commands for a PHY group. This must be done
 * at load time, as option lists can not be modified on the fly.
 *
 * @param phyGroup - ble, prop or ieee_15_4
 */
function createCommandList(phyGroup) {
    const cmdCfg = CeCfg.commands;
    const cmds = [...cmdCfg.common, ..._.cloneDeep(cmdCfg[phyGroup])];
    CmdMap[phyGroup] = cmds;

    for (const cmd of cmds) {
        const name = cmd.name;
        const value = "RF_" + name;
        RfCommandNames[name] = value;
    }
}

/**
 * ======== updateCommandVisibility ========
 * Update the visibility state of the individual commands
 *
 * @param inst - Code Export Param instance
 * @param ui - UI state object
 */
function updateCommandVisibility(inst, ui) {
    const cmdListActive = getSelectedCommands(inst);
    const cdmListAll = CmdMap[inst.phyGroup];
    const custom = inst.symGenMethod === "Custom";

    // Show checked commands only
    for (const cmd of cdmListAll) {
        const id = cmd.name;
        if (cmdListActive.includes(id)) {
            ui[id].hidden = false;
        }
        else {
            ui[id].hidden = true;
        }
        ui[id].readOnly = !custom;
    }
}

/**
 * ======== updateStructVisibility ========
 * Update the visibility state of the individual structs
 *
 * @param inst - Code Export Param instance
 * @param ui - UI state object
 */
function updateStructVisibility(inst, ui) {
    // Show checked commands only
    if (inst.phyGroup in CeCfg.symbols) {
        for (const sym of Object.keys(CeCfg.symbols[inst.phyGroup])) {
            if (sym in ui) {
                ui[sym].hidden = false;
            }
        }
    }
}

/**
 *  ======== getSelectedCommands ========
 *  Get list of selected commands
 *
 *  @param inst - active instance
 */
function getSelectedCommands(inst) {
    const phyGroup = inst.phyGroup;
    const cmdList = "cmdList_" + phyGroup;
    let ret = [];
    if (cmdList in inst) {
        ret = inst[cmdList];
    }
    return ret;
}

/**
 *  ======== getHiddenCommands ========
 *  Get list of hidden commands
 *
 *  @param inst - active instance
 */
function getHiddenCommands(inst) {
    const hidden = [];
    const visible = getSelectedCommands(inst);

    _.each(RfCommandNames, (val, cmd) => {
        if (!visible.includes(cmd)) {
            hidden.push(cmd);
        }
    });
    return hidden;
}

/**
 *  ======== getSymNames ========
 *  Get variable names
 *
 *  @param inst - active instance
 */
function getSymNames(inst) {
    const cache = {};
    const activeCmds = getSelectedCommands(inst);
    const hiddenCmds = getHiddenCommands(inst);

    for (const name in inst) {
        if (!name.includes("$") && !hiddenCmds.includes(name)) {
            cache[name] = inst[name];
            const usePostfix = NeedPostfix.includes(name) || activeCmds.includes(name);
            if (usePostfix) {
                cache[name] = getDefaultValue(inst, name);
            }
        }
    }
    return cache;
}

/**
 * ======== onPatchUpdateComplete ========
 * Launch a patch update by scanning SDK for existing patches.
 *
 * @param inst - Code Export Param instance
 * @param ui - Code Export Param user interface
 * @param result - Result from patch scan
 *
 */
function onPatchUpdateComplete(inst, ui, result) {
    const files = result.data.trim().split("\n");
    const patches = [];
    files.forEach((file) => {
        patches.push(file.replace(".h", "").trim());
    });
    DevInfo.setPatchInfo(patches);
}

/**
 * ======== onCmdListChanged ========
 * The command selection in a command list has changed.
 *
 * @param inst - Code Export Param instance
 * @param ui - UI state object
 *
 */
function onCmdListChanged(inst, ui) {
    const cmdListName = "cmdList_" + inst.phyGroup;
    CustomCmdList = inst[cmdListName];
    updateCommandVisibility(inst, ui);
    onCmdFormatChanged(inst);
}

/**
 * ======== onPhyTypeChanged ========
 * Invoked when the PHY type changes.
 *
 * @param inst - Code Export Param instance
 * @param ui - UI state object
 *
 */
function onPhyTypeChanged(inst, ui) {
    const phyType = inst.phyType;
    const phyGroup = inst.phyGroup;
    const cmdList = "cmdList_" + phyGroup;

    if (cmdList in inst) {
        if (CustomCmdList.length === 0) {
            inst[cmdList] = getCmdList();
        }

        // Update visibility of individual commands
        updateCommandVisibility(inst, ui);
    }

    // Make sure the symbol names are updated
    if (inst.symGenMethod === "Automatic") {
        onSymGenMethodChanged(inst, ui);
    }

    if (isRCL) {
        updateStructVisibility(inst, ui);
    }

    function getCmdList() {
        const tmpList = PhyHandler.get(phyGroup, phyType).getCmdList("basic");
        const ccList = [];
        _.each(tmpList, (cmd) => {
            const id = _.camelCase(cmd);
            ccList.push(id);
        });
        return ccList;
    }
}

/**
 * ======== onPhyGroupChanged ========
 * Set the visibility state of RF commands according to their presence in the PHY
 * group of the active instance. Example: if BLE is used, only common and BLE commands
 * should be visible.
 *
 * @param inst - Code Export Param instance
 * @param ui - UI state object
 */
function onPhyGroupChanged(inst, ui) {
    const phyGroup = inst.phyGroup;
    CustomCmdList = [];

    // Update visibility of command selection
    const visDescr = {
        prop: phyGroup !== Common.PHY_PROP,
        ble: phyGroup !== Common.PHY_BLE,
        ieee_15_4: phyGroup !== Common.PHY_IEEE_15_4
    };

    _.each(visDescr, (hidden, pg) => {
        const cmdList = "cmdList_" + pg;
        if (cmdList in ui) {
            ui[cmdList].hidden = hidden;
        }
    });

    onPhyTypeChanged(inst, ui);
}

/**
 * ======== onSymGenMethodChanged ========
 * Invoked when symbol name generation method changed
 *
 * @param inst - active instance
 * @param ui - active UI
 */
function onSymGenMethodChanged(inst, ui) {
    const automatic = inst.symGenMethod === "Automatic";
    const custom = inst.symGenMethod === "Custom";
    const allVars = {...SymbolNames, ...RfCommandNames};

    // Reset symbols and update UI
    for (const name in allVars) {
        if (name in inst) {
            const cfgDef = inst.$module.$configByName[name];
            if (cfgDef) {
                inst[name] = cfgDef.default;
            }
            ui[name].readOnly = !custom;
        }
    }

    if (automatic) {
        // Update symbol names
        for (const name in allVars) {
            if (name in inst) {
                inst[name] = getDefaultValue(inst, name);
            }
        }
    }
    else {
        // Legacy or custom
        onCmdFormatChanged(inst);
    }

    if ("cmdPrefix" in ui) {
        ui.cmdPrefix.hidden = custom || automatic;
        ui.cmdFormat.hidden = custom || automatic;
    }

    // PHY abbreviation is not available in automatic mode
    if ("phyAbbr" in ui) {
        if (automatic) {
            inst.phyAbbr = false;
        }
        ui.phyAbbr.readOnly = automatic;
    }
}

/**
 *  ======== onPhyAbbrChanged ========
 *  Invoked when PHY abbreviation is enabled or disabled
 *
 *  @param inst - active instance
 *
 */
function onPhyAbbrChanged(inst) {
    // Update symbol names
    const allVars = {...SymbolNames, ...RfCommandNames};
    for (const name in allVars) {
        if (name in inst) {
            inst[name] = getDefaultValue(inst, name);
        }
    }
}

/**
 *  ======== getPostfix ========
 *  Create a postfix for a symbol name based on PHY abbreviation
 *
 *  @param inst - active instance
 *
 */
function getPostfix(inst) {
    let postfix = _.camelCase(inst.phyType.replace("_gen", ""));
    postfix = Common.ucFirst(postfix);

    return postfix;
}

/**
 * ======== onSymGenMethodChanged ========
 * Invoked when symbol name format for RF Commands changed
 *
 * @param inst - active instance
 */
function onCmdFormatChanged(inst) {
    if (inst.symGenMethod !== "Legacy") {
        return;
    }
    const activeCommands = getSelectedCommands(inst);

    _.each(activeCommands, (cmdName) => {
        const name = getDefaultValue(inst, cmdName).replace("RF_", "").replace("rcl", "");
        let newName = inst.cmdPrefix;
        if (inst.cmdFormat === "camelCase") {
            newName += name;
        }
        else {
            newName += _.snakeCase(name);
        }
        inst[cmdName] = newName;
    });
}

/**
 * ======== onPatchChanged ========
 * Invoked when a patch selection changes
 *
 * @param inst - active instance
 * @param ui - active UI
 */
function onPatchChanged(inst, ui) {
    const ch = PhyHandler.get(inst.phyGroup, inst.phyType);
    const cpe = inst.cpePatch === "Automatic" ? null : inst.cpePatch;
    const rfe = inst.rfePatch === "Automatic" ? null : inst.rfePatch;
    const mce = inst.mcePatch === "Automatic" ? null : inst.mcePatch;
    ch.setManualPatch(cpe, rfe, mce);
}

/**
 * ======== getDefaultValue ========
 * Get default value of a configurable
 *
 * @param inst - active instance
 * @param cfgName - configurable name
 */
function getDefaultValue(inst, cfgName) {
    const configs = inst.$module.config;
    let ret = "";
    function setRet(cfg) {
        if (cfgName === cfg.name) {
            ret = cfg.default;
            return false;
        }
        return true;
    }
    _.each(configs, (cfg) => {
        if (_.has(cfg, "config")) {
            _.each(cfg.config, (subCfg) => {
                setRet(subCfg);
            });
        }
        else {
            setRet(cfg);
        }
    });

    if (inst.symGenMethod === "Automatic") {
        const name = inst.$name;
        const instIdx = name.substr(name.length - 1);
        ret += "_" + inst.phyType + "_" + instIdx;
    }
    else if (inst.phyAbbr) {
        ret += getPostfix(inst);
    }
    return ret;
}

/**
 * ======== getInstanceIndex ========
 * Get the index of a Code Export configuration instance
 *
 * @param inst - active instance
 */
function getInstanceIndex(inst) {
    return inst.$name.match(/\d+/)[0];
}

/**
 * ======== getPatchOptions ========
 * Get patch selection options
 *
 * @param filter - cpe, rfe or mce
 */
function getPatchOptions(filter) {
    const opts = [{
        name: "Automatic"
    }];
    const patches = DevInfo.getPatchInfo();

    for (const patch of patches) {
        if (patch.includes(filter)) {
            const opt = {
                name: patch
            };
            opts.push(opt);
        }
    }
    return opts;
}

/**
 * ======== checkDuplicateSymbols ========
 * Check if symbols are duplicated
 *
 * @param myInst - active instance
 * @param validation - validation object
 */
function checkDuplicateSymbols(myInst, validation) {
    const modules = system.modules;
    const cSymbols = [];
    let hasLegacy = false;

    // Iterate RadioConfig modules
    _.each(modules, (mod) => {
        if (mod.$name.includes("radioconfig/phy_groups")) {
            const instances = mod.$instances;
            // Iterate module instances
            _.each(instances, (inst) => {
                const ce = inst.codeExportConfig;
                const isLegacy = ce.symGenMethod === "Legacy" && !ce.phyAbbr;

                // Check if more than one instance used legacy code generation
                if (isLegacy) {
                    if (!hasLegacy) {
                        hasLegacy = true;
                    }
                    else {
                        // eslint-disable-next-line max-len
                        Common.logError(validation, myInst, "symGenMethod", "SmartRF Studio code generation can only be applied to one PHY.");
                        return false;
                    }
                }

                // Iterate symbols
                const rfCmds = getSelectedCommands(ce);
                const symList = NeedPostfix.concat(rfCmds);

                let nRfCmdErr = 0;
                _.each(symList, (sym) => {
                    if (sym in myInst) {
                        const cSymbol = ce[sym];
                        if (cSymbols.includes(cSymbol)) {
                            // Symbol already in use
                            if (rfCmds.includes(sym) && isLegacy) {
                                nRfCmdErr += 1;
                            }
                            else {
                                Common.logError(validation, myInst, sym, "symbol duplicated");
                            }
                        }
                        else {
                            // Mark symbol as used
                            cSymbols.push(cSymbol);
                        }
                    }
                });

                if (nRfCmdErr > 0) {
                    const msg = "RF Command symbols duplicated. Make sure that command prefix"
                        + " or command format are unique across all settings.";
                    Common.logError(validation, myInst, "cmdPrefix", msg);
                    return false;
                }
                return true;
            });
        }
    });
}

/**
*  ======== getCustomOverrideInfo ========
*  Get information on stack overrides
*/
function getCustomOverrideInfo() {
    const modules = system.modules;
    const paths = [];
    const macros = [];

    // Iterate RadioConfig modules
    _.each(modules, (mod) => {
        if (mod.$name.includes("radioconfig/phy_groups")) {
            const instances = mod.$instances;
            _.each(instances, (inst) => {
                const ce = inst.codeExportConfig;

                // Stack overrides
                let path = ce.stackOverride;
                let macro = ce.stackOverrideMacro;
                if (path !== "" && macro !== "") {
                    paths.push(path);
                    macros.push(macro);
                }

                // Application overrides
                path = ce.appOverride;
                macro = ce.appOverrideMacro;
                if (path !== "" && macro !== "") {
                    paths.push(path);
                    macros.push(macro);
                }
            });
        }
    });
    return {
        paths: _.union(paths),
        macros: _.union(macros)
    };
}

/**
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  @param inst - Code Export Param instance to be validated
 *  @param validation - Issue reporting object
 */
function validate(inst, validation) {
    // Verify unique symbols
    checkDuplicateSymbols(inst, validation);

    // Verify valid C symbols
    for (const name in SymbolNames) {
        if (name in inst) {
            const val = inst[name];
            if (Common.isCName(val)) {
                if (val === "") {
                    Common.logError(validation, inst, name, "empty variable name not allowed");
                }
            }
            else {
                Common.logError(validation, inst, name, "'" + val + "' is not a valid C identifier");
            }
        }
    }

    // Verify stack override file path name and stack override macro name
    if ("stackOverride" in inst) {
        const filePath = inst.stackOverride;
        if (filePath !== "") {
            // Check that the file is in the '/ti' name space
            if (filePath.substr(0, 3) !== "ti/") {
                // eslint-disable-next-line max-len
                Common.logError(validation, inst, "stackOverride", "Stack override file must reside in 'ti' name-space.");
            }
            else {
                validateCustomOverride(inst, validation, "stackOverride", "stackOverrideMacro");
            }
        }
    }

    if ("appOverride" in inst) {
        // Verify application override file path name and application override macro name
        validateCustomOverride(inst, validation, "appOverride", "appOverrideMacro");
    }

    if ("cpePatch" in inst) {
        // Verify patch name
        if (!Common.isCName(inst.cpePatch)) {
            Common.logError(validation, inst, "cpePatch", "'" + inst.cpePatch + "' is not a valid patch name");
        }
    }
}

/**
 *  ======== validateCustomOverride ========
 *  Validate path- and macro names for custom overrides
 *
 *  @param inst - Code Export Param instance to be validated
 *  @param validation - Issue reporting object
 *  @param path - Name of "path" configurable, stores custom override file name
 *  @param macro - Name of "macro" configurable, stores macro name
 */
function validateCustomOverride(inst, validation, path, macro) {
    const filePath = inst[path];
    const macroName = inst[macro];

    if (filePath !== "") {
        // Check that the file has the extension .h and is a combination of letters, digits and '_'
        // The path may be of the pattern /dir/file.h or ../../file.h
        // eslint-disable-next-line prefer-regex-literals
        const pathRegEx = RegExp(/^(\.\.\/)*([a-zA-Z0-9_]+\/)*([a-zA-Z0-9_]+)+\.h$/);
        if (!pathRegEx.test(filePath)) {
            // eslint-disable-next-line max-len
            Common.logError(validation, inst, path, "File name must have extension .h and contain only alphanumeric and underscore letters. "
                + "UNIX relative path format expected.");
        }

        // Check that the macro is valid
        if (Common.isCName(macroName)) {
            if (macroName === "") {
                Common.logError(validation, inst, macro, "Override macro name must be present.");
            }
        }
        else {
            Common.logError(validation, inst, macro, "'" + macroName + "' is not a valid C identifier");
        }
    }
    else if (macroName !== "") {
        Common.logError(validation, inst, path, "Override file name must be present.");
    }
}

// Module exports
exports = {
    displayName: "Code Export Configuration",
    config: Config,
    validate: validate,
    getSymNames: getSymNames,
    getInstanceIndex: getInstanceIndex,
    getCustomOverrideInfo: getCustomOverrideInfo
};
