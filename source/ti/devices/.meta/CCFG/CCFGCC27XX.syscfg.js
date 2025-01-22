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
const MAX_READPROT_MAINSECTORS = 0x0F;
const MAX_READPROT_CCFGSECTOR = 0x3F;
const MAX_PIN_TRIGGER_DIO = 0x3F; // TODO: Find correct value for CC27XX
const MAX_CAP_ARRAY = 0x3F;
const MAX_TEMP_THRESHOLD = 125;
const MIN_TEMP_THRESHOLD = -50;
const SCFG_UNDEFINED_SLOT_ADDRESS = 0xFFFFFFFF;
const SCFG_UNDEFINED_SLOT_LENGTH = 0xFFFFFFFF;
const SCFG_BOOT_SEED_DISABLED = 0xFF;
const SECTORSIZE = 0x800;
const FLASHSIZE = 0x100000;

const moduleDesc = `
The device has 2 dedicated configuration areas in flash that must contain a valid configuration

* The Customer Configuration Area (CCFG) is used by boot ROM and TI provided drivers to configure the device. It starts at 0x4E020000 and has a size of 0x800 bytes.
* The Security Configuration Area (SCFG) must contain a valid Security Configuration. It starts at 0x4E040000 and has a size of 0x400 bytes.

All of these configurations are done by simply letting SysConfig generate file ti_devices_config.c and including it in the project.`;

