/******************************************************************************

 @file  tx_power.h

 @brief This file contains the TX Power service definitions and prototypes.

 Group: WCS, BTS
 $Target Device: DEVICES $

 ******************************************************************************
 $License: BSD3 2010 $
 ******************************************************************************
 $Release Name: PACKAGE NAME $
 $Release Date: PACKAGE RELEASE DATE $
 *****************************************************************************/

#ifndef TX_POWER_H
#define TX_POWER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Service UUID
#define TPL_SERV_UUID 0x1804

/************************************
 ***** Characteristic defines *******
 ************************************/

// Tx Power Level
#define TPLS_TX_POWER_LEVEL_ID   0
#define TPLS_TX_POWER_LEVEL_UUID 0x2A07

/************************************
 ********** AI Product Data *********
 ************************************/

#define TPLS_TX_POWER_LEVEL_LEN    1

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * Profile Callbacks
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * @fn      TPLS_addService
 *
 * @brief   This function initializes the TX Power Level service
 *          by registering GATT attributes with the GATT server.
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t TPLS_addService( void );

/*********************************************************************
 * @fn      TPLS_setParameter
 *
 * @brief   Set an TX Power Level Service parameter.
 *
 * @param   param - Characteristic UUID
 * @param   pValue - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 * @param   len - length of data to write
 *
 * @return  SUCCESS or error status
 */
bStatus_t TPLS_setParameter(uint8 param, void *pValue, uint16 len);

/*********************************************************************
 * @fn      TPLS_getParameter
 *
 * @brief   Get an TX Power Level parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  SUCCESS or error status
 */
bStatus_t TPLS_getParameter( uint8 param, void *pValue );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* TX_POWER_H */
