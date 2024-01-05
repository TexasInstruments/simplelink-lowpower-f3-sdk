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
 *  ======== prop.syscfg.js ========
 *  Radio configuration support for proprietary settings.
 */

"use strict";

/* Common utility functions */
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");

/* Other dependencies */
const RFBase = Common.getScript("radioconfig");
const DevInfo = Common.getScript("device_info.js");
const RfDesign = Common.getScript("rfdesign");

/* PHY handler */
const PhyHandler = Common.getPhyHandler();

/* Documentation */
const PropDocs = Common.getScript("phy_groups/prop_docs.js");
const SharedDocs = Common.getScript("phy_groups/shared_docs.js");

/* PHY group */
const PHY_GROUP = Common.PHY_PROP;

/* Short-hands from Common */
const HAS_SUB1G = Common.isSub1gDevice();
const HAS_24G = Common.HAS_24G_PROP;
const logError = Common.logError;
const logWarning = Common.logWarning;

/* High PA support */
const highPaSupport = DevInfo.hasHighPaSupport();

/* PHY group specific configurables */
const tmp = DevInfo.getConfiguration(PHY_GROUP);
const config = _.cloneDeep(tmp.configs);

const propSpecific = {
    displayName: "RF Settings Proprietary",
    description: "RF Settings Proprietary",
    longDescription: "RF Settings module for proprietary protocols",
    phyGroup: PHY_GROUP,
    validate: validate,
    config: config
};

// Special handling for PHY with RF commands
const VisibleWithRfCmd = [
    "txPower", "frequency", "syncWord", "packetLengthSize"
];

const RxFilterBwOptions7 = []; // For decimMode = 7
const RxFilterBwOptions0 = []; // For decimMode = 0

// Configurables to preserve during reload
const configPreserve = ["txPower", "txPowerHi", "txPower169", "txPower433", "txPower433Hi", "txPower2400"];

/*
 *  ======== freqBandOnChange ========
 *  Change handler for frequency band
 *
 */
function freqBandOnChange(inst, ui) {
    const prop2400 = inst.freqBand === "2400";

    const c868hidden = inst.freqBand !== "868";
    const c433hidden = inst.freqBand !== "433";
    const c169hidden = inst.freqBand !== "169";

    if (HAS_SUB1G) {
        ui.phyType868.hidden = c868hidden;
        ui.txPower.hidden = c868hidden;

        if ("phyType433" in ui) {
            ui.phyType433.hidden = c433hidden;
            ui.txPower433.hidden = c433hidden;
        }
        if ("phyType169" in ui) {
            ui.phyType169.hidden = c169hidden;
            ui.txPower169.hidden = c169hidden;
        }
    }

    if (HAS_24G) {
        ui.phyType2400.hidden = !prop2400;
        ui.txPower2400.hidden = !prop2400;
    }

    // Get the PHY type for the current frequency band
    const phyType = Common.getPhyType(inst);

    // Refresh the instance from PHY setting
    RFBase.reloadInstanceFromPhy(inst, ui, phyType, PHY_GROUP, configPreserve);
    updateVisibility(inst, ui);
}

/*
 *  ======== phyTypeOnChange ========
 *  Change handler for PHY type configurable
 *
 */
function phyTypeOnChange(inst, ui) {
    // Change to new PHY setting
    const phyType = Common.getPhyType(inst);

    // Refresh the instance from PHY setting
    RFBase.reloadInstanceFromPhy(inst, ui, phyType, PHY_GROUP, configPreserve);
    updateVisibility(inst, ui);

    // Special handling of OOK settings
    if (phyType.includes("ook")) {
        const fb = inst.freqBand;
        const opts = RfDesign.getTxPowerOptions(fb, false);
        if (fb === "433") {
            inst.txPower433 = opts[1].name;
        }
        else {
            inst.txPower = opts[1].name;
        }
    }

    // Update PHY property visibility
    const phyHandler = PhyHandler.get(PHY_GROUP, phyType);
    for (const cfg of VisibleWithRfCmd) {
        if (cfg in ui) {
            const visible = phyHandler.isPhyPropSupported(inst, cfg);
            ui[cfg].hidden = !visible;
        }
    }
}

