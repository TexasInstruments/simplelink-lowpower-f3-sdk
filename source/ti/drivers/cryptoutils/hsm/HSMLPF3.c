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

#include <stdint.h>
#include <string.h>

#include <ti/drivers/cryptoutils/hsm/HSMLPF3.h>
#include <ti/drivers/cryptoutils/hsm/HSMLPF3Utility.h>

#include <ti/drivers/SHA2.h>
#include <ti/drivers/sha2/SHA2LPF3HSM.h>

#include <ti/drivers/ECDSA.h>
#include <ti/drivers/ecdsa/ECDSALPF3HSM.h>

#include <ti/drivers/ECDH.h>
#include <ti/drivers/ecdh/ECDHLPF3HSM.h>

#include <ti/drivers/AESGCM.h>
#include <ti/drivers/aesgcm/AESGCMLPF3HSM.h>

#include <ti/drivers/AESECB.h>
#include <ti/drivers/aesecb/AESECBLPF3.h>

#include <ti/drivers/AESCCM.h>
#include <ti/drivers/aesccm/AESCCMLPF3.h>

#include <ti/drivers/AESCTR.h>
#include <ti/drivers/aesctr/AESCTRLPF3.h>

#include <ti/drivers/AESCBC.h>
#include <ti/drivers/aescbc/AESCBCLPF3.h>

#include <ti/drivers/AESCMAC.h>
#include <ti/drivers/aescmac/AESCMACLPF3.h>

#include <third_party/hsmddk/include/Integration/HSMSAL/HSMSAL.h>
#include <third_party/hsmddk/include/Kit/EIP130/DomainHelper/incl/eip130_domain_ecc_curves.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_asset.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_common.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_crypto.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_hash.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_mac.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_pk.h>
#include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/c_adapter_vex.h>

#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/power/PowerCC27XX.h>
#include <ti/devices/DeviceFamily.h>

#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_vims.h)
#include DeviceFamily_constructPath(inc/hw_clkctl.h)
#include DeviceFamily_constructPath(inc/hw_hsm.h)
#include DeviceFamily_constructPath(inc/hw_hsmcrypto.h)
#include DeviceFamily_constructPath(inc/hw_tcm.h)
#include DeviceFamily_constructPath(inc/hw_ints.h)

/* Defines and enumerations */
#define BOOT_DELAY 0xFFFFF

#define SLEEP_TOKEN_WORD0 0x5F000000

#define BOOT_TOKEN_WORD0 0xCF000000

#define CRYPTO_OFFICER_ID 0x03725746

#define OUTPUT_TOKEN_ERROR 0x80000000

#define AESGCM_IV_LAST_WORD 0x01000000

#define AES_BLOCK_SIZE_ALIGN 0X0F

#define BLOCK_SIZE 16U

/* Synchronizes access to the HSM */
static SemaphoreP_Struct HSMLPF3_accessSemaphore;
/* Used by crypto drivers in blocking mode to wait on a result */
static SemaphoreP_Struct HSMLPF3_operationSemaphore;

static HwiP_Struct HSMLPF3_hwi;
static HSMLPF3_Operation operation;

static bool HSMLPF3_isInitialized   = false;
static bool HSMLPF3_rtosInitialized = false;

static int_fast16_t HSMLPF3_hsmReturnStatus;

static bool HSMLPF3_operationInProgress = false;

/* Forward declarations */
static void HSMLPF3_writeToken(const uint32_t *token, uint32_t len);
static void HSMLPF3_hwiFxn(uintptr_t arg0);
static int_fast16_t HSMLPF3_boot(void);
static void HSMLPF3_initMbox(void);
static void HSMLPF3_enableClock(void);

/* Write directly to HSM Mailbox */
static void HSMLPF3_writeToken(const uint32_t *token, uint32_t len)
{
    uint32_t i;

    /* Wait for mbx1_in_full to be false */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXSTAT) & HSMCRYPTO_MBXSTAT_MBX1INFULL) == HSMCRYPTO_MBXSTAT_MBX1INFULL)
    {}

    for (i = 0U; i < len; i++)
    {
        HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBX1IN + i*4) = token[i];
    }
    /* Mark mbx1 in as full*/
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXCTRL) = HSMCRYPTO_MBXCTRL_MBX1INFULL;
}

/*
 *  ======== HSMLPF3_hwiFxn ========
 */
static void HSMLPF3_hwiFxn(uintptr_t arg0)
{
    uint8_t MailboxNumber = HSMSAL_GetMailBoxNumber();

    (void)HwiP_clearInterrupt(INT_HSM_SEC_IRQ);
    (void)HwiP_disableInterrupt(INT_HSM_SEC_IRQ);

    /* Check which AIC interrupts have been triggered and read the result token */
    (void)HSMSAL_ScanAndReadMailbox(&operation.resultToken, MailboxNumber);

    HSMLPF3_operationInProgress = false;

    /* The HSMLPF3_hwiFxn will not be triggered in polling mode */
    if (operation.returnBehavior == HSMLPF3_RETURN_BEHAVIOR_BLOCKING)
    {
        SemaphoreP_post(&HSMLPF3_operationSemaphore);
    }
    else if (operation.returnBehavior == HSMLPF3_RETURN_BEHAVIOR_CALLBACK)
    {
        /* Call driver post-processing function and pass back the handle */
        (void)operation.callbackFxn(operation.driverHandle);
    }
}

/*
 *  ======== HSMLPF3_boot ========
 */
