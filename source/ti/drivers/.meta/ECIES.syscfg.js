/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ECIES.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/* generic configuration parameters for ECIES instances */
let config = [];

/* get device ID */
let deviceId = system.deviceData.deviceId;

/*
 *  ======== base ========
 *  Define the base ECIES properties and methods
 */
let base = {
    displayName         : "ECIES",
    description         : "Elliptic Curve Integrated Encryption Driver",
    alwaysShowLongDescription : true,
    longDescription     : `
The [__ECIES driver__][1] driver provides a public-key encryption scheme based
on Elliptic Curve Cryptography. It uses the NIST P-256 Elliptic Curve to generate
a shared secret which is used as an input for ANSI X9.36 Key Derivation Function
(KDF) to derive the key and IV for authenticated encryption using AES-128-GCM.

* [Usage Synopsis][2]
* [Examples][3]

[1]: /drivers/doxygen/html/_e_c_i_e_s_8h.html#details "C API reference"
[2]: /drivers/doxygen/html/_e_c_i_e_s_8h.html#ti_drivers_ECIES_Synopsis "Basic C usage summary"
[3]: /drivers/doxygen/html/_e_c_i_e_s_8h.html#ti_drivers_ECIES_Examples "C usage examples"
[4]: /drivers/syscfg/html/ConfigDoc.html#ECIES_Configuration_Options "Configuration options reference"
`,
    config              : Common.addNameConfig(config, "/ti/drivers/ecies", "CONFIG_ECIES_"),
    defaultInstanceName : "CONFIG_ECIES_",
    /* RNG module is required because it has a global configuration needed for
     * the dynamic RNG driver instance created by the ECIES driver.
     */
    modules: (inst) => {
        let forcedModules = ["Board", "Power", "RNG"];

        if (deviceId.match(/CC23/)) {
            /* LAES driver requires DMA module */
            forcedModules.push("DMA");
        }

        return Common.autoForceModules(forcedModules)();
    }
};

/* For now, there is only one generic implementation. We may get dedicated hardware
 * backed implementations later.
 */
let devECIES = system.getScript("/ti/drivers/ecies/ECIESXX");

/* extend the base exports to include family-specific content */
exports = devECIES.extend(base);
