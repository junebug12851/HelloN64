#ifndef GFX_SCREEN
#define GFX_SCREEN

#if defined(NTSC_TV) || defined(MPAL_TV)
	#define SCREEN_WD   320
	#define SCREEN_HT   240
#endif

// For PAL TV we adjust the size accordingly since we use Fullscreen PAL
#ifdef PAL_TV
	#define SCREEN_WD   320
	#define SCREEN_HT   288
#endif


#endif // ! GFX_SCREEN
