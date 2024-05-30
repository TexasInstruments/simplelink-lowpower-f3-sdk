/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated
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
 *  @file       HSMLPF3.h
 *
 *  @brief      Interface to for all HSM-related operations
 *
 *  This module provides functions for use of the Hardware Security Module.
 *
 *  The HSM initialization procedure must first enable the clock, then initialize the
 *  mailbox, and finally boot the device.
 *
 *  APIs are provided to synchronize access to the HSM and submit command tokens,
 *  constructed by TI crypto drivers.
 *
 * Additional APIs to construct command tokens, interpret result token data as well as additional key/asset management
 * operations.
 *
 *
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/SHA2.h>
#include <ti/drivers/sha2/SHA2LPF3HSM.h>

#include <ti/drivers/ECDSA.h>
#include <ti/drivers/ecdsa/ECDSALPF3HSM.h>

#include <ti/drivers/AESGCM.h>
#include <ti/drivers/aesgcm/AESGCMLPF3HSM.h>

#include <ti/drivers/AESECB.h>
#include <ti/drivers/aesecb/AESECBLPF3.h>

#include <ti/drivers/AESCTR.h>
#include <ti/drivers/aesctr/AESCTRLPF3.h>

#include <ti/drivers/AESCBC.h>
#include <ti/drivers/aescbc/AESCBCLPF3.h>

#include <ti/drivers/AESCMAC.h>
#include <ti/drivers/aescmac/AESCMACLPF3.h>

#include <ti/drivers/AESCCM.h>
#include <ti/drivers/aesccm/AESCCMLPF3.h>

#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_common.h>

/*!
 *  @brief   Successful status code.
 *
 *  Functions return #HSMLPF3_STATUS_SUCCESS if the function was executed
 *  successfully.
 */
#define HSMLPF3_STATUS_SUCCESS ((int_fast16_t)0)

/*!
 *  @brief   Generic error status code.
 *
 *  Functions return #HSMLPF3_STATUS_ERROR if the function was not executed
 *  successfully and no more specific error is applicable.
 */
#define HSMLPF3_STATUS_ERROR ((int_fast16_t)-1)

/*!
 *  @brief   Timeout for polling mode response.
 *
 *  Functions return #HSMLPF3_STATUS_TIMEOUT if the #HSMLPF3_ReturnBehavior is
 *  set to #HSMLPF3_RETURN_BEHAVIOR_POLLING and the HSM does not provide an output
 *  token in sufficient time.
 */
#define HSMLPF3_STATUS_TIMEOUT ((int_fast16_t)-2)

/*!
 * @brief   The way in which #HSMLPF3_waitForResult() function calls return after performing an
 * calling #HSMLPF3_submitToken().
 *
 *
 * #HSMLPF3_waitForResult() has restrictions on the context from
 * which it may be called, depending on the return behavior.
 *
 * |                                   | Task  | Hwi   | Swi   |
 * |-----------------------------------|-------|-------|-------|
 * |HSMLPF3_RETURN_BEHAVIOR_CALLBACK   | X     | X     | X     |
 * |HSMLPF3_RETURN_BEHAVIOR_BLOCKING   | X     |       |       |
 * |HSMLPF3_RETURN_BEHAVIOR_POLLING    | X     | X     | X     |
 *
 */
typedef enum
{
    HSMLPF3_RETURN_BEHAVIOR_CALLBACK = 1, /*!< The #HSMLPF3_waitForResult() call will return immediately while
                                           *   the HSM operation goes on in the background. The registered
                                           *   callback function is called after the operation completes.
                                           *   The callback function is a driver-specific post-processing
                                           *   function that will be called from the common HWI.
                                           */

    HSMLPF3_RETURN_BEHAVIOR_BLOCKING = 2, /*!< The #HSMLPF3_waitForResult() call will block while the HSM
                                           *   operation goes on in the background. The HWI will post the
                                           *   semaphore. HSM operation results are available after the
                                           *   function returns.
                                           */

    HSMLPF3_RETURN_BEHAVIOR_POLLING = 4, /*!< The #HSMLPF3_waitForResult() call will continuously poll a flag
                                          *   while the HSM operation goes on in the background. The HWI will
                                          *   set the flag. SHA2 operation results are available after the
                                          *   function returns.
                                          */

} HSMLPF3_ReturnBehavior;

/*!
 *  @brief  Pointer to crypto driver post-processing functions.
 *
 *  Crypto drivers pass an #HSMLPF3_CallbackFxn as a parameter to #HSMLPF3_submitToken().
 *  In callback mode, the HSMLPF3 HWI will call the drivers' post-processing functions, as
 *  stored in the #HSMLPF3_Operation struct.
 */
typedef void (*HSMLPF3_CallbackFxn)(uintptr_t arg0);

