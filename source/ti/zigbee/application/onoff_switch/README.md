# On/Off Cleint Switch ZED

* [Introduction](#intro)
* [Software Prerequisites](#software-prereqs)
* [Functional Description](#functional-desc)
    * [Software Overview](#software-overview)
        * [Application Files](#application)
* [Configuration With SysConfig](#sysconfig)
* [UART Logging](#uart-logging)
* [Example Usage](#usage)
    * [Buttons](#usage-buttons)
    * [Commissioning the Device Into the Network](#Commission-Device)

# <a name="intro"></a> Introduction

This document discusses how to use the On/Off Cleint Switch ZED Sample App and the different parts that compose it. On/Off Cleint Switch Sample Application is an
example that exercises different features of TI Zigbee for a Zigbee End Device (ZED)
implementation.

Some of the features exercised include:

- Zigbee End Device commissioning
- Switch device that acts as *Initiator* of Finding and Binding
- On/Off Toggle data frame

# <a name="software-prereqs"></a> Software Prerequisites

- [Code Composer Studio&trade; (CCS)](http://processors.wiki.ti.com/index.php/Download_CCS#Download_the_latest_CCS) v12.7 or newer

- [SimpleLink&trade; LOWPOWER SDK](http://www.ti.com/tool/SIMPLELINK-LOWPOWER-SDK)

# <a name="functional-desc"></a> Functional Description

## <a name="software-overview"></a> Software Overview

This section describes the software components and the corresponding source files.

### <a name="application"></a> Application Files

The On/Off Cleint Switch ZED Sample App works in conjuction with a Zigbee Coordinator (ZC),
which is assumed to have formed a Zigbee network that the On/Off Cleint Switch ZED can
join. For this, the On/Off Cleint Output ZC Sample App can be used. Upon startup, On/Off
Server Switch ZED sends a commissioning request to attempt to join the network. Once it
has joined the network successfully, BTN1 on the Launchpad can be pressed to send a Toggle
command.

- **on_off_switch.c:** Contains the application's event loop, device initialization
function calls, and commissioning and on/off toggle command specific logic.

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

## <a name="usage-buttons"></a> Buttons

- `BTN-1`: Sends On/Off Toggle commands

## <a name="Commission-Device"></a> Commissioning the device Into the network

Zigbee End Devices can only join networks. At startup, the On/Off Cleint Switch ZED
Sample App goes into *ZB_BDB_NETWORK_STEERING* mode where it will first send a
*Beacon Request*, wait for beacons, and then send a *Network Commissioning Request*
packet to a ZC/ZR. The ZR/ZC then responds and the commissioning process will continue
until the ZED is joined into the network.
---

# <a name="uart-logging"></a> UART Logging

This example has logging enabled by default (`zigbee.loggingEnabled = true` in the `.syscfg` file).

## How It Works

Logs are encoded in a compact **binary format** over UART at **115200 baud**. A standard serial terminal will show garbled characters — you must use `tilogger` to decode them.

The binary format stores log level, module ID, and arguments as raw bytes rather than ASCII strings. This keeps UART traffic minimal so logging has negligible impact on real-time Zigbee operation.

## Decoding Logs with tilogger

`tilogger` runs on **native Windows only** (not WSL — binary frames get corrupted over WSL serial).

```powershell
# Install (once):
pip install tilogger

# Decode live from UART:
tilogger --elf <path_to_build>/<example_name>.out uart <COM_PORT> 115200 stdout

# Example:
tilogger --elf examples/rtos/LP_EM_CC2755P20/zigbee/onoff_switch/freertos/ticlang/onoff_switch.out uart COM24 115200 stdout
```

## Enabling / Disabling Logging

In the example `.syscfg` file:

```javascript
// Enable (default):
zigbee.loggingEnabled = true;

// Disable (saves ~2KB RAM and reduces code size):
zigbee.loggingEnabled = false;
```

When disabled, also remove the `LogSinkUART` section from the syscfg.

## Log Modules

| Module | What it covers |
|--------|---------------|
| `LogModule_Zigbee_App` | Application-level events (commissioning, OTA status, signals) |
| `LogModule_Zigbee_OSIF` | OSIF-level events (NVS open/close, flash ops, timers) |
| `LogModule_Zigbee_LLMAC` | MAC-level events (uses `LogSinkBuf` — too high-frequency for UART) |
