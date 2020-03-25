/*========================================================
                Initialize the graphics
========================================================*/
/*=================================
            Libraries
=================================*/

#include <nusys.h>
#include "gfx_screen.h"
#include "gfx_glist.h"

/*=================================
         Global Variables
=================================*/

// Initialize the game camera
static Vp viewport = {
  SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ, 0,
  SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ, 0,
};

// Initialize the RSP
Gfx rspinit_dl[] = {
    // Set viewport measurements
    gsSPViewport(&viewport),

    // Disable many features
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
                            G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                            G_TEXTURE_GEN_LINEAR | G_LOD),

    // Disable Textures
    gsSPTexture(0, 0, 0, 0, G_OFF),

    // End List
    gsSPEndDisplayList(),
};

// Initialize the RDP
Gfx rdpinit_dl[] = {
    // 1 Pixel per machine cycle
    gsDPSetCycleType(G_CYC_1CYCLE),

    // Drawing region in screen coordinates
    // Draw all scan-lines, Area to draw (whole screen)
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),

    // Disable Chroma-Keying
    // Chroma-Keying was an unfinished product, it never left beta
    gsDPSetCombineKey(G_CK_NONE),

    // No alpha-comparing do do various fade effects in the blender
    gsDPSetAlphaCompare(G_AC_NONE),

    // No blender rendinering modes
    gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),

    // Disable color dithering
    gsDPSetColorDither(G_CD_DISABLE),

    // Wait for pixels to be processed and issue update
    gsDPPipeSync(),

    // End list
    gsSPEndDisplayList(),
};

/*=================================
              RCPInit
 Prepare the RCP for incoming work
=================================*/

void RCPInit(Gfx* glistp)
{
    // Set the segment register
    // Sets base address and segment register
    gSPSegment(glistp++, 0, 0);

    // Execute our RSP Display List
    gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(rspinit_dl));

    // Execute our RDP Display List
    gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(rdpinit_dl));
}
