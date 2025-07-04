/*
 * Copyright (c) 2024-2025 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== CryptoKeyKeyStore_PSACC27XX.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/* Amount of bytes necessary to store a key's metadata in KeyStore. Keys of
 * every type require a slot, each with a constant size.
 */
const slotMetadataSize = 83;

/* The largest key that can be stored is an encrypted AES256 key.
 * The breakdown is as follows: For each key, there is 32 bytes of data.
 * Each key is wrapped separately, due to an HSM limitation involving asset policy.
 * This adds 16 bytes to each key. Furthermore, each of the two keys requires 32 bytes
 * of memory allocation overhead. (32 + 16 + 32) * 2 = 160 bytes.
 */
const keyItemSizeMaxDefault = 160;

/* volatileMemoryPoolSize defines the size of the memory pool used for an internal memory
 * allocator in KeyStore. This is primarily for key material, but there are some intermediate
 * use cases of this volatile memory. These extra overhead bytes are to ensure there is enough
 * space in the pool for the key material and the intermediate operations. See
 * psa_persistent_key_storage_format in
 * source\third_party\hsmddk\include\Integration\Adapter_PSA\Adapter_mbedTLS\src\psa_crypto_storage.c
 * for details.
 */
const constantOverheadRam = 40;

const defaultAssetStoreSlots = 3;

const defaultPersistentSlots = 3;

const defaultVolatileSlots = 0;

let defaultTotalSlots = defaultAssetStoreSlots + defaultPersistentSlots + defaultVolatileSlots;

/* 1 is added to defaultPersistentSlots to account for the temporary memory used
 * when importing a persistent key.
 */
let defaultRamUsage = (defaultTotalSlots * slotMetadataSize) + constantOverheadRam +
                      ((keyItemSizeMaxDefault) * (defaultPersistentSlots + 1));

/* Even if no RAM is used for volatile keys in the default configuration, there must still be some
 * allocated to allow for imports of persistent keys that still make use of KeyStore's internal
 * memory allocator. By default, there must be RAM available for caching defaultPersistentSlots number
 * of persistent keys, plus some for overhead and temporary allocations.
 */
let defaultVolatileMemoryPoolSize = (keyItemSizeMaxDefault * (defaultPersistentSlots + 1)) + constantOverheadRam;

/* The useable flash size on 1M CC27XX devices is 0xE8000. The default
 * KeyStore flash size is 8KB, or 0x2000 bytes. The default placement
 * of KeyStore flash is at the end of flash, the final 8KB starting at
 * 0xE6000.
 */
const defaultFlashOffset = 0xE6000;
const sectorSize = 0x800;
const baseAddress = 0x00000000;

/*
 *  ======== getLibs ========
 *  Argument to the /ti/utils/build/GenLibs.cmd.xdt template
 */
function getLibs(mod)
{
    /* Get device information from GenLibs */
    let GenLibs = system.getScript("/ti/utils/build/GenLibs");

    let libGroup = {
        name: "/third_party/hsmddk",
        deps: [],
        libs: [],
        allowDuplicates: true
    };

    if (!system.modules["/ti/utils/TrustZone"]) {
        libGroup.libs.push(GenLibs.libPath("third_party/hsmddk", "hsmddk_cc27xx_its.a"));
    }

    return (libGroup);
}

