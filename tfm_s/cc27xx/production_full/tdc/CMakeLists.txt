#-------------------------------------------------------------------------------
# Copyright (c) 2024, Texas Instruments Incorporated
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

# Allow the target_link_libraries() command to be called from any directory to
# add link dependencies and link interface libraries to targets created in other
# directories
cmake_policy(SET CMP0079 NEW)

# Fetch all _veneers.c files in driverlib
file(GLOB driverlib_veneers_c_sources ${DRIVERLIB_SOURCE_DIR}/ti/devices/cc27xxx10/driverlib/*_veneers.c)

target_sources(tfm_s
    PRIVATE
        ${driverlib_veneers_c_sources}
)

target_link_libraries(tfm_s
    PRIVATE
        $<$<C_COMPILER_ID:ARMClang>:${DRIVERLIB_SOURCE_DIR}/ti/devices/cc27xxx10/driverlib/lib/ticlang/driverlib.a>
        $<$<C_COMPILER_ID:GNU>:${DRIVERLIB_SOURCE_DIR}/ti/devices/cc27xxx10/driverlib/lib/gcc/driverlib.a>
        $<$<C_COMPILER_ID:IAR>:${DRIVERLIB_SOURCE_DIR}/ti/devices/cc27xxx10/driverlib/lib/iar/driverlib.a>
)
