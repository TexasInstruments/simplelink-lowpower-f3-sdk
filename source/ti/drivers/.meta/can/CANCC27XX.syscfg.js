/*
 * Copyright (c) 2023-2026 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== CANCC27XX.syscfg.js ========
 */

"use strict";

/* Get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;
let logWarning = Common.logWarning;

const deviceId = system.deviceData.deviceId;

let freertosModule = system.modules["/freertos/FreeRTOS"];

/* The default task stack size is based on worst case usage which is currently
 * dictated by FreeRTOS with GCC compiler with ~110B headroom. Size must be a
 * word multiple. For CC27XXX7/CC27XXX10 flash devices only due to errata SYS_211.
 */
const defaultTaskStackSize = 512;
/* The default task priority is set to two less than the max priorities
 * configured in FreeRTOS. This gives CAN IRQ task a high priority while still
 * allowing for higher priority tasks if needed. An arbitrary mid-priority value
 * is used if FreeRTOS module is not present. For CC27XXX7/CC27XXX10 flash devices
 * only due to errata SYS_211.
 */
const defaultTaskPriority = (freertosModule !== undefined) ? freertosModule.$static.maxPriorities - 2 : 6;

/* IRQ task configuration options for CC27XXX7/CC27XXX10 flash devices due to errata SYS_211 */
let irqTaskConfigs = [
    {
        name: "irqTaskPriority",
        displayName: "Interrupt Task Priority",
        description: "Priority of the interrupt handling task.",
        longDescription: "Higher numbers denote higher priority. The max value depends on the RTOS configuration. " +
            "It is recommended to use a high priority to ensure timely processing of CAN interrupts.",
        default: defaultTaskPriority
    },
    {
        name: "taskStackSize",
        displayName: "IRQ Task Stack Size",
        description: "Stack size (in bytes) for the CAN interrupt handling task.",
        longDescription: "Configures the stack size for the interrupt handling task. Adjust as needed for your " +
            "application's event callback stack usage.",
        default: defaultTaskStackSize
    }
];

let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "CAN peripheral interrupt priority";

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base CAN configuration
 */
let devSpecific = {
    config: [
        intPriority,
        ...(deviceId.match(/CC274[4-5][A-Z](7|10)/) ? irqTaskConfigs : [])
    ],

    /* Override generic requirements with device-specific reqs (if any) */
    pinmuxRequirements: pinmuxRequirements,
    _getPinResources: _getPinResources,

    /* PIN instances */
    moduleInstances: moduleInstances,

    /* Override generic filterHardware with ours */
    filterHardware: filterHardware
};

/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{
    let pin;
    let rxPinName = "Unassigned";
    let txPinName = "Unassigned";

    if (inst.can) {
        if (inst.can.txPin) {
            txPinName = inst.can.txPin.$solution.devicePinName.replace("_", "");
        }
        if (inst.can.rxPin) {
            rxPinName = inst.can.rxPin.$solution.devicePinName.replace("_", "");
        }

        pin = "\nTx: " + txPinName + "\nRx: " + rxPinName;

        if (inst.$hardware && inst.$hardware.displayName) {
            pin += "\n" + inst.$hardware.displayName;
        }
    }

    return (pin);
}

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config
 */
function pinmuxRequirements(inst)
{
    let can = {
        name: "can",
        displayName: "CAN Peripheral",
        interfaceName: "CAN",
        canShareWith: "CAN",
        resources: [
            {
                name: "txPin",         /* Config script name */
                displayName: "Tx Pin", /* GUI name */
                description: "CAN Tx", /* GUI description */
                interfaceNames: ["TX"] /* Pinmux tool name */
            },
            {
                name: "rxPin",         /* Config script name */
                displayName: "Rx Pin", /* GUI name */
                description: "CAN Rx", /* GUI description */
                interfaceNames: ["RX"] /* Pinmux tool name */
            }
        ],
        signalTypes: {
            txPin: ["CAN_TX"],
            rxPin: ["CAN_RX"]
        }
    };

    return ([can]);
}

