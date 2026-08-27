/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 *  ======== aesccmBareMetal.c ========
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
#define KEY_LENGTH     16
#define MAX_MSG_LENGTH 256
#define AAD_LENGTH     32
#define NONCE_LENGTH   7
#define MAC_LENGTH     16

/* UART pre-formatted strings */
static const char promptStartup[]   = "\n\n\rBare Metal AES operations via the Hardware Security Module (HSM) Demo\r";
static const char promptSeparator[] = "\n\n\r**********************************************************************\r";
static const char promptEnter[]     = "\n\n\rEnter a message to encrypt:\n\n\r";
static const char promptKeyChoice[] = "\n\n\rStore Key in HSM Asset Store (Y/N): ";
static const char promptCipher[]    = "\n\n\rThe Encrypted Result: ";
static const char promptMac[]       = "\n\n\rThe MAC Result: ";
static const char promptEncAsset[]  = "\n\n\rThe Encryption Asset ID: ";
static const char promptDecAsset[]  = "\n\n\rThe Decryption Asset ID: ";
static const char promptPlain[]     = "\n\n\rThe Decrypted Result: ";
static const char promptFail[]      = "\n\n\r****FAILURE!!!!!****";

/* Message buffers */
static uint8_t key[KEY_LENGTH] =
    {0xf9, 0xfd, 0xca, 0x4a, 0xc6, 0x4f, 0xe7, 0xf0, 0x14, 0xde, 0x0f, 0x43, 0x03, 0x9c, 0x75, 0x71};
static uint8_t aad[AAD_LENGTH]     = {0x37, 0x96, 0xcf, 0x51, 0xb8, 0x72, 0x66, 0x52, 0xa4, 0x20, 0x47,
                                      0x33, 0xb8, 0xfb, 0xb0, 0x47, 0xcf, 0x00, 0xfb, 0x91, 0xa9, 0x83,
                                      0x7e, 0x22, 0xec, 0x22, 0xb1, 0xa2, 0x68, 0xf8, 0x8e, 0x2c};
static uint8_t nonce[NONCE_LENGTH] = {0x5a, 0x8a, 0xa4, 0x85, 0xc3, 0x16, 0xe9};

static char formattedMsg[MAX_MSG_LENGTH]    = {0};
static char plaintextBuffer[MAX_MSG_LENGTH] = {0};
static uint8_t cipherBuffer[MAX_MSG_LENGTH] = {0};
static uint8_t outputMac[MAC_LENGTH]        = {0};

UART2_Handle uart2Handle;
HSMBareMetal_AssetOperationStruct assetOperationStruct;
HSMBareMetal_AESOperationStruct AESOperationStruct;
HSMBareMetal_KeyInput AESKeyInputType = HSMBareMetal_KEY_INPUT_PLAINTEXT;

static uint32_t encKeyAssetId = 0U;
static uint32_t decKeyAssetId = 0U;
static char inputMessage[MAX_MSG_LENGTH];
static uint16_t inputMessageLength = 0;

/* Helper Functions defines */
static void aesccmBareMetal_createAssets();
static void aesccmBareMetal_freeAssets();
static void aesccmBareMetal_AESEncrypt();
static void aesccmBareMetal_AESDecrypt();

static void toggleLEDInLoop(void)
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
 *  ======== printPlaintext ========
 */
static void printPlaintext(UART2_Handle handle, uint8_t *msg, uint8_t msgLength)
{
    /* Print prompt */
    UART2_write(handle, promptPlain, strlen(promptPlain), NULL);
    /* Print result */
    UART2_write(handle, plaintextBuffer, inputMessageLength, NULL);
}

/*
 *  ======== printCipher ========
 */
static void printCipher(UART2_Handle handle, uint8_t *msg, uint8_t msgLength)
{
    uint32_t i;

    /* Format the message as printable hex */
    for (i = 0; i < msgLength; i++)
    {
        sprintf(formattedMsg + (i * 2), "%02X", *(msg + i));
    }

    /* Print prompt */
    UART2_write(handle, promptCipher, strlen(promptCipher), NULL);
    /* Print result */
    UART2_write(handle, formattedMsg, strlen(formattedMsg), NULL);
}

