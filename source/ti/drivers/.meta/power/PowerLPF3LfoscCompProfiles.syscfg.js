/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated - http://www.ti.com
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
 * ======== PowerLPF3LfoscCompProfiles.syscfg.js ========
 */

/* get Common /ti/drivers utility functions */
let Common     = system.getScript("/ti/drivers/Common.js");

var instConfigs = [
    {
        name: "systemWakeupInterval",
        displayName: "System Wakeup Interval",
        description: "The expected interval in us between system wakeups.",
        default: 1000000
    },
    {
        name: "ppmRequirement",
        displayName: "System PPM Requirement",
        default: "PowerLPF3_LFOSC_PPM_500",
        options: [
            {
                displayName: "500 PPM",
                name: "PowerLPF3_LFOSC_PPM_500"
            }
        ]
    },
    {
        name: "maxAllowedJitter",
        displayName: "Maximum Allowed Jitter",
        description: "The maximum allowed jitter per system wakeup in microseconds.",
        longDescription:
`The jitter is added to the system's PPM requirement, relaxing the number of
calibrations needed per system wakeup.`,
        isInteger: true,
        range: [16, 16],    /* For now, allow only a Jitter of 16 us */
        default: 16
    },
    {
        name: "temperatureGradient",
        displayName: "Temperature Gradient",
        description: "The max absolute temperature gradient in Celsius/s where the specified PPM requirement should still be satisfied.",
        longDescription: `The LFOSC is sensitive to changes in temperature, and the faster the temperature changes
the more often the system has to wake up and perform LFOSC calibrations to ensure
that the specified PPM requirements are still met.`,
        isInteger: false,
        range: [0, 65.5],
        default: 1.0
    }
];

/*
 *  ======== getInstConfigsWithName ========
 *  Calls Common.addNameConfig() and explicitly sets the "$name" config as not
 *  hidden
 */
function getInstConfigsWithName(config)
{
    /* Add a "$name" config to the front of the config list */
    var tmpInstConfigs = Common.addNameConfig(config, "/ti/drivers/power/PowerLPF3LfoscCompProfiles", "CONFIG_LFOSC_COMP_PROFILE_");

    /* Modify the "$name" config to explicitly not be hidden */
    tmpInstConfigs[0]["hidden"] = false;

    /* Return modified config */
    return tmpInstConfigs;
}

exports = {
    displayName: "LFOSC_COMP_PROFILES",
    defaultInstanceName: "configLfoscCompProfile",
    config: getInstConfigsWithName(instConfigs),
    uiAdd: "staticAndInstance",
    templates           : {
        boardc      : "/ti/drivers/power/PowerLPF3LfoscCompProfiles.Board.c.xdt",
        boardh      : "/ti/drivers/power/PowerLPF3LfoscCompProfiles.Board.h.xdt"
    }
};
