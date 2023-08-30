// SETTINGS FOR PHY BASED ON RADIO CONTROL LAYER (SOURCE FILE)
//
//
// CODE EXPORT INFORMATION
// This file is generated
//
// Tool name                   SmartRF Studio 8
// Tool version                0.6.0.263 INTERNAL
//
//
// WORKSPACE INFORMATION
//
// Workspace file              srf_cli.workspace
// Device                      CC2340R5
//     Package                 QFN40 5x5 RKP
//     Revision(s)             B (2.0)
// SDK                         SimpleLink CC23x0 SDK 7.20.00.00
// Board                       LP-EM-CC2340R5
// PHY                         2.4 GHz - Proprietary - 500 kbps, 125 kHz dev, GFSK
// PHY abbreviation            gfsk_500_kbps
//
//
// PHY PROPERTIES
//
// Run-time properties:
//     Frequency               2440.00000 MHz
//     TX output power         5.0 dBm 
// Start of packet:
//     Synchronization word    0x930B51DE 
// Packet data:
//     Whitening               Disabled 
//     Payload length          Variable, 8-bit length field 

#include "rcl_settings_gfsk_500_kbps.h"
#include DeviceFamily_constructPath(rf_patches/lrf_pbe_binary_generic.h)
#include DeviceFamily_constructPath(rf_patches/lrf_mce_binary_genfsk.h)
#include DeviceFamily_constructPath(rf_patches/lrf_rfe_binary_genfsk.h)


