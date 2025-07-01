/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2025 Texas Instruments Incorporated
 *
 * Original license:
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "bootutil/compressed.h"
#include "bootutil/bootutil_log.h"
#include "bootutil/bootutil_public.h"
#include "bootutil/crypto/sha.h"
#include "bootutil/image.h"
#include "bootutil_priv.h"
#include "Lzma2Dec.h"
#include "LzmaConfig.h"
#include <os/os_malloc.h>

#define LZMA2_HEADER_SIZE 2

#if defined(MCUBOOT_SIGN_RSA)
#if MCUBOOT_SIGN_RSA_LEN == 2048
#define EXPECTED_SIG_TLV IMAGE_TLV_RSA2048_PSS
#elif MCUBOOT_SIGN_RSA_LEN == 3072
#define EXPECTED_SIG_TLV IMAGE_TLV_RSA3072_PSS
#else
#error "Unsupported RSA signature length"
#endif
#define SIG_BUF_SIZE (MCUBOOT_SIGN_RSA_LEN / 8)
#define EXPECTED_SIG_LEN(x) ((x) == SIG_BUF_SIZE) /* 2048 bits */
#elif defined(MCUBOOT_SIGN_EC256) || defined(MCUBOOT_SIGN_EC384) ||            \
    defined(MCUBOOT_SIGN_EC)
#define EXPECTED_SIG_TLV IMAGE_TLV_ECDSA_SIG
#define SIG_BUF_SIZE 128
#define EXPECTED_SIG_LEN(x) (1) /* always true, ASN.1 will validate */
#elif defined(MCUBOOT_SIGN_ED25519)
#define EXPECTED_SIG_TLV IMAGE_TLV_ED25519
#define SIG_BUF_SIZE 64
#define EXPECTED_SIG_LEN(x) ((x) == SIG_BUF_SIZE)
#else
#define SIG_BUF_SIZE 32 /* no signing, sha256 digest only */
#endif

#define UNUSED_VAR(x) (void)(x)
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

static uint8_t lzma2_hdr[LZMA2_HEADER_SIZE];
static uint8_t compressed_buf[LZMA2_DECOMP_CHUNK_SIZE];
static uint8_t decompressed_buf[LZMA2_DECOMP_CHUNK_SIZE];

static void *SzAlloc(ISzAllocPtr p, size_t size) {
    (void)(p);
    (void)(size);
    return NULL;
}

static void SzFree(ISzAllocPtr p, void *address) {
    (void)(p);
    (void)(address);
}

/**
 * Determines the size of the protected TLVs when the image is decompressed
 *
 * This function calculates the total size of the protected TLVs, ignoring the
 * TLVs related to the compressed image. It's useful for determining what
 * protected TLVs size would be once the image is decompressed.
 *
 * @param hdr Pointer to the image header structure.
 * @param fap Pointer to the flash area structure.
 * @param out_tlvs_sz Pointer to store the calculated size of the protected
 * TLVs section.
 *
 * @return 0 if the size is calculated successfully, or an error code if there
 * is an error.
 */
static int boot_get_protected_tlvs_size(const struct image_header *hdr,
                                        const struct flash_area *fap,
                                        uint16_t *out_tlvs_sz) {
    int rc;
    struct image_tlv_iter it;
    uint32_t offset;
    uint16_t len;
    uint16_t type;

    rc = bootutil_tlv_iter_begin(&it, hdr, fap, IMAGE_TLV_ANY, true);

    if (rc) {
        return rc;
    }

    *out_tlvs_sz = hdr->ih_protect_tlv_size;

    while (true) {
        rc = bootutil_tlv_iter_next(&it, &offset, &len, &type);
        if (rc < 0) {
            return -1;
        } else if (rc > 0) {
            break;
        }

        if (type == IMAGE_TLV_DECOMP_SIZE || type == IMAGE_TLV_DECOMP_SHA ||
            type == IMAGE_TLV_DECOMP_SIGNATURE ||
            type == IMAGE_TLV_COMP_DEC_SIZE) {
            *out_tlvs_sz -= (sizeof(struct image_tlv) + len);
        }
    }

    /* If there are no actual TLVs, we must remove the entire protected TLV
     * section altogether. Otherwise, integrity and signature verification will
     * fail */
    if (*out_tlvs_sz == sizeof(struct image_tlv_info)) {
        *out_tlvs_sz = 0;
    }

    return 0;
}

