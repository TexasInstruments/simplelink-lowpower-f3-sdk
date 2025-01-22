/******************************************************************************

@file  bleapputil_extctrl_dispathcer.h

@brief This file processes and send the data from the UART to the Applications
       and vice versa.


Group: WCS, BTS
Target Device: cc23xx

******************************************************************************

 Copyright (c) 2024, Texas Instruments Incorporated
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
 *  @defgroup bleapputil_extctrl_dispatcher bleapputil_extctrl_dispatcher
 *  @brief This module implements external control Dispatcher functions
 *  @{
 *  @file  bleapputil_extctrl_dispatcher.h
 *  @brief This file processes and send the data from the UART to the Application
 *  and vice versa.
 */


#ifndef BLEAPPUTIL_EXTCTRL_HOST_DISPATCHER_H
#define BLEAPPUTIL_EXTCTRL_HOST_DISPATCHER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <bcomdef.h>

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */
#define CHIP_ID_SIZE                               6      //!< Chip Identifier Size
#define DISPATCHER_MSG_HEADER                      4      //!< The Host Message Header

#define EXTCTRL_HOST_CMD_IDENTIFY                  0x00   //!< EXTCTRL HOST command for identification
#define EXTCTRL_HOST_CMD                           0x90   //!< EXTCTRL HOST command wrapper

#define EXTCTRL_HOST_EVT_ERROR                     0x81    //!< EXTCTRL HOST async event

/*********************************************************************
 * TYPEDEFS
 */
typedef void (*MsgHandler_t)(uint8_t *pData);


// The external control host event handler prototype @ref Dispatcher_processEvents function
typedef bStatus_t (*ExtCtrlHost_eventHandler_t)(uint8_t *, uint16_t );
/*********************************************************************
 * Enumerators
 */

/// @brief Enumeration for external control Status
typedef enum
{
  EXTCTRL_HOST_SUCCESS,
  EXTCTRL_HOST_FAIL,
  EXTCTRL_HOST_LINK_ESTAB_FAIL,
  EXTCTRL_HOST_LINK_TERMINATED,
  EXTCTRL_HOST_OUT_OF_MEMORY,
  EXTCTRL_HOST_CONFIG_NOT_SUPPORTED,
  EXTCTRL_HOST_ILLEGAL_CMD
} DispatcherStatus_e;

/*********************************************************************
 * Structures
 */

/**
 *  @defgroup bleapputil_extctrl_dispatcher_Structures BLEAppUtil External Control
 *  Dispatcher Structures
 *  @brief This module implements BLEAppUtil External Control Dispatcher Structures
 *  @{
 */

// External Control capabilities @ref EXTCTRL_HOST_IDENTIFY
typedef struct
{
  uint32_t capab;                        // Capabilities
  uint16_t revNum;                       // Revision
  uint8_t  devId;                        // Device ID
  uint8_t  identifier[CHIP_ID_SIZE];     // Unique identifier
  uint8_t  maxNumConns;                  // Maximum number of supported connections
} DispatcherCapabilities_t;


/** @} End bleapputil_extctrl_dispatcher_Structures */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/**
 *  @defgroup bleapputil_extctrl_dispatcher_Functions BLEAppUtil External Control
 *  Dispatcher Functions
 *  @brief This module implements BLEAppUtil External Control Dispatcher functions
 *  @{
 */

/*********************************************************************
 * @fn      Dispatcher_registerMsgHandler
 *
 * @brief   This function called by the applications to register their message handler.
 *          and store the registered application into the global array,
 *          and append their capabilities.
 *
 * @param   appSpecifier - The app specifier to be registered to this module
 * @param   msgHandler  - pointer that contains the message handler and the appSpecifier.
 * @param   appCapab     - the capabilities that the application support.
 *
 * @return  The function that processes the events in this module if success; otherwise
 *          return NULL
 */
ExtCtrlHost_eventHandler_t Dispatcher_registerMsgHandler(uint8_t appSpecifier, MsgHandler_t msgHandler, uint32_t appCapab);

/** @} End bleapputil_extctrl_dispatcher_Functions */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

#ifdef __cplusplus
}
#endif

#endif /* BLEAPPUTIL_EXTCTRL_HOST_DISPATCHER_H */

/** @} End bleapputil_extctrl_dispatcher */
