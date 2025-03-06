/******************************************************************************

 @file  flash_interface_internal.c

 @brief Driverlib based access of CC23xx internal flash

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2013-2025, Texas Instruments Incorporated
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

/*********************************************************************
 * INCLUDES
 */
#include <stdbool.h>
#include <string.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/flash.h)
#include DeviceFamily_constructPath(inc/hw_vims.h)
#include "ti/common/cc26xx/flash_interface/flash_interface.h"


/*********************************************************************
 * MACROS
 */
#define HAL_ENTER_CRITICAL_SECTION(x)  \
  do { (x) = !IntDisableMaster(); } while (0)

#define HAL_EXIT_CRITICAL_SECTION(x) \
  do { if (x) { (void) IntEnableMaster(); } } while (0)

/*********************************************************************
 * TYPEDEFS
 */

typedef uint32_t halIntState_t;

/*********************************************************************
 * @fn      flash_open
 *
 * @brief   Open an OAD target for download. For internal flash
 *          this function is a stub.
 *
 * @param   none
 *
 * @return  Always TRUE for internal flash
 */
bool flash_open(void)
{
  return (false);
}

/*********************************************************************
 * @fn      flash_close
 *
 * @brief   For internal flash this function is a stub.
 *
 * @param   none
 *
 * @return  none
 */
void flash_close(void)
{
	// Intentionally left blank.
}

/*********************************************************************
 * @fn      hasExternalFlash
 *
 * @brief   Check if the target has external flash
 *
 * @param   none
 *
 * @return  Always return FALSE, there is no external flash.
 */
bool hasExternalFlash(void)
{
  return (false);
}

/*********************************************************************
 * @fn      readFlash
 *
 * @brief   Read data from flash.
 *
 * @param   page   - page to read from in flash
 * @param   offset - offset into flash page to begin reading
 * @param   pBuf   - pointer to buffer into which data is read.
 * @param   len    - length of data to read in bytes.
 *
 * @return  None.
 */
uint8_t readFlash(uint_least32_t addr, uint8_t *pBuf, size_t len)
{
  halIntState_t cs;
  uint8_t *ptr = (uint8_t *)addr;

  // Enter critical section.
  HAL_ENTER_CRITICAL_SECTION(cs);

  // Read from pointer into buffer.
  while (len--)
  {
    *pBuf++ = *ptr++;
  }

  // Exit critical section.
  HAL_EXIT_CRITICAL_SECTION(cs);

  return (FLASH_SUCCESS);
}

/*********************************************************************
 * @fn      readFlash
 *
 * @brief   Read data from flash.
 *
 * @param   page   - page to read from in flash
 * @param   offset - offset into flash page to begin reading
 * @param   pBuf   - pointer to buffer into which data is read.
 * @param   len    - length of data to read in bytes.
 *
 * @return  None.
 */
uint8_t readFlashPg(uint8_t page, uint32_t offset, uint8_t *pBuf, uint16_t len)
{
  halIntState_t cs;
  uint8_t *ptr = (uint8_t *)FLASH_ADDRESS(page, offset);

  // Enter critical section.
  HAL_ENTER_CRITICAL_SECTION(cs);

  // Read from pointer into buffer.
  while (len--)
  {
    *pBuf++ = *ptr++;
  }

  // Exit critical section.
  HAL_EXIT_CRITICAL_SECTION(cs);

  return (FLASH_SUCCESS);
}

/*********************************************************************
 * @fn      writeFlash
 *
 * @brief   Write data to flash.
 *
 * @param   page   - page to write to in flash
 * @param   offset - offset into flash page to begin writing
 * @param   pBuf   - pointer to buffer of data to write
 * @param   len    - length of data to write in bytes
 *
 * @return  None.
 */
uint8_t writeFlash(uint_least32_t addr, uint8_t *pBuf, size_t len)
{
  uint32_t flashStat = FLASH_SUCCESS;

  flashStat = FlashProgram((uint8_t*)pBuf, (uint32_t)addr, len);

  return ((flashStat == FAPI_STATUS_SUCCESS) ? FLASH_SUCCESS : FLASH_FAILURE);
}

/*********************************************************************
 * @fn      writeFlash
 *
 * @brief   Write data to flash.
 *
 * @param   page   - page to write to in flash
 * @param   offset - offset into flash page to begin writing
 * @param   pBuf   - pointer to buffer of data to write
 * @param   len    - length of data to write in bytes
 *
 * @return  None.
 */
uint8_t writeFlashPg(uint8_t page, uint32_t offset, uint8_t *pBuf, uint16_t len)
{
  uint32_t flashStat = FLASH_SUCCESS;

  flashStat = FlashProgram(pBuf, (uint32_t)FLASH_ADDRESS(page, offset), len);

  return ((flashStat == FAPI_STATUS_SUCCESS) ? FLASH_SUCCESS : FLASH_FAILURE);
}

/*********************************************************************
 * @fn      eraseFlashPg
 *
 * @brief   Erase selected flash page.
 *
 * @param   page - the page to erase.
 *
 * @return  None.
 */
uint8_t eraseFlashPg(uint8_t page)
{
  uint8_t flashStat = FLASH_SUCCESS;

  flashStat = FlashEraseSector((uint32_t)FLASH_ADDRESS(page, 0));

  return ((flashStat == FAPI_STATUS_SUCCESS) ? FLASH_SUCCESS : FLASH_FAILURE);
}
