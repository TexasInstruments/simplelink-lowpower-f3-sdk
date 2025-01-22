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

#include <stdint.h>
#include <string.h>

#include "HSMBareMetal.h"
#include "HSMBareMetalECCUtility.h"

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_vims.h)
#include DeviceFamily_constructPath(inc/hw_clkctl.h)
#include DeviceFamily_constructPath(inc/hw_hsm.h)
#include DeviceFamily_constructPath(inc/hw_hsmcrypto.h)

/* Defines */

/* General Defines */
#define BIT_8                       ((uint32_t)0x00000100U)
#define BIT_14                      ((uint32_t)0x00004000U)
#define BIT_15                      ((uint32_t)0x00008000U)
#define MASK_4_BITS                 (((uint32_t)0x00000010U) - 1U)
#define MASK_5_BITS                 (((uint32_t)0x00000020U) - 1U)
#define HSM_OUTPUT_TOKEN_WORD_COUNT 20
#define HSM_TOKEN_MAX_LENGTH        64

/* Boot-related Defines */
#define HSM_BOOT_TOKEN_WORD0        0xCF000000
#define HSM_BOOT_TOKEN_WORD1        0x03725746
#define HSM_BOOT_DELAY              0xFFFFF
#define HSM_TOKEN_WORD1_OFFSET      0x4
#define HSM_CRYPTO_OFFICER_ID       0x4F5A3647
#define HSM_OUTPUT_TOKEN_ERROR_MASK 0xFF000000
#define HSM_OUTPUT_TOKEN_VRFY_ERROR 0x92000000
#define HSM_SYSINFO_TOKEN_WORD0     0x0F030000
#define HSM_SYSINFO_PATCH_MASK      0x000000FF
#define HSM_SYSINFO_MINOR_MASK      0x0000FF00
#define HSM_SYSINFO_MAJOR_MASK      0x00FF0000
#define HSM_BOOT_TOKEN_WORD_COUNT   2

/* Encryption Defines */
#define HSM_ENCRYPTION_TOKEN_WORD0      0x01000000
#define HSM_ENCRYPTION_TOKEN_WORD11_ECB 0x00000000
#define HSM_ENCRYPTION_TOKEN_WORD11_CBC 0x00000010
#define HSM_ENCRYPTION_TOKEN_WORD11_CTR 0x00000020
#define HSM_ENCRYPTION_TOKEN_WORD11_CCM 0x00000050
#define HSM_ENCRYPTION_TOKEN_WORD11_GCM 0x00000070
#define AES_BLOCK_SIZE                  16U
#define AES_BLOCK_SIZE_ALIGN            (((uint32_t)AES_BLOCK_SIZE) - 1U)
#define AES_GCM_IV_LAST_WORD            0x01000000
#define HSM_ENCRYPTION_TOKEN_WORD_COUNT 37

/* HASH Defines */
#define HSM_HASH_TOKEN_WORD0       0x12000000
#define HSM_HASH_DIGEST_LENGTH_224 28
#define HSM_HASH_DIGEST_LENGTH_256 32
#define HSM_HASH_DIGEST_LENGTH_384 48
#define HSM_HASH_DIGEST_LENGTH_512 64
#define HSM_HASH_TOKEN_WORD_COUNT  12

/* MAC Defines */
#define HSM_MAC_TOKEN_WORD0      0x13000000
#define HSM_MAC_TOKEN_WORD_COUNT 30

/* Asset Management Defines */
#define HSM_ASSET_MANAGEMENT_CREATE_TOKEN_WORD0 0x17000000
#define HSM_ASSET_MANAGEMENT_LOAD_TOKEN_WORD0   0x27000000
#define HSM_ASSET_MANAGEMENT_DELETE_TOKEN_WORD0 0x37000000
#define HSM_ASSET_MANAGEMENT_READ_TOKEN_WORD0   0x47000000
#define HSM_ASSET_LOAD_PLAINTEXT                0x08000000
#define HSM_ASSET_MANAGEMENT_WORD_COUNT         12

/* Asset Policy Defines */
#define HSM_ASSET_POLICY_SYM_HASH_MAC   0x00012801
#define HSM_ASSET_POLICY_SYM_AES_MAC    0x00022801
#define HSM_ASSET_POLICY_SYM_AES_BULK   0x00032801
#define HSM_ASSET_POLICY_SYM_AES_AUTH   0x00042801
#define HSM_ASSET_POLICY_ASYM_SIGNVRFY  0x00004001
#define HSM_ASSET_POLICY_ASYM_KEYEXCH   0x00014001
#define HSM_ASSET_POLICY_ASYM_KEYPARAMS 0x000F4401
#define HSM_ASSET_POLICY_GENERIC_DATA   0x00000401

#define HSM_ASSET_POLICY_PRIVATEDATA 0x00000800
#define HSM_ASSET_POLICY_ST_ANY      0x04000000

#define HSM_ASSET_POLICY_DIR_ENC_GEN  0x00100000
#define HSM_ASSET_POLICY_DIR_DEC_VRFY 0x00200000

#define HSM_ASSET_POLICY_AIH_SHA2_224 0x01000000
#define HSM_ASSET_POLICY_AIH_SHA2_256 0x01400000
#define HSM_ASSET_POLICY_AIH_SHA2_384 0x01800000
#define HSM_ASSET_POLICY_AIH_SHA2_512 0x01C00000

#define HSM_ASSET_POLICY_SYM_MODE_ECB     0x00000000
#define HSM_ASSET_POLICY_SYM_MODE_CBC     0x08000000
#define HSM_ASSET_POLICY_SYM_MODE_CTR     0x18000000
#define HSM_ASSET_POLICY_SYM_MODE_CCM     0x00000000
#define HSM_ASSET_POLICY_SYM_MODE_GCM     0x08000000
#define HSM_ASSET_POLICY_SYM_MODE_CMAC    0x00000000
#define HSM_ASSET_POLICY_SYM_MODE_CBC_MAC 0x08000000

#define HSM_ASSET_POLICY_ASYM_ECDH       0x00200000
#define HSM_ASSET_POLICY_ASYM_ECDSA      0x00300000
#define HSM_ASSET_POLICY_ASYM_CURVE25519 0x00400000

#define HSM_ASSET_POLICY_ASYM_SHA2_224 0x20000000
#define HSM_ASSET_POLICY_ASYM_SHA2_256 0x28000000
#define HSM_ASSET_POLICY_ASYM_SHA2_384 0x30000000
#define HSM_ASSET_POLICY_ASYM_SHA2_512 0x38000000

/* RNG Defines */
#define HSM_RNG_GET_TOKEN_WORD0           0x04000000
#define HSM_RNG_CONFIG_TOKEN_WORD0        0x14000000
#define HSM_RNG_CONFIG_RESEED_TOKEN_WORD2 0x00000001
#define HSM_RNG_TOKEN_WORD_COUNT          5

/* ECC Defines */
#define HSM_PK_TOKEN_WORD0                         0x19000000
#define HSM_PK_TOKEN_WORD2_CMD_ECC_KEYS_CHECK      0x00000001
#define HSM_PK_TOKEN_WORD2_CMD_ECDSA_SIGN          0x00000006
#define HSM_PK_TOKEN_WORD2_CMD_ECDSA_VRFY          0x00000007
#define HSM_PK_TOKEN_WORD2_CMD_ECC_GEN_PUBKEY      0x00000014
#define HSM_PK_TOKEN_WORD2_CMD_ECDH_GEN_SHRD_SCRT  0x00000016
#define HSM_PK_TOKEN_WORD2_CMD_25519_GEN_PUBKEY    0x00000028
#define HSM_PK_TOKEN_WORD2_CMD_25519_GEN_SHRD_SCRT 0x0000002A
#define HSM_PK_TOKEN_WORD_COUNT                    40

#define HSM_PK_TOKEN_WORD3_SV_SHRD_SCRT 0x80000000
#define HSM_PK_TOKEN_WORD3_EXP_DGST     0x40000000

#define HSM_RNG_RAW_DATA_BLOCK_SIZE 256
#define HSM_RNG_WORD_LENGTH         4
#define HSM_DRBG_MAX_LENGTH         (1 << 16)

/* Metadata Structures */

/*!
 *  @brief  Enum for the curve domain id supported by the driver.
 */
typedef enum
{
    HSMBareMetal_ECC_DOMAIN_ID_SEC = 0,
    HSMBareMetal_ECC_DOMAIN_ID_BRP = 1,
} HSMBareMetal_ECCDomainID;

/*!
 *  @brief  Struct containing the parameters required for ECC operations.
 */
typedef struct
{
    HSMBareMetal_ECCOperationStruct *operation;
    HSMBareMetal_CryptoKeyStruct curveParams;
    HSMBareMetal_ECCOperationCurveLength operationCurveLength;
    HSMBareMetal_ECCOperationDigestLength operationDigestLength;
    HSMBareMetal_ECCDomainID domainId;
    uint32_t privateKeyAssetId;
    uint32_t publicKeyAssetId;
    uint32_t publicDataAssetId;
    uint32_t paramAssetId;
    const uint8_t *curveParam;
    uint32_t curveParamSize;
    uint8_t output[HSM_ASYM_COMPONENT_VECTOR_LENGTH];
} HSMBareMetal_ECCOperationObject;

/* Metadata global variables */

static bool HSMBareMetal_isInitialized = false;
static uint32_t HSMBareMetal_inputToken[HSM_TOKEN_MAX_LENGTH];
static uint32_t HSMBareMetal_outputToken[HSM_TOKEN_MAX_LENGTH];

static uint8_t HSMBareMetal_rawDataPool[HSM_RNG_RAW_DATA_BLOCK_SIZE] = {0};

static HSMBareMetal_NRBGMode currentNRBGType = NRBG_TYPE_CRNG;

