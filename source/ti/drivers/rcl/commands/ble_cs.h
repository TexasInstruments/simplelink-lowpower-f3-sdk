/*
 * Copyright (c) 2022-2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ti_drivers_RCL_commands_ble_cs_h__include
#define ti_drivers_RCL_commands_ble_cs_h__include

#include <ti/drivers/rcl/RCL_Command.h>
#include <ti/drivers/rcl/RCL_Buffer.h>
#include <ti/drivers/rcl/handlers/ble_cs.h>

#include <ti/drivers/utils/List.h>

/* Forward declaration of types */
typedef struct RCL_CMD_BLE_CS_t                 RCL_CmdBleCs;
typedef struct RCL_CMD_BLE_CS_STEP_t            RCL_CmdBleCs_Step;
typedef struct RCL_CMD_BLE_CS_STEP_RESULT_t     RCL_CmdBleCs_StepResult;
typedef struct RCL_CMD_BLE_CS_S2R_t             RCL_CmdBleCs_S2r;
typedef struct RCL_CMD_BLE_CS_OUTPUT_t          RCL_CmdBleCs_Output;

typedef enum   RCL_CMD_BLE_CS_Phy_e             RCL_CmdBleCs_Phy;
typedef enum   RCL_CMD_BLE_CS_Role_e            RCL_CmdBleCs_Role;
typedef enum   RCL_CMD_BLE_CS_StepMode_e        RCL_CmdBleCs_StepMode;
typedef enum   RCL_CMD_BLE_CS_PacketResult_e    RCL_CmdBleCs_PacketResult;
typedef enum   RCL_CMD_BLE_CS_Tfcs_e            RCL_CmdBleCs_Tfcs;
typedef enum   RCL_CMD_BLE_CS_Tpm_e             RCL_CmdBleCs_Tpm;
typedef enum   RCL_CMD_BLE_CS_Tip_e             RCL_CmdBleCs_Tip;
typedef enum   RCL_CMD_BLE_CS_Tsw_e             RCL_CmdBleCs_Tsw;
typedef enum   RCL_CMD_BLE_CS_AntennaConfig_e   RCL_CmdBleCs_AntennaConfig;
typedef enum   RCL_CMD_BLE_CS_Payload_e         RCL_CmdBleCs_Payload;

/* Command IDs for generic commands */
#define RCL_CMDID_BLE_CS               0x1001U

/* Helper macros to convert between time units */
#define RCL_BLE_CS_US_TO_MCE_TIMER(x)        ((x)*48)
#define RCL_BLE_CS_US_TO_PBE_TIMER(x)        ((x)*4)
#define RCL_BLE_CS_MCE_TIMER_TO_US(x)        ((x)/48)
#define RCL_BLE_CS_PBE_TIMER_TO_US(x)        ((x)/4)
#define RCL_BLE_CS_MCE_TIMER_TO_PBE_TIMER(x) ((x)/12)

/* Helper macros for constants */
#define RCL_BLE_CS_MAX_NUM_ANT        4
#define RCL_BLE_CS_MAX_NUM_ANT_PATH   5
#define RCL_BLE_CS_MAX_PAYLOAD_SIZE   4
#define RCL_BLE_CS_NUM_CORR           3
#define RCL_BLE_CS_NUM_STIM           2
#define RCL_BLE_CS_LEN_S2R            512

/**
 *  @brief BLE Channel Sounding
 *
 *  Command to execute a BLE Channel Sounding event
 */
struct RCL_CMD_BLE_CS_t {
    RCL_Command common;
    struct {
      uint16_t role:1;                         /*!< Role of the device @ref RCL_CmdBleCs_Role */
      uint16_t phy:1;                          /*!< Phy used for packet exchange @ref RCL_CmdBleCs_Phy */
      uint16_t repeatSteps:1;                  /*!< Enable continuous repetition of step list */
      uint16_t chFilterEnable:1;               /*!< Enable filtering restricted channels (2402, 2403, 2425, 2426, 2427, 2479, 2480 MHz) */
      uint16_t nS2R:4;                         /*!< Total number of Samples-To-Ram containers in the list */
      uint16_t nSteps:8;                       /*!< Total number of steps within the BLE CS Sub-Event */
    } mode;

    struct {
      uint8_t select;                          /*!< Antenna pattern selection by index @ref RCL_CmdBleCs_AntennaConfig */
      uint8_t gpoMask;                         /*!< Mask of GPOs on the LRF controlling the antennas */
      uint8_t gpoVal[RCL_BLE_CS_MAX_NUM_ANT];  /*!< GPOCTRL word for antenna path 0..3 */
    } antennaConfig;