/**
 * Determines the size of the unprotected TLVs when the image is decompressed
 *
 * This function calculates the total size of the unprotected TLVs, ignoring the
 * TLVs related to the compressed image. It's useful for determining what
 * unprotected TLVs size would be once the image is decompressed.
 *
 * @param hdr Pointer to the image header structure.
 * @param fap Pointer to the flash area structure.
 * @param out_tlvs_sz Pointer to store the size of the unprotected TLVs.
 * @return 0 on success, or an error code on failure.
 */
static int boot_get_unprotected_tlvs_size(const struct image_header *hdr,
                                          const struct flash_area *fap,
                                          uint16_t *out_tlvs_sz) {
    int rc;
    struct image_tlv_iter it;
    uint32_t offset;
    uint16_t len;
    uint16_t type;

    *out_tlvs_sz = 0;

    rc = bootutil_tlv_iter_begin(&it, hdr, fap, IMAGE_TLV_ANY, false);
    if (rc) {
        return rc;
    }

    while (true) {
        rc = bootutil_tlv_iter_next(&it, &offset, &len, &type);
        if (rc < 0) {
            return -1;
        } else if (rc > 0) {
            break;
        }

        /* Add the DECOMP_SHA and DECOMP_SIGNATUE to the calculation because
        these will be moved into the decompressed image's unprotected TLV
        section */
        if (bootutil_tlv_iter_is_prot(&it, offset) &&
            (type == IMAGE_TLV_DECOMP_SHA &&
             type == IMAGE_TLV_DECOMP_SIGNATURE)) {
            *out_tlvs_sz += (sizeof(struct image_tlv) + len);
            continue;
        }

        /* Ignore these TLVs. IMAGE_TLV_COMP_DEC_SIZE will be moved to the
         * header of decompressed image while EXPECTED_HASH_TLV and
         * EXPECTED_SIG_TLV will be discarded */
        if (type == IMAGE_TLV_COMP_DEC_SIZE || type == EXPECTED_HASH_TLV ||
            type == EXPECTED_SIG_TLV) {
            continue;
        }

        /* Everything else must be added to calculation */
        *out_tlvs_sz += (sizeof(struct image_tlv) + len);
    }

    return 0;
}

static int boot_get_decompressed_image_size(const struct image_header *hdr,
                                            const struct flash_area *fap,
                                            uint32_t *out_decomp_sz) {
    int rc;
    struct image_tlv_iter it;
    uint32_t offset;
    uint16_t len;
    uint16_t type;

    rc = bootutil_tlv_iter_begin(&it, hdr, fap, IMAGE_TLV_DECOMP_SIZE, true);
    if (rc) {
        return rc;
    }

    rc = bootutil_tlv_iter_next(&it, &offset, &len, &type);
    if (rc < 0) {
        return -1;
    }

    rc = LOAD_IMAGE_DATA(hdr, fap, offset, out_decomp_sz, len);
    if (rc) {
        return rc;
    }

    return 0;
}

