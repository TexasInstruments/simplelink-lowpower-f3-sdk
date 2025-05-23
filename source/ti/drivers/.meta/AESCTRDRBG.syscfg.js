/*
 * Copyright (c) 2019-2025, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== AESCTRDRBG.syscfg.js ========
 *  AES (Advanced Encryption Standard) Counter Deterministic Random Bit
 *  Generator
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/* generic configuration parameters for AESCTRDRBG instances */
let config = []; /* nothing (yet) beyond generic driver configs */

/* get device ID */
let deviceId = system.deviceData.deviceId;

/*
 *  ======== validate ========
 */
function validate(inst, validation)
{
    if (system.modules["/ti/utils/TrustZone"]) {
        if (deviceId.match(/CC(13|26).[34]/) && (inst.$module.$instances.length != 1)) {
            validation.logError(
                `When TrustZone is enabled for Secure/Non-secure isolation, ` +
                `the number of Crypto driver instances is fixed in the TF-M ` +
                `image. Only one ` + base.displayName + ` instance is supported.`, inst);
        }
    }
}

/*
 *  ======== base ========
 *  Define the base AESCTRDRBG properties and methods
 */
let base = {
    displayName         : "AESCTRDRBG",
    description         : "AES (Advanced Encryption Standard) Counter"
        + " Deterministic Random Bit Generator (CTRDRBG) Driver",
    alwaysShowLongDescription : true,
    longDescription     : `
The [__AESCTRDRBG driver__][1] generates cryptographically secure random
numbers. Its output may be used as keying material or for other security
related purposes.

* [Usage Synopsis][2]
* [Examples][3]

[1]: /secure_drivers/doxygen/html/_a_e_s_c_t_r_d_r_b_g_8h.html#details "C API reference"
[2]: /secure_drivers/doxygen/html/_a_e_s_c_t_r_d_r_b_g_8h.html#ti_drivers_AESCTRDRBG_Synopsis "Basic C usage summary"
[3]: /secure_drivers/doxygen/html/_a_e_s_c_t_r_d_r_b_g_8h.html#ti_drivers_AESCTRDRBG_Examples "C usage examples"
[4]: /drivers/syscfg/html/ConfigDoc.html#AESCTRDRBG_Configuration_Options "Configuration options reference"
`,
    config              : Common.addNameConfig(config, "/ti/drivers/aesctrdrbg", "CONFIG_AESCTRDRBG_"),
    defaultInstanceName : "CONFIG_AESCTRDRBG_",
    validate            : validate
};

/* extend the base exports to include family-specific content */
/* For now, there is only one generic implementation. We may get dedicated hardware
 * backed implementations later.
 */
let devAESCTRDRBG = system.getScript("/ti/drivers/aesctrdrbg/AESCTRDRBGXX");

exports = devAESCTRDRBG.extend(base);
