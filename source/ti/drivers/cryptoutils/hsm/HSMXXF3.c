/*
 * Copyright (c) 2023-2025, Texas Instruments Incorporated
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

#include <ti/drivers/cryptoutils/hsm/HSMXXF3.h>
#include <ti/drivers/cryptoutils/hsm/HSMXXF3Utility.h>

#include <ti/drivers/SHA2.h>
#include <ti/drivers/sha2/SHA2XXF3HSM.h>

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #include <ti/drivers/EDDSA.h>
    #include <ti/drivers/eddsa/EDDSAXXF3HSM.h>
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

#include <ti/drivers/TRNG.h>
#include <ti/drivers/trng/TRNGXXF3HSM.h>

#include <ti/drivers/AESGCM.h>
#include <ti/drivers/aesgcm/AESGCMXXF3HSM.h>

#include <ti/drivers/AESECB.h>
#include <ti/drivers/aesecb/AESECBXXF3.h>

#include <ti/drivers/AESCCM.h>
#include <ti/drivers/aesccm/AESCCMXXF3.h>

#include <ti/drivers/AESCTR.h>
#include <ti/drivers/aesctr/AESCTRXXF3.h>

#include <ti/drivers/AESCBC.h>
#include <ti/drivers/aescbc/AESCBCXXF3.h>

#include <ti/drivers/AESCMAC.h>
#include <ti/drivers/aescmac/AESCMACXXF3.h>

#include <ti/drivers/ECDH.h>
#include <ti/drivers/ecdh/ECDHXXF3HSM.h>

#include <ti/drivers/ECDSA.h>
#include <ti/drivers/ecdsa/ECDSAXXF3HSM.h>

#include <third_party/hsmddk/include/Integration/HSMSAL/HSMSAL.h>
#include <third_party/hsmddk/include/Kit/EIP130/DomainHelper/incl/eip130_domain_ecc_curves.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_asset.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_common.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_crypto.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_hash.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_mac.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_pk.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_publicdata.h>
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_random.h>
#include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/c_adapter_vex.h>
#include <third_party/hsmddk/include/Kit/EIP201/incl/eip201.h>
#include <third_party/hsmddk/include/Kit/DriverFramework/Device_API/incl/device_mgmt.h>
#include <third_party/hsmddk/include/Integration/Adapter_Generic/incl/adapter_interrupts.h>

#include <ti/drivers/cryptoutils/cryptokey/CryptoKey.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/HwiP.h>

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #include <ti/drivers/power/PowerCC27XX.h>
#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    #include <ti/drivers/power/PowerWFF3.h>
#endif

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    #include DeviceFamily_constructPath(inc/hw_memmap.h)
    #include DeviceFamily_constructPath(inc/hw_types.h)
    #include DeviceFamily_constructPath(inc/hw_hsm.h)
    #include DeviceFamily_constructPath(inc/hw_hsm_sec.h)
    #include DeviceFamily_constructPath(inc/hw_hsm_non_sec.h)
    #include DeviceFamily_constructPath(inc/hw_ints.h)
    #include DeviceFamily_constructPath(inc/hw_soc_aon.h)
#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #include DeviceFamily_constructPath(inc/hw_memmap.h)
    #include DeviceFamily_constructPath(inc/hw_types.h)
    #include DeviceFamily_constructPath(inc/hw_vims.h)
    #include DeviceFamily_constructPath(inc/hw_clkctl.h)
    #include DeviceFamily_constructPath(inc/hw_hsm.h)
    #include DeviceFamily_constructPath(inc/hw_hsmcrypto.h)
    #include DeviceFamily_constructPath(inc/hw_tcm.h)
    #include DeviceFamily_constructPath(inc/hw_ints.h)
#endif

/* Defines and enumerations */
typedef struct
{
    uint8_t patch;
    uint8_t minor;
    uint8_t major;
} HSMXXF3_systemInfoVersion_t;

typedef struct
{
    /* Word 0 is omitted */
    /* Word 1 */
    HSMXXF3_systemInfoVersion_t rambusFwVersion;
    uint8_t rollbackID;
    /* Word 2 */
    HSMXXF3_systemInfoVersion_t rambusHwVersion;
    uint8_t res0;
    /* Word 3 */
    uint16_t memorySize;
    uint16_t hostId:3;
    uint16_t ns:1;
    uint16_t res1:7;
    uint16_t co:1;
    uint16_t mode:4;
    /* Word 4 */
    uint32_t identity;
    /* Word 5 */
    uint16_t res2:12;
    uint16_t otpAnomaly:4;
    uint16_t selfTestActive:16;
    /* Word 6 */
    HSMXXF3_systemInfoVersion_t rambusBootFwVersion;
    uint8_t res3;
    /* Word 7 */
    HSMXXF3_systemInfoVersion_t customBootFwVersion;
    uint8_t res4;
    /* Word 8 */
    HSMXXF3_systemInfoVersion_t customFwVersion;
    uint8_t res5;
    /* Word 9 */
    HSMXXF3_systemInfoVersion_t customHwVersion;
    uint8_t res6;
} HSMXXF3_SystemInfo_t;

#define BOOT_DELAY 0xFFFFF

#define SLEEP_TOKEN_WORD0 0x4F000000

#define WAKEUP_TOKEN_WORD0 0x5F000000

#define RNG_CONFIG_TOKEN_WORD0 0x14000000
#define RNG_CONFIG_TOKEN_WORD2 0x00000004
#define RNG_CONFIG_TOKEN_CRNG  0x00000010

#define BOOT_TOKEN_WORD0 0xCF000000

#define BOOT_TOKEN_WORD1 0x03725746

#define SYSTEMINFO_TOKEN_WORD0 0x0F030000

#define CRYPTO_OFFICER_ID 0x4F5A3647

#define OUTPUT_TOKEN_ERROR 0x80000000

#define AESGCM_IV_LAST_WORD 0x01000000

#define AES_BLOCK_SIZE_ALIGN 0X0F

#define BLOCK_SIZE 16U

#define HSM_TOKEN_WORD1_OFFSET 0x4

#define HSM_HUK_ALREADY_PROVISIONED 0x87

#define HUK_PROVISION_TOKEN_WORD0 0x97000000

/* bit 17 has to be high to indicate a 256bit HUK asset size. */
#define HUK_PROVISION_TOKEN_WORD2_256BIT 0x00020000
#define HUK_PROVISION_TOKEN_WORD2        RNG_CONFIG_TOKEN_WORD2 | HUK_PROVISION_TOKEN_WORD2_256BIT

#define HSM_CRNG_RAW_KEY_ENC 0x7264
#define HSM_TRNG_RAW_KEY_ENC 0x5244

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    /* HSM Register names for CC35XX are different compared to CC27XX
     * Below mapping helps to keep the source code same between
     * both devices.
     */
    #define HSMCRYPTO_BASE              HSM_BASE
    #define HSMCRYPTO_O_MBSTA           HSM_O_MAILBOX_STAT
    #define HSMCRYPTO_MBSTA_MB1IN       HSM_MAILBOX_STAT_INFULL1
    #define HSMCRYPTO_MBSTA_MB1IN_FULL  HSM_MAILBOX_STAT_INFULL1
    #define HSMCRYPTO_O_MB1IN           HSM_O_EIP130_072_MAILBOX1_IN
    #define HSMCRYPTO_O_MBCTL           HSM_O_MBXCTL
    #define HSMCRYPTO_MBCTL_MB1IN_FULL  HSM_MBXCTL_INFULL1
    #define INT_HSM_SEC_IRQ             INT_OSPR_HSM_HOST_0_SEC_IRQ
    #define HSMCRYPTO_MBCTL_MB1LNK_LNK  HSM_MBXCTL_LINK1
    #define HSMCRYPTO_O_MBLNKID         HSM_O_MAILBOX_LINKID
    #define HSMCRYPTO_O_MBLCKOUT        HSM_O_MAILBOX_LOCKOUT
    #define HSMCRYPTO_MBSTA_MB1OUT_M    HSM_MAILBOX_STAT_OUTFULL1
    #define HSMCRYPTO_MBSTA_MB1OUT_FULL HSM_MAILBOX_STAT_OUTFULL1
    #define HSMCRYPTO_O_MB1OUT          HSM_O_EIP130_072_MAILBOX1_IN
    #define HSMCRYPTO_MBCTL_MB1OUT_EMTY HSM_MBXCTL_OUTEMP1
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX) */

/* Synchronizes access to the HSM */
static SemaphoreP_Struct HSMXXF3_accessSemaphore;
/* Used by crypto drivers in blocking mode to wait on a result */
static SemaphoreP_Struct HSMXXF3_operationSemaphore;