/* Forward declarations */
static void HSMBareMetal_writeToken(const uint32_t *token, uint32_t len);
static void HSMBareMetal_initMbox(void);
static void HSMBareMetal_enableClock(void);
static int_fast16_t HSMBareMetal_boot(void);
static int_fast16_t HSMBareMetal_writeTokenAndWaitForResults(uint32_t wordCount);
static int_fast16_t HSMBareMetal_isHSMInFatalStatus();
static int_fast16_t HSMBareMetal_isHSMfirmwareImgAccepted();
static int_fast16_t HSMBareMetal_ECCOperationGetCurveLength(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationCreateCurveParamsAsset(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationCreateAndLoadPrivateKeyAsset(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationCreateAndLoadPublicKeyAsset(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationCreatePublicDataAsset(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationCreateAllAssets(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationFreeAllAssets(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationValidateParams(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationCheckECCKeys(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationReadPublicDataAsset(uint32_t *assetId,
                                                                 uint32_t assetSize,
                                                                 uintptr_t outputAddress);
static int_fast16_t HSMBareMetal_ECCOperationInitializeOperation(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationInternal(HSMBareMetal_ECCOperationObject *object);
static int_fast16_t HSMBareMetal_ECCOperationInternalPostProcessing(HSMBareMetal_ECCOperationObject *object);

/*
 *  ======== HSMBareMetal_enableClock ========
 */
static void HSMBareMetal_enableClock(void)
{
    /* Set the clock, and wait for it to enable */
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET1) = CLKCTL_CLKENSET1_HSM_CLK_SET;
    while ((HWREG(CLKCTL_BASE + CLKCTL_O_CLKCFG1) & CLKCTL_CLKCFG1_HSM_M) == CLKCTL_CLKCFG1_HSM_CLK_DIS) {}

    /* Unlock CPUID0 and CPUID1 */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLCKOUT) = 0xFFFFFCFC;

    /* Change CPU ID to app */
    HWREG(HSM_BASE + HSM_O_CTL) = HSM_CTL_CPUIDSEL_APPID | HSM_CTL_CPUIDUNLK_UNLOCK;

    /* Lock the CPU ID to app & disable DMA firewall */
    HWREG(HSM_BASE + HSM_O_CTL) = HSM_CTL_CPUIDUNLK_LOCK | HSM_CTL_DMAFWDIS_DIS;
}

/*
 *  ======== HSMBareMetal_disableClock ========
 */
void HSMBareMetal_disableClock(void)
{
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR1) = CLKCTL_CLKENCLR1_HSM_CLK_CLR;
    /* Wait for clock to be disabled */
    while ((HWREG(CLKCTL_BASE + CLKCTL_O_CLKCFG1) & CLKCTL_CLKCFG1_HSM_M) == CLKCTL_CLKCFG1_HSM_CLK_EN) {}
}

/*
 *  ======== HSMBareMetal_isHSMInFatalStatus ========
 */
static int_fast16_t HSMBareMetal_isHSMInFatalStatus()
{
    return ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODSTA) & HSMCRYPTO_MODSTA_FATAL_M) == HSMCRYPTO_MODSTA_FATAL);
}

/*
 *  ======== HSMBareMetal_isHSMfirmwareImgAccepted ========
 */
static int_fast16_t HSMBareMetal_isHSMfirmwareImgAccepted()
{
    return ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODSTA) & HSMCRYPTO_MODSTA_FWACPTD_M) == HSMCRYPTO_MODSTA_FWACPTD);
}

/*
 *  ======== HSMBareMetal_initMbox ========
 */
static void HSMBareMetal_initMbox(void)
{
    /* Link mailbox */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1LNK_LNK | HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA);

    /* Allow non-secure/secure access (Set bits 7 and 3 to 1 if we need secure access) */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLNKID) = 0x00;

    /* Make sure CPU_ID=0 host can access mailbox 1 & 2 (no lockout) */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLCKOUT) = 0xFFFFFF77 & HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLCKOUT);
}

/*
 *  ======== HSMBareMetal_boot ========
 */
static int_fast16_t HSMBareMetal_boot(void)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;
    uint32_t delay;

    if (HSMBareMetal_isHSMInFatalStatus())
    {
        /* Do nothing. Error will be returned. */
        status = HSMBAREMETAL_STATUS_HW_ERROR;
    }
    else
    {
        if (HSMBareMetal_isHSMfirmwareImgAccepted())
        {
            /* If HSM is already booted*/
            status = HSMBAREMETAL_STATUS_SUCCESS;
        }
        else
        {
            (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_BOOT_TOKEN_WORD_COUNT);
            (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

            HSMBareMetal_inputToken[0] = HSM_BOOT_TOKEN_WORD0;
            HSMBareMetal_inputToken[1] = HSM_BOOT_TOKEN_WORD1;

            status = HSMBareMetal_writeTokenAndWaitForResults(HSM_BOOT_TOKEN_WORD_COUNT);

            if (status == HSMBAREMETAL_STATUS_SUCCESS)
            {
                for (delay = HSM_BOOT_DELAY; delay; delay--)
                {
                    if (HSMBareMetal_isHSMfirmwareImgAccepted())
                    {
                        status = HSMBAREMETAL_STATUS_SUCCESS;

                        break;
                    }
                }
            }
        }
    }

    return status;
}

/*
 *  ======== HSMBareMetal_getHSMFirmwareVersion ========
 */
int_fast16_t HSMBareMetal_getHSMFirmwareVersion(HSMBareMetal_systemInfoVersionStruct *firmwareVersionStruct)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    /* Check whether the HSM has been initialized or not. */
    if (!HSMBareMetal_isInitialized)
    {
        /* HSM IP has not been initialized. A call to HSMBareMetal_init() is required before any cryptography operation
         * is conducted. */
        return HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED;
    }
    else if (HSMBareMetal_isHSMInFatalStatus())
    {
        /* The HSM IP entered in a fatal error condition and cannot be resuscitated. */
        return HSMBAREMETAL_STATUS_HW_ERROR;
    }

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_PK_TOKEN_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    HSMBareMetal_inputToken[0] = HSM_SYSINFO_TOKEN_WORD0;

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_PK_TOKEN_WORD_COUNT);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        firmwareVersionStruct->patch = HSMBareMetal_outputToken[8] & HSM_SYSINFO_PATCH_MASK;
        firmwareVersionStruct->minor = (HSMBareMetal_outputToken[8] & HSM_SYSINFO_MINOR_MASK) >> 8;
        firmwareVersionStruct->major = (HSMBareMetal_outputToken[8] & HSM_SYSINFO_MAJOR_MASK) >> 16;
    }

    return status;
}

/*
 *  ======== HSMBareMetal_init ========
 */
int_fast16_t HSMBareMetal_init(void)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    if (!HSMBareMetal_isInitialized && !HSMBareMetal_isHSMfirmwareImgAccepted())
    {
        /* Initialize HSM clock and mailbox, then boot it */
        HSMBareMetal_enableClock();

        HSMBareMetal_initMbox();

        if (HSMBareMetal_boot() == HSMBAREMETAL_STATUS_SUCCESS)
        {
            HSMBareMetal_isInitialized = true;

            status = HSMBAREMETAL_STATUS_SUCCESS;
        }
    }
    else
    {
        HSMBareMetal_isInitialized = true;

        status = HSMBAREMETAL_STATUS_HSM_ALREADY_INITIALIZED;
    }

    return status;
}

/*
 *  ======== HSMBareMetal_deInit ========
 */
int_fast16_t HSMBareMetal_deInit(void)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(HSMBareMetal_inputToken));
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(HSMBareMetal_outputToken));

    /* Turn off the HSM IP */
    HSMBareMetal_disableClock();

    HSMBareMetal_isInitialized = false;

    return status;
}

/*
 *  ======== HSMBareMetal_writeToken ========
 * Write directly to the HSM engine.
 */
static void HSMBareMetal_writeToken(const uint32_t *token, uint32_t len)
{
    uint32_t i;

    /* Wait for mbx1_in_full to be false */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1IN) == HSMCRYPTO_MBSTA_MB1IN_FULL) {}

    for (i = 0U; i < len; i++)
    {
        HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1IN + i*4) = token[i];
    }

    /* Mark mbx1 in as full*/
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1IN_FULL;
}

/*
 *  ======== HSMBareMetal_writeTokenAndWaitForResults ========
 */
static int_fast16_t HSMBareMetal_writeTokenAndWaitForResults(uint32_t wordCount)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;
    uint32_t *output    = (uint32_t *)(HSMCRYPTO_BASE);

    /* Turn on the HSM IP and initialize the HSM mailbox */
    HSMBareMetal_enableClock();

    HSMBareMetal_initMbox();

    if (HSMBareMetal_inputToken[0] != HSM_BOOT_TOKEN_WORD0)
    {
        HSMBareMetal_inputToken[1] = HSM_CRYPTO_OFFICER_ID;
    }

    /* Write token to mbx1_in */
    HSMBareMetal_writeToken(HSMBareMetal_inputToken, wordCount);

    /* Wait for result in mbx1_out */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL) {}

    /* Copy the mailbox 1 out register into the static HSMBareMetal_outputToken buffer. */
    (void)memcpy((void *)&HSMBareMetal_outputToken, (void *)&output[0], sizeof(HSMBareMetal_outputToken));

    if ((HSMBareMetal_outputToken[0] & HSM_OUTPUT_TOKEN_ERROR_MASK) == 0)
    {
        status = HSMBAREMETAL_STATUS_SUCCESS;
    }

    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

    /* Turn off the HSM IP. */
    HSMBareMetal_disableClock();

    return status;
}

/*
 *  ======== HSMBareMetal_RNGOperation ========
 */
