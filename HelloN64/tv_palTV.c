#include <nusys.h>

void tvPalPreNmi()
{
    // We're dodging around a difficult NuSys bug by doing this for the time being
    // Turn off the display and re-fix Y-Scale back to 1
    nuGfxDisplayOff();
    osViSetYScale(1);
}

void tvPalInit()
{
    // Set video mode to Full-Screen PAL
    // 320x288
    // (L)ow-Resolution, (A)nti-Aliasing, (N)on-Interlaced, (1)6-bit
    osViSetMode(&osViModeTable[OS_VI_FPAL_LAN1]);

    // We're dodging around a difficult NuSys bug by doing this for the time being
    // Squish image slightly
    // If Pre-NMI event occurs (Reset Button) this will crash if not fixed back correctly
    osViSetYScale(0.833); // Dirty but works
}
