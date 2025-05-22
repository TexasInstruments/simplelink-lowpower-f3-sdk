/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

/*
 *  ======== keyBlobBareMetal.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART2.h>
#include <ti/drivers/SHA2.h>
#include "HSMBareMetal.h"

/* Driver configuration */
#include "ti_drivers_config.h"

/* Defines */
#define SYM_KEY_LENGTH_128      16
#define KEY_BLOB_KEY_LENGTH_128 HSM_KEYBLOB_SIZE(SYM_KEY_LENGTH_128)
#define MSG_MAX_LENGTH          512
#define AAD_LENGTH              32
#define NONCE_LENGTH            7
#define MAC_LENGTH              16

/* UART pre-formatted strings */
static const char
    promptStartup[] = "\n\n\rBare Metal Asset and AES operations via the Hardware Security Module (HSM) Demo\r";
static const char promptSeparator[] = "\n\n\r**********************************************************************\r";
static const char promptKeyGenerationDescription
    [] = "\n\n\rKey generation happens through RNG operations. HSMBareMetal supports \n\rin-API generation which means "
         "RNG operations can happen within asset operations.\r";
static const char promptKeyGenerationChoice[] = "\n\n\rIn-API key generation? (Y/N):";
static const char
    promptRNGKeyGeneration[] = "\n\n\rSwitching to TRNG and generating a symmetric key through an RNG operation\r";
static const char
    promptLoadAndExport[] = "\n\n\rLoading Plaintext and getting an asset ID pair and key blob pair....\r";
static const char promptImportKeyBlob[]    = "\n\n\rImporting key blob pair and getting and asset ID pair....\r";
static const char promptEncKeyBlob[]       = "\n\n\r\tThe Encryption Asset Key Blob: ";
static const char promptDecKeyBlob[]       = "\n\n\r\tThe Decryption Asset Key Blob: ";
static const char promptEncAsset[]         = "\n\n\r\tThe Encryption Asset ID: ";
static const char promptDecAsset[]         = "\n\n\r\tThe Decryption Asset ID: ";
static const char promptUserInputMessage[] = "\n\n\rEnter a message to Encrypt:\n\n\r";
static const char promptEncrypting[]       = "\n\n\rEncrypting input text via AES-CCM....\r";
static const char promptCipher[]           = "\n\n\rThe Encrypted Result: ";
static const char promptDecrypting[]       = "\n\n\rDecrypting input text via AES-CCM....\r";
static const char promptPlain[]            = "\n\n\rThe Decrypted Result: ";
static const char promptFail[]             = "\n\n\r****FAILURE!!!!!****";

/* Metadata buffers */
static uint8_t plaintextKeyMaterial[SYM_KEY_LENGTH_128] = {0};
static uint8_t aad[AAD_LENGTH]     = {0x37, 0x96, 0xcf, 0x51, 0xb8, 0x72, 0x66, 0x52, 0xa4, 0x20, 0x47,
                                      0x33, 0xb8, 0xfb, 0xb0, 0x47, 0xcf, 0x00, 0xfb, 0x91, 0xa9, 0x83,
                                      0x7e, 0x22, 0xec, 0x22, 0xb1, 0xa2, 0x68, 0xf8, 0x8e, 0x2c};
static uint8_t nonce[NONCE_LENGTH] = {0x5a, 0x8a, 0xa4, 0x85, 0xc3, 0x16, 0xe9};

static uint8_t encGenKeyBlob[KEY_BLOB_KEY_LENGTH_128]  = {0};
static uint8_t decVrfyKeyBlob[KEY_BLOB_KEY_LENGTH_128] = {0};

static uint32_t encGenKeyAssetId  = 0U;
static uint32_t decVrfyKeyAssetId = 0U;

static char inputMessage[MSG_MAX_LENGTH];
static char plaintextBuffer[MSG_MAX_LENGTH] = {0};
static uint8_t cipherBuffer[MSG_MAX_LENGTH] = {0};
static uint8_t outputMac[MAC_LENGTH]        = {0};

/* Metadata variables */
UART2_Handle uart2Handle;

