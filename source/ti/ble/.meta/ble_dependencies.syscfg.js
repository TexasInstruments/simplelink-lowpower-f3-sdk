/*
 * Copyright (c) 2025 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble_dependencies.js ========
 *  Shared driver dependencies for all BLE modules (ble, ble_controller, ble_host).
 */

"use strict";

/*
 * ======== modules ========
 * Static driver dependencies: RCL
 * Hidden so they appear in the "Other Dependencies" section.
 */
const modules = [
    {
        name: "rcl",
        displayName: "RCL",
        moduleName: "/ti/drivers/RCL",
        hidden: true
    }
];

/*
 * ======== moduleInstances ========
 * Driver instance dependencies: RNG, AESCCM, ECDH, AESCTRDRBG, AESECB
 * Hidden so they appear in the "Other Dependencies" section.
 */
const moduleInstances = [
    {
        name: "rng",
        displayName: "RNG",
        moduleName: "/ti/drivers/RNG",
        hidden: true
    },
    {
        name: "aesccm",
        displayName: "AESCCM",
        moduleName: "/ti/drivers/AESCCM",
        hidden: true
    },
    {
        name: "ecdh",
        displayName: "ECDH",
        moduleName: "/ti/drivers/ECDH",
        hidden: true
    },
    {
        name: "aesctrdrbg",
        displayName: "AESCTRDRBG",
        moduleName: "/ti/drivers/AESCTRDRBG",
        hidden: true
    },
    {
        name: "aesecb",
        displayName: "AESECB",
        moduleName: "/ti/drivers/AESECB",
        hidden: true
    }
];

/*
 * ======== ccfgDependencies ========
 * CCFG module dependency shared by all BLE modules.
 * Watches srcClkLF and updates useRcosc / peripheralExtraLFOSCPPM visibility.
 */
const ccfgDependencies = {
    modules: { "/ti/devices/CCFG": ["srcClkLF"] },
    onModuleChanged: function (inst, depInst, moduleName, configurable) {
        let ui = inst.$uiState;
        if (depInst) {
            if (depInst.srcClkLF == "LF RCOSC") {
                inst.useRcosc = true;
                ui.peripheralExtraLFOSCPPM.hidden = false;
            }
            else {
                inst.useRcosc = false;
                ui.peripheralExtraLFOSCPPM.hidden = true;
            }
        }
    }
};

exports = {
    modules: modules,
    moduleInstances: moduleInstances,
    ccfgDependencies: ccfgDependencies
};
