/*
 * Copyright (c) 2024-2025, Texas Instruments Incorporated. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "config_tfm.h"
#include "tfm_attest_hal.h"
#include "tfm_plat_boot_seed.h"
#include "tfm_plat_device_id.h"
#include "tfm_strnlen.h"
#include "utilities.h"

/* TI CC27xx SDK include(s) */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_dbgss.h)
#include DeviceFamily_constructPath(inc/hw_dcb.h)
#include DeviceFamily_constructPath(inc/hw_fcfg.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_scfg.h)
#include DeviceFamily_constructPath(inc/hw_types.h)     /* HWREG() for verified_reg_write() */

#define CCFG_FIELD(f) (((volatile const ccfg_t *)CCFG_BASE)->f)
#define SCFG_FIELD(f) (((volatile const scfg_t *)SCFG_BASE)->f)

#define SCFG_BOOT_SEED_MAX_OFFSET 13
#define CBOR_HEADER_SIZE          8

#if ATTEST_TOKEN_PROFILE_PSA_2_0_0
    #error "ATTEST_TOKEN_PROFILE_PSA_2_0_0 is not supported on this platform"
#elif ATTEST_TOKEN_PROFILE_ARM_CCA
    #error "ATTEST_TOKEN_PROFILE_ARM_CCA is not supported on this platform"
#endif

/* Verification service URL for initial attestation token */
static const char verification_service_url[] = "www.psacertified.org";

/* Profile definition document for initial attestation token */
static const char attestation_profile_definition[] = "PSA_IOT_PROFILE_1";

#if ATTEST_INCLUDE_OPTIONAL_CLAIMS
/* Certification reference for "PSA_IOT_PROFILE_1" is EAN-13 (format [0-9]{13}) per
 * https://www.ietf.org/archive/id/draft-tschofenig-rats-psa-token-07.html#name-certification-reference;
 * however, www.psacertified.org is issuing certification numbers with format '[0-9]{13}-[0-9]{5}'.
 * which matches "http://arm.com/psa/2.0.0" profile per
 * https://www.ietf.org/archive/id/draft-tschofenig-rats-psa-token-09.html#name-certification-reference
 * The latter format is used although it does not adhere to the spec for PSA_IOT_PROFILE_1
 * since the value is provided by the certification authority.
 */
#warning "cert_ref must be updated with certification number of format '[0-9]{13}-[0-9]{5}' provided by the certification authority"

static const char cert_ref[CERTIFICATION_REF_MAX_SIZE] = "0011223344556-77889";
#endif /* ATTEST_INCLUDE_OPTIONAL_CLAIMS */

enum tfm_security_lifecycle_t tfm_attest_hal_get_security_lifecycle(void)
{
    enum tfm_security_lifecycle_t slc = TFM_SLC_UNKNOWN;
    uint32_t ccfg_debug_cfg;
    uint32_t dbgss_app_auth;

    /* The only valid Security Life Cycles (SLCs) to report via attestation are:
     * - "Secured"
     * - "Non-PRoT Debug"
     * - "PRoT Debug"
     *
     * The manufacturer must ensure that the device has debug authorization
     * enabled before shipping the device to the customer since secure
     * flash could be read or written while in secure debug state which would
     * compromise sensitive data and render it untrustworthy after debug.
     *
     * The TF-M will fail to boot if PRoT provisioning fails or the device is
     * decommissioned. Thus, it is not possible to request attestation while
     * in any of the following SLCs:
     * - "Device Assembly & Test"
     * - "PRoT Provisioning"
     * - "Decommissioned"
     */

    /* Secure boot must be enabled in order to report a valid SLC */
    if ((SCFG_FIELD(secBootCfg.policyCfg.authMethod) == SCFG_POLICY_SIGNATURE) ||
        (SCFG_FIELD(secBootCfg.policyCfg.authMethod) == SCFG_POLICY_HASH_LOCK))
    {
        ccfg_debug_cfg = CCFG_FIELD(debugCfg.authorization);

        if (ccfg_debug_cfg == CCFG_DBGAUTH_DBGOPEN)
        {
            slc = TFM_SLC_UNKNOWN;
        }
        else if (ccfg_debug_cfg == CCFG_DBGAUTH_DBGFORBID)
        {
            slc = TFM_SLC_SECURED;
        }
        else if (ccfg_debug_cfg == CCFG_DBGAUTH_ONLY_NON_INVASIVE)
        {
            slc = TFM_SLC_SECURED;
        }
        else if (ccfg_debug_cfg == CCFG_DBGAUTH_REQAUTH)
        {
            /* Read the current debug authorization privileges */
            dbgss_app_auth = HWREG(DBGSS_BASE + DBGSS_O_APP_AUTH);

            /* If all debug authorization privileges are disabled, SLC is "Secured" */
            if (dbgss_app_auth == 0)
            {
                slc = TFM_SLC_SECURED;
            }
            else if (dbgss_app_auth == DBGSS_APP_AUTH_NIDEN_EN)
            {
                /* If debug authorization is configured for non-invasive debug enable,
                * SLC is "Non-PRoT Debug".
                */
                slc = TFM_SLC_NON_PSA_ROT_DEBUG;
            }
            else if (dbgss_app_auth == (DBGSS_APP_AUTH_NIDEN_EN | DBGSS_APP_AUTH_DBGEN_EN))
            {
                /* If debug authorization is configured for invasive non-secure
                * debug enable, SLC is "Non-PRoT Debug".
                */
                slc = TFM_SLC_NON_PSA_ROT_DEBUG;
            }
            else if (dbgss_app_auth == (DBGSS_APP_AUTH_SPNIDEN_EN | DBGSS_APP_AUTH_SPIDEN_EN |
                                        DBGSS_APP_AUTH_NIDEN_EN | DBGSS_APP_AUTH_DBGEN_EN))
            {
                /* If debug authorization is configured for invasive secure debug,
                * the SLC is "PRoT Debug". This lifecycle is considered recoverable
                * since it shall be protected using public key based authentication.
                * Only trusted users will have access to the required private key
                * needed to successfully complete the challenge response sequence
                * to enter secure debug.
                */
                slc = TFM_SLC_RECOVERABLE_PSA_ROT_DEBUG;
            }
            else
            {
                slc = TFM_SLC_UNKNOWN;
            }
        }
        else
        {
            slc = TFM_SLC_UNKNOWN;
        }
    }
    else
    {
        slc = TFM_SLC_UNKNOWN;
    }