static int_fast16_t HSMLPF3_boot(void)
{
    uint32_t token[2];
    uint32_t moduleStatus;
    uint32_t delay;
    uint32_t result = HSMLPF3_STATUS_ERROR;

    token[0] = BOOT_TOKEN_WORD0;
    token[1] = CRYPTO_OFFICER_ID;

    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODULESTATUS) & HSMCRYPTO_MODULESTATUS_FATALERROR) ==
         HSMCRYPTO_MODULESTATUS_FATALERROR)
    {
        /* Do nothing. Error will be returned. */
    }
    else
    {
        moduleStatus = HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODULESTATUS);

        /* If HSM is already booted*/
        if ((moduleStatus & HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED) == HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED)
        {
            result = HSMLPF3_STATUS_SUCCESS;
        }
        else
        {
            HSMLPF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

            /* Wait for result in mbx1_out */
            while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXSTAT) & HSMCRYPTO_MBXSTAT_MBX1OUTFULL) !=
                    HSMCRYPTO_MBXSTAT_MBX1OUTFULL)
            {}

            if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBX1OUT) & OUTPUT_TOKEN_ERROR) != 0)
            {
                /* Notify the HSM that the mailbox has been read */
                HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXCTRL) = HSMCRYPTO_MBXCTRL_MBX1OUTEMPTY;
                /* The result returned from this path is HSMLPF3_STATUS_ERROR */
            }
            else
            {
                /* Notify the HSM that the mailbox has been read */
                HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXCTRL) = HSMCRYPTO_MBXCTRL_MBX1OUTEMPTY;

                for (delay = BOOT_DELAY; delay; delay--)
                {
                    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODULESTATUS) & HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED) ==
                         HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED)
                    {
                        result = HSMLPF3_STATUS_SUCCESS;
                        break;
                    }
                }
            }
        }
    }

    return result;
}

/*
 *  ======== HSMLPF3_initMbox ========
 */
static void HSMLPF3_initMbox(void)
{
    /* Link mailbox */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXCTRL) = HSMCRYPTO_MBXCTRL_MBX1LINK |
                                                  HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXSTAT);

    /* Allow non-secure/secure access (Set bits 7 and 3 to 1 if we need secure access) */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXLINKID) = 0x00;

    /* Make sure CPU_ID=0 host can access mailbox 1 & 2 (no lockout) */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXLOCKOUT) = 0xFFFFFF77 & HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXLOCKOUT);
}

/*
 *  ======== HSMLPF3_enableClock ========
 */
static void HSMLPF3_enableClock(void)
{
    uint32_t temp                 = 0;
    /* Disable VIMS lock, allocate 3 32KB blocks to HSM */
    temp                          = HWREG(VIMS_BASE + VIMS_O_CFG);
    HWREG(VIMS_BASE + VIMS_O_CFG) = (temp & (~VIMS_CFG_HSMSZ_M)) | (VIMS_CFG_HSMSZ_SIZE_96) | VIMS_CFG_LOCK_DIS;

    /* Set the clock, and wait for it to enable */
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET1) = CLKCTL_CLKENSET1_HSM_CLK_SET;
    while ((HWREG(CLKCTL_BASE + CLKCTL_O_CLKCFG1) & CLKCTL_CLKCFG1_HSM_M) == CLKCTL_CLKCFG1_HSM_CLK_DIS) {}

    /* Unlock CPUID0 and CPUID1 */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXLOCKOUT) = 0xFFFFFCFC;

    /* Change CPU ID to app */
    HWREG(HSM_BASE + HSM_O_CTL) = HSM_CTL_CPUIDSEL_APPID | HSM_CTL_CPUIDUNLK_UNLOCK;

    /* Lock the CPU ID to app & disable DMA firewall */
    HWREG(HSM_BASE + HSM_O_CTL) = HSM_CTL_CPUIDUNLK_LOCK | HSM_CTL_DMAFWDIS_DIS;
}

/*
 *  ======== HSMLPF3_constructRTOSObjects ========
 */
void HSMLPF3_constructRTOSObjects(void)
{
    HwiP_Params hwiParams;
    uintptr_t key;

    key = HwiP_disable();

    if (!HSMLPF3_rtosInitialized)
    {
        HwiP_Params_init(&hwiParams);
        hwiParams.priority  = (uint32_t)~0U;
        /* The interrupt is triggered when a token is submitted, and disabled when
         * a result token is received in HSMLPF3_hwiFxn
         */
        hwiParams.enableInt = false;
        (void)HwiP_construct(&HSMLPF3_hwi, INT_HSM_SEC_IRQ, HSMLPF3_hwiFxn, &hwiParams);

        (void)SemaphoreP_constructBinary(&HSMLPF3_accessSemaphore, 1U);
        (void)SemaphoreP_constructBinary(&HSMLPF3_operationSemaphore, 0U);

        HSMLPF3_rtosInitialized = true;
    }

    HwiP_restore(key);
}

/*
 *  ======== HSMLPF3_disableClock ========
 */
void HSMLPF3_disableClock(void)
{
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR1) = CLKCTL_CLKENCLR1_HSM_CLK_CLR;
    /* Wait for clock to be disabled */
    while ((HWREG(CLKCTL_BASE + CLKCTL_O_CLKCFG1) & CLKCTL_CLKCFG1_HSM_M) == CLKCTL_CLKCFG1_HSM_CLK_EN) {}
}

/*
 *  ======== HSMLPF3_sleep ========
 */
int_fast16_t HSMLPF3_sleep(void)
{
    uint32_t token[2];
    int_fast16_t result;

    token[0] = SLEEP_TOKEN_WORD0;
    token[1] = CRYPTO_OFFICER_ID;

    HSMLPF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

    /* Wait for result in mbx1_out */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXSTAT) & HSMCRYPTO_MBXSTAT_MBX1OUTFULL) !=
            HSMCRYPTO_MBXSTAT_MBX1OUTFULL)
    {}

    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBX1OUT) & OUTPUT_TOKEN_ERROR) == 0)
    {
        result = HSMLPF3_STATUS_SUCCESS;
    }
    else
    {
        result = HSMLPF3_STATUS_ERROR;
    }

    /* Mark mbx1_out as empty */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXCTRL) = HSMCRYPTO_MBXCTRL_MBX1OUTEMPTY;

    return result;
}

