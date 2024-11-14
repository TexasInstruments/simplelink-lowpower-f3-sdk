# On/Off Server Output ZC

* [Introduction](#intro)
* [Software Prerequisites](#software-prereqs)
* [Functional Description](#functional-desc)
    * [Software Overview](#software-overview)
        * [Application Files](#application)
* [Configuration With SysConfig](#sysconfig)
* [Example Usage](#usage)
    * [Buttons](#usage-buttons)
    * [Commissioning the Device Into the Network](#Commission-Device)

# <a name="intro"></a> Introduction

This document discusses how to use the On/Off Server Output Sample App and the
different parts that compose it. On/Off Server Output Sample App is an examples
that exercises different features of TI Zigbee Coordinator (ZC) implementation.

Some of the features exercised include:

- Zigbee Coordinator network formation
- Output device that acts as *Target* of Finding and Binding
- Receive and respond to On/Off Toggle data frame

# <a name="software-prereqs"></a> Software Prerequisites

- [Code Composer Studio&trade; (CCS)](http://processors.wiki.ti.com/index.php/Download_CCS#Download_the_latest_CCS) v12.7 or newer

- [SimpleLink&trade; LOWPOWER SDK](http://www.ti.com/tool/SIMPLELINK-LOWPOWER-SDK)

# <a name="functional-desc"></a> Functional Description

## <a name="software-overview"></a> Software Overview

This section describes the software components and the corresponding source files.

### <a name="application"></a> Application Files

The On/Off Server Output ZC Sample App works in conjunction with a Zigbee End
Device (ZED) or a Zigbee Router (ZR). For this, the On/Off Server Switch ZED
Sample App can be used. Upon startup, the On/Off Server Output ZC initializes
necessary hardware components, including the GPIO connected to Green LED. It then
starts the network formation. When the ZED/ZR is connected, the ZC is ready
to receive On/Off toggle commands to toggle the Green LED.

- **on_off_light.c:** Contains the application's event loop, device initialization
function calls, and network formation and on/off toggle command specific logic.

# <a name="sysconfig"></a> Configuration With SysConfig

SysConfig is a GUI configuration tool that allows for TI driver and stack configurations.

To configure using SysConfig, import the SysConfig-enabled project into CCS. Double
click the `*.syscfg` file from the CCS project explorer, where `*` is the name of the
example project. The SysConfig GUI window will appear, where Zigbee stack and TI driver
configurations can be adjusted. These settings will be reflected in the generated files.

The example project comes with working default settings for SysConfig. For the purposes
of this README, it is not recommended to change the default driver settings, as any
changes may impact the functionality of the example. The Zigbee stack settings may be
changed as required for your use case.

Note that some Zigbee settings are stored in non-volatile storage, and Zigbee
prioritizes stored settings over SysConfig settings. To guarantee SysConfig settings are
applied, perform a factory reset of the device to  clear non-volatile storage.

# <a name="usage"></a> Example Usage

This section describes how to use this sample application.

## <a name="Commission-Device"></a> Forming the Network and Commissioning a Device into the Network

Zigbee Coordinator forms the network. At startup, the On/Off Server Output ZC
Sample App starts the Zigbee network and wait for *Beacon Requests* from ZED/ZR
attempting to join the network. The ZC will then respond and the commissioning
process will continue until the ZED is joined into the network. At this point,
On/Off Server Output ZC is ready to respond to On/Off Toggle commands from the
ZED/ZR.