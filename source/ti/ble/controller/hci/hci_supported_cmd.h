/******************************************************************************
 @file:       hci_supported_cmd.h

 @brief:    This file contains definitions for the supported Bluetooth
      commands in the BLE Controller. It includes macros for each
      supported command and defines the supported command bytes
      based on the configuration.

      The supported commands are defined according to the Bluetooth
      Core Specification 5.4, Vol. 4, Part E, Section 6.27.

 Group: WCS, BTS
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2025, Texas Instruments Incorporated

 All rights reserved not granted herein.
 Limited License.

 Texas Instruments Incorporated grants a world-wide, royalty-free,
 non-exclusive license under copyrights and patents it now or hereafter
 owns or controls to make, have made, use, import, offer to sell and sell
 ("Utilize") this software subject to the terms herein. With respect to the
 foregoing patent license, such license is granted solely to the extent that
 any such patent is necessary to Utilize the software alone. The patent
 license shall not apply to any combinations which include this software,
 other than combinations with devices manufactured by or for TI ("TI
 Devices"). No hardware patent is licensed hereunder.

 Redistributions must preserve existing copyright notices and reproduce
 this license (including the above copyright notice and the disclaimer and
 (if applicable) source code license limitations below) in the documentation
 and/or other materials provided with the distribution.

 Redistribution and use in binary form, without modification, are permitted
 provided that the following conditions are met:

   * No reverse engineering, decompilation, or disassembly of this software
     is permitted with respect to any software provided in binary form.
   * Any redistribution and use are licensed by TI for use only with TI Devices.
   * Nothing shall obligate TI to provide you with source code for the software
     licensed and provided to you in object code.

 If software source code is provided to you, modification and redistribution
 of the source code are permitted provided that the following conditions are
 met:

   * Any redistribution and use of the source code, including any resulting
     derivative works, are licensed by TI for use only with TI Devices.
   * Any redistribution and use of any object code compiled from the source
     code and any resulting derivative works, are licensed by TI for use
     only with TI Devices.

 Neither the name of Texas Instruments Incorporated nor the names of its
 suppliers may be used to endorse or promote products derived from this
 software without specific prior written permission.

 DISCLAIMER.

 THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/
#ifndef HCI_SUPPORTED_CMD_H
#define HCI_SUPPORTED_CMD_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * DEFINES
 */
/************************************/

/*
** Bluetooth Supported Commands
** Core Specification, Vol. 2, Part E, Section 6.27
*/

// Local Supported Commands
#define SUPPORTED_COMMAND_LEN                                                 64

// No Command Supported in Byte
#define SUP_CMD_NONE                                                          0x00U

