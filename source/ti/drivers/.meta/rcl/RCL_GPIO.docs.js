/*
 * Copyright (c) 2024-2025, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== RCL_GPIO.docs.js ========
 */

/*
 *******************************************************************************
 Base Module
 *******************************************************************************
 */

const base = {

};

/*
 *******************************************************************************
 RCL Driver Module
 *******************************************************************************
 */

 const config = {
    useCase: {
        displayName: "Use Case",
        description: "Select use case for RCL GPIOs",
        longDescription:`This lets the user select predefined GPIO configurations. For example, checking "Radio Digital Debug Pins"
        activates the LNA enable and PA enable signals. This allows the user to see the when the radio is in TX mode and RX mode
        which can be useful when debugging the radio`,
        standard: {
            displayName: "PA/LNA Pins",
            description: "This activates the LNA enable and PA enable signals. The PA enable signal is asserted when the radio is in TX and the LNA enable signal is asserted when the radio is in RX. Note that the signals will be asserted a bit before the RF event and deasserted a bit after the event."
        },
        fpga: {
            displayName: "FPGA",
            description: "Routes RF baseband signals as GPIOs. Typically this is used by the FPGA."
        }
    },
    signals: {
        displayName: "Additional RF GPIO Signals",
        description: "Choose which signals to enable",
        longDescription:`Enable specific radio signals beyond the selected use case  `
    },
rclConfig: {
    displayName: "RCL Configuration",
    longDescription: `
Enable and disable different settings for the RCL module.`
},
loggingEnabled: {
    displayName: "Enable Logging",
    longDescription: 'This setting will enable logging for the RCL module.'
    },
rclPbeGpoMask: {
    displayName: "Antenna Selection Mask",
    description: `Bit mask indicating which bits in LRFDPBE_GPOCTRL register are written`,
    longDescription:  `A default antenna selection value can be written to the LRFDPBE_GPOCTRL register.
The lower 8 bits of the LRFDPBE_GPOCTRL register correspond to GPO0 through GPO7.
These bits determine the logic levels of the muxed GPIO pins, if any. The mask specifies which bits in the register are written, depending on the antenna coding scheme.

For example, if two antennas are controlled through GPO2 and GPO3, the mask can be 0xC. This means the lower two bits (GPO0 and GPO1) are not written, while the upper two bits (GPO2 and GPO3) are written.

The default value of this mask is 0x00, meaning no default antenna selection is applied.

This mask must be set to a non-default value in applications that use channel sounding or normal BLE5 PHYs.`,
    },
rclObservables: {
    displayName: "RCL Observables",
    longDescription: `
Enable radio signals from the radio (LRF) domain. This can be done by selecting 'Use Case' or by
selecting specific signals in the 'Signals' drop-down menu.`
},
rclPinOptions: {
    paEnable: {
        displayName: "RFEGPO0 (PA_EN)",
        description: "Select RFE output line 0"
    },
    lnaEnable: {
        displayName: "RFEGPO1 (LNA_EN)",
        description: "Select RFE output line 1"
    },
    rfeGpo2: {
        displayName: "RFEGPO2",
        description: "Select RFE output line 2"
    },
    rfeGpo3: {
        displayName: "RFEGPO3",
        description: "Select RFE output line 3"
    },
    rfeGpo4: {
        displayName: "RFEGPO4",
        description: "Select RFE output line 4"
    },
    rfeGpo5: {
        displayName: "RFEGPO5",
        description: "Select RFE output line 5"
    },
    rfeGpo6: {
        displayName: "RFEGPO6",
        description: "Select RFE output line 6"
    },
    rfeGpo7: {
        displayName: "RFEGPO7",
        description: "Select RFE output line 7"
    },
    mceGpo0: {
        displayName: "MCEGPO0",
        description: "Select MCE output line 0"
    },
    mceGpo1: {
        displayName: "MCEGPO1",
        description: "Select MCE output line 1"
    },
    mceGpo2: {
        displayName: "MCEGPO2",
        description: "Select MCE output line 2"
    },
    mceGpo3: {
        displayName: "MCEGPO3",
        description: "Select MCE output line 3"
    },
    mceGpo4: {
        displayName: "MCEGPO4",
        description: "Select MCE output line 4"
    },
    mceGpo5: {
        displayName: "MCEGPO5",
        description: "Select MCE output line 5"
    },
    mceGpo6: {
        displayName: "MCEGPO6",
        description: "Select MCE output line 6"
    },
    mceGpo7: {
        displayName: "MCEGPO7",
        description: "Select MCE output line 7"
    },
    pbeGpo0: {
        displayName: "PBEGPO0 (COEX_REQUEST)",
        description: "Select PBE output line 0"
    },
    pbeGpo1: {
        displayName: "PBEGPO1 (COEX_PRIORITY)",
        description: "Select PBE output line 1"
    },
    pbeGpo2: {
        displayName: "PBEGPO2",
        description: "Select PBE output line 2"
    },
    pbeGpo3: {
        displayName: "PBEGPO3",
        description: "Select PBE output line 3"
    },
    pbeGpo4: {
        displayName: "PBEGPO4",
        description: "Select PBE output line 4"
    },
    pbeGpo5: {
        displayName: "PBEGPO5",
        description: "Select PBE output line 5"
    },
    pbeGpo6: {
        displayName: "PBEGPO6",
        description: "Select PBE output line 6"
    },
    pbeGpo7: {
        displayName: "PBEGPO7",
        description: "Select PBE output line 7"
    },
    coexGrant: {
        displayName: "COEX_GRANT",
        description: "Select coexistence GRANT input"
    }
}
};

