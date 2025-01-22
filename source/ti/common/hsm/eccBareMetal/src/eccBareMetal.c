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
 *  ======== eccBareMetal.c ========
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
#define PRIVATE_KEY_LENGTH          32
#define PUBLIC_KEY_LENGTH           65
#define ECDSA_SIGNATURE_LENGTH      64
#define ECDSA_SIGNATURE_COMP_LENGTH PRIVATE_KEY_LENGTH
#define MSG_MAX_LENGTH              512
#define DIGEST_MAX_LENGTH           PRIVATE_KEY_LENGTH

/* UART pre-formatted strings */
static const char promptStartup[]          = "\n\n\rBare Metal ECC operations via the Hardware Security Module (HSM) Demo\r";
static const char promptSeparator[]        = "\n\n\r**********************************************************************\r";
static const char promptUserInputMessage[] = "\n\n\rEnter a message to Hash:\n\n\r";
static const char digestPrompt[]           = "\n\n\rThe message SHA256 digest is: ";
static const char signaturePrompt[]        = "\n\n\rThe message signature is: ";
static const char verifyPassedPrompt[]     = "\n\n\rSignature Verification Passed";
static const char promptFail[]             = "\n\n\r****FAILURE!!!!!****";

/* Message buffers */
UART2_Handle uart2Handle;

static uint8_t privateKeyMaterial[PRIVATE_KEY_LENGTH] = {0};
static uint8_t publicKeyMaterial[PUBLIC_KEY_LENGTH]   = {0};
static uint8_t ecdsaSignature[ECDSA_SIGNATURE_LENGTH] = {0};
static char inputMessage[MSG_MAX_LENGTH];
static uint8_t inputMessageDigest[DIGEST_MAX_LENGTH] = {0};

static uint16_t inputMessageLength = 0;

/* Static API forward declarations */
static void generatePrivateKey(void);
static void generatePublicKey(void);
static void hashInputMessage(void);
static void ecdsaSignUserMessage(void);
static void ecdsaVerifyUserMessage(void);

static void displayFailureToUser(UART2_Handle handle);
static void displaySuccessToUser(void);
static void printPrompt(UART2_Handle handle, const char *prompt, const uint8_t promptLength);
static void printToConsole(UART2_Handle handle,
                           const char *prompt,
                           const uint8_t promptLength,
                           uint8_t *msg,
                           uint8_t msgLength);
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

    result = HSMBareMetal_init();

    if (result != HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Print Failure */
        UART2_write(uart2Handle, promptFail, strlen(promptFail), NULL);

        /* HSMBareMetal_init() failed.
         * Toggle LED0 in an infinite loop.
         */
        displayFailureToUser(uart2Handle);
    }

    generatePrivateKey();

    generatePublicKey();

    /* Prompt startup message */
    printPrompt(uart2Handle, promptStartup, strlen(promptStartup));

    /* Loop forever */
    while (1)
    {
        /* Print separator */
        printPrompt(uart2Handle, promptSeparator, strlen(promptSeparator));

        /* Print prompt */
        printPrompt(uart2Handle, promptUserInputMessage, strlen(promptUserInputMessage));

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

        hashInputMessage();

        printToConsole(uart2Handle, digestPrompt, strlen(digestPrompt), inputMessageDigest, DIGEST_MAX_LENGTH);

        ecdsaSignUserMessage();

        printToConsole(uart2Handle, signaturePrompt, strlen(signaturePrompt), ecdsaSignature, ECDSA_SIGNATURE_LENGTH);

        ecdsaVerifyUserMessage();

        printPrompt(uart2Handle, verifyPassedPrompt, strlen(verifyPassedPrompt));

        displaySuccessToUser();
    }
}

/*
 *  ======== generatePrivateKey ========
 */
static void generatePrivateKey(void)
{
    int_fast16_t retval = HSMBAREMETAL_STATUS_ERROR;
    HSMBareMetal_RNGOperationStruct rngOperationStruct;

    retval = HSMBareMetal_RNGSwitchNRBGMode(NRBG_TYPE_TRNG);

    if (retval != HSMBAREMETAL_STATUS_SUCCESS)
    {
        displayFailureToUser(uart2Handle);
    }

    HSMBareMetal_RNGOperation_init(&rngOperationStruct);

    rngOperationStruct.random       = privateKeyMaterial;
    rngOperationStruct.randomLength = PRIVATE_KEY_LENGTH;
    rngOperationStruct.entropyType  = ENTROPY_TYPE_DRBG;

    retval = HSMBareMetal_RNGOperation(&rngOperationStruct);

    if (retval != HSMBAREMETAL_STATUS_SUCCESS)
    {
        displayFailureToUser(uart2Handle);
    }
}

/*
 *  ======== generatePublicKey ========
 */
