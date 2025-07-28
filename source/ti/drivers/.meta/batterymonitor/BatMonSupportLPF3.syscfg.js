/*
 * Copyright (c) 2022-2025, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== BatMonSupportLPF3.syscfg.js ========
 */

"use strict";

let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";

let config = [
    intPriority,
    {
        name: "interruptNumber",
        displayName: "Interrupt Number",
        default: "INT_CPUIRQ2",
        description: "Specifies the configurable interrupt line to be used uniquely by BATMON.",
        options: [
            { name: "INT_CPUIRQ0" },
            { name: "INT_CPUIRQ2" }
        ]
    }
];


/*
 *  ======== onModuleChanged ========
 *  onModuleChanged for when dependencies change
 */
function onModuleChanged(inst, dependentInst, moduleName, configurables)
{
    if (moduleName == "/ti/utils/TrustZone")
    {
        if (dependentInst != undefined)
        {
            /* The TrustZone module is added. The following configs are fixed
             * by the secure image:
             *  - Interrupt Number
             *
             * Set the value of these configs to the value fixed in the secure
             * image, and make them read only.
             */

            inst.interruptNumber = "INT_CPUIRQ2";
            inst.$uiState.interruptNumber.readOnly = "When TrustZone is enabled, the interrupt number is locked to INT_CPUIRQ2";
        }
        else
        {
            /* TrustZone module is not enabled, allow modifications. */
            inst.$uiState.interruptNumber.readOnly = false;
        }
    }
}

/*
 *  ======== base ========
 *  Define the base BatteryMonitor properties and methods
 */
let base = {
    displayName         : "BatMon Support",
    description         : "BatMon Support Layer",
    longDescription: ``,
    maxInstances        : 1,
    initPriority        : -1, /* lower numbers initialize earlier */
    moduleStatic: {
        config: config,
        dependencies: {
            modules: {
                "/ti/utils/TrustZone" : ["secureImage"]
            },
            onModuleChanged: onModuleChanged
        }
    },
    templates: {
        boardc: "/ti/drivers/batterymonitor/BatMonSupportLPF3.Board.c.xdt"
    }
};

exports = base;
