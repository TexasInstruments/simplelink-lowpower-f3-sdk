/*
 * Copyright (c) 2023-2026 Texas Instruments Incorporated
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

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #include <ti/drivers/EDDSA.h>
    #include <ti/drivers/eddsa/EDDSALPF3HSM.h>
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

#include <ti/drivers/TRNG.h>
#include <ti/drivers/trng/TRNGLPF3HSM.h>

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

#include <ti/drivers/ECDH.h>
#include <ti/drivers/ecdh/ECDHLPF3HSM.h>

#include <ti/drivers/ECDSA.h>
#include <ti/drivers/ecdsa/ECDSALPF3HSM.h>

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
#include <third_party/hsmddk/include/Kit/EIP130/TokenHelper/incl/eip130_token_result.h>
#include <third_party/hsmddk/include/Integration/Adapter_VEX/incl/c_adapter_vex.h>
#include <third_party/hsmddk/include/Kit/EIP201/incl/eip201.h>
#include <third_party/hsmddk/include/Kit/DriverFramework/Device_API/incl/device_mgmt.h>
#include <third_party/hsmddk/include/Integration/Adapter_Generic/incl/adapter_interrupts.h>

#include <ti/drivers/cryptoutils/sharedresources/HSMResourceXXF3.h>
#include <ti/drivers/cryptoutils/sharedresources/CommonResourceXXF3.h>
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
} HSMLPF3_systemInfoVersion_t;

typedef struct
{
    /* Word 0 is omitted */
    /* Word 1 */
    HSMLPF3_systemInfoVersion_t rambusFwVersion;
    uint8_t rollbackID;
    /* Word 2 */
    HSMLPF3_systemInfoVersion_t rambusHwVersion;
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
    HSMLPF3_systemInfoVersion_t rambusBootFwVersion;
    uint8_t res3;
    /* Word 7 */
    HSMLPF3_systemInfoVersion_t customBootFwVersion;
    uint8_t res4;
    /* Word 8 */
    HSMLPF3_systemInfoVersion_t customFwVersion;
    uint8_t res5;
    /* Word 9 */
    HSMLPF3_systemInfoVersion_t customHwVersion;
    uint8_t res6;
} HSMLPF3_SystemInfo_t;

/* Sleep state for the HSM
 *
 * The device may only enter standby in HSMLPF3_SLEEP_STATE_ASLEEP.
 * The HSM may only be used in HSMLPF3_SLEEP_STATE_AWAKE.
 *
 * State transitions are performed by HSMLPF3_wakeUp() and HSMLPF3_sleep().
 * HSMLPF3_SLEEP_STATE_WAKE_UP_TOKEN_NEEDED is normally only a transient state
 * within HSMLPF3_wakeUp(), which advances from HSMLPF3_SLEEP_STATE_ASLEEP
 * through it to HSMLPF3_SLEEP_STATE_AWAKE in a single call. If the wake up
 * token fails the state remains at HSMLPF3_SLEEP_STATE_WAKE_UP_TOKEN_NEEDED,
 * leaving the HSM clocks and mailbox enabled but the HSM not ready for use. A
 * subsequent HSMLPF3_sleep() call will still submit the sleep token and disable
 * the clocks before the device enters standby.
 *
 * ┌──────────────────────────────────────────┐
 * │  HSMLPF3_SLEEP_STATE_ASLEEP              │◄─────────────────────────────┐
 * └──────────────────────────────────────────┘                              │
 *              │                                                            │
 *              │  1. Enable HSM clocks                                      │
 *              │  2. Enable HSM mailbox (initMbox)                          │
 *              │  3. Initialize AIC                                         │
 *              ▼                                                            │
 * ┌──────────────────────────────────────────┐    HSMLPF3_sleep():          │
 * │ HSMLPF3_SLEEP_STATE_WAKE_UP_TOKEN_NEEDED ├─── 1. Submit sleep token ────┤
 * └──────────────────────────────────────────┘    2. Disable HSM clocks     │
 *              │ ▲                                                          │
 *              │ └── [wake up token failed]                                 │
 *              │  1. Submit wake up token                                   │
 *              │     [success: TRNG ok, or CRNG ok /                        │
 *              │      CRNG repetition count fail /                          │
 *              │      CRNG adaptive proportion fail]                        │
 *              ▼                                                            │
 * ┌──────────────────────────────────────────┐    HSMLPF3_sleep():          │
 * │ HSMLPF3_SLEEP_STATE_AWAKE                ├─── 1. Submit sleep token ────┘
 * └──────────────────────────────────────────┘    2. Disable HSM clocks
 *
 */
typedef enum
{
    /* The HSM is asleep.
     * The HSM must be in this state before the device enters standby.
     * The HSM cannot be used in this state.
     */
    HSMLPF3_SLEEP_STATE_ASLEEP = 0,

    /* The HSM clocks are enabled, the mailbox is initialized and the AIC is
     * configured, but a wake up token has not yet been submitted. This state is
     * normally only seen transiently during HSMLPF3_wakeUp(), which submits the
     * wake up token immediately after entering it. If the wake up token fails,
     * the state persists here with clocks and mailbox enabled but the HSM not
     * ready for use. HSMLPF3_sleep() will still submit the sleep token and
     * disable the clocks in this state, allowing the device to enter standby.
     * The HSM cannot be used in this state.
     */
    HSMLPF3_SLEEP_STATE_WAKE_UP_TOKEN_NEEDED = 1,

    /* The HSM is fully awake. The HSM can be used in this state and the device
     * should not enter standby in this state.
     */
    HSMLPF3_SLEEP_STATE_AWAKE = 2,
} HSMLPF3_SleepState_t;

#define BOOT_DELAY 0xFFFFF

#define SLEEP_TOKEN_WORD0 0x4F000000

#define WAKEUP_TOKEN_WORD0 0x5F000000

#define RNG_CONFIG_TOKEN_WORD0 0x14000000
#define RNG_CONFIG_TOKEN_WORD2 0x00000004
#define RNG_CONFIG_TOKEN_CRNG  0x00000010

#define BOOT_TOKEN_WORD0 0xCF000000

#define BOOT_TOKEN_WORD1 0x03725746

#define SYSTEMINFO_TOKEN_WORD0 0x0F030000
#define SYSTEMINFO_PATCH_MASK  0x000000FF
#define SYSTEMINFO_MINOR_MASK  0x0000FF00
#define SYSTEMINFO_MAJOR_MASK  0x00FF0000

#define CRYPTO_OFFICER_ID 0x4F5A3647

/* ======== HSM Input Token bit fields ======== */
/* Field: [27:24] OpCode */
#define INPUT_TOKEN_OPCODE_M  (0x0F000000U)
/* Field: [27:24] SubCode */
#define INPUT_TOKEN_SUBCODE_M (0xF0000000U)