static bool isKeyGeneratedInternally = false;
static uint16_t inputMessageLength   = 0;

HSMBareMetal_AssetPairStruct keyAssetIdPair;
HSMBareMetal_AssetPairKeyBlobStruct keyBlobPair;
HSMBareMetal_RNGOperationStruct rngOperationStruct;
HSMBareMetal_AssetOperationStruct assetOperationStruct;
HSMBareMetal_AESOperationStruct AESOperationStruct;

/* Static API forward declarations */
static void keyBlobBareMetal_generateSymmetricKey(void);
static void keyBlobBareMetal_loadPlaintextAndExportKeyBlob(void);
static void keyBlobBareMetal_importKeyBlob(void);
static void keyBlobBareMetal_AESEncrypt(void);
static void keyBlobBareMetal_AESDecrypt(void);

static void toggleRedLEDInLoop(void);
static void displayFailureToUser(UART2_Handle handle);
static void displaySuccessToUser(void);
static void printPrompt(UART2_Handle handle, const char *prompt, const uint8_t promptLength);
static void printToConsole(UART2_Handle handle,
                           const char *prompt,
                           const uint8_t promptLength,
                           uint8_t *msg,
                           uint8_t msgLength);
static void printPlaintext(UART2_Handle handle,
                           const char *prompt,
                           const uint8_t promptLength,
                           uint8_t *msg,
                           uint8_t msgLength);
static void readInputFromUser(UART2_Handle handle, const char *prompt, const uint8_t promptLength);

/*
 *  ======== toggleRedLEDInLoop ========
 */
static void toggleRedLEDInLoop(void)
{
    /* 1 second delay */
    uint32_t time = 1;

    while (1)
    {
        sleep(time);
        GPIO_toggle(CONFIG_GPIO_LED_0);
    }
}

/*
 *  ======== displayFailureToUser ========
 */
static void displayFailureToUser(UART2_Handle handle)
{
    if (handle != NULL)
    {
        printPrompt(handle, promptFail, strlen(promptFail));
    }

    toggleRedLEDInLoop();
}

/*
 *  ======== displaySuccessToUser ========
 */
static void displaySuccessToUser(void)
{
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);
}

/*
 *  ======== printPrompt ========
 */
static void printPrompt(UART2_Handle handle, const char *prompt, const uint8_t promptLength)
{
    if (handle == NULL)
    {
        return;
    }

    /* Print prompt */
    UART2_write(handle, prompt, promptLength, NULL);
}

/*
 *  ======== printToConsole ========
 */
static void printToConsole(UART2_Handle handle,
                           const char *prompt,
                           const uint8_t promptLength,
                           uint8_t *msg,
                           uint8_t msgLength)
{
    char formattedMsg[MSG_MAX_LENGTH] = {0};
    uint32_t i;

    if (handle == NULL)
    {
        return;
    }

    /* Format the message as printable hex */
    for (i = 0; i < msgLength; i++)
    {
        sprintf(formattedMsg + (i * 2), "%02X", *(msg + i));
    }

    /* Print prompt */
    printPrompt(handle, prompt, promptLength);
    /* Print result */
    UART2_write(handle, formattedMsg, strlen(formattedMsg), NULL);
}

/*
 *  ======== printPlaintext ========
 */
static void printPlaintext(UART2_Handle handle,
                           const char *prompt,
                           const uint8_t promptLength,
                           uint8_t *msg,
                           uint8_t msgLength)
{
    /* Print prompt */
    printPrompt(handle, prompt, promptLength);
    /* Print result */
    UART2_write(handle, msg, msgLength, NULL);
}

/*
 *  ======== readInputFromUser ========
 */
