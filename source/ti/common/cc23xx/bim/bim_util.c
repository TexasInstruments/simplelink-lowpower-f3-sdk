/******************************************************************************

 @file  bim_util.c

 @brief This module contains the definitions for the main functionality of a
        Boot Image Manager.

 Group: CMCU
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2024, Texas Instruments Incorporated
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

/*******************************************************************************
 *                                          Includes
 */

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/hapi.h)

#include "ti/common/cc26xx/oad/oad_image_header.h"
#include "ti/common/cc26xx/flash_interface/flash_interface.h"

/*******************************************************************************
 * LOCAL VARIABLES
 */

const uint8_t OAD_IMG_ID[OAD_IMG_ID_LEN] = OAD_IMG_ID_VAL;

#ifndef BIM_ONCHIP /* used for off-chip case only */
const uint8_t OAD_EXTFL_ID[OAD_IMG_ID_LEN] = OAD_EXTFL_ID_VAL;
#endif //#ifdef BIM_ONCHIP

/*******************************************************************************
 * LOCAL FUNCTIONS
 */

/*******************************************************************************
 * @fn          jumpToPrgEntry
 *
 * @brief       This function jumps the execution to program entry to execute
 *              application
 *
 * @param       vectorTable - address of application vector table.
 *
 * @return      None.
 */

void jumpToPrgEntry(uint32_t *vectorTable)
{
    /* The following code resets the SP to the value specified in the
     * provided vector table, and then the Reset Handler is invoked.
     *
     * Per ARM Cortex specification:
     *
     *           ARM Cortex VTOR
     *
     *
     *   Offset             Vector
     *
     * 0x00000000  ++++++++++++++++++++++++++
     *             |    Initial SP value    |
     * 0x00000004  ++++++++++++++++++++++++++
     *             |         Reset          |
     * 0x00000008  ++++++++++++++++++++++++++
     *             |          NMI           |
     *             ++++++++++++++++++++++++++
     *             |           .            |
     *             |           .            |
     *             |           .            |
     *
     * */

    /* Reset the SP with the value stored at vector_table[0] */
    __asm volatile ("MSR msp, %0" : : "r" (vectorTable[0]) : );

    /* Jump to the Reset Handler address at vector_table[1] */

    ( (void (*)(void)) (*(vectorTable + 1)) )();
}

/*******************************************************************************
 * @fn          imgIDCheck
 *
 * @brief       This function check if the buffer contains OAD image
 *              identification value.
 *
 * @param       imgHdr - pointer to image identification header buffer.
 *
 * @return      None.
 */
bool imgIDCheck(imgFixedHdr_t *imgHdr)
{
    uint8_t* pBuff = imgHdr->imgID;

    int8_t idLen = OAD_IMG_ID_LEN - 1;
    while( idLen >= 0)
    {
        if (pBuff[idLen] != OAD_IMG_ID[idLen])
        {
            return(false);
        }
        idLen--;
    }
    return(true);
}

/*******************************************************************************
 * @fn     setLowPowerMode
 *
 * @brief  This function sets the device to the lowest power mode, and does not
 *         return.
 */
void setLowPowerMode()
{
    /* Disable interrupts */
    IntDisableMaster();


    /* - Save CPU state on MSP and MSP in CLKCTL_STBYPTR
     * - Enter standby
     * - Exit standby
     * - Restore CPU state from MSP
     * - Apply copyList
     */
    HapiEnterStandby(NULL);

    /* Should never return from HapiEnterStandby */
}

#ifndef BIM_ONCHIP /* used for off-chip case only */
/*******************************************************************************
 * @fn          metadataIDCheck
 *
 * @brief       This function check if the buffer contains external flash OAD
 *              metadata identification value.
 *
 * @param       imgHdr - pointer to flash metadata header buffer.
 *
 * @return      None.
 */
bool metadataIDCheck(imgFixedHdr_t *imgHdr)
{
    uint8_t* pBuff = imgHdr->imgID;

    int8_t idLen = OAD_IMG_ID_LEN - 1;
    while( idLen >= 0)
    {
        if (pBuff[idLen] != OAD_EXTFL_ID[idLen])
        {
            return(false);
        }
        idLen--;
    }
    return(true);
}

#endif // #ifndef BIM_ONCHIP


#ifdef BIM_ONCHIP /* used for on-chip case only */
/*******************************************************************************
 * @fn     evenBitCount
 *
 * @brief  Finds if the value field has even number of bits set to 1's.
 *
 * @param  value  - input field on which to calculate the even number of 1's.
 *
 * @return True when the value field has even number of 1's, otherwise returns
 *         False.
 */
bool evenBitCount(uint32_t value)
{
  uint8_t count;

  for (count = 0; value; count++)
  {
    value &= value - 1;
  }

  if (count % 2)
  {
    return false;
  }
  else
  {
    return true;
  }
}

#endif //#ifdef BIM_ONCHIP

/*******************************************************************************
*/
