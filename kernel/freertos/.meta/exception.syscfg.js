/*
 * Copyright (c) 2024 Texas Instruments Incorporated - http://www.ti.com
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
 */

/*
 *  ======== exception.syscfg.js ========
 */

"use strict";

/* get device specific Settings */
let Settings = system.getScript("/freertos/Settings.syscfg.js");
let GenLibs = system.getScript("/ti/utils/build/GenLibs.syscfg.js");

function getCFiles(kernel)
{
    let baseFiles = [];

    var isa = GenLibs.getDeviceIsa();

    if (isa === "m0p") {
        baseFiles.push("exception/ExceptionArmV6M.c");
    }
    else if (isa === "m4" || isa === "m4f" || isa === "m3" || isa === "m3f") {
        baseFiles.push("exception/ExceptionArmV7M.c");

    }
    else if (isa === "m33f" || isa === "m33") {
        baseFiles.push("exception/ExceptionArmV8M.c");
    }

    return baseFiles;
}

/*
 *  ======== moduleInstancesStatic ========
 *  returns static instances
 */
function moduleInstancesStatic(inst) {
    let staticInstances = new Array();


    /* Check every configurable whether any configurables are set to
     * "Exception_handlerMax" Disable eslint for _ variable since SysConfig
     * already imports lodash. Eslint requires single line C-style comments to
     * parse them.
     */
    /* jslint nomen:true */
    /* globals _ */
    if (_.some(_.keys(inst.$module.$configByName), (key)=>inst[key] === "Exception_handlerMax")) {
        staticInstances.push(
            {
                name: "LogModule",
                displayName: "Exception Log Configuration",
                moduleName: "/ti/log/LogModule",
                collapsed: true,
                requiredArgs: {
                    $name: "LogModule_Exception"
                },
                args: {
                    enable_DEBUG: false,
                    enable_INFO: false,
                    enable_VERBOSE: false,
                    enable_WARNING: false,
                    // Only enable ERROR by default. The module does not use
                    // other levels
                    enable_ERROR: true
                }
            }
        );
    }
    return (staticInstances);
}

/*
 *  ======== base ========
 *  Module definition object
 */
