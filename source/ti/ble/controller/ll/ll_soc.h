/******************************************************************************

 @file  ll_soc.h

 @brief SoC-specific tuning parameters for the BLE Link Layer.

        The defines in this file are intentionally named with the
        SOC_ prefix to signal that their values differ across
        device families and must not be treated as fixed constants.
        Any addition here should be accompanied by a comment explaining
        which hardware characteristic drives the per-SoC difference.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2026, Texas Instruments Incorporated

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
#ifndef LL_SOC_H
#define LL_SOC_H

#include <ti/devices/DeviceFamily.h>
#include "ti/ble/controller/ll/ll_ble.h"

/*******************************************************************************
 * SOC-SPECIFIC CONSTANTS
 *
 * CC27XX  - Hardware DRBG and faster CPU allow a tighter offset.
 * Default - All other platforms (CC23X0, ...) use software DRBG or have a
 *           slower CPU, requiring a larger lead time before the first CS subevent.
 */

/* Minimum CS event offset (in units of 0.625 ms) from the ACL anchor point
 * to the start of the first CS subevent. The value must satisfy the spec
 * minimum (500 us) and must be large enough for the SoC's processing
 * pipeline (DRBG generation, step buffer fill, RCL command setup). */
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #define SOC_CS_OWN_OFFSET_MIN    1500
#else
    #define SOC_CS_OWN_OFFSET_MIN    3000
#endif

/* Lead time (in RAT ticks) added to the current time when scheduling the
 * first CS test subevent anchor point. Must cover the full SW path from
 * test-command reception to RCL submission, including DRBG step generation.
 * CC27XX: 500 us is sufficient for hardware DRBG platforms.
 * Default: software DRBG and slower CPU require 1.5 ms. */
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_CC27XX)
    #define SOC_CS_TEST_ANCHOR_LEAD_TIME    RAT_TICKS_IN_500US
#else
    #define SOC_CS_TEST_ANCHOR_LEAD_TIME    RAT_TICKS_IN_1_5MS
#endif

#endif /* LL_SOC_H */
