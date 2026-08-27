/*
 * Copyright (c) 2025, Texas Instruments Incorporated - http://www.ti.com
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

#include <stdbool.h>
#include <stdint.h>

#include "PSACrypto_s.h"

#include <third_party/tfm/secure_fw/partitions/crypto/tfm_crypto_api.h>
#include <third_party/tfm/secure_fw/spm/core/spm.h>
#include <third_party/tfm/interface/include/psa/error.h>
#include <third_party/tfm/interface/include/psa/service.h>

#include <third_party/tfm/platform/ext/target/ti/cc27xx/cmse.h> /* TI CMSE helper functions */

#include <third_party/tfm/platform/ext/target/ti/cc27xx/tfm_builtin_key_ids.h>

#include <psa_manifest/crypto_sp.h> /* Auto-generated header */

#ifdef TI_CRYPTO_ITS_INTEGRATION
    #include <third_party/tfm/secure_fw/partitions/internal_trusted_storage/tfm_internal_trusted_storage.h>
#endif

#include "tfm_mbedcrypto_include.h" /* Must be included before psa/crypto.h */
/* PSA Crypto header file */
#include <third_party/mbedtls/include/psa/crypto.h>
#include <ti/drivers/psa/ti_psa_crypto.h>
#include <ti/drivers/psa/ti_psa_crypto_helper.h>

#include <ti/drivers/AESCBC.h>
#include <ti/drivers/AESCCM.h>
#include <ti/drivers/AESCMAC.h>
#include <ti/drivers/AESCTR.h>
#include <ti/drivers/AESCTRDRBG.h>
#include <ti/drivers/AESECB.h>
#include <ti/drivers/AESGCM.h>
#include <ti/drivers/ECDH.h>
#include <ti/drivers/ECDSA.h>
#include <ti/drivers/EDDSA.h>
#include <ti/drivers/SHA2.h>

#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyKeyStore_PSA_helpers.h>
#include <ti/drivers/cryptoutils/hsm/HSMXXF3.h>

#include <ti/drivers/dpl/HwiP.h>

#include <ti/drivers/tfm/SecureCallback_s.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_ints.h)

#ifdef ENABLE_ITS_IPC_INTEGRATION
    #include "CryptoTFM_ITS_s.c"
#endif

#include <psa/crypto.h>

static psa_msg_t msg;

/* Stores pointer to non-secure callback */
PSACrypto_s_SecureCallback *PSACrypto_s_secureCB;

/* Externs */
extern psa_status_t tfm_crypto_call_srv(const psa_msg_t *msg);
extern psa_status_t tfm_crypto_init_alloc(void);
extern void psa_setup_return_behavior(bool ns_caller);
extern void SystemCC27XX_init(void);

/*
 *  ======== PSACrypto_s_aesCallbackHandler ========
 */
static void PSACrypto_s_aesCallbackHandler(int_fast16_t returnValue)
{
    /* Map return value to PSA status in NS callback object */
    PSACrypto_s_secureCB->status = map_AES_status(returnValue);

    if (PSACrypto_s_secureCB->tfmCryptoOperationHandle != NULL)
    {
        /* Release the secure operation context */
        (void)tfm_crypto_operation_release((uint32_t *)PSACrypto_s_secureCB->tfmCryptoOperationHandle);
        PSACrypto_s_secureCB->tfmCryptoOperationHandle = NULL;
    }

    /* Trigger the interrupt for the non-secure callback dispatcher */
    SecureCallback_s_post(&PSACrypto_s_secureCB->object);
}

/*
 *  ======== PSACrypto_s_aescbcHwiCallback ========
 */
void PSACrypto_s_aescbcHwiCallback(AESCBC_Handle handle,
                                   int_fast16_t returnValue,
                                   AESCBC_OperationUnion *operation,
                                   AESCBC_OperationType operationType)
{
    if (((AESCBCXXF3_Object *)(handle->object))->common.isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        AESCBC_close(handle);
    }
    PSACrypto_s_aesCallbackHandler(returnValue);
}

/*
 *  ======== PSACrypto_s_aesccmHwiCallback ========
 */
void PSACrypto_s_aesccmHwiCallback(AESCCM_Handle handle,
                                   int_fast16_t returnValue,
                                   AESCCM_OperationUnion *operation,
                                   AESCCM_OperationType operationType)
{
    if (((AESCCMXXF3_Object *)(handle->object))->common.isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        AESCCM_close(handle);
    }
    PSACrypto_s_aesCallbackHandler(returnValue);
}