let base = {
    staticOnly: true,
    displayName: "Exceptions",
    description: "Exception configuration",
    longDescription: `
This modules handles the configuration and generation of the initial vector table. The handlers used by default are
either set to spin or pre-determined by FreeRTOS.

All handlers except the reset ISR handler may be changed. It is not recommended to change FreeRTOS handlers unless
the application is prepared to accomodate FreeRTOS's use of them.

The application may optionally implement a hook function that will be called by Exception_handlerMax and
Exception_handlerMin with interrupts disabled. The hook has the following function signature:
void Exception_hookFxn(Exception_ExceptionContext *exceptionContext);

Configurable handlers are:
- Exception_handlerSpin: Spins with interrupts disabled.
- Exception_handlerMin: Disables interrupts, saves the CPU register state when the fault occurs, calls the hook
function, and spins.
- Exception_handlerMax: Disables interrupts, saves the CPU register state when the fault occurs, decodes and logs the
extended fault and exception state, calls the hook function, and spins.
`,
    moduleStatic: {
        name: "moduleGlobal",
        moduleInstances: moduleInstancesStatic,
        config: [
            {
                name: "nmiHandler",
                displayName: "NMI Handler",
                longDescription: `
Configure which handler is used when an NMI is generated.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: "Custom" }
                ],
                default: "Exception_handlerSpin",
                onChange:(inst, ui) => {
                    ui.nmiHandlerCustomFxn.hidden = inst.nmiHandler !== "Custom";
                }
            },
            {
                name: "nmiHandlerCustomFxn",
                displayName: "Custom NMI Handler Name",
                longDescription: `
Specify which custom fault handler should be called when an NMI triggers.`,
                default: "",
                hidden: true
            },
            {
                name: "hardFaultHandler",
                displayName: "Hard Fault Handler",
                longDescription: `
Configure which handler is used when a hard fault is generated.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: "Custom" }
                ],
                default: "Exception_handlerSpin",
                onChange:(inst, ui) => {
                    ui.hardFaultHandlerCustomFxn.hidden = inst.hardFaultHandler !== "Custom";
                }
            },
            {
                name: "hardFaultHandlerCustomFxn",
                displayName: "Custom Hard Fault Handler Name",
                longDescription: `
Specify which custom fault handler should be called when a hard fault triggers.`,
                default: "",
                hidden: true
            },
            {
                name: "mpuFaultHandler",
                displayName: "MPU Fault Handler",
                longDescription: `
Configure which handler is used when an MPU fault is generated.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: "Custom" }
                ],
                default: "Exception_handlerSpin",
                hidden: GenLibs.getDeviceIsa() === "m0p",
                onChange:(inst, ui) => {
                    ui.mpuFaultHandlerCustomFxn.hidden = inst.mpuFaultHandler !== "Custom";
                }
            },
            {
                name: "mpuFaultHandlerCustomFxn",
                displayName: "Custom MPU Handler Name",
                longDescription: `
Specify which custom fault handler should be called when an MPU fault triggers.`,
                default: "",
                hidden: true
            },
            {
                name: "busFaultHandler",
                displayName: "BusFault Handler",
                longDescription: `
Configure which handler is used when a BusFault is generated.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: "Custom" }
                ],
                default: "Exception_handlerSpin",
                hidden: GenLibs.getDeviceIsa() === "m0p",
                onChange:(inst, ui) => {
                    ui.busFaultHandlerCustomFxn.hidden = inst.mpuFaultHandlerCustomFxn !== "Custom";
                }
            },
            {
                name: "busFaultHandlerCustomFxn",
                displayName: "Custom BusFault Handler Name",
                longDescription: `
Specify which custom fault handler should be called when a BusFault triggers.`,
                default: "",
                hidden: true
            },
            {
                name: "usageFaultHandler",
                displayName: "UsageFault Handler",
                longDescription: `
Configure which handler is used when a UsageFault is generated.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: "Custom" }
                ],
                default: "Exception_handlerSpin",
                hidden: GenLibs.getDeviceIsa() === "m0p",
                onChange:(inst, ui) => {
                    ui.usageFaultHandlerCustomFxn.hidden = inst.busFaultHandlerCustomFxn !== "Custom";
                }
            },
            {
                name: "usageFaultHandlerCustomFxn",
                displayName: "Custom Usage Fault Handler Name",
                longDescription: `
Specify which custom fault handler should be called when a usage fault triggers.`,
                default: "",
                hidden: true
            },
            {
                name: "secureFaultHandler",
                displayName: "Secure Fault Handler",
                longDescription: `
Configure which handler is used when a secure fault is generated.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: "Custom" }
                ],
                default: "Exception_handlerSpin",
                hidden: !(GenLibs.getDeviceIsa().match("m33")),
                onChange:(inst, ui) => {
                    ui.secureFaultHandlerCustomFxn.hidden = inst.secureFaultHandler !== "Custom";
                }
            },
            {
                name: "secureFaultHandlerCustomFxn",
                displayName: "Custom Secure Fault Handler Name",
                longDescription: `
Specify which custom fault handler should be called when a secure fault triggers.`,
                default: "",
                hidden: true
            },
            {
                name: "svCallHandler",
                displayName: "SV Call Handler",
                longDescription: `
Configure which handler is used when a supervisor call is generated. It is not recommended to change this setting unless
the application can accomodate FreeRTOS's use of it as well.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: Settings.defaultSvCallHandler },
                    { name: "Custom" }
                ],
                default: Settings.defaultSvCallHandler,
                onChange:(inst, ui) => {
                    ui.svCallHandlerCustomFxn.hidden = inst.svCallHandler !== "Custom";
                }
            },
            {
                name: "svCallHandlerCustomFxn",
                displayName: "Custom SV Call Handler Name",
                longDescription: `
Specify which custom fault handler should be called when an SV call triggers.`,
                default: "",
                hidden: true
            },
            {
                name: "debugMonitorHandler",
                displayName: "Debug Monitor Handler",
                longDescription: `
Configure which handler is used when a debug monitor exception is generated.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: "Custom" }

                ],
                default: "Exception_handlerSpin",
                hidden: GenLibs.getDeviceIsa() === "m0p",
                onChange:(inst, ui) => {
                    ui.debugMonitorHandlerCustomFxn.hidden = inst.debugMonitorHandler !== "Custom";
                }
            },
            {
                name: "debugMonitorHandlerCustomFxn",
                displayName: "Custom Debug Monitor Handler Name",
                longDescription: `
Specify which custom fault handler should be called when a debug monitor exception triggers.`,
                default: "",
                hidden: true
            },
            {
                name: "pendSvHandler",
                displayName: "PendSV Handler",
                longDescription: `
Configure which handler is used when a PendSV interrupt is generated. It is not recommended to change this setting
unless the application can accomodate FreeRTOS's use of it as well.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: Settings.defaultPendSvHandler },
                    { name: "Custom" }
                ],
                default: Settings.defaultPendSvHandler,
                onChange:(inst, ui) => {
                    ui.pendSvFaultHandlerCustomFxn.hidden = inst.pendSvHandler !== "Custom";
                }
            },
            {
                name: "pendSvFaultHandlerCustomFxn",
                displayName: "Custom PendSV Handler Name",
                longDescription: `
Specify which custom fault handler should be called when a PendSV exception triggers.`,
                default: "",
                hidden: true
            },
            {
                name: "sysTickHandler",
                displayName: "SysTick Handler",
                longDescription: `
Configure which handler is used when a SysTick interrupt is generated. It is not recommended to change this setting
unless the application can accomodate FreeRTOS's use of it as well.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: Settings.defaultSysTickHandler },
                    { name: "Custom" }
                ],
                default: Settings.defaultSysTickHandler,
                onChange:(inst, ui) => {
                    ui.sysTickHandlerCustomFxn.hidden = inst.sysTickHandler !== "Custom";
                }
            },
            {
                name: "sysTickHandlerCustomFxn",
                displayName: "Custom SysTick Handler Name",
                longDescription: `
Specify which custom fault handler should be called when a SysTick interrupt triggers.`,
                default: "",
                hidden: true
            },
            {
                name: "defaultHandler",
                displayName: "Default Interrupt Handler",
                longDescription: `
Configure which default handler is used when a non-Cortex-M interrupt is generated.`,
                options: [
                    { name: "Exception_handlerSpin" },
                    { name: "Exception_handlerMin" },
                    { name: "Exception_handlerMax" },
                    { name: "Custom" }
                ],
                default: "Exception_handlerSpin",
                onChange:(inst, ui) => {
                    ui.defaultHandlerCustomFxn.hidden = inst.defaultHandler !== "Custom";
                }
            },
            {
                name: "defaultHandlerCustomFxn",
                displayName: "Custom Default Handler Name",
                longDescription: `
Specify which custom fault handler should be called when a non-Cortex-M interrupt triggers.`,
                default: "",
                hidden: true
            }
        ]
    },
    getCFiles:getCFiles
};

/* export the module */
exports = base;
