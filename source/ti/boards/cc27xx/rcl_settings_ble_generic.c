// SETTINGS FOR PHY BASED ON RADIO CONTROL LAYER (SOURCE FILE)
//
// Usage                  Generic
//
//
// CODE EXPORT INFORMATION
// This file is generated
//
// Tool name              SmartRF Studio 8
// Tool version           0.6.0.316 INTERNAL
//
//
// WORKSPACE INFORMATION
//
// Workspace file         srf_cli.workspace
// Device                 CC2745P10Q1
//     Package            QFN40 6x6 RKP
//     Revision(s)        A (1.0), C (1.1)
// SDK                    SimpleLink LPF3 SDK 7.40.00.57
// Board                  LP-EM-CC2745R10
// PHY                    2.4 GHz - Bluetooth 5, LE - 1 Mbps / 2 Mbps / Coded
// PHY abbreviation       ble
//
//
// PHY PROPERTIES
//
// Run-time properties:
//     Sub-PHY            1 Mbps 
//     Channel            17 (2440 MHz) 
//     TX output power    5.0 dBm 

#include "rcl_settings_ble_generic.h"
#include DeviceFamily_constructPath(rf_patches/lrf_pbe_binary_generic.h)
#include DeviceFamily_constructPath(rf_patches/lrf_mce_binary_ble5_phac.h)
#include DeviceFamily_constructPath(rf_patches/lrf_rfe_binary_ble5.h)