/* ======== HSM Output Token bit fields ======== */
/* Field: [31:31] Error
 *
 * Enums:
 * - SUCCESS: No error occurred, operation completed successfully.
 * - ERROR: Error occurred.
 */
#define OUTPUT_TOKEN_ERROR_M       (0x80000000U)
#define OUTPUT_TOKEN_ERROR_SUCCESS (0x00000000U)
#define OUTPUT_TOKEN_ERROR_ERROR   (0x80000000U)

/* Field: [30:29] ResultSrc */
#define OUTPUT_TOKEN_RESULT_SRC_M (0x60000000U)

/* Field: [28:24] Result */
#define OUTPUT_TOKEN_RESULT_M (0x1F000000U)

/* Field: [31:24] Combined Result
 *
 * This is an aggregated field consisting of the Error, ResultSrc and Result
 * fields.
 *
 * Enums:
 * - SUCCESS: No error occurred, operation completed successfully.
 * - CRNG_REPETITION_COUNT_FAIL: CRNG 'Repetition Count' test failure.
 * - CRNG_ADAPTIVE_PROPORTION_FAIL: CRNG 'Adaptive Proportion' test failure.
 */
#define OUTPUT_TOKEN_COMBINED_RESULT_M                             (OUTPUT_TOKEN_ERROR_M | OUTPUT_TOKEN_RESULT_SRC_M | OUTPUT_TOKEN_RESULT_M)
#define OUTPUT_TOKEN_COMBINED_RESULT_SUCCESS                       (0x00000000U)
#define OUTPUT_TOKEN_COMBINED_RESULT_CRNG_REPETITION_COUNT_FAIL    (0xCA000000U)
#define OUTPUT_TOKEN_COMBINED_RESULT_CRNG_ADAPTIVE_PROPORTION_FAIL (0xCC000000U)

/* Field: [17:17] RNG Warning
 *
 * If the bit in this field is set in the output token it indicates that the RNG
 * is in a warning state. Meaning a reseed is needed within the next 64KB of
 * random data generation.
 */
#define OUTPUT_TOKEN_RNG_WARNING_M (0x00020000U)

#define AESGCM_IV_LAST_WORD 0x01000000

#define AES_BLOCK_SIZE_ALIGN 0X0F

#define BLOCK_SIZE 16U

#define HSM_TOKEN_WORD1_OFFSET 0x4

#define HSM_HUK_ASSET_NUMBER 0x61

#define HUK_PROVISION_TOKEN_WORD0 0x97000000

#define HSM_SEARCH_TOKEN_WORD0 0x07000000

/* bit 17 has to be high to indicate a 256bit HUK asset size. */
#define HUK_PROVISION_TOKEN_WORD2_256BIT 0x00020000
#define HUK_PROVISION_TOKEN_WORD2        RNG_CONFIG_TOKEN_WORD2 | HUK_PROVISION_TOKEN_WORD2_256BIT

#define HSM_CRNG_RAW_KEY_ENC 0x7264
#define HSM_TRNG_RAW_KEY_ENC 0x5244

#define HSM_ECDH_GEN_PUB_KEY_ASSET_ID_UPPDER_VALUE 0xFFFFFFFF00000000

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

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    /* RNG Configuration retry limit (CC27XX only) */
    #define HSMLPF3_RNG_CONFIG_MAX_RETRIES 100
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

/* Used by crypto drivers in blocking mode to wait on a result */
static SemaphoreP_Struct HSMLPF3_operationSemaphore;

static HwiP_Struct HSMLPF3_hwi;
static HSMLPF3_Operation operation;

static bool HSMLPF3_isInitialized   = false;
static bool HSMLPF3_rtosInitialized = false;

static volatile HSMLPF3_SystemInfo_t HSMLPF3_engineSystemInfo;

static int_fast16_t HSMLPF3_hsmReturnStatus;

static bool HSMLPF3_operationInProgress = false;

static HSMLPF3_SleepState_t HSMLPF3_sleepState = HSMLPF3_SLEEP_STATE_AWAKE;

static Power_NotifyObj postNotify;

/* Keep a global variable to track the overall HSM RNG NRBG engine mode */
static HSMLPF3_NRBGMode HSMLPF3_nrbgMode = HSMLPF3_MODE_CRNG;

/* Forward declarations */
static void HSMLPF3_writeToken(const uint32_t *token, uint32_t len);
static void HSMLPF3_hwiFxn(uintptr_t arg0);
static int_fast16_t HSMLPF3_boot(void);
static void HSMLPF3_initMbox(void);
static void HSMLPF3_enableClock(void);
static void HSMLPF3_initAIC(void);
static int_fast16_t HSMLPF3_getEngineSystemInfo();

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
static int_fast16_t HSMLPF3_submitResetToken(void);
static int_fast16_t HSMLPF3_searchStaticAsset(uint32_t assetNumber, uint32_t *assetID);
static int_fast16_t HSMLPF3_isHSMfirmwareImgAccepted(void);
static int_fast16_t HSMLPF3_handleRngWarning(void);
static int_fast16_t HSMLPF3_initializeRng(void);
static uint32_t HSMLPF3_configureRng(void);
static int_fast16_t HSMLPF3_provisionHukInternal(HSMLPF3_NRBGMode nrbgMode);

/*
 *  ======== HSMLPF3_isHSMfirmwareImgAccepted ========
 */
static int_fast16_t HSMLPF3_isHSMfirmwareImgAccepted(void)
{
    return ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODSTA) & HSMCRYPTO_MODSTA_FWACPTD_M) == HSMCRYPTO_MODSTA_FWACPTD);
}

#endif

/* Write directly to HSM Mailbox */
static void HSMLPF3_writeToken(const uint32_t *token, uint32_t len)
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
        if (operation.callbackFxn != NULL)
        {
            /* Call driver post-processing function and pass back the handle */
            (void)operation.callbackFxn(operation.driverHandle);
        }
    }
}

/*
 *  ======== HSMLPF3_postNotifyFxn ========
 */