bool boot_is_header_valid_compressed(const struct image_header *hdr,
                                     const struct flash_area *fap,
                                     struct boot_loader_state *state) {
    int rc;
    int primary_fa_id;
    uint32_t primary_slot_sz;
    uint32_t secondary_slot_decomp_sz;
    uint32_t total_decompressed_sz;
    uint16_t protected_tlv_sz;
    uint16_t unprotected_tlv_sz;

    primary_fa_id = flash_area_id_from_multi_image_slot(BOOT_CURR_IMG(state),
                                                        BOOT_PRIMARY_SLOT);

    rc = flash_area_open(primary_fa_id,
                         &BOOT_IMG_AREA(state, BOOT_PRIMARY_SLOT));
    assert(rc == 0);

    primary_slot_sz =
        (uint32_t)BOOT_IMG_AREA(state, BOOT_PRIMARY_SLOT)->fa_size;
    boot_get_decompressed_image_size(hdr, fap, &secondary_slot_decomp_sz);
    boot_get_protected_tlvs_size(hdr, fap, &protected_tlv_sz);
    boot_get_unprotected_tlvs_size(hdr, fap, &unprotected_tlv_sz);

    if (!boot_u32_safe_add(&total_decompressed_sz, secondary_slot_decomp_sz,
                           hdr->ih_hdr_size)) {
        return false;
    }

    if (!boot_u32_safe_add(&total_decompressed_sz, total_decompressed_sz,
                           (uint32_t)protected_tlv_sz)) {
        return false;
    }

    if (!boot_u32_safe_add(&total_decompressed_sz, total_decompressed_sz,
                           (uint32_t)unprotected_tlv_sz)) {
        return false;
    }

    if (primary_slot_sz < total_decompressed_sz) {
        return false;
    }

    return true;
}

static int boot_copy_protected_tlvs_compressed(
    const struct image_header *hdr, const struct flash_area *fap_src,
    const struct flash_area *fap_dst, uint32_t off_dst,
    uint32_t decomp_protected_tlvs_sz, uint8_t *scratch_buf,
    uint32_t scratch_buf_sz, uint32_t *out_sz) {

    int rc;
    struct image_tlv_iter it;
    uint32_t offset;
    uint16_t len;
    uint16_t type;
    struct image_tlv tlv_hdr;
    struct image_tlv_info tlv_info_hdr;

    *out_sz = 0;

    if (decomp_protected_tlvs_sz <= 0) {
        BOOT_LOG_INF("There are no protected TLVs to copy.");
        return 0;
    }

    /* Create the new TLV info header and write to the destination flash area */
    tlv_info_hdr.it_magic = IMAGE_TLV_PROT_INFO_MAGIC;
    tlv_info_hdr.it_tlv_tot = decomp_protected_tlvs_sz;
    rc = flash_area_write(fap_dst, off_dst, &tlv_info_hdr,
                          sizeof(struct image_tlv_info));
    if (rc) {
        return rc;
    }

    off_dst += sizeof(struct image_tlv_info);
    *out_sz += sizeof(struct image_tlv_info);

    rc = bootutil_tlv_iter_begin(&it, hdr, fap_src, IMAGE_TLV_ANY, true);
    if (rc) {
        return rc;
    }

    while (true) {
        rc = bootutil_tlv_iter_next(&it, &offset, &len, &type);
        if (rc < 0 || scratch_buf_sz < len) {
            return -1;
        } else if (rc > 0) {
            break;
        }

        if (type == IMAGE_TLV_DECOMP_SIZE || type == IMAGE_TLV_DECOMP_SHA ||
            type == IMAGE_TLV_DECOMP_SIGNATURE ||
            type == IMAGE_TLV_COMP_DEC_SIZE) {
            continue;
        }

        tlv_hdr.it_type = type;
        tlv_hdr.it_len = len;
        memcpy(scratch_buf, &tlv_hdr, sizeof(struct image_tlv));
        rc = flash_area_read(fap_src, offset, scratch_buf + sizeof(tlv_hdr),
                             len);
        if (rc) {
            return rc;
        }

        rc = flash_area_write(fap_dst, off_dst, scratch_buf,
                              sizeof(struct image_tlv) + len);
        off_dst += (sizeof(struct image_tlv) + len);
        if (rc) {
            return rc;
        }

        *out_sz += (sizeof(struct image_tlv) + len);
    }

    return 0;
}

