/******************************************************************************

@file  debugInfo.h

@brief This file contains the debugInfo for use
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
/**
 *  @defgroup DebugInfo DebugInfo
 *  @brief This module implements DebugInfo functions
 *  @{
 *  @file  debugInfo.h
 *  @brief This file contains the debugInfo_api for use
 *  with the Bluetooth Low Energy Protocol Stack.
 */


#ifndef DEBUGINFO_H
#define DEBUGINFO_H

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

/// @cond NODOC

/************************************************/
/********** General debug info MACROS  **********/
/************************************************/

// New BV definition for MIRSA-C compliance
#ifndef BVU16
#define BVU16(n)      (uint16_t)((uint16_t)1U << (n))
#endif

#ifndef BVU8
#define BVU8(n)      (uint8_t)((uint8_t)1U << (n))
#endif

// Number of scheduler history records
#ifndef DBGINF_SCHED_MAX_NUM_OF_RECORDS
#define DBGINF_SCHED_MAX_NUM_OF_RECORDS            (uint8_t)(10)
#endif  // DBGINF_SCHED_MAX_NUM_OF_RECORDS

// Number of connection history records
#ifndef DBGINF_CONN_MAX_NUM_OF_TERM_RECORDS
#define DBGINF_CONN_MAX_NUM_OF_TERM_RECORDS        (uint8_t)(4)
#endif  // DBGINF_CONN_MAX_NUM_OF_RECORDS

// Number of connection established records
#ifndef DBGINF_CONN_MAX_NUM_OF_EST_RECORDS
#if defined( HOST_CONFIG ) && ( HOST_CONFIG & ( PERIPHERAL_CFG | CENTRAL_CFG ) )
#define DBGINF_CONN_MAX_NUM_OF_EST_RECORDS        (uint8_t)(MAX_NUM_BLE_CONNS)
#else
#define DBGINF_CONN_MAX_NUM_OF_EST_RECORDS        (uint8_t)(1)
#endif  // HOST_CONFIG
#endif  // DBGINF_CONN_MAX_NUM_OF_RECORDS

// Number of errors records
#ifndef DBGINF_ERROR_MAX_NUM_OF_ERROR_RECORDS
#define DBGINF_ERROR_MAX_NUM_OF_ERROR_RECORDS      (uint8_t)(11)
#endif  // DBGINF_CONN_MAX_NUM_OF_RECORDS

#define DBGINF_MAX_NUM_DOMAINS         (uint8_t)(3)

// Debug info domains bitmap
#define  DBGINF_DOMAIN_NONE            (uint16_t)0U             //!< None
#define  DBGINF_DOMAIN_SCHEDULER       (uint16_t)BVU16(0)       //!< Scheduler domain
#define  DBGINF_DOMAIN_CONNECTION      (uint16_t)BVU16(1)       //!< Connection domain
#define  DBGINF_DOMAIN_ERROR           (uint16_t)BVU16(2)       //!< Error domain
#define  DBGINF_DOMAIN_ALL             (uint16_t)0xFFFF         //!< All domains

// Mask of all valid domains
#define  DBGINF_DOMAIN_VALID           (uint16_t)( DBGINF_DOMAIN_SCHEDULER  |\
                                                   DBGINF_DOMAIN_CONNECTION |\
                                                   DBGINF_DOMAIN_ERROR )
// Debug info general defines
#define DBGINF_GEN_BITMAP_LEN          (uint16_t)2     //!< Domains bitmap length in bytes
#define DBGINF_GEN_DATA_HDR_LEN        (uint16_t)8     //!< General header length in bytes
#define DBGINF_GEN_DOMAIN_HDR_LEN      (uint16_t)4     //!< Domain header length in bytes
#define DBGINF_GEN_WATERMARK           0xDBDBDBDBU     //!< Water mark for get API

/************************************************/
/*********** Scheduler domain MACROS  ***********/
/************************************************/

// Scheduler domain defines
#define DBGINF_SCHED_GEN_INFO_SIZE     (uint16_t)8     //!< Scheduler domain general data size in bytes
#define DBGINF_SCHED_RECORD_SIZE       (uint16_t)12    //!< Scheduler record size in bytes
#define DBGINF_SCHED_HISTORY_SIZE      (uint16_t)( DBGINF_SCHED_RECORD_SIZE * DBGINF_SCHED_MAX_NUM_OF_RECORDS ) //!< Scheduler history size in bytes
#define DBGINF_SCHED_TOTAL_SIZE        (uint16_t)( DBGINF_SCHED_GEN_INFO_SIZE + DBGINF_SCHED_HISTORY_SIZE)      //!< Scheduler domain size in bytes

