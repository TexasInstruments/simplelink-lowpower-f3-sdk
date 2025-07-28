/******************************************************************************

 @file  ll_dmm_dynamic_priority.h

 @brief This file contains the BLE Link Layer DMM Priority calculation and
        management functions.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025, Texas Instruments Incorporated

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

/*********************************************************************
 *
 * WARNING!!!
 *
 * THE API'S FOUND IN THIS FILE ARE FOR INTERNAL STACK USE ONLY!
 * FUNCTIONS SHOULD NOT BE CALLED DIRECTLY FROM APPLICATIONS, AND ANY
 * CALLS TO THESE FUNCTIONS FROM OUTSIDE OF THE STACK MAY RESULT IN
 * UNEXPECTED BEHAVIOR.
 *
 */


#ifndef LL_DMM_DYNAMIC_PRIORITY_H
#define LL_DMM_DYNAMIC_PRIORITY_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */

#include <string.h>
#include <stdint.h>
#include <ti/drivers/rcl/RCL.h>
#include <ti/drivers/rcl/commands/ble5.h>

#include "ti/ble/stack_util/osal/osal.h"
#include "ti/ble/controller/ll/ll.h"
#include "ti/ble/controller/ll/ll_scheduler.h"

/*******************************************************************************
* MACROS
*/
#define DMM_POLICY_EXTRACT_PRIORITY(x) ((x) & 0x03) // Extract priority from activity
#define DMM_ADV_SCAN_DURATION_TO_US(x) ((x) * 10000) // 10ms ticks
/*******************************************************************************
 * CONSTANTS
 */
#define DMM_POLICY_ACTIVITY_INDEX_INITIATING     0
#define DMM_POLICY_ACTIVITY_INDEX_CONNECTION     1
#define DMM_POLICY_ACTIVITY_INDEX_BROADCASTING   2
#define DMM_POLICY_ACTIVITY_INDEX_OBSERVING      3
#define DMM_POLICY_ACTIVITY_INDEX_PERIODIC_SCAN  4
#define DMM_POLICY_ACTIVITY_INDEX_PERIODIC_ADV   5

#define DMM_POLICY_ACTIVITY_INITIATING     (((DMM_POLICY_ACTIVITY_INDEX_INITIATING    + 1) * 1000) << 16)     //0x3E80000
#define DMM_POLICY_ACTIVITY_CONNECTION     (((DMM_POLICY_ACTIVITY_INDEX_CONNECTION    + 1) * 1000) << 16)     //0x7D00000
#define DMM_POLICY_ACTIVITY_BROADCASTING   (((DMM_POLICY_ACTIVITY_INDEX_BROADCASTING  + 1) * 1000) << 16)     //0xBB80000
#define DMM_POLICY_ACTIVITY_OBSERVING      (((DMM_POLICY_ACTIVITY_INDEX_OBSERVING     + 1) * 1000) << 16)     //0xFA00000
#define DMM_POLICY_ACTIVITY_PERIODIC_SCAN  (((DMM_POLICY_ACTIVITY_INDEX_PERIODIC_SCAN + 1) * 1000) << 16)     //0x13880000
#define DMM_POLICY_ACTIVITY_PERIODIC_ADV   (((DMM_POLICY_ACTIVITY_INDEX_PERIODIC_ADV  + 1) * 1000) << 16)     //0x17700000
#define DMM_POLICY_PRIORITY_NORMAL         0
#define DMM_POLICY_PRIORITY_HIGH           1
#define DMM_POLICY_PRIORITY_URGENT         2

#define DMM_POLICY_TIME_LIMIT_500MS        2000000   // Time in ticks
#define DMM_POLICY_TIME_LIMIT_1S           4000000   // Time in ticks
#define DMM_POLICY_TIME_LIMIT_2S           8000000   // Time in ticks

#define DMM_POLICY_50_PERCENT_LIMIT      50        // 50% of consecutive commands aborted out of total events
#define DMM_POLICY_80_PERCENT_LIMIT    80        // 80% of consecutive commands aborted out of total events
#define DMM_1_25MS_INTERVAL_US (1250) // 1.25ms interval
// Number of periodic adv events for a channel map update as baseline reference on priority calculation
#define DMM_PERIODIC_ADV_CHANMAP_UPDATE_NUM_EVENTS 7
#define DMM_PERIODIC_SCAN_FIRST_MISSED_EVENT 1
#define DMM_HANDLE_INVALID 0xFF
#define DMM_MAX_NUM_ABORTS 0xFF
#define DMM_THRESHOLD_RESET TRUE
#define DMM_THRESHOLD_NO_RESET FALSE

#define DMM_ADV_HANDLE_CLEAR TRUE
#define DMM_ADV_HANDLE_NO_CLEAR FALSE
/*******************************************************************************
 * TYPEDEFS
 */

// DMM Policy feature
typedef struct
{
  uint32 time;                                // Time passed from last successful transmission
  uint8_t aborts;                              // Number of consecutive aborted commands
  uint32_t advHandle;                         // Advertisement handle
} llDmmDynamicPriThresholdAdv_t;

typedef struct
{
  uint32 time;                                // Time passed from last successful transmission
  uint8_t aborts;                              // Number of consecutive aborted commands
} llDmmDynamicPriThreshold_t;

typedef struct
{
  llDmmDynamicPriThresholdAdv_t adv[AE_DEFAULT_NUM_ADV_SETS]; // Array of advertise data (maxSupportedAdvSets)
  llDmmDynamicPriThreshold_t  init;          // Create connection (1 instant)
  llDmmDynamicPriThreshold_t  scan;          // Scan (1 instant)
} llDmmDynamicPriManager_t;

// Dynamic Priority Test Callback Function Type
typedef void (*pDMMDynamicPriTestCBack_t)(RCL_Command *pCmd, uint8_t advId, uint32_t dynamicPriority);


/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */
void llDmmDynamicPri_init(void);
void llDmmDynamicPri_registerTestCb(pDMMDynamicPriTestCBack_t pCb);
uint8_t llDmmDynamicPri_isCmdPreempted(RCL_CommandStatus status);
void llDmmDynamicPri_SetAdvHandle(uint8 handle, uint8 clearHandle);
llDmmDynamicPriThresholdAdv_t *llDmmDynamicPri_GetAdvPriStruct(uint8 handle);
void llDmmDynamicPri_SetCmdPreemptionCounters(uint16_t cmdId, uint8_t advHandle, uint8_t reset);
void llDmmDynamicPri_SetDMMPriority(RCL_Command *cmd, uint32_t dmmActivityPriority);
void llDmmDynamicPri_SetCoexPriority(RCL_Command *cmd, RCL_Command_CoexPriority coexPriority);
RCL_Command_CoexPriority llDmmDynamicPri_GetCoexPriority(uint32_t dmmActivityPriority);
uint32_t llDMMDynamicPri_GetDMMPriority(RCL_Command *cmd);
void llDmmDynamicPri_updatePreemptionCounters(uint16_t cmdId, RCL_CommandStatus cmdStatus, uint8_t advHandle);
#ifdef __cplusplus
}
#endif

#endif /* LL_DMM_DYNAMIC_PRIORITY_H */
