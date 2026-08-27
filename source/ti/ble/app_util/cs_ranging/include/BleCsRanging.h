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
#include <ti/drivers/utils/List.h>
/**
 * Type/Data structure definitions
 */
#define PCT_LEN                         75   /*!< Maximum length of PCT vector */
#define MAX_NUM_ANTPATH                 4    /*!< Maximum number of antenna path */
#define BLECSRANGING_MAX_RANGE          150  /*!< Maximum range can be estimated */
#define BLECSRANGING_MAX_MUSIC_SPECTRUM 320  /*!< Maximum music spectrum can be estimated */
#define BLECSRANGING_HEAP_SIZE          1500 /*!< Size of heap of RangingLib in number of floats */

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

/*!< Sign-extend a 12-bit unsigned bitfield value (from BleCsRanging_Tone_t.i/.q) to int16_t */
#define BLECSRANGING_SIGN_EXT12(v) ((int16_t)(((uint16_t)(v) & 0x800u) ? ((uint16_t)(v) | 0xF000u) : (uint16_t)(v)))

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
    uint16_t numAntPath;                      /*!< Number of antenna paths, max 4 */
    uint16_t numChannels;                     /*!< Number of actual CS steps, up to 75 */
    BleCsRanging_TimingParams_t timingParams; /*!< Timing parameters */
    uint16_t maxDistance;                     /*!< Maximum distance to measure in meter, must less than 150m */
    float distanceOffset;                     /*!< Distance offset from calibration, in meters */
    float maxVelocity;                        /*!< Maximum velocity in meters per second for motion compensation */
    BleCsRanging_MAP_e sumAntPath;            /*!< Individual or Summation before estimating distance */
    BleCsRanging_GapInterp_e gapInterp;       /*!< Interpolation method for gap */
    BleCsRanging_Algorithm_e algorithm;       /*!< Enum to select the algorithm for distance */
    BleCsRanging_DistanceFusion_e distFusion; /*!< Combine Antenna Path Method */
    BleCsRanging_AdaptiveProfile_e adaptiveProfile; /*!< NLOS adaptive profile */
    float tqiThresh;                                /*!< TQI threshold */
    float peakEnergyThresh;                         /*!< Peak energy threshold */
    uint16_t peakDiffThresh;                        /*!< Peak distance difference threshold */
    float dVarMax;                                  /*!< Max distance variance*/
    uint16_t resetHist;                             /*!< Reset history when disconnected for a long time*/
    float iirCoeff;      /*!< IIR filter coefficient sets weight of current dataset vs history
                              data buffer, in range [0, 1]. Higher value gives more weight to current dataset.*/
    uint8_t *pBuffer;    /*!< For Adaptive algorithm: application needs to provide pointer to a history
                           data buffer for BleCsRanging library to store the last datasets for smoother estimation.
                           The application must use the function @ref BleCsRanging_getHeapSize to get the required buffer
                           size. */
    uint8_t *pBufferPCT; /*!< Buffer to store PCT values for adaptive algorithm */
    bool isInlinePCT; /*!< Set @c true to enable BT v6.3 Inline PCT (one-sided) mode.
                       *   When @c true, @c pReflectorSubevents passed to
                       *   @c BleCsRanging_estimatePbr is unused; reflector tones are
                       *   synthesised internally as unit complex values so the two-way
                       *   multiply degrades to a one-way (initiator-only) operation.
                       *   Default: @c false (standard two-way PBR). */
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
    float distanceMusic;              /*!< Distance MUSIC algo of each antenna path */
    float distanceNN;                 /*!< Distance NN algo of each antenna path */
    float distanceIFFT;               /*!< Distance IFFT algo of each antenna path */
    float confidence;                 /*!< Confidence metric of each antenna path. Reserved for future use. */
    uint16_t numMPC;                  /*!< Number of MUSIC multipath-components of each antenna path */
    float quality[MAX_NUM_ANTPATH];   /*!< Quality metric QQ3 of each antenna path */
    float tqi_score[MAX_NUM_ANTPATH]; /*!< Debug information: Reserved for future use */
    float dcand;                      /*!< Debug information: Reserved for future use */
    float cf;                         /*!< Debug information: Reserved for future use */
    float d_var;                      /*!< Debug information: Reserved for future use */
    uint16_t class;                   /*!< Debug information: Reserved for future use */
    float runtime_ms;                 /*!< Debug information: Reserved for future use */
    float runtimeProfile[10];         /*!< Debug information: Reserved for future use */
    uint16_t peakBinIFFT;             /*!< Peak Bin IFFT*/
    uint16_t peakCountIFFT;           /*!< Peak Count IFFT*/
    uint16_t ifftValid;               /*!< IFFT Valid*/
} BleCsRanging_DebugResult_t;

