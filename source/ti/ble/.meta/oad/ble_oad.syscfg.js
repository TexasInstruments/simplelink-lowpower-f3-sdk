/*
 * Copyright (c) 2024 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble_oad.syscfg.js ========
 */

"use strict";

// Get OAD long descriptions
const Docs = system.getScript("/ti/ble/oad/ble_oad_docs.js");

// MS-OAD layout utility (device constants + computation)
const MsOadLayout = system.getScript("/ti/ble/oad/ms_oad_layout.js");

// Dual Image OAD layout utility (device constants)
const DualImageLayout = system.getScript("/ti/ble/oad/dual_image_oad_layout.js");

// Persistent OAD layout utility (device constants)
const PersistentOadLayout = system.getScript("/ti/ble/oad/persistent_oad_layout.js");

// On-Chip OAD layout utility (device constants)
const OnchipOadLayout = system.getScript("/ti/ble/oad/onchip_oad_layout.js");

// Off-Chip OAD layout utility (device constants)
const OffchipOadLayout = system.getScript("/ti/ble/oad/offchip_oad_layout.js");

// Per-device defaults for MS-OAD configurable fields
const _defaultMsOadInputs = MsOadLayout.getDeviceInputs(system.deviceData.deviceId);

// Per-device defaults for Dual Image OAD configurable fields
const _defaultDualImageLayout = DualImageLayout.getDualImageLayout(system.deviceData.deviceId);

// Per-device defaults for Persistent OAD configurable fields
const _defaultPersistentLayout = PersistentOadLayout.getPersistentOadLayout(system.deviceData.deviceId);

// Per-device defaults for On-Chip OAD configurable fields
const _defaultOnchipLayout = OnchipOadLayout.getOnchipOadLayout(system.deviceData.deviceId);

// Per-device defaults for Off-Chip OAD configurable fields
const _defaultOffchipLayout = OffchipOadLayout.getOffchipOadLayout(system.deviceData.deviceId);

/*
 * Per-method OAD support is derived from each layout file's device table.
 * A device is supported for a given OAD method if and only if its deviceId
 * matches an entry in that method's layout.  This keeps the layout files as
 * the single source of truth for device support.
 *
 * Secure-boot devices (CC27xx) are not covered here — their OAD compiler/linker
 * flags must be set directly in the ExampleList / project build settings.
 */
const isMsOadSupported         = (_defaultMsOadInputs    !== null);
const isDualImageOadSupported  = (_defaultDualImageLayout !== null);
const isPersistentOadSupported = (_defaultPersistentLayout !== null);
const isOnchipOadSupported     = (_defaultOnchipLayout   !== null);
const isOffchipOadSupported    = (_defaultOffchipLayout  !== null);

const _isOadSupported = isMsOadSupported || isDualImageOadSupported
                     || isPersistentOadSupported || isOnchipOadSupported
                     || isOffchipOadSupported;