let board = system.deviceData.board;
let tzEnabled = system.modules["/ti/utils/TrustZone"]; /* UI callbacks cannot access system modules. Preload into variable */
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
                default: hfxtDefaultParams.hfxtCapArrayQ1,
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
                default: hfxtDefaultParams.hfxtCapArrayQ2,
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
                        default: true,
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
                        range: [MIN_TEMP_THRESHOLD, MAX_TEMP_THRESHOLD]
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
                        hidden: false
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
                        hidden: false,
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
                name: "lfoscCompConfig",
                displayName: "LFOSC Compensation",
                longDescription: `Settings for LFOSC compensation. When LFOSC Compensation is enabled, HFXT will
periodically be turned on to calibrate the LFOSC.`,
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
                        displayName: "Customer Configuration Permissions",
                        config: [
                            {
                                name: "allowDebugPort",
                                displayName: "Allow SWD Debug Port",
                                description: "Allow access to SWD Debug Port",
                                longDescription: `If not allowed, the SWD port will be disabled when the device boots into the application in flash.
Must match the Secure Configuration Permissions to be effective.`,
                                default: true
                            },
                            {
                                name: "allowEnergyTrace",
                                displayName: "Allow Energy Trace",
                                description: "Allow access to energy trace AP",
                                longDescription: `Allow access to energy trace AP (if present). Must match the Secure Configuration Permissions to be effective.`,
                                hidden: true,
                                default: true
                            },
                            {
                                name: "allowFlashVerify",
                                displayName: "Allow Flash Verify",
                                description: "Allow Flash Verify by SACI.",
                                longDescription: "Allow flash verify by SACI. Must match the Secure Configuration Permissions to be effective.",
                                default: true
                            },
                            {
                                name: "allowFlashProgram",
                                displayName: "Allow Flash Program",
                                description: "Allow Flash Program by SACI",
                                longDescription: "Allow Flash Program by SACI. Must match the Secure Configuration Permissions to be effective.",
                                default: true
                            },
                            {
                                name: "allowChipErase",
                                displayName: "Allow Chip Erase",
                                description: "Allow Chip Erase by SACI",
                                longDescription: "Allow Chip Erase by SACI. Must match the Secure Configuration Permissions to be effective.",
                                default: true
                            },
                            {
                                name: "allowToolsClientMode",
                                deprecated: true,
                                displayName: "Allow Tools Client Mode",
                                description: "Allow RAM-only tools client mode to be enabled by SACI.",
                                longDescription: "Allow RAM-only tools client mode to be enabled by SACI. Must match the Secure Configuration Permissions to be effective.",
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
                                longDescription: "Allow return to factory by SACI. Must match the Secure Configuration Permissions to be effective.",
                                default: true
                            },
                            {
                                name: "allowMainAppErase",
                                displayName: "Allow Main App Erase",
                                description: "Allow Main App Erase by SACI.",
                                longDescription: "Allow Main App Erase by SACI. Must match the Secure Configuration Permissions to be effective.",
                                default: true
                            }
                        ]
                    },
                    {
                        displayName: "Secure Configuration Permissions",
                        config: [
                            {
                                name: "scfgAllowDebugPort",
                                displayName: "Allow SWD Debug Port",
                                description: "Allow access to SWD Debug Port",
                                longDescription: `If not allowed, it will not be possible to access any part of the debug system. Must match the Customer Configuration Permissions to be effective.`,
                                default: true
                            },
                            {
                                name: "scfgAllowEnergyTrace",
                                displayName: "Allow Energy Trace",
                                description: "Allow access to energy trace AP",
                                longDescription: `Allow access to energy trace AP (if present). Must match the Customer Configuration Permissions to be effective.`,
                                hidden: true,
                                default: true
                            },
                            {
                                name: "scfgAllowFlashVerify",
                                displayName: "Allow Flash Verify",
                                description: "Allow Flash Verify by SACI.",
                                longDescription: "Allow flash verify by SACI. Must match the Customer Configuration Permissions to be effective.",
                                default: true
                            },
                            {
                                name: "scfgAllowFlashProgram",
                                displayName: "Allow Flash Program",
                                description: "Allow Flash Program by SACI",
                                longDescription: "Allow Flash Program by SACI. Must match the Customer Configuration Permissions to be effective.",
                                default: true
                            },
                            {
                                name: "scfgAllowChipErase",
                                displayName: "Allow Chip Erase",
                                description: "Allow Chip Erase by SACI",
                                longDescription: "Allow Chip Erase by SACI. Must match the Customer Configuration Permissions to be effective.",
                                default: true
                            },
                            {
                                name: "scfgAllowToolsClientMode",
                                displayName: "Allow Tools Client Mode",
                                description: "Allow RAM-only tools client mode to be enabled by SACI.",
                                longDescription: "Allow RAM-only tools client mode to be enabled by SACI. Must match the Customer Configuration Permissions to be effective.",
                                default: true
                            },
                            {
                                name: "scfgAllowReturnToFactory",
                                displayName: "Allow Return To Factory",
                                description: "Allow return to factory by SACI.",
                                longDescription: "Allow return to factory by SACI. Must match the Customer Configuration Permissions to be effective.",
                                default: true
                            },
                            {
                                name: "scfgAllowMainAppErase",
                                displayName: "Allow Main App Erase",
                                description: "Allow Main App Erase by SACI.",
                                longDescription: "Allow Main App Erase by SACI. Must match the Customer Configuration Permissions to be effective.",
                                default: true
                            }
                        ]
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
                            { name: "Non-Invasive only"},
                            { name: "Require debug authentication" },
                            { name: "Debug not allowed" }
                        ]
                    },
                    {
                        name: "debugAllowBldr",
                        displayName: "Allow Bootloader Debugging",
                        default: true,
                        deprecated  : true
                    },
                    {
                        name: "debugPwdId",
                        displayName: "Password ID",
                        description: "Hex-formatted password ID",
                        longDescription: `64-bit customer-defined password ID readable through SACI command (may be used by to calculate or look up debug password)`,
                        hidden: true,
                        deprecated: true,
                        default: "0101020305080D15"
                    },
                    {
                        name: "debugPwdHash",
                        displayName: "Password Hash",
                        description: "Hex-formatted SHA256-hash of user-supplied password",
                        hidden: true,
                        deprecated: true,
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
                        longDescription: "Optional customer public key hash for authenticating HSM updates.",
                        default: "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
                    },
                    {
                        name: "authMethod",
                        displayName: "Authentication Method",
                        description: "Authentication method for image verification",
                        longDescription: "Authentication method for image verification. No Authentication: Secure Boot will not perform any validation. Signature: Secure Boot will perform signature verification every time. Hash Lock: Secure Boot will perform a signature verification once, and a sha256 check on the image at subsequent times.",
                        default: "No Authentication",
                        options: [
                            { name: "No Authentication" },
                            { name: "Signature" },
                            { name: "Hash Lock" }
                        ],
                        onChange: onChangeAuthMethod
                    },
                    {
                        name: "authAlg",
                        displayName: "Authentication Algorithm",
                        description: "Authentication Algorithm for image verification",
                        longDescription: "Authentication algorithm for image verification. Supported algorithms are RSA 3K PKCS, ECDSA P256, and ECDSA P521.",
                        default: "RSA 3K PKCS",
                        hidden: true,
                        options: [
                            { name: "RSA 3K PKCS" },
                            { name: "ECDSA P256" },
                            { name: "ECDSA P521" }
                        ]
                    },
                    {
                        name: "mode",
                        displayName: "Update Mode",
                        description: "Application update mode",
                        longDescription: "Application updates will be performed based on the selected mode. In Overwrite mode, the current image will be overwritten with the new image. In XIP Revert Enabled mode, the new image will be executed, but it is possible to revert to the previous image if the new image becomes invalid. In XIP Revert Disabled mode, when a new image is accepted, the previous image will be deleted.",
                        default: "Overwrite",
                        hidden: true,
                        options: [
                            { name: "Overwrite" },
                            { name: "XIP Revert Enabled" },
                            { name: "XIP Revert Disabled" }
                        ],
                        onChange: onChangeMode
                    },
                    {
                        name: "ssbEnabled",
                        displayName: "Secondary Secure Bootloader Enabled",
                        description: "Enables/Disables Secondary Secure Bootloader support",
                        longDescription: "Enables/Disables Secondary Secure Bootloader support. The application must send a command to ROM using HAPI to request that ROM boots the Secondary Secure Bootloader. Refer to the Technical Reference Manual (TRM) for usage details.",
                        hidden: true,
                        default: false,
                        onChange: onChangeSsbEnabled
                    },
                    {
                        name: "bootSeedOffset",
                        displayName: "Boot Seed",
                        description: "Enables/Disabled Boot Seed at boot time",
                        longDescription: "Boot Seed offset from RAM base, in multiples of 16-byte blocks, up to an offset of 208 bytes (13 16-byte blocks). Min: 0, Max: 0x0D (13), Disabled: 0xFF.",
                        displayFormat: "hex",
                        default: 0xff,
                        hidden: true
                    },
                    {
                        name: "imgTypeSingleOvrWrt",
                        displayName: "Image Type",
                        description: "Image type of build target",
                        longDescription: "Image type associated with the current build target. APP: Application, SSB: Secondary Secure Bootloader.",
                        default: "APP",
                        hidden: true,
                        options: [
                            { name: "APP" },
                            { name: "SSB" }
                        ]
                    },
                    {
                        name: "imgTypeSingleXIP",
                        displayName: "Image Type",
                        description: "Image type of build target",
                        longDescription: "Image type associated with the current build target. APP for Primary: Application to be executed from Primary slot, APP for Secondary: Application to be executed from Secondary slot, SSB: Secondary Secure Bootloader.",
                        default: "APP for Primary",
                        hidden: true,
                        options: [
                            { name: "APP for Primary" },
                            { name: "APP for Secondary" },
                            { name: "SSB" }
                        ]
                    },
                    {
                        name: "imgTypeDual",
                        displayName: "Image Type",
                        description: "Image type of build target",
                        longDescription: "Image type associated with the current build target. APP 0: Application for Primary Secure slot, APP 1: Application for Primary Non-Secure slot, SSB: Secondary Secure Bootloader.",
                        default: "APP 0",
                        hidden: true,
                        options: [
                            { name: "APP 0" },
                            { name: "APP 1" },
                            { name: "SSB" }
                        ]
                    },
                    {
                        name: "secCnt",
                        displayName: "Security Counter",
                        description: "Security Counter for Antirollback validation",
                        longDescription: "Security Counter associated to the current image to perform Antirollback validation.",
                        default: 0,
                        hidden: true
                    },
                    {
                        name: "hdrSize",
                        displayName: "Secure Boot Header size",
                        description: "Secure Boot Header size.",
                        readOnly: true,
                        displayFormat: "hex",
                        default : 0x80,
                        hidden: true
                    },
                    {
                        name: "version",
                        displayName: "Version",
                        description: "Version associated with this image.",
                        default: "1.0",
                        hidden: true
                    },
                    {
                        name: "privKey",
                        displayName: "Private Key",
                        description: "Private key to sign the target image",
                        longDescription: "Private key to be used to sign the target image. The key type must match the Authentication Algorithm selected.",
                        hidden: true,
                        default: "",
                        fileFilter: ".pem"
                    },
                    {
                        displayName: "Secondary Secure Bootloader",
                        config: [
                            {
                                name: "ssbStart",
                                displayName: "Start",
                                description: "Slot start address",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            },
                            {
                                name: "ssbLen",
                                displayName: "Length",
                                description: "Slot length in bytes",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            }
                        ]
                    },
                    {
                        displayName: "Primary",
                        config: [
                            {
                                name: "prim0StartSingle",
                                displayName: "Start",
                                description: "Slot start address",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            },
                            {
                                name: "prim0LenSingle",
                                displayName: "Length",
                                description: "Slot length in bytes",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            }
                        ]
                    },
                    {
                        displayName: "Primary Secure",
                        config: [
                            {
                                name: "prim0StartSecure",
                                displayName: "Start",
                                description: "Slot start address",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            },
                            {
                                name: "prim0LenSecure",
                                displayName: "Length",
                                description: "Slot length in bytes",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            }
                        ]
                    },
                    {
                        displayName: "Primary Non-Secure",
                        config: [
                            {
                                name: "prim1Start",
                                displayName: "Start",
                                description: "Slot start address",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            },
                            {
                                name: "prim1Len",
                                displayName: "Length",
                                description: "Slot length in bytes",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            }
                        ]
                    },
                    {
                        displayName: "Secondary",
                        config: [
                            {
                                name: "sec0StartSingle",
                                displayName: "Start",
                                description: "Slot start address",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            },
                            {
                                name: "sec0LenSingle",
                                displayName: "Length",
                                description: "Slot length in bytes",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            }
                        ]
                    },
                    {
                        displayName: "Secondary Secure",
                        config: [
                            {
                                name: "sec0StartSecure",
                                displayName: "Start",
                                description: "Slot start address",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            },
                            {
                                name: "sec0LenSecure",
                                displayName: "Length",
                                description: "Slot length in bytes",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            }
                        ]
                    },
                    {
                        displayName: "Secondary Non-Secure",
                        config: [
                            {
                                name: "sec1Start",
                                displayName: "Start",
                                description: "Slot start address",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            },
                            {
                                name: "sec1Len",
                                displayName: "Length",
                                description: "Slot length in bytes",
                                displayFormat: "hex",
                                hidden: true,
                                default      : 0xFFFFFFFF
                            }
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
        { name: "CRC_CCFG_DEBUG_end", value: 0x4E0207FB },

        { name: "CRC_SCFG_begin", value: 0x4e040000 },
        { name: "CRC_SCFG_end", value: 0x4e0400E3 }
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

/*
 *  ======== onChangeMode ========
 *  onChange callback function for Secure Boot mode config
 */
function onChangeMode(inst, ui) {
    /* Prevent UI calling this function asynchronously when sysconfig file is loaded */
    if (inst.authMethod == "No Authentication") {
        return;
    }

    if (!tzEnabled) {
        if (inst.mode == "Overwrite") {
            ui.imgTypeSingleOvrWrt.hidden = false;
            ui.imgTypeSingleXIP.hidden = true;
            ui.imgTypeDual.hidden = true;
        } else {
            ui.imgTypeSingleOvrWrt.hidden = true;
            ui.imgTypeSingleXIP.hidden = false;
            ui.imgTypeDual.hidden = true;
        }

        ui.prim0StartSingle.hidden = false;
        ui.prim0LenSingle.hidden = false;
        ui.prim0StartSecure.hidden = true;
        ui.prim0LenSecure.hidden = true;
        ui.sec0StartSingle.hidden = false;
        ui.sec0LenSingle.hidden = false;
        ui.sec0StartSecure.hidden = true;
        ui.sec0LenSecure.hidden = true;

        ui.prim1Start.hidden = true;
        ui.prim1Len.hidden = true;
        ui.sec1Start.hidden = true;
        ui.sec1Len.hidden = true;
    } else {
        ui.imgTypeSingleOvrWrt.hidden = true;
        ui.imgTypeSingleXIP.hidden = true;
        ui.imgTypeDual.hidden = false;

        ui.prim0StartSingle.hidden = true;
        ui.prim0LenSingle.hidden = true;
        ui.prim0StartSecure.hidden = false;
        ui.prim0LenSecure.hidden = false;
        ui.sec0StartSingle.hidden = true;
        ui.sec0LenSingle.hidden = true;
        ui.sec0StartSecure.hidden = false;
        ui.sec0LenSecure.hidden = false;

        ui.prim1Start.hidden = false;
        ui.prim1Len.hidden = false;
        ui.sec1Start.hidden = false;
        ui.sec1Len.hidden = false;
    }
}

/*
 *  ======== onChangeSsbEnabled ========
 *  onChange callback function for Secondary Secure Bootloader
 */
function onChangeSsbEnabled(inst, ui) {
    /* Prevent UI calling this function asynchronously when sysconfig file is loaded */
    if (inst.authMethod == "No Authentication") {
        return;
    }

    if (inst.ssbEnabled) {
        ui.ssbStart.hidden = false;
        ui.ssbLen.hidden = false;
    } else {
        ui.ssbStart.hidden = true;
        ui.ssbLen.hidden = true;
    }
}

/*
 *  ======== onChangeAuthMethod ========
 *  onChange callback function for Secure Boot authMethod config
 */

function onChangeAuthMethod(inst, ui) {
    let setHidden = false;

    if (inst.authMethod == "No Authentication") {
        setHidden = true;
    } else {
        setHidden = false;
    }

    if (setHidden) {
        ui.imgTypeSingleOvrWrt.hidden = setHidden;
        ui.imgTypeSingleXIP.hidden = setHidden;
        ui.imgTypeDual.hidden = setHidden;
        ui.prim0StartSingle.hidden = setHidden;
        ui.prim0LenSingle.hidden = setHidden;
        ui.prim0StartSecure.hidden = setHidden;
        ui.prim0LenSecure.hidden = setHidden;
        ui.sec0StartSingle.hidden = setHidden;
        ui.sec0LenSingle.hidden = setHidden;
        ui.sec0StartSecure.hidden = setHidden;
        ui.sec0LenSecure.hidden = setHidden;
        ui.prim1Start.hidden = setHidden;
        ui.prim1Len.hidden = setHidden;
        ui.sec1Start.hidden = setHidden;
        ui.sec1Len.hidden = setHidden;
    } else {
        onChangeMode(inst, ui);
    }

    ui.authAlg.hidden = setHidden;
    ui.mode.hidden = setHidden;
    ui.bootSeedOffset.hidden = setHidden;
    ui.ssbEnabled.hidden = setHidden;
    ui.privKey.hidden = setHidden;
    ui.secCnt.hidden = setHidden;
    ui.hdrSize.hidden = setHidden;
    ui.version.hidden = setHidden;

    if (setHidden) {
        ui.ssbStart.hidden = setHidden;
        ui.ssbLen.hidden = setHidden;
    } else {
        onChangeSsbEnabled(inst, ui);
    }
}

/*
 *  ======== checkSlotStart ========
 *  checkSlotStart function to perform sanity check on configured slots address field
 */
function checkSlotStart(inst, validation, field, name)
{
    if (field != SCFG_UNDEFINED_SLOT_ADDRESS) {
        if (0 != (field % SECTORSIZE))
        {
            Common.logError(validation, inst, name,
                "Slot start address must be aligned to a 0x" + SECTORSIZE.toString(16) + " boundary");
        }
    }
}

/*
 *  ======== checkSlotLength ========
 *  checkSlotLength function to perform sanity check on configured slots length field
 */
function checkSlotLength(inst, validation, field, name)
{

    if ((field != SCFG_UNDEFINED_SLOT_LENGTH) && (0 != (field % SECTORSIZE))) {
        Common.logError(validation, inst, name,
            "Slot length must be a multiple of 0x" + SECTORSIZE.toString(16));
    }
}

/*
 *  ======== checkSlotParameters ========
 *  checkSlotParameters function to perform sanity check on slot parameters (start and length)
 */
function checkSlotParameters (inst, validation, name_start, start, name_len, len)
{
    if (IsSlotDefined(start, len)) {
        checkSlotStart(inst, validation, start, name_start);
        if ((start != SCFG_UNDEFINED_SLOT_ADDRESS) && (len == SCFG_UNDEFINED_SLOT_LENGTH)) {
            Common.logError(validation, inst, name_len,
                "Slot length must be defined if its start address is defined");
        }

        checkSlotLength(inst, validation, len, name_len);
        if ((start == SCFG_UNDEFINED_SLOT_ADDRESS) && (len != SCFG_UNDEFINED_SLOT_LENGTH)) {
            Common.logError(validation, inst, name_start,
                "Slot start must be defined if its length is defined");
        }

        if ((start + len) > FLASHSIZE) {
            Common.logError(validation, inst, name_start,
                "Slot start address + length must be less than max flash size of 0x" + FLASHSIZE.toString(16));
            Common.logError(validation, inst, name_len,
                "Slot start address + length must be less than max flash size of 0x" + FLASHSIZE.toString(16));
        }
    }
}

/*
 *  ======== checkMode ========
 *  checkMode function to perform sanity check on allowed Secure Boot mode configuration
 */
function checkMode(inst, validation)
{

    if ((inst.mode == "XIP Revert Enabled") && (inst.authMethod == "Hash Lock")) {
        Common.logError(validation, inst, "mode",
            "Hash Lock is not allowed for XIP Revert Enabled");
    }

    if ((inst.mode != "Overwrite") && tzEnabled) {
        Common.logError(validation, inst, "mode",
            "XIP is not allowed for TrustZone Enabled mode, only Overwrite");
    }
}

/*
 *  ======== checkBootSeedOffset ========
 *  checkBootSeedOffset function to perform sanity check Boot Seed configuration
 */
function checkBootSeedOffset(inst, validation)
{

    if (SCFG_BOOT_SEED_DISABLED != inst.bootSeedOffset) {
        if (inst.bootSeedOffset < 0) {
            Common.logError(validation, inst, "bootSeedOffset",
                "Boot Seed offset min value is 0");
        } else if (inst.bootSeedOffset > 13) {
            Common.logError(validation, inst, "bootSeedOffset",
                "Boot Seed offset max value is 13");
        }
    }
}

/*
 *  ======== CheckRegionOverlap ========
 *  Helper function to detect slot overlap
 */
function CheckRegionOverlap (start_a, end_a,
                             start_b, end_b)
{
    if ((start_b >= start_a) && (start_b <= end_a)) {
        return true;
    } else if ((end_b >= start_a) && (end_b <= end_a)) {
        return true;
    }

    if ((start_a >= start_b) && (start_a <= end_b)) {
        return true;
    } else if ((end_a > start_b) && (end_a <= end_b)) {
        return true;
    }

    return false;
}

/*
 *  ======== IsSlotDefined ========
 *  Helper function to detect if slot start and length are defined
 */
function IsSlotDefined (start, len)
{
    if ((start != SCFG_UNDEFINED_SLOT_ADDRESS) && (len != SCFG_UNDEFINED_SLOT_LENGTH)) {
        return true;
    }

    return false;
}

/*
 *  ======== checkSlotConfiguration ========
 *  checkSlotConfiguration function to validate that slots do not overlap
 *  (if slot is valid). Although ROM Secure Boot allows for scattered slots,
 *  here the following order is enforced:
 *  - SSB
 *  - Primary 0
 *  - Primary 1
 *  - Secondary 0
 *  - Secondary 1
 *
 */
function checkSlotConfiguration(inst, validation)
{
    if (tzEnabled) { /* The following is just to throw an error if TrustZone module is */
        Common.logError(validation, inst, "authMethod", "The combination of TrustZone and Secure Boot is not yet supported. Before TrustZone can be enabled, this combination must be properly handled. Please reach out to R&D");
    }

    let slots = [
        {cond: inst.ssbEnabled, name: "Secondary Secure Bootloader", name_start: "ssbStart",   name_len: "ssbLen",   start: inst.ssbStart, len: inst.ssbLen},
        {cond: !tzEnabled, name: "Primary",  name_start: "prim0StartSingle", name_len: "prim0LenSingle", start: inst.prim0StartSingle, len: inst.prim0LenSingle},
        {cond: tzEnabled, name: "Primary Secure",  name_start: "prim0StartSecure", name_len: "prim0LenSecure", start: inst.prim0StartSecure, len: inst.prim0LenSecure},
        {cond: tzEnabled, name: "Primary Non-Secure",  name_start: "prim1Start", name_len: "prim1Len", start: inst.prim1Start, len: inst.prim1Len},
        {cond: !tzEnabled, name: "Secondary",  name_start: "sec0StartSingle", name_len: "sec0LenSingle", start: inst.sec0StartSingle, len: inst.sec0LenSingle},
        {cond: tzEnabled, name: "Secondary Secure",  name_start: "sec0StartSecure", name_len: "sec0LenSecure", start: inst.sec0StartSecure, len: inst.sec0LenSecure},
        {cond: tzEnabled, name: "Secondary Non-Secure",  name_start: "sec1Start", name_len: "sec1Len", start: inst.sec1Start, len: inst.sec1Len}
    ];
    let start_a, end_a, start_b, end_b;

    for (let i=0; i < slots.length; i++) {
        if (slots[i].cond) {

            checkSlotParameters(inst, validation, slots[i].name_start, slots[i].start, slots[i].name_len, slots[i].len);

            for (let j=i+1; j < slots.length; j++) {
                if (slots[j].cond &&
                    IsSlotDefined(slots[i].start, slots[i].len) &&
                    IsSlotDefined(slots[j].start, slots[j].len)) {

                    start_a = slots[i].start;
                    end_a = start_a + slots[i].len - 1;
                    start_b = slots[j].start;
                    end_b = start_b + slots[j].len - 1;

                    if (true == CheckRegionOverlap(start_a, end_a, start_b, end_b)) {
                        Common.logError(validation, inst, slots[i].name_start, slots[i].name + " overlaps with " + slots[j].name);
                        Common.logError(validation, inst, slots[i].name_len, slots[i].name + " overlaps with " + slots[j].name);
                        Common.logError(validation, inst, slots[j].name_start, slots[j].name + " overlaps with " + slots[i].name);
                        Common.logError(validation, inst, slots[j].name_len, slots[j].name + " overlaps with " + slots[i].name);
                    }

                    if (slots[i].start >= slots[j].start) {
                        Common.logError(validation, inst, slots[i].name_start, slots[i].name + " must be located before " + slots[j].name);
                        Common.logError(validation, inst, slots[i].name_len, slots[i].name + " must be located before " + slots[j].name);
                        Common.logError(validation, inst, slots[j].name_start, slots[j].name + " must be located after " + slots[i].name);
                        Common.logError(validation, inst, slots[j].name_len, slots[j].name + " must be located after " + slots[i].name);
                    }
                }
            }
        }
    }

    if (inst.ssbEnabled) {
        if (inst.ssbStart == SCFG_UNDEFINED_SLOT_ADDRESS) {
            Common.logError(validation, inst, "ssbStart", "Start must be defined if Secondary Secure Bootloader is Enabled");
        }
        if (inst.ssbLen == SCFG_UNDEFINED_SLOT_LENGTH) {
            Common.logError(validation, inst, "ssbLen", "Length must be defined if Secondary Secure Bootloader is Enabled");
        }
    }

    if (tzEnabled) {
        if (inst.prim0StartSecure == SCFG_UNDEFINED_SLOT_ADDRESS) {
            Common.logError(validation, inst, "prim0StartSecure", "Start must be defined in TrustZone Enabled mode");
        }
        if (inst.prim0LenSecure == SCFG_UNDEFINED_SLOT_LENGTH) {
            Common.logError(validation, inst, "prim0LenSecure", "Length must be defined in TrustZone Enabled mode");
        }
        if (inst.prim1Start == SCFG_UNDEFINED_SLOT_ADDRESS) {
            Common.logError(validation, inst, "prim1Start", "Start must be defined in TrustZone Enabled mode");
        }
        if (inst.prim1Len == SCFG_UNDEFINED_SLOT_LENGTH) {
            Common.logError(validation, inst, "prim1Len", "Length must be defined in TrustZone Enabled mode");
        }
        if (inst.sec1Len != SCFG_UNDEFINED_SLOT_LENGTH) {
            if (inst.sec1Len != inst.prim1Len) {
                Common.logError(validation, inst, "sec1Len", "Secondary Non-Secure slot length must match Primary Non-Secure slot length");
            }
        }
    }

    if (inst.mode == "Overwrite") {
        if (tzEnabled) {
            if (inst.sec0LenSecure != SCFG_UNDEFINED_SLOT_LENGTH) {
                if (inst.sec0LenSecure != inst.prim0LenSecure) {
                    Common.logError(validation, inst, "sec0LenSecure", "Secondary Secure slot length must match Primary Secure slot length");
                }
            }
        } else {
            if (inst.sec0LenSingle != SCFG_UNDEFINED_SLOT_LENGTH) {
                if (inst.sec0LenSingle != inst.prim0LenSingle) {
                    Common.logError(validation, inst, "sec0LenSingle", "Secondary slot length must match Primary slot length");
                }
            }
        }
    }
}

/*
 *  ======== checkPrivateKey ========
 *  checkPrivateKey funtion to validate that a private key file has been specified
 */
function checkPrivateKey (inst, validation)
{
    if (inst.authMethod != "No Authentication") {
        if (inst.privKey == "") {
            Common.logError(validation, inst, "privKey", "A private key must be specified if Secure Boot is enabled");
        }
    }
}

/*!
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  @param inst       - CCFG instance to be validated
 *  @param validation - Issue reporting object
 */
function validate(inst, validation) {

    checkBootSeedOffset(inst, validation);
    checkMode(inst, validation);
    checkSlotConfiguration(inst, validation);
    checkPrivateKey(inst, validation);

    if (inst.debugAuthorization == "Require debug authentication") {
        Common.logError(validation, inst, "debugAuthorization",
            "Public Key based authentication is not implemented");
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

    /* If LFOSC compensation is enabled, include the temperature driver. */
    if (inst.enableHFXTComp || (inst.srcClkLF === "LF RCOSC" && inst.enableLfoscComp)) {
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
