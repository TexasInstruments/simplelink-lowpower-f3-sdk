/******************************************************************************

 @file  ll_csdrbg.h

 @brief This file contains the CS DRBG constants, typedefs and externs.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2023-2024, Texas Instruments Incorporated

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

/*******************************************************************************
 * INCLUDES
 */
#ifndef LL_CSDRBG_H
#define LL_CSDRBG_H
#include "ll_al.h"
#include "ll_common.h"
#include "ll_config.h"
#include <ti/drivers/rcl/RCL.h>
#include <ti/drivers/rcl/commands/ble5.h>

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */
/*******************************************************************************
 * TYPEDEFS
 */
typedef struct drbgParams_t
{
    uint8 kDrbg[16]; // kDrbg initailzed to zero. updated evrey cs procedure
    uint8 vDrbg[16]; // vDrbg initailzed to zero. updated evrey cs procedure
    uint8 csProcedureCounter; // vDrbg initailzed to zero. updated evrey cs
                              // procedure
    uint16 CSStepCounter;     // 16 bits - CS Step Counter = (VDRBG[31:16] +
                              // CSStepCount) mod 2^16
    uint8 TransactionID; //   8 bits - CS TransactionIdentifier = (VDRBG[15:8] +
                         //   CSpTransactionID) mod 2^8
    uint8 TransactionCounter; // 8 bits - (VDRBG[7:0] + CSpTransactionCounter)
                              // mod 2^8
} drbgParams_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * APIs
 */
/****************************************************************************
 * @fn LL_CSDRBG_Init
 *
 * @brief the function initalize the first kDrbg, vDrbg from CS_IV, CS_IN,
 * CS_PV.
 *
 * input parameters
 * @param CS_IV 128 bits result of the CS Security Start procedure.
 *
 * @param CS_IN 64 bits result of the CS Security Start procedure.
 *
 * @param CS_PV 128 bits result of the CS Security Start procedure.
 *
 * @param params - pointer to drbgParams_t. see typedef
 *
 * output parameters
 * @param params - params -> kDrbg, params -> vDrbg are initialized.
 *
 * @return LL_STATUS_ERROR_BAD_PARAMETER if one of the pointers is null or
 * LL_STATUS_SUCCESS.
 */
llStatus_t LL_CSDRBG_Init(uint8* CS_IV, uint8* CS_IN, uint8* CS_PV,
                          drbgParams_t* params);

/****************************************************************************
 * @fn LL_CSDRBG_GetDrbg
 *
 * @brief the function create a new DRBG
 *
 *  input parameters
 *
 * @param prandomBits - pointer to the 128 bits block.
 *
 * @param params - pointer to drbgParams_t. see typedef
 *
 * output parameters:
 *
 * @param prandomBits 128 bits of random numbers
 *
 * @return LL_STATUS_ERROR_BAD_PARAMETER if one of the pointers is null or
 * LL_STATUS_SUCCESS.
 */
llStatus_t LL_CSDRBG_GetDrbg(uint8* prandomBits, drbgParams_t* params);

/****************************************************************************
 * @fn LL_CSDRBG_BackTracking
 *
 * @brief Backtracking resistance shall be invoked every time the CSProcCount is
 * incremented. the backtracting uses f9() to update kDrbg, vDrbg by taking SM =
 * {0}, kDrbg and vDrbg[31:16] + CSStepCount + 1. (CSStepCount from last proc.)
 * and cs proc. counter increses by 1.
 *
 * input parameters
 * @param params - pointer to drbgParams_t. see typedef
 *
 * output parameters
 * @param params - params -> kDrbg, params -> vDrbg are updated. cs proc.
 * counter + 1.
 *
 * @return
 */
llStatus_t LL_CSDRBG_BackTracking(drbgParams_t* params);

/****************************************************************************
 * @fn csDrbgSelfTest
 *
 * @brief known answer tset - tests the CS deterministic random bit generator
 * with sample data from the spec.
 *
 * @return TRUE if all the test for several K, V to generate randombits and
 * several backtaring test all passed. FALSE otherwise.
 */
uint8 csDrbgSelfTest();

#endif