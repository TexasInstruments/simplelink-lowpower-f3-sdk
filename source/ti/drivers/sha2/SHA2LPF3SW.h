/*
 * Copyright (c) 2021-2024, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       SHA2LPF3SW.h
 *
 *  @brief      SHA2 software driver implementation for the Low Power F3 family
 *
 *  This file should only be included in the board file to fill the SHA2_config
 *  struct.
 *
 *  Some devices in the Low Power F3 family do not have a dedicated hardware
 *  crypto accelerator. As a result, there are several limitations and
 *  advantages over accelerator based implementations.
 *
 *  # Limitations #
 *  - Asynchronously canceling an operation is not possible. The APIs will
 *    always run to completion.
 *  - Invoking driver APIs from multiple contexts at the same time using the
 *    same handle is not permitted or guarded against.
 *  - Callback return mode is not supported.
 *  - Blocking return mode does not yield and operates exactly the same as
 *    polling mode.
 *
 *  # Advantages #
 *  - Since this is a software implementation that uses dependency injection, it
 *    is possible for multiple clients to simultaneously invoke driver APIs with
 *    their respective handles.
 *
 *  # Supported Digest Sizes #
 *
 *  The driver is built on top of a SHA256 software implementation stored in
 *  ROM. As a result, digest sizes other than 256 bits are not supported with
 *  this driver.
 *
 *  | SHA2_HashTypes Supported |
 *  |--------------------------|
 *  | SHA2_HASH_TYPE_256       |
 *
 */

/* !!!!!!!!!!!!! WARNING !!!!!!!!!!!!!
 * For CC27XX implementation, please refer to SHA2LPF3HSM.h instead
 */

#ifndef ti_drivers_sha2_SHA2LPF3SW__include
#define ti_drivers_sha2_SHA2LPF3SW__include

#include <stdint.h>
#include <stdbool.h>

#include <ti/drivers/SHA2.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/hapi.h)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief Hardware-specific configuration attributes
 *
 *  SHA2LPF3SW hardware attributes are used in the board file by the
 *  #SHA2_Config struct.
 */
typedef struct
{
    uint8_t reserved1;
} SHA2LPF3SW_HWAttrs;

/*! \cond Internal APIs */

/*
 *  SHA2LPF3SW Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    uint32_t digestBuffer[SHA2_DIGEST_LENGTH_BYTES_256 / sizeof(uint32_t)];
    SHA256SW_Object sha256swObject;
    bool isOpen;
} SHA2LPF3SW_Object;

/*! \endcond */

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_sha2_SHA2LPF3SW__include */
