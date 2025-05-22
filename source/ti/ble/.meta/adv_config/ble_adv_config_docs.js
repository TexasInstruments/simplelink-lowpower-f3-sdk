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
 *  ======== ble_adv_config_docs.js ========
 */

"use strict";

// Long description for the disableDisplayModule configuration parameter
const disableDisplayModuleLongDescription = `If This option is active, all display statements\
are removed and no display operations will take place.\n
For more information, refer to Display.h and the [BLE User's Guide](ble5stack/ble_user_guide/html/\
ble-stack-5.x/stack-configuration-cc23xx.html#run-time-bluetooth-low-energy-protocol-\
stack-configuration).\n
__Default__: False (unchecked)\n`

// Long description for the halAssert configuration parameter
const halAssertLongDescription = `Defining this assert will forward \
ble5stack asserts to the application.\n
For more information, refer to the [BLE User's Guide](ble5stack/ble_user_guide/html/\
ble-stack-5.x/stack-configuration-cc23xx.html#run-time-bluetooth-low-energy-protocol-stack-\
configuration).\n
__Default__: False (unchecked)\n`

// Long description for the dontTransmitNewRpa configuration parameter
const dontTransmitNewRpaLongDescription = `According to E10336 errata, when \
using connectable directed extended advertising, it is recommended that the \
advertiser will use a different initiator RPA in the connect response than \
the one that was used in the connect request.\n
For backwards compatibility the DONT_TRANSMIT_NEW_RPA flag was added. \n
When DONT_TRANSMIT_NEW_RPA flag is enabled, this recommendation will not take \
affect and the advertiser will use the same RPA. \n
__Default__: False (unchecked)\n`

// Long description for the extendedStackSettings configuration parameter
const extendedStackSettingsLongDescription = `
In our BLE5-Stack, there are 2 options for LL scheduler to form anchor points: \n

Options | Description
--- | ---
Default | The anchor points are formed randomly with no restriction on timing.\
This is the case for all our out of box examples
Guard Time | The anchor points are formed randomly with minimum 5ms guard time\n

__Note__: \n
We can achieve 32 connections running out of box simple_central project. \n
To enable this functionality choose Guard Time.\n
For more information, refer to the [BLE User's Guide](ble5stack/ble_user_guide/html/\
ble-stack-common/link-layer-cc23xx.html#central-role).\n`

// Long description for the maxNumEntIcall configuration parameter
const maxNumEntIcallLongDescription = `Defines maximum number of \
entities that use ICall, including service entities and application entities. \
Modify only if adding a new TI-RTOS task that uses ICall services.\n
For more information, refer to the [BLE User's Guide](ble5stack/ble_user_guide/html/\
ble-stack-5.x/the-application-cc23xx.html#icall).\n
__Default__: 6\n`

// Long description for the maxNumIcallEnabledTasks configuration parameter
const maxNumIcallEnabledTasksLongDescription = `Defines the number of ICall aware \
tasks. Modify only if adding a new TI-RTOS task that uses ICall services.\n
For more information, refer to the [BLE User's Guide](ble5stack/ble_user_guide/html/\
ble-stack-5.x/the-application-cc23xx.html#icall).\n
__Default__: 3\n`

// Long description for the flowControl configuration parameter
const flowControlLongDescription = `Defines whether the NPI will enable packet flow control.\n
__Note__: The only way to prevent RxBuf overflow is to enable NPI_FLOW_CTRL. If the buffer \
has overflowed there is no way to safely recover. All received bytes can be packet fragments \
so if a packet fragment is lost the frame parser behavior becomes undefined.\n
If NPI_FLOW_CTRL is not enabled then  the peripheral cannot control the central's transfer rate. \
With NPI_FLOW_CTRL the peripheral has SRDY to use as a software flow control mechanism.\n
When using NPI_FLOW_CTRL make sure to increase NPI_TL_BUF_SIZE to suit the NPI frame length \
that is expected to be received.\n
__Default__: 0\n`

