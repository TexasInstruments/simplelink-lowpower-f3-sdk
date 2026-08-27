/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

/*
 *  ======== CANCC27XXX20.c ========
 */
#include <stddef.h>
#include <stdint.h>

#include <ti/drivers/can/CANCC27XX.h>
#include <ti/drivers/CAN.h>

#include <third_party/mcan/MCAN.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/can.h)
#include DeviceFamily_constructPath(inc/hw_canfd.h)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_types.h)

extern void CANCC27XX_irqHandler(void *arg);

/*
 *  ======== MCAN_writeReg ========
 *  Write a value to a MCAN register or message RAM location.
 */
void MCAN_writeReg(uint32_t offset, uint32_t value)
{
    uint32_t addr;

    /* Determine if the offset is for an MCAN register or message RAM */
    if (offset < CANFD_SRAM_BASE)
    {
        /* MCAN register: add the offset to CAN-FD peripheral base address */
        addr = CANFD_BASE + offset;
    }
    else
    {
        /* MCAN message RAM location: directly use the offset as the address */
        addr = offset;
    }

    HWREG(addr) = value;
}

/*
 *  ======== CANCC27XX_hwiFxn ========
 */
void CANCC27XX_hwiFxn(uintptr_t arg)
{
    CANCC27XX_irqHandler((void *)arg);
}

/*
 *  ======== CANCC27XXXX_init ========
 */
int_fast16_t CANCC27XXXX_init(const CAN_Config *config)
{
    return CAN_STATUS_SUCCESS;
}

/*
 *  ======== CANCC27XXXX_close ========
 */
void CANCC27XXXX_close(CAN_Handle handle)
{
    (void)handle; /* unused arg */
}
