# Hardware Security Module (HSM) FW

This folder contains the HSM FW for each device which requires it. The HSM FW
must be provisioned, at least once, to any device which will make use of the HSM
IP. This, for example, is true for most all crypto driver functionality.

If an attempt is made to use such crypto driver functionality without the HSM
FW already provisioned then the driver will return with a error indicating that
the HSM FW could not boot.

# Security of the HSM FW

The HSM FW and the topic of the HSM in general requires certain security
measures to be taken. This includes but is not limited to:

- RSA3K signature verification during every boot of the HSM FW
- RSA3K signature verification during HSM FW updates
    - These signatures not only confirm integrity of the HSM FW but also that
    the HSM FW has been genuinely signed by TI.
- AES decryption during HSM FW updates
- HW based read, write and erase protection of the HSM FW region of MAIN FLASH
    - This prevents the application as well as the debugger from ever having
    access to the HSM FW

## Can the HSM FW be erased?

Although the HSM FW is write/erase protected from the application. It is still
possible to erase the HSM FW via back-to-back Chip Erases. This is unlikely, but
possible to be done accidentally due to various programming errors. If this
should happen, the HSM FW need only be provisioned once again.

## Restricting updates based on a customer signature

As described above, the device will not accept any HSM FW updates unless they
are genuinely signed by TI. This will in effect restrict updates to HSM FW
images that are OK'd by TI. What if in addition you would like to ensure the
HSM FW on your device is only updated when you have OK'd it?

This can be achieved by wrapping the HSM FW provided in the SDK with your own
customer signature. The steps to achieve this are described in
`<sdk>/tools/hsm/README.md`

# How to Provision the HSM FW

There are multiple methods that are available for you to provision the HSM FW on
your device.

1. CCS / Uniflash
    - Both CCS and Uniflash provide the same HSM FW provisioning functionality
    - This method will have no other effects on the contents of MAIN FLASH
    meaning if your application was already programmed, it will still be there
    after provisioning
    - Please see the "Programming the Hardware Security Module (HSM)" section
    of the SDK docs more details on how to use CCS or Uniflash to provision the
    HSM FW with the `*.bin` file in this directory
2. IAR
    - IAR provides similar HSM FW provisioning functionality as CCS & Uniflash
    - Please see the "Programming the Hardware Security Module (HSM)" for more
    details on how to use IAR to provision the HSM FW with the `*_iar.hex` file
    in this directory
3. In the Field Updates
    - Although HSM FW updates will be increasingly infrequent, the HSM FW can
    indeed be updated
    - Requires the new HSM FW update binary to be staged into MAIN FLASH
    (max 96KB)
    - Requires the app to communicate basic information to System ROM finally
    ending in a software reset of the device
    - ROM will securely perform the HSM FW update and return functionality back
    to the app which can evaluate the result accordingly
    - Please find the example demonstrating this method in the SDK at
    `<sdk>/examples/<nortos|rtos>/LP_EM_CC2745R10_Q1/hsm/hsmfwupdate`
    - The `hsmfwupdate` example has been pre-built for you resulting in a
    `*.out` file in this directory which you can program directly to your
    device
    - Learn more about how to use this example by reading the `README.md` at
    the root of the `hsmfwupdate` example directory