const config = {
    name: "oadConfig",
    displayName: "OAD Configuration",
    description: "Configure Over-the-Air Download (OAD) Settings",
    config: [
        {
            name: "oadMethod",
            displayName: "OAD Method",
            description: "Select the OAD method to use",
            longDescription: Docs.oadMethodLongDescription,
            default: "NONE",
            hidden: true,
            onChange: onOadMethodChange,
            options: [
                {
                    displayName: "None",
                    name: "NONE",
                    description: "No Over-the-Air Download method selected"
                },
                ...(isDualImageOadSupported ? [{
                    displayName: "Dual Image OAD",
                    name: "DUAL_IMAGE",
                    description: "Dual image OAD with separate primary and secondary flash slots"
                }] : []),
                ...(isOnchipOadSupported ? [{
                    displayName: "On-Chip OAD",
                    name: "OAD_APP_ONCHIP",
                    description: "On-chip OAD with a single application slot for firmware updates"
                }] : []),
                ...(isOffchipOadSupported ? [{
                    displayName: "Off-Chip OAD",
                    name: "OAD_APP_OFFCHIP",
                    description: "Off-chip OAD using external flash for firmware updates"
                }] : []),
                ...(isMsOadSupported ? [{
                    displayName: "Multistep OAD",
                    name: "MULTISTEP_OAD",
                    description: "Multistep OAD with half-image architecture for reliable updates"
                }] : [])
            ]
        },
        {
            name: "oadAutoAddModuleInstances",
            displayName: "Automatically Add Module Instances",
            description: "When enabled, automatically adds the required module instances " +
                         "(e.g., NVS regions) for the selected OAD method",
            default: true,
            hidden: true,
        },
        {
            name: "halfModeBuild",
            displayName: "Half-Mode Build",
            description: "Enable half-mode build for Multistep OAD",
            longDescription: Docs.halfModeBuildLongDescription,
            default: false,
            hidden: true,
            onChange: onHalfModeBuildChange
        },
        {
            name: "slot1ObjectsList",
            displayName: "Slot 1 Objects List",
            description: "List of object files to place in APP_1_SLOT (one per line)",
            longDescription: Docs.slot1ObjectsListLongDescription,
            multiline: true,
            default: "",
            hidden: true,
            placeholder: "obj1.obj\nobj2.obj\nmain.obj"
        },
        {
            name        : "msOadInitSlotSize",
            displayName : "Init Slot Size",
            description : "Size of the INIT_SLOT region reserved for reset vectors and .cinit",
            longDescription: Docs.initSlotSizeLongDescription,
            default     : 0x700,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "msOadApp1HdrAddr",
            displayName : "App1 Header Address",
            description : "Base address of MCUBoot end / App1 image header in flash",
            default     : _defaultMsOadInputs ? _defaultMsOadInputs.APP_1_HDR_ADDR : 0,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "dualImageAppHdrAddr",
            displayName : "Application Header Address",
            description : "Base address of the primary application image header in flash",
            default     : _defaultDualImageLayout ? _defaultDualImageLayout.APP_HDR_ADDR : 0,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "dualImageAppSlotBase",
            displayName : "OAD Download Slot Base Address",
            description : "Base address of the secondary (OAD download) slot in flash",
            default     : _defaultDualImageLayout ? _defaultDualImageLayout.APP_SLOT_BASE : 0,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "dualImageAppSlotSize",
            displayName : "OAD Download Slot Size",
            description : "Size of the secondary (OAD download) slot in flash",
            default     : _defaultDualImageLayout ? _defaultDualImageLayout.APP_SLOT_SIZE : 0,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "persistentAppSlotBase",
            displayName : "Application Slot Base Address",
            description : "Base address of the application slot in flash (where APP_HDR_ADDR starts)",
            default     : _defaultPersistentLayout ? _defaultPersistentLayout.APP_SLOT_BASE : 0,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "persistentAppSlotSize",
            displayName : "Application Slot Size",
            description : "Size of the application slot in flash",
            default     : _defaultPersistentLayout ? _defaultPersistentLayout.APP_SLOT_SIZE : 0,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "persistentImageSize",
            displayName : "Persistent Image Size",
            description : "Size of the persistent image binary itself, used as --slot-size for imgtool signing",
            default     : _defaultPersistentLayout ? _defaultPersistentLayout.APP_PERSISTENT_SIZE : 0,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "onchipAppSlotBase",
            displayName : "Application Slot Base Address",
            description : "Base address of the on-chip OAD application slot in flash",
            default     : _defaultOnchipLayout ? _defaultOnchipLayout.APP_SLOT_BASE : 0,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "onchipAppSlotSize",
            displayName : "Application Slot Size",
            description : "Size of the on-chip OAD application slot in flash",
            default     : _defaultOnchipLayout ? _defaultOnchipLayout.APP_SLOT_SIZE : 0,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "offchipAppHdrAddr",
            displayName : "Application Header Address",
            description : "Base address of the application image header in flash for off-chip OAD",
            default     : _defaultOffchipLayout ? _defaultOffchipLayout.APP_HDR_ADDR : 0,
            displayFormat: "hex",
            hidden      : true,
        },
        {
            name        : "mcubootSignVersion1",
            displayName : "MCUBoot Signing Version (V1)",
            description : "Image version for the first signed image",
            default     : "1.0.0",
            hidden      : true,
        },
        {
            name        : "mcubootSignVersion2",
            displayName : "MCUBoot Signing Version (V2)",
            description : "Image version for the second signed image. Leave empty to skip V2 signing.",
            default     : "2.0.0",
            hidden      : true,
        },
        {
            name        : "mcubootKeyPath",
            displayName : "MCUBoot Signing Key Path",
            description : "Relative path from SDK root to the MCUBoot signing key",
            default     : "source/third_party/mcuboot/root-ec-p256.pem",
            hidden      : true,
        },
    ]
};

