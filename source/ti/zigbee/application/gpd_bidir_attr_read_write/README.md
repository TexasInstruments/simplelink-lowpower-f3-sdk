# Green Power Read/Write/Request Attribute

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

This document discusses how to use the Green Power Read/Write/Req Attribute Application and the different parts that compose it.
Green Power Read/Write/Req Attribute Application is an example which exercises different features of TI Zigbee for a green power device implementation.
The example can operate as battery powered device while using bidirectional commissioning and bidirectional operation.

Some of the features exercised include:

- Green Power application ID configuration for source ID or IEEE.
- Security level configuration.
- Commissioning data frame.
- Sequence number capabilities.
- Bidirectional operational mode including:
    - Write Attribute (RX)
    - Read Attribute (RX)
    - Read Attribute Response (TX)
    - Request Attribute (TX)

# <a name="software-prereqs"></a> Software Prerequisites

- [Code Composer Studio&trade; (CCS)](http://processors.wiki.ti.com/index.php/Download_CCS#Download_the_latest_CCS) v12.5 or newer

- [SimpleLink&trade; LOWPOWER SDK](http://www.ti.com/tool/SIMPLELINK-LOWPOWER-SDK)

# <a name="functional-desc"></a> Functional Description

## <a name="software-overview"></a> Software Overview

This section describes the software components and the corresponding source files.

### <a name="application"></a> Application Files

The device will behave as a battery operated device, being in deep sleep until a key is pressed.

- **gpd_bidir_attr_read_write.c:** Contains the application's event loop, device initialization function calls, and all Commissioning/Attribute Reporting command specific logic.

# <a name="sysconfig"></a> Configuration With SysConfig

SysConfig is a GUI configuration tool that allows for TI driver and stack configurations.

To configure using SysConfig, import the SysConfig-enabled project into CCS. Double click the `*.syscfg` file from the CCS project explorer, where `*` is the name of the example project. The SysConfig GUI window will appear, where Zigbee stack and TI driver configurations can be adjusted. These settings will be reflected in the generated files.

The example project comes with working default settings for SysConfig. For the purposes of this README, it is not recommended to change the default driver settings, as any changes may impact the functionality of the example. The Zigbee stack settings may be changed as required for your use case.

Note that some Zigbee settings are stored in non-volatile storage, and Zigbee prioritizes stored settings over SysConfig settings. To guarantee SysConfig settings are applied, perform a factory reset of the device to  clear non-volatile storage.

# <a name="usage"></a> Example Usage

This section describes how to use this sample application.

## <a name="usage-buttons"></a> Buttons

Buttons have different usage depending on whether the device is commissioned or not.

When not commissioned (first start):
- `BTN-1`: Starts the commissioning process.
- `BTN-2`: N/A

After commissioning, the device will send a request attribute command one time.

Then, every 15 seconds, the button state is read:
- `BTN-1`: Send a request attribute command.
- `BTN-2`: Send an attribute reporting command.


## <a name="Commission-Device"></a> Commissioning the device Into the network

Green Power Devices can be commissioned to a Zigbee network by sending a commissioning GPDF, a GPDF with auto-commissioning or a commissioning tool. A Zigbee device with green power proxy capabilities can create an entry for a Green Power Device with the commissioning data. The proxy device will translate the GPDF commands to ZCL notifications for the Zigbee network.