static int_fast16_t HSMLPF3_postNotifyFxn(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg)
{
    int_fast16_t result = Power_NOTIFYERROR;

    /* Send in a sleep/wakeup token depending on eventType */
    if (eventType == PowerLPF3_ENTERING_STANDBY)
    {
        if (HSMLPF3_sleep() == HSMLPF3_STATUS_SUCCESS)
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
 *  ======== HSMLPF3_boot ========
 */
static int_fast16_t HSMLPF3_boot(void)
{
    uint32_t token[2];
    uint32_t result = HSMLPF3_STATUS_ERROR;

    /* Initialize HSM clock and mailbox, then boot it */
    HSMLPF3_enableClock();

    HSMLPF3_initMbox();

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
        if (HSMLPF3_isHSMfirmwareImgAccepted())
        {
            result = HSMLPF3_STATUS_SUCCESS;
        }
        else
        {
            HSMLPF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

            /* Wait for result in mbx1_out */
            while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) !=
                    HSMCRYPTO_MBSTA_MB1OUT_FULL)
            {}

            if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR_M) != OUTPUT_TOKEN_ERROR_SUCCESS)
            {
                /* Notify the HSM that the mailbox has been read */
                HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;
                /* The result returned from this path is HSMLPF3_STATUS_ERROR */
            }
            else
            {
                /* Notify the HSM that the mailbox has been read */
                HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

                for (delay = BOOT_DELAY; delay; delay--)
                {
                    if (HSMLPF3_isHSMfirmwareImgAccepted())
                    {
                        result = HSMLPF3_STATUS_SUCCESS;

                        break;
                    }
                }

                /* Initialize RNG to ensure the DRBG has a valid seed before
                 * any operation that might need random numbers from the DRBG.
                 */
                if (result == HSMLPF3_STATUS_SUCCESS)
                {
                    result = HSMLPF3_initializeRng();
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
            HSMLPF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

            /* Wait for result in mbx1_out */
            while ((HWREG(HSMCRYPTO_BASE + HSM_O_MAILBOX_STAT) & HSM_MAILBOX_STAT_OUTFULL1) !=
                    HSM_MAILBOX_STAT_OUTFULL1)
            {}

            if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR_M) != OUTPUT_TOKEN_ERROR_SUCCESS)
            {
                /* Notify the HSM that the mailbox has been read */
                HWREG(HSMCRYPTO_BASE + HSM_O_MBXCTL) = HSM_MBXCTL_OUTEMP1;
                /* The result returned from this path is HSMLPF3_STATUS_ERROR */
            }
            else
            {
                /* Notify the HSM that the mailbox has been read */
                HWREG(HSMCRYPTO_BASE + HSM_O_MBXCTL) = HSM_MBXCTL_OUTEMP1;
                result                               = HSMLPF3_STATUS_SUCCESS;
                /* Register power notification function */
                Power_registerNotify(&postNotify, PowerWFF3_ENTERING_SLEEP, HSMLPF3_postNotifyFxn, (uintptr_t)NULL);
            }
        }
    }
#endif

    return result;
}

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== HSMLPF3_handleRngWarning ========
 * The HSM will indicate in all output tokens if a DRBG re-seed is needed within
 * 64KiB of random data. If the HSM is within the 64KiB limit, we will trigger
 * a re-seed from software to be able to retry the re-seed if it is failing.
 * The retry logic is implemented in HSMLPF3_initializeRng() which is called
 * from this function if the HSM is within the 64KiB limit.
 */
static int_fast16_t HSMLPF3_handleRngWarning(void)
{
    int_fast16_t status    = HSMLPF3_STATUS_ERROR;
    uint32_t resTokenWord0 = operation.resultToken.W[0];

    if ((resTokenWord0 & OUTPUT_TOKEN_RNG_WARNING_M) != 0U)
    {
        status = HSMLPF3_initializeRng();
    }
    else
    {
        status = HSMLPF3_STATUS_SUCCESS;
    }

    return status;
}

/*
 *  ======== HSMLPF3_configureRng ========
 * This function configures the HSM RNG engine to operate in either CRNG or TRNG
 * mode based on the current setting of HSMLPF3_nrbgMode. And return the first
 * word of the HSM output token masked by OUTPUT_TOKEN_COMBINED_RESULT_M.
 */
static uint32_t HSMLPF3_configureRng(void)
{
    uint32_t result;
    uint32_t token[4] = {
        RNG_CONFIG_TOKEN_WORD0,                                                                           /* Word 0 */
        CRYPTO_OFFICER_ID,                                                                                /* Word 1 */
        (RNG_CONFIG_TOKEN_WORD2 | ((HSMLPF3_nrbgMode == HSMLPF3_MODE_CRNG) ? RNG_CONFIG_TOKEN_CRNG : 0)), /* Word 2 */
        0                                                                                                 /* Word 3 */
    };

    /* Submit token */
    HSMLPF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

    /* Poll for result */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL) {}

    /* Read result token */
    result = HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT);

    /* Clear mailbox */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

    /* Extract and return error code */
    return (result & OUTPUT_TOKEN_COMBINED_RESULT_M);
}

/*
 *  ======== HSMLPF3_initializeRng ========
 * This function attempts to initialize the HSM RNG engine in the currently
 * selected NRBG mode (CRNG or TRNG) with a retry mechanism in case of failure.
 * If the initialization fails after the maximum number of retries, it switches
 * to TRNG mode and tries one final time before returning an error status.
 */
static int_fast16_t HSMLPF3_initializeRng(void)
{
    uint32_t errorCode;
    uint16_t retry;

    /* Retry loop up to HSMLPF3_RNG_CONFIG_MAX_RETRIES times */
    for (retry = 0; retry < HSMLPF3_RNG_CONFIG_MAX_RETRIES; retry++)
    {
        /* Attempt RNG configuration (with NRBG mode selected by
         * HSMLPF3_nrbgMode)
         */
        errorCode = HSMLPF3_configureRng();

        /* Check if successful */
        if (errorCode == OUTPUT_TOKEN_COMBINED_RESULT_SUCCESS)
        {
            return HSMLPF3_STATUS_SUCCESS;
        }
    }

    /* If all the retry attempts failed, switch to TRNG and try one more time.
     */
    HSMLPF3_nrbgMode = HSMLPF3_MODE_TRNG;

    /* Configure RNG using TRNG as the selected NRBG mode. */
    errorCode = HSMLPF3_configureRng();

    /* Check if successful */
    if (errorCode == OUTPUT_TOKEN_COMBINED_RESULT_SUCCESS)
    {
        return HSMLPF3_STATUS_SUCCESS;
    }
    else
    {
        return HSMLPF3_STATUS_ERROR;
    }
}
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

/*
 *  ======== HSMLPF3_initMbox ========
 */
static void HSMLPF3_initMbox(void)
{
    /* Link mailbox */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1LNK_LNK | HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA);

    /* Allow non-secure/secure access (Set bits 7 and 3 to 1 if we need secure access) */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLNKID) = 0x00;

    /* Make sure CPU_ID=0 host can access mailbox 1 & 2 (no lockout) */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLCKOUT) = 0xFFFFFF77 & HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBLCKOUT);
}

/*
 *  ======== HSMLPF3_initAIC ========
 */
static void HSMLPF3_initAIC(void)
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
 *  ======== HSMLPF3_enableClock ========
 */
