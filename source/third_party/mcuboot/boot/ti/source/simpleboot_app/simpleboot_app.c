/*
 * Copyright (c) 2026, Texas Instruments Incorporated
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

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "flash_map_backend.h"
#include <ti/devices/DeviceFamily.h>
#include <ti/drivers/power/PowerCC27XX.h>
#include DeviceFamily_constructPath(driverlib/hapi.h)

#include "ti/common/cc26xx/debug/led_debug.h"


/* Boot into Secondary by default, boot into Primary by reading update status */
#define SIMPLEBOOT_PRIMARY (2U)

static __attribute__((naked)) void start_app(uint32_t *vector_table) {

    /* The following code resets the SP to the value specified in the
     * provided vector table, and then the Reset Handler is invoked.
     *
     * Per ARM Cortex specification:
     *
     *           ARM Cortex VTOR
     *
     *
     *   Offset             Vector
     *
     * 0x00000000  ++++++++++++++++++++++++++
     *             |    Initial SP value    |
     * 0x00000004  ++++++++++++++++++++++++++
     *             |         Reset          |
     * 0x00000008  ++++++++++++++++++++++++++
     *             |          NMI           |
     *             ++++++++++++++++++++++++++
     *             |           .            |
     *             |           .            |
     *             |           .            |
     *
     * */

    __asm volatile ("ldr r1, [r0, #0]\n"
                    "msr msp, r1\n"
                    "ldr r1, [r0, #4]\n"
                    "mov pc, r1\n"
                    );
}

static void do_boot(struct boot_rsp *rsp) {

    /* We run from internal flash. Base address of this medium is 0x0 */
    uint32_t vector_table = 0x0 + rsp->br_image_off + rsp->br_hdr->ih_hdr_size;

    start_app((uint32_t *)vector_table);
}

static void mcubootFail(void) {
    powerUpGpio();
    while(1)
    {
        blinkLed(RED_LED, 10, 50);
        blinkLed(RED_LED, 2, 1000);
    }
}
/*
 *  ======== mainThread ========
 */
int main(void)
{
    struct boot_rsp bootRsp;

    if(HapiSbGetId() == SIMPLEBOOT_PRIMARY)
    {
        /* Primary is treated as SSB.
         * Check if ssbHeader is valid, if not set bootRsp to app.
         * This will be verified later on.
         */
        struct image_header *ssbHeader = (struct image_header *)BOOT_PRIMARY_1_BASE_ADDRESS;
        if (ssbHeader->ih_magic == IMAGE_MAGIC)
        {
            bootRsp.br_flash_dev_id = 0;
            bootRsp.br_image_off = BOOT_PRIMARY_1_BASE_ADDRESS;
            bootRsp.br_hdr = (struct image_header *)BOOT_PRIMARY_1_BASE_ADDRESS;
        }
        else
        {
            bootRsp.br_flash_dev_id = 1;
            bootRsp.br_image_off = BOOT_SECONDARY_1_BASE_ADDRESS;
            bootRsp.br_hdr = (struct image_header *)BOOT_SECONDARY_1_BASE_ADDRESS;
        }
        
    }
    else
    {
        /* Secondary is treated as APP
         * Check if appHeader is valid, if not set bootRsp to ssb.
         * This will be verified later on.
         */
        struct image_header *appHeader = (struct image_header *)BOOT_SECONDARY_1_BASE_ADDRESS;
        if (appHeader->ih_magic == IMAGE_MAGIC)
        {
            bootRsp.br_flash_dev_id = 1;
            bootRsp.br_image_off = BOOT_SECONDARY_1_BASE_ADDRESS;
            bootRsp.br_hdr = (struct image_header *)BOOT_SECONDARY_1_BASE_ADDRESS;
        }
        else
        {
            bootRsp.br_flash_dev_id = 0;
            bootRsp.br_image_off = BOOT_PRIMARY_1_BASE_ADDRESS;
            bootRsp.br_hdr = (struct image_header *)BOOT_PRIMARY_1_BASE_ADDRESS;
        }
        
    }

    powerUpGpio();
    lightRedLed();

    if (bootRsp.br_hdr->ih_magic == IMAGE_MAGIC)
    {
        blinkLed(GREEN_LED, 3, 500);
        do_boot(&bootRsp);
    }
    else
    {
        mcubootFail();
    }
}
