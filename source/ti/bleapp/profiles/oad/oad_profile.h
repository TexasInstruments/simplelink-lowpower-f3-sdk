/******************************************************************************

@file  oad_profile.h

 @brief /////

Group: WCS, BTS
$Target Device: DEVICES $

******************************************************************************
$License: BSD3 2022 $
******************************************************************************
$Release Name: PACKAGE NAME $
$Release Date: PACKAGE RELEASE DATE $
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
 * @{
 */
typedef enum OADProfile_App_Msg_e
{
    OAD_PROFILE_MSG_REVOKE_IMG_HDR, //!< Status from profile to app revoke app header
    OAD_PROFILE_MSG_NEW_IMG_IDENDIFY, //!< For off-chip inform app about new image identify
    OAD_PROFILE_MSG_START_DOWNLOAD, //!< Start download new image
    OAD_PROFILE_MSG_FINISH_DOWNLOAD, //!< Dowanload of new image has finished
    OAD_PROFILE_MSG_RESET_REQ, //!< Status from profile to app for get permission to do reset
}OADProfile_App_Msg_e;

typedef OADProfile_AppCommand_e (*OADProfile_AppCallback_t)(OADProfile_App_Msg_e msg);

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
