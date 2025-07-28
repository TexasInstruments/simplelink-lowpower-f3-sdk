# MAC Sensor

* [Introduction](#intro)
* [Software Prerequisites](#software-prereqs)
* [Functional Description](#functional-desc)
    * [Peripherals Exercised](#PeripheralsExercised)
    * [Software Overview](#software-overview)
        * [Application Files](#application)
* [Configuration With SysConfig](#sysconfig)
* [Additional Configuration Settings](#sensor-mac-configuration-settings)
* [Example Usage](#usage)
    * [Example Application Dataflow](#ExampleApplicationDataflow)
    * [Callbacks for Incoming Data Packets](#IncomingData)

# <a name="intro"></a> Introduction

This document discusses how to use the MAC Sensor App and the different parts that compose it. The MAC Sensor App is an examples
that exercises different features of the TI MAC device implementation.

# <a name="software-prereqs"></a> Software Prerequisites

- [Code Composer Studio&trade; (CCS)](http://processors.wiki.ti.com/index.php/Download_CCS#Download_the_latest_CCS)

- SimpleLink&trade; LOWPOWER SDK

# <a name="functional-desc"></a> Functional Description

## <a name="PeripheralsExercised"></a>Peripherals Exercised

> To trigger various events, buttons can be used.

* `CONFIG_LED_RED` - Turns on after the sensor connects to the collector.
* `CONFIG_BTN_LEFT` - Press to initialize the sensor application.

## <a name="software-overview"></a> Software Overview

This section describes the software components and the corresponding source files.

### <a name="application"></a> Application Files

- **sensor.c:** Contains the application's event loop, device initialization function calls, button-handling functions, reading clock handler logic, and functionality for creating and sending sensor data messages to the collector.
- **jdllc.c:** Contains the event loop for the sensor's logical link controller for functionalities such as scans, association, polls, and data indication.
- **translate.c:** This is a "translation layer" where the ZBOSS format of MAC callbacks (macro header and buffer ID) are defined and passed in. The correct type of MAC response (ex: `zb_mac_beacon_notify_indication_t` is retrieved from an input buffer and then passed up as opposed to passing a buffer containing the payload), and that response type is passed up to either the JDLLC or sensor application layer.
- **sensor_mac_config_settings.h:** Contains additional configuration settings from the 15.4 Stack that have been mapped to ZBOSS.

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

# <a name="sensor-mac-config-settings"></a> Additional Configuration Settings

There are additional configurations that the user can set in `sensor_mac_config_settings.h`. This allows you to configure application behavior such as setting various clock intervals or channel(s) to scan on.

# <a name="usage"></a> Example Usage

This example project implements a sensor end device: one of potentially many network devices in a PAN. This end device reads sensor information and sends it to the PAN-coordinator at a configured interval. This example assumes a second LPF2 Launchpad is running the default 15.4-Stack Collector application code to act as the PAN-coordinator. There is no UI to view any example output.

## <a name="ExampleApplicationDataflow"></a>Example Application Dataflow

The sensor application has two processing loops each handling a different set of
events. These are as follows:

* Sensor_process: Sensor application event handling
	* Sensor event handling:
		* Start sensor scan for network (SENSOR_START_EVT)
		* Read sensor values and report to collector (SENSOR_READING_TIMEOUT_EVT)
		* Disassociate sensor (SENSOR_DISASSOC_EVT) (NOT IMPLEMENTED YET)
	* Triggers Jdllc_process
    * Button press input handling
* Jdllc_process: Sensor logical link controller event handling
	* Trickle timer handling (PAN Advertisement/PAN Configuration message events)
	* Poll event handling (JDLLC_POLL_EVT)
	* Association request handling (JDLLC_ASSOCIATE_REQ_EVT)
	* Coordinator realignment handling (JDLLC_COORD_REALIGN)
	* Scan backoff handling (JDLLC_SCAN_BACKOFF)
	* State change handling

The two processing loops handle specialized tasks to service sensor functionality.

An overview of the sensor jdllc states and state transitions is as follows:

	               Jdllc_states_initWaiting
	                          | SENSOR_START_EVT, initiated upon startup,
	                          |
	                          |
	         Existing         |          New
	         Network          |          Network
	            +-------------+-------------+
	            |                           |
	            V                           V
	  +--> Jdllc_states_                Jdllc_states_
	  |    initRestoring                joining
	  |         |                           |
	  |         V                           V
	  |    Jdllc_states_                Jdllc_states_
	  |    rejoined                     joined
	  |         |                           |
	  |         +-------------+-------------+
	  |                       | MAC reports sync loss (BCN mode, not implemented yet) or
	  |                       | CONFIG_MAX_DATA_FAILURES consecutive data frames
	  |                       | fail to be ACK'ed by collector
	  | Orphan scan +         |
	  | Coord realign         V
	  +----------------- Jdllc_states_
	                     orphan

## <a name="IncomingData"></a>Callbacks for Incoming Data Packets

The translation layer (specified in `translate.h` and `translate.c`) will process all incoming messages from the MAC layer and calls the corresponding callback in either the sensor application layer or jdllc layer. For incoming data packets, one of these two callbacks will be triggered:

* Data Indication Callback: Triggered when the MAC has successfully processed a valid data frame
* Comm Status Indication Callback: Triggered when a data frame is rejected due to a security failure. The status code in the ApiMac_mlmeCommStatusInd_t struct will contain more details regarding the error. Note that this is only applicable if MAC security is not disabled. (NOT IMPLEMENTED YET)