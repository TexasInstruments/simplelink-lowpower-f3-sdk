/******************************************************************************

 @file  osal.c

 @brief This API allows the software components in the Z-stack to be written
        independently of the specifics of the operating system, kernel or
        tasking environment (including control loops or connect-to-interrupt
        systems).

 Group: WCS, LPC, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2004-2025, Texas Instruments Incorporated
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
#include "ti/ble/stack_util/osal/osal_memory.h"
#include "ti/ble/stack_util/osal/osal_timers.h"
#include <string.h>
#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/stack_util/osal/osal.h"
#include "ti/ble/stack_util/osal/osal_tasks.h"
#include "ti/ble/stack_util/osal/osal_pwrmgr.h"
#include "ti/ble/stack_util/osal/osal_clock.h"

#ifdef IAR_ARMCM3_LM
  #include "FreeRTOSConfig.h"
#endif

#include "ti/ble/stack_util/icall/app/icall.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
// A bit mask to use to indicate a proxy OSAL task ID.
#define OSAL_PROXY_ID_FLAG       0x80

// Type of event OSAL will send to signal a proxy task
#define OSAL_EVENT_SERVICE      0
#define OSAL_EVENT_MSG          1

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
uint32 *tasksEvents;

// Message Pool Definitions
osal_msg_q_t osal_qHead;

// OSAL event loop hook function pointer
void (*osal_eventloop_hook)(void) = NULL;
osal_icallMsg_hook_t osal_icallMsg_hook = NULL;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Index of active task
static uint8 activeTaskID = TASK_NO_TASK;

// Maximum number of proxy tasks
#ifndef OSAL_MAX_NUM_PROXY_TASKS
#define OSAL_MAX_NUM_PROXY_TASKS 4
#endif // OSAL_MAX_NUM_PROXY_TASKS

// ICall entity ID value used to indicate invalid value
#define OSAL_INVALID_DISPATCH_ID 0xffu

// Synchronous object associated with OSAL RTOS thread receive queue
#ifdef ICALL_EVENTS
ICall_SyncHandle osal_syncHandle;
#else /* !ICALL_EVENTS */
ICall_Semaphore osal_semaphore;
#endif /* ICALL_EVENTS */

// Entity ID that OSAL RTOS thread has registered with
ICall_EntityID osal_entity;

// RTOS tick period in microseconds
uint_least32_t osal_tickperiod;

// Maximum timeout value in milliseconds that can be used with an RTOS timer
uint_least32_t osal_max_msecs;

// Timer ID for RTOS timer as back-end engine for OSAL timer
static ICall_TimerID osal_timerid_msec_timer;

// Timer callback sequence tracking counter to handle race condition
static unsigned osal_msec_timer_seq = 0;

// proxy task ID map
static uint8 osal_proxy_tasks[OSAL_MAX_NUM_PROXY_TASKS];

// service dispatcher entity IDs corresponding to OSAL tasks
static uint8 *osal_dispatch_entities;

static uint8 osal_notask_entity;

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

static uint8 osal_msg_enqueue_push( uint8 destination_task, uint8 *msg_ptr, uint8 urgent );

static ICall_EntityID osal_proxy2alien(uint8 proxyid);
static uint8 osal_dispatch2id(ICall_EntityID entity);

static void osal_msec_timer_cback(void *arg);

/*********************************************************************
 * EXTERN FUNCTION PROTOTYPES
 */

extern void llInitCompleteNotify( int status );

/*********************************************************************
 * @fn      osal_strlen
 *
 * @brief
 *
 *   Calculates the length of a string.  The string must be null
 *   terminated.
 *
 * @param   char *pString - pointer to text string
 *
 * @return  int - number of characters
 */
int osal_strlen( char *pString )
{
  return (int)( strlen( pString ) );
}

/*********************************************************************
 * @fn      osal_memcpy
 *
 * @brief
 *
 *   Generic memory copy.
 *
 *   Note: This function differs from the standard memcpy(), since
 *         it returns the pointer to the next destination uint8. The
 *         standard memcpy() returns the original destination address.
 *
 * @param   dst - destination address
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to end of destination buffer
 */
void *osal_memcpy( void *dst, const void GENERIC *src, unsigned int len )
{
  uint8 *pDst;
  const uint8 GENERIC *pSrc;

  pSrc = src;
  pDst = dst;

  while ( len-- )
    *pDst++ = *pSrc++;

  return ( pDst );
}

/*********************************************************************
 * @fn      osal_revmemcpy
 *
 * @brief   Generic reverse memory copy.  Starts at the end of the
 *   source buffer, by taking the source address pointer and moving
 *   pointer ahead "len" bytes, then decrementing the pointer.
 *
 *   Note: This function differs from the standard memcpy(), since
 *         it returns the pointer to the next destination uint8. The
 *         standard memcpy() returns the original destination address.
 *
 * @param   dst - destination address
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to end of destination buffer
 */
void *osal_revmemcpy( void *dst, const void GENERIC *src, unsigned int len )
{
  uint8 *pDst;
  const uint8 GENERIC *pSrc;

  pSrc = src;
  pSrc += (len-1);
  pDst = dst;

  while ( len-- )
    *pDst++ = *pSrc--;

  return ( pDst );
}

/*********************************************************************
 * @fn      osal_memdup
 *
 * @brief   Allocates a buffer [with osal_mem_alloc()] and copies
 *          the src buffer into the newly allocated space.
 *
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to the new allocated buffer, or NULL if
 *          allocation problem.
 */
void *osal_memdup( const void GENERIC *src, unsigned int len )
{
  uint8 *pDst;

  pDst = osal_mem_alloc( len );
  if ( pDst )
  {
    VOID osal_memcpy( pDst, src, len );
  }

  return ( (void *)pDst );
}