/*!
 *  @brief  Struct to hold metadata for a crypto driver's operation.
 *
 *  #HSMLPF3_acquireLock() populates the #HSMLPF3_Operation.driverHandle while
 *  the rest of the metadata is populated by #HSMLPF3_submitToken(). The HWI
 *  accesses this stored information to respond to the calling driver.
 */
typedef struct
{
    Eip130Token_Command_t commandToken;
    Eip130Token_Result_t resultToken;
    HSMLPF3_ReturnBehavior returnBehavior;
    HSMLPF3_CallbackFxn callbackFxn;
    uintptr_t driverHandle;
} HSMLPF3_Operation;

#define AES_MODE_ENCRYPT 1U
#define AES_MODE_DECRYPT 0U

#define HSMLPF3_RETVAL_MASK MASK_8_BITS

/*!
 *  @brief  Initializes the HWI and semaphores for HSMLPF3.
 *
 *  This function registers a hardware interrupt for result tokens
 *  from the HSM, and constructs the HSMLPF3_accessSemaphore and
 *  HSMLPF3_operationSemaphore.
 *
 *  This function should be called from driver init() functions.
 *
 *  @post   #HSMLPF3_init()
 */
void HSMLPF3_constructRTOSObjects(void);

/*!
 *  @brief  Disables clock for HSM, effectively powering it off.
 *
 *  This function provides an alternative to putting the HSM to sleep
 *  when it no longer needs to be used.
 */
void HSMLPF3_disableClock(void);

/*!
 *  @brief  Puts the HSM to sleep, lowering its power consumption.
 *
 *  This function submits a sleep token to the HSM, and waits for a
 *  successful token result.
 *
 *  Calling this function when the HSM is already asleep will still
 *  submit a sleep token.
 *
 *  @pre    #HSMLPF3_init() has to be called first.
 *
 *  @retval #HSMLPF3_STATUS_SUCCESS               HSM put to sleep successfully.
 *  @retval #HSMLPF3_STATUS_ERROR                 Error. Error output token, or HSM in bad state.
 */
int_fast16_t HSMLPF3_sleep(void);

/*!
 *  @brief  Initializes the HSM and HSMLPF3 driver for token submissions.
 *
 *  This function enables the HSM clock, initializes the HSM mailbox,
 *  boots the HSM, and performs extra initialization for the token submission
 *  process. The return value reflects if any of these operations fail.
 *
 *  This function should be called in driver construct() functions.
 *
 *  @pre    #HSMLPF3_constructRTOSObjects()
 *  @post   #HSMLPF3_acquireLock()
 *
 *  @retval #HSMLPF3_STATUS_SUCCESS               HSM booted and initialized successfully.
 *  @retval #HSMLPF3_STATUS_ERROR                 Error. Failed boot or mailbox initiailization.
 */
int_fast16_t HSMLPF3_init(void);

/*!
 *  @brief  Acquires the HSMLPF3_accessSemaphore.
 *
 *  This function will attempt to take the HSMLPF3_accessSemaphore.
 *  The calling driver must decide whether timeout is
 *  SemaphoreP_NO_WAIT or another value.
 *
 *  @param  [in] timeout         Amount of time to wait for semaphore
 *                               to be posted.
 *
 *  @param  [in] driverHandle   The driver's handle.
 *
 *  @pre    #HSMLPF3_init() has to be called first.
 *  @post   #HSMLPF3_submitToken()
 *
 *  @retval Returns True on success, False on timeout
 */
bool HSMLPF3_acquireLock(uint32_t timeout, uintptr_t driverHandle);

/*!
 *  @brief  Releases the lock on HSM access.
 *
 *  This function will release the HSMLPF3_accessSemaphore.
 *  It will also clear the #HSMLPF3_Operation.driverHandle member.
 *
 *  @pre    #HSMLPF3_init() has to be called first.
 */
void HSMLPF3_releaseLock(void);

/*!
 *  @brief  Submits a token to the HSM mailbox.
 *
 *  This function will convert the provided logical token
 *  into a physical token for the HSM to process. It will also
 *  save the operation metadata to the HSMLPF3 driver, for use
 *  in #HSMLPF3_waitForResult().
 *
 *  @param  [in] retBehavior    Driver's return behavior for use in
 *                              #HSMLPF3_waitForResult().
 *
 *  @param  [in] callbackFxn    Pointer to driver's result post-processing
 *                              function. This post-processing function will
 *                              call the user-provided callback function, if
 *                              the driver is in callback mode.
 *
 *  @param  [in] driverHandle   The driver's handle.
 *
 *  @pre    #HSMLPF3_acquireLock() has to be successfully called first.
 *  @post   #HSMLPF3_waitForResult()
 *
 *  @retval #HSMLPF3_STATUS_SUCCESS               Token successfully written to HSM mailbox.
 *  @retval #HSMLPF3_STATUS_ERROR                 Error. Invalid token or unsuccessful write to mailbox.
 */
