/******************************************************************************

 @file  gapbondmgr_internal.h

 @brief This file contains internal interfaces for the gapbondmgr.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2025, Texas Instruments Incorporated

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
 * UNEXPECTED BEHAVIOR
 *
 * These API's shall begin with a lower-case letter
 */

#ifndef GAPBONDMGR_INTERNAL_H
#define GAPBONDMGR_INTERNAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/host/gap/gap.h"
#include "ti/ble/host/gapbondmgr/gapbondmgr.h"

/*********************************************************************
 * MACROS
 */

// Used calculate the index/offset in to NV space
#define CALC_NV_ID(Idx, offset)         (((((Idx) * GAP_BOND_REC_IDS) + (offset))) + BLE_NVID_GAP_BOND_START)
#define MAIN_RECORD_NV_ID(bondIdx)      (CALC_NV_ID((bondIdx), GAP_BOND_REC_ID_OFFSET))
#define LOCAL_LTK_NV_ID(bondIdx)        (CALC_NV_ID((bondIdx), GAP_BOND_LOCAL_LTK_OFFSET))
#define DEV_LTK_NV_ID(bondIdx)          (CALC_NV_ID((bondIdx), GAP_BOND_DEV_LTK_OFFSET))
#define DEV_IRK_NV_ID(bondIdx)          (CALC_NV_ID((bondIdx), GAP_BOND_DEV_IRK_OFFSET))
#define DEV_CSRK_NV_ID(bondIdx)         (CALC_NV_ID((bondIdx), GAP_BOND_DEV_CSRK_OFFSET))
#define DEV_SIGN_COUNTER_NV_ID(bondIdx) (CALC_NV_ID((bondIdx), GAP_BOND_DEV_SIGN_COUNTER_OFFSET))

// Used to calculate the GATT index/offset in to NV space
#define GATT_CFG_NV_ID(Idx)                    ((Idx) + BLE_NVID_GATT_CFG_START)

/*********************************************************************
 * CONSTANTS
 */

/**
 * @brief This parameter is only used by the Host Test app Application (network processor)
 *        The Id of this parameter is used to store the password locally.
 *        the GAP Bond manager is not storing any default password anymore,
 *        everytime a password is needed, pfnPasscodeCB_t will be call.
 *
 * @note an embedded application MUST used @ref pfnPasscodeCB_t
 *
 * size: uint32_t
 *
 * default: 0
 *
 * range: 0 - 999999
 */
#define GAPBOND_DEFAULT_PASSCODE      0x408

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

extern bStatus_t gapBondMgr_syncResolvingList(void);
extern uint8_t gapBondMgr_CheckNVLen(uint16_t id, uint8_t len);

/**
 * This should only be used in osal_icall_ble.c in osalInitTasks()
 *
 * @param task_id OSAL task ID
 */
extern void GAPBondMgr_Init(uint8_t task_id, uint8_t cfg_gapBond_maxBonds, uint8_t cfg_gapBond_maxCharCfg, uint8_t cfg_gapBond_gatt_no_client, uint8_t cfg_gapBond_gatt_no_service_changed);

/**
 * This should only be used in osal_icall_ble.c in tasksArr[]
 *
 * @param  task_id OSAL task ID
 * @param  events  OSAL event received
 *
 * @return bitmask of events that weren't processed
 */
extern uint32 GAPBondMgr_ProcessEvent(uint8_t task_id, uint32 events);

/**
 * Set/clear the service change indication in a bond record.
 *
 * @param connectionHandle connection handle of the connected device or 0xFFFF
 *                                 if all devices in database.
 * @param setParam TRUE to set the service change indication, FALSE to clear it.
 *
 * @return @ref SUCCESS bond record found and changed,
 * @return @ref bleNoResources bond record not found
 * @return @ref bleNotConnected connection not found connectionHandle is invalid
 *         (for non-0xFFFF connectionHandle).
 */
bStatus_t gapBondMgr_ServiceChangeInd(uint16_t connectionHandle,
                                             uint8_t setParam);

/**
 * Respond to a passcode request.
 *
 * @param connectionHandle connection handle of the connected device or 0xFFFF
 *        if all devices in database.
 * @param status SUCCESS if passcode is available, otherwise see
 *        @ref GAPBondMgr_Pairing_Failed.
 * @param passcode integer value containing the passcode.
 *
 * @return @ref SUCCESS bond record found and changed,
 * @return @ref bleIncorrectMode Link not found.
 * @return @ref INVALIDPARAMETER : passcode is out of range
 * @return @ref bleMemAllocError : heap is out of memory
 */