/*********************************************************************
 * @fn      osal_memcmp
 *
 * @brief
 *
 *   Generic memory compare.
 *
 * @param   src1 - source 1 address
 * @param   src2 - source 2 address
 * @param   len - number of bytes to compare
 *
 * @return  TRUE - same, FALSE - different
 */
uint8 osal_memcmp( const void GENERIC *src1, const void GENERIC *src2, unsigned int len )
{
  const uint8 GENERIC *pSrc1;
  const uint8 GENERIC *pSrc2;

  pSrc1 = src1;
  pSrc2 = src2;

  while ( len-- )
  {
    if( *pSrc1++ != *pSrc2++ )
      return FALSE;
  }
  return TRUE;
}


/*********************************************************************
 * @fn      osal_memset
 *
 * @brief
 *
 *   Set memory buffer to value.
 *
 * @param   dest - pointer to buffer
 * @param   value - what to set each uint8 of the message
 * @param   size - how big
 *
 * @return  pointer to destination buffer
 */
void *osal_memset( void *dest, uint8 value, int len )
{
  return memset( dest, value, len );
}

/*********************************************************************
 * @fn      osal_build_uint16
 *
 * @brief
 *
 *   Build a uint16 out of 2 bytes (0 then 1).
 *
 * @param   swapped - 0 then 1
 *
 * @return  uint16
 */
uint16 osal_build_uint16( uint8 *swapped )
{
  return ( BUILD_UINT16( swapped[0], swapped[1] ) );
}

/*********************************************************************
 * @fn      osal_build_uint32
 *
 * @brief
 *
 *   Build a uint32 out of sequential bytes.
 *
 * @param   swapped - sequential bytes
 * @param   len - number of bytes in the uint8 array
 *
 * @return  uint32
 */
uint32_t osal_build_uint32( uint8 *swapped, uint8 len )
{
  if ( len == 2 )
    return ( BUILD_UINT32( swapped[0], swapped[1], 0L, 0L ) );
  else if ( len == 3 )
    return ( BUILD_UINT32( swapped[0], swapped[1], swapped[2], 0L ) );
  else if ( len == 4 )
    return ( BUILD_UINT32( swapped[0], swapped[1], swapped[2], swapped[3] ) );
  else
    return ( (uint32_t)swapped[0] );
}

/*********************************************************************
 * @fn        osal_rand
 *
 * @brief    Random number generator
 *
 * @param   none
 *
 * @return  uint16 - new random number
 */
uint16 osal_rand( void )
{
  extern uint8 LL_Rand( uint8 *, uint8 );
  uint16 randNum;

  LL_Rand( (uint8 *)&randNum, 2 );

  return ( randNum );
}

/*********************************************************************
 * API FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osal_prepare_svc_enroll
 *
 * @brief   Initialize data structures that map OSAL task ids to
 *          ICall entity ids.
 *
 * @param   none
 *
 * @return  none
 */
static void osal_prepare_svc_enroll(void)
{
  osal_dispatch_entities = (uint8 *) osal_mem_alloc(tasksCnt * 2);
  osal_memset(osal_dispatch_entities, OSAL_INVALID_DISPATCH_ID, tasksCnt * 2);
  osal_memset(osal_proxy_tasks, OSAL_INVALID_DISPATCH_ID,
              OSAL_MAX_NUM_PROXY_TASKS);
}

/*********************************************************************
 * @fn      osal_enroll_dispatchid
 *
 * @brief   Map a task id to an ICall entity id for messaging in
 *          both directions (sending and receiving).
 *
 * @param   taskid       OSAL task id
 * @param   dispatchid   ICall entity id
 *
 * @return  none
 */
void osal_enroll_dispatchid(uint8 taskid, ICall_EntityID dispatchid)
{
  osal_dispatch_entities[taskid] = dispatchid;
  osal_dispatch_entities[tasksCnt + taskid] = dispatchid;
}

/*********************************************************************
 * @fn      osal_enroll_senderid
 *
 * @brief   Map a task id to an ICall entity id, which shall be used
 *          just for sending a message from an OSAL task.
 *          Note that osal_enroll_dispatchid() must never be called
 *          with the same OSAL task id used in this function call.
 *          However, it is OK to call osal_enroll_dispatchid()
 *          with the same ICall entity id and a different OSAL task id.
 *
 * @param   taskid       OSAL task id
 * @param   dispatchid   ICall entity id
 *
 * @return  none
 */
void osal_enroll_senderid(uint8 taskid, ICall_EntityID dispatchid)
{
  osal_dispatch_entities[tasksCnt + taskid] = dispatchid;
}

/*********************************************************************
 * @fn      osal_enroll_notasksender
 *
 * @brief   Map no task id to an ICall entity id, which shall be used
 *          just for sending a message from non-OSAL task.
 *
 * @param   dispatchid   ICall entity id
 *
 * @return  none
 */
void osal_enroll_notasksender(ICall_EntityID dispatchid)
{
  osal_notask_entity = dispatchid;
}

/*********************************************************************
 * @fn      osal_msg_allocate
 *
 * @brief
 *
 *    This function is called by a task to allocate a message buffer
 *    into which the task will encode the particular message it wishes
 *    to send.  This common buffer scheme is used to strictly limit the
 *    creation of message buffers within the system due to RAM size
 *    limitations on the microprocessor.   Note that all message buffers
 *    are a fixed size (at least initially).  The parameter len is kept
 *    in case a message pool with varying fixed message sizes is later
 *    created (for example, a pool of message buffers of size LARGE,
 *    MEDIUM and SMALL could be maintained and allocated based on request
 *    from the tasks).
 *
 *
 * @param   uint8 len  - wanted buffer length
 *
 *
 * @return  pointer to allocated buffer or NULL if allocation failed.
 */
