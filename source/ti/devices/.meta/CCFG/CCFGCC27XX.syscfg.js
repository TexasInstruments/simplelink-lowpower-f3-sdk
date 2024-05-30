/*
 * Copyright (c) 2022-2024, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== CCFGCC27XX.syscfg.js ========
 */

const Common = system.getScript("/ti/drivers/Common.js");

const MAX_PBLDRVTOR = 0xFFFFFFEF;
const MAX_SERIALIOCFGINDEX = 0x07;
const MAX_SACITIMEOUTEXP = 0x07;
const MAX_READPROT_MAINSECTORS = 0x0F;
const MAX_READPROT_CCFGSECTOR = 0x3F;
const MAX_PIN_TRIGGER_DIO = 0x3F; // TODO: Find correct value for CC27XX
const MAX_CAP_ARRAY = 0x3F;
const MAX_HSM_SIZE = 0x07;

const moduleDesc = `
The device has 3 dedicated configuration areas in flash that must contain a valid configuration

* The Customer Configuration Area (CCFG) is used by boot ROM and TI provided drivers to configure the device. It starts at 0x4E020000 and has a size of 0x800 bytes.
* The Security Configuration Area (SCFG) must contain a valid Security Configuration. It starts at 0x4E040000 and has a size of 0x400 bytes.
* The HSM OTP area (HSMOTP) must contain a valid configuration for the HSM. It starts at 0x4E020800 and has a size of 0x800 bytes.

All of these configurations are done by simply letting SysConfig generate file ti_devices_config.c and including it in the project.`;

let board = system.deviceData.board;
let defaultOverrideHfxtCapArray = false;
let defaultHfxtCapArrayQ1 = 0x00;
let defaultHfxtCapArrayQ2 = 0x00;

/* Override cap array values for LP_EM_CC2745R10_Q1 */
if (board !== undefined) {
    if (board.name.match(/LP_EM_CC2745R10_Q1/)) {
        defaultOverrideHfxtCapArray = true;
        defaultHfxtCapArrayQ1 = 0x27;
        defaultHfxtCapArrayQ2 = 0x27;
    }
}

