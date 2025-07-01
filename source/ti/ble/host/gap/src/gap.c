/******************************************************************************

 @file  gap.c

 @brief This file contains the GAP Configuration API.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2011-2025, Texas Instruments Incorporated

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

#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/stack_util/osal/osal_snv.h"
#include "ti/ble/host/gap/gap.h"
#include "ti/ble/host/sm/sm.h"
#include "ti/ble/host/gapbondmgr/gapbondmgr_internal.h"
#include "ti/ble/host/gapbondmgr/gapbondmgr.h"
#include "ti/ble/host/gap/gap_internal.h"
#include "ti/ble/host/gap/gap_advertiser_internal.h"
#include "ti/ble/host/gap/gap_scanner_internal.h"
#include "ti/ble/stack_util/lib_opt/map_direct.h"
// Stub headers
#include "ti/ble/stack_util/lib_opt/host_stub_gap_bond_mgr.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * GLOBAL VARIABLES - These are available as part of the internal API
 *                (gap_internal.h), not part of the public API (gap.h)
 */
uint8 gapAppTaskID    = INVALID_TASK_ID; // default task ID to send events.
uint8 gapEndAppTaskID = INVALID_TASK_ID; // end application task ID to send events.
                                         // if bond manager is not enabled,
                                         // gapEndAppTaskID and gapAppTaskID
                                         // will be the same.

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */
void GAP_DeviceInit_per_role(uint8_t profileRole);

/*********************************************************************
 * API FUNCTIONS
 * OneBleLib: Duplicate GAP_DeviceInit without init gap bond manager - save flash size when not using GapBondMgr
 * OneBleLib comopiled as PERIPHERAL and CENTRAL:
 *         to privent for example init scan module in BROADCASTER
 */

/*********************************************************************
 * Validate correct role, address modes and init state
 */
bStatus_t GAP_DeviceInit_validate_params(uint8_t profileRole, uint8_t taskID,
                         GAP_Addr_Modes_t addrMode, uint8_t* pRandomAddr)
{
  bStatus_t stat = INVALIDPARAMETER;   // Return status

  // Ensure that initialization hasn't occurred
  if (MAP_gapGetInitState() == GAP_INITSTATE_READY)
  {
    return bleIncorrectMode;
  }

  // Check for valid address mode...
  if (
      (addrMode > ADDRMODE_RP_WITH_RANDOM_ID) ||
      // And if a random mode, that an address is passed in
      ((addrMode == ADDRMODE_RANDOM
        || addrMode == ADDRMODE_RP_WITH_RANDOM_ID
       ) && (pRandomAddr == NULL)))
  {
    return ( INVALIDPARAMETER );
  }

  // If using a random address, check that it is valid:
  if((addrMode == ADDRMODE_RANDOM
       || addrMode == ADDRMODE_RP_WITH_RANDOM_ID
      ) &&
     // If all bits excluding the 2 MSBs are all 0's...
     ((MAP_osal_isbufset(pRandomAddr, 0x00, B_ADDR_LEN - 1) &&
      ((pRandomAddr[B_ADDR_LEN - 1] & 0x3F) == 0)) ||
      // Or all bites are 1's
      MAP_osal_isbufset(pRandomAddr, 0xFF, B_ADDR_LEN) ||
      // Or the 2 MSBs are not 11b
      !(GAP_IS_ADDR_RS(pRandomAddr))))
  {
    // This is an invalid ramdom static address
    return( INVALIDPARAMETER );
  }

  // Valid profile roles and supported combinations
  switch ( profileRole )
  {
    case GAP_PROFILE_BROADCASTER:
      #if ( HOST_CONFIG & ( BROADCASTER_CFG | PERIPHERAL_CFG ) )
      {
        stat = SUCCESS;
      }
      #endif
      break;

    case GAP_PROFILE_OBSERVER:
      #if ( HOST_CONFIG & ( OBSERVER_CFG | CENTRAL_CFG ) )
      {
        stat = SUCCESS;
      }
      #endif
      break;

    case GAP_PROFILE_PERIPHERAL:
      #if ( HOST_CONFIG & PERIPHERAL_CFG )
      {
        stat = SUCCESS;
      }
      #endif
      break;

    case GAP_PROFILE_CENTRAL:
      #if ( HOST_CONFIG & CENTRAL_CFG )
      {
        stat = SUCCESS;
      }
      #endif
      break;

    case (GAP_PROFILE_BROADCASTER | GAP_PROFILE_OBSERVER):
      #if ( ( HOST_CONFIG & ( BROADCASTER_CFG | PERIPHERAL_CFG ) ) && \
            ( HOST_CONFIG & ( OBSERVER_CFG | CENTRAL_CFG ) ) )
      {
        stat = SUCCESS;
      }
      #endif
      break;

    case (GAP_PROFILE_PERIPHERAL | GAP_PROFILE_OBSERVER):
      #if ( ( HOST_CONFIG & PERIPHERAL_CFG ) && \
            ( HOST_CONFIG & ( OBSERVER_CFG | CENTRAL_CFG ) ) )
      {
        stat = SUCCESS;
      }
      #endif
      break;

    case (GAP_PROFILE_CENTRAL | GAP_PROFILE_BROADCASTER):
      #if ( ( HOST_CONFIG & CENTRAL_CFG ) && \
            ( HOST_CONFIG & ( BROADCASTER_CFG | PERIPHERAL_CFG ) ) )
      {
        stat = SUCCESS;
      }
      #endif
      break;

    case (GAP_PROFILE_CENTRAL | GAP_PROFILE_PERIPHERAL):
      #if ( HOST_CONFIG & ( CENTRAL_CFG | PERIPHERAL_CFG ) )
      {
        stat = SUCCESS;
      }
      #endif
      break;

    // Invalid profile roles
    default:
      stat = INVALIDPARAMETER;
      break;
  }
  return stat;
}

