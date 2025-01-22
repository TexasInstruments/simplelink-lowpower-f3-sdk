/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 */

/*
 *  ======== zigbee_network.syscfg.js ========
 */

"use strict";

const Common = system.getScript("/ti/zigbee/zigbee_common.js");

const defaultNwkKeyDescription = `Default network key. Set to 0 to generate \
a random key.`;

const defaultNwkKeyLongDescription = defaultNwkKeyDescription + `\n\n\
**Default:** 0x0

**Range:** Any ${Common.NWK_KEY_LEN * 8} bit number (hexidecimal format)`;

const nwkMaxChildrenDescription = `Number of nodes that the local device can \
support in its association table`;

const nwkMaxChildrenLongDescription = nwkMaxChildrenDescription
+ `\n\n **Default:** 20

**Range:** 0 - 255`;

const endDeviceTimeoutDescription = `Value of End Device Timeout`;

const endDeviceTimeoutLongDescription = endDeviceTimeoutDescription
+ `\n\n
If ZED, this is the value used when sending End Device Timeout Request.

If ZC/ZR, this is the default value if any End Device that does not
 negotiate a different timeout value

**Default:** ED_AGING_TIMEOUT_256MIN`;

/* End device timeout values */
const timeoutValues = [
    {name: 'ED_AGING_TIMEOUT_10SEC', displayName: "0 - 10 seconds"},
    {name: 'ED_AGING_TIMEOUT_2MIN', displayName: "1 - 2 minutes"},
    {name: 'ED_AGING_TIMEOUT_4MIN', displayName: "2 - 4 minutes"},
    {name: 'ED_AGING_TIMEOUT_8MIN', displayName: "3 - 8 minutes"},
    {name: 'ED_AGING_TIMEOUT_16MIN', displayName: "4 - 16 minutes"},
    {name: 'ED_AGING_TIMEOUT_32MIN', displayName: "5 - 32 minutes"},
    {name: 'ED_AGING_TIMEOUT_64MIN', displayName: "6 - 64 minutes"},
    {name: 'ED_AGING_TIMEOUT_128MIN', displayName: "7 - 128 minutes"},
    {name: 'ED_AGING_TIMEOUT_256MIN', displayName: "8 - 256 minutes"},
    {name: 'ED_AGING_TIMEOUT_512MIN', displayName: "9 - 512 minutes"},
    {name: 'ED_AGING_TIMEOUT_1024MIN', displayName: "10 - 1024 minutes"},
    {name: 'ED_AGING_TIMEOUT_2048MIN', displayName: "11 - 2048 minutes"},
    {name: 'ED_AGING_TIMEOUT_4096MIN', displayName: "12 - 4096 minutes"},
    {name: 'ED_AGING_TIMEOUT_8192MIN', displayName: "13 - 8192 minutes"},
    {name: 'ED_AGING_TIMEOUT_16384MIN', displayName: "14 - 16384 minutes"}
];

/* Network submodule for the zstack module */
const config = {
    displayName: "Network",
    description: "Configure network identification and security settings",
    config: [
        {
            name: "defaultNwkKey",
            displayName: "Default Network Key",
            description: defaultNwkKeyDescription,
            longDescription: defaultNwkKeyLongDescription,
            default: "00000000000000000000000000001234"
        },
        {
            name: "nwkMaxChildren",
            displayName: "Network Maximum Children",
            description: nwkMaxChildrenDescription,
            longDescription: nwkMaxChildrenLongDescription,
            default: 20,
            hidden: true
        },
        {
            name: "endDeviceTimeout",
            displayName: "End Device Timeout",
            description: endDeviceTimeoutDescription,
            longDescription: endDeviceTimeoutLongDescription,
            default: 'ED_AGING_TIMEOUT_256MIN',
            options: timeoutValues
        },
    ],
};

/* Function to handle changes in deviceType configurable */
function onDeviceTypeChange(inst, ui)
{
    if(!inst.deviceType.includes("gpd"))
    {
        let element = null;
        for(element of config.config)
        {
            ui[element.name].hidden = false;
        }

        if(inst.deviceType.includes("zc") || inst.deviceType.includes("zr"))
        {
            ui.nwkMaxChildren.hidden = false;
        }
        else /* zed */
        {
            ui.nwkMaxChildren.hidden = true;
        }
    }
    else
    {
        let element = null;
        for(element of config.config)
        {
            ui[element.name].hidden = true;
        }
    }
}

/* Validation function for the network submodule */
function validate(inst, validation)
{
    /* Validate Default Network Key */
    const defaultNwkKeyReg = new RegExp(
        "^[0-9A-Fa-f]{" + Common.NWK_KEY_LEN * 2 + "}$", "g"
    );
    if(defaultNwkKeyReg.test(inst.defaultNwkKey) === false)
    {
        validation.logError(
            "Default Network Key must be a valid hexidecimal number (...) "
            + "of length " + Common.NWK_KEY_LEN * 8 + " bits",
            inst, "defaultNwkKey"
        );
    }

    /* Info for random Network Key (0x0) */
    const defaultNwkKeyRandomReg = new RegExp(
        "^[0]{" + Common.NWK_KEY_LEN * 2 + "}$", "g"
    );
    if(defaultNwkKeyRandomReg.test(inst.defaultNwkKey) === true)
    {
        validation.logInfo(
            "Setting Default Network Key to 0x0 will cause the network key "
            + "to be randomly generated.", inst, "defaultNwkKey"
        );
    }

    /* Validate Network Max Device List */
    Common.validateRange(inst, validation, inst.nwkMaxChildren,
        "nwkMaxChildren", "Network Maximum Device List", 1, 255);
}

exports = {
    config: config,
    validate: validate,
    onDeviceTypeChange: onDeviceTypeChange
};