    return slc;
}

enum tfm_plat_err_t
tfm_attest_hal_get_verification_service(uint32_t *size, uint8_t *buf)
{
    size_t copy_size;

    /* String length excluding null terminator up to provided size */
    copy_size = tfm_strnlen(verification_service_url, *size);

    (void)spm_memcpy(buf, verification_service_url, copy_size);

    *size = copy_size;

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t
tfm_attest_hal_get_profile_definition(uint32_t *size, uint8_t *buf)
{
    size_t copy_size;

    /* String length excluding null terminator up to provided size */
    copy_size = tfm_strnlen(attestation_profile_definition, *size);

    (void)spm_memcpy(buf, attestation_profile_definition, copy_size);

    *size = copy_size;

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_get_boot_seed(uint32_t size, uint8_t *buf)
{
    uint8_t bootSeedOffset;
    enum tfm_plat_err_t err = TFM_PLAT_ERR_UNSUPPORTED;

    /* ROM code writes a random 32-byte boot seed prefixed with a 8-byte CBOR
     * header to: Secure RAM start addr + (scfg.bootSeedOffset * 16).
     */
    bootSeedOffset = SCFG_FIELD(bootSeedOffset);

    if (bootSeedOffset <= SCFG_BOOT_SEED_MAX_OFFSET)
    {
        /* Offset is in multiples of 16-bytes */
        bootSeedOffset *= 16;
        /* Skip CBOR header */
        bootSeedOffset += CBOR_HEADER_SIZE;

        (void)spm_memcpy(buf, (void *)(SRAM_S_BASE + bootSeedOffset), size);

        err = TFM_PLAT_ERR_SUCCESS;
    }
    else
    {
        /* Secure boot is not enabled. This is not a valid production
         * configuration but for testing purposes, return zeros for the boot
         * seed.
         */
        (void)spm_memcpy(buf, 0, size);

        err = TFM_PLAT_ERR_SUCCESS;
    }

    return err;
}

enum tfm_plat_err_t tfm_plat_get_implementation_id(uint32_t *size,
                                                   uint8_t  *buf)
{
    size_t copy_size;
    uint8_t id[32] = {0};

    /* FCFG data will be used to generate the 32-byte Implementation ID:
     *  - Die ID (FCFG.deviceInfo.dieId) will be first 16-bytes
     *  - MAC BLE (FCFG.deviceInfo.bleAddr) will be next 8-bytes
     *  - MAC 15.4 (FCFG.deviceInfo.macAddr) will be last 8-bytes
     */
    (void)spm_memcpy(&id[0], &fcfg->deviceInfo.dieId[0], 16);
    (void)spm_memcpy(&id[16], &fcfg->deviceInfo.bleAddr[0], 8);
    (void)spm_memcpy(&id[24], &fcfg->deviceInfo.macAddr[0], 8);

    /* Copy size is the smaller of provided size or actual size */
    copy_size = *size < sizeof(id) ? *size : sizeof(id);

    (void)spm_memcpy(buf, id, copy_size);

    *size = copy_size;

    return TFM_PLAT_ERR_SUCCESS;
}

#if ATTEST_INCLUDE_OPTIONAL_CLAIMS

enum tfm_plat_err_t tfm_plat_get_cert_ref(uint32_t *size, uint8_t *buf)
{
    size_t copy_size;

    /* String length excluding null terminator up to provided size */
    copy_size = tfm_strnlen(cert_ref, *size);

    (void)spm_memcpy(buf, cert_ref, copy_size);

    *size = copy_size;

    return TFM_PLAT_ERR_SUCCESS;
}

#endif /* ATTEST_INCLUDE_OPTIONAL_CLAIMS */
