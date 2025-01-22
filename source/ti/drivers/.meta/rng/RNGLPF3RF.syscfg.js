/*
 * Copyright (c) 2021-2024, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== RNGLPF3RF.syscfg.js ========
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");

let logError = Common.logError;
let logInfo  = Common.logInfo;
let logWarning  = Common.logWarning;

/* Interrupt Priority for internal AESCTR instance */
let intPriority = Common.newIntPri()[0];
intPriority.name = "interruptPriority";
intPriority.displayName = "Interrupt Priority";
intPriority.description = "Crypto peripheral interrupt priority";

/*
* CBC-MAC driver is used to condition the entropy buffer provided by RCL.
* The RCL entropy buffer has approximately 0.1 bits of entropy per bit.
* Based on NIST 800-90B, 1280-bits of RCL TRNG entropy buffer with 128-bits
* of entropy will be required as input by the conditioning component to
* generate 128-bits of seed with full entropy. Since AESCTRDRBG requires a
* 256-bit seed, RNG requires a 2560-bit buffer from RCL, which is 80 words.
* However, 152 words is chosen as it provides a high margin (~ 75%).
*/
const minRawNoiseLenWords = 152;

/* Maximum number of words which can be read from RCL's entropy buffer */
const maxRawNoiseLenWords = 1024;

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base RNG configuration
 */
let devSpecific = {
    moduleStatic: {
        name: "rngSettings",
        displayName: "Settings for RNG",
        config: [
            {
                name        : "rngPoolSize",
                displayName : "RNG Entropy Pool Size",
                description : 'Size of entropy pool kept in memory by the RNG '
                            + 'driver for faster responses to entropy '
                            + 'requests.',
                default     : 32
            },
            {
                name        : "rawNoiseLenWords",
                displayName : "RNG Raw Noise Input Size In Words",
                description : 'Number of words read from the RCL\'s ADC '
                            + 'and conditioned using CBC-MAC before use as the RNG seed',
                default     : minRawNoiseLenWords
            },
            {
                name        : "noiseConditioningKeyW0",
                displayName : "Noise Conditioning Key Word 0",
                description : 'Word 0 of the 128-bit key used by CBC MAC driver to '
                            + 'compute the seed value for RNG driver'
                            + 'from RCL entropy buffer',
                displayFormat: {radix: "hex", bitSize: 32},
                default: 0x111de874

            },
            {
                name        : "noiseConditioningKeyW1",
                displayName : "Noise Conditioning Key Word 1",
                description : 'Word 1 of the 128-bit key used by CBC MAC driver to '
                            + 'compute the seed value for RNG driver'
                            + 'from RCL entropy buffer',
                displayFormat: {radix: "hex", bitSize: 32},
                default: 0x6cecb00e
            },
            {
                name        : "noiseConditioningKeyW2",
                displayName : "Noise Conditioning Key Word 2",
                description : 'Word 2 of the 128-bit key used by CBC MAC driver to '
                            + 'compute the seed value for RNG driver'
                            + 'from RCL entropy buffer',
                displayFormat: {radix: "hex", bitSize: 32},
                default: 0x7fb76dc5
            },
            {
                name        : "noiseConditioningKeyW3",
                displayName : "Noise Conditioning Key Word 3",
                description : 'Word 3 of the  128-bit key used by CBC MAC driver to '
                            + 'compute the seed value for RNG driver'
                            + 'from RCL entropy buffer',
                displayFormat: {radix: "hex", bitSize: 32},
                default: 0x8e020ca2
            },
            {
                name        : "rngReturnBehavior",
                displayName : "Return Behavior",
                description : 'The return behavior (RNG_RETURN_BEHAVIOR) '
                            + 'which will be used for all instances of the '
                            + 'driver. All instances must be opened using '
                            + 'this setting.',
                default     : "RNG_RETURN_BEHAVIOR_POLLING",
                options     : [ {
                                    name: "RNG_RETURN_BEHAVIOR_POLLING",
                                    displayName: "Polling"
                                },
                                {
                                    name: "RNG_RETURN_BEHAVIOR_BLOCKING",
                                    displayName: "Blocking"
                                }
                              ]
            },
            {
                name: "HealthChecks",
                displayName: "RNG Health Check Tests",
                collapsed: false,
                config: [
                    {
                        name: "RepetitionCountTest",
                        displayName: "Repetition Count Test",
                        description: "Conduct Repetition Count Test on noise data.",
                        default: true,
                        onChange: (inst, ui) => ui.RCTThreshold.hidden = !inst.RepetitionCountTest
                    },
                    {
                        name        : "RCTThreshold",
                        displayName : "Repetition Count Test Threshold",
                        longDescription : 'Set a threshold value for the maximum number of times the same code '
                                    + 'is repeated. This test compares 3 codes at a time which has two impacts. '
                                    + 'First, the threshold value must be a multiple of 3.'
                                    + 'Second, up to 2 repeated codes may go undetected.'
                                    + 'For example, if the threshold is 24, the test may not indicate a failure until 26 repeated codes have occurred.'
                                    + 'See NIST SP 800-90B section 4.4.1 for information about adjusting this value.',
                        default     : 24,
                        hidden      : false
                    },
                    {
                        name: "AdaptiveProportionTest",
                        displayName: "Adaptive Proportion Test",
                        description: "Conduct Adaptive Proportion Test on noise data.",
                        default: true,
                        onChange: (inst, ui) => ui.APTThreshold.hidden = !inst.AdaptiveProportionTest
                    },
                    {
                        name        : "APTThreshold",
                        displayName : "Adaptive Proportion Test Threshold",
                        longDescription : 'Set a threshold value for the expected entropy per value.'
                                    + 'See NIST SP 800-90B section 4.4.2 for information about adjusting this value.',
                        default     : 461,
                        hidden      : false
                    },
                    {
                        name        : "APTBimodalThreshold",
                        displayName : "APT Bimodal Threshold",
                        longDescription : 'Set a threshold value for the expected entropy between two code values.'
                                    + 'This is an additional test which is not specified by NSIT SP 800-90B.'
                                    + 'TI recommends setting this value to 80% of the Adaptive Proportion Test Threshold above.',
                        default     : 369,
                        hidden      : true
                    }
                ]
            },
            
            {
                name: "HW_ATTR_GROUP",
                displayName: "AESCTRDRBG HW Attributes",
                collapsed: false,
                config: [
                    intPriority
                ]
            }
        ]
    },

    config: [],

    templates : {
        boardc      : "/ti/drivers/rng/RNGLPF3RF.Board.c.xdt",
        boardh      : "/ti/drivers/rng/RNG.Board.h.xdt"
    }
};


