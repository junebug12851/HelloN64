
#ifdef PAL_TV

// To prevent crashing when the reset button is pressed
// This is only called when compiled in PAL Mode
void hack_tvPreNmi()
{
    nuGfxDisplayOff();
    osViSetYScale(1);
}

void hack_tvFix()
{
    // Set video mode to Full-Screen PAL
    // (L)ow-Resolution, (A)nti-Aliasing, (N)on-Interlaced, (1)6-bit
    osViSetMode(&osViModeTable[OS_VI_FPAL_LAN1]);

    // Squish image slightly
    // If Pre-NMI event occurs (Reset Button) this will crash if not fixed back correctly
    osViSetYScale(0.833); // Dirty but works
}

#endif