static void readInputFromUser(UART2_Handle handle, const char *prompt, const uint8_t promptLength)
{
    uint8_t input;

    /* Print prompt */
    printPrompt(uart2Handle, prompt, promptLength);

    /* Reset message length */
    inputMessageLength = 0;

    /* Read in from the console until carriage-return is detected */
    while (1)
    {
        UART2_read(uart2Handle, &input, 1, NULL);

        /* If not carriage-return, echo input and continue to read */
        if (input != 0x0D)
        {
            UART2_write(uart2Handle, &input, 1, NULL);
        }
        else
        {
            break;
        }
        /* Store the received message */
        inputMessage[inputMessageLength] = input;
        inputMessageLength++;

        /* Check if message buffer is full */
        if (inputMessageLength == MSG_MAX_LENGTH)
        {
            /* We can't read in any more data, continue on to perform the encryption/decryption */
            break;
        }
    }
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    int_fast16_t result;
    UART2_Params uart2Params;
    uint8_t input;

    GPIO_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF);

    /* Open UART for console output */
    UART2_Params_init(&uart2Params);
    uart2Params.readReturnMode = UART2_ReadReturnMode_FULL;

    uart2Handle = UART2_open(CONFIG_UART2_0, &uart2Params);

    if (!uart2Handle)
    {
        /* UART2_open() failed.
         * Toggle LED0 in an infinite loop.
         */
        displayFailureToUser(uart2Handle);
    }

    keyAssetIdPair.encGenKeyAssetID  = &encGenKeyAssetId;
    keyAssetIdPair.decVrfyKeyAssetID = &decVrfyKeyAssetId;

    keyBlobPair.encGenKeyBlob  = (uint8_t *)encGenKeyBlob;
    keyBlobPair.decVrfyKeyBlob = (uint8_t *)decVrfyKeyBlob;

    result = HSMBareMetal_init();

    if (result != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure
         * HSMBareMetal_init() failed.
         * Toggle LED0 in an infinite loop.
         */
        displayFailureToUser(uart2Handle);
    }

    result = HSMBareMetal_provisionHUK();

    if (result == HSMBAREMETAL_STATUS_ERROR)
    {
        /* Print Failure
         * HSMBareMetal_provisionHUK() failed.
         * Toggle LED0 in an infinite loop.
         */
        displayFailureToUser(uart2Handle);
    }
    else if (result == HSMBAREMETAL_STATUS_HUK_ALREADY_PROVISIONED)
    {
        /* This means the device has already been provisioned with an HUK. */
    }

    /* Prompt startup message */
    printPrompt(uart2Handle, promptStartup, strlen(promptStartup));

    /* Loop forever */
    while (1)
    {
        /* Print separator */
        printPrompt(uart2Handle, promptSeparator, strlen(promptSeparator));

        printPrompt(uart2Handle, promptKeyGenerationDescription, strlen(promptKeyGenerationDescription));

        /* Print prompt */
        printPrompt(uart2Handle, promptKeyGenerationChoice, strlen(promptKeyGenerationChoice));

        UART2_read(uart2Handle, &input, 1, NULL);

        UART2_write(uart2Handle, &input, 1, NULL);

        /* If input is Y or y, then change the key type to */
        if ((input == 0x59) || (input == 0x79))
        {
            isKeyGeneratedInternally = true;
        }
        else
        {
            /* Print prompt */
            printPrompt(uart2Handle, promptRNGKeyGeneration, strlen(promptRNGKeyGeneration));

            keyBlobBareMetal_generateSymmetricKey();
        }

        /* Print prompt */
        printPrompt(uart2Handle, promptLoadAndExport, strlen(promptLoadAndExport));

        /* Load plaintext key and export a key blob and return asset ID pairs */
        keyBlobBareMetal_loadPlaintextAndExportKeyBlob();

        HSMBareMetal_freeAssetPair(keyAssetIdPair);

        /* Print prompt */
        printPrompt(uart2Handle, promptImportKeyBlob, strlen(promptImportKeyBlob));

        keyBlobBareMetal_importKeyBlob();

        /* Read user input text for encryption */
        readInputFromUser(uart2Handle, promptUserInputMessage, strlen(promptUserInputMessage));

        /* Print prompt */
        printPrompt(uart2Handle, promptEncrypting, strlen(promptEncrypting));

        keyBlobBareMetal_AESEncrypt();

        /* Print prompt */
        printPrompt(uart2Handle, promptDecrypting, strlen(promptDecrypting));

        keyBlobBareMetal_AESDecrypt();

        HSMBareMetal_freeAssetPair(keyAssetIdPair);

        displaySuccessToUser();
    }
}

