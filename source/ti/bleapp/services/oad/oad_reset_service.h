/******************************************************************************

@file  oad_reset_service.h

 @brief This file contains the OAD profile GATT reset service, including
        service table, GATT callback and API.

Group: WCS, BTS
$Target Device: DEVICES $

******************************************************************************
$License: BSD3 2022 $
******************************************************************************
$Release Name: PACKAGE NAME $
$Release Date: PACKAGE RELEASE DATE $
*****************************************************************************/

#ifndef OAD_RESET_SERVICE_H
#define OAD_RESET_SERVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "hal_types.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*!
 * Stores information related to OAD reset write event
 */
typedef struct
{
  uint16 connHandle;
  uint8  cmd;
} oadResetWrite_t;

//CB from service to profile
typedef void (*oadServiceCB_t)(char* pData);
/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      OadReset_AddService
 *
 * @brief   Initializes the OAD reset service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   pfnOadServiceCB - Callback function from service
 *                            to profile after context switch.
 *
 * @return  SUCCESS or INVALIDPARAMETER
 */
extern bStatus_t OadReset_AddService(oadServiceCB_t pfnOadServiceCB);

/*********************************************************************
 * CONSTANTS
 */

/*!
 * 16-bit root of the OAD reset Service UUID.
 * \note the service and chars will use the 128-bit TI base UUID
 */
#define OAD_RESET_SERVICE_UUID          0xFFD0
/*!
 * 16-bit root of the OAD reset Characteristic UUID.
 */
#define OAD_RESET_CHAR_UUID             0xFFD1

/*!
 * Start OAD reset external control command
 * This command is used to tell the target
 * device that need to invalid self header
 * and start reboot
 */
#define OAD_RESET_CMD_START_OAD         0x01

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OAD_RESET_SERVICE_H */
