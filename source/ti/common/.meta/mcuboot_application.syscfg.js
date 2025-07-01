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
 *  ======== mcuboot_application.syscfg.js ========
 */

const MCUbootTemplate = system.getScript("/ti/common/mcuboot/mcubootTemplate.syscfg.js");
const mcubootSettings = MCUbootTemplate.mcubootSettings;
const deviceGroup = MCUbootTemplate.deviceGroup;
const externalFlashSectorSize = MCUbootTemplate.externalFlashSectorSize;
const externalFlashSize = MCUbootTemplate.externalFlashSize;
const externalFlashBase = MCUbootTemplate.externalFlashBase;
const compressionDicSizeValues = MCUbootTemplate.compressionDicSizeValues;

const Common = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;
let logWarning  = Common.logWarning;

const moduleDesc = "MCUboot Application Configurations\n\n" +
"The configurations in this module apply to the following generated files: \n" +
"1. **ti_utils_build_linker.cmd.genmap**: Additional variables to use in the linker script\n" +
"2. **imgtool_args.txt**: New file that contains imgtool arguments\n\n" +
"**NOTE**: Simply adding this module does not automatically apply changes to the application. \n" +
"To take advantage of the outputs of this module, the following are the steps: \n" +
"1. Use the new generated `imgtool_args.txt` as input to the `imgtool sign` command.\n" +
"2. Use the new variables in `ti_utils_build_linker.cmd.genmap` in the linker script.\n\n"

let mcubootApplication = {
    displayName: "MCUboot App",
    description: "MCUboot Application Configurations",
    longDescription: moduleDesc,
    alwaysShowLongDescription : true,
    maxInstances: 1,
    templates: {
        "/ti/common/templates/imgtool_args.txt.xdt"     : "/ti/common/templates/imgtool_args.txt.xdt",
        "/ti/utils/build/GenMap.cmd.xdt":{modName: "/ti/common/mcuboot_application", getLinkerDefs: getLinkerDefs},
    },
    moduleStatic: {
        validate: validate,
        config: [
            {
                name        : "imageCompression",
                displayName : "Enable Image Compression",
                description : "Enable MCUboot to decompress image from secondary slot",
                default     : false,
                hidden      : (deviceGroup == "DeviceGroup_CC23X0") ? false : true,
                onChange    : changeImageCompression
            },
            {
                name        : "appVersion",
                displayName : "Version",
                longDescription : "The version of this project. This value will be used to sign" +
                                "the image after it has been built." +
                                "\n\nFormat: maj.min.rev+build",
                default     : "1.0.0+0",
                readOnly    : false,
                hidden      : false
            },
            {
                name        : "appBase",
                displayName : "Base Address",
                description : "Base Address of Application Image",
                displayFormat: { radix: "hex", bitSize: 32 },
                default     : mcubootSettings["image1"]["tzDisabledBase"]["primaryBase"],
                readOnly    : false,
                hidden      : false
            },
            {
                name        : "appSlotSize",
                displayName : "Slot Size",
                description : "Slot size allocated for the application",
                displayFormat: { radix: "hex", bitSize: 32 },
                default     : mcubootSettings["image1"]["primarySize"],
                readOnly    : false,
                hidden      : false
            },
            {
                name        : "appSlotSizeCompressed",
                displayName : "Compressed Slot Size",
                description : "Slot size allocated to the compressed application",
                displayFormat: { radix: "hex", bitSize: 32 },
                default     : mcubootSettings["image1Compressed"]["secondarySize"],
                readOnly    : false,
                hidden      : true
            },
            {
                displayName: "LZMA Encoder",
                description: "LZMA Encoder Configurations",
                collapsed: true,
                config  : [
                    {
                        name        : "lzmaLc",
                        displayName : "Literal Context Bits (LC)",
                        longDescription : "Determines how much of the previous byte's data is used " +
                                        "as context for encoding/decoding literals. Higher values " +
                                        "increase the compression ratio but require more memory. " +
                                        "This value must match the one used during compression." +
                                        "\n\n **Default:** 0" +
                                        "\n\n**Range:** 0 - 8",
                        displayFormat: "dec",
                        default     : 0,
                        range       : [0, 8],
                        readOnly    : false,
                        hidden      : true
                    },
                    {
                        name        : "lzmaLp",
                        displayName : "Literal Pos Bits (LP)",
                        longDescription : "Determines how the position in the decompressed data " +
                                        "stream affects the decoding of literals. Useful for " +
                                        "position-dependent data. This must be set " +
                                        "to the same value used during compression." +
                                        "\n\n **Default:** 0" +
                                        "\n\n**Range:** 0 - 4",
                        displayFormat: "dec",
                        default     : 0,
                        range       : [0, 4],
                        readOnly    : false,
                        hidden      : true
                    },
                    {
                        name        : "lzmaPb",
                        displayName : "Pos Bits (PB)",
                        longDescription : "Defines how many bits of the position are used to select " +
                                        "the probability model for match distances. Higher values " +
                                        "are better for larger files with repeated patterns. This " +
                                        "must be set to the same value used during compression." +
                                        "\n\n **Default:** 0" +
                                        "\n\n**Range:** 0 - 4",
                        displayFormat: "dec",
                        default     : 0,
                        range       : [0, 4],
                        readOnly    : false,
                        hidden      : true
                    },
                    {
                        name        : "lzmaDicSize",
                        displayName : "Dictionary Size",
                        longDescription : "Dictionary size. Must match the one used during compression.",
                        default     : 'LZMA2_DIC_SIZE_12288',
                        options     : compressionDicSizeValues,
                        readOnly    : false,
                        hidden      : true
                    }
                ]
            },
        ]
    }
};

