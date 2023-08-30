/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       ECIES.h
 *
 *  @brief      ECIES driver header
 *
 *  @anchor ti_drivers_ECIES_Overview
 *  # Overview #
 *
 *  The Elliptic Curve Integrated Encryption Scheme (ECIES) driver provides a
 *  public-key encryption scheme based on Elliptic Curve Cryptography. This
 *  driver supports the ECIES implementation defined in the SEC-1 v2.0 standard
 *  https://www.secg.org/sec1-v2.pdf except the cipher and MAC functions were
 *  changed to use AES-128-GCM:
 *
 *  | Function       | Implementation  |
 *  |----------------|-----------------|
 *  | Key Agreement  | ECDH NIST P-256 |
 *  | KDF            | ANSI X9.63      |
 *  | Hash           | SHA-256         |
 *  | Cipher         | AES-128-GCM     |
 *  | MAC            | AES-128-GCM     |
 *
 *  @anchor ti_drivers_ECIES_Usage
 *  # Usage #
 *
 *  Before starting a ECIES operation, the application must do the following:
 *      - Call #ECIES_init() to initialize the driver.
 *      - Call #ECIES_Params_init() to initialize the ECIES_Params to default values.
 *      - Modify the #ECIES_Params as desired.
 *      - Call #ECIES_open() to open an instance of the driver.
 *
 *  @anchor ti_drivers_ECIES_Synopsis
 *  # Synopsis
 *
 *  @anchor ti_drivers_ECIES_Synopsis_Code
 *  @code
 *
 *  // Import ECIES Driver definitions
 *  #include <ti/drivers/ECIES.h>
 *
 *  // Import driver configuration
 *  #include "ti_drivers_config.h"
 *
 *  // Initialize driver
 *  ECIES_init();
 *
 *  // Open driver instance
 *  handle = ECIES_open(CONFIG_ECIES_0, NULL);
 *
 *  // Perform ECIES encryption
 *  result = ECIES_encrypt(handle, &publicKey, input, sizeof(input), output, sizeof(output));
 *
 *  // Close driver instance
 *  ECIES_close(handle);
 *  @endcode
 *
 *  @anchor ti_drivers_ECIES_Example
 *  # Example #
 *
 *  The #ECIES_encrypt() function performs an ECIES encryption operation in
 *  a single call.
 *
 *  After an ECIES operation completes, the application may either start
 *  another operation or close the driver by calling #ECIES_close().
 *
 *  @code
 *  #define INPUT_SIZE 24
 *
 *  CryptoKey publicKey;
 *  ECIES_Params params;
 *  ECIES_Handle handle;
 *  int_fast16_t result;
 *  uint8_t sharedInfo[] = {0x75, 0xee, 0xf8, 0x1a, 0xa3, 0x04, 0x1e, 0x33,
 *                          0xb8, 0x09, 0x71, 0x20, 0x3d, 0x2c, 0x0c, 0x52};
 *  uint8_t input[INPUT_SIZE] = {0x22, 0x51, 0x8b, 0x10, 0xe7, 0x0f, 0x2a, 0x3f,
 *                               0x24, 0x38, 0x10, 0xae, 0x32, 0x54, 0x13, 0x9e,
 *                               0xfb, 0xee, 0x04, 0xaa, 0x57, 0xc7, 0xaf, 0x7d};
 *  uint8_t output[ECIES_PADDING_BYTES + ECIES_PUBLIC_KEY_SIZE + INPUT_SIZE + ECIES_TAG_SIZE];
 *  uint8_t publicKeyMaterial[] = {0x04,
 *                                 // X coordinate
 *                                 0x33,0x91,0x50,0x84,0x4E,0xC1,0x52,0x34,
 *                                 0x80,0x7F,0xE8,0x62,0xA8,0x6B,0xE7,0x79,
 *                                 0x77,0xDB,0xFB,0x3A,0xE3,0xD9,0x6F,0x4C,
 *                                 0x22,0x79,0x55,0x13,0xAE,0xAA,0xB8,0x2F,
 *                                 // Y coordinate
 *                                 0xB1,0xC1,0x4D,0xDF,0xDC,0x8E,0xC1,0xB2,
 *                                 0x58,0x3F,0x51,0xE8,0x5A,0x5E,0xB3,0xA1,
 *                                 0x55,0x84,0x0F,0x20,0x34,0x73,0x0E,0x9B,
 *                                 0x5A,0xDA,0x38,0xB6,0x74,0x33,0x6A,0x21};
 *
 *  // RNG initialization should be handled by the application after the RNG
 *  // driver is seeded with the noise input from Radio Control Layer.
 *
 *  ECIES_init();
 *
 *  ECIES_Params_init(&params);
 *  params.returnBehavior = ECIES_RETURN_BEHAVIOR_POLLING;
 *
 *  handle = ECIES_open(CONFIG_ECIES_0, &params);
 *  assert(handle != NULL);
 *
 *  CryptoKeyPlaintext_initKey(&publicKey, publicKeyMaterial, sizeof(publicKeyMaterial));
 *
 *  result = ECIES_encrypt(handle, &publicKey, input, sizeof(input), output, sizeof(output));
 *  assert(result == ECIES_STATUS_SUCCESS);
 *
 *  ECIES_close(handle);
 *  @endcode
 */

