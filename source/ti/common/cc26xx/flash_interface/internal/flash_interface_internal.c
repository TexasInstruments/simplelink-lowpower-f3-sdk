/******************************************************************************

 @file  flash_interface_internal.c

 @brief Driverlib based access of CC26xx internal flash

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 $License: BSD3 2017-2025 $
 ******************************************************************************
 
 
 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <stdbool.h>
#include <string.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/flash.h)
#if DeviceFamily_PARENT == DeviceFamily_PARENT_CC23X0 || DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX
    #include DeviceFamily_constructPath(inc/hw_vims.h)
#else
    #include DeviceFamily_constructPath(driverlib/vims.h)
#endif
#include "ti/common/cc26xx/flash_interface/flash_interface.h"

/*********************************************************************
 * MACROS
 */
#define HAL_ENTER_CRITICAL_SECTION(x) \
    do                                \
    {                                 \
        (x) = !IntMasterDisable();    \
    } while (0)

#define HAL_EXIT_CRITICAL_SECTION(x) \
    do                               \
    {                                \
        if (x)                       \
        {                            \
            (void)IntMasterEnable(); \
        }                            \
    } while (0)

/*********************************************************************
 * TYPEDEFS
 */

typedef uint32_t halIntState_t;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static uint8_t disableCache(void);
static void enableCache(uint8_t state);

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
    uint8_t cacheState;
    uint32_t flashStat = FLASH_SUCCESS;

    cacheState = disableCache();

    flashStat = FlashProgram((uint8_t *)pBuf, (uint32_t)addr, len);

    enableCache(cacheState);

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
    uint8_t cacheState;
    uint32_t flashStat = FLASH_SUCCESS;

    cacheState = disableCache();

    flashStat = FlashProgram(pBuf, (uint32_t)FLASH_ADDRESS(page, offset), len);

    enableCache(cacheState);

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
    uint8_t cacheState;
    uint8_t flashStat = FLASH_SUCCESS;

    cacheState = disableCache();

    flashStat = FlashSectorErase((uint32_t)FLASH_ADDRESS(page, 0));

    enableCache(cacheState);

    return ((flashStat == FAPI_STATUS_SUCCESS) ? FLASH_SUCCESS : FLASH_FAILURE);
}

/*********************************************************************
 * PRIVATE FUNCTIONS
 */

/*********************************************************************
 * @fn      disableCache
 *
 * @brief   Resumes system after a write to flash, if necessary.
 *
 * @param   None.
 *
 * @return  VIMS_MODE_ENABLED if cache was in use before this operation,
 *          VIMS_MODE_DISABLED otherwise.
 */
static uint8_t disableCache(void)
{
    uint8_t state = VIMSModeGet(VIMS_BASE);

    // Check VIMS state
    if (state != VIMS_MODE_DISABLED)
    {
        // Invalidate cache
        VIMSModeSet(VIMS_BASE, VIMS_MODE_DISABLED);

        // Wait for disabling to be complete
        while (VIMSModeGet(VIMS_BASE) != VIMS_MODE_DISABLED)
        {
            ;
        }
    }

    return (state);
}

/*********************************************************************
 * @fn      enableCache
 *
 * @brief   Prepares system for a write to flash, if necessary.
 *
 * @param   None.
 *
 * @return  None.
 */
static void enableCache(uint8_t state)
{
    if (state != VIMS_MODE_DISABLED)
    {
        // Enable the Cache.
        VIMSModeSet(VIMS_BASE, VIMS_MODE_ENABLED);
    }
}
