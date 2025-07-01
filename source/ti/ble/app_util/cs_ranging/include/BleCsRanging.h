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
 * Type/Data structure Definitions
 * Utilize RCL as much as possible for type consistency but can be redefined for extension
 */
#define PCT_LEN         75  /*!< Maximum length of PCT vector*/
#define MAX_NUM_ANTPATH 4   /*!< Maximum number of antenna path*/
#define MAX_RANGE       150 /*!< Maximum range can be estimated*/

typedef uint16_t BleCsRanging_Return_t;

/**
 *
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
    BleCsRanging_Algorithm_Ifft,
    BleCsRanging_Algorithm_Music,
    BleCsRanging_Algorithm_NN,
    BleCsRanging_Algorithm_Adaptive
} BleCsRanging_Algorithm_e;

/**
 * @brief Enumerator of distances fusion method
 */
typedef enum BleCsRanging_DistanceFusion_e
{
    BleCsRanging_DistanceFusion_Min,
    BleCsRanging_DistanceFusion_Median,
    BleCsRanging_DistanceFusion_Ranking
} BleCsRanging_DistanceFusion_e;

/**
 * @brief Enumerator of interpolation method for gap recovery
 */
typedef enum BleCsRanging_GapInterp_e
{
    BleCsRanging_GapInterp_Linear,
    BleCsRanging_GapInterp_Spline,
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

typedef struct BleCsRanging_Config_t
{
    uint16_t maxDistance;                     /*!< Maximum Distance to measure in meter, must less than 150m*/
    uint16_t numAntPath;                      /*!< Number of antenna path, must less than 5*/
    uint16_t numChannels;                     /*!< Number of actual steps, must less than 75*/
    uint16_t qq3Thresh;                       /*!< Quality Threshold to select algorithm dynamically-very good signal*/
    uint16_t qq3Thresh2;                      /*!< Second Quality Threshold to select algorithm dynamically-very bad signal*/
    float distanceOffset;                     /*!< Distance Offset from Calibration in meters*/
    BleCsRanging_MAP_e sumAntPath;            /*!< Individutal or Summation before estimating distance*/
    BleCsRanging_GapInterp_e gapInterp;       /*!< Interplation method for gap*/
    BleCsRanging_Algorithm_e algorithm;       /*!< Enum to select the algorithm for distance*/
    BleCsRanging_DistanceFusion_e distFusion; /*!< Combine Antenna Path Method*/
} BleCsRanging_Config_t;

/**
 *  @brief Algorithm status
 *
 *  Gives information on the Algorithm, and if finished, how it finished.
 */
typedef enum
{
    BleCsRanging_Status_Success,       /*!< BleCsRanging_AlgorithmStatus_Success */
    BleCsRanging_Status_InvalidInput,  /*!< BleCsRanging_AlgorithmStatus_InvalidInput */
    BleCsRanging_Status_InvalidOutput, /*!< BleCsRanging_AlgorithmStatus_InvalidOutput */
    BleCsRanging_Status_APUFail        /*!< BleCsRanging_AlgorithmStatus_APUFail */
} BleCsRanging_Status_e;

typedef struct
{
    float distanceMusic[MAX_NUM_ANTPATH]; /*!< distance MUSIC of each antenna path*/
    float distanceNN[MAX_NUM_ANTPATH];    /*!< distance NN of each antenna path*/
    uint16_t numMPC[MAX_NUM_ANTPATH];     /*!< number of multipath-component (MPC) of each antenna path*/
    float quality[MAX_NUM_ANTPATH];       /*!< quality metric QQ3 of each antenna path*/
    float confidence[MAX_NUM_ANTPATH];    /*!< confidence of each antenna path*/
} BleCsRanging_DebugResult_t;

typedef struct
{
    float distance;                           /*!< estimated distance*/
    float quality;                            /*!< quality metric QQ3 of the estimated distance*/
    float confidence;                         /*!< confidence of the estimation*/
    uint16_t numMPC;                          /*!< number of multipath-component (MPC) of the estimated distance*/
    BleCsRanging_DebugResult_t *pDebugResult; /*!< debug result*/
} BleCsRanging_Result_t;

/***********************************************************************************
 * Functions/APIs definitions
 **********************************************************************************/
/**
 * Main function for Phase-based Ranging algorithms
 * NOTE: Current version assumed input is stored as
 *  [tone_ant0[PCT_LEN], tone_ant1[PCT_LEN], tone_ant2[PCT_LEN], tone_ant3[PCT_LEN]]
 * Future version will relax this assumption
 */
BleCsRanging_Status_e BleCsRanging_estimatePbr(BleCsRanging_Result_t *pResult,
                                               BleCsRanging_Tone_t *pTone_i,  // Tone with TQI from Initiator
                                               BleCsRanging_Tone_t *pTone_r,  // Tone with TQI from Reflector
                                               BleCsRanging_Config_t *pConfig // General config
);

/**
 * A default funciton to init the default config
 * NOTE: Declared as ((weak)) for backward-compatible and open to be override
 */
extern BleCsRanging_Return_t __attribute__((weak)) BleCsRanging_initConfig(BleCsRanging_Config_t *pConfig);

#endif //_BLECSRANGING_H_
