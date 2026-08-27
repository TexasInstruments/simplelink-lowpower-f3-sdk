## Example Summary

Application that toggles RED and GREEN LEDs.

## Peripherals & Pin Assignments

When this project is built, the SysConfig tool will generate the TI-Driver
configurations into the __ti_drivers_config.c__ and __ti_drivers_config.h__
files. Information on pins and resources used is present in both generated
files. Additionally, the System Configuration file (\*.syscfg) present in the
project may be opened with SysConfig's graphical user interface to determine
pins and resources used.

* `CONFIG_GPIO_LED_0` - Indicates that the board was initialized within
`mainThread()` then toggles every 500ms
* `CONFIG_GPIO_LED_1` - toggles every 500ms

## BoosterPacks, Board Resources & Jumper Settings

For board specific jumper settings, resources and BoosterPack modifications,
refer to the __Board.html__ file.

> If you're using an IDE such as Code Composer Studio (CCS) or IAR, please
refer to Board.html in your project directory for resources used and
board-specific jumper settings.

The Board.html can also be found in your SDK installation:

        <SDK_INSTALL_DIR>/source/ti/boards/<BOARD>

## Example Usage

* Run the example. `CONFIG_GPIO_LED_0` turns ON to indicate driver
initialization is complete.

* `CONFIG_GPIO_LED_0` is toggled every 500ms.
* `CONFIG_GPIO_LED_1` is toggled every 500ms.

## HW Antirollback Protection

MCUboot has a hardware-based downgrade prevention feature by using a security counter that is stored in each image's protected TLV area. If MCUboot is built with `Anti Rollback Protection` option selected in SysConfig, in the project post-build steps for the target application, add option `-s <val>`, where `<val>` specifies the value of the security counter for the image. In this scenario, besides comparing 
the version stored in the header of each image, MCUboot will compare the value of the image security counter against the current security counter stored in the last sector of the MCUboot region, and accept the new 
image if its security counter has the same or higher value.</br>

**Note**: This feature is not supported in CC13x2x7/CC26x2x7 devices.

## Encrypted Images

MCUboot allows upgradable images to be encrypted and can decrypt images on-the-fly while upgrading. If MCUboot is built with `Upgrade using Encrypted Images` option selected in SysConfig, in the project post-build steps for the target application, add option `--encrypted <key>`, where `<key>` is the public key to encrypt the image. </br>

**Note**: This feature is not supported in CC23xx or CC27xx devices.

## Image Compression

MCUboot allows for compressed update images. If MCUboot is built with `Enable Image Compression` option in sysconfig (or add these macros - `MCUBOOT_DECOMPRESS_IMAGES` to mcuboot_config.h) , in the project post-build steps for the target application, add option --compression-props <lp>,<lc>,<pb> --compression-dict-size <dict_size>, where <lp>,<lc>,<pb> are values defaulted to 0 and <dict_size> is the dictionary size defaulted to 12288.

**Note**: This feature is only supported for CC23XX devices.

## TZ Enabled build configuration

When Enable TrustZone is enabled in sysconfig, make sure to **also** flash file `mcuboot_blinky_*.2.bin` to the primary 2 slot. This is needed because MCUboot will verify primary slots 1 and 2 as a requirement to boot primary 1. `mcuboot_blinky_*.2.bin` is an identical copy of the primary 1 binary, but processed for a slot length of 0x4e800, which is the slot length configured for primary 2 and secondary 2 slots.

**Note**: This feature is only supported for CC13X4/CC26x4/CC26x3 and CC27XX devices.

## Generate Keys for MCUboot

1. Create a Public Key to sign image
```
# Generate Key Pair
python imgtool.py keygen -k <key_pair> -t <algo>

```
With the Public Key, add the option `--key <key>` invoking imgtool.

2. Create Public and Private Keys to encrypt image

```
# Generate Key Pair
python imgtool.py keygen -k <key_pair> -t <algo>

# Extract Private Key and display it
# Note that the Private Key array and size must be copied into keys.c in MCUboot
python imgtool.py getpriv --minimal -k <key_pair>

# Generate Public Key and store to file
python imgtool.py getpub -k <key_pair> --encoding <encoding> > <public_key>

```
The private key is outputted to the terminal as C byte array along with the the key size. Those should be copied to 'keys.c' in your MCUboot project. With the public key, add the option `--encrypted <key>` when invoking image tool.