/*
 *  ======== HSMLPF3_init ========
 */
int_fast16_t HSMLPF3_init(void)
{
    if (!HSMLPF3_isInitialized)
    {
        /* Initialize HSM clock and mailbox, then boot it */
        HSMLPF3_enableClock();

        HSMLPF3_initMbox();

        if (HSMLPF3_boot() != HSMLPF3_STATUS_SUCCESS)
        {
            HSMLPF3_hsmReturnStatus = HSMLPF3_STATUS_ERROR;
        }
        else
        {
            if (HSMSAL_Init() != HSMSAL_SUCCESS)
            {
                HSMLPF3_hsmReturnStatus = HSMLPF3_STATUS_ERROR;
            }
            else
            {
                HSMLPF3_isInitialized = true;

                HSMLPF3_hsmReturnStatus = HSMLPF3_STATUS_SUCCESS;
            }
        }
    }

    if (HSMLPF3_isInitialized)
    {
        Power_setDependency(PowerLPF3_PERIPH_HSM);
    }

    return HSMLPF3_hsmReturnStatus;
}

/*
 *  ======== HSMLPF3_acquireLock ========
 */
bool HSMLPF3_acquireLock(uint32_t timeout, uintptr_t driverHandle)
{
    SemaphoreP_Status resourceAcquired;

    /* Try and obtain access to the crypto module */
    resourceAcquired = SemaphoreP_pend(&HSMLPF3_accessSemaphore, timeout);

    if (resourceAcquired == SemaphoreP_OK)
    {
        operation.driverHandle = driverHandle;

        (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
        (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));
    }

    return (resourceAcquired == SemaphoreP_OK);
}

/*
 *  ======== HSMLPF3_releaseLock ========
 */
void HSMLPF3_releaseLock(void)
{
    SemaphoreP_post(&HSMLPF3_accessSemaphore);
}

/*
 *  ======== HSMLPF3_submitToken ========
 */
int_fast16_t HSMLPF3_submitToken(HSMLPF3_ReturnBehavior retBehavior,
                                 HSMLPF3_CallbackFxn callbackFxn,
                                 uintptr_t driverHandle)
{
    int_fast16_t result;
    HSMSALStatus_t status;
    uintptr_t key;

    /* Verify that the caller is the driver with the HSMLPF3_accessSemaphore */
    if (driverHandle == operation.driverHandle)
    {
        /* Populate global operation struct */
        operation.returnBehavior = retBehavior;
        operation.callbackFxn    = callbackFxn;

        /* Enable interrupt in blocking and callback modes */
        if (operation.returnBehavior != HSMLPF3_RETURN_BEHAVIOR_POLLING)
        {
            HwiP_clearInterrupt(INT_HSM_SEC_IRQ);
            HwiP_enableInterrupt(INT_HSM_SEC_IRQ);
        }
        else
        {
            HwiP_clearInterrupt(INT_HSM_SEC_IRQ);
            HwiP_disableInterrupt(INT_HSM_SEC_IRQ);
        }

        /* Starting the operation and setting HSMLPF3_operationInProgress must be atomic */
        key = HwiP_disable();

        /* Submit token */
        status = HSMSAL_SubmitPhysicalToken(&operation.commandToken);

        if (status == HSMSAL_SUCCESS)
        {
            HSMLPF3_operationInProgress = true;

            HwiP_restore(key);

            result = HSMLPF3_STATUS_SUCCESS;
        }
        else
        {
            HwiP_restore(key);

            result = HSMLPF3_STATUS_ERROR;

            HwiP_clearInterrupt(INT_HSM_SEC_IRQ);
            HwiP_disableInterrupt(INT_HSM_SEC_IRQ);
        }
    }
    else
    {
        result = HSMLPF3_STATUS_ERROR;
    }

    return result;
}

/*
 *  ======== HSMLPF3_waitForResult ========
 */
int_fast16_t HSMLPF3_waitForResult(void)
{
    int_fast16_t result = HSMLPF3_STATUS_SUCCESS;
    HSMSALStatus_t hsmsalStatus;

    switch (operation.returnBehavior)
    {
        case HSMLPF3_RETURN_BEHAVIOR_POLLING:
            hsmsalStatus                = HSMSAL_WaitForResultPolling(&operation.resultToken);
            /* The above function will either return a timeout, or successfully
             * populate the result
             */
            HSMLPF3_operationInProgress = false;

            if (hsmsalStatus == HSMSAL_RESPONSE_TIMEOUT)
            {
                result = HSMLPF3_STATUS_TIMEOUT;
            }
            break;
        case HSMLPF3_RETURN_BEHAVIOR_BLOCKING:
            SemaphoreP_pend(&HSMLPF3_operationSemaphore, SemaphoreP_WAIT_FOREVER);
            break;
        case HSMLPF3_RETURN_BEHAVIOR_CALLBACK:
            /* No action necessary */
            break;
        default:
            result = HSMLPF3_STATUS_ERROR;
    }

    if ((result == HSMLPF3_STATUS_SUCCESS) && ((operation.returnBehavior == HSMLPF3_RETURN_BEHAVIOR_POLLING) ||
                                               (operation.returnBehavior == HSMLPF3_RETURN_BEHAVIOR_BLOCKING)))
    {
        if (operation.callbackFxn != NULL)
        {
            (void)operation.callbackFxn(operation.driverHandle);
        }
    }

    return result;
}

/*
 *  ======== HSMLPF3_cancelOperation ========
 */
