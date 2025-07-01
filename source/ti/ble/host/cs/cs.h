/******************************************************************************

 @file  cs.h

 @brief This file contains the structures used by Channel Sounding.

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
#ifndef CS_H
#define CS_H
/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/cs_types.h"

/*******************************************************************************
 * MACROS
 */
#define STEP_HDR_LEN    3U  //!< @ref csSubeventResultsStep_t header length

/*******************************************************************************
 * TYPEDEFS
 */
typedef enum
{
  CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT , //!< CS event Remote capabilities complete @ref CS_readRemoteCapabEvt_t
  CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT,               //!< CS event Read Remote FAE Table Complete @ref CS_readRemFAECompleteEvt_t
  CS_SECURITY_ENABLE_COMPLETE_EVENT,                     //!< CS Event Security Enable Complete @ref CS_securityEnableCompleteEvt_t
  CS_CONFIG_COMPLETE_EVENT,                              //!< CS event create config complete @ref CS_configCompleteEvt_t
  CS_PROCEDURE_ENABLE_COMPLETE_EVENT,                    //!< CS Procedure Enable Complete @ref CS_procEnableCompleteEvt_t
  CS_SUBEVENT_RESULT,                                    //!< CS Subevent Result @ref CS_subeventResultsEvt_t
  CS_SUBEVENT_CONTINUE_RESULT,                           //!< CS Subevent continue Result @ref CS_subeventResultsContinueEvt_t
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
  uint8_t          pwrDelta;           //!< Tx Power Delta, in signed dB
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
  csFaeTbl_t  reflectorFaeTable;   //!< Pointer to the reflector FAE table
} CS_writeRemoteFAETableCmdParams_t;

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
  uint8_t    txPwrDelta;           //!< Tx Power Delta, in signed dB
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
  uint16_t connHandle;  //!< Connection handle
  uint8_t  configID;    //!< Configuration ID
  uint8_t* role;        //!< Output role
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
 * @fn      CS_WriteRemoteFAETable
 *
 * @brief   Write the remote FAE table for Channel Sounding.
 *
 * @param   params - Pointer to the parameters for writing the remote FAE table.
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE
 */
csStatus_e CS_WriteRemoteFAETable(CS_writeRemoteFAETableCmdParams_t *params);

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
 * @fn          CS_calcNumPathsFromAntennaMask
 *
 * @brief       This function returns number of antenna paths based on a given
 *              antenna paths mask.
 *
 * @param       antPathMask - first 4 bits, each one represent an antenna path.
 *                            Lsb bit represent the first antenna path.
 *                            Valid values: 0x1, 0x3, 0x7, 0xF
 *
 * @return      Number of antenna paths: 1 - 4
 * @return      0 If the given parameter is invalid
 */
uint8_t CS_calcNumPathsFromAntennaMask(uint8_t antPathMask);

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

#endif // CS_H