    struct {
      uint16_t tFcs;                           /*!< Config of time of Frequency Change period @ref RCL_CmdBleCs_Tfcs*/
      uint16_t tFm;                            /*!< Config of time of Frequency Measurement period */
      uint16_t tPm;                            /*!< Config of time of Phase Measurement Duration @ref RCL_CmdBleCs_Tpm*/
      uint16_t tIp1;                           /*!< Config of time of Interlude Period 1 @ref RCL_CmdBleCs_Tip*/
      uint16_t tIp2;                           /*!< Config of time of Interlude Period 2 @ref RCL_CmdBleCs_Tip*/
      uint16_t reserved;
      uint16_t tSw;                            /*!< Config of time of Antenna Switching duration @ref RCL_CmdBleCs_Tsw*/
      uint16_t tSwAdjustA;                     /*!< Time adjustment of first set of antenna switching within a step */
      uint16_t tSwAdjustB;                     /*!< Time adjustment of second set of antenna switching within a step */
    } timing;

    struct {
      uint8_t txPower;                         /*!< Index value of TX power */
      uint8_t rxGain;                          /*!< Index value of RX gain */
      uint8_t foffOverride;                    /*!< Frequency offset compensation override value in [4x FOFF] units. */
      uint8_t foffOverrideEnable:1;            /*!< Disables automatic estimate foff estimation and enforces the use of the provided override */
      uint8_t reserved:7;
    } frontend;

    uint16_t reserved1;

    RCL_CmdBleCs_Step       *steps;            /*!< Pointer to step list */
    RCL_CmdBleCs_StepResult *results;          /*!< Pointer to result list */
    RCL_CmdBleCs_S2r        *s2rResults;       /*!< Pointer to container list */
    RCL_CmdBleCs_Output     *output;           /*!< Pointer to statistics */
};

/* Default configuration of command */
#define RCL_CmdBleCs_Default()                          \
{                                                       \
    .common = RCL_Command_Default(RCL_CMDID_BLE_CS,     \
                                  RCL_Handler_BLE_CS),  \
}
#define RCL_CmdBleCs_DefaultRuntime() (RCL_CmdBleCs) RCL_CmdBleCs_Default()

/**
 *  @brief BLE Channel Sounding Step
 *
 *  Descriptor to configure a single step within the BLE Channel Sounding Event
 */
struct RCL_CMD_BLE_CS_STEP_t {
    uint16_t channelIdx;                             /*!< Integer index of channel information (0: 2400MHz) */
    uint16_t mode;                                   /*!< Step mode @ref RCL_CmdBleCs_StepMode */
    struct {
        uint16_t toneExtension : 2;                  /*!< Configuration of tone extension */
        uint16_t reserved : 13;
        uint16_t infiniteRx : 1;                     /*!< Disable timeout on Reflector Mode 0 */
    } options;
    uint16_t foffErr;                                /*!< Internal! Used for frequency offset compensation */
    uint16_t tAdjustA;                               /*!< Internal! Used for timegrid adjustment */
    uint16_t tAdjustB;                               /*!< Internal! Used for timegrid adjustment */
    uint16_t reserved0;
    uint16_t payloadLen;                             /*!< Length of payload in units of 32bit words @ref RCL_CmdBleCs_Payload */
    uint32_t payloadTx[RCL_BLE_CS_MAX_PAYLOAD_SIZE]; /*!< Payload to transmit containing random bit sequence (TX) */
    uint32_t payloadRx[RCL_BLE_CS_MAX_PAYLOAD_SIZE]; /*!< Expected payload to receive containing random bit sequence (RX) */
    uint32_t aaTx;                                   /*!< Access Address to be transmitted */
    uint32_t aaRx;                                   /*!< Access Address to be received */
    uint16_t antennaPermIdx;                         /*!< Index of entry to be used from the antenna permutation table @ref RCL_CmdBleCs_AntennaConfig */
    uint16_t antennaSequence;                        /*!< Internal! Decoded antenna control sequence based on permutation table */
    uint16_t tStep;                                  /*!< Internal! The total duration of step dynamically calculated */
    uint16_t tAntenna;                               /*!< Internal! The duration of optional payload, if any. Needed for antenna timing adjustment*/
};

/**
 *  @brief BLE Channel Sounding IQ Sample
 *
 *  Container to store complex vectors
 */
typedef struct {
    int16_t q;   /*!< Sample Q-branch */
    int16_t i;   /*!< Sample I-branch */
} IQSample;

