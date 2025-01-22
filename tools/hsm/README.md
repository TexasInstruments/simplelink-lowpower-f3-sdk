# HSM FW Update Signing

If you should want your device to only accept HSM FW updates that have been OK'd
by you. Then you will need to wrap the HSM FW provided in this SDK with your own
signature.

The steps below will walk you through using the provided python script to:

1. Generate a pair of private/public keys if you do not already have them
2. Dump the public key hash to configure the SCFG of your device properly
3. Sign the HSM FW update image from this SDK with the new private key

Although it is not required in order to run the python script, it is highly
recommended that you setup a virtual environment to reduce system package
pollution. See instructions for this below.

## System Requirements
- Python >= 3.8.10
- `venv` python module
    - See the venv [docs](https://docs.python.org/3/library/venv.html) for more
    information as necessary

# Environment Setup

The following instructions assume you're using a Unix-Like shell. `Git Bash`,
`WSL2` and native linux are all examples of this.

## Create a virtual environment in the current directory

    $ python -m venv .venv

## Activate the virtual environment (OS specific)

### Windows

    $ source .venv/Scripts/activate

### Linux

    $ source .venv/bin/activate


For clarity, this document will use `$` to denote the prompt of your system
shell and `$(venv)` to denote the prompt of your virtual environment shell.
Therefore, the following command is expected to be ran with your virtual
environment activated.

## Confirm the virtual environment is activated

    $(venv) which python
    <USER_DEPENDENT>/.venv/bin/python

You should see that the `python` executable is no longer referencing your system
wide python version but is instead referencing the one inside of your virtual
environment.

## Install requirements

    $(venv) pip install -r ./requirements.txt

This will install all requirements necessary to run the `sign_hsm_fw.py` script


# Running the `sign_hsm_fw.py` script

Once you have activated the virtual environment as described above. The
`sign_hsm_fw.py` script has a help menu that can be seen with the `-h/--help`
options.


## Generate a new private/public key pair

    $(venv) python ./sign_hsm_fw.py --gen_keys

## Configure your device with the necessary public key information

In order for your device to verify your signature, you must first configure the
device with the public key information. This information shall be configured in
the SCFG of your device so that only your public key can be used to perform
verification.

### Dump your public key hash

    $(venv) python ./sign_hsm_fw.py --dump_key_hash --key <private_key_file>

### Update the SCFG of your application

Open your application's `*.syscfg` file and copy/paste the `public_key_hash`
displayed from the previous step into the following field.

> Device Configuration->Security Configuration->HSM Public Key Hash

## Sign the HSM FW with your private key

    $(venv) python sign_hsm_fw.py --sign --key <private_key_file> --input <sdk>/bin/hsm/<hsm_fw_name>.bin

# Program the new HSM FW

Now that you have a new HSM FW binary image, you can provision your devices with
it or send it as an update to your devices in the field.

- For provisioning , see `<sdk>/bin/hsm/README.md`.
- For in the field updates, see the SDK example at
    `<sdk>/examples/<nortos|rtos>/LP_EM_CC2745R10_Q1/hsm/hsmfwupdate`

# Customizing the signing process

The `sign_hsm_fw.py` script is only an example for how to obtain the necessary
public key hash information and how to sign the original HSM FW image shipped in
this SDK. For best security practices it is unlikely that your company will have
the private key simply residing on an engineer's machine. Instead, this private
key should have restricted access which precludes the ability to pass it in as
an argument to this script.

TI is unable to create a signing script/process that fits all customers process
requirements. Therefore, it is your responsibility to take this example script
as a starting point to creating a script/process that fits your requirements.

The critical information that should be taken from the `sign_hsm_fw.py` script
are:

1. How to create the correct keys
2. How to retrieve the necessary public key and program the device's SCFG
3. How to sign the provided HSM FW image

These three things must be done exactly the same in order for your custom
signing process to result in an HSM FW image which is accepted.



