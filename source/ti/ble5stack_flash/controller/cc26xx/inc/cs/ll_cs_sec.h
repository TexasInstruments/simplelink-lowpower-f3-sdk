/******************************************************************************

 @file  ll_cs_sec.h

 @brief Generates the LL CS Vectors.
        Triggers the CS DRBG to generate random bits for the different
        DRBG transactions.

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
#include "bcomdef.h"
#include "ll_cs_common.h"
/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */

/*******************************************************************************
 * @fn          llCsSecGenerateIV
 *
 * @brief       This function is used to generate a device's half of the CS_IV.
 * Random numbers generated using the requirements for random
 * number generation defined in specification[Vol 2] Part H, Section 2
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       csIV - pointer to Initialization vector
 *
 * output parameters
 *
 * @param       csIV.
 *
 * @return      Status
 */
uint8 llCsSecGenerateIV(uint8* csIV);

/*******************************************************************************
 * @fn          llCsSecGenerateIN
 *
 * @brief       This function is used to generate a device's half of the CS_IN.
 * Random numbers generated using the requirements for random
 * number generation defined in specification[Vol 2] Part H, Section 2
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       csIN - pointer to Instantiation vector
 *
 * output parameters
 *
 * @param       csIN.
 *
 * @return      Status
 */
uint8 llCsSecGenerateIN(uint8* csIN);

/*******************************************************************************
 * @fn          llCsSecGeneratePV
 *
 * @brief       This function is used to generate a device's half of the CS_PV.
 * Random numbers generated using the requirements for random
 * number generation defined in specification[Vol 2] Part H, Section 2
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       csPV - pointer to personalization vector

 *
 * output parameters
 *
 * @param       csPV.
 *
 * @return      None
 */
uint8 llCsSecGeneratePV(uint8* csPV);

/*******************************************************************************
 * @fn          llCsSecGenRandomBits
 *
 * @brief       Generates Random bits using the CS DRBG
 * based on the transactionId this function shall set the generated random
 * bits in the CS DB.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       transactionId  - CS Transaction ID.
 * @param       rndBits - random bits pointer
 *
 * output parameters
 *
 * @param       rndBits.
 *
 * @return      None
 */
uint8 llCsSecGenRandomBits(uint8 transactionId, uint8* rndBits);

/*******************************************************************************
 * @fn          llCsProcedureInitDrbg
 *
 * @brief       Initialize the CS DRBG
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       connId   - connId
 * @param       configId - configId
 *
 * output parameters
 *
 * @param       rndBits.
 *
 * @return      status
 */
csStatus_e llCsProcedureInitDrbg(uint16 connId, uint8 configId);

/*******************************************************************************
 * @fn          llCsSecIncreaseStepCount
 *
 * @brief       Increase the step counter
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       rndBits.
 *
 * @return      None
 */
void llCsSecIncreaseStepCount(void);

/*******************************************************************************
 * @fn          llCsSecGetStepCount
 *
 * @brief       Get DRBG param step count
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      DRBG num steps
 */
uint16 llCsSecGetStepCount(void);

/*******************************************************************************
 * @fn          llCsSecGetProcedureCount
 *
 * @brief       Get DRBG param procedure count
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      DRBG num procedures
 */
uint16 llCsSecGetProcedureCount(void);

/*******************************************************************************
 * @fn          llCsSecSetProcedureCount
 *
 * @brief       Set DRBG param procedure count
 * This API shall be used only in case of CS Procedure Termination when CS
 * Procedure Count in the CS_TERMINATE_RSP is larger than that of the
 * CS_TERMINATE_REQ. In these case we must use the larger value to keep the
 * Link Layers sychronized.
 *
 * input parameters
 *
 * @param       procCnt - Procedure Count
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSecSetProcedureCount(uint16 procCnt);

/*******************************************************************************
 * @fn          llCsSecResetStepCount
 *
 * @brief       Reset step counter
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSecResetStepCount(void);

/*******************************************************************************
 * @fn          llCsSecIncProcCounter
 *
 * @brief       Increment CS procedure counter
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None
 */
void llCsSecIncProcCounter(void);

/*******************************************************************************
 * @fn          hr1
 *
 * @brief       Channel Sounding random number generation function.
 * The CS random number generation function hr1 as defined below is
 * used to generate random numbers within an arbitrary range.
 *
 * @design      BLE_LOKI-506
 * input parameters
 *
 * @param       r - arbitrary range 0 to R-1 from which a random number is to be
 *              generated
 * @param       tId - transactionID
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      Rout - random number
 */
uint8 hr1(uint8 r, csTransactionId_e tId);

/*******************************************************************************
 * @fn          csDrbg
 *
 * @brief       Random Bit Generation Function
 * Returns random bits depending on the number of required bits
 * from the DRBG outout vector of the specific transaction ID.
 * In CS SPEC see Random bit generation function CS_DRBG
 * And Channel Sounding random bit generation.
 *              CS_DRBG( number of required bits ) ⊆ randomBits[0:127]
 *
 * @design      BLE_LOKI-506
 * input parameters
 *
 * @param       randomBitsRequired - num of required bits
 * @param       pRndBits - random bits result
 * @param       transactionId - the CS transaction Id
 *
 * output parameters
 *
 * @param       pRndBits - pointer to random bits
 *
 * @return      None
 */
void csDrbg(uint8 randomBitsRequired, uint8* pRndBits, csTransactionId_e transactionId);

/*******************************************************************************
 * @fn          cr1
 *
 * @brief       Shuffling function cr1
 * When deriving the channel index selection within a CS procedure,
 * it is needed to shuffle the list of available channels.
 * The channel index shuffling function cr1 is defined for this
 * purpose.
 *
 * @design      BLE_LOKI-506
 *
 * input parameters
 *
 * @param       pChannelArray - output array
 * @param       filterdArr    - input array
 * @param       nChannels     - size of array (number of channels)
 * @param       trId          - transaction Id
 *
 * output parameters
 *
 * @param       pChannelArray - result of shuffling filteredArr
 *
 * @return      None.
 */
void cr1(uint8* pChannelArray, uint8* filterdArr, uint8 nChannels,
         csTransactionId_e trId);

/*******************************************************************************
 * @fn          llCsSecIsTestMode
 *
 * @brief       Check if Test Mode is enabled and overrdie the DRBG Nonce
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      TRUE - if CS Test mode is ON
 *              FALSE - otherwise
 */
uint8 llCsSecIsTestMode(void);