/*
 *  ======== PSACrypto_s_aescmacHwiCallback ========
 */
void PSACrypto_s_aescmacHwiCallback(AESCMAC_Handle handle,
                                    int_fast16_t returnValue,
                                    AESCMAC_Operation *operation,
                                    AESCMAC_OperationType operationType)
{
    if (((AESCMACXXF3_Object *)(handle->object))->common.isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        AESCMAC_close(handle);
    }
    PSACrypto_s_aesCallbackHandler(returnValue);
}

/*
 *  ======== PSACrypto_s_aescbcmacHwiCallback ========
 */
void PSACrypto_s_aescbcmacHwiCallback(AESCMAC_Handle handle,
                                      int_fast16_t returnValue,
                                      AESCMAC_Operation *operation,
                                      AESCMAC_OperationType operationType)
{
    if (((AESCMACXXF3_Object *)(handle->object))->common.isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        AESCMAC_close(handle);
    }
    PSACrypto_s_aesCallbackHandler(returnValue);
}

/*
 *  ======== PSACrypto_s_aesctrHwiCallback ========
 */
void PSACrypto_s_aesctrHwiCallback(AESCTR_Handle handle,
                                   int_fast16_t returnValue,
                                   AESCTR_OperationUnion *operation,
                                   AESCTR_OperationType operationType)
{
    if (((AESCTRXXF3_Object *)(handle->object))->common.isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        AESCTR_close(handle);
    }
    PSACrypto_s_aesCallbackHandler(returnValue);
}

/*
 *  ======== PSACrypto_s_aesecbHwiCallback ========
 */
void PSACrypto_s_aesecbHwiCallback(AESECB_Handle handle,
                                   int_fast16_t returnValue,
                                   AESECB_Operation *operation,
                                   AESECB_OperationType operationType)
{
    if (((AESECBXXF3_Object *)(handle->object))->common.isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        AESECB_close(handle);
    }
    PSACrypto_s_aesCallbackHandler(returnValue);
}

/*
 *  ======== PSACrypto_s_aesgcmHwiCallback ========
 */
void PSACrypto_s_aesgcmHwiCallback(AESGCM_Handle handle,
                                   int_fast16_t returnValue,
                                   AESGCM_OperationUnion *operation,
                                   AESGCM_OperationType operationType)
{
    if (((AESGCMXXF3HSM_Object *)(handle->object))->common.isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        AESGCM_close(handle);
    }
    PSACrypto_s_aesCallbackHandler(returnValue);
}

/*
 *  ======== PSACrypto_s_ecdhHwiCallback ========
 */
void PSACrypto_s_ecdhHwiCallback(ECDH_Handle handle,
                                 int_fast16_t returnStatus,
                                 ECDH_Operation operation,
                                 ECDH_OperationType operationType)
{
    /* Map return value to PSA status in NS callback object */
    PSACrypto_s_secureCB->status = map_ECDH_status(returnStatus);

    if (PSACrypto_s_secureCB->tfmCryptoOperationHandle != NULL)
    {
        /* Release the secure operation context */
        (void)tfm_crypto_operation_release((uint32_t *)PSACrypto_s_secureCB->tfmCryptoOperationHandle);
        PSACrypto_s_secureCB->tfmCryptoOperationHandle = NULL;
    }

    if (((ECDHXXF3HSM_Object *)(handle->object))->isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        ECDH_close(handle);
    }

    /* Trigger the interrupt for the non-secure callback dispatcher */
    SecureCallback_s_post(&PSACrypto_s_secureCB->object);
}

/*
 *  ======== PSACrypto_s_ecdsaHwiCallback ========
 */
