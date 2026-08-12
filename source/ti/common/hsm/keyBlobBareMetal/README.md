## Example Summary

This example demonstrates the usage of the HSM IP in a BareMetal environment.
The target accepts input over a serial port, conducts a series of RNG, key-related,
and AES-CCM operations. It displays the asset IDs obtained from the HSM and the key
blobs associated with each asset. The target also displays the results of a CCM
encryption and decryption operations The input is limited to 512 bytes.
If more than 512 bytes are entered, the application will not accept additional
input and hash and sign the initial 512 bytes received.

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

* Choose between an independent RNG operation or In-API (Asset API) key generation.

* Then, the target will perform an asset operation to load plaintext, generate an asset ID pair, and generate a key blob pair.

* Then, the target will free the asset ID pair and import the key blob pair back into the HSM and generate a new Asset ID pair.

* Then, the target will read input from the user.

* Then, the target will use the AES-CCM algorithm to encrypt and decrypt the user-provided input.

## Application Design Details

This example is intended for CC27XX devices only.

CC27XX devices leverage a standalone HSM hardware accelerator and this application
leverages the HSM BareMetal SW architecture and interacts directly with the HSM
bypassing the existing SW architecture which is driver-dependent.

The central aspect of HSMBareMetal design stems from the structure of the HSM FW tokens.
We combined all modes of operations under 6 main categories. For each main category,
there exists a single API interface.

