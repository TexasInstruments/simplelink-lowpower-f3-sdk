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
 *  ======== ADCLPF3.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base ADC configuration
 */
let devSpecific = {
    config: [
        {
            name: "referenceSource",
            displayName: "Reference Source",
            default: "VDDS",
            description: "Specifies the ADC's reference source.",
            options: [
                { name: "VDDS" },
                { name: "Internal 1.4V" },
                { name: "Internal 2.5V" },
                { name: "External reference" }
            ],
            onChange: onReferenceSourceChange
        },
        {
            name: "referenceVoltage",
            displayName: "Reference Voltage",
            description: "Specifies the ADC's reference voltage in microvolts.",
            default: 3300000,
            readOnly: false
        },
        {
            name: "resolution",
            displayName: "Resolution",
            description: "Specifies the ADC's resolution",
            default: "12 Bits",
            hidden: false,
            options: [
                { name: "12 Bits" },
                { name: "10 Bits" },
                { name: "8 Bits" }
            ]
        },
        {
            name: "adcClockDivider",
            displayName: "ADC Clock Divider",
            description: "Specifies the divider value from system clock to ADC clock. This option, along with the " +
                "ADC sample duration (in adc clock-cycles) must ensure a minimum sampling time of 250 ns.",
            default: "8",
            options: [
                { name: "1" },
                { name: "2" },
                { name: "4" },
                { name: "8" },
                { name: "16" },
                { name: "24" },
                { name: "32" },
                { name: "48" }
            ]
        },
        {
            name: "adcSampleTime",
            displayName: "ADC Sample Duration (ADC Clk Cycles)",
            description: "Specifies the ADC sample duration in ADC clock cycles. A value of 0 will count as 1 cycle. " +
                "This option, along with the ADC clock divider must ensure a minimum sampling time of 250 ns.",
            default: 16
        },
        {
            name: "adjustSampleValue",
            displayName: "Adjust Sample Value",
            longDescription: "Specifies if the returned raw sample value is adjusted or not. If not, the returned value " +
                "might differ noticably between different devices. Any sample that is converted to micro-volts " +
                " through the driver API is automatically adjusted, regardless of this setting.",
            default: false
        },
        {
            name: "internalSignal",
            displayName: "Internal Signal",
            description: "Specifies internal signal(s) to use as a"
                + " sample source.",
            default: "None",
            options: [
                { name: "None" },
                {
                    name: "Battery Channel",
                    description: "Adds a channel to sample the internal"
                        + " battery voltage, VDDS."
                }
            ]
        }
    ],

    /* override generic requirements with device-specific reqs (if any) */
    pinmuxRequirements: pinmuxRequirements,

    modules: Common.autoForceModules(["Board", "Power"]),

    /* GPIO instances */
    moduleInstances: moduleInstances,

    templates: {
        boardc: "/ti/drivers/adc/ADCLPF3.Board.c.xdt",
        boardh: "/ti/drivers/adc/ADC.Board.h.xdt"
    },

    _getPinResources: _getPinResources,

    _pinToADCChannel: _pinToADCChannel
};


/*
 *  ======== _pinToADCChannel ========
 */
function _pinToADCChannel(devicePin)
{
    /* Input is a pin with a peripheralPinName of the format "ADC2" etc
     */
    let peripheralPinName = devicePin.peripheralPinName.substring(3);
    return parseInt(peripheralPinName, 10);
}


/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{
    let pin;

    if (inst.internalSignal === "None") {
        let devicePinName = inst.adc.adcPin.$solution.devicePinName;
        pin = "DIO" + devicePinName.substring(4);
    }
    else {
        pin = inst.internalSignal;
    }

    return (pin);
}


/*
 *  ======== pinmuxRequirements ========
 *  Returns peripheral pin requirements of the specified instance
 */
