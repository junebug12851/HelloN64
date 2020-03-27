/*========================================================
				Stage 0 logic and rendering
========================================================*/
/*=================================
			Libraries
=================================*/
#include <nusys.h>
#include "gfx_screen.h"
#include "spiral.h"
#include "controller_state.h"
#include "gfx_glist.h"
#include "gfx_basis.h"
#include "stage_manager.h"
#include "gfx_materialColor.h"
#include "gfx_basics.h"
#include "font_tilemap.h"
#include "font_tilemap_conversion.h"

/*=================================
		 Global Variables
=================================*/

struct BoxState
{
	// Positions
	int x;
	int y;

	// Moving Right
	u8 moveRight;

	// Moving Down
	u8 moveDown;

	// Growing
	u8 grow;

	// Current size
	u8 size;

	// Border Color Ind
	// To keep track of the color list position
	u8 colorInd;

	// Border Color
	// Current color to render
	struct ColorRGB color;
};

struct Box {
	// Minimum size
	u8 minSize;

	// Maximum Size
	u8 maxSize;

	// Border Thickness
	u8 thickness;

	// Generic speed of many factors including x bounce
	u8 speed;

	// Slowness Y-Bounce
	u8 slownessY;

	// Slowness Growth
	u8 slownessSize;

	// Tmp State Variables
	struct BoxState state;
};

struct BG {
	// Color Ind
	// To keep track of the color list position
	u8 colorInd;

	// BG Color
	struct ColorRGB color;
};

struct Box box = {
	10, // Min
	40, // Max
	2,  // Thickness
	10, // Speed
	5,  // Slowness Y
	8,  // Slowness Size
	{
		SCREEN_CENTER_X, // Initial Position X
		SCREEN_CENTER_Y, // Initial Position Y
		1, // Initially Moving Right
		0, // Initially Moving Down
		1,  // Initially Growing
		30, // Size
		MATERIAL_LIGHT_BLUE // Initial Color Index
	}
};

struct BG bg = {
	MATERIAL_DARK_BLUE_GREY // Initial Color Index
};

// Constant ticking number
u16 counter = 0;

/*=================================
		   ClearBackground
 Wipe the background with a color
=================================*/

// Cycle forward and wrap-around box color
void incRectInd()
{
	box.state.colorInd++;

	if (box.state.colorInd > MATERIAL_DEEP_ORANGE)
		box.state.colorInd = MATERIAL_RED;

	box.state.color = materialColorList[box.state.colorInd];
}

// Cycle forward and wrap-around background color
void incBgInd()
{
	bg.colorInd++;

	if (bg.colorInd > MATERIAL_DARK_BLUE_GREY)
		bg.colorInd = MATERIAL_BROWN;

	bg.color = materialColorList[bg.colorInd];
}

void drawSpiral(int x, int y)
{
	// Standard 1 Pixel per Cycle
	// 1 and 2 cycle modes are only ones that allow z-depth
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);

	// Makes detailed settings regarding the input sources to the CC
	// 2nd argument isnt used as we're only using 1 cycle
	// DECALRGBA does a flat, basic copy
	gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);

	// Sets the blender's rendering mode
	// Textured surfaces with alpha edge mask
	// Alpha outlined objects, allows pixel to be on or off and non-transparent edges are blurry
	gDPSetRenderMode(glistp++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);

	// Sets which depth source value to use for comparisons with the Z buffer
	// Use the primitive depth register's Z value and deltaZ value as opposed to the individual pixels
	gDPSetDepthSource(glistp++, G_ZS_PRIM);

	// Sets the depth (Z) of the primitive in the RDP.
	// Presuming what your're about to draw
	// Z and Delta-Z are both set to 0, not sure what delta z is but I guess we'll find out
	// Docs don't really clarify
	gDPSetPrimDepth(glistp++, 0, 0);

	// Enables/disables texture map perspective correction
	// Disable perspective correction
	gDPSetTexturePersp(glistp++, G_TP_NONE);

	// Loads texture as a contiguous texture block
	// S = horizontal
	// T = Vertical
	gDPLoadTextureBlock(
		glistp++,
		spiral,          		  // timg - Our sprite array (Address of image)
		G_IM_FMT_RGBA,            // fmt - Our image format (RGBA-Format)
		G_IM_SIZ_16b,             // size - Pixel size (Pixel Size 16-bits)
		16, 16,                   // width, height
		0,                        // Palette to use (always 0) (only for 4-bit color index texture)
		G_TX_WRAP, G_TX_WRAP,     // cms, cmt (enable wrap operations for t and s)
		G_TX_NOMASK, G_TX_NOMASK, // masks, maskt (Mask id for t and s, set to both none)
		G_TX_NOLOD, G_TX_NOLOD    // shifts, shiftt (t & s shift value, both set to none)
	);

	gSPTextureRectangle(glistp++,
		(x - 8) << 2, (y - 8) << 2,   // Top left corner of the rectangle
		(x + 8) << 2, (y + 8) << 2,   // Bottom right corner of the rectangle
		G_TX_RENDERTILE,          // Tile descriptor
		0 << 5, 0 << 5,           // Starting S T Coordinate
		1 << 10, 1 << 10          // S T Increment
	);

	gDPPipeSync(glistp++);
}

