#ifndef GFX_BASICS_H
#define GFX_BASICS_H

#include <nusys.h>

void clearZBuffer();
void clearArea(u8 r, u8 g, u8 b, int x, int y, int w, int h);
void clearScreen(u8 r, u8 g, u8 b);
void clearAll(u8 r, u8 g, u8 b);
void gfxDone();

#endif // !GFX_BASICS_H
