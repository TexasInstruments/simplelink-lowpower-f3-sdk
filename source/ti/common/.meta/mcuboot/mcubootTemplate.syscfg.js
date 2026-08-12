/*
 * Copyright (c) 2024-2025, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== mcubootTemplate.syscfg.js ========
 */

"use strict";
let mcubootSettings = {};
let deviceGroup = null;

const deviceGroupMcubootSettings = {
    
    DeviceGroup_CC13X2X7_CC26X2X7: {
        tzEnabled:{ readOnly: true, hidden: true, tzConfigurable: false },
        image1:
        {
            tzEnabledBase:
            {
                primaryBase: null,
                secondaryBase: null,
            },
            tzDisabledBase:
            {
                primaryBase: 0x0,
                secondaryBase: 0x56000,
            },
            primarySize: 0x56000,
            secondarySize: 0x56000
        },
        /*This device does not support TZ enabled, so the defaults are 0 */
        image2: 
        {
            primaryBase: 0x0,
            primarySize: 0x0,
            secondaryBase: 0x0,
            secondarySize: 0x0
        },
        image1Compressed:
        {
            tzEnabledBase:
            {
                primaryBase: null,
                secondaryBase: null,
            },
            tzDisabledBase:
            {
                primaryBase: 0x07000,
                secondaryBase: 0x4D000,
            },
            primarySize: 0x46000,
            secondarySize: 0x31000
        },
        antiRollbackProtection: { readOnly: true, hidden: true },
        enableEncryptedImage: { enabled: true },
        bootloader:
        { 
            tzEnabledBase:
            {
                base: null,
            },
            tzDisabledBase:
            {
                base: 0xAC000,
            },
            size: 0x4000 
        },
        alignment: { flashBoundary: 0xB0000, sectorSize : 0x2000 }
   },
   DeviceGroup_CC13X4_CC26X3_CC26X4: {
        tzEnabled: { readOnly: false, hidden: false, tzConfigurable: false},
        image1:
        {
            tzEnabledBase:
            {
                primaryBase: 0xd000,
                secondaryBase: 0x86800,
            },
            tzDisabledBase:
            {
                primaryBase: 0x6000,
                secondaryBase: 0x31000,
            },
            primarySize: 0x2B000,
            secondarySize: 0x2B000
        },
        image2: 
        {
            primaryBase: 0x38000,
            primarySize: 0x4E800,
            secondaryBase: 0xB1800,
            secondarySize: 0x4E800
        },
        image1Compressed:
        {
            tzEnabledBase:
            {
                primaryBase: null,
                secondaryBase: null,
            },
            tzDisabledBase:
            {
                primaryBase: 0x07000,
                secondaryBase: 0x4D000,
            },
            primarySize: 0x46000,
            secondarySize: 0x31000
        },
        antiRollbackProtection: { readOnly: false, hidden: false },
        enableEncryptedImage: { enabled: true },
        bootloader:
        { 
            tzEnabledBase:
            {
                base: 0x800,
            },
            tzDisabledBase:
            {
                base: 0x0,
            },
            size: 0x6000,
            compressionEnabledSize: 0x7000,
        },
        alignment: { flashBoundary: 0x100000, sectorSize : 0x800 }
   },
   DeviceGroup_CC23X0: {
        tzEnabled: { readOnly: true, hidden: true },
        image1:
        {
            tzEnabledBase:
            {
                primaryBase: null,
                secondaryBase: null,
            },
            tzDisabledBase:
            {
                primaryBase: 0x06000,
                secondaryBase: 0x43000,
            },
            primarySize: 0x3D000,
            secondarySize: 0x3D000
        },
        /*This device does not support TZ enabled, so the defaults are 0 */
        image2: 
        {
            primaryBase: 0x0,
            primarySize: 0x0,
            secondaryBase: 0x0,
            secondarySize: 0x0
        },
        image1Compressed:
        {
            tzEnabledBase:
            {
                primaryBase: null,
                secondaryBase: null,
            },
            tzDisabledBase:
            {
                primaryBase: 0x07000,
                secondaryBase: 0x4D000,
            },
            primarySize: 0x46000,
            secondarySize: 0x31000
        },
        antiRollbackProtection: { readOnly: false, hidden: false, tzConfigurable: false },
        enableEncryptedImage:{ enabled: false },
        bootloader:
        { 
            tzEnabledBase:
            {
                base: null,
            },
            tzDisabledBase:
            {
                base: 0x0,
            },
            size: 0x6000,
            compressionEnabledSize: 0x7000,
        },
        alignment: { flashBoundary: 0x80000, sectorSize : 0x800 }   
   },
   DeviceGroup_CC27XX: {
        tzEnabled: { readOnly: false, hidden: false, tzConfigurable: true},
        image1:
        {
            tzEnabledBase:
            {
                primaryBase: 0x6800,
                secondaryBase: 0x7A800,
            },
            tzDisabledBase:
            {
                primaryBase: 0x6000,
                secondaryBase: 0x31000,
            },
            primarySize: 0x2B000,
            secondarySize: 0x2B000
        },
        image2: 
        {
            /* The gap between image 1 primary and image 2 primary is for ITS, mcuboot does not care about ITS, so it is up to the user to configure accordingly. */
            primaryBase: 0x38000,
            primarySize: 0x42000,
            secondaryBase: 0xA5800,
            secondarySize: 0x42000
        },
        image1Compressed:
        {
            tzEnabledBase:
            {
                primaryBase: null,
                secondaryBase: null,
            },
            tzDisabledBase:
            {
                primaryBase: 0x07000,
                secondaryBase: 0x4D000,
            },
            primarySize: 0x46000,
            secondarySize: 0x31000
        },
        antiRollbackProtection: { readOnly: false, hidden: false },
        enableEncryptedImage:{ enabled: false },
        bootloader:
        { 
            tzEnabledBase:
            {
                base: 0x800,
            },
            tzDisabledBase:
            {
                base: 0x0,
            },
            size: 0x6000,
            compressionEnabledSize: 0x7000,
        },
        alignment: { flashBoundary: 0xE8000, sectorSize : 0x800 }
   }
};
let externalFlashSectorSize = 0x1000;
let externalFlashSize = 0x400000;
let externalFlashBase = 0x0;