void _stageInit(void)
{
	// Fill in initital color with color from struct
	// We have to wait until this function because we can't pull array values at compile time
	bg.color = materialColorList[bg.colorInd];
	box.state.color = materialColorList[box.state.colorInd];
}

void _stageUpdate(void)
{
	struct MotionReturn joyData;
	int x;
	int y;

	// Get controller input
	controllerRefreshButtonState();

	joyData = controllerJoyMotion(0);
	x = box.state.x + (joyData.x * box.speed);
	y = box.state.y + (joyData.y * box.speed);

	if (x >= (SCREEN_WD - box.state.size) ||
		x <= (box.state.size))
		x = box.state.x;

	if (y >= (SCREEN_HT - box.state.size) ||
		y <= (box.state.size))
		y = box.state.y;

	box.state.x = x;
	box.state.y = y;

	// Counter always counts up and overflows back to zero
	//counter++;

	//// Move in direction
	//if (box.state.moveRight == 1)
	//	box.state.x += box.speed;
	//else
	//	box.state.x -= box.speed;

	//// Check for boundraries and adjust accordingly
	//if (box.state.x >= (SCREEN_WD - box.state.size)) {
	//	box.state.x -= (box.speed * 2);
	//	box.state.moveRight = 0;
	//	incRectInd();
	//}
	//else if (box.state.x <= (box.state.size)) {
	//	box.state.x += (box.speed * 2);
	//	box.state.moveRight = 1;
	//	incRectInd();
	//}

	//// Move up/down in intervals
	//if ((counter % box.slownessY) == 0)
	//{
	//	if (box.state.moveDown == 1)
	//		box.state.y += box.speed;
	//	else
	//		box.state.y -= box.speed;

	//	if (box.state.y >= (SCREEN_HT - box.state.size)) {
	//		box.state.y -= (box.speed * 2);
	//		box.state.moveDown = 0;
	//		incBgInd();
	//	}
	//	else if (box.state.y <= (box.state.size)) {
	//		box.state.y += (box.speed * 2);
	//		box.state.moveDown = 1;
	//		incBgInd();
	//	}
	//}

	//if ((counter % box.slownessSize) == 0)
	//{
	//	if (box.state.grow == 1)
	//		box.state.size += box.speed;
	//	else
	//		box.state.size -= box.speed;

	//	if (box.state.size >= box.maxSize) {
	//		box.state.size -= (box.speed * 2);
	//		box.state.grow = 0;
	//	}
	//	else if (box.state.size <= box.minSize) {
	//		box.state.size += (box.speed * 2);
	//		box.state.grow = 1;
	//	}
	//}
}

void _stageDraw(void)
{
	// Start RCP instructions Over Again
	initRCP();

	// Wipe the background with a color
	clearAll(bg.color.r, bg.color.g, bg.color.b);

	// Draw the rectangle first
	clearArea(
		box.state.color.r, 
		box.state.color.g, 
		box.state.color.b,
		box.state.x - box.state.size,
		box.state.y - box.state.size,
		box.state.x + box.state.size,
		box.state.y + box.state.size);

	// Fill the inside with same fill color
	clearArea(
		bg.color.r,
		bg.color.g, 
		bg.color.b,
		(box.state.x - box.state.size) + box.thickness,
		(box.state.y - box.state.size) + box.thickness,
		(box.state.x + box.state.size) - box.thickness,
		(box.state.y + box.state.size) - box.thickness);

	// Draw Spiral Centered
	//drawSpiral(box.state.x, box.state.y);

	// Print Uppercase A followed by an Exclamation mark
	gfxPrintFontTile(box.state.x, box.state.y, FONT_UPPER_A);
	gfxPrintFontTile(box.state.x + 8, box.state.y, FONT_UPPER_B);
	gfxPrintFontTile(box.state.x + 16, box.state.y, FONT_EXCLAMATION);
	gfxEndPrintFont();

	// Mark us done with the gfx, this inserts some final instructions and then
	// transmits it to the gpu
	gfxDone();
}

void _stageDestruct()
{

}

// Register interface functions
// These are automatically called on demand and as needed
struct StageInterface stageStartup = {
	_stageInit,
	_stageUpdate,
	_stageDraw,
	_stageDestruct
};
