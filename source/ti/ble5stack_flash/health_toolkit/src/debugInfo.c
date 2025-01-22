/******************************************************************************

@file  debugInfo.c

@brief This file contains the DebugInfo module task function and related
       functionality

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


/*********************************************************************
 * INCLUDES
 */
#ifdef BLE_HEALTH
#include <health_toolkit/inc/debugInfo.h>
#include <health_toolkit/inc/debugInfo_internal.h>
#include "osal.h"
#include "map_direct.h"
#include "hal_mcu.h"
#include <ti/drivers/utils/Math.h>

/*********************************************************************
 * MACROS
 */


/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/

// Debug info parameters
typedef struct DbgInf_params_t_
{
  uint16_t activeDomains;   // bits to indicate which domains are active
  uint8_t  enable;          // bit to indicate if the debug info module is enabled
} DbgInf_params_t;

// Init function type for each debug info domain
typedef void (*DbgInf_initDomainFunc_t)(void);

// Get function type for each debug info domain
typedef uint16_t (*DbgInf_getDomainFunc_t)( uint8_t *pBuf, uint16_t len);

// Clear function type for each debug info domain
typedef void (*DbgInf_clearDomainFunc_t)(void);

/*********************************************************************
* GLOBAL VARIABLES
*/

/*********************************************************************
* LOCAL VARIABLES
*/

DbgInf_params_t      DbgInf_domainParams = {0};      // bitmap indicates which domains are active
DbgInf_SchedInfo_t   DbgInf_schedInfo = {0};         // scheduler domain data
DbgInf_ConnInfo_t    DbgInf_connInfo = {0};          // connection domain data
DbgInf_ErrorInfo_t   DbgInf_errorInfo = {0};         // error domain data

/*********************************************************************
* LOCAL FUNCTIONS - DECLERATION
*/

uint8_t DbgInf_updateDomainHdr(uint8_t *pBuf, uint16_t domainSize, uint16_t domainType);
uint8_t DbgInf_getHdr( uint8_t *pBuf, uint16_t domainBitmap ,uint16_t len );
uint8_t DbgInf_isDomainActive( uint16_t domain );

void DbgInf_initSched(void);
void DbgInf_clearSched(void);
uint16_t DbgInf_getSchedData( uint8_t *pBuf, uint16_t len );

void DbgInf_initConn( void );
void DbgInf_clearConn(void);
uint16_t DbgInf_getConnData( uint8_t *pBuf, uint16_t len );
uint8_t DbgInf_getTermReasonIndex( uint8_t termReason );

void DbgInf_initError(void);
void DbgInf_clearError(void);
uint16_t DbgInf_getErrorData( uint8_t *pBuf, uint16_t len );

/*************  Functions pointers  *************/

// Init function type for each debug info domain
const DbgInf_initDomainFunc_t DbgInf_initDomainTable[] =
{
  DbgInf_initSched,
  DbgInf_initConn,
  DbgInf_initError
};

// Get function type for each debug info domain
const DbgInf_getDomainFunc_t DbgInf_getDomainTable[] =
{
 DbgInf_getSchedData,
 DbgInf_getConnData,
 DbgInf_getErrorData
};

// Clear function type for each debug info domain
const DbgInf_clearDomainFunc_t DbgInf_clearDomainTable[] =
{
  DbgInf_clearSched,
  DbgInf_clearConn,
  DbgInf_clearError
};

/*********************************************************************
 * EXTERN FUNCTIONS
*/

/*********************************************************************
* CALLBACKS
*/

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      DbgInf_init
 *
 * @brief   Initialize the Debug Info data
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
int32_t DbgInf_init(uint16_t domainBitmap)
{
  int32_t status = SUCCESS;
  uint16_t domainsToInit = 0U;

  // Clear active domains
  DbgInf_domainParams.activeDomains = 0U;

  // Domains bitmap for init
  domainsToInit |= ( domainBitmap & DBGINF_DOMAIN_VALID );

  // Check if there are valid domains for initialize
  if ( domainsToInit != DBGINF_DOMAIN_NONE )
  {
    // Go over all domains
    for ( uint8_t i = 0; i < DBGINF_MAX_NUM_DOMAINS; i++ )
    {
      // Check if the domain is active and init function is existing
      if ((( BVU16(i) & domainsToInit) != DBGINF_DOMAIN_NONE ) &&
           ( DbgInf_initDomainTable[i] != NULL ))
      {
        // Init debug info domain
        (void)DbgInf_initDomainTable[i]();
      }
    }

    // activate the debug info module
    DbgInf_domainParams.enable = UTRUE;
  }

  // Return status value
  return (status);
}

