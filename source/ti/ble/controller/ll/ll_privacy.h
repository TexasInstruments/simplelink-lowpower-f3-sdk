/******************************************************************************

 @file  ll_privacy.h

 @brief This file contains the data structures and APIs for handling
        Bluetooth Low Energy Privacy 1.2

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2015-2025, Texas Instruments Incorporated

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
 * UNEXPECTED BEHAVIOR.
 *
 */

#ifndef LL_PRIVACY_H
#define LL_PRIVACY_H

/*******************************************************************************
 * INCLUDES
 */

#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/controller/ll/ll.h"
#include "ti/ble/controller/ll/ll_common.h"
#include "ti/ble/controller/ll/ll_al.h"
#include "ti/ble/controller/ll/ll_dfl.h"

/*******************************************************************************
 * MACROS
 */

#define MASK_ID_ADDRTYPE( addrType ) ((addrType) & LL_DEV_ADDR_TYPE_MASK)

#define ADDRTYPE_TO_OWNADDR( addrType )                                        \
        ( (MASK_ID_ADDRTYPE((addrType)) == LL_DEV_ADDR_TYPE_PUBLIC) ?          \
          (ownPublicAddr)                                           :          \
          (ownRandomAddr) )

/*******************************************************************************
 * CONSTANTS
 */

#define BLE_RESOLVING_LIST_SIZE      (rlSize) // Resolving List Size For Peer + Local IRK/RP/IdAddr
#define LOCAL_RL_INDEX               0
#define EMPTY_RESOLVE_LIST_ENTRY     0xFF
#define INVALID_RESOLVE_LIST_INDEX   0xFFU
#define INVALID_EXT_ACCEPT_LIST_INDEX 0
#define MIN_RPA_TIMEOUT              0x0001 // 1s
#define MAX_RPA_TIMEOUT              0xA1B8 // 41400s or 11.5 hours
#define DEFAULT_RPA_TIMEOUT          0x0384 // 900s or 15 mins

// Masks for MSB of Device Address
#define RANDOM_ADDR_MASK             0xC0
//
#define NON_RESOLVABLE_ADDR_MASK     0x00
#define RESOLVABLE_ADDR_MASK         0x40
#define STATIC_RANDOM_ADDR_MASK      0xC0

// Accept List Size
#define AL_SIZE_STANDARD             0
#define AL_SIZE_EXTENDED             1

// Accept List Ignore For Privacy
#define PRIV_IGNORE_AL_ENTRY         0
#define PRIV_USE_AL_ENTRY            1

#ifdef QUAL_TEST
// List size limited to 3 entries because of test case LL/CON/INI/BV-23-C
#define LOCAL_IRK_LIST_SIZE          3
#endif

// Optional status for privacy policy tests
#define POLICY_TESTS_SUCCESS               0U
#define POLICY_INVALID_PARAMETERS          0xFF

/*******************************************************************************
 * TYPEDEFS
 */

// Optional required tests Flags for privacy policys
union privTestflags_u {
  struct
  {
    uint8 resolveRPA            :   1;  // RPA is resolvable
    uint8 addressIsInAL         :   1;  // Device found in Accept List
    uint8 privacyMode           :   1;  // Whenever the peer address type is public, It required to
                                        // verify the the peer privacy mode is Device Privacy Mode
                                        // or IRK is zero.
    uint8 hostPeerAddrMatch     :   1;  // Verify that the host specified peer
                                        // address matches to the received peer
                                        // address.
    uint8 reserved              :   4;
  };
  uint8 flags;
};

typedef union privTestflags_u privTestflags_t;

// RPA TargetA Entry Configuration
// |   7..2   |    1     |    0    |
// | Reserved | addrType | bEnable |
typedef uint8 rpaTargetAConf_t;

typedef struct
{
  uint8 idAddrType;                  // Identity Address Type (Public or Random Static)
  uint8 idAddr[ B_ADDR_LEN ];        // Identity Address
  uint8 RPA[ B_ADDR_LEN ];           // Resolvable Private Address
  uint8 IRK[ KEYLEN ];               // Identity Resolving Key
  uint8 privMode;                    // Privacy Mode (NPM - 0 , DPM - 1)
} rlEntry_t;

