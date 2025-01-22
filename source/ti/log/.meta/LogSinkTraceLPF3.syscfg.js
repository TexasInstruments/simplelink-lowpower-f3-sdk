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
 *  ======== LogSinkTraceLPF3.syscfg.js ========
 */

"use strict";

let Common   = system.getScript("/ti/drivers/Common.js");

/*
 *  ======== config_instance ========
 *  Define the config params of the module instance
 */
let config_instance = [
    {
        name: "rfTraceMode",
        displayName: "RF Tracer Mode",
        description: "Mode for RF Tracer",
        longDescription: `
        The RF tracer modes allow for selecting the source of the trace messages.
        The source can be the radio (LRF domain) and/or the system CPU (Applications and other modules).
        `,
        hidden: false,
        default: "LogSinkTraceLPF3_CONFIG_CHANNELS_CPU_RADIO",
        options:
        [
            {
                name: "LogSinkTraceLPF3_CONFIG_CHANNELS_CPU_ONLY",
                displayName: "CPU only"
            },
            {
                name: "LogSinkTraceLPF3_CONFIG_CHANNELS_RADIO_ONLY",
                displayName: "Radio only"
            },
            {
                name: "LogSinkTraceLPF3_CONFIG_CHANNELS_CPU_RADIO",
                displayName: "CPU + Radio"
            }
        ]
    },
    {
        name: "rfTraceTimestamp",
        displayName: "Timestamp Enabled",
        description: "Enables Timestamp setting",
        longDescription: `
        When timestamps are enabled, the log elements have timestamps associated with them. This option is enabled and hidden by default.
        `,
        hidden: true,
        default: "LogSinkTraceLPF3_CONFIG_TIMESTAMP_ENABLED",
        options:
        [
            {
                name: "LogSinkTraceLPF3_CONFIG_TIMESTAMP_ENABLED",
                displayName: "Enabled",
                description: "Timestamps are enabled"
            },
            {
                name: "LogSinkTraceLPF3_CONFIG_TIMESTAMP_DISABLED",
                displayName: "Disabled",
                description: "Timestamps are disabled"
            }
        ]
    },
    {
        name: "rfTracePrescaler",
        displayName: "RF Tracer Prescaler",
        description: "Set speed of RF Tracer",
        longDescription: `
        This variable is the divisor that the RF Core main clock is divided by. "Divide by 3", for example, divides the clock speed by 3.
        The clock speed of the RF Tracer may need to be reduced if experiencing signal quality issues on the tracer signal.
        `,
        default: "LogSinkTraceLPF3_CONFIG_PRESCALER_DIV1",
        options:
        [
            {
                name: "LogSinkTraceLPF3_CONFIG_PRESCALER_DIV1",
                displayName: "Divide by 1",
                description: "Divide clock by 1"
            },
            {
                name: "LogSinkTraceLPF3_CONFIG_PRESCALER_DIV2",
                displayName: "Divide by 2",
                description: "Divide clock by 2"
            },
            {
                name: "LogSinkTraceLPF3_CONFIG_PRESCALER_DIV3",
                displayName: "Divide by 3",
                description: "Divide clock by 3"
            },
            {
                name: "LogSinkTraceLPF3_CONFIG_PRESCALER_DIV4",
                displayName: "Divide by 4",
                description: "Divide clock by 4"
            }
        ]
    },
    {
        name: "printfDelegate",
        displayName: "Printf Delegate Function",
        default: "LogSinkTraceLPF3_printfSingleton",
        readOnly: true
    },
    {
        name: "bufDelegate",
        displayName: "Buf Delegate Function",
        default: "LogSinkTraceLPF3_bufSingleton",
        readOnly: true,
        hidden: true
    }
];

/*
 * ======== moduleInstances ========
 */
function moduleInstances(inst)
{
    let moduleInstances = [];

    moduleInstances.push({
        name: "tracePinInstance",
        displayName: "Tracer PIN Configuration While Pin Is Not In Use", /* GUI name */
        moduleName: "/ti/drivers/GPIO",
        readOnly: false,
        collapsed: true,
        requiredArgs: {
            $name: "CONFIG_TRACE",
            parentInterfaceName: "lrfGpio",
            parentSignalName: "tracePin",
            parentSignalDisplayName: "Tracer Pin"
        },
        args: {
            mode: "Output",
            initialOutputState: "Low",
            outputStrength: "High",
            pull: "None"
        }
    });

    return moduleInstances;
}

/*!
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of config.
 *
 *  Called when a configuration changes in the module.
 *
 *  @param inst - Module instance containing the config that changed
 *  @return     - Array of pin requirements
 */
function pinmuxRequirements(inst)
{

    let resources = [];
    let tracerArray = [];

    resources.push({
        name: "tracePin",
        displayName:    "Trace Pin", /* GUI name */
        interfaceNames: ["D0","D1","D2","D3","D4","D5","D6","D7","D8", "D9", "D10", "D11", "D12", "D13", "D14", "D15"]
    });

    let lrfGpio = {
        name          : "lrfGpio",
        readOnly      : true,
        displayName   : "LRF GPIO",
        interfaceName : "LRF",
        canShareWith  : "LRF",
        resources     : resources,
        signalTypes   : {
        }
    };
    tracerArray.push(lrfGpio);

    return tracerArray;
}

/*
 * ======== validate ========
 */
function validate(inst, validation)
{
}

/*
 *  ======== base ========
 *  Module definition object
 */
let base = {
    displayName: "LogSinkTraceLPF3",
    description: "LogSinkTraceLPF3 module",
    longDescription: `
The [__LogSinkTraceLPF3__][1] provides a log sink that transports log out through Tracer.

[1]: /drivers/doxygen/html/index.html#log "Log framework"
`,
    config              : config_instance,
    defaultInstanceName : "CONFIG_ti_log_LogSinkTraceLPF3_",
    moduleInstances     : moduleInstances,
    pinmuxRequirements  : pinmuxRequirements,
    moduleStatic        : {
        modules: Common.autoForceModules(["Board"])
    },
    validate: validate,
    maxInstances        : 1,
    /* Board_init() priority to guarantee that the LogSink is initialized after GPIO */
    initPriority        : 5,
    templates: {
        "/ti/log/templates/ti_log_config.c.xdt":
        "/ti/log/templates/LogSinkTraceLPF3.Config.c.xdt",
        "/ti/log/templates/ti_log_config.h.xdt":
        "/ti/log/templates/LogSinkTraceLPF3.Config.h.xdt",
        boardc : "/ti/log/templates/LogSinkTraceLPF3.Board.c.xdt",
        board_initc: "/ti/log/templates/LogSinkTraceLPF3.Board_init.c.xdt"
    }

};

/* export the module */
exports = base;