static int boot_copy_unprotected_tlvs_compressed(
    const struct image_header *hdr, const struct flash_area *fap_src,
    const struct flash_area *fap_dst, uint32_t off_dst,
    uint32_t decomp_unprotected_tlvs_sz, uint8_t *scratch_buf,
    uint32_t scratch_buf_sz, uint32_t *out_sz) {

    int rc;
    struct image_tlv_iter it;
    struct image_tlv_iter it_protected;
    uint32_t offset;
    uint16_t len;
    uint16_t type;
    uint16_t tlv_type_src;
    uint16_t tlv_type_dst;
    struct image_tlv tlv_hdr;
    struct image_tlv_info tlv_info_hdr;

    *out_sz = 0;

    if (decomp_unprotected_tlvs_sz <= 0) {
        BOOT_LOG_INF("There are no protected TLVs to copy.");
        return 0;
    }

    /* Create the new TLV info header and write to the destination flash area */
    tlv_info_hdr.it_magic = IMAGE_TLV_INFO_MAGIC;
    tlv_info_hdr.it_tlv_tot = decomp_unprotected_tlvs_sz;
    rc = flash_area_write(fap_dst, off_dst, &tlv_info_hdr,
                          sizeof(struct image_tlv_info));
    if (rc) {
        return rc;
    }

    off_dst += sizeof(struct image_tlv_info);
    *out_sz += sizeof(struct image_tlv_info);

    rc = bootutil_tlv_iter_begin(&it, hdr, fap_src, IMAGE_TLV_ANY, false);
    if (rc) {
        return rc;
    }

    while (true) {
        rc = bootutil_tlv_iter_next(&it, &offset, &len, &type);
        if (rc < 0 || scratch_buf_sz < len) {
            return -1;
        } else if (rc > 0) {
            break;
        }

        /* Don't copy protected TLVs (with exceptions later in this function) */
        if (bootutil_tlv_iter_is_prot(&it, offset)) {
            continue;
        }

        switch (type) {
            {
            case EXPECTED_HASH_TLV:
                tlv_type_src = IMAGE_TLV_DECOMP_SHA;
                break;
            case EXPECTED_SIG_TLV:
                tlv_type_src = IMAGE_TLV_DECOMP_SIGNATURE;
                break;
            default:
                tlv_type_src = type;
                break;
            }
        }

        tlv_type_dst = type;

        /* These TLVs must be copied into the unprotected TLV section. But they
         * come from the protected TLV section, so we have to look for them
         * there first */
        if (tlv_type_src == IMAGE_TLV_DECOMP_SHA ||
            tlv_type_src == IMAGE_TLV_DECOMP_SIGNATURE) {
            rc = bootutil_tlv_iter_begin(&it_protected, hdr, fap_src,
                                         tlv_type_src, true);
            if (rc) {
                return rc;
            }
            while (true) {
                rc =
                    bootutil_tlv_iter_next(&it_protected, &offset, &len, &type);
                if (rc < 0) {
                    return rc;
                } else if (rc > 0) {
                    rc = 0;
                    break;
                }
            }
        }

        tlv_hdr.it_type = tlv_type_dst;
        tlv_hdr.it_len = len;
        memcpy(scratch_buf, &tlv_hdr, sizeof(struct image_tlv));
        rc = flash_area_read(fap_src, offset,
                             scratch_buf + sizeof(struct image_tlv), len);
        if (rc) {
            return rc;
        }

        rc = flash_area_write(fap_dst, off_dst, scratch_buf,
                              sizeof(struct image_tlv) + len);
        if (rc) {
            return rc;
        }

        off_dst += (sizeof(struct image_tlv) + len);
        *out_sz += (sizeof(struct image_tlv) + len);
    }

    return 0;
}

/* The following code decompresses the image one block at a time into RAM and
 * then using the result to compute the hash. First phase, is to decompress
 * and calculate has over the header. Second, the image itself, and finally,
 * the protected TLV area.
 */
