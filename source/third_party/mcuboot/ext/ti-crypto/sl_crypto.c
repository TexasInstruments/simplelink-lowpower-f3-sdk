/*
 * Copyright (c) 2015-2025, Texas Instruments Incorporated
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
#include <stdint.h>
#include "ti-crypto/sl_crypto.h"
#include "mcuboot_config/mcuboot_logging.h"
#include "mcuboot_config.h"
#include "string.h"

#if defined(IS_CC13XX_CC26XX)
#include "ti/common/cc26xx/sha2/sha2_driverlib.h"
#include "ti/common/cc26xx/ecc/ECDSACC26X4_driverlib.h"
#include "ti/common/cc26xx/ecc/AESCTRCC26X4_driverlib.h"
#elif defined(IS_CC27XX)
#include "ti/common/hsm/HSMBareMetal.h"
#include "ti/common/hsm/HSMBareMetalECCUtility.h"
#elif defined(IS_CC23XX)
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/hapi.h)
#include "ti/common/ecdsa_lpf3/ecdsa_lpf3.h"
#endif

/* define macros */
#if defined(DeviceFamily_CC13X2) || defined(DeviceFamily_CC26X2)
#define ECDSA_PUB_KEY_SIZE 64
#endif
#if defined(IS_CC27XX)
#define DIGEST_MAX_LENGTH 32U
#define BUFFER_MAX_LENGTH 256U
#endif

/* global variables */
#if defined(IS_CC23XX)
static SHA256SW_Object sha256SWObject;
static SHA256SW_Handle sha256SWHandle = &sha256SWObject;
#elif defined(IS_CC27XX)
/* sl_crypto handle for CC27XX */
typedef struct
{
    uint8_t remainingBuffer[BUFFER_MAX_LENGTH];
    uint8_t intermediateDigest[DIGEST_MAX_LENGTH];
    uint32_t remainingBufferSize;
    uint32_t blockSize;
    uint32_t totalMessageLength;
    bool     sha256HSMInitial;
} SHA256_Handle;


static HSMBareMetal_CryptoKeyStruct publicKey;
static HSMBareMetal_ECCOperationStruct ecdsaHSMObject;
static SHA256_Handle sha256SlCryptoHandle;
#endif

#if defined(IS_CC13X2_CC26X2)
uint32_t eccWorkzone[SECURE_FW_ECC_NIST_P256_WORKZONE_LEN_IN_BYTES + SECURE_FW_ECC_BUF_TOTAL_LEN(SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES)*5] = {0};
#endif

/* extern */
#if defined(IS_CC13X2_CC26X2)
extern uint8_t ECDSA_verif(uint32_t *, uint32_t *, uint32_t *, uint32_t *, uint32_t *);
#endif
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
extern AESCTR_OneStepOperation operation_g;
extern CryptoKey_Plaintext aesKey_g;
#endif

/* Utility functions */
#if defined(IS_CC13X2_CC26X2)

/*********************************************************************
 * @fn         reverseOrder
 * @brief      Reverse the byte order and copy to output buffer
 *
 * @param      pBufIn - pointer to input buffer
 * @param      pBufOut - pointer to output buffer
 */
static void reverseOrder(const uint8_t *pBufIn,uint8_t *pBufOut)
{
  uint8_t i=0;
  for(i=0;i<SECURE_FW_SIGN_LEN;i++)
  {
    pBufOut[i] = pBufIn[SECURE_FW_SIGN_LEN-1-i];
  }
}

/*********************************************************************
 * @fn         copyBytes
 * @brief      Copy data between memory locatins
 *
 * @param      pDst - pointer to destination buffer
 * @param      pSrc - pointer to source buffer
 * @param      len  - length of data to be copied
 */
static void copyBytes(uint8_t *pDst, const uint8_t *pSrc, uint32_t len)
{
  uint32_t i;
  for(i=0; i<len; i++)
  {
      pDst[i]=pSrc[i];
  }
}
#endif