/*
 *  ======== printMac ========
 */
static void printMac(UART2_Handle handle, uint8_t *msg)
{
    uint32_t i;

    /* Format the message as printable hex */
    for (i = 0; i < MAC_LENGTH; i++)
    {
        sprintf(formattedMsg + (i * 2), "%02X", *(msg + i));
    }

    /* Print prompt */
    UART2_write(handle, promptMac, strlen(promptMac), NULL);
    /* Print result */
    UART2_write(handle, formattedMsg, strlen(formattedMsg), NULL);
}

/*
 *  ======== printEncAssetID ========
 */
static void printEncAssetID(UART2_Handle handle)
{
    uint32_t i;
    uint8_t *msg = (uint8_t *)&encKeyAssetId;

    /* Format the message as printable hex */
    for (i = 0; i < sizeof(uint32_t); i++)
    {
        sprintf(formattedMsg + (i * 2), "%02X", *(msg + i));
    }

    /* Print prompt */
    UART2_write(handle, promptEncAsset, strlen(promptEncAsset), NULL);
    /* Print result */
    UART2_write(handle, formattedMsg, strlen(formattedMsg), NULL);
}

/*
 *  ======== printDecAssetID ========
 */
static void printDecAssetID(UART2_Handle handle)
{
    uint32_t i;
    uint8_t *msg = (uint8_t *)&decKeyAssetId;

    /* Format the message as printable hex */
    for (i = 0; i < sizeof(uint32_t); i++)
    {
        sprintf(formattedMsg + (i * 2), "%02X", *(msg + i));
    }

    /* Print prompt */
    UART2_write(handle, promptDecAsset, strlen(promptDecAsset), NULL);
    /* Print result */
    UART2_write(handle, formattedMsg, strlen(formattedMsg), NULL);
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

    /* Open UART for console output */
    UART2_Params_init(&uart2Params);
    uart2Params.readReturnMode = UART2_ReadReturnMode_FULL;

    uart2Handle = UART2_open(CONFIG_UART2_0, &uart2Params);

    if (!uart2Handle)
    {
        /* UART2_open() failed.
         * Toggle LED0 in an infinite loop.
         */
        toggleLEDInLoop();
    }

    result = HSMBareMetal_init();

    if (result != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure */
        UART2_write(uart2Handle, promptFail, strlen(promptFail), NULL);

        /* HSMBareMetal_init() failed.
         * Toggle LED0 in an infinite loop.
         */
        toggleLEDInLoop();
    }

    /* Prompt startup message */
    UART2_write(uart2Handle, promptStartup, strlen(promptStartup), NULL);

    /* Loop forever */
    while (1)
    {
        /* Print separator */
        UART2_write(uart2Handle, promptSeparator, strlen(promptSeparator), NULL);

        /* Print prompt */
        UART2_write(uart2Handle, promptEnter, strlen(promptEnter), NULL);

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
            if (inputMessageLength == 256U)
            {
                /* We can't read in any more data, continue on to perform the encryption/decryption */
                break;
            }
        }

        /* Print key choice prompt */
        UART2_write(uart2Handle, promptKeyChoice, strlen(promptKeyChoice), NULL);

        UART2_read(uart2Handle, &input, 1, NULL);

        UART2_write(uart2Handle, &input, 1, NULL);

        /* If input is Y or y, then change the key type to */
        if ((input == 0x59) || (input == 0x79))
        {
            AESKeyInputType = HSMBareMetal_KEY_INPUT_ASSETSTORE;
            aesccmBareMetal_createAssets();
        }
        else
        {
            AESKeyInputType = HSMBareMetal_KEY_INPUT_PLAINTEXT;
        }

        /* Do encryption operation. */
        aesccmBareMetal_AESEncrypt();

        /* Print out the ciphered result */
        printCipher(uart2Handle, cipherBuffer, inputMessageLength);

        /* Print out the mac result */
        printMac(uart2Handle, outputMac);

        /* Do decryption operation. */
        aesccmBareMetal_AESDecrypt();

        printPlaintext(uart2Handle, (uint8_t *)plaintextBuffer, inputMessageLength);

        if (AESKeyInputType == HSMBareMetal_KEY_INPUT_ASSETSTORE)
        {
            aesccmBareMetal_freeAssets();
        }
    }
}