// Byte 0
#define SUP_CMD_INQUIRY                                                       0x01U
#define SUP_CMD_INQUIRE_CANCEL                                                0x02U
#define SUP_CMD_PRD_INQUIRY_MODE                                              0x04U
#define SUP_CMD_EXIT_PRD_INQUIRY_MODE                                         0x08U
#define SUP_CMD_CREATE_CONNECTION                                             0x10U
#define SUP_CMD_DISCONNECT                                                    0x20U
#define SUP_CMD_ADD_SCO_CONNECTION                                            0x40U  // deprecated
#define SUP_CMD_CREATE_CONN_CANCEL                                            0x80U
// Byte 1
#define SUP_CMD_ACCEPT_CONN_REQUEST                                           0x01U
#define SUP_CMD_REJECT_CONN_REQUEST                                           0x02U
#define SUP_CMD_LINK_KEY_REQ_REPLY                                            0x04U
#define SUP_CMD_LINK_KEY_REQ_NEG_REPLY                                        0x08U
#define SUP_CMD_PIN_CODE_REQ_REPLY                                            0x10U
#define SUP_CMD_PIN_CODE_REQ_NEG_REPLY                                        0x20U
#define SUP_CMD_CHANGE_CONN_PACKET_TYPE                                       0x40U
#define SUP_CMD_AUTHENTICATION_REQUESTED                                      0x80U
// Byte 2
#define SUP_CMD_SET_CONN_ENCRYPTION                                           0x01U
#define SUP_CMD_CHANGE_CONN_LINK_KEY                                          0x02U
#define SUP_CMD_CENTRAL_LINK_KEY                                              0x04U
#define SUP_CMD_REMOTE_NAME_REQUEST                                           0x08U
#define SUP_CMD_REMOTE_NAME_REQ_CANCEL                                        0x10U
#define SUP_CMD_RD_REMOTE_SUPPORTED_FEATURES                                  0x20U
#define SUP_CMD_RD_REMOTE_EXTENDED_FEATURES                                   0x40U
#define SUP_CMD_RD_REMOTE_VERSION_INFO                                        0x80U
// Byte 3
#define SUP_CMD_RD_CLOCK_OFFSET                                               0x01U
#define SUP_CMD_RD_LMP_HANDLE                                                 0x02U
#define SUP_CMD_RFU_BYTE03_BIT03                                              0x04U
#define SUP_CMD_RFU_BYTE03_BIT04                                              0x08U
#define SUP_CMD_RFU_BYTE03_BIT05                                              0x10U
#define SUP_CMD_RFU_BYTE03_BIT06                                              0x20U
#define SUP_CMD_RFU_BYTE03_BIT07                                              0x40U
#define SUP_CMD_RFU_BYTE03_BIT08                                              0x80U
// Byte 4
#define SUP_CMD_RFU_BYTE04_BIT01                                              0x01U
#define SUP_CMD_HOLD_MODE                                                     0x02U
#define SUP_CMD_SNIFF_MODE                                                    0x04U
#define SUP_CMD_EXIT_SNIFF_MODE                                               0x08U
#define SUP_CMD_PARK_STATE                                                    0x10U
#define SUP_CMD_EXIT_PARK_STATE                                               0x20U
#define SUP_CMD_QOS_SETUP                                                     0x40U
#define SUP_CMD_ROLE_DISCOVERY                                                0x80U
// Byte 5
#define SUP_CMD_SWITCH_ROLE                                                   0x01U
#define SUP_CMD_RD_LINK_POLICY_SETTINGS                                       0x02U
#define SUP_CMD_WR_LINK_POLICY_SETTINGS                                       0x04U
#define SUP_CMD_RD_DEFAULT_LINK_POLICY_SETTINGS                               0x08U
#define SUP_CMD_WR_DEFAULT_LINK_POLICY_SETTINGS                               0x10U
#define SUP_CMD_FLOW_SPECIFICATION                                            0x20U
#define SUP_CMD_SET_EVENT_MASK                                                0x40U
#define SUP_CMD_RESET                                                         0x80U
// Byte 6
#define SUP_CMD_SET_EVENT_FILTER                                              0x01U
#define SUP_CMD_FLUSH                                                         0x02U
#define SUP_CMD_RD_PIN_TYPE                                                   0x04U
#define SUP_CMD_WR_PIN_TYPE                                                   0x08U
#define SUP_CMD_CREATE_NEW_UNIT_KEY                                           0x10U
#define SUP_CMD_RD_STORED_LINK_KEY                                            0x20U
#define SUP_CMD_WR_STORED_LINK_KEY                                            0x40U
#define SUP_CMD_DELETE_STORED_LINK_KEY                                        0x80U
// Byte 7
#define SUP_CMD_WR_LOCAL_NAME                                                 0x01U
#define SUP_CMD_RD_LOCAL_NAME                                                 0x02U
#define SUP_CMD_RD_CONN_ACCEPT_TIME                                           0x04U
#define SUP_CMD_WR_CONN_ACCEPT_TIME                                           0x08U
#define SUP_CMD_RD_PAGE_TIMEOUT                                               0x10U
#define SUP_CMD_WR_PAGE_TIMEOUT                                               0x20U
#define SUP_CMD_RD_SCAN_ENABLE                                                0x40U
#define SUP_CMD_WR_SCAN_ENABLE                                                0x80U
// Byte 8
#define SUP_CMD_RD_PAGE_SCAN_ACTIVITY                                         0x01U
#define SUP_CMD_WR_PAGE_SCAN_ACTIVITY                                         0x02U
#define SUP_CMD_RD_INQUIRY_SCAN_ACTIVITY                                      0x04U
#define SUP_CMD_WR_INQUIRY_SCAN_ACTIVITY                                      0x08U
#define SUP_CMD_RD_AUTHENTICATION_ENABLE                                      0x10U
#define SUP_CMD_WR_AUTHENTICATION_ENABLE                                      0x20U
#define SUP_CMD_RD_ENCRYPTION_MODE                                            0x40U  // deprecated
#define SUP_CMD_WR_ENCRYPTION_MODE                                            0x80U  // deprecated
// Byte 9
#define SUP_CMD_RD_CLASS_OF_DEVICE                                            0x01U
#define SUP_CMD_WR_CLASS_OF_DEVICE                                            0x02U
#define SUP_CMD_RD_VOICE_SETTING                                              0x04U
#define SUP_CMD_WR_VOICE_SETTING                                              0x08U
#define SUP_CMD_RD_AUTOMATIC_FLUSH_TIMEOUT                                    0x10U
#define SUP_CMD_WR_AUTOMATIC_FLUSH_TIMEOUT                                    0x20U
#define SUP_CMD_RD_NUMBER_BROADCAST_RETRANSMISSIONS                           0x40U
#define SUP_CMD_WR_NUMBER_BROADCAST_RETRANSMISSIONS                           0x80U
// Byte 10
#define SUP_CMD_RD_HOLD_MODE_ACTIVITY                                         0x01U
#define SUP_CMD_WR_HOLD_MODE_ACTIVITY                                         0x02U
#define SUP_CMD_RD_TRANSMIT_POWER_LEVEL                                       0x04U
#define SUP_CMD_RD_SYNCHRONOUS_FLOW_CONTROL_ENABLE                            0x08U
#define SUP_CMD_WR_SYNCHRONOUS_FLOW_CONTROL_ENABLE                            0x10U
#define SUP_CMD_SET_CONTROLLER_TO_HOST_FLOW_CONTROL                           0x20U
#define SUP_CMD_HOST_BUFFER_SIZE                                              0x40U
#define SUP_CMD_HOST_NUMBER_OF_COMPLETED_PACKETS                              0x80U
// Byte 11
#define SUP_CMD_RD_LINK_SUPERVISION_TIMEOUT                                   0x01U
#define SUP_CMD_WR_LINK_SUPERVISION_TIMEOUT                                   0x02U
#define SUP_CMD_RD_NUMBER_OF_SUPPORTED_IAC                                    0x04U
#define SUP_CMD_RD_CURRENT_IAC_LAP                                            0x08U
#define SUP_CMD_WR_CURRENT_IAC_LAP                                            0x10U
#define SUP_CMD_RD_PAGE_SCAN_MODE_PERIOD                                      0x20U  // deprecated
#define SUP_CMD_WR_PAGE_SCAN_MODE_PERIOD                                      0x40U  // deprecated
#define SUP_CMD_RD_PAGE_SCAN_MODE                                             0x80U  // deprecated
// Byte 12
#define SUP_CMD_WR_PAGE_SCAN_MODE                                             0x01U  // deprecated
#define SUP_CMD_SET_AFH_HOST_CHANNEL_CLASSIFICATION                           0x02U
#define SUP_CMD_CS_RD_REMOTE_FAE_TABLE                                        0x04U
#define SUP_CMD_CS_WR_CACHED_REMOTE_FAE_TABLE                                 0x08U
#define SUP_CMD_RD_INQUIRY_SCAN_TYPE                                          0x10U
#define SUP_CMD_WR_INQUIRY_SCAN_TYPE                                          0x20U
#define SUP_CMD_RD_INQUIRY_MODE                                               0x40U
#define SUP_CMD_WR_INQUIRY_MODE                                               0x80U
// Byte 13
#define SUP_CMD_RD_PAGE_SCAN_TYPE                                             0x01U
#define SUP_CMD_WR_PAGE_SCAN_TYPE                                             0x02U
#define SUP_CMD_RD_AFH_CHANNEL_ASSESSMENT_MODE                                0x04U
#define SUP_CMD_WR_AFH_CHANNEL_ASSESSMENT_MODE                                0x08U
#define SUP_CMD_RFU_BYTE13_BIT05                                              0x10U
#define SUP_CMD_RFU_BYTE13_BIT06                                              0x20U
#define SUP_CMD_RFU_BYTE13_BIT07                                              0x40U
#define SUP_CMD_RFU_BYTE13_BIT08                                              0x80U
// Byte 14
#define SUP_CMD_RFU_BYTE14_BIT01                                              0x01U
#define SUP_CMD_RFU_BYTE14_BIT02                                              0x02U
#define SUP_CMD_RFU_BYTE14_BIT03                                              0x04U
#define SUP_CMD_RD_LOCAL_VERSION_INFORMATION                                  0x08U
#define SUP_CMD_RFU_BYTE14_BIT05                                              0x10U
#define SUP_CMD_RD_LOCAL_SUPPORTED_FEATURES                                   0x20U
#define SUP_CMD_RD_LOCAL_EXTENDED_FEATURES                                    0x40U
#define SUP_CMD_RD_BUFFER_SIZE                                                0x80U
// Byte 15
#define SUP_CMD_RD_COUNTRY_CODE                                               0x01U  // deprecated
#define SUP_CMD_RD_BDADDR                                                     0x02U
#define SUP_CMD_RD_FAILED_CONTACT_COUNTER                                     0x04U
#define SUP_CMD_RESET_FAILED_CONTACT_COUNTER                                  0x08U
#define SUP_CMD_RD_LINK_QUALITY                                               0x10U
#define SUP_CMD_RD_RSSI                                                       0x20U
#define SUP_CMD_RD_AFH_CHANNEL_MAP                                            0x40U
#define SUP_CMD_RD_CLOCK                                                      0x80U
// Byte 16
#define SUP_CMD_RD_LOOPBACK_MODE                                              0x01U
#define SUP_CMD_WR_LOOPBACK_MODE                                              0x02U
#define SUP_CMD_ENABLE_DEVICE_UNDER_TEST_MODE                                 0x04U
#define SUP_CMD_SETUP_SYNCHRONOUS_CONN_REQUEST                                0x08U
#define SUP_CMD_ACCEPT_SYNCHRONOUS_CONN_REQUEST                               0x10U
#define SUP_CMD_REJECT_SYNCHRONOUS_CONN_REQUEST                               0x20U
#define SUP_CMD_CS_CREATE_CONFIG                                              0x40U
#define SUP_CMD_CS_RM_CONFIG                                                  0x80U
// Byte 17
#define SUP_CMD_RD_EXTENDED_INQUIRY_RESPONSE                                  0x01U
#define SUP_CMD_WR_EXTENDED_INQUIRY_RESPONSE                                  0x02U
#define SUP_CMD_REFRESH_ENCRYPTION_KEY                                        0x04U
#define SUP_CMD_RFU_BYTE17_BIT04                                              0x08U
#define SUP_CMD_SNIFF_SUBRATING                                               0x10U
#define SUP_CMD_RD_SIMPLE_PAIRING_MODE                                        0x20U
#define SUP_CMD_WR_SIMPLE_PAIRING_MODE                                        0x40U
#define SUP_CMD_RD_LOCAL_OOB_DATA                                             0x80U
// Byte 18
#define SUP_CMD_RD_INQUIRY_RESPONSE_TRANSMIT_POWER_LEVEL                      0x01U
#define SUP_CMD_WR_INQUIRY_TRANSMIT_POWER_LEVEL                               0x02U
#define SUP_CMD_RD_DEFAULT_ERRONEOUS_DATA_REPORTING                           0x04U
#define SUP_CMD_WR_DEFAULT_ERRONEOUS_DATA_REPORTING                           0x08U
#define SUP_CMD_RFU_BYTE18_BIT05                                              0x10U
#define SUP_CMD_RFU_BYTE18_BIT06                                              0x20U
#define SUP_CMD_RFU_BYTE18_BIT07                                              0x40U
#define SUP_CMD_IO_CAPABILITY_REQ_REPLY                                       0x80U
// Byte 19
#define SUP_CMD_USER_CONFIRMATION_REQ_REPLY                                   0x01U
#define SUP_CMD_USER_CONFIRMATION_REQ_NEGATIVE_REPLY                          0x02U
#define SUP_CMD_USER_PASSKEY_REQ_REPLY                                        0x04U
#define SUP_CMD_USER_PASSKEY_REQ_NEGATIVE_REPLY                               0x08U
#define SUP_CMD_REMOTE_OOB_DATA_REQ_REPLY                                     0x10U
#define SUP_CMD_WR_SIMPLE_PAIRING_DEBUG_MODE                                  0x20U
#define SUP_CMD_ENHANCED_FLUSH                                                0x40U
#define SUP_CMD_REMOTE_OOB_DATA_REQ_NEGATIVE_REPLY                            0x80U
// Byte 20
#define SUP_CMD_RFU_BYTE20_BIT01                                              0x01U
#define SUP_CMD_RFU_BYTE20_BIT02                                              0x02U
#define SUP_CMD_SEND_KEYPRESS_NOTIFICATION                                    0x04U
#define SUP_CMD_IO_CAPABILITY_REQ_NEGATIVE_REPLY                              0x08U
#define SUP_CMD_RD_ENCRYPTION_KEY_SIZE                                        0x10U
#define SUP_CMD_CS_RD_LOCAL_SUPPORTED_CAPABILITIES                            0x20U
#define SUP_CMD_CS_RD_REMOTE_SUPPORTED_CAPABILITIES                           0x40U
#define SUP_CMD_CS_WR_CACHED_REMOTE_SUPPORTED_CAPABILITIES                    0x80U
// Byte 21
#define SUP_CMD_CREATE_PHYSICAL_LINK                                          0x01U
#define SUP_CMD_ACCEPT_PHYSICAL_LINK                                          0x02U
#define SUP_CMD_DISCONNECT_PHYSICAL_LINK                                      0x04U
#define SUP_CMD_CREATE_LOGICAL_LINK                                           0x08U
#define SUP_CMD_ACCEPT_LOGICAL_LINK                                           0x10U
#define SUP_CMD_DISCONNECT_LOGICAL_LINK                                       0x20U
#define SUP_CMD_LOGICAL_LINK_CANCEL                                           0x40U
#define SUP_CMD_FLOW_SPEC_MDOIFY                                              0x80U
// Byte 22
#define SUP_CMD_RD_LOGICAL_LINK_ACCEPT_TIMEOUT                                0x01U
#define SUP_CMD_WR_LOGICAL_LINK_ACCEPT_TIMEOUT                                0x02U
#define SUP_CMD_SET_EVENT_MASK_PAGE_2                                         0x04U
#define SUP_CMD_RD_LOCATION_DATA                                              0x08U
#define SUP_CMD_WR_LOCATION_DATA                                              0x10U
#define SUP_CMD_RD_LOCAL_AMP_INFO                                             0x20U
#define SUP_CMD_RD_LOCAL_AMP_ASSOC                                            0x40U
#define SUP_CMD_WR_LOCAL_AMP_ASSOC                                            0x80U
// Byte 23
#define SUP_CMD_RD_FLOW_CONTROL_MODE                                          0x01U
#define SUP_CMD_WR_FLOW_CONTROL_MODE                                          0x02U
#define SUP_CMD_RD_DATA_BLOCK_SIZE                                            0x04U
#define SUP_CMD_CS_TEST                                                       0x08U
#define SUP_CMD_CS_TEST_END                                                   0x10U
#define SUP_CMD_ENABLE_AMP_RECEIVER_REPORTS                                   0x20U
#define SUP_CMD_AMP_TEST_END                                                  0x40U
#define SUP_CMD_AMP_TEST                                                      0x80U
// Byte 24
#define SUP_CMD_RD_ENHANCED_TRANSMIT_POWER_LEVEL                              0x01U
#define SUP_CMD_CS_SECURITY_ENABLE                                            0x02U
#define SUP_CMD_RD_BEST_EFFORT_FLUSH_TIMEOUT                                  0x04U
#define SUP_CMD_WR_BEST_EFFORT_FLUSH_TIMEOUT                                  0x08U
#define SUP_CMD_SHORT_RANGE_MODE                                              0x10U
#define SUP_CMD_RD_LE_HOST_SUPPORT                                            0x20U
#define SUP_CMD_WR_LE_HOST_SUPPORT                                            0x40U
#define SUP_CMD_CS_SET_DEFAULT_SETTINGS                                       0x80U
// Byte 25
#define SUP_CMD_LE_SET_EVENT_MASK                                             0x01U
#define SUP_CMD_LE_RD_BUFFER_SIZE_V1                                          0x02U
#define SUP_CMD_LE_RD_LOCAL_SUPPORTED_FEATURES                                0x04U
#define SUP_CMD_RFU_BYTE25_BIT03                                              0x08U
#define SUP_CMD_LE_SET_RANDOM_ADDRESS                                         0x10U
#define SUP_CMD_LE_SET_ADV_PARAMETERS                                         0x20U
#define SUP_CMD_LE_RD_ADV_CHANNEL_TX_POWER                                    0x40U
#define SUP_CMD_LE_SET_ADV_DATA                                               0x80U
// Byte 26
#define SUP_CMD_LE_SET_SCAN_RESPONSE_DATA                                     0x01U
#define SUP_CMD_LE_SET_ADVERTISE_ENABLE                                       0x02U
#define SUP_CMD_LE_SET_SCAN_PARAMETERS                                        0x04U
#define SUP_CMD_LE_SET_SCAN_ENABLE                                            0x08U
#define SUP_CMD_LE_CREATE_CONNECTION                                          0x10U
#define SUP_CMD_LE_CREATE_CONN_CANCEL                                         0x20U
#define SUP_CMD_LE_RD_ACCEPT_LIST_SIZE                                        0x40U
#define SUP_CMD_LE_CLEAR_ACCEPT_LIST                                          0x80U
// Byte 27
#define SUP_CMD_LE_ADD_DEVICE_TO_ACCEPT_LIST                                  0x01U
#define SUP_CMD_LE_RM_DEVICE_FROM_ACCEPT_LIST                                 0x02U
#define SUP_CMD_LE_CONN_UPDATE                                                0x04U
#define SUP_CMD_LE_SET_HOST_CHANNEL_CLASSIFICATION                            0x08U
#define SUP_CMD_LE_RD_CHANNEL_MAP                                             0x10U
#define SUP_CMD_LE_RD_REMOTE_USED_FEATURES                                    0x20U
#define SUP_CMD_LE_ENCRYPT                                                    0x40U
#define SUP_CMD_LE_RAND                                                       0x80U
// Byte 28
#define SUP_CMD_LE_START_ENCRYPTION                                           0x01U
#define SUP_CMD_LE_LONG_TERM_KEY_REQ_REPLY                                    0x02U
#define SUP_CMD_LE_LONG_TERM_KEY_REQ_NEGATIVE_REPLY                           0x04U
#define SUP_CMD_LE_RD_SUPPORTED_STATES                                        0x08U
#define SUP_CMD_LE_RECEIVER_TEST_V1                                           0x10U
#define SUP_CMD_LE_TRANSMITTER_TEST_V1                                        0x20U
#define SUP_CMD_LE_TEST_END                                                   0x40U
#define SUP_CMD_RFU_BYTE28_BIT08                                              0x80U
// Byte 29
#define SUP_CMD_CS_SET_CHANNEL_CLASSIFICATION                                 0x01U
#define SUP_CMD_CS_SET_PROCEDURE_PARAMETERS                                   0x02U
#define SUP_CMD_CS_PROCEDURE_ENABLE                                           0x04U
#define SUP_CMD_ENHANCED_SETUP_SYNCHRONOUS_CONNECTION                         0x08U
#define SUP_CMD_ENHANCED_ACCEPT_SYNCHRONOUS_CONNECTION                        0x10U
#define SUP_CMD_RD_LOCAL_SUPPORTED_CODECS                                     0x20U
#define SUP_CMD_SET_MWS_CHANNEL_PARAMETERS_COMMAND                            0x40U
#define SUP_CMD_SET_EXTERNAL_FRAME_CONFIGURATION_COMMAND                      0x80U
// Byte 30
#define SUP_CMD_SET_MWS_SIGNALING_COMMAND                                     0x01U
#define SUP_CMD_SET_TRANSPORT_LAYER_COMMAND                                   0x02U
#define SUP_CMD_SET_MWS_SCAN_FREQUENCY_TABLE_COMMAND                          0x04U
#define SUP_CMD_GET_TRANSPORT_LAYER_CONFIGURATION_COMMAND                     0x08U
#define SUP_CMD_SET_MWS_PATTERN_CONFIGURATION_COMMAND                         0x10U
#define SUP_CMD_SET_TRIGGERED_CLOCK_CAPTURE                                   0x20U
#define SUP_CMD_TRUNCATED_PAGE                                                0x40U
#define SUP_CMD_TRUNCATED_PAGE_CANCEL                                         0x80U
// Byte 31
#define SUP_CMD_SET_CONNLESS_SLAVE_BR                                         0x01U
#define SUP_CMD_SET_CONNLESS_SLAVE_BR_RECEIVE                                 0x02U
#define SUP_CMD_START_SYNCHRONIZATION_TRAIN                                   0x04U
#define SUP_CMD_RECEIVE_SYNCHRONIZATION_TRAIN                                 0x08U
#define SUP_CMD_SET_RFU_LT_ADDR                                               0x10U
#define SUP_CMD_DELETE_RFU_LT_ADDR                                            0x20U
#define SUP_CMD_SET_CONNLESS_SLAVE_BR_DATA                                    0x40U
#define SUP_CMD_RD_SYNCHRONIZATION_TRAIN_PARAMETERS                           0x80U
// Byte 32
#define SUP_CMD_WR_SYNCHRONIZATION_TRAIN_PARAMETERS                           0x01U
#define SUP_CMD_REMOTE_OOB_EXTENDED_DATA_REQ_REPLY                            0x02U
#define SUP_CMD_RD_SECURE_CONNECTIONS_HOST_SUPPORT                            0x04U
#define SUP_CMD_WR_SECURE_CONNECTIONS_HOST_SUPPORT                            0x08U
#define SUP_CMD_RD_AUTHENTICATED_PAYLOAD_TIMEOUT                              0x10U
#define SUP_CMD_WR_AUTHENTICATED_PAYLOAD_TIMEOUT                              0x20U
#define SUP_CMD_RD_LOCAL_OOB_EXTENDED_DATA                                    0x40U
#define SUP_CMD_WR_SECURE_CONNECTIONS_TEST_MODE                               0x80U
// Byte 33
#define SUP_CMD_RD_EXTENDED_PAGE_TIMEOUT                                      0x01U
#define SUP_CMD_WR_EXTENDED_PAGE_TIMEOUT                                      0x02U
#define SUP_CMD_RD_EXTENDED_INQUIRY_LENGTH                                    0x04U
#define SUP_CMD_WR_EXTENDED_INQUIRY_LENGTH                                    0x08U
#define SUP_CMD_LE_REMOTE_CONN_PARAM_REPLY_COMMAND                            0x10U
#define SUP_CMD_LE_REMOTE_CONN_PARAM_NEGATIVE_REPLY_COMMAND                   0x20U
#define SUP_CMD_SET_DATA_LENGTH                                               0x40U
#define SUP_CMD_RD_SUGGESTED_DEFAULT_DATA_LENGTH                              0x80U
// Byte 34
#define SUP_CMD_LE_WR_SUGGESTED_DEFAULT_DATA_LENGTH                           0x01U
#define SUP_CMD_LE_RD_LOCAL_P256_PUBLIC_KEY                                   0x02U
#define SUP_CMD_LE_GENERATE_DH_KEY_V1                                         0x04U
#define SUP_CMD_LE_ADD_DEVICE_TO_RESOLVING_LIST                               0x08U
#define SUP_CMD_LE_RM_DEVICE_FROM_RESOLVING_LIST                              0x10U
#define SUP_CMD_LE_CLEAR_RESOLVING_LIST                                       0x20U
#define SUP_CMD_LE_RD_RESOLVING_LIST                                          0x40U
#define SUP_CMD_LE_RD_PEER_RESOLVABLE_ADDRESS                                 0x80U
// Byte 35
#define SUP_CMD_LE_RD_LOCAL_RESOLVABLE_ADDRESS                                0x01U
#define SUP_CMD_LE_SET_ADDRESS_RESOLUTION_ENABLE                              0x02U
#define SUP_CMD_LE_SET_RESOLVABLE_PRIVATE_ADDRESS_TIMEOUT                     0x04U
#define SUP_CMD_LE_RD_MAXIMUM_DATA_LENGTH                                     0x08U
#define SUP_CMD_LE_RD_PHY                                                     0x10U
#define SUP_CMD_LE_SET_DEFAULT_PHY                                            0x20U
#define SUP_CMD_LE_SET_PHY                                                    0x40U
#define SUP_CMD_LE_RECEIVER_TEST_V2                                           0x80U
// Byte 36
#define SUP_CMD_LE_TRANSMITTER_TEST_V2                                        0x01U
#define SUP_CMD_LE_SET_ADV_SET_RANDOM_ADDRESS                                 0x02U
#define SUP_CMD_LE_SET_EXTENDED_ADV_PARAMETERS                                0x04U
#define SUP_CMD_LE_SET_EXTENDED_ADV_DATA                                      0x08U
#define SUP_CMD_LE_SET_EXTENDED_SCAN_RESPONSE_DATA                            0x10U
#define SUP_CMD_LE_SET_EXTENDED_ADV_ENABLE                                    0x20U
#define SUP_CMD_LE_RD_MAXIMUM_ADV_DATA_LENGTH                                 0x40U
#define SUP_CMD_LE_RD_NUMBER_OF_SUPPORTED_ADV_SETS                            0x80U
// Byte 37
#define SUP_CMD_LE_RM_ADV_SET                                                 0x01U
#define SUP_CMD_LE_CLEAR_ADV_SETS                                             0x02U
#define SUP_CMD_LE_SET_PRD_ADV_PARAMETERS                                     0x04U
#define SUP_CMD_LE_SET_PRD_ADV_DATA                                           0x08U
#define SUP_CMD_LE_SET_PRD_ADV_ENABLE                                         0x10U
#define SUP_CMD_LE_SET_EXTENDED_SCAN_PARAMETERS                               0x20U
#define SUP_CMD_LE_SET_EXTENDED_SCAN_EANBLE                                   0x40U
#define SUP_CMD_LE_EXTENDED_CREATE_CONN_COMMAND                               0x80U
// Byte 38
#define SUP_CMD_LE_PRD_ADV_CREATE_SYNC                                        0x01U
#define SUP_CMD_LE_PRD_ADV_CREATE_SYNC_CANCEL                                 0x02U
#define SUP_CMD_LE_PRD_ADV_TERMINATE_SYNC                                     0x04U
#define SUP_CMD_LE_ADD_DEVICE_TO_PRD_ADVERTISER_LIST                          0x08U
#define SUP_CMD_LE_RM_DEVICE_FROM_PRD_ADVERTISER_LIST                         0x10U
#define SUP_CMD_LE_CLEAR_PRD_ADVERTISER_LIST                                  0x20U
#define SUP_CMD_LE_RD_PRD_ADVERTISER_LIST_SIZE                                0x40U
#define SUP_CMD_LE_RD_TRANSMIT_POWER                                          0x80U
// Byte 39
#define SUP_CMD_LE_RD_RF_PATH_COMPENSATION                                    0x01U
#define SUP_CMD_LE_WR_RF_PATH_COMPENSATION                                    0x02U
#define SUP_CMD_LE_SET_PRIVACY_MODE                                           0x04U
#define SUP_CMD_LE_RECEIVER_TEST_V3                                           0x08U
#define SUP_CMD_LE_TRANSMITTER_TEST_V3                                        0x10U
#define SUP_CMD_LE_SET_CONNLESS_CTE_PARAMS_TX                                 0x20U
#define SUP_CMD_LE_SET_CONNLESS_CTE_ENABLE_TX                                 0x40U
#define SUP_CMD_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE                            0x80U
// Byte 40
#define SUP_CMD_LE_SET_CONN_CTE_RECEIVE_PARAMS                                0x01U
#define SUP_CMD_LE_SET_CONN_CTE_TRANSMIT_PARAMS                               0x02U
#define SUP_CMD_LE_SET_CONN_CTE_REQ_ENABLE                                    0x04U
#define SUP_CMD_LE_SET_CONN_CTE_RESPONSE_ENABLE                               0x08U
#define SUP_CMD_LE_RD_ANTENNA_INFORMATION                                     0x10U
#define SUP_CMD_LE_SET_PRD_ADV_RECEIVE_ENABLE                                 0x20U
#define SUP_CMD_RFU_BYTE40_BIT07                                              0x40U
#define SUP_CMD_RFU_BYTE40_BIT08                                              0x80U
// Byte 41
#define SUP_CMD_LE_SET_PRD_ADV_SYNC_TRANSFER_PARAMETERS                       0x01U
#define SUP_CMD_LE_SET_DEFAULT_PRD_ADV_SYNC_TRANSFER_PARAMETERS               0x02U
#define SUP_CMD_LE_SET_GENERATE_DHKEY_V2                                      0x04U
#define SUP_CMD_RFU_BYTE41_BIT04                                              0x08U
#define SUP_CMD_RFU_BYTE41_BIT05                                              0x10U
#define SUP_CMD_RFU_BYTE41_BIT06                                              0x20U
#define SUP_CMD_RFU_BYTE41_BIT07                                              0x40U
#define SUP_CMD_RFU_BYTE41_BIT08                                              0x80U
// Byte 42 - we are not supporting any commands here yet
// Byte 43 - we are not supporting any commands here yet
// Byte 44
#define SUP_CMD_RFU_BYTE44_BIT01                                              0x01U
#define SUP_CMD_LE_SET_HOST_FEATURE                                           0x02U
#define SUP_CMD_RFU_BYTE44_BIT03                                              0x04U
#define SUP_CMD_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL                         0x08U
#define SUP_CMD_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL                           0x10U
#define SUP_CMD_RFU_BYTE44_BIT06                                              0x20U
#define SUP_CMD_RFU_BYTE44_BIT07                                              0x40U
#define SUP_CMD_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE                        0x80U