#if defined(IS_CC13X2X7_CC13X4_CC26X4)
int_fast16_t ECCParams_getUncompressedGeneratorPoint(const ECCParams_CurveParams *curveParams,
                                                     uint8_t *buffer,
                                                     size_t length)
{

    size_t paramLength = curveParams->length;
    size_t pointLength = (paramLength * 2) + 1;

    if (length < pointLength)
    {
        return -1;
    }

    /* Reverse and concatenate x and y */
    uint32_t i = 0;
    for (i = 0; i < paramLength; i++)
    {
        buffer[i + 1]               = curveParams->generatorX[paramLength - i - 1];
        buffer[i + 1 + paramLength] = curveParams->generatorY[paramLength - i - 1];
    }

    buffer[0] = 0x04;
    /* Fill the remaining buffer with 0 if needed */
    memset(buffer + pointLength, 0, length - pointLength);

    return 0;
}
#endif

/*
 *  ======== SHA2 & HMAC ========
 */
void SlCrypto_sha256_init(void)
{
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    SHA2_open();
#elif defined(IS_CC27XX)
    memset(&sha256SlCryptoHandle, 0x00, sizeof(SHA256_Handle));
    HSMBareMetal_init();
    /* Init the SHA256 sl_crypto handle*/
    sha256SlCryptoHandle.blockSize = 64U;
    sha256SlCryptoHandle.remainingBufferSize = 0U;
    sha256SlCryptoHandle.sha256HSMInitial = true;
#elif defined(IS_CC23XX)
    HapiSha256SwStart(sha256SWHandle);
#endif
}


void SlCrypto_sha256_drop(void)
{
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    SHA2_close();
#elif defined(IS_CC27XX)
    HSMBareMetal_deInit();
#endif
}
int SlCrypto_sha256_update(const void *data,
                           uint32_t data_len)
{
    int8_t rtn = -1;
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    SHA2_open();
    rtn = SHA2_addData(data, data_len);
#elif defined(IS_CC27XX)

    uint8_t inputData[BUFFER_MAX_LENGTH];
    uint8_t inputDataLen = 0U;
    HSMBareMetal_HASHOperationStruct sha256HSMObject;
    HSMBareMetal_HASHOperation_init(&sha256HSMObject);
    uint8_t digest[DIGEST_MAX_LENGTH] ={0U};

    /* if bytes in buffer is more than blockSize */
    if(sha256SlCryptoHandle.remainingBufferSize + data_len > sha256SlCryptoHandle.blockSize)
    {
        /* if there are still bytes in the handler buffer */
        if(sha256SlCryptoHandle.remainingBufferSize > 0)
        {
            /*If there is at least 1 block size of data, perform hsm operation */
            if(sha256SlCryptoHandle.remainingBufferSize >= sha256SlCryptoHandle.blockSize)
            {
                /* copy handler buffer data to the inputData buffer */
                memcpy(inputData, sha256SlCryptoHandle.remainingBuffer, sha256SlCryptoHandle.remainingBufferSize);
                inputDataLen += sha256SlCryptoHandle.remainingBufferSize;

                (void)memcpy(digest, sha256SlCryptoHandle.intermediateDigest, (DIGEST_MAX_LENGTH));
                /* perform baremetal operation */
                sha256HSMObject.input         = (uint8_t *)inputData;
                sha256HSMObject.inputLength   = inputDataLen;
                sha256HSMObject.digest        = digest;
                sha256HSMObject.operationMode = HSMBareMetal_HASH_MODE_SHA2_256;

                if(sha256SlCryptoHandle.sha256HSMInitial)
                {
                    sha256HSMObject.operationType = HSMBareMetal_HASH_TYPE_INIT_TO_CONT;
                    sha256SlCryptoHandle.sha256HSMInitial = false;
                }
                else
                {
                    sha256HSMObject.operationType = HSMBareMetal_HASH_TYPE_CONT_TO_CONT;
                }
                rtn = HSMBareMetal_HASHOperation(&sha256HSMObject);
                /* copy final digest */
                (void)memcpy(sha256SlCryptoHandle.intermediateDigest, (void *)sha256HSMObject.digest, (DIGEST_MAX_LENGTH));
                sha256SlCryptoHandle.totalMessageLength += inputDataLen;
                sha256SlCryptoHandle.remainingBufferSize = 0U;
            }
        }
        uint8_t remainingBytes = 0U;

        /* leave at least 1 blockSize in buffer */
        if (data_len > sha256SlCryptoHandle.blockSize)
        {
            inputDataLen = data_len - sha256SlCryptoHandle.blockSize;
            remainingBytes = sha256SlCryptoHandle.blockSize;
        }

        /* Is there any data left to process ? */
        if(inputDataLen > sha256SlCryptoHandle.blockSize)
        {
            /* is remaining blockSize aligned?
             * if no, copy upto the closest block aligned data for input and copy rest to buffer
             * if yes, copy the 1 blockSize of data from earlier
             */
            if(inputDataLen % sha256SlCryptoHandle.blockSize != 0U)
            {
                uint8_t leastAlignedBytes = (inputDataLen / sha256SlCryptoHandle.blockSize) * sha256SlCryptoHandle.blockSize;
                remainingBytes += inputDataLen - leastAlignedBytes;
                inputDataLen = leastAlignedBytes;
            }

            memcpy(sha256SlCryptoHandle.remainingBuffer, (uint8_t *)data + inputDataLen, remainingBytes);
            sha256SlCryptoHandle.remainingBufferSize = remainingBytes;
            memcpy(inputData, data, inputDataLen);

            (void)memcpy(digest, sha256SlCryptoHandle.intermediateDigest, (DIGEST_MAX_LENGTH));

            /* perform baremetal operation */
            sha256HSMObject.input         = (uint8_t *)inputData;
            sha256HSMObject.inputLength   = inputDataLen;
            sha256HSMObject.digest        = digest;
            sha256HSMObject.operationMode = HSMBareMetal_HASH_MODE_SHA2_256;

            if(sha256SlCryptoHandle.sha256HSMInitial)
            {
                sha256HSMObject.operationType = HSMBareMetal_HASH_TYPE_INIT_TO_CONT;
                sha256SlCryptoHandle.sha256HSMInitial = false;
            }
            else
            {
                sha256HSMObject.operationType = HSMBareMetal_HASH_TYPE_CONT_TO_CONT;
            }

            rtn = HSMBareMetal_HASHOperation(&sha256HSMObject);
            
            /* copy final digest */
            (void)memcpy(sha256SlCryptoHandle.intermediateDigest, (void *)sha256HSMObject.digest, (DIGEST_MAX_LENGTH));
            sha256SlCryptoHandle.totalMessageLength += inputDataLen;
        }

        /* copy all to buffer if there isn't enough data to process after reserving a block size */
        else
        {
            memcpy(sha256SlCryptoHandle.remainingBuffer, data, data_len);
            sha256SlCryptoHandle.remainingBufferSize = data_len;
        }
    }

    /* if bytes in buffer is the same size as blockSize or less than blockSize */
    else
    {
        uint8_t *bufferTail = &sha256SlCryptoHandle.remainingBuffer[sha256SlCryptoHandle.remainingBufferSize];
        memcpy(bufferTail, data, data_len);
        sha256SlCryptoHandle.remainingBufferSize += data_len;
        rtn = 0U;
    }
#elif defined(IS_CC23XX)
    rtn = HapiSha256SwAddData(sha256SWHandle, data, data_len);
#endif
    return rtn;
}

