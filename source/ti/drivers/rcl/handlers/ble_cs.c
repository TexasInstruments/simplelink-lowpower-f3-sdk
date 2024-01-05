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
/*
 *  ======== ble_cs.c ========
 */

#include <stdint.h>
#include <stdlib.h>

#include <ti/log/Log.h>

#include <ti/drivers/rcl/RCL_Command.h>
#include <ti/drivers/rcl/RCL_Buffer.h>
#include <ti/drivers/rcl/RCL_Scheduler.h>

#include <ti/drivers/rcl/hal/hal.h>
#include <ti/drivers/rcl/commands/ble_cs.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(inc/hw_lrfdtxf.h)
#include DeviceFamily_constructPath(inc/hw_lrfdrxf.h)
#include DeviceFamily_constructPath(inc/hw_lrfddbell.h)
#include DeviceFamily_constructPath(inc/hw_lrfdpbe.h)
#include DeviceFamily_constructPath(inc/hw_lrfdpbe32.h)
#include DeviceFamily_constructPath(inc/hw_lrfdmdm.h)
#include DeviceFamily_constructPath(inc/hw_lrfdmdm32.h)
#include DeviceFamily_constructPath(inc/hw_lrfdrfe.h)
#include DeviceFamily_constructPath(inc/hw_lrfdrfe32.h)
#include DeviceFamily_constructPath(inc/hw_lrfds2r.h)
#include DeviceFamily_constructPath(inc/rfe_common_ram_regs.h)
#include DeviceFamily_constructPath(inc/pbe_generic_ram_regs.h)
#include DeviceFamily_constructPath(inc/pbe_common_ram_regs.h)
#include DeviceFamily_constructPath(inc/pbe_ble5_ram_regs.h)
#include DeviceFamily_constructPath(inc/pbe_ble_cs_ram_regs.h)
#include DeviceFamily_constructPath(inc/pbe_ble_cs_regdef_regs.h)

#ifndef OCTOPUS
#include DeviceFamily_constructPath(inc/hw_types.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_gpio.h)
#endif

#define BLE_CS_BASE_FREQ_MHZ        2400
#define BLE_CS_MIN_CHANNEL          2
#define BLE_CS_MAX_CHANNEL          80
#define BLE_CS_NUM_CHANNELS         90
#define BLE_CS_NUM_INVALID_CHANNELS 7
#define BLE_CS_SILENT_CHANNEL       100
#define BLE_CS_NUM_PERMUTATION      24
#define BLE_CS_S2R_OVERSAMPLE_RATE  4
#define BLE_CS_FIRST_BATCH_SIZE     1
#define DECODE_ANTENNA(x, y) (((x) >> ((y)*2)) & 0x03)
#define ENCODE_ANTENNA(x, y) (((x) << ((y)*4)))

/* Prepare constant terms for step specific calculations */
#define TRD   240 // Constant 5us at 48MHz
#define TGD   480 // Constant 10us at 48MHz
#define TPLT  960 // Constant 20us at 48MHz
#define TPYL  0   // Unknown at this point
#define T9    432
#define T8    384

/*
 *  ======== Execution status ========
 */
struct
{
    struct {
        RCL_CommandStatus endStatus;
    } common;
    struct {
        uint32_t startTime;
    } ble_cs;
} bleCsHandlerState;

uint32_t tStepLut[RCL_CmdBleCs_StepMode_Length + 1];
uint16_t tAdjALut[RCL_CmdBleCs_StepMode_Length];
uint16_t tAdjBLut[RCL_CmdBleCs_StepMode_Length];
uint16_t tAntennaAdjLut[RCL_CmdBleCs_StepMode_Length];
uint16_t tInfiniteRxAdj;

/*
 *  ======== Type for indexing antenna sequence ========
 */
typedef union {
    struct {
        uint8_t a0 : 2;
        uint8_t a1 : 2;
        uint8_t a2 : 2;
        uint8_t a3 : 2;
    } a;
    uint8_t apn;
} RCL_AntennaPath_t;

/*
 *  ======== Type for mapping antenna permutation to antenna sequence ========
 */
typedef struct {
    uint8_t numPath : 3;    /*!< Total number of antenna path */
    uint8_t numPerm : 5;    /*!< Total number of permutation possible with this configuration */
    uint8_t increment;      /*!< How to index the permutation table */
    RCL_AntennaPath_t ap[2];
} RCL_AntennaConfig_t;

/*
 *  ======== Antenna permutation table ========
 */
const RCL_AntennaPath_t antennaPermutation[BLE_CS_NUM_PERMUTATION] = {
  /*[ 0:5 ]=*/{{0,1,2,3}}, {{0,1,3,2}}, {{0,2,3,1}}, {{0,2,1,3}}, {{0,3,1,2}}, {{0,3,2,1}},
  /*[ 6:11]=*/{{1,0,2,3}}, {{1,0,3,2}}, {{1,2,3,0}}, {{1,2,0,3}}, {{1,3,0,2}}, {{1,3,2,0}},
  /*[12:17]=*/{{2,0,1,3}}, {{2,0,3,1}}, {{2,1,3,0}}, {{2,1,0,3}}, {{2,3,0,1}}, {{2,3,1,0}},
  /*[18:24]=*/{{3,0,2,1}}, {{3,0,1,2}}, {{3,1,2,0}}, {{3,1,0,2}}, {{3,2,0,1}}, {{3,2,1,0}}
};

/*
 *  ======== Antenna permutation selection mapping ========
 */
const RCL_AntennaConfig_t antennaConfig[RCL_CmdBleCs_AntennaConfig_Length] = {
  {.numPath=1, .numPerm=1,  .increment=0, .ap={/*initiator=*/{{0,0,0,0}}, /*reflector=*/{{0,0,0,0}}}},
  {.numPath=2, .numPerm=2,  .increment=6, .ap={/*initiator=*/{{0,0,0,0}}, /*reflector=*/{{0,1,0,0}}}},
  {.numPath=3, .numPerm=6,  .increment=3, .ap={/*initiator=*/{{0,0,0,0}}, /*reflector=*/{{0,1,2,0}}}},
  {.numPath=4, .numPerm=24, .increment=1, .ap={/*initiator=*/{{0,0,0,0}}, /*reflector=*/{{0,1,2,3}}}},
  {.numPath=2, .numPerm=2,  .increment=6, .ap={/*initiator=*/{{0,1,0,0}}, /*reflector=*/{{0,0,0,0}}}},
  {.numPath=3, .numPerm=6,  .increment=3, .ap={/*initiator=*/{{0,1,2,0}}, /*reflector=*/{{0,0,0,0}}}},
  {.numPath=4, .numPerm=24, .increment=1, .ap={/*initiator=*/{{0,1,2,3}}, /*reflector=*/{{0,0,0,0}}}},
  {.numPath=4, .numPerm=24, .increment=1, .ap={/*initiator=*/{{0,0,1,1}}, /*reflector=*/{{0,1,1,0}}}}
};

/*
 *  ======== Type of configuration of MCE and FIDC accumulator used for PCT measurement ========
 */