typedef struct
{
    float distance;                           /*!< Estimated distance, meters */
    float quality;                            /*!< Quality metric QQ3 of the estimated distance */
    float confidence;                         /*!< Confidence of the estimation */
    float velocity;                           /*!< Estimated velocity (meters/second) used in motion compensation */
    BleCsRanging_DebugResult_t *pDebugResult; /*!< Debug results (optional) */
} BleCsRanging_Result_t;

/***********************************************************************************
 * CS Subevent data structures — multi-subevent support (BLECSRANGING-86)
 **********************************************************************************/

/*!
 * @brief CS procedure configuration parameters.
 *        Populated once per CS configuration from HCI_LE_CS_CONFIG_COMPLETE event.
 *        Passed separately to BleCsRanging_estimatePbr(); not linked to subevent list.
 */
typedef struct
{
    uint8_t tFCS;                       /*!< T_FCS: frequency change/settle period (us) */
    uint8_t tIP1;                       /*!< T_IP1: interlude period between CS packets (us) */
    uint8_t tIP2;                       /*!< T_IP2: interlude period between CS tones (us) */
    uint8_t tPM;                        /*!< T_PM: phase measurement period (us) */
    uint8_t tSW;                        /*!< T_SW: antenna switch period (us) */
    uint8_t csSync;                     /*!< CS_SYNC type (see BT Core Spec CS_SYNC_PHY) */
    uint8_t rttType;                    /*!< RTT type (see BT Core Spec RTT_Type) */
    uint8_t mode0Steps;                 /*!< Number of Mode 0 steps per subevent */
    uint8_t mainModeType;               /*!< Main_Mode_Type (1=Mode1, 2=Mode2, 3=Mode3) */
    uint8_t mainModeRepetition;         /*!< Main_Mode_Repetition count (0..3)*/
    uint8_t toneAntennaConfigSelection; /*!< Antenna Configuration Index (0..7) */
} BleCsRanging_CsConfig_t;

/*!
 * @brief CS subevent header. Represents one HCI_LE_CS_Subevent_Result event.
 *        One contiguous allocation block holds this header followed by all step data.
 *        numAntennaPaths applies to every step in this subevent.
 *
 *        Linked list usage:
 *          - elem.next / elem.prev managed by List_put() / List_get()
 *          - Cast to node: (BleCsRanging_CsSubevent_t *)List_head(&list)
 */
typedef struct
{
    List_Elem elem;                /*!< Linked list node — MUST be first member */
    int16_t frequencyCompensation; /*!< Frequency compensation, units: 0.01 ppm (initiator: measured; reflector: 0) */
    uint16_t numSteps;             /*!< Number of steps reported in this subevent */
    int8_t referencePowerLevel;    /*!< Reference power level (dBm) */
    uint8_t numAntennaPaths;       /*!< Antenna paths per step (valid range: 1..4) */
    uint8_t pad[2];                /*!< Reserved, set to 0 */
    /* Step data follows immediately in the same allocation block */
} BleCsRanging_CsSubevent_t;

/*!
 * @brief CS Mode 0 step data. Synchronization and frequency offset calibration.
 *        measuredFreqOffset is valid for initiator role only; reflector sets it to 0.
 */
typedef struct
{
    uint8_t stepMode;           /*!< Step mode = 0 */
    uint8_t stepChannel;        /*!< CS channel index for this step (2..80) */
    uint8_t packetQuality;      /*!< Packet quality indicator (bit mask per BT Core Spec) */
    int8_t packetRssi;          /*!< RSSI of received packet (dBm) */
    int16_t measuredFreqOffset; /*!< Measured frequency offset (units: 0.01 ppm, initiator only) */
    uint8_t packetAntenna;      /*!< Antenna ID used for this step (1-based index) */
    uint8_t pad;                /*!< Reserved, set to 0 */
} BleCsRanging_StepMode0_t;

