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
 *  ======== I2SLPF3.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");
let logWarning = Common.logWarning;

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base I2S configuration
 */
let devSpecific = {
    config: [
        {
            name        : "enableCCLK",
            legacyNames : ["enableMCLK"],
            displayName : "Enable CCLK",
            default     : false
        },
        {
            displayName: "Audio Frequency Clock Options",
            config: [
                {
                    name: "afclkSrc",
                    displayName: "Audio Frequency Clock Source",
                    description: "Audio frequency clock source",
                    longDescription: `This option selects the clock source for
the Audio Frequency Clock (also called AFCLK). For details on which AFCLK source
to select, please refer to [Selecting the Audio Frequency Clock](drivers/doxygen/html/_i2_s_l_p_f3_8h.html#ti_drivers_i2s_I2SLPF3_AFCLK).
`,
                    readOnly: false,
                    hidden: false,
                    options: [
                        { name: "I2SLPF3_AFCLK_SRC_CLKAF", displayName: "Auxiliary Frequency Oscillator (AFOSC)" },
                        { name: "I2SLPF3_AFCLK_SRC_CLKHF", displayName: "96MHz CLKHF" },
                        { name: "I2SLPF3_AFCLK_SRC_CLKREF", displayName: "48MHz reference clock (HFXT)" }
                    ],
                    default: "I2SLPF3_AFCLK_SRC_CLKHF",
                    onChange: (inst, ui) => {
                        ui.afoscFreq.hidden = inst.afclkSrc != "CKMD_AFCLKSEL_SRC_CLKAF";
                    }
                },
                {
                    name: "afoscFreq",
                    displayName: "Auxiliary Frequency Oscillator (AFOSC) Frequency",
                    description: "Select the the frequency of AFOSC",
                    readOnly: false,
                    hidden: true,
                    default: "PowerLPF3_AFOSC_FREQ_98P304MHZ",
                    options: [
                        {
                            name: "PowerLPF3_AFOSC_FREQ_98P304MHZ",
                            displayName: "98.304 MHz",
                            description: "Suitable for sample rates of 8 kHz, 16 kHz, 24 kHz, 32 kHz and 48 kHz."
                        },
                        {
                            name: "PowerLPF3_AFOSC_FREQ_90P3168MHZ",
                            displayName: "90.316 MHz",
                            description: "Suitable for sample rates of 22.05 kHz and 44.1 kHz."
                        },
                        {
                            name: "PowerLPF3_AFOSC_FREQ_80MHZ",
                            displayName: "80 MHz",
                            description: "Not intended for any standard sample rate, but it can still be selected if desired."
                        }
                    ]
                }
            ]
        }
    ],

    templates: {
        boardc : "/ti/drivers/i2s/I2SLPF3.Board.c.xdt",
        boardh : "/ti/drivers/i2s/I2S.Board.h.xdt"
    },

    pinmuxRequirements: pinmuxRequirements,
    moduleInstances: moduleInstances,
    modules: Common.autoForceModules(["Board", "Power"]),

    _getPinResources: _getPinResources
};

/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{
    let pin;
    let sckPin;
    let cclkPin;
    let wsPin;
    let sd0Pin;
    let sd1Pin;

    if (inst.i2s) {

        if (inst.i2s.SCKPin) {
            sckPin = inst.i2s.SCKPin.$solution.devicePinName.replace("_", "");
            pin = "\nSCK: " + sckPin;
        }

        if (inst.i2s.CCLKPin) {
            cclkPin = inst.i2s.CCLKPin.$solution.devicePinName.replace("_", "");
            pin += "\nCCLK: " + cclkPin;
        }

        if (inst.i2s.WSPin) {
            wsPin = inst.i2s.WSPin.$solution.devicePinName.replace("_", "");
            pin += "\nWS: " + wsPin;
        }

        if (inst.i2s.SD0Pin) {
            sd0Pin = inst.i2s.SD0Pin.$solution.devicePinName.replace("_", "");
            pin += "\nSD0: " + sd0Pin;
        }

        if (inst.i2s.SD1Pin) {
            sd1Pin = inst.i2s.SD1Pin.$solution.devicePinName.replace("_", "");
            pin += "\nSD1: " + sd1Pin;
        }

        if (inst.$hardware && inst.$hardware.displayName) {
            pin += "\n" + inst.$hardware.displayName;
        }
    }

    return (pin);
}

/*
 *  ======== moduleInstances ========
 *  returns PIN instances
 */
