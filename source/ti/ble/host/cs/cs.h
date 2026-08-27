/******************************************************************************

 @file  cs.h

 @brief This file contains the structures used by Channel Sounding.

 Group: WCS, BTS
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
#ifndef CS_H
#define CS_H
/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/cs_types.h"
#include "ti/ble/stack_util/lib_opt/ctrl_stub_cs.h"

/*******************************************************************************
 * FORWARD DECLARATIONS
 */
// Forward declaration for CS configuration structure (fully defined in ll_cs_common.h)
// The typedef csConfigurationSet_t is defined in ll_cs_common.h
struct csConfig_t;

/**
 * @defgroup CS_API Channel Sounding API
 * @brief BLE Channel Sounding (CS) Host API for distance measurement and ranging.
 *
 * @{
 *
 * @section cs_overview Overview
 *
 * Channel Sounding is a BLE 6.0 feature for distance measurement/ranging between
 * two connected BLE devices. It uses a combination of Round-Trip Time (RTT) and
 * phase-based measurements to calculate distance with high accuracy.
 *
 * Typical use cases include:
 * - Digital car keys (Key Node <-> Car Node)
 * - Asset tracking
 * - Indoor positioning
 * - Secure proximity detection
 *
 * @section cs_roles Roles
 *
 * - @b Initiator (@ref CS_ROLE_INITIATOR): Starts the CS procedure, sends CS packets first
 * - @b Reflector (@ref CS_ROLE_REFLECTOR): Responds to CS signals from initiator
 *
 * @section cs_modes CS Modes
 *
 * | Mode | Purpose | Data Collected |
 * |------|---------|----------------|
 * | Mode 0 | Synchronization/calibration | Frequency offset, RSSI |
 * | Mode 1 | RTT (Round-Trip Time) | ToD-ToA timing |
 * | Mode 2 | Phase-based measurement | I/Q samples for phase calculation |
 * | Mode 3 | Combined RTT + Phase | Both ToD-ToA and I/Q samples |
 *
 * @section cs_antenna Antenna Configuration Index (ACI)
 *
 * Supports up to 4 antennas on each device for multi-antenna ranging:
 *
 * | ACI Value | Configuration | Description |
 * |-----------|---------------|-------------|
 * | @ref ACI_A1_B1 (0) | 1x1 | Single antenna on both devices |
 * | @ref ACI_A2_B1 (1) | 2x1 | 2 antennas initiator, 1 reflector |
 * | @ref ACI_A3_B1 (2) | 3x1 | 3 antennas initiator, 1 reflector |
 * | @ref ACI_A4_B1 (3) | 4x1 | 4 antennas initiator, 1 reflector |
 * | @ref ACI_A1_B2 (4) | 1x2 | 1 antenna initiator, 2 reflector |
 * | @ref ACI_A1_B3 (5) | 1x3 | 1 antenna initiator, 3 reflector |
 * | @ref ACI_A1_B4 (6) | 1x4 | 1 antenna initiator, 4 reflector |
 * | @ref ACI_A2_B2 (7) | 2x2 | 2 antennas on both devices |
 *
 * @section cs_flow API Call and Event Flow
 *
 * The following diagram shows the sequence of API calls and corresponding events
 * required to perform a CS procedure and receive measurement results.
 *
 * @note Phases 1-4 are one-time setup. To re-trigger subsequent CS procedures,
 *       only @ref CS_ProcedureEnable needs to be called again.
 *
 * @subsection cs_flow_diagram Flow Diagram
 *
 * @code{.unparsed}
 *
 *   APPLICATION START
 *         |
 *         v
 *   CS_RegisterCB()  ---> Register callback for CS events
 *         |
 *         |
 *   ======|====== PHASE 1: CAPABILITY EXCHANGE (ONE-TIME) ======
 *         |
 *         v
 *   CS_ReadLocalSupportedCapabilities()
 *         |
 *         +---> (No event - returns synchronously)
 *         |
 *         v
 *   CS_ReadRemoteSupportedCapabilities()
 *         |
 *         +---> CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT
 *         |     [CS_readRemoteCapabEvt_t]
 *         |
 *         |
 *   ======|====== PHASE 2: SECURITY SETUP (ONE-TIME) ======
 *         |
 *         v
 *   CS_SecurityEnable()
 *         |
 *         +---> CS_SECURITY_ENABLE_COMPLETE_EVENT
 *         |     [CS_securityEnableCompleteEvt_t]
 *         |
 *         |
 *   ======|====== PHASE 3: CONFIGURATION (ONE-TIME) ======
 *         |
 *         v
 *   CS_SetDefaultSettings()
 *         |
 *         +---> (No event - command complete only)
 *         |
 *         v
 *   CS_ReadRemoteFAETable()
 *         |
 *         +---> CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT
 *         |     [CS_readRemFAECompleteEvt_t]
 *         |
 *         v
 *   CS_CreateConfig()
 *         |
 *         +---> CS_CONFIG_COMPLETE_EVENT
 *         |     [CS_configCompleteEvt_t]
 *         |
 *         |
 *   ======|====== PHASE 4: PROCEDURE PARAMETERS (ONE-TIME) ======
 *         |
 *         v
 *   CS_SetProcedureParameters()
 *         |
 *         +---> (No event - command complete only)
 *         |
 *         |
 *   ======|====== PHASE 5: PROCEDURE EXECUTION (REPEATABLE) ======
 *         |
 *         v
 *   +---> CS_ProcedureEnable() <----------------------------------+
 *   |     |                                                       |
 *   |     +---> CS_PROCEDURE_ENABLE_COMPLETE_EVENT                |
 *   |     |     [CS_procEnableCompleteEvt_t]                      |
 *   |     |                                                       |
 *   |     |                                                       |
 *   | ====|====== PHASE 6: RESULTS ======                         |
 *   |     |                                                       |
 *   |     v                                                       |
 *   | (CS Procedure Executes on Radio)                            |
 *   |     |                                                       |
 *   |     +---> CS_SUBEVENT_RESULT                                |
 *   |     |     [CS_subeventResultsEvt_t]                         |
 *   |     |     Contains: ToD-ToA, I/Q samples, RSSI, etc.        |
 *   |     |                                                       |
 *   |     +---> CS_SUBEVENT_CONTINUE_RESULT (if fragmented)       |
 *   |     |     [CS_subeventResultsContinueEvt_t]                 |
 *   |     |                                                       |
 *   |     v                                                       |
 *   | (Repeat subevents until procedureDoneStatus ==              |
 *   |  CS_PROCEDURE_DONE or CS_PROCEDURE_ABORTED)                 |
 *   |     |                                                       |
 *   |     v                                                       |
 *   | [Procedure Complete]                                        |
 *   |     |                                                       |
 *   |     +--- To run another CS procedure -----------------------+
 *   |          (Only CS_ProcedureEnable needed!)
 *   |
 *   +--- END (or continue with other operations)
 *
 * @endcode
 *
 * @subsection cs_retrigger Re-triggering CS Procedures
 *
 * After the initial setup (Phases 1-4), subsequent CS procedures can be
 * triggered by simply calling @ref CS_ProcedureEnable again. The configuration,
 * security, and capability exchange do not need to be repeated.
 *
 * @code{.c}
 * // After initial setup is complete and first procedure finished...
 *
 * // To run another CS procedure, just call:
 * CS_setProcedureEnableCmdParams_t enableParams = {
 *     .connHandle = connHandle,
 *     .configID = 0,
 *     .enable = CS_ENABLE
 * };
 * CS_ProcedureEnable(&enableParams);
 * // Wait for CS_PROCEDURE_ENABLE_COMPLETE_EVENT
 * // Then receive CS_SUBEVENT_RESULT events
 * @endcode
 *
 * @subsection cs_api_event_table API to Event Mapping
 *
 * | Phase | API Function | Async Event | Event Structure |
 * |-------|--------------|-------------|-----------------|
 * | 1 (one-time) | @ref CS_ReadLocalSupportedCapabilities | None (sync) | N/A |
 * | 1 (one-time) | @ref CS_ReadRemoteSupportedCapabilities | @ref CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT | @ref CS_readRemoteCapabEvt_t |
 * | 2 (one-time) | @ref CS_SecurityEnable | @ref CS_SECURITY_ENABLE_COMPLETE_EVENT | @ref CS_securityEnableCompleteEvt_t |
 * | 3 (one-time) | @ref CS_SetDefaultSettings | None | N/A |
 * | 3 (one-time) | @ref CS_ReadRemoteFAETable | @ref CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT | @ref CS_readRemFAECompleteEvt_t |
 * | 3 (one-time) | @ref CS_CreateConfig | @ref CS_CONFIG_COMPLETE_EVENT | @ref CS_configCompleteEvt_t |
 * | 4 (one-time) | @ref CS_SetProcedureParameters | None | N/A |
 * | 5 (repeatable) | @ref CS_ProcedureEnable | @ref CS_PROCEDURE_ENABLE_COMPLETE_EVENT | @ref CS_procEnableCompleteEvt_t |
 * | 6 (results) | (Radio executes) | @ref CS_SUBEVENT_RESULT | @ref CS_subeventResultsEvt_t |
 * | 6 (results) | (Fragmented) | @ref CS_SUBEVENT_CONTINUE_RESULT | @ref CS_subeventResultsContinueEvt_t |
 *
 * @note To trigger subsequent CS procedures after initial setup, only call
 *       @ref CS_ProcedureEnable (Phase 5). Phases 1-4 do not need to be repeated.
 *
 * @subsection cs_step_data Step Data Structures
 *
 * The @ref CS_subeventResultsEvt_t contains an array of step results. Each step
 * has a 3-byte header (@ref csSubeventResultsStep_t) followed by mode-specific data:
 *
 * @code{.unparsed}
 *
 *   csSubeventResultsStep_t (3-byte header + variable data)
 *   +-- stepMode      (Mode 0, 1, 2, or 3)
 *   +-- stepChnl      (Channel used)
 *   +-- stepDataLen   (Length of step data)
 *   +-- stepData      (One of the following based on mode/role):
 *
 *       MODE 0 - Initiator: CS_modeZeroInitStep_t
 *       +-- packetQuality, packetRssi, packetAntenna
 *       +-- measuredFreqOffset
 *
 *       MODE 0 - Reflector: CS_modeZeroReflStep_t
 *       +-- packetQuality, packetRssi, packetAntenna
 *
 *       MODE 1 - RTT: CS_modeOneStep_t
 *       +-- packetQuality, packetNadm, packetRssi
 *       +-- ToDToA (Time of Departure - Time of Arrival, 0.5ns units) <-- RANGING
 *       +-- packetAntenna
 *
 *       MODE 2 - Phase: CS_modeTwoStep_t
 *       +-- antennaPermutationIndex
 *       +-- data[numAntennaPath+1] of modeTwoStepData_t:
 *           +-- i (12-bit I sample)  <-- PHASE RANGING
 *           +-- q (12-bit Q sample)  <-- PHASE RANGING
 *           +-- tqi (tone quality indicator)
 *
 *       MODE 3 - Combined RTT + Phase: CS_modeThreeStep_t
 *       +-- packetQuality, packetNadm, packetRssi
 *       +-- ToDToA                    <-- RANGING
 *       +-- packetAntenna, antennaPermutationIndex
 *       +-- data[numAntennaPath+1]    <-- PHASE RANGING
 *
 * @endcode
 *
 * @subsection cs_status_flags Status Flags
 *
 * @b Procedure @b Done @b Status (procedureDoneStatus):
 * - @ref CS_PROCEDURE_DONE (0x0): Procedure completed successfully
 * - @ref CS_PROCEDURE_ACTIVE (0x1): Procedure is currently active
 * - @ref CS_PROCEDURE_ABORTED (0xF): Procedure was aborted
 *
 * @b Subevent @b Done @b Status (subeventDoneStatus):
 * - @ref CS_SUBEVENT_DONE (0x0): All results complete for subevent
 * - @ref CS_SUBEVENT_ACTIVE (0x1): Partial results, more to follow
 * - @ref CS_SUBEVENT_ABORTED (0xF): Subevent aborted
 *
 * @subsection cs_error_handling Error Handling
 *
 * The @ref CS_PROCEDURE_ENABLE_COMPLETE_EVENT may return with a non-zero status
 * indicating the procedure failed to start or was terminated. Applications @b must
 * check the @c csStatus field and handle errors appropriately.
 *
 * @b Status @b Codes @b for @b CS_PROCEDURE_ENABLE_COMPLETE_EVENT (from @ref csStatus_e):
 *
 * | Status Code | Value | Description | Recovery Action |
 * |-------------|-------|-------------|-----------------|
 * | @ref CS_STATUS_SUCCESS | 0x00 | Procedure started/completed successfully | Wait for CS_SUBEVENT_RESULT |
 * | @ref CS_STATUS_PEER_TERM | 0x13 | Remote device terminated the procedure | Retry with CS_ProcedureEnable |
 * | @ref CS_STATUS_HOST_TERM | 0x16 | Local host terminated the procedure | Retry if needed |
 * | @ref CS_STATUS_INVALID_LL_PARAM | 0x1E | Invalid LL parameter (e.g., configId) | Verify configuration |
 * | @ref CS_STATUS_INSTANT_PASSED | 0x28 | Scheduling instant has passed | Retry with CS_ProcedureEnable |
 * | @ref CS_STATUS_INSUFFICIENT_SECURITY | 0x2F | CS security not enabled | Call CS_SecurityEnable first |
 * | @ref CS_STATUS_PROCEDURE_IN_PROGRESS | 0x3A | Another CS procedure is running | Wait for completion, then retry |
 *
 * @note When @ref CS_PROCEDURE_ENABLE_COMPLETE_EVENT returns with @c enable = @ref CS_DISABLE
 *       and a non-success status, no @ref CS_SUBEVENT_RESULT events will be generated.
 *       The application should check the status and call @ref CS_ProcedureEnable again
 *       to re-trigger the procedure if appropriate.
 *
 * @section cs_example Example Usage
 *
 * @code{.c}
 * // 1. Register callback
 * csCBs_t csCBs = { .pfnCsEvtCB = myCSEventHandler };
 * CS_RegisterCB(&csCBs);
 *
 * // 2. Read remote capabilities (after connection established)
 * CS_readRemoteCapCmdParams_t readCapParams = { .connHandle = connHandle };
 * CS_ReadRemoteSupportedCapabilities(&readCapParams);
 * // Wait for CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT
 *
 * // 3. Enable security
 * CS_securityEnableCmdParams_t secParams = { .connHandle = connHandle };
 * CS_SecurityEnable(&secParams);
 * // Wait for CS_SECURITY_ENABLE_COMPLETE_EVENT
 *
 * // 4. Set default settings
 * CS_setDefaultSettingsCmdParams_t defSettings = {
 *     .connHandle = connHandle,
 *     .roleEnable = CS_ROLE_INITIATOR,
 *     .csSyncAntennaSelection = 0,
 *     .maxTxPower = 0
 * };
 * CS_SetDefaultSettings(&defSettings);
 *
 * // 5. Create configuration
 * CS_createConfigCmdParams_t configParams = {
 *     .connHandle = connHandle,
 *     .configID = 0,
 *     .createContext = 1,
 *     .mainMode = CS_MODE_1,  // RTT mode
 *     .subMode = CS_MODE_UNUSED,
 *     .role = CS_ROLE_INITIATOR,
 *     .rttType = RTT_COARSE,
 *     .csSyncPhy = CS_LE_2M_SYNC_PHY
 * };
 * CS_CreateConfig(&configParams);
 * // Wait for CS_CONFIG_COMPLETE_EVENT
 *
 * // 6. Set procedure parameters
 * CS_setProcedureParamsCmdParams_t procParams = {
 *     .connHandle = connHandle,
 *     .configID = 0,
 *     .maxProcedureDur = 0xFFFF,
 *     .maxProcedureCount = 1,
 *     .minSubEventLen = 0xD6D8,
 *     .maxSubEventLen = 0xD6D8,
 *     .aci = ACI_A1_B1,  // 1x1 antenna
 *     .phy = CS_LE_2M_PHY,
 *     .enable = CS_ENABLE
 * };
 * CS_SetProcedureParameters(&procParams);
 *
 * // 7. Enable procedure
 * CS_setProcedureEnableCmdParams_t enableParams = {
 *     .connHandle = connHandle,
 *     .configID = 0,
 *     .enable = CS_ENABLE
 * };
 * CS_ProcedureEnable(&enableParams);
 * // Wait for CS_PROCEDURE_ENABLE_COMPLETE_EVENT
 * // Check status - if success, receive CS_SUBEVENT_RESULT events
 * // If failure, check status and retry if appropriate
 *
 * // 8. Event handler with error handling
 * void myCSEventHandler(void *pEvt)
 * {
 *     csEvtHdr_t *pHdr = (csEvtHdr_t *)pEvt;
 *     switch (pHdr->opcode)
 *     {
 *         case CS_PROCEDURE_ENABLE_COMPLETE_EVENT:
 *         {
 *             CS_procEnableCompleteEvt_t *pEnable = (CS_procEnableCompleteEvt_t *)pEvt;
 *
 *             if (pEnable->csStatus == CS_STATUS_SUCCESS && pEnable->enable == CS_ENABLE)
 *             {
 *                 // Procedure started successfully
 *                 // CS_SUBEVENT_RESULT events will follow
 *             }
 *             else if (pEnable->enable == CS_DISABLE)
 *             {
 *                 // Procedure ended or failed - check status
 *                 switch (pEnable->csStatus)
 *                 {
 *                     case CS_STATUS_SUCCESS:
 *                         // Procedure completed normally (e.g., maxProcedureCount reached)
 *                         // Can re-trigger if needed
 *                         if (needAnotherMeasurement())
 *                         {
 *                             CS_ProcedureEnable(&enableParams);
 *                         }
 *                         break;
 *
 *                     case CS_STATUS_PEER_TERM:
 *                         // Remote device terminated - retry
 *                         CS_ProcedureEnable(&enableParams);
 *                         break;
 *
 *                     case CS_STATUS_INSTANT_PASSED:
 *                         // Scheduling instant passed - retry immediately
 *                         CS_ProcedureEnable(&enableParams);
 *                         break;
 *
 *                     case CS_STATUS_PROCEDURE_IN_PROGRESS:
 *                         // Another procedure running, retry later
 *                         scheduleRetry(RETRY_DELAY_MS);
 *                         break;
 *
 *                     case CS_STATUS_INSUFFICIENT_SECURITY:
 *                         // Security not enabled - must enable security first
 *                         handleSecurityError();
 *                         break;
 *
 *                     case CS_STATUS_INVALID_LL_PARAM:
 *                         // Invalid parameter - check configuration
 *                         handleConfigError();
 *                         break;
 *
 *                     default:
 *                         // Other error - log and optionally retry
 *                         logError(pEnable->csStatus);
 *                         CS_ProcedureEnable(&enableParams);
 *                         break;
 *                 }
 *             }
 *             break;
 *         }
 *
 *         case CS_SUBEVENT_RESULT:
 *         case CS_SUBEVENT_CONTINUE_RESULT:
 *         {
 *             // Both events share the same status fields - cast based on opcode
 *             uint8_t procedureDoneStatus;
 *             uint8_t subeventDoneStatus;
 *             uint8_t abortReason;
 *             uint8_t numStepsReported;
 *             uint8_t *pData;
 *
 *             if (pHdr->opcode == CS_SUBEVENT_RESULT)
 *             {
 *                 CS_subeventResultsEvt_t *pResult = (CS_subeventResultsEvt_t *)pEvt;
 *                 procedureDoneStatus = pResult->procedureDoneStatus;
 *                 subeventDoneStatus = pResult->subeventDoneStatus;
 *                 abortReason = pResult->abortReason;
 *                 numStepsReported = pResult->numStepsReported;
 *                 pData = pResult->data;
 *             }
 *             else
 *             {
 *                 CS_subeventResultsContinueEvt_t *pContinue =
 *                     (CS_subeventResultsContinueEvt_t *)pEvt;
 *                 procedureDoneStatus = pContinue->procedureDoneStatus;
 *                 subeventDoneStatus = pContinue->subeventDoneStatus;
 *                 abortReason = pContinue->abortReason;
 *                 numStepsReported = pContinue->numStepsReported;
 *                 pData = pContinue->data;
 *             }
 *
 *             // Process step results if data is valid
 *             if (procedureDoneStatus != CS_PROCEDURE_ABORTED &&
 *                 subeventDoneStatus != CS_SUBEVENT_ABORTED)
 *             {
 *                 // Parse step data for ToD-ToA and I/Q samples
 *                 processStepResults(pData, numStepsReported);
 *             }
 *
 *             // Check procedure status and handle accordingly
 *             if (procedureDoneStatus == CS_PROCEDURE_DONE)
 *             {
 *                 // Procedure complete - can trigger another if needed
 *                 if (needAnotherMeasurement())
 *                 {
 *                     CS_ProcedureEnable(&enableParams);
 *                 }
 *             }
 *             else if (procedureDoneStatus == CS_PROCEDURE_ABORTED)
 *             {
 *                 // Procedure was aborted - check abortReason and retry
 *                 handleAbort(abortReason);
 *                 CS_ProcedureEnable(&enableParams);
 *             }
 *             // else CS_PROCEDURE_ACTIVE - more results coming
 *             break;
 *         }
 *
 *         // Handle other events...
 *         default:
 *             break;
 *     }
 * }
 * @endcode
 *
 * @}
 */

