## How to make a CC27XX Secure Boot compatible image

The following are steps required to make any application compatible with CC27XX ROM Secure Boot.

## Key Generation

Run `imgtool` (found in `<SDK>/tools/common/mcuboot`) to generate 2 private key pairs. Optionally, if a Secondary Secure Boot loader is required, generate a 3rd key pair. Note that only RSA-3K-PKCS, ECDSA P256 and ECDSA P521 are supported.

For instance, to generate 3 key pairs, run the following commands:

`imgtool keygen --key app-rsa-3k-pkcs-priv.pem --type rsa-3072`<br />
`imgtool keygen --key ssb-rsa-3k-pkcs-priv.pem --type rsa-3072`<br />
`imgtool keygen --key kui-rsa-3k-pkcs-priv.pem --type rsa-3072`<br />

Store these files in a safe location, as these are the keys to be used when signing new images. `kui-rsa-3k-pkcs-priv.pem` will be used hereafter to sign any new key update images, either of APP or SSB type. 
Copy file `app-rsa-3k-pkcs-priv.pem` to the root directory of the target APP project. Optionally, copy `ssb-rsa-3k-pkcs-priv.pem` to the root directory of the SSB project. 

If desired, this step can be skipped and instead use the provided test keys as mentioned in section **Test Keys**.

## SCFG Key Ring Initialization

Once the key pairs have been generated, run `sbtool` (found in `<SDK>/tools/common/sbtool`) to generate the scfg key ring configuration:

`sbtool init --key-update-key kui-rsa-3k-pkcs-priv.pem --app-key app-rsa-3k-pkcs-priv.pem --ssb-key ssb-rsa-3k-pkcs-priv.pem `<br />


From directory `output`, copy file `sb_key_ring_init.h` to the root directory of the target project. This file will be required if the Authentication Method in SysConfig is other than `No Authentication`.

If desired, this step can be skipped and instead use the provided `sb_key_ring_init.h` as mentioned in section **Test Keys**.

## Test Keys

Test Keys along with the corresponding `sb_key_ring_init.h` are stored under `<SDK>/tools/common/sbtool/keys`. Note that there are separate directories for each of the algorithms supported by Secure Boot, so pick either the APP or SSB key (depending on what type of target is being built) and `sb_key_ring_init.h` from the directory that matches the algorithm that has been configured in SysConfig.

## Secure Boot Configuration (SysConfig)

Open the project's `*.syscfg` file. The Secure Boot configuration is found under TI DEVICES -> Device Configuration -> Security Configuration.

* Select the same **Authentication Algorithm** as the one used when generating the keys.
* Configure **Boot Seed** if needed. 
* Configure the **Primary/Secondary** and, optionally, **SSB** slots start address and length.
* Select the proper **Authentication Method** (Signature or Hash Lock).
* Select the **Update Mode** (Overwrite / XIP Revert Enabled / XIP Revert Disabled).
* Select the **Image Type** that the current target being build (APP 0, APP 1, or SSB).
* If **Update Mode** is any of the XIP variants, configure the **Target Slot** for which the current target must be linked for. Remember than in XIP mode, the image will be executed in the same slot space in which it has been written to.
* Enter the desired **Security Counter** to be associated with this target image.
* Enter the desired **Header Size**. Minimum is 0x80, and must be a multiple of 4.
* Enter the desired **Version** for the target image.

## Post-Build steps

Refer to the Secure Boot Example post-build steps. These need to be applied to your project's build steps to generate a Secure Boot compatible image.


* Build the project.
* Flash the `*_sb.hex` file. This will program CCFG, SCFG, and the target application. This needs to be done only when the very first image is being programmed. For subsequent update images, only the `*_sb.bin` file needs to be flashed to the corresponding slot, as it does not include the CCFG/SCFG configuration because it has already been written when the device was initially programmed with the first image `*_sb.hex`.
* If debugging is desired, go to Run -> Load -> Add Symbols and select the `.out` file. This is required for the debugger to resolve symbols.

## Key Update Procedure

Note that under the `output` directory, there are 2 files `app_kui.bin` and `ssb_kui.bin`. These are the generated Key Update Images that will be used when new keys are to be added to the system. 
* Run the steps as described in section `Key Generation`, and write the desired Key Update image to the desired stage address (specified by 3v3 sector flags), and trigger an update procedure.
* Generate a target signed with then new key pair (either APP or SSB), and flash the resulting `${ProjName}_sb.bin` to the corresponding slot.