int SlCrypto_sha256_final(uint8_t *output)
{
    int rtn;
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    SlCrypto_sha256_init();
    rtn = SHA2_finalize(output);
    SHA2_close();
#elif defined(IS_CC27XX)

    uint8_t inputData[BUFFER_MAX_LENGTH];
    uint8_t digest[DIGEST_MAX_LENGTH] = {0U};
    uint8_t inputDataLen = 0U;
    HSMBareMetal_HASHOperationStruct sha256HSMObject;
    HSMBareMetal_HASHOperation_init(&sha256HSMObject);
    
    /* copy remaining buffer to perform final operation */
    memcpy(inputData, sha256SlCryptoHandle.remainingBuffer, sha256SlCryptoHandle.remainingBufferSize);
    inputDataLen = sha256SlCryptoHandle.remainingBufferSize;

    (void)memcpy(digest, sha256SlCryptoHandle.intermediateDigest, (DIGEST_MAX_LENGTH));
    sha256HSMObject.input         = (uint8_t *)inputData;
    sha256HSMObject.inputLength   = inputDataLen;
    sha256HSMObject.digest        = digest;
    sha256HSMObject.operationMode = HSMBareMetal_HASH_MODE_SHA2_256;
    sha256HSMObject.totalInputLength = sha256SlCryptoHandle.totalMessageLength + inputDataLen;

    if(sha256SlCryptoHandle.sha256HSMInitial)
    {
        sha256HSMObject.operationType = HSMBareMetal_HASH_TYPE_INIT_TO_FINAL;
        sha256SlCryptoHandle.sha256HSMInitial = false;
    }
    else
    {
        sha256HSMObject.operationType = HSMBareMetal_HASH_TYPE_CONT_TO_FINAL;
    }

    rtn = HSMBareMetal_HASHOperation(&sha256HSMObject);
    sha256SlCryptoHandle.totalMessageLength += inputDataLen;

    /* copy final digest */
    (void)memcpy((void *)output, (void *)sha256HSMObject.digest, (DIGEST_MAX_LENGTH));
    
#elif defined(IS_CC23XX)
    rtn = HapiSha256SwFinalize(sha256SWHandle, (uint32_t*)output);
#endif

    return rtn;
}

