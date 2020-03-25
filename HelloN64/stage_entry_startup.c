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

/*=================================
			 Prototypes
=================================*/

static void ClearBackground(u8 r, u8 g, u8 b);

/*=================================
		 Global Variables
=================================*/

struct Color
{
	u8 r;
	u8 g;
	u8 b;
};

static struct Color color_list[19] = {
	{255,  82,  82}, //  0) Red
	{255,  64, 129}, //  1) Pink
	{224,  64, 251}, //  2) Purple
	{124,  77, 255}, //  3) Deep Purple
	{ 83, 109, 254}, //  4) Indigo
	{ 68, 138, 255}, //  5) Blue
	{  3, 169, 244}, //  6) Light Blue
	{  0, 188, 212}, //  7) Cyan
	{  0, 150, 136}, //  8) Teal
	{ 76, 175,  80}, //  9) Green
	{139, 195,  74}, // 10) Light Green
	{205, 220,  57}, // 11) Lime
	{255, 235,  59}, // 12) Yellow
	{255, 193,   7}, // 13) Amber
	{255, 152,   0}, // 14) Orange
	{255,  87,  34}, // 15) Deep Orange
	{121,  85,  72}, // 16) Brown
	{158, 158, 158}, // 17) Grey
	{ 69,  90, 100}, // 18) Dark Blue Grey
};

static struct Color color_bg;
static struct Color color_rect;

// Specify the position of the rectangle
static int x = SCREEN_WD / 2;
static int y = SCREEN_HT / 2;

static u8 rectMinSize = 10;
static u8 rectSize = 30;
static u8 rectMaxSize = 40;
static u8 rectThickness = 2;

static u8 moveRight = 1;
static u8 moveDown = 0;
static u8 grow = 1;

static u8 speed = 2;
static u8 slownessY = 5;
static u8 slownessSize = 8;

static u16 counter = 0;

static u8 bgInd = 16;
static u8 rectInd = 0;

/*=================================
			stage00_init
		Initialize the stage
=================================*/

void _stageInit(void)
{
	color_bg = color_list[bgInd];
	color_rect = color_list[rectInd];
}

/*=================================
			stage00_destruct
		Destruct The Stage
=================================*/

void _stageDestruct()
{
	
}

/*=================================
		  stage00_update
   Update variables and objects
=================================*/

void incRectInd()
{
	rectInd++;
	if (rectInd > 15) // Deep Orange
		rectInd = 0; // Red

	color_rect = color_list[rectInd];
}

void incBgInd()
{
	bgInd++;
	if (bgInd > 18) // Dark Blue Grey
		bgInd = 16; // Brown

	color_bg = color_list[bgInd];
}

void _stageUpdate(void)
{
	// Get controller input
	controllerRefreshButtonState();

	// Counter always counts up and overflows back to zero
	counter++;

	// Move in direction
	if (moveRight == 1)
		x += speed;
	else
		x -= speed;

	// Check for boundraries and adjust accordingly
	if (x >= (SCREEN_WD - rectSize)) {
		x -= (speed * 2);
		moveRight = 0;
		incRectInd();
	}
	else if (x <= (rectSize)) {
		x += (speed * 2);
		moveRight = 1;
		incRectInd();
	}

	// Move up/down in intervals
	if ((counter % slownessY) == 0)
	{
		if (moveDown == 1)
			y += speed;
		else
			y -= speed;

		if (y >= (SCREEN_HT - rectSize)) {
			y -= (speed * 2);
			moveDown = 0;
			incBgInd();
		}
		else if (y <= (rectSize)) {
			y += (speed * 2);
			moveDown = 1;
			incBgInd();
		}
	}

	if ((counter % slownessSize) == 0)
	{
		if (grow == 1)
			rectSize += speed;
		else
			rectSize -= speed;

		if (rectSize >= rectMaxSize) {
			rectSize -= (speed * 2);
			grow = 0;
		}
		else if (rectSize <= rectMinSize) {
			rectSize += (speed * 2);
			grow = 1;
		}
	}
}

/*=================================
			stage00_draw
		   Draw the stage
=================================*/

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

void _stageDraw(void)
{
	// Start RCP instructions Over Again
	initRCP();

	// Wipe the background with a color
	ClearBackground(color_bg.r, color_bg.g, color_bg.b);

	// Draw the rectangle
	gDPSetCycleType(glistp++, G_CYC_FILL);// Very fast "fill mode"

	// Limitation of fill is that is must use GPACK_RGBA5551 or GPACK_ZDZ and be in "Fill Mode"
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(color_rect.r, color_rect.g, color_rect.b, 1) << 16 | GPACK_RGBA5551(color_rect.r, color_rect.g, color_rect.b, 1)));
	gDPFillRectangle(glistp++, x - rectSize, y - rectSize, x + rectSize, y + rectSize);

	gDPSetFillColor(glistp++, (GPACK_RGBA5551(color_bg.r, color_bg.g, color_bg.b, 1) << 16 | GPACK_RGBA5551(color_bg.r, color_bg.g, color_bg.b, 1)));
	gDPFillRectangle(glistp++, (x - rectSize) + rectThickness, (y - rectSize) + rectThickness, (x + rectSize) - rectThickness, (y + rectSize) - rectThickness);

	// Draw Spiral Centered
	drawSpiral(x, y);

	// Syncronize the RCP and CPU
	gDPFullSync(glistp++);

	// Specify that our display list has ended
	gSPEndDisplayList(glistp++);

	// Start the display task
	nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx), NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
}

/*=================================
		   ClearBackground
 Wipe the background with a color
=================================*/

static void ClearBackground(u8 r, u8 g, u8 b)
{
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetDepthImage(glistp++, nuGfxZBuffer); // nuGfxZBuffer is Nusys’ Z-Buffer 
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, nuGfxZBuffer);
	gDPSetFillColor(glistp++, (GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)));
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
	gDPPipeSync(glistp++);

	// Specify the RDP Cycle type
	gDPSetCycleType(glistp++, G_CYC_FILL);

	// Specify the color type
	// Apparently takes a virtual address so we convert it from a physical address
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));

	// Specify the color
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));

	// Draw a rectangle to the screen
	gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);

	// Resyncronize for the next display list task
	gDPPipeSync(glistp++);
}

// Register interface functions
struct StageInterface stageStartup = {
	_stageInit,
	_stageUpdate,
	_stageDraw,
	_stageDestruct
};