// Byte 45-63 will define if we support commands in these bytes.

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_0  //
///////////////////////////////

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & INIT_CFG)
  #define SUPPORTED_COMMAND_BYTE_0                   (SUP_CMD_DISCONNECT)
#else  // !INIT_CFG
  #define SUPPORTED_COMMAND_BYTE_0                   (SUP_CMD_NONE)
#endif  // INIT_CFG

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_1  //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_1                     (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_2  //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_2                     (SUP_CMD_RD_REMOTE_VERSION_INFO)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_3  //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_3                     (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_4  //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_4                     (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_5  //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_5                     (SUP_CMD_RESET          | \
                                                      SUP_CMD_SET_EVENT_MASK)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_6  //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_6                     (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_7  //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_7                     (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_8  //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_8                     (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_9  //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_9                     (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_10 //
///////////////////////////////


#if defined(CTRL_CONFIG) && (CTRL_CONFIG & (ADV_CONN_CFG | INIT_CFG))
  #define SUPPORTED_COMMAND_BYTE_10                  (SUP_CMD_HOST_NUMBER_OF_COMPLETED_PACKETS    |     \
                                                      SUP_CMD_HOST_BUFFER_SIZE                    |     \
                                                      SUP_CMD_SET_CONTROLLER_TO_HOST_FLOW_CONTROL |     \
                                                      SUP_CMD_RD_TRANSMIT_POWER_LEVEL )

