#ifndef MAC_GATT_SENSOR_PROFILE_H
#define MAC_GATT_SENSOR_PROFILE_H

#include <stdint.h>
#include <stdbool.h>

// Service UUID
#define MACGATTSENSORPROFILE_SERV_UUID 0x1180

// Characteristic UUIDs
#define MACGATTSENSORPROFILE_SENSOR_DATA_CHAR_UUID 0x1181
#define MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_UUID 0x1182
#define MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR_UUID 0x1183 // New characteristic UUID

// Characteristic IDs
#define MACGATTSENSORPROFILE_SENSOR_DATA_CHAR 0  // Sensor Data characteristic
#define MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR 1  // Sensor Report Interval characteristic
#define MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR 2 // New characteristic ID

// Characteristic lengths
#define MACGATTSENSORPROFILE_SENSOR_DATA_CHAR_LEN 2
#define MACGATTSENSORPROFILE_SENSOR_REPORT_INTERVAL_CHAR_LEN 4
#define MACGATTSENSORPROFILE_IDENTIFY_REQUEST_CHAR_LEN 1 // New characteristic length

// Callback structure
typedef struct
{
  void (*pfnMacGattSensorProfileChange)(uint8_t paramID); // Callback for characteristic changes
} MacGattSensorProfile_CBs_t;

// Function prototypes
bStatus_t MacGattSensorProfile_addService(void);
bStatus_t MacGattSensorProfile_registerAppCBs(MacGattSensorProfile_CBs_t *appCallbacks);
bStatus_t MacGattSensorProfile_setParameter(uint8_t param, uint8_t len, void *value);
bStatus_t MacGattSensorProfile_getParameter(uint8_t param, void *value);

#endif // MAC_GATT_SENSOR_PROFILE_H
