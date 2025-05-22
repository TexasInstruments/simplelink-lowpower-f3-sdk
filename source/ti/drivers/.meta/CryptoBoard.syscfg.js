/*
 * Copyright (c) 2025, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== CryptoBoard.js ========
 *  Secure Drivers dependency linker
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/*
 *  ======== getLibs ========
 */
function getLibs(mod)
{
    /* Get device ID to select appropriate libs */
    let devId = system.deviceData.deviceId;

    /* Get device information from DriverLib */
    var DriverLib = system.getScript("/ti/devices/DriverLib");
    let family = DriverLib.getAttrs(devId).libName;

    /* Get toolchain specific information from GenLibs */
    let GenLibs = system.getScript("/ti/utils/build/GenLibs");

    /* The drivers libraries with logging enabled use a _log suffix vs the
     * unlogged library name.
     * If any LogModule is shared by a drivers module the _log suffix must
     * be selected.
     */
    var log_suffix = "";
    if (system.modules["/ti/log/LogModule"]) {
        // eslint-disable-next-line no-undef
        if (_.some(system.modules["/ti/log/LogModule"].$instances,
                   (inst) => {
                       if (inst.$ownedBy)
                       {
                           return inst.$ownedBy.$module.$name.startsWith("/ti/drivers");
                       }
                       else
                       {
                           return false;
                       }
                    }
                   )
            )
        {
            /* For now, logging is not supported on Secure Drivers.
             * When logging is enabled for Secure Drivers, suffix can be set to
             * appropriate string, for example '_log'.
             */
            log_suffix = "";
        }
    }

    let libPath = GenLibs.libPath;
    let libs = [];

    if (family != "") {
        /* Add dependency on PSA Crypto library if KeyStore module is present. */
        if (system.modules["/ti/drivers/CryptoKeyKeyStore_PSA"]) {
            libs.push(libPath("third_party/psa_crypto", "psa_crypto_" + family + ".a"));
        }

        /* secure_drivers must be ahead of regular drivers to satisfy the dependency */
        if (system.modules["/ti/drivers/CryptoBoard"]) {
            if (system.modules["/ti/utils/TrustZone"]) {
                libs.push(libPath("ti/drivers/secure","secure_drivers_" + family + "_ns" + log_suffix +".a"));
            }
            else {
                libs.push(libPath("ti/drivers/secure","secure_drivers_" + family + log_suffix +".a"));
            }
        }
    }

    if (libs == null) {
        throw Error("device2LinkCmd: unknown device family ('" + family + "') for deviceId '" + devId + "'");
    }

    /* Create a GenLibs input argument */
    var linkOpts = {
        name: "/ti/drivers/secure",
        vers: "1.0.0.0",
        deps: ["/ti/devices", "/ti/drivers"],
        libs: libs
    };

    if (system.modules["/ti/drivers/ECDSA"] || system.modules["/ti/drivers/ECIES"]
        || system.modules["/ti/drivers/ECDH"]) {
        /* Add dependency on ECC library for CC13x1/CC26x1 and CC23x0 */
        if (family.match(/cc(13|26).1/) || family.match(/cc23.0/)) {
            linkOpts.deps.push("/third_party/ecc");
        }
    }

    if (system.modules["/ti/drivers/AESCCM"] ||
        system.modules["/ti/drivers/AESCMAC"] ||
        system.modules["/ti/drivers/SHA2"] ||
        system.modules["/ti/drivers/AESECB"] ||
        system.modules["/ti/drivers/AESCTR"] ||
        system.modules["/ti/drivers/AESCTRDRBG"] ||
        system.modules["/ti/drivers/AESGCM"] ||
        system.modules["/ti/drivers/AESCBC"] ||
        system.modules["/ti/drivers/ECDH"] ||
        system.modules["/ti/drivers/ECDSA"] ||
        system.modules["/ti/drivers/EDDSA"] ||
        system.modules["/ti/drivers/TRNG"] ||
        system.modules["/ti/drivers/RNG"] ||
        system.modules["/ti/drivers/CryptoKeyKeyStore_PSA"]) {
        /* Add dependency on HSMDDK library for CC27XX and CC35XX */
        if (family.match(/cc27/) || family.match(/cc35/)) {
            linkOpts.deps.push("/third_party/hsmddk");
        }
    }

    return (linkOpts);
}

let base = {
    displayName         : "CryptoBoard",
    description         : "Secure Drivers dependency linker",
    alwaysShowLongDescription : true,
    longDescription     : `
This is a collection dependency used to indicate to the Board module that crypto drivers are present
and that the secure_drivers library should be loaded.
`,
    defaultInstanceName : "CONFIG_CRYPTO_COMMON_",
    config              : [],
    modules: (inst) => {
        let forcedModules = ["Board"];
        return Common.autoForceModules(forcedModules)();
    },

    templates    : {
        /* Contribute Secure Drivers libraries to linker command file */
        "/ti/utils/build/GenLibs.cmd.xdt"   :
            {modName: "/ti/drivers/CryptoBoard", getLibs: getLibs}
    }

};

exports = base;
