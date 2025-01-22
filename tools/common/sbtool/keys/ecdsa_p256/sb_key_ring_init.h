/*                                                                                 
 * Copyright (c) 2015-2024, Texas Instruments Incorporated                         
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
#ifndef __SCFG_KEY_RING_INIT_H__
#define __SCFG_KEY_RING_INIT_H__

#define SCFG_KEY_RING_MASTER_HASH {0x7b,0x80,0x34,0xcf,0xef,0x1b,0xf5,0xae,0x77,0x3c,0x4b,0x04,0x74,0xdd,0xdc,0xbd,0x5b,0xa9,0xa2,0xb5,0x2f,0x26,0xe9,0xdb,0xd9,0x86,0x77,0x82,0x54,0xca,0x49,0xf0}

#define SCFG_KEY_RING_APP_HASH {0xfa,0x2b,0xe2,0x7f,0xed,0xb6,0x99,0xff,0x88,0x46,0xd3,0x02,0x56,0x14,0x4e,0xc0,0x5e,0x32,0xca,0xed,0xe4,0x0f,0x8b,0x55,0x3f,0x29,0xbe,0xe5,0xf1,0xf9,0x93,0x1c}

#define SCFG_KEY_RING_APP_CRC32 0x5e62e120

#define SCFG_KEY_RING_APP .keyRingCfg.keyEntries[0] = \
{ \
    .type = SCFG_KEY_ENTRY_TYPE_APP, \
    .keyHash = SCFG_KEY_RING_APP_HASH, \
    .crc32 = SCFG_KEY_RING_APP_CRC32, \
    .status = SCFG_KEY_ENTRY_STATUS_ACTIVE, \
} \

#define SCFG_KEY_RING_SSB_HASH {0xc7,0x9d,0x62,0x94,0x67,0xef,0x2b,0x91,0x1a,0xef,0x66,0xf7,0xb9,0xc6,0xf8,0x89,0x8c,0xab,0x0d,0x00,0x06,0x66,0x67,0x7e,0xf0,0x91,0xe6,0x1b,0xe6,0x03,0xf4,0x5e}

#define SCFG_KEY_RING_SSB_CRC32 0x1a08f6cb

#define SCFG_KEY_RING_SSB .keyRingCfg.keyEntries[1] = \
{ \
    .type = SCFG_KEY_ENTRY_TYPE_BLDR, \
    .keyHash = SCFG_KEY_RING_SSB_HASH, \
    .crc32 = SCFG_KEY_RING_SSB_CRC32, \
    .status = SCFG_KEY_ENTRY_STATUS_ACTIVE, \
} 

#define SCFG_KEY_RING_CONFIG \
    SCFG_KEY_RING_APP, \
    SCFG_KEY_RING_SSB


#endif
