/*
 * Copyright (c) 2024, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== APU.syscfg.js ========
 */

"use strict";

let Common = system.getScript("/ti/drivers/Common.js");
let family = Common.device2Family(system.deviceData, "APU");

let intPriority = Common.newIntPri()[0];
intPriority.displayName = "Hardware Interrupt Priority";
intPriority.name = "interruptPriority";

let config = [
    intPriority,
    {
        name: "operationMode",
        displayName: "APU Operation Mode",
        default: "Mirrored",
        options: [
            { name: "Mirrored" }
        ]
    }
];

/*
 *  ======== base ========
 *  Define the base APU properties and methods
 */
let base = {
    displayName: "APU",
    staticOnly   : true,
    description: "Algorithm Processing Unit (APU) Driver",
    longDescription: `[__APU driver__][1] allows management of the Algorithm Processing Unit (APU), a generic
mathematical acceleration module that is able to operate with single-precision
floating point numbers (IEEE 754 format) and is optimized to work with
complex numbers.

[1]: /drivers/doxygen/html/_v_c_e_l_p_f3_8h.html#details "C API reference"
`,
    maxInstances: 1,
    moduleStatic: {
        config: config,
        modules: Common.autoForceModules(["Board"])
    },
    config: config,
    initPriority: 10
};

/* Get family-specific APU module */
let devAPU = system.getScript("/ti/drivers/apu/APU" + family);

exports = devAPU.extend(base);