function changeImageCompression(inst, ui)
{
    if (deviceGroup == "DeviceGroup_CC23X0")
    {
        if (inst.imageCompression)
        {
            inst.appBase = mcubootSettings["image1Compressed"]["tzDisabledBase"]["primaryBase"];
            inst.appSlotSize = mcubootSettings["image1Compressed"]["primarySize"];

            ui.lzmaLc.hidden = false;
            ui.lzmaLp.hidden = false;
            ui.lzmaPb.hidden = false;
            ui.lzmaDicSize.hidden = false;
            ui.appSlotSizeCompressed.hidden = false;
        }
        else
        {
            inst.appBase = mcubootSettings["image1"]["tzDisabledBase"]["primaryBase"];
            inst.appSlotSize = mcubootSettings["image1"]["primarySize"];

            ui.lzmaLc.hidden = true;
            ui.lzmaLp.hidden = true;
            ui.lzmaPb.hidden = true;
            ui.lzmaDicSize.hidden = true;
            ui.appSlotSizeCompressed.hidden = true;
        }
    }
}

function validate(inst, validation) {

    // If compression is enabled, the compressed image slot size must be smaller than the uncompressed slot size
    if (inst.imageCompression)
    {
        if(inst.appSlotSize < inst.appSlotSizeCompressed)
        {
            logError(validation, inst, "appSlotSize",
                    "Slot size for uncompressed image must be larger than compressed image slot size");
            logError(validation, inst, "appSlotSizeCompressed",
                    "Slot size for compressed image must be smaller than uncompressed image slot size");
        }
    }

    // Base and slot sizes must be sector size aligned
    if(inst.appBase % mcubootSettings["alignment"]["sectorSize"] !== 0)
    {
        logError(validation, inst, "appBase",
                 "Application base must be sector aligned.");
    }

    if(inst.appSlotSize % mcubootSettings["alignment"]["sectorSize"] !== 0)
    {
        logError(validation, inst, "appSlotSize",
                 "Application slot size must be sector aligned.");
    }

    if(inst.imageCompression && inst.appSlotSizeCompressed % mcubootSettings["alignment"]["sectorSize"] !== 0)
    {
        logError(validation, inst, "appSlotSizeCompressed",
                "Compressed application slot size must be sector aligned.");
    }
}

function getLinkerDefs()
{
    var mcuboot = system.modules["/ti/common/mcuboot_application"];

    var linkerDefs = [
        { name: "MCUBOOT_APP_BASE", value: mcuboot.$static.appBase },
        { name: "MCUBOOT_APP_SLOT_SIZE", value: mcuboot.$static.appSlotSize },
    ]

    if (mcuboot.$static.imageCompression)
    {
        linkerDefs.push({ name: "MCUBOOT_APP_SLOT_SIZE_COMPRESSED", value: mcuboot.$static.appSlotSizeCompressed });
    }

    return linkerDefs;

}

exports = mcubootApplication;