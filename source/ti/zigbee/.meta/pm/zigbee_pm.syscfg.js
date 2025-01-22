/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 *  ======== zigbee_pm.syscfg.js ========
 */

"use strict";

const Common = system.getScript("/ti/zigbee/zigbee_common.js");

/* Description text for configurables */

const powerModeLongDescription = `Specify whether the radio should always be \
on or be allowed to sleep (selected run-time configuration).

Note that for ZigBee Coordinators and ZigBee Routers, the power mode of \
operation is fixed to *Always On*. This is required for coordinators and \
routers. ZigBee End Devices can be freely configured between *Sleepy* and \
*Always On*.

For more information, refer to the [ZigBee Configuration](/zigbee/html/\
sysconfig/zigbee.html#zigbee-configuration) section of the ZigBee User's \
Guide.

**Default:** Sleepy (Low Power Mode enabled)`;

const pollPeriodDescription = `The period (in milliseconds) between poll \
messages.`;

const pollPeriodLongDescription = `The period (in milliseconds) between poll \
messages.

**Default:** 3000 ms

**Range:** Minimal Poll Period - ${Common.POLL_PERIOD_MAX} ms`;

/* Power management submodule for zstack module */
const config = {
    displayName: "Power Management",
    description: "Configure radio power settings",
    config: [
        {
            name: "powerModeOperation",
            displayName: "Power Mode of Operation of ZED",
            description: "Specify whether the radio should always be on "
                         + "or be allowed to sleep",
            longDescription: powerModeLongDescription,
            default: "sleepy",
            readOnly: true,
            options: [
                {
                    name: "alwaysOn",
                    displayName: "Always On (Low Power Mode disabled)"
                },
                {
                    name: "sleepy",
                    displayName: "Sleepy (Low Power Mode enabled)"
                }
            ],
            onChange: onPowerModeChange
        },
        {
            name: "pollPeriod",
            displayName: "Poll Period (ms)",
            description: pollPeriodDescription,
            longDescription: pollPeriodLongDescription,
            default: 3000,
            hidden: false
        },
    ],
};

/* Function to handle changes in deviceType configurable */
function onDeviceTypeChange(inst, ui)
{
    if(!inst.deviceType.includes("gpd"))
    {
        let element = null;
        for(element of config.config)
        {
            ui[element.name].hidden = false;
        }

        if(!inst.deviceType.includes("zed"))
        {
            inst.powerModeOperation = "alwaysOn";
            ui.powerModeOperation.readOnly = true;
            ui.powerModeOperation.hidden = true;
            ui.pollPeriod.hidden = true;
        }
        else
        {
            inst.powerModeOperation = "sleepy";
            ui.powerModeOperation.readOnly = false;
            ui.powerModeOperation.hidden = false;
            ui.pollPeriod.hidden = false;
        }
    }
    else
    {
        let element = null;
        for(element of config.config)
        {
            ui[element.name].hidden = true;
        }
    }
}


/* Function to handle changes in powerModeOperation configurable */
function onPowerModeChange(inst, ui)
{
    if(inst.powerModeOperation === "alwaysOn")
    {
        ui.pollPeriod.hidden = true;
    }
    else /* sleepy */
    {
        ui.pollPeriod.hidden = false;
    }
}

/* Validation function for the power management submodule */
function validate(inst, validation)
{
    /* Verify poll period max bound */
    if(inst.pollPeriod > Common.POLL_PERIOD_MAX)
    {
        validation.logError(
            "Poll period must be less than or equal to "
            + Common.POLL_PERIOD_MAX + " milliseconds (32 bits)",
            inst, "pollPeriod"
        );
    }
}

exports = {
    config: config,
    validate: validate,
    onDeviceTypeChange: onDeviceTypeChange
};