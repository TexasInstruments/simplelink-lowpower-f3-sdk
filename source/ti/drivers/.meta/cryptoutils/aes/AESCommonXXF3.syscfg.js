/*
 * Copyright (c) 2025 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== AESCommonXXF3.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/* get device ID */
let deviceId = system.deviceData.deviceId;

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "Crypto peripheral interrupt priority";

/*
 *  ======== pinmuxRequirements ========
 *  Define the pinmux requirements for AESCommonXXF3
 *
 *  @param inst       - AESCommon instance
 *
 *  @returns         - Pinmux requirements
 */
function pinmuxRequirements(inst)
{
    if (deviceId.match(/CC23|CC27/)) {
        let dataLaesChannelA = {
            name : "dataLaesChannelA",
            displayName : "DMA LAES Channel A",
            description : "DMA channel used for LAES Engine",
            canShareWith: "LAES",
            interfaceNames : ["TRGA"]
        };

        let dataLaesChannelB = {
            name : "dataLaesChannelB",
            displayName : "DMA LAES Channel B",
            description : "DMA channel used for LAES Engine",
            canShareWith: "LAES",
            interfaceNames : ["TRGB"]
        };

        let resources = [];
        resources.push(dataLaesChannelA);
        resources.push(dataLaesChannelB);

        let laesPinmux = {
            name: "laes",
            displayName: "LAES Peripheral",
            interfaceName: "LAES",
            canShareWith: "LAES",
            resources: resources
        };

        return [laesPinmux];
    } else {
        return [];
    }
}

/*
 *  ======== base ========
 *  Define the base AESCommon properties and methods
 */
let base = {
    displayName         : "AESCommonXXF3",
    description         : "LAES Common Module to support AES Drivers",
    longDescription     : `LAES Common Module that generates DMA channel info
for LAES engine. This module is used by all AES drivers that use LAES HW engine.`,
    maxInstances        : 1,
    initPriority        : -1, /* lower numbers initialize earlier */
    moduleStatic: {
        pinmuxRequirements: pinmuxRequirements
    },
    templates: {
        boardc: "/ti/drivers/cryptoutils/aes/AESCommonXXF3.Board.c.xdt"
    }
};

exports = base;
