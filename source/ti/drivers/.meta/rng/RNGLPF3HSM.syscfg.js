/*
 * Copyright (c) 2023-2025 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== RNGLPF3HSM.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/* get device ID */
let deviceId = system.deviceData.deviceId;

/*
 *  ======== getLibs ========
 *  Argument to the /ti/utils/build/GenLibs.cmd.xdt template
 */
function getLibs(mod)
{
    /* Get device information from GenLibs */
    let GenLibs = system.getScript("/ti/utils/build/GenLibs");

    let libGroup = {
        name: "/third_party/hsmddk",
        deps: [],
        libs: [],
        allowDuplicates: true
    };

    /* Add the appropriate HSMDDK library for the current device */
    if (deviceId.match(/CC27/)) {
        if (!system.modules["/ti/utils/TrustZone"]) {
            libGroup.libs.push(GenLibs.libPath("third_party/hsmddk", "hsmddk_cc27xx_its.a"));
        }
    } else if (deviceId.match(/CC35/)) {
        libGroup.libs.push(GenLibs.libPath("third_party/hsmddk", "hsmddk_cc35xx.a"));
    }

    return (libGroup);
}

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base RNG configuration
 */
let devSpecific = {
    moduleStatic: {
        name: "rngSettings",
        displayName: "Settings for RNG",
        config: [
            {
                name        : "rngReturnBehavior",
                displayName : "Return Behavior",
                description : 'The return behavior (RNG_RETURN_BEHAVIOR) '
                            + 'which will be used for all instances of the '
                            + 'driver. All instances must be opened using '
                            + 'this setting.',
                default     : "RNG_RETURN_BEHAVIOR_POLLING",
                options     : [ {
                                    name: "RNG_RETURN_BEHAVIOR_POLLING",
                                    displayName: "Polling"
                                },
                                {
                                    name: "RNG_RETURN_BEHAVIOR_BLOCKING",
                                    displayName: "Blocking"
                                }
                              ]
            }
        ]
    },

    config: [],

    templates : {
        boardc: "/ti/drivers/rng/RNGLPF3HSM.Board.c.xdt",
        boardh: "/ti/drivers/rng/RNG.Board.h.xdt",

        /* contribute libraries to linker command file */
        "/ti/utils/build/GenLibs.cmd.xdt":
                {modName: "/ti/drivers/RNG", getLibs: getLibs}
    }
};

/*
 *  ======== validate ========
 *  Validate this instance's configuration
 *
 *  @param inst       - RNG instance to be validated
 *  @param validation - object to hold detected validation issues
 *
 *  @param $super    - needed to call the generic module's functions
 */
function validate(inst, validation, $super) {
    if ($super.validate) {
        $super.validate(inst, validation);
    }
}

/*
 *  ======== extend ========
 */
function extend(base)
{
    /* display which driver implementation can be used */
    base = Common.addImplementationConfig(base, "RNG", null,
        [{name: "RNGLPF3HSM"}], null);

    /* override base validate */
    devSpecific.validate = function (inst, validation) {
        return validate(inst, validation, base);
    };

    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base RNG module */
    extend: extend
};
