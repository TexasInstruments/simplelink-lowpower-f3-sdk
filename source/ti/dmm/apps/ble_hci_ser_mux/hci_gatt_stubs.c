/******************************************************************************
 * @file  hci_gatt_stubs.c
 *
 * @brief Stub implementations for GATT callback functions used by the MAC
 *        sensor layer. In HCI mode, GATT profiles are managed by the external
 *        host via HCI commands, so these callbacks are intentional no-ops.
 ******************************************************************************/

void MACGatt_syncSensorAttributes_util(void) {}
void PROVGatt_syncProvisioningAttributes_util(void) {}