#ifndef ti_drivers_ECIES__include
#define ti_drivers_ECIES__include

#include <stddef.h>
#include <stdint.h>

#include <ti/drivers/ANSIX936KDF.h>
#include <ti/drivers/cryptoutils/ecc/ECCParams.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  Common ECIES status code reservation offset.
 *  ECIES driver implementations should offset status codes with
 *  ECIES_STATUS_RESERVED growing negatively.
 *
 *  Example implementation specific status codes:
 *  @code
 *  #define ECIESXYZ_STATUS_ERROR0    ECIES_STATUS_RESERVED - 0
 *  #define ECIESXYZ_STATUS_ERROR1    ECIES_STATUS_RESERVED - 1
 *  #define ECIESXYZ_STATUS_ERROR2    ECIES_STATUS_RESERVED - 2
 *  @endcode
 */
#define ECIES_STATUS_RESERVED ((int_fast16_t)-32)

/*!
 *  @brief  Successful status code.
 *
 *  Functions return ECIES_STATUS_SUCCESS if the function was executed
 *  successfully.
 */
#define ECIES_STATUS_SUCCESS ((int_fast16_t)0)

/*!
 *  @brief  Generic error status code.
 *
 *  Functions return ECIES_STATUS_ERROR if the function was not executed
 *  successfully and no more specific error is applicable.
 */
#define ECIES_STATUS_ERROR ((int_fast16_t)-1)

/*!
 *  @brief  An error status code returned if the hardware or software resource is
 *          currently unavailable.
 *
 *  ECIES driver implementations may have hardware or software limitations on how
 *  many clients can simultaneously perform operations. This status code is
 *  returned if the mutual exclusion mechanism signals that an operation cannot
 *  currently be performed.
 */
#define ECIES_STATUS_RESOURCE_UNAVAILABLE ((int_fast16_t)-2)

/*!
 *  @brief  An error status code returned if the output length is insufficient.
 */
#define ECIES_STATUS_INSUFFICIENT_OUTPUT_LENGTH ((int_fast16_t)-3)

/*!
 *  @brief  An error status code returned if the MAC provided by the application for
 *  a decryption operation does not match the one calculated during the operation.
 *
 *  This code is returned by ECIES_decrypt() if the verification of the MAC fails.
 */
#define ECIES_STATUS_MAC_INVALID ((int_fast16_t)-4)

/*!
 * @brief   The operation does not support non-word-aligned input and/or output.
 *
 * ECIES driver implementations may have restrictions on the alignment of
 * input/output data due to performance limitations of the hardware.
 */
#define ECIES_STATUS_UNALIGNED_IO_NOT_SUPPORTED AES_STATUS_UNALIGNED_IO_NOT_SUPPORTED

