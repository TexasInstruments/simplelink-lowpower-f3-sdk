/*
 * Copyright (c) 2018-2025 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble_device_identity.syscfg.js ========
 *  BLE device identity: device name, address mode, and RPA configuration.
 *  Used only by the full BLE module (not ble_host or ble_controller).
 */

"use strict";

// Get long descriptions
const Docs = system.getScript("/ti/ble/general/ble_device_identity_docs.js");

const configItems = [
    {
        name: "deviceName",
        displayName: "Device Name",
        default: "Simple Peripheral",
        longDescription: Docs.deviceNameLongDescription
    },
    {
        name: "hideAddressModeRPA",
        displayName: "Hide Address Mode RPA",
        default: false,
        hidden: true
    },
    {
        name: "addressMode",
        displayName: "Address Mode",
        default: "ADDRMODE_RP_WITH_PUBLIC_ID",
        longDescription: Docs.addressModeLongDescription,
        getDisabledOptions: generateDisabledOptions("addressMode"),
        onChange: onAddressModeChange,
        options: [
            {
                displayName: "Public Address",
                name: "ADDRMODE_PUBLIC",
                description: "Always Use Public Address"
            },
            {
                displayName: "Random Static Address",
                name: "ADDRMODE_RANDOM",
                description: "Always Use Random Static Address"
            },
            {
                displayName: "RPA with Public ID",
                name: "ADDRMODE_RP_WITH_PUBLIC_ID",
                description: "Always Use Resolvable Private Address with Public Identity Address"
            },
            {
                displayName: "RPA with Random ID",
                name: "ADDRMODE_RP_WITH_RANDOM_ID",
                description: "Always Use Resolvable Private Address with Random Identity Address"
            }
        ]
    },
    {
        name: "randomAddress",
        displayName: "Random Address",
        default: "00:00:00:00:00:00",
        placeholder: "00:00:00:00:00:00",
        textType: "mac_address_48",
        longDescription: Docs.randomAddressLongDescription,
        hidden: true
    },
    {
        name: "rpaRead",
        displayName: "RPA Read Period (ms)",
        default: 3000,
        longDescription: Docs.rpaReadLongDescription
    },
];

const config = {
    displayName: "Device Identity",
    description: "Configure BLE device name and address settings",
    config: configItems
};

/*
 *  ======== generateDisabledOptions ========
 *  Generates a list of options that should be disabled in a drop-down
 *
 * @returns Array - array of strings that should be disabled
 */
function generateDisabledOptions(name)
{
    return (inst) => {

        // Find the configurable we're going to generate a disabled list from
        const configurable = inst.$module.$configByName[name].options;

        if(inst.hideAddressModeRPA)
        {
            // List of invalid options
            const disabledOptions = _.filter(configurable,(conf) => conf.name.includes("RP") == true);
            // Add the "reason" why it's disabled, and return that information
            return disabledOptions.map((option) => ({ name: option.name, reason: "This is not a valid option" }));
        }

        return [];
    }
}

/*
 *  ======== onAddressModeChange ========
 *  Show/hide the randomAddress configurable based on the selected addressMode.
 *  @param inst  - Module instance containing the config that changed
 *  @param ui    - The User Interface object
 */
function onAddressModeChange(inst, ui)
{
    inst.addressMode == "ADDRMODE_RANDOM" ||
    inst.addressMode == "ADDRMODE_RP_WITH_RANDOM_ID" ?
    ui.randomAddress.hidden = false :
    ui.randomAddress.hidden = true;
}

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    if(inst.deviceName.length > 31)
    {
        validation.logError("Max Length of Device Name Attribute is 31", inst, "deviceName");
    }
    if(inst.rpaRead < 0 || inst.rpaRead > (Math.pow(2,32) - 1))
    {
        validation.logError("RPA read period range is 0 to sizeof(uint32)", inst, "rpaRead");
    }
}

/*
 *  ======== exports ========
 *  Export the BLE Device Identity module
 */
exports = {
    config: config,
    configItems: configItems,
    validate: validate
};
