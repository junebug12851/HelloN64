#include <nusys.h>
#include "gfx_glist.h"
#include "gfx_screen.h"

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