/**
 *  ======== getRxFilterBwOptions ========
 *  Get the options list for RX Filter Bandwidth. The list depends
 *  om decimMode.
 *
 *  @param inst - inst from which update data are fetched
 */
function getRxFilterBwOptions(inst) {
    const phyName = Common.getPhyType(inst);
    const phyHandler = PhyHandler.get(PHY_GROUP, phyName);

    const decimMode = "getDecimationMode" in phyHandler ? phyHandler.getDecimationMode() : "0";

    if (decimMode === "0") {
        // Default
        return RxFilterBwOptions0;
    }
    else if (decimMode === "7") {
        // TCXO
        return RxFilterBwOptions7;
    }
    throw Error("Decimation mode not supported: " + decimMode);
}

/**
 *  ======== createRxFilterBwOptions ========
 *  Create RX filter Bandwidth options for decimation modes 0 and 7.
 *
 *  @param rawOptions - raw option list from SmartRF Studio.
 */
function createRxFilterBwOptions(rawOptions) {
    _.each(rawOptions, (item) => {
        if ("info" in item) {
            // For now only the only info is: "decimMode=7"
            RxFilterBwOptions7.push({name: item.name});
        }
        else {
            RxFilterBwOptions0.push({name: item.name});
        }
    });
}

/**
 *  ======== initConfigurables ========
 *  Set visibility of configurables. Insert onChange functions where needed.
 *
 *  @param configurables - configurables to act on
 */
function initConfigurables(configurables) {
    const device24Only = Common.is24gOnlyDevice();

    let cfgPacketLengthConfig = null;
    let cfgFixedPacketLength = null;

    function processConfigurable(item) {
        if ("onChange" in item) {
            // Avoid overriding existing onChange handlers if they already exist
            return;
        }
        switch (item.name) {
        case "permission":
            item.onChange = onPermissionChange;
            break;
        case "freqBand":
            item.onChange = freqBandOnChange;
            break;
        case "phyType868":
            item.onChange = phyTypeOnChange;
            break;
        case "phyType433":
            item.onChange = phyTypeOnChange;
            break;
        case "phyType169":
            item.onChange = phyTypeOnChange;
            break;
        case "phyType2400":
            item.onChange = phyTypeOnChange;
            break;
        case "addressMode":
            item.onChange = updateVisibility;
            break;
        case "packetLengthConfig":
        case "packetLengthSize":
            item.onChange = updateVisibility;
            cfgPacketLengthConfig = item;
            break;
        case "fixedPacketLength":
            cfgFixedPacketLength = item;
            break;
        case "txPower":
            if (device24Only && Common.isPlatformRFD()) {
                item.hidden = true;
            }
            break;
        case "txPower433":
        case "txPower169":
            if (device24Only) {
                item.hidden = true;
            }
            break;
        case "highPA":
            if (device24Only) {
                item.hidden = true;
                item.default = false;
            }
            item.onChange = RFBase.highPaOnChange;
            break;
        case "rxFilterBw":
            createRxFilterBwOptions(item.options);
            item.options = (inst) => getRxFilterBwOptions(inst);
            break;
        case "txPower2400":
            item.hidden = !device24Only;
            break;
        default:
            break;
        }
    }

    _.each(configurables, (item) => {
        if (_.has(item, "config")) {
            _.each(item.config, (subItem) => {
                processConfigurable(subItem);
            });
        }
        else {
            processConfigurable(item);
        }
    });

    // Default visibility
    if (cfgFixedPacketLength !== null) {
        cfgFixedPacketLength.hidden = cfgPacketLengthConfig.default !== "Fixed";
    }
}

/**
 *  ======== onPermissionChange ========
 *  1. Change permission according to permission configurable.
 *  2. Change visibility of address filter according to parent
 */