/*!
 * @brief CS Mode 1 step data. Round-Trip Time (RTT) ranging.
 *        ToFdelta = ToA_ToD (initiator) or ToD_ToA (reflector).
 *        Sounding Sequence is not supported; Packet_PCT fields are omitted.
 */
typedef struct
{
    uint8_t stepMode;      /*!< Step mode = 1 */
    uint8_t stepChannel;   /*!< CS channel index for this step (2..80) */
    uint8_t packetQuality; /*!< Packet quality indicator (bit mask per BT Core Spec) */
    uint8_t packetNadm;    /*!< Normalized Attack Detector Metric(NADM) indicator */
    int8_t packetRssi;     /*!< RSSI of received packet (dBm) */
    int16_t ToFdelta;      /*!< Time-of-Flight delta (units: 0.5 ns) */
    uint8_t packetAntenna; /*!< Antenna Identifider used for CS_SYNC packets (1..4) */
} BleCsRanging_StepMode1_t;

/*!
 * @brief CS Mode 2 step data. Phase-Based Ranging (PBR) only.
 *        Sounding Sequence is not supported; Packet_PCT fields are omitted.
 *        tonePCT[] element count equals numAntennaPaths from the containing subevent header.
 */
typedef struct
{
    uint8_t stepMode;                /*!< Step mode = 2 */
    uint8_t stepChannel;             /*!< CS channel index for this step (2..80) */
    uint8_t antennaPermutationIndex; /*!< Antenna permutation index (see BT Core Spec Table) */
    uint8_t pad;                     /*!< Reserved for 4-byte alignment of tonePCT */
    BleCsRanging_Tone_t tonePCT[];   /*!< PCT+TQI per antenna path; count = numAntennaPaths */
} BleCsRanging_StepMode2_t;

/*!
 * @brief CS Mode 3 step data. RTT and Phase-Based Ranging (RTT + PBR).
 *        Combines Mode 1 RTT fields with Mode 2 PBR tone fields.
 *        Sounding Sequence is not supported; Packet_PCT fields are omitted.
 *        tonePCT[] element count equals numAntennaPaths from the containing subevent header.
 */
typedef struct
{
    uint8_t stepMode;                /*!< Step mode = 3 */
    uint8_t stepChannel;             /*!< CS channel index for this step (2..80) */
    uint8_t packetQuality;           /*!< Packet quality indicator (bit mask per BT Core Spec) */
    uint8_t packetNadm;              /*!< Normalized Attack Detector Metric(NADM) indicator */
    int8_t packetRssi;               /*!< RSSI of received packet (dBm) */
    int16_t ToFdelta;                /*!< Time-of-Flight delta (units: 0.5 ns) */
    uint8_t packetAntenna;           /*!< Antenna ID used for this step (1-based index) */
    uint8_t antennaPermutationIndex; /*!< Antenna permutation index */
    uint8_t pad[3];                  /*!< Reserved for 4-byte alignment of tonePCT */
    BleCsRanging_Tone_t tonePCT[];   /*!< PCT+TQI per antenna path; count = numAntennaPaths */
} BleCsRanging_StepMode3_t;

/* Step size macros */
#define BLECSRANGING_STEP_MODE0_SIZE (sizeof(BleCsRanging_StepMode0_t))
#define BLECSRANGING_STEP_MODE1_SIZE (sizeof(BleCsRanging_StepMode1_t))
#define BLECSRANGING_STEP_MODE2_SIZE(numAntPaths) \
    (sizeof(BleCsRanging_StepMode2_t) + (uint32_t)(numAntPaths) * sizeof(BleCsRanging_Tone_t))
#define BLECSRANGING_STEP_MODE3_SIZE(numAntPaths) \
    (sizeof(BleCsRanging_StepMode3_t) + (uint32_t)(numAntPaths) * sizeof(BleCsRanging_Tone_t))

/* Full subevent allocation size macros (header + all steps contiguous) */
#define BLECSRANGING_SUBEVENT_ALLOC_MODE0_MODE2(numMode0Steps, numMode2Steps, numAntPaths)          \
    (sizeof(BleCsRanging_CsSubevent_t) + (uint32_t)(numMode0Steps) * BLECSRANGING_STEP_MODE0_SIZE + \
     (uint32_t)(numMode2Steps) * BLECSRANGING_STEP_MODE2_SIZE(numAntPaths))