/*********************************************************************
 * @fn      DbgInf_clear
 *
 * @brief   Clear Debug Info data
 *
 * @param   domainBitmap - bits to indicate which domain to clear
 *                       - DBGINF_DOMAIN_NONE
 *                       - DBGINF_DOMAIN_SCHEDULER
 *                       - DBGINF_DOMAIN_CONNECTION
 *                       - DBGINF_DOMAIN_ERROR
 *                       - DBGINF_DOMAIN_ALL
 *
 * @status  @ref SUCCESS
 */
int32_t DbgInf_clear(uint16_t domainBitmap)
{
  int32_t status = SUCCESS;
  uint16_t domainsToClear = 0;

  // Domains bitmap for clear
  domainsToClear |= ( domainBitmap & DbgInf_domainParams.activeDomains );

  // Check if there are valid domains for clear
  if ( domainsToClear != DBGINF_DOMAIN_NONE )
  {
    // Go over all domains
    for ( uint8_t i = 0; i < DBGINF_MAX_NUM_DOMAINS; i++ )
    {
      // Check if the domain is active and clear function is existing
      if (( DBGINF_DOMAIN_NONE != ( BVU16(i) & domainsToClear )) &&
          ( DbgInf_clearDomainTable[i] != NULL ))
      {
        // Clear debug info domain data
        (void)DbgInf_clearDomainTable[i]();
      }
    }
  }

  // Return status value
  return (status);
}

/*********************************************************************
 * @fn      DbgInf_get
 *
 * @brief   Writes debug info data to a buffer
 *
 * @param   pBuf - Pointer to write the debug info data
 * @param   len - Number of bytes to write into pBuf
 * @param   reqDomainBitmap - bits to indicate which domain to read from
 *
 * @status  In case of success: Bytes read from debug info data
 * @return  In case of invalid input: 0
 */
uint16_t DbgInf_get( uint8_t * const pBuf, uint16_t len, uint16_t reqDomainBitmap )
{
  uint16_t hdrDomainBitmap = DBGINF_DOMAIN_NONE;
  uint16_t dataLen = 0;
  uint16_t domainToRead = 0;
  uint8_t  *pBufData = pBuf;

 // Domains bitmap for read
  domainToRead |= reqDomainBitmap & DbgInf_domainParams.activeDomains;

  // Validate input
  if (( domainToRead != DBGINF_DOMAIN_NONE ) &&
      ( pBuf != NULL ) &&
      ( len > DBGINF_GEN_DATA_HDR_LEN ))
  {
    // Copy debug info data after the header
    dataLen += DBGINF_GEN_DATA_HDR_LEN;

    // Go over all domains
    for (uint8_t i = 0; i < DBGINF_MAX_NUM_DOMAINS; i++ )
    {
      // Check len size, if the domain is active and clear function is existing
      if (( len > (dataLen + DBGINF_GEN_DOMAIN_HDR_LEN )) &&
          ( DBGINF_DOMAIN_NONE != ( BVU16(i) & domainToRead )) &&
          ( DbgInf_getDomainTable[i] != NULL ))
      {
        // Set the maximum bytes available to read from the current domain
        uint16_t domainSize = len - (dataLen + DBGINF_GEN_DOMAIN_HDR_LEN);

        // Read data from the requested domains
        domainSize = DbgInf_getDomainTable[i](&pBufData[dataLen + DBGINF_GEN_DOMAIN_HDR_LEN] , domainSize);

        // Copy domain header
        if( DbgInf_updateDomainHdr( &pBufData[dataLen], domainSize, BVU16(i)) == USUCCESS)
        {
          // Update data size
          dataLen += domainSize + DBGINF_GEN_DOMAIN_HDR_LEN;

          // Update header bitmap with this domain
          hdrDomainBitmap |= BVU16(i);
        }
      }
    }

    // Set debug info header
    (void)DbgInf_getHdr( pBuf, hdrDomainBitmap, dataLen );
  }

  // Return data length value
  return (dataLen);
}

