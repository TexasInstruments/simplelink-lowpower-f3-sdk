/******************************************************************************

 Group: WCS, LPC, BTS
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
/******************************************************************************

 @file  hci_api.h

 @brief HCI layer interface APIs

 *****************************************************************************/

#ifndef HCI_API_H_
#define HCI_API_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "comdef.h"
#include <stddef.h>

/*******************************************************************************
 * TYPEDEFS
 */

typedef struct hciControllerToHostCallbacks
{
  int (*send)(uint8 *pHciPkt, uint16 pktLen);
} hciControllerToHostCallbacks_t;

/*******************************************************************************
 * API FUNCTIONS
 */

/*******************************************************************************
 * @fn          HCI_ControllerToHostCallbacksInit
 *
 * @brief       This function initializes callback structure
 *
 * input parameters
 *
 * @param       hciControllerToHostCallbacks_t - A pointer to callback functions structure.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      SUCCESS / FAILURE.
 */
uint32 HCI_ControllerToHostCallbacksInit(hciControllerToHostCallbacks_t *pController2HostCallbacks);

/*******************************************************************************
 * @fn          HCI_ControllerToHostRegisterCb
 *
 * @brief       This function register callback function to HCI events
 *
 * input parameters
 *
 * @param       hciControllerToHostCallbacks_t pCbs - A pointer to callback functions structure.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      SUCCESS / FAILURE.
 */
uint32 HCI_ControllerToHostRegisterCb( const hciControllerToHostCallbacks_t *pCbs );

/********************************************************************************
 * @fn      HCI_HostToControllerSend
 *
 * @brief   Send raw HCI packet to the controller.
 *
 * @param   pHciPkt - A pointer to a raw buffer of HCI command or data packet.
 * @param   pktLen  - The hci packet length
 *
 * @return  0 for success, negative number for error.
 */
int HCI_HostToControllerSend(uint8_t *pHciPkt, uint16_t pktLen);

#ifdef __cplusplus
}
#endif

#endif /* HCI_API_H_ */