int_fast16_t HSMBareMetal_RNGOperation(HSMBareMetal_RNGOperationStruct *operationStruct)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;
    size_t randomLength = operationStruct->randomLength;

    /* Check whether the HSM has been initialized or not. */
    if (!HSMBareMetal_isInitialized)
    {
        /* HSM IP has not been initialized. A call to HSMBareMetal_init() is required before any cryptography operation
         * is conducted. */
        return HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED;
    }
    else if (HSMBareMetal_isHSMInFatalStatus())
    {
        /* The HSM IP entered in a fatal error condition and cannot be resuscitated. */
        return HSMBAREMETAL_STATUS_HW_ERROR;
    }

    if ((randomLength == 0) || (randomLength > HSM_DRBG_MAX_LENGTH) ||
        ((operationStruct->entropyType == ENTROPY_TYPE_DRBG) && (!HSM_IS_SIZE_MULTIPLE_OF_WORD(randomLength))) ||
        (((operationStruct->entropyType == ENTROPY_TYPE_TRNG) || (operationStruct->entropyType == ENTROPY_TYPE_CRNG)) &&
         (randomLength > HSM_RNG_RAW_DATA_BLOCK_SIZE) && (!HSM_IS_SIZE_MULTIPLE_OF_RAW_BLOCK(randomLength))))
    {
        /* Limitation:
         *  1. entropy length cannot be 0.
         *  2. entropy length less than or equal to 2^16 bytes.
         *  3. If entropy type is DRBG, the entropy length has to be 32-bit aligned (multiple of 4 Bytes).
         *  4. If entropy type is CRNG or TRNG, the entropy length can be anywhere between 1-256 bytes and multiple of
         *      256 bytes if larger than 256 bytes.
         */
        return HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS;
    }

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_RNG_TOKEN_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    HSMBareMetal_inputToken[0] = HSM_RNG_GET_TOKEN_WORD0;

    if ((operationStruct->entropyType == ENTROPY_TYPE_CRNG) || (operationStruct->entropyType == ENTROPY_TYPE_TRNG))
    {
        uint8_t numBlocks = (operationStruct->randomLength / HSM_RNG_RAW_DATA_BLOCK_SIZE);
        randomLength      = numBlocks == 0U ? 1U : numBlocks;

        HSMBareMetal_inputToken[2] = (operationStruct->entropyType << 16) | randomLength;

        if (operationStruct->randomLength < HSM_RNG_RAW_DATA_BLOCK_SIZE)
        {
            HSMBareMetal_inputToken[3] = (uintptr_t)(&HSMBareMetal_rawDataPool[0]);
        }
        else
        {
            HSMBareMetal_inputToken[3] = (uintptr_t)operationStruct->random;
        }
    }
    else
    {
        HSMBareMetal_inputToken[2] = randomLength;
        HSMBareMetal_inputToken[3] = (uintptr_t)operationStruct->random;
    }

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_RNG_TOKEN_WORD_COUNT);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        if ((operationStruct->entropyType == ENTROPY_TYPE_CRNG) || (operationStruct->entropyType == ENTROPY_TYPE_TRNG))
        {
            if (operationStruct->randomLength < HSM_RNG_RAW_DATA_BLOCK_SIZE)
            {
                /* Copy entropy from pool when request is less than 256 bytes. */
                (void)memcpy((void *)operationStruct->random,
                             (void *)&HSMBareMetal_rawDataPool[0],
                             operationStruct->randomLength);
            }
        }
    }

    return status;
}

/*
 *  ======== HSMBareMetal_RNGSwitchNRBGMode ========
 */
int_fast16_t HSMBareMetal_RNGSwitchNRBGMode(HSMBareMetal_NRBGMode NRBGMode)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    /* Check whether the HSM has been initialized or not. */
    if (!HSMBareMetal_isInitialized)
    {
        /* HSM IP has not been initialized. A call to HSMBareMetal_init() is required before any cryptography operation
         * is conducted. */
        return HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED;
    }
    else if (HSMBareMetal_isHSMInFatalStatus())
    {
        /* The HSM IP entered in a fatal error condition and cannot be resuscitated. */
        return HSMBAREMETAL_STATUS_HW_ERROR;
    }
    else if (currentNRBGType == NRBGMode)
    {
        /* The cached NRBG mode is already in mode. */
        return HSMBAREMETAL_STATUS_NRBG_ALREADY_IN_MODE;
    }

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_RNG_TOKEN_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(HSMBareMetal_outputToken));

    HSMBareMetal_inputToken[0] = HSM_RNG_GET_TOKEN_WORD0;
    HSMBareMetal_inputToken[2] = (NRBGMode << 4) | HSM_RNG_CONFIG_RESEED_TOKEN_WORD2;

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_RNG_TOKEN_WORD_COUNT);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        currentNRBGType = NRBGMode;
    }

    return status;
}

/*
 *  ======== HSMBareMetal_createKeyAssetId ========
 */
static int_fast16_t HSMBareMetal_createKeyAssetId(uint32_t assetPolicy, uint32_t keyLength, uint32_t *keyAssetId)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_ASSET_MANAGEMENT_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    HSMBareMetal_inputToken[0] = HSM_ASSET_MANAGEMENT_CREATE_TOKEN_WORD0;
    HSMBareMetal_inputToken[2] = assetPolicy;
    HSMBareMetal_inputToken[4] = keyLength;

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_ASSET_MANAGEMENT_WORD_COUNT);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Populate the user-provided keyAssetId buffer with the newly created ID. */
        *keyAssetId = HSMBareMetal_outputToken[1];
    }

    return status;
}

/*
 *  ======== HSMBareMetal_loadKeyAssetId ========
 */
static int_fast16_t HSMBareMetal_loadKeyAssetId(uint32_t *keyAssetId, uint8_t *key, uint32_t keyLength)
{
    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_ASSET_MANAGEMENT_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    HSMBareMetal_inputToken[0] = HSM_ASSET_MANAGEMENT_LOAD_TOKEN_WORD0;
    HSMBareMetal_inputToken[2] = *keyAssetId;
    HSMBareMetal_inputToken[3] = HSM_ASSET_LOAD_PLAINTEXT | keyLength;
    HSMBareMetal_inputToken[4] = (uintptr_t)key;

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    return HSMBareMetal_writeTokenAndWaitForResults(HSM_ASSET_MANAGEMENT_WORD_COUNT);
}

/*
 *  ======== HSMBareMetal_constructAssetPolicy ========
 */
static int_fast16_t HSMBareMetal_constructAssetPolicy(HSMBareMetal_AssetOperationStruct *operationStruct,
                                                      HSMBareMetal_operationDirection operationDirection,
                                                      uint32_t *assetPolicy)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_SUCCESS;

    /* Construct the asset policy. */
    if (operationStruct->algorithm == HSMBareMetal_OPERATION_ALGO_AES)
    {
        /* Operation is an AES operation.
         * Determine the correct usage policy
         */
        switch (operationStruct->aesOperationMode)
        {
            case HSMBareMetal_AES_MODE_ECB:
                *assetPolicy = HSM_ASSET_POLICY_SYM_AES_BULK | HSM_ASSET_POLICY_SYM_MODE_ECB;
                break;

            case HSMBareMetal_AES_MODE_CBC:
                *assetPolicy = HSM_ASSET_POLICY_SYM_AES_BULK | HSM_ASSET_POLICY_SYM_MODE_CBC;
                break;

            case HSMBareMetal_AES_MODE_CTR:
                *assetPolicy = HSM_ASSET_POLICY_SYM_AES_BULK | HSM_ASSET_POLICY_SYM_MODE_CTR;
                break;

            case HSMBareMetal_AES_MODE_CCM:
                *assetPolicy = HSM_ASSET_POLICY_SYM_AES_AUTH | HSM_ASSET_POLICY_SYM_MODE_CCM;
                break;

            case HSMBareMetal_AES_MODE_GCM:
                *assetPolicy = HSM_ASSET_POLICY_SYM_AES_AUTH | HSM_ASSET_POLICY_SYM_MODE_GCM;
                break;

            default:
                status = HSMBAREMETAL_STATUS_ERROR;
                break;
        }
    }
    else if (operationStruct->algorithm == HSMBareMetal_OPERATION_ALGO_MAC)
    {
        /* Operation is an MAC operation.
         * Determine the correct usage policy
         */
        switch (operationStruct->macOperationMode)
        {
            case HSMBareMetal_MAC_MODE_HMAC_224:
                *assetPolicy = HSM_ASSET_POLICY_SYM_HASH_MAC | HSM_ASSET_POLICY_AIH_SHA2_224;
                break;

            case HSMBareMetal_MAC_MODE_HMAC_256:
                *assetPolicy = HSM_ASSET_POLICY_SYM_HASH_MAC | HSM_ASSET_POLICY_AIH_SHA2_256;
                break;

            case HSMBareMetal_MAC_MODE_HMAC_384:
                *assetPolicy = HSM_ASSET_POLICY_SYM_HASH_MAC | HSM_ASSET_POLICY_AIH_SHA2_384;
                break;

            case HSMBareMetal_MAC_MODE_HMAC_512:
                *assetPolicy = HSM_ASSET_POLICY_SYM_HASH_MAC | HSM_ASSET_POLICY_AIH_SHA2_512;
                break;

            case HSMBareMetal_MAC_MODE_CMAC:
                *assetPolicy = HSM_ASSET_POLICY_SYM_AES_MAC | HSM_ASSET_POLICY_SYM_MODE_CMAC;
                break;

            case HSMBareMetal_MAC_MODE_CBC_MAC:
                *assetPolicy = HSM_ASSET_POLICY_SYM_AES_MAC | HSM_ASSET_POLICY_SYM_MODE_CBC_MAC;
                break;

            default:
                status = HSMBAREMETAL_STATUS_ERROR;
                break;
        }
    }
    else
    {
        /* Invalid operation type */
        status = HSMBAREMETAL_STATUS_ERROR;
    }

    /* The asset policy should contain whether the asset will be used for encrypt/generate or decrypt/verify operations.
     */
    if (operationDirection == HSMBareMetal_OPERATION_DIR_ENC_GEN)
    {
        *assetPolicy |= HSM_ASSET_POLICY_DIR_ENC_GEN;
    }
    else if (operationDirection == HSMBareMetal_OPERATION_DIR_DEC_VRFY)
    {
        *assetPolicy |= HSM_ASSET_POLICY_DIR_DEC_VRFY;
    }
    else
    {
        /* Invalid operation type */
        status = HSMBAREMETAL_STATUS_ERROR;
    }

    return status;
}

static int_fast16_t HSMBareMetal_createAndLoadSymKey(HSMBareMetal_AssetOperationStruct *operationStruct,
                                                     HSMBareMetal_operationDirection operationDirection)
{
    int_fast16_t status  = HSMBAREMETAL_STATUS_ERROR;
    uint32_t assetPolicy = 0U;
    uint32_t *keyAssetId = operationStruct->keyAssetIDs.encGenKeyAssetID;

    if (operationDirection == HSMBareMetal_OPERATION_DIR_DEC_VRFY)
    {
        keyAssetId = operationStruct->keyAssetIDs.decVrfyKeyAssetID;
    }

    /* Get the asset policy based on the user-provided data. */
    status = HSMBareMetal_constructAssetPolicy(operationStruct, operationDirection, &assetPolicy);

    if (status == HSMBAREMETAL_STATUS_ERROR)
    {
        /* The provided parameters do not match the supported features. */
        return HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS;
    }

    /* Create an asset based on the policy and the key length. */
    status = HSMBareMetal_createKeyAssetId(assetPolicy, operationStruct->keyLength, keyAssetId);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Load the key into the created asset. */
        status = HSMBareMetal_loadKeyAssetId(keyAssetId, operationStruct->key, operationStruct->keyLength);
    }

    return status;
}

