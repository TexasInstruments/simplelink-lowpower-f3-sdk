/*
 * Copyright (c) 2018 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble_central.syscfg.js ========
 */

"use strict";

// Get central long descriptions
const Docs = system.getScript("/ti/ble/central/ble_central_docs.js");

// Get common utility functions
const Common = system.getScript("/ti/ble/ble_common.js");

const config = {
    name: "centralConfig",
    displayName: "Central Configuration",
    description: "Configure Central Role Settings",
    config: [
        {
            name: "hideCentralGroup",
            default: true,
            hidden: true
        },
        {
            name: "initiatingPHY",
            displayName: "Initiating PHY",
            default: "INIT_PHY_1M",
            getDisabledOptions: initPhyDisableOptions,
            hidden: true,
            longDescription: Docs.initiatingPHYLongDescription,
            options: [
                { displayName: "1M",    name: "INIT_PHY_1M"     },
                { displayName: "2M",    name: "INIT_PHY_2M"     },
                { displayName: "Coded", name: "INIT_PHY_CODED"  }
            ]
        },
        {
            name: "centralScanInt",
            displayName: "Scan Interval (ms)",
            default: 10,
            hidden: true,
            longDescription: Docs.centralScanIntLongDescription
        },
        {
            name: "centralScanWin",
            displayName: "Scan Window (ms)",
            default: 10,
            hidden: true,
            longDescription: Docs.centralScanWinLongDescription
        },
        {
            name: "connIntMin",
            displayName: "Connection Interval Min (ms)",
            default: 100,
            hidden: true,
            longDescription: Docs.connIntMinLongDescription
        },
        {
            name: "connIntMax",
            displayName: "Connection Interval Max (ms)",
            default: 100,
            hidden: true,
            longDescription: Docs.connIntMaxLongDescription
        },
        {
            name: "centraConnLatency",
            displayName: "Connection Latency",
            default: 0,
            hidden: true,
            longDescription: Docs.centraConnLatencyLongDescription
        },
        {
            name: "centraSupTimeout",
            displayName: "Supervision Timeout (10ms)",
            default: 20000,
            hidden: true,
            longDescription: Docs.centraSupTimeoutLongDescription
        }
    ]
};


/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - Central instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    // Validate initiatingPhy
    // If Extended Advertising is disabled 2M and Coded PHYs are not supported
    if(inst.extAdv == false)
    {
        if(inst.initiatingPHY == "INIT_PHY_2M" || inst.initiatingPHY == "INIT_PHY_CODED")
        {
            validation.logError("2M PHY and Coded PHY are not supported when the Extended Advertising feature is disabled", inst, "initiatingPHY");
        }
    }

    // Validate connection interval
    Common.validateConnInterval(inst,validation,inst.connIntMin,"connIntMin",inst.connIntMax,"connIntMax");

    if(inst.centralScanInt < 2.5 || inst.centralScanInt > 10240)
    {
        validation.logError("Scan Inerval range is 2.5 to 10,240 ms", inst, "centralScanInt");
    }

    if(inst.centralScanWin < 2.5 || inst.centralScanWin > 10240)
    {
        validation.logError("Scan Window range is 2.5 to 10,240 ms", inst, "centralScanWin");
    }

    if(inst.centraConnLatency < 0 || inst.centraConnLatency > 499)
    {
        validation.logError("Connection Latency range is 0 to 499", inst, "centraConnLatency");
    }

    if(inst.centraSupTimeout < 100 || inst.centraSupTimeout > 32000)
    {
        validation.logError("Supervision Timeout range is 100 to 32,000 ms", inst, "centraSupTimeout");
    }

    if(inst.centralScanWin > inst.centralScanInt)
    {
        validation.logError("Scan Window must be less than or equal to Scan Interval", inst, "centralScanWin");
        validation.logError("Scan Interval must be greater than or equal to Scan Window", inst, "centralScanInt");
    }
}


/*
 *  ======== initPhyDisableOptions ========
 *  Generates a list of options that should be disabled in a
 *  drop-down
 *
 * @returns Array - array of strings that should be disabled
*/
function initPhyDisableOptions(inst)
{
    const extAdv = system.modules["/ti/ble/ble"].$static.extAdv;

    let disabledOptions = [];

    // Disable the option to choose Coded PHY when Extended Advertising is disabled
    if(extAdv == false)
    {
        disabledOptions = disabledOptions.concat({name: "INIT_PHY_2M", reason: "Not supported when the Extended Advertising feature is disabled"});
        disabledOptions = disabledOptions.concat({name: "INIT_PHY_CODED", reason: "Not supported when the Extended Advertising feature is disabled"});
    }

    return disabledOptions;
}

/*
 *  ======== moduleInstances ========
 *  Determines what modules are added as non-static submodules
 *
 *  @param inst  - Module instance containing the config that changed
 *  @returns     - Array containing dependency modules
 */
function moduleInstances(inst)
{
    const dependencyModule = [];

    if(!inst.hideCentralGroup)
    {
        dependencyModule.push({
            name: "connUpdateParamsCentral",
            group: "centralConfig",
            displayName: "Connection Update Request Params",
            moduleName: "/ti/ble/general/ble_conn_update_params",
            collapsed: true,
            args: {
                hideParamUpdateDelay: true
            }
        });
    }

    return(dependencyModule);
}

// Exports to the top level BLE module
exports = {
    config: config,
    validate: validate,
    moduleInstances: moduleInstances
};
