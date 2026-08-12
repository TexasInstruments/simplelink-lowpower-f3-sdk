/*
 * Copyright (c) 2022-2023, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== BatteryMonitor.syscfg.js ========
 */

"use strict";

let Common = system.getScript("/ti/drivers/Common.js");
let family = Common.device2Family(system.deviceData, "BatteryMonitor");

/*
 *  ======== base ========
 *  Define the base BatteryMonitor properties and methods
 */
let base = {
    displayName         : "Battery Monitor",
    description         : "Battery Monitor Driver",
    longDescription: `
The [__BatteryMonitor driver__][1] facilitates reading the current supply
voltage of a device. It also enables registering notification callbacks that
are invoked when the supply voltage crosses a specified threshold.

* [Usage Synopsis][2]
* [Examples][3]
* [Configuration Options][4]

[1]: /drivers/doxygen/html/_batterymonitor_8h.html#details "C API reference"
[2]: /drivers/doxygen/html/_batterymonitor_8h.html#ti_drivers_BatteryMonitor_Synopsis "Basic C usage summary"
[3]: /drivers/doxygen/html/_batterymonitor_8h.html#ti_drivers_BatteryMonitor_Examples "C usage examples"
[4]: /drivers/syscfg/html/ConfigDoc.html#BatteryMonitor_Configuration_Options "Configuration options reference"
`,
    maxInstances        : 1,
    initPriority        : -1 /* lower numbers init earlier */
};

/* get family-specific BatteryMonitor module */
let deviceTemperature = system.getScript("/ti/drivers/batterymonitor/BatteryMonitor" + family);
exports = deviceTemperature.extend(base);
