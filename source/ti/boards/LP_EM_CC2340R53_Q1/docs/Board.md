# SimpleLink&trade; CC2340R53-Q1 LaunchPad&trade; Settings & Resources

The __SimpleLink&trade; CC2340R53-Q1 LaunchPad&trade;__ contains a [__CC2340R53E0WRHBRQ1__][device] device.

## Jumper Settings

* Close the __`LEDs`__ jumpers to enable the on-board LEDs.
* Close the __`SCL`__ jumper to enable I2C SCL or the __`BTN1`__ jumper to enable Button 1.
  These two jumper settings are mutually exclusive.
* Close the __`SDA`__ jumper to enable I2C SDA or the __`BTN2`__ jumper to enable Button 2.
  These two jumper settings are mutually exclusive.

## SysConfig Board File

The [LP_EM_CC2340R53_Q1.syscfg.json](../.meta/LP_EM_CC2340R53_Q1.syscfg.json)
is a handcrafted file used by SysConfig. It describes the physical pin-out
and components on the LaunchPad.

## Driver Examples Resources

Examples utilize SysConfig to generate software configurations into
the __ti_drivers_config.c__ and __ti_drivers_config.h__ files. The SysConfig
user interface can be utilized to determine pins and resources used.
Information on pins and resources used is also present in both generated files.

## TI BoosterPacks&trade;

The following BoosterPacks are used with some driver examples.

### [__BOOSTXL-SHARP128 LCD & SD Card BoosterPack__][boostxl-sharp128]

* BoosterPack pin header BP.8 (__`SD_CS`__) must be jumpered to BP.27 when using the SD card.

### [__BP-BASSENSORSMKII BoosterPack__][bp-bassensorsmkii]

The TMP116's power pin is not compatible with this LaunchPad. Use the
following modification to enable the TMP116's usage with the
__i2ctmp__ and __portable__ examples.

* Use a jumper wire to connect BoosterPack pin header BP.28 (__`T_V+`__) to BP.1 (__`3V3`__).

The OPT3001's interrupt pin is not compatible with this LaunchPad. Use the
following modification to enable the OPT3001's usage with the
__i2copt3001_cpp__ example.

* Use a jumper wire to connect BoosterPack pin header BP.25 (__`O_INT`__) to BP.7 (unlabeled).

### [__CC3200 Audio BoosterPack__][cc3200audboost]

* No modifications are needed.

### __BP-EM-CS BoosterPack__

When using a CC23X0 board, connect the P3 and P4 jumpers, and disconnect the P1
and P2 jumpers on the BoosterPack. Add only the VC pins labelled CC23X0 in
SysConfig and make sure they are configured as output.

The VC pins are used to select the antenna on the BoosterPack:

|     | RF1 (J2) | RF2 (A1) | RF3 (A2) | RF4 (J3) |
|-----|----------|----------|----------|----------|
| VC1 | 0        | 0        | 1        | 1        |
| VC2 | 0        | 1        | 0        | 1        |

[device]: https://www.ti.com/product/CC2340R53-Q1
[boostxl-sharp128]: https://www.ti.com/tool/BOOSTXL-SHARP128
[bp-bassensorsmkii]: https://www.ti.com/tool/BP-BASSENSORSMKII
[cc3200audboost]: https://www.ti.com/tool/CC3200AUDBOOST
