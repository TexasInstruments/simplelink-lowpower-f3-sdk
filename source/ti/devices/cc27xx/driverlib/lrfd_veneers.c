/******************************************************************************
 *  Filename:       lrfd_veneers.c
 *
 *  Description:    Veneers for accessing LRFD API from non-secure context.
 *
 *  Copyright (c) 2025 Texas Instruments Incorporated
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

#include "lrfd.h"
#include "../inc/hw_memmap.h"

#include <third_party/tfm/secure_fw/include/security_defs.h> // __tz_c_veneer
#include <third_party/tfm/secure_fw/spm/include/utilities.h> // tfm_core_panic()

extern void LRFDSetDependencySetsPtr(uint16_t (*dependencySetsPtr)[LRFD_NUM_CLK_DEP]);

__tz_c_veneer void LRFDSetDependencySetsPtr_veneer(uint16_t (*dependencySetsPtr)[LRFD_NUM_CLK_DEP])
{
    // Verify that the pointer is in NS RAM
    if ((((uintptr_t)dependencySetsPtr) < SRAM_NS_BASE) ||
        ((((uintptr_t)dependencySetsPtr) + sizeof(*dependencySetsPtr)) > (SRAM_NS_BASE + SRAM_SIZE)))
    {
        tfm_core_panic();
    }

    LRFDSetDependencySetsPtr(dependencySetsPtr);
}