/*
 *  ======== HSMBareMetal_CryptoKeyPlaintext_initKey ========
 */
void HSMBareMetal_CryptoKeyPlaintext_initKey(HSMBareMetal_CryptoKeyStruct *cryptoKey, uint8_t *key, size_t keyLength)
{
    memset(cryptoKey, 0x00, sizeof(HSMBareMetal_CryptoKeyStruct));

    cryptoKey->keyInputType = HSMBareMetal_KEY_INPUT_PLAINTEXT;

    cryptoKey->u.plaintext.keyMaterial = key;
    cryptoKey->u.plaintext.keyLength   = keyLength;
}

/*
 *  ======== HSMBareMetal_CryptoKeyAssetStore_initKey ========
 */
int_fast16_t HSMBareMetal_CryptoKeyAssetStore_initKey(HSMBareMetal_CryptoKeyStruct *cryptoKey,
                                                      uint32_t assetId,
                                                      size_t keyLength)
{
    memset(cryptoKey, 0x00, sizeof(HSMBareMetal_CryptoKeyStruct));

    cryptoKey->u.assetStore.assetId   = assetId;
    cryptoKey->u.assetStore.keyLength = keyLength;

    return HSMBAREMETAL_STATUS_SUCCESS;
}

/*
 *  ======== HSMBareMetal_AssetOperation ========
 */
int_fast16_t HSMBareMetal_AssetOperation(HSMBareMetal_AssetOperationStruct *operationStruct)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    /* Check whether the HSM has been initialized or not. */
    if (!HSMBareMetal_isInitialized)
    {
        /* HSM IP has not been initialized. A call to HSMBareMetal_init() is required before any cryptography operation
         * is conducted. */
        return HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED;
    }
    else if (HSMBareMetal_isHSMInFatalStatus())
    {
        /* The HSM IP entered in a fatal error condition and cannot be resuscitated. */
        return HSMBAREMETAL_STATUS_HW_ERROR;
    }

    /* When user requests the key generated, then the customer is not expected to provide the key. The key will be
     * generated internally deposited in the HSM IP and only the keyAssetID is returned.
     */
    if (operationStruct->isKeyGenerated)
    {
        HSMBareMetal_RNGOperationStruct RNGOperation;

        RNGOperation.random       = &HSMBareMetal_rawDataPool[0];
        RNGOperation.randomLength = operationStruct->keyLength;
        RNGOperation.entropyType  = ENTROPY_TYPE_DRBG;

        /* Conduct an RNG DRBG operation and deposit plaintext in key. */
        status = HSMBareMetal_RNGOperation(&RNGOperation);

        if (status != HSMBAREMETAL_STATUS_SUCCESS)
        {
            return status;
        }

        operationStruct->key = &HSMBareMetal_rawDataPool[0];
    }

    /* Perform Asset create and Asset load operations for encrypt mode. */
    status = HSMBareMetal_createAndLoadSymKey(operationStruct, HSMBareMetal_OPERATION_DIR_ENC_GEN);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Perform Asset create and Asset load operations for decrypt mode. */
        status = HSMBareMetal_createAndLoadSymKey(operationStruct, HSMBareMetal_OPERATION_DIR_DEC_VRFY);
    }

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        (void)HSMBareMetal_freeAssetPair(operationStruct->keyAssetIDs);
    }

    /* Since the user requested to generate the key, we clear the key pointer. */
    if (operationStruct->isKeyGenerated)
    {
        operationStruct->key = NULL;
    }

    return status;
}

/*
 *  ======== HSMBareMetal_freeKeyAsset ========
 */
int_fast16_t HSMBareMetal_freeKeyAsset(uint32_t *keyAssetID)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    /* Check whether the HSM has been initialized or not. */
    if (!HSMBareMetal_isInitialized)
    {
        /* HSM IP has not been initialized. A call to HSMBareMetal_init() is required before any cryptography operation
         * is conducted. */
        return HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED;
    }
    else if (HSMBareMetal_isHSMInFatalStatus())
    {
        /* The HSM IP entered in a fatal error condition and cannot be resuscitated. */
        return HSMBAREMETAL_STATUS_HW_ERROR;
    }

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_ASSET_MANAGEMENT_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    HSMBareMetal_inputToken[0] = HSM_ASSET_MANAGEMENT_DELETE_TOKEN_WORD0;
    HSMBareMetal_inputToken[2] = *keyAssetID;

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_ASSET_MANAGEMENT_WORD_COUNT);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        *keyAssetID = 0U;
    }

    return status;
}

/*
 *  ======== HSMBareMetal_freeAssetPair ========
 */
int_fast16_t HSMBareMetal_freeAssetPair(HSMBareMetal_AssetPairStruct keyAssetPair)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    status = HSMBareMetal_freeKeyAsset(keyAssetPair.encGenKeyAssetID);

    status |= HSMBareMetal_freeKeyAsset(keyAssetPair.decVrfyKeyAssetID);

    return status;
}

/*
 *  ======== HSMBareMetal_RNGOperation_init ========
 */
void HSMBareMetal_RNGOperation_init(HSMBareMetal_RNGOperationStruct *operationStruct)
{
    memset(operationStruct, 0x00, sizeof(HSMBareMetal_RNGOperationStruct));
}

/*
 *  ======== HSMBareMetal_AssetOperation_init ========
 */
void HSMBareMetal_AssetOperation_init(HSMBareMetal_AssetOperationStruct *operationStruct)
{
    memset(operationStruct, 0x00, sizeof(HSMBareMetal_AssetOperationStruct));
}

/*
 *  ======== HSMBareMetal_HASHOperation_init ========
 */
void HSMBareMetal_HASHOperation_init(HSMBareMetal_HASHOperationStruct *operationStruct)
{
    memset(operationStruct, 0x00, sizeof(HSMBareMetal_HASHOperationStruct));
}

/*
 *  ======== HSMBareMetal_AESOperation_init ========
 */
void HSMBareMetal_AESOperation_init(HSMBareMetal_AESOperationStruct *operationStruct)
{
    memset(operationStruct, 0x00, sizeof(HSMBareMetal_AESOperationStruct));
}

/*
 *  ======== HSMBareMetal_MACOperation_init ========
 */
void HSMBareMetal_MACOperation_init(HSMBareMetal_MACOperationStruct *operationStruct)
{
    memset(operationStruct, 0x00, sizeof(HSMBareMetal_AESOperationStruct));
}

/*
 *  ======== HSMBareMetal_ECCOperation_init ========
 */
void HSMBareMetal_ECCOperation_init(HSMBareMetal_ECCOperationStruct *operationStruct)
{
    memset(operationStruct, 0x00, sizeof(HSMBareMetal_ECCOperationStruct));
}

/*
 *  ======== HSMBareMetal_AESOperation ========
 */
int_fast16_t HSMBareMetal_AESOperation(HSMBareMetal_AESOperationStruct *operationStruct)
{
    int_fast16_t status       = HSMBAREMETAL_STATUS_ERROR;
    uint32_t keyLengthCode    = 0U;
    uint32_t inputDataLength  = operationStruct->inputLength;
    uint32_t outputDataLength = operationStruct->inputLength;

    /* Check whether the HSM has been initialized or not. */
    if (!HSMBareMetal_isInitialized)
    {
        /* HSM IP has not been initialized. A call to HSMBareMetal_init() is required before any cryptography operation
         * is conducted. */
        return HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED;
    }
    else if (HSMBareMetal_isHSMInFatalStatus())
    {
        /* The HSM IP entered in a fatal error condition and cannot be resuscitated. */
        return HSMBAREMETAL_STATUS_HW_ERROR;
    }

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_ENCRYPTION_TOKEN_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    /* Input Data Length to the HSM needs to be block size aligned but the overall data length should not */
    inputDataLength += AES_BLOCK_SIZE_ALIGN;
    inputDataLength &= (uint32_t)(~AES_BLOCK_SIZE_ALIGN);
    outputDataLength = ((inputDataLength + 3U) & (size_t)~3UL);

    /* Construct the input token. */
    HSMBareMetal_inputToken[0] = HSM_ENCRYPTION_TOKEN_WORD0;
    HSMBareMetal_inputToken[2] = (uint32_t)operationStruct->inputLength;
    HSMBareMetal_inputToken[3] = (uintptr_t)operationStruct->input;
    HSMBareMetal_inputToken[5] = inputDataLength;
    HSMBareMetal_inputToken[6] = (uintptr_t)operationStruct->output;
    HSMBareMetal_inputToken[8] = outputDataLength;
    HSMBareMetal_inputToken[9] = (uintptr_t)operationStruct->aad;

    HSMBareMetal_inputToken[11] = (operationStruct->operationMode << 4);

    HSMBareMetal_inputToken[11] |= BIT_14;

    if (operationStruct->operationDirection == HSMBareMetal_OPERATION_DIR_ENC_GEN)
    {
        HSMBareMetal_inputToken[11] |= BIT_15;
    }
    else if (operationStruct->operationDirection == HSMBareMetal_OPERATION_DIR_DEC_VRFY)
    {
        /* Decryption operation. Copy the Tag to the input token */
        (void)memcpy((void *)&HSMBareMetal_inputToken[33], (void *)operationStruct->mac, operationStruct->macLength);
    }
    else
    {
        /* The user-provided operation direction is invalid. */
        return HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS;
    }

    switch (operationStruct->keyLength)
    {
        case (128 / 8):
            keyLengthCode = 1U;
            break;

        case (192 / 8):
            keyLengthCode = 2U;
            break;

        case (256 / 8):
            keyLengthCode = 3U;
            break;

        default:
            keyLengthCode = 0U;
            break;
    }

    if (keyLengthCode == 0U)
    {
        /* The user-provided key length is invalid. */
        return HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS;
    }

    HSMBareMetal_inputToken[11] |= (keyLengthCode << 16);

    HSMBareMetal_inputToken[11] |= (((uint32_t)operationStruct->nonceLength & MASK_4_BITS) << 20);

    HSMBareMetal_inputToken[11] |= (((uint32_t)operationStruct->macLength & MASK_5_BITS) << 24);

    if (operationStruct->isKeyInAssetStore == HSMBareMetal_KEY_INPUT_ASSETSTORE)
    {
        /* Load the AssetID and set AS_LoadKey */
        HSMBareMetal_inputToken[11] |= BIT_8;

        HSMBareMetal_inputToken[17] |= operationStruct->keyAssetID;
    }
    else
    {
        /* Otherwise, load the key as plaintext into the token. */
        (void)memcpy((void *)&HSMBareMetal_inputToken[17], (void *)operationStruct->key, operationStruct->keyLength);
    }

    if (operationStruct->ivLength != 0U)
    {
        /* If an IV was provided then copy it. This only applies to GCM, CBC, CTR operations. */
        (void)memcpy((void *)&HSMBareMetal_inputToken[13], (void *)operationStruct->iv, operationStruct->ivLength);
    }

    if (operationStruct->operationMode == HSMBareMetal_AES_MODE_GCM)
    {
        /* For a GCM operation, the last word for the IV has to be a specific value. */
        HSMBareMetal_inputToken[16] = AES_GCM_IV_LAST_WORD;
    }

    if (operationStruct->nonceLength != 0U)
    {
        /* In the case of a CCM operation, nonce is copied to the appropriate word within the command token. */
        (void)memcpy((void *)&HSMBareMetal_inputToken[29],
                     (void *)operationStruct->nonce,
                     operationStruct->nonceLength);
    }

    HSMBareMetal_inputToken[25] = (uint32_t)operationStruct->aadLength;

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_ENCRYPTION_TOKEN_WORD_COUNT);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        if ((operationStruct->operationMode == HSMBareMetal_AES_MODE_CBC) ||
            (operationStruct->operationMode == HSMBareMetal_AES_MODE_CTR) ||
            (operationStruct->operationMode == HSMBareMetal_AES_MODE_GCM))
        {
            /* Copy the updated IV back. */
            (void)memcpy((void *)operationStruct->iv, (void *)&HSMBareMetal_outputToken[2], operationStruct->ivLength);
        }

        if (((operationStruct->operationMode == HSMBareMetal_AES_MODE_CCM) ||
             (operationStruct->operationMode == HSMBareMetal_AES_MODE_GCM)) &&
            (operationStruct->operationDirection == HSMBareMetal_OPERATION_DIR_ENC_GEN))
        {
            /* In the case of an encrypt CCM or GCM operation, copy the Tag/MAC. */
            (void)memcpy((void *)operationStruct->mac,
                         (void *)&HSMBareMetal_outputToken[6],
                         operationStruct->macLength);
        }
    }

    return status;
}