function onPermissionChange(inst, ui) {
    // Set access to selected RF parameters
    const readOnly = inst.permission === "ReadOnly";

    // Frequency band
    // - Custom stack: ReadOnly
    // - Standard stack: controlled by the 'permission' configurable
    const freqReadOnly = readOnly || inst.parent === "Custom";
    ui.freqBand.readOnly = freqReadOnly;

    const configs = [
        "address0", "address1", "deviation", "symbolRate",
        "preambleCount", "preambleMode", "syncWordLength", "syncWord",
        "packetLengthConfig", "fixedPacketLength", "packetLengthRx",
        "addressMode"];

    for (const item of configs) {
        if (item in ui) {
            ui[item].readOnly = readOnly;
        }
    }

    // RX address filter hidden if parent is a stack
    if (inst.parent === "Stack" && "addressMode" in inst) {
        ui.address0.hidden = true;
        ui.address1.hidden = true;
        ui.addressMode.hidden = true;
    }

    // PHY type
    // - Custom stack, static PHY: ReadOnly
    // - Custom stack, custom PHY: ReadWrite
    // - Standard stack: controlled by the 'permission' configurable
    let phyTypeReadOnly = false;
    if (inst.parent === "Custom") {
        const phyType = Common.getPhyType(inst);
        phyTypeReadOnly = !phyType.includes("custom");
    }
    else {
        phyTypeReadOnly = readOnly;
    }

    if (HAS_SUB1G) {
        ui.phyType868.readOnly = phyTypeReadOnly;
        if ("phyType433" in ui) {
            ui.phyType433.readOnly = phyTypeReadOnly;
        }
        if ("phyType169" in ui) {
            ui.phyType169.readOnly = phyTypeReadOnly;
        }
    }

    if (HAS_24G) {
        ui.phyType2400.readOnly = phyTypeReadOnly;
    }
}

/**
 *  ======== updateVisibility ========
 *  Hides configurables that is not relevant for the chosen phyType
 *
 * @param inst - RF Settings instance
 * @param ui - RF Setting UI instance
 */
function updateVisibility(inst, ui) {
    const phyName = Common.getPhyType(inst);
    const phyHandler = PhyHandler.get(PHY_GROUP, phyName);
    const is154g = phyHandler.is154g();

    ui.syncWord.readOnly = is154g;

    if (Common.isPlatformRFD()) {
        // Read Only if IEEE 802.15.4
        ui.packetLengthConfig.hidden = is154g;
        ui.syncWordLength.readOnly = is154g;
        ui.preambleMode.readOnly = is154g;

        // Hide if IEEE 802.15.4 or owned by a stack
        ui.addressMode.hidden = is154g || inst.parent === "Stack";
        ui.fixedPacketLength.hidden = inst.packetLengthConfig !== "Fixed"; // Tx packet length

        // Address filter
        const hidden = inst.addressMode === "No address check";
        ui.address0.hidden = hidden;
        ui.address1.hidden = hidden;

        return;
    }

    // RCL interface
    const isNesb = "phyType2400" in inst && inst.phyType2400 === "nesb";
    ui.whitening.hidden = isNesb;
    ui.packetLengthSize.hidden = isNesb;
    ui.rxFixedPayloadLength.hidden = inst.packetLengthSize !== "Fixed";

    if ("codedTxRate" in ui) {
        ui.codedTxRate.hidden = !phyHandler.hasFeatureGroup("coded_tx_rate");
    }
    if ("subPhy" in ui) {
        ui.subPhy.hidden = !phyHandler.hasFeatureGroup("sub_phy");
    }
}

/**
 *  ======== validateFrequency ========
 *  Check if a frequency is valid
 *
 *  @param inst - RF Settings instance to be validated
 */