/*
 *  ======== updateOadVisibility ========
 * Apply show/hide rules for all OAD config items based on current inst state.
 * Called by every onChange handler so the logic lives in one place.
 *
 * @param inst  - Module instance
 * @param ui    - The User Interface object
 */
function updateOadVisibility(inst, ui)
{
    const persistentActive  = inst.enableOad && inst.enablePersistentOad;
    const msOadActive       = inst.enableOad && !persistentActive && inst.oadMethod === "MULTISTEP_OAD";
    const dualImageActive   = inst.enableOad && !persistentActive && inst.oadMethod === "DUAL_IMAGE";
    const onchipActive      = inst.enableOad && !persistentActive && inst.oadMethod === "OAD_APP_ONCHIP";
    const offchipActive     = inst.enableOad && !persistentActive && inst.oadMethod === "OAD_APP_OFFCHIP";

    ui.oadMethod.hidden = !inst.enableOad || persistentActive;
    ui.oadAutoAddModuleInstances.hidden = !inst.enableOad;

    // Multistep params
    ui.halfModeBuild.hidden      = true; // For debugging, use: !msOadActive;
    ui.slot1ObjectsList.hidden   = true; // For debugging, use: !msOadActive || inst.halfModeBuild;
    ui.msOadInitSlotSize.hidden  = !msOadActive || system.compiler === "iar";
    ui.msOadApp1HdrAddr.hidden   = !msOadActive;

    // Dual-Image params
    ui.dualImageAppHdrAddr.hidden  = !dualImageActive;
    ui.dualImageAppSlotBase.hidden = !dualImageActive;
    ui.dualImageAppSlotSize.hidden = !dualImageActive;

    // Persistent params
    ui.persistentAppSlotBase.hidden = !persistentActive;
    ui.persistentAppSlotSize.hidden = !persistentActive;
    ui.persistentImageSize.hidden   = !persistentActive;

    // On-Chip params
    ui.onchipAppSlotBase.hidden = !onchipActive;
    ui.onchipAppSlotSize.hidden = !onchipActive;

    // Off-Chip params
    ui.offchipAppHdrAddr.hidden = !offchipActive;

    // MCUBoot signing params (shown for all OAD methods)
    const showSigningFields = dualImageActive || onchipActive || offchipActive || persistentActive || msOadActive;
    ui.mcubootSignVersion1.hidden = !showSigningFields;
    ui.mcubootSignVersion2.hidden = !showSigningFields;
    ui.mcubootKeyPath.hidden      = !showSigningFields;
}

