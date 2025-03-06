/******************************************************************************

 @file  sign_util.c

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

/*******************************************************************************
 *                                          Includes
 */

#include "string.h"
#include "ti/common/cc23xx/ecc/sign_util.h"
#include "ti/common/cc26xx/flash_interface/flash_interface.h"
#include "ti/common/flash/no_rtos/extFlash/ext_flash.h"
#include "ti/common/ecdsa_lpf3/ecdsa_lpf3.h"
#include "ti/common/cc26xx/crc/crc32.h"

#include DeviceFamily_constructPath(driverlib/sha256sw.h)


/********************************************************************
 * GLOBAL VARIABLES
 ********************************************************************/
uint32_t finalHash[(ECDSA_KEY_LEN / sizeof(uint32_t))] = {0};

/********************************************************************
 * EXTERN VARIABLES
 ********************************************************************/
extern const certElement_t _secureCertElement;

/*********************************************************************
 * GLOBAL FUNCTION REFERENCES
 ********************************************************************/
extern uint8_t ECDSA_verif(uint32_t *, uint32_t *, uint32_t *, uint32_t *,
                                 uint32_t *);

/*********************************************************************
 * FUNCTION DEFINITIONS
 ********************************************************************/


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

/*!
 Utility function to compare the content of two memory locations

 Public function defined in secure_fw.h
 */
/*********************************************************************
 * @fn         compareBytes
 * @brief      Compare the content of two memory locations
 *
 * @param      pData1 - pointer to first memory location
 * @param      pData2 - pointer to second memory location
 * @param      len  - length of data to be compared
 */

int compareBytes(uint8_t *pData1, const uint8_t *pData2, uint8_t len)
{
  uint8_t i;
  for(i=0; i<len; i++) if(pData1[i]!=pData2[i]) return (1);
  return (0);
}


/*!
 Check the validity of cert element

 Public function defined in secure_fw.h
 */
uint8_t verifyCertElement(uint8_t *signerInfo)
{
  /* read type in sign element and compare with type in cert element */
  return compareBytes(signerInfo,
                               _secureCertElement.signerInfo,8);
}

/**
* @brief Check for Security Payload
*
*  Reads through the headers in the .bin file. If a security header is found
*  the function checks to see if the header has a populated payload.
*
*
*  @param       eFlStrAddr - The start address in external flash of the binary image
*
*  @return      0  - security not found
*  @return      1  - security found
*
*/
uint8_t  bimVerifyImage_ecc(const uint8_t *publicKeyX, const uint8_t *publicKeyY,
                           uint8_t *hash, uint8_t *sign1, uint8_t *sign2,
                           void* unused1, void* unused2)
{
    uint8_t status = SECURE_FW_ECC_STATUS_INVALID_SIGNATURE;
    uint8_t keyBuffer[sizeof(eccKey_t) + 1]; // +1 for the uncompressed (0x04) prefix
    eccKey_t *eccKey = (eccKey_t*)&keyBuffer[1];
    CryptoKey_Plaintext pubKey;
    ECDSA_OperationVerify operation;

    keyBuffer[0] = 0x04; //prefix
    memcpy(eccKey->pubKeyX, publicKeyX, SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);
    memcpy(eccKey->pubKeyY, publicKeyY, SECURE_FW_ECC_NIST_P256_KEY_LEN_IN_BYTES);

    pubKey.keyMaterial = keyBuffer;
    pubKey.keyLength = sizeof(keyBuffer);

    operation.theirPublicKey = &pubKey;
    operation.hash = hash;
    operation.r = sign1;
    operation.s = sign2;

    if (ECDSA_STATUS_SUCCESS == ECDSA_verify(&operation))
    {
        status = SECURE_FW_ECC_STATUS_VALID_SIGNATURE;
    }

    return status;
}

#ifdef SECURITY /* The functions below are are only used in performing Secure OAD */

/*********************************************************************
 * @fn         computeSha2Hash
 * @brief      Computes SHA256 hash
 *
 * @param      imgStartAddr - start address of the image binary
 * @param      useExtFl - is image binary is stored on external flash
 *
 * @return     pointer to output buffer containing computed hash value
 */
