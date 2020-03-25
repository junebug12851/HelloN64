
#ifdef MPAL_TV

void hack_tvPreNmi()
{
    
}

void hack_tvFix()
{
    // Nothing fancy here, just set to brazilian format
    // (L)ow-Resolution, (A)nti-Aliasing, (N)on-Interlaced, (1)6-bit
    osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
}

#endif
