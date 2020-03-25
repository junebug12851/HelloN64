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

/*=================================
              Main
=================================*/

void mainproc(void* dummy)
{
    // Init NuSys GFX First

    // Initialize and activate the graphics thread and Graphics Task Manager.
    // Sort of completes booting of the NuSys N64 OS
    nuGfxInit();

    // Then init our code that needs to be done before the app begins

    // Inits the TV format being compiled for
    tvInit();

    // Initialize the controllers
    controllerInit();

    // Init stage manager which inits both the manager and loads up a stage
    stageManagerInit();

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
