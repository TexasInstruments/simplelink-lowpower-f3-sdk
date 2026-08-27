/*
 * Copyright (c) 2022-2026, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== NVSLPF3.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;
let logWarning = Common.logWarning;

/* Define Flash sector size based on device type */
let SECTORSIZE = 0x800;

let config = [
    {
        name         : "regionType",
        displayName  : "Region Type",
        description  : "Specifies the type of region used",
        longDescription:`
Specifies the type of region used.

* __Generated__ - An internal flash region is automatically generated.
* __Pointer__ - Provide a pointer to the memory location of a
predefined internal flash region.
`,
        default      : "Generated",
        options      : [
            {
                name: "Generated",
                description: "An internal flash region is automatically"
                    + " generated."
            },
            {
                name: "Pointer",
                description: "Provide a pointer to the memory location"
                    + " of a predefined internal flash region."
            }
        ]
    },
    {
        name         : "regionBase",
        displayName  : "Region Base",
        description  : "Base address of the region. Must be aligned on an"
            + " integer multiple of the sector size.",
        longDescription : "NOTE: This setting has no effect when using the GCC"
            + " toolchain. The linker script must be manually modified if"
            + " the region must be at a specific location. See the nvsinternal"
            + " example README file for details.",
        displayFormat: "hex",
        default      : 0x0
    },
    {
        name         : "regionSize",
        displayName  : "Region Size",
        description  : "Size of the region in bytes. Must be a non-zero"
            + " multiple of the Sector Size",
        displayFormat: "hex",
        default      : SECTORSIZE
    },
    {
        name         : "sectorSize",
        displayName  : "Sector Size",
        description  : "Size of the erase sector in bytes",
        displayFormat: "hex",
        default      : SECTORSIZE,
        hidden       : false,
        readOnly     : true
    }
];

/*
 *  ======== getUsableFlashRange ========
 *  Returns the usable flash range { base, end } for the current device,
 *  accounting for HSM firmware that occupies the top of flash on CC27xx
 *  devices. Returns null if the information is not available.
 */
function getUsableFlashRange()
{
    try {
        let DriverLib = system.getScript("/ti/devices/DriverLib");
        let linkerDefs = DriverLib.getLinkerDefs();
        let flashBase = null;
        let flashSize = null;
        let hsmFwBase = null;
        for (let def of linkerDefs) {
            if (def.name === "FLASH0_BASE") { flashBase = def.value; }
            if (def.name === "FLASH0_SIZE") { flashSize = def.value; }
            if (def.name === "HSM_FW_BASE") { hsmFwBase = def.value; }
        }
        if (flashBase === null || flashSize === null) {
            return null;
        }
        let flashEnd = flashBase + flashSize;
        /* If HSM FW resides within flash, the linker reserves that region,
         * so the NVS-accessible flash ends at HSM_FW_BASE.
         */
        if (hsmFwBase !== null && hsmFwBase < flashEnd) {
            flashEnd = hsmFwBase;
        }
        return { base: flashBase, end: flashEnd };
    }
    catch (e) {
        return null;
    }
}

/*
 *  ======== validate ========
 */
function validate(inst, validation)
{
    let regionBase = inst.regionBase;
    let regionSize = inst.regionSize;
    let sectorSize = inst.sectorSize;

    if (sectorSize == 0) {
        let message = "Invalid sectorSize value: " + inst.sectorSize + ".";
        logError(validation, inst, "sectorSize", message);
    }
    else if ((regionSize == 0) || (regionSize % sectorSize != 0)) {
        let message = "Invalid regionSize value: " + inst.regionSize + "." +
            "\nMust be a non-zero multiple of " + sectorSize + " bytes.";
        logError(validation, inst, "regionSize", message);
    }
    if (regionBase % sectorSize) {
        let message = "Region Base address must be aligned on a " + sectorSize
            + " page boundary.";
        logError(validation, inst, "regionBase", message);
    }

    /* Verify that the region fits within the device's usable flash */
    let flashRange = getUsableFlashRange();
    if (flashRange !== null) {
        let flashBaseStr = "0x" + flashRange.base.toString(16).toUpperCase();
        let flashEndStr  = "0x" + flashRange.end.toString(16).toUpperCase();
        if (regionBase < flashRange.base || regionBase >= flashRange.end) {
            let message = "Region Base (0x" + regionBase.toString(16).toUpperCase()
                + ") is outside the device's usable flash ["
                + flashBaseStr + ", " + flashEndStr + ").";
            logError(validation, inst, "regionBase", message);
        }
        else if ((regionBase + regionSize) > flashRange.end) {
            let regionEnd = "0x" + (regionBase + regionSize).toString(16).toUpperCase();
            let message = "NVS region end (" + regionEnd + ") exceeds the"
                + " device's usable flash (" + flashEndStr + ")."
                + " Reconfigure the region base/size to fit within flash.";
            logError(validation, inst, "regionBase", message);
            logError(validation, inst, "regionSize", message);
        }
    }

    /* verify that this region does not overlap within any other region */
    for (let i = 0; i < inst.$module.$instances.length; i++) {
        let tinst = inst.$module.$instances[i];
        if ((inst == tinst)) continue;
        let tRegionBase = tinst.regionBase;
        let tRegionSize = tinst.regionSize;
        if ((regionBase >= tRegionBase) &&
            (regionBase < (tRegionBase + tRegionSize))) {
            let message = "Region Base overlaps with NVS region: " + tinst.$ownedBy.$name + ".";
            logWarning(validation, inst, "regionBase", message);
            break;
        }
        if (((regionBase + regionSize) > tRegionBase) &&
            ((regionBase + regionSize) <= (tRegionBase + tRegionSize))) {
            let message = "Region Base + Region Size overlaps with NVS region: " + tinst.$ownedBy.$name + ".";
            logWarning(validation, inst, "regionBase", message);
            break;
        }
    }
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This should only modify the base config
 */
 function extend(base)
 {
    return (base);
 }


/*
 *  ======== exports ========
 */
exports = {
    config: config,
    validate: validate,
    extend
};
