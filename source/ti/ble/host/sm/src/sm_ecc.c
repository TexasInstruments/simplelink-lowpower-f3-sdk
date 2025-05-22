/******************************************************************************

 @file  sm_ecc.c

 @brief This file contains the SM ECC Event for P-256 and Diffie-Hellman keys
        When a network processor is used.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2011-2025, Texas Instruments Incorporated

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

#include "ti/ble/stack_util/bcomdef.h"
#include "ti/ble/stack_util/osal/osal.h"

#include "ti/ble/controller/hci/hci.h"
#include "ti/ble/host/sm/sm.h"
#include "ti/ble/stack_util/lib_opt/map_direct.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Application Task ID.  Used to send ECC Key events when an network processor
// application task is registered to do so.
extern uint8 SM_appTaskID;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION PROTOTYPES
 */

/*********************************************************************
 * CALLBACKS
 */

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      SM_p256KeyCB
 *
 * @brief   Read P-256 Key Event Callback from the Controller.  The secret key
 *          used to generate the public key pair is also input here.
 *
 * @params  pK         - pointer to HCI BLE Read P256 Public Key Complete struct
 *                       which contain the X and Y Coordinates of the ECC Public
 *                       Keys.
 *
 * @param   privateKey - the private key used to generated the ECC Public Keys.
 *
 * @return  None
 */
void SM_p256KeyCB( hciEvt_BLEReadP256PublicKeyComplete_t *pK, uint8 *privateKey )
{
  smEccKeysEvt_t *eccKeys;

  eccKeys = (smEccKeysEvt_t *)MAP_osal_msg_allocate( sizeof (smEccKeysEvt_t) );

  if ( eccKeys )
  {
    eccKeys->hdr.event = SM_MSG_EVENT;
    eccKeys->hdr.status = pK->status;
    eccKeys->opcode = SM_ECC_KEYS_EVENT;

    // If key generation was successful
    if (pK->status == SUCCESS)
    {
      MAP_osal_memcpy( eccKeys->privateKey, privateKey, SM_ECC_KEY_LEN );
      MAP_osal_memcpy( eccKeys->publicKeyX, pK->p256Key, SM_ECC_KEY_LEN );
      MAP_osal_memcpy( eccKeys->publicKeyY, &pK->p256Key[SM_ECC_KEY_LEN],
                       SM_ECC_KEY_LEN );
    }

    // Send to app.
    MAP_osal_msg_send( SM_appTaskID, (uint8 *)eccKeys );
  }
}

/*********************************************************************
 * @fn      SM_dhKeyCB
 *
 * @brief   BLE Generate Diffie-Hellman Key Event Callback from the controller.
 *
 * @params           - pointer to HCI BLE Read P256 Public Key Complete struct
 *                       which contain the X and Y Coordinates of the ECC Public
 *                       Keys.
 *
 * @param   pDhKey - the Diffie-Hellman X and Y Coordinates output from the
 *                   ECDH operation.
 *
 * @return  None
 */
void SM_dhKeyCB( hciEvt_BLEGenDHKeyComplete_t *pDhKey )
{
  smDhKeyEvt_t *dhKey;

  dhKey = (smDhKeyEvt_t *)MAP_osal_msg_allocate( sizeof (smDhKeyEvt_t) );

  if ( dhKey )
  {
    dhKey->hdr.event = SM_MSG_EVENT;
    dhKey->hdr.status = pDhKey->status;
    dhKey->opcode = SM_DH_KEY_EVENT;

    if ( pDhKey->status == SUCCESS )
    {
      MAP_osal_memcpy( dhKey->dhKey,pDhKey->dhKey, SM_ECC_KEY_LEN );
    }
    MAP_osal_msg_send( SM_appTaskID, (uint8 *)dhKey );
  }
}


/*********************************************************************
*********************************************************************/