int SlCrypto_sha256_setupHmac(const uint8_t *key, unsigned int key_size) {
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    return (SHA2_setupHmac(key, key_size));
#endif
    return 0U;
}

int SlCrypto_sha256_finalizeHmac(uint8_t *tag) {
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    return (SHA2_finalizeHmac(tag));
#endif
    return 0U;
}


/*
 *  ======== ECDSA & ECDH ========
 */

void SlCrypto_ecdsa_p256_init(void)
{
#if defined(IS_CC13X2_CC26X2)

    /* Store client parameters into ECC ROM parameters */
    eccRom_param_p  = &NIST_Curve_P256_p;
    eccRom_param_r  = &NIST_Curve_P256_r;
    eccRom_param_a  = &NIST_Curve_P256_a;
    eccRom_param_b  = &NIST_Curve_P256_b;
    eccRom_param_Gx = &NIST_Curve_P256_Gx;
    eccRom_param_Gy = &NIST_Curve_P256_Gy;

    /* Initialize window size */
    eccRom_windowSize = SECURE_FW_ECC_WINDOW_SIZE;

#endif

#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    ECDSA_open();
#endif
#if defined(IS_CC27XX)
    HSMBareMetal_init();
    HSMBareMetal_ECCOperation_init(&ecdsaHSMObject);
#endif
}

void SlCrypto_ecdsa_p256_drop(void)
{
#if defined(IS_CC13X2_CC26X2)
    return;
#elif defined(IS_CC13X2X7_CC13X4_CC26X4)
    ECDSA_close();
#elif defined(IS_CC27XX)
    HSMBareMetal_deInit();
#endif
}

