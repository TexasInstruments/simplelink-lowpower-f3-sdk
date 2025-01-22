/*
 * Copyright (c) 2019-2024, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== CCFGCC23X0.syscfg.js ========
 */

const Common = system.getScript("/ti/drivers/Common.js");


const MAX_PBLDRVTOR = 0xFFFFFFEF;
const MAX_SERIALIOCFGINDEX = 0x07;
const MAX_PIN_TRIGGER_DIO = 0x3F;
const MAX_CAP_ARRAY = 0x3F;
const MAX_TEMP_THRESHOLD = 125;
const MIN_TEMP_THRESHOLD = -50;

const moduleDesc = `
The CCFG area is a dedicated flash memory sector and must contain a
Customer Configuration section (CCFG) that is used by boot ROM and TI provided
drivers to configure the device. It starts at 0x4E020000 and has a size of 0x800
bytes This configuration is done by simply letting SysConfig generate the file
ti_devices_config.c and including it in the project.`;


let board = system.deviceData.board;
let defaultOverrideHfxtCapArray = false;
let hfxtDefaultParams = {
    "P0": 14.56160,
    "P1": -0.43677,
    "P2": -0.00853,
    "P3": 0.00010,
    "shift": 22,
    "hfxtCapArrayQ1" : 0,
    "hfxtCapArrayQ2" : 0
};

if(board !== undefined) {
    let crystal = board.components.HFXT;
    if(crystal !== undefined)
    {
        if (crystal.settings.overrideCapArray !== undefined)
        {
            defaultOverrideHfxtCapArray = crystal.settings.overrideCapArray;
        }
        hfxtDefaultParams = crystal.settings;
    }
}

let defaultpAppVtorStr = "resetVectors";

if (system.compiler == "iar")
{
    defaultpAppVtorStr = "__vector_table";
}

