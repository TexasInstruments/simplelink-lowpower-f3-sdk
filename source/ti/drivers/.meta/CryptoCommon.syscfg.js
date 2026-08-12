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
 *  ======== CryptoCommon.syscfg.js ========
 */

exports = {
    device2Family: device2Family,  /* get /ti/drivers device family name */

    init: init
};

/*
 *  ======== init ========
 */
function init()
{
}

/*!
 *  ======== device2Family ========
 *  Map a pimux device object to a TI-driver device family string
 *
 *  @param device   - a pinmux device object (system.deviceData)
 *  @param mod      - module name ("ADC", "PWM", etc)
 *
 *  @returns String - the name of a device family that's used to
 *                    create family specifc-implementation module
 *                    names.  Returns null, in the event of an
 *                    unsupported device.
 */
function device2Family(device, mod)
{
    /* device.deviceId prefix -> /ti/drivers family name */
    let DEV2FAMILY = [
        {prefix: "CC13.4",   family: "CC26X4"},
        {prefix: "CC26.4",   family: "CC26X4"},
        {prefix: "CC2653",   family: "CC26X4"},
        {prefix: "CC13.2",   family: "CC26X2"},
        {prefix: "CC26.2",   family: "CC26X2"},
        {prefix: "CC13.1",   family: "CC26X1"},
        {prefix: "CC26.1",   family: "CC26X1"},
        {prefix: "CC13",     family: "CC26XX"},
        {prefix: "CC26",     family: "CC26XX"},
        {prefix: "CC23.0",   family: "CC23X0"},
        {prefix: "CC27",     family: "CC27XX"},
        {prefix: "CC35",     family: "CC35XX"}
    ];

    /* CC26X4 specific module delegates */
    let cc26x4Mods = {
        "ECDH" :           "CC26X2",
        "ECDSA" :          "CC26X2",
        "ECJPAKE" :        "CC26X2",
        "EDDSA" :          "CC26X2",
        "SHA2" :           "CC26X2",
        "AESCCM" :         "CC26X4",
        "AESGCM" :         "CC26X4"
    };

    /* CC26X2 and CC26X2R7 specific module delegates */
    let cc26x2Mods = {
        "ECDH" :        "CC26X2",
        "ECDSA" :       "CC26X2",
        "ECJPAKE" :     "CC26X2",
        "EDDSA" :       "CC26X2",
        "SHA2" :        "CC26X2"
    };

    /* CC26X1 specific module delegates */
    let cc26x1Mods = {
        "ECDH" :        "CC26X1",
        "ECDSA" :       "CC26X1",
        "SHA2" :        "CC26X1"
    };

    /* CC23X0 specific module delegates
     * Note, the default family name returned below is LPF3, so this list must
     * contain all CC23X0 specific modules
     */
    let cc23x0Mods = {
        "Board" :        "CC23X0",
        "CAN" :          "CC23X0",
        "SHA2" :         "LPF3SW",
        "ECDH" :         "LPF3SW",
        "ECDSA" :        "LPF3SW",
        "RNG"  :         "LPF3RF"
    };

    /* CC27XX specific module delegates
     * Note, the default family name returned below is LPF3, so this list must
     * contain all CC27XX specific modules
     */
    let cc27xxMods = {
        "Board" :          "CC27XX",
        "CAN" :            "CC27XX",
        "ECDH" :           "LPF3HSM",
        "ECDSA" :          "LPF3HSM",
        "EDDSA":           "LPF3HSM",
        "SHA2" :           "LPF3HSM",
        "TRNG":            "LPF3HSM",
        "RNG"  :           "LPF3HSM",
        "AESGCM":          "LPF3HSM",
        "CryptoKeyKeyStore_PSA" : "CC27XX"
    };

    /* CC35XX specific module delegates
     * Note, the default family name returned below is WFF3, so this list must
     * contain all CC35XX specific modules
     */
    let cc35xxMods = {
        "AESGCM":          "LPF3HSM",
        "AESECB":          "LPF3",
        "AESCBC":          "LPF3",
        "AESCCM":          "LPF3",
        "AESCMAC":         "LPF3",
        "AESCTR":          "LPF3",
        "CryptoKeyKeyStore_PSA": "CC35XX",
        "ECDH":            "LPF3HSM",
        "ECDSA" :          "LPF3HSM",
        "RNG"  :           "LPF3HSM",
        "SHA2":            "LPF3HSM",
        "TRNG":            "LPF3HSM"
    };

    /* deviceId is the directory name within the pinmux/deviceData */
    let deviceId = device.deviceId;

    for (let i = 0; i < DEV2FAMILY.length; i++) {
        let d2f = DEV2FAMILY[i];

        if (deviceId.match(d2f.prefix)) {
            /* trap device specific mods */
            if (d2f.family == "CC26X4") {
                if (mod in cc26x4Mods) {
                    return (cc26x4Mods[mod]);
                }
                else {
                    return ("CC26XX");
                }
            }
            else if (d2f.family == "CC26X2") {
                if (mod in cc26x2Mods) {
                    return (cc26x2Mods[mod]);
                }
                else {
                    return ("CC26XX");
                }
            }
            else if (d2f.family == "CC26X1") {
                if (mod in cc26x1Mods) {
                    return (cc26x1Mods[mod]);
                }
                else {
                    return ("CC26XX");
                }
            }
            else if (d2f.prefix == "CC23.0") {
                if (mod in cc23x0Mods) {
                    return (cc23x0Mods[mod]);
                }
                else {
                    return ("LPF3");
                }
            }
            else if (d2f.prefix == "CC27") {
                if (mod in cc27xxMods) {
                    return (cc27xxMods[mod]);
                }
                else {
                    return ("LPF3");
                }
            }
            else if (d2f.prefix == "CC35") {
                if (mod in cc35xxMods) {
                    return (cc35xxMods[mod]);
                }
                else {
                    return ("WFF3");
                }
            }
            else {
                return (d2f.family);
            }
        }
    }
    return (null);
}
