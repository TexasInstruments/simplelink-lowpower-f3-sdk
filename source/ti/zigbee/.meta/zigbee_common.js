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
 *  ======== zstack_common.js ========
 */

"use strict";

/* Min poll period (ms) */
const POLL_PERIOD_MIN = 100;

/* Max poll period (ms) */
const POLL_PERIOD_MAX = 0xFFFFFFFF;

/* Network Key length (bytes) */
const NWK_KEY_LEN = 16;

/* Function that converts the channel array from the RF channel configurables
 * to a bitmask in hex string format */
function chanArrToBitmask(chanArr)
{
    let i;
    let bitmask = 0;
    for(i = 0; i < chanArr.length; i++)
    {
        const channel = chanArr[i];

        // Shift the channel bit depending on what channel is selected
        const bit = 0x1 << (channel);

        // Add channel bit to the bitmask
        bitmask |= bit;
    }
    return("0x" + bitmask.toString(16).padStart(8, "0"));
}

/* Helper function for range validation (making sure a configurable is
* between a minimum and maximum value */
function validateRange(inst, validation, configVal, configName, configDisplay,
    min, max)
{
    if(min != null && configVal < min)
    {
        validation.logError(
            configDisplay + " must be greater than or equal to " + min,
            inst, configName
        );
    }
    else if(max != null && configVal > max)
    {
        validation.logError(
            configDisplay + " must be less than or equal to " + max,
            inst, configName
        );
    }
}

exports = {
    chanArrToBitmask: chanArrToBitmask,
    validateRange: validateRange,
    POLL_PERIOD_MIN: POLL_PERIOD_MIN,
    POLL_PERIOD_MAX: POLL_PERIOD_MAX,
    NWK_KEY_LEN: NWK_KEY_LEN,

}