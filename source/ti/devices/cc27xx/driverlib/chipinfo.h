/******************************************************************************
 *  Filename:       chipinfo.h
 *
 *  Description:    Collection of functions returning chip information.
 *
 *  Copyright (c) 2022-2025 Texas Instruments Incorporated
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

#ifndef __CHIP_INFO_H__
#define __CHIP_INFO_H__

//*****************************************************************************
//
//! \addtogroup system_control_group
//! @{
//! \addtogroup chipinfo_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../inc/hw_sys0.h"
#include "../inc/hw_fcfg.h"

//*****************************************************************************
//
//! \brief Chip type enumeration
//
//*****************************************************************************
typedef enum
{
    CHIP_TYPE_CC2744 = 0x3548U, //!< Chip type is CC2744
    CHIP_TYPE_CC2745 = 0xAF23U, //!< Chip type is CC2745
    CHIP_TYPE_CC2755 = 0x6301U, //!< Chip type is CC2755
} ChipType;

//*****************************************************************************
//
//! \brief Chip part ID enumeration
//
//*****************************************************************************
typedef enum
{
    CC2744R74E0WRHARQ1 = 0x483548U, //!< Part ID is CC2744R74E0WRHARQ1
    CC2745R10E0WRHARQ1 = 0xE8AF23U, //!< Part ID is CC2745R10E0WRHARQ1
    CC2745R10E1WRHARQ1 = 0xE8AF23U, //!< Part ID is CC2745R10E1WRHARQ1
    CC2745P10E0WRHARQ1 = 0xDEAF23U, //!< Part ID is CC2745P10E0WRHARQ1
    CC2745R74E0WRHARQ1 = 0x87AF23U, //!< Part ID is CC2745R74E0WRHARQ1
    CC2755R105E0WRHAR  = 0x9D6301U, //!< Part ID is CC2755R105E0WRHAR
    CC2755P105E0WRHAR  = 0xE66301U, //!< Part ID is CC2755P105E0WRHAR
    CC2755R105E0YCJR   = 0x316301U, //!< Part ID is CC2755R105E0YCJR
} ChipPartId;

//*****************************************************************************
//
//! \brief Returns hardware version number
//!
//! The version number is set to 0 for the first chip and thereafter
//! incremented by 1 for each minor hardware change.
//!
//! \return Returns the hardware version number
//
//*****************************************************************************
__STATIC_INLINE uint32_t ChipInfoGetVersion(void)
{
    return (HWREG(SYS0_BASE + SYS0_O_DEVICEID) & SYS0_DEVICEID_VERSION_M) >> SYS0_DEVICEID_VERSION_S;
}

#ifndef DRIVERLIB_NS
//*****************************************************************************
//
// Below functions will only be compiled into the driverlib.a library and not
// the driverlib_ns.a library.
// Non-secure applications might be able to access some/all of these functions
// through veneers declared further below in the #else case.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Returns chip type.
//!
//! \return
//! Returns \ref ChipType
//
//*****************************************************************************
__STATIC_INLINE ChipType ChipInfoGetChipType(void)
{
    return (ChipType)(fcfg->deviceInfo.partId.part);
}

//*****************************************************************************
//
//! \brief Returns part ID.
//!
//! \return
//! Returns \ref ChipPartId
//
//*****************************************************************************
__STATIC_INLINE ChipPartId ChipInfoGetPartId(void)
{
    return (ChipPartId)(fcfg->deviceInfo.partId.val32 & 0x00FFFFFFU);
}

#else
//*****************************************************************************
//
// Secure-only API functions available from non-secure context through
// veneers.
//
// These functions must be linked in from from the secure veneer library.
//
//*****************************************************************************

extern ChipType ChipInfoGetChipType_veneer(void);
    #define ChipInfoGetChipType ChipInfoGetChipType_veneer

extern ChipPartId ChipInfoGetPartId_veneer(void);
    #define ChipInfoGetPartId   ChipInfoGetPartId_veneer

#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
//! Close the Doxygen group.
//! @}
//! @}
//
//*****************************************************************************

#endif // __CHIP_INFO_H__