/*******************************************************************************
 * MACROS
 */
#define STEP_HDR_LEN    3U  //!< @ref csSubeventResultsStep_t header length

/*******************************************************************************
 * TYPEDEFS
 */
typedef enum
{
  CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT ,  //!< CS event Remote capabilities complete @ref CS_readRemoteCapabEvt_t
  CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT              ,  //!< CS event Read Remote FAE Table Complete @ref CS_readRemFAECompleteEvt_t
  CS_SECURITY_ENABLE_COMPLETE_EVENT                    ,  //!< CS Event Security Enable Complete @ref CS_securityEnableCompleteEvt_t
  CS_CONFIG_COMPLETE_EVENT                             ,  //!< CS event create config complete @ref CS_configCompleteEvt_t
  CS_PROCEDURE_ENABLE_COMPLETE_EVENT                   ,  //!< CS Procedure Enable Complete @ref CS_procEnableCompleteEvt_t
  CS_SUBEVENT_RESULT                                   ,  //!< CS Subevent Result @ref CS_subeventResultsEvt_t
  CS_SUBEVENT_CONTINUE_RESULT                          ,  //!< CS Subevent continue Result @ref CS_subeventResultsContinueEvt_t
} csEventOpcodes_e;

/**
 * @brief Structure for Channel Sounding Event Header.
 *
 * This structure contains the opcode for a Channel Sounding event, and
 * it is used to easely classify the incoming event type.
 */
