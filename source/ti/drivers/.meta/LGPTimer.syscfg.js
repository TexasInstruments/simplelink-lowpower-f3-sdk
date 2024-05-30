/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== LGPTimer.syscfg.js ========
 */

"use strict";

let Common   = system.getScript("/ti/drivers/Common.js");
let family   = Common.device2Family(system.deviceData, "LGPTimer");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "Timer Interrupt Priority";


let timerConfig = [
    intPriority
];

/*
 *  ======== base ========
 *  Define the base LGPTimer properties and methods
 */
let base = {
    displayName         : "LGPTimer",
    description         : "LGPTimer Driver",
    longDescription     : `
The [__LGPTimer driver__][1] allows you to manage a LGPTimer peripheral via
simple and portable APIs.

* [Peripheral properties][2]
* [Example][3]

[1]: /drivers/doxygen/html/_l_g_p_timer_l_p_f3_8h.html#details "C API reference"
[2]: /drivers/doxygen/html/_l_g_p_timer_l_p_f3_8h.html#ti_drivers_LGPTimer_PeripheralProperties "Peripheral properties"
[3]: /drivers/doxygen/html/_l_g_p_timer_l_p_f3_8h.html#ti_drivers_LGPTimer_Examples "C usage examples"
`,
    defaultInstanceName : "CONFIG_LGPTIMER_",
    config              : Common.addNameConfig(timerConfig, "/ti/drivers/lgptimer", "CONFIG_LGPTIMER_"),
    modules             : Common.autoForceModules(["Board", "Power"])
};

/* get family-specific LGPTimer module */
let devTimer = system.getScript("/ti/drivers/lgptimer/LGPTimer" + family);

exports = devTimer.extend(base);
