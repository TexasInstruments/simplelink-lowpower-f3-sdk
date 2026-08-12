/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

/*
 *  ======== zigbee_gpd_advanced.syscfg.js ========
 */

"use strict";

/* Description text for configurables */
const gpdfRetriesDescription = `The number of Green Power Data Frame retries \
per packet. This is recommended to be low for reduced power consumption.`;

const gpdfRetriesLongDescription = gpdfRetriesDescription + `\n\n\
**Default:** 3

**Range:** Any 8 bit number (hexidecimal format)`;

const uniDirCommDescription = `If True, this device will use unidirectional commissioning. \
If False, this device will use bidirectional commissioning`;

const uniDirCommDescriptionLongDescription = uniDirCommDescription + `\n\n\
**Default:** false`;

const batterylessDescription = `If True, this deivce will act as a batteryless device. \
If False, this device will act as a battery device.`;

const batterylessDescriptionLongDescription = batterylessDescription + `\n\n\
**Default:** False`;

/* GPD network submodule for zigbee module */
const gpdNetworkModule = {
    config: [
        {
            name: "gpdfRetries",
            displayName: "Data Frame Retries",
            description: gpdfRetriesDescription,
            longDescription: gpdfRetriesLongDescription,
            default: 3
        },
        {
            name: "uniDirComm",
            displayName: "Unidirectional Commissioning",
            description: uniDirCommDescription,
            longDescription: uniDirCommDescriptionLongDescription,
            default: false
        },
        {
            name: "batteryless",
            displayName: "Batteryless",
            description: batterylessDescription,
            longDescription: batterylessDescriptionLongDescription,
            default: false
        },
    ],
    validate: validate
};

/* Validation function for the GPD advanced submodule */
function validate(inst, validation)
{

}

exports = gpdNetworkModule;