// Configuration: Common
static const uint32_t LRF_commonRegConfig[] =
{
    0x00000040,                          // Segment length = 64
    0x0004A001,                          //     Data structure 32-bit region (start byte position = 4, count = 2)
    (uint32_t) &LRF_txPowerTableBle,     //         LRF_swParam : txPowerTable
    (uint32_t) &(fcfg->appTrims),        //         LRF_swParam : trimDef
    0x0000300B,                          //     HW sparse region (address/value pairs, count = 12)
    0x10940000,                          //         LRFDPBE.MDMCMDPAR1
    0x20C00003,                          //         LRFDMDM.ADCDIGCONF
    0x20C8001F,                          //         LRFDMDM.MODSYMMAP0
    0x20DC0000,                          //         LRFDMDM.MODMAIN
    0x20F00000,                          //         LRFDMDM.DEMIQMC0
    0x20F80080,                          //         LRFDMDM.DEMDSBU2
    0x21000000,                          //         LRFDMDM.DEMFIDC0
    0x22A00003,                          //         LRFDMDM.BAUDCOMP
    0x30840000,                          //         LRFDRFE.MAGNTHRCFG
    0x308C0000,                          //         LRFDRFE.RSSIOFFSET
    0x3120D820,                          //         LRFDRFE.MISC0
    0x31300C07,                          //         LRFDRFE.PHEDISC
    0x14502004,                          //     HW 32-bit region (start address = 0x1450, count = 5)
    0x22000000,                          //         LRFDPBE.POLY0H                      LRFDPBE.POLY0L
    0x00065B00,                          //         LRFDPBE.POLY1H                      LRFDPBE.POLY1L
    0x00210000,                          //         LRFDPBE.FCFG0                       LRFDPBE.PHACFG
    0x008600C4,                          //         LRFDPBE.FCFG2                       LRFDPBE.FCFG1
    0x00440080,                          //         LRFDPBE.FCFG4                       LRFDPBE.FCFG3
    0x14682000,                          //     HW 32-bit region (start address = 0x1468, count = 1)
    0x00020004,                          //         LRFDPBE.TXFWBTHRS                   LRFDPBE.RXFRBTHRS
    0x10DC1001,                          //     HW 16-bit region (start address = 0x10DC, count = 2)
    0x0B0B0202,                          //         LRFDPBE.TIMPRE                      LRFDPBE.TIMCTL
    0x30AC1001,                          //     HW 16-bit region (start address = 0x30AC, count = 2)
    0x7C000000,                          //         LRFDRFE.SPARE5                      LRFDRFE.SPARE4
    0x345C2000,                          //     HW 32-bit region (start address = 0x345C, count = 1)
    0x29400006,                          //         LRFDRFE.PA0                         LRFDRFE.IFAMPRFLDO
    0x30C40005,                          //     HW zero region (start address = 0x30C4, count = 6)
    0x30E4100C,                          //     HW 16-bit region (start address = 0x30E4, count = 13)
    0x00000200,                          //         LRFDRFE.DCO                         LRFDRFE.ATSTREFH
    0x00000008,                          //         LRFDRFE.DIVLDO                      LRFDRFE.DIV
    0x00000000,                          //         LRFDRFE.DCOLDO0                     LRFDRFE.TDCLDO
    0x07060000,                          //         LRFDRFE.PRE0                        LRFDRFE.DCOLDO1
    0x06050000,                          //         LRFDRFE.PRE2                        LRFDRFE.PRE1
    0x40080603,                          //         LRFDRFE.CAL0                        LRFDRFE.PRE3
    0x00007F00,                          //         -                                   LRFDRFE.CAL1
    0x31381002,                          //     HW 16-bit region (start address = 0x3138, count = 3)
    0x047FDF7F,                          //         LRFDRFE.PLLMON1                     LRFDRFE.PLLMON0
    0x00001804,                          //         -                                   LRFDRFE.MOD0
    0x2022500A,                          //     RAM 16-bit region (start address = 0x2022, count = 11)
    0x009000B4,                          //         PBE_GENERIC_RAM.PKTCFG              PBE_GENERIC_RAM.SYNTHCALTIMEOUT
    0x03B00018,                          //         PBE_GENERIC_RAM.FIFOCFG             PBE_GENERIC_RAM.NUMCRCBITS
    0x00510000,                          //         PBE_GENERIC_RAM.WHITEINIT           PBE_GENERIC_RAM.EXTRABYTES
    0x55000000,                          //         PBE_GENERIC_RAM.CRCINITL            < GAP >
    0x01085555,                          //         PBE_GENERIC_RAM.LENCFG              PBE_GENERIC_RAM.CRCINITH
    0x00000000,                          //         -                                   PBE_GENERIC_RAM.LENOFFSET
    0x68046005,                          //     RAM 32-bit region (start address = 0x6804, count = 6)
    0x03000012,                          //         RFE_COMMON_RAM.TDCCAL0              RFE_COMMON_RAM.SYNTHCTL
    0x00100000,                          //         RFE_COMMON_RAM.TDCCAL2              RFE_COMMON_RAM.TDCCAL1
    0x569B0400,                          //         RFE_COMMON_RAM.K1LSB                RFE_COMMON_RAM.TDCPLL
    0x012D010A,                          //         RFE_COMMON_RAM.K2BL                 RFE_COMMON_RAM.K1MSB
    0x132C0034,                          //         RFE_COMMON_RAM.K3BL                 RFE_COMMON_RAM.K2AL
    0x916F07AB,                          //         RFE_COMMON_RAM.K5                   RFE_COMMON_RAM.K3AL
    0x68206005,                          //     RAM 32-bit region (start address = 0x6820, count = 6)
    0x00080000,                          //         RFE_COMMON_RAM.RTRIMMIN             RFE_COMMON_RAM.RTRIMOFF
    0x48080008,                          //         RFE_COMMON_RAM.DIVF                 RFE_COMMON_RAM.DIVI
    0x00000000,                          //         RFE_COMMON_RAM.DIVLDOF              RFE_COMMON_RAM.DIVLDOI
    0x00470014,                          //         RFE_COMMON_RAM.LDOSETTLE            RFE_COMMON_RAM.DIVLDOIOFF
    0x0005002E,                          //         RFE_COMMON_RAM.DCOSETTLE            RFE_COMMON_RAM.CHRGSETTLE
    0x0000FE00,                          //         RFE_COMMON_RAM.IFAMPRFLDODEFAULT    RFE_COMMON_RAM.IFAMPRFLDOTX
    0x68444001                           //     RAM zero region (start address = 0x6844, count = 2)
};