typedef struct
{
  uint8_t opcode;     // CS Event Opcode @ref csEventOpcodes_e
} csEvtHdr_t;

/*
* @brief Host CS Callback
*
* @param pEvt - the CS event struct
*
* @return None
*/
typedef void(*pfnCsCB_t)(void *pEvt);

/**
 * @brief Host CS callbacks
 */
typedef struct
{
  pfnCsCB_t pfnCsEvtCB;    //!< CS Event Callback, returns CS event struct that can be addressed as @ref csEvtHdr_t
} csCBs_t;

typedef struct
{
  csEventOpcodes_e csEvtOpcode;      //!< CS Event Code @ref csEventOpcodes_e
  uint16_t   connHandle;             //!< connection handle
  uint8_t    csStatus;               //!< Event status
  uint8_t    numConfig;              //!< Number of CS configurations supported per conn
  uint16_t   maxProcedures;          //!< Max num of CS procedures supported
  uint8_t    numAntennas;            //!< the number of antenna elements that are available for CS tone exchanges
  uint8_t    maxAntPath;             //!< max number of antenna paths that are supported
  uint8_t    role;                   //!< initiator or reflector @ref CS_Role
  uint8_t    optionalModes;          //!< indicates which of the optional CS modes are supported
  uint8_t    rttCap;                 //!< indicate which of the time-of-flight accuracy requirements are met
  uint8_t    rttAAOnlyN;             //!< Number of CS steps of single packet exchanges needed
  uint8_t    rttSoundingN;           //!< Number of CS steps of single packet exchanges needed
  uint8_t    rttRandomPayloadN;      //!< Num of CS steps of single packet exchange needed
  uint16_t   nadmSounding;           //!< NADM Sounding Capability
  uint16_t   nadmRandomSeq;          //!< NADM Random Sequence Capability
  uint8_t    optionalCsSyncPhy;      //!< supported CS sync PHYs, bit mapped field
  uint8_t    noFAE;                  //!< No FAE
  uint8_t    chSel3c;                //!< channel selection 3c support
  uint8_t    csBasedRanging;         //!< CS based ranging
  uint16_t   tIp1Cap;                //!< tIP1 Capability
  uint16_t   tIp2Cap;                //!< tTP2 Capability
  uint16_t   tFcsCap;                //!< tFCS Capability
  uint16_t   tPmCsap;                //!< tPM Capability
  uint8_t    tSwCap;                 //!< Antenna switch time capability
  uint8_t    snrTxCap;               //!< Spec defines an additional byte for RFU
} CS_readRemoteCapabEvt_t;