static HwiP_Struct HSMXXF3_hwi;
static HSMXXF3_Operation operation;

static bool HSMXXF3_isInitialized   = false;
static bool HSMXXF3_rtosInitialized = false;

static int_fast16_t HSMXXF3_hsmReturnStatus;

static bool HSMXXF3_operationInProgress = false;

static bool HSMXXF3_isHSMInSleepMode = false;

static Power_NotifyObj postNotify;

/* Keep a global variable to track the overall HSM RNG NRBG engine mode */
static HSMXXF3_NRBGMode HSMXXF3_nrbgMode = HSMXXF3_MODE_CRNG;

/* Forward declarations */
static void HSMXXF3_writeToken(const uint32_t *token, uint32_t len);
static void HSMXXF3_hwiFxn(uintptr_t arg0);
static int_fast16_t HSMXXF3_boot(void);
static void HSMXXF3_initMbox(void);
static void HSMXXF3_enableClock(void);
static void HSMXXF3_initAIC(void);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
static int_fast16_t HSMXXF3_submitResetToken(void);
static int_fast16_t HSMXXF3_isHSMfirmwareImgAccepted(void);

/*
 *  ======== HSMXXF3_isHSMfirmwareImgAccepted ========
 */
static int_fast16_t HSMXXF3_isHSMfirmwareImgAccepted(void)
{
    return ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODSTA) & HSMCRYPTO_MODSTA_FWACPTD_M) == HSMCRYPTO_MODSTA_FWACPTD);
}

#endif

/* Write directly to HSM Mailbox */
static void HSMXXF3_writeToken(const uint32_t *token, uint32_t len)
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
 *  ======== HSMXXF3_hwiFxn ========
 */
static void HSMXXF3_hwiFxn(uintptr_t arg0)
{
    uint8_t MailboxNumber = HSMSAL_GetMailBoxNumber();

    (void)HwiP_clearInterrupt(INT_HSM_SEC_IRQ);
    (void)HwiP_disableInterrupt(INT_HSM_SEC_IRQ);

    /* Check which AIC interrupts have been triggered and read the result token */
    (void)HSMSAL_ScanAndReadMailbox(&operation.resultToken, MailboxNumber);

    HSMXXF3_operationInProgress = false;

    /* The HSMXXF3_hwiFxn will not be triggered in polling mode */
    if (operation.returnBehavior == HSMXXF3_RETURN_BEHAVIOR_BLOCKING)
    {
        SemaphoreP_post(&HSMXXF3_operationSemaphore);
    }
    else if (operation.returnBehavior == HSMXXF3_RETURN_BEHAVIOR_CALLBACK)
    {
        if (operation.callbackFxn != NULL)
        {
            /* Call driver post-processing function and pass back the handle */
            (void)operation.callbackFxn(operation.driverHandle);
        }
    }
}

/*
 *  ======== HSMXXF3_postNotifyFxn ========
 */
static int_fast16_t HSMXXF3_postNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg)
{
    int_fast16_t result = Power_NOTIFYERROR;

    /* Send in a sleep/wakeup token depending on eventType */
    if (eventType == PowerLPF3_ENTERING_STANDBY)
    {
        if (HSMXXF3_sleep() == HSMXXF3_STATUS_SUCCESS)
        {
            result = Power_NOTIFYDONE;
        }
    }
    else if (eventType == PowerLPF3_AWAKE_STANDBY)
    {
        result = Power_NOTIFYDONE;
    }
    else
    {
        /* Do nothing. */
    }

    return result;
}

/*
 *  ======== HSMXXF3_boot ========
 */
static int_fast16_t HSMXXF3_boot(void)
{
    uint32_t token[2];
    uint32_t result = HSMXXF3_STATUS_ERROR;

    /* Initialize HSM clock and mailbox, then boot it */
    HSMXXF3_enableClock();

    HSMXXF3_initMbox();

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    uint32_t delay;
    token[0] = BOOT_TOKEN_WORD0;
    token[1] = BOOT_TOKEN_WORD1;

    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODSTA) & HSMCRYPTO_MODSTA_FATAL_M) == HSMCRYPTO_MODSTA_FATAL)
    {
        /* Do nothing. Error will be returned. */
    }
    else
    {
        /* If HSM is already booted*/
        if (HSMXXF3_isHSMfirmwareImgAccepted())
        {
            result = HSMXXF3_STATUS_SUCCESS;
        }
        else
        {
            HSMXXF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

            /* Wait for result in mbx1_out */
            while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) !=
                    HSMCRYPTO_MBSTA_MB1OUT_FULL)
            {}

            if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR) != 0)
            {
                /* Notify the HSM that the mailbox has been read */
                HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;
                /* The result returned from this path is HSMXXF3_STATUS_ERROR */
            }
            else
            {
                /* Notify the HSM that the mailbox has been read */
                HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

                for (delay = BOOT_DELAY; delay; delay--)
                {
                    if (HSMXXF3_isHSMfirmwareImgAccepted())
                    {
                        result = HSMXXF3_STATUS_SUCCESS;

                        break;
                    }
                }
            }
        }
    }
#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    uint32_t moduleStatus;
    token[0] = SYSTEMINFO_TOKEN_WORD0;
    token[1] = CRYPTO_OFFICER_ID;

    if ((HWREG(HSMCRYPTO_BASE + HSM_O_MODULE_STATUS) & HSM_MODULE_STATUS_FATALERR) == HSM_MODULE_STATUS_FATALERR)
    {
        /* Do nothing. Error will be returned. */
    }
    else
    {
        moduleStatus = HWREG(HSMCRYPTO_BASE + HSM_O_MODULE_STATUS);
        if ((moduleStatus & HSM_MODULE_STATUS_FATALERR) != HSM_MODULE_STATUS_FATALERR)
        {
            /* HSM is already booted, when the clocks are applied. */
            HSMXXF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

            /* Wait for result in mbx1_out */
            while ((HWREG(HSMCRYPTO_BASE + HSM_O_MAILBOX_STAT) & HSM_MAILBOX_STAT_OUTFULL1) !=
                    HSM_MAILBOX_STAT_OUTFULL1)
            {}

            if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR) != 0)
            {
                /* Notify the HSM that the mailbox has been read */
                HWREG(HSMCRYPTO_BASE + HSM_O_MBXCTL) = HSM_MBXCTL_OUTEMP1;
                /* The result returned from this path is HSMXXF3_STATUS_ERROR */
            }
            else
            {
                /* Notify the HSM that the mailbox has been read */
                HWREG(HSMCRYPTO_BASE + HSM_O_MBXCTL) = HSM_MBXCTL_OUTEMP1;
                result                               = HSMXXF3_STATUS_SUCCESS;
                /* Register power notification function */
                Power_registerNotify(&postNotify, PowerWFF3_ENTERING_SLEEP, HSMXXF3_postNotifyFxn, (uintptr_t)NULL);
            }
        }
    }
#endif

    return result;
}

/*
 *  ======== HSMXXF3_initMbox ========
 */
static void HSMXXF3_initMbox(void)
{
    /* Link mailbox */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1LNK_LNK | HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA);

    /* Allow non-secure/secure access (Set bits 7 and 3 to 1 if we need secure access) */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLNKID) = 0x00;

    /* Make sure CPU_ID=0 host can access mailbox 1 & 2 (no lockout) */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLCKOUT) = 0xFFFFFF77 & HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLCKOUT);
}

/*
 *  ======== HSMXXF3_initAIC ========
 */
static void HSMXXF3_initAIC(void)
{
    Device_Handle_t gl_Aic = Device_Find("EIP130_AIC");
    if (gl_Aic != NULL)
    {
        /* Configure them all for an edge detect.  We should probably
         * only have the interrupts we need.  We could also use
         * EIP201_Config_Change(), but there's no adapter function(!)
         */
        EIP201_SourceSettings_t settings = {
            .Source  = 0xFF,
            .Config  = EIP201_CONFIG_RISING_EDGE,
            .fEnable = false /* enable source only when active */
        };
        EIP201_Initialize(gl_Aic, &settings, 1);
    }
}

/*
 *  ======== HSMXXF3_enableClock ========
 */
