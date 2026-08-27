# Texas Instruments Simpleboot Example Quick-start Guide

## Overview

The `simpleboot_app` example is a minimal Secureboot/MCUboot bootloader that skips all cryptographic image verification. Instead of performing the full  image validation pipeline (signature check, hash check, and so on), the bootloader only checks for the MCUboot image magic number (`IMAGE_MAGIC`) in the image header before booting the target application.

This makes it useful in scenarios such as:

- Early-stage development where signing images on every build iteration is not practical.
- Testing flash layout and boot flow before integrating a signing pipeline.
- Environments where security is enforced by another mechanism (for example, customers own secure bootloader).

## Configuration

### Boot Slot Selection

Simpleboot allows the application to choose which image to boot into after a reset by reading `HapiSbGetId()`. This status can be set by application using 
`HapiSbSetId()`

### No Upgrade Logic

Unlike the standard MCUboot configurations (XIP, Overwrite, Swap), this example does **not** implement any image upgrade logic. There is no slot comparison, no image copy, and no swap. The bootloader boots directly from the configured slot.

## Image Slots

The flash layout used by this example is fixed at compile time:
The primary and secondary slot start addresses and sizes are defined in `flash_map_backend.h`.

## How to Build a Compatible Image

Refer to the MCUBoot Blinky example in the SDK 

## Example Usage

At boot, the bootloader performs the following steps:

1. Powers up the GPIO for LED indication.
2. Lights the Red LED to indicate boot is in progress.
3. Reads the image header at the configured slot address and checks for the MCUboot image magic number.
4. If the magic number is valid:
   - Blinks the **Green LED** 3 times.
   - Jumps to the application image.
5. If the magic number is not found:
   - Blinks the **Red LED** in a repeating error pattern.
   - Halts (does not attempt the other slot).

## Supported Devices

This example supports the following device families:

- CC27XX