function validate_rng_settings(inst, validation) {
    let RNG = system.modules["/ti/drivers/RNG"];

    if (RNG.$static.rngPoolSize < 16) {
        logError(validation, RNG.$static, "rngPoolSize",
                 "Value must be 16 or greater");
    }
    else {
        if (RNG.$static.rngPoolSize % 16 != 0) {
            logError(validation, RNG.$static, "rngPoolSize",
                     "Value must be a multiple of 16");
        }

        if (RNG.$static.rngPoolSize < 32) {
            logInfo(validation, RNG.$static, "rngPoolSize",
                    "Consider using a larger value for better performance");
        }
    }

    /* Validate entropy buffer length */
    if (RNG.$static.rawNoiseLenWords < minRawNoiseLenWords) {
        logError(validation, RNG.$static, "rawNoiseLenWords",
            "Value must be a minimum of " + minRawNoiseLenWords +
            " words for full entropy of generated seed");
    }
    else if (RNG.$static.rawNoiseLenWords > maxRawNoiseLenWords)
    {
        logError(validation, RNG.$static, "rawNoiseLenWords",
                 "Maximum allowed word length is " + maxRawNoiseLenWords);
    }

    /* Validate Key with default value */
    if ((RNG.$static.noiseConditioningKeyW0 == 0x111de874) &&
        (RNG.$static.noiseConditioningKeyW1 == 0x6cecb00e) &&
        (RNG.$static.noiseConditioningKeyW2 == 0x7fb76dc5) &&
        (RNG.$static.noiseConditioningKeyW3 == 0x8e020ca2))
    {
        /* Print warning that default conditioning key is being used */
        logWarning(validation, RNG.$static, "noiseConditioningKeyW3",
                   'This the default noise conditioning key provided by TI, '+
                   'which is acceptable per NIST SP 800-90B. However, TI recommends ' +
                   'you change this default value to a custom random value for your product');
    }

    /* Validate that the health check threshold values are greater than 1 */
    if ((RNG.$static.RCTThreshold <= 1) || (RNG.$static.RCTThreshold >= RNG.$static.rawNoiseLenWords * 3) || (RNG.$static.RCTThreshold % 3 != 0)) {
        logError(validation, RNG.$static, "RCTThreshold",
                 "Value must be greater than 1, less than 3 times the Input Size, and a multiple of 3");
    }
    if ((RNG.$static.APTThreshold <= 400) || (RNG.$static.APTThreshold >= 517))  {
        logError(validation, RNG.$static, "APTThreshold",
                 "Value must be greater than 400 and less than 517");
    }
    if ((RNG.$static.APTBimodalThreshold <= 1) || (RNG.$static.APTBimodalThreshold >= 517)) {
        logError(validation, RNG.$static, "APTBimodalThreshold",
                 "Value must be greater than 1 and less than 517");
    }
}

function validate(inst, validation, $super) {
    validate_rng_settings(inst, validation);

    if ($super.validate) {
        $super.validate(inst, validation);
    }
}

/*
 *  ======== extend ========
 */
function extend(base)
{
    /* display which driver implementation can be used */
    base = Common.addImplementationConfig(base, "RNG", null,
        [{name: "RNGLPF3RF"}], null);

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
    /* required function, called by base RNG module */
    extend: extend
};