static void HSMXXF3_enableClock(void)
{
    uint32_t temp = 0;

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)

    /* Disable VIMS lock, allocate 3 32KB blocks to HSM */
    temp                          = HWREG(VIMS_BASE + VIMS_O_CFG);
    HWREG(VIMS_BASE + VIMS_O_CFG) = (temp & (~VIMS_CFG_HSMSZ_M)) | (VIMS_CFG_HSMSZ_SIZE_96) | VIMS_CFG_LOCK_DIS;

    /* Set the clock, and wait for it to enable */
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET1) = CLKCTL_CLKENSET1_HSM_CLK_SET;
    while ((HWREG(CLKCTL_BASE + CLKCTL_O_CLKCFG1) & CLKCTL_CLKCFG1_HSM_M) == CLKCTL_CLKCFG1_HSM_CLK_DIS) {}

    /* Unlock CPUID0 and CPUID1 */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLCKOUT) = 0xFFFFFCFC;

    /* Change CPU ID to app */
    HWREG(HSM_BASE + HSM_O_CTL) = HSM_CTL_CPUIDSEL_APPID | HSM_CTL_CPUIDUNLK_UNLOCK;

    /* Lock the CPU ID to app & disable DMA firewall */
    HWREG(HSM_BASE + HSM_O_CTL) = HSM_CTL_CPUIDUNLK_LOCK | HSM_CTL_DMAFWDIS_DIS;

#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)

    /* Initialize HSM Clock */
    HWREG(HSM_NON_SEC_BASE + HSM_NON_SEC_O_CLK_MEM_CTRL) = 0x3F;

    temp                                   = HWREG(HSM_SEC_BASE + HSM_SEC_O_CLKCTL);
    HWREG(HSM_SEC_BASE + HSM_SEC_O_CLKCTL) = temp | (HSM_SEC_CLKCTL_CLKGO_EN | HSM_SEC_CLKCTL_HIFCLKGO_EN |
                                                     HSM_SEC_CLKCTL_CNTCLKGO_EN);

    /* Unlock CPUID0 and CPUID1 */
    HWREG(HSMCRYPTO_BASE + HSM_O_MAILBOX_LOCKOUT) = 0xFFFFFCFC;
#endif /* (DeviceFamily_PARENT != DeviceFamily_PARENT_CC35XX) */
}

/*
 *  ======== HSMXXF3_constructRTOSObjects ========
 */
void HSMXXF3_constructRTOSObjects(void)
{
    HwiP_Params hwiParams;
    uintptr_t key;

    key = HwiP_disable();

    if (!HSMXXF3_rtosInitialized)
    {
        HwiP_Params_init(&hwiParams);
        hwiParams.priority  = (uint32_t)~0U;
        /* The interrupt is triggered when a token is submitted, and disabled when
         * a result token is received in HSMXXF3_hwiFxn
         */
        hwiParams.enableInt = false;
        (void)HwiP_construct(&HSMXXF3_hwi, INT_HSM_SEC_IRQ, HSMXXF3_hwiFxn, &hwiParams);

        (void)SemaphoreP_constructBinary(&HSMXXF3_accessSemaphore, 1U);
        (void)SemaphoreP_constructBinary(&HSMXXF3_operationSemaphore, 0U);

        HSMXXF3_rtosInitialized = true;
    }

    HwiP_restore(key);
}

/*
 *  ======== HSMXXF3_disableClock ========
 */
void HSMXXF3_disableClock(void)
{
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENCLR1) = CLKCTL_CLKENCLR1_HSM_CLK_CLR;
    /* Wait for clock to be disabled */
    while ((HWREG(CLKCTL_BASE + CLKCTL_O_CLKCFG1) & CLKCTL_CLKCFG1_HSM_M) == CLKCTL_CLKCFG1_HSM_CLK_EN) {}
#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    uint32_t temp                                 = 0;
    temp                                          = HWREG(HSM_SEC_BASE + HSM_SEC_O_CLKCTL);
    /* Clear bits to disable clock(s) */
    HWREG(HSM_SEC_BASE + HSM_SEC_O_CLKCTL)        = temp & ~(HSM_SEC_CLKCTL_CLKGO_EN | HSM_SEC_CLKCTL_HIFCLKGO_EN |
                                                      HSM_SEC_CLKCTL_CNTCLKGO_EN);

    temp                                                 = HWREG(HSM_NON_SEC_BASE + HSM_NON_SEC_O_CLK_MEM_CTRL);
    HWREG(HSM_NON_SEC_BASE + HSM_NON_SEC_O_CLK_MEM_CTRL) = temp & ~(0x3F);
#endif
}

/*
 *  ======== HSMXXF3_sleep ========
 */
int_fast16_t HSMXXF3_sleep(void)
{
    int_fast16_t result = HSMXXF3_STATUS_ERROR;
    uint32_t token[2];

    if (!HSMXXF3_isHSMInSleepMode)
    {
        token[0] = SLEEP_TOKEN_WORD0;
        token[1] = CRYPTO_OFFICER_ID;

        HSMXXF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

        /* Wait for result in mbx1_out */
        while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL) {}

        if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR) == 0)
        {
            result = HSMXXF3_STATUS_SUCCESS;

            /* Mark mbx1_out as empty */
            HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

            HSMXXF3_disableClock();

            HSMXXF3_isHSMInSleepMode = true;
        }
    }
    else
    {
        result = HSMXXF3_STATUS_SUCCESS;
    }

    return result;
}

/*
 *  ======== HSMXXF3_wakeUp ========
 */
int_fast16_t HSMXXF3_wakeUp(void)
{
    int_fast16_t result = HSMXXF3_STATUS_ERROR;
    uint32_t token[2];

    if (HSMXXF3_isHSMInSleepMode)
    {
        HSMXXF3_enableClock();

        HSMXXF3_initMbox();

        HSMXXF3_initAIC();

        token[0] = WAKEUP_TOKEN_WORD0;
        token[1] = CRYPTO_OFFICER_ID;

        HSMXXF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

        /* Wait for result in mbx1_out */
        while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL) {}

        if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR) == 0)
        {
            result = HSMXXF3_STATUS_SUCCESS;

            HSMXXF3_isHSMInSleepMode = false;
        }

        /* Mark mbx1_out as empty */
        HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;
    }
    else
    {
        result = HSMXXF3_STATUS_SUCCESS;
    }

    return result;
}

/*
 *  ======== HSMXXF3_init ========
 */
int_fast16_t HSMXXF3_init(void)
{
    uintptr_t key;

    if (!HSMXXF3_isInitialized)
    {
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
        /* Disable HSM Firewall on CC35XX. */
        HWREG(SOC_AON_BASE + SOC_AON_O_HSMCFG) |= SOC_AON_HSMCFG_FIREWALL;
#endif

        key = HwiP_disable();

        if (HSMXXF3_boot() != HSMXXF3_STATUS_SUCCESS)
        {
            HSMXXF3_hsmReturnStatus = HSMXXF3_STATUS_ERROR;

            HwiP_restore(key);
        }
        else
        {
            HwiP_restore(key);

            if (HSMSAL_Init() != HSMSAL_SUCCESS)
            {
                /* HSMSAL_Init() can fail if HSM interrupt engine is unresponsive. */
                HSMXXF3_hsmReturnStatus = HSMXXF3_STATUS_ERROR;
            }
            else
            {
                HSMXXF3_isInitialized = true;

                /* Register power notification function */
                Power_registerNotify(&postNotify, PowerLPF3_ENTERING_STANDBY, HSMXXF3_postNotifyFxn, (uintptr_t)0U);

                HSMXXF3_hsmReturnStatus = HSMXXF3_STATUS_SUCCESS;
            }
        }

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
        HSMXXF3_nrbgMode = HSMXXF3_MODE_TRNG;
#else
        HSMXXF3_nrbgMode = HSMXXF3_MODE_CRNG;
#endif
    }

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    if (HSMXXF3_isInitialized)
    {
        Power_setDependency(PowerLPF3_PERIPH_HSM);
    }
#endif

    return HSMXXF3_hsmReturnStatus;
}

/*
 *  ======== HSMXXF3_provisionHUK ========
 */
