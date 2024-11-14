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
 *  ======== radioconfig_common.js ========
 *  Common functionality for the RadioConfig module
 */

"use strict";

const Device = system.deviceData.deviceId;
const BasePath = "/ti/devices/radioconfig/";
const PhyGroupPath = BasePath + "phy_groups/";

// Mapping SysCfg device name notation to SmartRF Studio format
const SysconfigRfconfigBoardMap = system.getScript("/ti/devices/radioconfig/data/sysconfig_rfconfig_board_map.json");
const DevNameMap = system.getScript("/ti/devices/radioconfig/data/sysconfig_studio_device_names.json");
const DevFamilyMap = system.getScript("/ti/devices/radioconfig/data/device_family_map.json");
const DevPhyProtocolMap = system.getScript("/ti/devices/radioconfig/config/rcl/device_phy_protocol_mapping.json");

// SmartRF Studio compatible device name
const DeviceName = DevNameMap[Device] || "none";
const isValidDevice = DeviceName !== "none";

const isDeviceClass10 = Device.match(/[157]4[RP]10|2653P10/) !== null;

const PHY_BLE = "ble";
const PHY_PROP = "prop";
const PHY_IEEE_15_4 = "ieee_15_4";

// Determine if a device is accessed via RF Driver or RCL.
const isPlatformRFD = Device.match(/CC26|CC13/) !== null;

// Paths for configuration data
const ConfigPath = isPlatformRFD ? BasePath + "config/rfd/" : BasePath + "config/rcl/";

const BLE_CONFIG = readConfigurationFile(PHY_BLE);
const PROP_CONFIG = readConfigurationFile(PHY_PROP);
const IEEE_CONFIG = readConfigurationFile(PHY_IEEE_15_4);

// Check that configuration files exists to check that the device actually has supported phys
const hasProp = isValidDevice && DevPhyProtocolMap[DeviceName.toUpperCase()].includes("Proprietary");

const hasBle = isValidDevice && DevPhyProtocolMap[DeviceName.toUpperCase()].includes("BLE");

const hasIeee = isValidDevice && DevPhyProtocolMap[DeviceName.toUpperCase()].includes("IEEE");

// Manage protocol support
const has24gProp = hasProp && !(Device.includes("CC131") || isDeviceClass10);

const ConfigPathRclCommon = BasePath + "config/rcl_common/";

// Keep track of PA table generation
// eslint-disable-next-line prefer-const
let paTableDone = false;

const deferred = {
    errs: [],
    warn: [],
    info: [],
    logError: (inst, field, msg) => {
        this.errs.push({inst: inst, field: field, msg: msg});
    },
    logWarning: (inst, field, msg) => {
        this.warn.push({inst: inst, field: field, msg: msg});
    },
    logInfo: (inst, field, msg) => {
        this.info.push({inst: inst, field: field, msg: msg});
    }
};

/**
 *  ======== loadConfiguration ========
 *  Load configuration data of a PHY group
 *
 * @param phyGroup - ble, prop, ieee_154
 * @returns the cached device configuration options for a given device, null on invalid parameters
 */
function loadConfiguration(phyGroup) {
    switch (phyGroup) {
    case PHY_BLE:
        return BLE_CONFIG;
    case PHY_PROP:
        return PROP_CONFIG;
    case PHY_IEEE_15_4:
        return IEEE_CONFIG;
    default:
        return null;
    }
}

/**
 *  ======== readConfigurationFile ========
 *  Read configuration file belonging to a given phy group
 *
 * @param phyGroup - ble, prop, ieee_154
 * @returns the device configuration options for a given device
 */
function readConfigurationFile(phyGroup) {
    let internalGroup = phyGroup;

    // TBD: fix naming inconsistency
    if (phyGroup.includes("ieee")) {
        internalGroup = "ieee";
    }
    const fileName = ConfigPath + `param_syscfg_${internalGroup}_${DeviceName}.json`;
    try {
        const devCfg = system.getScript(fileName);
        return devCfg;
    }
    catch (err) {
        return null;
    }
}

/**
 *  ======== getScript ========
 *  Get a script relative to the application base path.
 *
 *  @param file - Script file name.
 *  @returns The loaded script
 */
function getScript(file) {
    return system.getScript(BasePath + file);
}

