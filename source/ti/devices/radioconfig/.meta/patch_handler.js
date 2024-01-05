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
 *  ======== patch_handler.js ========
 *  Functions for generating patch information
 */

"use strict";

// Common utility functions
const Common = system.getScript("/ti/devices/radioconfig/radioconfig_common.js");

// Module base path
const PhyGroupPath = Common.phyGroupPath;

// Command handler, contains patch information
const PhyHandler = Common.getPhyHandler();

/**
*  ======== generateIncludesC ========
*  Generated include directives for use in the implementation file (*.c)
*
*  @param modules - all modules in the SDK
*  @param coexType - ble or ieee
*/
function generateIncludesC(modules, coexType) {
    // Common includes (for compatibility with SmartRF Studio)
    let incl = "";
    let patchIncludes = {};

    _.each(modules, (modpath) => {
        let phyGroup;

        // Add DriverLib includes
        if (modpath === PhyGroupPath + "prop") {
            phyGroup = Common.PHY_PROP;
        }
        else if (modpath === PhyGroupPath + "ble") {
            phyGroup = Common.PHY_BLE;
        }
        else if (modpath === PhyGroupPath + "ieee_15_4") {
            phyGroup = Common.PHY_IEEE_15_4;
        }
        else {
            // Not a RadioConfig module, skip
            return true;
        }

        // Aggregate patch includes
        const mod = system.modules[modpath];
        patchIncludes = Object.assign(patchIncludes, getPatchIncludes(phyGroup, mod.$instances, coexType));

        return true;
    });

    // Iterate patch includes
    _.each(patchIncludes, (value, patchName) => {
        incl += patchInclude(patchName);
    });

    return incl;
}

/**
*  ======== getPatchIncludes ========
*  When generated code for multiple settings, header files
*  for all required patches must be included.
*
*  @param phyGroup - ble, prop or ieee_154
*  @param instances - list of instances to generate patch includes for
*  @param coexType - coex_ble or coex_ieee
*/
function getPatchIncludes(phyGroup, instances, coexType) {
    const includes = {};

    _.each(instances, (inst) => {
        let phy = null;

        if ("phyType" in inst) {
            phy = inst.phyType;
        }
        else if ("freqBand" in inst) {
            phy = Common.getPhyType(inst);
        }
        else {
            throw Error("Unknown PHY type");
        }

        const protocol = getPatchType(inst.codeExportConfig, coexType);
        const patch = PhyHandler.get(phyGroup, phy).getPatchInfo(protocol);
        if (typeof (patch.cpe) === "string") {
            includes[patch.cpe] = true;
        }
        if (typeof (patch.rfe) === "string") {
            includes[patch.rfe] = true;
        }
        if (typeof (patch.mce) === "string") {
            includes[patch.mce] = true;
        }
    });
    return includes;
}

/**
*  ======== getPatchType ========
*  Get the patch type: multi-protocol or default.
*
*  @param ceConfig - Code export configuration
*/
function getPatchType(ceConfig, coexType) {
    if (coexType) {
        return coexType;
    }

    const ui = ceConfig.$uiState;
    if (ui.useMulti.hidden) {
        return "default";
    }
    return ceConfig.useMulti ? "multi" : "default";
}

/**
*  ======== patchInclude ========
*  Generate include directive for RF patches
*
*  @param patchName - name of the patch (e.g. rf_patch_cpe_bt5)
*/
function patchInclude(patchName) {
    return "#include DeviceFamily_constructPath(rf_patches/" + patchName + ".h)\n";
}

/**
 *  ======== getAllFwImages ========
 *  Construct collection of all firmware images that is used in a configuration.
 *  This function only applies to RCL interfaces only.
 */
function getAllFwImages() {
    const modules = Object.keys(system.modules).sort();
    const images = {};

    // Iterate all SysConfig modules
    for (const modpath of modules) {
        // RadioConfig module ?
        if (modpath.includes("radioconfig/phy_groups")) {
            const mod = system.modules[modpath];
            // Iterate each PHY definition instance
            for (const inst of mod.$instances) {
                const phyGroup = Common.getPhyGroup(inst);
                const phyID = Common.getPhyType(inst, phyGroup);
                const phyDef = PhyHandler.get(phyGroup, phyID);
                // Iterate FW images in a PHY definition
                for (const item of phyDef.getLrfImages()) {
                    const key = item.variable;
                    // Avoid duplicates
                    if (!(key in images)) {
                        item.phy = phyID; // The PHY definition under which the FW image code is to be inserted
                        item.lrfImage = inst.codeExportConfig.lrfImages; // Code configuration: none, include or embed
                        images[key] = item;
                    }
                }
            }
        }
    }
    return images;
}

/**
 *  ======== generateLrfFwIncludes ========
 *  Generate code for including LRF firmware images.
 *
 */
function generateLrfFwIncludes() {
    let code = "";
    const allImages = getAllFwImages();

    // eslint-disable-next-line guard-for-in
    for (const key in allImages) {
        const imgInfo = allImages[key];
        if (imgInfo.lrfImage === "header") {
            code += `#include DeviceFamily_constructPath(${imgInfo.header})\n`;
        }
    }
    return code;
}

/**
 *  ======== generateFwImageCode ========
*  Generate code for the firmware images.
*
*  @param phyDef  - The PHY definition instance to generate code for
*/
function generateFwImageCode(phyDef) {
    let code = "";
    const fwInfo = phyDef.getLrfImages();
    const allImages = getAllFwImages();

    // Iterate images
    for (const item of fwInfo) {
        const sym = item.variable;
        if (sym in allImages) {
            const imgInfo = allImages[sym];

            if (imgInfo.lrfImage === "embed" && imgInfo.phy === phyDef.getPhyName()) {
                const [role1, role2] = item.role.split("_");
                const fwFile = item.data;
                const header = `\n// LRF ${role1.toUpperCase()} ${role2} (${fwFile})\n`;

                code += header + `static const uint32_t ${sym}[] =\n{\n`;

                // Iterate HEX code lines
                for (const line of item.code) {
                    code += "    " + line + "\n";
                }
                code += "};\n";
            }
        }
    }
    return code;
}

// Exported from this module
exports = {
    // RFD
    generateIncludesC: generateIncludesC,
    getPatchType: getPatchType,
    // RCL
    generateLrfFwIncludes: generateLrfFwIncludes,
    generateFwImageCode: generateFwImageCode
};