void PSACrypto_s_ecdsaHwiCallback(ECDSA_Handle handle,
                                  int_fast16_t returnStatus,
                                  ECDSA_Operation operation,
                                  ECDSA_OperationType operationType)
{
    bool isVerifyOp = false;

    if (operationType == ECDSA_OPERATION_TYPE_VERIFY)
    {
        isVerifyOp = true;
    }

    /* Map return value to PSA status in NS callback object */
    PSACrypto_s_secureCB->status = map_ECDSA_status(returnStatus, isVerifyOp);

    if (PSACrypto_s_secureCB->tfmCryptoOperationHandle != NULL)
    {
        /* Release the secure operation context */
        (void)tfm_crypto_operation_release((uint32_t *)PSACrypto_s_secureCB->tfmCryptoOperationHandle);
        PSACrypto_s_secureCB->tfmCryptoOperationHandle = NULL;
    }

    if (((ECDSAXXF3HSM_Object *)(handle->object))->isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        ECDSA_close(handle);
    }

    /* Trigger the interrupt for the non-secure callback dispatcher */
    SecureCallback_s_post(&PSACrypto_s_secureCB->object);
}

/*
 *  ======== PSACrypto_s_eddsaHwiCallback ========
 */
void PSACrypto_s_eddsaHwiCallback(EDDSA_Handle handle,
                                  int_fast16_t returnStatus,
                                  EDDSA_Operation operation,
                                  EDDSA_OperationType operationType)
{
    bool isVerifyOp = false;

    if (operationType == EDDSA_OPERATION_TYPE_VERIFY)
    {
        isVerifyOp = true;
    }

    /* Map return value to PSA status in NS callback object */
    PSACrypto_s_secureCB->status = map_EDDSA_status(returnStatus, isVerifyOp);

    if (PSACrypto_s_secureCB->tfmCryptoOperationHandle != NULL)
    {
        /* Release the secure operation context */
        (void)tfm_crypto_operation_release((uint32_t *)PSACrypto_s_secureCB->tfmCryptoOperationHandle);
        PSACrypto_s_secureCB->tfmCryptoOperationHandle = NULL;
    }

    if (((EDDSAXXF3HSM_Object *)(handle->object))->isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        EDDSA_close(handle);
    }

    /* Trigger the interrupt for the non-secure callback dispatcher */
    SecureCallback_s_post(&PSACrypto_s_secureCB->object);
}

/*
 *  ======== PSACrypto_s_sha2HwiCallback ========
 */
void PSACrypto_s_sha2HwiCallback(SHA2_Handle handle, int_fast16_t returnStatus)
{
    /* Map return value to PSA status in NS callback object */
    PSACrypto_s_secureCB->status = map_SHA2_status(returnStatus);

    if (PSACrypto_s_secureCB->tfmCryptoOperationHandle != NULL)
    {
        /* Release the secure operation context */
        (void)tfm_crypto_operation_release((uint32_t *)PSACrypto_s_secureCB->tfmCryptoOperationHandle);
        PSACrypto_s_secureCB->tfmCryptoOperationHandle = NULL;
    }

    if (((SHA2XXF3HSM_Object *)(handle->object))->isOpen == false)
    {
        /* Close the driver handle - after any one-shot or final segment operation is finished,
         * the ti_psa layer will set isOpen to false to indicate that the secure callback
         * can do this. This is necessary because segmented operations must be 'setup'
         * again after 'finish', and each 'setup' call constructs a new driver handle.
         */
        SHA2_close(handle);
    }

    /* Trigger the interrupt for the non-secure callback dispatcher */
    SecureCallback_s_post(&PSACrypto_s_secureCB->object);
}

/*
 *  ======== PSACrypto_s_handleRegisterCallback ========
 */
static psa_status_t PSACrypto_s_handleRegisterCallback(psa_msg_t *msg)
{
    psa_status_t status = PSA_ERROR_PROGRAMMER_ERROR;
    size_t bytesCopied;

    /* Only non-secure callers should be registering callbacks */
    if (TFM_CLIENT_ID_IS_NS(msg->client_id) && (msg->in_size[0] == sizeof(PSACrypto_s_secureCB)))
    {
        /* Read the pointer to the NS callback struct */
        bytesCopied = psa_read(msg->handle, 0, &PSACrypto_s_secureCB, sizeof(PSACrypto_s_secureCB));

        if (bytesCopied == sizeof(PSACrypto_s_secureCB))
        {
            status = PSA_SUCCESS;
        }

        /* Verify the callback struct is in NS memory */
        if (cmse_has_unpriv_nonsecure_rw_access(PSACrypto_s_secureCB, sizeof(PSACrypto_s_SecureCallback)) == NULL)
        {
            PSACrypto_s_secureCB = NULL;
            status               = PSA_ERROR_PROGRAMMER_ERROR;
        }
    }

    return status;
}

