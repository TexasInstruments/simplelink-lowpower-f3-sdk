/*
 * Copyright (c) 2022-2024, Texas Instruments Incorporated - https://www.ti.com
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
 *  ======== ComparatorLPF3LP.syscfg.js ========
 */
"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "Analog Comparator peripheral interrupt priority";

/*
 *  ======= devSpecific =====
 *  Device-specific extensions to be added to base Comparator configuration
 */
let devSpecific = {
    config: [
        {
            name: "positiveInputSource",
            displayName: "Positive Input Source",
            default: "External Pin",
            description: "Specifies Comparator's positive terminal input source.",
            options: [
                { name: "External Pin" },
                { name: "Internal reference VDDS", legacyNames: ["Internal reference VDDA"] }
            ]
        },
        {
            name: "negativeInputSource",
            displayName: "Negative Input Source",
            default: "Internal reference VDDS",
            description: "Specifies Comparator's negative terminal input source.",
            options: [
                { name: "External Pin" },
                { name: "Internal reference VDDS", legacyNames: ["Internal reference VDDA"] },
                { name: "Internal reference VDDD" }
            ]
        },
        {
            name: "voltageDividerPath",
            displayName: "Voltage Divider Path",
            default: "Negative Input",
            description: "Specifies which Comparator input the voltage divider is applied to.",
            options: [
                { name: "Positive Input" },
                { name: "Negative Input" }
            ]
        },
        {
            name: "voltageDividerRatio",
            displayName: "Voltage Divider Ratio",
            default: "1/1",
            description: "Specifies Comparator's voltage divider ratio applied to one of the two inputs.",
            options: [
                { name: "1/1" },
                { name: "3/4" },
                { name: "1/2" },
                { name: "1/3" },
                { name: "1/4" }
            ]
        },
        intPriority,
        {
            name: "interruptNumber",
            displayName: "Interrupt Number",
            default: "INT_CPUIRQ0",
            description: "Specifies the configurable interrupt line to be used uniquely by the Comparator.",
            longDescription: "This value is ignored if the Comparator driver is opened with parameter "
                + ".interruptTrigger = Comparator_TRIGGER_NONE.",
            options: [
                { name: "INT_CPUIRQ0" },
                { name: "INT_CPUIRQ2" }
            ]
        }
    ],

    /* Override default pinmuxRequirements with device-specific requirements */
    pinmuxRequirements: pinmuxRequirements,

    templates: {
        boardc: "/ti/drivers/comparator/ComparatorLPF3LP.Board.c.xdt",
        boardh: "/ti/drivers/comparator/Comparator.Board.h.xdt"
    },

    _getPinResources: _getPinResources,

    /* PIN instances */
    moduleInstances: moduleInstances

};

/*
 *  ========= _getPinResources ===========
 *
 *
 *
 */
function _getPinResources(inst)
{
    let pin;
    /* Default to this, otherwise will be overwritten */
    let positivePin = "External Pin";
    let negativePin = "Internal reference VDDS";

    if(inst.comparator) {
        if(inst.comparator.positivePin) {
            positivePin = inst.comparator.positivePin.$solution.devicePinName;
        }
        if(inst.comparator.negativePin) {
            negativePin = inst.comparator.negativePin.$solution.devicePinName;
        }
        pin = "\nPositive Terminal: " + positivePin + "\nNegative terminal: "
                + negativePin;
        if(inst.$hardware && inst.$hardware.displayName) {
            pin += ", " + inst.$hardware.displayName;
        }
    }

    return (pin);
}

/*
 *  ========= pinmuxRequirements =========
 *  Returns a peripheral pin requirements of the specified instance
 *
 *  @param inst     - a fully configured Comparator instance
 *
 *  @returns req[]  - an array of pin requirements needed by inst
 */
function pinmuxRequirements(inst)
{
    let comparator = {
        name: "comparator",
        displayName: "Low Power Comparator Peripheral",
        interfaceName: "LPC",
        canShareWith: "LPC",
        resources: [],
        signalTypes: {
            "posPin" : "AIN",
            "negPin" : "AIN"
        }
    };

    /* Only request positive pin if needed */
    if(inst.positiveInputSource === "External Pin") {
        comparator.resources.push(
            {
                name: "positivePin",
                hidden: false,
                displayName: "Positive Pin",
                interfaceNames: [
                    "PLUS"
                ]
            }
        );
    }

    /* Only request negative pin if needed */
    if(inst.negativeInputSource === "External Pin") {
        comparator.resources.push(
            {
                name: "negativePin",
                hidden: false,
                displayName: "Negative Pin",
                interfaceNames: [
                    "MINUS"
                ]
            }
        );
    }

    return ([comparator]);
}

/*
 *  ======== moduleInstances ========
 *  returns PIN instances
 */
function moduleInstances(inst)
{
    let pinInstances = new Array();
    let shortName = inst.$name.replace("CONFIG_", "");

    if(inst.positiveInputSource !== "Internal reference VDDS") {
        pinInstances.push(
            {
                name: "positivePinInstance",
                displayName: "Positive input pin configuration",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                requiredArgs: {
                    parentInterfaceName: "comparator",
                    parentSignalName: "positivePin",
                    parentSignalDisplayName: "Positive Pin"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_PLUS",
                    mode: "Input",
                    pull: "None"
                }
            }
        );
    }
    if(inst.negativeInputSource !== "Internal reference VDDS" &&
       inst.negativeInputSource !== "Internal reference VDDD") {
        pinInstances.push(
            {
                name: "negativePinInstance",
                displayName: "Negative input pin configuration",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                requiredArgs: {
                    parentInterfaceName: "comparator",
                    parentSignalName: "negativePin",
                    parentSignalDisplayName: "Negative Pin"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_MINUS",
                    mode: "Input",
                    pull: "None"
                }
            }
        );
    }

    return (pinInstances);
}

/*
 *  ======= extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic Comparator module to
 *  allow us to augment/override as needed.
 */
function extend(base)
{
    /* display which driver implementation can be used */
    base = Common.addImplementationConfig(base, "Comparator", null,
    [{name: "ComparatorLPF3LP"}], null);

    let result = Object.assign({}, base, devSpecific);

    result.config = base.config.concat(devSpecific.config);

    return (result);

}

/*
 *  ======= exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by Comparator module */
    extend: extend
};