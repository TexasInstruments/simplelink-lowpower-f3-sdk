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
 *  ======== override_handler.js ========
 *  Module to process and store Override data, SmartRF Studio 8 data format.
 */

"use strict";

// Common utility functions
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");

// Other dependencies
const RfDesign = Common.getScript("rfdesign");

// Populated during init()
let Overrides;
let VddrBoost;

let CCFG;

/**
 *  ======== init ========
 *  Load overrides from settings file
 *
 *  @param rfCmds - RF commands from settings file
 *  @param highPA - true if High PA enabled
 */
function init(rfCmds, highPA) {
    // Override table must be re-generated for each setting
    Overrides = [];
    VddrBoost = false;

    for (const gcmd of rfCmds) {
        const cmd = _.cloneDeep(gcmd);
        const cmdName = cmd.name;

        if ("rfd_command_field" in cmd) {
            // Iterate fields
            const fields = Common.forceArray(cmd.rfd_command_field);

            for (const field of fields) {
                if ("name" in field) {
                    const ovrName = field.name;

                    if (ovrName.includes("pRegOverride") && "rfd_override_block" in field) {
                        // Skip TxPower overrides unless High PA is set.
                        if (ovrName.includes("pRegOverrideTx") && !highPA) {
                            return;
                        }

                        const entry = {
                            ptrName: ovrName,
                            cmdName: cmdName
                        };

                        // Iterate blocks
                        const blocks = Common.forceArray(field.rfd_override_block);
                        for (let j = 0; j < blocks.length; j++) {
                            const block = blocks[j];
                            const brief = block.brief;

                            entry[brief] = block;
                        }
                        Overrides.push(entry);
                    }
                }
            }
        }
    }
}

/**
 *  ======== generateCode ========
 *  Generates the contents of all Overrides arrays
 *  used by this setting.
 *
 *  @param symName - symbol name for the override
 *  @param data    - data used for code generation
 *  @param custom  - information on custom overrides
 *  @returns object with offsets and generated code
 */
function generateCode(symName, data, custom) {
    const ret = {
        code: "",
        stackOffset: 0,
        appOffset: 0
    };
    CCFG = system.modules["/ti/devices/CCFG"];

    // Create lookup-table with pointer names as keys
    const ovrTmp = {};
    for (const item of Overrides) {
        ovrTmp[item.ptrName] = item;
    }

    let tmpCustom = custom;
    for (const item of Object.values(ovrTmp)) {
        const struct = generateStruct(item, data, tmpCustom);
        ret.code += struct.code;
        if (tmpCustom !== null) {
            ret.stackOffset = struct.stackOffset;
            ret.appOffset = struct.appOffset;
        }
        // Custom override only applies to the common structure (first in the list)
        tmpCustom = null;
    }
    ret.code = ret.code.replace(/pRegOverride/g, symName);

    return ret;
}

/**
 *  ======== getRepetitionFactor ========
 *  Get the repetition factor for the symbol rate.
 *
 *  @returns repetition factor number
 */
function getRepetitionFactor() {
    let rep = 1;

    for (const ovr of Overrides) {
        if (ovr.cmdName.includes("CMD_PROP_RADIO_DIV_SETUP")) {
            for (const key in ovr) {
                if (key === "cmdName" || key === "ptrName") {
                    // eslint-disable-next-line no-continue
                    continue;
                }
                const obuf = ovr[key].rfd_override_element;
                const items = Common.forceArray(obuf);
                // eslint-disable-next-line no-loop-func
                _.each(items, (el) => {
                    if (el.type === "HW_REG_OVERRIDE") {
                        const val = el.params;
                        if (val.includes("0x5324")) {
                            // Extract repetition factor
                            rep = parseInt(val.replace("0x5324,", ""));
                        }
                    }
                });
            }
        }
    }
    return rep;
}

/**
 *  ======== getStructNames ========
 *  Get array with the names of the override structs
 *
 *  @param symName - symbol name for the override tables
 *  @returns array with the names of the override structs
 */
function getStructNames(symName) {
    const names = [];

    for (const ovr of Overrides) {
        const name = ovr.ptrName.replace("pRegOverride", symName);
        names.push(name);
    }

    // Remove duplicates
    return _.uniq(names);
}

/**
 *  ======== updateTxPowerOverride ========
 *  Generate TX power table (PA table)
 *
 *  @param txPower - selected txPower (dBm)
 *  @param freq - frequency (MHz)
 *  @param highPA - True if using high PA
 */
function updateTxPowerOverride(txPower, freq, highPA) {
    // Find override for actual TxPower setting
    const paList = RfDesign.getPaTable(freq, highPA);
    VddrBoost = false;

    _.each(paList, (pv) => {
        // Check if current TX Power value has a VDDR boost option
        const val = pv._text;
        if (val === txPower && pv.boost_vddr) {
            VddrBoost = true;
        }
    });
}

/**
 *  ======== generateStruct ========
 *  Generate code for the override structure
 *
 *  @param override - override symbol information
 *  @param data     - override data
 *  @param custom   - custom override info
 *  @returns generated struct code and offsets
 */