/*
 *  ======== keyBlobBareMetal_generateSymmetricKey ========
 */
static void keyBlobBareMetal_generateSymmetricKey(void)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    status = HSMBareMetal_RNGSwitchNRBGMode(NRBG_TYPE_TRNG);

    if (status == HSMBAREMETAL_STATUS_ERROR)
    {
        /* Print Failure
         * HSMBareMetal_RNGSwitchNRBGMode() failed.
         * Toggle LED0 in an infinite loop.
         */
        displayFailureToUser(uart2Handle);
    }

    HSMBareMetal_RNGOperation_init(&rngOperationStruct);

    rngOperationStruct.random       = plaintextKeyMaterial;
    rngOperationStruct.randomLength = SYM_KEY_LENGTH_128;
    rngOperationStruct.entropyType  = ENTROPY_TYPE_DRBG;

    status = HSMBareMetal_RNGOperation(&rngOperationStruct);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure
         * HSMBareMetal_RNGOperation() failed.
         * Toggle LED0 in an infinite loop.
         */
        displayFailureToUser(uart2Handle);
    }
}

/*
 *  ======== keyBlobBareMetal_loadPlaintextAndExportKeyBlob ========
 */
static void keyBlobBareMetal_loadPlaintextAndExportKeyBlob(void)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    HSMBareMetal_AssetOperation_init(&assetOperationStruct);

    if (isKeyGeneratedInternally)
    {
        assetOperationStruct.isKeyGenerated = true;
    }
    else
    {
        assetOperationStruct.key = plaintextKeyMaterial;
    }

    assetOperationStruct.keyLength        = SYM_KEY_LENGTH_128;
    assetOperationStruct.keyBlobs         = keyBlobPair;
    assetOperationStruct.keyAssetIDs      = keyAssetIdPair;
    assetOperationStruct.algorithm        = HSMBareMetal_OPERATION_ALGO_AES;
    assetOperationStruct.operationType    = HSMBareMetal_ASSET_OPERATION_TYPE_LOAD_EXPORT_KEY_BLOB;
    assetOperationStruct.aesOperationMode = HSMBareMetal_AES_MODE_CCM;

    status = HSMBareMetal_AssetOperation(&assetOperationStruct);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure
         * HSMBareMetal_AssetOperation() failed.
         * Toggle LED0 in an infinite loop.
         */
        displayFailureToUser(uart2Handle);
    }

    printToConsole(uart2Handle, promptEncAsset, strlen(promptEncAsset), (uint8_t *)&encGenKeyAssetId, sizeof(uint32_t));

    printToConsole(uart2Handle,
                   promptEncKeyBlob,
                   strlen(promptEncKeyBlob),
                   (uint8_t *)encGenKeyBlob,
                   KEY_BLOB_KEY_LENGTH_128);

    printToConsole(uart2Handle,
                   promptDecAsset,
                   strlen(promptDecAsset),
                   (uint8_t *)&decVrfyKeyAssetId,
                   sizeof(uint32_t));

    printToConsole(uart2Handle,
                   promptDecKeyBlob,
                   strlen(promptDecKeyBlob),
                   (uint8_t *)encGenKeyBlob,
                   KEY_BLOB_KEY_LENGTH_128);
}

/*
 *  ======== keyBlobBareMetal_importKeyBlob ========
 */
static void keyBlobBareMetal_importKeyBlob(void)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    HSMBareMetal_AssetOperation_init(&assetOperationStruct);

    assetOperationStruct.keyLength        = SYM_KEY_LENGTH_128;
    assetOperationStruct.keyBlobs         = keyBlobPair;
    assetOperationStruct.keyAssetIDs      = keyAssetIdPair;
    assetOperationStruct.algorithm        = HSMBareMetal_OPERATION_ALGO_AES;
    assetOperationStruct.operationType    = HSMBareMetal_ASSET_OPERATION_TYPE_LOAD_IMPORT_KEY_BLOB;
    assetOperationStruct.aesOperationMode = HSMBareMetal_AES_MODE_CCM;

    status = HSMBareMetal_AssetOperation(&assetOperationStruct);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure
         * HSMBareMetal_AssetOperation() failed.
         * Toggle LED0 in an infinite loop.
         */
        displayFailureToUser(uart2Handle);
    }

    printToConsole(uart2Handle, promptEncAsset, strlen(promptEncAsset), (uint8_t *)&encGenKeyAssetId, sizeof(uint32_t));

    printToConsole(uart2Handle,
                   promptDecAsset,
                   strlen(promptDecAsset),
                   (uint8_t *)&decVrfyKeyAssetId,
                   sizeof(uint32_t));
}