/*!
 * @brief Get a pointer to the first step in a subevent's step data area.
 *
 * @param[in] pSubevent  Pointer to the subevent header.
 * @return               Pointer to the first step byte, or NULL if numSteps == 0.
 */
static inline const void *BleCsRanging_subEventFirstStep(const BleCsRanging_CsSubevent_t *pSubevent)
{
    if (pSubevent->numSteps == 0)
    {
        return (NULL);
    }

    return ((const uint8_t *)pSubevent + sizeof(BleCsRanging_CsSubevent_t));
}

/*!
 * @brief Advance a step pointer to the next step.
 *        Reads stepMode from the current step to determine its size.
 *
 * @param[in] pStep           Pointer to the current step (any mode).
 * @param[in] numAntennaPaths numAntennaPaths from the containing subevent header.
 * @return                    Pointer to the next step, or NULL on unrecognized mode.
 */
static inline const void *BleCsRanging_stepAdvance(const void *pStep, uint8_t numAntennaPaths)
{
    const uint8_t *p      = (const uint8_t *)pStep;
    volatile uint8_t mode = p[0]; /* stepMode is always the first byte of any step struct */
    volatile uint32_t size;

    switch (mode)
    {
        case 0:
            size = BLECSRANGING_STEP_MODE0_SIZE;
            break;

        case 1:
            size = BLECSRANGING_STEP_MODE1_SIZE;
            break;

        case 2:
            size = BLECSRANGING_STEP_MODE2_SIZE(numAntennaPaths);
            break;

        case 3:
            size = BLECSRANGING_STEP_MODE3_SIZE(numAntennaPaths);
            break;

        default:
            return (NULL);
    }

    return (p + size);
}

/***********************************************************************************
 * Functions/APIs definitions
 **********************************************************************************/
/*!
 * @brief Estimate distance using Phase-Based Ranging (PBR) from multiple CS subevents.
 *
 * @pre  pInitiatorSubevents and pReflectorSubevents must contain the same number of
 *       subevent nodes, ordered identically (subevent index 0 first in each list).
 * @pre  Each subevent in both lists must have identical numAntennaPaths.
 * @pre  pCsConfig, pConfig, and pResult must be non-NULL.
 *
 * @param[in]  pInitiatorSubevents  Pointer to initiator CS subevent linked list (List_List).
 *                                  Each node is a BleCsRanging_CsSubevent_t allocation block.
 * @param[in]  pReflectorSubevents  Pointer to reflector CS subevent linked list (List_List).
 *                                  Each node is a BleCsRanging_CsSubevent_t allocation block.
 * @param[in]  pCsConfig            CS procedure configuration (from HCI_LE_CS_CONFIG_COMPLETE).
 * @param[in]  pConfig              Algorithm configuration parameters (legacy interface, unchanged).
 * @param[out] pResult              Distance estimation result (legacy interface, unchanged).
 *
 * @return BleCsRanging_Status_e indicating success or error.
 *
 * @retval BleCsRanging_Status_Success       Success. The result in @p pResult is valid.
 * @retval BleCsRanging_Status_InvalidInput  Invalid input. The result in @p pResult is invalid.
 * @retval BleCsRanging_Status_Undefined     Stub: implementation pending (BLECSRANGING-86).
 */
BleCsRanging_Status_e BleCsRanging_estimatePbr(List_List *pInitiatorSubevents,
                                               List_List *pReflectorSubevents,
                                               const BleCsRanging_CsConfig_t *pCsConfig,
                                               const BleCsRanging_Config_t *pConfig,
                                               BleCsRanging_Result_t *pResult);

/* Forward declaration — full definition in BleCsRangingProcess.h */
typedef struct BleCsRanging_PbrInput_t BleCsRanging_PbrInput_t;