function generateStruct(override, data, custom) {
    const ret = {
        code: "// Overrides for " + override.cmdName + "\n",
        stackOffset: 0,
        appOffset: 0
    };
    ret.code += "uint32_t " + override.ptrName + "[] =\n{\n";
    let nEntries = 0;
    const coExEnabled = Common.getCoexConfig() !== null;

    // Generate the code
    for (const key in override) {
        if (key === "cmdName" || key === "ptrName") {
            // eslint-disable-next-line no-continue
            continue;
        }
        if (!("rfd_override_element" in override[key])) {
            // eslint-disable-next-line no-continue
            continue;
        }
        const obuf = override[key].rfd_override_element;
        if (obuf.length === 0) {
            // eslint-disable-next-line no-continue
            continue;
        }
        // 14 dBm TX Power only apply to default override and skip unless VDDR enabled
        if (key.includes("14dbm") && (override.ptrName !== "pRegOverride" || !VddrBoost)) {
            // eslint-disable-next-line no-continue
            continue;
        }
        // Skip HPOSC unless enabled in CCFG
        if (key.includes("override_hposc")) {
            if (!CCFG.$static.enableXoscHfComp) {
                // eslint-disable-next-line no-continue
                continue;
            }
        }
        // Skip Co-Ex unless available and enabled
        if (key.includes("coex")) {
            const isCoExOvr = !key.includes("non_coex");
            let bGenerateCode;
            if (coExEnabled) {
                bGenerateCode = isCoExOvr;
            }
            else {
                bGenerateCode = !isCoExOvr;
            }
            if (!bGenerateCode) {
                // eslint-disable-next-line no-continue
                continue;
            }
        }
        // Name of override file
        ret.code += "    // " + key + "\n";

        // If there is more than one element, use array
        const items = Common.forceArray(obuf);

        // Process override array
        ret.code = processOverrideArray(items, ret.code, data);
        nEntries += items.length;
    }
    // Add app/stack specific overrides if applicable
    if (custom !== null) {
        let path = custom.stackOverride;
        if (path !== "") {
            ret.code += "    // " + path + "\n";
            ret.code += "    " + custom.stackOverrideMacro + "(),\n";
            ret.stackOffset = nEntries;
            nEntries += 1;
        }

        path = custom.appOverride;
        if (path !== "") {
            ret.code += "    // " + path + "\n";
            ret.code += "    " + custom.appOverrideMacro + "(),\n";
            ret.appOffset = nEntries;
        }
    }
    // Add termination
    ret.code += "    (uint32_t)0xFFFFFFFF\n};\n\n";

    return ret;
}

/**
 *  ======== processOverrideArray ========
 *  Generate code for the override structure
 *
 *  @param items     - override items
 *  @param codeParam - code structure
 *  @param data      - override data
 *  @returns generated code for override array
 */
function processOverrideArray(items, codeParam, data) {
    let code = codeParam;
    let hasTx20 = false;

    _.each(items, (el) => {
        code += "    // " + el.brief + "\n";

        let txPowStd;
        let txPow20;
        let anaDiv;

        const elType = el.type;
        const elValue = el.params;

        switch (elType) {
        case "HW_REG_OVERRIDE":
        case "MCE_RFE_OVERRIDE":
        case "HW32_ARRAY_OVERRIDE":
        case "ADI_HALFREG_OVERRIDE":
        case "ADI_REG_OVERRIDE":
        case "ADI_2HALFREG_OVERRIDE":
        case "HPOSC_OVERRIDE":
            code += "    " + elType + "(" + elValue + ")";
            break;
        case "TX_STD_POWER_OVERRIDE":
            txPowStd = data.txPower;
            code += "    TX_STD_POWER_OVERRIDE(" + Common.int2hex(txPowStd, 4) + ")";
            break;
        case "TX20_POWER_OVERRIDE":
            txPow20 = data.txPowerHi;
            code += "    TX20_POWER_OVERRIDE(" + Common.int2hex(txPow20, 8) + ")";
            hasTx20 = true;
            break;
        case "ANADIV":
            anaDiv = calcAnaDiv(data, hasTx20);
            code += "    (uint32_t)" + Common.int2hex(anaDiv, 8);
            break;
        case "ELEMENT":
        case "POINTER":
            // Convert to uppercase HEX if a hex value, otherwise leave as is
            {
                let val = elValue;
                if (Common.isHex(val)) {
                    val = Common.int2hex(val, 8);
                }
                code += "    (uint32_t)" + val;
            }
            break;
        default:
            code += "//** ERROR: Element type not implemented: " + elType;
            break;
        }
        code += ",\n";
    });

    return code;
}

/**
 *  ======== calcAnaDiv ========
 *  Calculate analog divider
 *
 *  @param data   - front end settings
 *  @param isTx20 - true if high PA applies
 *  @returns Calculated ahalog divider
 */
function calcAnaDiv(data, isTx20) {
    let fsOnly = 0;
    let frontEndMode = 0;
    let txSetting = 0;

    if (isTx20) {
        frontEndMode = 255;
    }
    else {
        frontEndMode = parseInt(data.frontend);
    }

    switch (parseInt(data.loDivider)) {
    case 0:
        fsOnly = 0x0502;
        break;
    case 2:
        fsOnly = 0x0102;
        break;
    case 4:
    case 6:
    case 12:
        fsOnly = 0xF101;
        break;
    case 5:
    case 10:
    case 15:
    case 30:
        fsOnly = 0x1101;
        break;
    default:
        // Error, should not occur!
    }

    /* eslint-disable no-bitwise */
    if (frontEndMode === 255) {
        // Special value meaning 20 dBm PA
        txSetting = (fsOnly | 0x00C0) & ~0x0400;
    }
    else if (frontEndMode === 0) {
        // Differential
        txSetting = fsOnly | 0x0030;
    }
    else if (frontEndMode & 1) {
        // Single ended on RFP
        txSetting = fsOnly | 0x0010;
    }
    else {
        // Single ended on RFN
        txSetting = fsOnly | 0x0020;
    }

    return ((txSetting << 16) | 0x0703);
}

// Exported functions
exports = {
    init: init,
    updateTxPowerOverride: updateTxPowerOverride,
    getStructNames: getStructNames,
    getRepetitionFactor: getRepetitionFactor,
    generateCode: generateCode
};