int SlCrypto_ecdsa_p256_verify(const uint8_t *pk, const uint8_t *hash, const uint8_t *sig)
{
#if defined(IS_CC13X2_CC26X2)
    uint8_t *publicKeyXBuf;
    uint8_t *publicKeyYBuf;
    uint8_t *hashBuf;
    uint8_t *sign1Buf;
    uint8_t *sign2Buf;

    /* clear the ECC work zone Buffer */
    uint32_t *eccPayloadWorkzone = eccWorkzone;
    memset(eccPayloadWorkzone, 0, sizeof(eccWorkzone));

    // Verify the hash
    // Create temp buffer used for ECDSA sign verify, it should 6*ECDSA_KEY_LEN
    uint8_t tempWorkzone[ECDSA_SHA_TEMPWORKZONE_LEN];
    memset(tempWorkzone, 0, ECDSA_SHA_TEMPWORKZONE_LEN);

    // Variables to be allocated on the tempworkzone,
    /* Split allocated memory into buffers */
    uint8_t *reversedHash = tempWorkzone;
    uint8_t *reversedPubKeyX = reversedHash + ECDSA_KEY_LEN;
    uint8_t *reversedPubKeyY = reversedPubKeyX + ECDSA_KEY_LEN;
    uint8_t *reversedSign1 = reversedPubKeyY + ECDSA_KEY_LEN;
    uint8_t *reversedSign2 = reversedSign1 + ECDSA_KEY_LEN;

    reverseOrder(hash, reversedHash);
    reverseOrder(pk, reversedPubKeyX);
    reverseOrder(pk+32, reversedPubKeyY);
    reverseOrder(sig, reversedSign1);
    reverseOrder(sig+32, reversedSign2);

    /*total memory for operation: workzone and 5 key buffers*/
    eccRom_workzone = &eccWorkzone[0];

    /* Split allocated memory into buffers */
    publicKeyXBuf = (uint8_t *)eccRom_workzone +
                 SECURE_FW_ECC_NIST_P256_WORKZONE_LEN_IN_BYTES;
    publicKeyYBuf = publicKeyXBuf +
                 SECURE_FW_ECC_BUF_TOTAL_LEN(SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);
    hashBuf =  publicKeyYBuf +
               SECURE_FW_ECC_BUF_TOTAL_LEN(SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);
    sign1Buf  = hashBuf +
             SECURE_FW_ECC_BUF_TOTAL_LEN(SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);
    sign2Buf  = sign1Buf +
             SECURE_FW_ECC_BUF_TOTAL_LEN(SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);

    /* Set length of keys in words in the first word of each buffer*/
    *((uint32_t *)&publicKeyXBuf[SECURE_FW_ECC_KEY_LEN_OFFSET]) =
      (uint32_t)(SECURE_FW_ECC_UINT32_BLK_LEN(SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES));

    *((uint32_t *)&publicKeyYBuf[SECURE_FW_ECC_KEY_LEN_OFFSET]) =
     (uint32_t)(SECURE_FW_ECC_UINT32_BLK_LEN(SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES));

    *((uint32_t *)&hashBuf[SECURE_FW_ECC_KEY_LEN_OFFSET]) =
      (uint32_t)(SECURE_FW_ECC_UINT32_BLK_LEN(SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES));

    *((uint32_t *)&sign1Buf[SECURE_FW_ECC_KEY_LEN_OFFSET]) =
      (uint32_t)(SECURE_FW_ECC_UINT32_BLK_LEN(SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES));

    *((uint32_t *)&sign2Buf[SECURE_FW_ECC_KEY_LEN_OFFSET]) =
      (uint32_t)(SECURE_FW_ECC_UINT32_BLK_LEN(SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES));

    /* Copy input key into buffer */
    copyBytes( publicKeyXBuf + SECURE_FW_ECC_KEY_OFFSET,
               reversedPubKeyX,
               SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);
    copyBytes( publicKeyYBuf + SECURE_FW_ECC_KEY_OFFSET,
               reversedPubKeyY,
               SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);
     /* copy hash into buffer */
    copyBytes( hashBuf + SECURE_FW_ECC_KEY_OFFSET,
               reversedHash,
               SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);

    copyBytes( sign1Buf + SECURE_FW_ECC_KEY_OFFSET,
               reversedSign1,
               SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);
    copyBytes( sign2Buf + SECURE_FW_ECC_KEY_OFFSET,
               reversedSign2,
               SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);


    uint8_t status = ECDSA_verif((uint32_t *)publicKeyXBuf,
                                 (uint32_t *)publicKeyYBuf,
                                 (uint32_t *)hashBuf,
                                 (uint32_t *)sign1Buf,
                                 (uint32_t *)sign2Buf);

    if (status != SECURE_FW_ECC_STATUS_VALID_SIGNATURE) {
        MCUBOOT_LOG_ERR("SlCrypto_ecdsa_p256_verify: fail");
        return -1;
    }

    return 0;
#elif defined(IS_CC13X2X7_CC13X4_CC26X4) || defined(IS_CC23XX)
    CryptoKey_Plaintext publicKey;
    ECDSA_OperationVerify operationVerify;
    int_fast16_t operationResult;

    /* Initialize the publicKey */
    uint8_t publicKeyingMaterial[2 * SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES + 1] =  {0};
    publicKeyingMaterial[0] = 0x04;
    memcpy( &publicKeyingMaterial[1], pk, SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);
    memcpy( &publicKeyingMaterial[1 + SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES],
            pk + SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES,
            SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);

    CryptoKeyPlaintext_initKey(&publicKey,
                               publicKeyingMaterial,
                               sizeof(publicKeyingMaterial));

    /* Initialize the operation */
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    operationVerify.curve           = &ECCParams_NISTP256;
#endif
    operationVerify.theirPublicKey  = &publicKey;
    operationVerify.hash            = hash;
    operationVerify.r               = sig;
    operationVerify.s               = sig + SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES;

    operationResult = ECDSA_verify(&operationVerify);

    if (operationResult != ECDSA_STATUS_SUCCESS) {
        MCUBOOT_LOG_ERR("SlCrypto_ecdsa_p256_verify: fail");
        return -1;
    }
    return 0;
#elif defined(IS_CC27XX)
    uint8_t newpk[65] = {0};
    newpk[0] = 0x04;
    memcpy(&newpk[1], pk, 64);
    HSMBareMetal_CryptoKeyPlaintext_initKey(&publicKey, (uint8_t *)&newpk, 65);

    ecdsaHSMObject.publicKey          = &publicKey;
    ecdsaHSMObject.hashDigest         = (uint8_t *)hash;
    ecdsaHSMObject.signatureR         = (uint8_t *)sig;
    ecdsaHSMObject.signatureS         = (uint8_t *)sig + 32;
    ecdsaHSMObject.operationMode      = HSMBareMetal_PK_MODE_ECDSA_VERIFY;
    ecdsaHSMObject.operationCurveType = HSMBareMetal_PK_CURVE_TYPE_SEC_P_256_R1;

    return HSMBareMetal_ECCOperation(&ecdsaHSMObject);
#else
    return -1;
#endif
}