static void aesccmBareMetal_createAssets()
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    HSMBareMetal_AssetOperation_init(&assetOperationStruct);

    assetOperationStruct.keyAssetIDs.encGenKeyAssetID  = &encKeyAssetId;
    assetOperationStruct.keyAssetIDs.decVrfyKeyAssetID = &decKeyAssetId;
    assetOperationStruct.key                           = NULL;
    assetOperationStruct.isKeyGenerated                = true;
    assetOperationStruct.keyLength                     = KEY_LENGTH;
    assetOperationStruct.algorithm                     = HSMBareMetal_OPERATION_ALGO_AES;
    assetOperationStruct.aesOperationMode              = HSMBareMetal_AES_MODE_CCM;

    status = HSMBareMetal_AssetOperation(&assetOperationStruct);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure */
        UART2_write(uart2Handle, promptFail, strlen(promptFail), NULL);

        /* HSMBareMetal_AESOperation() failed.
         * Toggle LED0 in an infinite loop.
         */
        toggleLEDInLoop();
    }

    printEncAssetID(uart2Handle);

    printDecAssetID(uart2Handle);
}

static void aesccmBareMetal_freeAssets()
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    status = HSMBareMetal_freeKeyAsset(&encKeyAssetId);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure */
        UART2_write(uart2Handle, promptFail, strlen(promptFail), NULL);

        /* HSMBareMetal_AESOperation() failed.
         * Toggle LED0 in an infinite loop.
         */
        toggleLEDInLoop();
    }

    status = HSMBareMetal_freeKeyAsset(&decKeyAssetId);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure */
        UART2_write(uart2Handle, promptFail, strlen(promptFail), NULL);

        /* HSMBareMetal_AESOperation() failed.
         * Toggle LED0 in an infinite loop.
         */
        toggleLEDInLoop();
    }

    encKeyAssetId = 0U;
    decKeyAssetId = 0U;
}

static void aesccmBareMetal_AESEncrypt()
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    HSMBareMetal_AESOperation_init(&AESOperationStruct);

    if (AESKeyInputType == HSMBareMetal_KEY_INPUT_ASSETSTORE)
    {
        AESOperationStruct.keyAssetID = encKeyAssetId;
    }
    else
    {
        AESOperationStruct.key = (uint8_t *)key;
    }

    AESOperationStruct.keyLength          = KEY_LENGTH;
    AESOperationStruct.isKeyInAssetStore  = AESKeyInputType;
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
        /* Print Failure */
        UART2_write(uart2Handle, promptFail, strlen(promptFail), NULL);

        /* HSMBareMetal_AESOperation() failed.
         * Toggle LED0 in an infinite loop.
         */
        toggleLEDInLoop();
    }
}

static void aesccmBareMetal_AESDecrypt()
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    HSMBareMetal_AESOperation_init(&AESOperationStruct);

    if (AESKeyInputType == HSMBareMetal_KEY_INPUT_ASSETSTORE)
    {
        AESOperationStruct.keyAssetID = decKeyAssetId;
    }
    else
    {
        AESOperationStruct.key = (uint8_t *)key;
    }

    AESOperationStruct.key                = (uint8_t *)key;
    AESOperationStruct.keyLength          = KEY_LENGTH;
    AESOperationStruct.isKeyInAssetStore  = AESKeyInputType;
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
        /* Print Failure */
        UART2_write(uart2Handle, promptFail, strlen(promptFail), NULL);

        /* HSMBareMetal_AESOperation() failed.
         * Toggle LED0 in an infinite loop.
         */
        toggleLEDInLoop();
    }
}
