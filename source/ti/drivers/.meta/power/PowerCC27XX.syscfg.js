/*
 * Copyright (c) 2022-2024, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== PowerCC27XX.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common   = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;
let isCName  = Common.isCName;

/* Array of power configurables for this device family */
let config = [
    /* Policy function handling */
    {
        name        : "policyFunction",
        displayName : "Policy Function",
        description : "Power policy function called from the idle loop.",
        longDescription:`
When enabled, this function is invoked in the idle loop, to opportunistically
select and activate sleep states.

Two reference policies are provided:

* __PowerCC27XX_standbyPolicy__
* __PowerCC27XX_doWFI__

In addition to this static selection, the Power Policy can be
dynamically changed at runtime, via the Power_setPolicy() API.
`,
        default     : "PowerCC27XX_standbyPolicy",
        onChange    : onChangePolicyFxn,
        options     :
        [
            {
                name: "PowerCC27XX_standbyPolicy",
                description: "An agressive policy that checks constraints and"
                  + " time until next scheduled work, and optionally chooses"
                  + " STANDBY, IDLE (power down), or WFI, in that"
                  + " order of preference."
            },
            {
                name: "PowerCC27XX_doWFI",
                description: "A simple policy that will invoke CPU wait for"
                    + " interrupt (WFI)."
            },
            {
                name: "Custom",
                description: "Custom policies can be written and specified"
                    + " via configuration of a Custom Policy Function."
            }
        ]
    },

    {
        name        : "policyCustomFunction",
        displayName : "Custom Policy Function",
        description : "User-provided Custom Policy Function.",
        default     : "customPolicyFxn",
        hidden      : true
    },

    {
        name        : "policyInitFunction",
        displayName : "Policy Init Function",
        default     : "Not Required",
        onChange    : onChangePolicyInitFxn,
        description : "The initialization function for the Power policy.",
        options     :
        [
            {
                name: "Not Required",
                description: "The selected Policy Function does not require an"
                  + " Init function."
            },
            {
                name: "Custom",
                description: "A custom initialization function can be written"
                  + " and specified via configuration of a Custom Policy Init"
                  + " Function."
            }
        ]
    },

    {
        name        : "policyInitCustomFunction",
        displayName : "Custom Policy Init Function",
        description : "User-provided Custom Policy Init Function. " +
                      "Usage not typical.",
        default     : "customPolicyInitFxn",
        hidden      : true
    },

    {
        name: "loggingEnabled",
        displayName: "Enable Logging",
        hidden : false,
        description: `This setting will enable logging for the Power module.`,
        default: false
    }
];

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base Power configuration
 */
let devSpecific = {
    getClockFrequencies : getClockFrequencies,
    moduleStatic        : {
        config   : config,
        moduleInstances: moduleInstances,
        validate : validate,
        modules: Common.autoForceModules(["Board", "ti/devices/CCFG"])
    },
    templates           : {
        boardc      : "/ti/drivers/power/PowerCC27XX.Board.c.xdt",
        board_initc : "/ti/drivers/power/PowerCC27XX.Board_init.c.xdt"
    }
};

/*
 *  ======== onChangePolicyInitFxn ========
 *  onChange callback function for the policyInitFunction config
 */
function onChangePolicyInitFxn(inst, ui)
{
    let subState = (inst.policyInitFunction !== 'Custom');
    ui.policyInitCustomFunction.hidden = subState;
}

/*
 *  ======== onChangePolicyFxn ========
 *  onChange callback function for the policyFunction config
 */
function onChangePolicyFxn(inst, ui)
{
    let subState = (inst.policyFunction !== 'Custom');
    ui.policyCustomFunction.hidden = subState;
}

/*
 *  ======== getClockFrequencies ========
 *  Return the value of the CC27XX main CPU clock frequency
 */
function getClockFrequencies(clock)
{
    return [ 96000000 ];
}

/*
 *  ======== moduleInstances ========
 *  returns Power instances
 */
function moduleInstances(inst)
{
    let powerInstances = new Array();

    /* If logging is enabled, push a dependency on a log module */
    if (inst.loggingEnabled) {
        powerInstances.push(
            {
                name: "LogModule",
                displayName: "Power Log Configuration",
                moduleName: "/ti/log/LogModule",
                collapsed: true,
                requiredArgs: {
                    $name: "LogModule_Power"
                },
                args: {
                    enable_DEBUG: false,
                    enable_INFO: false,
                    enable_VERBOSE: false,
                    // Only enable WARNING and ERROR enabled by default
                    enable_WARNING: true,
                    enable_ERROR: true
                }
            }
        );
    }

    return (powerInstances);
}

/*
 *  ======== validate ========
 *  Validate this module's configuration
 *
 *  param inst - Power instance to be validated
 *  param vo   - Issue reporting object
 *
 */
function validate(inst, vo)
{
    if (inst.policyInitFunction === 'Custom') {
        if (!isCName(inst.policyInitCustomFunction)) {
            logError(vo, inst, "policyInitCustomFunction",
                "Not a valid C identifier");
        }
        if (inst.policyInitCustomFunction === '') {
            logError(vo, inst, "policyInitCustomFunction",
                "Must contain a valid function name if the " +
                "Policy Init Function field == 'Custom'");
        }
    }
    if (inst.policyFunction === 'Custom') {
        if (!isCName(inst.policyCustomFunction)) {
            logError(vo, inst, "policyCustomFunction",
                "is not a valid C identifier");
        }
        if (inst.policyCustomFunction === '') {
            logError(vo, inst, "policyCustomFunction",
                "Must contain a valid function name if the " +
                "Policy Function field == 'Custom'");
        }
    }
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic Power module to
 *  allow us to augment/override as needed for the CC27XX
 */
function extend(base)
{
    /* display which driver implementation can be used */
    devSpecific = Common.addImplementationConfig(devSpecific, "Power", null,
        [{name: "PowerCC27XX"}], null);

    return (Object.assign({}, base, devSpecific));
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base ADCBuf module */
    extend: extend,
    getClockFrequencies: getClockFrequencies
};
