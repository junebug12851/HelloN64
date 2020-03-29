/*========================================================
                Initialize the game
========================================================*/
/*=================================
            Libraries
=================================*/

#include <nusys.h>
#include "stage_manager.h"
#include "tv.h"
#include "onEvents.h"
#include "controller_state.h"
#include "font_tilemap_conversion.h"
#include "memory.h"

/*=================================
              Main
=================================*/

void mainproc(void* dummy)
{
    // Init NuSys GFX First

    // Initialize and activate the graphics thread and Graphics Task Manager.
    // Sort of completes booting of the NuSys N64 OS
    nuGfxInit();

    // Init heap, assert crash if failure
    if (memory_initHeap() != 0)
        return;

    // Then init our code that needs to be done before the app begins

    // Inits the TV format based on the device this is plugged into
    tvInit();

    // Initialize the controllers
    controllerInit();

    // Init stage manager which inits both the manager and loads up a stage
    stageManagerInit();

    // Load ascii-specific font tile ids into memory associated with
    // the ascii character they represent
    // UTF-8 tiles have to be accessed manually
    // This allows spedy lookup of string characters and a fast conversion
    fontTilemapConvCacheInit();

    // Now finish initing the rom

    // Tell NuSys to call this function on every vsync (I'm presuming "frame" or when the scanline
    // is in the vblank area on TV output before it moves back to the top)
    nuGfxFuncSet((NUGfxFunc)onVSync);

    // Register call-back function for Pre-NMI Event (Reset Button)
    nuPreNMIFuncSet((NUScPreNMIFunc)onPreNmi);

    // Turn on the screen
    nuGfxDisplayOn();

    // Loop forever to keep the idle thread busy
    while (1)
        ;
}
