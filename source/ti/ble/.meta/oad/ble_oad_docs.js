/*
 * Copyright (c) 2024 Texas Instruments Incorporated - http://www.ti.com
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
 *
 */

/*
 *  ======== ble_oad_docs.js ========
 */

"use strict";

const enableOadLongDescription = `
Enable Over-the-Air Download (OAD) functionality for this project.

When enabled, the application will include OAD support allowing firmware
updates to be performed wirelessly over BLE.

__Note__: OAD requires additional configuration including:
- Proper flash layout configuration
- Boot loader (Mcuboot) setup
- Image signing tools

For more information, refer to the OAD section in the BLE User's Guide.
`;

const oadMethodLongDescription = `
Select the OAD method to use for firmware updates.

__Dual-Image OAD__: Stores two full application images in internal flash (Image A
and Image B). mcuboot selects which image to boot based on image metadata and
validity. When a new image is downloaded, it is written to the inactive slot and
verified before mcuboot switches to it on the next reboot.
No external flash required

__On-Chip OAD__: Uses a single on-chip flash slot for firmware updates.
The new image is downloaded into the application slot and verified before
switching to it. This method works together with the persistent OAD example,
sharing the same flash layout where a small persistent app manages the OAD
process. This method provides:
- Simple single-slot architecture
- No external flash required
- Suitable for devices with sufficient internal flash

__Off-Chip OAD__: Uses external flash to store the downloaded firmware
image. The new image is received over BLE and written to external flash,
then copied to the internal flash application slot. This method provides:
- Full application flash space preserved during download
- External flash stores the new image until ready to apply

__Multistep OAD__: Uses a half-image architecture that maximizes flash utilization
for application code. The code strictly necessary to perform OAD (most of the
BLE stack, the OAD application logic, and essential drivers) is placed in APP_1_SLOT
first. The remaining application code fills whatever space is left in APP_1_SLOT and
overflows into APP_2_SLOT only if necessary (TI-Clang only — with IAR, all remaining
code is always placed in APP_2_SLOT).

To identify which object files belong in the first slot, this method uses a
two-step build process:
1. __Test build__ (\`MS_OAD_HALF_MODE_BUILD\` defined): Compiles with most
   application code excluded, producing a map file that reveals which objects
   are needed for OAD.
2. __Main build__ (no \`MS_OAD_HALF_MODE_BUILD\`): Uses the object list derived
   from the test build to place code into the correct slots.

For more information, refer to the OAD section in the BLE User's Guide.
`;

const halfModeBuildLongDescription = `
Enable half-mode build for Multistep OAD.

When enabled, the build will define \`MS_OAD_HALF_MODE_BUILD\` which instructs
the linker to place all code sections in APP_1_SLOT. This is used during the
test build phase to identify which objects are needed for slot placement.

__Automatic Management__:
This setting is automatically managed by the \`msoad_prebuild\` script during
the build process:
1. Prebuild enables this option and runs a test build
2. Prebuild parses the resulting map file to identify required objects
3. Prebuild populates the Slot 1 Objects List automatically
4. Prebuild disables this option for the main build

__Manual Use__:
In most cases, you should not need to manually change this setting. However,
it can be useful for debugging or understanding the slot placement:
- Enable to see all code placed in APP_1_SLOT (test build mode)
- Disable to perform the full split build with slot placement

__Note__: When manually configuring these settings, remove the prebuild step
from the project build configuration to prevent it from overwriting your changes.
`;

const slot1ObjectsListLongDescription = `
List of libraries and standalone objects to place in APP_1_SLOT (one per line).

This option is only available when Half-Mode Build is disabled. It controls
which code is placed in each application slot for Multistep OAD.

__Automatic Management__:
This list is automatically populated by the \`msoad_prebuild\` script during
the build process. The prebuild script:
1. Runs a test build with Half-Mode Build enabled
2. Parses the resulting map file using \`mapfile_parser\`
3. Extracts library names and standalone objects
4. Populates this list automatically

__Format__:
Library names (.a) and standalone object names (.obj), one per line:
\`\`\`
OneLib.a
libc.a
main.obj
app.obj
\`\`\`

__Behavior__:
- Entire libraries (.a) listed will be placed in APP_1_SLOT
- Standalone objects (.obj/.o) listed will be placed in APP_1_SLOT
- All remaining code (not listed) will be placed in APP_2_SLOT

__Manual Configuration__:
When manually editing this list, remove the prebuild step from the project
build configuration to prevent it from overwriting your changes.

`;

const initSlotSizeLongDescription = `
Size (in bytes) of the INIT_SLOT memory region reserved at the start of APP_1
for reset vectors and linker-generated initialization sections (\`.cinit\`,
\`.binit\`, \`.pinit\`, \`.init_array\`).

__Purpose__:
The TI-Clang linker's split operator (\`>>\`) can displace small fixed sections
when repacking code across APP_1_SLOT and APP_2_SLOT. Reserving a dedicated
INIT_SLOT prevents alignment conflicts and guarantees these sections have space.

__Sizing Guidelines__:
- The default value (0x700) is sufficient for most applications.
- Increase if you see linker errors about \`.cinit\` not fitting in INIT_SLOT.
- The value is subtracted from APP_1_SLOT, so larger values reduce available
  code space in APP_1_SLOT.
- Must be a multiple of 4 (word-aligned).

__What goes in INIT_SLOT__:
- \`.resetVecs\` — Cortex-M vector table (~0x8C bytes)
- \`.cinit\` — Compressed C initialization data (typically 0x400–0x600 bytes)
- \`.binit\`, \`.pinit\`, \`.init_array\` — Other initialization tables (small)
`;

exports = {
    enableOadLongDescription: enableOadLongDescription,
    oadMethodLongDescription: oadMethodLongDescription,
    halfModeBuildLongDescription: halfModeBuildLongDescription,
    slot1ObjectsListLongDescription: slot1ObjectsListLongDescription,
    initSlotSizeLongDescription: initSlotSizeLongDescription
};
