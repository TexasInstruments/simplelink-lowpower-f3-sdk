/******************************************************************************
 Group: CMCU LPRF
 Target Device: cc23xx

 ******************************************************************************
 
 Copyright (c) 2024-2025, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/
/*  PURPOSE: Source match buffer initialization code when configurable memory is used */
#ifndef ZB_SRC_MATCHING_H
#define ZB_SRC_MATCHING_H 1

#ifdef ZB_MAC_SHORT_MATCH_LIST_SIZE
#undef ZB_MAC_SHORT_MATCH_LIST_SIZE
#endif

extern zb_uint8_t gc_zb_mac_short_match_list_size;
#define ZB_MAC_SHORT_MATCH_LIST_SIZE gc_zb_mac_short_match_list_size


#if ZB_CONFIG_OVERALL_NETWORK_SIZE > RCL_CMD_IEEE_SOURCE_MATCH_TABLE_SHORT_MAX_LEN
#define ZB_CONFIG_MAC_SHORT_MATCH_LIST_SIZE ((offsetof(RCL_CmdIeee_SourceMatchingTableShort, shortEntry) / sizeof(uint32_t)) + RCL_CMD_IEEE_SOURCE_MATCH_TABLE_SHORT_MAX_LEN)
#else
#define ZB_CONFIG_MAC_SHORT_MATCH_LIST_SIZE ((offsetof(RCL_CmdIeee_SourceMatchingTableShort, shortEntry) / sizeof(uint32_t)) + ZB_CONFIG_OVERALL_NETWORK_SIZE)
#endif // ZB_CONFIG_OVERALL_NETWORK_SIZE > RCL_CMD_IEEE_SOURCE_MATCH_TABLE_SHORT_MAX_LEN

uint32_t gc_srcMatchTableBuffer[ZB_CONFIG_MAC_SHORT_MATCH_LIST_SIZE] = { 0 };
zb_uint8_t gc_zb_mac_short_match_list_size = ZB_CONFIG_MAC_SHORT_MATCH_LIST_SIZE;

uint8_t gc_neighborToSrcMatchTable[ZB_CONFIG_MAC_SHORT_MATCH_LIST_SIZE];

#endif /* ZB_SRC_MATCHING_H */