// Long description for the ptm configuration parameter
const ptmLongDescription = `Production Test Mode (PTM), allows for an embedded\
 software application to support direct test mode without exposing the HCI to the UART\
 pins under normal operation.\n
 __Note__: The pins used for PTM can also be used for an application UART interface.\
 In this case, it is necessary to ensure that the other device that is connected to the\
 UART interface does not run at the same time that DTM is being exercised. If the device\
 powers up and goes into PTM mode (by a GPIO being held high or low or some other stimulus),\
 the UART will then be used for DTM commands. If the device powers up normally and does\
 not go into PTM mode, then the UART can be initialized by the application and used to\
 communicate with the other device. DTM commands can also be called by the embedded BLE\
 application.\n
For more information, refer to the [BLE User's Guide](ble5stack/ble_user_guide/html/\
ble-stack-5.x/ptm-and-dtm.html#production-test-mode-ptm).\n
__Default__: False (unchecked)\n`

const peripheralExtraLFOSCPPM = `Peripheral RX Window Widening is relevant when Source Clock LFOSC is selected in the CCFG module.\n
The BLE Stack will always try to find the Central device by slowly widening the RX window in case a Connection Event was missed.\n
Users are allowed to choose the extra PPM for RX Window Widening.\n
Choosing greater values will cause the BLE Stack to open a big RX window in order to quickly find the Central device.\n
Choosing smaller values will save power due to smaller RX windows but will take more time to find the Central device.\n
__Default__: 1500PPM\n`

// Long description for the peerConnParamUpdateRejectInd configuration parameter
const peerConnParamUpdateRejectIndLongDescription = `When enabling this parameter\
 the application will be notified on any incoming connection parameter\
 update request that has been received from the peer and was rejected by\
 the Link Layer. \n
__Default__: False (unchecked)\n`

const adaptivityLongDescription =  `Enable Adaptivity module,\
 This module is responsible for monitoring and limiting the transmit time on each BLE channel,\
 This module can be used to help meet the adaptivity requirements of some regulatory regions.\n
__Default__: Disabled\n`

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

const rxWindowDurationLongDescription =  `Duration of the time the RX window opens to validate the channel\
 status (noisy or clear). this value must be minimum 120µs.\n
__Default__: 12\n`

const fixedObservTimeLongDescription =  `When unmarked, the Observation Period is calculated by dwell time multiplied by 100,\
 Dwell time - The time between frequency changes for FHSS equipment.\
 EN 300 328 notes that this time may comprise transmit, receive, and idle phases of the equipment\n
__Default__: False\n`

const observationTimeLongDescription =  `Fixed observation period. This value will not change according to app behavior\n
__Note__: in units of 100 ms\n
__Default__: False\n`

const blockingChannelTimeLongDescription =  `When the channel is determined to be noisy, only empty packets will sent \
 on the channel for the selected time period.\n
__Default__: False\n`

 // Exports the long descriptions for each configurable
 exports = {
    disableDisplayModuleLongDescription: disableDisplayModuleLongDescription,
    halAssertLongDescription: halAssertLongDescription,
    dontTransmitNewRpaLongDescription: dontTransmitNewRpaLongDescription,
    extendedStackSettingsLongDescription: extendedStackSettingsLongDescription,
    maxNumEntIcallLongDescription: maxNumEntIcallLongDescription,
    maxNumIcallEnabledTasksLongDescription: maxNumIcallEnabledTasksLongDescription,
    ptmLongDescription: ptmLongDescription,
    flowControlLongDescription: flowControlLongDescription,
    peerConnParamUpdateRejectIndLongDescription: peerConnParamUpdateRejectIndLongDescription,
    peripheralExtraLFOSCPPM: peripheralExtraLFOSCPPM,
    adaptivityLongDescription: adaptivityLongDescription,
    AdaptivityModeLongDescription: AdaptivityModeLongDescription,
    externalTxPowerGainLongDescription: externalTxPowerGainLongDescription,
    externalRxPowerGainLongDescription: externalRxPowerGainLongDescription,
};