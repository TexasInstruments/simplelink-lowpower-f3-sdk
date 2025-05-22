/*
 * Copyright (c) 2018-2025, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== TRNG.syscfg.js ========
 *  True Random Number Generator
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common   = system.getScript("/ti/drivers/Common.js");
let CryptoCommon = system.getScript("/ti/drivers/CryptoCommon.syscfg.js");

/* get /ti/drivers family name from device object */
let family   = CryptoCommon.device2Family(system.deviceData, "TRNG");

let config = [];

/* get device ID */
let deviceId = system.deviceData.deviceId;

/*
 *  ======== validate ========
 */
function validate(inst, validation)
{
    if (system.modules["/ti/utils/TrustZone"]) {
        if (deviceId.match(/CC(13|26).[34]/) && (inst.$module.$instances.length != 2)) {
            validation.logError(
                `When TrustZone is enabled for Secure/Non-secure isolation, ` +
                `the number of Crypto driver instances is fixed in the TF-M ` +
                `image. Two ` + base.displayName + ` instances are supported.`, inst);
        }
        else if (deviceId.match(/CC27/) && (inst.$module.$instances.length != 0)) {
            validation.logError(
                `When TrustZone is enabled for Secure/Non-secure isolation, no ` +
                base.displayName + ` instances can be added. PSA Crypto APIs must be ` +
                `used instead.`, inst);
        }
    }
}

/*
 *  ======== base ========
 *  Define the base TRNG properties and methods
 */
let base = {
    displayName         : "TRNG",
    description         : "True Random Number Generator (TRNG)",
    longDescription     : `
The [__TRNG driver__][1] generates numbers of
variable lengths from a source of entropy. The output
is suitable for applications requiring cryptographically
secure random numbers such as keying material for
private or symmetric keys.

* [Usage Synopsis][2]
* [Examples][3]
* [Configuration Options][4]

[1]: /secure_drivers/doxygen/html/_t_r_n_g_8h.html#details "C API reference"
[2]: /secure_drivers/doxygen/html/_t_r_n_g_8h.html#ti_drivers_TRNG_Synopsis "Basic C usage summary"
[3]: /secure_drivers/doxygen/html/_t_r_n_g_8h.html#ti_drivers_TRNG_Examples "C usage examples"
[4]: /drivers/syscfg/html/ConfigDoc.html#TRNG_Configuration_Options "Configuration options reference"
`,
    defaultInstanceName : "CONFIG_TRNG_",
    config              : Common.addNameConfig(config, "/ti/drivers/TRNG", "CONFIG_TRNG_"),
    modules: (inst) => {
        let forcedModules = ["Board", "Power", "CryptoBoard"];

        if (deviceId.match(/CC27/)) {
            /* HSM library requires Key Store module */
            forcedModules.push("CryptoKeyKeyStore_PSA");
        }

        return Common.autoForceModules(forcedModules)();
    },
    validate            : validate
};

/* get family-specific TRNG module */
let devTRNG = system.getScript("/ti/drivers/trng/TRNG" + family);
exports = devTRNG.extend(base);
