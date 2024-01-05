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
 *  ======== rcldriver.syscfg.js ========
 */

"use strict";

/* Common /ti/drivers utility functions */
const Common = system.getScript("/ti/drivers/Common.js");

const config = [

];

/*
 *******************************************************************************
 Status Functions
 *******************************************************************************
 */
/*
 *  ======== filterHardware ========
 *  Check component signals for compatibility with RCL.
 *
 *  @param component    - A hardware component
 *  @return             - Bool
 */
function filterHardware(component) {
    return (Common.typeMatches(component.type, ["RCL"]));
}



/*
 *******************************************************************************
 Get Functions
 *******************************************************************************
 */



/*
 *******************************************************************************
 OnChange functions
 *******************************************************************************
 */
 function onHardwareChanged(inst, ui) {

}


/*
 *******************************************************************************
 Update functions
 *******************************************************************************
 */


/*
 *******************************************************************************
 Module Dependencies
 *******************************************************************************
 * When change occurs within module, the default module functions (if declared)
 * will be triggered when a configurable changes state:
 *  - pinmuxRequirements(...)
 *  - moduleInstances(...)
 *  - sharedModuleInstances(...)
 *  - modules(...)
 */

/*!
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config.
 *
 *  Called when a configuration changes in the module.
 *
 *  @param inst - Module instance containing the config that changed
 *  @return     - Array of pin requirements
 */
function pinmuxRequirements(inst) {
    const rclArray = [];

    return rclArray;
}



/*
 *  ======== moduleInstances ========
 *  Determines what modules are added as non-static submodules.
 *
 *  Called when a configuration changes in the module.
 *
 *  @param inst - Module instance containing the config that changed
 *  @return     - Array of PIN instances
 */
function moduleInstances(inst) {
    const dependencyModules = [];

    return dependencyModules;
}

/*
 *  ======== sharedModuleInstances ========
 *  Determines what modules are added as shared static sub-modules.
 *
 *  Called when a configuration changes in the module.
 *
 *  @param inst - Module instance containing the config that changed
 *  @return     - Array containing dependency modules
 */
function sharedModuleInstances(inst){
    return [];
}

/*
 *  ======== modules ========
 *  Determines what modules are added as static sub-modules.
 *
 *  Called when a configuration changes in the module.
 *
 *  @param inst - Module instance containing the config that changed
 *  @return     - Array containing static dependency modules
 */
function modules(inst) {

    const dependencies = ["Board", "Power", "Temperature"];

    return Common.autoForceModules(dependencies)();
}
/*
 *******************************************************************************
 GenLibs
 *******************************************************************************
 */

/*!
 *  ======== getLibs ========
 *  Return link option argument for GenLibs.
 *
 *  @param mod  - Module object
 *  @return     - Link option object
 */
function getLibs(mod) {
    const GenLibs = system.getScript("/ti/utils/build/GenLibs.syscfg.js");
    const DriverLib = system.getScript("/ti/devices/DriverLib");

    /* get device information from DriverLib */
    const deviceId = system.deviceData.deviceId;
    var libName = DriverLib.getAttrs(deviceId).libName;

    var link_info = {
        name: "/ti/drivers/rcl",
        deps: [
            "/ti/drivers"
        ],
        libs: [
            GenLibs.libPath('ti/drivers/rcl', `rcl_${libName}.a`),
            GenLibs.libPath(`ti/devices/${libName}/rf_patches`, `lrf_${libName}.a`)
        ]
    };

    return link_info;
}

function getOpts()
{

    let board = system.deviceData.board;
    let isFPGA = false;

    /* Users may be using SysConfig with no provided board */
    if (board) {
        isFPGA = board.name.match(/FPGA/);
    }

    if (isFPGA) {
        return ["-DSOCFPGA"];
    }
    else {
        return [];
    }
}

/*
 *******************************************************************************
 Module Validation
 *******************************************************************************
 */

/*!
 *  ======== validate ========
 *  Validate rfdriver module configuration.
 *
 *  @param inst         - Module instance containing the config that changed
 *  @param validation   - Object to hold detected validation issues
 */
function validate(inst, validation) {

}

/*
 *******************************************************************************
 Module Export
 *******************************************************************************
 */

 /*
 *  ======== rcldriverModule ========
 *  Define the RCL Driver module properties and methods.
 */
let rcldriverModule = {
    moduleStatic: {
        config,
        pinmuxRequirements,
        moduleInstances,
        sharedModuleInstances,
        modules,
        validate,
        filterHardware,
        onHardwareChanged
    },

    /* override device-specific templates */
    templates: {
        "/ti/utils/build/GenLibs.cmd.xdt":
            { modName: "/ti/drivers/RCL", getLibs },
        "/ti/utils/build/GenOpts.opt.xdt":
            { modName: "/ti/drivers/RCL", getOpts : getOpts },
        boardc: "/ti/drivers/rcl/templates/RCL.Board.c.xdt",
        board_initc: "/ti/drivers/rcl/templates/RCL.Board_init.c.xdt",
        boardh: "/ti/drivers/rcl/templates/RCL.Board.h.xdt"
    }
};


/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic RCL module to
 *  allow us to augment/override as needed for the CC23XX
 */
function extend(base) {
    /* merge and overwrite base module attributes */
    return({...base, ...rcldriverModule});
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by generic RCL module */
    extend: extend
};