static void HSMLPF3_enableClock(void)
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

        /* Initialize the CommonResource access semaphore, needed due to errata SYS_211. */
        CommonResourceXXF3_constructRTOSObjects();
        HSMResourceXXF3_constructRTOSObject();
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
 *  ======== HSMLPF3_sleep ========
 */
int_fast16_t HSMLPF3_sleep(void)
{
    int_fast16_t result = HSMLPF3_STATUS_ERROR;
    uint32_t token[2];

    /* If the HSM is not currently asleep, submit sleep token and disable HSM
     * clocks.
     */
    if (HSMLPF3_sleepState != HSMLPF3_SLEEP_STATE_ASLEEP)
    {
        token[0] = SLEEP_TOKEN_WORD0;
        token[1] = CRYPTO_OFFICER_ID;

        HSMLPF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

        /* Wait for result in mbx1_out */
        while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL) {}

        /* Check if output token indicates success. */
        if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR_M) == OUTPUT_TOKEN_ERROR_SUCCESS)
        {
            result = HSMLPF3_STATUS_SUCCESS;

            /* Mark mbx1_out as empty */
            HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

            HSMLPF3_disableClock();

            HSMLPF3_sleepState = HSMLPF3_SLEEP_STATE_ASLEEP;
        }
    }
    else
    {
        result = HSMLPF3_STATUS_SUCCESS;
    }

    return result;
}

/*
 *  ======== HSMLPF3_wakeUp ========
 */
int_fast16_t HSMLPF3_wakeUp(void)
{
    int_fast16_t result = HSMLPF3_STATUS_ERROR;
    uint32_t resTokenCode;
    uint32_t token[2];

    /* Enable clocks, mailbox and AIC if in sleep state. */
    if (HSMLPF3_sleepState == HSMLPF3_SLEEP_STATE_ASLEEP)
    {
        HSMLPF3_enableClock();

        HSMLPF3_initMbox();

        HSMLPF3_initAIC();

        HSMLPF3_sleepState = HSMLPF3_SLEEP_STATE_WAKE_UP_TOKEN_NEEDED;
    }

    /* Submit wakeup token if in "WakeUp token needed" state. */
    if (HSMLPF3_sleepState == HSMLPF3_SLEEP_STATE_WAKE_UP_TOKEN_NEEDED)
    {

        token[0] = WAKEUP_TOKEN_WORD0;
        token[1] = CRYPTO_OFFICER_ID;

        HSMLPF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

        /* Wait for result in mbx1_out */
        while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL) {}

        resTokenCode = (HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_COMBINED_RESULT_M);

        if (HSMLPF3_nrbgMode == HSMLPF3_MODE_TRNG)
        {
            if (resTokenCode == OUTPUT_TOKEN_COMBINED_RESULT_SUCCESS)
            {
                HSMLPF3_sleepState = HSMLPF3_SLEEP_STATE_AWAKE;
            }
        }
        else if (HSMLPF3_nrbgMode == HSMLPF3_MODE_CRNG)
        {
            /* Treat CRNG 'Repetition Count' and 'Adaptive Proportion' test
             * failures as successful. These tests are HW driven and cannot be
             * bypassed, but it is not necessary since any subsequent (re)seeds
             * will take care of these test failures.
             */
            if ((resTokenCode == OUTPUT_TOKEN_COMBINED_RESULT_SUCCESS) ||
                (resTokenCode == OUTPUT_TOKEN_COMBINED_RESULT_CRNG_REPETITION_COUNT_FAIL) ||
                (resTokenCode == OUTPUT_TOKEN_COMBINED_RESULT_CRNG_ADAPTIVE_PROPORTION_FAIL))
            {
                HSMLPF3_sleepState = HSMLPF3_SLEEP_STATE_AWAKE;
            }
        }

        /* Mark mbx1_out as empty */
        HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;
    }

    /* Return success if in Awake state. */
    if (HSMLPF3_sleepState == HSMLPF3_SLEEP_STATE_AWAKE)
    {
        result = HSMLPF3_STATUS_SUCCESS;
    }

    return result;
}

/*
 *  ======== HSMLPF3_getEngineSystemInfo ========
 */
static int_fast16_t HSMLPF3_getEngineSystemInfo(void)
{
    int_fast16_t status    = HSMLPF3_STATUS_ERROR;
    uint32_t inputToken[2] = {0};
    uint32_t *outputToken  = (uint32_t *)(HSMCRYPTO_BASE);

    /* Try and obtain access to the crypto module */
    if (!HSMLPF3_acquireLock(SemaphoreP_NO_WAIT, (uintptr_t)0U))
    {
        /* Acquiring the lock failed so we return immediately */
        return HSMLPF3_STATUS_RESOURCE_UNAVAILABLE;
    }

    inputToken[0] = SYSTEMINFO_TOKEN_WORD0;
    inputToken[1] = CRYPTO_OFFICER_ID;

    HSMLPF3_writeToken(inputToken, sizeof(inputToken) / sizeof(uint32_t));

    /* Wait for result in mbx1_out */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL) {}

    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR_M) == OUTPUT_TOKEN_ERROR_SUCCESS)
    {
        status = HSMLPF3_STATUS_SUCCESS;

        memcpy((void *)&HSMLPF3_engineSystemInfo, &outputToken[1], sizeof(HSMLPF3_SystemInfo_t));
    }

    /* Mark mbx1_out as empty */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

    /* Release the access semaphore */
    HSMLPF3_releaseLock();

    return status;
}

/*
 *  ======== HSMLPF3_isStandaloneDMASupportEnabled ========
 */
bool HSMLPF3_isStandaloneDMASupportEnabled(void)
{
    bool retval = false;

    /* HSM FW versions beyond 3.1.0 have DMA operations decoupled from ECC operations. */
    if ((HSMLPF3_isInitialized) && (((HSMLPF3_engineSystemInfo.customFwVersion.major == 3) &&
                                     (HSMLPF3_engineSystemInfo.customFwVersion.minor == 1)) ||
                                    (HSMLPF3_engineSystemInfo.customFwVersion.major >= 4)))
    {
        retval = true;
    }

    return retval;
}

/*
 *  ======== HSMLPF3_init ========
 */