int_fast16_t HSMXXF3_provisionHUK(void)
{
    int_fast16_t status = HSMXXF3_STATUS_ERROR;
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    uint32_t token[3];

    /* Try and obtain access to the crypto module */
    if (!HSMXXF3_acquireLock(SemaphoreP_NO_WAIT, (uintptr_t)0U))
    {
        /* Acquiring the lock failed so we return immediately */
        return HSMXXF3_STATUS_RESOURCE_UNAVAILABLE;
    }

    /* Set the token for HUK provisioning */
    token[0] = HUK_PROVISION_TOKEN_WORD0;
    token[1] = CRYPTO_OFFICER_ID;
    token[2] = HUK_PROVISION_TOKEN_WORD2;

    if (HSMXXF3_nrbgMode == HSMXXF3_MODE_CRNG)
    {
        /* For CRNG mode, Bit 4 has to be high */
        token[2] |= RNG_CONFIG_TOKEN_CRNG;
    }
    else
    {
        /* When request is TRNG, do nothing. */
    }

    /* Enable the OTP interrupt event */
    HWREG(HSM_BASE + HSM_O_CTL) |= HSM_CTL_OTPEVTEN_EN;

    /* Write the token to the HSM */
    HSMXXF3_writeToken(&token[0], sizeof(token) / sizeof(uint32_t));

    /* The HSM has processed the token and returned a token back */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL)
    {
        HWREG(HSM_BASE + HSM_O_CTL) |= HSM_CTL_OTPEVTCLR_CLR;
    }

    /* Check the result to see if we have an error or if the HUK was provisioned already */
    if (((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & HSMXXF3_RETVAL_MASK) == 0) ||
         ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & HSMXXF3_RETVAL_MASK) == HSM_HUK_ALREADY_PROVISIONED))
    {
        status = HSMXXF3_STATUS_SUCCESS;
    }
    else
    {
        /* Do nothing */
    }

    /* Clear the mailbox */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

    /* Disable the OTP interrupt event */
    HWREG(HSM_BASE + HSM_O_CTL) &= ~HSM_CTL_OTPEVTEN_EN;

    /* Release the access semaphore */
    HSMXXF3_releaseLock();

    /* Reset after OTP writes */
    if (HSMXXF3_STATUS_SUCCESS == status)
    {
        status = HSMXXF3_submitResetToken();
    }
#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    /* HUK is provisioned on CC35XX at boot time by TI Device
     * boot loader. As a result, HUK provisioning is not needed.
     * Returning sucess here.
     */
    status = HSMXXF3_STATUS_SUCCESS;
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

    return status;
}

/*
 *  ======== HSMXXF3_acquireLock ========
 */
bool HSMXXF3_acquireLock(uint32_t timeout, uintptr_t driverHandle)
{
    bool status = false;
    SemaphoreP_Status resourceAcquired;

    /* Try and obtain access to the crypto module */
    resourceAcquired = SemaphoreP_pend(&HSMXXF3_accessSemaphore, timeout);

    if (resourceAcquired == SemaphoreP_OK)
    {
        operation.driverHandle = driverHandle;

        (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
        (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

        if (HSMXXF3_isHSMInSleepMode)
        {
            HSMXXF3_wakeUp();
        }

        Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

        status = true;
    }

    return status;
}

/*
 *  ======== HSMXXF3_releaseLock ========
 */
void HSMXXF3_releaseLock(void)
{
    operation.driverHandle = 0U;

    SemaphoreP_post(&HSMXXF3_accessSemaphore);

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
}

/*
 *  ======== HSMXXF3_submitToken ========
 */
int_fast16_t HSMXXF3_submitToken(HSMXXF3_ReturnBehavior retBehavior,
                                 HSMXXF3_CallbackFxn callbackFxn,
                                 uintptr_t driverHandle)
{
    int_fast16_t result;
    HSMSALStatus_t status;
    uintptr_t key;

    if (HSMXXF3_isHSMInSleepMode)
    {
        return HSMXXF3_STATUS_IN_SLEEP_MODE;
    }

    /* Verify that the caller is the driver with the HSMXXF3_accessSemaphore */
    if (driverHandle == operation.driverHandle)
    {
        /* Populate global operation struct */
        operation.returnBehavior = retBehavior;
        operation.callbackFxn    = callbackFxn;

        /* Enable interrupt in blocking and callback modes */
        if (operation.returnBehavior != HSMXXF3_RETURN_BEHAVIOR_POLLING)
        {
            HwiP_clearInterrupt(INT_HSM_SEC_IRQ);
            HwiP_enableInterrupt(INT_HSM_SEC_IRQ);
        }
        else
        {
            HwiP_clearInterrupt(INT_HSM_SEC_IRQ);
            HwiP_disableInterrupt(INT_HSM_SEC_IRQ);
        }

        /* Starting the operation and setting HSMXXF3_operationInProgress must be atomic */
        key = HwiP_disable();

        /* Submit token */
        status = HSMSAL_SubmitPhysicalToken(&operation.commandToken);

        if (status == HSMSAL_SUCCESS)
        {
            HSMXXF3_operationInProgress = true;

            HwiP_restore(key);

            result = HSMXXF3_STATUS_SUCCESS;
        }
        else
        {
            HwiP_restore(key);

            result = HSMXXF3_STATUS_ERROR;

            HwiP_clearInterrupt(INT_HSM_SEC_IRQ);
            HwiP_disableInterrupt(INT_HSM_SEC_IRQ);
        }
    }
    else
    {
        result = HSMXXF3_STATUS_ERROR;
    }

    return result;
}

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== HSMXXF3_submitResetToken ========
 */
static int_fast16_t HSMXXF3_submitResetToken(void)
{
    int_fast16_t status = HSMXXF3_STATUS_ERROR;

    uint32_t token[1] = {0};

    /* Try and obtain access to the crypto module */
    if (!HSMXXF3_acquireLock(SemaphoreP_NO_WAIT, (uintptr_t)0U))
    {
        /* Acquiring the lock failed so we return immediately */
        return HSMXXF3_STATUS_RESOURCE_UNAVAILABLE;
    }

    /* Set the token for reset */
    token[0] = (EIP130TOKEN_OPCODE_SYSTEM | EIP130TOKEN_SUBCODE_RESET);

    /* Write the token to the HSM */
    HSMXXF3_writeToken(&token[0], sizeof(token) / sizeof(uint32_t));

    /* The HSM has processed the token and returned a token back */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL)
    {
        HWREG(HSM_BASE + HSM_O_CTL) |= HSM_CTL_OTPEVTCLR_CLR;
    }

    /* Check the result to see if anything went wrong */
    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & HSMXXF3_RETVAL_MASK) == 0)
    {
        status = HSMXXF3_STATUS_SUCCESS;
    }
    else
    {
        /* Do nothing */
    }

    /* Clear the mailbox */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

    /* Release the access semaphore */
    HSMXXF3_releaseLock();

    return status;
}
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

/*
 *  ======== HSMXXF3_waitForResult ========
 */
int_fast16_t HSMXXF3_waitForResult(void)
{
    int_fast16_t result = HSMXXF3_STATUS_SUCCESS;
    HSMSALStatus_t hsmsalStatus;

    switch (operation.returnBehavior)
    {
        case HSMXXF3_RETURN_BEHAVIOR_POLLING:
            hsmsalStatus                = HSMSAL_WaitForResultPolling(&operation.resultToken);
            /* The above function will either return a timeout, or successfully
             * populate the result
             */
            HSMXXF3_operationInProgress = false;

            if (hsmsalStatus == HSMSAL_RESPONSE_TIMEOUT)
            {
                result = HSMXXF3_STATUS_TIMEOUT;
            }
            break;
        case HSMXXF3_RETURN_BEHAVIOR_BLOCKING:
            SemaphoreP_pend(&HSMXXF3_operationSemaphore, SemaphoreP_WAIT_FOREVER);
            break;
        case HSMXXF3_RETURN_BEHAVIOR_CALLBACK:
            /* No action necessary */
            break;
        default:
            result = HSMXXF3_STATUS_ERROR;
    }

    if ((result == HSMXXF3_STATUS_SUCCESS) && ((operation.returnBehavior == HSMXXF3_RETURN_BEHAVIOR_POLLING) ||
                                               (operation.returnBehavior == HSMXXF3_RETURN_BEHAVIOR_BLOCKING)))
    {
        if (operation.callbackFxn != NULL)
        {
            (void)operation.callbackFxn(operation.driverHandle);
        }
    }

    return result;
}

/*
 *  ======== HSMXXF3_cancelOperation ========
 */
int_fast16_t HSMXXF3_cancelOperation(void)
{
    int_fast16_t result = HSMXXF3_STATUS_SUCCESS;
    HSMSALStatus_t hsmsalStatus;
    uintptr_t key;

    key = HwiP_disable();

    if (HSMXXF3_operationInProgress)
    {
        (void)HwiP_clearInterrupt(INT_HSM_SEC_IRQ);
        (void)HwiP_disableInterrupt(INT_HSM_SEC_IRQ);

        HwiP_restore(key);

        /* Since the HSM cannot cancel an in-progress token, we must wait for the result to allow for subsequent token
         * submissions to succeed.
         */
        hsmsalStatus = HSMSAL_WaitForResultPolling(&operation.resultToken);

        if (hsmsalStatus == HSMSAL_RESPONSE_TIMEOUT)
        {
            result = HSMXXF3_STATUS_TIMEOUT;
        }

        HSMXXF3_operationInProgress = false;

        /* The post-processing function typically releases the lock and power constraint,
         * but the cancel operation is now responsible for it.
         */
        HSMXXF3_releaseLock();
    }
    else
    {
        HwiP_restore(key);
    }

    return result;
}