typedef union {
  struct {
    uint8_t period       : 2;   /*!< Configures the accumulation period of DEMFIDC module */
    uint8_t numIteration : 2;   /*!< Configures how many times to run the accumulator period */
    uint8_t fracWaitAdj  : 4;
    uint8_t fracWait;
  };
  uint16_t val;
} RCL_PctConfig_t;

typedef union {
  struct {
    uint8_t magnIteration;
    uint8_t magnAvg;
  };
  uint16_t val;
} RCL_MagnConfig_t;

typedef struct {
      int16_t mode0[2];
      int16_t mode1[2];
      int16_t mode2[2];
      int16_t mode3[2];
} RCL_Tadjust_t;

/*
 *  ======== Type of configuration for phy specific settings ========
 */
typedef struct {
    /* Modem specific registers */
    uint16_t        baud;
    uint16_t        symmap;
    uint16_t        demmisc3;

    /* Timing parameters */
    uint16_t        tPostProcessDiv1;   /*!<  Duration set aside for post processing */
    uint16_t        tPostProcessDiv12;  /*!<  Duration set aside for post processing */
    uint16_t        tRxTimeoutI0;       /*!<  Initiator must terminate packet reception before end of step */
    uint16_t        tRxTimeoutI3;       /*!<  Initiator must terminate packet reception before end of step */
    int16_t         tPilotAdjust;       /*!<  Pilot tone need to be adjusted with shaper delay */
    uint16_t        tStartup;           /*!<  Duration set aside for starting up the topSMs and start to emit a tone on the antenna */
    uint16_t        tPacket;            /*!<  Duration of packet [preamble + AA + Trailer] */
    uint16_t        tRxTimeoutRn;       /*!<  Reception timeout on reflector side */
    uint16_t        tInfiniteRx;
    uint16_t        tTr;
    uint16_t        tCorr;
    uint16_t        tDem;
    uint16_t        tFidc;
    uint16_t        tTxModDelay;

    /* Adjustment of antenna timings for mode 2 & 3 */
    uint16_t tAntennaAdj;

    /* Accumulator period and iteration number in order to fit into the same TPM period. */
    RCL_PctConfig_t pctConfig[RCL_CmdBleCs_Tpm_Length];

    /* Magnitude accumulator configuration used by RFE for tone quality estimation */
    RCL_MagnConfig_t magnConfig[RCL_CmdBleCs_Tpm_Length];

    /* Adjustment of TIP period per mode */
    RCL_Tadjust_t tAdj[RCL_CmdBleCs_Role_Length];
} RCL_PhyConfig_t;

/*
 *  ======== Configuration per PHY ========
 */
const RCL_PhyConfig_t phyConfigLut[RCL_CmdBleCs_Phy_Length] = {
    { /* Phy_1M */
      .baud     = 0x4000,
      .symmap   = 0x001F,
      .demmisc3 = 0x1182,

      .tPostProcessDiv1  = RCL_BLE_CS_US_TO_MCE_TIMER(21),
      .tPostProcessDiv12 = RCL_BLE_CS_US_TO_PBE_TIMER(21),
      .tRxTimeoutI0      = RCL_BLE_CS_US_TO_MCE_TIMER(69),
      .tRxTimeoutI3      = RCL_BLE_CS_US_TO_MCE_TIMER(56),
      .tPilotAdjust      = 261,
      .tStartup          = RCL_BLE_CS_US_TO_MCE_TIMER(2),
      .tPacket           = RCL_BLE_CS_US_TO_MCE_TIMER(44),
      .tRxTimeoutRn      = RCL_BLE_CS_US_TO_MCE_TIMER(75),
      .tAntennaAdj       = RCL_BLE_CS_US_TO_MCE_TIMER(1),
      .tTr               = RCL_BLE_CS_US_TO_MCE_TIMER(4),
      .tInfiniteRx       = 488,

      /* From simulation */
      .tCorr             = 373, //7.78us
      .tDem              = 888, //18.51us
      .tFidc             = RCL_BLE_CS_US_TO_MCE_TIMER(4),
      .tTxModDelay       = RCL_BLE_CS_US_TO_MCE_TIMER(4),

      .pctConfig = {
        /*TPM = 10US*/ {.period = (LRFDMDM_DEMFIDC0_ACCPERIOD_SMPL32 >> LRFDMDM_DEMFIDC0_ACCPERIOD_S),  .numIteration = 1, .fracWait = 32,  .fracWaitAdj = 0},
        /*TPM = 20US*/ {.period = (LRFDMDM_DEMFIDC0_ACCPERIOD_SMPL32 >> LRFDMDM_DEMFIDC0_ACCPERIOD_S),  .numIteration = 2, .fracWait = 32,  .fracWaitAdj = 0},
        /*TPM = 40US*/ {.period = (LRFDMDM_DEMFIDC0_ACCPERIOD_SMPL128 >> LRFDMDM_DEMFIDC0_ACCPERIOD_S), .numIteration = 1, .fracWait = 128, .fracWaitAdj = 0}
      },

      .magnConfig = {
        /*TPM = 10US*/ {.magnIteration = 3,  .magnAvg = 2},
        /*TPM = 20US*/ {.magnIteration = 7,  .magnAvg = 3},
        /*TPM = 40US*/ {.magnIteration = 15, .magnAvg = 4}
      },

      .tAdj = { /* { pkt-tn, tn-tn} */
        {
            /* Initiator */
            .mode0 = {0, 0},
            .mode1 = {0, 0},
            .mode2 = {-48, 228},
            .mode3 = {0, 28}
        },
        {
            /* Reflector */
            .mode0 = {-112, 0},
            .mode1 = {-112, 0},
            .mode2 = {-18, -319},
            .mode3 = {-50, -114}
        }
      },
    },
    { /* Phy_2M */
      .baud     = 0x8000,
      .symmap   = 0x002E,
      .demmisc3 = 0x1081,

      .tPostProcessDiv1  = RCL_BLE_CS_US_TO_MCE_TIMER(15),
      .tPostProcessDiv12 = RCL_BLE_CS_US_TO_PBE_TIMER(15),
      .tRxTimeoutI0      = RCL_BLE_CS_US_TO_MCE_TIMER(50),
      .tRxTimeoutI3      = RCL_BLE_CS_US_TO_MCE_TIMER(38),
      .tPilotAdjust      = -7,
      .tStartup          = RCL_BLE_CS_US_TO_MCE_TIMER(6),
      .tPacket           = RCL_BLE_CS_US_TO_MCE_TIMER(26),
      .tRxTimeoutRn      = RCL_BLE_CS_US_TO_MCE_TIMER(63),
      .tAntennaAdj       = RCL_BLE_CS_US_TO_MCE_TIMER(5),
      .tTr               = RCL_BLE_CS_US_TO_MCE_TIMER(2),
      .tInfiniteRx       = 326,

      /* From simulation */
      .tCorr             = RCL_BLE_CS_US_TO_MCE_TIMER(4),
      .tDem              = RCL_BLE_CS_US_TO_MCE_TIMER(11),
      .tFidc             = 130, //2.7us
      .tTxModDelay       = 124, //2.6us

      .pctConfig = {
        /*TPM = 10US*/{.period = (LRFDMDM_DEMFIDC0_ACCPERIOD_SMPL32 >> LRFDMDM_DEMFIDC0_ACCPERIOD_S),  .numIteration = 2, .fracWait = 32,  .fracWaitAdj = 9},
        /*TPM = 20US*/{.period = (LRFDMDM_DEMFIDC0_ACCPERIOD_SMPL128 >> LRFDMDM_DEMFIDC0_ACCPERIOD_S), .numIteration = 1, .fracWait = 128, .fracWaitAdj = 0},
        /*TPM = 40US*/{.period = (LRFDMDM_DEMFIDC0_ACCPERIOD_SMPL128 >> LRFDMDM_DEMFIDC0_ACCPERIOD_S), .numIteration = 2, .fracWait = 128, .fracWaitAdj = 0}
      },

      .magnConfig = {
        /*TPM = 10US*/{.magnIteration = 7,  .magnAvg = 3},
        /*TPM = 20US*/{.magnIteration = 15, .magnAvg = 4},
        /*TPM = 40US*/{.magnIteration = 31, .magnAvg = 5}
      },

      .tAdj = {
        {
            /* Initiator */
            .mode0 = {0, 0},
            .mode1 = {0, 0},
            .mode2 = {0, 191},
            .mode3 = {0, +16}
        },
           {
            /* Reflector */
            .mode0 = {-112, 0},
            .mode1 = {-112, 0},
            .mode2 = {-92, -244},
            .mode3 = {-32, -132}
        }
      },
    }
};

