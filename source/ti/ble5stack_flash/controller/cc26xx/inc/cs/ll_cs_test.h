/******************************************************************************

 @file  ll_cs_test.h

 @brief CS Test Mode header file.

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

#ifndef LL_CS_TEST_H
#define LL_CS_TEST_H
/*******************************************************************************
 * INCLUDES
 */
#include "cs/ll_cs_common.h"

/*******************************************************************************
 * CONSTANTS
 */
#define CS_TEST_USER_PAYLOAD                   0x80
#define CS_TEST_PRBS9_PAYLOAD                  0x00
#define CS_TEST_PRBS15_PAYLOAD                 0x03
#define CS_TEST_NUM_PAYLOADS                   0x08
#define CS_TEST_MODE_CONN_ID                   0
#define CS_TEST_MODE_CONFIG_ID                 0

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * EXTERNS
 */

/*******************************************************************************
 * TYPEDEFS
 */
/* CS Test Mode States */
typedef enum csTestMode_e
{
    CS_TEST_MODE_ENABLE,
    CS_TEST_MODE_TERMINATE,
    CS_TEST_MODE_FINISHED,
    CS_TEST_MODE_DISABLE,
} csTestMode_e;

/* Bitwise field that tells which config uses overrides */
typedef struct csOverrideCfg
{
    uint8 chanCfg           :1;
    uint8 rfu0              :1;
    uint8 numMainModeSteps  :1;
    uint8 toneExtension     :1;
    uint8 antennaPermutation:1;
    uint8 aa                :1;
    uint8 ssMarkerPosition  :1;
    uint8 ssMarkerValue     :1;
    uint8 payload           :1;
    uint8 rfu1              :1;
    uint8 stablePhaseTest   :1;
    uint8 rfu2              :4;
} csOverrideCfg_t;

/* CS Test Override Data */
typedef struct csTestOverrideData
{
    uint16 drbgNonce;
    csOverrideCfg_t overrideCfg;
    uint8  overrideLen;
    uint8* overrideParams;
    struct
    {
        uint8 nMainModeSteps;
        uint8 toneExt;
        uint8 antennaPermuation;
        uint32_t aaTxInit;
        uint32_t aaTxRef;
        uint8 payloadPattern;
        uint32_t userPayload[4];
    } parsedParams;
} csTestOverrideData_t;

/* CS Test Parameters */
typedef struct csTestParams
{
    uint8 mainMode;
    uint8 subMode;
    uint8 mainModeRep;
    uint8 nMode0Steps;
    csRole_e role;
    uint8 rttType;
    uint8 csSyncPhy;
    uint8 csSyncAntSel;
    uint32 subeventLen;
    uint16 subeventInterval;
    uint8 maxNumSubevents;
    int8 tpl; /* Transmit Power Level */
    uint8 tIp1;
    uint8 tIp2;
    uint8 tFcs;
    uint8 tPm;
    uint8 tSw;
    uint8 toneAntCfg;
    uint8 rfu;
    uint8 snrCtrlInit;
    uint8 snrCtrlRef;
    uint16 drbgNonce;
    uint8 chmRep;
    csOverrideCfg_t overrideCfg;
    uint8 overrideLen;
    uint8* overrideParams;
} csTestParams_t;

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * @fn          llCsCheckTestParams
 *
 * @brief       Check CS Test Parameters
 * This function checks the values of the parameters of the CS Test command.
 *
 * input parameters
 *
 * @param       pParams - pointer to CS Test Params
 *
 * output parameters
 *
 * @param       None
 *
 * @return      CS_STATUS_SUCCESS - if parameters values are accepted
 *              CS_STATUS_UNEXPECTED_PARAMETER - parameter value isn't accepted
 *              CS_STATUS_FEATURE_NOT_SUPPORTED - feature isn't supported
 */
csStatus_e llCsCheckTestParams(csTestParams_t* pParams);

/*******************************************************************************
 * @fn          llCsTestGetPayload
 *
 * @brief       Get Test Payload per payload pattern
 *
 * input parameters
 *
 * @param       plPtrn - Payload Pattern
 * @param       ptr - Pointer to copy pattern
 *
 * output parameters
 *
 * @param       ptr - Copied payload pattern
 *
 * @return      None
 */
void llCsTestGetPayload(uint8 plPtrn, uint32_t* ptr);

/*******************************************************************************
 * @fn          llCsInitChanIdxArrOverride
 *
 * @brief       Initialize the Channel Index Array Override function
 * This function checks if Channel Override is used, if it is it does nothing.
 * Because when overrides are used, the channel index arrays are not used and
 * need not be initialized.
 * If it isn't, calls the original initialize function
 *
 * input parameters
 *
 * @param       configId - Configuration Id
 * @param       connId - Connection Id
 * @param       csConfig - Pointer to CS config
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Status
 */
