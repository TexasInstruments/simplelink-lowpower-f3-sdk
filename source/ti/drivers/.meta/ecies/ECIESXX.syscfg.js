/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  ======== ECIESXX.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/* get device ID */
let deviceId = system.deviceData.deviceId;

/* Interrupt Priority for internal AES-GCM instance */
let aesIntPriority = Common.newIntPri()[0];
aesIntPriority.name = "aesInterruptPriority";
aesIntPriority.displayName = "AES Interrupt Priority";
aesIntPriority.description = "AES interrupt priority used for AES-128-GCM encryption";

/* Interrupt Priority for internal RNG instance */
let rngIntPriority = Common.newIntPri()[0];
rngIntPriority.name = "rngInterruptPriority";
rngIntPriority.displayName = "RNG Interrupt Priority";
rngIntPriority.description = "RNG interrupt priority used to generate the random private key";

/* Interrupt Priority for internal KDF instance */
let kdfIntPriority = Common.newIntPri()[0];
kdfIntPriority.name = "kdfInterruptPriority";
kdfIntPriority.displayName = "KDF Interrupt Priority";
kdfIntPriority.description = "KDF interrupt priority used to derive the AES-128-GCM key and IV";

/* Interrupt Priority for internal ECDH instance */
let ecdhIntPriority = Common.newIntPri()[0];
ecdhIntPriority.name = "ecdhInterruptPriority";
ecdhIntPriority.displayName = "ECDH Interrupt Priority";
ecdhIntPriority.description = "ECDH interrupt priority used for public key generation and shared secret computation";

let cfg = [aesIntPriority, rngIntPriority];

if (!deviceId.match(/CC23|CC13.1|CC26.1/))
{
    /* Add interrupt priority for KDF and ECDH */
    cfg.push(kdfIntPriority);
    cfg.push(ecdhIntPriority);
}

/*
 *  ======== validate ========
 */
function validate(inst, validation) {
    /* Validate RNG global params */
    let rng = system.modules["/ti/drivers/RNG"];
    rng.validate(null, validation);
}

/*
 *  ======== getLibs ========
 *  Argument to the /ti/utils/build/GenLibs.cmd.xdt template
 */
function getLibs(mod)
{
    /* Get device information from GenLibs */
    let GenLibs = system.getScript("/ti/utils/build/GenLibs");

    let libGroup = {
        name: "/third_party/ecc",
        deps: [],
        libs: [GenLibs.libPath("third_party/ecc", "ecc.a")],
        allowDuplicates: true
    };

    return (libGroup);
}

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base ECIES configuration
 */
let devSpecific = {
    validate: validate,

    config: cfg,

    /* override device-specific templates */
    templates: {
        boardh: "/ti/drivers/ecies/ECIES.Board.h.xdt",
        boardc: "/ti/drivers/ecies/ECIESXX.Board.c.xdt",

        /* contribute libraries to linker command file */
        "/ti/utils/build/GenLibs.cmd.xdt":
                {modName: "/ti/drivers/ECIES", getLibs: getLibs}
    }
};

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic ECIES module to
 *  allow us to augment/override as needed for the generic implementation.
 */
function extend(base)
{
    /* display which driver implementation can be used */
    base = Common.addImplementationConfig(base, "ECIES", null,
        [{name: "ECIESXX"}], null);

    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base ECIES module */
    extend: extend
};