uint8 * osal_msg_allocate( uint16 len )
{
  osal_msg_hdr_t *hdr;

  if ( len == 0 )
    return ( NULL );

  hdr = (osal_msg_hdr_t *) osal_mem_alloc( (short)(len + sizeof( osal_msg_hdr_t )) );
  if ( hdr )
  {
    hdr->next = NULL;
    hdr->len = len;
    hdr->dest_id = TASK_NO_TASK;
    return ( (uint8 *) (hdr + 1) );
  }
  else
    return ( NULL );
}

/*********************************************************************
 * @fn      osal_msg_allocateLimited
 *
 * @brief
 *
 *    This function is called by a task to allocate a message buffer
 *    into which the task will encode the particular message it wishes
 *    to send.  This common buffer scheme is used to strictly limit the
 *    creation of message buffers within the system due to RAM size
 *    limitations on the microprocessor.   Note that all message buffers
 *    are a fixed size (at least initially).  The parameter len is kept
 *    in case a message pool with varying fixed message sizes is later
 *    created (for example, a pool of message buffers of size LARGE,
 *    MEDIUM and SMALL could be maintained and allocated based on request
 *    from the tasks).
 *    Note that this function will first check if there is enough heap
 *    memory left after the allocation.
 *
 *
 * @param   uint8 len  - wanted buffer length
 *
 *
 * @return  pointer to allocated buffer or NULL if allocation failed.
 */
uint8 * osal_msg_allocateLimited( uint16 len )
{
  osal_msg_hdr_t *hdr;

  if ( len == 0 )
    return ( NULL );

  hdr = (osal_msg_hdr_t *) osal_mem_allocLimited( (short)(len + sizeof( osal_msg_hdr_t )) );
  if ( hdr )
  {
    hdr->next = NULL;
    hdr->len = len;
    hdr->dest_id = TASK_NO_TASK;
    return ( (uint8 *) (hdr + 1) );
  }
  else
    return ( NULL );
}

/*********************************************************************
 * @fn      osal_msg_deallocate
 *
 * @brief
 *
 *    This function is used to deallocate a message buffer. This function
 *    is called by a task (or processing element) after it has finished
 *    processing a received message.
 *
 *
 * @param   uint8 *msg_ptr - pointer to new message buffer
 *
 * @return  SUCCESS, INVALID_MSG_POINTER
 */
uint8 osal_msg_deallocate( uint8 *msg_ptr )
{
  uint8 *x;

  if ( msg_ptr == NULL )
    return ( INVALID_MSG_POINTER );

  // don't deallocate queued buffer
  if ( OSAL_MSG_ID( msg_ptr ) != TASK_NO_TASK )
    return ( MSG_BUFFER_NOT_AVAIL );

  x = (uint8 *)((uint8 *)msg_ptr - sizeof( osal_msg_hdr_t ));

  osal_mem_free( (void *)x );

  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_event_send
 *
 * @brief
 *
 *    This function is a sub routine for osal_msg_send and
 *    osal_service_copmplete.
 *    since the difference between those 2 functions are only the type of event
 *    to send to the proxy task.
 *
 *
 * @param   uint8 type - Type of event to send: OSAL_EVENT_SERVICE or
 *                       OSAL_EVENT_MSG
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to new message buffer
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
static int8 osal_event_send( uint8 type, uint8 destination_task, uint8 *msg_ptr )
{
    /* Destination is a proxy task */
    osal_msg_hdr_t *hdr = (osal_msg_hdr_t *)msg_ptr - 1;
    ICall_EntityID src, dst;

    uint8 taskid = osal_self();
    if (taskid == TASK_NO_TASK)
    {
      /* Call must have been made from either an ISR or a user-thread */
      src = osal_notask_entity;
    }
    else
    {
      src = (ICall_EntityID) osal_dispatch_entities[taskid + tasksCnt];
    }
    if (src == OSAL_INVALID_DISPATCH_ID)
    {
      /* The source entity is not registered */
      /* abort */
      ICall_abort();
      return FAILURE;
    }
    dst = osal_proxy2alien(destination_task);
    hdr->dest_id = TASK_NO_TASK;
    if (type == OSAL_EVENT_SERVICE)
    {
      if (ICall_sendServiceComplete(src, dst, ICALL_MSG_FORMAT_KEEP, msg_ptr) ==
        ICALL_ERRNO_SUCCESS)
      {
        return SUCCESS;
      }

    }
    else if (type == OSAL_EVENT_MSG)
    {
      if (ICall_send(src, dst, ICALL_MSG_FORMAT_KEEP, msg_ptr) ==
          ICALL_ERRNO_SUCCESS)
      {
        return SUCCESS;
      }
    }
    osal_msg_deallocate(msg_ptr);

    return FAILURE;
}
/*********************************************************************
 * @fn      osal_msg_send
 *
 * @brief
 *
 *    This function is called by a task to send a command message to
 *    another task or processing element.  The sending_task field must
 *    refer to a valid task, since the task ID will be used
 *    for the response message.  This function will also set a message
 *    ready event in the destination tasks event list.
 *
 *
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to new message buffer
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
uint8 osal_msg_send( uint8 destination_task, uint8 *msg_ptr )
{
  BLE_LOG_INT_INT(0, BLE_LOG_MODULE_OSAL_TASK, "OASL: msg send from taskId=%d, to taskId=%d\n", osal_self(), destination_task);

  if (destination_task & OSAL_PROXY_ID_FLAG)
  {
    return( osal_event_send( OSAL_EVENT_MSG, destination_task, msg_ptr ) );
  }
  return ( osal_msg_enqueue_push( destination_task, msg_ptr, FALSE ) );
}

/*********************************************************************
 * @fn      osal_service_complete
 *
 * @brief
 *
 *    This function is called by icall lite translation when it is done
 *     servicing the direct API requested.  The sending_task field
 *    refer the initial calling stack. it is alwasy consider as valid.
 *    This function will send a message to the proxy task.
 *    ready event in the destination tasks event list.
 *
 *
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to new message buffer
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
uint8 osal_service_complete( uint8 destination_task, uint8 *msg_ptr )
{
  if (destination_task & OSAL_PROXY_ID_FLAG)
  {
    return( osal_event_send( OSAL_EVENT_SERVICE, destination_task, msg_ptr ) );
  }
  return FAILURE;
}

/*********************************************************************
 * @fn      osal_msg_push_front
 *
 * @brief
 *
 *    This function is called by a task to push a command message
 *    to the head of the OSAL queue. The destination_task field
 *    must refer to a valid task, since the task ID will be used to
 *    send the message to. This function will also set a message
 *    ready event in the destination task's event list.
 *
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to message buffer
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
uint8 osal_msg_push_front( uint8 destination_task, uint8 *msg_ptr )
{
  return ( osal_msg_enqueue_push( destination_task, msg_ptr, TRUE ) );
}

/*********************************************************************
 * @fn      osal_msg_enqueue_push
 *
 * @brief
 *
 *    This function is called by a task to either enqueue (append to
 *    queue) or push (prepend to queue) a command message to the OSAL
 *    queue. The destination_task field must refer to a valid task,
 *    since the task ID will be used to send the message to. This
 *    function will also set a message ready event in the destination
 *    task's event list.
 *
 * @param   uint8 destination_task - Send msg to Task ID
 * @param   uint8 *msg_ptr - pointer to message buffer
 * @param   uint8 push - TRUE to push, otherwise enqueue
 *
 * @return  SUCCESS, INVALID_TASK, INVALID_MSG_POINTER
 */
static uint8 osal_msg_enqueue_push( uint8 destination_task, uint8 *msg_ptr, uint8 push )
{
  if ( msg_ptr == NULL )
  {
    return ( INVALID_MSG_POINTER );
  }

  if (destination_task & OSAL_PROXY_ID_FLAG)
  {
    ICall_abort();
  }

  if ( destination_task >= tasksCnt )
  {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_TASK );
  }

  // Check the message header
  if ( OSAL_MSG_NEXT( msg_ptr ) != NULL ||
       OSAL_MSG_ID( msg_ptr ) != TASK_NO_TASK )
  {
    osal_msg_deallocate( msg_ptr );
    return ( INVALID_MSG_POINTER );
  }

  OSAL_MSG_ID( msg_ptr ) = destination_task;

  if ( push == TRUE )
  {
    // prepend the message
    osal_msg_push( &osal_qHead, msg_ptr );
  }
  else
  {
    // append the message
    osal_msg_enqueue( &osal_qHead, msg_ptr );
  }

  // Signal the task that a message is waiting
  osal_set_event( destination_task, SYS_EVENT_MSG );

  return ( SUCCESS );
}

