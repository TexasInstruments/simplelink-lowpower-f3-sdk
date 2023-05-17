/******************************************************************************

 @file  cgm_profile.h

 @brief This file contains the CGM profile definitions and prototypes.

 Group: WCS, BTS
 $Target Device: DEVICES $

 ******************************************************************************
 $License: BSD3 2010 $
 ******************************************************************************
 $Release Name: PACKAGE NAME $
 $Release Date: PACKAGE RELEASE DATE $
 *****************************************************************************/

#ifndef CGMPROFILE_H
#define CGMPROFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <ti/bleapp/ble_app_util/inc/bleapputil_api.h>
#include <ti/bleapp/services/continuous_glucose_monitoring/cgm_server.h>

/*********************************************************************
 * DEFINES
 */
// CGMP default session run time
#define CGMP_DEFAUALT_SRT         12

// CGMP convert minutes to milliseconds
#define CGMP_MIM_TO_MSEC          60000

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * Profile Callback
 */

//Callback to indicate client characteristic configuration has been updated for Measurement characteristic
typedef void ( *CGMP_measOnCccUpdate_t )( uint16 connHandle, uint16 pValue );

//Callback to indicate client characteristic configuration has been updated for RACP characteristic
typedef void ( *CGMP_racpOnCccUpdate_t )( uint16 connHandle, uint16 pValue );

//Callback to indicate client characteristic configuration has been updated for CGMCP characteristic
typedef void ( *CGMP_cgmcpOnCccUpdate_t )( uint16 connHandle, uint16 pValue );

//Callback to indicate that session start time value has been updated
typedef void ( *CGMP_sstUpdate_t )( CGMS_sst_t *pValue );

typedef struct
{
  CGMP_measOnCccUpdate_t   pfnMeasOnCccUpdateCB;   // Called when client characteristic configuration has been updated on Measurement characteristic
  CGMP_racpOnCccUpdate_t   pfnRACPOnCccUpdateCB;   // Called when client characteristic configuration has been updated on RACP characteristic
  CGMP_cgmcpOnCccUpdate_t  pfnCGMCPOnCccUpdateCB;  // Called when client characteristic configuration has been updated on CGMCP characteristic
  CGMP_sstUpdate_t         pfnSSTUpdateCB;         // Called when session start time value has been updated
} CGMP_cb_t;

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      CGMP_start
 *
 * @brief   This function adds the CGM Server service,
 *          registers the service callback function and initializes
 *          the service characteristics
 *
 * @param   appCallbacks - pointer to application callback
 * @param   feat - CGM Feature parameters
 * @param   timeOffset - CGM initial time offset parameter
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t CGMP_start( CGMP_cb_t *appCallbacks, CGMS_feat_t feat, uint16 timeOffset );

/*
 * @fn      CGMP_addMeaserment
 *
 * @brief   Add CGM measurement to data base
 *
 * @param   glucoseConcen - a Glucose concentration in mg/dL units
 * @param   timeOffset - Minutes since the Session Start Time
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t CGMP_addMeaserment( uint16  glucoseConcen, uint16  timeOffset );

/*
 * @fn      CGMP_updateStatus
 *
 * @brief   Update status characteristic on the server attributes table
 *
 * @param   status - updated status value
 * @param   len - length of data to write
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t CGMP_updateStatus( CGMS_status_t stat );

/*
 * @fn      CGMP_updateSessionRunTime
 *
 * @brief   Update session run time characteristic on the server attributes table
 *
 * @param   timeToReduce - value in hours to reduce from session run time charicterstic
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t CGMP_updateSessionRunTime( uint16 timeToReduce );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* CGMROFILE_H */