#else  // !ADV_CONN_CFG && !INIT_CFG
  #define SUPPORTED_COMMAND_BYTE_10                  (SUP_CMD_HOST_NUMBER_OF_COMPLETED_PACKETS |        \
                                                      SUP_CMD_HOST_BUFFER_SIZE                 |        \
                                                      SUP_CMD_SET_CONTROLLER_TO_HOST_FLOW_CONTROL)
#endif  // ADV_CONN_CFG | INIT_CFG

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_11 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_11                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_12 //
///////////////////////////////
#ifdef CHANNEL_SOUNDING
#define SUPPORTED_COMMAND_BYTE_12                    (SUP_CMD_CS_RD_REMOTE_FAE_TABLE)
#else
#define SUPPORTED_COMMAND_BYTE_12                    (SUP_CMD_NONE)
#endif

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_13 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_13                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_14 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_14                    (SUP_CMD_RD_LOCAL_SUPPORTED_FEATURES | \
                                                      SUP_CMD_RD_LOCAL_VERSION_INFORMATION)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_15 //
///////////////////////////////

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & (ADV_CONN_CFG | INIT_CFG))
  #define SUPPORTED_COMMAND_BYTE_15                  (SUP_CMD_RD_RSSI | \
                                                      SUP_CMD_RD_BDADDR)
