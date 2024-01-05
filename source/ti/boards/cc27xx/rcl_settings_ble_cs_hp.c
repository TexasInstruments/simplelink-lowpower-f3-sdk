// SETTINGS FOR PHY BASED ON RADIO CONTROL LAYER (SOURCE FILE)
//
// Usage               Protocol stack
//
//
// CODE EXPORT INFORMATION
// This file is generated
//
// Tool name           SmartRF Studio 8
// Tool version        0.6.0.316 INTERNAL
//
//
// WORKSPACE INFORMATION
//
// Workspace file      srf_cli.workspace
// Device              CC2745P10Q1
//     Package         QFN40 6x6 RKP
//     Revision(s)     A (1.0), C (1.1)
// SDK                 SimpleLink LPF3 SDK 7.40.00.57
// Board               LP-EM-CC2745R10
// PHY                 2.4 GHz - Bluetooth 6, LE - Channel Sounding High Performance
// PHY abbreviation    ble_cs_hp

#include "rcl_settings_ble_cs_hp.h"
#include DeviceFamily_constructPath(rf_patches/lrf_pbe_binary_ble_cs.h)
#include DeviceFamily_constructPath(rf_patches/lrf_mce_binary_ble_cs.h)
#include DeviceFamily_constructPath(rf_patches/lrf_rfe_binary_ble_cs.h)


// Configuration: Common
static const uint32_t LRF_commonRegConfig[] =
{
    0x00000051,                             // Segment length = 81
    0x0000A002,                             //     Data structure 32-bit region (start byte position = 0, count = 3)
    (uint32_t) &LRF_swConfigBleCsHp,        //         LRF_swParam : swConfig
    (uint32_t) &LRF_txPowerTableBleCsHp,    //         LRF_swParam : txPowerTable
    (uint32_t) &(fcfg->appTrims),           //         LRF_swParam : trimDef
    0x0000300A,                             //     HW sparse region (address/value pairs, count = 11)
    0x1090800F,                             //         LRFDPBE.MDMCMDPAR0
    0x1098AAAA,                             //         LRFDPBE.MDMCMDPAR2
    0x20B80015,                             //         LRFDMDM.SYSTIMEVTMUX0
    0x20C00003,                             //         LRFDMDM.ADCDIGCONF
    0x20C8001F,                             //         LRFDMDM.MODSYMMAP0
    0x2134005F,                             //         LRFDMDM.DEMSWQU0
    0x22A00003,                             //         LRFDMDM.BAUDCOMP
    0x30840000,                             //         LRFDRFE.MAGNTHRCFG
    0x308C0000,                             //         LRFDRFE.RSSIOFFSET
    0x3120D820,                             //         LRFDRFE.MISC0
    0x31300C07,                             //         LRFDRFE.PHEDISC
    0x14502001,                             //     HW 32-bit region (start address = 0x1450, count = 2)
    0x22000000,                             //         LRFDPBE.POLY0H                      LRFDPBE.POLY0L
    0x00065B00,                             //         LRFDPBE.POLY1H                      LRFDPBE.POLY1L
    0x10B41004,                             //     HW 16-bit region (start address = 0x10B4, count = 5)
    0x00C40021,                             //         LRFDPBE.FCFG1                       LRFDPBE.FCFG0
    0x00800086,                             //         LRFDPBE.FCFG3                       LRFDPBE.FCFG2
    0x00000044,                             //         -                                   LRFDPBE.FCFG4
    0x14682000,                             //     HW 32-bit region (start address = 0x1468, count = 1)
    0x00020004,                             //         LRFDPBE.TXFWBTHRS                   LRFDPBE.RXFRBTHRS
    0x10DC1001,                             //     HW 16-bit region (start address = 0x10DC, count = 2)
    0x0B0B0202,                             //         LRFDPBE.TIMPRE                      LRFDPBE.TIMCTL
    0x20D41002,                             //     HW 16-bit region (start address = 0x20D4, count = 3)
    0x000C8000,                             //         LRFDMDM.BAUDPRE                     LRFDMDM.BAUD
    0x00000000,                             //         -                                   LRFDMDM.MODMAIN
    0x20E41010,                             //     HW 16-bit region (start address = 0x20E4, count = 17)
    0x00FB0001,                             //         LRFDMDM.DEMMISC2                    LRFDMDM.DEMMISC1
    0x00001081,                             //         LRFDMDM.DEMIQMC0                    LRFDMDM.DEMMISC3
    0x00800087,                             //         LRFDMDM.DEMDSBU2                    LRFDMDM.DEMDSBU
    0x00000F70,                             //         LRFDMDM.DEMFIDC0                    LRFDMDM.DEMCODC0
    0x00050224,                             //         LRFDMDM.DEMFIFE0                    LRFDMDM.DEMFEXB0
    0x7B200400,                             //         LRFDMDM.DEMMAFI1                    LRFDMDM.DEMMAFI0
    0x000F00C1,                             //         LRFDMDM.DEMC1BE0                    LRFDMDM.DEMMAFI2
    0x017F7F27,                             //         LRFDMDM.DEMC1BE2                    LRFDMDM.DEMC1BE1
    0x00000A2C,                             //         -                                   LRFDMDM.SPARE0
    0x24D42000,                             //     HW 32-bit region (start address = 0x24D4, count = 1)
    0x00540004,                             //         LRFDMDM.DEMD2XB0                    LRFDMDM.DEMDSXB0
    0x30A41006,                             //     HW 16-bit region (start address = 0x30A4, count = 7)
    0x1F40A357,                             //         LRFDRFE.SPARE3                      LRFDRFE.SPARE2
    0x7C000000,                             //         LRFDRFE.SPARE5                      LRFDRFE.SPARE4
    0x00060002,                             //         LRFDRFE.IFAMPRFLDO                  LRFDRFE.LNA
    0x00002940,                             //         -                                   LRFDRFE.PA0
    0x30C40005,                             //     HW zero region (start address = 0x30C4, count = 6)
    0x30E4100C,                             //     HW 16-bit region (start address = 0x30E4, count = 13)
    0x00000200,                             //         LRFDRFE.DCO                         LRFDRFE.ATSTREFH
    0x00000008,                             //         LRFDRFE.DIVLDO                      LRFDRFE.DIV
    0x00000000,                             //         LRFDRFE.DCOLDO0                     LRFDRFE.TDCLDO
    0x03030000,                             //         LRFDRFE.PRE0                        LRFDRFE.DCOLDO1
    0x06050000,                             //         LRFDRFE.PRE2                        LRFDRFE.PRE1
    0x40080603,                             //         LRFDRFE.CAL0                        LRFDRFE.PRE3
    0x00007F00,                             //         -                                   LRFDRFE.CAL1
    0x31381002,                             //     HW 16-bit region (start address = 0x3138, count = 3)
    0x047FDF7F,                             //         LRFDRFE.PLLMON1                     LRFDRFE.PLLMON0
    0x00001804,                             //         -                                   LRFDRFE.MOD0
    0x00007000,                             //     RAM sparse region (address/value pairs, count = 1)
    0x20200103,                             //         PBE_BLE_CS_RAM.MODE
    0x20224005,                             //     RAM zero region (start address = 0x2022, count = 6)
    0x202E5001,                             //     RAM 16-bit region (start address = 0x202E, count = 2)
    0x00500050,                             //         PBE_BLE_CS_RAM.TFM                  PBE_BLE_CS_RAM.TFCS
    0x20346000,                             //     RAM 32-bit region (start address = 0x2034, count = 1)
    0x00280028,                             //         PBE_BLE_CS_RAM.TIP2                 PBE_BLE_CS_RAM.TIP1
    0x68046005,                             //     RAM 32-bit region (start address = 0x6804, count = 6)
    0x03000010,                             //         RFE_COMMON_RAM.TDCCAL0              RFE_COMMON_RAM.SYNTHCTL
    0x00100000,                             //         RFE_COMMON_RAM.TDCCAL2              RFE_COMMON_RAM.TDCCAL1
    0x569B0000,                             //         RFE_COMMON_RAM.K1LSB                RFE_COMMON_RAM.TDCPLL
    0x0969010A,                             //         RFE_COMMON_RAM.K2BL                 RFE_COMMON_RAM.K1MSB
    0x5FDFA5A6,                             //         RFE_COMMON_RAM.K3BL                 RFE_COMMON_RAM.K2AL
    0x916F0BFB,                             //         RFE_COMMON_RAM.K5                   RFE_COMMON_RAM.K3AL
    0x68206005,                             //     RAM 32-bit region (start address = 0x6820, count = 6)
    0x00080000,                             //         RFE_COMMON_RAM.RTRIMMIN             RFE_COMMON_RAM.RTRIMOFF
    0x48080008,                             //         RFE_COMMON_RAM.DIVF                 RFE_COMMON_RAM.DIVI
    0x00000000,                             //         RFE_COMMON_RAM.DIVLDOF              RFE_COMMON_RAM.DIVLDOI
    0x00470014,                             //         RFE_COMMON_RAM.LDOSETTLE            RFE_COMMON_RAM.DIVLDOIOFF
    0x0005002E,                             //         RFE_COMMON_RAM.DCOSETTLE            RFE_COMMON_RAM.CHRGSETTLE
    0x0000FE00,                             //         RFE_COMMON_RAM.IFAMPRFLDODEFAULT    RFE_COMMON_RAM.IFAMPRFLDOTX
    0x68444001                              //     RAM zero region (start address = 0x6844, count = 2)
};