typedef struct
{
  csEventOpcodes_e csEvtOpcode;        //!< CS Event Code
  uint16_t         connHandle;         //!< connection handle
  uint8_t          csStatus;           //!< Event status
  uint8_t          configId;           //!< CS configuration ID
  uint8_t          state;              //!< 0b00 disabled, 0b01 enabled
  uint8_t          mainMode;           //!< which CS modes are to be used @ref CS_Mode
  uint8_t          subMode;            //!< which CS modes are to be used @ref CS_Mode
  uint8_t          mainModeMinSteps;   //!< range of Main_Mode steps to be executed before
  uint8_t          mainModeMaxSteps;   //!< a Sub_Mode step is executed
  uint8_t          mainModeRepetition; //!< num of main mode steps from the last CS subevent to be repeated
  uint8_t          modeZeroSteps;      //!< number of mode 0 steps to be included at the beginning of each CS Subevent
  uint8_t          role;               //!< initiator or reflector @ref CS_Role
  uint8_t          rttType;            //!< which RTT variant is to be used @ref CS_RTT_Type
  uint8_t          csSyncPhy;          //!< transmit and receive PHY to be used @ref CS_Sync_Phy_Supported
  csChm_t          channelMap;         //!< channel map @ref csChm_t
  uint8_t          chMRepetition;      //!< number of times the ChM field will be cycled through
  uint8_t          chSel;              //!< channel selection algorithm to be used @ref CS_Chan_Sel_Alg
  uint8_t          ch3cShape;          //!< selected shape to be rendered
  uint8_t          ch3CJump;           //!< one of the valid CSChannelJump values
  uint8_t          rfu0;               //!< reserved for future use
  uint8_t          tIP1;               //!< Index of the period used between RTT packets
  uint8_t          tIP2;               //!< Index of the interlude period used between CS tones
  uint8_t          tFCs;               //!< Index used for frequency changes
  uint8_t          tPM;                //!< Index for the measurement period of CS tones
  uint8_t          rfu1;               //!< reserved for future use
} CS_configCompleteEvt_t;