/* Frequency compensation value due to the fixed CMIXN value use. Resolution is 4x FOFF unit.
   Calculated based on the formula:
   foffError = 4x ((((1<<21) * fif) / (BLE_CS_BASE_FREQ_MHZ + pStep->channelIdx) / 1e3)
             - ((1<<21) * (1024 - pStep->cmixn) / 288 / 1024)); */
const int8_t foffErrorLut[BLE_CS_NUM_CHANNELS] = {
    /*2400 MHz =*/  38, 38, 38, 34, 34, 34, 30, 30, 30, 26,
    /*2410 MHz =*/  26, 26, 26, 22, 22, 22, 18, 18, 18, 14,
    /*2420 MHz =*/  14, 14, 10, 10, 10, 10,  6,  6,  6,  2,
    /*2430 MHz =*/   2,  2, -2, -2, -2, -2, -6, -6, -6,-10,
    /*2440 MHz =*/ -10,-10,-14,-14,-14,-14,-18,-18,-18,-22,
    /*2450 MHz =*/ -22,-22,-26,-26,-26,-26,-30,-30,-30,-34,
    /*2460 MHz =*/ -34,-34,-34,-38,-38,-38,-42,-42,-42,-46,
    /*2470 MHz =*/ -46,-46,-46,-50,-50,-50,-54,-54,-54,-54,
    /*2480 MHz =*/ -58,-58,-58,-62,-62,-62,-62,-66,-66,-66
};

/* The following channels are not allowed to be used for Channel Sounding */
const uint8_t channelFilter[BLE_CS_NUM_INVALID_CHANNELS] = {
    2, 3, 25, 26, 27, 79, 80
};

/* Look up table to payload length in units of MCE Timer at rate of [1M]. Scaling dynamically if needed. */
const uint16_t payloadLut[RCL_CmdBleCs_Payload_Length] = {
    /*0 bits =*/   RCL_BLE_CS_US_TO_MCE_TIMER(0),
    /*32 bits =*/  RCL_BLE_CS_US_TO_MCE_TIMER(32),
    /*64 bits =*/  RCL_BLE_CS_US_TO_MCE_TIMER(64),
    /*96 bits =*/  RCL_BLE_CS_US_TO_MCE_TIMER(96),
    /*128 bits =*/ RCL_BLE_CS_US_TO_MCE_TIMER(128)
};

/* Forward declarations */
static void RCL_Handler_BLE_CS_configureTxRxFIFO(RCL_CmdBleCs* pCmd);
static void RCL_Handler_BLE_CS_fillTxBuffer(RCL_CmdBleCs* pCmd);
static void RCL_Handler_BLE_CS_readRxBuffer(RCL_CmdBleCs* pCmd);
static void RCL_Handler_BLE_CS_configureS2R(RCL_CmdBleCs *pCmd);
static void RCL_Handler_BLE_CS_readS2RSamples(RCL_CmdBleCs *pCmd);
static void RCL_Handler_BLE_CS_readStatistics(RCL_CmdBleCs *pCmd);
static void RCL_Handler_BLE_CS_preprocessCommand(RCL_CmdBleCs *pCmd);
static RCL_CmdBleCs_Step* RCL_Handler_BLE_CS_fetchNextStep(RCL_CmdBleCs *pCmd);
static RCL_CmdBleCs_Step* RCL_Handler_BLE_CS_preprocessStep(RCL_CmdBleCs *pCmd, RCL_CmdBleCs_Step *pStep);
static RCL_CmdBleCs_StepResult* RCL_Handler_BLE_CS_fetchNextStepResult(RCL_CmdBleCs *pCmd);
static RCL_CommandStatus RCL_Handler_BLE_CS_findPbeErrorEndStatus(uint16_t pbeEndStatus);

/*
 *  ======== Configure the BUFRAM size and interrupts for BLE CS ========
 */