/*
 *  ======== HSMXXF3_isOperationInProgress ========
 */
bool HSMXXF3_isOperationInProgress(void)
{
    return HSMXXF3_operationInProgress;
}

/*
 *  ======== HSMXXF3_getCurrentNRBGMode ========
 */
HSMXXF3_NRBGMode HSMXXF3_getCurrentNRBGMode(void)
{
    return HSMXXF3_nrbgMode;
}

/*
 *  ======== HSMXXF3_updateInternalNRBGMode ========
 */
void HSMXXF3_updateInternalNRBGMode(void)
{
    if (HSMXXF3_nrbgMode == HSMXXF3_MODE_CRNG)
    {
        HSMXXF3_nrbgMode = HSMXXF3_MODE_TRNG;
    }
    else
    {
        HSMXXF3_nrbgMode = HSMXXF3_MODE_CRNG;
    }
}

/*
 *  ================ APIs to handle result token data ================
 */

/*
 *  ======== HSMXXF3_getResultCode ========
 */
int32_t HSMXXF3_getResultCode(void)
{
    return Eip130Token_Result_Code(&operation.resultToken);
}

/*
 *  ======== HSMXXF3_getResultAssetID ========
 */
uint32_t HSMXXF3_getResultAssetID(void)
{
    return operation.resultToken.W[1];
}

/*
 *  ======== HSMXXF3_getResultDigest ========
 */
void HSMXXF3_getResultDigest(uint8_t *digest, size_t digestLength)
{
    Eip130Token_Result_Hash_CopyState(&operation.resultToken, digestLength, digest);
}

/*
 *  ======== HSMXXF3_getAESEncryptTag ========
 */
void HSMXXF3_getAESEncryptTag(void *mac, size_t macLength)
{
    (void)memcpy(mac, &operation.resultToken.W[6], macLength);
}

/*
 *  ======== HSMXXF3_getAESIV ========
 */
void HSMXXF3_getAESIV(uint8_t *iv)
{
    Eip130Token_Result_Crypto_CopyIV(&operation.resultToken, iv);
}

/*
 *  ======== HSMXXF3_getAESCMACSignMac ========
 */
void HSMXXF3_getAESCMACSignMac(uint8_t *mac, uint8_t macLength)
{
    Eip130Token_Result_Mac_CopyFinalMAC(&operation.resultToken, macLength, mac);
}

/*
 *  ======== HSMXXF3_getPublicDataRead ========
 */
void HSMXXF3_getPublicDataRead(uint32_t assetId, const uint8_t *data, uint8_t dataLength)
{
    Eip130Token_Command_PublicData_Read(&operation.commandToken, assetId, (uintptr_t)data, dataLength);
}

/*
 *  ================ APIs to construct key/asset management-related command tokens ================
 */

/*
 *  ======== HSMXXF3_constructCreateAssetToken ========
 */
void HSMXXF3_constructCreateAssetToken(uint64_t assetPolicy, uint32_t assetLength)
{
    Eip130Token_Command_AssetCreate(&operation.commandToken, assetPolicy, assetLength);
}

/*
 *  ======== HSMXXF3_constructLoadPlaintextAssetToken ========
 */
void HSMXXF3_constructLoadPlaintextAssetToken(const uint8_t *input_p, const uint32_t inputLength, uint32_t assetId)
{
    Eip130Token_Command_AssetLoad_Plaintext(&operation.commandToken, assetId);

    Eip130Token_Command_AssetLoad_SetInput(&operation.commandToken, (uintptr_t)input_p, inputLength);
}

/*
 *  ======== HSMXXF3_constructDeleteAssetToken ========
 */
void HSMXXF3_constructDeleteAssetToken(uint32_t assetId)
{
    Eip130Token_Command_AssetDelete(&operation.commandToken, (Eip130TokenAssetId_t)assetId);
}

/*
 *  ================ APIs to construct driver-specific command tokens ================
 */

/*
 *  ======== HSMXXF3_constructSHA2PhysicalToken ========
 */
void HSMXXF3_constructSHA2PhysicalToken(SHA2XXF3HSM_Object *object)
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
 *  ======== HSMXXF3_constructECDSASignPhysicalToken ========
 */
void HSMXXF3_constructECDSASignPhysicalToken(ECDSAXXF3HSM_Object *object)
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
 *  ======== HSMXXF3_constructECDHGenPubPhysicalToken ========
 */
void HSMXXF3_constructECDHGenPubPhysicalToken(ECDHXXF3HSM_Object *object)
{
    const uint8_t nWord            = HSM_ASYM_DATA_SIZE_B2W(object->curveLength);
    Eip130TokenDmaAddress_t output = (uintptr_t)object->output;
    uint8_t outputSize             = HSM_ASYM_DATA_SIZE_VWB(object->curveLength);
    uint8_t command                = VEXTOKEN_PKAS_ECDH_ECDSA_GEN_PUBKEY;

    if (object->curveType == ECDH_TYPE_CURVE_25519)
    {
        command = VEXTOKEN_PKAS_CURVE25519_GEN_PUBKEY;
    }
    else
    {
        outputSize = HSM_SIGNATURE_VCOUNT * outputSize;
    }

    Eip130Token_Command_Pk_Asset_Command(&operation.commandToken,
                                         command,
                                         nWord,
                                         nWord,
                                         0,
                                         object->privateKeyAssetID,
                                         object->paramAssetID,
                                         object->publicKeyAssetID,
                                         0,
                                         0,
                                         output,
                                         outputSize);
}

/*
 *  ======== HSMXXF3_constructECDHVerifyKeysPhysicalToken ========
 */
void HSMXXF3_constructECDHVerifyKeysPhysicalToken(ECDHXXF3HSM_Object *object)
{
    uint32_t privateKeyAssetID = object->privateKeyAssetID;
    uint32_t publicKeyAssetID  = 0U;

    if (object->operationType == ECDH_OPERATION_TYPE_COMPUTE_SHARED_SECRET)
    {
        privateKeyAssetID = 0;
        publicKeyAssetID  = object->publicKeyAssetID;
    }

    Eip130Token_Command_Pk_Asset_Command(&operation.commandToken,
                                         VEXTOKEN_PKAS_ECDH_ECDSA_KEYCHK,
                                         HSM_ASYM_DATA_SIZE_B2W(object->curveLength),
                                         HSM_ASYM_DATA_SIZE_B2W(object->curveLength),
                                         0,
                                         publicKeyAssetID,
                                         object->paramAssetID,
                                         privateKeyAssetID,
                                         0,
                                         0,
                                         0,
                                         0);
}

/*
 *  ======== HSMXXF3_constructECDHGenShrdSecPhysicalToken ========
 */
void HSMXXF3_constructECDHGenShrdSecPhysicalToken(ECDHXXF3HSM_Object *object)
{
    const uint8_t nWord = HSM_ASYM_DATA_SIZE_B2W(object->curveLength);
    uint8_t command     = VEXTOKEN_PKAS_ECDH_GEN_SKEYPAIR_SHARED_SECRET;

    if (object->curveType == ECDH_TYPE_CURVE_25519)
    {
        command = VEXTOKEN_PKAS_CURVE25519_GEN_SHARED_SECRET;
    }

    Eip130Token_Command_Pk_Asset_Command(&operation.commandToken,
                                         command,
                                         nWord,
                                         nWord,
                                         1U,
                                         object->privateKeyAssetID,
                                         object->paramAssetID,
                                         object->publicKeyAssetID,
                                         0,
                                         0,
                                         0,
                                         0);

    Eip130Token_Command_Pk_Asset_SaveSharedSecret(&operation.commandToken);

    Eip130Token_Command_Pk_Asset_SetAdditionalAssetId(&operation.commandToken, object->publicDataAssetID);
}
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== HSMXXF3_constructEDDSAGenPubKeyPhysicalToken ========
 */
