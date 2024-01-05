/* eslint-disable max-len */
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
 *  ======== rfdesign.syscfg.js ========
 */

"use strict";

// Common utility functions
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");
const ConfigPath = Common.isPlatformRFD() ? Common.ConfigPath : Common.ConfigPathRclCommon;

// Other dependencies
const DeviceInfo = Common.getScript("device_info.js");
const Docs = Common.getScript("radioconfig_docs.js");

// Configurable prefix
const Prefix = "fb";

// Load board info
const bd = loadBoardFile();
const Config = bd.config;
const BoardData = bd.boards;

let TiBoard = Common.getBoardName();
let HasTiBoard = TiBoard !== "";
let CurrentDesign = null;
let BoardName;

// Currently active frequency bands
let ActiveFreqBands = {};

// If a TI board is selected globally, select it
if (HasTiBoard) {
    if (TiBoard in BoardData) {
        CurrentDesign = BoardData[TiBoard];
    }
    else {
        throw Error("No such RF design: " + TiBoard);
    }
    BoardName = TiBoard;
}
else {
    // Custom design, pick the first design from the list
    TiBoard = null;
    BoardName = Config[0].default;
    CurrentDesign = BoardData[BoardName];
}

/**
 *  ======== loadBoardFile ========
 *  Load the board file for the device. Includes configurables.
 *
 */
function loadBoardFile() {
    const device = DeviceInfo.getDeviceName();
    const file = `${ConfigPath}boards_${device}.json`;

    return system.getScript(file);
}

/**
 *  ======== initConfigurables ========
 *  Initialize configurables that are not completed in pre-processing script.
 *
 */
function initConfigurables() {
    for (const item of Config) {
        switch (item.name) {
        case "rfDesign":
            item.onChange = onRfDesignChange;
            break;
        case "pa20":
            item.onChange = onPaChange;
            break;
        case "fbSub1g":
            item.onChange = onSub1gFreqBandChanged;
            break;
        case "fb24g":
            item.onChange = on24gFreqBandChanged;
            break;
        case "feSub1g":
            item.onChange = onSub1gFrontendChanged;
            break;
        case "fe24g":
            item.onChange = on24gFrontendChanged;
            break;
        default:
            break;
        }
    }
}
/*
 *  ======== onRfDesignChange ========
 *  Invoked when the rfDesign configurable is changed
 *
 *  @param inst - instance
 *  @param ui - UI info
 *
 */
function onRfDesignChange(inst, ui) {
    const name = inst.rfDesign;
    if (!(name in BoardData)) {
        throw new Error("RF Design does not exists: " + name);
    }
    CurrentDesign = BoardData[name];

    // Update frequency band and frontend selection
    if ("fbSub1g" in inst) {
        inst.fbSub1g = CurrentDesign.fbSub1g;
        inst.feSub1g = CurrentDesign.feSub1g;
        ui.feSub1g.hidden = inst.fbSub1g === "none";
    }

    if ("fb24g" in inst) {
        inst.fb24g = CurrentDesign.fb24g;
        inst.fe24g = CurrentDesign.fe24g;
        ui.fe24g.hidden = inst.fb24g === "none";
    }

    // Special handling of HPA devices
    if ("pa20" in inst) {
        inst.pa20 = CurrentDesign.hpa_band;
    }

    updateFreqBands(inst);
}

/*
 *  ======== onPaChange ========
 *  Invoked when the HPA configurable is changed
 *
 *  @param inst - instance
 *
 */
function onPaChange(inst) {
    CurrentDesign.hpa_band = inst.pa20;
}

/*
 *  ======== onSub1gFreqBandChanged ========
 *  Invoked when the sub-1G frequency band is changed
 *
 *  @param inst - instance
 *  @param ui - UI instance
 *
 */
function onSub1gFreqBandChanged(inst, ui) {
    ui.feSub1g.hidden = inst.fbSub1g === "none";
    CurrentDesign.fbSub1g = inst.fbSub1g;
    updateFreqBands(inst);
}

/*
 *  ======== on24gFreqBandChanged ========
 *  Invoked when a frequency band is changed
 *
 *  @param inst - instance
 *  @param ui - UI instance
 *
 */
