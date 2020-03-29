#include <nusys.h>

void tvMpalPreNmi()
{
    
}

void tvMpalInit()
{
    // Nothing fancy here, just set to brazilian format
    // 320x240
    // (L)ow-Resolution, (A)nti-Aliasing, (N)on-Interlaced, (1)6-bit
    osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
}
