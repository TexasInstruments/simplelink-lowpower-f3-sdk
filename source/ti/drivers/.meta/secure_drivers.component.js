/*
 * Copyright (c) 2025, Texas Instruments Incorporated - https://www.ti.com
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
 *  ======== drivers.component.js ========
 */

"use strict";

let topModules;
let displayName = "TI Security Drivers";
let description = "TI Security Drivers System Configuration";
let deviceId = system.deviceData.deviceId;

if (deviceId.match(/CC13.4|CC26.4|CC2653/)) {
    /* CC13X4 & CC26X4 */
    topModules = [
        {
            displayName: displayName,
            description: description,
            modules: [
                "/ti/drivers/RNG",
                "/ti/drivers/TRNG"
            ],
            "categories": [
                {
                    "displayName": "Crypto Drivers",
                    "description": "Encryption, Decryption and Authentication",
                    "modules": [
                        "/ti/drivers/AESCBC",
                        "/ti/drivers/AESCCM",
                        "/ti/drivers/AESCMAC",
                        "/ti/drivers/AESCTR",
                        "/ti/drivers/AESCTRDRBG",
                        "/ti/drivers/AESECB",
                        "/ti/drivers/AESGCM",
                        "/ti/drivers/ANSIX936KDF",
                        //"/ti/drivers/CryptoKey", // unused - no configuration currently required for CC26X4 family
                        "/ti/drivers/ECDH",
                        "/ti/drivers/ECDSA",
                        "/ti/drivers/ECJPAKE",
                        "/ti/drivers/EDDSA",
                        "/ti/drivers/SHA2"
                    ]
                }
            ]
        }
    ];
} else if (deviceId.match(/CC13.2|CC26.2/)) {
    /* CC13X2 & CC26X2 */
    topModules = [
        {
            displayName: displayName,
            description: description,
            modules: [
                "/ti/drivers/RNG",
                "/ti/drivers/TRNG"
            ],
            "categories": [
                {
                    "displayName": "Crypto Drivers",
                    "description": "Encryption, Decryption and Authentication",
                    "modules": [
                        "/ti/drivers/AESCBC",
                        "/ti/drivers/AESCCM",
                        "/ti/drivers/AESCMAC",
                        "/ti/drivers/AESCTR",
                        "/ti/drivers/AESCTRDRBG",
                        "/ti/drivers/AESECB",
                        "/ti/drivers/AESGCM",
                        "/ti/drivers/ANSIX936KDF",
                        "/ti/drivers/ECDH",
                        "/ti/drivers/ECDSA",
                        "/ti/drivers/ECJPAKE",
                        "/ti/drivers/EDDSA",
                        "/ti/drivers/SHA2"
                    ]
                }
            ]
        }
    ];
} else if (deviceId.match(/CC13.1|CC26.1/)) {
    /* CC13X1 & CC26X1 */
    topModules = [
        {
            displayName: displayName,
            description: description,
            modules: [
                "/ti/drivers/RNG",
                "/ti/drivers/TRNG"
            ],
            "categories": [
                {
                    "displayName": "Crypto Drivers",
                    "description": "Encryption, Decryption and Authentication",
                    "modules": [
                        "/ti/drivers/AESCBC",
                        "/ti/drivers/AESCCM",
                        "/ti/drivers/AESCMAC",
                        "/ti/drivers/AESCTR",
                        "/ti/drivers/AESCTRDRBG",
                        "/ti/drivers/AESECB",
                        "/ti/drivers/ANSIX936KDF",
                        "/ti/drivers/ECDH",
                        "/ti/drivers/ECDSA",
                        "/ti/drivers/SHA2"
                    ]
                }
            ]
        }
    ];
} else if (deviceId.match(/CC23.0/)) {
    /* CC23X0 */
    topModules = [
        {
            displayName: displayName,
            description: description,
            modules: [
                "/ti/drivers/RNG"
            ],
            "categories": [
                {
                    "displayName": "Crypto Drivers",
                    "description": "Encryption, Decryption and Authentication",
                    "modules": [
                        "/ti/drivers/AESCBC",
                        "/ti/drivers/AESCCM",
                        "/ti/drivers/AESCMAC",
                        "/ti/drivers/AESCTR",
                        "/ti/drivers/AESCTRDRBG",
                        "/ti/drivers/AESECB",
                        "/ti/drivers/ANSIX936KDF",
                        "/ti/drivers/ECDH",
                        "/ti/drivers/ECDSA",
                        "/ti/drivers/ECIES",
                        "/ti/drivers/SHA2"
                    ]
                }
            ]
        }
    ];
} else if (deviceId.match(/CC27/)) {
    /* CC27XX */
    topModules = [
        {
            displayName: displayName,
            description: description,
            modules: [
            ],
            "categories": [
                {
                    "displayName": "Crypto Drivers",
                    "description": "Encryption, Decryption and Authentication",
                    "modules": [
                        "/ti/drivers/AESCBC",
                        "/ti/drivers/AESCCM",
                        "/ti/drivers/AESCMAC",
                        "/ti/drivers/AESCTR",
                        "/ti/drivers/AESECB",
                        "/ti/drivers/AESGCM",
                        "/ti/drivers/CryptoKeyKeyStore_PSA",
                        "/ti/drivers/ECDH",
                        "/ti/drivers/ECDSA",
                        "/ti/drivers/EDDSA",
                        "/ti/drivers/SHA2",
                        "/ti/drivers/RNG",
                        "/ti/drivers/TRNG"
                    ]
                }
            ]
        }
    ];
} else if (deviceId.match(/CC35.*/)) {
    /* CC35XX */
    topModules = [
        {
            displayName: displayName,
            description: description,
            modules: [
            ],
            "categories": [
                {
                    "displayName": "Crypto Drivers",
                    "description": "Encryption, Decryption and Authentication",
                    "modules": [
                        "/ti/drivers/AESCBC",
                        "/ti/drivers/AESCCM",
                        "/ti/drivers/AESCMAC",
                        "/ti/drivers/AESCTR",
                        "/ti/drivers/AESECB",
                        "/ti/drivers/AESGCM",
                        "/ti/drivers/CryptoKeyKeyStore_PSA",
                        "/ti/drivers/ECDSA",
                        "/ti/drivers/RNG",
                        "/ti/drivers/SHA2",
                        "/ti/drivers/TRNG"
                    ]
                }
            ]
        }
    ];
}

let templates = [];

exports = {
    displayName: displayName,
    topModules: topModules,
    templates: templates
};
