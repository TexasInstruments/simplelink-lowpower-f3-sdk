# Texas Instruments MCUBOOT Example Quick-start Guide

## Disclaimer
MCUboot is a open source bootloader. This example is the result of Texas Instruments forking the original [MCUboot repo](https://github.com/mcu-tools/mcuboot) and providing support for [CC13XX_CC26XX and CC23XX_CC27XX](https://www.ti.com/wireless-connectivity/overview.html) class devices.
The following information is for reference only. In a production environment, the end user is must to do the proper modifications that best serve the needs of the particular application. 

## Configuration
This release supports three configurations. Change the current configuration within SysConfig or add the define macros to mcuboot_config.h.

* XIP (`MCUBOOT_DIRECT_XIP`) (default):

In this configuration MCUboot runs an image directly from either the primary or the secondary slot (without moving and or copying it into the primary slot). In other words, MCUboot e**X**ecutes **I**n **P**lace (**XIP**).
The image update client, which downloads the new images must be aware which slot contains the active image and which acts as a staging area.
In addition, the image update client is responsible for loading the proper images into the proper slot. Because MCUboot runs the image directly from the
slot it resides in, the image must be compiled/linked to execute within the slot it will be programmed to.
</br></br>
At boot time the MCUboot first looks for images in both slots and then inspects the version numbers in the image headers.
It selects the newest image (with the highest version number) and then checks its validity (integrity check, signature verification etc.). If the image 
is invalid, MCUboot erases its memory slot and starts to validate the other image. After a successful validation of the selected image the MCUboot executes it.
This mode is only allowed when 'Enable TrustZone' is not selected in sysconfig (or `MCUBOOT_IMAGE_NUMBER = 1` is defined in mcuboot_config.h).

* Overwrite (`MCUBOOT_OVERWRITE_ONLY`):

In this configuration MCUboot only runs an image from the primary slot. If during boot time the MCUboot identifies the secondary slot to contain the image it needs to execute, MCUboot will first copy the image from the secondary to the primary slot, as long as the image in secondary slot has a version higher or equal to the one in primary. Afterward it will erase the secondary slot. This mode is supported if 'Enable TrustZone' is enabled or disabled in sysconfig (or `MCUBOOT_IMAGE_NUMBER = 1` or `MCUBOOT_IMAGE_NUMBER = 2` is defined in mcuboot_config.h). See [Image Slots](#image-slots) section for further explaination.

* Swap (using Move) (`MCUBOOT_SWAP_USING_MOVE`):

In this configuration MCUboot only runs an image from the primary slot, similar to overwrite. If during boot time the MCUboot identifies the secondary slot to contain the image it needs to execute, MCUboot will first **swap** the images from the secondary and primary slots, as long as the image in secondary slot has a version higher or equal to the one in primary. After a swap has occured correctly, MCUboot allows for 2 follow up operations - commit or revert. To commit an image, the application image must set the Image OK Flag in the trailer to 1. After a reset, MCUboot will see that the primary image has bene committed and will only boot into the update image, until there is a new update image. To perform a revert, simply reset the device again and MCUboot will only boot into the original image until a new update image is presented.

## HW Antirollback Protection

MCUboot has a hardware-based downgrade prevention feature by using a security counter that is stored in each image's protected TLV area. If MCUboot is built with 'Anti Rollback Protection' option enabled in sysconfig (or `MCUBOOT_HW_ROLLBACK_PROT` configuration enabled in mcuboot_config.h), in the project post-build steps for the target application, add option `-s <val>`, where `<val>` specifies the value of the security counter for the image. In this scenario, besides comparing
the version stored in the header of each image, MCUboot will compare the value of the image security counter against the current security counter stored in the last sector of the MCUboot region, and accept the new
image if its security counter has the same or higher value.</br>

**Note**: This feature is not supported in CC13x2x7/CC26x2x7 devices.

## ECIES

MCUboot allows for encrypted update images. If MCUboot is built with 'Upgrade Using Encrypted Images' option in sysconfig (or add these macros - `ECDH_BIG_ENDIAN_KEY`, `MCUBOOT_ENC_IMAGES`, `MCUBOOT_ENCRYPT_EC256` to mcuboot_config.h) , in the project post-build steps for the target application, add option --encrypted <key>, where <key> is the public key to encrypt the image.

**Note**: This feature is only supported for CC13x2x7/CC26x2x7 and CC13X4/CC26x4/CC26x3 devices.

## Image Compression

MCUboot allows for compressed update images. If MCUboot is built with 'Enable Image Compression' option in sysconfig (or add these macros - `MCUBOOT_DECOMPRESS_IMAGES` to mcuboot_config.h) , in the project post-build steps for the target application, add option --compression-props <lp>,<lc>,<pb> --compression-dict-size <dict_size>, where <lp>,<lc>,<pb> are values defaulted to 0 and <dict_size> is the dictionary size defaulted to 12288.

**Note**: This feature is only supported for CC23XX devices.

## Image Slots
The flash memory is partitioned into two image slots: a primary slot and a secondary slot. Each slot must have a fixed location and associated length, which is set up in SysConfig.
Default values are provided, but user is free to modify the partitions defined for a particular device if needed. Note that the address of a partition is required to be aligned to a sector boundary, and its length must be multiple
of sector size. 

There is also an option in sysconfig 'Enable TrustZone' (or configure `MCUBOOT_IMAGE_NUMBER = 2` in mcuboot_config.h) that enables dual primary and secondary slots; that is, 2 primary slots, and 2 secondary slots. This is required for applications that make use of secure features.
In such case, primary 1 is for the secure application, and primary 2 for non secure. If an upgrade is to occur, then the secondary 1 image will overwrite primary 1, and secondary 2 will overwrite
primary 2. Then, if both primary 1 and 2 are successfully verified, MCUboot will proceed to boot primary 1. 


## How to build an image that is compatible with MCUboot

Refer to example `mcuboot_blinky` for different linker definitions depending on the current device and toolchain in use. 

* Linker variable `FLASH_BASE` controls the address for which the application will be linked for. `MCUBOOT_HDR_BASE` defines the start address of the MCUboot image header.
In the examples provided in this SDK, a length of 0x80 is used for the MCUboot header. 

* If application needs to access the contents of the MCUboot header, add the following code as shown below. Note that symbol `MCUBOOT_HDR_BASE` has been defined in the linker file, and is accessed by the C source code to initialize 
the address of variable `mcubootHdr` so that it can be used to access the current application's MCUboot header. 
</br>

```
 #include "bootutil/bootutil.h"
 #include "bootutil/image.h"

extern int MCUBOOT_HDR_BASE;
struct image_header *mcubootHdr = (struct image_header *)&MCUBOOT_HDR_BASE;
```
* Add post processing in CCS

    * Extract the binary image `<proj_name>_noheader.bin` from ELF file `<proj_name>.out`.       
    * Process `<proj_name>_noheader.bin` with imgtool (`${COM_TI_SIMPLELINK_SDK_INSTALL_DIR}/tools/common/mcuboot/imgtool.exe`) to get `<proj_name>.bin`, which is the compatible MCUboot image format.

See example below:
</br></br>NOTE: The key root-ec-p256.pem is for example use only. Provide your own key for your application.
	
```
 ${CG_TOOL_ROOT}/bin/arm-none-eabi-objcopy ${ProjName}.out -O binary ${ProjName}-noheader.bin
 ${COM_TI_SIMPLELINK_SDK_INSTALL_DIR}/tools/common/mcuboot/imgtool sign --header-size 0x80 --align 4 --slot-size 0x2A000 --version 2.0.0 --pad-header --key ${COM_TI_SIMPLELINK_SDK_INSTALL_DIR}/source/third_party/mcuboot/root-ec-p256.pem ${ProjName}-noheader.bin ${ProjName}.bin
```

Refer to example `mcuboot_blinky` for different post-build steps depending on the current device and toolchain in use. Refer to the example on how to generate keys for signing and encrypting images.

See [MCUboot Project imgtool](https://github.com/mcu-tools/mcuboot/blob/main/docs/imgtool.md) description for further reference on how to create an image compatible with MCUboot.

## MCUboot Image Upgrade in OVERWRITE mode for devices that support TZ_Enabled and TZ_Disabled configurations

For 1 upgradeable image, MCUboot will overwrite the Primary slot image with that one from Secondary slot if:

* Secondary image is valid and Primary image is invalid.
* Secondary image is valid and its version is higher or equal than that in Primary.

MCUboot will attempt to boot Primary after performing an update, if any. 

**NOTE**: When using the **TZ Disabled** build configuration, make sure to set **Address of Flash Vector Table** to 0x00000000, under SysConfig **Device Configuration**.


For 2 upgradeable images:

The same procedure as for 1 upgradeable image described above is followed, but in this case Primary 0 is compared against Secondary 0, and Primary 1 will be compared against Secondary 1.

MCUboot will attempt to boot Primary 0 after performing an update, if any.

**NOTE 1**: When using the **TZ Enabled** build configuration, make sure to set **Address of Flash Vector Table** to 0x00000800, under SysConfig **Device Configuration**.</br>
**NOTE 2**: The SDK includes the **Secure Image** ELF file (tfm_s.axf) and a **Non-Secure example** project called tfm_aescbc, which can be imported into CCS, so that both secure and non-secure images be used in MCUboot 2-image upgrade mode. </br>
**NOTE 3**: HW Antirollback Protection is not supported in this mode.</br></br>

Per section **How to build an image that is compatible with MCUBOOT** above, find file `${COM_TI_SIMPLELINK_SDK_INSTALL_DIR}/tfm_s/build/cc26x4/production_full/tfm_s.axf` and run the following steps:<br><br>

```
${CG_TOOL_ROOT}/bin/arm-none-eabi-objcopy -O binary tfm_s.axf tfm_s-noheader.bin
${COM_TI_SIMPLELINK_SDK_INSTALL_DIR}/tools/common/mcuboot/imgtool.exe sign --header-size 0x100 --align 4 --slot-size 0x2b000 --version 1.0.0 --pad-header --pad --key ${COM_TI_SIMPLELINK_SDK_INSTALL_DIR}/source/third_party/mcuboot/root-ec-p256.pem tfm_s-noheader.bin tfm_s.bin

${CG_TOOL_ROOT}/bin/arm-none-eabi-objcopy tfm_aescbc.out -O binary tfm_aescbc-noheader.bin --remove-section=.ccfg
${COM_TI_SIMPLELINK_SDK_INSTALL_DIR}/tools/common/mcuboot/imgtool.exe sign --header-size 0x100 --align 4 --slot-size 0x4e800 --version 1.0.0 --pad-header --pad --key ${COM_TI_SIMPLELINK_SDK_INSTALL_DIR}/source/third_party/mcuboot/root-ec-p256.pem tfm_aescbc-noheader.bin tfm_aescbc.bin
```
The resulting files tfm_s.bin and tfm_aescbc.bin can now be flashed in slots Primary 1 and 2, accordingly.

## Example Usage

This project is an example implementation of MCUboot while `mcuboot_blinky` is an example implementation of an application to be programmed and run by MCUboot. If MCUboot finds a valid image to boot, it will blink the Green LED 3 times and the proceed to boot the target image. Otherwise, it will blink the Red LED repeatedly.

Refer to SDK example `mcuboot_blinky` for additional details.

To enable MCUboot for LP_EM_CC2340r53-YBG, the user must define `BOARD_YBG`.

To enable MCUboot for CC27xx devices, the user must have already provisioned the HSM.

### Enable as a Secondary Secure Bootloader

MCUboot can also be enabled as a secondary secure bootloader to Secure Boot. 

Open the project's `*.syscfg` file. The Secure Boot configuration is found under TI DEVICES -> Device Configuration -> Security Configuration.
If **Secondary Secure Bootloader Enabled** is checked in sysconfig and **SSB** slots are defined, Secure Boot can verify MCUboot and once verification is complete, transfer control to it. From there, MCUboot can verify the images in each slot. The boot loader base and sizes are determined by Secure Boot, meaning those options in MCUboot SysConfig will be overwritten by Secure Boot. In this example MCUboot still has control over the slot definitions.

Note that MCUboot does not support the same authentication algorithms that Secure Boot supports. This means that an application signed by imgtool will not work for Secure Boot. Likewise, an application signed by sbtool will not work for MCUboot.

Refer to SDK example `secure_boot` for additional details on how to enable Secure Boot and sign Secure Boot images.

