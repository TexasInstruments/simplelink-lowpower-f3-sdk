/*
 * Copyright (c) 2025-2026 Texas Instruments Incorporated
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

/* get device ID */
let deviceId = system.deviceData.deviceId;

/*
 *  ======== getLinkerDefs ========
 */
function getLinkerDefs()
{
    /* Get device ID to select appropriate libs */
    let devId = system.deviceData.deviceId;
    let linkerDefs = [];
    var DriverLib = system.getScript("/ti/devices/DriverLib");
    let family = DriverLib.getAttrs(devId).libName;

    /* Create linker definitions for CC27XX and CC23X1, as CC35XX KeyStore location
     * is not yet configurable.
     */
    if (family.match(/CC27/i) || family.match(/CC23.1/i)) {
        let keystoreModule = system.modules["/ti/drivers/CryptoKeyKeyStore_PSA"];
        if (keystoreModule) {
            let keystoreInst = keystoreModule.$static;
            linkerDefs.push(
                {
                    "name": "KEYSTORE_BASE",
                    "value": keystoreInst.flashAddress
                },
                {
                    "name": "KEYSTORE_SIZE",
                    "value": keystoreInst.flashSize
                }
            );
        }
        else
        {
            linkerDefs.push(
                {
                    "name": "KEYSTORE_BASE",
                    "value": ""
                },
            );
        }
    }

    return linkerDefs;
}

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

    var mbedtls_crypto_suffix = "";
    if (family.match(/cc27/) || family.match(/cc35/) || family.match(/cc23.1/)) {
        if (system.modules["/ti/drivers/CryptoKeyKeyStore_PSA"]) {
            let keystoreModule = system.modules["/ti/drivers/CryptoKeyKeyStore_PSA"];
            let inst = keystoreModule.$static;

            if (inst.useSWCrypto) {
                if (!system.modules["/ti/utils/TrustZone"]) {
                    /* If the CryptoKeyKeyStore_PSA module is present, but TrustZone is not,
                    * then we must check which secure_drivers library to use. There is one
                    * that will make use of SW Crypto from mbedTLS, and one that will not.
                    * When TrustZone is enabled, there is only a single secure_drivers library
                    * because SW Crypto from mbedTLS is always unsupported in such applications.
                    */
                    mbedtls_crypto_suffix = "_mbedtls";
                }
            }
        }
    }

    /* Note that mbedtls_crypto_suffix will always be an empty string for a TFM-enabled build. */
    var lib_suffix = mbedtls_crypto_suffix + log_suffix;

    if (family != "") {
        /* secure_drivers must be ahead of regular drivers to satisfy the dependency */
        if (system.modules["/ti/drivers/CryptoBoard"]) {
            if (system.modules["/ti/utils/TrustZone"]) {
                libs.push(libPath("ti/drivers/secure","secure_drivers_" + family + "_ns" + lib_suffix +".a"));
            }
            else {
                libs.push(libPath("ti/drivers/secure","secure_drivers_" + family + lib_suffix +".a"));
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

    if (system.modules["/ti/drivers/CryptoBoard"]) {
        /* Add dependency on HSMDDK library for CC27XX, CC35XX and CC23X1 */
        if (family.match(/cc27/) || family.match(/cc35/) || family.match(/cc23.1/)) {
            linkOpts.deps.push("/third_party/hsmddk");
        }
    }

    /* For HSM-enabled devices, determine which mbedTLS library to use for PSA implementation. */
    if (family.match(/cc27/) || family.match(/cc35/) || family.match(/cc23.1/)) {
        /* Add PSA implementation only if PSA/KeyStore module is present */
        if (system.modules["/ti/drivers/CryptoKeyKeyStore_PSA"]){
            let keystoreModule = system.modules["/ti/drivers/CryptoKeyKeyStore_PSA"];
            let inst = keystoreModule.$static;
            /* If SW Crypto is enabled, then add the mbedTLS library. Note that
             * SW Crypto cannot be enabled when TrustZone is enabled.
             */
            if (inst.useSWCrypto && !system.modules["/ti/utils/TrustZone"]) {
                libs.push(libPath("third_party/mbedtls/ti", "mbedtls.a"));
            }
        }
    }
    else {
        /* For non-HSM devices with the KeyStore module, add dependency on PSA Crypto library */
        if (system.modules["ti/drivers/CryptoKeyKeyStore_PSA"]) {
            libs.push(libPath("third_party/psa_crypto", "psa_crypto_" + family + ".a"));
        }
    }

    return (linkOpts);
}

/*
 *  ======== addStaticModules ========
 */
function addStaticModules(inst)
{
    let forcedModules = [];
    if (deviceId.match(/CC27/) || deviceId.match(/CC23/)) {
        /* LAES engine requires AES CommonXXF3 module */
        forcedModules.push(["cryptoutils/aes/AESCommonXXF3"]);
    }
    return Common.autoForceModules(forcedModules)();
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

    moduleStatic: {
        modules: addStaticModules
    },

    templates    : {
        /* Contribute Secure Drivers libraries to linker command file */
        "/ti/utils/build/GenLibs.cmd.xdt"   :
            {modName: "/ti/drivers/CryptoBoard", getLibs: getLibs},

        /* Contribute Secure Drivers definitions to linker command file */
        "/ti/utils/build/GenMap.cmd.xdt"   :
            {modName: "/ti/drivers/CryptoBoard", getLinkerDefs: getLinkerDefs}
    }

};

exports = base;
