/******************************************************************************

 @file dmm_scheduler.h

 @brief RCL based Dynamic Multi-Protocol Scheduler Interface

 Group: WCS LPC
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025-2026, Texas Instruments Incorporated

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

/**
 * @mainpage DMM Scheduler Overview
 *
 * The Dynamic Multi-Protocol Scheduler (DMM Scheduler) is responsible for
 * managing and scheduling commands from multiple protocol stacks in a
 * time-shared manner. The scheduler ensures that commands are executed
 * based on their priority, timing requirements, and the "allow delay"
 * parameter. The scheduling logic is designed to handle both absolute
 * time and now time commands.
 *
 * ### Command Insertion
 *
 * Commands are inserted into the DMM command queue based on their type:
 *
 * - **Absolute Timing Commands**:
 *   These commands are inserted in time order relative to other absolute
 *   timing commands. Priority is only checked at execution time, not during
 *   insertion.
 *
 * - **Now Time Commands**:
 *   These commands are scheduled in priority order between clients. For
 *   commands scheduled by the same client, FIFO (First-In-First-Out) ordering
 *   is always used to maintain execution consistency.
 *
 * ### Scheduling Logic
 *
 * Commands are prioritized and selected for execution based on the
 * following criteria:
 *
 * 1. **Allow Delay = False, Absolute Time**
 *    - Commands with absolute timing requirements and no allowance for delay
 *      are given the highest priority.
 *    - If selected, a priority check using the GPT is performed between the
 *      incoming command and the currently active command.
 *
 * 2. **Allow Delay = False, Now Time**
 *    - Commands that must execute immediately and cannot be delayed are
 *      prioritized after absolute time commands with no delay.
 *    - Similar to absolute time commands, a priority check is performed,
 *      and one command may be aborted if the check fails.
 *
 * 3. **Allow Delay = True, Absolute Time**
 *    - Commands with absolute timing requirements but that allow delay
 *      are considered next.
 *    - These commands may be delayed if higher-priority commands are
 *      present in the queue, but will still go through a priority check
 *      once the execution time arrives.
 *
 * 4. **Allow Delay = True, Now Time**
 *    - Commands that can execute immediately but allow indefinite delay
 *      are given the lowest priority.
 *    - These commands will only execute if no higher-priority commands
 *      are pending and cannot preempt currently running commands.
 *
 * ### Same Client Commands
 *
 * - Commands scheduled by the same client are not preempted if the
 *   incoming command has `allowDelay = true`.
 * - In all other cases, a priority check is performed, and one of the
 *   commands may be preempted.
 *
 */

#ifndef ti_dmm_scheduler_h__include
#define ti_dmm_scheduler_h__include

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <ti/drivers/rcl/LRF.h>
#include <ti/drivers/RCL.h>
#include <ti/drivers/rcl/RCL_Scheduler.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/utils/List.h>
#include <ti/drivers/rcl/RCL_Command.h>
#include <ti/drivers/rcl/RCL_Buffer.h>
#include <ti/drivers/rcl/commands/ieee.h>
#include <dmm_policy.h>


/**
 *  @brief Defines the preemption callback.
 *
 *  The function is invoked when a preemption occurs in DMM Scheduler.
 *
 * @param stackRolePreempted    Stack role for command was preempted.
 */
typedef void (*DMMSch_PreemptionCb)(DMMPolicy_StackRole stackRolePreempted);

/* End internal definitions */

/** @brief  Function that initializes the DMMSch module
 *
 * @return true: DMM Scheduler successfully intitialized, false: parameter error, DMM Scheduler cannot be initialized
 */
bool DMMSch_init(void);

/**
 * @brief Register a DMM Scheduler client.
 *
 * This function registers a client with the DMM Scheduler, associating it with a specific stack role
 * and stack ID. It also updates the Policy Manager with the stack ID and role information.
 *
 * @param c         Pointer to the RCL client.
 * @param stackRole Stack role associated with the client.
 * @param stackId   Unique stack ID associated with the client.
 *
 * @return true if the client was successfully registered, false otherwise.
 */
