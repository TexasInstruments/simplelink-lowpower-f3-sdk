# Temperature Sensor

* [Introduction](#intro)
* [Software Prerequisites](#software-prereqs)
* [Functional Description](#functional-desc)
    * [Software Overview](#software-overview)
        * [Application Files](#application)
* [Configuration With SysConfig](#sysconfig)
* [Example Usage](#usage)
    * [Connecting the Device to the Amazon Echo](#setup)
    * [Setting the Reporting Period](#set-reporting-period)
    * [Code Flow of Retrieving Temperature](#get-temperature)
    * [Commissioning the Device Into the Network](#Commission-Device)

# <a name="intro"></a> Introduction

This document discusses how to use the Temperature Sensor Sample App and the
different parts that compose it. Temperature Sensor Sample App is an example
that exercises different features of TI End Device (ZED) implementation.

Some of the features exercised include:

- Zigbee End Device network formation
- Output device that acts as *Target* of Finding and Binding
- Periodically receiving temperature data

# <a name="software-prereqs"></a> Software Prerequisites

- [Code Composer Studio&trade; (CCS)](http://processors.wiki.ti.com/index.php/Download_CCS#Download_the_latest_CCS) v12.7 or newer

- [SimpleLink&trade; LOWPOWER SDK](http://www.ti.com/tool/SIMPLELINK-LOWPOWER-SDK)

- Amazon Echo Device

- Amazon Alexa App (downloaded on your phone)

# <a name="functional-desc"></a> Functional Description

## <a name="software-overview"></a> Software Overview

This section describes the software components and the corresponding source files.

### <a name="application"></a> Application Files

The Temperature Sensor ZED Sample App works in conjunction with a Zigbee Coordinator
(ZC), in particular an Amazon Echo. Upon startup, the Temperature Sensor ZED initializes
necessary hardware components. It then starts the network formation. After the ZED joins the network, it will be ready
to receive temperature data that will show up on the Amazon Alexa app when viewing your temperature sensor device.

- **temperature_sensor.c:** Contains the application's event loop, device initialization
function calls, network formation, logic to set the frequency of receiving the temperature, 
and the callback function to retrieve the temperature.

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

Assuming that you built your temperature sensor image using the settings mentioned in the sysconfig section, flash the image
onto your device. Make sure that your device is within 30 feet of your Amazon Echo. Press the
reset button on your device in order to put it into pairing mode.

On the Amazon Alexa app, tap the light bulb icon on the button left of the app. Tap the
"+" button to add a new device, then tap "add device". Tap the type of device to add. For this
example, you will be adding a temperature sensor, which isn't part of the dropdown of devices to add.
You will have to scroll down to the bottom of the devices list and tap the "Other" button.
When asked what brand your device is, tap Zigbee.

## <a name="set-reporting-period"></a> Setting the Reporting Period

You can set how often the temperature sensor reports the temperature by modifying the `TEMP_TIMEOUT` macro in
`temperature_sensor.c`. The macro is listed in the "Application defines" section of the source file. The default reporting period is
set to one minute, meaning that the temperature will be retrieved by the sensor every minute.
You are free to change the reporting period to whatever you like, but keep in mind that the reporting period is
measured in microseconds, hence why the default period has a value of 60 million.

## <a name="get-temperature"></a> Code Flow of Retrieving Temperature

In `temperature_sensor.c`, the `MAIN()` function initializes a clock handle through `ClockP` to call the
`tempTimeoutHandler()` function every `TEMP_TIMEOUT` microseconds. When `tempTimeoutHandler()` gets called,
it will take the value returned by `Temperature_getTemperature()` and then multiply that value by 100. The
`Temperature_getTemperature()` function gets the current temperature of the device's chip in Celsius. This
function is defined externally in `<ti/drivers/Temperature.h>`. The `g_attr_temp_measurement_value` variable
is set to the value returned by `Temperature_getTemperature() * 100`. `g_attr_temp_measurement_value` is part of the attributes
value in the Temperature Measurement cluster, and is ultimately the value that you will see on the screen.

Note that during the first `TEMP_TIMEOUT` microseconds (in other words, before the first retrieval occurs), you will see on the
Amazon Alexa app that the temperature is -1.0 degrees. This is because the retrieval function `tempTimeoutHandler()` hasn't been
called yet since the first retrieval hasn't occurred yet. The -1.0 likely comes from the initial value of `g_attr_temp_measurement_value`
being set to `ZB_ZCL_TEMP_MEASUREMENT_VALUE_DEFAULT_VALUE`, which in turn is set at `ZB_ZCL_NON_VALUE_INT16`, which is `0x8000`. This value
will be overwritten once `tempTimeoutHandler()` is called for the first time and the temperature is properly retrieved, which will in turn
update the value on the app to display the correct temperature.

## <a name="Commission-Device"></a> Forming the Network and Commissioning a Device into the Network

Assuming you have the Amazon Echo set up and added as a device on your Amazon Alexa app,
the Echo will function as a Zigbee Coordinator, which forms the network. At startup, the Echo will
start the Zigbee network and wait for *Beacon Requests* from the ZED attempting to join the network.
The Echo will send management permit joining request packets to signify that it is trying to discover
a device to connect to the network. If the device successfully connects to the network, you should
see the following packets sent over the air:
- Device Announce packets that signify the presence of the new temperature sensor
- Active Endpoints Request/Response packets
- Simple Descriptor Request/Response packets
- Node Descriptor Request/Response packets
- Request Key sent from the temperature sensor to the Echo + Acknowledgement
- Transport Key sent from the Echo to the temperature sensor + Acknowledgement
- Verify Key sent from the temperature sensor to the Echo + Acknowledgement
- Confirm Key sent from the Echo to the temperature sensor + Acknowledgement
- Basic: Read Attributes + Response packets
- Temperature Measurement: Configure Reporting + Response packets
- Bind Request/Response packets
- Temperature Measurement: Report Attributes + Response packets
- Identify: Read Attributes + Response packets

The "Management Permit Join Request: Disabled" packets are sent out when the Echo ends the process
of looking for devices. After these packets get sent, you will either see on your app that the Echo
successfully found a device, or that it failed to connect to your device. If you see the latter on your
app, then make sure that you pressed the reset button on your device before trying again since this
puts the device into pairing mode. Otherwise, you are done, and the device has successfully been found
and connected to the network.

After connecting to the network, the Temperature Sensor will periodically update the temperature, with
the period being determined by the value of `TEMP_TIMEOUT`, as described earlier. Note again that the
temperature will initially be reported as -1.0 until the first `TEMP_TIMEOUT` microseconds have passed,
which happens because the function to retrieve the temperature does not get called at the start of execution,
only after `TEMP_TIMEOUT` microseconds. You can see the temperature on the Amazon Alexa app by tapping on the
temperature sensor device in your list of devices connected to the network.