/*
 * Copyright (c) 2025 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ble_adaptivity_docs.js ========
 */

const AdaptivityModeLongDescription = `Select the level of control for transmit (TX) usage on BLE channels. When channel utilization exceeds a defined threshold, then the
Adaptivity will be enabled.\n
__Default__: Adaptive Mode\n`

const externalTxPowerGainLongDescription = `The total transmit (TX) power (Pout) gain in dBm.\
 This should include gain from any external PA as well as any antenna gain. This will relax the detection threshold level (TL) according to the formula .\
 TL = -70 dBm/MHz + 10 × log10 (100 mW / Pout).\n
__Default__: 0\n`

const externalRxPowerGainLongDescription = `The total receive (RX) power gain in dB (Gdb).\
 This should include gain from any external LNA as well as any antenna gain. This will compensate the detection threshold level (TL) according to\
 TL = -70dBm/MHz + Gdb\n
__Default__: 0\n`

const rxWindowDurationLongDescription = `Duration of the time the RX window opens to validate the channel\
 status (noisy or clear). this value must be minimum 120µs.\n
__Default__: 12\n`

const fixedObservTimeLongDescription = `When unmarked, the Observation Period is calculated by dwell time multiplied by 100,\
 Dwell time - The time between frequency changes for FHSS equipment.\
 EN 300 328 notes that this time may comprise transmit, receive, and idle phases of the equipment\n
__Default__: False\n`

const observationTimeLongDescription = `Fixed observation period. This value will not change according to app behavior\n
__Note__: in units of 100 ms\n
__Default__: False\n`

const blockingChannelTimeLongDescription = `When the channel is determined to be noisy, only empty packets will sent \
 on the channel for the selected time period.\n
__Default__: False\n`

exports = {
    AdaptivityModeLongDescription:      AdaptivityModeLongDescription,
    externalTxPowerGainLongDescription: externalTxPowerGainLongDescription,
    externalRxPowerGainLongDescription: externalRxPowerGainLongDescription,
    rxWindowDurationLongDescription:    rxWindowDurationLongDescription,
    fixedObservTimeLongDescription:     fixedObservTimeLongDescription,
    observationTimeLongDescription:     observationTimeLongDescription,
    blockingChannelTimeLongDescription: blockingChannelTimeLongDescription,
};
