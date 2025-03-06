# SimpleLink™ Low Power F3 SDK

This repository redistributes TI's [SimpleLink Low Power F3 Software Development
Kit](https://www.ti.com/tool/download/SIMPLELINK-LOWPOWER-F3-SDK) (LPF3 SDK), a
comprehensive software package for developing 2.4 GHz applications. The LPF3 SDK
is one of many within the [SimpleLink Low Power
ecosystem](https://www.ti.com/tool/SIMPLELINK-LOWPOWER-SDK).

The LPF3 SDK delivers components that enable engineers to develop applications
on the Texas Instruments SimpleLink CC23xx and CC27xx family of wireless
microcontrollers (MCUs). This software toolkit provides a cohesive and
consistent software experience for all SimpleLink CC23xx and CC27xx wireless MCU
users by packaging essential software components, such as:

* Bluetooth® Low Energy (BLE) protocol stack
* Zigbee® protocol stack supporting low power wireless mesh networks
* ProprietaryRF 2.4 GHz stack
* TI Drivers

## What's New

This is version 8.40.02.01 of the LPF3 SDK. This release includes:

* Certified Zigbee Stack.
* Zigbee OTA support.
* Minor bug fixes in SCCM.

More details, including supported devices, IDEs, and toolchains are provided in
the [LPF3 8.40.02.01 SDK release notes][sdk release notes].

## About this Repository

Although TI also offers this SDK via [a classic
installer](https://www.ti.com/tool/download/SIMPLELINK-LOWPOWER-F3-SDK), we made
it available as a Git repository to cater to various use cases:

* **Smaller download size**: By omitting documentation (in the **docs/**
  directory) and examples (in the **examples/** directory), the size of the SDK
  can be reduced.
  * The documentation is [available online][sdk docs] in the TI Developers Zone;
    just be sure to browse the appropriate version.
  * Examples are provided in separate repositories, as noted below.
* **CI/CD tooling integration**: Using Git instead of installers can ease
  integration with CI/CD tooling pipelines.
* **Increased visibility and change tracking**: Git improves the ability to
  identify changes that may affect (or not!) your application.
* **Simplified workflow for rebasing**: Git makes it simpler for you to rebase
  your changes to new releases.

## Example Repos

* [Bluetooth Low Energy
  (BLE)](https://github.com/TexasInstruments/simplelink-ble5stack-examples)
* [Proprietary
  RF](https://github.com/TexasInstruments/simplelink-prop_rf-examples)
* [Core SDK, including
  drivers](https://github.com/TexasInstruments/simplelink-coresdk-examples)

## Setup Instructions

### Edit **imports.mak**

At the base of the SDK, you will find **imports.mak**; this file is used by the
build system to find your installed dependencies. Please update **imports.mak**
with full paths to where you have these dependencies installed on your system.

Notably for Windows users, the _Windows_ variant of some tools may be required.
For example, the `SYSCONFIG_TOOL` will need to have a **.bat** extension.

Refer to the comments in **imports.mak** for details on setting variables, and
the [release notes][sdk release notes] for recommended versions. Also see the
[Resources](#resources) section below for download locations.

### Build SDK Libraries

With a few exceptions (libraries without sources), the SDK is provided without
prebuilt libraries. To build the SDK libraries (after editing **imports.mak** as
described above!), navigate to the root of the SDK (the same directory where
**imports.mak** resides) and run GNU `make`.

Assuming it is on your path, you can simply:

```bash
make
```

To clean/rebuild:

```bash
make clean
make
```

At this point, the libraries will be built and installed into the appropriate
locations throughout the SDK.

## Resources

Dependency download locations:

* [SysConfig (SYSCONFIG_TOOL)][sysconfig download]
  * This is also included with CCS
* [TI CLANG Compiler (TICLANG_ARMCOMPILER)][ticlang download]
* [ARM Gnu Toolchain (GCC_ARMCOMPILER)][gcc download]
* [CMake](https://cmake.org/)
* [GNU make](https://www.gnu.org/software/make/)
  * This is also included with CCS in the **ccs/utils/bin/** directory (and
    called `gmake`).

## Technical Support

Please consider creating a post on [TI's E2E forum](https://e2e.ti.com).

[sdk release notes]: https://dev.ti.com/tirex/explore/node?node=A__ADMjnimJ4C5BfFnmM3X-jg__com.ti.SIMPLELINK_LOWPOWER_F3_SDK__58mgN04__8.40.02.01
[sdk docs]: https://dev.ti.com/tirex/explore/node?node=A__AHaph7YfvcrVy2cDlmb4sQ__com.ti.SIMPLELINK_LOWPOWER_F3_SDK__58mgN04__8.40.02.01
[sysconfig download]: https://www.ti.com/tool/SYSCONFIG
[ticlang download]: https://www.ti.com/tool/download/ARM-CGT-CLANG
[gcc download]: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
