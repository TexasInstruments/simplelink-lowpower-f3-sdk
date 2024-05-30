/******************************************************************************

 @file  ll_dfl.h

 @brief This file contains the APIs for handling with the dynamic filter list
        data structures. This filter list using by the radio core, and
        managed by the controller. The dynamic filter list working with LRU
        (Last Recent Use) mechanism which maintained by the API's in this
        file.
        Each entry in the dynamic filter list has a rank so the higher the
        rank, the older the entry.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2024, Texas Instruments Incorporated

 All rights reserved not granted herein.
 Limited License.

 Texas Instruments Incorporated grants a world-wide, royalty-free,
 non-exclusive license under copyrights and patents it now or hereafter
 owns or controls to make, have made, use, import, offer to sell and sell
 ("Utilize") this software subject to the terms herein. With respect to the
 foregoing patent license, such license is granted solely to the extent that
 any such patent is necessary to Utilize the software alone. The patent
 license shall not apply to any combinations which include this software,
 other than combinations with devices manufactured by or for TI ("TI
 Devices"). No hardware patent is licensed hereunder.

 Redistributions must preserve existing copyright notices and reproduce
 this license (including the above copyright notice and the disclaimer and
 (if applicable) source code license limitations below) in the documentation
 and/or other materials provided with the distribution.

 Redistribution and use in binary form, without modification, are permitted
 provided that the following conditions are met:

   * No reverse engineering, decompilation, or disassembly of this software
     is permitted with respect to any software provided in binary form.
   * Any redistribution and use are licensed by TI for use only with TI Devices.
   * Nothing shall obligate TI to provide you with source code for the software
     licensed and provided to you in object code.

 If software source code is provided to you, modification and redistribution
 of the source code are permitted provided that the following conditions are
 met:

   * Any redistribution and use of the source code, including any resulting
     derivative works, are licensed by TI for use only with TI Devices.
   * Any redistribution and use of any object code compiled from the source
     code and any resulting derivative works, are licensed by TI for use
     only with TI Devices.

 Neither the name of Texas Instruments Incorporated nor the names of its
 suppliers may be used to endorse or promote products derived from this
 software without specific prior written permission.

 DISCLAIMER.

 THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

#ifndef LL_DFL_H
#define LL_DFL_H
/*******************************************************************************
 * INCLUDES
 */
#include "ll_dfl_internal.h"

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * LOCAL FUNCTION DECLARATIONS
 */

/*******************************************************************************
 * @fn          LL_DFL_GetDynamicFLSize
 *
 * @brief       This subroutine used to get the number of entries of the filter
 *              list.
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * @param       dynamicFL - pointer to dynamic filter list
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      The number of entries of the filter list.
 */
uint8 LL_DFL_GetDynamicFLSize( dynamicFL_t *dynamicFL );

/*******************************************************************************
 * @fn          LL_DFL_SetDynamicFLSize
 *
 * @brief       This subroutine used to set the number of entries of the filter
 *              list.
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * @param       dynamicFL - pointer to dynamic filter list
 * @param       size - The number of entries of the filter list
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_DFL_SetDynamicFLSize( dynamicFL_t *dynamicFL, uint8 size );

/*******************************************************************************
 * @fn          LL_DFL_GetDynamicFLEntries
 *
 * @brief       This subroutine used to get pointer to the entries of the
 *              filter list.
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * @param       dynamicFL - pointer to dynamic filter list
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      pointer to the filter list entries.
 */
dynamicFLEntry_t* LL_DFL_GetDynamicFLEntries( dynamicFL_t *dynamicFL );

/*******************************************************************************
 * @fn          rfBleDpl_GetRadioFLPtr
 *
 * @brief       This subroutine used to get pointer to the radio filter list.
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * @param       dynamicFL - pointer to dynamic filter list
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      pointer to the to the radio filter list.
 */

void* rfBleDpl_GetRadioFLPtr( dynamicFL_t *dynamicFL );
/*******************************************************************************
 * @fn          LL_DFL_Init
 *
 * @brief       This subroutine used to prepare the the rank table of the
 *              dynamic filter list and the dynamic filter list.
 *              The ranks in the rank table will set to DFL_SIZE.
 *              The entries in the dynamic filter list will set the addresses
 *              to 0x0, and their flags will be cleared.
 *
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * @param       pDynamicFL   - pointer to dynamic filter list
 * @param       pRankFLTable - pointer to the rank table of the dynamic filter
 *                             list.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS               - for success.
 *              LL_STATUS_ERROR_INVALID_PARAMS  - for invalid input.
 */