/************************************************/
/*********** Connection domain MACROS  **********/
/************************************************/

// Connection domain defines
#define DBGINF_CONN_FLAG_ACTIVE        (uint8_t)BVU8(0)                 //!< Flag to indicate if this connection is active
#define DBGINF_CONN_FLAG_BLE_ROLE      (uint8_t)BVU8(1)                 //!< Flag to indicate if the connection formed as Central (0) or Peripheral (1)
#define DBGINF_CONN_FLAG_ENC           (uint8_t)BVU8(2)                 //!< Flag to indicate if encryption is enabled for this connection
#define DBGINF_CONN_PERI_ROLE          (uint8_t)1                       //!< Flag to indicate BLE connection role as peripheral

#define DBGINF_CONN_NUM_OF_TERM_STAT   (uint8_t)13                      //!< Number of disconnect reason codes
#define DBGINF_CONN_GEN_INFO_SIZE      (uint16_t)(6 + 2*DBGINF_CONN_NUM_OF_TERM_STAT)     //!< Connection domain general data size in bytes
#define DBGINF_CONN_EST_INFO_SIZE      (uint16_t)(4*DBGINF_CONN_MAX_NUM_OF_EST_RECORDS)   //!< Connection established data size in bytes
#define DBGINF_CONN_TERM_INFO_SIZE     (uint16_t)(12*DBGINF_CONN_MAX_NUM_OF_TERM_RECORDS) //!< Connection termination data size in bytes
#define DBGINF_CONN_TOTAL_SIZE         (uint16_t)(DBGINF_CONN_GEN_INFO_SIZE + DBGINF_CONN_EST_INFO_SIZE + DBGINF_CONN_TERM_INFO_SIZE)//!< Connection domain size in bytes

// Disconnect Reason Codes - controller error codes
#define DBGINF_CONN_TERM_LL_AUTH_FAILURE                   (uint8_t)(0x05)    //!< Authentication failure
#define DBGINF_CONN_TERM_LL_CONN_TIMEOUT                   (uint8_t)(0x08)    //!< Connection timeout
#define DBGINF_CONN_TERM_LL_PEER_TERM                      (uint8_t)(0x13)    //!< Remote user terminated connection
#define DBGINF_CONN_TERM_LL_PEER_TERM_LOW_RESOURCES        (uint8_t)(0x14)    //!< Remote device terminated connection due to low resources
#define DBGINF_CONN_TERM_LL_PEER_TERM_POWER_OFF            (uint8_t)(0x15)    //!< Remote device terminated connection due to power off
#define DBGINF_CONN_TERM_LL_HOST_TERM                      (uint8_t)(0x16)    //!< Connection terminated by local host
#define DBGINF_CONN_TERM_LL_UNSUPPORTED_REMOTE_FEATURE     (uint8_t)(0x1A)    //!< Unsupported remote feature
#define DBGINF_CONN_TERM_LL_TIMEOUT                        (uint8_t)(0x22)    //!< Link layer response timeout
#define DBGINF_CONN_TERM_LL_INSTANT_PASSED                 (uint8_t)(0x28)    //!< Instant passed
#define DBGINF_CONN_TERM_LL_KEY_PAIRING_NOT_SUPPORTED      (uint8_t)(0x29)    //!< Pairing with unit key not supported
#define DBGINF_CONN_TERM_LL_UNACCEPTABLE_CONN_PARAMETERS   (uint8_t)(0x3B)    //!< Unacceptable connection parameters
#define DBGINF_CONN_TERM_LL_MIC_FAILURE                    (uint8_t)(0x3D)    //!< Connection terminated due to MIC failure
#define DBGINF_CONN_TERM_LL_CONN_FAILED_TO_BE_ESTABLISHED  (uint8_t)(0x3E)    //!< Connection failed to be established

