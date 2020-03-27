#include <nusys.h>
#include "gfx_glist.h"
#include "gfx_screen.h"
#include "font_tilemap.h"
#include "font_tilemap_conversion.h"

u8 fontTilesetLoaded = 0;

void clearZBuffer()
{
	// Clear Z-Buffer
	// Enter Fill Mode
	gDPSetCycleType(glistp++, G_CYC_FILL);

	// Select the Z-Buffer to be nuSys Z-Buffer
	gDPSetDepthImage(glistp++, nuGfxZBuffer); // nuGfxZBuffer is Nusys’ Z-Buffer 

	// Set attributes on the Z-Buffer
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, nuGfxZBuffer);

	// Set to erase the Z-Buffer, the values below are special to clearing out the Z-Buffer
	gDPSetFillColor(glistp++, (GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)));

	// Wipe-Out Z-Buffer
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);

	// Stop here and wait until GPU is cleared out
	gDPPipeSync(glistp++);
}

void clearArea(u8 r, u8 g, u8 b, int x, int y, int w, int h)
{
	// Specify the RDP Cycle type
	gDPSetCycleType(glistp++, G_CYC_FILL);

	// Specify the color type
	// Apparently takes a virtual address so we convert it from a physical address
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));

	// Specify the color
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));

	// Draw a rectangle to the screen
	gDPFillRectangle(glistp++, x, y, w - 1, h - 1);

	// Resyncronize for the next display list task
	gDPPipeSync(glistp++);
}

void clearScreen(u8 r, u8 g, u8 b)
{
	clearArea(r, g, b, 0, 0, SCREEN_WD, SCREEN_HT);
}

void clearAll(u8 r, u8 g, u8 b)
{
	clearZBuffer();
	clearScreen(r, g, b);
}

void gfxDone()
{
	// Syncronize the RCP and CPU
	// Signals the end of a frame
	// Basically tell the GPU to notify  us that it's done and shut down
	gDPFullSync(glistp++);

	// Specify that our display list has ended
	gSPEndDisplayList(glistp++);

	// Start the display task
	// Basically send all the instructions over to the gpu
	nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
}

// Loads Font Tileset X into TMEM
// This is more efficient than loading a single tile
// 0 = 
// A-Z a-z 0-9 . " "
// 1 =
// All the others
void gfxLoadFontTilesetBank(u8 fontBank)
{
	u8* fontTilemap = font_tilemap_1;
	if (fontBank > 0)
		fontTilemap = font_tilemap_2;

	// Load RGBA16 LUT for 4-bit textures into TMEM at slot 0
	gDPSetTextureLUT(glistp++, G_TT_RGBA16);
	gDPLoadTLUT_pal16(glistp++, 0, font_tilemap_lut);

	// Load LUT Tilemap Bank
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

	gDPPipeSync(glistp++);
	gDPSetTextureLUT(glistp++, G_TT_NONE);

	fontTilesetLoaded = fontBank + 1;
}

void gfxPrintFontTile(int x, int y, u8 tileID)
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
	u8 localTileID = (row >= FONT_TILEMAP_PART_HEIGHT_TILES)
		? tileID - FONT_TILE_PART_COUNT
		: tileID;

	u8 localRow = localTileID / FONT_TILE_SIZE_RADIX;
	u8 localCol = localTileID % FONT_TILE_SIZE_RADIX;

	u8 localX = localRow * FONT_TILE_SIZE_PIXELS;
	u8 localY = localCol * FONT_TILE_SIZE_PIXELS;

	if (row >= FONT_TILEMAP_PART_HEIGHT_TILES)
		requiresTileset = 2;
	else
		requiresTileset = 1;

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

	gDPPipeSync(glistp++);
}

void gfxEndPrintFont()
{
	fontTilesetLoaded = 0;
}