int_fast16_t HSMLPF3_submitToken(HSMLPF3_ReturnBehavior retBehavior,
                                 HSMLPF3_CallbackFxn callbackFxn,
                                 uintptr_t driverHandle);

/*!
 *  @brief  Waits for HSM response to previously submitted token.
 *
 *  This function is responsible for reporting the result back to the
 *  driver that submitted an operation request. Depending on the
 *  return behavior provided in #HSMLPF3_submitToken(), this function
 *  will poll a flag, block on an operation semaphore, or return
 *  immediately.
 *
 *  @pre    #HSMLPF3_submitToken() has to be called first.
 *
 *  @retval #HSMLPF3_STATUS_SUCCESS               Successfully waited for the result.
 *  @retval #HSMLPF3_STATUS_ERROR                 Error. Provided return behavior invalid.
 */
int_fast16_t HSMLPF3_waitForResult(void);

/*!
 *  @brief Cancels the operation currently in progress.
 *
 *  This API checks to see if there is an operation currently running.
 *  If so, wait on the HSM to finish in polling mode and clear the appropriate interrupts.
 *
 *  @retval #HSMLPF3_STATUS_SUCCESS               Successfully canceled operation in progress.
 *  @retval #HSMLPF3_STATUS_ERROR                 Error.
 */
int_fast16_t HSMLPF3_cancelOperation(void);

/*!
 *  @brief returns the value of HSMLPF3_operationInProgress
 *
 *  @retval true                        Operation in progress
 *  @retval false                       No operation in progress
 */
bool HSMLPF3_isOperationInProgress(void);

/*
 *  ================ APIs to handle result token data ================
 */

/*!
 *  @brief  Get operation result code
 *
 *  This API should be called during a driver's post processing sequence after a call
 *  to #HSMLPF3_submitToken() that submits an asset creation token.
 *
 *  @retval Result token return code.
 */
int32_t HSMLPF3_getResultCode(void);

/*!
 *  @brief  Fetches the asset ID from the HSM's result token
 *
 *  This API should be called during a driver's post processing sequence after a call
 *  to #HSMLPF3_submitToken() that submits an asset creation token.
 *
 *  @retval Asset Identification Number
 */
uint32_t HSMLPF3_getResultAssetID(void);

/*!
 *  @brief  Copies a hash operation's digest to user space
 *
 *  @param  [in] digest             User specified destination address
 *
 *  @param  [in] digestLength User's digest length
 */
void HSMLPF3_getResultDigest(uint32_t *digest, size_t digestLength);

/*!
 *  @brief  Fetches the AES tag
 *
 *  @param  [in] object             Pointer to copy tag to
 */
void HSMLPF3_getAESEncryptTag(uint8_t *mac);

/*!
 *  @brief  Fetches the AES IV
 *
 *  @param  [in] object             Pointer to copy iv to
 */
void HSMLPF3_getAESIV(uint8_t *iv);

/*!
 *  @brief  Fetches the Final mac from result token
 *
 *  @param  [in] mac                Buffer to copy mac to
 *  @param  [in] macLength          Length of the mac
 */
void HSMLPF3_getAESCMACSignMac(uint8_t *mac, uint8_t macLength);

/*
 *  ================ APIs to construct key/asset management-related command tokens ================
 */

/*!
 *  @brief  Constructs an asset create command token
 *
 *  @param  [in] assetPolicy        User's asset policy to create an asset
 *
 *  @param  [in] assetLength        User's asset length to allocate on HSM RAM
 */
void HSMLPF3_constructCreateAssetToken(uint64_t assetPolicy, uint32_t assetLength);

/*!
 *  @brief  Constructs an asset load plaintext command token
 *
 *  @pre    A successfully conducted asset creation operation that returned an assetID.
 *
 *  @param  [in] input_p            User's asset policy to create an asset
 *
 *  @param  [in] inputLength        User's key material length which should be consistent
 *                                  with the key length passed during an asset create operation
 *
 *  @param  [in] assetId            Asset ID to load the key material to on HSM RAM
 */
void HSMLPF3_constructLoadPlaintextAssetToken(const uint8_t *input_p, const uint32_t inputLength, uint32_t assetId);

/*!
 *  @brief  Constructs an asset delete command token
 *
 *  @param  [in] assetId            Asset ID to load the key material to on HSM RAM
 *
 *  @pre    A successfully conducted asset creation operation that returned an assetID.
 */
void HSMLPF3_constructDeleteAssetToken(uint32_t assetId);

/*
 *  ================ APIs to construct driver-specific command tokens ================
 */

