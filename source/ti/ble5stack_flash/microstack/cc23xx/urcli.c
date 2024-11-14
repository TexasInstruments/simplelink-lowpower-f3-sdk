/******************************************************************************

 @file  urcli.c

 @brief This file contains the RCL driver interfacing API for the Micro
        BLE Stack.

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

/*********************************************************************
 * INCLUDES
 */
#if defined(FEATURE_MONITOR)

#include <urcli.h>
#include <ll_common.h>
#include <port.h>
#include <uble.h>
#include <ull.h>
#include "ti_radio_config.h"

/*********************************************************************
 * CONSTANTS
 */

#if (!defined(RF_SINGLEMODE) && !defined(RF_MULTIMODE)) ||                   \
    (defined(RF_SINGLEMODE) && defined(RF_MULTIMODE))
  #error "Either RF_SINGLEMODE or RF_MULTIMODE should be defined."
#endif /* RF_SINGLEMODE, RF_MULTIMODE */

#if (defined(FEATURE_SCANNER) && defined(FEATURE_MONITOR))
  #error "FEATURE_SCANNER and FEATURE_MONITOR cannot be defined at the same time."
#endif /* FEATURE_SCANNER, FEATURE_MONITOR */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */
/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
/*********************************************************************
 * GLOBAL VARIABLES
 */

RCL_Handle    urcliHandle;

RCL_CommandStatus urcliGenericRxHandle = RCL_CommandStatus_Idle;

/* RCL_CmdGenericRx Command */
RCL_CmdBle5GenericRx urcliGenericRxCmd;
RCL_StatsGenericRx urcliGenericRxCmdStats;
RCL_CtxGenericRx urcliCtxGenericRx;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
extern void ull_setupMonitorDataEntryQueue( void );

/*********************************************************************
 * LOCAL VARIABLES
 */

RCL_Client  rfClient;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      urcli_initGenericRxCmd
 *
 * @brief   Initialize Scan RF command
 *
 * @param   None
 *
 * @return  None
 */
void urcli_initGenericRxCmd(void)
{
  urcliGenericRxCmd = RCL_CmdBle5GenericRx_DefaultRuntime();
  urcliGenericRxCmdStats = RCL_StatsGenericRx_DefaultRuntime();
  ull_setupMonitorDataEntryQueue();

  urcliGenericRxCmd.stats = &urcliGenericRxCmdStats;
  urcliCtxGenericRx.config.repeated = TRUE;
  urcliGenericRxCmd.ctx   = &urcliCtxGenericRx;
}

/*********************************************************************
 * @fn      urcli_init
 *
 * @brief   Initialize radio interface and radio commands
 *
 * @param   None
 *
 * @return  SUCCESS - RF driver has been successfully opened
 *          FAILURE - Failed to open RF driver
 */
bStatus_t urcli_init(void)
{
  RCL_init();

  /* Open client and provide settings */
  urcliHandle = RCL_open(&rfClient, &LRF_config);

#if defined(FEATURE_MONITOR)
  urcli_initGenericRxCmd();
#endif  /* FEATURE_SCANNER */

  return SUCCESS;
}

/*********************************************************************
 * CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
*********************************************************************/
#endif //#if defined(FEATURE_MONITOR)
