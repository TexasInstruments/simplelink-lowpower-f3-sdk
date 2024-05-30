/******************************************************************************

@file  debugInfo_internal.h

@brief This file contains the debugInfo_internal for use
with the Bluetooth Low Energy Protocol Stack.

Group: WCS, BTS
Target Device: cc23xx

******************************************************************************

 Copyright (c) 2022-2024, Texas Instruments Incorporated
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

#ifndef DEBUGINFO_INTERNAL_H
#define DEBUGINFO_INTERNAL_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "bcomdef.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/**
 * @brief  DebugInfo Scheduler new record Data Structure
 *
 * Used to store the data of new scheduler record
 */
typedef struct DbgInf_schedNewRec_t_
{
  uint8_t     activeTasks;      //!< bits that indicate which tasks are active
  uint8_t     taskID;           //!< type of LL task
  uint8_t     llState;          //!< state of LL
  uint8_t     reserved;
  uint32_t    cmdStartTime;     //!< the start time of the new scheduled command
  uint32_t    timeStamp;        //!< time stamp for scheduling the new command
} DbgInf_schedNewRec_t;

/**
 * @brief  DebugInfo Scheduler new record Data Structure
 *
 * Used to store the data of new scheduler record
 */
typedef struct DbgInf_connTermRec_t_
{
  uint8_t    peerAddr[2];      //!< 2 LSB bytes of peer device address
  uint8_t    connId;           //!< bits that indicate which tasks are active
  uint8_t    termReason;       //!< termination reason code
  uint16_t   connInterval;     //!< connection interval
  uint16_t   connEvent;        //!< current event number
} DbgInf_connTermRec_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * CONSTS
 */

/*********************************************************************
 * FUNCTIONS
 */

/**
 *  @defgroup DbgInf_Internal_Functions Debug Info Internal Functions
 *  @brief This module implements Debug Info internal functions
 *  @{
 */

/**
 * @brief   Adds new scheduler record and updates the scheduler domain
 *
 * @param   newRec - new scheduler record @ref DbgInf_schedNewRec_t
 *
 * @return  @ref SUCCESS
 * @return  @ref INVALIDPARAMETER : the input is not valid
 * @return  @ref FAILURE : the scheduler domain is not initialized
 */
uint8_t DbgInf_addSchedRec( DbgInf_schedNewRec_t * const newRec );

/**
 * @brief   Adds new connection establishment event to the connection domain
 *
 * @param   connId        - Connection handle
 * @param   connRole      - Connection formed as Central or Peripheral
 * @param   encEnabled    - Flag to indicate that encryption is enabled for this connection
 *
 * @return  @ref SUCCESS
 * @return  @ref INVALIDPARAMETER : the input is not valid
 * @return  @ref FAILURE : the coonection domain is not initialized or debug info module is not active
 */
uint8_t DbgInf_addConnEst( uint16_t connId, uint8_t connRole, uint8_t encEnabled );

/**
 * @brief   Adds new connection terminated event to the connection domain
 *
 * @param   newRec - new connection terminated record @ref DbgInf_connTermRec_t
 *
 * @return  @ref SUCCESS
 * @return  @ref INVALIDPARAMETER : the input is not valid
 * @return  @ref FAILURE : the coonection domain is not initialized or debug info module is not active
 */
uint8_t DbgInf_addConnTerm( DbgInf_connTermRec_t * const newRec );

/**
 * @brief   Adds new error record to the error domain
 *
 * @param   newRec - new error record
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE : the error domain is not initialized or debug info module is not active
 */
uint8_t DbgInf_addErrorRec( uint16_t newError );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* DEBUGINFO_INTERNAL_H */