int_fast16_t HSMLPF3_init(void)
{
    uintptr_t key;

    if (!HSMLPF3_isInitialized)
    {
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
        /* Disable HSM Firewall on CC35XX. */
        HWREG(SOC_AON_BASE + SOC_AON_O_HSMCFG) |= SOC_AON_HSMCFG_FIREWALL;
#endif

        key = HwiP_disable();

        memset((void *)&HSMLPF3_engineSystemInfo, 0, sizeof(HSMLPF3_engineSystemInfo));

        if (HSMLPF3_boot() != HSMLPF3_STATUS_SUCCESS)
        {
            HSMLPF3_hsmReturnStatus = HSMLPF3_STATUS_ERROR;

            HwiP_restore(key);
        }
        else
        {
            HwiP_restore(key);

            /* Register power notification function */
            Power_registerNotify(&postNotify, PowerLPF3_ENTERING_STANDBY, HSMLPF3_postNotifyFxn, (uintptr_t)0U);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
            Power_setDependency(PowerLPF3_PERIPH_HSM);
#endif

            if (HSMSAL_Init() != HSMSAL_SUCCESS)
            {
                /* HSMSAL_Init() can fail if HSM interrupt engine is unresponsive. */
                HSMLPF3_hsmReturnStatus = HSMLPF3_STATUS_ERROR;
            }
            else
            {
                HSMLPF3_hsmReturnStatus = HSMLPF3_getEngineSystemInfo();

                if (HSMLPF3_hsmReturnStatus == HSMLPF3_STATUS_SUCCESS)
                {
                    HSMLPF3_isInitialized = true;
                }
            }
        }

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
        HSMLPF3_nrbgMode = HSMLPF3_MODE_TRNG;
#else
        HSMLPF3_nrbgMode = HSMLPF3_MODE_CRNG;
#endif
    }

    return HSMLPF3_hsmReturnStatus;
}

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== HSMLPF3_searchStaticAsset ========
 */
static int_fast16_t HSMLPF3_searchStaticAsset(uint32_t assetNumber, uint32_t *assetID)
{
    int_fast16_t status = HSMLPF3_STATUS_ERROR;
    uint32_t token[5]   = {
        HSM_SEARCH_TOKEN_WORD0, /* Word 0*/
        CRYPTO_OFFICER_ID,      /* Word 1 */
        0,                      /* Word 2 */
        0,                      /* Word 3 */
        (assetNumber << 16)     /* Word 4 */
    };

    /* Try and obtain access to the crypto module */
    HSMLPF3_acquireLock(SemaphoreP_WAIT_FOREVER, (uintptr_t)0U);

    /* Acquire HSM semaphore to prevent AHB bus master transactions. There is no
     * protection against I2S bus master so I2S cannot be used at the same
     * time as CAN.
     */
    CommonResourceXXF3_acquireLock(SemaphoreP_WAIT_FOREVER);

    /* Write the token to the HSM */
    HSMLPF3_writeToken(&token[0], sizeof(token) / sizeof(uint32_t));

    /* Wait for result in mbx1_out */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL) {}

    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR_M) == OUTPUT_TOKEN_ERROR_SUCCESS)
    {
        *assetID = HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT + HSM_TOKEN_WORD1_OFFSET);

        status = HSMLPF3_STATUS_SUCCESS;
    }

    /* Mark mbx1_out as empty */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

    CommonResourceXXF3_releaseLock();

    /* Release the access semaphore */
    HSMLPF3_releaseLock();

    return status;
}

/*
 *  ======== HSMLPF3_provisionHukInternal ========
 * Provisions the HUK using the NRBG mode specified by the caller. This function
 * is intended to be called internally by HSMLPF3_provisionHUK after acquiring
 * the HSM lock and the CommonResourceXXF3 lock, and it assumes that the caller
 * has already checked that the HUK is not provisioned before calling this
 * function.
 */
static int_fast16_t HSMLPF3_provisionHukInternal(HSMLPF3_NRBGMode nrbgMode)
{
    int_fast16_t status = HSMLPF3_STATUS_ERROR;
    uint32_t token[3]   = {
        HUK_PROVISION_TOKEN_WORD0,                                                                  /* Word 0 */
        CRYPTO_OFFICER_ID,                                                                          /* Word 1 */
        (HUK_PROVISION_TOKEN_WORD2 | ((nrbgMode == HSMLPF3_MODE_CRNG) ? RNG_CONFIG_TOKEN_CRNG : 0)) /* Word 2 */
    };

    /* Enable the OTP interrupt event */
    HWREG(HSM_BASE + HSM_O_CTL) |= HSM_CTL_OTPEVTEN_EN;

    /* Write the token to the HSM */
    HSMLPF3_writeToken(&token[0], sizeof(token) / sizeof(uint32_t));

    /* Wait until the the HSM has processed the token and returned a token
     * back.
     */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL)
    {
        HWREG(HSM_BASE + HSM_O_CTL) |= HSM_CTL_OTPEVTCLR_CLR;
    }

    /* Check the result */
    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR_M) == OUTPUT_TOKEN_ERROR_SUCCESS)
    {
        status = HSMLPF3_STATUS_SUCCESS;
    }
    else
    {
        /* Do nothing - error status already set */
    }

    /* Clear the mailbox */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

    /* Disable the OTP interrupt event */
    HWREG(HSM_BASE + HSM_O_CTL) &= ~HSM_CTL_OTPEVTEN_EN;

    return status;
}
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

/*
 *  ======== HSMLPF3_provisionHUK ========
 */