function validateFrequency(inst) {
    const status = {
        valid: false,
        msg: ""
    };

    const MIN_FREQ = 0;
    const MAX_FREQ = 3000;
    const BAND_24G = inst.freqBand === "2400";
    const BAND_433 = inst.freqBand === "433";
    const BAND_169 = inst.freqBand === "169";

    const freqCfgName = "carrierFrequency" in inst ? "carrierFrequency" : "frequency";
    const freq = inst[freqCfgName];
    const loFreq = freq < Common.FreqHigher433;

    // Sanity check
    if (freq < MIN_FREQ || freq > MAX_FREQ) {
        status.msg = "Valid range: " + MIN_FREQ + " to " + MAX_FREQ + " MHz";
        return status;
    }

    // Check for correct frequency band
    if (BAND_24G) {
        if (freq < 2400) {
            status.msg = "Carrier frequency is not within the 2.4 GHz frequency Band";
            return status;
        }
    }
    else if (BAND_433) {
        if (freq > Common.FreqHigher433 || freq < Common.FreqLower433) {
            status.msg = "Carrier frequency is not within the 433 MHz frequency Band";
            return status;
        }
    }
    else if (BAND_169) {
        if (freq < Common.FreqLower169 || freq > Common.FreqHigher169) {
            status.msg = "Carrier frequency is not within the 169 MHz frequency Band ["
                + Common.FreqLower169 + " .. " + Common.FreqHigher169 + "]";
            return status;
        }
    }
    else if (loFreq || freq > 2399) {
        status.msg = "Carrier frequency is not within the 868 MHz frequency Band";
        return status;
    }

    const prop2400 = BAND_24G;
    const highPA = highPaSupport && !prop2400 ? inst.highPA : false;
    const freqBand = RfDesign.getFrequencyBandByFreq(freq, highPA);

    // Check if PA table is supported
    if (freqBand === null) {
        // No PA-table, not a valid range
        const freqRanges = RfDesign.getFreqBands();

        const midFreq = parseInt(inst.freqBand);
        let msg = "Frequency out of range. Valid range:";
        _.each(freqRanges, (range) => {
            if (midFreq >= range.min && midFreq <= range.max) {
                msg += " [" + range.min + ".." + range.max + "]";
            }
        });
        status.msg = msg;
        return status;
    }
    // Validation passed
    status.valid = true;

    return status;
}

/**
 *  ======== validateSymbolRate ========
 *  Check if the symbol rate is valid
 *
 *  @param inst - RF Settings instance to be validated
 */
function validateSymbolRateRxBw(inst) {
    const status = {
        valid: false,
        msg: "",
        config: ""
    };

    // Workaround for validation issues (Wi-SUN #5, and sub-1GHz ZigBee 500 kbps)
    let phyNeedsWorkaround = false;
    if ("phyType868" in inst) {
        const phyType = inst.phyType868;
        phyNeedsWorkaround = phyType.includes("2gfsk300kbps75dev915wsun5")
            || phyType.includes("2gfsk500kbps154g");
    }

    if (phyNeedsWorkaround) {
        status.valid = true;
        return status;
    }

    // Need PHY handler for validation of RX filter BW and Symbol Rate
    const phyName = Common.getPhyType(inst);
    const phyHandler = PhyHandler.get(PHY_GROUP, phyName);

    // Validate symbol rate
    let result = phyHandler.validatePhyProperty("symbolRate");
    if (result !== null) {
        return result;
    }
    // Validate RX filter bandwidth
    result = phyHandler.validatePhyProperty("rxFilterBw");
    if (result !== null) {
        return result;
    }
    // Validation passed
    status.valid = true;

    return status;
}

/**
 *  ======== validateDeviation ========
 *  Check if deviation is valid
 *
 *  @param inst - RF Settings instance to be validated
 */
function validateDeviation(inst) {
    const DEV_MIN = 0;
    const DEV_MAX = 1000;

    const status = {
        valid: false,
        msg: ""
    };

    if (inst.deviation < DEV_MIN || inst.deviation > DEV_MAX) {
        status.msg = "Valid range: " + DEV_MIN + " to " + DEV_MAX + " kHz";
        return status;
    }
    // Validation passed
    status.valid = true;

    return status;
}

/**
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  @param inst       - RF Settings instance to be validated
 *  @param validation - Issue reporting object
 */
