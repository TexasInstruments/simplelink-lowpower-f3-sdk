/******************************************************************************

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025, Texas Instruments Incorporated
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
 *  @file  opt_dependencies.h
 *  @brief      Configuration dependencies for BLE stack.
 */

#ifndef OPT_DEPENDENCIES_H
#define OPT_DEPENDENCIES_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */


/*********************************************************************
 * CONSTANTS
 */

/// @cond NODOC

/********************************************************************
 * EXTERNAL CONTROLLER DEFINES DEPENDENCIES:
 *  These are the defines that are required by the stack and must be defined.
 */
// Set the Controller Configuration
#if defined ( HOST_CONFIG )

  #if ( HOST_CONFIG & CENTRAL_CFG )
    #ifndef CTRL_SCANNER_CFG
        #define CTRL_SCANNER_CFG
    #endif // CTRL_SCANNER_CFG
    #ifndef CTRL_INITIATOR_CFG
        #define CTRL_INITIATOR_CFG
    #endif // CTRL_INITIATOR_CFG
    #ifndef CTRL_CONNECTABLE_CFG
        #define CTRL_CONNECTABLE_CFG
    #endif // CTRL_CONNECTABLE_CFG
  #endif // HOST_CONFIG & CENTRAL_CFG

  #if ( HOST_CONFIG & PERIPHERAL_CFG )
    #ifndef CTRL_ADV_NCONN_CFG
        #define CTRL_ADV_NCONN_CFG
    #endif // CTRL_ADV_NCONN_CFG
    #ifndef CTRL_ADV_CONN_CFG
        #define CTRL_ADV_CONN_CFG
    #endif // CTRL_ADV_CONN_CFG
    #ifndef CTRL_CONNECTABLE_CFG
        #define CTRL_CONNECTABLE_CFG
    #endif // CTRL_CONNECTABLE_CFG
  #endif // HOST_CONFIG & PERIPHERAL_CFG

  #if ( HOST_CONFIG & OBSERVER_CFG )
    #ifndef CTRL_SCANNER_CFG
        #define CTRL_SCANNER_CFG
    #endif // CTRL_SCANNER_CFG
  #endif // HOST_CONFIG & OBSERVER_CFG

  #if ( HOST_CONFIG & BROADCASTER_CFG )
    #ifndef CTRL_ADV_NCONN_CFG
        #define CTRL_ADV_NCONN_CFG
    #endif // CTRL_ADV_NCONN_CFG
  #endif // HOST_CONFIG & BROADCASTER_CFG

  #if ( !(HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG | OBSERVER_CFG | BROADCASTER_CFG)) )
    #error "Build Configuration Error: Invalid Host Role!"
  #endif // !(HOST_CONFIG & (CENTRAL_CFG | PERIPHERAL_CFG | OBSERVER_CFG | BROADCASTER_CFG))

#endif  // HOST_CONFIG


/********************************************************************
 * INTERNAL CONTROLLER DEFINES DEPENDENCIES:
 *  These are the defines that are required by the stack and must be defined.
 */

// Temporary set controller defines from CTRL_CONFIG:
#if defined(CTRL_CONFIG) && (CTRL_CONFIG & INIT_CFG)
    // scanner initiator
    #define CTRL_INITIATOR_CFG
#endif // CTRL_CONFIG & INIT_CFG

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & SCAN_CFG)
    // scanner
    #define CTRL_SCANNER_CFG
#endif // CTRL_CONFIG & SCAN_CFG

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & ADV_CONN_CFG)
    // adv connectable
    #define CTRL_ADV_CONN_CFG
#endif // CTRL_CONFIG & ADV_CONN_CFG

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & ADV_NCONN_CFG)
    // adv non connectable
    #define CTRL_ADV_NCONN_CFG
#endif // CTRL_CONFIG & ADV_NCONN_CFG

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & (ADV_CONN_CFG | INIT_CFG))
    // connectable
    #define CTRL_CONNECTABLE_CFG
#endif // CTRL_CONFIG & (ADV_CONN_CFG | INIT_CFG)


// Roles defines dependencies:
#if (defined(CTRL_INITIATOR_CFG)) && (!defined(CTRL_SCANNER_CFG))
    #define CTRL_SCANNER_CFG
    #warning "CTRL_INITIATOR_CFG is defined without CTRL_SCANNER_CFG. Defining CTRL_SCANNER_CFG automatically."
#endif // CTRL_INITIATOR_CFG && !CTRL_SCANNER_CFG

#if (defined(CTRL_ADV_CONN_CFG)) && (!defined(CTRL_ADV_NCONN_CFG))
    #define CTRL_ADV_NCONN_CFG
    #warning "CTRL_ADV_CONN_CFG is defined without CTRL_ADV_NCONN_CFG. Defining CTRL_ADV_NCONN_CFG automatically."
#endif // CTRL_ADV_CONN_CFG && !CTRL_ADV_NCONN_CFG

#if (!defined(CTRL_CONNECTABLE_CFG)) && (!defined(CTRL_ADV_CONN_CFG)) && (!defined(CTRL_ADV_NCONN_CFG)) && (!defined(CTRL_SCANNER_CFG)) && (!defined(CTRL_INITIATOR_CFG))
    #error "Build Configuration Error: Invalid Controller Role!"
#endif // !CTRL_CONNECTABLE_CFG && !CTRL_ADV_CONN_CFG && !CTRL_ADV_NCONN_CFG && !CTRL_SCANNER_CFG


/********************************************************************
 * INTERNAL HOST DEFINES DEPENDENCIES:
 *  These are the defines that are required by the stack and must be defined.
 */

#if (defined(GAP_BOND_MGR) && defined(NO_OSAL_SNV))
    #error "Bond Manager cannot be used since NO_OSAL_SNV used! Disable in buildConfig.opt"
#endif // GAP_BOND_MGR && NO_OSAL_SNV

#if (defined(GAP_BOND_MGR) && defined(HOST_CONFIG) && !(HOST_CONFIG & (PERIPHERAL_CFG | CENTRAL_CFG)))
    #error "Bond Manager cannot be used since no Peripheral or Central role is defined! Disable in buildConfig.opt"
#endif // GAP_BOND_MGR && HOST_CONFIG && !(HOST_CONFIG & (PERIPHERAL_CFG | CENTRAL_CFG))

#ifdef __cplusplus
}
#endif

#endif /* OPT_DEPENDENCIES_H */