// Configuration: Sub-PHY = 1 Mbps, 2 Mbps
static const uint32_t LRF_subPhy1Mbps2MbpsRegConfig[] =
{
    0x00024015,                          // Segment length = 21
    0x00003006,                          //     HW sparse region (address/value pairs, count = 7)
    0x1098AAAA,                          //         LRFDPBE.MDMCMDPAR2
    0x20B80015,                          //         LRFDMDM.SYSTIMEVTMUX0
    0x20D8000C,                          //         LRFDMDM.BAUDPRE
    0x20F40087,                          //         LRFDMDM.DEMDSBU
    0x2134005F,                          //         LRFDMDM.DEMSWQU0
    0x232C7F00,                          //         LRFDMDM.DEMC1BE12
    0x30A81F40,                          //         LRFDRFE.SPARE3
    0x20E41001,                          //     HW 16-bit region (start address = 0x20E4, count = 2)
    0x05FB0001,                          //         LRFDMDM.DEMMISC2                    LRFDMDM.DEMMISC1
    0x21081006,                          //     HW 16-bit region (start address = 0x2108, count = 7)
    0x04000005,                          //         LRFDMDM.DEMMAFI0                    LRFDMDM.DEMFIFE0
    0x00C17B20,                          //         LRFDMDM.DEMMAFI2                    LRFDMDM.DEMMAFI1
    0x7F27000F,                          //         LRFDMDM.DEMC1BE1                    LRFDMDM.DEMC1BE0
    0x0000017F,                          //         -                                   LRFDMDM.DEMC1BE2
    0x24D42000,                          //     HW 32-bit region (start address = 0x24D4, count = 1)
    0x00540004,                          //         LRFDMDM.DEMD2XB0                    LRFDMDM.DEMDSXB0
    0x30B4B000,                          //     HW masked region (mask/value pairs, count = 1)
    0xFF0F0002,                          //         LRFDRFE.LNA
    0x68406000,                          //     RAM 32-bit region (start address = 0x6840, count = 1)
    0x00000045                           //         RFE_COMMON_RAM.AGCINFO              RFE_COMMON_RAM.SPARE1SHADOW
};

// Configuration: Sub-PHY = 1 Mbps, Coded
static const uint32_t LRF_subPhy1MbpsCodedRegConfig[] =
{
    0x00014005,                          // Segment length = 5
    0x0000A000,                          //     Data structure 32-bit region (start byte position = 0, count = 1)
    (uint32_t) &LRF_swConfig1MbpsBle,    //         LRF_swParam : swConfig
    0x00003001,                          //     HW sparse region (address/value pairs, count = 2)
    0x20D44000,                          //         LRFDMDM.BAUD
    0x20E00387                           //         LRFDMDM.DEMMISC0
};

// Configuration: Sub-PHY = 2 Mbps, Coded
static const uint32_t LRF_subPhy2MbpsCodedRegConfig[] =
{
    0x00030004,                          // Segment length = 4
    0x00003000,                          //     HW sparse region (address/value pairs, count = 1)
    0x21040A24,                          //         LRFDMDM.DEMFEXB0
    0x00007000,                          //     RAM sparse region (address/value pairs, count = 1)
    0x683C004C                           //         RFE_COMMON_RAM.PHYRSSIOFFSET
};

// Configuration: Sub-PHY = 1 Mbps
static const uint32_t LRF_subPhy1MbpsRegConfig[] =
{
    0x0003400D,                          // Segment length = 13
    0x00003007,                          //     HW sparse region (address/value pairs, count = 8)
    0x10908007,                          //         LRFDPBE.MDMCMDPAR0
    0x10E803E8,                          //         LRFDPBE.TIMPER1
    0x20C40007,                          //         LRFDMDM.MODPRECTRL
    0x20EC1182,                          //         LRFDMDM.DEMMISC3
    0x20FC0F50,                          //         LRFDMDM.DEMCODC0
    0x21040224,                          //         LRFDMDM.DEMFEXB0
    0x21240A18,                          //         LRFDMDM.SPARE0
    0x30A4A246,                          //         LRFDRFE.SPARE2
    0x00007000,                          //     RAM sparse region (address/value pairs, count = 1)
    0x20200000,                          //         PBE_GENERIC_RAM.PHY
    0x683C6000,                          //     RAM 32-bit region (start address = 0x683C, count = 1)
    0x75F80050                           //         RFE_COMMON_RAM.SPARE0SHADOW         RFE_COMMON_RAM.PHYRSSIOFFSET
};