// Connection termination statistics indexes
#define DBGINF_CONN_TERM_AUTH_FAILURE                    (uint8_t)0    //!< Authentication failure
#define DBGINF_CONN_TERM_CONN_TIMEOUT                    (uint8_t)1    //!< Connection timeout
#define DBGINF_CONN_TERM_PEER_TERM                       (uint8_t)2    //!< Remote user terminated connection
#define DBGINF_CONN_TERM_PEER_TERM_LOW_RESOURCES         (uint8_t)3    //!< Remote device terminated connection due to low resources
#define DBGINF_CONN_TERM_PEER_TERM_POWER_OFF             (uint8_t)4    //!< Remote device terminated connection due to power off
#define DBGINF_CONN_TERM_HOST_TERM                       (uint8_t)5    //!< Connection terminated by local host
#define DBGINF_CONN_TERM_UNSUPPORTED_REMOTE_FEATURE      (uint8_t)6    //!< Unsupported remote feature
#define DBGINF_CONN_TERM_TIMEOUT                         (uint8_t)7    //!< Link layer response timeout
#define DBGINF_CONN_TERM_INSTANT_PASSED                  (uint8_t)8    //!< Instant passed
#define DBGINF_CONN_TERM_KEY_PAIRING_NOT_SUPPORTED       (uint8_t)9    //!< Pairing with unit key not supported
#define DBGINF_CONN_TERM_UNACCEPTABLE_CONN_PARAMETERS    (uint8_t)10   //!< Unacceptable connection parameters
#define DBGINF_CONN_TERM_MIC_FAILURE                     (uint8_t)11   //!< Connection terminated due to MIC failure
#define DBGINF_CONN_TERM_CONN_FAILED_TO_BE_ESTABLISHED   (uint8_t)12   //!< Connection failed to be established

/************************************************/
/************* Error domain MACROS  *************/
/************************************************/

#define DBGINF_ERROR_GEN_INFO_SIZE      (uint16_t)2     //!< Connection domain general data size in bytes
#define DBGINF_ERROR_TOTAL_SIZE         (uint16_t)(DBGINF_ERROR_GEN_INFO_SIZE + 2*DBGINF_ERROR_MAX_NUM_OF_ERROR_RECORDS) //!< Error domain size in bytes

/// @endcond // NODOC

/*********************************************************************
 * Enumerators
 */

/**
 *  @defgroup HealthTK_DebugInfo_Enumerators_Typedefs DebugInfo Enumerators Typedefs
 *  @brief This module implements DebugInfo Enumerators Typedefs
 *  @{
 */


/** @} End BLEAppUtil_Enumerators_Typedefs */

/*********************************************************************
 * Structures
 */

/**
 *  @defgroup DbgInf_Structures Debug Info Structures
 *  @brief This module implements Debug Info structures
 *  @{
 */

/************************************************/
/********  General debug info structures  *******/
/************************************************/

/**
 * @brief DebugInfo header data structure
 *
 * Used to store the debug info header for @ref DbgInf_get
 */
typedef struct DbgInf_getHdr_t_
{
 uint32_t      waterMark;      //!< Water mark
 uint16_t      len;            //!< Number of bytes read from debug info
 uint16_t      bitmap;         //!< Bits that indicate which domain are included in the data
} DbgInf_getHdr_t;


/************************************************/
/*********  Scheduler domain structures  ********/
/************************************************/

/**
 * @brief  DebugInfo scheduler record data structure
 *
 * Used to store the data of scheduler record
 */
typedef struct DbgInf_schedRec_t_
{
  uint8_t      taskID;           //!< Type of LL task
  uint8_t      llState;          //!< State of LL
  uint16_t     reserved;
  uint32_t     startTimeDelta;   //!< The time difference between current task start time and previous task end time
  uint32_t     taskDuration;     //!< Task duration
} DbgInf_schedRec_t;

/**
 * @brief DebugInfo scheduler domain data structure
 *
 * Used to store the data of scheduler domain
 */
typedef struct DbgInf_SchedInfo_t_
{
  uint8_t             activeTasks;       //!< Bits that indicate which tasks are active
  uint8_t             recCnt;            //!< Scheduler records counter
  uint16_t            reserved;
  uint32_t            lastCmdStartTime;  //!< The start time of the last scheduled command
  DbgInf_schedRec_t   schedHistory[DBGINF_SCHED_MAX_NUM_OF_RECORDS];    //!< Cyclic buffer of scheduler records
} DbgInf_SchedInfo_t;


/************************************************/
/*********  Connection domain structures  ********/
/************************************************/

/**
 * @brief Connection establishment data structure
 *
 * Used to store data of active connections
 */