/**
 *  @brief BLE Channel Sounding QQ data
 *
 *  Container to store RSSI data for postprocess of the quality of tone
 */
typedef struct {
    int16_t magnMin;     /*!< Minimum of signal magnitude during TPM */
    int16_t magnMax;     /*!< Maximum of signal magnitude during TPM */
    int16_t magnAvg;     /*!< Average of signal magnitude during TPM */
    int16_t magnAvgdB;   /*!< Average of signal magnitude during TPM in dB */
} MagnData;

/**
 *  @brief BLE Channel Sounding Step Result
 *
 *  Container to store the results of a single step within the BLE Channel Sounding Event
 */
struct RCL_CMD_BLE_CS_STEP_RESULT_t {
    uint16_t pktResult;                            /*!< Result of packet reception @ref RCL_CmdBleCs_PacketResult */
    int16_t  foffMeasured;                         /*!< Frequency offset between devices (only set on Initiator Mode 0) */
    uint16_t rtt;                                  /*!< RTT timestamp */
    uint16_t corr[RCL_BLE_CS_NUM_CORR];            /*!< Correlator values */
    uint16_t stim[RCL_BLE_CS_NUM_STIM];            /*!< STIM values: time offset, gardner error */
    IQSample dc;                                   /*!< DC offset measured on packet */
    IQSample pct[RCL_BLE_CS_MAX_NUM_ANT_PATH];     /*!< Phase Correction Terms per antenna path (incl. extension) */
    uint32_t payload[RCL_BLE_CS_MAX_PAYLOAD_SIZE]; /*!< Payload containing random bit sequence (RX) */
    MagnData magn[RCL_BLE_CS_MAX_NUM_ANT_PATH];    /*!< Received magnitude data of tone per antenna path for quality estimation (incl. extension) */
};

/**
 *  @brief BLE Channel Sounding S2R Sample type definition
 *
 *  One S2R sample can be interpreted multiple ways, depending of the signal source
 */
typedef union {
    uint32_t word;                 /*!< Sample format for copying data */
    uint8_t  byte[4];              /*!< Sample format of the Decimation Stage */
    IQSample iq;                   /*!< Sample format of the Front End Stage */
} S2RSample;

/**
 *  @brief BLE Channel Sounding S2R IQ Data
 *
 *  Container to store raw S2R samples of a BLE Channel Sounding Step
 */
struct RCL_CMD_BLE_CS_S2R_t {
  uint16_t  idx;                       /*!< The index of entry in the step list which the data belongs to */
  uint16_t  length;                    /*!< Number of valid samples in data */
  S2RSample data[RCL_BLE_CS_LEN_S2R];  /*!< Raw samples captured */
};

/**
 *  @brief BLE Channel Sounding Output Parameters
 *
 *  Container to store the statistical outputs of the BLE Channel Sounding Event
 */
struct RCL_CMD_BLE_CS_OUTPUT_t {
  uint16_t nStepsWritten;      /*!< Number of steps sent to the PBE through the FIFO */
  uint16_t nResultsRead;       /*!< Number of results read from the PBE through the FIFO */
  uint16_t nStepsDone;         /*!< Number of steps have been executed by the PBE */
  uint16_t nRxOk;              /*!< Number of steps where pktResult == OK, filled by the CM0 */
  uint16_t nRxNok;             /*!< Number of steps where pktResult != OK, filled by the CM0 */
  uint8_t  nS2RDone;           /*!< Number of Samples-To-Ram containers filled by the CM0 */
  int8_t   lastRssi;           /*!< Last valid RSSI received by the PBE */
  int16_t  lastFoff;           /*!< Last valid frequency offset received by the PBE */
};

/**
 *  @brief Enumerator of phy types
 *
 *  Describes the phy packets shall be transmitted
 */
enum RCL_CMD_BLE_CS_Phy_e {
    RCL_CmdBleCs_Phy_1M,
    RCL_CmdBleCs_Phy_2M,
    RCL_CmdBleCs_Phy_Length
};

/**
 *  @brief Enumerator of role types
 *
 *  Describes the role of the device during a BLE channel sounding event
 */
enum RCL_CMD_BLE_CS_Role_e {
    RCL_CmdBleCs_Role_Initiator,
    RCL_CmdBleCs_Role_Reflector,
    RCL_CmdBleCs_Role_Length
};

/**
 *  @brief Enumerator of step types
 *
 *  Describes the available modes of a BLE CS step
 */
