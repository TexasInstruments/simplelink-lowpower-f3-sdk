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
 *  ======== offchip_oad_layout.js ========
 *  Off-Chip OAD flash slot configuration utility.
 *
 *  Per-device input constants shared between ble_oad.syscfg.js and
 *  the example .syscfg file. Each entry is the single source of truth for
 *  all device-specific build configuration for Off-Chip OAD:
 *
 *    APP_HDR_ADDR  - Base address of the application image header in flash
 *
 *  Note: Off-chip OAD stores the downloaded image on external flash.
 */

"use strict";

/*
 * Per-device off-chip OAD configuration.
 * Key: substring matched against system.deviceData.deviceId
 */
const OFFCHIP_OAD_DEVICE_INPUTS = {
    "CC2340R2": {
        APP_HDR_ADDR:  0x6000,
        APP_SLOT_SIZE: 0x36000,     // Used to generate the signing command
    },
    "CC2340R5": {
        APP_HDR_ADDR:  0x6000,
        APP_SLOT_SIZE: 0x76000,     // Used to generate the signing command
    },
};

/*
 *  ======== getOffchipOadLayout ========
 *  Return the off-chip OAD configuration for the given device.
 *
 *  @param  {string} deviceId  - Device ID from system.deviceData.deviceId
 *  @returns {object|null}     - Config object, or null if device unsupported
 */
function getOffchipOadLayout(deviceId)
{
    for (let key of Object.keys(OFFCHIP_OAD_DEVICE_INPUTS)) {
        if (deviceId.includes(key)) {
            return OFFCHIP_OAD_DEVICE_INPUTS[key];
        }
    }
    return null;
}

/*
 *  ======== getCompilerDefines ========
 * Return the compiler defines for Off-Chip OAD based on the user-configured
 * header address.
 *
 * @param deviceId   - Device ID from system.deviceData.deviceId
 * @param appHdrAddr - User-configured APP_HDR_ADDR value
 * @returns          - Array of -D compiler define strings
 */
function getCompilerDefines(deviceId, appHdrAddr)
{
    return [
        "-DOAD_APP_OFFCHIP",
        "-DAPP_HDR_ADDR=0x" + appHdrAddr.toString(16),
    ];
}

/*
 *  ======== getLinkerSymbols ========
 * Return the linker symbol definitions for Off-Chip OAD based on the
 * user-configured header address.
 *
 * @param deviceId   - Device ID from system.deviceData.deviceId
 * @param appHdrAddr - User-configured APP_HDR_ADDR value
 * @returns          - Array of {name, value} linker symbol descriptors
 */
function getLinkerSymbols(deviceId, appHdrAddr)
{
    return [
        { name: "OAD_APP_OFFCHIP", value: 1 },
        { name: "APP_HDR_ADDR",    value: appHdrAddr },
    ];
}

/*
 *  ======== moduleInstances ========
 * Add NVS module instance required for Off-Chip OAD.
 * Creates one external NVS region linked to the board's external flash
 * hardware component.
 *
 * @param deviceId    - Device ID from system.deviceData.deviceId
 * @param hwComponent - Board hardware component for external flash
 *                      (e.g. system.deviceData.board.components.MX25R8035F)
 * @returns           - Array of module instance descriptors
 */
function moduleInstances(deviceId, hwComponent)
{
    const layout = getOffchipOadLayout(deviceId);
    if (!layout)
    {
        return [];
    }

    const nvsInstance = {
        name        : "offchipOadNvsExtSlot",
        displayName : "Off-Chip OAD External NVS",
        moduleName  : "/ti/drivers/NVS",
        group       : "oadConfig",
        hidden      : true,
        collapsed   : true,
        requiredArgs: {
            $name   : "CONFIG_NVSEXTERNAL",
            nvsType : "External"
        },
    };

    if (hwComponent)
    {
        nvsInstance.hardware = hwComponent;
    }

    return [nvsInstance];
}

exports = {
    OFFCHIP_OAD_DEVICE_INPUTS,
    getOffchipOadLayout,
    getCompilerDefines,
    getLinkerSymbols,
    moduleInstances,
};
