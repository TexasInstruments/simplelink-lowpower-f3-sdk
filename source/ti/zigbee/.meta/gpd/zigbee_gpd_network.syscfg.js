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
 *  ======== zigbee_gpd_network.syscfg.js ========
 */

"use strict";

/* Description text for configurables */

const gpdIDTypeLongDescription = `Specify whether the Green Power Device ID \
is a 32 bit configurable value or is the 64 bit IEEE address.

Choosing *Configurable GPD ID* will allow the Green Power Device ID to be \
configured in the *Green Power Device ID* field. Choosing \
*Use IEEE address as GPD ID* will automatically set the Green Power Device \
ID as the IEEE address of the Green Power Device.

**Default:** Configurable GPD ID`;

const gpdIDDescription = `Specifies the 32 bit Green \
Power Device ID of the device.`;

const gpdIDLongDescription = gpdIDDescription + `\n\n\
**Default:** 0xabacadac

**Range:** Any 32 bit number (hexidecimal format)`;

/* GPD network submodule for zigbee module */
const gpdNetworkModule = {
    config: [
        {
            name: "gpdIDType",
            displayName: "Green Power Device ID Type",
            description: "Specify whether the Green power Device ID is a 32 "
                         + "bit configurable value or the 64 bit IEEE address.",
            longDescription: gpdIDTypeLongDescription,
            default: 0,
            options: [
                {
                    name: 0,
                    displayName: "Configurable GPD ID"
                },
                {
                    name: 2,
                    displayName: "Use IEEE Address as GPD ID"
                }
            ],
            onChange: onGpdIDTypeChange
        },
        {
            name: "gpdID",
            displayName: "Green Power Device ID",
            description: gpdIDDescription,
            longDescription: gpdIDLongDescription,
            default: "0xabacadac"
        }
    ],
    validate: validate
};

/* Function to handle changes in gpdIDType configurable */
function onGpdIDTypeChange(inst, ui)
{
    if(inst.gpdIDType === 0)
    {
        ui.gpdID.hidden = false;
    }
    else
    {
        ui.gpdID.hidden = true;
    }
}

/* Validation function for the GPD advanced submodule */
function validate(inst, validation)
{

}

exports = gpdNetworkModule;