let devSpecific = {
    longDescription: moduleDesc,
    templates: {
        /* Contributes CRC symbols to linker file */
        "/ti/utils/build/GenMap.cmd.xdt": { modName: "/ti/devices/CCFG", getLinkerSyms: getLinkerSyms }
    },
    moduleStatic: {
        modules: modules,
        validate: validate,
        config: [
            {
                name: "srcClkLF",
                displayName: "Low Frequency Clock Source",
                description: "Low frequency clock source",
                longDescription: `When choosing the external clock, supply
a 31.25 kHz square wave, with a peak voltage of 3V and an offset of 1.5V to the appropriate pin.`,
                readOnly: false,
                hidden: false,
                options: [
                    { name: "LF XOSC" },
                    { name: "LF RCOSC" },
                    { name: "External LF clock" }
                ],
                default: "LF XOSC",
                onChange: onChangeSrcClkLF
            },
            {
                name: "overrideHfxtCapArray",
                displayName: "Override HFXT Cap Array Trims",
                description: "Enables setting the HFXT cap-array ramp and target values",
                default: defaultOverrideHfxtCapArray,
                readOnly: false,
                hidden: false,
                onChange: (inst, ui) => {
                    if (inst.overrideHfxtCapArray === true) {
                        ui.hfxtCapArrayQ1.hidden = false;
                        ui.hfxtCapArrayQ2.hidden = false;
                    }
                    else {
                        ui.hfxtCapArrayQ1.hidden = true;
                        ui.hfxtCapArrayQ2.hidden = true;
                    }
                }
            },
            {
                name: "hfxtCapArrayQ1",
                displayName: "HFXT Cap Array Q1 Ramp Value",
                description: "Set the high frequency oscillator cap-array Q1 value, changing the frequency offset",
                longDescription: `The cap-array Q1 value is used when turning on HFXT.
The crystal's frequency offset can be controlled by changing this value.
Q1 and Q2 should not differ by more than one step.`,
                displayFormat: { radix: "hex", bitSize: 2 },
                default: defaultHfxtCapArrayQ1,
                readOnly: false,
                hidden: !defaultOverrideHfxtCapArray
            },
            {
                name: "hfxtCapArrayQ2",
                displayName: "HFXT Cap Array Q2 Ramp Value",
                description: "Set the high frequency oscillator cap-array Q2 value, changing the frequency offset",
                longDescription: `The cap-array Q2 value is used when turning on HFXT.
The crystal's frequency offset can be controlled by changing this value.
Q1 and Q2 should not differ by more than one step.`,
                displayFormat: { radix: "hex", bitSize: 2 },
                default: defaultHfxtCapArrayQ2,
                readOnly: false,
                hidden: !defaultOverrideHfxtCapArray
            },
            {
                name: "enableInitialHfxtAmpComp",
                displayName: "Initial HFXT Amplitude Compensation",
                longDescription: `This enables the [__Initial HFXT Amplitude Compensation__][1] feature.

[1]: /drivers/doxygen/html/_power_c_c27_x_x_8h#ti_drivers_PowerCC27XX_Initial_HFXT_Amplitude_Compensation "Initial HFXT Amplitude Compensation"
`,
                default: false
            },
            {
                name: "lfoscCompConfig",
                displayName: "LFOSC Compensation",
                longDescription: `Settings for LFOSC compensation. When LFOSC Compensation is enabled, HFXT will
periodically be turned on to calibrate the LFOSC.

Note, the LFOSC Compensation is to be considered beta quality. An improved
accuracy of LFOSC is expected with best effort, but an absolute accuracy cannot
be guaranteed.`,
                config: [
                    {
                        name: "enableLfoscComp",
                        displayName: "Enable LFOSC Compensation",
                        hidden: true,
                        default: false,
                        onChange: onChangeLfoscComp
                    },
                    {
                        displayName: "Default Compensation Profile",
                        longDescription: `Selects the default LFOSC Compensation Profile to be used after startup.
If a profile is selected, the Board_init() function will configure the LFOSC
Compensation to use the selected profile and also enable LFOSC Compensation.
If no profile is selected, the application must set the profile and
enable compensation.`,
                        hideNoneOption: false,
                        isArray: false,
                        hidden: true,
                        name: "defaultLfoscCompProfile",
                        options: function (inst) {
                            return system.modules["/ti/drivers/power/PowerLPF3LfoscCompProfiles"].$instances;
                        }
                    }
                ]
            },
            {
                displayName: "Bootloader Configuration",
                config: [
                    {
                        name: "bldrSetting",
                        displayName: "Bootloader Configration",
                        longDescription: `This configurable chooses whether to use default bootloader,
default bootloader with customer settings, or user-specific bootloader`,
                        default: "Default FCFG bootloader, with CCFG settings",
                        options: [
                            { name: "Default FCFG bootloader" },
                            { name: "Default FCFG bootloader, with CCFG settings" },
                            { name: "User-specific bootloader" },
                            { name: "Any bootloader forbidden" }
                        ],
                        onChange: function (inst, ui) {
                            updateBldrVisibility(inst, ui);
                        }
                    },
                    {
                        name: "pBldrVtor",
                        displayName: "Bootloader Vector Table",
                        description: "Bootloader vector table address",
                        longDescription: `This configurable sets the address of the user-specific bootloader`,
                        hidden: false,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0x00000000
                    },
                    {
                        name: "pAppVtor",
                        displayName: "Application Vector Table",
                        description: "Application vector table address",
                        longDescription: `Pointer to application vector table. Used by bootloader upon exit
or invoked directly by boot sequence if neither user bootloader nor default bootloader is allowed.
0xFFFFFFFF: No user application vector table`,
                        hidden: false,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0x00000000
                    },
                    {
                        name: "bldrEnabled",
                        displayName: "Enable Serial Bootloader",
                        longDescription: `true: All serial bootloader commands are accepted.
false: All serial bootloader commands except BLDR_CMD_GET_STATUS are rejected.`,
                        hidden: false,
                        default: false,
                        onChange: (inst, ui) => {
                            updateBldrVisibility(inst, ui);
                        }
                    },
                    {
                        name: "serialIoCfgIndex",
                        displayName: "Serial IO Configuration Index",
                        longDescription: `Index in table holding predefined set of serial bootloader IO configurations.`,
                        hidden: true,
                        default: 0
                    },
                    {
                        name: "pinTriggerEnabled",
                        displayName: "Enable Pin Trigger",
                        description: "Enable Pin triggering of serial bootloader.",
                        hidden: true,
                        default: false,
                        onChange: (inst, ui) => {
                            updateBldrVisibility(inst, ui);
                        }
                    },
                    {
                        name: "pinTriggerDio",
                        displayName: "Pin Trigger DIO",
                        description: "Pin Trigger DIO number",
                        longDescription: `DIO pin number used for trigger of serial bootloader if pin triggering is enabled`,
                        hidden: true,
                        default: 0
                    },
                    {
                        name: "pinTriggerLevel",
                        displayName: "Pin Trigger Level",
                        longDescription: `level on the DIO selected, which will force serial bootloader
to be triggered during boot. Only valid if pin triggering is enabled.`,
                        hidden: true,
                        default: "LOW",
                        options: [
                            { name: "HIGH" },
                            { name: "LOW" }
                        ]
                    }
                ]
            },
            {
                displayName: "Hardware Options",
                config: [
                    {
                        name: "hwOpts0",
                        displayName: "Hardware Options 1",
                        description: "Hardware Options 1",
                        longDescription: `Value written to both the PMCTL:HWOPT0 and CLKCTL:HWOPT0 registers by ROM code
on PRODDEV at execution transfer from boot code/bootloader to application image`,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0xFFFFFFFF
                    },
                    {
                        name: "hwOpts1",
                        displayName: "Hardware Options 2",
                        description: "Hardware Options 2",
                        longDescription: `Value written to both the PMCTL:HWOPT1 and CLKCTL:HWOPT1 registers by ROM code
on PRODDEV at execution transfer from boot code/bootloader to application image`,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0xFFFFFFFF
                    }
                ]
            },
            {
                displayName: "Device Permission Settings",
                config: [
                    {
                        name: "allowDebugPort",
                        displayName: "Allow SWD Debug Port",
                        description: "Allow access to SWD Debug Port",
                        longDescription: `If not allowed, it will not be possible to access any part of the debug
system, meaning that it will not be possible to program or erase the chip.`,
                        default: true
                    },
                    {
                        name: "allowEnergyTrace",
                        displayName: "Allow Energy Trace",
                        description: "Allow access to energy trace AP",
                        longDescription: `Allow access to energy trace AP (if present)`,
                        hidden: true,
                        default: true
                    },
                    {
                        name: "allowFlashVerify",
                        displayName: "Allow Flash Verify",
                        description: "Allow flash verify by SACI.",
                        default: true
                    },
                    {
                        name: "allowFlashProgram",
                        displayName: "Allow Flash Program",
                        description: "Allow Flash Program by SACI",
                        default: true
                    },
                    {
                        name: "allowChipErase",
                        displayName: "Allow Chip Erase",
                        description: "Allow Chip Erase by SACI",
                        default: true
                    },
                    {
                        name: "allowToolsClientMode",
                        displayName: "Allow Tools Client Mode",
                        description: "Allow RAM-only tools client mode to be enabled by SACI.",
                        default: true
                    },
                    {
                        name: "allowFakeStby",
                        displayName: "Allow Fake Standby",
                        longDescription: `Allow fake standby. Controls setting of the
DBGSS:SPECIAL_AUTH_CLR.FAKESTBYEN_CLR register bit field done by ROM code.`,
                        default: true
                    },
                    {
                        name: "allowReturnToFactory",
                        displayName: "Allow Return To Factory",
                        description: "Allow return to factory by SACI.",
                        default: true
                    }
                ]
            },
            {
                displayName: "SACI Settings (Secure AP Command Interface)",
                config: [
                    {
                        name: "saciTimeoutOverride",
                        displayName: "SACI Timeout Override",
                        description: "Override FCFG SACI timeout value",
                        longDescription: `This configurable chooses whether to use the FCFG SACI timeout value,
or override it with the Customer Configured SACI timeout value defined below.`,
                        default: true,
                        onChange: (inst, ui) => {
                            ui["saciTimeoutExp"].hidden = !(inst.saciTimeoutOverride);
                        }
                    },
                    {
                        name: "saciTimeoutExp",
                        displayName: "SACI Timeout Override Value",
                        longDescription: `This configurable sets the SACI timeout, if override is enabled.
0: Infinite
Any other: (2^saciTimeout)*64 ms`,
                        default: 7
                    }
                ]
            },
            {
                displayName: "Flash Protection Settings",
                config: [
                    {
                        name: "writeEraseProt_mainSectors0_31",
                        displayName: "Main Sectors 0-31",
                        description: "Sets write/erase protection for main sectors 0-31 (1 sector/bit)",
                        longDescription: `Value is written to VIMS:WEPRA register by ROM code on PRODDEV at execution transfer
from boot code/bootloader to application image. Each bit corresponds to 1 sector. The register has sticky-0 bits.`,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0xFFFFFFFF
                    },
                    {
                        name: "writeEraseProt_mainSectors32_255",
                        displayName: "Main Sectors 32-255",
                        description: "Sets write/erase protection for main sectors 32-255 (8 sectors/bit)",
                        longDescription: `Value is written to VIMS:WEPRB0 register by ROM code on PRODDEV at execution transfer
from boot code/bootloader to application image. Each bit corresponds to 8 sectors. The register has sticky-0 bits.`,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0xFFFFFFFF
                    },
                    {
                        name: "writeEraseProt_mainSectors256_511",
                        displayName: "Main Sectors 256-511",
                        description: "Sets write/erase protection for main sectors 256-511 (8 sectors/bit)",
                        longDescription: `Value is written to VIMS:WEPRB1 register by ROM code on PRODDEV at execution transfer
from boot code/bootloader to application image. Each bit corresponds to 8 sectors. The register has sticky-0 bits.`,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0xFFFFFFFF
                    },
                    {
                        name: "writeEraseProt_ccfgSector",
                        displayName: "Write/Erase Protection, CCFG Sector",
                        description: "Sets write/erase protection for CCFG sector",
                        default: true
                    },
                    {
                        name: "writeEraseProt_fcfgSector",
                        displayName: "Write/Erase Protection, FCFG Sector",
                        description: "Sets write/erase protection for FCFG sector",
                        default: true
                    },
                    {
                        name: "writeEraseProt_engrSector",
                        displayName: "Write/Erase Protection, ENGR Sector",
                        description: "Sets write/erase protection for ENGR sector",
                        default: true
                    },
                    {
                        name: "writeEraseProt_scfgSector",
                        displayName: "Write/Erase Protection, SCFG Sector",
                        description: "Sets write/erase protection for SCFG sector",
                        default: true
                    },
                    {
                        name: "writeEraseProt_vlogSector",
                        displayName: "Write/Erase Protection, VLOG Sector",
                        description: "Sets write/erase protection for VLOG sector",
                        default: true
                    },
                    {
                        name: "ccfgSector",
                        displayName: "Read Protection, CCFG Sector",
                        description: "Sets read protection for CCFG sector",
                        longDescription: `Read protection for CCFG (16B granular watermark).
Value is written to the VIMS:RDPRNMN register by ROM code on PRODDEV at execution transfer
from boot code/bootloader to application image. The register has sticky-0 bits.`,
                        displayFormat: { radix: "hex", bitSize: 8 },
                        default: 0x3F
                    },
                    {
                        name: "mainSectors",
                        displayName: "Read Protection Main Sector",
                        description: "Sets read protection for main sector",
                        longDescription: `Value is written to the VIMS:RDPRMN register by ROM code on PRODDEV at execution transfer
from boot code/bootloader to application image. The register has sticky-0 bits.`,
                        displayFormat: { radix: "hex", bitSize: 4 },
                        default: 0x0F
                    },
                    {
                        name: "chipEraseRetain_mainSectors0_31",
                        displayName: "Erase/Retain, Main Sectors 0-31",
                        description: "Sets chip write/erase protection for main sectors 0-31",
                        longDescription: `Used by the SC_FLASH_ERASE_CHIP SACI command for main sector erase protection.`,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0x00
                    },
                    {
                        name: "chipEraseRetain_mainSectors32_255",
                        displayName: "Erase/Retain, Main Sectors 32-255",
                        description: "Sets chip write/erase protection for main sectors 32-255",
                        longDescription: `Used by the SC_FLASH_ERASE_CHIP SACI command for main sector erase protection.`,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0x00
                    },
                    {
                        name: "chipEraseRetain_mainSectors256_511",
                        displayName: "Erase/Retain, Main Sectors 256-511",
                        description: "Sets chip write/erase protection for main sectors 256-511",
                        longDescription: `Used by the SC_FLASH_ERASE_CHIP SACI command for main sector erase protection.`,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0xFC000000
                    }
                ]
            },
            {
                displayName: "Hardware Initialisation And User Data",
                longDescription: `The hardware initialisation list must be defined in a seperate file, which is included and inserted
into the ccfg structure. The preprocessor-symbol that defines the list, and the file in which the symbol is defined
must be supplied below. The list must be terminated with CPYLST_EOL
Example-file:

#define CCFG_HW_INIT_COPYLIST = CPYLST_WAIT(100), CPYLST_NOP, CPYLST_EOL

Available macros for use in CopyList:

CPYLST_EOL

CPYLST_NOP

CPYLST_WAIT(x)

CPYLST_CPY(a, n)

CPYLST_CPYFULLADDR(a)

CPYLST_JUMP(a)

CPYLST_CALL(a)`,

                config: [
                    {
                        name: "useHwInitCopyList",
                        displayName: "Hardware Initialisation Copylist",
                        longDescription: `CopyList is executed by ROM code at execution transfer from boot code/bootloader to application image.
May be used by customer/SYSCFG to initialize hardware right before application is entered.
Also used to pad out CCFG to correct size.`,
                        default: false,
                        onChange: (inst, ui) => {
                            let setHidden = !(inst.useHwInitCopyList);
                            ui["hwInitCopyListMacro"].hidden = setHidden;
                            ui["hwInitCopyListFile"].hidden = setHidden;
                        }
                    },
                    {
                        name: "hwInitCopyListMacro",
                        displayName: "Copylist Macro",
                        description: "Hardware initialisation CopyList macro that defines a list of CopyList values",
                        hidden: true,
                        default: "CCFG_HW_INIT_COPYLIST"
                    },
                    {
                        name: "hwInitCopyListFile",
                        displayName: "Copylist File",
                        description: "Hardware initialisation CopyList file in which the CopyList Macro is defined",
                        hidden: true,
                        default: "",
                        fileFilter: ".h,.hpp,.txt"
                    },
                    {
                        name: "enableUserRecord",
                        displayName: "Enable User Record",
                        description: "Enable user record area in CCFG",
                        longDescription: `User record can contain any data, it has no dependencies in ROM boot code. This area is also
programmable through separate SACI command. User record size is fixed at 124 bytes, plus an additional 4 bytes to hold a CRC32 checksum.
The User Record Macro must be defined in the User Record File to be a list of values to be placed in the User Record Area`,
                        default: false,
                        onChange: (inst, ui) => {
                            let setHidden = !(inst.enableUserRecord);
                            ui["userRecordMacro"].hidden = setHidden;
                            ui["userRecordFile"].hidden = setHidden;
                            ui["userRecordCRC"].hidden = setHidden;
                        }
                    },
                    {
                        name: "userRecordMacro",
                        displayName: "User Record Macro",
                        description: "Macro that defines a list of User Record Values",
                        hidden: true,
                        default: "CCFG_USER_RECORD"
                    },
                    {
                        name: "userRecordFile",
                        displayName: "User Record File",
                        description: "File in which the User Record Macro is defined",
                        hidden: true,
                        default: "",
                        fileFilter: ".h,.hpp,.txt"
                    },
                    {
                        name: "userRecordCRC",
                        displayName: "User Record CRC",
                        longDescription: `Enable generation of user record begin/end symbols in the ELF executable.
These symbols can be used by ELF-based tools (e.g. crc_tool) to manage the optional user record's CRC.`,
                        hidden: true,
                        default: false
                    }
                ]
            },
            {
                displayName: "Debug Configuration",
                config: [
                    {
                        name: "debugAuthorization",
                        displayName: "Debug Authorization Configuration",
                        default: "Debug always allowed",
                        options: [
                            { name: "Debug always allowed" },
                            { name: "Require debug authentication" },
                            { name: "Debug not allowed" }
                        ],
                        onChange: (inst, ui) => {
                            ui["debugAllowBldr"].hidden = (inst.debugAuthorization == "Debug not allowed");
                            ui["debugPwdId"].hidden = (inst.debugAuthorization != "Require debug authentication");
                            ui["debugPwdHash"].hidden = (inst.debugAuthorization != "Require debug authentication");
                        }
                    },
                    {
                        name: "debugAllowBldr",
                        displayName: "Allow Bootloader Debugging",
                        default: true
                    },
                    {
                        name: "debugPwdId",
                        displayName: "Password ID",
                        description: "Hex-formatted password ID",
                        longDescription: `64-bit customer-defined password ID readable through SACI command (may be used by to calculate or look up debug password)`,
                        hidden: true,
                        default: "0101020305080D15"
                    },
                    {
                        name: "debugPwdHash",
                        displayName: "Password Hash",
                        description: "Hex-formatted SHA256-hash of user-supplied password",
                        hidden: true,
                        default: "6dd7e436ebf431df95ae15ee03ba8ee4c4c63fd8453f675e74d7c2012c9058e5"
                    }
                ]
            },
            {
                displayName: "Security Configuration",
                config: [
                    {
                        name: "hsmPublicKeyHash",
                        displayName: "HSM Public Key Hash",
                        longDescription: `Optional customer public key hash for authenticating HSM updates.`,
                        default: "c665564a4785eaf7e2c41e4508dd95517a3e7d849fe0c133a76455284a6c9d42"
                    },
                    {
                        name: "hsmSize",
                        displayName: "HSM Size",
                        longDescription: `This value is copied to VIMS.CFG.HSM_SIZE to during bootup sequence.`,
                        default: 0x03
                    },
                    {
                        name: "flashBanksMode",
                        displayName: "Flash Banks Mode",
                        longDescription: `This value is copied to VIMS.CFG.SPLMODE`,
                        default: "Monolithic (0x00)",
                        options: [
                            { name: "Monolithic (0x00)" },
                            { name: "Split (0x01)" }
                        ]
                    }
                ]
            },
            {
                name: "voltageRegulator",
                displayName: "Voltage Regulator",
                description: "Choose between using the internal DCDC or GLDO voltage regulator",
                default: "DCDC",
                longDescription: `On the CC27XX devices, the DCDC regulator is
disabled by default in hardware. This setting enables the DCDC regulator upon
device startup. It is possible to choose between using the internal DCDC or GLDO
regulator. Enabling the DCDC regulator means that it is predominantly used, but the GLDO will still
automatically turn on and take over if the voltage drops too low.
`,
                options:
                    [
                        {
                            name: "DCDC",
                            description: "The internal DCDC regulator will be enabled for use."
                        },
                        {
                            name: "GLDO",
                            description: "The internal GLDO regulator will be enabled for use." +
                                "This will disable the DCDC regulator."
                        }
                    ]
            }
        ]
    }
};

