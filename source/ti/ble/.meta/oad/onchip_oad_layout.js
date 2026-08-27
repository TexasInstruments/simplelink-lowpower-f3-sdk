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
 *  ======== onchip_oad_layout.js ========
 *  On-Chip OAD flash slot configuration utility.
 *
 *  Per-device input constants shared between ble_oad.syscfg.js and
 *  the example .syscfg file. Each entry is the single source of truth for
 *  all device-specific build configuration for On-Chip OAD:
 *
 *    APP_SLOT_BASE     - Base address of the OAD application slot (NVS region)
 *    APP_SLOT_SIZE     - Size of the OAD application slot (NVS region)
 *
 *  Note: For on-chip OAD, APP_HDR_ADDR equals APP_SLOT_BASE (the header
 *  is at the start of the slot, same as persistent OAD).
 */

"use strict";

/*
 * Per-device on-chip OAD slot configuration.
 * Key: substring matched against system.deviceData.deviceId
 */
const ONCHIP_OAD_DEVICE_INPUTS = {
    "CC2340R5": {
        APP_SLOT_BASE: 0x34000,
        APP_SLOT_SIZE: 0x48000,
    },
};

/*
 *  ======== getOnchipOadLayout ========
 *  Return the on-chip OAD slot configuration for the given device.
 *
 *  @param  {string} deviceId  - Device ID from system.deviceData.deviceId
 *  @returns {object|null}     - Slot config, or null if device unsupported
 */
function getOnchipOadLayout(deviceId)
{
    for (let key of Object.keys(ONCHIP_OAD_DEVICE_INPUTS)) {
        if (deviceId.includes(key)) {
            return ONCHIP_OAD_DEVICE_INPUTS[key];
        }
    }
    return null;
}

/*
 *  ======== getCompilerDefines ========
 * Return the compiler defines for On-Chip OAD based on the user-configured
 * header address.
 *
 * @param deviceId   - Device ID from system.deviceData.deviceId
 * @param appHdrAddr - User-configured APP_HDR_ADDR value
 * @returns          - Array of -D compiler define strings
 */
function getCompilerDefines(deviceId, appHdrAddr)
{
    return [
        "-DOAD_APP_ONCHIP",
        "-DAPP_HDR_ADDR=0x" + appHdrAddr.toString(16),
    ];
}

/*
 *  ======== getLinkerSymbols ========
 * Return the linker symbol definitions for On-Chip OAD based on the
 * user-configured header address.
 *
 * @param deviceId   - Device ID from system.deviceData.deviceId
 * @param appHdrAddr - User-configured APP_HDR_ADDR value
 * @returns          - Array of {name, value} linker symbol descriptors
 */
function getLinkerSymbols(deviceId, appHdrAddr)
{
    return [
        { name: "OAD_APP_ONCHIP", value: 1 },
        { name: "APP_HDR_ADDR",   value: appHdrAddr },
    ];
}

/*
 *  ======== moduleInstances ========
 * Add NVS module instance required for On-Chip OAD.
 * Creates one NVS region for the application slot.
 *
 * appSlotBase and appSlotSize override the layout defaults when provided,
 * allowing the user-configured values from ble_oad.syscfg.js to take effect.
 *
 * @param deviceId    - Device ID from system.deviceData.deviceId
 * @param appSlotBase - (optional) Override for APP_SLOT_BASE
 * @param appSlotSize - (optional) Override for APP_SLOT_SIZE
 * @returns           - Array of module instance descriptors
 */
function moduleInstances(deviceId, appSlotBase, appSlotSize)
{
    const layout = getOnchipOadLayout(deviceId);
    if (!layout)
    {
        return [];
    }

    const slotBase = (appSlotBase !== undefined) ? appSlotBase : layout.APP_SLOT_BASE;
    const slotSize = (appSlotSize !== undefined) ? appSlotSize : layout.APP_SLOT_SIZE;

    return [
        {
            name        : "onchipOadNvsAppSlot",
            displayName : "On-Chip OAD App Slot NVS",
            moduleName  : "/ti/drivers/NVS",
            group       : "oadConfig",
            hidden      : true,
            collapsed   : true,
            requiredArgs: {
                $name   : "CONFIG_NVSINTERNAL1",
                nvsType : "Internal"
            },
            args: {
                internalFlash: {
                    regionBase: slotBase,
                    regionSize: slotSize,
                    regionType: "Pointer"
                }
            }
        }
    ];
}

exports = {
    ONCHIP_OAD_DEVICE_INPUTS,
    getOnchipOadLayout,
    getCompilerDefines,
    getLinkerSymbols,
    moduleInstances,
};
