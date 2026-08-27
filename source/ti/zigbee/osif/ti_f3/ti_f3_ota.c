/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2026, Texas Instruments Incorporated
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

#include "zboss_api.h"
#include "zb_osif_ota.h"
#include "flash_interface.h"
#include <ti/log/Log.h>

#ifdef OTA_ONCHIP
#define PAGE_SIZE               INTFLASH_PAGE_SIZE
#else // OTA_OFFCHIP
#define PAGE_SIZE               EFL_PAGE_SIZE
#endif // OTA_ONCHIP

  /*
   * set ZB_F3_OTA_ENABLE_OTA_FW_TRAIL_BITS to 1 to enable trailer bits.
   *
   * NOTE: To be used only when using _ota_nopad.bin for OTA transfer.
   * When using _ota_2nd_slot.bin, BOOT_MAGIC is already embedded at slot_size-16
   * by imgtool --pad, ZB_F3_OTA_ENABLE_OTA_FW_TRAIL_BITS should not be enabled
   * in those cases
   */
#define ZB_F3_OTA_ENABLE_OTA_FW_TRAIL_BITS (0U)

// OTA Header Magic Number Bytes
static const zb_uint8_t otaHdrMagic[] = {0x1E, 0xF1, 0xEE, 0x0B};
static zb_uint16_t otaHdrLen = 0;
static zb_uint16_t otaElemTag;
static zb_uint32_t otaElemLen;
static zb_uint16_t otaHdrSubElemLen = 0;
static zb_bool_t otaHdrSubElemProcessed = ZB_FALSE;
static zb_uint16_t curFileOffset = 0;
#define OTA_MAGIC_SIZE      0X4
#define OTA_HEADER_MAX_SIZE 0x45
#define OTA_SUB_ELEM_TAG_LEN_SIZE 0x6 // 2 bytes for tag, 4 bytes for length
#define OTA_SUB_ELEM_DATA_MAX_SIZE 0X50
#define OTA_HDR_SUB_ELE_MAX_SIZE (OTA_HEADER_MAX_SIZE + OTA_MAGIC_SIZE + OTA_SUB_ELEM_TAG_LEN_SIZE + OTA_SUB_ELEM_DATA_MAX_SIZE)
#define OTA_HDR_LEN_OFFSET  0x6

zb_uint8_t clientWriteState;

zb_bool_t zb_osif_ota_open_storage(void)
{
  zb_bool_t ret = ZB_TRUE;

  ret = flash_open();

  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_open_storage");

  return ret;
}

zb_bool_t zb_osif_ota_fw_size_ok(zb_uint32_t image_size)
{
  zb_bool_t ret = ZB_FALSE;
  zb_uint32_t slot_size = (zb_uint32_t) &_SECONDARY_SLOT_SIZE;
  zb_uint32_t limit = slot_size + OTA_HDR_SUB_ELE_MAX_SIZE;

  if ( image_size <= limit )
  {
    ret = ZB_TRUE;
    clientWriteState = OTA_CLIENT_HDR_MAGIC_0_STATE;
  }

  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "fw_size_ok: image=%lu slot=%lu overhead=%d limit=%lu ret=%d",
    (unsigned long)image_size, (unsigned long)slot_size,
    OTA_HDR_SUB_ELE_MAX_SIZE, (unsigned long)limit, ret);

  return ret;
}

zb_uint32_t zb_osif_ota_get_erase_portion(void)
{
  zb_uint32_t erase_portion = 0;

  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_get_erase_portion");
  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_get_erase_portion ret %d", erase_portion);

  return erase_portion;
}

void zb_osif_ota_erase_fw(void *dev, zb_uint_t offset, zb_uint32_t size)
{
  ZVUNUSED(dev);

  zb_uint32_t numFlashPages = size / PAGE_SIZE;
  zb_uint32_t page = (0 + offset) / PAGE_SIZE;

  /* Note: eraseFlashPg takes uint8_t page — if numFlashPages > 255,
   * pages 256+ will truncate to 0+ and re-erase the beginning.
   * Fix requires sccm Conan package to change eraseFlashPg(uint8_t) -> uint32_t */
  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "erase_fw: size=%lu PAGE_SIZE=%d numPages=%lu startPage=%lu",
    (unsigned long)size, PAGE_SIZE,
    (unsigned long)numFlashPages, (unsigned long)page);

  for ( ; page < numFlashPages; page++ )
  {
    if (eraseFlashPg(page) != FLASH_SUCCESS)
    {
      Log_printf(LogModule_Zigbee_App, Log_INFO, "erase FAILED at page %lu", (unsigned long)page);
      while(1){
        asm("NOP");
      }
    }
  }

  Log_printf(LogModule_Zigbee_App, Log_INFO,
    "erase_fw DONE offset %d size %d pages erased %lu",
    offset, size, (unsigned long)numFlashPages);
}