int_fast16_t HSMLPF3_provisionHUK(void)
{
    int_fast16_t status = HSMLPF3_STATUS_ERROR;

    if (!HSMLPF3_isInitialized)
    {
        return status;
    }

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    uint32_t hukAssetID = 0U;
    uint16_t retry;

    /* #HSMLPF3_init() retrieves the HSM engine info.
     * - If the .co field in the returned data is high, it means a previous run
     *   of the application called this API.
     * - If the .co filed is low, it means this is the first time this API is
     *   called.
     */
    if (HSMLPF3_engineSystemInfo.co)
    {
        /* A previous call to #HSMLPF3_provisionHUK() API already provisioned the HUK. */
        return HSMLPF3_STATUS_SUCCESS;
    }

    /* Perform static asset search first to determine if the HUK has already
     * been provisioned.
     */
    status = HSMLPF3_searchStaticAsset(HSM_HUK_ASSET_NUMBER, &hukAssetID);

    if ((status == HSMLPF3_STATUS_SUCCESS) && (hukAssetID != 0U))
    {
        /* HUK already provisioned, return success */
        return HSMLPF3_STATUS_SUCCESS;
    }
    else
    {
        /* HUK asset not found, continue with provisioning. It is possible
         * that the static asset search returned an error other than
         * 'Invalid Asset', but that shouldn't prevent our attempt
         * to provision the HUK.
         */
    }

    /* Try and obtain access to the crypto module */
    HSMLPF3_acquireLock(SemaphoreP_WAIT_FOREVER, (uintptr_t)0U);

    /* Acquire HSM semaphore to prevent AHB bus master transactions. There is no
     * protection against I2S bus master so I2S cannot be used at the same
     * time as CAN.
     */
    CommonResourceXXF3_acquireLock(SemaphoreP_WAIT_FOREVER);

    /* Retry loop up to HSMLPF3_RNG_CONFIG_MAX_RETRIES times */
    for (retry = 0; retry < HSMLPF3_RNG_CONFIG_MAX_RETRIES; retry++)
    {
        /* Attempt to provision HUK. */
        status = HSMLPF3_provisionHukInternal(HSMLPF3_nrbgMode);

        /* Check if successful */
        if (HSMLPF3_STATUS_SUCCESS == status)
        {
            break;
        }
    }

    /* If all attempts to provision HUK failed, try one last time using TRNG. */
    if (HSMLPF3_STATUS_SUCCESS != status)
    {
        status = HSMLPF3_provisionHukInternal(HSMLPF3_MODE_TRNG);
    }

    CommonResourceXXF3_releaseLock();

    /* Release the access semaphore */
    HSMLPF3_releaseLock();

    /* Reset after OTP writes */
    if (HSMLPF3_STATUS_SUCCESS == status)
    {
        status = HSMLPF3_submitResetToken();
    }

    /* If the reset was successful, then we need to re-initialize the RNG,
     * to make sure future operations requiring random numbers have a valid
     * seed. This will be done using the NRBG mode that was selected before
     * HSMLPF3_provisionHUK was called.
     */
    if (HSMLPF3_STATUS_SUCCESS == status)
    {
        status = HSMLPF3_initializeRng();
    }

    /* Perform static asset search again to verify that the HUK has now been
     * provisioned.
     */
    if (HSMLPF3_STATUS_SUCCESS == status)
    {
        status = HSMLPF3_searchStaticAsset(HSM_HUK_ASSET_NUMBER, &hukAssetID);
    }

    if ((HSMLPF3_STATUS_SUCCESS == status) && (hukAssetID == 0U))
    {
        /* Treat a HUK asset id of 0 as an error, even if the asset search
         * itself was successful.
         */
        status = HSMLPF3_STATUS_ERROR;
    }

#elif (DeviceFamily_PARENT == DeviceFamily_PARENT_CC35XX)
    /* HUK is provisioned on CC35XX at boot time by TI Device
     * boot loader. As a result, HUK provisioning is not needed.
     * Returning sucess here.
     */
    status = HSMLPF3_STATUS_SUCCESS;
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

    return status;
}

/*
 *  ======== HSMLPF3_acquireLock ========
 */
bool HSMLPF3_acquireLock(uint32_t timeout, uintptr_t driverHandle)
{
    bool status         = false;
    int_fast16_t retval = HSMLPF3_STATUS_ERROR;
    bool isResourceAcquired;

    /* Try and obtain access to the crypto module */
    isResourceAcquired = HSMResourceXXF3_acquireLock(timeout);

    if (isResourceAcquired)
    {
        operation.driverHandle = driverHandle;

        (void)memset(&operation.commandToken, 0, sizeof(Eip130Token_Command_t));
        (void)memset(&operation.resultToken, 0, sizeof(Eip130Token_Result_t));

        /* Wakeup HSM. The HSMLPF3_wakeUp() function will just return success
         * if HSM is already awake.
         */
        retval = HSMLPF3_wakeUp();

        if (retval == HSMLPF3_STATUS_SUCCESS)
        {
            Power_setConstraint(PowerLPF3_DISALLOW_STANDBY);

            status = true;
        }
        else
        {
            operation.driverHandle = 0U;

            HSMResourceXXF3_releaseLock();
        }
    }

    return status;
}

/*
 *  ======== HSMLPF3_releaseLock ========
 */