int_fast16_t HSMLPF3_cancelOperation(void)
{
    int_fast16_t result = HSMLPF3_STATUS_SUCCESS;
    HSMSALStatus_t hsmsalStatus;

    if (HSMLPF3_isOperationInProgress())
    {
        (void)HwiP_disableInterrupt(INT_HSM_SEC_IRQ);

        /* Since the HSM cannot cancel an in-progress token, we must wait for the result to allow for subsequent token
         * submissions to succeed.
         */
        hsmsalStatus = HSMSAL_WaitForResultPolling(&operation.resultToken);

        if (hsmsalStatus == HSMSAL_RESPONSE_TIMEOUT)
        {
            result = HSMLPF3_STATUS_TIMEOUT;
        }

        (void)HwiP_clearInterrupt(INT_HSM_SEC_IRQ);

        HSMLPF3_operationInProgress = false;

        /* The post-processing function typically releases the lock and power constraint,
         * but the cancel operation is now responsible for it.
         */
        HSMLPF3_releaseLock();

        Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
    }

    return result;
}

/*
 *  ======== HSMLPF3_isOperationInProgress ========
 */
bool HSMLPF3_isOperationInProgress(void)
{
    return HSMLPF3_operationInProgress;
}

/*
 *  ================ APIs to handle result token data ================
 */

/*
 *  ======== HSMLPF3_getResultCode ========
 */
int32_t HSMLPF3_getResultCode(void)
{
    return Eip130Token_Result_Code(&operation.resultToken);
}

/*
 *  ======== HSMLPF3_getResultAssetID ========
 */
uint32_t HSMLPF3_getResultAssetID(void)
{
    return operation.resultToken.W[1];
}

/*
 *  ======== HSMLPF3_getResultDigest ========
 */
void HSMLPF3_getResultDigest(uint32_t *digest, size_t digestLength)
{
    Eip130Token_Result_Hash_CopyState(&operation.resultToken, digestLength, (uint8_t *)digest);
}

/*
 *  ======== HSMLPF3_getAESEncryptTag ========
 */
void HSMLPF3_getAESEncryptTag(uint8_t *mac)
{
    Eip130Token_Result_Crypto_CopyTag(&operation.resultToken, mac);
}

/*
 *  ======== HSMLPF3_getAESIV ========
 */
void HSMLPF3_getAESIV(uint8_t *iv)
{
    Eip130Token_Result_Crypto_CopyIV(&operation.resultToken, iv);
}

/*
 *  ======== HSMLPF3_getAESCMACSignMac ========
 */
void HSMLPF3_getAESCMACSignMac(uint8_t *mac, uint8_t macLength)
{
    Eip130Token_Result_Mac_CopyFinalMAC(&operation.resultToken, macLength, mac);
}

/*
 *  ================ APIs to construct key/asset management-related command tokens ================
 */

/*
 *  ======== HSMLPF3_constructCreateAssetToken ========
 */
void HSMLPF3_constructCreateAssetToken(uint64_t assetPolicy, uint32_t assetLength)
{
    Eip130Token_Command_AssetCreate(&operation.commandToken, assetPolicy, assetLength);
}

/*
 *  ======== HSMLPF3_constructLoadPlaintextAssetToken ========
 */
void HSMLPF3_constructLoadPlaintextAssetToken(const uint8_t *input_p, const uint32_t inputLength, uint32_t assetId)
{
    Eip130Token_Command_AssetLoad_Plaintext(&operation.commandToken, assetId);

    Eip130Token_Command_AssetLoad_SetInput(&operation.commandToken, (uintptr_t)input_p, inputLength);
}

/*
 *  ======== HSMLPF3_constructDeleteAssetToken ========
 */
void HSMLPF3_constructDeleteAssetToken(uint32_t assetId)
{
    Eip130Token_Command_AssetDelete(&operation.commandToken, (Eip130TokenAssetId_t)assetId);
}

/*
 *  ================ APIs to construct driver-specific command tokens ================
 */

/*
 *  ======== HSMLPF3_constructSHA2PhysicalToken ========
 */
void HSMLPF3_constructSHA2PhysicalToken(SHA2LPF3HSM_Object *object)
{
    bool isInitWithDefault;
    bool isFinalize;

    switch (object->mode)
    {
        case VEXTOKEN_MODE_HASH_MAC_INIT2CONT:
            isInitWithDefault = true;
            isFinalize        = false;
            break;

        case VEXTOKEN_MODE_HASH_MAC_CONT2FINAL:
            isInitWithDefault = false;
            isFinalize        = true;
            break;

        case VEXTOKEN_MODE_HASH_MAC_CONT2CONT:
            isInitWithDefault = false;
            isFinalize        = false;
            break;

        case VEXTOKEN_MODE_HASH_MAC_INIT2FINAL:
            /* For this case, we fall through to the default case. */
        default:
            /* If no valid mode we assume a one step operation to conclude a hash operation. */
            isInitWithDefault = true;
            isFinalize        = true;
            break;
    }

    if (object->key)
    {
        Eip130Token_Command_Mac(&operation.commandToken,
                                object->algorithm,
                                isInitWithDefault,
                                isFinalize,
                                (uintptr_t)object->input,
                                object->inputLength);

        /* Key ID to specify the asset from HSM asset store */
        Eip130Token_Command_Mac_SetASIDKey(&operation.commandToken, object->keyAssetID);

        if (object->tempAssetID)
        {
            Eip130Token_Command_Mac_SetASIDState(&operation.commandToken, object->tempAssetID);
        }
    }
    else
    {
        Eip130Token_Command_Hash(&operation.commandToken,
                                 object->algorithm,
                                 isInitWithDefault,
                                 isFinalize,
                                 (uintptr_t)object->input,
                                 object->inputLength);

        Eip130Token_Command_Hash_CopyState(&operation.commandToken, (uint8_t *)object->digest, object->digestLength);
    }

    Eip130Token_Command_Hash_SetTotalMessageLength(&operation.commandToken, object->totalDataLength);
}

