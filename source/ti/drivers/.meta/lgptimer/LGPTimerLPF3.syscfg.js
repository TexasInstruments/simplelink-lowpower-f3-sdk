/*
 * Copyright (c) 2023 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== LGPTimerLPF3.syscfg.js ========
 */
"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base Timer configuration
 */
let devSpecific = {
    config: [
        {
            name        : "channel0Connection",
            displayName : "Channel 0 Connection",
            default     : 'Internal',
            options     : [
                { name : 'Internal' },
                { name : 'Pin' }
            ]
        },
        {
            name        : "channel1Connection",
            displayName : "Channel 1 Connection",
            default     : 'Internal',
            options     : [
                { name : 'Internal' },
                { name : 'Pin' }
            ]
        },
        {
            name        : "channel2Connection",
            displayName : "Channel 2 Connection",
            default     : 'Internal',
            options     : [
                { name : 'Internal' },
                { name : 'Pin' }
            ]
        },
        {
            name        : "channel0NConnection",
            displayName : "Complementary Channel 0 Connection",
            default     : 'Internal',
            options     : [
                { name : 'Internal' },
                { name : 'Pin' }
            ]
        },
        {
            name        : "channel1NConnection",
            displayName : "Complementary Channel 1 Connection",
            default     : 'Internal',
            options     : [
                { name : 'Internal' },
                { name : 'Pin' }
            ]
        },
        {
            name        : "channel2NConnection",
            displayName : "Complementary Channel 2 Connection",
            default     : 'Internal',
            options     : [
                { name : 'Internal' },
                { name : 'Pin' }
            ]
        }
    ],

    /* referenced LGPTimer module instances */
    pinmuxRequirements: pinmuxRequirements,

    /* PIN instances */
    moduleInstances: moduleInstances,

    templates: {
        boardc : "/ti/drivers/lgptimer/LGPTimerLPF3.Board.c.xdt",
        boardh : "/ti/drivers/lgptimer/LGPTimerLPF3.Board.h.xdt"
    },

    _getPinResources: _getPinResources
};

/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{
    let pin;
    let ch0Pin  = "Unassigned";
    let ch0NPin = "Unassigned";
    let ch1Pin  = "Unassigned";
    let ch1NPin = "Unassigned";
    let ch2Pin  = "Unassigned";
    let ch2NPin  = "Unassigned";

    if (inst.timer) {
        if (inst.timer.ch0Pin) {
            ch0Pin = inst.timer.ch0Pin.$solution.devicePinName.replace("_", "");
        }
        if (inst.timer.ch0NPin) {
            ch0NPin = inst.timer.ch0NPin.$solution.devicePinName.replace("_", "");
        }
        pin = "\nCH0: " + ch0Pin + "\nCH0N: " + ch0NPin;

        if (inst.timer.ch1Pin) {
            ch1Pin = inst.timer.ch1Pin.$solution.devicePinName.replace("_", "");
        }
        if (inst.timer.ch1NPin) {
            ch1NPin = inst.timer.ch1NPin.$solution.devicePinName.replace("_", "");
        }
        pin = "\nCH1: " + ch1Pin + "\nCH1N: " + ch1NPin;

        if (inst.timer.ch2Pin) {
            ch2Pin = inst.timer.ch2Pin.$solution.devicePinName.replace("_", "");
        }
        if (inst.timer.ch2NPin) {
            ch2NPin = inst.timer.ch2NPin.$solution.devicePinName.replace("_", "");
        }
        pin = "\nCH2: " + ch2Pin + "\nCH2N: " + ch2NPin;

        if (inst.$hardware && inst.$hardware.displayName) {
            pin += "\n" + inst.$hardware.displayName;
        }
    }

    return (pin);
}

/*
 *  ======== pinmuxRequirements ========
 */