static void RCL_Handler_BLE_CS_configureTxRxFIFO(RCL_CmdBleCs* pCmd)
{
    /* Override the FIFO location and size */
    HWREG(LRFDPBE_BASE + LRFDPBE_O_FCFG1)    = 128;   // TXFIFO base address
    HWREG(LRFDPBE_BASE + LRFDPBE_O_FCFG2)    = 128;   // 128 x 32bit word
    HWREG(LRFDPBE_BASE + LRFDPBE_O_FCFG3)    = 256;   // RXFIFO base address
    HWREG(LRFDPBE_BASE + LRFDPBE_O_FCFG4)    = 128;   // 128 x 32bit word

    /* Reset FIFOs to have a clean start */
    /* Writing to FCMD is safe since the PBE is not running, ref. RCL-367 */
    HWREG(LRFDPBE_BASE + LRFDPBE_O_FCMD) = LRFDPBE_FCMD_DATA_FIFO_RESET;

    /* Calculate how many entries can be stored simultaneous in given size of tx/rx fifos */
    uint16_t nStepsFit = HWREG_READ_LRF(LRFDPBE_BASE + LRFDPBE_O_TXFWRITABLE) / sizeof(RCL_CmdBleCs_Step);
    HWREG(LRFDPBE_BASE + LRFDPBE_O_TXFWBTHRS) = (nStepsFit - 1) * sizeof(RCL_CmdBleCs_Step);

    uint16_t nResultsFit = HWREG_READ_LRF(LRFDPBE_BASE + LRFDPBE_O_RXFWRITABLE) / sizeof(RCL_CmdBleCs_StepResult);
    HWREG(LRFDPBE_BASE + LRFDPBE_O_RXFRBTHRS) = (nResultsFit - 1) * sizeof(RCL_CmdBleCs_StepResult);

    #ifndef PG2
    /* CDDS BUG00003 - Temporarily workaround */
    HWREG(LRFDPBE_BASE + LRFDPBE_O_RXFRBTHRS) = sizeof(RCL_CmdBleCs_StepResult);
    #endif

    /* FIFO pointers should auto-commit/auto-dealloc, enable threshold events */
    HWREG(LRFDPBE_BASE + LRFDPBE_O_FCFG0)   = HWREG_READ_LRF(LRFDPBE_BASE + LRFDPBE_O_FCFG0) | (LRFDPBE_FCFG0_TXACOM_M | LRFDPBE_FCFG0_TXADEAL_M | LRFDPBE_FCFG0_RXACOM_M | LRFDPBE_FCFG0_RXADEAL_M);
    HWREG(LRFDPBE_BASE + LRFDPBE_O_FCFG0)   = HWREG_READ_LRF(LRFDPBE_BASE + LRFDPBE_O_FCFG0) & (~(LRFDPBE_FCFG0_RXIRQMET_M | LRFDPBE_FCFG0_TXIRQMET_M));

    /* Clear any interrupts left from uninitialized fifos */
    HWREG(LRFDPBE_BASE + LRFDPBE_O_EVTCLR1)  =  (LRFDPBE_EVTCLR1_RXRDBTHR_M |  LRFDPBE_EVTCLR1_RXWRBTHR_M | LRFDPBE_EVTCLR1_TXRDBTHR_M | LRFDPBE_EVTCLR1_TXWRBTHR_M);
    HWREG(LRFDDBELL_BASE + LRFDDBELL_O_ICLR0) = HWREG_READ_LRF(LRFDDBELL_BASE + LRFDDBELL_O_ICLR0) | (LRFDDBELL_ICLR0_RXFIFO_M | LRFDDBELL_ICLR0_TXFIFO_M);
}

/*
 *  ======== Fill the BLE CS step descriptors into the BUFRAM TX FIFO ========
 */
static void RCL_Handler_BLE_CS_fillTxBuffer(RCL_CmdBleCs* pCmd)
{
    if (pCmd->output->nStepsWritten < pCmd->mode.nSteps)
    {
        /* Calculate how many additional step will fit (which might be all) */
        uint16_t nWritable  = HWREG_READ_LRF(LRFDPBE_BASE + LRFDPBE_O_TXFWRITABLE) / sizeof(RCL_CmdBleCs_Step);
        uint16_t nRemaining = pCmd->mode.nSteps - pCmd->output->nStepsWritten;
        uint16_t nBatch     = (nWritable < nRemaining) ? nWritable : nRemaining;

        /* Limited time is available when we start the command (ABS_START_TIME_OFFSET),
           so the first batch is reduced */
        #ifdef PG2
            /* CDDS BUG00003 - Temporarily workaround */
            if (pCmd->output->nStepsWritten == 0)
        #else
            if (true)
        #endif
        {
            nBatch = (nBatch < BLE_CS_FIRST_BATCH_SIZE) ? nBatch : BLE_CS_FIRST_BATCH_SIZE;
        }

        /* Transfer the calculated number of steps into the fifo */
        for(uint16_t i=0; i<nBatch; i++, pCmd->output->nStepsWritten++)
        {
            /* Pop the next step */
            RCL_CmdBleCs_Step *pStep = RCL_Handler_BLE_CS_fetchNextStep(pCmd);

            /* Send it to PBE */
            /* Using push operations is safe because the FIFO is used in
               streaming mode, ref. RCL-367. This means that the BLE CS PBE
               does not write to FCMD. */
            uint32_t *ptr = (uint32_t *) RCL_Handler_BLE_CS_preprocessStep(pCmd, pStep);
            for(uint8_t j=0; j<sizeof(RCL_CmdBleCs_Step)/sizeof(uint32_t); j++)
            {
                HWREG(LRFDTXF_BASE + LRFDTXF_O_TXD) = *(ptr+j);
            }

        }

        if (pCmd->output->nStepsWritten == pCmd->mode.nSteps)
        {
            if (pCmd->mode.repeatSteps)
            {
                /* Pretend that this is the beginning of the sequence */
                pCmd->output->nStepsWritten = 0;
            }
            else
            {
                /* No more steps left, no more interrupts needed, boost threshold to max value */
                HWREG(LRFDPBE_BASE + LRFDPBE_O_TXFWBTHRS) = 0x3FF;
            }
        }
    }
}

/*
 *  ======== Read the BLE CS Step results from the BUFRAM RX fifo ========
 */
static void RCL_Handler_BLE_CS_readRxBuffer(RCL_CmdBleCs* pCmd)
{
    if (pCmd->output->nResultsRead < pCmd->mode.nSteps)
    {
        /* Calculate how many result entries are available in rx fifo (might be all that has left) */
        uint16_t nReadable  = HWREG_READ_LRF(LRFDPBE_BASE + LRFDPBE_O_RXFREADABLE) / sizeof(RCL_CmdBleCs_StepResult);
        uint16_t nRemaining = pCmd->mode.nSteps - pCmd->output->nResultsRead;
        uint16_t nBatch     = (nReadable < nRemaining) ? nReadable : nRemaining;

        /* Read out the calculated number of entries */
        for(uint16_t i=0; i<nBatch; i++, pCmd->output->nResultsRead++)
        {
            RCL_CmdBleCs_StepResult *pResult = RCL_Handler_BLE_CS_fetchNextStepResult(pCmd);

            /* Using pop operations is safe because the FIFO is used in
               streaming mode, ref. RCL-367. This means that the BLE CS PBE
               does not write to FCMD. */
            uint32_t *ptr = (uint32_t *) pResult;
            for(uint8_t j=0; j<sizeof(RCL_CmdBleCs_StepResult)/sizeof(uint32_t); j++)
            {
                *(ptr+j) = HWREG_READ_LRF(LRFDRXF_BASE + LRFDRXF_O_RXD);
            }

            if (pResult->pktResult == RCL_CmdBleCs_PacketResult_Ok)
            {
                pCmd->output->nRxOk += 1;
            }
            else
            {
                pCmd->output->nRxNok += 1;
            }
        }

        /* Reduce the batch size if there is only a few entries missing */
        nRemaining = pCmd->mode.nSteps - pCmd->output->nResultsRead;
        if (nRemaining < nBatch)
        {
            if (nRemaining)
            {
                HWREG(LRFDPBE_BASE + LRFDPBE_O_RXFRBTHRS) = (nRemaining * sizeof(RCL_CmdBleCs_StepResult));
            }
            else if (pCmd->mode.repeatSteps)
            {
                pCmd->output->nResultsRead = 0;
            }
            else
            {
                HWREG(LRFDPBE_BASE + LRFDPBE_O_RXFRBTHRS) = 0x3FF;
            }
        }
    }
}


/*
 *  ======== Load the command and step descriptors into LRF REGISTERS/BUFRAM ========
 */