typedef struct
{
  csEventOpcodes_e    csEvtOpcode;             //!< CS Event Code @ref csEventOpcodes_e
  uint16_t            connHandle;              //!< connection handle
  uint8_t             csStatus;                //!< Event status
  csFaeTbl_t          faeTable;                //!< Remote CS capabilities
} CS_readRemFAECompleteEvt_t;

typedef struct
{
  csEventOpcodes_e    csEvtOpcode;            //!< CS Event Code @ref csEventOpcodes_e
  uint16_t            connHandle;             //!< connection handle
  uint8_t             csStatus;               //!< Event status
} CS_securityEnableCompleteEvt_t;

typedef struct
{
  csEventOpcodes_e csEvtOpcode;        //!< CS Event Code
  uint8_t          csStatus;           //!< Event status
  uint16_t         connHandle;         //!< connection handle
  uint8_t          configId;           //!< configuration ID
  uint8_t          enable;             //!< enable/disable @ref CS_Enable
  csACI_e          ACI;                //!< Antenna Config Index @ref csACI_e
  int8_t           selectedTxPower;    //!< Transmit power level used for CS procedure. Units: dBm
  uint32_t         subEventLen;        //!< sub-event length in microseconds, range 1250us to 4s
  uint8_t          subEventsPerEvent;  //!< number of CS SubEvents in a CS Event
  uint16_t         subEventInterval;   //!< sub-event interval in units of 625 us
  uint16_t         eventInterval;      //!< event interval in units of connection interval
  uint16_t         procedureInterval;  //!< procedure interval
  uint16_t         procedureCount;     //!< procedure count
} CS_procEnableCompleteEvt_t;

typedef struct
{
  csEventOpcodes_e csEvtOpcode;                  //!< CS Event Code @ref csEventOpcodes_e
  uint16_t         connHandle;                   //!< connection handle
  uint8_t          configID;                     //!< configuration ID
  uint16_t         startAclConnectionEvent;      //!< start ACL connection event
  uint16_t         procedureCounter;             //!< procedure counter
  int16_t          frequencyCompensation;        //!< frequency compensation
  int8_t           referencePowerLevel;          //!< reference power level
  uint8_t          procedureDoneStatus;          //!< procedure done status @ref CS_Procedure_Done_Status
  uint8_t          subeventDoneStatus;           //!< subevent done status
  uint8_t          abortReason;                  //!< abort reason @ref CS_Abort_Reason
  uint8_t          numAntennaPath;               //!< number of antenna paths
  uint8_t          numStepsReported;             //!< number of steps reported
  uint16_t         dataLen;                      //!< Data length
  uint8_t          data[];                       //!< Array of type @ref csSubeventResultsStep_t
} CS_subeventResultsEvt_t;

typedef struct
{
  csEventOpcodes_e csEvtOpcode;         //!< CS Event Code @ref csEventOpcodes_e
  uint16_t         connHandle;          //!< connection handle
  uint8_t          configID;            //!< configuration ID
  uint8_t          procedureDoneStatus; //!< procedure done status @ref CS_Procedure_Done_Status
  uint8_t          subeventDoneStatus;  //!< subevent done status
  uint8_t          abortReason;         //!< abort reason @ref CS_Abort_Reason
  uint8_t          numAntennaPath;      //!< number of antenna paths
  uint8_t          numStepsReported;    //!< number of steps reported
  uint16_t         dataLen;             //!< Data length
  uint8_t          data[];              //!< Array of type @ref csSubeventResultsStep_t
} CS_subeventResultsContinueEvt_t;