/*********************************************************************
 * @fn      osal_msg_receive
 *
 * @brief
 *
 *    This function is called by a task to retrieve a received command
 *    message. The calling task must deallocate the message buffer after
 *    processing the message using the osal_msg_deallocate() call.
 *
 * @param   uint8 task_id - receiving tasks ID
 *
 * @return  *uint8 - message information or NULL if no message
 */
uint8 *osal_msg_receive( uint8 task_id )
{
  osal_msg_hdr_t *listHdr;
  osal_msg_hdr_t *prevHdr = NULL;
  osal_msg_hdr_t *foundHdr = NULL;
  halIntState_t   intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  // Point to the top of the queue
  listHdr = osal_qHead;

  // Look through the queue for a message that belongs to the asking task
  while ( listHdr != NULL )
  {
    if ( (listHdr - 1)->dest_id == task_id )
    {
      if ( foundHdr == NULL )
      {
        // Save the first one
        foundHdr = listHdr;
      }
      else
      {
        // Second msg found, stop looking
        break;
      }
    }
    if ( foundHdr == NULL )
    {
      prevHdr = listHdr;
    }
    listHdr = OSAL_MSG_NEXT( listHdr );
  }

  // Is there more than one?
  if ( listHdr != NULL )
  {
    // Yes, Signal the task that a message is waiting
    osal_set_event( task_id, SYS_EVENT_MSG );
  }
  else
  {
    // No more
    osal_clear_event( task_id, SYS_EVENT_MSG );
  }

  // Did we find a message?
  if ( foundHdr != NULL )
  {
    // Take out of the link list
    osal_msg_extract( &osal_qHead, foundHdr, prevHdr );
  }

  // Release interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);

  return ( (uint8*) foundHdr );
}

/**************************************************************************************************
 * @fn          osal_msg_find
 *
 * @brief       This function finds in place an OSAL message matching the task_id and event
 *              parameters.
 *
 * input parameters
 *
 * @param       task_id - The OSAL task id that the enqueued OSAL message must match.
 * @param       event - The OSAL event id that the enqueued OSAL message must match.
 *
 * output parameters
 *
 * None.
 *
 * @return      NULL if no match, otherwise an in place pointer to the matching OSAL message.
 **************************************************************************************************
 */
osal_event_hdr_t *osal_msg_find(uint8 task_id, uint8 event)
{
  osal_msg_hdr_t *pHdr;
  halIntState_t intState;

  HAL_ENTER_CRITICAL_SECTION(intState);  // Hold off interrupts.

  pHdr = osal_qHead;  // Point to the top of the queue.

  // Look through the queue for a message that matches the task_id and event parameters.
  while (pHdr != NULL)
  {
    if (((pHdr-1)->dest_id == task_id) && (((osal_event_hdr_t *)pHdr)->event == event))
    {
      break;
    }

    pHdr = OSAL_MSG_NEXT(pHdr);
  }

  HAL_EXIT_CRITICAL_SECTION(intState);  // Release interrupts.

  return (osal_event_hdr_t *)pHdr;
}