// LRF register configuration list
static const LRF_RegConfigList LRF_regConfigList = {
    .numEntries = 1,
    .entries = {
        (LRF_ConfigWord*) LRF_commonRegConfig 
    }
};

// LRF_TxShape data structure
const LRF_TxShape LRF_shapeBaseGfsk05BleCsHp = {
    .scale                 = 0x0F183,
    .numCoeff              = 0x0014,
    .coeff                 = { 0x01, 0x02, 0x03, 0x06, 0x0A, 0x11, 0x1A, 0x27, 0x37, 0x4B, 0x62, 0x7B, 0x94, 0xAD, 0xC4, 0xD8, 0xE7, 0xF3, 0xFB, 0xFF } 
};

// LRF_SwConfig data structure
const LRF_SwConfig LRF_swConfigBleCsHp = {
    .rxIntFrequency        = 850000,
    .rxFrequencyOffset     = -850000,
    .txFrequencyOffset     = 0,
    .modFrequencyDeviation = 0x0003D090,
    .txShape               = &LRF_shapeBaseGfsk05BleCsHp,
    .bwIndex               = 0x00,
    .bwIndexDither         = 0x01 
};

// LRF_TxPowerTable data structure
const LRF_TxPowerTable LRF_txPowerTableBleCsHp = {
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
const LRF_Config LRF_configBleCsHp = {
    .pbeImage              = (const LRF_TOPsmImage*) LRF_PBE_binary_ble_cs,
    .mceImage              = (const LRF_TOPsmImage*) LRF_MCE_binary_ble_cs,
    .rfeImage              = (const LRF_TOPsmImage*) LRF_RFE_binary_ble_cs,
    .regConfigList         = &LRF_regConfigList 
};
