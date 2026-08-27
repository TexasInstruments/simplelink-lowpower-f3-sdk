/******************************************************************************

@file  ms_oad_profile.h

@brief This file contains the device info application functionality

Group: WCS, BTS
Target Device: cc23xx

******************************************************************************

 Copyright (c) 2022-2026, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************


*****************************************************************************/

#ifdef MS_OAD

#ifndef MS_OAD_PROFILE_H
#define MS_OAD_PROFILE_H

//*****************************************************************************
//! Includes
//*****************************************************************************

#include "ti/ble/app_util/sw_update/sw_update.h"

//*****************************************************************************
//! Defines
//*****************************************************************************

//*****************************************************************************
//! Typedefs
//*****************************************************************************

// Middle man module
typedef enum
{
    MS_OAD_UNINITIALIZED = 0,           // MS-OAD module not initialized yet

    MS_OAD_IDLE,                        // Currently running the most recent image

    MS_OAD_WAIT_FIRST_HALF,             // Currently running the original image (first half), waiting for the newer image (first half)

    MS_OAD_WAIT_SECOND_HALF,            // Currently running the newer image (first half), waiting for the newer image (second half).
                                        // When done, will wait for OAD_EVT_ENABLE_IMG command, then reboot into MS_OAD_NEW_IMAGE_READY state.
                                        // If client fails to detect MS_OAD_NEW_IMAGE_READY state, or failed to connect, it shall consider
                                        // the OAD process as failed, and wait for the device to reboot into MS_OAD_WAIT_FIRST_HALF_REVERT state.

    MS_OAD_NEW_IMAGE_READY,             // Fully booted with the new image, waiting for the client to commit (send OAD_MS_OAD_COMMIT command),
                                        // then the device will commit the new image and set its state to MS_OAD_IDLE.
                                        // Reboot is needed after that so mcuboot will commit also.
                                        // If client fails to commit, the device will reboot into MS_OAD_WAIT_FIRST_HALF_REVERT state.

    MS_OAD_WAIT_FIRST_HALF_REVERT,      // Currently running the newer image (first half), waiting for the older image (first half) in order to revert

    MS_OAD_WAIT_SECOND_HALF_REVERT,     // Currently running the original image (first half), waiting for the older image (second half) in order to revert

    MS_OAD_IMAGE_READY_REVERT,          // Fully booted with the reverted image, waiting for the client to commit (send OAD_MS_OAD_COMMIT command),
                                        // then the device will commit the reverted image and set its state to MS_OAD_IDLE.
                                        // Reboot is needed after that so mcuboot will commit also.

    MS_OAD_WAIT_IMAGE_ENABLE,           // Half-image download complete, waiting for OAD_EVT_ENABLE_IMG command from the client.
                                        // Upon receiving the enable command, the device will reboot.

    MS_OAD_DOWNLOADING,                 // Download in progress. Saves the previous state and transitions here when download starts.
                                        // On download done/failed, uses the saved previous state to determine next action.
                                        // Note: This state can't be queried by the client, as it is only used internally during the OAD process.

    MS_OAD_NUM_STATES
} msOadState_e;

//*****************************************************************************
//! Globals
//*****************************************************************************

//*****************************************************************************
//! Functions
//*****************************************************************************

/*********************************************************************
 * @fn      MSOAD_InitIfRequired
 *
 * @brief   Initialize MS-OAD module if an OAD process is in progress.
 *
 *          This function checks the boot state to determine if MS-OAD
 *          initialization is required. If the device is in the middle
 *          of an MS-OAD process, it initializes the BLE stack to continue
 *          the OAD operation.
 *
 *          IMPORTANT: Call this function early in the application's
 *          boot sequence before application initialization.
 *          If it returns true, the MS-OAD has been initialized and the
 *          application must not continue with any other initialization.
 *
 * @return  true - MS-OAD was initialized.
 *                 Application should not continue.
 *          false - MS-OAD is not active. Application should proceed
 *                  with normal initialization.
 */
bool MSOAD_InitIfRequired();

/*********************************************************************
 * @fn      MSOAD_InitAndReset
 *
 * @brief   Initialize MS-OAD process and prepare for new image download.
 *
 *          This function initiates the MS-OAD state machine by:
 *          1. Validating the new image version is greater than committed version
 *          2. Erasing the journal
 *          3. Writing the version entry with committed and pending versions
 *          4. Setting the initMsOad flag
 *          5. Rebooting the device
 *
 *          After reboot, the device will enter WAIT_FIRST_HALF state
 *          and begin waiting for the first half of the new image.
 *
 * @param   pNewImageVersion - Pointer to the new image version to download.
 *                             Must be greater than the current committed version.
 *
 * @return  SUCCESS - MS-OAD initialization succeeded, device will reboot.
 *          FAILURE - Initialization failed (invalid version, NVS error, etc.).
 */
bStatus_t MSOAD_InitAndReset(struct image_version* pNewImageVersion);

/*********************************************************************
 * @fn      MSOAD_ImageVerified
 *
 * @brief   Confirm that the application has verified the new image.
 *
 *          This function should be called by the main application after
 *          successfully verifying the downloaded image functionality.
 *          It:
 *          1. Sets the image_ok flag to confirm the current image
 *          2. Erases the journal to mark OAD completion
 *          3. Reboots the device to enter IDLE state
 *
 *          IMPORTANT: If this API is not called within the watchdog timeout
 *          after booting into NEW_IMAGE_READY or IMAGE_READY_REVERT state,
 *          the device will reset automatically and revert to the previous
 *          working image.
 *
 * @return  SUCCESS - Image verification recorded, device will reboot.
 *          FAILURE - Verification failed (NVS error, etc.).
 */
bStatus_t MSOAD_ImageVerified();

/*********************************************************************
 * @fn      MSOAD_GetState
 *
 * @brief   Get the current MS-OAD state.
 *
 *          Returns the current state of the MS-OAD state machine.
 *          This can be used by the application to determine:
 *          - Whether a new image needs verification (NEW_IMAGE_READY state)
 *          - Whether an OAD process is in progress
 *          - The current phase of the OAD operation
 *
 *          Possible states:
 *          - MS_OAD_IDLE: No OAD in progress, running committed image
 *          - MS_OAD_WAIT_FIRST_HALF: Waiting for first half download
 *          - MS_OAD_WAIT_SECOND_HALF: Waiting for second half download
 *          - MS_OAD_NEW_IMAGE_READY: New image booted, needs verification
 *          - MS_OAD_WAIT_FIRST_HALF_REVERT: Reverting, waiting for first half
 *          - MS_OAD_WAIT_SECOND_HALF_REVERT: Reverting, waiting for second half
 *          - MS_OAD_IMAGE_READY_REVERT: Reverted image booted, needs verification
 *          - MS_OAD_WAIT_IMAGE_ENABLE: Waiting for enable command before reboot
 *          - MS_OAD_DOWNLOADING: Download in progress
 *
 * @return  Current MS-OAD state (msOadState_e)
 */
msOadState_e MSOAD_GetState();

#endif //MS_OAD_PROFILE_H

#endif //MS_OAD