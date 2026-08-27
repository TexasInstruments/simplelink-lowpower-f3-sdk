/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

#ifndef FLASH_MAP_H
#define FLASH_MAP_H

/**
 *
 * Provides abstraction of flash regions for type of use.
 * I.e. dude where's my image?
 *
 * System will contain a map which contains flash areas. Every
 * region will contain flash identifier, offset within flash and length.
 *
 * 1. This system map could be in a file within filesystem (Initializer
 * must know/figure out where the filesystem is at).
 * 2. Map could be at fixed location for project (compiled to code)
 * 3. Map could be at specific place in flash (put in place at mfg time).
 *
 * Note that the map you use must be valid for BSP it's for,
 * match the linker scripts when platform executes from flash,
 * and match the target offset specified in download script.
 */
#include <inttypes.h>

/**
 * @brief Structure describing an area on a flash device.
 *
 * Multiple flash devices may be available in the system, each of
 * which may have its own areas. For this reason, flash areas track
 * which flash device they are part of.
 */
struct flash_area {
    /**
     * This flash area's ID; unique in the system.
     */
    uint8_t fa_id;

    /**
     * ID of the flash device this area is a part of.
     */
    uint8_t fa_device_id;

    uint16_t pad16;

    /**
     * This area's offset, relative to the beginning of its flash
     * device's storage.
     */
    uint32_t fa_off;

    /**
     * This area's size, in bytes.
     */
    uint32_t fa_size;
};

/**
 * @brief Structure describing a sector within a flash area.
 *
 * Each sector has an offset relative to the start of its flash area
 * (NOT relative to the start of its flash device), and a size. A
 * flash area may contain sectors with different sizes.
 */
struct flash_sector {
    /**
     * Offset of this sector, from the start of its flash area (not device).
     */
    uint32_t fs_off;

    /**
     * Size of this sector, in bytes.
     */
    uint32_t fs_size;
};

struct flash_map_entry {
    uint32_t magic;
    struct flash_area area;
    unsigned int ref_count;
};

/*
 * Retrieve a memory-mapped flash device's base address.
 * On success, the address will be stored in the value pointed to by
 * ret.
 * Returns 0 on success, or an error code on failure.
 */
int flash_device_base(uint8_t fd_id, uintptr_t *ret);

/*< Opens the area for use. id is one of the ``fa_id``s */
int flash_area_open(uint8_t id, const struct flash_area **);
void flash_area_close(const struct flash_area *);
/*< Reads ``len`` bytes of flash memory at ``off`` to the buffer at ``dst`` */
int flash_area_read(const struct flash_area *, uint32_t off, void *dst,
                     uint32_t len);
/*< Writes ``len`` bytes of flash memory at ``off`` from the buffer at ``src`` */
int flash_area_write(const struct flash_area *, uint32_t off,
                     const void *src, uint32_t len);
/*< Erases ``len`` bytes of flash memory at ``off`` */
int flash_area_erase(const struct flash_area *, uint32_t off, uint32_t len);
/*< Returns this ``flash_area``s alignment */
size_t flash_area_align(const struct flash_area *);
/*< Initializes an array of flash_area elements for the slot's sectors */
int flash_area_to_sectors(int idx, int *cnt, struct flash_area *ret);
/*< Returns the ``fa_id`` for slot, where slot is 0 (primary) or 1 (secondary) */
int flash_area_id_from_image_slot(int slot);
/*< Returns the slot, for the ``fa_id`` supplied */
int flash_area_id_to_image_slot(int area_id);

int flash_area_id_from_multi_image_slot(int image_index, int slot);
int flash_area_id_to_multi_image_slot(int image_index, int area_id);
#ifdef MCUBOOT_USE_FLASH_AREA_GET_SECTORS
int flash_area_get_sectors(int idx, uint32_t *cnt, struct flash_sector *ret);
#endif
int flash_area_get_sector(const struct flash_area *fa, uint32_t off, struct flash_sector *sector);
#ifdef MCUBOOT_HW_ROLLBACK_PROT
void flash_area_lock(const struct flash_area *fa);
#endif
/*
 * Returns the value expected to be read when accessing any erased
 * flash byte.
 */
uint8_t flash_area_erased_val(const struct flash_area *fap);

/*
 * Reads len bytes from off, and checks if the read data is erased.
 *
 * Returns 1 if erased, 0 if non-erased, and -1 on failure.
 */
int flash_area_read_is_empty(const struct flash_area *fa, uint32_t off,
        void *dst, uint32_t len);

static inline uint32_t flash_area_get_off(const struct flash_area *fa)
{
    return (uint32_t)fa->fa_off;
}

static inline uint32_t flash_area_get_size(const struct flash_area *fa)
{
    return (uint32_t)fa->fa_size;
}

static inline uint32_t flash_sector_get_off(const struct flash_sector *fs)
{
    return fs->fs_off;
}

static inline uint32_t flash_sector_get_size(const struct flash_sector *fs)
{
    return fs->fs_size;
}

static inline uint8_t flash_area_get_device_id(const struct flash_area *fa)
{
    return fa->fa_device_id;
}

static inline uint8_t flash_area_get_id(const struct flash_area *fa)
{
    return fa->fa_id;
}
#endif /* FLASH_MAP_H */