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

#ifndef ZB_HAL_FLASH_H
#define ZB_HAL_FLASH_H 1

/* as per CC1352P datasheet 8.7 Nonvolatile (Flash) Memory Characteristics:
  Maximum number of write operations per row before sector erase - 83 Write Operations
  Each wordline is 2048 bits (or 256 bytes) wide. This limitation corresponds to sequential memory writes of 4 (3.1) bytes minimum per
  write over a whole wordline. If additional writes to the same wordline are required, a sector erase is required once the maximum
  number of write operations per row is reached.

  it means that we can modify only one 4-bytes word per 256-bytes wordline, it will make out 64 Write Operations,
  otherwise we'll exceed the limit in 83 Write Operations
*/
#define ZB_FLASH_MAX_WRITES               1u
#define ZB_FLASH_WORD_SIZE_BYTES          4u

#define ZB_OSIF_NVRAM_PHY_PAGE_SIZE       0x0800UL /* 2KB */
#define ZB_NVRAM_PAGE_COUNT               2
#define ZB_OSIF_NVRAM_PAGE_SIZE           ZB_OSIF_NVRAM_PHY_PAGE_SIZE
#define ZB_NVRAM_PHY_PAGE_SIZE            ZB_OSIF_NVRAM_PHY_PAGE_SIZE

#if defined(ZB_ZGPD_ROLE)
/* 20480 */
#define ZB_FLASH_MSS_TOTAL_BITS           (ZB_MSS_PAGE_COUNT * ZB_OSIF_NVRAM_PAGE_SIZE / ZB_FLASH_WORD_SIZE_BYTES * ZB_FLASH_MAX_WRITES)
/* This value should be approximately equal to (ZB_FLASH_MSS_TOTAL_BITS ^ 1/3) / ZB_FLASH_MAX_WRITES */
#define ZB_FLASH_MSS_ONE_LEVEL_WORDS      26u /* 78728 bytes of 81920 (10 pages) will be used */
#endif // ZB_ZGPD_ROLE

#if defined(ZB_USE_NVRAM)

/**
 * @brief osif NVRAM initializer
 */
#ifdef ZB_ZGPD_ROLE
extern void zb_osif_nvram_init();
#else // ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE || ZB_ED_ROLE
extern void zb_osif_nvram_init(const zb_char_t *name);
#endif // ZB_ZGPD_ROLE

/**
 * @brief Get NVRAM page length
 *
 * @return NVRAM page length
 */
#ifdef ZB_ZGPD_ROLE
extern zb_uint16_t zb_get_nvram_page_length(void);
#else // ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE || ZB_ED_ROLE
extern zb_uint32_t zb_get_nvram_page_length(void);
#endif // ZB_ZGPD_ROLE
/**
 * @brief Get NVRAM page count
 *
 * @return NVRAM page count
 */
#ifdef ZB_ZGPD_ROLE
extern zb_uint16_t zb_get_nvram_page_count(void);
#else // ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE || ZB_ED_ROLE
extern zb_uint8_t zb_get_nvram_page_count(void);
#endif // ZB_ZGPD_ROLE

/**
 * @brief Read from NVRAM directly, by address
 * Read some bytes from NVRAM as is using the address, the operation doesn't suppose any other side-effects,
 * all required validation and processing of read data must be done at upper layer
 *
 * @param address - NVRAM address
 * @param buf - (in) buffer for contains read data
 * @param len - count bytes from read data
 *
 * @return RET_OK if success or code error
 */
#ifdef ZB_ZGPD_ROLE
extern zb_ret_t zb_osif_nvram_read_memory(zb_uint32_t address, zb_uint8_t *buf, zb_uint16_t len );
#else // ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE || ZB_ED_ROLE
extern zb_ret_t zb_osif_nvram_read_memory(zb_uint32_t address, zb_uint32_t len, zb_uint8_t *buf);
#endif // ZB_ZGPD_ROLE

/**
 * @brief Write to NVRAM directly by address
 * Write some bytes to NVRAM as is, the operation doesn't suppose any other side-effects,
 * all required validation and modification of data intended to be written must be done at upper layer
 *
 * @param address - An offset in the NVRAM array
 * @param buf     - buffer contains data for write
 * @param len     - count bytes for write data
 *
 * @return RET_OK if success or code error
 */
#ifdef ZB_ZGPD_ROLE
extern zb_ret_t zb_osif_nvram_write_memory(zb_uint32_t address, zb_uint8_t *buf, zb_uint16_t len );
#else // ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE || ZB_ED_ROLE
extern zb_ret_t zb_osif_nvram_write_memory(zb_uint32_t address, zb_uint32_t len, zb_uint8_t *buf);
#endif // ZB_ZGPD_ROLE

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE
zb_ret_t zb_osif_nvram_erase_async(zb_uint8_t page);
#endif // defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE ||  defined ZB_ED_ROLE || !defined ZB_ZGPD_ROLE

/**
 * @brief Erase NVRAM page
 * Fill NVRAM page by 0xFF.
 *
 * @param page - page index
 *
 * @return RET_OK if success or code error
 */
#ifdef ZB_ZGPD_ROLE
extern zb_ret_t zb_osif_nvram_erase(zb_uint16_t page);
#else // ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE || ZB_ED_ROLE
extern zb_ret_t zb_osif_nvram_erase_memory(zb_uint32_t address, zb_uint32_t len);
#endif // ZB_ZGPD_ROLE

/**
 * @brief Flush NVRAM page
 * Flish NVRAM page to file or NVRAM.
 * Different to hardware device
 *
 */
extern void zb_osif_nvram_flush();

#endif // ZB_USE_NVRAM
#endif //ZB_HAL_FLASH_H
