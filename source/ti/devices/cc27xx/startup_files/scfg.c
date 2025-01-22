/******************************************************************************
 *  Copyright (c) 2023-2024 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1) Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2) Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  3) Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#ifndef __SCFC_C__
#define __SCFC_C__

#include "../inc/hw_scfg.h"

/*! \brief Default scfg struct definition
 *  Defines the scfg structure and field configuration with default values.
 *  If fields are to be overridden, the structure below should be modified in
 *  place or copied into a new C file to avoid contaminating the local SDK
 *  installation.
 */
#if defined(__IAR_SYSTEMS_ICC__)
__root const scfg_t __scfg @ ".scfg" =
#elif defined(__llvm__)
    #pragma GCC diagnostic ignored "-Woverride-init"
    #pragma GCC diagnostic ignored "-Wmissing-braces"
const scfg_t __scfg __attribute__((section(".scfg"), retain)) =
#elif (defined(__GNUC__))
    #pragma GCC diagnostic ignored "-Woverride-init"
    #pragma GCC diagnostic ignored "-Wmissing-braces"
const scfg_t __scfg __attribute__((section(".scfg"), used)) =
#else
    #error "Unsupported compiler used. Expected one of [TI Clang, IAR, GCC]"
#endif
    {
        SCFG_DEFAULT_VALUES,

        // Manual overrides below like this
        .crc32 = 0x2BAD2BAD,

        // Some flashloader versions will not program undefined key entries (see
        // comments in SCFG_DEFAULT_VALUES for some details). Due to an issue
        // with some flashloaders, the resulting programming length is below
        // what the SACI programming interface on the device allows. Therefore,
        // we add 2 dummy entries here to ensure flash programming is
        // successful.
        .keyRingCfg.keyEntries[0] = {SCFG_DUMMY_KEY_ENTRY},
        .keyRingCfg.keyEntries[1] = {SCFG_DUMMY_KEY_ENTRY},
    };

#endif // __SCFC_C__