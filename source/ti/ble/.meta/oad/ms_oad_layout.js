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
 *  ======== ms_oad_layout.js ========
 *  MS-OAD flash memory layout utility.
 *
 *  Per-device input constants and layout computation shared between
 *  ble_oad.syscfg.js and the ble_oad_linker.cmd.xdt template.
 *
 *  Inputs (per device, hardcoded):
 *    APP_1_HDR_ADDR             - MCUBoot end / App1 header start address
 *    MCUBOOT_TLV_MAX_SIZE       - Max TLV area reserved at end of each app slot
 *    APP_SWAP_SLOT_SIZE         - Swap page size
 *    APP_MS_OAD_STATE_SLOT_SIZE - MS-OAD journal state page size
 *    MCU_HDR_SIZE               - MCUBoot image header size
 *    MCUBOOT_SIZE               - MCUBoot image size
 *    FLASH_BASE                 - Flash origin address
 *
 *  The FLASH_SIZE input is derived dynamically from getBondNVBaseAddress()
 *  in ble_common.js, since OAD builds always have NVS enabled and
 *  FLASH_SIZE = NVS_BASE (the effective flash end for application use).
 */

"use strict";

// BLE common utilities (provides getBondNVBaseAddress for flash size)
const Common = system.getScript("/ti/ble/ble_common.js");

/*
 * Per-device MS-OAD layout input constants.
 * Key: substring matched against system.deviceData.deviceId
 */
const MS_OAD_DEVICE_INPUTS = {
    "CC2340R5": {
        APP_1_HDR_ADDR:             0x6000,  // MCUBoot end / App1 header start
        MCUBOOT_TLV_MAX_SIZE:       0x800,   // Max TLV area per app slot
        APP_SWAP_SLOT_SIZE:         0x800,   // Swap page size
        APP_MS_OAD_STATE_SLOT_SIZE: 0x800,   // MS-OAD state page size
        MCU_HDR_SIZE:               0x100,   // MCUBoot image header size
        MCUBOOT_SIZE:               0x6000,  // MCUBoot image size
        FLASH_BASE:                 0x0,     // Flash origin
    }
};

/*
 *  ======== getDeviceInputs ========
 *  Return the raw MS-OAD device input constants for the given device.
 *
 *  @param  {string} deviceId  - Device ID from system.deviceData.deviceId
 *  @returns {object|null}     - Device input constants, or null if unsupported
 */
function getDeviceInputs(deviceId)
{
    for (let key of Object.keys(MS_OAD_DEVICE_INPUTS)) {
        if (deviceId.includes(key)) {
            return MS_OAD_DEVICE_INPUTS[key];
        }
    }
    return null;
}

/*
 *  ======== computeMsOadLayout ========
 *  Compute the full MS-OAD flash memory layout.
 *
 *  @param  {string} deviceId      - Device ID from system.deviceData.deviceId
 *  @param  {number} [app1HdrAddr] - Optional override for APP_1_HDR_ADDR
 *  @returns {object|null}         - Computed layout, or null if device unsupported
 */
function computeMsOadLayout(deviceId, app1HdrAddr)
{
    const flashSize = Common.getBondNVBaseAddress();
    const c = getDeviceInputs(deviceId);

    if (!c) return null;

    const effectiveApp1HdrAddr = (app1HdrAddr !== undefined) ? app1HdrAddr : c.APP_1_HDR_ADDR;

    /* Sizes (matching formulas from ble_oad_linker.cmd.xdt) */
    const TOTAL_APP_SIZE     = flashSize - effectiveApp1HdrAddr
                               - c.APP_SWAP_SLOT_SIZE - c.APP_MS_OAD_STATE_SLOT_SIZE;
    const APP_SLOT_SIZE      = Math.floor(TOTAL_APP_SIZE / 2);
    const APP_SIZE           = APP_SLOT_SIZE - c.MCU_HDR_SIZE - c.MCUBOOT_TLV_MAX_SIZE;

    /* Base addresses */
    const MCUBOOT_BASE                  = c.FLASH_BASE;
    const APP_1_HDR_BASE                = effectiveApp1HdrAddr;
    const APP_1_BASE                    = APP_1_HDR_BASE + c.MCU_HDR_SIZE;
    const APP_1_TLV_PLACEHOLDER_BASE    = APP_1_BASE + APP_SIZE;
    const SWAP_PAGE_BASE                = APP_1_TLV_PLACEHOLDER_BASE + c.MCUBOOT_TLV_MAX_SIZE;
    const APP_2_HDR_BASE                = SWAP_PAGE_BASE + c.APP_SWAP_SLOT_SIZE;
    const APP_2_BASE                    = APP_2_HDR_BASE + c.MCU_HDR_SIZE;
    const APP_2_TLV_PLACEHOLDER_BASE    = APP_2_BASE + APP_SIZE;
    const MS_OAD_STATE_BASE             = APP_2_TLV_PLACEHOLDER_BASE + c.MCUBOOT_TLV_MAX_SIZE;

    return {

        /* Inputs */
        MCUBOOT_SIZE:               c.MCUBOOT_SIZE,
        APP_HDR_SIZE:               c.MCU_HDR_SIZE,
        MCU_HDR_SIZE:               c.MCU_HDR_SIZE,
        MCUBOOT_TLV_MAX_SIZE:       c.MCUBOOT_TLV_MAX_SIZE,
        APP_TLV_PLACEHOLDER_SIZE:   c.MCUBOOT_TLV_MAX_SIZE,
        SWAP_PAGE_SIZE:             c.APP_SWAP_SLOT_SIZE,
        APP_SWAP_SLOT_SIZE:         c.APP_SWAP_SLOT_SIZE,
        APP_MS_OAD_STATE_SLOT_SIZE: c.APP_MS_OAD_STATE_SLOT_SIZE,

        /* Computed sizes */
        TOTAL_APP_SIZE,
        APP_SLOT_SIZE,
        APP_SIZE,

        /* Base addresses */
        MCUBOOT_BASE,
        APP_1_HDR_BASE,
        APP_1_BASE,
        APP_1_TLV_PLACEHOLDER_BASE,
        SWAP_PAGE_BASE,
        APP_2_HDR_BASE,
        APP_2_BASE,
        APP_2_TLV_PLACEHOLDER_BASE,
        MS_OAD_STATE_BASE,
    };
}