/*********************************************************************
 * @fn      DbgInf_halt
 *
 * @brief   Halt the debug info module
 *
 * @param   None
 *
 * @return  SUCCESS
 */
int32_t DbgInf_halt( void )
{
  // Clear debug info enable bit
  DbgInf_domainParams.enable = UFALSE;

  return (SUCCESS);
}

/*********************************************************************
* LOCAL FUNCTIONS - IMPLEMENTAION
*/

/*********************************************************************
 * @fn      DbgInf_getDominHdr
 *
 * @brief   Write domain header (domain name and size) to a buffer
 *
 * @param   pBuf - Pointer to write the domain header data
 * @param   domainSize - domain data size
 * @param   domainType - domain type
 *
 * @return  @ref SUCCESS
 * @return  @ref INVALIDPARAMETER : the input is not valid
 */
uint8_t DbgInf_updateDomainHdr(uint8_t *pBuf, uint16_t domainSize, uint16_t domainType)
{
  uint8_t status = USUCCESS;

  // Validate input
  if ( pBuf == NULL )
  {
    status = INVALIDPARAMETER;
  }
  else
  {
    // Copy domain name and size to the header
    (void)MAP_osal_memcpy( pBuf , &domainType, DBGINF_GEN_BITMAP_LEN );
    (void)MAP_osal_memcpy( &pBuf[DBGINF_GEN_BITMAP_LEN] , &domainSize, DBGINF_GEN_BITMAP_LEN );
  }

  // Return status value
  return (status);
}

/*********************************************************************
 * @fn      DbgInf_getHdr
 *
 * @brief   Get debug info header
 *
 * @param   pBuf - pointer to write the debug info header
 * @param   domainBitmap - bits to indicate which domain to clear
 *                       - DBGINF_DOMAIN_NONE
 *                       - DBGINF_DOMAIN_SCHEDULER
 *                       - DBGINF_DOMAIN_CONNECTION
 *                       - DBGINF_DOMAIN_ERROR
 *                       - DBGINF_DOMAIN_ALL
 * @param   len - number of bytes read from debug info
 *
 * @return  @ref SUCCESS
 * @return  @ref INVALIDPARAMETER : the input is not valid
 */
uint8_t DbgInf_getHdr( uint8_t *pBuf, uint16_t domainBitmap ,uint16_t len )
{
  uint8_t status = USUCCESS;
  DbgInf_getHdr_t hdr;

  // Validate input
  if (( pBuf == NULL ) && ( domainBitmap != DBGINF_DOMAIN_NONE ))
  {
    status = INVALIDPARAMETER;
  }
  else
  {
    // Set header
    hdr.waterMark = DBGINF_GEN_WATERMARK;
    hdr.bitmap = domainBitmap;
    hdr.len = len;

    // Copy header
    (void)MAP_osal_memcpy( pBuf, &hdr, DBGINF_GEN_DATA_HDR_LEN );
  }

  // Return status value
  return (status);
}

/*********************************************************************
 * @fn      DbgInf_isDomainActive
 *
 * @brief   Check if the debug info module is active and the domain is initialized
 *
 * @param   domainBitmap - bits to indicate which domain to clear
 *                       - DBGINF_DOMAIN_NONE
 *                       - DBGINF_DOMAIN_SCHEDULER
 *                       - DBGINF_DOMAIN_CONNECTION
 *                       - DBGINF_DOMAIN_ERROR
 *                       - DBGINF_DOMAIN_ALL
 *
 * @return  @ref SUCCESS - the debug info module is active and the domain is initialized
 * @return  @ref FAILURE -  the debug info module is not active or the domain is not initialized
 */
