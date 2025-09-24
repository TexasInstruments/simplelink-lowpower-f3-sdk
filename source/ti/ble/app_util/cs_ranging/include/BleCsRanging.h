/*
 * Copyright (c) 2024-2025 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free,
 * non-exclusive license under copyrights and patents it now or hereafter
 * owns or controls to make, have made, use, import, offer to sell and sell ("Utilize")
 * this software subject to the terms herein.  With respect to the foregoing patent
 * license, such license is granted  solely to the extent that any such patent is necessary
 * to Utilize the software alone.  The patent license shall not apply to any combinations which
 * include this software, other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license (including the
 * above copyright notice and the disclaimer and (if applicable) source code license limitations below)
 * in the documentation and/or other materials provided with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided that the following
 * conditions are met:
 *
 *   * No reverse engineering, decompilation, or disassembly of this software is permitted with respect to any
 *     software provided in binary form.
 *   * any redistribution and use are licensed by TI for use only with TI Devices.
 *   * Nothing shall obligate TI to provide you with source code for the software licensed and provided to you in object
 * code.
 *
 * If software source code is provided to you, modification and redistribution of the source code are permitted
 * provided that the following conditions are met:
 *
 *   * any redistribution and use of the source code, including any resulting derivative works, are licensed by
 *     TI for use only with TI Devices.
 *   * any redistribution and use of any object code compiled from the source code and any resulting derivative
 *     works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be used to endorse or
 * promote products derived from this software without specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _BLECSRANGING_H_
#define _BLECSRANGING_H_

#include <stdint.h>
#include <stdbool.h>
#include <float.h>

/**
 * Type/Data structure definitions
 */
#define PCT_LEN         75  /*!< Maximum length of PCT vector*/
#define MAX_NUM_ANTPATH 4   /*!< Maximum number of antenna path*/
#define MAX_RANGE       150 /*!< Maximum range can be estimated*/
#define MAX_NNCC_BIN    64

typedef uint16_t BleCsRanging_Return_t; /*!< Function return code, see BleCsRanging_Status_e */

typedef int8_t BleCsRanging_PathLoss_t; /*!< Path loss, dB */

typedef int8_t BleCsRanging_RPL_t;      /*!< Reference Power Level, dBm */

/**
 * @brief Container format for IQ samples (measured PCT)
 */
typedef struct
{
    int16_t i; /*!< Sample I-branch */
    int16_t q; /*!< Sample Q-branch */
} BleCsRanging_IQSample_t;

/**
 *  @brief Container format for tones
 *
 */
typedef struct BleCsRanging_Tone_t
{
    uint32_t i:12;
    uint32_t q:12;
    uint32_t quality:8;
} BleCsRanging_Tone_t;

/**
 *  @brief Enumerator of algorithm option
 *
 *  Describes the available options for algorithm selection
 */
typedef enum
{
    BleCsRanging_Algorithm_Ifft,    /*!< IFFT based algorithm */
    BleCsRanging_Algorithm_Music,   /*!< MUltiple SIgnal Classificatoin (MUSIC) based algorithm */
    BleCsRanging_Algorithm_NN,      /*!< Neural Network (NN) based algorithm */
    BleCsRanging_Algorithm_Adaptive /*!< Adaptive selection of MUSIC or NN algorithms */
} BleCsRanging_Algorithm_e;

/**
 * @brief Enumerator of distances fusion method
 */
typedef enum BleCsRanging_DistanceFusion_e
{
    BleCsRanging_DistanceFusion_Min,
    BleCsRanging_DistanceFusion_Median,
    BleCsRanging_DistanceFusion_Ranking,
    BleCsRanging_DistanceFusion_MinAvg,
} BleCsRanging_DistanceFusion_e;

/**
 * @brief Enumerator of interpolation method for gap recovery
 */
typedef enum BleCsRanging_GapInterp_e
{
    BleCsRanging_GapInterp_Linear,
    BleCsRanging_GapInterp_Spline, /*!< Not implemented. For future use! */
    BleCsRanging_GapInterp_OMP
} BleCsRanging_GapInterp_e;

/**
 * @brief Enumerator of multiple antennapath preprocessing (MAP)
 */
typedef enum BleCsRanging_MAP_e
{
    BleCsRanging_MAP_Individual,
    BleCsRanging_MAP_Averaging
} BleCsRanging_MAP_e;

/**
 * @brief Filter chain
 *
 * This enum defines the different filter chains to be used.
 */
typedef enum
{
    BleCsRanging_FilterChain_None,
    BleCsRanging_FilterChain_Average,
    BleCsRanging_FilterChain_Kalman,
    BleCsRanging_FilterChain_AverageKalman
} BleCsRanging_FilterChain_e;

/**
 * @brief Configuration for BLE CS ranging algorithm method
 */
