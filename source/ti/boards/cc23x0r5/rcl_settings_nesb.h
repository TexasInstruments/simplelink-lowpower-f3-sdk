// SETTINGS FOR PHY BASED ON RADIO CONTROL LAYER (HEADER FILE)
//
//
// CODE EXPORT INFORMATION
// This file is generated
//
// Tool name                   SmartRF Studio
// Tool version                0.6.0.193 INTERNAL
//
//
// WORKSPACE INFORMATION
//
// Workspace file              srf_cli.workspace
// Device                      CC2340R5
//     Package                 QFN40 5x5 RKP
//     Revision(s)             B (2.0)
// SDK                         SimpleLink CC23x0 SDK 7.11.00.36
// Board                       LP-EM-CC2340R5
// PHY                         2.4 GHz - NESB - 1 Mbps / 2 Mbps
// PHY abbreviation            nesb
//
//
// PHY PROPERTIES
//
// Run-time properties:
//     Sub-PHY                 1 Mbps 
//     Frequency               2420.00000 MHz
//     TX output power         5.0 dBm 
// Start of packet:
//     Synchronization word    0x930B51DE 

#ifndef RCLSETTINGSNESB_H
#define RCLSETTINGSNESB_H

#include <ti/devices/DeviceFamily.h>
#include <ti/drivers/rcl/commands/generic.h>
#include DeviceFamily_constructPath(inc/hw_fcfg.h)


// PHY features
#define RCL_PHY_FEATURE_SUB_PHY_1_MBPS_NESB    0x0000
#define RCL_PHY_FEATURE_SUB_PHY_2_MBPS_NESB    0x0001


// LRF data structures
extern const LRF_TxShape      LRF_shapeBaseGfsk067Nesb;
extern const LRF_TxShape      LRF_shapeBaseGfsk05Nesb;
extern const LRF_SwConfig     LRF_swConfig1MbpsNesb;
extern const LRF_SwConfig     LRF_swConfig2MbpsNesb;
extern const LRF_TxPowerTable LRF_txPowerTableNesb;
extern const LRF_Config       LRF_configNesb;


// LRF register configuration summary