/*
 *  ======== HSMBareMetal_MACOperation ========
 */
int_fast16_t HSMBareMetal_MACOperation(HSMBareMetal_MACOperationStruct *operationStruct)
{
    int_fast16_t status      = HSMBAREMETAL_STATUS_ERROR;
    uint32_t inputDataLength = operationStruct->inputLength;
    uint32_t padBytes        = 0U;

    /* Check whether the HSM has been initialized or not. */
    if (!HSMBareMetal_isInitialized)
    {
        /* HSM IP has not been initialized. A call to HSMBareMetal_init() is required before any cryptography operation
         * is conducted. */
        return HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED;
    }
    else if (HSMBareMetal_isHSMInFatalStatus())
    {
        /* The HSM IP entered in a fatal error condition and cannot be resuscitated. */
        return HSMBAREMETAL_STATUS_HW_ERROR;
    }

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_MAC_TOKEN_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    if ((operationStruct->operationMode == HSMBareMetal_MAC_MODE_CMAC) ||
        (operationStruct->operationMode == HSMBareMetal_MAC_MODE_CBC_MAC))
    {
        /* Determine the number of padding bytes and a bytes aligned input data length. */
        inputDataLength = (inputDataLength + AES_BLOCK_SIZE - 1U) & ~(AES_BLOCK_SIZE - 1U);
        if ((inputDataLength == 0U) && (operationStruct->operationMode == HSMBareMetal_MAC_MODE_CMAC))
        {
            inputDataLength += AES_BLOCK_SIZE;
        }

        padBytes = inputDataLength - operationStruct->inputLength;
        if ((padBytes > 0U) && (operationStruct->operationMode == HSMBareMetal_MAC_MODE_CMAC))
        {
            uint8_t *data_p = operationStruct->input + operationStruct->inputLength;
            *data_p         = 0x80;

            if (padBytes == AES_BLOCK_SIZE)
            {
                padBytes--;
            }
        }
    }
    else
    {
        /* For input token 10th word HMAC operation, the number of padding bytes is the input data length. */
        padBytes = inputDataLength;
    }

    /* Construct the input token. */
    HSMBareMetal_inputToken[0]  = HSM_MAC_TOKEN_WORD0;
    HSMBareMetal_inputToken[2]  = inputDataLength;
    HSMBareMetal_inputToken[3]  = (uintptr_t)operationStruct->input;
    HSMBareMetal_inputToken[5]  = ((inputDataLength + 3U) & (uint32_t)~3UL);
    HSMBareMetal_inputToken[6]  = operationStruct->operationMode;
    HSMBareMetal_inputToken[8]  = operationStruct->keyAssetID;
    HSMBareMetal_inputToken[10] = padBytes;

    if (operationStruct->operationDirection == HSMBareMetal_OPERATION_DIR_DEC_VRFY)
    {
        /* Verify operation. Copy the MAC to the input token */
        (void)memcpy((void *)&HSMBareMetal_inputToken[14], (void *)operationStruct->mac, operationStruct->macLength);
    }

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_MAC_TOKEN_WORD_COUNT);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        if (operationStruct->operationDirection == HSMBareMetal_OPERATION_DIR_ENC_GEN)
        {
            /* Read out the mac from the result token and copy it to the user-provided buffer. */
            (void)memcpy((void *)operationStruct->mac,
                         (void *)&HSMBareMetal_outputToken[2],
                         operationStruct->macLength);
        }
        else
        {
            /* Verify operation. Return success. */
        }
    }
    else if ((operationStruct->operationDirection == HSMBareMetal_OPERATION_DIR_DEC_VRFY) &&
             (HSMBareMetal_outputToken[0] == HSM_OUTPUT_TOKEN_VRFY_ERROR))
    {
        status = HSMBAREMETAL_STATUS_INVALID_MAC;
    }

    return status;
}

/*
 *  ======== HSMBareMetal_HASHOperation ========
 */
int_fast16_t HSMBareMetal_HASHOperation(HSMBareMetal_HASHOperationStruct *operationStruct)
{
    int_fast16_t status   = HSMBAREMETAL_STATUS_ERROR;
    uint32_t digestLength = 0U;

    /* Check whether the HSM has been initialized or not. */
    if (!HSMBareMetal_isInitialized)
    {
        /* HSM IP has not been initialized. A call to HSMBareMetal_init() is required before any cryptography operation
         * is conducted. */
        return HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED;
    }
    else if (HSMBareMetal_isHSMInFatalStatus())
    {
        /* The HSM IP entered in a fatal error condition and cannot be resuscitated. */
        return HSMBAREMETAL_STATUS_HW_ERROR;
    }

    /* Determine the correct digest length. */
    switch (operationStruct->operationMode)
    {
        case HSMBareMetal_HASH_MODE_SHA2_224:
            digestLength = HSM_HASH_DIGEST_LENGTH_224;
            break;

        case HSMBareMetal_HASH_MODE_SHA2_384:
            digestLength = HSM_HASH_DIGEST_LENGTH_384;
            break;

        case HSMBareMetal_HASH_MODE_SHA2_512:
            digestLength = HSM_HASH_DIGEST_LENGTH_512;
            break;

        case HSMBareMetal_HASH_MODE_SHA2_256:
        default:
            digestLength = HSM_HASH_DIGEST_LENGTH_256;
            break;
    }

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_HASH_TOKEN_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    /* Construct the input token. */
    HSMBareMetal_inputToken[0]  = HSM_HASH_TOKEN_WORD0;
    HSMBareMetal_inputToken[2]  = operationStruct->inputLength;
    HSMBareMetal_inputToken[3]  = (uintptr_t)operationStruct->input;
    HSMBareMetal_inputToken[5]  = operationStruct->inputLength;
    HSMBareMetal_inputToken[6]  = operationStruct->operationMode;
    HSMBareMetal_inputToken[10] = operationStruct->inputLength;

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_HASH_TOKEN_WORD_COUNT);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Read out the hash digest from the result token and copy it to the user-provided buffer. */
        (void)memcpy((void *)operationStruct->digest, (void *)&HSMBareMetal_outputToken[2], digestLength);
    }

    return status;
}

/*
 *  ======== HSMBareMetal_ECCOperationGetCurveLength ========
 */
