/* eslint-disable guard-for-in */
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
 *  ======== rfd_phy_handler.js ========
 *  Module to process RFD PHY definitions
 */

"use strict";

// Common utility functions
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");
const ConfigPath = Common.ConfigPath;

// Other dependencies
const DeviceInfo = Common.getScript("device_info.js");
const RfDesign = Common.getScript("rfdesign");
const CmdHandler = Common.getScript("cmd_handler.js");
const OverrideHandler = Common.getScript("override_handler.js");

// Utility functions shared with SmartRF Studio 8
const Srfs8Util = system.getScript(ConfigPath + "srfs8_util.js");

// Ensure that PHY Handlers are not created twice
const PhyHandlerCache = {};

// TX power cache
const TxPowerCache = {
    default: "-1",
    t169: "-1",
    t433: "-1",
    t2400: "-1",
    high: "-1",
    t433Hi: "-1"
};

// Constants
const BLE_DEVICE_ADDRESS_LENGTH = 6;
const IS_HPA_DEVICE = DeviceInfo.hasHighPaSupport();

// Utility functions
const int2hex = Common.int2hex;

// Exported functions
exports = {
    get: get,
    getUpdatedRfCommands: getUpdatedRfCommands
};

/**
 *  ======== get ========
 *  Get the Command Handler for a PHY layer. Create the
 *  Command Handler if it does not already exist.
 *
 *  @param phyGroup - ble, prop, ieee_154
 *  @param phyID - short name for the PHY definition
 *  @useSelectivity - set true if sensitivity preferred to selectivity
 */
function get(phyGroup, phyID, useSelectivity = false) {
    let key = phyID;

    if (useSelectivity) {
        key += "H";
    }
    if (!(key in PhyHandlerCache)) {
        PhyHandlerCache[key] = create(phyGroup, phyID, true, useSelectivity);
    }

    return PhyHandlerCache[key];
}

/**
 *  ======== getUpdatedRfCommands ========
 *  Create a new PHY setting and update with data from the instance.
 *  This leaves the original PHY setting unchanged.
 *
 *  @param inst - inst from which update data are fetched
 *  @param phyGroup - BLE, IEEE_15_4 or PROP
 */
function getUpdatedRfCommands(inst, phyGroup) {
    const phyID = Common.getPhyType(inst, phyGroup);
    const useSelectivity = inst.settingGroup === "Selectivity";

    // Update performed on the duplicate instance
    const phyClone = create(phyGroup, phyID, false, useSelectivity);
    phyClone.updateRfCommands(inst);

    return phyClone;
}

/**
 *  ======== create ========
 *  Create a PHY specific instance of the PHY handler, using SmartRF Studio 8 data.
 *
 *  @param phyGroup - ble, prop, ieee_154
 *  @param phyID - short name for the PHY definition
 *  @param first - first time execution
 *  @param useSelectivity - set true if sensitivity preferred to selectivity
 */