// Configuration: Sub-PHY = 2 Mbps
static const uint32_t LRF_subPhy2MbpsRegConfig[] =
{
    0x0001000E,                          // Segment length = 14
    0x0000A000,                          //     Data structure 32-bit region (start byte position = 0, count = 1)
    (uint32_t) &LRF_swConfig2MbpsBle,    //         LRF_swParam : swConfig
    0x00003007,                          //     HW sparse region (address/value pairs, count = 8)
    0x1090800F,                          //         LRFDPBE.MDMCMDPAR0
    0x20C40017,                          //         LRFDMDM.MODPRECTRL
    0x20D48000,                          //         LRFDMDM.BAUD
    0x20E00399,                          //         LRFDMDM.DEMMISC0
    0x20EC1081,                          //         LRFDMDM.DEMMISC3
    0x20FC0F70,                          //         LRFDMDM.DEMCODC0
    0x21240A2C,                          //         LRFDMDM.SPARE0
    0x30A4A357,                          //         LRFDRFE.SPARE2
    0x00007001,                          //     RAM sparse region (address/value pairs, count = 2)
    0x20200009,                          //         PBE_GENERIC_RAM.PHY
    0x683E55FA                           //         RFE_COMMON_RAM.SPARE0SHADOW
};

// Configuration: Sub-PHY = Coded
static const uint32_t LRF_subPhyCodedRegConfig[] =
{
    0x00020024,                          // Segment length = 36
    0x00003007,                          //     HW sparse region (address/value pairs, count = 8)
    0x10900097,                          //         LRFDPBE.MDMCMDPAR0
    0x10983C3C,                          //         LRFDPBE.MDMCMDPAR2
    0x20C40097,                          //         LRFDMDM.MODPRECTRL
    0x20D8A00C,                          //         LRFDMDM.BAUDPRE
    0x20F4008D,                          //         LRFDMDM.DEMDSBU
    0x20FC0670,                          //         LRFDMDM.DEMCODC0
    0x212401B0,                          //         LRFDMDM.SPARE0
    0x2134001F,                          //         LRFDMDM.DEMSWQU0
    0x20E41002,                          //     HW 16-bit region (start address = 0x20E4, count = 3)
    0x00CF0003,                          //         LRFDMDM.DEMMISC2                    LRFDMDM.DEMMISC1
    0x00003582,                          //         -                                   LRFDMDM.DEMMISC3
    0x24842002,                          //     HW 32-bit region (start address = 0x2484, count = 3)
    0x0000000C,                          //         LRFDMDM.DEMMAFI0                    LRFDMDM.DEMFIFE0
    0x00505014,                          //         LRFDMDM.DEMMAFI2                    LRFDMDM.DEMMAFI1
    0x7F7F001F,                          //         LRFDMDM.DEMC1BE1                    LRFDMDM.DEMC1BE0
    0x24D42000,                          //     HW 32-bit region (start address = 0x24D4, count = 1)
    0x00560024,                          //         LRFDMDM.DEMD2XB0                    LRFDMDM.DEMDSXB0
    0x22B41001,                          //     HW 16-bit region (start address = 0x22B4, count = 2)
    0x5C404040,                          //         LRFDMDM.DEMFB2P1                    LRFDMDM.DEMFB2P0
    0x25682001,                          //     HW 32-bit region (start address = 0x2568, count = 2)
    0xFD90030C,                          //         LRFDMDM.DEMPHAC1                    LRFDMDM.DEMPHAC0
    0x0000783C,                          //         LRFDMDM.DEMPHAC3                    LRFDMDM.DEMPHAC2
    0x23241002,                          //     HW 16-bit region (start address = 0x2324, count = 3)
    0x7F7F8804,                          //         LRFDMDM.DEMC1BE11                   LRFDMDM.DEMC1BE10
    0x00002C7F,                          //         -                                   LRFDMDM.DEMC1BE12
    0x344C2000,                          //     HW 32-bit region (start address = 0x344C, count = 1)
    0x34F21307,                          //         LRFDRFE.SPARE0                      LRFDRFE.MAGNCTL1
    0x30A41001,                          //     HW 16-bit region (start address = 0x30A4, count = 2)
    0x00B00013,                          //         LRFDRFE.SPARE3                      LRFDRFE.SPARE2
    0x30B4B000,                          //     HW masked region (mask/value pairs, count = 1)
    0xFF0F0006,                          //         LRFDRFE.LNA
    0x00007000,                          //     RAM sparse region (address/value pairs, count = 1)
    0x20200012,                          //         PBE_GENERIC_RAM.PHY
    0x68406000,                          //     RAM 32-bit region (start address = 0x6840, count = 1)
    0x0001001C                           //         RFE_COMMON_RAM.AGCINFO              RFE_COMMON_RAM.SPARE1SHADOW
};

