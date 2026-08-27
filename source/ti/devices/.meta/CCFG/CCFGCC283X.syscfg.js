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
 *  ======== CCFGCC283X.syscfg.js ========
 */

const Common = system.getScript("/ti/drivers/Common.js");

const moduleDesc = `
The device has a dedicated configuration areas in flash that must contain a valid configuration

* The Customer Configuration Area (CCFG) is used by boot ROM and TI provided drivers to configure the device. It starts at 0x4E020000 and has a size of 0x800 bytes.

All of these configurations are done by simply letting SysConfig generate file ti_devices_config.c and including it in the project.`;

let defaultpAppVtorStr = "resetVectors";

if (system.compiler == "iar")
{
    defaultpAppVtorStr = "__vector_table";
}

let devSpecific = {
    longDescription: moduleDesc,
    templates: {
        /* Contributes CRC symbols and Secure Boot definitions to linker file */
        "/ti/utils/build/GenMap.cmd.xdt": {
            modName: "/ti/devices/CCFG",
            getLinkerDefs: getLinkerDefs,
            getLinkerSyms: getLinkerSyms
        }
    },
    moduleStatic: {
        validate: validate,
        config: [
            {
                displayName: "Boot Configuration",
                config: [
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

        { name: "CRC_CCFG_BOOT_CFG_begin", value: 0x4E020000 },
        { name: "CRC_CCFG_BOOT_CFG_end", value: 0x4E020013 },

        { name: "CRC_CCFG_begin", value: 0x4E020018 },
        { name: "CRC_CCFG_end", value: 0x4E02011B }
    ];

    return linkerSyms;
}

/*!
 * ======== getLinkerDefs ========
 *  Used by GenMap to define linker definitions
 */
function getLinkerDefs()
{
    return [];
}

/*!
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  @param inst       - CCFG instance to be validated
 *  @param validation - Issue reporting object
 */
function validate(inst, validation)
{
    if (Common.isCName(inst.pAppVtorStr) == false)
    {
        let pAppVtorInt = parseInt(inst.pAppVtorStr);
        if (isNaN(pAppVtorInt) || pAppVtorInt > 0xFFFFFFFF || pAppVtorInt < 0)
        {
            Common.logError(validation, inst, "pAppVtorStr",
            "Must be either a C symbol or a 32-bit value");
        }
    }
}


/*
 *  ======== extend ========
 */
function extend(base) {
    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    result.moduleStatic.moduleInstances = base.moduleStatic.moduleInstances;

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