/*********************************************************************
 * Called to setup the device.  Call just once.
 *
 * Public function defined in gap.h.
 */
bStatus_t GAP_DeviceInit(uint8_t profileRole, uint8_t taskID,
                         GAP_Addr_Modes_t addrMode, uint8_t* pRandomAddr)
{
  bStatus_t stat = INVALIDPARAMETER;   // Return status

  // Validate correct role, address modes and init state
  stat = GAP_DeviceInit_validate_params(profileRole, taskID, addrMode, pRandomAddr);

  if (stat == SUCCESS)
  {

    stat = OPT_GAPBondMgr_UpdateRandomAddr( addrMode, pRandomAddr );
    if(stat != SUCCESS)
    {
      return (bleInternalError);
    }

    // Set the internal GAP address mode
    gapDeviceAddrMode = addrMode;

    // If own address type is random static or RPA with random static...
    if(gapDeviceAddrMode == ADDRMODE_RANDOM
       || gapDeviceAddrMode == ADDRMODE_RP_WITH_RANDOM_ID
      )
    {
      // If valid random static address, put it to the controller
      MAP_LL_SetRandomAddress(pRandomAddr);
    }

    // Set the task ID to receive GAP events.
    gapEndAppTaskID = taskID;

    // If using the gapbondmgr, use its taskID as default application task ID
    gapAppTaskID = OPT_GAPBondMgr_GetAuthTaskID();

    // If there is no gapbondmgr
    if(gapAppTaskID == 0)
    {
      // If bond manager is included in the build but not used,
      // use end application's task ID as the default appliation task ID
      gapAppTaskID = gapEndAppTaskID;
    }

    // Setup the device configuration parameters
    stat = MAP_gap_ParamsInit( profileRole );
    if ( stat == SUCCESS )
    {
      uint8_t tempKey[KEYLEN] = {0};

      // IRK is set based on the following rules:
      // 1. If the own address type is public or random static,
      //    set IRK to all 0's.
      // 2. If the own address type is neither public nor random static,
      //    2-1. If the IRK was set by user with GapConfig_SetParameter() before
      //         GAP_DeviceInit(), use it.
      //    2-2. If the IRK hasn't been set by user, read it from the NV.
      //         2-2-1. If the value is valid, use it.
      //         2-2-2. If not, generate a random value.
      //
      // SRK is set based on the following rules:
      // 1. If the SRK was set by user with GapConfig_SetParameter() before
      //    GAP_DeviceInit(), use it.
      // 2. If the SRK hasn't been set by user, read it from the NV.
      //    2-1. If the value is valid, use it.
      //    2-2. If not, generate a random value.

      if (gapDeviceAddrMode == ADDRMODE_PUBLIC ||
          gapDeviceAddrMode == ADDRMODE_RANDOM)
      {
        // If we are going to use only either public or random static address
        // as the own address, the IRK shall be all 0's.
        MAP_GapConfig_SetParameter(GAP_CONFIG_PARAM_IRK, tempKey);
      }
      else
      {
        OPT_GAPBondMgr_ReadIRKFromNV(tempKey);
      }

      OPT_GAPBondMgr_ReadCSRKFromNV(tempKey);

      // Init GAP security, privacy, advertising and scan per role
      GAP_DeviceInit_per_role(profileRole);
    }
  }

  return ( stat );
}