function on24gFreqBandChanged(inst, ui) {
    ui.fe24g.hidden = inst.fb24g === "none";
    CurrentDesign.fb24g = inst.fb24g;
}

/*
 *  ======== onSub1gFrontendChanged ========
 *  Invoked when the sub-1G frequency band is changed
 *
 *  @param inst - instance
 *
 */
function onSub1gFrontendChanged(inst) {
    if (!HasTiBoard) {
        CurrentDesign.feSub1g = inst.feSub1g;
    }
}

/*
 *  ======== on24gFrontendChanged ========
 *  Invoked when a frequency band is changed
 *
 *  @param inst - instance
 *
 */
function on24gFrontendChanged(inst) {
    if (!HasTiBoard) {
        CurrentDesign.fe24g = inst.fe24g;
    }
}

/**
 *  ======== getHighPaAssociation ========
 *  Get the current high PA frequency band association.
 *
 *  @param inst - instance
 */
function getHighPaAssociation(inst) {
    if ("pa20" in inst) {
        return inst.pa20;
    }
    return "none";
}

/**
 *  ======== isFreqBandSelected ========
 *  Return true if the frequency band is included in the design.
 *
 *  @param inst - instance
 *  @param target - current target
 */
function isFreqBandSelected(inst, target) {
    if (target === null) {
        return false;
    }

    const freq = target.max;
    const cfg = Prefix + getFreqBandShortName(freq);

    if (freq > 2400) {
        let use = inst.fb24g === cfg;
        if (use && DeviceInfo.hasHighPaSupport()) {
            const info = getPaTableInfo(target, true);
            if (info) {
                const hpaMax = CurrentDesign.hpa_max;
                if (info.pa === "20" && hpaMax === "10") {
                    use = false;
                }
                if (info.pa === "10" && hpaMax === "20") {
                    use = false;
                }
            }
        }
        return use;
    }
    return inst.fbSub1g === cfg;
}

/**
 *  ======== getFreqBandShortName ========
 *  Get the short-hand name for the band og a given frequency.
 *
 *  @param freq - frequency in MHz
 */
function getFreqBandShortName(freq) {
    if (freq >= 168 && freq <= 170) {
        return "169";
    }

    if (freq >= 359 && freq <= 530) {
        return "433";
    }

    if (freq >= 770 && freq <= 930) {
        return "868";
    }

    if (freq >= 2300) {
        return "2400";
    }
    throw Error("No frequency band for: " + freq);
}

/**
 *  ======== getPaTableExportMethods ========
 *  Check what PA table export methods are used in the design
 *
 */
function getPaTableExportMethods() {
    const methods = {
        total: 0,
        separate: 0,
        combined: 0
    };
    const modules = system.modules;

    _.each(modules, (mod) => {
        if (mod.$name.includes("radioconfig/phy_groups")) {
            // Iterate module instances
            for (const inst of mod.$instances) {
                const paExport = inst.codeExportConfig.paExport;
                if (paExport === "combined") {
                    methods.combined += 1;
                }
                else if (paExport !== "none") {
                    methods.separate += 1;
                }
            }
        }
    });
    methods.total = methods.separate + methods.combined;

    return methods;
}

/**
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  @param rfinst - RF Design instance to be validated
 *  @param validation - Issue reporting object
 */
