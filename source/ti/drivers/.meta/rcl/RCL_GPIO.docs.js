/*
 * Copyright (c) 2024, Texas Instruments Incorporated - http://www.ti.com
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
    rftracer: {
        displayName: "RF Tracer",
        description: "Enable the RF Tracer as log sink",
        longDescription: `
Enable and configure radio trace output.

The RF Tracer is a dedicated hardware module in the radio
used to serialize log statements (traces) embedded in the radio code.

**Requrirements:**
* This feature requires the use of an output pin, which can be configured in the options below.
* The traces are given in a proprietary serial format, and decoding the trace messages will require support from TI.

**Note:** Enabling of trace messages may affect radio performance.
        `
},
rftrcMode: {
    displayName: "RF Tracer Mode",
    description: "Mode for RF Tracer",
    longDescription:`
The RF tracer modes allows for selecting the source of the trace messages.
The source can be the radio (LRF domain) and/or the system CPU (Radio Control Layer).
    `,
    cpu: {
        displayName: "CPU only",
    },
    radio: {
        displayName: "Radio only",
    },
    both: {
        displayName: "CPU + Radio",
    }
},
rftrcTimestamp: {
    displayName: "Timestamp Enabled",
    description: "Enables Timestamp setting",
    longDescription: `
When timestamps are enabled, the log elements have timestamps beside them. This option is enabled and hidden by default.
    `,
    on: {
        displayName: "Enabled",
        description: "Timestamps are enabled"
    },
    off: {
        displayName: "Disabled",
        description: "Timestamps are disabled"
    }
},
rftrcPrescaler: {
    displayName: "RF Tracer Prescaler",
    description: "Set speed of RF Tracer",
    longDescription: `
This variable is the divisor that the max clock speed is divided by. DIV3, for example, divides the clock speed by 3.
The clock speed of the RF Tracer may need to be reduced if experiencing signal quality issues on the tracer signal.
    `,
    div1: {
        displayName: "Divide by 1",
        description: "Divide clock by 1"
    },
    div2: {
        displayName: "Divide by 2",
        description: "Divide clock by 2"
    },
    div3: {
        displayName: "Divide by 3",
        description: "Divide clock by 3"
    },
    div4: {
        displayName: "Divide by 4",
        description: "Divide clock by 4"
    }
},
rclObservables: {
    displayName: "RCL Observables",
    longDescription: `
Enable radio signals from the radio (LRF) domain. This can be done by selecting 'Use Case' or by
selecting specific singals in the 'Signals' drop-down menu.`
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
    tracerEnable: {
        displayName: "RFCTRC (Tracer)",
        description: "Select RFCTRC output line 7"
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
        displayName: "PBEGPO0",
        description: "Select PBE output line 0"
    },
    pbeGpo1: {
        displayName: "PBEGPO1",
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
    }
}
};

exports = {
    base,
    config
};