uint8 llCsInitChanIdxArrOverride(uint8 configId, uint16 connId, csConfigurationSet_t* csConfig);

/*******************************************************************************
 * @fn          llCsSelectStepChanOverride
 *
 * @brief       Select Step Channel Override Function
 * If Channel override is used, there is no need to shuffle. Just choose the
 * channel from the array.
 * If not used, then the shuffle function is called before selecting channel.
 *
 * input parameters
 *
 * @param       stepMode - CS Step mode
 * @param       connId - Connection Id
 * @param       config - Pointer to CS config
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Channel
 */
uint8 llCsSelectStepChanOverride(uint8 stepMode, uint16 connId, uint8* config);

/*******************************************************************************
 * @fn          llCsSelectAAOverride
 *
 * @brief       Select Access Address Override
 * If AA Override is used, the relevant override value is fetched
 * Otherwise, the original AA selection function is called
 *
 * input parameters
 *
 * @param       csRole - CS Role
 * @param       aaRx - Pointer to Rx AA
 * @param       aaTx - Pointer to Tx AA
 *
 * output parameters
 *
 * @param       aaRx - Pointer to Rx AA
 * @param       aaTx - Pointer to Tx AA
 *
 * @return      None
 */
void llCsSelectAAOverride(uint8 csRole, uint32_t* aaRx, uint32_t* aaTx);

/*******************************************************************************
 * @fn          llCsAAOverride
 *
 * @brief       Override Access Address
 * This function overrides the Access Address in a CS step according to the
 * Override Parameters in a CS Test.
 *
 * input parameters
 *
 * @param       csRole - CS initiator or reflector
 * @param       aaRx - pointer to Rx Access Address
 * @param       aaTx - pointer to TX Access Address
 *
 * output parameters
 *
 * @param       aaRx - RX AA
 * @param       aaTx - RX AA
 *
 * @return      TRUE - if CS test mode is enabled and AA override is used
 *              FALSE - if CS test mode is disabled or AA override isn't used
 */
uint8 llCsAAOverride(uint8 csRole, uint32_t* aaRx, uint32_t* aaTx);

/*******************************************************************************
 * @fn          llCsGetRandomSequenceOveride
 *
 * @brief       Random Sequence Override function
 * If Payload Override is used, the relevant override value is fetched
 * Otherwise, the original Payload selection function is called
 * input parameters
 *
 * @param       csRole - CS Role
 * @param       pTx - Pointer to Tx Payload
 * @param       pRx - Pointer to Rx Payload
 * @param       plLen - Payload length
 *
 * output parameters
 *
 * @param       pTx - Pointer to Tx Payload
 * @param       pRx - Pointer to Rx Payload
 *
 * @return      None
 */
void llCsGetRandomSequenceOveride(uint8 csRole, uint32_t* pTx, uint32_t* pRx, uint8 plLen);

/*******************************************************************************
 * @fn          llCsPayloadOverride
 *
 * @brief       Override the payload in a CS Step
 * Used when CS Test is in progress and the payload override config bit is on
 *
 * input parameters
 *
 * @param       csRole - initiator or reflector
 * @param       pTx - payload for TX
 * @param       pRx - payload for RX
 *
 * output parameters
 *
 * @param       pTx - payload for TX
 * @param       pRx - payload for RX
 *
 * @return      TRUE - if CS test mode is enabled and PL override is used
 *              FALSE - if CS test mode is disabled or PL override isn't used
 */
uint8 llCsPayloadOverride(uint8 csRole, uint32_t* pTx, uint32_t* pRx);

/*******************************************************************************
 * @fn          llCsGetToneExtentionOverride
 *
 * @brief       Tone Extension Override
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None
 *
 * @return      Tone Extension
 */
uint8 llCsGetToneExtentionOverride(void);

/*******************************************************************************
 * @fn          llCsToneExtentionOverride
 *
 * @brief       Override Tone Extension
 * This function will override the tone extension bit if a CS test mode is
 * enabled and the tone extension bit in the override config is enabled.
 * If the override tone extension value is 0, 1, or 2. The value will be
 * returned as is. If the tone extension config value is 4, then on each
 * iteration it will loop over 0, 1 or 2.
 *
 * input parameters
 *
 * @param       pToneExt - Pointer to tone extension
 *
 * output parameters
 *
 * @param       pToneExt - The override value for Tone Extension
 *
 * @return      TRUE - if tone extension override is used
 *              FALSE - if tone extension override is not used
 */
uint8 llCsToneExtentionOverride(void);

#endif