int SlCrypto_ecdh_p256_computeSharedSecret(const uint8_t *pk, const uint8_t *sk, uint8_t *z) 
{
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    ECDH_OperationComputeSharedSecret operation;
    CryptoKey_Plaintext privateKey;
    CryptoKey_Plaintext publicKey;
    CryptoKey_Plaintext SharedKey;

    uint8_t privateKeyingMaterial[SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES] =  {0};

    // set the size to be SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES for little endian
    uint8_t publicKeyingMaterial[2* SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES+ 1] =  {0};

    memcpy(privateKeyingMaterial, sk, SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);
    CryptoKeyPlaintext_initKey(&privateKey,
                               privateKeyingMaterial,
                               sizeof(privateKeyingMaterial));

    // set the size to be SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES for little endian
    memcpy(publicKeyingMaterial, pk, 2*SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES+1);
    CryptoKeyPlaintext_initKey(&publicKey,
                               publicKeyingMaterial,
                               sizeof(publicKeyingMaterial));

    // set the size to be SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES for little endian
    CryptoKeyPlaintext_initKey(&SharedKey, z,
                               2*SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES+1);

    /* Initialize the operation */
    operation.curve           = &ECCParams_NISTP256;
    operation.myPrivateKey = &privateKey;
    operation.theirPublicKey = &publicKey;
    operation.sharedSecret = &SharedKey;
    return ECDH_computeSharedSecret(&operation);
#endif
    return -1;
}

/*
 *  ======== AESCTR ========
 */

void SlCrypto_aesctr_init(void)
{
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    AES_open();
#endif
}

void SlCrypto_aesctr_drop(void)
{
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    AES_close();
#endif
}


int SlCrypto_aesctr_setKey(const uint8_t *keyingMaterial)
{
#if defined(IS_CC13X2X7_CC13X4_CC26X4)

    //ensure the prcm peripherals are enabled
    SlCrypto_aesctr_init();

    /* init operation */
    memset(&operation_g, 0x00, sizeof(AESCTR_OneStepOperation));

    CryptoKeyPlaintext_initKey(&aesKey_g, (uint8_t *) keyingMaterial, AES_CTR_KEY_SIZE);

    /* Get the key */
    operation_g.key = &aesKey_g;

    return 0;
#endif
    return 0U;
}

int SlCrypto_aesctr_encrypt(uint8_t *counter, const uint8_t *m, uint32_t mlen, size_t blk_off, uint8_t *c)
{
#if defined(IS_CC13X2X7_CC13X4_CC26X4)
    
    //ensure the prcm peripherals are enabled
    SlCrypto_aesctr_init(); 
    operation_g.input             = m;
    operation_g.inputLength       = mlen;
    operation_g.initialCounter    = counter;
    operation_g.output            = c;

    return AESCTR_oneStepEncrypt (&operation_g);
#endif
    return 0U;
}

int SlCrypto_aesctr_decrypt(uint8_t *counter, const uint8_t *c, uint32_t clen, size_t blk_off, uint8_t *m)
{
#if defined(IS_CC13X2X7_CC13X4_CC26X4)

    //ensure the prcm peripherals are enabled
    SlCrypto_aesctr_init(); 
    operation_g.input             = c;
    operation_g.inputLength       = clen;
    operation_g.initialCounter    = counter;
    operation_g.output            = m;

    return AESCTR_oneStepDecrypt(&operation_g);
#endif
    return 0U;
}