#else  // !ADV_CONN_CFG && !INIT_CFG
  #define SUPPORTED_COMMAND_BYTE_15                  (SUP_CMD_RD_BDADDR)
#endif  // ADV_CONN_CFG | INIT_CFG

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_16 //
///////////////////////////////

#ifdef CHANNEL_SOUNDING
#define SUPPORTED_COMMAND_BYTE_16                    (SUP_CMD_CS_CREATE_CONFIG | \
                                                      SUP_CMD_CS_RM_CONFIG)
#else
#define SUPPORTED_COMMAND_BYTE_16                    (SUP_CMD_NONE)
#endif

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_17 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_17                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_18 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_18                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_19 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_19                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_20 //
///////////////////////////////

#ifdef CHANNEL_SOUNDING
#define SUPPORTED_COMMAND_BYTE_20                    (SUP_CMD_CS_RD_REMOTE_SUPPORTED_CAPABILITIES |\
                                                      SUP_CMD_CS_RD_LOCAL_SUPPORTED_CAPABILITIES)
#else
#define SUPPORTED_COMMAND_BYTE_20                    (SUP_CMD_NONE)
#endif
///////////////////////////////
// SUPPORTED_COMMAND_BYTE_21 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_21                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_22 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_22                    (SUP_CMD_SET_EVENT_MASK_PAGE_2)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_23 //
///////////////////////////////

