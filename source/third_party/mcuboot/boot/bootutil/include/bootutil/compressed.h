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

#include "../src/bootutil_priv.h"
#include "bootutil/image.h"
#include <stdbool.h>
#include <stdint.h>


#if defined(MCUBOOT_DECOMPRESS_IMAGES) && !defined(MCUBOOT_OVERWRITE_ONLY)
#error "MCUBOOT_DECOMPRESS_IMAGES is only supported in MCUBOOT_OVERWRITE_ONLY mode."
#endif

#if defined(MCUBOOT_DECOMPRESS_IMAGES) && defined(MCUBOOT_ENC_IMAGES)
#error "MCUBOOT_DECOMPRESS_IMAGES is not supported with MCUBOOT_ENC_IMAGES"
#endif

/**
 * Check if the decompressed header of a compressed image is valid.
 *
 * @param hdr The image header.
 * @param fap The flash area where the compressed image is stored.
 * @param state The boot loader state.
 * @return true if the header is valid, false otherwise.
 */
bool boot_is_header_valid_compressed(const struct image_header *hdr,
                                     const struct flash_area *fap,
                                     struct boot_loader_state *state);

/**
 * Calculate the decompressed hash of a compressed image.
 *
 * @param enc_state The encryption state.
 * @param image_index The index of the compressed image.
 * @param hdr The image header.
 * @param fap The flash area where the compressed image is stored.
 * @param tmp_buf The temporary buffer.
 * @param tmp_buf_sz The size of the temporary buffer.
 * @param hash_result The resulting hash.
 * @param seed The seed for the hash calculation.
 * @param seed_len The length of the seed.
 * @return 0 on success, an error code otherwise.
 */
int bootutil_img_hash_compressed(struct enc_key_data *enc_state,
                                 int image_index, struct image_header *hdr,
                                 const struct flash_area *fap, uint8_t *tmp_buf,
                                 uint32_t tmp_buf_sz, uint8_t *hash_result,
                                 uint8_t *seed, int seed_len);

/**
 * Decompress a compressed region into another flash area.
 *
 * @param bl_state The boot loader state.
 * @param fap_src The source flash area.
 * @param fap_dst The destination flash area.
 * @param off_src The offset in the source flash area.
 * @param off_dst The offset in the destination flash area.
 * @param sz The size of the region to copy.
 * @return 0 on success, an error code otherwise.
 */
int boot_copy_region_compressed(struct boot_loader_state *bl_state,
                                const struct flash_area *fap_src,
                                const struct flash_area *fap_dst,
                                uint32_t off_src, uint32_t off_dst,
                                uint32_t sz);