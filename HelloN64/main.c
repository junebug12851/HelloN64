/*========================================================
                Initialize the game
========================================================*/
/*=================================
            Libraries
=================================*/

#include <nusys.h>
#include "stageManager.h"
#include "hack_tvFormat.h"
#include "onEvents.h"
#include "controllerState.h"

/*=================================
              Main
=================================*/

void mainproc(void* dummy)
{
    // Register call-back function for Pre-NMI Event (Reset Button)
    nuPreNMIFuncSet((NUScPreNMIFunc)onPreNmi);

    // This imposes a hack to fix it to work correctly for the TV Format being compiled for
    hack_tvFix();

    // Initialize and activate the graphics thread and Graphics Task Manager.
    // Sort of completes booting of the NuSys N64 OS
    nuGfxInit();

    // Initialize the controllers
    controller_init();

    // Tell NuSys to call this function on every vsync (I'm presuming "frame" or when the scanline
    // is in the vblank area on TV output before it moves back to the top)
    nuGfxFuncSet((NUGfxFunc)onVSync);

    // Initialize the default stage on boot
    stage_init();

    // Turn on the screen
    nuGfxDisplayOn();

    // Loop forever to keep the idle thread busy
    while (1)
        ;
}
