/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== I2CTarget.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/* get /ti/drivers family name from device object */
let family = Common.device2Family(system.deviceData, "I2CTarget");

/* generic configuration parameters for I2CTarget instances */
let config = [];

/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *  @param inst - I2CTarget instance to be validated
 *  @param vo   - object to hold detected validation issues
 */
function validate(inst, vo) {
    /* Do nothing */
}

/*
 *  ========= filterHardware ========
 *  Check 'component' signals for compatibility with I2C
 *
 *  @param component - hardware object describing signals and
 *                     pins to which they're attached
 *
 *  @returns true iff component is recognized as an I2C bus
 */
function filterHardware(component) {
    let sda, scl;

    for (let sig in component.signals) {
        let type = component.signals[sig].type;
        if (Common.typeMatches(type, ["I2C_SCL"])) scl = sig;
        if (Common.typeMatches(type, ["I2C_SDA"])) sda = sig;
    }
    let result = (scl && sda) ? true : false;

    return (result);
}

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of an instance's config
 */
function pinmuxRequirements(inst) {
    let i2c = {
        /* name is the name of a property that will be added to inst;
         * this property's value is an object returned by the
         * pinmux solver
         */
        name: "i2c",
        displayName: "I2C Peripheral",

        interfaceName: "I2C",            /* pinmux interface name */

        /* request specific signals for I2C */
        resources: [
            {
                name: "sdaPin",          /* config script name */
                displayName: "SDA Pin",  /* GUI name */
                interfaceNames: ["SDA"]  /* pinmux tool name */
            },
            {
                name: "sclPin",
                displayName: "SCL Pin",
                interfaceNames: ["SCL"]
            }
        ]
    };

    /* map injected pinmux solution names to sig type names */
    i2c.signalTypes = {
        sdaPin: ["I2C_SDA"], /* warning: device-specific signal name */
        sclPin: ["I2C_SCL"]  /* warning: device-specific signal name */
    };

    return ([i2c]);
}

/*
 *  ======== _getPinResources ========
 */
/* istanbul ignore next */
function _getPinResources(inst) {
    return;
}

/*
 *  ======== base ========
 *  Define the base properties and methods
 */
let base = {
    /* generic sysconfig module interface */
    displayName: "I2CTarget",
    description: "Inter-Integrated Circuit (I2C) Bus Target Driver",

    longDescription: `
The [__I2CTarget driver__][1] provides a portable application
interface to act as a Target an I2C bus.

* [Usage Synopsis][2]
* [Examples][3]
* [Configuration Options][4]

[1]: /drivers/doxygen/html/_i2_c_target_8h.html#details "C API reference"
[2]: /drivers/doxygen/html/_i2_c_target_8h.html#ti_drivers_I2CTARGET_Synopsis "Basic C usage summary"
[3]: /drivers/doxygen/html/_i2_c_target_8h.html#ti_drivers_I2CTARGET_Examples "C usage examples"
[4]: /drivers/syscfg/html/ConfigDoc.html#I2C_Configuration_Options "Configuration options reference"
`,

    defaultInstanceName: "CONFIG_I2CTARGET_",

    /* instance properties and methods */
    config: Common.addNameConfig(
        config, "/ti/drivers/I2CTarget", "CONFIG_I2CTARGET_"),
    modules: Common.autoForceModules(["Board", "Power"]),
    validate: validate,

    /* pinmux interface */
    busModule: true, /* true => this module's insts are shareable */
    filterHardware: filterHardware,
    pinmuxRequirements: pinmuxRequirements,

    _getPinResources: _getPinResources
};

/* extend the base exports to include family-specific content */
let deviceI2C = system.getScript("/ti/drivers/i2ctarget/I2CTarget" + family);
exports = deviceI2C.extend(base);
