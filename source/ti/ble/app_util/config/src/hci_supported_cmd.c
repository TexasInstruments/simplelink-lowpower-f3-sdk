/******************************************************************************
 @file:       hci_supported_cmd.c

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
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#include "ti/ble/controller/hci/hci_supported_cmd.h"
#include "ti/ble/controller/hci/hci.h"

/**
 * @brief This function populates the provided buffer with the supported HCI commands.
 * It is implemented in the application layer and overrides the function in the controller.
 *
 * @param[out] pSupportedCmdBuf Pointer to the buffer where the supported HCI commands will be stored.
 */
void HCI_GetLocalSupportedCommands(uint8_t* pSupportedCmdBuf)
{
    // Note: There are 64 bytes of supported commands per the Core spec. One
    //       additional byte is added as a status when the table is returned
    //       as a parameter for event generation.
    const uint8_t supportedCmdsArray[SUPPORTED_COMMAND_LEN] =
    {
      // supported commands
      SUPPORTED_COMMAND_BYTE_0,
      SUPPORTED_COMMAND_BYTE_1,
      SUPPORTED_COMMAND_BYTE_2,
      SUPPORTED_COMMAND_BYTE_3,
      SUPPORTED_COMMAND_BYTE_4,
      SUPPORTED_COMMAND_BYTE_5,
      SUPPORTED_COMMAND_BYTE_6,
      SUPPORTED_COMMAND_BYTE_7,
      SUPPORTED_COMMAND_BYTE_8,
      SUPPORTED_COMMAND_BYTE_9,
      SUPPORTED_COMMAND_BYTE_10,
      SUPPORTED_COMMAND_BYTE_11,
      SUPPORTED_COMMAND_BYTE_12,
      SUPPORTED_COMMAND_BYTE_13,
      SUPPORTED_COMMAND_BYTE_14,
      SUPPORTED_COMMAND_BYTE_15,
      SUPPORTED_COMMAND_BYTE_16,
      SUPPORTED_COMMAND_BYTE_17,
      SUPPORTED_COMMAND_BYTE_18,
      SUPPORTED_COMMAND_BYTE_19,
      SUPPORTED_COMMAND_BYTE_20,
      SUPPORTED_COMMAND_BYTE_21,
      SUPPORTED_COMMAND_BYTE_22,
      SUPPORTED_COMMAND_BYTE_23,
      SUPPORTED_COMMAND_BYTE_24,
      SUPPORTED_COMMAND_BYTE_25,
      SUPPORTED_COMMAND_BYTE_26,
      SUPPORTED_COMMAND_BYTE_27,
      SUPPORTED_COMMAND_BYTE_28,
      SUPPORTED_COMMAND_BYTE_29,
      SUPPORTED_COMMAND_BYTE_30,
      SUPPORTED_COMMAND_BYTE_31,
      SUPPORTED_COMMAND_BYTE_32,
      SUPPORTED_COMMAND_BYTE_33,
      SUPPORTED_COMMAND_BYTE_34,
      SUPPORTED_COMMAND_BYTE_35,
      SUPPORTED_COMMAND_BYTE_36,
      SUPPORTED_COMMAND_BYTE_37,
      SUPPORTED_COMMAND_BYTE_38,
      SUPPORTED_COMMAND_BYTE_39,
      SUPPORTED_COMMAND_BYTE_40,
      SUPPORTED_COMMAND_BYTE_41,
      SUPPORTED_COMMAND_BYTE_42,
      SUPPORTED_COMMAND_BYTE_43,
      SUPPORTED_COMMAND_BYTE_44,
      SUPPORTED_COMMAND_BYTE_45,
      SUPPORTED_COMMAND_BYTE_46,
      SUPPORTED_COMMAND_BYTE_47,
      SUPPORTED_COMMAND_BYTE_48,
      SUPPORTED_COMMAND_BYTE_49,
      SUPPORTED_COMMAND_BYTE_50,
      SUPPORTED_COMMAND_BYTE_51,
      SUPPORTED_COMMAND_BYTE_52,
      SUPPORTED_COMMAND_BYTE_53,
      SUPPORTED_COMMAND_BYTE_54,
      SUPPORTED_COMMAND_BYTE_55,
      SUPPORTED_COMMAND_BYTE_56,
      SUPPORTED_COMMAND_BYTE_57,
      SUPPORTED_COMMAND_BYTE_58,
      SUPPORTED_COMMAND_BYTE_59,
      SUPPORTED_COMMAND_BYTE_60,
      SUPPORTED_COMMAND_BYTE_61,
      SUPPORTED_COMMAND_BYTE_62,
      SUPPORTED_COMMAND_BYTE_63
    };

    if (pSupportedCmdBuf != NULL)
    {
        memcpy(pSupportedCmdBuf, supportedCmdsArray, SUPPORTED_COMMAND_LEN);
    }
}
#ifdef __cplusplus
}
#endif
