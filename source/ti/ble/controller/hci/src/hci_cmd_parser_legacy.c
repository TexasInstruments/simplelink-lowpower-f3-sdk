/******************************************************************************

 @file  hci_cmd_parser_legacy.c

 @brief This file contains the legacy HCI command mode checking logic for
        preventing mixing of BT4 (legacy) and BT5 (extended) advertising
        and scanning commands.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2026, Texas Instruments Incorporated

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

/*******************************************************************************
 * INCLUDES
 */

#include "ti/ble/controller/hci/hci.h"
#include "ti/ble/controller/hci/hci_tl.h"

/*******************************************************************************
 * CONSTANTS
 */

#define HCI_LEGACY_CMD_STATUS_UNDEFINED            0
#define HCI_LEGACY_CMD_STATUS_BT4_ADV              1
#define HCI_LEGACY_CMD_STATUS_BT5_ADV              2
#define HCI_LEGACY_CMD_STATUS_BT4_SCAN             3
#define HCI_LEGACY_CMD_STATUS_BT5_SCAN             4

/*******************************************************************************
 * GLOBAL VARIABLES
 */

// Store the type of command that are used.
static uint8_t legacyCmdStatusAdv = HCI_LEGACY_CMD_STATUS_UNDEFINED;
static uint8_t legacyCmdStatusScan = HCI_LEGACY_CMD_STATUS_UNDEFINED;

/*******************************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      checkLegacyHCICmdMode
 *
 * @brief   Check command opcode and return operation mode.
 *
 * @param   opcode - command opcode
 *
 * @return  operation mode (HCI_LEGACY_CMD_STATUS_*)
 */
static uint8_t checkLegacyHCICmdMode(uint16_t opcode)
{
  uint8_t rtnVal = HCI_LEGACY_CMD_STATUS_UNDEFINED;

  switch(opcode)
  {
    case HCI_LE_SET_ADV_PARAM:
    case HCI_LE_READ_ADV_CHANNEL_TX_POWER:
    case HCI_LE_SET_ADV_DATA:
    case HCI_LE_SET_SCAN_RSP_DATA:
    case HCI_LE_SET_ADV_ENABLE:
    {
      rtnVal = HCI_LEGACY_CMD_STATUS_BT4_ADV;
      break;
    }
    case HCI_LE_SET_EXT_ADV_PARAMETERS:
    case HCI_LE_SET_EXT_ADV_DATA:
    case HCI_LE_SET_EXT_SCAN_RESPONSE_DATA:
    case HCI_LE_SET_EXT_ADV_ENABLE:
    case HCI_LE_READ_MAX_ADV_DATA_LENGTH:
    case HCI_LE_READ_NUM_SUPPORTED_ADV_SETS:
    case HCI_LE_REMOVE_ADV_SET:
    case HCI_LE_CLEAR_ADV_SETS:
    case HCI_LE_SET_PERIODIC_ADV_PARAMETERS:
    case HCI_LE_SET_PERIODIC_ADV_DATA:
    case HCI_LE_SET_PERIODIC_ADV_ENABLE:
    case HCI_EXT_LE_SET_EXT_ADV_DATA:
    {
      rtnVal = HCI_LEGACY_CMD_STATUS_BT5_ADV;
      break;
    }
    case HCI_LE_SET_SCAN_PARAM:
    case HCI_LE_SET_SCAN_ENABLE:
    case HCI_LE_CREATE_CONNECTION:
    {
      rtnVal = HCI_LEGACY_CMD_STATUS_BT4_SCAN;
      break;
    }
    case HCI_LE_SET_EXT_SCAN_PARAMETERS:
    case HCI_LE_SET_EXT_SCAN_ENABLE:
    case HCI_LE_EXT_CREATE_CONN:
    case HCI_LE_PERIODIC_ADV_CREATE_SYNC:
    case HCI_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL:
    case HCI_LE_PERIODIC_ADV_TERMINATE_SYNC:
    case HCI_LE_ADD_DEVICE_TO_PERIODIC_ADV_LIST:
    case HCI_LE_REMOVE_DEVICE_FROM_PERIODIC_ADV_LIST:
    case HCI_LE_CLEAR_PERIODIC_ADV_LIST:
    case HCI_LE_READ_PERIODIC_ADV_LIST_SIZE:
    {
      rtnVal = HCI_LEGACY_CMD_STATUS_BT5_SCAN;
      break;
    }
    default:
    {
      rtnVal = HCI_LEGACY_CMD_STATUS_UNDEFINED;
      break;
    }
  }
  return rtnVal;
}

/*******************************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      HCI_LegacyCmd_PreCheck
 *
 * @brief   Pre-check legacy command compatibility before parsing.
 *          Prevents mixing of BT4 (legacy) and BT5 (extended) commands
 *          for advertising and scanning operations.
 *
 * @param   cmdOpCode - HCI command opcode
 *
 * @return  HCI_SUCCESS: Command mode is valid
 *          LL_STATUS_ERROR_COMMAND_DISALLOWED: Command mode mixing detected
 */
hciStatus_t HCI_LegacyCmd_PreCheck(uint16_t cmdOpCode)
{
  uint8_t mode = checkLegacyHCICmdMode(cmdOpCode);

  // Check advertising commands
  if ((mode == HCI_LEGACY_CMD_STATUS_BT4_ADV) || (mode == HCI_LEGACY_CMD_STATUS_BT5_ADV))
  {
    if (legacyCmdStatusAdv == HCI_LEGACY_CMD_STATUS_UNDEFINED)
    {
      // First advertising command - set the mode
      legacyCmdStatusAdv = mode;
      return HCI_SUCCESS;
    }
    else
    {
      // Check if mode matches the established mode
      if (legacyCmdStatusAdv != mode)
      {
        // Mode mismatch - reject command
        return LL_STATUS_ERROR_COMMAND_DISALLOWED;
      }
      return HCI_SUCCESS;
    }
  }
  // Check scanning commands
  else if ((mode == HCI_LEGACY_CMD_STATUS_BT4_SCAN) || (mode == HCI_LEGACY_CMD_STATUS_BT5_SCAN))
  {
    if (legacyCmdStatusScan == HCI_LEGACY_CMD_STATUS_UNDEFINED)
    {
      // First scanning command - set the mode
      legacyCmdStatusScan = mode;
      return HCI_SUCCESS;
    }
    else
    {
      // Check if mode matches the established mode
      if (legacyCmdStatusScan != mode)
      {
        // Mode mismatch - reject command
        return LL_STATUS_ERROR_COMMAND_DISALLOWED;
      }
      return HCI_SUCCESS;
    }
  }
  else
  {
    // Command is not a legacy-checked command
    return HCI_SUCCESS;
  }
}

/*********************************************************************
 * @fn      HCI_LegacyCmd_Reset
 *
 * @brief   Reset the legacy command mode tracking.
 *
 * @return  None
 */
void HCI_LegacyCmd_Reset(void)
{
  legacyCmdStatusAdv = HCI_LEGACY_CMD_STATUS_UNDEFINED;
  legacyCmdStatusScan = HCI_LEGACY_CMD_STATUS_UNDEFINED;
}

/*********************************************************************
*********************************************************************/