/*
 *  ======== generateIAK ========
 */
psa_status_t generateIAK(void)
{
    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t outputKeyID;

    /* Configure key attributes for the IAK */
    psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
    psa_set_key_bits(&attributes, 256);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH);
    psa_set_key_id(&attributes, TFM_BUILTIN_KEY_ID_IAK);
    psa_set_key_lifetime(&attributes,
                         PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_DEFAULT,
                                                                        PSA_KEY_LOCATION_LOCAL_STORAGE));

    /* Generate the private key */
    status = ti_psa_generate_key(&attributes, &outputKeyID);

    return status;
}

/*
 *  ======== Crypto_sp_main ========
 *  Crypto Secure Partition entry point
 */
void Crypto_sp_main(void)
{
    psa_key_attributes_t keyAttributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_status_t status;
    uint32_t signals;

    /* Initialize system including power driver */
    SystemCC27XX_init();

    /* Initialize secure contexts for multi-part operations */
    if (tfm_crypto_init_alloc() != PSA_SUCCESS)
    {
        psa_panic();
    }

    /* Initialize Key Store which will handle initialization of ITS */
    if (KeyStore_PSA_init() != KEYSTORE_PSA_STATUS_SUCCESS)
    {
        psa_panic();
    }

    /* Initialize PSA Crypto which will handle initialization of underlying
     * HSM and SimpleLink Crypto drivers.
     */
    if (psa_crypto_init() != PSA_SUCCESS)
    {
        psa_panic();
    }

    /* Check for existence of Initial Attestation Key (IAK) */
    status = ti_psa_get_key_attributes(TFM_BUILTIN_KEY_ID_IAK, &keyAttributes);
    if (status != PSA_SUCCESS)
    {
        /* IAK does not exist so we must generate it */
        if (generateIAK() != PSA_SUCCESS)
        {
            psa_panic();
        }
    }

    /* Enable external interrupt for Power driver.
     *
     * Note: SystemCC27XX_init() -> Power_init() enables the CPUIRQ3 interrupt
     * so the following call to psa_irq_enable is superfluous but left for
     * consistency.
     */
    psa_irq_enable(CPUIRQ3_IRQn_SIGNAL);

    /* Enable external interrupts for HSM.
     *
     * Note: HSM_OTP_IRQn_SIGNAL is purposely left disabled since HUK
     * provisioning enables the OTP interrupt event but there is no
     * OTP interrupt handling required.
     */
    psa_irq_enable(HSM_Secure_IRQn_SIGNAL);
    psa_irq_disable(HSM_OTP_IRQn_SIGNAL);

    while (1)
    {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);

        if (signals & TI_CRYPTO_SERVICE_SIGNAL)
        {
            psa_get(TI_CRYPTO_SERVICE_SIGNAL, &msg);

            switch (msg.type)
            {
                case PSA_IPC_CALL:
                    psa_setup_return_behavior(TFM_CLIENT_ID_IS_NS(msg.client_id));
                    psa_reply(msg.handle, tfm_crypto_call_srv(&msg));
                    break;

                case PSA_CRYPTO_REGISTER_SECURE_CALLBACK_TYPE:
                    psa_reply(msg.handle, PSACrypto_s_handleRegisterCallback(&msg));
                    break;

                default:
                    psa_reply(msg.handle, PSA_ERROR_PROGRAMMER_ERROR);
                    break;
            }
        }
#ifdef ENABLE_ITS_IPC_INTEGRATION
        else if (signals & TFM_ITS_SET_SIGNAL)
        {
            its_signal_handle(TFM_ITS_SET_SIGNAL, tfm_its_set_ipc);
        }
        else if (signals & TFM_ITS_GET_SIGNAL)
        {
            its_signal_handle(TFM_ITS_GET_SIGNAL, tfm_its_get_ipc);
        }
        else if (signals & TFM_ITS_GET_INFO_SIGNAL)
        {
            its_signal_handle(TFM_ITS_GET_INFO_SIGNAL, tfm_its_get_info_ipc);
        }
        else if (signals & TFM_ITS_REMOVE_SIGNAL)
        {
            its_signal_handle(TFM_ITS_REMOVE_SIGNAL, tfm_its_remove_ipc);
        }
#endif /* ENABLE_ITS_IPC_INTEGRATION */
        else
        {
            psa_panic();
        }
    }

    return;
}
