/******************************************************************************

 @file  ll_cs_handover.h

 @brief This file contains the data structures and APIs for handling
        Bluetooth Low Energy handover process for Channel Sounding on the
        serving node

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025, Texas Instruments Incorporated

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

#ifndef LL_CS_HANDOVER_H
#define LL_CS_HANDOVER_H

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/controller/ll/ll_handover.h"
#include "ti/ble/controller/ll/ll_cs_common.h"
#include "ti/ble/controller/ll/ll_csdrbg.h"

/*******************************************************************************
 * MACROS
 */

// #define CS_HANDOVER_TEST 1

/*******************************************************************************
 * CONSTANTS
 */
// Full CS Handover Configuration Bitmap - all 4 configuration transmitted
#define LL_CS_CONFIG_ID_0_BITMAP        0x01
#define LL_CS_CONFIG_ID_1_BITMAP        0x02
#define LL_CS_CONFIG_ID_2_BITMAP        0x04
#define LL_CS_CONFIG_ID_3_BITMAP        0x08

#define LL_CS_CONFIG_ID_FULL_BITMAP     (LL_CS_CONFIG_ID_0_BITMAP | \
                                         LL_CS_CONFIG_ID_1_BITMAP | \
                                         LL_CS_CONFIG_ID_2_BITMAP | \
                                         LL_CS_CONFIG_ID_3_BITMAP)

// Handover header size
#define LL_CS_HANDOVER_HEADER_SIZE      LL_HANDOVER_COMPONENT_HEADER_SIZE

// Minimal size of the handover data. Should include the place for the HEADER_SIZE.
// In the future might include other mandatory sizes
#define LL_CS_HANDOVER_MINIMAL_DATA_SIZE LL_CS_HANDOVER_HEADER_SIZE

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/**
 * @brief Structure to hold the run-time allocated parameters for the Handover process
 * on both serving and reflector node sides.
 *
 * @param faeSize Size of the FAE.
 * @param runtimeConfigSizes Array of sizes for the per configuration parameters.
 */
typedef struct
{
  uint8_t faeSize;                      /* Size of the FAE. */
} llCSHandoverRuntimeAllocParamsSizes_t;


/**
 * @brief Structure to hold the parameters for the Handover process on both serving and reflector node sides.
 *
 * This structure contains the parameters required for the Handover process, including DRBG parameters,
 * CS state information, and various sizes for dynamically allocated elements.
 *
 */
typedef struct
{
  drbgParams_t llcsDrbgParams;          /* Parameters for the DRBG. */
  llCs_t       llCs;                    /* CS state information. */
                                        /* For future optimizations:
                                           - remove filteredChanIdx array
                                           - remove procedureEnableData
                                           - remove csTerminateInfo_t */

  uint8_t      validCsConfigsBitmap;       /* Number of valid CS configurations */
  llCSHandoverRuntimeAllocParamsSizes_t csRuntimeAllocInfo;  /* Run-time allocation information for the CS. */
  uint8_t      runtimeAllocParams[];    /* Flexible array member to hold run-time allocated elements. The presence of these elements
                                             is determined by the relevant element sizes described in llCSHandoverRuntimeAllocParamsSizes_t.
                                             The expected data buffer structure is:
                                             | fae
                                            */
} llCSHandoverParams_t;

#endif //LL_CS_HANDOVER_H