// Configuration: Common
static const uint32_t LRF_commonRegConfig[] =
{
    0x00000055,                                 // Segment length = 85
    0x0000A002,                                 //     Data structure 32-bit region (start byte position = 0, count = 3)
    (uint32_t) &LRF_swConfigGfsk500Kbps,        //         LRF_swParam : swConfig
    (uint32_t) &LRF_txPowerTableGfsk500Kbps,    //         LRF_swParam : txPowerTable
    (uint32_t) &(fcfg->appTrims),               //         LRF_swParam : trimDef
    0x14482001,                                 //     HW 32-bit region (start address = 0x1448, count = 2)
    0x00000037,                                 //         LRFDPBE.MDMCMDPAR1                  LRFDPBE.MDMCMDPAR0
    0x0000AAAA,                                 //         < GAP >                             LRFDPBE.MDMCMDPAR2
    0x10A00002,                                 //     HW zero region (start address = 0x10A0, count = 3)
    0x10AC1006,                                 //     HW 16-bit region (start address = 0x10AC, count = 7)
    0x00001021,                                 //         LRFDPBE.PHACFG                      LRFDPBE.POLY1H
    0x00C40021,                                 //         LRFDPBE.FCFG1                       LRFDPBE.FCFG0
    0x00800086,                                 //         LRFDPBE.FCFG3                       LRFDPBE.FCFG2
    0x00000043,                                 //         -                                   LRFDPBE.FCFG4
    0x14682000,                                 //     HW 32-bit region (start address = 0x1468, count = 1)
    0x00020004,                                 //         LRFDPBE.TXFWBTHRS                   LRFDPBE.RXFRBTHRS
    0x10DC1001,                                 //     HW 16-bit region (start address = 0x10DC, count = 2)
    0x000B0002,                                 //         LRFDPBE.TIMPRE                      LRFDPBE.TIMCTL
    0x00003004,                                 //     HW sparse region (address/value pairs, count = 5)
    0x20C00003,                                 //         LRFDMDM.ADCDIGCONF
    0x2134001F,                                 //         LRFDMDM.DEMSWQU0
    0x30880000,                                 //         LRFDRFE.RSSIOFFSET
    0x31201820,                                 //         LRFDRFE.MISC0
    0x31300C07,                                 //         LRFDRFE.PHEDISC
    0x24642000,                                 //     HW 32-bit region (start address = 0x2464, count = 1)
    0x3D1F3D1F,                                 //         LRFDMDM.MODSYMMAP1                  LRFDMDM.MODSYMMAP0
    0x20D41002,                                 //     HW 16-bit region (start address = 0x20D4, count = 3)
    0x000C2000,                                 //         LRFDMDM.BAUDPRE                     LRFDMDM.BAUD
    0x00000000,                                 //         -                                   LRFDMDM.MODMAIN
    0x20E41012,                                 //     HW 16-bit region (start address = 0x20E4, count = 19)
    0x00F40019,                                 //         LRFDMDM.DEMMISC2                    LRFDMDM.DEMMISC1
    0x00004682,                                 //         LRFDMDM.DEMIQMC0                    LRFDMDM.DEMMISC3
    0x06708D83,                                 //         LRFDMDM.DEMCODC0                    LRFDMDM.DEMDSBU
    0x01860000,                                 //         LRFDMDM.DEMFEXB0                    LRFDMDM.DEMFIDC0
    0x00050004,                                 //         LRFDMDM.DEMFIFE0                    LRFDMDM.DEMDSXB0
    0x7C200400,                                 //         LRFDMDM.DEMMAFI1                    LRFDMDM.DEMMAFI0
    0x002F00C2,                                 //         LRFDMDM.DEMC1BE0                    LRFDMDM.DEMMAFI2
    0x027F2727,                                 //         LRFDMDM.DEMC1BE2                    LRFDMDM.DEMC1BE1
    0x40000A18,                                 //         LRFDMDM.SPARE1                      LRFDMDM.SPARE0
    0x00000000,                                 //         -                                   LRFDMDM.SPARE2
    0x24D02000,                                 //     HW 32-bit region (start address = 0x24D0, count = 1)
    0x06700630,                                 //         LRFDMDM.VITBRMETRIC76               LRFDMDM.VITBRMETRIC54
    0x30941001,                                 //     HW 16-bit region (start address = 0x3094, count = 2)
    0x34F21307,                                 //         LRFDRFE.SPARE0                      LRFDRFE.MAGNCTL1
    0x30A01006,                                 //     HW 16-bit region (start address = 0x30A0, count = 7)
    0x0AB03F13,                                 //         LRFDRFE.SPARE3                      LRFDRFE.SPARE2
    0x7C000000,                                 //         LRFDRFE.SPARE5                      LRFDRFE.SPARE4
    0x0006000A,                                 //         LRFDRFE.IFAMPRFLDO                  LRFDRFE.LNA
    0x00000000,                                 //         -                                   LRFDRFE.PA0
    0x30C40005,                                 //     HW zero region (start address = 0x30C4, count = 6)
    0x30E4100C,                                 //     HW 16-bit region (start address = 0x30E4, count = 13)
    0x00000200,                                 //         LRFDRFE.DCO                         LRFDRFE.ATSTREFH
    0x00000008,                                 //         LRFDRFE.DIVLDO                      LRFDRFE.DIV
    0x00000000,                                 //         LRFDRFE.DCOLDO0                     LRFDRFE.TDCLDO
    0x0F0E0000,                                 //         LRFDRFE.PRE0                        LRFDRFE.DCOLDO1
    0x06050000,                                 //         LRFDRFE.PRE2                        LRFDRFE.PRE1
    0x40080603,                                 //         LRFDRFE.CAL0                        LRFDRFE.PRE3
    0x00007F00,                                 //         -                                   LRFDRFE.CAL1
    0x31381002,                                 //     HW 16-bit region (start address = 0x3138, count = 3)
    0x047FDF7F,                                 //         LRFDRFE.PLLMON1                     LRFDRFE.PLLMON0
    0x00001804,                                 //         -                                   LRFDRFE.MOD0
    0x20206005,                                 //     RAM 32-bit region (start address = 0x2020, count = 6)
    0x00B40000,                                 //         PBE_GENERIC_RAM.SYNTHCALTIMEOUT     PBE_GENERIC_RAM.PHY
    0x00100088,                                 //         PBE_GENERIC_RAM.NUMCRCBITS          PBE_GENERIC_RAM.PKTCFG
    0x00024BB0,                                 //         PBE_GENERIC_RAM.EXTRABYTES          PBE_GENERIC_RAM.FIFOCFG
    0x00000000,                                 //         < GAP >                             PBE_GENERIC_RAM.WHITEINIT
    0xFFFF0000,                                 //         PBE_GENERIC_RAM.CRCINITH            PBE_GENERIC_RAM.CRCINITL
    0x00000100,                                 //         PBE_GENERIC_RAM.LENOFFSET           PBE_GENERIC_RAM.LENCFG
    0x68046005,                                 //     RAM 32-bit region (start address = 0x6804, count = 6)
    0x03000012,                                 //         RFE_COMMON_RAM.TDCCAL0              RFE_COMMON_RAM.SYNTHCTL
    0x00100000,                                 //         RFE_COMMON_RAM.TDCCAL2              RFE_COMMON_RAM.TDCCAL1
    0x569B0000,                                 //         RFE_COMMON_RAM.K1LSB                RFE_COMMON_RAM.TDCPLL
    0x02BE010A,                                 //         RFE_COMMON_RAM.K2BL                 RFE_COMMON_RAM.K1MSB
    0x132C0074,                                 //         RFE_COMMON_RAM.K3BL                 RFE_COMMON_RAM.K2AL
    0x916F07AB,                                 //         RFE_COMMON_RAM.K5                   RFE_COMMON_RAM.K3AL
    0x68206005,                                 //     RAM 32-bit region (start address = 0x6820, count = 6)
    0x00000000,                                 //         RFE_COMMON_RAM.RTRIMMIN             RFE_COMMON_RAM.RTRIMOFF
    0x48080008,                                 //         RFE_COMMON_RAM.DIVF                 RFE_COMMON_RAM.DIVI
    0x00000000,                                 //         RFE_COMMON_RAM.DIVLDOF              RFE_COMMON_RAM.DIVLDOI
    0x00470014,                                 //         RFE_COMMON_RAM.LDOSETTLE            RFE_COMMON_RAM.DIVLDOIOFF
    0x0005002E,                                 //         RFE_COMMON_RAM.DCOSETTLE            RFE_COMMON_RAM.CHRGSETTLE
    0x0000FE00,                                 //         RFE_COMMON_RAM.IFAMPRFLDODEFAULT    RFE_COMMON_RAM.IFAMPRFLDOTX
    0x00007000,                                 //     RAM sparse region (address/value pairs, count = 1)
    0x683E005A,                                 //         RFE_COMMON_RAM.PHYRSSIOFFSET
    0x68425001,                                 //     RAM 16-bit region (start address = 0x6842, count = 2)
    0x0001002E                                  //         RFE_COMMON_RAM.AGCINFO              RFE_COMMON_RAM.SPARE1SHADOW
};


