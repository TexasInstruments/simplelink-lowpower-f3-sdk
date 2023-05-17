/******************************************************************************

@file  bleapputil_api_legacy.h

@brief This file contains the bleapputil_api_legacy for use
with the CC13XX_CC26XX Bluetooth Low Energy Protocol Stack.

Group: WCS, BTS
$Target Device: DEVICES $

******************************************************************************
$License: BSD3 2022 $
******************************************************************************
$Release Name: PACKAGE NAME $
$Release Date: PACKAGE RELEASE DATE $
*****************************************************************************/

#ifndef BLEAPPUTIL_API_LEGACY_H
#define BLEAPPUTIL_API_LEGACY_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * TYPEDEFS
 */

/* Map for legacy devices */
#if !defined(DeviceFamily_CC26X1) && !defined(DeviceFamily_CC13X4) && !defined(DeviceFamily_CC23X0R5)

#define GAP_PERIPHERAL_REQUESTED_SECURITY_EVENT   GAP_SLAVE_REQUESTED_SECURITY_EVENT
#define gapPeripheralSecurityReqEvent_t           gapSlaveSecurityReqEvent_t

#endif


#ifdef __cplusplus
}
#endif

#endif /* BLEAPPUTIL_API_LEGACY_H */