/*
 *  ======== HSMLPF3_constructECDSASignPhysicalToken ========
 */
void HSMLPF3_constructECDSASignPhysicalToken(ECDSALPF3HSM_Object *object)
{
    const uint8_t nWord               = HSM_ASYM_DATA_SIZE_B2W(object->curveLength);
    Eip130TokenDmaAddress_t signature = (uintptr_t)object->signature;
    uint32_t signSize                 = (HSM_SIGNATURE_VCOUNT * (HSM_ASYM_DATA_SIZE_VWB(object->curveLength)));
    uint8_t command                   = VEXTOKEN_PKAS_ECDSA_SIGN;

    if (object->operationType == ECDSA_OPERATION_TYPE_VERIFY)
    {
        command = VEXTOKEN_PKAS_ECDSA_VERIFY;
    }

    Eip130Token_Command_Pk_Asset_Command(&operation.commandToken,
                                         command,
                                         nWord,
                                         nWord,
                                         0,
                                         object->keyAssetID,
                                         object->paramAssetID,
                                         0,
                                         0,
                                         0,
                                         signature,
                                         signSize);

    Eip130Token_Command_Pk_Asset_SetExplicitDigest(&operation.commandToken,
                                                   object->input,
                                                   BITS_TO_BYTES(object->digestLength));
}

/*
 *  ======== HSMLPF3_constructECDHnumLoadPhysicalToken ========
 */
void HSMLPF3_constructECDHnumLoadPhysicalToken(const uint8_t *buffer, uint8_t index, uint8_t length)
{
    Eip130Token_Command_Pk_NumLoad(&operation.commandToken, index, (uintptr_t)buffer, length);
}

/*
 *  ======== HSMLPF3_constructECDHnumSetPhysicalToken ========
 */
void HSMLPF3_constructECDHnumSetPhysicalToken(uint8_t length)
{
    Eip130Token_Command_Pk_Claim(&operation.commandToken, length, 0, 0);
}

/*
 *  ======== HSMLPF3_constructECDHPKAOperationPhysicalToken ========
 */
void HSMLPF3_constructECDHPKAOperationPhysicalToken(uint8_t commandOperation,
                                                    uint8_t *input,
                                                    uint8_t *output,
                                                    uint32_t inputLength)
{
    Eip130Token_Command_Pk_Operation(&operation.commandToken,
                                     commandOperation,
                                     0,
                                     (uintptr_t)input,
                                     inputLength,
                                     (uintptr_t)output,
                                     inputLength);
}

/*
 *  ======== HSMLPF3_constructCommonAESGCM ========
 */
static void HSMLPF3_constructCommonAESGCM(AESGCMLPF3HSM_Object *object)
{
    Eip130TokenDmaAddress_t aad = (uintptr_t)(object->aad + (object->totalAADLength - object->totalAADLengthRemaining));

    bool isEncrypt = AESGCM_OP_TYPE_ONESTEP_ENCRYPT;
    if ((object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_FINALIZE_DECRYPT))
    {
        isEncrypt = 0U;
    }

    Eip130Token_Command_Crypto_Operation(&operation.commandToken,
                                         (uint8_t)EIP130TOKEN_CRYPTO_ALGO_AES,
                                         (uint8_t)VEXTOKEN_MODE_CIPHER_GCM,
                                         isEncrypt,
                                         object->totalDataLength);

    Eip130Token_Command_Crypto_SetKeyLength(&operation.commandToken, object->common.key.u.plaintext.keyLength);

    Eip130Token_Command_Crypto_CopyKey(&operation.commandToken,
                                       object->common.key.u.plaintext.keyMaterial,
                                       object->common.key.u.plaintext.keyLength);

    Eip130Token_Command_Crypto_SetNoRandomIV(&operation.commandToken);

    Eip130Token_Command_Crypto_CopyIV(&operation.commandToken, object->iv);

    operation.commandToken.W[16] = AESGCM_IV_LAST_WORD;

    Eip130Token_Command_Crypto_CopyTag(&operation.commandToken, object->mac, object->macLength);

    Eip130Token_Command_Crypto_SetADAddress(&operation.commandToken, aad, object->totalAADLength);
}

/*
 *  ======== HSMLPF3_constructAESGCMOneStepPhysicalToken ========
 */
void HSMLPF3_constructAESGCMOneStepPhysicalToken(AESGCMLPF3HSM_Object *object)
{
    HSMLPF3_constructAESGCMSegmentedDataPhysicalToken(object);
}

/*
 *  ======== HSMLPF3_constructAESGCMSegmentedAADPhysicalToken ========
 */
void HSMLPF3_constructAESGCMSegmentedAADPhysicalToken(AESGCMLPF3HSM_Object *object)
{
    HSMLPF3_constructCommonAESGCM(object);

    /* Any part except the last must have AS_SaveIV=1 */
    Eip130Token_Command_Crypto_SetASSaveIV(&operation.commandToken, object->tempAssetID);

    /* Any part except the first must have AS_Load_IV=1 */
    if ((object->totalAADLength - object->totalAADLengthRemaining) != 0U)
    {
        Eip130Token_Command_Crypto_SetASLoadIV(&operation.commandToken, object->tempAssetID);
    }
    else
    {
        /* Do nothing. AS_Load_IV=0 */
    }

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetADPartLen(&operation.commandToken, object->aadLength);

    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken, (uintptr_t)NULL, 0U, (uintptr_t)NULL, 0U);
}