static int_fast16_t HSMBareMetal_ECCOperationGetCurveLength(HSMBareMetal_ECCOperationObject *object)
{
    int_fast16_t status                          = HSMBAREMETAL_STATUS_SUCCESS;
    HSMBareMetal_ECCOperationCurveType curveType = object->operation->operationCurveType;
    object->domainId                             = HSMBareMetal_ECC_DOMAIN_ID_SEC;

    switch (curveType)
    {
        case HSMBareMetal_PK_CURVE_TYPE_SEC_P_224_R1:
            object->operationCurveLength  = HSMBareMetal_PK_CURVE_LENGTH_224;
            object->operationDigestLength = HSMBareMetal_PK_DIGEST_LENGTH_224;
            break;
        case HSMBareMetal_PK_CURVE_TYPE_BRP_P_256_R1:
        case HSMBareMetal_PK_CURVE_TYPE_SEC_P_256_R1:
            object->operationCurveLength  = HSMBareMetal_PK_CURVE_LENGTH_256;
            object->operationDigestLength = HSMBareMetal_PK_DIGEST_LENGTH_256;
            if (curveType == HSMBareMetal_PK_CURVE_TYPE_BRP_P_256_R1)
            {
                object->domainId = HSMBareMetal_ECC_DOMAIN_ID_BRP;
            }
            break;
        case HSMBareMetal_PK_CURVE_TYPE_BRP_P_384_R1:
        case HSMBareMetal_PK_CURVE_TYPE_SEC_P_384_R1:
            object->operationCurveLength  = HSMBareMetal_PK_CURVE_LENGTH_384;
            object->operationDigestLength = HSMBareMetal_PK_DIGEST_LENGTH_384;
            if (curveType == HSMBareMetal_PK_CURVE_TYPE_BRP_P_384_R1)
            {
                object->domainId = HSMBareMetal_ECC_DOMAIN_ID_BRP;
            }
            break;
        case HSMBareMetal_PK_CURVE_TYPE_BRP_P_512_R1:
            object->operationCurveLength  = HSMBareMetal_PK_CURVE_LENGTH_512;
            object->operationDigestLength = HSMBareMetal_PK_DIGEST_LENGTH_512;
            object->domainId              = HSMBareMetal_ECC_DOMAIN_ID_BRP;
            break;
        case HSMBareMetal_PK_CURVE_TYPE_SEC_P_521_R1:
            object->operationCurveLength  = HSMBareMetal_PK_CURVE_LENGTH_521;
            object->operationDigestLength = HSMBareMetal_PK_DIGEST_LENGTH_512;
            break;
        case HSMBareMetal_PK_CURVE_TYPE_25519:
            object->operationCurveLength  = HSMBareMetal_PK_CURVE_LENGTH_255;
            object->operationDigestLength = HSMBareMetal_PK_DIGEST_LENGTH_256;
            break;
        default:
            /* Invalid CurveType. Set object->curveFamily to NONE and return an error.
             * A valid CurveType that is supported by the ECDH driver should be
             * provided in order to perform a public key or a shared secret
             * operation.
             */
            status = HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS;
            break;
    }

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        return status;
    }

    return HSMBareMetalECCParams_getCurveParameters(curveType,
                                                    (const uint8_t **)&object->curveParams.u.plaintext.keyMaterial,
                                                    &object->curveParams.u.plaintext.keyLength);
}

/*
 *  ======== HSMBareMetal_ECCOperationCreateCurveParamsAsset ========
 */
static int_fast16_t HSMBareMetal_ECCOperationCreateCurveParamsAsset(HSMBareMetal_ECCOperationObject *object)
{
    int_fast16_t status      = HSMBAREMETAL_STATUS_ERROR;
    uint8_t *curveParamsPtr  = object->curveParams.u.plaintext.keyMaterial;
    uint32_t curveParamsSize = object->curveParams.u.plaintext.keyLength;

    status = HSMBareMetal_createKeyAssetId(HSM_ASSET_POLICY_ASYM_KEYPARAMS, curveParamsSize, &object->paramAssetId);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        status = HSMBareMetal_loadKeyAssetId(&object->paramAssetId, curveParamsPtr, curveParamsSize);
    }

    return status;
}

/*
 *  ======== HSMBareMetal_formatCurve25519PrivateKeyScratch ========
 */
static void HSMBareMetal_formatCurve25519PrivateKeyScratch(uint8_t *myPrivateKey)
{
    /*
     * As per RFC 7748, the private key of Curve25519 is pruned so that
     * the three LSB's are cleared, bit 255 is cleared, and bit 254 is
     * set.
     */
    /* Clear bit 0, 1, and 2 */
    myPrivateKey[0] &= 0xF8UL;
    /* Clear bit 255 */
    myPrivateKey[31] &= 0x7FFUL;
    /* Set bit 254 */
    myPrivateKey[31] |= 0x40UL;
}

/*
 *  ======== HSMBareMetal_ECCOperationCreateAndLoadPrivateKeyAsset ========
 */
static int_fast16_t HSMBareMetal_ECCOperationCreateAndLoadPrivateKeyAsset(HSMBareMetal_ECCOperationObject *object)
{
    int_fast16_t status  = HSMBAREMETAL_STATUS_ERROR;
    uint32_t assetPolicy = 0U;
    uint32_t assetSize   = HSM_ASYM_DATA_SIZE_VWB(object->operationCurveLength);

    if ((object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY) ||
        (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT))
    {
        assetPolicy = HSM_ASSET_POLICY_ASYM_KEYEXCH | HSM_ASSET_POLICY_PRIVATEDATA | HSM_ASSET_POLICY_ST_ANY;

        if (object->operation->operationCurveType == HSMBareMetal_PK_CURVE_TYPE_25519)
        {
            assetPolicy |= HSM_ASSET_POLICY_ASYM_CURVE25519;
        }
        else
        {
            assetPolicy |= HSM_ASSET_POLICY_ASYM_ECDH;
        }
    }
    else
    {
        assetPolicy = HSM_ASSET_POLICY_ASYM_SIGNVRFY | HSM_ASSET_POLICY_PRIVATEDATA | HSM_ASSET_POLICY_ASYM_ECDSA;

        switch (object->operation->operationCurveType)
        {
            case HSMBareMetal_PK_CURVE_TYPE_SEC_P_224_R1:
                assetPolicy |= HSM_ASSET_POLICY_ASYM_SHA2_224;
                break;
            case HSMBareMetal_PK_CURVE_TYPE_SEC_P_256_R1:
            case HSMBareMetal_PK_CURVE_TYPE_BRP_P_256_R1:
                assetPolicy |= HSM_ASSET_POLICY_ASYM_SHA2_256;
                break;
            case HSMBareMetal_PK_CURVE_TYPE_SEC_P_384_R1:
            case HSMBareMetal_PK_CURVE_TYPE_BRP_P_384_R1:
                assetPolicy |= HSM_ASSET_POLICY_ASYM_SHA2_384;
                break;
            case HSMBareMetal_PK_CURVE_TYPE_SEC_P_521_R1:
            case HSMBareMetal_PK_CURVE_TYPE_BRP_P_512_R1:
                assetPolicy |= HSM_ASSET_POLICY_ASYM_SHA2_512;
                break;
            default:
                /* Do nothing. Curve type is invalid */
                break;
        }
    }

    status = HSMBareMetal_createKeyAssetId(assetPolicy, assetSize, &object->privateKeyAssetId);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        /* Initialize a buffer that will hold the single- or multi-component vector for the operation's key */
        uint8_t componentVector[HSM_ASYM_COMPONENT_VECTOR_LENGTH];
        (void)memset(&componentVector[0], 0, assetSize);

        HSMBareMetalECCParams_asymDHPriKeyToHW(object->operation->privateKey->u.plaintext.keyMaterial,
                                               object->operationCurveLength,
                                               object->domainId,
                                               &componentVector[0]);
        if (object->operation->operationCurveType == HSMBareMetal_PK_CURVE_TYPE_25519)
        {
            /* HSMBareMetal needs to prune the private key. */
            HSMBareMetal_formatCurve25519PrivateKeyScratch(&componentVector[HSM_ASYM_DATA_VHEADER]);
        }

        status = HSMBareMetal_loadKeyAssetId(&object->privateKeyAssetId, &componentVector[0], assetSize);
    }

    return status;
}

/*
 *  ======== HSMBareMetal_ECCOperationCreateAndLoadPublicKeyAsset ========
 */
static int_fast16_t HSMBareMetal_ECCOperationCreateAndLoadPublicKeyAsset(HSMBareMetal_ECCOperationObject *object)
{
    int_fast16_t status  = HSMBAREMETAL_STATUS_ERROR;
    uint32_t assetPolicy = 0U;
    uint32_t assetSize   = HSM_ASYM_DATA_SIZE_VWB(object->operationCurveLength);
    uint32_t itemCount   = HSM_ASYM_CURVE25519_PUB_KEY_VCOUNT;

    if ((object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY) ||
        (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT))
    {
        assetPolicy = HSM_ASSET_POLICY_ASYM_KEYEXCH;

        if (object->operation->operationCurveType == HSMBareMetal_PK_CURVE_TYPE_25519)
        {
            assetPolicy |= HSM_ASSET_POLICY_ASYM_CURVE25519;
        }
        else
        {
            assetPolicy |= HSM_ASSET_POLICY_ASYM_ECDH;
            assetSize *= HSM_ASYM_ECC_PUB_KEY_VCOUNT;
            itemCount = HSM_ASYM_ECC_PUB_KEY_VCOUNT;
        }
    }
    else
    {
        assetPolicy = HSM_ASSET_POLICY_ASYM_SIGNVRFY | HSM_ASSET_POLICY_ASYM_ECDSA;

        switch (object->operation->operationCurveType)
        {
            case HSMBareMetal_PK_CURVE_TYPE_SEC_P_224_R1:
                assetPolicy |= HSM_ASSET_POLICY_ASYM_SHA2_224;
                break;
            case HSMBareMetal_PK_CURVE_TYPE_SEC_P_256_R1:
            case HSMBareMetal_PK_CURVE_TYPE_BRP_P_256_R1:
                assetPolicy |= HSM_ASSET_POLICY_ASYM_SHA2_256;
                break;
            case HSMBareMetal_PK_CURVE_TYPE_SEC_P_384_R1:
            case HSMBareMetal_PK_CURVE_TYPE_BRP_P_384_R1:
                assetPolicy |= HSM_ASSET_POLICY_ASYM_SHA2_384;
                break;
            case HSMBareMetal_PK_CURVE_TYPE_SEC_P_521_R1:
            case HSMBareMetal_PK_CURVE_TYPE_BRP_P_512_R1:
                assetPolicy |= HSM_ASSET_POLICY_ASYM_SHA2_512;
                break;
            default:
                /* Do nothing. Curve type is invalid */
                break;
        }

        assetSize *= HSM_ASYM_ECC_PUB_KEY_VCOUNT;
        itemCount = HSM_ASYM_ECC_PUB_KEY_VCOUNT;
    }

    status = HSMBareMetal_createKeyAssetId(assetPolicy, assetSize, &object->publicKeyAssetId);

    if ((status == HSMBAREMETAL_STATUS_SUCCESS) &&
        (object->operation->operationMode != HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY))
    {
        /* Initialize a buffer that will hold the single- or multi-component vector for the operation's key */
        uint8_t componentVector[HSM_ASYM_COMPONENT_VECTOR_LENGTH];
        (void)memset(&componentVector[0], 0, assetSize);

        HSMBareMetalECCParams_asymDHPubKeyToHW(object->operation->publicKey->u.plaintext.keyMaterial,
                                               object->operationCurveLength,
                                               itemCount,
                                               object->domainId,
                                               &componentVector[0]);

        status = HSMBareMetal_loadKeyAssetId(&object->publicKeyAssetId, &componentVector[0], assetSize);
    }

    return status;
}