static void RCL_Handler_BLE_CS_configureS2R(RCL_CmdBleCs *pCmd)
{
    LRF_setClockEnable(LRFDDBELL_CLKCTL_S2RRAM_M | LRFDDBELL_CLKCTL_S2R_M, LRF_CLK_ENA_RCL);

    /* Decode length */
    uint16_t len = (uint16_t)(sizeof(RCL_CmdBleCs_S2r)/sizeof(uint32_t) - 1); // -1 = container header

    /* Store 32bit words in S2R (offset 3072) memory; don't arm yet */
    HWREG(LRFDS2R_BASE + LRFDS2R_O_START) = 0x0C00;
    HWREG(LRFDS2R_BASE + LRFDS2R_O_STOP)  = HWREG_READ_LRF(LRFDS2R_BASE + LRFDS2R_O_START) + len;
    HWREG(LRFDS2R_BASE + LRFDS2R_O_CFG)   = ((LRFDS2R_CFG_CTL_EN) & LRFDS2R_CFG_CTL_M) |
                  //((LRFDS2R_CFG_SEL_DECSTAGE) & LRFDS2R_CFG_SEL_M) |
                  ((LRFDS2R_CFG_SEL_FRONTEND) & LRFDS2R_CFG_SEL_M) |
                  ((LRFDS2R_CFG_TRIGMODE_ONESHOT) & LRFDS2R_CFG_TRIGMODE_M) |
                  ((LRFDS2R_CFG_LAST0_DIS) & LRFDS2R_CFG_LAST0_M);
}

/*
 *  ======== Read the S2R memory ========
 */
void RCL_Handler_BLE_CS_readS2RSamples(RCL_CmdBleCs *pCmd)
{
    #define BLE_CS_S2R_SAMPLE_INCREMENT  32
    #define BLE_CS_S2R_SAMPLE_SIZE       4

    /* Read and erase s2r memory */
    RCL_CmdBleCs_S2r *container = pCmd->s2rResults;

    //HWREG(GPIO_BASE + GPIO_O_DOE31_0)  |= (1<<9);
    //HWREG(GPIO_BASE + GPIO_O_DOUT31_0) |= (1<<9);

    /* Avoid NULL pointer dereferencing */
    if (container)
    {
        /* Avoid indexing out of array */
        if (pCmd->output->nS2RDone < pCmd->mode.nS2R)
        {
            /* Find the next empty entry */
            container = &container[pCmd->output->nS2RDone++];

            /* Store the step index */
            container->idx = HWREGH_READ_LRF(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_S2ROUTIDX);

            /* Number of valid samples to be copied out */
            container->length = pCmd->steps[container->idx].payloadLen
                              * BLE_CS_S2R_SAMPLE_INCREMENT
                              * BLE_CS_S2R_OVERSAMPLE_RATE;

            /* Store the data */
            uint32_t *src = (uint32_t *)S2R_RAM_BASE_ADDR;
            uint32_t *dst = (uint32_t *)&container->data;
            for(uint16_t i=0; i<container->length; i++)
            {
                *dst++ = *src;
                *src++ = 0;
            }
        }
    }
    //HWREG(GPIO_BASE + GPIO_O_DOUT31_0) &= ~(1<<9);
}

/*
 *  ======== Read the statistics from the PBE ========
 */
static void RCL_Handler_BLE_CS_readStatistics(RCL_CmdBleCs *pCmd)
{
    if (pCmd->output) {
        pCmd->output->nStepsDone = HWREGH_READ_LRF(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_NSTEPSDONE);
        pCmd->output->lastRssi   = HWREGH_READ_LRF(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_RSSILAST);
        pCmd->output->lastFoff   = HWREGH_READ_LRF(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_FOFFLAST);
    }
}

/*
 *  ======== Load the command and step descriptors into LRF REGISTERS/BUFRAM ========
 */
