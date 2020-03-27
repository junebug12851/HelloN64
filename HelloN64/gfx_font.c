#include <nusys.h>
#include "gfx_font.h"
#include "gfx_glist.h"
#include "font_tilemap.h"
#include "font_tilemap_conversion.h"

// Currently loaded font bank
// 0 = Not in font mode
// 1 = Font Bank #
u8 fontTilesetLoaded = 0;

// Has to be called before font operations
void gfxFontBegin()
{
	// Ensure this is reset
	fontTilesetLoaded = 0;

	// 1 Cycle as we need Z-Buffer but don't need any particular special effects
	// Do a hard paste over whatevers there
	// Textures with alpha coloring
	// For z-value, don't compare per-pixel, just the whole primitive
	// Give a Z-value of 10 (Min increments of 8)
	// No texture perspective, not a 3D world
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);
	gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
	gDPSetRenderMode(glistp++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
	gDPSetDepthSource(glistp++, G_ZS_PRIM);
	gDPSetPrimDepth(glistp++, 10, 0);
	gDPSetTexturePersp(glistp++, G_TP_NONE);

	// Load RGBA16 LUT for 4-bit textures into TMEM at slot 0
	gDPSetTextureLUT(glistp++, G_TT_RGBA16);
	gDPLoadTLUT_pal16(glistp++, 0, font_tilemap_lut);
}

// Loads Font Tileset X into TMEM
// This is more efficient than loading a single tile
void gfxFontLoadTilesetBank(u8 fontBank)
{
	// Load correct requested bank
	u8* fontTilemap = font_tilemap_1;
	if (fontBank == 1)
		fontTilemap = font_tilemap_2;
	else if (fontBank == 2)
		fontTilemap = font_tilemap_3;
	if (fontBank == 3)
		fontTilemap = font_tilemap_4;

	// Load Correct Tilemap Bank into TMEM
	gDPLoadTextureBlock_4b(glistp++,
		fontTilemap, 				// timg
		G_IM_FMT_RGBA, 						// fmt
		FONT_TILEMAP_PART_WIDTH_PIXELS,		// Tilemap Width
		FONT_TILEMAP_PART_HEIGHT_PIXELS,	// Tilemap Height
		0, 					// PAL
		G_TX_WRAP, 			// CM S
		G_TX_WRAP, 			// CM T
		G_TX_NOMASK, 		// Mask S
		G_TX_NOMASK, 		// Mask T
		G_TX_NOLOD, 		// Shift S
		G_TX_NOLOD);		// Shift T

	//gDPPipeSync(glistp++);
	fontTilesetLoaded = fontBank + 1;
}

void gfxFontPrintTile(int x, int y, u8 tileID)
{
	// There is a method to the layout of tiles
	// Each row is exactly 16 tiles 0x0 - 0xF
	// This was designed for fast lookup on a Gameboy originally
	// and to perfectly fit into the Gameboy's lower-third memory as a bonus
	u8 row = tileID / FONT_TILE_SIZE_RADIX;
	u8 col = tileID % FONT_TILE_SIZE_RADIX;

	// A hack
	// Tileset 0: Not Loaded
	// Tileset 1 or 2: Needs Tileset 1 or 2
	// Tileset 2 starts at row index 4
	// true == 1, Check for tileset 2 or 1 and add 1 to make it a vlid requires number
	//u8 requiresTileset = (row >= FONT_TILEMAP_PART_HEIGHT_TILES) + 1;
	u8 requiresTileset = 0;

	// Offset so we can pick out the tile from within the bank
	/*u8 localTileID = (row >= FONT_TILEMAP_PART_HEIGHT_TILES)
		? tileID - FONT_TILE_PART_COUNT
		: tileID;*/

	u8 localTileID;

	u8 localRow;
	u8 localCol;

	u8 localX;
	u8 localY;

	// Still a bit hardcoded
	// @TODO calculate better for macros
	if (row < 2)
		localTileID = tileID - (FONT_TILE_PART_COUNT * 0);
	else if (row < 4)
		localTileID = tileID - (FONT_TILE_PART_COUNT * 1);
	else if (row < 6)
		localTileID = tileID - (FONT_TILE_PART_COUNT * 2);
	else
		localTileID = tileID - (FONT_TILE_PART_COUNT * 3);

	localRow = localTileID / FONT_TILE_SIZE_RADIX;
	localCol = localTileID % FONT_TILE_SIZE_RADIX;

	localX = localRow * FONT_TILE_SIZE_PIXELS;
	localY = localCol * FONT_TILE_SIZE_PIXELS;

	// Sort of a cheap way to calculate the required bank number
	// Need to be using the defines
	// @TODO Switch to using defines and not hard-coded numbers
	if (row < 2)
		requiresTileset = 1;
	else if (row < 4)
		requiresTileset = 2;
	else if (row < 6)
		requiresTileset = 3;
	else
		requiresTileset = 4;

	// Issue a pipe sync if we've already drawn some stuff and are about to switch tileset banks
	if (fontTilesetLoaded != requiresTileset &&
		fontTilesetLoaded > 0)
		gDPPipeSync(glistp++);

	// Load the tileset into memory if it's not already
	if (fontTilesetLoaded != requiresTileset)
		gfxLoadFontTilesetBank(requiresTileset - 1);

	// If I understand this correctly
	// Load a texture at the UL and BR coordinates with the size
	// of a tile. Use calculated tile offset coordinates to load correct
	// tile. Don't skip any pixels in the loading process
	gSPTextureRectangle(glistp++,
		x << 2, y << 2,							// UL Rectangle
		x + FONT_TILE_SIZE_PIXELS << 2,			// BR X Rectangle
		y + FONT_TILE_SIZE_PIXELS << 2,			// BR Y Rectangle
		G_TX_RENDERTILE,						// Descriptor Index
		localX * FONT_TILE_SIZE_PIXELS << 5,	// Texture Coordinaate S UL
		localY * FONT_TILE_SIZE_PIXELS << 5,	// Texture Coordinaate T UL
		1 << 10, 1 << 10);						// Change in S/T for each X/Y

	//gDPPipeSync(glistp++);
}

// Has to be callled at the end of font operations
void gfxFontEnd()
{
	fontTilesetLoaded = 0;
	gDPSetTextureLUT(glistp++, G_TT_NONE);

	// Issue a pipe sync before ending
	gDPPipeSync(glistp++);
}