#if defined(CHANNEL_SOUNDING) && defined(CS_TEST)
#define SUPPORTED_COMMAND_BYTE_23                    (SUP_CMD_CS_TEST_END | \
                                                      SUP_CMD_CS_TEST)
#else
#define SUPPORTED_COMMAND_BYTE_23                    (SUP_CMD_NONE)
#endif

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_24 //
///////////////////////////////
#ifdef CHANNEL_SOUNDING
#define SUPPORTED_COMMAND_BYTE_24                    (SUP_CMD_CS_SECURITY_ENABLE | \
                                                      SUP_CMD_CS_SET_DEFAULT_SETTINGS)
#else
#define SUPPORTED_COMMAND_BYTE_24                    (SUP_CMD_NONE)
#endif

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_25 //
///////////////////////////////

#define BYTE_25_COMMON                               (SUP_CMD_LE_SET_RANDOM_ADDRESS          | \
                                                      SUP_CMD_LE_RD_LOCAL_SUPPORTED_FEATURES | \
                                                      SUP_CMD_LE_RD_BUFFER_SIZE_V1           | \
                                                      SUP_CMD_LE_SET_EVENT_MASK)

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & (ADV_NCONN_CFG | ADV_CONN_CFG))
  #define BYTE_25_ADV                                (SUP_CMD_LE_SET_ADV_DATA            | \
                                                      SUP_CMD_LE_RD_ADV_CHANNEL_TX_POWER | \
                                                      SUP_CMD_LE_SET_ADV_PARAMETERS)
#else  // !defined(CTRL_CONFIG) && (CTRL_CONFIG & (ADV_NCONN_CFG | ADV_CONN_CFG))
  #define BYTE_25_ADV                                (SUP_CMD_NONE)
#endif  // defined(CTRL_CONFIG) && (CTRL_CONFIG & (ADV_NCONN_CFG | ADV_CONN_CFG))

#define SUPPORTED_COMMAND_BYTE_25                    (BYTE_25_COMMON |  \
                                                      BYTE_25_ADV)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_26 //
///////////////////////////////

#define BYTE_26_COMMON                               (SUP_CMD_LE_CLEAR_ACCEPT_LIST   | \
                                                      SUP_CMD_LE_RD_ACCEPT_LIST_SIZE)

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & (ADV_NCONN_CFG | ADV_CONN_CFG))
  #define BYTE_26_ADV                                (SUP_CMD_LE_SET_ADVERTISE_ENABLE | \
                                                      SUP_CMD_LE_SET_SCAN_RESPONSE_DATA)
#else  // !defined(CTRL_CONFIG) && (CTRL_CONFIG & (ADV_NCONN_CFG | ADV_CONN_CFG))
  #define BYTE_26_ADV                                (SUP_CMD_NONE)
#endif  // defined(CTRL_CONFIG) && (CTRL_CONFIG & (ADV_NCONN_CFG | ADV_CONN_CFG))

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & SCAN_CFG)
  #define BYTE_26_SCAN                               (SUP_CMD_LE_SET_SCAN_ENABLE | \
                                                      SUP_CMD_LE_SET_SCAN_PARAMETERS)
