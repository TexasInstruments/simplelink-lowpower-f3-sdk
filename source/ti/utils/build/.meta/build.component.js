/*
 * Copyright (c) 2019-2023 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== build.component.js ========
 *  Define the SysConfig modules in the TI Utils Build component
 */

"use strict";

const displayName = "Build Assist";

let templates = [
    {
        "name": "/ti/utils/build/GenLibs.cmd.xdt",
        "outputPath": "ti_utils_build_linker.cmd.genlibs",
        "alwaysRun": true /* so make can _always_ add this to link line */
    },
    {
        "name": "/ti/utils/build/GenMap.cmd.xdt",
        "outputPath": "ti_utils_build_linker.cmd.genmap",
        "alwaysRun": true /* so users can _always_ include this file */
    },
    {
        "name": "/ti/utils/build/GenOpts.opt.xdt",
        "outputPath": "ti_utils_build_compiler.opt",
        "alwaysRun": true /* so make can _always_ add this to compile line */
    }
];

/*
 * No topModules means no components are visible in the SysConfig UI.
 */
exports = {
    displayName,
    templates: templates
};
