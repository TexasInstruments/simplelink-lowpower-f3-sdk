#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "ti/common/cc26xx/oad/oad_image_header.h"
#include <unistd.h>

extern const imgHdr_t _imgHdr;
extern int IMAGE_HDR;
imgHdr_t *bimHdr = (imgHdr_t *)&IMAGE_HDR;

uint8_t *exe_footprint = (uint8_t*)0x20001000;

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    
#if defined(SECURITY)
    sprintf((char*)exe_footprint, "%s %s %u", APP_BANNER, "Ver:", bimHdr->secInfoSeg.secVer);
#else
    sprintf((char*)exe_footprint, "%s", APP_BANNER);
#endif
    while(1);
}