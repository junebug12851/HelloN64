#ifndef GFX_FONT_H
#define GFX_FONT_H

void gfxFontBegin();
void gfxFontLoadTilesetBank(u8 fontBank);
void gfxFontPrintTile(int x, int y, u8 tileID);
void gfxFontEnd();

extern u8 fontTilesetLoaded;

#endif // ! GFX_FONT_H