function moduleInstances(inst)
{
    let pinInstances = new Array();
    let shortName = inst.$name.replace("CONFIG_", "");

    if(inst.sd0DataDirection != "Disabled") {
        pinInstances.push({
            name: "sd0PinInstance",
            displayName: "SD0 configuration when not in use",
            moduleName: "/ti/drivers/GPIO",
            collapsed: true,
            requiredArgs: {
                parentInterfaceName: "i2s",
                parentSignalName: "SD0Pin",
                parentSignalDisplayName: "SD0"
            },
            args: {
                $name: "CONFIG_GPIO_" + shortName + "_SD0",
                mode: "Input",
                pull: "Pull Down"
            }
        });
    }

    if(inst.sd1DataDirection != "Disabled") {
        pinInstances.push({
            name: "sd1PinInstance",
            displayName: "SD1 configuration when not in use",
            moduleName: "/ti/drivers/GPIO",
            collapsed: true,
            requiredArgs: {
                parentInterfaceName: "i2s",
                parentSignalName: "SD1Pin",
                parentSignalDisplayName: "SD1"
            },
            args: {
                $name: "CONFIG_GPIO_" + shortName + "_SD1",
                mode: "Input",
                pull: "Pull Down"
            }
        });
    }

    pinInstances.push({
        name: "sckPinInstance",
        displayName: "SCK configuration when not in use",
        moduleName: "/ti/drivers/GPIO",
        collapsed: true,
        requiredArgs: {
            parentInterfaceName: "i2s",
            parentSignalName: "SCKPin",
            parentSignalDisplayName: "SCK"
        },
        args: {
            $name: "CONFIG_GPIO_" + shortName + "_SCK",
            mode: "Input",
            pull: "Pull Down"
        }
    });

    pinInstances.push({
        name: "wsPinInstance",
        displayName: "WS configuration when not in use",
        moduleName: "/ti/drivers/GPIO",
        collapsed: true,
        requiredArgs: {
            parentInterfaceName: "i2s",
            parentSignalName: "WSPin",
            parentSignalDisplayName: "WS"
        },
        args: {
            $name: "CONFIG_GPIO_" + shortName + "_WS",
            mode: "Input",
            pull: "Pull Down"
        }
    });

    if(inst.enableCCLK) {
        pinInstances.push({
            name: "cclkPinInstance",
            legacyNames: ["mclkPinInstance"],
            displayName: "CCLK configuration when not in use",
            moduleName: "/ti/drivers/GPIO",
            collapsed: true,
            requiredArgs: {
                parentInterfaceName: "i2s",
                parentSignalName: "CCLKPin",
                parentSignalDisplayName: "CCLK"
            },
            args: {
                $name: "CONFIG_GPIO_" + shortName + "_CCLK",
                mode: "Input",
                pull: "Pull Down"
            }
        });
    }

    return (pinInstances);
}

/*
 *  ======== pinmuxRequirements ========
 */
function pinmuxRequirements(inst)
{

    let sd0 = {
        name: "SD0Pin",
        displayName: "SD0 Pin",
        interfaceNames: ["DATA0"]
    };

    let sd1 = {
        name: "SD1Pin",
        displayName: "SD1 Pin",
        interfaceNames: ["DATA1"]
    };

    let sck = {
        name: "SCKPin",
        displayName: "SCK Pin",
        interfaceNames: ["SCLK"]
    };

    let ws = {
        name: "WSPin",
        displayName: "WS Pin",
        interfaceNames: ["WS"]
    };

    let cclk = {
        name: "CCLKPin",
        legacyNames: ["MCLKPin"],
        displayName: "CCLK Pin",
        interfaceNames: ["MCLK"]
    };

    let resources = [];

    if (inst.sd0DataDirection != "Disabled") {
        resources.push(sd0);
    }

    if (inst.sd1DataDirection != "Disabled") {
        resources.push(sd1);
    }

    resources.push(sck);
    resources.push(ws);

    if (inst.enableCCLK == true) {
        resources.push(cclk);
    }

    let i2s = {
        name: "i2s",
        displayName: "I2S Peripheral",
        description: "I2S Multichannel Audio Serial Port",
        interfaceName: "I2S",
        resources     : resources,
        signalTypes   : {
            SD0Pin     : ['I2S_SD0'],
            SD1Pin     : ['I2S_SD1'],
            SCKPin     : ['I2S_SCK'],
            WSPin      : ['I2S_WS'],
            CCLKPin    : ['I2S_CCLK']
        }
    };

    return ([i2s]);
}

/*
 *  ======== validate ========
 *  Validate this instance's configuration
 *
 *  param inst       - I2S instance to be validated
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

    if (inst.afclkSrc == "I2SLPF3_AFCLK_SRC_CLKAF") {
        if (system.modules["/ti/drivers/CAN"])
        {
                message = "The CAN driver is also using AFOSC. The CAN and I2S drivers must not be open at the same time.";
                logWarning(validation, inst, "afclkSrc", message);
        }
    }
}

/*
 *  ======== extend ========
 */
function extend(base)
{
    /* display which driver implementation can be used */
    base = Common.addImplementationConfig(base, "I2S", null,
        [{name: "I2SLPF3"}], null);

    /* override base validate */
    devSpecific.validate = function (inst, validation) {
        return validate(inst, validation, base);
    };

    /* overwrite base module attributes */
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);

    return (result);
}

/*
 *  ======== exports ========
 */
exports = {
    extend: extend
};