function validate(rfinst, validation) {
    // Validate RF design versus system board selection
    if (HasTiBoard) {
        if (TiBoard !== rfinst.rfDesign) {
            Common.logError(validation, rfinst, "rfDesign", "RF Design must align with board selection: " + TiBoard);
            return;
        }

        const lockMsg = "Changing front-end is only possible on custom boards";
        if (rfinst.feSub1g !== CurrentDesign.feSub1g) {
            Common.logError(validation, rfinst, "feSub1g", lockMsg);
            return;
        }

        if (rfinst.fe24g !== CurrentDesign.fe24g) {
            Common.logError(validation, rfinst, "fe24g", lockMsg);
            return;
        }
    }

    // Validate High PA selection
    const useHpa = getHighPaAssociation(rfinst) !== "none";

    if (useHpa) {
        if (rfinst.fb24g === "none" && rfinst.pa20 === "fb24g") {
            Common.logWarning(validation, rfinst, "pa20", "The High Power Output selection requires the 2.4 GHz band to be included.");
            return;
        }
        if (rfinst.fbSub1g === "none" && rfinst.pa20 === "fbSub1g") {
            Common.logWarning(validation, rfinst, "fbSub1g", "The High Power Output selection requires a Sub-1 GHz band to be included.");
            return;
        }
    }

    // Check consistency between used of Power Amplifiers and frequency bands
    const modules = system.modules;

    // Check if settings selection align with frequency band selection
    _.each(modules, (mod) => {
        if (mod.$name.includes("radioconfig/phy_groups")) {
            const instances = mod.$instances;
            // Iterate module instances
            for (const inst of instances) {
                let freqBand;
                if ("freqBand" in inst) {
                    // Proprietary
                    freqBand = parseInt(inst.freqBand);
                }
                else {
                    // BLE or IEEE 802.15.4
                    freqBand = 2400;
                }

                // Validate selected settings versus selected frequency bands
                switch (freqBand) {
                case 169:
                    if (rfinst.fbSub1g !== "fb169") {
                        const href = system.getReference(rfinst, "fbSub1g");
                        Common.logError(validation, inst, "freqBand", `The 169 MHz band is not supported in this ${href}`);
                        return;
                    }
                    break;
                case 433:
                    if (rfinst.fbSub1g !== "fb433") {
                        const href = system.getReference(rfinst, "fbSub1g");
                        Common.logError(validation, inst, "freqBand", `The 433 MHz band is not supported in this ${href}`);
                        return;
                    }
                    break;
                case 868:
                    if (rfinst.fbSub1g !== "fb868") {
                        const href = system.getReference(rfinst, "fbSub1g");
                        Common.logError(validation, inst, "freqBand", `The 868 MHz band is not supported in this ${href}`);
                        return;
                    }
                    break;
                case 2400:
                    if (rfinst.fb24g === "none") {
                        const href = system.getReference(rfinst, "fb24g");
                        if ("freqBand" in inst) {
                            // Proprietary
                            Common.logError(validation, inst, "freqBand", `The 2.4 GHz band is not supported in this ${href}`);
                        }
                        else if ("frequency" in inst) {
                            // BLE and IEEE 802.15.4
                            Common.logError(validation, inst, "frequency", `The 2.4 GHz band is not supported in this ${href}`);
                        }
                        return;
                    }
                    break;
                default:
                    throw Error("Frequency band not supported: " + freqBand);
                }

                // Validate High PA RF design versus combined PA table
                const href = system.getReference(rfinst, "pa20");
                if (useHpa) {
                    if (inst.codeExportConfig.paExport === "combined") {
                        if (rfinst.pa20 === "none") {
                            const msg = `Combined PA tables without High PA in the ${href} is not possible`;
                            Common.logError(validation, inst.codeExportConfig, "paExport", msg);
                            return;
                        }
                    }
                    // Check if High PA for the frequency band is allowed
                    if (rfinst.pa20 !== "fb24g" && freqBand === 2400 && inst.highPA) {
                        Common.logError(validation, inst, "highPA", "High PA for the " + freqBand + ` frequency band is not supported in this ${href}`);
                    }
                    if (rfinst.pa20 !== "fbSub1g" && freqBand < 1000 && inst.highPA) {
                        Common.logError(validation, inst, "highPA", "High PA for the " + freqBand + ` frequency band is not supported in this ${href}`);
                    }
                }
                else if (inst.highPA) {
                    Common.logError(validation, inst, "highPA", `High PA is not supported in this ${href}`);
                }
            }
        }
    });
}

/**
 *  ======== getTxPowerOptions ========
 *  Get list of available Tx power values for a given frequency.
 *
 *  @param freq - selected frequency (kHz)
 *  @param highPA - set if using high PA table
 */
function getTxPowerOptions(freq, highPA = false) {
    const ret = [];

    const paList = getPaTable(freq, highPA);
    _.each(paList, (pv) => {
        const item = {};
        item.name = pv._text;
        ret.push(item);
    });

    return ret;
}

/**
 *  ======== getPaTable ========
 *  Get a PA table by frequency and PA state (high, default)
 *
 *  @param freq - selected frequency (kHz)
 *  @param highPA - set if using high PA table
 */