/*
 *  ======== filterHardware ========
 *  Check 'component' signals for compatibility with CAN
 *
 *  param component - hardware object describing signals and
 *                     resources they're attached to
 *
 *  returns Boolean indicating whether or not to allow the component to
 *           be assigned to an instance's $hardware config
 */
function filterHardware(component)
{
    return (Common.typeMatches(component.type, ["CAN"]));
}

/*
 *  ======== moduleInstances ========
 *  returns PIN instances
 */
function moduleInstances(inst) {
    let pinInstances = new Array();
    let shortName = inst.$name.replace("CONFIG_", "");

    pinInstances.push(
        {
            name: "txPinInstance",
            displayName: "CAN Tx configuration when not in use",
            moduleName: "/ti/drivers/GPIO",
            collapsed: true,
            requiredArgs: {
                parentInterfaceName: "can",
                parentSignalName: "txPin",
                parentSignalDisplayName: "TX"
            },
            args: {
                $name: "CONFIG_GPIO_" + shortName + "_TX",
                mode: "Input",
                pull: "None"
            }
        }
    );

    pinInstances.push(
        {
            name: "rxPinInstance",
            displayName: "CAN Rx configuration when not in use",
            moduleName: "/ti/drivers/GPIO",
            collapsed: true,
            requiredArgs: {
                parentInterfaceName: "can",
                parentSignalName: "rxPin",
                parentSignalDisplayName: "RX"
            },
            args: {
                $name: "CONFIG_GPIO_" + shortName + "_RX",
                mode: "Input",
                pull: "None"
            }
        }
    );

    return (pinInstances);
}

/*
 *  ======== validate ========
 *  Validate this instance's configuration
 *
 *  @param inst       - instance to be validated
 *  @param validation - object to hold detected validation issues
 *  @param $super     - needed to call the generic module's functions
 */
function validate(inst, validation, $super)
{
    /* Validate task configs for CC27XXX7/CC27XXX10 flash devices due to errata SYS_211 */
    if (deviceId.match(/CC274[4-5][A-Z](7|10)/)) {
        let message;
        let freertosModule = system.modules["/freertos/FreeRTOS"];

        if (freertosModule === undefined) {
            message = 'FreeRTOS module is required for the CAN driver';
            validation.logError(message, inst);
        }
        else {
            let maxPriority = freertosModule.$static.maxPriorities - 1;

            if (inst.irqTaskPriority < 0) {
                message = 'Task priority cannot be negative';
                logError(validation, inst, "irqTaskPriority", message);
            }
            else if (inst.irqTaskPriority > maxPriority) {
                message = 'Task priority cannot be greater than ' + maxPriority;
                logError(validation, inst, "irqTaskPriority", message);
            }
            else if (inst.irqTaskPriority < (maxPriority / 2)) {
                message = 'It is recommended to use a higher task priority';
                logWarning(validation, inst, "irqTaskPriority", message);
            }
        }

        if ((inst.taskStackSize & 0x3) != 0) {
            message = 'Task stack size must be a word multiple';
            logError(validation, inst, "taskStackSize", message);
        }
        else if (inst.taskStackSize < defaultTaskStackSize) {
            message = 'Task stack size is less than the recommended minimum ' +
                defaultTaskStackSize + ' bytes';
            logWarning(validation, inst, "taskStackSize", message);
        }
    }

    /* Call generic module's validation */
    if ($super.validate) {
        $super.validate(inst, validation);
    }
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic CAN module to
 *  allow us to augment/override as needed
 */
function extend(base)
{
    /* Display which driver implementation can be used */
    base = Common.addImplementationConfig(base, "CAN", null,
        [{name: "CANCC27XX"}], null);

    /* Override base validate */
    devSpecific.validate = function (inst, validation) {
        return validate(inst, validation, base);
    };

    /* Merge and overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* Concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* Required function, called by base CAN module */
    extend
};