function pinmuxRequirements(inst)
{
    let ch0 = {
        name           : "ch0Pin",    /* config script name */
        displayName    : "Ch0 Pin",   /* GUI name */
        interfaceNames : ["C0"]       /* pinmux tool name */
    };

    let ch0N = {
        name           : "ch0NPin",    /* config script name */
        displayName    : "Ch0N Pin",   /* GUI name */
        interfaceNames : ["C0N"]       /* pinmux tool name */
    };

    let ch1 = {
        name           : "ch1Pin",    /* config script name */
        displayName    : "Ch1 Pin",   /* GUI name */
        interfaceNames : ["C1"]       /* pinmux tool name */
    };

    let ch1N = {
        name           : "ch1NPin",    /* config script name */
        displayName    : "Ch1N Pin",   /* GUI name */
        interfaceNames : ["C1N"]       /* pinmux tool name */
    };

    let ch2 = {
        name           : "ch2Pin",    /* config script name */
        displayName    : "Ch2 Pin",   /* GUI name */
        interfaceNames : ["C2"]       /* pinmux tool name */
    };

    let ch2N = {
        name           : "ch2NPin",    /* config script name */
        displayName    : "Ch2N Pin",   /* GUI name */
        interfaceNames : ["C2N"]       /* pinmux tool name */
    };
    let resources = [];

    if (inst.channel0Connection != 'Internal') {
        resources.push(ch0);
    }
    if (inst.channel0NConnection != 'Internal') {
        resources.push(ch0N);
    }
    if (inst.channel1Connection != 'Internal') {
        resources.push(ch1);
    }
    if (inst.channel1NConnection != 'Internal') {
        resources.push(ch1N);
    }
    if (inst.channel2Connection != 'Internal') {
        resources.push(ch2);
    }
    if (inst.channel2NConnection != 'Internal') {
        resources.push(ch2N);
    }

    let timer = {
        name          : "timer",
        displayName   : "Timer Peripheral",
        interfaceName : "LGPT",
        resources     : resources,
        signalTypes   : {
            ch0Pin    : ['LGPT_C0'],
            ch0NPin   : ['LGPT_C0N'],
            ch1Pin    : ['LGPT_C1'],
            ch1NPin   : ['LGPT_C1N'],
            ch2Pin    : ['LGPT_C2'],
            ch2NPin   : ['LGPT_C2N']
        }
    };

    return ([timer]);
}

/*
 *  ======== moduleInstances ========
 *  returns PIN instances
 */
function moduleInstances(inst)
{
    let pinInstances = new Array();
    let shortName = inst.$name.replace("CONFIG_", "");

    if (inst.channel0Connection == "Pin") {
        pinInstances.push(
            {
                name: "ch0PinInstance",
                displayName: "CH0 configuration when not in use",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                requiredArgs: {
                    parentInterfaceName: "timer",
                    parentSignalName: "ch0Pin",
                    parentSignalDisplayName: "CH0"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_CH0",
                    initialOutputState: "Low",
                    mode: "Output",
                    pull: "None"
                }
            }
        );
    }

    if (inst.channel0NConnection == "Pin") {
        pinInstances.push(
            {
                name: "ch0NPinInstance",
                displayName: "Complementary CH0 configuration when not in use",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                requiredArgs: {
                    parentInterfaceName: "timer",
                    parentSignalName: "ch0NPin",
                    parentSignalDisplayName: "CH0N"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_CH0N",
                    initialOutputState: "High",
                    mode: "Output",
                    pull: "None"
                }
            }
        );
    }

    if (inst.channel1Connection == "Pin") {
        pinInstances.push(
            {
                name: "ch1PinInstance",
                displayName: "CH1 configuration when not in use",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                requiredArgs: {
                    parentInterfaceName: "timer",
                    parentSignalName: "ch1Pin",
                    parentSignalDisplayName: "CH1"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_CH1",
                    initialOutputState: "Low",
                    mode: "Output",
                    pull: "None"
                }
            }
        );
    }

    if (inst.channel1NConnection == "Pin") {
        pinInstances.push(
            {
                name: "ch1NPinInstance",
                displayName: "Complementary CH1 configuration when not in use",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                requiredArgs: {
                    parentInterfaceName: "timer",
                    parentSignalName: "ch1NPin",
                    parentSignalDisplayName: "CH1N"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_CH1N",
                    initialOutputState: "High",
                    mode: "Output",
                    pull: "None"
                }
            }
        );
    }

    if (inst.channel2Connection == "Pin") {
        pinInstances.push(
            {
                name: "ch2PinInstance",
                displayName: "CH2 configuration when not in use",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                requiredArgs: {
                    parentInterfaceName: "timer",
                    parentSignalName: "ch2Pin",
                    parentSignalDisplayName: "CH2"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_CH2",
                    initialOutputState: "Low",
                    mode: "Output",
                    pull: "None"
                }
            }
        );
    }

    if (inst.channel2NConnection == "Pin") {
        pinInstances.push(
            {
                name: "ch2NPinInstance",
                displayName: "Complementary CH2 configuration when not in use",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                requiredArgs: {
                    parentInterfaceName: "timer",
                    parentSignalName: "ch2NPin",
                    parentSignalDisplayName: "CH2N"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_CH2N",
                    initialOutputState: "High",
                    mode: "Output",
                    pull: "None"
                }
            }
        );
    }

    return (pinInstances);
}


/*
 *  ======== extend ========
 *  Extends a base exports object to include any device-specifics
 *
 *  This function is invoked by the generic Timer module to
 *  allow us to augment/override as needed for the LPF3SW.
 */
function extend(base)
{
    /* display which driver implementation can be used */
    base = Common.addImplementationConfig(base, "LGPTimer", null,
        [{name: "LGPTimerLPF3"}], null);

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
    /* required function, called by base Timer module */
    extend: extend
};
