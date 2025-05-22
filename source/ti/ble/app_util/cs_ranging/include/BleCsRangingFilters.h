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

#ifndef _BLECSRANGINGFILTERS_H_
#define _BLECSRANGINGFILTERS_H_

#include "BleCsRanging.h"

/**
 * @brief Structure to represent a Kalman filter
 */
typedef struct
{
    float x_est;   ///< Estimated distance (in meters)
    float px_est;  ///< Estimated error covariance (in meters^2)
    float v_est;   ///< Estimated velocity (in m/s)
    float pv_est;  ///< Estimated velocity error covariance
    float t;       ///< Time (in seconds)
    float K_n;     ///< Kalman gain
    float x_pred;  ///< Predicted state (estimated distance) in meters
    float px_pred; ///< Predicted error covariance
    float v_pred;  ///< Predicted velocity (in m/s)
    float pv_pred; ///< Predicted velocity error covariance
} BleCsRanging_KalmanFilter_t;

/**
 * @brief Structure to represent a Moving Average filter
 */
typedef struct
{
    uint16_t sizeM; ///< Size of the filter
    uint16_t sizeN; ///< Size of the effective buffer
    float *buffer;  ///< Buffer to store previous values
    uint16_t index; ///< Current index in the buffer
    uint16_t count; ///< Total number of values passed to buffer
} BleCsRanging_MovingAverageFilter_t;

/**
 * @brief Filter chain
 * 
 * This enum defines the different filter chains to be used.
 */
typedef enum
{
    BleCsRanging_FilterChain_Average,
    BleCsRanging_FilterChain_Kalman,
    BleCsRanging_FilterChain_AverageKalman
} BleCsRanging_FilterChain_e;


/**
 * @brief Filter all function
 *
 * This function filters the given estimated distance using the filter chain.
 *
 * @param maf Pointer to the Moving Average filter structure
 * @param kf Pointer to the Kalman filter structure
 * @param filterChain The filter chain to use
 * @param d_est Estimated distance in meters
 * @param ts Time in seconds
 * @return The filtered distance
 */
float BleCsRanging_filterAll(BleCsRanging_MovingAverageFilter_t *maf, //
                             BleCsRanging_KalmanFilter_t *kf,         //
                             BleCsRanging_FilterChain_e filterChain,
                             float d_est, //
                             float ts);

/**
 * @brief Initialize the Kalman filter
 *
 * This function initializes the Kalman filter structure with the given parameters.
 *
 * @param kf Pointer to the Kalman filter structure
 * @param x_0 Initial state
 * @param px_0 Initial error covariance
 * @param v0 Initial velocity
 * @param pv_0 Initial velocity error covariance
 * @param t0 Initial time in seconds
 */
BleCsRanging_Return_t BleCsRanging_initKalmanFilter(BleCsRanging_KalmanFilter_t *kf,
                                                    float x_0,
                                                    float px_0,
                                                    float v0,
                                                    float pv_0,
                                                    float t0);

/**
 * @brief Predict the state using the Kalman filter
 *
 * This function predicts the state of the Kalman filter based on the current time.
 *
 * @param kf Pointer to the Kalman filter structure
 * @param t_cur Current time in seconds
 */
void BleCsRanging_predictKalmanFilter(BleCsRanging_KalmanFilter_t *kf, float t_cur);

/**
 * @brief Update the state using the Kalman filter
 *
 * This function updates the state of the Kalman filter based on a measurement and its variance.
 *
 * @param kf Pointer to the Kalman filter structure
 * @param z_n Measurement
 * @param r_n Measurement noise
 */
void BleCsRanging_updateKalmanFilter(BleCsRanging_KalmanFilter_t *kf, float z_n, float r_n);

/**
 * @brief Initialize the Moving Average filter
 *
 * This function initializes the Moving Average filter structure with the given parameters.
 *
 * @param maf Pointer to the Moving Average filter structure
 * @param M Size of the filter
 * @param N Size of the effective buffer
 */
void BleCsRanging_initMovingAverageFilter(BleCsRanging_MovingAverageFilter_t *maf, uint16_t M, uint16_t N);

/**
 * @brief Update the Moving Average filter
 *
 * This function updates the Moving Average filter with a new value and returns the averaged value.
 *
 * @param maf Pointer to the Moving Average filter structure
 * @param value New value to add to the filter in meter
 * @return The averaged value
 */
float BleCsRanging_computeMovingAverage(BleCsRanging_MovingAverageFilter_t *maf, float value);

/**
 * @brief Free the resources allocated by the Moving Average filter
 *
 * @param maf Pointer to the Moving Average filter structure
 */
void freeMovingAverageFilter(BleCsRanging_MovingAverageFilter_t *maf);

/**
 * @brief Compute the Kalman filter estimate
 *
 * This function predicts the state using the Kalman filter and then updates the state based on a measurement.
 *
 * @param kf Pointer to the Kalman filter structure
 * @param t_cur_second Current time in seconds
 * @param d_est_t Estimated distance measurement (in meters)
 * @return The estimated state (x_est) of the Kalman filter (in meters)
 */
float BleCsRanging_computeKalman(BleCsRanging_KalmanFilter_t *kf, float t_cur_second, float d_est_t);

#endif //_BLECSRANGINGFILTERS_H_