typedef struct DbgInf_ConnEst_t_
{
  uint8_t    flags;        //!< Connection flags bitmap:
                           //!< 0: connection active (1) or inactive (0)
                           //!< 1: peripheral (1), central (0)
                           //!< 2: encryption enabled (1) or disabled (0)
  uint8_t    reserved;
  uint16_t   eventNumEst;  //!< Event number for connection establish
}DbgInf_ConnEst_t;

/**
 * @brief DebugInfo connection termination data structure
 *
 * Used to store data of previous connections
 */
typedef struct DbgInf_ConnTerm_t_
{
  uint8_t    peerAddr[2];      //!< 2 LSB bytes of peer device address
  uint8_t    termReason;       //!< Termination reason code
  uint8_t    flags;            //!< Connection flags
  uint16_t   connInterval;     //!< Connection interval
  uint16_t   connEvent;        //!< Current event number
  uint16_t   eventNumEst;      //!< Event number for connection establish
  uint16_t   eventNumTerm;     //!< Event number for connection terminate
}DbgInf_ConnTerm_t;

/**
 * @brief DebugInfo connection domain data structure
 *
 * Used to store the data of connection domain
 */
typedef struct DbgInf_ConnInfo_t_
{
  uint8_t           numActiveConns;      //!< Number of current active BLE connections
  uint8_t           maxActiveConns;      //!< Maximum of active BLE connections reported
  uint16_t          eventCtr;            //!< Connection domain event number
  uint16_t          termReasonStat[DBGINF_CONN_NUM_OF_TERM_STAT];   //!< Disconnect reason statistics
  uint8_t           connTermCnt;         //!< Connection terminated records counter
  uint8_t           reserved;
  DbgInf_ConnEst_t  connEst[DBGINF_CONN_MAX_NUM_OF_EST_RECORDS];    //!< Connection information
  DbgInf_ConnTerm_t connTerm[DBGINF_CONN_MAX_NUM_OF_TERM_RECORDS];  //!< Previous connections records
} DbgInf_ConnInfo_t;

/************************************************/
/*********  Error domain structures  ********/
/************************************************/

/**
 * @brief DebugInfo error domain data structure
 *
 * Used to store the data of error domain
 */
typedef struct DbgInf_ErrorInfo_t_
{
  uint8_t      errorCnt;     //!< Errors records counter
  uint8_t      reserved;
  uint16       errorHistory[DBGINF_ERROR_MAX_NUM_OF_ERROR_RECORDS];   //!< Errors records
} DbgInf_ErrorInfo_t;

/** @} End DebugInfo_Structures */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/**
 *  @defgroup DbgInf_Functions Debug Info Functions
 *  @brief This module implements Debug Info functions
 *  @{
 */

/**
 * @brief   Initialize the debug info data
 *
 * @param   domainBitmap - bits to indicate which domain to initialize.
 *                       - DBGINF_DOMAIN_NONE
 *                       - DBGINF_DOMAIN_SCHEDULER
 *                       - DBGINF_DOMAIN_CONNECTION
 *                       - DBGINF_DOMAIN_ERROR
 *                       - DBGINF_DOMAIN_ALL
 *
 * @return  SUCCESS
 */
int32_t DbgInf_init( uint16_t domainBitmap );

/**
 * @brief   Clear Debug Info data
 *
 * @param   domainBitmap - bits to indicate which domain to clear
 *                       - DBGINF_DOMAIN_NONE
 *                       - DBGINF_DOMAIN_SCHEDULER
 *                       - DBGINF_DOMAIN_CONNECTION
 *                       - DBGINF_DOMAIN_ERROR
 *                       - DBGINF_DOMAIN_ALL
 *
 * @return  SUCCESS
 */
int32_t DbgInf_clear( uint16_t domainBitmap );

/**
 * @brief   Writes debug info data to a buffer
 *
 * @param   pBuf - Pointer to write the debug info data
 * @param   len - Number of bytes to write into pBuf
 * @param   reqDomainBitmap - bits to indicate which domain to read from
 *
 * @return  Bytes read from debug info data
 */
uint16_t DbgInf_get( uint8_t * const pBuf, uint16_t len, uint16_t reqDomainBitmap );

/**
 * @brief   Halt the debug info module
 *
 * @param   None
 *
 * @return  None
 */
int32_t DbgInf_halt( void );

/** @} End DbgInf_Functions */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* DEBUGINFO_H */

/** @} End DebugInfo */
