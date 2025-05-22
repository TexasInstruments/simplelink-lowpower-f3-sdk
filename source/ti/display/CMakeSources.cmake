cmake_minimum_required(VERSION 3.21.3)

set(SOURCES_COMMON Display.c DisplayHost.c DisplayUart2.c)

set(SOURCES_GRLIB DisplayExt.c DisplaySharp.c lcd/SharpGrLib.c)

set(SOURCES_CC13XX_CC26XX ${SOURCES_COMMON} ${SOURCES_GRLIB} DisplayDogm1286.c lcd/LCDDogm1286.c lcd/LCDDogm1286_util.c)