const coex = {
    enable: {
        displayName: "Enable RF Coexistence",
        description: "Enable RF coexistence feature",
        longDescription: `
Enable the use of external pin signaling to communicate with another RF capable
device, for the devices to coexist in the same frequency band.
The coexistence (coex) feature is a wired signal interface between a BLE or IEEE
15.4 device and a Wi-Fi device:

\`\`\`
Coex client             Coex host
+---------+             +---------+
|  BLE /  |   Signals   |  Wi-Fi  |
|IEEE 15.4| <----/----> |         |
+---------+      n      +---------+
\`\`\`

Resources:
* [Wired RF Coexistence chapter of the SDK User Guide][1]

[1]: /rcl/html/rcl_coex.html

`
    },
    configGroup: {
        displayName: "RF Coexistence Configuration"
    },
    mode: {
        displayName: "Coex Mode",
        description: "Select coex mode of operation",
        longDescription: `
The coex interface provides a set of signals based on the supported
Packet Traffic Arbitration (PTA) approach:

* REQUEST: Output signal, indicating a request to perform RF activity.
* PRIORITY: Output signal, time-shared between indicating (1) request priority and (2) type of RF activity.
* GRANT: Input signal, indicating the response to the request to perform RF activity.

The available combinations of these signals are defined as _coex modes_.
This option is used to match the interface of the coex host with which the device
is coexisting. The following signals are enabled for the different modes:

|Coex Mode                | REQUEST | PRIORITY | GRANT |
|-------------------------|:-------:|:--------:|:-----:|
| 3-Wire                  | x       | x        | x     |
| 2-Wire REQUEST-GRANT    | x       |          | x     |
| 2-Wire REQUEST-PRIORITY | x       | x        |       |
| 1-Wire REQUEST          | x       |          |       |
| 1-Wire GRANT            |         |          | x     |
`,
        threeWire: {
            displayName: "3-Wire",
            description: "Use pins for REQUEST, PRIORITY and GRANT signals"
        },
        twoWireRequestGrant: {
            displayName: "2-Wire REQUEST-GRANT",
            description: "Use pins for REQUEST and GRANT signals"
        },
        twoWireRequestPriority: {
            displayName: "2-Wire REQUEST-PRIORITY",
            description: "Use pins for REQUEST and PRIORITY signals"
        },
        oneWireRequest: {
            displayName: "1-Wire REQUEST",
            description: "Use pin for REQUEST signal"
        },
        oneWireGrant: {
            displayName: "1-Wire GRANT",
            description: "Use pin for GRANT signal"
        }
    },
    priorityIndicationEnable: {
        displayName: "Enable Priority Indication",
        description: "Indicate if the PRIORITY line should give priority indication before switching to RX and TX indication",
        longDescription: `
The PRIORITY signal is time-shared between indicating (1) request priority and (2) type of
RF activity. If this option is deselected, (1) is skipped.
`
    },
    priorityIndicationTime: {
        displayName: "Priority Indication Time",
        description: "Duration in us for time-shared PRIORITY signal to indicate priority",
        longDescription: `
The PRIORITY signal is time-shared between indicating (1) request priority and
(2) type of RF activity. The __Priority Indication Time__ option specifies for
how long in microseconds (from when the REQUEST signal is asserted) the
PRIORITY signal indicates (1) request priority. After the specified time, the
REQUEST signal indicates (2) type of RF activity. The valid range is
3&ndash;30 microseconds.
`
    },
    requestPinIdleLevel: {
        displayName: "REQUEST Signal Idle Level",
        description: "Pin value when REQUEST signal is idle",
        longDescription: `
The signal idle level is the opposite of the signal active level. When the
REQUEST signal is configured with idle level _low_, the signal is active _high_.
This option is used to specify how the signal level should be interpreted.

| Idle level | REQUEST asserted level |
|:----------:|:----------------------:|
| Low        | High                   |
| High       | Low                    |
`
    },
    priorityPinIdleLevel: {
        displayName: "PRIORITY Signal Idle Level",
        description: "Pin value when PRIORITY signal is idle",
        longDescription: `
The signal idle level is the opposite of the signal active level, which
indicates TX. When the PRIORITY signal is configured with idle level _low_,
the signal is active _high_. This option is used to specify how the signal
level should be interpreted.

| Idle level | PRIORITY level indicating RX | PRIORITY level indicating TX |
|:----------:|:----------------------------:|:----------------------------:|
| Low        | Low                          | High                         |
| High       | High                         | Low                          |
`
    },
    priorityIndicationLowLevel: {
        displayName: "PRIORITY Signal Low Priority Level",
        description: "Pin value when PRIORITY signal is giving low priority indication",
        longDescription: `
The low priority level is the opposite of the high priority level. This signal
level is used during the priority indication. The level after priority
indication is given with the __PRIORITY Signal Idle Level__ setting.

| Low priority level | High priority level |
|:------------------:|:-------------------:|
| Low                | High                |
| High               | Low                 |
`
    },
    requestRfActivityLatencyTime: {
        displayName: "Coex RF Activity Latency Time",
        description: "Coex RF Activity Latency Time to Request Signal (us)",
        longDescription: `
The Coex RF Activity Latency Time Latency Time (T1) is used to synchronize
assertion of the REQUEST signal with the expected time it takes for the coex
host to respond with the GRANT signal or turn off its transmitter. The valid
range is 90&ndash;150 microseconds.

\`\`\`
               |-----T1-----|
               |--glt--|-T3-|
               .       .  . .____
RF     ________._______.__._| tx |___________
               ._______.__._________
REQ    ________|       .  .         |________
       ________________.  .
GRANT                  |__.__________________
                          ^
                          tg: Grant Sample Point
Legend:
  T1:  RF Activity Latency Time Latency Time; time from REQUEST is asserted to RF activity
  T3:  Time period where GRANT signal is expected to be safe to sample
  tg:  GRANT Sample Point, fixed time 1-2us before RF activity
  glt: GRANT Latency Time
\`\`\``
    },
    grantPinIdleLevel: {
        displayName: "GRANT Signal Idle Level",
        description: "Pin value when GRANT signal is indicating no grant",
        longDescription: `
The signal idle level is the opposite of the signal active level. When the
GRANT signal is configured with idle level _low_, the signal is active _high_.
This option is used to specify how the signal level should be interpreted.

| Idle level | GRANT given level |
|:----------:|:-----------------:|
| Low        | High              |
| High       | Low               |
`
    },
    runtimeChangeable: {
        displayName: "Coex Configuration Changeable",
        description: "Allow coex configuration to be changed run-time",
        longDescription: `
Allow the coex configuration to be changed run-time. If this option is deselected, the coex configuration
is declared const and cannot be changed after the device has been started. If selected the coex configuration
will reside in RAM and an extern statement to the configuration structure \`lrfCoexConfiguration\` is given.
`
    },
    pinRequestIeeeNumCorr: {
        displayName: "IEEE 802.15.4 REQUEST Threshold",
        description: "IEEE 802.15.4 only: Number of preamble symbols needed to assert REQUEST signal in RX",
        longDescription: `
This setting is only used on IEEE 802.15.4. If the command uses 
\`.coexControl.rxMode = RCL_CoexRxMode_RequestOnPacket\`, the REQUEST signal is asserted during RX
when this number of subsequent preamble symbols is detected. (Each symbol is 4 bits, and a standard 
preamble has 8 symbols.) A lower number gives faster detection, but more false positives.
`
    }
};


exports = {
    base,
    config,
    coex
};