function getPaTable(freq, highPA) {
    const fb = getFrequencyBandByFreq(freq, highPA);
    if (fb === null) {
        return null; // CurrentDesign.paDefault;
    }

    if (highPA) {
        if (fb.paTable10.length > 0) {
            return fb.paTable10;
        }
        if (fb.paTableHi.length > 0) {
            return fb.paTableHi;
        }
        return null;
    }

    return fb.paTable;
}

/**
 *  ======== getFrontEnd ========
 *
 *  Get the current front-end selection by frequency band
 *
 */
function getFrontEnd(freqBand) {
    if (freqBand === 2400) {
        return CurrentDesign.fe24g;
    }
    return CurrentDesign.feSub1g;
}

/**
 *  ======== updateFreqBands ========
 *
 *  Collect the frequency bands of the currently selected target
 *
 */
function updateFreqBands(inst = null) {
    let fbsub1g = "fb868";
    let fb24g = "fb2400";

    if (inst != null) {
        fbsub1g = inst.fbSub1g;
        fb24g = inst.fb24g;
    }

    ActiveFreqBands = {};
    const freqBands = ["fb169", "fb433", "fb868", "fb2400"];

    for (const fb of freqBands) {
        let tmp = "none";
        if (fb === fbsub1g) {
            tmp = fbsub1g;
        }
        else if (fb === fb24g) {
            tmp = fb24g;
        }
        if (tmp !== "none") {
            const freq = parseInt(fb.replace("fb", ""));
            const fbInfo = getFrequencyBandByFreq(freq, false);
            ActiveFreqBands[tmp] = fbInfo;
        }
    }
}

/**
 *  ======== getActiveFreqBands ========
 *
 *  Return the current frequency bands.
 *
 */
function getActiveFreqBands() {
    return ActiveFreqBands;
}

/**
 *  ======== getFrequencyBandByFreq ========
 *
 *  Return the frequency band info by frequency.
 *
 *  @param frequency - frequency to search for
 *  @param highPA - default or high PA
 *
 */
function getFrequencyBandByFreq(frequency, highPA) {
    // First try current design
    let band = getFrequencyBandInDesign(CurrentDesign, frequency, highPA);

    if (band !== null) {
        if (highPA && band.hiPa) {
            return band;
        }
        if (!highPA) {
            return band;
        }
    }

    // Try other designs
    for (const boardName in BoardData) {
        const design = BoardData[boardName];

        band = getFrequencyBandInDesign(design, frequency, highPA);
        if (band !== null) {
            if (highPA && band.hiPa) {
                return band;
            }
            if (!highPA) {
                return band;
            }
        }
    }
    // Error: no design found
    return null;
}

/**
 *  ======== getFrequencyBandInDesign ========
 *
 *  Return the frequency band info by frequency within one RF design.
 *
 *  @param design - The RF design searched
 *  @param frequency - Frequency to search for
 *  @param highPA - Default or high PA
 *
 */
function getFrequencyBandInDesign(design, frequency, highPA) {
    // Frequency band with split PA tables
    const partialFb = {
        paTable: [],
        paTableHi: [],
        paTable10: [],
        hiPa: false
    };
    const maxHpa = design.max_hpa;

    // Search in available frequency bands
    const freqBands = design.freq_range;
    for (const fb of freqBands) {
        const min = fb.min_freq;
        const max = fb.max_freq;

        if (frequency >= min && frequency <= max) {
            // Frequency band found, extract pertinent PA table entries
            for (const item of fb.pa_table_entry) {
                // Item that is compatible with legacy format
                const newItem = {
                    _text: item.tx_power,
                    boost_vddr: item.option_list === "boost_vddr"
                };

                if (item.rf_port === "tx_high_pa") {
                    newItem.Value = "0xFFFF";
                    newItem.TxHighPa = item.value;
                    if (maxHpa === "20") {
                        partialFb.paTableHi.push(newItem);
                    }
                    else {
                        partialFb.paTable10.push(newItem);
                    }
                    partialFb.hiPa = true;
                }
                else {
                    newItem.Value = item.value;
                    partialFb.paTable.push(newItem);
                }
            }
            partialFb.min = min;
            partialFb.max = max;

            return partialFb;
        }
    }
    return null;
}

