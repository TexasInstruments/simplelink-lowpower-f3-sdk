/******************************************************************************

@file  oad_profile.h

 @brief /////

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

#ifndef OAD_PROFILE_H
#define OAD_PROFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * MACROS
 */


/*********************************************************************
 * TYPEDEFS
 */
typedef enum OADProfile_AppCommand_e
{
    OAD_PROFILE_PROCEED,
    OAD_PROFILE_CANCEL
}OADProfile_AppCommand_e;

/*!
 * @OADProfile_App_Msg_e OAD msg from oad profile to app
 *
 * Events marked "REQUIRES INTERVENTION" need application to return OAD_PROFILE_PROCEED
 * or OAD_PROFILE_CANCEL. Events marked "NOTIFICATION" are informational only.
 * @{
 */
typedef enum OADProfile_App_Msg_e
{
    /**
     * Revoke image header (REQUIRES INTERVENTION) - On-chip OAD only
     * pData: NULL
     * Return: PROCEED to allow revocation, CANCEL to reject
     */
    OAD_PROFILE_MSG_REVOKE_IMG_HDR,

    /**
     * New image identification (REQUIRES INTERVENTION)
     * pData: Pointer to struct image_header (candidate image metadata)
     * Return: PROCEED to accept image, CANCEL to reject
     */
    OAD_PROFILE_MSG_NEW_IMG_IDENTIFY,

    /**
     * Start download request (REQUIRES INTERVENTION)
     * pData: NULL
     * Return: PROCEED to allow download, CANCEL to reject
     */
    OAD_PROFILE_MSG_START_DOWNLOAD,

    /**
     * Download finished (NOTIFICATION)
     * pData: NULL
     */
    OAD_PROFILE_MSG_FINISH_DOWNLOAD,

    /**
     * Download cancelled (NOTIFICATION)
     * pData: NULL
     */
    OAD_PROFILE_MSG_CANCEL_DOWNLOAD,

    /**
     * Reset request (REQUIRES INTERVENTION)
     * pData: NULL
     * Return: PROCEED to allow reset, CANCEL to delay
     */
    OAD_PROFILE_MSG_RESET_REQ,

    /**
     * Download failed (NOTIFICATION)
     * pData: NULL
     */
    OAD_PROFILE_MSG_DOWNLOAD_FAILED,

    /**
     * OAD inactivity timeout (NOTIFICATION)
     * Occurs after 10 seconds of inactivity in CONFIG, DOWNLOAD, or COMPLETE state.
     * pData: NULL
     */
    OAD_PROFILE_MSG_TIMEOUT,

#ifdef MS_OAD
    /**
     * Get MS-OAD state query (REQUIRES INTERVENTION - OUTPUT)
     * Profile queries application for current MS-OAD state to send to client.
     * pData: Pointer to uint8 output variable (write msOadState_e value here)
     * Return: PROCEED after writing state, CANCEL on error
     */
    OAD_PROFILE_MSG_GET_MS_OAD_STATE,

    /**
     * MS-OAD initialization request (REQUIRES INTERVENTION)
     * Application should call MSOAD_InitAndReset() if approved (device will reset).
     * pData: NULL
     * Return: PROCEED to approve, CANCEL to reject
     */
    OAD_PROFILE_MSG_MS_OAD_INIT_REQ,

    /**
     * MS-OAD commit request (REQUIRES INTERVENTION)
     * Application should call MSOAD_ImageVerified() to mark image as good.
     * pData: NULL
     * Return: PROCEED after verification, CANCEL to reject
     */
    OAD_PROFILE_MSG_MS_OAD_COMMIT,
#endif
}OADProfile_App_Msg_e;

/*
 * @OADProfile_AppCallback_t OAD profile callback function prototype
 *
 * @param   msg - message ID
 * @param   pData - pointer to message specific data. For some events, pData might be
 *                  used as a return value from the callback function to the profile.
 *
 * @return  OADProfile_AppCommand_e indicating whether to proceed or cancel the operation
 *
 * @see OADProfile_App_Msg_e for details on which events require pData
 *      usage and expected data types.
 *
 * @see OADProfile_App_Msg_e for details on which events needs the application
 *      intervention and which are informational only.
 */
typedef OADProfile_AppCommand_e (*OADProfile_AppCallback_t)(OADProfile_App_Msg_e msg, void *pData);

/*********************************************************************
 * API FUNCTIONS
 */
extern bStatus_t OADProfile_start(OADProfile_AppCallback_t pOADAppCB);

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OAD_PROFILE_H */