static void RCL_Handler_BLE_CS_preprocessCommand(RCL_CmdBleCs *pCmd)
{
    /* Decode mapping of configuration to antennas */
    const RCL_AntennaConfig_t *antennaEntry = &antennaConfig[pCmd->antennaConfig.select];

    /* Decode the Phy specific settings */
    const RCL_PhyConfig_t *config = &phyConfigLut[pCmd->mode.phy];

    /* Clear output statistics */
    if (pCmd->output)
    {
        uint8_t *ptr = (uint8_t *) pCmd->output;
        for (uint8_t i=0; i < sizeof(RCL_CmdBleCs_Output); i++)
        {
            *ptr++ = 0;
        }
    }

    /* Mode */
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_MODE) = ((pCmd->mode.role        << PBE_BLE_CS_RAM_MODE_ROLE_S)      & PBE_BLE_CS_RAM_MODE_ROLE_M) |
                            ((pCmd->mode.phy         << PBE_BLE_CS_RAM_MODE_PHY_S)       & PBE_BLE_CS_RAM_MODE_PHY_M) |
                            ((pCmd->mode.repeatSteps << PBE_BLE_CS_RAM_MODE_INFINIT_S)   & PBE_BLE_CS_RAM_MODE_INFINIT_M) |
                            ((pCmd->mode.nSteps      << PBE_BLE_CS_RAM_MODE_NUM_STEPS_S) & PBE_BLE_CS_RAM_MODE_NUM_STEPS_M);

    /* Antenna */
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_ANTN)   = antennaEntry->numPath;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_ANTMSK) = pCmd->antennaConfig.gpoMask;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_ANT0)   = pCmd->antennaConfig.gpoVal[0];
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_ANT1)   = pCmd->antennaConfig.gpoVal[1];
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_ANT2)   = pCmd->antennaConfig.gpoVal[2];
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_ANT3)   = pCmd->antennaConfig.gpoVal[3];

    /* Timing */
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TFCS)        = pCmd->timing.tFcs - (TPLT + config->tStartup); // Pilot tone + startup time is included into the TFCS budget
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TFM)         = pCmd->timing.tFm;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TPM)         = pCmd->timing.tPm;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TIP1)        = pCmd->timing.tIp1;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TIP2)        = pCmd->timing.tIp2;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TSW)         = pCmd->timing.tSw;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TSWADJA)     = pCmd->timing.tSwAdjustA;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TSWADJB)     = pCmd->timing.tSwAdjustB;

    /* Timegrid adjustment shall be initialized with maximum unsigned value (ca. 536s @ 4MHz) */
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TSTEPACCTHRH)= 0x7FFF;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TSTEPACCTHRL)= 0xFFFF;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TSTEPACCH)   = 0;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TSTEPACCL)   = 0;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TSTEPCOMP)   = 0;

    /* Initialize MOD.FOFF values. Usueful for sub-events without mode 0 steps */
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_FOFFSUM)     = pCmd->frontend.foffOverride;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_FOFFNUM)     = pCmd->frontend.foffOverrideEnable;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_NSTEPSDONE)  = 0;

    /* Program frequency dependent config: hardcode on 2440MHz, to be updated on the fly */
    HWREG(LRFDMDM32_BASE + LRFDMDM32_O_DEMFRAC1_DEMFRAC0) = 0x0D800000;     // P: Constant
    HWREG(LRFDMDM32_BASE + LRFDMDM32_O_DEMFRAC3_DEMFRAC2) = 0x0E4C0000;     // Q: Being dynammically scaled by RFE per channel

    /* Shaper gain */
    HWREG(LRFDRFE_BASE + LRFDRFE_O_MOD0) = 0x1824;

    /* Configure RX gain */
    HWREG(LRFDRFE_BASE + LRFDRFE_O_SPARE0) = (HWREG(LRFDRFE_BASE + LRFDRFE_O_SPARE0) & 0x0F) | (pCmd->frontend.rxGain << 4);

    if (pCmd->mode.role == RCL_CmdBleCs_Role_Initiator) {
        /* Initiator: timeout is hardcoded in the MCE in order to comply with the static timegrid.
           This parameter is therefor ignored.  */
        HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TRXTIMEOUT) = 0;

    } else {
        /* The miminum value is defined by the static timegrid. Different for each datarate. */
        HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TRXTIMEOUT) = config->tRxTimeoutRn;
    }

    uint8_t lutIdx = RCL_CmdBleCs_Tpm_40us;
    if (pCmd->timing.tPm <= RCL_BLE_CS_US_TO_MCE_TIMER(20)) {
        lutIdx = RCL_CmdBleCs_Tpm_20us;
    }
    if (pCmd->timing.tPm <= RCL_BLE_CS_US_TO_MCE_TIMER(10)) {
        lutIdx = RCL_CmdBleCs_Tpm_10us;
    }

    /* Baudrate specific settings */
    HWREG(LRFDMDM_BASE + LRFDMDM_O_SPARE0)                       = config->pctConfig[lutIdx].val;
    HWREG(LRFDMDM_BASE + LRFDMDM_O_BAUD)                         = config->baud;
    HWREG(LRFDMDM_BASE + LRFDMDM_O_MODSYMMAP0)                   = config->symmap;
    HWREG(LRFDRFE_BASE + LRFDRFE_O_SPARE3)                       = config->magnConfig[lutIdx].val;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_DEMMISC3)          = config->demmisc3;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TPOSTPROCESSDIV1)  = config->tPostProcessDiv1;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TPOSTPROCESSDIV12) = config->tPostProcessDiv12;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TRXTIMEOUTI0)      = config->tRxTimeoutI0;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TRXTIMEOUTI3)      = config->tRxTimeoutI3;
    HWREGH(LRFD_BUFRAM_BASE + PBE_BLE_CS_RAM_O_TPILOTADJ)         = config->tPilotAdjust;

    /* Alias for dynamic parameters for calculations */
    uint16_t tIp1  = pCmd->timing.tIp1;
    uint16_t tIp2  = pCmd->timing.tIp2;
    uint16_t tFm   = pCmd->timing.tFm;
    uint16_t tPm   = pCmd->timing.tPm;
    uint16_t tFcs  = pCmd->timing.tFcs;
    uint16_t tSw   = pCmd->timing.tSw;
    uint16_t nPath = antennaEntry->numPath;

    /* Phy specific delays */
    uint16_t tStartup    = config->tStartup;
    uint16_t tPkt        = config->tPacket;
    uint16_t tCorr       = config->tCorr;
    uint16_t tDem        = config->tDem;
    uint16_t tTr         = config->tTr;
    uint16_t tFidc       = config->tFidc;
    uint16_t tTxModDelay = config->tTxModDelay;

    /* Calculate base terms of step duration for each mode.
       Later can be extended with optional payload per step before sending it over to the PBE. */
    tStepLut[RCL_CmdBleCs_StepMode_0] = (tFcs + ((tPkt + TRD) << 1) + TGD + tIp1 + tFm);
    tStepLut[RCL_CmdBleCs_StepMode_1] = (tFcs + ((tPkt + TPYL + TRD) << 1) + tIp1);
    tStepLut[RCL_CmdBleCs_StepMode_2] = (tFcs + ((((tPm + tSw) * (nPath + 1)) + TRD) << 1) + tIp2);
    tStepLut[RCL_CmdBleCs_StepMode_3] = (tFcs + ((tPkt + TPYL + TGD + ((tPm + tSw) * (nPath + 1)) + TRD) << 1) + tIp2);

    /* Special value for infinite RX on Reflector Mode 0 step, where syncronization of packet is implemented */
    tStepLut[RCL_CmdBleCs_StepMode_Length] = (0 - tDem + tTr + TRD + tIp1 + tPkt + TGD + tFm + TRD);
    tInfiniteRxAdj = config->tInfiniteRx;

    const RCL_Tadjust_t *tAdj = &config->tAdj[pCmd->mode.role];

    if (pCmd->mode.role == RCL_CmdBleCs_Role_Initiator)
    {
        /* Calculate base terms of interlude period. Later will scale with payload. */

        /* TX */
        tAdjALut[RCL_CmdBleCs_StepMode_0] = tAdj->mode0[0] + 0xFFFF;
        tAdjALut[RCL_CmdBleCs_StepMode_1] = tAdj->mode1[0] + 0xFFFF;
        tAdjALut[RCL_CmdBleCs_StepMode_2] = tAdj->mode2[0] + 0x0000; // No packet, variable used for pilot adjustment
        tAdjALut[RCL_CmdBleCs_StepMode_3] = tAdj->mode3[0] + 0xFFFF;

        /* RX */
        tAdjBLut[RCL_CmdBleCs_StepMode_0] = tAdj->mode0[1] + TRD + tIp1 - TPLT;
        tAdjBLut[RCL_CmdBleCs_StepMode_1] = tAdj->mode1[1] + TRD + tIp1 - TPLT;
        tAdjBLut[RCL_CmdBleCs_StepMode_2] = tAdj->mode2[1] + TRD + tIp2 + tSw + tFidc;
        tAdjBLut[RCL_CmdBleCs_StepMode_3] = tAdj->mode3[1] + TRD + tIp2 + tSw + tFidc;
    }
    else
    {
        /* RX */
        tAdjALut[RCL_CmdBleCs_StepMode_0] = tAdj->mode0[0] - tCorr + tTr + TRD + tIp1 - TPLT - tTxModDelay;
        tAdjALut[RCL_CmdBleCs_StepMode_1] = tAdj->mode1[0] - tCorr + tTr + TRD + tIp1 - TPLT - tTxModDelay;
        tAdjALut[RCL_CmdBleCs_StepMode_2] = tAdj->mode2[0] + tStartup + TPLT + tSw + tFidc;
        tAdjALut[RCL_CmdBleCs_StepMode_3] = tAdj->mode3[0] - tCorr + tTr + TGD + tSw + tFidc;

        /* TX */
        tAdjBLut[RCL_CmdBleCs_StepMode_0] = tAdj->mode0[1];
        tAdjBLut[RCL_CmdBleCs_StepMode_1] = tAdj->mode1[1];
        tAdjBLut[RCL_CmdBleCs_StepMode_2] = tAdj->mode2[1] - tFidc - tSw /* + TRD */ + tIp2 - TPLT; // Ramp-down delay is added by the topSM
        tAdjBLut[RCL_CmdBleCs_StepMode_3] = tAdj->mode3[1] - tFidc - tSw /* + TRD */ + tIp2 - TPLT - tTxModDelay;
    }

    /* Adjustment of antenna control per mode per phy */
    uint16_t tAntennaAdj = config->tAntennaAdj;
    tAntennaAdjLut[RCL_CmdBleCs_StepMode_0] = 0;
    tAntennaAdjLut[RCL_CmdBleCs_StepMode_1] = 0;
    tAntennaAdjLut[RCL_CmdBleCs_StepMode_2] = tAntennaAdj + TPLT;
    tAntennaAdjLut[RCL_CmdBleCs_StepMode_3] = tAntennaAdj + TPLT + tPkt + TGD;

    /* Enforce modulator and demodulator clock. RTL bug with automatic control. */
    LRF_setClockEnable(LRFDDBELL_CLKCTL_DEM_M | LRFDDBELL_CLKCTL_MOD_M, LRF_CLK_ENA_RCL);
}