/*********************************************************************
 * Init GAP security, privacy, advertising and scan per role
 */
void GAP_DeviceInit_per_role(uint8_t profileRole)
{
      if ((profileRole & GAP_PROFILE_CENTRAL) || (profileRole & GAP_PROFILE_PERIPHERAL))
      //#if ( HOST_CONFIG & ( CENTRAL_CFG | PERIPHERAL_CFG ) )
      {
        // The signCounter parameter is not needed anymore since it is handled
        // by the gapbondmgr. The API is not changed in order to prevent
        // patching ROM
        MAP_gap_SecParamsInit(MAP_gapGetSRK(), NULL);
      }
      //#endif

      // Set IRK GAP Parameter
      MAP_gap_PrivacyInit(MAP_GAP_GetIRK());

#if ( HOST_CONFIG & ( CENTRAL_CFG | OBSERVER_CFG ) )
      if ((profileRole & GAP_PROFILE_CENTRAL) || (profileRole & GAP_PROFILE_OBSERVER))
      {
        // Initialize GAP Scanner module
        VOID MAP_gapScan_init();
#if ( HOST_CONFIG & CENTRAL_CFG )
        if (profileRole & GAP_PROFILE_CENTRAL)
        {
          // Register GAP Central Connection processing functions
          MAP_gap_CentConnRegister();

          // Initialize SM Initiator
          VOID MAP_SM_InitiatorInit();
        }
#endif
      }
#endif

#if ( HOST_CONFIG & ( PERIPHERAL_CFG | BROADCASTER_CFG ) )
      if ((profileRole & GAP_PROFILE_PERIPHERAL) || (profileRole & GAP_PROFILE_BROADCASTER))
      {
        // Initialize GAP Advertiser module
        VOID MAP_gapAdv_init();
#if ( HOST_CONFIG & PERIPHERAL_CFG )
        if (profileRole & GAP_PROFILE_PERIPHERAL)
        {
          // Register GAP Peripheral Connection processing functions
          MAP_gap_PeriConnRegister();

          // Initialize SM Responder
          VOID MAP_SM_ResponderInit();
        }
#endif
      }
#endif
}
/*********************************************************************
 * Public function defined in gap.h.
 */
void GAP_UpdateResolvingList(uint8_t *pIRK)
{
  // Sync bond records with Resolving List. Bond Manager also manages local
  // IRK in Resolving List
  gapBondMgr_syncResolvingList();
}

/*********************************************************************
*********************************************************************/
