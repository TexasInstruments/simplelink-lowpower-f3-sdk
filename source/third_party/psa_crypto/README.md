## Overview

Arm's Platform Security Architecture (PSA) is a holistic set of threat models,
security analyses, hardware and firmware architecture specifications, an open
source firmware reference implementation, and an independent evaluation and
certification scheme. PSA provides a recipe, based on industry best practice,
that allows security to be consistently designed in, at both a hardware and
firmware level.

The PSA Cryptographic API (Crypto API) is an important PSA component that
provides a portable interface to cryptographic operations on a wide range of
hardware. This implementation is provided as a wrapper around TI SimpleLink
Crypto Drivers. Refer to [PSA Certified Crypto API
v1.1.0](https://arm-software.github.io/psa-api/crypto/) for details on the API.

Hardware acceleration is implemented for AES-CBC, AES-CCM, AES-CMAC, AES-CBCMAC,
AES-CTR, AES-ECB, AES-GCM, ECDH, ECDSA, SHA2, and TRNG.

## Using PSA Crypto

Pre-built libraries are provided in the `source/third_party/psa_crypto/lib`
folder.

To utilize PSA Crypto APIs, include the following header:
`source/third_party/psa_crypto/include/psa/crypto.h`

For a TF-M enabled project, link the library
`source/third_party/psa_crypto/lib/<compiler>/<arch>/psa_crypto_<device>_ns.a`.

For a secure-only project, link the library
`source/third_party/psa_crypto/lib/<compiler>/<arch>/psa_crypto_<device>.a`.

## Limitations

Currently, this software library only supports TF-M enabled projects for
CC13x4/CC26x4/x3 and secure-only projects for CC27xx.

For information on support and limitations for crypto drivers, see the
SimpleLink MCU SDK Driver API Reference.