/*!
 *  @brief  Constructs a SHA2 onestep/segmented command token
 *
 *  @param  [in] object             SHA2LPF3HSM object
 *
 */
void HSMLPF3_constructSHA2PhysicalToken(SHA2LPF3HSM_Object *object);

/*!
 *  @brief  Constructs a ECDSA sign/verify command token
 *
 *  @param  [in] object             ECDSALPF3HSM object
 */
void HSMLPF3_constructECDSASignPhysicalToken(ECDSALPF3HSM_Object *object);

/*!
 *  @brief  Constructs a ECDH num load command token
 *
 *  @param  [in] buffer             Input data to load on to the HSM
 *  @param  [in] index              Input index to pass on to the HSM
 *  @param  [in] length             Input data length
 */
void HSMLPF3_constructECDHnumLoadPhysicalToken(const uint8_t *buffer, uint8_t index, uint8_t length);

/*!
 *  @brief  Constructs a ECDH num set command token
 *
 *  @param  [in] length             Curve length to notify the HSM about
 */
void HSMLPF3_constructECDHnumSetPhysicalToken(uint8_t length);

/*!
 *  @brief  Constructs a ECDH PK command token
 *
 *  @param  [in] operation          Operation type
 *  @param  [in] input              Input data address
 *  @param  [in] output             Output data address
 *  @param  [in] inputLength        Input data length
 */
void HSMLPF3_constructECDHPKAOperationPhysicalToken(uint8_t operation,
                                                    uint8_t *input,
                                                    uint8_t *output,
                                                    uint32_t inputLength);

/*!
 *  @brief  Constructs an AES-GCM one-step/segmented command token
 *
 *  @param  [in] object             The AESGCMLPF3HSM object that contains necessary data
 */
void HSMLPF3_constructAESGCMOneStepPhysicalToken(AESGCMLPF3HSM_Object *object);

/*!
 *  @brief  Populate the command token to reflect an AES-GCM segmented AAD operation
 *
 *  @param  [in] object             The AESGCMLPF3HSM object that contains necessary data
 */
void HSMLPF3_constructAESGCMSegmentedAADPhysicalToken(AESGCMLPF3HSM_Object *object);

/*!
 *  @brief  Populate the command token to reflect an AES-GCM segmented or final data operation
 *
 *  @param  [in] object             The AESGCMLPF3HSM object that contains necessary data
 */
void HSMLPF3_constructAESGCMSegmentedDataPhysicalToken(AESGCMLPF3HSM_Object *object);

/*!
 *  @brief  Constructs an AES-CCM one-step/segmented command token
 *
 *  @param  [in] object             The AESCCMLPF3HSM object that contains necessary data
 */
void HSMLPF3_constructAESCCMOneStepPhysicalToken(const AESCCMLPF3_Object *object);

/*!
 *  @brief  Populate the command token to reflect an AES-CCM segmented AAD operation
 *
 *  @param  [in] object             The AESCCMLPF3HSM object that contains necessary data
 */
void HSMLPF3_constructAESCCMSegmentedAADPhysicalToken(const AESCCMLPF3_Object *object);

/*!
 *  @brief  Populate the command token to reflect an AES-CCM segmented or final data operation
 *
 *  @param  [in] object             The AESCCMLPF3HSM object that contains necessary data
 */
void HSMLPF3_constructAESCCMSegmentedDataPhysicalToken(const AESCCMLPF3_Object *object);

/*!
 *  @brief  Constructs an AES-ECB one-step command token
 *
 *  @param  [in] object             The AESECBLPF3 object that contains necessary data
 */
void HSMLPF3_constructAESECBOneStepPhysicalToken(AESECBLPF3_Object *object);

/*!
 *  @brief  Constructs an AES-CTR one-step command token
 *
 *  @param  [in] object             The AESCTRLPF3 object that contains necessary data
 */
void HSMLPF3_constructAESCTROneStepPhysicalToken(AESCTRLPF3_Object *object);

/*
 *  @brief  Constructs an AES-CBC one-step command token
 *
 *  @param  [in] object             The AESCBCLPF3 object that contains necessary data
 */
void HSMLPF3_constructAESCBCOneStepPhysicalToken(AESCBCLPF3_Object *object);

/*!
 *  @brief  Constructs an AES-CMAC one-step command token
 *
 *  @param  [in] object             The AESCMACLPF3 object that contains necessary data
 */
void HSMLPF3_constructAESCMACOneStepPhysicalToken(AESCMACLPF3_Object *object);

/*!
 *  @brief  Constructs an AES-CMAC update command token
 *
 *  @param  [in] object             The AESCMACLPF3 object that contains necessary data
 */
void HSMLPF3_constructAESCMACUpdatePhysicalToken(AESCMACLPF3_Object *object, bool isInitWithDefault);