zb_uint8_t zb_osif_ota_write(void *dev, zb_uint8_t *data, zb_uint_t off, zb_uint_t size, zb_uint32_t image_size)
{
  ZVUNUSED(dev);
  ZVUNUSED(image_size);

  zb_uint32_t page;
  zb_uint32_t offset;
  zb_uint8_t status;
  zb_uint8_t i;

  if (!otaHdrSubElemProcessed)
  {
    for (i = 0; i<size; i++)
    {
      switch (clientWriteState)
      {
      case OTA_CLIENT_HDR_MAGIC_0_STATE:
      case OTA_CLIENT_HDR_MAGIC_1_STATE:
      case OTA_CLIENT_HDR_MAGIC_2_STATE:
      case OTA_CLIENT_HDR_MAGIC_3_STATE:
        if (data[i] != otaHdrMagic[clientWriteState])
        {
          return ZB_ZCL_STATUS_FAIL;
        }
        clientWriteState++;
        break;

      case OTA_CLIENT_HDR_LEN_1_STATE:
        if (curFileOffset == OTA_HDR_LEN_OFFSET)
        {
          otaHdrLen = data[i];
          clientWriteState++;
        }
        break;

      case OTA_CLIENT_HDR_LEN_2_STATE:
        otaHdrLen |= ( ( ( uint16_t ) data[i] ) << 8 ) & 0xFF00;
        clientWriteState++;
        break;

      case OTA_CLIENT_SUB_ELEM_TAG_1_STATE:
        if (curFileOffset == otaHdrLen)
        {
          otaElemTag = data[i];
          clientWriteState++;
        }
        break;

      case OTA_CLIENT_SUB_ELEM_TAG_2_STATE:
        otaElemTag |= ( ( ( uint16_t ) data[i] ) << 8 ) & 0xFF00;
        clientWriteState++;
        break;

      case OTA_CLIENT_SUB_ELEM_LEN_1_STATE:
        otaElemLen = data[i];
        clientWriteState++;
        break;

      case OTA_CLIENT_SUB_ELEM_LEN_2_STATE:
        otaElemLen |= ( ( uint32_t ) data[i] << 8 ) & 0x0000FF00;
        clientWriteState++;
        break;

      case OTA_CLIENT_SUB_ELEM_LEN_3_STATE:
        otaElemLen |= ( ( uint32_t ) data[i] << 16 ) & 0x00FF0000;
        clientWriteState++;
        break;

      case OTA_CLIENT_SUB_ELEM_LEN_4_STATE:
        otaElemLen |= ( ( uint32_t ) data[i] << 24 ) & 0xFF000000;
        clientWriteState++;
        otaHdrSubElemProcessed = ZB_TRUE;
        break;

      // TODO Currently we only fully support sub elemnt tag Update Image (0x0).
      // TODO Logic needs to be added to handle other tag types validation.

      default:
        // Nothing to do
        break;
      }

      curFileOffset++;
      if (otaHdrSubElemProcessed)
      {
        if(OTA_SUB_ELEM_TAG_UPDATE_IMAGE == otaElemTag)
        {
          // In this case, otaElemLen is the len of the image so do not inlcude.
          otaHdrSubElemLen = otaHdrLen + OTA_SUB_ELEM_TAG_LEN_SIZE;
        }
        else
        {
          otaHdrSubElemLen = otaHdrLen + OTA_SUB_ELEM_TAG_LEN_SIZE + otaElemLen;
        }
        break;
      }
    }
  }

  if (otaHdrSubElemProcessed)
  {
    if (otaHdrSubElemLen < ( off + size ))
    {
      // Wite image with offset of OTA Header and Sub Element length
      if (otaHdrSubElemLen > off)
      {
        page = 0;
        offset = 0;
        data += (size - ((off + size) - otaHdrSubElemLen));
        size = (off + size) - otaHdrSubElemLen;
      }
      else
      {
        page = ((zb_uint32_t) off - otaHdrSubElemLen) / PAGE_SIZE;
        offset = (off - otaHdrSubElemLen) % PAGE_SIZE;
      }

      status = writeFlashPg(page, offset, data, size);
      if (status != FLASH_SUCCESS)
      {
        return ZB_ZCL_STATUS_FAIL;
      }
    }
  }

  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_write offset %d size %d image size %d",
             off, size, image_size);
  return ZB_ZCL_STATUS_SUCCESS;
}

