# SIMPLELINK LOW POWER F3 SDK
This repository contains TI's SimpleLink Low Power F3 Software Development Kit (SDK).

## Summary

The SimpleLink™ Low Power F3 Software Development Kit provides a comprehensive software package for developing 2.4 GHz applications, including support for Bluetooth® Low and proprietary RF solutions on the SimpleLink CC23xx wireless MCUs.

An overview of the full SDK is hosted on TI Resource Explorer.

## Background
This repository contains a subset of TI's [SimpleLink Low Power F3 SDK](https://www.ti.com/tool/SIMPLELINK-LOWPOWER-SDK).  It is provided via a Git repo to meet several use cases, including:

* Reduces download size
* Enables CI/CD tooling to use Git rather than installers
* Increases visibility of changes, better enabling benefit/risk assessments
* Simplifies the workflow for rebasing user changes to different releases

It intentionally omits documentation (the **docs/** directory) and examples (**examples/**) to reduce size.  Regarding those omissions:

* [Documentation is available online](https://dev.ti.com/tirex/explore/node?node=A__AJE8rC9SSzb3kRUd2VMM9w__com.ti.SIMPLELINK_LOWPOWER_F3_SDK__58mgN04__LATEST&placeholder=true).  Please match your git version to the documentation version in the TI Developer Zone.
* Examples are available in separate repositories, as noted in [Examples](#example-repos).

## Features


- Industry leading Bluetooth Low Energy (BLE) software protocol stack supporting Bluetooth 5.3 (learn more [TI.com/ble](https://www.ti.com/ble))
- Support for Proprietary 2.4 GHz applications supporting basic RX and TX examples


## Example Repos

- [Bluetooth Low Energy (BLE)](https://github.com/TexasInstruments/simplelink-ble5stack-examples)
- [Proprietary RF](https://github.com/TexasInstruments/simplelink-prop_rf-examples)


## Setup Instructions

### Edit **imports.mak**
The imports.mak file, located at the root of the SDK, is a generic sample and will not work out of the box.   Please update imports.mak with the tools (compilers, cmake, etc.) paths installed on your system.

For a Linux build, settings must be updated to match your build system's setup.  The only outlier may be Python, as most python3.6+ interpreters will work.  Please note cmake must be 3.21+, or the builds will fail.  If using CCS ccs1220, the SysConfig installed is incompatible with the SDKs.  Either upgrade to the latest CCS or install SysConfig 1.17.0 from [https://www.ti.com/tool/SYSCONFIG](https://www.ti.com/tool/SYSCONFIG).  See [Resources](#resources) for URL's of tools that need installation to build the SDKs and examples.

By default TICLANG and GCC toolchains are enabled.  If a toolchain is not needed, unset the compiler, for example, `GCC_ARMCOMPILER ?=`.

### Default imports.mak

`SYSCONFIG_TOOL`         ?= /home/username/ti/CCS_Install/ccs/utils/sysconfig_1.17.0/sysconfig_cli.sh

`FREERTOS_INSTALL_DIR`   ?= /home/username/FreeRTOSv202104.00

`CMAKE`                  ?= /home/username/cmake-3.21.3/bin/cmake

`PYTHON`                 ?= python3

`TICLANG_ARMCOMPILER`    ?= /home/username/ti/CCS_Install/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS-0

`GCC_ARMCOMPILER`        ?= /home/username/ti/CCS_Install/ccs/tools/compiler/9.2019.q4.major-0

Edit **imports.mak** and update all dependencies to reflect your paths.

Notably for Windows users, the _Windows_ variant of some tools may be required.
For example, the `SYSCONFIG_TOOL` will need to have a **.bat** extension.

## Build SDK Libraries
SDK libraries must exist before building any examples.  After editing imports.mak, build SDK libraries from the root  of the SDK (the same directory where imports.mak resides) by:

$ `make`

The make will go through the SDK and build all libraries.  Depending on the build machine, the make will run for a few minutes.


## Resources

Dependency download locations:

* [SysConfig (SYSCONFIG_TOOL)](https://www.ti.com/tool/SYSCONFIG), or if using ccs1330+ it is part of CCS
* [FreeRTOS (FREERTOS_INSTALL_DIR)](https://github.com/FreeRTOS/FreeRTOS/releases/download/202104.00/FreeRTOSv202104.00.zip)
* [TI CLANG Compiler (TICLANG_ARMCOMPILER)](https://www.ti.com/tool/download/ARM-CGT-CLANG)
* [ARM Gnu Toolchain (GCC_ARMCOMPILER)](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads/9-2019-q4-major)

## Supported Devices

All devices supported by the SimpleLink Low Power F3 SDK.

## More Information
For licensing information, please refer to licensing.

## Technical Support
Please consider creating a post on [TI's E2E forum](https://e2e.ti.com). Additionally, we welcome any feedback.