/*!
 *  @brief  The way in which ECIES function calls return after performing an
 *          operation.
 *
 *  Not all ECIES operations exhibit the specified return behavior. Functions
 *  that do not require significant computation and cannot offload that
 *  computation to a background thread behave like regular functions. Which
 *  functions exhibit the specified return behavior is not implementation
 *  dependent. Specifically, a software-backed implementation run on the same CPU
 *  as the application will emulate the return behavior while not actually
 *  offloading the computation to the background thread.
 *
 *  ECIES functions exhibiting the specified return behavior have restrictions on
 *  the context from which they may be called.
 *
 *   |                                       | Task  | Hwi   | Swi   |
 *   |---------------------------------------|-------|-------|-------|
 *   |ECIES_RETURN_BEHAVIOR_BLOCKING         | X     |       |       |
 *   |ECIES_RETURN_BEHAVIOR_POLLING          | X     | X     | X     |
 *
 */
typedef enum
{
    /*! The function call will block while the ECIES operation goes
     *  on in the background. ECIES operation results are available
     *  after the function returns.
     */
    ECIES_RETURN_BEHAVIOR_BLOCKING = 2,

    /*! The function call will continuously poll a flag while the ECIES
     *  operation goes on in the background. ECIES operation results
     *  are available after the function returns.
     */
    ECIES_RETURN_BEHAVIOR_POLLING = 4,
} ECIES_ReturnBehavior;

/*!
 *  @brief  ECIES authentication tag size in bytes
 */
#define ECIES_TAG_SIZE 16U

#ifndef ECCParams_NISTP256_LENGTH
    /*!
     *  @brief Length of NIST P256 curve parameters in bytes
     */
    #define ECCParams_NISTP256_LENGTH 32U
#endif

/*!
 *  @brief  ECIES public key size in bytes
 *
 *  The size of the ECIES public key in uncompressed octet string format (0x04 || x || y)
 */
#define ECIES_PUBLIC_KEY_SIZE (1U + (2U * ECCParams_NISTP256_LENGTH))

/*!
 *  @brief  ECIES padding size in bytes
 *
 * The beginning of the input to #ECIES_decrypt and the beginning of the output
 * to #ECIES_encrypt must be padded with 3-bytes in order to word-align the
 * ciphertext or plaintext as required by the AESGCMLPF3 driver.
 */
#define ECIES_PADDING_BYTES 3U

/*!
 *  @brief ECIES Global configuration
 *
 *  The %ECIES_Config structure contains a set of pointers used to characterize
 *  the ECIES driver implementation.
 *
 *  This structure needs to be defined before calling #ECIES_init() and it must
 *  not be changed thereafter.
 *
 *  @sa     ECIES_init()
 */
typedef struct
{
    /*! Pointer to a driver specific data object */
    void *object;

    /*! Pointer to a driver specific hardware attributes structure */
    void const *hwAttrs;
} ECIES_Config;

/*!
 *  @brief  A handle that is returned from an ECIES_open() call.
 */
typedef ECIES_Config *ECIES_Handle;

/*!
 *  @brief  ECIES Parameters
 *
 *  ECIES Parameters are used to with the ECIES_open() call. Default values for
 *  these parameters are set using ECIES_Params_init().
 *
 *  @sa     ECIES_Params_init()
 */
typedef struct
{
    ECIES_ReturnBehavior returnBehavior; /*!< Blocking or polling return behavior
                                          *   @warning This setting must match the RNG's
                                          *   global return behavior
                                          */
    uint32_t timeout;                    /*!< Timeout before the driver returns an error in
                                          *   ::ECIES_RETURN_BEHAVIOR_BLOCKING
                                          */
} ECIES_Params;

/*!
 * @brief Global ECIES configuration struct.
 *
 * Specifies context objects and hardware attributes for every
 * driver instance.
 *
 * This variable is supposed to be defined in the board file.
 */
extern const ECIES_Config ECIES_config[];

/*!
 * @brief Global ECIES configuration count.
 *
 * Specifies the number of available ECIES driver instances.
 *
 * This variable is supposed to be defined in the board file.
 */
extern const uint_least8_t ECIES_count;