llStatus_t  LL_DFL_Init( dynamicFL_t       *pDynamicFL,
                         rankDynamicFL_t   *pRankFLTable );

/*******************************************************************************
 * @fn          LL_DFL_AddEntry
 *
 * @brief       This subroutine used to get index of available entry in the
 *              dynamic filter list.
 *              The subroutine will return index of an empty entry, and if all
 *              the entries in used, the subroutine will return the oldest
 *              entry (i.e the entry with the highest rank).
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * @param       pDynamicFL   - pointer to dynamic filter list
 * @param       pRankFLTable - pointer to the rank table of the dynamic filter
 *                             list.
 * @param       devAddr      - pointer to the device address.
 * @param       devAddrType  - address type of the device.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      The index of the new/replaced entry in the dynamic filter list.
 *              Valid index shall be in the range (0 - (DFL_SIZE-1)).
 *              DFL_SIZE index indicates that the input parameters invalid.
 */
uint8 LL_DFL_AddEntry( dynamicFL_t      *pDynamicFL,
                       rankDynamicFL_t  *pRankFLTable,
                       uint8            *devAddr,
                       uint8            devAddrType );

/*******************************************************************************
 * @fn          LL_DFL_RemoveEntry
 *
 * @brief       This subroutine used to remove an entry in the dynamicFL using
 *              the index of the entry.
 *
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * @param       pDynamicFL   - pointer to dynamic filter list.
 * @param       pRankFLTable - pointer to the rank table of the dynamic filter
 *                             list.
 * @param       indexEntry   - index entry to be removed in the dynamic filter
 *                             list. (shall be in the range 0-DFL_SIZE)
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      LL_STATUS_SUCCESS              - for success.
 *              LL_STATUS_ERROR_INVALID_PARAMS - for invalid input.
 */
llStatus_t LL_DFL_RemoveEntry( dynamicFL_t      *pDynamicFL,
                               rankDynamicFL_t  *pRankFLTable,
                               uint8            indexEntry );

/*******************************************************************************
 * @fn          LL_DFL_UpdateEntry
 *
 * @brief       This subroutine used to find and replace a RPA address in the
 *              dynamicFL with an updated RPA. If the old RPA not found in the
 *              dynamicFL, new entry will be added.
 *              The updated Entry rank will set to 0 in order to maintaining
 *              the LRU mechanism.
 *              The updates/new entry index will be return. Invalid index will
 *              be returned when the input parameters are invalid.
 *              The flag bits of the entry remains the same.
 *
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * @param       pDynamicFL   - pointer to dynamic filter list
 * @param       pRankFLTable - pointer to the rank table of the dynamic filter
 *                             list.
 * @param       oldRPA       - The old RPA address of the device.
 * @param       newRPA       - The new RPA address of the device.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      The index of the new/updated entry in the dynamic filter list.
 *              Valid index shall be in the range (0 - (DFL_SIZE-1)).
 *              DFL_SIZE index indicates that the input parameters invalid.
 */
uint8 LL_DFL_UpdateEntry( dynamicFL_t       *pDynamicFL,
                          rankDynamicFL_t   *pRankFLTable,
                          uint8             *oldRPA,
                          uint8             *newRPA );

/*******************************************************************************
 * @fn          LL_DFL_FindEntry
 *
 * @brief       This subroutine used to find an entry in the dynamic filter
 *              list by device address and address type.
 *              If found, the index of the entry will return. If not found, a
 *              fail value will return.
 *
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * @param       pDynamicFL   - pointer to dynamic filter list
 * @param       devAddr      - pointer to the device address.
 * @param       devAddrType  - address type of the device.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      The index of the found entry in the dynamic filter list.
 *              Valid index shall be in the range (0 - (DFL_SIZE-1)).
 *              DFL_SIZE index indicates that the input parameters invalid.
 */
uint8 LL_DFL_FindEntry( dynamicFL_t    *pDynamicFL,
                        uint8          *devAddr,
                        uint8          devAddrType);

/*******************************************************************************
 * @fn          LL_DFL_GetDynamicFilterlist
 *
 * @brief       This subroutine used to get pointer of the dynamic filter list.
 *
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      pointer to dynamic filter list.
 */
dynamicFL_t *LL_DFL_GetDynamicFilterlist( void );

/*******************************************************************************
 * @fn          LL_DFL_GetRankTable
 *
 * @brief       This subroutine used to get pointer to the rank table of the
 *              dynamic filter list.
 *
 *
 * @design      BLE_LOKI-355
 * input parameters
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      pointer to the rank table of the dynamic filter list.
 */
rankDynamicFL_t *LL_DFL_GetRankTable( void );

#endif /* LL_DFL_H */