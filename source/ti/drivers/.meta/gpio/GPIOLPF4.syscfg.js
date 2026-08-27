/*
 * Copyright (c) 2025-2026, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== GPIOLPF4.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");
/* get device ID */
let deviceId = system.deviceData.deviceId;

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base GPIO configuration
 */
let devSpecific = {
    templates:
    {
        boardc: "/ti/drivers/gpio/GPIOLPF4.Board.c.xdt",
        board_initc: "/ti/drivers/gpio/GPIO.Board_init.c.xdt",
        boardh: "/ti/drivers/gpio/GPIO.Board.h.xdt"
    },

    validatePinmux: validatePinmux,

    _getPinResources: _getPinResources,
    _getDefaultAttrs: _getDefaultAttrs,
    _getHwSpecificAttrs: _getHwSpecificAttrs,
    _pinToDio: _pinToDio,
    _getPinMuxModeFromPin: _getPinMuxModeFromPin,
    _getShuffleNetworkSize: _getShuffleNetworkSize,
    _getShuffleNetworkRoutes: _getShuffleNetworkRoutes
};

/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst) {
    let pin;

    if (inst.gpioPin) {
        pin = inst.gpioPin.$solution.devicePinName;
        pin = pin.replace("_", "");

        if (inst.$hardware && inst.$hardware.displayName) {
            pin += ", " + inst.$hardware.displayName;
        }
    }

    return (pin);
}

/*
 *  ======== _getDefaultAttrs ========
 */
/* istanbul ignore next */
function _getDefaultAttrs(dioNumber)
{
    /* By default, SysConfig configures all pins to have no pull. However, this
     * should not occur for all DIOs. Some DIOs should configure a pull up/down
     * resistor by default, as seen from their reset value in the I/O controller
     * register summary. For such DIOs, their config value is set to
     * "GPIO_CFG_DO_NOT_CONFIG" so the reset value is not modified and the pull
     * is maintained.
     */
    if (deviceId.match(/CC283/))
    {
        if (dioNumber == "3" || dioNumber == "4")
        {
            return "GPIO_CFG_DO_NOT_CONFIG";
        }
    }
    return "GPIO_CFG_NO_DIR";
}

/*
 *  ======== _getHwSpecificAttrs ========
 */
function _getHwSpecificAttrs(inst) {
    return [];
}

/*
 *  ======== _pinToDio ========
 */
function _pinToDio(pinSolution, devicePin) {
    /* The description passed here comes from the device metadata
     * and is of the format DIOnn[_desc]. Replace gets rid of DIO,
     * then we split on underscore and parse the first chunk as an int.
     */
    let pinNumberParts = devicePin.description.replace("DIO", "").split("_");
    return parseInt(pinNumberParts[0], 10);
}


/*
 *  ======== _getPinMuxModeFromPin ========
 */
function _getPinMuxModeFromPin(pin) {

    /* The FSEL value is the first byte of the muxMode */
    return parseInt(pin.$solution.muxMode, 10) & 0xFF;
}

function _getShuffleNetworkSize()
{
    /* Ideally the shuffle network size should come directly from the device
     * metadata, but that is not available yet. So for now, it is hardcoded here
     * instead.
     */
    if (deviceId.match(/CC283/))
    {
        return 52;
    }
    else
    {
        throw new Error("Shuffle Network size is not known for this device");
    }
}

function _getShuffleNetworkRoutes(gpioModule)
{
    let routes = [];

    let instances = gpioModule.$instances;


    for (let instance of instances) {
        let pinSolution = gpioModule.getPinSolutionForInst(instance);

        let muxMode = parseInt(pinSolution.muxMode, 10);
        let fsel = (muxMode & 0xFF);
        let inputNumber = (muxMode >> 16) & 0xFF;
        let outputNumber = (muxMode >> 8) & 0xFF;

        let devicePin = system.deviceData.devicePins[pinSolution.packagePinName];

        /* The shuffle network is only used if the FSEL value is 5, 6, or 7 */
        if (fsel == 5 || fsel == 6 || fsel == 7)
        {
            let route = {
                input: inputNumber,
                output: outputNumber,
                pinSolution: pinSolution,
                devicePin: devicePin
            };
            routes.push(route);
        }
    }

    return routes;
}

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config
 *  Called on instantiation and every config change.
 *
 *  $super is needed to call the generic module's functions
 */
function pinmuxRequirements(inst, $super) {
    let result = $super.pinmuxRequirements ? $super.pinmuxRequirements(inst) : [];
    return (result);
}

/*
 *  ======== validatePinmux ========
 *  Validate this instance's pinmux
 *
 *  @param inst       - GPIO instance to be validated
 *  @param validation - object to hold detected validation issues
 */
function validatePinmux(inst, validation)
{
    let pinSolution = system.modules["/ti/drivers/GPIO"].getPinSolutionForInst(inst);

    let muxMode = parseInt(pinSolution.muxMode, 10);
    let fsel = (muxMode & 0xFF);
    let inputNumber = (muxMode >> 16) & 0xFF;
    let outputNumber = (muxMode >> 8) & 0xFF;

    /* The shuffle network is only used if the FSEL value is 5, 6, or 7 */
    if (fsel == 5 || fsel == 6 || fsel == 7)
    {
        let inputNumberCount = 0;
        let outputNumberCount = 0;
        let routes = _getShuffleNetworkRoutes(system.modules["/ti/drivers/GPIO"]);

        for (let route of routes)
        {
            if (route.input == inputNumber)
            {
                inputNumberCount++;
            }
            if (route.output == outputNumber)
            {
                outputNumberCount++;
            }
        }

        if (inputNumberCount > 1)
        {
            Common.logError(validation, inst, "pinPeripheral",
                "Peripheral signal \"" + pinSolution.peripheralPinName + "\" cannot be muxed to multiple DIOs."
            );
        }

        if (outputNumberCount > 1)
        {
            Common.logError(validation, inst, "pinPeripheral",
                "Multiple peripheral signals cannot be connected to " + pinSolution.devicePinName + " (with FSEL=" + fsel + ") through the Shuffle Network."
            );
        }
    }


}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic module to
 *  allow us to augment and override as needed.
 */
function extend(base) {
    /* override base pinmuxRequirements */
    devSpecific.pinmuxRequirements = function (inst) {
        return pinmuxRequirements(inst, base);
    };

    /* display which driver implementation can be used */
    base = Common.addImplementationConfig(
        base,
        "GPIO",
        null,
        [{ name: "GPIOLPF4" }],
        null
    );

    /* Merge and overwrite base module attributes */
    let mergedObject = (Object.assign({}, base, devSpecific));

    /* Append to the long description of the "alternativeMux" config */
    for (let config of mergedObject.config)
    {
        if (config.name === "alternativeMux") {
            config.longDescription += `\n
In addition, when selecting an alternative mux option which is muxed using the
Shuffle Network, the relevant data structures for the Shuffle Network will be
configured to route the selected signal to the selected DIO, such that the
application just need to make a call to
[\`GPIO_setConfigAndMux()\`](/drivers/doxygen/html/_g_p_i_o_8h.html#a53cc86d668c93f510dc3de7332410ebd)
to mux the signal to the IO.
`;
        }
    }

    return mergedObject;
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    extend: extend
};