function validate(inst, validation) {
    const BAND_24G = inst.freqBand === "2400";

    // Validation common to all PHY groups
    Common.validateBasic(inst, validation);

    // Validate PHY properties
    const phyType = Common.getPhyType(inst);
    RFBase.validatePhyProperty(inst, validation, phyType, PHY_GROUP);

    // Validate front-end
    RFBase.validateFrontendSettings(inst, validation, PHY_GROUP);

    // Validate frequency ranges
    let status = validateFrequency(inst);
    if (!status.valid) {
        const freqCfgName = "carrierFrequency" in inst ? "carrierFrequency" : "frequency";
        logError(validation, inst, freqCfgName, status.msg);
        return;
    }

    const freq = inst.carrierFrequency;
    const freq169 = freq < Common.FreqHigher169;
    const freq433 = freq < Common.FreqHigher433 && !freq169;

    const prop24 = BAND_24G;
    const highPA = highPaSupport && !prop24 ? inst.highPA : false;
    const paSetting = RfDesign.getPaTable(freq, highPA);

    if (paSetting !== null) {
        // Valid range, check if characterized
        let isOutsideRange = true;
        let cRanges;

        if (freq169) {
            cRanges = [
                {
                    Min: 169,
                    Max: 170
                }
            ];
        }
        else if (freq433) {
            cRanges = [
                {
                    Min: 420,
                    Max: 527
                }
            ];
        }
        else if (inst.freqBand === "868") {
            cRanges = [
                {
                    Min: 863,
                    Max: 876
                },
                {
                    Min: 902,
                    Max: 930
                }
            ];
        }
        else {
            cRanges = [
                {
                    Min: 2402,
                    Max: 2480
                }
            ];
        }

        let rangeMsg = "";
        _.each(cRanges, (range) => {
            rangeMsg += " [" + range.Min + ".." + range.Max + "]";
            const tempResult = (freq < range.Min || freq > range.Max);
            isOutsideRange = isOutsideRange && tempResult;
        });

        if (isOutsideRange) {
            let msg = "The selected frequency belongs to a range that has not been characterized"
            + ", settings can give non-optimal performance";
            msg += " Characterized ranges:" + rangeMsg;
            logWarning(validation, inst, "carrierFrequency", msg);
        }
    }

    // RF command fields must be updated before validating Symbol Rate and RX Filter bandwidth
    const phyGroup = Common.getPhyGroup(inst);
    PhyHandler.getUpdatedRfCommands(inst, phyGroup);

    if ("phyType2400" in inst && inst.phyType2400 === "nesb") {
        return;
    }

    // Validate symbol rate
    if ("symbolRate" in inst) {
        status = validateSymbolRateRxBw(inst);
        if (!status.valid) {
            logError(validation, inst, status.config, status.msg);
            return;
        }
    }

    // Validate deviation
    if ("deviation" in inst) {
        status = validateDeviation(inst);
        if (!status.valid) {
            logError(validation, inst, "deviation", status.msg);
            return;
        }
    }

    if ("address0" in inst && "address1" in inst) {
        // Validate Address0
        if (inst.address0 > 0xFF || inst.address0 < 0) {
            logError(validation, inst, "address0", "RX Address0 must be a number between 0x00 and 0xFF");
            return;
        }

        // Validate Address1
        if (inst.address1 > 0xFF || inst.address1 < 0) {
            logError(validation, inst, "address1", "RX Address1 must be a number between 0x00 and 0xFF");
            return;
        }
    }

    // Validate SyncWord
    if ("syncWord" in inst) {
        if (inst.syncWord > 0xFFFFFFFF || inst.syncWord < 0) {
            logError(validation, inst, "syncWord", "Sync Word must be a number between 0x0 and 0xFFFFFFFF");
            return;
        }

        if ("syncWordLength" in inst) {
            const syncWordBits = _.toInteger(inst.syncWordLength.replace(" Bits", ""));
            const maxLength = (2 ** syncWordBits) - 1;
            let maxLengthHex = _.toUpper(maxLength.toString(16));
            if (maxLengthHex.length !== 8) {
                maxLengthHex = "0".repeat(8 - maxLengthHex.length) + maxLengthHex;
            }
            if (inst.syncWord > maxLength || inst.syncWord < 0) {
                logWarning(validation, inst, "syncWord", "Sync Word will ignore bits above "
                    + inst.syncWordLength + ", Max 0x" + maxLengthHex);
            }
        }
    }

    if ("packetLengthRx" in inst) {
        // Validate packetLengthRx
        const pktLen16 = inst.codeExportConfig.cmdList_prop.includes("cmdPropRxAdv");
        if (pktLen16) {
            if (inst.packetLengthRx > 2047 || inst.packetLengthRx < 0) {
                logError(validation, inst, "packetLengthRx", "Max Packet Length must be between 0 and 2047");
                return;
            }
        }
        else if (inst.packetLengthRx > 255 || inst.packetLengthRx < 0) {
            logError(validation, inst, "packetLengthRx", "Max Packet Length must be between 0 and 255");
            return;
        }

        if ("fixedPacketLength" in inst) {
            // Validate fixedPacketLength
            if (pktLen16) {
                if (inst.fixedPacketLength > 2047 || inst.fixedPacketLength < 0) {
                    logError(validation, inst, "fixedPacketLength", "Fixed Packet Length must be between 0 and 2047");
                    return;
                }
            }
            else if (inst.fixedPacketLength > 255 || inst.fixedPacketLength < 0) {
                logError(validation, inst, "fixedPacketLength", "Fixed Packet Length must be between 0 and 255");
                return;
            }
        }
    }

    // Validate TX power version CCFG vddr (not applicable to RCL)
    const ccfgPath = "/ti/devices/CCFG";
    if (ccfgPath in system.modules) {
        const ccfg = system.modules[ccfgPath];
        const txPower = freq433 ? inst.txPower433 : inst.txPower;
        const cfg = freq433 ? "txPower433" : "txPower";
        const forceVddrHref = system.getReference(ccfg.$static, "forceVddr");

        // Force VDDR off
        if (validateTxPower(txPower, freq, highPA) && ccfg.$static.forceVddr === false) {
            logWarning(validation,
                inst,
                cfg,
                `The selected TX Power requires Force VDDR in ${forceVddrHref} to be enabled.`);
        }
        // Force VDDR on
        else if (!validateTxPower(txPower, freq, highPA) && ccfg.$static.forceVddr === true) {
            logWarning(validation, inst, cfg, `The selected TX Power requires Force VDDR in ${forceVddrHref}`
                + " to be disabled, otherwise the output power may be wrong and the current draw too high.");
        }
    }
}

