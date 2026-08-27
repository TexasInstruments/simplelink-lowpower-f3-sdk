## Example Summary

This example demonstrates the usage of the HSM IP in a BareMetal environment.
The target accepts input over a serial port, conducts a series of RNG, ECDH, ECDSA operations,
and displays the output hash digest as well as the DS signature. The input is limited to
512 bytes. If more than 512 bytes are entered, the application will not accept
additional input and hash and sign the initial 512 bytes received.

## Peripherals & Pin Assignments

When this project is built, the SysConfig tool will generate the TI-Driver
configurations into the __ti_drivers_config.c__ and __ti_drivers_config.h__
files. Information on pins and resources used is present in both generated
files. Additionally, the System Configuration file (\*.syscfg) present in the
project may be opened with SysConfig's graphical user interface to determine
pins and resources used.

## BoosterPacks, Board Resources & Jumper Settings

For board specific jumper settings, resources and BoosterPack modifications,
refer to the __Board.html__ file.

The Board.html can be found in your SDK installation:

        <SDK_INSTALL_DIR>/source/ti/boards/<BOARD>

## Example Usage

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
Homepage"), etc.) to the appropriate COM port.
    * The COM port can be determined via Device Manager in Windows or via
`ls /dev/tty*` in Linux.

The connection should have the following settings
```
    Baud-rate:  115200
    Data bits:       8
    Stop bits:       1
    Parity:       None
    Flow Control: None
```

The console application also needs to be setup to send the carriage-return (CR)
character when 'Enter' is pressed. This is because it is used as the termination
character in this demo. If the demo is not working properly, check the default
configuration of the console application in question and make sure that it is
sending the carriage-return character as expected.

* Run the example.

* Send the string to hash and sign over the serial port. The string must be terminated by
  sending a carriage-return character.

* The target will hash using SHA2-256 and ECDSA sign using SEC_P_256_R1 curve.

* Then, the target will print the digest and the signature.

* Then, the target will verify the generated signature against the digest and print either success or failure.

To confirm the example is behaving properly, the expected digest for the input
string "This is a demo string." is
`FAE812FBA876DA7D4BC07C45485C27DBEA11D0627816C049FFF78CDD48FAA545`.

An example ECDSA signature of the above using SEC_P_256_R1 curve is
`9467EF4BBB6E660609D8B4475D76486973F439E7C87EA45E5D2BB0520534C97DBC065E9F085C4C8396474E3D197762D930A0AB27B3567EA8103DF9DA99EAC67C`

## Application Design Details

This example is intended for CC27XX devices only.

CC27XX devices leverage a standalone HSM hardware accelerator and this application
leverages the HSM BareMetal SW architecture and interacts directly with the HSM
bypassing the existing SW architecture which is driver-dependent.

The central aspect of HSMBareMetal design stems from the structure of the HSM FW tokens.
We combined all modes of operations under 5 main categories. For each main category,
there exists a single API interface.