/*!
 * ======== getLinkerSyms ========
 *  Used by GenMaps to define linker symbols, for example CRC checksum symbols
 */
function getLinkerSyms(inst) {

    let linkerSyms = [

        { name: "CRC_CCFG_BOOT_CFG_begin", value: 0x4e020000 },
        { name: "CRC_CCFG_BOOT_CFG_end", value: 0x4e02000B },

        { name: "CRC_CCFG_begin", value: 0x4E020010 },
        { name: "CRC_CCFG_end", value: 0x4E02074B },

        { name: "CRC_CCFG_DEBUG_begin", value: 0x4E0207D0 },
        { name: "CRC_CCFG_DEBUG_end", value: 0x4E0207FB },

        { name: "CRC_SCFG_begin", value: 0x4e040000 },
        { name: "CRC_SCFG_end", value: 0x4e04003B }
    ];

    if (inst.$static.enableUserRecord && inst.$static.userRecordCRC) {
        linkerSyms.push(
            { name: "CRC_CCFG_USER_RECORD_begin", value: 0x4E020750 },
            { name: "CRC_CCFG_USER_RECORD_end", value: 0x4E0207CB },
        );
    }

    return linkerSyms;
}

/*!
 *  ======== updateBldrVisibility ========
 *  Update the visibility of all fields in Bootloader settings
 *  whenever one of the fields change
 *
 *  @param inst - CCFG instance to be validated
 *  @param ui   -   GUI state
 */