/**
 *  ======== validateTxPower ========
 * Function to verify if the value of CCFG_FORCE_VDDR_HH in the ccfg.c
 * file needs to be high or low
 *
 * @param txPower - The current selected TX Power
 * @param freq - The selected carrier frequency [MHz]
 * @param highPA - True if using high PA
 */
function validateTxPower(txPower, freq, highPA) {
    const paTable = RfDesign.getPaTable(freq, highPA);
    let vddr = false;
    _.forEach(paTable, (values) => {
        if (values._text === txPower) {
            if (values.boost_vddr) {
                vddr = true;
                return false;
            }
        }
        return true;
    });
    return vddr;
}

/**
 *  ======== extend ========
 *  Extends Proprietary object to include RadioConfig Basic module
 *
 */
function extend(base) {
    /* Make sure our copy of configurables is updated */
    let phyHandler;

    /* First configurable is "freqBands", follow by one phyType for each band */
    const nFreqBands = config[0].options.length;

    /* Initialize the default PHY in each frequency band */
    for (let i = nFreqBands; i > 0; i--) {
        const cfg = config[i].name;
        if (cfg.includes("phyType")) {
            if ((HAS_SUB1G && cfg !== "phyType2400") || (HAS_24G && cfg === "phyType2400")) {
                phyHandler = PhyHandler.get(PHY_GROUP, config[i].default);
                phyHandler.initConfigurables(propSpecific.config);
            }
        }
    }

    /* Initialize state of UI elements (readOnly/hidden when appropriate) */
    Common.initLongDescription(propSpecific.config, PropDocs.propDocs);
    Common.initLongDescription(propSpecific.config, SharedDocs.sharedDocs);

    /* Initialize configurables */
    initConfigurables(propSpecific.config);

    /* Remove invalid elements from configurables */
    RFBase.pruneConfig(propSpecific.config);

    return {...base, ...propSpecific};
}

exports = extend(RFBase);
