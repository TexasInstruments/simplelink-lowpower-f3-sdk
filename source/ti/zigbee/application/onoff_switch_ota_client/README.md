# On/Off Server Switch OTA Client ZED

* [Introduction](#intro)
* [Software Prerequisites](#software-prereqs)
* [Functional Description](#functional-desc)
    * [Software Overview](#software-overview)
        * [Application Files](#application)
* [Configuration With SysConfig](#sysconfig)
* [Example Usage](#usage)
    * [Buttons](#usage-buttons)
    * [Commissioning the Device Into the Network](#Commission-Device)
    * [OTA Client Required Images](#Required-Images)
    * [OTA Client Signing Image for Server](#OTA-Client-Image)

# <a name="intro"></a> Introduction

This document discusses how to use the On/Off Server Switch OTA Client ZED Sample App and the different parts that compose it. On/Off Server Switch OTA Client Sample Application is an
example that exercises different features of TI Zigbee for a Zigbee End Device (ZED)
implementation.

Some of the features exercised include:

- Zigbee End Device commissioning
- Switch device that acts as *Initiator* of Finding and Binding
- On/Off Toggle data frame
- Support for OTA upgrades

# <a name="software-prereqs"></a> Software Prerequisites

- [Code Composer Studio&trade; (CCS)](http://processors.wiki.ti.com/index.php/Download_CCS#Download_the_latest_CCS) v12.7 or newer

- [SimpleLink&trade; LOWPOWER SDK](http://www.ti.com/tool/SIMPLELINK-LOWPOWER-SDK)

# <a name="functional-desc"></a> Functional Description

## <a name="software-overview"></a> Software Overview

This section describes the software components and the corresponding source files.

### <a name="application"></a> Application Files

The On/Off Server Switch OTA Client ZED Sample App works in conjuction with a Zigbee Coordinator (ZC),
which is assumed to have formed a Zigbee network that the On/Off Server Switch OTA Client ZED can
join. Upon startup, On/OffSwitch OTA Client ZED sends a commissioning request to attempt to join the network.
Once it has joined the network successfully, BTN1 on the Launchpad can be pressed to send a Toggle command.

The OTA Client will automatically find a OTA Server in the network and query it for a new image.
If new image is avaiable, it will then reqeust to start the OTA upgrade process.

- **on_off_switch_ota_client.c:** Contains the application's event loop, device initialization
function calls, and commissioning and on/off toggle command specific logic.

- **ota_client_interface.c:** Handles the OTA Client initialization and signals from the stack.

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

Zigbee End Devices can only join networks. At startup, the On/Off Server Switch ZED
Sample App goes into *ZB_BDB_NETWORK_STEERING* mode where it will first send a
*Beacon Request*, wait for beacons, and then send a *Network Commissioning Request*
packet to a ZC/ZR. The ZR/ZC then responds and the commissioning process will continue
until the ZED is joined into the network.

## <a name="Required-Images"></a> OTA Client Required Images
This OTA Client requires two images to be flash to the device:

1. MCUBoot Image
    * Example: `<SDK_INSTALL_DIR>/examples/nortos/<PLATFORM>/mcuboot/mcuboot`
        * After importing to CCS, change the following settings in `mcuboot.syscfg`:
            * **For On-Chip OTA**:
                1. **Upgrade Mode** - Set to Overwrite mode.
                3. **Bootloader Configurations** - Base: `0x00000000`, Size: `0x00004000`
                4. **Image 1 -> Primary Image** - Base Address: `0x00004000`, Image Size: `0x0003D800`
                5. **Image 1 -> Secondary Image** - Base Address: `0x00041800`, Image Size: `0x0003D800`
            * **For Off-Chip OTA**:
                1. **Upgrade Mode** - Set to Overwrite mode.
                2. **Enable External Flash** - Check this box.
                3. **Bootloader Configurations** - Base: `0x00000000`, Size: `0x00004000`
                4. **Image 1 -> Primary Image** - Base Address: `0x00004000`, Image Size: `0x00070000`
                5. **Image 1 -> Secondary Image** - Base Address: `0x00000000`, Image Size: `0x00070000`

2. Signed on_off_switch_ota_client image
    This image is created with the post build steps. To modifying the version, slot size, etc, you can update the post build test for `imgtool`.
    In CCS, right click on the on_off_switch_ota_client example -> Properties -> Build -> Steps.
    **IMPORTANT:** When calling imgtool, the arg --slot-size must match the defined size in the linker file and in the MCUBoot configuration.

## <a name="OTA-Client-Image"></a> OTA Client Signing Image for Server

To test this OTA Client, the CC13XX_26XX family of devices with the <zc/zr>_ota_server example can be used.
Prepare the client image by signing it with <CC13XX_26XX_SDK_INSTALL_DIR>/tools/zstack/zigbee_ota_image_converter/zOTAfileGen.

zOTAfileGen args:
```
zOTAfileGen inputFileName outputDirectory manufactCode imgType fileVer
```
Example call:
```
OTAfileGen <EXAMPLE_BUILD_DIR>\onoff_switch_ota_client_offchip_src_LP_EM_CC2340R5_freertos_ticlang_ota.bin <OUTPUT_DIR> BEBE 2340 00000002
```

The output of this tool can then be provided to the OtaServer PC tool.

For more details, please follow the ota_server readme and the Zstack user guide in the CC13XX_26XX SDK.