/*
 *  ======== HSMBareMetal_ECCOperationCreatePublicDataAsset ========
 */
static int_fast16_t HSMBareMetal_ECCOperationCreatePublicDataAsset(HSMBareMetal_ECCOperationObject *object)
{
    return HSMBareMetal_createKeyAssetId(HSM_ASSET_POLICY_GENERIC_DATA,
                                         HSM_ASYM_DATA_SIZE_B2WB(object->operationCurveLength),
                                         &object->publicDataAssetId);
}

/*
 *  ======== HSMBareMetal_ECCOperationCreateAllAssets ========
 */
static int_fast16_t HSMBareMetal_ECCOperationCreateAllAssets(HSMBareMetal_ECCOperationObject *object)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    if (object->operation->operationMode != HSMBareMetal_PK_MODE_ECDSA_VERIFY)
    {
        /* Perform two HSM operations:
         * - Create an asset that holds the operation's asymmetric private key.
         * - Load the operation's private key onto HSM RAM.
         */
        status = HSMBareMetal_ECCOperationCreateAndLoadPrivateKeyAsset(object);

        if (status != HSMBAREMETAL_STATUS_SUCCESS)
        {
            return status;
        }
    }

    if (object->operation->operationMode != HSMBareMetal_PK_MODE_ECDSA_SIGN)
    {
        /* Perform two HSM operations:
         * - Create An asset that holds the operation's asymmetric public key.
         * - If the operation is to ECDH generate a shared secret or ECDSA verify, load the operation's public key onto
         * HSM RAM.
         */
        status = HSMBareMetal_ECCOperationCreateAndLoadPublicKeyAsset(object);

        if (status != HSMBAREMETAL_STATUS_SUCCESS)
        {
            return status;
        }
    }

    if (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT)
    {
        /* In the case of shared secret generation, the HSMBareMetal module needs to create a public data asset to store
         * the shared secret data. */
        status = HSMBareMetal_ECCOperationCreatePublicDataAsset(object);

        if (status != HSMBAREMETAL_STATUS_SUCCESS)
        {
            return status;
        }
    }

    /* Perform two HSM operations:
     * - Create an asset that holds the operation's ECC curve parameters
     * - Load the appropriate ECC curve parameters onto HSM RAM
     */
    status = HSMBareMetal_ECCOperationCreateCurveParamsAsset(object);

    return status;
}

/*
 *  ======== HSMBareMetal_ECCOperationValidateParams ========
 */
static int_fast16_t HSMBareMetal_ECCOperationValidateParams(HSMBareMetal_ECCOperationObject *object)
{
    HSMBareMetal_CryptoKeyStruct *privateKey   = object->operation->privateKey;
    HSMBareMetal_CryptoKeyStruct *publicKey    = object->operation->publicKey;
    HSMBareMetal_CryptoKeyStruct *sharedSecret = object->operation->sharedSecret;

    /* Check the operation's mode. */
    if ((object->operation->operationMode != HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY) &&
        (object->operation->operationMode != HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT) &&
        (object->operation->operationMode != HSMBareMetal_PK_MODE_ECDSA_SIGN) &&
        (object->operation->operationMode != HSMBareMetal_PK_MODE_ECDSA_VERIFY))
    {
        return HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS;
    }

    if ((object->operation->operationCurveType != HSMBareMetal_PK_CURVE_TYPE_SEC_P_224_R1) &&
        (object->operation->operationCurveType != HSMBareMetal_PK_CURVE_TYPE_SEC_P_256_R1) &&
        (object->operation->operationCurveType != HSMBareMetal_PK_CURVE_TYPE_SEC_P_384_R1) &&
        (object->operation->operationCurveType != HSMBareMetal_PK_CURVE_TYPE_SEC_P_521_R1) &&
        (object->operation->operationCurveType != HSMBareMetal_PK_CURVE_TYPE_BRP_P_256_R1) &&
        (object->operation->operationCurveType != HSMBareMetal_PK_CURVE_TYPE_BRP_P_384_R1) &&
        (object->operation->operationCurveType != HSMBareMetal_PK_CURVE_TYPE_BRP_P_512_R1) &&
        (object->operation->operationCurveType != HSMBareMetal_PK_CURVE_TYPE_25519))
    {
        return HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS;
    }

    if (((object->operation->operationMode == HSMBareMetal_PK_MODE_ECDSA_SIGN) ||
         (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDSA_VERIFY)) &&
        (object->operation->operationCurveType == HSMBareMetal_PK_CURVE_TYPE_25519))
    {
        return HSMBAREMETAL_STATUS_INVALID_INPUT_PARAMETERS;
    }

    /* Check private key length. It should match the curve length after conversion. */
    if ((object->operation->operationMode != HSMBareMetal_PK_MODE_ECDSA_VERIFY) &&
        (privateKey->u.plaintext.keyLength != BITS_TO_BYTES(object->operationCurveLength)))
    {
        return HSMBAREMETAL_STATUS_INVALID_PRIVATE_KEY_SIZE;
    }

    /* For big endian, the formatting is the same no matter what the curve type is.
     * The format for uncompressed big endian, it is 1 + (BITS_TO_BYTES(object->object->operationCurveLength) * 2).
     */
    if ((object->operation->operationMode != HSMBareMetal_PK_MODE_ECDSA_SIGN) &&
        (publicKey->u.plaintext.keyLength !=
         (HSM_ASYM_ECC_PUB_KEY_VCOUNT * BITS_TO_BYTES(object->operationCurveLength) +
          HSM_ASYM_ECC_PUB_KEY_UNCOMP_ENC_LENG)))
    {
        return HSMBAREMETAL_STATUS_INVALID_PUBLIC_KEY_SIZE;
    }

    /* For shared secret operations in big endian format,
     * the HSM_ASYM_ECC_PUB_KEY_UNCOMP_ENC_LENG mentioned above is the octet formatting string for the public key
     * indicating what the public key contains:
     * - Value 0x04 means that it is a uncompressed public key with X and Y components.
     */
    if ((object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT) &&
        (publicKey->u.plaintext.keyMaterial[0] != HSM_ASYM_ECC_UNCOMP_ENC_VALUE))
    {
        return HSMBAREMETAL_STATUS_INVALID_PUBLIC_KEY_OCTET_VALUE;
    }

    if (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT)
    {
        /* We cannot guarantee what the user will pass in as a key length for the shared secret so we check against the
         * known permutations.
         */
        if ((sharedSecret->u.plaintext.keyLength != BITS_TO_BYTES(object->operationCurveLength)) &&
            (sharedSecret->u.plaintext.keyLength !=
             (BITS_TO_BYTES(object->operationCurveLength) * HSM_ASYM_ECC_PUB_KEY_VCOUNT)) &&
            (sharedSecret->u.plaintext.keyLength !=
             (BITS_TO_BYTES(object->operationCurveLength) * HSM_ASYM_ECC_PUB_KEY_VCOUNT +
              HSM_ASYM_ECC_PUB_KEY_UNCOMP_ENC_LENG)))
        {
            return HSMBAREMETAL_STATUS_INVALID_SHARED_SECRET_KEY_SIZE;
        }
    }

    return HSMBAREMETAL_STATUS_SUCCESS;
}

/*
 *  ======== HSMBareMetal_ECCOperationInitializeOperation ========
 */
static int_fast16_t HSMBareMetal_ECCOperationInitializeOperation(HSMBareMetal_ECCOperationObject *object)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    /* Check whether the HSM has been initialized or not. */
    if (!HSMBareMetal_isInitialized)
    {
        /* HSM IP has not been initialized. A call to HSMBareMetal_init() is required before any cryptography operation
         * is conducted.
         */
        return HSMBAREMETAL_STATUS_HSM_NOT_INITIALIZED;
    }
    else if (HSMBareMetal_isHSMInFatalStatus())
    {
        /* The HSM IP entered in a fatal error condition and cannot be resuscitated. */
        return HSMBAREMETAL_STATUS_HW_ERROR;
    }

    /* Initializes critical ECC metadata and retrieves and stores ECC curve parameters in the object */
    status = HSMBareMetal_ECCOperationGetCurveLength(object);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        return status;
    }

    /* Validate key length for private, public, and shared secret CryptoKeys */
    status = HSMBareMetal_ECCOperationValidateParams(object);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        return status;
    }

    /* Create and load assets necessary to conduct the operation:
     * - Private key, for both types of operations.
     * - Public key, for both types of operations.
     * - Public data object, for shared secret operations only.
     * - ECC curve parameters, for both types of operations.
     */
    status = HSMBareMetal_ECCOperationCreateAllAssets(object);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        return status;
    }

    if ((object->operation->operationCurveType != HSMBareMetal_PK_CURVE_TYPE_25519) &&
        ((object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY) ||
         (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT)))
    {
        status = HSMBareMetal_ECCOperationCheckECCKeys(object);
    }

    return status;
}

/*
 *  ======== HSMBareMetal_ECCOperationCheckECCKeys ========
 */
static int_fast16_t HSMBareMetal_ECCOperationCheckECCKeys(HSMBareMetal_ECCOperationObject *object)
{
    int_fast16_t status        = HSMBAREMETAL_STATUS_ERROR;
    uint32_t nWord             = HSM_ASYM_DATA_SIZE_B2W(object->operationCurveLength);
    uint32_t privateKeyAssetId = object->privateKeyAssetId;
    uint32_t publicKeyAssetId  = object->publicKeyAssetId;

    if (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY)
    {
        publicKeyAssetId = 0U;
    }
    else
    {
        privateKeyAssetId = 0U;
    }

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_HASH_TOKEN_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    /* Construct the input token. */
    HSMBareMetal_inputToken[0] = HSM_PK_TOKEN_WORD0;
    HSMBareMetal_inputToken[2] = (nWord << 24) | (nWord << 16) | HSM_PK_TOKEN_WORD2_CMD_ECC_KEYS_CHECK;
    HSMBareMetal_inputToken[4] = publicKeyAssetId;
    HSMBareMetal_inputToken[5] = object->paramAssetId;
    HSMBareMetal_inputToken[6] = privateKeyAssetId;

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_HASH_TOKEN_WORD_COUNT);

    return status;
}

