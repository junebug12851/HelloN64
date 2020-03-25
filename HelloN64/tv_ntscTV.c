#ifdef NTSC_TV

#include <nusys.h>

void tvPreNmi()
{

}

void tvInit()
{
    // 320x240
    // (L)ow-Resolution, (A)nti-Aliasing, (N)on-Interlaced, (1)6-bit
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
}

#endif