/*!
 * @brief Extract PBR measurement inputs from CS subevent linked lists.
 *
 * Validates inputs and walks the paired initiator/reflector subevent lists,
 * collecting mode-2 and mode-3 tone data into @p pInput. Also validates
 * mode-0 step counts for link quality assurance.
 *
 * @param[out] pInput               Caller-allocated struct to fill.
 * @param[in]  pInitiatorSubevents  Initiator subevent linked list.
 * @param[in]  pReflectorSubevents  Reflector subevent linked list.
 * @param[in]  pCsConfig            CS procedure configuration.
 * @param[in]  pConfig              Algorithm configuration.
 *
 * @return BleCsRanging_Status_e
 * @retval BleCsRanging_Status_Success      Extraction succeeded; pInput is valid.
 * @retval BleCsRanging_Status_InvalidInput NULL argument, mismatched step counts,
 *                                          zero tones, or failed mode-0 check.
 */
BleCsRanging_Status_e BleCsRanging_extractPbrInput(BleCsRanging_PbrInput_t *pInput,
                                                   List_List *pInitiatorSubevents,
                                                   List_List *pReflectorSubevents,
                                                   const BleCsRanging_CsConfig_t *pCsConfig,
                                                   const BleCsRanging_Config_t *pConfig);

/*!
 * @brief Validate subevent pairs and descramble tone data in-place.
 *
 * Validates the paired initiator/reflector subevent lists (NULL checks,
 * @c numAntPath range, per-subevent mode-0 step count and packet quality).
 * For every mode-2 and mode-3 step, de-scrambles @c tonePCT in-place using
 * @c BleCsRanging_sortTonePCT and resets @c antennaPermutationIndex to 0 so
 * subsequent callers see tones in canonical antenna-path order.
 *
 * Must be called before @c BleCsRanging_stitchSubevents and
 * @c BleCsRanging_extractPbrInput.
 *
 * @param[in,out] pInitiatorSubevents  Initiator subevent linked list (tones modified in-place)
 * @param[in,out] pReflectorSubevents  Reflector subevent linked list (tones modified in-place)
 * @param[in]     pCsConfig            CS procedure configuration (@c mode0Steps)
 * @return BleCsRanging_Status_Success or BleCsRanging_Status_InvalidInput
 */
BleCsRanging_Status_e BleCsRanging_preprocessSubevents(List_List *pInitiatorSubevents,
                                                       List_List *pReflectorSubevents,
                                                       const BleCsRanging_CsConfig_t *pCsConfig);

/*!
 * @brief Apply inter-subevent phase stitching to initiator tone data.
 *
 * When @c pCsConfig->mainModeRepetition > 0, each subevent's last
 * @c mainModeRepetition mode-2 steps are repeated at the start of the next
 * subevent (same channels). This function estimates the inter-subevent phase
 * drift per antenna path from those repeated steps and applies the inverse
 * rotation to every mode-2/3 initiator tone in each subsequent subevent.
 *
 * Must be called before @c BleCsRanging_extractPbrInput so that extracted
 * tones are already phase-corrected.
 * No-op when @c mainModeRepetition == 0 or fewer than 2 subevents are present.
 * Only initiator tones are modified; reflector data is read-only.
 *
 * @param[in,out] pInitiatorSubevents  Linked list of initiator subevent structs
 * @param[in]     pReflectorSubevents  Linked list of reflector subevent structs
 * @param[in]     pCsConfig            CS configuration (@c mainModeRepetition, @c mode0Steps)
 * @return BleCsRanging_Status_Success, or BleCsRanging_Status_InvalidInput if
 *         the subevent lists are mismatched
 */
BleCsRanging_Status_e BleCsRanging_stitchSubevents(List_List *pInitiatorSubevents,
                                                   List_List *pReflectorSubevents,
                                                   const BleCsRanging_CsConfig_t *pCsConfig);

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

/**
 * @brief Calculate the required heap buffer size for the PCT algorithm
 *
 * This function calculates the required buffer size (in bytes) if a history data buffer is
 * to be used. The application must call this function to determine the buffer size
 * needed for the pBufferPCT field in BleCsRanging_Config_t before calling BleCsRanging_estimatePbr.
 *
 * @param pConfig Pointer to the configuration structure
 *
 * @return Required buffer size in bytes. If pConfig is NULL or invalid, returns 0.
 *
 * @see BleCsRanging_Config_t::pBufferPCT
 */
uint16_t BleCsRanging_getHeapSizePCT(BleCsRanging_Config_t *pConfig);

#endif //_BLECSRANGING_H_
