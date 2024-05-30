# SimpleLink&trade; CC2340R53 LaunchPad&trade; Settings & Resources

The [__SimpleLink&trade; CC2340R53 LaunchPad&trade;__][launchpad] contains a
[__CC2340R53E0RKPR__][device] device.

## Jumper Settings

* Close the __`LEDs`__ jumper to enable the on-board LEDs.

## SysConfig Board File

The [LP_EM_CC2340R53.syscfg.json](../.meta/LP_EM_CC2340R53.syscfg.json)
is a handcrafted file used by SysConfig. It describes the physical pin-out
and components on the LaunchPad.

## Driver Examples Resources

Examples utilize SysConfig to generate software configurations into
the __ti_drivers_config.c__ and __ti_drivers_config.h__ files. The SysConfig
user interface can be utilized to determine pins and resources used.
Information on pins and resources used is also present in both generated files.

## TI BoosterPacks&trade;

The following BoosterPacks are used with some driver examples.

### [__BOOSTXL-CANFD-LIN SPI to CAN FD + LIN BoosterPack__][boostxl-canfd-lin]

The BoosterPack requires an external voltage supply connected to the barrel jack __`J2`__ in the range of 6 V to 24 V.

The following modifications are required:

* On the BoosterPack, remove jumpers __`J11`__ and __`J12`__ to disconnect the BoosterPack's 3.3 V and 5 V rails if the
  LaunchPad is powered from the debugger.
* On the LaunchPad, bend down header pin BP.33 (__`SWDCK`__) to prevent the signal from interfering with the
  BoosterPack __`nWKRQ`__ signal.
* On the BoosterPack, use a jumper wire to connect header pin BP.31 (__`nINT`__) to pin BP.36 (__`unlabeled`__).

Place the BoosterPack directly atop the LaunchPad.

### [__BOOSTXL-SHARP128 LCD & SD Card BoosterPack__][boostxl-sharp128]

* BoosterPack pin header BP.8 (__`SD_CS`__) must be jumpered to BP.28 when using the SD card.

### [__BP-BASSENSORSMKII BoosterPack__][bp-bassensorsmkii]

* No modifications are needed.

### [__CC3200 Audio BoosterPack__][cc3200audboost]

* No modifications are needed.

[device]: https://www.ti.com/product/CC2340R53
[launchpad]: https://www.ti.com/tool/LP-EM-CC2340R53
[boostxl-canfd-lin]: https://www.ti.com/tool/BOOSTXL-CANFD-LIN
[boostxl-sharp128]: https://www.ti.com/tool/BOOSTXL-SHARP128
[bp-bassensorsmkii]: https://www.ti.com/tool/BP-BASSENSORSMKII
[cc3200audboost]: https://www.ti.com/tool/CC3200AUDBOOST
