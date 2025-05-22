/******************************************************************************

 @file  ll_tx_power.h

 @brief This file contains the Link Layer (LL) tx power APIs

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
#ifndef LL_TX_POWER_H
#define LL_TX_POWER_H
/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/stack_util/comdef.h"
#include "ti/ble/controller/ll/ll_config.h"
#include <ti/drivers/rcl/RCL.h>
#include <ti/drivers/rcl/commands/ble5.h>

/*******************************************************************************
 * CONSTANTS
 */

// Typedef for TX power to be used in the stack that is aligned with the RCL defined
typedef RCL_Command_TxPower llTxPowerRfCmd_t;

// Default TX power fraction value - FALSE mean not use

#define TX_POWER_DEFAULT_FRACTION UFALSE

/*******************************************************************************
 * MACROS
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

/* Link Layer inner functions for stack usage */

/*******************************************************************************
 * @fn          LL_TxPower_SetTxPowerCmd
 *
 * @brief       Sets the tx power value into the BLE RF command
 *
 * input parameters
 *
 * @param       pRfCmd   - Rf command type
 * @param       txPower  - Tx power entry
 * @param       fraction - If set to TRUE, raises the requested power
 *                         level by 0.5 dB
 *
 * output parameters
 *
 * @param       pRfCmd   - Fill in the tx power rf type in the command
 *
 * @return      None
 */
void LL_TxPower_SetTxPowerCmd(uint32_t *pRfCmd, int8_t txPower, bool fraction);

/*******************************************************************************
 * @fn          LL_TxPower_SetTxPowerRfType
 *
 * @brief       Set the tx power type by the tx power dBm and fraction value
 *
 * input parameters
 *
 * @param       txPowerDbm - Tx power dBm value
 * @param       fraction   - If set to TRUE, raises the requested power
 *                           level by 0.5 dB
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Tx power type in llTxPowerRfCmd_t type
 */
llTxPowerRfCmd_t LL_TxPower_SetTxPowerRfType( int8_t txPowerDbm,
                                              bool fraction );

/*******************************************************************************
 * @fn          LL_TxPower_SetTxPowerCurrentRfType
 *
 * @brief       This routine is used to save the TX Power value provided by user
 *              to the global gCurTxPowerVal
 *
 * input parameters
 *
 * @param       txPower - Tx Power
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void LL_TxPower_SetTxPowerCurrentRfType(llTxPowerRfCmd_t txPower);

/*******************************************************************************
 * @fn          LL_TxPower_GetTxPowerMinDbm
 *
 * @brief       Get the minimum tx power entry
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Tx power minimum dBm value
 */
int8_t LL_TxPower_GetTxPowerMinDbm(void);

/*******************************************************************************
 * @fn          LL_TxPower_GetTxPowerMaxDbm
 *
 * @brief       Get the maximum tx power entry
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Tx power maximum dBm value
 */
int8_t LL_TxPower_GetTxPowerMaxDbm(void);

/*******************************************************************************
 * @fn          LL_TxPower_GetTxPowerCurrentFields
 *
 * @brief       This function is used to get the current global tx power dBm and
 *              fraction fields
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       pTxPowerDbm - pointer to fill in the tx power dBm field
 * @param       pFraction   - pointer to fill the fraction field
 *
 * @return      None
 */
void LL_TxPower_GetTxPowerCurrentFields(int8_t *pTxPowerDbm, bool *pFraction);

/*******************************************************************************
 * @fn          LL_TxPower_GetTxPowerCurrentRfType
 *
 * @brief       This function is used to get the current global tx power in
 *              llTxPowerRfCmd_t type
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Tx Power Value in llTxPowerRfCmd_t type
 */
llTxPowerRfCmd_t LL_TxPower_GetTxPowerCurrentRfType(void);

/*******************************************************************************
 * @fn          LL_TxPower_TxPowerIsValid
 *
 * @brief       Validate th tx power is valid to use
 *
 * input parameters
 *
 * @param       txPower - Tx power to validate
 *
 * output parameters
 *
 * @param       None
 *
 * @return      FALSE, TRUE
 */
bool LL_TxPower_TxPowerIsValid(llTxPowerRfCmd_t txPower);

/*******************************************************************************
 * @fn          LL_TxPower_ResetTxPowerToDefault
 *
 * @brief       Set the current tx power level to the default value define by
 *              the Host/App
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      None
 */
void LL_TxPower_ResetTxPowerToDefault(void);

#endif // LL_TX_POWER_H
