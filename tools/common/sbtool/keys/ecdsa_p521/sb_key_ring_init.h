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

#define SCFG_KEY_RING_MASTER_HASH {0xac,0x57,0x53,0x0e,0xf4,0x85,0xc3,0xab,0xfc,0x8f,0x1e,0x60,0xd5,0xb0,0x85,0x87,0xf9,0xe8,0xb1,0x54,0xf5,0x4a,0xcc,0x95,0xe4,0x4f,0x7d,0x83,0x12,0x14,0x07,0xac}

#define SCFG_KEY_RING_APP_HASH {0x9c,0xfe,0x46,0x5c,0x35,0x0a,0x8e,0x95,0x8a,0x74,0xfe,0x72,0x42,0xb5,0xdb,0x80,0xfa,0xe4,0xa9,0xa8,0xc1,0x5b,0x5b,0x23,0xab,0x63,0x13,0xe7,0xac,0xb0,0x4a,0x6d}

#define SCFG_KEY_RING_APP_CRC32 0xffef1be9

#define SCFG_KEY_RING_APP .keyRingCfg.keyEntries[0] = \
{ \
    .type = SCFG_KEY_ENTRY_TYPE_APP, \
    .keyHash = SCFG_KEY_RING_APP_HASH, \
    .crc32 = SCFG_KEY_RING_APP_CRC32, \
    .status = SCFG_KEY_ENTRY_STATUS_ACTIVE, \
} \

#define SCFG_KEY_RING_SSB_HASH {0x6c,0xd4,0xc1,0x34,0x18,0x45,0x15,0xfb,0x29,0x57,0xbd,0x03,0xe0,0x3f,0xd8,0x3e,0x81,0x14,0xdf,0x7f,0xf7,0x8d,0x96,0x96,0x08,0x9f,0xbd,0x54,0xf9,0x27,0x38,0x6d}

#define SCFG_KEY_RING_SSB_CRC32 0x29682030

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