void HSMLPF3_releaseLock(void)
{
    operation.driverHandle = 0U;

    HSMResourceXXF3_releaseLock();

    Power_releaseConstraint(PowerLPF3_DISALLOW_STANDBY);
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
    uint32_t comTokenWord0 = operation.commandToken.W[0];

    if (HSMLPF3_sleepState != HSMLPF3_SLEEP_STATE_AWAKE)
    {
        return HSMLPF3_STATUS_IN_SLEEP_MODE;
    }

    if ((comTokenWord0 & (INPUT_TOKEN_OPCODE_M | INPUT_TOKEN_SUBCODE_M)) ==
        (RNG_CONFIG_TOKEN_WORD0 & (INPUT_TOKEN_OPCODE_M | INPUT_TOKEN_SUBCODE_M)))
    {
        /* HSMLPF3 will DISALLOW any higher level SW stacks to submit operations
         * that tampers with the RNG system.
         * Mainly, This restriction applies to the TRNGLPF3HSM driver for two
         * operations:
         * 1. Switch NRBG entorpy source.
         * 2. On-demand force re-seed of the DRNG engine.
         *
         * For the 1st usecase, countermeasures are implemented in the HSMLPF3
         * module to automatically switch the NRBG entroy source when the
         * default fails.
         * For the 2nd usecase, this is handled automatically by both the HSM
         * engine and the HSMLPF3 module.
         */
        return HSMLPF3_STATUS_ERROR;
    }

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

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
/*
 *  ======== HSMLPF3_submitResetToken ========
 */
static int_fast16_t HSMLPF3_submitResetToken(void)
{
    int_fast16_t status = HSMLPF3_STATUS_ERROR;

    uint32_t token[1] = {0};

    /* Try and obtain access to the crypto module */
    if (!HSMLPF3_acquireLock(SemaphoreP_NO_WAIT, (uintptr_t)0U))
    {
        /* Acquiring the lock failed so we return immediately */
        return HSMLPF3_STATUS_RESOURCE_UNAVAILABLE;
    }

    /* Set the token for reset */
    token[0] = (EIP130TOKEN_OPCODE_SYSTEM | EIP130TOKEN_SUBCODE_RESET);

    /* Write the token to the HSM */
    HSMLPF3_writeToken(&token[0], sizeof(token) / sizeof(uint32_t));

    /* The HSM has processed the token and returned a token back */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBSTA) & HSMCRYPTO_MBSTA_MB1OUT_M) != HSMCRYPTO_MBSTA_MB1OUT_FULL)
    {
        HWREG(HSM_BASE + HSM_O_CTL) |= HSM_CTL_OTPEVTCLR_CLR;
    }

    /* Check the result to see if anything went wrong */
    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MB1OUT) & OUTPUT_TOKEN_ERROR_M) == OUTPUT_TOKEN_ERROR_SUCCESS)
    {
        status = HSMLPF3_STATUS_SUCCESS;
    }
    else
    {
        /* Do nothing */
    }

    /* Clear the mailbox */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBCTL) = HSMCRYPTO_MBCTL_MB1OUT_EMTY;

    /* Release the access semaphore */
    HSMLPF3_releaseLock();

    return status;
}
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

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
    uintptr_t key;

    key = HwiP_disable();

    if (HSMLPF3_operationInProgress)
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
            result = HSMLPF3_STATUS_TIMEOUT;
        }

        HSMLPF3_operationInProgress = false;

        /* Release the CommonResource semaphore. */
        CommonResourceXXF3_releaseLock();

        /* The post-processing function typically releases the lock and power constraint,
         * but the cancel operation is now responsible for it.
         */
        HSMLPF3_releaseLock();
    }
    else
    {
        HwiP_restore(key);
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
 *  ======== HSMLPF3_getCurrentNRBGMode ========
 */
HSMLPF3_NRBGMode HSMLPF3_getCurrentNRBGMode(void)
{
    return HSMLPF3_nrbgMode;
}

/*
 *  ======== HSMLPF3_updateInternalNRBGMode ========
 */
void HSMLPF3_updateInternalNRBGMode(void)
{
    if (HSMLPF3_nrbgMode == HSMLPF3_MODE_CRNG)
    {
        HSMLPF3_nrbgMode = HSMLPF3_MODE_TRNG;
    }
    else
    {
        HSMLPF3_nrbgMode = HSMLPF3_MODE_CRNG;
    }
}

/*
 *  ================ APIs to handle result token data ================
 */

/*
 *  ======== HSMLPF3_getResultCode ========
 */
int32_t HSMLPF3_getResultCode(void)
{
    int_fast16_t status = HSMLPF3_STATUS_SUCCESS;
    int32_t resultCode  = Eip130Token_Result_Code(&operation.resultToken);

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    status = HSMLPF3_handleRngWarning();

    if (status != HSMLPF3_STATUS_SUCCESS)
    {
        /* If handling the RNG warning fails it means that reseeding the DRBG
         * failed HSMLPF3_RNG_CONFIG_MAX_RETRIES number of times with the
         * currently selected NRBG mode AND then a final attempt to reseed using
         * TRNG also failed.
         * There is nothing we can do about that. Return a panic error.
         */
        resultCode = EIP130TOKEN_RESULT_PANIC_ERROR;
    }
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

    return resultCode;
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
void HSMLPF3_getResultDigest(uint8_t *digest, size_t digestLength)
{
    Eip130Token_Result_Hash_CopyState(&operation.resultToken, digestLength, digest);
}

/*
 *  ======== HSMLPF3_getAESEncryptTag ========
 */
void HSMLPF3_getAESEncryptTag(void *mac, size_t macLength)
{
    (void)memcpy(mac, &operation.resultToken.W[6], macLength);
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
 *  ======== HSMLPF3_getPublicDataRead ========
 */
void HSMLPF3_getPublicDataRead(uint32_t assetId, const uint8_t *data, uint8_t dataLength)
{
    Eip130Token_Command_PublicData_Read(&operation.commandToken, assetId, (uintptr_t)data, dataLength);
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
 *  ======== HSMLPF3_constructECDSAPhysicalToken ========
 */
void HSMLPF3_constructECDSAPhysicalToken(ECDSALPF3HSM_Object *object)
{
    const uint8_t nWord            = HSM_ASYM_DATA_SIZE_B2W(object->curveLength);
    Eip130TokenDmaAddress_t output = 0U;
    uint32_t signSize              = (HSM_SIGNATURE_VCOUNT * (HSM_ASYM_DATA_SIZE_VWB(object->curveLength)));
    uint8_t command                = VEXTOKEN_PKAS_ECDSA_SIGN;

    if (object->operationType == ECDSA_OPERATION_TYPE_VERIFY)
    {
        command = VEXTOKEN_PKAS_ECDSA_VERIFY;
    }

    if (HSMLPF3_isStandaloneDMASupportEnabled())
    {
        output = HSM_ECDH_GEN_PUB_KEY_ASSET_ID_UPPDER_VALUE | ((Eip130TokenDmaAddress_t)object->publicObjAssetID);
    }
    else
    {
        output = (uintptr_t)&object->signature[0];
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
                                         output,
                                         signSize);

    Eip130Token_Command_Pk_Asset_SetExplicitDigest(&operation.commandToken,
                                                   object->input,
                                                   BITS_TO_BYTES(object->digestLength));
}

/*
 *  ======== HSMLPF3_constructECDHGenPubPhysicalToken ========
 */
void HSMLPF3_constructECDHGenPubPhysicalToken(ECDHLPF3HSM_Object *object)
{
    const uint8_t nWord            = HSM_ASYM_DATA_SIZE_B2W(object->curveLength);
    Eip130TokenDmaAddress_t output = 0U;
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

    if (HSMLPF3_isStandaloneDMASupportEnabled())
    {
        output = HSM_ECDH_GEN_PUB_KEY_ASSET_ID_UPPDER_VALUE | ((Eip130TokenDmaAddress_t)object->publicDataAssetID);
    }
    else
    {
        output = (uintptr_t)&object->output[0];
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
 *  ======== HSMLPF3_constructECDHVerifyKeysPhysicalToken ========
 */
void HSMLPF3_constructECDHVerifyKeysPhysicalToken(ECDHLPF3HSM_Object *object)
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
 *  ======== HSMLPF3_constructECDHGenShrdSecPhysicalToken ========
 */
void HSMLPF3_constructECDHGenShrdSecPhysicalToken(ECDHLPF3HSM_Object *object)
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
 *  ======== HSMLPF3_constructEDDSAGenPubKeyPhysicalToken ========
 */
void HSMLPF3_constructEDDSAGenPubKeyPhysicalToken(EDDSALPF3HSM_Object *object)
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
 *  ======== HSMLPF3_constructEDDSASignInitialPhysicalToken ========
 */
void HSMLPF3_constructEDDSASignInitialPhysicalToken(EDDSALPF3HSM_Object *object)
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
 *  ======== HSMLPF3_constructEDDSAIntermediateHashPhysicalToken ========
 */
void HSMLPF3_constructEDDSAIntermediateHashPhysicalToken(const uint8_t *input, size_t inputLength, uint32_t tempAssetID)
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
 *  ======== HSMLPF3_constructEDDSASignUpdatePhysicalToken ========
 */
void HSMLPF3_constructEDDSASignUpdatePhysicalToken(EDDSALPF3HSM_Object *object)
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
 *  ======== HSMLPF3_constructEDDSASignFinalizePhysicalToken ========
 */
void HSMLPF3_constructEDDSASignFinalizePhysicalToken(EDDSALPF3HSM_Object *object)
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
 *  ======== HSMLPF3_constructEDDSAVerifyInitialPhysicalToken ========
 */
void HSMLPF3_constructEDDSAVerifyInitialPhysicalToken(EDDSALPF3HSM_Object *object)
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
 *  ======== HSMLPF3_constructEDDSAVerifyFinalizePhysicalToken ========
 */
void HSMLPF3_constructEDDSAVerifyFinalizePhysicalToken(EDDSALPF3HSM_Object *object)
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
 *  ======== HSMLPF3_constructAESKey ========
 */
static void HSMLPF3_constructAESKey(uint8_t *key,
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
 *  ======== HSMLPF3_constructGCMToken ========
 */
void HSMLPF3_constructGCMToken(const AESGCMLPF3HSM_Object *object, bool saveIV, bool loadIV)
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
        HSMLPF3_constructAESKey((uint8_t *)&object->KeyStore_keyingMaterial[0],
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
 *  ======== HSMLPF3_constructCCMToken ========
 */
void HSMLPF3_constructCCMToken(const AESCCMLPF3_Object *object, bool saveIV, bool loadIV)
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
        HSMLPF3_constructAESKey((uint8_t *)&object->KeyStore_keyingMaterial[0],
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
 *  ======== HSMLPF3_constructCommonAESECB ========
 */
static void HSMLPF3_constructCommonAESECB(AESECBLPF3_Object *object, uint8_t *key)
{
    bool isEncrypt     = true;
    /* Drivers should verify the key encoding before HSMLPF3 token construction begins,
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
        HSMLPF3_constructAESKey(key, keyLength, object->keyAssetID, object->keyLocation);
    }
#endif
}

/*
 *  ======== HSMLPF3_constructAESECBOneStepPhysicalToken ========
 */
void HSMLPF3_constructAESECBOneStepPhysicalToken(AESECBLPF3_Object *object, uint8_t *key)
{
    Eip130TokenDmaAddress_t input  = (uintptr_t)object->operation->input;
    Eip130TokenDmaAddress_t output = (uintptr_t)object->operation->output;

    uint32_t inputDataLength  = object->operation->inputLength;
    uint32_t outputDataLength = object->operation->inputLength;

    outputDataLength = HSMLPF3_getOutputBufferLength(inputDataLength);

    HSMLPF3_constructCommonAESECB(object, key);

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
static void HSMLPF3_constructCommonAESCTR(AESCTRLPF3_Object *object, uint8_t *key)
{
    uint32_t srcDataLength = object->inputLength;
    bool isEncrypt         = AESCTR_OPERATION_TYPE_ENCRYPT;
    /* Drivers should verify the key encoding before HSMLPF3 token construction begins,
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
        HSMLPF3_constructAESKey(key, keyLength, object->keyAssetID, object->keyLocation);
    }
#endif

    Eip130Token_Command_Crypto_CopyIV(&operation.commandToken, (uint8_t *)&object->counter[0]);
}

/*
 *  ======== HSMLPF3_constructAESCTROneStepPhysicalToken ========
 */
void HSMLPF3_constructAESCTROneStepPhysicalToken(AESCTRLPF3_Object *object, uint8_t *key)
{

    uint32_t inputDataLength  = object->inputLength;
    uint32_t outputDataLength = object->inputLength;

    inputDataLength += AES_BLOCK_SIZE_ALIGN;
    inputDataLength &= (uint32_t)(~AES_BLOCK_SIZE_ALIGN);

    outputDataLength = HSMLPF3_getOutputBufferLength(inputDataLength);

    HSMLPF3_constructCommonAESCTR(object, key);

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
static void HSMLPF3_constructCommonAESCBC(AESCBCLPF3_Object *object, uint8_t *key)
{
    uint32_t srcDataLength = object->inputLength;
    bool isEncrypt         = true;
    /* Drivers should verify the key encoding before HSMLPF3 token construction begins,
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
        HSMLPF3_constructAESKey(key, keyLength, object->keyAssetID, object->keyLocation);
    }
#endif

    Eip130Token_Command_Crypto_CopyIV(&operation.commandToken, (uint8_t *)&object->iv[0]);
}

/*
 *  ======== HSMLPF3_constructAESCBCOneStepPhysicalToken ========
 */
void HSMLPF3_constructAESCBCOneStepPhysicalToken(AESCBCLPF3_Object *object, uint8_t *key)
{
    uint32_t inputDataLength  = object->inputLength;
    uint32_t outputDataLength = object->inputLength;

    outputDataLength = HSMLPF3_getOutputBufferLength(inputDataLength);

    HSMLPF3_constructCommonAESCBC(object, key);

    /* Set data addresses */
    Eip130Token_Command_Crypto_SetDataAddresses(&operation.commandToken,
                                                (uintptr_t)object->input,
                                                inputDataLength,
                                                (uintptr_t)object->output,
                                                outputDataLength);
}

/*
 *  ======== HSMLPF3_constructCMACToken ========
 */
void HSMLPF3_constructCMACToken(AESCMACLPF3_Object *object, bool isFirst, bool isFinal)
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
 *  ======== HSMLPF3_constructRNGSwitchNRBGWithDefaultsPhysicalToken ========
 */
void HSMLPF3_constructRNGSwitchNRBGWithDefaultsPhysicalToken(HSMLPF3_NRBGMode HSMLPF3_nrbgMode)
{
    operation.commandToken.W[0] = RNG_CONFIG_TOKEN_WORD0;
    operation.commandToken.W[2] = RNG_CONFIG_TOKEN_WORD2;

    if (HSMLPF3_nrbgMode == HSMLPF3_MODE_CRNG)
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
 *  ======== HSMLPF3_constructRNGReseedDRBGPhysicalToken ========
 */
void HSMLPF3_constructRNGReseedDRBGPhysicalToken(void)
{
    Eip130Token_Command_PRNG_ReseedNow(&operation.commandToken);
}
#endif /* (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX) */

/*
 *  ======== HSMLPF3_constructRNGGetRandomNumberPhysicalToken ========
 */
void HSMLPF3_constructRNGGetRandomNumberPhysicalToken(uintptr_t entropyBuffer, size_t entropyRequested)
{
    Eip130Token_Command_RandomNumber_Generate(&operation.commandToken, entropyRequested, entropyBuffer);
}

/*
 *  ======== HSMLPF3_constructRNGGetRawRandomNumberPhysicalToken ========
 */
void HSMLPF3_constructRNGGetRawRandomNumberPhysicalToken(uintptr_t entropyBuffer, size_t entropyRequested)
{
    uint16_t rawKey = HSM_CRNG_RAW_KEY_ENC;

    if (HSMLPF3_nrbgMode == HSMLPF3_MODE_TRNG)
    {
        rawKey = HSM_TRNG_RAW_KEY_ENC;
    }

    Eip130Token_Command_RandomNumber_Generate(&operation.commandToken, entropyRequested, entropyBuffer);

    Eip130Token_Command_RandomNumber_SetRawKey(&operation.commandToken, rawKey);
}