function onOadMethodChange(inst, ui)
{
    updateOadVisibility(inst, ui);

    // Reset address fields to device defaults when a new method is selected
    if (inst.oadMethod === "MULTISTEP_OAD" && _defaultMsOadInputs)
    {
        inst.msOadApp1HdrAddr = _defaultMsOadInputs.APP_1_HDR_ADDR;
        inst.slot1ObjectsList = "";
    }
    else if (inst.oadMethod === "DUAL_IMAGE" && _defaultDualImageLayout)
    {
        inst.dualImageAppHdrAddr  = _defaultDualImageLayout.APP_HDR_ADDR;
        inst.dualImageAppSlotBase = _defaultDualImageLayout.APP_SLOT_BASE;
        inst.dualImageAppSlotSize = _defaultDualImageLayout.APP_SLOT_SIZE;
    }
    else if (inst.oadMethod === "OAD_APP_ONCHIP" && _defaultOnchipLayout)
    {
        inst.onchipAppSlotBase = _defaultOnchipLayout.APP_SLOT_BASE;
        inst.onchipAppSlotSize = _defaultOnchipLayout.APP_SLOT_SIZE;
    }
    else if (inst.oadMethod === "OAD_APP_OFFCHIP" && _defaultOffchipLayout)
    {
        inst.offchipAppHdrAddr = _defaultOffchipLayout.APP_HDR_ADDR;
    }
}
function onHalfModeBuildChange(inst, ui){ updateOadVisibility(inst, ui); }

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - OAD instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    // Early return if oad is disabled
    if (!inst.enableOad)
    {
        return;
    }

    if(inst.oadMethod === "NONE" && !inst.enablePersistentOad)
    {
        validation.logError("An OAD method must be selected when OAD is enabled", inst, "oadMethod");
    }

    if(inst.enablePersistentOad && inst.oadMethod !== "NONE")
    {
        validation.logError(
            "Persistent OAD cannot be used together with another OAD method. " +
            "Set the OAD Method dropdown to None, or disable Persistent OAD.",
            inst, "oadMethod"
        );
    }

    if(inst.oadMethod !== "NONE")
    {
        validation.logInfo(
            "OAD may require pre-build or post-build steps to be configured in your project. " +
            "These commands are auto-generated by SysConfig into ti_ble_oad_prebuild.cfg and " +
            "ti_ble_oad_postbuild.cfg, and executed by the commands runner script defined in " +
            "the project settings.",
            inst, "oadMethod"
        );
    }


    const addressChangedMsg =
        "When changing OAD address parameters, ensure pre-build and post-build " +
        "script parameters are updated accordingly.";

    if(inst.oadMethod === "MULTISTEP_OAD" && _defaultMsOadInputs)
    {
        if(inst.msOadApp1HdrAddr !== _defaultMsOadInputs.APP_1_HDR_ADDR)
        {
            validation.logWarning(addressChangedMsg, inst, "msOadApp1HdrAddr");
        }
    }

    if(inst.oadMethod === "DUAL_IMAGE" && _defaultDualImageLayout)
    {
        if(inst.dualImageAppHdrAddr  !== _defaultDualImageLayout.APP_HDR_ADDR  ||
           inst.dualImageAppSlotBase !== _defaultDualImageLayout.APP_SLOT_BASE ||
           inst.dualImageAppSlotSize !== _defaultDualImageLayout.APP_SLOT_SIZE)
        {
            validation.logWarning(addressChangedMsg, inst, "dualImageAppHdrAddr");
        }
    }

    if(inst.enablePersistentOad && _defaultPersistentLayout)
    {
        if(inst.persistentAppSlotBase !== _defaultPersistentLayout.APP_SLOT_BASE ||
           inst.persistentAppSlotSize !== _defaultPersistentLayout.APP_SLOT_SIZE)
        {
            validation.logWarning(addressChangedMsg, inst, "persistentAppSlotBase");
        }
    }

    if(inst.oadMethod === "OAD_APP_ONCHIP" && _defaultOnchipLayout)
    {
        if(inst.onchipAppSlotBase !== _defaultOnchipLayout.APP_SLOT_BASE ||
           inst.onchipAppSlotSize !== _defaultOnchipLayout.APP_SLOT_SIZE)
        {
            validation.logWarning(addressChangedMsg, inst, "onchipAppSlotBase");
        }
    }

    if(inst.oadMethod === "OAD_APP_OFFCHIP" && _defaultOffchipLayout)
    {
        if(inst.offchipAppHdrAddr !== _defaultOffchipLayout.APP_HDR_ADDR)
        {
            validation.logWarning(addressChangedMsg, inst, "offchipAppHdrAddr");
        }
    }

    // MCUBoot signing field validation
    // imgtool version format: MAJOR.MINOR.REVISION+BUILD_NUM (minor/revision/build optional)
    // Maps to struct image_version { uint8_t major, minor; uint16_t revision; uint32_t build_num; }
    const signingActive = (inst.oadMethod === "DUAL_IMAGE" || inst.oadMethod === "OAD_APP_ONCHIP" ||
                           inst.oadMethod === "OAD_APP_OFFCHIP" || inst.oadMethod === "MULTISTEP_OAD" ||
                           inst.enablePersistentOad);
    if(signingActive)
    {
        const versionRegex = /^([1-9]\d*|0)(\.([1-9]\d*|0)(\.([1-9]\d*|0)(\+([1-9]\d*|0))?)?)?$/;
        if(!inst.mcubootSignVersion1 || !inst.mcubootSignVersion1.trim())
        {
            validation.logError("MCUBoot Signing Version (V1) must not be empty", inst, "mcubootSignVersion1");
        }
        else if(!versionRegex.test(inst.mcubootSignVersion1.trim()))
        {
            validation.logError(
                "MCUBoot Signing Version (V1) format: MAJOR.MINOR.REVISION+BUILD (e.g., 1.0.0 or 2.1.5+42)",
                inst, "mcubootSignVersion1"
            );
        }
        if(inst.mcubootSignVersion2 && inst.mcubootSignVersion2.trim() &&
           !versionRegex.test(inst.mcubootSignVersion2.trim()))
        {
            validation.logError(
                "MCUBoot Signing Version (V2) format: MAJOR.MINOR.REVISION+BUILD (e.g., 2.0.0 or 2.1.5+42)",
                inst, "mcubootSignVersion2"
            );
        }
        if(!inst.mcubootKeyPath || !inst.mcubootKeyPath.trim())
        {
            validation.logError("MCUBoot Signing Key Path must not be empty", inst, "mcubootKeyPath");
        }
    }

    if(inst.oadMethod === "MULTISTEP_OAD")
    {
        if(inst.msOadInitSlotSize % 4 !== 0)
        {
            validation.logError("Init Slot Size must be a multiple of 4 (word-aligned)", inst, "msOadInitSlotSize");
        }
        if(inst.msOadInitSlotSize < 0x400)
        {
            validation.logWarning("Init Slot Size may be too small for .cinit and reset vectors. Minimum recommended: 0x400", inst, "msOadInitSlotSize");
        }

        // For non-half-mode builds, slot1ObjectsList should be specified.
        // This is a warning (not error) because the prebuild script populates
        // this list automatically. In IAR, SysConfig runs before build actions,
        // so the list may be empty on the first build until the prebuild runs.
        if(!inst.halfModeBuild && !inst.slot1ObjectsList.trim())
        {
            validation.logWarning(
                "Slot 1 Objects List is empty. The MS-OAD prebuild script will " +
                "populate this automatically. If not using the prebuild, specify " +
                "libraries (.a) and objects (.obj/.o) for APP_1_SLOT manually. ",
                inst, "slot1ObjectsList"
            );
        }
    }
}