#else  // !SCAN_CFG
  #define BYTE_26_SCAN                               (SUP_CMD_NONE)
#endif  // SCAN_CFG

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & INIT_CFG)
  #define BYTE_26_INIT                               (SUP_CMD_LE_CREATE_CONN_CANCEL |  \
                                                      SUP_CMD_LE_CREATE_CONNECTION)
#else  //!INIT_CFG

  #define BYTE_26_INIT                               (SUP_CMD_NONE)

#endif  // INIT_CFG

#define SUPPORTED_COMMAND_BYTE_26                    (BYTE_26_COMMON |  \
                                                      BYTE_26_ADV    |  \
                                                      BYTE_26_SCAN   |  \
                                                      BYTE_26_INIT)
///////////////////////////////
// SUPPORTED_COMMAND_BYTE_27 //
///////////////////////////////

#define BYTE_27_COMMON                               (SUP_CMD_LE_RAND                       |        \
                                                      SUP_CMD_LE_RD_REMOTE_USED_FEATURES    |        \
                                                      SUP_CMD_LE_RM_DEVICE_FROM_ACCEPT_LIST |        \
                                                      SUP_CMD_LE_ADD_DEVICE_TO_ACCEPT_LIST)

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & ADV_CONN_CFG)
  #define BYTE_27_ADV                                (SUP_CMD_LE_RD_CHANNEL_MAP             |        \
                                                      SUP_CMD_LE_CONN_UPDATE                |        \
                                                      SUP_CMD_LE_RD_REMOTE_USED_FEATURES)
#else  // ADV_CONN_CFG
  #define BYTE_27_ADV                                (SUP_CMD_NONE)
#endif  // ADV_NCONN_CFG | ADV_CONN_CFG

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & INIT_CFG)
 #define BYTE_27_INIT                                (SUP_CMD_LE_ENCRYPT                         |      \
                                                      SUP_CMD_LE_RD_CHANNEL_MAP                  |      \
                                                      SUP_CMD_LE_SET_HOST_CHANNEL_CLASSIFICATION |      \
                                                      SUP_CMD_LE_CONN_UPDATE)
#else  //!INIT_CFG

  #define BYTE_27_INIT                               (SUP_CMD_NONE)
#endif  // INIT_CFG

#define SUPPORTED_COMMAND_BYTE_27                    (BYTE_27_COMMON |                                        \
                                                      BYTE_27_ADV    |                                        \
                                                      BYTE_27_INIT)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_28 //
///////////////////////////////

#define BYTE_28_COMMON                               (SUP_CMD_LE_TEST_END            |                     \
                                                      SUP_CMD_LE_TRANSMITTER_TEST_V1 |                     \
                                                      SUP_CMD_LE_RECEIVER_TEST_V1    |                     \
                                                      SUP_CMD_LE_RD_SUPPORTED_STATES)

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & ADV_CONN_CFG)
  #define BYTE_28_ADV                                (SUP_CMD_LE_LONG_TERM_KEY_REQ_NEGATIVE_REPLY | \
                                                      SUP_CMD_LE_LONG_TERM_KEY_REQ_REPLY)
#else  // ADV_CONN_CFG
  #define BYTE_28_ADV                                (SUP_CMD_NONE)
#endif  // ADV_NCONN_CFG | ADV_CONN_CFG

#if defined(CTRL_CONFIG) && (CTRL_CONFIG & INIT_CFG)
  #define BYTE_28_INIT                               (SUP_CMD_LE_START_ENCRYPTION)
#else  //!INIT_CFG
  #define BYTE_28_INIT                               (SUP_CMD_NONE)
#endif  // INIT_CFG

#define SUPPORTED_COMMAND_BYTE_28                    (BYTE_28_COMMON |                                        \
                                                      BYTE_28_ADV    |                                        \
                                                      BYTE_28_INIT)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_29 //
///////////////////////////////

#ifdef CHANNEL_SOUNDING
#define SUPPORTED_COMMAND_BYTE_29                    (SUP_CMD_CS_SET_CHANNEL_CLASSIFICATION | \
                                                      SUP_CMD_CS_SET_PROCEDURE_PARAMETERS   | \
                                                      SUP_CMD_CS_PROCEDURE_ENABLE )
#else
#define SUPPORTED_COMMAND_BYTE_29                    (SUP_CMD_NONE)
#endif

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_30 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_30                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_31 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_31                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_32 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_32                    (SUP_CMD_WR_AUTHENTICATED_PAYLOAD_TIMEOUT |     \
                                                      SUP_CMD_RD_AUTHENTICATED_PAYLOAD_TIMEOUT)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_33 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_33                    (SUP_CMD_LE_REMOTE_CONN_PARAM_NEGATIVE_REPLY_COMMAND |  \
                                                      SUP_CMD_LE_REMOTE_CONN_PARAM_REPLY_COMMAND          |  \
                                                      SUP_CMD_SET_DATA_LENGTH                             |  \
                                                      SUP_CMD_RD_SUGGESTED_DEFAULT_DATA_LENGTH)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_34 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_34                    (SUP_CMD_LE_WR_SUGGESTED_DEFAULT_DATA_LENGTH | \
                                                      SUP_CMD_LE_RD_LOCAL_P256_PUBLIC_KEY         | \
                                                      SUP_CMD_LE_GENERATE_DH_KEY_V1               | \
                                                      SUP_CMD_LE_ADD_DEVICE_TO_RESOLVING_LIST     | \
                                                      SUP_CMD_LE_RM_DEVICE_FROM_RESOLVING_LIST    | \
                                                      SUP_CMD_LE_CLEAR_RESOLVING_LIST             | \
                                                      SUP_CMD_LE_RD_RESOLVING_LIST                | \
                                                      SUP_CMD_LE_RD_PEER_RESOLVABLE_ADDRESS)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_35 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_35                    (SUP_CMD_LE_RD_LOCAL_RESOLVABLE_ADDRESS            | \
                                                      SUP_CMD_LE_SET_ADDRESS_RESOLUTION_ENABLE          | \
                                                      SUP_CMD_LE_SET_RESOLVABLE_PRIVATE_ADDRESS_TIMEOUT | \
                                                      SUP_CMD_LE_RD_MAXIMUM_DATA_LENGTH                 | \
                                                      SUP_CMD_LE_RD_PHY                                 | \
                                                      SUP_CMD_LE_SET_DEFAULT_PHY                        | \
                                                      SUP_CMD_LE_SET_PHY                                | \
                                                      SUP_CMD_LE_RECEIVER_TEST_V2)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_36 //
///////////////////////////////
#define BYTE_36_COMMON                               (SUP_CMD_LE_TRANSMITTER_TEST_V2                    | \
                                                      SUP_CMD_LE_RD_MAXIMUM_ADV_DATA_LENGTH             | \
                                                      SUP_CMD_LE_RD_NUMBER_OF_SUPPORTED_ADV_SETS)
