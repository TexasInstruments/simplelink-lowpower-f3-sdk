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
#include <math.h>
#include <complex.h>

/**
 * Type/Data structure definitions
 */
#define PCT_LEN              75  /*!< Maximum length of PCT vector */
#define MAX_NUM_ANTPATH      4   /*!< Maximum number of antenna path */
#define MAX_RANGE            150 /*!< Maximum range can be estimated */
#define MAX_MUSIC_SPECTRUM   256
#define RANGINGLIB_HEAP_SIZE 1000 /*!< Size of heap of RangingLib in number of floats */

typedef uint16_t BleCsRanging_Return_t; /*!< Function return code, see BleCsRanging_Status_e */

typedef int8_t BleCsRanging_PathLoss_t; /*!< Path loss, dB */

typedef int8_t BleCsRanging_RPL_t; /*!< Reference Power Level, dBm */

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
 *  @brief Container format for timing parameters
 * The values to use should be same value as in CS_CONFIG_COMPLETE event.
 * Ranginglib will convert to us internally.
 * Note: T_SW is special, see index mapping below.
 *
 */
typedef struct BleCsRanging_TimingParams_t
{
    uint8_t tIP1; /*!< The interlude period used between RTT packets (us)*/
    uint8_t tIP2; /*!< The interlude period used between CS tones (us) */
    uint8_t tFCs; /*!< The period of frequency changes (us) */
    uint8_t tPM;  /*!< The measurement period of CS tones (us) */
    uint8_t tSw;  /*!< The switch period (us) */
} BleCsRanging_TimingParams_t;

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
    BleCsRanging_Algorithm_Adaptive /*!< Adaptive algorithm using a fusion algorithm */
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
    BleCsRanging_GapInterp_Circle,
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
    BleCsRanging_FilterChain_None,         /*!< No filter chain */
    BleCsRanging_FilterChain_Average,      /*!< Average filter chain */
    BleCsRanging_FilterChain_Kalman,       /*!< Kalman filter chain */
    BleCsRanging_FilterChain_AverageKalman /*!< Average + Kalman filter chain */
} BleCsRanging_FilterChain_e;

/**
 * @brief Enumerator of adaptive profile
 *
 * This enum defines the different adaptive profiles that can be used
 */
typedef enum
{
    BleCsRanging_AdaptiveProfile_Low,
    BleCsRanging_AdaptiveProfile_Medium,
    BleCsRanging_AdaptiveProfile_High
} BleCsRanging_AdaptiveProfile_e;

/**
 * @brief Configuration for BLE CS ranging algorithm method
 */
typedef struct BleCsRanging_Config_t
{
    uint16_t maxDistance;                           /*!< Maximum distance to measure in meter, must less than 150m */
    uint16_t numAntPath;                            /*!< Number of antenna paths, max 4 */
    uint16_t numChannels;                           /*!< Number of actual CS steps, up to 75 */
    float distanceOffset;                           /*!< Distance offset from calibration, in meters */
    float maxVelocity;                              /*!< Maximum velocity in meters per second for motion compensation */
    BleCsRanging_MAP_e sumAntPath;                  /*!< Individual or Summation before estimating distance */
    BleCsRanging_GapInterp_e gapInterp;             /*!< Interpolation method for gap */
    BleCsRanging_Algorithm_e algorithm;             /*!< Enum to select the algorithm for distance */
    BleCsRanging_DistanceFusion_e distFusion;       /*!< Combine Antenna Path Method */
    BleCsRanging_FilterChain_e antFilter;           /*!< Antenna Path Filtering Method */
    BleCsRanging_AdaptiveProfile_e adaptiveProfile; /*!< NLOS adaptive profile */
    float tqiThresh;                                /*!< TQI threshold */
    float peakEnergyThresh;                         /*!< Peak energy threshold */
    uint16_t peakDiffThresh;                        /*!< Peak distance difference threshold */
    float dVarMax;                                  /*!< Max distance variance*/
    uint16_t resetHist;                             /*!< Reset history when disconnected for a long time*/
    float iirCoeff;                                 /*!< IIR filter coefficient sets weight of current dataset vs history
                                                         data buffer, in range [0, 1]. Higher value gives more weight to current dataset.*/
    BleCsRanging_TimingParams_t timingParams;       /*!< Timing parameters */
    uint8_t *pBuffer; /*!< For Adaptive algorithm: application needs to provide pointer to a history
                          data buffer for BleCsRanging library to store the last datasets for smoother estimation.
                          The application must use the function @ref BleCsRanging_getHeapSize to get the required buffer
                         size. */
} BleCsRanging_Config_t;

/**
 *  @brief Algorithm status
 *
 *  Gives information on the Algorithm, and if finished, how it finished.
 */
typedef enum
{
    BleCsRanging_Status_Success = 0,   /*!< Success */
    BleCsRanging_Status_InvalidInput,  /*!< Invalid input argument error */
    BleCsRanging_Status_InvalidOutput, /*!< Invalid output argument error */
    BleCsRanging_Status_APUError,      /*!< APU access error */
    BleCsRanging_Status_MemAllocError, /*!< Dynamic memory allocation on heap failed */
    BleCsRanging_Status_Undefined /*!< Undefined, used as uninitialized value, application should never see this. */
} BleCsRanging_Status_e;

