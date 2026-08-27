/******************************************************************************

 @file  ll_ts_periodic.h

 @brief This file contains the Link Layer (LL) types, constants,
        API's etc. for the Bluetooth Low Energy (BLE) Controller
        Periodic Advertising Time Sync.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2026, Texas Instruments Incorporated

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

#ifndef LL_TS_PERIODIC_H
#define LL_TS_PERIODIC_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "ti/ble/controller/ll/ll_ae.h"
#include "ti/ble/controller/ll/ll_padv_advertiser.h"

/*******************************************************************************
 * MACROS
 ******************************************************************************/

/*******************************************************************************
 * CONSTANTS
 ******************************************************************************/

// Invalid advertising handle for TSA
#define LL_TS_TSA_INVALID_ADV_HANDLE          0xFF
// Invalid sync handle for TSO
#define LL_TS_TSO_INVALID_SYNC_HANDLE         0xFFFF

// Size of time sync data in periodic advertising (4 bytes for uint32_t time value)
#define LL_PADV_TS_DATA_SIZE                  4   

/*******************************************************************************
 * TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/

/*******************************************************************************
 * @fn          LL_TS_startTSO
 *
 * @brief       Start the TSO side of time sync by applying the syncHandle
 *
 * @param       syncHandle - Sync handle to receive events for (0xFFFF for all)
 *
 * @return      None
 */
void LL_TS_startTSO( uint16_t syncHandle );

/*******************************************************************************
 * @fn          LL_TS_calcPadvTimeDiff
 *
 * @brief       Calculate the time difference between the timestamp embedded
 *              in the first 4 bytes of pPkt and the given timeStamp, then
 *              store the result back in pPkt. The packet length must be at
 *              least 4 bytes to contain valid time sync data.
 *
 * input parameters
 *
 * @param       timeStamp  - Reference timestamp to subtract from pPkt time.
 * @param       syncHandle - Sync handle to match against registered handle.
 * @param       pPkt       - Pointer to packet data (first 4 bytes hold time).
 * @param       pktLen     - Length of the packet data in bytes.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_TS_calcPadvTimeDiff( uint32_t timeStamp, uint16_t syncHandle, uint8_t* pPkt, uint16_t pktLen );

/*******************************************************************************
 * TSA (Time Sync Advertiser) Functions
 ******************************************************************************/

/*******************************************************************************
 * @fn          LL_TS_startTSA
 *
 * @brief       Enable time sync for a specific periodic advertising handle.
 *              When enabled, the absStartTime will be included in the periodic
 *              advertising data.
 *
 * input parameters
 *
 * @param       advHandle - Advertising handle to enable time sync for.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_TS_startTSA( uint8_t advHandle );

/*************************************************************
 * @fn          LL_TS_isEnabled
 *
 * @brief       Check if time sync is enabled for a specific advertising handle.
 *
 * input parameters
 *
 * @param       advHandle - Advertising handle to check.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      TRUE if time sync is enabled for the specified handle,
 *              FALSE otherwise.
 */
uint8_t LL_TS_isEnabled( uint8_t advHandle );

/*******************************************************************************
 * @fn          LL_TS_prepareTimeSyncPacket
 *
 * @brief       Prepare the time sync packet by reserving space for time sync
 *              data in the common extended packet format. This sets pAdvData
 *              and advDataLen - the actual time value will be written later
 *              by LL_TS_insertTimeSyncData.
 *
 * input parameters
 *
 * @param       advHandle - Advertising handle.
 * @param       pComPkt   - Pointer to the common extended packet format.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_TS_prepareTimeSyncPacket( uint8_t advHandle, comExtPktFormat_t *pComPkt );

/*******************************************************************************
 * @fn          LL_TS_insertTimeSyncData
 *
 * @brief       Insert the time sync data into an already-built TX packet.
 *              This writes the actual over-the-air start time into the advData
 *              section of the AUX_SYNC_IND packet. The commanded start time
 *              (pPeriodicAdv->startTime) is corrected by the RCL start-time
 *              delay (PLL lock + ramp + sync overhead) so the transmitted
 *              timestamp reflects when the first bit actually leaves the
 *              antenna, not when the command was scheduled. Should be called
 *              after the packet is built and before transmission (e.g., missed
 *              events case, or just before TX).
 *
 * input parameters
 *
 * @param       pPeriodicAdv - Periodic advertising set. The handle, commanded
 *                             start time, RCL command, and TX buffer are all
 *                             derived from it.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_TS_insertTimeSyncData( const llPeriodicAdvSet_t *pPeriodicAdv );

#ifdef __cplusplus
}
#endif

#endif /* LL_TS_PERIODIC_H */