/**
 *  ======== getPhyHandler ========
 *  Return the PHY handler for RFD and RCL platforms respectively.
 *
 * @returns rfd or rcl phy handler depending on platform
 */
function getPhyHandler() {
    const file = isPlatformRFD ? "rfd_phy_handler.js" : "rcl_phy_handler.js";
    return getScript(file);
}

/**
 *  ======== isPhyExternalRelease ========
 *  Check whether or not a phy is marked as external relase
 *
 *  @param phyGroup - ble, ieee_15_4 or prop
 *  @param name - PHY id/abbreviation, e.g. "gfsk_2_mbps"
 *  @returns boolean
 */
function isPhyExternalRelease(phyGroup, name) {
    return getPhyHandler().get(phyGroup, name).isExternalRelease();
}

/**
 *  ======== getBoardName ========
 *  Get the SmartRF Studio name of the board
 *
 *  @returns String - Name of the board with prefix /ti/boards and
 *                    suffix .syscfg.json stripped off. If no board
 *                    was specified, an empty string  is returned.
 */
function getBoardName() {
    let boardName = "";

    if (system.deviceData.board != null) {
        boardName = system.deviceData.board.source;

        // Strip off everything up to and including the last '/'
        boardName = boardName.replace(/.*\//, "");

        // Strip off everything after and including the first '.'
        boardName = boardName.replace(/\..*/, "");

        // Convert board name to SmartRF Studio notation
        if (boardName.includes("_LAUNCHXL")) {
            boardName = "LAUNCHXL-" + boardName.replace("_LAUNCHXL", "");
            boardName = boardName.replace("_", "-");
        }
        else if (boardName.includes("LPSTK")) {
            boardName = boardName.replace("LPSTK_CC1352R", "LPSTK-CC1352R1");
        }
        else if (boardName.includes("LP_")) {
            boardName = boardName.replace("P7_", "P7-");
        }
        else {
            throw new Error("RadioConfig: Unknown board [" + boardName + "]");
        }
    }
    // Some board names don't need to be translated, check if the selected one does
    if (boardName in SysconfigRfconfigBoardMap) {
        return SysconfigRfconfigBoardMap[boardName];
    }
    return boardName;
}

/**
 *  ======== isHex ========
 *  Return true if string is a hex value
 *
 *  @param str - string to test against
 *  @returns boolean
 */
function isHex(str) {
    const value = str.replace("0x", "");
    const regexp = /^[0-9a-fA-F]+$/;
    return regexp.test(value);
}

/**
 *  ======== int2hex ========
 *  Convert an integer to a hexadecimal string (0xAB...)
 *
 *  @param num - numerical value or string
 *  @param width - required width of the resulting hexadecimal number
 *
 *  @returns - string with hex prefix (0x)
 */
function int2hex(num, width) {
    // Handle negative numbers
    let numl = num;
    if (numl < 0) {
        numl = 2 ** (width * 4) + Number(num);
    }
    // Convert to HEX string
    const numStr = Number(numl).toString(16);

    // Prepend with 0x000 ...
    return "0x" + numStr.padStart(width, "0").slice(-width).toUpperCase();
}

/**
 *  ======== ucFirst ========
 *  Make the first characters of a string uppercase.
 *
 *  @param str - String to be processed
 *  @returns String with capitalized first letter
 */
function ucFirst(str) {
    return str.charAt(0).toUpperCase() + str.slice(1);
}

/**
 *  ======== fract ========
 *  Calculate the fraction of a floating point number
 *
 *  @param val - floating point number to calculate the fraction of
 *  @returns Fractional part of number
 */
function fract(val) {
    const res = parseFloat(val).toString().split(".");
    return res[1] / 1000;
}

/**
 *  ======== replaceLastChar ========
 *  Replace the last occurrence of character in a string
 *
 *  @param str - String to operate on
 *  @param c1  - Sub-string to replace
 *  @param c2  - Replacement character
 *  @returns string with last occurence of c1 replaced with c2
 */
function replaceLastChar(str, c1, c2 = "") {
    const pos = str.lastIndexOf(c1);
    return str.slice(0, pos) + c2 + str.slice(pos + 1);
}

/**
 *  ======== replaceLastCharBeforeLastSlash ========
 *  Replace the last occurence of a character occuring before the last slash
 *  If input contains no slash, last occurence is replaced
 *
 *  Currently used to remove trailing commas from generated code, but not in comments
 *
 *  @param str - String to operate on
 *  @param c1  - Sub-string to replace
 *  @param c2  - Replacement character
 *  @returns string with first occurence of c1 replaced with c2
 *  @example
 *  // returns "let a = 5 // example comment with comma,"
 *  replaceLastCharBeforeLastSlash("let a = 5, // example comment with comma,", ",")
 */
function replaceLastCharBeforeLastSlash(str, c1, c2 = "") {
    let pos;
    if (str.includes("/")) {
        const lastLine = str.lastIndexOf("/");
        pos = str.lastIndexOf(c1, lastLine);
    }
    else {
        pos = str.lastIndexOf(c1);
    }
    return str.slice(0, pos) + c2 + str.slice(pos + 1);
}

/**
 *  ======== forceArray ========
 *  Force a variable to become an array.
 *
 *  - if array, keep as is
 *  - if not array, create a single element array
 *
 *  @param arg - array or single value
 *  @returns Array with contents
 */
function forceArray(arg) {
    if (Array.isArray(arg)) {
        return arg;
    }
    return [arg];
}

/**
 *  ======== calculateWidth ========
 *  Calculate the width of a byte index
 *
 *  @param range - array of two items describing a range
 *  @returns width of given byte index
 */
function calculateWidth(range) {
    return (range[1] - range[0] + 1);
}

/**
 *  ======== getBitfieldValue ========
 *  Get the bit field of a word based on bit position and field width.
 *
 *  @param word   - word to extract the bit field value from
 *  @param offset - position of the least significant bit of the field
 *  @param width  - width of the bit field
 *  @returns Bit field of given word
 */
function getBitfieldValue(word, offset, width) {
    /* eslint-disable no-bitwise */
    const val = word >> offset;
    const mask = ~(-1 << width);

    return val & mask;
    /* eslint-enable no-bitwise */
}

/**
 *  ======== getFrequencyBand ========
 *  Get the frequency band of this setting
 *
 *  @param phyGroup - ble, prop, ieee_154
 *  @param phyDefFile - name of the PHY definition file
 *  @returns Frequency band of this setting
 */
function getFrequencyBand(phyGroup, phyDefFile) {
    let freqBand = 2400; // BLE, IEEE 802.15.4

    if (phyGroup === PHY_PROP) {
        if (phyDefFile.includes("_779_930_")) {
            freqBand = 868;
        }
        else if (phyDefFile.match(/_431_527_|_359_430_/)) {
            freqBand = 433;
        }
        else if (phyDefFile.includes("_169_")) {
            freqBand = 169;
        }
    }
    return freqBand;
}

/**
 *  ======== getPhyGroup ========
 *  Return the PHY group of an instance (prop, ble, ieee_15_4)
 *
 *  @param inst - module instance object
 *  @returns phy group or null if none is found
 */
function getPhyGroup(inst) {
    const name = inst.$module.$name;
    let phyGroup = null;

    if (name.includes("ble")) {
        phyGroup = "ble";
    }
    else if (name.includes("prop")) {
        phyGroup = "prop";
    }
    else if (name.includes("ieee")) {
        phyGroup = "ieee_15_4";
    }
    else {
        throw Error("PHY group can not be derived from: " + name);
    }
    return phyGroup;
}

/**
 *  ======== getPhyType ========
 *  Return the PHY type of an instance based on the selected frequency band.
 *
 *  @param inst   - module instance object
 *  @returns inst phy type based on frequency band
 */
function getPhyType(inst) {
    switch (inst.freqBand) {
    case "868":
        return inst.phyType868;
    case "433":
        return inst.phyType433;
    case "169":
        return inst.phyType169;
    case "2400":
        return inst.phyType2400;
    default:
    }
    return inst.phyType;
}

/**
 * ======== flattenConfigs ========
 * Make an array of depth one from a group of configurables that
 * may be a tree structure.
 *
 *  @param configList - object that contains grouped configurables
 *  @returns flattened array
 */
function flattenConfigs(configList) {
    const flatConfigList = [];

    _.forEach(configList, (config) => {
        if (_.has(config, "config")) {
            _.forEach(config.config, (subconfig) => {
                flatConfigList.push(subconfig);
            });
        }
        else {
            flatConfigList.push(config);
        }
    });

    return flatConfigList;
}

/**
 * ======== getCoexConfig ========
 * Return the Co-ex config structure if the device support BLE/Wi-Fi Coex,
 * otherwise return null;
 *
 * @returns Co-ex config structure or null
 */
function getCoexConfig() {
    // CoEx config applies only to RF Driver
    if (!isPlatformRFD) {
        return null;
    }

    const modules = system.modules;
    let coexPhy = null;

    // Iterate RadioConfig modules
    _.each(modules, (mod) => {
        if (mod.$name.includes("radioconfig/phy_groups/ble")) {
            coexPhy = "ble";
        }
        if (mod.$name.includes("radioconfig/phy_groups/ieee")) {
            coexPhy = "ieee_15_4";
        }
    });

    if (coexPhy) {
        const RF = system.modules["/ti/drivers/RF"];
        const CoExConfig = RF.getCoexConfig(coexPhy);
        return CoExConfig.coExEnable.bCoExEnable === 1 ? CoExConfig : null;
    }
    return null;
}

/**
 * ======== initLongDescription ========
 * Searches through the docs object to find the correct longDescription
 *
 *  @param configurable - configurable to apply the long description to
 *  @param docs   - documentation object
 */
function initLongDescription(configurable, docs) {
    /**
     * Set longDescription for item
     *
     * @param item - configurable item with no sub-items
     */
    function setDocs(item) {
        const configurableDocs = _.find(docs, ["configurable", item.name]);
        // If the configurable has a longDescription, add it to the configurable
        if (configurableDocs) {
            item.longDescription = configurableDocs.longDescription;
        }
        else {
            // If the configurable still doesn't got a text, loop through all docs
            // to find if it is in a array
            _.forEach(docs, (docItem) => {
                if (_.isArray(docItem.configurable) && _.includes(docItem.configurable, item.name)) {
                    item.longDescription = docItem.longDescription;
                }
            });
        }
    }
    // Loop through all configurables
    _.each(configurable, (item) => {
        if (_.has(item, "config")) {
            _.each(item.config, (subItem) => {
                setDocs(subItem);
            });
        }
        else {
            setDocs(item);
        }
    });
}

/**
 *  ======== validateTxPower ========
 *  Validate TX Power options
 *
 *  @param inst   - module instance object
 *  @param validation   - validation object
 */
function validateTxPower(inst, validation) {
    // validation for txPowerHi
    if ("txPowerHi" in inst) {
        if (inst.highPA) {
            const validOptions = inst.$module.$configByName.txPowerHi.options(inst);
            const selectedOptions = inst.txPowerHi;
            const found = _.find(validOptions, (o) => o.name === selectedOptions);
            if (!found) {
                validation[`log${"Error"}`]("Selected option "
                    + selectedOptions + " is invalid, please reselect.", inst, "txPowerHi");
            }
        }
    }
}

/**
 *  ======== validateNames ========
 *  Validate that all names defined by inst are globally unique and
 *  valid C identifiers.
 *
 *  @param inst   - module instance object
 *  @param validation   - validation object
 */
function validateNames(inst, validation) {
    const myNames = {}; /* all C identifiers defined by inst */

    // check that $name is a C identifier
    if (inst.$name !== "") {
        const token = inst.$name;
        if (!isCName(token)) {
            logError(validation, inst, "$name", "'" + token + "' is not a valid a C identifier");
        }
        myNames[token] = 1;
    }

    // check that cAliases are all C identifiers and there are no duplicates
    let tokens = [];
    /* eslint-disable-next-line */
    if ("cAliases" in inst && inst.cAliases != "") {
        tokens = inst.cAliases.split(/[,;\s]+/);
    }

    for (let i = 0; i < tokens.length; i++) {
        const token = tokens[i];
        if (!isCName(token)) {
            logError(validation, inst, "cAliases", "'" + token + "' is not a valid a C identifier");
        }
        if (myNames[token] !== null) {
            logError(validation, inst, "cAliases", "'" + token + "' is defined twice");
        }
        myNames[token] = 1;
    }

    // ensure all inst C identifiers are globally unique
    const mods = system.modules;
    /* eslint-disable-next-line */
    for (const i in mods) {
        // for all instances in all modules
        const instances = mods[i].$instances;
        for (let j = 0; j < instances.length; j++) {
            const other = instances[j];

            // skip self
            if (inst.$name !== other.$name) {
                // compute all other names
                let name = other.$name;
                if (name !== "" && name in myNames) {
                    logError(validation, inst, "cAliases", "multiple instances with the same name: '"
                        + name + "': " + inst.$name + " and " + other.$name);
                    break;
                }
                /* eslint-disable-next-line */
                if (other.cAliases != null && other.cAliases != "") {
                    tokens = other.cAliases.split(/[,;\s]+/);
                    for (let k = 0; k < tokens.length; k++) {
                        name = tokens[k];
                        if (name !== "" && name in myNames) {
                            logError(validation, inst, "cAliases", "multiple instances with the same name: '" + name
                                + "': " + inst.$name + " and " + other.$name);
                            break;
                        }
                    }
                }
            }
        }
    }
}

/**
 *  ======== logError ========
 *  Log a new error
 *
 *  @param vo     - a validation object passed to the validate() method
 *  @param inst   - module instance object
 *  @param field  - instance property name, or array of property names, with
 *                  which this error is associated
 *  @param msg    - message to display
 */
function logError(vo, inst, field, msg) {
    let lvo = vo;

    /* eslint-disable-next-line */
    if (typeof global !== "undefined" && global.__coverage__) {
        lvo = deferred;
    }
    if (typeof (field) === "string") {
        lvo.logError(msg, inst, field);
    }
    else {
        for (let i = 0; i < field.length; i++) {
            lvo.logError(msg, inst, field[i]);
        }
    }
}

/**
 *  ======== logInfo ========
 *  Log a new remark
 *
 *  @param vo     - a validation object passed to the validate() method
 *  @param inst   - module instance object
 *  @param field  - instance property name, or array of property names, with
 *                  which this remark is associated
 *  @param msg    - message to display
 */
function logInfo(vo, inst, field, msg) {
    let lvo = vo;

    /* eslint-disable-next-line */
    if (typeof global !== "undefined" && global.__coverage__) {
        lvo = deferred;
    }
    if (typeof (field) === "string") {
        lvo.logInfo(msg, inst, field);
    }
    else {
        for (let i = 0; i < field.length; i++) {
            lvo.logInfo(msg, inst, field[i]);
        }
    }
}

/**
 *  ======== logWarning ========
 *  Log a new warning
 *
 *  @param vo     - a validation object passed to the validate() method
 *  @param inst   - module instance object
 *  @param field  - instance property name, or array of property names, with
 *                  which this warning is associated
 *  @param msg    - message to display
 */
function logWarning(vo, inst, field, msg) {
    let lvo = vo;

    /* eslint-disable-next-line */
    if (typeof global !== "undefined" && global.__coverage__) {
        lvo = deferred;
    }
    if (typeof (field) === "string") {
        lvo.logWarning(msg, inst, field);
    }
    else {
        for (let i = 0; i < field.length; i++) {
            lvo.logWarning(msg, inst, field[i]);
        }
    }
}

/**
 *  ======== isCName ========
 *  Determine if specified id is either empty or a valid C identifier
 *
 *  @param id  - String that may/may not be a valid C identifier
 *
 *  @returns true if id is a valid C identifier OR is the empty
 *           string; otherwise false.
 */
function isCName(id) {
    if ((id !== null && id !== undefined && id.match(/^[a-zA-Z_][0-9a-zA-Z_]*$/) !== null)
        || id === "") { /* "" is a special value that means "default" */
        return true;
    }
    return false;
}

/**
 *  ======== autoForceModules ========
 *  Returns an implementation of a module's modules method that just
 *  forces the addition of the specified modules
 *
 *  @param args An array of module name strings.
 *
 *  @returns An array with module instance objects
 *
 *  @example
 *  modules: Common.autoForceModules(["Board", "DMA"])
 */
function autoForceModules(args) {
    return (function() {
        const modArray = [];

        if (args === undefined || args === null || !Array.isArray(args)) {
            return modArray;
        }

        for (let arg = args.length - 1; args >= 0; arg--) {
            let modPath = args[arg];
            if (modPath.indexOf("/") === -1) {
                modPath = "/ti/drivers/" + modPath;
            }
            modArray.push({
                name: modPath.substring(modPath.lastIndexOf("/") + 1),
                moduleName: modPath,
                hidden: true
            });
        }
        return modArray;
    });
}

/**
 *  ======== getPhyInfo  ========
 *  Get the information about a given PHY (PHY definition file and description)
 *
 *  @param phyList  - List of available PHY
 *  @param phyGroup - Group where to search ("ble", "prop" ...)
 *  @param phyName  - PHY id (abbreviation) to look for
 *  @returns PHY definition file and PHY description
 */
function getPhyInfo(phyList, phyGroup, phyName) {
    let item = null;

    if (phyGroup === "prop") {
        const groups = ["prop868", "prop433", "prop2400", "prop169"];
        for (const pg of groups) {
            if (pg in phyList) {
                const items = phyList[pg];
                item = items.filter((phy) => phy.name === phyName);
                if (item.length > 0) {
                    break;
                }
            }
        }
    }
    else {
        const pg = phyGroup.replace("_15_4", "");
        const items = phyList[pg];
        item = items.filter((phy) => phy.name === phyName);
    }

    if (item === null) {
        throw Error("PHY name not found:" + phyName);
    }

    return {
        file: item[0].file8,
        label: item[0].label
    };
}

/**
 *  ======== getOptions ========
 *  Get the options array of a configurable.
 *
 * @param configs - Configurable array
 * @param name - Name of the configurable
 * @returns boolean, or null, if configs is empty
 */
function getOptions(configs, name) {
    let opts = null;

    _.each(configs, (item) => {
        if (item.name === name) {
            if ("options" in item) {
                opts = item.options;
            }
            return false;
        }
        else if (_.has(item, "config")) {
            let returns;
            _.each(item.config, (subItem) => {
                if (subItem.name === name) {
                    if ("options" in subItem) {
                        opts = subItem.options;
                    }
                    returns = false;
                    return returns;
                }
                // Continue _.each iteration
                return true;
            });
            return returns;
        }
        // Continue _.each iteration
        return true;
    });
    return opts;
}

// Constants for channel&frequency
const BLE_ADV_CHAN_37 = 37;
const BLE_ADV_CHAN_38 = 38;
const BLE_ADV_CHAN_39 = 39;

const BLE_ADV_FREQ_37 = 2402;
const BLE_ADV_FREQ_38 = 2426;
const BLE_ADV_FREQ_39 = 2480;

const BLE_BASE_FREQ = 2404;
const BLE_MAX_FREQ = 2480;
const BLE_CHAN_INTV = 2;
const BLE_BASE_CHAN_RAW = 0x68;

const IEEE_15_4_BASE_FREQ = 2405;
const IEEE_15_4_MAX_FREQ = 2480;
const IEEE_15_4_BASE_CHAN = 11;
const IEEE_15_4_CHAN_INTV = 5;

/**
 *  ======== bleFreqToChan ========
 * Function to convert from frequency (MHz) to BLE channel (0-39)
 *
 * @param freq - frequency
 * @returns channel switched to
 */
function bleFreqToChan(freq) {
    let chan;

    switch (freq) {
    // Advertising channels
    case BLE_ADV_FREQ_37:
        chan = BLE_ADV_CHAN_37;
        break;
    case BLE_ADV_FREQ_38:
        chan = BLE_ADV_CHAN_38;
        break;
    case BLE_ADV_FREQ_39:
        chan = BLE_ADV_CHAN_39;
        break;
    default:
        // data channels
        if (freq >= BLE_BASE_FREQ && freq < BLE_ADV_FREQ_38) {
            chan = (freq - BLE_BASE_FREQ) / BLE_CHAN_INTV;
        }
        else if (freq < BLE_MAX_FREQ) {
            // adjust for advertising channel 38 (2426)
            chan = ((freq - BLE_BASE_FREQ)) / BLE_CHAN_INTV - 1;
        }
        else {
            throw Error("Invalid BLE frequency: " + freq);
        }
    }
    return chan;
}

/**
 *  ======== bleFreqToChanRaw ========
 * Function to convert from frequency (MHz) to raw BLE channel value
 *
 * @param freq - frequency
 * @returns raw BLE channel value
 */
function bleFreqToChanRaw(freq) {
    return (freq - BLE_BASE_FREQ) + BLE_BASE_CHAN_RAW;
}

/**
 *  ======== ieee154FreqToChan ========
 * Function to convert from frequency (MHz) to IEEE 802.15.4 channel (11-16)
 *
 * @param freq - frequency
 * @returns IEEE 802.15.4 channel
 */
function ieee154FreqToChan(freq) {
    if (freq >= IEEE_15_4_BASE_FREQ && freq <= IEEE_15_4_MAX_FREQ) {
        if ((freq % IEEE_15_4_CHAN_INTV) === 0) {
            return IEEE_15_4_BASE_CHAN + (freq - IEEE_15_4_BASE_FREQ) / IEEE_15_4_CHAN_INTV;
        }
    }
    throw Error("Invalid IEEE 802.15.4 frequency: " + freq);
}

/**
 * Get device family which is used in mapping register settings common for loki low etc
 *
 * @returns name of device family, currently cc23xx or cc27xx
 */
// TODO - Find a way to remove all references to hand-maintained device family
// https://jira.itg.ti.com/browse/SLWSTUDIO-1955
function getDeviceFamily() {
    return DevFamilyMap[DeviceName];
}

// Exported from this module
exports = {
    getScript: getScript,
    ConfigPath: ConfigPath,
    ConfigPathRclCommon: ConfigPathRclCommon,
    basePath: BasePath,
    phyGroupPath: PhyGroupPath,
    Device: Device,
    DeviceName: DeviceName,
    PHY_BLE: PHY_BLE,
    PHY_PROP: PHY_PROP,
    PHY_IEEE_15_4: PHY_IEEE_15_4,
    HAS_PROP: hasProp,
    HAS_BLE: hasBle,
    HAS_24G: hasBle,
    HAS_IEEE_15_4: hasIeee,
    HAS_24G_PROP: has24gProp,
    loadConfiguration: loadConfiguration,
    paTableDone: paTableDone,
    getDeviceFamily: getDeviceFamily,
    isSub1gDevice: () => Device.includes("CC13") || Device.match("CC267[24]"),
    isSub1gOnlyDevice: () => Device.includes("CC131"),
    is24gOnlyDevice: () => Device.match(/CC26[457]|CC2340/) && !Device.match("CC267[24]"),
    isDeviceClass3: () => Device.match(/1[PR]3R|CC2651R3SIPA/) !== null,
    isDeviceClass7: () => Device.match(/2[PR]7R/) !== null,
    isDeviceClass10: () => isDeviceClass10,
    isDeviceStandard: () => Device.match(/2[PR]B?1F|CC1312PSIP/),
    isDevice23x0: () => Device.match(/CC2340/),
    isPlatformRFD: () => isPlatformRFD,
    isPlatformRCL: () => !isPlatformRFD,
    FreqLower169: 169.4,
    FreqHigher169: 169.475,
    FreqLower433: 420,
    FreqHigher433: 528,
    FreqLower24G: 2400,
    int2hex: int2hex,
    isHex: isHex,
    ucFirst: ucFirst,
    fract: fract,
    replaceLastChar: replaceLastChar,
    replaceLastCharBeforeLastSlash: replaceLastCharBeforeLastSlash,
    forceArray: forceArray,
    calculateWidth: calculateWidth,
    getBitfieldValue: getBitfieldValue,
    getBoardName: getBoardName,
    getPhyType: getPhyType,
    getPhyGroup: getPhyGroup,
    getCoexConfig: getCoexConfig,
    isPhyExternalRelease: isPhyExternalRelease,
    validateBasic: (inst, validation) => {
        validateNames(inst, validation);
        validateTxPower(inst, validation);
    },
    flattenConfigs: flattenConfigs,
    getOptions: getOptions,
    logError: logError,
    logWarning: logWarning,
    logInfo: logInfo,
    isCName: isCName,
    initLongDescription: initLongDescription,
    autoForceModules: autoForceModules,
    getFrequencyBand: getFrequencyBand,
    getPhyHandler: getPhyHandler,
    getPhyInfo: getPhyInfo,
    bleFreqToChan: bleFreqToChan,
    bleFreqToChanRaw: bleFreqToChanRaw,
    ieee154FreqToChan: ieee154FreqToChan
};