uint8_t DbgInf_isDomainActive( uint16_t domain )
{
  uint8_t status = USUCCESS;

  // Check if the debug info module is active and the domain is initialized
  if (( DbgInf_domainParams.enable == UFALSE ) ||
      ( DBGINF_DOMAIN_NONE == ( DbgInf_domainParams.activeDomains & domain )))
  {
    status = UFAILURE;
  }

  // Return status value
  return (status);
}

/********************************************************************/
/******************* Scheduler domain functions  ********************/
/********************************************************************/

/*********************************************************************
 * @fn      DbgInf_initSched
 *
 * @brief   Initialize scheduler domain data
 *
 * @param   None
 *
 * @return  None
 */
void DbgInf_initSched( void )
{
  // Clear scheduler domain data
  (void)DbgInf_clearSched();

  // Set scheduler domain as active
  DbgInf_domainParams.activeDomains |= DBGINF_DOMAIN_SCHEDULER;
}

/*********************************************************************
 * @fn      DbgInf_clearSched
 *
 * @brief   Clear scheduler domain data
 *
 * @param   None
 *
 * @return  None
 */
void DbgInf_clearSched( void )
{
  uint32_t cs;

  HAL_ENTER_CRITICAL_SECTION(cs);

  // Clear scheduler domain data
  (void)MAP_osal_memset( &DbgInf_schedInfo, 0, (int32_t)sizeof(DbgInf_schedInfo) );

  HAL_EXIT_CRITICAL_SECTION(cs);
}

/*********************************************************************
 * @fn      DbgInf_getSchedData
 *
 * @brief   Writes scheduler domain data
 *
 * @param   pBuf - Pointer to write the debug info data
 * @param   len - Number of bytes to write into pBuf
 *
 * @return  In case of success: Bytes read from scheduler domain
 * @return  In case of invalid input: 0
 */
uint16_t DbgInf_getSchedData( uint8_t *pBuf, uint16_t len )
{
  uint16_t dataLen = 0U;
  uint32_t cs;

  // Validate input
  if (( pBuf != NULL ) && ( len > 0U ))
  {
    HAL_ENTER_CRITICAL_SECTION(cs);

    // Copy scheduler domain data
    dataLen = (uint16_t)Math_MIN( len, DBGINF_SCHED_TOTAL_SIZE );
    (void)MAP_osal_memcpy( pBuf, (uint8_t *)&DbgInf_schedInfo, dataLen );

    HAL_EXIT_CRITICAL_SECTION(cs);
  }

  // Return dataLen value
  return ( dataLen );
}

/*********************************************************************
 * @fn      DbgInf_addSchedRec
 *
 * @brief   Adds new scheduler record and updates the scheduler domain
 *
 * @param   newRec - new scheduler record @ref DbgInf_schedNewRec_t
 *
 * @return  @ref SUCCESS
 * @return  @ref INVALIDPARAMETER : the input is not valid
 * @return  @ref FAILURE : the scheduler domain is not initialized or debug info module is not active
 */
uint8_t DbgInf_addSchedRec( DbgInf_schedNewRec_t * const newRec )
{
  uint8_t status = USUCCESS;
  uint8_t index = 0U;
  uint32_t cs;

  // Check if the debug info module is active and the scheduler domain is initialized
  status = DbgInf_isDomainActive(DBGINF_DOMAIN_SCHEDULER);

  // validate input
  if (( status == USUCCESS ) &&
      (( newRec == NULL ) || ( newRec->cmdStartTime < newRec->timeStamp )))
  {
    status = INVALIDPARAMETER;
  }

  if ( status == USUCCESS )
  {
      // Index for the scheduler record
      index = DbgInf_schedInfo.recCnt % DBGINF_SCHED_MAX_NUM_OF_RECORDS;

      HAL_ENTER_CRITICAL_SECTION(cs);

     // First scheduler command
     if (( DbgInf_schedInfo.recCnt == 0U ) &&
         ( DbgInf_schedInfo.lastCmdStartTime == 0U ) &&
         ( DbgInf_schedInfo.schedHistory[0].startTimeDelta == 0U ))
     {

         // The delta of the first command's start time should be 0
         DbgInf_schedInfo.schedHistory[ index ].startTimeDelta = 0U;
     }
     // Not the first scheduler command
     else
     {
         // Set the previous command duration time
         DbgInf_schedInfo.schedHistory[ index ].taskDuration = ( newRec->timeStamp - DbgInf_schedInfo.lastCmdStartTime );

         // Increase the scheduler records counter
         DbgInf_schedInfo.recCnt++;
         if ( DbgInf_schedInfo.recCnt == (uint8_t)(0xFF) )
         {
           DbgInf_schedInfo.recCnt = 0U;
         }

         // update the index for the new scheduler record
         index = DbgInf_schedInfo.recCnt % DBGINF_SCHED_MAX_NUM_OF_RECORDS;

         // Set the new record start time
         DbgInf_schedInfo.schedHistory[ index ].startTimeDelta = ( newRec->cmdStartTime - newRec->timeStamp );

         // Reset the new record task duration
         DbgInf_schedInfo.schedHistory[ index ].taskDuration = 0U;
     }

     // Set new record data
     DbgInf_schedInfo.schedHistory[ index ].taskID = newRec->taskID;
     DbgInf_schedInfo.schedHistory[ index ].llState = newRec->llState;

     // Set general info data
     DbgInf_schedInfo.activeTasks = newRec->activeTasks;
     DbgInf_schedInfo.lastCmdStartTime = newRec->cmdStartTime;

     HAL_EXIT_CRITICAL_SECTION(cs);
  }

  // Return status value
  return (status);
}