bool DMMSch_registerClient(RCL_Client *c, DMMPolicy_StackRole stackRole, DMM_stackID stackId);

/**
 * @brief De-register a DMM Scheduler client.
 *
 * This function removes all cached data regarding a client from the DMM Scheduler.
 * Note; RCL_close() should also be called if the stack is not to schedule any new RCL commands.
 *
 * @param h Handle to the RCL client to be de-registered.
 *
 * @return true if the client was successfully de-registered, false otherwise.
 */
bool DMMSch_deregisterClient(RCL_Handle h);

/**
 * @brief Initializes the RCL driver state
 *
 * Resets global state and initialize hardware.
 *
 * @note Must be called before using any other RCL API
 */
void DMMSch_RCL_init(void);

/**
 * @brief Initializes an RCL client instance
 *
 * @param[in] c - Client object struct to be initialized
 * @param[in] lrfConfig - Radio configuration to be used by client
 *
 * @return Instance %RCL_Handle handle or NULL
 */
RCL_Handle DMMSch_RCL_open(RCL_Client *c, const LRF_Config *lrfConfig);

/**
 * @brief Closes client instance and deallocates open resources
 *
 * @param[in] h - Client handle
 */
void DMMSch_RCL_close(RCL_Handle h);

/**
 * @brief Submit RCL command object to be scheduled for execution
 *
 * This API returns immediately with either %RCL_CommandStatus_Error or the asynchronous
 * current state of the command.
 *
 * @param[in] h - Client handle
 * @param[in] c - Command handle
 *
 * @return %RCL_CommandStatus result of the submission
 */
RCL_CommandStatus DMMSch_RCL_Command_submit(RCL_Handle h, RCL_Command_Handle c);

/**
 * @brief Wait for a submitted command to complete.
 *
 * Uses %SemaphoreP_pend to block in the callers context.
 *
 * @pre This function must be called from a task context, with interrupts enabled.
 *
 * @param[in] c - Client handle
 */
RCL_CommandStatus DMMSch_RCL_Command_pend(RCL_Command_Handle c);

/**
 * @brief Stop a command
 *
 * Sends the message to try to stop a command. When the function returns, the command may still be
 * running. Depending on the stop type, the command may stop after some time. %RCL_Command_pend
 * may be used to wait for the command to finish.
 *
 * @param c [in] - Command handle
 * @param stopType [in] - Stop type; telling which situations the command will stop
 *
 * @return Status of the command; if the command is not finished, a wait is needed.
 */
RCL_CommandStatus DMMSch_RCL_Command_stop(RCL_Command_Handle c, RCL_StopType stopType);

/* DMM Scheduler API's utilized by the DMM Policy Manager */

/** @brief  Turn on Block mode
 *
 *  @param  stackRole   stack role associated with Task handle
 *  @return true: success, false: the stack role cannot be found
 */
bool DMMSch_setBlockModeOn(DMMPolicy_StackRole stackRole);

/** @brief  Turn off Block mode
 *
 *  @param  stackRole   stack role associated with Task handle
 *  @return true: success, false: the stack role cannot be found
 */
bool DMMSch_setBlockModeOff(DMMPolicy_StackRole stackRole);

/** @brief  Get Block mode status
 *
 *  @param  stackRole   stack role associated with Task handle
 *  @return true: Block Mode is On, False: Block Mode is Off
 */
bool DMMSch_getBlockModeStatus(DMMPolicy_StackRole stackRole);

/** @brief  allows policy manager to register a callback on command preemption
 *
 *  @param  dmmSchPreemptionCb     callback to register
 */
void DMMSch_registerPreemptionCb(DMMSch_PreemptionCb dmmSchPreemptionCb);

/* IEEE API overrides */