/**************************************************************************************************
 * @fn          osal_msg_count
 *
 * @brief       This function counts the number of messages, in the OSAL message queue with a
 *              a given task ID and message event type.
 *
 * input parameters
 *
 * @param       task_id - The OSAL task id that the enqueued OSAL message must match.
 * @param       event - The OSAL event id that the enqueued OSAL message must match. 0xFF for
 *              all events.
 *
 * output parameters
 *
 * None.
 *
 * @return      The number of OSAL messages that match the task ID and Event.
 **************************************************************************************************
 */
uint8 osal_msg_count( uint8 task_id, uint8 event )
{
  uint8 count = 0;
  osal_msg_hdr_t *pHdr;
  halIntState_t intState;

  HAL_ENTER_CRITICAL_SECTION(intState);  // Hold off interrupts.

  pHdr = osal_qHead;  // Point to the top of the queue.

  // Look through the queue for a message that matches the task_id and event parameters.
  while (pHdr != NULL)
  {
    if ( ((pHdr-1)->dest_id == task_id)
        && ((event == 0xFF) || (((osal_event_hdr_t *)pHdr)->event == event)) )
    {
      count++;
    }

    pHdr = OSAL_MSG_NEXT(pHdr);
  }

  HAL_EXIT_CRITICAL_SECTION(intState);  // Release interrupts.

  return ( count );
}

/*********************************************************************
 * @fn      osal_msg_enqueue
 *
 * @brief
 *
 *    This function enqueues an OSAL message into an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 *
 * @return  none
 */