/********************************************************************/
/******************* Connection domain functions  *******************/
/********************************************************************/

/*********************************************************************
 * @fn      DbgInf_initConn
 *
 * @brief   Initialize connection domain data
 *
 * @param   None
 *
 * @return  None
 */
void DbgInf_initConn( void )
{
  // Clear connection domain data
  (void)DbgInf_clearConn();

  // Set connection domain as active
  DbgInf_domainParams.activeDomains |= DBGINF_DOMAIN_CONNECTION;
}

/*********************************************************************
 * @fn      DbgInf_clearConn
 *
 * @brief   Clear scheduler domain data
 *
 * @param   None
 *
 * @return  None
 */
void DbgInf_clearConn( void )
{
  uint32_t cs;

  HAL_ENTER_CRITICAL_SECTION(cs);

  // Clear connection domain data
  (void)MAP_osal_memset( &DbgInf_connInfo, 0, (int32_t)sizeof(DbgInf_connInfo) );

  HAL_EXIT_CRITICAL_SECTION(cs);
}

/*********************************************************************
 * @fn      DbgInf_getConnData
 *
 * @brief   Writes scheduler domain data
 *
 * @param   pBuf - Pointer to write the debug info data
 * @param   len - Number of bytes to write into pBuf
 *
 * @return  In case of success: Bytes read from scheduler domain
 * @return  In case of invalid input: 0
 */
uint16_t DbgInf_getConnData( uint8_t *pBuf, uint16_t len )
{
  uint16_t dataLen = 0U;
  uint32_t cs;

  // Validate input
  if (( pBuf != NULL ) && ( len > 0U))
  {
    HAL_ENTER_CRITICAL_SECTION(cs);

    // Copy connection domain data
    dataLen = (uint16_t)Math_MIN( len, DBGINF_CONN_TOTAL_SIZE );
    (void)MAP_osal_memcpy( pBuf, (uint8_t *)&DbgInf_connInfo, dataLen );

    HAL_EXIT_CRITICAL_SECTION(cs);
  }

  // Return dataLen value
  return (dataLen);
}
/*********************************************************************
 * @fn      DbgInf_addConnEst
 *
 * @brief   Adds new connection establishment event to the connection domain
 *
 * @param   connId        - Connection handle
 * @param   connRole      - Connection formed as Central or Peripheral
 * @param   encEnabled    - Flag to indicate that encryption is enabled for this connection
 *
 * @return  @ref SUCCESS
 * @return  @ref INVALIDPARAMETER : the input is not valid
 * @return  @ref FAILURE : the connection domain is not initialized or debug info module is not active
 */