/*
 *  ======== getOpts ========
 * Return compiler options/defines for OAD configuration
 *
 * @param inst  - Module instance
 * @returns     - Array of compiler defines
 */
function getOpts(inst)
{
    let result = [];

    if(inst.enableOad)
    {
        result.push("-DOAD_CFG");

        if(inst.oadMethod === "MULTISTEP_OAD")
        {
            if(inst.halfModeBuild)
            {
                result.push("-DMS_OAD_HALF_MODE_BUILD");
            }

            // Add MS-OAD address defines from computed layout
            result.push(...MsOadLayout.getCompilerDefines(system.deviceData.deviceId, inst.msOadApp1HdrAddr));
        }
        else if(inst.oadMethod === "DUAL_IMAGE")
        {
            result.push(...DualImageLayout.getCompilerDefines(system.deviceData.deviceId, inst.dualImageAppHdrAddr));
        }
        else if(inst.enablePersistentOad)
        {
            result.push(...PersistentOadLayout.getCompilerDefines(system.deviceData.deviceId, inst.persistentAppSlotBase));
        }
        else if(inst.oadMethod === "OAD_APP_ONCHIP")
        {
            result.push(...OnchipOadLayout.getCompilerDefines(system.deviceData.deviceId, inst.onchipAppSlotBase));
        }
        else if(inst.oadMethod === "OAD_APP_OFFCHIP")
        {
            result.push(...OffchipOadLayout.getCompilerDefines(system.deviceData.deviceId, inst.offchipAppHdrAddr));
        }
    }

    return result;
}

/*
 *  ======== getLinkerDefs ========
 * Return linker symbol definitions for OAD configuration.
 * These are emitted to ti_utils_build_linker.cmd.genmap as
 * ti_utils_build_GenMap_MS_OAD, which the linker files alias to MS_OAD.
 *
 * @param inst  - Module instance
 * @returns     - Array of {name, value} linker symbol descriptors
 */