/*
 *  ======== HSMLPF3_constructAESGCMSegmentedDataPhysicalToken ========
 */
void HSMLPF3_constructAESGCMSegmentedDataPhysicalToken(AESGCMLPF3HSM_Object *object)
{
    Eip130TokenDmaAddress_t input  = (uintptr_t)(object->input +
                                                (object->totalDataLength - object->totalDataLengthRemaining));
    Eip130TokenDmaAddress_t output = (uintptr_t)(object->output +
                                                 (object->totalDataLength - object->totalDataLengthRemaining));

    uint32_t inputDataLength  = object->inputLength;
    uint32_t outputDataLength = object->inputLength;

    if (object->inputLength == object->totalDataLengthRemaining)
    {
        inputDataLength += AES_BLOCK_SIZE_ALIGN;
        inputDataLength &= (uint32_t)(~AES_BLOCK_SIZE_ALIGN);
    }

    outputDataLength = HSMLPF3_getOutputBufferLength(inputDataLength);

    HSMLPF3_constructCommonAESGCM(object);

    if (object->tempAssetID != 0U)
    {
        if (((object->totalAADLength != 0U) && (object->totalAADLengthRemaining < object->totalAADLength)) ||
            (object->totalDataLengthRemaining < object->totalDataLength))
        {
            /* Any part except the first must have AS_Load_IV=1 */
            Eip130Token_Command_Crypto_SetASLoadIV(&operation.commandToken, object->tempAssetID);
        }
        else
        {
            /* Do nothing. AS_Load_IV=0 */
        }

        /* Any part except the last must have AS_SaveIV=1 */
        if ((object->totalDataLengthRemaining - object->inputLength) > 0U)
        {
            Eip130Token_Command_Crypto_SetASSaveIV(&operation.commandToken, object->tempAssetID);
        }
        else
        {
            /* Do nothing. AS_SaveIV=0 */
        }

        Eip130Token_Command_Crypto_SetADPartLen(&operation.commandToken, object->aadLength);
    }
    else
    {
        /* Do nothing. One step operation */
    }

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken,
                                                (uintptr_t)input,
                                                inputDataLength,
                                                (uintptr_t)output,
                                                outputDataLength);
}

/*
 *  ======== HSMLPF3_constructCommonAESCCM ========
 */
static void HSMLPF3_constructCommonAESCCM(const AESCCMLPF3_Object *object)
{
    Eip130TokenDmaAddress_t aad = (uintptr_t)(object->aad + (object->totalAADLength - object->totalAADLengthRemaining));
    bool isEncrypt              = AESCCM_OP_TYPE_ONESTEP_ENCRYPT;

    if ((object->operationType == AESCCM_OP_TYPE_ONESTEP_DECRYPT) ||
        (object->operationType == AESCCM_OP_TYPE_AAD_DECRYPT) ||
        (object->operationType == AESCCM_OP_TYPE_DATA_DECRYPT) ||
        (object->operationType == AESCCM_OP_TYPE_FINALIZE_DECRYPT))
    {
        isEncrypt = 0U;
    }

    Eip130Token_Command_Crypto_Operation(&operation.commandToken,
                                         (uint8_t)EIP130TOKEN_CRYPTO_ALGO_AES,
                                         (uint8_t)VEXTOKEN_MODE_CIPHER_CCM,
                                         isEncrypt,
                                         object->totalDataLength);

    Eip130Token_Command_Crypto_SetKeyLength(&operation.commandToken, object->common.key.u.plaintext.keyLength);

    Eip130Token_Command_Crypto_CopyKey(&operation.commandToken,
                                       object->common.key.u.plaintext.keyMaterial,
                                       object->common.key.u.plaintext.keyLength);

    Eip130Token_Command_Crypto_SetNoRandomIV(&operation.commandToken);

    Eip130Token_Command_Crypto_CopyNonce(&operation.commandToken, object->nonce, (uint32_t)object->nonceLength);

    Eip130Token_Command_Crypto_CopyTag(&operation.commandToken, object->mac, object->macLength);

    Eip130Token_Command_Crypto_SetADAddress(&operation.commandToken, aad, object->totalAADLength);
}

/*
 *  ======== HSMLPF3_constructAESCCMOneStepPhysicalToken ========
 */
void HSMLPF3_constructAESCCMOneStepPhysicalToken(const AESCCMLPF3_Object *object)
{
    HSMLPF3_constructAESCCMSegmentedDataPhysicalToken(object);
}

/*
 *  ======== HSMLPF3_constructAESCCMSegmentedAADPhysicalToken ========
 */
void HSMLPF3_constructAESCCMSegmentedAADPhysicalToken(const AESCCMLPF3_Object *object)
{
    (void)HSMLPF3_constructCommonAESCCM(object);

    /* any part except the last must have AS_SaveIV=1 */
    Eip130Token_Command_Crypto_SetASSaveIV(&operation.commandToken, object->tempAssetID);

    /* any part except the first must have AS_Load_IV=1 */
    if ((object->totalAADLength - object->totalAADLengthRemaining) != 0)
    {
        Eip130Token_Command_Crypto_SetASLoadIV(&operation.commandToken, object->tempAssetID);
    }
    else
    {
        /* Do nothing. AS_Load_IV=0 */
    }

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetADPartLen(&operation.commandToken, object->aadLength);

    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken, (uintptr_t)NULL, 0, (uintptr_t)NULL, 0);
}

/*
 *  ======== HSMLPF3_constructAESCCMSegmentedDataPhysicalToken ========
 */