void osal_msg_enqueue( osal_msg_q_t *q_ptr, void *msg_ptr )
{
  void *list;
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  OSAL_MSG_NEXT( msg_ptr ) = NULL;
  // If first message in queue
  if ( *q_ptr == NULL )
  {
    *q_ptr = msg_ptr;
  }
  else
  {
    // Find end of queue
    for ( list = *q_ptr; OSAL_MSG_NEXT( list ) != NULL; list = OSAL_MSG_NEXT( list ) );

    // Add message to end of queue
    OSAL_MSG_NEXT( list ) = msg_ptr;
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_dequeue
 *
 * @brief
 *
 *    This function dequeues an OSAL message from an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 *
 * @return  void * - pointer to OSAL message or NULL of queue is empty.
 */
void *osal_msg_dequeue( osal_msg_q_t *q_ptr )
{
  void *msg_ptr = NULL;
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  if ( *q_ptr != NULL )
  {
    // Dequeue message
    msg_ptr = *q_ptr;
    *q_ptr = OSAL_MSG_NEXT( msg_ptr );
    OSAL_MSG_NEXT( msg_ptr ) = NULL;
    OSAL_MSG_ID( msg_ptr ) = TASK_NO_TASK;
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);

  return msg_ptr;
}

/*********************************************************************
 * @fn      osal_msg_push
 *
 * @brief
 *
 *    This function pushes an OSAL message to the head of an OSAL
 *    queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 *
 * @return  none
 */
void osal_msg_push( osal_msg_q_t *q_ptr, void *msg_ptr )
{
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  // Push message to head of queue
  OSAL_MSG_NEXT( msg_ptr ) = *q_ptr;
  *q_ptr = msg_ptr;

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_extract
 *
 * @brief
 *
 *    This function extracts and removes an OSAL message from the
 *    middle of an OSAL queue.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message to be extracted
 * @param   void *prev_ptr  - OSAL message before msg_ptr in queue
 *
 * @return  none
 */
void osal_msg_extract( osal_msg_q_t *q_ptr, void *msg_ptr, void *prev_ptr )
{
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  if ( msg_ptr == *q_ptr )
  {
    // remove from first
    *q_ptr = OSAL_MSG_NEXT( msg_ptr );
  }
  else if (prev_ptr != NULL)
  {
    // remove from middle
    OSAL_MSG_NEXT( prev_ptr ) = OSAL_MSG_NEXT( msg_ptr );
  }
  OSAL_MSG_NEXT( msg_ptr ) = NULL;
  OSAL_MSG_ID( msg_ptr ) = TASK_NO_TASK;

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_msg_enqueue_max
 *
 * @brief
 *
 *    This function enqueues an OSAL message into an OSAL queue if
 *    the length of the queue is less than max.
 *
 * @param   osal_msg_q_t *q_ptr - OSAL queue
 * @param   void *msg_ptr  - OSAL message
 * @param   uint8 max - maximum length of queue
 *
 * @return  TRUE if message was enqueued, FALSE otherwise
 */
uint8 osal_msg_enqueue_max( osal_msg_q_t *q_ptr, void *msg_ptr, uint8 max )
{
  void *list;
  uint8 ret = FALSE;
  halIntState_t intState;

  // Hold off interrupts
  HAL_ENTER_CRITICAL_SECTION(intState);

  // If first message in queue
  if ( *q_ptr == NULL )
  {
    *q_ptr = msg_ptr;
    ret = TRUE;
  }
  else
  {
    // Find end of queue or max
    list = *q_ptr;
    max--;
    while ( (OSAL_MSG_NEXT( list ) != NULL) && (max > 0) )
    {
      list = OSAL_MSG_NEXT( list );
      max--;
    }

    // Add message to end of queue if max not reached
    if ( max != 0 )
    {
      OSAL_MSG_NEXT( list ) = msg_ptr;
      ret = TRUE;
    }
  }

  // Re-enable interrupts
  HAL_EXIT_CRITICAL_SECTION(intState);

  return ret;
}

/*********************************************************************
 * @fn      osal_set_event
 *
 * @brief
 *
 *    This function is called to set the event flags for a task. The
 *    event passed in is OR'd into the task's event variable.
 *
 * @param   uint8 task_id - receiving tasks ID
 * @param   uint8 event_flag - what event to set
 *
 * @return  SUCCESS, MSG_BUFFER_NOT_AVAIL, FAILURE, INVALID_TASK
 */
uint8 osal_set_event( uint8 task_id, uint32 event_flag )
{
  if (task_id & OSAL_PROXY_ID_FLAG)
  {
    /* Destination is a proxy task */
    osal_msg_hdr_t *hdr;
    ICall_EntityID src, dst;
    uint8 taskid;

    taskid = osal_self();
    if (taskid == TASK_NO_TASK)
    {
      /* Call must have been made from either an ISR or a user-thread */
      src = osal_notask_entity;
    }
    else
    {
      src = (ICall_EntityID) osal_dispatch_entities[taskid + tasksCnt];
    }

    if (src == OSAL_INVALID_DISPATCH_ID)
    {
      ICall_abort();
      return FAILURE;
    }

    struct _osal_event_msg_t
    {
      uint16 signature;
      uint32 event_flag;
    } *msg_ptr = (struct _osal_event_msg_t *)
      osal_msg_allocate(sizeof(*msg_ptr));

    if (!msg_ptr)
    {
      return MSG_BUFFER_NOT_AVAIL;
    }
    msg_ptr->signature = 0xffffu;
    msg_ptr->event_flag = event_flag;
    hdr = (osal_msg_hdr_t *)msg_ptr - 1;

    dst = osal_proxy2alien(task_id);
    hdr->dest_id = TASK_NO_TASK;
    if (ICall_send(src, dst,
                   ICALL_MSG_FORMAT_KEEP, msg_ptr) ==
        ICALL_ERRNO_SUCCESS)
    {
      return SUCCESS;
    }
    osal_msg_deallocate((uint8 *) msg_ptr);
    return FAILURE;
  }

  if ( task_id < tasksCnt )
  {
    halIntState_t   intState;
    HAL_ENTER_CRITICAL_SECTION(intState);    // Hold off interrupts
    tasksEvents[task_id] |= event_flag;  // Stuff the event bit(s)
    HAL_EXIT_CRITICAL_SECTION(intState);     // Release interrupts
#ifdef ICALL_EVENTS
    ICall_signal(osal_syncHandle);
#else /* !ICALL_EVENTS */
    ICall_signal(osal_semaphore);
#endif /* ICALL_EVENTS */
    return ( SUCCESS );
  }
   else
  {
    return ( INVALID_TASK );
  }
}

/*********************************************************************
 * @fn      osal_clear_event
 *
 * @brief
 *
 *    This function is called to clear the event flags for a task. The
 *    event passed in is masked out of the task's event variable.
 *
 * @param   uint8 task_id - receiving tasks ID
 * @param   uint8 event_flag - what event to clear
 *
 * @return  SUCCESS, INVALID_TASK
 */
uint8 osal_clear_event( uint8 task_id, uint32 event_flag )
{
  if ( task_id < tasksCnt )
  {
    halIntState_t   intState;
    HAL_ENTER_CRITICAL_SECTION(intState);    // Hold off interrupts
    tasksEvents[task_id] &= ~(event_flag);   // Clear the event bit(s)
    HAL_EXIT_CRITICAL_SECTION(intState);     // Release interrupts
    return ( SUCCESS );
  }
   else
  {
    return ( INVALID_TASK );
  }
}

static uint8 osal_task_events_alloc( void )
{
  tasksEvents = (uint32 *)osal_mem_alloc( (uint16)(sizeof( uint32 ) * tasksCnt));

  // Verify allocation succeeded
  if ( NULL != tasksEvents )
  {
    ( void )memset( (void *)tasksEvents, 0, (sizeof( uint32 ) * tasksCnt));
  }
  else
  {
    HAL_ASSERT_FORCED();
  }

  return ( NULL != tasksEvents ) ? ( USUCCESS ) : ( UFAILURE );
}
/*********************************************************************
 * @fn      osal_init_system
 *
 * @brief
 *
 *   This function initializes the "task" system by creating the
 *   tasks defined in the task table (osal_tasks.h).
 *
 * @param   void
 *
 * @return  SUCCESS
 */
uint8 osal_init_system( void )
{
  uint8 ret = USUCCESS;

  ret = osal_task_events_alloc();
  if (USUCCESS == ret)
  {
  // Initialize the message queue
  osal_qHead = NULL;

  // Initialize the timers
  osalTimerInit();

  // different power management in Osprey
  // Initialize the Power Management System
  osal_pwrmgr_init();

  /* Prepare memory space for service enrollment */
  osal_prepare_svc_enroll();

  // Initialize the system tasks.
  osalInitTasks();

  /* Reduce ceiling considering potential latency */
  osal_max_msecs -= 2;
  }

  return ( ret );
}

/*********************************************************************
 * @fn      osal_timer_init
 *
 * @brief
 *
 *   This function initialize global OSAL timer variable.
 *
 * @param   tickPeriod: period of a RTOS tick in us
 *          osalMaxMsecs: max possible timer duration
 *
 * @return  none
 */
void osal_timer_init(uint_least32_t tickPeriod, uint_least32_t osalMaxMsecs )
{
  osal_tickperiod = tickPeriod;
  osal_max_msecs = osalMaxMsecs;
}
/*********************************************************************
 * @fn      osal_start_system
 *
 * @brief
 *
 *   This function is the main loop function of the task system.
 *   This Function doesn't return.
 *
 * @param   void
 *
 * @return  none
 */
void osal_start_system( void )
{

  /* The Stack Initialization has completed.
   * Notify the Synchronous Task Create */
  llInitCompleteNotify(SUCCESS);

  /* Kick off timer service in order to allocate resources upfront.
   * The first timeout is required to schedule next OSAL timer event
   * as well. */
  ICall_Errno errno = ICall_setTimer(1, osal_msec_timer_cback,
                                     (void *) osal_msec_timer_seq,
                                     &osal_timerid_msec_timer);
  if (errno != ICALL_ERRNO_SUCCESS)
  {
    ICall_abort();
  }

  for(;;)  // Forever Loop
  {
    osal_run_system();

    ICall_wait(ICALL_TIMEOUT_FOREVER);
  }
}

/*********************************************************************
 * @fn      osal_alien2proxy
 *
 * @brief
 *
 *   Assign or retrieve a proxy OSAL task id for an external ICall entity.
 *
 * @param   origid  ICall entity id
 *
 * @return  proxy OSAL task id
 */
uint8 osal_alien2proxy(ICall_EntityID origid)
{
  size_t i;

  for (i = 0; i < OSAL_MAX_NUM_PROXY_TASKS; i++)
  {
    if (osal_proxy_tasks[i] == OSAL_INVALID_DISPATCH_ID)
    {
      /* proxy not found. Create a new one */
      osal_proxy_tasks[i] = (uint8) origid;
      return (OSAL_PROXY_ID_FLAG | i);
    }
    else if ((ICall_EntityID) osal_proxy_tasks[i] == origid)
    {
      return (OSAL_PROXY_ID_FLAG | i);
    }
  }
  /* abort */
  ICall_abort();
  return TASK_NO_TASK;
}

/*********************************************************************
 * @fn      osal_proxy2alien
 *
 * @brief
 *
 *   Retrieve the ICall entity id for a proxy OSAL task id
 *
 * @param   proxyid  Proxy OSAL task id
 *
 * @return  ICall entity id
 */
static ICall_EntityID osal_proxy2alien(uint8 proxyid)
{
  proxyid ^= OSAL_PROXY_ID_FLAG;
  if (proxyid >= OSAL_MAX_NUM_PROXY_TASKS)
  {
    /* abort */
    ICall_abort();
  }
  return (ICall_EntityID) osal_proxy_tasks[proxyid];
}

/*********************************************************************
 * @fn      osal_dispatch2id
 *
 * @brief
 *
 *   Retrieve OSAL task id mapped to a designated ICall entity id
 *
 * @param   entity  ICall entity id
 *
 * @return  OSAL task id
 */
static uint8 osal_dispatch2id(ICall_EntityID entity)
{
  size_t i;

  for (i = 0; i < tasksCnt; i++)
  {
    if ((ICall_EntityID) osal_dispatch_entities[i] == entity)
    {
      return i;
    }
  }
  return TASK_NO_TASK;
}

/*********************************************************************
 * @fn      osal_msec_timer_cback
 *
 * @brief
 *
 *   This function is a callback function for ICall_setTimer() service
 *   used to implement OSAL timer
 *
 * @param   arg  In this case, the timer sequence number is passed.
 *
 * @return  None
 */
static void osal_msec_timer_cback(void *arg)
{
  unsigned seq = (unsigned) arg;
  halIntState_t intState;

  HAL_ENTER_CRITICAL_SECTION(intState);
  if (seq == osal_msec_timer_seq)
  {
#ifdef ICALL_EVENTS
    ICall_signal(osal_syncHandle);
#else /* !ICALL_EVENTS */
    ICall_signal(osal_semaphore);
#endif /* ICALL_EVENTS */
  }
  HAL_EXIT_CRITICAL_SECTION(intState);
}

/*********************************************************************
 * @fn      osal_service_entry
 *
 * @brief
 *
 *   This function is service function for messaging service
 *
 * @param   args  arguments.
 *
 * @return  ICall error code
 */
ICall_Errno osal_service_entry(ICall_FuncArgsHdr *args)
{
  if (args->func == ICALL_MSG_FUNC_GET_LOCAL_MSG_ENTITY_ID)
  {
    /* Get proxy ID */
    ((ICall_GetLocalMsgEntityIdArgs *)args)->localId =
      osal_alien2proxy(((ICall_GetLocalMsgEntityIdArgs *)args)->entity);
    if (((ICall_GetLocalMsgEntityIdArgs *)args)->localId == TASK_NO_TASK)
    {
      return ICALL_ERRNO_NO_RESOURCE;
    }
  }
  else
  {
    return ICALL_ERRNO_INVALID_FUNCTION;
  }
  return ICALL_ERRNO_SUCCESS;
}

/*********************************************************************
 * @fn      osal_set_icall_hook
 *
 * @brief
 *
 *   This function will set the hook used to parse icall message.
 *
 * @param   param pointer to a function that will be called if a icall message
 *          is found.
 *
 * @return  nones
 */
void osal_set_icall_hook(osal_icallMsg_hook_t param)
{
  osal_icallMsg_hook =  (osal_icallMsg_hook_t) param;
}

/*********************************************************************
 * @fn      osal_run_system
 *
 * @brief
 *
 *   This function will make one pass through the OSAL taskEvents table
 *   and call the task_event_processor() function for the first task that
 *   is found with at least one event pending. If there are no pending
 *   events (all tasks), this function puts the processor into Sleep.
 *
 * @param   void
 *
 * @return  none
 */
void osal_run_system( void )
{
  uint8 idx = 0;

  uint32 next_timeout_prior = osal_next_timeout();

  if (next_timeout_prior)
  {
    osal_timer_refTimeUpdate();
    /* Set a value that will never match osal_next_timeout()
     * return value so that the next time can be scheduled.
     */
    next_timeout_prior = 0xfffffffful;
  }
  if (osal_eventloop_hook)
  {
    osal_eventloop_hook();
  }

  for (;;)
  {
    void *msg;
    ICall_EntityID src, dst;
    osal_msg_hdr_t *hdr;
    uint8 dest_id;

    if (ICall_fetchMsg(&src, &dst, &msg) != ICALL_ERRNO_SUCCESS)
    {
      break;
    }

    hdr = (osal_msg_hdr_t *) msg - 1;

    if (hdr->format == ICALL_MSG_FORMAT_DIRECT_API_ID)
    {
        if (osal_icallMsg_hook)
        {
          (*osal_icallMsg_hook)(msg);
        }
    }
    else
    {
      dest_id = osal_dispatch2id(dst);
      if (dest_id == TASK_NO_TASK)
      {
        /* Something wrong */
        ICall_abort();
      }
      else
      {
        /* Message towards one of the tasks */
        /* Create a proxy task ID if necessary and
         * queue the message to the OSAL internal queue.
         */
        uint8 proxyid = osal_alien2proxy(hdr->srcentity);

        if (hdr->format == ICALL_MSG_FORMAT_1ST_CHAR_TASK_ID)
        {
          uint8 *bytes = msg;
          *bytes = proxyid;
        }
        else if (hdr->format == ICALL_MSG_FORMAT_3RD_CHAR_TASK_ID)
        {
          uint8 *bytes = msg;
          bytes[2] = proxyid;
        }
        /* now queue the message to the OSAL queue */
        osal_msg_send(dest_id, msg);
      }
    }
  }

  do {
    if (tasksEvents[idx] != 0U)  // Task is highest priority that is ready.
    {
      break;
    }
  } while (++idx < tasksCnt);

  if (idx < tasksCnt)
  {
    uint32 events;
    halIntState_t intState;

    HAL_ENTER_CRITICAL_SECTION(intState);
    events = tasksEvents[idx];
    tasksEvents[idx] = 0;  // Clear the Events for this task.
    HAL_EXIT_CRITICAL_SECTION(intState);

    activeTaskID = idx;
    events = (tasksArr[idx])( idx, events );
    activeTaskID = TASK_NO_TASK;

    HAL_ENTER_CRITICAL_SECTION(intState);
    tasksEvents[idx] |= events;  // Add back unprocessed events to the current task.
    HAL_EXIT_CRITICAL_SECTION(intState);
  }

  /* Note that scheduling wakeup at this point instead of
   * scheduling it upon ever OSAL start timer request,
   * would only work if OSAL start timer call is made
   * from OSAL tasks, but not from either ISR or
   * non-OSAL application thread.
   * In case, OSAL start timer is called from non-OSAL
   * task, the scheduling should be part of OSAL_Timers
   * module.
   * Such a change to OSAL_Timers module was not made
   * in order not to diverge the OSAL implementations
   * too drastically between pure OSAL solution vs.
   * OSAL upon service dispatcher (RTOS).
   * TODO: reconsider the above statement.
   */
  {

    uint32 next_timeout_post = osal_next_timeout();
    if (next_timeout_post != next_timeout_prior)
    {
      /* Next wakeup time has to be scheduled */
      if (next_timeout_post == 0)
      {
        /* No timer. Set time to the max */
        next_timeout_post = OSAL_TIMERS_MAX_TIMEOUT;
      }
      if (next_timeout_post > osal_max_msecs)
      {
        next_timeout_post = osal_max_msecs;
      }
      /* Restart timer */
      ICall_stopTimer(osal_timerid_msec_timer);
      ICall_setTimerMSecs(next_timeout_post, osal_msec_timer_cback,
                          (void *) (++osal_msec_timer_seq),
                          &osal_timerid_msec_timer);
    }

#ifdef ICALL_EVENTS
    /*
     * When using events, OSAL needs to be sure that it will not block itself
     * if any message have yet to been processed.  The OSAL event must be
     * signaled when any messages remain unprocessed at the end of this
     * function.
     */
    if (osal_qHead)
    {
      ICall_signal(osal_syncHandle);
    }
    else
    {
      for (uint8 i = 0; i < tasksCnt; i++)
      {
        if (tasksEvents[i] != 0)
        {
          ICall_signal(osal_syncHandle);
          break;
        }
      }
    }
#endif /* ICALL_EVENTS */
  }
}

/*********************************************************************
 * @fn      osal_buffer_uint32
 *
 * @brief
 *
 *   Buffer an uint32 value - LSB first.
 *
 * @param   buf - buffer
 * @param   val - uint32 value
 *
 * @return  pointer to end of destination buffer
 */
uint8* osal_buffer_uint32( uint8 *buf, uint32 val )
{
  *buf++ = BREAK_UINT32( val, 0 );
  *buf++ = BREAK_UINT32( val, 1 );
  *buf++ = BREAK_UINT32( val, 2 );
  *buf++ = BREAK_UINT32( val, 3 );

  return buf;
}

/*********************************************************************
 * @fn      osal_isbufset
 *
 * @brief
 *
 *   Is all of the array elements set to a value?
 *
 * @param   buf - buffer to check
 * @param   val - value to check each array element for
 * @param   len - length to check
 *
 * @return  TRUE if all "val"
 *          FALSE otherwise
 */
uint8 osal_isbufset( uint8 *buf, uint8 val, uint8 len )
{
  uint8 x;

  if ( buf == NULL )
  {
    return ( FALSE );
  }

  for ( x = 0; x < len; x++ )
  {
    // Check for non-initialized value
    if ( buf[x] != val )
    {
      return ( FALSE );
    }
  }
  return ( TRUE );
}

/*********************************************************************
 * @fn      osal_self
 *
 * @brief
 *
 *   This function returns the task ID of the current (active) task.
 *
 * @param   void
 *
 * @return   active task ID or TASK_NO_TASK if no task is active
 */
uint8 osal_self( void )
{
  return ( activeTaskID );
}

/*-------------------------------------------------------------------
 */