bStatus_t gapBondMgr_PasscodeRsp(uint16_t connectionHandle,
                                        uint8_t status, uint32_t passcode);

/**
 * @brief   Set remote's OOB parameters for secure connection
 *
 *
 * @param   remoteOobData - struct holding remote random number and confirm value
 * @param   OOBDataFlag - 1 - OOB Authentication data from remote device present, else 0
 *
 * @return  SUCCESS - Finished successfully.
 *          FAILURE - If OOBDataFlag is set but remote's OOB parameters are not
 *          or if OOBDataFlag is not set but OOB parameters are set.
 */
bStatus_t gapBondMgr_SCSetRemoteOOBParameters(gapBondOOBData_t *remoteOobData,
                                              uint8_t OOBDataFlag);

/**
 * @brief   Set remote's OOB parameters for secure connection
 *
 * @design /ref did_244389729
 *
 * @param   remoteOobData - struct holding remote random number and confirm value
 * @param   OOBDataFlag - 1 - OOB Authentication data from remote device present, else 0
 *
 * @return  SUCCESS - Finished successfully.
 *          FAILURE - If OOBDataFlag is set but remote's OOB parameters are not
 *          or if OOBDataFlag is not set but OOB parameters are set.
 */
bStatus_t gapBondMgr_SCGetLocalOOBParameters(gapBondOOBData_t *localOobData);

/**
 * Start (or re-start) the pairing process
 *
 * This API can be used to initiate pairing in cases where it is not started
 * automatically by the gapbondmgr or to re-pair after the initial pairing has
 * occurred if, for example, the pairing requirements have changed.
 *
 * @note Pairing will occur automatically if @ref GAPBOND_PAIRING_MODE is set to
 * @ref GAPBOND_PAIRING_MODE_INITIATE or @ref GAPBOND_PAIRING_MODE is set to
 * @ref GAPBOND_PAIRING_MODE_WAIT_FOR_REQ and a pairing request is received. In
 * these cases, this API is not needed and will fail since pairing is already
 * in progress.
 *
 * @param connHandle connection handle to initiate pairing with
 *
 * @return @ref SUCCESS
 * @return @ref bleAlreadyInRequestedMode already pairing
 */
bStatus_t gapBondMgr_Pair(uint16_t connHandle);

/**
 * @brief  Search for an address in the bonding table. If the address is a
 * random private resolvable address, attempt to resolve the random address
 * against all IRK's in bonding table. As output parameters, return the
 * following:
 * - index into bonding table if the address (or resolved address) is found
 * - identity address type if a resolved address was found. Only valid if the
 *   input address is random private resolvable
 * - identity address if a resolved address was found. Only valid if the
 *   input address is random private resolvable
 *
 * Note that the output parameters are optional. If it is not desired for them
 * to be returned, a NULL pointer can safely be passed.
 *
 * @param pDevAddr peer's address
 * @param addrType peer's address type
 * @param pIdx pointer to byte to put the bond index if found
 * @param pIdentityAddrType pointer to byte to put the identity address type
 * if applicable
 * @param pIdentityAddr pointer to buffer to put the identity address type
 * if applicable
 *
 * @return SUCCESS if the address was found or resolved to an address in the
 * bonding table
 * @return INVALIDPARAMETER if a NULL address is passed in
 * @return bleGAPNotFound if the address was not found and didn't resolve to any
 * addresses in the bonding table
 */
bStatus_t gapBondMgr_FindAddr(uint8_t *pDevAddr,
                              GAP_Peer_Addr_Types_t addrType,
                              uint8_t *pIdx,
                              GAP_Peer_Addr_Types_t *pIdentityAddrType,
                              uint8_t *pIdentityAddr);

/**
 * Get a GAP Bond Manager parameter.
 *
 * @note The "pValue" field must point to a "uint16_t".
 *
 * @param param Profile parameter ID: @ref GAPBondMgr_Params
 * @param pValue pointer to location to get the value.  This is dependent on
 *        the parameter ID and WILL be cast to the appropriate
 *        data type (example: data type of uint16_t will be cast to
 *        uint16_t pointer).
 *
 * @return @ref SUCCESS
 * @return @ref INVALIDPARAMETER
 */
bStatus_t gapBondMgr_GetParameter(uint16_t param, void *pValue);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GAPBONDMGR_INTERNAL_H */
