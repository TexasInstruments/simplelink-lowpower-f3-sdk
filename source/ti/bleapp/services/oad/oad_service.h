/******************************************************************************

 @file  oad_service.h

 @brief This file contains the OAD profile GATT service, including
        service table, GATT callback and API.

 Group: WCS, BTS
 $Target Device: DEVICES $

 ******************************************************************************
$License: BSD3 2022 $
 ******************************************************************************
 $Release Name: PACKAGE NAME $
 $Release Date: PACKAGE RELEASE DATE $
 *****************************************************************************/

#ifndef OAD_SERVICE_H
#define OAD_SERVICE_H

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
 * Event internal to the OAD module, use in state machine processing
 */
typedef enum
{
    OAD_SRV_IDENTIFY_REQ = 0, //!< Image identify received
    OAD_SRV_BLOCK_REQ    = 1, //!< Image block request/response
    OAD_SRV_CTRL_CMD     = 2, //!< External Control Command received
}oadServiceChar_e;

/*!
 * Number of characteristics in the OAD service
 */
#define OAD_CHAR_CNT                        3

/*!
 * Stores information related to OAD write request
 */
typedef struct
{
    oadServiceChar_e  serviceChar;  //!< char that write req occurred
    uint16  connHandle;             //!< Connection event was received on
    uint16  len;                    //!< Length of data received
    uint8  *pData;                  //!< Pointer to data received
} oadSrvWriteReq_t;

//CB from service to profile
typedef void (*oadServiceCB_t)(char* pData);

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      OAD_AddService
 *
 * @brief   Initializes the OAD service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   pfnOadServiceCB - Callback function from service
 *                            to profile after context switch.
 *
 * @return  SUCCESS, INVALIDPARAMETER or bleMemAllocError
 */
extern bStatus_t OAD_AddService(oadServiceCB_t pfnOadServiceCB);

/*********************************************************************
 * @fn      OADService_setParameter
 *
 * @brief   Set a OAD Characteristic parameter.
 *
 * @param   param - Service parameter ID
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
extern bStatus_t OADService_setParameter(oadServiceChar_e srvChar, uint8 len, void *value);

/*********************************************************************
 * @fn      OADService_setActiveConnHandle
 *
 * @brief   Set an active conncetion handel
 *
 * @param   connHandle - Active connection handle

 */
extern void OADService_setActiveConnHandle(uint16 connHandle);

/*********************************************************************
 * CONSTANTS
 */

/*!
 * This define controls whether the OAD profile requires BLE security
 * By default, this is off.
 */
#ifdef OAD_BLE_SECURITY
    #define OAD_WRITE_PERMIT     GATT_PERMIT_AUTHEN_WRITE
#else
    #define OAD_WRITE_PERMIT     GATT_PERMIT_WRITE
#endif //OAD_SECURITY

/*!
 * 16-bit root of the OAD Service UUID.
 * \note the service and chars will use the 128-bit TI base UUID
 */
#define OAD_SERVICE_UUID                    0xFFC0

/*!
 * 16-bit root of the OAD Image Identify Characteristic UUID.
 */
#define OAD_IMG_IDENTIFY_UUID               0xFFC1

/*!
 * 16-bit root of the OAD Image Block Characteristic UUID.
 */
#define OAD_IMG_BLOCK_UUID                  0xFFC2

/*!
 * 16-bit root of the OAD Extended Control Point Characteristic UUID.
 */
#define OAD_EXT_CTRL_UUID                   0xFFC5

/** @} End OAD_SVC_UUIDS */

/*!
 * @defgroup OAD_SVC_INDEXES OAD Service Indexes
 * @{
 */

/*!
 * Index of the image identify characteristic within the OAD service array
 */
//#define OAD_IDX_IMG_IDENTIFY                0

/*!
 * Index of the image block characteristic within the OAD service array
 */
//#define OAD_IDX_IMG_BLOCK                   1

/*!
 * Index of the extended control characteristic within the OAD service array
 */
//#define OAD_IDX_EXT_CTRL                    2

/** @} End OAD_SVC_INDEXES */



/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OAD_SERVICE_H */
