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
#include "gfx_font.h"

/*=================================
		 Global Variables
=================================*/

struct BoxState
{
	// Positions
	float x;
	float y;

	// Moving Right
	u8 moveRight;

	// Moving Down
	u8 moveDown;

	// Growing
	u8 grow;

	// Current size
	float size;

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
	float speed;

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
	0.25f, // Speed
	5,  // Slowness Y
	8,  // Slowness Size
	{
		SCREEN_CENTER_X, // Initial Position X
		SCREEN_CENTER_Y, // Initial Position Y
		1, // Initially Moving Right
		0, // Initially Moving Down
		1,  // Initially Growing
		30.0f, // Size
		MATERIAL_LIGHT_BLUE // Initial Color Index
	}
};

struct BG bg = {
	MATERIAL_DARK_BLUE_GREY // Initial Color Index
};

// Constant ticking number
u16 counter = 0;
u8 num = 0;

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

void decRectInd()
{
	box.state.colorInd--;

	if (box.state.colorInd >= 0xFF)
		box.state.colorInd = MATERIAL_DEEP_ORANGE;

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

void decBgInd()
{
	bg.colorInd--;

	if (bg.colorInd < MATERIAL_BROWN)
		bg.colorInd = MATERIAL_DARK_BLUE_GREY;

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
	float x;
	float y;

	float size;

	// Get controller input
	controllerRefreshButtonState();

	joyData = controllerJoyMotion(0);
	x = box.state.x + (joyData.x * box.speed * 4);
	y = box.state.y + (joyData.y * box.speed * 4);
	size = box.state.size;

	if (x >= (SCREEN_WD - box.state.size) ||
		x <= (box.state.size))
		x = box.state.x;

	if (y >= (SCREEN_HT - box.state.size) ||
		y <= (box.state.size))
		y = box.state.y;

	if(controllerBtnReleasedAtLeast1(0))
		num++;

	box.state.x = x;
	box.state.y = y;

	if (controllerBtnReleasedMatchConfig(0, U_JPAD))
		incRectInd();

	if (controllerBtnReleasedMatchConfig(0, D_JPAD))
		decRectInd();

	if (controllerBtnReleasedMatchConfig(0, R_JPAD))
		incBgInd();

	if (controllerBtnReleasedMatchConfig(0, L_JPAD))
		decBgInd();

	if (controllerBtnDownMatchConfig(0, A_BUTTON)) {
		size += box.speed;

		if (size >= box.maxSize)
			size = box.state.size;

		else if (size <= box.minSize)
			size = box.state.size;
	}

	if (controllerBtnDownMatchConfig(0, B_BUTTON)) {
		size -= box.speed;

		if (size >= box.maxSize)
			size = box.state.size;

		else if (size <= box.minSize)
			size = box.state.size;
	}

	box.state.size = size;
}

void debug()
{
	// Grab Controller Data
	NUContData* data = &controllerData[0];

	// Grab Raw Joy Data
	s8 x = data->stick_x;
	s8 y = -data->stick_y; // Y is actually inverted, we need to correct it
	u8 angled = 0;

	// Prepare return ranges between -1.0 and 1.0
	float retX = 0.0f;
	float retY = 0.0f;

	// Load into a return value
	struct MotionReturn ret;

	// Text is next color in list
	u8 tmpColorInd = box.state.colorInd + 1;
	struct ColorRGB tmpColor;

	if (tmpColorInd > MATERIAL_DEEP_ORANGE)
		tmpColorInd = MATERIAL_RED;

	tmpColor = materialColorList[tmpColorInd];

	// Clamp X & Y to both a deadzone and max ranges
	if (x <= CONTROLLER_JOY_DEADZONE_X && x >= -CONTROLLER_JOY_DEADZONE_X)
		x = 0;
	else if (x >= CONTROLLER_JOY_RNG_X)
		x = CONTROLLER_JOY_RNG_X;
	else if (x <= -CONTROLLER_JOY_RNG_X)
		x = -CONTROLLER_JOY_RNG_X;

	if (y <= CONTROLLER_JOY_DEADZONE_Y && y >= -CONTROLLER_JOY_DEADZONE_Y)
		y = 0;
	else if (y >= CONTROLLER_JOY_RNG_Y)
		y = CONTROLLER_JOY_RNG_Y;
	else if (y <= -CONTROLLER_JOY_RNG_Y)
		y = -CONTROLLER_JOY_RNG_Y;

	// Mark if it's angled, angles have a narrower range
	if ((x > 0 || x < 0) &&
		(y > 0 || y < 0))
		angled = 1;

	// Convert the number to a range depending on angle
	if (angled == 0) {
		retX = x / CONTROLLER_JOY_RNG_X;
		retY = y / CONTROLLER_JOY_RNG_Y;
	}
	else {
		retX = x / CONTROLLER_JOY_RNG_ANG_X;
		retY = y / CONTROLLER_JOY_RNG_ANG_Y;
	}

	// Cap it at 1.00 if it's past 1.00
	if (retX > 1.00f)
		retX = 1.00f;
	else if (retX < -1.00f)
		retX = -1.00f;

	if (retY > 1.00f)
		retY = 1.00f;
	else if (retY < -1.00f)
		retY = -1.00f;

	// Print Uppercase A followed by an Exclamation mark
	//controllerBtnReleased[0]
	gfxFontBeginColor(tmpColor.r, tmpColor.g, tmpColor.b, 1);

	gfxFontPrintTile(25, 25, FONT_UPPER_D);
	gfxFontPrintTile(25, 35, FONT_UPPER_P);
	gfxFontPrintTile(25, 45, FONT_UPPER_R);

	gfxFontPrintTile(25, 65, FONT_UPPER_X);
	gfxFontPrintTile(25, 75, FONT_UPPER_Y);
	gfxFontPrintTile(25, 85, FONT_UPPER_A);

	gfxFontPrintTile(65, 65, FONT_PERCENT);
	gfxFontPrintTile(65, 75, FONT_PERCENT);

	gfxFontPrintNumber(35, 25, controllerBtnDown[0]);
	gfxFontPrintNumber(35, 35, controllerBtnPressed[0]);
	gfxFontPrintNumber(35, 45, controllerBtnReleased[0]);

	gfxFontPrintNumber(35, 65, x);
	gfxFontPrintNumber(35, 75, y);
	gfxFontPrintNumber(35, 85, angled);

	gfxFontPrintNumber(75, 65, retX * 100);
	gfxFontPrintNumber(75, 75, retY * 100);

	gfxFontPrintNumber(box.state.x, box.state.y, num);

	gfxFontEnd();
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

	debug();

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