#if defined(USE_AE)
  #define BYTE_36_EXT_ADV                            (SUP_CMD_LE_SET_ADV_SET_RANDOM_ADDRESS            | \
                                                      SUP_CMD_LE_SET_EXTENDED_ADV_PARAMETERS           | \
                                                      SUP_CMD_LE_SET_EXTENDED_ADV_DATA                 | \
                                                      SUP_CMD_LE_SET_EXTENDED_SCAN_RESPONSE_DATA       | \
                                                      SUP_CMD_LE_SET_EXTENDED_ADV_ENABLE)
#else  //!defined(USE_AE)
    #define BYTE_36_EXT_ADV                          (SUP_CMD_NONE)
#endif  //defined(USE_AE)

#define SUPPORTED_COMMAND_BYTE_36                    (BYTE_36_COMMON | \
                                                      BYTE_36_EXT_ADV)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_37 //
///////////////////////////////

#define BYTE_37_COMMON                               (SUP_CMD_LE_RM_ADV_SET  | \
                                                      SUP_CMD_LE_CLEAR_ADV_SETS)
#if defined(USE_AE)
  #define BYTE_37_EXT_ADV                            (SUP_CMD_LE_SET_EXTENDED_SCAN_PARAMETERS  | \
                                                      SUP_CMD_LE_SET_EXTENDED_SCAN_EANBLE      | \
                                                      SUP_CMD_LE_EXTENDED_CREATE_CONN_COMMAND)
#else  //!defined(USE_AE)
  #define BYTE_37_EXT_ADV                            (SUP_CMD_NONE)
#endif  //defined(USE_AE)

#if defined(USE_PERIODIC_ADV) && defined(USE_AE)
  #define BYTE_37_PRD_ADV                            (SUP_CMD_LE_SET_PRD_ADV_PARAMETERS  | \
                                                      SUP_CMD_LE_SET_PRD_ADV_DATA        | \
                                                      SUP_CMD_LE_SET_PRD_ADV_ENABLE)
#else  //!defined(USE_PERIODIC_ADV) && defined(USE_AE)
  #define BYTE_37_PRD_ADV                       (SUP_CMD_NONE)
#endif  //defined(USE_PERIODIC_ADV) && defined(USE_AE)

#define SUPPORTED_COMMAND_BYTE_37                    (BYTE_37_COMMON     |                                        \
                                                      BYTE_37_EXT_ADV    |                                        \
                                                      BYTE_37_PRD_ADV)
///////////////////////////////
// SUPPORTED_COMMAND_BYTE_38 //
///////////////////////////////

#define BYTE_38_COMMON                               (SUP_CMD_LE_RD_TRANSMIT_POWER)

#if defined(USE_PERIODIC_SCAN) && defined(USE_AE)
  #define BYTE_38_PRD_ADV                            (SUP_CMD_LE_PRD_ADV_CREATE_SYNC                    | \
                                                      SUP_CMD_LE_PRD_ADV_CREATE_SYNC_CANCEL             | \
                                                      SUP_CMD_LE_PRD_ADV_TERMINATE_SYNC                 | \
                                                      SUP_CMD_LE_ADD_DEVICE_TO_PRD_ADVERTISER_LIST      | \
                                                      SUP_CMD_LE_RM_DEVICE_FROM_PRD_ADVERTISER_LIST     | \
                                                      SUP_CMD_LE_CLEAR_PRD_ADVERTISER_LIST              | \
                                                      SUP_CMD_LE_RD_PRD_ADVERTISER_LIST_SIZE)
#else  //!defined(USE_PERIODIC_SCAN) && defined(USE_AE)
  #define BYTE_38_PRD_ADV                       (SUP_CMD_NONE)
#endif  //defined(USE_PERIODIC_SCAN) && defined(USE_AE)

#define SUPPORTED_COMMAND_BYTE_38                    (BYTE_38_COMMON     |                                 \
                                                      BYTE_38_PRD_ADV)
///////////////////////////////
// SUPPORTED_COMMAND_BYTE_39 //
///////////////////////////////

#define BYTE_39_COMMON                               (SUP_CMD_LE_RD_RF_PATH_COMPENSATION                  | \
                                                      SUP_CMD_LE_WR_RF_PATH_COMPENSATION                  | \
                                                      SUP_CMD_LE_SET_PRIVACY_MODE                         | \
                                                      SUP_CMD_LE_RECEIVER_TEST_V3                         | \
                                                      SUP_CMD_LE_TRANSMITTER_TEST_V3)
#define BYTE_39_RTLS_CTE_TX                          (SUP_CMD_NONE)

#define BYTE_39_RTLS_CTE_RX                          (SUP_CMD_NONE)

#define SUPPORTED_COMMAND_BYTE_39                    (BYTE_39_COMMON      | \
                                                      BYTE_39_RTLS_CTE_TX | \
                                                      BYTE_39_RTLS_CTE_RX)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_40 //
///////////////////////////////

#if defined(USE_PERIODIC_SCAN) && defined(USE_AE)
  #define BYTE_40_PRD_ADV                       (SUP_CMD_LE_SET_PRD_ADV_RECEIVE_ENABLE)
#else  //!defined(USE_PERIODIC_SCAN) && defined(USE_AE)
  #define BYTE_40_PRD_ADV                       (SUP_CMD_NONE)
#endif  //defined(USE_PERIODIC_SCAN) && defined(USE_AE)


#define BYTE_40_RTLS_CTE                            (SUP_CMD_NONE)

#define SUPPORTED_COMMAND_BYTE_40                    (BYTE_40_PRD_ADV     |                                   \
                                                      BYTE_40_RTLS_CTE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_41 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_41                    (SUP_CMD_LE_SET_GENERATE_DHKEY_V2)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_42 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_42                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_43 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_43                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_44 //
///////////////////////////////

#if defined(POWER_CONTROL)
  #define SUPPORTED_COMMAND_BYTE_44                   (SUP_CMD_LE_SET_HOST_FEATURE                    | \
                                                       SUP_CMD_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL  | \
                                                       SUP_CMD_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL    | \
                                                       SUP_CMD_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE)
#else
  #define SUPPORTED_COMMAND_BYTE_44                  (SUP_CMD_LE_SET_HOST_FEATURE)
#endif

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_45 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_45                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_46 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_46                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_47 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_47                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_48 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_48                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_49 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_49                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_50 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_50                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_51 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_51                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_52 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_52                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_53 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_53                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_54 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_54                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_55 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_55                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_56 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_56                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_57 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_57                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_58 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_58                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_59 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_59                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_60 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_60                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_61 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_61                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_62 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_62                    (SUP_CMD_NONE)

///////////////////////////////
// SUPPORTED_COMMAND_BYTE_63 //
///////////////////////////////

#define SUPPORTED_COMMAND_BYTE_63                    (SUP_CMD_NONE)


#ifdef __cplusplus
}
#endif

#endif /* HCI_SUPPORTED_CMD_H */
