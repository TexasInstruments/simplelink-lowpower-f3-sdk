# Doorlock 

* [Introduction](#intro)
* [Software Prerequisites](#software-prereqs)
* [Functional Description](#functional-desc)
    * [Software Overview](#software-overview)
        * [Application Files](#application)
* [Configuration With SysConfig](#sysconfig)
* [Example Usage](#usage)
    * [Connecting the Device to the Amazon Echo](#setup)
    * [Commissioning the Device Into the Network](#Commission-Device)

# <a name="intro"></a> Introduction

This document discusses how to use the Door Lock Sample App and the
different parts that compose it. Door Lock Sample App is an example
that exercises different features of TI Zigbee Router (ZR) implementation.

Some of the features exercised include:

- Zigbee Router network formation
- Output device that acts as *Target* of Finding and Binding
- Receive and respond to Lock/Unlock Toggle data frame

# <a name="software-prereqs"></a> Software Prerequisites

- [Code Composer Studio&trade; (CCS)](http://processors.wiki.ti.com/index.php/Download_CCS#Download_the_latest_CCS) v12.7 or newer

- [SimpleLink&trade; LOWPOWER SDK](http://www.ti.com/tool/SIMPLELINK-LOWPOWER-SDK)

- Amazon Echo Device

- Amazon Alexa App (downloaded on your phone)

# <a name="functional-desc"></a> Functional Description

## <a name="software-overview"></a> Software Overview

This section describes the software components and the corresponding source files.

### <a name="application"></a> Application Files

The Door Lock ZR Sample App works in conjunction with a Zigbee Coordinator
(ZC), in particular an Amazon Echo. Upon startup, the Door Lock ZR initializes
necessary hardware components, including the GPIO connected to Green LED. It then
starts the network formation. After the ZR joins the network, it will be ready
to receive Lock/Unlock toggle commands to toggle the Green LED.

- **door_lock.c:** Contains the application's event loop, device initialization
function calls, and network formation and lock/unlock toggle command specific logic.

# <a name="sysconfig"></a> Configuration With SysConfig

SysConfig is a GUI configuration tool that allows for TI driver and stack configurations.

To configure using SysConfig, import the SysConfig-enabled project into CCS. Double
click the `*.syscfg` file from the CCS project explorer, where `*` is the name of the
example project. The SysConfig GUI window will appear, where Zigbee stack and TI driver
configurations can be adjusted. These settings will be reflected in the generated files.

The example project comes with working default settings for SysConfig. For the purposes
of this README, it is not recommended to change the default driver settings, as any
changes may impact the functionality of the example. The Zigbee stack settings may be
changed as required for your use case. In our case, we set the radio channels to
11, 15, 20, and 25 because the Amazon Echo devices broadcast on these channels.
If none of those channels are selected, then the device won't connect to the Amazon Echo.
Note that the app actions are only displayed over the air on one of the four channels.
Typically, the actions are seen on channel 11.

Note that some Zigbee settings are stored in non-volatile storage, and Zigbee
prioritizes stored settings over SysConfig settings. To guarantee SysConfig settings are
applied, perform a factory reset of the device to  clear non-volatile storage.

# <a name="usage"></a> Example Usage

This section describes how to use this sample application. As pre-requisites, you need to have your
Amazon Echo set up and the Amazon Alexa app on your phone. Add the Echo to your devices on the app.

## <a name="setup"></a> Connecting a Device to the Amazon Echo

Assuming that you built your door lock image using the settings mentioned in the sysconfig section, flash the image
onto your device. Make sure that your device is within 30 feet of your Amazon Echo. Press the
reset button on your device in order to put it into pairing mode.

On the Amazon Alexa app, tap the light bulb icon on the button left of the app. Tap the
"+" button to add a new device, then tap "add device". Tap the type of device to add. For this
example, you will be adding a lock, so tap the "Lock" button. When asked what brand your device is,
tap Zigbee. If Zigbee isn't listed as an option, then tap "Other", and then tap Zigbee.

## <a name="Commission-Device"></a> Forming the Network and Commissioning a Device into the Network

Assuming you have the Amazon Echo set up and added as a device on your Amazon Alexa app,
the Echo will function as a Zigbee Coordinator, which forms the network. At startup, the Echo will
start the Zigbee network and wait for *Beacon Requests* from the ZR attempting to join the network.
The Echo will send management permit joining request packets to signify that it is trying to discover
a device to connect to the network. If the device successfully connects to the network, you should
see the following packets sent over the air:
- Device Announce packets that signify the presence of the new door lock
- Active Endpoints Request/Response packets
- Simple Descriptor Request/Response packets
- Node Descriptor Request/Response packets
- Request Key sent from the door lock to the Echo + Acknowledgement
- Transport Key sent from the Echo to the door lock + Acknowledgement
- Verify Key sent from the door lock to the Echo + Acknowledgement
- Confirm Key sent from the Echo to the door lock + Acknowledgement
- Basic: Read Attributes + Response packets
- Door Lock: Configure Reporting + Response packets
- Bind Request/Response packets
- Door Lock: Read Attributes + Response packets
- Identify: Read Attributes + Response packets

The "Management Permit Join Request: Disabled" packets are sent out when the Echo ends the process
of looking for devices. After these packets get sent, you will either see on your app that the Echo
successfully found a device, or that it failed to connect to your device. If you see the latter on your
app, then make sure that you pressed the reset button on your device before trying again since this
puts the device into pairing mode. Otherwise, you are done, and the device has successfully been found
and connected to the network.

At this point, the Door Lock is ready to respond to the Lock/Unlock toggle commands from the ZC. You
can trigger these toggle commands on the Amazon Alexa app by tapping on your new lock device and then
tapping the button on the center of the screen that allows you to lock/unlock the device.