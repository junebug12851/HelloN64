#ifdef PAL_TV

#include <nusys.h>

void tv_PreNmi()
{
    
}

void tv_Init()
{
    // Set video mode to Full-Screen PAL
    // 320x288
    // (L)ow-Resolution, (A)nti-Aliasing, (N)on-Interlaced, (1)6-bit
    osViSetMode(&osViModeTable[OS_VI_FPAL_LAN1]);
}

#endif
