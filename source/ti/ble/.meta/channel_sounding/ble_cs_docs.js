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
 *  ======== ble_cs_docs.js ========
 */

"use strict";

// Long description for the antennasMuxValues configuration parameter
const antennasMuxValuesLongDescription =
`The antennas Muxing Values configuration allows specifying the muxing bitmap for the antennas.\n
Each 2 bits represent the muxing value to be set in GPIO pin in order to select each antenna, for example:
| 4th Antenna | 3rd Antenna | 2nd Antenna | 1st Antenna |
|-------------|-------------|-------------|-------------|
|     11      |     10      |     01      |     00      |\n

The GPIO output to be set in order to select the first antenna will be 00.\n
The GPIO output to be set in order to select the second antenna will be 01.
And so on.\n
\n
Therefore the final value for this bitmap will be: 0xE4\n
__Note__: The first antenna will be used for common BLE communications.\n
__Note__: In order to enable the right gpios for muxing, go to RCL module under
TI Drivers -> RCL Observables -> signals -> enable PBEGPO2 and PBEGPO3.
RCL will set default gpios pins. To change the default pins - use the PinMux section inside the module.
PBEGPO2 will be mapped to the lsb and PBEGPO3 to the msb.\n`

 // Exports the long descriptions for each configurable
 exports = {
    antennasMuxValuesLongDescription: antennasMuxValuesLongDescription
};