int bootutil_img_hash_compressed(struct enc_key_data *enc_state,
                                 int image_index, struct image_header *hdr,
                                 const struct flash_area *fap, uint8_t *tmp_buf,
                                 uint32_t tmp_buf_sz, uint8_t *hash_result,
                                 uint8_t *seed, int seed_len) {
    int rc;
    bootutil_sha_context sha_ctx;
    uint32_t read_offset;
    static struct image_header bl_hdr_decomp;
    uint32_t temp_decomp_ih_img_size;
    uint16_t temp_decomp_protect_tlv_size;
    size_t in_processed;
    size_t out_processed;
    ELzmaStatus status;

    /* Initialize LZMA2 decoder state */
    CLzma2Dec dec_state;
    ISzAlloc allocator = {SzAlloc, SzFree};
    Lzma2Dec_Construct(&dec_state);

    bootutil_sha_init(&sha_ctx);

    /* First, calculate hash over the decompressed header */
    memset(&bl_hdr_decomp, 0xFF, sizeof(struct image_header));
    memcpy(&bl_hdr_decomp, hdr, sizeof(struct image_header));
    rc = boot_get_decompressed_image_size(hdr, fap, &temp_decomp_ih_img_size);
    if (rc != 0) {
        return rc;
    }
    rc = boot_get_protected_tlvs_size(hdr, fap, &temp_decomp_protect_tlv_size);
    if (rc != 0) {
        return rc;
    }
    memcpy(&bl_hdr_decomp.ih_img_size, &temp_decomp_ih_img_size,
           sizeof(temp_decomp_ih_img_size));
    memcpy(&bl_hdr_decomp.ih_protect_tlv_size, &temp_decomp_protect_tlv_size,
           sizeof(temp_decomp_protect_tlv_size));
    bl_hdr_decomp.ih_flags &= ~COMPRESSIONFLAGS;

    rc = bootutil_sha_update(&sha_ctx, &bl_hdr_decomp, sizeof(struct image_header));
    if (rc != 0) {
        return rc;
    }

    read_offset = sizeof(struct image_header);
    while (read_offset < hdr->ih_hdr_size) {
        in_processed = MIN(hdr->ih_hdr_size - read_offset,
                           LZMA2_DECOMP_CHUNK_SIZE);
        rc = flash_area_read(fap, read_offset, decompressed_buf, in_processed);
        if (rc != 0) {
            return rc;
        }
        rc = bootutil_sha_update(&sha_ctx, decompressed_buf, in_processed);
        if (rc != 0) {
            return rc;
        }
        read_offset += in_processed;
    }

    /* Second, calculate the hash over the decompressed image */

    /* Read the LZMA2 header and use its values to allocated the probability
     * table, which is used for decompression algorithm */
    rc =
        flash_area_read(fap, read_offset, (void *)lzma2_hdr, LZMA2_HEADER_SIZE);
    if (rc != 0) {
        return BOOT_EFLASH;
    }
    read_offset = LZMA2_HEADER_SIZE;
    rc = Lzma2Dec_Allocate(&dec_state, lzma2_hdr[0], &allocator);
    if (rc != SZ_OK) {
        return SZ_ERROR_MEM;
    }
    Lzma2Dec_Init(&dec_state);

    while (read_offset < hdr->ih_img_size) {
        in_processed = MIN(hdr->ih_img_size - read_offset,
                           LZMA2_DECOMP_CHUNK_SIZE);
        out_processed = LZMA2_DECOMP_CHUNK_SIZE;
        rc = flash_area_read(fap, (hdr->ih_hdr_size + read_offset), compressed_buf, in_processed);
        if (rc != 0) {
            return rc;
        }

        rc = Lzma2Dec_DecodeToBuf(&dec_state, decompressed_buf, &out_processed,
                                  compressed_buf, &in_processed,
                                  LZMA_FINISH_ANY, &status);
        if (rc != SZ_OK) {
            return rc;
        }

        rc = bootutil_sha_update(&sha_ctx, decompressed_buf, out_processed);
        if (rc != 0) {
            return rc;
        }
        read_offset += in_processed;
    }

    /* Finally, calculate the hash over the protected TLV section */
    struct image_tlv_iter it;
    uint32_t offset;
    uint16_t len;
    uint16_t type;
    struct image_tlv tlv_hdr;
    struct image_tlv_info tlv_info_hdr;

    if (temp_decomp_protect_tlv_size > 0) {
        tlv_info_hdr.it_magic = IMAGE_TLV_PROT_INFO_MAGIC;
        tlv_info_hdr.it_tlv_tot = temp_decomp_protect_tlv_size;
        rc = bootutil_sha_update(&sha_ctx, &tlv_info_hdr,
                                sizeof(struct image_tlv_info));
        if (rc != 0) {
            return rc;
        }
        // read_offset += sizeof(struct image_tlv_info);

        rc = bootutil_tlv_iter_begin(&it, hdr, fap, IMAGE_TLV_ANY, true);
        if (rc) {
            return rc;
        }

        while (true) {
            rc = bootutil_tlv_iter_next(&it, &offset, &len, &type);
            if (rc < 0) {
                return -1;
            } else if (rc > 0) {
                break;
            }

            if (type == IMAGE_TLV_DECOMP_SIZE || type == IMAGE_TLV_DECOMP_SHA ||
                type == IMAGE_TLV_DECOMP_SIGNATURE ||
                type == IMAGE_TLV_COMP_DEC_SIZE) {
                continue;
            }

            tlv_hdr.it_type = type;
            tlv_hdr.it_len = len;
            memcpy(decompressed_buf, &tlv_hdr, sizeof(struct image_tlv));
            rc = flash_area_read(fap, offset, decompressed_buf + sizeof(tlv_hdr),
                                len);
            if (rc) {
                return rc;
            }

            rc = bootutil_sha_update(&sha_ctx, decompressed_buf,
                                    sizeof(tlv_hdr) + len);
            if (rc) {
                return rc;
            }
        }
    }

    rc = bootutil_sha_finish(&sha_ctx, hash_result);
    if (rc) {
        return rc;
    }

    return 0;
}

