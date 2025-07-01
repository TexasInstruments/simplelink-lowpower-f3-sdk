# On/Off Server Switch OTA Client ZED

* [Introduction](#intro)
* [Software Prerequisites](#software-prereqs)
* [Functional Description](#functional-desc)
    * [Software Overview](#software-overview)
        * [Application Files](#application)
* [Configuration With SysConfig](#sysconfig)
    * [OTA Client Image Slot Configuration](#Ota-Client-Slot-Config)
* [Example Usage](#usage)
    * [Buttons](#usage-buttons)
    * [Commissioning the Device Into the Network](#Commission-Device)
    * [OTA Client Required Images](#Required-Images)
    * [Flashing Client Image](#Flashing-Client-Image)
        * [Flashing with Uniflash](#Flash-Uniflash)
        * [Flashing with CCS](#Flash-CCS)
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

## <a name="Ota-Client-Slot-Config"></a> OTA Client Image Slot Configuration

**The MCUBoot settings must match in both MCUBoot example and OTA Client Example.**

### OTA Client Example
#### OTA Client example -> on_off_switch_ota*.syscfg -> **MCUBoot App**

* The default settings here are to be used as referrences to set MCUBoot seetings.
* When updating the image slots, you must also update the  on_off_switch_ota*.syscfg -> **TI DRIVERS** -> **STORAGE INTERFACES** -> **NVS**:

    * **CONFIG_NVSINTERNAL1** (on-chip)
    * **CONFIG_NVSEXTERNAL** (off-chip)

        *The region base/size will need to match the secondary image slot base/size.*

* If using OTA compression, the secondary image slot can be up to **27%** smaller than primary image slot.
* **NOTE**: When toggling the "enable compression" setting, the slot settings will be changed to high level defaults, which will need to be modified for app function.

### MCUBoot example
#### MCUBoot example -> mcuboot.syscfg -> **MCUBoot**
##### MCUBoot Settings:
* **Upgrade Mode** - Set to Overwrite mode.
* **Enable Image Compression** - Check this box when using Compression.
* **Enable External Flash** - Check this box when using External Flash
* **Bootloader Configurations** -
    * Base: `0x00000000`
    * Size: `MCUboot App -> Base Address`
* **Image 1 -> Primary Image** -
    * Base Address: `MCUboot App -> Base Address`
    * Image Size: `MCUboot App -> Slot Size`
* **Image 1 -> Secondary Image** -
    * Base Address: `(MCUboot App -> Base Address) + (MCUboot App -> Slot Size)`
    * Image Size:
        * **If using Compression**: `MCUboot App -> Compressed Slot Size`
        * **If NOT using compression**: `MCUboot App -> Slot Size`


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

## <a name="Ota-Client-Behavior"></a> OTA Client Behavior
15s after joining the network, the OTA Client will start its OTA process. It first sends a Match Descriptor Request. If an OTA Server is found, 60s later the OTA Cient will start the upgrade process by sending a Query next Image Request.

## <a name="Required-Images"></a> OTA Client Required Images
This OTA Client requires two images to be flash to the device:

1. MCUBoot Image
    * Example: `<SDK_INSTALL_DIR>/examples/nortos/<PLATFORM>/mcuboot/mcuboot`

2. Signed on_off_switch_ota_client image binary. This will be writen into image slot 1 in flash.
    * This image is created with the post build steps. Look for the file "Project name"_ota_1st_slot.bin.This image will always be uncompressed.

    * The "Project name"_ota_2nd_slot.bin is meant for the secondary slot and may be compressed if enabled. This image is meant to be signed with a Zigbee OTA header and used in the OTA upgrade.

**NOTE:**
Both ota_\*_slot.bin images will have the same application version. To update the application version, there are two location:

1. OTA Client example -> on_off_switch_ota*.syscfg -> **Image BootLoarders** -> **MCUBoot Application** -> **Version**
2. on_off_switch_ota_client.c --> **g_dev_ctx.ota_attr.file_version**

## <a name="Flashing-Client-Image"></a> Flashing Client Image
#### <a name="Flash-Uniflash"></a> Flashing with Uniflash
1. Connect device
2. Under Program, first select MCUBoot flash image path.
3. Then click + to add \<Project name\>_ota_v1.bin path.
4. Enter Load Addr. This will be the base address of the Primary Image slot.
5. Click Load Images
#### <a name="Flash-CCS"></a> Flashing with CCS
1. With MCUBoot project highlighted, click debug on toolbar (or navigate to Run -> Debug). This will flash MCUBoot.
2. Once the Deubugger is connected, navigate to Tools -> On-Chip Flash. In the On-Chip Flash window, select Do not erase.
*NOTE: deselect Do not erase to flash another image (mcuboot or other).*
3. Now we need to load Image 1 into its slot.
    1. Navigate to View -> Memory Browser.
    2. In the Memory Browser window, next to the Save Memory icon, click the down arrow and select Load Memory.
    3. Set File Type to Binary and file to the path of \<Project name\>_ota_v1.bin. Then click next.
    4. Set Start Address to the base address of the Primary Image slot. Then click finished.


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