/**
 *  ======== generatePaTableCode ========
 *  Generate PA-table code
 *
 *  @param paList - list of PA settings
 *  @param combined - true if default and high output PA settings are combined
 */
function generatePaTableCode(paList, combined) {
    let code = "";
    let val = -100;

    // Generate code
    _.eachRight(paList, (pv) => {
        const paEntry = generatePaEntryString(pv);
        if (pv.boost_vddr && !combined) {
            code += "    // This setting requires CCFG_FORCE_VDDR_HH = 1.\n";
        }

        const dbm = parseFloat(pv._text);
        if (!Number.isInteger(dbm)) {
            code += "    // The original PA value (" + pv._text + " dBm) has been rounded to an integer value.\n";
        }
        let iDbm = combined ? Math.floor(dbm) : Math.round(dbm);
        if (iDbm === val) {
            iDbm += 1;
        }

        // Raw value as comment
        let value = parseInt(pv.Value, 16);
        const highPA = value === 0xFFFF;
        let rawValue;
        if (highPA) {
            value = parseInt(pv.TxHighPa, 16);
            rawValue = Common.int2hex(value, 6);
        }
        else {
            rawValue = Common.int2hex(value, 4);
        }

        // Generate PA entry
        const str = "    {" + iDbm + ", " + paEntry + " }, // " + rawValue + "\n";

        code += str;
        val = iDbm;
    });
    return code;
}

/**
 *  ======== generatePaEntryString ========
 *  Generate the PA entry code string
 *
 *  @param pv - PA table entry
 */
/* eslint-disable no-bitwise */
function generatePaEntryString(pv) {
    if (Common.isPlatformRFD()) {
        const value = parseInt(pv.Value, 16);
        const highPA = value === 0xFFFF;
        const val = highPA ? pv.TxHighPa : value;
        const bias = val & 0x3f; /* bit 0..5 */
        const gain = (val >> 6) & 0x03; /* bit 6..7 */
        const boost = (val >> 8) & 0x01; /* bit 8 */
        const coefficient = (val >> 9) & 0x7f; /* bit 9..15 */
        let paEntry;

        if (highPA) {
            const ldoTrim = (val >> 16) & 0x3f; /* bit 16..21 */
            paEntry = "RF_TxPowerTable_HIGH_PA_ENTRY(" + bias + ", " + gain + ", "
                + boost + ", " + coefficient + ", " + ldoTrim + ")";
        }
        else {
            paEntry = "RF_TxPowerTable_DEFAULT_PA_ENTRY(" + bias + ", " + gain + ", "
                + boost + ", " + coefficient + ")";
        }
        return paEntry;
    }
    // RCL: don't generate code here
    return "";
}

/* eslint-enable no-bitwise */

/**
 *  ======== generateTxPowerCode ========
 *  Generate TX Power table (PA table)
 *
 *  @param inst - module instance
 *  @param target - target data
 *  @param nTable - the number (index) of this table
 */
function generateTxPowerCode(inst, target, nTable) {
    const paExport = getPaTableExportMethods();

    if (paExport.total > 0) {
        const typeName = "RF_TxPowerTable_Entry ";
        const tableTerminate = "    RF_TxPowerTable_TERMINATION_ENTRY\n};\n";

        // Standard PA table
        const infoStd = getPaTableInfo(target, false);

        let code = "\n// " + infoStd.description + "\n";
        code += typeName + infoStd.symTxPower + "[" + infoStd.symTxPowerSize + "] =\n{\n";
        code += generatePaTableCode(target.paTable, false);
        code += tableTerminate;

        // Need to generate code for high PA ?
        const infoHiPa = getPaTableInfo(target, true); // HighPA for current frequency band
        const highPaSupported = isHighPaSupported(inst, target.min) && infoHiPa !== null;

        if (highPaSupported) {
            if (paExport.separate > 0) {
                // High PA table
                code += "\n// " + infoHiPa.description + "\n";
                code += typeName + infoHiPa.symTxPower
                    + "[" + infoHiPa.symTxPowerSize + "] =\n{\n";
                code += generatePaTableCode(infoHiPa.paTable, false);
                code += tableTerminate;
            }

            if (paExport.combined > 0) {
                // Combined PA table
                const paTable = combinePaTable(target.paTable, infoHiPa.paTable);
                const codeComb = generatePaTableCode(paTable, true);

                const info = mergePaTableInfo(infoStd, infoHiPa);
                code += "\n// " + info.description + "\n";
                code += typeName + info.symTxPower + "[" + info.symTxPowerSize + "] =\n{\n";
                code += codeComb;
                code += tableTerminate;
            }
        }

        if (nTable === 1) {
            // Add header to the first PA table
            return Docs.txPowerDescription + code;
        }
        return code;
    }
    return null;
}