typedef struct BleCsRanging_Config_t
{
    uint16_t maxDistance;   /*!< Maximum distance to measure in meter, must less than 150m */
    uint16_t numAntPath;    /*!< Number of antenna paths, max 4 */
    uint16_t numChannels;   /*!< Number of actual CS steps, up to 75 */
    uint16_t qq3Thresh;     /*!< Quality threshold to select algorithm dynamically-very good signal */
    uint16_t qq3Thresh2;    /*!< Second quality threshold to select algorithm dynamically-very bad signal */
    int8_t NnPathLossThres; /*!< When PathLoss = txPower - RSSI < NnPathLossThres for debugging purposes */
    float distanceOffset;   /*!< Distance offset from calibration, in meters */
    BleCsRanging_MAP_e sumAntPath;            /*!< Individual or Summation before estimating distance */
    BleCsRanging_GapInterp_e gapInterp;       /*!< Interpolation method for gap */
    BleCsRanging_Algorithm_e algorithm;       /*!< Enum to select the algorithm for distance */
    BleCsRanging_DistanceFusion_e distFusion; /*!< Combine Antenna Path Method */
    BleCsRanging_FilterChain_e antFilter;     /*!< Antenna Path Filtering Method */
} BleCsRanging_Config_t;

/**
 *  @brief Algorithm status
 *
 *  Gives information on the Algorithm, and if finished, how it finished.
 */
typedef enum
{
    BleCsRanging_Status_Success,       /*!< Success */
    BleCsRanging_Status_InvalidInput,  /*!< Invalid Input */
    BleCsRanging_Status_InvalidOutput, /*!< Invalid Output */
    BleCsRanging_Status_APUFail,       /*!< APU Failed */
    BleCsRanging_Status_Undefined      /*!< Undefined, used as uninitialized value, application should never see this. */
} BleCsRanging_Status_e;

typedef struct
{
    float distanceMusic[MAX_NUM_ANTPATH];  /*!< Distance MUSIC algo of each antenna path */
    float distanceNN[MAX_NUM_ANTPATH];     /*!< Distance NN algo of each antenna path */
    float confidence[MAX_NUM_ANTPATH];     /*!< Confidence metric of each antenna path. Reserved for future use. */
    uint16_t numMPC[MAX_NUM_ANTPATH];      /*!< Number of MUSIC multipath-components of each antenna path */
    float quality[MAX_NUM_ANTPATH];        /*!< Quality metric QQ3 of each antenna path */
    float zoneProfile[MAX_NUM_ANTPATH][5]; /*!< Debug information: Reserved for future use */
    float normDistance[MAX_NUM_ANTPATH];   /*!< Debug information: Reserved for future use */
    float normTerm[MAX_NUM_ANTPATH];       /*!< Debug information: Reserved for future use */
    float LOS_d[MAX_NUM_ANTPATH];          /*!< Debug information: Reserved for future use */
    float thLOS[MAX_NUM_ANTPATH];          /*!< Debug information: Reserved for future use */
    float dth[MAX_NUM_ANTPATH];            /*!< Debug information: Reserved for future use */
    float powerDelayProfile[MAX_NUM_ANTPATH][MAX_NNCC_BIN]; /*!< Debug information: Reserved for future use */
    float tau_mean[MAX_NUM_ANTPATH];       /*!< Debug information: Reserved for future use */
    float tau_rms[MAX_NUM_ANTPATH];        /*!< Debug information: Reserved for future use */
    float runtime_ms;                      /*!< Debug information: Reserved for future use */
} BleCsRanging_DebugResult_t;

typedef struct
{
    float distance;                           /*!< Estimated distance, meters */
    float quality;                            /*!< Quality metric QQ3 of the estimated distance */
    float confidence;                         /*!< Confidence of the estimation */
    uint16_t numMPC;                          /*!< Number of multipath-component (MPC) of the estimated distance */
    BleCsRanging_DebugResult_t *pDebugResult; /*!< Debug results (optional) */
} BleCsRanging_Result_t;

/***********************************************************************************
 * Functions/APIs definitions
 **********************************************************************************/
/**
 * Main function for Phase-based Ranging algorithms
 * NOTE: Current version assumed input is stored as
 *  [tone_ant0[PCT_LEN], tone_ant1[PCT_LEN], tone_ant2[PCT_LEN], tone_ant3[PCT_LEN]]
 * Future version will relax this assumption
 *
 * @param pResult Result struct
 * @param pTone_i Tone with TQI from Initiator
 * @param pTone_r Tone with TQI from Reflector
 * @param pathLoss pathLoss = txPower-RSSI for each pConfig.numAntPath, set to NULL to disable NN selection based on
 * pConfig.NnPathLossThres
 * @param pConfig General config
 */

BleCsRanging_Status_e BleCsRanging_estimatePbr(BleCsRanging_Result_t *pResult,
                                               BleCsRanging_Tone_t *pTone_i,
                                               BleCsRanging_Tone_t *pTone_r,
                                               BleCsRanging_PathLoss_t *pathLoss,
                                               BleCsRanging_RPL_t *pRPL_i, // RPL initiator
                                               BleCsRanging_RPL_t *pRPL_r, // RPL reflector
                                               BleCsRanging_Config_t *pConfig);

/**
 * A default funciton to init the default config
 * NOTE: Declared as ((weak)) for backward-compatible and open to be override
 */
extern BleCsRanging_Return_t __attribute__((weak)) BleCsRanging_initConfig(BleCsRanging_Config_t *pConfig);

#endif //_BLECSRANGING_H_