function getLinkerDefs(inst)
{
    if(!inst.enableOad)
    {
        return [];
    }

    if (inst.oadMethod === "MULTISTEP_OAD")
    {
        return MsOadLayout.getLinkerSymbols(system.deviceData.deviceId);
    }

    if (inst.oadMethod === "DUAL_IMAGE")
    {
        return DualImageLayout.getLinkerSymbols(system.deviceData.deviceId, inst.dualImageAppHdrAddr);
    }

    if (inst.enablePersistentOad)
    {
        return PersistentOadLayout.getLinkerSymbols(system.deviceData.deviceId, inst.persistentAppSlotBase);
    }

    if (inst.oadMethod === "OAD_APP_ONCHIP")
    {
        return OnchipOadLayout.getLinkerSymbols(system.deviceData.deviceId, inst.onchipAppSlotBase);
    }

    if (inst.oadMethod === "OAD_APP_OFFCHIP")
    {
        return OffchipOadLayout.getLinkerSymbols(system.deviceData.deviceId, inst.offchipAppHdrAddr);
    }

    return [];
}

/*
 *  ======== getMsOadLayout ========
 * Compute the MS-OAD flash memory layout for the current device.
 * FLASH_SIZE is obtained from getBondNVBaseAddress() since OAD builds always
 * have NVS enabled and the linker sets FLASH_SIZE = NVS_BASE.
 *
 * @returns {object|null} - Computed layout object, or null if unsupported device
 */
function getMsOadLayout()
{
    const deviceId  = system.deviceData.deviceId;
    return MsOadLayout.computeMsOadLayout(deviceId);
}

/*
 *  ======== getDualImageOadLayout ========
 * Return the dual image OAD flash memory layout for the current device.
 *
 * @returns {object|null} - Layout object, or null if unsupported device
 */
function getDualImageOadLayout()
{
    const deviceId = system.deviceData.deviceId;
    return DualImageLayout.getDualImageLayout(deviceId);
}

/*
 *  ======== getPersistentOadLayout ========
 * Return the persistent OAD flash memory layout for the current device.
 *
 * @returns {object|null} - Layout object, or null if unsupported device
 */
function getPersistentOadLayout()
{
    const deviceId = system.deviceData.deviceId;
    return PersistentOadLayout.getPersistentOadLayout(deviceId);
}

/*
 *  ======== moduleInstances ========
 * Dispatch to the per-method module instance factory in each layout file.
 * Each layout file owns the NVS region descriptors for its OAD method.
 * When OAD is enabled with auto-add, the returned module instances will be added
 * to the configuration automatically. This is used to add the required NVS regions
 * for each OAD method.
 * When auto-add is disabled, the NVS regions must be added manually by the user.
 *
 * @param inst  - Module instance
 * @returns     - Array of module instance descriptors
 */
function moduleInstances(inst)
{
    if (!inst.enableOad || !inst.oadAutoAddModuleInstances)
    {
        return [];
    }

    const deviceId = system.deviceData.deviceId;

    if (inst.oadMethod === "MULTISTEP_OAD")
    {
        return MsOadLayout.moduleInstances(deviceId, inst.msOadApp1HdrAddr);
    }

    if (inst.oadMethod === "DUAL_IMAGE")
    {
        return DualImageLayout.moduleInstances(deviceId, inst.dualImageAppSlotBase, inst.dualImageAppSlotSize);
    }

    if (inst.enablePersistentOad)
    {
        return PersistentOadLayout.moduleInstances(deviceId, inst.persistentAppSlotBase, inst.persistentAppSlotSize);
    }

    if (inst.oadMethod === "OAD_APP_ONCHIP")
    {
        return OnchipOadLayout.moduleInstances(deviceId, inst.onchipAppSlotBase, inst.onchipAppSlotSize);
    }

    if (inst.oadMethod === "OAD_APP_OFFCHIP")
    {
        const hwComp = (system.deviceData.board && system.deviceData.board.components)
                        ? system.deviceData.board.components.MX25R8035F : undefined;
        return OffchipOadLayout.moduleInstances(deviceId, hwComp);
    }

    return [];
}

/*
 *  ======== exports ========
 *  Export the BLE OAD Configuration module
 */
exports = {
    config: config,
    validate: validate,
    getOpts: getOpts,
    getLinkerDefs: getLinkerDefs,
    moduleInstances: moduleInstances,
    updateOadVisibility: updateOadVisibility,
    isOadSupported: _isOadSupported,
};