// Configuration: Coded TX rate = S8 (8 symbols per bit = 125 kbps)
static const uint32_t LRF_codedTxRateS8RegConfig[] =
{
    0x00044002,                          // Segment length = 2
    0x00003000,                          //     HW sparse region (address/value pairs, count = 1)
    0x21280000                           //         LRFDMDM.SPARE1
};

// Configuration: Coded TX rate = S2 (2 symbols per bit = 500 kbps)
static const uint32_t LRF_codedTxRateS2RegConfig[] =
{
    0x00040002,                          // Segment length = 2
    0x00003000,                          //     HW sparse region (address/value pairs, count = 1)
    0x21280001                           //         LRFDMDM.SPARE1
};


// LRF register configuration list
static const LRF_RegConfigList LRF_regConfigList = {
    .numEntries = 9,
    .entries = {
        (LRF_ConfigWord*) LRF_commonRegConfig,
        (LRF_ConfigWord*) LRF_subPhy1Mbps2MbpsRegConfig,
        (LRF_ConfigWord*) LRF_subPhy1MbpsCodedRegConfig,
        (LRF_ConfigWord*) LRF_subPhy2MbpsCodedRegConfig,
        (LRF_ConfigWord*) LRF_subPhy1MbpsRegConfig,
        (LRF_ConfigWord*) LRF_subPhy2MbpsRegConfig,
        (LRF_ConfigWord*) LRF_subPhyCodedRegConfig,
        (LRF_ConfigWord*) LRF_codedTxRateS8RegConfig,
        (LRF_ConfigWord*) LRF_codedTxRateS2RegConfig 
    }
};

// LRF_TxShape data structure
const LRF_TxShape LRF_shapeBaseGfsk067Ble = {
    .scale                 = 0x0FDE2,
    .numCoeff              = 0x0011,
    .coeff                 = { 0x01, 0x02, 0x05, 0x0A, 0x14, 0x22, 0x37, 0x52, 0x71, 0x91, 0xB0, 0xCB, 0xE0, 0xEE, 0xF8, 0xFD, 0xFF } 
};

// LRF_TxShape data structure
const LRF_TxShape LRF_shapeBaseGfsk05Ble = {
    .scale                 = 0x0F183,
    .numCoeff              = 0x0014,
    .coeff                 = { 0x01, 0x02, 0x03, 0x06, 0x0A, 0x11, 0x1A, 0x27, 0x37, 0x4B, 0x62, 0x7B, 0x94, 0xAD, 0xC4, 0xD8, 0xE7, 0xF3, 0xFB, 0xFF } 
};

// LRF_SwConfig data structure
const LRF_SwConfig LRF_swConfig1MbpsBle = {
    .rxIntFrequency        = 1000000,
    .rxFrequencyOffset     = 0,
    .txFrequencyOffset     = 1000000,
    .modFrequencyDeviation = 0x0003D090,
    .txShape               = &LRF_shapeBaseGfsk067Ble,
    .bwIndex               = 0x00,
    .bwIndexDither         = 0x01 
};

// LRF_SwConfig data structure
const LRF_SwConfig LRF_swConfig2MbpsBle = {
    .rxIntFrequency        = 850000,
    .rxFrequencyOffset     = 0,
    .txFrequencyOffset     = 850000,
    .modFrequencyDeviation = 0x0007A120,
    .txShape               = &LRF_shapeBaseGfsk05Ble,
    .bwIndex               = 0x01,
    .bwIndexDither         = 0x01 
};