/*!
 *  @brief  Default ECIES_Params structure
 *
 *  @sa     #ECIES_Params_init()
 */
extern const ECIES_Params ECIES_defaultParams;

/*!
 *  @brief  Initializes the ECIES driver module.
 *
 *  @pre    The #ECIES_config structure must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other ECIES driver APIs. This function call does not modify any
 *          peripheral registers.
 */
void ECIES_init(void);

/*!
 *  @brief  Initializes @c params with default values.
 *
 *  @param  params      A pointer to #ECIES_Params structure for
 *                      initialization.
 *
 *  Default values:  <br>
 *      returnBehavior    = ECIES_RETURN_BEHAVIOR_POLLING         <br>
 *      timeout           = SemaphoreP_WAIT_FOREVER               <br>
 */
void ECIES_Params_init(ECIES_Params *params);

/*!
 *  @brief  Initializes a ECIES driver instance and returns a handle.
 *
 *  @warning The return behavior specified in the #ECIES_Params struct must match
 *           the RNG's global return behavior setting for this function to succeed.
 *
 *  @pre    ECIES controller has been initialized using #ECIES_init()
 *
 *  @param  index         Logical peripheral number for the ECIES indexed into
 *                        the #ECIES_config table.
 *
 *  @param  params        Pointer to a #ECIES_Params struct, if NULL it will use
 *                        default values.
 *
 *  @return A #ECIES_Handle on success, otherwise NULL if an error occurs or if
 *          the driver instance has been opened already.
 *
 *  @sa     #ECIES_init()
 *  @sa     #ECIES_close()
 */
ECIES_Handle ECIES_open(uint_least8_t index, const ECIES_Params *params);

/*!
 *  @brief  Closes a ECIES peripheral specified by @c handle.
 *
 *  @pre    #ECIES_open() has to be called first.
 *
 *  @param  handle A #ECIES_Handle returned from ECIES_open()
 *
 *  @sa     #ECIES_open()
 */
void ECIES_close(ECIES_Handle handle);

/*!
 *  @brief  Performs ECIES encryption of a message.
 *
 *  Performs authenticated encryption (AES-128-GCM) of a message to a NIST P-256
 *  public key which is used to derive the AES-128-GCM encryption key and
 *  Initialization Vector (IV).
 *
 *  @note   The @c paddedOutput will be padded with #ECIES_PADDING_BYTES of zeros.
 *          This padding must be discarded by the application.
 *
 *  @pre    #ECIES_open() has to be called first.
 *
 *  @param  handle        A #ECIES_Handle returned from #ECIES_open().
 *
 *  @param  publicKey     A pointer to the NIST P-256 public key in uncompressed
 *                        octet string format and big-endian byte order.
 *
 *  @param  input         A pointer to the input buffer containing the data to
 *                        encrypt.
 *
 *  @param  inputLen      The length of @c input in bytes.
 *
 *  @param  paddedOutput  A pointer to the location to write the output (pad || Q || C || T)
 *                        where pad is #ECIES_PADDING_BYTES of zeros,
 *                        Q is the public key, C is the ciphertext, and T is the
 *                        authentication tag.
 *
 *  @param  paddedOutputLen  Output buffer length in bytes. It should be at least
 *                           (#ECIES_PADDING_BYTES + #ECIES_PUBLIC_KEY_SIZE +
 *                           inputLen + #ECIES_TAG_SIZE).
 *
 *  @retval #ECIES_STATUS_SUCCESS               The operation succeeded.
 *  @retval #ECIES_STATUS_ERROR                 The operation failed.
 *  @retval #ECIES_STATUS_RESOURCE_UNAVAILABLE  The required hardware resource
 *                                              was not available. Try again
 *                                              later.
 *  @retval #ECIES_STATUS_INSUFFICIENT_OUTPUT_LENGTH  The outputLen is insufficient
 *                                                    to write the output.
 *  @retval #ECIES_STATUS_UNALIGNED_IO_NOT_SUPPORTED  The input and/or output buffer
 *                                                    were not word-aligned.
 *
 *  @sa     #ECIES_open()
 */