let aesConfig = {
    displayName : "Volatile AES Key Configuration",
    name: "aesConfig",
    config: [
        {
            name          : "aes128PlaintextNumVolatile",
            displayName   : "AES-128 Plaintext Keys",
            description   : "This is the number of AES-128 keys planned to be used and stored in plaintext in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "aes192PlaintextNumVolatile",
            displayName   : "AES-192 Plaintext Keys",
            description   : "This is the number of AES-192 keys planned to be used and stored in plaintext in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "aes256PlaintextNumVolatile",
            displayName   : "AES-256 Plaintext Keys",
            description   : "This is the number of AES-256 keys planned to be used and stored in plaintext in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "aes128EncryptedNumVolatile",
            displayName   : "AES-128 Encrypted Keys",
            description   : "This is the number of AES-128 keys planned to be used and stored in encrypted format in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "aes192EncryptedNumVolatile",
            displayName   : "AES-192 Encrypted Keys",
            description   : "This is the number of AES-192 keys planned to be used and stored in encrypted format in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "aes256EncryptedNumVolatile",
            displayName   : "AES-256 Encrypted Keys",
            description   : "This is the number of AES-256 keys planned to be used and stored in encrypted format in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        }
    ]
};

let eccConfig = {
    displayName : "Volatile ECC Key Configuration",
    config: [
        {
            name          : "ecc224PlaintextNumVolatile",
            displayName   : "ECC-224 Plaintext Keys",
            description   : "This is the number of ECC-224 keys planned to be used and stored in plaintext in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "ecc256PlaintextNumVolatile",
            displayName   : "ECC-256 Plaintext Keys",
            description   : "This is the number of ECC-256 keys planned to be used and stored in plaintext in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "ecc384PlaintextNumVolatile",
            displayName   : "ECC-384 Plaintext Keys",
            description   : "This is the number of ECC-384 keys planned to be used and stored in plaintext in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "ecc512PlaintextNumVolatile",
            displayName   : "ECC-512 Plaintext Keys",
            description   : "This is the number of ECC-512 keys planned to be used and stored in plaintext in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "ecc521PlaintextNumVolatile",
            displayName   : "ECC-521 Plaintext Keys",
            description   : "This is the number of ECC-521 keys planned to be used and stored in plaintext in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "ecc224EncryptedNumVolatile",
            displayName   : "ECC-224 Encrypted Keys",
            description   : "This is the number of ECC-224 keys planned to be used and stored in encrypted format in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "ecc256EncryptedNumVolatile",
            displayName   : "ECC-256 Encrypted Keys",
            description   : "This is the number of ECC-256 keys planned to be used and stored in encrypted format in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "ecc384EncryptedNumVolatile",
            displayName   : "ECC-384 Encrypted Keys",
            description   : "This is the number of ECC-384 keys planned to be used and stored in encrypted format in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "ecc512EncryptedNumVolatile",
            displayName   : "ECC-512 Encrypted Keys",
            description   : "This is the number of ECC-512 keys planned to be used and stored in encrypted format in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        },
        {
            name          : "ecc521EncryptedNumVolatile",
            displayName   : "ECC-521 Encrypted Keys",
            description   : "This is the number of ECC-521 keys planned to be used and stored in encrypted format in Key Store RAM.",
            default       : 0,
            displayFormat : "dec",
            onChange      : onChangeUpdateSlotCountAndSize
        }
    ]
};

let keyConfig = [aesConfig, eccConfig];

let configBase = [
    {
        name          : "totalSlotCount",
        displayName   : "Total Key Slots",
        description   : "The total number of keys (of any lifetime) that can be placed in KeyStore. "
                        + "Each key slot, regardless of key type, occupies 83 bytes of data for "
                        + "metadata alone.",
        default       : defaultTotalSlots,
        displayFormat : "dec",
        hidden        : true
    },
    {
        name          : "assetStoreSlotCount",
        displayName   : "Asset Store Key Slots",
        description   : "Sets the number of keys that can be actively stored in the HSM Asset Store. "
                        + "There is a limit of 5, since some space in HSM dynamic RAM must be reserved "
                        + "for crypto drivers to make use of.",
        default       : defaultAssetStoreSlots,
        options       : [
            { name: 1 },
            { name: 2 },
            { name: 3 },
            { name: 4 },
            { name: 5 }
        ],
        onChange      : onChangeUpdateSlotCountAndSize,
        displayFormat : "dec"
    },
    {
        name          : "persistentKeyCacheSlots",
        displayName   : "Persistent Key Cache Slots",
        description   : "Pre-define a non-configurable value for the number of slots that will "
                        + "be reserved to cache keys that also get stored in NVM.",
        default       : defaultPersistentSlots,
        hidden        : true,
        readOnly      : true,
        displayFormat : "dec"
    },
    {
        name          : "volatileSlotCount",
        displayName   : "Volatile Key Slots",
        description   : "The number of volatile keys that can be stored in KeyStore at a given time. "
                        + "This is determined by the number of volatile keys that are selected for usage.",
        default       : defaultVolatileSlots,
        onChange      : onChangeUpdateSlotCountAndSize,
        displayFormat : "dec",
        hidden        : true
    },
    {
        name          : "volatileMemoryPoolSize",
        displayName   : "Volatile Key Material Storage Space",
        description   : "This is the amount of RAM space that will be necessary to hold the specified "
                        + "number of volatile keys of the given sizes, as well as the space required for "
                        + "caching 3 persistent keys, in KeyStore's memory allocator pool.",
        default       : defaultVolatileMemoryPoolSize,
        displayFormat : "dec",
        hidden        : true
    },
    {
        name              : "flashSizeConfig",
        displayName       : "KeyStore Flash Size",
        description       : "Amount of flash space occupied by persistent keys and their metadata.",
        longDescription   : "This is the amount of flash space to allocate to KeyStore flash. Each "
                          + "size has a maximum number of keys associated with it - regardless of key size. "
                          + "The value is based off the largest supported key type, an ECC-521 key.",
        default           : "8KB (23 Persistent Keys Max)",
        options: [
            { name: "4KB (11 Persistent Keys Max)" },
            { name: "8KB (23 Persistent Keys Max)" },
            { name: "10KB (35 Persistent Keys Max)" }
        ],
        onChange          : onChangeUpdateFlash,
        hidden            : false
    },
    {
        name            : "ramUsage",
        displayName     : "KeyStore RAM Usage (bytes)",
        description     : "RAM space required for all keys and key metadata.",
        longDescription : "This is the amount of RAM space that will be necessary to hold the specified "
                        + "number of keys of the given sizes & types, as well as the space required for "
                        + "caching 3 persistent keys and storing all key metadata.",
        default         : defaultRamUsage, /* Default value for 3 asset store key slots and 3 persistent key slots */
        displayFormat   : "dec",
        hidden          : false,
        readOnly        : true
    },
    {
        name              : "flashSize",
        displayName       : "KeyStore Flash Size Value",
        description       : "Amount of flash space occupied by persistent keys and their metadata.",
        default           : 0x2000,
        displayFormat     : "hex",
        hidden            : true
    },
    {
        name              : "flashRegionType",
        displayName       : "KeyStore Flash Region Type",
        description       : "Specifies the type of flash region is used with regards to the linker command file.",
        longDescription:`
Specifies the type of region used.

* __Generated__ - An internal flash region is automatically generated.
* __Pointer__ - Provide a pointer to the memory location of a
predefined internal flash region.
`,
        default      : "Pointer",
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
        ],
        hidden            : false
    },
    {
        name              : "flashOffset",
        displayName       : "KeyStore Flash Address Offset",
        description       : "KeyStore flash region offset, from this device's application-accessible base flash address.",
        longDescription   : "This is the offset from the base application-accessible flash address to the "
                          + "start of the KeyStore flash region. For example, if the base address of the "
                          + "application-accessible flash is 0x0021A000, an offset of 0x1000 would place "
                          + "the start of the KeyStore flash region at 0x0021B000. When changing KeyStore "
                          + "flash address offset, be sure that the specified flash region will fit in the "
                          + "device's available application flash and will not collide with vector table.",
        default           : defaultFlashOffset,
        onChange          : onChangeUpdateFlash,
        displayFormat     : "hex",
        hidden            : false
    },
    {
        name              : "flashAddress",
        displayName       : "KeyStore Flash Address",
        description       : "KeyStore flash region address, calculated from offset and base address.",
        default           : defaultFlashOffset, /* Since CC27XX's base address is 0, the default offset and address are the same */
        displayFormat     : "hex",
        hidden            : true
    },
    {
        name          : "persistentNumKeys",
        displayName   : "Number of Persistent Keys",
        description   : "This is the number of keys that can be stored in KeyStore flash at a given time.",
        default       : 23,
        displayFormat : "dec",
        hidden        : true
    }
];

/*
 *  ======== updateKeySize ========
 *  Update RAM usage based on total key slot count and number of volatile keys
 */
function updateKeySize(inst, ui)
{
    let volatileSize = 0;

    for (const keyCategory of keyConfig)
    {
        for (const keyType of keyCategory.config)
        {
            let name = keyType.name;

            /* If none of this key type are selected for use, we do not need to calculate the key
             * size for it.
             */
            if (inst[name] == 0)
            {
                continue;
            }

            /* All key type variables start with 'ecc' or 'aes', followed by
             * 3 characters specifying the key size in bits. For example,
             * aes128PlaintextNumVolatile has '128' in indices 3-5 of the string.
             */
            let keySize = parseInt(name.substring(3, 6));

            /* Calculate bytes needed from key size in bits */
            keySize = Math.ceil(keySize / 8);

            /* Add 32 bytes of overhead for storing any item in KeyStore memory allocation region.
             * Note that this is the same value for persistent key storage format.
             */
            keySize += 32;

            if (name.includes("Encrypted"))
            {
                /* Add 16 bytes for encrypted key wrapping overhead */
                keySize += 16;

                if (name.includes("aes"))
                {
                    /* Multiply the size of the key by two, since we must store 2 encrypted versions
                     * of the same key for it to be used symmetrically. This is an implementation detail
                     * having to do with HSM Asset Policies.
                     */
                    keySize *= 2;
                }
            }

            if (name.includes("Volatile"))
            {
                volatileSize += (inst[name] * keySize);
            }
        }
    }

    /* Number of bytes required to store the slots themselves, with their metadata */
    let metadataOverheadRAM = inst.totalSlotCount * slotMetadataSize;

    /* Account for the pre-configured number of persistent keys that will be also stored in
     * the cache, assuming any persistent keys have been selected for use. Add 32 header bytes
     * that are required for every key that is allocated to the KeyStore RAM buffer.
     */
    let persistentKeysRAM = (inst.persistentNumKeys > 0) ? (keyItemSizeMaxDefault) * inst.persistentKeyCacheSlots : 0;

    /* Technically, metadataOverheadRAM is only necessary to show RAM usage to customer -
     * it does not actually need to be a part of the define that is used for the memory
     * pool size. keyItemSizeMaxDefault is added to account for the temporary memory used
     * when importing a persistent key.
     */
    inst.volatileMemoryPoolSize = volatileSize + persistentKeysRAM + constantOverheadRam + keyItemSizeMaxDefault;

    /* Customer-facing value about how much RAM KeyStore will require between selected keys + overhead */
    ui.ramUsage.readOnly = false;
    inst.ramUsage = inst.volatileMemoryPoolSize + metadataOverheadRAM;
    ui.ramUsage.readOnly = true;
}

/*
 *  ======== onChangeUpdateSlotCountAndSize ========
 *  Update total slot count and memory sizes after adjustment to their component values
 */
function onChangeUpdateSlotCountAndSize(inst, ui)
{
    /* Update volatile slot count */
    let count = 0;
    for (const keyCategory of keyConfig)
    {
        for (const keyType of keyCategory.config)
        {
            let name = keyType.name;
            if (name.includes("Volatile")) {
                count += inst[name];
            }
        }
    }

    inst.volatileSlotCount = count;

    /* persistentKeyCacheSlots is a pre-defined value for the number of persistent key slots. It
     * does not need to be configurable, as persistent keys get stored to flash regardless of slot
     * availability.
     */
    inst.totalSlotCount = (inst.volatileSlotCount + inst.assetStoreSlotCount + inst.persistentKeyCacheSlots);

    updateKeySize(inst, ui);
}

/*
 *  ======== onChangeUpdateFlash ========
 *  Update number of persistent keys and flash size based on flash size configuration
 */
function onChangeUpdateFlash(inst)
{
    if (inst.flashSizeConfig == "4KB (11 Persistent Keys Max)")
    {
        inst.persistentNumKeys = 11;
        inst.flashSize = 0x1000;
    }
    else if (inst.flashSizeConfig == "8KB (23 Persistent Keys Max)")
    {
        inst.persistentNumKeys = 23;
        inst.flashSize = 0x2000;
    }
    else
    {
        /* inst.flashSizeConfig == "10KB (35 Persistent Keys Max)" */
        inst.persistentNumKeys = 35;
        inst.flashSize = 0x2800;
    }

    /* Update the flash address based on the offset and base address */
    inst.flashAddress = baseAddress + inst.flashOffset;
}

/*
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  @param inst - Instance to be validated
 *  @param validation - Object to hold detected validation issues
 *
 */
function validate(inst, validation)
{
    if (system.modules["/ti/utils/TrustZone"]) {
        validation.logInfo(
            "Note: The KeyStore configuration is fixed when TrustZone is enabled. " +
            "If different settings are needed, modify tfm_s/cc27xx/production_full/config/config_tfm_project.h " +
            "and rebuild the secure image.", inst);
    }

    if (inst.flashOffset % sectorSize) {
        let message = "KeyStore flash offset must be aligned on a " + sectorSize
            + " page boundary.";
        Common.logError(validation, inst, "flashOffset", message);
    }
}

/*
 *  ======== onModuleChanged ========
 *  Changes visibility of KeyStore config depending on the presence of TrustZone module
 */
function onModuleChanged(inst, dependentInst, moduleName, configurables) {
    if (moduleName == "/ti/utils/TrustZone") {
        if (dependentInst !== undefined) {
            /* Hide base config */
            inst.$uiState.assetStoreSlotCount.hidden = true;
            inst.$uiState.flashSizeConfig.hidden = true;
            inst.$uiState.ramUsage.hidden = true;
            inst.$uiState.flashRegionType.hidden = true;
            inst.$uiState.flashOffset.hidden = true;

            /* Hide volatile key configs */
            Object.keys(inst.$uiState).forEach(key => {
                if (key.includes('NumVolatile')) {
                    inst.$uiState[key].hidden = true;
                }
            });
        }
        else {
            /* Display base config */
            inst.$uiState.assetStoreSlotCount.hidden = false;
            inst.$uiState.flashSizeConfig.hidden = false;
            inst.$uiState.ramUsage.hidden = false;
            inst.$uiState.flashRegionType.hidden = false;
            inst.$uiState.flashOffset.hidden = false;

            /* Display volatile key configs */
            Object.keys(inst.$uiState).forEach(key => {
                if (key.includes('NumVolatile')) {
                    inst.$uiState[key].hidden = false;
                }
            });
        }
    }
}

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base CryptoKeyKeyStore_PSA configuration
 */
let devSpecific = {
    moduleStatic: {
        config   : configBase.concat(keyConfig),
        validate : validate,
        /* AES drivers which support LAES require DMA module and an RNG module
         * is required since it defines global return behavior variable.
         */
        modules  : Common.autoForceModules(["Board", "Power", "CryptoBoard", "DMA", "RNG"]),
        dependencies: {
            modules: {
                "/ti/utils/TrustZone" : ["secureImage"]
            },
            onModuleChanged: onModuleChanged
        }
    },
    templates : {
        boardc: "/ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSACC27XX.Board.c.xdt",
        boardh: "/ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA.Board.h.xdt",

        /* Contribute libraries to linker command file */
        "/ti/utils/build/GenLibs.cmd.xdt":
                {modName: "/ti/drivers/CryptoKeyKeyStore_PSA", getLibs: getLibs}
    }
};

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 */
function extend(base)
{
    /* display which driver implementation can be used */
    base = Common.addImplementationConfig(base, "CryptoKeyKeyStore_PSA", null,
        [{name: "CryptoKeyKeyStore_PSA"}], null);

    return (Object.assign({}, base, devSpecific));
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    validate: validate,
    /* required function, called by base CryptoKeyKeyStore_PSA module */
    extend: extend
};