function updateBldrVisibility(inst, ui) {
    let setHidden = inst.bldrSetting == "Any bootloader forbidden" || inst.bldrSetting == "Default FCFG bootloader";
    ui["pBldrVtor"].hidden = setHidden;
    ui["pAppVtor"].hidden = setHidden;
    ui["bldrEnabled"].hidden = setHidden;

    setHidden = setHidden || !(inst.bldrEnabled);
    ui["serialIoCfgIndex"].hidden = setHidden;
    ui["pinTriggerEnabled"].hidden = setHidden;

    setHidden = setHidden || !(inst.pinTriggerEnabled);
    ui["pinTriggerDio"].hidden = setHidden;
    ui["pinTriggerLevel"].hidden = setHidden;
}

/*
 *  ======== onChangeSrcClkLF ========
 *  onChange callback function for the srcClkLF config
 */
function onChangeSrcClkLF(inst, ui) {
    ui.enableLfoscComp.hidden = inst.srcClkLF != "LF RCOSC";
    if (ui.enableLfoscComp.hidden) {
        inst.enableLfoscComp = false;
    }
    onChangeLfoscComp(inst, ui);
}

/*
 *  ======== onChangeLfoscComp ========
 *  onChange callback function for the LFOSC Compensation config
 */
function onChangeLfoscComp(inst, ui) {
    ui.defaultLfoscCompProfile.hidden = !inst.enableLfoscComp;
}

