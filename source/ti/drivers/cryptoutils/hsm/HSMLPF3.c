/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

#include "HSMLPF3.h"
#include <ti/devices/DeviceFamily.h>

#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_types.h)

#include DeviceFamily_constructPath(inc/hw_vims.h)
#include DeviceFamily_constructPath(inc/hw_clkctl.h)
#include DeviceFamily_constructPath(inc/hw_hsm.h)
#include DeviceFamily_constructPath(inc/hw_hsmcrypto.h)
#include DeviceFamily_constructPath(inc/hw_tcm.h)

/* Defines and enumerations */
#define BOOT_DELAY 0xFFFFF

#define SLEEP_TOKEN_WORD0 0x5F000000

#define BOOT_TOKEN_WORD0 0xCF000000

#define CRYPTO_OFFICER_ID 0x03725746

#define OUTPUT_TOKEN_ERROR 0x80000000

#define CLKCTL_O_HSMCFG 0x0000004CU

#define CLKCTL_HSMCFG_POST_M 0x00000001U

#define CLKCTL_HSMCFG_POST_DIS 0x00000001U

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
 *  ======== HSMLPF3_enableClock ========
 */
void HSMLPF3_enableClock(void)
{
    uint32_t temp                 = 0;
    /* Disable VIMS lock, allocate 3 32KB blocks to HSM */
    temp                          = HWREG(VIMS_BASE + VIMS_O_CFG);
    HWREG(VIMS_BASE + VIMS_O_CFG) = (temp & (~VIMS_CFG_HSMSZ_M)) | (VIMS_CFG_HSMSZ_SIZE_96) | VIMS_CFG_LOCK_DIS;

    /* Set the clock, and wait for it to enable */
    HWREG(CLKCTL_BASE + CLKCTL_O_CLKENSET1) = CLKCTL_CLKENSET1_HSM_CLK_SET;
    while ((HWREG(CLKCTL_BASE + CLKCTL_O_CLKCFG1) & CLKCTL_CLKCFG1_HSM_M) == CLKCTL_CLKCFG1_HSM_CLK_DIS) {}

    /* Change CPU ID and unlock the CPUID 0 and 1 for both mailboxes */

    /* Change CPU ID to ROM */
    HWREG(HSM_BASE + HSM_O_CTL) = HSM_CTL_CPUIDSEL_ROMID | HSM_CTL_CPUIDUNLK_UNLOCK;

    /* Read back to ensure previous write went through */
    HWREG(HSM_BASE + HSM_O_CTL);

    /* Unlock CPUID0 and CPUID1 */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXLOCKOUT) = 0xFFFFFCFC;

    /* Change CPU ID to APP */
    HWREG(HSM_BASE + HSM_O_CTL) = HSM_CTL_CPUIDSEL_APPID | HSM_CTL_CPUIDUNLK_UNLOCK;

    /* Lock the CPU ID TO APP */
    HWREG(HSM_BASE + HSM_O_CTL) = HSM_CTL_CPUIDUNLK_LOCK;

    /* Disable DMA firewall */
    HWREG(HSM_BASE + HSM_O_CTL) |= HSM_CTL_DMAFWDIS_DIS;

    /* Check if POST is disabled */
    if ((HWREG(CLKCTL_BASE + CLKCTL_O_HSMCFG) & CLKCTL_HSMCFG_POST_M) == CLKCTL_HSMCFG_POST_DIS)
    {
        /* Nothing to check, as ROM FW check is skipped */
    }
    else
    {
        /* Check ROM FW CRC check is completed */
        while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODULESTATUS) & HSMCRYPTO_MODULESTATUS_CRC24OK) !=
                HSMCRYPTO_MODULESTATUS_CRC24OK)
        {}
    }
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
 *  ======== HSMLPF3_boot ========
 */
int_fast16_t HSMLPF3_boot(void)
{
    uint32_t token[2];
    uint32_t result;
    uint32_t delay;

    token[0] = BOOT_TOKEN_WORD0;
    token[1] = CRYPTO_OFFICER_ID;

    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODULESTATUS) & HSMCRYPTO_MODULESTATUS_FATALERROR) ==
         HSMCRYPTO_MODULESTATUS_FATALERROR)
    {
        return HSMLPF3_STATUS_ERROR;
    }

    result = HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODULESTATUS);

    /* If HSM is already booted*/
    if ((result & HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED) == HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED)
    {
        return HSMLPF3_STATUS_SUCCESS;
    }

    HSMLPF3_writeToken(token, sizeof(token) / sizeof(uint32_t));

    /* Wait for result in mbx1_out */
    while ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXSTAT) & HSMCRYPTO_MBXSTAT_MBX1OUTFULL) !=
            HSMCRYPTO_MBXSTAT_MBX1OUTFULL)
    {}

    if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBX1OUT) & OUTPUT_TOKEN_ERROR) != 0)
    {
        HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXCTRL) = HSMCRYPTO_MBXCTRL_MBX1OUTEMPTY;
        return HSMLPF3_STATUS_ERROR;
    }

    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXCTRL) = HSMCRYPTO_MBXCTRL_MBX1OUTEMPTY;

    for (delay = BOOT_DELAY; delay; delay--)
    {
        if ((HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MODULESTATUS) & HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED) ==
             HSMCRYPTO_MODULESTATUS_FWIMGACCEPTED)
        {
            return HSMLPF3_STATUS_SUCCESS;
        }
    }

    return HSMLPF3_STATUS_ERROR;
}

/*
 *  ======== HSMLPF3_initMbox ========
 */
void HSMLPF3_initMbox(void)
{
    /* Link mailbox */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXCTRL) = (HSMCRYPTO_MBXCTRL_MBX1LINK |
                                                   HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXSTAT));

    /* Allow non-secure/secure access (Set bits 7 and 3 to 1 if we need secure access) */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXLINKID) = 0x00;

    /* Make sure CPU_ID=0 host can access mailbox 1 & 2 (no lockout) */
    HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXLOCKOUT) = 0xFFFFFF77 & HWREG(HSMCRYPTO_BASE + HSMCRYPTO_O_MBXLOCKOUT);
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