void HSMXXF3_constructEDDSAGenPubKeyPhysicalToken(EDDSAXXF3HSM_Object *object)
{
    (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
    (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

    Eip130Token_Command_Pk_Asset_Command(&operation.commandToken,
                                         VEXTOKEN_PKAS_EDDSA_GEN_PUBKEY,
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         0,
                                         object->privateKeyAssetID,
                                         object->paramAssetID,
                                         object->publicKeyAssetID,
                                         0,
                                         0,
                                         (uintptr_t)object->signature,
                                         HSM_ASYM_DATA_SIZE_VWB(EDDSA_CURVE_LENGTH_25519));
}

/*
 *  ======== HSMXXF3_constructEDDSASignInitialPhysicalToken ========
 */
void HSMXXF3_constructEDDSASignInitialPhysicalToken(EDDSAXXF3HSM_Object *object)
{
    (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
    (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

    Eip130Token_Command_Pk_Asset_Command(&operation.commandToken,
                                         VEXTOKEN_PKAS_EDDSA_SIGN_INITIAL,
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         0,
                                         object->privateKeyAssetID,
                                         object->paramAssetID,
                                         object->tempAssetID,
                                         (uintptr_t)object->input,
                                         object->inputLength,
                                         0U,
                                         0U);
}

/*
 *  ======== HSMXXF3_constructEDDSAIntermediateHashPhysicalToken ========
 */
void HSMXXF3_constructEDDSAIntermediateHashPhysicalToken(const uint8_t *input, size_t inputLength, uint32_t tempAssetID)
{
    (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
    (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

    Eip130Token_Command_Hash(&operation.commandToken,
                             VEXTOKEN_ALGO_HASH_SHA512,
                             false,
                             false,
                             (uintptr_t)input,
                             inputLength);

    Eip130Token_Command_Hash_SetStateASID(&operation.commandToken, tempAssetID);
}

/*
 *  ======== HSMXXF3_constructEDDSASignUpdatePhysicalToken ========
 */
void HSMXXF3_constructEDDSASignUpdatePhysicalToken(EDDSAXXF3HSM_Object *object)
{
    EDDSA_OperationSign *signOperation = (EDDSA_OperationSign *)object->operation;

    (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
    (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

    Eip130Token_Command_Pk_Asset_Command(&operation.commandToken,
                                         VEXTOKEN_PKAS_EDDSA_SIGN_UPDATE,
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         0U,
                                         object->publicKeyAssetID,
                                         0U,
                                         object->stateAssetID,
                                         (uintptr_t)object->input,
                                         object->inputLength,
                                         0U,
                                         0U);

    Eip130Token_Command_Pk_Asset_SetAdditionalLength(&operation.commandToken, signOperation->preHashedMessageLength);
}

/*
 *  ======== HSMXXF3_constructEDDSASignFinalizePhysicalToken ========
 */
void HSMXXF3_constructEDDSASignFinalizePhysicalToken(EDDSAXXF3HSM_Object *object)
{
    EDDSA_OperationSign *signOperation = (EDDSA_OperationSign *)object->operation;

    (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
    (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

    Eip130Token_Command_Pk_Asset_Command(&operation.commandToken,
                                         VEXTOKEN_PKAS_EDDSA_SIGN_FINAL,
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         0U,
                                         0U,
                                         0U,
                                         object->stateAssetID,
                                         (uintptr_t)object->input,
                                         object->inputLength,
                                         (uintptr_t)object->signature,
                                         EDDSA_COMPONENT_VECTOR_LENGTH_BYTES);

    Eip130Token_Command_Pk_Asset_SetAdditionalLength(&operation.commandToken, signOperation->preHashedMessageLength);
}

/*
 *  ======== HSMXXF3_constructEDDSAVerifyInitialPhysicalToken ========
 */
void HSMXXF3_constructEDDSAVerifyInitialPhysicalToken(EDDSAXXF3HSM_Object *object)
{
    (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
    (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

    Eip130Token_Command_Pk_Asset_Command(&operation.commandToken,
                                         VEXTOKEN_PKAS_EDDSA_VERIFY_INITIAL,
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         0U,
                                         object->publicKeyAssetID,
                                         object->paramAssetID,
                                         object->tempAssetID,
                                         (uintptr_t)object->input,
                                         object->inputLength,
                                         (uintptr_t)object->signature,
                                         EDDSA_COMPONENT_VECTOR_LENGTH_BYTES);
}

/*
 *  ======== HSMXXF3_constructEDDSAVerifyFinalizePhysicalToken ========
 */
void HSMXXF3_constructEDDSAVerifyFinalizePhysicalToken(EDDSAXXF3HSM_Object *object)
{
    EDDSA_OperationVerify *verifyOperation = (EDDSA_OperationVerify *)object->operation;

    (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
    (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

    Eip130Token_Command_Pk_Asset_Command(&operation.commandToken,
                                         VEXTOKEN_PKAS_EDDSA_VERIFY_FINAL,
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         HSM_ASYM_DATA_SIZE_B2W(EDDSA_CURVE_LENGTH_25519),
                                         0U,
                                         0U,
                                         0U,
                                         object->stateAssetID,
                                         (uintptr_t)object->input,
                                         object->inputLength,
                                         0U,
                                         0U);

    Eip130Token_Command_Pk_Asset_SetAdditionalLength(&operation.commandToken, verifyOperation->preHashedMessageLength);
}
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

#if (ENABLE_KEY_STORAGE == 1)
/*
 *  ======== HSMXXF3_constructAESKey ========
 */
static void HSMXXF3_constructAESKey(uint8_t *key,
                                    uint32_t keyLength,
                                    uint32_t keyAssetID,
                                    KeyStore_PSA_KeyLocation location)
{
    /* If the key's location is PSA KeyStore, then the key material is guaranteed to be in plaintext at the provided
     * pointer. The AES driver has already retrieved it from KeyStore and placed it in a local buffer.
     */
    if (location == KEYSTORE_PSA_KEY_LOCATION_LOCAL_STORAGE)
    {
        (void)memcpy(&operation.commandToken.W[17], key, keyLength);
    }
    else
    {
        (void)key;
        (void)keyLength;
        /* The key's location is HSM Asset Store, so the key material must be provided via the asset, which the AES
         * driver has already loaded and retrieved an Asset ID for.
         */
        (void)memcpy(&operation.commandToken.W[17], &keyAssetID, 4);

        /* Word 11 bit 8 must be set high to indicate that the key is loaded from an asset. */
        operation.commandToken.W[11] |= (1 << 8);
    }
}
#endif

/*
 *  ======== HSMXXF3_constructGCMToken ========
 */
void HSMXXF3_constructGCMToken(const AESGCMXXF3HSM_Object *object, bool saveIV, bool loadIV)
{
    size_t inputLength     = object->inputLength;
    uint32_t keyLengthCode = 0U;
    uint32_t keyLength     = 0U;

    (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
    (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

    if ((inputLength > 0U) && (inputLength < AES_BLOCK_SIZE))
    {
        inputLength = AES_BLOCK_SIZE;
    }

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyLength = object->common.key.u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE_HSM)
    {
        /* If we have reached this point, then these are the only two key encodings possible */
        keyLength = object->common.key.u.keyStore.keyLength;
    }
#endif

    operation.commandToken.W[0] = HSM_ENCRYPTION_TOKEN_WORD0;
    operation.commandToken.W[2] = object->totalDataLength;
    operation.commandToken.W[3] = (uintptr_t)object->input;
    operation.commandToken.W[5] = inputLength;
    operation.commandToken.W[6] = (uintptr_t)object->output;
    operation.commandToken.W[8] = inputLength;
    operation.commandToken.W[9] = (uintptr_t)object->aad;

    operation.commandToken.W[11] = (HSM_ENCRYPTION_TOKEN_WORD11) | (HSM_ENCRYPTION_TOKEN_WORD11_GCM) | (loadIV << 9) |
                                   (saveIV << 12);

    if (saveIV)
    {
        operation.commandToken.W[12] = object->tempAssetID;
    }

    if (loadIV)
    {
        operation.commandToken.W[13] = object->tempAssetID;
    }
    else
    {
        /* In the case of a CCM operation, nonce is copied to the appropriate word within the command token. */
        (void)memcpy((void *)&operation.commandToken.W[13], (void *)object->iv, object->ivLength);
    }

    if ((object->operationType == AESGCM_OP_TYPE_ONESTEP_ENCRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_AAD_ENCRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_DATA_ENCRYPT) ||
        (object->operationType == AESGCM_OP_TYPE_FINALIZE_ENCRYPT))
    {
        operation.commandToken.W[11] |= HSM_ENCRYPTION_TOKEN_WORD11_ENC;
    }
    else if ((object->operationType == AESGCM_OP_TYPE_ONESTEP_DECRYPT) ||
             (object->operationType == AESGCM_OP_TYPE_AAD_DECRYPT) ||
             (object->operationType == AESGCM_OP_TYPE_DATA_DECRYPT) ||
             (object->operationType == AESGCM_OP_TYPE_FINALIZE_DECRYPT))
    {
        /* Decryption operation. Copy the Tag to the input token */
        (void)memcpy((void *)&operation.commandToken.W[33], (void *)object->mac, object->macLength);
    }
    else
    {
        /* Do nothing. */
    }

    switch (keyLength)
    {
        case HSM_AES_128_KEY_LENGTH:
            keyLengthCode = HSM_ENCRYPTION_TOKEN_WORD11_KEY128;
            break;

        case HSM_AES_192_KEY_LENGTH:
            keyLengthCode = HSM_ENCRYPTION_TOKEN_WORD11_KEY192;
            break;

        case HSM_AES_256_KEY_LENGTH:
            keyLengthCode = HSM_ENCRYPTION_TOKEN_WORD11_KEY256;
            break;

        default:
            keyLengthCode = 0U;
            break;
    }

    operation.commandToken.W[11] |= keyLengthCode;

    operation.commandToken.W[11] |= (((uint32_t)object->macLength & MASK_5_BITS) << 24);

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        /* In this case, the key material is guaranteed to be in plaintext at the provided pointer */
        (void)memcpy((void *)&operation.commandToken.W[17],
                     (void *)object->common.key.u.plaintext.keyMaterial,
                     keyLength);
    }
#if (ENABLE_KEY_STORAGE == 1)
    else
    {
        HSMXXF3_constructAESKey((uint8_t *)&object->KeyStore_keyingMaterial[0],
                                keyLength,
                                object->keyAssetID,
                                object->keyLocation);
    }
#endif

    operation.commandToken.W[16] = HSM_ENCRYPTION_TOKEN_WORD16_GCM_IV;

    operation.commandToken.W[25] = object->totalAADLength;

    if (object->tempAssetID != 0U)
    {
        operation.commandToken.W[26] = object->aadLength;
    }
}

/*
 *  ======== HSMXXF3_constructCCMToken ========
 */
void HSMXXF3_constructCCMToken(const AESCCMXXF3_Object *object, bool saveIV, bool loadIV)
{
    size_t inputLength     = object->inputLength;
    uint32_t keyLengthCode = 0U;
    uint32_t keyLength     = 0U;

    (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
    (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

    if ((inputLength > 0U) && (inputLength < AES_BLOCK_SIZE))
    {
        inputLength = AES_BLOCK_SIZE;
    }

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyLength = object->common.key.u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE_HSM)
    {
        /* If we have reached this point, then these are the only two key encodings possible */
        keyLength = object->common.key.u.keyStore.keyLength;
    }
#endif

    operation.commandToken.W[0] = HSM_ENCRYPTION_TOKEN_WORD0;
    operation.commandToken.W[2] = object->totalDataLength;
    operation.commandToken.W[3] = (uintptr_t)object->input;
    operation.commandToken.W[5] = inputLength;
    operation.commandToken.W[6] = (uintptr_t)object->output;
    operation.commandToken.W[8] = inputLength;
    operation.commandToken.W[9] = (uintptr_t)object->aad;

    operation.commandToken.W[11] = (HSM_ENCRYPTION_TOKEN_WORD11) | (HSM_ENCRYPTION_TOKEN_WORD11_CCM) | (loadIV << 9) |
                                   (saveIV << 12);

    if (saveIV)
    {
        operation.commandToken.W[12] = object->tempAssetID;
    }

    if (loadIV)
    {
        operation.commandToken.W[13] = object->tempAssetID;
    }

    if ((object->operationType == AESCCM_OP_TYPE_ONESTEP_ENCRYPT) ||
        (object->operationType == AESCCM_OP_TYPE_AAD_ENCRYPT) ||
        (object->operationType == AESCCM_OP_TYPE_DATA_ENCRYPT) ||
        (object->operationType == AESCCM_OP_TYPE_FINALIZE_ENCRYPT))
    {
        operation.commandToken.W[11] |= HSM_ENCRYPTION_TOKEN_WORD11_ENC;
    }
    else if ((object->operationType == AESCCM_OP_TYPE_ONESTEP_DECRYPT) ||
             (object->operationType == AESCCM_OP_TYPE_AAD_DECRYPT) ||
             (object->operationType == AESCCM_OP_TYPE_DATA_DECRYPT) ||
             (object->operationType == AESCCM_OP_TYPE_FINALIZE_DECRYPT))
    {
        /* Decryption operation. Copy the Tag to the input token */
        (void)memcpy((void *)&operation.commandToken.W[33], (void *)object->mac, object->macLength);
    }
    else
    {
        /* Do nothing. */
    }

    switch (keyLength)
    {
        case (128 / 8):
            keyLengthCode = HSM_ENCRYPTION_TOKEN_WORD11_KEY128;
            break;

        case (192 / 8):
            keyLengthCode = HSM_ENCRYPTION_TOKEN_WORD11_KEY192;
            break;

        case (256 / 8):
            keyLengthCode = HSM_ENCRYPTION_TOKEN_WORD11_KEY256;
            break;

        default:
            keyLengthCode = 0U;
            break;
    }

    operation.commandToken.W[11] |= keyLengthCode;

    operation.commandToken.W[11] |= (((uint32_t)object->nonceLength & MASK_4_BITS) << 20);

    operation.commandToken.W[11] |= (((uint32_t)object->macLength & MASK_5_BITS) << 24);

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        /* In this case, the key material is guaranteed to be in plaintext at the provided pointer */
        (void)memcpy((void *)&operation.commandToken.W[17],
                     (void *)object->common.key.u.plaintext.keyMaterial,
                     keyLength);
    }
#if (ENABLE_KEY_STORAGE == 1)
    else
    {
        HSMXXF3_constructAESKey((uint8_t *)&object->KeyStore_keyingMaterial[0],
                                keyLength,
                                object->keyAssetID,
                                object->keyLocation);
    }
#endif

    if (object->nonceLength != 0U)
    {
        /* In the case of a CCM operation, nonce is copied to the appropriate word within the command token. */
        (void)memcpy((void *)&operation.commandToken.W[29], (void *)object->nonce, object->nonceLength);
    }

    operation.commandToken.W[25] = object->totalAADLength;

    if (object->tempAssetID != 0U)
    {
        operation.commandToken.W[26] = object->aadLength;
    }
}

/*
 *  ======== HSMXXF3_constructCommonAESECB ========
 */
static void HSMXXF3_constructCommonAESECB(AESECBXXF3_Object *object, uint8_t *key)
{
    bool isEncrypt     = true;
    /* Drivers should verify the key encoding before HSMXXF3 token construction begins,
     * so this variable should be populated with an actual value in all cases.
     */
    uint32_t keyLength = 0U;
    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyLength = object->common.key.u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE_HSM)
    {
        /* If we have reached this point, then these are the only two key encodings possible */
        keyLength = object->common.key.u.keyStore.keyLength;
    }
#endif

    if ((object->operationType == AESECB_OPERATION_TYPE_DECRYPT) ||
        (object->operationType == AESECB_OPERATION_TYPE_DECRYPT_SEGMENTED) ||
        (object->operationType == AESECB_OPERATION_TYPE_FINALIZE_DECRYPT_SEGMENTED))
    {
        isEncrypt = false;
    }

    Eip130Token_Command_Crypto_Operation(&operation.commandToken,
                                         (uint8_t)EIP130TOKEN_CRYPTO_ALGO_AES,
                                         (uint8_t)VEXTOKEN_MODE_CIPHER_ECB,
                                         isEncrypt,
                                         object->operation->inputLength);

    Eip130Token_Command_Crypto_SetKeyLength(&operation.commandToken, keyLength);

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        /* In this case, the key material is guaranteed to be in plaintext at the provided pointer */
        Eip130Token_Command_Crypto_CopyKey(&operation.commandToken, key, keyLength);
    }
#if (ENABLE_KEY_STORAGE == 1)
    else
    {
        HSMXXF3_constructAESKey(key, keyLength, object->keyAssetID, object->keyLocation);
    }
#endif
}

/*
 *  ======== HSMXXF3_constructAESECBOneStepPhysicalToken ========
 */
void HSMXXF3_constructAESECBOneStepPhysicalToken(AESECBXXF3_Object *object, uint8_t *key)
{
    Eip130TokenDmaAddress_t input  = (uintptr_t)object->operation->input;
    Eip130TokenDmaAddress_t output = (uintptr_t)object->operation->output;

    uint32_t inputDataLength  = object->operation->inputLength;
    uint32_t outputDataLength = object->operation->inputLength;

    outputDataLength = HSMXXF3_getOutputBufferLength(inputDataLength);

    HSMXXF3_constructCommonAESECB(object, key);

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken,
                                                input,
                                                inputDataLength,
                                                output,
                                                outputDataLength);
}

/*
 *  ======== HSMXXF3_constructCommonAESCTR ========
 */
static void HSMXXF3_constructCommonAESCTR(AESCTRXXF3_Object *object, uint8_t *key)
{
    uint32_t srcDataLength = object->inputLength;
    bool isEncrypt         = AESCTR_OPERATION_TYPE_ENCRYPT;
    /* Drivers should verify the key encoding before HSMXXF3 token construction begins,
     * so this variable should be populated with an actual value in all cases.
     */
    uint32_t keyLength     = 0U;
    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyLength = object->common.key.u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE_HSM)
    {
        /* If we have reached this point, then these are the only two key encodings possible */
        keyLength = object->common.key.u.keyStore.keyLength;
    }
#endif

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

    Eip130Token_Command_Crypto_SetKeyLength(&operation.commandToken, keyLength);

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        /* In this case, the key material is guaranteed to be in plaintext at the provided pointer */
        Eip130Token_Command_Crypto_CopyKey(&operation.commandToken, key, keyLength);
    }
#if (ENABLE_KEY_STORAGE == 1)
    else
    {
        HSMXXF3_constructAESKey(key, keyLength, object->keyAssetID, object->keyLocation);
    }
#endif

    Eip130Token_Command_Crypto_CopyIV(&operation.commandToken, (uint8_t *)&object->counter[0]);
}

/*
 *  ======== HSMXXF3_constructAESCTROneStepPhysicalToken ========
 */
void HSMXXF3_constructAESCTROneStepPhysicalToken(AESCTRXXF3_Object *object, uint8_t *key)
{

    uint32_t inputDataLength  = object->inputLength;
    uint32_t outputDataLength = object->inputLength;

    inputDataLength += AES_BLOCK_SIZE_ALIGN;
    inputDataLength &= (uint32_t)(~AES_BLOCK_SIZE_ALIGN);

    outputDataLength = HSMXXF3_getOutputBufferLength(inputDataLength);

    HSMXXF3_constructCommonAESCTR(object, key);

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken,
                                                (uintptr_t)object->input,
                                                inputDataLength,
                                                (uintptr_t)object->output,
                                                outputDataLength);
}

/*
 *  ======== HSMXXF3_constructCommonAESCBC ========
 */
static void HSMXXF3_constructCommonAESCBC(AESCBCXXF3_Object *object, uint8_t *key)
{
    uint32_t srcDataLength = object->inputLength;
    bool isEncrypt         = true;
    /* Drivers should verify the key encoding before HSMXXF3 token construction begins,
     * so this variable should be populated with an actual value in all cases.
     */
    uint32_t keyLength     = 0U;
    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        keyLength = object->common.key.u.plaintext.keyLength;
    }
#if (ENABLE_KEY_STORAGE == 1)
    else if (object->common.key.encoding == CryptoKey_KEYSTORE_HSM)
    {
        /* If we have reached this point, then these are the only two key encodings possible */
        keyLength = object->common.key.u.keyStore.keyLength;
    }
#endif

    if ((object->operationType == AESCBC_OP_TYPE_ONESTEP_DECRYPT) ||
        (object->operationType == AESCBC_OP_TYPE_DECRYPT_SEGMENTED) ||
        (object->operationType == AESCBC_OP_TYPE_FINALIZE_DECRYPT_SEGMENTED))
    {
        isEncrypt = false;
    }

    Eip130Token_Command_Crypto_Operation(&operation.commandToken,
                                         (uint8_t)EIP130TOKEN_CRYPTO_ALGO_AES,
                                         (uint8_t)VEXTOKEN_MODE_CIPHER_CBC,
                                         isEncrypt,
                                         srcDataLength);

    Eip130Token_Command_Crypto_SetKeyLength(&operation.commandToken, keyLength);

    if (object->common.key.encoding == CryptoKey_PLAINTEXT_HSM)
    {
        /* In this case, the key material is guaranteed to be in plaintext at the provided pointer */
        Eip130Token_Command_Crypto_CopyKey(&operation.commandToken, key, keyLength);
    }
#if (ENABLE_KEY_STORAGE == 1)
    else
    {
        HSMXXF3_constructAESKey(key, keyLength, object->keyAssetID, object->keyLocation);
    }
#endif

    Eip130Token_Command_Crypto_CopyIV(&operation.commandToken, (uint8_t *)&object->iv[0]);
}

/*
 *  ======== HSMXXF3_constructAESCBCOneStepPhysicalToken ========
 */
void HSMXXF3_constructAESCBCOneStepPhysicalToken(AESCBCXXF3_Object *object, uint8_t *key)
{
    uint32_t inputDataLength  = object->inputLength;
    uint32_t outputDataLength = object->inputLength;

    outputDataLength = HSMXXF3_getOutputBufferLength(inputDataLength);

    HSMXXF3_constructCommonAESCBC(object, key);

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken,
                                                (uintptr_t)object->input,
                                                inputDataLength,
                                                (uintptr_t)object->output,
                                                outputDataLength);
}

/*
 *  ======== HSMXXF3_constructCMACToken ========
 */
void HSMXXF3_constructCMACToken(AESCMACXXF3_Object *object, bool isFirst, bool isFinal)
{
    uint32_t operationAlgo = HSM_MAC_TOKEN_WORD6_CMAC;
    uint32_t inputLength   = (uint32_t)object->inputLength;
    uint32_t padbytes      = 0U;

    (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
    (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

    if (object->operationalMode == AESCMAC_OPMODE_CBCMAC)
    {
        operationAlgo = HSM_MAC_TOKEN_WORD6_CBC_MAC;
    }

    /* This piece of logic handles whenever a token request requries padding.
     * And it happens under the following 3 scenarios:
     *  1. Total length opration is zero.
     *  2. Total length is less than block size.
     *  3. Length-to-process is the last non-block-size-multiple chunk.
     */
    if (object->inputLength < AES_BLOCK_SIZE)
    {
        if (operationAlgo == HSM_MAC_TOKEN_WORD6_CMAC)
        {
            *(object->input + object->inputLength) = 0x80;
        }

        padbytes = AES_BLOCK_SIZE - inputLength;

        if (padbytes == AES_BLOCK_SIZE)
        {
            padbytes = 0xF;
        }

        inputLength = AES_BLOCK_SIZE;
    }

    operation.commandToken.W[0] = HSM_MAC_TOKEN_WORD0;
    operation.commandToken.W[2] = inputLength;
    operation.commandToken.W[3] = (uintptr_t)object->input;
    operation.commandToken.W[5] = inputLength;

    operation.commandToken.W[6] = ((!isFinal) << 5) | ((!isFirst) << 4) | operationAlgo;
    operation.commandToken.W[7] = object->tempAssetID;
    operation.commandToken.W[8] = object->keyAssetID;

    if (operationAlgo == HSM_MAC_TOKEN_WORD6_CMAC)
    {
        operation.commandToken.W[10] = padbytes;
    }
}

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== HSMXXF3_constructRNGSwitchNRBGWithDefaultsPhysicalToken ========
 */
void HSMXXF3_constructRNGSwitchNRBGWithDefaultsPhysicalToken(HSMXXF3_NRBGMode HSMXXF3_nrbgMode)
{
    operation.commandToken.W[0] = RNG_CONFIG_TOKEN_WORD0;
    operation.commandToken.W[2] = RNG_CONFIG_TOKEN_WORD2;

    if (HSMXXF3_nrbgMode == HSMXXF3_MODE_CRNG)
    {
        /* For CRNG mode, Bit 4 has to be high */
        operation.commandToken.W[2] |= RNG_CONFIG_TOKEN_CRNG;
    }
    else
    {
        /* When request is TRNG, do nothing. */
    }
}

/*
 *  ======== HSMXXF3_constructRNGReseedDRBGPhysicalToken ========
 */
void HSMXXF3_constructRNGReseedDRBGPhysicalToken(void)
{
    Eip130Token_Command_PRNG_ReseedNow(&operation.commandToken);
}
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

/*
 *  ======== HSMXXF3_constructRNGGetRandomNumberPhysicalToken ========
 */
void HSMXXF3_constructRNGGetRandomNumberPhysicalToken(uintptr_t entropyBuffer, size_t entropyRequested)
{
    Eip130Token_Command_RandomNumber_Generate(&operation.commandToken, entropyRequested, entropyBuffer);
}

/*
 *  ======== HSMXXF3_constructRNGGetRawRandomNumberPhysicalToken ========
 */
void HSMXXF3_constructRNGGetRawRandomNumberPhysicalToken(uintptr_t entropyBuffer, size_t entropyRequested)
{
    uint16_t rawKey = HSM_CRNG_RAW_KEY_ENC;

    if (HSMXXF3_nrbgMode == HSMXXF3_MODE_TRNG)
    {
        rawKey = HSM_TRNG_RAW_KEY_ENC;
    }

    Eip130Token_Command_RandomNumber_Generate(&operation.commandToken, entropyRequested, entropyBuffer);

    Eip130Token_Command_RandomNumber_SetRawKey(&operation.commandToken, rawKey);
}