/*!
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  @param inst       - CCFG instance to be validated
 *  @param validation - Issue reporting object
 */
function validate(inst, validation) {

    if(inst.enableLfoscComp === true)
    {
        Common.logInfo(validation, inst, "enableLfoscComp",
            "The LFOSC Compensation is to be considered beta quality. An improved accuracy of LFOSC is expected with best effort, but an absolute accuracy cannot be guaranteed.");
    }

    if (inst.hfxtCapArrayQ1 > MAX_CAP_ARRAY) {
        Common.logError(validation, inst, "hfxtCapArrayQ1",
            "Must be less than 0x" + (MAX_CAP_ARRAY + 1).toString(16));
    }

    if (inst.hfxtCapArrayQ2 > MAX_CAP_ARRAY) {
        Common.logError(validation, inst, "hfxtCapArrayQ2",
            "Must be less than 0x" + (MAX_CAP_ARRAY + 1).toString(16));
    }

    if (Math.abs(inst.hfxtCapArrayQ1 - inst.hfxtCapArrayQ2) > 1) {
        Common.logError(validation, inst, "hfxtCapArrayQ1",
            "The Q1 and Q2 cap trims may not differ by more than one step to avoid excessive RF noise.");
        Common.logError(validation, inst, "hfxtCapArrayQ2",
            "The Q1 and Q2 cap trims may not differ by more than one step to avoid excessive RF noise.");
    }

    if (inst.pBldrVtor > MAX_PBLDRVTOR) {
        Common.logError(validation, inst, "pBldrVtor",
            "Must be less than 0x" + (MAX_PBLDRVTOR + 1).toString(16));
    }

    if (inst.serialIoCfgIndex > MAX_SERIALIOCFGINDEX) {
        Common.logError(validation, inst, "serialIoCfgIndex",
            "Must be less than 0x" + (MAX_SERIALIOCFGINDEX + 1).toString(16));
    }

    if (inst.pinTriggerDio > MAX_PIN_TRIGGER_DIO) {
        Common.logError(validation, inst, "pinTriggerDio",
            "Must be less than 0x" + (MAX_PIN_TRIGGER_DIO + 1).toString(16));
    }

    if (inst.saciTimeoutExp > MAX_SACITIMEOUTEXP) {
        Common.logError(validation, inst, "saciTimeoutExp",
            "Must be less than 0x" + (MAX_SACITIMEOUTEXP + 1).toString(16));
    }

    if (!(inst.debugPwdHash.match(/^[a-fA-F0-9]{0,64}$/))) {
        Common.logError(validation, inst, "debugPwdHash",
            "Must be valid hex-formatted SHA256 hash");
    }

    if (!(inst.debugPwdId.match(/^[a-fA-F0-9]{0,16}$/))) {
        Common.logError(validation, inst, "debugPwdId",
            "Must be 64-bit hex-formatted value");
    }

    if (inst.hwOpts0 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "hwOpts0",
            "Must be 32-bit value");
    }

    if (inst.hwOpts1 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "hwOpts1",
            "Must be 32-bit value");
    }

    if (inst.pAppVtor > 0xFFFFFFFF) {
        Common.logError(validation, inst, "pAppVtor",
            "Must be 32-bit value");
    }

    if (inst.writeEraseProt_mainSectors0_31 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "writeEraseProt_mainSectors0_31",
            "Must be 32-bit value");
    }

    if (inst.writeEraseProt_mainSectors32_255 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "writeEraseProt_mainSectors32_255",
            "Must be 32-bit value");
    }

    if (inst.writeEraseProt_mainSectors256_511 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "writeEraseProt_mainSectors256_511",
            "Must be 32-bit value");
    }

    if (inst.ccfgSector > MAX_READPROT_CCFGSECTOR) {
        Common.logError(validation, inst, "ccfgSector",
            "Must be less than 0x" + (MAX_READPROT_CCFGSECTOR + 1).toString(16));
    }

    if (inst.mainSectors > MAX_READPROT_MAINSECTORS) {
        Common.logError(validation, inst, "mainSectors",
            "Must be less than 0x" + (MAX_READPROT_MAINSECTORS + 1).toString(16));
    }

    if (inst.chipEraseRetain_mainSectors0_31 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "chipEraseRetain_mainSectors0_31",
            "Must be 32-bit value");
    }

    if (inst.chipEraseRetain_mainSectors32_255 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "chipEraseRetain_mainSectors32_255",
            "Must be 32-bit value");
    }

    if (inst.chipEraseRetain_mainSectors256_511 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "chipEraseRetain_mainSectors256_511",
            "Must be 32-bit value");
    }

    if (!(inst.hsmPublicKeyHash.match(/^[a-fA-F0-9]{0,64}$/))) {
        Common.logError(validation, inst, "hsmPublicKeyHash",
            "Must be valid hex-formatted SHA256 hash");
    }

    if (inst.hsmSize > MAX_HSM_SIZE) {
        Common.logError(validation, inst, "hsmSize",
            "Must be less than 0x" + (MAX_HSM_SIZE + 1).toString(16));
    }
}