PACKED_TYPEDEF_STRUCT
{
  uint8 offset;                      //Index which indicates the end of the standard AL and the beginning of the ExtAL
  rpaTargetAConf_t conf;             //TargetA RPA configuration
  uint8 address[ LSB_2_BYTES ];      //Least significant 16 bits of the address contained in the entry
  uint8 addressHi[ MSB_4_BYTES ];    //Most significant 32 bits of the address contained in the entry
} rpaTargetAList_t;

PACKED_TYPEDEF_STRUCT
{
  uint8 *pRpaInitA;                  //Pointer to alternative address to send as InitA in a CONNECT_IND or AUX_CONNECT_REQ
  rpaTargetAList_t *pRpaTargetAList; //Pointer to a list of alternative addresses to send as TargetA in an AUX_CONNECT_RSP
} rpaCfg_t;

// Optional configured privacy policy
typedef enum {
    POLICY_AL_UNUSED,                // Accept List not used
    POLICY_AL_USED ,                 // Accept List used
    POLICY_INIT_PEER_ADDR            // Initiator peer address used
} llPrivacyPolicy_t;

#ifdef QUAL_TEST
typedef struct
{
  uint8 peerAddrType;           // Peer Identity Address Type (Public or Random Static)
  uint8 peerAddr[ B_ADDR_LEN ]; // Peer Identity Address
  uint8 IRK[ KEYLEN ];          // Local Identity Resolving Key
} localIrkList_t;
#endif

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

extern privInfo_t privInfo;
extern rlEntry_t  *resolvingList;
extern rpaCfg_t   *pRpaCfg;
/*******************************************************************************
 * GLOBAL ROUTINES
 */

extern void             LL_PRIV_Init( void );

extern uint32           LL_PRIV_Ah( uint8 *irk, uint8 *prand );

extern void             LL_PRIV_GenerateRPA( uint8 *irk, uint8 *rpa );

extern void             LL_PRIV_GenerateNRPA( uint8 *publicAddr, uint8 *nrpa );

extern void             LL_PRIV_GenerateRSA( uint8 *rsa );

extern uint8            LL_PRIV_ResolveRPA( uint8 *rpa, uint8 *irk );

extern uint8            LL_PRIV_IsRPA( uint8 rxAddrType, uint8 *rxAddr );

extern uint8            LL_PRIV_IsNRPA( uint8 rxAddrType, uint8 *rxAddr );

extern uint8            LL_PRIV_IsIDA( uint8 rxAddrType, uint8 *rxAddr );

extern uint8            LL_PRIV_IsResolvable( uint8 *rpa, rlEntry_t *resolvingList );

extern uint8            LL_PRIV_IsZeroIRK( uint8 *irk );

extern uint8            LL_PRIV_FindPeerInRL( rlEntry_t *resolvingList, uint8 idType, uint8 *idAddr );

extern void             LL_PRIV_UpdateRL( rlEntry_t *resolvingList );

extern uint8            LL_PRIV_NumberPeerRLEntries( rlEntry_t *resolvingList );

extern rlEntry_t        *LL_PRIV_GetResolvingList( void );

privTestflags_t         LL_PRIV_ValidatePrivacyCompliance( uint8* const peerAddr, uint8 peeraddrType, uint8 *pRLIndex, privTestflags_t requiredTests );

privTestflags_t         LL_PRIV_SetPrivacyTests(uint8* const peerAddr, uint8 peerAddrType, llPrivacyPolicy_t privacyPolicy);

extern llStatus_t       LL_PRIV_RemoveInvalidPeerId( rlEntry_t *pResolvingListEntry, RCL_FilterList *pDynamicFL, rankDynamicFL_t *prankFLTable );

uint8_t                 LL_PRIV_ValidatePeerAddress(uint8_t *peerAddr,  uint8_t peerAddrType, uint8_t peerRLIndex);

bool                    LL_PRIV_IsIDAddrAllowed( uint8 rlIndex );

#ifdef QUAL_TEST
extern void             LL_PRIV_UpdateLocalIrkList( uint8 *peerAddr, uint8 peerAddrType, uint8 *localIrk );

extern uint8            *LL_PRIV_GetLocalIrk( uint8 *peerAddr, uint8 peerAddrType);

#endif
/*******************************************************************************
 */

#endif /* LL_PRIVACY_H */