/*
 *  ======== Fetch the next step in the list and return a pointer to it ========
 */
static RCL_CmdBleCs_Step* RCL_Handler_BLE_CS_fetchNextStep(RCL_CmdBleCs *pCmd)
{
    /* Decode the offset of next step to be processed */
    return &pCmd->steps[pCmd->output->nStepsWritten];
}

/*
 *  ======== Preprocess the step ========
 */
static RCL_CmdBleCs_Step* RCL_Handler_BLE_CS_preprocessStep(RCL_CmdBleCs *pCmd, RCL_CmdBleCs_Step* pStep)
{
    /* Disable restricted channels if filtering is turned on */
    if (pCmd->mode.chFilterEnable == true)
    {
        uint16_t channelIdx = pStep->channelIdx;

        if( channelIdx < BLE_CS_MIN_CHANNEL || channelIdx > BLE_CS_MAX_CHANNEL)
        {
            /* The user configured a channel outside the allowed domain!
               We still keep the timegrid but silence this step. */
            pStep->channelIdx = BLE_CS_SILENT_CHANNEL;
        }
        else
        {
            for (uint8_t i = 0; i < sizeof(channelFilter); i++)
            {
                if (channelIdx == channelFilter[i])
                {
                    /* The user configured a restricted channel (i.e. BLE Advertisement channels)! */
                    pStep->channelIdx = BLE_CS_SILENT_CHANNEL;
                    break;
                }
            }
        }
    }

    /* Decode the base term for the complete duration of the step */
    uint32_t tStep = tStepLut[pStep->mode];
    uint16_t tAdjA = tAdjALut[pStep->mode];
    uint16_t tAdjB = tAdjBLut[pStep->mode];

    /* Handle special case of reflector mode 0 with infinite RX */
    if ((pCmd->mode.role == RCL_CmdBleCs_Role_Reflector) &&
        (pStep->mode == RCL_CmdBleCs_StepMode_0) &&
        (pStep->options.infiniteRx == true))
    {
        /* Remaining step duration is reduced */
        tStep = tStepLut[RCL_CmdBleCs_StepMode_Length];
        /* Adjust for wakeup on SYNC event instead of correlator */
        tAdjA -= tInfiniteRxAdj;
    }
    else
    {
        /* For everything else, disable infinite mode */
        pStep->options.infiniteRx = false;
    }

    /* Scale with bit length and 1M/2M rate */
    uint16_t tPyl = (payloadLut[pStep->payloadLen]) >> pCmd->mode.phy;

    /* Extend with timing of optional payload */
    if (pStep->payloadLen)
    {
        /* Adjustment only on Reflector */
        uint16_t tAdj = (pCmd->mode.role == RCL_CmdBleCs_Role_Reflector)
                      ? (tPyl)
                      : (0);

        /* Payload is only allowed for certain modes. */
        if (pStep->mode == RCL_CmdBleCs_StepMode_1 ||
            pStep->mode == RCL_CmdBleCs_StepMode_3)
        {
            /* Multiplied by two for combined effect of TX and RX */
            tStep += (tPyl << 1);

            /* The duration from AA sync until the next activity scales with payload on reflector only */
            tAdjA += tAdj;
        }
        else
        {
            /* Enforce no payload for mode 0 & 2 */
            pStep->payloadLen = 0;

            /* No adjustment needed */
            tPyl = 0;
        }
    }

    /* Configure the PBE with the scaled step duration */
    pStep->tStep = (uint16_t)RCL_BLE_CS_MCE_TIMER_TO_PBE_TIMER(tStep);

    /* Antenna timing from beginning of step */
    pStep->tAntenna = tAntennaAdjLut[pStep->mode] + tPyl;

    /* Configure the MCE with the scaled intermediate timegrid parameters */
    pStep->tAdjustA = tAdjA;
    pStep->tAdjustB = tAdjB;

    /* Antenna calculations */
    pStep->antennaSequence = 0;

    /* Decode mapping of configuration to antennas */
    const RCL_AntennaConfig_t *antennaEntry = &antennaConfig[pCmd->antennaConfig.select];

    /*  Avoid indexing out of range */
    if (pStep->antennaPermIdx < antennaEntry->numPerm)
    {
        /* Find the proper permutation pattern based on generic config and random step related selection.
            The same table is used, but we index the entries differently */
        uint8_t apn = antennaPermutation[pStep->antennaPermIdx * antennaEntry->increment].apn;

        /* We reuse the same permutation table for 1:1, 1:N, N:1 and 2:2,
            but only use the appropriate number of entries from each row */
        for(uint8_t k1=0; k1<antennaEntry->numPath; k1++)
        {
            uint8_t apm = antennaEntry->ap[pCmd->mode.role].apn;

            /* Decode bitfields */
            uint8_t k2 = DECODE_ANTENNA(apn, k1);
            uint8_t k3 = DECODE_ANTENNA(apm, k2);

            /* Antenna sequence is stored as [7:6]=A4, [5:4]=A3, [3:2]=A2, [1:0]=A1.
                The PBE will decode it accordingly. */
            pStep->antennaSequence |= ENCODE_ANTENNA(k3, k1);
        }
    }

    /* Look up the frequency offset actuation error (only used by mode 0) */
    pStep->foffErr = (uint16_t) foffErrorLut[pStep->channelIdx] ;

    /* Return with processed step */
    return pStep;
}

/*
 *  ======== Fetch the next step result container in the list and return a pointer to it ========
 */
static RCL_CmdBleCs_StepResult* RCL_Handler_BLE_CS_fetchNextStepResult(RCL_CmdBleCs *pCmd)
{
    return &pCmd->results[pCmd->output->nResultsRead];
}

/*
 *  ======== RCL_Handler_BLE_CS_findPbeErrorEndStatus ========
 */