uint8_t DbgInf_addConnEst( uint16_t connId, uint8_t connRole, uint8_t encEnabled )
{
  uint8_t status = USUCCESS;
  uint32_t cs;

  // Check if the debug info module is active and the connection domain is initialized
  status = DbgInf_isDomainActive(DBGINF_DOMAIN_CONNECTION);

  // Validate input
  if (( status == USUCCESS ) && (connId >= (uint8_t)DBGINF_CONN_MAX_NUM_OF_EST_RECORDS ))
  {
    status = INVALIDPARAMETER;
  }

  if ( status == USUCCESS )
  {
    HAL_ENTER_CRITICAL_SECTION(cs);

    // This connection is already active
    if ( ( DbgInf_connInfo.connEst[connId].flags & DBGINF_CONN_FLAG_ACTIVE ) == DBGINF_CONN_FLAG_ACTIVE )
    {
        if ( encEnabled == UTRUE )
        {
          DbgInf_connInfo.connEst[connId].flags |= DBGINF_CONN_FLAG_ENC;
        }
        else
        {
          DbgInf_connInfo.connEst[connId].flags &= (uint8_t)~DBGINF_CONN_FLAG_ENC;
        }
    }
    // This is new connection
    else
    {
      // Mark the connection as active
      DbgInf_connInfo.connEst[connId].flags |= DBGINF_CONN_FLAG_ACTIVE;

      // Set connection role
      if ( connRole == DBGINF_CONN_PERI_ROLE)
      {
        // Set connection BLE role as peripheral
        DbgInf_connInfo.connEst[connId].flags |= DBGINF_CONN_FLAG_BLE_ROLE;
      }

      // Set event num
      DbgInf_connInfo.connEst[connId].eventNumEst = DbgInf_connInfo.eventCtr;

      // Increase the event num
      DbgInf_connInfo.eventCtr++;

      // Update numActiveConns
      DbgInf_connInfo.numActiveConns++;

      // Update maxActiveConns
      DbgInf_connInfo.maxActiveConns = Math_MAX(DbgInf_connInfo.numActiveConns, DbgInf_connInfo.maxActiveConns);
    }

    HAL_EXIT_CRITICAL_SECTION(cs);
  }

  // Return status value
  return (status);
}

/*********************************************************************
 * @fn      DbgInf_addConnTerm
 *
 * @brief   Adds new connection establishment event to the connection domain
 *
 * @param   newRec - new connection establishment record @ref DbgInf_schedNewRec_t
 *
 * @return  @ref SUCCESS
 * @return  @ref INVALIDPARAMETER : the input is not valid
 * @return  @ref FAILURE : the connection domain is not initialized or debug info module is not active
 */
uint8_t DbgInf_addConnTerm( DbgInf_connTermRec_t * const newRec )
{
  uint8_t status = USUCCESS;
  uint8_t termReasonIndex = 0U;
  uint8_t connId = 0U;
  uint8_t index;
  uint32_t cs;

  // Check if the debug info module is active and the connection domain is initialized
  status = DbgInf_isDomainActive(DBGINF_DOMAIN_CONNECTION);

  // Validate input
  if (( status == USUCCESS ) &&
      (( newRec == NULL ) || ( newRec->connId >= (uint8_t)DBGINF_CONN_MAX_NUM_OF_EST_RECORDS )))
  {
    status = INVALIDPARAMETER;
  }

  if ( status == USUCCESS )
  {
    connId = newRec->connId;

    HAL_ENTER_CRITICAL_SECTION(cs);

    // Mark the connection as not active
    DbgInf_connInfo.connEst[connId].flags &= (uint8_t)~DBGINF_CONN_FLAG_ACTIVE;

    index = DbgInf_connInfo.connTermCnt % DBGINF_CONN_MAX_NUM_OF_TERM_RECORDS;

    (void)MAP_osal_memcpy(&DbgInf_connInfo.connTerm[index].peerAddr[0], &newRec->peerAddr[0], 2);
    DbgInf_connInfo.connTerm[index].termReason = newRec->termReason;
    DbgInf_connInfo.connTerm[index].flags = DbgInf_connInfo.connEst[connId].flags;
    DbgInf_connInfo.connTerm[index].connInterval = newRec->connInterval;
    DbgInf_connInfo.connTerm[index].connEvent = newRec->connEvent;
    DbgInf_connInfo.connTerm[index].eventNumEst = DbgInf_connInfo.connEst[connId].eventNumEst;
    DbgInf_connInfo.connTerm[index].eventNumTerm = DbgInf_connInfo.eventCtr;

    // Update event counter
    DbgInf_connInfo.eventCtr++;

    // Update number of active BLE connections
    DbgInf_connInfo.numActiveConns--;

    // Update disconnect reason statistics
    termReasonIndex = DbgInf_getTermReasonIndex(DbgInf_connInfo.connTerm[index].termReason);
    if (termReasonIndex < DBGINF_CONN_NUM_OF_TERM_STAT)
    {
      DbgInf_connInfo.termReasonStat[termReasonIndex]++;
    }

    // Update connection terminated records counter
    DbgInf_connInfo.connTermCnt++;
    if ( DbgInf_connInfo.connTermCnt == (uint8_t)(0xFF) )
    {
      DbgInf_connInfo.connTermCnt = 0U;
    }

    HAL_EXIT_CRITICAL_SECTION(cs);
  }

  // Return status value
  return ( status );
}

