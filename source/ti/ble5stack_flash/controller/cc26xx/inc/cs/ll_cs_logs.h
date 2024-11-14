/******************************************************************************

 @file  ll_cs_logs.h

 @brief Channel Sounding common header

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
#ifndef LL_CS_LOGS_H
#define LL_CS_LOGS_H
/*******************************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "cs/ll_cs_common.h"
#include "cs/ll_cs_rcl.h"
#include "ll_csdrbg.h"

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          llCsLogDrbgInitResults
 *
 * @brief       Log the K and V that are a result of CS DRBG init.
 * This will also log the Security Vectors since they affect the final Result.
 * This log requires the flag LOG_DRBG_RESULTS.
 *
 * input parameters
 *
 * @param       CS_IV - pointer to CS IV vector
 * @param       CS_IN - pointer to CS IN vector
 * @param       CS_PV - pointer to CS PV vector
 * @param       params - pointer of drbg params
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsLogDrbgInitResult(uint8* CS_IV, uint8* CS_IN, uint8* CS_PV,
                           drbgParams_t* params);

/*******************************************************************************
 * @fn          llCsLogDrbgParamsAndResults
 *
 * @brief       This will log the CS procedureCounter, Step Counter,
 * TransactionID and transactionCounter, and the Random Bits that were
 * generated as a result of calling getCSDrbg.
 * This requires the flag LOG_DRBG_RESULTS
 *
 * input parameters
 *
 * @param       prandomBits - pointer to randomBits
 * @param       params - pointer to drbg params
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsLogDrbgParamsAndResult(uint8* prandomBits, drbgParams_t* params);

/*******************************************************************************
 * @fn          llCsLogChannelIndexArray
 *
 * @brief       Logs the shuffled channel index array, requires flag
 *              LOG_CS_CHANNEL_INDEX_ARRAY
 *
 * input parameters
 *
 * @param       pShuffledArray - pointer to shuffled chanIndex array
 * @param       lengthOfArray - length of the array
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsLogChannelIndexArray(uint8* pShuffledArray, uint8 lengthOfarray);

/*******************************************************************************
 * @fn          llCsLogSelectedAccessAddress
 *
 * @brief       Log the selected Access Address
 *              Requires flag LOG_CS_ACCESS_ADDRESS
 *
 * input parameters
 *
 * @param       aaRx - pointer RX access Address
 * @param       aaTx - pointer to TX access Address
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsLogSelectedAccessAddress(uint32_t* aaRx, uint32_t* aaTx);

/*******************************************************************************
 * @fn          llCsLogRandomSequence
 *
 * @brief       Logs the random sequence payload
 *
 * input parameters
 *
 * @param       pTx - pointer to Tx payload
 * @param       pTx - pointer to RX payload
 * @param       payloadLen - length of the payload
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsLogRandomSequence(uint32_t* pTx, uint32_t* pRx, uint8 payloadLen);

/*******************************************************************************
 * @fn          llCsLogStep
 *
 * @brief       Log the step details includes: stepNum, stepmode, chanIdx
 *              Tone Extension Bit, AA Rx/Tx, Payload Rx/Tx
 *
 * input parameters
 *
 * @param       csStep - pointer to CS steps
 * @param       stepNum - number of step
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void llCsLogStep(RCL_CmdBleCs_Step* csStep, uint8 stepNum);

/*******************************************************************************
 * @fn          lCsLogResultStats
 *
 * @brief       Log CS result stats
 *
 * input parameters
 *
 * @param       stats - cs stats
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsLogResultStats(RCL_CmdBleCs_Stats* stats);

/*******************************************************************************
 * @fn          llCsLogStepResults
 *
 * @brief       Log step results
 *
 * input parameters
 *
 * @param       csNumSteps - number of steps to log
 * @param       csRclData - RCL data includes the step results
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsLogStepResults(uint8 csNumSteps, csRclCmdData_t csRclData);
#endif