uint8_t *computeSha2Hash(uint32_t imgStartAddr, uint8_t *SHABuff, uint16_t SHABuffLen, bool useExtFl)
{
    imgHdr_t *pImgHdr;

    /* Read first page of the image into the buffer. */
#ifndef BIM_ONCHIP //off-chip case
    if(!useExtFl)
    {
        CRC32_memCpy(SHABuff, (uint8_t *)(imgStartAddr), SHABuffLen);
    }
    else
    {
        extFlashRead(imgStartAddr, SHABuffLen, SHABuff);
    }
#else //on-chip case
    CRC32_memCpy(SHABuff, (uint8_t *)(imgStartAddr), SHABuffLen);
#endif

    pImgHdr = (imgHdr_t *)(SHABuff);

    if(pImgHdr->fixedHdr.len == 0)
    {
        return NULL;
    }

#ifdef BIM_DUAL_ONCHIP_IMAGE
    /* ImgVld field can be changed for switching between valid images */
    /* overwrite the imgVld field in read buffer to 0xFFFFFFFF (default) to allow re-verification of sign */
    /* oad image tool calculated the sign over a fresh image where this value was 0xFFFFFFFF */
    pImgHdr->fixedHdr.imgVld = 0xFFFFFFFF;

#ifdef BIM_RESTRICTED_ROLLBACK_VERIFY_COMMIT_IMAGE
    /* if sign is being calculated, it means the commitFlag field is not set to rejected */
    /* overwrite the commitFlag in read buffer to 0xFF (default) to allow re-verification of sign */
    /* oad image tool calculated the sign over a fresh image where this value was 0xFF */
    pImgHdr->fixedHdr.commitFlag = 0xFF;
#endif

    /* if sign is being calculated, it means the verifStatus field is not set to invalid */
    /* overwrite the verifStatus in read buffer to 0xFF (default) to allow re-verification of sign */
    /* oad image tool calculated the sign over a fresh image where this value was 0xFF */
    pImgHdr->secInfoSeg.verifStat = 0xFF;
#endif

    uint32_t addrRead = imgStartAddr + SHABuffLen;
    uint32_t secHdrLen = HDR_LEN_WITH_SECURITY_INFO;

    SHA256SW_Object sha256SWObject;
    SHA256SW_Handle sha256SWHandle = &sha256SWObject;
    SHA256SWStart(sha256SWHandle, SHA2SW_HASH_TYPE_256);
    SHA256SWAddData(sha256SWHandle, &SHABuff[12], 4);
    SHA256SWAddData(sha256SWHandle, &SHABuff[18], 47);
    SHA256SWAddData(sha256SWHandle, &SHABuff[secHdrLen], SHABuffLen - secHdrLen);

    uint32_t imgLengthLeft = pImgHdr->fixedHdr.len - SHABuffLen;
    uint32_t byteToRead = SHABuffLen;

    /* Read over image pages. */
    while(imgLengthLeft > 0)
    {
        /* Read data into the next buffer */
#ifndef BIM_ONCHIP //off-chip case
        if(!useExtFl)
        {
            CRC32_memCpy(SHABuff, (uint8_t *)addrRead, byteToRead);
        }
        else
        {
            extFlashRead(addrRead, byteToRead, SHABuff);
        }
#else // on-chip case
        CRC32_memCpy(SHABuff, (uint8_t *)addrRead, byteToRead);
#endif
        SHA256SWAddData(sha256SWHandle, SHABuff, byteToRead);

        imgLengthLeft -= byteToRead;
        if(imgLengthLeft > SHABuffLen)
            byteToRead = SHABuffLen;
        else
            byteToRead = imgLengthLeft;

        addrRead += SHABuffLen;
    } /* while(imgLengthLeft > 0) */

    SHA256SWFinalize(sha256SWHandle, finalHash);

    /* check that final hash is not all zeros */
    uint8_t i, finalHashChk = 0;

    for( i = 0; i < ECDSA_KEY_LEN; i++)
    {
        finalHashChk = finalHashChk | finalHash[i] ;
    }

    if(0 == finalHashChk)
    {
        return NULL;
    }

    /* final Hash is not all zeroes but a valid result */
    return((uint8_t*)finalHash);
}

#endif /*#ifdef SECURITY */