/*********************************************************************
 * @fn      DbgInf_getTermReasonIndex
 *
 * @brief   Convert ll terminate reason code to debug info 'termReasonStat' index
 *
 * @param   termReason - LL terminate reason code
 *
 * @return  @ref debug info 'termReasonStat' index
 * @return  @ref DBGINF_CONN_NUM_OF_TERM_STAT : if the input is not valid
 */
uint8_t DbgInf_getTermReasonIndex(uint8_t termReason)
{
  uint8_t termReasonIndex = DBGINF_CONN_NUM_OF_TERM_STAT;

  switch( termReason )
  {
    // Authentication failure
    case DBGINF_CONN_TERM_LL_AUTH_FAILURE:
    {
      termReasonIndex = DBGINF_CONN_TERM_AUTH_FAILURE;
    }
    break;

    // Connection timeout
    case DBGINF_CONN_TERM_LL_CONN_TIMEOUT:
    {
      termReasonIndex = DBGINF_CONN_TERM_CONN_TIMEOUT;
    }
    break;

    // Remote user terminated connection
    case DBGINF_CONN_TERM_LL_PEER_TERM:
    {
      termReasonIndex = DBGINF_CONN_TERM_PEER_TERM;
    }
    break;

    // Remote device terminated connection due to low resources
    case DBGINF_CONN_TERM_LL_PEER_TERM_LOW_RESOURCES:
    {
      termReasonIndex = DBGINF_CONN_TERM_PEER_TERM_LOW_RESOURCES;
    }
    break;

    // Remote device terminated connection due to power off
    case DBGINF_CONN_TERM_LL_PEER_TERM_POWER_OFF:
    {
      termReasonIndex = DBGINF_CONN_TERM_PEER_TERM_POWER_OFF;
    }
    break;

    // Connection terminated by local host
    case DBGINF_CONN_TERM_LL_HOST_TERM:
    {
      termReasonIndex = DBGINF_CONN_TERM_HOST_TERM;
    }
    break;

    // Unsupported remote feature
    case DBGINF_CONN_TERM_LL_UNSUPPORTED_REMOTE_FEATURE:
    {
      termReasonIndex = DBGINF_CONN_TERM_UNSUPPORTED_REMOTE_FEATURE;
    }
    break;

    // Link layer response timeout
    case DBGINF_CONN_TERM_LL_TIMEOUT:
    {
      termReasonIndex = DBGINF_CONN_TERM_TIMEOUT;
    }
    break;

    // Instant passed
    case DBGINF_CONN_TERM_LL_INSTANT_PASSED:
    {
      termReasonIndex = DBGINF_CONN_TERM_INSTANT_PASSED;
    }
    break;

    // Pairing with unit key not supported
    case DBGINF_CONN_TERM_LL_KEY_PAIRING_NOT_SUPPORTED:
    {
      termReasonIndex = DBGINF_CONN_TERM_KEY_PAIRING_NOT_SUPPORTED;
    }
    break;

    // Unacceptable connection parameters
    case DBGINF_CONN_TERM_LL_UNACCEPTABLE_CONN_PARAMETERS:
    {
      termReasonIndex = DBGINF_CONN_TERM_UNACCEPTABLE_CONN_PARAMETERS;
    }
    break;

    // Connection terminated due to MIC failure
    case DBGINF_CONN_TERM_LL_MIC_FAILURE:
    {
      termReasonIndex = DBGINF_CONN_TERM_MIC_FAILURE;
    }
    break;

    // Connection failed to be established
    case DBGINF_CONN_TERM_LL_CONN_FAILED_TO_BE_ESTABLISHED:
    {
      termReasonIndex = DBGINF_CONN_TERM_CONN_FAILED_TO_BE_ESTABLISHED;
    }
    break;

    default:
    break;
  }

  // Return terminate reason index
  return ( termReasonIndex );
}