static void generatePublicKey(void)
{
    int_fast16_t retval = HSMBAREMETAL_STATUS_ERROR;
    HSMBareMetal_CryptoKeyStruct privateKey;
    HSMBareMetal_CryptoKeyStruct publicKey;
    HSMBareMetal_ECCOperationStruct eccOperationStruct;

    HSMBareMetal_CryptoKeyPlaintext_initKey(&privateKey, privateKeyMaterial, PRIVATE_KEY_LENGTH);

    HSMBareMetal_CryptoKeyPlaintext_initKey(&publicKey, publicKeyMaterial, PUBLIC_KEY_LENGTH);

    HSMBareMetal_ECCOperation_init(&eccOperationStruct);

    eccOperationStruct.privateKey         = &privateKey;
    eccOperationStruct.publicKey          = &publicKey;
    eccOperationStruct.operationMode      = HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY;
    eccOperationStruct.operationCurveType = HSMBareMetal_PK_CURVE_TYPE_SEC_P_256_R1;

    retval = HSMBareMetal_ECCOperation(&eccOperationStruct);

    if (retval != HSMBAREMETAL_STATUS_SUCCESS)
    {
        displayFailureToUser(uart2Handle);
    }
}

/*
 *  ======== hashInputMessage ========
 */
static void hashInputMessage(void)
{
    int_fast16_t retval = HSMBAREMETAL_STATUS_ERROR;
    HSMBareMetal_HASHOperationStruct hashOperationStruct;

    HSMBareMetal_HASHOperation_init(&hashOperationStruct);

    hashOperationStruct.input         = (uint8_t *)inputMessage;
    hashOperationStruct.inputLength   = inputMessageLength;
    hashOperationStruct.digest        = inputMessageDigest;
    hashOperationStruct.operationMode = HSMBareMetal_HASH_MODE_SHA2_256;

    retval = HSMBareMetal_HASHOperation(&hashOperationStruct);

    if (retval != HSMBAREMETAL_STATUS_SUCCESS)
    {
        displayFailureToUser(uart2Handle);
    }
}

/*
 *  ======== ecdsaSignUserMessage ========
 */
static void ecdsaSignUserMessage(void)
{
    int_fast16_t retval = HSMBAREMETAL_STATUS_ERROR;
    HSMBareMetal_CryptoKeyStruct privateKey;
    HSMBareMetal_ECCOperationStruct eccOperationStruct;

    HSMBareMetal_CryptoKeyPlaintext_initKey(&privateKey, privateKeyMaterial, PRIVATE_KEY_LENGTH);

    HSMBareMetal_ECCOperation_init(&eccOperationStruct);

    eccOperationStruct.privateKey         = &privateKey;
    eccOperationStruct.hashDigest         = inputMessageDigest;
    eccOperationStruct.signatureR         = ecdsaSignature;
    eccOperationStruct.signatureS         = &ecdsaSignature[ECDSA_SIGNATURE_COMP_LENGTH];
    eccOperationStruct.operationMode      = HSMBareMetal_PK_MODE_ECDSA_SIGN;
    eccOperationStruct.operationCurveType = HSMBareMetal_PK_CURVE_TYPE_SEC_P_256_R1;

    retval = HSMBareMetal_ECCOperation(&eccOperationStruct);

    if (retval != HSMBAREMETAL_STATUS_SUCCESS)
    {
        displayFailureToUser(uart2Handle);
    }
}

/*
 *  ======== ecdsaVerifyUserMessage ========
 */
static void ecdsaVerifyUserMessage(void)
{
    int_fast16_t retval = HSMBAREMETAL_STATUS_ERROR;
    HSMBareMetal_CryptoKeyStruct publicKey;
    HSMBareMetal_ECCOperationStruct eccOperationStruct;

    HSMBareMetal_CryptoKeyPlaintext_initKey(&publicKey, publicKeyMaterial, PUBLIC_KEY_LENGTH);

    HSMBareMetal_ECCOperation_init(&eccOperationStruct);

    eccOperationStruct.publicKey          = &publicKey;
    eccOperationStruct.hashDigest         = inputMessageDigest;
    eccOperationStruct.signatureR         = ecdsaSignature;
    eccOperationStruct.signatureS         = &ecdsaSignature[ECDSA_SIGNATURE_COMP_LENGTH];
    eccOperationStruct.operationMode      = HSMBareMetal_PK_MODE_ECDSA_VERIFY;
    eccOperationStruct.operationCurveType = HSMBareMetal_PK_CURVE_TYPE_SEC_P_256_R1;

    retval = HSMBareMetal_ECCOperation(&eccOperationStruct);

    if (retval != HSMBAREMETAL_STATUS_SUCCESS)
    {
        displayFailureToUser(uart2Handle);
    }
}