let devSpecific = {
    hfxtDefaultParams: hfxtDefaultParams,
    longDescription: moduleDesc,
    templates: {
        /* Contributes CRC symbols to linker file */
        "/ti/utils/build/GenMap.cmd.xdt": { modName: "/ti/devices/CCFG", getLinkerSyms: getLinkerSyms }
    },
    moduleStatic: {
        modules: modules,
        validate: validate,
        migrateLegacyConfiguration: migrateLegacyConfiguration,
        config: [
            {
                name: "srcClkLF",
                displayName: "Low Frequency Clock Source",
                description: "Low frequency clock source",
                longDescription: `When choosing the external clock, supply
a 31.25 kHz square wave, with a peak voltage of VDDS and an offset of VDDS/2 to the appropriate pin.`,
                readOnly: false,
                hidden: false,
                options: [
                    { name: "LF XOSC" },
                    { name: "LF RCOSC" },
                    { name: "External LF clock" }
                ],
                default: "LF XOSC"
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
                displayName: "HFXT Cap Array Q1 Value",
                description: "Set the high frequency oscillator cap-array Q1 value, changing the frequency offset",
                longDescription: `The cap-array Q1 value is used when turning on HFXT.
The crystal's frequency offset can be controlled by changing this value.
Q1 and Q2 should not differ by more than one step.`,
                displayFormat: { radix: "hex", bitSize: 2 },
                default: hfxtDefaultParams.hfxtCapArrayQ1,
                readOnly: false,
                hidden: !defaultOverrideHfxtCapArray
            },
            {
                name: "hfxtCapArrayQ2",
                displayName: "HFXT Cap Array Q2 Value",
                description: "Set the high frequency oscillator cap-array Q2 value, changing the frequency offset",
                longDescription: `The cap-array Q2 value is used when turning on HFXT.
The crystal's frequency offset can be controlled by changing this value.
Q1 and Q2 should not differ by more than one step.`,
                displayFormat: { radix: "hex", bitSize: 2 },
                default: hfxtDefaultParams.hfxtCapArrayQ1,
                readOnly: false,
                hidden: !defaultOverrideHfxtCapArray
            },
            {
                name: "voltageRegulator",
                displayName: "Voltage Regulator",
                description: "Choose between using the internal DCDC or GLDO voltage regulator",
                default: "DCDC",
                longDescription: `On the CC23X0 devices, the DCDC regulator is
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
            },
            {
                name: "enableInitialHfxtAmpComp",
                displayName: "Initial HFXT Amplitude Compensation",
                longDescription: `This enables the [__Initial HFXT Amplitude Compensation__][1] feature.

[1]: /drivers/doxygen/html/_power_c_c23_x0_8h.html#ti_drivers_PowerCC23X0_Initial_HFXT_Amplitude_Compensation "Initial HFXT Amplitude Compensation"
`,
                default: false
            },
            {
                displayName: "RF Temperature Compensation",
                longDescription: `Settings for temperature compensation of HFXT. This will enable temperature
compensation of the system clock (CLKSVT) and the RF reference clock (CLKREF) to improve the frequency
stability over temperature.`,
                config: [
                    {
                        name: "enableHFXTComp",
                        displayName: "RF Temperature Compensation",
                        description: "Compensate HFXT frequency for temperature during radio transmissions.",
                        longDescription: `This feature improves the accuracy of the CLKSVT and CLKREF over temperature.
This should only be enabled if the selected HFXT source is not accurate enough for the selected RF protocol.`,
                        default: false,
                        onChange: onChangeHFXT
                    },
                    {
                        name: "HFXTCompTempThreshold",
                        displayName: "HFXT Compensation Threshold",
                        description: `Perform compensation only above this temperature`,
                        longDescription: `HFXT will only be automatically compensated when the measured device
temperature exceeds this threshold (in degrees Celsius). This can help to reduce power consumption if temperature
compensation is not required below a certain temperature. The device is only required to compensate in the range of [-40, +125],
but this threshold can be set as low as -50. This can be done to ensure compensation will trigger correctly at low temperatures, close to -40.`,
                        default: -50,
                        range: [MIN_TEMP_THRESHOLD, MAX_TEMP_THRESHOLD],
                        hidden: true
                    },
                    {
                        name: "HFXTCompTempDelta",
                        displayName: "HFXT Compensation Delta",
                        description: `How much the temperature must change before compensation is performed`,
                        longDescription: `HFXT will be automatically compensated if the temperature drifts more than
this delta-value in either direction (in degrees Celsius). For example, if the temperature is measured to 30 degrees
when the device boots, but later drifts to 30 + delta or 30 - delta, then HFXT temperature compensation will be
performed, and the temperature setpoint updated accordingly.`,
                        default: 2,
                        hidden: true
                    },
                    {
                        name: "customHFXTCoeff",
                        displayName: "Custom HFXT Coefficients",
                        description: "Use Custome HFXT Temperature Coefficients",
                        longDescription: `If the ppm offset of the HFXT can be approximated by a third order polynomial
function of temperature (degrees Celsius), ppm(T) = P3*T^3 + P2*T^2 + P1*T + P0, where the coefficients
P3, P2, P1, and P0 are known, they can be supplied below. The default coefficients represent the characteristics of the
48 MHz crystal mounted on the LaunchPad.`,
                        default: false,
                        hidden: true,
                        onChange: onChangeHFXT
                    },
                    {
                        name: "HFXTCoefficientP0",
                        displayName: "HFXT Coefficient P0",
                        description: "HFXT Coefficient P0",
                        default: hfxtDefaultParams["P0"],
                        hidden: true
                    },
                    {
                        name: "HFXTCoefficientP1",
                        displayName: "HFXT Coefficient P1",
                        description: "HFXT Coefficient P1",
                        default: hfxtDefaultParams["P1"],
                        hidden: true
                    },
                    {
                        name: "HFXTCoefficientP2",
                        displayName: "HFXT Coefficient P2",
                        description: "HFXT Coefficient P2",
                        default: hfxtDefaultParams["P2"],
                        hidden: true
                    },
                    {
                        name: "HFXTCoefficientP3",
                        displayName: "HFXT Coefficient P3",
                        description: "HFXT Coefficient P3",
                        default: hfxtDefaultParams["P3"],
                        hidden: true
                    }
                ]
            },
            {
                displayName: "Boot Configuration",
                config: [
                    {
                        name: "pAppVtor",
                        description: "Application vector table address",
                        default: 0x00000000,
                        deprecated: true
                    },
                    {
                        name: "pAppVtorStr",
                        displayName: "Application Vector Table",
                        description: "App vector table symbol or address",
                        longDescription: `Pointer to application vector table. Used by bootloader upon exit
or invoked directly by boot sequence if neither user bootloader nor default bootloader is allowed.
This can either be a the C symbol name of the vector table, for example resetVectors, or an integer specifying the
address of the vector table, for example 2048 or 0x800.
0xFFFFFFFF: No user application vector table`,
                        default: defaultpAppVtorStr
                    },
                    {
                        name: "bldrSetting",
                        displayName: "Bootloader Configuration",
                        longDescription: `This configurable chooses whether to use default bootloader,
default bootloader with customer settings, or user-specific bootloader`,
                        default: "Default FCFG bootloader",
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
                        description: "Bootloader vector table address",
                        deprecated: true,
                        default: 0x00000000
                    },
                    {
                        name: "pBldrVtorStr",
                        displayName: "Bootloader Vector Table",
                        description: "Bldr vector table address or symbol",
                        longDescription: `This configurable sets the address of the user-specific bootloader.
This can either be a the C symbol name of the vector table, or an integer specifying the address of the vector table,
for example 2048 or 0x800.`,
                        hidden: true,
                        default: "0x00000000"
                    },
                    {
                        name: "bldrEnabled",
                        displayName: "Enable Serial Bootloader",
                        longDescription: `true: All serial bootloader commands are accepted.
false: All serial bootloader commands except BLDR_CMD_GET_STATUS are rejected.`,
                        hidden: true,
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
                        deprecated: true,
                        displayName: "Hardware Options 1",
                        description: "Hardware Options 1",
                        longDescription: `Value written to both the PMCTL:HWOPT0 and CLKCTL:HWOPT0 registers by ROM code
on PRODDEV at execution transfer from boot code/bootloader to application image`,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0xFFFFFFFF
                    },
                    {
                        name: "hwOpts1",
                        deprecated: true,
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
                        longDescription: `If not allowed, the SWD port will be disabled when the device boots into the
application in flash. Not applicable for CC2340R5 rev A`,
                        default: true
                    },
                    {
                        name: "allowEnergyTrace",
                        displayName: "Allow Energy Trace",
                        description: "Allow access to energy trace AP",
                        longDescription: `Allow access to energy trace AP (if present). Not supported by any CC23X0 devices`,
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
                        deprecated: true,
                        displayName: "Allow Tools Client Mode",
                        description: "Allow RAM-only tools client mode to be enabled by SACI.",
                        default: true
                    },
                    {
                        name: "allowFakeStby",
                        displayName: "Allow Fake Standby",
                        longDescription: `Allow fake standby. Controls setting of the
DBGSS:SPECIAL_AUTH_CLR.FAKESTBYEN_CLR register bit field done by ROM code.`,
                        default: true,
                        deprecated: true
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
                        deprecated: true,
                        displayName: "SACI Timeout Override",
                        description: "Override FCFG SACI timeout value",
                        longDescription: `This configurable chooses whether to use the FCFG SACI timeout value,
or override it with the Customer Configured SACI timeout value defined below.`,
                        default: false
                    },
                    {
                        name: "saciTimeoutExp",
                        deprecated: true,
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
from boot code/bootloader to application image. The register has sticky-0 bits.`,
                        displayFormat: { radix: "hex", bitSize: 32 },
                        default: 0xFFFFFFFF
                    },
                    {
                        name: "writeEraseProt_mainSectors32_255",
                        displayName: "Main Sectors 32-255",
                        description: "Sets write/erase protection for main sectors 32-255 (8 sectors/bit)",
                        longDescription: `Value is written to VIMS:WEPRB register by ROM code on PRODDEV at execution transfer
from boot code/bootloader to application image. The register has sticky-0 bits.`,
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
The User Record Macro must be defined in the User Record File to be a list of values to be placed in the User Record Area.
The CRC32 checksum for the User Record will automatically be calculated and inserted regardless of whether the User Record is enabled or not`,
                        default: false,
                        onChange: (inst, ui) => {
                            let setHidden = !(inst.enableUserRecord);
                            ui["userRecordMacro"].hidden = setHidden;
                            ui["userRecordFile"].hidden = setHidden;
                            /* Also apply this to userRecordCRC once it's no longer unconditionally applied */
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
                        default: true
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
            }
        ]
    }
};

/*!
 * ======== migrateLegacyConfiguration ========
 *  Migrate deprecated configurations to the current configurations
 */
function migrateLegacyConfiguration(inst)
{
    if (inst.pAppVtor != undefined)
    {
        inst.pAppVtorStr = "0x" + inst.pAppVtor.toString(16).toUpperCase();
    }
    if (inst.pBldrVtor != undefined)
    {
        inst.pBldrVtorStr = "0x" + inst.pBldrVtor.toString(16).toUpperCase();
    }
}

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
        { name: "CRC_CCFG_DEBUG_end", value: 0x4E0207FB }
    ];

    if (inst.$static.userRecordCRC) {
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
    ui["pBldrVtorStr"].hidden = (inst.bldrSetting != "User-specific bootloader");

    let setHidden = (inst.bldrSetting == "Any bootloader forbidden") || (inst.bldrSetting == "Default FCFG bootloader");
    ui["bldrEnabled"].hidden = setHidden;

    setHidden = setHidden || !(inst.bldrEnabled);
    ui["serialIoCfgIndex"].hidden = setHidden;
    ui["pinTriggerEnabled"].hidden = setHidden;

    setHidden = setHidden || !(inst.pinTriggerEnabled);
    ui["pinTriggerDio"].hidden = setHidden;
    ui["pinTriggerLevel"].hidden = setHidden;
}

/*
 *  ======== onChangeEnableHFXTComp ========
 *  onChange callback function for the enableHFXTComp config
 */
function onChangeHFXT(inst, ui) {
    let subState = (inst.enableHFXTComp == false);
    ui.customHFXTCoeff.hidden = subState;
    ui.HFXTCompTempThreshold.hidden = subState;
    ui.HFXTCompTempDelta.hidden = subState;
    subState = subState || (inst.customHFXTCoeff == false);
    ui.HFXTCoefficientP0.hidden = subState;
    ui.HFXTCoefficientP1.hidden = subState;
    ui.HFXTCoefficientP2.hidden = subState;
    ui.HFXTCoefficientP3.hidden = subState;

}

/*!
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  @param inst       - CCFG instance to be validated
 *  @param validation - Issue reporting object
 */
function validate(inst, validation) {

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

    if (Common.isCName(inst.pBldrVtorStr) == false)
    {
        let pBldrVtorInt = parseInt(inst.pBldrVtorStr);
        if (isNaN(pBldrVtorInt) || pBldrVtorInt > MAX_PBLDRVTOR || pBldrVtorInt < 0)
        {
            Common.logError(validation, inst, "pAppVtorStr",
            "Must be either a C symbol or an unsigned integer less than 0x" + (MAX_PBLDRVTOR + 1).toString(16).toUpperCase());
        }
    }

    if (inst.serialIoCfgIndex > MAX_SERIALIOCFGINDEX) {
        Common.logError(validation, inst, "serialIoCfgIndex",
            "Must be less than 0x" + (MAX_SERIALIOCFGINDEX + 1).toString(16));
    }

    if (inst.pinTriggerDio > MAX_PIN_TRIGGER_DIO) {
        Common.logError(validation, inst, "pinTriggerDio",
            "Must be less than 0x" + (MAX_PIN_TRIGGER_DIO + 1).toString(16));
    }

    if (!(inst.debugPwdHash.match(/^[a-fA-F0-9]{0,64}$/))) {
        Common.logError(validation, inst, "debugPwdHash",
            "Must be valid hex-formatted SHA256 hash");
    }

    if (!(inst.debugPwdId.match(/^[a-fA-F0-9]{0,16}$/))) {
        Common.logError(validation, inst, "debugPwdId",
            "Must be 64-bit hex-formatted value");
    }

    if (Common.isCName(inst.pAppVtorStr) == false)
    {
        let pAppVtorInt = parseInt(inst.pAppVtorStr);
        if (isNaN(pAppVtorInt) || pAppVtorInt > 0xFFFFFFFF || pAppVtorInt < 0)
        {
            Common.logError(validation, inst, "pAppVtorStr",
            "Must be either a C symbol or a 32-bit value");
        }
    }

    if (inst.writeEraseProt_mainSectors0_31 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "writeEraseProt_mainSectors0_31",
            "Must be 32-bit value");
    }

    if (inst.writeEraseProt_mainSectors32_255 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "writeEraseProt_mainSectors32_255",
            "Must be 32-bit value");
    }

    if (inst.chipEraseRetain_mainSectors0_31 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "chipEraseRetain_mainSectors0_31",
            "Must be 32-bit value");
    }

    if (inst.chipEraseRetain_mainSectors32_255 > 0xFFFFFFFF) {
        Common.logError(validation, inst, "chipEraseRetain_mainSectors32_255",
            "Must be 32-bit value");
    }

    if (inst.HFXTCompTempDelta < 1) {
        Common.logError(validation, inst, "HFXTCompTempDelta",
            "Must be an integer greater than 0");
    }
}

/*
 *  ======== modules ========
 */
function modules(inst) {
    let tmpModules = [];

    /* If HFXT compensation is enabled, include the temperature driver. */
    if (inst.enableHFXTComp) {
        tmpModules.push({
            name: "Temperature",
            moduleName: "/ti/drivers/Temperature"
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

    let pinInstances = [];
    if (inst.srcClkLF === "External LF clock") {
        pinInstances.push({
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
    return $super.moduleStatic.moduleInstances().concat(pinInstances);
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