function pinmuxRequirements(inst)
{

    if (inst.internalSignal != "None" && inst.referenceSource != "External reference") {
        return ([]);
    }

    let adc = {
        name: "adc",
        hidden: true,
        displayName: "ADC Peripheral",
        interfaceName: "ADC",
        canShareWith: "ADC",
        resources: [

        ],
        signalTypes: {
            adcPin: ["AIN"],
            refPinPos: ["AIN"],
            refPinNeg: ["AIN"]
        }
    };

    let adcPinResources = [
        {
            name: "adcPin",
            canShareWith: "ADC",
            hidden: false,
            displayName: "ADC Pin",
            interfaceNames: [
                "PIN0", "PIN1", "PIN2", "PIN3",
                "PIN4", "PIN5", "PIN6", "PIN7",
                "PIN8", "PIN9", "PIN10", "PIN11"
            ]
        }
    ];

    let referenceResources = [
        {
            name: "refPinPos",
            canShareWith: "ADC",
            hidden: false,
            displayName: "ADC Reference Pin, Positive",
            interfaceNames: [
                "REF_P"
            ]
        },
        {
            name: "refPinNeg",
            canShareWith: "ADC",
            hidden: false,
            displayName: "ADC Reference Pin, Negative",
            interfaceNames: [
                "REF_N"
            ]
        }
    ];

    if (inst.internalSignal == "None") {
        adc.resources = adc.resources.concat(adcPinResources);
    }

    if (inst.referenceSource == "External reference") {
        adc.resources = adc.resources.concat(referenceResources);
    }

    return ([adc]);
}

/*
 *  ======== moduleInstances ========
 *  returns GPIO instances
 */
function moduleInstances(inst)
{
    /* This avoids constructions like CONFIG_GPIO_CONFIG_ADC_0_AIN */
    let shortName = inst.$name.replace("CONFIG_", "");
    let gpioInstances = new Array();

    if (inst.internalSignal == "None") {
        gpioInstances.push(
            {
                name: "adcPinInstance",
                displayName: "ADC input pin configuration while not in use",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                requiredArgs: {
                    parentInterfaceName: "adc",
                    parentSignalName: "adcPin",
                    parentSignalDisplayName: "ADC Pin"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_AIN",
                    mode: "Input"
                }
            }
        );
    }

    if (inst.referenceSource == "External reference") {
        gpioInstances.push(
            {
                name: "adcRefPInstance",
                displayName: "ADC positive reference pin configuration while not in use",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                hidden: true,
                requiredArgs: {
                    parentInterfaceName: "adc",
                    parentSignalName: "refPinPos",
                    parentSignalDisplayName: "ADC Reference Pin, Positive"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_REF_P",
                    mode: "Input"
                }
            }
        );
        gpioInstances.push(
            {
                name: "adcRefNInstance",
                displayName: "ADC negative reference pin configuration while not in use",
                moduleName: "/ti/drivers/GPIO",
                collapsed: true,
                hidden: true,
                requiredArgs: {
                    parentInterfaceName: "adc",
                    parentSignalName: "refPinNeg",
                    parentSignalDisplayName: "ADC Reference Pin, Negative"
                },
                args: {
                    $name: "CONFIG_GPIO_" + shortName + "_REF_N",
                    mode: "Input"
                }
            }
        );
    }

    return (gpioInstances);
}

/*
 * ======== onReferenceSourceChange ========
 */
function onReferenceSourceChange(inst, ui)
{
    ui.referenceVoltage.readOnly = true;

    if (inst.referenceSource == "VDDS" || inst.referenceSource == "External reference") {
        ui.referenceVoltage.readOnly = false;
    }
}

/*
 *  ======== validate ========
 *  Validate this instance's configuration
 *
 *  param inst       - ADC instance to be validated
 *  param validation - object to hold detected validation issues
 *
 *  @param $super    - needed to call the generic module's functions
 */
function validate(inst, validation, $super)
{
    let message;

    if ($super.validate) {
        $super.validate(inst, validation);
    }

    if (inst.adcSampleTime < 0 || inst.adcSampleTime > 1023) {
        message = 'ADC Sample Duration must be between 0 and 1023';
        logError(validation, inst, "adcSampleTime", message);
    }
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic ADC module to
 *  allow us to augment and override as needed.
 */
function extend(base)
{
    /* display which driver implementation can be used */
    base = Common.addImplementationConfig(base, "ADC", null,
        [{ name: "ADCLPF3" }], null);

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
    /* required function, called by base ADC module */
    extend: extend
};