static RCL_CommandStatus RCL_Handler_BLE_CS_findPbeErrorEndStatus(uint16_t pbeEndStatus)
{
    RCL_CommandStatus status;
    switch (pbeEndStatus)
    {
    case PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_RXF:
        status = RCL_CommandStatus_Error_RxFifo;
        break;
    case PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_TXF:
        status = RCL_CommandStatus_Error_TxFifo;
        break;
    case PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_SYNTH:
        status = RCL_CommandStatus_Error_Synth;
        break;
    case PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_STOP:
        status = RCL_Scheduler_findStopStatus(RCL_StopType_Hard);
        break;
     case PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_PAR:
        status = RCL_CommandStatus_Error_Param;
        break;
    case PBE_COMMON_RAM_ENDCAUSE_STAT_ERR_BADOP:
        status = RCL_CommandStatus_Error_UnknownOp;
        break;
    default:
        Log_printf(RclCore, Log_ERROR, "Unexpected error 0x%04X from PBE", pbeEndStatus);
        status = RCL_CommandStatus_Error;
        break;
    }
    return status;
}

/*
 *  ======== RCL_Handler_BLE_CS ========
 */
RCL_Events RCL_Handler_BLE_CS(RCL_Command *cmd, LRF_Events lrfEvents, RCL_Events rclEventsIn)
{
    RCL_CmdBleCs *pCmd = (RCL_CmdBleCs *) cmd;
    RCL_Events rclEvents = {.value = 0};

    if (rclEventsIn.setup != 0)
    {
        uint32_t earliestStartTime;

        /* Start by enabling refsys */
        earliestStartTime = LRF_enableSynthRefsys();

        /* Check if valid PLLDIV0 synth setting is used. */
        /* BLE CS currently supports only either 3 => FREF0=16MHz or 12 => FREF0=4MHz */
        uint32_t plldiv0 = (HWREG(LRFDRFE_BASE + LRFDRFE_O_PRE0) & LRFDRFE_PRE0_PLLDIV0_M) >> LRFDRFE_PRE0_PLLDIV0_S;
        if ((plldiv0 != 3U) && (plldiv0 != 12U))
        {
            /* Override to use PLLDIV0=12, gives FREF0=4MHz */
            Log_printf(RclCore, Log_WARNING, "Unsupported RFE_PRE0_PLLDIV0 synth setting detected. Will override to use 12 (FREF=4MHz)");
            HWREG(LRFDRFE_BASE + LRFDRFE_O_PRE0) = ((12U << LRFDRFE_PRE0_PLLDIV0_S) & LRFDRFE_PRE0_PLLDIV0_M) |
                                                   ((12U << LRFDRFE_PRE0_PLLDIV1_S) & LRFDRFE_PRE0_PLLDIV1_M);
        }

        /* Mark as active */
        cmd->status = RCL_CommandStatus_Active;

        /* Default end status */
        bleCsHandlerState.common.endStatus = RCL_CommandStatus_Finished;

        /* Enable radio */
        LRF_enable();

        RCL_CommandStatus startTimeStatus = RCL_Scheduler_setStartStopTimeEarliestStart(cmd, earliestStartTime);
        if (startTimeStatus >= RCL_CommandStatus_Finished)
        {
             cmd->status = startTimeStatus;
             rclEvents.lastCmdDone = 1;
        }
        else
        {
             /* Enable interrupts (TxCtrlAck => "S2R samples available") */
             HWREG(LRFDDBELL_BASE + LRFDDBELL_O_IMASK0) = HWREG_READ_LRF(LRFDDBELL_BASE + LRFDDBELL_O_IMASK0) | (LRF_EventOpDone.value | LRF_EventOpError.value | LRF_EventTxCtrlAck.value |
                              LRF_EventRxfifo.value);

             #ifdef PG2
             /* CDDS BUG00003 - Temporarily workaround */
             HWREG(LRFDDBELL_BASE + LRFDDBELL_O_IMASK0) = HWREG_READ_LRF(LRFDDBELL_BASE + LRFDDBELL_O_IMASK0) | LRF_EventTxfifo.value;
             #endif

             /* Initialize BLE CS specific registers and FIFO */
             RCL_Handler_BLE_CS_preprocessCommand(pCmd);

             /* Configure the BUFRAM fifo size and interrupts */
             RCL_Handler_BLE_CS_configureTxRxFIFO(pCmd);

             /* Configure S2R size and mode */
             RCL_Handler_BLE_CS_configureS2R(pCmd);

             /* Configure S2R size and mode */
             RCL_Handler_BLE_CS_configureS2R(pCmd);

             /* Fill the first batch of step descriptors into the BUFRAM TX fifo */
             RCL_Handler_BLE_CS_fillTxBuffer(pCmd);

             /* Post command */
             LRF_waitForTopsmReady();
             HWREGH(LRFD_BUFRAM_BASE + PBE_COMMON_RAM_O_MSGBOX) = 0;
             HWREG(LRFDPBE_BASE + LRFDPBE_O_API) = PBE_BLE_CS_REGDEF_API_OP_BLE_CS;

             /* Forward fill more steps */
             RCL_Handler_BLE_CS_fillTxBuffer(pCmd);
         }
     }

     if (cmd->status == RCL_CommandStatus_Active)
     {
         RCL_Handler_BLE_CS_readStatistics(pCmd);

         if (rclEventsIn.timerStart != 0)
         {
             rclEvents.cmdStarted = 1;
         }
         if (lrfEvents.rxfifo != 0)
         {
             /* Read the available results */
             RCL_Handler_BLE_CS_readRxBuffer(pCmd);
             RCL_Handler_BLE_CS_fillTxBuffer(pCmd);
         }
#ifdef PG2
         /* CDDS BUG00003, issue with FIFO prevents us to simultaneously access it from CM0 and PBE. */
         if (lrfEvents.txfifo != 0)
         {
            /* Fill the tx fifo with more steps */
            RCL_Handler_BLE_CS_fillTxBuffer(pCmd);
         }
#endif
         if (lrfEvents.txCtrlAck != 0)
         {
             /* Read S2R samples */
             RCL_Handler_BLE_CS_readS2RSamples(pCmd);
         }
         if (lrfEvents.opDone != 0)
         {
             /* Drain the rx fifo of the final batch of results */
             RCL_Handler_BLE_CS_readRxBuffer(pCmd);

             cmd->status = bleCsHandlerState.common.endStatus;
             rclEvents.lastCmdDone = 1;
         }
         else if (lrfEvents.opError != 0)
         {
             /* Drain the rx fifo of the final batch of results */
             RCL_Handler_BLE_CS_readRxBuffer(pCmd);

             RCL_CommandStatus endStatus = bleCsHandlerState.common.endStatus;
             if (endStatus == RCL_CommandStatus_Finished)
             {
                 cmd->status = RCL_Handler_BLE_CS_findPbeErrorEndStatus(HWREGH(LRFD_BUFRAM_BASE + PBE_COMMON_RAM_O_ENDCAUSE));
             }
             else
             {
                 cmd->status = endStatus;
             }
             rclEvents.lastCmdDone = 1;
         }
         else
         {
             /* Other events need to be handled unconditionally */
         }
    }

    if (rclEvents.lastCmdDone != 0)
    {
      LRF_disable();
      LRF_disableSynthRefsys();
    }
    return rclEvents;
}