/*
 * Command Parameters Structures
 */
typedef struct
{
  uint16_t     connHandle;
} CS_readRemoteCapCmdParams_t;

typedef struct
{
  uint16_t         connHandle;          //!< Connection handle
  csCapabilities_t remoteCapabilities;  //!< Remote CS capabilities
} CS_writeCachedRemoteCapCmdParams_t;

typedef struct
{
  uint16_t    connHandle;              //!< Connection handle
  uint8_t     configID;                //!< Configuration ID
  uint8_t     createContext;           //!< Create context flag
  uint8_t     mainMode;                //!< Main mode @ref CS_Mode
  uint8_t     subMode;                 //!< Sub mode @ref CS_Mode
  uint8_t     mainModeMinSteps;        //!< Minimum steps for main mode
  uint8_t     mainModeMaxSteps;        //!< Maximum steps for main mode
  uint8_t     mainModeRepetition;      //!< Main mode repetition
  uint8_t     modeZeroSteps;           //!< Steps for mode zero
  uint8_t     role;                    //!< Role @ref CS_Role
  uint8_t     rttType;                 //!< RTT type @ref CS_RTT_Type
  uint8_t     csSyncPhy;               //!< CS sync PHY @ref CS_Sync_Phy_Supported
  csChm_t     channelMap;              //!< Channel map @ref csChm_t
  uint8_t     chMRepetition;           //!< Channel map repetition
  uint8_t     chSel;                   //!< Channel selection algorithm to be used @ref CS_Chan_Sel_Alg
  uint8_t     ch3cShape;               //!< Channel 3C shape
  uint8_t     ch3CJump;                //!< Channel 3C jump
} CS_createConfigCmdParams_t;

typedef struct
{
  uint16_t connHandle;             //!< Connection handle
} CS_securityEnableCmdParams_t;

typedef struct
{
  uint16_t connHandle;              //!< Connection handle
  uint8_t  roleEnable;              //!< Role enable flag
  uint8_t  csSyncAntennaSelection;  //!< CS sync antenna selection
  int8_t   maxTxPower;              //!< Maximum TX power in dBm
} CS_setDefaultSettingsCmdParams_t;

typedef struct
{
  uint16_t connHandle;             //!< Connection handle
} CS_readRemoteFAETableCmdParams_t;

typedef struct
{
  uint16_t    connHandle;          //!< Connection handle
  csFaeTbl_t  remoteFaeTable;      //!< Pointer to the remote device FAE table
} CS_writeCachedRemoteFAETableCmdParams_t;

typedef struct
{
  uint16_t connHandle;           //!< Connection handle
  uint8_t  configID;             //!< Configuration ID
} CS_removeConfigCmdParams_t;

typedef struct
{
  csChm_t channelClassification; //!< Channel classification @ref csChm_t
} CS_setChannelClassificationCmdParams_t;

typedef struct
{
  uint16_t   connHandle;           //!< Connection handle
  uint8_t    configID;             //!< Configuration ID
  uint16_t   maxProcedureDur;      //!< Maximum procedure duration in 0.625 milliseconds
  uint16_t   minProcedureInterval; //!< Minimum number of connection events between consecutive CS procedures
  uint16_t   maxProcedureInterval; //!< Maximum number of connection events between consecutive CS procedures
  uint16_t   maxProcedureCount;    //!< Maximum number of CS procedures to be scheduled (0 - indefinite)
  uint32_t   minSubEventLen;       //!< Minimum SubEvent length in microseconds, range 1250us to 4s
  uint32_t   maxSubEventLen;       //!< Maximum SubEvent length in microseconds, range 1250us to 4s
  csACI_e    aci;                  //!< Antenna Configuration Index @ref csACI_e
  uint8_t    phy;                  //!< PHY @ref CS_Phy_Supported
  int8_t     txPwrDelta;           //!< Tx Power Delta, in signed dB
  uint8_t    preferredPeerAntenna; //!< Preferred peer antenna
  uint8_t    snrCtrlI;             //!< SNR Control Initiator
  uint8_t    snrCtrlR;             //!< SNR Control Reflector
  uint8_t    enable;               //!< Is procedure enabled @ref CS_Enable
} CS_setProcedureParamsCmdParams_t;

typedef struct
{
  uint16_t connHandle;  //!< Connection handle
  uint8_t  configID;    //!< Configuration ID
  uint8_t  enable;      //!< Enable or disable the procedure @ref CS_Enable
} CS_setProcedureEnableCmdParams_t;

typedef struct
{
  uint8_t defaultAntennaIndex;    //!< Index of the antenna to set as a default antenna for common BLE communications
} CS_setDefaultAntennaCmdParams_t;

typedef struct
{
  uint8_t* role;        //!< Output role
  uint16_t connHandle;  //!< Connection handle
  uint8_t  configID;    //!< Configuration ID
} CS_GetRoleCmdParams_t;

/*
 * The following structures are meant to help parsing the data from the subevent results
 */

typedef struct
{
    uint8_t stepMode;            //!< Step mode
    uint8_t stepChnl;            //!< Step channel
    uint8_t stepDataLen;         //!< Step data length

    /* Step data is one of these types:
     * @ref CS_modeZeroInitStep_t
     * @ref CS_modeZeroReflStep_t
     * @ref CS_modeOneStep_t
     * @ref CS_modeTwoStep_t
     * @ref CS_modeThreeStep_t
     */
    uint8_t stepData;
} csSubeventResultsStep_t;

PACKED_TYPEDEF_STRUCT
{
  uint8_t  packetQuality;         //!< Packet quality
  uint8_t  packetRssi;            //!< Packet RSSI
  uint8_t  packetAntenna;         //!< PacKet antenna
  uint16_t measuredFreqOffset;    //!< Measured Frequency Offset
} CS_modeZeroInitStep_t;

PACKED_TYPEDEF_STRUCT
{
  uint8_t packetQuality;        //!< Packet quality
  uint8_t packetRssi;           //!< Packet RSSI
  uint8_t packetAntenna;        //!< PacKet antenna
} CS_modeZeroReflStep_t;