typedef struct
{
    float distanceMusic[MAX_NUM_ANTPATH];  /*!< Distance MUSIC algo of each antenna path */
    float distanceNN[MAX_NUM_ANTPATH];     /*!< Distance NN algo of each antenna path */
    float distanceIFFT[MAX_NUM_ANTPATH];   /*!< Distance IFFT algo of each antenna path */
    float confidence[MAX_NUM_ANTPATH];     /*!< Confidence metric of each antenna path. Reserved for future use. */
    uint16_t numMPC[MAX_NUM_ANTPATH];      /*!< Number of MUSIC multipath-components of each antenna path */
    float quality[MAX_NUM_ANTPATH];        /*!< Quality metric QQ3 of each antenna path */
    float zoneProfile[MAX_NUM_ANTPATH][5]; /*!< Debug information: Reserved for future use */
    float normDistance[MAX_NUM_ANTPATH];   /*!< Debug information: Reserved for future use */
    float normTerm[MAX_NUM_ANTPATH];       /*!< Debug information: Reserved for future use */
    float tqi_score[MAX_NUM_ANTPATH];      /*!< Debug information: Reserved for future use */
    float peak_diff[MAX_NUM_ANTPATH];      /*!< Debug information: Reserved for future use */
    float dcand[MAX_NUM_ANTPATH];          /*!< Debug information: Reserved for future use */
    float cf[MAX_NUM_ANTPATH];             /*!< Debug information: Reserved for future use */
    float LOSd[MAX_NUM_ANTPATH];           /*!< Debug information: Reserved for future use */
    float d_var[MAX_NUM_ANTPATH];          /*!< Debug information: Reserved for future use */
    uint16_t class[MAX_NUM_ANTPATH];       /*!< Debug information: Reserved for future use */
    float runtime_ms;                      /*!< Debug information: Reserved for future use */
    float runtimeProfile[10];              /*!< Debug information: Reserved for future use */
    uint16_t peakBinIFFT;    /*!< Peak Bin IFFT*/
    uint16_t peakCountIFFT;  /*!< Peak Count IFFT*/
    uint16_t ifftValid;      /*!< IFFT Valid*/
} BleCsRanging_DebugResult_t;

typedef struct
{
    float distance;                           /*!< Estimated distance, meters */
    float quality;                            /*!< Quality metric QQ3 of the estimated distance */
    float confidence;                         /*!< Confidence of the estimation */
    uint16_t numMPC;                          /*!< Number of multipath-component (MPC) of the estimated distance */
    float velocity;                           /*!< Estimated velocity (meters/second) used in motion compensation */
    BleCsRanging_DebugResult_t *pDebugResult; /*!< Debug results (optional) */
} BleCsRanging_Result_t;

/***********************************************************************************
 * Functions/APIs definitions
 **********************************************************************************/
/**
 * Main function for Phase-based Ranging algorithms
 * NOTE: Current version assumed input is stored as
 *  [tone_ant0[PCT_LEN], tone_ant1[PCT_LEN], tone_ant2[PCT_LEN], tone_ant3[PCT_LEN]]
 * NOTE: The new vector with channel list will have 72 entries, chronological order.
 * The values in the array should be the same channel index as coming from the HCI report.
 * (lowest=2, highest=76).
 * Future version will relax this assumption
 *
 * @param pResult Result struct
 * @param pTone_i Tone with TQI from Initiator
 * @param pTone_r Tone with TQI from Reflector
 * @param pRPL_i Reference Power Level (RPL) from Initiator
 * @param pRPL_r Reference Power Level (RPL) from Reflector
 * @param pChannelIdxList Channel index list
 * @param pConfig General config
 *
 * @return Status of distance estimation
 *
 * @retval BleCsRanging_Status_Success          Success. The result in @pResult is valid.
 * @retval BleCsRanging_Status_InvalidInput     Invalid input error. The result in @pResult is invalid.
 * @retval BleCsRanging_Status_MemAllocError    Dynamic memory allocation on heap failed. Not enough memory. The result
 * in @pResult is invalid.
 * @retval BleCsRanging_Status_Undefined        Undefined error.
 */
BleCsRanging_Status_e BleCsRanging_estimatePbr(BleCsRanging_Result_t *pResult,
                                               BleCsRanging_Tone_t *pTone_i, /*!< Tone with TQI from Local */
                                               BleCsRanging_Tone_t *pTone_r, /*!< Tone with TQI from Remote */
                                               BleCsRanging_RPL_t *pRPL_i,   /*!<  Reference Power Level (RPL) local */
                                               BleCsRanging_RPL_t *pRPL_r,   /*!< Reference Power Level (RPL) remote */
                                               uint8_t *pChannelIdxList,     /*!< Channel index list */
                                               BleCsRanging_Config_t *pConfig);

/**
 * @brief Initialize the configuration structure with default values
 *
 * This function initializes the BleCsRanging_Config_t structure with default configuration values.
 * NOTE: Declared as ((weak)) for backward-compatibility and open to be overridden.
 *
 * @param pConfig Pointer to the configuration structure to initialize
 *
 * @return Status of initialization
 *
 * @retval BleCsRanging_Status_Success      Configuration initialized successfully
 * @retval BleCsRanging_Status_InvalidInput Invalid input error
 */
extern BleCsRanging_Status_e __attribute__((weak)) BleCsRanging_initConfig(BleCsRanging_Config_t *pConfig);

/**
 * @brief Calculate the required heap buffer size for the ranging algorithm
 *
 * This function calculates the required buffer size (in bytes) if a history data buffer is
 * to be used. The application must call this function to determine the buffer size
 * needed for the pBuffer field in BleCsRanging_Config_t before calling BleCsRanging_estimatePbr.
 *
 * @param pConfig Pointer to the configuration structure
 *
 * @return Required buffer size in bytes. If pConfig is NULL or invalid, returns 0.
 *
 * @see BleCsRanging_Config_t::pBuffer
 */
uint16_t BleCsRanging_getHeapSize(BleCsRanging_Config_t *pConfig);

#endif //_BLECSRANGING_H_
