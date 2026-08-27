# Copyright (c) 2025, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of Texas Instruments Incorporated nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# ---------------------------------------------------------------------------
# TI Combined Serial MUX — unit tests
#
# Build and run from the repo root:
#   cmake -S source/ti/dmm/combined_serial/tests \
#         -B build/combined_serial_tests -DCMAKE_BUILD_TYPE=Debug
#   cmake --build build/combined_serial_tests
#   ctest --test-dir build/combined_serial_tests --output-on-failure
#
# Notes:
#   P4.3 (embedded: mux_task_app, mux_uart) requires FreeRTOS and the
#   TI UART2 driver, which are only available on the target device.
#   Those tests must be executed on the CC2755P via the on-chip debugger
#   and are therefore not included here.
# ---------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

project(combined_serial_tests VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)   # _GNU_SOURCE for timerfd, signalfd, posix_openpt

enable_testing()

# ---------------------------------------------------------------------------
# Common compile / link flags
# ---------------------------------------------------------------------------
# AddressSanitizer + UBSan require a Debug (or RelWithDebInfo) build.
# They are enabled by default here because these are developer tests.
# Pass -DCMAKE_BUILD_TYPE=Release to skip if needed.
set(TEST_COMPILE_FLAGS
    -Wall -Wextra -Wpedantic -Wconversion -Wshadow
    -fstack-protector-strong
    $<$<NOT:$<CONFIG:Release>>:-g3 -fsanitize=address,undefined>
    $<$<CONFIG:Release>:-O2>
)
set(TEST_LINK_FLAGS
    $<$<NOT:$<CONFIG:Release>>:-fsanitize=address,undefined>
)

# ---------------------------------------------------------------------------
# Shared protocol library (P4.2)
# Compiles hdlc_spinel once, reused by all test executables.
# ---------------------------------------------------------------------------
add_library(proto STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../hdlc_spinel.c
)

target_include_directories(proto PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_options(proto PRIVATE ${TEST_COMPILE_FLAGS})
target_link_options(proto PUBLIC ${TEST_LINK_FLAGS})

# ---------------------------------------------------------------------------
# Helper function: add a single-source test executable and register it with
# ctest.
# ---------------------------------------------------------------------------
function(add_mux_test name source)
    add_executable(${name} ${source})
    target_include_directories(${name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}        # test_harness.h
        ${CMAKE_CURRENT_SOURCE_DIR}/..     # mux_common.h, hdlc_spinel.h, …
    )
    target_link_libraries(${name} PRIVATE proto)
    target_compile_options(${name} PRIVATE ${TEST_COMPILE_FLAGS})
    target_link_options(${name} PRIVATE ${TEST_LINK_FLAGS})
    add_test(NAME ${name} COMMAND ${name})
endfunction()

# ---------------------------------------------------------------------------
# P4.2 — hdlc_spinel unit tests
# ---------------------------------------------------------------------------
add_mux_test(test_hdlc_spinel test_hdlc_spinel.c)

# ---------------------------------------------------------------------------
# P4.3 — embedded tests (NOT built here; run on target hardware)
#
# mux_uart.c and mux_task_app.c depend on:
#   - FreeRTOS (configASSERT, xQueueCreate, xTaskCreate, …)
#   - TI UART2 driver (UART2_open, UART2_write, UART2_readCallback, …)
#   - SysConfig-generated ti_drivers_config.h
#
# These can only be executed on the CC2755P via the TI XDS110 debugger.
# To run them, build the mac_sensor_ble_basic firmware with
# MUX_UNIT_TEST defined and use the ROV / JTAG console for pass/fail output.
# ---------------------------------------------------------------------------
message(STATUS "P4.3 (embedded) tests: skipped — run on target hardware")