PACKED_TYPEDEF_STRUCT
{
  uint8_t  packetQuality;       //!< Packet quality
  uint8_t  packetNadm;          //!< Attack likelihood
  uint8_t  packetRssi;          //!< Packet RSSI
  uint16_t ToDToA;              //!< Time difference in 0.5 ns units between arrival and departure of CS packets
  uint8_t  packetAntenna;       //!< PacKet antenna
} CS_modeOneStep_t;

PACKED_TYPEDEF_STRUCT
{
  uint32_t i:12;              //!< I sample
  uint32_t q:12;              //!< Q sample
  uint32_t tqi:8;             //!< Tone quality indicator
} modeTwoStepData_t;

PACKED_TYPEDEF_STRUCT
{
  uint8_t antennaPermutationIndex; //!< Antenna permutation index
  modeTwoStepData_t data[];        //!< Array of length (numAntennaPath + 1), each element is of type @ref modeTwoStepData_t
} CS_modeTwoStep_t;

PACKED_TYPEDEF_STRUCT
{
  uint8_t  packetQuality;           //!< Packet quality
  uint8_t  packetNadm;              //!< Attack likelihood
  uint8_t  packetRssi;              //!< Packet RSSI
  uint16_t ToDToA;                  //!< Time difference in 0.5 ns units between arrival and departure of CS packets
  uint8_t  packetAntenna;           //!< PacKet antenna
  uint8_t  antennaPermutationIndex; //!< Antenna permutation index
  modeTwoStepData_t data[];         //!< Array of length (numAntennaPath + 1), each element is of type @ref modeTwoStepData_t
} CS_modeThreeStep_t;

/*******************************************************************************
 * API FUNCTIONS
 */

/**
 * @fn      CS_ReadLocalSupportedCapabilities
 *
 * @brief   Read the local supported capabilities for Channel Sounding.
 *
 * @param   localCapab Pointer to a capabilities structure that will be filled by the device.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_ReadLocalSupportedCapabilities(csCapabilities_t *localCapab);

/**
 * @fn      CS_ReadRemoteSupportedCapabilities
 *
 * @brief   Read the remote supported capabilities for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for reading remote capabilities.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_ReadRemoteSupportedCapabilities(CS_readRemoteCapCmdParams_t *params);

/**
 * @fn      CS_WriteCachedRemoteSupportedCapabilities
 *
 * @brief   Write cached remote supported capabilities for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for writing remote capabilities.
 *
 * @return      CS_STATUS_PROCEDURE_IN_PROGRESS - Currently in a middle of a procedure
 *              CS_STATUS_ERROR_COMMAND_DISALLOWED - If LL_CS_CAPABILITIES_REQ or
 *                            LL_CS_CAPABILITIES_RSP PDU has been received from the
 *                            remote Controller, or if a CS configuration has already
 *                            been created.
 *
 *              CS_STATUS_ERROR_INACTIVE_CONNECTION - if the connection handle is inactive
 *              CS_STATUS_INVOKE_FUNC_FAIL - If the command invocation failed
 *              CS_STATUS_SUCCESS - if successfully written
 */
csStatus_e CS_WriteCachedRemoteSupportedCapabilities(CS_writeCachedRemoteCapCmdParams_t *pParams);

/**
 * @fn      CS_SecurityEnable
 *
 * @brief   Enable security for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for enabling security.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_SecurityEnable(CS_securityEnableCmdParams_t *params);

/**
 * @fn      CS_SetDefaultSettings
 *
 * @brief   Set the default settings for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for setting default settings.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_SetDefaultSettings(CS_setDefaultSettingsCmdParams_t *params);

/**
 * @fn      CS_ReadRemoteFAETable
 *
 * @brief   Read the remote FAE table for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for reading the remote FAE table.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_ReadRemoteFAETable(CS_readRemoteFAETableCmdParams_t *params);

/**
 * @fn      CS_WriteCachedRemoteFAETable
 *
 * @brief   Write the cached remote FAE table for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for writing the remote FAE table.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_WriteCachedRemoteFAETable(CS_writeCachedRemoteFAETableCmdParams_t *params);

/**
 * @fn      CS_CreateConfig
 *
 * @brief   Create a configuration for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for creating the configuration.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_CreateConfig(CS_createConfigCmdParams_t *params);

/**
 * @fn      CS_RemoveConfig
 *
 * @brief   Remove a configuration for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for removing the configuration.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_RemoveConfig(CS_removeConfigCmdParams_t *params);

/**
 * @fn      CS_SetChannelClassification
 *
 * @brief   Set the channel classification for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for setting the channel classification.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_SetChannelClassification(CS_setChannelClassificationCmdParams_t *params);

/**
 * @fn      CS_SetProcedureParameters
 *
 * @brief   Set the procedure parameters for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for setting the procedure parameters.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_SetProcedureParameters(CS_setProcedureParamsCmdParams_t *params);

/**
 * @fn      CS_ProcedureEnable
 *
 * @brief   Enable or disable a procedure for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for enabling or disabling the procedure.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_ProcedureEnable(CS_setProcedureEnableCmdParams_t *params);

/**
 * @fn          CS_SetDefaultAntenna
 *
 * @brief       Sets the default antenna to be used for common BLE communications.
 *
 * @param       params - Pointer to the parameters for multiple antennas configuration.
 *
 * @return      @ref CS_STATUS_SUCCESS - The parameters has been set successfully
 *              @ref CS_STATUS_UNEXPECTED_PARAMETER - The given index is out of range
 *              @ref CS_STATUS_COMMAND_DISALLOWED - The device is not in Idle mode.
 *              @ref CS_STATUS_INVOKE_FUNC_FAIL - Function failed to execute.
 *
 * @attention   It is recommended to Call this function only when the device is idle
 *              to avoid frequent errors.
 *
 * @note        Calling this function is particularly useful during device
 *              initialization to ensure proper antenna configuration.
 */
csStatus_e CS_SetDefaultAntenna(CS_setDefaultAntennaCmdParams_t *pParams);

/**
 * @fn      CS_GetRole
 *
 * @brief   Gets the local role based on connection handle and config Id.
 *
 * @param   params - Pointer to the parameters
 *
 * @return  @ref CS_STATUS_SUCCESS
 * @return  @ref CS_STATUS_INVOKE_FUNC_FAIL if invoking the CS api failed
 * @return  @ref CS_STATUS_INACTIVE_CONNECTION if connection isn't active
 * @return  @ref CS_STATUS_UNEXPECTED_PARAMETER if config Id isn't valid
 */
