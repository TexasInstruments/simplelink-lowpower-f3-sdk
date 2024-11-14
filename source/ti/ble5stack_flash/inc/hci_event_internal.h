/******************************************************************************

 @file  hci_event_internal.h

 @brief This file contains the HCI Event internal h file

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2024, Texas Instruments Incorporated

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

#ifndef HCI_EVENT_INTERNAL_H
#define HCI_EVENT_INTERNAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */


/*******************************************************************************
 * MACROS
 */
#define LL_PHY_UPDATE_TYPE_CODED 3
// Note 7.7.65.12 - LE Phy Update Complete Event - LE Coded PHY type defined in TX_PHY and RX_PHY should be 0x03
#define LL_ConvertPhy(phy) ((phy == LL_PHY_CODED)? LL_PHY_UPDATE_TYPE_CODED : phy)
/*******************************************************************************
 * CONSTANTS
 */
#define HCI_EVENT_DATA_OFFSET    3
#define HCI_LE_EVENT_CODE_INDEX  3
#define HCI_EVENT_LE_DATA_OFFSET 4
// CTE report event samples indexing
#define HCI_CTE_SAMPLES_COUNT_REF_PERIOD         (8)    //number of samples in referece period according to spec
#define HCI_CTE_FIRST_SAMPLE_IDX_REF_PERIOD      (1)    //reference period start samples index for sample rate 1Mhz,2Mhz and 3Mhz
#define HCI_CTE_FIRST_SAMPLE_IDX_REF_PERIOD_4MHZ (0)    //reference period start samples index for sample rate 4Mhz
#define HCI_CTE_FIRST_SAMPLE_IDX_SLOT_1US        (37)   //1us start samples index for sample rate 1Mhz,2Mhz and 3Mhz
#define HCI_CTE_FIRST_SAMPLE_IDX_SLOT_2US        (45)   //2us start samples index for sample rate 1Mhz,2Mhz and 3Mhz
#define HCI_CTE_FIRST_SAMPLE_IDX_SLOT_1US_4MHZ   (36)   //1us start samples index for sample rate 4Mhz
#define HCI_CTE_FIRST_SAMPLE_IDX_SLOT_2US_4MHZ   (44)   //2us start samples index for sample rate 4Mhz

// CTE report event samples offsets
#define HCI_CTE_SAMPLE_JUMP_REF_PERIOD           (4)    //peek 1 sample every 4 samples
#define HCI_CTE_SAMPLE_JUMP_SLOT_1US             (8)    //peek 1 sample every 8 samples
#define HCI_CTE_SAMPLE_JUMP_SLOT_2US             (16)   //peek 1 sample every 16 samples

#define HCI_CTE_MAX_SAMPLES_PER_EVENT            (96)   //max samples data length in one event
#define HCI_CTE_MAX_RF_BUFFER_SIZE               (512)  //first buffer size (MCE RAM)
#define HCI_CTE_MAX_RF_EXT_BUFFER_SIZE           (512)  //second buffer size (RFE RAM)
#define HCI_CTE_SAMPLE_RATE_4MHZ                 (4)
#define HCI_CTE_SAMPLE_RATE_1MHZ                 (1)

#define HCI_PERIODIC_ADV_REPORT_MAX_DATA         (0xFF - HCI_PERIODIC_ADV_REPORT_EVENT_LEN)
#define HCI_PERIODIC_ADV_REPORT_DATA_INCOMPLETE  (1)
/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */



#ifdef __cplusplus
}
#endif

#endif /* HCI_EVENT_INTERNAL_H */