enum RCL_CMD_BLE_CS_StepMode_e {
    RCL_CmdBleCs_StepMode_0,       /*!< (M) Pkt-Pkt-Tn */
    RCL_CmdBleCs_StepMode_1,       /*!< (M) Pkt-Pkt */
    RCL_CmdBleCs_StepMode_2,       /*!< (M) Tn-Tn */
    RCL_CmdBleCs_StepMode_3,       /*!< (O) Pkt-Tn-Tn-Pkt */
    RCL_CmdBleCs_StepMode_Length
};

/**
 *  @brief Enumerator of packet status options
 *
 *  Describes the available packet status words
 */
enum RCL_CMD_BLE_CS_PacketResult_e {
    RCL_CmdBleCs_PacketResult_Error,    /*!< NA for mode 2 steps, otherwise command execution error. */
    RCL_CmdBleCs_PacketResult_Ok,       /*!< The packet reception was succesfull */
    RCL_CmdBleCs_PacketResult_BitError, /*!< The packet was received with one or more bit errors */
    RCL_CmdBleCs_PacketResult_Lost,     /*!< The packet reception was terminated by timeout */
    RCL_CmdBleCs_PacketResult_Length
};

/**
 *  @brief Enumerator of tFcs duration options
 *
 *  Describes the available durations for frequency change
 */
enum RCL_CMD_BLE_CS_Tfcs_e {
    RCL_CmdBleCs_Tfcs_80us,     /*!< (C.1) */
    RCL_CmdBleCs_Tfcs_100us,
    RCL_CmdBleCs_Tfcs_120us,
    RCL_CmdBleCs_Tfcs_150us,    /*!< (M) */
    RCL_CmdBleCs_Tfcs_Length,
};

/**
 *  @brief Enumerator of tPm duration options
 *
 *  Describes the available durations for phase measurement
 */
enum RCL_CMD_BLE_CS_Tpm_e {
    RCL_CmdBleCs_Tpm_10us,
    RCL_CmdBleCs_Tpm_20us,
    RCL_CmdBleCs_Tpm_40us,      /*!< (M) */
    RCL_CmdBleCs_Tpm_Length,
};

/**
 *  @brief Enumerator of tIp duration
 *
 *  Describes the available durations for interlude period
 */
enum RCL_CMD_BLE_CS_Tip_e {
    RCL_CmdBleCs_Tip_40us,      /*!< (C.1) */
    RCL_CmdBleCs_Tip_50us,
    RCL_CmdBleCs_Tip_60us,
    RCL_CmdBleCs_Tip_80us,      /*!< (C.1) */
    RCL_CmdBleCs_Tip_145us,     /*!< (M) */
    RCL_CmdBleCs_Tip_Length,
};

/**
 *  @brief Enumerator of antenna switch duration
 *
 *  Describes the available durations for antenna switching transient
 */
enum RCL_CMD_BLE_CS_Tsw_e {
    RCL_CmdBleCs_Tsw_0us,       /*!< (M) for 1x1 */
    RCL_CmdBleCs_Tsw_1us,
    RCL_CmdBleCs_Tsw_2us,
    RCL_CmdBleCs_Tsw_4us,
    RCL_CmdBleCs_Tsw_10us,      /*!< (M) */
    RCL_CmdBleCs_Tsw_Length
};

/**
 *  @brief Enumerator of antenna configuration
 *
 *  Describes the available values for configuration of
 *  antenna control for PCT measurement.
 */
enum RCL_CMD_BLE_CS_AntennaConfig_e {
    RCL_CmdBleCs_AntennaConfig_1x1,     /*!< (M) */
    RCL_CmdBleCs_AntennaConfig_1x2,
    RCL_CmdBleCs_AntennaConfig_1x3,
    RCL_CmdBleCs_AntennaConfig_1x4,
    RCL_CmdBleCs_AntennaConfig_2x1,
    RCL_CmdBleCs_AntennaConfig_3x1,
    RCL_CmdBleCs_AntennaConfig_4x1,
    RCL_CmdBleCs_AntennaConfig_2x2,
    RCL_CmdBleCs_AntennaConfig_Length,
};

/**
 *  @brief Enumerator of payload length
 *
 *  Describes the available payload lengths
 */
enum RCL_CMD_BLE_CS_Payload_e {
    RCL_CmdBleCs_Payload_None,
    RCL_CmdBleCs_Payload_32bit,
    RCL_CmdBleCs_Payload_64bit,
    RCL_CmdBleCs_Payload_96bit,
    RCL_CmdBleCs_Payload_128bit,
    RCL_CmdBleCs_Payload_Length
};
#endif