/**
 *  ======== generateTxPowerHeaderRFD ========
 *  Generate TX Power table header (PA table)
 *
 *  @param inst - module instance
 *  @param target - target data
 */
function generateTxPowerHeaderRFD(inst, target) {
    const paExport = getPaTableExportMethods();

    if (paExport.total > 0) {
        // Struct declaration
        const typeName = "extern RF_TxPowerTable_Entry ";

        // Standard PA table (always present)
        const infoStd = getPaTableInfo(target, false);
        let codeSz = "#define " + infoStd.symTxPowerSize + " " + (infoStd.paTable.length + 1)
            + " // " + infoStd.description + "\n";
        let code = typeName + infoStd.symTxPower + "[]; // " + infoStd.description + "\n";

        // Need to generate code for high PA ?
        const infoHiPa = getPaTableInfo(target, true); // HighPA for current frequency band
        const highPaSupported = isHighPaSupported(inst, target.min) && infoHiPa !== null;

        if (highPaSupported) {
            if (paExport.separate > 0) {
                // High PA table
                codeSz += "#define " + infoHiPa.symTxPowerSize + " " + (infoHiPa.paTable.length + 1)
                    + " // " + infoHiPa.description + "\n";
                code += typeName + infoHiPa.symTxPower + "[]; // " + infoHiPa.description + "\n";
            }
            if (paExport.combined) {
                // Combined PA table
                const paList = combinePaTable(target.paTable, infoHiPa.paTable);

                const info = mergePaTableInfo(infoStd, infoHiPa);
                codeSz += "#define " + info.symTxPowerSize + " " + (paList.length + 1)
                    + " // " + info.description + "\n";
                code += typeName + info.symTxPower + "[]; // " + info.description + "\n";
            }
        }
        const fbName = getFreqBandShortName(target.min);

        return {
            support: "#define SUPPORT_FREQBAND_" + fbName + "\n",
            define: codeSz,
            struct: code
        };
    }
    return null;
}

/**
 *  ======== generateTxPowerHeaderRCL ========
 *  Generate TX Power table header (PA table)
 *
 *  @param inst - module instance
 *  @param target - target data
 */
function generateTxPowerHeaderRCL(inst, target) {
    const fbName = getFreqBandShortName(target.min);

    return {
        support: "#define SUPPORT_FREQBAND_" + fbName + "\n",
        define: "",
        struct: ""
    };
}

/**
 *  ======== combinePaTable ========
 *  Merge PA tables for default and HIGH PA
 *
 *  @param paListStd - settings for default TX power
 *  @param paListHi - settings for high TX power
 */
function combinePaTable(paListStd, paListHi) {
    const paListComb = _.cloneDeep(paListHi.concat(paListStd));
    const ret = [];
    let high = -100;

    _.eachRight(paListComb, (item) => {
        const val = parseFloat(item._text);
        // Throw away items that break the ascending order
        if (val > high + 0.5) {
            ret.unshift(item);
            high = val;
            item._text = val;
        }
    });
    return ret;
}

/**
 *  ======== isHighPaSupported ========
 *  Return true a combined table is possible
 *
 *  @param inst - module instance
 *  @param freq - frequency from which to determine if a table is required
 */
function isHighPaSupported(inst, freq) {
    if (inst.fb24g !== "none" && inst.pa20 === "fb24g" && freq > 2000) {
        return true;
    }
    if (inst.fbSub1g !== "none" && inst.pa20 === "fbSub1g" && freq < 1000 && freq > Common.FreqHigher169) {
        return true;
    }
    return false;
}

