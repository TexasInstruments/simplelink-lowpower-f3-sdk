#ifndef ZB_LNT_PARAMS_H
#define ZB_LNT_PARAMS_H 1

#define ZB_SHORT_ADDR        (0xFFFF) // 0xFFFF is a special value telling the application to ignore
#define ZB_DENY_LIST         {}       // Empty array is a special value telling application to ignore
#define ZB_EXPECTED_NETWORK_SIZE (0)  // 0 is a special value telling application to ignore
#define ZB_ZCL_LNT_PAYLOAD_SIZE  (50) // Can either be 5, 50, or 200 bytes
#define ZB_METRICS_PRINT_INTERVAL_US  (10*60*1000000) // How often to print metrics from the coordinator
#define ZB_METRICS_REPORT_INTERVAL_US (10*60*1000000) // How often to report metrics to the coordinator
#define ZB_DATA_INTERVAL_US           (2*60*1000000)  // How often to send data of ZB_ZCL_LNT_PAYLOAD_SIZE to the coordinator

#endif /* ZB_LNT_PARAMS_H */
