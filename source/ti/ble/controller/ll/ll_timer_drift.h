/******************************************************************************

 @file  ll_timer_drift.h

 @brief This file contains the Link Layer (LL) types, constants,
        API's etc. for the Bluetooth Low Energy (ULE) Controller
        Timer Drift routines.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2009-2025, Texas Instruments Incorporated

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

#ifndef LL_TIMER_DRIFT_H
#define LL_TIMER_DRIFT_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/controller/ll/ll_ble.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

#define LL_SYNTH_CALIBRATION                (RAT_TICKS_IN_256US)
#define LL_RX_SETTLE_TIME                   (RAT_TICKS_IN_64US)
#define LL_RX_RAMP_OVERHEAD                 (LL_SYNTH_CALIBRATION + LL_RX_SETTLE_TIME)
#define LL_JITTER_CORRECTION                (RAT_TICKS_IN_16US)
#define LL_RX_SYNCH_OVERHEAD                (RAT_TICKS_IN_140US)
#define LL_RX_SYNCH_OVERHEAD_CODED          (RAT_TICKS_IN_900US)

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * API
 */

extern uint16 llCalcScaFactor( uint8 centralSCA );
extern uint32 llCalcPeriodicScaDriftPerInterval( uint8 perAdvSCA , uint16 periodicInterval);
uint32_t llScaOrdValueToPPM( uint8_t ordSCA );
uint32_t llCsCalcTimerDrift(uint32_t offset, uint32_t subeventInterval, uint16_t scaFactor);

#ifdef __cplusplus
}
#endif

#endif /* LL_TIMER_DRIFT_H */