void HSMLPF3_constructAESCCMSegmentedDataPhysicalToken(const AESCCMLPF3_Object *object)
{
    Eip130TokenDmaAddress_t input  = (uintptr_t)(object->input +
                                                (object->totalDataLength - object->totalDataLengthRemaining));
    Eip130TokenDmaAddress_t output = (uintptr_t)(object->output +
                                                 (object->totalDataLength - object->totalDataLengthRemaining));

    uint32_t inputDataLength  = object->inputLength;
    uint32_t outputDataLength = object->inputLength;

    if (object->inputLength == object->totalDataLengthRemaining)
    {
        inputDataLength += AES_BLOCK_SIZE_ALIGN;
        inputDataLength &= (uint32_t)(~AES_BLOCK_SIZE_ALIGN);
    }

    outputDataLength = HSMLPF3_getOutputBufferLength(inputDataLength);

    (void)HSMLPF3_constructCommonAESCCM(object);

    if (object->tempAssetID != 0)
    {
        if ((object->totalAADLength != 0 && object->totalAADLengthRemaining < object->totalAADLength) ||
            (object->totalDataLengthRemaining < object->totalDataLength))
        {
            /* any part except the first must have AS_Load_IV=1 */
            Eip130Token_Command_Crypto_SetASLoadIV(&operation.commandToken, object->tempAssetID);
        }
        else
        {
            /* Do nothing. AS_Load_IV=0 */
        }

        /* any part except the last must have AS_SaveIV=1 */
        if ((object->totalDataLengthRemaining - object->inputLength) > 0)
        {
            Eip130Token_Command_Crypto_SetASSaveIV(&operation.commandToken, object->tempAssetID);
        }
        else
        {
            /* Do nothing. AS_SaveIV=0 */
        }

        Eip130Token_Command_Crypto_SetADPartLen(&operation.commandToken, object->aadLength);
    }
    else
    {
        /* Do nothing. One step operation */
    }

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken,
                                                (uintptr_t)input,
                                                inputDataLength,
                                                (uintptr_t)output,
                                                outputDataLength);
}

/*
 *  ======== HSMLPF3_constructCommonAESECB ========
 */
static void HSMLPF3_constructCommonAESECB(AESECBLPF3_Object *object)
{
    bool isEncrypt = AESECB_OPERATION_TYPE_ENCRYPT;

    if ((object->operationType == AESECB_OPERATION_TYPE_DECRYPT) ||
        (object->operationType == AESECB_OPERATION_TYPE_DECRYPT_SEGMENTED) ||
        (object->operationType == AESECB_OPERATION_TYPE_FINALIZE_DECRYPT_SEGMENTED))
    {
        isEncrypt = 0U;
    }

    Eip130Token_Command_Crypto_Operation(&operation.commandToken,
                                         (uint8_t)EIP130TOKEN_CRYPTO_ALGO_AES,
                                         (uint8_t)VEXTOKEN_MODE_CIPHER_ECB,
                                         isEncrypt,
                                         object->operation->inputLength);

    Eip130Token_Command_Crypto_SetKeyLength(&operation.commandToken, object->common.key.u.plaintext.keyLength);

    Eip130Token_Command_Crypto_CopyKey(&operation.commandToken,
                                       object->common.key.u.plaintext.keyMaterial,
                                       object->common.key.u.plaintext.keyLength);
}

/*
 *  ======== HSMLPF3_constructAESECBOneStepPhysicalToken ========
 */
void HSMLPF3_constructAESECBOneStepPhysicalToken(AESECBLPF3_Object *object)
{
    Eip130TokenDmaAddress_t input  = (uintptr_t)object->operation->input;
    Eip130TokenDmaAddress_t output = (uintptr_t)object->operation->output;

    uint32_t inputDataLength  = object->operation->inputLength;
    uint32_t outputDataLength = object->operation->inputLength;

    outputDataLength = HSMLPF3_getOutputBufferLength(inputDataLength);

    HSMLPF3_constructCommonAESECB(object);

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken,
                                                input,
                                                inputDataLength,
                                                output,
                                                outputDataLength);
}

/*
 *  ======== HSMLPF3_constructCommonAESCTR ========
 */
static void HSMLPF3_constructCommonAESCTR(AESCTRLPF3_Object *object)
{
    uint32_t srcDataLength = object->inputLength;
    bool isEncrypt         = AESCTR_OPERATION_TYPE_ENCRYPT;

    srcDataLength += AES_BLOCK_SIZE_ALIGN;
    srcDataLength &= (uint32_t)(~AES_BLOCK_SIZE_ALIGN);

    if ((object->operationType == AESCTR_OPERATION_TYPE_DECRYPT) ||
        (object->operationType == AESCTR_OPERATION_TYPE_DECRYPT_SEGMENTED) ||
        (object->operationType == AESCTR_OPERATION_TYPE_DECRYPT_FINALIZE))
    {
        isEncrypt = 0U;
    }

    Eip130Token_Command_Crypto_Operation(&operation.commandToken,
                                         (uint8_t)EIP130TOKEN_CRYPTO_ALGO_AES,
                                         (uint8_t)VEXTOKEN_MODE_CIPHER_CTR,
                                         isEncrypt,
                                         srcDataLength);

    Eip130Token_Command_Crypto_SetKeyLength(&operation.commandToken, object->common.key.u.plaintext.keyLength);

    Eip130Token_Command_Crypto_CopyKey(&operation.commandToken,
                                       object->common.key.u.plaintext.keyMaterial,
                                       object->common.key.u.plaintext.keyLength);

    Eip130Token_Command_Crypto_CopyIV(&operation.commandToken, (uint8_t *)&object->counter[0]);
}

/*
 *  ======== HSMLPF3_constructAESCTROneStepPhysicalToken ========
 */
