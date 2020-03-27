#ifndef GFX_FONT_H
#define GFX_FONT_H

struct RowCol
{
	u8 row;
	u8 col;
};

struct Coords2D
{
	int x;
	int y;
};

void gfxFontBegin();
void gfxFontLoadTilesetBank(u8 fontBank);
struct RowCol gfxFontConvertTileIdToRowCol(u8 tileID);
u8 gfxFontConvertRowToFontBank(u8 row);
u8 gfxFontConvertTileIdToLocal(u8 tileID, u8 row);
struct Coords2D gfxFontConvertTileToCoords(u8 row, u8 col);
void gfxFontPrintTile(int x, int y, u8 tileID);
void gfxFontEnd();

extern u8 fontTilesetLoaded;

#endif // ! GFX_FONT_H