// Address    Module             Register             Bit(s)     Field              Value (1_MBPS)    Value (2_MBPS)
// -----------------------------------------------------------------------------------------------------------------
// 0x1090     LRFDPBE            MDMCMDPAR0           [15:0]     VAL                0x8007            0x8007
// 0x1098     LRFDPBE            MDMCMDPAR2           [15:0]     VAL                0xAAAA            0xAAAA
// 0x10A0     LRFDPBE            POLY0L               [15:0]     VALLSB             0x0000            0x0000
// 0x10A4     LRFDPBE            POLY0H               [15:0]     VALMSB             0x2200            0x2200
// 0x10A8     LRFDPBE            POLY1L               [15:0]     VALLSB             0x0000            0x0000
// 0x10AC     LRFDPBE            POLY1H               [15:0]     VALMSB             0x0700            0x0700
// 0x10B4     LRFDPBE            FCFG0                [5]        TXACOM             0x1               0x1
// 0x10B4     LRFDPBE            FCFG0                [4]        TXADEAL            0x0               0x0
// 0x10B4     LRFDPBE            FCFG0                [1]        RXACOM             0x0               0x0
// 0x10B4     LRFDPBE            FCFG0                [0]        RXADEAL            0x1               0x1
// 0x10B8     LRFDPBE            FCFG1                [8:0]      TXSTRT             0x0C4             0x0C4
// 0x10BC     LRFDPBE            FCFG2                [7:0]      TXSIZE             0x86              0x86
// 0x10C0     LRFDPBE            FCFG3                [8:0]      RXSTRT             0x080             0x080
// 0x10C4     LRFDPBE            FCFG4                [7:0]      RXSIZE             0x44              0x44
// 0x10D0     LRFDPBE            RXFRBTHRS            [9:0]      BYTES              0x004             0x004
// 0x10D4     LRFDPBE            TXFWBTHRS            [9:0]      BYTES              0x002             0x002
// 0x10DC     LRFDPBE            TIMCTL               [1]        SRC0               0x1               0x1
// 0x10E0     LRFDPBE            TIMPRE               [5:0]      PRE0               0x0B              0x0B
// 0x20B8     LRFDMDM            SYSTIMEVTMUX0        [11:6]     SEL1               0x00              0x00
// 0x20B8     LRFDMDM            SYSTIMEVTMUX0        [5:0]      SEL0               0x15              0x15
// 0x20C0     LRFDMDM            ADCDIGCONF           [1]        QBRANCHEN          0x1               0x1
// 0x20C0     LRFDMDM            ADCDIGCONF           [0]        IBRANCHEN          0x1               0x1
// 0x20C8     LRFDMDM            MODSYMMAP0           [7:4]      SYM1               0x1               0x1
// 0x20C8     LRFDMDM            MODSYMMAP0           [3:0]      SYM0               0xF               0xF
// 0x20D4     LRFDMDM            BAUD                 [15:0]     RATEWORD           0x4000            0x8000
// 0x20D8     LRFDMDM            BAUDPRE              [15:13]    ALIGNVALUE         0x0               0x0
// 0x20D8     LRFDMDM            BAUDPRE              [7:0]      PRESCALER          0x0C              0x0C
// 0x20DC     LRFDMDM            MODMAIN              [3:2]      FECSELECT          0x0               0x0
// 0x20DC     LRFDMDM            MODMAIN              [1:0]      MODLEVELS          0x0               0x0
// 0x20E4     LRFDMDM            DEMMISC1             [1:0]      CHFIBW             0x1               0x1
// 0x20E8     LRFDMDM            DEMMISC2             [13:12]    MAFCGAIN           0x0               0x1
// 0x20E8     LRFDMDM            DEMMISC2             [11]       STIMBYPASS         0x0               0x0
// 0x20E8     LRFDMDM            DEMMISC2             [10]       STIMESTONLY        0x1               0x0
// 0x20E8     LRFDMDM            DEMMISC2             [9:7]      STIMTEAPERIOD      0x3               0x1
// 0x20E8     LRFDMDM            DEMMISC2             [6:4]      STIMTEAGAIN        0x7               0x7
// 0x20E8     LRFDMDM            DEMMISC2             [3]        PDIFLINPREDEN      0x1               0x1
// 0x20E8     LRFDMDM            DEMMISC2             [2]        PDIFDESPECK        0x0               0x0
// 0x20E8     LRFDMDM            DEMMISC2             [1]        PDIFIQCONJEN       0x0               0x0
// 0x20E8     LRFDMDM            DEMMISC2             [0]        PDIFLIMITRANGE     0x1               0x1
// 0x20EC     LRFDMDM            DEMMISC3             [14:13]    BDE2DVGA           0x0               -
// 0x20EC     LRFDMDM            DEMMISC3             [12]       BDE1FILTMODE       0x1               0x1
// 0x20EC     LRFDMDM            DEMMISC3             [9:8]      BDE1DVGA           0x1               -
// 0x20EC     LRFDMDM            DEMMISC3             [7]        BDE1NUMSTAGES      0x1               0x1
// 0x20EC     LRFDMDM            DEMMISC3             [4:0]      BDE2DECRATIO       0x02              0x01
// 0x20F0     LRFDMDM            DEMIQMC0             [15:8]     GAINFACTOR         <TRIM>            <TRIM>
// 0x20F0     LRFDMDM            DEMIQMC0             [7:0]      PHASEFACTOR        <TRIM>            <TRIM>
// 0x20F4     LRFDMDM            DEMDSBU              [15:8]     DSBUAVGLENGTH      0x80              0x80
// 0x20F4     LRFDMDM            DEMDSBU              [7:0]      DSBUDELAY          0x87              0x87
// 0x20F8     LRFDMDM            DEMCODC0             [11]       ESTSEL             0x1               0x1
// 0x20F8     LRFDMDM            DEMCODC0             [10:9]     COMPSEL            0x3               0x3
// 0x20F8     LRFDMDM            DEMCODC0             [8]        IIRUSEINITIAL      0x1               0x1
// 0x20F8     LRFDMDM            DEMCODC0             [7:5]      IIRGAIN            0x2               0x3
// 0x20F8     LRFDMDM            DEMCODC0             [4]        IIREN              0x1               0x1
// 0x20FC     LRFDMDM            DEMFIDC0             [5:4]      COMPSEL            0x0               0x0
// 0x2100     LRFDMDM            DEMFEXB0             [10]       OUT1PASSTHROUGH    0x0               0x0
// 0x2100     LRFDMDM            DEMFEXB0             [9:8]      OUT1SRCSEL         0x2               0x2
// 0x2100     LRFDMDM            DEMFEXB0             [5:4]      B3SRCSEL           0x2               0x2
// 0x2100     LRFDMDM            DEMFEXB0             [3:2]      B2SRCSEL           0x1               0x1
// 0x2100     LRFDMDM            DEMFEXB0             [1:0]      B1SRCSEL           0x0               0x0
// 0x2104     LRFDMDM            DEMDSXB0             [5]        OUT2PASSTHROUGH    0x0               -
// 0x2104     LRFDMDM            DEMDSXB0             [3]        OUTSRCSEL2         0x0               0x0
// 0x2104     LRFDMDM            DEMDSXB0             [2]        OUTSRCSEL1         0x1               0x1
// 0x2104     LRFDMDM            DEMDSXB0             [1]        B2SRCSEL           0x0               0x0
// 0x2104     LRFDMDM            DEMDSXB0             [0]        B1SRCSEL           0x0               0x0
// 0x2108     LRFDMDM            DEMFIFE0             [11]       FINEFOESEL         0x0               0x0
// 0x2108     LRFDMDM            DEMFIFE0             [10:9]     FOCFFSEL           0x0               0x0
// 0x2108     LRFDMDM            DEMFIFE0             [4]        IIRUSEINITIAL      0x0               0x0
// 0x2108     LRFDMDM            DEMFIFE0             [3:1]      IIRGAIN            0x2               0x2
// 0x2108     LRFDMDM            DEMFIFE0             [0]        IIREN              0x1               0x1
// 0x210C     LRFDMDM            DEMMAFI0             [15:8]     C1C7               0x04              0x03
// 0x210C     LRFDMDM            DEMMAFI0             [7:0]      C0C8               0x00              0x00
// 0x2110     LRFDMDM            DEMMAFI1             [15:8]     C3C5               0x7B              0x4F
// 0x2110     LRFDMDM            DEMMAFI1             [7:0]      C2C6               0x20              0x15
// 0x2114     LRFDMDM            DEMMAFI2             [8:0]      C4                 0x0C1             0x07C
// 0x2118     LRFDMDM            DEMC1BE0             [15:11]    MASKB              0x00              0x00
// 0x2118     LRFDMDM            DEMC1BE0             [10:6]     MASKA              0x00              0x00
// 0x2118     LRFDMDM            DEMC1BE0             [5:4]      CASCCONF           0x2               0x2
// 0x2118     LRFDMDM            DEMC1BE0             [3:0]      COPYCONF           0xF               0xF
// 0x211C     LRFDMDM            DEMC1BE1             [15:8]     THRESHOLDB         0x27              0x27
// 0x211C     LRFDMDM            DEMC1BE1             [7:0]      THRESHOLDA         0x27              0x27
// 0x2120     LRFDMDM            DEMC1BE2             [9:8]      PEAKCONF           0x2               0x2
// 0x2120     LRFDMDM            DEMC1BE2             [7:0]      THRESHOLDC         0x7F              0x7F
// 0x2124     LRFDMDM            SPARE0               [15:0]     VAL                0x0A18            0x0A2C
// 0x2134     LRFDMDM            DEMSWQU0             [6]        AUTOMAFC           0x1               0x1
// 0x2134     LRFDMDM            DEMSWQU0             [4:0]      REFLEN             0x1F              0x1F
// 0x3080     LRFDRFE            MAGNTHRCFG           [1]        SEL                0x0               0x0
// 0x3088     LRFDRFE            RSSIOFFSET           [7:0]      VAL                <TRIM>            <TRIM>
// 0x30A0     LRFDRFE            SPARE2               [15:0]     VAL                0xA246            0xA357
// 0x30A4     LRFDRFE            SPARE3               [15:0]     VAL                0x1F40            0x1F40
// 0x30A8     LRFDRFE            SPARE4               [15:0]     VAL                0x0000            0x0000
// 0x30AC     LRFDRFE            SPARE5               [15:0]     VAL                0x7C00            0x7C00
// 0x30B0     LRFDRFE            LNA                  [7:4]      TRIM               <TRIM>            <TRIM>
// 0x30B0     LRFDRFE            LNA                  [3]        BIAS               0x1               0x1
// 0x30B0     LRFDRFE            LNA                  [2:1]      IB                 0x1               0x1
// 0x30B4     LRFDRFE            IFAMPRFLDO           [15:9]     TRIM               <TRIM>            <TRIM>
// 0x30B4     LRFDRFE            IFAMPRFLDO           [7:4]      AAFCAP             <TRIM>            <TRIM>
// 0x30B4     LRFDRFE            IFAMPRFLDO           [3:1]      IFAMPIB            0x3               0x3
// 0x30B8     LRFDRFE            PA0                  [4:0]      TRIM               <TRIM>            <TRIM>
// 0x30C4     LRFDRFE            IFADC0               [14:12]    DITHERTRIM         <TRIM>            <TRIM>
// 0x30C4     LRFDRFE            IFADC0               [11:10]    DITHEREN           <TRIM>            <TRIM>
// 0x30C4     LRFDRFE            IFADC0               [7:4]      INT2ADJ            <TRIM>            <TRIM>
// 0x30C4     LRFDRFE            IFADC0               [3:2]      AAFCAP             <TRIM>            <TRIM>
// 0x30C8     LRFDRFE            IFADC1               [15]       NRZ                <TRIM>            <TRIM>
// 0x30C8     LRFDRFE            IFADC1               [14:9]     TRIM               <TRIM>            <TRIM>
// 0x30CC     LRFDRFE            IFADCLF              [15:12]    FF3                <TRIM>            <TRIM>
// 0x30CC     LRFDRFE            IFADCLF              [11:8]     FF2                <TRIM>            <TRIM>
// 0x30CC     LRFDRFE            IFADCLF              [7:4]      FF1                <TRIM>            <TRIM>
// 0x30CC     LRFDRFE            IFADCLF              [3:0]      INT3               <TRIM>            <TRIM>
// 0x30D0     LRFDRFE            IFADCQUANT           [2:0]      QUANTTHR           <TRIM>            <TRIM>
// 0x30D4     LRFDRFE            IFADCALDO            [13:8]     TRIMOUT            <TRIM>            <TRIM>
// 0x30D8     LRFDRFE            IFADCDLDO            [13:8]     TRIMOUT            <TRIM>            <TRIM>
// 0x30E4     LRFDRFE            ATSTREFH             [14:10]    IREFTRIM           <TRIM>            <TRIM>
// 0x30E4     LRFDRFE            ATSTREFH             [9]        BIAS               0x1               0x1
// 0x30E8     LRFDRFE            DCO                  [6:3]      TAILRESTRIM        <TRIM>            <TRIM>
// 0x30E8     LRFDRFE            DCO                  [0]        CRSCAPCM           0x0               0x0
// 0x30EC     LRFDRFE            DIV                  [14:12]    NMIREFTRIM         0x0               0x0
// 0x30EC     LRFDRFE            DIV                  [11:9]     PMIREFTRIM         0x0               0x0
// 0x30EC     LRFDRFE            DIV                  [3]        OUT                0x1               0x1
// 0x30EC     LRFDRFE            DIV                  [2:0]      RATIO              0x0               0x0
// 0x30F0     LRFDRFE            DIVLDO               [14:8]     VOUTTRIM           <TRIM>            <TRIM>
// 0x30F4     LRFDRFE            TDCLDO               [14:8]     VOUTTRIM           <TRIM>            <TRIM>
// 0x30F8     LRFDRFE            DCOLDO0              [13:8]     SECONDTRIM         <TRIM>            <TRIM>
// 0x30F8     LRFDRFE            DCOLDO0              [7:4]      FIRSTTRIM          <TRIM>            <TRIM>
// 0x30FC     LRFDRFE            DCOLDO1              [10]       REFSRC             0x0               0x0
// 0x30FC     LRFDRFE            DCOLDO1              [9:8]      DIVATST            0x0               0x0
// 0x3100     LRFDRFE            PRE0                 [13:8]     PLLDIV1            0x0F              0x0F
// 0x3100     LRFDRFE            PRE0                 [5:0]      PLLDIV0            0x0E              0x0E
// 0x3104     LRFDRFE            PRE1                 [5:0]      HSDDC              0x00              0x00
// 0x3108     LRFDRFE            PRE2                 [15:12]    MIDCALDIVLSB       0x0               0x0
// 0x3108     LRFDRFE            PRE2                 [11:6]     CRSCALDIV          0x18              0x18
// 0x3108     LRFDRFE            PRE2                 [5:0]      FSMDIV             0x05              0x05
// 0x310C     LRFDRFE            PRE3                 [15:5]     FINECALDIV         0x030             0x030
// 0x310C     LRFDRFE            PRE3                 [4:0]      MIDCALDIVMSB       0x03              0x03
// 0x3110     LRFDRFE            CAL0                 [14:8]     FCSTART            0x40              0x40
// 0x3110     LRFDRFE            CAL0                 [3:2]      TDCAVG             0x2               0x2
// 0x3114     LRFDRFE            CAL1                 [14:8]     FCTOP              0x7F              0x7F
// 0x3114     LRFDRFE            CAL1                 [6:0]      FCBOT              0x00              0x00
// 0x3120     LRFDRFE            MISC0                [13]       PHCPT              0x0               0x0
// 0x3120     LRFDRFE            MISC0                [12]       TDCCALCORR         0x1               0x1
// 0x3120     LRFDRFE            MISC0                [11]       TDCMSBCORR         0x1               0x1
// 0x3120     LRFDRFE            MISC0                [5:4]      DLYCANCRS          0x2               0x2
// 0x3130     LRFDRFE            PHEDISC              [13:10]    CNT                0x3               0x3
// 0x3130     LRFDRFE            PHEDISC              [9:0]      THR                0x007             0x007
// 0x3138     LRFDRFE            PLLMON0              [15:14]    PHELOLCNT          0x3               0x3
// 0x3138     LRFDRFE            PLLMON0              [13:8]     PHELOLTHR          0x1F              0x1F
// 0x3138     LRFDRFE            PLLMON0              [6:0]      FCTHR              0x7F              0x7F
// 0x313C     LRFDRFE            PLLMON1              [12:8]     PHELOCKCNT         0x04              0x04
// 0x313C     LRFDRFE            PLLMON1              [7:0]      PHELOCKTHR         0x7F              0x7F
// 0x3140     LRFDRFE            MOD0                 [12:11]    SCHEME             0x3               0x3
// 0x3140     LRFDRFE            MOD0                 [10:8]     SYMSHP             0x0               0x0
// 0x3140     LRFDRFE            MOD0                 [3:2]      INTPFACT           0x1               0x1
// 0x2020     PBE_GENERIC_RAM    PHY                  [5:3]      RFESEL             0x0               0x1
// 0x2020     PBE_GENERIC_RAM    PHY                  [2:0]      MCESEL             0x0               0x1
// 0x2022     PBE_GENERIC_RAM    SYNTHCALTIMEOUT      [15:0]     VAL                0x00B4            0x00B4
// 0x2024     PBE_GENERIC_RAM    PKTCFG               [9]        BITORDER           0x1               0x1
// 0x2024     PBE_GENERIC_RAM    PKTCFG               [8]        HDRORDER           0x1               0x1
// 0x2024     PBE_GENERIC_RAM    PKTCFG               [7]        CRCINCHDR          0x1               0x1
// 0x2024     PBE_GENERIC_RAM    PKTCFG               [6]        CRCINCSW           0x1               0x1
// 0x2024     PBE_GENERIC_RAM    PKTCFG               [5:0]      NUMHDRBITS         0x0B              0x0B
// 0x2026     PBE_GENERIC_RAM    NUMCRCBITS           [5:0]      VAL                0x08              0x08
// 0x2028     PBE_GENERIC_RAM    FIFOCFG              [15]       APPENDTIMESTAMP    0x0               0x0
// 0x2028     PBE_GENERIC_RAM    FIFOCFG              [14]       APPENDRSSI         0x1               0x1
// 0x2028     PBE_GENERIC_RAM    FIFOCFG              [13]       APPENDFREQEST      0x0               0x0
// 0x2028     PBE_GENERIC_RAM    FIFOCFG              [12]       APPENDLQI          0x0               0x0
// 0x2028     PBE_GENERIC_RAM    FIFOCFG              [11]       APPENDSTATUS       0x1               0x1
// 0x2028     PBE_GENERIC_RAM    FIFOCFG              [10]       APPENDCRC          0x0               0x0
// 0x2028     PBE_GENERIC_RAM    FIFOCFG              [9]        AUTOFLUSHEMPTY     0x1               0x1
// 0x2028     PBE_GENERIC_RAM    FIFOCFG              [8]        AUTOFLUSHIGN       0x1               0x1
// 0x2028     PBE_GENERIC_RAM    FIFOCFG              [7]        AUTOFLUSHCRC       0x1               0x1
// 0x2028     PBE_GENERIC_RAM    FIFOCFG              [6:4]      LENOPTPAD          0x3               0x3
// 0x202A     PBE_GENERIC_RAM    EXTRABYTES           [15:0]     VAL                0x0002            0x0002
// 0x202C     PBE_GENERIC_RAM    WHITEINIT            [15:0]     VAL                0x0000            0x0000
// 0x2030     PBE_GENERIC_RAM    CRCINITL             [15:0]     VALLSB             0x0000            0x0000
// 0x2032     PBE_GENERIC_RAM    CRCINITH             [15:0]     VALMSB             0xFF00            0xFF00
// 0x2034     PBE_GENERIC_RAM    LENCFG               [9:5]      NUMLENBITS         0x08              0x08
// 0x2034     PBE_GENERIC_RAM    LENCFG               [4:0]      LENPOS             0x03              0x03
// 0x2036     PBE_GENERIC_RAM    LENOFFSET            [15:0]     VAL                0x0000            0x0000
// 0x203E     PBE_GENERIC_RAM    PRETXIFS             [15:0]     VAL                0x011A            0x0146
// 0x2040     PBE_GENERIC_RAM    PRERXIFS             [15:0]     VAL                0x0014            0x0014
// 0x209E     PBE_GENERIC_RAM    NESB                 [5:3]      PEERADRLEN         0x3               0x3
// 0x209E     PBE_GENERIC_RAM    NESB                 [2]        NESBMODE           0x1               0x1
// 0x6804     RFE_COMMON_RAM     SYNTHCTL             [7]        CHRGFILT           0x0               0x0
// 0x6804     RFE_COMMON_RAM     SYNTHCTL             [6]        VREFBP             0x0               0x0
// 0x6804     RFE_COMMON_RAM     SYNTHCTL             [5]        TXWAITMOD          0x0               0x0
// 0x6804     RFE_COMMON_RAM     SYNTHCTL             [4]        PHEDISC            0x1               0x1
// 0x6804     RFE_COMMON_RAM     SYNTHCTL             [3]        RTRIMTST           0x0               0x0
// 0x6804     RFE_COMMON_RAM     SYNTHCTL             [2]        IIR                0x0               0x0
// 0x6804     RFE_COMMON_RAM     SYNTHCTL             [1]        REFDTHR            0x1               0x1
// 0x6804     RFE_COMMON_RAM     SYNTHCTL             [0]        FCDEM              0x0               0x0
// 0x6806     RFE_COMMON_RAM     TDCCAL0              [10:8]     STOP               0x3               0x3
// 0x6808     RFE_COMMON_RAM     TDCCAL1              [7:0]      SUB                0x00              0x00
// 0x680A     RFE_COMMON_RAM     TDCCAL2              [5:0]      AVG                0x10              0x10
// 0x680C     RFE_COMMON_RAM     TDCPLL               [10:8]     STOP               0x0               0x0
// 0x680E     RFE_COMMON_RAM     K1LSB                [15:0]     VAL                0x569B            0x569B
// 0x6810     RFE_COMMON_RAM     K1MSB                [15:0]     VAL                0x010A            0x010A
// 0x6812     RFE_COMMON_RAM     K2BL                 [15]       HPM                0x0               0x0
// 0x6812     RFE_COMMON_RAM     K2BL                 [14:0]     VAL                0x02BE            0x02BE
// 0x6814     RFE_COMMON_RAM     K2AL                 [15]       HPM                0x0               0x0
// 0x6814     RFE_COMMON_RAM     K2AL                 [14:0]     VAL                0x0074            0x0074
// 0x6816     RFE_COMMON_RAM     K3BL                 [15:0]     VAL                0x132C            0x132C
// 0x6818     RFE_COMMON_RAM     K3AL                 [15:0]     VAL                0x07AB            0x07AB
// 0x681A     RFE_COMMON_RAM     K5                   [15:0]     VAL                0x916F            0x916F
// 0x6820     RFE_COMMON_RAM     RTRIMOFF             [3:0]      VAL                0x0               0x0
// 0x6822     RFE_COMMON_RAM     RTRIMMIN             [3:0]      VAL                0x8               0x8
// 0x6824     RFE_COMMON_RAM     DIVI                 [14:12]    NMIREFTRIM         0x0               0x0
// 0x6824     RFE_COMMON_RAM     DIVI                 [11:9]     PMIREFTRIM         0x0               0x0
// 0x6824     RFE_COMMON_RAM     DIVI                 [3]        OUT                0x1               0x1
// 0x6824     RFE_COMMON_RAM     DIVI                 [2:0]      RATIO              0x0               0x0
// 0x6826     RFE_COMMON_RAM     DIVF                 [14:12]    NMIREFTRIM         0x4               0x4
// 0x6826     RFE_COMMON_RAM     DIVF                 [11:9]     PMIREFTRIM         0x4               0x4
// 0x6826     RFE_COMMON_RAM     DIVF                 [3]        OUT                0x1               0x1
// 0x6826     RFE_COMMON_RAM     DIVF                 [2:0]      RATIO              0x0               0x0
// 0x6828     RFE_COMMON_RAM     DIVLDOI              [14:8]     VOUTTRIM           0x00              0x00
// 0x6828     RFE_COMMON_RAM     DIVLDOI              [0]        CTL                0x0               0x0
// 0x682A     RFE_COMMON_RAM     DIVLDOF              [14:8]     VOUTTRIM           0x00              0x00
// 0x682A     RFE_COMMON_RAM     DIVLDOF              [0]        CTL                0x0               0x0
// 0x682C     RFE_COMMON_RAM     DIVLDOIOFF           [6:0]      VAL                0x14              0x14
// 0x682E     RFE_COMMON_RAM     LDOSETTLE            [9:0]      VAL                0x047             0x047
// 0x6830     RFE_COMMON_RAM     CHRGSETTLE           [9:0]      VAL                0x02E             0x02E
// 0x6832     RFE_COMMON_RAM     DCOSETTLE            [9:0]      VAL                0x005             0x005
// 0x6834     RFE_COMMON_RAM     IFAMPRFLDOTX         [15:9]     TRIM               0x7F              0x7F
// 0x6836     RFE_COMMON_RAM     IFAMPRFLDODEFAULT    [15:9]     TRIM               0x00              0x00
// 0x683E     RFE_COMMON_RAM     PHYRSSIOFFSET        [7:0]      VAL                0x50              0x4C
// 0x6840     RFE_COMMON_RAM     SPARE0SHADOW         [15:0]     VAL                0x75F8            0x55FB
// 0x6842     RFE_COMMON_RAM     SPARE1SHADOW         [15:0]     VAL                0x0045            0x0045
// 0x6844     RFE_COMMON_RAM     AGCINFO              [0]        MODE               0x0               0x0


#endif
