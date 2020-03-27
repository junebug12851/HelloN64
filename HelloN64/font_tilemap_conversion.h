#ifndef FONT_TILEMAP_CONVERSION
#define FONT_TILEMAP_CONVERSION

#include <nusys.h>

// Size of ascii
#define FONT_TILEMAP_CONV_CACHE_SIZE 128

u8 fontTilemapConvertChar(u8 c);
void fontTilemapConvCacheInit();
u8 convertString(char* str, u8* out);

extern u8 fontTilemapConvCache[FONT_TILEMAP_CONV_CACHE_SIZE];

#endif