/*
 *  ======== HSMBareMetal_ECCOperationReadPublicDataAsset ========
 */
static int_fast16_t HSMBareMetal_ECCOperationReadPublicDataAsset(uint32_t *assetId,
                                                                 uint32_t assetSize,
                                                                 uintptr_t outputAddress)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_HASH_TOKEN_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    /* Construct the input token. */
    HSMBareMetal_inputToken[0] = HSM_ASSET_MANAGEMENT_READ_TOKEN_WORD0;
    HSMBareMetal_inputToken[2] = *assetId;
    HSMBareMetal_inputToken[3] = assetSize;
    HSMBareMetal_inputToken[4] = outputAddress;

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_HASH_TOKEN_WORD_COUNT);

    return status;
}

/*
 *  ======== HSMBareMetal_ECCOperationInternal ========
 */
static int_fast16_t HSMBareMetal_ECCOperationInternal(HSMBareMetal_ECCOperationObject *object)
{
    int_fast16_t status        = HSMBAREMETAL_STATUS_ERROR;
    uint32_t command           = 0U;
    uint32_t nWord             = HSM_ASYM_DATA_SIZE_B2W(object->operationCurveLength);
    uint32_t privateKeyAssetId = object->privateKeyAssetId;
    uint32_t publicKeyAssetId  = object->publicKeyAssetId;
    uint32_t paramsAssetId     = object->paramAssetId;
    uint32_t publicDataAssetId = object->publicDataAssetId;
    uint32_t output            = (uintptr_t)object->output;
    uint32_t outputSize        = HSM_ASYM_DATA_SIZE_VWB(object->operationCurveLength);

    if ((object->operation->operationMode == HSMBareMetal_PK_MODE_ECDSA_SIGN) ||
        (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDSA_VERIFY))
    {
        outputSize = HSM_SIGNATURE_VCOUNT * outputSize;
    }
    else if ((object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY) &&
             (object->operation->operationCurveType != HSMBareMetal_PK_CURVE_TYPE_25519))
    {
        outputSize = HSM_SIGNATURE_VCOUNT * outputSize;
    }

    switch (object->operation->operationMode)
    {
        case HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY:
            command = HSM_PK_TOKEN_WORD2_CMD_ECC_GEN_PUBKEY;
            if (object->operation->operationCurveType == HSMBareMetal_PK_CURVE_TYPE_25519)
            {
                command = HSM_PK_TOKEN_WORD2_CMD_25519_GEN_PUBKEY;
            }
            break;
        case HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT:
            command = HSM_PK_TOKEN_WORD2_CMD_ECDH_GEN_SHRD_SCRT;
            if (object->operation->operationCurveType == HSMBareMetal_PK_CURVE_TYPE_25519)
            {
                command = HSM_PK_TOKEN_WORD2_CMD_25519_GEN_SHRD_SCRT;
            }
            break;
        case HSMBareMetal_PK_MODE_ECDSA_SIGN:
            command = HSM_PK_TOKEN_WORD2_CMD_ECDSA_SIGN;
            break;
        case HSMBareMetal_PK_MODE_ECDSA_VERIFY:
            command = HSM_PK_TOKEN_WORD2_CMD_ECDSA_VRFY;
            break;
        default:
            command = 0U;
            break;
    }

    (void)memset(&HSMBareMetal_inputToken[0], 0, sizeof(uint32_t) * HSM_PK_TOKEN_WORD_COUNT);
    (void)memset(&HSMBareMetal_outputToken[0], 0, sizeof(uint32_t) * HSM_OUTPUT_TOKEN_WORD_COUNT);

    /* Construct the input token. */
    HSMBareMetal_inputToken[0] = HSM_PK_TOKEN_WORD0;
    HSMBareMetal_inputToken[2] = (nWord << 24) | (nWord << 16) | command;

    if (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT)
    {
        HSMBareMetal_inputToken[3] = HSM_PK_TOKEN_WORD3_SV_SHRD_SCRT | (1U << 8) | (4U);
    }
    else if ((object->operation->operationMode == HSMBareMetal_PK_MODE_ECDSA_SIGN) ||
             (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDSA_VERIFY))
    {
        HSMBareMetal_inputToken[3] = HSM_PK_TOKEN_WORD3_EXP_DGST | BITS_TO_BYTES(object->operationDigestLength);
    }

    HSMBareMetal_inputToken[4] = privateKeyAssetId;
    HSMBareMetal_inputToken[5] = paramsAssetId;
    HSMBareMetal_inputToken[6] = publicKeyAssetId;

    if (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY)
    {
        HSMBareMetal_inputToken[7]  = (outputSize << 16);
        HSMBareMetal_inputToken[10] = output;
    }
    else if (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT)
    {
        HSMBareMetal_inputToken[12] = publicDataAssetId;
    }
    else if ((object->operation->operationMode == HSMBareMetal_PK_MODE_ECDSA_SIGN) ||
             (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDSA_VERIFY))
    {
        if (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDSA_VERIFY)
        {
            HSMBareMetal_inputToken[4] = publicKeyAssetId;
            HSMBareMetal_inputToken[6] = 0U;

            memset(&object->output[0], 0, sizeof(object->output));
            HSMBareMetalECCParams_asymDsaSignatureToHW(object->operation->signatureR,
                                                       object->operation->signatureS,
                                                       object->operationCurveLength,
                                                       &object->output[0]);
        }

        HSMBareMetal_inputToken[7]  = (outputSize << 16);
        HSMBareMetal_inputToken[10] = output;

        (void)memcpy(&HSMBareMetal_inputToken[12],
                     &object->operation->hashDigest[0],
                     BITS_TO_BYTES(object->operationDigestLength));
    }

    /* Perform the following:
     *  - Write the command token to the mailbox 1 in register from #HSMBareMetal_inputToken.
     *  - Trigger the mailbox 1 control register for the HSM to process the request.
     *  - Poll on the out register and read out the result token to #HSMBareMetal_outputToken.
     */
    status = HSMBareMetal_writeTokenAndWaitForResults(HSM_PK_TOKEN_WORD_COUNT);

    return status;
}

/*
 *  ======== HSMBareMetal_ECCOperationFreeAllAssets ========
 */
static int_fast16_t HSMBareMetal_ECCOperationFreeAllAssets(HSMBareMetal_ECCOperationObject *object)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_SUCCESS;

    if (object->privateKeyAssetId != 0U)
    {
        status |= HSMBareMetal_freeKeyAsset(&object->privateKeyAssetId);
    }

    if (object->publicKeyAssetId != 0U)
    {
        status |= HSMBareMetal_freeKeyAsset(&object->publicKeyAssetId);
    }

    if (object->publicDataAssetId != 0U)
    {
        status |= HSMBareMetal_freeKeyAsset(&object->publicDataAssetId);
    }

    if (object->paramAssetId != 0U)
    {
        status |= HSMBareMetal_freeKeyAsset(&object->paramAssetId);
    }

    return status;
}

/*
 *  ======== HSMBareMetal_ECCOperationInternalPostProcessing ========
 */
static int_fast16_t HSMBareMetal_ECCOperationInternalPostProcessing(HSMBareMetal_ECCOperationObject *object)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_SUCCESS;

    if (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_PUB_KEY)
    {
        uint8_t itemCount = HSM_ASYM_ECC_PUB_KEY_VCOUNT;

        /* Public key for curve25519 is composed of only one component, pubkey.u.
         * For other curves, the public key has two components, pubkey.x and pubkey.y.
         */
        if (object->operation->operationCurveType == HSMBareMetal_PK_CURVE_TYPE_25519)
        {
            itemCount = HSM_ASYM_CURVE25519_PUB_KEY_VCOUNT;
        }

        HSMBareMetalECCParams_asymDHPubKeyFromHW(&object->output[0],
                                                 object->operationCurveLength,
                                                 itemCount,
                                                 object->operation->publicKey->u.plaintext.keyMaterial);
    }
    else if (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDH_GEN_SHRD_SCRT)
    {
        uint8_t componentVector[HSM_ASYM_COMPONENT_VECTOR_LENGTH];
        uint32_t assetSize = HSM_ASYM_DATA_SIZE_B2WB(object->operationCurveLength);
        (void)memset(&componentVector[0], 0, assetSize);

        status = HSMBareMetal_ECCOperationReadPublicDataAsset(&object->publicDataAssetId,
                                                              assetSize,
                                                              (uintptr_t)&componentVector);

        if (status == HSMBAREMETAL_STATUS_SUCCESS)
        {
            uint8_t *key_p = object->operation->sharedSecret->u.plaintext.keyMaterial;

            key_p[0] = HSM_ASYM_ECC_UNCOMP_ENC_VALUE;
            key_p += HSM_ASYM_ECC_PUB_KEY_UNCOMP_ENC_LENG;

            memcpy(key_p, componentVector, BITS_TO_BYTES(object->operationCurveLength));
        }
    }
    else if (object->operation->operationMode == HSMBareMetal_PK_MODE_ECDSA_SIGN)
    {
        HSMBareMetalECCParams_asymDsaSignatureFromHW(object->output,
                                                     object->operationCurveLength,
                                                     object->operation->signatureR,
                                                     object->operation->signatureS);
    }

    (void)HSMBareMetal_ECCOperationFreeAllAssets(object);

    return status;
}

/*
 *  ======== HSMBareMetal_ECCOperation ========
 */
int_fast16_t HSMBareMetal_ECCOperation(HSMBareMetal_ECCOperationStruct *operationStruct)
{
    int_fast16_t status = HSMBAREMETAL_STATUS_ERROR;
    HSMBareMetal_ECCOperationObject object;

    (void)memset(&object, 0, sizeof(HSMBareMetal_ECCOperationObject));

    object.operation = operationStruct;

    status = HSMBareMetal_ECCOperationInitializeOperation(&object);

    if (status != HSMBAREMETAL_STATUS_SUCCESS)
    {
        return status;
    }

    status = HSMBareMetal_ECCOperationInternal(&object);

    if (status == HSMBAREMETAL_STATUS_SUCCESS)
    {
        status = HSMBareMetal_ECCOperationInternalPostProcessing(&object);
    }

    return status;
}