/**
 *  ======== getPaTableSuffix ========
 *  Get the suffix for the PA table name, a function of the transmit power.
 *  (e.g. Tx5, Tx13, Tx20).
 *
 *  @param target - Target to operate on
*/
function getPaTableSuffix(paTable) {
    // Get the first (highest) entry of the PA table, this determines what PA table name to generate
    const paDefault = paTable[0];
    const pa = paDefault._text;

    let paSuffix;
    if (pa > 12.9 && pa < 14.9) {
        // Covers 13, 14 and 14.5
        paSuffix = "13";
    }
    else {
        // Use the highest entry directly
        paSuffix = Math.floor(pa).toString();
    }
    return paSuffix;
}

/**
 *  ======== getPaTableInfo ========
 *  Get the suffix and info string for the PA table name, a function of the transmit power.
 *  (e.g. Tx5, Tx13, Tx20).
 *
 *  @param target - virtual target to operate on
 *  @param highPa - if generating symbols for High PA
*/
function getPaTableInfo(target, highPa) {
    const paTable = getPaTable(target.max, highPa);
    if (paTable === null || paTable.length === 0) {
        // No PA table
        return null;
    }

    const paSuffix = getPaTableSuffix(paTable);
    const fbName = getFreqBandShortName(target.min);
    const ret = {
        paTable: paTable,
        pa: paSuffix,
        fbName: fbName
    };

    ret.description = fbName + " MHz, " + paSuffix + " dBm";
    ret.symTxPower = "txPowerTable_" + fbName + "_pa" + paSuffix;
    ret.symTxPowerSize = ret.symTxPower.toUpperCase() + "_SIZE";

    return ret;
}

/**
 *  ======== mergePaTableInfo ========
 *  Merge the PA table info for combined PA tables.
 *
 *  @param infoStd - information from standard PA table
 *  @param infoHigh - information from high power PA table
*/
function mergePaTableInfo(infoStd, infoHigh) {
    const ret = {...infoStd};

    ret.description = infoStd.fbName + " MHz, " + infoStd.pa + " + " + infoHigh.pa + " dBm";
    ret.symTxPower = "txPowerTable_" + infoStd.fbName + "_pa" + infoStd.pa + "_" + infoHigh.pa;
    ret.symTxPowerSize = ret.symTxPower.toUpperCase() + "_SIZE";

    return ret;
}

/**
 *  ======== genPaTableName ========
 *  Create a PA table name from PA and frequency band
 *
 *  @param fbName - frequency band id (169, 433, 868, 2400)
 *  @param pa - PA id ("5", "10", "13", "20")
 *  @param paHigh - PA id for combined table
*/
function genPaTableName(fbName, pa, paHigh) {
    let name = "txPowerTable_";

    const pa1 = modifyPaName(pa);
    const pa2 = modifyPaName(paHigh);
    name += fbName + "_pa" + pa1;

    if (pa2 !== null) {
        name += "_" + pa2;
    }
    return name;
}

/**
 *  ======== modifyPaName ========
 *
 *  Check if a PA name needs modification, change it if necessary.
 *
 *  @param - paOrig original name (any dBm value)
 *
 *  @return - PA id ("5", "10", "13", "20")
*/
function modifyPaName(paOrig) {
    let pa = paOrig;
    if (CurrentDesign.max_hpa === "10") {
        if (pa === "20") {
            pa = "10";
        }
    }
    if (pa > 12 && pa < 15) {
        pa = "13";
    }
    return pa;
}

/**
 *  ======== getTxPowerValueByDbm ========
 *  Get raw TX power value based on frequency, high PA and dbm
 *
 *  @param freqStr - selected frequency (kHz), represented as string
 *  @param highPA - use high PA settings
 *  @param dbm - value in dBm
 *
 *  @return - raw data value or null if not found
 */
function getTxPowerValueByDbm(freqStr, highPA, dbm) {
    const freq = parseFloat(freqStr);
    const paList = getPaTable(freq, highPA);

    for (const i in paList) {
        const pa = paList[i];
        if (pa._text === dbm) {
            let raw;
            if ("TxHighPa" in pa) {
                raw = pa.TxHighPa;
            }
            else {
                raw = pa.Value;
            }
            return raw;
        }
    }
    return null;
}

/**
 *  ======== getTxPowerDbmByRegValue ========
 *  Get dBm value based on frequency and raw register value
 *
 *  @param freqStr - selected frequency (kHz), represented as string
 *  @param raw - raw register value
 *
 *  @return - value in dBm
 */
