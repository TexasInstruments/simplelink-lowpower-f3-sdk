/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated
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

#ifndef ZB_OSIF_OTA_STUB_H
#define ZB_OSIF_OTA_STUB_H 1

zb_bool_t zb_osif_ota_open_storage(void);
zb_bool_t zb_osif_ota_fw_size_ok(zb_uint32_t image_size);
zb_uint32_t zb_osif_ota_get_erase_portion(void);
void zb_osif_ota_erase_fw(void *dev, zb_uint_t offset, zb_uint32_t size);
zb_uint8_t zb_osif_ota_write(void *dev, zb_uint8_t *data, zb_uint_t off, zb_uint_t size, zb_uint32_t image_size);
void zb_osif_ota_mark_fw_ready(void *dev, zb_uint32_t size, zb_uint32_t revision);
void zb_osif_ota_mark_fw_absent(void);
void zb_osif_ota_mark_fw_updated(void);
void zb_osif_ota_close_storage(void *dev);
zb_bool_t zb_osif_ota_verify_integrity(void *dev, zb_uint32_t raw_len);

/* WARNING: Works with absolute address! */
void zb_osif_ota_read(void *dev, zb_uint8_t *data, zb_uint32_t addr, zb_uint32_t size);

zb_bool_t zb_osif_ota_verify_integrity_async(void *dev, zb_uint32_t raw_len);
zb_uint8_t zb_erase_fw(zb_uint32_t address, zb_uint32_t pages_count);
zb_uint8_t zb_write_fw(zb_uint32_t address, zb_uint8_t *buf, zb_uint16_t len);
void Hash16_Calc(zb_uint32_t pBuffer, zb_uint32_t BufferLength, zb_uint8_t *hash16);

extern const zb_uint32_t _MCUBOOT_MAX_SLOT_SIZE;
extern const zb_uint32_t _PRIMARY_SLOT_BASE;
extern const zb_uint32_t _SECONDARY_SLOT_BASE;

// OTA Client write states
#define OTA_CLIENT_HDR_MAGIC_0_STATE        0
#define OTA_CLIENT_HDR_MAGIC_1_STATE        1
#define OTA_CLIENT_HDR_MAGIC_2_STATE        2
#define OTA_CLIENT_HDR_MAGIC_3_STATE        3
#define OTA_CLIENT_HDR_LEN_1_STATE          4
#define OTA_CLIENT_HDR_LEN_2_STATE          5
#define OTA_CLIENT_SUB_ELEM_TAG_1_STATE     6
#define OTA_CLIENT_SUB_ELEM_TAG_2_STATE     7
#define OTA_CLIENT_SUB_ELEM_LEN_1_STATE     8
#define OTA_CLIENT_SUB_ELEM_LEN_2_STATE     9
#define OTA_CLIENT_SUB_ELEM_LEN_3_STATE     10
#define OTA_CLIENT_SUB_ELEM_LEN_4_STATE     11

// OTA Sub Element Tags
#define OTA_SUB_ELEM_TAG_UPDATE_IMAGE               0x0000
#define OTA_SUB_ELEM_TAG_ECDSA_SIG_SUITE_1          0x0001
#define OTA_SUB_ELEM_TAG_ECDSA_SIG_CERT_SUITE_1     0x0002
#define OTA_SUB_ELEM_TAG_IMG_INTEGRITY_CODE         0x0003
#define OTA_SUB_ELEM_TAG_PIC_DATA                   0x0004
#define OTA_SUB_ELEM_TAG_ECDSA_SIG_SUITE_2          0x0005
#define OTA_SUB_ELEM_TAG_ECDSA_SIG_CERT_SUITE_2     0x0006


#endif /* ZB_OSIF_OTA_STUB_H */