int_fast16_t ECIES_encrypt(ECIES_Handle handle,
                           const CryptoKey *publicKey,
                           const void *input,
                           size_t inputLen,
                           void *paddedOutput,
                           size_t paddedOutputLen);

/*!
 *  @brief  Performs ECIES decryption of a message.
 *
 *  Performs authenticated decryption (AES-128-GCM) of a message to a NIST P-256
 *  private key which is used to derive the AES-128-GCM decryption key and
 *  Initialization Vector (IV).
 *
 *  @note   The @c paddedInput must be padded with #ECIES_PADDING_BYTES of zeros.
 *
 *  @pre    #ECIES_open() has to be called first.
 *
 *  @param  handle        A #ECIES_Handle returned from #ECIES_open().
 *
 *  @param  privateKey    A pointer to the NIST P-256 private key in uncompressed
 *                        octet string format and big-endian byte order.
 *
 *  @param  paddedInput   A pointer to the input buffer containing (pad || Q || C || T)
 *                        where pad is #ECIES_PADDING_BYTES of zeros,
 *                        Q is the public key, C is the ciphertext, and T is the
 *                        authentication tag.
 *
 *  @param  paddedInputLen  The length of @c input in bytes.
 *
 *  @param  output        A pointer to the location to write the output plaintext.
 *
 *  @param  outputLen     Output buffer length in bytes. It should be at least
 *                        inputLen - #ECIES_PUBLIC_KEY_SIZE - #ECIES_TAG_SIZE -
 *                        #ECIES_PADDING_BYTES.
 *
 *  @retval #ECIES_STATUS_SUCCESS               The operation succeeded.
 *  @retval #ECIES_STATUS_ERROR                 The operation failed.
 *  @retval #ECIES_STATUS_RESOURCE_UNAVAILABLE  The required hardware resource
 *                                              was not available. Try again
 *                                              later.
 *  @retval #ECIES_STATUS_INSUFFICIENT_OUTPUT_LENGTH  The outputLen is insufficient
 *                                                    to write the output.
 *  @retval #ECIES_STATUS_UNALIGNED_IO_NOT_SUPPORTED  The input and/or output buffer
 *                                                    were not word-aligned.
 *  @retval #ECIES_STATUS_MAC_INVALID           The provided authentication tag
 *                                              did not match the recomputed one.
 *
 *  @sa     #ECIES_open()
 */
int_fast16_t ECIES_decrypt(ECIES_Handle handle,
                           const CryptoKey *privateKey,
                           const void *paddedInput,
                           size_t paddedInputLen,
                           void *output,
                           size_t outputLen);

/*!
 *  @brief  Constructs a new ECIES object
 *
 *  Unlike #ECIES_open(), #ECIES_construct() does not require the hwAttrs and
 *  object to be allocated in a #ECIES_Config array that is indexed into.
 *  Instead, the #ECIES_Config, hwAttrs, and object can be allocated at any
 *  location. This allows for relatively simple run-time allocation of temporary
 *  driver instances on the stack or the heap.
 *  The drawback is that this makes it more difficult to write device-agnostic
 *  code. If you use an ifdef with DeviceFamily, you can choose the correct
 *  object and hwAttrs to allocate. That compilation unit will be tied to the
 *  device it was compiled for at this point. To change devices, recompilation
 *  of the application with a different DeviceFamily setting is necessary.
 *
 *  @pre    The #ECIES_Config struct must be zeroed out prior to
 *          calling this function. Otherwise, unexpected behavior may ensue.
 *
 *  @warning The return behavior specified in the #ECIES_Params struct must match
 *           the RNG's global return behavior setting for this function to succeed.
 *
 *  @param  config        A pointer to a #ECIES_Config struct describing the
 *                        location of the object and hwAttrs.
 *
 *  @param  params        A pointer to a #ECIES_Params struct to configure the
 *                        driver instance.
 *
 *  @return Returns a #ECIES_Handle on success or NULL on failure.
 */
ECIES_Handle ECIES_construct(ECIES_Config *config, const ECIES_Params *params);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_ECIES__include */
