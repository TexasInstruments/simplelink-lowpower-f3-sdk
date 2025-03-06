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
 *  ======== zigbee_rf.syscfg.js ========
 */

"use strict";

/* Description text for configurables */
const primaryChannelsLongDescription = `The primary IEEE 802.15.4 frequency \
channels to use in network formation or joining.

During network formation, these channels are scanned to find the lowest noise \
channel to form a channel on. During joining, these channels are scanned for \
existing networks to join.

For more information, refer to the [ZigBee Configuration](/zigbee/html/\
sysconfig/zigbee.html#zigbee-configuration) section of the ZigBee User's \
Guide.

**Default:** Channel 11

**Range:** Any combination of channels 11-26`;

/* Frequency channel options for enumeration configurables */
const channelOptions = [
    {name: 11, displayName: "11 - 2405 MHz"},
    {name: 12, displayName: "12 - 2410 MHz"},
    {name: 13, displayName: "13 - 2415 MHz"},
    {name: 14, displayName: "14 - 2420 MHz"},
    {name: 15, displayName: "15 - 2425 MHz"},
    {name: 16, displayName: "16 - 2430 MHz"},
    {name: 17, displayName: "17 - 2435 MHz"},
    {name: 18, displayName: "18 - 2440 MHz"},
    {name: 19, displayName: "19 - 2445 MHz"},
    {name: 20, displayName: "20 - 2450 MHz"},
    {name: 21, displayName: "21 - 2455 MHz"},
    {name: 22, displayName: "22 - 2460 MHz"},
    {name: 23, displayName: "23 - 2465 MHz"},
    {name: 24, displayName: "24 - 2470 MHz"},
    {name: 25, displayName: "25 - 2475 MHz"},
    {name: 26, displayName: "26 - 2480 MHz"}
];

const config = {
    displayName: "Radio",
    description: "Configure default radio channel",
    config: [
        {
            name: "primaryChannels",
            displayName: "Primary Channels",
            description: "The primary IEEE 802.15.4 frequency channels to use "
                         + "in network formation or joining.",
            longDescription: primaryChannelsLongDescription,
            default: [11],
            options: channelOptions,
            minSelections: 0
        },
    ]
}

/* Function to handle changes in deviceType configurable */
function onDeviceTypeChange(inst, ui)
{
    let element = null;
    for(element of config.config)
    {
        ui[element.name].hidden = inst.deviceType.includes("gpd");
    }
}

exports = {
    config: config,
    onDeviceTypeChange: onDeviceTypeChange
};