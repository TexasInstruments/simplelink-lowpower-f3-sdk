/******************************************************************************

 @file  sign_util.h

 @brief This module contains utility functions for sign verification

 Group: CMCU
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2025, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

#ifndef SIGN_UTIL_H
#define SIGN_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 *                                          Includes
 */

#include "stdbool.h"
#include <ti/devices/DeviceFamily.h>
#include "oad_image_header.h"
#include DeviceFamily_constructPath(driverlib/hapi.h)


/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * MACROS
 */

/*! Length of Sign */
#define SECURE_FW_SIGN_LEN 32

/*!< Success Return Code      */
#define SECURE_FW_STATUS_SUCCESS                              0
/*! Failure return code */
#define SECURE_FW_STATUS_FAIL                                 1
/*!< Illegal parameter        */
#define SECURE_FW_STATUS_ILLEGAL_PARAM                       -1
/*! Invalid ECC Signature         */
#define SECURE_FW_ECC_STATUS_INVALID_SIGNATURE             0x5A
/*! ECC Signature Successfully Verified  */
#define SECURE_FW_ECC_STATUS_VALID_SIGNATURE               0xA5

/* ECC Window Size.  Determines speed and workzone size of ECC operations.
 Recommended setting is 3. */
#define SECURE_FW_ECC_WINDOW_SIZE                3

/* Key size in uint32_t blocks */
#define SECURE_FW_ECC_UINT32_BLK_LEN(len)        (((len) + 3) / 4)

/* Offset value for number of bytes occupied by length field */
#define SECURE_FW_ECC_KEY_OFFSET                 4

/* Offset of Key Length field */
#define SECURE_FW_ECC_KEY_LEN_OFFSET             0

/* Total buffer size */
#define SECURE_FW_ECC_BUF_TOTAL_LEN(len)         ((len) + SECURE_FW_ECC_KEY_OFFSET)

/*!
 * ECC Workzone length in bytes for NIST P-256 key and shared secret generation.
 * For use with ECC Window Size 3 only.  Used to store intermediary values in
 * ECC calculations.
 */
#define BIM_ECC_NIST_P256_WORKZONE_LEN_IN_BYTES          1100

#ifndef SECURE_FW_ECC_NIST_P256_WORKZONE_LEN_IN_BYTES
    #define SECURE_FW_ECC_NIST_P256_WORKZONE_LEN_IN_BYTES (BIM_ECC_NIST_P256_WORKZONE_LEN_IN_BYTES)
#endif

/*!
 * ECC key length in bytes for NIST P-256 keys.
 */
#define SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES                32

/* Cert element length for ECC- size of secure_fw_cert_element_t*/
#define SECURE_CERT_LENGTH    sizeof(certElement_t)
#define SECURE_CERT_OPTIONS   0x0000
#define SECURE_SIGN_TYPE          1

/*******************************************************************************
 * Typedefs
 */


/*! ECC public key pair */
typedef struct {
  uint8_t pubKeyX[32];
  uint8_t pubKeyY[32];
} eccKey_t;

/*! Cert Payload */
typedef union {
  eccKey_t eccKey;
  uint8_t  aesKey[2];
} certPayload_t;

/*! Cert Element */
TYPEDEF_STRUCT_PACKED
{
  uint8_t  version;
  uint8_t  len;
  uint16_t options;
  uint8_t signerInfo[8];
  certPayload_t certPayload;
} certElement_t;



uint8_t verifyCertElement(uint8_t *signerInfo);
uint8_t *computeSha2Hash(uint32_t imgStartAddr, uint8_t *SHABuff,
                                uint16_t SHABuffLen, bool useExtFl);

uint8_t  bimVerifyImage_ecc(const uint8_t *publicKey_x, const uint8_t *publicKey_y,
                                uint8_t *hash, uint8_t *sign1, uint8_t *sign2,
                                void* unused1, void* unused2);


#ifdef __cplusplus
}
#endif

#endif /* SIGN_UTIL_H */