/********************************************************************/
/********************** Error domain functions  *********************/
/********************************************************************/

/*********************************************************************
 * @fn      DbgInf_initError
 *
 * @brief   Initialize error domain data
 *
 * @param   None
 *
 * @return  None
 */
void DbgInf_initError( void )
{
  // Clear error domain data
  (void)DbgInf_clearError();

  // Set error domain as active
  DbgInf_domainParams.activeDomains |= DBGINF_DOMAIN_ERROR;
}

/*********************************************************************
 * @fn      DbgInf_clearSched
 *
 * @brief   Clear error domain data
 *
 * @param   None
 *
 * @return  None
 */
void DbgInf_clearError( void )
{
  uint32_t cs;

  HAL_ENTER_CRITICAL_SECTION(cs);

  // Clear error domain data
  (void)MAP_osal_memset( &DbgInf_schedInfo, 0, (int32_t)sizeof(DbgInf_errorInfo) );

  HAL_EXIT_CRITICAL_SECTION(cs);
}

/*********************************************************************
 * @fn      DbgInf_getErrorData
 *
 * @brief   Writes error domain data
 *
 * @param   pBuf - Pointer to write the debug info data
 * @param   len - Number of bytes to write into pBuf
 *
 * @return  In case of success: Bytes read from scheduler domain
 * @return  In case of invalid input: 0
 */
uint16_t DbgInf_getErrorData( uint8_t *pBuf, uint16_t len )
{
  uint16_t dataLen = 0U;
  uint32_t cs;

  // Validate input
  if (( pBuf != NULL ) && ( len > 0))
  {
    HAL_ENTER_CRITICAL_SECTION(cs);

    // Copy error domain data
    dataLen = (uint16_t)Math_MIN( len, DBGINF_ERROR_TOTAL_SIZE);
    (void)MAP_osal_memcpy( pBuf, (uint8_t *)&DbgInf_errorInfo, dataLen );

    HAL_EXIT_CRITICAL_SECTION(cs);
  }

  // Return dataLen value
  return ( dataLen );
}

/*********************************************************************
 * @fn      DbgInf_addError
 *
 * @brief   Adds new error record to the error domain
 *
 * @param   newRec - new error record
 *
 * @return  @ref SUCCESS
 * @return  @ref FAILURE : the error domain is not initialized or debug info module is not active
 */
uint8_t DbgInf_addErrorRec( uint16_t newError )
{
  uint8_t status = USUCCESS;
  uint8_t index = 0U;
  uint32_t cs;

  // Check if the debug info module is active and the error domain is initialized
  status = DbgInf_isDomainActive(DBGINF_DOMAIN_ERROR);

  if ( status == USUCCESS )
  {
    // index for the error record
    index = DbgInf_errorInfo.errorCnt % DBGINF_ERROR_MAX_NUM_OF_ERROR_RECORDS;

    HAL_ENTER_CRITICAL_SECTION(cs);

    DbgInf_errorInfo.errorHistory[index] = newError;

    // Increase the error records counter
    DbgInf_errorInfo.errorCnt++;
    if ( DbgInf_errorInfo.errorCnt == (uint8_t)(0xFF) )
    {
      DbgInf_errorInfo.errorCnt = 0U;
    }

    HAL_EXIT_CRITICAL_SECTION(cs);
  }

  // Return status value
  return (status);
}

#endif // BLE_HEALTH