/**
 * @brief Update the RX action for an IEEE command.
 *
 * @param cmd Pointer to the IEEE RX/TX command structure.
 * @return Result of the update operation.
 */
RCL_IEEE_UpdateResult DMMSch_RCL_IEEE_updateRxAction(RCL_CmdIeeeRxTx *cmd);

/**
 * @brief Update the short source matching table for an IEEE command.
 *
 * @param cmd Pointer to the IEEE RX/TX command structure.
 * @param description Description of the operation to perform.
 * @param newPanIdAddr New PAN ID and address to set if the entry is changed.
 * @return Result of the update operation.
 */
RCL_IEEE_UpdateResult DMMSch_RCL_IEEE_updateSourceMatchingTableShort(RCL_CmdIeeeRxTx *cmd, RCL_IEEE_SourceMatchingUpdate description, RCL_CmdIeee_PanIdAddr newPanIdAddr);

/**
 * @brief Update the extended source matching table for an IEEE command.
 *
 * @param cmd Pointer to the IEEE RX/TX command structure.
 * @param description Description of the operation to perform.
 * @param newAddr Pointer to the new address to set if the entry is changed.
 * @return Result of the update operation.
 */
RCL_IEEE_UpdateResult DMMSch_RCL_IEEE_updateSourceMatchingTableExt(RCL_CmdIeeeRxTx *cmd, RCL_IEEE_SourceMatchingUpdate description, const uint64_t *newAddr);

/**
 * @brief Update the TX power for an IEEE command.
 *
 * @param cmd Pointer to the IEEE RX/TX command structure.
 * @param newTxPower New TX power value to set.
 * @return Result of the update operation.
 */
RCL_IEEE_UpdateResult DMMSch_RCL_IEEE_updateTxPower(RCL_CmdIeeeRxTx *cmd, RCL_Command_TxPower newTxPower);

/**
 * @brief Provide ACK data to be transmitted in response to a received frame.
 *
 * @param cmd Pointer to the IEEE RX/TX command structure.
 * @param ackData Pointer to the data to be appended.
 * @param numWords Number of 32-bit words provided.
 * @return Result of the ACK entry operation.
 */
RCL_IEEE_AckEntryResult DMMSch_RCL_IEEE_enterAck(RCL_CmdIeeeRxTx *cmd, uint32_t *ackData, uint8_t numWords);

/**
 * @brief Cancel the transmission of an ACK in response to a received frame.
 *
 * @param cmd Pointer to the IEEE RX/TX command structure.
 * @return Result of the cancel operation.
 */
RCL_IEEE_AckEntryResult DMMSch_RCL_IEEE_cancelAck(RCL_CmdIeeeRxTx *cmd);

/**
 * @brief Read a partially received frame into a buffer.
 *
 * @param cmd Pointer to the IEEE RX/TX command structure.
 * @param dataEntry Pointer to the buffer entry for storing data.
 * @param entrySize Number of bytes available in the entry.
 * @return Number of bytes read into the entry.
 */
size_t DMMSch_RCL_IEEE_readPartialFrame(RCL_CmdIeeeRxTx *cmd, RCL_Buffer_DataEntry *dataEntry, size_t entrySize);

/**
 * @brief Submit a TX action for an IEEE command.
 *
 * @param cmd Pointer to the IEEE RX/TX command structure.
 * @param txAction Pointer to the TX action to apply.
 * @return Status of the submission operation.
 */
RCL_CommandStatus DMMSch_RCL_IEEE_Tx_submit(RCL_CmdIeeeRxTx *cmd, RCL_CmdIeee_TxAction *txAction);

/**
 * @brief Stop a TX action in an IEEE command.
 *
 * @param cmd Pointer to the IEEE RX/TX command structure.
 * @param stopType Type of stop operation to perform.
 * @return Status of the stop operation.
 */
RCL_CommandStatus DMMSch_RCL_IEEE_Tx_stop(RCL_CmdIeeeRxTx *cmd, RCL_StopType stopType);

#endif