void HSMLPF3_constructAESCTROneStepPhysicalToken(AESCTRLPF3_Object *object)
{
    uint32_t inputDataLength  = object->inputLength;
    uint32_t outputDataLength = object->inputLength;

    inputDataLength += AES_BLOCK_SIZE_ALIGN;
    inputDataLength &= (uint32_t)(~AES_BLOCK_SIZE_ALIGN);

    outputDataLength = HSMLPF3_getOutputBufferLength(inputDataLength);

    HSMLPF3_constructCommonAESCTR(object);

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken,
                                                (uintptr_t)object->input,
                                                inputDataLength,
                                                (uintptr_t)object->output,
                                                outputDataLength);
}

/*
 *  ======== HSMLPF3_constructCommonAESCBC ========
 */
static void HSMLPF3_constructCommonAESCBC(AESCBCLPF3_Object *object)
{
    uint32_t srcDataLength = object->inputLength;

    bool isEncrypt = AESCBC_OPERATION_TYPE_ENCRYPT;
    if ((object->operationType == AESCBC_OP_TYPE_ONESTEP_DECRYPT) ||
        (object->operationType == AESCBC_OP_TYPE_DECRYPT_SEGMENTED) ||
        (object->operationType == AESCBC_OP_TYPE_FINALIZE_DECRYPT_SEGMENTED))
    {
        isEncrypt = 0U;
    }

    Eip130Token_Command_Crypto_Operation(&operation.commandToken,
                                         (uint8_t)EIP130TOKEN_CRYPTO_ALGO_AES,
                                         (uint8_t)VEXTOKEN_MODE_CIPHER_CBC,
                                         isEncrypt,
                                         srcDataLength);

    Eip130Token_Command_Crypto_SetKeyLength(&operation.commandToken, object->common.key.u.plaintext.keyLength);

    Eip130Token_Command_Crypto_CopyKey(&operation.commandToken,
                                       object->common.key.u.plaintext.keyMaterial,
                                       object->common.key.u.plaintext.keyLength);

    Eip130Token_Command_Crypto_CopyIV(&operation.commandToken, (uint8_t *)&object->iv[0]);
}

/*
 *  ======== HSMLPF3_constructAESCBCOneStepPhysicalToken ========
 */
void HSMLPF3_constructAESCBCOneStepPhysicalToken(AESCBCLPF3_Object *object)
{
    uint32_t inputDataLength  = object->inputLength;
    uint32_t outputDataLength = object->inputLength;

    outputDataLength = HSMLPF3_getOutputBufferLength(inputDataLength);

    HSMLPF3_constructCommonAESCBC(object);

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken,
                                                (uintptr_t)object->input,
                                                inputDataLength,
                                                (uintptr_t)object->output,
                                                outputDataLength);
}

/*
 *  ======== HSMLPF3_constructAESCMACOneStepPhysicalToken ========
 */
void HSMLPF3_constructAESCMACOneStepPhysicalToken(AESCMACLPF3_Object *object)
{
    uint8_t algorithm             = VEXTOKEN_ALGO_MAC_AES_CBC_MAC;
    bool isInitWithDefault        = true;
    Eip130TokenDmaAddress_t input = (uintptr_t)object->operation->input;
    uint32_t inputLength          = object->operation->inputLength;
    uint32_t padbytes             = 0U;

    if (object->operationalMode == AESCMAC_OPMODE_CMAC)
    {
        algorithm = (uint32_t)VEXTOKEN_ALGO_MAC_AES_CMAC;
    }

    inputLength = (inputLength + BLOCK_SIZE - 1U) & ~(BLOCK_SIZE - 1U);
    if ((inputLength == 0U) && (algorithm == VEXTOKEN_ALGO_MAC_AES_CMAC))
    {
        inputLength += BLOCK_SIZE;
    }

    padbytes = inputLength - object->operation->inputLength;
    if ((padbytes > 0U) && (algorithm == VEXTOKEN_ALGO_MAC_AES_CMAC))
    {
        uint8_t *data_p = object->operation->input + object->operation->inputLength;
        *data_p         = 0x80;

        if (padbytes == BLOCK_SIZE)
        {
            padbytes--;
        }
    }

    if (object->tempAssetID)
    {
        isInitWithDefault = false;

        Eip130Token_Command_Mac_SetASIDState(&operation.commandToken, object->tempAssetID);
    }

    Eip130Token_Command_Mac(&operation.commandToken, algorithm, isInitWithDefault, true, input, inputLength);

    Eip130Token_Command_Mac_SetASIDKey(&operation.commandToken, object->keyAssetID);

    if ((object->operationType & AESCMAC_OP_FLAG_SIGN) == 0U)
    {
        Eip130Token_Command_Mac_CopyVerifyMAC(&operation.commandToken,
                                              object->operation->mac,
                                              object->operation->macLength);
    }

    Eip130Token_Command_Mac_SetTotalMessageLength(&operation.commandToken, (uint64_t)padbytes);
}

/*
 *  ======== HSMLPF3_constructAESCMACUpdatePhysicalToken ========
 */
void HSMLPF3_constructAESCMACUpdatePhysicalToken(AESCMACLPF3_Object *object, bool isInitWithDefault)
{
    uint8_t algorithm = VEXTOKEN_ALGO_MAC_AES_CBC_MAC;

    Eip130TokenDmaAddress_t input = (uintptr_t)object->operation->input;

    if (object->operationalMode == AESCMAC_OPMODE_CMAC)
    {
        algorithm = (uint32_t)VEXTOKEN_ALGO_MAC_AES_CMAC;
    }

    Eip130Token_Command_Mac_SetASIDState(&operation.commandToken, object->tempAssetID);

    Eip130Token_Command_Mac(&operation.commandToken,
                            algorithm,
                            isInitWithDefault,
                            false,
                            input,
                            object->operation->inputLength);

    Eip130Token_Command_Mac_SetASIDKey(&operation.commandToken, object->keyAssetID);
}