/*
 *  ======== getCompilerDefines ========
 * Return the compiler defines for MS-OAD based on the user-configured
 * APP_1_HDR_ADDR and the derived layout.
 *
 * @param deviceId    - Device ID from system.deviceData.deviceId
 * @param app1HdrAddr - User-configured APP_1_HDR_ADDR value
 * @returns           - Array of -D compiler define strings
 */
function getCompilerDefines(deviceId, app1HdrAddr)
{
    const layout = computeMsOadLayout(deviceId, app1HdrAddr);
    if (!layout) return [];
    return [
        "-DMS_OAD",
        "-DAPP_HDR_ADDR=0x"           + layout.APP_1_HDR_BASE.toString(16),
        "-DMS_OAD_SLOT_1_HDR_ADDR=0x" + layout.APP_1_HDR_BASE.toString(16),
        "-DMS_OAD_SLOT_2_HDR_ADDR=0x" + layout.APP_2_HDR_BASE.toString(16),
    ];
}

/*
 *  ======== getLinkerSymbols ========
 * Return the linker symbol definitions for MS-OAD.
 * MS_OAD=1 is the only linker symbol; address defines are compiler-only.
 *
 * @param deviceId - Device ID from system.deviceData.deviceId
 * @returns        - Array of {name, value} linker symbol descriptors
 */
function getLinkerSymbols(deviceId)
{
    if (!getDeviceInputs(deviceId)) return [];
    return [{ name: "MS_OAD", value: 1 }];
}

/*
 *  ======== moduleInstances ========
 * Add NVS module instances required for Multistep OAD.
 * Creates three NVS regions whose addresses are computed from the MS-OAD
 * flash layout so they stay in sync with the linker file.
 *
 * @param deviceId    - Device ID from system.deviceData.deviceId
 * @param app1HdrAddr - Optional override for APP_1_HDR_ADDR
 * @returns           - Array of module instance descriptors
 */
function moduleInstances(deviceId, app1HdrAddr)
{
    const layout    = computeMsOadLayout(deviceId, app1HdrAddr);
    if (!layout)
    {
        return [];
    }

    return [
        {
            name        : "msOadNvsApp2Slot",
            displayName : "MS-OAD App2 Slot NVS",
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
                    regionBase: layout.APP_2_HDR_BASE,
                    regionSize: layout.APP_SLOT_SIZE,
                    regionType: "Pointer"
                }
            }
        },
        {
            name        : "msOadNvsPrimaryTrailer",
            displayName : "MS-OAD Primary Image Trailer NVS",
            moduleName  : "/ti/drivers/NVS",
            group       : "oadConfig",
            hidden      : true,
            collapsed   : true,
            requiredArgs: {
                $name   : "CONFIG_NV_PRIMARY_IMAGE_TRAILER",
                nvsType : "Internal"
            },
            args: {
                internalFlash: {
                    regionBase: layout.APP_1_TLV_PLACEHOLDER_BASE,
                    regionSize: layout.APP_TLV_PLACEHOLDER_SIZE,
                    regionType: "Pointer"
                }
            }
        },
        {
            name        : "msOadNvsStateSlot",
            displayName : "MS-OAD State Slot NVS",
            moduleName  : "/ti/drivers/NVS",
            group       : "oadConfig",
            hidden      : true,
            collapsed   : true,
            requiredArgs: {
                $name   : "CONFIG_NV_MS_OAD",
                nvsType : "Internal"
            },
            args: {
                internalFlash: {
                    regionBase: layout.MS_OAD_STATE_BASE,
                    regionSize: layout.APP_MS_OAD_STATE_SLOT_SIZE,
                    regionType: "Pointer"
                }
            }
        }
    ];
}

exports = {
    MS_OAD_DEVICE_INPUTS,
    getDeviceInputs,
    computeMsOadLayout,
    getCompilerDefines,
    getLinkerSymbols,
    moduleInstances,
};