void zb_osif_ota_mark_fw_ready(void *dev, zb_uint32_t size, zb_uint32_t revision)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_mark_fw_ready size %d revision %d",
             size, revision);

#if ZB_F3_OTA_ENABLE_OTA_FW_TRAIL_BITS
  /* BOOT_MAGIC write — only needed when using _ota_nopad.bin for OTA transfer.
   * When using _ota_2nd_slot.bin, BOOT_MAGIC is already embedded at slot_size-16
   * by imgtool --pad, so this function body can remain empty.
   *
   * To enable (e.g. if switching to nopad-based OTA):
   *   1. define ZB_F3_OTA_ENABLE_OTA_FW_TRAIL_BITS as 1
   *   2. Ensure sccm eraseFlashPg/writeFlashPg use uint32_t page (not uint8_t).
   *   3. Only write if revision > 1 to prevent V1 accidentally triggering a swap.
   *
   * BOOT_MAGIC layout:
   *   slot_size  = _SECONDARY_SLOT_SIZE = 0xE0000 = 917504 bytes
   *   magic_off  = slot_size - 16       = 0xDFFF0  (NVS offset within secondary slot)
   *   magic_page = 0xDFFF0 / 2048       = 447      (last page of secondary slot)
   *   page_off   = 0xDFFF0 % 2048       = 2032     (last 16 bytes of that page)
   *   BOOT_MAGIC absolute address       = secondaryBase + 0xDFFF0
   */

   if (revision <= 1)
   {
     return;
   }

   static const zb_uint8_t boot_magic[16] = {
     0x77, 0xc2, 0x95, 0xf3, 0x60, 0xd2, 0xef, 0x7f,
     0x35, 0x52, 0x50, 0x0f, 0x2c, 0xb6, 0x79, 0x80
   };

   zb_uint32_t slot_size  = (zb_uint32_t) &_SECONDARY_SLOT_SIZE;
   zb_uint32_t magic_off  = slot_size - sizeof(boot_magic);
   zb_uint32_t magic_page = magic_off / PAGE_SIZE;
   zb_uint32_t page_off   = magic_off % PAGE_SIZE;

   zb_uint8_t rc = eraseFlashPg(magic_page);
   Log_printf(LogModule_Zigbee_App, Log_INFO,
              "mark_fw_ready: erase page %lu rc=%d", (unsigned long)magic_page, rc);

   rc = writeFlashPg(magic_page, page_off, (zb_uint8_t *)boot_magic, sizeof(boot_magic));
   Log_printf(LogModule_Zigbee_App, Log_INFO,
              "mark_fw_ready: BOOT_MAGIC written page=%lu off=%lu rc=%d",
              (unsigned long)magic_page, (unsigned long)page_off, rc);
#endif
}

void zb_osif_ota_mark_fw_absent(void)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_mark_fw_absent");
}


void zb_osif_ota_mark_fw_updated(void)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_mark_fw_updated");
}

void zb_osif_ota_close_storage(void *dev)
{

  flash_close();

  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_close_storage");
}

zb_bool_t zb_osif_ota_verify_integrity(void *dev, zb_uint32_t raw_len)
{
  zb_bool_t ret = ZB_TRUE;

  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_verify_integrity len %d", raw_len);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_verify_integrity ret %d", ret);

  return ret;
}

/* WARNING: Works with absolute address! */
void zb_osif_ota_read(void *dev, zb_uint8_t *data, zb_uint32_t addr, zb_uint32_t size)
{
  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_read addr %d size %d", addr, size);

}

zb_bool_t zb_osif_ota_verify_integrity_async(void *dev, zb_uint32_t raw_len)
{
  zb_bool_t ret = ZB_FALSE;

  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_verify_integrity_async len %d", raw_len);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_osif_ota_verify_integrity_async ret %d", ret);
  return ret;
}


zb_uint8_t zb_erase_fw(zb_uint32_t address, zb_uint32_t pages_count)
{
  ZVUNUSED(address);
  ZVUNUSED(pages_count);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_erase_fw");
  return 0;
}

zb_uint8_t zb_write_fw(zb_uint32_t address, zb_uint8_t *buf, zb_uint16_t len)
{
  ZVUNUSED(address);
  ZVUNUSED(buf);
  ZVUNUSED(len);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_write_fw");
  return 0;
}


void Hash16_Calc(zb_uint32_t pBuffer, zb_uint32_t BufferLength, zb_uint8_t *hash16)
{
  ZVUNUSED(pBuffer);
  ZVUNUSED(BufferLength);
  ZVUNUSED(hash16);
  Log_printf(LogModule_Zigbee_App, Log_INFO, "zb_nvram_app_write_dataset");
}
