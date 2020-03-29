#include <nusys.h>

void tvNtscPreNmi()
{

}

void tvNtscInit()
{
    // 320x240
    // (L)ow-Resolution, (A)nti-Aliasing, (N)on-Interlaced, (1)6-bit
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
}
