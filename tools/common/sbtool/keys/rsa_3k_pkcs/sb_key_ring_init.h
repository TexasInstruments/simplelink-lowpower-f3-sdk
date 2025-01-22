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

#define SCFG_KEY_RING_MASTER_HASH {0x8d,0xde,0xe3,0x4f,0x37,0x35,0x94,0x9c,0xd4,0x1b,0x74,0x22,0xcb,0xcb,0x7d,0x16,0x00,0xc2,0xeb,0x12,0xbe,0x2b,0x23,0x05,0xf3,0x18,0xed,0x42,0x07,0xbb,0x89,0x95}

#define SCFG_KEY_RING_APP_HASH {0xe3,0xae,0xab,0x5e,0x3b,0x94,0xc2,0x9d,0x37,0xcb,0xbb,0xfb,0x79,0xf9,0x10,0x92,0x20,0xc0,0x92,0x99,0x94,0xf2,0x8a,0x1e,0xca,0x4b,0xc0,0xa1,0xe8,0x2f,0x10,0x2c}

#define SCFG_KEY_RING_APP_CRC32 0x5758b347

#define SCFG_KEY_RING_APP .keyRingCfg.keyEntries[0] = \
{ \
    .type = SCFG_KEY_ENTRY_TYPE_APP, \
    .keyHash = SCFG_KEY_RING_APP_HASH, \
    .crc32 = SCFG_KEY_RING_APP_CRC32, \
    .status = SCFG_KEY_ENTRY_STATUS_ACTIVE, \
} \

#define SCFG_KEY_RING_SSB_HASH {0x66,0x19,0x02,0x99,0x77,0xe8,0x2f,0xf7,0x76,0x10,0x11,0x94,0x50,0x71,0x92,0x0a,0x7d,0x40,0x5c,0x9d,0x45,0x55,0x5d,0xc4,0x1f,0x7d,0x64,0xb4,0x7a,0x04,0xa2,0xf0}

#define SCFG_KEY_RING_SSB_CRC32 0x7919694c

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
