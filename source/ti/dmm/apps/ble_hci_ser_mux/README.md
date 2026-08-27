# BLE HCI Serial MUX Application

## Table of Contents

* [Introduction](#Introduction)
* [Hardware Prerequisites](#HardwarePrerequisites)
* [Software Prerequisites](#SoftwarePrerequisites)
* [BLE Services Overview](#Services)
  * [Provisioning Service](#ProvisioningService)
  * [MAC GATT Sensor Service](#MACGATTSensorService)
* [Usage](#Usage)
  * [Provisioning the Device](#ProvisioningDevice)
  * [Sensor Data and Configuration](#SensorDataAndConfiguration)

## <a name="Introduction"></a>Introduction

The `ble_hci_ser_mux` application demonstrates the integration of BLE and 15.4 protocols using the Dynamic Multi-protocol Manager (DMM). It provides BLE GATT services for provisioning and sensor data reporting, enabling seamless communication between BLE central devices and 15.4 networks.

## <a name="HardwarePrerequisites"></a>Hardware Prerequisites

This application is designed for TI LaunchPad devices with BLE and 15.4 capabilities. Ensure the hardware supports dual-protocol communication.

## <a name="SoftwarePrerequisites"></a>Software Prerequisites

Refer to the SDK release notes for compatible versions of Code Composer Studio or IAR Embedded Workbench. Import the project into your IDE workspace and build it using the provided configurations.

## <a name="Services"></a>BLE Services Overview

This application implements two BLE GATT services:

1. **Provisioning Service**: Used to configure the device for connecting to a 15.4 network.
2. **MAC GATT Sensor Service**: Used to report sensor data.

### <a name="ProvisioningService"></a>Provisioning Service

The Provisioning Service is defined in `provisioning_gatt_profile.h`. It provides characteristics for configuring 15.4 network parameters.

| Characteristic             | UUID   | Format          | Properties       | Description                                                                 |
|----------------------------|--------|-----------------|------------------|-----------------------------------------------------------------------------|
| Network PAN ID             | 0x1191 | 2 Byte Integer  | Read/Write       | Configures the 15.4 network's PAN ID.                                    |
| Sensor Frequency           | 0x1192 | 1 Byte Integer  | Read             | Sets the frequency of sensor data reporting.                               |
| Sensor Channel             | 0x1193 | 17 Byte Integer | Read/Write       | Configures the communication channel for the sensor.                       |
| IEEE FFD Address           | 0x1194 | 8 Byte Integer  | Read             | Provides the IEEE address of the 15.4 device.                            |
| Network Key                | 0x1195 | 16 Byte Integer | Read/Write       | Currently Unsupported                                         |
| Provision Sensor        | 0x1196 | 1 Byte Integer  | Read/Write       | Write 0xAA to Provision the sensor, 0xDD to detach from the network.                           |
| Provisioning State         | 0x1197 | 1 Byte Integer  | Read             | Notifies the current provisioning state.                                   |

### <a name="MACGATTSensorService"></a>MAC GATT Sensor Service

The MAC GATT Sensor Service is defined in `mac_gatt_sensor_profile.h`. It provides characteristics for reporting sensor data and configuring sensor behavior.

| Characteristic             | UUID   | Format          | Properties       | Description                                                                 |
|----------------------------|--------|-----------------|------------------|-----------------------------------------------------------------------------|
| Sensor Data                | 0x1181 | 2 Byte Integer  | Read/Notify      | Reports the current sensor data (e.g., temperature).                       |
| Sensor Report Interval     | 0x1182 | 4 Byte Integer  | Read       | The interval for sensor data reporting.                         |
| Identify Request           | 0x1183 | 1 Byte Integer  | Write            | Allows the BLE Sensor to trigger an identification request to the Collector. |
## <a name="Usage"></a>Usage

### <a name="ProvisioningDevice"></a>Provisioning the Device

1. Use a BLE central device (e.g., smartphone app) to connect target device and view the Provisioning Service.
2. Configure the 15.4 network parameters using the characteristics:
   - Set the **Network PAN ID** to match the 15.4 network.
3. Write 0xAA to the **Provisioning Sensor** characteristic to start the provisioning process.
4. Monitor the **Provisioning State** characteristic to track the provisioning status.

### <a name="SensorDataAndConfiguration"></a>Sensor Data and Configuration

1. View to the MAC GATT Sensor Service using a BLE central device.
2. Enable notifications for the **Sensor Data** characteristic to receive periodic updates.
3. View the **Sensor Report Interval** characteristic to see the reporting frequency.
4. Write to the **Identify Request** characteristic to trigger an identification request to the Collector.

## Logging

The application uses the TI Logging framework for debugging and runtime information. Logs can be routed to various sinks, such as UART or circular buffers, for analysis.