int boot_copy_region_compressed(struct boot_loader_state *bl_state,
                                const struct flash_area *fap_src,
                                const struct flash_area *fap_dst,
                                uint32_t off_src, uint32_t off_dst,
                                uint32_t sz) {

    int rc;

    // Initialize LZMA2 decoder state
    CLzma2Dec dec_state;
    ISzAlloc allocator = {SzAlloc, SzFree};
    Lzma2Dec_Construct(&dec_state);

    struct image_header *bl_hdr = boot_img_hdr(bl_state, BOOT_SECONDARY_SLOT);

    off_src += bl_hdr->ih_hdr_size;

    /* Read the LZMA2 header and use its values to allocated the probability
     * table, which is used for decompression algorithm */
    rc =
        flash_area_read(fap_src, off_src, (void *)lzma2_hdr, LZMA2_HEADER_SIZE);
    if (rc != 0) {
        return BOOT_EFLASH;
    }
    rc = Lzma2Dec_Allocate(&dec_state, lzma2_hdr[0], &allocator);
    if (rc != SZ_OK) {
        return SZ_ERROR_MEM;
    }

    /* Initialize the decompressor */
    Lzma2Dec_Init(&dec_state);
    size_t in_pos = 0;
    size_t in_size = 0;
    ELzmaStatus status;

    /* We know from the MCUboot header the size of the compressed image.
     * Currently we have only processed the LZMA2 header of the compressed
     * image*/
    uint32_t compressed_size = bl_hdr->ih_img_size;
    uint32_t total_processed_size = LZMA2_HEADER_SIZE;
    uint32_t total_out_processed = 0;
    uint32_t temp_decomp_ih_img_size;
    uint16_t temp_decomp_protect_tlv_size;
    uint16_t unprotected_tlv_sz;
    size_t in_processed;
    size_t out_processed;
    struct image_header bl_hdr_decomp;

    /* Now we make a copy of the header and modify it to include the correct
     * values for image size, flags, and protected tlv size*/
    memset(&bl_hdr_decomp, 0xFF, sizeof(struct image_header));
    memcpy(&bl_hdr_decomp, bl_hdr, sizeof(struct image_header));
    rc = boot_get_decompressed_image_size(bl_hdr, fap_src,
                                          &temp_decomp_ih_img_size);
    if (rc != 0) {
        return rc;
    }
    rc = boot_get_protected_tlvs_size(bl_hdr, fap_src,
                                      &temp_decomp_protect_tlv_size);
    if (rc != 0) {
        return rc;
    }
    memcpy(&bl_hdr_decomp.ih_img_size, &temp_decomp_ih_img_size,
           sizeof(temp_decomp_ih_img_size));
    memcpy(&bl_hdr_decomp.ih_protect_tlv_size, &temp_decomp_protect_tlv_size,
           sizeof(temp_decomp_protect_tlv_size));
    bl_hdr_decomp.ih_flags &= ~COMPRESSIONFLAGS;

    /* Write the modified header into the beginning of the primary slot */
    rc = flash_area_write(fap_dst, off_dst, &bl_hdr_decomp,
                          sizeof(struct image_header));
    if (rc != 0) {
        return rc;
    }
    off_dst += bl_hdr_decomp.ih_hdr_size;

    /* Now that header is copied, decompress the image into the primary
     * slot. Here we keep decompressing a chunk until all the compressed
     * data has been read and processed. The processing of TLV sections are
     * not included in this while loop
     */
    while (total_processed_size < compressed_size) {
        /**
         * Only read when the input chunk has been decompressed fully. This
         * reduces the RAM required to store the input chunk as well as
         * reduce the NVS read overhead.
         */
        if (in_size == in_pos) {
            rc = flash_area_read(fap_src, off_src + total_processed_size,
                                 compressed_buf, LZMA2_DECOMP_CHUNK_SIZE);
            if (rc != 0) {
                return rc;
            }

            in_pos = 0;
            in_size = MIN(LZMA2_DECOMP_CHUNK_SIZE,
                          compressed_size - total_processed_size);
        }

        /**
         * This is the number of bytes you want to process, but it will also
         * return how many bytes were actually processed after the
         * decompression. The API will only decompress a portion of this up
         * to what can fit in the decompressed_buf.
         */
        in_processed = in_size - in_pos;
        /**
         * This number is the maximum number of bytes of uncompressed data
         * that can fit in the decompressed_buf. It is also the output for
         * how many bytes were actually decompressed and put into the
         * decompressed_buf
         */
        out_processed = LZMA2_DECOMP_CHUNK_SIZE;
        rc = Lzma2Dec_DecodeToBuf(&dec_state, decompressed_buf, &out_processed,
                                  compressed_buf + in_pos, &in_processed,
                                  LZMA_FINISH_ANY, &status);
        if (rc != SZ_OK) {
            return rc;
        }

        total_processed_size += in_processed;
        in_pos += in_processed;

        if (out_processed > 0) {
            rc = flash_area_write(fap_dst, off_dst + total_out_processed,
                                  decompressed_buf, out_processed);
            if (rc != 0) {
                return rc;
            }
        }

        total_out_processed += out_processed;
    }

    /* Deinitialize LZMA2 decompressor */
    Lzma2Dec_FreeProbs(&dec_state, &allocator);

    /* Copy the protected TLVs */
    off_dst += total_out_processed;
    boot_copy_protected_tlvs_compressed(
        bl_hdr, fap_src, fap_dst, off_dst, bl_hdr_decomp.ih_protect_tlv_size,
        decompressed_buf, LZMA2_DECOMP_CHUNK_SIZE, (uint32_t*)&out_processed);
    /* Copy the unprotected TLVs */
    off_dst += out_processed;
    total_out_processed += out_processed;
    boot_get_unprotected_tlvs_size(bl_hdr, fap_src, &unprotected_tlv_sz);
    boot_copy_unprotected_tlvs_compressed(
        bl_hdr, fap_src, fap_dst, off_dst, unprotected_tlv_sz, decompressed_buf,
        LZMA2_DECOMP_CHUNK_SIZE, (uint32_t*)&out_processed);

    return 0;
}