// LRF register configuration list
static const LRF_RegConfigList LRF_regConfigList = {
    .numEntries = 1,
    .entries = {
        (LRF_ConfigWord*) LRF_commonRegConfig 
    }
};

// LRF_TxShape data structure
const LRF_TxShape LRF_shapeBaseGfsk05Gfsk500Kbps = {
    .scale                 = 0x0F183,
    .numCoeff              = 0x0014,
    .coeff                 = { 0x01, 0x02, 0x03, 0x06, 0x0A, 0x11, 0x1A, 0x27, 0x37, 0x4B, 0x62, 0x7B, 0x94, 0xAD, 0xC4, 0xD8, 0xE7, 0xF3, 0xFB, 0xFF } 
};

// LRF_SwConfig data structure
const LRF_SwConfig LRF_swConfigGfsk500Kbps = {
    .rxIntFrequency        = 500000,
    .rxFrequencyOffset     = 0,
    .txFrequencyOffset     = 500000,
    .modFrequencyDeviation = 0x0001E848,
    .txShape               = &LRF_shapeBaseGfsk05Gfsk500Kbps,
    .bwIndex               = 0x00,
    .bwIndexDither         = 0x00 
};

// LRF_TxPowerTable data structure
const LRF_TxPowerTable LRF_txPowerTableGfsk500Kbps = {
    .numEntries            = 0x0000000E,
    .powerTable            = {
        { .power = { .fraction = 0, .dBm = -20 }, .tempCoeff = 0, .value = { .reserved = 0, .ib = 18, .gain = 0, .mode = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = -16 }, .tempCoeff = 0, .value = { .reserved = 0, .ib = 20, .gain = 1, .mode = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = -12 }, .tempCoeff = 5, .value = { .reserved = 0, .ib = 17, .gain = 3, .mode = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = -8 }, .tempCoeff = 12, .value = { .reserved = 0, .ib = 17, .gain = 4, .mode = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = -4 }, .tempCoeff = 25, .value = { .reserved = 0, .ib = 17, .gain = 5, .mode = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 0 }, .tempCoeff = 40, .value = { .reserved = 0, .ib = 19, .gain = 6, .mode = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 1 }, .tempCoeff = 65, .value = { .reserved = 0, .ib = 30, .gain = 6, .mode = 0, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 2 }, .tempCoeff = 41, .value = { .reserved = 0, .ib = 39, .gain = 4, .mode = 1, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 3 }, .tempCoeff = 43, .value = { .reserved = 0, .ib = 31, .gain = 5, .mode = 1, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 4 }, .tempCoeff = 50, .value = { .reserved = 0, .ib = 37, .gain = 5, .mode = 1, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 5 }, .tempCoeff = 55, .value = { .reserved = 0, .ib = 27, .gain = 6, .mode = 1, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 6 }, .tempCoeff = 75, .value = { .reserved = 0, .ib = 38, .gain = 6, .mode = 1, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 7 }, .tempCoeff = 80, .value = { .reserved = 0, .ib = 25, .gain = 7, .mode = 1, .noIfampRfLdoBypass = 0 } },
        { .power = { .fraction = 0, .dBm = 8 }, .tempCoeff = 180, .value = { .reserved = 0, .ib = 63, .gain = 7, .mode = 1, .noIfampRfLdoBypass = 0 } } 
    }
};

// LRF_Config data structure
const LRF_Config LRF_configGfsk500Kbps = {
    .pbeImage              = (const LRF_TOPsmImage*) LRF_PBE_binary_generic,
    .mceImage              = (const LRF_TOPsmImage*) LRF_MCE_binary_genfsk,
    .rfeImage              = (const LRF_TOPsmImage*) LRF_RFE_binary_genfsk,
    .regConfigList         = &LRF_regConfigList 
};