// LRF_TxPowerTable data structure
const LRF_TxPowerTable LRF_txPowerTableBle = {
    .numEntries            = 0x00000019,
    .powerTable            = {
        { .power = { .fraction = 0, .dBm = -20 }, .tempCoeff = 0, .value = { .ibBoost = 0, .ib = 18, .gain = 0, .mode = 0, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = -16 }, .tempCoeff = 3, .value = { .ibBoost = 0, .ib = 21, .gain = 1, .mode = 0, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = -12 }, .tempCoeff = 4, .value = { .ibBoost = 0, .ib = 18, .gain = 3, .mode = 0, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = -8 }, .tempCoeff = 20, .value = { .ibBoost = 0, .ib = 31, .gain = 3, .mode = 0, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = -4 }, .tempCoeff = 30, .value = { .ibBoost = 0, .ib = 28, .gain = 5, .mode = 0, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 0 }, .tempCoeff = 32, .value = { .ibBoost = 0, .ib = 34, .gain = 5, .mode = 1, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 1, .dBm = 0 }, .tempCoeff = 35, .value = { .ibBoost = 0, .ib = 37, .gain = 5, .mode = 1, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 1 }, .tempCoeff = 40, .value = { .ibBoost = 0, .ib = 41, .gain = 5, .mode = 1, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 1, .dBm = 1 }, .tempCoeff = 40, .value = { .ibBoost = 0, .ib = 47, .gain = 5, .mode = 1, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 2 }, .tempCoeff = 30, .value = { .ibBoost = 0, .ib = 53, .gain = 5, .mode = 1, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 1, .dBm = 2 }, .tempCoeff = 60, .value = { .ibBoost = 0, .ib = 38, .gain = 6, .mode = 1, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 3 }, .tempCoeff = 35, .value = { .ibBoost = 0, .ib = 47, .gain = 6, .mode = 1, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 1, .dBm = 3 }, .tempCoeff = 80, .value = { .ibBoost = 0, .ib = 27, .gain = 7, .mode = 1, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 4 }, .tempCoeff = 180, .value = { .ibBoost = 0, .ib = 63, .gain = 7, .mode = 1, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 1, .dBm = 4 }, .tempCoeff = 8, .value = { .ibBoost = 0, .ib = 29, .gain = 4, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 5 }, .tempCoeff = 10, .value = { .ibBoost = 0, .ib = 31, .gain = 4, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 1, .dBm = 5 }, .tempCoeff = 9, .value = { .ibBoost = 0, .ib = 33, .gain = 4, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 6 }, .tempCoeff = 10, .value = { .ibBoost = 0, .ib = 35, .gain = 4, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 1, .dBm = 6 }, .tempCoeff = 11, .value = { .ibBoost = 0, .ib = 37, .gain = 4, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 7 }, .tempCoeff = 15, .value = { .ibBoost = 0, .ib = 28, .gain = 5, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 1, .dBm = 7 }, .tempCoeff = 18, .value = { .ibBoost = 0, .ib = 30, .gain = 5, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 8 }, .tempCoeff = 20, .value = { .ibBoost = 0, .ib = 34, .gain = 5, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 9 }, .tempCoeff = 25, .value = { .ibBoost = 0, .ib = 22, .gain = 6, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 10 }, .tempCoeff = 40, .value = { .ibBoost = 0, .ib = 34, .gain = 6, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 12 }, .tempCoeff = 180, .value = { .ibBoost = 1, .ib = 63, .gain = 7, .mode = 2, .reserved = 0, .noIfampRfLdoBypass = 0 } } 
    }
};

// LRF_Config data structure
const LRF_Config LRF_configBle = {
    .pbeImage              = (const LRF_TOPsmImage*) LRF_PBE_binary_generic,
    .mceImage              = (const LRF_TOPsmImage*) LRF_MCE_binary_ble5_phac,
    .rfeImage              = (const LRF_TOPsmImage*) LRF_RFE_binary_ble5,
    .regConfigList         = &LRF_regConfigList 
};