/*
 *  ======== keyBlobBareMetal_AESEncrypt ========
 */
static void keyBlobBareMetal_AESEncrypt()
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    HSMBareMetal_AESOperation_init(&AESOperationStruct);

    AESOperationStruct.keyAssetID         = encGenKeyAssetId;
    AESOperationStruct.keyLength          = SYM_KEY_LENGTH_128;
    AESOperationStruct.isKeyInAssetStore  = HSMBareMetal_KEY_INPUT_ASSETSTORE;
    AESOperationStruct.aad                = (uint8_t *)aad;
    AESOperationStruct.aadLength          = AAD_LENGTH;
    AESOperationStruct.input              = (uint8_t *)inputMessage;
    AESOperationStruct.inputLength        = inputMessageLength;
    AESOperationStruct.nonce              = (uint8_t *)nonce;
    AESOperationStruct.nonceLength        = NONCE_LENGTH;
    AESOperationStruct.mac                = (uint8_t *)outputMac;
    AESOperationStruct.macLength          = MAC_LENGTH;
    AESOperationStruct.output             = (uint8_t *)cipherBuffer;
    AESOperationStruct.operationMode      = HSMBareMetal_AES_MODE_CCM;
    AESOperationStruct.operationDirection = HSMBareMetal_OPERATION_DIR_ENC_GEN;

    status = HSMBareMetal_AESOperation(&AESOperationStruct);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure
         * HSMBareMetal_AESOperation() failed.
         * Toggle LED0 in an infinite loop.
         */
        displayFailureToUser(uart2Handle);
    }

    printToConsole(uart2Handle, promptCipher, strlen(promptCipher), (uint8_t *)&cipherBuffer, inputMessageLength);
}

/*
 *  ======== keyBlobBareMetal_AESDecrypt ========
 */
static void keyBlobBareMetal_AESDecrypt()
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    HSMBareMetal_AESOperation_init(&AESOperationStruct);

    AESOperationStruct.keyAssetID         = decVrfyKeyAssetId;
    AESOperationStruct.keyLength          = SYM_KEY_LENGTH_128;
    AESOperationStruct.isKeyInAssetStore  = HSMBareMetal_KEY_INPUT_ASSETSTORE;
    AESOperationStruct.aad                = (uint8_t *)aad;
    AESOperationStruct.aadLength          = AAD_LENGTH;
    AESOperationStruct.input              = (uint8_t *)cipherBuffer;
    AESOperationStruct.inputLength        = inputMessageLength;
    AESOperationStruct.nonce              = (uint8_t *)nonce;
    AESOperationStruct.nonceLength        = NONCE_LENGTH;
    AESOperationStruct.mac                = (uint8_t *)outputMac;
    AESOperationStruct.macLength          = MAC_LENGTH;
    AESOperationStruct.output             = (uint8_t *)plaintextBuffer;
    AESOperationStruct.operationMode      = HSMBareMetal_AES_MODE_CCM;
    AESOperationStruct.operationDirection = HSMBareMetal_OPERATION_DIR_DEC_VRFY;

    status = HSMBareMetal_AESOperation(&AESOperationStruct);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure
         * HSMBareMetal_AESOperation() failed.
         * Toggle LED0 in an infinite loop.
         */
        displayFailureToUser(uart2Handle);
    }

    printPlaintext(uart2Handle, promptPlain, strlen(promptCipher), (uint8_t *)plaintextBuffer, inputMessageLength);
}