function create(phyGroup, phyID, first, useSelectivity = false) {
    const PhyID = phyID;
    const PhyGroup = phyGroup;

    // RF property to RF command mapping
    let fileData = system.getScript(ConfigPath + `param_cmd_mapping_${phyGroup}.json`);
    const ParamMap = addParameterMapping(fileData);

    // Get configuration for PHY group
    const DeviceConfig = DeviceInfo.getConfiguration(phyGroup);

    // Load PHY definition file
    const PhyInfo = Common.getPhyInfo(DeviceConfig.phys, phyGroup, phyID);
    let PhyDefFile = PhyInfo.file;

    // Check if optimized for selectivity
    if (useSelectivity) {
        PhyDefFile = PhyDefFile.replace(".json", "_sel.json");
    }
    fileData = system.getScript(ConfigPath + PhyDefFile);
    const Setting = _.cloneDeep(fileData.phy_def);

    // Create command handler
    const cmdHandler = CmdHandler.get(fileData.phy_def, phyGroup, phyID);
    const setCmdFieldValueByOpt = cmdHandler.setCmdFieldValueByOpt;
    const getCmdFieldValueByOpt = cmdHandler.getCmdFieldValueByOpt;
    const setCmdFieldValue = cmdHandler.setCmdFieldValue;
    const getCmdFieldByName = cmdHandler.getCmdFieldByName;
    const setCmdFieldValueDirect = cmdHandler.setCmdFieldValueDirect;
    const getCommandField = cmdHandler.getCommandField;

    // Get the frequency band (embedded in file name)
    const FreqBand = Common.getFrequencyBand(phyGroup, PhyDefFile);

    // True if txPower configurable for 2.4 GHz band is used (only for 2.4GHz proprietary settings)
    const UseTxPower2400 = PhyGroup === Common.PHY_PROP && Common.HAS_24G_PROP && FreqBand === 2400;

    // RF parameters (PHY properties)
    let RfParamCache = {};

    // RF properties, indexed by property name, e.g. "whitening"
    const RfPropMap = {};

    // Store for manual patches
    let ManualPatch = {};

    if (phyID in PhyHandlerCache) {
        ManualPatch = PhyHandlerCache[phyID].getManualPatch();
    }

    // Storage for TX power value for high PA (DBm)
    const TxPowerHi = {
        dbm: "",
        raw: "0"
    };

    // Get RF properties from SmartRF Studio 8 (PHY and Test properties)
    initRfProperties();

    // SmartRF Studio 8 shared JavaScript
    const exp = {
        getPhyProperty: getPhyProperty,
        getRfdCommandField: getRfdCommandField,
        getRfdOverrideParam: (arg) => OverrideHandler.getRepetitionFactor()
    };

    // Handle to shared JavaScript
    const UtilHandle = Srfs8Util.get(PhyGroup, PhyID, exp);

    // Initialize with values from the PHY definition file
    initRfParamCache();

    if (first) {
        // Apply defaults to configurables
        initConfigurables(DeviceConfig.configs);
    }

    // Ensure that RF command fields are updated
    updateRfCommands(RfParamCache);

    // Refresh cache
    getRfData();

    /**
     *  ======== addParameterMapping  ========
     *  Map RF parameters to RF commands. An RF parameter
     *  may be mapped to multiple RF commands (e.g. txPower).
     *
     *  @param - map of parameter versus RF commands
     */
    function addParameterMapping(map) {
        const paramMap = [];

        for (const parItem of map.mapping.settings.parameters.parameter) {
            const params = parItem._names.split(",");
            for (const par of params) {
                const cmds = parItem.rfCommand;
                const pmap = Common.forceArray(cmds);
                let lpar = par;
                if (par === "CARRIER_FREQUENCY") {
                    lpar = "CARRIERFREQUENCY";
                }
                paramMap[lpar.toUpperCase()] = pmap;
            }
        }
        return paramMap;
    }

    /**
     *  ======== getRfPropertyByName ========
     *  Initialize the RF parameters cache with values
     *  from the PHY definition file (PHY properties)
     *
     *  @param name  - RF property name
     *
     */
    function getRfPropertyByName(name) {
        if (name in RfPropMap) {
            return RfPropMap[name];
        }
        return null;
    }

    /**
     *  ======== initRfProperties ========
     *  Initialize the RF properties array
     */
    function initRfProperties() {
        // Join all RF properties
        let data = Common.forceArray(Setting.phy_properties);

        if ("test_properties" in Setting) {
            data = data.concat(Setting.test_properties);
        }
        // Create RF property map
        for (const group of data) {
            const properties = Common.forceArray(group.property);
            for (const prop of properties) {
                RfPropMap[prop.name] = prop;
            }
        }
    }

    /**
     *  ======== initRfParamCache ========
     *  Initialize the RF parameters cache with values
     *  from the PHY definition file (PHY properties)
     */
    function initRfParamCache() {
        // For backwards compatibility (TBD: consider removing)
        if (PhyGroup === Common.PHY_BLE) {
            RfParamCache.packetLengthBle = 30;
            RfParamCache.whitening = true;
        }

        // Differences in PHY property names between RadioConfig and SRFS8
        const phyPropTrans = {
            frequency: "carrierFrequency",
            rxAddress0: "address0",
            rxAddress1: "address1"
        };

        for (const property of Object.values(RfPropMap)) {
            let name = property.name;

            if (PhyGroup === Common.PHY_PROP && name in phyPropTrans) {
                name = phyPropTrans[name];
            }
            const type = property.type;
            const value = property.value;
            let fmtValue = value;

            if (type === "dec") {
                const opts = Common.getOptions(DeviceConfig.configs, name);
                fmtValue = parseInt(value);

                if (opts !== null) {
                    // Use drop-down list
                    _.each(opts, (opt) => {
                        if (opt.key === fmtValue) {
                            fmtValue = opt.name;
                        }
                    });
                }
            }
            else if (type === "float") {
                fmtValue = parseFloat(value);
            }
            else if (type === "hex") {
                fmtValue = parseInt(value, 16);
            }

            if (!(name in RfParamCache)) {
                RfParamCache[name] = fmtValue;
            }
        }
    }

    /**
     *  ======== initConfigurables ========
     *  Initialize the configurables with default values and options
     *
     *  @param config  - array of configurables
     */
    function initConfigurables(config) {
        const isProp = PhyGroup === Common.PHY_PROP;
        const freq = isProp ? RfParamCache.carrierFrequency : RfParamCache.frequency;

        // 1. Create options list for TxPower configurables
        function initTxPowerConfigs(item) {
            switch (item.name) {
            case "txPower":
                // 868 + BLE/IEEE 802.15.4 use "txPower"
                if (FreqBand === 868 || !isProp) {
                    item.options = RfDesign.getTxPowerOptions(freq, false);
                    item.default = item.options[0].name;
                    TxPowerCache.default = item.default;
                }
                break;
            case "txPower169":
                // 169 MHz band uses "txPower169"
                if (FreqBand === 169) {
                    item.options = RfDesign.getTxPowerOptions(169, false);
                    item.default = item.options[0].name;
                    TxPowerCache.t169 = item.default;
                }
                break;
            case "txPower433":
                // 420-527 MHz band uses "txPower433"
                if (FreqBand === 433) {
                    item.options = RfDesign.getTxPowerOptions(433, false);
                    item.default = item.options[0].name;
                    TxPowerCache.t433 = item.default;
                }
                break;
            case "txPower2400":
                // 2400 - 2480 MHz (proprietary only) use "txPower2400"
                if (UseTxPower2400) {
                    item.options = RfDesign.getTxPowerOptions(2400, false);
                    item.default = item.options[0].name;
                    TxPowerCache.t2400 = item.default;
                }
                break;
            case "txPowerHi":
                // CC1352P: 868 + 2400 MHz bands use "txPowerHi" for High PA
                if (FreqBand === 868 || FreqBand === 2400) {
                    // NB! Using dynamic enumerable due to 10 dBm PA table for P4 Launchpad and CC2652PSIP
                    const txOptions = RfDesign.getTxPowerOptions(freq, true);
                    item.options = (inst) => RfDesign.getTxPowerOptions(freq, true);
                    if (txOptions.length > 0) {
                        item.default = txOptions[0].name;
                    }
                    TxPowerCache.high = item.default;
                }
                break;
            case "txPower433Hi":
                // CC1352P4: 433 MHz band uses "txPower433Hi" for High PA (obsolete). TBD: remove
                if (FreqBand === 433) {
                    item.options = [
                        {name: "13"}
                    ];
                    item.default = item.options[0].name;
                    TxPowerCache.t433Hi = item.default;
                }
                break;
            default:
                break;
            }
        }

        // Initialize TX power configurables
        for (const item of config) {
            if ("config" in item) {
                for (const subItem of item.config) {
                    initTxPowerConfigs(subItem);
                }
            }
            else {
                initTxPowerConfigs(item);
            }
        }

        const rf = RfParamCache;
        // Initialize the other configurables
        function initOtherConfigs(item) {
            if (item.name in rf) {
                if (!item.name.includes("txPower")) {
                    item.default = rf[item.name];
                }
            }
        }

        for (const item of config) {
            if ("config" in item) {
                for (const subItem of item.config) {
                    initOtherConfigs(subItem);
                }
            }
            else {
                initOtherConfigs(item);
            }
        }
    }

    /**
      *  ======== getPhyProperty ========
      *  Support function for SmartRF Studio 8 shared JavaScript
      *
      *  @param name - Name of PHY property (SRFS8)
      */
    function getPhyProperty(name) {
        if (name in RfParamCache) {
            const ppVal = RfParamCache[name];
            const prop = getRfPropertyByName(name);
            if ("enum" in prop) {
                const val = getOptByName(name, ppVal);
                return val;
            }
            return ppVal;
        }
        else if (name === "bleChannel") {
            // 'bleChannel' isn't a RadioConfig configurable
            const freq = getCmdFieldValueByOpt("frequency", "frequency");
            return Common.bleFreqToChan(freq);
        }
        else if (name === "ieeeChannel") {
            // 'ieeeChannel' isn't a RadioConfig configurable
            const freq = getCmdFieldValueByOpt("frequency", "frequency");
            return Common.ieee154FreqToChan(freq);
        }
        throw Error("Unknown PHY property: " + name);
    }

    /**
     *  ======== getRfData ========
     *  Get initial settings
     */
    function getRfData() {
        const txPower = getTxPowerAll();
        const freq = getFrequency();

        // Update TX power override
        const txPowActual = getTxPower(txPower);
        const highPA = cmdHandler.getCommandField("txPower") === "0xFFFF";
        OverrideHandler.init(Setting.rfd_command, highPA);
        OverrideHandler.updateTxPowerOverride(txPowActual, freq, txPower.high);

        const cfgCommon = {};

        if (IS_HPA_DEVICE) {
            cfgCommon.txPowerHi = txPower.high;
        }

        if (PhyGroup === Common.PHY_PROP) {
            let cfgPropBase = {
                whitening: getWhitening(),
                symbolRate: parseFloat(getSymbolRate()),
                deviation: parseFloat(getDeviation()),
                rxFilterBw: getRxFilterBw(),
                carrierFrequency: parseFloat(freq),
                packetLengthRx: parseInt(getPacketLengthRx()),
                syncWord: parseInt(getSyncWord()),
                syncWordLength: getSyncWordLength(),
                preambleCount: getPreambleCount(),
                preambleMode: getPreambleMode()
            };

            const cfgTxPower = {};
            if (UseTxPower2400 && FreqBand === 2400) {
                cfgTxPower.txPower2400 = txPower.t2400;
            }
            else if (FreqBand === 169) {
                cfgTxPower.txPower169 = txPower.t169;
            }
            else if (FreqBand === 433) {
                cfgTxPower.txPower433 = txPower.t433;
            }
            else {
                cfgTxPower.txPower = txPower.default;
            }
            cfgPropBase = {...cfgPropBase, ...cfgTxPower};

            let cfgPropExtended = {};
            if (cmdHandler.cmdIsUsed("CMD_PROP_RX")) {
                cfgPropExtended = {
                    fixedPacketLength: parseInt(getFixedPacketLength()),
                    packetLengthConfig: getPktConf(),
                    addressMode: getAddressMode(),
                    address0: parseInt(getAddress0()),
                    address1: parseInt(getAddress1())
                };
            }
            RfParamCache = Object.assign(cfgCommon, cfgPropBase, cfgPropExtended);
        }
        else if (PhyGroup === Common.PHY_BLE) {
            const cfgBle = {
                txPower: txPower.default,
                frequency: freq,
                whitening: getWhitening()
            };
            RfParamCache = Object.assign(cfgCommon, cfgBle);
        }
        else if (phyGroup === Common.PHY_IEEE_15_4) {
            const cfg154 = {
                txPower: txPower.default,
                frequency: freq
            };
            RfParamCache = Object.assign(cfgCommon, cfg154);
        }

        return RfParamCache;
    }

    /**
     *  ======== executePhyPropertyUpdate ========
     *  Execute JavaScript code to update an RF property.
     *
     *  @param rfProp - Name of RF property (SRFS8)
     *
     *  @returns Value of the executed script
     */
    function executePhyPropertyUpdate(rfProp) {
        const prop = getRfPropertyByName(rfProp);
        if (prop !== null && "script" in prop) {
            const scripts = Common.forceArray(prop.script);
            const updScript = scripts[0];

            if (updScript.role.match(/update/)) {
                let code;

                if ("call" in updScript) {
                    // Call to function
                    code = "UtilHandle." + updScript.call;
                }
                else {
                    // In-line script
                    code = updScript["#text"];
                }
                // eslint-disable-next-line no-eval
                let val = eval(code);
                if (prop.type === "float") {
                    val = parseFloat(val).toFixed(prop.decimals);
                }
                else if ("enum" in prop) {
                    val = getOptByKey(rfProp, val);
                }
                return val;
            }
        }
        throw Error("RF property update failed: " + rfProp);
    }

    /**
     *  ======== validatePhyProperty ========
     *  Validate a PHY property
     *
     *  @param phyProp - Name of PHY property (SRFS8)
     */
    function validatePhyProperty(phyProp) {
        const prop = getRfPropertyByName(phyProp);
        if (prop !== null && "script" in prop) {
            const scripts = Common.forceArray(prop.script);
            // Requires validation ?
            if (scripts.length > 1) {
                const valScript = scripts[1];
                if (valScript.role.match(/validate_error/)) {
                    if ("call" in valScript) {
                        // Call to function
                        const code = "UtilHandle." + valScript.call;
                        // eslint-disable-next-line no-eval
                        const val = eval(code);
                        if (val !== "") {
                            // Validation failed
                            const status = {
                                valid: false,
                                msg: val,
                                config: phyProp
                            };
                            return status;
                        }
                        // Validation passed
                        return null;
                    }
                }
            }
        }
        throw Error("PHY property validation failed: " + phyProp);
    }

    /**
     *  ======== getFrequency ========
     *  Calculate Frequency in MHz based on raw CMD value
     *  (CMD_FS, frequency and fraction fields)
     *
     *  @returns number as string - frequency
     */
    function getFrequency() {
        if (PhyGroup === Common.PHY_PROP) {
            return executePhyPropertyUpdate("frequency");
        }
        // BLE and IEEE 802.15.4
        return cmdHandler.getCmdFieldValueByOpt("frequency", "frequency");
    }

    /**
     *  ======== getSymbolRate ========
     *  Calculate symbol rate
     *
     *  @returns number as string - frequency
     */
    function getSymbolRate() {
        return executePhyPropertyUpdate("symbolRate");
    }

    /**
     *  ======== getDeviation ========
     *  Calculate deviation based on raw CMD value
     *  (CMD_PROP_RADIO_DIV_SETUP)
     *
     *  @returns number as string - deviation
     */
    function getDeviation() {
        return executePhyPropertyUpdate("deviation");
    }

    /**
     *  ======== getWhitening ========
     *  Get Whitening as a descriptive string ("No whitening", "PN9 whitening" ....)
     */
    function getWhitening() {
        if (PhyGroup === Common.PHY_PROP) {
            return executePhyPropertyUpdate("whitening");
        }
        else if (PhyGroup === Common.PHY_BLE) {
            const val = cmdHandler.getCommandField("whitening.init");
            return val !== 0;
        } // Not applicable to IEEE 802.15.4
        throw Error("Whitening not applicable to IEEE 802.15.4-2006");
    }

    /**
     *  ======== getTxPower ========
     *  Get actual Tx Power value (dBm) according to frequency
     *  band and front-end PA state.
     *
     *  @txPowerAll - object containing TX power settings
     */
    function getTxPower(txPowerAll) {
        const freq = getFrequency();
        const freq433 = freq > Common.FreqHigher169 && freq < Common.FreqHigher433;
        const freq169 = freq < Common.FreqHigher169;
        let ret;

        if (IS_HPA_DEVICE) {
            if (UseTxPower2400) {
                ret = txPowerAll.t2400;
            }
            else {
                ret = freq433 ? txPowerAll.t433Hi : txPowerAll.high;
            }
        }
        else if (UseTxPower2400) {
            ret = txPowerAll.t2400;
        }
        else if (freq169) {
            ret = txPowerAll.t169;
        }
        else {
            ret = freq433 ? txPowerAll.t433 : txPowerAll.default;
        }
        return ret;
    }

    /**
     *  ======== getTxPowerFromInst ========
     *  Get actual Tx Power value (dBm) according to frequency
     *  band and front-end PA state.
     *
     *  @param inst - current instance
     */
    function getTxPowerFromInst(inst) {
        const freq = getFrequency();
        const freq433 = freq > Common.FreqHigher169 && freq < Common.FreqHigher433;
        const freq169 = freq < Common.FreqHigher169;

        let ret;

        if ("highPA" in inst && inst.highPA) {
            if (UseTxPower2400) {
                ret = inst.txPower2400;
            }
            else {
                ret = freq433 ? inst.txPower433Hi : inst.txPowerHi;
            }
        }
        else if (UseTxPower2400) {
            ret = inst.txPower2400;
        }
        else if (freq169) {
            ret = inst.txPower169;
        }
        else {
            ret = freq433 ? inst.txPower433 : inst.txPower;
        }
        return ret;
    }

    /**
     *  ======== getTxPowerAll ========
     *  Get Tx Power values for all txPower configurables
     */
    function getTxPowerAll() {
        const ret = TxPowerCache;
        const freq = getFrequency();
        const freq433 = freq > Common.FreqHigher169 && freq < Common.FreqHigher433;
        const freq169 = freq < Common.FreqHigher433;

        if (IS_HPA_DEVICE && !UseTxPower2400) {
            // High PA
            let raw = RfDesign.getTxPowerValueByDbm(freq, true, TxPowerHi.dbm);
            if (raw !== null) {
                TxPowerHi.raw = raw;
                if (freq433) {
                    ret.t433Hi = TxPowerHi.dbm;
                }
                else {
                    ret.high = TxPowerHi.dbm;
                }
            }
            else {
                TxPowerHi.dbm = freq433 ? TxPowerCache.t433Hi : TxPowerCache.high;
                TxPowerHi.raw = "0xFFFF";
            }

            // Default PA
            let dbm;
            if (freq433) {
                dbm = getCmdFieldValueByOpt("txPower433", "txPower");
            }
            else if (freq169) {
                dbm = getCmdFieldValueByOpt("txPower169", "txPower");
            }
            else {
                dbm = getCmdFieldValueByOpt("txPower", "txPower");
            }

            raw = RfDesign.getTxPowerValueByDbm(freq, false, dbm);
            if (raw !== null) {
                if (freq433) {
                    ret.t433 = dbm;
                }
                else if (freq169) {
                    ret.t169 = dbm;
                }
                else {
                    ret.default = dbm;
                }
            }
        }
        // Not P-device, read directly from register
        else if (freq433) {
            ret.t433 = getCmdFieldValueByOpt("txPower433", "txPower");
        }
        else if (freq169) {
            ret.t169 = getCmdFieldValueByOpt("txPower169", "txPower");
        }
        else if (UseTxPower2400) {
            ret.t2400 = getCmdFieldValueByOpt("txPower2400", "txPower");
        }
        else {
            ret.default = getCmdFieldValueByOpt("txPower", "txPower");
        }
        return ret;
    }

    /**
     *  ======== is154g ========
     *  True if the setting supports IEEE 802.15.4g (proprietary advanced RX/TX commands)
     */
    function is154g() {
        return cmdHandler.cmdIsUsed("CMD_PROP_RX_ADV");
    }

    /**
     *  ======== updateRfCommandsBLE ========
     *  Calculates RF command values based on the content
     *  the passed instance.
     *
     *  @param inst - current instance
     */
    function updateRfCommandsBLE(inst) {
        const frequency = inst.frequency;

        // Update target settings
        updateTarget(inst);

        // Hard-code value for commands that are not included in the setting
        const val = inst.whitening ? 0x51 : 0;
        setCmdFieldValue("whitening", "whitening.init", val);
        setCmdFieldValue("whitening", "whitening.bOverride", 1);
        setCmdFieldValueDirect("condition.rule", 1, false);

        // Frequency
        setCmdFieldValueByOpt("frequency", "frequency", frequency);

        // Channel
        const channel = Common.bleFreqToChanRaw(frequency);
        setCmdFieldValue("channel", "channel", channel);

        // PDU length
        setCmdFieldValue("packetLengthBle", "advLen", inst.packetLengthBle - BLE_DEVICE_ADDRESS_LENGTH);
    }

    /**
     *  ======== updateRfCommands154 ========
     *  Calculates RF command values based on the content
     *  the passed instance.
     *
     *  @param inst - current instance
     */
    function updateRfCommands154(inst) {
        const frequency = inst.frequency;

        // Update target settings
        updateTarget(inst);

        // Frequency
        setCmdFieldValueByOpt("frequency", "frequency", frequency);
    }

    /**
     *  ======== updateRfCommandsProp ========
     *  Calculates RF command values based on the content
     *  the passed instance.
     *
     *  @param inst - current instance
     */
    function updateRfCommandsProp(inst) {
        const frequency = inst.carrierFrequency;

        // Update target settings
        updateTarget(inst);

        // Update TX power override
        const txPower = getTxPowerFromInst(inst);
        let highPA = false;
        if ("highPA" in inst) {
            highPA = inst.highPA;
        }
        OverrideHandler.updateTxPowerOverride(txPower, frequency, highPA);

        // Deviation
        RfParamCache.deviation = inst.deviation;
        const deviation = UtilHandle.updCfDeviation();
        setRfdCommandField("CMD_PROP_RADIO_DIV_SETUP.modulation.deviation", deviation);

        // Symbol rate
        RfParamCache.symbolRate = inst.symbolRate;
        const symRate = UtilHandle.updCfRateWord();
        setRfdCommandField("CMD_PROP_RADIO_DIV_SETUP.symbolRate.rateWord", symRate);

        // RX Bandwidth
        RfParamCache.rxFilterBw = inst.rxFilterBw;
        setCmdFieldValueByOpt("rxFilterBw", "rxBw", inst.rxFilterBw);

        // Whitening
        setCmdFieldValueByOpt("whitening", "formatConf.whitenMode", inst.whitening);

        // Frequency
        RfParamCache.frequency = frequency;
        const freqCmd = Math.floor(Math.abs(frequency));
        setCmdFieldValue("carrierFrequency", "centerFreq", freqCmd);
        setCmdFieldValue("carrierFrequency", "frequency", freqCmd);

        const fractFreq = UtilHandle.updCfFractFreq();
        setRfdCommandField("CMD_FS.fractFreq", fractFreq);

        const loDiv = UtilHandle.updCfLoDivider();
        setRfdCommandField("CMD_PROP_RADIO_DIV_SETUP.loDivider", loDiv);

        // Preamble count
        setCmdFieldValueByOpt("preambleCount", "preamConf.nPreamBytes", inst.preambleCount);

        // Preamble mode
        setCmdFieldValueByOpt("preambleMode", "preamConf.preamMode", inst.preambleMode);

        // Sync word length
        setCmdFieldValueByOpt("syncWordLength", "formatConf.nSwBits", inst.syncWordLength);

        // RX packet length
        setCmdFieldValue("packetLengthRx", "maxPktLen", inst.packetLengthRx);

        // Sync word
        setCmdFieldValue("syncWord", "syncWord", inst.syncWord);
        setCmdFieldValue("syncWord", "syncWord0", inst.syncWord);
        setCmdFieldValue("syncWord", "syncWord1", 0);

        // Address mode: "No address check" or "Address check"
        setCmdFieldValueByOpt("addressMode", "pktConf.bChkAddress", inst.addressMode);

        // Fixed packet length
        setCmdFieldValue("fixedPacketLength", "pktLen", inst.fixedPacketLength);

        // Tx Packet length
        setCmdFieldValueByOpt("packetLengthConfig", "pktConf.bVarLen", inst.packetLengthConfig);

        // RX address filter
        setCmdFieldValue("address0", "address0", inst.address0);
        setCmdFieldValue("address1", "address1", inst.address1);
    }

    /**
     *  ======== setManualPatch ========
     *  Set manual patch. This overrides the patch of the PHY.
     *
     *  @param cpe - CPE patch
     *  @param rfe - RFE patch
     *  @param mce - MCE patch
     *
     */
    function setManualPatch(cpe, rfe, mce) {
        ManualPatch.cpe = cpe;
        ManualPatch.rfe = rfe;
        ManualPatch.mce = mce;
    }

    /**
     *  ======== getDecimationMode ========
     *  Get the decimation mode bit-field
     *  (CMD_PROP_RADIO_DIV_SETUP)
     *
     *  @returns number as string
     */
    function getDecimationMode() {
        return parseInt(getCommandField("symbolRate.decimMode")).toString();
    }

    /**
     *  ======== getPreambleCount ========
     *  Set Preamble count as string:
     *  ("1 Bits", "4 Bits", "1 Byte", "2 Bytes" .. "30 Bytes", "32 Bytes")
     */
    function getPreambleCount() {
        return executePhyPropertyUpdate("preambleCount");
    }

    /**
     *  ======== getSyncWordLength ========
     *  Get the Sync Word length as a string ("8 Bits" ... "32 Bits")
     */
    function getSyncWordLength() {
        return getCmdFieldValueByOpt("syncWordLength", "formatConf.nSwBits");
    }

    /**
     *  ======== getPreambleMode ========
     *  Get the Preamble mode as a descriptive string
     */
    function getPreambleMode() {
        return executePhyPropertyUpdate("preambleMode");
    }

    /**
     *  ======== getFixedPacketLength ========
     *  Get fixed packet length for TX
     */
    function getFixedPacketLength() {
        // CMD_PROP_TX.pktLen
        return getCommandField("pktLen");
    }

    /**
     *  ======== getPktConf ========
     *  Return TX packet length configuration, "Variable" or "Fixed"
     */
    function getPktConf(val) {
        return getCmdFieldValueByOpt("packetLengthConfig", "pktConf.bVarLen");
    }

    /**
     *  ======== getRxBandwidth ========
     *  Get RX filter bandwidth as a float, unit kHz
     */
    function getRxFilterBw() {
        return getCmdFieldValueByOpt("rxFilterBw", "rxBw");
    }

    /**
     *  ======== getSyncWord ========
     *  Get RX sync word
     */
    function getSyncWord() {
        const cmdUsed = cmdHandler.cmdIsUsed;
        if (cmdUsed("CMD_PROP_RX")) {
            return getCommandField("CMD_PROP_RX.syncWord");
        }
        else if (cmdUsed("CMD_PROP_RX_ADV")) {
            return getCommandField("CMD_PROP_RX_ADV.syncWord0");
        }
        return getCommandField("syncWord");
    }

    /**
     *  ======== getAddress0 ========
     *  Get value of CMD_PROP_RX.address0. No conversion required.
     */
    function getAddress0() {
        return getCommandField("address0");
    }

    /**
     *  ======== getAddress1 ========
     *  Get value of CMD_PROP_RX.address1. No conversion required.
     */
    function getAddress1() {
        return getCommandField("address1");
    }

    /**
     *  ======== getAddressMode ========
     *  Get value of CMD_PROP_RX.pktConf.bChkAddress (0 or 1)
     *  and convert it to a string ("No address check" or "Address check")
     */
    function getAddressMode() {
        return getCmdFieldValueByOpt("addressMode", "pktConf.bChkAddress");
    }

    /**
     *  ======== getPacketLengthRx ========
     *  Get maximum RX packet length
     */
    function getPacketLengthRx() {
        return getCommandField("maxPktLen");
    }

    /**
     *  ======== getPatchInfo ========
     *  Extract patch information from the database
     *
     *  @param protocol - single, multi, coex
     *
     */
    function getPatchInfo(protocol) {
        const patches = Common.forceArray(Setting.rfd_patch);

        const ret = {
            mode: Setting.rfd_rf_mode.name,
            cpe: 0,
            rfe: 0,
            mce: 0
        };

        for (const patch of patches) {
            const role = patch.role;
            const name = patch.name;

            if (ManualPatch.cpe) {
                ret.cpe = ManualPatch.cpe;
            }
            else if (role === "cpe_patch") {
                // Use patches from the setting
                if (protocol === "multi") {
                    ret.cpe = "rf_patch_cpe_multi_protocol";
                }
                else if (protocol.includes("coex_ble")) {
                    ret.cpe = "rf_patch_cpe_multi_bt5_coex";
                    if (protocol.includes("pg10")) {
                        ret.cpe = "rf_patch_cpe_ieee_coex_pg10";
                    }
                }
                else if (protocol.includes("coex_ieee")) {
                    ret.cpe = "rf_patch_cpe_ieee_coex";
                    if (protocol.includes("pg10")) {
                        ret.cpe = "rf_patch_cpe_ieee_coex_pg10";
                    }
                }
                else {
                    ret.cpe = name;
                }
            }

            if (ManualPatch.mce) {
                ret.mce = ManualPatch.mce;
            }
            else if (role === "mce_patch") {
                ret.mce = name;
            }

            if (ManualPatch.rfe) {
                ret.rfe = ManualPatch.rfe;
            }
            else if (role === "rfe_patch") {
                ret.rfe = name;
            }
        }

        if (protocol === "coex_ble") {
            ret.rfe = "rf_patch_rfe_ble_coex";
        }

        return ret;
    }

    /**
     *  ======== generatePatchCode ========
     *  Extract patch code from the database
     *
     *  @param protocol - single, multi, coex
     */
    function generatePatchCode(protocol) {
        const patch = getPatchInfo(protocol);
        let code = "    .rfMode = " + patch.mode + ",\n";

        if (typeof (patch.cpe) === "string") {
            code += "    .cpePatchFxn = &" + patch.cpe + ",\n";
        }
        else {
            code += "    .cpePatchFxn = 0,\n";
        }

        if (typeof (patch.mce) === "string") {
            code += "    .mcePatchFxn = &" + patch.mce + ",\n";
        }
        else {
            code += "    .mcePatchFxn = 0,\n";
        }

        if (typeof (patch.rfe) === "string") {
            code += "    .rfePatchFxn = &" + patch.rfe;
        }
        else {
            code += "    .rfePatchFxn = 0";
        }

        return code;
    }

    /**
     *  ======== generateOverrideCode ========
     *  Generate code for the Command overrides
     *
     *  @param ovr - Override variable name
     *  @param custom - Info on custom overrides
     */
    function generateOverrideCode(ovr, custom) {
        const freq = getFrequency();
        let txPowerDef = getCommandField("txPower");

        // High PA
        if (txPowerDef === "0xFFFF") {
            txPowerDef = RfDesign.getTxPowerValueDefault(freq, false);
        }

        const data = {
            txPower: txPowerDef,
            txPowerHi: RfDesign.getTxPowerValueByDbm(freq, true, TxPowerHi.dbm),
            loDivider: getCommandField("loDivider"),
            freq: freq,
            frontend: getCommandField("config.frontEndMode")
        };
        return OverrideHandler.generateCode(ovr, data, custom);
    }

    /**
     *  ======== getCommandMap ========
     *  Get a list of commands that map to the given RF parameter.
     *
     *  @param rfParam - RF parameter name
     */
    function getCommandMap(rfParam) {
        const uc = rfParam.toUpperCase();
        if (uc in ParamMap) {
            return ParamMap[uc];
        }
        return null;
    }

    /**
    * ======== getOptByKey ========
    * Get an option name by key.
    *
    * @param paramName - RF parameter name
    * @param key - Key to search for in option list
    */
    function getOptByKey(paramName, key) {
        const opts = Common.getOptions(DeviceConfig.configs, paramName);

        // Look for an option with the given key
        let val = null;
        _.each(opts, (opt) => {
            // eslint-disable-next-line
            if (opt.key == key) {
                val = opt.name;
                return false;
            }
            // Continue _.each iteration
            return true;
        });

        if (val === null) {
            // Workaround for problem with default TX Power values in some settings.
            // The default is not in the PA table, so we use the first in the list.
            val = opts[0].name;
        }
        return val;
    }

    /**
    * ======== getOptByName ========
    * Get an option value by name
    *
    * @param paramName - RF parameter name
    * @param key - Key to search for in option list
    */
    function getOptByName(paramName, name) {
        const opts = Common.getOptions(DeviceConfig.configs, paramName);

        // Look for an option with the given key
        let val = null;
        _.each(opts, (opt) => {
            // eslint-disable-next-line
            if (opt.name == name) {
                val = opt.key;
                return false;
            }
            // Continue _.each iteration
            return true;
        });

        return val;
    }

    /**
     *  ======== isPhyPropSupported ========
     *  Check if an RF parameter is supported by the current selection of RF commands
     *
     *  @param rfParam - RF parameter name
     *  @param usedCmds - currently used RF commands
     */
    function isPhyPropSupported(rfParam, usedCmds) {
        // Always used of part of the design
        if (rfParam === "highPA") {
            return true;
        }
        // Special case: multiple txPower and configurables
        let key = rfParam;
        if (key.includes("txPower")) {
            key = "txPower";
        }
        // Get parameter- > command map
        const cmdMap = getCommandMap(key);
        if (cmdMap === null) {
            return false;
        }
        // Check if the command is selected by the user
        const commands = _.intersection(usedCmds, cmdMap);
        if (commands.length === 0) {
            return false;
        }
        return true;
    }

    /**
     *  ======== generateParameterSummary ========
     *  Generate a parameter summary as C++ comments
     *
     *  @param inst - the current instance
     */
    function generateParameterSummary(inst) {
        // Pre-process and sort the parameters alphabetically
        const keys = [];
        const displayNames = [];
        const ui = inst.$uiState;

        function pushKeys(cfg) {
            const name = cfg.name;
            keys.push(name);
            displayNames[name] = cfg.displayName;
        }

        _.each(inst.$module.config, (cfg) => {
            if (_.has(cfg, "config")) {
                _.each(cfg.config, (subCfg) => {
                    pushKeys(subCfg);
                });
            }
            else {
                pushKeys(cfg);
            }
        });
        keys.sort();

        // Generate summary
        let summary = "";

        const usedCmds = cmdHandler.getUsedCommands(inst);

        _.each(keys, (key) => {
            // Parameter summary skipped for hidden configurables
            if (ui[key].hidden || key.includes("phyType")) {
                return;
            }

            // Ignore RF parameters that are not supported by an RF command
            if (!isPhyPropSupported(key, usedCmds)) {
                return;
            }

            let value = inst[key];

            // Overwrite with calculates values where applicable
            if (key === "carrierFrequency") {
                value = getFrequency();
            }

            if (key === "symbolRate") {
                value = getSymbolRate();
            }

            if (key === "deviation") {
                value = getDeviation();
            }

            // Convert integers to hex where applicable
            if (key === "address0" || key === "address1" || key === "syncWord") {
                value = int2hex(value);
            }

            // Append generated line
            const displayName = displayNames[key];
            summary += "// " + displayName + ": " + value + "\n";
        });

        if (summary === "") {
            // eslint-disable-next-line max-len
            summary = "// NB! Setting RF parameters in this design has no effect because no RF commands are selected.\n";
        }
        else {
            summary = "// PARAMETER SUMMARY\n" + summary;
        }
        return summary;
    }

    /**
     *  ======== getRfdCommandField ========
     *  The function getCommandField for use by SmartRF Studio 8 shared Javascript.
     *  Maps SETUP functions to High PA version when applicable.
     *
     *  @param fieldName - field name (full path)
     */
    function getRfdCommandField(fieldName) {
        let name = fieldName;
        if (IS_HPA_DEVICE && !name.includes("_PA")) {
            name = name.replace("_SETUP", "_SETUP_PA");
        }
        const field = getCmdFieldByName(name);

        if (field != null) {
            return field.value;
        }
        throw Error("RF command field not found: " + fieldName);
    }

    /**
     *  ======== setRfdCommandField ========
     *  Set an RF Driver command field.
     *  Maps SETUP functions to High PA version when applicable.
     *
     *  @param fieldName - Field name (full path)
     *  @param value - Value to apply
     */
    function setRfdCommandField(fieldName, value) {
        let name = fieldName;
        if (IS_HPA_DEVICE && !name.includes("_PA")) {
            name = name.replace("_SETUP", "_SETUP_PA");
        }
        const field = getCmdFieldByName(name);

        if (field === null) {
            throw Error("RF command field not found: " + fieldName);
        }
        field.value = value;
    }

    /**
    *  ======== updateFrontendSettings ========
    *  Update the front-end settings with values fetched from the RF Design module.
    */
    function updateFrontendSettings() {
        // Get target settings (index to front-end setting)
        const id = RfDesign.getFrontEnd(FreqBand);

        const fe = DeviceInfo.getFrontendSettings(PhyGroup, id);
        if (fe === null) {
            throw Error("FrontEnd not found[" + id + "]");
        }

        let frontEndCmds;
        if ("frequency_range" in fe) {
            frontEndCmds = Common.forceArray(fe.frequency_range[0].rfd_command);
        }
        else {
            frontEndCmds = Common.forceArray(fe.rfd_command);
        }

        let hasFrontEndOverride = false;

        // Patch front-end setting into current setting
        const currentCmds = Setting.rfd_command;
        _.each(frontEndCmds, (feCmd) => {
            let patchedCmd = null;
            _.each(currentCmds, (cmd) => {
                if (feCmd.name === cmd.name) {
                    patchedCmd = cmd;
                    return false;
                }
                // Continue
                return true;
            });

            if (patchedCmd !== null) {
                const temp = _.merge(patchedCmd.rfd_command_field, feCmd.rfd_command_field);
                patchedCmd.rfd_command_field = _.uniqBy(temp, "name");

                // Apply to Command buffer
                _.each(feCmd.rfd_command_field, (field) => {
                    setCmdFieldValueDirect(field.name, field.value, true);
                });

                if ("OverrideField" in feCmd) {
                    patchedCmd.OverridePatch = feCmd.OverrideField;
                    hasFrontEndOverride = true;
                }
            }
        });

        if (hasFrontEndOverride) {
            // Update override table
            const highPA = cmdHandler.getCommandField("txPower") === "0xFFFF";
            OverrideHandler.init(Setting.rfd_command, highPA);
        }
    }

    /**
     *  ======== updateTarget ========
     *  Updated target information according to PHY
     *
     *  @param inst - current instance
     */
    function updateTarget(inst) {
        // TxPower
        updateTxPower(inst);

        // Create override table
        const highPA = getCommandField("txPower") === "0xFFFF";
        OverrideHandler.init(Setting.rfd_command, highPA);
    }

    /**
     *  ======== updateTxPower ========
     *  Update txPower according to the content of configurables
     *  holding the TX power values.
     *
     *  @param inst - current instance
     */
    function updateTxPower(inst) {
        const freq = getFrequency();
        const freq433 = freq > Common.FreqHigher169 && freq < Common.FreqHigher433;
        const freq169 = freq < Common.FreqHigher169;
        let raw;

        if (inst.highPA) {
            // TX power is handled by overrides
            setCmdFieldValue("txPower", "txPower", "0xFFFF");
            TxPowerHi.dbm = freq433 ? inst.txPower433Hi : inst.txPowerHi;
        }
        else if (freq433 && "txPower433" in inst) {
            raw = RfDesign.getTxPowerValueByDbm(freq, false, inst.txPower433);
            setCmdFieldValue("txPower", "txPower", raw);
        }
        else if (freq169 && "txPower169" in inst) {
            raw = RfDesign.getTxPowerValueByDbm(freq, false, inst.txPower169);
            setCmdFieldValue("txPower", "txPower", raw);
        }
        else if (UseTxPower2400) {
            raw = RfDesign.getTxPowerValueByDbm(freq, false, inst.txPower2400);
            setCmdFieldValue("txPower", "txPower", raw);

            if ("highPA" in inst) {
                // TX Power high not used; set to default
                const txOptions = RfDesign.getTxPowerOptions(freq, true);
                if (txOptions.length > 0) {
                    TxPowerHi.dbm = txOptions[0].name;
                }
            }
        }
        else {
            raw = RfDesign.getTxPowerValueByDbm(freq, false, inst.txPower);
            setCmdFieldValue("txPower", "txPower", raw);
            if ("highPA" in inst) {
                // TX Power high not used; set to default
                const txOptions = RfDesign.getTxPowerOptions(freq, true);
                if (txOptions.length > 0) {
                    TxPowerHi.dbm = txOptions[0].name;
                }
            }
        }
    }

    /**
     *  ======== updateRfCommands ========
     *  Update the contents of the RF Command database with data extracted
     *  from the RF parameters instance.
     *
     *  @param inst - current instance
     */
    function updateRfCommands(inst) {
        switch (PhyGroup) {
        case Common.PHY_PROP:
            updateRfCommandsProp(inst);
            break;
        case Common.PHY_BLE:
            updateRfCommandsBLE(inst);
            break;
        case Common.PHY_IEEE_15_4:
            updateRfCommands154(inst);
            break;
        default:
            throw Error("No such PHY group: " + PhyGroup);
        }
    }

    /**
     *  ======== getIncludeFilesRFD ========
     *  Get the include files from the RFD PHY definition
     */
    function getIncludeFiles() {
        const files = [{name: "ti/devices/DeviceFamily.h", type: "common"}];

        const fileInfo = Setting.rflib_header_file;
        for (const item of fileInfo) {
            files.push({name: item.name, type: "device_family"});
        }
        files.push({name: "ti/drivers/rf/RF.h", type: "common"});

        return files;
    }

    /**
     *  ======== generateRfCommandCode ========
     *  Get the include files from the RFD PHY definition
     */
    function generateRfCommandCode(symNames, legacy, qualifier) {
        const cmds = cmdHandler.getCommands();

        let code = "";
        for (let j = 0; j < cmds.length; j++) {
            const cmd = cmds[j];
            const cmdName = cmdHandler.getCommandName(cmd);
            const cmdKey = _.camelCase(cmdName);
            if (cmdKey in symNames) {
                const cmdDescr = cmdHandler.getCommandDescription(cmdName);
                const varName = symNames[cmdKey].replace("SetupPa", "Setup");
                const rfCmdCode = cmdHandler.generateRfCmdCodeRFD(cmd, symNames, legacy);

                if (rfCmdCode.rfPar.length > 0) {
                    const ps = rfCmdCode.parStructName;
                    const tn = rfCmdCode.parTypeName;
                    code += `// Structure for ${cmdName}.pParam\n`;
                    code
+= `rfc_${tn}_t ${ps} =
{
${rfCmdCode.rfPar.join(",\n")}
};
`;
                }

                code += `
// ${cmdName}
// ${cmdDescr}
${qualifier}rfc_${cmdName}_t ${varName} =
{
${rfCmdCode.rfCmd.join(",\n")}
};
`;
            }
        }
        return code;
    }

    // Return the module's exported functions
    return {
        getPhyGroup: () => PhyGroup,
        getPhyName: () => PhyID,
        getPhydefFile: () => PhyDefFile,
        getCommands: () => cmdHandler.getCommands(),
        getPhydefLongName: () => PhyInfo.description,
        getCommandName: (cmd) => cmdHandler.getCommandName(cmd),
        is154g: is154g,
        updateRfCommands: updateRfCommands,
        initConfigurables: initConfigurables,
        getCmdList: cmdHandler.getCmdList,
        getCommandDescription: cmdHandler.getCommandDescription,
        getRfData: getRfData,
        validatePhyProperty: validatePhyProperty,
        generateParameterSummary: generateParameterSummary,
        getCommandMap: cmdHandler.getCommandMap,
        setManualPatch: setManualPatch,
        getManualPatch: () => ManualPatch,
        getPatchInfo: getPatchInfo,
        generatePatchCode: generatePatchCode,
        generateRfCommandCode: generateRfCommandCode,
        generateOverrideCode: generateOverrideCode,
        updateFrontendSettings: updateFrontendSettings,
        getDecimationMode: getDecimationMode,
        getUsedCommands: cmdHandler.getUsedCommands,
        isPhyPropSupported: isPhyPropSupported,
        getIncludeFiles: getIncludeFiles
    };
}