/*
 *  ======== modules ========
 */
function modules(inst) {
    let tmpModules = [];

    /* If LFOSC compensation is enabled, include the temperature driver. */
    if (inst.srcClkLF === "LF RCOSC" && inst.enableLfoscComp) {
        tmpModules.push({
            name: "Temperature",
            moduleName: "/ti/drivers/Temperature"
        });
    }

    /* If LFOSC compensation is enabled, include LFOSC compensation profiles. */
    if (inst.srcClkLF === "LF RCOSC" && inst.enableLfoscComp) {
        tmpModules.push({
            name: "LF RCOSC Compensation Profiles",
            moduleName: "/ti/drivers/power/PowerLPF3LfoscCompProfiles"
        });
    }
    return tmpModules;
}

/*
 * ======== moduleInstances ========
 * This overrides the base versions since we want to add our
 * module instances tot he base ones.
 */
function moduleInstances(inst, $super) {

    let modInstances = new Array();

    if (inst.srcClkLF === "External LF clock") {
        modInstances.push({
            name: "extlfPinInstance",
            displayName: "Low Frequency Clock Pin",
            moduleName: "/ti/drivers/GPIO",
            collapsed: true,
            hidden: false,
            requiredArgs: {
                parentInterfaceName: "extlf",
                parentSignalName: "extlfPin",
                parentSignalDisplayName: "Low frequency clock pin",
                mode: "Input",
                doNotConfig: true,
                invert: false
            },
            args: {
                $name: "CONFIG_GPIO_EXTLF"
            }
        });
    }


    /* If LFOSC compensation is enabled, add LFOSC compensation profile instance
     * adder. Enabling the user to create as many profiles as they want.
     */
    if (inst.srcClkLF === "LF RCOSC" && inst.enableLfoscComp) {
        modInstances.push({
            name: "lfoscCompProfiles",
            displayName: "LFOSC Compensation Profiles",
            moduleName: "/ti/drivers/power/PowerLPF3LfoscCompProfiles",
            useArray: true, /* Create an instance adder */
            group: "lfoscCompConfig",
            collapsed: false
        });
    }

    return $super.moduleStatic.moduleInstances().concat(modInstances);
}

/*
 *  ======== pinmuxRequirements ========
 *  Returns peripheral pin requirements of the specified instance
 */
function pinmuxRequirements(inst) {
    let extlfBypass = {
        name: "extlfPin",    /* config script name */
        displayName: "External LF clock pin",   /* GUI name */
        interfaceNames: ["BYPASS"]      /* pinmux tool name */
    };
    let extlf = [];
    if (inst.srcClkLF === "External LF clock") {
        extlf.push({
            name: "extlf",
            displayName: "External LF clock",
            interfaceName: "LFXT",
            resources: [extlfBypass]
        });
    }

    return (extlf);
}

/*
 *  ======== extend ========
 */
function extend(base) {
    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* moduleInstances should be retained from base, but we are
     * also adding our own modules. This is needed so the shown module
     * instances update dynamically.
     */
    result.moduleStatic.moduleInstances =
        function (inst) {
            return moduleInstances(inst, base);
        };

    result.moduleStatic.pinmuxRequirements = pinmuxRequirements;
    /* concatenate device-specific configs */
    result.moduleStatic.config = base.moduleStatic.config.concat(devSpecific.moduleStatic.config);

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base CCFG module */
    extend: extend
};