function getTxPowerDbmByRegValue(freqStr, raw) {
    const freq = parseFloat(freqStr);

    // Pick settings according to PA
    const fb = getFrequencyBandByFreq(freq, false);
    const paList = fb.paTable;

    for (const i in paList) {
        const pa = paList[i];
        if (pa.Value === raw) {
            return pa._text;
        }
    }
    // Return the highest value if nothing else is found
    return paList[0]._text;
}

/**
 *  ======== getTxPowerValueByDbm ========
 *  Get TX Power default value for a given frequency and PA selection
 *
 *  @param freqStr - selected frequency (kHz), represented as string
 *  @param highPA - use high PA settings
 *
 *  @return - raw TxPower value
 */
function getTxPowerValueDefault(freqStr, highPA) {
    const freq = parseFloat(freqStr);
    let ret;

    // Pick settings according to PA
    const paList = getPaTable(freq, highPA);
    const pa = paList[0];

    if ("TxHighPa" in pa) {
        ret = pa.TxHighPa;
    }
    else {
        ret = pa.Value;
    }
    return ret;
}

/**
 *  ======== generateFrontEndCode ========
 *  Generate defines for a Front-end
 *
 *  @param inst - Rf Design instance
 */
function generateFrontEndCode(inst) {
    let frontEndCode = "";

    if ("fbSub1g" in inst && inst.fbSub1g !== "none") {
        frontEndCode += getFrontEndCode(inst.feSub1g, "SUB1G");
    }
    if ("fb24g" in inst && inst.fb24g !== "none") {
        frontEndCode += getFrontEndCode(inst.fe24g, "24G");
    }
    return frontEndCode;

    function getFrontEndCode(fe, id) {
        let code = "";

        if (fe.includes("D")) {
            code += "#define FRONTEND_" + id + "_DIFF_RF\n";
        }
        else if (fe.includes("RFP")) {
            code += "#define FRONTEND_" + id + "_SE_RFP\n";
        }
        else {
            code += "#define FRONTEND_" + id + "_SE_RFN\n";
        }
        if (fe.includes("X")) {
            code += "#define FRONTEND_" + id + "_EXT_BIAS\n";
        }
        else {
            code += "#define FRONTEND_" + id + "_INT_BIAS\n";
        }
        return code;
    }
}

/**
 *  ======== moduleInstances ========
 *  Executed when the module is instantiated
 *
 *  @param inst - module instance
 */
function moduleInstances(inst) {
    TiBoard = Common.getBoardName();
    HasTiBoard = TiBoard !== "";

    updateFreqBands(inst);

    return [];
}

/*
 *  ======== moduleStatic ========
 *  Define the module's static configurables
 */
const moduleStatic = {
    config: Config,
    validate: validate,
    moduleInstances: moduleInstances
};

/*
 *  ======== module ========
 *  Define the module
 */
const module = {
    displayName: "RF Design",
    description: "RF Design",
    longDescription: Docs.rfDesignDescription,
    moduleStatic: moduleStatic,
    prefix: Prefix,
    has10dBmPA: () => CurrentDesign.max_hpa === "10",
    isFreqBandSelected: isFreqBandSelected,
    getFreqBandShortName: getFreqBandShortName,
    getFrequencyBandByFreq: getFrequencyBandByFreq,
    getHighPaAssociation: getHighPaAssociation,
    getFrontEnd: getFrontEnd,
    getPaTable: getPaTable,
    genPaTableName: genPaTableName,
    getPaTableSuffix: getPaTableSuffix,
    getTxPowerOptions: getTxPowerOptions,
    getTxPowerValueByDbm: getTxPowerValueByDbm,
    getTxPowerDbmByRegValue: getTxPowerDbmByRegValue,
    getFreqBands: getActiveFreqBands,
    getCurrentDesign: () => CurrentDesign,
    generateTxPowerCode: generateTxPowerCode,
    generateTxPowerHeader: Common.isPlatformRFD() ? generateTxPowerHeaderRFD : generateTxPowerHeaderRCL,
    generateFrontEndCode: generateFrontEndCode,
    getTxPowerValueDefault: getTxPowerValueDefault
};

// Initialize the module data
initConfigurables();

exports = module;