// Get the LaunchPad specific MCUboot Settings
if(system.deviceData.deviceId)
{
    mcubootSettings = Object.assign(mcubootSettings,
        deviceGroupMcubootSettings[device2DeviceGroup(system.deviceData.deviceId)]);
}

let compressionDicSizeValues = [
    {name: 'LZMA2_DIC_SIZE_4096', displayName: "4096"},
    {name: 'LZMA2_DIC_SIZE_6144', displayName: "6144"},
    {name: 'LZMA2_DIC_SIZE_8192', displayName: "8192"},
    {name: 'LZMA2_DIC_SIZE_12288', displayName: "12288"},
    {name: 'LZMA2_DIC_SIZE_16384', displayName: "16384"},
    {name: 'LZMA2_DIC_SIZE_24576', displayName: "24576"},
    {name: 'LZMA2_DIC_SIZE_32768', displayName: "32768"},
    {name: 'LZMA2_DIC_SIZE_49152', displayName: "49152"},
];

function device2DeviceGroup(deviceId)
{
    if(deviceId.match(/CC(?:13|26).[2].[7]/))
    {
        deviceGroup = "DeviceGroup_CC13X2X7_CC26X2X7";
    }
    else if(deviceId.match(/CC(?:13|26).[34]/))
    {
        deviceGroup = "DeviceGroup_CC13X4_CC26X3_CC26X4";
    }
    else if(deviceId.match(/CC23.0/))
    {
        deviceGroup = "DeviceGroup_CC23X0";
    }
    else if(deviceId.match(/CC27/))
    {
        deviceGroup = "DeviceGroup_CC27XX";
    }
    else
    {
        deviceGroup = "";
    }

    return(deviceGroup);
}


exports = {
    template : {},
    config : {},
    deviceGroup : deviceGroup,
    mcubootSettings: mcubootSettings,
    externalFlashSectorSize: externalFlashSectorSize,
    externalFlashSize: externalFlashSize,
    externalFlashBase: externalFlashBase,
    compressionDicSizeValues: compressionDicSizeValues,
};