csStatus_e CS_GetRole(CS_GetRoleCmdParams_t *pParams);

/**
 * @fn      CS_GetConfiguration
 *
 * @brief   Get const pointer to CS configuration from controller database.
 *
 * @note    This function provides READ-ONLY direct access to the CS configuration
 *          stored in the controller database. The returned pointer remains valid
 *          until the configuration is removed or the connection is terminated.
 *          Do not modify the data through this pointer.
 *
 * @note    This zero-copy approach eliminates the need for caching configuration
 *          parameters at the application layer, reducing memory overhead and
 *          ensuring data is always up-to-date with the controller state.
 *
 * @param   connHandle - Connection handle
 * @param   configId - Configuration ID (0-7)
 *
 * @return  Const pointer to configuration structure (struct csConfig_t)
 * @return  NULL if configuration not found or connection/configId invalid
 */
const struct csConfig_t* CS_GetConfiguration(uint16_t connHandle, uint8_t configId);

/*******************************************************************************
 * @fn          CS_ConvertTip
 *
 * @brief       Convert T_IP timing index to microseconds value.
 *
 * @note        T_IP (Interlude Period) is used between RTT packets and
 *              between CS tones. The index (0-7) maps to values in
 *              the range 10-145 microseconds.
 *
 *              Inline function for zero-overhead conversion via wrapper chain.
 *
 * @param       idx - T_IP index (0-7) from CS configuration
 *
 * @return      T_IP value in microseconds (uint16_t)
 */
uint16_t CS_ConvertTip(uint8_t idx);

/*******************************************************************************
 * @fn          CS_ConvertTfcs
 *
 * @brief       Convert T_FCS timing index to microseconds value.
 *
 * @note        T_FCS (Frequency Change Switching) is the period of frequency
 *              changes. The index (0-9) maps to values in the range
 *              15-150 microseconds.
 *
 *              Inline function for zero-overhead conversion via wrapper chain.
 *
 * @param       idx - T_FCS index (0-9) from CS configuration
 *
 * @return      T_FCS value in microseconds (uint16_t)
 */
uint16_t CS_ConvertTfcs(uint8_t idx);

/*******************************************************************************
 * @fn          CS_ConvertTpm
 *
 * @brief       Convert T_PM timing index to microseconds value.
 *
 * @note        T_PM (Phase Measurement) is the measurement period of CS tones.
 *              The index (0-3) maps to values in the range 10-40 microseconds.
 *
 *              Inline function for zero-overhead conversion via wrapper chain.
 *
 * @param       idx - T_PM index (0-3) from CS configuration
 *
 * @return      T_PM value in microseconds (uint16_t)
 */
uint16_t CS_ConvertTpm(uint8_t idx);

/*******************************************************************************
 * @fn          CS_GetTswByACI
 *
 * @brief       Calculate the antenna switch timing (T_SW) value based on
 *              Antenna Configuration Index (ACI) and device capabilities.
 *
 *              The T_SW value is determined by the ACI configuration:
 *              - ACI_A1_B1 (1x1): No antenna switching -> T_SW = 0
 *              - ACI_A2/3/4_B1: Uses initiator's T_SW capability
 *              - ACI_A1_B2/3/4: Uses reflector's T_SW capability
 *              - ACI_A2_B2 (2x2): Uses max(initiator, reflector) T_SW capability
 *
 * input parameters
 *
 * @param       ACI      - Antenna Configuration Index
 * @param       initTsw  - Initiator's T_SW capability (from tSwCap)
 * @param       reflTsw  - Reflector's T_SW capability (from tSwCap)
 *
 * output parameters
 *
 * @param       None
 *
 * @return      uint8_t - Calculated T_SW value in microseconds
 */
uint8_t CS_GetTswByACI(csACI_e ACI, uint8_t initTsw, uint8_t reflTsw);

/*******************************************************************************
 * @fn          CS_GetStepLength
 *
 * @brief       This function calculates the length of a step data, depends on
 *              role, mode, and number of antenna paths.
 *
 * @param       mode - Step mode. Should be one of:
 *                     @ref CS_MODE_0
 *                     @ref CS_MODE_1
 *                     @ref CS_MODE_2
 *                     @ref CS_MODE_3
 *
 * @param       role - Role of the device measured the step of types:
 *                     @ref CS_ROLE_INITIATOR or @ref CS_ROLE_REFLECTOR
 *
 * @param       numAntennaPath - Number of antenna paths used in the step
 *
 * @return      Length of the relevant step data
 * @return      0 for one of the following cases:
 *              - Invalid mode has been given.
 *              - mode is 0 and role is invalid.
 */
uint8_t CS_GetStepLength(uint8_t mode, uint8_t role, uint8_t numAntennaPath);

/*******************************************************************************
 * @fn          CS_calcNumPaths
 *
 * @brief       This function returns number of antenna paths based on a given
 *              ACI (antenna permutation index).
 *
 * @param       aci - antenna permutation index
 *
 * @return      Number of antenna paths: 1 - 4
 * @return      0 If the given parameter is invalid
 */
uint8_t CS_calcNumPaths(csACI_e aci);

/*******************************************************************************
 * @fn          CS_calcAntPathsMask
 *
 * @brief       This function returns the antenna paths mask based on a given
 *              ACI (antenna permutation index).
 *
 * @param       aci - antenna permutation index
 *
 * @return      Antenna paths mask
 */
uint8_t CS_calcAntPathsMask(csACI_e aci);

/**
 * @fn      CS_RegisterCB
 *
 * @brief   Register a callback function for Channel Sounding events.
 *
 * @note    It is mandatory to register a callback function to handle Channel Sounding events.
 *
 * @param   pCB - Pointer to the callback function.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
uint8_t CS_RegisterCB(const csCBs_t *pCBs);

/**
 * @fn      CS_ProcessEvent
 *
 * @brief   Process a Channel Sounding event.
 *
 * @param   pData - Pointer to the event data.
 *
 * @return  None
 */
extern void CS_ProcessEvent(csEvtHdr_t *pData);

/** @} */ /* end of CS_API group */

#endif // CS_H
